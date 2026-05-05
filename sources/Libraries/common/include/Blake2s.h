#pragma once

// BLAKE2s — самостоятельная реализация по RFC 7693.
// Размер дайджеста зафиксирован 32 байта (full BLAKE2s), keyed-режим не нужен
// для нашего use case'а (хеш пароля в HashPassword).
//
// Заменяет crypto::Blake2sHex из CryptoPP-фасада: bit-for-bit совместима,
// проверено через test vectors из RFC 7693 Appendix B.

#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>

namespace Corsairs::Common::Crypto {

// Полный 32-байтный BLAKE2s digest.
void Blake2s(const std::uint8_t* in, std::size_t inLen, std::uint8_t out[32]);

// 64-символьная hex-строка (uppercase). Точная замена прежнего
// crypto::Blake2sHex(input) — выход побайтово идентичен.
[[nodiscard]] std::string Blake2sHex(std::string_view input);

} // namespace Corsairs::Common::Crypto
