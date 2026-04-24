#include "stdafx.h"
#include "Config.h"
#include "IniFile.h"
#include "StringLib.h"

using namespace std;

char	szConfigFileN[defCONFIG_FILE_NAME_LEN] = "GameServer00.cfg";
CGameConfig g_Config;
CGameCommand g_Command;

CGameConfig::CGameConfig()
{
	SetDefault();
}

void CGameConfig::SetDefault()
{
	m_nGateCnt = 0;
	m_lSocketAlive = 1;
	m_mapList.clear();
	m_mapOK.clear();
	strcpy(m_szDBIP,  "192.168.1.233");
	strcpy(m_szDBUsr,  "usr");
	strcpy(m_szDBPass, "22222");

	memset(m_szTradeLogDBIP, 0, sizeof(m_szTradeLogDBIP));
	memset(m_szTradeLogDBName, 0, sizeof(m_szTradeLogDBName));
	memset(m_szTradeLogDBUsr, 0, sizeof(m_szTradeLogDBUsr));
	memset(m_szTradeLogDBPass, 0, sizeof(m_szTradeLogDBPass));

	memset( m_szEqument, 0, MAX_MAPNAME_LENGTH );
	m_nMaxPly = 3000;
	m_nMaxCha = 15000;
	m_nMaxItem = 10000;
	m_nMaxTNpc = 300;

	m_lItemShowTime = 300 * 1000;
	m_lItemProtTime = 30  * 1000;
	m_lSayInterval  =  3  * 1000;

	m_chMapMask = 1;
	m_lDBSave = 20 * 60 * 1000;

	strcpy(m_szResDir, "");
	strcpy(m_szLogDir, "log\\");

	m_bLogAI		= FALSE;
	m_bLogCha		= FALSE;
	m_bLogCal		= FALSE;
	m_bLogMission	= FALSE;

	m_bSuperCmd     = FALSE;

	m_vGMCmd.clear();

	m_bLogDB        = FALSE;

	m_bTradeLogIsConfig = FALSE;

	m_sGuildNum = 80;
	m_sGuildTryNum = 80;
	m_bOfflineMode = FALSE;
	m_bBlindChaos = FALSE;
	strcpy(m_szChaosMap, "NONE");
	m_bInstantIGS = FALSE;
	m_lWeather = 120;
	m_dwStallTime = 48;
	m_cSaveState[32] = {0};
	_assetDbPath = "../../databases/gamedata.sqlite";
}

