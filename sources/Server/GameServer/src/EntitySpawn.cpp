//=============================================================================
// FileName: EntitySpawn.cpp
// Creater: ZhangXuedong
// Date: 2004.09.10
// Comment: CChaSpawn class
//=============================================================================
#include "stdafx.h"
#include "EntitySpawn.h"
#include "excp.h"
#include "Character.h"
#include "GameCommon.h"
#include "GameAppNet.h"
#include "SubMap.h"
#include "CompCommand.h"
#include "GameApp.h"
#include "NPC.h"

#include <string>

extern const char* GetResPath(const char* pszRes);

_DBC_USING

//=============================================================================
CChaSpawn::CChaSpawn() = default;

CChaSpawn::~CChaSpawn() = default;

bool CChaSpawn::Init(const char *szMapName, const char *szSpawnTable)
{
	m_pCMap = nullptr;

	if (szMapName == nullptr || szMapName[0] == '\0' || szSpawnTable == nullptr) {
		THROW_EXCP(excpArr, RES_STRING(GM_ENTITYSPAWN_CPP_00001));
	}

	m_strMapName = szMapName;
	m_lCount = 0;

	m_pStore = std::make_unique<MonRefRecordStore>();

	std::string txtPath = std::string(GetResPath(szSpawnTable)) + ".txt";
	return m_pStore->Load(txtPath.c_str());
}

long CChaSpawn::Load(SubMap *pCMap)
{
	m_pCMap = pCMap;
	m_lCount = 0;

	if (!m_pStore) {
		ToLogService("errors", LogLevel::Error,
					 "CChaSpawn::Load: store is null (map='{}')", m_strMapName);
		return 0;
	}

	const Rect& area = pCMap->GetRange();
	bool reachedCap = false;

	m_pStore->ForEach([&](CMonRefRecord& rec) {
		if (reachedCap) return;

		CMonRefRecord* pMonRefRecord = &rec;
		long lNum = 0;

		for (int j = 0; j < defMAX_REGION_MONSTER_TYPE; j++) {
			for (int k = 0; k < pMonRefRecord->lMonster[j][1]; k++) {
				short sAngle = pMonRefRecord->sAngle;
				if (sAngle == -1) {
					sAngle = rand() % 360;
				}

				long l_x, l_y;
				if (pMonRefRecord->SRegion[0].x != pMonRefRecord->SRegion[1].x) {
					long lRand = rand();
					long lSub  = pMonRefRecord->SRegion[1].x - pMonRefRecord->SRegion[0].x;
					long lBase = (lSub / RAND_MAX > 0) ? (lRand % (lSub / RAND_MAX + 1) * RAND_MAX) : 0;
					l_x = lBase + lRand % (lSub - lBase) + pMonRefRecord->SRegion[0].x;
				}
				else {
					l_x = pMonRefRecord->SRegion[0].x;
				}
				if (pMonRefRecord->SRegion[0].y != pMonRefRecord->SRegion[1].y) {
					long lRand = rand();
					long lSub  = pMonRefRecord->SRegion[1].y - pMonRefRecord->SRegion[0].y;
					long lBase = (lSub / RAND_MAX > 0) ? (lRand % (lSub / RAND_MAX + 1) * RAND_MAX) : 0;
					l_y = lBase + lRand % (lSub - lBase) + pMonRefRecord->SRegion[0].y;
				}
				else {
					l_y = pMonRefRecord->SRegion[0].y;
				}
				Point l_pos = {l_x, l_y};

				CCharacter* pCCha = pCMap->ChaSpawn(
					pMonRefRecord->lMonster[j][0], enumCHACTRL_NONE, sAngle, &l_pos);
				if (pCCha) {
					pCCha->SetResumeTime(pMonRefRecord->lMonster[j][3] * 1000);
					m_lCount++;
					lNum++;
					if (m_lCount >= g_Config.m_nMaxCha) {
						ToLogService("common", LogLevel::Warning,
									 "Character count reached maximum, stopping spawn");
						reachedCap = true;
						return;
					}
				}
				else {
					ToLogService("errors", LogLevel::Error,
						"character born error, born information: map {}[{}, {}], "
						"character hatch list number {}, character list number {}, born position[{}, {}]",
						pCMap->GetName(), area.width(), area.height(),
						pMonRefRecord->Id, pMonRefRecord->lMonster[j][0], l_pos.x, l_pos.y);
				}
			}
		}
		ToLogService("common", "entry {} character number:\t{}", pMonRefRecord->Id, lNum);
	});

	return 1;
}

long CChaSpawn::Reload()
{
	// 
	return 0;
}

//=============================================================================
CMapSwitchEntitySpawn::CMapSwitchEntitySpawn() = default;

CMapSwitchEntitySpawn::~CMapSwitchEntitySpawn() = default;

bool CMapSwitchEntitySpawn::Init(const char *szMapName, const char *szSpawnTable)
{
	m_pCMap = nullptr;

	if (szMapName == nullptr || szMapName[0] == '\0' || szSpawnTable == nullptr) {
		THROW_EXCP(excpArr, RES_STRING(GM_ENTITYSPAWN_CPP_00005));
	}

	m_strMapName = szMapName;
	m_pStore = std::make_unique<SwitchMapRecordStore>();

	std::string txtPath = std::string(GetResPath(szSpawnTable)) + ".txt";
	return m_pStore->Load(txtPath.c_str());
}

