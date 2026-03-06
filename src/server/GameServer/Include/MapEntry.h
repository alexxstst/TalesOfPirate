#pragma once

//=============================================================================
// FileName: MapEntry.h
// Creater: ZhangXuedong
// Date: 2005.10.21
// Comment: Map Entry class
//=============================================================================

#ifndef MAPENTRY_H
#define MAPENTRY_H

#include "Entity.h"
#include "ToolClass.h"
#include "MapRes.h"
#include "EventRecord.h"

class	CMapEntryCopyCell
{
public:
	CMapEntryCopyCell(dbc::Short sMaxPlyNum = 0, dbc::Short sCurPlyNum = 0)
	{
		m_sMaxPlyNum = sMaxPlyNum;
		m_sCurPlyNum = sCurPlyNum;

		m_sPosID = -1;
	}

	void		SetMaxPlyNum(dbc::Short sPlyNum) {m_sMaxPlyNum = sPlyNum;}
	dbc::Short	GetMaxPlyNum(void) {return m_sMaxPlyNum;}
	void		SetCurPlyNum(dbc::Short sPlyNum) {m_sCurPlyNum = sPlyNum;}
	dbc::Short	GetCurPlyNum(void) {return m_sCurPlyNum;}
	bool		AddCurPlyNum(dbc::Short sAddNum) {dbc::Short sNum = m_sCurPlyNum + sAddNum; if (sNum < 0 || sNum > m_sMaxPlyNum) return false; m_sCurPlyNum = sNum; return true;}
	bool		HasFreePlyCount(dbc::Short sRequestNum) {return GetMaxPlyNum() - GetCurPlyNum() >= sRequestNum ? true : false;}

	dbc::Long	GetParam(dbc::Char chParamID) {if (chParamID < 0 || chParamID >= defMAPCOPY_INFO_PARAM_NUM) return 0; return m_lParam[chParamID];}
	bool		SetParam(dbc::Char chParamID, dbc::Long lParamVal) {if (chParamID < 0 || chParamID >= defMAPCOPY_INFO_PARAM_NUM) return false; m_lParam[chParamID] = lParamVal; return true;}

	void		WriteParamPacket(WPACKET &pk);

	void		SetPosID(dbc::Long lPosID) {m_sPosID = (dbc::Short)lPosID;}
	dbc::Long	GetPosID(void) {return m_sPosID;}

protected:

private:
	dbc::Short	m_sMaxPlyNum;
	dbc::Short	m_sCurPlyNum;

	dbc::Long	m_lParam[defMAPCOPY_INFO_PARAM_NUM];

	dbc::Short	m_sPosID;

};

// 
class	CDynMapEntryCell
{
public:
	CDynMapEntryCell()
	{
		m_lEntiID = 0;
		m_szMapName[0] = '\0';
		m_szTMapName[0] = '\0';
		m_pCEnt = NULL;

		m_CEvtObj.Init();
		m_CEvtObj.SetTouchType(enumEVENTT_RANGE);
		m_CEvtObj.SetExecType(enumEVENTE_DMAP_ENTRY);

		m_pPos = NULL;
	}

	void		SetPos(void *pPos) {m_pPos = pPos;}
	void*		GetPos(void) {return m_pPos;}

