#include "Packet.h"
#include <algorithm>
#include <cassert>

namespace net {

// ═══════════════════════════════════════════════════════════════
//  WPacket — реализация
// ═══════════════════════════════════════════════════════════════

WPacket::WPacket(int payloadCapacity) {
    int totalSize = 8 + payloadCapacity;
    _capacity = PacketPool::Shared().BucketSize(totalSize);
    _data = PacketPool::Shared().Allocate(totalSize);
    std::memset(_data, 0, 8);
    writeUInt16(_data, 8); // Начальный размер = заголовок
}

WPacket::WPacket(const WPacket& other)
    : _data(nullptr), _capacity(0) {
    if (other._data) {
        int size = other.GetPacketSize();
        _capacity = PacketPool::Shared().BucketSize(size);
        _data = PacketPool::Shared().Allocate(size);
        std::memcpy(_data, other._data, size);
    }
}

WPacket& WPacket::operator=(const WPacket& other) {
    if (this != &other) {
        WPacket tmp(other);
        std::swap(_data, tmp._data);
        std::swap(_capacity, tmp._capacity);
    }
    return *this;
}

WPacket::WPacket(WPacket&& other) noexcept
    : _data(other._data), _capacity(other._capacity) {
    other._data = nullptr;
    other._capacity = 0;
}

WPacket& WPacket::operator=(WPacket&& other) noexcept {
    if (this != &other) {
        if (_data) {
            PacketPool::Shared().Free(_data, _capacity);
        }
        _data = other._data;
        _capacity = other._capacity;
        other._data = nullptr;
        other._capacity = 0;
    }
    return *this;
}

WPacket::~WPacket() {
    if (_data) {
        PacketPool::Shared().Free(_data, _capacity);
        _data = nullptr;
    }
}

// ── Заголовок ──────────────────────────────────────────────

void WPacket::WriteCmd(uint16_t cmd) {
    writeUInt16(_data + 6, cmd);
}

void WPacket::WriteSess(uint32_t sess) {
    writeUInt32(_data + 2, sess);
}

uint16_t WPacket::GetCmd() const {
    if (!_data || _capacity < 8) return 0;
    return readUInt16(_data + 6);
}

uint32_t WPacket::GetSess() const {
    if (!_data || _capacity < 6) return 0;
    return readUInt32(_data + 2);
}

// ── Размеры ────────────────────────────────────────────────

int WPacket::GetPacketSize() const {
    if (!_data || _capacity < 2) return 0;
    return static_cast<int>(readUInt16(_data));
}

void WPacket::SetPacketSize(int size) {
    writeUInt16(_data, static_cast<uint16_t>(size));
}

int WPacket::PayloadLength() const {
    return GetPacketSize() - 8;
}

// ── EnsureCapacity ─────────────────────────────────────────

int WPacket::EnsureCapacity(int needed) {
    int pos = GetPacketSize();
    int required = pos + needed;

    if (required > _capacity) {
        int newCapacity = std::max(_capacity * 2, required);
        int newBucketSize = PacketPool::Shared().BucketSize(newCapacity);
        uint8_t* newData = PacketPool::Shared().Allocate(newCapacity);
        std::memcpy(newData, _data, pos);
        PacketPool::Shared().Free(_data, _capacity);
        _data = newData;
        _capacity = newBucketSize;
    }

    return pos;
}

// ── Запись payload ─────────────────────────────────────────

void WPacket::WriteUInt8(uint8_t v) {
    int pos = EnsureCapacity(1);
    writeUInt8(_data + pos, v);
    SetPacketSize(pos + 1);
}

void WPacket::WriteInt8(int8_t v) {
    int pos = EnsureCapacity(1);
    writeInt8(_data + pos, v);
    SetPacketSize(pos + 1);
}

void WPacket::WriteUInt16(uint16_t v) {
    int pos = EnsureCapacity(2);
    writeUInt16(_data + pos, v);
    SetPacketSize(pos + 2);
}

void WPacket::WriteInt16(int16_t v) {
    int pos = EnsureCapacity(2);
    writeInt16(_data + pos, v);
    SetPacketSize(pos + 2);
}

void WPacket::WriteUInt32(uint32_t v) {
    int pos = EnsureCapacity(4);
    writeUInt32(_data + pos, v);
    SetPacketSize(pos + 4);
}

void WPacket::WriteInt32(int32_t v) {
    int pos = EnsureCapacity(4);
    writeInt32(_data + pos, v);
    SetPacketSize(pos + 4);
}

void WPacket::WriteInt64(int64_t v) {
    int pos = EnsureCapacity(8);
    writeInt64(_data + pos, v);
    SetPacketSize(pos + 8);
}

void WPacket::WriteUInt64(uint64_t v) {
    int pos = EnsureCapacity(8);
    writeUInt64(_data + pos, v);
    SetPacketSize(pos + 8);
}

void WPacket::WriteFloat32(float v) {
    int pos = EnsureCapacity(4);
    writeFloat32(_data + pos, v);
    SetPacketSize(pos + 4);
}

void WPacket::WriteSequence(const uint8_t* data, uint16_t len) {
    WriteUInt16(len);
    if (len > 0) {
        int pos = EnsureCapacity(len);
        std::memcpy(_data + pos, data, len);
        SetPacketSize(pos + len);
    }
}

void WPacket::WriteSequence(const char* data, uint16_t len) {
    WriteSequence(reinterpret_cast<const uint8_t*>(data), len);
}

void WPacket::WriteString(const char* str) {
    if (!str || str[0] == '\0') {
        // Пустая строка: [len=1][null]
        WriteUInt16(1);
        int pos = EnsureCapacity(1);
        _data[pos] = 0;
        SetPacketSize(pos + 1);
    } else {
        uint16_t strLen = static_cast<uint16_t>(std::strlen(str));
        uint16_t totalLen = strLen + 1; // включая null
        WriteUInt16(totalLen);
        int pos = EnsureCapacity(totalLen);
        std::memcpy(_data + pos, str, strLen);
        _data[pos + strLen] = 0;
        SetPacketSize(pos + totalLen);
    }
}

// ── Reset ──────────────────────────────────────────────────

void WPacket::Reset() {
    std::memset(_data, 0, 8);
    SetPacketSize(8);
}

} // namespace net
