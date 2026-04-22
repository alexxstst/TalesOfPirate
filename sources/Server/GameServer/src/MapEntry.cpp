//=============================================================================
// FileName: MapEntry.cpp
// Creater: ZhangXuedong
// Date: 2005.10.21
// Comment: Map Entry class
//=============================================================================
#include "stdafx.h"
#include "MapEntry.h"
#include "LuaAPI.h"
#include "NetCommand.h"

using namespace std;

CDynMapEntry	g_CDMapEntry;

char	g_szTFightMapName[MAX_MAPNAME_LENGTH] = "";	// 

//=============================================================================
void CMapEntryCopyCell::WriteParamPacket(net::WPacket &pk)
{
	for (dbc::Char i = 0; i < defMAPCOPY_INFO_PARAM_NUM; i++)
		pk.WriteInt64(m_lParam[i]);
}

//=============================================================================
void CDynMapEntryCell::SetCopyNum(dbc::Short sCopyNum)
{
	if (sCopyNum > defMAX_MAP_COPY_NUM)
	{
		//LG("", "msg %d  %d!\n", sCopyNum, defMAX_MAP_COPY_NUM);
		{ char _buf[256]; std::snprintf(_buf, sizeof(_buf), RES_STRING(GM_GAMEAPP_CPP_00008), sCopyNum, defMAX_MAP_COPY_NUM); g_logManager.InternalLog(LogLevel::Error, "errors", _buf); }
		return;
	}

	if (m_LCopyInfo.GetSize() == 0)
		m_LCopyInfo.Init(defMAX_MAP_COPY_NUM);
	else
		m_LCopyInfo.Reset();

	m_sMapCopyNum = sCopyNum;
	m_LCopyInfo.SetCapacity(m_sMapCopyNum);
}

void CDynMapEntryCell::SetEventName(dbc::cChar *cszEventName)
{
	m_CEvtObj.SetName(cszEventName);

	if (m_pCEnt)
	{
		m_pCEnt->m_CEvent.SetName(cszEventName);
		m_pCEnt->SynEventInfo();
	}
}

void CDynMapEntryCell::FreeEnti()
{
	if (m_pCEnt)
	{
		m_pCEnt->Free();
		m_pCEnt = NULL;
	}
}

void CDynMapEntryCell::SynCopyParam(dbc::Short sCopyID)
{
	CMapEntryCopyCell	*pCCopyCell = GetCopy(sCopyID);
	if (!pCCopyCell)
		return;

	//  :   
	net::msg::GmMapEntryCopyParamMessage copyMsg;
	copyMsg.targetMapName = GetTMapName();
	copyMsg.srcMapName = GetMapName();
	copyMsg.copyId = sCopyID;
	for (dbc::Char i = 0; i < defMAPCOPY_INFO_PARAM_NUM; i++)
		copyMsg.params[i] = pCCopyCell->GetParam(i);
	auto wpk = net::msg::serialize(copyMsg);

	BEGINGETGATE();
	GateServer	*pGateServer = NULL;
	while (pGateServer = GETNEXTGATE())
	{
		pGateServer->SendData(wpk);
	}
}

void CDynMapEntryCell::SynCopyRun(dbc::Short sCopyID, dbc::Char chCdtType, dbc::Long chCdtVal)
{
	CMapEntryCopyCell	*pCCopyCell = GetCopy(sCopyID);
	if (!pCCopyCell)
		return;

	//  :    
	net::msg::MapEntryMessage meMsg;
	meMsg.srcMapName = GetTMapName();
	meMsg.targetMapName = GetMapName();
	meMsg.subType = net::msg::MAPENTRY_COPYRUN;
	meMsg.copyId = sCopyID;
	meMsg.condType = chCdtType;
	meMsg.condValue = chCdtVal;
	auto wpk = net::msg::serialize(meMsg, CMD_MT_MAPENTRY);

	BEGINGETGATE();
	GateServer	*pGateServer = NULL;
	while (pGateServer = GETNEXTGATE())
	{
		pGateServer->SendData(wpk);
		break;
	}
}

//=============================================================================
CDynMapEntryCell* CDynMapEntry::Add(CDynMapEntryCell *pCCell)
{
	CDynMapEntryCell	*pCObj;
	pCObj = GetEntry(pCCell->GetTMapName());
	if (!pCObj)
		pCObj = m_LEntryList.Add(pCCell);
	else
		pCObj->FreeEnti();
	if (!pCObj)
		return NULL;
	CEvent	*pCEvent = pCObj->GetEvent();
	pCEvent->SetTableRec(pCObj);

	return pCObj;
}