	void			SetMapName(dbc::cChar *cszMapName) {if (!cszMapName) return; strncpy(m_szMapName, cszMapName, MAX_MAPNAME_LENGTH - 1); m_szMapName[MAX_MAPNAME_LENGTH -1] = '\0';}
	dbc::cChar*		GetMapName(void) const {return m_szMapName;}
	void			SetTMapName(dbc::cChar *cszTMapName) {if (!cszTMapName) return; strncpy(m_szTMapName, cszTMapName, MAX_MAPNAME_LENGTH - 1); m_szTMapName[MAX_MAPNAME_LENGTH -1] = '\0';}
	dbc::cChar*		GetTMapName(void) const {return m_szTMapName;}
	const Point*	GetEntiPos(void) const {return &m_SEntiPos;}
	void			SetEntiPos(const Point* cpSPos) {m_SEntiPos = *cpSPos;}
	void			SetEntiPos(dbc::Long lPosX, dbc::Long lPosY) {m_SEntiPos.x = lPosX; m_SEntiPos.y = lPosY;}
	void			SetEntiID(dbc::Long lEntiID) {m_lEntiID = lEntiID;}
	dbc::Long		GetEntiID(void) {return m_lEntiID;}
	void			SetEventID(dbc::Long lEventID) {m_CEvtObj.SetID((dbc::uShort)lEventID);}
	dbc::Long		GetEventID(void) {return m_CEvtObj.GetID();}
	void			SetEventName(dbc::cChar *cszEventName);
	void			SetEnti(Entity *pCEnt) {m_pCEnt = pCEnt;}
	void			GetPosInfo(const char** pMapN, dbc::Long* lpPosX, dbc::Long* lpPosY, const char** pTMapN) { *pMapN = m_szMapName, * lpPosX = m_SEntiPos.x, * lpPosY = m_SEntiPos.y, * pTMapN = m_szTMapName; }
	CEvent*			GetEvent(void) {return &m_CEvtObj;}
	void			SetCopyNum(dbc::Short sCopyNum);
	dbc::Short		GetCopyNum(void) {return m_sMapCopyNum;}
	void			SetCopyPlyNum(dbc::Short sCopyNum) {m_sCopyPlyNum = sCopyNum;}
	dbc::Short		GetCopyPlyNum(void) {return m_sCopyPlyNum;}
	CMapEntryCopyCell*	AddCopy(CMapEntryCopyCell *pCCpyCell) {return m_LCopyInfo.Add(pCCpyCell);}
	CMapEntryCopyCell*	GetCopy(dbc::Short sCopyID) {return m_LCopyInfo.Get(sCopyID);}
	bool			ReleaseCopy(CMapEntryCopyCell *pCCpyCell) {return m_LCopyInfo.Del(pCCpyCell);}
	bool			ReleaseCopy(dbc::Long lCopyNO) {return m_LCopyInfo.Del(lCopyNO);}
	void			FreeEnti(void);

	void			SynCopyParam(dbc::Short sCopyID);
	void			SynCopyRun(dbc::Short sCopyID, dbc::Char chCdtType, dbc::Long chCdtVal);

protected:

private:
	dbc::Char	m_szMapName[MAX_MAPNAME_LENGTH];
	Point		m_SEntiPos;
	dbc::Long	m_lEntiID;
	dbc::Char	m_szTMapName[MAX_MAPNAME_LENGTH];
	CEvent		m_CEvtObj;
	Entity		*m_pCEnt;

	dbc::Short	m_sMapCopyNum;
	dbc::Short	m_sCopyPlyNum;
	CListArray<CMapEntryCopyCell>	m_LCopyInfo;

	void*		m_pPos;
};

// 
class	CDynMapEntry
{
public:
	CDynMapEntry() {m_LEntryList.Init();}
	~CDynMapEntry() {m_LEntryList.Free();}

	CDynMapEntryCell*	Add(CDynMapEntryCell *pCCell);
	bool	Del(CDynMapEntryCell *pCCell);
	bool	Del(dbc::cChar *cszTMapName);
	CDynMapEntryCell*	GetEntry(dbc::cChar *cszTMapN);
	void	AfterPlayerLogin(const char *cszName);

protected:

private:
	CResidentList<CDynMapEntryCell>	m_LEntryList;
};

extern CDynMapEntry g_CDMapEntry;

// 
extern void	g_SetTeamFightMapName(const char *cszMapName);

extern char	g_szTFightMapName[MAX_MAPNAME_LENGTH];	// 
//

#endif // MAPENTRY_H
