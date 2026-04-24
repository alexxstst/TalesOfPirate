#include "Packet.h"
#include <algorithm>

namespace net {

// 
//  WPacket   (msgpack payload)
// 

WPacket::WPacket(int payloadCapacity) {
    int totalSize = 8 + payloadCapacity;
    _capacity = PacketPool::Shared().BucketSize(totalSize);
    _data = PacketPool::Shared().Allocate(totalSize);
    std::memset(_data, 0, 8);
    writeUInt16(_data, 8); //   = 
    UpdateWriterPosition();
}

WPacket::WPacket(const RPacket& rpk)
    : _data(nullptr), _capacity(0) {
    if (rpk.Data()) {
        int size = rpk.GetPacketSize();
        if (size > 0) {
            _capacity = PacketPool::Shared().BucketSize(size);
            _data = PacketPool::Shared().Allocate(size);
            std::memcpy(_data, rpk.Data(), size);
        }
    }
    if (!_data) {
        _capacity = PacketPool::Shared().BucketSize(8);
        _data = PacketPool::Shared().Allocate(8);
        std::memset(_data, 0, 8);
        writeUInt16(_data, 8);
    }
    UpdateWriterPosition();
}

WPacket::WPacket(const WPacket& other)
    : _data(nullptr), _capacity(0) {
    if (other._data) {
        int size = other.GetPacketSize();
        _capacity = PacketPool::Shared().BucketSize(size);
        _data = PacketPool::Shared().Allocate(size);
        std::memcpy(_data, other._data, size);
    }
    if (!_data) {
        _capacity = PacketPool::Shared().BucketSize(8);
        _data = PacketPool::Shared().Allocate(8);
        std::memset(_data, 0, 8);
        writeUInt16(_data, 8);
    }
    UpdateWriterPosition();
}

WPacket& WPacket::operator=(const WPacket& other) {
    if (this != &other) {
        WPacket tmp(other);
        std::swap(_data, tmp._data);
        std::swap(_capacity, tmp._capacity);
        std::swap(_writer, tmp._writer);
        UpdateWriterPosition();
    }
    return *this;
}

WPacket::WPacket(WPacket&& other) noexcept
    : _data(other._data), _capacity(other._capacity), _writer(other._writer) {
    other._data = nullptr;
    other._capacity = 0;
}

WPacket& WPacket::operator=(WPacket&& other) noexcept {
    if (this != &other) {
        if (_data) {
            mpack_writer_destroy(&_writer);
            PacketPool::Shared().Free(_data, _capacity);
        }
        _data = other._data;
        _capacity = other._capacity;
        _writer = other._writer;
        other._data = nullptr;
        other._capacity = 0;
    }
    return *this;
}

WPacket::~WPacket() {
    if (_data) {
        mpack_writer_destroy(&_writer);
        PacketPool::Shared().Free(_data, _capacity);
        _data = nullptr;
    }
}

//  Writer position 

void WPacket::UpdateWriterPosition() {
    mpack_writer_init(&_writer, reinterpret_cast<char*>(_data + 8), _capacity - 8);
    _writer.position = reinterpret_cast<char*>(_data + 8) + PayloadLength();
}

void WPacket::SyncSize() {
    int payloadUsed = static_cast<int>(
        _writer.position - reinterpret_cast<char*>(_data + 8));
    SetPacketSize(8 + payloadUsed);
}

//   

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

//   

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

//  EnsureCapacity 

int WPacket::EnsureCapacity(int needed) {
    int pos = GetPacketSize();
    int required = pos + needed + 4; // +4   msgpack 

    if (required > _capacity) {
        mpack_writer_destroy(&_writer);
        int newCapacity = std::max(_capacity * 2, required);
        int newBucketSize = PacketPool::Shared().BucketSize(newCapacity);
        uint8_t* newData = PacketPool::Shared().Allocate(newCapacity);
        std::memcpy(newData, _data, pos);
        PacketPool::Shared().Free(_data, _capacity);
        _data = newData;
        _capacity = newBucketSize;
        UpdateWriterPosition();
    }

    return pos;
}

//   payload 

void WPacket::WriteInt64(int64_t v) {
    EnsureCapacity(9);
    mpack_write(&_writer, v);
    SyncSize();
}

void WPacket::WriteUInt64(uint64_t v)  { WriteInt64(static_cast<int64_t>(v)); }

void WPacket::WriteFloat32(float v) {
    EnsureCapacity(5);
    mpack_write(&_writer, v);
    SyncSize();
}

void WPacket::WriteSequence(const uint8_t* data, uint16_t len) {
    EnsureCapacity(5 + len);
    mpack_write_bin(&_writer, reinterpret_cast<const char*>(data), len);
    SyncSize();
}

void WPacket::WriteSequence(const char* data, uint16_t len) {
    WriteSequence(reinterpret_cast<const uint8_t*>(data), len);
}

void WPacket::WriteString(const std::string& str) {
    if (str.empty()) {
        EnsureCapacity(6);
        mpack_write_str(&_writer, "\0", 1); // 1  = null-terminator
    } else {
        const auto len = str.size() + 1; // +1 = null
        EnsureCapacity(5 + len);
        mpack_write_str(&_writer, str.c_str(), len);
    }
    SyncSize();
}

//  Reset 

void WPacket::Reset() {
    mpack_writer_destroy(&_writer);
    std::memset(_data, 0, 8);
    SetPacketSize(8);
    UpdateWriterPosition();
}

} // namespace net
