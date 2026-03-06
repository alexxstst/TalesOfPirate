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

#include "EntityAlloc.h"
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
#include "ItemAttr.h"
#include "LevelRecord.h"
#include "SailLvRecord.h"
#include "LifeLvRecord.h"
#include "CharForge.h"
#include "HairRecord.h"

#include "Parser.h"
#include "WorldEudemon.h"
#include "Birthplace.h"
#include "CharBoat.h"
#include "MapEntry.h"

using namespace std;
_DBC_USING;

bool		g_bLogEntity = false;

CItemRecordAttr*  g_pCItemAttr = NULL;
CCharacter*	g_pCSystemCha = NULL;
SubMap*		g_pScriptMap = NULL;		// 
long		g_lDeftMMaskLight = 21;
string		g_strChaState[2];


// cha
uLong		g_ulCurID = defINVALID_CHA_ID;
Long		g_lCurHandle = defINVALID_CHA_HANDLE;
//


extern BOOL g_bGameEnd;
extern std::string g_strLogName;

char		szChaInfoName[256]			= "CharacterInfo";
char		szSkillInfoName[256]		= "skillinfo";
char		szSkillStateInfoName[256]	= "skilleff";
char		szChaLvUpName[256]			= "character_lvup";
char		szChaSailLvUpName[256]		= "saillvup";
char		szChaLifeLvUpName[256]		= "lifelvup";
char		szItemInfoName[256]			= "ItemInfo";
char		szInitChaItName[256]		= "Int_Cha_Item";
char		g_szForgeTable[256]			= "forgeitem";
char		szIslandInfoName[256]		= "AreaSet";
char		szHairInfoName[64]			= "Hairs";		// 

CAreaSet * CAreaSet::_Instance = NULL;

void ChaException(uLong ulCurID, Long lCurHandle)
{
	if(g_ulCurID == defINVALID_CHA_ID || g_lCurHandle == defINVALID_CHA_HANDLE)
	{
		LG("exception3", "unknown (ID:%u, Handle:%d)occur abnormity\n", ulCurID, lCurHandle);
		return ;
	}

	Entity	*pCEnti = g_pGameApp->IsValidEntity(ulCurID, lCurHandle);
	if (!pCEnti)
	{
		//LG("exception3", "ID:%u, Handle:%d\n", ulCurID, lCurHandle);
		LG("exception3", "unknown entity(ID:%u, Handle:%d)occur abnormity\n", ulCurID, lCurHandle);
		return;
	}
	CCharacter *pCurCha = pCEnti->IsCharacter();
	if (!pCurCha)
	{
		//LG("exception3", "ID:%u, Handle:%d\n", ulCurID, lCurHandle);
		LG("exception3", "unknown character(ID:%u, Handle:%d)occur abnormity\n", ulCurID, lCurHandle);
		return;
	}

	try
	{
		//LG("exception3", "[%s] [%s], \n", pCurCha->GetLogName(), pCurCha->GetPlyMainCha()->GetLogName());
		LG("exception3", "character[%s] [%s]occur abnormity, will be kick out game\n", pCurCha->GetLogName(), pCurCha->GetPlyMainCha()->GetLogName());
		// ....
		CPlayer	*pCPlayer = pCurCha->GetPlayer();
		if (pCPlayer)
		{
			//LG("exception3", "[%s] [%s], [GoOutGame]\n", pCurCha->GetLogName(), pCurCha->GetPlyMainCha()->GetLogName());
			LG("exception3", "player character[%s] [%s]occur abnormity, [GoOutGame]\n", pCurCha->GetLogName(), pCurCha->GetPlyMainCha()->GetLogName());
			KICKPLAYER(pCPlayer, 0);
			LG("exception3", "End [KICKPLAYER], Begin[GoOutGame]\n");
			g_pGameApp->GoOutGame(pCPlayer, true);
			LG("exception3", "End [GoOutGame]\n");
			return;
		}
		else
		{
			//LG("exception3", "[%s], Begin\n", pCurCha->GetName());
			LG("exception3", "release bugbear character[%s], Begin\n", pCurCha->GetName());
			pCurCha->Free();
			//LG("exception3", "End \n");
			LG("exception3", "End release bugbear character\n");
		}
	}
	catch (...)
	{
		//LG("exception3", ", , [%s]\n", pCurCha->GetName());
		LG("exception3", "when character loop occur abnormity, the process kick character occur abnormity again, [%s]\n", pCurCha->GetName());
	}
}

