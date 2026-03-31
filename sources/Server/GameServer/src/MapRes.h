//=============================================================================
// FileName: MapRes.h
// Creater: ZhangXuedong
// Date: 2005.09.05
// Comment: Map Resource
//=============================================================================

#ifndef MAPRES_H
#define MAPRES_H

#include <tchar.h>
#include <stdio.h>
#include "TerrainAttrib.h"
#include "dbccommon.h"
#include "point.h"
#include "util2.h"
#include "Timer.h"
#include "Config.h"
#include <fstream>

class CChaSpawn;
class CMapSwitchEntitySpawn;
class CNpcSpawn;
class SubMap;

namespace mission
{
	class CNpc;
}

class CAreaData
{
public:
	CAreaData();
	~CAreaData();
	dbc::Long	Init(_TCHAR *chFile);
	void	Free();
	bool	GetUnitAttr(dbc::Short sUnitX, dbc::Short sUnitY, dbc::uShort &usAttribute);
	bool	GetUnitSize(dbc::Short *psWidth, dbc::Short *psHeight);
	bool	GetUnitIsland(dbc::Short sUnitX, dbc::Short sUnitY, dbc::uChar &uchIsland);
	dbc::Short	GetWidth() {return m_sUnitCountX;}
	dbc::Short	GetHeight() {return m_sUnitCountY;}
	bool	IsValidPos(dbc::Short sUnitX, dbc::Short sUnitY) {if (sUnitX < 0 || sUnitX >= GetWidth() || sUnitY < 0 || sUnitY >= GetHeight()) return false; return true;}

protected:

private:
	dbc::Short	m_sUnitCountX;	// 
	dbc::Short	m_sUnitCountY;	// 
	dbc::Short	m_sUnitWidth;	// 
	dbc::Short	m_sUnitHeight;	// 

	int			m_nID;

};

#define	defMAX_MAP_COPY_NUM	3000
#define defMAPCOPY_INFO_PARAM_NUM	16


enum EMapEntryStep
{
	enumMAPENTRY_CREATE,	// 
	enumMAPENTRY_DESTROY,	// 
	enumMAPENTRY_SUBPLAYER,	// 
	enumMAPENTRY_SUBCOPY,	// 
	enumMAPENTRY_RETURN,	// 
	enumMAPENTRY_COPYPARAM,	// 
	enumMAPENTRY_COPYRUN,	// 
};

enum EMapEntryOptRet
{
	enumMAPENTRYO_CREATE_SUC,		// 
	enumMAPENTRYO_DESTROY_SUC,		// 
	enumMAPENTRYO_COPY_CLOSE_SUC,	// 
};

enum EMapState
{
	enumMAP_STATE_OPEN,			// 
	enumMAP_STATE_CLOSE,		// 
	enumMAP_STATE_ASK_CLOSE,	// 
};

enum EMapEntryState
{
	enumMAPENTRY_STATE_ASK_OPEN,	// 
	enumMAPENTRY_STATE_OPEN,		// 
	enumMAPENTRY_STATE_CLOSE,		// 
	enumMAPENTRY_STATE_ASK_CLOSE,	// 
	enumMAPENTRY_STATE_CLOSE_SUC,	// 
};

enum EMapType // 
{
	enumMAPTYPE_NORMAL			= 1, // 
	enumMAPTYPE_GUILD_FIGHT		= 2, // 
	enumMAPTYPE_TEAM_FIGHT		= 3, // 
};

enum EMapCopyStartType // 
{
	enumMAPCOPY_START_NOW		= 1, // 
	enumMAPCOPY_START_PLAYER	= 2, // 
	enumMAPCOPY_START_CONDITION	= 3, // 
};

enum EMapCopyStartCdtType // 
{
	enumMAPCOPY_START_CDT_UNKN,		// 
	enumMAPCOPY_START_CDT_PLYNUM,	// 
};

class CMapRes
{
public:
	enum{m_eyeshotwidth = 2};

	CMapRes();
	virtual ~CMapRes();

