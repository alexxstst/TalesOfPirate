//=============================================================================
// FileName: GameApp.cpp
// Creater: ZhangXuedong
// Date: 2004.11.04
// Comment: CGameApp class
//=============================================================================

#include "stdafx.h"
#include "GameApp.h"
#include "GameAppNet.h"
#include "SystemDialog.h"
#include "lua_gamectrl.h"
#include "GameDB.h"

#include "TradeLogDB.h" //Add by lark.li 20080324

#include "GamePool.h"
#include "Character.h"
#include "Player.h"
#include "AttachManage.h"
#include "Item.h"
#include "CharTrade.h"
#include "Config.h"
#include "JobInitEquip.h"

#include "CharacterRecord.h"
#include "SkillRecord.h"
#include "SkillStateRecord.h"
#include "ItemRecord.h"
#include "ItemRecordStore.h"
#include "LevelRecordStore.h"
#include "SailLvRecordStore.h"
#include "LifeLvRecordStore.h"
#include "ChaRecordStore.h"
#include "SkillRecordStore.h"
#include "SkillStateRecordStore.h"
#include "HairRecordStore.h"
#include "ForgeRecordStore.h"
#include "JobEquipRecordStore.h"
#include "AreaRecordStore.h"
#include "ShipRecordStore.h"
#include "ShipPartRecordStore.h"
#include "AssetDatabase.h"
#include "ItemAttr.h"
#include "CharForge.h"
#include "HairRecord.h"

#include "LuaAPI.h"
#include "WorldEudemon.h"
#include "Birthplace.h"
#include "CharBoat.h"
#include "MapEntry.h"

using namespace std;
_DBC_USING;

bool g_bLogEntity = false;

std::unordered_map<int, CItemRecordAttr> g_itemAttrMap;
CCharacter* g_pCSystemCha = NULL;
SubMap* g_pScriptMap = NULL; //
long g_lDeftMMaskLight = 21;
string g_strChaState[2];


// cha
uLong g_ulCurID = defINVALID_CHA_ID;
Long g_lCurHandle = defINVALID_CHA_HANDLE;
//


extern BOOL g_bGameEnd;
extern std::string g_strLogName;

char szChaInfoName[256] = "CharacterInfo";
char szSkillInfoName[256] = "skillinfo";
char szSkillStateInfoName[256] = "skilleff";
char szChaLvUpName[256] = "character_lvup";
char szChaSailLvUpName[256] = "saillvup";
char szChaLifeLvUpName[256] = "lifelvup";
char szItemInfoName[256] = "ItemInfo";
char szHairInfoName[64] = "Hairs"; //

void ChaException(uLong ulCurID, Long lCurHandle) {
	if (g_ulCurID == defINVALID_CHA_ID || g_lCurHandle == defINVALID_CHA_HANDLE) {
		ToLogService("errors", LogLevel::Error, "unknown (ID:{}, Handle:{})occur abnormity", ulCurID, lCurHandle);
		return;
	}

	Entity* pCEnti = g_pGameApp->IsValidEntity(ulCurID, lCurHandle);
	if (!pCEnti) {
		//LG("exception3", "ID:%u, Handle:%d\n", ulCurID, lCurHandle);
		ToLogService("errors", LogLevel::Error, "unknown entity(ID:{}, Handle:{})occur abnormity", ulCurID, lCurHandle);
		return;
	}
	CCharacter* pCurCha = pCEnti->IsCharacter();
	if (!pCurCha) {
		//LG("exception3", "ID:%u, Handle:%d\n", ulCurID, lCurHandle);
		ToLogService("errors", LogLevel::Error, "unknown character(ID:{}, Handle:{})occur abnormity", ulCurID,
					 lCurHandle);
		return;
	}

	try {
		//LG("exception3", "[%s] [%s], \n", pCurCha->GetLogName(), pCurCha->GetPlyMainCha()->GetLogName());
		ToLogService("errors", LogLevel::Error, "character[{}] [{}]occur abnormity, will be kick out game",
					 pCurCha->GetLogName(), pCurCha->GetPlyMainCha()->GetLogName());
		// ....
		CPlayer* pCPlayer = pCurCha->GetPlayer();
		if (pCPlayer) {
			//LG("exception3", "[%s] [%s], [GoOutGame]\n", pCurCha->GetLogName(), pCurCha->GetPlyMainCha()->GetLogName());
			ToLogService("errors", LogLevel::Error, "player character[{}] [{}]occur abnormity, [GoOutGame]",
						 pCurCha->GetLogName(), pCurCha->GetPlyMainCha()->GetLogName());
			KICKPLAYER(pCPlayer, 0);
			ToLogService("errors", LogLevel::Error, "End [KICKPLAYER], Begin[GoOutGame]");
			g_pGameApp->GoOutGame(pCPlayer, true);
			ToLogService("errors", LogLevel::Error, "End [GoOutGame]");
			return;
		}
		else {
			//LG("exception3", "[%s], Begin\n", pCurCha->GetName());
			ToLogService("errors", LogLevel::Error, "release bugbear character[{}], Begin", pCurCha->GetName());
			pCurCha->Free();
			//LG("exception3", "End \n");
			ToLogService("errors", LogLevel::Error, "End release bugbear character");
		}
	}
	catch (...) {
		//LG("exception3", ", , [%s]\n", pCurCha->GetName());
		ToLogService("errors", LogLevel::Error,
					 "when character loop occur abnormity, the process kick character occur abnormity again, [{}]",
					 pCurCha->GetName());
	}
}

//
DWORD WINAPI g_GameLogicProcess(LPVOID lpParameter) {
#ifdef __CATCH
	// SEHTranslator translator;
#endif
	DWORD dwLastTick;
	DWORD dwCurTick;
	DWORD dwRunTick;
	//SYSTEMTIME st;
	static char szLogTime[128] = {0};
	char szTemp[128] = {0};
	std::string strLogName = "GameServerLog";

	/*GetLocalTime( &st );
	sprintf( szLogTime, "%d-%d-%d-%d", st.wYear, st.wMonth, st.wDay, st.wHour );
	g_strLogName = strLogName + szLogTime;*/

	while (!g_bGameEnd) {
		DWORD dwInterval = 50; //

		//lua_FrameMove();
		if (g_pGameApp->m_bExecLuaCmd) {
			luaL_dofile(g_pLuaState, "tmp.txt");
			g_pGameApp->m_bExecLuaCmd = FALSE;
		}

		/*GetLocalTime( &st );
		sprintf( szTemp, "%d-%d-%d-%d", st.wYear, st.wMonth, st.wDay, st.wHour );
		if(strcmp(szLogTime, szTemp))
		{
			strcpy(szLogTime, szTemp);
			g_strLogName = strLogName + szLogTime;
		}*/

		//
		dwLastTick = GetTickCount();
		g_pGameApp->Run(dwLastTick);
		dwCurTick = GetTickCount();
		dwRunTick = dwCurTick - dwLastTick;

		//
		dwLastTick = dwCurTick;
		PEEKPACKET(50 > dwRunTick ? 50 - dwRunTick : 0);
		dwCurTick = GetTickCount();
		dwRunTick += dwCurTick - dwLastTick;

		g_pGameApp->m_dwRunStep = 104;
	}
	//LG("init", "!\n");
	ToLogService("common", "game thread finish!");
	return 0;
}

// Add by lark.li 20080324 begin
void CDBLogMgr::TradeLog(const char* action, const char* pszChaFrom, const char* pszChaTo, const char* pszTrade) {
	if (g_Config.m_bTradeLogIsConfig) {
		tradeLog_db.ExecLogSQL(g_Config.m_szName, action, pszChaFrom, pszChaTo, pszTrade);
	}
	else {
		game_db.ExecTradeLogSQL(g_Config.m_szName, action, pszChaFrom, pszChaTo, pszTrade);
	}
}

// End

