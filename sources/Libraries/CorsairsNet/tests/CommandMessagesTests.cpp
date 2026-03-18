// CommandMessagesTests.cpp — roundtrip тесты CommandMessages.h
// Паттерн: serialize → WPacket → RPacket → deserialize → assert fields equal

#include "TestFramework.h"
#include "CommandMessages.h"
#include <string>
#include <vector>

using namespace net;
using namespace net::msg;

// =================================================================
//  Группа A: TP/PT — GateServer <-> GroupServer
// =================================================================

TEST(CommandMessages, TpLoginRequest_Roundtrip) {
    TpLoginRequest original{103, "Gate1"};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    TpLoginRequest restored;
    deserialize(r, restored);
    ASSERT_EQ(original.protocolVersion, restored.protocolVersion);
    ASSERT_EQ(original.gateName, restored.gateName);
}

TEST(CommandMessages, TpLoginResponse_Roundtrip) {
    TpLoginResponse original{0};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    TpLoginResponse restored;
    deserialize(r, restored);
    ASSERT_EQ(original.errCode, restored.errCode);
}

TEST(CommandMessages, TpUserLoginRequest_Roundtrip) {
    TpUserLoginRequest original{"player1", "pass123", "AA:BB:CC", 0x7F000001u, 42u, 911};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    TpUserLoginRequest restored;
    deserialize(r, restored);
    ASSERT_EQ(original.acctName, restored.acctName);
    ASSERT_EQ(original.acctPassword, restored.acctPassword);
    ASSERT_EQ(original.acctMac, restored.acctMac);
    ASSERT_EQ(original.clientIp, restored.clientIp);
    ASSERT_EQ(original.gateAddr, restored.gateAddr);
    ASSERT_EQ(original.cheatMarker, restored.cheatMarker);
}

TEST(CommandMessages, TpUserLoginResponse_Success_Roundtrip) {
    TpUserLoginResponseData data{
        2,
        {{true, "Pirate1", "Warrior", 50, 1001, std::vector<int64_t>(EQUIP_NUM, 0)},
         {false, "", "", 0, 0, {}}},
        true, 12345, 67890, 100u
    };
    TpUserLoginResponse original{0, data};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    TpUserLoginResponse restored;
    deserialize(r, restored);
    ASSERT_EQ(original.errCode, restored.errCode);
    ASSERT_TRUE(restored.data.has_value());
    ASSERT_EQ(data.maxChaNum, restored.data->maxChaNum);
    ASSERT_EQ(2u, restored.data->characters.size());
    ASSERT_TRUE(restored.data->characters[0].valid);
    ASSERT_EQ(std::string("Pirate1"), restored.data->characters[0].chaName);
    ASSERT_FALSE(restored.data->characters[1].valid);
    ASSERT_TRUE(restored.data->hasPassword2);
    ASSERT_EQ(data.acctId, restored.data->acctId);
    ASSERT_EQ(data.acctLoginId, restored.data->acctLoginId);
    ASSERT_EQ(data.gpAddr, restored.data->gpAddr);
}

TEST(CommandMessages, TpUserLoginResponse_Error_Roundtrip) {
    TpUserLoginResponse original{-1, std::nullopt};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    TpUserLoginResponse restored;
    deserialize(r, restored);
    ASSERT_EQ(original.errCode, restored.errCode);
    ASSERT_FALSE(restored.data.has_value());
}

TEST(CommandMessages, TpUserLogoutRequest_Roundtrip) {
    TpUserLogoutRequest original{1u, 2u};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    TpUserLogoutRequest restored;
    deserialize(r, restored);
    ASSERT_EQ(original.gateAddr, restored.gateAddr);
    ASSERT_EQ(original.gpAddr, restored.gpAddr);
}

TEST(CommandMessages, TpUserLogoutResponse_Roundtrip) {
    TpUserLogoutResponse original{0};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    TpUserLogoutResponse restored;
    deserialize(r, restored);
    ASSERT_EQ(original.errCode, restored.errCode);
}

TEST(CommandMessages, TpBgnPlayRequest_Roundtrip) {
    TpBgnPlayRequest original{2, 10u, 20u};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    TpBgnPlayRequest restored;
    deserialize(r, restored);
    ASSERT_EQ(original.chaIndex, restored.chaIndex);
    ASSERT_EQ(original.gateAddr, restored.gateAddr);
    ASSERT_EQ(original.gpAddr, restored.gpAddr);
}

TEST(CommandMessages, TpBgnPlayResponse_Success_Roundtrip) {
    TpBgnPlayResponseData data{"secret", 100, 1, "garner", 42};
    TpBgnPlayResponse original{0, data};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    TpBgnPlayResponse restored;
    deserialize(r, restored);
    ASSERT_EQ(original.errCode, restored.errCode);
    ASSERT_TRUE(restored.data.has_value());
    ASSERT_EQ(data.password2, restored.data->password2);
    ASSERT_EQ(data.chaId, restored.data->chaId);
    ASSERT_EQ(data.worldId, restored.data->worldId);
    ASSERT_EQ(data.mapName, restored.data->mapName);
    ASSERT_EQ(data.swiner, restored.data->swiner);
}

TEST(CommandMessages, TpEndPlayRequest_Roundtrip) {
    TpEndPlayRequest original{5u, 6u};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    TpEndPlayRequest restored;
    deserialize(r, restored);
    ASSERT_EQ(original.gateAddr, restored.gateAddr);
    ASSERT_EQ(original.gpAddr, restored.gpAddr);
}

TEST(CommandMessages, TpEndPlayResponse_Success_Roundtrip) {
    TpEndPlayResponseData data{1, 1, {{true, "Hero", "Mage", 30, 2001, std::vector<int64_t>(EQUIP_NUM, 0)}}};
    TpEndPlayResponse original{0, data};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    TpEndPlayResponse restored;
    deserialize(r, restored);
    ASSERT_EQ(original.errCode, restored.errCode);
    ASSERT_TRUE(restored.data.has_value());
    ASSERT_EQ(data.maxChaNum, restored.data->maxChaNum);
    ASSERT_EQ(1u, restored.data->characters.size());
    ASSERT_TRUE(restored.data->characters[0].valid);
    ASSERT_EQ(std::string("Hero"), restored.data->characters[0].chaName);
}

TEST(CommandMessages, TpNewChaRequest_Roundtrip) {
    TpNewChaRequest original{"Hero", "1990-01-01", 1, 2, 3, 10u, 20u};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    TpNewChaRequest restored;
    deserialize(r, restored);
    ASSERT_EQ(original.chaName, restored.chaName);
    ASSERT_EQ(original.birth, restored.birth);
    ASSERT_EQ(original.typeId, restored.typeId);
    ASSERT_EQ(original.hairId, restored.hairId);
    ASSERT_EQ(original.faceId, restored.faceId);
    ASSERT_EQ(original.gateAddr, restored.gateAddr);
    ASSERT_EQ(original.gpAddr, restored.gpAddr);
}

TEST(CommandMessages, TpNewChaResponse_Roundtrip) {
    TpNewChaResponse original{0};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    TpNewChaResponse restored;
    deserialize(r, restored);
    ASSERT_EQ(original.errCode, restored.errCode);
}

TEST(CommandMessages, TpDelChaRequest_Roundtrip) {
    TpDelChaRequest original{1, "pin123", 10u, 20u};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    TpDelChaRequest restored;
    deserialize(r, restored);
    ASSERT_EQ(original.chaIndex, restored.chaIndex);
    ASSERT_EQ(original.password2, restored.password2);
    ASSERT_EQ(original.gateAddr, restored.gateAddr);
    ASSERT_EQ(original.gpAddr, restored.gpAddr);
}

TEST(CommandMessages, TpDelChaResponse_Roundtrip) {
    TpDelChaResponse original{0};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    TpDelChaResponse restored;
    deserialize(r, restored);
    ASSERT_EQ(original.errCode, restored.errCode);
}

TEST(CommandMessages, TpChangePassRequest_Roundtrip) {
    TpChangePassRequest original{"new123", "pin456", 1u, 2u};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    TpChangePassRequest restored;
    deserialize(r, restored);
    ASSERT_EQ(original.newPass, restored.newPass);
    ASSERT_EQ(original.pin, restored.pin);
    ASSERT_EQ(original.gateAddr, restored.gateAddr);
    ASSERT_EQ(original.gpAddr, restored.gpAddr);
}

TEST(CommandMessages, TpRegisterRequest_Roundtrip) {
    TpRegisterRequest original{"user1", "pw1", "a@b.com"};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    TpRegisterRequest restored;
    deserialize(r, restored);
    ASSERT_EQ(original.username, restored.username);
    ASSERT_EQ(original.password, restored.password);
    ASSERT_EQ(original.email, restored.email);
}

