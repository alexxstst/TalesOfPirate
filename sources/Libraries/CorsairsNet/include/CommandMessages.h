#pragma once

// CommandMessages.h — Типизированные структуры команд + функции serialize/deserialize.
// Порт из F# (Corsairs.Platform.Network.CommandMessages).
// Header-only. Все поля сериализуются через WriteInt64/WriteString/ReadInt64/ReadString.

#include "Packet.h"
#include "../../common/include/NetCommand.h"
#include <string>
#include <vector>
#include <optional>
#include <cstdint>

namespace net {
	namespace msg {
		// =================================================================
		//  Вспомогательные типы
		// =================================================================

		/// Количество слотов экипировки (C++: enumEQUIP_NUM).
		constexpr int EQUIP_NUM = 34;
		constexpr int SHORT_CUT_NUM = 36; // MAX_FAST_ROW(3) * MAX_FAST_COL(12)
		constexpr int ESPE_KBGRID_NUM = 4; // defESPE_KBGRID_NUM
		constexpr int ITEM_INSTANCE_ATTR_NUM = 5; // defITEM_INSTANCE_ATTR_NUM
		constexpr int SKILL_RANGE_PARAM_NUM = 4; // defSKILL_RANGE_PARAM_NUM

		/// Значения из CompCommand.h (дублируем для независимости заголовков).
		constexpr int64_t SYN_LOOK_CHANGE = 1; // enumSYN_LOOK_CHANGE
		constexpr int64_t SYN_KITBAG_INIT = 0; // enumSYN_KITBAG_INIT
		constexpr int64_t RANGE_TYPE_NONE = 0; // enumRANGE_TYPE_NONE

		/// Слот персонажа на экране выбора.
		struct ChaSlotData {
			bool valid;
			std::string chaName;
			std::string job;
			int64_t degree;
			int64_t typeId;
			std::vector<int64_t> equipIds; // Look_Minimal.equip_IDs[34]
		};

		/// Запись в списке игроков (TpReqPlyLst).
		struct ReqPlyEntry {
			int64_t gtAddr;
			int64_t chaId;
		};

		/// Данные синхронизации игрока (TpSyncPlyLst).
		struct SyncPlayerData {
			int64_t gtAddr;
			int64_t acctLoginId;
			int64_t acctId;
		};

		/// Результат синхронизации одного игрока.
		struct SyncResultData {
			bool ok;
			int64_t playerPtr;
		};

		/// Участник команды (PcTeamRefresh).
		struct TeamMemberData {
			int64_t chaId;
			std::string chaName;
			std::string motto;
			int64_t icon;
		};

		/// Участник сессии чата (PcSessCreate).
		struct SessMemberData {
			int64_t chaId;
			std::string chaName;
			std::string motto;
			int64_t icon;
		};

		/// Участник команды (PmTeam -> GameServer).
		struct PmTeamMemberData {
			std::string gateName;
			int64_t gtAddr;
			int64_t chaId;
		};

		// =================================================================
		//  Группа A: GateServer <-> GroupServer (TP/PT) — RPC
		// =================================================================

		struct TpLoginRequest {
			int16_t protocolVersion;
			std::string gateName;
		};

		struct TpLoginResponse {
			int16_t errCode;
		};

		struct TpUserLoginRequest {
			std::string acctName;
			std::string acctPassword;
			std::string acctMac;
			uint32_t clientIp;
			uint32_t gateAddr;
			int64_t cheatMarker;
		};

		struct TpUserLoginResponseData {
			int64_t maxChaNum;
			std::vector<ChaSlotData> characters;
			bool hasPassword2;
			int64_t acctId;
			int64_t acctLoginId;
			uint32_t gpAddr;
		};

		struct TpUserLoginResponse {
			int16_t errCode;
			std::optional<TpUserLoginResponseData> data;
		};

		struct TpUserLogoutRequest {
			uint32_t gateAddr;
			uint32_t gpAddr;
		};

		struct TpUserLogoutResponse {
			int16_t errCode;
		};

		struct TpBgnPlayRequest {
			int64_t chaIndex;
			uint32_t gateAddr;
			uint32_t gpAddr;
		};

		struct TpBgnPlayResponseData {
			std::string password2;
			int64_t chaId;
			int64_t worldId;
			std::string mapName;
			int64_t swiner;
		};

		struct TpBgnPlayResponse {
			int16_t errCode;
			std::optional<TpBgnPlayResponseData> data;
		};

		struct TpEndPlayRequest {
			uint32_t gateAddr;
			uint32_t gpAddr;
		};

		struct TpEndPlayResponseData {
			int64_t maxChaNum;
			int64_t chaNum;
			std::vector<ChaSlotData> characters;
		};

		struct TpEndPlayResponse {
			int16_t errCode;
			std::optional<TpEndPlayResponseData> data;
		};

		struct TpNewChaRequest {
			std::string chaName;
			std::string birth;
			int64_t typeId;
			int64_t hairId;
			int64_t faceId;
			uint32_t gateAddr;
			uint32_t gpAddr;
		};

		struct TpNewChaResponse {
			int16_t errCode;
		};

		struct TpDelChaRequest {
			int64_t chaIndex;
			std::string password2;
			uint32_t gateAddr;
			uint32_t gpAddr;
		};

		struct TpDelChaResponse {
			int16_t errCode;
		};

		struct TpChangePassRequest {
			std::string newPass;
			std::string pin;
			uint32_t gateAddr;
			uint32_t gpAddr;
		};

		struct TpRegisterRequest {
			std::string username;
			std::string password;
			std::string email;
		};

		struct TpRegisterResponse {
			int64_t result;
			std::string message;
		};

		struct TpCreatePassword2Request {
			std::string password2;
			uint32_t gateAddr;
			uint32_t gpAddr;
		};

		struct TpUpdatePassword2Request {
			std::string oldPassword2;
			std::string newPassword2;
			uint32_t gateAddr;
			uint32_t gpAddr;
		};

		struct TpPassword2Response {
			int16_t errCode;
		};

		struct TpReqPlyLstResponse {
			std::vector<ReqPlyEntry> entries;
			int64_t plyNum;
		};

		struct TpSyncPlyLstRequest {
			int64_t num;
			std::string gateName;
			std::vector<SyncPlayerData> players;
		};

		struct TpSyncPlyLstResponse {
			int16_t errCode;
			int64_t num;
			std::vector<SyncResultData> results;
		};

		struct OsLoginRequest {
			int64_t version;
			std::string agentName;
		};

		struct OsLoginResponse {
			int16_t errCode;
		};

		// Группа A2: Async TP
		struct TpDiscMessage {
			int64_t actId;
			int64_t ipAddr;
			std::string reason;
		};

		// =================================================================
		//  Группа B: GroupServer <-> AccountServer (PA/AP)
		// =================================================================

		struct PaLoginRequest {
			std::string serverName;
			std::string serverPassword;
		};

		struct PaLoginResponse {
			int16_t errCode;
		};

		struct PaUserLoginRequest {
			std::string username;
			std::string password;
			std::string mac;
			int64_t clientIp;
		};

		struct PaUserLoginResponseData {
			int64_t acctLoginId;
			int64_t sessId;
			int64_t acctId;
			int64_t gmLevel;
		};

		struct PaUserLoginResponse {
			int16_t errCode;
			std::optional<PaUserLoginResponseData> data;
		};

		struct PaUserLogoutMessage {
			int64_t acctLoginId;
		};

		struct PaUserBillBgnMessage {
			std::string acctName;
			std::string passport;
		};

		struct PaUserBillEndMessage {
			std::string acctName;
		};

		struct PaChangePassMessage {
			std::string username;
			std::string newPassword;
		};

		struct PaRegisterMessage {
			std::string username;
			std::string password;
			std::string email;
		};

		struct PaGmBanMessage {
			std::string actName;
		};

		struct PaGmUnbanMessage {
			std::string actName;
		};

		struct PaUserDisableMessage {
			int64_t acctLoginId;
			int64_t minutes;
		};

		struct ApKickUserMessage {
			int64_t errCode;
			int64_t accountId;
		};

		struct ApExpScaleMessage {
			int64_t chaId;
			int64_t time;
		};

		// =================================================================
		//  Группа C: Client -> GroupServer (CP) — async
		// =================================================================

		struct CpTeamInviteMessage {
			std::string invitedName;
			uint32_t gateAddr;
			uint32_t gpAddr;
		};

		struct CpTeamAcceptMessage {
			int64_t inviterChaId;
			uint32_t gateAddr;
			uint32_t gpAddr;
		};

		struct CpTeamRefuseMessage {
			int64_t inviterChaId;
			uint32_t gateAddr;
			uint32_t gpAddr;
		};

		struct CpTeamLeaveMessage {
			uint32_t gateAddr;
			uint32_t gpAddr;
		};

		struct CpTeamKickMessage {
			int64_t kickedChaId;
			uint32_t gateAddr;
			uint32_t gpAddr;
		};

		struct CpFrndInviteMessage {
			std::string invitedName;
			uint32_t gateAddr;
			uint32_t gpAddr;
		};

		struct CpFrndAcceptMessage {
			int64_t inviterChaId;
			uint32_t gateAddr;
			uint32_t gpAddr;
		};

		struct CpFrndRefuseMessage {
			int64_t inviterChaId;
			uint32_t gateAddr;
			uint32_t gpAddr;
		};

		struct CpFrndDeleteMessage {
			int64_t deletedChaId;
			uint32_t gateAddr;
			uint32_t gpAddr;
		};

		struct CpFrndChangeGroupMessage {
			int64_t friendChaId;
			std::string groupName;
			uint32_t gateAddr;
			uint32_t gpAddr;
		};

		struct CpFrndRefreshInfoMessage {
			int64_t friendChaId;
			uint32_t gateAddr;
			uint32_t gpAddr;
		};

		struct CpSay2AllMessage {
			std::string content;
			uint32_t gateAddr;
			uint32_t gpAddr;
		};

		struct CpSay2TradeMessage {
			std::string content;
			uint32_t gateAddr;
			uint32_t gpAddr;
		};

		struct CpSay2YouMessage {
			std::string targetName;
			std::string content;
			uint32_t gateAddr;
			uint32_t gpAddr;
		};

		struct CpSay2TemMessage {
			std::string content;
			uint32_t gateAddr;
			uint32_t gpAddr;
		};

		struct CpSay2GudMessage {
			std::string content;
			uint32_t gateAddr;
			uint32_t gpAddr;
		};

		struct CpGm1SayMessage {
			std::string content;
			uint32_t gateAddr;
			uint32_t gpAddr;
		};

		struct CpGm1Say1Message {
			std::string content;
			int64_t color;
			uint32_t gateAddr;
			uint32_t gpAddr;
		};

		struct CpSessCreateMessage {
			int64_t chaNum;
			std::vector<std::string> chaNames;
			uint32_t gateAddr;
			uint32_t gpAddr;
		};

		struct CpSessAddMessage {
			int64_t sessId;
			std::string chaName;
			uint32_t gateAddr;
			uint32_t gpAddr;
		};

		struct CpSessSayMessage {
			int64_t sessId;
			std::string content;
			uint32_t gateAddr;
			uint32_t gpAddr;
		};

		struct CpSessLeaveMessage {
			int64_t sessId;
			uint32_t gateAddr;
			uint32_t gpAddr;
		};

		struct CpChangePersonInfoMessage {
			std::string motto;
			int64_t icon;
			int64_t refuseSess;
			uint32_t gateAddr;
			uint32_t gpAddr;
		};

		struct CpPingMessage {
			int64_t pingValue;
			uint32_t gateAddr;
			uint32_t gpAddr;
		};

		struct CpRefuseToMeMessage {
			int64_t refuseFlag;
			uint32_t gateAddr;
			uint32_t gpAddr;
		};

		struct CpReportWgMessage {
			uint32_t gateAddr;
			uint32_t gpAddr;
		};

		struct CpMasterRefreshInfoMessage {
			int64_t masterChaId;
			uint32_t gateAddr;
			uint32_t gpAddr;
		};

		struct CpPrenticeRefreshInfoMessage {
			int64_t prenticeChaId;
			uint32_t gateAddr;
			uint32_t gpAddr;
		};

		struct CpChangePassMessage {
			std::string newPass;
			std::string pin;
			uint32_t gateAddr;
			uint32_t gpAddr;
		};

		// =================================================================
		//  Группа D: GameServer -> GroupServer (MP) — async
		// =================================================================

		struct MpEnterMapMessage {
			int64_t isSwitch;
			uint32_t gateAddr;
			uint32_t gpAddr;
		};

		struct MpTeamCreateMessage {
			std::string memberName;
			std::string leaderName;
			uint32_t gateAddr;
			uint32_t gpAddr;
		};

		struct MpGuildCreateMessage {
			int64_t guildId;
			std::string gldName;
			std::string job;
			int64_t degree;
			uint32_t gateAddr;
			uint32_t gpAddr;
		};

		struct MpGuildApproveMessage {
			int64_t newMemberChaId;
			uint32_t gateAddr;
			uint32_t gpAddr;
		};

		struct MpGuildKickMessage {
			int64_t kickedChaId;
			uint32_t gateAddr;
			uint32_t gpAddr;
		};

		struct MpGuildLeaveMessage {
			uint32_t gateAddr;
			uint32_t gpAddr;
		};

		struct MpGuildDisbandMessage {
			uint32_t gateAddr;
			uint32_t gpAddr;
		};

		struct MpGuildMottoMessage {
			std::string motto;
			uint32_t gateAddr;
			uint32_t gpAddr;
		};

		struct MpGuildPermMessage {
			int64_t targetChaId;
			int64_t permission;
			uint32_t gateAddr;
			uint32_t gpAddr;
		};

		struct MpGuildChallMoneyMessage {
			int64_t guildId;
			int64_t money;
			std::string guildName1;
			std::string guildName2;
			uint32_t gateAddr;
			uint32_t gpAddr;
		};

		struct MpGuildChallPrizeMoneyMessage {
			int64_t guildId;
			int64_t money;
		};

		struct MpMasterCreateMessage {
			std::string prenticeName;
			int64_t prenticeChaid;
			std::string masterName;
			int64_t masterChaid;
		};

		struct MpMasterDelMessage {
			std::string prenticeName;
			int64_t prenticeChaid;
			std::string masterName;
			int64_t masterChaid;
		};

		struct MpMasterFinishMessage {
			int64_t prenticeChaid;
		};

		struct MpSay2AllMessage {
			int64_t succ;
			std::string chaName;
			std::string content;
			uint32_t gateAddr;
			uint32_t gpAddr;
		};

		struct MpSay2TradeMessage {
			int64_t succ;
			std::string chaName;
			std::string content;
			uint32_t gateAddr;
			uint32_t gpAddr;
		};

		struct MpGm1SayMessage {
			std::string content;
			uint32_t gateAddr;
			uint32_t gpAddr;
		};

		struct MpGm1Say1Message {
			std::string content;
			int64_t setNum;
			int64_t color;
		};

		struct MpGmBanMessage {
			std::string actName;
			uint32_t gateAddr;
			uint32_t gpAddr;
		};

		struct MpGmUnbanMessage {
			std::string actName;
			uint32_t gateAddr;
			uint32_t gpAddr;
		};

		struct MpGuildNoticeMessage {
			int64_t guildId;
			std::string content;
		};

		struct MpCanReceiveRequestsMessage {
			int64_t chaId;
			int64_t canSend;
		};

		struct MpMutePlayerMessage {
			std::string chaName;
			int64_t time;
			uint32_t gateAddr;
			uint32_t gpAddr;
		};

		struct MpGarner2UpdateMessage {
			int64_t nid;
			std::string chaName;
			int64_t level;
			std::string job;
			int64_t fightpoint;
			uint32_t gateAddr;
			uint32_t gpAddr;
		};

		struct MpGarner2GetOrderMessage {
			uint32_t gateAddr;
			uint32_t gpAddr;
		};

		struct MpGuildBankAckMessage {
			int64_t guildId;
			uint32_t gateAddr;
			uint32_t gpAddr;
		};

		// =================================================================
		//  Группа E: GroupServer -> Client (PC) — async
		// =================================================================

		struct PcTeamInviteMessage {
			std::string inviterName;
			int64_t chaId;
			int64_t icon;
		};

		struct PcTeamRefreshMessage {
			int64_t msg;
			int64_t count;
			std::vector<TeamMemberData> members;
		};

		struct PcFrndInviteMessage {
			std::string inviterName;
			int64_t chaId;
			int64_t icon;
		};

		struct PcFrndRefreshMessage {
			int64_t msg;
			std::string group;
			int64_t chaId;
			std::string chaName;
			std::string motto;
			int64_t icon;
		};

		struct PcFrndRefreshDelMessage {
			int64_t msg;
			int64_t chaId;
		};

		struct PcFrndChangeGroupMessage {
			int64_t friendChaId;
			std::string groupName;
		};

		struct PcFrndRefreshInfoMessage {
			int64_t chaId;
			std::string motto;
			int64_t icon;
			int64_t degree;
			std::string job;
			std::string guildName;
		};

		struct PcSay2AllMessage {
			std::string chaName;
			std::string content;
			int64_t color;
		};

		struct PcSay2TradeMessage {
			std::string chaName;
			std::string content;
			int64_t color;
		};

		struct PcSay2YouMessage {
			std::string sender;
			std::string target;
			std::string content;
			int64_t color;
		};

		struct PcSay2TemMessage {
			int64_t chaId;
			std::string content;
			int64_t color;
		};

		struct PcSay2GudMessage {
			std::string chaName;
			std::string content;
			int64_t color;
		};

		struct PcGm1SayMessage {
			std::string chaName;
			std::string content;
		};

		struct PcGm1Say1Message {
			std::string content;
			int64_t setNum;
			int64_t color;
		};

		struct PcGuildPermMessage {
			int64_t targetChaId;
			int64_t permission;
		};

		struct PcMasterRefreshAddMessage {
			int64_t msg;
			std::string group;
			int64_t chaId;
			std::string chaName;
			std::string motto;
			int64_t icon;
		};

		struct PcMasterRefreshDelMessage {
			int64_t msg;
			int64_t chaId;
		};

		struct PcSessCreateMessage {
			int64_t sessId;
			std::vector<SessMemberData> members;
			int64_t notiPlyCount;
		};

		struct PcSessAddMessage {
			int64_t sessId;
			int64_t chaId;
			std::string chaName;
			std::string motto;
			int64_t icon;
		};

		struct PcSessSayMessage {
			int64_t sessId;
			int64_t chaId;
			std::string content;
		};

		struct PcSessLeaveMessage {
			int64_t sessId;
			int64_t chaId;
		};

		struct PcChangePersonInfoMessage {
			std::string motto;
			int64_t icon;
			int64_t refuseSess;
		};

		struct PcErrMsgMessage {
			std::string message;
		};

		struct PcGuildNoticeMessage {
			std::string content;
		};

		struct PcRegisterMessage {
			int64_t result;
			std::string message;
		};

		// =================================================================
		//  Группа F: GroupServer -> GameServer (PM) — async
		// =================================================================

		struct PmTeamMessage {
			int64_t msg;
			int64_t count;
			std::vector<PmTeamMemberData> members;
		};

		struct PmExpScaleMessage {
			int64_t chaId;
			int64_t time;
		};

		struct PmSay2AllMessage {
			int64_t chaId;
			std::string content;
			int64_t money;
		};

		struct PmSay2TradeMessage {
			int64_t chaId;
			std::string content;
			int64_t money;
		};

		struct PmGuildDisbandMessage {
			int64_t guildId;
		};

		struct PmGuildChallMoneyMessage {
			int64_t leaderId;
			int64_t money;
			std::string guildName1;
			std::string guildName2;
		};

		struct PmGuildChallPrizeMoneyMessage {
			int64_t leaderId;
			int64_t money;
		};

		struct PtKickUserMessage {
			int64_t gpAddr;
			int64_t gtAddr;
		};

		// =================================================================
		//  Группа H: GateServer <-> GameServer (TM/MT)
		// =================================================================

		constexpr int MAPCOPY_INFO_PARAM_NUM = 16; // defMAPCOPY_INFO_PARAM_NUM

		/// CMD_MT_LOGIN (1501): GameServer → GateServer. Регистрация GameServer на GateServer.
		struct MtLoginMessage {
			std::string serverName;
			std::string mapNameList; // список карт через запятую
		};

		/// CMD_TM_LOGIN_ACK (1005): GateServer → GameServer. Ответ на MT_LOGIN.
		struct TmLoginAckMessage {
			int16_t errCode = 0;
			std::string gateName; // только при errCode == 0
		};

		/// CMD_TM_ENTERMAP (1003): GateServer → GameServer. Игрок входит на карту.
		/// Последние 2 поля читаются GameServer'ом с конца пакета (ReverseRead).
		struct TmEnterMapMessage {
			int64_t actId = 0; // actor login ID
			std::string password;
			int64_t dbCharId = 0;
			int64_t worldId = 0;
			std::string mapName;
			int64_t mapCopyNo = 0;
			int64_t posX = 0;
			int64_t posY = 0;
			int64_t loginFlag = 0; // CHAR: 0=offline mode, 1=normal
			// Trailer (reverse-read на стороне GameServer):
			int64_t winer = 0; // SHORT
			int64_t gateAddr = 0; // LONG — адрес GateServer
		};

		/// CMD_TM_GOOUTMAP (1004): GateServer → GameServer. Игрок покидает карту.
		/// Все поля читаются с конца пакета (ReverseRead).
		struct TmGoOutMapMessage {
			int64_t playerPtr = 0; // CPlayer pointer
			int64_t gateAddr = 0;
			int64_t offlineFlag = 0; // CHAR: 0=normal, non-0=offline/timeout
		};

		/// CMD_TM_CHANGE_PERSONINFO (1006): GateServer → GameServer. Смена motto/icon.
		struct TmChangePersonInfoMessage {
			std::string motto;
			int64_t icon = 0;
		};