// Log 5, 8000
void CDBLogMgr::Log(const char* type, const char* c1, const char* c2, const char* c3, const char* c4, const char* p,
					BOOL bAddToList) {
	return;

	if (bAddToList) {
		SDBLogData* pData = &_LogPool[_nPoolUseLoc];
		_nPoolUseLoc++;
		if (_nPoolUseLoc >= MAX_DBLOG_POOL) {
			_nPoolUseLoc = 0;
		}
		{
			auto _s = std::format("insert gamelog (action, c1, c2, c3, c4, content) "
				"values('{}', '{}', '{}', '{}', '{}', '{}')", type, c1, c2, c3, c4, p);
			std::strncpy(pData->szLog, _s.c_str(), sizeof(pData->szLog) - 1);
			pData->szLog[sizeof(pData->szLog) - 1] = 0;
		}
		_LogList.push_back(pData);
	}
	else {
		char szLog[8192];
		{
			auto _s = std::format("insert gamelog (action, c1, c2, c3, c4, content) "
				"values('{}', '{}', '{}', '{}', '{}', '{}')", type, c1, c2, c3, c4, p);
			std::strncpy(szLog, _s.c_str(), sizeof(szLog) - 1);
			szLog[sizeof(szLog) - 1] = 0;
		}
		game_db.ExecLogSQL(szLog);
	}
}


// , logDB
void CDBLogMgr::HandleLogList() {
	if (_LogList.empty()) {
		_nLogLeft = 0;
		return;
	}

	_nLogLeft = (int)(_LogList.size());


	MPTimer t;
	t.Begin();


	BOOL bFlush = FALSE;
	//
	for (int i = 0; i < _nPerLogCnt; i++) {
		if (_LogList.empty()) break;
		SDBLogData* pData = _LogList.front();
		game_db.ExecLogSQL(pData->szLog);
		_LogList.pop_front();

		// pool
		if (pData->nLoc > _nPoolUseLoc) {
			if (pData->nLoc - _nPoolUseLoc < 10) // , ,
			{
				bFlush = TRUE;
				//LG("dblog_error", "DBLogPoolHandleLoc=[%d], PoolUseLoc=[%d]DBLog, Flushlog!\n", pData->nLoc, _nPoolUseLoc);
				ToLogService("errors", LogLevel::Error,
							 "DBLogPool position will superpose HandleLoc=[{}], PoolUseLoc=[{}]deal with DBLog speed abnormity, carry out Flush all leavings log!",
							 pData->nLoc, _nPoolUseLoc);
				break;
			}
		}
	}

	if (bFlush) {
		FlushLogList();
	}

	ToLogService("common", "dblog exec sql use time = {}", t.End());
}

// GameServer, logDB
void CDBLogMgr::FlushLogList() {
	for (list<SDBLogData*>::iterator it = _LogList.begin(); it != _LogList.end(); it++) {
		SDBLogData* pData = (*it);
		game_db.ExecLogSQL(pData->szLog);
	}

	_LogList.clear();
}


CGameApp::CGameApp()
	: _dwLastTick(0),
	  _dwTempRunCnt(0),
	  m_dwRunCnt(0),
	  m_dwFPS(0),
	  m_bExecLuaCmd(0),
	  m_dwChaCnt(0),
	  m_dwPlayerCnt(0),
	  m_dwRunStep(0),
	  m_mapnum(0),
	  m_ulLeftSec(0) {
	extern CGameApp* g_pGameApp;
	g_pGameApp = this;
	for (int i = 0; i < MAX_GATE; i++)
		m_GatePlayer[i].pCPlayerL = 0;
	for (int i = 0; i <= defMAX_SKILL_NO; i++)
		for (int j = 0; j <= defMAX_SKILL_LV; j++)
			m_pCSkillTData[i][j] = 0;

	m_lCabinHeapNum = 0;
	m_lTradeDataHeapNum = 0;
	m_lSkillTDataHeapNum = 0;
	m_lMapMgrUnitHeapNum = 0;
	m_lEntityListHeapNum = 0;
	m_lMgrNodeHeapNum = 0;
	m_PicSet = NULL;
	m_fGlobalDropRate = 0;
	m_fGlobalExpRate = 0;
	ChaAttrMaxValInit(false);
}


CGameApp::~CGameApp() {
	//Log("", "haha", "", "" ,"", "");
	Log("close", "haha", "", "", "", "");
	FlushLogList();

	for (short i = 0; i < m_mapnum; i++) {
		SAFE_DELETE(m_MapList[i]);
	}

	CloseLuaScript();
	//g_CParser.Free();

	SAFE_DELETE(m_PicSet);

	m_vecVolunteerList.clear();
}


const char* GetResPath(const char* pszRes) {
	static char g_szTableName[255];
	string str = g_Config.m_szResDir;
	if (str.size() > 0) {
		str += "/";
	}
	str += pszRes;
	strcpy(g_szTableName, str.c_str());
	return g_szTableName;
}

BOOL CGameApp::Init() {
	//LG("init", "GameApp\n");
	ToLogService("common", "start initialization GameApp");

	//LG("init", "...\n");
	ToLogService("common", "initialization DB...");
	if (game_db.Init()) {
		//
		ToLogService("common", "database init...ok");
	}
	else {
		ToLogService("common", "database init...Fail!");
		return FALSE;
	}

	// Add by lark.li 20080324 begin
	if (tradeLog_db.Init()) {
		//
		ToLogService("common", "database init...ok");
	}
	else {
		ToLogService("common", "database init...Fail!");
		return FALSE;
	}
	// End

	ChaAttrMaxValInit(false);
	if (!CTextFilter::LoadFile(GetResPath("ChaNameFilter.txt"))) {
		//LG( "init", "msg!\n" );
		g_logManager.InternalLog(LogLevel::Debug, "common", RES_STRING(GM_GAMEAPP_CPP_00004));
		return FALSE;
	}

	//m_Ident.m_maxID = g_Config.m_ulBaseID; //ID
	m_Ident.m_maxID = 0xafffffff; //ID
	if (!m_Ident.m_maxID)
		//LG("init", "ID!!!\n");
		ToLogService("common", "error ID base!!!");
	m_ItemIdent.m_maxID = 1;

	// GamePool::Instance() инициализируется лениво, память выделяется по запросу через TrackedPool.
	g_pCSystemCha = GetNewCharacter();
	g_pCSystemCha->SetID(m_Ident.GetID());
	//g_pCSystemCha->SetName("");
	g_pCSystemCha->SetName(RES_STRING(GM_GAMEAPP_CPP_00018));
	g_pCSystemCha->setAttr(ATTR_CHATYPE, enumCHACTRL_NONE, 1);

	//LG("init", "Entity\n");
	ToLogService("common", "start to assign every Entity memory");
	// TrackedPool не требует Init() — std::pmr выделяет по запросу

	//LG("init", "...\n");
	ToLogService("common", "initialization every form...");
	AssetDatabase::Instance()->Open(g_Config._assetDbPath);
	auto& db = AssetDatabase::Instance()->GetDb();

	ItemRecordStore::Instance()->Load(db);
	LevelRecordStore::Instance()->Load(db);
	SailLvRecordStore::Instance()->Load(db);
	LifeLvRecordStore::Instance()->Load(db);
	ChaRecordStore::Instance()->Load(db);
	SkillRecordStore::Instance()->Load(db);
	SkillStateRecordStore::Instance()->Load(db);
	HairRecordStore::Instance()->Load(db);
	ForgeRecordStore::Instance()->Load(db);
	JobEquipRecordStore::Instance()->Load(db);
	AreaRecordStore::Instance()->Load(db);
	ShipRecordStore::Instance()->Load(db);
	ShipPartRecordStore::Instance()->Load(db);

	ItemRecordStore::Instance()->ForEach([](CItemRecord& item) {
		g_itemAttrMap[static_cast<int>(item.lID)].Init(item);
	});

	ToLogService("common", "Loading LuaJIT {}", LUAJIT_VERSION);

	InitLuaScript();

	if (InitMap() == FALSE) {
		//LG("init", "!, !\n");
		ToLogService("common", "initialization map failed!, exit!");
		return FALSE;
	}

	InitSStateTraOnTime();

	if (!IsChaAttrMaxValInit()) {
		//LG("init", "...Fail!\n");
		ToLogService("common", "character attribute max...Fail!");
		return FALSE;
	}

	//LG("init", "GameApp!\n");
	ToLogService("common", "GameApp initialization finish!");

	ToLogService("common", "sizeof(Entity) = {},  sizeof(Character) = {}", sizeof(Entity), sizeof(CCharacter));

	m_CTimerItem.Begin(3 * 1000);

	return TRUE;
}


