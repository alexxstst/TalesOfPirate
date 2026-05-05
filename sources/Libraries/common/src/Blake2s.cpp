// BLAKE2s по RFC 7693. Самостоятельная реализация — никаких внешних
// зависимостей кроме <cstdint>/<cstring>/<string>.
//
// Алгоритм описан в RFC 7693 (https://datatracker.ietf.org/doc/html/rfc7693),
// reference-код в Appendix C — public domain (CC0). Эта файловая копия
// упрощена под нужды проекта: фиксированный 32-байтный output, без keyed-режима,
// без personalization/salt-параметров.
//
// Замена для CryptoPP::BLAKE2s (старый crypto::Blake2sHex). Тесты-векторы из
// RFC 7693 Appendix B, см. AssetLoaderTests --self-test-blake2s.

#include "Blake2s.h"

#include <array>
#include <cstring>

namespace Corsairs::Common::Crypto {
namespace {

// IV из SHA-256 (RFC 7693 §2.6, RFC 6234).
constexpr std::array<std::uint32_t, 8> kIV = {
    0x6A09E667u, 0xBB67AE85u, 0x3C6EF372u, 0xA54FF53Au,
    0x510E527Fu, 0x9B05688Cu, 0x1F83D9ABu, 0x5BE0CD19u
};

// Sigma — таблица перестановок индексов message words (RFC 7693 §2.7).
// BLAKE2s использует 10 раундов (BLAKE2b — 12). Каждая строка — перестановка
// 16 индексов; раунды переиспользуют таблицу циклически (для BLAKE2s ровно 10).
constexpr std::uint8_t kSigma[10][16] = {
    {  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15 },
    { 14, 10,  4,  8,  9, 15, 13,  6,  1, 12,  0,  2, 11,  7,  5,  3 },
    { 11,  8, 12,  0,  5,  2, 15, 13, 10, 14,  3,  6,  7,  1,  9,  4 },
    {  7,  9,  3,  1, 13, 12, 11, 14,  2,  6,  5, 10,  4,  0, 15,  8 },
    {  9,  0,  5,  7,  2,  4, 10, 15, 14,  1, 11, 12,  6,  8,  3, 13 },
    {  2, 12,  6, 10,  0, 11,  8,  3,  4, 13,  7,  5, 15, 14,  1,  9 },
    { 12,  5,  1, 15, 14, 13,  4, 10,  0,  7,  6,  3,  9,  2,  8, 11 },
    { 13, 11,  7, 14, 12,  1,  3,  9,  5,  0, 15,  4,  8,  6,  2, 10 },
    {  6, 15, 14,  9, 11,  3,  0,  8, 12,  2, 13,  7,  1,  4, 10,  5 },
    { 10,  2,  8,  4,  7,  6,  1,  5, 15, 11,  9, 14,  3, 12, 13,  0 }
};

constexpr std::size_t kBlockSize = 64;
constexpr std::size_t kDigestSize = 32;

[[nodiscard]] constexpr std::uint32_t Rotr32(std::uint32_t x, unsigned r) {
    return (x >> r) | (x << (32 - r));
}

// Read little-endian 32-bit word из буфера.
[[nodiscard]] std::uint32_t LoadU32Le(const std::uint8_t* p) {
    return static_cast<std::uint32_t>(p[0])
         | (static_cast<std::uint32_t>(p[1]) << 8)
         | (static_cast<std::uint32_t>(p[2]) << 16)
         | (static_cast<std::uint32_t>(p[3]) << 24);
}

// Mixing function G (RFC 7693 §3.1).
inline void G(std::uint32_t v[16], int a, int b, int c, int d,
              std::uint32_t x, std::uint32_t y) {
    v[a] = v[a] + v[b] + x;
    v[d] = Rotr32(v[d] ^ v[a], 16);
    v[c] = v[c] + v[d];
    v[b] = Rotr32(v[b] ^ v[c], 12);
    v[a] = v[a] + v[b] + y;
    v[d] = Rotr32(v[d] ^ v[a], 8);
    v[c] = v[c] + v[d];
    v[b] = Rotr32(v[b] ^ v[c], 7);
}

// State chain h[0..7], counter t (64 бита, лежат как два DWORD'а t0+t1).
struct State {
    std::uint32_t h[8];
    std::uint64_t t = 0;          // total bytes compressed
    std::uint8_t  buf[kBlockSize] = {};
    std::size_t   buflen = 0;     // bytes в buf (не закомпрешены)
};

// Init для BLAKE2s с output_len = 32 (без keying / salt / personalization).
void Init(State& s) {
    for (int i = 0; i < 8; ++i) {
        s.h[i] = kIV[i];
    }
    // Param block для full-32-byte digest без ключа: digest_length=32,
    // key_length=0, fanout=1, depth=1; остальные нули.
    // Первое 32-битное слово параметров: 0x01010000 ^ digest_length.
    s.h[0] ^= 0x01010000u ^ 32u;
    s.t = 0;
    s.buflen = 0;
}

// Compression F (RFC 7693 §3.2). `lastBlock` ставит final-flag.
void Compress(State& s, const std::uint8_t block[kBlockSize], bool lastBlock) {
    std::uint32_t m[16];
    for (int i = 0; i < 16; ++i) {
        m[i] = LoadU32Le(block + 4 * i);
    }

    std::uint32_t v[16];
    for (int i = 0; i < 8; ++i) {
        v[i]     = s.h[i];
        v[i + 8] = kIV[i];
    }
    v[12] ^= static_cast<std::uint32_t>(s.t & 0xFFFFFFFFu);
    v[13] ^= static_cast<std::uint32_t>(s.t >> 32);
    if (lastBlock) {
        v[14] ^= 0xFFFFFFFFu;
    }

    for (int round = 0; round < 10; ++round) {
        const auto& sg = kSigma[round];
        G(v, 0, 4,  8, 12, m[sg[ 0]], m[sg[ 1]]);
        G(v, 1, 5,  9, 13, m[sg[ 2]], m[sg[ 3]]);
        G(v, 2, 6, 10, 14, m[sg[ 4]], m[sg[ 5]]);
        G(v, 3, 7, 11, 15, m[sg[ 6]], m[sg[ 7]]);
        G(v, 0, 5, 10, 15, m[sg[ 8]], m[sg[ 9]]);
        G(v, 1, 6, 11, 12, m[sg[10]], m[sg[11]]);
        G(v, 2, 7,  8, 13, m[sg[12]], m[sg[13]]);
        G(v, 3, 4,  9, 14, m[sg[14]], m[sg[15]]);
    }

    for (int i = 0; i < 8; ++i) {
        s.h[i] ^= v[i] ^ v[i + 8];
    }
}

void Update(State& s, const std::uint8_t* in, std::size_t inLen) {
    while (inLen > 0) {
        // Если в буфере уже что-то есть — добиваем его до полного блока, но
        // НЕ сбрасываем последний — он может оказаться финальным (last-block
        // флаг для финализации). RFC 7693 §3.3: процессим блок только когда
        // знаем, что он не последний (т.е. появился ещё один байт).
        const std::size_t avail = kBlockSize - s.buflen;
        const std::size_t toCopy = (inLen <= avail) ? inLen : avail;
        std::memcpy(s.buf + s.buflen, in, toCopy);
        s.buflen += toCopy;
        in     += toCopy;
        inLen  -= toCopy;

        if (s.buflen == kBlockSize && inLen > 0) {
            // Гарантированно есть ещё данные — текущий блок не последний.
            s.t += kBlockSize;
            Compress(s, s.buf, /*lastBlock=*/false);
            s.buflen = 0;
        }
    }
}

void Finalize(State& s, std::uint8_t out[kDigestSize]) {
    // Допиваем последний блок нулями.
    s.t += s.buflen;
    std::memset(s.buf + s.buflen, 0, kBlockSize - s.buflen);
    Compress(s, s.buf, /*lastBlock=*/true);

    // h[0..7] → little-endian → out[0..31].
    for (int i = 0; i < 8; ++i) {
        const std::uint32_t w = s.h[i];
        out[4 * i + 0] = static_cast<std::uint8_t>( w        & 0xFF);
        out[4 * i + 1] = static_cast<std::uint8_t>((w >>  8) & 0xFF);
        out[4 * i + 2] = static_cast<std::uint8_t>((w >> 16) & 0xFF);
        out[4 * i + 3] = static_cast<std::uint8_t>((w >> 24) & 0xFF);
    }
}

} // namespace

void Blake2s(const std::uint8_t* in, std::size_t inLen, std::uint8_t out[32]) {
    State s;
    Init(s);
    Update(s, in, inLen);
    Finalize(s, out);
}

std::string Blake2sHex(std::string_view input) {
    std::uint8_t digest[kDigestSize];
    Blake2s(reinterpret_cast<const std::uint8_t*>(input.data()),
            input.size(), digest);

    // Uppercase hex, чтобы совпадало с прежним CryptoPP::HexEncoder
    // (он по умолчанию uppercase).
    static constexpr char kHex[] = "0123456789ABCDEF";
    std::string out(kDigestSize * 2, '\0');
    for (std::size_t i = 0; i < kDigestSize; ++i) {
        out[2 * i + 0] = kHex[(digest[i] >> 4) & 0x0F];
        out[2 * i + 1] = kHex[ digest[i]       & 0x0F];
    }
    return out;
}

} // namespace Corsairs::Common::Crypto
