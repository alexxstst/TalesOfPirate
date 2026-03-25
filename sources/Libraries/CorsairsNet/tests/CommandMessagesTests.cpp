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

// =================================================================
//  Геймплейные команды: Фаза 1 — простые
// =================================================================

// -----------------------------------------------------------------
//  Cmd-only команды (только код команды, без полей)
// -----------------------------------------------------------------

TEST(GameplayPhase1, CmOfflineMode_Cmd) {
    // Проверяем, что пакет создаётся и имеет ненулевой размер
    auto w = serializeCmOfflineModeCmd();
    ASSERT_TRUE(w.GetPacketSize() > 0);
}

TEST(GameplayPhase1, CmCancelExit_Cmd) {
    // Отмена выхода из игры — только код команды
    auto w = serializeCmCancelExitCmd();
    ASSERT_TRUE(w.GetPacketSize() > 0);
}

TEST(GameplayPhase1, CmKitbagLock_Cmd) {
    // Запрос блокировки инвентаря
    auto w = serializeCmKitbagLockCmd();
    ASSERT_TRUE(w.GetPacketSize() > 0);
}

TEST(GameplayPhase1, CmKitbagCheck_Cmd) {
    // Запрос проверки состояния инвентаря
    auto w = serializeCmKitbagCheckCmd();
    ASSERT_TRUE(w.GetPacketSize() > 0);
}

TEST(GameplayPhase1, CmKitbagTempSync_Cmd) {
    // Синхронизация временного инвентаря
    auto w = serializeCmKitbagTempSyncCmd();
    ASSERT_TRUE(w.GetPacketSize() > 0);
}

TEST(GameplayPhase1, CmReadBookStart_Cmd) {
    // Начало чтения книги
    auto w = serializeCmReadBookStartCmd();
    ASSERT_TRUE(w.GetPacketSize() > 0);
}

TEST(GameplayPhase1, CmReadBookClose_Cmd) {
    // Закрытие книги
    auto w = serializeCmReadBookCloseCmd();
    ASSERT_TRUE(w.GetPacketSize() > 0);
}

TEST(GameplayPhase1, CmBoatCancel_Cmd) {
    // Отмена управления лодкой
    auto w = serializeCmBoatCancelCmd();
    ASSERT_TRUE(w.GetPacketSize() > 0);
}

TEST(GameplayPhase1, CmBoatGetInfo_Cmd) {
    // Запрос информации о лодке
    auto w = serializeCmBoatGetInfoCmd();
    ASSERT_TRUE(w.GetPacketSize() > 0);
}

TEST(GameplayPhase1, CmVolunteerAdd_Cmd) {
    // Добавление в список добровольцев
    auto w = serializeCmVolunteerAddCmd();
    ASSERT_TRUE(w.GetPacketSize() > 0);
}

TEST(GameplayPhase1, CmVolunteerDel_Cmd) {
    // Удаление из списка добровольцев
    auto w = serializeCmVolunteerDelCmd();
    ASSERT_TRUE(w.GetPacketSize() > 0);
}

TEST(GameplayPhase1, CmStoreClose_Cmd) {
    // Закрытие магазина
    auto w = serializeCmStoreCloseCmd();
    ASSERT_TRUE(w.GetPacketSize() > 0);
}

TEST(GameplayPhase1, CmStallClose_Cmd) {
    // Закрытие прилавка
    auto w = serializeCmStallCloseCmd();
    ASSERT_TRUE(w.GetPacketSize() > 0);
}

TEST(GameplayPhase1, CmMisLog_Cmd) {
    // Запрос лога миссий
    auto w = serializeCmMisLogCmd();
    ASSERT_TRUE(w.GetPacketSize() > 0);
}

TEST(GameplayPhase1, CmDailyBuffRequest_Cmd) {
    // Запрос ежедневного баффа
    auto w = serializeCmDailyBuffRequestCmd();
    ASSERT_TRUE(w.GetPacketSize() > 0);
}

TEST(GameplayPhase1, CmRequestDropRate_Cmd) {
    // Запрос текущего шанса дропа
    auto w = serializeCmRequestDropRateCmd();
    ASSERT_TRUE(w.GetPacketSize() > 0);
}

TEST(GameplayPhase1, CmRequestExpRate_Cmd) {
    // Запрос текущего множителя опыта
    auto w = serializeCmRequestExpRateCmd();
    ASSERT_TRUE(w.GetPacketSize() > 0);
}

// -----------------------------------------------------------------
//  CM — клиентские команды с полями (Client -> Server)
// -----------------------------------------------------------------

TEST(GameplayPhase1, CmDieReturnMessage_Roundtrip) {
    // Выбор типа воскрешения (2 — воскрешение в городе)
    CmDieReturnMessage original{2};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    CmDieReturnMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.reliveType, restored.reliveType);
}

TEST(GameplayPhase1, CmAutoKitbagLockMessage_Roundtrip) {
    // Автоматическая блокировка инвентаря (1 = включено)
    CmAutoKitbagLockMessage original{1};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    CmAutoKitbagLockMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.autoLock, restored.autoLock);
}

TEST(GameplayPhase1, CmStallSearchMessage_Roundtrip) {
    // Поиск предмета на прилавках по itemId
    CmStallSearchMessage original{55001};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    CmStallSearchMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.itemId, restored.itemId);
}

TEST(GameplayPhase1, CmForgeItemMessage_Roundtrip) {
    // Ковка предмета по индексу слота
    CmForgeItemMessage original{7};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    CmForgeItemMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.index, restored.index);
}

TEST(GameplayPhase1, CmEntityEventMessage_Roundtrip) {
    // Взаимодействие с сущностью по её entityId
    CmEntityEventMessage original{0xFFFF00};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    CmEntityEventMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.entityId, restored.entityId);
}

TEST(GameplayPhase1, CmStallOpenMessage_Roundtrip) {
    // Открытие прилавка персонажа
    CmStallOpenMessage original{12345};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    CmStallOpenMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.charId, restored.charId);
}

TEST(GameplayPhase1, CmMisLogInfoMessage_Roundtrip) {
    // Запрос информации о записи в логе миссий
    CmMisLogInfoMessage original{42};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    CmMisLogInfoMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.id, restored.id);
}

TEST(GameplayPhase1, CmMisLogClearMessage_Roundtrip) {
    // Очистка записи из лога миссий
    CmMisLogClearMessage original{99};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    CmMisLogClearMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.id, restored.id);
}

TEST(GameplayPhase1, CmStoreBuyAskMessage_Roundtrip) {
    // Запрос на покупку товара (commodityId)
    CmStoreBuyAskMessage original{777};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    CmStoreBuyAskMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.comId, restored.comId);
}

TEST(GameplayPhase1, CmStoreChangeAskMessage_Roundtrip) {
    // Запрос на обмен в магазине (количество)
    CmStoreChangeAskMessage original{5};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    CmStoreChangeAskMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.num, restored.num);
}

TEST(GameplayPhase1, CmStoreQueryMessage_Roundtrip) {
    // Запрос страницы магазина
    CmStoreQueryMessage original{10};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    CmStoreQueryMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.num, restored.num);
}

TEST(GameplayPhase1, CmTeamFightAnswerMessage_Roundtrip) {
    // Ответ на приглашение в командный бой (1 = принять)
    CmTeamFightAnswerMessage original{1};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    CmTeamFightAnswerMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.accept, restored.accept);
}

TEST(GameplayPhase1, CmItemRepairAnswerMessage_Roundtrip) {
    // Подтверждение ремонта предмета (1 = да)
    CmItemRepairAnswerMessage original{1};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    CmItemRepairAnswerMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.accept, restored.accept);
}

TEST(GameplayPhase1, CmItemForgeAnswerMessage_Roundtrip) {
    // Подтверждение ковки предмета (0 = отказ)
    CmItemForgeAnswerMessage original{0};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    CmItemForgeAnswerMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.accept, restored.accept);
}

TEST(GameplayPhase1, CmItemLotteryAnswerMessage_Roundtrip) {
    // Подтверждение лотереи предметов (1 = участвовать)
    CmItemLotteryAnswerMessage original{1};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    CmItemLotteryAnswerMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.accept, restored.accept);
}

TEST(GameplayPhase1, CmVolunteerOpenMessage_Roundtrip) {
    // Открытие списка добровольцев с указанным количеством
    CmVolunteerOpenMessage original{25};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    CmVolunteerOpenMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.num, restored.num);
}

TEST(GameplayPhase1, CmVolunteerListMessage_Roundtrip) {
    // Запрос списка добровольцев: страница 3, по 20 записей
    CmVolunteerListMessage original{3, 20};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    CmVolunteerListMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.page, restored.page);
    ASSERT_EQ(original.num, restored.num);
}

TEST(GameplayPhase1, CmStoreListAskMessage_Roundtrip) {
    // Запрос списка товаров: класс 5, страница 2, по 15 записей
    CmStoreListAskMessage original{5, 2, 15};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    CmStoreListAskMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.clsId, restored.clsId);
    ASSERT_EQ(original.page, restored.page);
    ASSERT_EQ(original.num, restored.num);
}

TEST(GameplayPhase1, CmCaptainConfirmAsrMessage_Roundtrip) {
    // Подтверждение назначения капитана: ret=1, teamId=4096
    CmCaptainConfirmAsrMessage original{1, 4096};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    CmCaptainConfirmAsrMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.ret, restored.ret);
    ASSERT_EQ(original.teamId, restored.teamId);
}

TEST(GameplayPhase1, CmMapMaskMessage_Roundtrip) {
    // Запрос маски карты по имени
    CmMapMaskMessage original{"MapForest01"};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    CmMapMaskMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.mapName, restored.mapName);
}

TEST(GameplayPhase1, CmStoreOpenAskMessage_Roundtrip) {
    // Запрос открытия магазина с паролем
    CmStoreOpenAskMessage original{"test_password_123"};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    CmStoreOpenAskMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.password, restored.password);
}

TEST(GameplayPhase1, CmVolunteerSelMessage_Roundtrip) {
    // Выбор добровольца по имени
    CmVolunteerSelMessage original{"Pirate"};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    CmVolunteerSelMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.name, restored.name);
}

TEST(GameplayPhase1, CmKitbagUnlockMessage_Roundtrip) {
    // Разблокировка инвентаря паролем
    CmKitbagUnlockMessage original{"s3cret_key!"};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    CmKitbagUnlockMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.password, restored.password);
}

// -----------------------------------------------------------------
//  MC — серверные команды с полями (Server -> Client)
// -----------------------------------------------------------------

TEST(GameplayPhase1, McFailedActionMessage_Roundtrip) {
    // Уведомление о неудачном действии: worldId + тип действия + причина
    McFailedActionMessage original{8001, 2, 7};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McFailedActionMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.worldId, restored.worldId);
    ASSERT_EQ(original.actionType, restored.actionType);
    ASSERT_EQ(original.reason, restored.reason);
}

TEST(GameplayPhase1, McChaEndSeeMessage_Roundtrip) {
    // Персонаж вышел из зоны видимости: тип 1, worldId 5050
    McChaEndSeeMessage original{1, 5050};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McChaEndSeeMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.seeType, restored.seeType);
    ASSERT_EQ(original.worldId, restored.worldId);
}

TEST(GameplayPhase1, McItemDestroyMessage_Roundtrip) {
    // Предмет уничтожен (worldId предмета)
    McItemDestroyMessage original{3003};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McItemDestroyMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.worldId, restored.worldId);
}

