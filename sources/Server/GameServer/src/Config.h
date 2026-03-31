#pragma once

#include <string>
#include <vector>
#include <cstdint>

#define MAX_GATE 5
#define MAX_MAPNAME_LENGTH	32

class CGameCommand {
public:
	CGameCommand();
	bool Load(const char* pszFileName);
	void SetDefault();

	// commands
	char m_cMove[32];
	char m_cMake[32];
	char m_cNotice[32];
	char m_cHide[32];
	char m_cUnhide[32];
	char m_cGoto[32];
	char m_cKick[32];
	char m_cMute[32];
	char m_cReload[32];
	char m_cRelive[32];
	char m_cQcha[32];
	char m_cQitem[32];
	char m_cCall[32];
	char m_cGamesvrstop[32];
	char m_cUpdateall[32];
	char m_cMisreload[32];
	char m_cSummon[32];
	char m_cSummonex[32];
	char m_cKill[32];
	char m_cAddmoney[32];
	char m_cAddexp[32];
	char m_cAttr[32];
	char m_cItemattr[32];
	char m_cSkill[32];
	char m_cDelitem[32];
	char m_cLuaall[32];
	char m_cAddkb[32];
	char m_cLua[32];
	char m_cAddImp[32];
	char m_cScrollNotice[32];
	char m_cgenCharBag[32];
	char m_cDistance[32];
};

class CGameConfig {
public:
	CGameConfig();

	bool Load(char* pszFileName);
	void SetDefault();
	bool Reload(char* pszFileName);
	bool LoadCmd(char* pszFileName);

public:
	// , , string,
	//
	char m_szGateIP[MAX_GATE][64]; // GateIP
	int m_nGatePort[MAX_GATE]; // Gate Port
	int m_nGateCnt; // Gate
	std::vector<std::string> m_mapList;
	std::vector<uint8_t> m_mapOK; // Статус загрузки каждой карты (0/1)
	char m_szEqument[MAX_MAPNAME_LENGTH]; //
	char m_szName[64]; //
	char m_szDBIP[64]; // DB IP
	char m_szDBUsr[32]; // DB
	char m_szDBPass[32]; // DB

	// Add by lark.li 20080321 begin
	char m_szTradeLogDBIP[64]; // DB IP
	char m_szTradeLogDBName[32]; // DB IP
	char m_szTradeLogDBUsr[32]; // DB
	char m_szTradeLogDBPass[32]; // DB

	BOOL m_bTradeLogIsConfig;
	// End
	char m_szDBName[32]; // kong@pkodev.net 09.22.2017

	long m_lSocketAlive; // Socket
	int m_nMaxPly; //
	int m_nMaxCha; //
	int m_nMaxItem; //
	int m_nMaxTNpc; // NPC
	unsigned long m_ulBaseID; // ID
	long m_lItemShowTime; //
	long m_lItemProtTime; //
	long m_lSayInterval; //
	char m_szResDir[255]; //
	char m_szLogDir[255]; // Log
	char m_chMapMask; //
	long m_lDBSave; //

	BOOL m_bLogAI; // AIlog
	BOOL m_bLogCha; // log
	BOOL m_bLogCal; // log
	BOOL m_bLogMission; // Missionlog

	BOOL m_bSuperCmd;

	// Add by lark.li 20080731 begin
	std::vector<int> m_vGMCmd;
	// End
	short m_sGuildNum;
	short m_sGuildTryNum;
	BOOL m_bOfflineMode;
	BOOL m_bDiscStall;
	BOOL m_bBlindChaos;
	char m_szChaosMap[32];
	DWORD m_dwStallTime;

	BOOL m_bLogDB; //
	BOOL m_bInstantIGS;

	long m_lWeather;

	std::string _assetDbPath{}; // Путь к SQLite базе игровых данных

	unsigned char m_cSaveState[32];
};

#define	defCONFIG_FILE_NAME_LEN	512
extern char szConfigFileN[defCONFIG_FILE_NAME_LEN];

extern CGameConfig g_Config;
extern CGameCommand g_Command;