TEST(CommandMessages, TpRegisterResponse_Roundtrip) {
    TpRegisterResponse original{1, "OK"};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    TpRegisterResponse restored;
    deserialize(r, restored);
    ASSERT_EQ(original.result, restored.result);
    ASSERT_EQ(original.message, restored.message);
}

TEST(CommandMessages, TpCreatePassword2Request_Roundtrip) {
    TpCreatePassword2Request original{"secret", 1u, 2u};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    TpCreatePassword2Request restored;
    deserialize(r, restored);
    ASSERT_EQ(original.password2, restored.password2);
    ASSERT_EQ(original.gateAddr, restored.gateAddr);
    ASSERT_EQ(original.gpAddr, restored.gpAddr);
}

TEST(CommandMessages, TpUpdatePassword2Request_Roundtrip) {
    TpUpdatePassword2Request original{"old", "new", 1u, 2u};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    TpUpdatePassword2Request restored;
    deserialize(r, restored);
    ASSERT_EQ(original.oldPassword2, restored.oldPassword2);
    ASSERT_EQ(original.newPassword2, restored.newPassword2);
    ASSERT_EQ(original.gateAddr, restored.gateAddr);
    ASSERT_EQ(original.gpAddr, restored.gpAddr);
}

TEST(CommandMessages, TpPassword2Response_Roundtrip) {
    TpPassword2Response original{0};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    TpPassword2Response restored;
    deserialize(r, restored);
    ASSERT_EQ(original.errCode, restored.errCode);
}

TEST(CommandMessages, TpSyncPlyLstRequest_Roundtrip) {
    TpSyncPlyLstRequest original{
        2, "Gate1",
        {{100, 200, 300}, {400, 500, 600}}
    };
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    TpSyncPlyLstRequest restored;
    deserialize(r, restored);
    ASSERT_EQ(original.num, restored.num);
    ASSERT_EQ(original.gateName, restored.gateName);
    ASSERT_EQ(2u, restored.players.size());
    ASSERT_EQ(original.players[0].gtAddr, restored.players[0].gtAddr);
    ASSERT_EQ(original.players[1].acctId, restored.players[1].acctId);
}

TEST(CommandMessages, TpSyncPlyLstResponse_Roundtrip) {
    TpSyncPlyLstResponse original{0, 2, {{true, 111}, {false, 222}}};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    TpSyncPlyLstResponse restored;
    deserialize(r, restored);
    ASSERT_EQ(original.errCode, restored.errCode);
    ASSERT_EQ(original.num, restored.num);
    ASSERT_EQ(2u, restored.results.size());
    ASSERT_TRUE(restored.results[0].ok);
    ASSERT_FALSE(restored.results[1].ok);
}

TEST(CommandMessages, OsLoginRequest_Roundtrip) {
    OsLoginRequest original{1, "agent1"};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    OsLoginRequest restored;
    deserialize(r, restored);
    ASSERT_EQ(original.version, restored.version);
    ASSERT_EQ(original.agentName, restored.agentName);
}

TEST(CommandMessages, OsLoginResponse_Roundtrip) {
    OsLoginResponse original{0};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    OsLoginResponse restored;
    deserialize(r, restored);
    ASSERT_EQ(original.errCode, restored.errCode);
}

TEST(CommandMessages, TpDiscMessage_Roundtrip) {
    TpDiscMessage original{1, 2, "timeout"};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    TpDiscMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.actId, restored.actId);
    ASSERT_EQ(original.ipAddr, restored.ipAddr);
    ASSERT_EQ(original.reason, restored.reason);
}

// =================================================================
//  Группа B: PA/AP — GroupServer <-> AccountServer
// =================================================================

TEST(CommandMessages, PaLoginRequest_Roundtrip) {
    PaLoginRequest original{"server1", "secret"};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    PaLoginRequest restored;
    deserialize(r, restored);
    ASSERT_EQ(original.serverName, restored.serverName);
    ASSERT_EQ(original.serverPassword, restored.serverPassword);
}

TEST(CommandMessages, PaLoginResponse_Roundtrip) {
    PaLoginResponse original{0};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    PaLoginResponse restored;
    deserialize(r, restored);
    ASSERT_EQ(original.errCode, restored.errCode);
}

TEST(CommandMessages, PaUserLoginRequest_Roundtrip) {
    PaUserLoginRequest original{"user1", "pass", "AA:BB", 12345};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    PaUserLoginRequest restored;
    deserialize(r, restored);
    ASSERT_EQ(original.username, restored.username);
    ASSERT_EQ(original.password, restored.password);
    ASSERT_EQ(original.mac, restored.mac);
    ASSERT_EQ(original.clientIp, restored.clientIp);
}

TEST(CommandMessages, PaUserLoginResponse_Success_Roundtrip) {
    PaUserLoginResponseData data{42, 99, 42, 0};
    PaUserLoginResponse original{0, data};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    PaUserLoginResponse restored;
    deserialize(r, restored);
    ASSERT_EQ(original.errCode, restored.errCode);
    ASSERT_TRUE(restored.data.has_value());
    ASSERT_EQ(data.acctLoginId, restored.data->acctLoginId);
    ASSERT_EQ(data.sessId, restored.data->sessId);
    ASSERT_EQ(data.acctId, restored.data->acctId);
    ASSERT_EQ(data.gmLevel, restored.data->gmLevel);
}

TEST(CommandMessages, PaUserLoginResponse_Error_Roundtrip) {
    PaUserLoginResponse original{3, std::nullopt};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    PaUserLoginResponse restored;
    deserialize(r, restored);
    ASSERT_EQ(3, restored.errCode);
    ASSERT_FALSE(restored.data.has_value());
}

TEST(CommandMessages, PaUserLogoutMessage_Roundtrip) {
    PaUserLogoutMessage original{42};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    PaUserLogoutMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.acctLoginId, restored.acctLoginId);
}

TEST(CommandMessages, PaUserBillBgnMessage_Roundtrip) {
    PaUserBillBgnMessage original{"player1", "passport1"};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    PaUserBillBgnMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.acctName, restored.acctName);
    ASSERT_EQ(original.passport, restored.passport);
}

TEST(CommandMessages, PaUserBillEndMessage_Roundtrip) {
    PaUserBillEndMessage original{"player1"};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    PaUserBillEndMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.acctName, restored.acctName);
}

TEST(CommandMessages, PaChangePassMessage_Roundtrip) {
    PaChangePassMessage original{"user1", "newpass"};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    PaChangePassMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.username, restored.username);
    ASSERT_EQ(original.newPassword, restored.newPassword);
}

TEST(CommandMessages, PaRegisterMessage_Roundtrip) {
    PaRegisterMessage original{"user1", "pass1", "a@b.com"};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    PaRegisterMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.username, restored.username);
    ASSERT_EQ(original.password, restored.password);
    ASSERT_EQ(original.email, restored.email);
}

TEST(CommandMessages, PaGmBanMessage_Roundtrip) {
    PaGmBanMessage original{"badplayer"};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    PaGmBanMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.actName, restored.actName);
}

TEST(CommandMessages, PaGmUnbanMessage_Roundtrip) {
    PaGmUnbanMessage original{"goodplayer"};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    PaGmUnbanMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.actName, restored.actName);
}

TEST(CommandMessages, PaUserDisableMessage_Roundtrip) {
    PaUserDisableMessage original{42, 60};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    PaUserDisableMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.acctLoginId, restored.acctLoginId);
    ASSERT_EQ(original.minutes, restored.minutes);
}

TEST(CommandMessages, ApKickUserMessage_Roundtrip) {
    ApKickUserMessage original{1, 999};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    ApKickUserMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.errCode, restored.errCode);
    ASSERT_EQ(original.accountId, restored.accountId);
}

TEST(CommandMessages, ApExpScaleMessage_Roundtrip) {
    ApExpScaleMessage original{42, 3600};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    ApExpScaleMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.chaId, restored.chaId);
    ASSERT_EQ(original.time, restored.time);
}

// =================================================================
//  Группа C: CP — Client -> GroupServer
// =================================================================

TEST(CommandMessages, CpTeamInviteMessage_Roundtrip) {
    CpTeamInviteMessage original{"friend1", 10u, 200u};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    CpTeamInviteMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.invitedName, restored.invitedName);
    ASSERT_EQ(original.gateAddr, restored.gateAddr);
    ASSERT_EQ(original.gpAddr, restored.gpAddr);
}

TEST(CommandMessages, CpTeamAcceptMessage_Roundtrip) {
    CpTeamAcceptMessage original{5, 1u, 2u};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    CpTeamAcceptMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.inviterChaId, restored.inviterChaId);
    ASSERT_EQ(original.gateAddr, restored.gateAddr);
    ASSERT_EQ(original.gpAddr, restored.gpAddr);
}

TEST(CommandMessages, CpTeamLeaveMessage_Roundtrip) {
    CpTeamLeaveMessage original{1u, 2u};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    CpTeamLeaveMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.gateAddr, restored.gateAddr);
    ASSERT_EQ(original.gpAddr, restored.gpAddr);
}

