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
    // Tree-based .lmo / .lxo (lwModelInfo с lwITreeNode-деревом lwModelNodeInfo)
    // -----------------------------------------------------------------------

    static LW_RESULT LoadModel(LW_NAMESPACE::lwModelInfo& info, std::string_view file);
    static LW_RESULT SaveModel(LW_NAMESPACE::lwModelInfo& info, std::string_view file);

    // Расширенная диагностика для tree-based .lxo (используется PkoTool/тулзами).
    // На успех — `diag.status=Ok`, `diag.version=info._head.version`. На неуспех —
    // конкретный `LgoLoadStatus` + текстовый detail.
    static LW_RESULT LoadModelEx(LW_NAMESPACE::lwModelInfo& info, std::string_view file,
                                 LgoLoadDiagnostics& diag);

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

// =============================================================================
// .eff — Effect-файл MindPower3D (header + массив I_Effect-элементов)
// =============================================================================

} // namespace Corsairs::Engine::Render

// Forward-decl в глобальном пространстве (полные определения в MPModelEff.h /
// I_Effect.h). EffectLoader работает по ссылке; клиенты, конструирующие
// EffectFileInfo или передающие I_Effect&, должны включить эти заголовки.
struct EffectFileInfo;
class  I_Effect;

namespace Corsairs::Engine::Render {

enum class EffectLoadStatus : std::uint32_t {
    Ok = 0,
    FileOpenFailed,        // fopen(file) не удался
    VersionTruncated,      // < 4 байт в файле — version DWORD не прочитан
    VersionUnknown,        // version вне поддерживаемого диапазона (1..7 на момент написания)
    HeaderTruncated,       // не дочитан header (idxTech / paths / rotation block)
    ParseFailed            // I_Effect::LoadFromFile вернул false
};

struct EffectLoadDiagnostics {
    EffectLoadStatus status{EffectLoadStatus::Ok};
    std::string detail;
    std::uint32_t version{0};
};

// Все алгоритмы сериализации .eff живут здесь. По смыслу — параллель LgoLoader,
// но для другого формата данных. Само сохранение использует существующие
// `I_Effect::Save/LoadFromFile`-методы поэлементно (миграция этих методов
// внутрь EffectLoader — отдельная задача; см. план в issue tracker).
class EffectLoader {
public:
    // Текущая on-disk version, которую пишет Save (latest format).
    static constexpr std::uint32_t kCurrentVersion = 7;

    [[nodiscard]] static LW_RESULT Load(::EffectFileInfo& info, std::string_view file);
    [[nodiscard]] static LW_RESULT LoadEx(::EffectFileInfo& info, std::string_view file,
                                          EffectLoadDiagnostics& diag);
    static LW_RESULT Save(::EffectFileInfo& info, std::string_view file);

    // YAML-сериализация .eff для round-trip-тестов и человекочитаемого
    // редактирования. Формат — индент-структурированный YAML с inline-массивами
    // и hex-float'ами (`{:a}`), чтобы текстовое представление давало побайтно
    // точный обратный binary при пересохранении. Парсер поддерживает только
    // подмножество YAML, нужное для собственного эмиттера. На неуспех — пишет
    // в errors-канал и возвращает LW_RET_FAILED. Реализация в EffectYaml.cpp.
    [[nodiscard]] static LW_RESULT ExportToYaml(const ::EffectFileInfo& info,
                                                std::string_view file);
    [[nodiscard]] static LW_RESULT ImportFromYaml(::EffectFileInfo& info,
                                                  std::string_view file);

