#pragma once

#include "Validator.h"

#include <cstddef>
#include <vector>

namespace pkotool {

struct FixSummary {
    std::size_t deleted{0};        // Error → файл удалён
    std::size_t resaved{0};        // Warning → .bak + Save
    std::size_t skippedOk{0};      // Ok → не трогаем
    std::size_t skippedUnsupported{0};  // расширение, для которого fix не реализован
    std::size_t failed{0};         // действие не удалось (rename/save/remove)
};

// Применяет fix-операции в соответствии с записями. Всегда реально пишет на диск.
FixSummary ApplyFix(const std::vector<ValidationRecord>& records);

} // namespace pkotool
