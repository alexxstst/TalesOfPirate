//=============================================================================
// FileName: MapRes.cpp
// Creater: ZhangXuedong
// Date: 2005.09.05
// Comment: Map Resource
//=============================================================================
#include "stdafx.h"   //add by sunny 20080312

#include "MapRes.h"
#include "Script.h"
#include "SubMap.h"
#include "LuaAPI.h"

using namespace std;

CMapID g_MapID;
const char g_cchLogMapEntry = 0;

_DBC_USING

const char* GetResPath(const char *pszRes);

CMapRes::CMapRes()
:m_pCLeftMap(0),m_pCTopMap(0),m_pCRightMap(0),m_pCBelowMap(0)
,m_pCMonsterSpawn(0)
,m_pCMapSwitchEntitySpawn(0),m_pNpcSpawn(0)
,m_csEyeshotCellWidth(800),m_csEyeshotCellHeight(800)
,m_csStateCellWidth(200),m_csStateCellHeight(200)
,m_csBlockUnitWidth(50),m_csBlockUnitHeight(50)
,m_byMapID(0),m_bCanStall(true),m_bCanGuild(true)
{
	m_bValid = false;
	m_chState = enumMAP_STATE_CLOSE;

	m_sMapCpyNum = 0;
	m_pCMapCopy = 0;
	m_timeMgr.Begin(5 * 1000);
	m_timeRun.Begin(100);
	m_pfEntryFile = 0;
}

CMapRes::~CMapRes()
{
	if (m_pCMonsterSpawn)
	{
		delete m_pCMonsterSpawn;
		m_pCMonsterSpawn = 0;
	}
	if (m_pCMapSwitchEntitySpawn)
	{
		delete m_pCMapSwitchEntitySpawn;
		m_pCMapSwitchEntitySpawn = 0;
	}
	SAFE_DELETE( m_pNpcSpawn );

	if (m_pCMapCopy)
	{
		delete [] m_pCMapCopy;
		m_pCMapCopy = 0;
	}

	if (m_pfEntryFile)
	{
		fclose(m_pfEntryFile);
		m_pfEntryFile = 0;
	}
}

bool CMapRes::Init()
{
	SetGuildWar(false);

	if( !g_MapID.GetID( m_strMapName.c_str(), m_byMapID ) )
	{
		//LG("initmap", "%sID!", m_strMapName.c_str() );
		ToLogService("common", "configure map({})ID info error", m_strMapName.c_str() );
		char szData[128];
		sprintf( szData, RES_STRING(GM_MAPRES_CPP_00001), m_strMapName.c_str() );
		//MessageBox( NULL, szData, "", IDOK );
		MessageBox( NULL, szData, RES_STRING(GM_MAPRES_CPP_00002), IDOK );
		return false;
	}
	else
	{
		//LG("initmap", "%sID = %d!", m_strMapName.c_str(), m_byMapID );
		ToLogService("common", "set makp ({})ID = {}!", m_strMapName.c_str(), m_byMapID );
	}
	g_MapID.SetMap( m_byMapID, this );	

	if (m_CBlock.Load(GetResPath(m_szObstacleFile)) == 0)
	{
		//LG("init", "[%s], !\n", m_szObstacleFile);
		ToLogService("common", " Loa object obstacle file[{}] error,continue!", m_szObstacleFile);
		return false;
	}

	int	nMapWidth = m_CBlock.getWidth() / 2, nMapHeight = m_CBlock.getHeight() / 2;
	m_SRange.ltop.x = 0;
	m_SRange.ltop.y = 0;
	m_SRange.rbtm.x = nMapWidth * 100;
	m_SRange.rbtm.y = nMapHeight * 100;
	//
	if((m_SRange.width() % m_csEyeshotCellWidth) || (m_SRange.height() % m_csEyeshotCellHeight))
	{
		//LG("init", "[%s]\n", m_strMapName);
		ToLogService("common", "the map[{}]'s length or width isn't the multiple of manage cell", m_strMapName);
		return false;
	}

	if (m_CTerrain.Init((_TCHAR*)GetResPath(m_szSectionFile)) == 0)
	{
		//LG("init", "[%s]!", m_szSectionFile);
		ToLogService("common", "Load the map section file [{}]error !", m_szSectionFile);
		//return false;
	}

	m_sStateCellCol = (nMapWidth * 100 + m_csStateCellWidth - 1) / m_csStateCellWidth;
	m_sStateCellLin = (nMapHeight * 100 + m_csStateCellHeight - 1) / m_csStateCellHeight;
	m_sEyeshotCellCol = (nMapWidth * 100 + m_csEyeshotCellWidth - 1) / m_csEyeshotCellWidth;
	m_sEyeshotCellLin = (nMapHeight * 100 + m_csEyeshotCellHeight - 1) / m_csEyeshotCellHeight;

	m_pCMonsterSpawn = new CChaSpawn();
	if (!m_pCMonsterSpawn->Init(m_szMonsterSpawnFile, 200))
	{
		//THROW_EXCP( excpMem, "!" );
		THROW_EXCP( excpMem, RES_STRING(GM_MAPRES_CPP_00003) );
	}
	m_pCMapSwitchEntitySpawn = new CMapSwitchEntitySpawn();
	if (!m_pCMapSwitchEntitySpawn->Init(m_szMapSwitchFile, 100))
	{
		//THROW_EXCP( excpMem, "!" );
		THROW_EXCP( excpMem, RES_STRING(GM_MAPRES_CPP_00004) );
	}
	m_pNpcSpawn = new CNpcSpawn();
	if (!m_pNpcSpawn->Init(m_szNpcSpawnFile, 200))
	{
		//THROW_EXCP( excpMem, "NPC!" );
		THROW_EXCP( excpMem, RES_STRING(GM_MAPRES_CPP_00005) );
	}

	if (!InitCtrl())
		//THROW_EXCP( excpMem, "!" );
		THROW_EXCP( excpMem, RES_STRING(GM_MAPRES_CPP_00006) );

	//LG("init", " %s \n", m_strMapName.c_str());
	ToLogService("common", "map {} init resource succeed", m_strMapName.c_str());

	m_chCopyStartCdtType = enumMAPCOPY_START_CDT_UNKN;
	m_bValid = true;
	Open();
	return true;
}