	bool		Init(void);
	bool		IsValid(void) {return m_bValid;}
	bool		IsOpen(void) {return m_bValid && m_chState == enumMAP_STATE_OPEN;}
	bool		SetCopyNum(dbc::Short sCpyNum);
	dbc::Short	GetCopyNum(void) {return m_sMapCpyNum;}
	SubMap*		GetCopy(dbc::Short sCpyNO = -1);
	void		SetCopyPlyNum(dbc::Short sPlyNum) {m_sCopyPlyNum = sPlyNum;}
	dbc::Short	GetCopyPlyNum(void) {return m_sCopyPlyNum;}
	bool		InitCtrl(void);

	bool		Open(void);
	bool		Close(void);
	bool		OpenEntry(void);
	bool		CloseEntry(void);
	bool		CreateEntry(void);
	bool		DestroyEntry(void);
	void		Run(DWORD dwCurTime);
	bool		CopyClose(dbc::Short sCopyNO = -1);
	bool		CopyNotice(const char *szString, dbc::Short sCopyNO = -1);
	bool		ReleaseCopy(dbc::Short sCopyNO = 0);

	bool		SetEntryMapName(dbc::cChar *szMapName);
	void		CheckEntryState(dbc::Char chState);
	bool		SubEntryPlayer(dbc::Short sCopyNO);
	bool		SubEntryCopy(dbc::Short sCopyNO);
	bool		HasDynEntry(void) {return strcmp(m_szEntryMapName, "") != 0 ? true : false;}
	void		SetCanSavePos(bool bCan = true) {m_bCanSavePos = bCan;}
	bool		CanSavePos(void) {return m_bCanSavePos;}
	void		SetCanPK(bool bCan = true) {m_bCanPK = bCan;}
	bool		CanPK(void) {return m_bCanPK;}
	void		SetCanTeam(bool bCan = true) {m_bCanTeam = bCan;}
	void		SetCanStall(bool bCan = true){m_bCanStall = bCan;}//
	void		SetCanGuild(bool bCan = true) { m_bCanGuild = bCan; }
	void		SetGuildWar(bool bGuildWar) { m_bGuildWar = bGuildWar; }
	bool		CanGuildWar() { return m_bGuildWar; }
	bool		CanTeam(void) {return m_bCanTeam;}
	bool		CanStall(void) {return m_bCanStall;}//
	bool		CanGuild(void) { return m_bCanGuild; }
	void		SetType(dbc::Char chType = enumMAPTYPE_NORMAL) {m_chType = chType;}
	dbc::Char	GetType(void) {return m_chType;}
	void		SetCopyStartType(dbc::Char chStartType = enumMAPCOPY_START_PLAYER) {m_chCopyStartType = chStartType;}
	dbc::Char	GetCopyStartType(void) {return m_chCopyStartType;}
	void		SetCopyStartCondition(dbc::Char chType, dbc::Long lVal) {m_chCopyStartCdtType = chType; m_lCopyStartCdtVal = lVal;}
	dbc::Char	GetCopyStartCdtType(void) {return m_chCopyStartCdtType;}
	dbc::Long	GetCopyStartCdtVal(void) {return m_lCopyStartCdtVal;}

	void		SetName(dbc::cChar *cszName) {m_strMapName = cszName;}
	const char*	GetName(void) {return m_strMapName.c_str();}
	const Rect&	GetRange(void) {return m_SRange;}
	BYTE		GetMapID() {return m_byMapID;}

	BOOL		SummonNpc( USHORT sAreaID, const char szNpc[], USHORT sTime );
	void		SetRepatriateDie(bool bRepatriate = true) {m_bRepatriateDie = bRepatriate;}
	bool		IsRepatriateDie(void) {return m_bRepatriateDie;}

	void		BeginGetUsedCopy(void) {m_sUsedCopySearch = 0;}
	SubMap*		GetNextUsedCopy(void);

	mission::CNpc*		FindNpc( const char szName[] );

