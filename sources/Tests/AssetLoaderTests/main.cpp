// Интеграционный тест round-trip загрузки .lmo через
// Corsairs::Engine::Render::LgoLoader.
//
// Алгоритм:
//   1. Найти repo root (поиск вверх по дереву, маркер — Client/model/character/).
//   2. Скопировать Client/model/scene/*.lmo (array lwModelObjInfo) в
//      bin/runs/source/scene/.
//   3. Загрузить каждую копию через LgoLoader::LoadModelObjEx — собрать список неудач.
//   4. Удачно загруженные пересохранить через LgoLoader::SaveModelObj в
//      bin/runs/saved/scene/.
//   5. Сравнить пары source vs saved побайтово — сообщить о расхождениях.
//
// Round-trip для .lgo (Client/model/{character,effect,item}/*.lgo) временно
// выключен; пайплайн обобщён под `AssetKind`, так что включить .lgo обратно —
// это вернуть `lgoKind` и второй вызов RunRoundTripPipeline.
//
// CLI:
//   AssetLoaderTests.exe [repo_root] [--limit N] [--console|--no-console]
//                        [--nocopy] [--remove_ok]
//     repo_root      — корень проекта (по умолчанию ищется вверх от cwd)
//     --limit N      — обработать первые N файлов в каждой категории (для отладки)
//     --console      — дублировать логи в консоль (по умолчанию)
//     --no-console   — писать только в файлы logs/<channel>.YYYYMMDD.log
//     --nocopy       — не очищать runs/ и не копировать заново; перебираем то,
//                      что уже лежит в runs/source/<category>/. Полезно вместе
//                      с --remove_ok: на повторных запусках в runs/ остаются
//                      только проблемные файлы.
//     --remove_ok    — после полностью успешного прогона (load OK + save OK +
//                      round-trip совпал, либо load OK + save OK для legacy-
//                      файла) удалять обе копии (source + saved), чтобы в
//                      runs/ оседали только файлы, требующие разбора.
//                      Файлы с trailer-warning, load-fail, save-fail и
//                      round-trip-diff остаются на диске.

#include <Windows.h>

#include <algorithm>
#include <array>
#include <cstdint>
#include <cstdio>
#include <filesystem>
#include <format>
#include <fstream>
#include <functional>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "AssetLoaders.h"
#include "logutil.h"
#include "lwExpObj.h"

namespace {
constexpr const char* kLogChannel = "asset_loader_tests";
} // namespace

namespace fs = std::filesystem;