bool CMapRes::SetCopyNum(dbc::Short sCpyNum)
{
	if (m_pCMapCopy || sCpyNum < 1 || sCpyNum > defMAX_MAP_COPY_NUM)
	{
		//LG("", "msg %d  %d!\n", sCpyNum, defMAX_MAP_COPY_NUM);
		{ char _buf[256]; sprintf(_buf, RES_STRING(GM_GAMEAPP_CPP_00008), sCpyNum, defMAX_MAP_COPY_NUM); g_logManager.InternalLog(LogLevel::Error, "errors", _buf); }
		return false;
	}
	m_sMapCpyNum = sCpyNum;
	return true;
}

SubMap* CMapRes::GetCopy(dbc::Short sCpyNO)
{
	if (sCpyNO < 0) return m_pCMapCopy;
	if (sCpyNO >= m_sMapCpyNum) return 0;
	return m_pCMapCopy + sCpyNO;
}

BOOL CMapRes::SummonNpc( USHORT sAreaID, const char szNpc[], USHORT sTime )
{
	return m_pNpcSpawn->SummonNpc( szNpc, sAreaID, sTime );
}

// 
bool CMapRes::InitCtrl(void)
{
	if (g_IsFileExist(GetResPath(m_szCtrlFile)))
		luaL_dofile(g_pLuaState, GetResPath(m_szCtrlFile));

	m_szEntryMapName[0] = '\0';
	m_SEntryPos.x = 0;
	m_SEntryPos.y = 0;
	m_chEntryState = 0;
	m_tEntryFirstTm = 0;
	m_tEntryTmDis = 0;
	m_tEntryOutTmDis = 0;
	m_tMapClsTmDis = 0;

	m_sMapCpyNum = 1;
	SetCanSavePos();
	SetCanPK(false);
	SetCanTeam();
	SetRepatriateDie(true);
	SetType();
	SetCopyStartType();

	g_luaAPI.Call("init_entry", this);
	g_luaAPI.Call("config", this);

	// 
	m_pCMapCopy = new SubMap[m_sMapCpyNum];
	if (!m_pCMapCopy)
		//THROW_EXCP(excpMem,"");
		THROW_EXCP(excpMem,RES_STRING(GM_MAPRES_CPP_00007));
	for (Short i = 0; i < m_sMapCpyNum; i++)
	{
		if (!m_pCMapCopy[i].Init(this, i))
			return false;
	}

	m_pfEntryFile = fopen(GetResPath(m_szEntryFile), "rb");

	return true;
}

