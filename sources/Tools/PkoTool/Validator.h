#pragma once

#include <cstdint>
#include <filesystem>
#include <string>
#include <string_view>

namespace pkotool {

enum class ValidationStatus {
    Ok,
    Warning,  // файл валиден, но при пересохранении изменится (например, trailing data)
    Error,    // файл повреждён или нечитаем
};

struct ValidationRecord {
    std::filesystem::path file;
    ValidationStatus      status{ValidationStatus::Ok};
    std::uint32_t         version{0};        // 0 если неприменимо (например, .bmp/.png)
    std::string           problem;            // пусто для Ok
    std::string           recommendation;     // пусто для Ok
    std::string           extension;          // lowercase, для группировки/фильтра
};

[[nodiscard]] std::string_view ToString(ValidationStatus s) noexcept;

// Диспатч по расширению файла. Возвращает запись с заполненным статусом.
[[nodiscard]] ValidationRecord ValidateFile(const std::filesystem::path& file);

} // namespace pkotool
