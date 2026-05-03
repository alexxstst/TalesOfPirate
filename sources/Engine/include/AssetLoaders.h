#pragma once

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <stdexcept>
#include <string>
#include <string_view>

#include "lwClassDecl.h"

// AssetLoaders.h — единственный API чтения/записи .lgo и .lmo. Пользуется
// типами lwGeomObjInfo, lwModelObjInfo, lwModelInfo и т.д. — потому
// инклюдим полный заголовок lwExpObj.h (в т.ч. ради внутреннего типа
// lwModelObjInfo::lwModelObjInfoHeader).
#include "lwExpObj.h"

namespace Corsairs::Engine::Render {

// Причина отказа `LgoLoader::LoadEx`. `Ok` при успешной загрузке; всё остальное —
// конкретный вид поломки, который тулза/тест может отобразить пользователю.
enum class LgoLoadStatus : std::uint32_t {
    Ok = 0,
    OkWithTrailingData,    // success: info валиден, но в файле остался хвост, который наш парсер не читает
    FileOpenFailed,        // fopen(file) не удался (нет файла, прав и т.п.)
    VersionTruncated,      // меньше 4 байт в файле — DWORD version не прочитан
    VersionUnknown,        // version не из {0x0000, 0x1000, 0x1001, 0x1002, 0x1003, 0x1004, 0x1005}
    HeaderTruncated,       // не удалось прочитать lwGeomObjInfoHeader (116 байт)
    BlockSizesInconsistent,// header.{mtl,mesh,helper,anim}_size + 120 > file_size — заявлено больше чем есть
    ParseFailed,           // LoadFromStream вернул LW_RET_FAILED (mtl_size > 100000 и т.п.)
    UnconsumedTrailingData // после LoadFromStream остались непрочитанные байты файла (legacy enum, теперь не используется как fail)
};

// Диагностический отчёт `LoadEx`. После успеха `status==Ok`, `version` заполнен;
// после провала — `status` ≠ Ok, `detail` содержит человекочитаемый текст.
struct LgoLoadDiagnostics {
    LgoLoadStatus status{LgoLoadStatus::Ok};
    std::string detail;
    std::uint32_t version{0};
};

// Все алгоритмы сериализации .lgo и .lmo живут здесь. Никаких свободных функций
// рядом с классом нет — всё, что использует только сам loader, — приватный
// статический член. Data-структуры (lwGeomObjInfo, lwMeshInfo, lwAnimDataInfo,
// lwHelperInfo, lwModelInfo, lwModelNodeInfo, lwHelperDummyObjInfo) обязаны
// оставаться без I/O-методов; для доступа к приватным полям сами data-классы
// объявляют LgoLoader другом (см. `friend class Corsairs::Engine::Render::LgoLoader;`
// в lwExpObj.h).
class LgoLoader {
public:
    // -----------------------------------------------------------------------
    // Константы и enum'ы
    // -----------------------------------------------------------------------

    // Sanity-лимит на одну аллокацию массива при чтении .lgo. Реальные ассеты
    // упираются в единицы мегабайт (самый большой `mesh_size` в датасете
    // ~4.5 МБ). Любое требование сверх этого порога — почти всегда битое поле
    // `*_num` в файле, и LW_NEW попытается выделить мусорный объём.
    static constexpr std::size_t kMaxArrayBytes = 32u * 1024u * 1024u;

    // -----------------------------------------------------------------------
    // .lgo (одиночный lwGeomObjInfo)
    // -----------------------------------------------------------------------

    // Старая совместимая обёртка: возвращает nullptr при любой ошибке без объяснений.
    [[nodiscard]] static LW_NAMESPACE::lwGeomObjInfo* Load(std::string_view file);

    // Расширенная загрузка с диагностикой — для тулзы и тестов. При неуспехе
    // возвращает nullptr и заполняет `diag.status` + `diag.detail`.
    [[nodiscard]] static LW_NAMESPACE::lwGeomObjInfo* LoadEx(std::string_view file,
                                                              LgoLoadDiagnostics& diag);

    static LW_RESULT Save(LW_NAMESPACE::lwGeomObjInfo* info, std::string_view file);

    static LW_RESULT LoadFromStream(LW_NAMESPACE::lwGeomObjInfo* info, std::FILE* fp, DWORD version);
    static LW_RESULT SaveToStream(LW_NAMESPACE::lwGeomObjInfo* info, std::FILE* fp);