TEST(CommandMessages, CpSay2AllMessage_Roundtrip) {
    CpSay2AllMessage original{"Hello world!", 1u, 2u};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    CpSay2AllMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.content, restored.content);
    ASSERT_EQ(original.gateAddr, restored.gateAddr);
    ASSERT_EQ(original.gpAddr, restored.gpAddr);
}

TEST(CommandMessages, CpSay2YouMessage_Roundtrip) {
    CpSay2YouMessage original{"target", "msg", 1u, 2u};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    CpSay2YouMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.targetName, restored.targetName);
    ASSERT_EQ(original.content, restored.content);
    ASSERT_EQ(original.gateAddr, restored.gateAddr);
    ASSERT_EQ(original.gpAddr, restored.gpAddr);
}

TEST(CommandMessages, CpSessCreateMessage_Roundtrip) {
    CpSessCreateMessage original{3, {"a", "b", "c"}, 1u, 2u};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    CpSessCreateMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.chaNum, restored.chaNum);
    ASSERT_EQ(3u, restored.chaNames.size());
    ASSERT_EQ(std::string("a"), restored.chaNames[0]);
    ASSERT_EQ(std::string("b"), restored.chaNames[1]);
    ASSERT_EQ(std::string("c"), restored.chaNames[2]);
    ASSERT_EQ(original.gateAddr, restored.gateAddr);
    ASSERT_EQ(original.gpAddr, restored.gpAddr);
}

TEST(CommandMessages, CpChangePersonInfoMessage_Roundtrip) {
    CpChangePersonInfoMessage original{"Hello!", 5, 0, 1u, 2u};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    CpChangePersonInfoMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.motto, restored.motto);
    ASSERT_EQ(original.icon, restored.icon);
    ASSERT_EQ(original.refuseSess, restored.refuseSess);
    ASSERT_EQ(original.gateAddr, restored.gateAddr);
    ASSERT_EQ(original.gpAddr, restored.gpAddr);
}

TEST(CommandMessages, CpPingMessage_Roundtrip) {
    CpPingMessage original{42, 1u, 2u};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    CpPingMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.pingValue, restored.pingValue);
    ASSERT_EQ(original.gateAddr, restored.gateAddr);
    ASSERT_EQ(original.gpAddr, restored.gpAddr);
}

TEST(CommandMessages, CpChangePassMessage_Roundtrip) {
    CpChangePassMessage original{"new123", "pin456", 1u, 2u};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    CpChangePassMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.newPass, restored.newPass);
    ASSERT_EQ(original.pin, restored.pin);
    ASSERT_EQ(original.gateAddr, restored.gateAddr);
    ASSERT_EQ(original.gpAddr, restored.gpAddr);
}

// =================================================================
//  Группа D: MP — GameServer -> GroupServer
// =================================================================

TEST(CommandMessages, MpEnterMapMessage_Roundtrip) {
    MpEnterMapMessage original{1, 10u, 20u};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    MpEnterMapMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.isSwitch, restored.isSwitch);
    ASSERT_EQ(original.gateAddr, restored.gateAddr);
    ASSERT_EQ(original.gpAddr, restored.gpAddr);
}

TEST(CommandMessages, MpGuildCreateMessage_Roundtrip) {
    MpGuildCreateMessage original{42, "Pirates", "Leader", 10, 1u, 2u};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    MpGuildCreateMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.guildId, restored.guildId);
    ASSERT_EQ(original.gldName, restored.gldName);
    ASSERT_EQ(original.job, restored.job);
    ASSERT_EQ(original.degree, restored.degree);
    ASSERT_EQ(original.gateAddr, restored.gateAddr);
    ASSERT_EQ(original.gpAddr, restored.gpAddr);
}

TEST(CommandMessages, MpMasterCreateMessage_Roundtrip) {
    MpMasterCreateMessage original{"student", 1, "teacher", 2};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    MpMasterCreateMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.prenticeName, restored.prenticeName);
    ASSERT_EQ(original.prenticeChaid, restored.prenticeChaid);
    ASSERT_EQ(original.masterName, restored.masterName);
    ASSERT_EQ(original.masterChaid, restored.masterChaid);
}

TEST(CommandMessages, MpSay2AllMessage_Roundtrip) {
    MpSay2AllMessage original{1, "Hero", "Hi all!", 1u, 2u};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    MpSay2AllMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.succ, restored.succ);
    ASSERT_EQ(original.chaName, restored.chaName);
    ASSERT_EQ(original.content, restored.content);
    ASSERT_EQ(original.gateAddr, restored.gateAddr);
    ASSERT_EQ(original.gpAddr, restored.gpAddr);
}

TEST(CommandMessages, MpGuildChallMoneyMessage_Roundtrip) {
    MpGuildChallMoneyMessage original{1, 1000, "Guild1", "Guild2", 10u, 20u};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    MpGuildChallMoneyMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.guildId, restored.guildId);
    ASSERT_EQ(original.money, restored.money);
    ASSERT_EQ(original.guildName1, restored.guildName1);
    ASSERT_EQ(original.guildName2, restored.guildName2);
    ASSERT_EQ(original.gateAddr, restored.gateAddr);
    ASSERT_EQ(original.gpAddr, restored.gpAddr);
}

TEST(CommandMessages, MpGuildNoticeMessage_Roundtrip) {
    MpGuildNoticeMessage original{42, "Important notice"};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    MpGuildNoticeMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.guildId, restored.guildId);
    ASSERT_EQ(original.content, restored.content);
}

TEST(CommandMessages, MpGarner2UpdateMessage_Roundtrip) {
    MpGarner2UpdateMessage original{1, "Hero", 50, "Warrior", 9999, 1u, 2u};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    MpGarner2UpdateMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.nid, restored.nid);
    ASSERT_EQ(original.chaName, restored.chaName);
    ASSERT_EQ(original.level, restored.level);
    ASSERT_EQ(original.job, restored.job);
    ASSERT_EQ(original.fightpoint, restored.fightpoint);
    ASSERT_EQ(original.gateAddr, restored.gateAddr);
    ASSERT_EQ(original.gpAddr, restored.gpAddr);
}

// =================================================================
//  Группа E: PC — GroupServer -> Client
// =================================================================

TEST(CommandMessages, PcSay2AllMessage_Roundtrip) {
    PcSay2AllMessage original{"Hero", "Hello world!", 0xFF0000};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    PcSay2AllMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.chaName, restored.chaName);
    ASSERT_EQ(original.content, restored.content);
    ASSERT_EQ(original.color, restored.color);
}

TEST(CommandMessages, PcSay2YouMessage_Roundtrip) {
    PcSay2YouMessage original{"Hero", "Victim", "Hi!", 0x00FF00};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    PcSay2YouMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.sender, restored.sender);
    ASSERT_EQ(original.target, restored.target);
    ASSERT_EQ(original.content, restored.content);
    ASSERT_EQ(original.color, restored.color);
}

TEST(CommandMessages, PcTeamInviteMessage_Roundtrip) {
    PcTeamInviteMessage original{"inviter", 42, 5};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    PcTeamInviteMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.inviterName, restored.inviterName);
    ASSERT_EQ(original.chaId, restored.chaId);
    ASSERT_EQ(original.icon, restored.icon);
}

TEST(CommandMessages, PcTeamRefreshMessage_Roundtrip) {
    PcTeamRefreshMessage original{
        1, 2,
        {{100, "Hero", "Hi", 1}, {200, "Mage", "Hello", 2}}
    };
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    PcTeamRefreshMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.msg, restored.msg);
    ASSERT_EQ(original.count, restored.count);
    ASSERT_EQ(2u, restored.members.size());
    ASSERT_EQ(original.members[0].chaId, restored.members[0].chaId);
    ASSERT_EQ(original.members[0].chaName, restored.members[0].chaName);
    ASSERT_EQ(original.members[1].chaId, restored.members[1].chaId);
}

TEST(CommandMessages, PcFrndRefreshMessage_Roundtrip) {
    PcFrndRefreshMessage original{1, "friends", 42, "Hero", "My motto", 5};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    PcFrndRefreshMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.msg, restored.msg);
    ASSERT_EQ(original.group, restored.group);
    ASSERT_EQ(original.chaId, restored.chaId);
    ASSERT_EQ(original.chaName, restored.chaName);
    ASSERT_EQ(original.motto, restored.motto);
    ASSERT_EQ(original.icon, restored.icon);
}

TEST(CommandMessages, PcFrndRefreshInfoMessage_Roundtrip) {
    PcFrndRefreshInfoMessage original{42, "Hello", 5, 50, "Warrior", "Pirates"};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    PcFrndRefreshInfoMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.chaId, restored.chaId);
    ASSERT_EQ(original.motto, restored.motto);
    ASSERT_EQ(original.icon, restored.icon);
    ASSERT_EQ(original.degree, restored.degree);
    ASSERT_EQ(original.job, restored.job);
    ASSERT_EQ(original.guildName, restored.guildName);
}