// 
DWORD WINAPI g_GameLogicProcess(LPVOID lpParameter)
{
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

    while (!g_bGameEnd)
	{
		T_B
		
		DWORD	dwInterval = 50; // 

		//lua_FrameMove();
		if(g_pGameApp->m_bExecLuaCmd) 
		{
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

		T_EXIT
	}
	//LG("init", "!\n");
	LG("init", "game thread finish!\n");
	return 0;
}

// Add by lark.li 20080324 begin
void CDBLogMgr::TradeLog(const char* action, const char *pszChaFrom, const char *pszChaTo, const char *pszTrade)
{
	game_db.ExecTradeLogSQL(g_Config.m_szName, action, pszChaFrom, pszChaTo, pszTrade);
}

// End

// Log 5, 8000
void CDBLogMgr::Log(const char *type, const char *c1, const char *c2, const char *c3, const char *c4, const char *p, BOOL bAddToList)
{	
	return;
	
	if(bAddToList)
	{
		SDBLogData *pData = &_LogPool[_nPoolUseLoc];
		_nPoolUseLoc++;
		if(_nPoolUseLoc >= MAX_DBLOG_POOL)
		{
			_nPoolUseLoc = 0;
		}
		sprintf(pData->szLog, "insert gamelog (action, c1, c2, c3, c4, content) \
			   values('%s', '%s', '%s', '%s', '%s', '%s')", type, c1, c2, c3, c4, p);
		_LogList.push_back(pData);
	}
	else
	{
		char szLog[8192];	
		sprintf(szLog, "insert gamelog (action, c1, c2, c3, c4, content) \
			   values('%s', '%s', '%s', '%s', '%s', '%s')", type, c1, c2, c3, c4, p);
		game_db.ExecLogSQL(szLog);
	}
}


// , logDB
void CDBLogMgr::HandleLogList() 
{
	if(_LogList.empty()) 
	{
		_nLogLeft = 0;
		return;
	}
		
	_nLogLeft = (int)(_LogList.size());
		
	
	MPTimer t; t.Begin();
	
	
	BOOL bFlush = FALSE;
	// 
	for(int i = 0; i < _nPerLogCnt; i++)
	{
		if(_LogList.empty()) break;
		SDBLogData *pData = _LogList.front();
		game_db.ExecLogSQL(pData->szLog);
		_LogList.pop_front();

		// pool
		if(pData->nLoc > _nPoolUseLoc)
		{
			if(pData->nLoc - _nPoolUseLoc < 10) // , , 
			{
				bFlush = TRUE;
				//LG("dblog_error", "DBLogPoolHandleLoc=[%d], PoolUseLoc=[%d]DBLog, Flushlog!\n", pData->nLoc, _nPoolUseLoc);
				LG("dblog_error", "DBLogPool position will superpose HandleLoc=[%d], PoolUseLoc=[%d]deal with DBLog speed abnormity, carry out Flush all leavings log!\n", pData->nLoc, _nPoolUseLoc);
				break;
			}
		}
	}

	if(bFlush)
	{
		FlushLogList();
	}

	LG("dblog", "dblog exec sql use time = %d\n", t.End());
}

// GameServer, logDB
void CDBLogMgr::FlushLogList()  
{
	for(list<SDBLogData*>::iterator it = _LogList.begin(); it!=_LogList.end(); it++)
	{
		SDBLogData *pData = (*it);
		game_db.ExecLogSQL(pData->szLog);
	}

	_LogList.clear();
}




CGameApp::CGameApp()
:_dwLastTick(0),
 _dwTempRunCnt(0),
 m_dwRunCnt(0),
 m_dwFPS(0),
 m_bExecLuaCmd(0),
 m_dwChaCnt(0),
 m_dwPlayerCnt(0),
 m_dwRunStep(0),
 m_CabinHeap(1, 1000),
 m_MapStateCellHeap(1, 2000),
 m_ChaListHeap(1, 2000),
 m_StateCellNodeHeap(1, 1000),
 m_SkillTDataHeap(1, defMAX_SKILL_NO),
 m_TradeDataHeap(1, ROLE_MAXSIZE_TRADEDATA),
 m_StallDataHeap(1, ROLE_MAXSIZE_STALLDATA),
 m_mapnum(0),
 m_ulLeftSec(0)
{T_B
	extern CGameApp *g_pGameApp;
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
	m_pCEntSpace = 0;
	m_pCPlySpace = 0;
	m_PicSet = NULL;
	m_fGlobalDropRate = 0;
	m_fGlobalExpRate = 0;
	ChaAttrMaxValInit(false);
T_E}


CGameApp::~CGameApp()
{T_B
	
	//Log("", "haha", "", "" ,"", "");
	Log("close", "haha", "", "" ,"", "");
	FlushLogList();

	for(short i=0; i< m_mapnum; i++)
	{
		SAFE_DELETE(m_MapList[i]);
	}
	
	CloseLuaScript();
	//g_CParser.Free();
	
	SAFE_DELETE(m_CChaRecordSet);
	SAFE_DELETE(m_CSkillRecordSet);
	SAFE_DELETE(m_pCEntSpace);
	SAFE_DELETE(m_pCPlySpace);

	SAFE_DELETE(m_PicSet);

	m_vecVolunteerList.clear();
T_E}


const char* GetResPath(const char *pszRes)
{
	static char g_szTableName[255];
	string str = g_Config.m_szResDir;
	if(str.size() > 0)
	{
		str+="/";
	}
	str+=pszRes;
	strcpy(g_szTableName, str.c_str());
	return g_szTableName;
}

BOOL LoadTable(CRawDataSet *pTable, const char *pszTableName)
{
	g_bBinaryTable = FALSE;
	string str = GetResPath(pszTableName);
	if(pTable->LoadRawDataInfo(str.c_str())==FALSE)
	{
		//LG("error", "msg[%s]\n", str.c_str());
		LG("error", RES_STRING(GM_GAMEAPP_CPP_00003), str.c_str());
		return FALSE;
	}
	return TRUE;
}


BOOL CGameApp::Init()
{T_B
	//LG("init", "GameApp\n");
	LG("init", "start initialization GameApp\n");

	//LG("init", "...\n");
	LG("init", "initialization DB...\n");
	if(game_db.Init())
	{	// 
		LG("init", "database init...ok\n");
	}
	else
	{
		LG("init", "database init...Fail!\n");
		return FALSE;
	}

	// Add by lark.li 20080324 begin
	if(tradeLog_db.Init())
	{	// 
		LG("init", "database init...ok\n");
	}
	else
	{
		LG("init", "database init...Fail!\n");
		return FALSE;
	}
	// End

	ChaAttrMaxValInit(false);
	if( !CTextFilter::LoadFile( GetResPath("ChaNameFilter.txt" )) )
	{
		//LG( "init", "msg!\n" );
		LG( "init", RES_STRING(GM_GAMEAPP_CPP_00004) );
		return FALSE;		
	}

	//m_Ident.m_maxID = g_Config.m_ulBaseID; //ID
	m_Ident.m_maxID = 0xafffffff; //ID
	if(!m_Ident.m_maxID)
		//LG("init", "ID!!!\n");
		LG("init", "error ID base!!!\n");
	m_ItemIdent.m_maxID = 1;

	m_pCPlySpace = new CPlayerAlloc(g_Config.m_nMaxPly);
	if (!m_pCPlySpace)
	{
		//LG("init", "msg!, !\n");
		LG("init", RES_STRING(GM_GAMEAPP_CPP_00005));
		return FALSE;
	}
	m_pCEntSpace = new CEntityAlloc(g_Config.m_nMaxCha + g_Config.m_nMaxPly * 3, g_Config.m_nMaxItem, g_Config.m_nMaxTNpc);
	if (!m_pCEntSpace)
	{
		//LG("init", "msg!, !\n");
		LG("init", RES_STRING(GM_GAMEAPP_CPP_00017));
		return FALSE;
	}
	g_pCSystemCha = GetNewCharacter();
	g_pCSystemCha->SetID(m_Ident.GetID());
	//g_pCSystemCha->SetName("");
	g_pCSystemCha->SetName(RES_STRING(GM_GAMEAPP_CPP_00018));
	g_pCSystemCha->setAttr(ATTR_CHATYPE, enumCHACTRL_NONE, 1);

    //LG("init", "Entity\n");
	LG("init", "start to assign every Entity memory\n");
	m_CabinHeap.Init();
	m_TradeDataHeap.Init();
	m_MapStateCellHeap.Init();
	m_ChaListHeap.Init();
	m_SkillTDataHeap.Init();
	m_StateCellNodeHeap.Init();

	//LG("init", "...\n");
	LG("init", "initialization every form...\n");
	int	nItemRecordNum = CItemRecord::enumItemMax;
	
	m_CChaRecordSet			= new CChaRecordSet(0, defMAX_CHARINFO_NO);			LoadTable(m_CChaRecordSet,		 szChaInfoName);
	m_CSkillRecordSet		= new CSkillRecordSet(0, defMAX_SKILL_NO);			LoadTable(m_CSkillRecordSet,	 szSkillInfoName);
	m_CLevelRecordSet		= new CLevelRecordSet(0, 600);			LoadTable(m_CLevelRecordSet,	 szChaLvUpName);
	m_CSailLvRecord			= new CSailLvRecordSet(0, 600);			LoadTable(m_CSailLvRecord,		 szChaSailLvUpName);
	m_CLifeLvRecord			= new CLifeLvRecordSet(0, 600);			LoadTable(m_CLifeLvRecord,		 szChaLifeLvUpName);
	m_CHairRecord			= new CHairRecordSet(0, 300);			LoadTable(m_CHairRecord,		 szHairInfoName);	
	// Modify by lark.li 20080822 begin
	//m_CSkillStateRecordSet	= new CSkillStateRecordSet(0, SKILL_STATE_MAXID);		LoadTable(m_CSkillStateRecordSet,szSkillStateInfoName);
	m_CSkillStateRecordSet	= new CSkillStateRecordSet(0, 300);		LoadTable(m_CSkillStateRecordSet,szSkillStateInfoName);
	// End
	
	m_CItemRecordSet		= new CItemRecordSet(0, nItemRecordNum);LoadTable(m_CItemRecordSet, szItemInfoName);

	//
	/*m_PicSet = new CPicSet();
	if(!m_PicSet->init())
	{
		LG( "init", "msg" );
		return FALSE;
	}*/
	
	g_pCItemAttr = new CItemRecordAttr[nItemRecordNum];
	for (int i = 0; i < nItemRecordNum; i++)
	{
		g_pCItemAttr[i].Init(i);
	}
	
	m_CJobEquipRecordSet = new CJobEquipRecordSet(0, MAX_JOB_TYPE + 1); LoadTable(m_CJobEquipRecordSet, szInitChaItName);
	m_CAreaRecordSet = new CAreaSet(0, 256);							LoadTable(m_CAreaRecordSet, szIslandInfoName);

	if( !g_ForgeSystem.LoadForgeData( g_szForgeTable ) )
	{
		//LG( "init", "msg" );
		LG( "init", RES_STRING(GM_GAMEAPP_CPP_00006) );
		return FALSE;
	}

	if( !g_CharBoat.Load( "ShipInfo", "ShipItemInfo"))
	{
		//LG( "init", "msg" );
		LG( "init", RES_STRING(GM_GAMEAPP_CPP_00007) );
		return FALSE;
	}

	printf("Loading %s\n", LUAJIT_VERSION);
	printf("Loading Serverfile 1.38\n");
	//LG("init", "Lua Script...\n");
	LG("init", "initialization Lua Script...\n");
	InitLuaScript();

	if(InitMap()==FALSE)
	{
		//LG("init", "!, !\n");
		LG("init", "initialization map failed!, exit!\n");
		return FALSE;
	}

	InitSStateTraOnTime();

	if (!IsChaAttrMaxValInit())
	{
		//LG("init", "...Fail!\n");
		LG("init", "character attribute max...Fail!\n");
		return FALSE;
	}

	//LG("init", "GameApp!\n");
	LG("init", "GameApp initialization finish!\n");

	LG("size", "sizeof(Entity) = %d,  sizeof(Character) = %d\n", sizeof(Entity), sizeof(CCharacter));
	
	m_CTimerItem.Begin(3 * 1000);

	return TRUE;
T_E}
    

void CGameApp::Run(DWORD dwCurTime)
{T_B
	m_dwRunStep = 0;

	DWORD	dwChaCnt, dwPlayerCnt, dwActiveMgrUnit;
	dwChaCnt = m_dwChaCnt;
	dwPlayerCnt = m_dwPlayerCnt;
	dwActiveMgrUnit = m_dwActiveMgrUnit;

	static DWORD	dwRunTick = GetTickCount();
	static Long		lLogTime = 0;
	MPTimer t, t1;

	t1.Begin();
	m_dwRunStep = 1;

	t.Begin();
    MgrUnitRun(dwCurTime);
	DWORD	dwMgrUnitRunTime = t.End();

	t.Begin();
	GameItemRun(dwCurTime);
	DWORD	dwItemRunTime = t.End();

	t.Begin();
	MapMgrRun(dwCurTime);
	DWORD	dwMapMgrRunTime = t.End();

	t.Begin();
	//if (dwChaCnt != m_dwChaCnt || dwPlayerCnt != m_dwPlayerCnt || dwActiveMgrUnit != m_dwActiveMgrUnit)
	//	LG("PanelData", "ChaNum=%5d,\tPlayerNum=%4d,\tActMgrUnit=%6d\n", m_dwChaCnt, m_dwPlayerCnt, m_dwActiveMgrUnit);
	// 
	//DataStatistic();
	DWORD	dwDataStatisticTime = t.End();

	t.Begin();
	m_dwRunStep = 100;
    g_CTimeSkillMgr.Run(dwCurTime);
	DWORD	dwSkillMgrRunTime = t.End();

	t.Begin();
	m_dwRunStep = 103;
	game_db.GetMapMaskTable()->HandleSaveList();
	DWORD	dwMapMaskSaveTime = t.End();

	DWORD	dwGameRunTime = t1.End();

	// 
    if(dwCurTime - _dwLastTick >= 1000)
    {
        m_dwFPS       = _dwTempRunCnt;
        _dwTempRunCnt = 0;
        _dwLastTick   = dwCurTime;
    }
    m_dwRunCnt++;     // , 
    _dwTempRunCnt++;  // 
    m_dwRunStep = 104;

	if (m_ulLeftSec > 0 && m_CTimerReset.IsOK(dwCurTime))
	{
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
	DWORD	dwRunTimeKey = 100;
	bool	bLogRunTime = false;
    if (dwCurTime - dwRunTick >= 30 * 1000)
		bLogRunTime = true;
	if (dwGameRunTime >= dwRunTimeKey)
	{
		if (lLogTime >= 0)
		{
			lLogTime++;
			if (lLogTime <= 6) bLogRunTime = true;
			else lLogTime = -1;
		}
		else
		{
			lLogTime--;
			if (lLogTime < -1000) lLogTime = 0;
		}
	}
	else
	{
		if (lLogTime > 0)
			lLogTime = 0;
	}
	if (bLogRunTime)
	{
		dwRunTick = dwCurTime;

		if (dwGameRunTime >= dwRunTimeKey)
			LG("GameRunTime", "!!!GameRunTime = %u\t\tMgrUnitRunTime = %u\tItemRunTime = %u\tMapMgrRunTime = %u\tDataStatisticTime = %u\tSkillMgrRunTime = %u\tMapMaskSaveTime = %u\n",
				dwGameRunTime, dwMgrUnitRunTime, dwItemRunTime, dwMapMgrRunTime, dwDataStatisticTime, dwSkillMgrRunTime, dwMapMaskSaveTime);
		else
			LG("GameRunTime", "...GameRunTime = %u\t\tMgrUnitRunTime = %u\tItemRunTime = %u\tMapMgrRunTime = %u\tDataStatisticTime = %u\tSkillMgrRunTime = %u\tMapMaskSaveTime = %u\n",
				dwGameRunTime, dwMgrUnitRunTime, dwItemRunTime, dwMapMgrRunTime, dwDataStatisticTime, dwSkillMgrRunTime, dwMapMaskSaveTime);
	}
	//
T_E}

void CGameApp::MgrUnitRun(DWORD dwCurTime)
{T_B
	SubMap		*pCSubMap;

	m_dwChaCnt = m_dwPlayerCnt;
	m_dwActiveMgrUnit = 0;

	static DWORD	dwTick = 0;
	if (dwCurTime - dwTick >= 1 * 60 * 1000)
	{
		dwTick = dwCurTime;
		LG("EntityNum", "Ply[%5d %5d %5d],\tCha[%5d %5d %5d],\tItem[%5d %5d %5d],\tTNpc[%5d %5d %5d]\n",
			m_pCPlySpace->GetHoldPlyNum(), m_pCPlySpace->GetMaxHoldPlyNum(), m_pCPlySpace->GetAllocPlyNum(),
			m_pCEntSpace->GetHoldChaNum(), m_pCEntSpace->GetMaxHoldChaNum(), m_pCEntSpace->GetAllocChaNum(),
			m_pCEntSpace->GetHoldItemNum(), m_pCEntSpace->GetMaxHoldItemNum(), m_pCEntSpace->GetAllocItemNum(),
			m_pCEntSpace->GetHoldTNpcNum(), m_pCEntSpace->GetMaxHoldTNpcNum(), m_pCEntSpace->GetAllocTNpcNum());
	}

	CEyeshotCell	*pCMgrUnit;
	CStateCell		*pCStateCell;
	Entity			*pCEnt, *pCFlagEnt;
	Short			sMapCpyNum;
	DWORD			dwActMgrCellNum;

	MPTimer			t, t1, t2;
	DWORD			dwPartRunTime[8]{};
	Char			chPartCount;
	CCharacter		*pCLongTimeCha;
	DWORD			dwTempTick1, dwTempTick2;
	static DWORD	dwMapRunTick[MAX_MAP] = {0};
	static Long		lMapLogTime[MAX_MAP] = {0};
	bool			bLogRun;
	DWORD			dwRTimeKey = 60;

	for (short i = 0; i < m_mapnum; i++)
	{
		if (!m_MapList[i]->IsOpen()) continue;

		t1.Begin();
		dwActMgrCellNum = 0;
		pCLongTimeCha = 0;
		dwTempTick1 = 0;
		chPartCount = 0;

		sMapCpyNum = m_MapList[i]->GetCopyNum();
		for (short j = 0; j < sMapCpyNum; j++)
		{
			pCSubMap = m_MapList[i]->GetCopy(j);
			if (!pCSubMap)
				continue;
			pCSubMap->CheckRun();
			if (!pCSubMap->IsRun())
				continue;

			extern SubMap *g_pScriptMap;
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
			while (pCMgrUnit = pCSubMap->m_CEyeshotCellL.GetNext())
			{
				pCEnt = pCMgrUnit->m_pCChaL;
				while (pCEnt)
				{
					g_ulCurID = pCEnt->GetID();
					g_lCurHandle = pCEnt->GetHandle();

					t2.Begin();
					pCEnt->Run(dwCurTime);
					dwTempTick2 = t2.End();
					if (dwTempTick2 > dwTempTick1)
					{
						pCLongTimeCha = pCEnt->IsCharacter();
						dwTempTick1 = dwTempTick2;
					}

					pCFlagEnt = pCEnt;
					pCEnt = pCEnt->m_pCEyeshotCellNext;
					((CCharacter *)pCFlagEnt)->RunEnd( dwCurTime );
				}

				g_ulCurID = defINVALID_CHA_ID;
				g_lCurHandle = defINVALID_CHA_HANDLE;
			}
			dwPartRunTime[chPartCount++] = t.End();

			t.Begin();
			long	lActCount = 0;
			long	lTarNum = pCSubMap->m_CStateCellL.GetActiveNum();
			pCSubMap->m_CStateCellL.BeginGet();
			while (pCStateCell = pCSubMap->m_CStateCellL.GetNext())
			{
				if (++lActCount > lTarNum)
				{
					//LG("", " %d\n", lTarNum);
					LG("state cell error", "fact activation number %d\n", lTarNum);
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
		if (dwMMgrRunTime >= dwRTimeKey)
		{
			if (lMapLogTime[i] >= 0)
			{
				lMapLogTime[i]++;
				if (lMapLogTime[i] <= 6) bLogRun = true;
				else lMapLogTime[i] = -1;
			}
			else
			{
				lMapLogTime[i]--;
				if (lMapLogTime[i] < -1000) lMapLogTime[i] = 0;
			}
		}
		else
		{
			if (lMapLogTime[i] > 0)
				lMapLogTime[i] = 0;
		}
		if(bLogRun)
		{
			dwMapRunTick[i] = dwCurTime;

			if (chPartCount > 0)
			{
				if (dwMMgrRunTime >= dwRTimeKey)
					/*LG("map_time", "!!!%s(%d) = %d ms, WeatherRun = %u, CharacterRun = %u, StateRun = %u, ActiveMgrUnitNum = %d\n",
						m_MapList[i]->GetName(), sMapCpyNum, dwMMgrRunTime, dwPartRunTime[0], dwPartRunTime[1], dwPartRunTime[2], dwActMgrCellNum);*/
						LG("map_time", "!!!%s(%d) expend = %d ms, WeatherRun = %u, CharacterRun = %u, StateRun = %u, ActiveMgrUnitNum = %d\n",
						m_MapList[i]->GetName(), sMapCpyNum, dwMMgrRunTime, dwPartRunTime[0], dwPartRunTime[1], dwPartRunTime[2], dwActMgrCellNum);
				else
					/*LG("map_time", "...%s(%d) = %d ms, WeatherRun = %u, CharacterRun = %u, StateRun = %u, ActiveMgrUnitNum = %d\n",
						m_MapList[i]->GetName(), sMapCpyNum, dwMMgrRunTime, dwPartRunTime[0], dwPartRunTime[1], dwPartRunTime[2], dwActMgrCellNum);*/
						LG("map_time", "...%s(%d)expend = %d ms, WeatherRun = %u, CharacterRun = %u, StateRun = %u, ActiveMgrUnitNum = %d\n",
						m_MapList[i]->GetName(), sMapCpyNum, dwMMgrRunTime, dwPartRunTime[0], dwPartRunTime[1], dwPartRunTime[2], dwActMgrCellNum);

				if (pCLongTimeCha)
				{
					LG("map_time", "\t\"%s\" Check = %u, ActCache = %u, Resume = %u, Player = %u, AI = %u, Area = %u, Die = %u, Mission = %u, Team = %u, SkillState = %u, Move = %u, Fight = %u, DB = %u, Ping = %u\n",
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
T_E}

void CGameApp::GameItemRun(DWORD dwCurTime)
{
	if (!m_CTimerItem.IsOK(dwCurTime))
		return;

	m_pCEntSpace->BeginGetItem();
	CItem	*pCCur;
	while (pCCur = m_pCEntSpace->GetNextItem())
		pCCur->Run(dwCurTime);
}

void CGameApp::MapMgrRun(DWORD dwCurTime)
{
	CMapRes	*pCMap;

	for (short i = 0; i < m_mapnum; i++)
	{
		pCMap = m_MapList[i];
		if (!pCMap->IsValid()) continue;

		pCMap->Run(dwCurTime);
	}
}

void CGameApp::SetEntityEnableLog(bool bValid)
{T_B
	SubMap			*pCSubMap;
	//CEyeshotCell	*pUnit;
	Entity			*pCEnt;

	if (g_bLogEntity == bValid)
		return;

	g_bLogEntity = bValid;
	short	sMapCpyNum;
	for (short i = 0; i < m_mapnum; i++)
	{
		if (!m_MapList[i]->IsValid()) continue;
		sMapCpyNum = m_MapList[i]->GetCopyNum();
		for (short j = 0; j < sMapCpyNum; j++)
		{
			pCSubMap = m_MapList[i]->GetCopy(j);
			if (!pCSubMap)
				continue;

			for (short m = 0; m < pCSubMap->GetEyeshotCellLin(); m++)
			{
				for (short n = 0; n < pCSubMap->GetEyeshotCellCol(); n++)
				{
					pCEnt = pCSubMap->m_pCEyeshotCell[m][n].m_pCChaL;
					while (pCEnt)
					{
						pCEnt->m_CLog.SetEnable(bValid);
						pCEnt = pCEnt->m_pCEyeshotCellNext;
					}
					pCEnt = pCSubMap->m_pCEyeshotCell[m][n].m_pCItemL;
					while (pCEnt)
					{
						pCEnt->m_CLog.SetEnable(bValid);
						pCEnt = pCEnt->m_pCEyeshotCellNext;
					}
				}
			}
		}
	}
T_E}

// 
CPlayer* CGameApp::GetNewPlayer()
{T_B
	return m_pCPlySpace->GetNewPly();
T_E}

// 
CPlayer* CGameApp::GetPlayer(long lHandle)
{T_B
	return m_pCPlySpace->GetPly(lHandle);
T_E}

CPlayer* CGameApp::IsValidPlayer(long lID, long lHandle)
{T_B
	CPlayer	*pCPly = m_pCPlySpace->GetPly(lHandle);
	if (!pCPly)
		return 0;
	if (pCPly->GetID() != lID)
		return 0;

	return pCPly;
T_E}

// 
// chType01
CPlayer* CGameApp::CreateGamePlayer(const char szPassword[], uLong ulChaDBId, uLong ulWorldId, const char *cszMapName, char chType)
{T_B
	CPlayer	*pCOldPly = GetPlayerByDBID(ulChaDBId);
	if (pCOldPly)
	{
		//LG("", " %s[%u] \n", pCOldPly->GetMainCha()->GetName(), ulChaDBId);
		LG("repeat character", "when character %s[%u] enterfind it has exist\n", pCOldPly->GetMainCha()->GetName(), ulChaDBId);
		pCOldPly->GetCtrlCha()->BreakAction();
		pCOldPly->MisLogout();
		pCOldPly->MisGooutMap();

		ReleaseGamePlayer( pCOldPly );
		return NULL;
	}

    CPlayer  *l_player  = GetNewPlayer();
    CCharacter *pCMainCha = GetNewCharacter();
    if (!l_player || !pCMainCha)
	{
		if(l_player)
			l_player->Free();

		if(pCMainCha)
			pCMainCha->Free();

        // characterfree
       // LG("enter_map", "ID = %u \n", ulChaDBId);
		LG("enter_map", "when create new player characterID = %umemory assign failed \n", ulChaDBId);
        return NULL;
	}

	l_player->SetPassword( szPassword );
	l_player->SetID(ulWorldId);
    pCMainCha->SetPlayer(l_player);
    pCMainCha->SetID(ulWorldId);
	pCMainCha->_dwStallTick = 0;

    l_player->SetMisChar( *pCMainCha );
    l_player->SetMainCha(pCMainCha);
	l_player->SetCtrlCha(pCMainCha);

    l_player->SetDBChaId(ulChaDBId);
	l_player->SetMaskMapName(cszMapName);

    LG("enter_map", "atorID = %d, begin read data from database: \n", ulChaDBId);
    if (!game_db.ReadPlayer(l_player, ulChaDBId)) // Gate
    {
       // LG("enter_map", "atorID = %d, GameServerGateServer\n", ulChaDBId);
		 LG("enter_map", "atorID = %d,when get character dataappear errorlead to GameServer with GateServer disconnection\n", ulChaDBId);
        l_player->Free();
        return NULL;
    }

	// 
	if(!g_CharBoat.LoadBoat( *l_player->GetMainCha(), chType ))
    {
        LG("boat_excp", "Character %s[%d] Load Boat Failt.\n", l_player->GetMainCha()->GetName(), ulChaDBId);
        l_player->Free();
        return NULL;
    }

    AddPlayerIdx(ulChaDBId, l_player);
    g_pGameApp->m_dwPlayerCnt++;

    //LG("enter_map", "cha type = %d\n", pCMainCha->m_SChaPart.sTypeID); 
    //for(int i = 0; i < enumEQUIP_NUM; i++)
    //LG("enter_map", "olhe [%d] = %d\n", i, pCMainCha->m_SChaPart.SLink[i].sID);

    return l_player;

    LG("enter_map", "atorID = %d (%s) end entermap\n", ulChaDBId, pCMainCha->GetName());
    LG("enter_map", "atorID = %d, logname = [%s] end enter\n", ulChaDBId, pCMainCha->m_CLog.GetLogName());
T_E}

// 
void CGameApp::ReleaseGamePlayer(CPlayer* pPlayer)
{T_B
    // 
    if(pPlayer && pPlayer->IsValid())
	{
		CCharacter	*pCCha = pPlayer->GetCtrlCha();
		if (!pCCha)
			return;
		bool	bIsDie = !pCCha->IsLiveing();
		bool	bSavePos = false;
		SubMap	*pSrcMap = pCCha->GetSubMap();
		Point	SSrcPos;

		if (pSrcMap)
			pSrcMap->BeforePlyOutMap(pCCha);
		if (pSrcMap)
			bSavePos = pCCha->GetSubMap()->CanSavePos();
		if (bIsDie || !bSavePos) // 
		{
			if (bIsDie)
				g_CParser.DoString("Relive", enumSCRIPT_RETURN_NONE, 0, enumSCRIPT_PARAM_LIGHTUSERDATA, 1, pCCha, DOSTRING_PARAM_END);

			SSrcPos = pCCha->GetPos();
			if (pCCha->IsBoat()) // 
			{
				pCCha->SetToMainCha(bIsDie);

				pPlayer->GetMainCha()->ResetBirthInfo();
			}
			else
			{
				pCCha->ResetBirthInfo();
			}
		}

		LG("enter_map", "atorID = %d, begin goout\n", pPlayer->GetDBChaId());

		if(game_db.SavePlayer(pPlayer, enumSAVE_TYPE_OFFLINE)==false)
		{
			//LG("enter_map", "SavePlayer[%s]", pPlayer->GetMainCha()->GetName());
			LG("enter_map", "SavePlayer[%s] failed", pPlayer->GetMainCha()->GetName());
		}

		if (bIsDie || !bSavePos) // 
		{
			game_db.SavePlayerPos(pPlayer);
			pCCha->SetSubMap(pSrcMap);
			pCCha->SetPos(SSrcPos);
		}

		DelPlayerIdx(pPlayer->GetDBChaId());
		g_pGameApp->m_dwPlayerCnt--;
		pPlayer->Free();

		//////////////////////////////////////////////////////////////////////////
		// gate server
		pPlayer->OnLogoff();
		DELPLAYER(pPlayer);
		//////////////////////////////////////////////////////////////////////////

		LG("enter_map", "atorID = %d, goout\n", pPlayer->GetDBChaId());
	}
T_E}

void CGameApp::GoOutGame(CPlayer* pPlayer, bool bOffLine, bool mOffLine)
{
    if(pPlayer && pPlayer->IsValid())
	{
		//here there is a bug and this the fix , gotta test the bug first before apply the fix //
		if (g_Config.m_bOfflineMode && mOffLine) {
			pPlayer->SetCanReceiveRequests(false);
			return;
		}
		if(pPlayer->GetMainCha()->IsVolunteer()) {
			pPlayer->GetMainCha()->Cmd_DelVolunteer();
		}
		pPlayer->GetCtrlCha()->BreakAction();
		if (bOffLine) {
			pPlayer->MisLogout();
		}
		pPlayer->MisGooutMap();
		ReleaseGamePlayer(pPlayer);
	} else {
		LG("error", "GoOutGame\n");
	}
}

void CGameApp::NoticePlayerLogin(CPlayer *pCPlayer)
{
	if (!pCPlayer || !pCPlayer->GetCtrlCha())
		return;

	WPACKET WtPk  = GETWPACKET();
	WRITE_CMD(WtPk, CMD_MM_LOGIN);
	WRITE_LONG(WtPk, 0);
	WRITE_STRING(WtPk, pCPlayer->GetCtrlCha()->GetName());

	BEGINGETGATE();
	GateServer	*pGateServer;
	while (pGateServer = GETNEXTGATE())
	{
		WRITE_LONG(WtPk, 0);
		WRITE_LONG(WtPk, 0);
		WRITE_SHORT(WtPk, 1);
		pGateServer->SendData(WtPk);
		break;
	}
}

void CGameApp::AfterPlayerLogin(const char *cszPlyName)
{
	if (!cszPlyName)
		return;

	g_CDMapEntry.AfterPlayerLogin(cszPlyName);
}

// 
CCharacter* CGameApp::GetNewCharacter()
{T_B
	return m_pCEntSpace->GetNewCha();
T_E}

// 
CItem* CGameApp::GetNewItem()
{T_B
	return m_pCEntSpace->GetNewItem();
T_E}

// NPC
mission::CTalkNpc* CGameApp::GetNewTNpc()
{T_B
	return m_pCEntSpace->GetNewTNpc();
T_E}

Entity* CGameApp::GetEntity(long lHandle)
{T_B
	return m_pCEntSpace->GetEntity(lHandle);
T_E}

// 
Entity* CGameApp::IsValidEntity(unsigned long ulID, long lHandle)
{T_B
	Entity	*pEnti = g_pGameApp->GetEntity(lHandle);
	if (!pEnti)
		return 0;
	if (!pEnti->IsValid()
		|| ulID != pEnti->GetID())
		return 0;

	return pEnti;
T_E}

// 
Entity* CGameApp::IsLiveingEntity(unsigned long ulID, long lHandle)
{T_B
	Entity	*pEnti = IsValidEntity(ulID, lHandle);
	if (!pEnti)
		return 0;
	if (pEnti->IsCharacter() != g_pCSystemCha)
		if (!pEnti->IsLiveing() || !pEnti->GetSubMap())
			return 0;

	return pEnti;
T_E}

// 
Entity* CGameApp::IsMapEntity(unsigned long ulID, long lHandle)
{T_B
	Entity	*pEnti = IsValidEntity(ulID, lHandle);
	if (!pEnti)
		return 0;
	if (pEnti->IsCharacter() != g_pCSystemCha)
		if (!pEnti->GetSubMap())
			return 0;

	return pEnti;
T_E}

// 
Entity* CGameApp::IsLifeEntity(unsigned long ulID, long lHandle)
{T_B
	Entity	*pEnti = IsValidEntity(ulID, lHandle);
	if (!pEnti)
		return 0;
	if (pEnti->IsCharacter() != g_pCSystemCha)
		if (!pEnti->IsLiveing())
			return 0;

	return pEnti;
T_E}

// 
BOOL CGameApp::InitMap()
{T_B
	//LG("init", "...\n");
	LG("init", "initialization map list...\n");
	
	mission::g_WorldEudemon.Load( "Eudemon", g_Config.m_szEqument, -1 );

	//Map
	m_mapnum = g_Config.m_nMapCnt;
	if(m_mapnum < 1)
	{
		//LG("init", "0,, !\n");
		LG("init", "collocate map number 0,no map was initialization, exit!\n");
		return FALSE;
	}

	m_strMapNameList = "";

	//
	memset(m_MapList, 0, sizeof(CMapRes*) * MAX_MAP);
	for(short i=0; i< m_mapnum; i++)
	{
		m_MapList[i] = new CMapRes();
		m_MapList[i]->SetName(g_Config.m_szMapList[i]);

		strcpy(m_MapList[i]->m_szObstacleFile, m_MapList[i]->GetName());		//
		strcat(m_MapList[i]->m_szObstacleFile, "\\"); 
		strcat(m_MapList[i]->m_szObstacleFile, m_MapList[i]->GetName());
		strcat(m_MapList[i]->m_szObstacleFile, ".blk");
		strcpy(m_MapList[i]->m_szSectionFile, m_MapList[i]->GetName());			//
		strcat(m_MapList[i]->m_szSectionFile, "\\");
		strcat(m_MapList[i]->m_szSectionFile, m_MapList[i]->GetName());
		strcpy(m_MapList[i]->m_szMonsterSpawnFile, m_MapList[i]->GetName());	//
		strcat(m_MapList[i]->m_szMonsterSpawnFile, "\\");
		strcat(m_MapList[i]->m_szMonsterSpawnFile, m_MapList[i]->GetName());
		strcat(m_MapList[i]->m_szMonsterSpawnFile, "ChaSpn");
		strcpy(m_MapList[i]->m_szNpcSpawnFile, m_MapList[i]->GetName());		//npc
		strcat(m_MapList[i]->m_szNpcSpawnFile, "\\");
		strcat(m_MapList[i]->m_szNpcSpawnFile, m_MapList[i]->GetName());
		strcat(m_MapList[i]->m_szNpcSpawnFile, "NPC");
		strcpy(m_MapList[i]->m_szMapSwitchFile, m_MapList[i]->GetName());		//
		strcat(m_MapList[i]->m_szMapSwitchFile, "\\");
		strcat(m_MapList[i]->m_szMapSwitchFile, m_MapList[i]->GetName());
		strcat(m_MapList[i]->m_szMapSwitchFile, "SwhMap");
		strcpy(m_MapList[i]->m_szMonsterCofFile, m_MapList[i]->GetName());		//
		strcat(m_MapList[i]->m_szMonsterCofFile, "\\");
		strcat(m_MapList[i]->m_szMonsterCofFile, m_MapList[i]->GetName());
		strcat(m_MapList[i]->m_szMonsterCofFile, "monster_conf.lua");
		strcpy(m_MapList[i]->m_szCtrlFile, m_MapList[i]->GetName());		//
		strcat(m_MapList[i]->m_szCtrlFile, "\\");
		strcat(m_MapList[i]->m_szCtrlFile, "ctrl.lua");
		strcpy(m_MapList[i]->m_szEntryFile, m_MapList[i]->GetName());		//
		strcat(m_MapList[i]->m_szEntryFile, "\\");
		strcat(m_MapList[i]->m_szEntryFile, "entry.lua");

		if(m_MapList[i]->Init()==FALSE)
		{
			//LG("init", "[%s]!\n", m_MapList[i]->GetName());
			LG("init", "map [%s] initialization failed!\n", m_MapList[i]->GetName());
			g_Config.m_btMapOK[i] = 0;
			continue;
		}
		else
		{
			g_Config.m_btMapOK[i] = 1; // 
		}
		m_strMapNameList += m_MapList[i]->GetName();
		m_strMapNameList += ";";
		//LG("init", "[%s] ok!\n", m_MapList[i]->GetName());
		LG("init", "map [%s] ok!\n", m_MapList[i]->GetName());
	}
	
	
	luaL_dofile(g_pLuaState, GetResPath("script/help/AddHelpNPC.lua"));
	
	luaL_dofile(g_pLuaState, GetResPath("script/monster/mlist.lua"));
	
	return TRUE;

T_E}

BOOL CGameApp::SummonNpc( BYTE byMapID, USHORT sAreaID, const char szNpc[], USHORT sTime )
{T_B
	CMapRes* pMap = g_MapID.GetMap( byMapID );
	if( pMap )
	{
		return pMap->SummonNpc( sAreaID, szNpc, sTime );
	}

	return FALSE;
T_E}

// 
CMapRes *CGameApp::FindMapByName(const char *mapname, bool bIncUnRun)
{T_B
	if (!mapname)
		return 0;

	short i = 0;
	for(;i < m_mapnum;i++)
	{
		CMapRes *pMap = m_MapList[i];
		if(!pMap) continue; // 
	    //BUG
		/* if (!bIncUnRun)
			if (!(pMap->IsOpen())) continue;
		*/
		if(!strcmp(pMap->GetName(), mapname))
		{
			break;
		}
	}
	if(i < m_mapnum)
	{
		return m_MapList[i];
	}
	else
	{
		return 0;
	}
T_E}

void CGameApp::LoadAllTable()
{T_B
	LoadCharacterInfo();
	LoadSkillInfo();
	LoadItemInfo();
T_E}

void CGameApp::LoadCharacterInfo()
{T_B
	m_CChaRecordSet->Release();
	m_CChaRecordSet = new CChaRecordSet(0, defMAX_CHARINFO_NO);
	LoadTable(m_CChaRecordSet, szChaInfoName);
T_E}

void CGameApp::LoadSkillInfo()
{T_B
	m_CSkillRecordSet->Release();
	m_CSkillRecordSet = new CSkillRecordSet(0, defMAX_SKILL_NO);
	LoadTable(m_CSkillRecordSet, szSkillInfoName);
T_E}

void CGameApp::LoadItemInfo()
{T_B
	m_CItemRecordSet->Release();
	m_CItemRecordSet = new CItemRecordSet(0, CItemRecord::enumItemMax);
	LoadTable(m_CItemRecordSet, szItemInfoName);
T_E}

BOOL CGameApp::ReloadNpcInfo( CCharacter& character )
{T_B
	g_pGameApp->BeginGetTNpc();
	mission::CTalkNpc* pTalkNpc = NULL;
	while( (pTalkNpc = g_pGameApp->GetNextTNpc()) )
	{
		if( !pTalkNpc->InitScript( pTalkNpc->GetInitFunc(), pTalkNpc->GetName() ) )
		{
			//character.SystemNotice( "NPC[%s][%s]!", pTalkNpc->GetInitFunc(), pTalkNpc->GetName() );
			character.SystemNotice( RES_STRING(GM_GAMEAPP_CPP_00001), pTalkNpc->GetInitFunc(), pTalkNpc->GetName() );
		}
	}

	//mission::g_WorldEudemon.Load( "Eudemon", "", -1 );
	mission::g_WorldEudemon.Load( "Eudemon", "Eudemon", -1 );
	return TRUE;
T_E}

mission::CNpc* CGameApp::FindNpc( const char szName[] )
{
	if( szName )
	{
		for(short i=0; i< m_mapnum; i++)
		{
			mission::CNpc* pNpc = m_MapList[i]->FindNpc( szName );
			if( pNpc ) return pNpc;
		}
	}
	return NULL;
}

void CGameApp::NotiGameReset(unsigned long ulLeftSec)
{T_B
	char	szNotiMsg[1024];

	//sprintf(szNotiMsg, "\"%s\" %u \"%s\"", g_Config.m_szName, ulLeftSec, m_strMapNameList.c_str());
	sprintf(szNotiMsg, RES_STRING(GM_GAMEAPP_CPP_00002), g_Config.m_szName, ulLeftSec, m_strMapNameList.c_str());

	WPACKET wpk	= GETWPACKET();
	WRITE_CMD(wpk, CMD_MC_SYSINFO);
	WRITE_STRING(wpk, szNotiMsg);
	NotiPkToWorld(wpk);
T_E}

void CGameApp::BeginGetTNpc(void) 
{T_B
	m_pCEntSpace->BeginGetTNpc();
T_E}

mission::CTalkNpc*	CGameApp::GetNextTNpc(void)
{T_B
	return m_pCEntSpace->GetNextTNpc();
T_E}

void CGameApp::SaveAllPlayer(void)
{T_B
	BEGINGETGATE();
	CPlayer	*pCPlayer;
	GateServer	*pGateServer;
	LG("enter_map", "Begin SaveAllPlayer==============================================================\n");
	while (pGateServer = GETNEXTGATE())
	{
		if (!BEGINGETPLAYER(pGateServer))
			continue;
		int nCount = 0;
		while (pCPlayer = (CPlayer *)GETNEXTPLAYER(pGateServer))
		{
			if (++nCount > GETPLAYERCOUNT(pGateServer))
			{
				//LG("", ":%u, %s\n", GETPLAYERCOUNT(pGateServer), "SaveAllPlayer");
				LG("player chain list error", "player number:%u, %s\n", GETPLAYERCOUNT(pGateServer), "SaveAllPlayer");
				break;
			}
			LG("enter_map", "SaveAllPlayer\n");
			game_db.SavePlayer(pCPlayer, enumSAVE_TYPE_OFFLINE);
			LG("enter_map", "\n");
		}
	}
	game_db.GetMapMaskTable()->SaveAll(); // , 
	LG("enter_map", "End SaveAllPlayer################################################################\n");
T_E}

void CGameApp::DataStatistic(void)
{
	Long	lCabinHeapNum = m_CabinHeap.GetUsedNum();
	Long	lTradeDataHeapNum = m_TradeDataHeap.GetUsedNum();
	Long	lSkillTDataHeapNum = m_SkillTDataHeap.GetUsedNum();
	Long	lMapMgrUnitHeapNum = m_MapStateCellHeap.GetUsedNum();
	Long	lEntityListHeapNum = m_ChaListHeap.GetUsedNum();
	Long	lMgrNodeHeapNum = m_StateCellNodeHeap.GetUsedNum();

	if (lCabinHeapNum > m_lCabinHeapNum || lTradeDataHeapNum > m_lTradeDataHeapNum || lSkillTDataHeapNum > m_lSkillTDataHeapNum
		|| lMapMgrUnitHeapNum > m_lMapMgrUnitHeapNum || lEntityListHeapNum > m_lEntityListHeapNum || lMgrNodeHeapNum > m_lMgrNodeHeapNum)
	{
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

		LG("", "Cabin=%4u,\tTrade=%8u,\tSkillTemp=%8u,\tMapMgrUnit=%8u,\tEntityList=%8u,\tMgrNode=%8u\n",
			m_lCabinHeapNum, m_lTradeDataHeapNum, m_lSkillTDataHeapNum, m_lMapMgrUnitHeapNum, m_lEntityListHeapNum, m_lMgrNodeHeapNum);
	}
}

// GameServer
void CGameApp::WorldNotice(const char *szString)
{
	if (!szString)
		return;

	LG("WorldNotice", "WorldNotice: len = %d\n", strlen(szString));
	LG("WorldNotice", "WorldNotice: contend = %s\n", szString);

	WPACKET WtPk  = GETWPACKET();
	WRITE_CMD(WtPk, CMD_MM_NOTICE);
	WRITE_LONG(WtPk, 0);
	WRITE_STRING(WtPk, szString);

	BEGINGETGATE();
	GateServer	*pGateServer;
	while (pGateServer = GETNEXTGATE())
	{
		WRITE_LONG(WtPk, 0);
		WRITE_LONG(WtPk, 0);
		WRITE_SHORT(WtPk, 1);
		pGateServer->SendData(WtPk);
		break;
	}
}

void CGameApp::GuildNotice(unsigned long guildID, const char *szString)
{
	if (!szString)
		return;

	WPACKET WtPk = GETWPACKET();
	WRITE_CMD(WtPk, CMD_MP_GUILDNOTICE);
	WRITE_LONG(WtPk, guildID);
	WRITE_STRING(WtPk, szString);
	SENDTOGROUP(WtPk);
}

//add by sunny.sun20080804
void CGameApp::ScrollNotice(const char * szString,int SetNum, DWORD color)
{
	if(!szString)
		return;
	WPACKET WtPk  = GETWPACKET();
	WRITE_CMD(WtPk, CMD_MP_GM1SAY1);
	WRITE_STRING(WtPk, szString);
	WRITE_LONG(WtPk, SetNum);
	WRITE_LONG(WtPk, color);
	SENDTOGROUP(WtPk);
}
//add by sunny.sun20080821
void CGameApp::GMNotice( const char * szString )
{
	if(!szString)
		return;
	WPACKET WtPk  = GETWPACKET();
	WRITE_CMD(WtPk, CMD_MP_GM1SAY);
	WRITE_STRING(WtPk, szString);
	SENDTOGROUP(WtPk);
}

// GameServer
void CGameApp::LocalNotice(const char *szString)
{
	if (!szString)
		return;

	WPACKET wpk	= GETWPACKET();
	WRITE_CMD(wpk, CMD_MC_SYSINFO);
	WRITE_STRING(wpk, szString);
	SENDTOWORLD(wpk);
}

// szChaName == ""GameServer
void CGameApp::ChaNotice(const char *szNotiString, const char *szChaName)
{
	if (!szNotiString || !szChaName)
		return;

	WPACKET WtPk  = GETWPACKET();
	WRITE_CMD(WtPk, CMD_MM_CHA_NOTICE);
	WRITE_LONG(WtPk, 0);
	WRITE_STRING(WtPk, szNotiString);
	WRITE_STRING(WtPk, szChaName);

	BEGINGETGATE();
	GateServer	*pGateServer = NULL;
	while (pGateServer = GETNEXTGATE())
	{
		WRITE_LONG(WtPk, 0);
		WRITE_LONG(WtPk, 0);
		WRITE_SHORT(WtPk, 1);
		pGateServer->SendData(WtPk);
		break;
	}
}

void CGameApp::BanAccount(const char* szString)
{
	if (!szString)
		return;
	WPACKET WtPk  = GETWPACKET();
	WRITE_CMD(WtPk, CMD_MP_GMBANACCOUNT);
	WRITE_STRING(WtPk, szString);
	SENDTOGROUP(WtPk);
}

void CGameApp::UnbanAccount(const char* szString)
{
	if (!szString)
		return;
	WPACKET WtPk  = GETWPACKET();
	WRITE_CMD(WtPk, CMD_MP_GMUNBANACCOUNT);
	WRITE_STRING(WtPk, szString);
	SENDTOGROUP(WtPk);
}

void CGameApp::CanReceiveRequests(uLong chaID, bool CanSend) {
	WPACKET WtPk = GETWPACKET();
	WRITE_CMD(WtPk, CMD_MP_CANRECEIVEREQUESTS);
	WRITE_LONG(WtPk, chaID);
	WRITE_SHORT(WtPk, CanSend);
	SENDTOGROUP(WtPk);
}