bool CGameConfig::Load(char *pszFileName)
{
	ToLogService("common", "Load Game Config File [{}]", pszFileName);

	auto cfg = dbc::IniFile(pszFileName);

	auto& id = cfg["ID"];
	strncpy_s(m_szName, sizeof(m_szName), id.GetString("name", m_szName).c_str(), _TRUNCATE);
	strncpy_s(m_szEqument, sizeof(m_szEqument), id.GetString("equment").c_str(), _TRUNCATE);

	// [Gate]
	auto gateParts = SplitString(cfg["Gate"].GetString("gate"));
	if (gateParts.size() >= 2 && m_nGateCnt < MAX_GATE) {
		strncpy_s(m_szGateIP[m_nGateCnt], sizeof(m_szGateIP[0]), gateParts[0].c_str(), _TRUNCATE);
		m_nGatePort[m_nGateCnt] = std::stoi(gateParts[1]);
		m_nGateCnt++;
	}

	// [Map] — список карт через запятую: maps = garner, lonetower, teampk
	m_mapList = SplitString(cfg["Map"].GetString("maps"));
	m_mapOK.resize(m_mapList.size(), 0);

	// [Database]
	auto& db = cfg["Database"];
	strncpy_s(m_szDBName, sizeof(m_szDBName), db.GetString("db_name", m_szDBName).c_str(), _TRUNCATE);
	strncpy_s(m_szDBIP, sizeof(m_szDBIP), db.GetString("db_ip", m_szDBIP).c_str(), _TRUNCATE);
	strncpy_s(m_szDBUsr, sizeof(m_szDBUsr), db.GetString("db_usr", m_szDBUsr).c_str(), _TRUNCATE);
	strncpy_s(m_szDBPass, sizeof(m_szDBPass), db.GetString("db_pass", m_szDBPass).c_str(), _TRUNCATE);

	_assetDbPath = cfg["Assets"].GetString("StringAssetPack", "../../databases/gamedata.sqlite");

	// [Socket]
	m_lSocketAlive = static_cast<long>(cfg["Socket"].GetInt64("keep_alive", m_lSocketAlive));

	// [BaseID]
	auto baseIdStr = cfg["BaseID"].GetString("BaseID");
	if (!baseIdStr.empty()) {
		if (baseIdStr.find("0x") != std::string::npos || baseIdStr.find("0X") != std::string::npos)
			sscanf(baseIdStr.c_str(), "%x", &m_ulBaseID);
		else
			sscanf(baseIdStr.c_str(), "%d", &m_ulBaseID);
	}

	// [Entity]
	auto& entity = cfg["Entity"];
	m_nMaxPly  = static_cast<int>(entity.GetInt64("max_ply", m_nMaxPly));
	m_nMaxCha  = static_cast<int>(entity.GetInt64("max_cha", m_nMaxCha));
	m_nMaxItem = static_cast<int>(entity.GetInt64("max_item", m_nMaxItem));
	m_nMaxTNpc = static_cast<int>(entity.GetInt64("max_tnpc", m_nMaxTNpc));

	// [Guild]
	auto& guild = cfg["Guild"];
	m_sGuildNum    = static_cast<short>(guild.GetInt64("guild_num", m_sGuildNum));
	m_sGuildTryNum = static_cast<short>(guild.GetInt64("guild_try_num", m_sGuildTryNum));

	// [Item]
	auto& item = cfg["Item"];
	m_lItemShowTime = static_cast<long>(item.GetInt64("item_show_time", m_lItemShowTime / 1000)) * 1000;
	m_lItemProtTime = static_cast<long>(item.GetInt64("item_prot_time", m_lItemProtTime / 1000)) * 1000;

	// [Interval]
	auto& interval = cfg["Interval"];
	m_lSayInterval = static_cast<long>(interval.GetInt64("say_interval", m_lSayInterval / 1000)) * 1000;
	m_lWeather     = static_cast<long>(interval.GetInt64("weather_interval", m_lWeather));

	// [LOG]
	auto& log = cfg["LOG"];
	m_bLogCha     = static_cast<BOOL>(log.GetInt64("log_cha", m_bLogCha));
	m_bLogCal     = static_cast<BOOL>(log.GetInt64("log_cal", m_bLogCal));
	m_bLogAI      = static_cast<BOOL>(log.GetInt64("log_ai", m_bLogAI));
	m_bLogMission = static_cast<BOOL>(log.GetInt64("log_mission", m_bLogMission));

	// [Debug]
	auto& debug = cfg["Debug"];
	m_bSuperCmd = static_cast<BOOL>(debug.GetInt64("supercmd", m_bSuperCmd));
	auto gmcmdStr = debug.GetString("gmcmd");
	if (!gmcmdStr.empty())
		m_vGMCmd = SplitStringInt(gmcmdStr);

	// [Res]
	auto& res = cfg["Res"];
	strncpy_s(m_szResDir, sizeof(m_szResDir), res.GetString("res_dir", m_szResDir).c_str(), _TRUNCATE);
	strncpy_s(m_szLogDir, sizeof(m_szLogDir), res.GetString("log_dir", m_szLogDir).c_str(), _TRUNCATE);

	// [Large map switch]
	m_chMapMask = static_cast<char>(cfg["Large map switch"].GetInt64("db_mapmask", m_chMapMask));

	// [Corsairs]
	auto& corsairs = cfg["Corsairs"];
	m_bOfflineMode = static_cast<BOOL>(corsairs.GetInt64("stall_offline", m_bOfflineMode));
	m_bDiscStall   = static_cast<BOOL>(corsairs.GetInt64("stall_empty_dc", m_bDiscStall));
	m_dwStallTime  = static_cast<DWORD>(corsairs.GetInt64("stall_interval", m_dwStallTime));
	m_bInstantIGS  = static_cast<BOOL>(corsairs.GetInt64("igs_instant", m_bInstantIGS));
	strncpy_s(m_szChaosMap, sizeof(m_szChaosMap), corsairs.GetString("chaos_map", m_szChaosMap).c_str(), _TRUNCATE);
	m_bBlindChaos  = static_cast<BOOL>(corsairs.GetInt64("chaos_blind", m_bBlindChaos));

	auto persistStr = corsairs.GetString("persist_state");
	if (!persistStr.empty()) {
		auto states = SplitStringInt(persistStr);
		for (size_t i = 0; i < states.size() && i < 32; i++)
			m_cSaveState[i] = static_cast<unsigned char>(states[i]);
	}

	// Trade log DB
	auto& tradeLog = cfg["TradeLog"];
	strncpy_s(m_szTradeLogDBIP, sizeof(m_szTradeLogDBIP), tradeLog.GetString("tradelog_db_ip").c_str(), _TRUNCATE);
	strncpy_s(m_szTradeLogDBName, sizeof(m_szTradeLogDBName), tradeLog.GetString("tradelog_db_name").c_str(), _TRUNCATE);
	strncpy_s(m_szTradeLogDBUsr, sizeof(m_szTradeLogDBUsr), tradeLog.GetString("tradelog_db_usr").c_str(), _TRUNCATE);
	strncpy_s(m_szTradeLogDBPass, sizeof(m_szTradeLogDBPass), tradeLog.GetString("tradelog_db_pass").c_str(), _TRUNCATE);

	if (strlen(m_szTradeLogDBIP) > 0 && strlen(m_szTradeLogDBName) > 0 &&
		strlen(m_szTradeLogDBUsr) > 0 && strlen(m_szTradeLogDBPass) > 0)
		m_bTradeLogIsConfig = TRUE;

	return true;
}