TEST(GameplayPhase1, McForgeResultMessage_Roundtrip) {
    // Результат ковки: 1 = успех
    McForgeResultMessage original{1};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McForgeResultMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.result, restored.result);
}

TEST(GameplayPhase1, McUniteResultMessage_Roundtrip) {
    // Результат объединения: 0 = неудача
    McUniteResultMessage original{0};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McUniteResultMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.result, restored.result);
}

TEST(GameplayPhase1, McMillingResultMessage_Roundtrip) {
    // Результат переработки: 2 = критический успех
    McMillingResultMessage original{2};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McMillingResultMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.result, restored.result);
}

TEST(GameplayPhase1, McFusionResultMessage_Roundtrip) {
    // Результат синтеза: 1 = успех
    McFusionResultMessage original{1};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McFusionResultMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.result, restored.result);
}

TEST(GameplayPhase1, McUpgradeResultMessage_Roundtrip) {
    // Результат улучшения: 3 = выдающийся успех
    McUpgradeResultMessage original{3};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McUpgradeResultMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.result, restored.result);
}

TEST(GameplayPhase1, McPurifyResultMessage_Roundtrip) {
    // Результат очистки: -1 = провал
    McPurifyResultMessage original{-1};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McPurifyResultMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.result, restored.result);
}

TEST(GameplayPhase1, McFixResultMessage_Roundtrip) {
    // Результат починки: 1 = успешно
    McFixResultMessage original{1};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McFixResultMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.result, restored.result);
}

TEST(GameplayPhase1, McEidolonMetempsychosisMessage_Roundtrip) {
    // Результат перерождения эйдолона: 100 = особый результат
    McEidolonMetempsychosisMessage original{100};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McEidolonMetempsychosisMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.result, restored.result);
}

TEST(GameplayPhase1, McEidolonFusionMessage_Roundtrip) {
    // Результат слияния эйдолонов: 42
    McEidolonFusionMessage original{42};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McEidolonFusionMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.result, restored.result);
}

TEST(GameplayPhase1, McMessageMessage_Roundtrip) {
    // Текстовое сообщение от сервера
    McMessageMessage original{"Hello World!"};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McMessageMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.text, restored.text);
}

TEST(GameplayPhase1, McBickerNoticeMessage_Roundtrip) {
    // Уведомление о перебранке в чате
    McBickerNoticeMessage original{"Pirate was silenced for spam"};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McBickerNoticeMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.text, restored.text);
}

TEST(GameplayPhase1, McColourNoticeMessage_Roundtrip) {
    // Цветное уведомление: красный цвет (0xFF0000) + текст
    McColourNoticeMessage original{0xFF0000, "Server restarting soon!"};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McColourNoticeMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.color, restored.color);
    ASSERT_EQ(original.text, restored.text);
}

TEST(GameplayPhase1, McTriggerActionMessage_Roundtrip) {
    // Триггер серверного действия: тип + ID + количество + счётчик
    McTriggerActionMessage original{3, 256, 10, 5};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McTriggerActionMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.type, restored.type);
    ASSERT_EQ(original.id, restored.id);
    ASSERT_EQ(original.num, restored.num);
    ASSERT_EQ(original.count, restored.count);
}

TEST(GameplayPhase1, McNpcStateChangeMessage_Roundtrip) {
    // Изменение состояния NPC: npcId 1001, новое состояние 3
    McNpcStateChangeMessage original{1001, 3};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McNpcStateChangeMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.npcId, restored.npcId);
    ASSERT_EQ(original.state, restored.state);
}

TEST(GameplayPhase1, McEntityStateChangeMessage_Roundtrip) {
    // Изменение состояния сущности: entityId 7070, state 2
    McEntityStateChangeMessage original{7070, 2};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McEntityStateChangeMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.entityId, restored.entityId);
    ASSERT_EQ(original.state, restored.state);
}

TEST(GameplayPhase1, McCloseTalkMessage_Roundtrip) {
    // Закрытие диалога с NPC
    McCloseTalkMessage original{555};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McCloseTalkMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.npcId, restored.npcId);
}

TEST(GameplayPhase1, McKitbagCheckAnswerMessage_Roundtrip) {
    // Ответ на проверку инвентаря: 1 = заблокирован
    McKitbagCheckAnswerMessage original{1};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McKitbagCheckAnswerMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.locked, restored.locked);
}

TEST(GameplayPhase1, McPreMoveTimeMessage_Roundtrip) {
    // Время до начала перемещения (мс)
    McPreMoveTimeMessage original{1500};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McPreMoveTimeMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.time, restored.time);
}

TEST(GameplayPhase1, McItemUseSuccMessage_Roundtrip) {
    // Предмет успешно использован: worldId 9001, itemId 300
    McItemUseSuccMessage original{9001, 300};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McItemUseSuccMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.worldId, restored.worldId);
    ASSERT_EQ(original.itemId, restored.itemId);
}

TEST(GameplayPhase1, McChaPlayEffectMessage_Roundtrip) {
    // Воспроизведение эффекта на персонаже: worldId 4040, effectId 15
    McChaPlayEffectMessage original{4040, 15};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McChaPlayEffectMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.worldId, restored.worldId);
    ASSERT_EQ(original.effectId, restored.effectId);
}

TEST(GameplayPhase1, McSynDefaultSkillMessage_Roundtrip) {
    // Синхронизация умения по умолчанию: worldId 6060, skillId 88
    McSynDefaultSkillMessage original{6060, 88};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McSynDefaultSkillMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.worldId, restored.worldId);
    ASSERT_EQ(original.skillId, restored.skillId);
}

TEST(GameplayPhase1, McSayMessage_Roundtrip) {
    // Сообщение в чате: sourceId 777, зелёный цвет
    McSayMessage original{777, "Ahoy, matey!", 0x00FF00};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McSayMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.sourceId, restored.sourceId);
    ASSERT_EQ(original.content, restored.content);
    ASSERT_EQ(original.color, restored.color);
}

TEST(GameplayPhase1, McSysInfoMessage_Roundtrip) {
    // Системное информационное сообщение
    McSysInfoMessage original{"Server maintenance in 30 minutes"};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McSysInfoMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.info, restored.info);
}

TEST(GameplayPhase1, McPopupNoticeMessage_Roundtrip) {
    // Всплывающее уведомление
    McPopupNoticeMessage original{"You have been rewarded 100 gold!"};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McPopupNoticeMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.notice, restored.notice);
}

TEST(GameplayPhase1, McPingMessage_Roundtrip) {
    // Пинг с пятью значениями для проверки задержки
    McPingMessage original{100, 200, 300, 400, 500};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McPingMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.v1, restored.v1);
    ASSERT_EQ(original.v2, restored.v2);
    ASSERT_EQ(original.v3, restored.v3);
    ASSERT_EQ(original.v4, restored.v4);
    ASSERT_EQ(original.v5, restored.v5);
}

// =================================================================
//  Геймплейные команды: Фаза 2 — средние
// =================================================================

// -----------------------------------------------------------------
//  CM — Client → GameServer
// -----------------------------------------------------------------

TEST(GameplayPhase2, CmChangePassMessage_Roundtrip) {
    // Смена пароля аккаунта с PIN-кодом
    CmChangePassMessage original{"secret123", "pin456"};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    CmChangePassMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.pass, restored.pass);
    ASSERT_EQ(original.pin, restored.pin);
}

TEST(GameplayPhase2, CmGuildBankOperMessage_Roundtrip) {
    // Операция с гильд-банком: перемещение предметов между слотами
    CmGuildBankOperMessage original{1, 2, 1001, 50, 3, 2002};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    CmGuildBankOperMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.op, restored.op);
    ASSERT_EQ(original.srcType, restored.srcType);
    ASSERT_EQ(original.srcId, restored.srcId);
    ASSERT_EQ(original.srcNum, restored.srcNum);
    ASSERT_EQ(original.tarType, restored.tarType);
    ASSERT_EQ(original.tarId, restored.tarId);
}

TEST(GameplayPhase2, CmGuildBankGoldMessage_Roundtrip) {
    // Вложение золота в гильд-банк: направление 1 = вклад, 50000 золота
    CmGuildBankGoldMessage original{2, 1, 50000};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    CmGuildBankGoldMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.op, restored.op);
    ASSERT_EQ(original.direction, restored.direction);
    ASSERT_EQ(original.gold, restored.gold);
}

TEST(GameplayPhase2, CmUpdateHairMessage_Roundtrip) {
    // Смена причёски через NPC-парикмахера: scriptId и 4 слота сетки
    CmUpdateHairMessage original{3001, 10, 20, 30, 40};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    CmUpdateHairMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.scriptId, restored.scriptId);
    ASSERT_EQ(original.gridLoc0, restored.gridLoc0);
    ASSERT_EQ(original.gridLoc1, restored.gridLoc1);
    ASSERT_EQ(original.gridLoc2, restored.gridLoc2);
    ASSERT_EQ(original.gridLoc3, restored.gridLoc3);
}

TEST(GameplayPhase2, CmTeamFightAskMessage_Roundtrip) {
    // Запрос на командный бой: тип PvP, ID мира и хэндл арены
    CmTeamFightAskMessage original{2, 1001, 5555};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    CmTeamFightAskMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.type, restored.type);
    ASSERT_EQ(original.worldId, restored.worldId);
    ASSERT_EQ(original.handle, restored.handle);
}

TEST(GameplayPhase2, CmItemRepairAskMessage_Roundtrip) {
    // Запрос на ремонт предмета: NPC-кузнец, слот экипировки
    CmItemRepairAskMessage original{700, 7001, 1, 3};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    CmItemRepairAskMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.repairmanId, restored.repairmanId);
    ASSERT_EQ(original.repairmanHandle, restored.repairmanHandle);
    ASSERT_EQ(original.posType, restored.posType);
    ASSERT_EQ(original.posId, restored.posId);
}

TEST(GameplayPhase2, CmRequestTradeMessage_Roundtrip) {
    // Запрос на торговлю с другим игроком
    CmRequestTradeMessage original{1, 42};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    CmRequestTradeMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.type, restored.type);
    ASSERT_EQ(original.charId, restored.charId);
}

TEST(GameplayPhase2, CmAcceptTradeMessage_Roundtrip) {
    // Принятие предложения торговли
    CmAcceptTradeMessage original{1, 99};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    CmAcceptTradeMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.type, restored.type);
    ASSERT_EQ(original.charId, restored.charId);
}

TEST(GameplayPhase2, CmCancelTradeMessage_Roundtrip) {
    // Отмена торговли
    CmCancelTradeMessage original{1, 99};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    CmCancelTradeMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.type, restored.type);
    ASSERT_EQ(original.charId, restored.charId);
}

TEST(GameplayPhase2, CmValidateTradeDataMessage_Roundtrip) {
    // Валидация данных торговли перед подтверждением
    CmValidateTradeDataMessage original{2, 150};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    CmValidateTradeDataMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.type, restored.type);
    ASSERT_EQ(original.charId, restored.charId);
}

TEST(GameplayPhase2, CmValidateTradeMessage_Roundtrip) {
    // Финальное подтверждение торговой сделки
    CmValidateTradeMessage original{2, 150};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    CmValidateTradeMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.type, restored.type);
    ASSERT_EQ(original.charId, restored.charId);
}

