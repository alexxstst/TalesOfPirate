// PacketTests.cpp — тесты WPacket / RPacket (msgpack payload).
// По аналогии с .NET PacketTests.fs.

#include "TestFramework.h"
#include "Packet.h"
#include "MsgpackUtil.h"
#include "BinaryIO.h"
#include <cstdint>
#include <cstring>
#include <climits>
#include <cmath>
#include <string>
#include <iostream>

using namespace net;

// ═══════════════════════════════════════════════════════════════
//  Вспомогательная функция: вывод содержимого пакета
// ═══════════════════════════════════════════════════════════════

static std::string printPacket(const WPacket& w) {
    int payloadLen = w.GetPacketSize() - 8;
    if (payloadLen <= 0) return "[пустой payload]";
    return mpack_sequence_to_string(
        reinterpret_cast<const char*>(w.Data() + 8), payloadLen);
}

static std::string printPacket(RPacket& r) {
    int payloadLen = r.GetPacketSize() - 8;
    if (payloadLen <= 0) return "[пустой payload]";
    return mpack_sequence_to_string(
        reinterpret_cast<const char*>(r.Data() + 8), payloadLen);
}

// ═══════════════════════════════════════════════════════════════
//  Группа 1: WPacket — тесты записи
// ═══════════════════════════════════════════════════════════════

TEST(WPacketWrite, EmptyPacket_FrameLength8) {
    WPacket w(16);
    ASSERT_EQ(8, w.GetPacketSize());
    ASSERT_EQ(0, w.PayloadLength());
}

TEST(WPacketWrite, WriteCmd_BE_Position6) {
    WPacket w(16);
    w.WriteCmd(0x1234);
    ASSERT_EQ(0x12, w.Data()[6]);
    ASSERT_EQ(0x34, w.Data()[7]);
}

TEST(WPacketWrite, WriteSess_BE_Position2) {
    WPacket w(16);
    w.WriteSess(0xAABBCCDD);
    ASSERT_EQ(0xAA, w.Data()[2]);
    ASSERT_EQ(0xBB, w.Data()[3]);
    ASSERT_EQ(0xCC, w.Data()[4]);
    ASSERT_EQ(0xDD, w.Data()[5]);
}

TEST(WPacketWrite, SizeField_UpdatesOnWrite) {
    WPacket w(16);
    w.WriteCmd(1);
    ASSERT_EQ(8, static_cast<int>(readUInt16(w.Data())));
    w.WriteInt64(0xFF);
    // msgpack compact: 0xFF → cc ff (2 bytes)
    ASSERT_TRUE(w.GetPacketSize() > 8);
}

TEST(WPacketWrite, WriteUInt8_IncreasesSize) {
    WPacket w(16);
    int before = w.GetPacketSize();
    w.WriteInt64(42);
    ASSERT_TRUE(w.GetPacketSize() > before);
}

TEST(WPacketWrite, WriteString_NullTerminator) {
    WPacket w(64);
    w.WriteString("hello");
    // payload содержит msgpack str с "hello\0" (6 байт данных)
    std::string content = printPacket(w);
    ASSERT_TRUE(content.find("str(6)hello") != std::string::npos);
}

TEST(WPacketWrite, WriteSequence_MsgpackBin) {
    WPacket w(64);
    uint8_t data[] = {1, 2, 3, 4};
    w.WriteSequence(data, 4);
    std::string content = printPacket(w);
    ASSERT_TRUE(content.find("bin(4)") != std::string::npos);
}

TEST(WPacketWrite, Reset_ClearsPayload) {
    WPacket w(32);
    w.WriteInt64(12345);
    ASSERT_TRUE(w.PayloadLength() > 0);
    w.Reset();
    ASSERT_EQ(0, w.PayloadLength());
    ASSERT_EQ(8, w.GetPacketSize());
}

// ═══════════════════════════════════════════════════════════════
//  Группа 2: Roundtrip WPacket → RPacket
// ═══════════════════════════════════════════════════════════════

