#pragma once

// Packet.h  WPacket ()  RPacket ()   msgpack payload.
//  : [2b size BE][4b SESS BE][2b CMD BE][msgpack payload]
//    2    source of truth.
//    PacketPool::Shared().

#include "BinaryIO.h"
#include "PacketPool.h"
#include "MsgpackUtil.h"
#include "CommandNames.h"
#include "mpack.h"
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <string>

namespace net {

// Forward declaration
class RPacket;

// 
//  WPacket    (msgpack payload)
// 

class WPacket {
public:
    explicit WPacket(int payloadCapacity = 0);
    explicit WPacket(const RPacket& rpk);

    WPacket(const WPacket& other);
    WPacket& operator=(const WPacket& other);
    WPacket(WPacket&& other) noexcept;
    WPacket& operator=(WPacket&& other) noexcept;
    ~WPacket();

    //   ( ) 
    void     WriteCmd(uint16_t cmd);
    void     WriteSess(uint32_t sess);
    uint16_t GetCmd() const;
    uint32_t GetSess() const;

    //   
    int  GetPacketSize() const;
    void SetPacketSize(int size);
    int  PayloadLength() const;

    //   payload (    WriteInt64) 
    void WriteInt64(int64_t v);
    void WriteUInt64(uint64_t v);
    void WriteFloat32(float v);

    //    msgpack bin
    void WriteSequence(const uint8_t* data, uint16_t len);
    void WriteSequence(const char* data, uint16_t len);

    //   msgpack str ( null-terminator   zero-copy )
    void WriteString(const std::string& str);

    //     
    uint8_t*       Data()       { return _data; }
    const uint8_t* Data() const { return _data; }
    int            Capacity() const { return _capacity; }

    void Reset();
    explicit operator bool() const { return _data != nullptr; }

    //     
    std::string PrintCommand() const {
        if (!_data || GetPacketSize() < 8)
            return "WPacket[Empty]";
        int pl = PayloadLength();
        std::string payload = (pl > 0)
            ? mpack_sequence_to_string(
                  reinterpret_cast<const char*>(_data + 8), pl)
            : "";
        auto cmd = GetCmd();
        auto name = net::GetCommandName(cmd);
        std::string cmdStr = name
            ? std::string(name) + "(" + std::to_string(cmd) + ")"
            : std::to_string(cmd);
        return ">> WPacket[Cmd=" + cmdStr
            + " Sess=" + std::to_string(GetSess())
            + " Size=" + std::to_string(GetPacketSize())
            + payload + "]";
    }

    void WriteFloat(float v)        { WriteFloat32(v); }
    WPacket Duplicate() const       { return WPacket(*this); }

private:
    int EnsureCapacity(int needed);
    void UpdateWriterPosition();
    void SyncSize();

    uint8_t* _data;
    int _capacity;
    mpack_writer_t _writer;
};

// 
//  RPacket    (msgpack payload)
// 

class RPacket {
public:
    RPacket(uint8_t* data, int dataLen, bool ownsBuffer = false);
    RPacket();

    RPacket(const RPacket&) = delete;
    RPacket& operator=(const RPacket&) = delete;
    RPacket(RPacket&& other) noexcept;
    RPacket& operator=(RPacket&& other) noexcept;
    ~RPacket();

    //   
    uint32_t GetSess() const;
    uint16_t GetCmd() const;
    int      GetPacketSize() const;
    int      PayloadLength() const;

    //    
    int  RemainingBytes() const;
    int  Position() const { return _pos; }
    bool HasData() const;

    //    (    ReadInt64) 
    int64_t  ReadInt64();
    uint64_t ReadUInt64();
    float    ReadFloat32();

    //   (msgpack bin).    .
    const char* ReadSequence(uint16_t& outLen);

    //  (msgpack str  null-terminator).  std::string.
    std::string ReadString();

    //    (  ,  ) 
    int64_t  ReverseReadInt64();

    //  LIBDBC-  
    uint16_t ReadCmd()              { return GetCmd(); }
    float    ReadFloat()            { return ReadFloat32(); }
    int      RemainData() const     { return RemainingBytes(); }

    //   
    // count =  msgpack-     ( !)
    bool DiscardLast(int count);
    //   ,   ReverseRead*
    void DiscardLastByReverseIndex();
    void Skip(int count);
    void Reset();
    void ResetPosition();

    //     
    uint8_t*       Data()       { return _data; }
    const uint8_t* Data() const { return _data; }
    explicit operator bool() const { return _data != nullptr && _dataLen >= 8; }

    //     
    std::string PrintCommand() const {
        if (!_data || _dataLen < 8)
            return "RPacket[Empty]";
        int pl = PayloadLength();
        std::string payload = (pl > 0)
            ? mpack_sequence_to_string(
                  reinterpret_cast<const char*>(_data + 8), pl)
            : "";
        auto cmd = GetCmd();
        auto name = net::GetCommandName(cmd);
        std::string cmdStr = name
            ? std::string(name) + "(" + std::to_string(cmd) + ")"
            : std::to_string(cmd);
        return "<< RPacket[Cmd=" + cmdStr
            + " Sess=" + std::to_string(GetSess())
            + " Size=" + std::to_string(GetPacketSize())
            + payload + "]";
    }

private:
    int packetSize() const;
    void InitReader();
    void UpdateReverseIndex();

    uint8_t* _data;
    int _dataLen;
    int _pos;               //  (  reader)
    bool _ownsBuffer;

    mpack_reader_t _reader;
    int _parameterForwardIterator;
    int _reverseCurrentIndex;   // -1 =  
    int _allParameters;
};

} // namespace net