bool CDynMapEntry::Del(CDynMapEntryCell *pCCell)
{
	if (!pCCell)
		return false;

	pCCell->FreeEnti();
	return m_LEntryList.Del(pCCell);
}

bool CDynMapEntry::Del(dbc::cChar *cszTMapName)
{
	if (!cszTMapName)
		return false;

	CDynMapEntryCell	*pCCell;
	m_LEntryList.BeginGet();
	while (pCCell = m_LEntryList.GetNext())
	{
		if (!strncmp(pCCell->GetTMapName(), cszTMapName, MAX_MAPNAME_LENGTH))
		{
			pCCell->FreeEnti();
			return m_LEntryList.Del(pCCell);
		}
	}

	return false;
}

CDynMapEntryCell* CDynMapEntry::GetEntry(dbc::cChar *cszTMapName)
{
	CDynMapEntryCell	*pCCell;
	m_LEntryList.BeginGet();
	while (pCCell = m_LEntryList.GetNext())
	{
		if (!strncmp(pCCell->GetTMapName(), cszTMapName, MAX_MAPNAME_LENGTH))
			return pCCell;
	}

	return NULL;
}

void CDynMapEntry::AfterPlayerLogin(const char *cszName)
{
	if (!cszName)
		return;

	CDynMapEntryCell	*pCCell;
	m_LEntryList.BeginGet();
	while (pCCell = m_LEntryList.GetNext())
	{
		string	strScript = "after_player_login_";
		strScript += pCCell->GetTMapName();
		g_luaAPI.Call(strScript.c_str(), pCCell, cszName);
	}
}

//=============================================================================
void	g_SetTeamFightMapName(const char *cszMapName)
{
	if (cszMapName)
	{
		strncpy(g_szTFightMapName, cszMapName, MAX_MAPNAME_LENGTH - 1);
		g_szTFightMapName[MAX_MAPNAME_LENGTH - 1] = '\0';
	}
	else
	{
		g_szTFightMapName[0] = '\0';
	}
}

// ============================================================================
// Ранее inline-методы из MapEntry.h, вынесены в .cpp 2026-04-22.
// ============================================================================

CMapEntryCopyCell::CMapEntryCopyCell(dbc::Short sMaxPlyNum, dbc::Short sCurPlyNum) {
	m_sMaxPlyNum = sMaxPlyNum;
	m_sCurPlyNum = sCurPlyNum;
	m_sPosID = -1;
}

void       CMapEntryCopyCell::SetMaxPlyNum(dbc::Short sPlyNum) { m_sMaxPlyNum = sPlyNum; }
dbc::Short CMapEntryCopyCell::GetMaxPlyNum(void)               { return m_sMaxPlyNum; }
void       CMapEntryCopyCell::SetCurPlyNum(dbc::Short sPlyNum) { m_sCurPlyNum = sPlyNum; }
dbc::Short CMapEntryCopyCell::GetCurPlyNum(void)               { return m_sCurPlyNum; }

bool CMapEntryCopyCell::AddCurPlyNum(dbc::Short sAddNum) {
	dbc::Short sNum = m_sCurPlyNum + sAddNum;
	if (sNum < 0 || sNum > m_sMaxPlyNum) return false;
	m_sCurPlyNum = sNum;
	return true;
}
bool CMapEntryCopyCell::HasFreePlyCount(dbc::Short sRequestNum) {
	return GetMaxPlyNum() - GetCurPlyNum() >= sRequestNum;
}

dbc::Long CMapEntryCopyCell::GetParam(dbc::Char chParamID) {
	if (chParamID < 0 || chParamID >= defMAPCOPY_INFO_PARAM_NUM) return 0;
	return m_lParam[chParamID];
}
bool CMapEntryCopyCell::SetParam(dbc::Char chParamID, dbc::Long lParamVal) {
	if (chParamID < 0 || chParamID >= defMAPCOPY_INFO_PARAM_NUM) return false;
	m_lParam[chParamID] = lParamVal;
	return true;
}