bool CGameConfig::Reload(char *pszFileName)
{
	ToLogService("common", "Reload Game Config File [{}]", pszFileName);

	dbc::IniFile cfg;
	try {
		cfg = dbc::IniFile(pszFileName);
	} catch (const std::exception& e) {
		ToLogService("common", LogLevel::Error, "Config reload error: {}", e.what());
		return false;
	}

	auto& guild = cfg["Guild"];
	m_sGuildNum    = static_cast<short>(guild.GetInt64("guild_num", m_sGuildNum));
	m_sGuildTryNum = static_cast<short>(guild.GetInt64("guild_try_num", m_sGuildTryNum));

	auto& corsairs = cfg["Corsairs"];
	m_bOfflineMode = static_cast<BOOL>(corsairs.GetInt64("stall_offline", m_bOfflineMode));
	m_bInstantIGS  = static_cast<BOOL>(corsairs.GetInt64("igs_instant", m_bInstantIGS));
	m_bDiscStall   = static_cast<BOOL>(corsairs.GetInt64("stall_empty_dc", m_bDiscStall));
	m_bBlindChaos  = static_cast<BOOL>(corsairs.GetInt64("chaos_blind", m_bBlindChaos));

	return true;
}

CGameCommand::CGameCommand()
{
	SetDefault();
}

