#include "AssetLoaders.h"

#include "lwAnimKeySetPRS.h"
#include "lwTreeNode.h"

#include <array>
#include <format>
#include <memory>

using namespace LW_NAMESPACE;

namespace Corsairs::Engine::Render {

namespace {

// Только данные — никаких функций. Согласно правилу проекта вся логика чтения/
// записи .lgo и .lmo живёт исключительно в LgoLoader; namespace-level helpers
// больше не допускаются.

// Количество DWORD'ов размеров блоков anim_data в файле .lgo: bone + mat
// + opacity (LW_MAX_SUBSET_NUM) + texuv (subset×stage) + teximg (subset×stage).
// Версии < 1.0.0.5 пишут на LW_MAX_SUBSET_NUM меньше (без opacity-блока).
constexpr std::uint32_t kAnimDataInfoSizeFieldsCount =
    2u + LW_MAX_SUBSET_NUM + LW_MAX_SUBSET_NUM * LW_MAX_TEXTURESTAGE_NUM * 2u;

// Версии .lgo, которые умеет читать LoadFromStream. Любая другая
// — повреждённый или нестандартный файл.
constexpr std::array kKnownVersions = {
    EXP_OBJ_VERSION_0_0_0_0,
    EXP_OBJ_VERSION_1_0_0_0,
    EXP_OBJ_VERSION_1_0_0_1,
    EXP_OBJ_VERSION_1_0_0_2,
    EXP_OBJ_VERSION_1_0_0_3,
    EXP_OBJ_VERSION_1_0_0_4,
    EXP_OBJ_VERSION_1_0_0_5,
};

// RAII для std::FILE*: гарантирует fclose даже на ранних `return LW_RET_FAILED`
// после макросов LGO_FREAD_*. Используется в функциях, которые сами открывают файл
// (LoadEx, Save, LoadModelObj, SaveModelObj, GetModelObjHeader, LoadModel, SaveModel).
struct FileCloser {
    void operator()(std::FILE* fp) const noexcept {
        if (fp != nullptr) {
            std::fclose(fp);
        }
    }
};
using UniqueFile = std::unique_ptr<std::FILE, FileCloser>;

} // namespace

// Проверка `fread`. Раскрывается в `if (fread(...) != count) { лог + return LW_RET_FAILED; }`.
// Используется внутри методов LgoLoader, возвращающих LW_RESULT. Параметр `desc` —
// человекочитаемое описание читаемого блока, идёт в errors-лог.
#define LGO_FREAD_OR_RET(fp, ptr, sz, cnt, desc)                                          \
    do {                                                                                  \
        const std::size_t _lgo_n_ = (cnt);                                                \
        if (std::fread((ptr), (sz), _lgo_n_, (fp)) != _lgo_n_) {                          \
            ToLogService("errors", LogLevel::Error,                                       \
                         "[{}:{}] short fread: {} (expected {} bytes)",                   \
                         __FUNCTION__, __LINE__, (desc), (sz) * _lgo_n_);                 \
            return LW_RET_FAILED;                                                         \
        }                                                                                 \
    } while (0)

// =============================================================================
// Утилиты
// =============================================================================

void LgoLoader::ThrowSuspiciousAlloc(const char* typeName,
                                     std::size_t count,
                                     std::size_t elemSize,
                                     std::size_t totalBytes) {
    const std::string message = std::format(
        "[LgoLoader] suspicious array allocation: {}[{}] = {} bytes "
        "(elem={}, limit={}). Likely corrupt num field in .lgo.",
        (typeName != nullptr ? typeName : "?"),
        count, totalBytes, elemSize, kMaxArrayBytes);

    ToLogService("errors", LogLevel::Error, "{}", message);

    throw std::length_error(message);
}

bool LgoLoader::IsKnownVersion(DWORD v) {
    for (const DWORD k : kKnownVersions) {
        if (k == v) {
            return true;
        }
    }
    return false;
}

std::int64_t LgoLoader::FileSize(std::FILE* fp) {
    if (std::fseek(fp, 0, SEEK_END) != 0) {
        return -1;
    }
    const long sz = std::ftell(fp);
    if (std::fseek(fp, 0, SEEK_SET) != 0) {
        return -1;
    }
    return static_cast<std::int64_t>(sz);
}

void LgoLoader::LogLoadFailure(std::string_view file, const LgoLoadDiagnostics& diag) {
    ToLogService("errors", LogLevel::Error,
                 "[LgoLoader] {} ({}): file={}, version=0x{:08X}",
                 ToString(diag.status), diag.detail,
                 (file.empty() ? std::string_view{"(null)"} : file),
                 diag.version);
}

// =============================================================================
// .lgo: lwGeomObjInfo Load/Save (на FILE* и на путь)
// =============================================================================

LW_RESULT LgoLoader::LoadFromStream(lwGeomObjInfo* info, std::FILE* fp, DWORD version) {
    LGO_FREAD_OR_RET(fp, (lwGeomObjInfoHeader*)&info->id, sizeof(lwGeomObjInfoHeader), 1,
                     "lwGeomObjInfoHeader");

    if (info->mtl_size > 100000) {
        return LW_RET_FAILED;
    }

    if (info->mtl_size > 0) {
        if (LW_RESULT r = LoadMtlTexInfo(info, fp, version); LW_FAILED(r)) {
            ToLogService("errors", LogLevel::Error,
                         "[{}] LoadMtlTexInfo failed: ret={}",
                         __FUNCTION__, static_cast<long long>(r));
            return r;
        }
    }

    if (info->mesh_size > 0) {
        if (LW_RESULT r = LoadMeshInfo(info, fp, version); LW_FAILED(r)) {
            ToLogService("errors", LogLevel::Error,
                         "[{}] LoadMeshInfo failed: ret={}",
                         __FUNCTION__, static_cast<long long>(r));
            return r;
        }
    }

    if (info->helper_size > 0) {
        if (LW_RESULT r = LoadHelperInfo(info->helper_data, fp, version); LW_FAILED(r)) {
            ToLogService("errors", LogLevel::Error,
                         "[{}] LoadHelperInfo failed: ret={}",
                         __FUNCTION__, static_cast<long long>(r));
            return r;
        }
    }

    if (info->anim_size > 0) {
        if (LW_RESULT r = LoadAnimDataInfo(info->anim_data, fp, version); LW_FAILED(r)) {
            ToLogService("errors", LogLevel::Error,
                         "[{}] LoadAnimDataInfo failed: ret={}",
                         __FUNCTION__, static_cast<long long>(r));
            return r;
        }
    }

    return LW_RET_OK;
}

LW_RESULT LgoLoader::SaveToStream(lwGeomObjInfo* info, std::FILE* fp) {
    // Пересчитать размеры блоков ровно по фактическим данным в info, ДО записи
    // header'а. Иначе при round-trip Load→Save для файла, у которого исходный
    // header содержал «trailer» (header.*_size меньше реально записанных байт),
    // после save в файле снова окажется trailing data — потому что header
    // объявляет малый размер, а Save*Info пишут реальные блоки. Это и было
    // причиной повторных Warning-OkWithTrailingData на тех же 434 файлах.
    info->mtl_size    = GetMtlTexInfoSize(info);
    info->mesh_size   = GetMeshInfoSize(info);
    info->helper_size = GetHelperInfoSize(info->helper_data);
    info->anim_size   = GetAnimDataInfoSize(info->anim_data);

    fwrite((lwGeomObjInfoHeader*)&info->id, sizeof(lwGeomObjInfoHeader), 1, fp);

    if (info->mtl_size > 0) {
        SaveMtlTexInfo(info, fp);
    }

    if (info->mesh_size > 0) {
        SaveMeshInfo(info, fp);
    }

    if (info->helper_size > 0) {
        if (LW_RESULT r = SaveHelperInfo(info->helper_data, fp); LW_FAILED(r)) {
            ToLogService("errors", LogLevel::Error,
                         "[{}] SaveHelperInfo failed: ret={}",
                         __FUNCTION__, static_cast<long long>(r));
        }
    }

    if (info->anim_size > 0) {
        if (LW_RESULT r = SaveAnimDataInfo(info->anim_data, fp); LW_FAILED(r)) {
            ToLogService("errors", LogLevel::Error,
                         "[{}] SaveAnimDataInfo failed: ret={}",
                         __FUNCTION__, static_cast<long long>(r));
        }
    }

    return LW_RET_OK;
}

lwGeomObjInfo* LgoLoader::LoadEx(std::string_view file, LgoLoadDiagnostics& diag) {
    diag = {};

    // Трассировка каждого файла, прошедшего через парсер. Полный путь — чтобы
    // в логе можно было сопоставить с последующими warning/error-записями.
    ToLogService("loader", LogLevel::Trace,
                 "[LgoLoader] parsing file={}",
                 (file.empty() ? std::string_view{"(null)"} : file));

    UniqueFile fp{std::fopen(std::string{file}.c_str(), "rb")};
    if (!fp) {
        diag.status = LgoLoadStatus::FileOpenFailed;
        diag.detail = "fopen failed";
        LogLoadFailure(file, diag);
        return nullptr;
    }

    const std::int64_t fileSize = FileSize(fp.get());

    DWORD version = 0;
    if (std::fread(&version, sizeof(version), 1, fp.get()) != 1) {
        diag.status = LgoLoadStatus::VersionTruncated;
        diag.detail = "short read of version DWORD (file empty or truncated)";
        LogLoadFailure(file, diag);
        return nullptr;
    }
    diag.version = version;

    if (!IsKnownVersion(version)) {
        diag.status = LgoLoadStatus::VersionUnknown;
        diag.detail = std::format("version=0x{:08X} (expected 0x0000 or 0x1000..0x1005)", version);
        LogLoadFailure(file, diag);
        return nullptr;
    }

    auto* info = new lwGeomObjInfo;
    const LW_RESULT ret = LoadFromStream(info, fp.get(), version);

    // Сохраняем фактическое смещение файлового курсора после парсинга — для
    // sanity-check'а «дочитали ли весь файл». fp ещё не закрыт.
    const std::int64_t consumed = static_cast<std::int64_t>(std::ftell(fp.get()));

    if (LW_FAILED(ret)) {
        diag.status = LgoLoadStatus::ParseFailed;
        diag.detail = std::format("LoadFromStream returned ret={}", static_cast<long long>(ret));
        delete info;
        LogLoadFailure(file, diag);
        return nullptr;
    }

    // Sanity-check на согласованность header.{mtl,mesh,helper,anim}_size с
    // реальным размером файла.
    //
    //   expected_total = 4 (version) + sizeof(lwGeomObjInfoHeader) + sum(*_size)
    //
    //   - expected_total > file_size : loader попытается читать за пределами
    //     файла, в info попадёт мусор. Жёсткий fail.
    //   - expected_total == file_size : идеальная картина, ничего не делаем.
    //   - expected_total < file_size : у файла есть «трейлер» после задеклари-
    //     рованных блоков, которым наш парсер не пользуется. Игра десятки лет
    //     с такими файлами живёт (LoadFromStream останавливается на границе
    //     заявленных блоков, остаток просто игнорируется), поэтому fail-ить
    //     нельзя — пишем warning и возвращаем info.
    if (fileSize >= 0) {
        const std::int64_t sumBlocks =
            static_cast<std::int64_t>(info->mtl_size) +
            static_cast<std::int64_t>(info->mesh_size) +
            static_cast<std::int64_t>(info->helper_size) +
            static_cast<std::int64_t>(info->anim_size);
        const std::int64_t expectedTotal = sumBlocks + 4 + static_cast<std::int64_t>(sizeof(lwGeomObjInfoHeader));

        if (expectedTotal > fileSize) {
            diag.status = LgoLoadStatus::BlockSizesInconsistent;
            diag.detail = std::format(
                "header.mtl={}+mesh={}+helper={}+anim={}={} bytes; expected total={}; file={} bytes (overflow=+{})",
                info->mtl_size, info->mesh_size, info->helper_size, info->anim_size,
                sumBlocks, expectedTotal, fileSize, expectedTotal - fileSize);
            delete info;
            LogLoadFailure(file, diag);
            return nullptr;
        }

        if (expectedTotal < fileSize || consumed < fileSize) {
            const std::int64_t trailer = fileSize - expectedTotal;
            // Warning-уровень в отдельный канал, чтобы errors.log содержал
            // только реальные failures.
            ToLogService("warnings", LogLevel::Warning,
                         "[LgoLoader] trailing data ignored: parsed {} bytes, "
                         "header expected {} bytes, file {} bytes (trailer={} bytes) — file={}, version=0x{:08X}",
                         consumed, expectedTotal, fileSize, trailer,
                         (file.empty() ? std::string_view{"(null)"} : file),
                         version);
            diag.status = LgoLoadStatus::OkWithTrailingData;
            diag.detail = std::format(
                "trailer={} bytes (parsed {} of {})",
                trailer, consumed, fileSize);
            return info;
        }
    }

    diag.status = LgoLoadStatus::Ok;
    return info;
}

lwGeomObjInfo* LgoLoader::Load(std::string_view file) {
    LgoLoadDiagnostics diag;
    return LoadEx(file, diag);
}

LW_RESULT LgoLoader::Save(lwGeomObjInfo* info, std::string_view file) {
    UniqueFile fp{std::fopen(std::string{file}.c_str(), "wb")};
    if (!fp) {
        return LW_RET_FAILED;
    }

    DWORD version = EXP_OBJ_VERSION;
    fwrite(&version, sizeof(version), 1, fp.get());

    return SaveToStream(info, fp.get());
}

void LgoLoader::ApplyRuntimeDefaults(lwGeomObjInfo* info) {
    if (info == nullptr) {
        return;
    }

    // 1) Runtime-инварианты для всего объекта.
    info->state_ctrl.SetState(STATE_FRAMECULLING, 0);
    info->state_ctrl.SetState(STATE_UPDATETRANSPSTATE, 1);

    // 2) Per-material патчи (раньше жили в `lwMtlTexInfo_Load`, мешали
    //    round-trip Load→Save).
    for (DWORD m = 0; m < info->mtl_num; ++m) {
        lwMtlTexInfo* mt = &info->mtl_seq[m];

        // Mipmap pool/level для первой текстуры.
        mt->tex_seq[0].pool = D3DPOOL_MANAGED;
        mt->tex_seq[0].level = D3DX_DEFAULT;

        // Если у материала включён аддитивный/инвертный DESTBLEND, но нет явного
        // отключения освещения — дописать D3DRS_LIGHTING=0 в первый свободный слот.
        BOOL transp_flag = 0;
        DWORD i = 0;
        for (; i < LW_MTL_RS_NUM; ++i) {
            lwRenderStateAtom* rsa = &mt->rs_set[i];

            if (rsa->state == LW_INVALID_INDEX) {
                break;
            }

            if (rsa->state == D3DRS_DESTBLEND
                && (rsa->value0 == D3DBLEND_ONE || rsa->value0 == D3DBLEND_INVSRCCOLOR)) {
                transp_flag = 1;
            }
            if (rsa->state == D3DRS_LIGHTING && rsa->value0 == FALSE) {
                transp_flag += 1;
            }
        }
        if (transp_flag == 1 && i < (LW_MTL_RS_NUM - 1)) {
            RSA_VALUE(&mt->rs_set[i], D3DRS_LIGHTING, 0);
        }

        // Legacy-апгрейд значений transp_type из старых файлов.
        if (mt->transp_type == 1) {
            mt->transp_type = MTLTEX_TRANSP_ADDITIVE;
        }
        else if (mt->transp_type == 2) {
            mt->transp_type = MTLTEX_TRANSP_SUBTRACTIVE;
        }
    }
}

// =============================================================================
// mtl_seq
// =============================================================================

LW_RESULT LgoLoader::LoadMtlTexInfo(lwGeomObjInfo* info, std::FILE* fp, DWORD version) {
    if (version == EXP_OBJ_VERSION_0_0_0_0) {
        DWORD old_version = 0;
        LGO_FREAD_OR_RET(fp, &old_version, sizeof(old_version), 1, "legacy old_version DWORD");
        version = old_version;
    }

    DWORD num = 0;
    LGO_FREAD_OR_RET(fp, &num, sizeof(num), 1, "mtl num");

    lwMtlTexInfo* buf = LGO_NEW_ARRAY(lwMtlTexInfo, num);

    for (DWORD i = 0; i < num; i++) {
        LoadMtlTexInfoSingle(buf[i], fp, version);
    }

    info->mtl_seq = buf;
    info->mtl_num = num;
    return LW_RET_OK;
}

LW_RESULT LgoLoader::SaveMtlTexInfo(const lwGeomObjInfo* info, std::FILE* fp) {
    const DWORD num = info->mtl_num;
    fwrite(&num, sizeof(DWORD), 1, fp);

    for (DWORD i = 0; i < num; i++) {
        SaveMtlTexInfoSingle(info->mtl_seq[i], fp, MTLTEX_VERSION);
    }
    return LW_RET_OK;
}

DWORD LgoLoader::GetMtlTexInfoSize(const lwGeomObjInfo* info) {
    DWORD size = 0;
    for (DWORD i = 0; i < info->mtl_num; i++) {
        size += lwMtlTexInfo_GetDataSize(const_cast<lwMtlTexInfo*>(&info->mtl_seq[i]));
    }
    if (size > 0) {
        size += sizeof(DWORD); // number
    }
    return size;
}

// =============================================================================
// mesh
// =============================================================================

LW_RESULT LgoLoader::LoadMeshInfo(lwGeomObjInfo* info, std::FILE* fp, DWORD version) {
    lwMeshInfo* mesh = &info->mesh;

    if (version == EXP_OBJ_VERSION_0_0_0_0) {
        DWORD old_version = 0;
        LGO_FREAD_OR_RET(fp, &old_version, sizeof(old_version), 1, "legacy old_version DWORD");
        version = old_version;
    }

    // header
    if (version >= EXP_OBJ_VERSION_1_0_0_4) {
        LGO_FREAD_OR_RET(fp, &mesh->header, sizeof(mesh->header), 1, "lwMeshInfoHeader");
    }
    else if (version >= EXP_OBJ_VERSION_1_0_0_3) {
        lwMeshInfo_0003::lwMeshInfoHeader header;
        LGO_FREAD_OR_RET(fp, &header, sizeof(header), 1, "lwMeshInfo_0003::Header");
        mesh->fvf = header.fvf;
        mesh->pt_type = header.pt_type;
        mesh->vertex_num = header.vertex_num;
        mesh->index_num = header.index_num;
        mesh->subset_num = header.subset_num;
        mesh->bone_index_num = header.bone_index_num;
        mesh->bone_infl_factor = mesh->bone_index_num > 0 ? 2 : 0;
        mesh->vertex_element_num = 0;
    }
    else if ((version >= EXP_OBJ_VERSION_1_0_0_0) || (version == MESH_VERSION0001)) {
        lwMeshInfo_0003::lwMeshInfoHeader header;
        LGO_FREAD_OR_RET(fp, &header, sizeof(header), 1, "lwMeshInfo_0003::Header (1.0.0.0/0001)");
        mesh->fvf = header.fvf;
        mesh->pt_type = header.pt_type;
        mesh->vertex_num = header.vertex_num;
        mesh->index_num = header.index_num;
        mesh->subset_num = header.subset_num;
        mesh->bone_index_num = header.bone_index_num;
        mesh->bone_infl_factor = mesh->bone_index_num > 0 ? 2 : 0;
        mesh->vertex_element_num = 0;
    }
    else if (version == MESH_VERSION0000) {
        lwMeshInfo_0000::lwMeshInfoHeader header;
        LGO_FREAD_OR_RET(fp, &header, sizeof(header), 1, "lwMeshInfo_0000::Header");
        mesh->header.fvf = header.fvf;
        mesh->header.pt_type = header.pt_type;
        mesh->header.vertex_num = header.vertex_num;
        mesh->header.index_num = header.index_num;
        mesh->header.subset_num = header.subset_num;
        mesh->header.bone_index_num = header.bone_index_num;

        lwRenderStateValue* rsv = nullptr;
        for (DWORD j = 0; j < header.rs_set.SEQUENCE_SIZE; j++) {
            rsv = &header.rs_set.rsv_seq[0][j];
            if (rsv->state == LW_INVALID_INDEX) {
                break;
            }

            DWORD v = 0;
            switch (rsv->state) {
            case D3DRS_AMBIENTMATERIALSOURCE:
                v = D3DMCS_COLOR2;
                break;
            default:
                v = rsv->value;
            }

            mesh->header.rs_set[j].state = rsv->state;
            mesh->header.rs_set[j].value0 = v;
            mesh->header.rs_set[j].value1 = v;
        }
    }
    else {
        ToLogService("errors", LogLevel::Error,
                     "[{}] invalid mesh version: 0x{:08X}",
                     __FUNCTION__, version);
        return LW_RET_FAILED;
    }

    if (version >= EXP_OBJ_VERSION_1_0_0_4) {
        if (mesh->vertex_element_num > 0) {
            mesh->vertex_element_seq = LGO_NEW_ARRAY(D3DVERTEXELEMENTX, mesh->vertex_element_num);
            LGO_FREAD_OR_RET(fp, &mesh->vertex_element_seq[0], sizeof(D3DVERTEXELEMENTX), mesh->vertex_element_num, "vertex_element_seq[0]");
        }

        if (mesh->vertex_num > 0) {
            mesh->vertex_seq = LGO_NEW_ARRAY(lwVector3, mesh->vertex_num);
            LGO_FREAD_OR_RET(fp, &mesh->vertex_seq[0], sizeof(lwVector3), mesh->vertex_num, "vertex_seq[0]");
        }

        if (mesh->fvf & D3DFVF_NORMAL) {
            mesh->normal_seq = LGO_NEW_ARRAY(lwVector3, mesh->vertex_num);
            LGO_FREAD_OR_RET(fp, &mesh->normal_seq[0], sizeof(lwVector3), mesh->vertex_num, "normal_seq[0]");
        }

        if (mesh->fvf & D3DFVF_TEX1) {
            mesh->texcoord0_seq = LGO_NEW_ARRAY(lwVector2, mesh->vertex_num);
            LGO_FREAD_OR_RET(fp, &mesh->texcoord0_seq[0], sizeof(lwVector2), mesh->vertex_num, "texcoord0_seq[0]");
        }
        else if (mesh->fvf & D3DFVF_TEX2) {
            mesh->texcoord0_seq = LGO_NEW_ARRAY(lwVector2, mesh->vertex_num);
            mesh->texcoord1_seq = LGO_NEW_ARRAY(lwVector2, mesh->vertex_num);
            LGO_FREAD_OR_RET(fp, &mesh->texcoord0_seq[0], sizeof(lwVector2), mesh->vertex_num, "texcoord0_seq[0]");
            LGO_FREAD_OR_RET(fp, &mesh->texcoord1_seq[0], sizeof(lwVector2), mesh->vertex_num, "texcoord1_seq[0]");
        }
        else if (mesh->fvf & D3DFVF_TEX3) {
            mesh->texcoord0_seq = LGO_NEW_ARRAY(lwVector2, mesh->vertex_num);
            mesh->texcoord1_seq = LGO_NEW_ARRAY(lwVector2, mesh->vertex_num);
            mesh->texcoord2_seq = LGO_NEW_ARRAY(lwVector2, mesh->vertex_num);
            LGO_FREAD_OR_RET(fp, &mesh->texcoord0_seq[0], sizeof(lwVector2), mesh->vertex_num, "texcoord0_seq[0]");
            LGO_FREAD_OR_RET(fp, &mesh->texcoord1_seq[0], sizeof(lwVector2), mesh->vertex_num, "texcoord1_seq[0]");
            LGO_FREAD_OR_RET(fp, &mesh->texcoord2_seq[0], sizeof(lwVector2), mesh->vertex_num, "texcoord2_seq[0]");
        }
        else if (mesh->fvf & D3DFVF_TEX4) {
            mesh->texcoord0_seq = LGO_NEW_ARRAY(lwVector2, mesh->vertex_num);
            mesh->texcoord1_seq = LGO_NEW_ARRAY(lwVector2, mesh->vertex_num);
            mesh->texcoord2_seq = LGO_NEW_ARRAY(lwVector2, mesh->vertex_num);
            mesh->texcoord3_seq = LGO_NEW_ARRAY(lwVector2, mesh->vertex_num);
            LGO_FREAD_OR_RET(fp, &mesh->texcoord0_seq[0], sizeof(lwVector2), mesh->vertex_num, "texcoord0_seq[0]");
            LGO_FREAD_OR_RET(fp, &mesh->texcoord1_seq[0], sizeof(lwVector2), mesh->vertex_num, "texcoord1_seq[0]");
            LGO_FREAD_OR_RET(fp, &mesh->texcoord2_seq[0], sizeof(lwVector2), mesh->vertex_num, "texcoord2_seq[0]");
            LGO_FREAD_OR_RET(fp, &mesh->texcoord3_seq[0], sizeof(lwVector2), mesh->vertex_num, "texcoord3_seq[0]");
        }

        if (mesh->fvf & D3DFVF_DIFFUSE) {
            mesh->vercol_seq = LGO_NEW_ARRAY(DWORD, mesh->vertex_num);
            LGO_FREAD_OR_RET(fp, &mesh->vercol_seq[0], sizeof(DWORD), mesh->vertex_num, "vercol_seq[0]");
        }

        if (mesh->bone_index_num > 0) {
            mesh->blend_seq = LGO_NEW_ARRAY(lwBlendInfo, mesh->vertex_num);
            mesh->bone_index_seq = LGO_NEW_ARRAY(DWORD, mesh->bone_index_num);
            LGO_FREAD_OR_RET(fp, &mesh->blend_seq[0], sizeof(lwBlendInfo), mesh->vertex_num, "blend_seq[0]");
            LGO_FREAD_OR_RET(fp, &mesh->bone_index_seq[0], sizeof(DWORD), mesh->bone_index_num, "bone_index_seq[0]");
        }

        if (mesh->index_num > 0) {
            mesh->index_seq = LGO_NEW_ARRAY(DWORD, mesh->index_num);
            LGO_FREAD_OR_RET(fp, &mesh->index_seq[0], sizeof(DWORD), mesh->index_num, "index_seq[0]");
        }

        if (mesh->subset_num > 0) {
            mesh->subset_seq = LGO_NEW_ARRAY(lwSubsetInfo, mesh->subset_num);
            LGO_FREAD_OR_RET(fp, &mesh->subset_seq[0], sizeof(lwSubsetInfo), mesh->subset_num, "subset_seq[0]");
        }
    }
    else {
        mesh->subset_seq = LGO_NEW_ARRAY(lwSubsetInfo, mesh->subset_num);
        LGO_FREAD_OR_RET(fp, &mesh->subset_seq[0], sizeof(lwSubsetInfo), mesh->subset_num, "subset_seq[0]");

        mesh->vertex_seq = LGO_NEW_ARRAY(lwVector3, mesh->vertex_num);
        LGO_FREAD_OR_RET(fp, &mesh->vertex_seq[0], sizeof(lwVector3), mesh->vertex_num, "vertex_seq[0]");

        if (mesh->fvf & D3DFVF_NORMAL) {
            mesh->normal_seq = LGO_NEW_ARRAY(lwVector3, mesh->vertex_num);
            LGO_FREAD_OR_RET(fp, &mesh->normal_seq[0], sizeof(lwVector3), mesh->vertex_num, "normal_seq[0]");
        }

        if (mesh->fvf & D3DFVF_TEX1) {
            mesh->texcoord0_seq = LGO_NEW_ARRAY(lwVector2, mesh->vertex_num);
            LGO_FREAD_OR_RET(fp, &mesh->texcoord0_seq[0], sizeof(lwVector2), mesh->vertex_num, "texcoord0_seq[0]");
        }
        else if (mesh->fvf & D3DFVF_TEX2) {
            mesh->texcoord0_seq = LGO_NEW_ARRAY(lwVector2, mesh->vertex_num);
            mesh->texcoord1_seq = LGO_NEW_ARRAY(lwVector2, mesh->vertex_num);
            LGO_FREAD_OR_RET(fp, &mesh->texcoord0_seq[0], sizeof(lwVector2), mesh->vertex_num, "texcoord0_seq[0]");
            LGO_FREAD_OR_RET(fp, &mesh->texcoord1_seq[0], sizeof(lwVector2), mesh->vertex_num, "texcoord1_seq[0]");
        }
        else if (mesh->fvf & D3DFVF_TEX3) {
            mesh->texcoord0_seq = LGO_NEW_ARRAY(lwVector2, mesh->vertex_num);
            mesh->texcoord1_seq = LGO_NEW_ARRAY(lwVector2, mesh->vertex_num);
            mesh->texcoord2_seq = LGO_NEW_ARRAY(lwVector2, mesh->vertex_num);
            LGO_FREAD_OR_RET(fp, &mesh->texcoord0_seq[0], sizeof(lwVector2), mesh->vertex_num, "texcoord0_seq[0]");
            LGO_FREAD_OR_RET(fp, &mesh->texcoord1_seq[0], sizeof(lwVector2), mesh->vertex_num, "texcoord1_seq[0]");
            LGO_FREAD_OR_RET(fp, &mesh->texcoord2_seq[0], sizeof(lwVector2), mesh->vertex_num, "texcoord2_seq[0]");
        }
        else if (mesh->fvf & D3DFVF_TEX4) {
            mesh->texcoord0_seq = LGO_NEW_ARRAY(lwVector2, mesh->vertex_num);
            mesh->texcoord1_seq = LGO_NEW_ARRAY(lwVector2, mesh->vertex_num);
            mesh->texcoord2_seq = LGO_NEW_ARRAY(lwVector2, mesh->vertex_num);
            mesh->texcoord3_seq = LGO_NEW_ARRAY(lwVector2, mesh->vertex_num);
            LGO_FREAD_OR_RET(fp, &mesh->texcoord0_seq[0], sizeof(lwVector2), mesh->vertex_num, "texcoord0_seq[0]");
            LGO_FREAD_OR_RET(fp, &mesh->texcoord1_seq[0], sizeof(lwVector2), mesh->vertex_num, "texcoord1_seq[0]");
            LGO_FREAD_OR_RET(fp, &mesh->texcoord2_seq[0], sizeof(lwVector2), mesh->vertex_num, "texcoord2_seq[0]");
            LGO_FREAD_OR_RET(fp, &mesh->texcoord3_seq[0], sizeof(lwVector2), mesh->vertex_num, "texcoord3_seq[0]");
        }

        if (mesh->fvf & D3DFVF_DIFFUSE) {
            mesh->vercol_seq = LGO_NEW_ARRAY(DWORD, mesh->vertex_num);
            LGO_FREAD_OR_RET(fp, &mesh->vercol_seq[0], sizeof(DWORD), mesh->vertex_num, "vercol_seq[0]");
        }

        if (mesh->fvf & D3DFVF_LASTBETA_UBYTE4) {
            mesh->blend_seq = LGO_NEW_ARRAY(lwBlendInfo, mesh->vertex_num);
            // old version uses BYTE
            BYTE* byte_index_seq = LGO_NEW_ARRAY(BYTE, mesh->bone_index_num);

            LGO_FREAD_OR_RET(fp, &mesh->blend_seq[0], sizeof(lwBlendInfo), mesh->vertex_num, "blend_seq[0]");
            LGO_FREAD_OR_RET(fp, &byte_index_seq[0], sizeof(BYTE), mesh->bone_index_num, "byte_index_seq[0]");

            mesh->bone_index_seq = LGO_NEW_ARRAY(DWORD, mesh->bone_index_num);
            for (DWORD i = 0; i < mesh->bone_index_num; i++) {
                mesh->bone_index_seq[i] = byte_index_seq[i];
            }

            LW_DELETE_A(byte_index_seq);
        }

        if (mesh->index_num > 0) {
            mesh->index_seq = LGO_NEW_ARRAY(DWORD, mesh->index_num);
            LGO_FREAD_OR_RET(fp, &mesh->index_seq[0], sizeof(DWORD), mesh->index_num, "index_seq[0]");
        }
    }

    return LW_RET_OK;
}

LW_RESULT LgoLoader::SaveMeshInfo(const lwGeomObjInfo* info, std::FILE* fp) {
    const lwMeshInfo* mesh = &info->mesh;

    fwrite(&mesh->header, sizeof(mesh->header), 1, fp);

    if (mesh->vertex_element_num > 0) {
        fwrite(&mesh->vertex_element_seq[0], sizeof(D3DVERTEXELEMENTX), mesh->vertex_element_num, fp);
    }

    fwrite(&mesh->vertex_seq[0], sizeof(lwVector3), mesh->vertex_num, fp);

    if (mesh->fvf & D3DFVF_NORMAL) {
        fwrite(&mesh->normal_seq[0], sizeof(lwVector3), mesh->vertex_num, fp);
    }

    if (mesh->fvf & D3DFVF_TEX1) {
        fwrite(&mesh->texcoord0_seq[0], sizeof(lwVector2), mesh->vertex_num, fp);
    }
    else if (mesh->fvf & D3DFVF_TEX2) {
        fwrite(&mesh->texcoord0_seq[0], sizeof(lwVector2), mesh->vertex_num, fp);
        fwrite(&mesh->texcoord1_seq[0], sizeof(lwVector2), mesh->vertex_num, fp);
    }
    else if (mesh->fvf & D3DFVF_TEX3) {
        fwrite(&mesh->texcoord0_seq[0], sizeof(lwVector2), mesh->vertex_num, fp);
        fwrite(&mesh->texcoord1_seq[0], sizeof(lwVector2), mesh->vertex_num, fp);
        fwrite(&mesh->texcoord2_seq[0], sizeof(lwVector2), mesh->vertex_num, fp);
    }
    else if (mesh->fvf & D3DFVF_TEX4) {
        fwrite(&mesh->texcoord0_seq[0], sizeof(lwVector2), mesh->vertex_num, fp);
        fwrite(&mesh->texcoord1_seq[0], sizeof(lwVector2), mesh->vertex_num, fp);
        fwrite(&mesh->texcoord2_seq[0], sizeof(lwVector2), mesh->vertex_num, fp);
        fwrite(&mesh->texcoord3_seq[0], sizeof(lwVector2), mesh->vertex_num, fp);
    }

    if (mesh->fvf & D3DFVF_DIFFUSE) {
        fwrite(&mesh->vercol_seq[0], sizeof(DWORD), mesh->vertex_num, fp);
    }

    if (mesh->bone_index_num > 0) {
        fwrite(&mesh->blend_seq[0], sizeof(lwBlendInfo), mesh->vertex_num, fp);
        fwrite(&mesh->bone_index_seq[0], sizeof(DWORD), mesh->bone_index_num, fp);
    }

    if (mesh->index_num > 0) {
        fwrite(&mesh->index_seq[0], sizeof(DWORD), mesh->index_num, fp);
    }

    fwrite(&mesh->subset_seq[0], sizeof(lwSubsetInfo), mesh->subset_num, fp);

    return LW_RET_OK;
}

DWORD LgoLoader::GetMeshInfoSize(const lwGeomObjInfo* info) {
    const lwMeshInfo* mesh = &info->mesh;

    DWORD size = 0;
    size += sizeof(lwMeshInfoHeader);
    size += sizeof(D3DVERTEXELEMENTX) * mesh->vertex_element_num;
    size += sizeof(lwSubsetInfo) * mesh->subset_num;
    size += sizeof(lwVector3) * mesh->vertex_num;

    if (mesh->fvf & D3DFVF_NORMAL) {
        size += sizeof(lwVector3) * mesh->vertex_num;
    }

    if (mesh->fvf & D3DFVF_TEX1) {
        size += sizeof(lwVector2) * mesh->vertex_num;
    }
    else if (mesh->fvf & D3DFVF_TEX2) {
        size += sizeof(lwVector2) * mesh->vertex_num * 2;
    }
    else if (mesh->fvf & D3DFVF_TEX3) {
        size += sizeof(lwVector2) * mesh->vertex_num * 3;
    }
    else if (mesh->fvf & D3DFVF_TEX4) {
        size += sizeof(lwVector2) * mesh->vertex_num * 4;
    }

    if (mesh->fvf & D3DFVF_DIFFUSE) {
        size += sizeof(DWORD) * mesh->vertex_num;
    }

    if (mesh->fvf & D3DFVF_LASTBETA_UBYTE4) {
        size += sizeof(lwBlendInfo) * mesh->vertex_num;
        size += sizeof(DWORD) * mesh->bone_index_num;
    }

    if (mesh->index_num > 0) {
        size += sizeof(DWORD) * mesh->index_num;
    }

    return size;
}

// =============================================================================
// helper
// =============================================================================

LW_RESULT LgoLoader::LoadHelperInfo(lwHelperInfo& info, std::FILE* fp, DWORD version) {
    if (version == EXP_OBJ_VERSION_0_0_0_0) {
        DWORD old_version = 0;
        if (std::fread(&old_version, sizeof(old_version), 1, fp) != 1) {
            ToLogService("errors", LogLevel::Error,
                         "[{}] short read of legacy old_version DWORD",
                         __FUNCTION__);
            return LW_RET_FAILED;
        }
    }

    if (std::fread(&info.type, sizeof(info.type), 1, fp) != 1) {
        ToLogService("errors", LogLevel::Error,
                     "[{}] short read of helper type",
                     __FUNCTION__);
        return LW_RET_FAILED;
    }

    if (info.type & HELPER_TYPE_DUMMY) {
        if (LW_RESULT r = LoadHelperDummySection(info, fp, version); LW_FAILED(r)) {
            return r;
        }
    }
    if (info.type & HELPER_TYPE_BOX) {
        if (LW_RESULT r = LoadHelperBoxSection(info, fp, version); LW_FAILED(r)) {
            return r;
        }
    }
    if (info.type & HELPER_TYPE_MESH) {
        if (LW_RESULT r = LoadHelperMeshSection(info, fp, version); LW_FAILED(r)) {
            return r;
        }
    }
    if (info.type & HELPER_TYPE_BOUNDINGBOX) {
        if (LW_RESULT r = LoadBoundingBoxSection(info, fp, version); LW_FAILED(r)) {
            return r;
        }
    }
    if (info.type & HELPER_TYPE_BOUNDINGSPHERE) {
        if (LW_RESULT r = LoadBoundingSphereSection(info, fp, version); LW_FAILED(r)) {
            return r;
        }
    }

    return LW_RET_OK;
}

LW_RESULT LgoLoader::SaveHelperInfo(const lwHelperInfo& info, std::FILE* fp) {
    fwrite(&info.type, sizeof(info.type), 1, fp);

    if (info.type & HELPER_TYPE_DUMMY) {
        SaveHelperDummySection(info, fp);
    }
    if (info.type & HELPER_TYPE_BOX) {
        SaveHelperBoxSection(info, fp);
    }
    if (info.type & HELPER_TYPE_MESH) {
        SaveHelperMeshSection(info, fp);
    }
    if (info.type & HELPER_TYPE_BOUNDINGBOX) {
        SaveBoundingBoxSection(info, fp);
    }
    if (info.type & HELPER_TYPE_BOUNDINGSPHERE) {
        SaveBoundingSphereSection(info, fp);
    }

    return LW_RET_OK;
}

DWORD LgoLoader::GetHelperInfoSize(const lwHelperInfo& info) {
    DWORD size = 0;

    if (info.type & HELPER_TYPE_DUMMY) {
        size += sizeof(info.dummy_num);
        size += sizeof(lwHelperDummyInfo) * info.dummy_num;
    }

    if (info.type & HELPER_TYPE_BOX) {
        size += sizeof(info.box_num);
        for (DWORD i = 0; i < info.box_num; i++) {
            size += lwGetHelperBoxInfoSize(&info.box_seq[i]);
        }
    }

    if (info.type & HELPER_TYPE_MESH) {
        size += sizeof(info.mesh_num);
        for (DWORD i = 0; i < info.mesh_num; i++) {
            size += lwGetHelperMeshInfoSize(&info.mesh_seq[i]);
        }
    }

    if (info.type & HELPER_TYPE_BOUNDINGBOX) {
        size += sizeof(info.bbox_num);
        size += sizeof(lwBoundingBoxInfo) * info.bbox_num;
    }

    if (info.type & HELPER_TYPE_BOUNDINGSPHERE) {
        size += sizeof(info.bsphere_num);
        size += sizeof(lwBoundingSphereInfo) * info.bsphere_num;
    }

    if (size > 0) {
        size += sizeof(info.type);
    }

    return size;
}

LW_RESULT LgoLoader::LoadHelperDummySection(lwHelperInfo& info, std::FILE* fp, DWORD version) {
    if (version >= EXP_OBJ_VERSION_1_0_0_1) {
        LGO_FREAD_OR_RET(fp, &info.dummy_num, sizeof(info.dummy_num), 1, "info.dummy_num");
        info.dummy_seq = LGO_NEW_ARRAY(lwHelperDummyInfo, info.dummy_num);
        LGO_FREAD_OR_RET(fp, &info.dummy_seq[0], sizeof(lwHelperDummyInfo), info.dummy_num, "info.dummy_seq[0]");
    }
    else if (version <= EXP_OBJ_VERSION_1_0_0_0) {
        LGO_FREAD_OR_RET(fp, &info.dummy_num, sizeof(info.dummy_num), 1, "info.dummy_num");
        lwHelperDummyInfo_1000* old_s = LGO_NEW_ARRAY(lwHelperDummyInfo_1000, info.dummy_num);
        LGO_FREAD_OR_RET(fp, &old_s[0], sizeof(lwHelperDummyInfo_1000), info.dummy_num, "old_s[0]");

        info.dummy_seq = LGO_NEW_ARRAY(lwHelperDummyInfo, info.dummy_num);
        for (DWORD i = 0; i < info.dummy_num; i++) {
            info.dummy_seq[i].id = old_s[i].id;
            info.dummy_seq[i].mat = old_s[i].mat;
            info.dummy_seq[i].parent_type = 0;
            info.dummy_seq[i].parent_id = 0;
        }

        LW_DELETE_A(old_s);
    }
    return LW_RET_OK;
}

LW_RESULT LgoLoader::LoadHelperBoxSection(lwHelperInfo& info, std::FILE* fp, DWORD version) {
    LGO_FREAD_OR_RET(fp, &info.box_num, sizeof(info.box_num), 1, "info.box_num");

    info.box_seq = LGO_NEW_ARRAY(lwHelperBoxInfo, info.box_num);
    LGO_FREAD_OR_RET(fp, &info.box_seq[0], sizeof(lwHelperBoxInfo), info.box_num, "info.box_seq[0]");

    if (version <= EXP_OBJ_VERSION_1_0_0_1) {
        // Старый формат хранил box как (point, size); конвертируем в (center, radius).
        // NB: оригинал использовал bbox_num вместо box_num — баг в legacy-коде,
        // но в файлах version<=1001 эти два поля обычно совпадают, поэтому
        // поведение сохраняем 1:1 для round-trip.
        lwBox_1001 old_b{};
        for (DWORD i = 0; i < info.bbox_num; i++) {
            lwBox* b = &info.box_seq[i].box;
            old_b.p = b->c;
            old_b.s = b->r;

            b->r = old_b.s / 2;
            b->c = old_b.p + b->r;
        }
    }

    return LW_RET_OK;
}

LW_RESULT LgoLoader::LoadHelperMeshSection(lwHelperInfo& info, std::FILE* fp, DWORD version) {
    LGO_FREAD_OR_RET(fp, &info.mesh_num, sizeof(info.mesh_num), 1, "info.mesh_num");

    info.mesh_seq = LGO_NEW_ARRAY(lwHelperMeshInfo, info.mesh_num);

    for (DWORD i = 0; i < info.mesh_num; i++) {
        lwHelperMeshInfo* mi = &info.mesh_seq[i];

        LGO_FREAD_OR_RET(fp, &mi->id, sizeof(mi->id), 1, "mi->id");
        LGO_FREAD_OR_RET(fp, &mi->type, sizeof(mi->type), 1, "mi->type");
        LGO_FREAD_OR_RET(fp, &mi->sub_type, sizeof(mi->sub_type), 1, "mi->sub_type");
        LGO_FREAD_OR_RET(fp, &mi->name[0], sizeof(mi->name), 1, "mi->name[0]");
        LGO_FREAD_OR_RET(fp, &mi->state, sizeof(mi->state), 1, "mi->state");
        LGO_FREAD_OR_RET(fp, &mi->mat, sizeof(mi->mat), 1, "mi->mat");
        LGO_FREAD_OR_RET(fp, &mi->box, sizeof(mi->box), 1, "mi->box");
        LGO_FREAD_OR_RET(fp, &mi->vertex_num, sizeof(mi->vertex_num), 1, "mi->vertex_num");
        LGO_FREAD_OR_RET(fp, &mi->face_num, sizeof(mi->face_num), 1, "mi->face_num");

        mi->vertex_seq = LGO_NEW_ARRAY(lwVector3, mi->vertex_num);
        mi->face_seq = LGO_NEW_ARRAY(lwHelperMeshFaceInfo, mi->face_num);

        LGO_FREAD_OR_RET(fp, &mi->vertex_seq[0], sizeof(lwVector3), mi->vertex_num, "mi->vertex_seq[0]");
        LGO_FREAD_OR_RET(fp, &mi->face_seq[0], sizeof(lwHelperMeshFaceInfo), mi->face_num, "mi->face_seq[0]");
    }

    if (version <= EXP_OBJ_VERSION_1_0_0_1) {
        lwBox_1001 old_b{};
        for (DWORD i = 0; i < info.mesh_num; i++) {
            lwBox* b = &info.mesh_seq[i].box;
            old_b.p = b->c;
            old_b.s = b->r;

            b->r = old_b.s / 2;
            b->c = old_b.p + b->r;
        }
    }

    return LW_RET_OK;
}

LW_RESULT LgoLoader::LoadBoundingBoxSection(lwHelperInfo& info, std::FILE* fp, DWORD version) {
    LGO_FREAD_OR_RET(fp, &info.bbox_num, sizeof(DWORD), 1, "info.bbox_num");

    info.bbox_seq = LGO_NEW_ARRAY(lwBoundingBoxInfo, info.bbox_num);
    LGO_FREAD_OR_RET(fp, &info.bbox_seq[0], sizeof(lwBoundingBoxInfo), info.bbox_num, "info.bbox_seq[0]");

    if (version <= EXP_OBJ_VERSION_1_0_0_1) {
        lwBox_1001 old_b{};
        for (DWORD i = 0; i < info.bbox_num; i++) {
            lwBox* b = &info.bbox_seq[i].box;
            old_b.p = b->c;
            old_b.s = b->r;

            b->r = old_b.s / 2;
            b->c = old_b.p + b->r;
        }
    }

    return LW_RET_OK;
}

LW_RESULT LgoLoader::LoadBoundingSphereSection(lwHelperInfo& info, std::FILE* fp, DWORD /*version*/) {
    LGO_FREAD_OR_RET(fp, &info.bsphere_num, sizeof(DWORD), 1, "info.bsphere_num");

    info.bsphere_seq = LGO_NEW_ARRAY(lwBoundingSphereInfo, info.bsphere_num);
    LGO_FREAD_OR_RET(fp, &info.bsphere_seq[0], sizeof(lwBoundingSphereInfo), info.bsphere_num, "info.bsphere_seq[0]");

    return LW_RET_OK;
}

LW_RESULT LgoLoader::SaveHelperDummySection(const lwHelperInfo& info, std::FILE* fp) {
    fwrite(&info.dummy_num, sizeof(info.dummy_num), 1, fp);
    fwrite(&info.dummy_seq[0], sizeof(lwHelperDummyInfo), info.dummy_num, fp);
    return LW_RET_OK;
}

LW_RESULT LgoLoader::SaveHelperBoxSection(const lwHelperInfo& info, std::FILE* fp) {
    fwrite(&info.box_num, sizeof(info.box_num), 1, fp);
    fwrite(&info.box_seq[0], sizeof(lwHelperBoxInfo), info.box_num, fp);
    return LW_RET_OK;
}

LW_RESULT LgoLoader::SaveHelperMeshSection(const lwHelperInfo& info, std::FILE* fp) {
    fwrite(&info.mesh_num, sizeof(info.mesh_num), 1, fp);

    for (DWORD i = 0; i < info.mesh_num; i++) {
        const lwHelperMeshInfo* mi = &info.mesh_seq[i];

        fwrite(&mi->id, sizeof(mi->id), 1, fp);
        fwrite(&mi->type, sizeof(mi->type), 1, fp);
        fwrite(&mi->sub_type, sizeof(mi->sub_type), 1, fp);
        fwrite(&mi->name[0], sizeof(mi->name), 1, fp);
        fwrite(&mi->state, sizeof(mi->state), 1, fp);
        fwrite(&mi->mat, sizeof(mi->mat), 1, fp);
        fwrite(&mi->box, sizeof(mi->box), 1, fp);
        fwrite(&mi->vertex_num, sizeof(mi->vertex_num), 1, fp);
        fwrite(&mi->face_num, sizeof(mi->face_num), 1, fp);

        fwrite(&mi->vertex_seq[0], sizeof(lwVector3), mi->vertex_num, fp);
        fwrite(&mi->face_seq[0], sizeof(lwHelperMeshFaceInfo), mi->face_num, fp);
    }

    return LW_RET_OK;
}

LW_RESULT LgoLoader::SaveBoundingBoxSection(const lwHelperInfo& info, std::FILE* fp) {
    fwrite(&info.bbox_num, sizeof(DWORD), 1, fp);
    fwrite(&info.bbox_seq[0], sizeof(lwBoundingBoxInfo), info.bbox_num, fp);
    return LW_RET_OK;
}

LW_RESULT LgoLoader::SaveBoundingSphereSection(const lwHelperInfo& info, std::FILE* fp) {
    fwrite(&info.bsphere_num, sizeof(DWORD), 1, fp);
    fwrite(&info.bsphere_seq[0], sizeof(lwBoundingSphereInfo), info.bsphere_num, fp);
    return LW_RET_OK;
}

// =============================================================================
// anim_data
// =============================================================================
// Перенесено из методов lwAnimDataInfo::{Load,Save,GetDataSize}: data-объект
// `lwAnimDataInfo` ничего не знает про формат файла, всё чтение/запись и
// расчёт размера — здесь.

LW_RESULT LgoLoader::LoadAnimDataInfo(lwAnimDataInfo& info, std::FILE* fp, DWORD version) {
    if (version == EXP_OBJ_VERSION_0_0_0_0) {
        DWORD old_version = 0;
        if (std::fread(&old_version, sizeof(old_version), 1, fp) != 1) {
            ToLogService("errors", LogLevel::Error,
                         "[{}] short read of legacy old_version DWORD",
                         __FUNCTION__);
            return LW_RET_FAILED;
        }
    }

    DWORD data_bone_size = 0;
    DWORD data_mat_size = 0;
    DWORD data_mtlopac_size[LW_MAX_SUBSET_NUM] = {};
    DWORD data_texuv_size[LW_MAX_SUBSET_NUM][LW_MAX_TEXTURESTAGE_NUM] = {};
    DWORD data_teximg_size[LW_MAX_SUBSET_NUM][LW_MAX_TEXTURESTAGE_NUM] = {};

    if (std::fread(&data_bone_size, sizeof(DWORD), 1, fp) != 1
        || std::fread(&data_mat_size, sizeof(DWORD), 1, fp) != 1) {
        ToLogService("errors", LogLevel::Error,
                     "[{}] short read of bone/mat sizes",
                     __FUNCTION__);
        return LW_RET_FAILED;
    }

    if (version >= EXP_OBJ_VERSION_1_0_0_5) {
        if (std::fread(&data_mtlopac_size, sizeof(data_mtlopac_size), 1, fp) != 1) {
            ToLogService("errors", LogLevel::Error,
                         "[{}] short read of data_mtlopac_size",
                         __FUNCTION__);
            return LW_RET_FAILED;
        }
    }

    if (std::fread(&data_texuv_size, sizeof(data_texuv_size), 1, fp) != 1
        || std::fread(&data_teximg_size, sizeof(data_teximg_size), 1, fp) != 1) {
        ToLogService("errors", LogLevel::Error,
                     "[{}] short read of texuv/teximg sizes",
                     __FUNCTION__);
        return LW_RET_FAILED;
    }

    if (data_bone_size > 0) {
        info.anim_bone = LW_NEW(lwAnimDataBone);
        if (LW_RESULT r = LoadAnimDataBone(*info.anim_bone, fp, version); LW_FAILED(r)) {
            ToLogService("errors", LogLevel::Error,
                         "[{}] anim_bone.Load failed: ret={}",
                         __FUNCTION__, static_cast<long long>(r));
            return LW_RET_FAILED;
        }
    }

    if (data_mat_size > 0) {
#ifdef USE_ANIMKEY_PRS
        info.anim_mat = LW_NEW(lwAnimKeySetPRS);
        LoadAnimKeySetPRS(*info.anim_mat, fp);
#else
        info.anim_mat = LW_NEW(lwAnimDataMatrix);
        if (LW_RESULT r = LoadAnimDataMatrix(*info.anim_mat, fp, version); LW_FAILED(r)) {
            ToLogService("errors", LogLevel::Error,
                         "[{}] anim_mat.Load failed: ret={}",
                         __FUNCTION__, static_cast<long long>(r));
            return LW_RET_FAILED;
        }
#endif
    }

    if (version >= EXP_OBJ_VERSION_1_0_0_5) {
        for (DWORD i = 0; i < LW_MAX_SUBSET_NUM; i++) {
            if (data_mtlopac_size[i] == 0) {
                continue;
            }
            info.anim_mtlopac[i] = LW_NEW(lwAnimDataMtlOpacity);
            if (LW_RESULT r = LoadAnimDataMtlOpacity(*info.anim_mtlopac[i], fp, version); LW_FAILED(r)) {
                ToLogService("errors", LogLevel::Error,
                             "[{}] anim_mtlopac[{}].Load failed: ret={}",
                             __FUNCTION__, i, static_cast<long long>(r));
                return LW_RET_FAILED;
            }
        }
    }

    for (DWORD i = 0; i < LW_MAX_SUBSET_NUM; i++) {
        for (DWORD j = 0; j < LW_MAX_TEXTURESTAGE_NUM; j++) {
            if (data_texuv_size[i][j] == 0) {
                continue;
            }
            info.anim_tex[i][j] = LW_NEW(lwAnimDataTexUV);
            if (LW_RESULT r = LoadAnimDataTexUV(*info.anim_tex[i][j], fp, version); LW_FAILED(r)) {
                ToLogService("errors", LogLevel::Error,
                             "[{}] anim_tex[{}][{}].Load failed: ret={}",
                             __FUNCTION__, i, j, static_cast<long long>(r));
                return LW_RET_FAILED;
            }
        }
    }

    for (DWORD i = 0; i < LW_MAX_SUBSET_NUM; i++) {
        for (DWORD j = 0; j < LW_MAX_TEXTURESTAGE_NUM; j++) {
            if (data_teximg_size[i][j] == 0) {
                continue;
            }
            info.anim_img[i][j] = LW_NEW(lwAnimDataTexImg);
            if (LW_RESULT r = LoadAnimDataTexImg(*info.anim_img[i][j], fp, version); LW_FAILED(r)) {
                ToLogService("errors", LogLevel::Error,
                             "[{}] anim_img[{}][{}].Load failed: ret={}",
                             __FUNCTION__, i, j, static_cast<long long>(r));
                return LW_RET_FAILED;
            }
        }
    }

    return LW_RET_OK;
}

LW_RESULT LgoLoader::SaveAnimDataInfo(lwAnimDataInfo& info, std::FILE* fp) {
    DWORD data_bone_size = 0;
    DWORD data_mat_size = 0;
    DWORD data_mtlopac_size[LW_MAX_SUBSET_NUM] = {};
    DWORD data_texuv_size[LW_MAX_SUBSET_NUM][LW_MAX_TEXTURESTAGE_NUM] = {};
    DWORD data_teximg_size[LW_MAX_SUBSET_NUM][LW_MAX_TEXTURESTAGE_NUM] = {};

    if (info.anim_bone != nullptr) {
        data_bone_size = info.anim_bone->GetDataSize();
    }

    if (info.anim_mat != nullptr) {
#ifdef USE_ANIMKEY_PRS
        data_mat_size = lwGetAnimKeySetPRSSize(info.anim_mat);
#else
        data_mat_size = info.anim_mat->GetDataSize();
#endif
    }

    for (DWORD i = 0; i < LW_MAX_SUBSET_NUM; i++) {
        if (info.anim_mtlopac[i] != nullptr) {
            data_mtlopac_size[i] = info.anim_mtlopac[i]->GetDataSize();
            assert(data_mtlopac_size[i] > 0);
        }
    }

    for (DWORD i = 0; i < LW_MAX_SUBSET_NUM; i++) {
        for (DWORD j = 0; j < LW_MAX_TEXTURESTAGE_NUM; j++) {
            if (info.anim_tex[i][j] != nullptr) {
                data_texuv_size[i][j] = info.anim_tex[i][j]->GetDataSize();
            }
            if (info.anim_img[i][j] != nullptr) {
                data_teximg_size[i][j] = info.anim_img[i][j]->GetDataSize();
            }
        }
    }

    fwrite(&data_bone_size, sizeof(DWORD), 1, fp);
    fwrite(&data_mat_size, sizeof(DWORD), 1, fp);
    fwrite(&data_mtlopac_size, sizeof(data_mtlopac_size), 1, fp);
    fwrite(&data_texuv_size, sizeof(data_texuv_size), 1, fp);
    fwrite(&data_teximg_size, sizeof(data_teximg_size), 1, fp);

    if (data_bone_size > 0) {
        if (LW_RESULT r = SaveAnimDataBone(*info.anim_bone, fp); LW_FAILED(r)) {
            ToLogService("errors", LogLevel::Error,
                         "[{}] anim_bone.Save failed: ret={}",
                         __FUNCTION__, static_cast<long long>(r));
        }
    }

    if (data_mat_size > 0) {
#ifdef USE_ANIMKEY_PRS
        SaveAnimKeySetPRS(*info.anim_mat, fp);
#else
        if (LW_RESULT r = SaveAnimDataMatrix(*info.anim_mat, fp); LW_FAILED(r)) {
            ToLogService("errors", LogLevel::Error,
                         "[{}] anim_mat.Save failed: ret={}",
                         __FUNCTION__, static_cast<long long>(r));
        }
#endif
    }

    for (DWORD i = 0; i < LW_MAX_SUBSET_NUM; i++) {
        if (data_mtlopac_size[i] == 0) {
            continue;
        }
        if (LW_RESULT r = SaveAnimDataMtlOpacity(*info.anim_mtlopac[i], fp); LW_FAILED(r)) {
            ToLogService("errors", LogLevel::Error,
                         "[{}] anim_mtlopac.Save failed: i={}, ret={}",
                         __FUNCTION__, i, static_cast<long long>(r));
        }
    }

    for (DWORD i = 0; i < LW_MAX_SUBSET_NUM; i++) {
        for (DWORD j = 0; j < LW_MAX_TEXTURESTAGE_NUM; j++) {
            if (data_texuv_size[i][j] == 0) {
                continue;
            }
            if (LW_RESULT r = SaveAnimDataTexUV(*info.anim_tex[i][j], fp); LW_FAILED(r)) {
                ToLogService("errors", LogLevel::Error,
                             "[{}] anim_tex.Save failed: i={}, j={}, ret={}",
                             __FUNCTION__, i, j, static_cast<long long>(r));
            }
        }
    }

    for (DWORD i = 0; i < LW_MAX_SUBSET_NUM; i++) {
        for (DWORD j = 0; j < LW_MAX_TEXTURESTAGE_NUM; j++) {
            if (data_teximg_size[i][j] == 0) {
                continue;
            }
            if (LW_RESULT r = SaveAnimDataTexImg(*info.anim_img[i][j], fp); LW_FAILED(r)) {
                ToLogService("errors", LogLevel::Error,
                             "[{}] anim_img.Save failed: i={}, j={}, ret={}",
                             __FUNCTION__, i, j, static_cast<long long>(r));
            }
        }
    }

    return LW_RET_OK;
}

DWORD LgoLoader::GetAnimDataInfoSize(const lwAnimDataInfo& info) {
    DWORD size = 0;

    if (info.anim_bone != nullptr) {
        size += info.anim_bone->GetDataSize();
    }

    if (info.anim_mat != nullptr) {
#ifdef USE_ANIMKEY_PRS
        size += lwGetAnimKeySetPRSSize(info.anim_mat);
#else
        size += info.anim_mat->GetDataSize();
#endif
    }

    for (DWORD i = 0; i < LW_MAX_SUBSET_NUM; i++) {
        if (info.anim_mtlopac[i] != nullptr) {
            size += info.anim_mtlopac[i]->GetDataSize();
        }

        for (DWORD j = 0; j < LW_MAX_TEXTURESTAGE_NUM; j++) {
            if (info.anim_tex[i][j] != nullptr) {
                size += info.anim_tex[i][j]->GetDataSize();
            }
            if (info.anim_img[i][j] != nullptr) {
                size += info.anim_img[i][j]->GetDataSize();
            }
        }
    }

    if (size > 0) {
        size += sizeof(DWORD) * kAnimDataInfoSizeFieldsCount;
    }

    return size;
}

// =============================================================================
// .lmo: lwModelObjInfo (раньше lwModelObjInfo::{Load,Save,GetDataSize,GetHeader})
// =============================================================================

LW_RESULT LgoLoader::LoadModelObj(lwModelObjInfo& info, std::string_view file) {
    UniqueFile fp{std::fopen(std::string{file}.c_str(), "rb")};
    if (!fp) {
        return LW_RET_FAILED;
    }

    DWORD version = 0;
    LGO_FREAD_OR_RET(fp.get(), &version, sizeof(version), 1, "version");

    DWORD obj_num = 0;
    LGO_FREAD_OR_RET(fp.get(), &obj_num, sizeof(DWORD), 1, "obj_num");

    lwModelObjInfo::lwModelObjInfoHeader header[LW_MAX_MODEL_OBJ_NUM];
    LGO_FREAD_OR_RET(fp.get(), &header[0], sizeof(lwModelObjInfo::lwModelObjInfoHeader), obj_num,
                     "lwModelObjInfoHeader[]");

    info.geom_obj_num = 0;

    for (DWORD i = 0; i < obj_num; i++) {
        fseek(fp.get(), header[i].addr, SEEK_SET);

        switch (header[i].type) {
        case MODEL_OBJ_TYPE_GEOMETRY:
            info.geom_obj_seq[info.geom_obj_num] = new lwGeomObjInfo();
            if (version == EXP_OBJ_VERSION_0_0_0_0) {
                DWORD old_version = 0;
                LGO_FREAD_OR_RET(fp.get(), &old_version, sizeof(old_version), 1, "old_version");
            }
            LoadFromStream(info.geom_obj_seq[info.geom_obj_num], fp.get(), version);
            // ApplyRuntimeDefaults НЕ вызываем здесь — иначе ломается round-trip
            // Load→Save (тулзы сохраняли бы рантайм-мутации). Рантайм-callers
            // (lwResBufMgr::RegisterModelObjInfo) применяют его сами.
            info.geom_obj_num += 1;
            break;
        case MODEL_OBJ_TYPE_HELPER:
            LoadHelperInfo(info.helper_data, fp.get(), version);
            break;
        default:
            assert(0);
        }
    }

    return LW_RET_OK;
}

LW_RESULT LgoLoader::SaveModelObj(lwModelObjInfo& info, std::string_view file) {
    UniqueFile fp{std::fopen(std::string{file}.c_str(), "wb")};
    if (!fp) {
        return LW_RET_FAILED;
    }

    DWORD version = EXP_OBJ_VERSION;
    fwrite(&version, sizeof(version), 1, fp.get());

    DWORD obj_num = info.geom_obj_num;
    if (info.helper_data.type != HELPER_TYPE_INVALID) {
        obj_num += 1;
    }
    fwrite(&obj_num, sizeof(DWORD), 1, fp.get());

    lwModelObjInfo::lwModelObjInfoHeader header{};
    const DWORD base_offset_size = sizeof(version) + sizeof(obj_num) + sizeof(header) * obj_num;
    DWORD total_obj_size = 0;

    // geomobj headers
    for (DWORD i = 0; i < info.geom_obj_num; i++) {
        const lwGeomObjInfo* goi = info.geom_obj_seq[i];

        header.type = MODEL_OBJ_TYPE_GEOMETRY;
        header.addr = base_offset_size + total_obj_size;
        header.size = goi->GetDataSize();

        fwrite(&header, sizeof(header), 1, fp.get());
        total_obj_size += header.size;
    }

    // helper header
    if (info.helper_data.type != HELPER_TYPE_INVALID) {
        header.addr = base_offset_size + total_obj_size;
        header.size = GetHelperInfoSize(info.helper_data);
        header.type = MODEL_OBJ_TYPE_HELPER;
        fwrite(&header, sizeof(header), 1, fp.get());
        total_obj_size += header.size;
    }

    // geometry payload
    for (DWORD i = 0; i < info.geom_obj_num; i++) {
        SaveToStream(info.geom_obj_seq[i], fp.get());
    }

    // helper payload
    if (info.helper_data.type != HELPER_TYPE_INVALID) {
        if (LW_RESULT r = SaveHelperInfo(info.helper_data, fp.get()); LW_FAILED(r)) {
            ToLogService("errors", LogLevel::Error,
                         "[{}] SaveHelperInfo failed: ret={}",
                         __FUNCTION__, static_cast<long long>(r));
        }
    }

    return LW_RET_OK;
}

DWORD LgoLoader::GetModelObjSize(const lwModelObjInfo& info) {
    DWORD size = 0;
    for (DWORD i = 0; i < info.geom_obj_num; i++) {
        size += info.geom_obj_seq[i]->GetDataSize();
    }
    size += GetHelperInfoSize(info.helper_data);
    return size;
}

LW_RESULT LgoLoader::GetModelObjHeader(lwModelObjInfo::lwModelObjInfoHeader* out_seq,
                                       DWORD* out_num,
                                       std::string_view file) {
    UniqueFile fp{std::fopen(std::string{file}.c_str(), "rb")};
    if (!fp) {
        return LW_RET_FAILED;
    }

    DWORD version = 0;
    LGO_FREAD_OR_RET(fp.get(), &version, sizeof(DWORD), 1, "version");
    LGO_FREAD_OR_RET(fp.get(), out_num, sizeof(DWORD), 1, "out_num");
    LGO_FREAD_OR_RET(fp.get(), &out_seq[0], sizeof(lwModelObjInfo::lwModelObjInfoHeader), *out_num,
                     "lwModelObjInfoHeader[]");

    return LW_RET_OK;
}

// =============================================================================
// lwHelperDummyObjInfo (вызывается из LoadModelNode/SaveModelNode для NODE_DUMMY)
// =============================================================================

LW_RESULT LgoLoader::LoadHelperDummyObj(lwHelperDummyObjInfo& info, std::FILE* fp, DWORD /*version*/) {
    LGO_FREAD_OR_RET(fp, &info._id, sizeof(info._id), 1, "info._id");
    LGO_FREAD_OR_RET(fp, &info._mat, sizeof(info._mat), 1, "info._mat");

    LW_SAFE_RELEASE(info._anim_data);

    DWORD anim_data_flag = 0;
    LGO_FREAD_OR_RET(fp, &anim_data_flag, sizeof(anim_data_flag), 1, "anim_data_flag");
    if (anim_data_flag == 1) {
        lwAnimDataMatrix* anim_data = LW_NEW(lwAnimDataMatrix);
        if (LW_RESULT r = LoadAnimDataMatrix(*anim_data, fp, 0); LW_FAILED(r)) {
            ToLogService("errors", LogLevel::Error,
                         "[{}] AnimDataMatrix::Load failed: id={}, ret={}",
                         __FUNCTION__, static_cast<long long>(info._id),
                         static_cast<long long>(r));
            LW_DELETE(anim_data);
            return LW_RET_FAILED;
        }
        info._anim_data = anim_data;
    }

    return LW_RET_OK;
}

LW_RESULT LgoLoader::SaveHelperDummyObj(lwHelperDummyObjInfo& info, std::FILE* fp) {
    fwrite(&info._id, sizeof(info._id), 1, fp);
    fwrite(&info._mat, sizeof(info._mat), 1, fp);

    DWORD anim_data_flag = info._anim_data ? 1 : 0;
    fwrite(&anim_data_flag, sizeof(anim_data_flag), 1, fp);
    if (info._anim_data) {
        if (LW_RESULT r = SaveAnimDataMatrix(*(lwAnimDataMatrix*)info._anim_data, fp); LW_FAILED(r)) {
            ToLogService("errors", LogLevel::Error,
                         "[{}] AnimDataMatrix::Save failed: id={}, ret={}",
                         __FUNCTION__, static_cast<long long>(info._id),
                         static_cast<long long>(r));
            return LW_RET_FAILED;
        }
    }

    return LW_RET_OK;
}

// =============================================================================
// lwModelNodeInfo (одна нода tree-based .lmo)
// =============================================================================

LW_RESULT LgoLoader::LoadModelNode(lwModelNodeInfo& info, std::FILE* fp, DWORD version) {
    LGO_FREAD_OR_RET(fp, &info._head, sizeof(info._head), 1, "info._head");

    if (info._type == NODE_PRIMITIVE) {
        info._data = new lwGeomObjInfo;
        if (LW_RESULT r = LoadFromStream((lwGeomObjInfo*)info._data, fp, version); LW_FAILED(r)) {
            ToLogService("errors", LogLevel::Error,
                         "[{}] LgoLoader::LoadFromStream failed: version={}, ret={}",
                         __FUNCTION__, static_cast<long long>(version),
                         static_cast<long long>(r));
            return LW_RET_FAILED;
        }
        // ApplyRuntimeDefaults НЕ вызываем здесь — ломает round-trip
        // Load→Save в тулзах. Рантайм-callers (lwNodeObject::Load) применяют
        // его, обходя дерево после LoadModel.
    }
    else if (info._type == NODE_BONECTRL) {
        info._data = LW_NEW(lwAnimDataBone);
        if (LW_RESULT r = LoadAnimDataBone(*(lwAnimDataBone*)info._data, fp, version); LW_FAILED(r)) {
            ToLogService("errors", LogLevel::Error,
                         "[{}] LgoLoader::LoadAnimDataBone failed: version={}, ret={}",
                         __FUNCTION__, static_cast<long long>(version),
                         static_cast<long long>(r));
            return LW_RET_FAILED;
        }
    }
    else if (info._type == NODE_DUMMY) {
        info._data = LW_NEW(lwHelperDummyObjInfo);
        if (LW_RESULT r = LoadHelperDummyObj(*(lwHelperDummyObjInfo*)info._data, fp, version); LW_FAILED(r)) {
            ToLogService("errors", LogLevel::Error,
                         "[{}] LgoLoader::LoadHelperDummyObj failed: version={}, ret={}",
                         __FUNCTION__, static_cast<long long>(version),
                         static_cast<long long>(r));
            return LW_RET_FAILED;
        }
    }
    else if (info._type == NODE_HELPER) {
        info._data = LW_NEW(lwHelperInfo);
        if (LW_RESULT r = LoadHelperInfo(*(lwHelperInfo*)info._data, fp, version); LW_FAILED(r)) {
            ToLogService("errors", LogLevel::Error,
                         "[{}] LgoLoader::LoadHelperInfo failed: version={}, ret={}",
                         __FUNCTION__, static_cast<long long>(version),
                         static_cast<long long>(r));
            return LW_RET_FAILED;
        }
    }
    else {
        return LW_RET_FAILED;
    }

    return LW_RET_OK;
}

LW_RESULT LgoLoader::SaveModelNode(lwModelNodeInfo& info, std::FILE* fp) {
    fwrite(&info._head, sizeof(info._head), 1, fp);

    if (info._type == NODE_PRIMITIVE) {
        if (LW_RESULT r = SaveToStream((lwGeomObjInfo*)info._data, fp); LW_FAILED(r)) {
            ToLogService("errors", LogLevel::Error,
                         "[{}] LgoLoader::SaveToStream failed: ret={}",
                         __FUNCTION__, static_cast<long long>(r));
            return LW_RET_FAILED;
        }
    }
    else if (info._type == NODE_BONECTRL) {
        if (LW_RESULT r = SaveAnimDataBone(*(lwAnimDataBone*)info._data, fp); LW_FAILED(r)) {
            ToLogService("errors", LogLevel::Error,
                         "[{}] LgoLoader::SaveAnimDataBone failed: ret={}",
                         __FUNCTION__, static_cast<long long>(r));
            return LW_RET_FAILED;
        }
    }
    else if (info._type == NODE_DUMMY) {
        if (LW_RESULT r = SaveHelperDummyObj(*(lwHelperDummyObjInfo*)info._data, fp); LW_FAILED(r)) {
            ToLogService("errors", LogLevel::Error,
                         "[{}] LgoLoader::SaveHelperDummyObj failed: ret={}",
                         __FUNCTION__, static_cast<long long>(r));
            return LW_RET_FAILED;
        }
    }
    else if (info._type == NODE_HELPER) {
        if (LW_RESULT r = SaveHelperInfo(*(lwHelperInfo*)info._data, fp); LW_FAILED(r)) {
            ToLogService("errors", LogLevel::Error,
                         "[{}] LgoLoader::SaveHelperInfo failed: ret={}",
                         __FUNCTION__, static_cast<long long>(r));
            return LW_RET_FAILED;
        }
    }

    return LW_RET_OK;
}

// =============================================================================
// .lmo (tree-based, lwModelInfo с lwITreeNode-деревом)
// =============================================================================

LW_RESULT LgoLoader::LoadModel(lwModelInfo& info, std::string_view file) {
    UniqueFile fp{std::fopen(std::string{file}.c_str(), "rb")};
    if (!fp) {
        return LW_RET_FAILED;
    }

    LGO_FREAD_OR_RET(fp.get(), &info._head, sizeof(info._head), 1, "lwModelHeadInfo");

    DWORD obj_num = 0;
    LGO_FREAD_OR_RET(fp.get(), &obj_num, sizeof(obj_num), 1, "obj_num");

    if (obj_num == 0) {
        return LW_RET_OK;
    }

    // Tree-walker callback для поиска ноды по handle. Локальная state-структура
    // лежит на стеке этой функции; лямбда захватывает её через void*-param.
    struct FindCtx {
        lwITreeNode* node;
        DWORD handle;
    };
    auto findProc = +[](lwITreeNode* node, void* param) -> DWORD {
        auto* ctx = static_cast<FindCtx*>(param);
        lwModelNodeInfo* data = (lwModelNodeInfo*)node->GetData();
        if (data->_handle == ctx->handle) {
            ctx->node = node;
            return TREENODE_PROC_RET_ABORT;
        }
        return TREENODE_PROC_RET_CONTINUE;
    };

    for (DWORD i = 0; i < obj_num; i++) {
        lwModelNodeInfo* node_info = LW_NEW(lwModelNodeInfo);
        if (LW_RESULT r = LoadModelNode(*node_info, fp.get(), info._head.version); LW_FAILED(r)) {
            ToLogService("errors", LogLevel::Error,
                         "[{}] LgoLoader::LoadModelNode failed: file='{}', i={}, version={}, ret={}",
                         __FUNCTION__, (file.empty() ? std::string_view{"(null)"} : file),
                         static_cast<long long>(i),
                         static_cast<long long>(info._head.version),
                         static_cast<long long>(r));
            LW_DELETE(node_info);
            return LW_RET_FAILED;
        }

        lwITreeNode* tree_node = LW_NEW(lwTreeNode);
        tree_node->SetData(node_info);

        if (info._obj_tree == nullptr) {
            info._obj_tree = tree_node;
        }
        else {
            FindCtx ctx{nullptr, node_info->_parent_handle};
            info._obj_tree->EnumTree(findProc, (void*)&ctx, TREENODE_PROC_PREORDER);

            if (ctx.node == nullptr) {
                return LW_RET_FAILED;
            }

            if (LW_RESULT r = ctx.node->InsertChild(0, tree_node); LW_FAILED(r)) {
                ToLogService("errors", LogLevel::Error,
                             "[{}] InsertChild failed: file='{}', i={}, parent_handle={}, ret={}",
                             __FUNCTION__, (file.empty() ? std::string_view{"(null)"} : file),
                             static_cast<long long>(i),
                             static_cast<long long>(node_info->_parent_handle),
                             static_cast<long long>(r));
                return LW_RET_FAILED;
            }
        }
    }

    return LW_RET_OK;
}

LW_RESULT LgoLoader::SaveModel(lwModelInfo& info, std::string_view file) {
    UniqueFile fp{std::fopen(std::string{file}.c_str(), "wb")};
    if (!fp) {
        return LW_RET_FAILED;
    }

    info._head.version = MODELINFO_VERSION;
    _tcscpy(info._head.decriptor, "lwModelInfo");
    fwrite(&info._head, sizeof(info._head), 1, fp.get());

    DWORD obj_num = info._obj_tree ? info._obj_tree->GetNodeNum() : 0;
    fwrite(&obj_num, sizeof(obj_num), 1, fp.get());

    if (info._obj_tree == nullptr) {
        return LW_RET_FAILED;
    }

    // Локальная лямбда → static-thunk через указатель-на-функцию.
    // EnumTree принимает DWORD(*)(lwITreeNode*, void*); param передаём fp.
    auto saveProc = +[](lwITreeNode* node, void* param) -> DWORD {
        std::FILE* fp = static_cast<std::FILE*>(param);
        lwModelNodeInfo* data = (lwModelNodeInfo*)node->GetData();
        if (LW_RESULT r = LgoLoader::SaveModelNode(*data, fp); LW_FAILED(r)) {
            ToLogService("errors", LogLevel::Error,
                         "[{}] LgoLoader::SaveModelNode failed: ret={}",
                         __FUNCTION__, static_cast<long long>(r));
            return TREENODE_PROC_RET_ABORT;
        }
        return TREENODE_PROC_RET_CONTINUE;
    };

    LW_RESULT r = info._obj_tree->EnumTree(saveProc, fp.get(), TREENODE_PROC_PREORDER);
    return (r == TREENODE_PROC_RET_ABORT) ? LW_RET_FAILED : LW_RET_OK;
}

// =============================================================================
// FILE*-сериализация data-классов: раньше жили как невиртуальные методы
// Load(FILE*, DWORD)/Save(FILE*) на самих классах. Перенесены сюда — все
// data-классы остались чистыми POD без I/O-логики.
// =============================================================================

// --- lwMtlTexInfo (одна запись)

LW_RESULT LgoLoader::LoadMtlTexInfoSingle(lwMtlTexInfo& info, std::FILE* fp, DWORD version) {
    if (version >= EXP_OBJ_VERSION_1_0_0_0 || version == MTLTEX_VERSION0002) {
        LGO_FREAD_OR_RET(fp, &info.opacity, sizeof(info.opacity), 1, "opacity");
        LGO_FREAD_OR_RET(fp, &info.transp_type, sizeof(info.transp_type), 1, "transp_type");
        LGO_FREAD_OR_RET(fp, &info.mtl, sizeof(lwMaterial), 1, "lwMaterial");
        LGO_FREAD_OR_RET(fp, &info.rs_set[0], sizeof(info.rs_set), 1, "rs_set");
        LGO_FREAD_OR_RET(fp, &info.tex_seq[0], sizeof(info.tex_seq), 1, "tex_seq");
    }
    else if (version == MTLTEX_VERSION0001) {
        lwRenderStateSetMtl2 rsm{};
        lwTexInfo_0001 tex_info[LW_MAX_TEXTURESTAGE_NUM]{};

        LGO_FREAD_OR_RET(fp, &info.opacity, sizeof(info.opacity), 1, "opacity (v0001)");
        LGO_FREAD_OR_RET(fp, &info.transp_type, sizeof(info.transp_type), 1, "transp_type (v0001)");
        LGO_FREAD_OR_RET(fp, &info.mtl, sizeof(lwMaterial), 1, "lwMaterial (v0001)");
        LGO_FREAD_OR_RET(fp, &rsm, sizeof(rsm), 1, "rsm (v0001)");
        LGO_FREAD_OR_RET(fp, &tex_info[0], sizeof(tex_info), 1, "tex_info[] (v0001)");

        for (DWORD i = 0; i < rsm.SEQUENCE_SIZE; i++) {
            lwRenderStateValue* rsv = &rsm.rsv_seq[0][i];
            if (rsv->state == LW_INVALID_INDEX) {
                break;
            }
            DWORD v;
            switch (rsv->state) {
            case D3DRS_ALPHAFUNC: v = D3DCMP_GREATER; break;
            case D3DRS_ALPHAREF:  v = 129; break;
            default:              v = rsv->value;
            }
            info.rs_set[i].state = rsv->state;
            info.rs_set[i].value0 = v;
            info.rs_set[i].value1 = v;
        }

        for (DWORD i = 0; i < LW_MAX_TEXTURESTAGE_NUM; i++) {
            lwTexInfo_0001* p = &tex_info[i];
            if (p->stage == LW_INVALID_INDEX) {
                break;
            }
            lwTexInfo* t = &info.tex_seq[i];
            t->level = p->level;
            t->usage = p->usage;
            t->pool = p->pool;
            t->type = p->type;
            t->width = p->width;
            t->height = p->height;
            t->stage = p->stage;
            t->format = p->format;
            t->colorkey = p->colorkey;
            t->colorkey_type = p->colorkey_type;
            t->byte_alignment_flag = p->byte_alignment_flag;
            _tcscpy(t->file_name, p->file_name);

            for (DWORD j = 0; j < p->tss_set.SEQUENCE_SIZE; j++) {
                lwRenderStateValue* rsv = &p->tss_set.rsv_seq[0][j];
                if (rsv->state == LW_INVALID_INDEX) {
                    break;
                }
                t->tss_set[j].state = rsv->state;
                t->tss_set[j].value0 = rsv->value;
                t->tss_set[j].value1 = rsv->value;
            }
        }
    }
    else if (version == MTLTEX_VERSION0000) {
        lwRenderStateSetMtl2 rsm{};
        lwTexInfo_0000 tex_info[LW_MAX_TEXTURESTAGE_NUM]{};

        LGO_FREAD_OR_RET(fp, &info.mtl, sizeof(lwMaterial), 1, "lwMaterial (v0000)");
        LGO_FREAD_OR_RET(fp, &rsm, sizeof(lwRenderStateSetMtl2), 1, "rsm (v0000)");
        LGO_FREAD_OR_RET(fp, &tex_info[0], sizeof(tex_info), 1, "tex_info[] (v0000)");

        for (DWORD i = 0; i < rsm.SEQUENCE_SIZE; i++) {
            lwRenderStateValue* rsv = &rsm.rsv_seq[0][i];
            if (rsv->state == LW_INVALID_INDEX) {
                break;
            }
            DWORD v;
            switch (rsv->state) {
            case D3DRS_ALPHAFUNC: v = D3DCMP_GREATER; break;
            case D3DRS_ALPHAREF:  v = 129; break;
            default:              v = rsv->value;
            }
            info.rs_set[i].state = rsv->state;
            info.rs_set[i].value0 = v;
            info.rs_set[i].value1 = v;
        }

        for (DWORD i = 0; i < LW_MAX_TEXTURESTAGE_NUM; i++) {
            lwTexInfo_0000* p = &tex_info[i];
            if (p->stage == LW_INVALID_INDEX) {
                break;
            }
            lwTexInfo* t = &info.tex_seq[i];
            t->level = D3DX_DEFAULT;
            t->usage = 0;
            t->pool = D3DPOOL_DEFAULT;
            t->type = TEX_TYPE_FILE;
            t->stage = p->stage;
            t->format = p->format;
            t->colorkey = p->colorkey;
            t->colorkey_type = p->colorkey_type;
            t->byte_alignment_flag = 0;
            _tcscpy(t->file_name, p->file_name);

            for (DWORD j = 0; j < p->tss_set.SEQUENCE_SIZE; j++) {
                lwRenderStateValue* rsv = &p->tss_set.rsv_seq[0][j];
                if (rsv->state == LW_INVALID_INDEX) {
                    break;
                }
                t->tss_set[j].state = rsv->state;
                t->tss_set[j].value0 = rsv->value;
                t->tss_set[j].value1 = rsv->value;
            }
        }

        // Legacy 16-bit форматов: апгрейд до A8R8G8B8.
        if (info.tex_seq[0].format == D3DFMT_A4R4G4B4 ||
            info.tex_seq[0].format == D3DFMT_A1R5G5B5) {
            info.tex_seq[0].format = D3DFMT_A8R8G8B8;
        }
    }
    else {
        ToLogService("errors", LogLevel::Error,
                     "[{}] invalid mtltex version: 0x{:08X}", __FUNCTION__, version);
        return LW_RET_FAILED;
    }

    return LW_RET_OK;
}

LW_RESULT LgoLoader::SaveMtlTexInfoSingle(const lwMtlTexInfo& info, std::FILE* fp, DWORD /*version*/) {
    fwrite(&info.opacity, sizeof(info.opacity), 1, fp);
    fwrite(&info.transp_type, sizeof(info.transp_type), 1, fp);
    fwrite(&info.mtl, sizeof(lwMaterial), 1, fp);
    fwrite(&info.rs_set[0], sizeof(info.rs_set), 1, fp);
    fwrite(&info.tex_seq[0], sizeof(info.tex_seq), 1, fp);
    return LW_RET_OK;
}

// --- lwAnimDataTexUV

LW_RESULT LgoLoader::LoadAnimDataTexUV(lwAnimDataTexUV& info, std::FILE* fp, DWORD /*version*/) {
    LGO_FREAD_OR_RET(fp, &info._frame_num, sizeof(DWORD), 1, "AnimDataTexUV._frame_num");

    info._mat_seq = LGO_NEW_ARRAY(lwMatrix44, info._frame_num);
    LGO_FREAD_OR_RET(fp, &info._mat_seq[0], sizeof(lwMatrix44), info._frame_num,
                     "AnimDataTexUV._mat_seq");
    return LW_RET_OK;
}

LW_RESULT LgoLoader::SaveAnimDataTexUV(const lwAnimDataTexUV& info, std::FILE* fp) {
    fwrite(&info._frame_num, sizeof(DWORD), 1, fp);
    fwrite(&info._mat_seq[0], sizeof(lwMatrix44), info._frame_num, fp);
    return LW_RET_OK;
}

// --- lwAnimDataTexImg

LW_RESULT LgoLoader::LoadAnimDataTexImg(lwAnimDataTexImg& info, std::FILE* fp, DWORD version) {
    if (version == EXP_OBJ_VERSION_0_0_0_0) {
        ToLogService("errors", LogLevel::Error,
                     "[{}] old AnimDataTexImg version, re-export needed",
                     __FUNCTION__);
        return LW_RET_FAILED;
    }

    LGO_FREAD_OR_RET(fp, &info._data_num, sizeof(info._data_num), 1, "AnimDataTexImg._data_num");
    info._data_seq = LGO_NEW_ARRAY(lwTexInfo, info._data_num);
    LGO_FREAD_OR_RET(fp, info._data_seq, sizeof(lwTexInfo), info._data_num,
                     "AnimDataTexImg._data_seq");
    return LW_RET_OK;
}

LW_RESULT LgoLoader::SaveAnimDataTexImg(const lwAnimDataTexImg& info, std::FILE* fp) {
    fwrite(&info._data_num, sizeof(info._data_num), 1, fp);
    fwrite(info._data_seq, sizeof(lwTexInfo), info._data_num, fp);
    return LW_RET_OK;
}

// --- lwAnimDataMtlOpacity

LW_RESULT LgoLoader::LoadAnimDataMtlOpacity(lwAnimDataMtlOpacity& info, std::FILE* fp, DWORD /*version*/) {
    LW_RESULT ret = LW_RET_FAILED;

    DWORD num = 0;
    lwKeyFloat* seq = nullptr;

    if (std::fread(&num, sizeof(num), 1, fp) != 1) {
        ToLogService("errors", LogLevel::Error,
                     "[{}] short read of num (file truncated?)",
                     __FUNCTION__);
        goto __ret;
    }
    if (num == 0) {
        ToLogService("errors", LogLevel::Error,
                     "[{}] num=0 (corrupt block?)", __FUNCTION__);
        goto __ret;
    }

    seq = LGO_NEW_ARRAY(lwKeyFloat, num);

    if (std::fread(seq, sizeof(lwKeyFloat), num, fp) != num) {
        ToLogService("errors", LogLevel::Error,
                     "[{}] short read of seq: num={}",
                     __FUNCTION__, static_cast<long long>(num));
        goto __ret;
    }

    {
        auto* aks = LW_NEW(lwAnimKeySetFloat);
        if (LW_RESULT r = aks->SetKeySequence(seq, num); LW_FAILED(r)) {
            ToLogService("errors", LogLevel::Error,
                         "[{}] SetKeySequence failed: num={}, ret={}",
                         __FUNCTION__, static_cast<long long>(num),
                         static_cast<long long>(r));
            LW_DELETE(aks);
            goto __ret;
        }
        info.SetAnimKeySet(aks);
    }

    ret = LW_RET_OK;
__ret:
    LW_IF_DELETE_A(seq);
    return ret;
}

LW_RESULT LgoLoader::SaveAnimDataMtlOpacity(lwAnimDataMtlOpacity& info, std::FILE* fp) {
    auto* aks = info.GetAnimKeySet();
    if (aks == nullptr) {
        return LW_RET_FAILED;
    }

    DWORD num = aks->GetKeyNum();
    lwKeyFloat* seq = aks->GetKeySequence();
    if (num == 0 || seq == nullptr) {
        return LW_RET_FAILED;
    }

    fwrite(&num, sizeof(num), 1, fp);
    fwrite(&seq[0], sizeof(lwKeyFloat), num, fp);
    return LW_RET_OK;
}

// --- lwAnimDataBone

LW_RESULT LgoLoader::LoadAnimDataBone(lwAnimDataBone& info, std::FILE* fp, DWORD version) {
    if (version == EXP_OBJ_VERSION_0_0_0_0) {
        DWORD old_version = 0;
        LGO_FREAD_OR_RET(fp, &old_version, sizeof(old_version), 1, "old_version (AnimDataBone v0)");
    }

    if (info._base_seq != nullptr) {
        // Loader не должен переписывать уже загруженные данные.
        return LW_RET_FAILED;
    }

    LGO_FREAD_OR_RET(fp, &info._header, sizeof(lwAnimDataBone::lwBoneInfoHeader), 1,
                     "lwBoneInfoHeader");

    info._base_seq = LGO_NEW_ARRAY(lwBoneBaseInfo, info._bone_num);
    info._key_seq = LGO_NEW_ARRAY(lwBoneKeyInfo, info._bone_num);
    info._invmat_seq = LGO_NEW_ARRAY(lwMatrix44, info._bone_num);
    info._dummy_seq = LGO_NEW_ARRAY(lwBoneDummyInfo, info._dummy_num);

    LGO_FREAD_OR_RET(fp, info._base_seq, sizeof(lwBoneBaseInfo), info._bone_num,
                     "AnimDataBone._base_seq");
    LGO_FREAD_OR_RET(fp, info._invmat_seq, sizeof(lwMatrix44), info._bone_num,
                     "AnimDataBone._invmat_seq");
    LGO_FREAD_OR_RET(fp, info._dummy_seq, sizeof(lwBoneDummyInfo), info._dummy_num,
                     "AnimDataBone._dummy_seq");

    switch (info._key_type) {
    case BONE_KEY_TYPE_MAT43:
        for (DWORD i = 0; i < info._bone_num; i++) {
            auto* key = &info._key_seq[i];
            key->mat43_seq = LGO_NEW_ARRAY(lwMatrix43, info._frame_num);
            LGO_FREAD_OR_RET(fp, &key->mat43_seq[0], sizeof(lwMatrix43), info._frame_num,
                             "AnimDataBone.mat43_seq");
        }
        break;
    case BONE_KEY_TYPE_MAT44:
        for (DWORD i = 0; i < info._bone_num; i++) {
            auto* key = &info._key_seq[i];
            key->mat44_seq = LGO_NEW_ARRAY(lwMatrix44, info._frame_num);
            LGO_FREAD_OR_RET(fp, &key->mat44_seq[0], sizeof(lwMatrix44), info._frame_num,
                             "AnimDataBone.mat44_seq");
        }
        break;
    case BONE_KEY_TYPE_QUAT:
        if (version >= EXP_OBJ_VERSION_1_0_0_3) {
            for (DWORD i = 0; i < info._bone_num; i++) {
                auto* key = &info._key_seq[i];
                key->pos_seq = LGO_NEW_ARRAY(lwVector3, info._frame_num);
                LGO_FREAD_OR_RET(fp, &key->pos_seq[0], sizeof(lwVector3), info._frame_num,
                                 "AnimDataBone.pos_seq (quat)");
                key->quat_seq = LGO_NEW_ARRAY(lwQuaternion, info._frame_num);
                LGO_FREAD_OR_RET(fp, &key->quat_seq[0], sizeof(lwQuaternion), info._frame_num,
                                 "AnimDataBone.quat_seq");
            }
        }
        else {
            for (DWORD i = 0; i < info._bone_num; i++) {
                auto* key = &info._key_seq[i];
                DWORD pos_num = (info._base_seq[i].parent_id == LW_INVALID_INDEX) ? info._frame_num : 1;

                key->pos_seq = LGO_NEW_ARRAY(lwVector3, info._frame_num);
                LGO_FREAD_OR_RET(fp, &key->pos_seq[0], sizeof(lwVector3), pos_num,
                                 "AnimDataBone.pos_seq (legacy)");
                if (pos_num == 1) {
                    for (DWORD j = 1; j < info._frame_num; j++) {
                        key->pos_seq[j] = key->pos_seq[0];
                    }
                }

                key->quat_seq = LGO_NEW_ARRAY(lwQuaternion, info._frame_num);
                LGO_FREAD_OR_RET(fp, &key->quat_seq[0], sizeof(lwQuaternion), info._frame_num,
                                 "AnimDataBone.quat_seq (legacy)");
            }
        }
        break;
    default:
        ToLogService("errors", LogLevel::Error,
                     "[{}] unknown _key_type: 0x{:08X}", __FUNCTION__, info._key_type);
        return LW_RET_FAILED;
    }

    return LW_RET_OK;
}

LW_RESULT LgoLoader::SaveAnimDataBone(const lwAnimDataBone& info, std::FILE* fp) {
    fwrite(&info._header, sizeof(lwAnimDataBone::lwBoneInfoHeader), 1, fp);

    fwrite(info._base_seq, sizeof(lwBoneBaseInfo), info._bone_num, fp);
    fwrite(info._invmat_seq, sizeof(lwMatrix44), info._bone_num, fp);
    fwrite(info._dummy_seq, sizeof(lwBoneDummyInfo), info._dummy_num, fp);

    switch (info._key_type) {
    case BONE_KEY_TYPE_MAT43:
        for (DWORD i = 0; i < info._bone_num; i++) {
            const auto* key = &info._key_seq[i];
            fwrite(&key->mat43_seq[0], sizeof(lwMatrix43), info._frame_num, fp);
        }
        break;
    case BONE_KEY_TYPE_MAT44:
        for (DWORD i = 0; i < info._bone_num; i++) {
            const auto* key = &info._key_seq[i];
            fwrite(&key->mat44_seq[0], sizeof(lwMatrix44), info._frame_num, fp);
        }
        break;
    case BONE_KEY_TYPE_QUAT:
        for (DWORD i = 0; i < info._bone_num; i++) {
            const auto* key = &info._key_seq[i];
            fwrite(&key->pos_seq[0], sizeof(lwVector3), info._frame_num, fp);
            fwrite(&key->quat_seq[0], sizeof(lwQuaternion), info._frame_num, fp);
        }
        break;
    default:
        return LW_RET_FAILED;
    }
    return LW_RET_OK;
}

LW_RESULT LgoLoader::LoadAnimDataBone(lwAnimDataBone& info, std::string_view file) {
    UniqueFile fp{std::fopen(std::string{file}.c_str(), "rb")};
    if (!fp) {
        return LW_RET_FAILED;
    }

    DWORD version = 0;
    LGO_FREAD_OR_RET(fp.get(), &version, sizeof(version), 1, "AnimDataBone version");

    if (version < EXP_OBJ_VERSION_1_0_0_0) {
        ToLogService("errors", LogLevel::Error,
                     "[{}] old animation file (version=0x{:08X}), re-export needed: file={}",
                     __FUNCTION__, version,
                     (file.empty() ? std::string_view{"(null)"} : file));
        return LW_RET_FAILED;
    }

    if (LW_RESULT r = LoadAnimDataBone(info, fp.get(), version); LW_FAILED(r)) {
        ToLogService("errors", LogLevel::Error,
                     "[{}] LoadAnimDataBone(fp) failed: file='{}', version={}, ret={}",
                     __FUNCTION__, (file.empty() ? std::string_view{"(null)"} : file),
                     static_cast<long long>(version), static_cast<long long>(r));
        return r;
    }
    return LW_RET_OK;
}

LW_RESULT LgoLoader::SaveAnimDataBone(const lwAnimDataBone& info, std::string_view file) {
    UniqueFile fp{std::fopen(std::string{file}.c_str(), "wb")};
    if (!fp) {
        return LW_RET_FAILED;
    }

    DWORD version = EXP_OBJ_VERSION;
    fwrite(&version, sizeof(version), 1, fp.get());

    if (LW_RESULT r = SaveAnimDataBone(info, fp.get()); LW_FAILED(r)) {
        ToLogService("errors", LogLevel::Error,
                     "[{}] SaveAnimDataBone(fp) failed: file='{}', ret={}",
                     __FUNCTION__, (file.empty() ? std::string_view{"(null)"} : file),
                     static_cast<long long>(r));
        return r;
    }
    return LW_RET_OK;
}

// --- lwAnimDataMatrix

LW_RESULT LgoLoader::LoadAnimDataMatrix(lwAnimDataMatrix& info, std::FILE* fp, DWORD /*version*/) {
    LGO_FREAD_OR_RET(fp, &info._frame_num, sizeof(DWORD), 1, "AnimDataMatrix._frame_num");
    info._mat_seq = LGO_NEW_ARRAY(lwMatrix43, info._frame_num);
    LGO_FREAD_OR_RET(fp, &info._mat_seq[0], sizeof(lwMatrix43), info._frame_num,
                     "AnimDataMatrix._mat_seq");
    return LW_RET_OK;
}

LW_RESULT LgoLoader::SaveAnimDataMatrix(const lwAnimDataMatrix& info, std::FILE* fp) {
    fwrite(&info._frame_num, sizeof(DWORD), 1, fp);
    fwrite(&info._mat_seq[0], sizeof(lwMatrix43), info._frame_num, fp);
    return LW_RET_OK;
}

// --- lwAnimKeySetPRS (раньше — свободные функции lwLoadAnimKeySetPRS / lwSaveAnimKeySetPRS)

LW_RESULT LgoLoader::LoadAnimKeySetPRS(lwAnimKeySetPRS& info, std::FILE* fp) {
    LGO_FREAD_OR_RET(fp, &info.frame_num, sizeof(DWORD), 1, "AnimKeySetPRS.frame_num");
    LGO_FREAD_OR_RET(fp, &info.pos_num, sizeof(DWORD), 1, "AnimKeySetPRS.pos_num");
    LGO_FREAD_OR_RET(fp, &info.rot_num, sizeof(DWORD), 1, "AnimKeySetPRS.rot_num");
    LGO_FREAD_OR_RET(fp, &info.sca_num, sizeof(DWORD), 1, "AnimKeySetPRS.sca_num");

    if (info.pos_num > 0) {
        info.pos_seq = LGO_NEW_ARRAY(lwKeyDataVector3, info.pos_num);
        LGO_FREAD_OR_RET(fp, &info.pos_seq[0], sizeof(lwKeyDataVector3), info.pos_num,
                         "AnimKeySetPRS.pos_seq");
    }

    if (info.rot_num > 0) {
        info.rot_seq = LGO_NEW_ARRAY(lwKeyDataQuaternion, info.rot_num);
        LGO_FREAD_OR_RET(fp, &info.rot_seq[0], sizeof(lwKeyDataQuaternion), info.rot_num,
                         "AnimKeySetPRS.rot_seq");
    }

    if (info.sca_num > 0) {
        info.sca_seq = LGO_NEW_ARRAY(lwKeyDataVector3, info.sca_num);
        LGO_FREAD_OR_RET(fp, &info.sca_seq[0], sizeof(lwKeyDataVector3), info.sca_num,
                         "AnimKeySetPRS.sca_seq");
    }

    return LW_RET_OK;
}

LW_RESULT LgoLoader::SaveAnimKeySetPRS(const lwAnimKeySetPRS& info, std::FILE* fp) {
    fwrite(&info.frame_num, sizeof(DWORD), 1, fp);
    fwrite(&info.pos_num, sizeof(DWORD), 1, fp);
    fwrite(&info.rot_num, sizeof(DWORD), 1, fp);
    fwrite(&info.sca_num, sizeof(DWORD), 1, fp);

    if (info.pos_num > 0) {
        fwrite(&info.pos_seq[0], sizeof(lwKeyDataVector3), info.pos_num, fp);
    }
    if (info.rot_num > 0) {
        fwrite(&info.rot_seq[0], sizeof(lwKeyDataQuaternion), info.rot_num, fp);
    }
    if (info.sca_num > 0) {
        fwrite(&info.sca_seq[0], sizeof(lwKeyDataVector3), info.sca_num, fp);
    }

    return LW_RET_OK;
}

// =============================================================================
// Расширенная диагностика для .lmo и .lab (используется PkoTool / тулзами)
// =============================================================================

LW_RESULT LgoLoader::LoadModelObjEx(lwModelObjInfo& info, std::string_view file,
                                    LgoLoadDiagnostics& diag) {
    diag = {};

    UniqueFile fp{std::fopen(std::string{file}.c_str(), "rb")};
    if (!fp) {
        diag.status = LgoLoadStatus::FileOpenFailed;
        diag.detail = "fopen failed";
        return LW_RET_FAILED;
    }

    DWORD version = 0;
    if (std::fread(&version, sizeof(version), 1, fp.get()) != 1) {
        diag.status = LgoLoadStatus::VersionTruncated;
        diag.detail = "short read of version DWORD (file empty or truncated)";
        return LW_RET_FAILED;
    }
    diag.version = version;

    DWORD obj_num = 0;
    if (std::fread(&obj_num, sizeof(obj_num), 1, fp.get()) != 1) {
        diag.status = LgoLoadStatus::HeaderTruncated;
        diag.detail = "short read of obj_num DWORD";
        return LW_RET_FAILED;
    }

    if (obj_num == 0 || obj_num > LW_MAX_MODEL_OBJ_NUM) {
        diag.status = LgoLoadStatus::BlockSizesInconsistent;
        diag.detail = std::format("obj_num={} (expected 1..{})", obj_num, LW_MAX_MODEL_OBJ_NUM);
        return LW_RET_FAILED;
    }

    lwModelObjInfo::lwModelObjInfoHeader header[LW_MAX_MODEL_OBJ_NUM];
    if (std::fread(&header[0], sizeof(lwModelObjInfo::lwModelObjInfoHeader), obj_num, fp.get())
        != obj_num) {
        diag.status = LgoLoadStatus::HeaderTruncated;
        diag.detail = std::format("short read of {} headers", obj_num);
        return LW_RET_FAILED;
    }

    info.geom_obj_num = 0;

    for (DWORD i = 0; i < obj_num; i++) {
        if (std::fseek(fp.get(), header[i].addr, SEEK_SET) != 0) {
            diag.status = LgoLoadStatus::BlockSizesInconsistent;
            diag.detail = std::format("fseek to addr=0x{:08X} failed (i={})",
                                      header[i].addr, i);
            return LW_RET_FAILED;
        }

        switch (header[i].type) {
        case MODEL_OBJ_TYPE_GEOMETRY: {
            info.geom_obj_seq[info.geom_obj_num] = new lwGeomObjInfo();
            if (version == EXP_OBJ_VERSION_0_0_0_0) {
                DWORD old_version = 0;
                if (std::fread(&old_version, sizeof(old_version), 1, fp.get()) != 1) {
                    diag.status = LgoLoadStatus::VersionTruncated;
                    diag.detail = std::format("short read of legacy old_version (i={})", i);
                    return LW_RET_FAILED;
                }
            }
            if (LW_RESULT r = LoadFromStream(info.geom_obj_seq[info.geom_obj_num], fp.get(),
                                             version);
                LW_FAILED(r)) {
                diag.status = LgoLoadStatus::ParseFailed;
                diag.detail = std::format("LoadFromStream(geom={}) failed: ret={}",
                                          i, static_cast<long long>(r));
                return LW_RET_FAILED;
            }
            // ApplyRuntimeDefaults — задача рантайм-каллера; см. LoadModelObj.
            info.geom_obj_num += 1;
            break;
        }
        case MODEL_OBJ_TYPE_HELPER:
            if (LW_RESULT r = LoadHelperInfo(info.helper_data, fp.get(), version); LW_FAILED(r)) {
                diag.status = LgoLoadStatus::ParseFailed;
                diag.detail = std::format("LoadHelperInfo(i={}) failed: ret={}",
                                          i, static_cast<long long>(r));
                return LW_RET_FAILED;
            }
            break;
        default:
            diag.status = LgoLoadStatus::BlockSizesInconsistent;
            diag.detail = std::format("unknown obj type=0x{:08X} (i={})", header[i].type, i);
            return LW_RET_FAILED;
        }
    }

    diag.status = LgoLoadStatus::Ok;
    return LW_RET_OK;
}

LW_RESULT LgoLoader::LoadAnimDataBoneEx(lwAnimDataBone& info, std::string_view file,
                                        LgoLoadDiagnostics& diag) {
    diag = {};

    UniqueFile fp{std::fopen(std::string{file}.c_str(), "rb")};
    if (!fp) {
        diag.status = LgoLoadStatus::FileOpenFailed;
        diag.detail = "fopen failed";
        return LW_RET_FAILED;
    }

    DWORD version = 0;
    if (std::fread(&version, sizeof(version), 1, fp.get()) != 1) {
        diag.status = LgoLoadStatus::VersionTruncated;
        diag.detail = "short read of version DWORD";
        return LW_RET_FAILED;
    }
    diag.version = version;

    if (version < EXP_OBJ_VERSION_1_0_0_0) {
        diag.status = LgoLoadStatus::VersionUnknown;
        diag.detail = std::format("version=0x{:08X} (expected >= 0x1000)", version);
        return LW_RET_FAILED;
    }

    if (LW_RESULT r = LoadAnimDataBone(info, fp.get(), version); LW_FAILED(r)) {
        diag.status = LgoLoadStatus::ParseFailed;
        diag.detail = std::format("LoadAnimDataBone(fp) failed: ret={}", static_cast<long long>(r));
        return LW_RET_FAILED;
    }

    diag.status = LgoLoadStatus::Ok;
    return LW_RET_OK;
}

} // namespace Corsairs::Engine::Render