		/// CMD_TM_MAPENTRY (1007) / CMD_MT_MAPENTRY (1506): MapEntry sub-types.
		/// Подтипы: CREATE=0, DESTROY=1, SUBPLAYER=2, SUBCOPY=3, RETURN=4, COPYPARAM=5, COPYRUN=6.
		constexpr int64_t MAPENTRY_CREATE = 0;
		constexpr int64_t MAPENTRY_DESTROY = 1;
		constexpr int64_t MAPENTRY_SUBPLAYER = 2;
		constexpr int64_t MAPENTRY_SUBCOPY = 3;
		constexpr int64_t MAPENTRY_RETURN = 4;
		constexpr int64_t MAPENTRY_COPYPARAM = 5;
		constexpr int64_t MAPENTRY_COPYRUN = 6;

		/// Общая структура MAPENTRY (все подтипы). Не все поля заполнены — зависит от subType.
		struct MapEntryMessage {
			std::string srcMapName;
			std::string targetMapName;
			int64_t subType = 0;
			// CREATE:
			int64_t posX = 0;
			int64_t posY = 0;
			int64_t mapCopyNum = 0;
			int64_t copyPlayerNum = 0;
			std::vector<std::string> luaScriptLines;
			// RETURN:
			int64_t resultCode = 0; // 0=CREATE_SUC, 1=COPY_CLOSE_SUC, 2=DESTROY_SUC
			int64_t returnCopyNo = 0;
			// SUBPLAYER:
			int64_t copyNo = 0;
			int64_t numPlayers = 0;
			// COPYPARAM:
			int64_t copyId = 0;
			int64_t params[MAPCOPY_INFO_PARAM_NUM] = {};
			// COPYRUN:
			int64_t condType = 0;
			int64_t condValue = 0;
		};

		/// CMD_TM_MAPENTRY_NOMAP (1008): GateServer → GameServer. Пустая команда (нет целевой карты).
		struct TmMapEntryNoMapMessage {
		};

		/// CMD_TM_KICKCHA (1009): GateServer → GameServer. SESS-echo (запрос-ответ).
		struct TmKickChaMessage {
			int64_t charDbId = 0;
		};

		struct TmKickChaResponse {
			int64_t result = 0; // 0=not found, 1=success
		};

		/// CMD_TM_OFFLINE_MODE (1011): GateServer → GameServer. SESS-echo.
		/// Поля читаются с конца пакета (ReverseRead).
		struct TmOfflineModeMessage {
			int64_t playerPtr = 0;
			int64_t gateAddr = 0;
		};

		struct TmOfflineModeResponse {
			int64_t result = 0; // 0=Unknown, 1=Success, 2=Disabled, 3=Dead, 4=Refuse
		};

		/// CMD_MT_SWITCHMAP (1504): GameServer → GateServer. Переключение карты.
		/// Включает payload + ReflectINFof trailer.
		struct MtSwitchMapMessage {
			std::string currentMapName;
			int64_t currentCopyNo = 0;
			int64_t posX = 0;
			int64_t posY = 0;
			std::string targetMapName;
			int64_t targetCopyNo = 0;
			int64_t targetX = 0;
			int64_t targetY = 0;
			int64_t switchType = 0; // CHAR: 1=death, 0=normal
			// ReflectINFof trailer:
			int64_t playerDBID = 0;
			int64_t gateAddr = 0;
			int64_t aimNum = 1;
		};

		/// CMD_MT_KICKUSER (1505): GameServer → GateServer. Кик игрока.
		struct MtKickUserMessage {
			int64_t playerDBID = 0;
			int64_t gateAddr = 0;
			int64_t kickFlag = 1;
		};

		/// CMD_MT_PALYEREXIT (1508): GameServer → GateServer. Таймаут/принудительный выход.
		/// Пустой payload + ReflectINFof trailer.
		struct MtPlayerExitMessage {
			int64_t playerDBID = 0;
			int64_t gateAddr = 0;
			int64_t aimNum = 1;
		};

		// =================================================================
		//  Группа I: GameServer <-> GameServer (MM)
		// =================================================================

		/// Общий заголовок MM_ команд (читается ProcessInterGameMsg).
		/// Последние 3 поля — routing trailer (ReverseRead).
		struct MmHeader {
			int64_t srcId = 0; // lSrcID — ID персонажа/гильдии
			// Trailer (reverse-read):
			int64_t gatePlayerAddr = 0;
			int64_t gatePlayerId = 0;
			int64_t sNum = 0; // SHORT: count (обычно 1)
		};

		/// CMD_MM_GATE_CONNECT (4002): Уведомление о подключении GateServer.
		struct MmGateConnectMessage {
		};

		/// CMD_MM_GATE_RELEASE (4001): Уведомление об отключении GateServer.
		struct MmGateReleaseMessage {
		};

		/// CMD_MM_QUERY_CHA (4003): Запрос информации о персонаже.
		struct MmQueryChaMessage {
			int64_t srcId = 0;
			std::string chaName;
		};

		/// CMD_MM_QUERY_CHA ответ (отправляется как CMD_MC_QUERY_CHA).
		struct MmQueryChaResponse {
			int64_t srcId = 0;
			std::string chaName;
			std::string subMapName;
			int64_t posX = 0;
			int64_t posY = 0;
			int64_t chaId = 0;
		};

		/// CMD_MM_QUERY_CHAITEM (4004): Запрос инвентаря персонажа.
		struct MmQueryChaItemMessage {
			int64_t srcId = 0;
			std::string chaName;
		};

		/// CMD_MM_CALL_CHA (4005): Телепорт персонажа к вызывающему.
		struct MmCallChaMessage {
			int64_t srcId = 0;
			std::string targetName;
			int64_t isBoat = 0;
			std::string mapName;
			int64_t posX = 0;
			int64_t posY = 0;
			int64_t copyNo = 0;
		};

		/// CMD_MM_GOTO_CHA (4006): Телепорт к персонажу (2 режима).
		struct MmGotoChaMessage {
			int64_t srcId = 0;
			std::string targetName;
			int64_t mode = 0; // 1=query, 2=execute
			// mode==1: srcName
			std::string srcName;
			// mode==2: isBoat, mapName, posX, posY, copyNo
			int64_t isBoat = 0;
			std::string mapName;
			int64_t posX = 0;
			int64_t posY = 0;
			int64_t copyNo = 0;
		};

		/// CMD_MM_KICK_CHA (4007): Кик персонажа.
		struct MmKickChaMessage {
			int64_t srcId = 0;
			std::string targetName;
			int64_t kickDuration = 0; // мс, 0=немедленно
		};

		/// CMD_MM_GUILD_REJECT (4008): Отклонение заявки в гильдию.
		struct MmGuildRejectMessage {
			int64_t srcId = 0; // chaId заявителя
			std::string guildName;
		};

		/// CMD_MM_GUILD_APPROVE (4009): Одобрение заявки в гильдию.
		struct MmGuildApproveMessage {
			int64_t srcId = 0; // chaId заявителя
			int64_t guildId = 0;
			std::string guildName;
			std::string guildMotto;
		};

		/// CMD_MM_GUILD_KICK (4010): Исключение из гильдии.
		struct MmGuildKickMessage {
			int64_t srcId = 0; // chaId исключаемого
			std::string guildName;
		};

		/// CMD_MM_GUILD_DISBAND (4011): Расформирование гильдии.
		struct MmGuildDisbandMessage {
			int64_t srcId = 0; // guildId
		};

		/// CMD_MM_QUERY_CHAPING (4012): Запрос пинга персонажа.
		struct MmQueryChaPingMessage {
			int64_t srcId = 0;
			std::string chaName;
		};

		/// CMD_MM_NOTICE (4013): Системное уведомление (broadcast).
		struct MmNoticeMessage {
			std::string content;
		};

		/// CMD_MM_GUILD_MOTTO (4014): Обновление девиза гильдии.
		struct MmGuildMottoMessage {
			int64_t guildId = 0; // srcId = guildId
			std::string motto;
		};

		/// CMD_MM_DO_STRING (4015): Выполнение Lua-скрипта на всех серверах.
		struct MmDoStringMessage {
			int64_t srcId = 0;
			std::string luaCode;
		};

		/// CMD_MM_CHA_NOTICE (4016): Уведомление конкретному персонажу.
		struct MmChaNoticeMessage {
			std::string content;
			std::string chaName; // пусто = broadcast
		};

		/// CMD_MM_LOGIN (4017): Уведомление о входе игрока.
		struct MmLoginMessage {
			std::string chaName;
		};

		/// CMD_MM_GUILD_CHALL_PRIZEMONEY (4018): Призовые деньги гильдии.
		struct MmGuildChallPrizeMoneyMessage {
			int64_t charDbId = 0;
			int64_t money = 0;
		};

		/// CMD_MM_ADDCREDIT (4019): Добавление кредитов персонажу.
		struct MmAddCreditMessage {
			int64_t charDbId = 0;
			int64_t amount = 0;
		};

		/// CMD_MM_STORE_BUY (4020): Покупка из магазина.
		struct MmStoreBuyMessage {
			int64_t charDbId = 0;
			int64_t commodityId = 0;
			int64_t money = 0;
		};

		/// CMD_MM_ADDMONEY (4021): Добавление денег персонажу.
		struct MmAddMoneyMessage {
			int64_t charDbId = 0;
			int64_t money = 0;
		};

		/// CMD_MM_AUCTION (4022): Завершение аукциона.
		struct MmAuctionMessage {
			int64_t charDbId = 0;
		};

		/// CMD_MM_UPDATEGUILDBANK (4023): Синхронизация банка гильдии.
		struct MmUpdateGuildBankMessage {
			int64_t guildId = 0;
		};

		/// CMD_MM_UPDATEGUILDBANKGOLD (4024): Синхронизация золота банка гильдии.
		struct MmUpdateGuildBankGoldMessage {
			int64_t guildId = 0;
		};

		// =================================================================
		//  Группа G: Client -> GateServer (CM) — логин/выбор персонажа
		// =================================================================

		struct CmLoginRequest {
			std::string acctName;
			std::string passwordHash;
			std::string mac;
			int64_t cheatMarker;
			int64_t clientVersion;
		};

		/// Данные успешного ответа CMD_MC_LOGIN (GateServer → Client).
		struct McLoginResponseData {
			int64_t maxChaNum;
			std::vector<ChaSlotData> characters;
			bool hasPassword2;
		};

		/// CMD_MC_LOGIN ответ. errCode=0 + data = успех, errCode!=0 = ошибка.
		struct McLoginResponse {
			int16_t errCode;
			std::optional<McLoginResponseData> data;
		};

		/// CMD_MC_BGNPLAY ответ (GateServer → Client). Только errCode; при успехе клиент входит через ENTERMAP.
		struct McBgnPlayResponse {
			int16_t errCode;
		};

		/// Данные успешного ответа CMD_MC_ENDPLAY (GateServer → Client).
		struct McEndPlayResponseData {
			int64_t maxChaNum;
			std::vector<ChaSlotData> characters;
		};

		/// CMD_MC_ENDPLAY ответ. errCode=0 + data = успех, errCode!=0 = ошибка.
		struct McEndPlayResponse {
			int16_t errCode;
			std::optional<McEndPlayResponseData> data;
		};

		/// CMD_MC_NEWCHA ответ (GateServer → Client).
		struct McNewChaResponse {
			int16_t errCode;
		};

		/// CMD_MC_DELCHA ответ (GateServer → Client).
		struct McDelChaResponse {
			int16_t errCode;
		};

		/// CMD_MC_CREATE_PASSWORD2 ответ (GateServer → Client).
		struct McCreatePassword2Response {
			int16_t errCode;
		};

		/// CMD_MC_UPDATE_PASSWORD2 ответ (GateServer → Client).
		struct McUpdatePassword2Response {
			int16_t errCode;
		};

		// =================================================================
		//  CMD_MC_ENTERMAP — под-структуры (переиспользуемые)
		// =================================================================

		/// Информация о фракции персонажа.
		struct ChaSideInfo {
			int64_t sideId = 0;
		};

		/// Событие сущности.
		struct ChaEventInfo {
			int64_t entityId = 0;
			int64_t entityType = 0;
			int64_t eventId = 0;
			std::string eventName;
		};

		/// Слот экипировки во внешнем виде (Look).
		/// Формат зависит от synType: SYN_LOOK_CHANGE — краткий, иначе (SWITCH) — полный + extra.
		struct LookEquipSlot {
			int64_t id = 0;
			int64_t dbId = 0;
			int64_t needLv = 0;
			// Поля ниже только при id > 0. CHANGE: endure0,energy0,valid,tradable,expiration.
			// SWITCH: num,endure0,endure1,energy0,energy1,forgeLv,valid,tradable,expiration + extra блок.
			int64_t num = 0;
			int64_t endure0 = 0;
			int64_t endure1 = 0;
			int64_t energy0 = 0;
			int64_t energy1 = 0;
			int64_t forgeLv = 0;
			int64_t valid = 0;
			int64_t tradable = 0;
			int64_t expiration = 0;
			// Extra data (только SWITCH):
			bool hasExtra = false;
			int64_t forgeParam = 0;
			int64_t instId = 0;
			bool hasInstAttr = false;
			int64_t instAttr[ITEM_INSTANCE_ATTR_NUM][2] = {};
		};

		/// Части корабля (Look для isBoat=true).
		struct BoatLookParts {
			int64_t posId = 0;
			int64_t boatId = 0;
			int64_t header = 0;
			int64_t body = 0;
			int64_t engine = 0;
			int64_t cannon = 0;
			int64_t equipment = 0;
		};

		/// Внешний вид персонажа/корабля.
		struct ChaLookInfo {
			int64_t synType = 0;
			int64_t typeId = 0;
			bool isBoat = false;
			BoatLookParts boatParts; // valid if isBoat
			int64_t hairId = 0; // valid if !isBoat
			LookEquipSlot equips[EQUIP_NUM]; // valid if !isBoat
		};

		/// Слот доп. внешнего вида (костюмы и т.д.).
		struct AppendLookSlot {
			int64_t lookId = 0;
			int64_t valid = 0; // только если lookId != 0
		};

		/// Базовая информация о персонаже (ReadChaBasePacket).
		struct ChaBaseInfo {
			int64_t chaId = 0;
			int64_t worldId = 0;
			int64_t commId = 0;
			std::string commName;
			int64_t gmLv = 0;
			int64_t handle = 0;
			int64_t ctrlType = 0;
			std::string name;
			std::string motto;
			int64_t icon = 0;
			int64_t guildId = 0;
			std::string guildName;
			std::string guildMotto;
			int64_t guildPermission = 0;
			std::string stallName;
			int64_t state = 0;
			int64_t posX = 0;
			int64_t posY = 0;
			int64_t radius = 0;
			int64_t angle = 0;
			int64_t teamLeaderId = 0;
			int64_t isPlayer = 0;
			ChaSideInfo side;
			ChaEventInfo event;
			ChaLookInfo look;
			int64_t pkCtrl = 0;
			AppendLookSlot appendLook[ESPE_KBGRID_NUM];
		};

		/// Запись скилла в сумке.
		struct SkillEntry {
			int64_t id = 0;
			int64_t state = 0;
			int64_t level = 0;
			int64_t useSp = 0;
			int64_t useEndure = 0;
			int64_t useEnergy = 0;
			int64_t resumeTime = 0;
			int64_t range[SKILL_RANGE_PARAM_NUM] = {};
		};

		/// Сумка скиллов (ReadChaSkillBagPacket).
		struct ChaSkillBagInfo {
			int64_t defSkillId = 0;
			int64_t synType = 0;
			std::vector<SkillEntry> skills;
		};

		/// Запись активного состояния скилла.
		struct SkillStateEntry {
			int64_t stateId = 0;
			int64_t stateLv = 0;
			int64_t duration = 0;
			int64_t startTime = 0;
		};

		/// Активные состояния скиллов (ReadChaSkillStatePacket).
		struct ChaSkillStateInfo {
			int64_t currentTime = 0;
			std::vector<SkillStateEntry> states;
		};

		/// Запись атрибута.
		struct AttrEntry {
			int64_t attrId = 0;
			int64_t attrVal = 0;
		};

		/// Атрибуты персонажа (ReadChaAttrPacket).
		struct ChaAttrInfo {
			int64_t synType = 0;
			std::vector<AttrEntry> attrs;
		};

		/// Предмет в инвентаре (ReadChaKitbagPacket).
		/// isBoat должен быть установлен вызывающим кодом (сериализация) или callback'ом (десериализация).
		struct KitbagItem {
			int64_t gridId = 0;
			int64_t itemId = 0;
			// Только если itemId > 0:
			int64_t dbId = 0;
			int64_t needLv = 0;
			int64_t num = 0;
			int64_t endure0 = 0;
			int64_t endure1 = 0;
			int64_t energy0 = 0;
			int64_t energy1 = 0;
			int64_t forgeLv = 0;
			int64_t valid = 0;
			int64_t tradable = 0;
			int64_t expiration = 0;
			bool isBoat = false; // sType == enumItemTypeBoat
			int64_t boatWorldId = 0; // только если isBoat
			int64_t forgeParam = 0;
			int64_t instId = 0;
			bool hasInstAttr = false;
			int64_t instAttr[ITEM_INSTANCE_ATTR_NUM][2] = {};
		};

		/// Инвентарь (ReadChaKitbagPacket).
		struct ChaKitbagInfo {
			int64_t synType = 0;
			int64_t capacity = 0; // только при synType == SYN_KITBAG_INIT
			std::vector<KitbagItem> items;
		};

		/// Запись быстрой клавиши.
		struct ShortcutEntry {
			int64_t type = 0;
			int64_t gridId = 0;
		};

		/// Панель быстрого доступа (ReadChaShortcutPacket).
		struct ChaShortcutInfo {
			ShortcutEntry entries[SHORT_CUT_NUM];
		};

		/// Данные корабля (базовая инфо + атрибуты + инвентарь + состояния).
		struct BoatData {
			ChaBaseInfo baseInfo;
			ChaAttrInfo attr;
			ChaKitbagInfo kitbag;
			ChaSkillStateInfo skillState;
		};

		/// Данные успешного CMD_MC_ENTERMAP (GameServer → GateServer → Client).
		struct McEnterMapData {
			int64_t autoLock = 0;
			int64_t kitbagLock = 0;
			int64_t enterType = 0;
			int64_t isNewCha = 0;
			std::string mapName;
			int64_t canTeam = 0;
			int64_t imp = 0;
			ChaBaseInfo baseInfo;
			ChaSkillBagInfo skillBag;
			ChaSkillStateInfo skillState;
			ChaAttrInfo attr;
			ChaKitbagInfo kitbag;
			ChaShortcutInfo shortcut;
			std::vector<BoatData> boats;
			int64_t ctrlChaId = 0;
		};

		/// Trailing-поля MC_ENTERMAP, которые GateServer читает и срезает
		/// перед пересылкой клиенту (loginFlag/playerCount/playerAddr).
		/// Клиент их никогда не видит.
		struct McEnterMapTrailer {
			int64_t loginFlag = 0;
			int64_t playerCount = 0;
			int64_t playerAddr = 0;
		};

		/// CMD_MC_ENTERMAP. errCode=0 + data = успех, errCode!=0 = ошибка.
		struct McEnterMapMessage {
			int64_t errCode = 0;
			std::optional<McEnterMapData> data;
		};

		// =================================================================
		//  Serialize — функции сериализации структур в WPacket
		// =================================================================

		// --- Группа A: TP/PT ---

		inline WPacket serialize(const TpLoginRequest& msg) {
			WPacket w(64);
			w.WriteCmd(CMD_TP_LOGIN);
			w.WriteInt64(static_cast<int64_t>(msg.protocolVersion));
			w.WriteString(msg.gateName);
			return w;
		}

		inline WPacket serialize(const TpLoginResponse& msg) {
			WPacket w(16);
			w.WriteCmd(CMD_TP_LOGIN);
			w.WriteInt64(static_cast<int64_t>(msg.errCode));
			return w;
		}

		inline WPacket serialize(const TpUserLoginRequest& msg) {
			WPacket w(256);
			w.WriteCmd(CMD_TP_USER_LOGIN);
			w.WriteString(msg.acctName);
			w.WriteString(msg.acctPassword);
			w.WriteString(msg.acctMac);
			w.WriteInt64(static_cast<int64_t>(msg.clientIp));
			w.WriteInt64(static_cast<int64_t>(msg.gateAddr));
			w.WriteInt64(msg.cheatMarker);
			return w;
		}

		inline WPacket serialize(const TpUserLoginResponse& msg) {
			WPacket w(512);
			w.WriteCmd(CMD_TP_USER_LOGIN);
			w.WriteInt64(static_cast<int64_t>(msg.errCode));
			if (msg.data.has_value()) {
				const auto& d = msg.data.value();
				w.WriteInt64(d.maxChaNum);
				for (const auto& cha : d.characters) {
					w.WriteInt64(cha.valid ? 1 : 0);
					if (cha.valid) {
						w.WriteString(cha.chaName);
						w.WriteString(cha.job);
						w.WriteInt64(cha.degree);
						w.WriteInt64(cha.typeId);
						for (int i = 0; i < EQUIP_NUM; ++i)
							w.WriteInt64(i < static_cast<int>(cha.equipIds.size()) ? cha.equipIds[i] : 0);
					}
				}
				w.WriteInt64(d.hasPassword2 ? 1 : 0);
				w.WriteInt64(d.acctId);
				w.WriteInt64(d.acctLoginId);
				w.WriteInt64(static_cast<int64_t>(d.gpAddr));
			}
			return w;
		}

		inline WPacket serialize(const TpUserLogoutRequest& msg) {
			WPacket w(32);
			w.WriteCmd(CMD_TP_USER_LOGOUT);
			w.WriteInt64(static_cast<int64_t>(msg.gateAddr));
			w.WriteInt64(static_cast<int64_t>(msg.gpAddr));
			return w;
		}

