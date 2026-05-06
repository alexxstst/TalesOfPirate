// PkoTool — утилита для проверки и починки ассетов MindPower3D-движка
// (.lgo / .lmo / .lxo / .lab), а также текстур (.bmp / .png / .jpg / .tga).
//
// Поддерживаемые режимы:
//   --mode=validate       — обходит файлы по --scope, пишет markdown-отчёт.
//   --mode=fix            — destructive: удаляет битые, пересохраняет warning'и.
//                           Требует --confirm, иначе только dry-run.
//   --mode=export-yml     — распаковывает .eff / .par в человекочитаемый YAML
//                           рядом с оригиналом (`a.eff` → `a.eff.yaml`).
//   --mode=export-glb     — заглушка (TODO): экспорт в glTF/GLB.
//   --mode=pack-yml       — собирает .eff / .par обратно из *.eff.yaml /
//                           *.par.yaml (overwrite — пишет `a.eff` рядом с
//                           `a.eff.yaml`).
//   --mode=pack-glb       — заглушка (TODO): конверсия GLB → бинарь.
//
// Точечная распаковка/упаковка одного файла: `--scope=path/to/file.eff`
// (или `.par`/`.yaml`). Список через `;` или каталог + `--scope=eff,par`
// тоже работают.
//
// См. Cli.h::Options и `--help` для полного списка опций.

#include "Cli.h"
#include "Scanner.h"
#include "Validator.h"
#include "Reporter.h"
#include "Fixer.h"

#include "AssetLoaders.h"
#include "MPModelEff.h"      // EffectFileInfo
#include "MPParticleCtrl.h"  // CMPPartCtrl

#include "logutil.h"

#include <Windows.h>
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <filesystem>

