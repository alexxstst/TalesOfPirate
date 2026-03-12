#include "Packet.h"
#include <algorithm>
#include <cassert>

namespace net {

// ═══════════════════════════════════════════════════════════════
//  RPacket — реализация
// ═══════════════════════════════════════════════════════════════

RPacket::RPacket()
    : _data(nullptr), _dataLen(0), _pos(0), _revPos(0), _ownsBuffer(false) {}

RPacket::RPacket(uint8_t* data, int dataLen, bool ownsBuffer)
    : _data(data), _dataLen(dataLen), _pos(std::min(8, dataLen)),
      _revPos(0), _ownsBuffer(ownsBuffer) {}

RPacket::RPacket(RPacket&& other) noexcept
    : _data(other._data), _dataLen(other._dataLen), _pos(other._pos),
      _revPos(other._revPos), _ownsBuffer(other._ownsBuffer) {
    other._data = nullptr;
    other._dataLen = 0;
    other._pos = 0;
    other._revPos = 0;
    other._ownsBuffer = false;
}

RPacket& RPacket::operator=(RPacket&& other) noexcept {
    if (this != &other) {
        if (_data && _ownsBuffer) {
            PacketPool::Shared().Free(_data, _dataLen);
        }
        _data = other._data;
        _dataLen = other._dataLen;
        _pos = other._pos;
        _revPos = other._revPos;
        _ownsBuffer = other._ownsBuffer;

        other._data = nullptr;
        other._dataLen = 0;
        other._pos = 0;
        other._revPos = 0;
        other._ownsBuffer = false;
    }
    return *this;
}

RPacket::~RPacket() {
    if (_data && _ownsBuffer) {
        PacketPool::Shared().Free(_data, _dataLen);
        _data = nullptr;
    }
}

// ── Вспомогательные ────────────────────────────────────────

int RPacket::packetSize() const {
    if (!_data || _dataLen < 2) return 0;
    return static_cast<int>(readUInt16(_data));
}

void RPacket::ensureAvailable(int count) const {
    int ps = packetSize();
    if (_pos + count > ps) {
        throw std::runtime_error(
            "RPacket: insufficient data: need " + std::to_string(count) +
            " bytes, available " + std::to_string(ps - _pos));
    }
}

void RPacket::ensureReverseAvailable(int count) const {
    int ps = packetSize();
    if (ps < _revPos + count) {
        throw std::runtime_error(
            "RPacket: insufficient data for reverse read: need " + std::to_string(count) +
            " bytes, available " + std::to_string(ps - _revPos));
    }
}

// ── Заголовок ──────────────────────────────────────────────

uint32_t RPacket::GetSess() const {
    if (!_data || _dataLen < 6) return 0;
    return readUInt32(_data + 2);
}

uint16_t RPacket::GetCmd() const {
    if (!_data || _dataLen < 8) return 0;
    return readUInt16(_data + 6);
}

int RPacket::GetPacketSize() const {
    return packetSize();
}

int RPacket::PayloadLength() const {
    return packetSize() - 8;
}

// ── Состояние чтения ───────────────────────────────────────

int RPacket::RemainingBytes() const {
    return packetSize() - _pos;
}

bool RPacket::HasData() const {
    return _pos < packetSize();
}

// ── Последовательное чтение payload ────────────────────────

uint8_t RPacket::ReadUInt8() {
    ensureAvailable(1);
    uint8_t v = readUInt8(_data + _pos);
    _pos += 1;
    return v;
}

int8_t RPacket::ReadInt8() {
    ensureAvailable(1);
    int8_t v = readInt8(_data + _pos);
    _pos += 1;
    return v;
}

uint16_t RPacket::ReadUInt16() {
    ensureAvailable(2);
    uint16_t v = readUInt16(_data + _pos);
    _pos += 2;
    return v;
}

int16_t RPacket::ReadInt16() {
    ensureAvailable(2);
    int16_t v = readInt16(_data + _pos);
    _pos += 2;
    return v;
}

uint32_t RPacket::ReadUInt32() {
    ensureAvailable(4);
    uint32_t v = readUInt32(_data + _pos);
    _pos += 4;
    return v;
}

int32_t RPacket::ReadInt32() {
    ensureAvailable(4);
    int32_t v = readInt32(_data + _pos);
    _pos += 4;
    return v;
}

int64_t RPacket::ReadInt64() {
    ensureAvailable(8);
    int64_t v = readInt64(_data + _pos);
    _pos += 8;
    return v;
}

uint64_t RPacket::ReadUInt64() {
    ensureAvailable(8);
    uint64_t v = readUInt64(_data + _pos);
    _pos += 8;
    return v;
}

float RPacket::ReadFloat32() {
    ensureAvailable(4);
    float v = readFloat32(_data + _pos);
    _pos += 4;
    return v;
}

const char* RPacket::ReadSequence(uint16_t& outLen) {
    outLen = ReadUInt16();
    if (outLen == 0) return nullptr;

    ensureAvailable(outLen);
    const char* ptr = reinterpret_cast<const char*>(_data + _pos);
    _pos += outLen;
    return ptr;
}

const char* RPacket::ReadString(uint16_t* outLen) {
    uint16_t len;
    const char* seq = ReadSequence(len);

    if (outLen) *outLen = len;

    if (!seq || len == 0) return "";

    return seq;
}

// ── Обратное чтение ────────────────────────────────────────

uint8_t RPacket::ReverseReadUInt8() {
    ensureReverseAvailable(1);
    _revPos += 1;
    return readUInt8(_data + packetSize() - _revPos);
}

uint16_t RPacket::ReverseReadUInt16() {
    ensureReverseAvailable(2);
    _revPos += 2;
    return readUInt16(_data + packetSize() - _revPos);
}

uint32_t RPacket::ReverseReadUInt32() {
    ensureReverseAvailable(4);
    _revPos += 4;
    return readUInt32(_data + packetSize() - _revPos);
}

int64_t RPacket::ReverseReadInt64() {
    ensureReverseAvailable(8);
    _revPos += 8;
    return readInt64(_data + packetSize() - _revPos);
}

// ── Мутации ────────────────────────────────────────────────

bool RPacket::DiscardLast(int count) {
    int newSize = packetSize() - count;
    if (newSize >= 8) {
        _revPos = 0;
        writeUInt16(_data, static_cast<uint16_t>(newSize));
        return true;
    }
    return false;
}

void RPacket::Skip(int count) {
    ensureAvailable(count);
    _pos += count;
}

void RPacket::Reset() {
    _pos = std::min(8, _dataLen);
    _revPos = 0;
    if (_dataLen >= 2) {
        writeUInt16(_data, static_cast<uint16_t>(_dataLen));
    }
}

} // namespace net
