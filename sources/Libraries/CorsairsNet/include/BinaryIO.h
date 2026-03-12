#pragma once

// BinaryIO — inline-функции чтения/записи примитивов (big-endian).
// Аналог F# BinaryIO модуля из Corsairs.Platform.Network.
// Целочисленные типы хранятся в BE, float — в LE (совместимость с F# BinaryPrimitives).

#include <cstdint>
#include <cstring>
#include <intrin.h>

namespace net {

// ── Write (big-endian) ─────────────────────────────────────

inline void writeUInt8(uint8_t* dst, uint8_t v) {
    dst[0] = v;
}

inline void writeInt8(uint8_t* dst, int8_t v) {
    dst[0] = static_cast<uint8_t>(v);
}

inline void writeUInt16(uint8_t* dst, uint16_t v) {
    uint16_t be = _byteswap_ushort(v);
    std::memcpy(dst, &be, 2);
}

inline void writeInt16(uint8_t* dst, int16_t v) {
    writeUInt16(dst, static_cast<uint16_t>(v));
}

inline void writeUInt32(uint8_t* dst, uint32_t v) {
    uint32_t be = _byteswap_ulong(v);
    std::memcpy(dst, &be, 4);
}

inline void writeInt32(uint8_t* dst, int32_t v) {
    writeUInt32(dst, static_cast<uint32_t>(v));
}

inline void writeInt64(uint8_t* dst, int64_t v) {
    // Совместимость с F#: high word first, low word second (BE)
    uint64_t uv = static_cast<uint64_t>(v);
    writeUInt32(dst, static_cast<uint32_t>(uv >> 32));
    writeUInt32(dst + 4, static_cast<uint32_t>(uv & 0xFFFFFFFF));
}

inline void writeUInt64(uint8_t* dst, uint64_t v) {
    writeInt64(dst, static_cast<int64_t>(v));
}

// Float32 в LE (как F# BinaryPrimitives.WriteSingleLittleEndian)
inline void writeFloat32(uint8_t* dst, float v) {
    std::memcpy(dst, &v, 4);
}

// ── Read (big-endian) ──────────────────────────────────────

inline uint8_t readUInt8(const uint8_t* src) {
    return src[0];
}

inline int8_t readInt8(const uint8_t* src) {
    return static_cast<int8_t>(src[0]);
}

inline uint16_t readUInt16(const uint8_t* src) {
    uint16_t be;
    std::memcpy(&be, src, 2);
    return _byteswap_ushort(be);
}

inline int16_t readInt16(const uint8_t* src) {
    return static_cast<int16_t>(readUInt16(src));
}

inline uint32_t readUInt32(const uint8_t* src) {
    uint32_t be;
    std::memcpy(&be, src, 4);
    return _byteswap_ulong(be);
}

inline int32_t readInt32(const uint8_t* src) {
    return static_cast<int32_t>(readUInt32(src));
}

inline int64_t readInt64(const uint8_t* src) {
    // Совместимость с F#: high word first, low word second
    uint64_t high = static_cast<uint64_t>(readUInt32(src));
    uint64_t low  = static_cast<uint64_t>(readUInt32(src + 4));
    return static_cast<int64_t>((high << 32) | low);
}

inline uint64_t readUInt64(const uint8_t* src) {
    return static_cast<uint64_t>(readInt64(src));
}

// Float32 из LE
inline float readFloat32(const uint8_t* src) {
    float v;
    std::memcpy(&v, src, 4);
    return v;
}

} // namespace net