TEST(GameplayPhase2, CmAddItemMessage_Roundtrip) {
    // Добавление предмета в окно торговли: тип, ID персонажа, операция, индексы, количество
    CmAddItemMessage original{1, 42, 0, 5, 3001, 10};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    CmAddItemMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.type, restored.type);
    ASSERT_EQ(original.charId, restored.charId);
    ASSERT_EQ(original.opType, restored.opType);
    ASSERT_EQ(original.index, restored.index);
    ASSERT_EQ(original.itemIndex, restored.itemIndex);
    ASSERT_EQ(original.count, restored.count);
}

TEST(GameplayPhase2, CmAddMoneyMessage_Roundtrip) {
    // Добавление денег в торговлю: обычная валюта, 25000 золотых
    CmAddMoneyMessage original{1, 42, 0, 0, 25000};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    CmAddMoneyMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.type, restored.type);
    ASSERT_EQ(original.charId, restored.charId);
    ASSERT_EQ(original.opType, restored.opType);
    ASSERT_EQ(original.isImp, restored.isImp);
    ASSERT_EQ(original.money, restored.money);
}

TEST(GameplayPhase2, CmTigerStartMessage_Roundtrip) {
    // Запуск игры «Тигр» (азартная мини-игра): NPC и три выбора
    CmTigerStartMessage original{500, 1, 2, 3};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    CmTigerStartMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.npcId, restored.npcId);
    ASSERT_EQ(original.sel1, restored.sel1);
    ASSERT_EQ(original.sel2, restored.sel2);
    ASSERT_EQ(original.sel3, restored.sel3);
}

TEST(GameplayPhase2, CmTigerStopMessage_Roundtrip) {
    // Остановка мини-игры «Тигр»
    CmTigerStopMessage original{500, 7};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    CmTigerStopMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.npcId, restored.npcId);
    ASSERT_EQ(original.num, restored.num);
}

TEST(GameplayPhase2, CmVolunteerAsrMessage_Roundtrip) {
    // Ответ на запрос волонтёра: согласие, имя персонажа
    CmVolunteerAsrMessage original{1, "VolunteerHero"};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    CmVolunteerAsrMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.ret, restored.ret);
    ASSERT_EQ(original.name, restored.name);
}

TEST(GameplayPhase2, CmCreateBoatMessage_Roundtrip) {
    // Создание корабля: имя, части (корпус, двигатель, пушка, оборудование)
    CmCreateBoatMessage original{"BlackPearl", 101, 202, 303, 404};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    CmCreateBoatMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.boat, restored.boat);
    ASSERT_EQ(original.header, restored.header);
    ASSERT_EQ(original.engine, restored.engine);
    ASSERT_EQ(original.cannon, restored.cannon);
    ASSERT_EQ(original.equipment, restored.equipment);
}

TEST(GameplayPhase2, CmUpdateBoatMessage_Roundtrip) {
    // Обновление компонентов корабля
    CmUpdateBoatMessage original{111, 222, 333, 444};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    CmUpdateBoatMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.header, restored.header);
    ASSERT_EQ(original.engine, restored.engine);
    ASSERT_EQ(original.cannon, restored.cannon);
    ASSERT_EQ(original.equipment, restored.equipment);
}

TEST(GameplayPhase2, CmSelectBoatListMessage_Roundtrip) {
    // Выбор корабля из списка в доке
    CmSelectBoatListMessage original{600, 2, 5};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    CmSelectBoatListMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.npcId, restored.npcId);
    ASSERT_EQ(original.type, restored.type);
    ASSERT_EQ(original.index, restored.index);
}

TEST(GameplayPhase2, CmBoatLaunchMessage_Roundtrip) {
    // Спуск корабля на воду через NPC дока
    CmBoatLaunchMessage original{600, 3};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    CmBoatLaunchMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.npcId, restored.npcId);
    ASSERT_EQ(original.index, restored.index);
}

TEST(GameplayPhase2, CmBoatBagSelMessage_Roundtrip) {
    // Выбор трюма корабля
    CmBoatBagSelMessage original{600, 7};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    CmBoatBagSelMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.npcId, restored.npcId);
    ASSERT_EQ(original.index, restored.index);
}

TEST(GameplayPhase2, CmReportWgMessage_Roundtrip) {
    // Отправка репорта о нарушителе (античит)
    CmReportWgMessage original{"Speedhack detected near coords 1200,3400"};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    CmReportWgMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.info, restored.info);
}

TEST(GameplayPhase2, CmSay2CampMessage_Roundtrip) {
    // Сообщение в лагерный чат
    CmSay2CampMessage original{"All hands on deck!"};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    CmSay2CampMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.content, restored.content);
}

TEST(GameplayPhase2, CmStallBuyMessage_Roundtrip) {
    // Покупка товара с прилавка другого игрока
    CmStallBuyMessage original{42, 3, 5, 1001};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    CmStallBuyMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.charId, restored.charId);
    ASSERT_EQ(original.index, restored.index);
    ASSERT_EQ(original.count, restored.count);
    ASSERT_EQ(original.gridId, restored.gridId);
}

TEST(GameplayPhase2, CmSkillUpgradeMessage_Roundtrip) {
    // Прокачка навыка: ID скилла и количество добавляемых уровней
    CmSkillUpgradeMessage original{5001, 2};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    CmSkillUpgradeMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.skillId, restored.skillId);
    ASSERT_EQ(original.addGrade, restored.addGrade);
}

TEST(GameplayPhase2, CmRefreshDataMessage_Roundtrip) {
    // Запрос обновления данных персонажа по worldId и handle
    CmRefreshDataMessage original{1001, 8888};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    CmRefreshDataMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.worldId, restored.worldId);
    ASSERT_EQ(original.handle, restored.handle);
}

TEST(GameplayPhase2, CmPkCtrlMessage_Roundtrip) {
    // Переключение PK-режима: 1 = включить, 0 = выключить
    CmPkCtrlMessage original{1};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    CmPkCtrlMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.ctrl, restored.ctrl);
}

TEST(GameplayPhase2, CmItemAmphitheaterAskMessage_Roundtrip) {
    // Запрос на арену (амфитеатр): подтверждение и ID записи
    CmItemAmphitheaterAskMessage original{1, 2001};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    CmItemAmphitheaterAskMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.sure, restored.sure);
    ASSERT_EQ(original.reId, restored.reId);
}

TEST(GameplayPhase2, CmMasterInviteMessage_Roundtrip) {
    // Приглашение мастера (система ученичества)
    CmMasterInviteMessage original{777};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    CmMasterInviteMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.masterId, restored.masterId);
}

TEST(GameplayPhase2, CmMasterAsrMessage_Roundtrip) {
    // Ответ на приглашение мастера: согласие
    CmMasterAsrMessage original{1, 777};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    CmMasterAsrMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.agree, restored.agree);
    ASSERT_EQ(original.masterId, restored.masterId);
}

TEST(GameplayPhase2, CmMasterDelMessage_Roundtrip) {
    // Удаление связи с мастером
    CmMasterDelMessage original{777};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    CmMasterDelMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.masterId, restored.masterId);
}

TEST(GameplayPhase2, CmPrenticeInviteMessage_Roundtrip) {
    // Приглашение ученика
    CmPrenticeInviteMessage original{888};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    CmPrenticeInviteMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.prenticeId, restored.prenticeId);
}

TEST(GameplayPhase2, CmPrenticeAsrMessage_Roundtrip) {
    // Ответ на приглашение ученика: отказ
    CmPrenticeAsrMessage original{0, 888};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    CmPrenticeAsrMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.agree, restored.agree);
    ASSERT_EQ(original.prenticeId, restored.prenticeId);
}

TEST(GameplayPhase2, CmPrenticeDelMessage_Roundtrip) {
    // Удаление связи с учеником
    CmPrenticeDelMessage original{888};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    CmPrenticeDelMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.prenticeId, restored.prenticeId);
}

// -----------------------------------------------------------------
//  CM — NPC Talk (диалоги с NPC)
// -----------------------------------------------------------------

TEST(GameplayPhase2, CmRequestTalkMessage_Roundtrip) {
    // Начало диалога с NPC: ID NPC и команда
    CmRequestTalkMessage original{300, 1};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    CmRequestTalkMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.npcId, restored.npcId);
    ASSERT_EQ(original.cmd, restored.cmd);
}

TEST(GameplayPhase2, CmSelFunctionMessage_Roundtrip) {
    // Выбор функции в диалоге NPC: страница и индекс
    CmSelFunctionMessage original{300, 2, 5};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    CmSelFunctionMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.npcId, restored.npcId);
    ASSERT_EQ(original.pageId, restored.pageId);
    ASSERT_EQ(original.index, restored.index);
}

TEST(GameplayPhase2, CmSaleMessage_Roundtrip) {
    // Продажа предмета NPC: 15 штук из слота 4
    CmSaleMessage original{300, 4, 15};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    CmSaleMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.npcId, restored.npcId);
    ASSERT_EQ(original.index, restored.index);
    ASSERT_EQ(original.count, restored.count);
}

TEST(GameplayPhase2, CmBuyMessage_Roundtrip) {
    // Покупка предмета у NPC: тип, два индекса и количество
    CmBuyMessage original{300, 1, 2, 7, 20};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    CmBuyMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.npcId, restored.npcId);
    ASSERT_EQ(original.itemType, restored.itemType);
    ASSERT_EQ(original.index1, restored.index1);
    ASSERT_EQ(original.index2, restored.index2);
    ASSERT_EQ(original.count, restored.count);
}

TEST(GameplayPhase2, CmMissionPageMessage_Roundtrip) {
    // Навигация по страницам квестов у NPC
    CmMissionPageMessage original{300, 2, 3, 100};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    CmMissionPageMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.npcId, restored.npcId);
    ASSERT_EQ(original.cmd, restored.cmd);
    ASSERT_EQ(original.selItem, restored.selItem);
    ASSERT_EQ(original.param, restored.param);
}

TEST(GameplayPhase2, CmSelMissionMessage_Roundtrip) {
    // Выбор квеста из списка у NPC
    CmSelMissionMessage original{300, 2};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    CmSelMissionMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.npcId, restored.npcId);
    ASSERT_EQ(original.index, restored.index);
}

TEST(GameplayPhase2, CmBlackMarketExchangeReqMessage_Roundtrip) {
    // Обмен на чёрном рынке: NPC, время, ресурсы источника и цели
    CmBlackMarketExchangeReqMessage original{400, 3, 5001, 10, 6001, 5, 2};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    CmBlackMarketExchangeReqMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.npcId, restored.npcId);
    ASSERT_EQ(original.timeNum, restored.timeNum);
    ASSERT_EQ(original.srcId, restored.srcId);
    ASSERT_EQ(original.srcNum, restored.srcNum);
    ASSERT_EQ(original.tarId, restored.tarId);
    ASSERT_EQ(original.tarNum, restored.tarNum);
    ASSERT_EQ(original.index, restored.index);
}

// -----------------------------------------------------------------
//  MC — GameServer → Client
// -----------------------------------------------------------------

TEST(GameplayPhase2, McSay2CampMessage_Roundtrip) {
    // Сообщение в лагерном чате: имя отправителя и текст
    McSay2CampMessage original{"CaptainHook", "Set sail for adventure!"};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McSay2CampMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.chaName, restored.chaName);
    ASSERT_EQ(original.content, restored.content);
}