TEST(CommandMessages, PcSessCreateMessage_Roundtrip) {
    PcSessCreateMessage original{
        1,
        {{10, "Hero", "Hi", 1}, {20, "Mage", "Hey", 2}},
        5
    };
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    PcSessCreateMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.sessId, restored.sessId);
    ASSERT_EQ(2u, restored.members.size());
    ASSERT_EQ(original.members[0].chaId, restored.members[0].chaId);
    ASSERT_EQ(original.members[0].chaName, restored.members[0].chaName);
    ASSERT_EQ(original.members[1].chaId, restored.members[1].chaId);
    ASSERT_EQ(original.notiPlyCount, restored.notiPlyCount);
}

TEST(CommandMessages, PcErrMsgMessage_Roundtrip) {
    PcErrMsgMessage original{"Error occurred"};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    PcErrMsgMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.message, restored.message);
}

TEST(CommandMessages, PcRegisterMessage_Roundtrip) {
    PcRegisterMessage original{1, "Success"};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    PcRegisterMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.result, restored.result);
    ASSERT_EQ(original.message, restored.message);
}

// =================================================================
//  Группа F: PM — GroupServer -> GameServer
// =================================================================

TEST(CommandMessages, PmTeamMessage_Roundtrip) {
    PmTeamMessage original{
        1, 2,
        {{"Gate1", 100, 200}, {"Gate2", 300, 400}}
    };
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    PmTeamMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.msg, restored.msg);
    ASSERT_EQ(original.count, restored.count);
    ASSERT_EQ(2u, restored.members.size());
    ASSERT_EQ(original.members[0].gateName, restored.members[0].gateName);
    ASSERT_EQ(original.members[0].gtAddr, restored.members[0].gtAddr);
    ASSERT_EQ(original.members[1].chaId, restored.members[1].chaId);
}

TEST(CommandMessages, PmExpScaleMessage_Roundtrip) {
    PmExpScaleMessage original{42, 3600};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    PmExpScaleMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.chaId, restored.chaId);
    ASSERT_EQ(original.time, restored.time);
}

TEST(CommandMessages, PmSay2AllMessage_Roundtrip) {
    PmSay2AllMessage original{42, "broadcast", 1000};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    PmSay2AllMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.chaId, restored.chaId);
    ASSERT_EQ(original.content, restored.content);
    ASSERT_EQ(original.money, restored.money);
}

TEST(CommandMessages, PmGuildDisbandMessage_Roundtrip) {
    PmGuildDisbandMessage original{42};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    PmGuildDisbandMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.guildId, restored.guildId);
}

TEST(CommandMessages, PmGuildChallMoneyMessage_Roundtrip) {
    PmGuildChallMoneyMessage original{1, 5000, "G1", "G2"};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    PmGuildChallMoneyMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.leaderId, restored.leaderId);
    ASSERT_EQ(original.money, restored.money);
    ASSERT_EQ(original.guildName1, restored.guildName1);
    ASSERT_EQ(original.guildName2, restored.guildName2);
}

TEST(CommandMessages, PmGuildChallPrizeMoneyMessage_Roundtrip) {
    PmGuildChallPrizeMoneyMessage original{1, 3000};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    PmGuildChallPrizeMoneyMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.leaderId, restored.leaderId);
    ASSERT_EQ(original.money, restored.money);
}

TEST(CommandMessages, PtKickUserMessage_Roundtrip) {
    PtKickUserMessage original{100, 200};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    PtKickUserMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.gpAddr, restored.gpAddr);
    ASSERT_EQ(original.gtAddr, restored.gtAddr);
}

// ═══════════════════════════════════════════════════════════════
//  Группа D: MC — GateServer → Client
// ═══════════════════════════════════════════════════════════════

TEST(CommandMessages, McLoginResponse_Success_Roundtrip) {
    McLoginResponse original;
    original.errCode = 0;
    McLoginResponseData d;
    d.maxChaNum = 3;
    std::vector<int64_t> eqIds(EQUIP_NUM);
    for (int i = 0; i < EQUIP_NUM; ++i) eqIds[i] = i + 1;
    d.characters = {
        {true, "Pirate", "Warrior", 10, 1, eqIds},
        {false, "", "", 0, 0, {}},
        {true, "Corsair", "Mage", 25, 3, std::vector<int64_t>(EQUIP_NUM, 0)}
    };
    d.hasPassword2 = true;
    original.data = std::move(d);

    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McLoginResponse restored;
    deserialize(r, restored);

    ASSERT_EQ(0, restored.errCode);
    ASSERT_TRUE(restored.data.has_value());
    ASSERT_EQ(3, restored.data->maxChaNum);
    ASSERT_EQ(3u, restored.data->characters.size());
    ASSERT_TRUE(restored.data->characters[0].valid);
    ASSERT_EQ("Pirate", restored.data->characters[0].chaName);
    ASSERT_EQ("Warrior", restored.data->characters[0].job);
    ASSERT_EQ(10, restored.data->characters[0].degree);
    ASSERT_EQ(1, restored.data->characters[0].typeId);
    ASSERT_EQ(EQUIP_NUM, static_cast<int>(restored.data->characters[0].equipIds.size()));
    ASSERT_EQ(1, restored.data->characters[0].equipIds[0]);
    ASSERT_EQ(34, restored.data->characters[0].equipIds[33]);
    ASSERT_FALSE(restored.data->characters[1].valid);
    ASSERT_TRUE(restored.data->characters[2].valid);
    ASSERT_EQ("Corsair", restored.data->characters[2].chaName);
    ASSERT_EQ(EQUIP_NUM, static_cast<int>(restored.data->characters[2].equipIds.size()));
    ASSERT_TRUE(restored.data->hasPassword2);
}

TEST(CommandMessages, McLoginResponse_Error_Roundtrip) {
    McLoginResponse original;
    original.errCode = 1002;

    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McLoginResponse restored;
    deserialize(r, restored);

    ASSERT_EQ(1002, restored.errCode);
    ASSERT_FALSE(restored.data.has_value());
}

TEST(CommandMessages, McBgnPlayResponse_Roundtrip) {
    McBgnPlayResponse original{42};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McBgnPlayResponse restored;
    deserialize(r, restored);
    ASSERT_EQ(original.errCode, restored.errCode);
}

TEST(CommandMessages, McEndPlayResponse_Success_Roundtrip) {
    McEndPlayResponseData data;
    data.maxChaNum = 3;
    data.characters = {
        {true, "Pirate", "Sailor", 50, 2, {1, 2, 3}},
        {false, "", "", 0, 0, {}},
        {true, "Ninja", "Scout", 30, 1, {10}}
    };
    McEndPlayResponse original{0, data};

    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McEndPlayResponse restored;
    deserialize(r, restored);

    ASSERT_EQ(0, restored.errCode);
    ASSERT_TRUE(restored.data.has_value());
    ASSERT_EQ(3, restored.data->maxChaNum);
    ASSERT_EQ(3u, restored.data->characters.size());
    ASSERT_TRUE(restored.data->characters[0].valid);
    ASSERT_EQ("Pirate", restored.data->characters[0].chaName);
    ASSERT_EQ(50, restored.data->characters[0].degree);
    ASSERT_FALSE(restored.data->characters[1].valid);
    ASSERT_TRUE(restored.data->characters[2].valid);
    ASSERT_EQ("Ninja", restored.data->characters[2].chaName);
}

TEST(CommandMessages, McEndPlayResponse_Error_Roundtrip) {
    McEndPlayResponse original{-1, std::nullopt};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McEndPlayResponse restored;
    deserialize(r, restored);
    ASSERT_EQ(-1, restored.errCode);
    ASSERT_FALSE(restored.data.has_value());
}

TEST(CommandMessages, McNewChaResponse_Roundtrip) {
    McNewChaResponse original{0};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McNewChaResponse restored;
    deserialize(r, restored);
    ASSERT_EQ(original.errCode, restored.errCode);
}

TEST(CommandMessages, McDelChaResponse_Roundtrip) {
    McDelChaResponse original{-3};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McDelChaResponse restored;
    deserialize(r, restored);
    ASSERT_EQ(original.errCode, restored.errCode);
}

TEST(CommandMessages, McCreatePassword2Response_Roundtrip) {
    McCreatePassword2Response original{0};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McCreatePassword2Response restored;
    deserialize(r, restored);
    ASSERT_EQ(original.errCode, restored.errCode);
}

TEST(CommandMessages, McUpdatePassword2Response_Roundtrip) {
    McUpdatePassword2Response original{-5};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McUpdatePassword2Response restored;
    deserialize(r, restored);
    ASSERT_EQ(original.errCode, restored.errCode);
}

// =================================================================
//  CMD_MC_ENTERMAP — roundtrip тесты
// =================================================================