    // -----------------------------------------------------------------------
    // Подсчёт размеров (используется и сторонним кодом — `lwPrimitive::ExtractMeshInfo`,
    // `LoadModelObj/SaveModelObj` ниже).
    // -----------------------------------------------------------------------

    static DWORD GetMtlTexInfoSize(const LW_NAMESPACE::lwGeomObjInfo* info);
    static DWORD GetMeshInfoSize(const LW_NAMESPACE::lwGeomObjInfo* info);
    static DWORD GetHelperInfoSize(const LW_NAMESPACE::lwHelperInfo& info);
    static DWORD GetAnimDataInfoSize(const LW_NAMESPACE::lwAnimDataInfo& info);

    // -----------------------------------------------------------------------
    // Сериализация helper-блока. Дёргается из LoadFromStream/SaveToStream и
    // из .lmo-сериализаторов ниже.
    // -----------------------------------------------------------------------

    static LW_RESULT LoadHelperInfo(LW_NAMESPACE::lwHelperInfo& info, std::FILE* fp, DWORD version);
    static LW_RESULT SaveHelperInfo(const LW_NAMESPACE::lwHelperInfo& info, std::FILE* fp);

    // -----------------------------------------------------------------------
    // FILE*-сериализация data-классов (раньше жили как невиртуальные методы
    // Load(FILE*, DWORD)/Save(FILE*) на самих классах). Перенесены сюда,
    // чтобы все алгоритмы чтения/записи .lgo были в одном месте.
    // -----------------------------------------------------------------------

    static LW_RESULT LoadAnimDataBone(LW_NAMESPACE::lwAnimDataBone& info, std::FILE* fp, DWORD version);
    static LW_RESULT SaveAnimDataBone(const LW_NAMESPACE::lwAnimDataBone& info, std::FILE* fp);
    // Path-обёртки: открывают файл, читают/пишут DWORD-версию, делегируют в FILE*-вариант.
    // Раньше жили как виртуальные `lwIAnimDataBone::Load/Save(string_view)`. Перенесены сюда —
    // I/O `.lab`-файлов теперь полностью контролируется LgoLoader.
    static LW_RESULT LoadAnimDataBone(LW_NAMESPACE::lwAnimDataBone& info, std::string_view file);
    static LW_RESULT SaveAnimDataBone(const LW_NAMESPACE::lwAnimDataBone& info, std::string_view file);

    static LW_RESULT LoadAnimDataMatrix(LW_NAMESPACE::lwAnimDataMatrix& info, std::FILE* fp, DWORD version);
    static LW_RESULT SaveAnimDataMatrix(const LW_NAMESPACE::lwAnimDataMatrix& info, std::FILE* fp);

    static LW_RESULT LoadAnimDataTexUV(LW_NAMESPACE::lwAnimDataTexUV& info, std::FILE* fp, DWORD version);
    static LW_RESULT SaveAnimDataTexUV(const LW_NAMESPACE::lwAnimDataTexUV& info, std::FILE* fp);

    static LW_RESULT LoadAnimDataTexImg(LW_NAMESPACE::lwAnimDataTexImg& info, std::FILE* fp, DWORD version);
    static LW_RESULT SaveAnimDataTexImg(const LW_NAMESPACE::lwAnimDataTexImg& info, std::FILE* fp);

    static LW_RESULT LoadAnimDataMtlOpacity(LW_NAMESPACE::lwAnimDataMtlOpacity& info, std::FILE* fp, DWORD version);
    static LW_RESULT SaveAnimDataMtlOpacity(LW_NAMESPACE::lwAnimDataMtlOpacity& info, std::FILE* fp);

    // Сериализация одного lwMtlTexInfo (раньше — свободные функции
    // lwMtlTexInfo_Load/lwMtlTexInfo_Save в LW_NAMESPACE).
    static LW_RESULT LoadMtlTexInfoSingle(LW_NAMESPACE::lwMtlTexInfo& info, std::FILE* fp, DWORD version);
    static LW_RESULT SaveMtlTexInfoSingle(const LW_NAMESPACE::lwMtlTexInfo& info, std::FILE* fp, DWORD version);

