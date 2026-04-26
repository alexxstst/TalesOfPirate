// Единственная живая утилита из старого lwFileutil — выделение расширения из пути.
// Все остальные lwGet*/Load*File/lwGetOpenFileName/etc. удалены как dead code.
#pragma once

#include <string>
#include <string_view>

// Возвращает расширение файла без точки, либо пустую строку, если точки нет.
std::string lwGetPathFileNameExt(std::string_view path);