void CGameApp::Run(DWORD dwCurTime) {
	m_dwRunStep = 0;

	DWORD dwChaCnt, dwPlayerCnt, dwActiveMgrUnit;
	dwChaCnt = m_dwChaCnt;
	dwPlayerCnt = m_dwPlayerCnt;
	dwActiveMgrUnit = m_dwActiveMgrUnit;

	static DWORD dwRunTick = GetTickCount();
	static Long lLogTime = 0;
	MPTimer t, t1;

	t1.Begin();
	m_dwRunStep = 1;

	t.Begin();
	MgrUnitRun(dwCurTime);
	DWORD dwMgrUnitRunTime = t.End();

	t.Begin();
	GameItemRun(dwCurTime);
	DWORD dwItemRunTime = t.End();

	t.Begin();
	MapMgrRun(dwCurTime);
	DWORD dwMapMgrRunTime = t.End();

	t.Begin();
	//if (dwChaCnt != m_dwChaCnt || dwPlayerCnt != m_dwPlayerCnt || dwActiveMgrUnit != m_dwActiveMgrUnit)
	//	LG("PanelData", "ChaNum=%5d,\tPlayerNum=%4d,\tActMgrUnit=%6d\n", m_dwChaCnt, m_dwPlayerCnt, m_dwActiveMgrUnit);
	//
	//DataStatistic();
	DWORD dwDataStatisticTime = t.End();

	t.Begin();
	m_dwRunStep = 100;
	g_CTimeSkillMgr.Run(dwCurTime);
	DWORD dwSkillMgrRunTime = t.End();

	t.Begin();
	m_dwRunStep = 103;
	game_db.GetMapMaskTable()->HandleSaveList();
	DWORD dwMapMaskSaveTime = t.End();

	DWORD dwGameRunTime = t1.End();

	//
	if (dwCurTime - _dwLastTick >= 1000) {
		m_dwFPS = _dwTempRunCnt;
		_dwTempRunCnt = 0;
		_dwLastTick = dwCurTime;
	}
	m_dwRunCnt++; // ,
	_dwTempRunCnt++; //
	m_dwRunStep = 104;

	if (m_ulLeftSec > 0 && m_CTimerReset.IsOK(dwCurTime)) {
		NotiGameReset(m_ulLeftSec);
		m_dwRunStep = 501;
		m_ulLeftSec--;
		if (m_ulLeftSec == 0) //
		{
			SaveAllPlayer();
			m_dwRunStep = 502;
			g_bGameEnd = true;
			return;
		}
	}
	m_dwRunStep = 503;

	//
	DWORD dwRunTimeKey = 100;
	bool bLogRunTime = false;
	if (dwCurTime - dwRunTick >= 30 * 1000)
		bLogRunTime = true;
	if (dwGameRunTime >= dwRunTimeKey) {
		if (lLogTime >= 0) {
			lLogTime++;
			if (lLogTime <= 6) bLogRunTime = true;
			else lLogTime = -1;
		}
		else {
			lLogTime--;
			if (lLogTime < -1000) lLogTime = 0;
		}
	}
	else {
		if (lLogTime > 0)
			lLogTime = 0;
	}
	if (bLogRunTime) {
		dwRunTick = dwCurTime;

		if (dwGameRunTime >= dwRunTimeKey)
			ToLogService(
				"common",
				"!!!GameRunTime = {}\t\tMgrUnitRunTime = {}\tItemRunTime = {}\tMapMgrRunTime = {}\tDataStatisticTime = {}\tSkillMgrRunTime = {}\tMapMaskSaveTime = {}",
				dwGameRunTime, dwMgrUnitRunTime, dwItemRunTime, dwMapMgrRunTime, dwDataStatisticTime, dwSkillMgrRunTime,
				dwMapMaskSaveTime);
		else
			ToLogService(
				"common",
				"...GameRunTime = {}\t\tMgrUnitRunTime = {}\tItemRunTime = {}\tMapMgrRunTime = {}\tDataStatisticTime = {}\tSkillMgrRunTime = {}\tMapMaskSaveTime = {}",
				dwGameRunTime, dwMgrUnitRunTime, dwItemRunTime, dwMapMgrRunTime, dwDataStatisticTime, dwSkillMgrRunTime,
				dwMapMaskSaveTime);
	}
	//
}

void CGameApp::MgrUnitRun(DWORD dwCurTime) {
	SubMap* pCSubMap;

	m_dwChaCnt = m_dwPlayerCnt;
	m_dwActiveMgrUnit = 0;

	static DWORD dwTick = 0;
	if (dwCurTime - dwTick >= 1 * 60 * 1000) {
		dwTick = dwCurTime;
		auto& pool = GamePool::Instance();
		ToLogService("common", "Ply[{}], Cha[{}], Item[{}], TNpc[{}]",
					 pool.GetPlayerCount(),
					 pool.GetCharacterCount(),
					 pool.GetItemCount(),
					 pool.GetTalkNpcCount());
	}

	CEyeshotCell* pCMgrUnit;
	CStateCell* pCStateCell;
	Entity *pCEnt, *pCFlagEnt;
	Short sMapCpyNum;
	DWORD dwActMgrCellNum;

	MPTimer t, t1, t2;
	DWORD dwPartRunTime[8]{};
	Char chPartCount;
	CCharacter* pCLongTimeCha;
	DWORD dwTempTick1, dwTempTick2;
	static std::vector<DWORD> dwMapRunTick(m_mapnum, 0);
	static std::vector<Long> lMapLogTime(m_mapnum, 0);
	bool bLogRun;
	DWORD dwRTimeKey = 60;

	for (short i = 0; i < m_mapnum; i++) {
		if (!m_MapList[i]->IsOpen()) continue;

		t1.Begin();
		dwActMgrCellNum = 0;
		pCLongTimeCha = 0;
		dwTempTick1 = 0;
		chPartCount = 0;

		sMapCpyNum = m_MapList[i]->GetCopyNum();
		for (short j = 0; j < sMapCpyNum; j++) {
			pCSubMap = m_MapList[i]->GetCopy(j);
			if (!pCSubMap)
				continue;
			pCSubMap->CheckRun();
			if (!pCSubMap->IsRun())
				continue;

			extern SubMap* g_pScriptMap;
			g_pScriptMap = pCSubMap; // AI,

			m_dwChaCnt += pCSubMap->GetMonsterNum();
			dwActMgrCellNum += pCSubMap->m_CEyeshotCellL.GetActiveNum();

			chPartCount = 0;

			t.Begin();
			pCSubMap->m_WeatherMgr.Run(pCSubMap);
			dwPartRunTime[chPartCount++] = t.End();

			m_dwRunStep++;

			t.Begin();
			pCSubMap->m_CEyeshotCellL.BeginGet();
			while (pCMgrUnit = pCSubMap->m_CEyeshotCellL.GetNext()) {
				pCEnt = pCMgrUnit->m_pCChaL;
				assert(!pCEnt || GamePool::Instance().IsValidEntityPtr(pCEnt));
				while (pCEnt && GamePool::Instance().IsValidEntityPtr(pCEnt)) {
					g_ulCurID = pCEnt->GetID();
					g_lCurHandle = pCEnt->GetHandle();

					t2.Begin();
					pCEnt->Run(dwCurTime);
					dwTempTick2 = t2.End();
					if (dwTempTick2 > dwTempTick1) {
						pCLongTimeCha = pCEnt->IsCharacter();
						dwTempTick1 = dwTempTick2;
					}

					pCFlagEnt = pCEnt;
					pCEnt = pCEnt->m_pCEyeshotCellNext;
					assert(!pCEnt || GamePool::Instance().IsValidEntityPtr(pCEnt));
					((CCharacter*)pCFlagEnt)->RunEnd(dwCurTime);
				}

				g_ulCurID = defINVALID_CHA_ID;
				g_lCurHandle = defINVALID_CHA_HANDLE;
			}
			dwPartRunTime[chPartCount++] = t.End();

			t.Begin();
			long lActCount = 0;
			long lTarNum = pCSubMap->m_CStateCellL.GetActiveNum();
			pCSubMap->m_CStateCellL.BeginGet();
			while (pCStateCell = pCSubMap->m_CStateCellL.GetNext()) {
				if (++lActCount > lTarNum) {
					//LG("", " %d\n", lTarNum);
					ToLogService("errors", LogLevel::Error, "fact activation number {}", lTarNum);
					break;
				}
				pCStateCell->StateRun(dwCurTime, pCSubMap);
			}
			dwPartRunTime[chPartCount++] = t.End();
		}
		m_dwActiveMgrUnit += dwActMgrCellNum;

		DWORD dwMMgrRunTime = t1.End();
		bLogRun = false;
		if (dwCurTime - dwMapRunTick[i] >= 30 * 1000)
			bLogRun = true;
		if (dwMMgrRunTime >= dwRTimeKey) {
			if (lMapLogTime[i] >= 0) {
				lMapLogTime[i]++;
				if (lMapLogTime[i] <= 6) bLogRun = true;
				else lMapLogTime[i] = -1;
			}
			else {
				lMapLogTime[i]--;
				if (lMapLogTime[i] < -1000) lMapLogTime[i] = 0;
			}
		}
		else {
			if (lMapLogTime[i] > 0)
				lMapLogTime[i] = 0;
		}
		if (bLogRun) {
			dwMapRunTick[i] = dwCurTime;

			if (chPartCount > 0) {
				if (dwMMgrRunTime >= dwRTimeKey)
					ToLogService(
						"common",
						"!!!{}({}) expend = {} ms, WeatherRun = {}, CharacterRun = {}, StateRun = {}, ActiveMgrUnitNum = {}",
						m_MapList[i]->GetName(), static_cast<int>(sMapCpyNum), dwMMgrRunTime, dwPartRunTime[0],
						dwPartRunTime[1], dwPartRunTime[2], dwActMgrCellNum);
				else
					ToLogService(
						"common",
						"...{}({})expend = {} ms, WeatherRun = {}, CharacterRun = {}, StateRun = {}, ActiveMgrUnitNum = {}",
						m_MapList[i]->GetName(), static_cast<int>(sMapCpyNum), dwMMgrRunTime, dwPartRunTime[0],
						dwPartRunTime[1], dwPartRunTime[2], dwActMgrCellNum);

				if (pCLongTimeCha) {
					ToLogService(
						"common",
						"\t\"{}\" Check = {}, ActCache = {}, Resume = {}, Player = {}, AI = {}, Area = {}, Die = {}, Mission = {}, Team = {}, SkillState = {}, Move = {}, Fight = {}, DB = {}, Ping = {}",
						pCLongTimeCha->GetLogName(),
						pCLongTimeCha->m_dwCellRunTime[0], pCLongTimeCha->m_dwCellRunTime[1],
						pCLongTimeCha->m_dwCellRunTime[2], pCLongTimeCha->m_dwCellRunTime[3],
						pCLongTimeCha->m_dwCellRunTime[4], pCLongTimeCha->m_dwCellRunTime[5],
						pCLongTimeCha->m_dwCellRunTime[6], pCLongTimeCha->m_dwCellRunTime[7],
						pCLongTimeCha->m_dwCellRunTime[8], pCLongTimeCha->m_dwCellRunTime[9],
						pCLongTimeCha->m_dwCellRunTime[10], pCLongTimeCha->m_dwCellRunTime[11],
						pCLongTimeCha->m_dwCellRunTime[12], pCLongTimeCha->m_dwCellRunTime[13]);
				}
			}
		}
	}
}