void CGameCommand::SetDefault()
{
	strcpy(m_cMove, "move");
	strcpy(m_cMake, "make");
	strcpy(m_cNotice, "notice");
	strcpy(m_cHide, "hide");
	strcpy(m_cUnhide, "unhide");
	strcpy(m_cGoto, "goto");
	strcpy(m_cKick, "kick");
	strcpy(m_cMute, "mute");
	strcpy(m_cReload, "reload");
	strcpy(m_cRelive, "relive");
	strcpy(m_cQcha, "qcha");
	strcpy(m_cQitem, "qitem");
	strcpy(m_cCall, "call");
	strcpy(m_cGamesvrstop, "gamesvrstop");
	strcpy(m_cUpdateall, "updateall");
	strcpy(m_cMisreload, "misreload");
	strcpy(m_cSummon, "summon");
	strcpy(m_cSummonex, "summonex");
	strcpy(m_cKill, "kill");
	strcpy(m_cAddmoney, "addmoney");
	strcpy(m_cAddexp, "addexp");
	strcpy(m_cAttr, "attr");
	strcpy(m_cItemattr, "itemattr");
	strcpy(m_cSkill, "skill");
	strcpy(m_cDelitem, "delitem");
	strcpy(m_cLuaall, "lua_all");
	strcpy(m_cAddkb, "addkb");
	strcpy(m_cLua, "lua");
	strcpy(m_cAddImp, "addimp");
	strcpy(m_cScrollNotice, "scroll");
	strcpy(m_cgenCharBag, "gencharbag");
	strcpy(m_cDistance, "distance");
}

bool CGameCommand::Load(const char *pszFileName)
{
	ToLogService("common", "Load Command Config [{}]", pszFileName);

	auto cfg = dbc::IniFile(pszFileName);

	// Все команды в одной секции (или без секций — попадут в дефолтную при чтении)
	// Ищем по всем секциям
	auto tryGet = [&]<size_t N>(const char* key, char (&dest)[N]) {
		for (int i = 0; i < cfg.SectCount(); i++) {
			auto val = cfg[i].GetString(key);
			if (!val.empty()) {
				strncpy_s(dest, val.c_str(), _TRUNCATE);
				return;
			}
		}
	};

	// Нет — cmd.cfg без секций, все ключи попадут в последнюю секцию.
	// IniFile требует секцию. Пусть cmd.cfg парсится старым способом — ключи без секции
	// не поддерживаются IniFile. Добавим поддержку: ключи без секции попадут в секцию "".
	// Но сейчас IniFile игнорирует ключи до первой секции (currentSection == nullptr -> exception).
	// TODO: Нужна поддержка дефолтной секции в IniFile.
	// Пока используем простой подход — читаем из любой секции:

	auto& cmds = cfg["Commands"];

	auto readCmd = [&]<size_t N>(const char* key, char (&dest)[N]) {
		auto val = cmds.GetString(key);
		if (!val.empty())
			strncpy_s(dest, val.c_str(), _TRUNCATE);
	};

	readCmd("cmd_move", m_cMove);
	readCmd("cmd_make", m_cMake);
	readCmd("cmd_notice", m_cNotice);
	readCmd("cmd_hide", m_cHide);
	readCmd("cmd_unhide", m_cUnhide);
	readCmd("cmd_goto", m_cGoto);
	readCmd("cmd_kick", m_cKick);
	readCmd("cmd_mute", m_cMute);
	readCmd("cmd_reload", m_cReload);
	readCmd("cmd_relive", m_cRelive);
	readCmd("cmd_qcha", m_cQcha);
	readCmd("cmd_qitem", m_cQitem);
	readCmd("cmd_call", m_cCall);
	readCmd("cmd_gamesvrstop", m_cGamesvrstop);
	readCmd("cmd_updateall", m_cUpdateall);
	readCmd("cmd_misreload", m_cMisreload);
	readCmd("cmd_summon", m_cSummon);
	readCmd("cmd_summonex", m_cSummonex);
	readCmd("cmd_kill", m_cKill);
	readCmd("cmd_addmoney", m_cAddmoney);
	readCmd("cmd_addexp", m_cAddexp);
	readCmd("cmd_attr", m_cAttr);
	readCmd("cmd_itemattr", m_cItemattr);
	readCmd("cmd_skill", m_cSkill);
	readCmd("cmd_delitem", m_cDelitem);
	readCmd("cmd_lua_all", m_cLuaall);
	readCmd("cmd_addkb", m_cAddkb);
	readCmd("cmd_lua", m_cLua);
	readCmd("cmd_addimp", m_cAddImp);
	readCmd("cmd_scroll", m_cScrollNotice);
	readCmd("cmd_gencharbag", m_cgenCharBag);
	readCmd("cmd_distance", m_cDistance);

	return true;
}
