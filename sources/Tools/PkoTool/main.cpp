// PkoTool — утилита для проверки и починки ассетов MindPower3D-движка
// (.lgo / .lmo / .lxo / .lab), а также текстур (.bmp / .png / .jpg / .tga).
//
// Поддерживаемые режимы:
//   --mode=validate       — обходит файлы по --scope, пишет markdown-отчёт.
//   --mode=fix            — destructive: удаляет битые, пересохраняет warning'и.
//                           Требует --confirm, иначе только dry-run.
//   --mode=export-yml     — заглушка (TODO): экспорт в YAML.
//   --mode=export-glb     — заглушка (TODO): экспорт в glTF/GLB.
//   --mode=pack-yml       — заглушка (TODO): обратная упаковка из YAML.
//   --mode=pack-glb       — заглушка (TODO): конверсия GLB → бинарь.
//
// См. Cli.h::Options и `--help` для полного списка опций.

#include "Cli.h"
#include "Scanner.h"
#include "Validator.h"
#include "Reporter.h"
#include "Fixer.h"

#include "logutil.h"

#include <Windows.h>
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
    case pkotool::Mode::ExportYml: // fallthrough
    case pkotool::Mode::ExportGlb:
    case pkotool::Mode::PackYml:
    case pkotool::Mode::PackGlb:
        rc = RunNotImplemented(opt);
        break;
    }

    g_logManager.Shutdown();
    return rc;
}