    // Сериализация lwAnimKeySetPRS (раньше — свободные функции
    // lwLoadAnimKeySetPRS/lwSaveAnimKeySetPRS в LW_NAMESPACE).
    static LW_RESULT LoadAnimKeySetPRS(LW_NAMESPACE::lwAnimKeySetPRS& info, std::FILE* fp);
    static LW_RESULT SaveAnimKeySetPRS(const LW_NAMESPACE::lwAnimKeySetPRS& info, std::FILE* fp);

    // -----------------------------------------------------------------------
    // .lmo (lwModelObjInfo — array geom_obj + helper по offsets)
    // -----------------------------------------------------------------------

    static LW_RESULT LoadModelObj(LW_NAMESPACE::lwModelObjInfo& info, std::string_view file);
    static LW_RESULT SaveModelObj(LW_NAMESPACE::lwModelObjInfo& info, std::string_view file);
    static DWORD     GetModelObjSize(const LW_NAMESPACE::lwModelObjInfo& info);
    static LW_RESULT GetModelObjHeader(LW_NAMESPACE::lwModelObjInfo::lwModelObjInfoHeader* out_seq,
                                       DWORD* out_num,
                                       std::string_view file);

    // Расширенная диагностика для тулз и валидаторов: при неуспехе заполняет
    // `diag.status`+`diag.detail`. По смыслу аналогична `LoadEx` для .lgo.
    static LW_RESULT LoadModelObjEx(LW_NAMESPACE::lwModelObjInfo& info, std::string_view file,
                                    LgoLoadDiagnostics& diag);
    static LW_RESULT LoadAnimDataBoneEx(LW_NAMESPACE::lwAnimDataBone& info, std::string_view file,
                                        LgoLoadDiagnostics& diag);

    // -----------------------------------------------------------------------
    // Tree-based .lmo (lwModelInfo с lwITreeNode-деревом lwModelNodeInfo)
    // -----------------------------------------------------------------------

    static LW_RESULT LoadModel(LW_NAMESPACE::lwModelInfo& info, std::string_view file);
    static LW_RESULT SaveModel(LW_NAMESPACE::lwModelInfo& info, std::string_view file);

    // -----------------------------------------------------------------------
    // Прочее
    // -----------------------------------------------------------------------

    // Применить рантайм-инварианты к свежезагруженному info: STATE_FRAMECULLING := 0,
    // STATE_UPDATETRANSPSTATE := 1. В файлах эти поля хранятся как «после экспорта»
    // (обычно 0/0), но рантайм требует другие значения, чтобы primitive после Load
    // корректно пересчитал прозрачность и не отсекался frustum-culling'ом. В Load/
    // LoadFromStream этого делать НЕЛЬЗЯ, иначе ломается round-trip Load→Save.
    static void ApplyRuntimeDefaults(LW_NAMESPACE::lwGeomObjInfo* info);

    // Безопасная замена `LW_NEW(T[N])` для loader/copy-кода .lgo: тот же
    // `new T[N]`, но при превышении `kMaxArrayBytes` логирует контекст и
    // бросает `std::length_error`, чтобы битое поле `*_num` не превратилось
    // в десятки гигабайт `new T[N]` и в последующий `fread` поверх кучи.
    // Бросок одинаков в Debug и Release.
    template <typename T>
    [[nodiscard]] static T* CheckedNewArray(std::size_t count, const char* typeName) {
        const std::size_t bytes = count * sizeof(T);
        if (bytes > kMaxArrayBytes) {
            ThrowSuspiciousAlloc(typeName, count, sizeof(T), bytes);
        }
        return new T[count];
    }

private:
    // -----------------------------------------------------------------------
    // Приватные подблоки .lgo. Раньше жили как свободные функции (`lwLoadMtlTexInfo`,
    // `lwMeshInfo_Load`) или методы data-классов (`lwAnimDataInfo::Load`/`Save`).
    // -----------------------------------------------------------------------

    static LW_RESULT LoadMtlTexInfo(LW_NAMESPACE::lwGeomObjInfo* info, std::FILE* fp, DWORD version);
    static LW_RESULT SaveMtlTexInfo(const LW_NAMESPACE::lwGeomObjInfo* info, std::FILE* fp);
    static LW_RESULT LoadMeshInfo(LW_NAMESPACE::lwGeomObjInfo* info, std::FILE* fp, DWORD version);
    static LW_RESULT SaveMeshInfo(const LW_NAMESPACE::lwGeomObjInfo* info, std::FILE* fp);
    static LW_RESULT LoadAnimDataInfo(LW_NAMESPACE::lwAnimDataInfo& info, std::FILE* fp, DWORD version);
    static LW_RESULT SaveAnimDataInfo(LW_NAMESPACE::lwAnimDataInfo& info, std::FILE* fp);