		inline WPacket serialize(const TpUserLogoutResponse& msg) {
			WPacket w(16);
			w.WriteCmd(CMD_TP_USER_LOGOUT);
			w.WriteInt64(static_cast<int64_t>(msg.errCode));
			return w;
		}

		inline WPacket serialize(const TpBgnPlayRequest& msg) {
			WPacket w(32);
			w.WriteCmd(CMD_TP_BGNPLAY);
			w.WriteInt64(msg.chaIndex);
			w.WriteInt64(static_cast<int64_t>(msg.gateAddr));
			w.WriteInt64(static_cast<int64_t>(msg.gpAddr));
			return w;
		}

		inline WPacket serialize(const TpBgnPlayResponse& msg) {
			WPacket w(256);
			w.WriteCmd(CMD_TP_BGNPLAY);
			w.WriteInt64(static_cast<int64_t>(msg.errCode));
			if (msg.data.has_value()) {
				const auto& d = msg.data.value();
				w.WriteString(d.password2);
				w.WriteInt64(d.chaId);
				w.WriteInt64(d.worldId);
				w.WriteString(d.mapName);
				w.WriteInt64(d.swiner);
			}
			return w;
		}

		inline WPacket serialize(const TpEndPlayRequest& msg) {
			WPacket w(32);
			w.WriteCmd(CMD_TP_ENDPLAY);
			w.WriteInt64(static_cast<int64_t>(msg.gateAddr));
			w.WriteInt64(static_cast<int64_t>(msg.gpAddr));
			return w;
		}

		inline WPacket serialize(const TpEndPlayResponse& msg) {
			WPacket w(512);
			w.WriteCmd(CMD_TP_ENDPLAY);
			w.WriteInt64(static_cast<int64_t>(msg.errCode));
			if (msg.data.has_value()) {
				const auto& d = msg.data.value();
				w.WriteInt64(d.maxChaNum);
				w.WriteInt64(d.chaNum);
				for (const auto& cha : d.characters) {
					w.WriteInt64(cha.valid ? 1 : 0);
					if (cha.valid) {
						w.WriteString(cha.chaName);
						w.WriteString(cha.job);
						w.WriteInt64(cha.degree);
						w.WriteInt64(cha.typeId);
						for (int i = 0; i < EQUIP_NUM; ++i)
							w.WriteInt64(i < static_cast<int>(cha.equipIds.size()) ? cha.equipIds[i] : 0);
					}
				}
			}
			return w;
		}

		inline WPacket serialize(const TpNewChaRequest& msg) {
			WPacket w(128);
			w.WriteCmd(CMD_TP_NEWCHA);
			w.WriteString(msg.chaName);
			w.WriteString(msg.birth);
			w.WriteInt64(msg.typeId);
			w.WriteInt64(msg.hairId);
			w.WriteInt64(msg.faceId);
			w.WriteInt64(static_cast<int64_t>(msg.gateAddr));
			w.WriteInt64(static_cast<int64_t>(msg.gpAddr));
			return w;
		}

		inline WPacket serialize(const TpNewChaResponse& msg) {
			WPacket w(16);
			w.WriteCmd(CMD_TP_NEWCHA);
			w.WriteInt64(static_cast<int64_t>(msg.errCode));
			return w;
		}

		inline WPacket serialize(const TpDelChaRequest& msg) {
			WPacket w(128);
			w.WriteCmd(CMD_TP_DELCHA);
			w.WriteInt64(msg.chaIndex);
			w.WriteString(msg.password2);
			w.WriteInt64(static_cast<int64_t>(msg.gateAddr));
			w.WriteInt64(static_cast<int64_t>(msg.gpAddr));
			return w;
		}

		inline WPacket serialize(const TpDelChaResponse& msg) {
			WPacket w(16);
			w.WriteCmd(CMD_TP_DELCHA);
			w.WriteInt64(static_cast<int64_t>(msg.errCode));
			return w;
		}

		inline WPacket serialize(const TpChangePassRequest& msg) {
			WPacket w(128);
			w.WriteCmd(CMD_TP_CHANGEPASS);
			w.WriteString(msg.newPass);
			w.WriteString(msg.pin);
			w.WriteInt64(static_cast<int64_t>(msg.gateAddr));
			w.WriteInt64(static_cast<int64_t>(msg.gpAddr));
			return w;
		}

		inline WPacket serialize(const TpRegisterRequest& msg) {
			WPacket w(256);
			w.WriteCmd(CMD_TP_REGISTER);
			w.WriteString(msg.username);
			w.WriteString(msg.password);
			w.WriteString(msg.email);
			return w;
		}

		inline WPacket serialize(const TpRegisterResponse& msg) {
			WPacket w(128);
			w.WriteCmd(CMD_TP_REGISTER);
			w.WriteInt64(msg.result);
			w.WriteString(msg.message);
			return w;
		}

		inline WPacket serialize(const TpCreatePassword2Request& msg) {
			WPacket w(64);
			w.WriteCmd(CMD_TP_CREATE_PASSWORD2);
			w.WriteString(msg.password2);
			w.WriteInt64(static_cast<int64_t>(msg.gateAddr));
			w.WriteInt64(static_cast<int64_t>(msg.gpAddr));
			return w;
		}

		inline WPacket serialize(const TpUpdatePassword2Request& msg) {
			WPacket w(128);
			w.WriteCmd(CMD_TP_UPDATE_PASSWORD2);
			w.WriteString(msg.oldPassword2);
			w.WriteString(msg.newPassword2);
			w.WriteInt64(static_cast<int64_t>(msg.gateAddr));
			w.WriteInt64(static_cast<int64_t>(msg.gpAddr));
			return w;
		}

		inline WPacket serialize(const TpPassword2Response& msg) {
			WPacket w(16);
			w.WriteCmd(CMD_TP_CREATE_PASSWORD2);
			w.WriteInt64(static_cast<int64_t>(msg.errCode));
			return w;
		}

		inline WPacket serialize(const TpReqPlyLstResponse& msg) {
			WPacket w(256);
			w.WriteCmd(CMD_TP_REQPLYLST);
			for (const auto& e : msg.entries) {
				w.WriteInt64(e.gtAddr);
				w.WriteInt64(e.chaId);
			}
			w.WriteInt64(msg.plyNum);
			return w;
		}

		inline WPacket serialize(const TpSyncPlyLstRequest& msg) {
			WPacket w(256);
			w.WriteCmd(CMD_TP_SYNC_PLYLST);
			w.WriteInt64(msg.num);
			w.WriteString(msg.gateName);
			for (const auto& p : msg.players) {
				w.WriteInt64(p.gtAddr);
				w.WriteInt64(p.acctLoginId);
				w.WriteInt64(p.acctId);
			}
			return w;
		}

		inline WPacket serialize(const TpSyncPlyLstResponse& msg) {
			WPacket w(256);
			w.WriteCmd(CMD_TP_SYNC_PLYLST);
			w.WriteInt64(static_cast<int64_t>(msg.errCode));
			w.WriteInt64(msg.num);
			for (const auto& r : msg.results) {
				w.WriteInt64(r.ok ? 1 : 0);
				w.WriteInt64(r.playerPtr);
			}
			return w;
		}

		inline WPacket serialize(const OsLoginRequest& msg) {
			WPacket w(64);
			w.WriteCmd(CMD_OS_LOGIN);
			w.WriteInt64(msg.version);
			w.WriteString(msg.agentName);
			return w;
		}

		inline WPacket serialize(const OsLoginResponse& msg) {
			WPacket w(16);
			w.WriteCmd(CMD_OS_LOGIN);
			w.WriteInt64(static_cast<int64_t>(msg.errCode));
			return w;
		}

		inline WPacket serialize(const TpDiscMessage& msg) {
			WPacket w(128);
			w.WriteCmd(CMD_TP_DISC);
			w.WriteInt64(msg.actId);
			w.WriteInt64(msg.ipAddr);
			w.WriteString(msg.reason);
			return w;
		}

		// --- Группа B: PA/AP ---

		inline WPacket serialize(const PaLoginRequest& msg) {
			WPacket w(128);
			w.WriteCmd(CMD_PA_LOGIN);
			w.WriteString(msg.serverName);
			w.WriteString(msg.serverPassword);
			return w;
		}

		inline WPacket serialize(const PaLoginResponse& msg) {
			WPacket w(16);
			w.WriteCmd(CMD_PA_LOGIN);
			w.WriteInt64(static_cast<int64_t>(msg.errCode));
			return w;
		}

		inline WPacket serialize(const PaUserLoginRequest& msg) {
			WPacket w(256);
			w.WriteCmd(CMD_PA_USER_LOGIN);
			w.WriteString(msg.username);
			w.WriteString(msg.password);
			w.WriteString(msg.mac);
			w.WriteInt64(msg.clientIp);
			return w;
		}

		inline WPacket serialize(const PaUserLoginResponse& msg) {
			WPacket w(64);
			w.WriteCmd(0);
			w.WriteInt64(static_cast<int64_t>(msg.errCode));
			if (msg.data.has_value()) {
				const auto& d = msg.data.value();
				w.WriteInt64(d.acctLoginId);
				w.WriteInt64(d.sessId);
				w.WriteInt64(d.acctId);
				w.WriteInt64(d.gmLevel);
			}
			return w;
		}

		inline WPacket serialize(const PaUserLogoutMessage& msg) {
			WPacket w(16);
			w.WriteCmd(CMD_PA_USER_LOGOUT);
			w.WriteInt64(msg.acctLoginId);
			return w;
		}

		inline WPacket serialize(const PaUserBillBgnMessage& msg) {
			WPacket w(128);
			w.WriteCmd(CMD_PA_USER_BILLBGN);
			w.WriteString(msg.acctName);
			w.WriteString(msg.passport);
			return w;
		}

		inline WPacket serialize(const PaUserBillEndMessage& msg) {
			WPacket w(64);
			w.WriteCmd(CMD_PA_USER_BILLEND);
			w.WriteString(msg.acctName);
			return w;
		}

		inline WPacket serialize(const PaChangePassMessage& msg) {
			WPacket w(128);
			w.WriteCmd(CMD_PA_CHANGEPASS);
			w.WriteString(msg.username);
			w.WriteString(msg.newPassword);
			return w;
		}

		inline WPacket serialize(const PaRegisterMessage& msg) {
			WPacket w(256);
			w.WriteCmd(CMD_PA_REGISTER);
			w.WriteString(msg.username);
			w.WriteString(msg.password);
			w.WriteString(msg.email);
			return w;
		}

		inline WPacket serialize(const PaGmBanMessage& msg) {
			WPacket w(64);
			w.WriteCmd(CMD_PA_GMBANACCOUNT);
			w.WriteString(msg.actName);
			return w;
		}

		inline WPacket serialize(const PaGmUnbanMessage& msg) {
			WPacket w(64);
			w.WriteCmd(CMD_PA_GMUNBANACCOUNT);
			w.WriteString(msg.actName);
			return w;
		}

		inline WPacket serialize(const PaUserDisableMessage& msg) {
			WPacket w(32);
			w.WriteCmd(CMD_PA_USER_DISABLE);
			w.WriteInt64(msg.acctLoginId);
			w.WriteInt64(msg.minutes);
			return w;
		}

		inline WPacket serialize(const ApKickUserMessage& msg) {
			WPacket w(32);
			w.WriteCmd(CMD_AP_KICKUSER);
			w.WriteInt64(msg.errCode);
			w.WriteInt64(msg.accountId);
			return w;
		}

		inline WPacket serialize(const ApExpScaleMessage& msg) {
			WPacket w(32);
			w.WriteCmd(CMD_AP_EXPSCALE);
			w.WriteInt64(msg.chaId);
			w.WriteInt64(msg.time);
			return w;
		}

		// --- Группа C: CP ---

		inline WPacket serialize(const CpTeamInviteMessage& msg) {
			WPacket w(64);
			w.WriteCmd(CMD_CP_TEAM_INVITE);
			w.WriteString(msg.invitedName);
			w.WriteInt64(static_cast<int64_t>(msg.gateAddr));
			w.WriteInt64(static_cast<int64_t>(msg.gpAddr));
			return w;
		}

		inline WPacket serialize(const CpTeamAcceptMessage& msg) {
			WPacket w(32);
			w.WriteCmd(CMD_CP_TEAM_ACCEPT);
			w.WriteInt64(msg.inviterChaId);
			w.WriteInt64(static_cast<int64_t>(msg.gateAddr));
			w.WriteInt64(static_cast<int64_t>(msg.gpAddr));
			return w;
		}

		inline WPacket serialize(const CpTeamRefuseMessage& msg) {
			WPacket w(32);
			w.WriteCmd(CMD_CP_TEAM_REFUSE);
			w.WriteInt64(msg.inviterChaId);
			w.WriteInt64(static_cast<int64_t>(msg.gateAddr));
			w.WriteInt64(static_cast<int64_t>(msg.gpAddr));
			return w;
		}

		inline WPacket serialize(const CpTeamLeaveMessage& msg) {
			WPacket w(32);
			w.WriteCmd(CMD_CP_TEAM_LEAVE);
			w.WriteInt64(static_cast<int64_t>(msg.gateAddr));
			w.WriteInt64(static_cast<int64_t>(msg.gpAddr));
			return w;
		}

		inline WPacket serialize(const CpTeamKickMessage& msg) {
			WPacket w(32);
			w.WriteCmd(CMD_CP_TEAM_KICK);
			w.WriteInt64(msg.kickedChaId);
			w.WriteInt64(static_cast<int64_t>(msg.gateAddr));
			w.WriteInt64(static_cast<int64_t>(msg.gpAddr));
			return w;
		}

		inline WPacket serialize(const CpFrndInviteMessage& msg) {
			WPacket w(64);
			w.WriteCmd(CMD_CP_FRND_INVITE);
			w.WriteString(msg.invitedName);
			w.WriteInt64(static_cast<int64_t>(msg.gateAddr));
			w.WriteInt64(static_cast<int64_t>(msg.gpAddr));
			return w;
		}

		inline WPacket serialize(const CpFrndAcceptMessage& msg) {
			WPacket w(32);
			w.WriteCmd(CMD_CP_FRND_ACCEPT);
			w.WriteInt64(msg.inviterChaId);
			w.WriteInt64(static_cast<int64_t>(msg.gateAddr));
			w.WriteInt64(static_cast<int64_t>(msg.gpAddr));
			return w;
		}

		inline WPacket serialize(const CpFrndRefuseMessage& msg) {
			WPacket w(32);
			w.WriteCmd(CMD_CP_FRND_REFUSE);
			w.WriteInt64(msg.inviterChaId);
			w.WriteInt64(static_cast<int64_t>(msg.gateAddr));
			w.WriteInt64(static_cast<int64_t>(msg.gpAddr));
			return w;
		}

		inline WPacket serialize(const CpFrndDeleteMessage& msg) {
			WPacket w(32);
			w.WriteCmd(CMD_CP_FRND_DELETE);
			w.WriteInt64(msg.deletedChaId);
			w.WriteInt64(static_cast<int64_t>(msg.gateAddr));
			w.WriteInt64(static_cast<int64_t>(msg.gpAddr));
			return w;
		}

		inline WPacket serialize(const CpFrndChangeGroupMessage& msg) {
			WPacket w(64);
			w.WriteCmd(CMD_CP_FRND_CHANGE_GROUP);
			w.WriteInt64(msg.friendChaId);
			w.WriteString(msg.groupName);
			w.WriteInt64(static_cast<int64_t>(msg.gateAddr));
			w.WriteInt64(static_cast<int64_t>(msg.gpAddr));
			return w;
		}

		inline WPacket serialize(const CpFrndRefreshInfoMessage& msg) {
			WPacket w(32);
			w.WriteCmd(CMD_CP_FRND_REFRESH_INFO);
			w.WriteInt64(msg.friendChaId);
			w.WriteInt64(static_cast<int64_t>(msg.gateAddr));
			w.WriteInt64(static_cast<int64_t>(msg.gpAddr));
			return w;
		}

		inline WPacket serialize(const CpSay2AllMessage& msg) {
			WPacket w(256);
			w.WriteCmd(CMD_CP_SAY2ALL);
			w.WriteString(msg.content);
			w.WriteInt64(static_cast<int64_t>(msg.gateAddr));
			w.WriteInt64(static_cast<int64_t>(msg.gpAddr));
			return w;
		}

		inline WPacket serialize(const CpSay2TradeMessage& msg) {
			WPacket w(256);
			w.WriteCmd(CMD_CP_SAY2TRADE);
			w.WriteString(msg.content);
			w.WriteInt64(static_cast<int64_t>(msg.gateAddr));
			w.WriteInt64(static_cast<int64_t>(msg.gpAddr));
			return w;
		}

		inline WPacket serialize(const CpSay2YouMessage& msg) {
			WPacket w(256);
			w.WriteCmd(CMD_CP_SAY2YOU);
			w.WriteString(msg.targetName);
			w.WriteString(msg.content);
			w.WriteInt64(static_cast<int64_t>(msg.gateAddr));
			w.WriteInt64(static_cast<int64_t>(msg.gpAddr));
			return w;
		}

		inline WPacket serialize(const CpSay2TemMessage& msg) {
			WPacket w(256);
			w.WriteCmd(CMD_CP_SAY2TEM);
			w.WriteString(msg.content);
			w.WriteInt64(static_cast<int64_t>(msg.gateAddr));
			w.WriteInt64(static_cast<int64_t>(msg.gpAddr));
			return w;
		}

		inline WPacket serialize(const CpSay2GudMessage& msg) {
			WPacket w(256);
			w.WriteCmd(CMD_CP_SAY2GUD);
			w.WriteString(msg.content);
			w.WriteInt64(static_cast<int64_t>(msg.gateAddr));
			w.WriteInt64(static_cast<int64_t>(msg.gpAddr));
			return w;
		}

		inline WPacket serialize(const CpGm1SayMessage& msg) {
			WPacket w(256);
			w.WriteCmd(CMD_CP_GM1SAY);
			w.WriteString(msg.content);
			w.WriteInt64(static_cast<int64_t>(msg.gateAddr));
			w.WriteInt64(static_cast<int64_t>(msg.gpAddr));
			return w;
		}

		inline WPacket serialize(const CpGm1Say1Message& msg) {
			WPacket w(256);
			w.WriteCmd(CMD_CP_GM1SAY1);
			w.WriteString(msg.content);
			w.WriteInt64(msg.color);
			w.WriteInt64(static_cast<int64_t>(msg.gateAddr));
			w.WriteInt64(static_cast<int64_t>(msg.gpAddr));
			return w;
		}

		inline WPacket serialize(const CpSessCreateMessage& msg) {
			WPacket w(256);
			w.WriteCmd(CMD_CP_SESS_CREATE);
			w.WriteInt64(msg.chaNum);
			for (const auto& name : msg.chaNames) {
				w.WriteString(name);
			}
			w.WriteInt64(static_cast<int64_t>(msg.gateAddr));
			w.WriteInt64(static_cast<int64_t>(msg.gpAddr));
			return w;
		}

		inline WPacket serialize(const CpSessAddMessage& msg) {
			WPacket w(128);
			w.WriteCmd(CMD_CP_SESS_ADD);
			w.WriteInt64(msg.sessId);
			w.WriteString(msg.chaName);
			w.WriteInt64(static_cast<int64_t>(msg.gateAddr));
			w.WriteInt64(static_cast<int64_t>(msg.gpAddr));
			return w;
		}

		inline WPacket serialize(const CpSessSayMessage& msg) {
			WPacket w(256);
			w.WriteCmd(CMD_CP_SESS_SAY);
			w.WriteInt64(msg.sessId);
			w.WriteString(msg.content);
			w.WriteInt64(static_cast<int64_t>(msg.gateAddr));
			w.WriteInt64(static_cast<int64_t>(msg.gpAddr));
			return w;
		}

		inline WPacket serialize(const CpSessLeaveMessage& msg) {
			WPacket w(32);
			w.WriteCmd(CMD_CP_SESS_LEAVE);
			w.WriteInt64(msg.sessId);
			w.WriteInt64(static_cast<int64_t>(msg.gateAddr));
			w.WriteInt64(static_cast<int64_t>(msg.gpAddr));
			return w;
		}

		inline WPacket serialize(const CpChangePersonInfoMessage& msg) {
			WPacket w(128);
			w.WriteCmd(CMD_CP_CHANGE_PERSONINFO);
			w.WriteString(msg.motto);
			w.WriteInt64(msg.icon);
			w.WriteInt64(msg.refuseSess);
			w.WriteInt64(static_cast<int64_t>(msg.gateAddr));
			w.WriteInt64(static_cast<int64_t>(msg.gpAddr));
			return w;
		}

		inline WPacket serialize(const CpPingMessage& msg) {
			WPacket w(32);
			w.WriteCmd(CMD_CP_PING);
			w.WriteInt64(msg.pingValue);
			w.WriteInt64(static_cast<int64_t>(msg.gateAddr));
			w.WriteInt64(static_cast<int64_t>(msg.gpAddr));
			return w;
		}

		inline WPacket serialize(const CpRefuseToMeMessage& msg) {
			WPacket w(32);
			w.WriteCmd(CMD_CP_REFUSETOME);
			w.WriteInt64(msg.refuseFlag);
			w.WriteInt64(static_cast<int64_t>(msg.gateAddr));
			w.WriteInt64(static_cast<int64_t>(msg.gpAddr));
			return w;
		}

		inline WPacket serialize(const CpReportWgMessage& msg) {
			WPacket w(32);
			w.WriteCmd(CMD_CP_REPORT_WG);
			w.WriteInt64(static_cast<int64_t>(msg.gateAddr));
			w.WriteInt64(static_cast<int64_t>(msg.gpAddr));
			return w;
		}

