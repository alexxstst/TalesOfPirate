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

extern BOOL LoadTable(CRawDataSet *pTable, const char*);

_DBC_USING

//=============================================================================
CChaSpawn::CChaSpawn()
{
	m_lRegionNum = 0;
	m_pSMonInfo = 0;
	m_pCMonRefRecordSet = 0;
}

CChaSpawn::~CChaSpawn()
{
	m_lRegionNum = 0;
	if (m_pSMonInfo)
		delete [] m_pSMonInfo;
	if (m_pCMonRefRecordSet)
		delete m_pCMonRefRecordSet;
}

bool CChaSpawn::Init(char *szSpawnTable, long lRegionNum)
{
	m_pCMap = 0;

	if (lRegionNum <= 0)
		//THROW_EXCP(excpArr, "ˢ�ֵ�������Ŀ����");
		THROW_EXCP(excpArr, RES_STRING(GM_ENTITYSPAWN_CPP_00001));
	strcpy(m_szSpawnTable, szSpawnTable);
	m_lRecordNum = lRegionNum;

	m_pCMonRefRecordSet = new CMonRefRecordSet(0, lRegionNum);
	if (!m_pCMonRefRecordSet)
		//THROW_EXCP(excpMem,"ˢ�ּ�¼����������з����ڴ�ʧ��");
		THROW_EXCP(excpMem,RES_STRING(GM_ENTITYSPAWN_CPP_00002));
	if (!LoadTable(m_pCMonRefRecordSet, m_szSpawnTable))
		return false;

	m_lRegionNum = lRegionNum;
	m_pSMonInfo = new SMonInfo[m_lRegionNum];
	if (!m_pSMonInfo)
		//THROW_EXCP(excpMem,"ˢ�ֶ���������з����ڴ�ʧ��");
		THROW_EXCP(excpMem,RES_STRING(GM_ENTITYSPAWN_CPP_00003));

	memset(m_pSMonInfo, 0, sizeof(SMonInfo) * m_lRegionNum);
	m_lCount = 0;

	return true;
}

long CChaSpawn::Load(SubMap *pCMap)
{
	m_pCMap = pCMap;

	long	lRet = 1;
	CMonRefRecord	*pMonRefRecord;

	m_lCount = 0;
	
	//Char	szSpawnError[512] = "��ɫ��������";
	Char szSpawnError[512] = "Cha born error";

	Char	szMap[512];
	sprintf(szMap, "spawn mum %s", pCMap->GetName());
	Long	lNum;
	const Rect	&area = pCMap->GetRange();
	//LG(szSpawnError, "��ɫ��������Ŀ���ԭ��1����ʼ����λ�÷Ƿ���2���ڽ�ɫ�����Ҳ�����Ӧ�ļ�¼��3���ڳ���λ��120�׷�Χ��û���ҵ��ʺϸý�ɫ����������\n\n\n");
	for (int i = 0; i < m_lRegionNum; i++)
	{
		pMonRefRecord = GetMonRefRecordInfo(i + 1);
		if (pMonRefRecord == NULL)
			continue;
		lNum = 0;
		for (int j = 0; j < defMAX_REGION_MONSTER_TYPE; j++)
		{
			for (int k = 0; k < pMonRefRecord->lMonster[j][1]; k++)
			{
				short sAngle = pMonRefRecord->sAngle;
				if (sAngle == -1)
					sAngle = rand() % 360;
				long l_x, l_y;
				long lSub;
				long lRand;
				long lBase;
				if (pMonRefRecord->SRegion[0].x != pMonRefRecord->SRegion[1].x)
				{
					lRand = rand();
					lSub = pMonRefRecord->SRegion[1].x - pMonRefRecord->SRegion[0].x;
					if (lSub / RAND_MAX > 0)
						lBase = lRand % (lSub / RAND_MAX + 1) * RAND_MAX;
					else
						lBase = 0;
					l_x = (lBase +  lRand % (lSub - lBase) + pMonRefRecord->SRegion[0].x);
				}
				else
					l_x = pMonRefRecord->SRegion[0].x;
				if (pMonRefRecord->SRegion[0].y != pMonRefRecord->SRegion[1].y)
				{
					lRand = rand();
					lSub = pMonRefRecord->SRegion[1].y - pMonRefRecord->SRegion[0].y;
					if (lSub / RAND_MAX > 0)
						lBase = lRand % (lSub / RAND_MAX + 1) * RAND_MAX;
					else
						lBase = 0;
					l_y = (lBase +  lRand % (lSub - lBase) + pMonRefRecord->SRegion[0].y);
				}
				else
					l_y = pMonRefRecord->SRegion[0].y;
				Point l_pos = {l_x, l_y};

				CCharacter	*pCCha;
				if (pCCha = pCMap->ChaSpawn(pMonRefRecord->lMonster[j][0], enumCHACTRL_NONE, sAngle, &l_pos))
				{
					pCCha->SetResumeTime(pMonRefRecord->lMonster[j][3] * 1000);
					m_lCount++;
					lNum++;
					if(m_lCount >= g_Config.m_nMaxCha)
					{
						//LG(szMap, "msg�����������Ŀ��������ֵ��������\n");
						ToLogService("common", LogLevel::Warning, "Character count reached maximum, stopping spawn");
						return 1;
					}
				}
				else
					//LG(szSpawnError, "��ɫ�������󣬳�����Ϣ����ͼ %s[%d, %d]����ɫ��������� %d����ɫ����� %d������λ��[%d, %d]\n", pCMap->GetName(), area.width(), area.height(), i + 1, pMonRefRecord->lMonster[j][0], l_pos.x, l_pos.y);
					ToLogService("errors", LogLevel::Error, "character born error, born information: map {}[{}, {}], character hatch list number {}, character list number {}, born position[{}, {}]",
							pCMap->GetName(), area.width(), area.height(), i + 1, pMonRefRecord->lMonster[j][0], l_pos.x, l_pos.y);
			}
		}
		//LG(szMap, "��Ŀ %d �Ľ�ɫ����\t%d\n", i + 1, lNum);
		ToLogService("common", "entry {} character number:\t{}", i + 1, lNum);
	}
	return lRet;
}

