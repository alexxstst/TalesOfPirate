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
		struct TmEnterMapMessage {
			int64_t actId = 0; // actor login ID
			std::string password;
			int64_t dbCharId = 0;
			int64_t worldId = 0;
			std::string mapName;
			int64_t mapCopyNo = 0;
			int64_t posX = 0;
			int64_t posY = 0;
			int64_t loginFlag = 0; // 0=initial login, 1=map switch
			int64_t gateAddr = 0; // адрес GateServer
			int64_t winer = 0; // garner winner
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
		//  Группа J: GameServer -> Client (MC) — игровые уведомления
		// =================================================================

		/// CMD_MC_SYSINFO (517): Системное сообщение клиенту.
		struct McSysInfoMessage {
			std::string info;
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
			w.WriteInt64(msg.gateAddr);
			w.WriteInt64(msg.winer);
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

		// --- Группа J: MC ---

		inline WPacket serialize(const McSysInfoMessage& msg) {
			WPacket w(static_cast<int>(msg.info.size()) + 32);
			w.WriteCmd(CMD_MC_SYSINFO);
			w.WriteString(msg.info);
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

		/// Сериализация ChaEventInfo.
		inline void serializeChaEventInfo(WPacket& w, const ChaEventInfo& e) {
			w.WriteInt64(e.entityId);
			w.WriteInt64(e.entityType);
			w.WriteInt64(e.eventId);
			w.WriteString(e.eventName);
		}

		/// Десериализация ChaEventInfo.
		inline void deserializeChaEventInfo(RPacket& pk, ChaEventInfo& e) {
			e.entityId = pk.ReadInt64();
			e.entityType = pk.ReadInt64();
			e.eventId = pk.ReadInt64();
			e.eventName = pk.ReadString();
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

		/// Сериализация ChaLookInfo (standalone — для McCharacterAction и т.д.).
		inline void serializeChaLookInfo(WPacket& w, const ChaLookInfo& look) {
			w.WriteInt64(look.synType);
			w.WriteInt64(look.typeId);
			w.WriteInt64(look.isBoat ? 1 : 0);
			if (look.isBoat) {
				w.WriteInt64(look.boatParts.posId);
				w.WriteInt64(look.boatParts.boatId);
				w.WriteInt64(look.boatParts.header);
				w.WriteInt64(look.boatParts.body);
				w.WriteInt64(look.boatParts.engine);
				w.WriteInt64(look.boatParts.cannon);
				w.WriteInt64(look.boatParts.equipment);
			}
			else {
				w.WriteInt64(look.hairId);
				for (int i = 0; i < EQUIP_NUM; ++i) {
					const auto& eq = look.equips[i];
					w.WriteInt64(eq.id);
					w.WriteInt64(eq.dbId);
					w.WriteInt64(eq.needLv);
					if (eq.id == 0) continue;
					if (look.synType == SYN_LOOK_CHANGE) {
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
			r.gateAddr = pk.ReadInt64();
			r.winer = pk.ReadInt64();
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

		// --- Группа J: MC ---

		inline void deserialize(RPacket& pk, McSysInfoMessage& r) {
			r.info = pk.ReadString();
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

		/// Десериализация ChaLookInfo (standalone — для McCharacterAction и т.д.).
		inline void deserializeChaLookInfo(RPacket& pk, ChaLookInfo& look) {
			look.synType = pk.ReadInt64();
			look.typeId = pk.ReadInt64();
			look.isBoat = pk.ReadInt64() != 0;
			if (look.isBoat) {
				look.boatParts.posId = pk.ReadInt64();
				look.boatParts.boatId = pk.ReadInt64();
				look.boatParts.header = pk.ReadInt64();
				look.boatParts.body = pk.ReadInt64();
				look.boatParts.engine = pk.ReadInt64();
				look.boatParts.cannon = pk.ReadInt64();
				look.boatParts.equipment = pk.ReadInt64();
			}
			else {
				look.hairId = pk.ReadInt64();
				for (int i = 0; i < EQUIP_NUM; ++i) {
					auto& eq = look.equips[i];
					eq.id = pk.ReadInt64();
					eq.dbId = pk.ReadInt64();
					eq.needLv = pk.ReadInt64();
					if (eq.id == 0) continue;
					if (look.synType == SYN_LOOK_CHANGE) {
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

		// =================================================================
		//  Геймплейные команды Client ↔ GameServer (Фаза 1 — простые)
		// =================================================================

		// ─── CM — простые (1-3 поля) ───────────────────────────────

		struct CmDieReturnMessage { int64_t reliveType; };
		struct CmAutoKitbagLockMessage { int64_t autoLock; };
		struct CmStallSearchMessage { int64_t itemId; };
		struct CmForgeItemMessage { int64_t index; };
		struct CmEntityEventMessage { int64_t entityId; };
		struct CmStallOpenMessage { int64_t charId; };
		struct CmMisLogInfoMessage { int64_t id; };
		struct CmMisLogClearMessage { int64_t id; };
		struct CmStoreBuyAskMessage { int64_t comId; };
		struct CmStoreChangeAskMessage { int64_t num; };
		struct CmStoreQueryMessage { int64_t num; };
		struct CmTeamFightAnswerMessage { int64_t accept; };
		struct CmItemRepairAnswerMessage { int64_t accept; };
		struct CmItemForgeAnswerMessage { int64_t accept; };
		struct CmItemLotteryAnswerMessage { int64_t accept; };
		struct CmVolunteerOpenMessage { int64_t num; };
		struct CmVolunteerListMessage { int64_t page; int64_t num; };
		struct CmStoreListAskMessage { int64_t clsId; int64_t page; int64_t num; };
		struct CmCaptainConfirmAsrMessage { int64_t ret; int64_t teamId; };
		struct CmMapMaskMessage { std::string mapName; };
		struct CmStoreOpenAskMessage { std::string password; };
		struct CmVolunteerSelMessage { std::string name; };
		struct CmKitbagUnlockMessage { std::string password; };

		// ─── MC — простые ──────────────────────────────────────────

		struct McFailedActionMessage { int64_t worldId; int64_t actionType; int64_t reason; };
		struct McChaEndSeeMessage { int64_t seeType; int64_t worldId; };
		struct McItemDestroyMessage { int64_t worldId; };
		struct McForgeResultMessage { int64_t result; };
		struct McUniteResultMessage { int64_t result; };
		struct McMillingResultMessage { int64_t result; };
		struct McFusionResultMessage { int64_t result; };
		struct McUpgradeResultMessage { int64_t result; };
		struct McPurifyResultMessage { int64_t result; };
		struct McFixResultMessage { int64_t result; };
		struct McEidolonMetempsychosisMessage { int64_t result; };
		struct McEidolonFusionMessage { int64_t result; };
		struct McMessageMessage { std::string text; };
		struct McBickerNoticeMessage { std::string text; };
		struct McColourNoticeMessage { int64_t color; std::string text; };
		struct McTriggerActionMessage { int64_t type; int64_t id; int64_t num; int64_t count; };
		struct McNpcStateChangeMessage { int64_t npcId; int64_t state; };
		struct McEntityStateChangeMessage { int64_t entityId; int64_t state; };
		struct McCloseTalkMessage { int64_t npcId; };
		struct McKitbagCheckAnswerMessage { int64_t locked; };
		struct McPreMoveTimeMessage { int64_t time; };
		struct McItemUseSuccMessage { int64_t worldId; int64_t itemId; };
		struct McChaPlayEffectMessage { int64_t worldId; int64_t effectId; };
		struct McSynDefaultSkillMessage { int64_t worldId; int64_t skillId; };

		// ─── MC — ReadSequence→ReadString миграция ─────────────────

		struct McSayMessage { int64_t sourceId; std::string content; int64_t color; };
		struct McPopupNoticeMessage { std::string notice; };
		struct McPingMessage { int64_t v1; int64_t v2; int64_t v3; int64_t v4; int64_t v5; };

		// =================================================================
		//  Геймплейные команды: Фаза 2 — средние
		// =================================================================

		// ─── CM — средние ──────────────────────────────────────────

		struct CmChangePassMessage { std::string pass; std::string pin; };
		struct CmGuildBankOperMessage { int64_t op; int64_t srcType; int64_t srcId; int64_t srcNum; int64_t tarType; int64_t tarId; };
		struct CmGuildBankGoldMessage { int64_t op; int64_t direction; int64_t gold; };
		struct CmUpdateHairMessage { int64_t scriptId; int64_t gridLoc0; int64_t gridLoc1; int64_t gridLoc2; int64_t gridLoc3; };
		struct CmTeamFightAskMessage { int64_t type; int64_t worldId; int64_t handle; };
		struct CmItemRepairAskMessage { int64_t repairmanId; int64_t repairmanHandle; int64_t posType; int64_t posId; };
		struct CmRequestTradeMessage { int64_t type; int64_t charId; };
		struct CmAcceptTradeMessage { int64_t type; int64_t charId; };
		struct CmCancelTradeMessage { int64_t type; int64_t charId; };
		struct CmValidateTradeDataMessage { int64_t type; int64_t charId; };
		struct CmValidateTradeMessage { int64_t type; int64_t charId; };
		struct CmAddItemMessage { int64_t type; int64_t charId; int64_t opType; int64_t index; int64_t itemIndex; int64_t count; };
		struct CmAddMoneyMessage { int64_t type; int64_t charId; int64_t opType; int64_t isImp; int64_t money; };
		struct CmTigerStartMessage { int64_t npcId; int64_t sel1; int64_t sel2; int64_t sel3; };
		struct CmTigerStopMessage { int64_t npcId; int64_t num; };
		struct CmVolunteerAsrMessage { int64_t ret; std::string name; };
		struct CmCreateBoatMessage { std::string boat; int64_t header; int64_t engine; int64_t cannon; int64_t equipment; };
		struct CmUpdateBoatMessage { int64_t header; int64_t engine; int64_t cannon; int64_t equipment; };
		struct CmSelectBoatListMessage { int64_t npcId; int64_t type; int64_t index; };
		struct CmBoatLaunchMessage { int64_t npcId; int64_t index; };
		struct CmBoatBagSelMessage { int64_t npcId; int64_t index; };
		struct CmReportWgMessage { std::string info; };
		struct CmSay2CampMessage { std::string content; };
		struct CmStallBuyMessage { int64_t charId; int64_t index; int64_t count; int64_t gridId; };
		struct CmSkillUpgradeMessage { int64_t skillId; int64_t addGrade; };
		struct CmRefreshDataMessage { int64_t worldId; int64_t handle; };
		struct CmPkCtrlMessage { int64_t ctrl; };
		struct CmItemAmphitheaterAskMessage { int64_t sure; int64_t reId; };
		struct CmMasterInviteMessage { int64_t masterId; };
		struct CmMasterAsrMessage { int64_t agree; int64_t masterId; };
		struct CmMasterDelMessage { int64_t masterId; };
		struct CmPrenticeInviteMessage { int64_t prenticeId; };
		struct CmPrenticeAsrMessage { int64_t agree; int64_t prenticeId; };
		struct CmPrenticeDelMessage { int64_t prenticeId; };

		// ─── NPC Talk составные команды ─────────────────────────────
		struct CmRequestTalkMessage { int64_t npcId; int64_t cmd; };
		struct CmSelFunctionMessage { int64_t npcId; int64_t pageId; int64_t index; };
		struct CmSaleMessage { int64_t npcId; int64_t index; int64_t count; };
		struct CmBuyMessage { int64_t npcId; int64_t itemType; int64_t index1; int64_t index2; int64_t count; };
		struct CmMissionPageMessage { int64_t npcId; int64_t cmd; int64_t selItem; int64_t param; };
		struct CmSelMissionMessage { int64_t npcId; int64_t index; };
		struct CmBlackMarketExchangeReqMessage { int64_t npcId; int64_t timeNum; int64_t srcId; int64_t srcNum; int64_t tarId; int64_t tarNum; int64_t index; };

		// ─── MC — средние ──────────────────────────────────────────
		struct McSay2CampMessage { std::string chaName; std::string content; };
		struct McTalkInfoMessage { int64_t npcId; int64_t cmd; std::string text; };
		struct McTradeDataMessage { int64_t npcId; int64_t page; int64_t index; int64_t itemId; int64_t count; int64_t price; };
		struct McTradeResultMessage { int64_t type; int64_t index; int64_t count; int64_t itemId; int64_t money; };
		struct McUpdateHairResMessage { int64_t worldId; int64_t scriptId; std::string reason; };
		struct McSynTLeaderIdMessage { int64_t worldId; int64_t leaderId; };
		struct McKitbagCapacityMessage { int64_t worldId; int64_t capacity; };
		struct McItemForgeAskMessage { int64_t type; int64_t money; };
		struct McItemForgeAnswerMessage { int64_t worldId; int64_t type; int64_t result; };
		struct McQueryChaMessage { int64_t chaId; std::string name; std::string mapName; int64_t posX; int64_t posY; int64_t chaId2; };
		struct McQueryChaPingMessage { int64_t chaId; std::string name; std::string mapName; int64_t ping; };
		struct McQueryReliveMessage { int64_t chaId; std::string sourceName; int64_t reliveType; };
		struct McGmMailMessage { std::string title; std::string content; int64_t time; };
		struct McSynStallNameMessage { int64_t charId; std::string name; };
		struct McMapCrashMessage { std::string text; };
		struct McVolunteerStateMessage { int64_t state; };
		struct McVolunteerAskMessage { std::string name; };
		struct McMasterAskMessage { std::string name; int64_t chaId; };
		struct McPrenticeAskMessage { std::string name; int64_t chaId; };
		struct McItemRepairAskMcMessage { std::string itemName; int64_t repairCost; };
		struct McItemLotteryAsrMessage { int64_t success; };

		// =================================================================
		//  Геймплейные команды: Фаза 3 — сложные
		// =================================================================

		struct McSynAttributeMessage { int64_t worldId; ChaAttrInfo attr; };
		struct McSynSkillStateMessage { int64_t worldId; ChaSkillStateInfo skillState; };

		struct McAStateEndSeeMessage { int64_t areaX; int64_t areaY; };

		struct AStateBeginSeeEntry { int64_t stateId; int64_t stateLv; int64_t worldId; int64_t fightId; };
		struct McAStateBeginSeeMessage { int64_t areaX; int64_t areaY; std::vector<AStateBeginSeeEntry> states; };

		struct McDelItemChaMessage { int64_t mainChaId; int64_t worldId; };

		struct McSynEventInfoMessage { int64_t entityId; int64_t entityType; int64_t eventId; std::string eventName; };
		struct McSynSideInfoMessage { int64_t worldId; ChaSideInfo side; };

		/// CMD_MC_ITEMBEGINSEE — предмет появился на земле (9 полей + ChaEventInfo).
		struct McItemCreateMessage {
			int64_t worldId = 0; int64_t handle = 0; int64_t itemId = 0;
			int64_t posX = 0; int64_t posY = 0; int64_t angle = 0; int64_t num = 0;
			int64_t appeType = 0; int64_t fromId = 0;
			ChaEventInfo event;
		};

		/// CMD_MC_SYNSKILLBAG — синхронизация сумки скиллов.
		struct McSynSkillBagMessage { int64_t worldId = 0; ChaSkillBagInfo skillBag; };

		// ─── Квестовые ─────────────────────────────────────────────
		struct McMissionInfoMessage { int64_t npcId; int64_t listType; int64_t prev; int64_t next; int64_t prevCmd; int64_t nextCmd; std::vector<std::string> items; };

		/// Константы типов потребностей квеста (mission::MIS_NEED_TYPE).
		constexpr int64_t MIS_NEED_ITEM = 0;
		constexpr int64_t MIS_NEED_KILL = 1;
		constexpr int64_t MIS_NEED_DESP = 5;

		/// Запись потребности квеста (условная сериализация по needType).
		struct MisNeedEntry {
			int64_t needType = 0;
			int64_t param1 = 0; int64_t param2 = 0; int64_t param3 = 0; // MIS_NEED_ITEM/KILL
			std::string desp; // MIS_NEED_DESP
		};

		/// Запись награды квеста.
		struct MisPrizeEntry { int64_t type = 0; int64_t param1 = 0; int64_t param2 = 0; };

		/// CMD_MC_MISPAGE — страница квеста (сложный формат с условной сериализацией).
		struct McMisPageMessage {
			int64_t cmd = 0; int64_t npcId = 0; std::string name;
			std::vector<MisNeedEntry> needs;
			int64_t prizeSelType = 0; std::vector<MisPrizeEntry> prizes;
			std::string description;
		};

		struct MisLogEntry { int64_t misId; int64_t state; };
		struct McMisLogMessage { std::vector<MisLogEntry> logs; };

		/// CMD_MC_MISLOGINFO — детали записи журнала квестов (формат аналогичен MisPage).
		struct McMisLogInfoMessage {
			int64_t misId = 0; std::string name;
			std::vector<MisNeedEntry> needs;
			int64_t prizeSelType = 0; std::vector<MisPrizeEntry> prizes;
			std::string description;
		};
		struct McMisLogClearMcMessage { int64_t missionId; };
		struct McMisLogAddMessage { int64_t missionId; int64_t state; };
		struct McMisLogStateMessage { int64_t missionId; int64_t state; };

		// ─── NPC диалоги ───────────────────────────────────────────
		struct FuncInfoFuncItem { std::string name; };
		struct FuncInfoMissionItem { std::string name; int64_t state; };
		struct McFuncInfoMessage {
			int64_t npcId; int64_t page; std::string talkText;
			std::vector<FuncInfoFuncItem> funcItems;
			std::vector<FuncInfoMissionItem> missionItems;
		};

		// ─── Volunteer с массивами ─────────────────────────────────
		struct VolunteerEntry { std::string name; int64_t level; int64_t job; std::string map; };
		struct McVolunteerListMessage { int64_t pageTotal; int64_t page; std::vector<VolunteerEntry> volunteers; };
		struct McVolunteerOpenMessage { int64_t state; int64_t pageTotal; std::vector<VolunteerEntry> volunteers; };

		// ─── StallSearch / Ranking (count в начале — новый формат) ─
		struct StallSearchEntry { std::string name; std::string stallName; std::string location; int64_t count; int64_t cost; };
		struct McShowStallSearchMessage { std::vector<StallSearchEntry> entries; };

		struct RankingEntry { std::string name; std::string guild; int64_t level; int64_t job; int64_t score; };
		struct McShowRankingMessage { std::vector<RankingEntry> entries; };

		// ─── EspeItem ──────────────────────────────────────────────
		struct EspeItemEntry { int64_t position; int64_t endure; int64_t energy; int64_t tradable; int64_t expiration; };
		struct McEspeItemMessage { int64_t worldId; std::vector<EspeItemEntry> items; };

		// ─── BlackMarket/Exchange ──────────────────────────────────
		struct BlackMarketExchangeEntry { int64_t srcId; int64_t srcCount; int64_t tarId; int64_t tarCount; int64_t timeValue; };
		struct McBlackMarketExchangeDataMessage { int64_t npcId; std::vector<BlackMarketExchangeEntry> exchanges; };

		struct ExchangeEntry { int64_t srcId; int64_t srcCount; int64_t tarId; int64_t tarCount; };
		struct McExchangeDataMessage { int64_t npcId; std::vector<ExchangeEntry> exchanges; };

		struct McBlackMarketExchangeUpdateMessage { int64_t npcId; std::vector<BlackMarketExchangeEntry> exchanges; };

		struct McBlackMarketExchangeAsrMessage { int64_t success; int64_t srcId; int64_t srcCount; int64_t tarId; int64_t tarCount; };

		// ─── MC — торговля NPC (полные данные) ─────────────────────

		/// Предмет на торговой странице (товары NPC).
		struct TradePageItem { int64_t itemId = 0; int64_t count = 0; int64_t price = 0; int64_t level = 0; };
		/// Страница торговли NPC — тип товаров + список предметов.
		struct TradePage { int64_t itemType = 0; std::vector<TradePageItem> items; };
		/// CMD_MC_TRADE_ALLDATA — полные данные торговли NPC.
		struct McTradeAllDataMessage { int64_t npcId = 0; int64_t tradeType = 0; int64_t param = 0; std::vector<TradePage> pages; };

		// ─── MC — магазин (Store) ──────────────────────────────────

		/// Запись объявления магазина.
		struct StoreAfficheEntry { int64_t afficheId = 0; std::string title; std::string comId; };
		/// Запись категории магазина.
		struct StoreClassEntry { int64_t classId = 0; std::string className; int64_t parentId = 0; };
		/// CMD_MC_STORE_OPEN_ASR — ответ на открытие магазина.
		struct McStoreOpenAnswerMessage {
			bool isValid = false; int64_t vip = 0; int64_t moBean = 0; int64_t replMoney = 0;
			std::vector<StoreAfficheEntry> affiches; std::vector<StoreClassEntry> classes;
		};

		/// Вариант товара (предмет внутри товара магазина).
		struct StoreVariantEntry { int64_t itemId = 0; int64_t itemNum = 0; int64_t flute = 0; };
		/// Атрибут товара магазина.
		struct StoreAttrEntry { int64_t attrId = 0; int64_t attrVal = 0; };
		/// Товар магазина.
		struct StoreProductEntry {
			int64_t comId = 0; std::string comName; int64_t price = 0; std::string remark;
			bool isHot = false; int64_t time = 0; int64_t quantity = 0; int64_t expire = 0;
			std::vector<StoreVariantEntry> variants; StoreAttrEntry attrs[5];
		};
		/// CMD_MC_STORE_LIST_ASR — список товаров магазина.
		struct McStoreListAnswerMessage { int64_t pageTotal = 0; int64_t pageCurrent = 0; std::vector<StoreProductEntry> products; };

		/// Запись истории покупок магазина.
		struct StoreHistoryEntry { std::string time; int64_t itemId = 0; std::string name; int64_t money = 0; };
		/// CMD_MC_STORE_QUERY — история покупок магазина.
		struct McStoreHistoryMessage { std::vector<StoreHistoryEntry> records; };

		/// CMD_MC_STORE_VIP — VIP-данные магазина.
		struct McStoreVipMessage { int64_t success = 0; int64_t vipId = 0; int64_t months = 0; int64_t shuijing = 0; int64_t modou = 0; };

		// ─── MC — синхронизация команды (пати) ─────────────────────

		/// CMD_MC_TEAM — синхронизация данных члена пати.
		struct McSynTeamMessage {
			int64_t memberId = 0; int64_t hp = 0; int64_t maxHP = 0; int64_t sp = 0; int64_t maxSP = 0; int64_t level = 0;
			ChaLookInfo look;
		};

		// ─── McChaBeginSee ─────────────────────────────────────────

		/// Информация о наклоне (poseType == 1).
		struct LeanInfo { int64_t leanState = 0; int64_t pose = 0; int64_t angle = 0; int64_t posX = 0; int64_t posY = 0; int64_t height = 0; };
		/// Информация о сидячей позе (poseType == 2).
		struct SeatInfo { int64_t seatAngle = 0; int64_t seatPose = 0; };

		/// CMD_MC_CHABEGINSEE — персонаж появился в зоне видимости.
		struct McChaBeginSeeMessage {
			int64_t seeType = 0;
			ChaBaseInfo base;
			int64_t npcType = 0;
			int64_t npcState = 0;
			int64_t poseType = 0;
			LeanInfo lean;  // валидно при poseType == 1
			SeatInfo seat;  // валидно при poseType == 2
			ChaAttrInfo attr;
			ChaSkillStateInfo skillState;
		};

		/// CMD_MC_ADD_ITEM_CHA — добавление предметного персонажа (item cha).
		struct McAddItemChaMessage {
			int64_t mainChaId = 0;
			ChaBaseInfo base;
			ChaAttrInfo attr;
			ChaKitbagInfo kitbag;
			ChaSkillStateInfo skillState;
		};

		// ─── McCharacterAction (CMD_MC_NOTIACTION) ──────────────

		/// Константы типов действий (enumACTION_*).
		namespace ActionType {
			constexpr int64_t NONE       = 0;
			constexpr int64_t MOVE       = 1;
			constexpr int64_t SKILL      = 2;
			constexpr int64_t SKILL_SRC  = 3;
			constexpr int64_t SKILL_TAR  = 4;
			constexpr int64_t LOOK       = 5;
			constexpr int64_t KITBAG     = 6;
			constexpr int64_t ITEM_FAILED = 15;
			constexpr int64_t LEAN       = 16;
			constexpr int64_t CHANGE_CHA = 17;
			constexpr int64_t FACE       = 19;
			constexpr int64_t SKILL_POSE = 21;
			constexpr int64_t TEMP       = 24;
			constexpr int64_t SHORTCUT   = 25;
			constexpr int64_t BANK       = 26;
			constexpr int64_t KITBAGTMP  = 28;
			constexpr int64_t GUILDBANK  = 30;
		}

		/// Константы состояний.
		constexpr int64_t MSTATE_ON = 0x00;
		constexpr int64_t FSTATE_ON = 0x0000;

		/// Запись эффекта атрибута (SKILL_SRC / SKILL_TAR).
		struct ActionEffectEntry {
			int64_t attrId = 0;
			int64_t attrVal = 0;
		};

		/// Запись состояния скилла (SKILL_SRC).
		struct ActionStateEntry {
			int64_t stateId = 0;
			int64_t stateLv = 0;
		};

		/// Запись состояния скилла в SKILL_TAR (4 поля).
		struct ActionTarStateEntry {
			int64_t stateId = 0;
			int64_t stateLv = 0;
			int64_t duration = 0;
			int64_t startTime = 0;
		};

		/// Данные действия MOVE.
		struct ActionMoveData {
			int64_t moveState = 0;
			int64_t stopState = 0;
			std::vector<uint8_t> waypoints; // бинарные Point[]
		};

		/// Данные действия SKILL_SRC (источник скилла).
		struct ActionSkillSrcData {
			int64_t fightId = 0;
			int64_t angle = 0;
			int64_t state = 0;
			int64_t stopState = 0;
			int64_t skillId = 0;
			int64_t skillSpeed = 0;
			int64_t targetType = 0;
			int64_t targetId = 0;
			int64_t targetX = 0;
			int64_t targetY = 0;
			int64_t execTime = 0;
			std::vector<ActionEffectEntry> effects;
			std::vector<ActionStateEntry> states;
		};

		/// Данные действия SKILL_TAR (цель скилла).
		struct ActionSkillTarData {
			int64_t fightId = 0;
			int64_t state = 0;
			bool doubleAttack = false;
			bool miss = false;
			bool beatBack = false;
			int64_t beatBackX = 0;
			int64_t beatBackY = 0;
			int64_t srcId = 0;
			int64_t srcPosX = 0;
			int64_t srcPosY = 0;
			int64_t skillId = 0;
			int64_t skillTargetX = 0;
			int64_t skillTargetY = 0;
			int64_t execTime = 0;
			int64_t synType = 0;
			std::vector<ActionEffectEntry> effects;
			bool hasStates = false;
			int64_t stateTime = 0;
			std::vector<ActionTarStateEntry> states;
			bool hasSrcEffect = false;
			int64_t srcState = 0;
			int64_t srcSynType = 0;
			std::vector<ActionEffectEntry> srcEffects;
		};

		/// Данные действия LEAN (наклон/присаживание).
		struct ActionLeanData {
			int64_t leanState = 0;
			int64_t pose = 0;
			int64_t angle = 0;
			int64_t posX = 0;
			int64_t posY = 0;
			int64_t height = 0;
		};

		/// Данные действия FACE / SKILL_POSE.
		struct ActionFaceData {
			int64_t angle = 0;
			int64_t pose = 0;
		};

		/// CMD_MC_NOTIACTION — уведомление о действии персонажа.
		/// actionType определяет, какое из полей-данных заполнено.
		struct McCharacterActionMessage {
			int64_t worldId = 0;
			int64_t packetId = 0;
			int64_t actionType = 0;
			// Одно из этих полей заполнено в зависимости от actionType:
			ActionMoveData move;
			ActionSkillSrcData skillSrc;
			ActionSkillTarData skillTar;
			ActionLeanData lean;
			ActionFaceData face;
			int64_t itemFailedId = 0;
			int64_t tempItemId = 0;
			int64_t tempPartId = 0;
			int64_t changeChaMainId = 0;
			ChaLookInfo look;
			ChaKitbagInfo kitbag;
			ChaShortcutInfo shortcut;
		};

		// ─── Serialize: cmd-only ───────────────────────────────────

		inline WPacket serializeCmOfflineModeCmd() { WPacket w(8); w.WriteCmd(CMD_CM_OFFLINE_MODE); return w; }
		inline WPacket serializeCmCancelExitCmd() { WPacket w(8); w.WriteCmd(CMD_CM_CANCELEXIT); return w; }
		inline WPacket serializeCmKitbagLockCmd() { WPacket w(8); w.WriteCmd(CMD_CM_KITBAG_LOCK); return w; }
		inline WPacket serializeCmKitbagCheckCmd() { WPacket w(8); w.WriteCmd(CMD_CM_KITBAG_CHECK); return w; }
		inline WPacket serializeCmKitbagTempSyncCmd() { WPacket w(8); w.WriteCmd(CMD_CM_KITBAGTEMP_SYNC); return w; }
		inline WPacket serializeCmReadBookStartCmd() { WPacket w(8); w.WriteCmd(CMD_CM_READBOOK_START); return w; }
		inline WPacket serializeCmReadBookCloseCmd() { WPacket w(8); w.WriteCmd(CMD_CM_READBOOK_CLOSE); return w; }
		inline WPacket serializeCmBoatCancelCmd() { WPacket w(8); w.WriteCmd(CMD_CM_BOAT_CANCEL); return w; }
		inline WPacket serializeCmBoatGetInfoCmd() { WPacket w(8); w.WriteCmd(CMD_CM_BOAT_GETINFO); return w; }
		inline WPacket serializeCmVolunteerAddCmd() { WPacket w(8); w.WriteCmd(CMD_CM_VOLUNTER_ADD); return w; }
		inline WPacket serializeCmVolunteerDelCmd() { WPacket w(8); w.WriteCmd(CMD_CM_VOLUNTER_DEL); return w; }
		inline WPacket serializeCmStoreCloseCmd() { WPacket w(8); w.WriteCmd(CMD_CM_STORE_CLOSE); return w; }
		inline WPacket serializeCmStallCloseCmd() { WPacket w(8); w.WriteCmd(CMD_CM_STALL_CLOSE); return w; }
		inline WPacket serializeCmMisLogCmd() { WPacket w(8); w.WriteCmd(CMD_CM_MISLOG); return w; }
		inline WPacket serializeCmDailyBuffRequestCmd() { WPacket w(8); w.WriteCmd(CMD_CM_DailyBuffRequest); return w; }
		inline WPacket serializeCmRequestDropRateCmd() { WPacket w(8); w.WriteCmd(CMD_CM_REQUEST_DROP_RATE); return w; }
		inline WPacket serializeCmRequestExpRateCmd() { WPacket w(8); w.WriteCmd(CMD_CM_REQUEST_EXP_RATE); return w; }

		// ─── Serialize: CM с полями ────────────────────────────────

		inline WPacket serialize(const CmDieReturnMessage& m) { WPacket w(16); w.WriteCmd(CMD_CM_DIE_RETURN); w.WriteInt64(m.reliveType); return w; }
		inline WPacket serialize(const CmAutoKitbagLockMessage& m) { WPacket w(16); w.WriteCmd(CMD_CM_KITBAG_AUTOLOCK); w.WriteInt64(m.autoLock); return w; }
		inline WPacket serialize(const CmStallSearchMessage& m) { WPacket w(16); w.WriteCmd(CMD_CM_STALLSEARCH); w.WriteInt64(m.itemId); return w; }
		inline WPacket serialize(const CmForgeItemMessage& m) { WPacket w(16); w.WriteCmd(CMD_CM_FORGE); w.WriteInt64(m.index); return w; }
		inline WPacket serialize(const CmEntityEventMessage& m) { WPacket w(16); w.WriteCmd(CMD_CM_ENTITY_EVENT); w.WriteInt64(m.entityId); return w; }
		inline WPacket serialize(const CmStallOpenMessage& m) { WPacket w(16); w.WriteCmd(CMD_CM_STALL_OPEN); w.WriteInt64(m.charId); return w; }
		inline WPacket serialize(const CmMisLogInfoMessage& m) { WPacket w(16); w.WriteCmd(CMD_CM_MISLOGINFO); w.WriteInt64(m.id); return w; }
		inline WPacket serialize(const CmMisLogClearMessage& m) { WPacket w(16); w.WriteCmd(CMD_CM_MISLOG_CLEAR); w.WriteInt64(m.id); return w; }
		inline WPacket serialize(const CmStoreBuyAskMessage& m) { WPacket w(16); w.WriteCmd(CMD_CM_STORE_BUY_ASK); w.WriteInt64(m.comId); return w; }
		inline WPacket serialize(const CmStoreChangeAskMessage& m) { WPacket w(16); w.WriteCmd(CMD_CM_STORE_CHANGE_ASK); w.WriteInt64(m.num); return w; }
		inline WPacket serialize(const CmStoreQueryMessage& m) { WPacket w(16); w.WriteCmd(CMD_CM_STORE_QUERY); w.WriteInt64(m.num); return w; }
		inline WPacket serialize(const CmTeamFightAnswerMessage& m) { WPacket w(16); w.WriteCmd(CMD_CM_TEAM_FIGHT_ASR); w.WriteInt64(m.accept); return w; }
		inline WPacket serialize(const CmItemRepairAnswerMessage& m) { WPacket w(16); w.WriteCmd(CMD_CM_ITEM_REPAIR_ASR); w.WriteInt64(m.accept); return w; }
		inline WPacket serialize(const CmItemForgeAnswerMessage& m) { WPacket w(16); w.WriteCmd(CMD_CM_ITEM_FORGE_ASR); w.WriteInt64(m.accept); return w; }
		inline WPacket serialize(const CmItemLotteryAnswerMessage& m) { WPacket w(16); w.WriteCmd(CMD_CM_ITEM_LOTTERY_ASR); w.WriteInt64(m.accept); return w; }
		inline WPacket serialize(const CmVolunteerOpenMessage& m) { WPacket w(16); w.WriteCmd(CMD_CM_VOLUNTER_OPEN); w.WriteInt64(m.num); return w; }
		inline WPacket serialize(const CmVolunteerListMessage& m) { WPacket w(24); w.WriteCmd(CMD_CM_VOLUNTER_LIST); w.WriteInt64(m.page); w.WriteInt64(m.num); return w; }
		inline WPacket serialize(const CmStoreListAskMessage& m) { WPacket w(32); w.WriteCmd(CMD_CM_STORE_LIST_ASK); w.WriteInt64(m.clsId); w.WriteInt64(m.page); w.WriteInt64(m.num); return w; }
		inline WPacket serialize(const CmCaptainConfirmAsrMessage& m) { WPacket w(24); w.WriteCmd(CMD_CM_CAPTAIN_CONFIRM_ASR); w.WriteInt64(m.ret); w.WriteInt64(m.teamId); return w; }
		inline WPacket serialize(const CmMapMaskMessage& m) { WPacket w(64); w.WriteCmd(CMD_CM_MAP_MASK); w.WriteString(m.mapName); return w; }
		inline WPacket serialize(const CmStoreOpenAskMessage& m) { WPacket w(64); w.WriteCmd(CMD_CM_STORE_OPEN_ASK); w.WriteString(m.password); return w; }
		inline WPacket serialize(const CmVolunteerSelMessage& m) { WPacket w(64); w.WriteCmd(CMD_CM_VOLUNTER_SEL); w.WriteString(m.name); return w; }
		inline WPacket serialize(const CmKitbagUnlockMessage& m) { WPacket w(64); w.WriteCmd(CMD_CM_KITBAG_UNLOCK); w.WriteString(m.password); return w; }

		// ─── Serialize: MC простые ─────────────────────────────────

		inline WPacket serialize(const McFailedActionMessage& m) { WPacket w(32); w.WriteCmd(CMD_MC_FAILEDACTION); w.WriteInt64(m.worldId); w.WriteInt64(m.actionType); w.WriteInt64(m.reason); return w; }
		inline WPacket serialize(const McChaEndSeeMessage& m) { WPacket w(24); w.WriteCmd(CMD_MC_CHAENDSEE); w.WriteInt64(m.seeType); w.WriteInt64(m.worldId); return w; }
		inline WPacket serialize(const McItemDestroyMessage& m) { WPacket w(16); w.WriteCmd(CMD_MC_ITEMENDSEE); w.WriteInt64(m.worldId); return w; }
		inline WPacket serialize(const McForgeResultMessage& m) { WPacket w(16); w.WriteCmd(CMD_MC_BEGIN_ITEM_FORGE); w.WriteInt64(m.result); return w; }
		inline WPacket serialize(const McUniteResultMessage& m) { WPacket w(16); w.WriteCmd(CMD_MC_BEGIN_ITEM_UNITE); w.WriteInt64(m.result); return w; }
		inline WPacket serialize(const McMillingResultMessage& m) { WPacket w(16); w.WriteCmd(CMD_MC_BEGIN_ITEM_MILLING); w.WriteInt64(m.result); return w; }
		inline WPacket serialize(const McFusionResultMessage& m) { WPacket w(16); w.WriteCmd(CMD_MC_BEGIN_ITEM_FUSION); w.WriteInt64(m.result); return w; }
		inline WPacket serialize(const McUpgradeResultMessage& m) { WPacket w(16); w.WriteCmd(CMD_MC_BEGIN_ITEM_UPGRADE); w.WriteInt64(m.result); return w; }
		inline WPacket serialize(const McPurifyResultMessage& m) { WPacket w(16); w.WriteCmd(CMD_MC_BEGIN_ITEM_PURIFY); w.WriteInt64(m.result); return w; }
		inline WPacket serialize(const McFixResultMessage& m) { WPacket w(16); w.WriteCmd(CMD_MC_BEGIN_ITEM_FIX); w.WriteInt64(m.result); return w; }
		inline WPacket serialize(const McEidolonMetempsychosisMessage& m) { WPacket w(16); w.WriteCmd(CMD_MC_BEGIN_ITEM_EIDOLON_METEMPSYCHOSIS); w.WriteInt64(m.result); return w; }
		inline WPacket serialize(const McEidolonFusionMessage& m) { WPacket w(16); w.WriteCmd(CMD_MC_BEGIN_ITEM_EIDOLON_FUSION); w.WriteInt64(m.result); return w; }
		inline WPacket serialize(const McMessageMessage& m) { WPacket w(64); w.WriteCmd(CMD_MC_MESSAGE); w.WriteString(m.text); return w; }
		inline WPacket serialize(const McBickerNoticeMessage& m) { WPacket w(64); w.WriteCmd(CMD_MC_BICKER_NOTICE); w.WriteString(m.text); return w; }
		inline WPacket serialize(const McColourNoticeMessage& m) { WPacket w(64); w.WriteCmd(CMD_MC_COLOUR_NOTICE); w.WriteInt64(m.color); w.WriteString(m.text); return w; }
		inline WPacket serialize(const McTriggerActionMessage& m) { WPacket w(40); w.WriteCmd(CMD_MC_TRIGGER_ACTION); w.WriteInt64(m.type); w.WriteInt64(m.id); w.WriteInt64(m.num); w.WriteInt64(m.count); return w; }
		inline WPacket serialize(const McNpcStateChangeMessage& m) { WPacket w(24); w.WriteCmd(CMD_MC_NPCSTATECHG); w.WriteInt64(m.npcId); w.WriteInt64(m.state); return w; }
		inline WPacket serialize(const McEntityStateChangeMessage& m) { WPacket w(24); w.WriteCmd(CMD_MC_ENTITY_CHGSTATE); w.WriteInt64(m.entityId); w.WriteInt64(m.state); return w; }
		inline WPacket serialize(const McCloseTalkMessage& m) { WPacket w(16); w.WriteCmd(CMD_MC_CLOSETALK); w.WriteInt64(m.npcId); return w; }
		inline WPacket serialize(const McKitbagCheckAnswerMessage& m) { WPacket w(16); w.WriteCmd(CMD_MC_KITBAG_CHECK_ASR); w.WriteInt64(m.locked); return w; }
		inline WPacket serialize(const McPreMoveTimeMessage& m) { WPacket w(16); w.WriteCmd(CMD_MC_PREMOVE_TIME); w.WriteInt64(m.time); return w; }
		inline WPacket serialize(const McItemUseSuccMessage& m) { WPacket w(24); w.WriteCmd(CMD_MC_ITEM_USE_SUC); w.WriteInt64(m.worldId); w.WriteInt64(m.itemId); return w; }
		inline WPacket serialize(const McChaPlayEffectMessage& m) { WPacket w(24); w.WriteCmd(CMD_MC_CHAPLAYEFFECT); w.WriteInt64(m.worldId); w.WriteInt64(m.effectId); return w; }
		inline WPacket serialize(const McSynDefaultSkillMessage& m) { WPacket w(24); w.WriteCmd(CMD_MC_SYNDEFAULTSKILL); w.WriteInt64(m.worldId); w.WriteInt64(m.skillId); return w; }

		// ─── Serialize: MC — ReadSequence→ReadString ───────────────

		inline WPacket serialize(const McSayMessage& m) { WPacket w(128); w.WriteCmd(CMD_MC_SAY); w.WriteInt64(m.sourceId); w.WriteString(m.content); w.WriteInt64(m.color); return w; }
		inline WPacket serialize(const McPopupNoticeMessage& m) { WPacket w(128); w.WriteCmd(CMD_MC_ALARM); w.WriteString(m.notice); return w; }
		inline WPacket serialize(const McPingMessage& m) { WPacket w(48); w.WriteCmd(CMD_MC_PING); w.WriteInt64(m.v1); w.WriteInt64(m.v2); w.WriteInt64(m.v3); w.WriteInt64(m.v4); w.WriteInt64(m.v5); return w; }

		// ─── Serialize: CM — средние (Фаза 2) ────────────────────────

		inline WPacket serialize(const CmChangePassMessage& m) { WPacket w(128); w.WriteCmd(CMD_CP_CHANGEPASS); w.WriteString(m.pass); w.WriteString(m.pin); return w; }
		inline WPacket serialize(const CmGuildBankOperMessage& m) { WPacket w(56); w.WriteCmd(CMD_CP_GUILDBANK); w.WriteInt64(m.op); w.WriteInt64(m.srcType); w.WriteInt64(m.srcId); w.WriteInt64(m.srcNum); w.WriteInt64(m.tarType); w.WriteInt64(m.tarId); return w; }
		inline WPacket serialize(const CmGuildBankGoldMessage& m) { WPacket w(32); w.WriteCmd(CMD_CP_GUILDBANK); w.WriteInt64(m.op); w.WriteInt64(m.direction); w.WriteInt64(m.gold); return w; }
		inline WPacket serialize(const CmUpdateHairMessage& m) { WPacket w(48); w.WriteCmd(CMD_CM_UPDATEHAIR); w.WriteInt64(m.scriptId); w.WriteInt64(m.gridLoc0); w.WriteInt64(m.gridLoc1); w.WriteInt64(m.gridLoc2); w.WriteInt64(m.gridLoc3); return w; }
		inline WPacket serialize(const CmTeamFightAskMessage& m) { WPacket w(32); w.WriteCmd(CMD_CM_TEAM_FIGHT_ASK); w.WriteInt64(m.type); w.WriteInt64(m.worldId); w.WriteInt64(m.handle); return w; }
		inline WPacket serialize(const CmItemRepairAskMessage& m) { WPacket w(40); w.WriteCmd(CMD_CM_ITEM_REPAIR_ASK); w.WriteInt64(m.repairmanId); w.WriteInt64(m.repairmanHandle); w.WriteInt64(m.posType); w.WriteInt64(m.posId); return w; }
		inline WPacket serialize(const CmRequestTradeMessage& m) { WPacket w(24); w.WriteCmd(CMD_CM_CHARTRADE_REQUEST); w.WriteInt64(m.type); w.WriteInt64(m.charId); return w; }
		inline WPacket serialize(const CmAcceptTradeMessage& m) { WPacket w(24); w.WriteCmd(CMD_CM_CHARTRADE_ACCEPT); w.WriteInt64(m.type); w.WriteInt64(m.charId); return w; }
		inline WPacket serialize(const CmCancelTradeMessage& m) { WPacket w(24); w.WriteCmd(CMD_CM_CHARTRADE_CANCEL); w.WriteInt64(m.type); w.WriteInt64(m.charId); return w; }
		inline WPacket serialize(const CmValidateTradeDataMessage& m) { WPacket w(24); w.WriteCmd(CMD_CM_CHARTRADE_VALIDATEDATA); w.WriteInt64(m.type); w.WriteInt64(m.charId); return w; }
		inline WPacket serialize(const CmValidateTradeMessage& m) { WPacket w(24); w.WriteCmd(CMD_CM_CHARTRADE_VALIDATE); w.WriteInt64(m.type); w.WriteInt64(m.charId); return w; }
		inline WPacket serialize(const CmAddItemMessage& m) { WPacket w(56); w.WriteCmd(CMD_CM_CHARTRADE_ITEM); w.WriteInt64(m.type); w.WriteInt64(m.charId); w.WriteInt64(m.opType); w.WriteInt64(m.index); w.WriteInt64(m.itemIndex); w.WriteInt64(m.count); return w; }
		inline WPacket serialize(const CmAddMoneyMessage& m) { WPacket w(48); w.WriteCmd(CMD_CM_CHARTRADE_MONEY); w.WriteInt64(m.type); w.WriteInt64(m.charId); w.WriteInt64(m.opType); w.WriteInt64(m.isImp); w.WriteInt64(m.money); return w; }
		inline WPacket serialize(const CmTigerStartMessage& m) { WPacket w(40); w.WriteCmd(CMD_CM_TIGER_START); w.WriteInt64(m.npcId); w.WriteInt64(m.sel1); w.WriteInt64(m.sel2); w.WriteInt64(m.sel3); return w; }
		inline WPacket serialize(const CmTigerStopMessage& m) { WPacket w(24); w.WriteCmd(CMD_CM_TIGER_STOP); w.WriteInt64(m.npcId); w.WriteInt64(m.num); return w; }
		inline WPacket serialize(const CmVolunteerAsrMessage& m) { WPacket w(80); w.WriteCmd(CMD_CM_VOLUNTER_ASR); w.WriteInt64(m.ret); w.WriteString(m.name); return w; }
		inline WPacket serialize(const CmCreateBoatMessage& m) { WPacket w(96); w.WriteCmd(CMD_CM_CREATE_BOAT); w.WriteString(m.boat); w.WriteInt64(m.header); w.WriteInt64(m.engine); w.WriteInt64(m.cannon); w.WriteInt64(m.equipment); return w; }
		inline WPacket serialize(const CmUpdateBoatMessage& m) { WPacket w(40); w.WriteCmd(CMD_CM_UPDATEBOAT_PART); w.WriteInt64(m.header); w.WriteInt64(m.engine); w.WriteInt64(m.cannon); w.WriteInt64(m.equipment); return w; }
		inline WPacket serialize(const CmSelectBoatListMessage& m) { WPacket w(32); w.WriteCmd(CMD_CM_BOAT_SELECT); w.WriteInt64(m.npcId); w.WriteInt64(m.type); w.WriteInt64(m.index); return w; }
		inline WPacket serialize(const CmBoatLaunchMessage& m) { WPacket w(24); w.WriteCmd(CMD_CM_BOAT_LUANCH); w.WriteInt64(m.npcId); w.WriteInt64(m.index); return w; }
		inline WPacket serialize(const CmBoatBagSelMessage& m) { WPacket w(24); w.WriteCmd(CMD_CM_BOAT_BAGSEL); w.WriteInt64(m.npcId); w.WriteInt64(m.index); return w; }
		inline WPacket serialize(const CmReportWgMessage& m) { WPacket w(128); w.WriteCmd(CMD_CP_REPORT_WG); w.WriteString(m.info); return w; }
		inline WPacket serialize(const CmSay2CampMessage& m) { WPacket w(128); w.WriteCmd(CMD_CM_SAY2CAMP); w.WriteString(m.content); return w; }
		inline WPacket serialize(const CmStallBuyMessage& m) { WPacket w(40); w.WriteCmd(CMD_CM_STALL_BUY); w.WriteInt64(m.charId); w.WriteInt64(m.index); w.WriteInt64(m.count); w.WriteInt64(m.gridId); return w; }
		inline WPacket serialize(const CmSkillUpgradeMessage& m) { WPacket w(24); w.WriteCmd(CMD_CM_SKILLUPGRADE); w.WriteInt64(m.skillId); w.WriteInt64(m.addGrade); return w; }
		inline WPacket serialize(const CmRefreshDataMessage& m) { WPacket w(24); w.WriteCmd(CMD_CM_REFRESH_DATA); w.WriteInt64(m.worldId); w.WriteInt64(m.handle); return w; }
		inline WPacket serialize(const CmPkCtrlMessage& m) { WPacket w(16); w.WriteCmd(CMD_CM_PK_CTRL); w.WriteInt64(m.ctrl); return w; }
		inline WPacket serialize(const CmItemAmphitheaterAskMessage& m) { WPacket w(24); w.WriteCmd(CMD_CM_ITEM_AMPHITHEATER_ASK); w.WriteInt64(m.sure); w.WriteInt64(m.reId); return w; }
		inline WPacket serialize(const CmMasterInviteMessage& m) { WPacket w(16); w.WriteCmd(CMD_CM_MASTER_INVITE); w.WriteInt64(m.masterId); return w; }
		inline WPacket serialize(const CmMasterAsrMessage& m) { WPacket w(24); w.WriteCmd(CMD_CM_MASTER_ASR); w.WriteInt64(m.agree); w.WriteInt64(m.masterId); return w; }
		inline WPacket serialize(const CmMasterDelMessage& m) { WPacket w(16); w.WriteCmd(CMD_CM_MASTER_DEL); w.WriteInt64(m.masterId); return w; }
		inline WPacket serialize(const CmPrenticeInviteMessage& m) { WPacket w(16); w.WriteCmd(CMD_CM_PRENTICE_INVITE); w.WriteInt64(m.prenticeId); return w; }
		inline WPacket serialize(const CmPrenticeAsrMessage& m) { WPacket w(24); w.WriteCmd(CMD_CM_PRENTICE_ASR); w.WriteInt64(m.agree); w.WriteInt64(m.prenticeId); return w; }
		inline WPacket serialize(const CmPrenticeDelMessage& m) { WPacket w(16); w.WriteCmd(CMD_CM_PRENTICE_DEL); w.WriteInt64(m.prenticeId); return w; }

		// ─── Serialize: NPC Talk составные (Фаза 2) ──────────────────

		// CmRequestTalkMessage: [npcId, CMD_CM_TALKPAGE, cmd]
		inline WPacket serialize(const CmRequestTalkMessage& m) {
			WPacket w(32); w.WriteCmd(CMD_CM_REQUESTTALK);
			w.WriteInt64(m.npcId); w.WriteInt64(CMD_CM_TALKPAGE); w.WriteInt64(m.cmd); return w;
		}
		// CmSelFunctionMessage: [npcId, CMD_CM_FUNCITEM, pageId, index]
		inline WPacket serialize(const CmSelFunctionMessage& m) {
			WPacket w(40); w.WriteCmd(CMD_CM_REQUESTTALK);
			w.WriteInt64(m.npcId); w.WriteInt64(CMD_CM_FUNCITEM); w.WriteInt64(m.pageId); w.WriteInt64(m.index); return w;
		}
		// CmSaleMessage: [npcId, CMD_CM_TRADEITEM, ROLE_TRADE_SALE(0), index, count]
		inline WPacket serialize(const CmSaleMessage& m) {
			WPacket w(48); w.WriteCmd(CMD_CM_REQUESTTRADE);
			w.WriteInt64(m.npcId); w.WriteInt64(CMD_CM_TRADEITEM); w.WriteInt64(0); w.WriteInt64(m.index); w.WriteInt64(m.count); return w;
		}
		// CmBuyMessage: [npcId, CMD_CM_TRADEITEM, ROLE_TRADE_BUY(1), itemType, index1, index2, count]
		inline WPacket serialize(const CmBuyMessage& m) {
			WPacket w(64); w.WriteCmd(CMD_CM_REQUESTTRADE);
			w.WriteInt64(m.npcId); w.WriteInt64(CMD_CM_TRADEITEM); w.WriteInt64(1); w.WriteInt64(m.itemType); w.WriteInt64(m.index1); w.WriteInt64(m.index2); w.WriteInt64(m.count); return w;
		}
		// CmMissionPageMessage: [npcId, CMD_CM_MISSION, cmd, selItem, param]
		inline WPacket serialize(const CmMissionPageMessage& m) {
			WPacket w(48); w.WriteCmd(CMD_CM_REQUESTTALK);
			w.WriteInt64(m.npcId); w.WriteInt64(CMD_CM_MISSION); w.WriteInt64(m.cmd); w.WriteInt64(m.selItem); w.WriteInt64(m.param); return w;
		}
		// CmSelMissionMessage: [npcId, CMD_CM_MISSION, ROLE_MIS_SEL(4), index]
		inline WPacket serialize(const CmSelMissionMessage& m) {
			WPacket w(40); w.WriteCmd(CMD_CM_REQUESTTALK);
			w.WriteInt64(m.npcId); w.WriteInt64(CMD_CM_MISSION); w.WriteInt64(4); w.WriteInt64(m.index); return w;
		}
		// CmBlackMarketExchangeReqMessage: [npcId, CMD_CM_BLACKMARKET_EXCHANGE_REQ, timeNum, srcId, srcNum, tarId, tarNum, index]
		inline WPacket serialize(const CmBlackMarketExchangeReqMessage& m) {
			WPacket w(72); w.WriteCmd(CMD_CM_REQUESTTRADE);
			w.WriteInt64(m.npcId); w.WriteInt64(CMD_CM_BLACKMARKET_EXCHANGE_REQ); w.WriteInt64(m.timeNum);
			w.WriteInt64(m.srcId); w.WriteInt64(m.srcNum); w.WriteInt64(m.tarId); w.WriteInt64(m.tarNum); w.WriteInt64(m.index); return w;
		}

		// ─── Serialize: MC — средние (Фаза 2) ────────────────────────

		inline WPacket serialize(const McSay2CampMessage& m) { WPacket w(128); w.WriteCmd(CMD_MC_SAY2CAMP); w.WriteString(m.chaName); w.WriteString(m.content); return w; }
		inline WPacket serialize(const McTalkInfoMessage& m) { WPacket w(128); w.WriteCmd(CMD_MC_TALKPAGE); w.WriteInt64(m.npcId); w.WriteInt64(m.cmd); w.WriteString(m.text); return w; }
		inline WPacket serialize(const McTradeDataMessage& m) { WPacket w(56); w.WriteCmd(CMD_MC_TRADE_DATA); w.WriteInt64(m.npcId); w.WriteInt64(m.page); w.WriteInt64(m.index); w.WriteInt64(m.itemId); w.WriteInt64(m.count); w.WriteInt64(m.price); return w; }
		inline WPacket serialize(const McTradeResultMessage& m) { WPacket w(48); w.WriteCmd(CMD_MC_TRADERESULT); w.WriteInt64(m.type); w.WriteInt64(m.index); w.WriteInt64(m.count); w.WriteInt64(m.itemId); w.WriteInt64(m.money); return w; }
		inline WPacket serialize(const McUpdateHairResMessage& m) { WPacket w(96); w.WriteCmd(CMD_MC_UPDATEHAIR_RES); w.WriteInt64(m.worldId); w.WriteInt64(m.scriptId); w.WriteString(m.reason); return w; }
		inline WPacket serialize(const McSynTLeaderIdMessage& m) { WPacket w(24); w.WriteCmd(CMD_MC_TLEADER_ID); w.WriteInt64(m.worldId); w.WriteInt64(m.leaderId); return w; }
		inline WPacket serialize(const McKitbagCapacityMessage& m) { WPacket w(24); w.WriteCmd(CMD_MC_KITBAG_CAPACITY); w.WriteInt64(m.worldId); w.WriteInt64(m.capacity); return w; }
		inline WPacket serialize(const McItemForgeAskMessage& m) { WPacket w(24); w.WriteCmd(CMD_MC_ITEM_FORGE_ASK); w.WriteInt64(m.type); w.WriteInt64(m.money); return w; }
		inline WPacket serialize(const McItemForgeAnswerMessage& m) { WPacket w(32); w.WriteCmd(CMD_MC_ITEM_FORGE_ASR); w.WriteInt64(m.worldId); w.WriteInt64(m.type); w.WriteInt64(m.result); return w; }
		inline WPacket serialize(const McQueryChaMessage& m) { WPacket w(128); w.WriteCmd(CMD_MC_QUERY_CHA); w.WriteInt64(m.chaId); w.WriteString(m.name); w.WriteString(m.mapName); w.WriteInt64(m.posX); w.WriteInt64(m.posY); w.WriteInt64(m.chaId2); return w; }
		inline WPacket serialize(const McQueryChaPingMessage& m) { WPacket w(128); w.WriteCmd(CMD_MC_QUERY_CHAPING); w.WriteInt64(m.chaId); w.WriteString(m.name); w.WriteString(m.mapName); w.WriteInt64(m.ping); return w; }
		inline WPacket serialize(const McQueryReliveMessage& m) { WPacket w(96); w.WriteCmd(CMD_MC_QUERY_RELIVE); w.WriteInt64(m.chaId); w.WriteString(m.sourceName); w.WriteInt64(m.reliveType); return w; }
		inline WPacket serialize(const McGmMailMessage& m) { WPacket w(256); w.WriteCmd(CMD_MC_GM_MAIL); w.WriteString(m.title); w.WriteString(m.content); w.WriteInt64(m.time); return w; }
		inline WPacket serialize(const McSynStallNameMessage& m) { WPacket w(64); w.WriteCmd(CMD_MC_STALL_NAME); w.WriteInt64(m.charId); w.WriteString(m.name); return w; }
		inline WPacket serialize(const McMapCrashMessage& m) { WPacket w(128); w.WriteCmd(CMD_MC_MAPCRASH); w.WriteString(m.text); return w; }
		inline WPacket serialize(const McVolunteerStateMessage& m) { WPacket w(16); w.WriteCmd(CMD_MC_VOLUNTER_STATE); w.WriteInt64(m.state); return w; }
		inline WPacket serialize(const McVolunteerAskMessage& m) { WPacket w(64); w.WriteCmd(CMD_MC_VOLUNTER_ASK); w.WriteString(m.name); return w; }
		inline WPacket serialize(const McMasterAskMessage& m) { WPacket w(80); w.WriteCmd(CMD_MC_MASTER_ASK); w.WriteString(m.name); w.WriteInt64(m.chaId); return w; }
		inline WPacket serialize(const McPrenticeAskMessage& m) { WPacket w(80); w.WriteCmd(CMD_MC_PRENTICE_ASK); w.WriteString(m.name); w.WriteInt64(m.chaId); return w; }
		inline WPacket serialize(const McItemRepairAskMcMessage& m) { WPacket w(80); w.WriteCmd(CMD_MC_ITEM_REPAIR_ASK); w.WriteString(m.itemName); w.WriteInt64(m.repairCost); return w; }
		inline WPacket serialize(const McItemLotteryAsrMessage& m) { WPacket w(16); w.WriteCmd(CMD_MC_ITEM_LOTTERY_ASR); w.WriteInt64(m.success); return w; }

		// ─── Deserialize: CM с полями ──────────────────────────────

		inline void deserialize(RPacket& pk, CmDieReturnMessage& m) { m.reliveType = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, CmAutoKitbagLockMessage& m) { m.autoLock = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, CmStallSearchMessage& m) { m.itemId = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, CmForgeItemMessage& m) { m.index = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, CmEntityEventMessage& m) { m.entityId = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, CmStallOpenMessage& m) { m.charId = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, CmMisLogInfoMessage& m) { m.id = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, CmMisLogClearMessage& m) { m.id = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, CmStoreBuyAskMessage& m) { m.comId = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, CmStoreChangeAskMessage& m) { m.num = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, CmStoreQueryMessage& m) { m.num = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, CmTeamFightAnswerMessage& m) { m.accept = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, CmItemRepairAnswerMessage& m) { m.accept = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, CmItemForgeAnswerMessage& m) { m.accept = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, CmItemLotteryAnswerMessage& m) { m.accept = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, CmVolunteerOpenMessage& m) { m.num = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, CmVolunteerListMessage& m) { m.page = pk.ReadInt64(); m.num = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, CmStoreListAskMessage& m) { m.clsId = pk.ReadInt64(); m.page = pk.ReadInt64(); m.num = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, CmCaptainConfirmAsrMessage& m) { m.ret = pk.ReadInt64(); m.teamId = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, CmMapMaskMessage& m) { m.mapName = pk.ReadString(); }
		inline void deserialize(RPacket& pk, CmStoreOpenAskMessage& m) { m.password = pk.ReadString(); }
		inline void deserialize(RPacket& pk, CmVolunteerSelMessage& m) { m.name = pk.ReadString(); }
		inline void deserialize(RPacket& pk, CmKitbagUnlockMessage& m) { m.password = pk.ReadString(); }

		// ─── Deserialize: MC простые ───────────────────────────────

		inline void deserialize(RPacket& pk, McFailedActionMessage& m) { m.worldId = pk.ReadInt64(); m.actionType = pk.ReadInt64(); m.reason = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, McChaEndSeeMessage& m) { m.seeType = pk.ReadInt64(); m.worldId = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, McItemDestroyMessage& m) { m.worldId = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, McForgeResultMessage& m) { m.result = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, McUniteResultMessage& m) { m.result = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, McMillingResultMessage& m) { m.result = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, McFusionResultMessage& m) { m.result = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, McUpgradeResultMessage& m) { m.result = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, McPurifyResultMessage& m) { m.result = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, McFixResultMessage& m) { m.result = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, McEidolonMetempsychosisMessage& m) { m.result = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, McEidolonFusionMessage& m) { m.result = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, McMessageMessage& m) { m.text = pk.ReadString(); }
		inline void deserialize(RPacket& pk, McBickerNoticeMessage& m) { m.text = pk.ReadString(); }
		inline void deserialize(RPacket& pk, McColourNoticeMessage& m) { m.color = pk.ReadInt64(); m.text = pk.ReadString(); }
		inline void deserialize(RPacket& pk, McTriggerActionMessage& m) { m.type = pk.ReadInt64(); m.id = pk.ReadInt64(); m.num = pk.ReadInt64(); m.count = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, McNpcStateChangeMessage& m) { m.npcId = pk.ReadInt64(); m.state = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, McEntityStateChangeMessage& m) { m.entityId = pk.ReadInt64(); m.state = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, McCloseTalkMessage& m) { m.npcId = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, McKitbagCheckAnswerMessage& m) { m.locked = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, McPreMoveTimeMessage& m) { m.time = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, McItemUseSuccMessage& m) { m.worldId = pk.ReadInt64(); m.itemId = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, McChaPlayEffectMessage& m) { m.worldId = pk.ReadInt64(); m.effectId = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, McSynDefaultSkillMessage& m) { m.worldId = pk.ReadInt64(); m.skillId = pk.ReadInt64(); }

		// ─── Deserialize: MC — ReadSequence→ReadString ─────────────

		inline void deserialize(RPacket& pk, McSayMessage& m) { m.sourceId = pk.ReadInt64(); m.content = pk.ReadString(); m.color = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, McPopupNoticeMessage& m) { m.notice = pk.ReadString(); }
		inline void deserialize(RPacket& pk, McPingMessage& m) { m.v1 = pk.ReadInt64(); m.v2 = pk.ReadInt64(); m.v3 = pk.ReadInt64(); m.v4 = pk.ReadInt64(); m.v5 = pk.ReadInt64(); }

		// ─── Deserialize: CM — средние (Фаза 2) ──────────────────────

		inline void deserialize(RPacket& pk, CmChangePassMessage& m) { m.pass = pk.ReadString(); m.pin = pk.ReadString(); }
		inline void deserialize(RPacket& pk, CmGuildBankOperMessage& m) { m.op = pk.ReadInt64(); m.srcType = pk.ReadInt64(); m.srcId = pk.ReadInt64(); m.srcNum = pk.ReadInt64(); m.tarType = pk.ReadInt64(); m.tarId = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, CmGuildBankGoldMessage& m) { m.op = pk.ReadInt64(); m.direction = pk.ReadInt64(); m.gold = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, CmUpdateHairMessage& m) { m.scriptId = pk.ReadInt64(); m.gridLoc0 = pk.ReadInt64(); m.gridLoc1 = pk.ReadInt64(); m.gridLoc2 = pk.ReadInt64(); m.gridLoc3 = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, CmTeamFightAskMessage& m) { m.type = pk.ReadInt64(); m.worldId = pk.ReadInt64(); m.handle = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, CmItemRepairAskMessage& m) { m.repairmanId = pk.ReadInt64(); m.repairmanHandle = pk.ReadInt64(); m.posType = pk.ReadInt64(); m.posId = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, CmRequestTradeMessage& m) { m.type = pk.ReadInt64(); m.charId = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, CmAcceptTradeMessage& m) { m.type = pk.ReadInt64(); m.charId = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, CmCancelTradeMessage& m) { m.type = pk.ReadInt64(); m.charId = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, CmValidateTradeDataMessage& m) { m.type = pk.ReadInt64(); m.charId = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, CmValidateTradeMessage& m) { m.type = pk.ReadInt64(); m.charId = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, CmAddItemMessage& m) { m.type = pk.ReadInt64(); m.charId = pk.ReadInt64(); m.opType = pk.ReadInt64(); m.index = pk.ReadInt64(); m.itemIndex = pk.ReadInt64(); m.count = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, CmAddMoneyMessage& m) { m.type = pk.ReadInt64(); m.charId = pk.ReadInt64(); m.opType = pk.ReadInt64(); m.isImp = pk.ReadInt64(); m.money = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, CmTigerStartMessage& m) { m.npcId = pk.ReadInt64(); m.sel1 = pk.ReadInt64(); m.sel2 = pk.ReadInt64(); m.sel3 = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, CmTigerStopMessage& m) { m.npcId = pk.ReadInt64(); m.num = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, CmVolunteerAsrMessage& m) { m.ret = pk.ReadInt64(); m.name = pk.ReadString(); }
		inline void deserialize(RPacket& pk, CmCreateBoatMessage& m) { m.boat = pk.ReadString(); m.header = pk.ReadInt64(); m.engine = pk.ReadInt64(); m.cannon = pk.ReadInt64(); m.equipment = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, CmUpdateBoatMessage& m) { m.header = pk.ReadInt64(); m.engine = pk.ReadInt64(); m.cannon = pk.ReadInt64(); m.equipment = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, CmSelectBoatListMessage& m) { m.npcId = pk.ReadInt64(); m.type = pk.ReadInt64(); m.index = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, CmBoatLaunchMessage& m) { m.npcId = pk.ReadInt64(); m.index = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, CmBoatBagSelMessage& m) { m.npcId = pk.ReadInt64(); m.index = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, CmReportWgMessage& m) { m.info = pk.ReadString(); }
		inline void deserialize(RPacket& pk, CmSay2CampMessage& m) { m.content = pk.ReadString(); }
		inline void deserialize(RPacket& pk, CmStallBuyMessage& m) { m.charId = pk.ReadInt64(); m.index = pk.ReadInt64(); m.count = pk.ReadInt64(); m.gridId = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, CmSkillUpgradeMessage& m) { m.skillId = pk.ReadInt64(); m.addGrade = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, CmRefreshDataMessage& m) { m.worldId = pk.ReadInt64(); m.handle = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, CmPkCtrlMessage& m) { m.ctrl = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, CmItemAmphitheaterAskMessage& m) { m.sure = pk.ReadInt64(); m.reId = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, CmMasterInviteMessage& m) { m.masterId = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, CmMasterAsrMessage& m) { m.agree = pk.ReadInt64(); m.masterId = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, CmMasterDelMessage& m) { m.masterId = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, CmPrenticeInviteMessage& m) { m.prenticeId = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, CmPrenticeAsrMessage& m) { m.agree = pk.ReadInt64(); m.prenticeId = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, CmPrenticeDelMessage& m) { m.prenticeId = pk.ReadInt64(); }

		// ─── Deserialize: NPC Talk составные (Фаза 2) ────────────────

		inline void deserialize(RPacket& pk, CmRequestTalkMessage& m) {
			m.npcId = pk.ReadInt64(); pk.ReadInt64(); m.cmd = pk.ReadInt64();
		}
		inline void deserialize(RPacket& pk, CmSelFunctionMessage& m) {
			m.npcId = pk.ReadInt64(); pk.ReadInt64(); m.pageId = pk.ReadInt64(); m.index = pk.ReadInt64();
		}
		inline void deserialize(RPacket& pk, CmSaleMessage& m) {
			m.npcId = pk.ReadInt64(); pk.ReadInt64(); pk.ReadInt64(); m.index = pk.ReadInt64(); m.count = pk.ReadInt64();
		}
		inline void deserialize(RPacket& pk, CmBuyMessage& m) {
			m.npcId = pk.ReadInt64(); pk.ReadInt64(); pk.ReadInt64(); m.itemType = pk.ReadInt64(); m.index1 = pk.ReadInt64(); m.index2 = pk.ReadInt64(); m.count = pk.ReadInt64();
		}
		inline void deserialize(RPacket& pk, CmMissionPageMessage& m) {
			m.npcId = pk.ReadInt64(); pk.ReadInt64(); m.cmd = pk.ReadInt64(); m.selItem = pk.ReadInt64(); m.param = pk.ReadInt64();
		}
		inline void deserialize(RPacket& pk, CmSelMissionMessage& m) {
			m.npcId = pk.ReadInt64(); pk.ReadInt64(); pk.ReadInt64(); m.index = pk.ReadInt64();
		}
		inline void deserialize(RPacket& pk, CmBlackMarketExchangeReqMessage& m) {
			m.npcId = pk.ReadInt64(); pk.ReadInt64(); m.timeNum = pk.ReadInt64();
			m.srcId = pk.ReadInt64(); m.srcNum = pk.ReadInt64(); m.tarId = pk.ReadInt64(); m.tarNum = pk.ReadInt64(); m.index = pk.ReadInt64();
		}

		// ─── Deserialize: MC — средние (Фаза 2) ──────────────────────

		inline void deserialize(RPacket& pk, McSay2CampMessage& m) { m.chaName = pk.ReadString(); m.content = pk.ReadString(); }
		inline void deserialize(RPacket& pk, McTalkInfoMessage& m) { m.npcId = pk.ReadInt64(); m.cmd = pk.ReadInt64(); m.text = pk.ReadString(); }
		inline void deserialize(RPacket& pk, McTradeDataMessage& m) { m.npcId = pk.ReadInt64(); m.page = pk.ReadInt64(); m.index = pk.ReadInt64(); m.itemId = pk.ReadInt64(); m.count = pk.ReadInt64(); m.price = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, McTradeResultMessage& m) { m.type = pk.ReadInt64(); m.index = pk.ReadInt64(); m.count = pk.ReadInt64(); m.itemId = pk.ReadInt64(); m.money = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, McUpdateHairResMessage& m) { m.worldId = pk.ReadInt64(); m.scriptId = pk.ReadInt64(); m.reason = pk.ReadString(); }
		inline void deserialize(RPacket& pk, McSynTLeaderIdMessage& m) { m.worldId = pk.ReadInt64(); m.leaderId = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, McKitbagCapacityMessage& m) { m.worldId = pk.ReadInt64(); m.capacity = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, McItemForgeAskMessage& m) { m.type = pk.ReadInt64(); m.money = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, McItemForgeAnswerMessage& m) { m.worldId = pk.ReadInt64(); m.type = pk.ReadInt64(); m.result = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, McQueryChaMessage& m) { m.chaId = pk.ReadInt64(); m.name = pk.ReadString(); m.mapName = pk.ReadString(); m.posX = pk.ReadInt64(); m.posY = pk.ReadInt64(); m.chaId2 = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, McQueryChaPingMessage& m) { m.chaId = pk.ReadInt64(); m.name = pk.ReadString(); m.mapName = pk.ReadString(); m.ping = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, McQueryReliveMessage& m) { m.chaId = pk.ReadInt64(); m.sourceName = pk.ReadString(); m.reliveType = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, McGmMailMessage& m) { m.title = pk.ReadString(); m.content = pk.ReadString(); m.time = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, McSynStallNameMessage& m) { m.charId = pk.ReadInt64(); m.name = pk.ReadString(); }
		inline void deserialize(RPacket& pk, McMapCrashMessage& m) { m.text = pk.ReadString(); }
		inline void deserialize(RPacket& pk, McVolunteerStateMessage& m) { m.state = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, McVolunteerAskMessage& m) { m.name = pk.ReadString(); }
		inline void deserialize(RPacket& pk, McMasterAskMessage& m) { m.name = pk.ReadString(); m.chaId = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, McPrenticeAskMessage& m) { m.name = pk.ReadString(); m.chaId = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, McItemRepairAskMcMessage& m) { m.itemName = pk.ReadString(); m.repairCost = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, McItemLotteryAsrMessage& m) { m.success = pk.ReadInt64(); }

		// ─── Serialize: Фаза 3 ─────────────────────────────────────

		inline WPacket serialize(const McSynAttributeMessage& m) {
			WPacket w(256); w.WriteCmd(CMD_MC_SYNATTR);
			w.WriteInt64(m.worldId);
			serializeChaAttrInfo(w, m.attr);
			return w;
		}

		inline WPacket serialize(const McSynSkillStateMessage& m) {
			WPacket w(256); w.WriteCmd(CMD_MC_SYNASKILLSTATE);
			w.WriteInt64(m.worldId);
			serializeChaSkillStateInfo(w, m.skillState);
			return w;
		}

		inline WPacket serialize(const McAStateEndSeeMessage& m) {
			WPacket w(24); w.WriteCmd(CMD_MC_ASTATEENDSEE);
			w.WriteInt64(m.areaX); w.WriteInt64(m.areaY);
			return w;
		}

		inline WPacket serialize(const McAStateBeginSeeMessage& m) {
			WPacket w(128); w.WriteCmd(CMD_MC_ASTATEBEGINSEE);
			w.WriteInt64(m.areaX); w.WriteInt64(m.areaY);
			w.WriteInt64(static_cast<int64_t>(m.states.size()));
			for (auto& s : m.states) {
				w.WriteInt64(s.stateId);
				if (s.stateId != 0) {
					w.WriteInt64(s.stateLv);
					w.WriteInt64(s.worldId);
					w.WriteInt64(s.fightId);
				}
			}
			return w;
		}

		inline WPacket serialize(const McDelItemChaMessage& m) {
			WPacket w(24); w.WriteCmd(CMD_MC_DEL_ITEM_CHA);
			w.WriteInt64(m.mainChaId); w.WriteInt64(m.worldId); return w;
		}

		inline WPacket serialize(const McSynEventInfoMessage& m) {
			WPacket w(64); w.WriteCmd(CMD_MC_EVENT_INFO);
			w.WriteInt64(m.entityId); w.WriteInt64(m.entityType);
			w.WriteInt64(m.eventId); w.WriteString(m.eventName); return w;
		}

		inline WPacket serialize(const McSynSideInfoMessage& m) {
			WPacket w(24); w.WriteCmd(CMD_MC_SIDE_INFO);
			w.WriteInt64(m.worldId); w.WriteInt64(m.side.sideId); return w;
		}

		inline WPacket serialize(const McItemCreateMessage& m) {
			WPacket w(256); w.WriteCmd(CMD_MC_ITEMBEGINSEE);
			w.WriteInt64(m.worldId); w.WriteInt64(m.handle); w.WriteInt64(m.itemId);
			w.WriteInt64(m.posX); w.WriteInt64(m.posY); w.WriteInt64(m.angle);
			w.WriteInt64(m.num); w.WriteInt64(m.appeType); w.WriteInt64(m.fromId);
			serializeChaEventInfo(w, m.event);
			return w;
		}

		inline WPacket serialize(const McSynSkillBagMessage& m) {
			WPacket w(512); w.WriteCmd(CMD_MC_SYNSKILLBAG);
			w.WriteInt64(m.worldId);
			serializeChaSkillBagInfo(w, m.skillBag);
			return w;
		}

		inline WPacket serialize(const McMissionInfoMessage& m) {
			WPacket w(256); w.WriteCmd(CMD_MC_MISSION);
			w.WriteInt64(m.npcId); w.WriteInt64(m.listType);
			w.WriteInt64(m.prev); w.WriteInt64(m.next);
			w.WriteInt64(m.prevCmd); w.WriteInt64(m.nextCmd);
			w.WriteInt64(static_cast<int64_t>(m.items.size()));
			for (auto& item : m.items) w.WriteString(item);
			return w;
		}

		/// Сериализация потребностей квеста (условная по needType).
		inline void serializeMisNeeds(WPacket& w, const std::vector<MisNeedEntry>& needs) {
			w.WriteInt64(static_cast<int64_t>(needs.size()));
			for (const auto& n : needs) {
				w.WriteInt64(n.needType);
				if (n.needType == MIS_NEED_ITEM || n.needType == MIS_NEED_KILL) {
					w.WriteInt64(n.param1);
					w.WriteInt64(n.param2);
					w.WriteInt64(n.param3);
				} else if (n.needType == MIS_NEED_DESP) {
					w.WriteString(n.desp);
				}
			}
		}

		/// Сериализация наград квеста.
		inline void serializeMisPrizes(WPacket& w, const std::vector<MisPrizeEntry>& prizes) {
			w.WriteInt64(static_cast<int64_t>(prizes.size()));
			for (const auto& p : prizes) {
				w.WriteInt64(p.type);
				w.WriteInt64(p.param1);
				w.WriteInt64(p.param2);
			}
		}

		inline WPacket serialize(const McMisPageMessage& m) {
			WPacket w(512); w.WriteCmd(CMD_MC_MISPAGE);
			w.WriteInt64(m.cmd); w.WriteInt64(m.npcId); w.WriteString(m.name);
			serializeMisNeeds(w, m.needs);
			w.WriteInt64(m.prizeSelType);
			serializeMisPrizes(w, m.prizes);
			w.WriteString(m.description);
			return w;
		}

		inline WPacket serialize(const McMisLogMessage& m) {
			WPacket w(256); w.WriteCmd(CMD_MC_MISLOG);
			w.WriteInt64(static_cast<int64_t>(m.logs.size()));
			for (auto& l : m.logs) { w.WriteInt64(l.misId); w.WriteInt64(l.state); }
			return w;
		}

		inline WPacket serialize(const McMisLogInfoMessage& m) {
			WPacket w(512); w.WriteCmd(CMD_MC_MISLOGINFO);
			w.WriteInt64(m.misId); w.WriteString(m.name);
			serializeMisNeeds(w, m.needs);
			w.WriteInt64(m.prizeSelType);
			serializeMisPrizes(w, m.prizes);
			w.WriteString(m.description);
			return w;
		}

		inline WPacket serialize(const McMisLogClearMcMessage& m) {
			WPacket w(16); w.WriteCmd(CMD_MC_MISLOG_CLEAR);
			w.WriteInt64(m.missionId); return w;
		}

		inline WPacket serialize(const McMisLogAddMessage& m) {
			WPacket w(24); w.WriteCmd(CMD_MC_MISLOG_ADD);
			w.WriteInt64(m.missionId); w.WriteInt64(m.state); return w;
		}

		inline WPacket serialize(const McMisLogStateMessage& m) {
			WPacket w(24); w.WriteCmd(CMD_MC_MISLOG_CHANGE);
			w.WriteInt64(m.missionId); w.WriteInt64(m.state); return w;
		}

		inline WPacket serialize(const McFuncInfoMessage& m) {
			WPacket w(512); w.WriteCmd(CMD_MC_FUNCPAGE);
			w.WriteInt64(m.npcId); w.WriteInt64(m.page); w.WriteString(m.talkText);
			w.WriteInt64(static_cast<int64_t>(m.funcItems.size()));
			for (auto& f : m.funcItems) w.WriteString(f.name);
			w.WriteInt64(static_cast<int64_t>(m.missionItems.size()));
			for (auto& mi : m.missionItems) { w.WriteString(mi.name); w.WriteInt64(mi.state); }
			return w;
		}

		inline WPacket serialize(const McVolunteerListMessage& m) {
			WPacket w(256); w.WriteCmd(CMD_MC_VOLUNTER_LIST);
			w.WriteInt64(m.pageTotal); w.WriteInt64(m.page);
			w.WriteInt64(static_cast<int64_t>(m.volunteers.size()));
			for (auto& v : m.volunteers) { w.WriteString(v.name); w.WriteInt64(v.level); w.WriteInt64(v.job); w.WriteString(v.map); }
			return w;
		}

		inline WPacket serialize(const McVolunteerOpenMessage& m) {
			WPacket w(256); w.WriteCmd(CMD_MC_VOLUNTER_OPEN);
			w.WriteInt64(m.state); w.WriteInt64(m.pageTotal);
			w.WriteInt64(static_cast<int64_t>(m.volunteers.size()));
			for (auto& v : m.volunteers) { w.WriteString(v.name); w.WriteInt64(v.level); w.WriteInt64(v.job); w.WriteString(v.map); }
			return w;
		}

		inline WPacket serialize(const McShowStallSearchMessage& m) {
			WPacket w(512); w.WriteCmd(CMD_MC_STALLSEARCH);
			w.WriteInt64(static_cast<int64_t>(m.entries.size()));
			for (auto& e : m.entries) { w.WriteString(e.name); w.WriteString(e.stallName); w.WriteString(e.location); w.WriteInt64(e.count); w.WriteInt64(e.cost); }
			return w;
		}

		inline WPacket serialize(const McShowRankingMessage& m) {
			WPacket w(512); w.WriteCmd(CMD_MC_RANK);
			w.WriteInt64(static_cast<int64_t>(m.entries.size()));
			for (auto& e : m.entries) { w.WriteString(e.name); w.WriteString(e.guild); w.WriteInt64(e.level); w.WriteInt64(e.job); w.WriteInt64(e.score); }
			return w;
		}

		inline WPacket serialize(const McEspeItemMessage& m) {
			WPacket w(128); w.WriteCmd(CMD_MC_ESPE_ITEM);
			w.WriteInt64(m.worldId);
			w.WriteInt64(static_cast<int64_t>(m.items.size()));
			for (auto& i : m.items) { w.WriteInt64(i.position); w.WriteInt64(i.endure); w.WriteInt64(i.energy); w.WriteInt64(i.tradable); w.WriteInt64(i.expiration); }
			return w;
		}

		inline WPacket serialize(const McBlackMarketExchangeDataMessage& m) {
			WPacket w(128); w.WriteCmd(CMD_MC_BLACKMARKET_EXCHANGEDATA);
			w.WriteInt64(m.npcId);
			w.WriteInt64(static_cast<int64_t>(m.exchanges.size()));
			for (auto& e : m.exchanges) { w.WriteInt64(e.srcId); w.WriteInt64(e.srcCount); w.WriteInt64(e.tarId); w.WriteInt64(e.tarCount); w.WriteInt64(e.timeValue); }
			return w;
		}

		inline WPacket serialize(const McExchangeDataMessage& m) {
			WPacket w(128); w.WriteCmd(CMD_MC_EXCHANGEDATA);
			w.WriteInt64(m.npcId);
			w.WriteInt64(static_cast<int64_t>(m.exchanges.size()));
			for (auto& e : m.exchanges) { w.WriteInt64(e.srcId); w.WriteInt64(e.srcCount); w.WriteInt64(e.tarId); w.WriteInt64(e.tarCount); }
			return w;
		}

		inline WPacket serialize(const McBlackMarketExchangeUpdateMessage& m) {
			WPacket w(128); w.WriteCmd(CMD_MC_BLACKMARKET_EXCHANGEUPDATE);
			w.WriteInt64(m.npcId);
			w.WriteInt64(static_cast<int64_t>(m.exchanges.size()));
			for (auto& e : m.exchanges) { w.WriteInt64(e.srcId); w.WriteInt64(e.srcCount); w.WriteInt64(e.tarId); w.WriteInt64(e.tarCount); w.WriteInt64(e.timeValue); }
			return w;
		}

		inline WPacket serialize(const McBlackMarketExchangeAsrMessage& m) {
			WPacket w(48); w.WriteCmd(CMD_MC_BLACKMARKET_EXCHANGE_ASR);
			w.WriteInt64(m.success); w.WriteInt64(m.srcId); w.WriteInt64(m.srcCount); w.WriteInt64(m.tarId); w.WriteInt64(m.tarCount);
			return w;
		}

		// ─── MC — торговля NPC (полные данные) ─────────────────────

		inline WPacket serialize(const McTradeAllDataMessage& m) {
			WPacket w(512); w.WriteCmd(CMD_MC_TRADE_ALLDATA);
			w.WriteInt64(m.npcId); w.WriteInt64(m.tradeType); w.WriteInt64(m.param);
			w.WriteInt64(static_cast<int64_t>(m.pages.size()));
			for (auto& page : m.pages) {
				w.WriteInt64(page.itemType);
				w.WriteInt64(static_cast<int64_t>(page.items.size()));
				for (auto& item : page.items) { w.WriteInt64(item.itemId); }
				// Если TRADE_GOODS (type==1) — доп. поля count/price/level
				if (m.tradeType == 1) {
					for (auto& item : page.items) { w.WriteInt64(item.count); w.WriteInt64(item.price); w.WriteInt64(item.level); }
				}
			}
			return w;
		}

		// ─── MC — магазин (Store) ──────────────────────────────────

		inline WPacket serialize(const McStoreOpenAnswerMessage& m) {
			WPacket w(512); w.WriteCmd(CMD_MC_STORE_OPEN_ASR);
			w.WriteInt64(m.isValid ? 1 : 0);
			if (m.isValid) {
				w.WriteInt64(m.vip); w.WriteInt64(m.moBean); w.WriteInt64(m.replMoney);
				w.WriteInt64(static_cast<int64_t>(m.affiches.size()));
				for (auto& a : m.affiches) { w.WriteInt64(a.afficheId); w.WriteString(a.title); w.WriteString(a.comId); }
				w.WriteInt64(static_cast<int64_t>(m.classes.size()));
				for (auto& c : m.classes) { w.WriteInt64(c.classId); w.WriteString(c.className); w.WriteInt64(c.parentId); }
			}
			return w;
		}

		inline WPacket serialize(const McStoreListAnswerMessage& m) {
			WPacket w(1024); w.WriteCmd(CMD_MC_STORE_LIST_ASR);
			w.WriteInt64(m.pageTotal); w.WriteInt64(m.pageCurrent);
			w.WriteInt64(static_cast<int64_t>(m.products.size()));
			for (auto& p : m.products) {
				w.WriteInt64(p.comId); w.WriteString(p.comName); w.WriteInt64(p.price);
				w.WriteString(p.remark); w.WriteInt64(p.isHot ? 1 : 0);
				w.WriteInt64(p.time); w.WriteInt64(p.quantity); w.WriteInt64(p.expire);
				w.WriteInt64(static_cast<int64_t>(p.variants.size()));
				for (auto& v : p.variants) { w.WriteInt64(v.itemId); w.WriteInt64(v.itemNum); w.WriteInt64(v.flute); }
				// Всегда ровно 5 атрибутов
				for (int i = 0; i < 5; ++i) { w.WriteInt64(p.attrs[i].attrId); w.WriteInt64(p.attrs[i].attrVal); }
			}
			return w;
		}

		inline WPacket serialize(const McStoreHistoryMessage& m) {
			WPacket w(256); w.WriteCmd(CMD_MC_STORE_QUERY);
			w.WriteInt64(static_cast<int64_t>(m.records.size()));
			for (auto& r : m.records) { w.WriteString(r.time); w.WriteInt64(r.itemId); w.WriteString(r.name); w.WriteInt64(r.money); }
			return w;
		}

		inline WPacket serialize(const McStoreVipMessage& m) {
			WPacket w(48); w.WriteCmd(CMD_MC_STORE_VIP);
			w.WriteInt64(m.success);
			if (m.success != 0) { w.WriteInt64(m.vipId); w.WriteInt64(m.months); w.WriteInt64(m.shuijing); w.WriteInt64(m.modou); }
			return w;
		}

		// ─── MC — синхронизация команды (пати) ─────────────────────

		inline WPacket serialize(const McSynTeamMessage& m) {
			WPacket w(512); w.WriteCmd(CMD_MC_TEAM);
			w.WriteInt64(m.memberId); w.WriteInt64(m.hp); w.WriteInt64(m.maxHP);
			w.WriteInt64(m.sp); w.WriteInt64(m.maxSP); w.WriteInt64(m.level);
			serializeChaLookInfo(w, m.look);
			return w;
		}

		inline WPacket serialize(const McChaBeginSeeMessage& m) {
			WPacket w(4096);
			w.WriteCmd(CMD_MC_CHABEGINSEE);
			w.WriteInt64(m.seeType);
			serializeChaBaseInfo(w, m.base);
			w.WriteInt64(m.npcType);
			w.WriteInt64(m.npcState);
			w.WriteInt64(m.poseType);
			if (m.poseType == 1) {
				w.WriteInt64(m.lean.leanState); w.WriteInt64(m.lean.pose); w.WriteInt64(m.lean.angle);
				w.WriteInt64(m.lean.posX); w.WriteInt64(m.lean.posY); w.WriteInt64(m.lean.height);
			} else if (m.poseType == 2) {
				w.WriteInt64(m.seat.seatAngle); w.WriteInt64(m.seat.seatPose);
			}
			serializeChaAttrInfo(w, m.attr);
			serializeChaSkillStateInfo(w, m.skillState);
			return w;
		}

		inline WPacket serialize(const McAddItemChaMessage& m) {
			WPacket w(4096);
			w.WriteCmd(CMD_MC_ADD_ITEM_CHA);
			w.WriteInt64(m.mainChaId);
			serializeChaBaseInfo(w, m.base);
			serializeChaAttrInfo(w, m.attr);
			serializeChaKitbagInfo(w, m.kitbag);
			serializeChaSkillStateInfo(w, m.skillState);
			return w;
		}

		// ─── McCharacterAction (CMD_MC_NOTIACTION) — Serialize ───

		/// Сериализация данных действия MOVE.
		inline void serializeActionMove(WPacket& w, const ActionMoveData& d) {
			w.WriteInt64(d.moveState);
			if (d.moveState != MSTATE_ON)
				w.WriteInt64(d.stopState);
			w.WriteSequence(d.waypoints.data(), static_cast<uint16_t>(d.waypoints.size()));
		}

		/// Сериализация данных действия SKILL_SRC.
		inline void serializeActionSkillSrc(WPacket& w, const ActionSkillSrcData& d) {
			w.WriteInt64(d.fightId); w.WriteInt64(d.angle); w.WriteInt64(d.state);
			if (d.state != FSTATE_ON)
				w.WriteInt64(d.stopState);
			w.WriteInt64(d.skillId); w.WriteInt64(d.skillSpeed);
			w.WriteInt64(d.targetType);
			if (d.targetType == 1) {
				w.WriteInt64(d.targetId); w.WriteInt64(d.targetX); w.WriteInt64(d.targetY);
			} else if (d.targetType == 2) {
				w.WriteInt64(d.targetX); w.WriteInt64(d.targetY);
			}
			w.WriteInt64(d.execTime);
			// Эффекты атрибутов
			w.WriteInt64(static_cast<int64_t>(d.effects.size()));
			for (const auto& e : d.effects) { w.WriteInt64(e.attrId); w.WriteInt64(e.attrVal); }
			// Состояния скиллов
			w.WriteInt64(static_cast<int64_t>(d.states.size()));
			for (const auto& s : d.states) { w.WriteInt64(s.stateId); w.WriteInt64(s.stateLv); }
		}

		/// Сериализация данных действия SKILL_TAR.
		inline void serializeActionSkillTar(WPacket& w, const ActionSkillTarData& d) {
			w.WriteInt64(d.fightId); w.WriteInt64(d.state);
			w.WriteInt64(d.doubleAttack ? 1 : 0);
			w.WriteInt64(d.miss ? 1 : 0);
			w.WriteInt64(d.beatBack ? 1 : 0);
			if (d.beatBack) { w.WriteInt64(d.beatBackX); w.WriteInt64(d.beatBackY); }
			w.WriteInt64(d.srcId); w.WriteInt64(d.srcPosX); w.WriteInt64(d.srcPosY);
			w.WriteInt64(d.skillId); w.WriteInt64(d.skillTargetX); w.WriteInt64(d.skillTargetY);
			w.WriteInt64(d.execTime);
			// Эффекты цели: synType + count + entries
			w.WriteInt64(d.synType);
			w.WriteInt64(static_cast<int64_t>(d.effects.size()));
			for (const auto& e : d.effects) { w.WriteInt64(e.attrId); w.WriteInt64(e.attrVal); }
			// Состояния цели
			if (d.hasStates) {
				w.WriteInt64(1);
				w.WriteInt64(d.stateTime);
				w.WriteInt64(static_cast<int64_t>(d.states.size()));
				for (const auto& s : d.states) {
					w.WriteInt64(s.stateId); w.WriteInt64(s.stateLv);
					w.WriteInt64(s.duration); w.WriteInt64(s.startTime);
				}
			} else {
				w.WriteInt64(0);
			}
			// Эффекты источника
			if (d.hasSrcEffect) {
				w.WriteInt64(1);
				w.WriteInt64(d.srcState);
				w.WriteInt64(d.srcSynType);
				w.WriteInt64(static_cast<int64_t>(d.srcEffects.size()));
				for (const auto& e : d.srcEffects) { w.WriteInt64(e.attrId); w.WriteInt64(e.attrVal); }
			} else {
				w.WriteInt64(0);
			}
		}

		/// Сериализация CMD_MC_NOTIACTION.
		inline WPacket serialize(const McCharacterActionMessage& m) {
			WPacket w(4096);
			w.WriteCmd(CMD_MC_NOTIACTION);
			w.WriteInt64(m.worldId);
			w.WriteInt64(m.packetId);
			w.WriteInt64(m.actionType);
			if (m.actionType == ActionType::MOVE) {
				serializeActionMove(w, m.move);
			} else if (m.actionType == ActionType::SKILL_SRC) {
				serializeActionSkillSrc(w, m.skillSrc);
			} else if (m.actionType == ActionType::SKILL_TAR) {
				serializeActionSkillTar(w, m.skillTar);
			} else if (m.actionType == ActionType::LEAN) {
				w.WriteInt64(m.lean.leanState);
				if (m.lean.leanState == 0) {
					w.WriteInt64(m.lean.pose); w.WriteInt64(m.lean.angle);
					w.WriteInt64(m.lean.posX); w.WriteInt64(m.lean.posY); w.WriteInt64(m.lean.height);
				}
			} else if (m.actionType == ActionType::FACE || m.actionType == ActionType::SKILL_POSE) {
				w.WriteInt64(m.face.angle); w.WriteInt64(m.face.pose);
			} else if (m.actionType == ActionType::ITEM_FAILED) {
				w.WriteInt64(m.itemFailedId);
			} else if (m.actionType == ActionType::TEMP) {
				w.WriteInt64(m.tempItemId); w.WriteInt64(m.tempPartId);
			} else if (m.actionType == ActionType::CHANGE_CHA) {
				w.WriteInt64(m.changeChaMainId);
			} else if (m.actionType == ActionType::LOOK) {
				serializeChaLookInfo(w, m.look);
			} else if (m.actionType == ActionType::KITBAG || m.actionType == ActionType::BANK
					|| m.actionType == ActionType::GUILDBANK || m.actionType == ActionType::KITBAGTMP) {
				serializeChaKitbagInfo(w, m.kitbag);
			} else if (m.actionType == ActionType::SHORTCUT) {
				serializeChaShortcutInfo(w, m.shortcut);
			}
			return w;
		}

		// ─── Deserialize: Фаза 3 ──────────────────────────────────

		inline void deserialize(RPacket& pk, McSynAttributeMessage& m) {
			m.worldId = pk.ReadInt64();
			deserializeChaAttrInfo(pk, m.attr);
		}

		inline void deserialize(RPacket& pk, McSynSkillStateMessage& m) {
			m.worldId = pk.ReadInt64();
			deserializeChaSkillStateInfo(pk, m.skillState);
		}

		inline void deserialize(RPacket& pk, McAStateEndSeeMessage& m) {
			m.areaX = pk.ReadInt64(); m.areaY = pk.ReadInt64();
		}

		inline void deserialize(RPacket& pk, McAStateBeginSeeMessage& m) {
			m.areaX = pk.ReadInt64(); m.areaY = pk.ReadInt64();
			auto count = pk.ReadInt64();
			m.states.resize(static_cast<size_t>(count));
			for (auto& s : m.states) {
				s.stateId = pk.ReadInt64();
				if (s.stateId != 0) {
					s.stateLv = pk.ReadInt64();
					s.worldId = pk.ReadInt64();
					s.fightId = pk.ReadInt64();
				} else {
					s.stateLv = 0; s.worldId = 0; s.fightId = 0;
				}
			}
		}

		inline void deserialize(RPacket& pk, McDelItemChaMessage& m) { m.mainChaId = pk.ReadInt64(); m.worldId = pk.ReadInt64(); }

		inline void deserialize(RPacket& pk, McSynEventInfoMessage& m) {
			m.entityId = pk.ReadInt64(); m.entityType = pk.ReadInt64();
			m.eventId = pk.ReadInt64(); m.eventName = pk.ReadString();
		}

		inline void deserialize(RPacket& pk, McSynSideInfoMessage& m) {
			m.worldId = pk.ReadInt64(); m.side.sideId = pk.ReadInt64();
		}

		inline void deserialize(RPacket& pk, McItemCreateMessage& m) {
			m.worldId = pk.ReadInt64(); m.handle = pk.ReadInt64(); m.itemId = pk.ReadInt64();
			m.posX = pk.ReadInt64(); m.posY = pk.ReadInt64(); m.angle = pk.ReadInt64();
			m.num = pk.ReadInt64(); m.appeType = pk.ReadInt64(); m.fromId = pk.ReadInt64();
			deserializeChaEventInfo(pk, m.event);
		}

		inline void deserialize(RPacket& pk, McSynSkillBagMessage& m) {
			m.worldId = pk.ReadInt64();
			deserializeChaSkillBagInfo(pk, m.skillBag);
		}

		inline void deserialize(RPacket& pk, McMissionInfoMessage& m) {
			m.npcId = pk.ReadInt64(); m.listType = pk.ReadInt64();
			m.prev = pk.ReadInt64(); m.next = pk.ReadInt64();
			m.prevCmd = pk.ReadInt64(); m.nextCmd = pk.ReadInt64();
			auto count = pk.ReadInt64();
			m.items.resize(static_cast<size_t>(count));
			for (auto& item : m.items) item = pk.ReadString();
		}

		/// Десериализация потребностей квеста (условная по needType).
		inline void deserializeMisNeeds(RPacket& pk, std::vector<MisNeedEntry>& needs) {
			auto count = pk.ReadInt64();
			needs.resize(static_cast<size_t>(count));
			for (auto& n : needs) {
				n.needType = pk.ReadInt64();
				if (n.needType == MIS_NEED_ITEM || n.needType == MIS_NEED_KILL) {
					n.param1 = pk.ReadInt64();
					n.param2 = pk.ReadInt64();
					n.param3 = pk.ReadInt64();
				} else if (n.needType == MIS_NEED_DESP) {
					n.desp = pk.ReadString();
				}
			}
		}

		/// Десериализация наград квеста.
		inline void deserializeMisPrizes(RPacket& pk, std::vector<MisPrizeEntry>& prizes) {
			auto count = pk.ReadInt64();
			prizes.resize(static_cast<size_t>(count));
			for (auto& p : prizes) {
				p.type = pk.ReadInt64();
				p.param1 = pk.ReadInt64();
				p.param2 = pk.ReadInt64();
			}
		}

		inline void deserialize(RPacket& pk, McMisPageMessage& m) {
			m.cmd = pk.ReadInt64(); m.npcId = pk.ReadInt64(); m.name = pk.ReadString();
			deserializeMisNeeds(pk, m.needs);
			m.prizeSelType = pk.ReadInt64();
			deserializeMisPrizes(pk, m.prizes);
			m.description = pk.ReadString();
		}

		inline void deserialize(RPacket& pk, McMisLogMessage& m) {
			auto count = pk.ReadInt64();
			m.logs.resize(static_cast<size_t>(count));
			for (auto& l : m.logs) { l.misId = pk.ReadInt64(); l.state = pk.ReadInt64(); }
		}

		inline void deserialize(RPacket& pk, McMisLogInfoMessage& m) {
			m.misId = pk.ReadInt64(); m.name = pk.ReadString();
			deserializeMisNeeds(pk, m.needs);
			m.prizeSelType = pk.ReadInt64();
			deserializeMisPrizes(pk, m.prizes);
			m.description = pk.ReadString();
		}
		inline void deserialize(RPacket& pk, McMisLogClearMcMessage& m) { m.missionId = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, McMisLogAddMessage& m) { m.missionId = pk.ReadInt64(); m.state = pk.ReadInt64(); }
		inline void deserialize(RPacket& pk, McMisLogStateMessage& m) { m.missionId = pk.ReadInt64(); m.state = pk.ReadInt64(); }

		inline void deserialize(RPacket& pk, McFuncInfoMessage& m) {
			m.npcId = pk.ReadInt64(); m.page = pk.ReadInt64(); m.talkText = pk.ReadString();
			auto fCount = pk.ReadInt64();
			m.funcItems.resize(static_cast<size_t>(fCount));
			for (auto& f : m.funcItems) f.name = pk.ReadString();
			auto mCount = pk.ReadInt64();
			m.missionItems.resize(static_cast<size_t>(mCount));
			for (auto& mi : m.missionItems) { mi.name = pk.ReadString(); mi.state = pk.ReadInt64(); }
		}

		inline void deserialize(RPacket& pk, McVolunteerListMessage& m) {
			m.pageTotal = pk.ReadInt64(); m.page = pk.ReadInt64();
			auto count = pk.ReadInt64();
			m.volunteers.resize(static_cast<size_t>(count));
			for (auto& v : m.volunteers) { v.name = pk.ReadString(); v.level = pk.ReadInt64(); v.job = pk.ReadInt64(); v.map = pk.ReadString(); }
		}

		inline void deserialize(RPacket& pk, McVolunteerOpenMessage& m) {
			m.state = pk.ReadInt64(); m.pageTotal = pk.ReadInt64();
			auto count = pk.ReadInt64();
			m.volunteers.resize(static_cast<size_t>(count));
			for (auto& v : m.volunteers) { v.name = pk.ReadString(); v.level = pk.ReadInt64(); v.job = pk.ReadInt64(); v.map = pk.ReadString(); }
		}

		inline void deserialize(RPacket& pk, McShowStallSearchMessage& m) {
			auto count = pk.ReadInt64();
			m.entries.resize(static_cast<size_t>(count));
			for (auto& e : m.entries) { e.name = pk.ReadString(); e.stallName = pk.ReadString(); e.location = pk.ReadString(); e.count = pk.ReadInt64(); e.cost = pk.ReadInt64(); }
		}

		inline void deserialize(RPacket& pk, McShowRankingMessage& m) {
			auto count = pk.ReadInt64();
			m.entries.resize(static_cast<size_t>(count));
			for (auto& e : m.entries) { e.name = pk.ReadString(); e.guild = pk.ReadString(); e.level = pk.ReadInt64(); e.job = pk.ReadInt64(); e.score = pk.ReadInt64(); }
		}

		inline void deserialize(RPacket& pk, McEspeItemMessage& m) {
			m.worldId = pk.ReadInt64();
			auto count = pk.ReadInt64();
			m.items.resize(static_cast<size_t>(count));
			for (auto& i : m.items) { i.position = pk.ReadInt64(); i.endure = pk.ReadInt64(); i.energy = pk.ReadInt64(); i.tradable = pk.ReadInt64(); i.expiration = pk.ReadInt64(); }
		}

		inline void deserialize(RPacket& pk, McBlackMarketExchangeDataMessage& m) {
			m.npcId = pk.ReadInt64();
			auto count = pk.ReadInt64();
			m.exchanges.resize(static_cast<size_t>(count));
			for (auto& e : m.exchanges) { e.srcId = pk.ReadInt64(); e.srcCount = pk.ReadInt64(); e.tarId = pk.ReadInt64(); e.tarCount = pk.ReadInt64(); e.timeValue = pk.ReadInt64(); }
		}

		inline void deserialize(RPacket& pk, McExchangeDataMessage& m) {
			m.npcId = pk.ReadInt64();
			auto count = pk.ReadInt64();
			m.exchanges.resize(static_cast<size_t>(count));
			for (auto& e : m.exchanges) { e.srcId = pk.ReadInt64(); e.srcCount = pk.ReadInt64(); e.tarId = pk.ReadInt64(); e.tarCount = pk.ReadInt64(); }
		}

		inline void deserialize(RPacket& pk, McBlackMarketExchangeUpdateMessage& m) {
			m.npcId = pk.ReadInt64();
			auto count = pk.ReadInt64();
			m.exchanges.resize(static_cast<size_t>(count));
			for (auto& e : m.exchanges) { e.srcId = pk.ReadInt64(); e.srcCount = pk.ReadInt64(); e.tarId = pk.ReadInt64(); e.tarCount = pk.ReadInt64(); e.timeValue = pk.ReadInt64(); }
		}

		inline void deserialize(RPacket& pk, McBlackMarketExchangeAsrMessage& m) {
			m.success = pk.ReadInt64(); m.srcId = pk.ReadInt64(); m.srcCount = pk.ReadInt64(); m.tarId = pk.ReadInt64(); m.tarCount = pk.ReadInt64();
		}

		// ─── MC — торговля NPC (полные данные) ─────────────────────

		inline void deserialize(RPacket& pk, McTradeAllDataMessage& m) {
			m.npcId = pk.ReadInt64(); m.tradeType = pk.ReadInt64(); m.param = pk.ReadInt64();
			auto pageCount = static_cast<size_t>(pk.ReadInt64());
			m.pages.resize(pageCount);
			for (auto& page : m.pages) {
				page.itemType = pk.ReadInt64();
				auto itemCount = static_cast<size_t>(pk.ReadInt64());
				page.items.resize(itemCount);
				// Сначала читаем itemId для всех предметов
				for (auto& item : page.items) { item.itemId = pk.ReadInt64(); }
				// Если TRADE_GOODS (type==1) — доп. поля count/price/level
				if (m.tradeType == 1) {
					for (auto& item : page.items) { item.count = pk.ReadInt64(); item.price = pk.ReadInt64(); item.level = pk.ReadInt64(); }
				}
			}
		}

		// ─── MC — магазин (Store) ──────────────────────────────────

		inline void deserialize(RPacket& pk, McStoreOpenAnswerMessage& m) {
			m.isValid = pk.ReadInt64() != 0;
			if (m.isValid) {
				m.vip = pk.ReadInt64(); m.moBean = pk.ReadInt64(); m.replMoney = pk.ReadInt64();
				auto afficheCount = static_cast<size_t>(pk.ReadInt64());
				m.affiches.resize(afficheCount);
				for (auto& a : m.affiches) { a.afficheId = pk.ReadInt64(); a.title = pk.ReadString(); a.comId = pk.ReadString(); }
				auto classCount = static_cast<size_t>(pk.ReadInt64());
				m.classes.resize(classCount);
				for (auto& c : m.classes) { c.classId = pk.ReadInt64(); c.className = pk.ReadString(); c.parentId = pk.ReadInt64(); }
			}
		}

		inline void deserialize(RPacket& pk, McStoreListAnswerMessage& m) {
			m.pageTotal = pk.ReadInt64(); m.pageCurrent = pk.ReadInt64();
			auto productCount = static_cast<size_t>(pk.ReadInt64());
			m.products.resize(productCount);
			for (auto& p : m.products) {
				p.comId = pk.ReadInt64(); p.comName = pk.ReadString(); p.price = pk.ReadInt64();
				p.remark = pk.ReadString(); p.isHot = pk.ReadInt64() != 0;
				p.time = pk.ReadInt64(); p.quantity = pk.ReadInt64(); p.expire = pk.ReadInt64();
				auto variantCount = static_cast<size_t>(pk.ReadInt64());
				p.variants.resize(variantCount);
				for (auto& v : p.variants) { v.itemId = pk.ReadInt64(); v.itemNum = pk.ReadInt64(); v.flute = pk.ReadInt64(); }
				// Всегда ровно 5 атрибутов
				for (int i = 0; i < 5; ++i) { p.attrs[i].attrId = pk.ReadInt64(); p.attrs[i].attrVal = pk.ReadInt64(); }
			}
		}

		inline void deserialize(RPacket& pk, McStoreHistoryMessage& m) {
			auto recordCount = static_cast<size_t>(pk.ReadInt64());
			m.records.resize(recordCount);
			for (auto& r : m.records) { r.time = pk.ReadString(); r.itemId = pk.ReadInt64(); r.name = pk.ReadString(); r.money = pk.ReadInt64(); }
		}

		inline void deserialize(RPacket& pk, McStoreVipMessage& m) {
			m.success = pk.ReadInt64();
			if (m.success != 0) { m.vipId = pk.ReadInt64(); m.months = pk.ReadInt64(); m.shuijing = pk.ReadInt64(); m.modou = pk.ReadInt64(); }
		}

		// ─── MC — синхронизация команды (пати) ─────────────────────

		inline void deserialize(RPacket& pk, McSynTeamMessage& m) {
			m.memberId = pk.ReadInt64(); m.hp = pk.ReadInt64(); m.maxHP = pk.ReadInt64();
			m.sp = pk.ReadInt64(); m.maxSP = pk.ReadInt64(); m.level = pk.ReadInt64();
			deserializeChaLookInfo(pk, m.look);
		}

		inline void deserialize(RPacket& pk, McChaBeginSeeMessage& m) {
			m.seeType = pk.ReadInt64();
			deserializeChaBaseInfo(pk, m.base);
			m.npcType = pk.ReadInt64();
			m.npcState = pk.ReadInt64();
			m.poseType = pk.ReadInt64();
			if (m.poseType == 1) {
				m.lean.leanState = pk.ReadInt64(); m.lean.pose = pk.ReadInt64(); m.lean.angle = pk.ReadInt64();
				m.lean.posX = pk.ReadInt64(); m.lean.posY = pk.ReadInt64(); m.lean.height = pk.ReadInt64();
			} else if (m.poseType == 2) {
				m.seat.seatAngle = pk.ReadInt64(); m.seat.seatPose = pk.ReadInt64();
			}
			deserializeChaAttrInfo(pk, m.attr);
			deserializeChaSkillStateInfo(pk, m.skillState);
		}

		inline void deserialize(RPacket& pk, McAddItemChaMessage& m) {
			m.mainChaId = pk.ReadInt64();
			deserializeChaBaseInfo(pk, m.base);
			deserializeChaAttrInfo(pk, m.attr);
			deserializeChaKitbagInfo(pk, m.kitbag);
			deserializeChaSkillStateInfo(pk, m.skillState);
		}

		// ─── McCharacterAction (CMD_MC_NOTIACTION) — Deserialize ─

		/// Десериализация данных действия MOVE.
		inline void deserializeActionMove(RPacket& pk, ActionMoveData& d) {
			d.moveState = pk.ReadInt64();
			d.stopState = (d.moveState != MSTATE_ON) ? pk.ReadInt64() : 0;
			uint16_t len = 0;
			auto* ptr = pk.ReadSequence(len);
			d.waypoints.assign(reinterpret_cast<const uint8_t*>(ptr),
							   reinterpret_cast<const uint8_t*>(ptr) + len);
		}

		/// Десериализация данных действия SKILL_SRC.
		inline void deserializeActionSkillSrc(RPacket& pk, ActionSkillSrcData& d) {
			d.fightId = pk.ReadInt64(); d.angle = pk.ReadInt64(); d.state = pk.ReadInt64();
			d.stopState = (d.state != FSTATE_ON) ? pk.ReadInt64() : 0;
			d.skillId = pk.ReadInt64(); d.skillSpeed = pk.ReadInt64();
			d.targetType = pk.ReadInt64();
			if (d.targetType == 1) {
				d.targetId = pk.ReadInt64(); d.targetX = pk.ReadInt64(); d.targetY = pk.ReadInt64();
			} else if (d.targetType == 2) {
				d.targetId = 0; d.targetX = pk.ReadInt64(); d.targetY = pk.ReadInt64();
			}
			d.execTime = pk.ReadInt64();
			auto effCount = static_cast<size_t>(pk.ReadInt64());
			d.effects.resize(effCount);
			for (auto& e : d.effects) { e.attrId = pk.ReadInt64(); e.attrVal = pk.ReadInt64(); }
			auto stCount = static_cast<size_t>(pk.ReadInt64());
			d.states.resize(stCount);
			for (auto& s : d.states) { s.stateId = pk.ReadInt64(); s.stateLv = pk.ReadInt64(); }
		}

		/// Десериализация данных действия SKILL_TAR.
		inline void deserializeActionSkillTar(RPacket& pk, ActionSkillTarData& d) {
			d.fightId = pk.ReadInt64(); d.state = pk.ReadInt64();
			d.doubleAttack = pk.ReadInt64() != 0;
			d.miss = pk.ReadInt64() != 0;
			d.beatBack = pk.ReadInt64() != 0;
			if (d.beatBack) { d.beatBackX = pk.ReadInt64(); d.beatBackY = pk.ReadInt64(); }
			d.srcId = pk.ReadInt64(); d.srcPosX = pk.ReadInt64(); d.srcPosY = pk.ReadInt64();
			d.skillId = pk.ReadInt64(); d.skillTargetX = pk.ReadInt64(); d.skillTargetY = pk.ReadInt64();
			d.execTime = pk.ReadInt64();
			// Эффекты цели: synType + count + entries
			d.synType = pk.ReadInt64();
			auto effCount = static_cast<size_t>(pk.ReadInt64());
			d.effects.resize(effCount);
			for (auto& e : d.effects) { e.attrId = pk.ReadInt64(); e.attrVal = pk.ReadInt64(); }
			// Состояния цели
			d.hasStates = pk.ReadInt64() == 1;
			if (d.hasStates) {
				d.stateTime = pk.ReadInt64();
				auto stCount = static_cast<size_t>(pk.ReadInt64());
				d.states.resize(stCount);
				for (auto& s : d.states) {
					s.stateId = pk.ReadInt64(); s.stateLv = pk.ReadInt64();
					s.duration = pk.ReadInt64(); s.startTime = pk.ReadInt64();
				}
			}
			// Эффекты источника
			d.hasSrcEffect = pk.ReadInt64() != 0;
			if (d.hasSrcEffect) {
				d.srcState = pk.ReadInt64();
				d.srcSynType = pk.ReadInt64();
				auto srcCount = static_cast<size_t>(pk.ReadInt64());
				d.srcEffects.resize(srcCount);
				for (auto& e : d.srcEffects) { e.attrId = pk.ReadInt64(); e.attrVal = pk.ReadInt64(); }
			}
		}

		/// Десериализация CMD_MC_NOTIACTION — диспетчер по actionType.
		inline void deserialize(RPacket& pk, McCharacterActionMessage& m) {
			m.worldId = pk.ReadInt64();
			m.packetId = pk.ReadInt64();
			m.actionType = pk.ReadInt64();
			if (m.actionType == ActionType::MOVE) {
				deserializeActionMove(pk, m.move);
			} else if (m.actionType == ActionType::SKILL_SRC) {
				deserializeActionSkillSrc(pk, m.skillSrc);
			} else if (m.actionType == ActionType::SKILL_TAR) {
				deserializeActionSkillTar(pk, m.skillTar);
			} else if (m.actionType == ActionType::LEAN) {
				m.lean.leanState = pk.ReadInt64();
				if (m.lean.leanState == 0) {
					m.lean.pose = pk.ReadInt64(); m.lean.angle = pk.ReadInt64();
					m.lean.posX = pk.ReadInt64(); m.lean.posY = pk.ReadInt64(); m.lean.height = pk.ReadInt64();
				}
			} else if (m.actionType == ActionType::FACE || m.actionType == ActionType::SKILL_POSE) {
				m.face.angle = pk.ReadInt64(); m.face.pose = pk.ReadInt64();
			} else if (m.actionType == ActionType::ITEM_FAILED) {
				m.itemFailedId = pk.ReadInt64();
			} else if (m.actionType == ActionType::TEMP) {
				m.tempItemId = pk.ReadInt64(); m.tempPartId = pk.ReadInt64();
			} else if (m.actionType == ActionType::CHANGE_CHA) {
				m.changeChaMainId = pk.ReadInt64();
			} else if (m.actionType == ActionType::LOOK) {
				deserializeChaLookInfo(pk, m.look);
			} else if (m.actionType == ActionType::KITBAG || m.actionType == ActionType::BANK
					|| m.actionType == ActionType::GUILDBANK || m.actionType == ActionType::KITBAGTMP) {
				deserializeChaKitbagInfo(pk, m.kitbag);
			} else if (m.actionType == ActionType::SHORTCUT) {
				deserializeChaShortcutInfo(pk, m.shortcut);
			}
		}
	}
} // namespace net::msg