// .
bool CMapRes::CreateEntry(void)
{
	if(!m_pfEntryFile)
		return false;
	fseek(m_pfEntryFile, 0, SEEK_SET);

	string	strScript = "get_map_entry_pos_";
	strScript += GetName();
	auto result = g_luaAPI.CallMulti(strScript.c_str());
	if (!result.hasFailed() && result.size() >= 2)
	{
		auto rx = result[0].cast<int>();
		auto ry = result[1].cast<int>();
		m_SEntryPos.x = (rx ? rx.value() : 0) * 100;
		m_SEntryPos.y = (ry ? ry.value() : 0) * 100;
	}

	cShort	csLineCharNum = 2048;
	Char	szLine[csLineCharNum + 1];
	Char	*pszPos;
	bool	bRead = true;
	Short	sLineNum = 0;
	szLine[csLineCharNum] = '\0';

	if (g_cchLogMapEntry)
	{
		//LG("", "\n");
		ToLogService("common", "");
		//LG("", " %s --> %s[%u, %u]\n", GetName(), m_szEntryMapName, m_SEntryPos.x, m_SEntryPos.y);
		ToLogService("common", "ask for found entrance : position {} --> {}[{}, {}]", GetName(), m_szEntryMapName, m_SEntryPos.x, m_SEntryPos.y);
	}
	//  :      
	net::msg::GmMapEntryCreateMessage entryMsg;
	entryMsg.targetMapName = m_szEntryMapName;
	entryMsg.srcMapName = GetName();
	entryMsg.posX = m_SEntryPos.x;
	entryMsg.posY = m_SEntryPos.y;
	entryMsg.copyNum = GetCopyNum();
	entryMsg.copyPlyNum = GetCopyPlyNum();
	while (!feof(m_pfEntryFile))
	{
		szLine[0] = '\0';
		if (!fgets(szLine, csLineCharNum, m_pfEntryFile))
		{
			if (!feof(m_pfEntryFile))
			{
				{ char _buf[512]; sprintf(_buf, RES_STRING(GM_GAMEAPP_CPP_00009), GetName(), csLineCharNum, szLine); g_logManager.InternalLog(LogLevel::Error, "errors", _buf); }
				return false;
			}
		}
		if ((pszPos = strstr(szLine, "--")) != NULL)
			*pszPos = '\0';
		if (!strcmp(szLine, ""))
			continue;
		if ((pszPos = strstr(szLine, "\x0a")) != NULL)
			*pszPos = ' ';
		if ((pszPos = strstr(szLine, "\x0d")) != NULL)
			*pszPos = ' ';
		entryMsg.scriptLines.push_back(szLine);
	}
	auto wpk = net::msg::serialize(entryMsg);

	BEGINGETGATE();
	GateServer	*pGateServer;
	while (pGateServer = GETNEXTGATE())
	{
		pGateServer->SendData(wpk);
		break;
	}

	return true;
}

// 
bool CMapRes::DestroyEntry(void)
{
	if (g_cchLogMapEntry)
		//LG("", " %s --> %s\n", GetName(), m_szEntryMapName);
		ToLogService("common", "ask for close entrance:position {} --> {}", GetName(), m_szEntryMapName);
	//  :    
	net::msg::MapEntryMessage meMsg;
	meMsg.srcMapName = m_szEntryMapName;
	meMsg.targetMapName = GetName();
	meMsg.subType = net::msg::MAPENTRY_DESTROY;
	auto wpk = net::msg::serialize(meMsg, CMD_MT_MAPENTRY);

	BEGINGETGATE();
	GateServer	*pGateServer;
	while (pGateServer = GETNEXTGATE())
	{
		pGateServer->SendData(wpk);
		break;
	}

	return true;
}