long CChaSpawn::Reload()
{
	// ���������ͼ�ϵ����й���
	return 0;
}

//=============================================================================
CMapSwitchEntitySpawn::CMapSwitchEntitySpawn()
{
	m_lRecordNum = 0;
	m_pCSwitchMapRecSet = 0;
}

CMapSwitchEntitySpawn::~CMapSwitchEntitySpawn()
{
	m_lRecordNum = 0;
	if (m_pCSwitchMapRecSet)
		delete m_pCSwitchMapRecSet;
}

bool CMapSwitchEntitySpawn::Init(char *szSpawnTable, long lRecordNum)
{
	m_pCMap = 0;

	if (lRecordNum <= 0)
		//THROW_EXCP(excpArr, "��¼��Ŀ����");
		THROW_EXCP(excpArr, RES_STRING(GM_ENTITYSPAWN_CPP_00005));
	strcpy(m_szSpawnTable, szSpawnTable);
	m_lRecordNum = lRecordNum;

	m_pCSwitchMapRecSet = new CSwitchMapRecordSet(0, lRecordNum);
	if (!m_pCSwitchMapRecSet)
		//THROW_EXCP(excpMem,"��ͼ�л���¼����������з����ڴ�ʧ��");
		THROW_EXCP(excpMem,RES_STRING(GM_ENTITYSPAWN_CPP_00006));
	if (!LoadTable(m_pCSwitchMapRecSet, m_szSpawnTable))
		return false;

	return true;
}

long CMapSwitchEntitySpawn::Load(SubMap *pCMap)
{
	m_pCMap = pCMap;

	long	lRet = 1;
	CSwitchMapRecord	*pCSwitchMapRecord;

	{
		SItemGrid	SItemCont;
		CItem		*pCItem;
		for (int i = 0; i < m_lRecordNum; i++)
 		{
			pCSwitchMapRecord = GetSwitchMapRecordInfo(i + 1);
			if (pCSwitchMapRecord == NULL)
				continue;
			SItemCont.sID = (short)pCSwitchMapRecord->lEntityID;
			SItemCont.sNum = 1;
			SItemCont.SetDBParam(-1, 0);
			SItemCont.chForgeLv = 0;
			SItemCont.SetInstAttrInvalid();
			CEvent	CEvtCont;
			CEvtCont.SetID((short)pCSwitchMapRecord->lEventID);
			CEvtCont.SetTouchType(enumEVENTT_RANGE);
			CEvtCont.SetExecType(enumEVENTE_SMAP_ENTRY);
			CEvtCont.SetTableRec(pCSwitchMapRecord);
			pCItem = pCMap->ItemSpawn(&SItemCont, pCSwitchMapRecord->SEntityPos.x, pCSwitchMapRecord->SEntityPos.y, enumITEM_APPE_NATURAL, 0, g_pCSystemCha->GetID(), g_pCSystemCha->GetHandle(), -1, -1,
				&CEvtCont);
			if (pCItem)
				pCItem->SetOnTick(0);
		}
	}
	return lRet;
}