    // Сериализация одного элемента .eff. Раньше жили как методы I_Effect
    // (`SaveToFile`/`LoadFromFile`); по правилу проекта I/O в data-классах
    // запрещён, поэтому перенесены сюда и вызываются из:
    //  • EffectLoader::Save/LoadEx (целый .eff = header + N элементов);
    //  • CMPModelEff::SaveToFile (тот же формат с собственными полями);
    //  • CMPResManger::LoadEffectFromFile (legacy-loader, постепенно
    //    мигрируется на EffectLoader::Load).
    [[nodiscard]] static bool LoadElement(::I_Effect& effect, std::FILE* fp, DWORD version);
    static bool SaveElement(::I_Effect& effect, std::FILE* fp);
};

[[nodiscard]] constexpr std::string_view ToString(EffectLoadStatus s) noexcept {
    switch (s) {
        case EffectLoadStatus::Ok:                return "Ok";
        case EffectLoadStatus::FileOpenFailed:    return "FileOpenFailed";
        case EffectLoadStatus::VersionTruncated:  return "VersionTruncated";
        case EffectLoadStatus::VersionUnknown:    return "VersionUnknown";
        case EffectLoadStatus::HeaderTruncated:   return "HeaderTruncated";
        case EffectLoadStatus::ParseFailed:       return "ParseFailed";
    }
    return "?";
}

// =============================================================================
// .par — Particle-controller файл (CMPPartCtrl: name, particle systems, strips, models)
// =============================================================================

} // namespace Corsairs::Engine::Render

class CMPPartCtrl;
class CMPPartSys;
class CMPStrip;
class CChaModel;

namespace Corsairs::Engine::Render {

enum class PartCtrlLoadStatus : std::uint32_t {
    Ok = 0,
    FileOpenFailed,        // fopen(file) не удался
    VersionTruncated,      // < 4 байт в файле — version DWORD не прочитан
    VersionUnknown,        // version вне поддерживаемого диапазона (2..ParVersion=15)
    ParseFailed            // CMPPartCtrl::LoadFromFile вернул false
};

struct PartCtrlLoadDiagnostics {
    PartCtrlLoadStatus status{PartCtrlLoadStatus::Ok};
    std::string detail;
    std::uint32_t version{0};
};

// Чтение и запись .par-файла (CMPPartCtrl + per-element CMPPartSys/CMPStrip/
// CChaModel). Все методы Load/Save data-классов перенесены сюда; loader имеет
// доступ к их полям через friend-объявление в каждом из них.
class PartCtrlLoader {
public:
    [[nodiscard]] static LW_RESULT Load(::CMPPartCtrl& ctrl, std::string_view file);
    [[nodiscard]] static LW_RESULT LoadEx(::CMPPartCtrl& ctrl, std::string_view file,
                                          PartCtrlLoadDiagnostics& diag);
    static LW_RESULT Save(::CMPPartCtrl& ctrl, std::string_view file);

    // YAML-сериализация .par для round-trip-тестов и человекочитаемого
    // редактирования. Реализация — PartCtrlYaml.cpp; общий YAML-движок —
    // YamlCommon.h. Round-trip побайтно точный после первого binary save'а.
    [[nodiscard]] static LW_RESULT ExportToYaml(const ::CMPPartCtrl& ctrl,
                                                std::string_view file);
    [[nodiscard]] static LW_RESULT ImportFromYaml(::CMPPartCtrl& ctrl,
                                                  std::string_view file);

    // Per-element сериализация — приватная (вызывается только из Load/Save
    // выше). public-API остаётся минимальным.
private:
    static bool LoadCtrl(::CMPPartCtrl& ctrl, std::string_view file);
    static bool SaveCtrl(::CMPPartCtrl& ctrl, std::string_view file);

    static bool LoadPartSys(::CMPPartSys& ps, std::FILE* fp, DWORD version);
    static bool SavePartSys(::CMPPartSys& ps, std::FILE* fp);

    static bool LoadStrip(::CMPStrip& s, std::FILE* fp, DWORD version);
    static bool SaveStrip(::CMPStrip& s, std::FILE* fp);

    static void LoadCharModel(::CChaModel& m, std::FILE* fp);
    static void SaveCharModel(::CChaModel& m, std::FILE* fp);
};

[[nodiscard]] constexpr std::string_view ToString(PartCtrlLoadStatus s) noexcept {
    switch (s) {
        case PartCtrlLoadStatus::Ok:                return "Ok";
        case PartCtrlLoadStatus::FileOpenFailed:    return "FileOpenFailed";
        case PartCtrlLoadStatus::VersionTruncated:  return "VersionTruncated";
        case PartCtrlLoadStatus::VersionUnknown:    return "VersionUnknown";
        case PartCtrlLoadStatus::ParseFailed:       return "ParseFailed";
    }
    return "?";
}

} // namespace Corsairs::Engine::Render