void       CMapEntryCopyCell::SetPosID(dbc::Long lPosID) { m_sPosID = (dbc::Short)lPosID; }
dbc::Long  CMapEntryCopyCell::GetPosID(void)             { return m_sPosID; }

// --- CDynMapEntryCell ---

CDynMapEntryCell::CDynMapEntryCell() {
	m_lEntiID       = 0;
	m_szMapName[0]  = '\0';
	m_szTMapName[0] = '\0';
	m_pCEnt         = nullptr;

	m_CEvtObj.Init();
	m_CEvtObj.SetTouchType(enumEVENTT_RANGE);
	m_CEvtObj.SetExecType(enumEVENTE_DMAP_ENTRY);

	m_pPos = nullptr;
}

void  CDynMapEntryCell::SetPos(void* pPos) { m_pPos = pPos; }
void* CDynMapEntryCell::GetPos(void)       { return m_pPos; }

void CDynMapEntryCell::SetMapName(dbc::cChar* cszMapName) {
	if (!cszMapName) return;
	strncpy(m_szMapName, cszMapName, MAX_MAPNAME_LENGTH - 1);
	m_szMapName[MAX_MAPNAME_LENGTH - 1] = '\0';
}
dbc::cChar* CDynMapEntryCell::GetMapName(void) const { return m_szMapName; }

void CDynMapEntryCell::SetTMapName(dbc::cChar* cszTMapName) {
	if (!cszTMapName) return;
	strncpy(m_szTMapName, cszTMapName, MAX_MAPNAME_LENGTH - 1);
	m_szTMapName[MAX_MAPNAME_LENGTH - 1] = '\0';
}
dbc::cChar* CDynMapEntryCell::GetTMapName(void) const     { return m_szTMapName; }

const Point* CDynMapEntryCell::GetEntiPos(void) const     { return &m_SEntiPos; }
void         CDynMapEntryCell::SetEntiPos(const Point* p) { m_SEntiPos = *p; }
void         CDynMapEntryCell::SetEntiPos(dbc::Long x, dbc::Long y) { m_SEntiPos.x = x; m_SEntiPos.y = y; }
void         CDynMapEntryCell::SetEntiID(dbc::Long lEntiID)         { m_lEntiID = lEntiID; }
dbc::Long    CDynMapEntryCell::GetEntiID(void)                      { return m_lEntiID; }

void         CDynMapEntryCell::SetEventID(dbc::Long lEventID)       { m_CEvtObj.SetID((dbc::uShort)lEventID); }
dbc::Long    CDynMapEntryCell::GetEventID(void)                     { return m_CEvtObj.GetID(); }

void CDynMapEntryCell::SetEnti(Entity* pCEnt) { m_pCEnt = pCEnt; }

void CDynMapEntryCell::GetPosInfo(const char** pMapN, dbc::Long* lpPosX, dbc::Long* lpPosY, const char** pTMapN) {
	*pMapN  = m_szMapName;
	*lpPosX = m_SEntiPos.x;
	*lpPosY = m_SEntiPos.y;
	*pTMapN = m_szTMapName;
}

CEvent*     CDynMapEntryCell::GetEvent(void)                   { return &m_CEvtObj; }
dbc::Short  CDynMapEntryCell::GetCopyNum(void)                 { return m_sMapCopyNum; }
void        CDynMapEntryCell::SetCopyPlyNum(dbc::Short sCopyNum) { m_sCopyPlyNum = sCopyNum; }
dbc::Short  CDynMapEntryCell::GetCopyPlyNum(void)              { return m_sCopyPlyNum; }

CMapEntryCopyCell* CDynMapEntryCell::AddCopy(CMapEntryCopyCell* pCCpyCell) { return m_LCopyInfo.Add(pCCpyCell); }
CMapEntryCopyCell* CDynMapEntryCell::GetCopy(dbc::Short sCopyID)           { return m_LCopyInfo.Get(sCopyID); }
bool               CDynMapEntryCell::ReleaseCopy(CMapEntryCopyCell* p)     { return m_LCopyInfo.Del(p); }
bool               CDynMapEntryCell::ReleaseCopy(dbc::Long lCopyNO)        { return m_LCopyInfo.Del(lCopyNO); }

// --- CDynMapEntry ---

CDynMapEntry::CDynMapEntry()  { m_LEntryList.Init(); }
CDynMapEntry::~CDynMapEntry() { m_LEntryList.Free(); }