TEST(Roundtrip, UInt8) {
    WPacket w(32);
    w.WriteCmd(100);
    w.WriteInt64(0);
    w.WriteInt64(127);
    w.WriteInt64(255);

    RPacket r(w.Data(), w.GetPacketSize());
    ASSERT_EQ(100, r.GetCmd());
    ASSERT_EQ(0, r.ReadInt64());
    ASSERT_EQ(127, r.ReadInt64());
    ASSERT_EQ(255, r.ReadInt64());
}

TEST(Roundtrip, Int8) {
    WPacket w(32);
    w.WriteCmd(101);
    w.WriteInt64(-128);
    w.WriteInt64(0);
    w.WriteInt64(127);

    RPacket r(w.Data(), w.GetPacketSize());
    ASSERT_EQ(-128, r.ReadInt64());
    ASSERT_EQ(0, r.ReadInt64());
    ASSERT_EQ(127, r.ReadInt64());
}

TEST(Roundtrip, UInt16) {
    WPacket w(32);
    w.WriteCmd(102);
    w.WriteInt64(0);
    w.WriteInt64(12345);
    w.WriteInt64(65535);

    RPacket r(w.Data(), w.GetPacketSize());
    ASSERT_EQ(0, r.ReadInt64());
    ASSERT_EQ(12345, r.ReadInt64());
    ASSERT_EQ(65535, r.ReadInt64());
}

TEST(Roundtrip, Int16) {
    WPacket w(32);
    w.WriteCmd(103);
    w.WriteInt64(-32768);
    w.WriteInt64(0);
    w.WriteInt64(32767);

    RPacket r(w.Data(), w.GetPacketSize());
    ASSERT_EQ(-32768, r.ReadInt64());
    ASSERT_EQ(0, r.ReadInt64());
    ASSERT_EQ(32767, r.ReadInt64());
}

TEST(Roundtrip, UInt32) {
    WPacket w(32);
    w.WriteCmd(104);
    w.WriteInt64(0);
    w.WriteInt64(123456789);
    w.WriteInt64(0xFFFFFFFF);

    RPacket r(w.Data(), w.GetPacketSize());
    ASSERT_EQ(0u, r.ReadInt64());
    ASSERT_EQ(123456789u, r.ReadInt64());
    ASSERT_EQ(0xFFFFFFFFu, r.ReadInt64());
}

TEST(Roundtrip, Int32) {
    WPacket w(32);
    w.WriteCmd(105);
    w.WriteInt64(INT_MIN);
    w.WriteInt64(0);
    w.WriteInt64(INT_MAX);

    RPacket r(w.Data(), w.GetPacketSize());
    ASSERT_EQ(INT_MIN, r.ReadInt64());
    ASSERT_EQ(0, r.ReadInt64());
    ASSERT_EQ(INT_MAX, r.ReadInt64());
}

TEST(Roundtrip, Int64) {
    WPacket w(32);
    w.WriteCmd(106);
    w.WriteInt64(INT64_MIN);
    w.WriteInt64(0);
    w.WriteInt64(INT64_MAX);

    RPacket r(w.Data(), w.GetPacketSize());
    ASSERT_EQ(INT64_MIN, r.ReadInt64());
    ASSERT_EQ(0LL, r.ReadInt64());
    ASSERT_EQ(INT64_MAX, r.ReadInt64());
}

TEST(Roundtrip, UInt64) {
    WPacket w(32);
    w.WriteCmd(107);
    w.WriteUInt64(0);
    w.WriteUInt64(UINT64_MAX);

    RPacket r(w.Data(), w.GetPacketSize());
    ASSERT_EQ(0ULL, r.ReadUInt64());
    ASSERT_EQ(UINT64_MAX, r.ReadUInt64());
}

TEST(Roundtrip, Float32) {
    WPacket w(32);
    w.WriteCmd(108);
    w.WriteFloat32(0.0f);
    w.WriteFloat32(3.14f);
    w.WriteFloat32(-1.5f);

    RPacket r(w.Data(), w.GetPacketSize());
    ASSERT_FLOAT_EQ(0.0f, r.ReadFloat32());
    ASSERT_FLOAT_EQ(3.14f, r.ReadFloat32());
    ASSERT_FLOAT_EQ(-1.5f, r.ReadFloat32());
}