// 
bool CMapRes::SubEntryPlayer(dbc::Short sCopyNO)
{
	if (!strcmp(m_szEntryMapName, ""))
		return true;

	//  :     
	net::msg::MapEntryMessage meMsg;
	meMsg.srcMapName = m_szEntryMapName;
	meMsg.targetMapName = GetName();
	meMsg.subType = net::msg::MAPENTRY_SUBPLAYER;
	meMsg.copyNo = sCopyNO;
	meMsg.numPlayers = 1;
	auto wpk = net::msg::serialize(meMsg, CMD_MT_MAPENTRY);

	BEGINGETGATE();
	GateServer	*pGateServer;
	while (pGateServer = GETNEXTGATE())
	{
		pGateServer->SendData(wpk);
		break;
	}

	return true;
}

// 
bool CMapRes::SubEntryCopy(dbc::Short sCopyNO)
{
	if (!strcmp(m_szEntryMapName, ""))
		return true;

	if (g_cchLogMapEntry)
		//LG("", "%s%d\n", GetName(), sCopyNO);
		ToLogService("common", "ask for close copy map({}{})", GetName(), sCopyNO);
	//  :   
	net::msg::MapEntryMessage meMsg;
	meMsg.srcMapName = m_szEntryMapName;
	meMsg.targetMapName = GetName();
	meMsg.subType = net::msg::MAPENTRY_SUBCOPY;
	meMsg.copyNo = sCopyNO;
	auto wpk = net::msg::serialize(meMsg, CMD_MT_MAPENTRY);

	BEGINGETGATE();
	GateServer	*pGateServer;
	while (pGateServer = GETNEXTGATE())
	{
		pGateServer->SendData(wpk);
		break;
	}

	return true;
}

bool CMapRes::SetEntryMapName(const char *szMapName)
{
	if (!szMapName) return false;
	strncpy(m_szEntryMapName, szMapName, MAX_MAPNAME_LENGTH - 1);
	m_szEntryMapName[MAX_MAPNAME_LENGTH - 1] = '\0';

	return true;
}

bool CMapRes::Open(void)
{
	if (m_chState == enumMAP_STATE_OPEN)
		return true;

	m_chState = enumMAP_STATE_OPEN;

	return true;
}

bool CMapRes::Close(void)
{
	if (m_chState == enumMAP_STATE_CLOSE)
	{
		if (g_cchLogMapEntry)
			//LG("", "%s\n", GetName());
			ToLogService("common", "already close the map:{}", GetName());
		return true;
	}

	CloseEntry();

	if (m_chEntryState == enumMAPENTRY_STATE_CLOSE || m_chEntryState == enumMAPENTRY_STATE_CLOSE_SUC)
	{
		m_chState = enumMAP_STATE_CLOSE;
		m_chEntryState = enumMAPENTRY_STATE_CLOSE;
		CopyClose();

		if (g_cchLogMapEntry)
			//LG("", "%s\n", GetName());
			ToLogService("common", "close map succeed :{}", GetName());
		return true;
	}

	if (m_chState != enumMAP_STATE_CLOSE && m_chEntryState == enumMAPENTRY_STATE_ASK_CLOSE)
	{
		m_chState = enumMAP_STATE_ASK_CLOSE;
		if (g_cchLogMapEntry)
			//LG("", "%s\n", GetName());
			ToLogService("common", "ask for close map:{}", GetName());
	}

	return false;
}

