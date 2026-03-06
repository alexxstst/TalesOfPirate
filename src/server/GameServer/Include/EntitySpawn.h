#pragma once

//=============================================================================
// FileName: EntitySpawn.h
// Creater: ZhangXuedong
// Date: 2004.09.10
// Comment: CChaSpawn class
//=============================================================================

#ifndef ENTITYSPAWN_H
#define ENTITYSPAWN_H

#include "GameAppNet.h"
#include "MonRefRecord.h"
#include "SwitchMapRecord.h"
#include "EventRecord.h"
#include <NpcRecord.h>
#include "Npc.h"

class SubMap;

class CChaSpawn
{
public:
	struct SMonInfo
	{
		long	lRegionID;
		long	lMonsterNum[defMAX_REGION_MONSTER_TYPE];
	};

	CChaSpawn();
	virtual ~CChaSpawn();

	bool	Init(char *szSpawnTable, long lRegionNum);
	long	Load(SubMap *pCMap);
	long	Reload();

	long	GetChaCount(void) {return m_lCount;}

protected:

private:
	long	m_lRegionNum;
	SMonInfo	*m_pSMonInfo;

	char	m_szSpawnTable[_MAX_FNAME];
	long	m_lRecordNum;
	CMonRefRecordSet	*m_pCMonRefRecordSet;

	SubMap	*m_pCMap;

	long	m_lCount;

};

class CMapSwitchEntitySpawn
{
public:
	CMapSwitchEntitySpawn();
	virtual ~CMapSwitchEntitySpawn();

	bool	Init(char *szSpawnTable, long lRecordNum);
	long	Load(SubMap *pCMap);
	long	Reload();

protected:

private:
	char	m_szSpawnTable[_MAX_FNAME];
	long	m_lRecordNum;
	SubMap	*m_pCMap;
	CSwitchMapRecordSet	*m_pCSwitchMapRecSet;

};

class CNpcSpawn
{
public:
	CNpcSpawn();
	~CNpcSpawn();

	bool	Init(char *szSpawnTable, long lRecordNum);
	long	Load( SubMap& submap );
	long	Reload();
	void	Clear();
	CNpcRecord* GetNpcInfo( USHORT sNpcID );
	
	// NPC
	BOOL	SummonNpc( const char szNpc[], USHORT sAreaID, USHORT sTime );
	mission::CNpc* FindNpc( const char szName[] );

protected:
	char	m_szSpawnTable[_MAX_FNAME];
	long	m_lRecordNum;
	CNpcRecordSet*	m_pNpcRecordSet;

	// NPC
	mission::CNpc*	m_NpcList[ROLE_MAXNUM_MAPNPC];
	USHORT			m_sNumNpc;

};

#endif // ENTITYSPAWN_H