long CMapSwitchEntitySpawn::Reload()
{
	return 0;
}

//=============================================================================
CNpcSpawn::CNpcSpawn()
{
	m_pNpcRecordSet = 0;
}

CNpcSpawn::~CNpcSpawn()
{
	Clear();	
}

bool CNpcSpawn::Init( char *szSpawnTable, long lRecordNum )
{
	if( lRecordNum <= 0 || szSpawnTable == NULL ) 
	{
		char szTemp[128];
		sprintf( szTemp, RES_STRING(GM_ENTITYSPAWN_CPP_00007), szSpawnTable, lRecordNum );
		THROW_EXCP( excpArr, szTemp );
	}

	strcpy( m_szSpawnTable, szSpawnTable );
	m_lRecordNum = lRecordNum;

	m_pNpcRecordSet = new CNpcRecordSet( 0, lRecordNum );
	if( !m_pNpcRecordSet )
	{
		//THROW_EXCP( excpMem, "NPC��¼�����ݹ�������з����ڴ�ʧ�ܣ�" );
		THROW_EXCP( excpMem, RES_STRING(GM_ENTITYSPAWN_CPP_00008) );
	}
	if( !LoadTable(m_pNpcRecordSet, m_szSpawnTable ))
		return false;

	return true;
}

void CNpcSpawn::Clear()
{
	SAFE_DELETE( m_pNpcRecordSet );
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
	// ��ʼ���õ�ͼװ�ص�NPCָ���б�
	memset( m_NpcList, 0, sizeof(mission::CNpc*)*ROLE_MAXNUM_MAPNPC );
	m_sNumNpc = 0;

	ToLogService("common", "Loading [{}] files... ", m_szSpawnTable );

	bool hasError = false;
	for( int i = 0; i < m_lRecordNum; i++ )
	{
		CNpcRecord* pNpcRecord  = (CNpcRecord*)m_pNpcRecordSet->GetRawDataInfo( i );
		if( pNpcRecord == NULL ) {
			continue;
		}

		CChaRecord* pCharRecord = GetChaRecordInfo( pNpcRecord->sCharID );
		if( pCharRecord == NULL ) {
			hasError = true;
			C_PRINT("\nerror: NPC %d model %d unfound!", i, pNpcRecord->sCharID );
			//LG( "npcinit_error", "��ͼ��ʼ������δ�ҵ�ָ��ID�Ľ�ɫ������Ϣ��ID = %d", pNpcRecord->sCharID );
			ToLogService("errors", LogLevel::Error, "initialization map error��not find appoint ID roll attribute information��ID = {}", pNpcRecord->sCharID );
			continue;
		}
		
		switch( pNpcRecord->sNpcType )
		{
		case mission::CNpc::TALK:
			{
				mission::CTalkNpc* pTalk = g_pGameApp->GetNewTNpc();
				if( pTalk == NULL ) 
				{
					break;
				}
				if( pTalk->Load( *pNpcRecord, *pCharRecord ) == FALSE )
				{
					pTalk->Free();
					continue;
				}
				// 
				Square SShape = {{pNpcRecord->dwxPos0, pNpcRecord->dwyPos0}, pCharRecord->sRadii};
				if( !submap.Enter( &SShape, pTalk ) )
				{
					pTalk->Free();
					continue;
				}
				if( m_sNumNpc < ROLE_MAXNUM_MAPNPC )
				{
					m_NpcList[m_sNumNpc++] = pTalk;
				}
			}
			break;
		default:
			break;
		}

	}
	if (!hasError)
	{
		C_PRINT("success!\n");
	}
	else
	{
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
	if( m_pNpcRecordSet )
	{
		(CNpcRecord*)m_pNpcRecordSet->GetRawDataInfo( sNpcID );
	}

	return NULL;
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