// 
void CMapRes::Run(DWORD dwCurTime)
{
	if (!m_timeRun.IsOK(dwCurTime))
		return;

	for (Short i = 0; i < m_sMapCpyNum; i++)
		m_pCMapCopy[i].Run(dwCurTime);


	if (!m_timeMgr.IsOK(dwCurTime))
		return;

	string	strScript = "map_run_";
	strScript += GetName();
	g_luaAPI.Call(strScript.c_str(), this);

	if (!HasDynEntry())
		return;
	time_t	tNowTime = time(NULL);
	if (tNowTime < m_tEntryFirstTm)
		return;

	bool bOpenEntry = false, bCloseEntry = false, bClose = false;
	time_t	tDist = tNowTime - m_tEntryFirstTm;
	if (m_tEntryTmDis != 0)
		tDist = tDist % m_tEntryTmDis;
	if (m_tEntryOutTmDis == 0) // 
		bOpenEntry = true;
	else
	{
		if (tDist < m_tEntryOutTmDis) // 
			bOpenEntry = true;
	}

	if (tDist >= m_tEntryOutTmDis) // 
	{
		bCloseEntry = true;
		if (m_tEntryOutTmDis == 0) // 
			bCloseEntry = false;
	}
	if (tDist >= m_tMapClsTmDis) // 
	{
		bClose = true;
		if (m_tMapClsTmDis == 0) // 
			bClose = false;
	}

	if (bOpenEntry)
	{
		string	strScript = "can_open_entry_";
		strScript += GetName();
		if (g_luaAPI.HasFunction(strScript.c_str()))
		{
			auto val = g_luaAPI.CallR<int>(strScript.c_str(), this);
			if (val.value_or(0) == 0)
				bOpenEntry = false;
		}
	}
	if (bOpenEntry)
		OpenEntry();
	if (bCloseEntry)
		CloseEntry();
	if (bClose)
		Close();

	time_t	tBeepT = m_tMapClsTmDis - tDist;
	if (m_chState == enumMAP_STATE_OPEN && tBeepT > 0 && tBeepT < 50)
	{
		Char szInfo[128];
		sprintf(szInfo, RES_STRING(GM_MAPRES_CPP_00008), tBeepT);
		CopyNotice(szInfo);
	}

}

// 
bool CMapRes::OpenEntry(void)
{
	if (m_chEntryState == enumMAPENTRY_STATE_OPEN)
		return true;

	if (m_chEntryState == enumMAPENTRY_STATE_CLOSE_SUC)
		return true;

	if (m_chEntryState == enumMAPENTRY_STATE_ASK_CLOSE)
		return false;

	if (m_chEntryState == enumMAPENTRY_STATE_CLOSE || m_chEntryState == enumMAPENTRY_STATE_ASK_OPEN)
	{
		if (!CreateEntry())
			return false;
	}

	m_chEntryState = enumMAPENTRY_STATE_ASK_OPEN;

	return true;
}

// 
bool CMapRes::CloseEntry(void)
{
	if (m_chEntryState == enumMAPENTRY_STATE_ASK_OPEN)
		return false;

	if (m_chEntryState == enumMAPENTRY_STATE_CLOSE || m_chEntryState == enumMAPENTRY_STATE_CLOSE_SUC)
		return true;

	if (m_chEntryState == enumMAPENTRY_STATE_OPEN || m_chEntryState == enumMAPENTRY_STATE_ASK_CLOSE)
	{
		if (!DestroyEntry())
			return false;
	}

	m_chEntryState = enumMAPENTRY_STATE_ASK_CLOSE;

	return true;
}

// 
bool CMapRes::CopyClose(dbc::Short sCopyNO)
{
	if (sCopyNO >= GetCopyNum())
		return false;

	if (g_cchLogMapEntry)
		//LG("", "%s%d\n", GetName(), sCopyNO);
		ToLogService("common", "close map copy {}{}", GetName(), sCopyNO);
	if (sCopyNO < 0)
	{
		for (Short i = 0; i < m_sMapCpyNum; i++)
		{
			m_pCMapCopy[i].Close();
		}
	}
	else
	{
		m_pCMapCopy[sCopyNO].Close();
	}

	return true;
}

bool CMapRes::CopyNotice(const char *szString, dbc::Short sCopyNO)
{
	if (sCopyNO >= GetCopyNum())
		return false;

	if (sCopyNO < 0)
		for (Short i = 0; i < m_sMapCpyNum; i++)
			m_pCMapCopy[i].Notice(szString);
	else
		m_pCMapCopy[sCopyNO].Notice(szString);

	return true;
}

// 
bool CMapRes::ReleaseCopy(dbc::Short sCopyNO)
{
	return SubEntryCopy(sCopyNO);
}

void CMapRes::CheckEntryState(dbc::Char chState)
{
	if (chState == enumMAPENTRY_STATE_OPEN)
	{
		if (m_chEntryState == enumMAPENTRY_STATE_ASK_OPEN)
		{
			m_chEntryState = chState;
			Open();
		}
		else // 
		{
		}

		for (Short i = 0; i < m_sMapCpyNum; i++)
		{
			if (GetCopyStartType() == enumMAPCOPY_START_NOW)
				m_pCMapCopy[i].Open();
		}
	}
	else if (chState == enumMAPENTRY_STATE_CLOSE_SUC)
	{
		if (m_chEntryState == enumMAPENTRY_STATE_ASK_CLOSE)
		{
			m_chEntryState = chState;
		}
		else // 
		{
		}

		if (m_chState == enumMAP_STATE_ASK_CLOSE)
		{
			Close();
		}
	}
}