TEST(GameplayPhase2, McTalkInfoMessage_Roundtrip) {
    // Ответ NPC на запрос диалога: ID NPC, команда и текст
    McTalkInfoMessage original{300, 1, "Welcome to my shop, sailor!"};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McTalkInfoMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.npcId, restored.npcId);
    ASSERT_EQ(original.cmd, restored.cmd);
    ASSERT_EQ(original.text, restored.text);
}

TEST(GameplayPhase2, McTradeDataMessage_Roundtrip) {
    // Данные товара у NPC: страница, позиция, ID предмета, количество, цена
    McTradeDataMessage original{300, 1, 5, 4001, 99, 1500};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McTradeDataMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.npcId, restored.npcId);
    ASSERT_EQ(original.page, restored.page);
    ASSERT_EQ(original.index, restored.index);
    ASSERT_EQ(original.itemId, restored.itemId);
    ASSERT_EQ(original.count, restored.count);
    ASSERT_EQ(original.price, restored.price);
}

TEST(GameplayPhase2, McTradeResultMessage_Roundtrip) {
    // Результат торговой сделки: тип операции, позиция, количество, предмет, деньги
    McTradeResultMessage original{1, 3, 10, 4001, 15000};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McTradeResultMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.type, restored.type);
    ASSERT_EQ(original.index, restored.index);
    ASSERT_EQ(original.count, restored.count);
    ASSERT_EQ(original.itemId, restored.itemId);
    ASSERT_EQ(original.money, restored.money);
}

TEST(GameplayPhase2, McUpdateHairResMessage_Roundtrip) {
    // Результат смены причёски: worldId, новый скрипт и причина (если ошибка)
    McUpdateHairResMessage original{1001, 3001, "Success"};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McUpdateHairResMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.worldId, restored.worldId);
    ASSERT_EQ(original.scriptId, restored.scriptId);
    ASSERT_EQ(original.reason, restored.reason);
}

TEST(GameplayPhase2, McSynTLeaderIdMessage_Roundtrip) {
    // Синхронизация ID лидера группы
    McSynTLeaderIdMessage original{1001, 42};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McSynTLeaderIdMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.worldId, restored.worldId);
    ASSERT_EQ(original.leaderId, restored.leaderId);
}

TEST(GameplayPhase2, McKitbagCapacityMessage_Roundtrip) {
    // Обновление ёмкости инвентаря персонажа
    McKitbagCapacityMessage original{1001, 48};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McKitbagCapacityMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.worldId, restored.worldId);
    ASSERT_EQ(original.capacity, restored.capacity);
}

TEST(GameplayPhase2, McItemForgeAskMessage_Roundtrip) {
    // Запрос кузницы предметов: тип ковки и стоимость
    McItemForgeAskMessage original{2, 7500};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McItemForgeAskMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.type, restored.type);
    ASSERT_EQ(original.money, restored.money);
}

TEST(GameplayPhase2, McItemForgeAnswerMessage_Roundtrip) {
    // Результат ковки: worldId, тип и результат (успех/провал)
    McItemForgeAnswerMessage original{1001, 2, 1};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McItemForgeAnswerMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.worldId, restored.worldId);
    ASSERT_EQ(original.type, restored.type);
    ASSERT_EQ(original.result, restored.result);
}

TEST(GameplayPhase2, McQueryChaMessage_Roundtrip) {
    // Информация о найденном персонаже: координаты, карта, два ID
    McQueryChaMessage original{42, "SeaDog", "ArgentCity", 1200, 3400, 43};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McQueryChaMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.chaId, restored.chaId);
    ASSERT_EQ(original.name, restored.name);
    ASSERT_EQ(original.mapName, restored.mapName);
    ASSERT_EQ(original.posX, restored.posX);
    ASSERT_EQ(original.posY, restored.posY);
    ASSERT_EQ(original.chaId2, restored.chaId2);
}

TEST(GameplayPhase2, McQueryChaPingMessage_Roundtrip) {
    // Пинг-информация о персонаже: имя, карта, задержка
    McQueryChaPingMessage original{42, "SeaDog", "MagicOcean", 85};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McQueryChaPingMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.chaId, restored.chaId);
    ASSERT_EQ(original.name, restored.name);
    ASSERT_EQ(original.mapName, restored.mapName);
    ASSERT_EQ(original.ping, restored.ping);
}

TEST(GameplayPhase2, McQueryReliveMessage_Roundtrip) {
    // Запрос на воскрешение: кто воскрешает и тип воскрешения
    McQueryReliveMessage original{42, "Healer", 1};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McQueryReliveMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.chaId, restored.chaId);
    ASSERT_EQ(original.sourceName, restored.sourceName);
    ASSERT_EQ(original.reliveType, restored.reliveType);
}

TEST(GameplayPhase2, McGmMailMessage_Roundtrip) {
    // GM-рассылка: заголовок, текст и время отправки
    McGmMailMessage original{"Server Update", "Maintenance scheduled at 03:00 UTC", 1711500000};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McGmMailMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.title, restored.title);
    ASSERT_EQ(original.content, restored.content);
    ASSERT_EQ(original.time, restored.time);
}

TEST(GameplayPhase2, McSynStallNameMessage_Roundtrip) {
    // Синхронизация названия прилавка: ID персонажа + название
    McSynStallNameMessage original{12345, "Captain's Rare Goods"};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McSynStallNameMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.charId, restored.charId);
    ASSERT_EQ(original.name, restored.name);
}

TEST(GameplayPhase2, McMapCrashMessage_Roundtrip) {
    // Уведомление о краше карты
    McMapCrashMessage original{"Map instance 'DeadIsland' crashed unexpectedly"};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McMapCrashMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.text, restored.text);
}

TEST(GameplayPhase2, McVolunteerStateMessage_Roundtrip) {
    // Состояние волонтёра: 1 = активен
    McVolunteerStateMessage original{1};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McVolunteerStateMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.state, restored.state);
}

TEST(GameplayPhase2, McVolunteerAskMessage_Roundtrip) {
    // Запрос от волонтёра: имя запрашивающего
    McVolunteerAskMessage original{"HelpfulPirate"};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McVolunteerAskMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.name, restored.name);
}

TEST(GameplayPhase2, McMasterAskMessage_Roundtrip) {
    // Запрос от мастера: имя и ID персонажа мастера
    McMasterAskMessage original{"GrandMaster", 777};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McMasterAskMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.name, restored.name);
    ASSERT_EQ(original.chaId, restored.chaId);
}

TEST(GameplayPhase2, McPrenticeAskMessage_Roundtrip) {
    // Запрос от ученика: имя и ID персонажа ученика
    McPrenticeAskMessage original{"YoungSailor", 888};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McPrenticeAskMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.name, restored.name);
    ASSERT_EQ(original.chaId, restored.chaId);
}

TEST(GameplayPhase2, McItemRepairAskMcMessage_Roundtrip) {
    // Стоимость ремонта предмета: имя предмета и цена
    McItemRepairAskMcMessage original{"Enchanted Cutlass", 3500};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McItemRepairAskMcMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.itemName, restored.itemName);
    ASSERT_EQ(original.repairCost, restored.repairCost);
}

TEST(GameplayPhase2, McItemLotteryAsrMessage_Roundtrip) {
    // Результат лотереи предметов: 1 = выигрыш
    McItemLotteryAsrMessage original{1};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McItemLotteryAsrMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.success, restored.success);
}

// =================================================================
//  Геймплейные команды: Фаза 3 — сложные
// =================================================================

TEST(GameplayPhase3, McSynAttributeMessage_Roundtrip) {
    // Синхронизация атрибутов персонажа: worldId + 2 атрибута
    McSynAttributeMessage original;
    original.worldId = 42;
    original.attr.synType = 1;
    original.attr.attrs = {{10, 100}, {20, 200}};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McSynAttributeMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.worldId, restored.worldId);
    ASSERT_EQ(original.attr.synType, restored.attr.synType);
    ASSERT_EQ(2u, restored.attr.attrs.size());
    ASSERT_EQ(10, restored.attr.attrs[0].attrId);
    ASSERT_EQ(100, restored.attr.attrs[0].attrVal);
    ASSERT_EQ(20, restored.attr.attrs[1].attrId);
    ASSERT_EQ(200, restored.attr.attrs[1].attrVal);
}

TEST(GameplayPhase3, McSynSkillStateMessage_Roundtrip) {
    // Синхронизация состояний скиллов: worldId + 2 состояния
    McSynSkillStateMessage original;
    original.worldId = 55;
    original.skillState.currentTime = 9999;
    original.skillState.states = {{1, 3, 5000, 1000}, {2, 5, 8000, 2000}};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McSynSkillStateMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.worldId, restored.worldId);
    ASSERT_EQ(original.skillState.currentTime, restored.skillState.currentTime);
    ASSERT_EQ(2u, restored.skillState.states.size());
    ASSERT_EQ(1, restored.skillState.states[0].stateId);
    ASSERT_EQ(3, restored.skillState.states[0].stateLv);
    ASSERT_EQ(5000, restored.skillState.states[0].duration);
    ASSERT_EQ(1000, restored.skillState.states[0].startTime);
    ASSERT_EQ(2, restored.skillState.states[1].stateId);
    ASSERT_EQ(5, restored.skillState.states[1].stateLv);
}

TEST(GameplayPhase3, McAStateEndSeeMessage_Roundtrip) {
    // Конец видимости area-состояний: только координаты области (без массива)
    McAStateEndSeeMessage original{100, 200};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McAStateEndSeeMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.areaX, restored.areaX);
    ASSERT_EQ(original.areaY, restored.areaY);
}

TEST(GameplayPhase3, McAStateBeginSeeMessage_Roundtrip) {
    // Начало видимости area-состояний: запись с stateId=0 (без доп. полей) и stateId=5 (полная)
    McAStateBeginSeeMessage original;
    original.areaX = 300;
    original.areaY = 400;
    AStateBeginSeeEntry e0{0, 0, 0, 0};   // stateId=0 — сериализуется без stateLv/worldId/fightId
    AStateBeginSeeEntry e1{5, 10, 77, 88}; // stateId=5 — полная запись
    original.states = {e0, e1};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McAStateBeginSeeMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.areaX, restored.areaX);
    ASSERT_EQ(original.areaY, restored.areaY);
    ASSERT_EQ(2u, restored.states.size());
    // Запись с stateId=0: поля обнуляются при десериализации
    ASSERT_EQ(0, restored.states[0].stateId);
    ASSERT_EQ(0, restored.states[0].stateLv);
    ASSERT_EQ(0, restored.states[0].worldId);
    ASSERT_EQ(0, restored.states[0].fightId);
    // Запись с stateId=5: полные данные
    ASSERT_EQ(5, restored.states[1].stateId);
    ASSERT_EQ(10, restored.states[1].stateLv);
    ASSERT_EQ(77, restored.states[1].worldId);
    ASSERT_EQ(88, restored.states[1].fightId);
}

TEST(GameplayPhase3, McDelItemChaMessage_Roundtrip) {
    // Удаление предмета персонажа: mainChaId + worldId
    McDelItemChaMessage original{1001, 2002};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McDelItemChaMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.mainChaId, restored.mainChaId);
    ASSERT_EQ(original.worldId, restored.worldId);
}

TEST(GameplayPhase3, McSynEventInfoMessage_Roundtrip) {
    // Синхронизация информации о событии сущности
    McSynEventInfoMessage original{50, 2, 100, "TreasureHunt"};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McSynEventInfoMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.entityId, restored.entityId);
    ASSERT_EQ(original.entityType, restored.entityType);
    ASSERT_EQ(original.eventId, restored.eventId);
    ASSERT_EQ(original.eventName, restored.eventName);
}

