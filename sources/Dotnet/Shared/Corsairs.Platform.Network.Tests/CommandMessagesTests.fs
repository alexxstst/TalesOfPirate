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
          ClientIp = 0x7F000001u; GateAddr = 42L; CheatMarker = 911L }
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
          HasPassword2 = true; AcctId = 100L; AcctLoginId = 200L; GpAddr = 300L }
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
        Assert.Equal(300L, d.GpAddr)
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
    let original = { TpUserLogoutRequest.GateAddr = 10L; GpAddr = 20L }
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
    let original = { ChaIndex = 2L; GateAddr = 10L; GpAddr = 20L }
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
    let original = { TpEndPlayRequest.GateAddr = 5L; GpAddr = 15L }
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
          GateAddr = 42L; GpAddr = 100L }
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
    let original = { ChaIndex = 1L; Password2 = "pin"; GateAddr = 10L; GpAddr = 20L }
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
    let original = { TpChangePassRequest.NewPass = "new123"; Pin = "pin456"; GateAddr = 1L; GpAddr = 2L }
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
    let original = { TpCreatePassword2Request.Password2 = "pw2"; GateAddr = 1L; GpAddr = 2L }
    let restored = roundtrip Serialize.tpCreatePassword2Request Deserialize.tpCreatePassword2Request original
    Assert.Equal(original.Password2, restored.Password2)

[<Fact>]
let ``TpUpdatePassword2Request roundtrip`` () =
    let original = { OldPassword2 = "old"; NewPassword2 = "new"; GateAddr = 1L; GpAddr = 2L }
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
    let original = { CpTeamInviteMessage.InvitedName = "friend1"; GateAddr = 10L; GpAddr = 200L }
    let restored = roundtrip Serialize.cpTeamInviteMessage Deserialize.cpTeamInviteMessage original
    Assert.Equal(original.InvitedName, restored.InvitedName)
    Assert.Equal(original.GateAddr, restored.GateAddr)
    Assert.Equal(original.GpAddr, restored.GpAddr)

[<Fact>]
let ``CpTeamAcceptMessage roundtrip`` () =
    let original = { CpTeamAcceptMessage.InviterChaId = 5L; GateAddr = 1L; GpAddr = 2L }
    let restored = roundtrip Serialize.cpTeamAcceptMessage Deserialize.cpTeamAcceptMessage original
    Assert.Equal(original.InviterChaId, restored.InviterChaId)

[<Fact>]
let ``CpTeamLeaveMessage roundtrip`` () =
    let original = { CpTeamLeaveMessage.GateAddr = 1L; GpAddr = 2L }
    let restored = roundtrip Serialize.cpTeamLeaveMessage Deserialize.cpTeamLeaveMessage original
    Assert.Equal(original.GateAddr, restored.GateAddr)

[<Fact>]
let ``CpSay2AllMessage roundtrip`` () =
    let original = { CpSay2AllMessage.Content = "Hello!"; GateAddr = 1L; GpAddr = 2L }
    let restored = roundtrip Serialize.cpSay2AllMessage Deserialize.cpSay2AllMessage original
    Assert.Equal(original.Content, restored.Content)

[<Fact>]
let ``CpSay2YouMessage roundtrip`` () =
    let original = { TargetName = "Bob"; Content = "Hi Bob!"; GateAddr = 1L; GpAddr = 2L }
    let restored = roundtrip Serialize.cpSay2YouMessage Deserialize.cpSay2YouMessage original
    Assert.Equal(original.TargetName, restored.TargetName)
    Assert.Equal(original.Content, restored.Content)

[<Fact>]
let ``CpSessCreateMessage roundtrip`` () =
    let original =
        { ChaNum = 2L; ChaNames = [| "Alice"; "Bob" |]; GateAddr = 1L; GpAddr = 2L }
    let restored = roundtrip Serialize.cpSessCreateMessage Deserialize.cpSessCreateMessage original
    Assert.Equal(original.ChaNum, restored.ChaNum)
    Assert.Equal(2, restored.ChaNames.Length)
    Assert.Equal("Alice", restored.ChaNames[0])
    Assert.Equal("Bob", restored.ChaNames[1])

[<Fact>]
let ``CpChangePersonInfoMessage roundtrip`` () =
    let original = { Motto = "YOLO"; Icon = 3L; RefuseSess = 1L; GateAddr = 1L; GpAddr = 2L }
    let restored = roundtrip Serialize.cpChangePersonInfoMessage Deserialize.cpChangePersonInfoMessage original
    Assert.Equal(original.Motto, restored.Motto)
    Assert.Equal(original.Icon, restored.Icon)

[<Fact>]
let ``CpPingMessage roundtrip`` () =
    let original = { CpPingMessage.PingValue = 12345L; GateAddr = 1L; GpAddr = 2L }
    let restored = roundtrip Serialize.cpPingMessage Deserialize.cpPingMessage original
    Assert.Equal(original.PingValue, restored.PingValue)

[<Fact>]
let ``CpChangePassMessage roundtrip`` () =
    let original = { CpChangePassMessage.NewPass = "np"; Pin = "pp"; GateAddr = 1L; GpAddr = 2L }
    let restored = roundtrip Serialize.cpChangePassMessage Deserialize.cpChangePassMessage original
    Assert.Equal(original.NewPass, restored.NewPass)
    Assert.Equal(original.Pin, restored.Pin)

// ═══════════════════════════════════════════════════════════════
//  Группа D: MP — GameServer → GroupServer
// ═══════════════════════════════════════════════════════════════

[<Fact>]
let ``MpEnterMapMessage roundtrip`` () =
    let original = { MpEnterMapMessage.IsSwitch = 1L; GateAddr = 10L; GpAddr = 20L }
    let restored = roundtrip Serialize.mpEnterMapMessage Deserialize.mpEnterMapMessage original
    Assert.Equal(original.IsSwitch, restored.IsSwitch)
    Assert.Equal(original.GateAddr, restored.GateAddr)

[<Fact>]
let ``MpGuildCreateMessage roundtrip`` () =
    let original =
        { GuildId = 100L; GldName = "Pirates"; Job = "Captain"; Degree = 50L
          GateAddr = 1L; GpAddr = 2L }
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
        { MpSay2AllMessage.Succ = 1L; ChaName = "Hero"; Content = "Hi all!"; GateAddr = 1L; GpAddr = 2L }
    let restored = roundtrip Serialize.mpSay2AllMessage Deserialize.mpSay2AllMessage original
    Assert.Equal(original.Succ, restored.Succ)
    Assert.Equal(original.ChaName, restored.ChaName)
    Assert.Equal(original.Content, restored.Content)

[<Fact>]
let ``MpGuildChallMoneyMessage roundtrip`` () =
    let original =
        { GuildId = 1L; Money = 10000L; GuildName1 = "G1"; GuildName2 = "G2"
          GateAddr = 1L; GpAddr = 2L }
    let restored = roundtrip Serialize.mpGuildChallMoneyMessage Deserialize.mpGuildChallMoneyMessage original
    Assert.Equal(original.GuildName1, restored.GuildName1)
    Assert.Equal(original.Money, restored.Money)

[<Fact>]
let ``MpGarner2UpdateMessage roundtrip`` () =
    let original =
        { Nid = 1L; ChaName = "X"; Level = 50L; Job = "Mage"; Fightpoint = 9999L
          GateAddr = 1L; GpAddr = 2L }
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
let ``PcTeamCancelMessage roundtrip`` () =
    let original = { PcTeamCancelMessage.Reason = 3L; ChaId = 42L }
    let restored = roundtrip Serialize.pcTeamCancelMessage Deserialize.pcTeamCancelMessage original
    Assert.Equal(original.Reason, restored.Reason)
    Assert.Equal(original.ChaId, restored.ChaId)

[<Fact>]
let ``PcFrndRefreshMessage roundtrip`` () =
    let original =
        { PcFrndRefreshMessage.Msg = 2L; Group = "Friends"; ChaId = 1L
          ChaName = "X"; Motto = "hi"; Icon = 3L }
    let restored = roundtrip Serialize.pcFrndRefreshMessage Deserialize.pcFrndRefreshMessage original
    Assert.Equal(original.Group, restored.Group)
    Assert.Equal(original.ChaName, restored.ChaName)

[<Fact>]
let ``PcFrndCancelMessage roundtrip`` () =
    let original = { PcFrndCancelMessage.Reason = 7L; ChaId = 99L }
    let restored = roundtrip Serialize.pcFrndCancelMessage Deserialize.pcFrndCancelMessage original
    Assert.Equal(original.Reason, restored.Reason)
    Assert.Equal(original.ChaId, restored.ChaId)

[<Fact>]
let ``PcFrndRefreshInfoMessage roundtrip`` () =
    let original =
        { PcFrndRefreshInfoMessage.ChaId = 1L; Motto = "m"; Icon = 2L
          Degree = 50L; Job = "Mage"; GuildName = "Guild1" }
    let restored = roundtrip Serialize.pcFrndRefreshInfoMessage Deserialize.pcFrndRefreshInfoMessage original
    Assert.Equal(original.ChaId, restored.ChaId)
    Assert.Equal(original.GuildName, restored.GuildName)

// ═══════════════════════════════════════════════════════════════
//  PcGmInfoMessage — тегированное сообщение PC_GM_INFO
// ═══════════════════════════════════════════════════════════════

[<Fact>]
let ``PcGmInfoMessage START roundtrip`` () =
    let original =
        { PcGmInfoMessage.Type = 1L; ChaId = 0L
          Entries = [|
            { GmFrndEntry.ChaId = 10L; ChaName = "GM_Alpha"; Motto = "Justice"; Icon = 5L; Status = 4L }
            { GmFrndEntry.ChaId = 20L; ChaName = "GM_Beta"; Motto = "Order"; Icon = 6L; Status = 5L }
          |]
          AddEntry = { GmFrndAddEntry.Group = ""; ChaId = 0L; ChaName = ""; Motto = ""; Icon = 0L } }
    let restored = roundtrip Serialize.pcGmInfoMessage Deserialize.pcGmInfoMessage original
    Assert.Equal(original.Type, restored.Type)
    Assert.Equal(original.Entries.Length, restored.Entries.Length)
    Assert.Equal(original.Entries.[0].ChaId, restored.Entries.[0].ChaId)
    Assert.Equal(original.Entries.[0].ChaName, restored.Entries.[0].ChaName)
    Assert.Equal(original.Entries.[0].Motto, restored.Entries.[0].Motto)
    Assert.Equal(original.Entries.[0].Icon, restored.Entries.[0].Icon)
    Assert.Equal(original.Entries.[0].Status, restored.Entries.[0].Status)
    Assert.Equal(original.Entries.[1].ChaId, restored.Entries.[1].ChaId)
    Assert.Equal(original.Entries.[1].ChaName, restored.Entries.[1].ChaName)

[<Fact>]
let ``PcGmInfoMessage ONLINE roundtrip`` () =
    let original =
        { PcGmInfoMessage.Type = 4L; ChaId = 42L; Entries = [||]
          AddEntry = { GmFrndAddEntry.Group = ""; ChaId = 0L; ChaName = ""; Motto = ""; Icon = 0L } }
    let restored = roundtrip Serialize.pcGmInfoMessage Deserialize.pcGmInfoMessage original
    Assert.Equal(original.Type, restored.Type)
    Assert.Equal(original.ChaId, restored.ChaId)

[<Fact>]
let ``PcGmInfoMessage OFFLINE roundtrip`` () =
    let original =
        { PcGmInfoMessage.Type = 5L; ChaId = 77L; Entries = [||]
          AddEntry = { GmFrndAddEntry.Group = ""; ChaId = 0L; ChaName = ""; Motto = ""; Icon = 0L } }
    let restored = roundtrip Serialize.pcGmInfoMessage Deserialize.pcGmInfoMessage original
    Assert.Equal(original.Type, restored.Type)
    Assert.Equal(original.ChaId, restored.ChaId)

[<Fact>]
let ``PcGmInfoMessage DEL roundtrip`` () =
    let original =
        { PcGmInfoMessage.Type = 3L; ChaId = 55L; Entries = [||]
          AddEntry = { GmFrndAddEntry.Group = ""; ChaId = 0L; ChaName = ""; Motto = ""; Icon = 0L } }
    let restored = roundtrip Serialize.pcGmInfoMessage Deserialize.pcGmInfoMessage original
    Assert.Equal(original.Type, restored.Type)
    Assert.Equal(original.ChaId, restored.ChaId)

[<Fact>]
let ``PcGmInfoMessage ADD roundtrip`` () =
    let original =
        { PcGmInfoMessage.Type = 2L; ChaId = 0L; Entries = [||]
          AddEntry = { GmFrndAddEntry.Group = "GM"; ChaId = 99L; ChaName = "NewGM"; Motto = "Hi"; Icon = 8L } }
    let restored = roundtrip Serialize.pcGmInfoMessage Deserialize.pcGmInfoMessage original
    Assert.Equal(original.Type, restored.Type)
    Assert.Equal(original.AddEntry.Group, restored.AddEntry.Group)
    Assert.Equal(original.AddEntry.ChaId, restored.AddEntry.ChaId)
    Assert.Equal(original.AddEntry.ChaName, restored.AddEntry.ChaName)
    Assert.Equal(original.AddEntry.Motto, restored.AddEntry.Motto)
    Assert.Equal(original.AddEntry.Icon, restored.AddEntry.Icon)

// ═══════════════════════════════════════════════════════════════
//  PcFrndRefreshFullMessage — тегированное сообщение PC_FRND_REFRESH
// ═══════════════════════════════════════════════════════════════

[<Fact>]
let ``PcFrndRefreshFullMessage ONLINE roundtrip`` () =
    let original =
        { PcFrndRefreshFullMessage.Type = 4L; ChaId = 33L
          AddEntry = { GmFrndAddEntry.Group = ""; ChaId = 0L; ChaName = ""; Motto = ""; Icon = 0L }
          Self = { FrndSelfData.ChaId = 0L; ChaName = ""; Motto = ""; Icon = 0L }
          Groups = [||] }
    let restored = roundtrip Serialize.pcFrndRefreshFullMessage Deserialize.pcFrndRefreshFullMessage original
    Assert.Equal(original.Type, restored.Type)
    Assert.Equal(original.ChaId, restored.ChaId)

[<Fact>]
let ``PcFrndRefreshFullMessage OFFLINE roundtrip`` () =
    let original =
        { PcFrndRefreshFullMessage.Type = 5L; ChaId = 44L
          AddEntry = { GmFrndAddEntry.Group = ""; ChaId = 0L; ChaName = ""; Motto = ""; Icon = 0L }
          Self = { FrndSelfData.ChaId = 0L; ChaName = ""; Motto = ""; Icon = 0L }
          Groups = [||] }
    let restored = roundtrip Serialize.pcFrndRefreshFullMessage Deserialize.pcFrndRefreshFullMessage original
    Assert.Equal(original.Type, restored.Type)
    Assert.Equal(original.ChaId, restored.ChaId)

[<Fact>]
let ``PcFrndRefreshFullMessage DEL roundtrip`` () =
    let original =
        { PcFrndRefreshFullMessage.Type = 3L; ChaId = 55L
          AddEntry = { GmFrndAddEntry.Group = ""; ChaId = 0L; ChaName = ""; Motto = ""; Icon = 0L }
          Self = { FrndSelfData.ChaId = 0L; ChaName = ""; Motto = ""; Icon = 0L }
          Groups = [||] }
    let restored = roundtrip Serialize.pcFrndRefreshFullMessage Deserialize.pcFrndRefreshFullMessage original
    Assert.Equal(original.Type, restored.Type)
    Assert.Equal(original.ChaId, restored.ChaId)

[<Fact>]
let ``PcFrndRefreshFullMessage ADD roundtrip`` () =
    let original =
        { PcFrndRefreshFullMessage.Type = 2L; ChaId = 0L
          AddEntry = { GmFrndAddEntry.Group = "Friends"; ChaId = 88L; ChaName = "Buddy"; Motto = "Yo"; Icon = 7L }
          Self = { FrndSelfData.ChaId = 0L; ChaName = ""; Motto = ""; Icon = 0L }
          Groups = [||] }
    let restored = roundtrip Serialize.pcFrndRefreshFullMessage Deserialize.pcFrndRefreshFullMessage original
    Assert.Equal(original.Type, restored.Type)
    Assert.Equal(original.AddEntry.Group, restored.AddEntry.Group)
    Assert.Equal(original.AddEntry.ChaId, restored.AddEntry.ChaId)
    Assert.Equal(original.AddEntry.ChaName, restored.AddEntry.ChaName)
    Assert.Equal(original.AddEntry.Motto, restored.AddEntry.Motto)
    Assert.Equal(original.AddEntry.Icon, restored.AddEntry.Icon)

[<Fact>]
let ``PcFrndRefreshFullMessage START roundtrip`` () =
    let original =
        { PcFrndRefreshFullMessage.Type = 1L; ChaId = 0L
          AddEntry = { GmFrndAddEntry.Group = ""; ChaId = 0L; ChaName = ""; Motto = ""; Icon = 0L }
          Self = { FrndSelfData.ChaId = 100L; ChaName = "Me"; Motto = "MyMotto"; Icon = 3L }
          Groups = [|
            { FrndGroupData.GroupName = "BestFriends"
              Members = [|
                { GmFrndEntry.ChaId = 1L; ChaName = "A"; Motto = "ma"; Icon = 1L; Status = 4L }
                { GmFrndEntry.ChaId = 2L; ChaName = "B"; Motto = "mb"; Icon = 2L; Status = 5L }
              |] }
            { FrndGroupData.GroupName = "Others"
              Members = [|
                { GmFrndEntry.ChaId = 3L; ChaName = "C"; Motto = "mc"; Icon = 3L; Status = 4L }
              |] }
          |] }
    let restored = roundtrip Serialize.pcFrndRefreshFullMessage Deserialize.pcFrndRefreshFullMessage original
    Assert.Equal(original.Type, restored.Type)
    Assert.Equal(original.Self.ChaId, restored.Self.ChaId)
    Assert.Equal(original.Self.ChaName, restored.Self.ChaName)
    Assert.Equal(original.Self.Motto, restored.Self.Motto)
    Assert.Equal(original.Self.Icon, restored.Self.Icon)
    Assert.Equal(original.Groups.Length, restored.Groups.Length)
    Assert.Equal(original.Groups.[0].GroupName, restored.Groups.[0].GroupName)
    Assert.Equal(original.Groups.[0].Members.Length, restored.Groups.[0].Members.Length)
    Assert.Equal(original.Groups.[0].Members.[0].ChaId, restored.Groups.[0].Members.[0].ChaId)
    Assert.Equal(original.Groups.[0].Members.[0].ChaName, restored.Groups.[0].Members.[0].ChaName)
    Assert.Equal(original.Groups.[0].Members.[0].Status, restored.Groups.[0].Members.[0].Status)
    Assert.Equal(original.Groups.[0].Members.[1].ChaId, restored.Groups.[0].Members.[1].ChaId)
    Assert.Equal(original.Groups.[1].GroupName, restored.Groups.[1].GroupName)
    Assert.Equal(original.Groups.[1].Members.Length, restored.Groups.[1].Members.Length)
    Assert.Equal(original.Groups.[1].Members.[0].ChaId, restored.Groups.[1].Members.[0].ChaId)