		inline WPacket serialize(const CpMasterRefreshInfoMessage& msg) {
			WPacket w(32);
			w.WriteCmd(CMD_CP_MASTER_REFRESH_INFO);
			w.WriteInt64(msg.masterChaId);
			w.WriteInt64(static_cast<int64_t>(msg.gateAddr));
			w.WriteInt64(static_cast<int64_t>(msg.gpAddr));
			return w;
		}

		inline WPacket serialize(const CpPrenticeRefreshInfoMessage& msg) {
			WPacket w(32);
			w.WriteCmd(CMD_CP_PRENTICE_REFRESH_INFO);
			w.WriteInt64(msg.prenticeChaId);
			w.WriteInt64(static_cast<int64_t>(msg.gateAddr));
			w.WriteInt64(static_cast<int64_t>(msg.gpAddr));
			return w;
		}

		inline WPacket serialize(const CpChangePassMessage& msg) {
			WPacket w(128);
			w.WriteCmd(CMD_CP_CHANGEPASS);
			w.WriteString(msg.newPass);
			w.WriteString(msg.pin);
			w.WriteInt64(static_cast<int64_t>(msg.gateAddr));
			w.WriteInt64(static_cast<int64_t>(msg.gpAddr));
			return w;
		}

		// --- Группа D: MP ---

		inline WPacket serialize(const MpEnterMapMessage& msg) {
			WPacket w(32);
			w.WriteCmd(CMD_MP_ENTERMAP);
			w.WriteInt64(msg.isSwitch);
			w.WriteInt64(static_cast<int64_t>(msg.gateAddr));
			w.WriteInt64(static_cast<int64_t>(msg.gpAddr));
			return w;
		}

		inline WPacket serialize(const MpTeamCreateMessage& msg) {
			WPacket w(128);
			w.WriteCmd(CMD_MP_TEAM_CREATE);
			w.WriteString(msg.memberName);
			w.WriteString(msg.leaderName);
			w.WriteInt64(static_cast<int64_t>(msg.gateAddr));
			w.WriteInt64(static_cast<int64_t>(msg.gpAddr));
			return w;
		}

		inline WPacket serialize(const MpGuildCreateMessage& msg) {
			WPacket w(128);
			w.WriteCmd(CMD_MP_GUILD_CREATE);
			w.WriteInt64(msg.guildId);
			w.WriteString(msg.gldName);
			w.WriteString(msg.job);
			w.WriteInt64(msg.degree);
			w.WriteInt64(static_cast<int64_t>(msg.gateAddr));
			w.WriteInt64(static_cast<int64_t>(msg.gpAddr));
			return w;
		}

		inline WPacket serialize(const MpGuildApproveMessage& msg) {
			WPacket w(32);
			w.WriteCmd(CMD_MP_GUILD_APPROVE);
			w.WriteInt64(msg.newMemberChaId);
			w.WriteInt64(static_cast<int64_t>(msg.gateAddr));
			w.WriteInt64(static_cast<int64_t>(msg.gpAddr));
			return w;
		}

		inline WPacket serialize(const MpGuildKickMessage& msg) {
			WPacket w(32);
			w.WriteCmd(CMD_MP_GUILD_KICK);
			w.WriteInt64(msg.kickedChaId);
			w.WriteInt64(static_cast<int64_t>(msg.gateAddr));
			w.WriteInt64(static_cast<int64_t>(msg.gpAddr));
			return w;
		}

		inline WPacket serialize(const MpGuildLeaveMessage& msg) {
			WPacket w(32);
			w.WriteCmd(CMD_MP_GUILD_LEAVE);
			w.WriteInt64(static_cast<int64_t>(msg.gateAddr));
			w.WriteInt64(static_cast<int64_t>(msg.gpAddr));
			return w;
		}

		inline WPacket serialize(const MpGuildDisbandMessage& msg) {
			WPacket w(32);
			w.WriteCmd(CMD_MP_GUILD_DISBAND);
			w.WriteInt64(static_cast<int64_t>(msg.gateAddr));
			w.WriteInt64(static_cast<int64_t>(msg.gpAddr));
			return w;
		}

		inline WPacket serialize(const MpGuildMottoMessage& msg) {
			WPacket w(128);
			w.WriteCmd(CMD_MP_GUILD_MOTTO);
			w.WriteString(msg.motto);
			w.WriteInt64(static_cast<int64_t>(msg.gateAddr));
			w.WriteInt64(static_cast<int64_t>(msg.gpAddr));
			return w;
		}

		inline WPacket serialize(const MpGuildPermMessage& msg) {
			WPacket w(32);
			w.WriteCmd(CMD_MP_GUILD_PERM);
			w.WriteInt64(msg.targetChaId);
			w.WriteInt64(msg.permission);
			w.WriteInt64(static_cast<int64_t>(msg.gateAddr));
			w.WriteInt64(static_cast<int64_t>(msg.gpAddr));
			return w;
		}

		inline WPacket serialize(const MpGuildChallMoneyMessage& msg) {
			WPacket w(256);
			w.WriteCmd(CMD_MP_GUILD_CHALLMONEY);
			w.WriteInt64(msg.guildId);
			w.WriteInt64(msg.money);
			w.WriteString(msg.guildName1);
			w.WriteString(msg.guildName2);
			w.WriteInt64(static_cast<int64_t>(msg.gateAddr));
			w.WriteInt64(static_cast<int64_t>(msg.gpAddr));
			return w;
		}

		inline WPacket serialize(const MpGuildChallPrizeMoneyMessage& msg) {
			WPacket w(32);
			w.WriteCmd(CMD_MP_GUILD_CHALL_PRIZEMONEY);
			w.WriteInt64(msg.guildId);
			w.WriteInt64(msg.money);
			return w;
		}

		inline WPacket serialize(const MpMasterCreateMessage& msg) {
			WPacket w(128);
			w.WriteCmd(CMD_MP_MASTER_CREATE);
			w.WriteString(msg.prenticeName);
			w.WriteInt64(msg.prenticeChaid);
			w.WriteString(msg.masterName);
			w.WriteInt64(msg.masterChaid);
			return w;
		}

		inline WPacket serialize(const MpMasterDelMessage& msg) {
			WPacket w(128);
			w.WriteCmd(CMD_MP_MASTER_DEL);
			w.WriteString(msg.prenticeName);
			w.WriteInt64(msg.prenticeChaid);
			w.WriteString(msg.masterName);
			w.WriteInt64(msg.masterChaid);
			return w;
		}

		inline WPacket serialize(const MpMasterFinishMessage& msg) {
			WPacket w(16);
			w.WriteCmd(CMD_MP_MASTER_FINISH);
			w.WriteInt64(msg.prenticeChaid);
			return w;
		}

		inline WPacket serialize(const MpSay2AllMessage& msg) {
			WPacket w(256);
			w.WriteCmd(CMD_MP_SAY2ALL);
			w.WriteInt64(msg.succ);
			w.WriteString(msg.chaName);
			w.WriteString(msg.content);
			w.WriteInt64(static_cast<int64_t>(msg.gateAddr));
			w.WriteInt64(static_cast<int64_t>(msg.gpAddr));
			return w;
		}

		inline WPacket serialize(const MpSay2TradeMessage& msg) {
			WPacket w(256);
			w.WriteCmd(CMD_MP_SAY2TRADE);
			w.WriteInt64(msg.succ);
			w.WriteString(msg.chaName);
			w.WriteString(msg.content);
			w.WriteInt64(static_cast<int64_t>(msg.gateAddr));
			w.WriteInt64(static_cast<int64_t>(msg.gpAddr));
			return w;
		}

		inline WPacket serialize(const MpGm1SayMessage& msg) {
			WPacket w(256);
			w.WriteCmd(CMD_MP_GM1SAY);
			w.WriteString(msg.content);
			w.WriteInt64(static_cast<int64_t>(msg.gateAddr));
			w.WriteInt64(static_cast<int64_t>(msg.gpAddr));
			return w;
		}

		inline WPacket serialize(const MpGm1Say1Message& msg) {
			WPacket w(256);
			w.WriteCmd(CMD_MP_GM1SAY1);
			w.WriteString(msg.content);
			w.WriteInt64(msg.setNum);
			w.WriteInt64(msg.color);
			return w;
		}

		inline WPacket serialize(const MpGmBanMessage& msg) {
			WPacket w(64);
			w.WriteCmd(CMD_MP_GMBANACCOUNT);
			w.WriteString(msg.actName);
			w.WriteInt64(static_cast<int64_t>(msg.gateAddr));
			w.WriteInt64(static_cast<int64_t>(msg.gpAddr));
			return w;
		}

		inline WPacket serialize(const MpGmUnbanMessage& msg) {
			WPacket w(64);
			w.WriteCmd(CMD_MP_GMUNBANACCOUNT);
			w.WriteString(msg.actName);
			w.WriteInt64(static_cast<int64_t>(msg.gateAddr));
			w.WriteInt64(static_cast<int64_t>(msg.gpAddr));
			return w;
		}

		inline WPacket serialize(const MpGuildNoticeMessage& msg) {
			WPacket w(256);
			w.WriteCmd(CMD_MP_GUILDNOTICE);
			w.WriteInt64(msg.guildId);
			w.WriteString(msg.content);
			return w;
		}

		inline WPacket serialize(const MpCanReceiveRequestsMessage& msg) {
			WPacket w(32);
			w.WriteCmd(CMD_MP_CANRECEIVEREQUESTS);
			w.WriteInt64(msg.chaId);
			w.WriteInt64(msg.canSend);
			return w;
		}

		inline WPacket serialize(const MpMutePlayerMessage& msg) {
			WPacket w(128);
			w.WriteCmd(CMD_MP_MUTE_PLAYER);
			w.WriteString(msg.chaName);
			w.WriteInt64(msg.time);
			w.WriteInt64(static_cast<int64_t>(msg.gateAddr));
			w.WriteInt64(static_cast<int64_t>(msg.gpAddr));
			return w;
		}

		inline WPacket serialize(const MpGarner2UpdateMessage& msg) {
			WPacket w(256);
			w.WriteCmd(CMD_MP_GARNER2_UPDATE);
			w.WriteInt64(msg.nid);
			w.WriteString(msg.chaName);
			w.WriteInt64(msg.level);
			w.WriteString(msg.job);
			w.WriteInt64(msg.fightpoint);
			w.WriteInt64(static_cast<int64_t>(msg.gateAddr));
			w.WriteInt64(static_cast<int64_t>(msg.gpAddr));
			return w;
		}

		inline WPacket serialize(const MpGarner2GetOrderMessage& msg) {
			WPacket w(32);
			w.WriteCmd(CMD_MP_GARNER2_CGETORDER);
			w.WriteInt64(static_cast<int64_t>(msg.gateAddr));
			w.WriteInt64(static_cast<int64_t>(msg.gpAddr));
			return w;
		}

		inline WPacket serialize(const MpGuildBankAckMessage& msg) {
			WPacket w(32);
			w.WriteCmd(CMD_MP_GUILDBANK);
			w.WriteInt64(msg.guildId);
			w.WriteInt64(static_cast<int64_t>(msg.gateAddr));
			w.WriteInt64(static_cast<int64_t>(msg.gpAddr));
			return w;
		}

		// --- Группа E: PC ---

		inline WPacket serialize(const PcTeamInviteMessage& msg) {
			WPacket w(128);
			w.WriteCmd(CMD_PC_TEAM_INVITE);
			w.WriteString(msg.inviterName);
			w.WriteInt64(msg.chaId);
			w.WriteInt64(msg.icon);
			return w;
		}

		inline WPacket serialize(const PcTeamRefreshMessage& msg) {
			WPacket w(256);
			w.WriteCmd(CMD_PC_TEAM_REFRESH);
			w.WriteInt64(msg.msg);
			w.WriteInt64(msg.count);
			for (const auto& m : msg.members) {
				w.WriteInt64(m.chaId);
				w.WriteString(m.chaName);
				w.WriteString(m.motto);
				w.WriteInt64(m.icon);
			}
			return w;
		}

		inline WPacket serialize(const PcFrndInviteMessage& msg) {
			WPacket w(128);
			w.WriteCmd(CMD_PC_FRND_INVITE);
			w.WriteString(msg.inviterName);
			w.WriteInt64(msg.chaId);
			w.WriteInt64(msg.icon);
			return w;
		}

		inline WPacket serialize(const PcFrndRefreshMessage& msg) {
			WPacket w(256);
			w.WriteCmd(CMD_PC_FRND_REFRESH);
			w.WriteInt64(msg.msg);
			w.WriteString(msg.group);
			w.WriteInt64(msg.chaId);
			w.WriteString(msg.chaName);
			w.WriteString(msg.motto);
			w.WriteInt64(msg.icon);
			return w;
		}

		inline WPacket serialize(const PcFrndRefreshDelMessage& msg) {
			WPacket w(32);
			w.WriteCmd(CMD_PC_FRND_REFRESH);
			w.WriteInt64(msg.msg);
			w.WriteInt64(msg.chaId);
			return w;
		}

		inline WPacket serialize(const PcFrndChangeGroupMessage& msg) {
			WPacket w(64);
			w.WriteCmd(CMD_PC_FRND_CHANGE_GROUP);
			w.WriteInt64(msg.friendChaId);
			w.WriteString(msg.groupName);
			return w;
		}

		inline WPacket serialize(const PcFrndRefreshInfoMessage& msg) {
			WPacket w(256);
			w.WriteCmd(CMD_PC_FRND_REFRESH_INFO);
			w.WriteInt64(msg.chaId);
			w.WriteString(msg.motto);
			w.WriteInt64(msg.icon);
			w.WriteInt64(msg.degree);
			w.WriteString(msg.job);
			w.WriteString(msg.guildName);
			return w;
		}

		inline WPacket serialize(const PcSay2AllMessage& msg) {
			WPacket w(256);
			w.WriteCmd(CMD_PC_SAY2ALL);
			w.WriteString(msg.chaName);
			w.WriteString(msg.content);
			w.WriteInt64(msg.color);
			return w;
		}

		inline WPacket serialize(const PcSay2TradeMessage& msg) {
			WPacket w(256);
			w.WriteCmd(CMD_PC_SAY2TRADE);
			w.WriteString(msg.chaName);
			w.WriteString(msg.content);
			w.WriteInt64(msg.color);
			return w;
		}

		inline WPacket serialize(const PcSay2YouMessage& msg) {
			WPacket w(256);
			w.WriteCmd(CMD_PC_SAY2YOU);
			w.WriteString(msg.sender);
			w.WriteString(msg.target);
			w.WriteString(msg.content);
			w.WriteInt64(msg.color);
			return w;
		}

		inline WPacket serialize(const PcSay2TemMessage& msg) {
			WPacket w(256);
			w.WriteCmd(CMD_PC_SAY2TEM);
			w.WriteInt64(msg.chaId);
			w.WriteString(msg.content);
			w.WriteInt64(msg.color);
			return w;
		}

		inline WPacket serialize(const PcSay2GudMessage& msg) {
			WPacket w(256);
			w.WriteCmd(CMD_PC_SAY2GUD);
			w.WriteString(msg.chaName);
			w.WriteString(msg.content);
			w.WriteInt64(msg.color);
			return w;
		}

		inline WPacket serialize(const PcGm1SayMessage& msg) {
			WPacket w(256);
			w.WriteCmd(CMD_PC_GM1SAY);
			w.WriteString(msg.chaName);
			w.WriteString(msg.content);
			return w;
		}

		inline WPacket serialize(const PcGm1Say1Message& msg) {
			WPacket w(256);
			w.WriteCmd(CMD_PC_GM1SAY1);
			w.WriteString(msg.content);
			w.WriteInt64(msg.setNum);
			w.WriteInt64(msg.color);
			return w;
		}

		inline WPacket serialize(const PcGuildPermMessage& msg) {
			WPacket w(32);
			w.WriteCmd(CMD_PC_GUILD_PERM);
			w.WriteInt64(msg.targetChaId);
			w.WriteInt64(msg.permission);
			return w;
		}

		inline WPacket serialize(const PcMasterRefreshAddMessage& msg) {
			WPacket w(256);
			w.WriteCmd(CMD_PC_MASTER_REFRESH);
			w.WriteInt64(msg.msg);
			w.WriteString(msg.group);
			w.WriteInt64(msg.chaId);
			w.WriteString(msg.chaName);
			w.WriteString(msg.motto);
			w.WriteInt64(msg.icon);
			return w;
		}

		inline WPacket serialize(const PcMasterRefreshDelMessage& msg) {
			WPacket w(32);
			w.WriteCmd(CMD_PC_MASTER_REFRESH);
			w.WriteInt64(msg.msg);
			w.WriteInt64(msg.chaId);
			return w;
		}

		inline WPacket serialize(const PcSessCreateMessage& msg) {
			WPacket w(256);
			w.WriteCmd(CMD_PC_SESS_CREATE);
			w.WriteInt64(msg.sessId);
			w.WriteInt64(static_cast<int64_t>(msg.members.size()));
			for (const auto& m : msg.members) {
				w.WriteInt64(m.chaId);
				w.WriteString(m.chaName);
				w.WriteString(m.motto);
				w.WriteInt64(m.icon);
			}
			w.WriteInt64(msg.notiPlyCount);
			return w;
		}

		inline WPacket serialize(const PcSessAddMessage& msg) {
			WPacket w(256);
			w.WriteCmd(CMD_PC_SESS_ADD);
			w.WriteInt64(msg.sessId);
			w.WriteInt64(msg.chaId);
			w.WriteString(msg.chaName);
			w.WriteString(msg.motto);
			w.WriteInt64(msg.icon);
			return w;
		}

		inline WPacket serialize(const PcSessSayMessage& msg) {
			WPacket w(256);
			w.WriteCmd(CMD_PC_SESS_SAY);
			w.WriteInt64(msg.sessId);
			w.WriteInt64(msg.chaId);
			w.WriteString(msg.content);
			return w;
		}

		inline WPacket serialize(const PcSessLeaveMessage& msg) {
			WPacket w(32);
			w.WriteCmd(CMD_PC_SESS_LEAVE);
			w.WriteInt64(msg.sessId);
			w.WriteInt64(msg.chaId);
			return w;
		}

		inline WPacket serialize(const PcChangePersonInfoMessage& msg) {
			WPacket w(128);
			w.WriteCmd(CMD_PC_CHANGE_PERSONINFO);
			w.WriteString(msg.motto);
			w.WriteInt64(msg.icon);
			w.WriteInt64(msg.refuseSess);
			return w;
		}

		inline WPacket serialize(const PcErrMsgMessage& msg) {
			WPacket w(128);
			w.WriteCmd(CMD_PC_ERRMSG);
			w.WriteString(msg.message);
			return w;
		}

		inline WPacket serialize(const PcGuildNoticeMessage& msg) {
			WPacket w(256);
			w.WriteCmd(CMD_PC_GUILDNOTICE);
			w.WriteString(msg.content);
			return w;
		}

		inline WPacket serialize(const PcRegisterMessage& msg) {
			WPacket w(128);
			w.WriteCmd(CMD_PC_REGISTER);
			w.WriteInt64(msg.result);
			w.WriteString(msg.message);
			return w;
		}

		// --- Группа F: PM ---

		inline WPacket serialize(const PmTeamMessage& msg) {
			WPacket w(256);
			w.WriteCmd(CMD_PM_TEAM);
			w.WriteInt64(msg.msg);
			w.WriteInt64(msg.count);
			for (const auto& m : msg.members) {
				w.WriteString(m.gateName);
				w.WriteInt64(m.gtAddr);
				w.WriteInt64(m.chaId);
			}
			return w;
		}

		inline WPacket serialize(const PmExpScaleMessage& msg) {
			WPacket w(32);
			w.WriteCmd(CMD_PM_EXPSCALE);
			w.WriteInt64(msg.chaId);
			w.WriteInt64(msg.time);
			return w;
		}

		inline WPacket serialize(const PmSay2AllMessage& msg) {
			WPacket w(256);
			w.WriteCmd(CMD_PM_SAY2ALL);
			w.WriteInt64(msg.chaId);
			w.WriteString(msg.content);
			w.WriteInt64(msg.money);
			return w;
		}

		inline WPacket serialize(const PmSay2TradeMessage& msg) {
			WPacket w(256);
			w.WriteCmd(CMD_PM_SAY2TRADE);
			w.WriteInt64(msg.chaId);
			w.WriteString(msg.content);
			w.WriteInt64(msg.money);
			return w;
		}

		inline WPacket serialize(const PmGuildDisbandMessage& msg) {
			WPacket w(16);
			w.WriteCmd(CMD_PM_GUILD_DISBAND);
			w.WriteInt64(msg.guildId);
			return w;
		}

		inline WPacket serialize(const PmGuildChallMoneyMessage& msg) {
			WPacket w(256);
			w.WriteCmd(CMD_PM_GUILD_CHALLMONEY);
			w.WriteInt64(msg.leaderId);
			w.WriteInt64(msg.money);
			w.WriteString(msg.guildName1);
			w.WriteString(msg.guildName2);
			return w;
		}

		inline WPacket serialize(const PmGuildChallPrizeMoneyMessage& msg) {
			WPacket w(32);
			w.WriteCmd(CMD_PM_GUILD_CHALL_PRIZEMONEY);
			w.WriteInt64(msg.leaderId);
			w.WriteInt64(msg.money);
			return w;
		}

		inline WPacket serialize(const PtKickUserMessage& msg) {
			WPacket w(32);
			w.WriteCmd(CMD_PT_KICKUSER);
			w.WriteInt64(msg.gpAddr);
			w.WriteInt64(msg.gtAddr);
			return w;
		}

		// --- Группа H: TM/MT ---

		inline WPacket serialize(const MtLoginMessage& msg) {
			WPacket w(256);
			w.WriteCmd(CMD_MT_LOGIN);
			w.WriteString(msg.serverName);
			w.WriteString(msg.mapNameList);
			return w;
		}