TEST(Roundtrip, Float32_SpecialValues) {
    WPacket w(32);
    w.WriteFloat32(std::numeric_limits<float>::infinity());
    w.WriteFloat32(-std::numeric_limits<float>::infinity());

    RPacket r(w.Data(), w.GetPacketSize());
    ASSERT_TRUE(std::isinf(r.ReadFloat32()));
    ASSERT_TRUE(std::isinf(r.ReadFloat32()));
}

TEST(Roundtrip, String_ASCII) {
    WPacket w(64);
    w.WriteCmd(109);
    w.WriteString("hello world");

    RPacket r(w.Data(), w.GetPacketSize());
    ASSERT_EQ(std::string("hello world"), r.ReadString());
}

TEST(Roundtrip, String_Empty) {
    WPacket w(32);
    w.WriteString("");

    RPacket r(w.Data(), w.GetPacketSize());
    std::string s = r.ReadString();
    ASSERT_EQ(0, (int)s.size());
}

TEST(Roundtrip, String_Null) {
    WPacket w(32);
    w.WriteString("");

    RPacket r(w.Data(), w.GetPacketSize());
    std::string s = r.ReadString();
    ASSERT_EQ(0, (int)s.size());
}

TEST(Roundtrip, Sequence) {
    WPacket w(64);
    uint8_t data[] = {0xAA, 0xBB, 0xCC, 0xDD};
    w.WriteSequence(data, 4);

    RPacket r(w.Data(), w.GetPacketSize());
    uint16_t len = 0;
    const char* ptr = r.ReadSequence(len);
    ASSERT_EQ(4, (int)len);
    ASSERT_EQ((char)0xAA, ptr[0]);
    ASSERT_EQ((char)0xBB, ptr[1]);
    ASSERT_EQ((char)0xCC, ptr[2]);
    ASSERT_EQ((char)0xDD, ptr[3]);
}

TEST(Roundtrip, Sequence_Empty) {
    WPacket w(32);
    w.WriteSequence(static_cast<const uint8_t*>(nullptr), 0);

    RPacket r(w.Data(), w.GetPacketSize());
    uint16_t len = 0;
    const char* ptr = r.ReadSequence(len);
    ASSERT_EQ(0, (int)len);
}

TEST(Roundtrip, Combined_AllTypes) {
    WPacket w(256);
    w.WriteCmd(200);
    w.WriteInt64(42);
    w.WriteInt64(-100);
    w.WriteInt64(999999);
    w.WriteInt64(-1234567890LL);
    w.WriteFloat32(2.5f);
    w.WriteString("test");
    uint8_t bin[] = {1, 2, 3};
    w.WriteSequence(bin, 3);

    std::cout << "    Combined payload: " << printPacket(w) << "\n";

    RPacket r(w.Data(), w.GetPacketSize());
    ASSERT_EQ(200, r.GetCmd());
    ASSERT_EQ(42, r.ReadInt64());
    ASSERT_EQ(-100, r.ReadInt64());
    ASSERT_EQ(999999u, r.ReadInt64());
    ASSERT_EQ(-1234567890LL, r.ReadInt64());
    ASSERT_FLOAT_EQ(2.5f, r.ReadFloat32());
    ASSERT_EQ(std::string("test"), r.ReadString());
    uint16_t seqLen = 0;
    const char* seqPtr = r.ReadSequence(seqLen);
    ASSERT_EQ(3, (int)seqLen);
    ASSERT_EQ(1, (int)(uint8_t)seqPtr[0]);
}

// ═══════════════════════════════════════════════════════════════
//  Группа 3: Msgpack compact encoding (все числа → int64)
// ═══════════════════════════════════════════════════════════════

TEST(MsgpackCompact, SmallPositive_SingleByte) {
    // Числа 0..127 кодируются в 1 байт (positive fixint)
    WPacket w(16);
    w.WriteInt64(42);
    ASSERT_EQ(9, w.GetPacketSize()); // 8 header + 1 byte payload
}