namespace {

struct LoadFailure {
    fs::path file;
};

struct CompareFailure {
    fs::path source;
    fs::path saved;
    std::uintmax_t sourceSize{};
    std::uintmax_t savedSize{};
    std::uint64_t firstDiffOffset{};
};

struct AssetKindStats {
    std::size_t copied = 0;
    std::size_t loaded = 0;
    std::size_t saved = 0;
    std::size_t roundtripOk = 0;
    std::size_t legacySkipped = 0;
    std::size_t trailerWarnings = 0;
    std::vector<LoadFailure> loadFailures;
    std::vector<CompareFailure> compareFailures;
};

// Описание одного вида ассета, прогоняемого через round-trip.
//   label       — короткий идентификатор для логов и summary ("lgo" / "lmo").
//   extensions  — расширения, по которым EnumerateAssets фильтрует файлы
//                 (lower- и UPPER-варианты, как в исходных директориях).
//   categories  — поддиректории Client/model/<cat>/, где лежит этот вид.
//   processOne  — load+save одного файла. Возвращает nullopt, если Load*Ex
//                 провалился (diag заполнен), иначе LW_RESULT от Save*. Лямбда
//                 сама владеет in-memory структурой и освобождает её до return.
struct AssetKind {
    std::string_view label;
    std::vector<std::string_view> extensions;
    std::vector<std::string_view> categories;
    std::function<std::optional<LW_RESULT>(const std::string&,
                                            const std::string&,
                                            Corsairs::Engine::Render::LgoLoadDiagnostics&)>
        processOne;
};

[[nodiscard]] fs::path FindRepoRoot() {
    fs::path cur = fs::current_path();
    for (int hops = 0; hops < 12; ++hops) {
        if (fs::exists(cur / "Client" / "model" / "character")) {
            return cur;
        }
        if (!cur.has_parent_path() || cur == cur.parent_path()) {
            break;
        }
        cur = cur.parent_path();
    }
    return {};
}

[[nodiscard]] std::optional<std::uint32_t> ReadAssetVersion(const fs::path& p) {
    std::ifstream f{p, std::ios::binary};
    if (!f) {
        return std::nullopt;
    }
    std::uint32_t v = 0;
    f.read(reinterpret_cast<char*>(&v), sizeof(v));
    if (f.gcount() != static_cast<std::streamsize>(sizeof(v))) {
        return std::nullopt;
    }
    return v;
}

[[nodiscard]] std::vector<fs::path> EnumerateAssets(const fs::path& dir,
                                                     std::span<const std::string_view> extensions) {
    std::vector<fs::path> result;
    if (!fs::exists(dir)) {
        return result;
    }
    for (const auto& entry : fs::directory_iterator{dir}) {
        if (!entry.is_regular_file()) {
            continue;
        }
        const auto ext = entry.path().extension().string();
        const bool match = std::any_of(extensions.begin(), extensions.end(),
                                       [&](std::string_view e) { return ext == e; });
        if (match) {
            result.push_back(entry.path());
        }
    }
    std::sort(result.begin(), result.end());
    return result;
}

[[nodiscard]] bool BinariesEqual(const fs::path& a, const fs::path& b, std::uint64_t& diffOffset,
                                 std::uintmax_t& aSize, std::uintmax_t& bSize) {
    aSize = fs::file_size(a);
    bSize = fs::file_size(b);
    if (aSize != bSize) {
        diffOffset = std::min<std::uintmax_t>(aSize, bSize);
        return false;
    }

    std::ifstream af{a, std::ios::binary};
    std::ifstream bf{b, std::ios::binary};
    if (!af || !bf) {
        diffOffset = 0;
        return false;
    }

    constexpr std::size_t kBuf = 64 * 1024;
    std::vector<char> abuf(kBuf);
    std::vector<char> bbuf(kBuf);
    std::uint64_t pos = 0;
    while (af && bf) {
        af.read(abuf.data(), static_cast<std::streamsize>(kBuf));
        bf.read(bbuf.data(), static_cast<std::streamsize>(kBuf));
        const auto an = af.gcount();
        const auto bn = bf.gcount();
        if (an != bn) {
            diffOffset = pos + std::min<std::streamsize>(an, bn);
            return false;
        }
        for (std::streamsize i = 0; i < an; ++i) {
            if (abuf[i] != bbuf[i]) {
                diffOffset = pos + static_cast<std::uint64_t>(i);
                return false;
            }
        }
        pos += static_cast<std::uint64_t>(an);
        if (an == 0) {
            break;
        }
    }
    return true;
}

void RunRoundTripPipeline(const AssetKind& kind,
                          const fs::path& repoRoot,
                          const fs::path& sourceRoot,
                          const fs::path& savedRoot,
                          std::size_t limit,
                          bool noCopy,
                          bool removeOk,
                          AssetKindStats& stats) {
    std::error_code ec;
    for (const auto category : kind.categories) {
        const fs::path src = repoRoot / "Client" / "model" / category;
        const fs::path catSource = sourceRoot / category;
        const fs::path catSaved = savedRoot / category;
        fs::create_directories(catSource, ec);
        fs::create_directories(catSaved, ec);

        // С --nocopy входной список — то, что лежит в runs/source/<category>/
        // (то есть результат прошлого прогона, возможно отфильтрованный
        // --remove_ok). Иначе — оригиналы из Client/model/<category>/.
        auto files = EnumerateAssets(noCopy ? catSource : src,
                                     std::span{kind.extensions});
        const std::size_t available = files.size();
        if (limit > 0 && files.size() > limit) {
            files.resize(limit);
        }
        ToLogService(kLogChannel, LogLevel::Info,
                     "[{}/{}] {} файлов (из {})", kind.label, category,
                     files.size(), available);

        const std::size_t total = files.size();
        const int width = total < 10 ? 1 : (total < 100 ? 2 : (total < 1000 ? 3 : 4));

        for (std::size_t idx = 0; idx < total; ++idx) {
            const auto& original = files[idx];
            const auto fileName = original.filename();
            const fs::path copyPath = catSource / fileName;
            const fs::path savePath = catSaved / fileName;

            const std::string prefix = std::format("[{:>{}}/{}] {}/{}/{}",
                                                   idx + 1, width, total,
                                                   kind.label, category, fileName.string());

            if (!noCopy) {
                fs::copy_file(original, copyPath, fs::copy_options::overwrite_existing, ec);
                if (ec) {
                    ToLogService(kLogChannel, LogLevel::Error,
                                 "{}: copy=FAIL ({})", prefix, ec.message());
                    ec.clear();
                    continue;
                }
            }
            ++stats.copied;

            const auto versionOpt = ReadAssetVersion(copyPath);
            const std::string versionStr = versionOpt
                ? std::format("version=0x{:04X}", *versionOpt)
                : std::string{"version=?????"};

            Corsairs::Engine::Render::LgoLoadDiagnostics diag;
            const auto saveRetOpt = kind.processOne(copyPath.string(), savePath.string(), diag);

            if (!saveRetOpt) {
                ToLogService(kLogChannel, LogLevel::Error,
                             "{}: {} load=FAIL ({}: {})",
                             prefix, versionStr,
                             Corsairs::Engine::Render::ToString(diag.status),
                             diag.detail);
                stats.loadFailures.push_back({copyPath});
                continue;
            }
            ++stats.loaded;

            const LW_RESULT saveRet = *saveRetOpt;
            if (LW_FAILED(saveRet)) {
                ToLogService(kLogChannel, LogLevel::Error,
                             "{}: {} load=OK save=FAIL (ret={})",
                             prefix, versionStr, static_cast<long long>(saveRet));
                continue;
            }
            ++stats.saved;

            // Save всегда пишет в актуальной версии (EXP_OBJ_VERSION). Для файлов
            // старее текущей формат меняется → байтовое сравнение бессмысленно.
            if (versionOpt && *versionOpt < MindPower::EXP_OBJ_VERSION) {
                ++stats.legacySkipped;
                if (removeOk) {
                    fs::remove(copyPath, ec);
                    ec.clear();
                    fs::remove(savePath, ec);
                    ec.clear();
                }
                continue;
            }

            // Файлы с трейлером: парсер прочитал заявленные блоки, остаток
            // (vertex weights / extension format) ушёл «в никуда». Save пишет
            // только то, что есть в info — итоговый размер заведомо меньше
            // оригинала. Бинарное сравнение бессмысленно, печатаем дельту.
            if (diag.status == Corsairs::Engine::Render::LgoLoadStatus::OkWithTrailingData) {
                ++stats.trailerWarnings;
                std::error_code sec;
                const std::uintmax_t srcSize = fs::file_size(copyPath, sec);
                const std::uintmax_t savedSize = fs::file_size(savePath, sec);
                const std::int64_t delta = static_cast<std::int64_t>(savedSize) - static_cast<std::int64_t>(srcSize);
                ToLogService(kLogChannel, LogLevel::Warning,
                             "{}: {} load=OK-with-warning save=OK ({}) src={} saved={} delta={:+}",
                             prefix, versionStr, diag.detail, srcSize, savedSize, delta);
                continue;
            }

            std::uint64_t diffOffset = 0;
            std::uintmax_t srcSize = 0;
            std::uintmax_t savedSize = 0;
            if (BinariesEqual(copyPath, savePath, diffOffset, srcSize, savedSize)) {
                ++stats.roundtripOk;
                if (removeOk) {
                    fs::remove(copyPath, ec);
                    ec.clear();
                    fs::remove(savePath, ec);
                    ec.clear();
                }
            }
            else {
                stats.compareFailures.push_back({copyPath, savePath, srcSize, savedSize, diffOffset});
                ToLogService(kLogChannel, LogLevel::Error,
                             "{}: {} load=OK save=OK roundtrip=DIFF (src={}, saved={}, off={})",
                             prefix, versionStr, srcSize, savedSize, diffOffset);
            }
        }
    }
}

void PrintSummary(std::string_view label, const AssetKindStats& stats) {
    ToLogService(kLogChannel, LogLevel::Info, "=== Итоги [{}] ===", label);
    ToLogService(kLogChannel, LogLevel::Info, "  Скопировано:        {}", stats.copied);
    ToLogService(kLogChannel, LogLevel::Info,
                 "  Загружено:          {} (failures: {})",
                 stats.loaded, stats.loadFailures.size());
    ToLogService(kLogChannel, LogLevel::Info, "  Сохранено:          {}", stats.saved);
    ToLogService(kLogChannel, LogLevel::Info,
                 "  Round-trip пропущен: {} (legacy < 0x{:04X})",
                 stats.legacySkipped, static_cast<unsigned>(MindPower::EXP_OBJ_VERSION));
    ToLogService(kLogChannel, LogLevel::Info,
                 "  С warning (trailer): {} (success, но при пересохранении часть данных теряется)",
                 stats.trailerWarnings);
    ToLogService(kLogChannel, LogLevel::Info,
                 "  Round-trip совпал:   {} (failures: {})",
                 stats.roundtripOk, stats.compareFailures.size());

    if (!stats.loadFailures.empty()) {
        ToLogService(kLogChannel, LogLevel::Info,
                     "  --- Load failures ({}) ---", stats.loadFailures.size());
        for (const auto& f : stats.loadFailures) {
            ToLogService(kLogChannel, LogLevel::Info, "    {}", f.file.string());
        }
    }

    if (!stats.compareFailures.empty()) {
        ToLogService(kLogChannel, LogLevel::Info,
                     "  --- Round-trip mismatches ({}) ---", stats.compareFailures.size());
        std::size_t shown = 0;
        for (const auto& f : stats.compareFailures) {
            ToLogService(kLogChannel, LogLevel::Info,
                         "    {}: src={} bytes, saved={} bytes, first diff @ offset {}",
                         f.source.filename().string(),
                         f.sourceSize, f.savedSize, f.firstDiffOffset);
            if (++shown >= 50 && stats.compareFailures.size() > 50) {
                ToLogService(kLogChannel, LogLevel::Info,
                             "    ... ещё {}", stats.compareFailures.size() - shown);
                break;
            }
        }
    }
}

} // namespace

