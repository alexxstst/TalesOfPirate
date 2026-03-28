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

    /// Отмена приглашения в команду (причина + chaId инициатора).
    [<Struct>]
    type PcTeamCancelMessage = { Reason: int64; ChaId: int64 }

    type PcFrndInviteMessage = { InviterName: string; ChaId: int64; Icon: int64 }

    /// Отмена приглашения в друзья (причина + chaId инициатора).
    [<Struct>]
    type PcFrndCancelMessage = { Reason: int64; ChaId: int64 }

    type PcFrndRefreshMessage =
        { Msg: int64; Group: string; ChaId: int64; ChaName: string; Motto: string; Icon: int64 }

    [<Struct>]
    type PcFrndRefreshDelMessage = { Msg: int64; ChaId: int64 }

    type PcFrndChangeGroupMessage = { FriendChaId: int64; GroupName: string }

    /// Запись друга/GM: chaId, имя, девиз, иконка, статус.
    type GmFrndEntry =
        { ChaId: int64; ChaName: string; Motto: string; Icon: int64; Status: int64 }

    /// Запись добавления друга/GM: группа, chaId, имя, девиз, иконка.
    type GmFrndAddEntry =
        { Group: string; ChaId: int64; ChaName: string; Motto: string; Icon: int64 }

    /// Тегированное сообщение PC_GM_INFO.
    /// Type определяет, какие поля заполнены:
    ///  START(1) — Entries; ONLINE(4)/OFFLINE(5)/DEL(3) — ChaId; ADD(2) — AddEntry.
    type PcGmInfoMessage =
        { Type: int64
          Entries: GmFrndEntry[]
          ChaId: int64
          AddEntry: GmFrndAddEntry }

    /// Данные о себе в списке друзей (FRND_REFRESH START).
    [<Struct>]
    type FrndSelfData =
        { ChaId: int64; ChaName: string; Motto: string; Icon: int64 }

    /// Группа друзей: имя группы + список участников.
    type FrndGroupData =
        { GroupName: string; Members: GmFrndEntry[] }

    /// Тегированное сообщение PC_FRND_REFRESH.
    /// Type определяет, какие поля заполнены:
    ///  START(1) — Self + Groups; ONLINE(4)/OFFLINE(5)/DEL(3) — ChaId; ADD(2) — AddEntry.
    type PcFrndRefreshFullMessage =
        { Type: int64
          ChaId: int64
          AddEntry: GmFrndAddEntry
          Self: FrndSelfData
          Groups: FrndGroupData[] }

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

    /// Запись участника гильдии (CMD_PC_GUILD: MSG_GUILD_START / MSG_GUILD_ADD).
    type GuildMemberEntry =
        { Online: int64; ChaId: int64; ChaName: string; Motto: string
          Job: string; Degree: int64; Icon: int64; Permission: int64 }

    /// CMD_PC_GUILD — составное сообщение гильдии (подкоманда в Msg).
    type PcGuildMessage =
        { Msg: int64
          ChaId: int64
          PacketIndex: int64
          GuildId: int64; GuildName: string; LeaderId: int64
          Members: GuildMemberEntry[]
          AddMember: GuildMemberEntry }

    type PcMasterRefreshAddMessage =
        { Msg: int64; Group: string; ChaId: int64; ChaName: string; Motto: string; Icon: int64 }

    [<Struct>]
    type PcMasterRefreshDelMessage = { Msg: int64; ChaId: int64 }

    type PcSessCreateMessage =
        { SessId: int64; ErrorMsg: string; Members: SessMemberData[]; NotiPlyCount: int64 }

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
    //  Группа H: GateServer → GameServer (TM) — async
    // ═══════════════════════════════════════════════════════════════

    type TmEnterMapMessage =
        { ActId: uint32; Password: string; DatabaseId: uint32; WorldId: uint32
          MapName: string; MapCopyNo: int; X: uint32; Y: uint32; IsSwitch: bool
          GateAddr: uint32; GarnerWinner: int16 }

    // ═══════════════════════════════════════════════════════════════
    //  Группа: Геймплейные команды Client ↔ GameServer
    // ═══════════════════════════════════════════════════════════════

    // ─── CM — пустые (cmd-only) ────────────────────────────────
    // Для cmd-only команд сериализаторы пишут только WriteCmd, десериализаторы — пустые.
    // Типы-маркеры не нужны: сериализатор возвращает WPacket с одним cmd.

    // ─── CM — простые (1-3 поля) ───────────────────────────────

    [<Struct>]
    type CmDieReturnMessage = { ReliveType: int64 }

    [<Struct>]
    type CmAutoKitbagLockMessage = { AutoLock: int64 }

    [<Struct>]
    type CmStallSearchMessage = { ItemId: int64 }

    [<Struct>]
    type CmForgeItemMessage = { Index: int64 }

    [<Struct>]
    type CmEntityEventMessage = { EntityId: int64 }

    [<Struct>]
    type CmStallOpenMessage = { CharId: int64 }

    [<Struct>]
    type CmMisLogInfoMessage = { Id: int64 }

    [<Struct>]
    type CmMisLogClearMessage = { Id: int64 }

    [<Struct>]
    type CmStoreBuyAskMessage = { ComId: int64 }

    [<Struct>]
    type CmStoreChangeAskMessage = { Num: int64 }

    [<Struct>]
    type CmStoreQueryMessage = { Num: int64 }

    [<Struct>]
    type CmTeamFightAnswerMessage = { Accept: int64 }

    [<Struct>]
    type CmItemRepairAnswerMessage = { Accept: int64 }

    [<Struct>]
    type CmItemForgeAnswerMessage = { Accept: int64 }

    [<Struct>]
    type CmItemLotteryAnswerMessage = { Accept: int64 }

    [<Struct>]
    type CmVolunteerOpenMessage = { Num: int64 }

    [<Struct>]
    type CmVolunteerListMessage = { Page: int64; Num: int64 }

    [<Struct>]
    type CmStoreListAskMessage = { ClsId: int64; Page: int64; Num: int64 }

    [<Struct>]
    type CmCaptainConfirmAsrMessage = { Ret: int64; TeamId: int64 }

    type CmMapMaskMessage = { MapName: string }

    type CmStoreOpenAskMessage = { Password: string }

    type CmVolunteerSelMessage = { Name: string }

    type CmKitbagUnlockMessage = { Password: string }

    // ─── MC — простые ──────────────────────────────────────────

    [<Struct>]
    type McFailedActionMessage = { WorldId: int64; ActionType: int64; Reason: int64 }

    [<Struct>]
    type McChaEndSeeMessage = { SeeType: int64; WorldId: int64 }

    [<Struct>]
    type McItemDestroyMessage = { WorldId: int64 }

    [<Struct>]
    type McForgeResultMessage = { Result: int64 }

    [<Struct>]
    type McUniteResultMessage = { Result: int64 }

    [<Struct>]
    type McMillingResultMessage = { Result: int64 }

    [<Struct>]
    type McFusionResultMessage = { Result: int64 }

    [<Struct>]
    type McUpgradeResultMessage = { Result: int64 }

    [<Struct>]
    type McPurifyResultMessage = { Result: int64 }

    [<Struct>]
    type McFixResultMessage = { Result: int64 }

    [<Struct>]
    type McEidolonMetempsychosisMessage = { Result: int64 }

    [<Struct>]
    type McEidolonFusionMessage = { Result: int64 }

    type McMessageMessage = { Text: string }

    type McBickerNoticeMessage = { Text: string }

    type McColourNoticeMessage = { Color: int64; Text: string }

    [<Struct>]
    type McTriggerActionMessage = { Type: int64; Id: int64; Num: int64; Count: int64 }

    [<Struct>]
    type McNpcStateChangeMessage = { NpcId: int64; State: int64 }

    [<Struct>]
    type McEntityStateChangeMessage = { EntityId: int64; State: int64 }

    [<Struct>]
    type McCloseTalkMessage = { NpcId: int64 }

    [<Struct>]
    type McKitbagCheckAnswerMessage = { Locked: int64 }

    [<Struct>]
    type McPreMoveTimeMessage = { Time: int64 }

    [<Struct>]
    type McItemUseSuccMessage = { WorldId: int64; ItemId: int64 }

    [<Struct>]
    type McChaPlayEffectMessage = { WorldId: int64; EffectId: int64 }

    [<Struct>]
    type McSynDefaultSkillMessage = { WorldId: int64; SkillId: int64 }

    // ─── MC — ReadSequence→ReadString миграция ─────────────────

    type McSayMessage = { SourceId: int64; Content: string; Color: int64 }

    type McSysInfoMessage = { Info: string }

    type McPopupNoticeMessage = { Notice: string }

    type McPingMessage = { V1: int64; V2: int64; V3: int64; V4: int64; V5: int64 }

    // ─── Фаза 2: CM — средние ─────────────────────────────────

    type CmChangePassMessage = { Pass: string; Pin: string }

    type CmGuildBankOperMessage =
        { Op: int64; SrcType: int64; SrcId: int64; SrcNum: int64; TarType: int64; TarId: int64 }

    [<Struct>]
    type CmGuildBankGoldMessage = { Op: int64; Direction: int64; Gold: int64 }

    type CmUpdateHairMessage = { ScriptId: int64; GridLoc0: int64; GridLoc1: int64; GridLoc2: int64; GridLoc3: int64 }

    [<Struct>]
    type CmTeamFightAskMessage = { Type: int64; WorldId: int64; Handle: int64 }

    [<Struct>]
    type CmItemRepairAskMessage = { RepairmanId: int64; RepairmanHandle: int64; PosType: int64; PosId: int64 }

    [<Struct>]
    type CmRequestTradeMessage = { Type: int64; CharId: int64 }

    [<Struct>]
    type CmAcceptTradeMessage = { Type: int64; CharId: int64 }

    [<Struct>]
    type CmCancelTradeMessage = { Type: int64; CharId: int64 }

    [<Struct>]
    type CmValidateTradeDataMessage = { Type: int64; CharId: int64 }

    [<Struct>]
    type CmValidateTradeMessage = { Type: int64; CharId: int64 }

    type CmAddItemMessage =
        { Type: int64; CharId: int64; OpType: int64; Index: int64; ItemIndex: int64; Count: int64 }

    type CmAddMoneyMessage =
        { Type: int64; CharId: int64; OpType: int64; IsImp: int64; Money: int64 }

    [<Struct>]
    type CmTigerStartMessage = { NpcId: int64; Sel1: int64; Sel2: int64; Sel3: int64 }

    [<Struct>]
    type CmTigerStopMessage = { NpcId: int64; Num: int64 }

    type CmVolunteerAsrMessage = { Ret: int64; Name: string }

    type CmCreateBoatMessage = { Boat: string; Header: int64; Engine: int64; Cannon: int64; Equipment: int64 }

    [<Struct>]
    type CmUpdateBoatMessage = { Header: int64; Engine: int64; Cannon: int64; Equipment: int64 }

    [<Struct>]
    type CmSelectBoatListMessage = { NpcId: int64; Type: int64; Index: int64 }

    [<Struct>]
    type CmBoatLaunchMessage = { NpcId: int64; Index: int64 }

    [<Struct>]
    type CmBoatBagSelMessage = { NpcId: int64; Index: int64 }

    type CmReportWgMessage = { Info: string }

    type CmSay2CampMessage = { Content: string }

    [<Struct>]
    type CmStallBuyMessage = { CharId: int64; Index: int64; Count: int64; GridId: int64 }

    [<Struct>]
    type CmSkillUpgradeMessage = { SkillId: int64; AddGrade: int64 }

    [<Struct>]
    type CmRefreshDataMessage = { WorldId: int64; Handle: int64 }

    [<Struct>]
    type CmPkCtrlMessage = { Ctrl: int64 }

    [<Struct>]
    type CmItemAmphitheaterAskMessage = { Sure: int64; ReId: int64 }

    type CmMasterInviteMessage = { Name: string; ChaId: int64 }

    type CmMasterAsrMessage = { Agree: int64; Name: string; ChaId: int64 }

    type CmMasterDelMessage = { Name: string; ChaId: int64 }

    type CmPrenticeInviteMessage = { Name: string; ChaId: int64 }

    type CmPrenticeAsrMessage = { Agree: int64; Name: string; ChaId: int64 }

    type CmPrenticeDelMessage = { Name: string; ChaId: int64 }

    // ─── Фаза 2: NPC Talk compound commands ────────────────────

    /// ROLE_TRADE_SALE = 0 — продажа NPC
    [<Literal>]
    let ROLE_TRADE_SALE = 0L

    /// ROLE_TRADE_BUY = 1 — покупка у NPC
    [<Literal>]
    let ROLE_TRADE_BUY = 1L

    /// ROLE_MIS_SEL = 0 — выбор квеста
    [<Literal>]
    let ROLE_MIS_SEL = 0L

    [<Struct>]
    type CmRequestTalkMessage = { NpcId: int64; Cmd: int64 }

    [<Struct>]
    type CmSelFunctionMessage = { NpcId: int64; PageId: int64; Index: int64 }

    [<Struct>]
    type CmSaleMessage = { NpcId: int64; Index: int64; Count: int64 }

    type CmBuyMessage = { NpcId: int64; ItemType: int64; Index1: int64; Index2: int64; Count: int64 }

    [<Struct>]
    type CmMissionPageMessage = { NpcId: int64; Cmd: int64; SelItem: int64; Param: int64 }

    [<Struct>]
    type CmSelMissionMessage = { NpcId: int64; Index: int64 }

    type CmBlackMarketExchangeReqMessage =
        { NpcId: int64; TimeNum: int64; SrcId: int64; SrcNum: int64; TarId: int64; TarNum: int64; Index: int64 }

    // ─── Фаза 2: MC — средние ─────────────────────────────────

    type McSay2CampMessage = { ChaName: string; Content: string }

    type McTalkInfoMessage = { NpcId: int64; Cmd: int64; Text: string }

    type McTradeDataMessage =
        { NpcId: int64; Page: int64; Index: int64; ItemId: int64; Count: int64; Price: int64 }

    type McTradeResultMessage =
        { Type: int64; Index: int64; Count: int64; ItemId: int64; Money: int64 }

    type McUpdateHairResMessage = { WorldId: int64; ScriptId: int64; Reason: string }

    [<Struct>]
    type McSynTLeaderIdMessage = { WorldId: int64; LeaderId: int64 }

    [<Struct>]
    type McKitbagCapacityMessage = { WorldId: int64; Capacity: int64 }

    [<Struct>]
    type McItemForgeAskMessage = { Type: int64; Money: int64 }

    [<Struct>]
    type McItemForgeAnswerMessage = { WorldId: int64; Type: int64; Result: int64 }

    type McQueryChaMessage =
        { ChaId: int64; Name: string; MapName: string; PosX: int64; PosY: int64; ChaId2: int64 }

    type McQueryChaPingMessage = { ChaId: int64; Name: string; MapName: string; Ping: int64 }

    type McQueryReliveMessage = { ChaId: int64; SourceName: string; ReliveType: int64 }

    type McGmMailMessage = { Title: string; Content: string; Time: int64 }

    type McSynStallNameMessage = { CharId: int64; Name: string }

    type McMapCrashMessage = { Text: string }

    [<Struct>]
    type McVolunteerStateMessage = { State: int64 }

    type McVolunteerAskMessage = { Name: string }

    type McMasterAskMessage = { Name: string; ChaId: int64 }

    type McPrenticeAskMessage = { Name: string; ChaId: int64 }

    type McItemRepairAskMessage = { ItemName: string; RepairCost: int64 }

    [<Struct>]
    type McItemLotteryAsrMessage = { Success: int64 }

    // ─── MC/CM — Фаза 4: простые ────────────────────────────────

    [<Struct>]
    type McChaEmotionMessage = { WorldId: int64; Emotion: int64 }

    [<Struct>]
    type McStartExitMessage = { ExitTime: int64 }

    // CMD_MC_CANCELEXIT — cmd-only (без полей)
    // CMD_MC_OPENHAIR — cmd-only (без полей)
    // CMD_MC_BEGIN_ITEM_REPAIR — cmd-only (без полей)
    // CMD_MC_BEGIN_GM_SEND — cmd-only (без полей)

    [<Struct>]
    type McGmRecvMessage = { NpcId: int64 }

    [<Struct>]
    type McStallDelGoodsMessage = { CharId: int64; Grid: int64; Count: int64 }

    [<Struct>]
    type McStallCloseMessage = { CharId: int64 }

    [<Struct>]
    type McStallSuccessMessage = { CharId: int64 }

    type McUpdateGuildGoldMessage = { Data: string }

    [<Struct>]
    type McQueryChaItemMessage = { ChaId: int64 }

    [<Struct>]
    type McDisconnectMessage = { Reason: int64 }

    [<Struct>]
    type McLifeSkillShowMessage = { Type: int64 }

    type McLifeSkillMessage = { Type: int64; Result: int64; Text: string }

    type McLifeSkillAsrMessage = { Type: int64; Time: int64; Text: string }

    [<Struct>]
    type McDropLockAsrMessage = { Success: int64 }

    [<Struct>]
    type McUnlockItemAsrMessage = { Result: int64 }

    [<Struct>]
    type McStoreBuyAnswerMessage = { Success: int64; NewMoney: int64 }

    [<Struct>]
    type McStoreChangeAnswerMessage = { Success: int64; MoBean: int64; ReplMoney: int64 }

    type McDailyBuffInfoMessage = { ImgName: string; LabelInfo: string }

    [<Struct>]
    type McRequestDropRateMessage = { Rate: float32 }

    [<Struct>]
    type McRequestExpRateMessage = { Rate: float32 }

    [<Struct>]
    type McTigerItemIdMessage = { Num: int64; ItemId0: int64; ItemId1: int64; ItemId2: int64 }

    // ─── Гильдейские команды (CM/MC) ─────────────────────────────

    // CM — клиент → сервер
    type CmGuildPutNameMessage = { Confirm: int64; GuildName: string; Passwd: string }

    [<Struct>]
    type CmGuildTryForMessage = { GuildId: int64 }

    [<Struct>]
    type CmGuildTryForCfmMessage = { Confirm: int64 }

    // CMD_CM_GUILD_LISTTRYPLAYER — cmd-only, без полей

    [<Struct>]
    type CmGuildApproveMessage = { ChaId: int64 }

    [<Struct>]
    type CmGuildRejectMessage = { ChaId: int64 }

    [<Struct>]
    type CmGuildKickMessage = { ChaId: int64 }

    // CMD_CM_GUILD_LEAVE — cmd-only, без полей

    type CmGuildDisbandMessage = { Passwd: string }

    type CmGuildMottoMessage = { Motto: string }

    [<Struct>]
    type CmGuildChallMessage = { Level: int64; Money: int64 }

    [<Struct>]
    type CmGuildLeizhuMessage = { Level: int64; Money: int64 }

    // MC — сервер → клиент
    // CMD_MC_GUILD_GETNAME — cmd-only, без полей

    type McGuildTryForCfmMessage = { Name: string }

    type McGuildMottoMessage = { Motto: string }

    // CMD_MC_GUILD_LEAVE — cmd-only, без полей
    // CMD_MC_GUILD_KICK — cmd-only, без полей

    type McGuildInfoMessage =
        { CharId: int64; GuildId: int64; GuildName: string
          GuildMotto: string; GuildPermission: int64 }

    /// Запись уровня вызова гильдий (условная по Level <> 0).
    type GuildChallEntry =
        { Level: int64; Start: int64; GuildName: string; ChallName: string; Money: int64 }

    type McGuildListChallMessage = { IsLeader: int64; Entries: GuildChallEntry[] }

    // ═══════════════════════════════════════════════════════════════
    //  Sub-packet типы (для вложенных структур)
    // ═══════════════════════════════════════════════════════════════

    [<Struct>]
    type ChaSideInfo = { SideId: int64 }

    type ChaEventInfo = { EntityId: int64; EntityType: int64; EventId: int64; EventName: string }

    /// Константы типов потребностей квеста (mission::MIS_NEED_TYPE).
    [<Literal>]
    let MIS_NEED_ITEM = 0L
    [<Literal>]
    let MIS_NEED_KILL = 1L
    [<Literal>]
    let MIS_NEED_DESP = 5L

    /// Количество параметров дальности скилла (defSKILL_RANGE_PARAM_NUM).
    [<Literal>]
    let SKILL_RANGE_PARAM_NUM = 4

    /// Запись потребности квеста (условная сериализация по NeedType).
    /// Для MIS_NEED_ITEM/MIS_NEED_KILL: Param1..Param3, Desp = "".
    /// Для MIS_NEED_DESP: Desp, Param1..3 = 0.
    type MisNeedEntry =
        { NeedType: int64; Param1: int64; Param2: int64; Param3: int64; Desp: string }

    /// Запись награды квеста.
    [<Struct>]
    type MisPrizeEntry = { Type: int64; Param1: int64; Param2: int64 }

    /// Запись скилла в сумке (ReadChaSkillBagPacket).
    type SkillEntry =
        { Id: int64; State: int64; Level: int64; UseSp: int64; UseEndure: int64
          UseEnergy: int64; ResumeTime: int64; Range: int64[] }

    /// Сумка скиллов (ReadChaSkillBagPacket).
    type ChaSkillBagInfo = { DefSkillId: int64; SynType: int64; Skills: SkillEntry[] }

    [<Struct>]
    type AttrEntry = { AttrId: int64; AttrVal: int64 }

    type ChaAttrInfo = { SynType: int64; Attrs: AttrEntry[] }

    [<Struct>]
    type SkillStateEntry = { StateId: int64; StateLv: int64; Duration: int64; StartTime: int64 }

    type ChaSkillStateInfo = { CurrentTime: int64; States: SkillStateEntry[] }

    [<Literal>]
    let ESPE_KBGRID_NUM = 4
    [<Literal>]
    let ITEM_INSTANCE_ATTR_NUM = 5
    [<Literal>]
    let SHORT_CUT_NUM = 36
    [<Literal>]
    let SYN_LOOK_CHANGE = 1L
    [<Literal>]
    let SYN_KITBAG_INIT = 0L
    [<Literal>]
    let RANGE_TYPE_NONE = 0L

    /// Слот экипировки во внешнем виде.
    type LookEquipSlot =
        { Id: int64; DbId: int64; NeedLv: int64
          Num: int64; Endure0: int64; Endure1: int64; Energy0: int64; Energy1: int64
          ForgeLv: int64; Valid: int64; Tradable: int64; Expiration: int64
          HasExtra: bool; ForgeParam: int64; InstId: int64
          HasInstAttr: bool; InstAttr: (int64 * int64)[] }

    /// Части корабля.
    type BoatLookParts =
        { PosId: int64; BoatId: int64; Header: int64; Body: int64
          Engine: int64; Cannon: int64; Equipment: int64 }

    /// Внешний вид персонажа/корабля.
    type ChaLookInfo =
        { SynType: int64; TypeId: int64; IsBoat: bool
          BoatParts: BoatLookParts voption  // Some если isBoat
          HairId: int64                      // только если !isBoat
          Equips: LookEquipSlot[] }          // только если !isBoat

    /// Слот доп. внешнего вида (костюмы).
    [<Struct>]
    type AppendLookSlot = { LookId: int64; Valid: int64 }

    /// Базовая информация о персонаже.
    type ChaBaseInfo =
        { ChaId: int64; WorldId: int64; CommId: int64; CommName: string
          GmLv: int64; Handle: int64; CtrlType: int64
          Name: string; Motto: string; Icon: int64; GuildId: int64
          GuildName: string; GuildMotto: string; GuildPermission: int64
          StallName: string; State: int64
          PosX: int64; PosY: int64; Radius: int64; Angle: int64
          TeamLeaderId: int64; IsPlayer: int64
          Side: ChaSideInfo; Event: ChaEventInfo
          Look: ChaLookInfo; PkCtrl: int64
          AppendLook: AppendLookSlot[] }

    /// Предмет в инвентаре.
    type KitbagItem =
        { GridId: int64; ItemId: int64
          DbId: int64; NeedLv: int64; Num: int64
          Endure0: int64; Endure1: int64; Energy0: int64; Energy1: int64
          ForgeLv: int64; Valid: int64; Tradable: int64; Expiration: int64
          IsBoat: bool; BoatWorldId: int64
          ForgeParam: int64; InstId: int64
          HasInstAttr: bool; InstAttr: (int64 * int64)[] }

    /// Инвентарь.
    type ChaKitbagInfo =
        { SynType: int64; Capacity: int64; Items: KitbagItem[] }

    /// Запись быстрой клавиши.
    [<Struct>]
    type ShortcutEntry = { Type: int64; GridId: int64 }

    /// Панель быстрого доступа.
    type ChaShortcutInfo = { Entries: ShortcutEntry[] }

    // ─── McChaBeginSee и связанные ────────────────────────────

    type PoseType = | PoseNone = 0 | PoseLean = 1 | PoseSeat = 2

    type LeanInfo =
        { LeanState: int64; Pose: int64; Angle: int64; PosX: int64; PosY: int64; Height: int64 }

    [<Struct>]
    type SeatInfo = { SeatAngle: int64; SeatPose: int64 }

    type McChaBeginSeeMessage =
        { SeeType: int64; Base: ChaBaseInfo
          NpcType: int64; NpcState: int64
          PoseType: int64
          Lean: LeanInfo voption   // если poseType == PoseLean
          Seat: SeatInfo voption   // если poseType == PoseSeat
          Attr: ChaAttrInfo; SkillState: ChaSkillStateInfo }

    type McAddItemChaMessage =
        { MainChaId: int64; Base: ChaBaseInfo; Attr: ChaAttrInfo
          Kitbag: ChaKitbagInfo; SkillState: ChaSkillStateInfo }

    // ─── McCharacterAction (CMD_MC_NOTIACTION) ──────────────────

    /// Константы типов действий (enumACTION_*).
    [<Literal>] let ACT_NONE       = 0L
    [<Literal>] let ACT_MOVE       = 1L
    [<Literal>] let ACT_SKILL      = 2L
    [<Literal>] let ACT_SKILL_SRC  = 3L
    [<Literal>] let ACT_SKILL_TAR  = 4L
    [<Literal>] let ACT_LOOK       = 5L
    [<Literal>] let ACT_KITBAG     = 6L
    [<Literal>] let ACT_ITEM_FAILED = 15L
    [<Literal>] let ACT_LEAN       = 16L
    [<Literal>] let ACT_CHANGE_CHA = 17L
    [<Literal>] let ACT_FACE       = 19L
    [<Literal>] let ACT_SKILL_POSE = 21L
    [<Literal>] let ACT_TEMP       = 24L
    [<Literal>] let ACT_SHORTCUT   = 25L
    [<Literal>] let ACT_BANK       = 26L
    [<Literal>] let ACT_KITBAGTMP  = 28L
    [<Literal>] let ACT_GUILDBANK  = 30L

    /// Константы состояний движения/боя.
    [<Literal>]
    let enumMSTATE_ON = 0x00L
    [<Literal>]
    let enumFSTATE_ON = 0x0000L

    // AttrEntry = AttrEntry (идентичная структура { AttrId; AttrVal }).

    /// Запись состояния скилла (в SKILL_SRC).
    [<Struct>]
    type ActionStateEntry = { AStateId: int64; AStateLv: int64 }

    /// Запись состояния скилла в SKILL_TAR (4 поля на запись).
    [<Struct>]
    type ActionTarStateEntry =
        { TarStateId: int64; TarStateLv: int64; TarDuration: int64; TarStartTime: int64 }

    /// Данные действия MOVE.
    type ActionMoveData =
        { MoveState: int64; StopState: int64; Waypoints: byte[] }

    /// Данные действия SKILL_SRC (источник скилла).
    type ActionSkillSrcData =
        { FightId: int64; Angle: int64; State: int64; StopState: int64
          SkillId: int64; SkillSpeed: int64
          TargetType: int64; TargetId: int64; TargetX: int64; TargetY: int64
          ExecTime: int64; Effects: AttrEntry[]; States: ActionStateEntry[] }

    /// Данные действия SKILL_TAR (цель скилла).
    type ActionSkillTarData =
        { FightId: int64; State: int64; DoubleAttack: bool; Miss: bool
          BeatBack: bool; BeatBackX: int64; BeatBackY: int64
          SrcId: int64; SrcPosX: int64; SrcPosY: int64
          SkillId: int64; SkillTargetX: int64; SkillTargetY: int64
          ExecTime: int64
          SynType: int64
          Effects: AttrEntry[]
          HasStates: bool; StateTime: int64; States: ActionTarStateEntry[]
          HasSrcEffect: bool; SrcState: int64; SrcSynType: int64; SrcEffects: AttrEntry[] }

    /// Данные действия LEAN (наклон/присаживание).
    type ActionLeanData =
        { ActionLeanState: int64; ActionPose: int64; ActionAngle: int64
          ActionPosX: int64; ActionPosY: int64; ActionHeight: int64 }

    /// Данные действия FACE / SKILL_POSE.
    [<Struct>]
    type ActionFaceData = { FaceAngle: int64; FacePose: int64 }

    /// Размеченное объединение — вариант данных действия.
    type McCharacterActionData =
        | ActionMove of ActionMoveData
        | ActionSkillSrc of ActionSkillSrcData
        | ActionSkillTar of ActionSkillTarData
        | ActionLean of ActionLeanData
        | ActionFace of ActionFaceData
        | ActionSkillPose of ActionFaceData
        | ActionItemFailed of failedId: int64
        | ActionTemp of itemId: int64 * partId: int64
        | ActionChangeCha of mainChaId: int64
        | ActionLook of ChaLookInfo
        | ActionKitbag of ChaKitbagInfo
        | ActionBank of ChaKitbagInfo
        | ActionGuildBank of ChaKitbagInfo
        | ActionKitbagTmp of ChaKitbagInfo
        | ActionShortcut of ChaShortcutInfo
        | ActionUnknown of actionType: int64

    /// Сообщение CMD_MC_NOTIACTION — уведомление о действии персонажа.
    type McCharacterActionMessage =
        { WorldId: int64; PacketId: int64; Action: McCharacterActionData }

    // ═══════════════════════════════════════════════════════════════
    //  Фаза 3: Сложные MC команды
    // ═══════════════════════════════════════════════════════════════

    // ─── MC — с вложенными sub-packet ──────────────────────────

    type McSynAttributeMessage = { WorldId: int64; Attr: ChaAttrInfo }

    type McSynSkillStateMessage = { WorldId: int64; SkillState: ChaSkillStateInfo }

    type McAStateEndSeeMessage = { AreaX: int64; AreaY: int64 }

    [<Struct>]
    type AStateBeginSeeEntry = { StateId: int64; StateLv: int64; WorldId: int64; FightId: int64 }

    type McAStateBeginSeeMessage = { AreaX: int64; AreaY: int64; States: AStateBeginSeeEntry[] }

    [<Struct>]
    type McDelItemChaMessage = { MainChaId: int64; WorldId: int64 }

    type McSynEventInfoMessage = { EntityId: int64; EntityType: int64; EventId: int64; EventName: string }

    type McSynSideInfoMessage = { WorldId: int64; Side: ChaSideInfo }

    /// CMD_MC_ITEMBEGINSEE — предмет появился на земле (9 полей + ChaEventInfo).
    type McItemCreateMessage =
        { WorldId: int64; Handle: int64; ItemId: int64; PosX: int64; PosY: int64
          Angle: int64; Num: int64; AppeType: int64; FromId: int64
          Event: ChaEventInfo }

    /// CMD_MC_SYNSKILLBAG — синхронизация сумки скиллов.
    type McSynSkillBagMessage = { WorldId: int64; SkillBag: ChaSkillBagInfo }

    // ─── MC — квестовые ────────────────────────────────────────

    type McMissionInfoMessage =
        { NpcId: int64; ListType: int64; Prev: int64; Next: int64; PrevCmd: int64; NextCmd: int64
          Items: string[] }

    /// CMD_MC_MISPAGE — страница квеста (сложный формат с условной сериализацией).
    type McMisPageMessage =
        { Cmd: int64; NpcId: int64; Name: string
          Needs: MisNeedEntry[]; PrizeSelType: int64; Prizes: MisPrizeEntry[]
          Description: string }

    [<Struct>]
    type MisLogEntry = { MisId: int64; State: int64 }

    type McMisLogMessage = { Logs: MisLogEntry[] }

    /// CMD_MC_MISLOGINFO — детали записи журнала квестов (формат аналогичен MisPage).
    type McMisLogInfoMessage =
        { MisId: int64; Name: string
          Needs: MisNeedEntry[]; PrizeSelType: int64; Prizes: MisPrizeEntry[]
          Description: string }

    [<Struct>]
    type McMisLogClearMessage = { MissionId: int64 }

    [<Struct>]
    type McMisLogAddMessage = { MissionId: int64; State: int64 }

    [<Struct>]
    type McMisLogStateMessage = { MissionId: int64; State: int64 }

    // ─── MC — NPC диалоги ──────────────────────────────────────

    type FuncInfoFuncItem = { Name: string }
    type FuncInfoMissionItem = { Name: string; State: int64 }

    type McFuncInfoMessage =
        { NpcId: int64; Page: int64; TalkText: string
          FuncItems: FuncInfoFuncItem[]; MissionItems: FuncInfoMissionItem[] }

    // ─── MC — volunteer/ranking с массивами ─────────────────────

    type VolunteerEntry = { Name: string; Level: int64; Job: int64; Map: string }

    type McVolunteerListMessage = { PageTotal: int64; Page: int64; Volunteers: VolunteerEntry[] }

    type McVolunteerOpenMessage = { State: int64; PageTotal: int64; Volunteers: VolunteerEntry[] }

    // ─── MC — stall search / ranking (бывший ReverseRead) ──────

    type StallSearchEntry = { Name: string; StallName: string; Location: string; Count: int64; Cost: int64 }

    type McShowStallSearchMessage = { Entries: StallSearchEntry[] }

    type RankingEntry = { Name: string; Guild: string; Level: int64; Job: int64; Score: int64 }

    type McShowRankingMessage = { Entries: RankingEntry[] }

    // ─── MC — EspeItem (специальные предметы инвентаря) ────────

    [<Struct>]
    type EspeItemEntry = { Position: int64; Endure: int64; Energy: int64; Tradable: int64; Expiration: int64 }

    type McEspeItemMessage = { WorldId: int64; Items: EspeItemEntry[] }

    // ─── MC — BlackMarket/Exchange ─────────────────────────────

    [<Struct>]
    type BlackMarketExchangeEntry = { SrcId: int64; SrcCount: int64; TarId: int64; TarCount: int64; TimeValue: int64 }

    type McBlackMarketExchangeDataMessage = { NpcId: int64; Exchanges: BlackMarketExchangeEntry[] }

    [<Struct>]
    type ExchangeEntry = { SrcId: int64; SrcCount: int64; TarId: int64; TarCount: int64 }

    type McExchangeDataMessage = { NpcId: int64; Exchanges: ExchangeEntry[] }

    type McBlackMarketExchangeUpdateMessage = { NpcId: int64; Exchanges: BlackMarketExchangeEntry[] }

    type McBlackMarketExchangeAsrMessage = { Success: int64; SrcId: int64; SrcCount: int64; TarId: int64; TarCount: int64 }

    // ─── MC — торговля NPC (полные данные) ────────────────────────

    /// Предмет на торговой странице (товары NPC).
    type TradePageItem = { ItemId: int64; Count: int64; Price: int64; Level: int64 }

    /// Страница торговли NPC — тип товаров + список предметов.
    type TradePage = { ItemType: int64; Items: TradePageItem[] }

    /// CMD_MC_TRADE_ALLDATA — полные данные торговли NPC.
    type McTradeAllDataMessage =
        { NpcId: int64; TradeType: int64; Param: int64; Pages: TradePage[] }

    // ─── MC — магазин (Store) ────────────────────────────────────

    /// Запись объявления магазина.
    type StoreAfficheEntry = { AfficheId: int64; Title: string; ComId: string }

    /// Запись категории магазина.
    type StoreClassEntry = { ClassId: int64; ClassName: string; ParentId: int64 }

    /// CMD_MC_STORE_OPEN_ASR — ответ на открытие магазина.
    type McStoreOpenAnswerMessage =
        { IsValid: bool; Vip: int64; MoBean: int64; ReplMoney: int64
          Affiches: StoreAfficheEntry[]; Classes: StoreClassEntry[] }

    /// Вариант товара (предмет внутри товара магазина), включая 5 атрибутов.
    type StoreVariantEntry = { ItemId: int64; ItemNum: int64; Flute: int64; Attrs: AttrEntry[] }

    /// Товар магазина.
    type StoreProductEntry =
        { ComId: int64; ComName: string; Price: int64; Remark: string
          IsHot: bool; Time: int64; Quantity: int64; Expire: int64
          Variants: StoreVariantEntry[] }

    /// CMD_MC_STORE_LIST_ASR — список товаров магазина.
    type McStoreListAnswerMessage =
        { PageTotal: int64; PageCurrent: int64; Products: StoreProductEntry[] }

    /// Запись истории покупок магазина.
    type StoreHistoryEntry = { Time: string; ItemId: int64; Name: string; Money: int64 }

    /// CMD_MC_STORE_QUERY — история покупок магазина.
    type McStoreHistoryMessage = { Records: StoreHistoryEntry[] }

    /// CMD_MC_STORE_VIP — VIP-данные магазина.
    type McStoreVipMessage = { Success: int64; VipId: int64; Months: int64; Shuijing: int64; Modou: int64 }

    // ─── MC — синхронизация команды (пати) ───────────────────────

    /// CMD_MC_TEAM — синхронизация данных члена пати.
    type McSynTeamMessage =
        { MemberId: int64; HP: int64; MaxHP: int64; SP: int64; MaxSP: int64; Level: int64
          Look: ChaLookInfo }

    // ─── CMD_MC_NOTIACTION REQUESTGUILDLOGS/UPDATEGUILDLOGS ──────

    /// Константы ActionType для логов банка гильдии.
    [<Literal>]
    let ACT_REQUESTGUILDLOGS = 32L
    [<Literal>]
    let ACT_UPDATEGUILDLOGS  = 33L

    /// Константа типа предмета «корабль» (enumItemTypeBoat).
    [<Literal>]
    let ITEM_TYPE_BOAT = 26L

    /// Запись лога банка гильдии.
    type GuildBankLogEntry =
        { Type: int64; Time: int64; Parameter: int64; Quantity: int64; UserId: int64 }

    /// CMD_MC_NOTIACTION + enumACTION_UPDATEGUILDLOGS: обновление логов банка гильдии.
    type McUpdateGuildLogsMessage =
        { WorldId: int64; PacketId: int64; TotalSize: int64
          Logs: GuildBankLogEntry[]; Terminated: bool }

    /// CMD_MC_NOTIACTION + enumACTION_REQUESTGUILDLOGS: запрос логов банка гильдии.
    type McRequestGuildLogsMessage =
        { WorldId: int64; PacketId: int64
          Logs: GuildBankLogEntry[]; Terminated: bool }

    // ─── CMD_MC_CHARTRADE + CMD_MC_CHARTRADE_ITEM ────────────────

    /// Данные корабля в торговле.
    type TradeBoatData =
        { HasBoat: bool; Name: string
          Ship: int64; Lv: int64; Cexp: int64
          HP: int64; MxHP: int64; SP: int64; MxSP: int64
          MnAtk: int64; MxAtk: int64; Def: int64
          MSpd: int64; ASpd: int64
          UseGridNum: int64; Capacity: int64; Price: int64 }

    /// Данные обычного предмета в торговле.
    type TradeItemData =
        { Endure0: int64; Endure1: int64; Energy0: int64; Energy1: int64
          ForgeLv: int64; Valid: int64; Tradable: int64; Expiration: int64
          ForgeParam: int64; InstId: int64
          HasInstAttr: bool; InstAttr: (int64 * int64)[] }

    /// Данные удаления предмета из торговли (TRADE_DRAGTO_ITEM).
    type McCharTradeItemRemoveData =
        { BagIndex: int64; TradeIndex: int64; Count: int64 }

    /// Данные добавления предмета в торговлю (TRADE_DRAGTO_TRADE).
    type McCharTradeItemAddData =
        { ItemId: int64; BagIndex: int64; TradeIndex: int64; Count: int64
          ItemType: int64; EquipData: Choice<TradeBoatData, TradeItemData> }

    /// Вариант данных торговли предметом.
    type McCharTradeItemData =
        | Remove of McCharTradeItemRemoveData
        | Add of McCharTradeItemAddData

    /// CMD_MC_CHARTRADE + CMD_MC_CHARTRADE_ITEM: единое сообщение торговли предметом.
    type McCharTradeItemMessage =
        { MainChaId: int64; OpType: int64; Data: McCharTradeItemData }

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

        let pcTeamCancelMessage (msg: PcTeamCancelMessage) =
            let mutable w = WPacket(32)
            w.WriteCmd(Commands.CMD_PC_TEAM_CANCEL)
            w.WriteInt64(msg.Reason)
            w.WriteInt64(msg.ChaId)
            w

        let pcFrndInviteMessage (msg: PcFrndInviteMessage) =
            let mutable w = WPacket(128)
            w.WriteCmd(Commands.CMD_PC_FRND_INVITE)
            w.WriteString(msg.InviterName)
            w.WriteInt64(msg.ChaId)
            w.WriteInt64(msg.Icon)
            w

        let pcFrndCancelMessage (msg: PcFrndCancelMessage) =
            let mutable w = WPacket(32)
            w.WriteCmd(Commands.CMD_PC_FRND_CANCEL)
            w.WriteInt64(msg.Reason)
            w.WriteInt64(msg.ChaId)
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

        /// Сериализация тегированного сообщения PC_GM_INFO.
        let pcGmInfoMessage (msg: PcGmInfoMessage) =
            let mutable w = WPacket(1024)
            w.WriteCmd(Commands.CMD_PC_GM_INFO)
            w.WriteInt64(msg.Type)
            match msg.Type with
            | 1L -> // MSG_FRND_REFRESH_START
                w.WriteInt64(int64 msg.Entries.Length)
                for e in msg.Entries do
                    w.WriteInt64(e.ChaId)
                    w.WriteString(e.ChaName)
                    w.WriteString(e.Motto)
                    w.WriteInt64(e.Icon)
                    w.WriteInt64(e.Status)
            | 4L | 5L | 3L -> // ONLINE / OFFLINE / DEL
                w.WriteInt64(msg.ChaId)
            | 2L -> // MSG_FRND_REFRESH_ADD
                w.WriteString(msg.AddEntry.Group)
                w.WriteInt64(msg.AddEntry.ChaId)
                w.WriteString(msg.AddEntry.ChaName)
                w.WriteString(msg.AddEntry.Motto)
                w.WriteInt64(msg.AddEntry.Icon)
            | _ -> ()
            w

        /// Сериализация тегированного сообщения PC_FRND_REFRESH.
        let pcFrndRefreshFullMessage (msg: PcFrndRefreshFullMessage) =
            let mutable w = WPacket(4096)
            w.WriteCmd(Commands.CMD_PC_FRND_REFRESH)
            w.WriteInt64(msg.Type)
            match msg.Type with
            | 4L | 5L | 3L -> // ONLINE / OFFLINE / DEL
                w.WriteInt64(msg.ChaId)
            | 2L -> // MSG_FRND_REFRESH_ADD
                w.WriteString(msg.AddEntry.Group)
                w.WriteInt64(msg.AddEntry.ChaId)
                w.WriteString(msg.AddEntry.ChaName)
                w.WriteString(msg.AddEntry.Motto)
                w.WriteInt64(msg.AddEntry.Icon)
            | 1L -> // MSG_FRND_REFRESH_START
                w.WriteInt64(msg.Self.ChaId)
                w.WriteString(msg.Self.ChaName)
                w.WriteString(msg.Self.Motto)
                w.WriteInt64(msg.Self.Icon)
                w.WriteInt64(int64 msg.Groups.Length)
                for g in msg.Groups do
                    w.WriteString(g.GroupName)
                    w.WriteInt64(int64 g.Members.Length)
                    for m in g.Members do
                        w.WriteInt64(m.ChaId)
                        w.WriteString(m.ChaName)
                        w.WriteString(m.Motto)
                        w.WriteInt64(m.Icon)
                        w.WriteInt64(m.Status)
            | _ -> ()
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

        /// Вспомогательная: сериализация одного участника гильдии.
        let inline private serializeGuildMemberEntry (w: byref<WPacket>) (e: GuildMemberEntry) =
            w.WriteInt64(e.Online); w.WriteInt64(e.ChaId)
            w.WriteString(e.ChaName); w.WriteString(e.Motto); w.WriteString(e.Job)
            w.WriteInt64(e.Degree); w.WriteInt64(e.Icon); w.WriteInt64(e.Permission)

        /// CMD_PC_GUILD — составное сообщение (подкоманда в Msg).
        let pcGuildMessage (msg: PcGuildMessage) =
            let mutable w = WPacket(2048)
            w.WriteCmd(Commands.CMD_PC_GUILD)
            w.WriteInt64(msg.Msg)
            // MSG_GUILD_START=1, ADD=2, DEL=3, ONLINE=4, OFFLINE=5, STOP=6
            match msg.Msg with
            | 4L | 5L | 3L -> // ONLINE / OFFLINE / DEL
                w.WriteInt64(msg.ChaId)
            | 1L -> // START
                w.WriteInt64(int64 msg.Members.Length)
                w.WriteInt64(msg.PacketIndex)
                if msg.PacketIndex = 0L && msg.Members.Length > 0 then
                    w.WriteInt64(msg.GuildId)
                    w.WriteString(msg.GuildName)
                    w.WriteInt64(msg.LeaderId)
                for m in msg.Members do serializeGuildMemberEntry &w m
            | 6L -> () // STOP — нет данных
            | 2L -> // ADD
                serializeGuildMemberEntry &w msg.AddMember
            | _ -> ()
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
            if msg.SessId = 0L then
                w.WriteString(msg.ErrorMsg)
            else
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

        // ─── Группа H: TM ────────────────────────────────────

        let tmEnterMapMessage (msg: TmEnterMapMessage) =
            let mutable w = WPacket(256)
            w.WriteCmd(Commands.CMD_TM_ENTERMAP)
            w.WriteInt64(int64 msg.ActId)
            w.WriteString(msg.Password)
            w.WriteInt64(int64 msg.DatabaseId)
            w.WriteInt64(int64 msg.WorldId)
            w.WriteString(msg.MapName)
            w.WriteInt64(int64 msg.MapCopyNo)
            w.WriteInt64(int64 msg.X)
            w.WriteInt64(int64 msg.Y)
            w.WriteInt64(if msg.IsSwitch then 1L else 0L)
            w.WriteInt64(int64 msg.GateAddr)
            w.WriteInt64(int64 msg.GarnerWinner)
            w

        // ─── Группа: Геймплейные CM (cmd-only) ────────────────────
        let cmOfflineModeCmd () =
            let mutable w = WPacket(8)
            w.WriteCmd(Commands.CMD_CM_OFFLINE_MODE)
            w

        let cmCancelExitCmd () =
            let mutable w = WPacket(8)
            w.WriteCmd(Commands.CMD_CM_CANCELEXIT)
            w

        let cmKitbagLockCmd () =
            let mutable w = WPacket(8)
            w.WriteCmd(Commands.CMD_CM_KITBAG_LOCK)
            w

        let cmKitbagCheckCmd () =
            let mutable w = WPacket(8)
            w.WriteCmd(Commands.CMD_CM_KITBAG_CHECK)
            w

        let cmKitbagTempSyncCmd () =
            let mutable w = WPacket(8)
            w.WriteCmd(Commands.CMD_CM_KITBAGTEMP_SYNC)
            w

        let cmReadBookStartCmd () =
            let mutable w = WPacket(8)
            w.WriteCmd(Commands.CMD_CM_READBOOK_START)
            w

        let cmReadBookCloseCmd () =
            let mutable w = WPacket(8)
            w.WriteCmd(Commands.CMD_CM_READBOOK_CLOSE)
            w

        let cmBoatCancelCmd () =
            let mutable w = WPacket(8)
            w.WriteCmd(Commands.CMD_CM_BOAT_CANCEL)
            w

        let cmBoatGetInfoCmd () =
            let mutable w = WPacket(8)
            w.WriteCmd(Commands.CMD_CM_BOAT_GETINFO)
            w

        let cmVolunteerAddCmd () =
            let mutable w = WPacket(8)
            w.WriteCmd(Commands.CMD_CM_VOLUNTER_ADD)
            w

        let cmVolunteerDelCmd () =
            let mutable w = WPacket(8)
            w.WriteCmd(Commands.CMD_CM_VOLUNTER_DEL)
            w

        let cmStoreCloseCmd () =
            let mutable w = WPacket(8)
            w.WriteCmd(Commands.CMD_CM_STORE_CLOSE)
            w

        let cmStallCloseCmd () =
            let mutable w = WPacket(8)
            w.WriteCmd(Commands.CMD_CM_STALL_CLOSE)
            w

        let cmMisLogCmd () =
            let mutable w = WPacket(8)
            w.WriteCmd(Commands.CMD_CM_MISLOG)
            w

        let cmDailyBuffRequestCmd () =
            let mutable w = WPacket(8)
            w.WriteCmd(Commands.CMD_CM_DailyBuffRequest)
            w

        let cmRequestDropRateCmd () =
            let mutable w = WPacket(8)
            w.WriteCmd(Commands.CMD_CM_REQUEST_DROP_RATE)
            w

        let cmRequestExpRateCmd () =
            let mutable w = WPacket(8)
            w.WriteCmd(Commands.CMD_CM_REQUEST_EXP_RATE)
            w

        // ─── Группа: Геймплейные CM (с полями) ────────────────────
        let cmDieReturnMessage (msg: CmDieReturnMessage) =
            let mutable w = WPacket(16)
            w.WriteCmd(Commands.CMD_CM_DIE_RETURN)
            w.WriteInt64(msg.ReliveType)
            w

        let cmAutoKitbagLockMessage (msg: CmAutoKitbagLockMessage) =
            let mutable w = WPacket(16)
            w.WriteCmd(Commands.CMD_CM_KITBAG_AUTOLOCK)
            w.WriteInt64(msg.AutoLock)
            w

        let cmStallSearchMessage (msg: CmStallSearchMessage) =
            let mutable w = WPacket(16)
            w.WriteCmd(Commands.CMD_CM_STALLSEARCH)
            w.WriteInt64(msg.ItemId)
            w

        let cmForgeItemMessage (msg: CmForgeItemMessage) =
            let mutable w = WPacket(16)
            w.WriteCmd(Commands.CMD_CM_FORGE)
            w.WriteInt64(msg.Index)
            w

        let cmEntityEventMessage (msg: CmEntityEventMessage) =
            let mutable w = WPacket(16)
            w.WriteCmd(Commands.CMD_CM_ENTITY_EVENT)
            w.WriteInt64(msg.EntityId)
            w

        let cmStallOpenMessage (msg: CmStallOpenMessage) =
            let mutable w = WPacket(16)
            w.WriteCmd(Commands.CMD_CM_STALL_OPEN)
            w.WriteInt64(msg.CharId)
            w

        let cmMisLogInfoMessage (msg: CmMisLogInfoMessage) =
            let mutable w = WPacket(16)
            w.WriteCmd(Commands.CMD_CM_MISLOGINFO)
            w.WriteInt64(msg.Id)
            w

        let cmMisLogClearMessage (msg: CmMisLogClearMessage) =
            let mutable w = WPacket(16)
            w.WriteCmd(Commands.CMD_CM_MISLOG_CLEAR)
            w.WriteInt64(msg.Id)
            w

        let cmStoreBuyAskMessage (msg: CmStoreBuyAskMessage) =
            let mutable w = WPacket(16)
            w.WriteCmd(Commands.CMD_CM_STORE_BUY_ASK)
            w.WriteInt64(msg.ComId)
            w

        let cmStoreChangeAskMessage (msg: CmStoreChangeAskMessage) =
            let mutable w = WPacket(16)
            w.WriteCmd(Commands.CMD_CM_STORE_CHANGE_ASK)
            w.WriteInt64(msg.Num)
            w

        let cmStoreQueryMessage (msg: CmStoreQueryMessage) =
            let mutable w = WPacket(16)
            w.WriteCmd(Commands.CMD_CM_STORE_QUERY)
            w.WriteInt64(msg.Num)
            w

        let cmTeamFightAnswerMessage (msg: CmTeamFightAnswerMessage) =
            let mutable w = WPacket(16)
            w.WriteCmd(Commands.CMD_CM_TEAM_FIGHT_ASR)
            w.WriteInt64(msg.Accept)
            w

        let cmItemRepairAnswerMessage (msg: CmItemRepairAnswerMessage) =
            let mutable w = WPacket(16)
            w.WriteCmd(Commands.CMD_CM_ITEM_REPAIR_ASR)
            w.WriteInt64(msg.Accept)
            w

        let cmItemForgeAnswerMessage (msg: CmItemForgeAnswerMessage) =
            let mutable w = WPacket(16)
            w.WriteCmd(Commands.CMD_CM_ITEM_FORGE_ASR)
            w.WriteInt64(msg.Accept)
            w

        let cmItemLotteryAnswerMessage (msg: CmItemLotteryAnswerMessage) =
            let mutable w = WPacket(16)
            w.WriteCmd(Commands.CMD_CM_ITEM_LOTTERY_ASR)
            w.WriteInt64(msg.Accept)
            w

        let cmVolunteerOpenMessage (msg: CmVolunteerOpenMessage) =
            let mutable w = WPacket(16)
            w.WriteCmd(Commands.CMD_CM_VOLUNTER_OPEN)
            w.WriteInt64(msg.Num)
            w

        let cmVolunteerListMessage (msg: CmVolunteerListMessage) =
            let mutable w = WPacket(24)
            w.WriteCmd(Commands.CMD_CM_VOLUNTER_LIST)
            w.WriteInt64(msg.Page)
            w.WriteInt64(msg.Num)
            w

        let cmStoreListAskMessage (msg: CmStoreListAskMessage) =
            let mutable w = WPacket(32)
            w.WriteCmd(Commands.CMD_CM_STORE_LIST_ASK)
            w.WriteInt64(msg.ClsId)
            w.WriteInt64(msg.Page)
            w.WriteInt64(msg.Num)
            w

        let cmCaptainConfirmAsrMessage (msg: CmCaptainConfirmAsrMessage) =
            let mutable w = WPacket(24)
            w.WriteCmd(Commands.CMD_CM_CAPTAIN_CONFIRM_ASR)
            w.WriteInt64(msg.Ret)
            w.WriteInt64(msg.TeamId)
            w

        let cmMapMaskMessage (msg: CmMapMaskMessage) =
            let mutable w = WPacket(64)
            w.WriteCmd(Commands.CMD_CM_MAP_MASK)
            w.WriteString(msg.MapName)
            w

        let cmStoreOpenAskMessage (msg: CmStoreOpenAskMessage) =
            let mutable w = WPacket(64)
            w.WriteCmd(Commands.CMD_CM_STORE_OPEN_ASK)
            w.WriteString(msg.Password)
            w

        let cmVolunteerSelMessage (msg: CmVolunteerSelMessage) =
            let mutable w = WPacket(64)
            w.WriteCmd(Commands.CMD_CM_VOLUNTER_SEL)
            w.WriteString(msg.Name)
            w

        let cmKitbagUnlockMessage (msg: CmKitbagUnlockMessage) =
            let mutable w = WPacket(64)
            w.WriteCmd(Commands.CMD_CM_KITBAG_UNLOCK)
            w.WriteString(msg.Password)
            w

        // ─── Группа: Геймплейные MC (простые) ─────────────────────

        let mcFailedActionMessage (msg: McFailedActionMessage) =
            let mutable w = WPacket(32)
            w.WriteCmd(Commands.CMD_MC_FAILEDACTION)
            w.WriteInt64(msg.WorldId)
            w.WriteInt64(msg.ActionType)
            w.WriteInt64(msg.Reason)
            w

        let mcChaEndSeeMessage (msg: McChaEndSeeMessage) =
            let mutable w = WPacket(24)
            w.WriteCmd(Commands.CMD_MC_CHAENDSEE)
            w.WriteInt64(msg.SeeType)
            w.WriteInt64(msg.WorldId)
            w

        let mcItemDestroyMessage (msg: McItemDestroyMessage) =
            let mutable w = WPacket(16)
            w.WriteCmd(Commands.CMD_MC_ITEMENDSEE)
            w.WriteInt64(msg.WorldId)
            w

        let mcForgeResultMessage (msg: McForgeResultMessage) =
            let mutable w = WPacket(16)
            w.WriteCmd(Commands.CMD_MC_BEGIN_ITEM_FORGE)
            w.WriteInt64(msg.Result)
            w

        let mcUniteResultMessage (msg: McUniteResultMessage) =
            let mutable w = WPacket(16)
            w.WriteCmd(Commands.CMD_MC_BEGIN_ITEM_UNITE)
            w.WriteInt64(msg.Result)
            w

        let mcMillingResultMessage (msg: McMillingResultMessage) =
            let mutable w = WPacket(16)
            w.WriteCmd(Commands.CMD_MC_BEGIN_ITEM_MILLING)
            w.WriteInt64(msg.Result)
            w

        let mcFusionResultMessage (msg: McFusionResultMessage) =
            let mutable w = WPacket(16)
            w.WriteCmd(Commands.CMD_MC_BEGIN_ITEM_FUSION)
            w.WriteInt64(msg.Result)
            w

        let mcUpgradeResultMessage (msg: McUpgradeResultMessage) =
            let mutable w = WPacket(16)
            w.WriteCmd(Commands.CMD_MC_BEGIN_ITEM_UPGRADE)
            w.WriteInt64(msg.Result)
            w

        let mcPurifyResultMessage (msg: McPurifyResultMessage) =
            let mutable w = WPacket(16)
            w.WriteCmd(Commands.CMD_MC_BEGIN_ITEM_PURIFY)
            w.WriteInt64(msg.Result)
            w

        let mcFixResultMessage (msg: McFixResultMessage) =
            let mutable w = WPacket(16)
            w.WriteCmd(Commands.CMD_MC_BEGIN_ITEM_FIX)
            w.WriteInt64(msg.Result)
            w

        let mcEidolonMetempsychosisMessage (msg: McEidolonMetempsychosisMessage) =
            let mutable w = WPacket(16)
            w.WriteCmd(Commands.CMD_MC_BEGIN_ITEM_EIDOLON_METEMPSYCHOSIS)
            w.WriteInt64(msg.Result)
            w

        let mcEidolonFusionMessage (msg: McEidolonFusionMessage) =
            let mutable w = WPacket(16)
            w.WriteCmd(Commands.CMD_MC_BEGIN_ITEM_EIDOLON_FUSION)
            w.WriteInt64(msg.Result)
            w

        let mcMessageMessage (msg: McMessageMessage) =
            let mutable w = WPacket(64)
            w.WriteCmd(Commands.CMD_MC_MESSAGE)
            w.WriteString(msg.Text)
            w

        let mcBickerNoticeMessage (msg: McBickerNoticeMessage) =
            let mutable w = WPacket(64)
            w.WriteCmd(Commands.CMD_MC_BICKER_NOTICE)
            w.WriteString(msg.Text)
            w

        let mcColourNoticeMessage (msg: McColourNoticeMessage) =
            let mutable w = WPacket(64)
            w.WriteCmd(Commands.CMD_MC_COLOUR_NOTICE)
            w.WriteInt64(msg.Color)
            w.WriteString(msg.Text)
            w

        let mcTriggerActionMessage (msg: McTriggerActionMessage) =
            let mutable w = WPacket(40)
            w.WriteCmd(Commands.CMD_MC_TRIGGER_ACTION)
            w.WriteInt64(msg.Type)
            w.WriteInt64(msg.Id)
            w.WriteInt64(msg.Num)
            w.WriteInt64(msg.Count)
            w

        let mcNpcStateChangeMessage (msg: McNpcStateChangeMessage) =
            let mutable w = WPacket(24)
            w.WriteCmd(Commands.CMD_MC_NPCSTATECHG)
            w.WriteInt64(msg.NpcId)
            w.WriteInt64(msg.State)
            w

        let mcEntityStateChangeMessage (msg: McEntityStateChangeMessage) =
            let mutable w = WPacket(24)
            w.WriteCmd(Commands.CMD_MC_ENTITY_CHGSTATE)
            w.WriteInt64(msg.EntityId)
            w.WriteInt64(msg.State)
            w

        let mcCloseTalkMessage (msg: McCloseTalkMessage) =
            let mutable w = WPacket(16)
            w.WriteCmd(Commands.CMD_MC_CLOSETALK)
            w.WriteInt64(msg.NpcId)
            w

        let mcKitbagCheckAnswerMessage (msg: McKitbagCheckAnswerMessage) =
            let mutable w = WPacket(16)
            w.WriteCmd(Commands.CMD_MC_KITBAG_CHECK_ASR)
            w.WriteInt64(msg.Locked)
            w

        let mcPreMoveTimeMessage (msg: McPreMoveTimeMessage) =
            let mutable w = WPacket(16)
            w.WriteCmd(Commands.CMD_MC_PREMOVE_TIME)
            w.WriteInt64(msg.Time)
            w

        let mcItemUseSuccMessage (msg: McItemUseSuccMessage) =
            let mutable w = WPacket(24)
            w.WriteCmd(Commands.CMD_MC_ITEM_USE_SUC)
            w.WriteInt64(msg.WorldId)
            w.WriteInt64(msg.ItemId)
            w

        let mcChaPlayEffectMessage (msg: McChaPlayEffectMessage) =
            let mutable w = WPacket(24)
            w.WriteCmd(Commands.CMD_MC_CHAPLAYEFFECT)
            w.WriteInt64(msg.WorldId)
            w.WriteInt64(msg.EffectId)
            w

        let mcSynDefaultSkillMessage (msg: McSynDefaultSkillMessage) =
            let mutable w = WPacket(24)
            w.WriteCmd(Commands.CMD_MC_SYNDEFAULTSKILL)
            w.WriteInt64(msg.WorldId)
            w.WriteInt64(msg.SkillId)
            w

        // ─── MC — ReadSequence→ReadString миграция ─────────────────

        let mcSayMessage (msg: McSayMessage) =
            let mutable w = WPacket(128)
            w.WriteCmd(Commands.CMD_MC_SAY)
            w.WriteInt64(msg.SourceId)
            w.WriteString(msg.Content)
            w.WriteInt64(msg.Color)
            w

        let mcSysInfoMessage (msg: McSysInfoMessage) =
            let mutable w = WPacket(128)
            w.WriteCmd(Commands.CMD_MC_SYSINFO)
            w.WriteString(msg.Info)
            w

        let mcPopupNoticeMessage (msg: McPopupNoticeMessage) =
            let mutable w = WPacket(128)
            w.WriteCmd(Commands.CMD_MC_ALARM)
            w.WriteString(msg.Notice)
            w

        let mcPingMessage (msg: McPingMessage) =
            let mutable w = WPacket(48)
            w.WriteCmd(Commands.CMD_MC_PING)
            w.WriteInt64(msg.V1)
            w.WriteInt64(msg.V2)
            w.WriteInt64(msg.V3)
            w.WriteInt64(msg.V4)
            w.WriteInt64(msg.V5)
            w

        // ─── Фаза 2: CM — средние ─────────────────────────────────

        let cmChangePassMessage (msg: CmChangePassMessage) =
            let mutable w = WPacket(128)
            w.WriteCmd(Commands.CMD_CP_CHANGEPASS)
            w.WriteString(msg.Pass)
            w.WriteString(msg.Pin)
            w

        let cmGuildBankOperMessage (msg: CmGuildBankOperMessage) =
            let mutable w = WPacket(56)
            w.WriteCmd(Commands.CMD_CP_GUILDBANK)
            w.WriteInt64(msg.Op)
            w.WriteInt64(msg.SrcType)
            w.WriteInt64(msg.SrcId)
            w.WriteInt64(msg.SrcNum)
            w.WriteInt64(msg.TarType)
            w.WriteInt64(msg.TarId)
            w

        let cmGuildBankGoldMessage (msg: CmGuildBankGoldMessage) =
            let mutable w = WPacket(32)
            w.WriteCmd(Commands.CMD_CP_GUILDBANK)
            w.WriteInt64(msg.Op)
            w.WriteInt64(msg.Direction)
            w.WriteInt64(msg.Gold)
            w

        let cmUpdateHairMessage (msg: CmUpdateHairMessage) =
            let mutable w = WPacket(48)
            w.WriteCmd(Commands.CMD_CM_UPDATEHAIR)
            w.WriteInt64(msg.ScriptId)
            w.WriteInt64(msg.GridLoc0)
            w.WriteInt64(msg.GridLoc1)
            w.WriteInt64(msg.GridLoc2)
            w.WriteInt64(msg.GridLoc3)
            w

        let cmTeamFightAskMessage (msg: CmTeamFightAskMessage) =
            let mutable w = WPacket(32)
            w.WriteCmd(Commands.CMD_CM_TEAM_FIGHT_ASK)
            w.WriteInt64(msg.Type)
            w.WriteInt64(msg.WorldId)
            w.WriteInt64(msg.Handle)
            w

        let cmItemRepairAskMessage (msg: CmItemRepairAskMessage) =
            let mutable w = WPacket(40)
            w.WriteCmd(Commands.CMD_CM_ITEM_REPAIR_ASK)
            w.WriteInt64(msg.RepairmanId)
            w.WriteInt64(msg.RepairmanHandle)
            w.WriteInt64(msg.PosType)
            w.WriteInt64(msg.PosId)
            w

        let cmRequestTradeMessage (msg: CmRequestTradeMessage) =
            let mutable w = WPacket(24)
            w.WriteCmd(Commands.CMD_CM_CHARTRADE_REQUEST)
            w.WriteInt64(msg.Type)
            w.WriteInt64(msg.CharId)
            w

        let cmAcceptTradeMessage (msg: CmAcceptTradeMessage) =
            let mutable w = WPacket(24)
            w.WriteCmd(Commands.CMD_CM_CHARTRADE_ACCEPT)
            w.WriteInt64(msg.Type)
            w.WriteInt64(msg.CharId)
            w

        let cmCancelTradeMessage (msg: CmCancelTradeMessage) =
            let mutable w = WPacket(24)
            w.WriteCmd(Commands.CMD_CM_CHARTRADE_CANCEL)
            w.WriteInt64(msg.Type)
            w.WriteInt64(msg.CharId)
            w

        let cmValidateTradeDataMessage (msg: CmValidateTradeDataMessage) =
            let mutable w = WPacket(24)
            w.WriteCmd(Commands.CMD_CM_CHARTRADE_VALIDATEDATA)
            w.WriteInt64(msg.Type)
            w.WriteInt64(msg.CharId)
            w

        let cmValidateTradeMessage (msg: CmValidateTradeMessage) =
            let mutable w = WPacket(24)
            w.WriteCmd(Commands.CMD_CM_CHARTRADE_VALIDATE)
            w.WriteInt64(msg.Type)
            w.WriteInt64(msg.CharId)
            w

        let cmAddItemMessage (msg: CmAddItemMessage) =
            let mutable w = WPacket(56)
            w.WriteCmd(Commands.CMD_CM_CHARTRADE_ITEM)
            w.WriteInt64(msg.Type)
            w.WriteInt64(msg.CharId)
            w.WriteInt64(msg.OpType)
            w.WriteInt64(msg.Index)
            w.WriteInt64(msg.ItemIndex)
            w.WriteInt64(msg.Count)
            w

        let cmAddMoneyMessage (msg: CmAddMoneyMessage) =
            let mutable w = WPacket(48)
            w.WriteCmd(Commands.CMD_CM_CHARTRADE_MONEY)
            w.WriteInt64(msg.Type)
            w.WriteInt64(msg.CharId)
            w.WriteInt64(msg.OpType)
            w.WriteInt64(msg.IsImp)
            w.WriteInt64(msg.Money)
            w

        let cmTigerStartMessage (msg: CmTigerStartMessage) =
            let mutable w = WPacket(40)
            w.WriteCmd(Commands.CMD_CM_TIGER_START)
            w.WriteInt64(msg.NpcId)
            w.WriteInt64(msg.Sel1)
            w.WriteInt64(msg.Sel2)
            w.WriteInt64(msg.Sel3)
            w

        let cmTigerStopMessage (msg: CmTigerStopMessage) =
            let mutable w = WPacket(24)
            w.WriteCmd(Commands.CMD_CM_TIGER_STOP)
            w.WriteInt64(msg.NpcId)
            w.WriteInt64(msg.Num)
            w

        let cmVolunteerAsrMessage (msg: CmVolunteerAsrMessage) =
            let mutable w = WPacket(64)
            w.WriteCmd(Commands.CMD_CM_VOLUNTER_ASR)
            w.WriteInt64(msg.Ret)
            w.WriteString(msg.Name)
            w

        let cmCreateBoatMessage (msg: CmCreateBoatMessage) =
            let mutable w = WPacket(96)
            w.WriteCmd(Commands.CMD_CM_CREATE_BOAT)
            w.WriteString(msg.Boat)
            w.WriteInt64(msg.Header)
            w.WriteInt64(msg.Engine)
            w.WriteInt64(msg.Cannon)
            w.WriteInt64(msg.Equipment)
            w

        let cmUpdateBoatMessage (msg: CmUpdateBoatMessage) =
            let mutable w = WPacket(40)
            w.WriteCmd(Commands.CMD_CM_UPDATEBOAT_PART)
            w.WriteInt64(msg.Header)
            w.WriteInt64(msg.Engine)
            w.WriteInt64(msg.Cannon)
            w.WriteInt64(msg.Equipment)
            w

        let cmSelectBoatListMessage (msg: CmSelectBoatListMessage) =
            let mutable w = WPacket(32)
            w.WriteCmd(Commands.CMD_CM_BOAT_SELECT)
            w.WriteInt64(msg.NpcId)
            w.WriteInt64(msg.Type)
            w.WriteInt64(msg.Index)
            w

        let cmBoatLaunchMessage (msg: CmBoatLaunchMessage) =
            let mutable w = WPacket(24)
            w.WriteCmd(Commands.CMD_CM_BOAT_LUANCH)
            w.WriteInt64(msg.NpcId)
            w.WriteInt64(msg.Index)
            w

        let cmBoatBagSelMessage (msg: CmBoatBagSelMessage) =
            let mutable w = WPacket(24)
            w.WriteCmd(Commands.CMD_CM_BOAT_BAGSEL)
            w.WriteInt64(msg.NpcId)
            w.WriteInt64(msg.Index)
            w

        let cmReportWgMessage (msg: CmReportWgMessage) =
            let mutable w = WPacket(128)
            w.WriteCmd(Commands.CMD_CP_REPORT_WG)
            w.WriteString(msg.Info)
            w

        let cmSay2CampMessage (msg: CmSay2CampMessage) =
            let mutable w = WPacket(128)
            w.WriteCmd(Commands.CMD_CM_SAY2CAMP)
            w.WriteString(msg.Content)
            w

        let cmStallBuyMessage (msg: CmStallBuyMessage) =
            let mutable w = WPacket(40)
            w.WriteCmd(Commands.CMD_CM_STALL_BUY)
            w.WriteInt64(msg.CharId)
            w.WriteInt64(msg.Index)
            w.WriteInt64(msg.Count)
            w.WriteInt64(msg.GridId)
            w

        let cmSkillUpgradeMessage (msg: CmSkillUpgradeMessage) =
            let mutable w = WPacket(24)
            w.WriteCmd(Commands.CMD_CM_SKILLUPGRADE)
            w.WriteInt64(msg.SkillId)
            w.WriteInt64(msg.AddGrade)
            w

        let cmRefreshDataMessage (msg: CmRefreshDataMessage) =
            let mutable w = WPacket(24)
            w.WriteCmd(Commands.CMD_CM_REFRESH_DATA)
            w.WriteInt64(msg.WorldId)
            w.WriteInt64(msg.Handle)
            w

        let cmPkCtrlMessage (msg: CmPkCtrlMessage) =
            let mutable w = WPacket(16)
            w.WriteCmd(Commands.CMD_CM_PK_CTRL)
            w.WriteInt64(msg.Ctrl)
            w

        let cmItemAmphitheaterAskMessage (msg: CmItemAmphitheaterAskMessage) =
            let mutable w = WPacket(24)
            w.WriteCmd(Commands.CMD_CM_ITEM_AMPHITHEATER_ASK)
            w.WriteInt64(msg.Sure)
            w.WriteInt64(msg.ReId)
            w

        let cmMasterInviteMessage (msg: CmMasterInviteMessage) =
            let mutable w = WPacket(80)
            w.WriteCmd(Commands.CMD_CM_MASTER_INVITE)
            w.WriteString(msg.Name)
            w.WriteInt64(msg.ChaId)
            w

        let cmMasterAsrMessage (msg: CmMasterAsrMessage) =
            let mutable w = WPacket(88)
            w.WriteCmd(Commands.CMD_CM_MASTER_ASR)
            w.WriteInt64(msg.Agree)
            w.WriteString(msg.Name)
            w.WriteInt64(msg.ChaId)
            w

        let cmMasterDelMessage (msg: CmMasterDelMessage) =
            let mutable w = WPacket(80)
            w.WriteCmd(Commands.CMD_CM_MASTER_DEL)
            w.WriteString(msg.Name)
            w.WriteInt64(msg.ChaId)
            w

        let cmPrenticeInviteMessage (msg: CmPrenticeInviteMessage) =
            let mutable w = WPacket(80)
            w.WriteCmd(Commands.CMD_CM_PRENTICE_INVITE)
            w.WriteString(msg.Name)
            w.WriteInt64(msg.ChaId)
            w

        let cmPrenticeAsrMessage (msg: CmPrenticeAsrMessage) =
            let mutable w = WPacket(88)
            w.WriteCmd(Commands.CMD_CM_PRENTICE_ASR)
            w.WriteInt64(msg.Agree)
            w.WriteString(msg.Name)
            w.WriteInt64(msg.ChaId)
            w

        let cmPrenticeDelMessage (msg: CmPrenticeDelMessage) =
            let mutable w = WPacket(80)
            w.WriteCmd(Commands.CMD_CM_PRENTICE_DEL)
            w.WriteString(msg.Name)
            w.WriteInt64(msg.ChaId)
            w

        // ─── Фаза 2: NPC Talk compound commands ────────────────────
        // Составные команды: WriteCmd пишет внешнюю команду,
        // затем внутри тела пакета встраивается суб-команда как int64.

        /// CMD_CM_REQUESTTALK → [npcId, CMD_CM_TALKPAGE, cmd]
        let cmRequestTalkMessage (msg: CmRequestTalkMessage) =
            let mutable w = WPacket(32)
            w.WriteCmd(Commands.CMD_CM_REQUESTTALK)
            w.WriteInt64(msg.NpcId)
            w.WriteInt64(int64 Commands.CMD_CM_TALKPAGE)
            w.WriteInt64(msg.Cmd)
            w

        /// CMD_CM_REQUESTTALK → [npcId, CMD_CM_FUNCITEM, pageId, index]
        let cmSelFunctionMessage (msg: CmSelFunctionMessage) =
            let mutable w = WPacket(40)
            w.WriteCmd(Commands.CMD_CM_REQUESTTALK)
            w.WriteInt64(msg.NpcId)
            w.WriteInt64(int64 Commands.CMD_CM_FUNCITEM)
            w.WriteInt64(msg.PageId)
            w.WriteInt64(msg.Index)
            w

        /// CMD_CM_REQUESTTRADE → [npcId, CMD_CM_TRADEITEM, ROLE_TRADE_SALE, index, count]
        let cmSaleMessage (msg: CmSaleMessage) =
            let mutable w = WPacket(48)
            w.WriteCmd(Commands.CMD_CM_REQUESTTRADE)
            w.WriteInt64(msg.NpcId)
            w.WriteInt64(int64 Commands.CMD_CM_TRADEITEM)
            w.WriteInt64(ROLE_TRADE_SALE)
            w.WriteInt64(msg.Index)
            w.WriteInt64(msg.Count)
            w

        /// CMD_CM_REQUESTTRADE → [npcId, CMD_CM_TRADEITEM, ROLE_TRADE_BUY, itemType, index1, index2, count]
        let cmBuyMessage (msg: CmBuyMessage) =
            let mutable w = WPacket(64)
            w.WriteCmd(Commands.CMD_CM_REQUESTTRADE)
            w.WriteInt64(msg.NpcId)
            w.WriteInt64(int64 Commands.CMD_CM_TRADEITEM)
            w.WriteInt64(ROLE_TRADE_BUY)
            w.WriteInt64(msg.ItemType)
            w.WriteInt64(msg.Index1)
            w.WriteInt64(msg.Index2)
            w.WriteInt64(msg.Count)
            w

        /// CMD_CM_REQUESTTALK → [npcId, CMD_CM_MISSION, cmd, selItem, param]
        let cmMissionPageMessage (msg: CmMissionPageMessage) =
            let mutable w = WPacket(48)
            w.WriteCmd(Commands.CMD_CM_REQUESTTALK)
            w.WriteInt64(msg.NpcId)
            w.WriteInt64(int64 Commands.CMD_CM_MISSION)
            w.WriteInt64(msg.Cmd)
            w.WriteInt64(msg.SelItem)
            w.WriteInt64(msg.Param)
            w

        /// CMD_CM_REQUESTTALK → [npcId, CMD_CM_MISSION, ROLE_MIS_SEL, index]
        let cmSelMissionMessage (msg: CmSelMissionMessage) =
            let mutable w = WPacket(40)
            w.WriteCmd(Commands.CMD_CM_REQUESTTALK)
            w.WriteInt64(msg.NpcId)
            w.WriteInt64(int64 Commands.CMD_CM_MISSION)
            w.WriteInt64(ROLE_MIS_SEL)
            w.WriteInt64(msg.Index)
            w

        /// CMD_CM_REQUESTTRADE → [npcId, CMD_CM_BLACKMARKET_EXCHANGE_REQ, timeNum, srcId, srcNum, tarId, tarNum, index]
        let cmBlackMarketExchangeReqMessage (msg: CmBlackMarketExchangeReqMessage) =
            let mutable w = WPacket(72)
            w.WriteCmd(Commands.CMD_CM_REQUESTTRADE)
            w.WriteInt64(msg.NpcId)
            w.WriteInt64(int64 Commands.CMD_CM_BLACKMARKET_EXCHANGE_REQ)
            w.WriteInt64(msg.TimeNum)
            w.WriteInt64(msg.SrcId)
            w.WriteInt64(msg.SrcNum)
            w.WriteInt64(msg.TarId)
            w.WriteInt64(msg.TarNum)
            w.WriteInt64(msg.Index)
            w

        // ─── Фаза 2: MC — средние ─────────────────────────────────

        let mcSay2CampMessage (msg: McSay2CampMessage) =
            let mutable w = WPacket(128)
            w.WriteCmd(Commands.CMD_MC_SAY2CAMP)
            w.WriteString(msg.ChaName)
            w.WriteString(msg.Content)
            w

        let mcTalkInfoMessage (msg: McTalkInfoMessage) =
            let mutable w = WPacket(128)
            w.WriteCmd(Commands.CMD_MC_TALKPAGE)
            w.WriteInt64(msg.NpcId)
            w.WriteInt64(msg.Cmd)
            w.WriteString(msg.Text)
            w

        let mcTradeDataMessage (msg: McTradeDataMessage) =
            let mutable w = WPacket(56)
            w.WriteCmd(Commands.CMD_MC_TRADE_DATA)
            w.WriteInt64(msg.NpcId)
            w.WriteInt64(msg.Page)
            w.WriteInt64(msg.Index)
            w.WriteInt64(msg.ItemId)
            w.WriteInt64(msg.Count)
            w.WriteInt64(msg.Price)
            w

        let mcTradeResultMessage (msg: McTradeResultMessage) =
            let mutable w = WPacket(48)
            w.WriteCmd(Commands.CMD_MC_TRADERESULT)
            w.WriteInt64(msg.Type)
            w.WriteInt64(msg.Index)
            w.WriteInt64(msg.Count)
            w.WriteInt64(msg.ItemId)
            w.WriteInt64(msg.Money)
            w

        let mcUpdateHairResMessage (msg: McUpdateHairResMessage) =
            let mutable w = WPacket(64)
            w.WriteCmd(Commands.CMD_MC_UPDATEHAIR_RES)
            w.WriteInt64(msg.WorldId)
            w.WriteInt64(msg.ScriptId)
            w.WriteString(msg.Reason)
            w

        let mcSynTLeaderIdMessage (msg: McSynTLeaderIdMessage) =
            let mutable w = WPacket(24)
            w.WriteCmd(Commands.CMD_MC_TLEADER_ID)
            w.WriteInt64(msg.WorldId)
            w.WriteInt64(msg.LeaderId)
            w

        let mcKitbagCapacityMessage (msg: McKitbagCapacityMessage) =
            let mutable w = WPacket(24)
            w.WriteCmd(Commands.CMD_MC_KITBAG_CAPACITY)
            w.WriteInt64(msg.WorldId)
            w.WriteInt64(msg.Capacity)
            w

        let mcItemForgeAskMessage (msg: McItemForgeAskMessage) =
            let mutable w = WPacket(24)
            w.WriteCmd(Commands.CMD_MC_ITEM_FORGE_ASK)
            w.WriteInt64(msg.Type)
            w.WriteInt64(msg.Money)
            w

        let mcItemForgeAnswerMessage (msg: McItemForgeAnswerMessage) =
            let mutable w = WPacket(32)
            w.WriteCmd(Commands.CMD_MC_ITEM_FORGE_ASR)
            w.WriteInt64(msg.WorldId)
            w.WriteInt64(msg.Type)
            w.WriteInt64(msg.Result)
            w

        let mcQueryChaMessage (msg: McQueryChaMessage) =
            let mutable w = WPacket(128)
            w.WriteCmd(Commands.CMD_MC_QUERY_CHA)
            w.WriteInt64(msg.ChaId)
            w.WriteString(msg.Name)
            w.WriteString(msg.MapName)
            w.WriteInt64(msg.PosX)
            w.WriteInt64(msg.PosY)
            w.WriteInt64(msg.ChaId2)
            w

        let mcQueryChaPingMessage (msg: McQueryChaPingMessage) =
            let mutable w = WPacket(128)
            w.WriteCmd(Commands.CMD_MC_QUERY_CHAPING)
            w.WriteInt64(msg.ChaId)
            w.WriteString(msg.Name)
            w.WriteString(msg.MapName)
            w.WriteInt64(msg.Ping)
            w

        let mcQueryReliveMessage (msg: McQueryReliveMessage) =
            let mutable w = WPacket(64)
            w.WriteCmd(Commands.CMD_MC_QUERY_RELIVE)
            w.WriteInt64(msg.ChaId)
            w.WriteString(msg.SourceName)
            w.WriteInt64(msg.ReliveType)
            w

        let mcGmMailMessage (msg: McGmMailMessage) =
            let mutable w = WPacket(256)
            w.WriteCmd(Commands.CMD_MC_GM_MAIL)
            w.WriteString(msg.Title)
            w.WriteString(msg.Content)
            w.WriteInt64(msg.Time)
            w

        let mcSynStallNameMessage (msg: McSynStallNameMessage) =
            let mutable w = WPacket(64)
            w.WriteCmd(Commands.CMD_MC_STALL_NAME)
            w.WriteInt64(msg.CharId)
            w.WriteString(msg.Name)
            w

        let mcMapCrashMessage (msg: McMapCrashMessage) =
            let mutable w = WPacket(64)
            w.WriteCmd(Commands.CMD_MC_MAPCRASH)
            w.WriteString(msg.Text)
            w

        let mcVolunteerStateMessage (msg: McVolunteerStateMessage) =
            let mutable w = WPacket(16)
            w.WriteCmd(Commands.CMD_MC_VOLUNTER_STATE)
            w.WriteInt64(msg.State)
            w

        let mcVolunteerAskMessage (msg: McVolunteerAskMessage) =
            let mutable w = WPacket(64)
            w.WriteCmd(Commands.CMD_MC_VOLUNTER_ASK)
            w.WriteString(msg.Name)
            w

        let mcMasterAskMessage (msg: McMasterAskMessage) =
            let mutable w = WPacket(64)
            w.WriteCmd(Commands.CMD_MC_MASTER_ASK)
            w.WriteString(msg.Name)
            w.WriteInt64(msg.ChaId)
            w

        let mcPrenticeAskMessage (msg: McPrenticeAskMessage) =
            let mutable w = WPacket(64)
            w.WriteCmd(Commands.CMD_MC_PRENTICE_ASK)
            w.WriteString(msg.Name)
            w.WriteInt64(msg.ChaId)
            w

        let mcItemRepairAskMessage (msg: McItemRepairAskMessage) =
            let mutable w = WPacket(64)
            w.WriteCmd(Commands.CMD_MC_ITEM_REPAIR_ASK)
            w.WriteString(msg.ItemName)
            w.WriteInt64(msg.RepairCost)
            w

        let mcItemLotteryAsrMessage (msg: McItemLotteryAsrMessage) =
            let mutable w = WPacket(16)
            w.WriteCmd(Commands.CMD_MC_ITEM_LOTTERY_ASR)
            w.WriteInt64(msg.Success)
            w

        // ─── MC/CM — Фаза 4: простые ────────────────────────────

        let mcChaEmotionMessage (msg: McChaEmotionMessage) =
            let mutable w = WPacket(24)
            w.WriteCmd(Commands.CMD_MC_CHA_EMOTION)
            w.WriteInt64(msg.WorldId)
            w.WriteInt64(msg.Emotion)
            w

        let mcStartExitMessage (msg: McStartExitMessage) =
            let mutable w = WPacket(16)
            w.WriteCmd(Commands.CMD_MC_STARTEXIT)
            w.WriteInt64(msg.ExitTime)
            w

        let mcCancelExitCmd () =
            let mutable w = WPacket(8)
            w.WriteCmd(Commands.CMD_MC_CANCELEXIT)
            w

        let mcOpenHairCutCmd () =
            let mutable w = WPacket(8)
            w.WriteCmd(Commands.CMD_MC_OPENHAIR)
            w

        let mcBeginItemRepairCmd () =
            let mutable w = WPacket(8)
            w.WriteCmd(Commands.CMD_MC_BEGIN_ITEM_REPAIR)
            w

        let mcGmSendCmd () =
            let mutable w = WPacket(8)
            w.WriteCmd(Commands.CMD_MC_BEGIN_GM_SEND)
            w

        let mcGmRecvMessage (msg: McGmRecvMessage) =
            let mutable w = WPacket(16)
            w.WriteCmd(Commands.CMD_MC_BEGIN_GM_RECV)
            w.WriteInt64(msg.NpcId)
            w

        let mcStallDelGoodsMessage (msg: McStallDelGoodsMessage) =
            let mutable w = WPacket(32)
            w.WriteCmd(Commands.CMD_MC_STALL_DELGOODS)
            w.WriteInt64(msg.CharId)
            w.WriteInt64(msg.Grid)
            w.WriteInt64(msg.Count)
            w

        let mcStallCloseMessage (msg: McStallCloseMessage) =
            let mutable w = WPacket(16)
            w.WriteCmd(Commands.CMD_MC_STALL_CLOSE)
            w.WriteInt64(msg.CharId)
            w

        let mcStallSuccessMessage (msg: McStallSuccessMessage) =
            let mutable w = WPacket(16)
            w.WriteCmd(Commands.CMD_MC_STALL_START)
            w.WriteInt64(msg.CharId)
            w

        let mcUpdateGuildGoldMessage (msg: McUpdateGuildGoldMessage) =
            let mutable w = WPacket(64)
            w.WriteCmd(Commands.CMD_MC_UPDATEGUILDBANKGOLD)
            w.WriteString(msg.Data)
            w

        let mcQueryChaItemMessage (msg: McQueryChaItemMessage) =
            let mutable w = WPacket(16)
            w.WriteCmd(Commands.CMD_MM_QUERY_CHAITEM)
            w.WriteInt64(msg.ChaId)
            w

        let mcDisconnectMessage (msg: McDisconnectMessage) =
            let mutable w = WPacket(16)
            w.WriteCmd(Commands.CMD_TC_DISCONNECT)
            w.WriteInt64(msg.Reason)
            w

        let mcLifeSkillShowMessage (msg: McLifeSkillShowMessage) =
            let mutable w = WPacket(16)
            w.WriteCmd(Commands.CMD_MC_LIFESKILL_BGING)
            w.WriteInt64(msg.Type)
            w

        let mcLifeSkillMessage (msg: McLifeSkillMessage) =
            let mutable w = WPacket(128)
            w.WriteCmd(Commands.CMD_MC_LIFESKILL_ASK)
            w.WriteInt64(msg.Type)
            w.WriteInt64(msg.Result)
            w.WriteString(msg.Text)
            w

        let mcLifeSkillAsrMessage (msg: McLifeSkillAsrMessage) =
            let mutable w = WPacket(128)
            w.WriteCmd(Commands.CMD_MC_LIFESKILL_ASR)
            w.WriteInt64(msg.Type)
            w.WriteInt64(msg.Time)
            w.WriteString(msg.Text)
            w

        let mcDropLockAsrMessage (msg: McDropLockAsrMessage) =
            let mutable w = WPacket(16)
            w.WriteCmd(Commands.CMD_CM_ITEM_LOCK_ASR)
            w.WriteInt64(msg.Success)
            w

        let mcUnlockItemAsrMessage (msg: McUnlockItemAsrMessage) =
            let mutable w = WPacket(16)
            w.WriteCmd(Commands.CMD_MC_ITEM_UNLOCK_ASR)
            w.WriteInt64(msg.Result)
            w

        let mcStoreBuyAnswerMessage (msg: McStoreBuyAnswerMessage) =
            let mutable w = WPacket(24)
            w.WriteCmd(Commands.CMD_MC_STORE_BUY_ASR)
            w.WriteInt64(msg.Success)
            w.WriteInt64(msg.NewMoney)
            w

        let mcStoreChangeAnswerMessage (msg: McStoreChangeAnswerMessage) =
            let mutable w = WPacket(32)
            w.WriteCmd(Commands.CMD_MC_STORE_CHANGE_ASR)
            w.WriteInt64(msg.Success)
            w.WriteInt64(msg.MoBean)
            w.WriteInt64(msg.ReplMoney)
            w

        let mcDailyBuffInfoMessage (msg: McDailyBuffInfoMessage) =
            let mutable w = WPacket(128)
            w.WriteCmd(Commands.CMD_MC_RecDailyBuffInfo)
            w.WriteString(msg.ImgName)
            w.WriteString(msg.LabelInfo)
            w

        let mcRequestDropRateMessage (msg: McRequestDropRateMessage) =
            let mutable w = WPacket(16)
            w.WriteCmd(Commands.CMD_MC_REQUEST_DROP_RATE)
            w.WriteFloat32(msg.Rate)
            w

        let mcRequestExpRateMessage (msg: McRequestExpRateMessage) =
            let mutable w = WPacket(16)
            w.WriteCmd(Commands.CMD_MC_REQUEST_EXP_RATE)
            w.WriteFloat32(msg.Rate)
            w

        let mcTigerItemIdMessage (msg: McTigerItemIdMessage) =
            let mutable w = WPacket(40)
            w.WriteCmd(Commands.CMD_MC_TIGER_ITEM_ID)
            w.WriteInt64(msg.Num)
            w.WriteInt64(msg.ItemId0)
            w.WriteInt64(msg.ItemId1)
            w.WriteInt64(msg.ItemId2)
            w

        // ─── Serialize: гильдейские CM ──────────────────────────────

        let cmGuildPutNameMessage (msg: CmGuildPutNameMessage) =
            let mutable w = WPacket(128)
            w.WriteCmd(Commands.CMD_CM_GUILD_PUTNAME)
            w.WriteInt64(msg.Confirm)
            w.WriteString(msg.GuildName)
            w.WriteString(msg.Passwd)
            w

        let cmGuildTryForMessage (msg: CmGuildTryForMessage) =
            let mutable w = WPacket(16)
            w.WriteCmd(Commands.CMD_CM_GUILD_TRYFOR)
            w.WriteInt64(msg.GuildId)
            w

        let cmGuildTryForCfmMessage (msg: CmGuildTryForCfmMessage) =
            let mutable w = WPacket(16)
            w.WriteCmd(Commands.CMD_CM_GUILD_TRYFORCFM)
            w.WriteInt64(msg.Confirm)
            w

        let cmGuildListTryPlayerCmd () =
            let mutable w = WPacket(8)
            w.WriteCmd(Commands.CMD_CM_GUILD_LISTTRYPLAYER)
            w

        let cmGuildApproveMessage (msg: CmGuildApproveMessage) =
            let mutable w = WPacket(16)
            w.WriteCmd(Commands.CMD_CM_GUILD_APPROVE)
            w.WriteInt64(msg.ChaId)
            w

        let cmGuildRejectMessage (msg: CmGuildRejectMessage) =
            let mutable w = WPacket(16)
            w.WriteCmd(Commands.CMD_CM_GUILD_REJECT)
            w.WriteInt64(msg.ChaId)
            w

        let cmGuildKickMessage (msg: CmGuildKickMessage) =
            let mutable w = WPacket(16)
            w.WriteCmd(Commands.CMD_CM_GUILD_KICK)
            w.WriteInt64(msg.ChaId)
            w

        let cmGuildLeaveCmd () =
            let mutable w = WPacket(8)
            w.WriteCmd(Commands.CMD_CM_GUILD_LEAVE)
            w

        let cmGuildDisbandMessage (msg: CmGuildDisbandMessage) =
            let mutable w = WPacket(64)
            w.WriteCmd(Commands.CMD_CM_GUILD_DISBAND)
            w.WriteString(msg.Passwd)
            w

        let cmGuildMottoMessage (msg: CmGuildMottoMessage) =
            let mutable w = WPacket(128)
            w.WriteCmd(Commands.CMD_CM_GUILD_MOTTO)
            w.WriteString(msg.Motto)
            w

        let cmGuildChallMessage (msg: CmGuildChallMessage) =
            let mutable w = WPacket(24)
            w.WriteCmd(Commands.CMD_CM_GUILD_CHALLENGE)
            w.WriteInt64(msg.Level)
            w.WriteInt64(msg.Money)
            w

        let cmGuildLeizhuMessage (msg: CmGuildLeizhuMessage) =
            let mutable w = WPacket(24)
            w.WriteCmd(Commands.CMD_CM_GUILD_LEIZHU)
            w.WriteInt64(msg.Level)
            w.WriteInt64(msg.Money)
            w

        // ─── Serialize: гильдейские MC ──────────────────────────────

        let mcGuildTryForCfmMessage (msg: McGuildTryForCfmMessage) =
            let mutable w = WPacket(64)
            w.WriteCmd(Commands.CMD_MC_GUILD_TRYFORCFM)
            w.WriteString(msg.Name)
            w

        let mcGuildMottoMessage (msg: McGuildMottoMessage) =
            let mutable w = WPacket(128)
            w.WriteCmd(Commands.CMD_MC_GUILD_MOTTO)
            w.WriteString(msg.Motto)
            w

        let mcGuildInfoMessage (msg: McGuildInfoMessage) =
            let mutable w = WPacket(256)
            w.WriteCmd(Commands.CMD_MC_GUILD_INFO)
            w.WriteInt64(msg.CharId)
            w.WriteInt64(msg.GuildId)
            w.WriteString(msg.GuildName)
            w.WriteString(msg.GuildMotto)
            w.WriteInt64(msg.GuildPermission)
            w

        let mcGuildListChallMessage (msg: McGuildListChallMessage) =
            let mutable w = WPacket(512)
            w.WriteCmd(Commands.CMD_MC_GUILD_LISTCHALL)
            w.WriteInt64(msg.IsLeader)
            for i in 0..2 do
                let e = msg.Entries[i]
                w.WriteInt64(e.Level)
                if e.Level <> 0L then
                    w.WriteInt64(e.Start)
                    w.WriteString(e.GuildName)
                    w.WriteString(e.ChallName)
                    w.WriteInt64(e.Money)
            w

        // ─── Sub-packet helpers ────────────────────────────────────

        let inline serializeChaAttrInfo (w: byref<WPacket>) (a: ChaAttrInfo) =
            w.WriteInt64(a.SynType)
            w.WriteInt64(int64 a.Attrs.Length)
            for attr in a.Attrs do
                w.WriteInt64(attr.AttrId)
                w.WriteInt64(attr.AttrVal)

        let inline serializeChaSkillStateInfo (w: byref<WPacket>) (s: ChaSkillStateInfo) =
            w.WriteInt64(s.CurrentTime)
            w.WriteInt64(int64 s.States.Length)
            for st in s.States do
                w.WriteInt64(st.StateId)
                w.WriteInt64(st.StateLv)
                w.WriteInt64(st.Duration)
                w.WriteInt64(st.StartTime)

        /// Сериализация сумки скиллов (ChaSkillBagInfo) — условная запись range.
        let inline serializeChaSkillBagInfo (w: byref<WPacket>) (s: ChaSkillBagInfo) =
            w.WriteInt64(s.DefSkillId)
            w.WriteInt64(s.SynType)
            w.WriteInt64(int64 s.Skills.Length)
            for sk in s.Skills do
                w.WriteInt64(sk.Id)
                w.WriteInt64(sk.State)
                w.WriteInt64(sk.Level)
                w.WriteInt64(sk.UseSp)
                w.WriteInt64(sk.UseEndure)
                w.WriteInt64(sk.UseEnergy)
                w.WriteInt64(sk.ResumeTime)
                w.WriteInt64(sk.Range.[0])
                if sk.Range.[0] <> 0L then
                    for j = 1 to SKILL_RANGE_PARAM_NUM - 1 do
                        w.WriteInt64(sk.Range.[j])

        /// Сериализация ChaEventInfo.
        let inline serializeChaEventInfo (w: byref<WPacket>) (e: ChaEventInfo) =
            w.WriteInt64(e.EntityId)
            w.WriteInt64(e.EntityType)
            w.WriteInt64(e.EventId)
            w.WriteString(e.EventName)

        /// Сериализация слота экипировки (условная по synType и HasExtra).
        let inline serializeLookEquipSlot (w: byref<WPacket>) (synType: int64) (s: LookEquipSlot) =
            w.WriteInt64(s.Id)
            w.WriteInt64(s.DbId)
            w.WriteInt64(s.NeedLv)
            if s.Id <> 0L then
                if synType = SYN_LOOK_CHANGE then
                    w.WriteInt64(s.Endure0)
                    w.WriteInt64(s.Energy0)
                    w.WriteInt64(s.Valid)
                    w.WriteInt64(s.Tradable)
                    w.WriteInt64(s.Expiration)
                else
                    w.WriteInt64(s.Num)
                    w.WriteInt64(s.Endure0)
                    w.WriteInt64(s.Endure1)
                    w.WriteInt64(s.Energy0)
                    w.WriteInt64(s.Energy1)
                    w.WriteInt64(s.ForgeLv)
                    w.WriteInt64(s.Valid)
                    w.WriteInt64(s.Tradable)
                    w.WriteInt64(s.Expiration)
                    w.WriteInt64(if s.HasExtra then 1L else 0L)
                    if s.HasExtra then
                        w.WriteInt64(s.ForgeParam)
                        w.WriteInt64(s.InstId)
                        w.WriteInt64(if s.HasInstAttr then 1L else 0L)
                        if s.HasInstAttr then
                            for (a, b) in s.InstAttr do
                                w.WriteInt64(a)
                                w.WriteInt64(b)

        /// Сериализация внешнего вида (ChaLookInfo) — корабль или персонаж.
        let inline serializeChaLookInfo (w: byref<WPacket>) (look: ChaLookInfo) =
            w.WriteInt64(look.SynType)
            w.WriteInt64(look.TypeId)
            w.WriteInt64(if look.IsBoat then 1L else 0L)
            if look.IsBoat then
                match look.BoatParts with
                | ValueSome bp ->
                    w.WriteInt64(bp.PosId); w.WriteInt64(bp.BoatId); w.WriteInt64(bp.Header)
                    w.WriteInt64(bp.Body); w.WriteInt64(bp.Engine); w.WriteInt64(bp.Cannon); w.WriteInt64(bp.Equipment)
                | ValueNone -> ()
            else
                w.WriteInt64(look.HairId)
                for eq in look.Equips do
                    serializeLookEquipSlot &w look.SynType eq

        /// Сериализация доп. внешнего вида (AppendLook).
        let inline serializeAppendLook (w: byref<WPacket>) (slots: AppendLookSlot[]) =
            for s in slots do
                w.WriteInt64(s.LookId)
                if s.LookId <> 0L then
                    w.WriteInt64(s.Valid)

        /// Сериализация базовой информации персонажа (ChaBaseInfo).
        let inline serializeChaBaseInfo (w: byref<WPacket>) (b: ChaBaseInfo) =
            w.WriteInt64(b.ChaId); w.WriteInt64(b.WorldId); w.WriteInt64(b.CommId)
            w.WriteString(b.CommName)
            w.WriteInt64(b.GmLv); w.WriteInt64(b.Handle); w.WriteInt64(b.CtrlType)
            w.WriteString(b.Name); w.WriteString(b.Motto)
            w.WriteInt64(b.Icon); w.WriteInt64(b.GuildId)
            w.WriteString(b.GuildName); w.WriteString(b.GuildMotto); w.WriteInt64(b.GuildPermission)
            w.WriteString(b.StallName)
            w.WriteInt64(b.State); w.WriteInt64(b.PosX); w.WriteInt64(b.PosY)
            w.WriteInt64(b.Radius); w.WriteInt64(b.Angle)
            w.WriteInt64(b.TeamLeaderId); w.WriteInt64(b.IsPlayer)
            w.WriteInt64(b.Side.SideId)
            serializeChaEventInfo &w b.Event
            serializeChaLookInfo &w b.Look
            w.WriteInt64(b.PkCtrl)
            serializeAppendLook &w b.AppendLook

        /// Сериализация инвентаря (ChaKitbagInfo).
        let inline serializeChaKitbagInfo (w: byref<WPacket>) (k: ChaKitbagInfo) =
            w.WriteInt64(k.SynType)
            if k.SynType = SYN_KITBAG_INIT then
                w.WriteInt64(k.Capacity)
            for item in k.Items do
                w.WriteInt64(item.GridId)
                w.WriteInt64(item.ItemId)
                if item.ItemId > 0L then
                    w.WriteInt64(item.DbId); w.WriteInt64(item.NeedLv); w.WriteInt64(item.Num)
                    w.WriteInt64(item.Endure0); w.WriteInt64(item.Endure1)
                    w.WriteInt64(item.Energy0); w.WriteInt64(item.Energy1)
                    w.WriteInt64(item.ForgeLv); w.WriteInt64(item.Valid)
                    w.WriteInt64(item.Tradable); w.WriteInt64(item.Expiration)
                    w.WriteInt64(if item.IsBoat then 1L else 0L)
                    if item.IsBoat then w.WriteInt64(item.BoatWorldId)
                    w.WriteInt64(item.ForgeParam); w.WriteInt64(item.InstId)
                    w.WriteInt64(if item.HasInstAttr then 1L else 0L)
                    if item.HasInstAttr then
                        for (a, b) in item.InstAttr do
                            w.WriteInt64(a); w.WriteInt64(b)
            w.WriteInt64(-1L) // sentinel — конец списка предметов

        /// Сериализация панели быстрого доступа (ChaShortcutInfo).
        let inline serializeChaShortcutInfo (w: byref<WPacket>) (s: ChaShortcutInfo) =
            for e in s.Entries do
                w.WriteInt64(e.Type); w.WriteInt64(e.GridId)

        // ─── McChaBeginSee ─────────────────────────────────────────

        let mcChaBeginSeeMessage (msg: McChaBeginSeeMessage) =
            let mutable w = WPacket(4096)
            w.WriteCmd(Commands.CMD_MC_CHABEGINSEE)
            w.WriteInt64(msg.SeeType)
            serializeChaBaseInfo &w msg.Base
            w.WriteInt64(msg.NpcType)
            w.WriteInt64(msg.NpcState)
            w.WriteInt64(msg.PoseType)
            match msg.PoseType with
            | 1L -> // PoseLean
                match msg.Lean with
                | ValueSome l ->
                    w.WriteInt64(l.LeanState); w.WriteInt64(l.Pose); w.WriteInt64(l.Angle)
                    w.WriteInt64(l.PosX); w.WriteInt64(l.PosY); w.WriteInt64(l.Height)
                | ValueNone -> ()
            | 2L -> // PoseSeat
                match msg.Seat with
                | ValueSome s -> w.WriteInt64(s.SeatAngle); w.WriteInt64(s.SeatPose)
                | ValueNone -> ()
            | _ -> ()
            serializeChaAttrInfo &w msg.Attr
            serializeChaSkillStateInfo &w msg.SkillState
            w

        let mcAddItemChaMessage (msg: McAddItemChaMessage) =
            let mutable w = WPacket(4096)
            w.WriteCmd(Commands.CMD_MC_ADD_ITEM_CHA)
            w.WriteInt64(msg.MainChaId)
            serializeChaBaseInfo &w msg.Base
            serializeChaAttrInfo &w msg.Attr
            serializeChaKitbagInfo &w msg.Kitbag
            serializeChaSkillStateInfo &w msg.SkillState
            w

        // ─── McCharacterAction (CMD_MC_NOTIACTION) ──────────────

        /// Сериализация данных действия MOVE.
        let inline private serializeActionMove (w: byref<WPacket>) (d: ActionMoveData) =
            w.WriteInt64(d.MoveState)
            if d.MoveState <> enumMSTATE_ON then
                w.WriteInt64(d.StopState)
            w.WriteSequence(System.ReadOnlySpan(d.Waypoints))

        /// Сериализация данных действия SKILL_SRC.
        let inline private serializeActionSkillSrc (w: byref<WPacket>) (d: ActionSkillSrcData) =
            w.WriteInt64(d.FightId); w.WriteInt64(d.Angle); w.WriteInt64(d.State)
            if d.State <> enumFSTATE_ON then
                w.WriteInt64(d.StopState)
            w.WriteInt64(d.SkillId); w.WriteInt64(d.SkillSpeed)
            w.WriteInt64(d.TargetType)
            if d.TargetType = 1L then
                w.WriteInt64(d.TargetId); w.WriteInt64(d.TargetX); w.WriteInt64(d.TargetY)
            elif d.TargetType = 2L then
                w.WriteInt64(d.TargetX); w.WriteInt64(d.TargetY)
            w.WriteInt64(d.ExecTime)
            // Эффекты атрибутов
            w.WriteInt64(int64 d.Effects.Length)
            for e in d.Effects do
                w.WriteInt64(e.AttrId); w.WriteInt64(e.AttrVal)
            // Состояния скиллов
            w.WriteInt64(int64 d.States.Length)
            for s in d.States do
                w.WriteInt64(s.AStateId); w.WriteInt64(s.AStateLv)

        /// Сериализация данных действия SKILL_TAR.
        let inline private serializeActionSkillTar (w: byref<WPacket>) (d: ActionSkillTarData) =
            w.WriteInt64(d.FightId); w.WriteInt64(d.State)
            w.WriteInt64(if d.DoubleAttack then 1L else 0L)
            w.WriteInt64(if d.Miss then 1L else 0L)
            w.WriteInt64(if d.BeatBack then 1L else 0L)
            if d.BeatBack then
                w.WriteInt64(d.BeatBackX); w.WriteInt64(d.BeatBackY)
            w.WriteInt64(d.SrcId); w.WriteInt64(d.SrcPosX); w.WriteInt64(d.SrcPosY)
            w.WriteInt64(d.SkillId); w.WriteInt64(d.SkillTargetX); w.WriteInt64(d.SkillTargetY)
            w.WriteInt64(d.ExecTime)
            // Эффекты цели: synType + count + entries
            w.WriteInt64(d.SynType)
            w.WriteInt64(int64 d.Effects.Length)
            for e in d.Effects do
                w.WriteInt64(e.AttrId); w.WriteInt64(e.AttrVal)
            // Состояния цели
            if d.HasStates then
                w.WriteInt64(1L)
                w.WriteInt64(d.StateTime) // currentTime из WriteSkillState
                w.WriteInt64(int64 d.States.Length)
                for s in d.States do
                    w.WriteInt64(s.TarStateId); w.WriteInt64(s.TarStateLv)
                    w.WriteInt64(s.TarDuration); w.WriteInt64(s.TarStartTime)
            else
                w.WriteInt64(0L)
            // Эффекты источника
            if d.HasSrcEffect then
                w.WriteInt64(1L)
                w.WriteInt64(d.SrcState)
                w.WriteInt64(d.SrcSynType)
                w.WriteInt64(int64 d.SrcEffects.Length)
                for e in d.SrcEffects do
                    w.WriteInt64(e.AttrId); w.WriteInt64(e.AttrVal)
            else
                w.WriteInt64(0L)

        /// Сериализация CMD_MC_NOTIACTION.
        let mcCharacterActionMessage (msg: McCharacterActionMessage) =
            let mutable w = WPacket(4096)
            w.WriteCmd(Commands.CMD_MC_NOTIACTION)
            w.WriteInt64(msg.WorldId)
            w.WriteInt64(msg.PacketId)
            match msg.Action with
            | ActionMove d ->
                w.WriteInt64(ACT_MOVE)
                serializeActionMove &w d
            | ActionSkillSrc d ->
                w.WriteInt64(ACT_SKILL_SRC)
                serializeActionSkillSrc &w d
            | ActionSkillTar d ->
                w.WriteInt64(ACT_SKILL_TAR)
                serializeActionSkillTar &w d
            | ActionLean d ->
                w.WriteInt64(ACT_LEAN)
                w.WriteInt64(d.ActionLeanState)
                if d.ActionLeanState = 0L then
                    w.WriteInt64(d.ActionPose); w.WriteInt64(d.ActionAngle)
                    w.WriteInt64(d.ActionPosX); w.WriteInt64(d.ActionPosY); w.WriteInt64(d.ActionHeight)
            | ActionFace d ->
                w.WriteInt64(ACT_FACE)
                w.WriteInt64(d.FaceAngle); w.WriteInt64(d.FacePose)
            | ActionSkillPose d ->
                w.WriteInt64(ACT_SKILL_POSE)
                w.WriteInt64(d.FaceAngle); w.WriteInt64(d.FacePose)
            | ActionItemFailed failedId ->
                w.WriteInt64(ACT_ITEM_FAILED)
                w.WriteInt64(failedId)
            | ActionTemp (itemId, partId) ->
                w.WriteInt64(ACT_TEMP)
                w.WriteInt64(itemId); w.WriteInt64(partId)
            | ActionChangeCha mainChaId ->
                w.WriteInt64(ACT_CHANGE_CHA)
                w.WriteInt64(mainChaId)
            | ActionLook look ->
                w.WriteInt64(ACT_LOOK)
                serializeChaLookInfo &w look
            | ActionKitbag kb ->
                w.WriteInt64(ACT_KITBAG)
                serializeChaKitbagInfo &w kb
            | ActionBank kb ->
                w.WriteInt64(ACT_BANK)
                serializeChaKitbagInfo &w kb
            | ActionGuildBank kb ->
                w.WriteInt64(ACT_GUILDBANK)
                serializeChaKitbagInfo &w kb
            | ActionKitbagTmp kb ->
                w.WriteInt64(ACT_KITBAGTMP)
                serializeChaKitbagInfo &w kb
            | ActionShortcut sc ->
                w.WriteInt64(ACT_SHORTCUT)
                serializeChaShortcutInfo &w sc
            | ActionUnknown at ->
                w.WriteInt64(at)
            w

        /// Сериализация потребностей квеста (условная по NeedType).
        let inline serializeMisNeeds (w: byref<WPacket>) (needs: MisNeedEntry[]) =
            w.WriteInt64(int64 needs.Length)
            for n in needs do
                w.WriteInt64(n.NeedType)
                if n.NeedType = MIS_NEED_ITEM || n.NeedType = MIS_NEED_KILL then
                    w.WriteInt64(n.Param1)
                    w.WriteInt64(n.Param2)
                    w.WriteInt64(n.Param3)
                elif n.NeedType = MIS_NEED_DESP then
                    w.WriteString(n.Desp)

        /// Сериализация наград квеста.
        let inline serializeMisPrizes (w: byref<WPacket>) (prizes: MisPrizeEntry[]) =
            w.WriteInt64(int64 prizes.Length)
            for p in prizes do
                w.WriteInt64(p.Type)
                w.WriteInt64(p.Param1)
                w.WriteInt64(p.Param2)

        // ─── Фаза 3: MC — сложные ─────────────────────────────────

        let mcSynAttributeMessage (msg: McSynAttributeMessage) =
            let mutable w = WPacket(256)
            w.WriteCmd(Commands.CMD_MC_SYNATTR)
            w.WriteInt64(msg.WorldId)
            serializeChaAttrInfo &w msg.Attr
            w

        let mcSynSkillStateMessage (msg: McSynSkillStateMessage) =
            let mutable w = WPacket(256)
            w.WriteCmd(Commands.CMD_MC_SYNASKILLSTATE)
            w.WriteInt64(msg.WorldId)
            serializeChaSkillStateInfo &w msg.SkillState
            w

        let mcAStateEndSeeMessage (msg: McAStateEndSeeMessage) =
            let mutable w = WPacket(24)
            w.WriteCmd(Commands.CMD_MC_ASTATEENDSEE)
            w.WriteInt64(msg.AreaX)
            w.WriteInt64(msg.AreaY)
            w

        let mcAStateBeginSeeMessage (msg: McAStateBeginSeeMessage) =
            let mutable w = WPacket(128)
            w.WriteCmd(Commands.CMD_MC_ASTATEBEGINSEE)
            w.WriteInt64(msg.AreaX)
            w.WriteInt64(msg.AreaY)
            w.WriteInt64(int64 msg.States.Length)
            for s in msg.States do
                w.WriteInt64(s.StateId)
                if s.StateId <> 0L then
                    w.WriteInt64(s.StateLv)
                    w.WriteInt64(s.WorldId)
                    w.WriteInt64(s.FightId)
            w

        let mcDelItemChaMessage (msg: McDelItemChaMessage) =
            let mutable w = WPacket(24)
            w.WriteCmd(Commands.CMD_MC_DEL_ITEM_CHA)
            w.WriteInt64(msg.MainChaId)
            w.WriteInt64(msg.WorldId)
            w

        let mcSynEventInfoMessage (msg: McSynEventInfoMessage) =
            let mutable w = WPacket(64)
            w.WriteCmd(Commands.CMD_MC_EVENT_INFO)
            w.WriteInt64(msg.EntityId)
            w.WriteInt64(msg.EntityType)
            w.WriteInt64(msg.EventId)
            w.WriteString(msg.EventName)
            w

        let mcSynSideInfoMessage (msg: McSynSideInfoMessage) =
            let mutable w = WPacket(24)
            w.WriteCmd(Commands.CMD_MC_SIDE_INFO)
            w.WriteInt64(msg.WorldId)
            w.WriteInt64(msg.Side.SideId)
            w

        let mcItemCreateMessage (msg: McItemCreateMessage) =
            let mutable w = WPacket(256)
            w.WriteCmd(Commands.CMD_MC_ITEMBEGINSEE)
            w.WriteInt64(msg.WorldId)
            w.WriteInt64(msg.Handle)
            w.WriteInt64(msg.ItemId)
            w.WriteInt64(msg.PosX)
            w.WriteInt64(msg.PosY)
            w.WriteInt64(msg.Angle)
            w.WriteInt64(msg.Num)
            w.WriteInt64(msg.AppeType)
            w.WriteInt64(msg.FromId)
            serializeChaEventInfo &w msg.Event
            w

        let mcSynSkillBagMessage (msg: McSynSkillBagMessage) =
            let mutable w = WPacket(512)
            w.WriteCmd(Commands.CMD_MC_SYNSKILLBAG)
            w.WriteInt64(msg.WorldId)
            serializeChaSkillBagInfo &w msg.SkillBag
            w

        let mcMissionInfoMessage (msg: McMissionInfoMessage) =
            let mutable w = WPacket(256)
            w.WriteCmd(Commands.CMD_MC_MISSION)
            w.WriteInt64(msg.NpcId)
            w.WriteInt64(msg.ListType)
            w.WriteInt64(msg.Prev)
            w.WriteInt64(msg.Next)
            w.WriteInt64(msg.PrevCmd)
            w.WriteInt64(msg.NextCmd)
            w.WriteInt64(int64 msg.Items.Length)
            for item in msg.Items do w.WriteString(item)
            w

        let mcMisPageMessage (msg: McMisPageMessage) =
            let mutable w = WPacket(512)
            w.WriteCmd(Commands.CMD_MC_MISPAGE)
            w.WriteInt64(msg.Cmd)
            w.WriteInt64(msg.NpcId)
            w.WriteString(msg.Name)
            serializeMisNeeds &w msg.Needs
            w.WriteInt64(msg.PrizeSelType)
            serializeMisPrizes &w msg.Prizes
            w.WriteString(msg.Description)
            w

        let mcMisLogMessage (msg: McMisLogMessage) =
            let mutable w = WPacket(256)
            w.WriteCmd(Commands.CMD_MC_MISLOG)
            w.WriteInt64(int64 msg.Logs.Length)
            for l in msg.Logs do
                w.WriteInt64(l.MisId)
                w.WriteInt64(l.State)
            w

        let mcMisLogInfoMessage (msg: McMisLogInfoMessage) =
            let mutable w = WPacket(512)
            w.WriteCmd(Commands.CMD_MC_MISLOGINFO)
            w.WriteInt64(msg.MisId)
            w.WriteString(msg.Name)
            serializeMisNeeds &w msg.Needs
            w.WriteInt64(msg.PrizeSelType)
            serializeMisPrizes &w msg.Prizes
            w.WriteString(msg.Description)
            w

        let mcMisLogClearMessage (msg: McMisLogClearMessage) =
            let mutable w = WPacket(16)
            w.WriteCmd(Commands.CMD_MC_MISLOG_CLEAR)
            w.WriteInt64(msg.MissionId)
            w

        let mcMisLogAddMessage (msg: McMisLogAddMessage) =
            let mutable w = WPacket(24)
            w.WriteCmd(Commands.CMD_MC_MISLOG_ADD)
            w.WriteInt64(msg.MissionId)
            w.WriteInt64(msg.State)
            w

        let mcMisLogStateMessage (msg: McMisLogStateMessage) =
            let mutable w = WPacket(24)
            w.WriteCmd(Commands.CMD_MC_MISLOG_CHANGE)
            w.WriteInt64(msg.MissionId)
            w.WriteInt64(msg.State)
            w

        let mcFuncInfoMessage (msg: McFuncInfoMessage) =
            let mutable w = WPacket(512)
            w.WriteCmd(Commands.CMD_MC_FUNCPAGE)
            w.WriteInt64(msg.NpcId)
            w.WriteInt64(msg.Page)
            w.WriteString(msg.TalkText)
            w.WriteInt64(int64 msg.FuncItems.Length)
            for f in msg.FuncItems do w.WriteString(f.Name)
            w.WriteInt64(int64 msg.MissionItems.Length)
            for m in msg.MissionItems do
                w.WriteString(m.Name)
                w.WriteInt64(m.State)
            w

        let mcVolunteerListMessage (msg: McVolunteerListMessage) =
            let mutable w = WPacket(256)
            w.WriteCmd(Commands.CMD_MC_VOLUNTER_LIST)
            w.WriteInt64(msg.PageTotal)
            w.WriteInt64(msg.Page)
            w.WriteInt64(int64 msg.Volunteers.Length)
            for v in msg.Volunteers do
                w.WriteString(v.Name)
                w.WriteInt64(v.Level)
                w.WriteInt64(v.Job)
                w.WriteString(v.Map)
            w

        let mcVolunteerOpenMessage (msg: McVolunteerOpenMessage) =
            let mutable w = WPacket(256)
            w.WriteCmd(Commands.CMD_MC_VOLUNTER_OPEN)
            w.WriteInt64(msg.State)
            w.WriteInt64(msg.PageTotal)
            w.WriteInt64(int64 msg.Volunteers.Length)
            for v in msg.Volunteers do
                w.WriteString(v.Name)
                w.WriteInt64(v.Level)
                w.WriteInt64(v.Job)
                w.WriteString(v.Map)
            w

        let mcShowStallSearchMessage (msg: McShowStallSearchMessage) =
            let mutable w = WPacket(512)
            w.WriteCmd(Commands.CMD_MC_STALLSEARCH)
            w.WriteInt64(int64 msg.Entries.Length)
            for e in msg.Entries do
                w.WriteString(e.Name)
                w.WriteString(e.StallName)
                w.WriteString(e.Location)
                w.WriteInt64(e.Count)
                w.WriteInt64(e.Cost)
            w

        let mcShowRankingMessage (msg: McShowRankingMessage) =
            let mutable w = WPacket(512)
            w.WriteCmd(Commands.CMD_MC_RANK)
            w.WriteInt64(int64 msg.Entries.Length)
            for e in msg.Entries do
                w.WriteString(e.Name)
                w.WriteString(e.Guild)
                w.WriteInt64(e.Level)
                w.WriteInt64(e.Job)
                w.WriteInt64(e.Score)
            w

        let mcEspeItemMessage (msg: McEspeItemMessage) =
            let mutable w = WPacket(128)
            w.WriteCmd(Commands.CMD_MC_ESPE_ITEM)
            w.WriteInt64(msg.WorldId)
            w.WriteInt64(int64 msg.Items.Length)
            for item in msg.Items do
                w.WriteInt64(item.Position)
                w.WriteInt64(item.Endure)
                w.WriteInt64(item.Energy)
                w.WriteInt64(item.Tradable)
                w.WriteInt64(item.Expiration)
            w

        let mcBlackMarketExchangeDataMessage (msg: McBlackMarketExchangeDataMessage) =
            let mutable w = WPacket(128)
            w.WriteCmd(Commands.CMD_MC_BLACKMARKET_EXCHANGEDATA)
            w.WriteInt64(msg.NpcId)
            w.WriteInt64(int64 msg.Exchanges.Length)
            for e in msg.Exchanges do
                w.WriteInt64(e.SrcId)
                w.WriteInt64(e.SrcCount)
                w.WriteInt64(e.TarId)
                w.WriteInt64(e.TarCount)
                w.WriteInt64(e.TimeValue)
            w

        let mcExchangeDataMessage (msg: McExchangeDataMessage) =
            let mutable w = WPacket(128)
            w.WriteCmd(Commands.CMD_MC_EXCHANGEDATA)
            w.WriteInt64(msg.NpcId)
            w.WriteInt64(int64 msg.Exchanges.Length)
            for e in msg.Exchanges do
                w.WriteInt64(e.SrcId)
                w.WriteInt64(e.SrcCount)
                w.WriteInt64(e.TarId)
                w.WriteInt64(e.TarCount)
            w

        let mcBlackMarketExchangeUpdateMessage (msg: McBlackMarketExchangeUpdateMessage) =
            let mutable w = WPacket(128)
            w.WriteCmd(Commands.CMD_MC_BLACKMARKET_EXCHANGEUPDATE)
            w.WriteInt64(msg.NpcId)
            w.WriteInt64(int64 msg.Exchanges.Length)
            for e in msg.Exchanges do
                w.WriteInt64(e.SrcId)
                w.WriteInt64(e.SrcCount)
                w.WriteInt64(e.TarId)
                w.WriteInt64(e.TarCount)
                w.WriteInt64(e.TimeValue)
            w

        let mcBlackMarketExchangeAsrMessage (msg: McBlackMarketExchangeAsrMessage) =
            let mutable w = WPacket(48)
            w.WriteCmd(Commands.CMD_MC_BLACKMARKET_EXCHANGE_ASR)
            w.WriteInt64(msg.Success)
            w.WriteInt64(msg.SrcId)
            w.WriteInt64(msg.SrcCount)
            w.WriteInt64(msg.TarId)
            w.WriteInt64(msg.TarCount)
            w

        // ─── MC — торговля NPC (полные данные) ───────────────────

        let mcTradeAllDataMessage (msg: McTradeAllDataMessage) =
            let mutable w = WPacket(512)
            w.WriteCmd(Commands.CMD_MC_TRADE_ALLDATA)
            w.WriteInt64(msg.NpcId)
            w.WriteInt64(msg.TradeType)
            w.WriteInt64(msg.Param)
            w.WriteInt64(int64 msg.Pages.Length)
            for page in msg.Pages do
                w.WriteInt64(page.ItemType)
                w.WriteInt64(int64 page.Items.Length)
                for item in page.Items do
                    w.WriteInt64(item.ItemId)
                // Если TRADE_GOODS (type==1) — доп. поля count/price/level
                if msg.TradeType = 1L then
                    for item in page.Items do
                        w.WriteInt64(item.Count)
                        w.WriteInt64(item.Price)
                        w.WriteInt64(item.Level)
            w

        // ─── MC — магазин (Store) ────────────────────────────────

        let mcStoreOpenAnswerMessage (msg: McStoreOpenAnswerMessage) =
            let mutable w = WPacket(512)
            w.WriteCmd(Commands.CMD_MC_STORE_OPEN_ASR)
            w.WriteInt64(if msg.IsValid then 1L else 0L)
            if msg.IsValid then
                w.WriteInt64(msg.Vip)
                w.WriteInt64(msg.MoBean)
                w.WriteInt64(msg.ReplMoney)
                w.WriteInt64(int64 msg.Affiches.Length)
                for a in msg.Affiches do
                    w.WriteInt64(a.AfficheId)
                    w.WriteString(a.Title)
                    w.WriteString(a.ComId)
                w.WriteInt64(int64 msg.Classes.Length)
                for c in msg.Classes do
                    w.WriteInt64(c.ClassId)
                    w.WriteString(c.ClassName)
                    w.WriteInt64(c.ParentId)
            w

        let mcStoreListAnswerMessage (msg: McStoreListAnswerMessage) =
            let mutable w = WPacket(1024)
            w.WriteCmd(Commands.CMD_MC_STORE_LIST_ASR)
            w.WriteInt64(msg.PageTotal)
            w.WriteInt64(msg.PageCurrent)
            w.WriteInt64(int64 msg.Products.Length)
            for p in msg.Products do
                w.WriteInt64(p.ComId)
                w.WriteString(p.ComName)
                w.WriteInt64(p.Price)
                w.WriteString(p.Remark)
                w.WriteInt64(if p.IsHot then 1L else 0L)
                w.WriteInt64(p.Time)
                w.WriteInt64(p.Quantity)
                w.WriteInt64(p.Expire)
                w.WriteInt64(int64 p.Variants.Length)
                // Атрибуты записываются внутри каждого варианта (по 5 пар на вариант)
                for v in p.Variants do
                    w.WriteInt64(v.ItemId)
                    w.WriteInt64(v.ItemNum)
                    w.WriteInt64(v.Flute)
                    for i in 0 .. 4 do
                        let a = if i < v.Attrs.Length then v.Attrs[i] else { AttrEntry.AttrId = 0L; AttrVal = 0L }
                        w.WriteInt64(a.AttrId)
                        w.WriteInt64(a.AttrVal)
            w

        let mcStoreHistoryMessage (msg: McStoreHistoryMessage) =
            let mutable w = WPacket(256)
            w.WriteCmd(Commands.CMD_MC_STORE_QUERY)
            w.WriteInt64(int64 msg.Records.Length)
            for r in msg.Records do
                w.WriteString(r.Time)
                w.WriteInt64(r.ItemId)
                w.WriteString(r.Name)
                w.WriteInt64(r.Money)
            w

        let mcStoreVipMessage (msg: McStoreVipMessage) =
            let mutable w = WPacket(48)
            w.WriteCmd(Commands.CMD_MC_STORE_VIP)
            w.WriteInt64(msg.Success)
            if msg.Success <> 0L then
                w.WriteInt64(msg.VipId)
                w.WriteInt64(msg.Months)
                w.WriteInt64(msg.Shuijing)
                w.WriteInt64(msg.Modou)
            w

        // ─── MC — синхронизация команды (пати) ──────────────────

        let mcSynTeamMessage (msg: McSynTeamMessage) =
            let mutable w = WPacket(512)
            w.WriteCmd(Commands.CMD_MC_TEAM)
            w.WriteInt64(msg.MemberId)
            w.WriteInt64(msg.HP)
            w.WriteInt64(msg.MaxHP)
            w.WriteInt64(msg.SP)
            w.WriteInt64(msg.MaxSP)
            w.WriteInt64(msg.Level)
            serializeChaLookInfo &w msg.Look
            w

        // ─── CMD_MC_NOTIACTION UPDATEGUILDLOGS/REQUESTGUILDLOGS ──

        /// Сериализация CMD_MC_NOTIACTION + UPDATEGUILDLOGS.
        let mcUpdateGuildLogsMessage (msg: McUpdateGuildLogsMessage) =
            let mutable w = WPacket(1024)
            w.WriteCmd(Commands.CMD_MC_NOTIACTION)
            w.WriteInt64(msg.WorldId)
            w.WriteInt64(msg.PacketId)
            w.WriteInt64(ACT_UPDATEGUILDLOGS)
            w.WriteInt64(msg.TotalSize)
            for l in msg.Logs do
                w.WriteInt64(l.Type); w.WriteInt64(l.Time)
                w.WriteInt64(l.Parameter); w.WriteInt64(l.Quantity); w.WriteInt64(l.UserId)
            if msg.Terminated then w.WriteInt64(9L)
            w

        /// Сериализация CMD_MC_NOTIACTION + REQUESTGUILDLOGS.
        let mcRequestGuildLogsMessage (msg: McRequestGuildLogsMessage) =
            let mutable w = WPacket(1024)
            w.WriteCmd(Commands.CMD_MC_NOTIACTION)
            w.WriteInt64(msg.WorldId)
            w.WriteInt64(msg.PacketId)
            w.WriteInt64(ACT_REQUESTGUILDLOGS)
            for l in msg.Logs do
                w.WriteInt64(l.Type); w.WriteInt64(l.Time)
                w.WriteInt64(l.Parameter); w.WriteInt64(l.Quantity); w.WriteInt64(l.UserId)
            if msg.Terminated then w.WriteInt64(9L)
            w

        // ─── CMD_MC_CHARTRADE + CMD_MC_CHARTRADE_ITEM ───────────

        /// Сериализация CMD_MC_CHARTRADE + CMD_MC_CHARTRADE_ITEM (единое сообщение).
        let mcCharTradeItemMessage (msg: McCharTradeItemMessage) =
            let mutable w = WPacket(512)
            w.WriteCmd(Commands.CMD_MC_CHARTRADE)
            w.WriteInt64(int64 Commands.CMD_MC_CHARTRADE_ITEM)
            w.WriteInt64(msg.MainChaId); w.WriteInt64(msg.OpType)
            match msg.Data with
            | Remove rem ->
                w.WriteInt64(rem.BagIndex); w.WriteInt64(rem.TradeIndex); w.WriteInt64(rem.Count)
            | Add add ->
                w.WriteInt64(add.ItemId); w.WriteInt64(add.BagIndex)
                w.WriteInt64(add.TradeIndex); w.WriteInt64(add.Count); w.WriteInt64(add.ItemType)
                match add.EquipData with
                | Choice1Of2 boat ->
                    if boat.HasBoat then
                        w.WriteInt64(1L); w.WriteString(boat.Name)
                        w.WriteInt64(boat.Ship); w.WriteInt64(boat.Lv)
                        w.WriteInt64(boat.Cexp); w.WriteInt64(boat.HP)
                        w.WriteInt64(boat.MxHP); w.WriteInt64(boat.SP)
                        w.WriteInt64(boat.MxSP); w.WriteInt64(boat.MnAtk)
                        w.WriteInt64(boat.MxAtk); w.WriteInt64(boat.Def)
                        w.WriteInt64(boat.MSpd); w.WriteInt64(boat.ASpd)
                        w.WriteInt64(boat.UseGridNum); w.WriteInt64(boat.Capacity)
                        w.WriteInt64(boat.Price)
                    else
                        w.WriteInt64(0L)
                | Choice2Of2 item ->
                    w.WriteInt64(item.Endure0); w.WriteInt64(item.Endure1)
                    w.WriteInt64(item.Energy0); w.WriteInt64(item.Energy1)
                    w.WriteInt64(item.ForgeLv); w.WriteInt64(item.Valid)
                    w.WriteInt64(item.Tradable); w.WriteInt64(item.Expiration)
                    w.WriteInt64(item.ForgeParam); w.WriteInt64(item.InstId)
                    if item.HasInstAttr then
                        w.WriteInt64(1L)
                        for j in 0 .. ITEM_INSTANCE_ATTR_NUM - 1 do
                            let (a, b) = item.InstAttr[j]
                            w.WriteInt64(a); w.WriteInt64(b)
                    else
                        w.WriteInt64(0L)
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

        let pcTeamCancelMessage (pk: IRPacket) : PcTeamCancelMessage =
            { Reason = pk.ReadInt64(); ChaId = pk.ReadInt64() }

        let pcFrndInviteMessage (pk: IRPacket) : PcFrndInviteMessage =
            { InviterName = pk.ReadString(); ChaId = pk.ReadInt64(); Icon = pk.ReadInt64() }

        let pcFrndCancelMessage (pk: IRPacket) : PcFrndCancelMessage =
            { Reason = pk.ReadInt64(); ChaId = pk.ReadInt64() }

        let pcFrndRefreshMessage (pk: IRPacket) : PcFrndRefreshMessage =
            { Msg = pk.ReadInt64(); Group = pk.ReadString(); ChaId = pk.ReadInt64()
              ChaName = pk.ReadString(); Motto = pk.ReadString(); Icon = pk.ReadInt64() }

        let pcFrndRefreshDelMessage (pk: IRPacket) : PcFrndRefreshDelMessage =
            { Msg = pk.ReadInt64(); ChaId = pk.ReadInt64() }

        /// Десериализация тегированного сообщения PC_GM_INFO.
        let pcGmInfoMessage (pk: IRPacket) : PcGmInfoMessage =
            let ty = pk.ReadInt64()
            let emptyAdd = { GmFrndAddEntry.Group = ""; ChaId = 0L; ChaName = ""; Motto = ""; Icon = 0L }
            match ty with
            | 1L -> // MSG_FRND_REFRESH_START
                let count = int (pk.ReadInt64())
                let entries = Array.init count (fun _ ->
                    { GmFrndEntry.ChaId = pk.ReadInt64(); ChaName = pk.ReadString()
                      Motto = pk.ReadString(); Icon = pk.ReadInt64(); Status = pk.ReadInt64() })
                { Type = ty; Entries = entries; ChaId = 0L; AddEntry = emptyAdd }
            | 4L | 5L | 3L -> // ONLINE / OFFLINE / DEL
                { Type = ty; Entries = [||]; ChaId = pk.ReadInt64(); AddEntry = emptyAdd }
            | 2L -> // MSG_FRND_REFRESH_ADD
                let add =
                    { GmFrndAddEntry.Group = pk.ReadString(); ChaId = pk.ReadInt64()
                      ChaName = pk.ReadString(); Motto = pk.ReadString(); Icon = pk.ReadInt64() }
                { Type = ty; Entries = [||]; ChaId = 0L; AddEntry = add }
            | _ ->
                { Type = ty; Entries = [||]; ChaId = 0L; AddEntry = emptyAdd }

        /// Десериализация тегированного сообщения PC_FRND_REFRESH.
        let pcFrndRefreshFullMessage (pk: IRPacket) : PcFrndRefreshFullMessage =
            let ty = pk.ReadInt64()
            let emptyAdd = { GmFrndAddEntry.Group = ""; ChaId = 0L; ChaName = ""; Motto = ""; Icon = 0L }
            let emptySelf = { FrndSelfData.ChaId = 0L; ChaName = ""; Motto = ""; Icon = 0L }
            match ty with
            | 4L | 5L | 3L -> // ONLINE / OFFLINE / DEL
                { Type = ty; ChaId = pk.ReadInt64(); AddEntry = emptyAdd; Self = emptySelf; Groups = [||] }
            | 2L -> // MSG_FRND_REFRESH_ADD
                let add =
                    { GmFrndAddEntry.Group = pk.ReadString(); ChaId = pk.ReadInt64()
                      ChaName = pk.ReadString(); Motto = pk.ReadString(); Icon = pk.ReadInt64() }
                { Type = ty; ChaId = 0L; AddEntry = add; Self = emptySelf; Groups = [||] }
            | 1L -> // MSG_FRND_REFRESH_START
                let self =
                    { FrndSelfData.ChaId = pk.ReadInt64(); ChaName = pk.ReadString()
                      Motto = pk.ReadString(); Icon = pk.ReadInt64() }
                let groupCount = int (pk.ReadInt64())
                let groups = Array.init groupCount (fun _ ->
                    let gn = pk.ReadString()
                    let mc = int (pk.ReadInt64())
                    let members = Array.init mc (fun _ ->
                        { GmFrndEntry.ChaId = pk.ReadInt64(); ChaName = pk.ReadString()
                          Motto = pk.ReadString(); Icon = pk.ReadInt64(); Status = pk.ReadInt64() })
                    { FrndGroupData.GroupName = gn; Members = members })
                { Type = ty; ChaId = 0L; AddEntry = emptyAdd; Self = self; Groups = groups }
            | _ ->
                { Type = ty; ChaId = 0L; AddEntry = emptyAdd; Self = emptySelf; Groups = [||] }

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

        /// Вспомогательная: десериализация одного участника гильдии.
        let inline private deserializeGuildMemberEntry (pk: IRPacket) : GuildMemberEntry =
            { Online = pk.ReadInt64(); ChaId = pk.ReadInt64()
              ChaName = pk.ReadString(); Motto = pk.ReadString(); Job = pk.ReadString()
              Degree = pk.ReadInt64(); Icon = pk.ReadInt64(); Permission = pk.ReadInt64() }

        /// CMD_PC_GUILD — составное сообщение (подкоманда в Msg).
        let pcGuildMessage (pk: IRPacket) : PcGuildMessage =
            let emptyMember = { Online = 0L; ChaId = 0L; ChaName = ""; Motto = ""; Job = ""; Degree = 0L; Icon = 0L; Permission = 0L }
            let msg = pk.ReadInt64()
            match msg with
            | 4L | 5L | 3L -> // ONLINE / OFFLINE / DEL
                { Msg = msg; ChaId = pk.ReadInt64(); PacketIndex = 0L
                  GuildId = 0L; GuildName = ""; LeaderId = 0L
                  Members = [||]; AddMember = emptyMember }
            | 1L -> // START
                let count = int (pk.ReadInt64())
                let packetIndex = pk.ReadInt64()
                let guildId, guildName, leaderId =
                    if packetIndex = 0L && count > 0 then
                        pk.ReadInt64(), pk.ReadString(), pk.ReadInt64()
                    else
                        0L, "", 0L
                let members = Array.init count (fun _ -> deserializeGuildMemberEntry pk)
                { Msg = msg; ChaId = 0L; PacketIndex = packetIndex
                  GuildId = guildId; GuildName = guildName; LeaderId = leaderId
                  Members = members; AddMember = emptyMember }
            | 6L -> // STOP — нет данных
                { Msg = msg; ChaId = 0L; PacketIndex = 0L
                  GuildId = 0L; GuildName = ""; LeaderId = 0L
                  Members = [||]; AddMember = emptyMember }
            | 2L -> // ADD
                let addMember = deserializeGuildMemberEntry pk
                { Msg = msg; ChaId = 0L; PacketIndex = 0L
                  GuildId = 0L; GuildName = ""; LeaderId = 0L
                  Members = [||]; AddMember = addMember }
            | _ ->
                { Msg = msg; ChaId = 0L; PacketIndex = 0L
                  GuildId = 0L; GuildName = ""; LeaderId = 0L
                  Members = [||]; AddMember = emptyMember }

        let pcMasterRefreshAddMessage (pk: IRPacket) : PcMasterRefreshAddMessage =
            { Msg = pk.ReadInt64(); Group = pk.ReadString(); ChaId = pk.ReadInt64()
              ChaName = pk.ReadString(); Motto = pk.ReadString(); Icon = pk.ReadInt64() }

        let pcMasterRefreshDelMessage (pk: IRPacket) : PcMasterRefreshDelMessage =
            { Msg = pk.ReadInt64(); ChaId = pk.ReadInt64() }

        let pcSessCreateMessage (pk: IRPacket) : PcSessCreateMessage =
            let sessId = pk.ReadInt64()
            if sessId = 0L then
                { SessId = 0L; ErrorMsg = pk.ReadString(); Members = [||]; NotiPlyCount = 0L }
            else
                let count = int (pk.ReadInt64())
                let members = Array.init count (fun _ ->
                    { SessMemberData.ChaId = pk.ReadInt64(); ChaName = pk.ReadString()
                      Motto = pk.ReadString(); Icon = pk.ReadInt64() })
                { SessId = sessId; ErrorMsg = ""; Members = members; NotiPlyCount = pk.ReadInt64() }

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

        // ─── Группа H: TM ────────────────────────────────────

        let tmEnterMapMessage (pk: IRPacket) : TmEnterMapMessage =
            { ActId = uint32 (pk.ReadInt64()); Password = pk.ReadString()
              DatabaseId = uint32 (pk.ReadInt64()); WorldId = uint32 (pk.ReadInt64())
              MapName = pk.ReadString(); MapCopyNo = int (pk.ReadInt64())
              X = uint32 (pk.ReadInt64()); Y = uint32 (pk.ReadInt64())
              IsSwitch = pk.ReadInt64() <> 0L
              GateAddr = uint32 (pk.ReadInt64()); GarnerWinner = int16 (pk.ReadInt64()) }

        // ─── Группа: Геймплейные CM (с полями) ────────────────────

        let cmDieReturnMessage (pk: IRPacket) : CmDieReturnMessage =
            { ReliveType = pk.ReadInt64() }

        let cmAutoKitbagLockMessage (pk: IRPacket) : CmAutoKitbagLockMessage =
            { AutoLock = pk.ReadInt64() }

        let cmStallSearchMessage (pk: IRPacket) : CmStallSearchMessage =
            { ItemId = pk.ReadInt64() }

        let cmForgeItemMessage (pk: IRPacket) : CmForgeItemMessage =
            { Index = pk.ReadInt64() }

        let cmEntityEventMessage (pk: IRPacket) : CmEntityEventMessage =
            { EntityId = pk.ReadInt64() }

        let cmStallOpenMessage (pk: IRPacket) : CmStallOpenMessage =
            { CharId = pk.ReadInt64() }

        let cmMisLogInfoMessage (pk: IRPacket) : CmMisLogInfoMessage =
            { Id = pk.ReadInt64() }

        let cmMisLogClearMessage (pk: IRPacket) : CmMisLogClearMessage =
            { Id = pk.ReadInt64() }

        let cmStoreBuyAskMessage (pk: IRPacket) : CmStoreBuyAskMessage =
            { ComId = pk.ReadInt64() }

        let cmStoreChangeAskMessage (pk: IRPacket) : CmStoreChangeAskMessage =
            { Num = pk.ReadInt64() }

        let cmStoreQueryMessage (pk: IRPacket) : CmStoreQueryMessage =
            { Num = pk.ReadInt64() }

        let cmTeamFightAnswerMessage (pk: IRPacket) : CmTeamFightAnswerMessage =
            { Accept = pk.ReadInt64() }

        let cmItemRepairAnswerMessage (pk: IRPacket) : CmItemRepairAnswerMessage =
            { Accept = pk.ReadInt64() }

        let cmItemForgeAnswerMessage (pk: IRPacket) : CmItemForgeAnswerMessage =
            { Accept = pk.ReadInt64() }

        let cmItemLotteryAnswerMessage (pk: IRPacket) : CmItemLotteryAnswerMessage =
            { Accept = pk.ReadInt64() }

        let cmVolunteerOpenMessage (pk: IRPacket) : CmVolunteerOpenMessage =
            { Num = pk.ReadInt64() }

        let cmVolunteerListMessage (pk: IRPacket) : CmVolunteerListMessage =
            { Page = pk.ReadInt64(); Num = pk.ReadInt64() }

        let cmStoreListAskMessage (pk: IRPacket) : CmStoreListAskMessage =
            { ClsId = pk.ReadInt64(); Page = pk.ReadInt64(); Num = pk.ReadInt64() }

        let cmCaptainConfirmAsrMessage (pk: IRPacket) : CmCaptainConfirmAsrMessage =
            { Ret = pk.ReadInt64(); TeamId = pk.ReadInt64() }

        let cmMapMaskMessage (pk: IRPacket) : CmMapMaskMessage =
            { MapName = pk.ReadString() }

        let cmStoreOpenAskMessage (pk: IRPacket) : CmStoreOpenAskMessage =
            { Password = pk.ReadString() }

        let cmVolunteerSelMessage (pk: IRPacket) : CmVolunteerSelMessage =
            { Name = pk.ReadString() }

        let cmKitbagUnlockMessage (pk: IRPacket) : CmKitbagUnlockMessage =
            { Password = pk.ReadString() }

        // ─── Группа: Геймплейные MC (простые) ─────────────────────

        let mcFailedActionMessage (pk: IRPacket) : McFailedActionMessage =
            { WorldId = pk.ReadInt64(); ActionType = pk.ReadInt64(); Reason = pk.ReadInt64() }

        let mcChaEndSeeMessage (pk: IRPacket) : McChaEndSeeMessage =
            { SeeType = pk.ReadInt64(); WorldId = pk.ReadInt64() }

        let mcItemDestroyMessage (pk: IRPacket) : McItemDestroyMessage =
            { WorldId = pk.ReadInt64() }

        let mcForgeResultMessage (pk: IRPacket) : McForgeResultMessage =
            { Result = pk.ReadInt64() }

        let mcUniteResultMessage (pk: IRPacket) : McUniteResultMessage =
            { Result = pk.ReadInt64() }

        let mcMillingResultMessage (pk: IRPacket) : McMillingResultMessage =
            { Result = pk.ReadInt64() }

        let mcFusionResultMessage (pk: IRPacket) : McFusionResultMessage =
            { Result = pk.ReadInt64() }

        let mcUpgradeResultMessage (pk: IRPacket) : McUpgradeResultMessage =
            { Result = pk.ReadInt64() }

        let mcPurifyResultMessage (pk: IRPacket) : McPurifyResultMessage =
            { Result = pk.ReadInt64() }

        let mcFixResultMessage (pk: IRPacket) : McFixResultMessage =
            { Result = pk.ReadInt64() }

        let mcEidolonMetempsychosisMessage (pk: IRPacket) : McEidolonMetempsychosisMessage =
            { Result = pk.ReadInt64() }

        let mcEidolonFusionMessage (pk: IRPacket) : McEidolonFusionMessage =
            { Result = pk.ReadInt64() }

        let mcMessageMessage (pk: IRPacket) : McMessageMessage =
            { Text = pk.ReadString() }

        let mcBickerNoticeMessage (pk: IRPacket) : McBickerNoticeMessage =
            { Text = pk.ReadString() }

        let mcColourNoticeMessage (pk: IRPacket) : McColourNoticeMessage =
            { Color = pk.ReadInt64(); Text = pk.ReadString() }

        let mcTriggerActionMessage (pk: IRPacket) : McTriggerActionMessage =
            { Type = pk.ReadInt64(); Id = pk.ReadInt64(); Num = pk.ReadInt64(); Count = pk.ReadInt64() }

        let mcNpcStateChangeMessage (pk: IRPacket) : McNpcStateChangeMessage =
            { NpcId = pk.ReadInt64(); State = pk.ReadInt64() }

        let mcEntityStateChangeMessage (pk: IRPacket) : McEntityStateChangeMessage =
            { EntityId = pk.ReadInt64(); State = pk.ReadInt64() }

        let mcCloseTalkMessage (pk: IRPacket) : McCloseTalkMessage =
            { NpcId = pk.ReadInt64() }

        let mcKitbagCheckAnswerMessage (pk: IRPacket) : McKitbagCheckAnswerMessage =
            { Locked = pk.ReadInt64() }

        let mcPreMoveTimeMessage (pk: IRPacket) : McPreMoveTimeMessage =
            { Time = pk.ReadInt64() }

        let mcItemUseSuccMessage (pk: IRPacket) : McItemUseSuccMessage =
            { WorldId = pk.ReadInt64(); ItemId = pk.ReadInt64() }

        let mcChaPlayEffectMessage (pk: IRPacket) : McChaPlayEffectMessage =
            { WorldId = pk.ReadInt64(); EffectId = pk.ReadInt64() }

        let mcSynDefaultSkillMessage (pk: IRPacket) : McSynDefaultSkillMessage =
            { WorldId = pk.ReadInt64(); SkillId = pk.ReadInt64() }

        // ─── MC — ReadSequence→ReadString миграция ─────────────────

        let mcSayMessage (pk: IRPacket) : McSayMessage =
            { SourceId = pk.ReadInt64(); Content = pk.ReadString(); Color = pk.ReadInt64() }

        let mcSysInfoMessage (pk: IRPacket) : McSysInfoMessage =
            { Info = pk.ReadString() }

        let mcPopupNoticeMessage (pk: IRPacket) : McPopupNoticeMessage =
            { Notice = pk.ReadString() }

        let mcPingMessage (pk: IRPacket) : McPingMessage =
            { V1 = pk.ReadInt64(); V2 = pk.ReadInt64(); V3 = pk.ReadInt64()
              V4 = pk.ReadInt64(); V5 = pk.ReadInt64() }

        // ─── Фаза 2: CM — средние ─────────────────────────────────

        let cmChangePassMessage (pk: IRPacket) : CmChangePassMessage =
            { Pass = pk.ReadString(); Pin = pk.ReadString() }

        let cmGuildBankOperMessage (pk: IRPacket) : CmGuildBankOperMessage =
            { Op = pk.ReadInt64(); SrcType = pk.ReadInt64(); SrcId = pk.ReadInt64()
              SrcNum = pk.ReadInt64(); TarType = pk.ReadInt64(); TarId = pk.ReadInt64() }

        let cmGuildBankGoldMessage (pk: IRPacket) : CmGuildBankGoldMessage =
            { Op = pk.ReadInt64(); Direction = pk.ReadInt64(); Gold = pk.ReadInt64() }

        let cmUpdateHairMessage (pk: IRPacket) : CmUpdateHairMessage =
            { ScriptId = pk.ReadInt64(); GridLoc0 = pk.ReadInt64(); GridLoc1 = pk.ReadInt64()
              GridLoc2 = pk.ReadInt64(); GridLoc3 = pk.ReadInt64() }

        let cmTeamFightAskMessage (pk: IRPacket) : CmTeamFightAskMessage =
            { Type = pk.ReadInt64(); WorldId = pk.ReadInt64(); Handle = pk.ReadInt64() }

        let cmItemRepairAskMessage (pk: IRPacket) : CmItemRepairAskMessage =
            { RepairmanId = pk.ReadInt64(); RepairmanHandle = pk.ReadInt64()
              PosType = pk.ReadInt64(); PosId = pk.ReadInt64() }

        let cmRequestTradeMessage (pk: IRPacket) : CmRequestTradeMessage =
            { Type = pk.ReadInt64(); CharId = pk.ReadInt64() }

        let cmAcceptTradeMessage (pk: IRPacket) : CmAcceptTradeMessage =
            { Type = pk.ReadInt64(); CharId = pk.ReadInt64() }

        let cmCancelTradeMessage (pk: IRPacket) : CmCancelTradeMessage =
            { Type = pk.ReadInt64(); CharId = pk.ReadInt64() }

        let cmValidateTradeDataMessage (pk: IRPacket) : CmValidateTradeDataMessage =
            { Type = pk.ReadInt64(); CharId = pk.ReadInt64() }

        let cmValidateTradeMessage (pk: IRPacket) : CmValidateTradeMessage =
            { Type = pk.ReadInt64(); CharId = pk.ReadInt64() }

        let cmAddItemMessage (pk: IRPacket) : CmAddItemMessage =
            { Type = pk.ReadInt64(); CharId = pk.ReadInt64(); OpType = pk.ReadInt64()
              Index = pk.ReadInt64(); ItemIndex = pk.ReadInt64(); Count = pk.ReadInt64() }

        let cmAddMoneyMessage (pk: IRPacket) : CmAddMoneyMessage =
            { Type = pk.ReadInt64(); CharId = pk.ReadInt64(); OpType = pk.ReadInt64()
              IsImp = pk.ReadInt64(); Money = pk.ReadInt64() }

        let cmTigerStartMessage (pk: IRPacket) : CmTigerStartMessage =
            { NpcId = pk.ReadInt64(); Sel1 = pk.ReadInt64(); Sel2 = pk.ReadInt64(); Sel3 = pk.ReadInt64() }

        let cmTigerStopMessage (pk: IRPacket) : CmTigerStopMessage =
            { NpcId = pk.ReadInt64(); Num = pk.ReadInt64() }

        let cmVolunteerAsrMessage (pk: IRPacket) : CmVolunteerAsrMessage =
            { Ret = pk.ReadInt64(); Name = pk.ReadString() }

        let cmCreateBoatMessage (pk: IRPacket) : CmCreateBoatMessage =
            { Boat = pk.ReadString(); Header = pk.ReadInt64(); Engine = pk.ReadInt64()
              Cannon = pk.ReadInt64(); Equipment = pk.ReadInt64() }

        let cmUpdateBoatMessage (pk: IRPacket) : CmUpdateBoatMessage =
            { Header = pk.ReadInt64(); Engine = pk.ReadInt64()
              Cannon = pk.ReadInt64(); Equipment = pk.ReadInt64() }

        let cmSelectBoatListMessage (pk: IRPacket) : CmSelectBoatListMessage =
            { NpcId = pk.ReadInt64(); Type = pk.ReadInt64(); Index = pk.ReadInt64() }

        let cmBoatLaunchMessage (pk: IRPacket) : CmBoatLaunchMessage =
            { NpcId = pk.ReadInt64(); Index = pk.ReadInt64() }

        let cmBoatBagSelMessage (pk: IRPacket) : CmBoatBagSelMessage =
            { NpcId = pk.ReadInt64(); Index = pk.ReadInt64() }

        let cmReportWgMessage (pk: IRPacket) : CmReportWgMessage =
            { Info = pk.ReadString() }

        let cmSay2CampMessage (pk: IRPacket) : CmSay2CampMessage =
            { Content = pk.ReadString() }

        let cmStallBuyMessage (pk: IRPacket) : CmStallBuyMessage =
            { CharId = pk.ReadInt64(); Index = pk.ReadInt64()
              Count = pk.ReadInt64(); GridId = pk.ReadInt64() }

        let cmSkillUpgradeMessage (pk: IRPacket) : CmSkillUpgradeMessage =
            { SkillId = pk.ReadInt64(); AddGrade = pk.ReadInt64() }

        let cmRefreshDataMessage (pk: IRPacket) : CmRefreshDataMessage =
            { WorldId = pk.ReadInt64(); Handle = pk.ReadInt64() }

        let cmPkCtrlMessage (pk: IRPacket) : CmPkCtrlMessage =
            { Ctrl = pk.ReadInt64() }

        let cmItemAmphitheaterAskMessage (pk: IRPacket) : CmItemAmphitheaterAskMessage =
            { Sure = pk.ReadInt64(); ReId = pk.ReadInt64() }

        let cmMasterInviteMessage (pk: IRPacket) : CmMasterInviteMessage =
            { Name = pk.ReadString(); ChaId = pk.ReadInt64() }

        let cmMasterAsrMessage (pk: IRPacket) : CmMasterAsrMessage =
            { Agree = pk.ReadInt64(); Name = pk.ReadString(); ChaId = pk.ReadInt64() }

        let cmMasterDelMessage (pk: IRPacket) : CmMasterDelMessage =
            { Name = pk.ReadString(); ChaId = pk.ReadInt64() }

        let cmPrenticeInviteMessage (pk: IRPacket) : CmPrenticeInviteMessage =
            { Name = pk.ReadString(); ChaId = pk.ReadInt64() }

        let cmPrenticeAsrMessage (pk: IRPacket) : CmPrenticeAsrMessage =
            { Agree = pk.ReadInt64(); Name = pk.ReadString(); ChaId = pk.ReadInt64() }

        let cmPrenticeDelMessage (pk: IRPacket) : CmPrenticeDelMessage =
            { Name = pk.ReadString(); ChaId = pk.ReadInt64() }

        // ─── Фаза 2: NPC Talk compound commands ────────────────────
        // Десериализация составных команд: суб-команда читается и отбрасывается (|> ignore).
        // Маршрутизация по суб-команде выполняется внешним диспетчером.

        let cmRequestTalkMessage (pk: IRPacket) : CmRequestTalkMessage =
            let npcId = pk.ReadInt64()
            pk.ReadInt64() |> ignore  // CMD_CM_TALKPAGE
            { NpcId = npcId; Cmd = pk.ReadInt64() }

        let cmSelFunctionMessage (pk: IRPacket) : CmSelFunctionMessage =
            let npcId = pk.ReadInt64()
            pk.ReadInt64() |> ignore  // CMD_CM_FUNCITEM
            { NpcId = npcId; PageId = pk.ReadInt64(); Index = pk.ReadInt64() }

        let cmSaleMessage (pk: IRPacket) : CmSaleMessage =
            let npcId = pk.ReadInt64()
            pk.ReadInt64() |> ignore  // CMD_CM_TRADEITEM
            pk.ReadInt64() |> ignore  // ROLE_TRADE_SALE
            { NpcId = npcId; Index = pk.ReadInt64(); Count = pk.ReadInt64() }

        let cmBuyMessage (pk: IRPacket) : CmBuyMessage =
            let npcId = pk.ReadInt64()
            pk.ReadInt64() |> ignore  // CMD_CM_TRADEITEM
            pk.ReadInt64() |> ignore  // ROLE_TRADE_BUY
            { NpcId = npcId; ItemType = pk.ReadInt64(); Index1 = pk.ReadInt64()
              Index2 = pk.ReadInt64(); Count = pk.ReadInt64() }

        let cmMissionPageMessage (pk: IRPacket) : CmMissionPageMessage =
            let npcId = pk.ReadInt64()
            pk.ReadInt64() |> ignore  // CMD_CM_MISSION
            { NpcId = npcId; Cmd = pk.ReadInt64(); SelItem = pk.ReadInt64(); Param = pk.ReadInt64() }

        let cmSelMissionMessage (pk: IRPacket) : CmSelMissionMessage =
            let npcId = pk.ReadInt64()
            pk.ReadInt64() |> ignore  // CMD_CM_MISSION
            pk.ReadInt64() |> ignore  // ROLE_MIS_SEL
            { NpcId = npcId; Index = pk.ReadInt64() }

        let cmBlackMarketExchangeReqMessage (pk: IRPacket) : CmBlackMarketExchangeReqMessage =
            let npcId = pk.ReadInt64()
            pk.ReadInt64() |> ignore  // CMD_CM_BLACKMARKET_EXCHANGE_REQ
            { NpcId = npcId; TimeNum = pk.ReadInt64(); SrcId = pk.ReadInt64()
              SrcNum = pk.ReadInt64(); TarId = pk.ReadInt64(); TarNum = pk.ReadInt64()
              Index = pk.ReadInt64() }

        // ─── Фаза 2: MC — средние ─────────────────────────────────

        let mcSay2CampMessage (pk: IRPacket) : McSay2CampMessage =
            { ChaName = pk.ReadString(); Content = pk.ReadString() }

        let mcTalkInfoMessage (pk: IRPacket) : McTalkInfoMessage =
            { NpcId = pk.ReadInt64(); Cmd = pk.ReadInt64(); Text = pk.ReadString() }

        let mcTradeDataMessage (pk: IRPacket) : McTradeDataMessage =
            { NpcId = pk.ReadInt64(); Page = pk.ReadInt64(); Index = pk.ReadInt64()
              ItemId = pk.ReadInt64(); Count = pk.ReadInt64(); Price = pk.ReadInt64() }

        let mcTradeResultMessage (pk: IRPacket) : McTradeResultMessage =
            { Type = pk.ReadInt64(); Index = pk.ReadInt64(); Count = pk.ReadInt64()
              ItemId = pk.ReadInt64(); Money = pk.ReadInt64() }

        let mcUpdateHairResMessage (pk: IRPacket) : McUpdateHairResMessage =
            { WorldId = pk.ReadInt64(); ScriptId = pk.ReadInt64(); Reason = pk.ReadString() }

        let mcSynTLeaderIdMessage (pk: IRPacket) : McSynTLeaderIdMessage =
            { WorldId = pk.ReadInt64(); LeaderId = pk.ReadInt64() }

        let mcKitbagCapacityMessage (pk: IRPacket) : McKitbagCapacityMessage =
            { WorldId = pk.ReadInt64(); Capacity = pk.ReadInt64() }

        let mcItemForgeAskMessage (pk: IRPacket) : McItemForgeAskMessage =
            { Type = pk.ReadInt64(); Money = pk.ReadInt64() }

        let mcItemForgeAnswerMessage (pk: IRPacket) : McItemForgeAnswerMessage =
            { WorldId = pk.ReadInt64(); Type = pk.ReadInt64(); Result = pk.ReadInt64() }

        let mcQueryChaMessage (pk: IRPacket) : McQueryChaMessage =
            { ChaId = pk.ReadInt64(); Name = pk.ReadString(); MapName = pk.ReadString()
              PosX = pk.ReadInt64(); PosY = pk.ReadInt64(); ChaId2 = pk.ReadInt64() }

        let mcQueryChaPingMessage (pk: IRPacket) : McQueryChaPingMessage =
            { ChaId = pk.ReadInt64(); Name = pk.ReadString(); MapName = pk.ReadString()
              Ping = pk.ReadInt64() }

        let mcQueryReliveMessage (pk: IRPacket) : McQueryReliveMessage =
            { ChaId = pk.ReadInt64(); SourceName = pk.ReadString(); ReliveType = pk.ReadInt64() }

        let mcGmMailMessage (pk: IRPacket) : McGmMailMessage =
            { Title = pk.ReadString(); Content = pk.ReadString(); Time = pk.ReadInt64() }

        let mcSynStallNameMessage (pk: IRPacket) : McSynStallNameMessage =
            { CharId = pk.ReadInt64(); Name = pk.ReadString() }

        let mcMapCrashMessage (pk: IRPacket) : McMapCrashMessage =
            { Text = pk.ReadString() }

        let mcVolunteerStateMessage (pk: IRPacket) : McVolunteerStateMessage =
            { State = pk.ReadInt64() }

        let mcVolunteerAskMessage (pk: IRPacket) : McVolunteerAskMessage =
            { Name = pk.ReadString() }

        let mcMasterAskMessage (pk: IRPacket) : McMasterAskMessage =
            { Name = pk.ReadString(); ChaId = pk.ReadInt64() }

        let mcPrenticeAskMessage (pk: IRPacket) : McPrenticeAskMessage =
            { Name = pk.ReadString(); ChaId = pk.ReadInt64() }

        let mcItemRepairAskMessage (pk: IRPacket) : McItemRepairAskMessage =
            { ItemName = pk.ReadString(); RepairCost = pk.ReadInt64() }

        let mcItemLotteryAsrMessage (pk: IRPacket) : McItemLotteryAsrMessage =
            { Success = pk.ReadInt64() }

        // ─── MC/CM — Фаза 4: простые ────────────────────────────

        let mcChaEmotionMessage (pk: IRPacket) : McChaEmotionMessage =
            { WorldId = pk.ReadInt64(); Emotion = pk.ReadInt64() }

        let mcStartExitMessage (pk: IRPacket) : McStartExitMessage =
            { ExitTime = pk.ReadInt64() }

        let mcGmRecvMessage (pk: IRPacket) : McGmRecvMessage =
            { NpcId = pk.ReadInt64() }

        let mcStallDelGoodsMessage (pk: IRPacket) : McStallDelGoodsMessage =
            { CharId = pk.ReadInt64(); Grid = pk.ReadInt64(); Count = pk.ReadInt64() }

        let mcStallCloseMessage (pk: IRPacket) : McStallCloseMessage =
            { CharId = pk.ReadInt64() }

        let mcStallSuccessMessage (pk: IRPacket) : McStallSuccessMessage =
            { CharId = pk.ReadInt64() }

        let mcUpdateGuildGoldMessage (pk: IRPacket) : McUpdateGuildGoldMessage =
            { Data = pk.ReadString() }

        let mcQueryChaItemMessage (pk: IRPacket) : McQueryChaItemMessage =
            { ChaId = pk.ReadInt64() }

        let mcDisconnectMessage (pk: IRPacket) : McDisconnectMessage =
            { Reason = pk.ReadInt64() }

        let mcLifeSkillShowMessage (pk: IRPacket) : McLifeSkillShowMessage =
            { Type = pk.ReadInt64() }

        let mcLifeSkillMessage (pk: IRPacket) : McLifeSkillMessage =
            { Type = pk.ReadInt64(); Result = pk.ReadInt64(); Text = pk.ReadString() }

        let mcLifeSkillAsrMessage (pk: IRPacket) : McLifeSkillAsrMessage =
            { Type = pk.ReadInt64(); Time = pk.ReadInt64(); Text = pk.ReadString() }

        let mcDropLockAsrMessage (pk: IRPacket) : McDropLockAsrMessage =
            { Success = pk.ReadInt64() }

        let mcUnlockItemAsrMessage (pk: IRPacket) : McUnlockItemAsrMessage =
            { Result = pk.ReadInt64() }

        let mcStoreBuyAnswerMessage (pk: IRPacket) : McStoreBuyAnswerMessage =
            { Success = pk.ReadInt64(); NewMoney = pk.ReadInt64() }

        let mcStoreChangeAnswerMessage (pk: IRPacket) : McStoreChangeAnswerMessage =
            { Success = pk.ReadInt64(); MoBean = pk.ReadInt64(); ReplMoney = pk.ReadInt64() }

        let mcDailyBuffInfoMessage (pk: IRPacket) : McDailyBuffInfoMessage =
            { ImgName = pk.ReadString(); LabelInfo = pk.ReadString() }

        let mcRequestDropRateMessage (pk: IRPacket) : McRequestDropRateMessage =
            { Rate = pk.ReadFloat32() }

        let mcRequestExpRateMessage (pk: IRPacket) : McRequestExpRateMessage =
            { Rate = pk.ReadFloat32() }

        let mcTigerItemIdMessage (pk: IRPacket) : McTigerItemIdMessage =
            { Num = pk.ReadInt64(); ItemId0 = pk.ReadInt64(); ItemId1 = pk.ReadInt64(); ItemId2 = pk.ReadInt64() }

        // ─── Deserialize: гильдейские CM ─────────────────────────────

        let cmGuildPutNameMessage (pk: IRPacket) : CmGuildPutNameMessage =
            { Confirm = pk.ReadInt64(); GuildName = pk.ReadString(); Passwd = pk.ReadString() }

        let cmGuildTryForMessage (pk: IRPacket) : CmGuildTryForMessage =
            { GuildId = pk.ReadInt64() }

        let cmGuildTryForCfmMessage (pk: IRPacket) : CmGuildTryForCfmMessage =
            { Confirm = pk.ReadInt64() }

        let cmGuildApproveMessage (pk: IRPacket) : CmGuildApproveMessage =
            { ChaId = pk.ReadInt64() }

        let cmGuildRejectMessage (pk: IRPacket) : CmGuildRejectMessage =
            { ChaId = pk.ReadInt64() }

        let cmGuildKickMessage (pk: IRPacket) : CmGuildKickMessage =
            { ChaId = pk.ReadInt64() }

        let cmGuildDisbandMessage (pk: IRPacket) : CmGuildDisbandMessage =
            { Passwd = pk.ReadString() }

        let cmGuildMottoMessage (pk: IRPacket) : CmGuildMottoMessage =
            { Motto = pk.ReadString() }

        let cmGuildChallMessage (pk: IRPacket) : CmGuildChallMessage =
            { Level = pk.ReadInt64(); Money = pk.ReadInt64() }

        let cmGuildLeizhuMessage (pk: IRPacket) : CmGuildLeizhuMessage =
            { Level = pk.ReadInt64(); Money = pk.ReadInt64() }

        // ─── Deserialize: гильдейские MC ─────────────────────────────

        let mcGuildTryForCfmMessage (pk: IRPacket) : McGuildTryForCfmMessage =
            { Name = pk.ReadString() }

        let mcGuildMottoMessage (pk: IRPacket) : McGuildMottoMessage =
            { Motto = pk.ReadString() }

        let mcGuildInfoMessage (pk: IRPacket) : McGuildInfoMessage =
            { CharId = pk.ReadInt64(); GuildId = pk.ReadInt64()
              GuildName = pk.ReadString(); GuildMotto = pk.ReadString()
              GuildPermission = pk.ReadInt64() }

        let mcGuildListChallMessage (pk: IRPacket) : McGuildListChallMessage =
            let isLeader = pk.ReadInt64()
            let entries = Array.init 3 (fun _ ->
                let level = pk.ReadInt64()
                if level <> 0L then
                    { Level = level; Start = pk.ReadInt64()
                      GuildName = pk.ReadString(); ChallName = pk.ReadString()
                      Money = pk.ReadInt64() }
                else
                    { Level = 0L; Start = 0L; GuildName = ""; ChallName = ""; Money = 0L })
            { IsLeader = isLeader; Entries = entries }

        // ─── Sub-packet helpers ────────────────────────────────────

        let inline deserializeChaAttrInfo (pk: IRPacket) : ChaAttrInfo =
            let synType = pk.ReadInt64()
            let count = int (pk.ReadInt64())
            let attrs = Array.init count (fun _ -> { AttrId = pk.ReadInt64(); AttrVal = pk.ReadInt64() })
            { SynType = synType; Attrs = attrs }

        let inline deserializeChaSkillStateInfo (pk: IRPacket) : ChaSkillStateInfo =
            let currentTime = pk.ReadInt64()
            let count = int (pk.ReadInt64())
            let states = Array.init count (fun _ ->
                { StateId = pk.ReadInt64(); StateLv = pk.ReadInt64()
                  Duration = pk.ReadInt64(); StartTime = pk.ReadInt64() })
            { CurrentTime = currentTime; States = states }

        /// Десериализация сумки скиллов (ChaSkillBagInfo).
        let inline deserializeChaSkillBagInfo (pk: IRPacket) : ChaSkillBagInfo =
            let defSkillId = pk.ReadInt64()
            let synType = pk.ReadInt64()
            let count = int (pk.ReadInt64())
            let skills = Array.init count (fun _ ->
                let id = pk.ReadInt64()
                let state = pk.ReadInt64()
                let level = pk.ReadInt64()
                let useSp = pk.ReadInt64()
                let useEndure = pk.ReadInt64()
                let useEnergy = pk.ReadInt64()
                let resumeTime = pk.ReadInt64()
                let range = Array.zeroCreate SKILL_RANGE_PARAM_NUM
                range.[0] <- pk.ReadInt64()
                if range.[0] <> 0L then
                    for j = 1 to SKILL_RANGE_PARAM_NUM - 1 do
                        range.[j] <- pk.ReadInt64()
                { Id = id; State = state; Level = level; UseSp = useSp; UseEndure = useEndure
                  UseEnergy = useEnergy; ResumeTime = resumeTime; Range = range })
            { DefSkillId = defSkillId; SynType = synType; Skills = skills }

        /// Десериализация ChaEventInfo.
        let inline deserializeChaEventInfo (pk: IRPacket) : ChaEventInfo =
            { EntityId = pk.ReadInt64(); EntityType = pk.ReadInt64()
              EventId = pk.ReadInt64(); EventName = pk.ReadString() }

        /// Десериализация слота экипировки (условная по synType).
        let inline deserializeLookEquipSlot (pk: IRPacket) (synType: int64) : LookEquipSlot =
            let id = pk.ReadInt64()
            let dbId = pk.ReadInt64()
            let needLv = pk.ReadInt64()
            if id = 0L then
                { Id = 0L; DbId = dbId; NeedLv = needLv; Num = 0L
                  Endure0 = 0L; Endure1 = 0L; Energy0 = 0L; Energy1 = 0L
                  ForgeLv = 0L; Valid = 0L; Tradable = 0L; Expiration = 0L
                  HasExtra = false; ForgeParam = 0L; InstId = 0L; HasInstAttr = false; InstAttr = [||] }
            elif synType = SYN_LOOK_CHANGE then
                { Id = id; DbId = dbId; NeedLv = needLv; Num = 0L
                  Endure0 = pk.ReadInt64(); Endure1 = 0L
                  Energy0 = pk.ReadInt64(); Energy1 = 0L
                  ForgeLv = 0L
                  Valid = pk.ReadInt64(); Tradable = pk.ReadInt64(); Expiration = pk.ReadInt64()
                  HasExtra = false; ForgeParam = 0L; InstId = 0L; HasInstAttr = false; InstAttr = [||] }
            else
                let num = pk.ReadInt64()
                let e0 = pk.ReadInt64() in let e1 = pk.ReadInt64()
                let en0 = pk.ReadInt64() in let en1 = pk.ReadInt64()
                let forgeLv = pk.ReadInt64()
                let valid = pk.ReadInt64() in let tradable = pk.ReadInt64() in let expiration = pk.ReadInt64()
                let hasExtra = pk.ReadInt64() <> 0L
                if not hasExtra then
                    { Id = id; DbId = dbId; NeedLv = needLv; Num = num
                      Endure0 = e0; Endure1 = e1; Energy0 = en0; Energy1 = en1
                      ForgeLv = forgeLv; Valid = valid; Tradable = tradable; Expiration = expiration
                      HasExtra = false; ForgeParam = 0L; InstId = 0L; HasInstAttr = false; InstAttr = [||] }
                else
                    let forgeParam = pk.ReadInt64()
                    let instId = pk.ReadInt64()
                    let hasInstAttr = pk.ReadInt64() <> 0L
                    let instAttr =
                        if hasInstAttr then Array.init ITEM_INSTANCE_ATTR_NUM (fun _ -> (pk.ReadInt64(), pk.ReadInt64()))
                        else [||]
                    { Id = id; DbId = dbId; NeedLv = needLv; Num = num
                      Endure0 = e0; Endure1 = e1; Energy0 = en0; Energy1 = en1
                      ForgeLv = forgeLv; Valid = valid; Tradable = tradable; Expiration = expiration
                      HasExtra = true; ForgeParam = forgeParam; InstId = instId
                      HasInstAttr = hasInstAttr; InstAttr = instAttr }

        /// Десериализация внешнего вида (ChaLookInfo).
        let inline deserializeChaLookInfo (pk: IRPacket) : ChaLookInfo =
            let synType = pk.ReadInt64()
            let typeId = pk.ReadInt64()
            let isBoat = pk.ReadInt64() <> 0L
            if isBoat then
                let bp = { PosId = pk.ReadInt64(); BoatId = pk.ReadInt64(); Header = pk.ReadInt64()
                           Body = pk.ReadInt64(); Engine = pk.ReadInt64(); Cannon = pk.ReadInt64(); Equipment = pk.ReadInt64() }
                { SynType = synType; TypeId = typeId; IsBoat = true; BoatParts = ValueSome bp; HairId = 0L; Equips = [||] }
            else
                let hairId = pk.ReadInt64()
                let equips = Array.init EQUIP_NUM (fun _ -> deserializeLookEquipSlot pk synType)
                { SynType = synType; TypeId = typeId; IsBoat = false; BoatParts = ValueNone; HairId = hairId; Equips = equips }

        /// Десериализация доп. внешнего вида (AppendLook).
        let inline deserializeAppendLook (pk: IRPacket) : AppendLookSlot[] =
            Array.init ESPE_KBGRID_NUM (fun _ ->
                let lookId = pk.ReadInt64()
                if lookId <> 0L then { LookId = lookId; Valid = pk.ReadInt64() }
                else { LookId = 0L; Valid = 0L })

        /// Десериализация базовой информации персонажа (ChaBaseInfo).
        let inline deserializeChaBaseInfo (pk: IRPacket) : ChaBaseInfo =
            let chaId = pk.ReadInt64() in let worldId = pk.ReadInt64() in let commId = pk.ReadInt64()
            let commName = pk.ReadString()
            let gmLv = pk.ReadInt64() in let handle = pk.ReadInt64() in let ctrlType = pk.ReadInt64()
            let name = pk.ReadString() in let motto = pk.ReadString()
            let icon = pk.ReadInt64() in let guildId = pk.ReadInt64()
            let guildName = pk.ReadString() in let guildMotto = pk.ReadString() in let guildPermission = pk.ReadInt64()
            let stallName = pk.ReadString()
            let state = pk.ReadInt64() in let posX = pk.ReadInt64() in let posY = pk.ReadInt64()
            let radius = pk.ReadInt64() in let angle = pk.ReadInt64()
            let teamLeaderId = pk.ReadInt64() in let isPlayer = pk.ReadInt64()
            let side = { ChaSideInfo.SideId = pk.ReadInt64() }
            let event = deserializeChaEventInfo pk
            let look = deserializeChaLookInfo pk
            let pkCtrl = pk.ReadInt64()
            let appendLook = deserializeAppendLook pk
            { ChaId = chaId; WorldId = worldId; CommId = commId; CommName = commName
              GmLv = gmLv; Handle = handle; CtrlType = ctrlType
              Name = name; Motto = motto; Icon = icon; GuildId = guildId
              GuildName = guildName; GuildMotto = guildMotto; GuildPermission = guildPermission
              StallName = stallName; State = state; PosX = posX; PosY = posY
              Radius = radius; Angle = angle; TeamLeaderId = teamLeaderId; IsPlayer = isPlayer
              Side = side; Event = event; Look = look; PkCtrl = pkCtrl; AppendLook = appendLook }

        /// Десериализация инвентаря (ChaKitbagInfo).
        let inline deserializeChaKitbagInfo (pk: IRPacket) : ChaKitbagInfo =
            let synType = pk.ReadInt64()
            let capacity = if synType = SYN_KITBAG_INIT then pk.ReadInt64() else 0L
            let items = System.Collections.Generic.List<KitbagItem>()
            let mutable go = true
            while go do
                let gridId = pk.ReadInt64()
                if gridId < 0L then go <- false
                else
                    let itemId = pk.ReadInt64()
                    if itemId <= 0L then
                        let emptyItem =
                            { GridId = gridId; ItemId = 0L; DbId = 0L; NeedLv = 0L; Num = 0L
                              Endure0 = 0L; Endure1 = 0L; Energy0 = 0L; Energy1 = 0L
                              ForgeLv = 0L; Valid = 0L; Tradable = 0L; Expiration = 0L
                              IsBoat = false; BoatWorldId = 0L; ForgeParam = 0L; InstId = 0L
                              HasInstAttr = false; InstAttr = [||] }
                        items.Add(emptyItem)
                    else
                        let dbId = pk.ReadInt64() in let needLv = pk.ReadInt64() in let num = pk.ReadInt64()
                        let e0 = pk.ReadInt64() in let e1 = pk.ReadInt64()
                        let en0 = pk.ReadInt64() in let en1 = pk.ReadInt64()
                        let forgeLv = pk.ReadInt64() in let valid = pk.ReadInt64()
                        let tradable = pk.ReadInt64() in let expiration = pk.ReadInt64()
                        let isBoat = pk.ReadInt64() <> 0L
                        let boatWorldId = if isBoat then pk.ReadInt64() else 0L
                        let forgeParam = pk.ReadInt64() in let instId = pk.ReadInt64()
                        let hasInstAttr = pk.ReadInt64() <> 0L
                        let instAttr =
                            if hasInstAttr then Array.init ITEM_INSTANCE_ATTR_NUM (fun _ -> (pk.ReadInt64(), pk.ReadInt64()))
                            else [||]
                        let fullItem =
                            { GridId = gridId; ItemId = itemId; DbId = dbId; NeedLv = needLv; Num = num
                              Endure0 = e0; Endure1 = e1; Energy0 = en0; Energy1 = en1
                              ForgeLv = forgeLv; Valid = valid; Tradable = tradable; Expiration = expiration
                              IsBoat = isBoat; BoatWorldId = boatWorldId; ForgeParam = forgeParam; InstId = instId
                              HasInstAttr = hasInstAttr; InstAttr = instAttr }
                        items.Add(fullItem)
            { SynType = synType; Capacity = capacity; Items = items.ToArray() }

        /// Десериализация панели быстрого доступа (ChaShortcutInfo).
        let inline deserializeChaShortcutInfo (pk: IRPacket) : ChaShortcutInfo =
            { Entries = Array.init SHORT_CUT_NUM (fun _ -> { ShortcutEntry.Type = pk.ReadInt64(); GridId = pk.ReadInt64() }) }

        /// Десериализация потребностей квеста (условная по NeedType).
        let inline deserializeMisNeeds (pk: IRPacket) : MisNeedEntry[] =
            let count = int (pk.ReadInt64())
            Array.init count (fun _ ->
                let needType = pk.ReadInt64()
                if needType = MIS_NEED_ITEM || needType = MIS_NEED_KILL then
                    { NeedType = needType; Param1 = pk.ReadInt64(); Param2 = pk.ReadInt64()
                      Param3 = pk.ReadInt64(); Desp = "" }
                elif needType = MIS_NEED_DESP then
                    { NeedType = needType; Param1 = 0L; Param2 = 0L; Param3 = 0L; Desp = pk.ReadString() }
                else
                    { NeedType = needType; Param1 = 0L; Param2 = 0L; Param3 = 0L; Desp = "" })

        /// Десериализация наград квеста.
        let inline deserializeMisPrizes (pk: IRPacket) : MisPrizeEntry[] =
            let count = int (pk.ReadInt64())
            Array.init count (fun _ ->
                { Type = pk.ReadInt64(); Param1 = pk.ReadInt64(); Param2 = pk.ReadInt64() })

        // ─── Фаза 3: MC — сложные ─────────────────────────────────

        let mcSynAttributeMessage (pk: IRPacket) : McSynAttributeMessage =
            { WorldId = pk.ReadInt64(); Attr = deserializeChaAttrInfo pk }

        let mcSynSkillStateMessage (pk: IRPacket) : McSynSkillStateMessage =
            { WorldId = pk.ReadInt64(); SkillState = deserializeChaSkillStateInfo pk }

        let mcAStateEndSeeMessage (pk: IRPacket) : McAStateEndSeeMessage =
            { AreaX = pk.ReadInt64(); AreaY = pk.ReadInt64() }

        let mcAStateBeginSeeMessage (pk: IRPacket) : McAStateBeginSeeMessage =
            let areaX = pk.ReadInt64()
            let areaY = pk.ReadInt64()
            let count = int (pk.ReadInt64())
            let states = Array.init count (fun _ ->
                let stateId = pk.ReadInt64()
                if stateId <> 0L then
                    { StateId = stateId; StateLv = pk.ReadInt64(); WorldId = pk.ReadInt64(); FightId = pk.ReadInt64() }
                else
                    { StateId = 0L; StateLv = 0L; WorldId = 0L; FightId = 0L })
            { AreaX = areaX; AreaY = areaY; States = states }

        let mcDelItemChaMessage (pk: IRPacket) : McDelItemChaMessage =
            { MainChaId = pk.ReadInt64(); WorldId = pk.ReadInt64() }

        let mcSynEventInfoMessage (pk: IRPacket) : McSynEventInfoMessage =
            { EntityId = pk.ReadInt64(); EntityType = pk.ReadInt64()
              EventId = pk.ReadInt64(); EventName = pk.ReadString() }

        let mcSynSideInfoMessage (pk: IRPacket) : McSynSideInfoMessage =
            { WorldId = pk.ReadInt64(); Side = { SideId = pk.ReadInt64() } }

        let mcItemCreateMessage (pk: IRPacket) : McItemCreateMessage =
            let worldId = pk.ReadInt64()
            let handle = pk.ReadInt64()
            let itemId = pk.ReadInt64()
            let posX = pk.ReadInt64()
            let posY = pk.ReadInt64()
            let angle = pk.ReadInt64()
            let num = pk.ReadInt64()
            let appeType = pk.ReadInt64()
            let fromId = pk.ReadInt64()
            let event = deserializeChaEventInfo pk
            { WorldId = worldId; Handle = handle; ItemId = itemId; PosX = posX; PosY = posY
              Angle = angle; Num = num; AppeType = appeType; FromId = fromId; Event = event }

        let mcSynSkillBagMessage (pk: IRPacket) : McSynSkillBagMessage =
            let worldId = pk.ReadInt64()
            let skillBag = deserializeChaSkillBagInfo pk
            { WorldId = worldId; SkillBag = skillBag }

        let mcMissionInfoMessage (pk: IRPacket) : McMissionInfoMessage =
            let npcId = pk.ReadInt64()
            let listType = pk.ReadInt64()
            let prev = pk.ReadInt64()
            let next = pk.ReadInt64()
            let prevCmd = pk.ReadInt64()
            let nextCmd = pk.ReadInt64()
            let count = int (pk.ReadInt64())
            { NpcId = npcId; ListType = listType; Prev = prev; Next = next; PrevCmd = prevCmd; NextCmd = nextCmd
              Items = Array.init count (fun _ -> pk.ReadString()) }

        let mcMisPageMessage (pk: IRPacket) : McMisPageMessage =
            let cmd = pk.ReadInt64()
            let npcId = pk.ReadInt64()
            let name = pk.ReadString()
            let needs = deserializeMisNeeds pk
            let prizeSelType = pk.ReadInt64()
            let prizes = deserializeMisPrizes pk
            let description = pk.ReadString()
            { Cmd = cmd; NpcId = npcId; Name = name; Needs = needs
              PrizeSelType = prizeSelType; Prizes = prizes; Description = description }

        let mcMisLogMessage (pk: IRPacket) : McMisLogMessage =
            let count = int (pk.ReadInt64())
            { Logs = Array.init count (fun _ ->
                { MisId = pk.ReadInt64(); State = pk.ReadInt64() }) }

        let mcMisLogInfoMessage (pk: IRPacket) : McMisLogInfoMessage =
            let misId = pk.ReadInt64()
            let name = pk.ReadString()
            let needs = deserializeMisNeeds pk
            let prizeSelType = pk.ReadInt64()
            let prizes = deserializeMisPrizes pk
            let description = pk.ReadString()
            { MisId = misId; Name = name; Needs = needs
              PrizeSelType = prizeSelType; Prizes = prizes; Description = description }

        let mcMisLogClearMessage (pk: IRPacket) : McMisLogClearMessage =
            { MissionId = pk.ReadInt64() }

        let mcMisLogAddMessage (pk: IRPacket) : McMisLogAddMessage =
            { MissionId = pk.ReadInt64(); State = pk.ReadInt64() }

        let mcMisLogStateMessage (pk: IRPacket) : McMisLogStateMessage =
            { MissionId = pk.ReadInt64(); State = pk.ReadInt64() }

        let mcFuncInfoMessage (pk: IRPacket) : McFuncInfoMessage =
            let npcId = pk.ReadInt64()
            let page = pk.ReadInt64()
            let talkText = pk.ReadString()
            let funcCount = int (pk.ReadInt64())
            let funcItems = Array.init funcCount (fun _ -> { FuncInfoFuncItem.Name = pk.ReadString() })
            let misCount = int (pk.ReadInt64())
            let misItems = Array.init misCount (fun _ -> { FuncInfoMissionItem.Name = pk.ReadString(); State = pk.ReadInt64() })
            { NpcId = npcId; Page = page; TalkText = talkText; FuncItems = funcItems; MissionItems = misItems }

        let mcVolunteerListMessage (pk: IRPacket) : McVolunteerListMessage =
            let pageTotal = pk.ReadInt64()
            let page = pk.ReadInt64()
            let count = int (pk.ReadInt64())
            { PageTotal = pageTotal; Page = page
              Volunteers = Array.init count (fun _ ->
                { Name = pk.ReadString(); Level = pk.ReadInt64(); Job = pk.ReadInt64(); Map = pk.ReadString() }) }

        let mcVolunteerOpenMessage (pk: IRPacket) : McVolunteerOpenMessage =
            let state = pk.ReadInt64()
            let pageTotal = pk.ReadInt64()
            let count = int (pk.ReadInt64())
            { State = state; PageTotal = pageTotal
              Volunteers = Array.init count (fun _ ->
                { Name = pk.ReadString(); Level = pk.ReadInt64(); Job = pk.ReadInt64(); Map = pk.ReadString() }) }

        let mcShowStallSearchMessage (pk: IRPacket) : McShowStallSearchMessage =
            let count = int (pk.ReadInt64())
            { Entries = Array.init count (fun _ ->
                { Name = pk.ReadString(); StallName = pk.ReadString(); Location = pk.ReadString()
                  Count = pk.ReadInt64(); Cost = pk.ReadInt64() }) }

        let mcShowRankingMessage (pk: IRPacket) : McShowRankingMessage =
            let count = int (pk.ReadInt64())
            { Entries = Array.init count (fun _ ->
                { Name = pk.ReadString(); Guild = pk.ReadString()
                  Level = pk.ReadInt64(); Job = pk.ReadInt64(); Score = pk.ReadInt64() }) }

        let mcEspeItemMessage (pk: IRPacket) : McEspeItemMessage =
            let worldId = pk.ReadInt64()
            let count = int (pk.ReadInt64())
            { WorldId = worldId; Items = Array.init count (fun _ ->
                { Position = pk.ReadInt64(); Endure = pk.ReadInt64(); Energy = pk.ReadInt64()
                  Tradable = pk.ReadInt64(); Expiration = pk.ReadInt64() }) }

        let mcBlackMarketExchangeDataMessage (pk: IRPacket) : McBlackMarketExchangeDataMessage =
            let npcId = pk.ReadInt64()
            let count = int (pk.ReadInt64())
            { NpcId = npcId; Exchanges = Array.init count (fun _ ->
                { SrcId = pk.ReadInt64(); SrcCount = pk.ReadInt64()
                  TarId = pk.ReadInt64(); TarCount = pk.ReadInt64(); TimeValue = pk.ReadInt64() }) }

        let mcExchangeDataMessage (pk: IRPacket) : McExchangeDataMessage =
            let npcId = pk.ReadInt64()
            let count = int (pk.ReadInt64())
            { NpcId = npcId; Exchanges = Array.init count (fun _ ->
                { SrcId = pk.ReadInt64(); SrcCount = pk.ReadInt64()
                  TarId = pk.ReadInt64(); TarCount = pk.ReadInt64() }) }

        let mcBlackMarketExchangeUpdateMessage (pk: IRPacket) : McBlackMarketExchangeUpdateMessage =
            let npcId = pk.ReadInt64()
            let count = int (pk.ReadInt64())
            { NpcId = npcId; Exchanges = Array.init count (fun _ ->
                { SrcId = pk.ReadInt64(); SrcCount = pk.ReadInt64()
                  TarId = pk.ReadInt64(); TarCount = pk.ReadInt64(); TimeValue = pk.ReadInt64() }) }

        let mcBlackMarketExchangeAsrMessage (pk: IRPacket) : McBlackMarketExchangeAsrMessage =
            { Success = pk.ReadInt64(); SrcId = pk.ReadInt64(); SrcCount = pk.ReadInt64()
              TarId = pk.ReadInt64(); TarCount = pk.ReadInt64() }

        // ─── MC — торговля NPC (полные данные) ───────────────────

        let mcTradeAllDataMessage (pk: IRPacket) : McTradeAllDataMessage =
            let npcId = pk.ReadInt64()
            let tradeType = pk.ReadInt64()
            let param = pk.ReadInt64()
            let pageCount = int (pk.ReadInt64())
            let pages = Array.init pageCount (fun _ ->
                let itemType = pk.ReadInt64()
                let itemCount = int (pk.ReadInt64())
                let itemIds = Array.init itemCount (fun _ -> pk.ReadInt64())
                let items =
                    if tradeType = 1L then
                        // TRADE_GOODS — доп. поля count/price/level для каждого предмета
                        Array.init itemCount (fun i ->
                            let c = pk.ReadInt64()
                            let p = pk.ReadInt64()
                            let l = pk.ReadInt64()
                            { ItemId = itemIds[i]; Count = c; Price = p; Level = l })
                    else
                        Array.init itemCount (fun i ->
                            { ItemId = itemIds[i]; Count = 0L; Price = 0L; Level = 0L })
                { ItemType = itemType; Items = items })
            { NpcId = npcId; TradeType = tradeType; Param = param; Pages = pages }

        // ─── MC — магазин (Store) ────────────────────────────────

        let mcStoreOpenAnswerMessage (pk: IRPacket) : McStoreOpenAnswerMessage =
            let isValid = pk.ReadInt64() <> 0L
            if isValid then
                let vip = pk.ReadInt64()
                let moBean = pk.ReadInt64()
                let replMoney = pk.ReadInt64()
                let afficheCount = int (pk.ReadInt64())
                let affiches = Array.init afficheCount (fun _ ->
                    { AfficheId = pk.ReadInt64(); Title = pk.ReadString(); ComId = pk.ReadString() })
                let classCount = int (pk.ReadInt64())
                let classes = Array.init classCount (fun _ ->
                    { ClassId = pk.ReadInt64(); ClassName = pk.ReadString(); ParentId = pk.ReadInt64() })
                { IsValid = true; Vip = vip; MoBean = moBean; ReplMoney = replMoney
                  Affiches = affiches; Classes = classes }
            else
                { IsValid = false; Vip = 0L; MoBean = 0L; ReplMoney = 0L
                  Affiches = [||]; Classes = [||] }

        let mcStoreListAnswerMessage (pk: IRPacket) : McStoreListAnswerMessage =
            let pageTotal = pk.ReadInt64()
            let pageCurrent = pk.ReadInt64()
            let productCount = int (pk.ReadInt64())
            let products = Array.init productCount (fun _ ->
                let comId = pk.ReadInt64()
                let comName = pk.ReadString()
                let price = pk.ReadInt64()
                let remark = pk.ReadString()
                let isHot = pk.ReadInt64() <> 0L
                let time = pk.ReadInt64()
                let quantity = pk.ReadInt64()
                let expire = pk.ReadInt64()
                let variantCount = int (pk.ReadInt64())
                // Атрибуты читаются внутри каждого варианта (по 5 пар на вариант)
                let variants = Array.init variantCount (fun _ ->
                    let itemId = pk.ReadInt64()
                    let itemNum = pk.ReadInt64()
                    let flute = pk.ReadInt64()
                    let attrs = Array.init 5 (fun _ ->
                        { AttrEntry.AttrId = pk.ReadInt64(); AttrVal = pk.ReadInt64() })
                    { ItemId = itemId; ItemNum = itemNum; Flute = flute; Attrs = attrs })
                { ComId = comId; ComName = comName; Price = price; Remark = remark
                  IsHot = isHot; Time = time; Quantity = quantity; Expire = expire
                  Variants = variants })
            { PageTotal = pageTotal; PageCurrent = pageCurrent; Products = products }

        let mcStoreHistoryMessage (pk: IRPacket) : McStoreHistoryMessage =
            let recordCount = int (pk.ReadInt64())
            { Records = Array.init recordCount (fun _ ->
                { Time = pk.ReadString(); ItemId = pk.ReadInt64()
                  Name = pk.ReadString(); Money = pk.ReadInt64() }) }

        let mcStoreVipMessage (pk: IRPacket) : McStoreVipMessage =
            let success = pk.ReadInt64()
            if success <> 0L then
                { Success = success; VipId = pk.ReadInt64(); Months = pk.ReadInt64()
                  Shuijing = pk.ReadInt64(); Modou = pk.ReadInt64() }
            else
                { Success = 0L; VipId = 0L; Months = 0L; Shuijing = 0L; Modou = 0L }

        // ─── MC — синхронизация команды (пати) ──────────────────

        let mcSynTeamMessage (pk: IRPacket) : McSynTeamMessage =
            let memberId = pk.ReadInt64()
            let hp = pk.ReadInt64()
            let maxHP = pk.ReadInt64()
            let sp = pk.ReadInt64()
            let maxSP = pk.ReadInt64()
            let level = pk.ReadInt64()
            let look = deserializeChaLookInfo pk
            { MemberId = memberId; HP = hp; MaxHP = maxHP; SP = sp; MaxSP = maxSP
              Level = level; Look = look }

        // ─── McChaBeginSee / McAddItemCha ──────────────────────────

        let mcChaBeginSeeMessage (pk: IRPacket) : McChaBeginSeeMessage =
            let seeType = pk.ReadInt64()
            let baseInfo = deserializeChaBaseInfo pk
            let npcType = pk.ReadInt64()
            let npcState = pk.ReadInt64()
            let poseType = pk.ReadInt64()
            let lean, seat =
                match poseType with
                | 1L -> // PoseLean
                    ValueSome { LeanState = pk.ReadInt64(); Pose = pk.ReadInt64(); Angle = pk.ReadInt64()
                                PosX = pk.ReadInt64(); PosY = pk.ReadInt64(); Height = pk.ReadInt64() }, ValueNone
                | 2L -> // PoseSeat
                    ValueNone, ValueSome { SeatAngle = pk.ReadInt64(); SeatPose = pk.ReadInt64() }
                | _ -> ValueNone, ValueNone
            let attr = deserializeChaAttrInfo pk
            let skillState = deserializeChaSkillStateInfo pk
            { SeeType = seeType; Base = baseInfo; NpcType = npcType; NpcState = npcState
              PoseType = poseType; Lean = lean; Seat = seat; Attr = attr; SkillState = skillState }

        let mcAddItemChaMessage (pk: IRPacket) : McAddItemChaMessage =
            let mainChaId = pk.ReadInt64()
            let baseInfo = deserializeChaBaseInfo pk
            let attr = deserializeChaAttrInfo pk
            let kitbag = deserializeChaKitbagInfo pk
            let skillState = deserializeChaSkillStateInfo pk
            { MainChaId = mainChaId; Base = baseInfo; Attr = attr; Kitbag = kitbag; SkillState = skillState }

        // ─── McCharacterAction (CMD_MC_NOTIACTION) ──────────────

        /// Десериализация данных действия MOVE.
        let inline private deserializeActionMove (pk: IRPacket) : ActionMoveData =
            let moveState = pk.ReadInt64()
            let stopState = if moveState <> enumMSTATE_ON then pk.ReadInt64() else 0L
            let wp = pk.ReadSequence()
            { MoveState = moveState; StopState = stopState; Waypoints = wp.ToArray() }

        /// Десериализация данных действия SKILL_SRC.
        let inline private deserializeActionSkillSrc (pk: IRPacket) : ActionSkillSrcData =
            let fightId = pk.ReadInt64()
            let angle = pk.ReadInt64()
            let state = pk.ReadInt64()
            let stopState = if state <> enumFSTATE_ON then pk.ReadInt64() else 0L
            let skillId = pk.ReadInt64()
            let skillSpeed = pk.ReadInt64()
            let targetType = pk.ReadInt64()
            let targetId, targetX, targetY =
                if targetType = 1L then pk.ReadInt64(), pk.ReadInt64(), pk.ReadInt64()
                elif targetType = 2L then 0L, pk.ReadInt64(), pk.ReadInt64()
                else 0L, 0L, 0L
            let execTime = pk.ReadInt64()
            let effectCount = int (pk.ReadInt64())
            let effects = Array.init effectCount (fun _ -> { AttrId = pk.ReadInt64(); AttrVal = pk.ReadInt64() })
            let stateCount = int (pk.ReadInt64())
            let states = Array.init stateCount (fun _ -> { AStateId = pk.ReadInt64(); AStateLv = pk.ReadInt64() })
            { FightId = fightId; Angle = angle; State = state; StopState = stopState
              SkillId = skillId; SkillSpeed = skillSpeed
              TargetType = targetType; TargetId = targetId; TargetX = targetX; TargetY = targetY
              ExecTime = execTime; Effects = effects; States = states }

        /// Десериализация данных действия SKILL_TAR.
        let inline private deserializeActionSkillTar (pk: IRPacket) : ActionSkillTarData =
            let fightId = pk.ReadInt64()
            let state = pk.ReadInt64()
            let doubleAttack = pk.ReadInt64() <> 0L
            let miss = pk.ReadInt64() <> 0L
            let beatBack = pk.ReadInt64() <> 0L
            let beatBackX, beatBackY =
                if beatBack then pk.ReadInt64(), pk.ReadInt64() else 0L, 0L
            let srcId = pk.ReadInt64()
            let srcPosX = pk.ReadInt64()
            let srcPosY = pk.ReadInt64()
            let skillId = pk.ReadInt64()
            let skillTargetX = pk.ReadInt64()
            let skillTargetY = pk.ReadInt64()
            let execTime = pk.ReadInt64()
            // Эффекты цели: synType + count + entries
            let synType = pk.ReadInt64()
            let effectCount = int (pk.ReadInt64())
            let effects = Array.init effectCount (fun _ -> { AttrId = pk.ReadInt64(); AttrVal = pk.ReadInt64() })
            // Состояния цели
            let hasStates = pk.ReadInt64() = 1L
            let stateTime, states =
                if hasStates then
                    let st = pk.ReadInt64() // currentTime
                    let cnt = int (pk.ReadInt64())
                    let arr = Array.init cnt (fun _ ->
                        { TarStateId = pk.ReadInt64(); TarStateLv = pk.ReadInt64()
                          TarDuration = pk.ReadInt64(); TarStartTime = pk.ReadInt64() })
                    st, arr
                else
                    0L, [||]
            // Эффекты источника
            let hasSrcEffect = pk.ReadInt64() <> 0L
            let srcState, srcSynType, srcEffects =
                if hasSrcEffect then
                    let ss = pk.ReadInt64()
                    let sst = pk.ReadInt64()
                    let cnt = int (pk.ReadInt64())
                    let arr = Array.init cnt (fun _ -> { AttrId = pk.ReadInt64(); AttrVal = pk.ReadInt64() })
                    ss, sst, arr
                else
                    0L, 0L, [||]
            { FightId = fightId; State = state; DoubleAttack = doubleAttack; Miss = miss
              BeatBack = beatBack; BeatBackX = beatBackX; BeatBackY = beatBackY
              SrcId = srcId; SrcPosX = srcPosX; SrcPosY = srcPosY
              SkillId = skillId; SkillTargetX = skillTargetX; SkillTargetY = skillTargetY
              ExecTime = execTime; SynType = synType; Effects = effects
              HasStates = hasStates; StateTime = stateTime; States = states
              HasSrcEffect = hasSrcEffect; SrcState = srcState; SrcSynType = srcSynType; SrcEffects = srcEffects }

        /// Десериализация CMD_MC_NOTIACTION — диспетчер по ActionType.
        let mcCharacterActionMessage (pk: IRPacket) : McCharacterActionMessage =
            let worldId = pk.ReadInt64()
            let packetId = pk.ReadInt64()
            let actionType = pk.ReadInt64()
            let action =
                match actionType with
                | ACT_MOVE ->
                    ActionMove (deserializeActionMove pk)
                | ACT_SKILL_SRC ->
                    ActionSkillSrc (deserializeActionSkillSrc pk)
                | ACT_SKILL_TAR ->
                    ActionSkillTar (deserializeActionSkillTar pk)
                | ACT_LEAN ->
                    let leanState = pk.ReadInt64()
                    if leanState = 0L then
                        ActionLean { ActionLeanState = 0L; ActionPose = pk.ReadInt64(); ActionAngle = pk.ReadInt64()
                                     ActionPosX = pk.ReadInt64(); ActionPosY = pk.ReadInt64(); ActionHeight = pk.ReadInt64() }
                    else
                        ActionLean { ActionLeanState = leanState; ActionPose = 0L; ActionAngle = 0L
                                     ActionPosX = 0L; ActionPosY = 0L; ActionHeight = 0L }
                | ACT_FACE ->
                    ActionFace { FaceAngle = pk.ReadInt64(); FacePose = pk.ReadInt64() }
                | ACT_SKILL_POSE ->
                    ActionSkillPose { FaceAngle = pk.ReadInt64(); FacePose = pk.ReadInt64() }
                | ACT_ITEM_FAILED ->
                    ActionItemFailed (pk.ReadInt64())
                | ACT_TEMP ->
                    ActionTemp (pk.ReadInt64(), pk.ReadInt64())
                | ACT_CHANGE_CHA ->
                    ActionChangeCha (pk.ReadInt64())
                | ACT_LOOK ->
                    ActionLook (deserializeChaLookInfo pk)
                | ACT_KITBAG ->
                    ActionKitbag (deserializeChaKitbagInfo pk)
                | ACT_BANK ->
                    ActionBank (deserializeChaKitbagInfo pk)
                | ACT_GUILDBANK ->
                    ActionGuildBank (deserializeChaKitbagInfo pk)
                | ACT_KITBAGTMP ->
                    ActionKitbagTmp (deserializeChaKitbagInfo pk)
                | ACT_SHORTCUT ->
                    ActionShortcut (deserializeChaShortcutInfo pk)
                | _ ->
                    ActionUnknown actionType
            { WorldId = worldId; PacketId = packetId; Action = action }

        // ─── CMD_MC_NOTIACTION UPDATEGUILDLOGS/REQUESTGUILDLOGS ──

        /// Десериализация CMD_MC_NOTIACTION + UPDATEGUILDLOGS.
        /// Формат: worldId, packetId, actionType(=33), totalSize, {type,time,param,qty,userId}*, [9].
        let mcUpdateGuildLogsMessage (pk: IRPacket) : McUpdateGuildLogsMessage =
            let worldId = pk.ReadInt64()
            let packetId = pk.ReadInt64()
            let _actionType = pk.ReadInt64() // ACT_UPDATEGUILDLOGS = 33
            let totalSize = pk.ReadInt64()
            let logs = System.Collections.Generic.List<GuildBankLogEntry>()
            let mutable terminated = false
            while (pk.RemainingBytes > 0) do
                let t = pk.ReadInt64()
                if t = 9L && not ((pk.RemainingBytes > 0)) then
                    terminated <- true
                else
                    let time = pk.ReadInt64()
                    let param = pk.ReadInt64()
                    let qty = pk.ReadInt64()
                    let uid = pk.ReadInt64()
                    logs.Add({ Type = t; Time = time; Parameter = param; Quantity = qty; UserId = uid })
            { WorldId = worldId; PacketId = packetId; TotalSize = totalSize
              Logs = logs.ToArray(); Terminated = terminated }

        /// Десериализация CMD_MC_NOTIACTION + REQUESTGUILDLOGS.
        /// Формат: worldId, packetId, actionType(=32), {type,time,param,qty,userId}*, [9].
        let mcRequestGuildLogsMessage (pk: IRPacket) : McRequestGuildLogsMessage =
            let worldId = pk.ReadInt64()
            let packetId = pk.ReadInt64()
            let _actionType = pk.ReadInt64() // ACT_REQUESTGUILDLOGS = 32
            let logs = System.Collections.Generic.List<GuildBankLogEntry>()
            let mutable terminated = false
            while (pk.RemainingBytes > 0) do
                let t = pk.ReadInt64()
                if t = 9L && not ((pk.RemainingBytes > 0)) then
                    terminated <- true
                else
                    let time = pk.ReadInt64()
                    let param = pk.ReadInt64()
                    let qty = pk.ReadInt64()
                    let uid = pk.ReadInt64()
                    logs.Add({ Type = t; Time = time; Parameter = param; Quantity = qty; UserId = uid })
            { WorldId = worldId; PacketId = packetId
              Logs = logs.ToArray(); Terminated = terminated }

        // ─── CMD_MC_CHARTRADE + CMD_MC_CHARTRADE_ITEM ───────────

        /// Десериализация CMD_MC_CHARTRADE + CMD_MC_CHARTRADE_ITEM (единое сообщение).
        /// Формат: subCmd(=814), mainChaId, opType, далее — данные Remove или Add в зависимости от opType.
        let mcCharTradeItemMessage (pk: IRPacket) : McCharTradeItemMessage =
            let _subCmd = pk.ReadInt64() // CMD_MC_CHARTRADE_ITEM = 814
            let mainChaId = pk.ReadInt64()
            let opType = pk.ReadInt64()
            if opType = 3L then // TRADE_DRAGTO_ITEM
                let bagIndex = pk.ReadInt64()
                let tradeIndex = pk.ReadInt64()
                let count = pk.ReadInt64()
                { MainChaId = mainChaId; OpType = opType
                  Data = Remove { BagIndex = bagIndex; TradeIndex = tradeIndex; Count = count } }
            else // TRADE_DRAGTO_TRADE
                let itemId = pk.ReadInt64()
                let bagIndex = pk.ReadInt64()
                let tradeIndex = pk.ReadInt64()
                let count = pk.ReadInt64()
                let itemType = pk.ReadInt64()
                let isBoat = (itemType = ITEM_TYPE_BOAT)
                if isBoat then
                    let hasBoat = pk.ReadInt64() <> 0L
                    let boat =
                        if hasBoat then
                            let name = pk.ReadString()
                            let ship = pk.ReadInt64()
                            let lv = pk.ReadInt64()
                            let cexp = pk.ReadInt64()
                            let hp = pk.ReadInt64()
                            let mxhp = pk.ReadInt64()
                            let sp = pk.ReadInt64()
                            let mxsp = pk.ReadInt64()
                            let mnatk = pk.ReadInt64()
                            let mxatk = pk.ReadInt64()
                            let def = pk.ReadInt64()
                            let mspd = pk.ReadInt64()
                            let aspd = pk.ReadInt64()
                            let useGridNum = pk.ReadInt64()
                            let capacity = pk.ReadInt64()
                            let price = pk.ReadInt64()
                            { HasBoat = true; Name = name; Ship = ship; Lv = lv; Cexp = cexp
                              HP = hp; MxHP = mxhp; SP = sp; MxSP = mxsp
                              MnAtk = mnatk; MxAtk = mxatk; Def = def
                              MSpd = mspd; ASpd = aspd
                              UseGridNum = useGridNum; Capacity = capacity; Price = price }
                        else
                            { HasBoat = false; Name = ""; Ship = 0L; Lv = 0L; Cexp = 0L
                              HP = 0L; MxHP = 0L; SP = 0L; MxSP = 0L
                              MnAtk = 0L; MxAtk = 0L; Def = 0L; MSpd = 0L; ASpd = 0L
                              UseGridNum = 0L; Capacity = 0L; Price = 0L }
                    { MainChaId = mainChaId; OpType = opType
                      Data = Add { ItemId = itemId; BagIndex = bagIndex
                                   TradeIndex = tradeIndex; Count = count; ItemType = itemType
                                   EquipData = Choice1Of2 boat } }
                else
                    let endure0 = pk.ReadInt64()
                    let endure1 = pk.ReadInt64()
                    let energy0 = pk.ReadInt64()
                    let energy1 = pk.ReadInt64()
                    let forgeLv = pk.ReadInt64()
                    let valid = pk.ReadInt64()
                    let tradable = pk.ReadInt64()
                    let expiration = pk.ReadInt64()
                    let forgeParam = pk.ReadInt64()
                    let instId = pk.ReadInt64()
                    let hasInstAttr = pk.ReadInt64() <> 0L
                    let instAttr =
                        if hasInstAttr then
                            Array.init ITEM_INSTANCE_ATTR_NUM (fun _ -> (pk.ReadInt64(), pk.ReadInt64()))
                        else
                            [||]
                    let item =
                        { Endure0 = endure0; Endure1 = endure1; Energy0 = energy0; Energy1 = energy1
                          ForgeLv = forgeLv; Valid = valid; Tradable = tradable; Expiration = expiration
                          ForgeParam = forgeParam; InstId = instId
                          HasInstAttr = hasInstAttr; InstAttr = instAttr }
                    { MainChaId = mainChaId; OpType = opType
                      Data = Add { ItemId = itemId; BagIndex = bagIndex
                                   TradeIndex = tradeIndex; Count = count; ItemType = itemType
                                   EquipData = Choice2Of2 item } }