TEST(GameplayPhase3, McSynSideInfoMessage_Roundtrip) {
    // Синхронизация стороны (фракции) персонажа
    McSynSideInfoMessage original;
    original.worldId = 77;
    original.side.sideId = 3;
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McSynSideInfoMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.worldId, restored.worldId);
    ASSERT_EQ(original.side.sideId, restored.side.sideId);
}

TEST(GameplayPhase3, McMissionInfoMessage_Roundtrip) {
    // Информация о квестах: NPC + параметры списка + массив строк-функций
    McMissionInfoMessage original;
    original.npcId = 9001; original.listType = 1;
    original.prev = 0; original.next = 1;
    original.prevCmd = 2; original.nextCmd = 3;
    original.items = {"FindTheMap", "DefeatKraken"};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McMissionInfoMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.npcId, restored.npcId);
    ASSERT_EQ(original.listType, restored.listType);
    ASSERT_EQ(original.prev, restored.prev);
    ASSERT_EQ(original.next, restored.next);
    ASSERT_EQ(original.prevCmd, restored.prevCmd);
    ASSERT_EQ(original.nextCmd, restored.nextCmd);
    ASSERT_EQ(2u, restored.items.size());
    ASSERT_EQ("FindTheMap", restored.items[0]);
    ASSERT_EQ("DefeatKraken", restored.items[1]);
}

TEST(GameplayPhase3, McMisPageMessage_Roundtrip_ItemKill) {
    // Страница квеста: потребности ITEM и KILL (условная сериализация)
    McMisPageMessage original;
    original.cmd = 1; original.npcId = 5001; original.name = "Kill the Pirates";
    original.needs = {
        {MIS_NEED_ITEM, 100, 5, 0, ""},
        {MIS_NEED_KILL, 200, 10, 3, ""}
    };
    original.prizeSelType = 0;
    original.prizes = {{0, 300, 1}};
    original.description = "Defeat pirates and collect loot";
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McMisPageMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.cmd, restored.cmd);
    ASSERT_EQ(original.npcId, restored.npcId);
    ASSERT_EQ(original.name, restored.name);
    ASSERT_EQ(2u, restored.needs.size());
    ASSERT_EQ(MIS_NEED_ITEM, restored.needs[0].needType);
    ASSERT_EQ(100, restored.needs[0].param1);
    ASSERT_EQ(5, restored.needs[0].param2);
    ASSERT_EQ(MIS_NEED_KILL, restored.needs[1].needType);
    ASSERT_EQ(200, restored.needs[1].param1);
    ASSERT_EQ(10, restored.needs[1].param2);
    ASSERT_EQ(3, restored.needs[1].param3);
    ASSERT_EQ(1u, restored.prizes.size());
    ASSERT_EQ(300, restored.prizes[0].param1);
    ASSERT_EQ(original.description, restored.description);
}

TEST(GameplayPhase3, McMisPageMessage_Roundtrip_Desp) {
    // Страница квеста: потребность-описание (MIS_NEED_DESP = 5)
    McMisPageMessage original;
    original.cmd = 2; original.npcId = 6001; original.name = "Explore the Island";
    original.needs = {
        {MIS_NEED_DESP, 0, 0, 0, "Find the hidden cave entrance"}
    };
    original.prizeSelType = 1;
    original.prizes = {{1, 500, 2}, {0, 400, 3}};
    original.description = "The island holds many secrets";
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McMisPageMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(1u, restored.needs.size());
    ASSERT_EQ(MIS_NEED_DESP, restored.needs[0].needType);
    ASSERT_EQ("Find the hidden cave entrance", restored.needs[0].desp);
    ASSERT_EQ(2u, restored.prizes.size());
    ASSERT_EQ(original.description, restored.description);
}

TEST(GameplayPhase3, McMisLogMessage_Roundtrip) {
    // Журнал миссий: 2 записи (id миссии + состояние)
    McMisLogMessage original;
    original.logs = {{10, 1}, {20, 3}};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McMisLogMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(2u, restored.logs.size());
    ASSERT_EQ(10, restored.logs[0].misId);
    ASSERT_EQ(1, restored.logs[0].state);
    ASSERT_EQ(20, restored.logs[1].misId);
    ASSERT_EQ(3, restored.logs[1].state);
}

TEST(GameplayPhase3, McMisLogInfoMessage_Roundtrip) {
    // Детали записи журнала квестов: формат аналогичен MisPage
    McMisLogInfoMessage original;
    original.misId = 7001; original.name = "Skeleton Hunt";
    original.needs = {
        {MIS_NEED_KILL, 300, 10, 0, ""},
        {MIS_NEED_DESP, 0, 0, 0, "Defeat 10 skeleton warriors"}
    };
    original.prizeSelType = 0;
    original.prizes = {{0, 1000, 1}};
    original.description = "Hunt skeletons in the graveyard";
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McMisLogInfoMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.misId, restored.misId);
    ASSERT_EQ(original.name, restored.name);
    ASSERT_EQ(2u, restored.needs.size());
    ASSERT_EQ(MIS_NEED_KILL, restored.needs[0].needType);
    ASSERT_EQ(300, restored.needs[0].param1);
    ASSERT_EQ(MIS_NEED_DESP, restored.needs[1].needType);
    ASSERT_EQ("Defeat 10 skeleton warriors", restored.needs[1].desp);
    ASSERT_EQ(1u, restored.prizes.size());
    ASSERT_EQ(original.description, restored.description);
}

TEST(GameplayPhase3, McMisLogClearMcMessage_Roundtrip) {
    // Очистка записи журнала миссий по ID миссии
    McMisLogClearMcMessage original{42};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McMisLogClearMcMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.missionId, restored.missionId);
}

TEST(GameplayPhase3, McMisLogAddMessage_Roundtrip) {
    // Добавление записи в журнал миссий: ID миссии + состояние
    McMisLogAddMessage original{99, 1};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McMisLogAddMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.missionId, restored.missionId);
    ASSERT_EQ(original.state, restored.state);
}

TEST(GameplayPhase3, McMisLogStateMessage_Roundtrip) {
    // Изменение состояния журнала миссий: ID миссии + новое состояние
    McMisLogStateMessage original{3001, 7};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McMisLogStateMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.missionId, restored.missionId);
    ASSERT_EQ(original.state, restored.state);
}

TEST(GameplayPhase3, McFuncInfoMessage_Roundtrip) {
    // Информация NPC-функции: 2 функции + 1 квестовый пункт
    McFuncInfoMessage original;
    original.npcId = 500;
    original.page = 1;
    original.talkText = "Ahoy! What can I do for ye?";
    original.funcItems = {{"Shop"}, {"Repair"}};
    original.missionItems = {{"DeliverGoods", 2}};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McFuncInfoMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.npcId, restored.npcId);
    ASSERT_EQ(original.page, restored.page);
    ASSERT_EQ(original.talkText, restored.talkText);
    ASSERT_EQ(2u, restored.funcItems.size());
    ASSERT_EQ("Shop", restored.funcItems[0].name);
    ASSERT_EQ("Repair", restored.funcItems[1].name);
    ASSERT_EQ(1u, restored.missionItems.size());
    ASSERT_EQ("DeliverGoods", restored.missionItems[0].name);
    ASSERT_EQ(2, restored.missionItems[0].state);
}

TEST(GameplayPhase3, McVolunteerListMessage_Roundtrip) {
    // Список волонтёров: 2 записи с пагинацией
    McVolunteerListMessage original;
    original.pageTotal = 5;
    original.page = 2;
    original.volunteers = {{"Sailor1", 30, 2, "DeadIsland"}, {"Sailor2", 45, 3, "Atlantis"}};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McVolunteerListMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.pageTotal, restored.pageTotal);
    ASSERT_EQ(original.page, restored.page);
    ASSERT_EQ(2u, restored.volunteers.size());
    ASSERT_EQ("Sailor1", restored.volunteers[0].name);
    ASSERT_EQ(30, restored.volunteers[0].level);
    ASSERT_EQ(2, restored.volunteers[0].job);
    ASSERT_EQ("DeadIsland", restored.volunteers[0].map);
    ASSERT_EQ("Sailor2", restored.volunteers[1].name);
    ASSERT_EQ(45, restored.volunteers[1].level);
}

TEST(GameplayPhase3, McVolunteerOpenMessage_Roundtrip) {
    // Открытие панели волонтёров: состояние + 2 записи
    McVolunteerOpenMessage original;
    original.state = 1;
    original.pageTotal = 3;
    original.volunteers = {{"Helper1", 20, 1, "PortRoyal"}, {"Helper2", 35, 4, "TortugaBay"}};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McVolunteerOpenMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.state, restored.state);
    ASSERT_EQ(original.pageTotal, restored.pageTotal);
    ASSERT_EQ(2u, restored.volunteers.size());
    ASSERT_EQ("Helper1", restored.volunteers[0].name);
    ASSERT_EQ(20, restored.volunteers[0].level);
    ASSERT_EQ(1, restored.volunteers[0].job);
    ASSERT_EQ("PortRoyal", restored.volunteers[0].map);
    ASSERT_EQ("Helper2", restored.volunteers[1].name);
    ASSERT_EQ(35, restored.volunteers[1].level);
}

TEST(GameplayPhase3, McShowStallSearchMessage_Roundtrip) {
    // Результаты поиска прилавков: 2 записи
    McShowStallSearchMessage original;
    original.entries = {
        {"CaptainJack", "Jack's Stall", "Harbor", 5, 1500},
        {"MerchantAnne", "Anne's Goods", "Market", 12, 3200}
    };
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McShowStallSearchMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(2u, restored.entries.size());
    ASSERT_EQ("CaptainJack", restored.entries[0].name);
    ASSERT_EQ("Jack's Stall", restored.entries[0].stallName);
    ASSERT_EQ("Harbor", restored.entries[0].location);
    ASSERT_EQ(5, restored.entries[0].count);
    ASSERT_EQ(1500, restored.entries[0].cost);
    ASSERT_EQ("MerchantAnne", restored.entries[1].name);
    ASSERT_EQ("Anne's Goods", restored.entries[1].stallName);
    ASSERT_EQ("Market", restored.entries[1].location);
    ASSERT_EQ(12, restored.entries[1].count);
    ASSERT_EQ(3200, restored.entries[1].cost);
}

TEST(GameplayPhase3, McShowRankingMessage_Roundtrip) {
    // Таблица рейтинга: 2 записи
    McShowRankingMessage original;
    original.entries = {
        {"PirateKing", "SeaWolves", 80, 5, 99999},
        {"Navigator", "WindRiders", 75, 3, 88888}
    };
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McShowRankingMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(2u, restored.entries.size());
    ASSERT_EQ("PirateKing", restored.entries[0].name);
    ASSERT_EQ("SeaWolves", restored.entries[0].guild);
    ASSERT_EQ(80, restored.entries[0].level);
    ASSERT_EQ(5, restored.entries[0].job);
    ASSERT_EQ(99999, restored.entries[0].score);
    ASSERT_EQ("Navigator", restored.entries[1].name);
    ASSERT_EQ("WindRiders", restored.entries[1].guild);
    ASSERT_EQ(75, restored.entries[1].level);
    ASSERT_EQ(3, restored.entries[1].job);
    ASSERT_EQ(88888, restored.entries[1].score);
}