		inline WPacket serialize(const TmLoginAckMessage& msg) {
			WPacket w(64);
			w.WriteCmd(CMD_TM_LOGIN_ACK);
			w.WriteInt64(static_cast<int64_t>(msg.errCode));
			if (msg.errCode == 0)
				w.WriteString(msg.gateName);
			return w;
		}

		inline WPacket serialize(const TmEnterMapMessage& msg) {
			WPacket w(256);
			w.WriteCmd(CMD_TM_ENTERMAP);
			w.WriteInt64(msg.actId);
			w.WriteString(msg.password);
			w.WriteInt64(msg.dbCharId);
			w.WriteInt64(msg.worldId);
			w.WriteString(msg.mapName);
			w.WriteInt64(msg.mapCopyNo);
			w.WriteInt64(msg.posX);
			w.WriteInt64(msg.posY);
			w.WriteInt64(msg.loginFlag);
			// Trailer (GameServer читает ReverseRead):
			w.WriteInt64(msg.winer);
			w.WriteInt64(msg.gateAddr);
			return w;
		}

		inline WPacket serialize(const TmGoOutMapMessage& msg) {
			WPacket w(32);
			w.WriteCmd(CMD_TM_GOOUTMAP);
			// Все поля GameServer читает ReverseRead, пишем в обратном порядке:
			w.WriteInt64(msg.offlineFlag);
			w.WriteInt64(msg.gateAddr);
			w.WriteInt64(msg.playerPtr);
			return w;
		}

		inline WPacket serialize(const TmChangePersonInfoMessage& msg) {
			WPacket w(128);
			w.WriteCmd(CMD_TM_CHANGE_PERSONINFO);
			w.WriteString(msg.motto);
			w.WriteInt64(msg.icon);
			return w;
		}

		inline void serializeMapEntry(WPacket& w, const MapEntryMessage& msg) {
			w.WriteString(msg.srcMapName);
			w.WriteString(msg.targetMapName);
			w.WriteInt64(msg.subType);
			switch (msg.subType) {
			case MAPENTRY_CREATE:
				w.WriteInt64(msg.posX);
				w.WriteInt64(msg.posY);
				w.WriteInt64(msg.mapCopyNum);
				w.WriteInt64(msg.copyPlayerNum);
				for (const auto& line : msg.luaScriptLines)
					w.WriteString(line);
				w.WriteInt64(static_cast<int64_t>(msg.luaScriptLines.size()));
				break;
			case MAPENTRY_DESTROY:
				break;
			case MAPENTRY_SUBPLAYER:
				w.WriteInt64(msg.copyNo);
				w.WriteInt64(msg.numPlayers);
				break;
			case MAPENTRY_SUBCOPY:
				w.WriteInt64(msg.copyNo);
				break;
			case MAPENTRY_RETURN:
				w.WriteInt64(msg.resultCode);
				if (msg.resultCode == 1) // COPY_CLOSE_SUC
					w.WriteInt64(msg.returnCopyNo);
				break;
			case MAPENTRY_COPYPARAM:
				w.WriteInt64(msg.copyId);
				for (int i = 0; i < MAPCOPY_INFO_PARAM_NUM; ++i)
					w.WriteInt64(msg.params[i]);
				break;
			case MAPENTRY_COPYRUN:
				w.WriteInt64(msg.copyId);
				w.WriteInt64(msg.condType);
				w.WriteInt64(msg.condValue);
				break;
			}
		}

		inline WPacket serialize(const MapEntryMessage& msg, uint16_t cmd) {
			WPacket w(512);
			w.WriteCmd(cmd);
			serializeMapEntry(w, msg);
			return w;
		}

		inline WPacket serialize(const TmKickChaMessage& msg) {
			WPacket w(16);
			w.WriteCmd(CMD_TM_KICKCHA);
			w.WriteInt64(msg.charDbId);
			return w;
		}

		inline WPacket serialize(const TmKickChaResponse& msg) {
			WPacket w(16);
			w.WriteInt64(msg.result);
			return w;
		}

		inline WPacket serialize(const TmOfflineModeMessage& msg) {
			WPacket w(32);
			w.WriteCmd(CMD_TM_OFFLINE_MODE);
			// GameServer читает ReverseRead, пишем в обратном порядке:
			w.WriteInt64(msg.gateAddr);
			w.WriteInt64(msg.playerPtr);
			return w;
		}

		inline WPacket serialize(const TmOfflineModeResponse& msg) {
			WPacket w(16);
			w.WriteInt64(msg.result);
			return w;
		}

		inline WPacket serialize(const MtSwitchMapMessage& msg) {
			WPacket w(256);
			w.WriteCmd(CMD_MT_SWITCHMAP);
			w.WriteString(msg.currentMapName);
			w.WriteInt64(msg.currentCopyNo);
			w.WriteInt64(msg.posX);
			w.WriteInt64(msg.posY);
			w.WriteString(msg.targetMapName);
			w.WriteInt64(msg.targetCopyNo);
			w.WriteInt64(msg.targetX);
			w.WriteInt64(msg.targetY);
			w.WriteInt64(msg.switchType);
			// ReflectINFof trailer:
			w.WriteInt64(msg.playerDBID);
			w.WriteInt64(msg.gateAddr);
			w.WriteInt64(msg.aimNum);
			return w;
		}

		inline WPacket serialize(const MtKickUserMessage& msg) {
			WPacket w(32);
			w.WriteCmd(CMD_MT_KICKUSER);
			w.WriteInt64(msg.playerDBID);
			w.WriteInt64(msg.gateAddr);
			w.WriteInt64(msg.kickFlag);
			return w;
		}

		inline WPacket serialize(const MtPlayerExitMessage& msg) {
			WPacket w(32);
			w.WriteCmd(CMD_MT_PALYEREXIT);
			// ReflectINFof trailer (payload пуст):
			w.WriteInt64(msg.playerDBID);
			w.WriteInt64(msg.gateAddr);
			w.WriteInt64(msg.aimNum);
			return w;
		}

		// --- Группа I: MM ---

		inline WPacket serialize(const MmGateConnectMessage&) {
			WPacket w(16);
			w.WriteCmd(CMD_MM_GATE_CONNECT);
			w.WriteInt64(0);
			return w;
		}

		inline WPacket serialize(const MmGateReleaseMessage&) {
			WPacket w(16);
			w.WriteCmd(CMD_MM_GATE_RELEASE);
			w.WriteInt64(0);
			return w;
		}

		inline WPacket serialize(const MmQueryChaMessage& msg) {
			WPacket w(128);
			w.WriteCmd(CMD_MM_QUERY_CHA);
			w.WriteInt64(msg.srcId);
			w.WriteString(msg.chaName);
			return w;
		}

		inline WPacket serialize(const MmQueryChaItemMessage& msg) {
			WPacket w(128);
			w.WriteCmd(CMD_MM_QUERY_CHAITEM);
			w.WriteInt64(msg.srcId);
			w.WriteString(msg.chaName);
			return w;
		}

		inline WPacket serialize(const MmCallChaMessage& msg) {
			WPacket w(256);
			w.WriteCmd(CMD_MM_CALL_CHA);
			w.WriteInt64(msg.srcId);
			w.WriteString(msg.targetName);
			w.WriteInt64(msg.isBoat);
			w.WriteString(msg.mapName);
			w.WriteInt64(msg.posX);
			w.WriteInt64(msg.posY);
			w.WriteInt64(msg.copyNo);
			return w;
		}

		inline WPacket serialize(const MmGotoChaMessage& msg) {
			WPacket w(256);
			w.WriteCmd(CMD_MM_GOTO_CHA);
			w.WriteInt64(msg.srcId);
			w.WriteString(msg.targetName);
			w.WriteInt64(msg.mode);
			if (msg.mode == 1) {
				w.WriteString(msg.srcName);
			}
			else if (msg.mode == 2) {
				w.WriteInt64(msg.isBoat);
				w.WriteString(msg.mapName);
				w.WriteInt64(msg.posX);
				w.WriteInt64(msg.posY);
				w.WriteInt64(msg.copyNo);
			}
			return w;
		}

		inline WPacket serialize(const MmKickChaMessage& msg) {
			WPacket w(128);
			w.WriteCmd(CMD_MM_KICK_CHA);
			w.WriteInt64(msg.srcId);
			w.WriteString(msg.targetName);
			w.WriteInt64(msg.kickDuration);
			return w;
		}

		inline WPacket serialize(const MmGuildRejectMessage& msg) {
			WPacket w(128);
			w.WriteCmd(CMD_MM_GUILD_REJECT);
			w.WriteInt64(msg.srcId);
			w.WriteString(msg.guildName);
			return w;
		}

		inline WPacket serialize(const MmGuildApproveMessage& msg) {
			WPacket w(256);
			w.WriteCmd(CMD_MM_GUILD_APPROVE);
			w.WriteInt64(msg.srcId);
			w.WriteInt64(msg.guildId);
			w.WriteString(msg.guildName);
			w.WriteString(msg.guildMotto);
			return w;
		}

		inline WPacket serialize(const MmGuildKickMessage& msg) {
			WPacket w(128);
			w.WriteCmd(CMD_MM_GUILD_KICK);
			w.WriteInt64(msg.srcId);
			w.WriteString(msg.guildName);
			return w;
		}

		inline WPacket serialize(const MmGuildDisbandMessage& msg) {
			WPacket w(16);
			w.WriteCmd(CMD_MM_GUILD_DISBAND);
			w.WriteInt64(msg.srcId);
			return w;
		}

		inline WPacket serialize(const MmQueryChaPingMessage& msg) {
			WPacket w(128);
			w.WriteCmd(CMD_MM_QUERY_CHAPING);
			w.WriteInt64(msg.srcId);
			w.WriteString(msg.chaName);
			return w;
		}

		inline WPacket serialize(const MmNoticeMessage& msg) {
			WPacket w(256);
			w.WriteCmd(CMD_MM_NOTICE);
			w.WriteInt64(0); // srcId = 0
			w.WriteString(msg.content);
			return w;
		}

		inline WPacket serialize(const MmGuildMottoMessage& msg) {
			WPacket w(256);
			w.WriteCmd(CMD_MM_GUILD_MOTTO);
			w.WriteInt64(msg.guildId);
			w.WriteString(msg.motto);
			return w;
		}

		inline WPacket serialize(const MmDoStringMessage& msg) {
			WPacket w(512);
			w.WriteCmd(CMD_MM_DO_STRING);
			w.WriteInt64(msg.srcId);
			w.WriteString(msg.luaCode);
			return w;
		}

		inline WPacket serialize(const MmChaNoticeMessage& msg) {
			WPacket w(256);
			w.WriteCmd(CMD_MM_CHA_NOTICE);
			w.WriteInt64(0); // srcId = 0
			w.WriteString(msg.content);
			w.WriteString(msg.chaName);
			return w;
		}

		inline WPacket serialize(const MmLoginMessage& msg) {
			WPacket w(128);
			w.WriteCmd(CMD_MM_LOGIN);
			w.WriteInt64(0); // srcId = 0
			w.WriteString(msg.chaName);
			return w;
		}

		inline WPacket serialize(const MmGuildChallPrizeMoneyMessage& msg) {
			WPacket w(32);
			w.WriteCmd(CMD_MM_GUILD_CHALL_PRIZEMONEY);
			w.WriteInt64(0); // srcId = 0
			w.WriteInt64(msg.charDbId);
			w.WriteInt64(msg.money);
			return w;
		}

		inline WPacket serialize(const MmAddCreditMessage& msg) {
			WPacket w(32);
			w.WriteCmd(CMD_MM_ADDCREDIT);
			w.WriteInt64(0); // srcId = 0
			w.WriteInt64(msg.charDbId);
			w.WriteInt64(msg.amount);
			return w;
		}

		inline WPacket serialize(const MmStoreBuyMessage& msg) {
			WPacket w(32);
			w.WriteCmd(CMD_MM_STORE_BUY);
			w.WriteInt64(0); // srcId = 0
			w.WriteInt64(msg.charDbId);
			w.WriteInt64(msg.commodityId);
			w.WriteInt64(msg.money);
			return w;
		}

		inline WPacket serialize(const MmAddMoneyMessage& msg) {
			WPacket w(32);
			w.WriteCmd(CMD_MM_ADDMONEY);
			w.WriteInt64(0); // srcId = 0
			w.WriteInt64(msg.charDbId);
			w.WriteInt64(msg.money);
			return w;
		}

		inline WPacket serialize(const MmAuctionMessage& msg) {
			WPacket w(16);
			w.WriteCmd(CMD_MM_AUCTION);
			w.WriteInt64(0); // srcId = 0
			w.WriteInt64(msg.charDbId);
			return w;
		}

		inline WPacket serialize(const MmUpdateGuildBankMessage& msg) {
			WPacket w(16);
			w.WriteCmd(CMD_MM_UPDATEGUILDBANK);
			w.WriteInt64(0); // srcId
			w.WriteInt64(msg.guildId);
			return w;
		}

		inline WPacket serialize(const MmUpdateGuildBankGoldMessage& msg) {
			WPacket w(16);
			w.WriteCmd(CMD_MM_UPDATEGUILDBANKGOLD);
			w.WriteInt64(0); // srcId
			w.WriteInt64(msg.guildId);
			return w;
		}

		// --- Группа G: CM ---

		inline WPacket serialize(const CmLoginRequest& msg) {
			WPacket w(256);
			w.WriteCmd(CMD_CM_LOGIN);
			w.WriteString(msg.acctName);
			w.WriteString(msg.passwordHash);
			w.WriteString(msg.mac);
			w.WriteInt64(msg.cheatMarker);
			w.WriteInt64(msg.clientVersion);
			return w;
		}

		inline WPacket serialize(const McLoginResponse& msg) {
			WPacket w(512);
			w.WriteCmd(CMD_MC_LOGIN);
			if (msg.data.has_value()) {
				w.WriteInt64(0); // ERR_SUCCESS
				const auto& d = msg.data.value();
				w.WriteInt64(d.maxChaNum); // maxCharacters (SC_Login)
				w.WriteInt64(static_cast<int64_t>(d.characters.size())); // charCount (ReadSelectCharacters)
				for (const auto& cha : d.characters) {
					w.WriteInt64(cha.valid ? 1 : 0);
					if (cha.valid) {
						w.WriteString(cha.chaName);
						w.WriteString(cha.job);
						w.WriteInt64(cha.degree);
						w.WriteInt64(cha.typeId);
						for (int i = 0; i < EQUIP_NUM; ++i)
							w.WriteInt64(i < static_cast<int>(cha.equipIds.size()) ? cha.equipIds[i] : 0);
					}
				}
				w.WriteInt64(d.hasPassword2 ? 1 : 0);
			}
			else {
				w.WriteInt64(static_cast<int64_t>(msg.errCode));
			}
			return w;
		}

		inline WPacket serialize(const McBgnPlayResponse& msg) {
			WPacket w(16);
			w.WriteCmd(CMD_MC_BGNPLAY);
			w.WriteInt64(static_cast<int64_t>(msg.errCode));
			return w;
		}

		inline WPacket serialize(const McEndPlayResponse& msg) {
			WPacket w(4096);
			w.WriteCmd(CMD_MC_ENDPLAY);
			w.WriteInt64(static_cast<int64_t>(msg.errCode));
			if (msg.data.has_value()) {
				const auto& d = msg.data.value();
				w.WriteInt64(d.maxChaNum);
				w.WriteInt64(static_cast<int64_t>(d.characters.size()));
				for (const auto& cha : d.characters) {
					w.WriteInt64(cha.valid ? 1 : 0);
					if (cha.valid) {
						w.WriteString(cha.chaName);
						w.WriteString(cha.job);
						w.WriteInt64(cha.degree);
						w.WriteInt64(cha.typeId);
						for (int i = 0; i < EQUIP_NUM; ++i)
							w.WriteInt64(i < static_cast<int>(cha.equipIds.size()) ? cha.equipIds[i] : 0);
					}
				}
			}
			return w;
		}

		inline WPacket serialize(const McNewChaResponse& msg) {
			WPacket w(16);
			w.WriteCmd(CMD_MC_NEWCHA);
			w.WriteInt64(static_cast<int64_t>(msg.errCode));
			return w;
		}

		inline WPacket serialize(const McDelChaResponse& msg) {
			WPacket w(16);
			w.WriteCmd(CMD_MC_DELCHA);
			w.WriteInt64(static_cast<int64_t>(msg.errCode));
			return w;
		}

		inline WPacket serialize(const McCreatePassword2Response& msg) {
			WPacket w(16);
			w.WriteCmd(CMD_MC_CREATE_PASSWORD2);
			w.WriteInt64(static_cast<int64_t>(msg.errCode));
			return w;
		}

		inline WPacket serialize(const McUpdatePassword2Response& msg) {
			WPacket w(16);
			w.WriteCmd(CMD_MC_UPDATE_PASSWORD2);
			w.WriteInt64(static_cast<int64_t>(msg.errCode));
			return w;
		}

		// --- CMD_MC_ENTERMAP: вспомогательные функции сериализации ---

		inline void serializeChaBaseInfo(WPacket& w, const ChaBaseInfo& b) {
			w.WriteInt64(b.chaId);
			w.WriteInt64(b.worldId);
			w.WriteInt64(b.commId);
			w.WriteString(b.commName);
			w.WriteInt64(b.gmLv);
			w.WriteInt64(b.handle);
			w.WriteInt64(b.ctrlType);
			w.WriteString(b.name);
			w.WriteString(b.motto);
			w.WriteInt64(b.icon);
			w.WriteInt64(b.guildId);
			w.WriteString(b.guildName);
			w.WriteString(b.guildMotto);
			w.WriteInt64(b.guildPermission);
			w.WriteString(b.stallName);
			w.WriteInt64(b.state);
			w.WriteInt64(b.posX);
			w.WriteInt64(b.posY);
			w.WriteInt64(b.radius);
			w.WriteInt64(b.angle);
			w.WriteInt64(b.teamLeaderId);
			w.WriteInt64(b.isPlayer);
			// Side
			w.WriteInt64(b.side.sideId);
			// Event
			w.WriteInt64(b.event.entityId);
			w.WriteInt64(b.event.entityType);
			w.WriteInt64(b.event.eventId);
			w.WriteString(b.event.eventName);
			// Look
			w.WriteInt64(b.look.synType);
			w.WriteInt64(b.look.typeId);
			w.WriteInt64(b.look.isBoat ? 1 : 0);
			if (b.look.isBoat) {
				w.WriteInt64(b.look.boatParts.posId);
				w.WriteInt64(b.look.boatParts.boatId);
				w.WriteInt64(b.look.boatParts.header);
				w.WriteInt64(b.look.boatParts.body);
				w.WriteInt64(b.look.boatParts.engine);
				w.WriteInt64(b.look.boatParts.cannon);
				w.WriteInt64(b.look.boatParts.equipment);
			}
			else {
				w.WriteInt64(b.look.hairId);
				for (int i = 0; i < EQUIP_NUM; ++i) {
					const auto& eq = b.look.equips[i];
					w.WriteInt64(eq.id);
					w.WriteInt64(eq.dbId);
					w.WriteInt64(eq.needLv);
					if (eq.id == 0) continue;
					if (b.look.synType == SYN_LOOK_CHANGE) {
						w.WriteInt64(eq.endure0);
						w.WriteInt64(eq.energy0);
						w.WriteInt64(eq.valid);
						w.WriteInt64(eq.tradable);
						w.WriteInt64(eq.expiration);
					}
					else {
						w.WriteInt64(eq.num);
						w.WriteInt64(eq.endure0);
						w.WriteInt64(eq.endure1);
						w.WriteInt64(eq.energy0);
						w.WriteInt64(eq.energy1);
						w.WriteInt64(eq.forgeLv);
						w.WriteInt64(eq.valid);
						w.WriteInt64(eq.tradable);
						w.WriteInt64(eq.expiration);
						w.WriteInt64(eq.hasExtra ? 1 : 0);
						if (eq.hasExtra) {
							w.WriteInt64(eq.forgeParam);
							w.WriteInt64(eq.instId);
							w.WriteInt64(eq.hasInstAttr ? 1 : 0);
							if (eq.hasInstAttr) {
								for (int j = 0; j < ITEM_INSTANCE_ATTR_NUM; ++j) {
									w.WriteInt64(eq.instAttr[j][0]);
									w.WriteInt64(eq.instAttr[j][1]);
								}
							}
						}
					}
				}
			}
			// PK ctrl
			w.WriteInt64(b.pkCtrl);
			// Append look
			for (int i = 0; i < ESPE_KBGRID_NUM; ++i) {
				w.WriteInt64(b.appendLook[i].lookId);
				if (b.appendLook[i].lookId != 0)
					w.WriteInt64(b.appendLook[i].valid);
			}
		}

		inline void serializeChaSkillBagInfo(WPacket& w, const ChaSkillBagInfo& s) {
			w.WriteInt64(s.defSkillId);
			w.WriteInt64(s.synType);
			w.WriteInt64(static_cast<int64_t>(s.skills.size()));
			for (const auto& sk : s.skills) {
				w.WriteInt64(sk.id);
				w.WriteInt64(sk.state);
				w.WriteInt64(sk.level);
				w.WriteInt64(sk.useSp);
				w.WriteInt64(sk.useEndure);
				w.WriteInt64(sk.useEnergy);
				w.WriteInt64(sk.resumeTime);
				w.WriteInt64(sk.range[0]);
				if (sk.range[0] != RANGE_TYPE_NONE)
					for (int j = 1; j < SKILL_RANGE_PARAM_NUM; ++j)
						w.WriteInt64(sk.range[j]);
			}
		}

		inline void serializeChaSkillStateInfo(WPacket& w, const ChaSkillStateInfo& s) {
			w.WriteInt64(s.currentTime);
			w.WriteInt64(static_cast<int64_t>(s.states.size()));
			for (const auto& st : s.states) {
				w.WriteInt64(st.stateId);
				w.WriteInt64(st.stateLv);
				w.WriteInt64(st.duration);
				w.WriteInt64(st.startTime);
			}
		}

