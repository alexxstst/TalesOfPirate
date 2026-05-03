#pragma once

#include "Cli.h"

#include <filesystem>
#include <vector>

namespace pkotool {

// Возвращает плоский список путей к файлам, попадающих в скоуп.
// - Если scope.paths не пуст — возвращает их (после нормализации, без проверки существования).
// - Иначе рекурсивно обходит root и фильтрует по scope.extensions.
std::vector<std::filesystem::path> CollectFiles(const Scope& scope,
                                                const std::filesystem::path& root,
                                                std::size_t limit);

} // namespace pkotool