void CGameApp::GameItemRun(DWORD dwCurTime) {
	if (!m_CTimerItem.IsOK(dwCurTime))
		return;

	GamePool::Instance().ForEachItem([dwCurTime](CItem* pCCur) {
		pCCur->Run(dwCurTime);
	});
}

void CGameApp::MapMgrRun(DWORD dwCurTime) {
	CMapRes* pCMap;

	for (short i = 0; i < m_mapnum; i++) {
		pCMap = m_MapList[i];
		if (!pCMap->IsValid()) continue;

		pCMap->Run(dwCurTime);
	}
}

void CGameApp::SetEntityEnableLog(bool bValid) {
	SubMap* pCSubMap;
	//CEyeshotCell	*pUnit;
	Entity* pCEnt;

	if (g_bLogEntity == bValid)
		return;

	g_bLogEntity = bValid;
	short sMapCpyNum;
	for (short i = 0; i < m_mapnum; i++) {
		if (!m_MapList[i]->IsValid()) continue;
		sMapCpyNum = m_MapList[i]->GetCopyNum();
		for (short j = 0; j < sMapCpyNum; j++) {
			pCSubMap = m_MapList[i]->GetCopy(j);
			if (!pCSubMap)
				continue;

			for (short m = 0; m < pCSubMap->GetEyeshotCellLin(); m++) {
				for (short n = 0; n < pCSubMap->GetEyeshotCellCol(); n++) {
					pCEnt = pCSubMap->m_pCEyeshotCell[m][n].m_pCChaL;
					while (pCEnt) {
						pCEnt = pCEnt->m_pCEyeshotCellNext;
					}
					pCEnt = pCSubMap->m_pCEyeshotCell[m][n].m_pCItemL;
					while (pCEnt) {
						pCEnt = pCEnt->m_pCEyeshotCellNext;
					}
				}
			}
		}
	}
}

//
CPlayer* CGameApp::GetNewPlayer() {
	return GamePool::Instance().AcquirePlayer();
}

//
CPlayer* CGameApp::GetPlayer(long lHandle) {
	return GamePool::Instance().FindPlayer(lHandle);
}

CPlayer* CGameApp::IsValidPlayer(long lID, long lHandle) {
	CPlayer* pCPly = GamePool::Instance().FindPlayer(lHandle);
	if (!pCPly)
		return 0;
	if (pCPly->GetID() != lID)
		return 0;

	return pCPly;
}

//
// chType01
CPlayer* CGameApp::CreateGamePlayer(const char szPassword[], uLong ulChaDBId, uLong ulWorldId, const char* cszMapName,
									char chType) {
	CPlayer* pCOldPly = GetPlayerByDBID(ulChaDBId);
	if (pCOldPly) {
		//LG("", " %s[%u] \n", pCOldPly->GetMainCha()->GetName(), ulChaDBId);
		ToLogService("common", "when character {}[{}] enterfind it has exist", pCOldPly->GetMainCha()->GetName(),
					 ulChaDBId);
		pCOldPly->GetCtrlCha()->BreakAction();
		pCOldPly->MisLogout();
		pCOldPly->MisGooutMap();

		ReleaseGamePlayer(pCOldPly);
		return NULL;
	}

	CPlayer* l_player = GetNewPlayer();
	CCharacter* pCMainCha = GetNewCharacter();
	if (!l_player || !pCMainCha) {
		if (l_player)
			l_player->Free();

		if (pCMainCha)
			pCMainCha->Free();

		// characterfree
		// LG("enter_map", "ID = %u \n", ulChaDBId);
		ToLogService("map", "when create new player characterID = {}memory assign failed ", ulChaDBId);
		return NULL;
	}

	l_player->SetPassword(szPassword);
	l_player->SetID(ulWorldId);
	pCMainCha->SetPlayer(l_player);
	pCMainCha->SetID(ulWorldId);
	pCMainCha->_dwStallTick = 0;

	l_player->SetMisChar(*pCMainCha);
	l_player->SetMainCha(pCMainCha);
	l_player->SetCtrlCha(pCMainCha);

	l_player->SetDBChaId(ulChaDBId);
	l_player->SetMaskMapName(cszMapName);

	ToLogService("map", "atorID = {}, begin read data from database: ", ulChaDBId);
	if (!game_db.ReadPlayer(*l_player, ulChaDBId)) // Gate
	{
		// LG("enter_map", "atorID = %d, GameServerGateServer\n", ulChaDBId);
		ToLogService(
			"map", "atorID = {},when get character dataappear errorlead to GameServer with GateServer disconnection",
			ulChaDBId);
		l_player->Free();
		return NULL;
	}

	//
	if (!g_CharBoat.LoadBoat(*l_player->GetMainCha(), chType)) {
		ToLogService("common", "Character {}[{}] Load Boat Failt.", l_player->GetMainCha()->GetName(), ulChaDBId);
		l_player->Free();
		return NULL;
	}

	AddPlayerIdx(ulChaDBId, l_player);
	g_pGameApp->m_dwPlayerCnt++;

	//LG("enter_map", "cha type = %d\n", pCMainCha->m_SChaPart.sTypeID);
	//for(int i = 0; i < enumEQUIP_NUM; i++)
	//LG("enter_map", "olhe [%d] = %d\n", i, pCMainCha->m_SChaPart.SLink[i].sID);

	return l_player;

	ToLogService("map", "atorID = {} ({}) end entermap", ulChaDBId, pCMainCha->GetName());
	ToLogService("map", "atorID = {}, logname = [{}] end enter", ulChaDBId, pCMainCha->GetLogName());
}

