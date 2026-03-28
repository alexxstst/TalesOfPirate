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

char	g_szTFightMapName[MAX_MAPNAME_LENGTH] = "";	// ������ս��ͼ��

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
		//LG("������Ŀ����", "msg�趨�ĸ�����Ŀ %d �������ֵ %d!\n", sCopyNum, defMAX_MAP_COPY_NUM);
		{ char _buf[256]; sprintf(_buf, RES_STRING(GM_GAMEAPP_CPP_00008), sCopyNum, defMAX_MAP_COPY_NUM); g_logManager.InternalLog(LogLevel::Error, "errors", _buf); }
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

	// Типизированная сериализация: параметры копии карты
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

	// Типизированная сериализация: запуск условия копии карты
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
		g_CParser.DoString(strScript.c_str(), enumSCRIPT_RETURN_NONE, 0, enumSCRIPT_PARAM_LIGHTUSERDATA, 1, pCCell, enumSCRIPT_PARAM_STRING, 1, cszName, DOSTRING_PARAM_END);
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
