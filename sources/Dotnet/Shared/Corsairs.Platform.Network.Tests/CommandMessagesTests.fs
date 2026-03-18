module Corsairs.Platform.Network.Tests.CommandMessagesTests

open Xunit
open Corsairs.Platform.Network.Protocol
open Corsairs.Platform.Network.Protocol.CommandMessages

/// Вспомогательная функция: serialize → RPacket → deserialize
let inline roundtrip (serialize: 'a -> WPacket) (deserialize: IRPacket -> 'b) (msg: 'a) : 'b =
    let mutable w = serialize msg
    use r = new RPacket(w.GetPacketMemory())
    let result = deserialize (r :> IRPacket)
    w.Dispose()
    result

// ═══════════════════════════════════════════════════════════════
//  Группа A: TP/PT — GateServer ↔ GroupServer
// ═══════════════════════════════════════════════════════════════

[<Fact>]
let ``TpLoginRequest roundtrip`` () =
    let original = { TpLoginRequest.ProtocolVersion = 103s; GateName = "Gate1" }
    let restored = roundtrip Serialize.tpLoginRequest Deserialize.tpLoginRequest original
    Assert.Equal(original.ProtocolVersion, restored.ProtocolVersion)
    Assert.Equal(original.GateName, restored.GateName)

[<Fact>]
let ``TpLoginResponse roundtrip`` () =
    let original = { TpLoginResponse.ErrCode = 0s }
    let restored = roundtrip Serialize.tpLoginResponse Deserialize.tpLoginResponse original
    Assert.Equal(original.ErrCode, restored.ErrCode)

[<Fact>]
let ``TpUserLoginRequest roundtrip`` () =
    let original =
        { AcctName = "player1"; AcctPassword = "pass123"; AcctMac = "AA:BB:CC"
          ClientIp = 0x7F000001u; GateAddr = 42u; CheatMarker = 911L }
    let restored = roundtrip Serialize.tpUserLoginRequest Deserialize.tpUserLoginRequest original
    Assert.Equal(original.AcctName, restored.AcctName)
    Assert.Equal(original.AcctPassword, restored.AcctPassword)
    Assert.Equal(original.AcctMac, restored.AcctMac)
    Assert.Equal(original.ClientIp, restored.ClientIp)
    Assert.Equal(original.GateAddr, restored.GateAddr)
    Assert.Equal(original.CheatMarker, restored.CheatMarker)

[<Fact>]
let ``TpUserLoginResponse success roundtrip`` () =
    let data =
        { MaxChaNum = 3L
          Characters = [|
            { Valid = true; ChaName = "Hero"; Job = "Warrior"; Degree = 50L; TypeId = 1L
              EquipIds = Array.init EQUIP_NUM (fun i -> int64 (i + 100)) }
            { Valid = true; ChaName = "Mage"; Job = "Mage"; Degree = 30L; TypeId = 2L
              EquipIds = Array.zeroCreate EQUIP_NUM }
            { Valid = false; ChaName = ""; Job = ""; Degree = 0L; TypeId = 0L
              EquipIds = Array.empty }
          |]
          HasPassword2 = true; AcctId = 100L; AcctLoginId = 200L; GpAddr = 300u }
    let original = TpUserLoginSuccess data
    let restored = roundtrip Serialize.tpUserLoginResponse Deserialize.tpUserLoginResponse original
    match restored with
    | TpUserLoginSuccess d ->
        Assert.Equal(3L, d.MaxChaNum)
        Assert.Equal(3, d.Characters.Length)
        Assert.True(d.Characters[0].Valid)
        Assert.Equal("Hero", d.Characters[0].ChaName)
        Assert.Equal(EQUIP_NUM, d.Characters[0].EquipIds.Length)
        Assert.Equal(100L, d.Characters[0].EquipIds[0])
        Assert.Equal(EQUIP_NUM, d.Characters[1].EquipIds.Length)
        Assert.Equal(0L, d.Characters[1].EquipIds[0])
        Assert.False(d.Characters[2].Valid)
        Assert.True(d.HasPassword2)
        Assert.Equal(100L, d.AcctId)
        Assert.Equal(300u, d.GpAddr)
    | TpUserLoginError _ -> failwith "Expected TpUserLoginSuccess"

[<Fact>]
let ``TpUserLoginResponse error roundtrip`` () =
    let original = TpUserLoginError 521s
    let restored = roundtrip Serialize.tpUserLoginResponse Deserialize.tpUserLoginResponse original
    match restored with
    | TpUserLoginError err -> Assert.Equal(521s, err)
    | TpUserLoginSuccess _ -> failwith "Expected TpUserLoginError"

[<Fact>]
let ``TpUserLogoutRequest roundtrip`` () =
    let original = { TpUserLogoutRequest.GateAddr = 10u; GpAddr = 20u }
    let restored = roundtrip Serialize.tpUserLogoutRequest Deserialize.tpUserLogoutRequest original
    Assert.Equal(original.GateAddr, restored.GateAddr)
    Assert.Equal(original.GpAddr, restored.GpAddr)

[<Fact>]
let ``TpUserLogoutResponse roundtrip`` () =
    let original = { TpUserLogoutResponse.ErrCode = 0s }
    let restored = roundtrip Serialize.tpUserLogoutResponse Deserialize.tpUserLogoutResponse original
    Assert.Equal(original.ErrCode, restored.ErrCode)

[<Fact>]
let ``TpBgnPlayRequest roundtrip`` () =
    let original = { ChaIndex = 2L; GateAddr = 10u; GpAddr = 20u }
    let restored = roundtrip Serialize.tpBgnPlayRequest Deserialize.tpBgnPlayRequest original
    Assert.Equal(original.ChaIndex, restored.ChaIndex)
    Assert.Equal(original.GateAddr, restored.GateAddr)

[<Fact>]
let ``TpBgnPlayResponse success roundtrip`` () =
    let original =
        { TpBgnPlayResponse.ErrCode = 0s
          Data = ValueSome { Password2 = "pin123"; ChaId = 1L; WorldId = 10L; MapName = "garner"; Swiner = 0L } }
    let restored = roundtrip Serialize.tpBgnPlayResponse Deserialize.tpBgnPlayResponse original
    Assert.Equal(0s, restored.ErrCode)
    Assert.Equal("pin123", restored.Data.Value.Password2)
    Assert.Equal("garner", restored.Data.Value.MapName)

[<Fact>]
let ``TpEndPlayRequest roundtrip`` () =
    let original = { TpEndPlayRequest.GateAddr = 5u; GpAddr = 15u }
    let restored = roundtrip Serialize.tpEndPlayRequest Deserialize.tpEndPlayRequest original
    Assert.Equal(original.GateAddr, restored.GateAddr)

[<Fact>]
let ``TpEndPlayResponse success roundtrip`` () =
    let original =
        { TpEndPlayResponse.ErrCode = 0s
          Data = ValueSome
            { MaxChaNum = 2L; ChaNum = 1L
              Characters = [|
                { Valid = true; ChaName = "X"; Job = "J"; Degree = 1L; TypeId = 2L
                  EquipIds = Array.zeroCreate EQUIP_NUM }
                { Valid = false; ChaName = ""; Job = ""; Degree = 0L; TypeId = 0L
                  EquipIds = Array.empty }
              |] } }
    let restored = roundtrip Serialize.tpEndPlayResponse Deserialize.tpEndPlayResponse original
    Assert.Equal(0s, restored.ErrCode)
    Assert.Equal(2L, restored.Data.Value.MaxChaNum)
    Assert.True(restored.Data.Value.Characters[0].Valid)

[<Fact>]
let ``TpNewChaRequest roundtrip`` () =
    let original =
        { ChaName = "NewHero"; Birth = "warrior"; TypeId = 1L; HairId = 3L; FaceId = 5L
          GateAddr = 42u; GpAddr = 100u }
    let restored = roundtrip Serialize.tpNewChaRequest Deserialize.tpNewChaRequest original
    Assert.Equal(original.ChaName, restored.ChaName)
    Assert.Equal(original.Birth, restored.Birth)
    Assert.Equal(original.TypeId, restored.TypeId)
    Assert.Equal(original.GpAddr, restored.GpAddr)

[<Fact>]
let ``TpNewChaResponse roundtrip`` () =
    let original = { TpNewChaResponse.ErrCode = 526s }
    let restored = roundtrip Serialize.tpNewChaResponse Deserialize.tpNewChaResponse original
    Assert.Equal(original.ErrCode, restored.ErrCode)

[<Fact>]
let ``TpDelChaRequest roundtrip`` () =
    let original = { ChaIndex = 1L; Password2 = "pin"; GateAddr = 10u; GpAddr = 20u }
    let restored = roundtrip Serialize.tpDelChaRequest Deserialize.tpDelChaRequest original
    Assert.Equal(original.ChaIndex, restored.ChaIndex)
    Assert.Equal(original.Password2, restored.Password2)

[<Fact>]
let ``TpDelChaResponse roundtrip`` () =
    let original = { TpDelChaResponse.ErrCode = 0s }
    let restored = roundtrip Serialize.tpDelChaResponse Deserialize.tpDelChaResponse original
    Assert.Equal(original.ErrCode, restored.ErrCode)

[<Fact>]
let ``TpChangePassRequest roundtrip`` () =
    let original = { TpChangePassRequest.NewPass = "new123"; Pin = "pin456"; GateAddr = 1u; GpAddr = 2u }
    let restored = roundtrip Serialize.tpChangePassRequest Deserialize.tpChangePassRequest original
    Assert.Equal(original.NewPass, restored.NewPass)
    Assert.Equal(original.Pin, restored.Pin)

[<Fact>]
let ``TpRegisterRequest roundtrip`` () =
    let original = { TpRegisterRequest.Username = "user1"; Password = "pw1"; Email = "a@b.com" }
    let restored = roundtrip Serialize.tpRegisterRequest Deserialize.tpRegisterRequest original
    Assert.Equal(original.Username, restored.Username)
    Assert.Equal(original.Email, restored.Email)

[<Fact>]
let ``TpRegisterResponse roundtrip`` () =
    let original = { TpRegisterResponse.Result = 1L; Message = "OK" }
    let restored = roundtrip Serialize.tpRegisterResponse Deserialize.tpRegisterResponse original
    Assert.Equal(original.Result, restored.Result)
    Assert.Equal(original.Message, restored.Message)

[<Fact>]
let ``TpCreatePassword2Request roundtrip`` () =
    let original = { TpCreatePassword2Request.Password2 = "pw2"; GateAddr = 1u; GpAddr = 2u }
    let restored = roundtrip Serialize.tpCreatePassword2Request Deserialize.tpCreatePassword2Request original
    Assert.Equal(original.Password2, restored.Password2)

[<Fact>]
let ``TpUpdatePassword2Request roundtrip`` () =
    let original = { OldPassword2 = "old"; NewPassword2 = "new"; GateAddr = 1u; GpAddr = 2u }
    let restored = roundtrip Serialize.tpUpdatePassword2Request Deserialize.tpUpdatePassword2Request original
    Assert.Equal(original.OldPassword2, restored.OldPassword2)
    Assert.Equal(original.NewPassword2, restored.NewPassword2)

[<Fact>]
let ``TpPassword2Response roundtrip`` () =
    let original = { TpPassword2Response.ErrCode = 0s }
    let restored = roundtrip Serialize.tpPassword2Response Deserialize.tpPassword2Response original
    Assert.Equal(original.ErrCode, restored.ErrCode)

[<Fact>]
let ``TpSyncPlyLstRequest roundtrip`` () =
    let original =
        { Num = 2L; GateName = "Gate1"
          Players = [|
            { GtAddr = 100L; AcctLoginId = 200L; AcctId = 300L }
            { GtAddr = 400L; AcctLoginId = 500L; AcctId = 600L }
          |] }
    let restored = roundtrip Serialize.tpSyncPlyLstRequest Deserialize.tpSyncPlyLstRequest original
    Assert.Equal(original.Num, restored.Num)
    Assert.Equal(original.GateName, restored.GateName)
    Assert.Equal(2, restored.Players.Length)
    Assert.Equal(100L, restored.Players[0].GtAddr)
    Assert.Equal(600L, restored.Players[1].AcctId)

[<Fact>]
let ``TpSyncPlyLstResponse roundtrip`` () =
    let original =
        { TpSyncPlyLstResponse.ErrCode = 0s; Num = 2L
          Results = [|
            { Ok = true; PlayerPtr = 111L }
            { Ok = false; PlayerPtr = 222L }
          |] }
    let restored = roundtrip Serialize.tpSyncPlyLstResponse Deserialize.tpSyncPlyLstResponse original
    Assert.Equal(0s, restored.ErrCode)
    Assert.Equal(2, restored.Results.Length)
    Assert.True(restored.Results[0].Ok)
    Assert.False(restored.Results[1].Ok)

[<Fact>]
let ``OsLoginRequest roundtrip`` () =
    let original = { OsLoginRequest.Version = 1L; AgentName = "Monitor1" }
    let restored = roundtrip Serialize.osLoginRequest Deserialize.osLoginRequest original
    Assert.Equal(original.Version, restored.Version)
    Assert.Equal(original.AgentName, restored.AgentName)

[<Fact>]
let ``OsLoginResponse roundtrip`` () =
    let original = { OsLoginResponse.ErrCode = 0s }
    let restored = roundtrip Serialize.osLoginResponse Deserialize.osLoginResponse original
    Assert.Equal(original.ErrCode, restored.ErrCode)

[<Fact>]
let ``TpDiscMessage roundtrip`` () =
    let original = { ActId = 42L; IpAddr = 100L; Reason = "timeout" }
    let restored = roundtrip Serialize.tpDiscMessage Deserialize.tpDiscMessage original
    Assert.Equal(original.ActId, restored.ActId)
    Assert.Equal(original.Reason, restored.Reason)

// ═══════════════════════════════════════════════════════════════
//  Группа B: PA/AP — GroupServer ↔ AccountServer
// ═══════════════════════════════════════════════════════════════

[<Fact>]
let ``PaLoginRequest roundtrip`` () =
    let original = { ServerName = "Group1"; ServerPassword = "secret" }
    let restored = roundtrip Serialize.paLoginRequest Deserialize.paLoginRequest original
    Assert.Equal(original.ServerName, restored.ServerName)
    Assert.Equal(original.ServerPassword, restored.ServerPassword)

[<Fact>]
let ``PaLoginResponse roundtrip`` () =
    let original = { PaLoginResponse.ErrCode = 0s }
    let restored = roundtrip Serialize.paLoginResponse Deserialize.paLoginResponse original
    Assert.Equal(original.ErrCode, restored.ErrCode)

[<Fact>]
let ``PaUserLoginRequest roundtrip`` () =
    let original = { Username = "user1"; Password = "pw1"; Mac = "AA:BB"; ClientIp = 100L }
    let restored = roundtrip Serialize.paUserLoginRequest Deserialize.paUserLoginRequest original
    Assert.Equal(original.Username, restored.Username)
    Assert.Equal(original.ClientIp, restored.ClientIp)

[<Fact>]
let ``PaUserLoginResponse success roundtrip`` () =
    let original = PaUserLoginSuccess { AcctLoginId = 42L; SessId = 99L; AcctId = 42L; GmLevel = 0L }
    let restored = roundtrip Serialize.paUserLoginResponse Deserialize.paUserLoginResponse original
    match restored with
    | PaUserLoginSuccess d ->
        Assert.Equal(42L, d.AcctLoginId)
        Assert.Equal(99L, d.SessId)
        Assert.Equal(42L, d.AcctId)
        Assert.Equal(0L, d.GmLevel)
    | PaUserLoginError _ -> failwith "Expected PaUserLoginSuccess"

[<Fact>]
let ``PaUserLoginResponse error roundtrip`` () =
    let original = PaUserLoginError 3s
    let restored = roundtrip Serialize.paUserLoginResponse Deserialize.paUserLoginResponse original
    match restored with
    | PaUserLoginError err -> Assert.Equal(3s, err)
    | PaUserLoginSuccess _ -> failwith "Expected PaUserLoginError"

[<Fact>]
let ``PaUserLogoutMessage roundtrip`` () =
    let original = { PaUserLogoutMessage.AcctLoginId = 42L }
    let restored = roundtrip Serialize.paUserLogoutMessage Deserialize.paUserLogoutMessage original
    Assert.Equal(original.AcctLoginId, restored.AcctLoginId)

[<Fact>]
let ``PaUserBillBgnMessage roundtrip`` () =
    let original = { AcctName = "acct1"; Passport = "pp" }
    let restored = roundtrip Serialize.paUserBillBgnMessage Deserialize.paUserBillBgnMessage original
    Assert.Equal(original.AcctName, restored.AcctName)

[<Fact>]
let ``PaUserBillEndMessage roundtrip`` () =
    let original = { PaUserBillEndMessage.AcctName = "acct1" }
    let restored = roundtrip Serialize.paUserBillEndMessage Deserialize.paUserBillEndMessage original
    Assert.Equal(original.AcctName, restored.AcctName)

[<Fact>]
let ``PaChangePassMessage roundtrip`` () =
    let original = { PaChangePassMessage.Username = "u1"; NewPassword = "new" }
    let restored = roundtrip Serialize.paChangePassMessage Deserialize.paChangePassMessage original
    Assert.Equal(original.Username, restored.Username)
    Assert.Equal(original.NewPassword, restored.NewPassword)

[<Fact>]
let ``PaRegisterMessage roundtrip`` () =
    let original = { PaRegisterMessage.Username = "u"; Password = "p"; Email = "e@e.com" }
    let restored = roundtrip Serialize.paRegisterMessage Deserialize.paRegisterMessage original
    Assert.Equal(original.Username, restored.Username)

[<Fact>]
let ``PaGmBanMessage roundtrip`` () =
    let original = { PaGmBanMessage.ActName = "baduser" }
    let restored = roundtrip Serialize.paGmBanMessage Deserialize.paGmBanMessage original
    Assert.Equal(original.ActName, restored.ActName)

[<Fact>]
let ``PaGmUnbanMessage roundtrip`` () =
    let original = { PaGmUnbanMessage.ActName = "gooduser" }
    let restored = roundtrip Serialize.paGmUnbanMessage Deserialize.paGmUnbanMessage original
    Assert.Equal(original.ActName, restored.ActName)

[<Fact>]
let ``PaUserDisableMessage roundtrip`` () =
    let original = { PaUserDisableMessage.AcctLoginId = 5L; Minutes = 60L }
    let restored = roundtrip Serialize.paUserDisableMessage Deserialize.paUserDisableMessage original
    Assert.Equal(original.AcctLoginId, restored.AcctLoginId)
    Assert.Equal(original.Minutes, restored.Minutes)

[<Fact>]
let ``ApKickUserMessage roundtrip`` () =
    let original = { ApKickUserMessage.ErrCode = 1L; AccountId = 42L }
    let restored = roundtrip Serialize.apKickUserMessage Deserialize.apKickUserMessage original
    Assert.Equal(original.ErrCode, restored.ErrCode)
    Assert.Equal(original.AccountId, restored.AccountId)

[<Fact>]
let ``ApExpScaleMessage roundtrip`` () =
    let original = { ApExpScaleMessage.ChaId = 10L; Time = 3600L }
    let restored = roundtrip Serialize.apExpScaleMessage Deserialize.apExpScaleMessage original
    Assert.Equal(original.ChaId, restored.ChaId)
    Assert.Equal(original.Time, restored.Time)

// ═══════════════════════════════════════════════════════════════
//  Группа C: CP — Client → GroupServer
// ═══════════════════════════════════════════════════════════════

[<Fact>]
let ``CpTeamInviteMessage roundtrip`` () =
    let original = { CpTeamInviteMessage.InvitedName = "friend1"; GateAddr = 10u; GpAddr = 200u }
    let restored = roundtrip Serialize.cpTeamInviteMessage Deserialize.cpTeamInviteMessage original
    Assert.Equal(original.InvitedName, restored.InvitedName)
    Assert.Equal(original.GateAddr, restored.GateAddr)
    Assert.Equal(original.GpAddr, restored.GpAddr)

[<Fact>]
let ``CpTeamAcceptMessage roundtrip`` () =
    let original = { CpTeamAcceptMessage.InviterChaId = 5L; GateAddr = 1u; GpAddr = 2u }
    let restored = roundtrip Serialize.cpTeamAcceptMessage Deserialize.cpTeamAcceptMessage original
    Assert.Equal(original.InviterChaId, restored.InviterChaId)

[<Fact>]
let ``CpTeamLeaveMessage roundtrip`` () =
    let original = { CpTeamLeaveMessage.GateAddr = 1u; GpAddr = 2u }
    let restored = roundtrip Serialize.cpTeamLeaveMessage Deserialize.cpTeamLeaveMessage original
    Assert.Equal(original.GateAddr, restored.GateAddr)

[<Fact>]
let ``CpSay2AllMessage roundtrip`` () =
    let original = { CpSay2AllMessage.Content = "Hello!"; GateAddr = 1u; GpAddr = 2u }
    let restored = roundtrip Serialize.cpSay2AllMessage Deserialize.cpSay2AllMessage original
    Assert.Equal(original.Content, restored.Content)

[<Fact>]
let ``CpSay2YouMessage roundtrip`` () =
    let original = { TargetName = "Bob"; Content = "Hi Bob!"; GateAddr = 1u; GpAddr = 2u }
    let restored = roundtrip Serialize.cpSay2YouMessage Deserialize.cpSay2YouMessage original
    Assert.Equal(original.TargetName, restored.TargetName)
    Assert.Equal(original.Content, restored.Content)

[<Fact>]
let ``CpSessCreateMessage roundtrip`` () =
    let original =
        { ChaNum = 2L; ChaNames = [| "Alice"; "Bob" |]; GateAddr = 1u; GpAddr = 2u }
    let restored = roundtrip Serialize.cpSessCreateMessage Deserialize.cpSessCreateMessage original
    Assert.Equal(original.ChaNum, restored.ChaNum)
    Assert.Equal(2, restored.ChaNames.Length)
    Assert.Equal("Alice", restored.ChaNames[0])
    Assert.Equal("Bob", restored.ChaNames[1])

[<Fact>]
let ``CpChangePersonInfoMessage roundtrip`` () =
    let original = { Motto = "YOLO"; Icon = 3L; RefuseSess = 1L; GateAddr = 1u; GpAddr = 2u }
    let restored = roundtrip Serialize.cpChangePersonInfoMessage Deserialize.cpChangePersonInfoMessage original
    Assert.Equal(original.Motto, restored.Motto)
    Assert.Equal(original.Icon, restored.Icon)

[<Fact>]
let ``CpPingMessage roundtrip`` () =
    let original = { CpPingMessage.PingValue = 12345L; GateAddr = 1u; GpAddr = 2u }
    let restored = roundtrip Serialize.cpPingMessage Deserialize.cpPingMessage original
    Assert.Equal(original.PingValue, restored.PingValue)

[<Fact>]
let ``CpChangePassMessage roundtrip`` () =
    let original = { CpChangePassMessage.NewPass = "np"; Pin = "pp"; GateAddr = 1u; GpAddr = 2u }
    let restored = roundtrip Serialize.cpChangePassMessage Deserialize.cpChangePassMessage original
    Assert.Equal(original.NewPass, restored.NewPass)
    Assert.Equal(original.Pin, restored.Pin)

// ═══════════════════════════════════════════════════════════════
//  Группа D: MP — GameServer → GroupServer
// ═══════════════════════════════════════════════════════════════

[<Fact>]
let ``MpEnterMapMessage roundtrip`` () =
    let original = { MpEnterMapMessage.IsSwitch = 1L; GateAddr = 10u; GpAddr = 20u }
    let restored = roundtrip Serialize.mpEnterMapMessage Deserialize.mpEnterMapMessage original
    Assert.Equal(original.IsSwitch, restored.IsSwitch)
    Assert.Equal(original.GateAddr, restored.GateAddr)

[<Fact>]
let ``MpGuildCreateMessage roundtrip`` () =
    let original =
        { GuildId = 100L; GldName = "Pirates"; Job = "Captain"; Degree = 50L
          GateAddr = 1u; GpAddr = 2u }
    let restored = roundtrip Serialize.mpGuildCreateMessage Deserialize.mpGuildCreateMessage original
    Assert.Equal(original.GuildId, restored.GuildId)
    Assert.Equal(original.GldName, restored.GldName)
    Assert.Equal(original.Job, restored.Job)
    Assert.Equal(original.Degree, restored.Degree)

[<Fact>]
let ``MpMasterCreateMessage roundtrip`` () =
    let original =
        { MpMasterCreateMessage.PrenticeName = "student"; PrenticeChaid = 1L; MasterName = "teacher"; MasterChaid = 2L }
    let restored = roundtrip Serialize.mpMasterCreateMessage Deserialize.mpMasterCreateMessage original
    Assert.Equal(original.PrenticeName, restored.PrenticeName)
    Assert.Equal(original.MasterChaid, restored.MasterChaid)

[<Fact>]
let ``MpSay2AllMessage roundtrip`` () =
    let original =
        { MpSay2AllMessage.Succ = 1L; ChaName = "Hero"; Content = "Hi all!"; GateAddr = 1u; GpAddr = 2u }
    let restored = roundtrip Serialize.mpSay2AllMessage Deserialize.mpSay2AllMessage original
    Assert.Equal(original.Succ, restored.Succ)
    Assert.Equal(original.ChaName, restored.ChaName)
    Assert.Equal(original.Content, restored.Content)

[<Fact>]
let ``MpGuildChallMoneyMessage roundtrip`` () =
    let original =
        { GuildId = 1L; Money = 10000L; GuildName1 = "G1"; GuildName2 = "G2"
          GateAddr = 1u; GpAddr = 2u }
    let restored = roundtrip Serialize.mpGuildChallMoneyMessage Deserialize.mpGuildChallMoneyMessage original
    Assert.Equal(original.GuildName1, restored.GuildName1)
    Assert.Equal(original.Money, restored.Money)

[<Fact>]
let ``MpGarner2UpdateMessage roundtrip`` () =
    let original =
        { Nid = 1L; ChaName = "X"; Level = 50L; Job = "Mage"; Fightpoint = 9999L
          GateAddr = 1u; GpAddr = 2u }
    let restored = roundtrip Serialize.mpGarner2UpdateMessage Deserialize.mpGarner2UpdateMessage original
    Assert.Equal(original.ChaName, restored.ChaName)
    Assert.Equal(original.Level, restored.Level)
    Assert.Equal(original.Fightpoint, restored.Fightpoint)

[<Fact>]
let ``MpGuildNoticeMessage roundtrip`` () =
    let original = { MpGuildNoticeMessage.GuildId = 5L; Content = "Notice text" }
    let restored = roundtrip Serialize.mpGuildNoticeMessage Deserialize.mpGuildNoticeMessage original
    Assert.Equal(original.GuildId, restored.GuildId)
    Assert.Equal(original.Content, restored.Content)

// ═══════════════════════════════════════════════════════════════
//  Группа E: PC — GroupServer → Client
// ═══════════════════════════════════════════════════════════════

[<Fact>]
let ``PcSay2AllMessage roundtrip`` () =
    let original = { PcSay2AllMessage.ChaName = "Hero"; Content = "Hello!"; Color = 0xFF0000L }
    let restored = roundtrip Serialize.pcSay2AllMessage Deserialize.pcSay2AllMessage original
    Assert.Equal(original.ChaName, restored.ChaName)
    Assert.Equal(original.Content, restored.Content)
    Assert.Equal(original.Color, restored.Color)

[<Fact>]
let ``PcSay2YouMessage roundtrip`` () =
    let original = { Sender = "A"; Target = "B"; Content = "msg"; Color = 1L }
    let restored = roundtrip Serialize.pcSay2YouMessage Deserialize.pcSay2YouMessage original
    Assert.Equal(original.Sender, restored.Sender)
    Assert.Equal(original.Target, restored.Target)
    Assert.Equal(original.Content, restored.Content)

[<Fact>]
let ``PcTeamInviteMessage roundtrip`` () =
    let original = { PcTeamInviteMessage.InviterName = "Leader"; ChaId = 1L; Icon = 5L }
    let restored = roundtrip Serialize.pcTeamInviteMessage Deserialize.pcTeamInviteMessage original
    Assert.Equal(original.InviterName, restored.InviterName)
    Assert.Equal(original.ChaId, restored.ChaId)

[<Fact>]
let ``PcTeamRefreshMessage roundtrip`` () =
    let original =
        { PcTeamRefreshMessage.Msg = 1L; Count = 2L
          Members = [|
            { TeamMemberData.ChaId = 1L; ChaName = "A"; Motto = "m1"; Icon = 1L }
            { TeamMemberData.ChaId = 2L; ChaName = "B"; Motto = "m2"; Icon = 2L }
          |] }
    let restored = roundtrip Serialize.pcTeamRefreshMessage Deserialize.pcTeamRefreshMessage original
    Assert.Equal(2L, restored.Count)
    Assert.Equal(2, restored.Members.Length)
    Assert.Equal("A", restored.Members[0].ChaName)
    Assert.Equal("B", restored.Members[1].ChaName)

[<Fact>]
let ``PcFrndRefreshMessage roundtrip`` () =
    let original =
        { PcFrndRefreshMessage.Msg = 2L; Group = "Friends"; ChaId = 1L
          ChaName = "X"; Motto = "hi"; Icon = 3L }
    let restored = roundtrip Serialize.pcFrndRefreshMessage Deserialize.pcFrndRefreshMessage original
    Assert.Equal(original.Group, restored.Group)
    Assert.Equal(original.ChaName, restored.ChaName)

[<Fact>]
let ``PcFrndRefreshInfoMessage roundtrip`` () =
    let original =
        { PcFrndRefreshInfoMessage.ChaId = 1L; Motto = "m"; Icon = 2L
          Degree = 50L; Job = "Mage"; GuildName = "Guild1" }
    let restored = roundtrip Serialize.pcFrndRefreshInfoMessage Deserialize.pcFrndRefreshInfoMessage original
    Assert.Equal(original.ChaId, restored.ChaId)
    Assert.Equal(original.GuildName, restored.GuildName)

[<Fact>]
let ``PcSessCreateMessage roundtrip`` () =
    let original =
        { PcSessCreateMessage.SessId = 1L; NotiPlyCount = 3L
          Members = [|
            { SessMemberData.ChaId = 1L; ChaName = "A"; Motto = "m"; Icon = 1L }
          |] }
    let restored = roundtrip Serialize.pcSessCreateMessage Deserialize.pcSessCreateMessage original
    Assert.Equal(original.SessId, restored.SessId)
    Assert.Equal(1, restored.Members.Length)
    Assert.Equal("A", restored.Members[0].ChaName)
    Assert.Equal(3L, restored.NotiPlyCount)

[<Fact>]
let ``PcErrMsgMessage roundtrip`` () =
    let original = { PcErrMsgMessage.Message = "Error!" }
    let restored = roundtrip Serialize.pcErrMsgMessage Deserialize.pcErrMsgMessage original
    Assert.Equal(original.Message, restored.Message)

[<Fact>]
let ``PcRegisterMessage roundtrip`` () =
    let original = { PcRegisterMessage.Result = 1L; Message = "Success" }
    let restored = roundtrip Serialize.pcRegisterMessage Deserialize.pcRegisterMessage original
    Assert.Equal(original.Result, restored.Result)
    Assert.Equal(original.Message, restored.Message)

// ═══════════════════════════════════════════════════════════════
//  Группа F: PM — GroupServer → GameServer
// ═══════════════════════════════════════════════════════════════

[<Fact>]
let ``PmTeamMessage roundtrip`` () =
    let original =
        { PmTeamMessage.Msg = 1L; Count = 2L
          Members = [|
            { GateName = "Gate1"; GtAddr = 100L; ChaId = 1L }
            { GateName = "Gate2"; GtAddr = 200L; ChaId = 2L }
          |] }
    let restored = roundtrip Serialize.pmTeamMessage Deserialize.pmTeamMessage original
    Assert.Equal(2L, restored.Count)
    Assert.Equal("Gate1", restored.Members[0].GateName)
    Assert.Equal(200L, restored.Members[1].GtAddr)

[<Fact>]
let ``PmExpScaleMessage roundtrip`` () =
    let original = { PmExpScaleMessage.ChaId = 1L; Time = 3600L }
    let restored = roundtrip Serialize.pmExpScaleMessage Deserialize.pmExpScaleMessage original
    Assert.Equal(original.ChaId, restored.ChaId)
    Assert.Equal(original.Time, restored.Time)

[<Fact>]
let ``PmSay2AllMessage roundtrip`` () =
    let original = { PmSay2AllMessage.ChaId = 1L; Content = "msg"; Money = 100L }
    let restored = roundtrip Serialize.pmSay2AllMessage Deserialize.pmSay2AllMessage original
    Assert.Equal(original.ChaId, restored.ChaId)
    Assert.Equal(original.Content, restored.Content)
    Assert.Equal(original.Money, restored.Money)

[<Fact>]
let ``PmGuildDisbandMessage roundtrip`` () =
    let original = { PmGuildDisbandMessage.GuildId = 42L }
    let restored = roundtrip Serialize.pmGuildDisbandMessage Deserialize.pmGuildDisbandMessage original
    Assert.Equal(original.GuildId, restored.GuildId)

[<Fact>]
let ``PmGuildChallMoneyMessage roundtrip`` () =
    let original = { LeaderId = 1L; Money = 5000L; GuildName1 = "A"; GuildName2 = "B" }
    let restored = roundtrip Serialize.pmGuildChallMoneyMessage Deserialize.pmGuildChallMoneyMessage original
    Assert.Equal(original.LeaderId, restored.LeaderId)
    Assert.Equal(original.GuildName1, restored.GuildName1)

[<Fact>]
let ``PtKickUserMessage roundtrip`` () =
    let original = { PtKickUserMessage.GpAddr = 100L; GtAddr = 200L }
    let restored = roundtrip Serialize.ptKickUserMessage Deserialize.ptKickUserMessage original
    Assert.Equal(original.GpAddr, restored.GpAddr)
    Assert.Equal(original.GtAddr, restored.GtAddr)

// ═══════════════════════════════════════════════════════════════
//  Группа D: GateServer → Client (MC)
// ═══════════════════════════════════════════════════════════════

[<Fact>]
let ``McLoginResponse success roundtrip`` () =
    let chars = [|
        { Valid = true; ChaName = "Pirate"; Job = "Warrior"; Degree = 10L; TypeId = 1L
          EquipIds = Array.init EQUIP_NUM (fun i -> int64 (i + 1)) }
        { Valid = false; ChaName = ""; Job = ""; Degree = 0L; TypeId = 0L
          EquipIds = Array.empty }
        { Valid = true; ChaName = "Corsair"; Job = "Mage"; Degree = 25L; TypeId = 3L
          EquipIds = Array.zeroCreate EQUIP_NUM }
    |]
    let original = McLoginSuccess { MaxChaNum = 3L; Characters = chars; HasPassword2 = true }
    let restored = roundtrip Serialize.mcLoginResponse Deserialize.mcLoginResponse original
    match restored with
    | McLoginSuccess data ->
        Assert.Equal(3, data.Characters.Length)
        Assert.True(data.Characters[0].Valid)
        Assert.Equal("Pirate", data.Characters[0].ChaName)
        Assert.Equal("Warrior", data.Characters[0].Job)
        Assert.Equal(10L, data.Characters[0].Degree)
        Assert.Equal(1L, data.Characters[0].TypeId)
        Assert.Equal(EQUIP_NUM, data.Characters[0].EquipIds.Length)
        Assert.Equal(1L, data.Characters[0].EquipIds[0])
        Assert.Equal(34L, data.Characters[0].EquipIds[33])
        Assert.False(data.Characters[1].Valid)
        Assert.True(data.Characters[2].Valid)
        Assert.Equal("Corsair", data.Characters[2].ChaName)
        Assert.Equal(EQUIP_NUM, data.Characters[2].EquipIds.Length)
        Assert.True(data.HasPassword2)
    | McLoginError _ -> Assert.Fail("Expected McLoginSuccess")

[<Fact>]
let ``McLoginResponse error roundtrip`` () =
    let original = McLoginError 1002s
    let restored = roundtrip Serialize.mcLoginResponse Deserialize.mcLoginResponse original
    match restored with
    | McLoginError errCode -> Assert.Equal(1002s, errCode)
    | McLoginSuccess _ -> Assert.Fail("Expected McLoginError")