long CMapSwitchEntitySpawn::Load(SubMap *pCMap)
{
	m_pCMap = pCMap;

	if (!m_pStore) {
		ToLogService("errors", LogLevel::Error,
					 "CMapSwitchEntitySpawn::Load: store is null (map='{}')", m_strMapName);
		return 0;
	}

	SItemGrid SItemCont;
	m_pStore->ForEach([&](CSwitchMapRecord& rec) {
		CSwitchMapRecord* pCSwitchMapRecord = &rec;

		SItemCont.sID = (short)pCSwitchMapRecord->lEntityID;
		SItemCont.sNum = 1;
		SItemCont.SetDBParam(-1, 0);
		SItemCont.chForgeLv = 0;
		SItemCont.SetInstAttrInvalid();

		CEvent CEvtCont;
		CEvtCont.SetID((short)pCSwitchMapRecord->lEventID);
		CEvtCont.SetTouchType(enumEVENTT_RANGE);
		CEvtCont.SetExecType(enumEVENTE_SMAP_ENTRY);
		CEvtCont.SetTableRec(pCSwitchMapRecord);  // адрес стабилен: store не перевыделяет vector после Load

		CItem* pCItem = pCMap->ItemSpawn(
			&SItemCont,
			pCSwitchMapRecord->SEntityPos.x, pCSwitchMapRecord->SEntityPos.y,
			enumITEM_APPE_NATURAL, 0,
			g_pCSystemCha->GetID(), g_pCSystemCha->GetHandle(),
			-1, -1, &CEvtCont);
		if (pCItem) {
			pCItem->SetOnTick(0);
		}
	});

	return 1;
}

long CMapSwitchEntitySpawn::Reload()
{
	return 0;
}

//=============================================================================
CNpcSpawn::CNpcSpawn() = default;

CNpcSpawn::~CNpcSpawn()
{
	Clear();
}

bool CNpcSpawn::Init(const char *szMapName, const char *szSpawnTable)
{
	if (szMapName == nullptr || szMapName[0] == '\0' || szSpawnTable == nullptr) {
		char szTemp[128];
		sprintf(szTemp, RES_STRING(GM_ENTITYSPAWN_CPP_00007), szSpawnTable ? szSpawnTable : "", 0);
		THROW_EXCP(excpArr, szTemp);
	}

	m_strMapName = szMapName;

	m_pNpcStore = std::make_unique<NpcRecordStore>();

	std::string txtPath = std::string(GetResPath(szSpawnTable)) + ".txt";
	return m_pNpcStore->Load(txtPath.c_str());
}

void CNpcSpawn::Clear()
{
	m_pNpcStore.reset();
}

mission::CNpc* CNpcSpawn::FindNpc( const char szName[] )
{
	for( int i = 0; i < m_sNumNpc; i++ )
	{
		if( strcmp( m_NpcList[i]->GetNpcName(), szName ) == 0 )
			return m_NpcList[i];
	}
	return NULL;
}

long CNpcSpawn::Load( SubMap& submap )
{
	// NPC
	memset( m_NpcList, 0, sizeof(mission::CNpc*)*ROLE_MAXNUM_MAPNPC );
	m_sNumNpc = 0;

	ToLogService("common", "Loading NPCs for map '{}'... ", m_strMapName);

	if (!m_pNpcStore) {
		ToLogService("errors", LogLevel::Error, "CNpcSpawn::Load: store is null (map='{}')", m_strMapName);
		return 0;
	}

	bool hasError = false;
	m_pNpcStore->ForEach([&](CNpcRecord& rec) {
		CNpcRecord* pNpcRecord = &rec;

		CChaRecord* pCharRecord = GetChaRecordInfo(pNpcRecord->sCharID);
		if (pCharRecord == nullptr) {
			hasError = true;
			C_PRINT("\nerror: NPC %d model %d unfound!", (int)pNpcRecord->Id, pNpcRecord->sCharID);
			ToLogService("errors", LogLevel::Error,
						 "initialization map errornot find appoint ID roll attribute informationID = {}",
						 pNpcRecord->sCharID);
			return;
		}

		switch (pNpcRecord->sNpcType) {
			case mission::CNpc::TALK: {
				mission::CTalkNpc* pTalk = g_pGameApp->GetNewTNpc();
				if (pTalk == nullptr) {
					break;
				}
				if (pTalk->Load(*pNpcRecord, *pCharRecord) == FALSE) {
					pTalk->Free();
					return;
				}
				Square SShape = {{pNpcRecord->dwxPos0, pNpcRecord->dwyPos0}, pCharRecord->sRadii};
				if (!submap.Enter(&SShape, pTalk)) {
					pTalk->Free();
					return;
				}
				if (m_sNumNpc < ROLE_MAXNUM_MAPNPC) {
					m_NpcList[m_sNumNpc++] = pTalk;
				}
			} break;
			default:
				break;
		}
	});

	if (!hasError) {
		C_PRINT("success!\n");
	}
	else {
		ToLogService("common", "");
	}
	return 0;
}

long CNpcSpawn::Reload()
{
	return 0;
}

CNpcRecord* CNpcSpawn::GetNpcInfo( USHORT sNpcID )
{
	if (m_pNpcStore) {
		return m_pNpcStore->Get(static_cast<int>(sNpcID));
	}
	return nullptr;
}

BOOL CNpcSpawn::SummonNpc( const char szNpc[], USHORT sAreaID, USHORT sTime )
{
	for( USHORT i = 0; i < m_sNumNpc; i++ )
	{
		if( m_NpcList[i] && m_NpcList[i]->GetIslandID() == sAreaID && strcmp( m_NpcList[i]->GetName(), szNpc ) == 0 )
		{
			m_NpcList[i]->Summoned( sTime );
			//return TRUE;
		}
	}
	return FALSE;
}