TEST(CommandMessages, McEnterMapMessage_Error_Roundtrip) {
    McEnterMapMessage original;
    original.errCode = 42;
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McEnterMapMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(42, restored.errCode);
    ASSERT_FALSE(restored.data.has_value());
}

// testIsBoatItem больше не нужен — isBoat читается из потока

TEST(CommandMessages, McEnterMapMessage_Success_Roundtrip) {
    McEnterMapMessage original;
    original.errCode = 0;
    McEnterMapData d;
    d.autoLock = 1;
    d.kitbagLock = 0;
    d.enterType = 2;
    d.isNewCha = 0;
    d.mapName = "garner";
    d.canTeam = 1;
    d.imp = 500;

    // BaseInfo
    auto& b = d.baseInfo;
    b.chaId = 100;
    b.worldId = 200;
    b.commId = 300;
    b.commName = "comm1";
    b.gmLv = 0;
    b.handle = 1;
    b.ctrlType = 1;
    b.name = "TestPirate";
    b.motto = "Yarrr";
    b.icon = 5;
    b.guildId = 42;
    b.guildName = "Pirates";
    b.guildMotto = "We sail!";
    b.guildPermission = 7;
    b.stallName = "";
    b.state = 0;
    b.posX = 1000;
    b.posY = 2000;
    b.radius = 50;
    b.angle = 90;
    b.teamLeaderId = 0;
    b.isPlayer = 1;
    b.side.sideId = 2;
    b.event = {0, 0, 0, ""};
    // Look — player, not boat
    b.look.synType = 0; // SWITCH
    b.look.typeId = 1;
    b.look.isBoat = false;
    b.look.hairId = 2001;
    // Equip slot 0 — с полными данными
    b.look.equips[0].id = 5001;
    b.look.equips[0].dbId = 10001;
    b.look.equips[0].needLv = 10;
    b.look.equips[0].num = 1;
    b.look.equips[0].endure0 = 80;
    b.look.equips[0].endure1 = 100;
    b.look.equips[0].energy0 = 50;
    b.look.equips[0].energy1 = 60;
    b.look.equips[0].forgeLv = 3;
    b.look.equips[0].valid = 1;
    b.look.equips[0].tradable = 1;
    b.look.equips[0].expiration = 0;
    b.look.equips[0].hasExtra = true;
    b.look.equips[0].forgeParam = 12345;
    b.look.equips[0].instId = 67890;
    b.look.equips[0].hasInstAttr = true;
    b.look.equips[0].instAttr[0][0] = 1;
    b.look.equips[0].instAttr[0][1] = 100;
    b.look.equips[0].instAttr[1][0] = 2;
    b.look.equips[0].instAttr[1][1] = 200;
    // equips[1..33] — id=0, пустые
    b.pkCtrl = 3;
    b.appendLook[0].lookId = 8001;
    b.appendLook[0].valid = 1;

    // SkillBag
    d.skillBag.defSkillId = 100;
    d.skillBag.synType = 0;
    d.skillBag.skills = {
        {1, 1, 5, 10, 5, 3, 1000, {1, 100, 200, 300}},
        {2, 0, 3, 20, 10, 5, 2000, {0, 0, 0, 0}}
    };

    // SkillState
    d.skillState.currentTime = 123456;
    d.skillState.states = {{10, 2, 5000, 120000}, {20, 1, 3000, 121000}};

    // Attr
    d.attr.synType = 0;
    d.attr.attrs = {{1, 50}, {2, 30}, {5, 100}};

    // Kitbag — обычный предмет + предмет-корабль + пустой слот
    d.kitbag.synType = SYN_KITBAG_INIT;
    d.kitbag.capacity = 24;
    KitbagItem normalItem;
    normalItem.gridId = 0;
    normalItem.itemId = 1001;
    normalItem.dbId = 50001;
    normalItem.needLv = 5;
    normalItem.num = 1;
    normalItem.endure0 = 90;
    normalItem.endure1 = 100;
    normalItem.energy0 = 0;
    normalItem.energy1 = 0;
    normalItem.forgeLv = 0;
    normalItem.valid = 1;
    normalItem.tradable = 1;
    normalItem.expiration = 0;
    normalItem.isBoat = false;
    normalItem.forgeParam = 0;
    normalItem.instId = 111;
    normalItem.hasInstAttr = false;
    KitbagItem boatItem;
    boatItem.gridId = 5;
    boatItem.itemId = 9999; // testIsBoatItem → true
    boatItem.dbId = 50002;
    boatItem.needLv = 1;
    boatItem.num = 1;
    boatItem.endure0 = 100;
    boatItem.endure1 = 100;
    boatItem.energy0 = 50;
    boatItem.energy1 = 50;
    boatItem.forgeLv = 0;
    boatItem.valid = 1;
    boatItem.tradable = 0;
    boatItem.expiration = 0;
    boatItem.isBoat = true;
    boatItem.boatWorldId = 77777;
    boatItem.forgeParam = 0;
    boatItem.instId = 0;
    boatItem.hasInstAttr = true;
    boatItem.instAttr[0][0] = 10;
    boatItem.instAttr[0][1] = 20;
    KitbagItem emptyItem;
    emptyItem.gridId = 10;
    emptyItem.itemId = 0;
    d.kitbag.items = {normalItem, boatItem, emptyItem};

    // Shortcut
    d.shortcut.entries[0] = {1, 100};
    d.shortcut.entries[1] = {2, 200};

    d.ctrlChaId = 200;

    original.data = std::move(d);
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McEnterMapMessage restored;
    deserialize(r, restored);

    ASSERT_EQ(0, restored.errCode);
    ASSERT_TRUE(restored.data.has_value());
    const auto& rd = restored.data.value();

    ASSERT_EQ(1, rd.autoLock);
    ASSERT_EQ(0, rd.kitbagLock);
    ASSERT_EQ(2, rd.enterType);
    ASSERT_EQ(0, rd.isNewCha);
    ASSERT_EQ("garner", rd.mapName);
    ASSERT_EQ(1, rd.canTeam);
    ASSERT_EQ(500, rd.imp);

    // BaseInfo
    ASSERT_EQ(100, rd.baseInfo.chaId);
    ASSERT_EQ(200, rd.baseInfo.worldId);
    ASSERT_EQ("TestPirate", rd.baseInfo.name);
    ASSERT_EQ("Yarrr", rd.baseInfo.motto);
    ASSERT_EQ(42, rd.baseInfo.guildId);
    ASSERT_EQ("Pirates", rd.baseInfo.guildName);
    ASSERT_EQ(1000, rd.baseInfo.posX);
    ASSERT_EQ(2000, rd.baseInfo.posY);
    ASSERT_EQ(2, rd.baseInfo.side.sideId);
    ASSERT_FALSE(rd.baseInfo.look.isBoat);
    ASSERT_EQ(2001, rd.baseInfo.look.hairId);
    ASSERT_EQ(5001, rd.baseInfo.look.equips[0].id);
    ASSERT_EQ(10001, rd.baseInfo.look.equips[0].dbId);
    ASSERT_EQ(80, rd.baseInfo.look.equips[0].endure0);
    ASSERT_EQ(100, rd.baseInfo.look.equips[0].endure1);
    ASSERT_TRUE(rd.baseInfo.look.equips[0].hasExtra);
    ASSERT_EQ(12345, rd.baseInfo.look.equips[0].forgeParam);
    ASSERT_EQ(67890, rd.baseInfo.look.equips[0].instId);
    ASSERT_TRUE(rd.baseInfo.look.equips[0].hasInstAttr);
    ASSERT_EQ(1, rd.baseInfo.look.equips[0].instAttr[0][0]);
    ASSERT_EQ(100, rd.baseInfo.look.equips[0].instAttr[0][1]);
    ASSERT_EQ(2, rd.baseInfo.look.equips[0].instAttr[1][0]);
    ASSERT_EQ(200, rd.baseInfo.look.equips[0].instAttr[1][1]);
    ASSERT_EQ(0, rd.baseInfo.look.equips[1].id);
    ASSERT_EQ(3, rd.baseInfo.pkCtrl);
    ASSERT_EQ(8001, rd.baseInfo.appendLook[0].lookId);
    ASSERT_EQ(1, rd.baseInfo.appendLook[0].valid);
    ASSERT_EQ(0, rd.baseInfo.appendLook[1].lookId);

    // SkillBag
    ASSERT_EQ(100, rd.skillBag.defSkillId);
    ASSERT_EQ(2u, rd.skillBag.skills.size());
    ASSERT_EQ(1, rd.skillBag.skills[0].id);
    ASSERT_EQ(5, rd.skillBag.skills[0].level);
    ASSERT_EQ(1, rd.skillBag.skills[0].range[0]);
    ASSERT_EQ(100, rd.skillBag.skills[0].range[1]);
    ASSERT_EQ(200, rd.skillBag.skills[0].range[2]);
    ASSERT_EQ(300, rd.skillBag.skills[0].range[3]);
    ASSERT_EQ(0, rd.skillBag.skills[1].range[0]);

    // SkillState
    ASSERT_EQ(123456, rd.skillState.currentTime);
    ASSERT_EQ(2u, rd.skillState.states.size());
    ASSERT_EQ(10, rd.skillState.states[0].stateId);
    ASSERT_EQ(5000, rd.skillState.states[0].duration);

    // Attr
    ASSERT_EQ(3u, rd.attr.attrs.size());
    ASSERT_EQ(1, rd.attr.attrs[0].attrId);
    ASSERT_EQ(50, rd.attr.attrs[0].attrVal);

    // Kitbag
    ASSERT_EQ(SYN_KITBAG_INIT, rd.kitbag.synType);
    ASSERT_EQ(24, rd.kitbag.capacity);
    ASSERT_EQ(3u, rd.kitbag.items.size());
    ASSERT_EQ(0, rd.kitbag.items[0].gridId);
    ASSERT_EQ(1001, rd.kitbag.items[0].itemId);
    ASSERT_EQ(50001, rd.kitbag.items[0].dbId);
    ASSERT_FALSE(rd.kitbag.items[0].isBoat);
    ASSERT_EQ(111, rd.kitbag.items[0].instId);
    ASSERT_FALSE(rd.kitbag.items[0].hasInstAttr);
    ASSERT_EQ(5, rd.kitbag.items[1].gridId);
    ASSERT_EQ(9999, rd.kitbag.items[1].itemId);
    ASSERT_TRUE(rd.kitbag.items[1].isBoat);
    ASSERT_EQ(77777, rd.kitbag.items[1].boatWorldId);
    ASSERT_TRUE(rd.kitbag.items[1].hasInstAttr);
    ASSERT_EQ(10, rd.kitbag.items[1].instAttr[0][0]);
    ASSERT_EQ(20, rd.kitbag.items[1].instAttr[0][1]);
    ASSERT_EQ(10, rd.kitbag.items[2].gridId);
    ASSERT_EQ(0, rd.kitbag.items[2].itemId);

    // Shortcut
    ASSERT_EQ(1, rd.shortcut.entries[0].type);
    ASSERT_EQ(100, rd.shortcut.entries[0].gridId);
    ASSERT_EQ(2, rd.shortcut.entries[1].type);
    ASSERT_EQ(0, rd.shortcut.entries[2].type);

    ASSERT_EQ(0u, rd.boats.size());
    ASSERT_EQ(200, rd.ctrlChaId);
    // loginFlag, playerCount, playerAddr — GateServer-only, клиент не читает
}

