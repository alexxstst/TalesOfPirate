#include "Reporter.h"

#include <chrono>
#include <format>
#include <fstream>

namespace pkotool {

namespace fs = std::filesystem;

namespace {

[[nodiscard]] std::string CurrentTimestamp() {
    const auto now = std::chrono::system_clock::now();
    const auto t  = std::chrono::system_clock::to_time_t(now);
    std::tm tm{};
#if defined(_WIN32)
    localtime_s(&tm, &t);
#else
    localtime_r(&t, &tm);
#endif
    return std::format("{:04}-{:02}-{:02} {:02}:{:02}:{:02}",
                       tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
                       tm.tm_hour, tm.tm_min, tm.tm_sec);
}

[[nodiscard]] std::string EscapeMarkdownCell(std::string s) {
    // Заменяем символы, ломающие markdown-таблицу: '|', '\n'.
    for (auto& ch : s) {
        if (ch == '|') ch = '\\';
        if (ch == '\n' || ch == '\r') ch = ' ';
    }
    return s;
}

void WriteSection(std::ofstream& f,
                  std::string_view title,
                  const std::vector<const ValidationRecord*>& items,
                  bool includeProblemColumns) {
    f << "## " << title << " (" << items.size() << ")\n\n";
    if (items.empty()) {
        f << "_no entries_\n\n";
        return;
    }
    if (includeProblemColumns) {
        f << "| File | Ext | Version | Problem | Recommendation |\n";
        f << "|------|-----|--------:|---------|----------------|\n";
    }
    else {
        f << "| File | Ext | Version |\n";
        f << "|------|-----|--------:|\n";
    }
    for (const auto* r : items) {
        f << "| `" << EscapeMarkdownCell(r->file.string()) << "` "
          << "| " << r->extension << " "
          << "| " << (r->version != 0 ? std::format("0x{:04X}", r->version) : std::string{"-"});
        if (includeProblemColumns) {
            f << " | " << EscapeMarkdownCell(r->problem)
              << " | " << EscapeMarkdownCell(r->recommendation);
        }
        f << " |\n";
    }
    f << "\n";
}

[[nodiscard]] std::string ScopeToString(const Scope& s) {
    if (!s.paths.empty()) {
        std::string out = "paths(";
        out += std::to_string(s.paths.size());
        out += ")";
        return out;
    }
    std::string out;
    for (std::size_t i = 0; i < s.extensions.size(); ++i) {
        if (i > 0) out += ", ";
        out += s.extensions[i];
    }
    return out;
}

} // namespace

bool WriteReport(const fs::path& reportPath, const Options& opt,
                 const std::vector<ValidationRecord>& records) {
    std::error_code ec;
    fs::create_directories(reportPath.parent_path(), ec);

    std::ofstream f{reportPath, std::ios::out | std::ios::trunc};
    if (!f) {
        return false;
    }

    std::vector<const ValidationRecord*> errors, warnings, oks;
    for (const auto& r : records) {
        switch (r.status) {
        case ValidationStatus::Error:   errors.push_back(&r); break;
        case ValidationStatus::Warning: warnings.push_back(&r); break;
        case ValidationStatus::Ok:      oks.push_back(&r); break;
        }
    }

    f << "# PkoTool report\n\n"
      << "- Mode: `" << ToString(opt.mode) << "`\n"
      << "- Scope: " << ScopeToString(opt.scope) << "\n"
      << "- Root: `" << opt.root.string() << "`\n"
      << "- Generated: " << CurrentTimestamp() << "\n"
      << "- Total: " << records.size()
      << " (Ok: "       << oks.size()
      << ", Warnings: " << warnings.size()
      << ", Errors: "   << errors.size() << ")\n\n";

    WriteSection(f, "Errors",   errors,   /*includeProblemColumns=*/true);
    WriteSection(f, "Warnings", warnings, /*includeProblemColumns=*/true);
    WriteSection(f, "Ok",       oks,      /*includeProblemColumns=*/false);

    return f.good();
}

} // namespace pkotool