TEST(GameplayPhase3, McEspeItemMessage_Roundtrip) {
    // Специальные предметы: worldId + 3 записи
    McEspeItemMessage original;
    original.worldId = 123;
    original.items = {
        {0, 100, 50, 1, 3600},
        {1, 200, 75, 0, 7200},
        {2, 300, 25, 1, 0}
    };
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McEspeItemMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.worldId, restored.worldId);
    ASSERT_EQ(3u, restored.items.size());
    ASSERT_EQ(0, restored.items[0].position);
    ASSERT_EQ(100, restored.items[0].endure);
    ASSERT_EQ(50, restored.items[0].energy);
    ASSERT_EQ(1, restored.items[0].tradable);
    ASSERT_EQ(3600, restored.items[0].expiration);
    ASSERT_EQ(1, restored.items[1].position);
    ASSERT_EQ(200, restored.items[1].endure);
    ASSERT_EQ(2, restored.items[2].position);
    ASSERT_EQ(300, restored.items[2].endure);
    ASSERT_EQ(25, restored.items[2].energy);
}

TEST(GameplayPhase3, McBlackMarketExchangeDataMessage_Roundtrip) {
    // Данные обмена чёрного рынка: npcId + 2 записи обменов
    McBlackMarketExchangeDataMessage original;
    original.npcId = 900;
    original.exchanges = {
        {1001, 5, 2001, 1, 3600},
        {1002, 10, 2002, 3, 7200}
    };
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McBlackMarketExchangeDataMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.npcId, restored.npcId);
    ASSERT_EQ(2u, restored.exchanges.size());
    ASSERT_EQ(1001, restored.exchanges[0].srcId);
    ASSERT_EQ(5, restored.exchanges[0].srcCount);
    ASSERT_EQ(2001, restored.exchanges[0].tarId);
    ASSERT_EQ(1, restored.exchanges[0].tarCount);
    ASSERT_EQ(3600, restored.exchanges[0].timeValue);
    ASSERT_EQ(1002, restored.exchanges[1].srcId);
    ASSERT_EQ(10, restored.exchanges[1].srcCount);
    ASSERT_EQ(2002, restored.exchanges[1].tarId);
    ASSERT_EQ(3, restored.exchanges[1].tarCount);
    ASSERT_EQ(7200, restored.exchanges[1].timeValue);
}

TEST(GameplayPhase3, McExchangeDataMessage_Roundtrip) {
    // Данные обмена (обычный): npcId + 2 записи без timeValue
    McExchangeDataMessage original;
    original.npcId = 800;
    original.exchanges = {
        {3001, 2, 4001, 1},
        {3002, 8, 4002, 4}
    };
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McExchangeDataMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.npcId, restored.npcId);
    ASSERT_EQ(2u, restored.exchanges.size());
    ASSERT_EQ(3001, restored.exchanges[0].srcId);
    ASSERT_EQ(2, restored.exchanges[0].srcCount);
    ASSERT_EQ(4001, restored.exchanges[0].tarId);
    ASSERT_EQ(1, restored.exchanges[0].tarCount);
    ASSERT_EQ(3002, restored.exchanges[1].srcId);
    ASSERT_EQ(8, restored.exchanges[1].srcCount);
    ASSERT_EQ(4002, restored.exchanges[1].tarId);
    ASSERT_EQ(4, restored.exchanges[1].tarCount);
}

TEST(GameplayPhase3, McBlackMarketExchangeUpdateMessage_Roundtrip) {
    // Обновление обменов чёрного рынка: npcId + 2 записи
    McBlackMarketExchangeUpdateMessage original;
    original.npcId = 950;
    original.exchanges = {
        {5001, 3, 6001, 2, 1800},
        {5002, 7, 6002, 5, 5400}
    };
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McBlackMarketExchangeUpdateMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.npcId, restored.npcId);
    ASSERT_EQ(2u, restored.exchanges.size());
    ASSERT_EQ(5001, restored.exchanges[0].srcId);
    ASSERT_EQ(3, restored.exchanges[0].srcCount);
    ASSERT_EQ(6001, restored.exchanges[0].tarId);
    ASSERT_EQ(2, restored.exchanges[0].tarCount);
    ASSERT_EQ(1800, restored.exchanges[0].timeValue);
    ASSERT_EQ(5002, restored.exchanges[1].srcId);
    ASSERT_EQ(7, restored.exchanges[1].srcCount);
    ASSERT_EQ(6002, restored.exchanges[1].tarId);
    ASSERT_EQ(5, restored.exchanges[1].tarCount);
    ASSERT_EQ(5400, restored.exchanges[1].timeValue);
}

TEST(GameplayPhase3, McBlackMarketExchangeAsrMessage_Roundtrip) {
    // Результат обмена на чёрном рынке: все 5 полей
    McBlackMarketExchangeAsrMessage original{1, 7001, 4, 8001, 2};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McBlackMarketExchangeAsrMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.success, restored.success);
    ASSERT_EQ(original.srcId, restored.srcId);
    ASSERT_EQ(original.srcCount, restored.srcCount);
    ASSERT_EQ(original.tarId, restored.tarId);
    ASSERT_EQ(original.tarCount, restored.tarCount);
}

// =================================================================
//  Торговля, магазин, лавки, пати — сложные команды
// =================================================================

TEST(TradeStoreStall, McTradeAllDataMessage_TradeGoods_Roundtrip) {
    // Полные данные торговли NPC: tradeType=1 (TRADE_GOODS) — с count/price/level
    McTradeAllDataMessage original;
    original.npcId = 5001; original.tradeType = 1; original.param = 42;
    TradePage page1; page1.itemType = 10;
    page1.items = { {100, 5, 200, 3}, {101, 10, 350, 5} };
    TradePage page2; page2.itemType = 20;
    page2.items = { {200, 1, 999, 10} };
    original.pages = { page1, page2 };

    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McTradeAllDataMessage restored;
    deserialize(r, restored);

    ASSERT_EQ(5001, restored.npcId);
    ASSERT_EQ(1, restored.tradeType);
    ASSERT_EQ(42, restored.param);
    ASSERT_EQ(2u, restored.pages.size());
    // Первая страница
    ASSERT_EQ(10, restored.pages[0].itemType);
    ASSERT_EQ(2u, restored.pages[0].items.size());
    ASSERT_EQ(100, restored.pages[0].items[0].itemId);
    ASSERT_EQ(5, restored.pages[0].items[0].count);
    ASSERT_EQ(200, restored.pages[0].items[0].price);
    ASSERT_EQ(3, restored.pages[0].items[0].level);
    ASSERT_EQ(101, restored.pages[0].items[1].itemId);
    ASSERT_EQ(10, restored.pages[0].items[1].count);
    ASSERT_EQ(350, restored.pages[0].items[1].price);
    ASSERT_EQ(5, restored.pages[0].items[1].level);
    // Вторая страница
    ASSERT_EQ(20, restored.pages[1].itemType);
    ASSERT_EQ(1u, restored.pages[1].items.size());
    ASSERT_EQ(200, restored.pages[1].items[0].itemId);
    ASSERT_EQ(999, restored.pages[1].items[0].price);
}

TEST(TradeStoreStall, McTradeAllDataMessage_NonGoods_Roundtrip) {
    // Полные данные торговли NPC: tradeType=0 — без count/price/level
    McTradeAllDataMessage original;
    original.npcId = 6001; original.tradeType = 0; original.param = 7;
    TradePage page; page.itemType = 30;
    page.items = { {300, 0, 0, 0}, {301, 0, 0, 0} };
    original.pages = { page };

    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McTradeAllDataMessage restored;
    deserialize(r, restored);

    ASSERT_EQ(6001, restored.npcId);
    ASSERT_EQ(0, restored.tradeType);
    ASSERT_EQ(1u, restored.pages.size());
    ASSERT_EQ(2u, restored.pages[0].items.size());
    ASSERT_EQ(300, restored.pages[0].items[0].itemId);
    ASSERT_EQ(0, restored.pages[0].items[0].count);
    ASSERT_EQ(0, restored.pages[0].items[0].price);
}

TEST(TradeStoreStall, McStoreOpenAnswerMessage_Valid_Roundtrip) {
    // Ответ на открытие магазина: valid=true с объявлениями и категориями
    McStoreOpenAnswerMessage original;
    original.isValid = true; original.vip = 1; original.moBean = 500; original.replMoney = 100;
    original.affiches = { {1, "Summer Sale", "SALE01"}, {2, "New Arrivals", "NEW02"} };
    original.classes = { {10, "Weapons", 0}, {11, "Swords", 10} };

    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McStoreOpenAnswerMessage restored;
    deserialize(r, restored);

    ASSERT_TRUE(restored.isValid);
    ASSERT_EQ(1, restored.vip);
    ASSERT_EQ(500, restored.moBean);
    ASSERT_EQ(100, restored.replMoney);
    ASSERT_EQ(2u, restored.affiches.size());
    ASSERT_EQ(1, restored.affiches[0].afficheId);
    ASSERT_EQ("Summer Sale", restored.affiches[0].title);
    ASSERT_EQ("SALE01", restored.affiches[0].comId);
    ASSERT_EQ(2, restored.affiches[1].afficheId);
    ASSERT_EQ(2u, restored.classes.size());
    ASSERT_EQ(10, restored.classes[0].classId);
    ASSERT_EQ("Weapons", restored.classes[0].className);
    ASSERT_EQ(0, restored.classes[0].parentId);
    ASSERT_EQ(11, restored.classes[1].classId);
    ASSERT_EQ("Swords", restored.classes[1].className);
    ASSERT_EQ(10, restored.classes[1].parentId);
}

TEST(TradeStoreStall, McStoreOpenAnswerMessage_Invalid_Roundtrip) {
    // Ответ на открытие магазина: valid=false — пустые данные
    McStoreOpenAnswerMessage original;
    original.isValid = false;

    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McStoreOpenAnswerMessage restored;
    deserialize(r, restored);

    ASSERT_FALSE(restored.isValid);
    ASSERT_EQ(0u, restored.affiches.size());
    ASSERT_EQ(0u, restored.classes.size());
}

TEST(TradeStoreStall, McStoreListAnswerMessage_Roundtrip) {
    // Список товаров магазина: 1 товар с 2 вариантами и 5 атрибутами
    McStoreListAnswerMessage original;
    original.pageTotal = 3; original.pageCurrent = 1;
    StoreProductEntry product;
    product.comId = 1001; product.comName = "Dark Sword"; product.price = 5000;
    product.remark = "Legendary weapon"; product.isHot = true;
    product.time = 86400; product.quantity = 50; product.expire = 0;
    product.variants = { {2001, 1, 0}, {2002, 3, 1} };
    product.attrs[0] = {1, 100}; product.attrs[1] = {2, 50};
    product.attrs[2] = {3, 25}; product.attrs[3] = {0, 0}; product.attrs[4] = {0, 0};
    original.products = { product };

    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McStoreListAnswerMessage restored;
    deserialize(r, restored);

    ASSERT_EQ(3, restored.pageTotal);
    ASSERT_EQ(1, restored.pageCurrent);
    ASSERT_EQ(1u, restored.products.size());
    auto& p = restored.products[0];
    ASSERT_EQ(1001, p.comId);
    ASSERT_EQ("Dark Sword", p.comName);
    ASSERT_EQ(5000, p.price);
    ASSERT_EQ("Legendary weapon", p.remark);
    ASSERT_TRUE(p.isHot);
    ASSERT_EQ(86400, p.time);
    ASSERT_EQ(50, p.quantity);
    ASSERT_EQ(0, p.expire);
    ASSERT_EQ(2u, p.variants.size());
    ASSERT_EQ(2001, p.variants[0].itemId);
    ASSERT_EQ(1, p.variants[0].itemNum);
    ASSERT_EQ(0, p.variants[0].flute);
    ASSERT_EQ(2002, p.variants[1].itemId);
    ASSERT_EQ(3, p.variants[1].itemNum);
    ASSERT_EQ(1, p.variants[1].flute);
    ASSERT_EQ(1, p.attrs[0].attrId);
    ASSERT_EQ(100, p.attrs[0].attrVal);
    ASSERT_EQ(2, p.attrs[1].attrId);
    ASSERT_EQ(50, p.attrs[1].attrVal);
}