//
void CGameApp::ReleaseGamePlayer(CPlayer* pPlayer) {
	assert(GamePool::Instance().IsValidPlayerPtr(pPlayer));

	if (pPlayer && pPlayer->IsValid()) {
		CCharacter* pCCha = pPlayer->GetCtrlCha();
		if (!pCCha)
			return;
		bool bIsDie = !pCCha->IsLiveing();
		bool bSavePos = false;
		SubMap* pSrcMap = pCCha->GetSubMap();

		if (pSrcMap)
			pSrcMap->BeforePlyOutMap(pCCha);
		if (pSrcMap)
			bSavePos = pCCha->GetSubMap()->CanSavePos();

		// Снимаем CtrlCha и (если отличается) MainCha с их eyeshot-клеток ДО любых
		// манипуляций с координатами/submap (ResetBirthInfo, SetToMainCha и т.д.).
		// Иначе m_pCEyeshotHost и позиция/карта рассинхронизируются, и финальный
		// Free()→Character::Finally→SubMap::Delete уйдёт в early-return, оставив в
		// m_pCChaL клетки dangling-указатель на возвращённую в GamePool сущность —
		// именно это ловит assert в MgrUnitRun.
		CCharacter* pMainCha = pPlayer->GetMainCha();
		if (pSrcMap) {
			pSrcMap->GoOut(pCCha);
		}
		if (pMainCha && pMainCha != pCCha) {
			if (SubMap* pMainMap = pMainCha->GetSubMap()) {
				pMainMap->GoOut(pMainCha);
			}
		}

		if (bIsDie || !bSavePos) //
		{
			if (bIsDie)
				g_luaAPI.Call("Relive", pCCha);

			if (pCCha->IsBoat()) //
			{
				pCCha->SetToMainCha(bIsDie);

				pPlayer->GetMainCha()->ResetBirthInfo();
			}
			else {
				pCCha->ResetBirthInfo();
			}
		}

		ToLogService("map", "atorID = {}, begin goout", pPlayer->GetDBChaId());

		// Персонаж УЖЕ снят с карты (GoOut выше), поэтому внутри SaveAllData
		// pCCtrlCha->GetSubMap() == nullptr и по-умолчанию позиция не пишется.
		// Форсируем сохранение позиции если карта её позволяла до снятия
		// (bSavePos, вычислено до GoOut) и персонаж жив. Иначе — позицию
		// перепишет отдельный SavePlayerPos ниже (birth-point после ResetBirthInfo).
		const bool bForceWithPos = !bIsDie && bSavePos;
		if (game_db.SavePlayer(*pPlayer, enumSAVE_TYPE_OFFLINE, bForceWithPos) == false) {
			//LG("enter_map", "SavePlayer[%s]", pPlayer->GetMainCha()->GetName());
			ToLogService("map", "SavePlayer[{}] failed", pPlayer->GetMainCha()->GetName());
		}

		if (bIsDie || !bSavePos) //
		{
			game_db.SavePlayerPos(*pPlayer);
			// SetSubMap/SetPos восстановление больше не нужно: персонажи уже сняты
			// с карты выше, Free()→Character::Finally увидит m_submap == nullptr
			// и корректно пропустит повторный GoOut.
		}

		DelPlayerIdx(pPlayer->GetDBChaId());
		g_pGameApp->m_dwPlayerCnt--;

		// gate server — на валидном объекте (до Free).
		pPlayer->OnLogoff();
		DELPLAYER(pPlayer);
		ToLogService("map", "atorID = {}, goout", pPlayer->GetDBChaId());

		// Free в конце: Finally() освобождает MainCha/лодки и возвращает CPlayer
		// в GamePool. Character::Finally увидит m_submap == nullptr (мы уже сняли
		// с карты) и не будет пытаться повторно снять — ни ошибок, ни утечек.
		pPlayer->Free();
	}
}

void CGameApp::GoOutGame(CPlayer* pPlayer, bool bOffLine, bool mOffLine) {
	if (pPlayer && pPlayer->IsValid()) {
		//here there is a bug and this the fix , gotta test the bug first before apply the fix //
		if (g_Config.m_bOfflineMode && mOffLine) {
			pPlayer->SetCanReceiveRequests(false);
			return;
		}
		if (pPlayer->GetMainCha()->IsVolunteer()) {
			pPlayer->GetMainCha()->Cmd_DelVolunteer();
		}
		pPlayer->GetCtrlCha()->BreakAction();
		if (bOffLine) {
			pPlayer->MisLogout();
		}
		pPlayer->MisGooutMap();
		ReleaseGamePlayer(pPlayer);
	}
	else {
		ToLogService("errors", LogLevel::Error, "GoOutGame");
	}
}

void CGameApp::NoticePlayerLogin(CPlayer* pCPlayer) {
	if (!pCPlayer || !pCPlayer->GetCtrlCha())
		return;

	//  :     +  trailer
	auto WtPk = net::msg::serialize(net::msg::MmLoginMessage{pCPlayer->GetCtrlCha()->GetName()});

	BEGINGETGATE();
	GateServer* pGateServer;
	while (pGateServer = GETNEXTGATE()) {
		WtPk.WriteInt64(0);
		WtPk.WriteInt64(0);
		WtPk.WriteInt64(1);
		pGateServer->SendData(WtPk);
		break;
	}
}

void CGameApp::AfterPlayerLogin(const char* cszPlyName) {
	if (!cszPlyName)
		return;

	g_CDMapEntry.AfterPlayerLogin(cszPlyName);
}

//
CCharacter* CGameApp::GetNewCharacter() {
	return GamePool::Instance().AcquireCharacter();
}

//
CItem* CGameApp::GetNewItem() {
	return GamePool::Instance().AcquireItem();
}

// NPC
mission::CTalkNpc* CGameApp::GetNewTNpc() {
	return GamePool::Instance().AcquireTalkNpc();
}

Entity* CGameApp::GetEntity(long lHandle) {
	return GamePool::Instance().FindEntity(lHandle);
}

//
Entity* CGameApp::IsValidEntity(unsigned long ulID, long lHandle) {
	Entity* pEnti = g_pGameApp->GetEntity(lHandle);
	if (!pEnti)
		return 0;
	if (!pEnti->IsValid()
		|| ulID != pEnti->GetID())
		return 0;

	return pEnti;
}

//
Entity* CGameApp::IsLiveingEntity(unsigned long ulID, long lHandle) {
	Entity* pEnti = IsValidEntity(ulID, lHandle);
	if (!pEnti)
		return 0;
	if (pEnti->IsCharacter() != g_pCSystemCha)
		if (!pEnti->IsLiveing() || !pEnti->GetSubMap())
			return 0;

	return pEnti;
}

//
Entity* CGameApp::IsMapEntity(unsigned long ulID, long lHandle) {
	Entity* pEnti = IsValidEntity(ulID, lHandle);
	if (!pEnti)
		return 0;
	if (pEnti->IsCharacter() != g_pCSystemCha)
		if (!pEnti->GetSubMap())
			return 0;

	return pEnti;
}

//
Entity* CGameApp::IsLifeEntity(unsigned long ulID, long lHandle) {
	Entity* pEnti = IsValidEntity(ulID, lHandle);
	if (!pEnti)
		return 0;
	if (pEnti->IsCharacter() != g_pCSystemCha)
		if (!pEnti->IsLiveing())
			return 0;

	return pEnti;
}