TEST(MsgpackCompact, SmallNegative_SingleByte) {
    // Числа -32..-1 кодируются в 1 байт (negative fixint)
    WPacket w(16);
    w.WriteInt64(-1);
    ASSERT_EQ(9, w.GetPacketSize()); // 8 header + 1 byte
}

TEST(MsgpackCompact, Zero_SingleByte) {
    WPacket w(16);
    w.WriteInt64(0);
    ASSERT_EQ(9, w.GetPacketSize()); // 8 + 1 (fixint 0)
}

TEST(MsgpackCompact, UInt8_LargeValue) {
    // 255 → cc ff (2 bytes: uint8 prefix + value)
    WPacket w(16);
    w.WriteInt64(255);
    ASSERT_EQ(10, w.GetPacketSize()); // 8 + 2
}

TEST(MsgpackCompact, UInt16_Value256) {
    // 256 → cd 01 00 (3 bytes: uint16 prefix + BE value)
    WPacket w(16);
    w.WriteInt64(256);
    ASSERT_EQ(11, w.GetPacketSize()); // 8 + 3
}

TEST(MsgpackCompact, UInt32_LargeValue) {
    // 0xFFFFFFFF → ce ff ff ff ff (5 bytes: uint32 prefix + BE value)
    WPacket w(16);
    w.WriteInt64(0xFFFFFFFF);
    ASSERT_EQ(13, w.GetPacketSize()); // 8 + 5
}

TEST(MsgpackCompact, Int64_Large) {
    // INT64_MAX → cf 7f ff ff ff ff ff ff ff (9 bytes)
    WPacket w(16);
    w.WriteInt64(INT64_MAX);
    ASSERT_EQ(17, w.GetPacketSize()); // 8 + 9
}

// ═══════════════════════════════════════════════════════════════
//  Группа 4: ReverseRead и DiscardLast
// ═══════════════════════════════════════════════════════════════

TEST(ReverseRead, UInt32_Single) {
    WPacket w(32);
    w.WriteCmd(300);
    w.WriteInt64(12345);

    RPacket r(w.Data(), w.GetPacketSize());
    ASSERT_EQ(12345u, r.ReverseReadInt64());
}

TEST(ReverseRead, Multiple_Values) {
    WPacket w(64);
    w.WriteCmd(301);
    w.WriteInt64(111); // elem 0
    w.WriteInt64(222); // elem 1
    w.WriteInt64(333); // elem 2

    RPacket r(w.Data(), w.GetPacketSize());
    // ReverseRead читает с конца
    ASSERT_EQ(333u, r.ReverseReadInt64());
    ASSERT_EQ(222u, r.ReverseReadInt64());
    ASSERT_EQ(111u, r.ReverseReadInt64());
}

TEST(ReverseRead, MixedTypes) {
    WPacket w(64);
    w.WriteInt64(10);    // elem 0
    w.WriteInt64(2000); // elem 1
    w.WriteInt64(30000);// elem 2

    RPacket r(w.Data(), w.GetPacketSize());
    ASSERT_EQ(30000u, r.ReverseReadInt64());
    ASSERT_EQ(2000, r.ReverseReadInt64());
    ASSERT_EQ(10, r.ReverseReadInt64());
}

TEST(ReverseRead, Int64) {
    WPacket w(32);
    w.WriteInt64(-999999LL);

    RPacket r(w.Data(), w.GetPacketSize());
    ASSERT_EQ(-999999LL, r.ReverseReadInt64());
}

TEST(DiscardLast, SingleElement) {
    WPacket w(32);
    w.WriteInt64(111);
    w.WriteInt64(222);

    RPacket r(w.Data(), w.GetPacketSize());
    ASSERT_TRUE(r.DiscardLast(1));

    // Осталось только 111
    ASSERT_EQ(111u, r.ReadInt64());
    ASSERT_FALSE(r.HasData());
}

TEST(DiscardLast, AllElements) {
    WPacket w(32);
    w.WriteInt64(111);
    w.WriteInt64(222);
    w.WriteInt64(333);

    RPacket r(w.Data(), w.GetPacketSize());
    ASSERT_TRUE(r.DiscardLast(3));
    ASSERT_EQ(8, r.GetPacketSize());
    ASSERT_FALSE(r.HasData());
}