TEST(TradeStoreStall, McStoreHistoryMessage_Roundtrip) {
    // История покупок магазина: 2 записи
    McStoreHistoryMessage original;
    original.records = {
        {"2026-03-27 10:00", 3001, "HP Potion", 100},
        {"2026-03-27 11:30", 3002, "SP Potion", 150}
    };

    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McStoreHistoryMessage restored;
    deserialize(r, restored);

    ASSERT_EQ(2u, restored.records.size());
    ASSERT_EQ("2026-03-27 10:00", restored.records[0].time);
    ASSERT_EQ(3001, restored.records[0].itemId);
    ASSERT_EQ("HP Potion", restored.records[0].name);
    ASSERT_EQ(100, restored.records[0].money);
    ASSERT_EQ("2026-03-27 11:30", restored.records[1].time);
    ASSERT_EQ(3002, restored.records[1].itemId);
    ASSERT_EQ("SP Potion", restored.records[1].name);
    ASSERT_EQ(150, restored.records[1].money);
}

TEST(TradeStoreStall, McStoreVipMessage_Success_Roundtrip) {
    // VIP-данные магазина: success=1
    McStoreVipMessage original;
    original.success = 1; original.vipId = 5; original.months = 3;
    original.shuijing = 1000; original.modou = 500;

    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McStoreVipMessage restored;
    deserialize(r, restored);

    ASSERT_EQ(1, restored.success);
    ASSERT_EQ(5, restored.vipId);
    ASSERT_EQ(3, restored.months);
    ASSERT_EQ(1000, restored.shuijing);
    ASSERT_EQ(500, restored.modou);
}

TEST(TradeStoreStall, McStoreVipMessage_Failure_Roundtrip) {
    // VIP-данные магазина: success=0 — пустые данные
    McStoreVipMessage original;
    original.success = 0;

    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McStoreVipMessage restored;
    deserialize(r, restored);

    ASSERT_EQ(0, restored.success);
    ASSERT_EQ(0, restored.vipId);
}

TEST(TradeStoreStall, McSynTeamMessage_Character_Roundtrip) {
    // Синхронизация члена пати: HP/SP/Level + ChaLookInfo (персонаж, не лодка)
    McSynTeamMessage original;
    original.memberId = 7001; original.hp = 500; original.maxHP = 1000;
    original.sp = 200; original.maxSP = 400; original.level = 45;
    original.look.synType = 0; original.look.typeId = 3; original.look.isBoat = false;
    original.look.hairId = 12;
    for (int i = 0; i < EQUIP_NUM; ++i) {
        original.look.equips[i].id = i + 1;
        original.look.equips[i].dbId = i * 10;
        original.look.equips[i].needLv = 0;
    }

    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McSynTeamMessage restored;
    deserialize(r, restored);

    ASSERT_EQ(7001, restored.memberId);
    ASSERT_EQ(500, restored.hp);
    ASSERT_EQ(1000, restored.maxHP);
    ASSERT_EQ(200, restored.sp);
    ASSERT_EQ(400, restored.maxSP);
    ASSERT_EQ(45, restored.level);
    ASSERT_FALSE(restored.look.isBoat);
    ASSERT_EQ(12, restored.look.hairId);
    ASSERT_EQ(1, restored.look.equips[0].id);
    ASSERT_EQ(0, restored.look.equips[0].dbId);
}

// =================================================================
//  Новые сложные команды: McItemCreate, McSynSkillBag
// =================================================================

TEST(GameplayPhase3, McItemCreateMessage_Roundtrip) {
    // Предмет на земле: 9 полей + ChaEventInfo
    McItemCreateMessage original;
    original.worldId = 1001; original.handle = 2002; original.itemId = 3003;
    original.posX = 500; original.posY = 600; original.angle = 45;
    original.num = 3; original.appeType = 1; original.fromId = 4004;
    original.event = {5005, 2, 100, "ItemDrop"};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McItemCreateMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.worldId, restored.worldId);
    ASSERT_EQ(original.handle, restored.handle);
    ASSERT_EQ(original.itemId, restored.itemId);
    ASSERT_EQ(original.posX, restored.posX);
    ASSERT_EQ(original.posY, restored.posY);
    ASSERT_EQ(original.angle, restored.angle);
    ASSERT_EQ(original.num, restored.num);
    ASSERT_EQ(original.appeType, restored.appeType);
    ASSERT_EQ(original.fromId, restored.fromId);
    ASSERT_EQ(original.event.entityId, restored.event.entityId);
    ASSERT_EQ(original.event.entityType, restored.event.entityType);
    ASSERT_EQ(original.event.eventId, restored.event.eventId);
    ASSERT_EQ(original.event.eventName, restored.event.eventName);
}

TEST(GameplayPhase3, McSynSkillBagMessage_Roundtrip_WithRange) {
    // Сумка скиллов: 2 скилла, один с range (условная сериализация)
    McSynSkillBagMessage original;
    original.worldId = 9001;
    original.skillBag.defSkillId = 100;
    original.skillBag.synType = 1;
    SkillEntry sk1; sk1.id = 10; sk1.state = 1; sk1.level = 5;
    sk1.useSp = 20; sk1.useEndure = 10; sk1.useEnergy = 30; sk1.resumeTime = 2000;
    sk1.range[0] = 1; sk1.range[1] = 50; sk1.range[2] = 100; sk1.range[3] = 150;
    SkillEntry sk2; sk2.id = 20; sk2.state = 0; sk2.level = 3;
    sk2.useSp = 15; sk2.useEndure = 5; sk2.useEnergy = 25; sk2.resumeTime = 1000;
    sk2.range[0] = 0; sk2.range[1] = 0; sk2.range[2] = 0; sk2.range[3] = 0;
    original.skillBag.skills = {sk1, sk2};
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McSynSkillBagMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.worldId, restored.worldId);
    ASSERT_EQ(original.skillBag.defSkillId, restored.skillBag.defSkillId);
    ASSERT_EQ(original.skillBag.synType, restored.skillBag.synType);
    ASSERT_EQ(2u, restored.skillBag.skills.size());
    // Первый скилл: range[0] != 0 → range полностью
    ASSERT_EQ(10, restored.skillBag.skills[0].id);
    ASSERT_EQ(5, restored.skillBag.skills[0].level);
    ASSERT_EQ(1, restored.skillBag.skills[0].range[0]);
    ASSERT_EQ(50, restored.skillBag.skills[0].range[1]);
    ASSERT_EQ(100, restored.skillBag.skills[0].range[2]);
    ASSERT_EQ(150, restored.skillBag.skills[0].range[3]);
    // Второй скилл: range[0] == 0 → range[1..3] = 0
    ASSERT_EQ(20, restored.skillBag.skills[1].id);
    ASSERT_EQ(0, restored.skillBag.skills[1].range[0]);
    ASSERT_EQ(0, restored.skillBag.skills[1].range[1]);
}

TEST(GameplayPhase3, McSynSkillBagMessage_Roundtrip_Empty) {
    // Пустая сумка скиллов
    McSynSkillBagMessage original;
    original.worldId = 9002;
    original.skillBag.defSkillId = 0;
    original.skillBag.synType = 0;
    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McSynSkillBagMessage restored;
    deserialize(r, restored);
    ASSERT_EQ(original.worldId, restored.worldId);
    ASSERT_EQ(0, restored.skillBag.defSkillId);
    ASSERT_EQ(0u, restored.skillBag.skills.size());
}

// ─── McChaBeginSee / McAddItemCha ──────────────────────────────

TEST(GameplayPhase3Advanced, McChaBeginSeeMessage_Roundtrip) {
    // Базовый roundtrip без позы (poseType == 0)
    McChaBeginSeeMessage original;
    original.seeType = 1;
    original.base.chaId = 42;
    original.base.worldId = 100;
    original.base.name = "TestPirate";
    original.base.motto = "Yarr!";
    original.base.posX = 500;
    original.base.posY = 600;
    original.base.isPlayer = 1;
    // look: не-корабль, все equip id == 0
    original.base.look.synType = 0;
    original.base.look.typeId = 1;
    original.base.look.isBoat = false;
    original.base.look.hairId = 5;
    // equips уже инициализированы нулями
    original.npcType = 0;
    original.npcState = 0;
    original.poseType = 0; // без позы
    // attr/skillState пустые по умолчанию

    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McChaBeginSeeMessage restored;
    deserialize(r, restored);

    ASSERT_EQ(original.seeType, restored.seeType);
    ASSERT_EQ(original.base.chaId, restored.base.chaId);
    ASSERT_EQ(original.base.name, restored.base.name);
    ASSERT_EQ(original.base.posX, restored.base.posX);
    ASSERT_FALSE(restored.base.look.isBoat);
    ASSERT_EQ(5, restored.base.look.hairId);
}

TEST(GameplayPhase3Advanced, McChaBeginSeeMessage_WithLean_Roundtrip) {
    // Roundtrip с наклоном (poseType == 1)
    McChaBeginSeeMessage original;
    original.seeType = 1;
    original.base.name = "LeanPirate";
    original.base.look.typeId = 1;
    original.poseType = 1; // наклон
    original.lean = {1, 2, 90, 100, 200, 50};

    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McChaBeginSeeMessage restored;
    deserialize(r, restored);

    ASSERT_EQ(1, restored.poseType);
    ASSERT_EQ(90, restored.lean.angle);
    ASSERT_EQ(50, restored.lean.height);
}

TEST(GameplayPhase3Advanced, McAddItemChaMessage_Roundtrip) {
    // Roundtrip предметного персонажа с атрибутами и инвентарём
    McAddItemChaMessage original;
    original.mainChaId = 42;
    original.base.name = "ItemCha";
    original.base.look.typeId = 1;
    original.attr.synType = 0;
    original.attr.attrs = {{1, 100}, {2, 200}};
    original.kitbag.synType = 0;
    original.kitbag.capacity = 24;
    // предметов нет, только sentinel
    original.skillState.currentTime = 12345;

    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McAddItemChaMessage restored;
    deserialize(r, restored);

    ASSERT_EQ(42, restored.mainChaId);
    ASSERT_EQ(std::string("ItemCha"), restored.base.name);
    ASSERT_EQ(2u, restored.attr.attrs.size());
    ASSERT_EQ(24, restored.kitbag.capacity);
    ASSERT_EQ(12345, restored.skillState.currentTime);
}

// =================================================================
//  McCharacterAction (CMD_MC_NOTIACTION) — roundtrip тесты
// =================================================================

