#pragma once

#include "Cli.h"
#include "Validator.h"

#include <filesystem>
#include <vector>

namespace pkotool {

// Записывает markdown-отчёт по результатам валидации.
// Возвращает true при успехе, false при ошибке записи.
bool WriteReport(const std::filesystem::path& reportPath,
                 const Options& opt,
                 const std::vector<ValidationRecord>& records);

} // namespace pkotool