TEST(DiscardLast, ZeroCount) {
    WPacket w(32);
    w.WriteInt64(111);

    RPacket r(w.Data(), w.GetPacketSize());
    ASSERT_TRUE(r.DiscardLast(0));
    ASSERT_EQ(111u, r.ReadInt64());
}

TEST(DiscardLast, EmptyPacket_ZeroCount) {
    WPacket w(16);

    RPacket r(w.Data(), w.GetPacketSize());
    ASSERT_TRUE(r.DiscardLast(0));
}

TEST(DiscardLastByReverseIndex, Basic) {
    WPacket w(64);
    w.WriteInt64(111); // elem 0
    w.WriteInt64(222); // elem 1
    w.WriteInt64(333); // elem 2
    w.WriteInt64(444); // elem 3

    RPacket r(w.Data(), w.GetPacketSize());
    // Читаем 2 элемента с конца
    ASSERT_EQ(444u, r.ReverseReadInt64());
    ASSERT_EQ(333u, r.ReverseReadInt64());

    // DiscardLastByReverseIndex: удалит прочитанные 333 и 444
    r.DiscardLastByReverseIndex();

    // Осталось 111 и 222
    r.ResetPosition();
    ASSERT_EQ(111u, r.ReadInt64());
    ASSERT_EQ(222u, r.ReadInt64());
    ASSERT_FALSE(r.HasData());
}

TEST(DiscardLastByReverseIndex, GateServerPattern) {
    // Паттерн из GateServer: ReverseRead данных из trailer, DiscardLastByReverseIndex, затем ForwardRead
    WPacket w(128);
    w.WriteCmd(500);
    w.WriteString("body_data");  // body
    w.WriteInt64(100);          // gpAddr (trailer)
    w.WriteInt64(200);          // loginId (trailer)
    w.WriteInt64(300);          // actId (trailer)
    w.WriteInt64(1);             // byPassword (trailer)

    RPacket r(w.Data(), w.GetPacketSize());
    ASSERT_EQ(500, r.GetCmd());

    // ReverseRead trailer
    uint8_t byPassword = r.ReverseReadInt64();
    uint32_t actId = r.ReverseReadInt64();
    uint32_t loginId = r.ReverseReadInt64();
    uint32_t gpAddr = r.ReverseReadInt64();

    ASSERT_EQ(1, byPassword);
    ASSERT_EQ(300u, actId);
    ASSERT_EQ(200u, loginId);
    ASSERT_EQ(100u, gpAddr);

    // Удалить trailer
    r.DiscardLastByReverseIndex();

    // ForwardRead body
    ASSERT_EQ(std::string("body_data"), r.ReadString());
    ASSERT_FALSE(r.HasData());
}

// ═══════════════════════════════════════════════════════════════
//  Группа 5: DiscardLast с routing pairs (паттерн ForwardMC)
// ═══════════════════════════════════════════════════════════════

TEST(DiscardLastRouting, ForwardMC_Pattern) {
    // Паттерн ForwardMC: body + routing pairs + aimnum
    WPacket w(128);
    w.WriteCmd(600);
    w.WriteString("action_data");  // body
    // Routing: 2 пары (playerId, dbid)
    w.WriteInt64(1001); // pair 0: playerId
    w.WriteInt64(2001); // pair 0: dbid
    w.WriteInt64(1002); // pair 1: playerId
    w.WriteInt64(2002); // pair 1: dbid
    w.WriteInt64(2);    // aimnum

    RPacket r(w.Data(), w.GetPacketSize());
    int aimnum = r.ReverseReadInt64();
    ASSERT_EQ(2, aimnum);

    uint32_t p1 = r.ReverseReadInt64();  // dbid pair 1
    uint32_t p1id = r.ReverseReadInt64(); // playerId pair 1
    ASSERT_EQ(2002u, p1);
    ASSERT_EQ(1002u, p1id);

    // DiscardLast: 2*aimnum + 1 элементов (пары + aimnum)
    r.DiscardLast(2 * aimnum + 1);

    r.ResetPosition();
    ASSERT_EQ(std::string("action_data"), r.ReadString());
    ASSERT_FALSE(r.HasData());
}