namespace {

constexpr const char* kLogChannel = "pkotool";
constexpr const char* kBanner =
    "PkoTool 1.2 created by alexxst.st (alexxst.st@gmail.com)";

namespace fs = std::filesystem;

int RunValidate(const pkotool::Options& opt) {
    auto files = pkotool::CollectFiles(opt.scope, opt.root, opt.limit);
    ToLogService(kLogChannel, LogLevel::Info,
                 "files found: {} (root={})",
                 files.size(), opt.root.string());

    if (files.empty()) {
        ToLogService(kLogChannel, LogLevel::Warning,
                     "scope produced no files");
        return 0;
    }

    std::vector<pkotool::ValidationRecord> records;
    records.reserve(files.size());

    const std::size_t total = files.size();
    const int width = total < 10 ? 1 : (total < 100 ? 2 : (total < 1000 ? 3 : 4));

    std::size_t okCount = 0, warnCount = 0, errCount = 0;
    for (std::size_t i = 0; i < total; ++i) {
        auto rec = pkotool::ValidateFile(files[i]);

        const auto level = (rec.status == pkotool::ValidationStatus::Error)
            ? LogLevel::Error
            : (rec.status == pkotool::ValidationStatus::Warning ? LogLevel::Warning : LogLevel::Trace);

        ToLogService(kLogChannel, level,
                     "[{:>{}}/{}] {} {}: {}",
                     i + 1, width, total,
                     pkotool::ToString(rec.status),
                     rec.file.string(),
                     rec.problem.empty() ? std::string{"(ok)"} : rec.problem);

        switch (rec.status) {
        case pkotool::ValidationStatus::Ok:      ++okCount;   break;
        case pkotool::ValidationStatus::Warning: ++warnCount; break;
        case pkotool::ValidationStatus::Error:   ++errCount;  break;
        }
        records.push_back(std::move(rec));
    }

    if (!pkotool::WriteReport(opt.report, opt, records)) {
        ToLogService(kLogChannel, LogLevel::Error,
                     "failed to write report: {}", opt.report.string());
        return 2;
    }

    ToLogService(kLogChannel, LogLevel::Info,
                 "=== Summary: {} (Ok: {}, Warnings: {}, Errors: {}) ===",
                 records.size(), okCount, warnCount, errCount);
    ToLogService(kLogChannel, LogLevel::Info, "report: {}", opt.report.string());

    return errCount == 0 ? 0 : 1;
}

int RunFix(const pkotool::Options& opt) {
    auto files = pkotool::CollectFiles(opt.scope, opt.root, opt.limit);
    ToLogService(kLogChannel, LogLevel::Info,
                 "files found: {} (root={})",
                 files.size(), opt.root.string());

    std::vector<pkotool::ValidationRecord> records;
    records.reserve(files.size());
    for (const auto& f : files) {
        records.push_back(pkotool::ValidateFile(f));
    }

    auto sum = pkotool::ApplyFix(records);

    ToLogService(kLogChannel, LogLevel::Info,
                 "=== Fix summary: deleted={}, resaved={}, ok={}, unsupported={}, failed={} ===",
                 sum.deleted, sum.resaved, sum.skippedOk,
                 sum.skippedUnsupported, sum.failed);

    // Re-validate after fix so that the report reflects the post-fix state.
    // Удалённые файлы выпадают из отчёта (их больше нет на диске); пересохранённые
    // должны теперь пройти как Ok.
    std::vector<pkotool::ValidationRecord> postRecords;
    postRecords.reserve(records.size());
    for (const auto& r : records) {
        if (r.status == pkotool::ValidationStatus::Error) {
            // файл удалён — пропускаем.
            continue;
        }
        postRecords.push_back(pkotool::ValidateFile(r.file));
    }

    if (!pkotool::WriteReport(opt.report, opt, postRecords)) {
        ToLogService(kLogChannel, LogLevel::Error,
                     "failed to write report: {}", opt.report.string());
        return 2;
    }
    ToLogService(kLogChannel, LogLevel::Info, "report: {}", opt.report.string());
    return sum.failed == 0 ? 0 : 1;
}

int RunNotImplemented(const pkotool::Options& opt) {
    ToLogService(kLogChannel, LogLevel::Error,
                 "mode `{}` is not implemented yet. Use --mode=validate or --mode=fix.",
                 pkotool::ToString(opt.mode));
    return 2;
}

// ----------------------------------------------------------------------------
// YAML pack / unpack
// ----------------------------------------------------------------------------

[[nodiscard]] std::string ToLower(std::string s) {
    for (auto& ch : s) {
        ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
    }
    return s;
}

// Двойное расширение `<name>.eff.yaml` / `<name>.par.yaml` → выводит ".eff"/".par".
// Возвращает пустую строку, если `inner extension` не похож на наш.
[[nodiscard]] std::string DetectInnerExt(const fs::path& yamlFile) {
    const auto stem = yamlFile.stem();
    return ToLower(stem.extension().string());
}

[[nodiscard]] bool IsYamlExt(std::string_view ext) {
    return ext == ".yaml" || ext == ".yml";
}

int RunExportYml(const pkotool::Options& opt) {
    auto files = pkotool::CollectFiles(opt.scope, opt.root, opt.limit);
    ToLogService(kLogChannel, LogLevel::Info,
                 "files found: {} (root={})",
                 files.size(), opt.root.string());

    if (files.empty()) {
        ToLogService(kLogChannel, LogLevel::Warning, "scope produced no files");
        return 0;
    }

    std::size_t okCount = 0, failCount = 0, skipCount = 0;
    const std::size_t total = files.size();
    const int width = total < 10 ? 1 : (total < 100 ? 2 : (total < 1000 ? 3 : 4));

    for (std::size_t i = 0; i < total; ++i) {
        const auto& src = files[i];
        const std::string ext = ToLower(src.extension().string());
        // Пишем рядом: a.eff → a.eff.yaml.  Так pack-yml ниже находит файл по
        // двойному расширению без догадок.
        const fs::path out = src.string() + ".yaml";

        const std::string prefix =
            std::format("[{:>{}}/{}] {}", i + 1, width, total, src.filename().string());

        if (ext == ".eff") {
            EffectFileInfo info;
            Corsairs::Engine::Render::EffectLoadDiagnostics diag;
            if (LW_FAILED(Corsairs::Engine::Render::EffectLoader::LoadEx(
                    info, src.string(), diag))) {
                ToLogService(kLogChannel, LogLevel::Error,
                             "{}: load FAIL ({}: {})",
                             prefix, Corsairs::Engine::Render::ToString(diag.status),
                             diag.detail);
                ++failCount;
                continue;
            }
            if (LW_FAILED(Corsairs::Engine::Render::EffectLoader::ExportToYaml(
                    info, out.string()))) {
                ToLogService(kLogChannel, LogLevel::Error,
                             "{}: export FAIL", prefix);
                ++failCount;
                continue;
            }
            ToLogService(kLogChannel, LogLevel::Info,
                         "{}: eff → {}", prefix, out.string());
            ++okCount;
        }
        else if (ext == ".par") {
            CMPPartCtrl ctrl;
            Corsairs::Engine::Render::PartCtrlLoadDiagnostics diag;
            if (LW_FAILED(Corsairs::Engine::Render::PartCtrlLoader::LoadEx(
                    ctrl, src.string(), diag))) {
                ToLogService(kLogChannel, LogLevel::Error,
                             "{}: load FAIL ({}: {})",
                             prefix, Corsairs::Engine::Render::ToString(diag.status),
                             diag.detail);
                ++failCount;
                continue;
            }
            if (LW_FAILED(Corsairs::Engine::Render::PartCtrlLoader::ExportToYaml(
                    ctrl, out.string()))) {
                ToLogService(kLogChannel, LogLevel::Error,
                             "{}: export FAIL", prefix);
                ++failCount;
                continue;
            }
            ToLogService(kLogChannel, LogLevel::Info,
                         "{}: par → {}", prefix, out.string());
            ++okCount;
        }
        else {
            ToLogService(kLogChannel, LogLevel::Warning,
                         "{}: skip (unsupported ext for export-yml: {})", prefix, ext);
            ++skipCount;
        }
    }

    ToLogService(kLogChannel, LogLevel::Info,
                 "=== Export-yml summary: ok={}, fail={}, skip={} ===",
                 okCount, failCount, skipCount);
    return failCount == 0 ? 0 : 1;
}

int RunPackYml(const pkotool::Options& opt) {
    auto files = pkotool::CollectFiles(opt.scope, opt.root, opt.limit);
    ToLogService(kLogChannel, LogLevel::Info,
                 "files found: {} (root={})",
                 files.size(), opt.root.string());

    if (files.empty()) {
        ToLogService(kLogChannel, LogLevel::Warning, "scope produced no files");
        return 0;
    }

    std::size_t okCount = 0, failCount = 0, skipCount = 0;
    const std::size_t total = files.size();
    const int width = total < 10 ? 1 : (total < 100 ? 2 : (total < 1000 ? 3 : 4));

    for (std::size_t i = 0; i < total; ++i) {
        const auto& yamlPath = files[i];
        const std::string outerExt = ToLower(yamlPath.extension().string());

        const std::string prefix =
            std::format("[{:>{}}/{}] {}", i + 1, width, total,
                        yamlPath.filename().string());

        if (!IsYamlExt(outerExt)) {
            ToLogService(kLogChannel, LogLevel::Warning,
                         "{}: skip (not a yaml file: {})", prefix, outerExt);
            ++skipCount;
            continue;
        }

        const std::string innerExt = DetectInnerExt(yamlPath);
        // Вывод: убрать .yaml — `<name>.eff.yaml` → `<name>.eff`.
        const fs::path outBin = yamlPath.parent_path() / yamlPath.stem();

        if (innerExt == ".eff") {
            EffectFileInfo info;
            if (LW_FAILED(Corsairs::Engine::Render::EffectLoader::ImportFromYaml(
                    info, yamlPath.string()))) {
                ToLogService(kLogChannel, LogLevel::Error,
                             "{}: yaml import FAIL", prefix);
                ++failCount;
                continue;
            }
            if (LW_FAILED(Corsairs::Engine::Render::EffectLoader::Save(
                    info, outBin.string()))) {
                ToLogService(kLogChannel, LogLevel::Error,
                             "{}: save FAIL ({})", prefix, outBin.string());
                ++failCount;
                continue;
            }
            ToLogService(kLogChannel, LogLevel::Info,
                         "{}: eff ← yaml → {}", prefix, outBin.string());
            ++okCount;
        }
        else if (innerExt == ".par") {
            CMPPartCtrl ctrl;
            if (LW_FAILED(Corsairs::Engine::Render::PartCtrlLoader::ImportFromYaml(
                    ctrl, yamlPath.string()))) {
                ToLogService(kLogChannel, LogLevel::Error,
                             "{}: yaml import FAIL", prefix);
                ++failCount;
                continue;
            }
            if (LW_FAILED(Corsairs::Engine::Render::PartCtrlLoader::Save(
                    ctrl, outBin.string()))) {
                ToLogService(kLogChannel, LogLevel::Error,
                             "{}: save FAIL ({})", prefix, outBin.string());
                ++failCount;
                continue;
            }
            ToLogService(kLogChannel, LogLevel::Info,
                         "{}: par ← yaml → {}", prefix, outBin.string());
            ++okCount;
        }
        else {
            ToLogService(kLogChannel, LogLevel::Error,
                         "{}: cannot detect target format from name "
                         "(expect *.eff.yaml or *.par.yaml, inner ext = '{}')",
                         prefix, innerExt);
            ++failCount;
        }
    }

    ToLogService(kLogChannel, LogLevel::Info,
                 "=== Pack-yml summary: ok={}, fail={}, skip={} ===",
                 okCount, failCount, skipCount);
    return failCount == 0 ? 0 : 1;
}

} // namespace