		inline void serializeChaAttrInfo(WPacket& w, const ChaAttrInfo& a) {
			w.WriteInt64(a.synType);
			w.WriteInt64(static_cast<int64_t>(a.attrs.size()));
			for (const auto& attr : a.attrs) {
				w.WriteInt64(attr.attrId);
				w.WriteInt64(attr.attrVal);
			}
		}

		inline void serializeChaKitbagInfo(WPacket& w, const ChaKitbagInfo& k) {
			w.WriteInt64(k.synType);
			if (k.synType == SYN_KITBAG_INIT)
				w.WriteInt64(k.capacity);
			for (const auto& item : k.items) {
				w.WriteInt64(item.gridId);
				w.WriteInt64(item.itemId);
				if (item.itemId > 0) {
					w.WriteInt64(item.dbId);
					w.WriteInt64(item.needLv);
					w.WriteInt64(item.num);
					w.WriteInt64(item.endure0);
					w.WriteInt64(item.endure1);
					w.WriteInt64(item.energy0);
					w.WriteInt64(item.energy1);
					w.WriteInt64(item.forgeLv);
					w.WriteInt64(item.valid);
					w.WriteInt64(item.tradable);
					w.WriteInt64(item.expiration);
					w.WriteInt64(item.isBoat ? 1 : 0);
					if (item.isBoat)
						w.WriteInt64(item.boatWorldId);
					w.WriteInt64(item.forgeParam);
					w.WriteInt64(item.instId);
					w.WriteInt64(item.hasInstAttr ? 1 : 0);
					if (item.hasInstAttr) {
						for (int j = 0; j < ITEM_INSTANCE_ATTR_NUM; ++j) {
							w.WriteInt64(item.instAttr[j][0]);
							w.WriteInt64(item.instAttr[j][1]);
						}
					}
				}
			}
			w.WriteInt64(-1); // sentinel
		}

		inline void serializeChaShortcutInfo(WPacket& w, const ChaShortcutInfo& s) {
			for (int i = 0; i < SHORT_CUT_NUM; ++i) {
				w.WriteInt64(s.entries[i].type);
				w.WriteInt64(s.entries[i].gridId);
			}
		}

		// Маркеры для отладки CMD_MC_ENTERMAP — помогают найти секцию с расхождением.
		constexpr int64_t ENTERMAP_MARK_BASEINFO = 0xBEEF0001;
		constexpr int64_t ENTERMAP_MARK_SKILLBAG = 0xBEEF0002;
		constexpr int64_t ENTERMAP_MARK_SKILLSTATE = 0xBEEF0003;
		constexpr int64_t ENTERMAP_MARK_ATTR = 0xBEEF0004;
		constexpr int64_t ENTERMAP_MARK_KITBAG = 0xBEEF0005;
		constexpr int64_t ENTERMAP_MARK_SHORTCUT = 0xBEEF0006;
		constexpr int64_t ENTERMAP_MARK_BOATS = 0xBEEF0007;

		inline WPacket serialize(const McEnterMapMessage& msg) {
			WPacket w(16384);
			w.WriteCmd(CMD_MC_ENTERMAP);
			w.WriteInt64(msg.errCode);
			if (msg.data.has_value()) {
				const auto& d = msg.data.value();
				w.WriteInt64(d.autoLock);
				w.WriteInt64(d.kitbagLock);
				w.WriteInt64(d.enterType);
				w.WriteInt64(d.isNewCha);
				w.WriteString(d.mapName);
				w.WriteInt64(d.canTeam);
				w.WriteInt64(d.imp);
				serializeChaBaseInfo(w, d.baseInfo);
				w.WriteInt64(ENTERMAP_MARK_BASEINFO);
				serializeChaSkillBagInfo(w, d.skillBag);
				w.WriteInt64(ENTERMAP_MARK_SKILLBAG);
				serializeChaSkillStateInfo(w, d.skillState);
				w.WriteInt64(ENTERMAP_MARK_SKILLSTATE);
				serializeChaAttrInfo(w, d.attr);
				w.WriteInt64(ENTERMAP_MARK_ATTR);
				serializeChaKitbagInfo(w, d.kitbag);
				w.WriteInt64(ENTERMAP_MARK_KITBAG);
				serializeChaShortcutInfo(w, d.shortcut);
				w.WriteInt64(ENTERMAP_MARK_SHORTCUT);
				w.WriteInt64(static_cast<int64_t>(d.boats.size()));
				for (const auto& boat : d.boats) {
					serializeChaBaseInfo(w, boat.baseInfo);
					serializeChaAttrInfo(w, boat.attr);
					serializeChaKitbagInfo(w, boat.kitbag);
					serializeChaSkillStateInfo(w, boat.skillState);
				}
				w.WriteInt64(ENTERMAP_MARK_BOATS);
				w.WriteInt64(d.ctrlChaId);
				// loginFlag, playerCount, playerAddr — НЕ часть сообщения клиенту.
				// Дописываются через serializeEnterMapTrailer() перед ReflectINFof.
				// GateServer срезает их через DiscardLast(6).
			}
			return w;
		}

		/// Дописать trailing-поля MC_ENTERMAP для GateServer (после serialize, перед ReflectINFof).
		inline void serializeEnterMapTrailer(WPacket& w, const McEnterMapTrailer& t) {
			w.WriteInt64(t.loginFlag);
			w.WriteInt64(t.playerCount);
			w.WriteInt64(t.playerAddr);
		}

		// =================================================================
		//  Deserialize — функции десериализации из RPacket в структуры
		// =================================================================

		// --- Группа A: TP/PT ---

		inline void deserialize(RPacket& pk, TpLoginRequest& r) {
			r.protocolVersion = static_cast<int16_t>(pk.ReadInt64());
			r.gateName = pk.ReadString();
		}

		inline void deserialize(RPacket& pk, TpLoginResponse& r) {
			r.errCode = static_cast<int16_t>(pk.ReadInt64());
		}

		inline void deserialize(RPacket& pk, TpUserLoginRequest& r) {
			r.acctName = pk.ReadString();
			r.acctPassword = pk.ReadString();
			r.acctMac = pk.ReadString();
			r.clientIp = static_cast<uint32_t>(pk.ReadInt64());
			r.gateAddr = static_cast<uint32_t>(pk.ReadInt64());
			r.cheatMarker = pk.ReadInt64();
		}

		inline void deserialize(RPacket& pk, TpUserLoginResponse& r) {
			r.errCode = static_cast<int16_t>(pk.ReadInt64());
			if (r.errCode == 0) {
				TpUserLoginResponseData d;
				d.maxChaNum = pk.ReadInt64();
				d.characters.resize(static_cast<size_t>(d.maxChaNum));
				for (int64_t i = 0; i < d.maxChaNum; ++i) {
					ChaSlotData cha;
					cha.valid = pk.ReadInt64() != 0;
					if (cha.valid) {
						cha.chaName = pk.ReadString();
						cha.job = pk.ReadString();
						cha.degree = pk.ReadInt64();
						cha.typeId = pk.ReadInt64();
						cha.equipIds.resize(EQUIP_NUM);
						for (auto& eq : cha.equipIds) eq = pk.ReadInt64();
					}
					else {
						cha.degree = 0;
						cha.typeId = 0;
					}
					d.characters[static_cast<size_t>(i)] = std::move(cha);
				}
				d.hasPassword2 = pk.ReadInt64() != 0;
				d.acctId = pk.ReadInt64();
				d.acctLoginId = pk.ReadInt64();
				d.gpAddr = static_cast<uint32_t>(pk.ReadInt64());
				r.data = std::move(d);
			}
		}

		inline void deserialize(RPacket& pk, TpUserLogoutRequest& r) {
			r.gateAddr = static_cast<uint32_t>(pk.ReadInt64());
			r.gpAddr = static_cast<uint32_t>(pk.ReadInt64());
		}

		inline void deserialize(RPacket& pk, TpUserLogoutResponse& r) {
			r.errCode = static_cast<int16_t>(pk.ReadInt64());
		}

		inline void deserialize(RPacket& pk, TpBgnPlayRequest& r) {
			r.chaIndex = pk.ReadInt64();
			r.gateAddr = static_cast<uint32_t>(pk.ReadInt64());
			r.gpAddr = static_cast<uint32_t>(pk.ReadInt64());
		}

		inline void deserialize(RPacket& pk, TpBgnPlayResponse& r) {
			r.errCode = static_cast<int16_t>(pk.ReadInt64());
			if (r.errCode == 0) {
				TpBgnPlayResponseData d;
				d.password2 = pk.ReadString();
				d.chaId = pk.ReadInt64();
				d.worldId = pk.ReadInt64();
				d.mapName = pk.ReadString();
				d.swiner = pk.ReadInt64();
				r.data = std::move(d);
			}
		}

		inline void deserialize(RPacket& pk, TpEndPlayRequest& r) {
			r.gateAddr = static_cast<uint32_t>(pk.ReadInt64());
			r.gpAddr = static_cast<uint32_t>(pk.ReadInt64());
		}

		inline void deserialize(RPacket& pk, TpEndPlayResponse& r) {
			r.errCode = static_cast<int16_t>(pk.ReadInt64());
			if (r.errCode == 0) {
				TpEndPlayResponseData d;
				d.maxChaNum = pk.ReadInt64();
				d.chaNum = pk.ReadInt64();
				d.characters.resize(static_cast<size_t>(d.maxChaNum));
				for (int64_t i = 0; i < d.maxChaNum; ++i) {
					ChaSlotData cha;
					cha.valid = pk.ReadInt64() != 0;
					if (cha.valid) {
						cha.chaName = pk.ReadString();
						cha.job = pk.ReadString();
						cha.degree = pk.ReadInt64();
						cha.typeId = pk.ReadInt64();
						cha.equipIds.resize(EQUIP_NUM);
						for (auto& eq : cha.equipIds) eq = pk.ReadInt64();
					}
					else {
						cha.degree = 0;
						cha.typeId = 0;
					}
					d.characters[static_cast<size_t>(i)] = std::move(cha);
				}
				r.data = std::move(d);
			}
		}

		inline void deserialize(RPacket& pk, TpNewChaRequest& r) {
			r.chaName = pk.ReadString();
			r.birth = pk.ReadString();
			r.typeId = pk.ReadInt64();
			r.hairId = pk.ReadInt64();
			r.faceId = pk.ReadInt64();
			r.gateAddr = static_cast<uint32_t>(pk.ReadInt64());
			r.gpAddr = static_cast<uint32_t>(pk.ReadInt64());
		}

		inline void deserialize(RPacket& pk, TpNewChaResponse& r) {
			r.errCode = static_cast<int16_t>(pk.ReadInt64());
		}

		inline void deserialize(RPacket& pk, TpDelChaRequest& r) {
			r.chaIndex = pk.ReadInt64();
			r.password2 = pk.ReadString();
			r.gateAddr = static_cast<uint32_t>(pk.ReadInt64());
			r.gpAddr = static_cast<uint32_t>(pk.ReadInt64());
		}

		inline void deserialize(RPacket& pk, TpDelChaResponse& r) {
			r.errCode = static_cast<int16_t>(pk.ReadInt64());
		}

		inline void deserialize(RPacket& pk, TpChangePassRequest& r) {
			r.newPass = pk.ReadString();
			r.pin = pk.ReadString();
			r.gateAddr = static_cast<uint32_t>(pk.ReadInt64());
			r.gpAddr = static_cast<uint32_t>(pk.ReadInt64());
		}

		inline void deserialize(RPacket& pk, TpRegisterRequest& r) {
			r.username = pk.ReadString();
			r.password = pk.ReadString();
			r.email = pk.ReadString();
		}

		inline void deserialize(RPacket& pk, TpRegisterResponse& r) {
			r.result = pk.ReadInt64();
			r.message = pk.ReadString();
		}

		inline void deserialize(RPacket& pk, TpCreatePassword2Request& r) {
			r.password2 = pk.ReadString();
			r.gateAddr = static_cast<uint32_t>(pk.ReadInt64());
			r.gpAddr = static_cast<uint32_t>(pk.ReadInt64());
		}

		inline void deserialize(RPacket& pk, TpUpdatePassword2Request& r) {
			r.oldPassword2 = pk.ReadString();
			r.newPassword2 = pk.ReadString();
			r.gateAddr = static_cast<uint32_t>(pk.ReadInt64());
			r.gpAddr = static_cast<uint32_t>(pk.ReadInt64());
		}

		inline void deserialize(RPacket& pk, TpPassword2Response& r) {
			r.errCode = static_cast<int16_t>(pk.ReadInt64());
		}

		inline void deserialize(RPacket& pk, TpReqPlyLstResponse& r, int count) {
			r.entries.resize(static_cast<size_t>(count));
			for (int i = 0; i < count; ++i) {
				r.entries[i].gtAddr = pk.ReadInt64();
				r.entries[i].chaId = pk.ReadInt64();
			}
			r.plyNum = pk.ReadInt64();
		}

		inline void deserialize(RPacket& pk, TpSyncPlyLstRequest& r) {
			r.num = pk.ReadInt64();
			r.gateName = pk.ReadString();
			r.players.resize(static_cast<size_t>(r.num));
			for (int64_t i = 0; i < r.num; ++i) {
				r.players[static_cast<size_t>(i)].gtAddr = pk.ReadInt64();
				r.players[static_cast<size_t>(i)].acctLoginId = pk.ReadInt64();
				r.players[static_cast<size_t>(i)].acctId = pk.ReadInt64();
			}
		}

		inline void deserialize(RPacket& pk, TpSyncPlyLstResponse& r) {
			r.errCode = static_cast<int16_t>(pk.ReadInt64());
			r.num = pk.ReadInt64();
			r.results.resize(static_cast<size_t>(r.num));
			for (int64_t i = 0; i < r.num; ++i) {
				r.results[static_cast<size_t>(i)].ok = pk.ReadInt64() != 0;
				r.results[static_cast<size_t>(i)].playerPtr = pk.ReadInt64();
			}
		}

		inline void deserialize(RPacket& pk, OsLoginRequest& r) {
			r.version = pk.ReadInt64();
			r.agentName = pk.ReadString();
		}

		inline void deserialize(RPacket& pk, OsLoginResponse& r) {
			r.errCode = static_cast<int16_t>(pk.ReadInt64());
		}

		inline void deserialize(RPacket& pk, TpDiscMessage& r) {
			r.actId = pk.ReadInt64();
			r.ipAddr = pk.ReadInt64();
			r.reason = pk.ReadString();
		}

		// --- Группа B: PA/AP ---

		inline void deserialize(RPacket& pk, PaLoginRequest& r) {
			r.serverName = pk.ReadString();
			r.serverPassword = pk.ReadString();
		}

		inline void deserialize(RPacket& pk, PaLoginResponse& r) {
			r.errCode = static_cast<int16_t>(pk.ReadInt64());
		}

		inline void deserialize(RPacket& pk, PaUserLoginRequest& r) {
			r.username = pk.ReadString();
			r.password = pk.ReadString();
			r.mac = pk.ReadString();
			r.clientIp = pk.ReadInt64();
		}

		inline void deserialize(RPacket& pk, PaUserLoginResponse& r) {
			r.errCode = static_cast<int16_t>(pk.ReadInt64());
			if (r.errCode == 0) {
				PaUserLoginResponseData d;
				d.acctLoginId = pk.ReadInt64();
				d.sessId = pk.ReadInt64();
				d.acctId = pk.ReadInt64();
				d.gmLevel = pk.ReadInt64();
				r.data = std::move(d);
			}
		}

		inline void deserialize(RPacket& pk, PaUserLogoutMessage& r) {
			r.acctLoginId = pk.ReadInt64();
		}

		inline void deserialize(RPacket& pk, PaUserBillBgnMessage& r) {
			r.acctName = pk.ReadString();
			r.passport = pk.ReadString();
		}

		inline void deserialize(RPacket& pk, PaUserBillEndMessage& r) {
			r.acctName = pk.ReadString();
		}

		inline void deserialize(RPacket& pk, PaChangePassMessage& r) {
			r.username = pk.ReadString();
			r.newPassword = pk.ReadString();
		}

		inline void deserialize(RPacket& pk, PaRegisterMessage& r) {
			r.username = pk.ReadString();
			r.password = pk.ReadString();
			r.email = pk.ReadString();
		}

		inline void deserialize(RPacket& pk, PaGmBanMessage& r) {
			r.actName = pk.ReadString();
		}

		inline void deserialize(RPacket& pk, PaGmUnbanMessage& r) {
			r.actName = pk.ReadString();
		}

		inline void deserialize(RPacket& pk, PaUserDisableMessage& r) {
			r.acctLoginId = pk.ReadInt64();
			r.minutes = pk.ReadInt64();
		}

		inline void deserialize(RPacket& pk, ApKickUserMessage& r) {
			r.errCode = pk.ReadInt64();
			r.accountId = pk.ReadInt64();
		}

		inline void deserialize(RPacket& pk, ApExpScaleMessage& r) {
			r.chaId = pk.ReadInt64();
			r.time = pk.ReadInt64();
		}

		// --- Группа C: CP ---

		inline void deserialize(RPacket& pk, CpTeamInviteMessage& r) {
			r.invitedName = pk.ReadString();
			r.gateAddr = static_cast<uint32_t>(pk.ReadInt64());
			r.gpAddr = static_cast<uint32_t>(pk.ReadInt64());
		}

		inline void deserialize(RPacket& pk, CpTeamAcceptMessage& r) {
			r.inviterChaId = pk.ReadInt64();
			r.gateAddr = static_cast<uint32_t>(pk.ReadInt64());
			r.gpAddr = static_cast<uint32_t>(pk.ReadInt64());
		}

		inline void deserialize(RPacket& pk, CpTeamRefuseMessage& r) {
			r.inviterChaId = pk.ReadInt64();
			r.gateAddr = static_cast<uint32_t>(pk.ReadInt64());
			r.gpAddr = static_cast<uint32_t>(pk.ReadInt64());
		}

		inline void deserialize(RPacket& pk, CpTeamLeaveMessage& r) {
			r.gateAddr = static_cast<uint32_t>(pk.ReadInt64());
			r.gpAddr = static_cast<uint32_t>(pk.ReadInt64());
		}

		inline void deserialize(RPacket& pk, CpTeamKickMessage& r) {
			r.kickedChaId = pk.ReadInt64();
			r.gateAddr = static_cast<uint32_t>(pk.ReadInt64());
			r.gpAddr = static_cast<uint32_t>(pk.ReadInt64());
		}

		inline void deserialize(RPacket& pk, CpFrndInviteMessage& r) {
			r.invitedName = pk.ReadString();
			r.gateAddr = static_cast<uint32_t>(pk.ReadInt64());
			r.gpAddr = static_cast<uint32_t>(pk.ReadInt64());
		}

		inline void deserialize(RPacket& pk, CpFrndAcceptMessage& r) {
			r.inviterChaId = pk.ReadInt64();
			r.gateAddr = static_cast<uint32_t>(pk.ReadInt64());
			r.gpAddr = static_cast<uint32_t>(pk.ReadInt64());
		}

		inline void deserialize(RPacket& pk, CpFrndRefuseMessage& r) {
			r.inviterChaId = pk.ReadInt64();
			r.gateAddr = static_cast<uint32_t>(pk.ReadInt64());
			r.gpAddr = static_cast<uint32_t>(pk.ReadInt64());
		}

		inline void deserialize(RPacket& pk, CpFrndDeleteMessage& r) {
			r.deletedChaId = pk.ReadInt64();
			r.gateAddr = static_cast<uint32_t>(pk.ReadInt64());
			r.gpAddr = static_cast<uint32_t>(pk.ReadInt64());
		}

		inline void deserialize(RPacket& pk, CpFrndChangeGroupMessage& r) {
			r.friendChaId = pk.ReadInt64();
			r.groupName = pk.ReadString();
			r.gateAddr = static_cast<uint32_t>(pk.ReadInt64());
			r.gpAddr = static_cast<uint32_t>(pk.ReadInt64());
		}

		inline void deserialize(RPacket& pk, CpFrndRefreshInfoMessage& r) {
			r.friendChaId = pk.ReadInt64();
			r.gateAddr = static_cast<uint32_t>(pk.ReadInt64());
			r.gpAddr = static_cast<uint32_t>(pk.ReadInt64());
		}

		inline void deserialize(RPacket& pk, CpSay2AllMessage& r) {
			r.content = pk.ReadString();
			r.gateAddr = static_cast<uint32_t>(pk.ReadInt64());
			r.gpAddr = static_cast<uint32_t>(pk.ReadInt64());
		}

		inline void deserialize(RPacket& pk, CpSay2TradeMessage& r) {
			r.content = pk.ReadString();
			r.gateAddr = static_cast<uint32_t>(pk.ReadInt64());
			r.gpAddr = static_cast<uint32_t>(pk.ReadInt64());
		}

		inline void deserialize(RPacket& pk, CpSay2YouMessage& r) {
			r.targetName = pk.ReadString();
			r.content = pk.ReadString();
			r.gateAddr = static_cast<uint32_t>(pk.ReadInt64());
			r.gpAddr = static_cast<uint32_t>(pk.ReadInt64());
		}

		inline void deserialize(RPacket& pk, CpSay2TemMessage& r) {
			r.content = pk.ReadString();
			r.gateAddr = static_cast<uint32_t>(pk.ReadInt64());
			r.gpAddr = static_cast<uint32_t>(pk.ReadInt64());
		}

		inline void deserialize(RPacket& pk, CpSay2GudMessage& r) {
			r.content = pk.ReadString();
			r.gateAddr = static_cast<uint32_t>(pk.ReadInt64());
			r.gpAddr = static_cast<uint32_t>(pk.ReadInt64());
		}