[<Fact>]
let ``PcSessCreateMessage roundtrip`` () =
    let original =
        { PcSessCreateMessage.SessId = 1L; ErrorMsg = ""; NotiPlyCount = 3L
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

// ═══════════════════════════════════════════════════════════════
//  Геймплейные команды: Фаза 1 — простые
// ═══════════════════════════════════════════════════════════════

// ─── Cmd-only (без полей): проверяем, что пакет создаётся корректно ───

[<Fact>]
let ``CmOfflineMode cmd roundtrip`` () =
    let mutable w = Serialize.cmOfflineModeCmd ()
    Assert.True(w.GetPacketSize() > 0)
    w.Dispose()

[<Fact>]
let ``CmCancelExit cmd roundtrip`` () =
    let mutable w = Serialize.cmCancelExitCmd ()
    Assert.True(w.GetPacketSize() > 0)
    w.Dispose()

[<Fact>]
let ``CmKitbagLock cmd roundtrip`` () =
    let mutable w = Serialize.cmKitbagLockCmd ()
    Assert.True(w.GetPacketSize() > 0)
    w.Dispose()

[<Fact>]
let ``CmKitbagCheck cmd roundtrip`` () =
    let mutable w = Serialize.cmKitbagCheckCmd ()
    Assert.True(w.GetPacketSize() > 0)
    w.Dispose()

[<Fact>]
let ``CmKitbagTempSync cmd roundtrip`` () =
    let mutable w = Serialize.cmKitbagTempSyncCmd ()
    Assert.True(w.GetPacketSize() > 0)
    w.Dispose()

[<Fact>]
let ``CmReadBookStart cmd roundtrip`` () =
    let mutable w = Serialize.cmReadBookStartCmd ()
    Assert.True(w.GetPacketSize() > 0)
    w.Dispose()

[<Fact>]
let ``CmReadBookClose cmd roundtrip`` () =
    let mutable w = Serialize.cmReadBookCloseCmd ()
    Assert.True(w.GetPacketSize() > 0)
    w.Dispose()

[<Fact>]
let ``CmBoatCancel cmd roundtrip`` () =
    let mutable w = Serialize.cmBoatCancelCmd ()
    Assert.True(w.GetPacketSize() > 0)
    w.Dispose()

[<Fact>]
let ``CmBoatGetInfo cmd roundtrip`` () =
    let mutable w = Serialize.cmBoatGetInfoCmd ()
    Assert.True(w.GetPacketSize() > 0)
    w.Dispose()

[<Fact>]
let ``CmVolunteerAdd cmd roundtrip`` () =
    let mutable w = Serialize.cmVolunteerAddCmd ()
    Assert.True(w.GetPacketSize() > 0)
    w.Dispose()

[<Fact>]
let ``CmVolunteerDel cmd roundtrip`` () =
    let mutable w = Serialize.cmVolunteerDelCmd ()
    Assert.True(w.GetPacketSize() > 0)
    w.Dispose()

[<Fact>]
let ``CmStoreClose cmd roundtrip`` () =
    let mutable w = Serialize.cmStoreCloseCmd ()
    Assert.True(w.GetPacketSize() > 0)
    w.Dispose()

[<Fact>]
let ``CmStallClose cmd roundtrip`` () =
    let mutable w = Serialize.cmStallCloseCmd ()
    Assert.True(w.GetPacketSize() > 0)
    w.Dispose()

[<Fact>]
let ``CmMisLog cmd roundtrip`` () =
    let mutable w = Serialize.cmMisLogCmd ()
    Assert.True(w.GetPacketSize() > 0)
    w.Dispose()

[<Fact>]
let ``CmDailyBuffRequest cmd roundtrip`` () =
    let mutable w = Serialize.cmDailyBuffRequestCmd ()
    Assert.True(w.GetPacketSize() > 0)
    w.Dispose()

[<Fact>]
let ``CmRequestDropRate cmd roundtrip`` () =
    let mutable w = Serialize.cmRequestDropRateCmd ()
    Assert.True(w.GetPacketSize() > 0)
    w.Dispose()

[<Fact>]
let ``CmRequestExpRate cmd roundtrip`` () =
    let mutable w = Serialize.cmRequestExpRateCmd ()
    Assert.True(w.GetPacketSize() > 0)
    w.Dispose()

// ─── CM типизированные: клиент → GameServer ───

[<Fact>]
let ``CmDieReturnMessage roundtrip`` () =
    let original = { CmDieReturnMessage.ReliveType = 2L }
    let restored = roundtrip Serialize.cmDieReturnMessage Deserialize.cmDieReturnMessage original
    Assert.Equal(original.ReliveType, restored.ReliveType)

[<Fact>]
let ``CmAutoKitbagLockMessage roundtrip`` () =
    let original = { CmAutoKitbagLockMessage.AutoLock = 1L }
    let restored = roundtrip Serialize.cmAutoKitbagLockMessage Deserialize.cmAutoKitbagLockMessage original
    Assert.Equal(original.AutoLock, restored.AutoLock)

[<Fact>]
let ``CmStallSearchMessage roundtrip`` () =
    let original = { CmStallSearchMessage.ItemId = 50301L }
    let restored = roundtrip Serialize.cmStallSearchMessage Deserialize.cmStallSearchMessage original
    Assert.Equal(original.ItemId, restored.ItemId)

[<Fact>]
let ``CmForgeItemMessage roundtrip`` () =
    let original = { CmForgeItemMessage.Index = 7L }
    let restored = roundtrip Serialize.cmForgeItemMessage Deserialize.cmForgeItemMessage original
    Assert.Equal(original.Index, restored.Index)

[<Fact>]
let ``CmEntityEventMessage roundtrip`` () =
    let original = { CmEntityEventMessage.EntityId = 0xFFFF00L }
    let restored = roundtrip Serialize.cmEntityEventMessage Deserialize.cmEntityEventMessage original
    Assert.Equal(original.EntityId, restored.EntityId)

[<Fact>]
let ``CmStallOpenMessage roundtrip`` () =
    let original = { CmStallOpenMessage.CharId = 12345L }
    let restored = roundtrip Serialize.cmStallOpenMessage Deserialize.cmStallOpenMessage original
    Assert.Equal(original.CharId, restored.CharId)

[<Fact>]
let ``CmMisLogInfoMessage roundtrip`` () =
    let original = { CmMisLogInfoMessage.Id = 1001L }
    let restored = roundtrip Serialize.cmMisLogInfoMessage Deserialize.cmMisLogInfoMessage original
    Assert.Equal(original.Id, restored.Id)

[<Fact>]
let ``CmMisLogClearMessage roundtrip`` () =
    let original = { CmMisLogClearMessage.Id = 2002L }
    let restored = roundtrip Serialize.cmMisLogClearMessage Deserialize.cmMisLogClearMessage original
    Assert.Equal(original.Id, restored.Id)

[<Fact>]
let ``CmStoreBuyAskMessage roundtrip`` () =
    let original = { CmStoreBuyAskMessage.ComId = 777L }
    let restored = roundtrip Serialize.cmStoreBuyAskMessage Deserialize.cmStoreBuyAskMessage original
    Assert.Equal(original.ComId, restored.ComId)

[<Fact>]
let ``CmStoreChangeAskMessage roundtrip`` () =
    let original = { CmStoreChangeAskMessage.Num = 5L }
    let restored = roundtrip Serialize.cmStoreChangeAskMessage Deserialize.cmStoreChangeAskMessage original
    Assert.Equal(original.Num, restored.Num)

[<Fact>]
let ``CmStoreQueryMessage roundtrip`` () =
    let original = { CmStoreQueryMessage.Num = 10L }
    let restored = roundtrip Serialize.cmStoreQueryMessage Deserialize.cmStoreQueryMessage original
    Assert.Equal(original.Num, restored.Num)

[<Fact>]
let ``CmTeamFightAnswerMessage roundtrip`` () =
    let original = { CmTeamFightAnswerMessage.Accept = 1L }
    let restored = roundtrip Serialize.cmTeamFightAnswerMessage Deserialize.cmTeamFightAnswerMessage original
    Assert.Equal(original.Accept, restored.Accept)

[<Fact>]
let ``CmItemRepairAnswerMessage roundtrip`` () =
    let original = { CmItemRepairAnswerMessage.Accept = 0L }
    let restored = roundtrip Serialize.cmItemRepairAnswerMessage Deserialize.cmItemRepairAnswerMessage original
    Assert.Equal(original.Accept, restored.Accept)

[<Fact>]
let ``CmItemForgeAnswerMessage roundtrip`` () =
    let original = { CmItemForgeAnswerMessage.Accept = 1L }
    let restored = roundtrip Serialize.cmItemForgeAnswerMessage Deserialize.cmItemForgeAnswerMessage original
    Assert.Equal(original.Accept, restored.Accept)

[<Fact>]
let ``CmItemLotteryAnswerMessage roundtrip`` () =
    let original = { CmItemLotteryAnswerMessage.Accept = 1L }
    let restored = roundtrip Serialize.cmItemLotteryAnswerMessage Deserialize.cmItemLotteryAnswerMessage original
    Assert.Equal(original.Accept, restored.Accept)

[<Fact>]
let ``CmVolunteerOpenMessage roundtrip`` () =
    let original = { CmVolunteerOpenMessage.Num = 42L }
    let restored = roundtrip Serialize.cmVolunteerOpenMessage Deserialize.cmVolunteerOpenMessage original
    Assert.Equal(original.Num, restored.Num)

[<Fact>]
let ``CmVolunteerListMessage roundtrip`` () =
    let original = { CmVolunteerListMessage.Page = 3L; Num = 20L }
    let restored = roundtrip Serialize.cmVolunteerListMessage Deserialize.cmVolunteerListMessage original
    Assert.Equal(original.Page, restored.Page)
    Assert.Equal(original.Num, restored.Num)

[<Fact>]
let ``CmStoreListAskMessage roundtrip`` () =
    let original = { CmStoreListAskMessage.ClsId = 100L; Page = 2L; Num = 15L }
    let restored = roundtrip Serialize.cmStoreListAskMessage Deserialize.cmStoreListAskMessage original
    Assert.Equal(original.ClsId, restored.ClsId)
    Assert.Equal(original.Page, restored.Page)
    Assert.Equal(original.Num, restored.Num)

[<Fact>]
let ``CmCaptainConfirmAsrMessage roundtrip`` () =
    let original = { CmCaptainConfirmAsrMessage.Ret = 1L; TeamId = 9999L }
    let restored = roundtrip Serialize.cmCaptainConfirmAsrMessage Deserialize.cmCaptainConfirmAsrMessage original
    Assert.Equal(original.Ret, restored.Ret)
    Assert.Equal(original.TeamId, restored.TeamId)

[<Fact>]
let ``CmMapMaskMessage roundtrip`` () =
    let original = { CmMapMaskMessage.MapName = "MapForest01" }
    let restored = roundtrip Serialize.cmMapMaskMessage Deserialize.cmMapMaskMessage original
    Assert.Equal(original.MapName, restored.MapName)

[<Fact>]
let ``CmStoreOpenAskMessage roundtrip`` () =
    let original = { CmStoreOpenAskMessage.Password = "test_password_123" }
    let restored = roundtrip Serialize.cmStoreOpenAskMessage Deserialize.cmStoreOpenAskMessage original
    Assert.Equal(original.Password, restored.Password)

[<Fact>]
let ``CmVolunteerSelMessage roundtrip`` () =
    let original = { CmVolunteerSelMessage.Name = "Pirate" }
    let restored = roundtrip Serialize.cmVolunteerSelMessage Deserialize.cmVolunteerSelMessage original
    Assert.Equal(original.Name, restored.Name)

[<Fact>]
let ``CmKitbagUnlockMessage roundtrip`` () =
    let original = { CmKitbagUnlockMessage.Password = "unlock_pass_42" }
    let restored = roundtrip Serialize.cmKitbagUnlockMessage Deserialize.cmKitbagUnlockMessage original
    Assert.Equal(original.Password, restored.Password)

// ─── MC типизированные: GameServer → клиент ───

[<Fact>]
let ``McFailedActionMessage roundtrip`` () =
    let original = { McFailedActionMessage.WorldId = 55555L; ActionType = 2L; Reason = 7L }
    let restored = roundtrip Serialize.mcFailedActionMessage Deserialize.mcFailedActionMessage original
    Assert.Equal(original.WorldId, restored.WorldId)
    Assert.Equal(original.ActionType, restored.ActionType)
    Assert.Equal(original.Reason, restored.Reason)

[<Fact>]
let ``McChaEndSeeMessage roundtrip`` () =
    let original = { McChaEndSeeMessage.SeeType = 3L; WorldId = 88888L }
    let restored = roundtrip Serialize.mcChaEndSeeMessage Deserialize.mcChaEndSeeMessage original
    Assert.Equal(original.SeeType, restored.SeeType)
    Assert.Equal(original.WorldId, restored.WorldId)

[<Fact>]
let ``McItemDestroyMessage roundtrip`` () =
    let original = { McItemDestroyMessage.WorldId = 0xDEADL }
    let restored = roundtrip Serialize.mcItemDestroyMessage Deserialize.mcItemDestroyMessage original
    Assert.Equal(original.WorldId, restored.WorldId)

[<Fact>]
let ``McForgeResultMessage roundtrip`` () =
    let original = { McForgeResultMessage.Result = 1L }
    let restored = roundtrip Serialize.mcForgeResultMessage Deserialize.mcForgeResultMessage original
    Assert.Equal(original.Result, restored.Result)

[<Fact>]
let ``McUniteResultMessage roundtrip`` () =
    let original = { McUniteResultMessage.Result = 2L }
    let restored = roundtrip Serialize.mcUniteResultMessage Deserialize.mcUniteResultMessage original
    Assert.Equal(original.Result, restored.Result)

[<Fact>]
let ``McMillingResultMessage roundtrip`` () =
    let original = { McMillingResultMessage.Result = 3L }
    let restored = roundtrip Serialize.mcMillingResultMessage Deserialize.mcMillingResultMessage original
    Assert.Equal(original.Result, restored.Result)

[<Fact>]
let ``McFusionResultMessage roundtrip`` () =
    let original = { McFusionResultMessage.Result = 0L }
    let restored = roundtrip Serialize.mcFusionResultMessage Deserialize.mcFusionResultMessage original
    Assert.Equal(original.Result, restored.Result)

[<Fact>]
let ``McUpgradeResultMessage roundtrip`` () =
    let original = { McUpgradeResultMessage.Result = 1L }
    let restored = roundtrip Serialize.mcUpgradeResultMessage Deserialize.mcUpgradeResultMessage original
    Assert.Equal(original.Result, restored.Result)

[<Fact>]
let ``McPurifyResultMessage roundtrip`` () =
    let original = { McPurifyResultMessage.Result = 4L }
    let restored = roundtrip Serialize.mcPurifyResultMessage Deserialize.mcPurifyResultMessage original
    Assert.Equal(original.Result, restored.Result)

[<Fact>]
let ``McFixResultMessage roundtrip`` () =
    let original = { McFixResultMessage.Result = 1L }
    let restored = roundtrip Serialize.mcFixResultMessage Deserialize.mcFixResultMessage original
    Assert.Equal(original.Result, restored.Result)

[<Fact>]
let ``McEidolonMetempsychosisMessage roundtrip`` () =
    let original = { McEidolonMetempsychosisMessage.Result = 100L }
    let restored = roundtrip Serialize.mcEidolonMetempsychosisMessage Deserialize.mcEidolonMetempsychosisMessage original
    Assert.Equal(original.Result, restored.Result)

[<Fact>]
let ``McEidolonFusionMessage roundtrip`` () =
    let original = { McEidolonFusionMessage.Result = 77L }
    let restored = roundtrip Serialize.mcEidolonFusionMessage Deserialize.mcEidolonFusionMessage original
    Assert.Equal(original.Result, restored.Result)

[<Fact>]
let ``McMessageMessage roundtrip`` () =
    let original = { McMessageMessage.Text = "Hello World!" }
    let restored = roundtrip Serialize.mcMessageMessage Deserialize.mcMessageMessage original
    Assert.Equal(original.Text, restored.Text)

[<Fact>]
let ``McBickerNoticeMessage roundtrip`` () =
    let original = { McBickerNoticeMessage.Text = "Pirate attack!" }
    let restored = roundtrip Serialize.mcBickerNoticeMessage Deserialize.mcBickerNoticeMessage original
    Assert.Equal(original.Text, restored.Text)

[<Fact>]
let ``McColourNoticeMessage roundtrip`` () =
    let original = { McColourNoticeMessage.Color = 0xFF0000L; Text = "Red alert!" }
    let restored = roundtrip Serialize.mcColourNoticeMessage Deserialize.mcColourNoticeMessage original
    Assert.Equal(original.Color, restored.Color)
    Assert.Equal(original.Text, restored.Text)

[<Fact>]
let ``McTriggerActionMessage roundtrip`` () =
    let original = { McTriggerActionMessage.Type = 3L; Id = 501L; Num = 10L; Count = 5L }
    let restored = roundtrip Serialize.mcTriggerActionMessage Deserialize.mcTriggerActionMessage original
    Assert.Equal(original.Type, restored.Type)
    Assert.Equal(original.Id, restored.Id)
    Assert.Equal(original.Num, restored.Num)
    Assert.Equal(original.Count, restored.Count)

[<Fact>]
let ``McNpcStateChangeMessage roundtrip`` () =
    let original = { McNpcStateChangeMessage.NpcId = 3001L; State = 2L }
    let restored = roundtrip Serialize.mcNpcStateChangeMessage Deserialize.mcNpcStateChangeMessage original
    Assert.Equal(original.NpcId, restored.NpcId)
    Assert.Equal(original.State, restored.State)

[<Fact>]
let ``McEntityStateChangeMessage roundtrip`` () =
    let original = { McEntityStateChangeMessage.EntityId = 7070L; State = 5L }
    let restored = roundtrip Serialize.mcEntityStateChangeMessage Deserialize.mcEntityStateChangeMessage original
    Assert.Equal(original.EntityId, restored.EntityId)
    Assert.Equal(original.State, restored.State)

[<Fact>]
let ``McCloseTalkMessage roundtrip`` () =
    let original = { McCloseTalkMessage.NpcId = 9001L }
    let restored = roundtrip Serialize.mcCloseTalkMessage Deserialize.mcCloseTalkMessage original
    Assert.Equal(original.NpcId, restored.NpcId)

[<Fact>]
let ``McKitbagCheckAnswerMessage roundtrip`` () =
    let original = { McKitbagCheckAnswerMessage.Locked = 1L }
    let restored = roundtrip Serialize.mcKitbagCheckAnswerMessage Deserialize.mcKitbagCheckAnswerMessage original
    Assert.Equal(original.Locked, restored.Locked)

[<Fact>]
let ``McPreMoveTimeMessage roundtrip`` () =
    let original = { McPreMoveTimeMessage.Time = 3500L }
    let restored = roundtrip Serialize.mcPreMoveTimeMessage Deserialize.mcPreMoveTimeMessage original
    Assert.Equal(original.Time, restored.Time)

[<Fact>]
let ``McItemUseSuccMessage roundtrip`` () =
    let original = { McItemUseSuccMessage.WorldId = 40400L; ItemId = 10201L }
    let restored = roundtrip Serialize.mcItemUseSuccMessage Deserialize.mcItemUseSuccMessage original
    Assert.Equal(original.WorldId, restored.WorldId)
    Assert.Equal(original.ItemId, restored.ItemId)

[<Fact>]
let ``McChaPlayEffectMessage roundtrip`` () =
    let original = { McChaPlayEffectMessage.WorldId = 60600L; EffectId = 305L }
    let restored = roundtrip Serialize.mcChaPlayEffectMessage Deserialize.mcChaPlayEffectMessage original
    Assert.Equal(original.WorldId, restored.WorldId)
    Assert.Equal(original.EffectId, restored.EffectId)

[<Fact>]
let ``McSynDefaultSkillMessage roundtrip`` () =
    let original = { McSynDefaultSkillMessage.WorldId = 80800L; SkillId = 1042L }
    let restored = roundtrip Serialize.mcSynDefaultSkillMessage Deserialize.mcSynDefaultSkillMessage original
    Assert.Equal(original.WorldId, restored.WorldId)
    Assert.Equal(original.SkillId, restored.SkillId)

[<Fact>]
let ``McSayMessage roundtrip`` () =
    let original = { McSayMessage.SourceId = 42L; Content = "Ahoy, matey!"; Color = 0x00FF00L }
    let restored = roundtrip Serialize.mcSayMessage Deserialize.mcSayMessage original
    Assert.Equal(original.SourceId, restored.SourceId)
    Assert.Equal(original.Content, restored.Content)
    Assert.Equal(original.Color, restored.Color)

[<Fact>]
let ``McSysInfoMessage roundtrip`` () =
    let original = { McSysInfoMessage.Info = "Server maintenance at 03:00" }
    let restored = roundtrip Serialize.mcSysInfoMessage Deserialize.mcSysInfoMessage original
    Assert.Equal(original.Info, restored.Info)

[<Fact>]
let ``McPopupNoticeMessage roundtrip`` () =
    let original = { McPopupNoticeMessage.Notice = "Double XP event started!" }
    let restored = roundtrip Serialize.mcPopupNoticeMessage Deserialize.mcPopupNoticeMessage original
    Assert.Equal(original.Notice, restored.Notice)

[<Fact>]
let ``McPingMessage roundtrip`` () =
    let original = { McPingMessage.V1 = 11L; V2 = 22L; V3 = 33L; V4 = 44L; V5 = 55L }
    let restored = roundtrip Serialize.mcPingMessage Deserialize.mcPingMessage original
    Assert.Equal(original.V1, restored.V1)
    Assert.Equal(original.V2, restored.V2)
    Assert.Equal(original.V3, restored.V3)
    Assert.Equal(original.V4, restored.V4)
    Assert.Equal(original.V5, restored.V5)

// ═══════════════════════════════════════════════════════════════
//  Геймплейные команды: Фаза 2 — средние
// ═══════════════════════════════════════════════════════════════

// ─── CM — средние (Client → GameServer) ───

[<Fact>]
let ``CmChangePassMessage roundtrip`` () =
    // Смена пароля: пароль + PIN-код
    let original = { CmChangePassMessage.Pass = "NewSecure#42"; Pin = "7890" }
    let restored = roundtrip Serialize.cmChangePassMessage Deserialize.cmChangePassMessage original
    Assert.Equal(original.Pass, restored.Pass)
    Assert.Equal(original.Pin, restored.Pin)

[<Fact>]
let ``CmGuildBankOperMessage roundtrip`` () =
    // Операция с банком гильдии: перемещение предмета между слотами
    let original =
        { CmGuildBankOperMessage.Op = 3L; SrcType = 1L; SrcId = 10L; SrcNum = 5L; TarType = 2L; TarId = 20L }
    let restored = roundtrip Serialize.cmGuildBankOperMessage Deserialize.cmGuildBankOperMessage original
    Assert.Equal(original.Op, restored.Op)
    Assert.Equal(original.SrcType, restored.SrcType)
    Assert.Equal(original.SrcId, restored.SrcId)
    Assert.Equal(original.SrcNum, restored.SrcNum)
    Assert.Equal(original.TarType, restored.TarType)
    Assert.Equal(original.TarId, restored.TarId)

[<Fact>]
let ``CmGuildBankGoldMessage roundtrip`` () =
    // Операция с золотом банка гильдии: вклад/изъятие
    let original = { CmGuildBankGoldMessage.Op = 1L; Direction = 0L; Gold = 50000L }
    let restored = roundtrip Serialize.cmGuildBankGoldMessage Deserialize.cmGuildBankGoldMessage original
    Assert.Equal(original.Op, restored.Op)
    Assert.Equal(original.Direction, restored.Direction)
    Assert.Equal(original.Gold, restored.Gold)

[<Fact>]
let ``CmUpdateHairMessage roundtrip`` () =
    // Смена причёски: ID скрипта + 4 позиции в сетке
    let original =
        { CmUpdateHairMessage.ScriptId = 30100L; GridLoc0 = 0L; GridLoc1 = 1L; GridLoc2 = 2L; GridLoc3 = 3L }
    let restored = roundtrip Serialize.cmUpdateHairMessage Deserialize.cmUpdateHairMessage original
    Assert.Equal(original.ScriptId, restored.ScriptId)
    Assert.Equal(original.GridLoc0, restored.GridLoc0)
    Assert.Equal(original.GridLoc1, restored.GridLoc1)
    Assert.Equal(original.GridLoc2, restored.GridLoc2)
    Assert.Equal(original.GridLoc3, restored.GridLoc3)

[<Fact>]
let ``CmTeamFightAskMessage roundtrip`` () =
    // Запрос командного боя: тип, ID мира, хэндл
    let original = { CmTeamFightAskMessage.Type = 2L; WorldId = 99001L; Handle = 0xABCDL }
    let restored = roundtrip Serialize.cmTeamFightAskMessage Deserialize.cmTeamFightAskMessage original
    Assert.Equal(original.Type, restored.Type)
    Assert.Equal(original.WorldId, restored.WorldId)
    Assert.Equal(original.Handle, restored.Handle)

[<Fact>]
let ``CmItemRepairAskMessage roundtrip`` () =
    // Запрос починки предмета: ID ремонтника, хэндл, тип/ID позиции
    let original = { CmItemRepairAskMessage.RepairmanId = 5001L; RepairmanHandle = 0x1234L; PosType = 1L; PosId = 7L }
    let restored = roundtrip Serialize.cmItemRepairAskMessage Deserialize.cmItemRepairAskMessage original
    Assert.Equal(original.RepairmanId, restored.RepairmanId)
    Assert.Equal(original.RepairmanHandle, restored.RepairmanHandle)
    Assert.Equal(original.PosType, restored.PosType)
    Assert.Equal(original.PosId, restored.PosId)

[<Fact>]
let ``CmRequestTradeMessage roundtrip`` () =
    // Запрос обмена: тип + ID персонажа
    let original = { CmRequestTradeMessage.Type = 1L; CharId = 77001L }
    let restored = roundtrip Serialize.cmRequestTradeMessage Deserialize.cmRequestTradeMessage original
    Assert.Equal(original.Type, restored.Type)
    Assert.Equal(original.CharId, restored.CharId)

[<Fact>]
let ``CmAcceptTradeMessage roundtrip`` () =
    // Принятие обмена
    let original = { CmAcceptTradeMessage.Type = 1L; CharId = 77002L }
    let restored = roundtrip Serialize.cmAcceptTradeMessage Deserialize.cmAcceptTradeMessage original
    Assert.Equal(original.Type, restored.Type)
    Assert.Equal(original.CharId, restored.CharId)

[<Fact>]
let ``CmCancelTradeMessage roundtrip`` () =
    // Отмена обмена
    let original = { CmCancelTradeMessage.Type = 2L; CharId = 77003L }
    let restored = roundtrip Serialize.cmCancelTradeMessage Deserialize.cmCancelTradeMessage original
    Assert.Equal(original.Type, restored.Type)
    Assert.Equal(original.CharId, restored.CharId)

[<Fact>]
let ``CmValidateTradeDataMessage roundtrip`` () =
    // Валидация данных обмена
    let original = { CmValidateTradeDataMessage.Type = 1L; CharId = 77004L }
    let restored = roundtrip Serialize.cmValidateTradeDataMessage Deserialize.cmValidateTradeDataMessage original
    Assert.Equal(original.Type, restored.Type)
    Assert.Equal(original.CharId, restored.CharId)

[<Fact>]
let ``CmValidateTradeMessage roundtrip`` () =
    // Подтверждение обмена
    let original = { CmValidateTradeMessage.Type = 1L; CharId = 77005L }
    let restored = roundtrip Serialize.cmValidateTradeMessage Deserialize.cmValidateTradeMessage original
    Assert.Equal(original.Type, restored.Type)
    Assert.Equal(original.CharId, restored.CharId)

[<Fact>]
let ``CmAddItemMessage roundtrip`` () =
    // Добавление предмета в обмен: все 6 полей
    let original =
        { CmAddItemMessage.Type = 1L; CharId = 88001L; OpType = 2L; Index = 5L; ItemIndex = 10L; Count = 3L }
    let restored = roundtrip Serialize.cmAddItemMessage Deserialize.cmAddItemMessage original
    Assert.Equal(original.Type, restored.Type)
    Assert.Equal(original.CharId, restored.CharId)
    Assert.Equal(original.OpType, restored.OpType)
    Assert.Equal(original.Index, restored.Index)
    Assert.Equal(original.ItemIndex, restored.ItemIndex)
    Assert.Equal(original.Count, restored.Count)

[<Fact>]
let ``CmAddMoneyMessage roundtrip`` () =
    // Добавление золота в обмен
    let original =
        { CmAddMoneyMessage.Type = 1L; CharId = 88002L; OpType = 0L; IsImp = 1L; Money = 999999L }
    let restored = roundtrip Serialize.cmAddMoneyMessage Deserialize.cmAddMoneyMessage original
    Assert.Equal(original.Type, restored.Type)
    Assert.Equal(original.CharId, restored.CharId)
    Assert.Equal(original.OpType, restored.OpType)
    Assert.Equal(original.IsImp, restored.IsImp)
    Assert.Equal(original.Money, restored.Money)

[<Fact>]
let ``CmTigerStartMessage roundtrip`` () =
    // Мини-игра «Тигр»: ID NPC + 3 выбора
    let original = { CmTigerStartMessage.NpcId = 4001L; Sel1 = 0L; Sel2 = 1L; Sel3 = 2L }
    let restored = roundtrip Serialize.cmTigerStartMessage Deserialize.cmTigerStartMessage original
    Assert.Equal(original.NpcId, restored.NpcId)
    Assert.Equal(original.Sel1, restored.Sel1)
    Assert.Equal(original.Sel2, restored.Sel2)
    Assert.Equal(original.Sel3, restored.Sel3)

[<Fact>]
let ``CmTigerStopMessage roundtrip`` () =
    // Остановка мини-игры «Тигр»
    let original = { CmTigerStopMessage.NpcId = 4001L; Num = 5L }
    let restored = roundtrip Serialize.cmTigerStopMessage Deserialize.cmTigerStopMessage original
    Assert.Equal(original.NpcId, restored.NpcId)
    Assert.Equal(original.Num, restored.Num)

[<Fact>]
let ``CmVolunteerAsrMessage roundtrip`` () =
    // Ответ на запрос волонтёра
    let original = { CmVolunteerAsrMessage.Ret = 1L; Name = "VolunteerHero" }
    let restored = roundtrip Serialize.cmVolunteerAsrMessage Deserialize.cmVolunteerAsrMessage original
    Assert.Equal(original.Ret, restored.Ret)
    Assert.Equal(original.Name, restored.Name)

[<Fact>]
let ``CmCreateBoatMessage roundtrip`` () =
    // Создание корабля: имя + 4 компонента
    let original =
        { CmCreateBoatMessage.Boat = "BlackPearl"; Header = 101L; Engine = 202L; Cannon = 303L; Equipment = 404L }
    let restored = roundtrip Serialize.cmCreateBoatMessage Deserialize.cmCreateBoatMessage original
    Assert.Equal(original.Boat, restored.Boat)
    Assert.Equal(original.Header, restored.Header)
    Assert.Equal(original.Engine, restored.Engine)
    Assert.Equal(original.Cannon, restored.Cannon)
    Assert.Equal(original.Equipment, restored.Equipment)

[<Fact>]
let ``CmUpdateBoatMessage roundtrip`` () =
    // Обновление компонентов корабля
    let original = { CmUpdateBoatMessage.Header = 111L; Engine = 222L; Cannon = 333L; Equipment = 444L }
    let restored = roundtrip Serialize.cmUpdateBoatMessage Deserialize.cmUpdateBoatMessage original
    Assert.Equal(original.Header, restored.Header)
    Assert.Equal(original.Engine, restored.Engine)
    Assert.Equal(original.Cannon, restored.Cannon)
    Assert.Equal(original.Equipment, restored.Equipment)

[<Fact>]
let ``CmSelectBoatListMessage roundtrip`` () =
    // Выбор из списка кораблей
    let original = { CmSelectBoatListMessage.NpcId = 5001L; Type = 2L; Index = 0L }
    let restored = roundtrip Serialize.cmSelectBoatListMessage Deserialize.cmSelectBoatListMessage original
    Assert.Equal(original.NpcId, restored.NpcId)
    Assert.Equal(original.Type, restored.Type)
    Assert.Equal(original.Index, restored.Index)

[<Fact>]
let ``CmBoatLaunchMessage roundtrip`` () =
    // Спуск корабля на воду
    let original = { CmBoatLaunchMessage.NpcId = 5001L; Index = 1L }
    let restored = roundtrip Serialize.cmBoatLaunchMessage Deserialize.cmBoatLaunchMessage original
    Assert.Equal(original.NpcId, restored.NpcId)
    Assert.Equal(original.Index, restored.Index)

[<Fact>]
let ``CmBoatBagSelMessage roundtrip`` () =
    // Выбор сумки корабля
    let original = { CmBoatBagSelMessage.NpcId = 5002L; Index = 3L }
    let restored = roundtrip Serialize.cmBoatBagSelMessage Deserialize.cmBoatBagSelMessage original
    Assert.Equal(original.NpcId, restored.NpcId)
    Assert.Equal(original.Index, restored.Index)

[<Fact>]
let ``CmReportWgMessage roundtrip`` () =
    // Репорт о нарушении
    let original = { CmReportWgMessage.Info = "Suspected bot in garner map" }
    let restored = roundtrip Serialize.cmReportWgMessage Deserialize.cmReportWgMessage original
    Assert.Equal(original.Info, restored.Info)

[<Fact>]
let ``CmSay2CampMessage roundtrip`` () =
    // Сообщение в лагерь
    let original = { CmSay2CampMessage.Content = "Camp rally at 20:00!" }
    let restored = roundtrip Serialize.cmSay2CampMessage Deserialize.cmSay2CampMessage original
    Assert.Equal(original.Content, restored.Content)

[<Fact>]
let ``CmStallBuyMessage roundtrip`` () =
    // Покупка из прилавка: ID персонажа, индекс, количество, ID сетки
    let original = { CmStallBuyMessage.CharId = 66001L; Index = 2L; Count = 10L; GridId = 7L }
    let restored = roundtrip Serialize.cmStallBuyMessage Deserialize.cmStallBuyMessage original
    Assert.Equal(original.CharId, restored.CharId)
    Assert.Equal(original.Index, restored.Index)
    Assert.Equal(original.Count, restored.Count)
    Assert.Equal(original.GridId, restored.GridId)

[<Fact>]
let ``CmSkillUpgradeMessage roundtrip`` () =
    // Улучшение навыка
    let original = { CmSkillUpgradeMessage.SkillId = 20101L; AddGrade = 1L }
    let restored = roundtrip Serialize.cmSkillUpgradeMessage Deserialize.cmSkillUpgradeMessage original
    Assert.Equal(original.SkillId, restored.SkillId)
    Assert.Equal(original.AddGrade, restored.AddGrade)

[<Fact>]
let ``CmRefreshDataMessage roundtrip`` () =
    // Запрос обновления данных персонажа
    let original = { CmRefreshDataMessage.WorldId = 44001L; Handle = 0xBEEFL }
    let restored = roundtrip Serialize.cmRefreshDataMessage Deserialize.cmRefreshDataMessage original
    Assert.Equal(original.WorldId, restored.WorldId)
    Assert.Equal(original.Handle, restored.Handle)

[<Fact>]
let ``CmPkCtrlMessage roundtrip`` () =
    // Переключение PK-режима
    let original = { CmPkCtrlMessage.Ctrl = 1L }
    let restored = roundtrip Serialize.cmPkCtrlMessage Deserialize.cmPkCtrlMessage original
    Assert.Equal(original.Ctrl, restored.Ctrl)

[<Fact>]
let ``CmItemAmphitheaterAskMessage roundtrip`` () =
    // Запрос на арену: подтверждение + ID записи
    let original = { CmItemAmphitheaterAskMessage.Sure = 1L; ReId = 9900L }
    let restored = roundtrip Serialize.cmItemAmphitheaterAskMessage Deserialize.cmItemAmphitheaterAskMessage original
    Assert.Equal(original.Sure, restored.Sure)
    Assert.Equal(original.ReId, restored.ReId)

[<Fact>]
let ``CmMasterInviteMessage roundtrip`` () =
    // Приглашение мастера (имя + ID персонажа)
    let original = { CmMasterInviteMessage.Name = "TestMaster"; ChaId = 11001L }
    let restored = roundtrip Serialize.cmMasterInviteMessage Deserialize.cmMasterInviteMessage original
    Assert.Equal(original.Name, restored.Name)
    Assert.Equal(original.ChaId, restored.ChaId)

[<Fact>]
let ``CmMasterAsrMessage roundtrip`` () =
    // Ответ на приглашение мастера
    let original = { CmMasterAsrMessage.Agree = 1L; Name = "TestMaster"; ChaId = 11002L }
    let restored = roundtrip Serialize.cmMasterAsrMessage Deserialize.cmMasterAsrMessage original
    Assert.Equal(original.Agree, restored.Agree)
    Assert.Equal(original.Name, restored.Name)
    Assert.Equal(original.ChaId, restored.ChaId)

[<Fact>]
let ``CmMasterDelMessage roundtrip`` () =
    // Удаление мастера
    let original = { CmMasterDelMessage.Name = "TestMaster"; ChaId = 11003L }
    let restored = roundtrip Serialize.cmMasterDelMessage Deserialize.cmMasterDelMessage original
    Assert.Equal(original.Name, restored.Name)
    Assert.Equal(original.ChaId, restored.ChaId)

[<Fact>]
let ``CmPrenticeInviteMessage roundtrip`` () =
    // Приглашение ученика (имя + ID персонажа)
    let original = { CmPrenticeInviteMessage.Name = "TestPrentice"; ChaId = 22001L }
    let restored = roundtrip Serialize.cmPrenticeInviteMessage Deserialize.cmPrenticeInviteMessage original
    Assert.Equal(original.Name, restored.Name)
    Assert.Equal(original.ChaId, restored.ChaId)

[<Fact>]
let ``CmPrenticeAsrMessage roundtrip`` () =
    // Ответ на приглашение ученика
    let original = { CmPrenticeAsrMessage.Agree = 0L; Name = "TestPrentice"; ChaId = 22002L }
    let restored = roundtrip Serialize.cmPrenticeAsrMessage Deserialize.cmPrenticeAsrMessage original
    Assert.Equal(original.Agree, restored.Agree)
    Assert.Equal(original.Name, restored.Name)
    Assert.Equal(original.ChaId, restored.ChaId)

[<Fact>]
let ``CmPrenticeDelMessage roundtrip`` () =
    // Удаление ученика
    let original = { CmPrenticeDelMessage.Name = "TestPrentice"; ChaId = 22003L }
    let restored = roundtrip Serialize.cmPrenticeDelMessage Deserialize.cmPrenticeDelMessage original
    Assert.Equal(original.Name, restored.Name)
    Assert.Equal(original.ChaId, restored.ChaId)

// ─── NPC Talk составные команды (Client → GameServer) ───

[<Fact>]
let ``CmRequestTalkMessage roundtrip`` () =
    // Запрос диалога с NPC: ID + команда страницы
    let original = { CmRequestTalkMessage.NpcId = 6001L; Cmd = 0L }
    let restored = roundtrip Serialize.cmRequestTalkMessage Deserialize.cmRequestTalkMessage original
    Assert.Equal(original.NpcId, restored.NpcId)
    Assert.Equal(original.Cmd, restored.Cmd)

[<Fact>]
let ``CmSelFunctionMessage roundtrip`` () =
    // Выбор функции в диалоге NPC
    let original = { CmSelFunctionMessage.NpcId = 6002L; PageId = 3L; Index = 1L }
    let restored = roundtrip Serialize.cmSelFunctionMessage Deserialize.cmSelFunctionMessage original
    Assert.Equal(original.NpcId, restored.NpcId)
    Assert.Equal(original.PageId, restored.PageId)
    Assert.Equal(original.Index, restored.Index)

[<Fact>]
let ``CmSaleMessage roundtrip`` () =
    // Продажа предмета NPC: ID NPC, индекс, количество
    let original = { CmSaleMessage.NpcId = 6003L; Index = 4L; Count = 10L }
    let restored = roundtrip Serialize.cmSaleMessage Deserialize.cmSaleMessage original
    Assert.Equal(original.NpcId, restored.NpcId)
    Assert.Equal(original.Index, restored.Index)
    Assert.Equal(original.Count, restored.Count)

[<Fact>]
let ``CmBuyMessage roundtrip`` () =
    // Покупка предмета у NPC: все 5 полей
    let original =
        { CmBuyMessage.NpcId = 6004L; ItemType = 2L; Index1 = 0L; Index2 = 5L; Count = 3L }
    let restored = roundtrip Serialize.cmBuyMessage Deserialize.cmBuyMessage original
    Assert.Equal(original.NpcId, restored.NpcId)
    Assert.Equal(original.ItemType, restored.ItemType)
    Assert.Equal(original.Index1, restored.Index1)
    Assert.Equal(original.Index2, restored.Index2)
    Assert.Equal(original.Count, restored.Count)

[<Fact>]
let ``CmMissionPageMessage roundtrip`` () =
    // Навигация по страницам квестов NPC
    let original = { CmMissionPageMessage.NpcId = 6005L; Cmd = 1L; SelItem = 2L; Param = 100L }
    let restored = roundtrip Serialize.cmMissionPageMessage Deserialize.cmMissionPageMessage original
    Assert.Equal(original.NpcId, restored.NpcId)
    Assert.Equal(original.Cmd, restored.Cmd)
    Assert.Equal(original.SelItem, restored.SelItem)
    Assert.Equal(original.Param, restored.Param)

[<Fact>]
let ``CmSelMissionMessage roundtrip`` () =
    // Выбор квеста в списке NPC
    let original = { CmSelMissionMessage.NpcId = 6006L; Index = 2L }
    let restored = roundtrip Serialize.cmSelMissionMessage Deserialize.cmSelMissionMessage original
    Assert.Equal(original.NpcId, restored.NpcId)
    Assert.Equal(original.Index, restored.Index)

[<Fact>]
let ``CmBlackMarketExchangeReqMessage roundtrip`` () =
    // Обмен на чёрном рынке: все 7 полей
    let original =
        { CmBlackMarketExchangeReqMessage.NpcId = 6007L; TimeNum = 3L; SrcId = 50001L
          SrcNum = 10L; TarId = 50002L; TarNum = 5L; Index = 0L }
    let restored = roundtrip Serialize.cmBlackMarketExchangeReqMessage Deserialize.cmBlackMarketExchangeReqMessage original
    Assert.Equal(original.NpcId, restored.NpcId)
    Assert.Equal(original.TimeNum, restored.TimeNum)
    Assert.Equal(original.SrcId, restored.SrcId)
    Assert.Equal(original.SrcNum, restored.SrcNum)
    Assert.Equal(original.TarId, restored.TarId)
    Assert.Equal(original.TarNum, restored.TarNum)
    Assert.Equal(original.Index, restored.Index)

// ─── MC — средние (GameServer → клиент) ───

[<Fact>]
let ``McSay2CampMessage roundtrip`` () =
    // Сообщение в лагерь (сервер → клиент)
    let original = { McSay2CampMessage.ChaName = "CaptainJack"; Content = "All hands on deck!" }
    let restored = roundtrip Serialize.mcSay2CampMessage Deserialize.mcSay2CampMessage original
    Assert.Equal(original.ChaName, restored.ChaName)
    Assert.Equal(original.Content, restored.Content)

[<Fact>]
let ``McTalkInfoMessage roundtrip`` () =
    // Информация диалога NPC: ID, команда, текст
    let original = { McTalkInfoMessage.NpcId = 7001L; Cmd = 2L; Text = "Welcome, adventurer!" }
    let restored = roundtrip Serialize.mcTalkInfoMessage Deserialize.mcTalkInfoMessage original
    Assert.Equal(original.NpcId, restored.NpcId)
    Assert.Equal(original.Cmd, restored.Cmd)
    Assert.Equal(original.Text, restored.Text)

[<Fact>]
let ``McTradeDataMessage roundtrip`` () =
    // Данные торговли NPC: все 6 полей
    let original =
        { McTradeDataMessage.NpcId = 7002L; Page = 1L; Index = 3L; ItemId = 40100L; Count = 99L; Price = 1500L }
    let restored = roundtrip Serialize.mcTradeDataMessage Deserialize.mcTradeDataMessage original
    Assert.Equal(original.NpcId, restored.NpcId)
    Assert.Equal(original.Page, restored.Page)
    Assert.Equal(original.Index, restored.Index)
    Assert.Equal(original.ItemId, restored.ItemId)
    Assert.Equal(original.Count, restored.Count)
    Assert.Equal(original.Price, restored.Price)

[<Fact>]
let ``McTradeResultMessage roundtrip`` () =
    // Результат торговли: тип, индекс, количество, ID предмета, деньги
    let original =
        { McTradeResultMessage.Type = 1L; Index = 2L; Count = 5L; ItemId = 40200L; Money = 7500L }
    let restored = roundtrip Serialize.mcTradeResultMessage Deserialize.mcTradeResultMessage original
    Assert.Equal(original.Type, restored.Type)
    Assert.Equal(original.Index, restored.Index)
    Assert.Equal(original.Count, restored.Count)
    Assert.Equal(original.ItemId, restored.ItemId)
    Assert.Equal(original.Money, restored.Money)

[<Fact>]
let ``McUpdateHairResMessage roundtrip`` () =
    // Результат смены причёски: ID мира, ID скрипта, причина
    let original = { McUpdateHairResMessage.WorldId = 80001L; ScriptId = 30100L; Reason = "Success" }
    let restored = roundtrip Serialize.mcUpdateHairResMessage Deserialize.mcUpdateHairResMessage original
    Assert.Equal(original.WorldId, restored.WorldId)
    Assert.Equal(original.ScriptId, restored.ScriptId)
    Assert.Equal(original.Reason, restored.Reason)

[<Fact>]
let ``McSynTLeaderIdMessage roundtrip`` () =
    // Синхронизация ID лидера команды
    let original = { McSynTLeaderIdMessage.WorldId = 80002L; LeaderId = 33001L }
    let restored = roundtrip Serialize.mcSynTLeaderIdMessage Deserialize.mcSynTLeaderIdMessage original
    Assert.Equal(original.WorldId, restored.WorldId)
    Assert.Equal(original.LeaderId, restored.LeaderId)

[<Fact>]
let ``McKitbagCapacityMessage roundtrip`` () =
    // Синхронизация ёмкости инвентаря
    let original = { McKitbagCapacityMessage.WorldId = 80003L; Capacity = 48L }
    let restored = roundtrip Serialize.mcKitbagCapacityMessage Deserialize.mcKitbagCapacityMessage original
    Assert.Equal(original.WorldId, restored.WorldId)
    Assert.Equal(original.Capacity, restored.Capacity)

[<Fact>]
let ``McItemForgeAskMessage roundtrip`` () =
    // Запрос ковки: тип + стоимость
    let original = { McItemForgeAskMessage.Type = 3L; Money = 25000L }
    let restored = roundtrip Serialize.mcItemForgeAskMessage Deserialize.mcItemForgeAskMessage original
    Assert.Equal(original.Type, restored.Type)
    Assert.Equal(original.Money, restored.Money)

[<Fact>]
let ``McItemForgeAnswerMessage roundtrip`` () =
    // Результат ковки: ID мира, тип, результат
    let original = { McItemForgeAnswerMessage.WorldId = 80004L; Type = 2L; Result = 1L }
    let restored = roundtrip Serialize.mcItemForgeAnswerMessage Deserialize.mcItemForgeAnswerMessage original
    Assert.Equal(original.WorldId, restored.WorldId)
    Assert.Equal(original.Type, restored.Type)
    Assert.Equal(original.Result, restored.Result)

[<Fact>]
let ``McQueryChaMessage roundtrip`` () =
    // Запрос информации о персонаже: 6 полей
    let original =
        { McQueryChaMessage.ChaId = 90001L; Name = "SwordMaster"; MapName = "garner"
          PosX = 1234L; PosY = 5678L; ChaId2 = 90002L }
    let restored = roundtrip Serialize.mcQueryChaMessage Deserialize.mcQueryChaMessage original
    Assert.Equal(original.ChaId, restored.ChaId)
    Assert.Equal(original.Name, restored.Name)
    Assert.Equal(original.MapName, restored.MapName)
    Assert.Equal(original.PosX, restored.PosX)
    Assert.Equal(original.PosY, restored.PosY)
    Assert.Equal(original.ChaId2, restored.ChaId2)

[<Fact>]
let ``McQueryChaPingMessage roundtrip`` () =
    // Запрос пинга персонажа
    let original = { McQueryChaPingMessage.ChaId = 90003L; Name = "SpeedRunner"; MapName = "darkswamp"; Ping = 42L }
    let restored = roundtrip Serialize.mcQueryChaPingMessage Deserialize.mcQueryChaPingMessage original
    Assert.Equal(original.ChaId, restored.ChaId)
    Assert.Equal(original.Name, restored.Name)
    Assert.Equal(original.MapName, restored.MapName)
    Assert.Equal(original.Ping, restored.Ping)

[<Fact>]
let ``McQueryReliveMessage roundtrip`` () =
    // Запрос воскрешения: ID персонажа, имя источника, тип
    let original = { McQueryReliveMessage.ChaId = 90004L; SourceName = "Healer"; ReliveType = 2L }
    let restored = roundtrip Serialize.mcQueryReliveMessage Deserialize.mcQueryReliveMessage original
    Assert.Equal(original.ChaId, restored.ChaId)
    Assert.Equal(original.SourceName, restored.SourceName)
    Assert.Equal(original.ReliveType, restored.ReliveType)

[<Fact>]
let ``McGmMailMessage roundtrip`` () =
    // GM-почта: заголовок, содержимое, время
    let original = { McGmMailMessage.Title = "Server Notice"; Content = "Maintenance at 03:00 UTC"; Time = 1711500000L }
    let restored = roundtrip Serialize.mcGmMailMessage Deserialize.mcGmMailMessage original
    Assert.Equal(original.Title, restored.Title)
    Assert.Equal(original.Content, restored.Content)
    Assert.Equal(original.Time, restored.Time)

[<Fact>]
let ``McSynStallNameMessage roundtrip`` () =
    // Синхронизация имени прилавка: ID персонажа + название
    let original = { McSynStallNameMessage.CharId = 12345L; Name = "Best Potions Shop" }
    let restored = roundtrip Serialize.mcSynStallNameMessage Deserialize.mcSynStallNameMessage original
    Assert.Equal(original.CharId, restored.CharId)
    Assert.Equal(original.Name, restored.Name)

[<Fact>]
let ``McMapCrashMessage roundtrip`` () =
    // Уведомление о крахе карты
    let original = { McMapCrashMessage.Text = "Map instance garner_02 crashed" }
    let restored = roundtrip Serialize.mcMapCrashMessage Deserialize.mcMapCrashMessage original
    Assert.Equal(original.Text, restored.Text)

[<Fact>]
let ``McVolunteerStateMessage roundtrip`` () =
    // Статус волонтёра
    let original = { McVolunteerStateMessage.State = 2L }
    let restored = roundtrip Serialize.mcVolunteerStateMessage Deserialize.mcVolunteerStateMessage original
    Assert.Equal(original.State, restored.State)

[<Fact>]
let ``McVolunteerAskMessage roundtrip`` () =
    // Запрос от волонтёра: имя
    let original = { McVolunteerAskMessage.Name = "HelpfulPirate" }
    let restored = roundtrip Serialize.mcVolunteerAskMessage Deserialize.mcVolunteerAskMessage original
    Assert.Equal(original.Name, restored.Name)

[<Fact>]
let ``McMasterAskMessage roundtrip`` () =
    // Запрос от мастера: имя + ID
    let original = { McMasterAskMessage.Name = "GrandMaster"; ChaId = 55001L }
    let restored = roundtrip Serialize.mcMasterAskMessage Deserialize.mcMasterAskMessage original
    Assert.Equal(original.Name, restored.Name)
    Assert.Equal(original.ChaId, restored.ChaId)

[<Fact>]
let ``McPrenticeAskMessage roundtrip`` () =
    // Запрос от ученика: имя + ID
    let original = { McPrenticeAskMessage.Name = "YoungApprentice"; ChaId = 55002L }
    let restored = roundtrip Serialize.mcPrenticeAskMessage Deserialize.mcPrenticeAskMessage original
    Assert.Equal(original.Name, restored.Name)
    Assert.Equal(original.ChaId, restored.ChaId)

[<Fact>]
let ``McItemRepairAskMessage roundtrip`` () =
    // Запрос починки: имя предмета + стоимость
    let original = { McItemRepairAskMessage.ItemName = "Dragon Sword +7"; RepairCost = 35000L }
    let restored = roundtrip Serialize.mcItemRepairAskMessage Deserialize.mcItemRepairAskMessage original
    Assert.Equal(original.ItemName, restored.ItemName)
    Assert.Equal(original.RepairCost, restored.RepairCost)

[<Fact>]
let ``McItemLotteryAsrMessage roundtrip`` () =
    // Результат лотереи предметов
    let original = { McItemLotteryAsrMessage.Success = 1L }
    let restored = roundtrip Serialize.mcItemLotteryAsrMessage Deserialize.mcItemLotteryAsrMessage original
    Assert.Equal(original.Success, restored.Success)

// ═══════════════════════════════════════════════════════════════
//  Геймплейные команды: Фаза 4 — простые MC/CM
// ═══════════════════════════════════════════════════════════════

[<Fact>]
let ``McChaEmotionMessage roundtrip`` () =
    let original = { McChaEmotionMessage.WorldId = 42L; Emotion = 7L }
    let restored = roundtrip Serialize.mcChaEmotionMessage Deserialize.mcChaEmotionMessage original
    Assert.Equal(original.WorldId, restored.WorldId)
    Assert.Equal(original.Emotion, restored.Emotion)

[<Fact>]
let ``McStartExitMessage roundtrip`` () =
    let original = { McStartExitMessage.ExitTime = 10000L }
    let restored = roundtrip Serialize.mcStartExitMessage Deserialize.mcStartExitMessage original
    Assert.Equal(original.ExitTime, restored.ExitTime)

[<Fact>]
let ``McGmRecvMessage roundtrip`` () =
    let original = { McGmRecvMessage.NpcId = 555L }
    let restored = roundtrip Serialize.mcGmRecvMessage Deserialize.mcGmRecvMessage original
    Assert.Equal(original.NpcId, restored.NpcId)

[<Fact>]
let ``McStallDelGoodsMessage roundtrip`` () =
    let original = { McStallDelGoodsMessage.CharId = 100L; Grid = 5L; Count = 3L }
    let restored = roundtrip Serialize.mcStallDelGoodsMessage Deserialize.mcStallDelGoodsMessage original
    Assert.Equal(original.CharId, restored.CharId)
    Assert.Equal(original.Grid, restored.Grid)
    Assert.Equal(original.Count, restored.Count)

[<Fact>]
let ``McStallCloseMessage roundtrip`` () =
    let original = { McStallCloseMessage.CharId = 200L }
    let restored = roundtrip Serialize.mcStallCloseMessage Deserialize.mcStallCloseMessage original
    Assert.Equal(original.CharId, restored.CharId)

[<Fact>]
let ``McStallSuccessMessage roundtrip`` () =
    let original = { McStallSuccessMessage.CharId = 300L }
    let restored = roundtrip Serialize.mcStallSuccessMessage Deserialize.mcStallSuccessMessage original
    Assert.Equal(original.CharId, restored.CharId)

[<Fact>]
let ``McUpdateGuildGoldMessage roundtrip`` () =
    let original = { McUpdateGuildGoldMessage.Data = "12345678" }
    let restored = roundtrip Serialize.mcUpdateGuildGoldMessage Deserialize.mcUpdateGuildGoldMessage original
    Assert.Equal(original.Data, restored.Data)

[<Fact>]
let ``McQueryChaItemMessage roundtrip`` () =
    let original = { McQueryChaItemMessage.ChaId = 9001L }
    let restored = roundtrip Serialize.mcQueryChaItemMessage Deserialize.mcQueryChaItemMessage original
    Assert.Equal(original.ChaId, restored.ChaId)

[<Fact>]
let ``McDisconnectMessage roundtrip`` () =
    let original = { McDisconnectMessage.Reason = 3L }
    let restored = roundtrip Serialize.mcDisconnectMessage Deserialize.mcDisconnectMessage original
    Assert.Equal(original.Reason, restored.Reason)

[<Fact>]
let ``McLifeSkillShowMessage roundtrip`` () =
    let original = { McLifeSkillShowMessage.Type = 2L }
    let restored = roundtrip Serialize.mcLifeSkillShowMessage Deserialize.mcLifeSkillShowMessage original
    Assert.Equal(original.Type, restored.Type)

[<Fact>]
let ``McLifeSkillMessage roundtrip`` () =
    let original = { McLifeSkillMessage.Type = 1L; Result = 0L; Text = "iron_ore,3,5" }
    let restored = roundtrip Serialize.mcLifeSkillMessage Deserialize.mcLifeSkillMessage original
    Assert.Equal(original.Type, restored.Type)
    Assert.Equal(original.Result, restored.Result)
    Assert.Equal(original.Text, restored.Text)

[<Fact>]
let ``McLifeSkillAsrMessage roundtrip`` () =
    let original = { McLifeSkillAsrMessage.Type = 0L; Time = 5000L; Text = "composing..." }
    let restored = roundtrip Serialize.mcLifeSkillAsrMessage Deserialize.mcLifeSkillAsrMessage original
    Assert.Equal(original.Type, restored.Type)
    Assert.Equal(original.Time, restored.Time)
    Assert.Equal(original.Text, restored.Text)

[<Fact>]
let ``McDropLockAsrMessage roundtrip`` () =
    let original = { McDropLockAsrMessage.Success = 1L }
    let restored = roundtrip Serialize.mcDropLockAsrMessage Deserialize.mcDropLockAsrMessage original
    Assert.Equal(original.Success, restored.Success)

[<Fact>]
let ``McUnlockItemAsrMessage roundtrip`` () =
    let original = { McUnlockItemAsrMessage.Result = 2L }
    let restored = roundtrip Serialize.mcUnlockItemAsrMessage Deserialize.mcUnlockItemAsrMessage original
    Assert.Equal(original.Result, restored.Result)

[<Fact>]
let ``McStoreBuyAnswerMessage roundtrip`` () =
    let original = { McStoreBuyAnswerMessage.Success = 1L; NewMoney = 50000L }
    let restored = roundtrip Serialize.mcStoreBuyAnswerMessage Deserialize.mcStoreBuyAnswerMessage original
    Assert.Equal(original.Success, restored.Success)
    Assert.Equal(original.NewMoney, restored.NewMoney)

[<Fact>]
let ``McStoreChangeAnswerMessage roundtrip`` () =
    let original = { McStoreChangeAnswerMessage.Success = 1L; MoBean = 100L; ReplMoney = 9999L }
    let restored = roundtrip Serialize.mcStoreChangeAnswerMessage Deserialize.mcStoreChangeAnswerMessage original
    Assert.Equal(original.Success, restored.Success)
    Assert.Equal(original.MoBean, restored.MoBean)
    Assert.Equal(original.ReplMoney, restored.ReplMoney)

[<Fact>]
let ``McDailyBuffInfoMessage roundtrip`` () =
    let original = { McDailyBuffInfoMessage.ImgName = "buff_icon.png"; LabelInfo = "Daily EXP Boost +50%" }
    let restored = roundtrip Serialize.mcDailyBuffInfoMessage Deserialize.mcDailyBuffInfoMessage original
    Assert.Equal(original.ImgName, restored.ImgName)
    Assert.Equal(original.LabelInfo, restored.LabelInfo)

[<Fact>]
let ``McRequestDropRateMessage roundtrip`` () =
    let original = { McRequestDropRateMessage.Rate = 1.5f }
    let restored = roundtrip Serialize.mcRequestDropRateMessage Deserialize.mcRequestDropRateMessage original
    Assert.Equal(original.Rate, restored.Rate)

[<Fact>]
let ``McRequestExpRateMessage roundtrip`` () =
    let original = { McRequestExpRateMessage.Rate = 2.0f }
    let restored = roundtrip Serialize.mcRequestExpRateMessage Deserialize.mcRequestExpRateMessage original
    Assert.Equal(original.Rate, restored.Rate)

[<Fact>]
let ``McTigerItemIdMessage roundtrip`` () =
    let original = { McTigerItemIdMessage.Num = 3L; ItemId0 = 1001L; ItemId1 = 1002L; ItemId2 = 1003L }
    let restored = roundtrip Serialize.mcTigerItemIdMessage Deserialize.mcTigerItemIdMessage original
    Assert.Equal(original.Num, restored.Num)
    Assert.Equal(original.ItemId0, restored.ItemId0)
    Assert.Equal(original.ItemId1, restored.ItemId1)
    Assert.Equal(original.ItemId2, restored.ItemId2)

// ═══════════════════════════════════════════════════════════════
//  Геймплейные команды: Фаза 3 — сложные
// ═══════════════════════════════════════════════════════════════

[<Fact>]
let ``McSynAttributeMessage roundtrip`` () =
    // Синхронизация атрибутов персонажа: тип + массив пар (id, value)
    let original =
        { McSynAttributeMessage.WorldId = 42L
          Attr = { SynType = 1L
                   Attrs = [| { AttrId = 10L; AttrVal = 100L }
                              { AttrId = 20L; AttrVal = 200L }
                              { AttrId = 30L; AttrVal = 300L } |] } }
    let restored = roundtrip Serialize.mcSynAttributeMessage Deserialize.mcSynAttributeMessage original
    Assert.Equal(original.WorldId, restored.WorldId)
    Assert.Equal(original.Attr.SynType, restored.Attr.SynType)
    Assert.Equal(3, restored.Attr.Attrs.Length)
    Assert.Equal(10L, restored.Attr.Attrs[0].AttrId)
    Assert.Equal(100L, restored.Attr.Attrs[0].AttrVal)
    Assert.Equal(20L, restored.Attr.Attrs[1].AttrId)
    Assert.Equal(200L, restored.Attr.Attrs[1].AttrVal)
    Assert.Equal(30L, restored.Attr.Attrs[2].AttrId)
    Assert.Equal(300L, restored.Attr.Attrs[2].AttrVal)

[<Fact>]
let ``McSynSkillStateMessage roundtrip`` () =
    // Синхронизация состояний скиллов: текущее время + массив активных эффектов
    let original =
        { McSynSkillStateMessage.WorldId = 7777L
          SkillState = { CurrentTime = 1000000L
                         States = [| { StateId = 101L; StateLv = 3L; Duration = 60000L; StartTime = 999000L }
                                     { StateId = 202L; StateLv = 1L; Duration = 30000L; StartTime = 998000L } |] } }
    let restored = roundtrip Serialize.mcSynSkillStateMessage Deserialize.mcSynSkillStateMessage original
    Assert.Equal(original.WorldId, restored.WorldId)
    Assert.Equal(original.SkillState.CurrentTime, restored.SkillState.CurrentTime)
    Assert.Equal(2, restored.SkillState.States.Length)
    Assert.Equal(101L, restored.SkillState.States[0].StateId)
    Assert.Equal(3L, restored.SkillState.States[0].StateLv)
    Assert.Equal(60000L, restored.SkillState.States[0].Duration)
    Assert.Equal(999000L, restored.SkillState.States[0].StartTime)
    Assert.Equal(202L, restored.SkillState.States[1].StateId)
    Assert.Equal(30000L, restored.SkillState.States[1].Duration)

[<Fact>]
let ``McAStateEndSeeMessage roundtrip`` () =
    // Завершение видимых area-состояний: только координаты области (без массива)
    let original = { McAStateEndSeeMessage.AreaX = 512L; AreaY = 768L }
    let restored = roundtrip Serialize.mcAStateEndSeeMessage Deserialize.mcAStateEndSeeMessage original
    Assert.Equal(original.AreaX, restored.AreaX)
    Assert.Equal(original.AreaY, restored.AreaY)

[<Fact>]
let ``McAStateBeginSeeMessage roundtrip`` () =
    // Начало видимых area-состояний: StateId=0 пропускает доп. поля (условный skip)
    let original =
        { McAStateBeginSeeMessage.AreaX = 100L; AreaY = 200L
          States = [| { StateId = 0L; StateLv = 0L; WorldId = 0L; FightId = 0L }
                      { StateId = 5L; StateLv = 2L; WorldId = 9999L; FightId = 8888L } |] }
    let restored = roundtrip Serialize.mcAStateBeginSeeMessage Deserialize.mcAStateBeginSeeMessage original
    Assert.Equal(original.AreaX, restored.AreaX)
    Assert.Equal(original.AreaY, restored.AreaY)
    Assert.Equal(2, restored.States.Length)
    // Запись с StateId=0: все поля обнулены
    Assert.Equal(0L, restored.States[0].StateId)
    Assert.Equal(0L, restored.States[0].StateLv)
    Assert.Equal(0L, restored.States[0].WorldId)
    Assert.Equal(0L, restored.States[0].FightId)
    // Запись с StateId>0: поля сохраняются полностью
    Assert.Equal(5L, restored.States[1].StateId)
    Assert.Equal(2L, restored.States[1].StateLv)
    Assert.Equal(9999L, restored.States[1].WorldId)
    Assert.Equal(8888L, restored.States[1].FightId)

[<Fact>]
let ``McDelItemChaMessage roundtrip`` () =
    // Удаление предмета персонажа: два int64
    let original = { McDelItemChaMessage.MainChaId = 55001L; WorldId = 0xABCDL }
    let restored = roundtrip Serialize.mcDelItemChaMessage Deserialize.mcDelItemChaMessage original
    Assert.Equal(original.MainChaId, restored.MainChaId)
    Assert.Equal(original.WorldId, restored.WorldId)

[<Fact>]
let ``McSynEventInfoMessage roundtrip`` () =
    // Синхронизация информации о событии: 3 int64 + строка
    let original =
        { McSynEventInfoMessage.EntityId = 3001L; EntityType = 2L; EventId = 500L; EventName = "BossSpawn" }
    let restored = roundtrip Serialize.mcSynEventInfoMessage Deserialize.mcSynEventInfoMessage original
    Assert.Equal(original.EntityId, restored.EntityId)
    Assert.Equal(original.EntityType, restored.EntityType)
    Assert.Equal(original.EventId, restored.EventId)
    Assert.Equal(original.EventName, restored.EventName)

[<Fact>]
let ``McSynSideInfoMessage roundtrip`` () =
    // Синхронизация фракции: worldId + вложенная структура
    let original = { McSynSideInfoMessage.WorldId = 88001L; Side = { SideId = 3L } }
    let restored = roundtrip Serialize.mcSynSideInfoMessage Deserialize.mcSynSideInfoMessage original
    Assert.Equal(original.WorldId, restored.WorldId)
    Assert.Equal(original.Side.SideId, restored.Side.SideId)

[<Fact>]
let ``McMissionInfoMessage roundtrip`` () =
    // Информация о квестах: NPC + параметры списка + массив строк-функций
    let original =
        { McMissionInfoMessage.NpcId = 9001L; ListType = 1L; Prev = 0L; Next = 1L; PrevCmd = 2L; NextCmd = 3L
          Items = [| "KillWolves"; "GatherHerbs" |] }
    let restored = roundtrip Serialize.mcMissionInfoMessage Deserialize.mcMissionInfoMessage original
    Assert.Equal(original.NpcId, restored.NpcId)
    Assert.Equal(original.ListType, restored.ListType)
    Assert.Equal(original.Prev, restored.Prev)
    Assert.Equal(original.Next, restored.Next)
    Assert.Equal(original.PrevCmd, restored.PrevCmd)
    Assert.Equal(original.NextCmd, restored.NextCmd)
    Assert.Equal(2, restored.Items.Length)
    Assert.Equal("KillWolves", restored.Items[0])
    Assert.Equal("GatherHerbs", restored.Items[1])

[<Fact>]
let ``McMisPageMessage roundtrip — ITEM и KILL потребности`` () =
    // Страница квеста: условная сериализация потребностей (ITEM/KILL — 3 int64, DESP — строка)
    let original =
        { McMisPageMessage.Cmd = 1L; NpcId = 5001L; Name = "Kill the Pirates"
          Needs = [|
            { NeedType = MIS_NEED_ITEM; Param1 = 100L; Param2 = 5L; Param3 = 0L; Desp = "" }
            { NeedType = MIS_NEED_KILL; Param1 = 200L; Param2 = 10L; Param3 = 3L; Desp = "" }
          |]
          PrizeSelType = 0L
          Prizes = [| { Type = 0L; Param1 = 300L; Param2 = 1L } |]
          Description = "Defeat pirates and collect loot" }
    let restored = roundtrip Serialize.mcMisPageMessage Deserialize.mcMisPageMessage original
    Assert.Equal(original.Cmd, restored.Cmd)
    Assert.Equal(original.NpcId, restored.NpcId)
    Assert.Equal(original.Name, restored.Name)
    Assert.Equal(2, restored.Needs.Length)
    Assert.Equal(MIS_NEED_ITEM, restored.Needs[0].NeedType)
    Assert.Equal(100L, restored.Needs[0].Param1)
    Assert.Equal(5L, restored.Needs[0].Param2)
    Assert.Equal(MIS_NEED_KILL, restored.Needs[1].NeedType)
    Assert.Equal(200L, restored.Needs[1].Param1)
    Assert.Equal(10L, restored.Needs[1].Param2)
    Assert.Equal(3L, restored.Needs[1].Param3)
    Assert.Equal(1, restored.Prizes.Length)
    Assert.Equal(300L, restored.Prizes[0].Param1)
    Assert.Equal(original.Description, restored.Description)

[<Fact>]
let ``McMisPageMessage roundtrip — DESP потребность`` () =
    // Страница квеста: потребность-описание (MIS_NEED_DESP = 5)
    let original =
        { McMisPageMessage.Cmd = 2L; NpcId = 6001L; Name = "Explore the Island"
          Needs = [|
            { NeedType = MIS_NEED_DESP; Param1 = 0L; Param2 = 0L; Param3 = 0L; Desp = "Find the hidden cave entrance" }
          |]
          PrizeSelType = 1L
          Prizes = [| { Type = 1L; Param1 = 500L; Param2 = 2L }; { Type = 0L; Param1 = 400L; Param2 = 3L } |]
          Description = "The island holds many secrets" }
    let restored = roundtrip Serialize.mcMisPageMessage Deserialize.mcMisPageMessage original
    Assert.Equal(1, restored.Needs.Length)
    Assert.Equal(MIS_NEED_DESP, restored.Needs[0].NeedType)
    Assert.Equal("Find the hidden cave entrance", restored.Needs[0].Desp)
    Assert.Equal(2, restored.Prizes.Length)
    Assert.Equal(original.Description, restored.Description)

[<Fact>]
let ``McMisLogMessage roundtrip`` () =
    // Журнал квестов: массив записей (id миссии + состояние)
    let original =
        { McMisLogMessage.Logs =
            [| { MisLogEntry.MisId = 101L; State = 0L }
               { MisLogEntry.MisId = 102L; State = 1L } |] }
    let restored = roundtrip Serialize.mcMisLogMessage Deserialize.mcMisLogMessage original
    Assert.Equal(2, restored.Logs.Length)
    Assert.Equal(101L, restored.Logs[0].MisId)
    Assert.Equal(0L, restored.Logs[0].State)
    Assert.Equal(102L, restored.Logs[1].MisId)
    Assert.Equal(1L, restored.Logs[1].State)

[<Fact>]
let ``McMisLogInfoMessage roundtrip`` () =
    // Детали записи журнала квестов: формат аналогичен MisPage (misId + потребности + награды)
    let original =
        { McMisLogInfoMessage.MisId = 7001L; Name = "Skeleton Hunt"
          Needs = [|
            { NeedType = MIS_NEED_KILL; Param1 = 300L; Param2 = 10L; Param3 = 0L; Desp = "" }
            { NeedType = MIS_NEED_DESP; Param1 = 0L; Param2 = 0L; Param3 = 0L; Desp = "Defeat 10 skeleton warriors" }
          |]
          PrizeSelType = 0L
          Prizes = [| { Type = 0L; Param1 = 1000L; Param2 = 1L } |]
          Description = "Hunt skeletons in the graveyard" }
    let restored = roundtrip Serialize.mcMisLogInfoMessage Deserialize.mcMisLogInfoMessage original
    Assert.Equal(original.MisId, restored.MisId)
    Assert.Equal(original.Name, restored.Name)
    Assert.Equal(2, restored.Needs.Length)
    Assert.Equal(MIS_NEED_KILL, restored.Needs[0].NeedType)
    Assert.Equal(300L, restored.Needs[0].Param1)
    Assert.Equal(MIS_NEED_DESP, restored.Needs[1].NeedType)
    Assert.Equal("Defeat 10 skeleton warriors", restored.Needs[1].Desp)
    Assert.Equal(1, restored.Prizes.Length)
    Assert.Equal(original.Description, restored.Description)

[<Fact>]
let ``McMisLogClearMessage roundtrip`` () =
    // Очистка записи журнала квестов
    let original = { McMisLogClearMessage.MissionId = 5001L }
    let restored = roundtrip Serialize.mcMisLogClearMessage Deserialize.mcMisLogClearMessage original
    Assert.Equal(original.MissionId, restored.MissionId)

[<Fact>]
let ``McMisLogAddMessage roundtrip`` () =
    // Добавление записи в журнал квестов: ID миссии + состояние
    let original = { McMisLogAddMessage.MissionId = 5002L; State = 1L }
    let restored = roundtrip Serialize.mcMisLogAddMessage Deserialize.mcMisLogAddMessage original
    Assert.Equal(original.MissionId, restored.MissionId)
    Assert.Equal(original.State, restored.State)

[<Fact>]
let ``McMisLogStateMessage roundtrip`` () =
    // Обновление состояния записи журнала квестов: ID миссии + новое состояние
    let original = { McMisLogStateMessage.MissionId = 3001L; State = 7L }
    let restored = roundtrip Serialize.mcMisLogStateMessage Deserialize.mcMisLogStateMessage original
    Assert.Equal(original.MissionId, restored.MissionId)
    Assert.Equal(original.State, restored.State)

[<Fact>]
let ``McFuncInfoMessage roundtrip`` () =
    // Информация NPC-диалога: ID, страница, текст, функции и квесты
    let original =
        { McFuncInfoMessage.NpcId = 8001L; Page = 2L; TalkText = "Welcome, sailor!"
          FuncItems = [| { FuncInfoFuncItem.Name = "Shop" }; { FuncInfoFuncItem.Name = "Repair" } |]
          MissionItems = [| { FuncInfoMissionItem.Name = "DeliverGoods"; State = 1L } |] }
    let restored = roundtrip Serialize.mcFuncInfoMessage Deserialize.mcFuncInfoMessage original
    Assert.Equal(original.NpcId, restored.NpcId)
    Assert.Equal(original.Page, restored.Page)
    Assert.Equal(original.TalkText, restored.TalkText)
    Assert.Equal(2, restored.FuncItems.Length)
    Assert.Equal("Shop", restored.FuncItems[0].Name)
    Assert.Equal("Repair", restored.FuncItems[1].Name)
    Assert.Equal(1, restored.MissionItems.Length)
    Assert.Equal("DeliverGoods", restored.MissionItems[0].Name)
    Assert.Equal(1L, restored.MissionItems[0].State)

[<Fact>]
let ``McVolunteerListMessage roundtrip`` () =
    // Список волонтёров: пагинация + массив записей
    let original =
        { McVolunteerListMessage.PageTotal = 5L; Page = 2L
          Volunteers = [| { VolunteerEntry.Name = "Helper1"; Level = 45L; Job = 2L; Map = "garner" }
                          { VolunteerEntry.Name = "Helper2"; Level = 60L; Job = 1L; Map = "darkswamp" } |] }
    let restored = roundtrip Serialize.mcVolunteerListMessage Deserialize.mcVolunteerListMessage original
    Assert.Equal(original.PageTotal, restored.PageTotal)
    Assert.Equal(original.Page, restored.Page)
    Assert.Equal(2, restored.Volunteers.Length)
    Assert.Equal("Helper1", restored.Volunteers[0].Name)
    Assert.Equal(45L, restored.Volunteers[0].Level)
    Assert.Equal(2L, restored.Volunteers[0].Job)
    Assert.Equal("garner", restored.Volunteers[0].Map)
    Assert.Equal("Helper2", restored.Volunteers[1].Name)
    Assert.Equal(60L, restored.Volunteers[1].Level)

[<Fact>]
let ``McVolunteerOpenMessage roundtrip`` () =
    // Открытие окна волонтёров: статус + пагинация + записи
    let original =
        { McVolunteerOpenMessage.State = 1L; PageTotal = 3L
          Volunteers = [| { VolunteerEntry.Name = "Guide1"; Level = 70L; Job = 3L; Map = "shaitan" }
                          { VolunteerEntry.Name = "Guide2"; Level = 55L; Job = 0L; Map = "argent" } |] }
    let restored = roundtrip Serialize.mcVolunteerOpenMessage Deserialize.mcVolunteerOpenMessage original
    Assert.Equal(original.State, restored.State)
    Assert.Equal(original.PageTotal, restored.PageTotal)
    Assert.Equal(2, restored.Volunteers.Length)
    Assert.Equal("Guide1", restored.Volunteers[0].Name)
    Assert.Equal(70L, restored.Volunteers[0].Level)
    Assert.Equal(3L, restored.Volunteers[0].Job)
    Assert.Equal("shaitan", restored.Volunteers[0].Map)
    Assert.Equal("Guide2", restored.Volunteers[1].Name)
    Assert.Equal(55L, restored.Volunteers[1].Level)

[<Fact>]
let ``McShowStallSearchMessage roundtrip`` () =
    // Результаты поиска прилавков: count в начале (не ReverseRead)
    let original =
        { McShowStallSearchMessage.Entries =
            [| { StallSearchEntry.Name = "Potion"; StallName = "MagicShop"; Location = "garner"; Count = 50L; Cost = 100L }
               { StallSearchEntry.Name = "Sword"; StallName = "WeaponStore"; Location = "shaitan"; Count = 5L; Cost = 25000L } |] }
    let restored = roundtrip Serialize.mcShowStallSearchMessage Deserialize.mcShowStallSearchMessage original
    Assert.Equal(2, restored.Entries.Length)
    Assert.Equal("Potion", restored.Entries[0].Name)
    Assert.Equal("MagicShop", restored.Entries[0].StallName)
    Assert.Equal("garner", restored.Entries[0].Location)
    Assert.Equal(50L, restored.Entries[0].Count)
    Assert.Equal(100L, restored.Entries[0].Cost)
    Assert.Equal("Sword", restored.Entries[1].Name)
    Assert.Equal("WeaponStore", restored.Entries[1].StallName)
    Assert.Equal(25000L, restored.Entries[1].Cost)

[<Fact>]
let ``McShowRankingMessage roundtrip`` () =
    // Таблица рейтинга: массив записей (имя, гильдия, уровень, класс, счёт)
    let original =
        { McShowRankingMessage.Entries =
            [| { RankingEntry.Name = "TopPlayer"; Guild = "BestGuild"; Level = 99L; Job = 1L; Score = 999999L }
               { RankingEntry.Name = "Runner"; Guild = "FastGuild"; Level = 85L; Job = 2L; Score = 500000L } |] }
    let restored = roundtrip Serialize.mcShowRankingMessage Deserialize.mcShowRankingMessage original
    Assert.Equal(2, restored.Entries.Length)
    Assert.Equal("TopPlayer", restored.Entries[0].Name)
    Assert.Equal("BestGuild", restored.Entries[0].Guild)
    Assert.Equal(99L, restored.Entries[0].Level)
    Assert.Equal(1L, restored.Entries[0].Job)
    Assert.Equal(999999L, restored.Entries[0].Score)
    Assert.Equal("Runner", restored.Entries[1].Name)
    Assert.Equal("FastGuild", restored.Entries[1].Guild)
    Assert.Equal(500000L, restored.Entries[1].Score)

[<Fact>]
let ``McEspeItemMessage roundtrip`` () =
    // Специальные предметы инвентаря: worldId + массив с 5 полями каждый
    let original =
        { McEspeItemMessage.WorldId = 12345L
          Items = [| { EspeItemEntry.Position = 0L; Endure = 100L; Energy = 50L; Tradable = 1L; Expiration = 0L }
                     { EspeItemEntry.Position = 1L; Endure = 80L; Energy = 30L; Tradable = 0L; Expiration = 3600L }
                     { EspeItemEntry.Position = 5L; Endure = 200L; Energy = 0L; Tradable = 1L; Expiration = 7200L } |] }
    let restored = roundtrip Serialize.mcEspeItemMessage Deserialize.mcEspeItemMessage original
    Assert.Equal(original.WorldId, restored.WorldId)
    Assert.Equal(3, restored.Items.Length)
    Assert.Equal(0L, restored.Items[0].Position)
    Assert.Equal(100L, restored.Items[0].Endure)
    Assert.Equal(50L, restored.Items[0].Energy)
    Assert.Equal(1L, restored.Items[0].Tradable)
    Assert.Equal(0L, restored.Items[0].Expiration)
    Assert.Equal(1L, restored.Items[1].Position)
    Assert.Equal(80L, restored.Items[1].Endure)
    Assert.Equal(3600L, restored.Items[1].Expiration)
    Assert.Equal(5L, restored.Items[2].Position)
    Assert.Equal(200L, restored.Items[2].Endure)
    Assert.Equal(7200L, restored.Items[2].Expiration)

[<Fact>]
let ``McBlackMarketExchangeDataMessage roundtrip`` () =
    // Данные обмена на чёрном рынке: NPC ID + массив записей
    let original =
        { McBlackMarketExchangeDataMessage.NpcId = 9001L
          Exchanges = [| { BlackMarketExchangeEntry.SrcId = 101L; SrcCount = 10L; TarId = 201L; TarCount = 5L; TimeValue = 3600L }
                         { BlackMarketExchangeEntry.SrcId = 102L; SrcCount = 20L; TarId = 202L; TarCount = 15L; TimeValue = 7200L } |] }
    let restored = roundtrip Serialize.mcBlackMarketExchangeDataMessage Deserialize.mcBlackMarketExchangeDataMessage original
    Assert.Equal(original.NpcId, restored.NpcId)
    Assert.Equal(2, restored.Exchanges.Length)
    Assert.Equal(101L, restored.Exchanges[0].SrcId)
    Assert.Equal(10L, restored.Exchanges[0].SrcCount)
    Assert.Equal(201L, restored.Exchanges[0].TarId)
    Assert.Equal(5L, restored.Exchanges[0].TarCount)
    Assert.Equal(3600L, restored.Exchanges[0].TimeValue)
    Assert.Equal(102L, restored.Exchanges[1].SrcId)
    Assert.Equal(7200L, restored.Exchanges[1].TimeValue)

[<Fact>]
let ``McExchangeDataMessage roundtrip`` () =
    // Данные обычного обмена: NPC ID + массив записей (4 поля)
    let original =
        { McExchangeDataMessage.NpcId = 9002L
          Exchanges = [| { ExchangeEntry.SrcId = 301L; SrcCount = 50L; TarId = 401L; TarCount = 25L }
                         { ExchangeEntry.SrcId = 302L; SrcCount = 100L; TarId = 402L; TarCount = 75L } |] }
    let restored = roundtrip Serialize.mcExchangeDataMessage Deserialize.mcExchangeDataMessage original
    Assert.Equal(original.NpcId, restored.NpcId)
    Assert.Equal(2, restored.Exchanges.Length)
    Assert.Equal(301L, restored.Exchanges[0].SrcId)
    Assert.Equal(50L, restored.Exchanges[0].SrcCount)
    Assert.Equal(401L, restored.Exchanges[0].TarId)
    Assert.Equal(25L, restored.Exchanges[0].TarCount)
    Assert.Equal(302L, restored.Exchanges[1].SrcId)
    Assert.Equal(75L, restored.Exchanges[1].TarCount)

[<Fact>]
let ``McBlackMarketExchangeUpdateMessage roundtrip`` () =
    // Обновление данных обмена на чёрном рынке
    let original =
        { McBlackMarketExchangeUpdateMessage.NpcId = 9003L
          Exchanges = [| { BlackMarketExchangeEntry.SrcId = 501L; SrcCount = 1L; TarId = 601L; TarCount = 2L; TimeValue = 1800L }
                         { BlackMarketExchangeEntry.SrcId = 502L; SrcCount = 3L; TarId = 602L; TarCount = 4L; TimeValue = 5400L } |] }
    let restored = roundtrip Serialize.mcBlackMarketExchangeUpdateMessage Deserialize.mcBlackMarketExchangeUpdateMessage original
    Assert.Equal(original.NpcId, restored.NpcId)
    Assert.Equal(2, restored.Exchanges.Length)
    Assert.Equal(501L, restored.Exchanges[0].SrcId)
    Assert.Equal(1L, restored.Exchanges[0].SrcCount)
    Assert.Equal(601L, restored.Exchanges[0].TarId)
    Assert.Equal(2L, restored.Exchanges[0].TarCount)
    Assert.Equal(1800L, restored.Exchanges[0].TimeValue)
    Assert.Equal(502L, restored.Exchanges[1].SrcId)
    Assert.Equal(5400L, restored.Exchanges[1].TimeValue)

[<Fact>]
let ``McBlackMarketExchangeAsrMessage roundtrip`` () =
    // Результат обмена на чёрном рынке: 5 полей int64
    let original =
        { McBlackMarketExchangeAsrMessage.Success = 1L; SrcId = 50001L; SrcCount = 10L; TarId = 50002L; TarCount = 5L }
    let restored = roundtrip Serialize.mcBlackMarketExchangeAsrMessage Deserialize.mcBlackMarketExchangeAsrMessage original
    Assert.Equal(original.Success, restored.Success)
    Assert.Equal(original.SrcId, restored.SrcId)
    Assert.Equal(original.SrcCount, restored.SrcCount)
    Assert.Equal(original.TarId, restored.TarId)
    Assert.Equal(original.TarCount, restored.TarCount)

// ═══════════════════════════════════════════════════════════════
//  Торговля, магазин, лавки, пати — сложные команды
// ═══════════════════════════════════════════════════════════════

[<Fact>]
let ``McTradeAllDataMessage roundtrip — TRADE_GOODS (type=1)`` () =
    // Полные данные торговли NPC: type=1 (TRADE_GOODS) — с count/price/level
    let original =
        { McTradeAllDataMessage.NpcId = 5001L; TradeType = 1L; Param = 42L
          Pages = [|
            { ItemType = 10L
              Items = [| { ItemId = 100L; Count = 5L; Price = 200L; Level = 3L }
                         { ItemId = 101L; Count = 10L; Price = 350L; Level = 5L } |] }
            { ItemType = 20L
              Items = [| { ItemId = 200L; Count = 1L; Price = 999L; Level = 10L } |] }
          |] }
    let restored = roundtrip Serialize.mcTradeAllDataMessage Deserialize.mcTradeAllDataMessage original
    Assert.Equal(original.NpcId, restored.NpcId)
    Assert.Equal(original.TradeType, restored.TradeType)
    Assert.Equal(original.Param, restored.Param)
    Assert.Equal(2, restored.Pages.Length)
    // Первая страница
    Assert.Equal(10L, restored.Pages[0].ItemType)
    Assert.Equal(2, restored.Pages[0].Items.Length)
    Assert.Equal(100L, restored.Pages[0].Items[0].ItemId)
    Assert.Equal(5L, restored.Pages[0].Items[0].Count)
    Assert.Equal(200L, restored.Pages[0].Items[0].Price)
    Assert.Equal(3L, restored.Pages[0].Items[0].Level)
    Assert.Equal(101L, restored.Pages[0].Items[1].ItemId)
    Assert.Equal(10L, restored.Pages[0].Items[1].Count)
    Assert.Equal(350L, restored.Pages[0].Items[1].Price)
    Assert.Equal(5L, restored.Pages[0].Items[1].Level)
    // Вторая страница
    Assert.Equal(20L, restored.Pages[1].ItemType)
    Assert.Equal(1, restored.Pages[1].Items.Length)
    Assert.Equal(200L, restored.Pages[1].Items[0].ItemId)
    Assert.Equal(999L, restored.Pages[1].Items[0].Price)

[<Fact>]
let ``McTradeAllDataMessage roundtrip — не TRADE_GOODS (type=0)`` () =
    // Полные данные торговли NPC: type=0 — без count/price/level
    let original =
        { McTradeAllDataMessage.NpcId = 6001L; TradeType = 0L; Param = 7L
          Pages = [|
            { ItemType = 30L
              Items = [| { ItemId = 300L; Count = 0L; Price = 0L; Level = 0L }
                         { ItemId = 301L; Count = 0L; Price = 0L; Level = 0L } |] }
          |] }
    let restored = roundtrip Serialize.mcTradeAllDataMessage Deserialize.mcTradeAllDataMessage original
    Assert.Equal(original.NpcId, restored.NpcId)
    Assert.Equal(0L, restored.TradeType)
    Assert.Equal(1, restored.Pages.Length)
    Assert.Equal(2, restored.Pages[0].Items.Length)
    Assert.Equal(300L, restored.Pages[0].Items[0].ItemId)
    Assert.Equal(0L, restored.Pages[0].Items[0].Count)
    Assert.Equal(0L, restored.Pages[0].Items[0].Price)

[<Fact>]
let ``McStoreOpenAnswerMessage roundtrip — valid`` () =
    // Ответ на открытие магазина: valid=true с объявлениями и категориями
    let original =
        { McStoreOpenAnswerMessage.IsValid = true; Vip = 1L; MoBean = 500L; ReplMoney = 100L
          Affiches = [| { AfficheId = 1L; Title = "Летняя распродажа"; ComId = "SALE01" }
                        { AfficheId = 2L; Title = "Новинки"; ComId = "NEW02" } |]
          Classes = [| { ClassId = 10L; ClassName = "Оружие"; ParentId = 0L }
                       { ClassId = 11L; ClassName = "Мечи"; ParentId = 10L } |] }
    let restored = roundtrip Serialize.mcStoreOpenAnswerMessage Deserialize.mcStoreOpenAnswerMessage original
    Assert.True(restored.IsValid)
    Assert.Equal(1L, restored.Vip)
    Assert.Equal(500L, restored.MoBean)
    Assert.Equal(100L, restored.ReplMoney)
    Assert.Equal(2, restored.Affiches.Length)
    Assert.Equal(1L, restored.Affiches[0].AfficheId)
    Assert.Equal("Летняя распродажа", restored.Affiches[0].Title)
    Assert.Equal("SALE01", restored.Affiches[0].ComId)
    Assert.Equal(2L, restored.Affiches[1].AfficheId)
    Assert.Equal(2, restored.Classes.Length)
    Assert.Equal(10L, restored.Classes[0].ClassId)
    Assert.Equal("Оружие", restored.Classes[0].ClassName)
    Assert.Equal(0L, restored.Classes[0].ParentId)
    Assert.Equal(11L, restored.Classes[1].ClassId)
    Assert.Equal("Мечи", restored.Classes[1].ClassName)
    Assert.Equal(10L, restored.Classes[1].ParentId)

[<Fact>]
let ``McStoreOpenAnswerMessage roundtrip — invalid`` () =
    // Ответ на открытие магазина: valid=false — пустые данные
    let original =
        { McStoreOpenAnswerMessage.IsValid = false; Vip = 0L; MoBean = 0L; ReplMoney = 0L
          Affiches = [||]; Classes = [||] }
    let restored = roundtrip Serialize.mcStoreOpenAnswerMessage Deserialize.mcStoreOpenAnswerMessage original
    Assert.False(restored.IsValid)
    Assert.Equal(0, restored.Affiches.Length)
    Assert.Equal(0, restored.Classes.Length)

[<Fact>]
let ``McStoreListAnswerMessage roundtrip`` () =
    // Список товаров магазина: 1 товар с 2 вариантами, каждый с 5 атрибутами
    let attrs1 : AttrEntry[] =
        [| { AttrEntry.AttrId = 1L; AttrVal = 100L }
           { AttrEntry.AttrId = 2L; AttrVal = 50L }
           { AttrEntry.AttrId = 3L; AttrVal = 25L }
           { AttrEntry.AttrId = 0L; AttrVal = 0L }
           { AttrEntry.AttrId = 0L; AttrVal = 0L } |]
    let attrs2 : AttrEntry[] =
        [| { AttrEntry.AttrId = 4L; AttrVal = 200L }
           { AttrEntry.AttrId = 5L; AttrVal = 75L }
           { AttrEntry.AttrId = 0L; AttrVal = 0L }
           { AttrEntry.AttrId = 0L; AttrVal = 0L }
           { AttrEntry.AttrId = 0L; AttrVal = 0L } |]
    let original =
        { McStoreListAnswerMessage.PageTotal = 3L; PageCurrent = 1L
          Products = [|
            { ComId = 1001L; ComName = "Меч тьмы"; Price = 5000L; Remark = "Легендарное оружие"
              IsHot = true; Time = 86400L; Quantity = 50L; Expire = 0L
              Variants = [| { ItemId = 2001L; ItemNum = 1L; Flute = 0L; Attrs = attrs1 }
                            { ItemId = 2002L; ItemNum = 3L; Flute = 1L; Attrs = attrs2 } |] }
          |] }
    let restored = roundtrip Serialize.mcStoreListAnswerMessage Deserialize.mcStoreListAnswerMessage original
    Assert.Equal(3L, restored.PageTotal)
    Assert.Equal(1L, restored.PageCurrent)
    Assert.Equal(1, restored.Products.Length)
    let p = restored.Products[0]
    Assert.Equal(1001L, p.ComId)
    Assert.Equal("Меч тьмы", p.ComName)
    Assert.Equal(5000L, p.Price)
    Assert.Equal("Легендарное оружие", p.Remark)
    Assert.True(p.IsHot)
    Assert.Equal(86400L, p.Time)
    Assert.Equal(50L, p.Quantity)
    Assert.Equal(0L, p.Expire)
    Assert.Equal(2, p.Variants.Length)
    Assert.Equal(2001L, p.Variants[0].ItemId)
    Assert.Equal(1L, p.Variants[0].ItemNum)
    Assert.Equal(0L, p.Variants[0].Flute)
    Assert.Equal(5, p.Variants[0].Attrs.Length)
    Assert.Equal(1L, p.Variants[0].Attrs[0].AttrId)
    Assert.Equal(100L, p.Variants[0].Attrs[0].AttrVal)
    Assert.Equal(2002L, p.Variants[1].ItemId)
    Assert.Equal(3L, p.Variants[1].ItemNum)
    Assert.Equal(1L, p.Variants[1].Flute)
    Assert.Equal(5, p.Variants[1].Attrs.Length)
    Assert.Equal(4L, p.Variants[1].Attrs[0].AttrId)
    Assert.Equal(200L, p.Variants[1].Attrs[0].AttrVal)

[<Fact>]
let ``McStoreHistoryMessage roundtrip`` () =
    // История покупок магазина: 2 записи
    let original =
        { McStoreHistoryMessage.Records = [|
            { Time = "2026-03-27 10:00"; ItemId = 3001L; Name = "Зелье HP"; Money = 100L }
            { Time = "2026-03-27 11:30"; ItemId = 3002L; Name = "Зелье SP"; Money = 150L }
          |] }
    let restored = roundtrip Serialize.mcStoreHistoryMessage Deserialize.mcStoreHistoryMessage original
    Assert.Equal(2, restored.Records.Length)
    Assert.Equal("2026-03-27 10:00", restored.Records[0].Time)
    Assert.Equal(3001L, restored.Records[0].ItemId)
    Assert.Equal("Зелье HP", restored.Records[0].Name)
    Assert.Equal(100L, restored.Records[0].Money)
    Assert.Equal("2026-03-27 11:30", restored.Records[1].Time)
    Assert.Equal(3002L, restored.Records[1].ItemId)
    Assert.Equal("Зелье SP", restored.Records[1].Name)
    Assert.Equal(150L, restored.Records[1].Money)

[<Fact>]
let ``McStoreVipMessage roundtrip — success`` () =
    // VIP-данные магазина: success=1
    let original =
        { McStoreVipMessage.Success = 1L; VipId = 5L; Months = 3L; Shuijing = 1000L; Modou = 500L }
    let restored = roundtrip Serialize.mcStoreVipMessage Deserialize.mcStoreVipMessage original
    Assert.Equal(1L, restored.Success)
    Assert.Equal(5L, restored.VipId)
    Assert.Equal(3L, restored.Months)
    Assert.Equal(1000L, restored.Shuijing)
    Assert.Equal(500L, restored.Modou)

[<Fact>]
let ``McStoreVipMessage roundtrip — failure`` () =
    // VIP-данные магазина: success=0 — пустые данные
    let original =
        { McStoreVipMessage.Success = 0L; VipId = 0L; Months = 0L; Shuijing = 0L; Modou = 0L }
    let restored = roundtrip Serialize.mcStoreVipMessage Deserialize.mcStoreVipMessage original
    Assert.Equal(0L, restored.Success)
    Assert.Equal(0L, restored.VipId)

[<Fact>]
let ``McSynTeamMessage roundtrip — персонаж (не лодка)`` () =
    // Синхронизация члена пати: HP/SP/Level + ChaLookInfo (персонаж)
    let original =
        { McSynTeamMessage.MemberId = 7001L; HP = 500L; MaxHP = 1000L
          SP = 200L; MaxSP = 400L; Level = 45L
          Look =
            { SynType = 0L; TypeId = 3L; IsBoat = false; BoatParts = ValueNone
              HairId = 12L
              Equips = Array.init EQUIP_NUM (fun i ->
                { LookEquipSlot.Id = int64 (i + 1); DbId = int64 (i * 10)
                  NeedLv = 0L; Num = 0L; Endure0 = 0L; Endure1 = 0L; Energy0 = 0L; Energy1 = 0L
                  ForgeLv = 0L; Valid = 0L; Tradable = 0L; Expiration = 0L
                  HasExtra = false; ForgeParam = 0L; InstId = 0L
                  HasInstAttr = false; InstAttr = [||] }) } }
    let restored = roundtrip Serialize.mcSynTeamMessage Deserialize.mcSynTeamMessage original
    Assert.Equal(original.MemberId, restored.MemberId)
    Assert.Equal(original.HP, restored.HP)
    Assert.Equal(original.MaxHP, restored.MaxHP)
    Assert.Equal(original.SP, restored.SP)
    Assert.Equal(original.MaxSP, restored.MaxSP)
    Assert.Equal(original.Level, restored.Level)
    Assert.False(restored.Look.IsBoat)
    Assert.Equal(original.Look.HairId, restored.Look.HairId)
    Assert.Equal(EQUIP_NUM, restored.Look.Equips.Length)
    Assert.Equal(1L, restored.Look.Equips[0].Id)
    Assert.Equal(0L, restored.Look.Equips[0].DbId)

// ═══════════════════════════════════════════════════════════════
//  Новые сложные команды: McItemCreate, McSynSkillBag
// ═══════════════════════════════════════════════════════════════

[<Fact>]
let ``McItemCreateMessage roundtrip`` () =
    // Предмет на земле: 9 полей + ChaEventInfo
    let original =
        { McItemCreateMessage.WorldId = 1001L; Handle = 2002L; ItemId = 3003L
          PosX = 500L; PosY = 600L; Angle = 45L; Num = 3L; AppeType = 1L; FromId = 4004L
          Event = { EntityId = 5005L; EntityType = 2L; EventId = 100L; EventName = "ItemDrop" } }
    let restored = roundtrip Serialize.mcItemCreateMessage Deserialize.mcItemCreateMessage original
    Assert.Equal(original.WorldId, restored.WorldId)
    Assert.Equal(original.Handle, restored.Handle)
    Assert.Equal(original.ItemId, restored.ItemId)
    Assert.Equal(original.PosX, restored.PosX)
    Assert.Equal(original.PosY, restored.PosY)
    Assert.Equal(original.Angle, restored.Angle)
    Assert.Equal(original.Num, restored.Num)
    Assert.Equal(original.AppeType, restored.AppeType)
    Assert.Equal(original.FromId, restored.FromId)
    Assert.Equal(original.Event.EntityId, restored.Event.EntityId)
    Assert.Equal(original.Event.EntityType, restored.Event.EntityType)
    Assert.Equal(original.Event.EventId, restored.Event.EventId)
    Assert.Equal(original.Event.EventName, restored.Event.EventName)

[<Fact>]
let ``McSynSkillBagMessage roundtrip — с range`` () =
    // Сумка скиллов: 2 скилла, один с range (условная сериализация)
    let original =
        { McSynSkillBagMessage.WorldId = 9001L
          SkillBag =
            { DefSkillId = 100L; SynType = 1L
              Skills = [|
                { Id = 10L; State = 1L; Level = 5L; UseSp = 20L; UseEndure = 10L
                  UseEnergy = 30L; ResumeTime = 2000L; Range = [| 1L; 50L; 100L; 150L |] }
                { Id = 20L; State = 0L; Level = 3L; UseSp = 15L; UseEndure = 5L
                  UseEnergy = 25L; ResumeTime = 1000L; Range = [| 0L; 0L; 0L; 0L |] }
              |] } }
    let restored = roundtrip Serialize.mcSynSkillBagMessage Deserialize.mcSynSkillBagMessage original
    Assert.Equal(original.WorldId, restored.WorldId)
    Assert.Equal(original.SkillBag.DefSkillId, restored.SkillBag.DefSkillId)
    Assert.Equal(original.SkillBag.SynType, restored.SkillBag.SynType)
    Assert.Equal(2, restored.SkillBag.Skills.Length)
    // Первый скилл: range[0] != 0 → range полностью
    let sk0 = restored.SkillBag.Skills[0]
    Assert.Equal(10L, sk0.Id)
    Assert.Equal(5L, sk0.Level)
    Assert.Equal(SKILL_RANGE_PARAM_NUM, sk0.Range.Length)
    Assert.Equal(1L, sk0.Range[0])
    Assert.Equal(50L, sk0.Range[1])
    Assert.Equal(100L, sk0.Range[2])
    Assert.Equal(150L, sk0.Range[3])
    // Второй скилл: range[0] == 0 → range[1..3] = 0
    let sk1 = restored.SkillBag.Skills[1]
    Assert.Equal(20L, sk1.Id)
    Assert.Equal(0L, sk1.Range[0])
    Assert.Equal(0L, sk1.Range[1])

[<Fact>]
let ``McSynSkillBagMessage roundtrip — пустая сумка`` () =
    // Пустая сумка скиллов
    let original =
        { McSynSkillBagMessage.WorldId = 9002L
          SkillBag = { DefSkillId = 0L; SynType = 0L; Skills = [||] } }
    let restored = roundtrip Serialize.mcSynSkillBagMessage Deserialize.mcSynSkillBagMessage original
    Assert.Equal(original.WorldId, restored.WorldId)
    Assert.Equal(0L, restored.SkillBag.DefSkillId)
    Assert.Equal(0, restored.SkillBag.Skills.Length)

// ═══════════════════════════════════════════════════════════════
//  McChaBeginSee / McAddItemCha
// ═══════════════════════════════════════════════════════════════

/// Вспомогательный пустой LookEquipSlot (id=0, без условных полей).
let emptyEquipSlot : LookEquipSlot =
    { Id = 0L; DbId = 0L; NeedLv = 0L; Num = 0L; Endure0 = 0L; Endure1 = 0L
      Energy0 = 0L; Energy1 = 0L; ForgeLv = 0L; Valid = 0L; Tradable = 0L; Expiration = 0L
      HasExtra = false; ForgeParam = 0L; InstId = 0L; HasInstAttr = false; InstAttr = [||] }

/// Минимальный ChaBaseInfo: не корабль, все equip id=0, пустые массивы.
let minimalBaseInfo name =
    { ChaId = 1L; WorldId = 100L; CommId = 0L; CommName = ""; GmLv = 0L; Handle = 0L; CtrlType = 0L
      Name = name; Motto = "Yarr!"; Icon = 1L; GuildId = 0L
      GuildName = ""; GuildMotto = ""; GuildPermission = 0L; StallName = ""
      State = 0L; PosX = 500L; PosY = 600L; Radius = 50L; Angle = 90L
      TeamLeaderId = 0L; IsPlayer = 1L
      Side = { SideId = 0L }
      Event = { EntityId = 0L; EntityType = 0L; EventId = 0L; EventName = "" }
      Look = { SynType = 0L; TypeId = 1L; IsBoat = false; BoatParts = ValueNone; HairId = 5L
               Equips = Array.init EQUIP_NUM (fun _ -> emptyEquipSlot) }
      PkCtrl = 0L
      AppendLook = Array.init ESPE_KBGRID_NUM (fun _ -> { LookId = 0L; Valid = 0L }) }

[<Fact>]
let ``McChaBeginSeeMessage roundtrip — PoseNone`` () =
    let original =
        { McChaBeginSeeMessage.SeeType = 1L; Base = minimalBaseInfo "TestPirate"
          NpcType = 0L; NpcState = 0L; PoseType = 0L
          Lean = ValueNone; Seat = ValueNone
          Attr = { SynType = 0L; Attrs = [||] }
          SkillState = { CurrentTime = 0L; States = [||] } }
    let restored = roundtrip Serialize.mcChaBeginSeeMessage Deserialize.mcChaBeginSeeMessage original
    Assert.Equal(original.SeeType, restored.SeeType)
    Assert.Equal(original.Base.Name, restored.Base.Name)
    Assert.Equal(original.Base.PosX, restored.Base.PosX)
    Assert.Equal(original.Base.PosY, restored.Base.PosY)
    Assert.Equal(original.Base.Motto, restored.Base.Motto)
    Assert.Equal(original.Base.Look.HairId, restored.Base.Look.HairId)
    Assert.Equal(EQUIP_NUM, restored.Base.Look.Equips.Length)
    Assert.Equal(ESPE_KBGRID_NUM, restored.Base.AppendLook.Length)
    Assert.Equal(original.PoseType, restored.PoseType)
    Assert.True(restored.Lean.IsNone)
    Assert.True(restored.Seat.IsNone)
    Assert.Equal(0, restored.Attr.Attrs.Length)
    Assert.Equal(0, restored.SkillState.States.Length)

[<Fact>]
let ``McChaBeginSeeMessage roundtrip — PoseLean`` () =
    let original =
        { McChaBeginSeeMessage.SeeType = 2L; Base = minimalBaseInfo "LeanPirate"
          NpcType = 1L; NpcState = 5L; PoseType = 1L
          Lean = ValueSome { LeanState = 1L; Pose = 2L; Angle = 45L; PosX = 100L; PosY = 200L; Height = 10L }
          Seat = ValueNone
          Attr = { SynType = 0L; Attrs = [||] }
          SkillState = { CurrentTime = 0L; States = [||] } }
    let restored = roundtrip Serialize.mcChaBeginSeeMessage Deserialize.mcChaBeginSeeMessage original
    Assert.Equal(1L, restored.PoseType)
    Assert.True(restored.Lean.IsSome)
    Assert.Equal(45L, restored.Lean.Value.Angle)
    Assert.Equal(100L, restored.Lean.Value.PosX)
    Assert.Equal(200L, restored.Lean.Value.PosY)
    Assert.Equal(10L, restored.Lean.Value.Height)
    Assert.True(restored.Seat.IsNone)

[<Fact>]
let ``McChaBeginSeeMessage roundtrip — PoseSeat`` () =
    let original =
        { McChaBeginSeeMessage.SeeType = 3L; Base = minimalBaseInfo "SeatPirate"
          NpcType = 0L; NpcState = 0L; PoseType = 2L
          Lean = ValueNone
          Seat = ValueSome { SeatAngle = 90L; SeatPose = 1L }
          Attr = { SynType = 0L; Attrs = [||] }
          SkillState = { CurrentTime = 0L; States = [||] } }
    let restored = roundtrip Serialize.mcChaBeginSeeMessage Deserialize.mcChaBeginSeeMessage original
    Assert.Equal(2L, restored.PoseType)
    Assert.True(restored.Lean.IsNone)
    Assert.True(restored.Seat.IsSome)
    Assert.Equal(90L, restored.Seat.Value.SeatAngle)
    Assert.Equal(1L, restored.Seat.Value.SeatPose)

[<Fact>]
let ``McAddItemChaMessage roundtrip`` () =
    let original =
        { McAddItemChaMessage.MainChaId = 42L
          Base = minimalBaseInfo "ItemCha"
          Attr = { SynType = 0L; Attrs = [||] }
          Kitbag = { SynType = SYN_KITBAG_INIT; Capacity = 24L; Items = [||] }
          SkillState = { CurrentTime = 0L; States = [||] } }
    let restored = roundtrip Serialize.mcAddItemChaMessage Deserialize.mcAddItemChaMessage original
    Assert.Equal(42L, restored.MainChaId)
    Assert.Equal("ItemCha", restored.Base.Name)
    Assert.Equal(original.Base.PosX, restored.Base.PosX)
    Assert.Equal(SYN_KITBAG_INIT, restored.Kitbag.SynType)
    Assert.Equal(24L, restored.Kitbag.Capacity)
    Assert.Equal(0, restored.Kitbag.Items.Length)

[<Fact>]
let ``ChaBaseInfo roundtrip — boat look`` () =
    let boatBase =
        { ChaId = 10L; WorldId = 200L; CommId = 0L; CommName = ""; GmLv = 0L; Handle = 0L; CtrlType = 0L
          Name = "BoatPirate"; Motto = ""; Icon = 0L; GuildId = 0L
          GuildName = ""; GuildMotto = ""; GuildPermission = 0L; StallName = ""
          State = 0L; PosX = 1000L; PosY = 2000L; Radius = 100L; Angle = 180L
          TeamLeaderId = 0L; IsPlayer = 1L
          Side = { SideId = 0L }
          Event = { EntityId = 0L; EntityType = 0L; EventId = 0L; EventName = "" }
          Look = { SynType = 0L; TypeId = 5L; IsBoat = true
                   BoatParts = ValueSome { PosId = 1L; BoatId = 2L; Header = 3L; Body = 4L; Engine = 5L; Cannon = 6L; Equipment = 7L }
                   HairId = 0L; Equips = [||] }
          PkCtrl = 0L
          AppendLook = Array.init ESPE_KBGRID_NUM (fun _ -> { LookId = 0L; Valid = 0L }) }
    let original =
        { McChaBeginSeeMessage.SeeType = 1L; Base = boatBase
          NpcType = 0L; NpcState = 0L; PoseType = 0L
          Lean = ValueNone; Seat = ValueNone
          Attr = { SynType = 0L; Attrs = [||] }
          SkillState = { CurrentTime = 0L; States = [||] } }
    let restored = roundtrip Serialize.mcChaBeginSeeMessage Deserialize.mcChaBeginSeeMessage original
    Assert.True(restored.Base.Look.IsBoat)
    Assert.True(restored.Base.Look.BoatParts.IsSome)
    let bp = restored.Base.Look.BoatParts.Value
    Assert.Equal(1L, bp.PosId)
    Assert.Equal(2L, bp.BoatId)
    Assert.Equal(3L, bp.Header)
    Assert.Equal(4L, bp.Body)
    Assert.Equal(5L, bp.Engine)
    Assert.Equal(6L, bp.Cannon)
    Assert.Equal(7L, bp.Equipment)

[<Fact>]
let ``ChaKitbagInfo roundtrip — sentinel termination`` () =
    let kitbag =
        { SynType = SYN_KITBAG_INIT; Capacity = 48L
          Items = [|
            { GridId = 0L; ItemId = 101L; DbId = 1001L; NeedLv = 10L; Num = 5L
              Endure0 = 100L; Endure1 = 100L; Energy0 = 50L; Energy1 = 50L
              ForgeLv = 3L; Valid = 1L; Tradable = 1L; Expiration = 0L
              IsBoat = false; BoatWorldId = 0L; ForgeParam = 0L; InstId = 0L
              HasInstAttr = false; InstAttr = [||] }
            { GridId = 1L; ItemId = 0L; DbId = 0L; NeedLv = 0L; Num = 0L
              Endure0 = 0L; Endure1 = 0L; Energy0 = 0L; Energy1 = 0L
              ForgeLv = 0L; Valid = 0L; Tradable = 0L; Expiration = 0L
              IsBoat = false; BoatWorldId = 0L; ForgeParam = 0L; InstId = 0L
              HasInstAttr = false; InstAttr = [||] }
          |] }
    let original =
        { McAddItemChaMessage.MainChaId = 99L
          Base = minimalBaseInfo "KitbagTest"
          Attr = { SynType = 0L; Attrs = [||] }
          Kitbag = kitbag
          SkillState = { CurrentTime = 0L; States = [||] } }
    let restored = roundtrip Serialize.mcAddItemChaMessage Deserialize.mcAddItemChaMessage original
    Assert.Equal(SYN_KITBAG_INIT, restored.Kitbag.SynType)
    Assert.Equal(48L, restored.Kitbag.Capacity)
    Assert.Equal(2, restored.Kitbag.Items.Length)
    // Первый предмет: itemId > 0 → полная десериализация
    let item0 = restored.Kitbag.Items[0]
    Assert.Equal(101L, item0.ItemId)
    Assert.Equal(1001L, item0.DbId)
    Assert.Equal(5L, item0.Num)
    Assert.Equal(3L, item0.ForgeLv)
    Assert.False(item0.IsBoat)
    Assert.False(item0.HasInstAttr)
    // Второй предмет: itemId = 0 → пустой слот
    let item1 = restored.Kitbag.Items[1]
    Assert.Equal(0L, item1.ItemId)

// ═══════════════════════════════════════════════════════════════
//  McCharacterAction (CMD_MC_NOTIACTION) — roundtrip тесты
// ═══════════════════════════════════════════════════════════════

[<Fact>]
let ``McCharacterAction MOVE roundtrip`` () =
    // 3 waypoint'а по 8 байт (2 × int32): итого 24 байта
    let waypoints = Array.init 24 (fun i -> byte (i + 1))
    let original : McCharacterActionMessage =
        { WorldId = 1000L; PacketId = 42L
          Action = ActionMove { MoveState = 0x01L; StopState = 0x02L; Waypoints = waypoints } }
    let restored = roundtrip Serialize.mcCharacterActionMessage Deserialize.mcCharacterActionMessage original
    Assert.Equal(1000L, restored.WorldId)
    Assert.Equal(42L, restored.PacketId)
    match restored.Action with
    | ActionMove d ->
        Assert.Equal(0x01L, d.MoveState)
        Assert.Equal(0x02L, d.StopState)
        Assert.Equal<byte[]>(waypoints, d.Waypoints)
    | _ -> Assert.Fail("Ожидался ActionMove")

[<Fact>]
let ``McCharacterAction MOVE MoveState=0 без StopState`` () =
    let waypoints = Array.init 8 (fun i -> byte i)
    let original : McCharacterActionMessage =
        { WorldId = 1L; PacketId = 2L
          Action = ActionMove { MoveState = enumMSTATE_ON; StopState = 0L; Waypoints = waypoints } }
    let restored = roundtrip Serialize.mcCharacterActionMessage Deserialize.mcCharacterActionMessage original
    match restored.Action with
    | ActionMove d ->
        Assert.Equal(enumMSTATE_ON, d.MoveState)
        Assert.Equal(0L, d.StopState)
        Assert.Equal<byte[]>(waypoints, d.Waypoints)
    | _ -> Assert.Fail("Ожидался ActionMove")

[<Fact>]
let ``McCharacterAction SKILL_SRC roundtrip — targetType=1`` () =
    let original : McCharacterActionMessage =
        { WorldId = 500L; PacketId = 10L
          Action = ActionSkillSrc
            { FightId = 1L; Angle = 90L; State = 0x01L; StopState = 0x02L
              SkillId = 1001L; SkillSpeed = 200L
              TargetType = 1L; TargetId = 999L; TargetX = 100L; TargetY = 200L
              ExecTime = 500L
              Effects = [| { AttrId = 5L; AttrVal = -30L }; { AttrId = 10L; AttrVal = 50L } |]
              States = [| { AStateId = 3L; AStateLv = 2L } |] } }
    let restored = roundtrip Serialize.mcCharacterActionMessage Deserialize.mcCharacterActionMessage original
    Assert.Equal(500L, restored.WorldId)
    match restored.Action with
    | ActionSkillSrc d ->
        Assert.Equal(1L, d.FightId)
        Assert.Equal(90L, d.Angle)
        Assert.Equal(0x01L, d.State)
        Assert.Equal(0x02L, d.StopState)
        Assert.Equal(1001L, d.SkillId)
        Assert.Equal(1L, d.TargetType)
        Assert.Equal(999L, d.TargetId)
        Assert.Equal(100L, d.TargetX)
        Assert.Equal(200L, d.TargetY)
        Assert.Equal(2, d.Effects.Length)
        Assert.Equal(5L, d.Effects[0].AttrId)
        Assert.Equal(-30L, d.Effects[0].AttrVal)
        Assert.Equal(1, d.States.Length)
        Assert.Equal(3L, d.States[0].AStateId)
    | _ -> Assert.Fail("Ожидался ActionSkillSrc")

[<Fact>]
let ``McCharacterAction SKILL_SRC roundtrip — state=0 без StopState`` () =
    let original : McCharacterActionMessage =
        { WorldId = 1L; PacketId = 1L
          Action = ActionSkillSrc
            { FightId = 2L; Angle = 45L; State = enumFSTATE_ON; StopState = 0L
              SkillId = 100L; SkillSpeed = 100L
              TargetType = 2L; TargetId = 0L; TargetX = 50L; TargetY = 60L
              ExecTime = 300L; Effects = [||]; States = [||] } }
    let restored = roundtrip Serialize.mcCharacterActionMessage Deserialize.mcCharacterActionMessage original
    match restored.Action with
    | ActionSkillSrc d ->
        Assert.Equal(enumFSTATE_ON, d.State)
        Assert.Equal(0L, d.StopState)
        Assert.Equal(2L, d.TargetType)
        Assert.Equal(0L, d.TargetId)
        Assert.Equal(50L, d.TargetX)
    | _ -> Assert.Fail("Ожидался ActionSkillSrc")

[<Fact>]
let ``McCharacterAction SKILL_TAR roundtrip — полный`` () =
    let original : McCharacterActionMessage =
        { WorldId = 777L; PacketId = 55L
          Action = ActionSkillTar
            { FightId = 3L; State = 0x100L; DoubleAttack = true; Miss = false
              BeatBack = true; BeatBackX = 150L; BeatBackY = 250L
              SrcId = 400L; SrcPosX = 10L; SrcPosY = 20L
              SkillId = 2001L; SkillTargetX = 30L; SkillTargetY = 40L
              ExecTime = 600L; SynType = 2L
              Effects = [| { AttrId = 1L; AttrVal = -50L } |]
              HasStates = true; StateTime = 99999L
              States = [| { TarStateId = 7L; TarStateLv = 3L; TarDuration = 5000L; TarStartTime = 10000L } |]
              HasSrcEffect = true; SrcState = 0x200L; SrcSynType = 2L
              SrcEffects = [| { AttrId = 2L; AttrVal = -10L } |] } }
    let restored = roundtrip Serialize.mcCharacterActionMessage Deserialize.mcCharacterActionMessage original
    Assert.Equal(777L, restored.WorldId)
    match restored.Action with
    | ActionSkillTar d ->
        Assert.Equal(3L, d.FightId)
        Assert.Equal(0x100L, d.State)
        Assert.True(d.DoubleAttack)
        Assert.False(d.Miss)
        Assert.True(d.BeatBack)
        Assert.Equal(150L, d.BeatBackX)
        Assert.Equal(250L, d.BeatBackY)
        Assert.Equal(400L, d.SrcId)
        Assert.Equal(2001L, d.SkillId)
        Assert.Equal(1, d.Effects.Length)
        Assert.Equal(1L, d.Effects[0].AttrId)
        Assert.True(d.HasStates)
        Assert.Equal(99999L, d.StateTime)
        Assert.Equal(1, d.States.Length)
        Assert.Equal(7L, d.States[0].TarStateId)
        Assert.Equal(5000L, d.States[0].TarDuration)
        Assert.True(d.HasSrcEffect)
        Assert.Equal(0x200L, d.SrcState)
        Assert.Equal(1, d.SrcEffects.Length)
    | _ -> Assert.Fail("Ожидался ActionSkillTar")

[<Fact>]
let ``McCharacterAction SKILL_TAR roundtrip — минимальный`` () =
    let original : McCharacterActionMessage =
        { WorldId = 1L; PacketId = 1L
          Action = ActionSkillTar
            { FightId = 1L; State = 0L; DoubleAttack = false; Miss = true
              BeatBack = false; BeatBackX = 0L; BeatBackY = 0L
              SrcId = 10L; SrcPosX = 5L; SrcPosY = 6L
              SkillId = 100L; SkillTargetX = 7L; SkillTargetY = 8L
              ExecTime = 100L; SynType = 0L; Effects = [||]
              HasStates = false; StateTime = 0L; States = [||]
              HasSrcEffect = false; SrcState = 0L; SrcSynType = 0L; SrcEffects = [||] } }
    let restored = roundtrip Serialize.mcCharacterActionMessage Deserialize.mcCharacterActionMessage original
    match restored.Action with
    | ActionSkillTar d ->
        Assert.True(d.Miss)
        Assert.False(d.BeatBack)
        Assert.False(d.HasStates)
        Assert.False(d.HasSrcEffect)
    | _ -> Assert.Fail("Ожидался ActionSkillTar")

[<Fact>]
let ``McCharacterAction LEAN roundtrip — leanState=0`` () =
    let original : McCharacterActionMessage =
        { WorldId = 200L; PacketId = 5L
          Action = ActionLean { ActionLeanState = 0L; ActionPose = 1L; ActionAngle = 90L
                                ActionPosX = 100L; ActionPosY = 200L; ActionHeight = 50L } }
    let restored = roundtrip Serialize.mcCharacterActionMessage Deserialize.mcCharacterActionMessage original
    match restored.Action with
    | ActionLean d ->
        Assert.Equal(0L, d.ActionLeanState)
        Assert.Equal(1L, d.ActionPose)
        Assert.Equal(90L, d.ActionAngle)
        Assert.Equal(100L, d.ActionPosX)
        Assert.Equal(200L, d.ActionPosY)
        Assert.Equal(50L, d.ActionHeight)
    | _ -> Assert.Fail("Ожидался ActionLean")

[<Fact>]
let ``McCharacterAction LEAN roundtrip — leanState!=0`` () =
    let original : McCharacterActionMessage =
        { WorldId = 201L; PacketId = 6L
          Action = ActionLean { ActionLeanState = 1L; ActionPose = 0L; ActionAngle = 0L
                                ActionPosX = 0L; ActionPosY = 0L; ActionHeight = 0L } }
    let restored = roundtrip Serialize.mcCharacterActionMessage Deserialize.mcCharacterActionMessage original
    match restored.Action with
    | ActionLean d ->
        Assert.Equal(1L, d.ActionLeanState)
        // При leanState!=0 дополнительные поля не передаются
        Assert.Equal(0L, d.ActionPose)
    | _ -> Assert.Fail("Ожидался ActionLean")

[<Fact>]
let ``McCharacterAction FACE roundtrip`` () =
    let original : McCharacterActionMessage =
        { WorldId = 300L; PacketId = 7L
          Action = ActionFace { FaceAngle = 180L; FacePose = 2L } }
    let restored = roundtrip Serialize.mcCharacterActionMessage Deserialize.mcCharacterActionMessage original
    match restored.Action with
    | ActionFace d ->
        Assert.Equal(180L, d.FaceAngle)
        Assert.Equal(2L, d.FacePose)
    | _ -> Assert.Fail("Ожидался ActionFace")

[<Fact>]
let ``McCharacterAction SKILL_POSE roundtrip`` () =
    let original : McCharacterActionMessage =
        { WorldId = 301L; PacketId = 8L
          Action = ActionSkillPose { FaceAngle = 270L; FacePose = 5L } }
    let restored = roundtrip Serialize.mcCharacterActionMessage Deserialize.mcCharacterActionMessage original
    match restored.Action with
    | ActionSkillPose d ->
        Assert.Equal(270L, d.FaceAngle)
        Assert.Equal(5L, d.FacePose)
    | _ -> Assert.Fail("Ожидался ActionSkillPose")

[<Fact>]
let ``McCharacterAction ITEM_FAILED roundtrip`` () =
    let original : McCharacterActionMessage =
        { WorldId = 400L; PacketId = 9L
          Action = ActionItemFailed 42L }
    let restored = roundtrip Serialize.mcCharacterActionMessage Deserialize.mcCharacterActionMessage original
    match restored.Action with
    | ActionItemFailed fid -> Assert.Equal(42L, fid)
    | _ -> Assert.Fail("Ожидался ActionItemFailed")

[<Fact>]
let ``McCharacterAction TEMP roundtrip`` () =
    let original : McCharacterActionMessage =
        { WorldId = 500L; PacketId = 10L
          Action = ActionTemp (1001L, 2002L) }
    let restored = roundtrip Serialize.mcCharacterActionMessage Deserialize.mcCharacterActionMessage original
    match restored.Action with
    | ActionTemp (iid, pid) ->
        Assert.Equal(1001L, iid)
        Assert.Equal(2002L, pid)
    | _ -> Assert.Fail("Ожидался ActionTemp")

[<Fact>]
let ``McCharacterAction CHANGE_CHA roundtrip`` () =
    let original : McCharacterActionMessage =
        { WorldId = 600L; PacketId = 11L
          Action = ActionChangeCha 777L }
    let restored = roundtrip Serialize.mcCharacterActionMessage Deserialize.mcCharacterActionMessage original
    match restored.Action with
    | ActionChangeCha mid -> Assert.Equal(777L, mid)
    | _ -> Assert.Fail("Ожидался ActionChangeCha")

// ═══════════════════════════════════════════════════════════════
//  Гильдейские команды (CM/MC)
// ═══════════════════════════════════════════════════════════════

[<Fact>]
let ``CmGuildPutNameMessage roundtrip`` () =
    let original = { CmGuildPutNameMessage.Confirm = 1L; GuildName = "Pirates"; Passwd = "secret" }
    let restored = roundtrip Serialize.cmGuildPutNameMessage Deserialize.cmGuildPutNameMessage original
    Assert.Equal(original.Confirm, restored.Confirm)
    Assert.Equal(original.GuildName, restored.GuildName)
    Assert.Equal(original.Passwd, restored.Passwd)

[<Fact>]
let ``CmGuildTryForMessage roundtrip`` () =
    let original = { CmGuildTryForMessage.GuildId = 42L }
    let restored = roundtrip Serialize.cmGuildTryForMessage Deserialize.cmGuildTryForMessage original
    Assert.Equal(original.GuildId, restored.GuildId)

[<Fact>]
let ``CmGuildTryForCfmMessage roundtrip`` () =
    let original = { CmGuildTryForCfmMessage.Confirm = 1L }
    let restored = roundtrip Serialize.cmGuildTryForCfmMessage Deserialize.cmGuildTryForCfmMessage original
    Assert.Equal(original.Confirm, restored.Confirm)

[<Fact>]
let ``CmGuildApproveMessage roundtrip`` () =
    let original = { CmGuildApproveMessage.ChaId = 100L }
    let restored = roundtrip Serialize.cmGuildApproveMessage Deserialize.cmGuildApproveMessage original
    Assert.Equal(original.ChaId, restored.ChaId)

[<Fact>]
let ``CmGuildRejectMessage roundtrip`` () =
    let original = { CmGuildRejectMessage.ChaId = 200L }
    let restored = roundtrip Serialize.cmGuildRejectMessage Deserialize.cmGuildRejectMessage original
    Assert.Equal(original.ChaId, restored.ChaId)

[<Fact>]
let ``CmGuildKickMessage roundtrip`` () =
    let original = { CmGuildKickMessage.ChaId = 300L }
    let restored = roundtrip Serialize.cmGuildKickMessage Deserialize.cmGuildKickMessage original
    Assert.Equal(original.ChaId, restored.ChaId)

[<Fact>]
let ``CmGuildDisbandMessage roundtrip`` () =
    let original = { CmGuildDisbandMessage.Passwd = "disband123" }
    let restored = roundtrip Serialize.cmGuildDisbandMessage Deserialize.cmGuildDisbandMessage original
    Assert.Equal(original.Passwd, restored.Passwd)

[<Fact>]
let ``CmGuildMottoMessage roundtrip`` () =
    let original = { CmGuildMottoMessage.Motto = "Yo ho ho!" }
    let restored = roundtrip Serialize.cmGuildMottoMessage Deserialize.cmGuildMottoMessage original
    Assert.Equal(original.Motto, restored.Motto)

[<Fact>]
let ``CmGuildChallMessage roundtrip`` () =
    let original = { CmGuildChallMessage.Level = 2L; Money = 5000L }
    let restored = roundtrip Serialize.cmGuildChallMessage Deserialize.cmGuildChallMessage original
    Assert.Equal(original.Level, restored.Level)
    Assert.Equal(original.Money, restored.Money)

[<Fact>]
let ``CmGuildLeizhuMessage roundtrip`` () =
    let original = { CmGuildLeizhuMessage.Level = 1L; Money = 3000L }
    let restored = roundtrip Serialize.cmGuildLeizhuMessage Deserialize.cmGuildLeizhuMessage original
    Assert.Equal(original.Level, restored.Level)
    Assert.Equal(original.Money, restored.Money)

[<Fact>]
let ``McGuildTryForCfmMessage roundtrip`` () =
    let original = { McGuildTryForCfmMessage.Name = "OldGuild" }
    let restored = roundtrip Serialize.mcGuildTryForCfmMessage Deserialize.mcGuildTryForCfmMessage original
    Assert.Equal(original.Name, restored.Name)

[<Fact>]
let ``McGuildMottoMessage roundtrip`` () =
    let original = { McGuildMottoMessage.Motto = "For glory!" }
    let restored = roundtrip Serialize.mcGuildMottoMessage Deserialize.mcGuildMottoMessage original
    Assert.Equal(original.Motto, restored.Motto)

[<Fact>]
let ``McGuildInfoMessage roundtrip`` () =
    let original =
        { McGuildInfoMessage.CharId = 10L; GuildId = 20L
          GuildName = "SeaDogs"; GuildMotto = "Sail on!"; GuildPermission = 7L }
    let restored = roundtrip Serialize.mcGuildInfoMessage Deserialize.mcGuildInfoMessage original
    Assert.Equal(original.CharId, restored.CharId)
    Assert.Equal(original.GuildId, restored.GuildId)
    Assert.Equal(original.GuildName, restored.GuildName)
    Assert.Equal(original.GuildMotto, restored.GuildMotto)
    Assert.Equal(original.GuildPermission, restored.GuildPermission)

[<Fact>]
let ``McGuildListChallMessage roundtrip — все уровни заполнены`` () =
    let original : McGuildListChallMessage =
        { IsLeader = 1L
          Entries = [|
            { Level = 1L; Start = 10L; GuildName = "Alpha"; ChallName = "Beta"; Money = 1000L }
            { Level = 2L; Start = 20L; GuildName = "Gamma"; ChallName = "Delta"; Money = 2000L }
            { Level = 3L; Start = 30L; GuildName = "Epsilon"; ChallName = "Zeta"; Money = 3000L }
          |] }
    let restored = roundtrip Serialize.mcGuildListChallMessage Deserialize.mcGuildListChallMessage original
    Assert.Equal(original.IsLeader, restored.IsLeader)
    for i in 0..2 do
        Assert.Equal(original.Entries[i].Level, restored.Entries[i].Level)
        Assert.Equal(original.Entries[i].Start, restored.Entries[i].Start)
        Assert.Equal(original.Entries[i].GuildName, restored.Entries[i].GuildName)
        Assert.Equal(original.Entries[i].ChallName, restored.Entries[i].ChallName)
        Assert.Equal(original.Entries[i].Money, restored.Entries[i].Money)

[<Fact>]
let ``McGuildListChallMessage roundtrip — пустые уровни`` () =
    let original : McGuildListChallMessage =
        { IsLeader = 0L
          Entries = [|
            { Level = 0L; Start = 0L; GuildName = ""; ChallName = ""; Money = 0L }
            { Level = 1L; Start = 5L; GuildName = "Only"; ChallName = "One"; Money = 500L }
            { Level = 0L; Start = 0L; GuildName = ""; ChallName = ""; Money = 0L }
          |] }
    let restored = roundtrip Serialize.mcGuildListChallMessage Deserialize.mcGuildListChallMessage original
    Assert.Equal(0L, restored.Entries[0].Level)
    Assert.Equal(1L, restored.Entries[1].Level)
    Assert.Equal("Only", restored.Entries[1].GuildName)
    Assert.Equal(0L, restored.Entries[2].Level)

// ═══════════════════════════════════════════════════════════════
//  CMD_PC_GUILD — составное сообщение гильдии
// ═══════════════════════════════════════════════════════════════

/// Вспомогательная: пустая запись участника для тестов.
let emptyGuildMember : GuildMemberEntry =
    { Online = 0L; ChaId = 0L; ChaName = ""; Motto = ""; Job = ""; Degree = 0L; Icon = 0L; Permission = 0L }

[<Fact>]
let ``PcGuildMessage roundtrip — ONLINE`` () =
    let original : PcGuildMessage =
        { Msg = 4L; ChaId = 42L; PacketIndex = 0L
          GuildId = 0L; GuildName = ""; LeaderId = 0L
          Members = [||]; AddMember = emptyGuildMember }
    let restored = roundtrip Serialize.pcGuildMessage Deserialize.pcGuildMessage original
    Assert.Equal(4L, restored.Msg)
    Assert.Equal(42L, restored.ChaId)

[<Fact>]
let ``PcGuildMessage roundtrip — OFFLINE`` () =
    let original : PcGuildMessage =
        { Msg = 5L; ChaId = 99L; PacketIndex = 0L
          GuildId = 0L; GuildName = ""; LeaderId = 0L
          Members = [||]; AddMember = emptyGuildMember }
    let restored = roundtrip Serialize.pcGuildMessage Deserialize.pcGuildMessage original
    Assert.Equal(5L, restored.Msg)
    Assert.Equal(99L, restored.ChaId)

[<Fact>]
let ``PcGuildMessage roundtrip — DEL`` () =
    let original : PcGuildMessage =
        { Msg = 3L; ChaId = 77L; PacketIndex = 0L
          GuildId = 0L; GuildName = ""; LeaderId = 0L
          Members = [||]; AddMember = emptyGuildMember }
    let restored = roundtrip Serialize.pcGuildMessage Deserialize.pcGuildMessage original
    Assert.Equal(3L, restored.Msg)
    Assert.Equal(77L, restored.ChaId)

[<Fact>]
let ``PcGuildMessage roundtrip — STOP`` () =
    let original : PcGuildMessage =
        { Msg = 6L; ChaId = 0L; PacketIndex = 0L
          GuildId = 0L; GuildName = ""; LeaderId = 0L
          Members = [||]; AddMember = emptyGuildMember }
    let restored = roundtrip Serialize.pcGuildMessage Deserialize.pcGuildMessage original
    Assert.Equal(6L, restored.Msg)

[<Fact>]
let ``PcGuildMessage roundtrip — ADD`` () =
    let addEntry : GuildMemberEntry =
        { Online = 1L; ChaId = 10L; ChaName = "Pirate"; Motto = "Yarr!"
          Job = "Captain"; Degree = 5L; Icon = 3L; Permission = 7L }
    let original : PcGuildMessage =
        { Msg = 2L; ChaId = 0L; PacketIndex = 0L
          GuildId = 0L; GuildName = ""; LeaderId = 0L
          Members = [||]; AddMember = addEntry }
    let restored = roundtrip Serialize.pcGuildMessage Deserialize.pcGuildMessage original
    Assert.Equal(2L, restored.Msg)
    Assert.Equal(1L, restored.AddMember.Online)
    Assert.Equal(10L, restored.AddMember.ChaId)
    Assert.Equal("Pirate", restored.AddMember.ChaName)
    Assert.Equal("Yarr!", restored.AddMember.Motto)
    Assert.Equal("Captain", restored.AddMember.Job)
    Assert.Equal(5L, restored.AddMember.Degree)
    Assert.Equal(3L, restored.AddMember.Icon)
    Assert.Equal(7L, restored.AddMember.Permission)

[<Fact>]
let ``PcGuildMessage roundtrip — START первый пакет с заголовком`` () =
    let members = [|
        { Online = 1L; ChaId = 10L; ChaName = "Alpha"; Motto = "Go!"; Job = "Warrior"; Degree = 5L; Icon = 1L; Permission = 15L }
        { Online = 0L; ChaId = 20L; ChaName = "Beta"; Motto = "Wait"; Job = "Mage"; Degree = 3L; Icon = 2L; Permission = 3L }
    |]
    let original : PcGuildMessage =
        { Msg = 1L; ChaId = 0L; PacketIndex = 0L
          GuildId = 100L; GuildName = "SeaDogs"; LeaderId = 10L
          Members = members; AddMember = emptyGuildMember }
    let restored = roundtrip Serialize.pcGuildMessage Deserialize.pcGuildMessage original
    Assert.Equal(1L, restored.Msg)
    Assert.Equal(0L, restored.PacketIndex)
    Assert.Equal(100L, restored.GuildId)
    Assert.Equal("SeaDogs", restored.GuildName)
    Assert.Equal(10L, restored.LeaderId)
    Assert.Equal(2, restored.Members.Length)
    Assert.Equal(1L, restored.Members[0].Online)
    Assert.Equal(10L, restored.Members[0].ChaId)
    Assert.Equal("Alpha", restored.Members[0].ChaName)
    Assert.Equal(0L, restored.Members[1].Online)
    Assert.Equal(20L, restored.Members[1].ChaId)
    Assert.Equal("Beta", restored.Members[1].ChaName)
    Assert.Equal("Mage", restored.Members[1].Job)

[<Fact>]
let ``PcGuildMessage roundtrip — START второй пакет без заголовка`` () =
    let members = [|
        { Online = 1L; ChaId = 30L; ChaName = "Gamma"; Motto = ""; Job = "Thief"; Degree = 1L; Icon = 0L; Permission = 1L }
    |]
    let original : PcGuildMessage =
        { Msg = 1L; ChaId = 0L; PacketIndex = 1L
          GuildId = 0L; GuildName = ""; LeaderId = 0L
          Members = members; AddMember = emptyGuildMember }
    let restored = roundtrip Serialize.pcGuildMessage Deserialize.pcGuildMessage original
    Assert.Equal(1L, restored.Msg)
    Assert.Equal(1L, restored.PacketIndex)
    // Заголовок не сериализуется при packetIndex>0
    Assert.Equal(0L, restored.GuildId)
    Assert.Equal(1, restored.Members.Length)
    Assert.Equal(30L, restored.Members[0].ChaId)
    Assert.Equal("Gamma", restored.Members[0].ChaName)

[<Fact>]
let ``PcGuildMessage roundtrip — START пустой пакет (count=0)`` () =
    let original : PcGuildMessage =
        { Msg = 1L; ChaId = 0L; PacketIndex = 0L
          GuildId = 0L; GuildName = ""; LeaderId = 0L
          Members = [||]; AddMember = emptyGuildMember }
    let restored = roundtrip Serialize.pcGuildMessage Deserialize.pcGuildMessage original
    Assert.Equal(1L, restored.Msg)
    Assert.Equal(0L, restored.PacketIndex)
    Assert.Equal(0, restored.Members.Length)

// ═══════════════════════════════════════════════════════════════
//  Логи банка гильдии (UPDATEGUILDLOGS / REQUESTGUILDLOGS)
// ═══════════════════════════════════════════════════════════════

[<Fact>]
let ``McUpdateGuildLogsMessage roundtrip — с логами и стоппером`` () =
    let original : McUpdateGuildLogsMessage =
        { WorldId = 42L; PacketId = 7L; TotalSize = 100L
          Logs = [|
            { Type = 1L; Time = 1000L; Parameter = 50L; Quantity = 3L; UserId = 99L }
            { Type = 2L; Time = 2000L; Parameter = 60L; Quantity = 1L; UserId = 77L }
          |]
          Terminated = true }
    let restored = roundtrip Serialize.mcUpdateGuildLogsMessage Deserialize.mcUpdateGuildLogsMessage original
    Assert.Equal(original.WorldId, restored.WorldId)
    Assert.Equal(original.PacketId, restored.PacketId)
    Assert.Equal(original.TotalSize, restored.TotalSize)
    Assert.Equal(2, restored.Logs.Length)
    Assert.Equal(1L, restored.Logs[0].Type)
    Assert.Equal(1000L, restored.Logs[0].Time)
    Assert.Equal(50L, restored.Logs[0].Parameter)
    Assert.Equal(3L, restored.Logs[0].Quantity)
    Assert.Equal(99L, restored.Logs[0].UserId)
    Assert.Equal(2L, restored.Logs[1].Type)
    Assert.Equal(2000L, restored.Logs[1].Time)
    Assert.Equal(60L, restored.Logs[1].Parameter)
    Assert.Equal(1L, restored.Logs[1].Quantity)
    Assert.Equal(77L, restored.Logs[1].UserId)
    Assert.True(restored.Terminated)

[<Fact>]
let ``McUpdateGuildLogsMessage roundtrip — без стоппера`` () =
    let original : McUpdateGuildLogsMessage =
        { WorldId = 10L; PacketId = 1L; TotalSize = 50L
          Logs = [|
            { Type = 3L; Time = 3000L; Parameter = 70L; Quantity = 5L; UserId = 111L }
          |]
          Terminated = false }
    let restored = roundtrip Serialize.mcUpdateGuildLogsMessage Deserialize.mcUpdateGuildLogsMessage original
    Assert.Equal(original.WorldId, restored.WorldId)
    Assert.Equal(original.TotalSize, restored.TotalSize)
    Assert.Equal(1, restored.Logs.Length)
    Assert.Equal(3L, restored.Logs[0].Type)
    Assert.Equal(3000L, restored.Logs[0].Time)
    Assert.False(restored.Terminated)

[<Fact>]
let ``McUpdateGuildLogsMessage roundtrip — пустой список логов`` () =
    let original : McUpdateGuildLogsMessage =
        { WorldId = 5L; PacketId = 0L; TotalSize = 0L; Logs = [||]; Terminated = false }
    let restored = roundtrip Serialize.mcUpdateGuildLogsMessage Deserialize.mcUpdateGuildLogsMessage original
    Assert.Equal(0, restored.Logs.Length)
    Assert.False(restored.Terminated)

[<Fact>]
let ``McRequestGuildLogsMessage roundtrip — с логами и стоппером`` () =
    let original : McRequestGuildLogsMessage =
        { WorldId = 100L; PacketId = 3L
          Logs = [|
            { Type = 5L; Time = 5000L; Parameter = 80L; Quantity = 2L; UserId = 55L }
          |]
          Terminated = true }
    let restored = roundtrip Serialize.mcRequestGuildLogsMessage Deserialize.mcRequestGuildLogsMessage original
    Assert.Equal(original.WorldId, restored.WorldId)
    Assert.Equal(original.PacketId, restored.PacketId)
    Assert.Equal(1, restored.Logs.Length)
    Assert.Equal(5L, restored.Logs[0].Type)
    Assert.Equal(5000L, restored.Logs[0].Time)
    Assert.Equal(80L, restored.Logs[0].Parameter)
    Assert.Equal(2L, restored.Logs[0].Quantity)
    Assert.Equal(55L, restored.Logs[0].UserId)
    Assert.True(restored.Terminated)

[<Fact>]
let ``McRequestGuildLogsMessage roundtrip — без стоппера`` () =
    let original : McRequestGuildLogsMessage =
        { WorldId = 20L; PacketId = 2L; Logs = [||]; Terminated = false }
    let restored = roundtrip Serialize.mcRequestGuildLogsMessage Deserialize.mcRequestGuildLogsMessage original
    Assert.Equal(20L, restored.WorldId)
    Assert.Equal(0, restored.Logs.Length)
    Assert.False(restored.Terminated)

// ═══════════════════════════════════════════════════════════════
//  Торговля предметами (McCharTradeItemMessage — единое сообщение)
// ═══════════════════════════════════════════════════════════════

[<Fact>]
let ``McCharTradeItemMessage roundtrip — удаление предмета (Remove)`` () =
    let original : McCharTradeItemMessage =
        { MainChaId = 1001L; OpType = 3L // TRADE_DRAGTO_ITEM
          Data = Remove { BagIndex = 5L; TradeIndex = 3L; Count = 10L } }
    let restored = roundtrip Serialize.mcCharTradeItemMessage Deserialize.mcCharTradeItemMessage original
    Assert.Equal(original.MainChaId, restored.MainChaId)
    Assert.Equal(original.OpType, restored.OpType)
    match restored.Data with
    | Remove rem ->
        Assert.Equal(5L, rem.BagIndex)
        Assert.Equal(3L, rem.TradeIndex)
        Assert.Equal(10L, rem.Count)
    | Add _ -> Assert.Fail("Ожидался вариант Remove")

[<Fact>]
let ``McCharTradeItemMessage roundtrip — обычный предмет без instAttr`` () =
    let item : TradeItemData =
        { Endure0 = 100L; Endure1 = 90L; Energy0 = 50L; Energy1 = 40L
          ForgeLv = 3L; Valid = 1L; Tradable = 1L; Expiration = 9999L
          ForgeParam = 7L; InstId = 555L
          HasInstAttr = false; InstAttr = [||] }
    let original : McCharTradeItemMessage =
        { MainChaId = 2001L; OpType = 4L // TRADE_DRAGTO_TRADE
          Data = Add { ItemId = 300L; BagIndex = 2L; TradeIndex = 0L
                       Count = 5L; ItemType = 1L; EquipData = Choice2Of2 item } }
    let restored = roundtrip Serialize.mcCharTradeItemMessage Deserialize.mcCharTradeItemMessage original
    Assert.Equal(original.MainChaId, restored.MainChaId)
    Assert.Equal(original.OpType, restored.OpType)
    match restored.Data with
    | Add add ->
        Assert.Equal(300L, add.ItemId)
        Assert.Equal(2L, add.BagIndex)
        Assert.Equal(0L, add.TradeIndex)
        Assert.Equal(5L, add.Count)
        Assert.Equal(1L, add.ItemType)
        match add.EquipData with
        | Choice2Of2 restoredItem ->
            Assert.Equal(100L, restoredItem.Endure0)
            Assert.Equal(90L, restoredItem.Endure1)
            Assert.Equal(50L, restoredItem.Energy0)
            Assert.Equal(40L, restoredItem.Energy1)
            Assert.Equal(3L, restoredItem.ForgeLv)
            Assert.Equal(1L, restoredItem.Valid)
            Assert.Equal(1L, restoredItem.Tradable)
            Assert.Equal(9999L, restoredItem.Expiration)
            Assert.Equal(7L, restoredItem.ForgeParam)
            Assert.Equal(555L, restoredItem.InstId)
            Assert.False(restoredItem.HasInstAttr)
        | Choice1Of2 _ -> Assert.Fail("Ожидался Choice2Of2 (предмет)")
    | Remove _ -> Assert.Fail("Ожидался вариант Add")

[<Fact>]
let ``McCharTradeItemMessage roundtrip — обычный предмет с instAttr`` () =
    let attrs = [| (10L, 20L); (30L, 40L); (50L, 60L); (70L, 80L); (90L, 100L) |]
    let item : TradeItemData =
        { Endure0 = 200L; Endure1 = 180L; Energy0 = 60L; Energy1 = 55L
          ForgeLv = 5L; Valid = 1L; Tradable = 0L; Expiration = 5000L
          ForgeParam = 11L; InstId = 777L
          HasInstAttr = true; InstAttr = attrs }
    let original : McCharTradeItemMessage =
        { MainChaId = 3001L; OpType = 4L // TRADE_DRAGTO_TRADE
          Data = Add { ItemId = 400L; BagIndex = 3L; TradeIndex = 1L
                       Count = 1L; ItemType = 2L; EquipData = Choice2Of2 item } }
    let restored = roundtrip Serialize.mcCharTradeItemMessage Deserialize.mcCharTradeItemMessage original
    Assert.Equal(original.MainChaId, restored.MainChaId)
    match restored.Data with
    | Add add ->
        Assert.Equal(2L, add.ItemType)
        match add.EquipData with
        | Choice2Of2 restoredItem ->
            Assert.True(restoredItem.HasInstAttr)
            Assert.Equal(ITEM_INSTANCE_ATTR_NUM, restoredItem.InstAttr.Length)
            Assert.Equal((10L, 20L), restoredItem.InstAttr[0])
            Assert.Equal((30L, 40L), restoredItem.InstAttr[1])
            Assert.Equal((50L, 60L), restoredItem.InstAttr[2])
            Assert.Equal((70L, 80L), restoredItem.InstAttr[3])
            Assert.Equal((90L, 100L), restoredItem.InstAttr[4])
            Assert.Equal(200L, restoredItem.Endure0)
            Assert.Equal(777L, restoredItem.InstId)
        | Choice1Of2 _ -> Assert.Fail("Ожидался Choice2Of2 (предмет)")
    | Remove _ -> Assert.Fail("Ожидался вариант Add")

[<Fact>]
let ``McCharTradeItemMessage roundtrip — корабль (hasBoat=true)`` () =
    let boat : TradeBoatData =
        { HasBoat = true; Name = "BlackPearl"
          Ship = 1L; Lv = 10L; Cexp = 500L
          HP = 1000L; MxHP = 1500L; SP = 200L; MxSP = 300L
          MnAtk = 50L; MxAtk = 100L; Def = 30L
          MSpd = 15L; ASpd = 8L
          UseGridNum = 4L; Capacity = 20L; Price = 99999L }
    let original : McCharTradeItemMessage =
        { MainChaId = 4001L; OpType = 4L // TRADE_DRAGTO_TRADE
          Data = Add { ItemId = 500L; BagIndex = 0L; TradeIndex = 2L
                       Count = 1L; ItemType = ITEM_TYPE_BOAT; EquipData = Choice1Of2 boat } }
    let restored = roundtrip Serialize.mcCharTradeItemMessage Deserialize.mcCharTradeItemMessage original
    Assert.Equal(original.MainChaId, restored.MainChaId)
    match restored.Data with
    | Add add ->
        Assert.Equal(ITEM_TYPE_BOAT, add.ItemType)
        match add.EquipData with
        | Choice1Of2 restoredBoat ->
            Assert.True(restoredBoat.HasBoat)
            Assert.Equal("BlackPearl", restoredBoat.Name)
            Assert.Equal(1L, restoredBoat.Ship)
            Assert.Equal(10L, restoredBoat.Lv)
            Assert.Equal(500L, restoredBoat.Cexp)
            Assert.Equal(1000L, restoredBoat.HP)
            Assert.Equal(1500L, restoredBoat.MxHP)
            Assert.Equal(200L, restoredBoat.SP)
            Assert.Equal(300L, restoredBoat.MxSP)
            Assert.Equal(50L, restoredBoat.MnAtk)
            Assert.Equal(100L, restoredBoat.MxAtk)
            Assert.Equal(30L, restoredBoat.Def)
            Assert.Equal(15L, restoredBoat.MSpd)
            Assert.Equal(8L, restoredBoat.ASpd)
            Assert.Equal(4L, restoredBoat.UseGridNum)
            Assert.Equal(20L, restoredBoat.Capacity)
            Assert.Equal(99999L, restoredBoat.Price)
        | Choice2Of2 _ -> Assert.Fail("Ожидался Choice1Of2 (корабль)")
    | Remove _ -> Assert.Fail("Ожидался вариант Add")

[<Fact>]
let ``McCharTradeItemMessage roundtrip — корабль (hasBoat=false)`` () =
    let boat : TradeBoatData =
        { HasBoat = false; Name = ""; Ship = 0L; Lv = 0L; Cexp = 0L
          HP = 0L; MxHP = 0L; SP = 0L; MxSP = 0L
          MnAtk = 0L; MxAtk = 0L; Def = 0L; MSpd = 0L; ASpd = 0L
          UseGridNum = 0L; Capacity = 0L; Price = 0L }
    let original : McCharTradeItemMessage =
        { MainChaId = 5001L; OpType = 4L // TRADE_DRAGTO_TRADE
          Data = Add { ItemId = 600L; BagIndex = 0L; TradeIndex = 0L
                       Count = 1L; ItemType = ITEM_TYPE_BOAT; EquipData = Choice1Of2 boat } }
    let restored = roundtrip Serialize.mcCharTradeItemMessage Deserialize.mcCharTradeItemMessage original
    match restored.Data with
    | Add add ->
        match add.EquipData with
        | Choice1Of2 restoredBoat -> Assert.False(restoredBoat.HasBoat)
        | Choice2Of2 _ -> Assert.Fail("Ожидался Choice1Of2 (корабль)")
    | Remove _ -> Assert.Fail("Ожидался вариант Add")