// 
SubMap* CMapRes::GetNextUsedCopy(void)
{
	if (!m_pCMapCopy)
		return NULL;

	Short	sCopyNum = GetCopyNum();

	if (m_sUsedCopySearch >= sCopyNum)
		return NULL;

	for (Short i = m_sUsedCopySearch; i < sCopyNum; i++)
	{
		m_sUsedCopySearch = i + 1;
		if (m_pCMapCopy[i].IsRun())
			return m_pCMapCopy + i;
	}

	return NULL;
}

mission::CNpc* CMapRes::FindNpc( const char szName[] )
{
	if( szName )
	{
		return m_pNpcSpawn->FindNpc( szName );
	}
	return NULL;
}

//=============================================================================
CAreaData::CAreaData()
{
	m_sUnitCountX = 0;
	m_sUnitCountY = 0;
	m_sUnitWidth = 100;
	m_sUnitHeight = 100;
	m_nID = -1;
}

CAreaData::~CAreaData()
{
	Free();
}

Long CAreaData::Init(_TCHAR *chFile)
{
	if ((m_nID = s_openAttribFile(chFile)) == -1)
		return 0;

	unsigned int nWidth, nHeight;
	s_getAttribFileInfo(m_nID, nWidth, nHeight);

	m_sUnitCountX = (Short)nWidth;
	m_sUnitCountY = (Short)nHeight;

	return 1;
}

void CAreaData::Free()
{
}

bool CAreaData::GetUnitSize(Short *psWidth, Short *psHeight)
{
	*psWidth = m_sUnitWidth;
	*psHeight = m_sUnitHeight;

	return true;
}

bool CAreaData::GetUnitAttr(Short sUnitX, Short sUnitY, uShort &usAttribute)
{
	if (m_nID == -1)
		return false;
	//if (!IsValidPos(sUnitX, sUnitY))
	//	return false;
	return s_getTileAttrib(m_nID, sUnitX, sUnitY, usAttribute);
}

bool CAreaData::GetUnitIsland(Short sUnitX, Short sUnitY, uChar &uchIsland)
{
	if (m_nID == -1)
		return false;
	//if (!IsValidPos(sUnitX, sUnitY))
	//	return false;
	return s_getTileIsland(m_nID, sUnitX, sUnitY, uchIsland);
}

//=============================================================================
CMapID::CMapID()
{
	Clear();
}

CMapID::~CMapID()
{
	
}

void CMapID::Clear()
{
	m_MapInfo.clear();
	m_byNumMap = 0;
}

BOOL CMapID::AddInfo( const char szMap[], BYTE byID )
{
	MAP_INFO info{};
	strncpy( info.szMap, szMap, MAX_MAPNAME_LENGTH - 1 );
	info.byID = byID;
	info.pMap = nullptr;
	m_MapInfo.push_back(info);
	m_byNumMap++;
	return TRUE;
}

BOOL CMapID::GetID( const char szMap[], BYTE& byID )
{
	for( BYTE b = 0; b < m_byNumMap; b++ )
	{
		if( strcmp( m_MapInfo[b].szMap, szMap ) == 0 )
		{
			byID = m_MapInfo[b].byID;
			return TRUE;
		}
	}
	return FALSE;
}

CMapRes* CMapID::GetMap( BYTE byID )
{
	for( BYTE b = 0; b < m_byNumMap; b++ )
	{
		if( byID == m_MapInfo[b].byID )
		{
			return m_MapInfo[b].pMap;
		}
	}
	return NULL;
}

BOOL CMapID::SetMap( BYTE byID, CMapRes* pMap )
{
	for( BYTE b = 0; b < m_byNumMap; b++ )
	{
		if( byID == m_MapInfo[b].byID )
		{
			m_MapInfo[b].pMap = pMap;
			return TRUE;
		}
	}
	return FALSE;
}