TEST(CommandMessages, McEnterMapMessage_WithBoats_Roundtrip) {
    McEnterMapMessage original;
    original.errCode = 0;
    McEnterMapData d;
    d.autoLock = 0;
    d.kitbagLock = 0;
    d.enterType = 1;
    d.isNewCha = 0;
    d.mapName = "ocean1";
    d.canTeam = 0;
    d.imp = 0;

    // Минимальный BaseInfo для основного персонажа
    d.baseInfo.chaId = 1;
    d.baseInfo.worldId = 2;
    d.baseInfo.name = "Sailor";
    d.baseInfo.look.typeId = 1;
    d.baseInfo.look.isBoat = false;
    d.baseInfo.look.hairId = 2001;

    d.kitbag.synType = SYN_KITBAG_INIT;
    d.kitbag.capacity = 24;

    // Один корабль
    BoatData boat;
    boat.baseInfo.chaId = 1000;
    boat.baseInfo.worldId = 1001;
    boat.baseInfo.name = "GoodShip";
    boat.baseInfo.look.synType = 0;
    boat.baseInfo.look.typeId = 50;
    boat.baseInfo.look.isBoat = true;
    boat.baseInfo.look.boatParts = {1, 2, 3, 4, 5, 6, 7};
    boat.attr.synType = 0;
    boat.attr.attrs = {{1, 500}};
    boat.kitbag.synType = SYN_KITBAG_INIT;
    boat.kitbag.capacity = 10;
    boat.skillState.currentTime = 100000;
    d.boats.push_back(std::move(boat));

    d.ctrlChaId = 2;
    // loginFlag, playerCount, playerAddr — GateServer-only

    original.data = std::move(d);
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McEnterMapMessage restored;
    deserialize(r, restored);

    ASSERT_EQ(0, restored.errCode);
    ASSERT_TRUE(restored.data.has_value());
    ASSERT_EQ("ocean1", restored.data->mapName);
    ASSERT_EQ(1u, restored.data->boats.size());

    const auto& rb = restored.data->boats[0];
    ASSERT_EQ(1000, rb.baseInfo.chaId);
    ASSERT_EQ("GoodShip", rb.baseInfo.name);
    ASSERT_TRUE(rb.baseInfo.look.isBoat);
    ASSERT_EQ(50, rb.baseInfo.look.typeId);
    ASSERT_EQ(1, rb.baseInfo.look.boatParts.posId);
    ASSERT_EQ(7, rb.baseInfo.look.boatParts.equipment);
    ASSERT_EQ(1u, rb.attr.attrs.size());
    ASSERT_EQ(500, rb.attr.attrs[0].attrVal);
    ASSERT_EQ(100000, rb.skillState.currentTime);

    ASSERT_EQ(2, restored.data->ctrlChaId);
    // loginFlag, playerCount — GateServer-only
}