    // 5+5 разделов lwHelperInfo (dummy/box/mesh/bbox/bsphere).
    static LW_RESULT LoadHelperDummySection(LW_NAMESPACE::lwHelperInfo& info, std::FILE* fp, DWORD version);
    static LW_RESULT LoadHelperBoxSection(LW_NAMESPACE::lwHelperInfo& info, std::FILE* fp, DWORD version);
    static LW_RESULT LoadHelperMeshSection(LW_NAMESPACE::lwHelperInfo& info, std::FILE* fp, DWORD version);
    static LW_RESULT LoadBoundingBoxSection(LW_NAMESPACE::lwHelperInfo& info, std::FILE* fp, DWORD version);
    static LW_RESULT LoadBoundingSphereSection(LW_NAMESPACE::lwHelperInfo& info, std::FILE* fp, DWORD version);

    static LW_RESULT SaveHelperDummySection(const LW_NAMESPACE::lwHelperInfo& info, std::FILE* fp);
    static LW_RESULT SaveHelperBoxSection(const LW_NAMESPACE::lwHelperInfo& info, std::FILE* fp);
    static LW_RESULT SaveHelperMeshSection(const LW_NAMESPACE::lwHelperInfo& info, std::FILE* fp);
    static LW_RESULT SaveBoundingBoxSection(const LW_NAMESPACE::lwHelperInfo& info, std::FILE* fp);
    static LW_RESULT SaveBoundingSphereSection(const LW_NAMESPACE::lwHelperInfo& info, std::FILE* fp);

    // .lmo tree-узлы и dummy-helper-объект.
    static LW_RESULT LoadModelNode(LW_NAMESPACE::lwModelNodeInfo& info, std::FILE* fp, DWORD version);
    static LW_RESULT SaveModelNode(LW_NAMESPACE::lwModelNodeInfo& info, std::FILE* fp);
    static LW_RESULT LoadHelperDummyObj(LW_NAMESPACE::lwHelperDummyObjInfo& info, std::FILE* fp, DWORD version);
    static LW_RESULT SaveHelperDummyObj(LW_NAMESPACE::lwHelperDummyObjInfo& info, std::FILE* fp);

    // Утилиты.
    [[nodiscard]] static bool          IsKnownVersion(DWORD v);
    [[nodiscard]] static std::int64_t  FileSize(std::FILE* fp);
    static void                        LogLoadFailure(std::string_view file, const LgoLoadDiagnostics& diag);

    [[noreturn]] static void ThrowSuspiciousAlloc(const char* typeName,
                                                  std::size_t count,
                                                  std::size_t elemSize,
                                                  std::size_t totalBytes);
};

[[nodiscard]] constexpr std::string_view ToString(LgoLoadStatus s) noexcept {
    switch (s) {
        case LgoLoadStatus::Ok:                     return "Ok";
        case LgoLoadStatus::OkWithTrailingData:     return "OkWithTrailingData";
        case LgoLoadStatus::FileOpenFailed:         return "FileOpenFailed";
        case LgoLoadStatus::VersionTruncated:       return "VersionTruncated";
        case LgoLoadStatus::VersionUnknown:         return "VersionUnknown";
        case LgoLoadStatus::HeaderTruncated:        return "HeaderTruncated";
        case LgoLoadStatus::BlockSizesInconsistent: return "BlockSizesInconsistent";
        case LgoLoadStatus::ParseFailed:            return "ParseFailed";
        case LgoLoadStatus::UnconsumedTrailingData: return "UnconsumedTrailingData";
    }
    return "?";
}

// Макрос для всех LGO_NEW_ARRAY(T, N) call-sites. Раскрывается в
// LgoLoader::CheckedNewArray<T>(N, "T"). Сделан макросом ради автоматического
// stringify-имени типа в логе.
#define LGO_NEW_ARRAY(type, count) (::Corsairs::Engine::Render::LgoLoader::CheckedNewArray<type>(static_cast<std::size_t>(count), #type))

} // namespace Corsairs::Engine::Render
