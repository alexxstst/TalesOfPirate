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
#include <MonRefRecordStore.h>
#include "SwitchMapRecord.h"
#include <SwitchMapRecordStore.h>
#include "EventRecord.h"
#include <NpcRecord.h>
#include <NpcRecordStore.h>
#include "Npc.h"

#include <memory>
#include <string>

class SubMap;

class CChaSpawn
{
public:
	CChaSpawn();
	virtual ~CChaSpawn();

	bool	Init(const char *szMapName, const char *szSpawnTable);
	long	Load(SubMap *pCMap);
	long	Reload();

	long	GetChaCount(void) {return m_lCount;}

private:
	std::string m_strMapName;
	std::unique_ptr<MonRefRecordStore> m_pStore;

	SubMap	*m_pCMap{nullptr};
	long	m_lCount{0};
};

class CMapSwitchEntitySpawn
{
public:
	CMapSwitchEntitySpawn();
	virtual ~CMapSwitchEntitySpawn();

	bool	Init(const char *szMapName, const char *szSpawnTable);
	long	Load(SubMap *pCMap);
	long	Reload();

private:
	std::string m_strMapName;
	std::unique_ptr<SwitchMapRecordStore> m_pStore;
	SubMap	*m_pCMap{nullptr};
};

class CNpcSpawn
{
public:
	CNpcSpawn();
	~CNpcSpawn();

	bool	Init(const char *szMapName, const char *szSpawnTable);
	long	Load( SubMap& submap );
	long	Reload();
	void	Clear();
	CNpcRecord* GetNpcInfo( USHORT sNpcID );

	// NPC
	BOOL	SummonNpc( const char szNpc[], USHORT sAreaID, USHORT sTime );
	mission::CNpc* FindNpc( const char szName[] );

protected:
	std::string m_strMapName;
	std::unique_ptr<NpcRecordStore> m_pNpcStore;

	// NPC
	mission::CNpc*	m_NpcList[ROLE_MAXNUM_MAPNPC];
	USHORT			m_sNumNpc;

};

#endif // ENTITYSPAWN_H