int main(int argc, char** argv) {
    SetConsoleOutputCP(CP_UTF8);

    std::puts(kBanner);

    auto optMaybe = pkotool::ParseCli(argc, argv);
    if (!optMaybe) {
        return 2;
    }
    auto& opt = *optMaybe;

    // Логи рядом с exe (logs/<channel>.YYYYMMDD.log).
    const fs::path projectDir = fs::path{argv[0]}.parent_path();
    const fs::path logsDir = projectDir / "logs";
    g_logManager.InitLogger(logsDir.string());
    g_logManager.EnableGlobalConsole(opt.consoleOutput);
    g_logManager.AddLogger(kLogChannel);
    g_logManager.AddLogger("errors");
    g_logManager.AddLogger("warnings");
    g_logManager.AddLogger("loader");

    ToLogService(kLogChannel, LogLevel::Info,
                 "pkotool start: mode={}, root={}, report={}",
                 pkotool::ToString(opt.mode), opt.root.string(), opt.report.string());

    int rc = 0;
    switch (opt.mode) {
    case pkotool::Mode::Validate:  rc = RunValidate(opt); break;
    case pkotool::Mode::Fix:       rc = RunFix(opt); break;
    case pkotool::Mode::ExportYml: rc = RunExportYml(opt); break;
    case pkotool::Mode::PackYml:   rc = RunPackYml(opt); break;
    case pkotool::Mode::ExportGlb:
    case pkotool::Mode::PackGlb:
        rc = RunNotImplemented(opt);
        break;
    }

    g_logManager.Shutdown();
    return rc;
}
