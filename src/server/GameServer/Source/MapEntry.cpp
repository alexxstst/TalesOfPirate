//=============================================================================
// FileName: MapEntry.cpp
// Creater: ZhangXuedong
// Date: 2005.10.21
// Comment: Map Entry class
//=============================================================================
#include "stdafx.h"
#include "MapEntry.h"
#include "Parser.h"
#include "NetCommand.h"

using namespace std;

CDynMapEntry	g_CDMapEntry;

char	g_szTFightMapName[MAX_MAPNAME_LENGTH] = "";	// 

//=============================================================================
void CMapEntryCopyCell::WriteParamPacket(WPACKET &pk)
{
	for (dbc::Char i = 0; i < defMAPCOPY_INFO_PARAM_NUM; i++)
		WRITE_LONG(pk, m_lParam[i]);
}

//=============================================================================
void CDynMapEntryCell::SetCopyNum(dbc::Short sCopyNum)
{T_B
	if (sCopyNum > defMAX_MAP_COPY_NUM)
	{
		//LG("", "msg %d  %d!\n", sCopyNum, defMAX_MAP_COPY_NUM);
		LG("copy number error", RES_STRING(GM_GAMEAPP_CPP_00008), sCopyNum, defMAX_MAP_COPY_NUM);
		return;
	}

	if (m_LCopyInfo.GetSize() == 0)
		m_LCopyInfo.Init(defMAX_MAP_COPY_NUM);
	else
		m_LCopyInfo.Reset();

	m_sMapCopyNum = sCopyNum;
	m_LCopyInfo.SetCapacity(m_sMapCopyNum);
T_E}

void CDynMapEntryCell::SetEventName(dbc::cChar *cszEventName)
{T_B
	m_CEvtObj.SetName(cszEventName);

	if (m_pCEnt)
	{
		m_pCEnt->m_CEvent.SetName(cszEventName);
		m_pCEnt->SynEventInfo();
	}
T_E}

void CDynMapEntryCell::FreeEnti()
{T_B
	if (m_pCEnt)
	{
		m_pCEnt->Free();
		m_pCEnt = NULL;
	}
T_E}

void CDynMapEntryCell::SynCopyParam(dbc::Short sCopyID)
{T_B
	CMapEntryCopyCell	*pCCopyCell = GetCopy(sCopyID);
	if (!pCCopyCell)
		return;

	WPACKET	wpk	=GETWPACKET();
	WRITE_CMD(wpk, CMD_MT_MAPENTRY);
	WRITE_STRING(wpk, GetTMapName());
	WRITE_STRING(wpk, GetMapName());
	WRITE_CHAR(wpk, enumMAPENTRY_COPYPARAM);
	WRITE_SHORT(wpk, sCopyID);
	pCCopyCell->WriteParamPacket(wpk);

	BEGINGETGATE();
	GateServer	*pGateServer = NULL;
	while (pGateServer = GETNEXTGATE())
	{
		pGateServer->SendData(wpk);
	}
T_E}

void CDynMapEntryCell::SynCopyRun(dbc::Short sCopyID, dbc::Char chCdtType, dbc::Long chCdtVal)
{T_B
	CMapEntryCopyCell	*pCCopyCell = GetCopy(sCopyID);
	if (!pCCopyCell)
		return;

	WPACKET	wpk	=GETWPACKET();
	WRITE_CMD(wpk, CMD_MT_MAPENTRY);
	WRITE_STRING(wpk, GetTMapName());
	WRITE_STRING(wpk, GetMapName());
	WRITE_CHAR(wpk, enumMAPENTRY_COPYRUN);
	WRITE_SHORT(wpk, sCopyID);
	WRITE_CHAR(wpk, chCdtType);
	WRITE_LONG(wpk, chCdtVal);

	BEGINGETGATE();
	GateServer	*pGateServer = NULL;
	while (pGateServer = GETNEXTGATE())
	{
		pGateServer->SendData(wpk);
		break;
	}
T_E}

//=============================================================================
CDynMapEntryCell* CDynMapEntry::Add(CDynMapEntryCell *pCCell)
{T_B
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
T_E}

bool CDynMapEntry::Del(CDynMapEntryCell *pCCell)
{T_B
	if (!pCCell)
		return false;

	pCCell->FreeEnti();
	return m_LEntryList.Del(pCCell);
T_E}

bool CDynMapEntry::Del(dbc::cChar *cszTMapName)
{T_B
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
T_E}

CDynMapEntryCell* CDynMapEntry::GetEntry(dbc::cChar *cszTMapName)
{T_B
	CDynMapEntryCell	*pCCell;
	m_LEntryList.BeginGet();
	while (pCCell = m_LEntryList.GetNext())
	{
		if (!strncmp(pCCell->GetTMapName(), cszTMapName, MAX_MAPNAME_LENGTH))
			return pCCell;
	}

	return NULL;
T_E}

void CDynMapEntry::AfterPlayerLogin(const char *cszName)
{T_B
	if (!cszName)
		return;

	CDynMapEntryCell	*pCCell;
	m_LEntryList.BeginGet();
	while (pCCell = m_LEntryList.GetNext())
	{
		string	strScript = "after_player_login_";
		strScript += pCCell->GetTMapName();
		g_CParser.DoString(strScript.c_str(), enumSCRIPT_RETURN_NONE, 0, enumSCRIPT_PARAM_LIGHTUSERDATA, 1, pCCell, enumSCRIPT_PARAM_STRING, 1, cszName, DOSTRING_PARAM_END);
	}
T_E}

//=============================================================================
void	g_SetTeamFightMapName(const char *cszMapName)
{T_B
	if (cszMapName)
	{
		strncpy(g_szTFightMapName, cszMapName, MAX_MAPNAME_LENGTH - 1);
		g_szTFightMapName[MAX_MAPNAME_LENGTH - 1] = '\0';
	}
	else
	{
		g_szTFightMapName[0] = '\0';
	}
T_E}