//
BOOL CGameApp::InitMap() {
	//LG("init", "...\n");
	ToLogService("common", "initialization map list...");

	mission::g_WorldEudemon.Load("Eudemon", g_Config.m_szEqument, -1);

	//Map
	m_mapnum = static_cast<short>(g_Config.m_mapList.size());
	if (m_mapnum < 1) {
		ToLogService("common", "collocate map number 0,no map was initialization, exit!");
		return FALSE;
	}

	m_strMapNameList = "";

	m_MapList.assign(m_mapnum, nullptr);
	for (short i = 0; i < m_mapnum; i++) {
		m_MapList[i] = new CMapRes();
		m_MapList[i]->SetName(g_Config.m_mapList[i].c_str());

		strcpy(m_MapList[i]->m_szObstacleFile, m_MapList[i]->GetName()); //
		strcat(m_MapList[i]->m_szObstacleFile, "\\");
		strcat(m_MapList[i]->m_szObstacleFile, m_MapList[i]->GetName());
		strcat(m_MapList[i]->m_szObstacleFile, ".blk");
		strcpy(m_MapList[i]->m_szSectionFile, m_MapList[i]->GetName()); //
		strcat(m_MapList[i]->m_szSectionFile, "\\");
		strcat(m_MapList[i]->m_szSectionFile, m_MapList[i]->GetName());
		strcpy(m_MapList[i]->m_szMonsterSpawnFile, m_MapList[i]->GetName()); //
		strcat(m_MapList[i]->m_szMonsterSpawnFile, "\\");
		strcat(m_MapList[i]->m_szMonsterSpawnFile, m_MapList[i]->GetName());
		strcat(m_MapList[i]->m_szMonsterSpawnFile, "ChaSpn");
		strcpy(m_MapList[i]->m_szNpcSpawnFile, m_MapList[i]->GetName()); //npc
		strcat(m_MapList[i]->m_szNpcSpawnFile, "\\");
		strcat(m_MapList[i]->m_szNpcSpawnFile, m_MapList[i]->GetName());
		strcat(m_MapList[i]->m_szNpcSpawnFile, "NPC");
		strcpy(m_MapList[i]->m_szMapSwitchFile, m_MapList[i]->GetName()); //
		strcat(m_MapList[i]->m_szMapSwitchFile, "\\");
		strcat(m_MapList[i]->m_szMapSwitchFile, m_MapList[i]->GetName());
		strcat(m_MapList[i]->m_szMapSwitchFile, "SwhMap");
		strcpy(m_MapList[i]->m_szMonsterCofFile, m_MapList[i]->GetName()); //
		strcat(m_MapList[i]->m_szMonsterCofFile, "\\");
		strcat(m_MapList[i]->m_szMonsterCofFile, m_MapList[i]->GetName());
		strcat(m_MapList[i]->m_szMonsterCofFile, "monster_conf.lua");
		strcpy(m_MapList[i]->m_szCtrlFile, m_MapList[i]->GetName()); //
		strcat(m_MapList[i]->m_szCtrlFile, "\\");
		strcat(m_MapList[i]->m_szCtrlFile, "ctrl.lua");
		strcpy(m_MapList[i]->m_szEntryFile, m_MapList[i]->GetName()); //
		strcat(m_MapList[i]->m_szEntryFile, "\\");
		strcat(m_MapList[i]->m_szEntryFile, "entry.lua");

		if (m_MapList[i]->Init() == FALSE) {
			//LG("init", "[%s]!\n", m_MapList[i]->GetName());
			ToLogService("common", "map [{}] initialization failed!", m_MapList[i]->GetName());
			g_Config.m_mapOK[i] = 0;
			continue;
		}
		else {
			g_Config.m_mapOK[i] = 1;
		}
		m_strMapNameList += m_MapList[i]->GetName();
		m_strMapNameList += ";";
		//LG("init", "[%s] ok!\n", m_MapList[i]->GetName());
		ToLogService("common", "map [{}] ok!", m_MapList[i]->GetName());
	}


	luaL_dofile(g_pLuaState, GetResPath("script/help/AddHelpNPC.lua"));

	luaL_dofile(g_pLuaState, GetResPath("script/monster/mlist.lua"));

	return TRUE;
}

BOOL CGameApp::SummonNpc(BYTE byMapID, USHORT sAreaID, const char szNpc[], USHORT sTime) {
	CMapRes* pMap = g_MapID.GetMap(byMapID);
	if (pMap) {
		return pMap->SummonNpc(sAreaID, szNpc, sTime);
	}

	return FALSE;
}

//
CMapRes* CGameApp::FindMapByName(const char* mapname, bool bIncUnRun) {
	if (!mapname)
		return 0;

	short i = 0;
	for (; i < m_mapnum; i++) {
		CMapRes* pMap = m_MapList[i];
		if (!pMap) continue; //
		//BUG
		/* if (!bIncUnRun)
			if (!(pMap->IsOpen())) continue;
		*/
		if (!strcmp(pMap->GetName(), mapname)) {
			break;
		}
	}
	if (i < m_mapnum) {
		return m_MapList[i];
	}
	else {
		return 0;
	}
}

void CGameApp::LoadAllTable() {
	LoadCharacterInfo();
	LoadSkillInfo();
	LoadItemInfo();
}

void CGameApp::LoadCharacterInfo() {
	ChaRecordStore::Instance()->Load(AssetDatabase::Instance()->GetDb());
}

void CGameApp::LoadSkillInfo() {
	SkillRecordStore::Instance()->Load(AssetDatabase::Instance()->GetDb());
}

void CGameApp::LoadItemInfo() {
	ItemRecordStore::Instance()->Load(AssetDatabase::Instance()->GetDb());
}

BOOL CGameApp::ReloadNpcInfo(CCharacter& character) {
	GamePool::Instance().ForEachTalkNpc([&character](mission::CTalkNpc* pTalkNpc) {
		if (!pTalkNpc->InitScript(pTalkNpc->GetInitFunc(), pTalkNpc->GetName())) {
			character.SystemNotice(RES_STRING(GM_GAMEAPP_CPP_00001), pTalkNpc->GetInitFunc(), pTalkNpc->GetName());
		}
	});

	//mission::g_WorldEudemon.Load( "Eudemon", "", -1 );
	mission::g_WorldEudemon.Load("Eudemon", "Eudemon", -1);
	return TRUE;
}

mission::CNpc* CGameApp::FindNpc(const char szName[]) {
	if (szName) {
		for (short i = 0; i < m_mapnum; i++) {
			mission::CNpc* pNpc = m_MapList[i]->FindNpc(szName);
			if (pNpc) return pNpc;
		}
	}
	return NULL;
}

void CGameApp::NotiGameReset(unsigned long ulLeftSec) {
	char szNotiMsg[1024];

	std::snprintf(szNotiMsg, sizeof(szNotiMsg), RES_STRING(GM_GAMEAPP_CPP_00002), g_Config.m_szName, ulLeftSec, m_strMapNameList.c_str());

	//  :
	auto wpk = net::msg::serialize(net::msg::McSysInfoMessage{szNotiMsg});
	NotiPkToWorld(wpk);
}

void CGameApp::SaveAllPlayer(void) {
	BEGINGETGATE();
	GateServer* pGateServer;
	ToLogService("map", "Begin SaveAllPlayer==============================================================");
	while (pGateServer = GETNEXTGATE()) {
		for (CPlayer* pCPlayer : pGateServer->m_playerlist) {
			ToLogService("map", "SaveAllPlayer");
			game_db.SavePlayer(*pCPlayer, enumSAVE_TYPE_OFFLINE);
			ToLogService("map", "");
		}
	}
	game_db.GetMapMaskTable()->SaveAll(); // ,
	ToLogService("map", "End SaveAllPlayer################################################################");
}

