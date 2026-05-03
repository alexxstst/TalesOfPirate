#include "Scanner.h"

#include <algorithm>
#include <cctype>
#include <string>

namespace pkotool {

namespace fs = std::filesystem;

namespace {

[[nodiscard]] std::string ExtLower(const fs::path& p) {
    std::string ext = p.extension().string();
    if (!ext.empty() && ext.front() == '.') {
        ext.erase(ext.begin());
    }
    for (auto& ch : ext) {
        ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
    }
    return ext;
}

[[nodiscard]] bool MatchesAny(const std::string& ext, const std::vector<std::string>& list) {
    return std::find(list.begin(), list.end(), ext) != list.end();
}

} // namespace

std::vector<fs::path> CollectFiles(const Scope& scope, const fs::path& root, std::size_t limit) {
    std::vector<fs::path> result;

    if (!scope.paths.empty()) {
        // Явно указанные пути — берём как есть.
        result = scope.paths;
    }
    else {
        // Рекурсивный обход root.
        std::error_code ec;
        if (!fs::exists(root, ec) || !fs::is_directory(root, ec)) {
            return result;
        }
        for (const auto& entry : fs::recursive_directory_iterator(root,
                                                                  fs::directory_options::skip_permission_denied,
                                                                  ec)) {
            if (ec) {
                ec.clear();
                continue;
            }
            if (!entry.is_regular_file(ec)) {
                continue;
            }
            const std::string ext = ExtLower(entry.path());
            if (ext.empty()) {
                continue;
            }
            if (MatchesAny(ext, scope.extensions)) {
                result.push_back(entry.path());
                if (limit > 0 && result.size() >= limit) {
                    break;
                }
            }
        }
        std::sort(result.begin(), result.end());
    }

    if (limit > 0 && result.size() > limit) {
        result.resize(limit);
    }
    return result;
}

} // namespace pkotool
