#pragma once

#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

namespace pkotool {

enum class Mode {
    Validate,
    Fix,
    ExportYml,
    ExportGlb,
    PackYml,
    PackGlb,
};

struct Scope {
    // Один из двух режимов:
    //  - extensions != пусто, paths == пусто: рекурсивный обход root + фильтр по расширениям;
    //  - paths != пусто, extensions == пусто: только перечисленные файлы (root игнорируется).
    std::vector<std::string>             extensions;  // lowercase, без точки: "lgo", "lmo", "lxo", "lab", "bmp", "png"
    std::vector<std::filesystem::path>   paths;       // абсолютные пути, явно указанные в --scope
};

struct Options {
    Mode                       mode{Mode::Validate};
    Scope                      scope;
    std::filesystem::path      root;            // default: current_path()
    std::filesystem::path      report;          // default: cwd / "pkotool-report.md"
    bool                       consoleOutput{true};
    std::size_t                limit{0};        // 0 = без лимита
};

// Парсит argv. При ошибке формата — пишет в stderr и возвращает std::nullopt.
// Возвращаемое значение — Options (заполнено) или std::nullopt (тогда нужно завершиться с кодом 2).
[[nodiscard]] std::optional<Options> ParseCli(int argc, char** argv);

[[nodiscard]] std::string_view ToString(Mode m) noexcept;

} // namespace pkotool