void CGameApp::DataStatistic(void) {
	Long lCabinHeapNum = static_cast<Long>(m_CabinPool.GetUsedCount());
	Long lTradeDataHeapNum = static_cast<Long>(m_TradeDataPool.GetUsedCount());
	Long lSkillTDataHeapNum = static_cast<Long>(m_SkillTDataPool.GetUsedCount());
	Long lMapMgrUnitHeapNum = static_cast<Long>(m_MapStateCellPool.GetUsedCount());
	Long lEntityListHeapNum = static_cast<Long>(m_ChaListPool.GetUsedCount());
	Long lMgrNodeHeapNum = static_cast<Long>(m_StateCellNodePool.GetUsedCount());

	if (lCabinHeapNum > m_lCabinHeapNum || lTradeDataHeapNum > m_lTradeDataHeapNum || lSkillTDataHeapNum >
		m_lSkillTDataHeapNum
		|| lMapMgrUnitHeapNum > m_lMapMgrUnitHeapNum || lEntityListHeapNum > m_lEntityListHeapNum || lMgrNodeHeapNum >
		m_lMgrNodeHeapNum) {
		if (m_lCabinHeapNum < lCabinHeapNum)
			m_lCabinHeapNum = lCabinHeapNum;
		if (m_lTradeDataHeapNum < lTradeDataHeapNum)
			m_lTradeDataHeapNum = lTradeDataHeapNum;
		if (m_lSkillTDataHeapNum < lSkillTDataHeapNum)
			m_lSkillTDataHeapNum = lSkillTDataHeapNum;
		if (m_lMapMgrUnitHeapNum < lMapMgrUnitHeapNum)
			m_lMapMgrUnitHeapNum = lMapMgrUnitHeapNum;
		if (m_lEntityListHeapNum < lEntityListHeapNum)
			m_lEntityListHeapNum = lEntityListHeapNum;
		if (m_lMgrNodeHeapNum < lMgrNodeHeapNum)
			m_lMgrNodeHeapNum = lMgrNodeHeapNum;

		ToLogService("common", "Cabin=%4u,\tTrade=%8u,\tSkillTemp=%8u,\tMapMgrUnit=%8u,\tEntityList=%8u,\tMgrNode=%8u",
					 m_lCabinHeapNum, m_lTradeDataHeapNum, m_lSkillTDataHeapNum, m_lMapMgrUnitHeapNum,
					 m_lEntityListHeapNum, m_lMgrNodeHeapNum);
	}
}

// GameServer
void CGameApp::WorldNotice(const char* szString) {
	if (!szString)
		return;

	ToLogService("common", "WorldNotice: len = {}", strlen(szString));
	ToLogService("common", "WorldNotice: contend = {}", szString);

	//  :   +  trailer
	auto WtPk = net::msg::serialize(net::msg::MmNoticeMessage{szString});

	BEGINGETGATE();
	GateServer* pGateServer;
	while (pGateServer = GETNEXTGATE()) {
		WtPk.WriteInt64(0);
		WtPk.WriteInt64(0);
		WtPk.WriteInt64(1);
		pGateServer->SendData(WtPk);
		break;
	}
}

void CGameApp::GuildNotice(unsigned long guildID, const char* szString) {
	if (!szString)
		return;

	//  :   (GameServerGroup)
	auto WtPk = net::msg::serialize(net::msg::GmGuildNoticeMessage{(int64_t)guildID, szString});
	SENDTOGROUP(WtPk);
}

//add by sunny.sun20080804
void CGameApp::ScrollNotice(const char* szString, int SetNum, DWORD color) {
	if (!szString)
		return;
	//  :   (GameServerGroup)
	auto WtPk = net::msg::serialize(net::msg::GmScrollNoticeMessage{szString, (int64_t)SetNum, (int64_t)color});
	SENDTOGROUP(WtPk);
}

//add by sunny.sun20080821
void CGameApp::GMNotice(const char* szString) {
	if (!szString)
		return;
	//  : GM- (GameServerGroup)
	auto WtPk = net::msg::serialize(net::msg::GmGMNoticeMessage{szString});
	SENDTOGROUP(WtPk);
}

// GameServer
void CGameApp::LocalNotice(const char* szString) {
	if (!szString)
		return;

	//  :
	auto wpk = net::msg::serialize(net::msg::McSysInfoMessage{szString});
	SENDTOWORLD(wpk);
}

// szChaName == ""GameServer
void CGameApp::ChaNotice(const char* szNotiString, const char* szChaName) {
	if (!szNotiString || !szChaName)
		return;

	//  :   (GameServerGateServer)
	auto WtPk = net::msg::serialize(net::msg::GmChaNoticeMessage{0, szNotiString, szChaName});

	BEGINGETGATE();
	GateServer* pGateServer = NULL;
	while (pGateServer = GETNEXTGATE()) {
		WtPk.WriteInt64(0);
		WtPk.WriteInt64(0);
		WtPk.WriteInt64(1);
		pGateServer->SendData(WtPk);
		break;
	}
}

void CGameApp::BanAccount(const char* szString) {
	if (!szString)
		return;
	//  :   (GameServerGroup)
	auto WtPk = net::msg::serialize(net::msg::GmBanAccountMessage{szString});
	SENDTOGROUP(WtPk);
}

void CGameApp::UnbanAccount(const char* szString) {
	if (!szString)
		return;
	//  :   (GameServerGroup)
	auto WtPk = net::msg::serialize(net::msg::GmUnbanAccountMessage{szString});
	SENDTOGROUP(WtPk);
}

void CGameApp::CanReceiveRequests(uLong chaID, bool CanSend) {
	//  :    (GameServerGroup)
	auto WtPk = net::msg::serialize(net::msg::GmCanReceiveRequestsMessage{(int64_t)chaID, (int64_t)CanSend});
	SENDTOGROUP(WtPk);
}

// ============================================================================
// Ранее inline-методы из GameApp.h, вынесены в .cpp 2026-04-22.
// ============================================================================

CDBLogMgr::CDBLogMgr()
	: _nPerLogCnt(5), _nLogLeft(0), _nPoolUseLoc(0)
{
	for (int i = 0; i < MAX_DBLOG_POOL; i++) {
		_LogPool[i].nLoc = i;
	}
}

int CDBLogMgr::GetLogLeft()              { return _nLogLeft; }
int CDBLogMgr::SetPerLogCnt(int nCnt)    { _nPerLogCnt = nCnt; return _nPerLogCnt; }
int CDBLogMgr::GetPerLogCnt()            { return _nPerLogCnt; }

void  CGameApp::SetGlobalRates(float droprate, float exprate) {
	m_fGlobalDropRate = droprate;
	m_fGlobalExpRate  = exprate;
}
float CGameApp::GetGlobalDropRate()      { return m_fGlobalDropRate; }
float CGameApp::GetGlobalExpRate()       { return m_fGlobalExpRate; }

mission::CEventEntity* CGameApp::CreateEntity(BYTE byType) {
	return GamePool::Instance().AcquireEventEntity(byType);
}

bool  CGameApp::IsChaAttrMaxValInit(void)          { return m_bChaAttrMaxValInit; }
void  CGameApp::ChaAttrMaxValInit(bool bSet)       { m_bChaAttrMaxValInit = bSet; }

short CGameApp::GetMapNum()                        { return m_mapnum; }

CMapRes* CGameApp::GetMap(int no) {
	return m_MapList[no];
}

void CGameApp::AddPlayerIdx(DWORD dwDBID, CPlayer* pPlayer) {
	_PlayerIdx[dwDBID] = pPlayer;
}

void CGameApp::DelPlayerIdx(DWORD dwDBID) {
	std::map<DWORD, CPlayer*>::iterator it = _PlayerIdx.find(dwDBID);
	if (it != _PlayerIdx.end()) {
		_PlayerIdx.erase(it);
	}
	else {
		ToLogService("errors", LogLevel::Error, "when delete PlayerIdx it appear error, DB ID = {} no find index", dwDBID);
	}
}

CPlayer* CGameApp::GetPlayerByDBID(DWORD dwDBID) {
	return _PlayerIdx[dwDBID];
}

CPlayer* CGameApp::GetPlayerByMainChaName(const char* sMainChaName) {
	if (!sMainChaName) {
		return nullptr;
	}
	for (auto it = _PlayerIdx.begin(); it != _PlayerIdx.end(); ++it) {
		if (it->second && it->second->GetMainCha()) {
			if (!strcmp(it->second->GetMainCha()->GetName(), sMainChaName)) {
				return it->second;
			}
		}
	}
	return nullptr;
}

