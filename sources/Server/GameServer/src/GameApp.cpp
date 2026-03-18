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
SubMap*		g_pScriptMap = NULL;		// 脚本用初始化地图信息全局变量
long		g_lDeftMMaskLight = 21;
string		g_strChaState[2];


// 游戏循环中当前正在轮到执行的cha
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
char		szHairInfoName[64]			= "Hairs";		// 可更换的发型记录表

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
		//LG("exception3", "未知的实体（ID:%u, Handle:%d）发生了异常\n", ulCurID, lCurHandle);
		LG("exception3", "unknown entity(ID:%u, Handle:%d)occur abnormity\n", ulCurID, lCurHandle);
		return;
	}
	CCharacter *pCurCha = pCEnti->IsCharacter();
	if (!pCurCha)
	{
		//LG("exception3", "未知的角色（ID:%u, Handle:%d）发生了异常\n", ulCurID, lCurHandle);
		LG("exception3", "unknown character(ID:%u, Handle:%d)occur abnormity\n", ulCurID, lCurHandle);
		return;
	}

	try
	{
		//LG("exception3", "角色[%s] [%s]发生异常, 将被踢出游戏\n", pCurCha->GetLogName(), pCurCha->GetPlyMainCha()->GetLogName());
		LG("exception3", "character[%s] [%s]occur abnormity, will be kick out game\n", pCurCha->GetLogName(), pCurCha->GetPlyMainCha()->GetLogName());
		// ....
		CPlayer	*pCPlayer = pCurCha->GetPlayer();
		if (pCPlayer)
		{
			//LG("exception3", "玩家角色[%s] [%s]发生异常, [GoOutGame]\n", pCurCha->GetLogName(), pCurCha->GetPlyMainCha()->GetLogName());
			LG("exception3", "player character[%s] [%s]occur abnormity, [GoOutGame]\n", pCurCha->GetLogName(), pCurCha->GetPlyMainCha()->GetLogName());
			KICKPLAYER(pCPlayer, 0);
			LG("exception3", "End [KICKPLAYER], Begin[GoOutGame]\n");
			g_pGameApp->GoOutGame(pCPlayer, true);
			LG("exception3", "End [GoOutGame]\n");
			return;
		}
		else
		{
			//LG("exception3", "释放怪物角色[%s], Begin\n", pCurCha->GetName());
			LG("exception3", "release bugbear character[%s], Begin\n", pCurCha->GetName());
			pCurCha->Free();
			//LG("exception3", "End 释放怪物角色\n");
			LG("exception3", "End release bugbear character\n");
		}
	}
	catch (...)
	{
		//LG("exception3", "当角色循环发生异常时, 踢除角色的过程中又发生异常, [%s]\n", pCurCha->GetName());
		LG("exception3", "when character loop occur abnormity, the process kick character occur abnormity again, [%s]\n", pCurCha->GetName());
	}
}

// 游戏逻辑主线程
DWORD WINAPI g_GameLogicProcess(LPVOID lpParameter)
{
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

    while (!g_bGameEnd)
	{
		
		DWORD	dwInterval = 50; // 毫秒

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

		//状态遍历
		dwLastTick = GetTickCount();
		g_pGameApp->Run(dwLastTick);
		dwCurTick = GetTickCount();
		dwRunTick = dwCurTick - dwLastTick;

		//服务器间消息处理
		dwLastTick = dwCurTick;
		PEEKPACKET(50 > dwRunTick ? 50 - dwRunTick : 0);
		dwCurTick = GetTickCount();
		dwRunTick += dwCurTick - dwLastTick;

		//InfoServer消息处理
		dwLastTick = dwCurTick;
        g_gmsvr->GetInfoServer()->PeekMsg(50 > dwRunTick ? 50 - dwRunTick : 0);
		g_StoreSystem.Run(dwCurTick, 10000, 10000);
		dwCurTick = GetTickCount();
		dwRunTick += dwCurTick - dwLastTick;
		    
		g_pGameApp->m_dwRunStep = 104;

	}
	//LG("init", "游戏线程结束!\n");
	LG("init", "game thread finish!\n");
	return 0;
}

