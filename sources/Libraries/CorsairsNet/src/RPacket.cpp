#include "Packet.h"
#include "MsgpackUtil.h"
#include <algorithm>

namespace net {

// ═══════════════════════════════════════════════════════════════
//  RPacket — реализация (msgpack payload)
// ═══════════════════════════════════════════════════════════════

RPacket::RPacket()
    : _data(nullptr), _dataLen(0), _pos(0), _ownsBuffer(false),
      _parameterForwardIterator(0), _reverseCurrentIndex(-1), _allParameters(0) {
    std::memset(&_reader, 0, sizeof(_reader));
}

RPacket::RPacket(uint8_t* data, int dataLen, bool ownsBuffer)
    : _data(data), _dataLen(dataLen), _pos(std::min(8, dataLen)),
      _ownsBuffer(ownsBuffer),
      _parameterForwardIterator(0), _reverseCurrentIndex(-1), _allParameters(0) {
    if (_data && _dataLen >= 8) {
        mpack_reader_init_data(&_reader,
            reinterpret_cast<const char*>(_data + 8), PayloadLength());
    } else {
        std::memset(&_reader, 0, sizeof(_reader));
    }
}

RPacket::RPacket(RPacket&& other) noexcept
    : _data(other._data), _dataLen(other._dataLen), _pos(other._pos),
      _ownsBuffer(other._ownsBuffer), _reader(other._reader),
      _parameterForwardIterator(other._parameterForwardIterator),
      _reverseCurrentIndex(other._reverseCurrentIndex),
      _allParameters(other._allParameters) {
    other._data = nullptr;
    other._dataLen = 0;
    other._pos = 0;
    other._ownsBuffer = false;
    std::memset(&other._reader, 0, sizeof(other._reader));
}

RPacket& RPacket::operator=(RPacket&& other) noexcept {
    if (this != &other) {
        if (_data) {
            mpack_reader_destroy(&_reader);
            if (_ownsBuffer) {
                PacketPool::Shared().Free(_data, _dataLen);
            }
        }
        _data = other._data;
        _dataLen = other._dataLen;
        _pos = other._pos;
        _ownsBuffer = other._ownsBuffer;
        _reader = other._reader;
        _parameterForwardIterator = other._parameterForwardIterator;
        _reverseCurrentIndex = other._reverseCurrentIndex;
        _allParameters = other._allParameters;

        other._data = nullptr;
        other._dataLen = 0;
        other._pos = 0;
        other._ownsBuffer = false;
        std::memset(&other._reader, 0, sizeof(other._reader));
    }
    return *this;
}

RPacket::~RPacket() {
    if (_data) {
        mpack_reader_destroy(&_reader);
        if (_ownsBuffer) {
            PacketPool::Shared().Free(_data, _dataLen);
        }
        _data = nullptr;
    }
}

// ── Вспомогательные ────────────────────────────────────────

int RPacket::packetSize() const {
    if (!_data || _dataLen < 2) return 0;
    return static_cast<int>(readUInt16(_data));
}

void RPacket::InitReader() {
    if (_data && _dataLen >= 8) {
        mpack_reader_init_data(&_reader,
            reinterpret_cast<const char*>(_data + 8), PayloadLength());
    }
    _parameterForwardIterator = 0;
}

void RPacket::UpdateReverseIndex() {
    _allParameters = _reverseCurrentIndex = static_cast<int>(
        mpack_sequence_length(
            reinterpret_cast<const char*>(_data + 8), PayloadLength()));
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
    if (!_data) return 0;
    return static_cast<int>(_reader.end - _reader.data);
}

bool RPacket::HasData() const {
    return _data != nullptr && _reader.data < _reader.end;
}

// ── Последовательное чтение payload ────────────────────────

int64_t RPacket::ReadInt64() {
    mpack_tag_t tag = mpack_read_tag(&_reader);
    if (mpack_reader_error(&_reader) != mpack_ok) {
        throw std::runtime_error(
            "RPacket: can't parse value at position " +
            std::to_string(_parameterForwardIterator));
    }

    ++_parameterForwardIterator;

    switch (mpack_tag_type(&tag)) {
    case mpack_type_nil:  return 0;
    case mpack_type_bool: return mpack_tag_bool_value(&tag) ? 1 : 0;
    case mpack_type_int:  return mpack_tag_int_value(&tag);
    case mpack_type_uint: return static_cast<int64_t>(mpack_tag_uint_value(&tag));
    default:
        throw std::runtime_error(
            "RPacket: value at position " + std::to_string(_parameterForwardIterator - 1) +
            " is not a number, type: " + std::to_string(mpack_tag_type(&tag)));
    }
}

uint64_t RPacket::ReadUInt64()   { return static_cast<uint64_t>(ReadInt64()); }

float RPacket::ReadFloat32() {
    mpack_tag_t tag = mpack_read_tag(&_reader);
    if (mpack_reader_error(&_reader) != mpack_ok) {
        throw std::runtime_error(
            "RPacket: can't parse float at position " +
            std::to_string(_parameterForwardIterator));
    }

    ++_parameterForwardIterator;

    switch (mpack_tag_type(&tag)) {
    case mpack_type_nil:    return 0.0f;
    case mpack_type_bool:   return mpack_tag_bool_value(&tag) ? 1.0f : 0.0f;
    case mpack_type_int:    return static_cast<float>(mpack_tag_int_value(&tag));
    case mpack_type_uint:   return static_cast<float>(mpack_tag_uint_value(&tag));
    case mpack_type_float:  return mpack_tag_float_value(&tag);
    case mpack_type_double: return static_cast<float>(mpack_tag_double_value(&tag));
    default:
        throw std::runtime_error(
            "RPacket: value at position " + std::to_string(_parameterForwardIterator - 1) +
            " is not a float, type: " + std::to_string(mpack_tag_type(&tag)));
    }
}

const char* RPacket::ReadSequence(uint16_t& outLen) {
    mpack_tag_t tag = mpack_read_tag(&_reader);
    if (mpack_reader_error(&_reader) != mpack_ok) {
        throw std::runtime_error(
            "RPacket: can't parse sequence at position " +
            std::to_string(_parameterForwardIterator));
    }

    ++_parameterForwardIterator;

    switch (mpack_tag_type(&tag)) {
    case mpack_type_nil:
        outLen = 0;
        return nullptr;

    case mpack_type_bin: {
        auto length = mpack_tag_bin_length(&tag);
        auto data = mpack_read_bytes_inplace(&_reader, length);
        mpack_done_bin(&_reader);
        outLen = static_cast<uint16_t>(length);
        return data;
    }

    default:
        throw std::runtime_error(
            "RPacket: value at position " + std::to_string(_parameterForwardIterator - 1) +
            " is not a bin, type: " + std::to_string(mpack_tag_type(&tag)));
    }
}

std::string RPacket::ReadString() {
    mpack_tag_t tag = mpack_read_tag(&_reader);
    if (mpack_reader_error(&_reader) != mpack_ok) {
        throw std::runtime_error(
            "RPacket: can't parse string at position " +
            std::to_string(_parameterForwardIterator));
    }

    ++_parameterForwardIterator;

    switch (mpack_tag_type(&tag)) {
    case mpack_type_nil:
        return "";

    case mpack_type_str: {
        auto length = mpack_tag_str_length(&tag);
        auto data = mpack_read_bytes_inplace(&_reader, length);
        mpack_done_str(&_reader);
        // length включает null-terminator — убираем его
        auto strLen = length > 0 ? length - 1 : 0;
        return std::string(data, strLen);
    }

    case mpack_type_bin: {
        auto length = mpack_tag_bin_length(&tag);
        auto data = mpack_read_bytes_inplace(&_reader, length);
        mpack_done_bin(&_reader);
        auto strLen = length > 0 ? length - 1 : 0;
        return std::string(data, strLen);
    }

    default:
        throw std::runtime_error(
            "RPacket: value at position " + std::to_string(_parameterForwardIterator - 1) +
            " is not a string, type: " + std::to_string(mpack_tag_type(&tag)));
    }
}

// ── Обратное чтение ────────────────────────────────────────

int64_t RPacket::ReverseReadInt64() {
    if (_reverseCurrentIndex == -1) {
        UpdateReverseIndex();
    }

    --_reverseCurrentIndex;
    char* ptr = mpack_skeep_params(
        reinterpret_cast<const char*>(_data + 8), PayloadLength(), _reverseCurrentIndex);

    mpack_reader_t r{};
    mpack_reader_init_data(&r,
        reinterpret_cast<const char*>(_data + 8), PayloadLength());
    r.data = ptr;

    mpack_tag_t tag = mpack_read_tag(&r);
    if (mpack_reader_error(&r) != mpack_ok) {
        mpack_reader_destroy(&r);
        throw std::runtime_error(
            "RPacket: can't parse reverse value at index " +
            std::to_string(_reverseCurrentIndex));
    }

    int64_t value = 0;
    switch (mpack_tag_type(&tag)) {
    case mpack_type_nil:  value = 0; break;
    case mpack_type_bool: value = mpack_tag_bool_value(&tag) ? 1 : 0; break;
    case mpack_type_int:  value = mpack_tag_int_value(&tag); break;
    case mpack_type_uint: value = static_cast<int64_t>(mpack_tag_uint_value(&tag)); break;
    default:
        mpack_reader_destroy(&r);
        throw std::runtime_error(
            "RPacket: reverse value at index " + std::to_string(_reverseCurrentIndex) +
            " is not a number, type: " + std::to_string(mpack_tag_type(&tag)));
    }

    mpack_reader_destroy(&r);
    return value;
}


int64_t RPacket::ReverseReadLegacyInt64() {
    return ReverseReadInt64();
}

// ── Мутации ────────────────────────────────────────────────

bool RPacket::DiscardLast(int count) {
    int payloadLen = PayloadLength();
    if (payloadLen == 0 && count == 0) return true;
    if (payloadLen <= 0) return false;

    uint32_t newLen = mpack_discard_last(
        reinterpret_cast<char*>(_data + 8), payloadLen, count);
    int newSize = 8 + static_cast<int>(newLen);
    if (newSize >= 8) {
        writeUInt16(_data, static_cast<uint16_t>(newSize));
        _reverseCurrentIndex = -1; // invalidate
        // Обновить end указатель reader для корректного HasData()
        _reader.end = reinterpret_cast<const char*>(_data + 8 + newLen);
        return true;
    }
    return false;
}

void RPacket::DiscardLastByReverseIndex() {
    if (_reverseCurrentIndex == -1) {
        UpdateReverseIndex();
    }

    int toDiscard = _allParameters - _reverseCurrentIndex;
    if (toDiscard > 0) {
        uint32_t newLen = mpack_discard_last(
            reinterpret_cast<char*>(_data + 8), PayloadLength(), toDiscard);
        writeUInt16(_data, static_cast<uint16_t>(8 + newLen));
        // Обновить end указатель reader для корректного HasData()
        _reader.end = reinterpret_cast<const char*>(_data + 8 + newLen);
    }

    UpdateReverseIndex();
}

void RPacket::Skip(int count) {
    // Пропустить count msgpack-элементов
    for (int i = 0; i < count; ++i) {
        mpack_tag_t tag = mpack_read_tag(&_reader);
        if (mpack_reader_error(&_reader) != mpack_ok) {
            throw std::runtime_error("RPacket::Skip: can't skip element " + std::to_string(i));
        }
        mpack_skip_element_data(_reader, tag);
        ++_parameterForwardIterator;
    }
}

void RPacket::Reset() {
    if (_data) {
        mpack_reader_destroy(&_reader);
    }
    _pos = std::min(8, _dataLen);
    _reverseCurrentIndex = -1;
    _allParameters = 0;
    _parameterForwardIterator = 0;
    if (_dataLen >= 2) {
        writeUInt16(_data, static_cast<uint16_t>(_dataLen));
    }
    InitReader();
}

void RPacket::ResetPosition() {
    if (_data) {
        mpack_reader_destroy(&_reader);
    }
    _pos = std::min(8, _dataLen);
    _reverseCurrentIndex = -1;
    _allParameters = 0;
    _parameterForwardIterator = 0;
    InitReader();
}

} // namespace net