TEST(DiscardLastRouting, ForwardMC_ZeroTargets) {
    WPacket w(64);
    w.WriteCmd(601);
    w.WriteString("data");
    w.WriteInt64(0); // aimnum = 0

    RPacket r(w.Data(), w.GetPacketSize());
    int aimnum = r.ReverseReadInt64();
    ASSERT_EQ(0, aimnum);

    r.DiscardLast(2 * aimnum + 1); // discard just aimnum (1 element)

    r.ResetPosition();
    ASSERT_EQ(std::string("data"), r.ReadString());
    ASSERT_FALSE(r.HasData());
}

// ═══════════════════════════════════════════════════════════════
//  Группа 6: Skip
// ═══════════════════════════════════════════════════════════════

TEST(Skip, SkipElements) {
    WPacket w(64);
    w.WriteInt64(111);
    w.WriteInt64(222);
    w.WriteInt64(333);

    RPacket r(w.Data(), w.GetPacketSize());
    r.Skip(2); // пропустить 111 и 222
    ASSERT_EQ(333u, r.ReadInt64());
}

TEST(Skip, SkipString) {
    WPacket w(64);
    w.WriteString("skip_me");
    w.WriteInt64(42);

    RPacket r(w.Data(), w.GetPacketSize());
    r.Skip(1); // пропустить строку
    ASSERT_EQ(42u, r.ReadInt64());
}

// ═══════════════════════════════════════════════════════════════
//  Группа 7: WPacket из RPacket (копирование)
// ═══════════════════════════════════════════════════════════════

TEST(WPacketFromRPacket, CopyPreservesData) {
    WPacket orig(64);
    orig.WriteCmd(700);
    orig.WriteSess(0x12345678);
    orig.WriteInt64(42);
    orig.WriteString("copied");

    RPacket rpk(orig.Data(), orig.GetPacketSize());
    WPacket copy(rpk);

    ASSERT_EQ(700, copy.GetCmd());
    ASSERT_EQ(0x12345678u, copy.GetSess());

    // Читаем из копии
    RPacket r2(copy.Data(), copy.GetPacketSize());
    ASSERT_EQ(42u, r2.ReadInt64());
    ASSERT_EQ(std::string("copied"), r2.ReadString());
}

TEST(WPacketFromRPacket, AppendAfterCopy) {
    WPacket orig(32);
    orig.WriteCmd(701);
    orig.WriteInt64(100);

    RPacket rpk(orig.Data(), orig.GetPacketSize());
    WPacket copy(rpk);

    // Дописать в копию
    copy.WriteInt64(200);
    copy.WriteInt64(300);

    RPacket r(copy.Data(), copy.GetPacketSize());
    ASSERT_EQ(100u, r.ReadInt64());
    ASSERT_EQ(200u, r.ReadInt64());
    ASSERT_EQ(300u, r.ReadInt64());
}

// ═══════════════════════════════════════════════════════════════
//  Группа 8: HasData и RemainingBytes
// ═══════════════════════════════════════════════════════════════

TEST(State, HasData_Empty) {
    WPacket w(16);
    RPacket r(w.Data(), w.GetPacketSize());
    ASSERT_FALSE(r.HasData());
}

TEST(State, HasData_AfterRead) {
    WPacket w(32);
    w.WriteInt64(42);

    RPacket r(w.Data(), w.GetPacketSize());
    ASSERT_TRUE(r.HasData());
    r.ReadInt64();
    ASSERT_FALSE(r.HasData());
}

TEST(State, RemainingBytes_DecreasesOnRead) {
    WPacket w(32);
    w.WriteInt64(42);

    RPacket r(w.Data(), w.GetPacketSize());
    int before = r.RemainingBytes();
    ASSERT_TRUE(before > 0);
    r.ReadInt64();
    ASSERT_EQ(0, r.RemainingBytes());
}

// ═══════════════════════════════════════════════════════════════
//  Группа 9: ResetPosition
// ═══════════════════════════════════════════════════════════════

