namespace Corsairs.Platform.Network.Protocol

/// Типизированные структуры команд + функции serialize/deserialize.
/// Все поля сериализуются в прямом порядке (только ReadInt64/ReadString/WriteInt64/WriteString).
/// Trailer-поля (gateAddr, gpAddr и т.д.) включены как обычные поля в конце структуры.
module CommandMessages =

    // ═══════════════════════════════════════════════════════════════
    //  Вспомогательные типы
    // ═══════════════════════════════════════════════════════════════

    /// Количество слотов экипировки (C++: enumEQUIP_NUM).
    [<Literal>]
    let EQUIP_NUM = 34

    /// Слот персонажа на экране выбора.
    /// EquipIds — ID внешнего вида экипировки (C++: Look_Minimal.equip_IDs[34]).
    type ChaSlotData =
        { Valid: bool; ChaName: string; Job: string; Degree: int64; TypeId: int64
          EquipIds: int64[] }

    /// Запись в списке игроков (TpReqPlyLst).
    [<Struct>]
    type ReqPlyEntry = { GtAddr: int64; ChaId: int64 }

    /// Данные синхронизации игрока (TpSyncPlyLst).
    [<Struct>]
    type SyncPlayerData = { GtAddr: int64; AcctLoginId: int64; AcctId: int64 }

    /// Результат синхронизации одного игрока.
    [<Struct>]
    type SyncResultData = { Ok: bool; PlayerPtr: int64 }

    /// Участник команды (PcTeamRefresh).
    type TeamMemberData =
        { ChaId: int64; ChaName: string; Motto: string; Icon: int64 }

    /// Участник сессии чата (PcSessCreate).
    type SessMemberData =
        { ChaId: int64; ChaName: string; Motto: string; Icon: int64 }

    /// Участник команды (PmTeam → GameServer).
    type PmTeamMemberData =
        { GateName: string; GtAddr: int64; ChaId: int64 }

    // ═══════════════════════════════════════════════════════════════
    //  Группа A: GateServer ↔ GroupServer (TP/PT) — RPC
    // ═══════════════════════════════════════════════════════════════

    [<Struct>]
    type TpLoginRequest = { ProtocolVersion: int16; GateName: string }

    [<Struct>]
    type TpLoginResponse = { ErrCode: int16 }

    type TpUserLoginRequest =
        { AcctName: string; AcctPassword: string; AcctMac: string
          ClientIp: uint32; GateAddr: uint32; CheatMarker: int64 }

    type TpUserLoginResponseData =
        { MaxChaNum: int64; Characters: ChaSlotData[]
          HasPassword2: bool; AcctId: int64; AcctLoginId: int64; GpAddr: uint32 }

    type TpUserLoginResponse =
        | TpUserLoginSuccess of TpUserLoginResponseData
        | TpUserLoginError of errCode: int16

    [<Struct>]
    type TpUserLogoutRequest = { GateAddr: uint32; GpAddr: uint32 }

    [<Struct>]
    type TpUserLogoutResponse = { ErrCode: int16 }

    type TpBgnPlayRequest = { ChaIndex: int64; GateAddr: uint32; GpAddr: uint32 }

    type TpBgnPlayResponseData =
        { Password2: string; ChaId: int64; WorldId: int64; MapName: string; Swiner: int64 }

    type TpBgnPlayResponse =
        { ErrCode: int16; Data: TpBgnPlayResponseData voption }

    [<Struct>]
    type TpEndPlayRequest = { GateAddr: uint32; GpAddr: uint32 }

    type TpEndPlayResponseData =
        { MaxChaNum: int64; ChaNum: int64; Characters: ChaSlotData[] }

    type TpEndPlayResponse =
        { ErrCode: int16; Data: TpEndPlayResponseData voption }

    type TpNewChaRequest =
        { ChaName: string; Birth: string; TypeId: int64; HairId: int64; FaceId: int64
          GateAddr: uint32; GpAddr: uint32 }

    [<Struct>]
    type TpNewChaResponse = { ErrCode: int16 }

    type TpDelChaRequest =
        { ChaIndex: int64; Password2: string; GateAddr: uint32; GpAddr: uint32 }

    [<Struct>]
    type TpDelChaResponse = { ErrCode: int16 }

    type TpChangePassRequest =
        { NewPass: string; Pin: string; GateAddr: uint32; GpAddr: uint32 }

    type TpRegisterRequest = { Username: string; Password: string; Email: string }

    type TpRegisterResponse = { Result: int64; Message: string }

    type TpCreatePassword2Request =
        { Password2: string; GateAddr: uint32; GpAddr: uint32 }

    type TpUpdatePassword2Request =
        { OldPassword2: string; NewPassword2: string; GateAddr: uint32; GpAddr: uint32 }

    [<Struct>]
    type TpPassword2Response = { ErrCode: int16 }

    type TpReqPlyLstResponse = { Entries: ReqPlyEntry[]; PlyNum: int64 }

    type TpSyncPlyLstRequest =
        { Num: int64; GateName: string; Players: SyncPlayerData[] }

    type TpSyncPlyLstResponse =
        { ErrCode: int16; Num: int64; Results: SyncResultData[] }

    [<Struct>]
    type OsLoginRequest = { Version: int64; AgentName: string }

    [<Struct>]
    type OsLoginResponse = { ErrCode: int16 }

    // Группа A2: Async TP
    type TpDiscMessage = { ActId: int64; IpAddr: int64; Reason: string }

    // ═══════════════════════════════════════════════════════════════
    //  Группа B: GroupServer ↔ AccountServer (PA/AP)
    // ═══════════════════════════════════════════════════════════════

    type PaLoginRequest = { ServerName: string; ServerPassword: string }

    [<Struct>]
    type PaLoginResponse = { ErrCode: int16 }

    type PaUserLoginRequest =
        { Username: string; Password: string; Mac: string; ClientIp: int64 }

    type PaUserLoginResponseData = { AcctLoginId: int64; SessId: int64; AcctId: int64; GmLevel: int64 }

    type PaUserLoginResponse =
        | PaUserLoginSuccess of PaUserLoginResponseData
        | PaUserLoginError of errCode: int16

    [<Struct>]
    type PaUserLogoutMessage = { AcctLoginId: int64 }

    type PaUserBillBgnMessage = { AcctName: string; Passport: string }

    [<Struct>]
    type PaUserBillEndMessage = { AcctName: string }

    type PaChangePassMessage = { Username: string; NewPassword: string }

    type PaRegisterMessage = { Username: string; Password: string; Email: string }

    [<Struct>]
    type PaGmBanMessage = { ActName: string }

    [<Struct>]
    type PaGmUnbanMessage = { ActName: string }

    [<Struct>]
    type PaUserDisableMessage = { AcctLoginId: int64; Minutes: int64 }

    [<Struct>]
    type ApKickUserMessage = { ErrCode: int64; AccountId: int64 }

    [<Struct>]
    type ApExpScaleMessage = { ChaId: int64; Time: int64 }

    // ═══════════════════════════════════════════════════════════════
    //  Группа C: Client → GroupServer (CP) — async
    // ═══════════════════════════════════════════════════════════════

    type CpTeamInviteMessage = { InvitedName: string; GateAddr: uint32; GpAddr: uint32 }

    [<Struct>]
    type CpTeamAcceptMessage = { InviterChaId: int64; GateAddr: uint32; GpAddr: uint32 }

    [<Struct>]
    type CpTeamRefuseMessage = { InviterChaId: int64; GateAddr: uint32; GpAddr: uint32 }

    [<Struct>]
    type CpTeamLeaveMessage = { GateAddr: uint32; GpAddr: uint32 }

    [<Struct>]
    type CpTeamKickMessage = { KickedChaId: int64; GateAddr: uint32; GpAddr: uint32 }

    type CpFrndInviteMessage = { InvitedName: string; GateAddr: uint32; GpAddr: uint32 }

    [<Struct>]
    type CpFrndAcceptMessage = { InviterChaId: int64; GateAddr: uint32; GpAddr: uint32 }

    [<Struct>]
    type CpFrndRefuseMessage = { InviterChaId: int64; GateAddr: uint32; GpAddr: uint32 }

    [<Struct>]
    type CpFrndDeleteMessage = { DeletedChaId: int64; GateAddr: uint32; GpAddr: uint32 }

    type CpFrndChangeGroupMessage =
        { FriendChaId: int64; GroupName: string; GateAddr: uint32; GpAddr: uint32 }

    [<Struct>]
    type CpFrndRefreshInfoMessage = { FriendChaId: int64; GateAddr: uint32; GpAddr: uint32 }

    type CpSay2AllMessage = { Content: string; GateAddr: uint32; GpAddr: uint32 }

    type CpSay2TradeMessage = { Content: string; GateAddr: uint32; GpAddr: uint32 }

    type CpSay2YouMessage =
        { TargetName: string; Content: string; GateAddr: uint32; GpAddr: uint32 }

    type CpSay2TemMessage = { Content: string; GateAddr: uint32; GpAddr: uint32 }

    type CpSay2GudMessage = { Content: string; GateAddr: uint32; GpAddr: uint32 }

    type CpGm1SayMessage = { Content: string; GateAddr: uint32; GpAddr: uint32 }

    type CpGm1Say1Message =
        { Content: string; Color: int64; GateAddr: uint32; GpAddr: uint32 }

    type CpSessCreateMessage =
        { ChaNum: int64; ChaNames: string[]; GateAddr: uint32; GpAddr: uint32 }

    type CpSessAddMessage =
        { SessId: int64; ChaName: string; GateAddr: uint32; GpAddr: uint32 }

    type CpSessSayMessage =
        { SessId: int64; Content: string; GateAddr: uint32; GpAddr: uint32 }

    [<Struct>]
    type CpSessLeaveMessage = { SessId: int64; GateAddr: uint32; GpAddr: uint32 }

    type CpChangePersonInfoMessage =
        { Motto: string; Icon: int64; RefuseSess: int64; GateAddr: uint32; GpAddr: uint32 }

    [<Struct>]
    type CpPingMessage = { PingValue: int64; GateAddr: uint32; GpAddr: uint32 }

    [<Struct>]
    type CpRefuseToMeMessage = { RefuseFlag: int64; GateAddr: uint32; GpAddr: uint32 }

    [<Struct>]
    type CpReportWgMessage = { GateAddr: uint32; GpAddr: uint32 }

    [<Struct>]
    type CpMasterRefreshInfoMessage = { MasterChaId: int64; GateAddr: uint32; GpAddr: uint32 }

    [<Struct>]
    type CpPrenticeRefreshInfoMessage = { PrenticeChaId: int64; GateAddr: uint32; GpAddr: uint32 }

    type CpChangePassMessage =
        { NewPass: string; Pin: string; GateAddr: uint32; GpAddr: uint32 }

    // ═══════════════════════════════════════════════════════════════
    //  Группа D: GameServer → GroupServer (MP) — async
    // ═══════════════════════════════════════════════════════════════

    [<Struct>]
    type MpEnterMapMessage = { IsSwitch: int64; GateAddr: uint32; GpAddr: uint32 }

    type MpTeamCreateMessage =
        { MemberName: string; LeaderName: string; GateAddr: uint32; GpAddr: uint32 }

    type MpGuildCreateMessage =
        { GuildId: int64; GldName: string; Job: string; Degree: int64
          GateAddr: uint32; GpAddr: uint32 }

    [<Struct>]
    type MpGuildApproveMessage = { NewMemberChaId: int64; GateAddr: uint32; GpAddr: uint32 }

    [<Struct>]
    type MpGuildKickMessage = { KickedChaId: int64; GateAddr: uint32; GpAddr: uint32 }

    [<Struct>]
    type MpGuildLeaveMessage = { GateAddr: uint32; GpAddr: uint32 }

    [<Struct>]
    type MpGuildDisbandMessage = { GateAddr: uint32; GpAddr: uint32 }

    type MpGuildMottoMessage = { Motto: string; GateAddr: uint32; GpAddr: uint32 }

    [<Struct>]
    type MpGuildPermMessage =
        { TargetChaId: int64; Permission: int64; GateAddr: uint32; GpAddr: uint32 }

    type MpGuildChallMoneyMessage =
        { GuildId: int64; Money: int64; GuildName1: string; GuildName2: string
          GateAddr: uint32; GpAddr: uint32 }

    [<Struct>]
    type MpGuildChallPrizeMoneyMessage = { GuildId: int64; Money: int64 }

    type MpMasterCreateMessage =
        { PrenticeName: string; PrenticeChaid: int64; MasterName: string; MasterChaid: int64 }

    type MpMasterDelMessage =
        { PrenticeName: string; PrenticeChaid: int64; MasterName: string; MasterChaid: int64 }

    [<Struct>]
    type MpMasterFinishMessage = { PrenticeChaid: int64 }

    type MpSay2AllMessage =
        { Succ: int64; ChaName: string; Content: string; GateAddr: uint32; GpAddr: uint32 }

    type MpSay2TradeMessage =
        { Succ: int64; ChaName: string; Content: string; GateAddr: uint32; GpAddr: uint32 }

    type MpGm1SayMessage = { Content: string; GateAddr: uint32; GpAddr: uint32 }

    type MpGm1Say1Message = { Content: string; SetNum: int64; Color: int64 }

    type MpGmBanMessage = { ActName: string; GateAddr: uint32; GpAddr: uint32 }

    type MpGmUnbanMessage = { ActName: string; GateAddr: uint32; GpAddr: uint32 }

    type MpGuildNoticeMessage = { GuildId: int64; Content: string }

    [<Struct>]
    type MpCanReceiveRequestsMessage = { ChaId: int64; CanSend: int64 }

    type MpMutePlayerMessage =
        { ChaName: string; Time: int64; GateAddr: uint32; GpAddr: uint32 }

    type MpGarner2UpdateMessage =
        { Nid: int64; ChaName: string; Level: int64; Job: string; Fightpoint: int64
          GateAddr: uint32; GpAddr: uint32 }

    [<Struct>]
    type MpGarner2GetOrderMessage = { GateAddr: uint32; GpAddr: uint32 }

    [<Struct>]
    type MpGuildBankAckMessage = { GuildId: int64; GateAddr: uint32; GpAddr: uint32 }

    // ═══════════════════════════════════════════════════════════════
    //  Группа E: GroupServer → Client (PC) — async
    // ═══════════════════════════════════════════════════════════════

    type PcTeamInviteMessage = { InviterName: string; ChaId: int64; Icon: int64 }

    type PcTeamRefreshMessage =
        { Msg: int64; Count: int64; Members: TeamMemberData[] }

    type PcFrndInviteMessage = { InviterName: string; ChaId: int64; Icon: int64 }

    type PcFrndRefreshMessage =
        { Msg: int64; Group: string; ChaId: int64; ChaName: string; Motto: string; Icon: int64 }

    [<Struct>]
    type PcFrndRefreshDelMessage = { Msg: int64; ChaId: int64 }

    type PcFrndChangeGroupMessage = { FriendChaId: int64; GroupName: string }

    type PcFrndRefreshInfoMessage =
        { ChaId: int64; Motto: string; Icon: int64; Degree: int64; Job: string; GuildName: string }

    type PcSay2AllMessage = { ChaName: string; Content: string; Color: int64 }

    type PcSay2TradeMessage = { ChaName: string; Content: string; Color: int64 }

    type PcSay2YouMessage =
        { Sender: string; Target: string; Content: string; Color: int64 }

    type PcSay2TemMessage = { ChaId: int64; Content: string; Color: int64 }

    type PcSay2GudMessage = { ChaName: string; Content: string; Color: int64 }

    type PcGm1SayMessage = { ChaName: string; Content: string }

    type PcGm1Say1Message = { Content: string; SetNum: int64; Color: int64 }

    [<Struct>]
    type PcGuildPermMessage = { TargetChaId: int64; Permission: int64 }

    type PcMasterRefreshAddMessage =
        { Msg: int64; Group: string; ChaId: int64; ChaName: string; Motto: string; Icon: int64 }

    [<Struct>]
    type PcMasterRefreshDelMessage = { Msg: int64; ChaId: int64 }

    type PcSessCreateMessage =
        { SessId: int64; Members: SessMemberData[]; NotiPlyCount: int64 }

    type PcSessAddMessage =
        { SessId: int64; ChaId: int64; ChaName: string; Motto: string; Icon: int64 }

    type PcSessSayMessage = { SessId: int64; ChaId: int64; Content: string }

    [<Struct>]
    type PcSessLeaveMessage = { SessId: int64; ChaId: int64 }

    type PcChangePersonInfoMessage = { Motto: string; Icon: int64; RefuseSess: int64 }

    type PcErrMsgMessage = { Message: string }

    type PcGuildNoticeMessage = { Content: string }

    type PcRegisterMessage = { Result: int64; Message: string }

    /// CMD_PC_GARNER2_ORDER — рейтинг Top-5 (GroupServer → Client).
    type PcGarner2OrderEntry =
        { Name: string; Level: int64; Job: string; FightPoint: int64 }

    type PcGarner2OrderMessage = { Entries: PcGarner2OrderEntry[] }

    // ═══════════════════════════════════════════════════════════════
    //  Группа F: GroupServer → GameServer (PM) — async
    // ═══════════════════════════════════════════════════════════════

    type PmTeamMessage = { Msg: int64; Count: int64; Members: PmTeamMemberData[] }

    /// CMD_PM_GARNER2_UPDATE — обновление рейтинга (GroupServer → GateServer → GameServer).
    type PmGarner2UpdateMessage = { Nids: int64[]; OldIndex: int64 }

    [<Struct>]
    type PmExpScaleMessage = { ChaId: int64; Time: int64 }

    type PmSay2AllMessage = { ChaId: int64; Content: string; Money: int64 }

    type PmSay2TradeMessage = { ChaId: int64; Content: string; Money: int64 }

    [<Struct>]
    type PmGuildDisbandMessage = { GuildId: int64 }

    type PmGuildChallMoneyMessage =
        { LeaderId: int64; Money: int64; GuildName1: string; GuildName2: string }

    [<Struct>]
    type PmGuildChallPrizeMoneyMessage = { LeaderId: int64; Money: int64 }

    [<Struct>]
    type PtKickUserMessage = { GpAddr: int64; GtAddr: int64 }

    // ═══════════════════════════════════════════════════════════════
    //  Группа G: Client → GateServer (CM) — логин/выбор персонажа
    // ═══════════════════════════════════════════════════════════════

    type CmLoginRequest =
        { AcctName: string; PasswordHash: string; Mac: string
          CheatMarker: int64; ClientVersion: int64 }

    type McLoginResponseData =
        { MaxChaNum: int64; Characters: ChaSlotData[]; HasPassword2: bool }

    type McLoginResponse =
        | McLoginSuccess of McLoginResponseData
        | McLoginError of errCode: int16

    /// CMD_MC_BGNPLAY ответ (GateServer → Client). Только errCode; при успехе клиент входит через ENTERMAP.
    [<Struct>]
    type McBgnPlayResponse = { ErrCode: int16 }

    /// Данные успешного ответа CMD_MC_ENDPLAY (GateServer → Client).
    type McEndPlayResponseData =
        { MaxChaNum: int64; Characters: ChaSlotData[] }

    /// CMD_MC_ENDPLAY ответ.
    type McEndPlayResponse =
        { ErrCode: int16; Data: McEndPlayResponseData voption }

    /// CMD_MC_NEWCHA ответ (GateServer → Client).
    [<Struct>]
    type McNewChaResponse = { ErrCode: int16 }

    /// CMD_MC_DELCHA ответ (GateServer → Client).
    [<Struct>]
    type McDelChaResponse = { ErrCode: int16 }

    /// CMD_MC_CREATE_PASSWORD2 ответ (GateServer → Client).
    [<Struct>]
    type McCreatePassword2Response = { ErrCode: int16 }

    /// CMD_MC_UPDATE_PASSWORD2 ответ (GateServer → Client).
    [<Struct>]
    type McUpdatePassword2Response = { ErrCode: int16 }

    // ═══════════════════════════════════════════════════════════════
    //  Serialize — функции сериализации структур в WPacket
    // ═══════════════════════════════════════════════════════════════

    module Serialize =

        // ─── Группа A: TP/PT ──────────────────────────────────

        let tpLoginRequest (msg: TpLoginRequest) =
            let mutable w = WPacket(64)
            w.WriteCmd(Commands.CMD_TP_LOGIN)
            w.WriteInt64(int64 msg.ProtocolVersion)
            w.WriteString(msg.GateName)
            w

        let tpLoginResponse (msg: TpLoginResponse) =
            let mutable w = WPacket(16)
            w.WriteCmd(Commands.CMD_TP_LOGIN)
            w.WriteInt64(int64 msg.ErrCode)
            w

        let tpUserLoginRequest (msg: TpUserLoginRequest) =
            let mutable w = WPacket(256)
            w.WriteCmd(Commands.CMD_TP_USER_LOGIN)
            w.WriteString(msg.AcctName)
            w.WriteString(msg.AcctPassword)
            w.WriteString(msg.AcctMac)
            w.WriteInt64(int64 msg.ClientIp)
            w.WriteInt64(int64 msg.GateAddr)
            w.WriteInt64(msg.CheatMarker)
            w

        let tpUserLoginResponse (msg: TpUserLoginResponse) =
            let mutable w = WPacket(512)
            w.WriteCmd(Commands.CMD_TP_USER_LOGIN)
            match msg with
            | TpUserLoginSuccess data ->
                w.WriteInt64(int64 Commands.ERR_SUCCESS)
                w.WriteInt64(data.MaxChaNum)
                for cha in data.Characters do
                    w.WriteInt64(if cha.Valid then 1L else 0L)
                    if cha.Valid then
                        w.WriteString(cha.ChaName)
                        w.WriteString(cha.Job)
                        w.WriteInt64(cha.Degree)
                        w.WriteInt64(cha.TypeId)
                        for i in 0 .. EQUIP_NUM - 1 do
                            w.WriteInt64(if i < cha.EquipIds.Length then cha.EquipIds[i] else 0L)
                w.WriteInt64(if data.HasPassword2 then 1L else 0L)
                w.WriteInt64(data.AcctId)
                w.WriteInt64(data.AcctLoginId)
                w.WriteInt64(int64 data.GpAddr)
            | TpUserLoginError errCode ->
                w.WriteInt64(int64 errCode)
            w

        let tpUserLogoutRequest (msg: TpUserLogoutRequest) =
            let mutable w = WPacket(32)
            w.WriteCmd(Commands.CMD_TP_USER_LOGOUT)
            w.WriteInt64(int64 msg.GateAddr)
            w.WriteInt64(int64 msg.GpAddr)
            w

        let tpUserLogoutResponse (msg: TpUserLogoutResponse) =
            let mutable w = WPacket(16)
            w.WriteCmd(Commands.CMD_TP_USER_LOGOUT)
            w.WriteInt64(int64 msg.ErrCode)
            w

        let tpBgnPlayRequest (msg: TpBgnPlayRequest) =
            let mutable w = WPacket(32)
            w.WriteCmd(Commands.CMD_TP_BGNPLAY)
            w.WriteInt64(msg.ChaIndex)
            w.WriteInt64(int64 msg.GateAddr)
            w.WriteInt64(int64 msg.GpAddr)
            w

        let tpBgnPlayResponse (msg: TpBgnPlayResponse) =
            let mutable w = WPacket(256)
            w.WriteCmd(Commands.CMD_TP_BGNPLAY)
            w.WriteInt64(int64 msg.ErrCode)
            match msg.Data with
            | ValueSome data ->
                w.WriteString(data.Password2)
                w.WriteInt64(data.ChaId)
                w.WriteInt64(data.WorldId)
                w.WriteString(data.MapName)
                w.WriteInt64(data.Swiner)
            | ValueNone -> ()
            w

        let tpEndPlayRequest (msg: TpEndPlayRequest) =
            let mutable w = WPacket(32)
            w.WriteCmd(Commands.CMD_TP_ENDPLAY)
            w.WriteInt64(int64 msg.GateAddr)
            w.WriteInt64(int64 msg.GpAddr)
            w

        let tpEndPlayResponse (msg: TpEndPlayResponse) =
            let mutable w = WPacket(512)
            w.WriteCmd(Commands.CMD_TP_ENDPLAY)
            w.WriteInt64(int64 msg.ErrCode)
            match msg.Data with
            | ValueSome data ->
                w.WriteInt64(data.MaxChaNum)
                w.WriteInt64(data.ChaNum)
                for cha in data.Characters do
                    w.WriteInt64(if cha.Valid then 1L else 0L)
                    if cha.Valid then
                        w.WriteString(cha.ChaName)
                        w.WriteString(cha.Job)
                        w.WriteInt64(cha.Degree)
                        w.WriteInt64(cha.TypeId)
                        for i in 0 .. EQUIP_NUM - 1 do
                            w.WriteInt64(if i < cha.EquipIds.Length then cha.EquipIds[i] else 0L)
            | ValueNone -> ()
            w

        let tpNewChaRequest (msg: TpNewChaRequest) =
            let mutable w = WPacket(128)
            w.WriteCmd(Commands.CMD_TP_NEWCHA)
            w.WriteString(msg.ChaName)
            w.WriteString(msg.Birth)
            w.WriteInt64(msg.TypeId)
            w.WriteInt64(msg.HairId)
            w.WriteInt64(msg.FaceId)
            w.WriteInt64(int64 msg.GateAddr)
            w.WriteInt64(int64 msg.GpAddr)
            w

        let tpNewChaResponse (msg: TpNewChaResponse) =
            let mutable w = WPacket(16)
            w.WriteCmd(Commands.CMD_TP_NEWCHA)
            w.WriteInt64(int64 msg.ErrCode)
            w

        let tpDelChaRequest (msg: TpDelChaRequest) =
            let mutable w = WPacket(128)
            w.WriteCmd(Commands.CMD_TP_DELCHA)
            w.WriteInt64(msg.ChaIndex)
            w.WriteString(msg.Password2)
            w.WriteInt64(int64 msg.GateAddr)
            w.WriteInt64(int64 msg.GpAddr)
            w

        let tpDelChaResponse (msg: TpDelChaResponse) =
            let mutable w = WPacket(16)
            w.WriteCmd(Commands.CMD_TP_DELCHA)
            w.WriteInt64(int64 msg.ErrCode)
            w

        let tpChangePassRequest (msg: TpChangePassRequest) =
            let mutable w = WPacket(128)
            w.WriteCmd(Commands.CMD_TP_CHANGEPASS)
            w.WriteString(msg.NewPass)
            w.WriteString(msg.Pin)
            w.WriteInt64(int64 msg.GateAddr)
            w.WriteInt64(int64 msg.GpAddr)
            w

        let tpRegisterRequest (msg: TpRegisterRequest) =
            let mutable w = WPacket(256)
            w.WriteCmd(Commands.CMD_TP_REGISTER)
            w.WriteString(msg.Username)
            w.WriteString(msg.Password)
            w.WriteString(msg.Email)
            w

        let tpRegisterResponse (msg: TpRegisterResponse) =
            let mutable w = WPacket(128)
            w.WriteCmd(Commands.CMD_TP_REGISTER)
            w.WriteInt64(msg.Result)
            w.WriteString(msg.Message)
            w

        let tpCreatePassword2Request (msg: TpCreatePassword2Request) =
            let mutable w = WPacket(64)
            w.WriteCmd(Commands.CMD_TP_CREATE_PASSWORD2)
            w.WriteString(msg.Password2)
            w.WriteInt64(int64 msg.GateAddr)
            w.WriteInt64(int64 msg.GpAddr)
            w

        let tpUpdatePassword2Request (msg: TpUpdatePassword2Request) =
            let mutable w = WPacket(128)
            w.WriteCmd(Commands.CMD_TP_UPDATE_PASSWORD2)
            w.WriteString(msg.OldPassword2)
            w.WriteString(msg.NewPassword2)
            w.WriteInt64(int64 msg.GateAddr)
            w.WriteInt64(int64 msg.GpAddr)
            w

        let tpPassword2Response (msg: TpPassword2Response) =
            let mutable w = WPacket(16)
            w.WriteCmd(Commands.CMD_TP_CREATE_PASSWORD2)
            w.WriteInt64(int64 msg.ErrCode)
            w

        let tpReqPlyLstResponse (msg: TpReqPlyLstResponse) =
            let mutable w = WPacket(256)
            w.WriteCmd(Commands.CMD_TP_REQPLYLST)
            for e in msg.Entries do
                w.WriteInt64(e.GtAddr)
                w.WriteInt64(e.ChaId)
            w.WriteInt64(msg.PlyNum)
            w

        let tpSyncPlyLstRequest (msg: TpSyncPlyLstRequest) =
            let mutable w = WPacket(256)
            w.WriteCmd(Commands.CMD_TP_SYNC_PLYLST)
            w.WriteInt64(msg.Num)
            w.WriteString(msg.GateName)
            for p in msg.Players do
                w.WriteInt64(p.GtAddr)
                w.WriteInt64(p.AcctLoginId)
                w.WriteInt64(p.AcctId)
            w

        let tpSyncPlyLstResponse (msg: TpSyncPlyLstResponse) =
            let mutable w = WPacket(256)
            w.WriteCmd(Commands.CMD_TP_SYNC_PLYLST)
            w.WriteInt64(int64 msg.ErrCode)
            w.WriteInt64(msg.Num)
            for r in msg.Results do
                w.WriteInt64(if r.Ok then 1L else 0L)
                w.WriteInt64(r.PlayerPtr)
            w

        let osLoginRequest (msg: OsLoginRequest) =
            let mutable w = WPacket(64)
            w.WriteCmd(Commands.CMD_OS_LOGIN)
            w.WriteInt64(msg.Version)
            w.WriteString(msg.AgentName)
            w

        let osLoginResponse (msg: OsLoginResponse) =
            let mutable w = WPacket(16)
            w.WriteCmd(Commands.CMD_OS_LOGIN)
            w.WriteInt64(int64 msg.ErrCode)
            w

        let tpDiscMessage (msg: TpDiscMessage) =
            let mutable w = WPacket(128)
            w.WriteCmd(Commands.CMD_TP_DISC)
            w.WriteInt64(msg.ActId)
            w.WriteInt64(msg.IpAddr)
            w.WriteString(msg.Reason)
            w

        // ─── Группа B: PA/AP ──────────────────────────────────

        let paLoginRequest (msg: PaLoginRequest) =
            let mutable w = WPacket(128)
            w.WriteCmd(Commands.CMD_PA_LOGIN)
            w.WriteString(msg.ServerName)
            w.WriteString(msg.ServerPassword)
            w

        let paLoginResponse (msg: PaLoginResponse) =
            let mutable w = WPacket(16)
            w.WriteCmd(Commands.CMD_PA_LOGIN)
            w.WriteInt64(int64 msg.ErrCode)
            w

        let paUserLoginRequest (msg: PaUserLoginRequest) =
            let mutable w = WPacket(256)
            w.WriteCmd(Commands.CMD_PA_USER_LOGIN)
            w.WriteString(msg.Username)
            w.WriteString(msg.Password)
            w.WriteString(msg.Mac)
            w.WriteInt64(msg.ClientIp)
            w

        let paUserLoginResponse (msg: PaUserLoginResponse) =
            let mutable w = WPacket(64)
            w.WriteCmd(0us) // RPC-ответ
            match msg with
            | PaUserLoginSuccess data ->
                w.WriteInt64(int64 Commands.ERR_SUCCESS)
                w.WriteInt64(data.AcctLoginId)
                w.WriteInt64(data.SessId)
                w.WriteInt64(data.AcctId)
                w.WriteInt64(data.GmLevel)
            | PaUserLoginError errCode ->
                w.WriteInt64(int64 errCode)
            w

        let paUserLogoutMessage (msg: PaUserLogoutMessage) =
            let mutable w = WPacket(16)
            w.WriteCmd(Commands.CMD_PA_USER_LOGOUT)
            w.WriteInt64(msg.AcctLoginId)
            w

        let paUserBillBgnMessage (msg: PaUserBillBgnMessage) =
            let mutable w = WPacket(128)
            w.WriteCmd(Commands.CMD_PA_USER_BILLBGN)
            w.WriteString(msg.AcctName)
            w.WriteString(msg.Passport)
            w

        let paUserBillEndMessage (msg: PaUserBillEndMessage) =
            let mutable w = WPacket(64)
            w.WriteCmd(Commands.CMD_PA_USER_BILLEND)
            w.WriteString(msg.AcctName)
            w

        let paChangePassMessage (msg: PaChangePassMessage) =
            let mutable w = WPacket(128)
            w.WriteCmd(Commands.CMD_PA_CHANGEPASS)
            w.WriteString(msg.Username)
            w.WriteString(msg.NewPassword)
            w

        let paRegisterMessage (msg: PaRegisterMessage) =
            let mutable w = WPacket(256)
            w.WriteCmd(Commands.CMD_PA_REGISTER)
            w.WriteString(msg.Username)
            w.WriteString(msg.Password)
            w.WriteString(msg.Email)
            w

        let paGmBanMessage (msg: PaGmBanMessage) =
            let mutable w = WPacket(64)
            w.WriteCmd(Commands.CMD_PA_GMBANACCOUNT)
            w.WriteString(msg.ActName)
            w

        let paGmUnbanMessage (msg: PaGmUnbanMessage) =
            let mutable w = WPacket(64)
            w.WriteCmd(Commands.CMD_PA_GMUNBANACCOUNT)
            w.WriteString(msg.ActName)
            w

        let paUserDisableMessage (msg: PaUserDisableMessage) =
            let mutable w = WPacket(32)
            w.WriteCmd(Commands.CMD_PA_USER_DISABLE)
            w.WriteInt64(msg.AcctLoginId)
            w.WriteInt64(msg.Minutes)
            w

        let apKickUserMessage (msg: ApKickUserMessage) =
            let mutable w = WPacket(32)
            w.WriteCmd(Commands.CMD_AP_KICKUSER)
            w.WriteInt64(msg.ErrCode)
            w.WriteInt64(msg.AccountId)
            w

        let apExpScaleMessage (msg: ApExpScaleMessage) =
            let mutable w = WPacket(32)
            w.WriteCmd(Commands.CMD_AP_EXPSCALE)
            w.WriteInt64(msg.ChaId)
            w.WriteInt64(msg.Time)
            w

        // ─── Группа C: CP ────────────────────────────────────

        let cpTeamInviteMessage (msg: CpTeamInviteMessage) =
            let mutable w = WPacket(64)
            w.WriteCmd(Commands.CMD_CP_TEAM_INVITE)
            w.WriteString(msg.InvitedName)
            w.WriteInt64(int64 msg.GateAddr)
            w.WriteInt64(int64 msg.GpAddr)
            w

        let cpTeamAcceptMessage (msg: CpTeamAcceptMessage) =
            let mutable w = WPacket(32)
            w.WriteCmd(Commands.CMD_CP_TEAM_ACCEPT)
            w.WriteInt64(msg.InviterChaId)
            w.WriteInt64(int64 msg.GateAddr)
            w.WriteInt64(int64 msg.GpAddr)
            w

        let cpTeamRefuseMessage (msg: CpTeamRefuseMessage) =
            let mutable w = WPacket(32)
            w.WriteCmd(Commands.CMD_CP_TEAM_REFUSE)
            w.WriteInt64(msg.InviterChaId)
            w.WriteInt64(int64 msg.GateAddr)
            w.WriteInt64(int64 msg.GpAddr)
            w

        let cpTeamLeaveMessage (msg: CpTeamLeaveMessage) =
            let mutable w = WPacket(32)
            w.WriteCmd(Commands.CMD_CP_TEAM_LEAVE)
            w.WriteInt64(int64 msg.GateAddr)
            w.WriteInt64(int64 msg.GpAddr)
            w

        let cpTeamKickMessage (msg: CpTeamKickMessage) =
            let mutable w = WPacket(32)
            w.WriteCmd(Commands.CMD_CP_TEAM_KICK)
            w.WriteInt64(msg.KickedChaId)
            w.WriteInt64(int64 msg.GateAddr)
            w.WriteInt64(int64 msg.GpAddr)
            w

        let cpFrndInviteMessage (msg: CpFrndInviteMessage) =
            let mutable w = WPacket(64)
            w.WriteCmd(Commands.CMD_CP_FRND_INVITE)
            w.WriteString(msg.InvitedName)
            w.WriteInt64(int64 msg.GateAddr)
            w.WriteInt64(int64 msg.GpAddr)
            w

        let cpFrndAcceptMessage (msg: CpFrndAcceptMessage) =
            let mutable w = WPacket(32)
            w.WriteCmd(Commands.CMD_CP_FRND_ACCEPT)
            w.WriteInt64(msg.InviterChaId)
            w.WriteInt64(int64 msg.GateAddr)
            w.WriteInt64(int64 msg.GpAddr)
            w

        let cpFrndRefuseMessage (msg: CpFrndRefuseMessage) =
            let mutable w = WPacket(32)
            w.WriteCmd(Commands.CMD_CP_FRND_REFUSE)
            w.WriteInt64(msg.InviterChaId)
            w.WriteInt64(int64 msg.GateAddr)
            w.WriteInt64(int64 msg.GpAddr)
            w

        let cpFrndDeleteMessage (msg: CpFrndDeleteMessage) =
            let mutable w = WPacket(32)
            w.WriteCmd(Commands.CMD_CP_FRND_DELETE)
            w.WriteInt64(msg.DeletedChaId)
            w.WriteInt64(int64 msg.GateAddr)
            w.WriteInt64(int64 msg.GpAddr)
            w

        let cpFrndChangeGroupMessage (msg: CpFrndChangeGroupMessage) =
            let mutable w = WPacket(64)
            w.WriteCmd(Commands.CMD_CP_FRND_CHANGE_GROUP)
            w.WriteInt64(msg.FriendChaId)
            w.WriteString(msg.GroupName)
            w.WriteInt64(int64 msg.GateAddr)
            w.WriteInt64(int64 msg.GpAddr)
            w

        let cpFrndRefreshInfoMessage (msg: CpFrndRefreshInfoMessage) =
            let mutable w = WPacket(32)
            w.WriteCmd(Commands.CMD_CP_FRND_REFRESH_INFO)
            w.WriteInt64(msg.FriendChaId)
            w.WriteInt64(int64 msg.GateAddr)
            w.WriteInt64(int64 msg.GpAddr)
            w

        let cpSay2AllMessage (msg: CpSay2AllMessage) =
            let mutable w = WPacket(256)
            w.WriteCmd(Commands.CMD_CP_SAY2ALL)
            w.WriteString(msg.Content)
            w.WriteInt64(int64 msg.GateAddr)
            w.WriteInt64(int64 msg.GpAddr)
            w

        let cpSay2TradeMessage (msg: CpSay2TradeMessage) =
            let mutable w = WPacket(256)
            w.WriteCmd(Commands.CMD_CP_SAY2TRADE)
            w.WriteString(msg.Content)
            w.WriteInt64(int64 msg.GateAddr)
            w.WriteInt64(int64 msg.GpAddr)
            w

        let cpSay2YouMessage (msg: CpSay2YouMessage) =
            let mutable w = WPacket(256)
            w.WriteCmd(Commands.CMD_CP_SAY2YOU)
            w.WriteString(msg.TargetName)
            w.WriteString(msg.Content)
            w.WriteInt64(int64 msg.GateAddr)
            w.WriteInt64(int64 msg.GpAddr)
            w

        let cpSay2TemMessage (msg: CpSay2TemMessage) =
            let mutable w = WPacket(256)
            w.WriteCmd(Commands.CMD_CP_SAY2TEM)
            w.WriteString(msg.Content)
            w.WriteInt64(int64 msg.GateAddr)
            w.WriteInt64(int64 msg.GpAddr)
            w

        let cpSay2GudMessage (msg: CpSay2GudMessage) =
            let mutable w = WPacket(256)
            w.WriteCmd(Commands.CMD_CP_SAY2GUD)
            w.WriteString(msg.Content)
            w.WriteInt64(int64 msg.GateAddr)
            w.WriteInt64(int64 msg.GpAddr)
            w

        let cpGm1SayMessage (msg: CpGm1SayMessage) =
            let mutable w = WPacket(256)
            w.WriteCmd(Commands.CMD_CP_GM1SAY)
            w.WriteString(msg.Content)
            w.WriteInt64(int64 msg.GateAddr)
            w.WriteInt64(int64 msg.GpAddr)
            w

        let cpGm1Say1Message (msg: CpGm1Say1Message) =
            let mutable w = WPacket(256)
            w.WriteCmd(Commands.CMD_CP_GM1SAY1)
            w.WriteString(msg.Content)
            w.WriteInt64(msg.Color)
            w.WriteInt64(int64 msg.GateAddr)
            w.WriteInt64(int64 msg.GpAddr)
            w

        let cpSessCreateMessage (msg: CpSessCreateMessage) =
            let mutable w = WPacket(256)
            w.WriteCmd(Commands.CMD_CP_SESS_CREATE)
            w.WriteInt64(msg.ChaNum)
            for name in msg.ChaNames do
                w.WriteString(name)
            w.WriteInt64(int64 msg.GateAddr)
            w.WriteInt64(int64 msg.GpAddr)
            w

        let cpSessAddMessage (msg: CpSessAddMessage) =
            let mutable w = WPacket(128)
            w.WriteCmd(Commands.CMD_CP_SESS_ADD)
            w.WriteInt64(msg.SessId)
            w.WriteString(msg.ChaName)
            w.WriteInt64(int64 msg.GateAddr)
            w.WriteInt64(int64 msg.GpAddr)
            w

        let cpSessSayMessage (msg: CpSessSayMessage) =
            let mutable w = WPacket(256)
            w.WriteCmd(Commands.CMD_CP_SESS_SAY)
            w.WriteInt64(msg.SessId)
            w.WriteString(msg.Content)
            w.WriteInt64(int64 msg.GateAddr)
            w.WriteInt64(int64 msg.GpAddr)
            w

        let cpSessLeaveMessage (msg: CpSessLeaveMessage) =
            let mutable w = WPacket(32)
            w.WriteCmd(Commands.CMD_CP_SESS_LEAVE)
            w.WriteInt64(msg.SessId)
            w.WriteInt64(int64 msg.GateAddr)
            w.WriteInt64(int64 msg.GpAddr)
            w

        let cpChangePersonInfoMessage (msg: CpChangePersonInfoMessage) =
            let mutable w = WPacket(128)
            w.WriteCmd(Commands.CMD_CP_CHANGE_PERSONINFO)
            w.WriteString(msg.Motto)
            w.WriteInt64(msg.Icon)
            w.WriteInt64(msg.RefuseSess)
            w.WriteInt64(int64 msg.GateAddr)
            w.WriteInt64(int64 msg.GpAddr)
            w

        let cpPingMessage (msg: CpPingMessage) =
            let mutable w = WPacket(32)
            w.WriteCmd(Commands.CMD_CP_PING)
            w.WriteInt64(msg.PingValue)
            w.WriteInt64(int64 msg.GateAddr)
            w.WriteInt64(int64 msg.GpAddr)
            w

        let cpRefuseToMeMessage (msg: CpRefuseToMeMessage) =
            let mutable w = WPacket(32)
            w.WriteCmd(Commands.CMD_CP_REFUSETOME)
            w.WriteInt64(msg.RefuseFlag)
            w.WriteInt64(int64 msg.GateAddr)
            w.WriteInt64(int64 msg.GpAddr)
            w

        let cpReportWgMessage (msg: CpReportWgMessage) =
            let mutable w = WPacket(32)
            w.WriteCmd(Commands.CMD_CP_REPORT_WG)
            w.WriteInt64(int64 msg.GateAddr)
            w.WriteInt64(int64 msg.GpAddr)
            w

        let cpMasterRefreshInfoMessage (msg: CpMasterRefreshInfoMessage) =
            let mutable w = WPacket(32)
            w.WriteCmd(Commands.CMD_CP_MASTER_REFRESH_INFO)
            w.WriteInt64(msg.MasterChaId)
            w.WriteInt64(int64 msg.GateAddr)
            w.WriteInt64(int64 msg.GpAddr)
            w

        let cpPrenticeRefreshInfoMessage (msg: CpPrenticeRefreshInfoMessage) =
            let mutable w = WPacket(32)
            w.WriteCmd(Commands.CMD_CP_PRENTICE_REFRESH_INFO)
            w.WriteInt64(msg.PrenticeChaId)
            w.WriteInt64(int64 msg.GateAddr)
            w.WriteInt64(int64 msg.GpAddr)
            w

        let cpChangePassMessage (msg: CpChangePassMessage) =
            let mutable w = WPacket(128)
            w.WriteCmd(Commands.CMD_CP_CHANGEPASS)
            w.WriteString(msg.NewPass)
            w.WriteString(msg.Pin)
            w.WriteInt64(int64 msg.GateAddr)
            w.WriteInt64(int64 msg.GpAddr)
            w

        // ─── Группа D: MP ────────────────────────────────────

        let mpEnterMapMessage (msg: MpEnterMapMessage) =
            let mutable w = WPacket(32)
            w.WriteCmd(Commands.CMD_MP_ENTERMAP)
            w.WriteInt64(msg.IsSwitch)
            w.WriteInt64(int64 msg.GateAddr)
            w.WriteInt64(int64 msg.GpAddr)
            w

        let mpTeamCreateMessage (msg: MpTeamCreateMessage) =
            let mutable w = WPacket(128)
            w.WriteCmd(Commands.CMD_MP_TEAM_CREATE)
            w.WriteString(msg.MemberName)
            w.WriteString(msg.LeaderName)
            w.WriteInt64(int64 msg.GateAddr)
            w.WriteInt64(int64 msg.GpAddr)
            w

        let mpGuildCreateMessage (msg: MpGuildCreateMessage) =
            let mutable w = WPacket(128)
            w.WriteCmd(Commands.CMD_MP_GUILD_CREATE)
            w.WriteInt64(msg.GuildId)
            w.WriteString(msg.GldName)
            w.WriteString(msg.Job)
            w.WriteInt64(msg.Degree)
            w.WriteInt64(int64 msg.GateAddr)
            w.WriteInt64(int64 msg.GpAddr)
            w

        let mpGuildApproveMessage (msg: MpGuildApproveMessage) =
            let mutable w = WPacket(32)
            w.WriteCmd(Commands.CMD_MP_GUILD_APPROVE)
            w.WriteInt64(msg.NewMemberChaId)
            w.WriteInt64(int64 msg.GateAddr)
            w.WriteInt64(int64 msg.GpAddr)
            w

        let mpGuildKickMessage (msg: MpGuildKickMessage) =
            let mutable w = WPacket(32)
            w.WriteCmd(Commands.CMD_MP_GUILD_KICK)
            w.WriteInt64(msg.KickedChaId)
            w.WriteInt64(int64 msg.GateAddr)
            w.WriteInt64(int64 msg.GpAddr)
            w

        let mpGuildLeaveMessage (msg: MpGuildLeaveMessage) =
            let mutable w = WPacket(32)
            w.WriteCmd(Commands.CMD_MP_GUILD_LEAVE)
            w.WriteInt64(int64 msg.GateAddr)
            w.WriteInt64(int64 msg.GpAddr)
            w

        let mpGuildDisbandMessage (msg: MpGuildDisbandMessage) =
            let mutable w = WPacket(32)
            w.WriteCmd(Commands.CMD_MP_GUILD_DISBAND)
            w.WriteInt64(int64 msg.GateAddr)
            w.WriteInt64(int64 msg.GpAddr)
            w

        let mpGuildMottoMessage (msg: MpGuildMottoMessage) =
            let mutable w = WPacket(128)
            w.WriteCmd(Commands.CMD_MP_GUILD_MOTTO)
            w.WriteString(msg.Motto)
            w.WriteInt64(int64 msg.GateAddr)
            w.WriteInt64(int64 msg.GpAddr)
            w

        let mpGuildPermMessage (msg: MpGuildPermMessage) =
            let mutable w = WPacket(32)
            w.WriteCmd(Commands.CMD_MP_GUILD_PERM)
            w.WriteInt64(msg.TargetChaId)
            w.WriteInt64(msg.Permission)
            w.WriteInt64(int64 msg.GateAddr)
            w.WriteInt64(int64 msg.GpAddr)
            w

        let mpGuildChallMoneyMessage (msg: MpGuildChallMoneyMessage) =
            let mutable w = WPacket(256)
            w.WriteCmd(Commands.CMD_MP_GUILD_CHALLMONEY)
            w.WriteInt64(msg.GuildId)
            w.WriteInt64(msg.Money)
            w.WriteString(msg.GuildName1)
            w.WriteString(msg.GuildName2)
            w.WriteInt64(int64 msg.GateAddr)
            w.WriteInt64(int64 msg.GpAddr)
            w

        let mpGuildChallPrizeMoneyMessage (msg: MpGuildChallPrizeMoneyMessage) =
            let mutable w = WPacket(32)
            w.WriteCmd(Commands.CMD_MP_GUILD_CHALL_PRIZEMONEY)
            w.WriteInt64(msg.GuildId)
            w.WriteInt64(msg.Money)
            w

        let mpMasterCreateMessage (msg: MpMasterCreateMessage) =
            let mutable w = WPacket(128)
            w.WriteCmd(Commands.CMD_MP_MASTER_CREATE)
            w.WriteString(msg.PrenticeName)
            w.WriteInt64(msg.PrenticeChaid)
            w.WriteString(msg.MasterName)
            w.WriteInt64(msg.MasterChaid)
            w

        let mpMasterDelMessage (msg: MpMasterDelMessage) =
            let mutable w = WPacket(128)
            w.WriteCmd(Commands.CMD_MP_MASTER_DEL)
            w.WriteString(msg.PrenticeName)
            w.WriteInt64(msg.PrenticeChaid)
            w.WriteString(msg.MasterName)
            w.WriteInt64(msg.MasterChaid)
            w

        let mpMasterFinishMessage (msg: MpMasterFinishMessage) =
            let mutable w = WPacket(16)
            w.WriteCmd(Commands.CMD_MP_MASTER_FINISH)
            w.WriteInt64(msg.PrenticeChaid)
            w

        let mpSay2AllMessage (msg: MpSay2AllMessage) =
            let mutable w = WPacket(256)
            w.WriteCmd(Commands.CMD_MP_SAY2ALL)
            w.WriteInt64(msg.Succ)
            w.WriteString(msg.ChaName)
            w.WriteString(msg.Content)
            w.WriteInt64(int64 msg.GateAddr)
            w.WriteInt64(int64 msg.GpAddr)
            w

        let mpSay2TradeMessage (msg: MpSay2TradeMessage) =
            let mutable w = WPacket(256)
            w.WriteCmd(Commands.CMD_MP_SAY2TRADE)
            w.WriteInt64(msg.Succ)
            w.WriteString(msg.ChaName)
            w.WriteString(msg.Content)
            w.WriteInt64(int64 msg.GateAddr)
            w.WriteInt64(int64 msg.GpAddr)
            w

        let mpGm1SayMessage (msg: MpGm1SayMessage) =
            let mutable w = WPacket(256)
            w.WriteCmd(Commands.CMD_MP_GM1SAY)
            w.WriteString(msg.Content)
            w.WriteInt64(int64 msg.GateAddr)
            w.WriteInt64(int64 msg.GpAddr)
            w

        let mpGm1Say1Message (msg: MpGm1Say1Message) =
            let mutable w = WPacket(256)
            w.WriteCmd(Commands.CMD_MP_GM1SAY1)
            w.WriteString(msg.Content)
            w.WriteInt64(msg.SetNum)
            w.WriteInt64(msg.Color)
            w

        let mpGmBanMessage (msg: MpGmBanMessage) =
            let mutable w = WPacket(64)
            w.WriteCmd(Commands.CMD_MP_GMBANACCOUNT)
            w.WriteString(msg.ActName)
            w.WriteInt64(int64 msg.GateAddr)
            w.WriteInt64(int64 msg.GpAddr)
            w

        let mpGmUnbanMessage (msg: MpGmUnbanMessage) =
            let mutable w = WPacket(64)
            w.WriteCmd(Commands.CMD_MP_GMUNBANACCOUNT)
            w.WriteString(msg.ActName)
            w.WriteInt64(int64 msg.GateAddr)
            w.WriteInt64(int64 msg.GpAddr)
            w

        let mpGuildNoticeMessage (msg: MpGuildNoticeMessage) =
            let mutable w = WPacket(256)
            w.WriteCmd(Commands.CMD_MP_GUILDNOTICE)
            w.WriteInt64(msg.GuildId)
            w.WriteString(msg.Content)
            w

        let mpCanReceiveRequestsMessage (msg: MpCanReceiveRequestsMessage) =
            let mutable w = WPacket(32)
            w.WriteCmd(Commands.CMD_MP_CANRECEIVEREQUESTS)
            w.WriteInt64(msg.ChaId)
            w.WriteInt64(msg.CanSend)
            w

        let mpMutePlayerMessage (msg: MpMutePlayerMessage) =
            let mutable w = WPacket(128)
            w.WriteCmd(Commands.CMD_MP_MUTE_PLAYER)
            w.WriteString(msg.ChaName)
            w.WriteInt64(msg.Time)
            w.WriteInt64(int64 msg.GateAddr)
            w.WriteInt64(int64 msg.GpAddr)
            w

        let mpGarner2UpdateMessage (msg: MpGarner2UpdateMessage) =
            let mutable w = WPacket(256)
            w.WriteCmd(Commands.CMD_MP_GARNER2_UPDATE)
            w.WriteInt64(msg.Nid)
            w.WriteString(msg.ChaName)
            w.WriteInt64(msg.Level)
            w.WriteString(msg.Job)
            w.WriteInt64(msg.Fightpoint)
            w.WriteInt64(int64 msg.GateAddr)
            w.WriteInt64(int64 msg.GpAddr)
            w

        let mpGarner2GetOrderMessage (msg: MpGarner2GetOrderMessage) =
            let mutable w = WPacket(32)
            w.WriteCmd(Commands.CMD_MP_GARNER2_CGETORDER)
            w.WriteInt64(int64 msg.GateAddr)
            w.WriteInt64(int64 msg.GpAddr)
            w

        let mpGuildBankAckMessage (msg: MpGuildBankAckMessage) =
            let mutable w = WPacket(32)
            w.WriteCmd(Commands.CMD_MP_GUILDBANK)
            w.WriteInt64(msg.GuildId)
            w.WriteInt64(int64 msg.GateAddr)
            w.WriteInt64(int64 msg.GpAddr)
            w

        // ─── Группа E: PC ────────────────────────────────────

        let pcTeamInviteMessage (msg: PcTeamInviteMessage) =
            let mutable w = WPacket(128)
            w.WriteCmd(Commands.CMD_PC_TEAM_INVITE)
            w.WriteString(msg.InviterName)
            w.WriteInt64(msg.ChaId)
            w.WriteInt64(msg.Icon)
            w

        let pcTeamRefreshMessage (msg: PcTeamRefreshMessage) =
            let mutable w = WPacket(256)
            w.WriteCmd(Commands.CMD_PC_TEAM_REFRESH)
            w.WriteInt64(msg.Msg)
            w.WriteInt64(msg.Count)
            for m in msg.Members do
                w.WriteInt64(m.ChaId)
                w.WriteString(m.ChaName)
                w.WriteString(m.Motto)
                w.WriteInt64(m.Icon)
            w

        let pcFrndInviteMessage (msg: PcFrndInviteMessage) =
            let mutable w = WPacket(128)
            w.WriteCmd(Commands.CMD_PC_FRND_INVITE)
            w.WriteString(msg.InviterName)
            w.WriteInt64(msg.ChaId)
            w.WriteInt64(msg.Icon)
            w

        let pcFrndRefreshMessage (msg: PcFrndRefreshMessage) =
            let mutable w = WPacket(256)
            w.WriteCmd(Commands.CMD_PC_FRND_REFRESH)
            w.WriteInt64(msg.Msg)
            w.WriteString(msg.Group)
            w.WriteInt64(msg.ChaId)
            w.WriteString(msg.ChaName)
            w.WriteString(msg.Motto)
            w.WriteInt64(msg.Icon)
            w

        let pcFrndRefreshDelMessage (msg: PcFrndRefreshDelMessage) =
            let mutable w = WPacket(32)
            w.WriteCmd(Commands.CMD_PC_FRND_REFRESH)
            w.WriteInt64(msg.Msg)
            w.WriteInt64(msg.ChaId)
            w

        let pcFrndChangeGroupMessage (msg: PcFrndChangeGroupMessage) =
            let mutable w = WPacket(64)
            w.WriteCmd(Commands.CMD_PC_FRND_CHANGE_GROUP)
            w.WriteInt64(msg.FriendChaId)
            w.WriteString(msg.GroupName)
            w

        let pcFrndRefreshInfoMessage (msg: PcFrndRefreshInfoMessage) =
            let mutable w = WPacket(256)
            w.WriteCmd(Commands.CMD_PC_FRND_REFRESH_INFO)
            w.WriteInt64(msg.ChaId)
            w.WriteString(msg.Motto)
            w.WriteInt64(msg.Icon)
            w.WriteInt64(msg.Degree)
            w.WriteString(msg.Job)
            w.WriteString(msg.GuildName)
            w

        let pcSay2AllMessage (msg: PcSay2AllMessage) =
            let mutable w = WPacket(256)
            w.WriteCmd(Commands.CMD_PC_SAY2ALL)
            w.WriteString(msg.ChaName)
            w.WriteString(msg.Content)
            w.WriteInt64(msg.Color)
            w

        let pcSay2TradeMessage (msg: PcSay2TradeMessage) =
            let mutable w = WPacket(256)
            w.WriteCmd(Commands.CMD_PC_SAY2TRADE)
            w.WriteString(msg.ChaName)
            w.WriteString(msg.Content)
            w.WriteInt64(msg.Color)
            w

        let pcSay2YouMessage (msg: PcSay2YouMessage) =
            let mutable w = WPacket(256)
            w.WriteCmd(Commands.CMD_PC_SAY2YOU)
            w.WriteString(msg.Sender)
            w.WriteString(msg.Target)
            w.WriteString(msg.Content)
            w.WriteInt64(msg.Color)
            w

        let pcSay2TemMessage (msg: PcSay2TemMessage) =
            let mutable w = WPacket(256)
            w.WriteCmd(Commands.CMD_PC_SAY2TEM)
            w.WriteInt64(msg.ChaId)
            w.WriteString(msg.Content)
            w.WriteInt64(msg.Color)
            w

        let pcSay2GudMessage (msg: PcSay2GudMessage) =
            let mutable w = WPacket(256)
            w.WriteCmd(Commands.CMD_PC_SAY2GUD)
            w.WriteString(msg.ChaName)
            w.WriteString(msg.Content)
            w.WriteInt64(msg.Color)
            w

        let pcGm1SayMessage (msg: PcGm1SayMessage) =
            let mutable w = WPacket(256)
            w.WriteCmd(Commands.CMD_PC_GM1SAY)
            w.WriteString(msg.ChaName)
            w.WriteString(msg.Content)
            w

        let pcGm1Say1Message (msg: PcGm1Say1Message) =
            let mutable w = WPacket(256)
            w.WriteCmd(Commands.CMD_PC_GM1SAY1)
            w.WriteString(msg.Content)
            w.WriteInt64(msg.SetNum)
            w.WriteInt64(msg.Color)
            w

        let pcGarner2OrderMessage (msg: PcGarner2OrderMessage) =
            let mutable w = WPacket(512)
            w.WriteCmd(Commands.CMD_PC_GARNER2_ORDER)
            for entry in msg.Entries do
                w.WriteString(entry.Name)
                w.WriteInt64(entry.Level)
                w.WriteString(entry.Job)
                w.WriteInt64(entry.FightPoint)
            w

        let pcGuildPermMessage (msg: PcGuildPermMessage) =
            let mutable w = WPacket(32)
            w.WriteCmd(Commands.CMD_PC_GUILD_PERM)
            w.WriteInt64(msg.TargetChaId)
            w.WriteInt64(msg.Permission)
            w

        let pcMasterRefreshAddMessage (msg: PcMasterRefreshAddMessage) =
            let mutable w = WPacket(256)
            w.WriteCmd(Commands.CMD_PC_MASTER_REFRESH)
            w.WriteInt64(msg.Msg)
            w.WriteString(msg.Group)
            w.WriteInt64(msg.ChaId)
            w.WriteString(msg.ChaName)
            w.WriteString(msg.Motto)
            w.WriteInt64(msg.Icon)
            w

        let pcMasterRefreshDelMessage (msg: PcMasterRefreshDelMessage) =
            let mutable w = WPacket(32)
            w.WriteCmd(Commands.CMD_PC_MASTER_REFRESH)
            w.WriteInt64(msg.Msg)
            w.WriteInt64(msg.ChaId)
            w

        let pcSessCreateMessage (msg: PcSessCreateMessage) =
            let mutable w = WPacket(256)
            w.WriteCmd(Commands.CMD_PC_SESS_CREATE)
            w.WriteInt64(msg.SessId)
            w.WriteInt64(int64 msg.Members.Length)
            for m in msg.Members do
                w.WriteInt64(m.ChaId)
                w.WriteString(m.ChaName)
                w.WriteString(m.Motto)
                w.WriteInt64(m.Icon)
            w.WriteInt64(msg.NotiPlyCount)
            w

        let pcSessAddMessage (msg: PcSessAddMessage) =
            let mutable w = WPacket(256)
            w.WriteCmd(Commands.CMD_PC_SESS_ADD)
            w.WriteInt64(msg.SessId)
            w.WriteInt64(msg.ChaId)
            w.WriteString(msg.ChaName)
            w.WriteString(msg.Motto)
            w.WriteInt64(msg.Icon)
            w

        let pcSessSayMessage (msg: PcSessSayMessage) =
            let mutable w = WPacket(256)
            w.WriteCmd(Commands.CMD_PC_SESS_SAY)
            w.WriteInt64(msg.SessId)
            w.WriteInt64(msg.ChaId)
            w.WriteString(msg.Content)
            w

        let pcSessLeaveMessage (msg: PcSessLeaveMessage) =
            let mutable w = WPacket(32)
            w.WriteCmd(Commands.CMD_PC_SESS_LEAVE)
            w.WriteInt64(msg.SessId)
            w.WriteInt64(msg.ChaId)
            w

        let pcChangePersonInfoMessage (msg: PcChangePersonInfoMessage) =
            let mutable w = WPacket(128)
            w.WriteCmd(Commands.CMD_PC_CHANGE_PERSONINFO)
            w.WriteString(msg.Motto)
            w.WriteInt64(msg.Icon)
            w.WriteInt64(msg.RefuseSess)
            w

        let pcErrMsgMessage (msg: PcErrMsgMessage) =
            let mutable w = WPacket(128)
            w.WriteCmd(Commands.CMD_PC_ERRMSG)
            w.WriteString(msg.Message)
            w

        let pcGuildNoticeMessage (msg: PcGuildNoticeMessage) =
            let mutable w = WPacket(256)
            w.WriteCmd(Commands.CMD_PC_GUILDNOTICE)
            w.WriteString(msg.Content)
            w

        let pcRegisterMessage (msg: PcRegisterMessage) =
            let mutable w = WPacket(128)
            w.WriteCmd(Commands.CMD_PC_REGISTER)
            w.WriteInt64(msg.Result)
            w.WriteString(msg.Message)
            w

        // ─── Группа F: PM ────────────────────────────────────

        let pmTeamMessage (msg: PmTeamMessage) =
            let mutable w = WPacket(256)
            w.WriteCmd(Commands.CMD_PM_TEAM)
            w.WriteInt64(msg.Msg)
            w.WriteInt64(msg.Count)
            for m in msg.Members do
                w.WriteString(m.GateName)
                w.WriteInt64(m.GtAddr)
                w.WriteInt64(m.ChaId)
            w

        let pmGarner2UpdateMessage (msg: PmGarner2UpdateMessage) =
            let mutable w = WPacket(128)
            w.WriteCmd(Commands.CMD_PM_GARNER2_UPDATE)
            for nid in msg.Nids do
                w.WriteInt64(nid)
            w.WriteInt64(msg.OldIndex)
            w.WriteInt64(0L)
            w

        let pmExpScaleMessage (msg: PmExpScaleMessage) =
            let mutable w = WPacket(32)
            w.WriteCmd(Commands.CMD_PM_EXPSCALE)
            w.WriteInt64(msg.ChaId)
            w.WriteInt64(msg.Time)
            w

        let pmSay2AllMessage (msg: PmSay2AllMessage) =
            let mutable w = WPacket(256)
            w.WriteCmd(Commands.CMD_PM_SAY2ALL)
            w.WriteInt64(msg.ChaId)
            w.WriteString(msg.Content)
            w.WriteInt64(msg.Money)
            w

        let pmSay2TradeMessage (msg: PmSay2TradeMessage) =
            let mutable w = WPacket(256)
            w.WriteCmd(Commands.CMD_PM_SAY2TRADE)
            w.WriteInt64(msg.ChaId)
            w.WriteString(msg.Content)
            w.WriteInt64(msg.Money)
            w

        let pmGuildDisbandMessage (msg: PmGuildDisbandMessage) =
            let mutable w = WPacket(16)
            w.WriteCmd(Commands.CMD_PM_GUILD_DISBAND)
            w.WriteInt64(msg.GuildId)
            w

        let pmGuildChallMoneyMessage (msg: PmGuildChallMoneyMessage) =
            let mutable w = WPacket(256)
            w.WriteCmd(Commands.CMD_PM_GUILD_CHALLMONEY)
            w.WriteInt64(msg.LeaderId)
            w.WriteInt64(msg.Money)
            w.WriteString(msg.GuildName1)
            w.WriteString(msg.GuildName2)
            w

        let pmGuildChallPrizeMoneyMessage (msg: PmGuildChallPrizeMoneyMessage) =
            let mutable w = WPacket(32)
            w.WriteCmd(Commands.CMD_PM_GUILD_CHALL_PRIZEMONEY)
            w.WriteInt64(msg.LeaderId)
            w.WriteInt64(msg.Money)
            w

        let ptKickUserMessage (msg: PtKickUserMessage) =
            let mutable w = WPacket(32)
            w.WriteCmd(Commands.CMD_PT_KICKUSER)
            w.WriteInt64(msg.GpAddr)
            w.WriteInt64(msg.GtAddr)
            w

        // ─── Группа G: CM ────────────────────────────────────

        let cmLoginRequest (msg: CmLoginRequest) =
            let mutable w = WPacket(256)
            w.WriteCmd(Commands.CMD_CM_LOGIN)
            w.WriteString(msg.AcctName)
            w.WriteString(msg.PasswordHash)
            w.WriteString(msg.Mac)
            w.WriteInt64(msg.CheatMarker)
            w.WriteInt64(msg.ClientVersion)
            w

        let mcLoginResponse (msg: McLoginResponse) =
            let mutable w = WPacket(512)
            w.WriteCmd(Commands.CMD_MC_LOGIN)
            match msg with
            | McLoginSuccess data ->
                w.WriteInt64(int64 Commands.ERR_SUCCESS) // errCode = 0 (C++: SC_Login читает первым)
                w.WriteInt64(data.MaxChaNum) // maxCharacters (C++: SC_Login)
                w.WriteInt64(int64 data.Characters.Length) // charCount (C++: ReadSelectCharacters)
                for cha in data.Characters do
                    w.WriteInt64(if cha.Valid then 1L else 0L)
                    if cha.Valid then
                        w.WriteString(cha.ChaName)
                        w.WriteString(cha.Job)
                        w.WriteInt64(cha.Degree)
                        w.WriteInt64(cha.TypeId)
                        for i in 0 .. EQUIP_NUM - 1 do
                            w.WriteInt64(if i < cha.EquipIds.Length then cha.EquipIds[i] else 0L)
                w.WriteInt64(if data.HasPassword2 then 1L else 0L)
            | McLoginError errCode ->
                w.WriteInt64(int64 errCode)
            w

        let mcBgnPlayResponse (msg: McBgnPlayResponse) =
            let mutable w = WPacket(16)
            w.WriteCmd(Commands.CMD_MC_BGNPLAY)
            w.WriteInt64(int64 msg.ErrCode)
            w

        let mcEndPlayResponse (msg: McEndPlayResponse) =
            let mutable w = WPacket(4096)
            w.WriteCmd(Commands.CMD_MC_ENDPLAY)
            w.WriteInt64(int64 msg.ErrCode)
            match msg.Data with
            | ValueSome data ->
                w.WriteInt64(data.MaxChaNum)
                w.WriteInt64(int64 data.Characters.Length)
                for cha in data.Characters do
                    w.WriteInt64(if cha.Valid then 1L else 0L)
                    if cha.Valid then
                        w.WriteString(cha.ChaName)
                        w.WriteString(cha.Job)
                        w.WriteInt64(cha.Degree)
                        w.WriteInt64(cha.TypeId)
                        for i in 0 .. EQUIP_NUM - 1 do
                            w.WriteInt64(if i < cha.EquipIds.Length then cha.EquipIds[i] else 0L)
            | ValueNone -> ()
            w

        let mcNewChaResponse (msg: McNewChaResponse) =
            let mutable w = WPacket(16)
            w.WriteCmd(Commands.CMD_MC_NEWCHA)
            w.WriteInt64(int64 msg.ErrCode)
            w

        let mcDelChaResponse (msg: McDelChaResponse) =
            let mutable w = WPacket(16)
            w.WriteCmd(Commands.CMD_MC_DELCHA)
            w.WriteInt64(int64 msg.ErrCode)
            w

        let mcCreatePassword2Response (msg: McCreatePassword2Response) =
            let mutable w = WPacket(16)
            w.WriteCmd(Commands.CMD_MC_CREATE_PASSWORD2)
            w.WriteInt64(int64 msg.ErrCode)
            w

        let mcUpdatePassword2Response (msg: McUpdatePassword2Response) =
            let mutable w = WPacket(16)
            w.WriteCmd(Commands.CMD_MC_UPDATE_PASSWORD2)
            w.WriteInt64(int64 msg.ErrCode)
            w

    // ═══════════════════════════════════════════════════════════════
    //  Deserialize — функции десериализации из IRPacket в структуры
    // ═══════════════════════════════════════════════════════════════

    module Deserialize =

        // ─── Группа A: TP/PT ──────────────────────────────────

        let tpLoginRequest (pk: IRPacket) : TpLoginRequest =
            { ProtocolVersion = int16 (pk.ReadInt64()); GateName = pk.ReadString() }

        let tpLoginResponse (pk: IRPacket) : TpLoginResponse =
            { ErrCode = int16 (pk.ReadInt64()) }

        let tpUserLoginRequest (pk: IRPacket) : TpUserLoginRequest =
            { AcctName = pk.ReadString(); AcctPassword = pk.ReadString(); AcctMac = pk.ReadString()
              ClientIp = uint32 (pk.ReadInt64()); GateAddr = uint32 (pk.ReadInt64())
              CheatMarker = pk.ReadInt64() }

        let tpUserLoginResponse (pk: IRPacket) : TpUserLoginResponse =
            let errCode = int16 (pk.ReadInt64())
            if errCode = 0s then
                let maxChaNum = pk.ReadInt64()
                let chars = Array.init (int maxChaNum) (fun _ ->
                    let valid = pk.ReadInt64() <> 0L
                    if valid then
                        { Valid = true; ChaName = pk.ReadString(); Job = pk.ReadString()
                          Degree = pk.ReadInt64(); TypeId = pk.ReadInt64()
                          EquipIds = Array.init EQUIP_NUM (fun _ -> pk.ReadInt64()) }
                    else
                        { Valid = false; ChaName = ""; Job = ""; Degree = 0L; TypeId = 0L
                          EquipIds = Array.empty })
                let hasPassword2 = pk.ReadInt64() <> 0L
                let acctId = pk.ReadInt64()
                let acctLoginId = pk.ReadInt64()
                let gpAddr = uint32 (pk.ReadInt64())
                TpUserLoginSuccess { MaxChaNum = maxChaNum; Characters = chars
                                     HasPassword2 = hasPassword2; AcctId = acctId
                                     AcctLoginId = acctLoginId; GpAddr = gpAddr }
            else TpUserLoginError errCode

        let tpUserLogoutRequest (pk: IRPacket) : TpUserLogoutRequest =
            { GateAddr = uint32 (pk.ReadInt64()); GpAddr = uint32 (pk.ReadInt64()) }

        let tpUserLogoutResponse (pk: IRPacket) : TpUserLogoutResponse =
            { ErrCode = int16 (pk.ReadInt64()) }

        let tpBgnPlayRequest (pk: IRPacket) : TpBgnPlayRequest =
            { ChaIndex = pk.ReadInt64()
              GateAddr = uint32 (pk.ReadInt64()); GpAddr = uint32 (pk.ReadInt64()) }

        let tpBgnPlayResponse (pk: IRPacket) : TpBgnPlayResponse =
            let errCode = int16 (pk.ReadInt64())
            let data =
                if errCode = 0s then
                    ValueSome { Password2 = pk.ReadString(); ChaId = pk.ReadInt64()
                                WorldId = pk.ReadInt64(); MapName = pk.ReadString()
                                Swiner = pk.ReadInt64() }
                else ValueNone
            { ErrCode = errCode; Data = data }

        let tpEndPlayRequest (pk: IRPacket) : TpEndPlayRequest =
            { GateAddr = uint32 (pk.ReadInt64()); GpAddr = uint32 (pk.ReadInt64()) }

        let tpEndPlayResponse (pk: IRPacket) : TpEndPlayResponse =
            let errCode = int16 (pk.ReadInt64())
            let data =
                if errCode = 0s then
                    let maxChaNum = pk.ReadInt64()
                    let chaNum = pk.ReadInt64()
                    let chars = Array.init (int maxChaNum) (fun _ ->
                        let valid = pk.ReadInt64() <> 0L
                        if valid then
                            { Valid = true; ChaName = pk.ReadString(); Job = pk.ReadString()
                              Degree = pk.ReadInt64(); TypeId = pk.ReadInt64()
                              EquipIds = Array.init EQUIP_NUM (fun _ -> pk.ReadInt64()) }
                        else
                            { Valid = false; ChaName = ""; Job = ""; Degree = 0L; TypeId = 0L
                              EquipIds = Array.empty })
                    ValueSome { MaxChaNum = maxChaNum; ChaNum = chaNum; Characters = chars }
                else ValueNone
            { ErrCode = errCode; Data = data }

        let tpNewChaRequest (pk: IRPacket) : TpNewChaRequest =
            { ChaName = pk.ReadString(); Birth = pk.ReadString()
              TypeId = pk.ReadInt64(); HairId = pk.ReadInt64(); FaceId = pk.ReadInt64()
              GateAddr = uint32 (pk.ReadInt64()); GpAddr = uint32 (pk.ReadInt64()) }

        let tpNewChaResponse (pk: IRPacket) : TpNewChaResponse =
            { ErrCode = int16 (pk.ReadInt64()) }

        let tpDelChaRequest (pk: IRPacket) : TpDelChaRequest =
            { ChaIndex = pk.ReadInt64(); Password2 = pk.ReadString()
              GateAddr = uint32 (pk.ReadInt64()); GpAddr = uint32 (pk.ReadInt64()) }

        let tpDelChaResponse (pk: IRPacket) : TpDelChaResponse =
            { ErrCode = int16 (pk.ReadInt64()) }

        let tpChangePassRequest (pk: IRPacket) : TpChangePassRequest =
            { NewPass = pk.ReadString(); Pin = pk.ReadString()
              GateAddr = uint32 (pk.ReadInt64()); GpAddr = uint32 (pk.ReadInt64()) }

        let tpRegisterRequest (pk: IRPacket) : TpRegisterRequest =
            { Username = pk.ReadString(); Password = pk.ReadString(); Email = pk.ReadString() }

        let tpRegisterResponse (pk: IRPacket) : TpRegisterResponse =
            { Result = pk.ReadInt64(); Message = pk.ReadString() }

        let tpCreatePassword2Request (pk: IRPacket) : TpCreatePassword2Request =
            { Password2 = pk.ReadString()
              GateAddr = uint32 (pk.ReadInt64()); GpAddr = uint32 (pk.ReadInt64()) }

        let tpUpdatePassword2Request (pk: IRPacket) : TpUpdatePassword2Request =
            { OldPassword2 = pk.ReadString(); NewPassword2 = pk.ReadString()
              GateAddr = uint32 (pk.ReadInt64()); GpAddr = uint32 (pk.ReadInt64()) }

        let tpPassword2Response (pk: IRPacket) : TpPassword2Response =
            { ErrCode = int16 (pk.ReadInt64()) }

        let tpReqPlyLstResponse (pk: IRPacket) (count: int) : TpReqPlyLstResponse =
            let entries = Array.init count (fun _ ->
                { GtAddr = pk.ReadInt64(); ChaId = pk.ReadInt64() })
            { Entries = entries; PlyNum = pk.ReadInt64() }

        let tpSyncPlyLstRequest (pk: IRPacket) : TpSyncPlyLstRequest =
            let num = pk.ReadInt64()
            let gateName = pk.ReadString()
            let players = Array.init (int num) (fun _ ->
                { GtAddr = pk.ReadInt64(); AcctLoginId = pk.ReadInt64(); AcctId = pk.ReadInt64() })
            { Num = num; GateName = gateName; Players = players }

        let tpSyncPlyLstResponse (pk: IRPacket) : TpSyncPlyLstResponse =
            let errCode = int16 (pk.ReadInt64())
            let num = pk.ReadInt64()
            let results = Array.init (int num) (fun _ ->
                { Ok = pk.ReadInt64() <> 0L; PlayerPtr = pk.ReadInt64() })
            { ErrCode = errCode; Num = num; Results = results }

        let osLoginRequest (pk: IRPacket) : OsLoginRequest =
            { Version = pk.ReadInt64(); AgentName = pk.ReadString() }

        let osLoginResponse (pk: IRPacket) : OsLoginResponse =
            { ErrCode = int16 (pk.ReadInt64()) }

        let tpDiscMessage (pk: IRPacket) : TpDiscMessage =
            { ActId = pk.ReadInt64(); IpAddr = pk.ReadInt64(); Reason = pk.ReadString() }

        // ─── Группа B: PA/AP ──────────────────────────────────

        let paLoginRequest (pk: IRPacket) : PaLoginRequest =
            { ServerName = pk.ReadString(); ServerPassword = pk.ReadString() }

        let paLoginResponse (pk: IRPacket) : PaLoginResponse =
            { ErrCode = int16 (pk.ReadInt64()) }

        let paUserLoginRequest (pk: IRPacket) : PaUserLoginRequest =
            { Username = pk.ReadString(); Password = pk.ReadString()
              Mac = pk.ReadString(); ClientIp = pk.ReadInt64() }

        let paUserLoginResponse (pk: IRPacket) : PaUserLoginResponse =
            let errCode = int16 (pk.ReadInt64())
            if errCode = 0s then
                PaUserLoginSuccess { AcctLoginId = pk.ReadInt64(); SessId = pk.ReadInt64()
                                     AcctId = pk.ReadInt64(); GmLevel = pk.ReadInt64() }
            else PaUserLoginError errCode

        let paUserLogoutMessage (pk: IRPacket) : PaUserLogoutMessage =
            { AcctLoginId = pk.ReadInt64() }

        let paUserBillBgnMessage (pk: IRPacket) : PaUserBillBgnMessage =
            { AcctName = pk.ReadString(); Passport = pk.ReadString() }

        let paUserBillEndMessage (pk: IRPacket) : PaUserBillEndMessage =
            { AcctName = pk.ReadString() }

        let paChangePassMessage (pk: IRPacket) : PaChangePassMessage =
            { Username = pk.ReadString(); NewPassword = pk.ReadString() }

        let paRegisterMessage (pk: IRPacket) : PaRegisterMessage =
            { Username = pk.ReadString(); Password = pk.ReadString(); Email = pk.ReadString() }

        let paGmBanMessage (pk: IRPacket) : PaGmBanMessage =
            { ActName = pk.ReadString() }

        let paGmUnbanMessage (pk: IRPacket) : PaGmUnbanMessage =
            { ActName = pk.ReadString() }

        let paUserDisableMessage (pk: IRPacket) : PaUserDisableMessage =
            { AcctLoginId = pk.ReadInt64(); Minutes = pk.ReadInt64() }

        let apKickUserMessage (pk: IRPacket) : ApKickUserMessage =
            { ErrCode = pk.ReadInt64(); AccountId = pk.ReadInt64() }

        let apExpScaleMessage (pk: IRPacket) : ApExpScaleMessage =
            { ChaId = pk.ReadInt64(); Time = pk.ReadInt64() }

        // ─── Группа C: CP ────────────────────────────────────

        let cpTeamInviteMessage (pk: IRPacket) : CpTeamInviteMessage =
            { InvitedName = pk.ReadString()
              GateAddr = uint32 (pk.ReadInt64()); GpAddr = uint32 (pk.ReadInt64()) }

        let cpTeamAcceptMessage (pk: IRPacket) : CpTeamAcceptMessage =
            { InviterChaId = pk.ReadInt64()
              GateAddr = uint32 (pk.ReadInt64()); GpAddr = uint32 (pk.ReadInt64()) }

        let cpTeamRefuseMessage (pk: IRPacket) : CpTeamRefuseMessage =
            { InviterChaId = pk.ReadInt64()
              GateAddr = uint32 (pk.ReadInt64()); GpAddr = uint32 (pk.ReadInt64()) }

        let cpTeamLeaveMessage (pk: IRPacket) : CpTeamLeaveMessage =
            { GateAddr = uint32 (pk.ReadInt64()); GpAddr = uint32 (pk.ReadInt64()) }

        let cpTeamKickMessage (pk: IRPacket) : CpTeamKickMessage =
            { KickedChaId = pk.ReadInt64()
              GateAddr = uint32 (pk.ReadInt64()); GpAddr = uint32 (pk.ReadInt64()) }

        let cpFrndInviteMessage (pk: IRPacket) : CpFrndInviteMessage =
            { InvitedName = pk.ReadString()
              GateAddr = uint32 (pk.ReadInt64()); GpAddr = uint32 (pk.ReadInt64()) }

        let cpFrndAcceptMessage (pk: IRPacket) : CpFrndAcceptMessage =
            { InviterChaId = pk.ReadInt64()
              GateAddr = uint32 (pk.ReadInt64()); GpAddr = uint32 (pk.ReadInt64()) }

        let cpFrndRefuseMessage (pk: IRPacket) : CpFrndRefuseMessage =
            { InviterChaId = pk.ReadInt64()
              GateAddr = uint32 (pk.ReadInt64()); GpAddr = uint32 (pk.ReadInt64()) }

        let cpFrndDeleteMessage (pk: IRPacket) : CpFrndDeleteMessage =
            { DeletedChaId = pk.ReadInt64()
              GateAddr = uint32 (pk.ReadInt64()); GpAddr = uint32 (pk.ReadInt64()) }

        let cpFrndChangeGroupMessage (pk: IRPacket) : CpFrndChangeGroupMessage =
            { FriendChaId = pk.ReadInt64(); GroupName = pk.ReadString()
              GateAddr = uint32 (pk.ReadInt64()); GpAddr = uint32 (pk.ReadInt64()) }

        let cpFrndRefreshInfoMessage (pk: IRPacket) : CpFrndRefreshInfoMessage =
            { FriendChaId = pk.ReadInt64()
              GateAddr = uint32 (pk.ReadInt64()); GpAddr = uint32 (pk.ReadInt64()) }

        let cpSay2AllMessage (pk: IRPacket) : CpSay2AllMessage =
            { Content = pk.ReadString()
              GateAddr = uint32 (pk.ReadInt64()); GpAddr = uint32 (pk.ReadInt64()) }

        let cpSay2TradeMessage (pk: IRPacket) : CpSay2TradeMessage =
            { Content = pk.ReadString()
              GateAddr = uint32 (pk.ReadInt64()); GpAddr = uint32 (pk.ReadInt64()) }

        let cpSay2YouMessage (pk: IRPacket) : CpSay2YouMessage =
            { TargetName = pk.ReadString(); Content = pk.ReadString()
              GateAddr = uint32 (pk.ReadInt64()); GpAddr = uint32 (pk.ReadInt64()) }

        let cpSay2TemMessage (pk: IRPacket) : CpSay2TemMessage =
            { Content = pk.ReadString()
              GateAddr = uint32 (pk.ReadInt64()); GpAddr = uint32 (pk.ReadInt64()) }

        let cpSay2GudMessage (pk: IRPacket) : CpSay2GudMessage =
            { Content = pk.ReadString()
              GateAddr = uint32 (pk.ReadInt64()); GpAddr = uint32 (pk.ReadInt64()) }

        let cpGm1SayMessage (pk: IRPacket) : CpGm1SayMessage =
            { Content = pk.ReadString()
              GateAddr = uint32 (pk.ReadInt64()); GpAddr = uint32 (pk.ReadInt64()) }

        let cpGm1Say1Message (pk: IRPacket) : CpGm1Say1Message =
            { Content = pk.ReadString(); Color = pk.ReadInt64()
              GateAddr = uint32 (pk.ReadInt64()); GpAddr = uint32 (pk.ReadInt64()) }

        let cpSessCreateMessage (pk: IRPacket) : CpSessCreateMessage =
            let chaNum = pk.ReadInt64()
            let names = Array.init (int chaNum) (fun _ -> pk.ReadString())
            { ChaNum = chaNum; ChaNames = names
              GateAddr = uint32 (pk.ReadInt64()); GpAddr = uint32 (pk.ReadInt64()) }

        let cpSessAddMessage (pk: IRPacket) : CpSessAddMessage =
            { SessId = pk.ReadInt64(); ChaName = pk.ReadString()
              GateAddr = uint32 (pk.ReadInt64()); GpAddr = uint32 (pk.ReadInt64()) }

        let cpSessSayMessage (pk: IRPacket) : CpSessSayMessage =
            { SessId = pk.ReadInt64(); Content = pk.ReadString()
              GateAddr = uint32 (pk.ReadInt64()); GpAddr = uint32 (pk.ReadInt64()) }

        let cpSessLeaveMessage (pk: IRPacket) : CpSessLeaveMessage =
            { SessId = pk.ReadInt64()
              GateAddr = uint32 (pk.ReadInt64()); GpAddr = uint32 (pk.ReadInt64()) }

        let cpChangePersonInfoMessage (pk: IRPacket) : CpChangePersonInfoMessage =
            { Motto = pk.ReadString(); Icon = pk.ReadInt64(); RefuseSess = pk.ReadInt64()
              GateAddr = uint32 (pk.ReadInt64()); GpAddr = uint32 (pk.ReadInt64()) }

        let cpPingMessage (pk: IRPacket) : CpPingMessage =
            { PingValue = pk.ReadInt64()
              GateAddr = uint32 (pk.ReadInt64()); GpAddr = uint32 (pk.ReadInt64()) }

        let cpRefuseToMeMessage (pk: IRPacket) : CpRefuseToMeMessage =
            { RefuseFlag = pk.ReadInt64()
              GateAddr = uint32 (pk.ReadInt64()); GpAddr = uint32 (pk.ReadInt64()) }

        let cpReportWgMessage (pk: IRPacket) : CpReportWgMessage =
            { GateAddr = uint32 (pk.ReadInt64()); GpAddr = uint32 (pk.ReadInt64()) }

        let cpMasterRefreshInfoMessage (pk: IRPacket) : CpMasterRefreshInfoMessage =
            { MasterChaId = pk.ReadInt64()
              GateAddr = uint32 (pk.ReadInt64()); GpAddr = uint32 (pk.ReadInt64()) }

        let cpPrenticeRefreshInfoMessage (pk: IRPacket) : CpPrenticeRefreshInfoMessage =
            { PrenticeChaId = pk.ReadInt64()
              GateAddr = uint32 (pk.ReadInt64()); GpAddr = uint32 (pk.ReadInt64()) }

        let cpChangePassMessage (pk: IRPacket) : CpChangePassMessage =
            { NewPass = pk.ReadString(); Pin = pk.ReadString()
              GateAddr = uint32 (pk.ReadInt64()); GpAddr = uint32 (pk.ReadInt64()) }

        // ─── Группа D: MP ────────────────────────────────────

        let mpEnterMapMessage (pk: IRPacket) : MpEnterMapMessage =
            { IsSwitch = pk.ReadInt64()
              GateAddr = uint32 (pk.ReadInt64()); GpAddr = uint32 (pk.ReadInt64()) }

        let mpTeamCreateMessage (pk: IRPacket) : MpTeamCreateMessage =
            { MemberName = pk.ReadString(); LeaderName = pk.ReadString()
              GateAddr = uint32 (pk.ReadInt64()); GpAddr = uint32 (pk.ReadInt64()) }

        let mpGuildCreateMessage (pk: IRPacket) : MpGuildCreateMessage =
            { GuildId = pk.ReadInt64(); GldName = pk.ReadString(); Job = pk.ReadString()
              Degree = pk.ReadInt64()
              GateAddr = uint32 (pk.ReadInt64()); GpAddr = uint32 (pk.ReadInt64()) }

        let mpGuildApproveMessage (pk: IRPacket) : MpGuildApproveMessage =
            { NewMemberChaId = pk.ReadInt64()
              GateAddr = uint32 (pk.ReadInt64()); GpAddr = uint32 (pk.ReadInt64()) }

        let mpGuildKickMessage (pk: IRPacket) : MpGuildKickMessage =
            { KickedChaId = pk.ReadInt64()
              GateAddr = uint32 (pk.ReadInt64()); GpAddr = uint32 (pk.ReadInt64()) }

        let mpGuildLeaveMessage (pk: IRPacket) : MpGuildLeaveMessage =
            { GateAddr = uint32 (pk.ReadInt64()); GpAddr = uint32 (pk.ReadInt64()) }

        let mpGuildDisbandMessage (pk: IRPacket) : MpGuildDisbandMessage =
            { GateAddr = uint32 (pk.ReadInt64()); GpAddr = uint32 (pk.ReadInt64()) }

        let mpGuildMottoMessage (pk: IRPacket) : MpGuildMottoMessage =
            { Motto = pk.ReadString()
              GateAddr = uint32 (pk.ReadInt64()); GpAddr = uint32 (pk.ReadInt64()) }

        let mpGuildPermMessage (pk: IRPacket) : MpGuildPermMessage =
            { TargetChaId = pk.ReadInt64(); Permission = pk.ReadInt64()
              GateAddr = uint32 (pk.ReadInt64()); GpAddr = uint32 (pk.ReadInt64()) }

        let mpGuildChallMoneyMessage (pk: IRPacket) : MpGuildChallMoneyMessage =
            { GuildId = pk.ReadInt64(); Money = pk.ReadInt64()
              GuildName1 = pk.ReadString(); GuildName2 = pk.ReadString()
              GateAddr = uint32 (pk.ReadInt64()); GpAddr = uint32 (pk.ReadInt64()) }

        let mpGuildChallPrizeMoneyMessage (pk: IRPacket) : MpGuildChallPrizeMoneyMessage =
            { GuildId = pk.ReadInt64(); Money = pk.ReadInt64() }

        let mpMasterCreateMessage (pk: IRPacket) : MpMasterCreateMessage =
            { PrenticeName = pk.ReadString(); PrenticeChaid = pk.ReadInt64()
              MasterName = pk.ReadString(); MasterChaid = pk.ReadInt64() }

        let mpMasterDelMessage (pk: IRPacket) : MpMasterDelMessage =
            { PrenticeName = pk.ReadString(); PrenticeChaid = pk.ReadInt64()
              MasterName = pk.ReadString(); MasterChaid = pk.ReadInt64() }

        let mpMasterFinishMessage (pk: IRPacket) : MpMasterFinishMessage =
            { PrenticeChaid = pk.ReadInt64() }

        let mpSay2AllMessage (pk: IRPacket) : MpSay2AllMessage =
            { Succ = pk.ReadInt64(); ChaName = pk.ReadString(); Content = pk.ReadString()
              GateAddr = uint32 (pk.ReadInt64()); GpAddr = uint32 (pk.ReadInt64()) }

        let mpSay2TradeMessage (pk: IRPacket) : MpSay2TradeMessage =
            { Succ = pk.ReadInt64(); ChaName = pk.ReadString(); Content = pk.ReadString()
              GateAddr = uint32 (pk.ReadInt64()); GpAddr = uint32 (pk.ReadInt64()) }

        let mpGm1SayMessage (pk: IRPacket) : MpGm1SayMessage =
            { Content = pk.ReadString()
              GateAddr = uint32 (pk.ReadInt64()); GpAddr = uint32 (pk.ReadInt64()) }

        let mpGm1Say1Message (pk: IRPacket) : MpGm1Say1Message =
            { Content = pk.ReadString(); SetNum = pk.ReadInt64(); Color = pk.ReadInt64() }

        let mpGmBanMessage (pk: IRPacket) : MpGmBanMessage =
            { ActName = pk.ReadString()
              GateAddr = uint32 (pk.ReadInt64()); GpAddr = uint32 (pk.ReadInt64()) }

        let mpGmUnbanMessage (pk: IRPacket) : MpGmUnbanMessage =
            { ActName = pk.ReadString()
              GateAddr = uint32 (pk.ReadInt64()); GpAddr = uint32 (pk.ReadInt64()) }

        let mpGuildNoticeMessage (pk: IRPacket) : MpGuildNoticeMessage =
            { GuildId = pk.ReadInt64(); Content = pk.ReadString() }

        let mpCanReceiveRequestsMessage (pk: IRPacket) : MpCanReceiveRequestsMessage =
            { ChaId = pk.ReadInt64(); CanSend = pk.ReadInt64() }

        let mpMutePlayerMessage (pk: IRPacket) : MpMutePlayerMessage =
            { ChaName = pk.ReadString(); Time = pk.ReadInt64()
              GateAddr = uint32 (pk.ReadInt64()); GpAddr = uint32 (pk.ReadInt64()) }

        let mpGarner2UpdateMessage (pk: IRPacket) : MpGarner2UpdateMessage =
            { Nid = pk.ReadInt64(); ChaName = pk.ReadString(); Level = pk.ReadInt64()
              Job = pk.ReadString(); Fightpoint = pk.ReadInt64()
              GateAddr = uint32 (pk.ReadInt64()); GpAddr = uint32 (pk.ReadInt64()) }

        let mpGarner2GetOrderMessage (pk: IRPacket) : MpGarner2GetOrderMessage =
            { GateAddr = uint32 (pk.ReadInt64()); GpAddr = uint32 (pk.ReadInt64()) }

        let mpGuildBankAckMessage (pk: IRPacket) : MpGuildBankAckMessage =
            { GuildId = pk.ReadInt64()
              GateAddr = uint32 (pk.ReadInt64()); GpAddr = uint32 (pk.ReadInt64()) }

        // ─── Группа E: PC ────────────────────────────────────

        let pcTeamInviteMessage (pk: IRPacket) : PcTeamInviteMessage =
            { InviterName = pk.ReadString(); ChaId = pk.ReadInt64(); Icon = pk.ReadInt64() }

        let pcTeamRefreshMessage (pk: IRPacket) : PcTeamRefreshMessage =
            let msg = pk.ReadInt64()
            let count = pk.ReadInt64()
            let members = Array.init (int count) (fun _ ->
                { TeamMemberData.ChaId = pk.ReadInt64(); ChaName = pk.ReadString()
                  Motto = pk.ReadString(); Icon = pk.ReadInt64() })
            { Msg = msg; Count = count; Members = members }

        let pcFrndInviteMessage (pk: IRPacket) : PcFrndInviteMessage =
            { InviterName = pk.ReadString(); ChaId = pk.ReadInt64(); Icon = pk.ReadInt64() }

        let pcFrndRefreshMessage (pk: IRPacket) : PcFrndRefreshMessage =
            { Msg = pk.ReadInt64(); Group = pk.ReadString(); ChaId = pk.ReadInt64()
              ChaName = pk.ReadString(); Motto = pk.ReadString(); Icon = pk.ReadInt64() }

        let pcFrndRefreshDelMessage (pk: IRPacket) : PcFrndRefreshDelMessage =
            { Msg = pk.ReadInt64(); ChaId = pk.ReadInt64() }

        let pcFrndChangeGroupMessage (pk: IRPacket) : PcFrndChangeGroupMessage =
            { FriendChaId = pk.ReadInt64(); GroupName = pk.ReadString() }

        let pcFrndRefreshInfoMessage (pk: IRPacket) : PcFrndRefreshInfoMessage =
            { ChaId = pk.ReadInt64(); Motto = pk.ReadString(); Icon = pk.ReadInt64()
              Degree = pk.ReadInt64(); Job = pk.ReadString(); GuildName = pk.ReadString() }

        let pcSay2AllMessage (pk: IRPacket) : PcSay2AllMessage =
            { ChaName = pk.ReadString(); Content = pk.ReadString(); Color = pk.ReadInt64() }

        let pcSay2TradeMessage (pk: IRPacket) : PcSay2TradeMessage =
            { ChaName = pk.ReadString(); Content = pk.ReadString(); Color = pk.ReadInt64() }

        let pcSay2YouMessage (pk: IRPacket) : PcSay2YouMessage =
            { Sender = pk.ReadString(); Target = pk.ReadString()
              Content = pk.ReadString(); Color = pk.ReadInt64() }

        let pcSay2TemMessage (pk: IRPacket) : PcSay2TemMessage =
            { ChaId = pk.ReadInt64(); Content = pk.ReadString(); Color = pk.ReadInt64() }

        let pcSay2GudMessage (pk: IRPacket) : PcSay2GudMessage =
            { ChaName = pk.ReadString(); Content = pk.ReadString(); Color = pk.ReadInt64() }

        let pcGm1SayMessage (pk: IRPacket) : PcGm1SayMessage =
            { ChaName = pk.ReadString(); Content = pk.ReadString() }

        let pcGm1Say1Message (pk: IRPacket) : PcGm1Say1Message =
            { Content = pk.ReadString(); SetNum = pk.ReadInt64(); Color = pk.ReadInt64() }

        let pcGarner2OrderMessage (pk: IRPacket) : PcGarner2OrderMessage =
            let entries = Array.init 5 (fun _ ->
                { PcGarner2OrderEntry.Name = pk.ReadString(); Level = pk.ReadInt64()
                  Job = pk.ReadString(); FightPoint = pk.ReadInt64() })
            { Entries = entries }

        let pcGuildPermMessage (pk: IRPacket) : PcGuildPermMessage =
            { TargetChaId = pk.ReadInt64(); Permission = pk.ReadInt64() }

        let pcMasterRefreshAddMessage (pk: IRPacket) : PcMasterRefreshAddMessage =
            { Msg = pk.ReadInt64(); Group = pk.ReadString(); ChaId = pk.ReadInt64()
              ChaName = pk.ReadString(); Motto = pk.ReadString(); Icon = pk.ReadInt64() }

        let pcMasterRefreshDelMessage (pk: IRPacket) : PcMasterRefreshDelMessage =
            { Msg = pk.ReadInt64(); ChaId = pk.ReadInt64() }

        let pcSessCreateMessage (pk: IRPacket) : PcSessCreateMessage =
            let sessId = pk.ReadInt64()
            let count = int (pk.ReadInt64())
            let members = Array.init count (fun _ ->
                { SessMemberData.ChaId = pk.ReadInt64(); ChaName = pk.ReadString()
                  Motto = pk.ReadString(); Icon = pk.ReadInt64() })
            { SessId = sessId; Members = members; NotiPlyCount = pk.ReadInt64() }

        let pcSessAddMessage (pk: IRPacket) : PcSessAddMessage =
            { SessId = pk.ReadInt64(); ChaId = pk.ReadInt64()
              ChaName = pk.ReadString(); Motto = pk.ReadString(); Icon = pk.ReadInt64() }

        let pcSessSayMessage (pk: IRPacket) : PcSessSayMessage =
            { SessId = pk.ReadInt64(); ChaId = pk.ReadInt64(); Content = pk.ReadString() }

        let pcSessLeaveMessage (pk: IRPacket) : PcSessLeaveMessage =
            { SessId = pk.ReadInt64(); ChaId = pk.ReadInt64() }

        let pcChangePersonInfoMessage (pk: IRPacket) : PcChangePersonInfoMessage =
            { Motto = pk.ReadString(); Icon = pk.ReadInt64(); RefuseSess = pk.ReadInt64() }

        let pcErrMsgMessage (pk: IRPacket) : PcErrMsgMessage =
            { Message = pk.ReadString() }

        let pcGuildNoticeMessage (pk: IRPacket) : PcGuildNoticeMessage =
            { Content = pk.ReadString() }

        let pcRegisterMessage (pk: IRPacket) : PcRegisterMessage =
            { Result = pk.ReadInt64(); Message = pk.ReadString() }

        // ─── Группа F: PM ────────────────────────────────────

        let pmTeamMessage (pk: IRPacket) : PmTeamMessage =
            let msg = pk.ReadInt64()
            let count = pk.ReadInt64()
            let members = Array.init (int count) (fun _ ->
                { GateName = pk.ReadString(); GtAddr = pk.ReadInt64(); ChaId = pk.ReadInt64() })
            { Msg = msg; Count = count; Members = members }

        let pmExpScaleMessage (pk: IRPacket) : PmExpScaleMessage =
            { ChaId = pk.ReadInt64(); Time = pk.ReadInt64() }

        let pmSay2AllMessage (pk: IRPacket) : PmSay2AllMessage =
            { ChaId = pk.ReadInt64(); Content = pk.ReadString(); Money = pk.ReadInt64() }

        let pmSay2TradeMessage (pk: IRPacket) : PmSay2TradeMessage =
            { ChaId = pk.ReadInt64(); Content = pk.ReadString(); Money = pk.ReadInt64() }

        let pmGuildDisbandMessage (pk: IRPacket) : PmGuildDisbandMessage =
            { GuildId = pk.ReadInt64() }

        let pmGuildChallMoneyMessage (pk: IRPacket) : PmGuildChallMoneyMessage =
            { LeaderId = pk.ReadInt64(); Money = pk.ReadInt64()
              GuildName1 = pk.ReadString(); GuildName2 = pk.ReadString() }

        let pmGuildChallPrizeMoneyMessage (pk: IRPacket) : PmGuildChallPrizeMoneyMessage =
            { LeaderId = pk.ReadInt64(); Money = pk.ReadInt64() }

        let ptKickUserMessage (pk: IRPacket) : PtKickUserMessage =
            { GpAddr = pk.ReadInt64(); GtAddr = pk.ReadInt64() }

        // ─── Группа G: CM ────────────────────────────────────

        let cmLoginRequest (pk: IRPacket) : CmLoginRequest =
            { AcctName = pk.ReadString(); PasswordHash = pk.ReadString(); Mac = pk.ReadString()
              CheatMarker = pk.ReadInt64(); ClientVersion = pk.ReadInt64() }

        let mcLoginResponse (pk: IRPacket) : McLoginResponse =
            let errCode = int16 (pk.ReadInt64())
            if errCode <> 0s then McLoginError errCode
            else
                let maxChaNum = pk.ReadInt64()
                let charCount = pk.ReadInt64()
                let chars = Array.init (int charCount) (fun _ ->
                    let valid = pk.ReadInt64() <> 0L
                    if valid then
                        { Valid = true; ChaName = pk.ReadString(); Job = pk.ReadString()
                          Degree = pk.ReadInt64(); TypeId = pk.ReadInt64()
                          EquipIds = Array.init EQUIP_NUM (fun _ -> pk.ReadInt64()) }
                    else
                        { Valid = false; ChaName = ""; Job = ""; Degree = 0L; TypeId = 0L
                          EquipIds = Array.empty })
                let hasPassword2 = pk.ReadInt64() <> 0L
                McLoginSuccess { MaxChaNum = maxChaNum; Characters = chars; HasPassword2 = hasPassword2 }