		inline void deserialize(RPacket& pk, CpGm1SayMessage& r) {
			r.content = pk.ReadString();
			r.gateAddr = static_cast<uint32_t>(pk.ReadInt64());
			r.gpAddr = static_cast<uint32_t>(pk.ReadInt64());
		}

		inline void deserialize(RPacket& pk, CpGm1Say1Message& r) {
			r.content = pk.ReadString();
			r.color = pk.ReadInt64();
			r.gateAddr = static_cast<uint32_t>(pk.ReadInt64());
			r.gpAddr = static_cast<uint32_t>(pk.ReadInt64());
		}

		inline void deserialize(RPacket& pk, CpSessCreateMessage& r) {
			r.chaNum = pk.ReadInt64();
			r.chaNames.resize(static_cast<size_t>(r.chaNum));
			for (int64_t i = 0; i < r.chaNum; ++i) {
				r.chaNames[static_cast<size_t>(i)] = pk.ReadString();
			}
			r.gateAddr = static_cast<uint32_t>(pk.ReadInt64());
			r.gpAddr = static_cast<uint32_t>(pk.ReadInt64());
		}

		inline void deserialize(RPacket& pk, CpSessAddMessage& r) {
			r.sessId = pk.ReadInt64();
			r.chaName = pk.ReadString();
			r.gateAddr = static_cast<uint32_t>(pk.ReadInt64());
			r.gpAddr = static_cast<uint32_t>(pk.ReadInt64());
		}

		inline void deserialize(RPacket& pk, CpSessSayMessage& r) {
			r.sessId = pk.ReadInt64();
			r.content = pk.ReadString();
			r.gateAddr = static_cast<uint32_t>(pk.ReadInt64());
			r.gpAddr = static_cast<uint32_t>(pk.ReadInt64());
		}

		inline void deserialize(RPacket& pk, CpSessLeaveMessage& r) {
			r.sessId = pk.ReadInt64();
			r.gateAddr = static_cast<uint32_t>(pk.ReadInt64());
			r.gpAddr = static_cast<uint32_t>(pk.ReadInt64());
		}

		inline void deserialize(RPacket& pk, CpChangePersonInfoMessage& r) {
			r.motto = pk.ReadString();
			r.icon = pk.ReadInt64();
			r.refuseSess = pk.ReadInt64();
			r.gateAddr = static_cast<uint32_t>(pk.ReadInt64());
			r.gpAddr = static_cast<uint32_t>(pk.ReadInt64());
		}

		inline void deserialize(RPacket& pk, CpPingMessage& r) {
			r.pingValue = pk.ReadInt64();
			r.gateAddr = static_cast<uint32_t>(pk.ReadInt64());
			r.gpAddr = static_cast<uint32_t>(pk.ReadInt64());
		}

		inline void deserialize(RPacket& pk, CpRefuseToMeMessage& r) {
			r.refuseFlag = pk.ReadInt64();
			r.gateAddr = static_cast<uint32_t>(pk.ReadInt64());
			r.gpAddr = static_cast<uint32_t>(pk.ReadInt64());
		}

		inline void deserialize(RPacket& pk, CpReportWgMessage& r) {
			r.gateAddr = static_cast<uint32_t>(pk.ReadInt64());
			r.gpAddr = static_cast<uint32_t>(pk.ReadInt64());
		}

		inline void deserialize(RPacket& pk, CpMasterRefreshInfoMessage& r) {
			r.masterChaId = pk.ReadInt64();
			r.gateAddr = static_cast<uint32_t>(pk.ReadInt64());
			r.gpAddr = static_cast<uint32_t>(pk.ReadInt64());
		}

		inline void deserialize(RPacket& pk, CpPrenticeRefreshInfoMessage& r) {
			r.prenticeChaId = pk.ReadInt64();
			r.gateAddr = static_cast<uint32_t>(pk.ReadInt64());
			r.gpAddr = static_cast<uint32_t>(pk.ReadInt64());
		}

		inline void deserialize(RPacket& pk, CpChangePassMessage& r) {
			r.newPass = pk.ReadString();
			r.pin = pk.ReadString();
			r.gateAddr = static_cast<uint32_t>(pk.ReadInt64());
			r.gpAddr = static_cast<uint32_t>(pk.ReadInt64());
		}

		// --- Группа D: MP ---

		inline void deserialize(RPacket& pk, MpEnterMapMessage& r) {
			r.isSwitch = pk.ReadInt64();
			r.gateAddr = static_cast<uint32_t>(pk.ReadInt64());
			r.gpAddr = static_cast<uint32_t>(pk.ReadInt64());
		}

		inline void deserialize(RPacket& pk, MpTeamCreateMessage& r) {
			r.memberName = pk.ReadString();
			r.leaderName = pk.ReadString();
			r.gateAddr = static_cast<uint32_t>(pk.ReadInt64());
			r.gpAddr = static_cast<uint32_t>(pk.ReadInt64());
		}

		inline void deserialize(RPacket& pk, MpGuildCreateMessage& r) {
			r.guildId = pk.ReadInt64();
			r.gldName = pk.ReadString();
			r.job = pk.ReadString();
			r.degree = pk.ReadInt64();
			r.gateAddr = static_cast<uint32_t>(pk.ReadInt64());
			r.gpAddr = static_cast<uint32_t>(pk.ReadInt64());
		}

		inline void deserialize(RPacket& pk, MpGuildApproveMessage& r) {
			r.newMemberChaId = pk.ReadInt64();
			r.gateAddr = static_cast<uint32_t>(pk.ReadInt64());
			r.gpAddr = static_cast<uint32_t>(pk.ReadInt64());
		}

		inline void deserialize(RPacket& pk, MpGuildKickMessage& r) {
			r.kickedChaId = pk.ReadInt64();
			r.gateAddr = static_cast<uint32_t>(pk.ReadInt64());
			r.gpAddr = static_cast<uint32_t>(pk.ReadInt64());
		}

		inline void deserialize(RPacket& pk, MpGuildLeaveMessage& r) {
			r.gateAddr = static_cast<uint32_t>(pk.ReadInt64());
			r.gpAddr = static_cast<uint32_t>(pk.ReadInt64());
		}

		inline void deserialize(RPacket& pk, MpGuildDisbandMessage& r) {
			r.gateAddr = static_cast<uint32_t>(pk.ReadInt64());
			r.gpAddr = static_cast<uint32_t>(pk.ReadInt64());
		}

		inline void deserialize(RPacket& pk, MpGuildMottoMessage& r) {
			r.motto = pk.ReadString();
			r.gateAddr = static_cast<uint32_t>(pk.ReadInt64());
			r.gpAddr = static_cast<uint32_t>(pk.ReadInt64());
		}

		inline void deserialize(RPacket& pk, MpGuildPermMessage& r) {
			r.targetChaId = pk.ReadInt64();
			r.permission = pk.ReadInt64();
			r.gateAddr = static_cast<uint32_t>(pk.ReadInt64());
			r.gpAddr = static_cast<uint32_t>(pk.ReadInt64());
		}

		inline void deserialize(RPacket& pk, MpGuildChallMoneyMessage& r) {
			r.guildId = pk.ReadInt64();
			r.money = pk.ReadInt64();
			r.guildName1 = pk.ReadString();
			r.guildName2 = pk.ReadString();
			r.gateAddr = static_cast<uint32_t>(pk.ReadInt64());
			r.gpAddr = static_cast<uint32_t>(pk.ReadInt64());
		}

		inline void deserialize(RPacket& pk, MpGuildChallPrizeMoneyMessage& r) {
			r.guildId = pk.ReadInt64();
			r.money = pk.ReadInt64();
		}

		inline void deserialize(RPacket& pk, MpMasterCreateMessage& r) {
			r.prenticeName = pk.ReadString();
			r.prenticeChaid = pk.ReadInt64();
			r.masterName = pk.ReadString();
			r.masterChaid = pk.ReadInt64();
		}

		inline void deserialize(RPacket& pk, MpMasterDelMessage& r) {
			r.prenticeName = pk.ReadString();
			r.prenticeChaid = pk.ReadInt64();
			r.masterName = pk.ReadString();
			r.masterChaid = pk.ReadInt64();
		}

		inline void deserialize(RPacket& pk, MpMasterFinishMessage& r) {
			r.prenticeChaid = pk.ReadInt64();
		}

		inline void deserialize(RPacket& pk, MpSay2AllMessage& r) {
			r.succ = pk.ReadInt64();
			r.chaName = pk.ReadString();
			r.content = pk.ReadString();
			r.gateAddr = static_cast<uint32_t>(pk.ReadInt64());
			r.gpAddr = static_cast<uint32_t>(pk.ReadInt64());
		}

		inline void deserialize(RPacket& pk, MpSay2TradeMessage& r) {
			r.succ = pk.ReadInt64();
			r.chaName = pk.ReadString();
			r.content = pk.ReadString();
			r.gateAddr = static_cast<uint32_t>(pk.ReadInt64());
			r.gpAddr = static_cast<uint32_t>(pk.ReadInt64());
		}

		inline void deserialize(RPacket& pk, MpGm1SayMessage& r) {
			r.content = pk.ReadString();
			r.gateAddr = static_cast<uint32_t>(pk.ReadInt64());
			r.gpAddr = static_cast<uint32_t>(pk.ReadInt64());
		}

		inline void deserialize(RPacket& pk, MpGm1Say1Message& r) {
			r.content = pk.ReadString();
			r.setNum = pk.ReadInt64();
			r.color = pk.ReadInt64();
		}

		inline void deserialize(RPacket& pk, MpGmBanMessage& r) {
			r.actName = pk.ReadString();
			r.gateAddr = static_cast<uint32_t>(pk.ReadInt64());
			r.gpAddr = static_cast<uint32_t>(pk.ReadInt64());
		}

		inline void deserialize(RPacket& pk, MpGmUnbanMessage& r) {
			r.actName = pk.ReadString();
			r.gateAddr = static_cast<uint32_t>(pk.ReadInt64());
			r.gpAddr = static_cast<uint32_t>(pk.ReadInt64());
		}

		inline void deserialize(RPacket& pk, MpGuildNoticeMessage& r) {
			r.guildId = pk.ReadInt64();
			r.content = pk.ReadString();
		}

		inline void deserialize(RPacket& pk, MpCanReceiveRequestsMessage& r) {
			r.chaId = pk.ReadInt64();
			r.canSend = pk.ReadInt64();
		}

		inline void deserialize(RPacket& pk, MpMutePlayerMessage& r) {
			r.chaName = pk.ReadString();
			r.time = pk.ReadInt64();
			r.gateAddr = static_cast<uint32_t>(pk.ReadInt64());
			r.gpAddr = static_cast<uint32_t>(pk.ReadInt64());
		}

		inline void deserialize(RPacket& pk, MpGarner2UpdateMessage& r) {
			r.nid = pk.ReadInt64();
			r.chaName = pk.ReadString();
			r.level = pk.ReadInt64();
			r.job = pk.ReadString();
			r.fightpoint = pk.ReadInt64();
			r.gateAddr = static_cast<uint32_t>(pk.ReadInt64());
			r.gpAddr = static_cast<uint32_t>(pk.ReadInt64());
		}

		inline void deserialize(RPacket& pk, MpGarner2GetOrderMessage& r) {
			r.gateAddr = static_cast<uint32_t>(pk.ReadInt64());
			r.gpAddr = static_cast<uint32_t>(pk.ReadInt64());
		}

		inline void deserialize(RPacket& pk, MpGuildBankAckMessage& r) {
			r.guildId = pk.ReadInt64();
			r.gateAddr = static_cast<uint32_t>(pk.ReadInt64());
			r.gpAddr = static_cast<uint32_t>(pk.ReadInt64());
		}

		// --- Группа E: PC ---

		inline void deserialize(RPacket& pk, PcTeamInviteMessage& r) {
			r.inviterName = pk.ReadString();
			r.chaId = pk.ReadInt64();
			r.icon = pk.ReadInt64();
		}

		inline void deserialize(RPacket& pk, PcTeamRefreshMessage& r) {
			r.msg = pk.ReadInt64();
			r.count = pk.ReadInt64();
			r.members.resize(static_cast<size_t>(r.count));
			for (int64_t i = 0; i < r.count; ++i) {
				r.members[static_cast<size_t>(i)].chaId = pk.ReadInt64();
				r.members[static_cast<size_t>(i)].chaName = pk.ReadString();
				r.members[static_cast<size_t>(i)].motto = pk.ReadString();
				r.members[static_cast<size_t>(i)].icon = pk.ReadInt64();
			}
		}

		inline void deserialize(RPacket& pk, PcFrndInviteMessage& r) {
			r.inviterName = pk.ReadString();
			r.chaId = pk.ReadInt64();
			r.icon = pk.ReadInt64();
		}

		inline void deserialize(RPacket& pk, PcFrndRefreshMessage& r) {
			r.msg = pk.ReadInt64();
			r.group = pk.ReadString();
			r.chaId = pk.ReadInt64();
			r.chaName = pk.ReadString();
			r.motto = pk.ReadString();
			r.icon = pk.ReadInt64();
		}

		inline void deserialize(RPacket& pk, PcFrndRefreshDelMessage& r) {
			r.msg = pk.ReadInt64();
			r.chaId = pk.ReadInt64();
		}

		inline void deserialize(RPacket& pk, PcFrndChangeGroupMessage& r) {
			r.friendChaId = pk.ReadInt64();
			r.groupName = pk.ReadString();
		}

		inline void deserialize(RPacket& pk, PcFrndRefreshInfoMessage& r) {
			r.chaId = pk.ReadInt64();
			r.motto = pk.ReadString();
			r.icon = pk.ReadInt64();
			r.degree = pk.ReadInt64();
			r.job = pk.ReadString();
			r.guildName = pk.ReadString();
		}

		inline void deserialize(RPacket& pk, PcSay2AllMessage& r) {
			r.chaName = pk.ReadString();
			r.content = pk.ReadString();
			r.color = pk.ReadInt64();
		}

		inline void deserialize(RPacket& pk, PcSay2TradeMessage& r) {
			r.chaName = pk.ReadString();
			r.content = pk.ReadString();
			r.color = pk.ReadInt64();
		}

		inline void deserialize(RPacket& pk, PcSay2YouMessage& r) {
			r.sender = pk.ReadString();
			r.target = pk.ReadString();
			r.content = pk.ReadString();
			r.color = pk.ReadInt64();
		}

		inline void deserialize(RPacket& pk, PcSay2TemMessage& r) {
			r.chaId = pk.ReadInt64();
			r.content = pk.ReadString();
			r.color = pk.ReadInt64();
		}

		inline void deserialize(RPacket& pk, PcSay2GudMessage& r) {
			r.chaName = pk.ReadString();
			r.content = pk.ReadString();
			r.color = pk.ReadInt64();
		}

		inline void deserialize(RPacket& pk, PcGm1SayMessage& r) {
			r.chaName = pk.ReadString();
			r.content = pk.ReadString();
		}

		inline void deserialize(RPacket& pk, PcGm1Say1Message& r) {
			r.content = pk.ReadString();
			r.setNum = pk.ReadInt64();
			r.color = pk.ReadInt64();
		}

		inline void deserialize(RPacket& pk, PcGuildPermMessage& r) {
			r.targetChaId = pk.ReadInt64();
			r.permission = pk.ReadInt64();
		}

		inline void deserialize(RPacket& pk, PcMasterRefreshAddMessage& r) {
			r.msg = pk.ReadInt64();
			r.group = pk.ReadString();
			r.chaId = pk.ReadInt64();
			r.chaName = pk.ReadString();
			r.motto = pk.ReadString();
			r.icon = pk.ReadInt64();
		}

		inline void deserialize(RPacket& pk, PcMasterRefreshDelMessage& r) {
			r.msg = pk.ReadInt64();
			r.chaId = pk.ReadInt64();
		}

		inline void deserialize(RPacket& pk, PcSessCreateMessage& r) {
			r.sessId = pk.ReadInt64();
			int64_t count = pk.ReadInt64();
			r.members.resize(static_cast<size_t>(count));
			for (int64_t i = 0; i < count; ++i) {
				r.members[static_cast<size_t>(i)].chaId = pk.ReadInt64();
				r.members[static_cast<size_t>(i)].chaName = pk.ReadString();
				r.members[static_cast<size_t>(i)].motto = pk.ReadString();
				r.members[static_cast<size_t>(i)].icon = pk.ReadInt64();
			}
			r.notiPlyCount = pk.ReadInt64();
		}

		inline void deserialize(RPacket& pk, PcSessAddMessage& r) {
			r.sessId = pk.ReadInt64();
			r.chaId = pk.ReadInt64();
			r.chaName = pk.ReadString();
			r.motto = pk.ReadString();
			r.icon = pk.ReadInt64();
		}

		inline void deserialize(RPacket& pk, PcSessSayMessage& r) {
			r.sessId = pk.ReadInt64();
			r.chaId = pk.ReadInt64();
			r.content = pk.ReadString();
		}

		inline void deserialize(RPacket& pk, PcSessLeaveMessage& r) {
			r.sessId = pk.ReadInt64();
			r.chaId = pk.ReadInt64();
		}

		inline void deserialize(RPacket& pk, PcChangePersonInfoMessage& r) {
			r.motto = pk.ReadString();
			r.icon = pk.ReadInt64();
			r.refuseSess = pk.ReadInt64();
		}

		inline void deserialize(RPacket& pk, PcErrMsgMessage& r) {
			r.message = pk.ReadString();
		}

		inline void deserialize(RPacket& pk, PcGuildNoticeMessage& r) {
			r.content = pk.ReadString();
		}

		inline void deserialize(RPacket& pk, PcRegisterMessage& r) {
			r.result = pk.ReadInt64();
			r.message = pk.ReadString();
		}

		// --- Группа F: PM ---

		inline void deserialize(RPacket& pk, PmTeamMessage& r) {
			r.msg = pk.ReadInt64();
			r.count = pk.ReadInt64();
			r.members.resize(static_cast<size_t>(r.count));
			for (int64_t i = 0; i < r.count; ++i) {
				r.members[static_cast<size_t>(i)].gateName = pk.ReadString();
				r.members[static_cast<size_t>(i)].gtAddr = pk.ReadInt64();
				r.members[static_cast<size_t>(i)].chaId = pk.ReadInt64();
			}
		}

		inline void deserialize(RPacket& pk, PmExpScaleMessage& r) {
			r.chaId = pk.ReadInt64();
			r.time = pk.ReadInt64();
		}

		inline void deserialize(RPacket& pk, PmSay2AllMessage& r) {
			r.chaId = pk.ReadInt64();
			r.content = pk.ReadString();
			r.money = pk.ReadInt64();
		}

		inline void deserialize(RPacket& pk, PmSay2TradeMessage& r) {
			r.chaId = pk.ReadInt64();
			r.content = pk.ReadString();
			r.money = pk.ReadInt64();
		}

		inline void deserialize(RPacket& pk, PmGuildDisbandMessage& r) {
			r.guildId = pk.ReadInt64();
		}

		inline void deserialize(RPacket& pk, PmGuildChallMoneyMessage& r) {
			r.leaderId = pk.ReadInt64();
			r.money = pk.ReadInt64();
			r.guildName1 = pk.ReadString();
			r.guildName2 = pk.ReadString();
		}

		inline void deserialize(RPacket& pk, PmGuildChallPrizeMoneyMessage& r) {
			r.leaderId = pk.ReadInt64();
			r.money = pk.ReadInt64();
		}

		inline void deserialize(RPacket& pk, PtKickUserMessage& r) {
			r.gpAddr = pk.ReadInt64();
			r.gtAddr = pk.ReadInt64();
		}

		// --- Группа H: TM/MT ---

		inline void deserialize(RPacket& pk, MtLoginMessage& r) {
			r.serverName = pk.ReadString();
			r.mapNameList = pk.ReadString();
		}

		inline void deserialize(RPacket& pk, TmLoginAckMessage& r) {
			r.errCode = static_cast<int16_t>(pk.ReadInt64());
			if (r.errCode == 0)
				r.gateName = pk.ReadString();
		}

		inline void deserialize(RPacket& pk, TmEnterMapMessage& r) {
			r.actId = pk.ReadInt64();
			r.password = pk.ReadString();
			r.dbCharId = pk.ReadInt64();
			r.worldId = pk.ReadInt64();
			r.mapName = pk.ReadString();
			r.mapCopyNo = pk.ReadInt64();
			r.posX = pk.ReadInt64();
			r.posY = pk.ReadInt64();
			r.loginFlag = pk.ReadInt64();
			// Trailer (reverse-read):
			r.gateAddr = pk.ReverseReadInt64();
			r.winer = pk.ReverseReadInt64();
		}

		inline void deserialize(RPacket& pk, TmGoOutMapMessage& r) {
			// Все поля reverse-read (с конца пакета):
			r.playerPtr = pk.ReverseReadInt64();
			r.gateAddr = pk.ReverseReadInt64();
			r.offlineFlag = pk.ReverseReadInt64();
		}

		inline void deserialize(RPacket& pk, TmChangePersonInfoMessage& r) {
			r.motto = pk.ReadString();
			r.icon = pk.ReadInt64();
		}

