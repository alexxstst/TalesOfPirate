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
	CMapEntryCopyCell(dbc::Short sMaxPlyNum = 0, dbc::Short sCurPlyNum = 0);

	void		SetMaxPlyNum(dbc::Short sPlyNum);
	dbc::Short	GetMaxPlyNum(void);
	void		SetCurPlyNum(dbc::Short sPlyNum);
	dbc::Short	GetCurPlyNum(void);
	bool		AddCurPlyNum(dbc::Short sAddNum);
	bool		HasFreePlyCount(dbc::Short sRequestNum);

	dbc::Long	GetParam(dbc::Char chParamID);
	bool		SetParam(dbc::Char chParamID, dbc::Long lParamVal);

	void		WriteParamPacket(net::WPacket &pk);

	void		SetPosID(dbc::Long lPosID);
	dbc::Long	GetPosID(void);

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
	CDynMapEntryCell();

	void		    SetPos(void *pPos);
	void*		    GetPos(void);

	void		    SetMapName(dbc::cChar *cszMapName);
	dbc::cChar*	    GetMapName(void) const;
	void		    SetTMapName(dbc::cChar *cszTMapName);
	dbc::cChar*	    GetTMapName(void) const;
	const Point*    GetEntiPos(void) const;
	void            SetEntiPos(const Point* cpSPos);
	void            SetEntiPos(dbc::Long lPosX, dbc::Long lPosY);
	void		    SetEntiID(dbc::Long lEntiID);
	dbc::Long	    GetEntiID(void);
	void		    SetEventID(dbc::Long lEventID);
	dbc::Long	    GetEventID(void);
	void		    SetEventName(dbc::cChar *cszEventName);
	void		    SetEnti(Entity *pCEnt);
	void		    GetPosInfo(const char** pMapN, dbc::Long* lpPosX, dbc::Long* lpPosY, const char** pTMapN);
	CEvent*		    GetEvent(void);
	void		    SetCopyNum(dbc::Short sCopyNum);
	dbc::Short	    GetCopyNum(void);
	void		    SetCopyPlyNum(dbc::Short sCopyNum);
	dbc::Short	    GetCopyPlyNum(void);
	CMapEntryCopyCell* AddCopy(CMapEntryCopyCell *pCCpyCell);
	CMapEntryCopyCell* GetCopy(dbc::Short sCopyID);
	bool		    ReleaseCopy(CMapEntryCopyCell *pCCpyCell);
	bool		    ReleaseCopy(dbc::Long lCopyNO);
	void		    FreeEnti(void);

	void		    SynCopyParam(dbc::Short sCopyID);
	void		    SynCopyRun(dbc::Short sCopyID, dbc::Char chCdtType, dbc::Long chCdtVal);

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
	CDynMapEntry();
	~CDynMapEntry();

	CDynMapEntryCell*	Add(CDynMapEntryCell *pCCell);
	bool	Del(CDynMapEntryCell *pCCell);
	bool	Del(dbc::cChar *cszTMapName);
	CDynMapEntryCell*	GetEntry(dbc::cChar *cszTMapN);
	void	AfterPlayerLogin(const char *cszName);

private:
	CResidentList<CDynMapEntryCell>	m_LEntryList;
};

extern CDynMapEntry g_CDMapEntry;

//
extern void	g_SetTeamFightMapName(const char *cszMapName);

extern char	g_szTFightMapName[MAX_MAPNAME_LENGTH];	//
//

#endif // MAPENTRY_H