	// 
	struct
	{
		dbc::cShort		m_csEyeshotCellWidth;
		dbc::cShort		m_csEyeshotCellHeight;
		dbc::Short		m_sEyeshotCellLin;
		dbc::Short		m_sEyeshotCellCol;
	};
	// 
	struct
	{
		dbc::cShort		m_csStateCellWidth;
		dbc::cShort		m_csStateCellHeight;
		dbc::Short		m_sStateCellLin;
		dbc::Short		m_sStateCellCol;
	};

	CAreaData			m_CTerrain;
	struct
	{
		CBlockData		m_CBlock;
		dbc::cShort		m_csBlockUnitWidth;
		dbc::cShort		m_csBlockUnitHeight;
	};

	struct
	{
		std::string	m_strMapName;
		Rect			m_SRange;
		CMapRes			*m_pCLeftMap, *m_pCTopMap, *m_pCRightMap, *m_pCBelowMap;
	};

	CChaSpawn				*m_pCMonsterSpawn;
	CMapSwitchEntitySpawn	*m_pCMapSwitchEntitySpawn;
	CNpcSpawn*				m_pNpcSpawn;

	// 
	struct
	{
		dbc::Char	m_szEntryMapName[MAX_MAPNAME_LENGTH];
		Point		m_SEntryPos;
		dbc::Char	m_chEntryState;		// EMapEntryState

		time_t		m_tEntryFirstTm;	// 
		time_t		m_tEntryTmDis;		// 
		time_t		m_tEntryOutTmDis;	// 
		time_t		m_tMapClsTmDis;		// 

		FILE		*m_pfEntryFile;			// 
	};

	struct{
		char m_szObstacleFile[_MAX_PATH + _MAX_FNAME];
		char m_szSectionFile[_MAX_PATH + _MAX_FNAME];
		char m_szMonsterSpawnFile[_MAX_PATH + _MAX_FNAME];
		char m_szNpcSpawnFile[_MAX_PATH + _MAX_FNAME];
		char m_szMapSwitchFile[_MAX_PATH + _MAX_FNAME];
		char m_szMonsterCofFile[_MAX_PATH + _MAX_FNAME];
		char m_szCtrlFile[_MAX_PATH + _MAX_FNAME];
		char m_szEntryFile[_MAX_PATH + _MAX_FNAME];
	};

protected:

private:
	bool		m_bValid;	// 
	dbc::Char	m_chState;	// EMapState

	BYTE	m_byMapID; // ID

	struct
	{
		bool	m_bCanSavePos;	// 
		bool	m_bCanPK;		// PK
		bool	m_bCanTeam;		// 
		bool	m_bCanStall;	// 
		bool	m_bCanGuild;
	};

	CTimer	m_timeMgr;
	CTimer	m_timeRun;

	SubMap		*m_pCMapCopy;
	dbc::Short	m_sMapCpyNum;
	dbc::Short	m_sCopyPlyNum;
	dbc::Char	m_chType;

	struct
	{
		dbc::Char	m_chCopyStartType;		//  EMapCopyStartType
		dbc::Char	m_chCopyStartCdtType;	// 
		dbc::Long	m_lCopyStartCdtVal;	// 
	};

	bool	m_bRepatriateDie;	// 

	dbc::Short	m_sUsedCopySearch;

	bool		m_bGuildWar;

};

// IDID,
class CMapID
{
public:
	CMapID();
	~CMapID();
	
	void	Clear();

	BOOL	AddInfo( const char szMap[], BYTE byID );
	BOOL	GetID( const char szMap[], BYTE& byID );	
	BOOL	SetMap( BYTE byID, CMapRes* pMap );
	CMapRes* GetMap( BYTE byID );

private:
	struct MAP_INFO
	{
		char		szMap[MAX_MAPNAME_LENGTH];
		BYTE		byID;
		CMapRes*	pMap;
	};

	std::vector<MAP_INFO> m_MapInfo;
	BYTE	 m_byNumMap;
};

extern CMapID g_MapID;
extern const char g_cchLogMapEntry;

#endif