// Тест на основе реального пакета пользователя (item 9621 — НЕ корабль).
// Воспроизводит структуру: 7 скиллов, 74 атрибута, 4 предмета + пустые слоты,
// 36 шорткатов, 0 лодок. До фикса isBoat-флага вызывал ошибку десериализации
// на loginFlag (позиция 633), если клиент и сервер по-разному определяли isBoat.
TEST(CommandMessages, McEnterMapMessage_RealPacket_Roundtrip) {
    McEnterMapMessage original;
    original.errCode = 0;
    McEnterMapData d;
    d.autoLock = 0;
    d.kitbagLock = 0;
    d.enterType = 1;
    d.isNewCha = 0;
    d.mapName = "garner";
    d.canTeam = 1;
    d.imp = 0;

    // BaseInfo (из дампа: chaId=4, name=masha, gmLv=99)
    auto& b = d.baseInfo;
    b.chaId = 4;
    b.worldId = 1;
    b.commId = 1;
    b.commName = "masha";
    b.gmLv = 99;
    b.handle = 33566253;
    b.ctrlType = 1;
    b.name = "masha";
    b.motto = "";
    b.icon = 4;
    b.guildId = 0;
    b.guildName = "";
    b.guildMotto = "";
    b.guildPermission = 0;
    b.stallName = "";
    b.state = 1;
    b.posX = 209392;
    b.posY = 277335;
    b.radius = 40;
    b.angle = 191;
    b.teamLeaderId = 0;
    b.isPlayer = 1;
    b.side.sideId = 0;
    b.event = {1, 1, 0, ""};
    // Look — humanoid, typeId=4 (Lance)
    b.look.synType = 0;
    b.look.typeId = 4;
    b.look.isBoat = false;
    b.look.hairId = 2291;
    // Equips: slot 0..3 с предметами, остальные пустые
    b.look.equips[0].id = 2554;    // face
    b.look.equips[0].dbId = 0;
    b.look.equips[0].needLv = 0;
    // Все остальные слоты пустые (id=0)
    b.pkCtrl = 1;
    // appendLook — все пустые
    b.appendLook[0].lookId = 0;

    // SkillBag (7 скиллов из дампа)
    d.skillBag.defSkillId = 28;
    d.skillBag.synType = 0;
    d.skillBag.skills = {
        {28, 1, 1, 0, 0, 0, 0, {0, 0, 0, 0}},    // базовая атака
        {29, 0, 1, 0, 0, 0, 0, {0, 0, 0, 0}},
        {34, 0, 1, 0, 0, 0, 0, {0, 0, 0, 0}},
        {35, 0, 1, 0, 0, 0, 0, {0, 0, 0, 0}},
        {36, 0, 1, 0, 0, 0, 0, {0, 0, 0, 0}},
        {37, 0, 1, 0, 0, 0, 0, {0, 0, 0, 0}},
        {36660109, 0, 0, 0, 0, 0, 0, {0, 0, 0, 0}}  // системный скилл
    };

    // SkillState — пусто
    d.skillState.currentTime = 0;

    // Attr — 74 атрибута (из дампа)
    d.attr.synType = 0;
    d.attr.attrs.resize(74);
    // Ключевые значения из дампа
    int64_t attrVals[74] = {
        1, 45, 2, 23, 3, 0, 4, 0, 5, 0, 6, 4, 7, 0, 8, 0, 9, 0, 10, 1,
        11, 1, 12, 1, 13, 0, 14, 0, 15, 0, 16, 5, 17, 0, 18, 0, 19, 0, 20, 0,
        21, 625, 22, 0, 23, 0, 24, 1500, 25, 5, 26, 5, 27, 5, 28, 5, 29, 5, 30, 5,
        31, 100, 32, 23, 33, 21, 34, 25, 35, 6, 36, 10, 37, 11
    };
    // Заполняем как пары attrId/attrVal
    for (int i = 0; i < 74; i += 2) {
        d.attr.attrs[i / 2].attrId = attrVals[i];
        d.attr.attrs[i / 2].attrVal = attrVals[i + 1];
    }
    d.attr.attrs.resize(37); // 37 пар

    // Kitbag — 4 предмета + пустые слоты (item 9621 — НЕ корабль!)
    d.kitbag.synType = SYN_KITBAG_INIT;
    d.kitbag.capacity = 24;

    KitbagItem item0;
    item0.gridId = 0; item0.itemId = 1777;
    item0.dbId = 0; item0.needLv = 0; item0.num = 1;
    item0.endure0 = 0; item0.endure1 = 0; item0.energy0 = 0; item0.energy1 = 0;
    item0.forgeLv = 0; item0.valid = 1; item0.tradable = 1; item0.expiration = 0;
    item0.isBoat = false; item0.forgeParam = 0; item0.instId = 0; item0.hasInstAttr = false;

    KitbagItem item1;
    item1.gridId = 1; item1.itemId = 3116;
    item1.dbId = 0; item1.needLv = 0; item1.num = 1;
    item1.endure0 = 0; item1.endure1 = 0; item1.energy0 = 0; item1.energy1 = 0;
    item1.forgeLv = 0; item1.valid = 1; item1.tradable = 1; item1.expiration = 0;
    item1.isBoat = false; item1.forgeParam = 0; item1.instId = 0; item1.hasInstAttr = false;

    KitbagItem item3;
    item3.gridId = 3; item3.itemId = 436;
    item3.dbId = 0; item3.needLv = 0; item3.num = 1;
    item3.endure0 = 0; item3.endure1 = 0; item3.energy0 = 0; item3.energy1 = 0;
    item3.forgeLv = 0; item3.valid = 1; item3.tradable = 1; item3.expiration = 0;
    item3.isBoat = false; item3.forgeParam = 0; item3.instId = 0; item3.hasInstAttr = false;

    // Item 9621 — ключевой: это НЕ корабль, но до фикса клиент мог считать его кораблём
    KitbagItem item4;
    item4.gridId = 4; item4.itemId = 9621;
    item4.dbId = 0; item4.needLv = 0; item4.num = 1;
    item4.endure0 = 0; item4.endure1 = 0; item4.energy0 = 0; item4.energy1 = 0;
    item4.forgeLv = 1; item4.valid = 1; item4.tradable = 1; item4.expiration = 0;
    item4.isBoat = false; item4.forgeParam = 0; item4.instId = 0; item4.hasInstAttr = false;

    // Пустые слоты 2, 5..23
    KitbagItem emptyGrid2; emptyGrid2.gridId = 2; emptyGrid2.itemId = 0;

    d.kitbag.items = {item0, item1, emptyGrid2, item3, item4};
    for (int i = 5; i < 24; i++) {
        KitbagItem empty; empty.gridId = i; empty.itemId = 0;
        d.kitbag.items.push_back(empty);
    }

    // Shortcut — все пустые
    for (int i = 0; i < SHORT_CUT_NUM; i++) {
        d.shortcut.entries[i] = {0, 0};
    }

    // Без лодок
    d.ctrlChaId = 4;
    // loginFlag, playerCount, playerAddr — GateServer-only

    original.data = std::move(d);
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McEnterMapMessage restored;
    deserialize(r, restored);

    ASSERT_EQ(0, restored.errCode);
    ASSERT_TRUE(restored.data.has_value());
    const auto& rd = restored.data.value();

    ASSERT_EQ("garner", rd.mapName);
    ASSERT_EQ(4, rd.baseInfo.chaId);
    ASSERT_EQ("masha", rd.baseInfo.name);
    ASSERT_EQ(99, rd.baseInfo.gmLv);
    ASSERT_EQ(209392, rd.baseInfo.posX);
    ASSERT_EQ(277335, rd.baseInfo.posY);

    // Проверка скиллов
    ASSERT_EQ(28, rd.skillBag.defSkillId);
    ASSERT_EQ(7u, rd.skillBag.skills.size());
    ASSERT_EQ(28, rd.skillBag.skills[0].id);
    ASSERT_EQ(36660109, rd.skillBag.skills[6].id);

    // Проверка атрибутов
    ASSERT_EQ(37u, rd.attr.attrs.size());
    ASSERT_EQ(1, rd.attr.attrs[0].attrId);
    ASSERT_EQ(45, rd.attr.attrs[0].attrVal);

    // Проверка kitbag — item 9621 НЕ корабль
    ASSERT_EQ(24, rd.kitbag.capacity);
    // item 9621 (grid 4) — isBoat=false
    bool found9621 = false;
    for (const auto& item : rd.kitbag.items) {
        if (item.itemId == 9621) {
            found9621 = true;
            ASSERT_FALSE(item.isBoat);
            ASSERT_EQ(4, item.gridId);
            ASSERT_EQ(1, item.forgeLv);
        }
    }
    ASSERT_TRUE(found9621);

    // loginFlag — GateServer-only, клиент не читает
    ASSERT_EQ(4, rd.ctrlChaId);
    ASSERT_EQ(0u, rd.boats.size());
}

// =================================================================
//  Группа H: TM/MT — GateServer <-> GameServer
// =================================================================

TEST(CommandMessages, MtLoginMessage_Roundtrip) {
    MtLoginMessage original;
    original.serverName = "GameSvr1";
    original.mapNameList = "garner,ocean1,ocean2";
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    MtLoginMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.serverName, restored.serverName);
    ASSERT_EQ(original.mapNameList, restored.mapNameList);
}

TEST(CommandMessages, TmLoginAckMessage_Success_Roundtrip) {
    TmLoginAckMessage original;
    original.errCode = 0;
    original.gateName = "GateMain";
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    TmLoginAckMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(0, restored.errCode);
    ASSERT_EQ("GateMain", restored.gateName);
}

TEST(CommandMessages, TmLoginAckMessage_Error_Roundtrip) {
    TmLoginAckMessage original;
    original.errCode = -1;
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    TmLoginAckMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(-1, restored.errCode);
}

TEST(CommandMessages, TmEnterMapMessage_Roundtrip) {
    TmEnterMapMessage original;
    original.actId = 12345;
    original.password = "secret";
    original.dbCharId = 100;
    original.worldId = 200;
    original.mapName = "garner";
    original.mapCopyNo = 1;
    original.posX = 500;
    original.posY = 600;
    original.loginFlag = 1;
    original.winer = 42;
    original.gateAddr = 0xABCD;
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    TmEnterMapMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.actId, restored.actId);
    ASSERT_EQ(original.password, restored.password);
    ASSERT_EQ(original.dbCharId, restored.dbCharId);
    ASSERT_EQ(original.worldId, restored.worldId);
    ASSERT_EQ(original.mapName, restored.mapName);
    ASSERT_EQ(original.mapCopyNo, restored.mapCopyNo);
    ASSERT_EQ(original.posX, restored.posX);
    ASSERT_EQ(original.posY, restored.posY);
    ASSERT_EQ(original.loginFlag, restored.loginFlag);
    ASSERT_EQ(original.winer, restored.winer);
    ASSERT_EQ(original.gateAddr, restored.gateAddr);
}

TEST(CommandMessages, TmGoOutMapMessage_Roundtrip) {
    TmGoOutMapMessage original;
    original.playerPtr = 0x12345678;
    original.gateAddr = 0xDEADBEEF;
    original.offlineFlag = 1;
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    TmGoOutMapMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.playerPtr, restored.playerPtr);
    ASSERT_EQ(original.gateAddr, restored.gateAddr);
    ASSERT_EQ(original.offlineFlag, restored.offlineFlag);
}

TEST(CommandMessages, TmChangePersonInfoMessage_Roundtrip) {
    TmChangePersonInfoMessage original;
    original.motto = "Hello World!";
    original.icon = 5;
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    TmChangePersonInfoMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.motto, restored.motto);
    ASSERT_EQ(original.icon, restored.icon);
}

TEST(CommandMessages, TmKickChaMessage_Roundtrip) {
    TmKickChaMessage original;
    original.charDbId = 999;
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    TmKickChaMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.charDbId, restored.charDbId);
}

TEST(CommandMessages, TmOfflineModeMessage_Roundtrip) {
    TmOfflineModeMessage original;
    original.playerPtr = 0x11223344;
    original.gateAddr = 0x55667788;
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    TmOfflineModeMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.playerPtr, restored.playerPtr);
    ASSERT_EQ(original.gateAddr, restored.gateAddr);
}