// Add by lark.li 20080324 begin
void CDBLogMgr::TradeLog(const char* action, const char *pszChaFrom, const char *pszChaTo, const char *pszTrade)
{
	if( g_Config.m_bTradeLogIsConfig )
	{
		tradeLog_db.ExecLogSQL(g_Config.m_szName, action, pszChaFrom, pszChaTo, pszTrade);
	}
	else
	{
		game_db.ExecTradeLogSQL(g_Config.m_szName, action, pszChaFrom, pszChaTo, pszTrade);
	}
}

// End

// 可以Log 5个字符串字段, 最后一个长度为8000字符以内
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


// 被定时调用, 每次写入一定数量的log到DB
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
	// 每次只执行固定数量
	for(int i = 0; i < _nPerLogCnt; i++)
	{
		if(_LogList.empty()) break;
		SDBLogData *pData = _LogList.front();
		game_db.ExecLogSQL(pData->szLog);
		_LogList.pop_front();

		// 如果被执行的位置已经大于pool正被使用的位置
		if(pData->nLoc > _nPoolUseLoc)
		{
			if(pData->nLoc - _nPoolUseLoc < 10) // 两个位置已经很接近, 快要重叠了, 报警
			{
				bFlush = TRUE;
				//LG("dblog_error", "DBLogPool位置即将重叠HandleLoc=[%d], PoolUseLoc=[%d]处理DBLog速度不正常, 执行Flush所有剩余log!\n", pData->nLoc, _nPoolUseLoc);
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

// GameServer关闭的时候, 保证剩下的log都写入DB
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
{
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
}


CGameApp::~CGameApp()
{
	
	//Log("关闭", "haha", "", "" ,"", "");
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
}


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
		//LG("error", "msg读取表格[%s]失败\n", str.c_str());
		LG("error", RES_STRING(GM_GAMEAPP_CPP_00003), str.c_str());
		return FALSE;
	}
	return TRUE;
}


BOOL CGameApp::Init()
{
	//LG("init", "开始初始化GameApp\n");
	LG("init", "start initialization GameApp\n");

	//LG("init", "初始化数据库...\n");
	LG("init", "initialization DB...\n");
	if(game_db.Init())
	{	// 初始化数据库
		LG("init", "database init...ok\n");
	}
	else
	{
		LG("init", "database init...Fail!\n");
		return FALSE;
	}

	// Add by lark.li 20080324 begin
	if(tradeLog_db.Init())
	{	// 初始化数据库
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
		//LG( "init", "msg初始化装载文字过滤信息文件失败!\n" );
		LG( "init", RES_STRING(GM_GAMEAPP_CPP_00004) );
		return FALSE;		
	}

	//m_Ident.m_maxID = g_Config.m_ulBaseID; //获得ID分配器的基数
	m_Ident.m_maxID = 0xafffffff; //获得ID分配器的基数
	if(!m_Ident.m_maxID)
		//LG("init", "错误的ID基数!!!\n");
		LG("init", "error ID base!!!\n");
	m_ItemIdent.m_maxID = 1;

	m_pCPlySpace = new CPlayerAlloc(g_Config.m_nMaxPly);
	if (!m_pCPlySpace)
	{
		//LG("init", "msg分配玩家空间出错!, 退出!\n");
		LG("init", RES_STRING(GM_GAMEAPP_CPP_00005));
		return FALSE;
	}
	m_pCEntSpace = new CEntityAlloc(g_Config.m_nMaxCha + g_Config.m_nMaxPly * 3, g_Config.m_nMaxItem, g_Config.m_nMaxTNpc);
	if (!m_pCEntSpace)
	{
		//LG("init", "msg分配角色空间出错!, 退出!\n");
		LG("init", RES_STRING(GM_GAMEAPP_CPP_00017));
		return FALSE;
	}
	g_pCSystemCha = GetNewCharacter();
	g_pCSystemCha->SetID(m_Ident.GetID());
	//g_pCSystemCha->SetName("系统");
	g_pCSystemCha->SetName(RES_STRING(GM_GAMEAPP_CPP_00018));
	g_pCSystemCha->setAttr(ATTR_CHATYPE, enumCHACTRL_NONE, 1);

    //LG("init", "开始分配各项Entity内存\n");
	LG("init", "start to assign every Entity memory\n");
	m_CabinHeap.Init();
	m_TradeDataHeap.Init();
	m_MapStateCellHeap.Init();
	m_ChaListHeap.Init();
	m_SkillTDataHeap.Init();
	m_StateCellNodeHeap.Init();

	//LG("init", "初始化各项表格...\n");
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

	//防外挂暂时不上
	/*m_PicSet = new CPicSet();
	if(!m_PicSet->init())
	{
		LG( "init", "msg读取图片失败！" );
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
		//LG( "init", "msg读取精练数据表信息失败！" );
		LG( "init", RES_STRING(GM_GAMEAPP_CPP_00006) );
		return FALSE;
	}

	if( !g_CharBoat.Load( "ShipInfo", "ShipItemInfo"))
	{
		//LG( "init", "msg读取船只数据表信息失败！" );
		LG( "init", RES_STRING(GM_GAMEAPP_CPP_00007) );
		return FALSE;
	}

	printf("Loading %s\n", LUAJIT_VERSION);
	printf("Loading Serverfile 1.38\n");
	//LG("init", "初始化Lua Script...\n");
	LG("init", "initialization Lua Script...\n");
	InitLuaScript();

	if(InitMap()==FALSE)
	{
		//LG("init", "地图初始化失败!, 退出!\n");
		LG("init", "initialization map failed!, exit!\n");
		return FALSE;
	}

	InitSStateTraOnTime();

	if (!IsChaAttrMaxValInit())
	{
		//LG("init", "角色属性最大值...Fail!\n");
		LG("init", "character attribute max...Fail!\n");
		return FALSE;
	}

	//LG("init", "GameApp初始化结束!\n");
	LG("init", "GameApp initialization finish!\n");

	LG("size", "sizeof(Entity) = %d,  sizeof(Character) = %d\n", sizeof(Entity), sizeof(CCharacter));
	
	m_CTimerItem.Begin(3 * 1000);

	return TRUE;
}
    

void CGameApp::Run(DWORD dwCurTime)
{
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
	// 数据统计
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

	// 游戏循环速率和总时间统计
    if(dwCurTime - _dwLastTick >= 1000)
    {
        m_dwFPS       = _dwTempRunCnt;
        _dwTempRunCnt = 0;
        _dwLastTick   = dwCurTime;
    }
    m_dwRunCnt++;     // 循环总次数, 可以用作时间戳
    _dwTempRunCnt++;  // 帧计数
    m_dwRunStep = 104;

	if (m_ulLeftSec > 0 && m_CTimerReset.IsOK(dwCurTime))
	{
		NotiGameReset(m_ulLeftSec);
	    m_dwRunStep = 501;
		m_ulLeftSec--;
		if (m_ulLeftSec == 0) // 保存角色，结束游戏，此后的代码不应该再引用角色
		{
			SaveAllPlayer();
		    m_dwRunStep = 502;
			g_bGameEnd = true;
			return;
		}
	}
    m_dwRunStep = 503;

	// 纪录运行时间
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
}

void CGameApp::MgrUnitRun(DWORD dwCurTime)
{
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
			g_pScriptMap = pCSubMap; // 那么随后的AI循环中, 就不必指定当前地图

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
					//LG("状态单元错误", "实际激活数 %d\n", lTarNum);
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
					/*LG("map_time", "!!!%s(%d)消耗 = %d ms, WeatherRun = %u, CharacterRun = %u, StateRun = %u, ActiveMgrUnitNum = %d\n",
						m_MapList[i]->GetName(), sMapCpyNum, dwMMgrRunTime, dwPartRunTime[0], dwPartRunTime[1], dwPartRunTime[2], dwActMgrCellNum);*/
						LG("map_time", "!!!%s(%d) expend = %d ms, WeatherRun = %u, CharacterRun = %u, StateRun = %u, ActiveMgrUnitNum = %d\n",
						m_MapList[i]->GetName(), sMapCpyNum, dwMMgrRunTime, dwPartRunTime[0], dwPartRunTime[1], dwPartRunTime[2], dwActMgrCellNum);
				else
					/*LG("map_time", "...%s(%d)消耗 = %d ms, WeatherRun = %u, CharacterRun = %u, StateRun = %u, ActiveMgrUnitNum = %d\n",
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
}

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
{
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
}

// 分配一个新的玩家
CPlayer* CGameApp::GetNewPlayer()
{
	return m_pCPlySpace->GetNewPly();
}

// 根据玩家句柄取指针
CPlayer* CGameApp::GetPlayer(long lHandle)
{
	return m_pCPlySpace->GetPly(lHandle);
}

CPlayer* CGameApp::IsValidPlayer(long lID, long lHandle)
{
	CPlayer	*pCPly = m_pCPlySpace->GetPly(lHandle);
	if (!pCPly)
		return 0;
	if (pCPly->GetID() != lID)
		return 0;

	return pCPly;
}

// 创建玩家数据结构，读取数据库
// chType：0，角色上线。1，地图切换
CPlayer* CGameApp::CreateGamePlayer(const char szPassword[], uLong ulChaDBId, uLong ulWorldId, const char *cszMapName, char chType)
{
	CPlayer	*pCOldPly = GetPlayerByDBID(ulChaDBId);
	if (pCOldPly)
	{
		//LG("重复角色", "角色 %s[%u] 进入时，发现该角色已经存在！\n", pCOldPly->GetMainCha()->GetName(), ulChaDBId);
		LG("repeat character", "when character %s[%u] enter，find it has exist！\n", pCOldPly->GetMainCha()->GetName(), ulChaDBId);
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

        // 缺少character的free
       // LG("enter_map", "建立新玩家角色（ID = %u）时，分配内存失败 \n", ulChaDBId);
		LG("enter_map", "when create new player character（ID = %u），memory assign failed \n", ulChaDBId);
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
    if (!game_db.ReadPlayer(l_player, ulChaDBId)) // 读数据异常，切断与Gate的连接
    {
       // LG("enter_map", "atorID = %d, 在取角色资料时，出现错误，导致GameServer与此GateServer断开连接\n", ulChaDBId);
		 LG("enter_map", "atorID = %d,when get character data，appear error，lead to GameServer with GateServer disconnection\n", ulChaDBId);
        l_player->Free();
        return NULL;
    }

	// 建造玩家角色携带的船只
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
}

// 保存玩家资料，归还内存
void CGameApp::ReleaseGamePlayer(CPlayer* pPlayer)
{
    // 资料写数据库，释放角色
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
		if (bIsDie || !bSavePos) // 角色已经死亡
		{
			if (bIsDie)
				g_CParser.DoString("Relive", enumSCRIPT_RETURN_NONE, 0, enumSCRIPT_PARAM_LIGHTUSERDATA, 1, pCCha, DOSTRING_PARAM_END);

			SSrcPos = pCCha->GetPos();
			if (pCCha->IsBoat()) // 船只
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
			//LG("enter_map", "SavePlayer[%s]失败", pPlayer->GetMainCha()->GetName());
			LG("enter_map", "SavePlayer[%s] failed", pPlayer->GetMainCha()->GetName());
		}

		if (bIsDie || !bSavePos) // 角色已经死亡
		{
			game_db.SavePlayerPos(pPlayer);
			pCCha->SetSubMap(pSrcMap);
			pCCha->SetPos(SSrcPos);
		}

		DelPlayerIdx(pPlayer->GetDBChaId());
		g_pGameApp->m_dwPlayerCnt--;
		pPlayer->Free();

		//////////////////////////////////////////////////////////////////////////
		// 删除gate server对应的维护信息
		pPlayer->OnLogoff();
		DELPLAYER(pPlayer);
		//////////////////////////////////////////////////////////////////////////

		LG("enter_map", "atorID = %d, goout\n", pPlayer->GetDBChaId());
	}
}

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

// 分配一个新的角色
CCharacter* CGameApp::GetNewCharacter()
{
	return m_pCEntSpace->GetNewCha();
}

// 分配一个新的道具
CItem* CGameApp::GetNewItem()
{
	return m_pCEntSpace->GetNewItem();
}

// 分配一个新的NPC
mission::CTalkNpc* CGameApp::GetNewTNpc()
{
	return m_pCEntSpace->GetNewTNpc();
}

Entity* CGameApp::GetEntity(long lHandle)
{
	return m_pCEntSpace->GetEntity(lHandle);
}

// 角色是有效实体
Entity* CGameApp::IsValidEntity(unsigned long ulID, long lHandle)
{
	Entity	*pEnti = g_pGameApp->GetEntity(lHandle);
	if (!pEnti)
		return 0;
	if (!pEnti->IsValid()
		|| ulID != pEnti->GetID())
		return 0;

	return pEnti;
}

// 角色是有效实体，存在于地图且在生存状态
Entity* CGameApp::IsLiveingEntity(unsigned long ulID, long lHandle)
{
	Entity	*pEnti = IsValidEntity(ulID, lHandle);
	if (!pEnti)
		return 0;
	if (pEnti->IsCharacter() != g_pCSystemCha)
		if (!pEnti->IsLiveing() || !pEnti->GetSubMap())
			return 0;

	return pEnti;
}

// 角色是有效实体，存在于地图
Entity* CGameApp::IsMapEntity(unsigned long ulID, long lHandle)
{
	Entity	*pEnti = IsValidEntity(ulID, lHandle);
	if (!pEnti)
		return 0;
	if (pEnti->IsCharacter() != g_pCSystemCha)
		if (!pEnti->GetSubMap())
			return 0;

	return pEnti;
}

// 角色是有效实体，有生命
Entity* CGameApp::IsLifeEntity(unsigned long ulID, long lHandle)
{
	Entity	*pEnti = IsValidEntity(ulID, lHandle);
	if (!pEnti)
		return 0;
	if (pEnti->IsCharacter() != g_pCSystemCha)
		if (!pEnti->IsLiveing())
			return 0;

	return pEnti;
}

// 初始化所有地图
BOOL CGameApp::InitMap()
{
	//LG("init", "初始化地图列表...\n");
	LG("init", "initialization map list...\n");
	
	mission::g_WorldEudemon.Load( "Eudemon", g_Config.m_szEqument, -1 );

	//初始化Map信息
	m_mapnum = g_Config.m_nMapCnt;
	if(m_mapnum < 1)
	{
		//LG("init", "配置地图数量为0,没有地图被初始化, 退出!\n");
		LG("init", "collocate map number 0,no map was initialization, exit!\n");
		return FALSE;
	}

	m_strMapNameList = "";

	//初始化地图数据结构
	memset(m_MapList, 0, sizeof(CMapRes*) * MAX_MAP);
	for(short i=0; i< m_mapnum; i++)
	{
		m_MapList[i] = new CMapRes();
		m_MapList[i]->SetName(g_Config.m_szMapList[i]);

		strcpy(m_MapList[i]->m_szObstacleFile, m_MapList[i]->GetName());		//障碍文件名
		strcat(m_MapList[i]->m_szObstacleFile, "\\"); 
		strcat(m_MapList[i]->m_szObstacleFile, m_MapList[i]->GetName());
		strcat(m_MapList[i]->m_szObstacleFile, ".blk");
		strcpy(m_MapList[i]->m_szSectionFile, m_MapList[i]->GetName());			//区域属性文件名
		strcat(m_MapList[i]->m_szSectionFile, "\\");
		strcat(m_MapList[i]->m_szSectionFile, m_MapList[i]->GetName());
		strcpy(m_MapList[i]->m_szMonsterSpawnFile, m_MapList[i]->GetName());	//怪物刷新文件名
		strcat(m_MapList[i]->m_szMonsterSpawnFile, "\\");
		strcat(m_MapList[i]->m_szMonsterSpawnFile, m_MapList[i]->GetName());
		strcat(m_MapList[i]->m_szMonsterSpawnFile, "ChaSpn");
		strcpy(m_MapList[i]->m_szNpcSpawnFile, m_MapList[i]->GetName());		//npc产生文件名
		strcat(m_MapList[i]->m_szNpcSpawnFile, "\\");
		strcat(m_MapList[i]->m_szNpcSpawnFile, m_MapList[i]->GetName());
		strcat(m_MapList[i]->m_szNpcSpawnFile, "NPC");
		strcpy(m_MapList[i]->m_szMapSwitchFile, m_MapList[i]->GetName());		//地图切换文件名
		strcat(m_MapList[i]->m_szMapSwitchFile, "\\");
		strcat(m_MapList[i]->m_szMapSwitchFile, m_MapList[i]->GetName());
		strcat(m_MapList[i]->m_szMapSwitchFile, "SwhMap");
		strcpy(m_MapList[i]->m_szMonsterCofFile, m_MapList[i]->GetName());		//单独的怪物刷新文件名
		strcat(m_MapList[i]->m_szMonsterCofFile, "\\");
		strcat(m_MapList[i]->m_szMonsterCofFile, m_MapList[i]->GetName());
		strcat(m_MapList[i]->m_szMonsterCofFile, "monster_conf.lua");
		strcpy(m_MapList[i]->m_szCtrlFile, m_MapList[i]->GetName());		//地图控制文件名
		strcat(m_MapList[i]->m_szCtrlFile, "\\");
		strcat(m_MapList[i]->m_szCtrlFile, "ctrl.lua");
		strcpy(m_MapList[i]->m_szEntryFile, m_MapList[i]->GetName());		//地图入口文件名
		strcat(m_MapList[i]->m_szEntryFile, "\\");
		strcat(m_MapList[i]->m_szEntryFile, "entry.lua");

		if(m_MapList[i]->Init()==FALSE)
		{
			//LG("init", "地图[%s]初始化失败!\n", m_MapList[i]->GetName());
			LG("init", "map [%s] initialization failed!\n", m_MapList[i]->GetName());
			g_Config.m_btMapOK[i] = 0;
			continue;
		}
		else
		{
			g_Config.m_btMapOK[i] = 1; // 初始化成功标记
		}
		m_strMapNameList += m_MapList[i]->GetName();
		m_strMapNameList += ";";
		//LG("init", "地图[%s] ok!\n", m_MapList[i]->GetName());
		LG("init", "map [%s] ok!\n", m_MapList[i]->GetName());
	}
	
	
	luaL_dofile(g_pLuaState, GetResPath("script/help/AddHelpNPC.lua"));
	
	luaL_dofile(g_pLuaState, GetResPath("script/monster/mlist.lua"));
	
	return TRUE;

}

BOOL CGameApp::SummonNpc( BYTE byMapID, USHORT sAreaID, const char szNpc[], USHORT sTime )
{
	CMapRes* pMap = g_MapID.GetMap( byMapID );
	if( pMap )
	{
		return pMap->SummonNpc( sAreaID, szNpc, sTime );
	}

	return FALSE;
}

// 通过名称查找子地图
CMapRes *CGameApp::FindMapByName(const char *mapname, bool bIncUnRun)
{
	if (!mapname)
		return 0;

	short i = 0;
	for(;i < m_mapnum;i++)
	{
		CMapRes *pMap = m_MapList[i];
		if(!pMap) continue; // 有可能还没有初始化
	    //修改小地图隔一段时间无法进入的BUG
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
}

void CGameApp::LoadAllTable()
{
	LoadCharacterInfo();
	LoadSkillInfo();
	LoadItemInfo();
}

void CGameApp::LoadCharacterInfo()
{
	m_CChaRecordSet->Release();
	m_CChaRecordSet = new CChaRecordSet(0, defMAX_CHARINFO_NO);
	LoadTable(m_CChaRecordSet, szChaInfoName);
}

void CGameApp::LoadSkillInfo()
{
	m_CSkillRecordSet->Release();
	m_CSkillRecordSet = new CSkillRecordSet(0, defMAX_SKILL_NO);
	LoadTable(m_CSkillRecordSet, szSkillInfoName);
}

void CGameApp::LoadItemInfo()
{
	m_CItemRecordSet->Release();
	m_CItemRecordSet = new CItemRecordSet(0, CItemRecord::enumItemMax);
	LoadTable(m_CItemRecordSet, szItemInfoName);
}

BOOL CGameApp::ReloadNpcInfo( CCharacter& character )
{
	g_pGameApp->BeginGetTNpc();
	mission::CTalkNpc* pTalkNpc = NULL;
	while( (pTalkNpc = g_pGameApp->GetNextTNpc()) )
	{
		if( !pTalkNpc->InitScript( pTalkNpc->GetInitFunc(), pTalkNpc->GetName() ) )
		{
			//character.SystemNotice( "重新装载NPC[%s]初始化信息脚本函数[%s]失败!", pTalkNpc->GetInitFunc(), pTalkNpc->GetName() );
			character.SystemNotice( RES_STRING(GM_GAMEAPP_CPP_00001), pTalkNpc->GetInitFunc(), pTalkNpc->GetName() );
		}
	}

	//mission::g_WorldEudemon.Load( "Eudemon", "守护神", -1 );
	mission::g_WorldEudemon.Load( "Eudemon", "Eudemon", -1 );
	return TRUE;
}

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
{
	char	szNotiMsg[1024];

	//sprintf(szNotiMsg, "游戏服务器\"%s\"将在 %u 秒后重启，请在地图\"%s\"上的玩家做好准备", g_Config.m_szName, ulLeftSec, m_strMapNameList.c_str());
	sprintf(szNotiMsg, RES_STRING(GM_GAMEAPP_CPP_00002), g_Config.m_szName, ulLeftSec, m_strMapNameList.c_str());

	WPACKET wpk	= GETWPACKET();
	WRITE_CMD(wpk, CMD_MC_SYSINFO);
	WRITE_STRING(wpk, szNotiMsg);
	NotiPkToWorld(wpk);
}

void CGameApp::BeginGetTNpc(void) 
{
	m_pCEntSpace->BeginGetTNpc();
}

mission::CTalkNpc*	CGameApp::GetNextTNpc(void)
{
	return m_pCEntSpace->GetNextTNpc();
}

void CGameApp::SaveAllPlayer(void)
{
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
				//LG("玩家链表错误", "玩家数目:%u, %s\n", GETPLAYERCOUNT(pGateServer), "SaveAllPlayer");
				LG("player chain list error", "player number:%u, %s\n", GETPLAYERCOUNT(pGateServer), "SaveAllPlayer");
				break;
			}
			LG("enter_map", "SaveAllPlayer\n");
			game_db.SavePlayer(pCPlayer, enumSAVE_TYPE_OFFLINE);
			LG("enter_map", "\n");
		}
	}
	game_db.GetMapMaskTable()->SaveAll(); // 退出时, 一次性处理剩下的保存大地图请求
	LG("enter_map", "End SaveAllPlayer################################################################\n");
}

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

		LG("栈使用统计", "Cabin=%4u,\tTrade=%8u,\tSkillTemp=%8u,\tMapMgrUnit=%8u,\tEntityList=%8u,\tMgrNode=%8u\n",
			m_lCabinHeapNum, m_lTradeDataHeapNum, m_lSkillTDataHeapNum, m_lMapMgrUnitHeapNum, m_lEntityListHeapNum, m_lMgrNodeHeapNum);
	}
}

// 通知本组所有GameServer上的玩家
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

// 通知本GameServer上的玩家
void CGameApp::LocalNotice(const char *szString)
{
	if (!szString)
		return;

	WPACKET wpk	= GETWPACKET();
	WRITE_CMD(wpk, CMD_MC_SYSINFO);
	WRITE_STRING(wpk, szString);
	SENDTOWORLD(wpk);
}

// 通知指定玩家，如果szChaName == ""，则通告本组GameServer的所有玩家
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