CSkillTempData* CGameApp::GetSkillTData(short sSkillNo, char chSkillLv) {
	if (!m_pCSkillTData[sSkillNo][chSkillLv]) {
		m_pCSkillTData[sSkillNo][chSkillLv] = m_SkillTDataPool.Get();
		if (!m_pCSkillTData[sSkillNo][chSkillLv])
			return 0;
		CSkillRecord* pCSkillRec = GetSkillRecordInfo(sSkillNo);
		if (!pCSkillRec)
			return 0;

		m_sSkillSetNo  = sSkillNo;
		m_chSkillSetLv = chSkillLv;
		// SP
		if (pCSkillRec->szUseSP != "0")
			m_pCSkillTData[sSkillNo][chSkillLv]->sUseSP = (Short)g_luaAPI.CallR<int>(pCSkillRec->szUseSP, (int)chSkillLv).value_or(0);
		else
			m_pCSkillTData[sSkillNo][chSkillLv]->sUseSP = 0;

		if (pCSkillRec->szUseEndure != "0")
			m_pCSkillTData[sSkillNo][chSkillLv]->sUseEndure = (Short)g_luaAPI.CallR<int>(pCSkillRec->szUseEndure, (int)chSkillLv).value_or(0);
		else
			m_pCSkillTData[sSkillNo][chSkillLv]->sUseEndure = 0;

		if (pCSkillRec->szUseEnergy != "0")
			m_pCSkillTData[sSkillNo][chSkillLv]->sUseEnergy = (Short)g_luaAPI.CallR<int>(pCSkillRec->szUseEnergy, (int)chSkillLv).value_or(0);
		else
			m_pCSkillTData[sSkillNo][chSkillLv]->sUseEnergy = 0;

		m_pCSkillTData[sSkillNo][chSkillLv]->sRange[0] = enumRANGE_TYPE_NONE;
		if (pCSkillRec->szSetRange != "0")
			g_luaAPI.Call(pCSkillRec->szSetRange, (int)chSkillLv);

		m_pCSkillTData[sSkillNo][chSkillLv]->sStateParam[0] = SSTATE_NONE;
		if (pCSkillRec->szRangeState != "0")
			g_luaAPI.Call(pCSkillRec->szRangeState, (int)chSkillLv);

		if (pCSkillRec->szFireSpeed != "0")
			m_pCSkillTData[sSkillNo][chSkillLv]->lResumeTime = g_luaAPI.CallR<int>(pCSkillRec->szFireSpeed.c_str(), (int)chSkillLv).value_or(0);
		else
			m_pCSkillTData[sSkillNo][chSkillLv]->lResumeTime = 0;
	}

	return m_pCSkillTData[sSkillNo][chSkillLv];
}

void CGameApp::SetSkillTDataRange(short* psRange) {
	if (m_sSkillSetNo < 0 || m_sSkillSetNo > defMAX_SKILL_NO) return;
	if (m_chSkillSetLv < 0 || m_chSkillSetLv > defMAX_SKILL_LV) return;
	memcpy(m_pCSkillTData[m_sSkillSetNo][m_chSkillSetLv]->sRange, psRange, sizeof(short) * defSKILL_RANGE_EXTEP_NUM);
}

void CGameApp::SetSkillTDataState(short* psState) {
	if (m_sSkillSetNo < 0 || m_sSkillSetNo > defMAX_SKILL_NO) return;
	if (m_chSkillSetLv < 0 || m_chSkillSetLv > defMAX_SKILL_LV) return;
	memcpy(m_pCSkillTData[m_sSkillSetNo][m_chSkillSetLv]->sStateParam, psState, sizeof(short) * defSKILL_STATE_PARAM_NUM);
}

void CGameApp::InitSStateTraOnTime() {
	memset(m_lSStateTraOnTime, 0, sizeof(m_lSStateTraOnTime));
	SkillStateRecordStore::Instance()->ForEach([this](CSkillStateRecord& rec) {
		if (rec.Id < 1 || rec.Id > AREA_STATE_MAXID)
			return;
		if (rec.szOnTransfer == "0")
			return;
		if (!g_luaAPI.HasFunction(rec.szOnTransfer.c_str())) {
			ToLogService("lua", LogLevel::Warning,
						 "Skill state {} has szOnTransfer='{}' but function not found",
						 rec.Id, rec.szOnTransfer);
			return;
		}
		for (int j = 1; j <= SKILL_STATE_LEVEL; j++) {
			m_lSStateTraOnTime[rec.Id][j] = g_luaAPI.CallR<int>(rec.szOnTransfer.c_str(), (int)j).value_or(0);
		}
	});
}

long CGameApp::GetSStateTraOnTime(unsigned char uchStateID, unsigned char uchStateLv) {
	return m_lSStateTraOnTime[uchStateID][uchStateLv];
}

CCharacter* CGameApp::FindPlayerChaByName(const char* cszChaName) {
	BEGINGETGATE();
	GateServer* pGateServer;
	while (pGateServer = GETNEXTGATE()) {
		for (CPlayer* pCPlayer : pGateServer->m_playerlist) {
			CCharacter* pCha = pCPlayer->GetCtrlCha();
			if (!pCha)
				continue;
			if (!strcmp(pCha->GetName(), cszChaName))
				return pCha;
		}
	}
	return nullptr;
}

CCharacter* CGameApp::FindPlayerChaByNameLua(const char* cszChaName) {
	BEGINGETGATE();
	GateServer* pGateServer;
	while (pGateServer = GETNEXTGATE()) {
		for (CPlayer* pCPlayer : pGateServer->m_playerlist) {
			CCharacter* pCha = pCPlayer->GetCtrlCha();
			if (!pCha)
				continue;
			if (!strcmp(pCha->GetPlayer()->GetMainCha()->GetName(), cszChaName))
				return pCha;
		}
	}
	return nullptr;
}

int CGameApp::FindPlayerChaByActNameLua(const char* cszChaName, CCharacter* chas[3]) {
	BEGINGETGATE();
	GateServer* pGateServer;
	int count = 0;
	while (pGateServer = GETNEXTGATE()) {
		for (CPlayer* pCPlayer : pGateServer->m_playerlist) {
			CCharacter* pCha = pCPlayer->GetCtrlCha();
			if (!pCha)
				continue;
			if (!strcmp(pCPlayer->GetActName(), cszChaName))
				chas[count++] = pCha;
		}
	}
	return count;
}

bool CGameApp::DealAllInGuild(int guildID, const char* luaFunc, const char* luaParam) {
	BEGINGETGATE();
	GateServer* pGateServer;
	while (pGateServer = GETNEXTGATE()) {
		for (CPlayer* pCPlayer : pGateServer->m_playerlist) {
			CCharacter* pCha = pCPlayer->GetCtrlCha();
			if (!pCha)
				continue;
			if (pCha->GetPlayer()->GetMainCha()->GetValidGuildID() == guildID) {
				if (luaParam != nullptr && luaParam[0] != '\0') {
					g_luaAPI.Call(luaFunc, pCha, luaParam);
				}
				else {
					g_luaAPI.Call(luaFunc, pCha);
				}
			}
		}
	}
	return true;
}

CCharacter* CGameApp::FindPlayerChaByID(unsigned long ulChaID) {
	BEGINGETGATE();
	GateServer* pGateServer;
	while (pGateServer = GETNEXTGATE()) {
		for (CPlayer* pCPlayer : pGateServer->m_playerlist) {
			CCharacter* pCha = pCPlayer->GetCtrlCha();
			if (!pCha)
				continue;
			if (pCha->GetID() == ulChaID)
				return pCha;
		}
	}
	return nullptr;
}

CPlayer* CGameApp::FindPlayerByDBChaID(unsigned long ulDBChaID) {
	BEGINGETGATE();
	GateServer* pGateServer;
	while (pGateServer = GETNEXTGATE()) {
		for (CPlayer* pCPlayer : pGateServer->m_playerlist) {
			if (pCPlayer->GetDBChaId() == ulDBChaID)
				return pCPlayer;
		}
	}
	return nullptr;
}

CCharacter* CGameApp::FindMainPlayerChaByID(unsigned long ulChaID) {
	BEGINGETGATE();
	GateServer* pGateServer;
	while (pGateServer = GETNEXTGATE()) {
		for (CPlayer* pCPlayer : pGateServer->m_playerlist) {
			CCharacter* pCha = pCPlayer->GetMainCha();
			if (!pCha)
				continue;
			if (pCha->GetID() == ulChaID)
				return pCha;
		}
	}
	return nullptr;
}

CCharacter* CGameApp::FindChaByID(unsigned long ulChaID) {
	CCharacter* pResult = nullptr;
	GamePool::Instance().ForEachCharacter([ulChaID, &pResult](CCharacter* pCCha) {
		if (!pResult && pCCha->GetID() == ulChaID) {
			pResult = pCCha;
		}
	});
	return pResult;
}

CCharacter* CGameApp::FindChaByName(const char* cszChaName) {
	CCharacter* pResult = nullptr;
	GamePool::Instance().ForEachCharacter([cszChaName, &pResult](CCharacter* pCCha) {
		if (!pResult && !strcmp(pCCha->GetName(), cszChaName)) {
			pResult = pCCha;
		}
	});
	return pResult;
}