TEST(ResetPosition, ReReadsFromStart) {
    WPacket w(32);
    w.WriteInt64(111);
    w.WriteInt64(222);

    RPacket r(w.Data(), w.GetPacketSize());
    ASSERT_EQ(111u, r.ReadInt64());
    ASSERT_EQ(222u, r.ReadInt64());

    r.ResetPosition();
    ASSERT_EQ(111u, r.ReadInt64());
    ASSERT_EQ(222u, r.ReadInt64());
}

// ═══════════════════════════════════════════════════════════════
//  Группа 10: mpack_sequence_to_string
// ═══════════════════════════════════════════════════════════════

TEST(SequenceToString, AllTypes) {
    WPacket w(128);
    w.WriteInt64(42);
    w.WriteInt64(-100);
    w.WriteFloat32(3.14f);
    w.WriteString("hello");
    uint8_t bin[] = {0xAA, 0xBB};
    w.WriteSequence(bin, 2);

    std::string s = printPacket(w);
    std::cout << "    mpack_sequence_to_string: " << s << "\n";

    // Проверяем что все элементы присутствуют
    ASSERT_TRUE(s.find("|42") != std::string::npos);
    ASSERT_TRUE(s.find("|-100") != std::string::npos);
    ASSERT_TRUE(s.find("str(6)hello") != std::string::npos);
    ASSERT_TRUE(s.find("bin(2)") != std::string::npos);
}

TEST(SequenceToString, EmptyPayload) {
    std::string s = mpack_sequence_to_string(nullptr, 0);
    ASSERT_TRUE(s.empty());
}

// ═══════════════════════════════════════════════════════════════
//  Группа 11: Граничные значения и EnsureCapacity
// ═══════════════════════════════════════════════════════════════

TEST(Boundary, ManyWrites_TriggerResize) {
    WPacket w(8); // минимальный размер
    for (int i = 0; i < 100; ++i) {
        w.WriteInt64(static_cast<uint32_t>(i));
    }

    RPacket r(w.Data(), w.GetPacketSize());
    for (int i = 0; i < 100; ++i) {
        ASSERT_EQ(static_cast<uint32_t>(i), r.ReadInt64());
    }
}

TEST(Boundary, LongString) {
    std::string longStr(1000, 'A');
    WPacket w(16); // trigger resize
    w.WriteString(longStr);

    RPacket r(w.Data(), w.GetPacketSize());
    std::string result = r.ReadString();
    ASSERT_EQ(1000, (int)result.size());
    ASSERT_EQ(longStr, result);
}

TEST(Boundary, LargeSequence) {
    std::vector<uint8_t> data(500, 0xAB);
    WPacket w(16);
    w.WriteSequence(data.data(), static_cast<uint16_t>(data.size()));

    RPacket r(w.Data(), w.GetPacketSize());
    uint16_t len = 0;
    const char* ptr = r.ReadSequence(len);
    ASSERT_EQ(500, (int)len);
    ASSERT_EQ((char)0xAB, ptr[0]);
    ASSERT_EQ((char)0xAB, ptr[499]);
}

// ═══════════════════════════════════════════════════════════════
//  Группа 12: Move semantics
// ═══════════════════════════════════════════════════════════════

TEST(Move, WPacket_MoveConstruct) {
    WPacket w(32);
    w.WriteCmd(800);
    w.WriteInt64(42);

    WPacket moved(std::move(w));
    ASSERT_EQ(800, moved.GetCmd());

    RPacket r(moved.Data(), moved.GetPacketSize());
    ASSERT_EQ(42u, r.ReadInt64());
}

TEST(Move, RPacket_MoveConstruct) {
    WPacket w(32);
    w.WriteCmd(801);
    w.WriteInt64(99);

    RPacket r1(w.Data(), w.GetPacketSize());
    RPacket r2(std::move(r1));

    ASSERT_EQ(801, r2.GetCmd());
    ASSERT_EQ(99u, r2.ReadInt64());
}

// ═══════════════════════════════════════════════════════════════
//  main
// ═══════════════════════════════════════════════════════════════

int main() {
    return test::runAll();
}