		inline void deserializeMapEntry(RPacket& pk, MapEntryMessage& r) {
			r.srcMapName = pk.ReadString();
			r.targetMapName = pk.ReadString();
			r.subType = pk.ReadInt64();
			switch (r.subType) {
			case MAPENTRY_CREATE:
				r.posX = pk.ReadInt64();
				r.posY = pk.ReadInt64();
				r.copyPlayerNum = pk.ReadInt64();
				r.mapCopyNum = pk.ReadInt64();
				// Lua-скрипты: сначала строки, потом count
				// Note: в CREATE count идёт после строк — при десериализации неизвестно количество заранее.
				// GameServer читает поочерёдно до конца пакета. Для типизированной версии используем count в конце.
				break;
			case MAPENTRY_DESTROY:
				break;
			case MAPENTRY_SUBPLAYER:
				r.copyNo = pk.ReadInt64();
				r.numPlayers = pk.ReadInt64();
				break;
			case MAPENTRY_SUBCOPY:
				r.copyNo = pk.ReadInt64();
				break;
			case MAPENTRY_RETURN:
				r.resultCode = pk.ReadInt64();
				if (r.resultCode == 1) // COPY_CLOSE_SUC
					r.returnCopyNo = pk.ReadInt64();
				break;
			case MAPENTRY_COPYPARAM:
				r.copyId = pk.ReadInt64();
				for (int i = 0; i < MAPCOPY_INFO_PARAM_NUM; ++i)
					r.params[i] = pk.ReadInt64();
				break;
			case MAPENTRY_COPYRUN:
				r.copyId = pk.ReadInt64();
				r.condType = pk.ReadInt64();
				r.condValue = pk.ReadInt64();
				break;
			}
		}

		inline void deserialize(RPacket& pk, TmKickChaMessage& r) {
			r.charDbId = pk.ReadInt64();
		}

		inline void deserialize(RPacket& pk, TmKickChaResponse& r) {
			r.result = pk.ReadInt64();
		}

		inline void deserialize(RPacket& pk, TmOfflineModeMessage& r) {
			// Reverse-read (с конца пакета):
			r.playerPtr = pk.ReverseReadInt64();
			r.gateAddr = pk.ReverseReadInt64();
		}

		inline void deserialize(RPacket& pk, TmOfflineModeResponse& r) {
			r.result = pk.ReadInt64();
		}

		inline void deserialize(RPacket& pk, MtSwitchMapMessage& r) {
			r.currentMapName = pk.ReadString();
			r.currentCopyNo = pk.ReadInt64();
			r.posX = pk.ReadInt64();
			r.posY = pk.ReadInt64();
			r.targetMapName = pk.ReadString();
			r.targetCopyNo = pk.ReadInt64();
			r.targetX = pk.ReadInt64();
			r.targetY = pk.ReadInt64();
			r.switchType = pk.ReadInt64();
			// ReflectINFof trailer (reverse-read на GateServer):
			r.aimNum = pk.ReverseReadInt64();
			r.gateAddr = pk.ReverseReadInt64();
			r.playerDBID = pk.ReverseReadInt64();
		}

		inline void deserialize(RPacket& pk, MtKickUserMessage& r) {
			r.playerDBID = pk.ReadInt64();
			r.gateAddr = pk.ReadInt64();
			r.kickFlag = pk.ReadInt64();
		}

		inline void deserialize(RPacket& pk, MtPlayerExitMessage& r) {
			// ReflectINFof trailer (reverse-read на GateServer):
			r.aimNum = pk.ReverseReadInt64();
			r.gateAddr = pk.ReverseReadInt64();
			r.playerDBID = pk.ReverseReadInt64();
		}

		// --- Группа I: MM ---

		inline void deserializeMmHeader(RPacket& pk, MmHeader& h) {
			h.srcId = pk.ReadInt64();
			// Trailer (reverse-read):
			h.sNum = pk.ReverseReadInt64();
			h.gatePlayerAddr = pk.ReverseReadInt64();
			h.gatePlayerId = pk.ReverseReadInt64();
		}

		inline void deserialize(RPacket& pk, MmGateConnectMessage&) {
			pk.ReadInt64(); // char 0
		}

		inline void deserialize(RPacket& pk, MmGateReleaseMessage&) {
			pk.ReadInt64(); // char 0
		}

		inline void deserialize(RPacket& pk, MmQueryChaMessage& r) {
			r.srcId = pk.ReadInt64();
			r.chaName = pk.ReadString();
		}

		inline void deserialize(RPacket& pk, MmQueryChaResponse& r) {
			r.srcId = pk.ReadInt64();
			r.chaName = pk.ReadString();
			r.subMapName = pk.ReadString();
			r.posX = pk.ReadInt64();
			r.posY = pk.ReadInt64();
			r.chaId = pk.ReadInt64();
		}

		inline void deserialize(RPacket& pk, MmQueryChaItemMessage& r) {
			r.srcId = pk.ReadInt64();
			r.chaName = pk.ReadString();
		}

		inline void deserialize(RPacket& pk, MmCallChaMessage& r) {
			r.srcId = pk.ReadInt64();
			r.targetName = pk.ReadString();
			r.isBoat = pk.ReadInt64();
			r.mapName = pk.ReadString();
			r.posX = pk.ReadInt64();
			r.posY = pk.ReadInt64();
			r.copyNo = pk.ReadInt64();
		}

		inline void deserialize(RPacket& pk, MmGotoChaMessage& r) {
			r.srcId = pk.ReadInt64();
			r.targetName = pk.ReadString();
			r.mode = pk.ReadInt64();
			if (r.mode == 1) {
				r.srcName = pk.ReadString();
			}
			else if (r.mode == 2) {
				r.isBoat = pk.ReadInt64();
				r.mapName = pk.ReadString();
				r.posX = pk.ReadInt64();
				r.posY = pk.ReadInt64();
				r.copyNo = pk.ReadInt64();
			}
		}

		inline void deserialize(RPacket& pk, MmKickChaMessage& r) {
			r.srcId = pk.ReadInt64();
			r.targetName = pk.ReadString();
			r.kickDuration = pk.ReadInt64();
		}

		inline void deserialize(RPacket& pk, MmGuildRejectMessage& r) {
			r.srcId = pk.ReadInt64();
			r.guildName = pk.ReadString();
		}

		inline void deserialize(RPacket& pk, MmGuildApproveMessage& r) {
			r.srcId = pk.ReadInt64();
			r.guildId = pk.ReadInt64();
			r.guildName = pk.ReadString();
			r.guildMotto = pk.ReadString();
		}

		inline void deserialize(RPacket& pk, MmGuildKickMessage& r) {
			r.srcId = pk.ReadInt64();
			r.guildName = pk.ReadString();
		}

		inline void deserialize(RPacket& pk, MmGuildDisbandMessage& r) {
			r.srcId = pk.ReadInt64();
		}

		inline void deserialize(RPacket& pk, MmQueryChaPingMessage& r) {
			r.srcId = pk.ReadInt64();
			r.chaName = pk.ReadString();
		}

		inline void deserialize(RPacket& pk, MmNoticeMessage& r) {
			r.content = pk.ReadString();
		}

		inline void deserialize(RPacket& pk, MmGuildMottoMessage& r) {
			r.guildId = pk.ReadInt64();
			r.motto = pk.ReadString();
		}

		inline void deserialize(RPacket& pk, MmDoStringMessage& r) {
			r.srcId = pk.ReadInt64();
			r.luaCode = pk.ReadString();
		}

		inline void deserialize(RPacket& pk, MmChaNoticeMessage& r) {
			r.content = pk.ReadString();
			r.chaName = pk.ReadString();
		}

		inline void deserialize(RPacket& pk, MmLoginMessage& r) {
			r.chaName = pk.ReadString();
		}

		inline void deserialize(RPacket& pk, MmGuildChallPrizeMoneyMessage& r) {
			r.charDbId = pk.ReadInt64();
			r.money = pk.ReadInt64();
		}

		inline void deserialize(RPacket& pk, MmAddCreditMessage& r) {
			r.charDbId = pk.ReadInt64();
			r.amount = pk.ReadInt64();
		}

		inline void deserialize(RPacket& pk, MmStoreBuyMessage& r) {
			r.charDbId = pk.ReadInt64();
			r.commodityId = pk.ReadInt64();
			r.money = pk.ReadInt64();
		}

		inline void deserialize(RPacket& pk, MmAddMoneyMessage& r) {
			r.charDbId = pk.ReadInt64();
			r.money = pk.ReadInt64();
		}

		inline void deserialize(RPacket& pk, MmAuctionMessage& r) {
			r.charDbId = pk.ReadInt64();
		}

		inline void deserialize(RPacket& pk, MmUpdateGuildBankMessage& r) {
			r.guildId = pk.ReadInt64();
		}

		inline void deserialize(RPacket& pk, MmUpdateGuildBankGoldMessage& r) {
			r.guildId = pk.ReadInt64();
		}

		// --- Группа G: CM ---

		inline void deserialize(RPacket& pk, CmLoginRequest& r) {
			r.acctName = pk.ReadString();
			r.passwordHash = pk.ReadString();
			r.mac = pk.ReadString();
			r.cheatMarker = pk.ReadInt64();
			r.clientVersion = pk.ReadInt64();
		}

		inline void deserialize(RPacket& pk, McLoginResponse& r) {
			r.errCode = static_cast<int16_t>(pk.ReadInt64());
			if (r.errCode == 0) {
				McLoginResponseData d;
				d.maxChaNum = pk.ReadInt64(); // maxCharacters
				auto charCount = pk.ReadInt64(); // charCount
				d.characters.resize(static_cast<size_t>(charCount));
				for (auto& cha : d.characters) {
					cha.valid = pk.ReadInt64() != 0;
					if (cha.valid) {
						cha.chaName = pk.ReadString();
						cha.job = pk.ReadString();
						cha.degree = pk.ReadInt64();
						cha.typeId = pk.ReadInt64();
						cha.equipIds.resize(EQUIP_NUM);
						for (auto& eq : cha.equipIds) eq = pk.ReadInt64();
					}
				}
				d.hasPassword2 = pk.ReadInt64() != 0;
				r.data = std::move(d);
			}
		}

		inline void deserialize(RPacket& pk, McBgnPlayResponse& r) {
			r.errCode = static_cast<int16_t>(pk.ReadInt64());
		}

		inline void deserialize(RPacket& pk, McEndPlayResponse& r) {
			r.errCode = static_cast<int16_t>(pk.ReadInt64());
			if (r.errCode == 0) {
				McEndPlayResponseData d;
				d.maxChaNum = pk.ReadInt64();
				auto charCount = pk.ReadInt64();
				d.characters.resize(static_cast<size_t>(charCount));
				for (auto& cha : d.characters) {
					cha.valid = pk.ReadInt64() != 0;
					if (cha.valid) {
						cha.chaName = pk.ReadString();
						cha.job = pk.ReadString();
						cha.degree = pk.ReadInt64();
						cha.typeId = pk.ReadInt64();
						cha.equipIds.resize(EQUIP_NUM);
						for (auto& eq : cha.equipIds) eq = pk.ReadInt64();
					}
				}
				r.data = std::move(d);
			}
		}

		inline void deserialize(RPacket& pk, McNewChaResponse& r) {
			r.errCode = static_cast<int16_t>(pk.ReadInt64());
		}

		inline void deserialize(RPacket& pk, McDelChaResponse& r) {
			r.errCode = static_cast<int16_t>(pk.ReadInt64());
		}

		inline void deserialize(RPacket& pk, McCreatePassword2Response& r) {
			r.errCode = static_cast<int16_t>(pk.ReadInt64());
		}

		inline void deserialize(RPacket& pk, McUpdatePassword2Response& r) {
			r.errCode = static_cast<int16_t>(pk.ReadInt64());
		}

		// --- CMD_MC_ENTERMAP: вспомогательные функции десериализации ---

		inline void deserializeChaBaseInfo(RPacket& pk, ChaBaseInfo& b) {
			b.chaId = pk.ReadInt64();
			b.worldId = pk.ReadInt64();
			b.commId = pk.ReadInt64();
			b.commName = pk.ReadString();
			b.gmLv = pk.ReadInt64();
			b.handle = pk.ReadInt64();
			b.ctrlType = pk.ReadInt64();
			b.name = pk.ReadString();
			b.motto = pk.ReadString();
			b.icon = pk.ReadInt64();
			b.guildId = pk.ReadInt64();
			b.guildName = pk.ReadString();
			b.guildMotto = pk.ReadString();
			b.guildPermission = pk.ReadInt64();
			b.stallName = pk.ReadString();
			b.state = pk.ReadInt64();
			b.posX = pk.ReadInt64();
			b.posY = pk.ReadInt64();
			b.radius = pk.ReadInt64();
			b.angle = pk.ReadInt64();
			b.teamLeaderId = pk.ReadInt64();
			b.isPlayer = pk.ReadInt64();
			// Side
			b.side.sideId = pk.ReadInt64();
			// Event
			b.event.entityId = pk.ReadInt64();
			b.event.entityType = pk.ReadInt64();
			b.event.eventId = pk.ReadInt64();
			b.event.eventName = pk.ReadString();
			// Look
			b.look.synType = pk.ReadInt64();
			b.look.typeId = pk.ReadInt64();
			b.look.isBoat = pk.ReadInt64() != 0;
			if (b.look.isBoat) {
				b.look.boatParts.posId = pk.ReadInt64();
				b.look.boatParts.boatId = pk.ReadInt64();
				b.look.boatParts.header = pk.ReadInt64();
				b.look.boatParts.body = pk.ReadInt64();
				b.look.boatParts.engine = pk.ReadInt64();
				b.look.boatParts.cannon = pk.ReadInt64();
				b.look.boatParts.equipment = pk.ReadInt64();
			}
			else {
				b.look.hairId = pk.ReadInt64();
				for (int i = 0; i < EQUIP_NUM; ++i) {
					auto& eq = b.look.equips[i];
					eq.id = pk.ReadInt64();
					eq.dbId = pk.ReadInt64();
					eq.needLv = pk.ReadInt64();
					if (eq.id == 0) continue;
					if (b.look.synType == SYN_LOOK_CHANGE) {
						eq.endure0 = pk.ReadInt64();
						eq.energy0 = pk.ReadInt64();
						eq.valid = pk.ReadInt64();
						eq.tradable = pk.ReadInt64();
						eq.expiration = pk.ReadInt64();
					}
					else {
						eq.num = pk.ReadInt64();
						eq.endure0 = pk.ReadInt64();
						eq.endure1 = pk.ReadInt64();
						eq.energy0 = pk.ReadInt64();
						eq.energy1 = pk.ReadInt64();
						eq.forgeLv = pk.ReadInt64();
						eq.valid = pk.ReadInt64();
						eq.tradable = pk.ReadInt64();
						eq.expiration = pk.ReadInt64();
						eq.hasExtra = pk.ReadInt64() != 0;
						if (eq.hasExtra) {
							eq.forgeParam = pk.ReadInt64();
							eq.instId = pk.ReadInt64();
							eq.hasInstAttr = pk.ReadInt64() != 0;
							if (eq.hasInstAttr) {
								for (int j = 0; j < ITEM_INSTANCE_ATTR_NUM; ++j) {
									eq.instAttr[j][0] = pk.ReadInt64();
									eq.instAttr[j][1] = pk.ReadInt64();
								}
							}
						}
					}
				}
			}
			// PK ctrl
			b.pkCtrl = pk.ReadInt64();
			// Append look
			for (int i = 0; i < ESPE_KBGRID_NUM; ++i) {
				b.appendLook[i].lookId = pk.ReadInt64();
				if (b.appendLook[i].lookId != 0)
					b.appendLook[i].valid = pk.ReadInt64();
			}
		}

		inline void deserializeChaSkillBagInfo(RPacket& pk, ChaSkillBagInfo& s) {
			s.defSkillId = pk.ReadInt64();
			s.synType = pk.ReadInt64();
			auto count = pk.ReadInt64();
			s.skills.resize(static_cast<size_t>(count));
			for (auto& sk : s.skills) {
				sk.id = pk.ReadInt64();
				sk.state = pk.ReadInt64();
				sk.level = pk.ReadInt64();
				sk.useSp = pk.ReadInt64();
				sk.useEndure = pk.ReadInt64();
				sk.useEnergy = pk.ReadInt64();
				sk.resumeTime = pk.ReadInt64();
				sk.range[0] = pk.ReadInt64();
				if (sk.range[0] != RANGE_TYPE_NONE)
					for (int j = 1; j < SKILL_RANGE_PARAM_NUM; ++j)
						sk.range[j] = pk.ReadInt64();
			}
		}

		inline void deserializeChaSkillStateInfo(RPacket& pk, ChaSkillStateInfo& s) {
			s.currentTime = pk.ReadInt64();
			auto count = pk.ReadInt64();
			s.states.resize(static_cast<size_t>(count));
			for (auto& st : s.states) {
				st.stateId = pk.ReadInt64();
				st.stateLv = pk.ReadInt64();
				st.duration = pk.ReadInt64();
				st.startTime = pk.ReadInt64();
			}
		}

		inline void deserializeChaAttrInfo(RPacket& pk, ChaAttrInfo& a) {
			a.synType = pk.ReadInt64();
			auto count = pk.ReadInt64();
			a.attrs.resize(static_cast<size_t>(count));
			for (auto& attr : a.attrs) {
				attr.attrId = pk.ReadInt64();
				attr.attrVal = pk.ReadInt64();
			}
		}

		inline void deserializeChaKitbagInfo(RPacket& pk, ChaKitbagInfo& k) {
			k.synType = pk.ReadInt64();
			if (k.synType == SYN_KITBAG_INIT)
				k.capacity = pk.ReadInt64();
			k.items.clear();
			while (true) {
				int64_t gridId = pk.ReadInt64();
				if (gridId < 0) break;
				KitbagItem item;
				item.gridId = gridId;
				item.itemId = pk.ReadInt64();
				if (item.itemId > 0) {
					item.dbId = pk.ReadInt64();
					item.needLv = pk.ReadInt64();
					item.num = pk.ReadInt64();
					item.endure0 = pk.ReadInt64();
					item.endure1 = pk.ReadInt64();
					item.energy0 = pk.ReadInt64();
					item.energy1 = pk.ReadInt64();
					item.forgeLv = pk.ReadInt64();
					item.valid = pk.ReadInt64();
					item.tradable = pk.ReadInt64();
					item.expiration = pk.ReadInt64();
					item.isBoat = pk.ReadInt64() != 0;
					if (item.isBoat)
						item.boatWorldId = pk.ReadInt64();
					item.forgeParam = pk.ReadInt64();
					item.instId = pk.ReadInt64();
					item.hasInstAttr = pk.ReadInt64() != 0;
					if (item.hasInstAttr) {
						for (int j = 0; j < ITEM_INSTANCE_ATTR_NUM; ++j) {
							item.instAttr[j][0] = pk.ReadInt64();
							item.instAttr[j][1] = pk.ReadInt64();
						}
					}
				}
				k.items.push_back(std::move(item));
			}
		}

		inline void deserializeChaShortcutInfo(RPacket& pk, ChaShortcutInfo& s) {
			for (int i = 0; i < SHORT_CUT_NUM; ++i) {
				s.entries[i].type = pk.ReadInt64();
				s.entries[i].gridId = pk.ReadInt64();
			}
		}

		inline void deserialize(RPacket& pk, McEnterMapMessage& r) {
			r.errCode = pk.ReadInt64();
			if (r.errCode != 0) return;
			McEnterMapData d;
			d.autoLock = pk.ReadInt64();
			d.kitbagLock = pk.ReadInt64();
			d.enterType = pk.ReadInt64();
			d.isNewCha = pk.ReadInt64();
			d.mapName = pk.ReadString();
			d.canTeam = pk.ReadInt64();
			d.imp = pk.ReadInt64();
			deserializeChaBaseInfo(pk, d.baseInfo);
			{
				auto m = pk.ReadInt64();
				if (m != ENTERMAP_MARK_BASEINFO) {
					printf("ENTERMAP: BASEINFO marker mismatch! got=%lld pos=%d\n", (long long)m, pk.Position());
					return;
				}
			}
			deserializeChaSkillBagInfo(pk, d.skillBag);
			{
				auto m = pk.ReadInt64();
				if (m != ENTERMAP_MARK_SKILLBAG) {
					printf("ENTERMAP: SKILLBAG marker mismatch! got=%lld pos=%d\n", (long long)m, pk.Position());
					return;
				}
			}
			deserializeChaSkillStateInfo(pk, d.skillState);
			{
				auto m = pk.ReadInt64();
				if (m != ENTERMAP_MARK_SKILLSTATE) {
					printf("ENTERMAP: SKILLSTATE marker mismatch! got=%lld pos=%d\n", (long long)m, pk.Position());
					return;
				}
			}
			deserializeChaAttrInfo(pk, d.attr);
			{
				auto m = pk.ReadInt64();
				if (m != ENTERMAP_MARK_ATTR) {
					printf("ENTERMAP: ATTR marker mismatch! got=%lld pos=%d\n", (long long)m, pk.Position());
					return;
				}
			}
			deserializeChaKitbagInfo(pk, d.kitbag);
			{
				auto m = pk.ReadInt64();
				if (m != ENTERMAP_MARK_KITBAG) {
					printf("ENTERMAP: KITBAG marker mismatch! got=%lld pos=%d\n", (long long)m, pk.Position());
					return;
				}
			}
			deserializeChaShortcutInfo(pk, d.shortcut);
			{
				auto m = pk.ReadInt64();
				if (m != ENTERMAP_MARK_SHORTCUT) {
					printf("ENTERMAP: SHORTCUT marker mismatch! got=%lld pos=%d\n", (long long)m, pk.Position());
					return;
				}
			}
			auto boatNum = pk.ReadInt64();
			d.boats.resize(static_cast<size_t>(boatNum));
			for (auto& boat : d.boats) {
				deserializeChaBaseInfo(pk, boat.baseInfo);
				deserializeChaAttrInfo(pk, boat.attr);
				deserializeChaKitbagInfo(pk, boat.kitbag);
				deserializeChaSkillStateInfo(pk, boat.skillState);
			}
			{
				auto m = pk.ReadInt64();
				if (m != ENTERMAP_MARK_BOATS) {
					printf("ENTERMAP: BOATS marker mismatch! got=%lld pos=%d\n", (long long)m, pk.Position());
					return;
				}
			}
			d.ctrlChaId = pk.ReadInt64();
			// loginFlag, playerCount, playerAddr — GateServer уже срезал.
			r.data = std::move(d);
		}
	}
} // namespace net::msg
