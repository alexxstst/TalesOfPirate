#include "Cli.h"

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <optional>
#include <string>

namespace pkotool {

namespace {

[[nodiscard]] std::string ToLower(std::string s) {
    for (auto& ch : s) {
        ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
    }
    return s;
}

[[nodiscard]] std::optional<Mode> ParseMode(std::string_view v) {
    if (v == "validate")    return Mode::Validate;
    if (v == "fix")         return Mode::Fix;
    if (v == "export-yml")  return Mode::ExportYml;
    if (v == "export-glb")  return Mode::ExportGlb;
    if (v == "pack-yml")    return Mode::PackYml;
    if (v == "pack-glb")    return Mode::PackGlb;
    return std::nullopt;
}

// Решает, как интерпретировать содержимое --scope: список расширений или список путей.
// Эвристика: если строка содержит '/', '\\' или ':' — это путь (или несколько путей через ';').
[[nodiscard]] bool LooksLikePath(std::string_view s) {
    return s.find('/') != std::string_view::npos
        || s.find('\\') != std::string_view::npos
        || s.find(':') != std::string_view::npos;
}

void ParseScope(std::string_view raw, Scope& out) {
    if (LooksLikePath(raw)) {
        // ';' — разделитель путей (привычно по аналогии с PATH).
        std::size_t start = 0;
        while (start <= raw.size()) {
            const std::size_t end = raw.find(';', start);
            const std::size_t len = (end == std::string_view::npos ? raw.size() : end) - start;
            std::string_view piece{raw.data() + start, len};
            if (!piece.empty()) {
                out.paths.emplace_back(std::filesystem::path{piece});
            }
            if (end == std::string_view::npos) {
                break;
            }
            start = end + 1;
        }
        return;
    }

    // Список расширений: запятые или ';' допустимы.
    std::size_t start = 0;
    while (start <= raw.size()) {
        std::size_t end = raw.size();
        for (std::size_t i = start; i < raw.size(); ++i) {
            if (raw[i] == ',' || raw[i] == ';') {
                end = i;
                break;
            }
        }
        std::string ext{raw.substr(start, end - start)};
        // обрезать пробелы и ведущую точку
        while (!ext.empty() && (ext.front() == ' ' || ext.front() == '\t' || ext.front() == '.')) {
            ext.erase(ext.begin());
        }
        while (!ext.empty() && (ext.back() == ' ' || ext.back() == '\t')) {
            ext.pop_back();
        }
        if (!ext.empty()) {
            out.extensions.push_back(ToLower(std::move(ext)));
        }
        if (end == raw.size()) {
            break;
        }
        start = end + 1;
    }
}

[[nodiscard]] bool StartsWith(std::string_view s, std::string_view prefix) {
    return s.size() >= prefix.size() && s.compare(0, prefix.size(), prefix) == 0;
}

void PrintUsage() {
    std::fprintf(stderr,
        "Usage: pkotool --mode=<mode> --scope=<scope> [options]\n"
        "\n"
        "  --mode=<mode>      validate | fix | export-yml | export-glb | pack-yml | pack-glb\n"
        "                       (export-* and pack-* are not implemented yet)\n"
        "  --scope=<scope>    comma-separated extension list (lgo,lmo,bmp,png),\n"
        "                       or one/several paths separated by ';' (D:/a.lgo;D:/b.lmo).\n"
        "  --root=<dir>       root for recursive scan (default: cwd).\n"
        "  --report=<file>    markdown report path (default: cwd/pkotool-report.md).\n"
        "  --no-console       log to files only, skip console output.\n"
        "  --limit=N          process at most N files (debug aid).\n");
}

} // namespace

std::string_view ToString(Mode m) noexcept {
    switch (m) {
    case Mode::Validate:  return "validate";
    case Mode::Fix:       return "fix";
    case Mode::ExportYml: return "export-yml";
    case Mode::ExportGlb: return "export-glb";
    case Mode::PackYml:   return "pack-yml";
    case Mode::PackGlb:   return "pack-glb";
    }
    return "?";
}

std::optional<Options> ParseCli(int argc, char** argv) {
    Options opt;

    bool modeSet  = false;
    bool scopeSet = false;

    for (int i = 1; i < argc; ++i) {
        std::string_view a = argv[i];

        if (a == "-h" || a == "--help") {
            PrintUsage();
            return std::nullopt;
        }
        else if (StartsWith(a, "--mode=")) {
            auto m = ParseMode(a.substr(7));
            if (!m) {
                std::fprintf(stderr, "[pkotool] unknown --mode: %.*s\n",
                             static_cast<int>(a.size() - 7), a.data() + 7);
                return std::nullopt;
            }
            opt.mode = *m;
            modeSet = true;
        }
        else if (StartsWith(a, "--scope=")) {
            ParseScope(a.substr(8), opt.scope);
            if (opt.scope.extensions.empty() && opt.scope.paths.empty()) {
                std::fprintf(stderr, "[pkotool] empty --scope\n");
                return std::nullopt;
            }
            scopeSet = true;
        }
        else if (StartsWith(a, "--root=")) {
            opt.root = std::filesystem::path{a.substr(7)};
        }
        else if (StartsWith(a, "--report=")) {
            opt.report = std::filesystem::path{a.substr(9)};
        }
        else if (a == "--no-console") {
            opt.consoleOutput = false;
        }
        else if (a == "--console") {
            opt.consoleOutput = true;
        }
        else if (StartsWith(a, "--limit=")) {
            opt.limit = static_cast<std::size_t>(std::stoul(std::string{a.substr(8)}));
        }
        else {
            std::fprintf(stderr, "[pkotool] unknown argument: %.*s\n",
                         static_cast<int>(a.size()), a.data());
            return std::nullopt;
        }
    }

    if (!modeSet) {
        std::fprintf(stderr, "[pkotool] missing --mode\n");
        PrintUsage();
        return std::nullopt;
    }
    if (!scopeSet) {
        std::fprintf(stderr, "[pkotool] missing --scope\n");
        PrintUsage();
        return std::nullopt;
    }

    if (opt.root.empty()) {
        opt.root = std::filesystem::current_path();
    }
    if (opt.report.empty()) {
        opt.report = std::filesystem::current_path() / "pkotool-report.md";
    }

    return opt;
}

} // namespace pkotool