TEST(CommandMessages, MtSwitchMapMessage_Roundtrip) {
    MtSwitchMapMessage original;
    original.currentMapName = "garner";
    original.currentCopyNo = 0;
    original.posX = 100;
    original.posY = 200;
    original.targetMapName = "ocean1";
    original.targetCopyNo = 1;
    original.targetX = 300;
    original.targetY = 400;
    original.switchType = 0;
    original.playerDBID = 5555;
    original.gateAddr = 0xABCD;
    original.aimNum = 1;
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    MtSwitchMapMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.currentMapName, restored.currentMapName);
    ASSERT_EQ(original.currentCopyNo, restored.currentCopyNo);
    ASSERT_EQ(original.posX, restored.posX);
    ASSERT_EQ(original.posY, restored.posY);
    ASSERT_EQ(original.targetMapName, restored.targetMapName);
    ASSERT_EQ(original.targetCopyNo, restored.targetCopyNo);
    ASSERT_EQ(original.targetX, restored.targetX);
    ASSERT_EQ(original.targetY, restored.targetY);
    ASSERT_EQ(original.switchType, restored.switchType);
    ASSERT_EQ(original.playerDBID, restored.playerDBID);
    ASSERT_EQ(original.gateAddr, restored.gateAddr);
    ASSERT_EQ(original.aimNum, restored.aimNum);
}

TEST(CommandMessages, MtKickUserMessage_Roundtrip) {
    MtKickUserMessage original;
    original.playerDBID = 42;
    original.gateAddr = 0x1234;
    original.kickFlag = 1;
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    MtKickUserMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.playerDBID, restored.playerDBID);
    ASSERT_EQ(original.gateAddr, restored.gateAddr);
    ASSERT_EQ(original.kickFlag, restored.kickFlag);
}

TEST(CommandMessages, MtPlayerExitMessage_Roundtrip) {
    MtPlayerExitMessage original;
    original.playerDBID = 777;
    original.gateAddr = 0xF00D;
    original.aimNum = 1;
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    MtPlayerExitMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.playerDBID, restored.playerDBID);
    ASSERT_EQ(original.gateAddr, restored.gateAddr);
    ASSERT_EQ(original.aimNum, restored.aimNum);
}

TEST(CommandMessages, MapEntryMessage_Create_Roundtrip) {
    MapEntryMessage original;
    original.srcMapName = "garner";
    original.targetMapName = "ocean1";
    original.subType = MAPENTRY_CREATE;
    original.posX = 100;
    original.posY = 200;
    original.mapCopyNum = 3;
    original.copyPlayerNum = 10;
    original.luaScriptLines = {"print('hello')", "print('world')"};
    auto w = serialize(original, CMD_MT_MAPENTRY);
    RPacket r(w.Data(), w.GetPacketSize());
    MapEntryMessage restored;
    deserializeMapEntry(r, restored);
    ASSERT_EQ(original.srcMapName, restored.srcMapName);
    ASSERT_EQ(original.targetMapName, restored.targetMapName);
    ASSERT_EQ(MAPENTRY_CREATE, restored.subType);
    ASSERT_EQ(original.posX, restored.posX);
    ASSERT_EQ(original.posY, restored.posY);
}

TEST(CommandMessages, MapEntryMessage_Destroy_Roundtrip) {
    MapEntryMessage original;
    original.srcMapName = "garner";
    original.targetMapName = "ocean1";
    original.subType = MAPENTRY_DESTROY;
    auto w = serialize(original, CMD_TM_MAPENTRY);
    RPacket r(w.Data(), w.GetPacketSize());
    MapEntryMessage restored;
    deserializeMapEntry(r, restored);
    ASSERT_EQ(original.srcMapName, restored.srcMapName);
    ASSERT_EQ(MAPENTRY_DESTROY, restored.subType);
}

TEST(CommandMessages, MapEntryMessage_CopyParam_Roundtrip) {
    MapEntryMessage original;
    original.srcMapName = "garner";
    original.targetMapName = "ocean1";
    original.subType = MAPENTRY_COPYPARAM;
    original.copyId = 2;
    for (int i = 0; i < MAPCOPY_INFO_PARAM_NUM; ++i)
        original.params[i] = (i + 1) * 10;
    auto w = serialize(original, CMD_MT_MAPENTRY);
    RPacket r(w.Data(), w.GetPacketSize());
    MapEntryMessage restored;
    deserializeMapEntry(r, restored);
    ASSERT_EQ(MAPENTRY_COPYPARAM, restored.subType);
    ASSERT_EQ(2, restored.copyId);
    for (int i = 0; i < MAPCOPY_INFO_PARAM_NUM; ++i)
        ASSERT_EQ((i + 1) * 10, restored.params[i]);
}

// =================================================================
//  Группа I: MM — GameServer <-> GameServer
// =================================================================

TEST(CommandMessages, MmQueryChaMessage_Roundtrip) {
    MmQueryChaMessage original;
    original.srcId = 42;
    original.chaName = "TestPlayer";
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    MmQueryChaMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.srcId, restored.srcId);
    ASSERT_EQ(original.chaName, restored.chaName);
}

TEST(CommandMessages, MmCallChaMessage_Roundtrip) {
    MmCallChaMessage original;
    original.srcId = 1;
    original.targetName = "Pirate";
    original.isBoat = 0;
    original.mapName = "ocean1";
    original.posX = 500;
    original.posY = 600;
    original.copyNo = 0;
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    MmCallChaMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.srcId, restored.srcId);
    ASSERT_EQ(original.targetName, restored.targetName);
    ASSERT_EQ(original.isBoat, restored.isBoat);
    ASSERT_EQ(original.mapName, restored.mapName);
    ASSERT_EQ(original.posX, restored.posX);
    ASSERT_EQ(original.posY, restored.posY);
    ASSERT_EQ(original.copyNo, restored.copyNo);
}

TEST(CommandMessages, MmGotoChaMessage_Query_Roundtrip) {
    MmGotoChaMessage original;
    original.srcId = 10;
    original.targetName = "Target";
    original.mode = 1;
    original.srcName = "Source";
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    MmGotoChaMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.srcId, restored.srcId);
    ASSERT_EQ(original.targetName, restored.targetName);
    ASSERT_EQ(1, restored.mode);
    ASSERT_EQ(original.srcName, restored.srcName);
}

TEST(CommandMessages, MmGotoChaMessage_Execute_Roundtrip) {
    MmGotoChaMessage original;
    original.srcId = 10;
    original.targetName = "Target";
    original.mode = 2;
    original.isBoat = 1;
    original.mapName = "ocean1";
    original.posX = 100;
    original.posY = 200;
    original.copyNo = 3;
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    MmGotoChaMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(2, restored.mode);
    ASSERT_EQ(1, restored.isBoat);
    ASSERT_EQ(original.mapName, restored.mapName);
    ASSERT_EQ(original.posX, restored.posX);
    ASSERT_EQ(original.posY, restored.posY);
    ASSERT_EQ(original.copyNo, restored.copyNo);
}

TEST(CommandMessages, MmGuildApproveMessage_Roundtrip) {
    MmGuildApproveMessage original;
    original.srcId = 55;
    original.guildId = 100;
    original.guildName = "Pirates";
    original.guildMotto = "Arrr!";
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    MmGuildApproveMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.srcId, restored.srcId);
    ASSERT_EQ(original.guildId, restored.guildId);
    ASSERT_EQ(original.guildName, restored.guildName);
    ASSERT_EQ(original.guildMotto, restored.guildMotto);
}

TEST(CommandMessages, MmKickChaMessage_Roundtrip) {
    MmKickChaMessage original;
    original.srcId = 1;
    original.targetName = "BadPlayer";
    original.kickDuration = 60000;
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    MmKickChaMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.srcId, restored.srcId);
    ASSERT_EQ(original.targetName, restored.targetName);
    ASSERT_EQ(original.kickDuration, restored.kickDuration);
}

TEST(CommandMessages, MmNoticeMessage_Roundtrip) {
    MmNoticeMessage original;
    original.content = "Server maintenance in 10 minutes!";
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    // Десериализация пропускает srcId=0
    r.ReadInt64(); // skip srcId
    MmNoticeMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.content, restored.content);
}

TEST(CommandMessages, MmDoStringMessage_Roundtrip) {
    MmDoStringMessage original;
    original.srcId = 42;
    original.luaCode = "print('test')";
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    MmDoStringMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.srcId, restored.srcId);
    ASSERT_EQ(original.luaCode, restored.luaCode);
}

TEST(CommandMessages, MmStoreBuyMessage_Roundtrip) {
    MmStoreBuyMessage original;
    original.charDbId = 123;
    original.commodityId = 456;
    original.money = 1000;
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    r.ReadInt64(); // skip srcId=0
    MmStoreBuyMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.charDbId, restored.charDbId);
    ASSERT_EQ(original.commodityId, restored.commodityId);
    ASSERT_EQ(original.money, restored.money);
}
