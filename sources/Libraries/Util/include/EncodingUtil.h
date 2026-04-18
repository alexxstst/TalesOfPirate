// EncodingUtil — утилиты конвертации CP_ACP ↔ UTF-8 ↔ wide для миграции
// клиента Tales of Pirate на CP_UTF8 везде.
//
// Использование (примеры):
//   std::string utf8 = encoding::AnsiToUtf8(ansiFromWmChar);
//   encoding::AppendAnsiByteAsUtf8(byteFromWmChar, targetUtf8String);
//   encoding::PopLastUtf8Codepoint(utf8);          // бэкспейс
//   bool isUtf8Starter = encoding::IsUtf8StartByte(b);
//
// Все функции принимают std::string_view / std::string; hex-dump первых байт
// удобен для разведки формата сетевых пакетов.
#pragma once

#include <cstdint>
#include <string>
#include <string_view>

namespace encoding {

// --- UTF-8 byte classification -------------------------------------------

// True если байт — starter (0xxxxxxx ASCII или 11xxxxxx multi-byte lead).
// False для continuation-байтов 10xxxxxx.
inline bool IsUtf8StartByte(unsigned char b) noexcept {
    return (b & 0xC0) != 0x80;
}

// --- Конвертации ---------------------------------------------------------

// Полная строка CP_ACP → UTF-8 (через Wide). Пустой on error.
std::string AnsiToUtf8(std::string_view ansi);

// Полная строка UTF-8 → CP_ACP (через Wide). Пустой on error.
std::string Utf8ToAnsi(std::string_view utf8);

// UTF-16 (wide) ↔ UTF-8. Используются на границах Windows-W-API
// (ImmGetCompositionStringW, MessageBoxW, сtd.) и SQLite wchar-ошибок.
std::string  WideToUtf8(std::wstring_view wide);
std::wstring Utf8ToWide(std::string_view utf8);

// Один byte CP_ACP → UTF-8 bytes, дописать в out. Single-byte ASCII (<0x80)
// копируется как есть; high-bit байт (0x80..0xFF) конвертируется через
// MultiByteToWideChar(CP_ACP).
//
// Для DBCS-lead байтов (CJK в CP_ACP) одного byte недостаточно — потребуется
// накопить trail-byte и вызывать AnsiToUtf8 от двух byte. Для single-byte
// locale (CP1251/русская) достаточно этой функции.
void AppendAnsiByteAsUtf8(unsigned char byte, std::string& out);

// --- UTF-8 операции на std::string --------------------------------------

// Убрать последний UTF-8 codepoint из строки (эквивалент "удалить один символ
// для бэкспейса"). Удаляет все continuation-байты 10xxxxxx до starter-байта
// включительно. Пустая строка — no-op.
void PopLastUtf8Codepoint(std::string& s);

// Case-insensitive equality для ASCII-only строк. Заменитель legacy
// `_stricmp(a, b) == 0`. Для не-ASCII (кириллица/CJK) регистр игнорируется
// только по ASCII-маске — если нужно полноценное Unicode case-folding,
// используйте std::wstring + towlower.
bool EqualsIgnoreCaseAscii(std::string_view a, std::string_view b) noexcept;

// Hex-дамп первых `maxBytes` байт для логирования сетевых payload'ов.
// Формат: "48 65 6c 6c 6f ..." (маленькие буквы, пробел-разделённые).
std::string HexDump(std::string_view bytes, std::size_t maxBytes = 64);

}  // namespace encoding