int main(int argc, char** argv) {
    SetConsoleOutputCP(CP_UTF8);

    using LgoLoader = Corsairs::Engine::Render::LgoLoader;

    fs::path repoRootArg;
    std::size_t limit = 0;
    bool consoleOutput = true;  // По умолчанию дублируем логи в консоль.
    bool noCopy = false;
    bool removeOk = false;
    for (int i = 1; i < argc; ++i) {
        const std::string_view a = argv[i];
        if (a == "--limit" && i + 1 < argc) {
            limit = static_cast<std::size_t>(std::stoul(argv[++i]));
        }
        else if (a.starts_with("--limit=")) {
            limit = static_cast<std::size_t>(std::stoul(std::string{a.substr(8)}));
        }
        else if (a == "--console") {
            consoleOutput = true;
        }
        else if (a == "--no-console") {
            consoleOutput = false;
        }
        else if (a == "--nocopy" || a == "--no-copy") {
            noCopy = true;
        }
        else if (a == "--remove_ok" || a == "--remove-ok") {
            removeOk = true;
        }
        else if (repoRootArg.empty()) {
            repoRootArg = fs::path{a};
        }
        else {
            // Логгер ещё не инициализирован — выводим напрямую в stderr.
            std::fprintf(stderr, "[AssetLoaderTests] Неизвестный аргумент: %.*s\n",
                         static_cast<int>(a.size()), a.data());
            return 2;
        }
    }

    // Инициализация логгера: каталог logs/ рядом с exe-файлом, дублирование
    // в консоль управляется флагом --console / --no-console.
    const fs::path projectDir = fs::path{argv[0]}.parent_path();
    const fs::path logsDir = projectDir / "logs";
    g_logManager.InitLogger(logsDir.string());
    g_logManager.EnableGlobalConsole(consoleOutput);
    g_logManager.AddLogger(kLogChannel);
    g_logManager.AddLogger("errors");
    g_logManager.AddLogger("warnings");
    g_logManager.AddLogger("loader");

    const fs::path repoRoot = repoRootArg.empty() ? FindRepoRoot() : repoRootArg;
    if (repoRoot.empty()) {
        ToLogService(kLogChannel, LogLevel::Error,
                     "Не удалось найти repo root (Client/model/character/ не найден). "
                     "Запустите из корня проекта или передайте путь первым аргументом.");
        g_logManager.Shutdown();
        return 2;
    }

    const fs::path runsDir = projectDir / "runs";
    const fs::path sourceRoot = runsDir / "source";
    const fs::path savedRoot = runsDir / "saved";

    std::error_code ec;
    if (!noCopy) {
        // Полный сброс прошлого прогона: удаляем и source/, и saved/.
        fs::remove_all(runsDir, ec);
    }
    else {
        // Источник не трогаем — берём то, что уцелело от прошлого прогона
        // (например, после --remove_ok). saved/ всё равно перезаписывается
        // Save'ом, но безопаснее очистить — отчёт по дельте/diff станет
        // соответствовать только текущему запуску.
        fs::remove_all(savedRoot, ec);
    }
    fs::create_directories(sourceRoot, ec);
    fs::create_directories(savedRoot, ec);

    ToLogService(kLogChannel, LogLevel::Info, "repo = {}", repoRoot.string());
    ToLogService(kLogChannel, LogLevel::Info, "runs = {}", runsDir.string());
    ToLogService(kLogChannel, LogLevel::Info, "logs = {}", logsDir.string());
    if (limit > 0) {
        ToLogService(kLogChannel, LogLevel::Info, "limit = {} файлов на категорию", limit);
    }
    if (noCopy) {
        ToLogService(kLogChannel, LogLevel::Info,
                     "nocopy: перебираем существующие файлы из {} (Client/model/* не читается)",
                     sourceRoot.string());
    }
    if (removeOk) {
        ToLogService(kLogChannel, LogLevel::Info,
                     "remove_ok: успешные round-trip / legacy-файлы будут удаляться "
                     "из source/ и saved/ — в runs/ останутся только проблемные");
    }

    // .lmo: array-based lwModelObjInfo (LoadModelObjEx + SaveModelObj). Само
    // info живёт на стеке — деструктор зачистит geom_obj_seq[].
    const AssetKind lmoKind{
        .label = "lmo",
        .extensions = {".lmo", ".LMO"},
        .categories = {"scene"},
        .processOne = [](const std::string& srcPath,
                          const std::string& savePath,
                          Corsairs::Engine::Render::LgoLoadDiagnostics& diag)
                          -> std::optional<LW_RESULT> {
            MindPower::lwModelObjInfo info;
            const LW_RESULT loadRet = LgoLoader::LoadModelObjEx(info, srcPath, diag);
            if (LW_FAILED(loadRet)) {
                return std::nullopt;
            }
            return LgoLoader::SaveModelObj(info, savePath);
        }};

    AssetKindStats lmoStats;
    RunRoundTripPipeline(lmoKind, repoRoot, sourceRoot, savedRoot,
                         limit, noCopy, removeOk, lmoStats);

    PrintSummary("lmo", lmoStats);

    const bool allOk = lmoStats.loadFailures.empty()
                       && lmoStats.compareFailures.empty()
                       && lmoStats.copied > 0;
    
    Sleep(1000 * 5);
	g_logManager.Shutdown();
    return allOk ? 0 : 1;
}