TEST(McCharacterAction, Move_Roundtrip) {
    McCharacterActionMessage original;
    original.worldId = 1000;
    original.packetId = 42;
    original.actionType = ActionType::MOVE;
    original.move.moveState = 0x01; // не enumMSTATE_ON → пишем stopState
    original.move.stopState = 0x02;
    // 3 waypoint'а по 8 байт = 24 байта
    original.move.waypoints.resize(24);
    for (int i = 0; i < 24; ++i) original.move.waypoints[i] = static_cast<uint8_t>(i + 1);

    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McCharacterActionMessage restored;
    deserialize(r, restored);

    ASSERT_EQ(1000, restored.worldId);
    ASSERT_EQ(42, restored.packetId);
    ASSERT_EQ(ActionType::MOVE, restored.actionType);
    ASSERT_EQ(0x01, restored.move.moveState);
    ASSERT_EQ(0x02, restored.move.stopState);
    ASSERT_EQ(24u, restored.move.waypoints.size());
    ASSERT_EQ(1, restored.move.waypoints[0]);
    ASSERT_EQ(24, restored.move.waypoints[23]);
}

TEST(McCharacterAction, Move_MoveStateOn_NoStopState) {
    McCharacterActionMessage original;
    original.worldId = 1;
    original.packetId = 2;
    original.actionType = ActionType::MOVE;
    original.move.moveState = MSTATE_ON;
    original.move.waypoints = {10, 20, 30, 40, 50, 60, 70, 80};

    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McCharacterActionMessage restored;
    deserialize(r, restored);

    ASSERT_EQ(MSTATE_ON, restored.move.moveState);
    ASSERT_EQ(0, restored.move.stopState);
    ASSERT_EQ(8u, restored.move.waypoints.size());
}

TEST(McCharacterAction, SkillSrc_Roundtrip_TargetType1) {
    McCharacterActionMessage original;
    original.worldId = 500;
    original.packetId = 10;
    original.actionType = ActionType::SKILL_SRC;
    auto& d = original.skillSrc;
    d.fightId = 1; d.angle = 90; d.state = 0x01; d.stopState = 0x02;
    d.skillId = 1001; d.skillSpeed = 200;
    d.targetType = 1; d.targetId = 999; d.targetX = 100; d.targetY = 200;
    d.execTime = 500;
    d.effects = {{5, -30}, {10, 50}};
    d.states = {{3, 2}};

    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McCharacterActionMessage restored;
    deserialize(r, restored);

    ASSERT_EQ(ActionType::SKILL_SRC, restored.actionType);
    ASSERT_EQ(1, restored.skillSrc.fightId);
    ASSERT_EQ(90, restored.skillSrc.angle);
    ASSERT_EQ(0x01, restored.skillSrc.state);
    ASSERT_EQ(0x02, restored.skillSrc.stopState);
    ASSERT_EQ(1001, restored.skillSrc.skillId);
    ASSERT_EQ(1, restored.skillSrc.targetType);
    ASSERT_EQ(999, restored.skillSrc.targetId);
    ASSERT_EQ(100, restored.skillSrc.targetX);
    ASSERT_EQ(2u, restored.skillSrc.effects.size());
    ASSERT_EQ(5, restored.skillSrc.effects[0].attrId);
    ASSERT_EQ(-30, restored.skillSrc.effects[0].attrVal);
    ASSERT_EQ(1u, restored.skillSrc.states.size());
    ASSERT_EQ(3, restored.skillSrc.states[0].stateId);
}

TEST(McCharacterAction, SkillSrc_Roundtrip_StateOn_TargetType2) {
    McCharacterActionMessage original;
    original.worldId = 1;
    original.packetId = 1;
    original.actionType = ActionType::SKILL_SRC;
    auto& d = original.skillSrc;
    d.fightId = 2; d.angle = 45; d.state = FSTATE_ON; d.stopState = 0;
    d.skillId = 100; d.skillSpeed = 100;
    d.targetType = 2; d.targetX = 50; d.targetY = 60;
    d.execTime = 300;

    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McCharacterActionMessage restored;
    deserialize(r, restored);

    ASSERT_EQ(FSTATE_ON, restored.skillSrc.state);
    ASSERT_EQ(0, restored.skillSrc.stopState);
    ASSERT_EQ(2, restored.skillSrc.targetType);
    ASSERT_EQ(0, restored.skillSrc.targetId);
    ASSERT_EQ(50, restored.skillSrc.targetX);
}

TEST(McCharacterAction, SkillTar_Roundtrip_Full) {
    McCharacterActionMessage original;
    original.worldId = 777;
    original.packetId = 55;
    original.actionType = ActionType::SKILL_TAR;
    auto& d = original.skillTar;
    d.fightId = 3; d.state = 0x100; d.doubleAttack = true; d.miss = false;
    d.beatBack = true; d.beatBackX = 150; d.beatBackY = 250;
    d.srcId = 400; d.srcPosX = 10; d.srcPosY = 20;
    d.skillId = 2001; d.skillTargetX = 30; d.skillTargetY = 40;
    d.execTime = 600; d.synType = 2;
    d.effects = {{1, -50}};
    d.hasStates = true; d.stateTime = 99999;
    d.states = {{7, 3, 5000, 10000}};
    d.hasSrcEffect = true; d.srcState = 0x200; d.srcSynType = 2;
    d.srcEffects = {{2, -10}};

    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McCharacterActionMessage restored;
    deserialize(r, restored);

    ASSERT_EQ(ActionType::SKILL_TAR, restored.actionType);
    ASSERT_EQ(3, restored.skillTar.fightId);
    ASSERT_EQ(0x100, restored.skillTar.state);
    ASSERT_TRUE(restored.skillTar.doubleAttack);
    ASSERT_FALSE(restored.skillTar.miss);
    ASSERT_TRUE(restored.skillTar.beatBack);
    ASSERT_EQ(150, restored.skillTar.beatBackX);
    ASSERT_EQ(250, restored.skillTar.beatBackY);
    ASSERT_EQ(400, restored.skillTar.srcId);
    ASSERT_EQ(2001, restored.skillTar.skillId);
    ASSERT_EQ(1u, restored.skillTar.effects.size());
    ASSERT_EQ(1, restored.skillTar.effects[0].attrId);
    ASSERT_TRUE(restored.skillTar.hasStates);
    ASSERT_EQ(99999, restored.skillTar.stateTime);
    ASSERT_EQ(1u, restored.skillTar.states.size());
    ASSERT_EQ(7, restored.skillTar.states[0].stateId);
    ASSERT_EQ(5000, restored.skillTar.states[0].duration);
    ASSERT_TRUE(restored.skillTar.hasSrcEffect);
    ASSERT_EQ(0x200, restored.skillTar.srcState);
    ASSERT_EQ(1u, restored.skillTar.srcEffects.size());
}

TEST(McCharacterAction, SkillTar_Roundtrip_Minimal) {
    McCharacterActionMessage original;
    original.worldId = 1;
    original.packetId = 1;
    original.actionType = ActionType::SKILL_TAR;
    auto& d = original.skillTar;
    d.fightId = 1; d.state = 0; d.doubleAttack = false; d.miss = true;
    d.beatBack = false;
    d.srcId = 10; d.srcPosX = 5; d.srcPosY = 6;
    d.skillId = 100; d.skillTargetX = 7; d.skillTargetY = 8;
    d.execTime = 100;

    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McCharacterActionMessage restored;
    deserialize(r, restored);

    ASSERT_TRUE(restored.skillTar.miss);
    ASSERT_FALSE(restored.skillTar.beatBack);
    ASSERT_FALSE(restored.skillTar.hasStates);
    ASSERT_FALSE(restored.skillTar.hasSrcEffect);
}

TEST(McCharacterAction, Lean_Roundtrip_LeanState0) {
    McCharacterActionMessage original;
    original.worldId = 200;
    original.packetId = 5;
    original.actionType = ActionType::LEAN;
    original.lean = {0, 1, 90, 100, 200, 50};

    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McCharacterActionMessage restored;
    deserialize(r, restored);

    ASSERT_EQ(ActionType::LEAN, restored.actionType);
    ASSERT_EQ(0, restored.lean.leanState);
    ASSERT_EQ(1, restored.lean.pose);
    ASSERT_EQ(90, restored.lean.angle);
    ASSERT_EQ(100, restored.lean.posX);
    ASSERT_EQ(200, restored.lean.posY);
    ASSERT_EQ(50, restored.lean.height);
}

TEST(McCharacterAction, Lean_Roundtrip_LeanStateNonZero) {
    McCharacterActionMessage original;
    original.worldId = 201;
    original.packetId = 6;
    original.actionType = ActionType::LEAN;
    original.lean.leanState = 1;

    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McCharacterActionMessage restored;
    deserialize(r, restored);

    ASSERT_EQ(1, restored.lean.leanState);
    ASSERT_EQ(0, restored.lean.pose);
}

TEST(McCharacterAction, Face_Roundtrip) {
    McCharacterActionMessage original;
    original.worldId = 300;
    original.packetId = 7;
    original.actionType = ActionType::FACE;
    original.face = {180, 2};

    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McCharacterActionMessage restored;
    deserialize(r, restored);

    ASSERT_EQ(ActionType::FACE, restored.actionType);
    ASSERT_EQ(180, restored.face.angle);
    ASSERT_EQ(2, restored.face.pose);
}

TEST(McCharacterAction, SkillPose_Roundtrip) {
    McCharacterActionMessage original;
    original.worldId = 301;
    original.packetId = 8;
    original.actionType = ActionType::SKILL_POSE;
    original.face = {270, 5};

    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McCharacterActionMessage restored;
    deserialize(r, restored);

    ASSERT_EQ(ActionType::SKILL_POSE, restored.actionType);
    ASSERT_EQ(270, restored.face.angle);
    ASSERT_EQ(5, restored.face.pose);
}

TEST(McCharacterAction, ItemFailed_Roundtrip) {
    McCharacterActionMessage original;
    original.worldId = 400;
    original.packetId = 9;
    original.actionType = ActionType::ITEM_FAILED;
    original.itemFailedId = 42;

    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McCharacterActionMessage restored;
    deserialize(r, restored);

    ASSERT_EQ(ActionType::ITEM_FAILED, restored.actionType);
    ASSERT_EQ(42, restored.itemFailedId);
}

TEST(McCharacterAction, Temp_Roundtrip) {
    McCharacterActionMessage original;
    original.worldId = 500;
    original.packetId = 10;
    original.actionType = ActionType::TEMP;
    original.tempItemId = 1001;
    original.tempPartId = 2002;

    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McCharacterActionMessage restored;
    deserialize(r, restored);

    ASSERT_EQ(ActionType::TEMP, restored.actionType);
    ASSERT_EQ(1001, restored.tempItemId);
    ASSERT_EQ(2002, restored.tempPartId);
}

TEST(McCharacterAction, ChangeCha_Roundtrip) {
    McCharacterActionMessage original;
    original.worldId = 600;
    original.packetId = 11;
    original.actionType = ActionType::CHANGE_CHA;
    original.changeChaMainId = 777;

    auto w = serialize(original);
    RPacket r(w.Data(), w.GetPacketSize());
    McCharacterActionMessage restored;
    deserialize(r, restored);

    ASSERT_EQ(ActionType::CHANGE_CHA, restored.actionType);
    ASSERT_EQ(777, restored.changeChaMainId);
}
