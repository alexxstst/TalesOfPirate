// Mission.cpp Created by knight-gongjian 2004.12.13.
//---------------------------------------------------------
#include "stdafx.h"   //add by alfred.shi 20080312



#include "Mission.h"
#include "Character.h"
#include "Script.h"
#include "RoleCommon.h"
#include "GameAppNet.h"
#include "Player.h"
#include "lua_gamectrl.h"

//---------------------------------------------------------
namespace mission
{

	//#define ROLE_DEBUG_INFO

	CCharMission::CCharMission()
	{T_B
	T_E}

	CCharMission::~CCharMission()
	{T_B

	T_E}

	void CCharMission::Initially()
	{T_B
		m_byNumTrigger = 0;
		m_byNumMission = 0;
		memset( m_Trigger, 0, sizeof(TRIGGER_DATA)*ROLE_MAXNUM_CHARTRIGGER );
		memset( m_Mission, 0, sizeof(MISSION_INFO)*ROLE_MAXNUM_MISSION );
		m_RoleRecord.Clear();

		memset( m_MissionState, 0, sizeof(MISSION_STATE)*ROLE_MAXNUM_INSIDE_NPCCOUNT );
		m_byStateIndex = 0;
		m_byNumState = 0;
		m_byNumGotoMap = 0;

		memset( m_MissionCount, 0, sizeof(RAND_MISSION_COUNT)*ROLE_MAXNUM_MISSIONCOUNT );
		m_byNumMisCount = 0;

		// 
		m_byOnline = 1;
	T_E}

	void CCharMission::Finally()
	{T_B
		// MisClear
		/*
		m_byNumTrigger = 0;
		m_byNumMission = 0;
		memset( m_Trigger, 0, sizeof(TRIGGER_DATA)*ROLE_MAXNUM_CHARTRIGGER );
		memset( m_Mission, 0, sizeof(MISSION_INFO)*ROLE_MAXNUM_MISSION );
		m_RoleRecord.Clear();

		memset( m_MissionState, 0, sizeof(MISSION_STATE)*ROLE_MAXNUM_INSIDE_NPCCOUNT );
		m_byStateIndex = 0;
		m_byNumState = 0;
		m_byNumGotoMap = 0;

		memset( m_MissionCount, 0, sizeof(RAND_MISSION_COUNT)*ROLE_MAXNUM_MISSIONCOUNT );
		m_byNumMisCount = 0;

		// 
		m_byOnline = 1;
		*/
	T_E}

	BOOL CCharMission::MisInit( char* pszBuf )
	{T_B
		int nEdition(-1), nData1(0), nData2(0), nData3(0), nData4(0), nData5(0), nData6(0), 
			nData7(0), nData8(0), nData9(0), nData10(0), nData11(0);
		char* pTemp = pszBuf;
		sscanf( pTemp, "%d,", &nEdition );
#ifdef ROLE_DEBUG_INFO
		printf( "\nCCharMission:Init: Misinfo edition code = %d\n", nEdition );
#endif
		pTemp = strstr( pTemp, "," );
		if( pTemp == NULL ) return TRUE;
		pTemp++;

		if( nEdition != ROLE_MIS_MISINFO_EDITION )
		{
#ifdef ROLE_DEBUG_INFO
			printf( "\nCCharMission:Init: Convert to new misinfo edition! editon code = %d\n", nEdition );
#endif
			return ConvertMissionInfo( pTemp, nEdition );
		}

		sscanf( pTemp, "%d,", &nData1 );
		m_byNumMission = (BYTE)nData1;
#ifdef ROLE_DEBUG_INFO
		printf( "CCharMission:Init: m_byNumMission = %d\n", m_byNumMission );
#endif
		pTemp = strstr( pTemp, "," );
		if( pTemp == NULL ) return TRUE;
		pTemp++;
		if( m_byNumMission > ROLE_MAXNUM_MISSION )
			return FALSE;
		for( int i = 0; i < m_byNumMission; i++ )
		{
			sscanf( pTemp, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,", &nData1, &nData2, &nData3, &nData4, 
				&nData5, &nData6, &nData7, &nData8, &nData9, &nData10, &nData11 );
			m_Mission[i].wRoleID = (WORD)nData1;
			m_Mission[i].byState = (BYTE)nData2;
			m_Mission[i].byMisType = (BYTE)nData3;
			m_Mission[i].byType = (BYTE)nData4;
			m_Mission[i].byLevel = (BYTE)nData5;
			m_Mission[i].wItem = (WORD)nData6;
			m_Mission[i].wParam1 = (WORD)nData7;
			m_Mission[i].wParam2 = (WORD)nData8;
			m_Mission[i].dwExp = (DWORD)nData9;
			m_Mission[i].dwMoney = (DWORD)nData10;
			m_Mission[i].byNumData = (BYTE)nData11;
#ifdef ROLE_DEBUG_INFO
			printf( "\nRole:ID[%d],State[%d], MisType[%d], Type[%d], Level[%d], Item[%d], Param[%d], Param[%d], Exp[%d], Money[%d], NumData[%d]",
				m_Mission[i].wRoleID, m_Mission[i].byState, m_Mission[i].byMisType, m_Mission[i].byType, m_Mission[i].byLevel, m_Mission[i].wItem, 
				m_Mission[i].wParam1, m_Mission[i].wParam2, m_Mission[i].dwExp, m_Mission[i].dwMoney,	m_Mission[i].byNumData );
#endif

			// 
			for( int n = 0; n < 11; n++  )
			{
				pTemp = strstr( pTemp, "," );
				if( pTemp == NULL ) return TRUE;
				pTemp++;
			}

			// 
			for( int j = 0; j < ROLE_MAXNUM_RAND_DATA; j++ )
			{
				sscanf( pTemp, "%d,%d,%d,%d,%d,%d,", &nData1, &nData2, &nData3, &nData4, &nData5, &nData6 );
				m_Mission[i].RandData[j].wParam1 = (WORD)nData1;
				m_Mission[i].RandData[j].wParam2 = (WORD)nData2;
				m_Mission[i].RandData[j].wParam3 = (WORD)nData3;
				m_Mission[i].RandData[j].wParam4 = (WORD)nData4;
				m_Mission[i].RandData[j].wParam5 = (WORD)nData5;
				m_Mission[i].RandData[j].wParam6 = (WORD)nData6;

#ifdef ROLE_DEBUG_INFO
				printf( "\n[RandData%d] %d %d %d %d %d %d", j,
					m_Mission[i].RandData[j].wParam1, 
					m_Mission[i].RandData[j].wParam2,
					m_Mission[i].RandData[j].wParam3,
					m_Mission[i].RandData[j].wParam4,
					m_Mission[i].RandData[j].wParam5,
					m_Mission[i].RandData[j].wParam6 );
#endif

				// 
				for( int n = 0; n < 6; n++  )
				{
					pTemp = strstr( pTemp, "," );
					if( pTemp == NULL ) return TRUE;
					pTemp++;
				}
			}

#ifdef ROLE_DEBUG_INFO
			printf( "\n" );
#endif

			// 
			for( int j = 0; j < ROLE_MAXNUM_FLAGSIZE; j++ )
			{
				sscanf( pTemp, "%d,", &nData1 );
				m_Mission[i].RoleInfo.szFlag[j] = (BYTE)nData1;
#ifdef ROLE_DEBUG_INFO
				printf( "%d ", m_Mission[i].RoleInfo.szFlag[j] );
#endif
				pTemp = strstr( pTemp, "," );
				if( pTemp == NULL )
					return TRUE;
				pTemp++;
			}
		}

//#ifdef ROLE_DEBUG_INFO
//		printf( "\nRecord start!\n" );
//#endif
//		for( int i = 0; i < ROLE_MAXNUM_RECORDSIZE; i++ )
//		{
//			sscanf( pTemp, "%d ", &nData1 );
//			m_RoleRecord.szID[i] = (BYTE)nData1;
//#ifdef ROLE_DEBUG_INFO
//			printf( "%d ", m_RoleRecord.szID[i] );
//#endif
//			pTemp = strstr( pTemp, " " );
//			if( pTemp == NULL ) 
//				return TRUE;
//			pTemp++;
//		}
#ifdef ROLE_DEBUG_INFO
		printf( "\n" );
#endif
		return TRUE;
	T_E}

	BOOL CCharMission::MisGetData( char* pszBuf, DWORD dwSize )
	{T_B
		sprintf( pszBuf, "%d,", ROLE_MIS_MISINFO_EDITION );
#ifdef ROLE_DEBUG_INFO
		printf( "\nCCharMission:GetData: MisInfo edition code = %d", ROLE_MIS_TRIGGER_EDITION );
#endif

		sprintf( pszBuf + strlen( pszBuf ), "%d,", m_byNumMission );
#ifdef ROLE_DEBUG_INFO
		printf( "\nCCharMission:GetData: m_byNumMission = %d\nMission start!", m_byNumMission );
#endif
		for( int i = 0; i < m_byNumMission; i++ )
		{
			sprintf( pszBuf + strlen( pszBuf ), "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,", 
				m_Mission[i].wRoleID,
				m_Mission[i].byState,
				m_Mission[i].byMisType, 
				m_Mission[i].byType,
				m_Mission[i].byLevel,
				m_Mission[i].wItem,
				m_Mission[i].wParam1,
				m_Mission[i].wParam2,
				m_Mission[i].dwExp,
				m_Mission[i].dwMoney,
				m_Mission[i].byNumData
				);
#ifdef ROLE_DEBUG_INFO
			printf( "\nRole:ID[%d],State[%d], MisType[%d], Type[%d], Level[%d], Item[%d], Param[%d], Param[%d], Exp[%d], Money[%d], NumData[%d]",
				m_Mission[i].wRoleID, m_Mission[i].byState, m_Mission[i].byMisType, m_Mission[i].byType, m_Mission[i].byLevel, m_Mission[i].wItem, 
				m_Mission[i].wParam1, m_Mission[i].wParam2, m_Mission[i].dwExp, m_Mission[i].dwMoney,	m_Mission[i].byNumData );
#endif

			// 
			for( int j = 0; j < ROLE_MAXNUM_RAND_DATA; j++ )
			{
				sprintf( pszBuf + strlen( pszBuf ), "%d,%d,%d,%d,%d,%d,", 
					m_Mission[i].RandData[j].wParam1, 
					m_Mission[i].RandData[j].wParam2,
					m_Mission[i].RandData[j].wParam3,
					m_Mission[i].RandData[j].wParam4,
					m_Mission[i].RandData[j].wParam5,
					m_Mission[i].RandData[j].wParam6 );
#ifdef ROLE_DEBUG_INFO
				printf( "\n[RandData%d] %d %d %d %d %d %d", j,
					m_Mission[i].RandData[j].wParam1, 
					m_Mission[i].RandData[j].wParam2,
					m_Mission[i].RandData[j].wParam3,
					m_Mission[i].RandData[j].wParam4,
					m_Mission[i].RandData[j].wParam5,
					m_Mission[i].RandData[j].wParam6 );
#endif
			}
#ifdef ROLE_DEBUG_INFO
			printf( "\n" );
#endif
			for( int j = 0; j < ROLE_MAXNUM_FLAGSIZE; j++ )
			{
				sprintf( pszBuf + strlen( pszBuf ), "%d,", m_Mission[i].RoleInfo.szFlag[j] );
#ifdef ROLE_DEBUG_INFO
				printf( "%d ", m_Mission[i].RoleInfo.szFlag[j] );
#endif
			}
		}

//#ifdef ROLE_DEBUG_INFO
//		printf( "\nRecord start!\n" );
//#endif
//		for( int i = 0; i < ROLE_MAXNUM_RECORDSIZE; i++ )
//		{
//			sprintf( pszBuf + strlen( pszBuf ), "%d ", m_RoleRecord.szID[i] );
//#ifdef ROLE_DEBUG_INFO
//			printf( "%d ", m_RoleRecord.szID[i] );
//#endif
//		}
#ifdef ROLE_DEBUG_INFO
		printf( "\n" );
#endif
		if( strlen( pszBuf ) >= dwSize )
			return FALSE;

		pszBuf[strlen(pszBuf) + 1] = 0;

		return TRUE;
	T_E}

	BOOL CCharMission::MisInitRecord( char* pszBuf )
	{T_B
		int nEdition(-1), nData1(0);
		char* pTemp = pszBuf;
		sscanf( pTemp, "%d,", &nEdition );
#ifdef ROLE_DEBUG_INFO
		printf( "\nCCharMission:MisInitRecord: Misinfo record edition code = %d\n", nEdition );
#endif
		pTemp = strstr( pTemp, "," );
		if( pTemp == NULL ) return TRUE;
		pTemp++;

		if( nEdition != ROLE_MIS_RECORD_EDITION )
		{
#ifdef ROLE_DEBUG_INFO
			printf( "\nCCharMission:Init: Convert to new misinfo record edition! editon code = %d\n", nEdition );
#endif
			return ConvertMissionRecord( pTemp, nEdition );
		}

#ifdef ROLE_DEBUG_INFO
		printf( "\nRecord start!\n" );
#endif
		for( int i = 0; i < ROLE_MAXNUM_RECORDSIZE; i++ )
		{
			sscanf( pTemp, "%d,", &nData1 );
			m_RoleRecord.szID[i] = (BYTE)nData1;
#ifdef ROLE_DEBUG_INFO
			printf( "%d ", m_RoleRecord.szID[i] );
#endif
			pTemp = strstr( pTemp, "," );
			if( pTemp == NULL ) 
				return TRUE;
			pTemp++;
		}
#ifdef ROLE_DEBUG_INFO
		printf( "\n" );
#endif
		return TRUE;
	T_E}

	BOOL CCharMission::MisGetRecord( char* pszBuf, DWORD dwSize )
	{T_B
		sprintf( pszBuf, "%d,", ROLE_MIS_RECORD_EDITION );
#ifdef ROLE_DEBUG_INFO
		printf( "\nCCharMission:MisGetRecord: MisInfo record edition code = %d", ROLE_MIS_RECORD_EDITION );
#endif

#ifdef ROLE_DEBUG_INFO
		printf( "\nRecord start!\n" );
#endif
		for( int i = 0; i < ROLE_MAXNUM_RECORDSIZE; i++ )
		{
			sprintf( pszBuf + strlen( pszBuf ), "%d,", m_RoleRecord.szID[i] );
#ifdef ROLE_DEBUG_INFO
			printf( "%d ", m_RoleRecord.szID[i] );
#endif
		}
#ifdef ROLE_DEBUG_INFO
		printf( "\n" );
#endif
		if( strlen( pszBuf ) >= dwSize )
			return FALSE;

		pszBuf[strlen(pszBuf) + 1] = 0;

		return TRUE;
	T_E}

	BOOL CCharMission::MisInitTrigger( char* pszBuf )
	{T_B
		int nEdition(-1), nData1(0), nData2(0) , nData3(0), nData4(0), nData5(0), 
			nData6(0), nData7(0), nData8(0), nData9(0);

		char* pTemp = pszBuf;
		sscanf( pszBuf, "%d,", &nEdition );
#ifdef ROLE_DEBUG_INFO
		printf( "\nCCharMission:InitTrigger: Trigger edition code = %d\n", nEdition );
#endif
		pTemp = strstr( pTemp, "," );
		if( pTemp == NULL ) return TRUE;
		pTemp++;

		if( nEdition != ROLE_MIS_TRIGGER_EDITION )
		{
#ifdef ROLE_DEBUG_INFO
			printf( "\nCCharMission:InitTrigger: Convert to new trigger edition! editon code = %d\n", nEdition );
#endif
			return ConvertTriggerInfo( pTemp, nEdition );
		}

		sscanf( pTemp, "%d,", &nData1 );
		m_byNumTrigger = (BYTE)nData1;
#ifdef ROLE_DEBUG_INFO
		printf( "\nCCharMission:InitTrigger: m_byNumTrigger = %d", m_byNumTrigger );
#endif
		pTemp = strstr( pTemp, "," );
		if( pTemp == NULL ) return TRUE;
		pTemp++;

		if( m_byNumTrigger > ROLE_MAXNUM_CHARTRIGGER )
			return FALSE;
		for( int i = 0; i < m_byNumTrigger; i++ )
		{			
			sscanf( pTemp, "%d,%d,%d,%d,%d,%d,%d,%d,%d,", &nData1, &nData2, &nData3, &nData4, 
				&nData5, &nData6, &nData7, &nData8, &nData9 );
			m_Trigger[i].wTriggerID = (WORD)nData1;
			m_Trigger[i].wMissionID = (WORD)nData2;
			m_Trigger[i].byType = (BYTE)nData3; 
			m_Trigger[i].wParam1 = (WORD)nData4;
			m_Trigger[i].wParam2 = (WORD)nData5;
			m_Trigger[i].wParam3 = (WORD)nData6;
			m_Trigger[i].wParam4 = (WORD)nData7;
			m_Trigger[i].wParam5 = (WORD)nData8;
			m_Trigger[i].wParam6 = (WORD)nData9;

#ifdef ROLE_DEBUG_INFO
			printf( "\nTriggerID[%d], MisID[%d], Type[%d], p1[%d], p2[%d], p3[%d], p4[%d], p5[%d], p6[%d]", 
				m_Trigger[i].wTriggerID, m_Trigger[i].wMissionID, m_Trigger[i].byType, 
				m_Trigger[i].wParam1, m_Trigger[i].wParam2, m_Trigger[i].wParam3, m_Trigger[i].wParam4,
				m_Trigger[i].wParam5, m_Trigger[i].wParam6 );
#endif

			for( int n = 0; n < 9; n++ )
			{
				pTemp = strstr( pTemp, "," );
				if( pTemp == NULL ) 
					return TRUE;
				pTemp++;
			}
		}
#ifdef ROLE_DEBUG_INFO
		printf( "\n" );
#endif
		m_byNumGotoMap = 0;
		for( int t = 0; t < m_byNumTrigger; t++ )
		{
			if( m_Trigger[t].byType == mission::TE_GOTO_MAP )
			{
				m_byNumGotoMap++;
			}
		}
		return TRUE;		
	T_E}

	BOOL CCharMission::MisGetTrigger( char* pszBuf, DWORD dwSize )
	{T_B
		sprintf( pszBuf, "%d,", ROLE_MIS_TRIGGER_EDITION );
#ifdef ROLE_DEBUG_INFO
		printf( "\nCCharMission:GetTrigger: Trigger edition code = %d", ROLE_MIS_TRIGGER_EDITION );
#endif

		sprintf( pszBuf + strlen( pszBuf ), "%d,", m_byNumTrigger );
#ifdef ROLE_DEBUG_INFO
		printf( "\nCCharMission:GetTrigger: m_byNumTrigger = %d", m_byNumTrigger );
#endif
		for( int i = 0; i < m_byNumTrigger; i++ )
		{
			sprintf( pszBuf + strlen( pszBuf ), "%d,%d,%d,%d,%d,%d,%d,%d,%d,", m_Trigger[i].wTriggerID, 
				m_Trigger[i].wMissionID, m_Trigger[i].byType, m_Trigger[i].wParam1, m_Trigger[i].wParam2, 
				m_Trigger[i].wParam3, m_Trigger[i].wParam4, m_Trigger[i].wParam5, m_Trigger[i].wParam6 );
#ifdef ROLE_DEBUG_INFO
			printf( "\nTriggerID[%d], MisID[%d], Type[%d], p1[%d], p2[%d], p3[%d], p4[%d], p5[%d], p6[%d]", 
				m_Trigger[i].wTriggerID, m_Trigger[i].wMissionID, m_Trigger[i].byType, 
				m_Trigger[i].wParam1, m_Trigger[i].wParam2, m_Trigger[i].wParam3, m_Trigger[i].wParam4,
				m_Trigger[i].wParam5, m_Trigger[i].wParam6 );
#endif
		}
#ifdef ROLE_DEBUG_INFO
		printf( "\n" );
#endif
		if( strlen( pszBuf ) >= dwSize )
			return FALSE;

		pszBuf[strlen(pszBuf) + 1] = 0;
		return TRUE;
		
	T_E}

	BOOL CCharMission::MisInitMissionCount( char* pszBuf )
	{T_B
		// 
		//if( m_byOnline == 0 )
		//	return TRUE;

		int nData1(0), nData2(0), nData3(0), nEdition(-1);
		char* pTemp = pszBuf;
		sscanf( pszBuf, "%d,", &nEdition );
#ifdef ROLE_DEBUG_INFO
		printf( "\nCCharMission:InitMissionCount: MisCount edition code = %d\n", nEdition );
#endif
		pTemp = strstr( pTemp, "," );
		if( pTemp == NULL ) return TRUE;
		pTemp++;

		if( nEdition != ROLE_MIS_MISCOUNT_EDITION )
		{
#ifdef ROLE_DEBUG_INFO
			printf( "\nCCharMission:InitMissionCount: Convert to new miscount edition! editon code = %d\n", nEdition );
#endif
			return ConvertMisCountInfo( pTemp, nEdition );
		}

		sscanf( pTemp, "%d,", &nData1 );
		m_byNumMisCount = (BYTE)nData1;
#ifdef ROLE_DEBUG_INFO
		printf( "\nCCharMission:InitMissionCount: m_byNumMisCount = %d", m_byNumMisCount );
#endif
		pTemp = strstr( pTemp, "," );
		if( pTemp == NULL ) return TRUE;
		pTemp++;

		if( m_byNumMisCount > ROLE_MAXNUM_MISSIONCOUNT )
			return FALSE;

		for( int i = 0; i < m_byNumMisCount; i++ )
		{
			sscanf( pTemp, "%d,%d,%d,", &nData1, &nData2, &nData3 );
			m_MissionCount[i].wRoleID = (WORD)nData1;
			m_MissionCount[i].wCount  = (WORD)nData2;
			m_MissionCount[i].wNum  = (WORD)nData3;

#ifdef ROLE_DEBUG_INFO
			printf( "\nCCharMission:InitMissionCount: wRoleID[%d], wCount[%d], wNum[%d]", m_MissionCount[i].wRoleID, 
				m_MissionCount[i].wCount, m_MissionCount[i].wNum );
#endif
			for( int n = 0; n < 3; n++ )
			{
				pTemp = strstr( pTemp, "," );
				if( pTemp == NULL ) return TRUE;
				pTemp++;
			}
		}

#ifdef ROLE_DEBUG_INFO
		printf( "\n" );
#endif
		return TRUE;		
	T_E}

	BOOL CCharMission::MisGetMissionCount( char* pszBuf, DWORD dwSize )
	{T_B
		// 
		//if( m_byOnline == 0 )
		//	return TRUE;

		sprintf( pszBuf, "%d,", ROLE_MIS_MISCOUNT_EDITION );
#ifdef ROLE_DEBUG_INFO
		printf( "\nCCharMission:GetMissionCount: MisCount edition code = %d", ROLE_MIS_TRIGGER_EDITION );
#endif

		sprintf( pszBuf + strlen( pszBuf ), "%d,", m_byNumMisCount );
#ifdef ROLE_DEBUG_INFO
		printf( "\nCCharMission:GetMissionCount: m_byNumMisCount = %d", m_byNumMisCount );
#endif
		for( int i = 0; i < m_byNumMisCount; i++ )
		{
			sprintf( pszBuf + strlen( pszBuf ), "%d,%d,%d,", m_MissionCount[i].wRoleID, m_MissionCount[i].wCount, 
				m_MissionCount[i].wNum );
#ifdef ROLE_DEBUG_INFO
			printf( "\nCCharMission:GetMissionCount: wRoleID[%d], wCount[%d], wNum[%d]", m_MissionCount[i].wRoleID, 
				m_MissionCount[i].wCount, m_MissionCount[i].wNum );
#endif
		}
#ifdef ROLE_DEBUG_INFO
		printf( "\n" );
#endif
		if( strlen( pszBuf ) >= dwSize )
			return FALSE;

		pszBuf[strlen(pszBuf) + 1] = 0;
		return TRUE;		
	T_E}

	BOOL CCharMission::ConvertMissionRecord( const char* pszBuf, int nEdition )
	{T_B
		m_RoleRecord.Clear();
		return TRUE;
	T_E}

	BOOL CCharMission::ConvertMissionInfo( const char* pszBuf, int nEdition )
	{T_B
		m_byNumMission = 0;
		memset( m_Mission, 0, sizeof(MISSION_INFO)*ROLE_MAXNUM_MISSION );
		return TRUE;
	T_E}

	BOOL CCharMission::ConvertTriggerInfo( const char* pszBuf, int nEdition )
	{T_B
		m_byNumTrigger = 0;
		memset( m_Trigger, 0, sizeof(TRIGGER_DATA)*ROLE_MAXNUM_CHARTRIGGER );
		return TRUE;
	T_E}

	BOOL CCharMission::ConvertMisCountInfo( const char* pszBuf, int nEdition )
	{T_B
		m_byNumMisCount = 0;
		memset( m_MissionCount, 0, sizeof(RAND_MISSION_COUNT)*ROLE_MAXNUM_MISSIONCOUNT );
		return TRUE;
	T_E}

	void CCharMission::MisClear()
	{T_B
		m_byNumTrigger = 0;
		memset( m_Trigger, 0, sizeof(TRIGGER_DATA)*ROLE_MAXNUM_CHARTRIGGER );

		// 
		MISSION_INFO Info[ROLE_MAXNUM_MISSION];
		memset( Info, 0, sizeof(MISSION_INFO)*ROLE_MAXNUM_MISSION);
		memcpy( Info, m_Mission, sizeof(MISSION_INFO)*m_byNumMission ); 
		BYTE byNumMission = m_byNumMission;
		for( int n = 0; n < byNumMission; n++ )
		{
			MisCancelRole( Info[n].wRoleID );
		}

		// m_byNumMission = 0;
		// memset( m_Mission, 0, sizeof(MISSION_INFO)*ROLE_MAXNUM_MISSION );
		m_RoleRecord.Clear();

		memset( m_MissionState, 0, sizeof(MISSION_STATE)*ROLE_MAXNUM_INSIDE_NPCCOUNT );
		m_byStateIndex = 0;
		m_byNumState = 0;
		m_byNumGotoMap = 0;

		memset( m_MissionCount, 0, sizeof(RAND_MISSION_COUNT)*ROLE_MAXNUM_MISSIONCOUNT );
		m_byNumMisCount = 0;

		// 
		m_byOnline = 1;
	T_E}

	void CCharMission::KillWare( USHORT sWareID )
	{T_B
		for( int i = 0; i < m_byNumTrigger; i++ )
		{
			if( m_Trigger[i].byType == TE_KILL )
			{
				// ID
				if( sWareID == m_Trigger[i].wParam1 )
				{
					// lua
					lua_getglobal( g_pLuaState, "TriggerProc" );
					if( !lua_isfunction( g_pLuaState, -1 ) )
					{
						lua_pop( g_pLuaState, 1 );
						LG( "lua_invalidfunc", "TriggerProc" );
						return;
					}

					lua_pushlightuserdata( g_pLuaState, (void*)m_pRoleChar );
					lua_pushnumber( g_pLuaState, m_Trigger[i].wTriggerID );
					lua_pushnumber( g_pLuaState, m_Trigger[i].wParam1 ); // ID
					lua_pushnumber( g_pLuaState, m_Trigger[i].wParam2 ); // 
					lua_pushnumber( g_pLuaState, m_Trigger[i].wParam3 ); // 
					lua_pushnumber( g_pLuaState, m_Trigger[i].wParam2 );

					int nStatus = lua_pcall( g_pLuaState, 6, 1, 0 );
					if( nStatus )
					{
						m_pRoleChar->SystemNotice( "CCharMission::KillWare:[TriggerProc]" );
						lua_callalert( g_pLuaState, nStatus );
						lua_settop(g_pLuaState, 0);
						return;
					}

					DWORD dwResult = (DWORD)lua_tonumber( g_pLuaState, -1 );
					lua_settop(g_pLuaState, 0);
					if( dwResult == LUA_TRUE )
					{
						// 
						m_Trigger[i].wParam4++;
						WPACKET packet = GETWPACKET();
						WRITE_CMD(packet, CMD_MC_TRIGGER_ACTION );
						WRITE_CHAR(packet, m_Trigger[i].byType );
						WRITE_SHORT(packet, m_Trigger[i].wParam1 );
						WRITE_SHORT(packet, m_Trigger[i].wParam2 );
						WRITE_SHORT(packet, m_Trigger[i].wParam4 );
						m_pRoleChar->ReflectINFof( m_pRoleChar, packet );

						// 
						if( m_Trigger[i].wParam4 >= m_Trigger[i].wParam2 )
						{
#ifdef ROLE_DEBUG_INFO
							printf( "KillWare Complete!, ID=%d, p1=%d, p2=%d, p3=%d, p4=%d\n", m_Trigger[i].wTriggerID, 
								m_Trigger[i].wParam1, m_Trigger[i].wParam2, m_Trigger[i].wParam3, m_Trigger[i].wParam4 );
#endif
							ClearTrigger( i );
						}
					}
					else
					{
						//m_pRoleChar->SystemNotice( "CCharMission::KillWare:[TriggerProc]" );
						//m_pRoleChar->SystemNotice( RES_STRING(GM_MISSION_CPP_00001) );
					}

					return;
				}
			}
		}
	T_E}

	BOOL CCharMission::MisGetItemCount( WORD wRoleID, USHORT sItemID, USHORT& sCount )
	{
		for( int i = 0; i < m_byNumTrigger; i++ )
		{
			if( m_Trigger[i].wMissionID == wRoleID && m_Trigger[i].byType == TE_GET_ITEM )
			{
				// ID
				if( sItemID == m_Trigger[i].wParam1 )
				{
					sCount = m_Trigger[i].wParam4;
					return TRUE;
				}
			}
		}
		return FALSE;
	}

	void CCharMission::MisRefreshItemCount( USHORT sItemID )
	{
		USHORT sCount = 0;
		USHORT sNum = m_pRoleChar->m_CKitbag.GetUseGridNum();
		SItemGrid *pGridCont;
		for( int i = 0; i < sNum; i++ )
		{
			pGridCont = m_pRoleChar->m_CKitbag.GetGridContByNum( i );
			if( pGridCont )
			{
				if( sItemID == pGridCont->sID )
				{
					sCount += (USHORT)pGridCont->sNum;
				}
			}
		}

		for( int i = 0; i < m_byNumTrigger; i++ )
		{
			if( m_Trigger[i].byType == TE_GET_ITEM )
			{
				// ID
				if( sItemID == m_Trigger[i].wParam1 )
				{
					if( sCount >= m_Trigger[i].wParam2 )
					{
						m_Trigger[i].wParam4 = m_Trigger[i].wParam2;
					}
					else
					{
						m_Trigger[i].wParam4 = sCount;
					}

					return;
				}
			}
		}
	}

	void CCharMission::GetItem( USHORT sItemID, USHORT sCount )
	{T_B
		for( int i = 0; i < m_byNumTrigger; i++ )
		{
			if( m_Trigger[i].byType == TE_GET_ITEM )
			{
				// ID
				if( sItemID == m_Trigger[i].wParam1 )
				{
					// 
					if( m_Trigger[i].wParam4 >= m_Trigger[i].wParam2 )
					{
						continue;
					}

					for( int n = 0; n < sCount; n++ )
					{
						// lua
						lua_getglobal( g_pLuaState, "TriggerProc" );
						if( !lua_isfunction( g_pLuaState, -1 ) )
						{
							lua_pop( g_pLuaState, 1 );
							LG( "lua_invalidfunc", "TriggerProc" );
							return;
						}

						lua_pushlightuserdata( g_pLuaState, (void*)m_pRoleChar );
						lua_pushnumber( g_pLuaState, m_Trigger[i].wTriggerID );
						lua_pushnumber( g_pLuaState, m_Trigger[i].wParam1 ); // ID
						lua_pushnumber( g_pLuaState, m_Trigger[i].wParam2 ); // 
						lua_pushnumber( g_pLuaState, m_Trigger[i].wParam3 ); // 
						lua_pushnumber( g_pLuaState, m_Trigger[i].wParam2 );

						int nStatus = lua_pcall( g_pLuaState, 6, 1, 0 );
						if( nStatus )
						{
							//m_pRoleChar->SystemNotice( "CCharMission::GetItem:[TriggerProc]" );
							m_pRoleChar->SystemNotice( RES_STRING(GM_MISSION_CPP_00002) );
							lua_callalert( g_pLuaState, nStatus );
							lua_settop(g_pLuaState, 0);
							return;
						}

						DWORD dwResult = (DWORD)lua_tonumber( g_pLuaState, -1 );
						lua_settop(g_pLuaState, 0);
						if( dwResult == LUA_TRUE )
						{
							if( ++m_Trigger[i].wParam4 >= m_Trigger[i].wParam2 )
							{
								// 
								sCount -= n;
								break;
							}
						}
						else
						{
							//m_pRoleChar->SystemNotice( "CCharMission::GetItem:[TriggerProc]" );
							m_pRoleChar->SystemNotice( RES_STRING(GM_MISSION_CPP_00004) );
						}
					}

					WPACKET packet = GETWPACKET();
					WRITE_CMD(packet, CMD_MC_TRIGGER_ACTION );
					WRITE_CHAR(packet, m_Trigger[i].byType );
					WRITE_SHORT(packet, m_Trigger[i].wParam1 );
					WRITE_SHORT(packet, m_Trigger[i].wParam2 );
					WRITE_SHORT(packet, m_Trigger[i].wParam4 );
					m_pRoleChar->ReflectINFof( m_pRoleChar, packet );
					
					// 
					if( m_Trigger[i].wParam4 >= m_Trigger[i].wParam2 )
					{
#ifdef ROLE_DEBUG_INFO
						printf( "GetItem Complete!, ID=%d, p1=%d, p2=%d, p3=%d, p4=%d\n", m_Trigger[i].wTriggerID, 
							m_Trigger[i].wParam1, m_Trigger[i].wParam2, m_Trigger[i].wParam3, m_Trigger[i].wParam4 );
#endif
						// 
						// ClearTrigger( i-- );
					}
					return;
				}
			}
		}
	T_E}

	void CCharMission::TimeOut( USHORT sTime )
	{T_B

		//m_pRoleChar->SystemNotice( "Time Out!" );

		for( int i = 0; i < m_byNumTrigger; i++ )
		{
			if( m_Trigger[i].byType == TE_GAME_TIME )
			{
				// 
				if( ++m_Trigger[i].wParam4 < m_Trigger[i].wParam2 )
					continue;

				// lua
				lua_getglobal( g_pLuaState, "TriggerProc" );
				if( !lua_isfunction( g_pLuaState, -1 ) )
				{
					lua_pop( g_pLuaState, 1 );
					LG( "lua_invalidfunc", "TriggerProc" );
					return;
				}

				lua_pushlightuserdata( g_pLuaState, (void*)m_pRoleChar );
				lua_pushnumber( g_pLuaState, m_Trigger[i].wTriggerID );
				lua_pushnumber( g_pLuaState, m_Trigger[i].wParam1 );
				lua_pushnumber( g_pLuaState, m_Trigger[i].wParam2 );

				int nStatus = lua_pcall( g_pLuaState, 4, 1, 0 );
				if( nStatus )
				{
					//m_pRoleChar->SystemNotice( "CCharMission::TimeOut:[TriggerProc]" );
					m_pRoleChar->SystemNotice( RES_STRING(GM_MISSION_CPP_00003) );
					lua_callalert( g_pLuaState, nStatus );
					lua_settop(g_pLuaState, 0);
					continue;
				}

				DWORD dwResult = (DWORD)lua_tonumber( g_pLuaState, -1 );
				lua_settop(g_pLuaState, 0);
				if( dwResult == LUA_TRUE )
				{
					// 
					switch( m_Trigger[i].wParam1 )
					{
					case TT_CYCLETIME:
						{
							// 
							m_Trigger[i].wParam4 = 0;
						}
						break;
					case TT_MULTITIME:
						{
							if( m_Trigger[i].wParam3 > 0 )
							{
								m_Trigger[i].wParam3--;

								// 
								m_Trigger[i].wParam4 = 0;
							}
							else
							{
#ifdef ROLE_DEBUG_INFO
								printf( "TimeOut Complete!, ID=%d, p1=%d, p2=%d, p3=%d, p4=%d\n", m_Trigger[i].wTriggerID, 
									m_Trigger[i].wParam1, m_Trigger[i].wParam2, m_Trigger[i].wParam3, m_Trigger[i].wParam4 );
#endif
								// 
								ClearTrigger( i-- );
							}
						}
						break;
					default:
						{
							//LG( "trigger_error", "" );
							LG( "trigger_error", "unknown time trigger time slot type" );
							//m_pRoleChar->SystemNotice( "TID = %d, Type = %d", m_Trigger[i].wTriggerID, m_Trigger[i].wParam1 );
							m_pRoleChar->SystemNotice( RES_STRING(GM_MISSION_CPP_00005), m_Trigger[i].wTriggerID, m_Trigger[i].wParam1 );
							ClearTrigger( i-- );
						}
						break;
					}
				}
				else
				{
					//m_pRoleChar->SystemNotice( "CCharMission::TimeOut:[TriggerProc]" );
					m_pRoleChar->SystemNotice( RES_STRING(GM_MISSION_CPP_00004) );
				}
			}
		}
	T_E}

	void CCharMission::GotoMap( BYTE byMapID, WORD wxPos, WORD wyPos )
	{T_B
		for( int i = 0; i < m_byNumTrigger; i++ )
		{
			if( m_Trigger[i].byType == TE_GOTO_MAP )
			{
				// 
				if( byMapID != m_Trigger[i].wParam1 )
				{
					// 
					m_Trigger[i].wParam6 = 0;
					continue;
				}
				// 
				if( wxPos > m_Trigger[i].wParam2 && wyPos > m_Trigger[i].wParam3 && 
					wxPos < m_Trigger[i].wParam2 + m_Trigger[i].wParam4 && 
					wyPos < m_Trigger[i].wParam3 + m_Trigger[i].wParam4 )
				{
					// 
					if( m_Trigger[i].wParam6 )
						continue;					

					// lua
					lua_getglobal( g_pLuaState, "TriggerProc" );
					if( !lua_isfunction( g_pLuaState, -1 ) )
					{
						lua_pop( g_pLuaState, 1 );
						LG( "lua_invalidfunc", "TriggerProc" );
						return;
					}

					lua_pushlightuserdata( g_pLuaState, (void*)m_pRoleChar );
					lua_pushnumber( g_pLuaState, m_Trigger[i].wTriggerID );
					lua_pushnumber( g_pLuaState, 0 );
					lua_pushnumber( g_pLuaState, 0 );

					int nStatus = lua_pcall( g_pLuaState, 4, 1, 0 );
					if( nStatus )
					{
						//m_pRoleChar->SystemNotice( "CCharMission::GotoMap:[TriggerProc]" );
						m_pRoleChar->SystemNotice( RES_STRING(GM_MISSION_CPP_00006) );
						lua_callalert( g_pLuaState, nStatus );
						lua_settop(g_pLuaState, 0);
						continue;
					}

					DWORD dwResult = (DWORD)lua_tonumber( g_pLuaState, -1 );
					lua_settop(g_pLuaState, 0);
					if( dwResult == LUA_TRUE )
					{
#ifdef ROLE_DEBUG_INFO
						printf( "GotoMap Complete!, ID=%d, p1=%d, p2=%d, p3=%d, p4=%d\n", m_Trigger[i].wTriggerID, 
							m_Trigger[i].wParam1, m_Trigger[i].wParam2, m_Trigger[i].wParam3, m_Trigger[i].wParam4 );
#endif
						WPACKET packet = GETWPACKET();
						WRITE_CMD(packet, CMD_MC_TRIGGER_ACTION );
						WRITE_CHAR(packet, m_Trigger[i].byType );
						WRITE_SHORT(packet, m_Trigger[i].wParam1 );
						WRITE_SHORT(packet, m_Trigger[i].wParam2 );
						WRITE_SHORT(packet, m_Trigger[i].wParam3 );
						m_pRoleChar->ReflectINFof( m_pRoleChar, packet );

						if( m_Trigger[i].wParam5 )
						{
							ClearTrigger( i-- );
						}
					}
					else
					{
						//m_pRoleChar->SystemNotice( "CCharMission::GotoMap:[TriggerProc]" );
						m_pRoleChar->SystemNotice( RES_STRING(GM_MISSION_CPP_00007) );
					}

					// 
					m_Trigger[i].wParam6 = 1;
				}
				else
				{
					// 
					m_Trigger[i].wParam6 = 0;
				}
			}
		}
	T_E}

	void CCharMission::LevelUp( USHORT sLevel )
	{T_B
		for( int i = 0; i < m_byNumTrigger; i++ )
		{
			if( m_Trigger[i].byType == TE_LEVEL_UP )
			{
				if( m_Trigger[i].wParam2 == 1 && sLevel < m_Trigger[i].wParam1 )
					continue;

				// lua
				lua_getglobal( g_pLuaState, "TriggerProc" );
				if( !lua_isfunction( g_pLuaState, -1 ) )
				{
					lua_pop( g_pLuaState, 1 );
					LG( "lua_invalidfunc", "TriggerProc" );
					return;
				}

				lua_pushlightuserdata( g_pLuaState, (void*)m_pRoleChar );
				lua_pushnumber( g_pLuaState, m_Trigger[i].wTriggerID );
				lua_pushnumber( g_pLuaState, m_Trigger[i].wParam1 );
				lua_pushnumber( g_pLuaState, m_Trigger[i].wParam2 );

				int nStatus = lua_pcall( g_pLuaState, 4, 1, 0 );
				if( nStatus )
				{
					//m_pRoleChar->SystemNotice( "CCharMission::LevelUp:[TriggerProc]" );
					m_pRoleChar->SystemNotice( RES_STRING(GM_MISSION_CPP_00008) );
					lua_callalert( g_pLuaState, nStatus );
					lua_settop(g_pLuaState, 0);
					continue;
				}

				DWORD dwResult = (DWORD)lua_tonumber( g_pLuaState, -1 );
				lua_settop(g_pLuaState, 0);
				if( dwResult == LUA_TRUE )
				{
#ifdef ROLE_DEBUG_INFO
					printf( "LevelUp Complete!, ID=%d, p1=%d, p2=%d, p3=%d, p4=%d\n", m_Trigger[i].wTriggerID, 
						m_Trigger[i].wParam1, m_Trigger[i].wParam2, m_Trigger[i].wParam3, m_Trigger[i].wParam4 );
#endif
					WPACKET packet = GETWPACKET();
					WRITE_CMD(packet, CMD_MC_TRIGGER_ACTION );
					WRITE_CHAR(packet, m_Trigger[i].byType );
					WRITE_SHORT(packet, m_Trigger[i].wParam1 );
					WRITE_SHORT(packet, m_Trigger[i].wParam2 );
					WRITE_SHORT(packet, 0 );
					m_pRoleChar->ReflectINFof( m_pRoleChar, packet );

					// 
					if( m_Trigger[i].wParam2 )
					{
						ClearTrigger( i-- );
					}
				}
				else
				{
					//m_pRoleChar->SystemNotice( "CCharMission::LevelUp:[TriggerProc]" );
					m_pRoleChar->SystemNotice( RES_STRING(GM_MISSION_CPP_00009) );
				}
			}
		}
	T_E}

	void CCharMission::CharBorn()
	{T_B
		// lua
		lua_getglobal( g_pLuaState, "TriggerProc" );
		if( !lua_isfunction( g_pLuaState, -1 ) )
		{
			lua_pop( g_pLuaState, 1 );
			LG( "lua_invalidfunc", "TriggerProc" );
			return;
		}

		lua_pushlightuserdata( g_pLuaState, (void*)m_pRoleChar );
		lua_pushnumber( g_pLuaState, 88888 );
		lua_pushnumber( g_pLuaState, 0 );
		lua_pushnumber( g_pLuaState, 0 );

		int nStatus = lua_pcall( g_pLuaState, 4, 1, 0 );
		if( nStatus )
		{
			//m_pRoleChar->SystemNotice( "CCharMission::CharBorn:[TriggerProc]" );
			m_pRoleChar->SystemNotice( RES_STRING(GM_MISSION_CPP_00010) );
			lua_callalert( g_pLuaState, nStatus );
			lua_settop(g_pLuaState, 0);
			return;
		}

		DWORD dwResult = (DWORD)lua_tonumber( g_pLuaState, -1 );
		lua_settop(g_pLuaState, 0);
		if( dwResult == LUA_TRUE )
		{
			WPACKET packet = GETWPACKET();
			WRITE_CMD(packet, CMD_MC_TRIGGER_ACTION );
			WRITE_CHAR(packet, TE_MAP_INIT );
			WRITE_SHORT(packet, 0 );
			WRITE_SHORT(packet, 0 );
			WRITE_SHORT(packet, 0 );
			m_pRoleChar->ReflectINFof( m_pRoleChar, packet );
		}
		else
		{
			//m_pRoleChar->SystemNotice( "CCharMission::CharBorn:[TriggerProc]" );
			m_pRoleChar->SystemNotice( RES_STRING(GM_MISSION_CPP_00011) );
		}
	T_E}

	void CCharMission::EquipItem( USHORT sItemID, USHORT sTriID )
	{T_B
		// lua
		lua_getglobal( g_pLuaState, "TriggerProc" );
		if( !lua_isfunction( g_pLuaState, -1 ) )
		{
			lua_pop( g_pLuaState, 1 );
			LG( "lua_invalidfunc", "TriggerProc" );
			return;
		}

		lua_pushlightuserdata( g_pLuaState, (void*)m_pRoleChar );
		lua_pushnumber( g_pLuaState, sTriID );
		lua_pushnumber( g_pLuaState, 0 );
		lua_pushnumber( g_pLuaState, sItemID );

		int nStatus = lua_pcall( g_pLuaState, 4, 1, 0 );
		if( nStatus )
		{
			//m_pRoleChar->SystemNotice( "CCharMission::EquipItem:[TriggerProc]" );
			m_pRoleChar->SystemNotice( RES_STRING(GM_MISSION_CPP_00012) );
			lua_callalert( g_pLuaState, nStatus );
			lua_settop(g_pLuaState, 0);
			return;
		}

		DWORD dwResult = (DWORD)lua_tonumber( g_pLuaState, -1 );
		lua_settop(g_pLuaState, 0);
		if( dwResult == LUA_TRUE )
		{
			WPACKET packet = GETWPACKET();
			WRITE_CMD(packet, CMD_MC_TRIGGER_ACTION );
			WRITE_CHAR(packet, TE_EQUIP_ITEM );
			WRITE_SHORT(packet, 0 );
			WRITE_SHORT(packet, sItemID );
			WRITE_SHORT(packet, 0 );
			m_pRoleChar->ReflectINFof( m_pRoleChar, packet );
		}
		else
		{
			//m_pRoleChar->SystemNotice( "CCharMission::EquipItem:[TriggerProc]" );
			m_pRoleChar->SystemNotice( RES_STRING(GM_MISSION_CPP_00013) );
		}
	T_E}

	// 
	BOOL CCharMission::MisEventProc( TRIGGER_EVENT e, WPARAM wParam, LPARAM lParam )
	{
		switch( e )
		{
		case TE_MAP_INIT:
			{
				CharBorn();
			}
			break;
		case TE_NPC:
			break;
		case TE_KILL:
			{
				KillWare( (USHORT)wParam );
			}
			break;
		case TE_GAME_TIME:
			{
				TimeOut( (USHORT)wParam );
			}
			break;
		case TE_CHAT:
			break; 
		case TE_GET_ITEM:
			{
				GetItem( (USHORT)wParam, (USHORT)lParam );
			}
			break;
		case TE_EQUIP_ITEM:
			{
				EquipItem( (USHORT)wParam, (USHORT)lParam );
			}
			break;
		case TE_GOTO_MAP:
			{
				if( m_byNumGotoMap > 0 ) 
				{
					GotoMap( (BYTE)wParam, WORD(lParam>>16), WORD(lParam&0xffff) );
				}
			}
			break;
		case TE_LEVEL_UP:
			{
				LevelUp( USHORT(wParam) );
			}
			break;
		default:
			break;
		}

		DeleteTrigger();
		return TRUE;
	}

	BOOL CCharMission::MisNeedItem( USHORT sItemID )
	{
		for( int i = 0; i < m_byNumTrigger; i++ )
		{
			if( m_Trigger[i].byType == TE_GET_ITEM && sItemID == m_Trigger[i].wParam1)
			{
				// 
				return !(m_Trigger[i].wParam4 >= m_Trigger[i].wParam2);
			}
		}
		return FALSE;
	}

	BOOL CCharMission::MisAddMissionState( DWORD dwNpcID, BYTE byID, BYTE byState )
	{
		BOOL bAdd = TRUE;
		if( m_MissionState[m_byStateIndex].dwNpcID != dwNpcID )
		{
			m_byStateIndex = m_byNumState; 
			for( int i = 0; i < m_byNumState; i++ )
			{
				if( m_MissionState[i].dwNpcID == dwNpcID )
				{
					m_byStateIndex = i;
					bAdd = FALSE;
					break;
				}
			}
		}
		else
		{
			bAdd = FALSE;
		}
		
		if( m_byStateIndex >= ROLE_MAXNUM_INSIDE_NPCCOUNT )
			return FALSE;
		if( m_MissionState[m_byStateIndex].byMisNum + 1 >= ROLE_MAXNUM_MISSIONSTATE ) 
			return FALSE;

		//if( byState & ROLE_MIS_ACCEPT )
		//{

		//}
		//else if( byState & ROLE_MIS_DELIVERY )
		//{

		//}
		//else if( byState & ROLE_MIS_PENDING )
		//{

		//}
		//else
		//{
		//	m_pRoleChar->SystemNotice( "npcid = 0x%X, state = %d", dwNpcID, byState );
		//	return FALSE;
		//}

		if( bAdd ) m_byNumState++;
		m_MissionState[m_byStateIndex].dwNpcID = dwNpcID;
		m_MissionState[m_byStateIndex].StateInfo[m_MissionState[m_byStateIndex].byMisNum].byID = byID;
		m_MissionState[m_byStateIndex].StateInfo[m_MissionState[m_byStateIndex].byMisNum].byState = byState;
		m_MissionState[m_byStateIndex].byMisNum++;
		m_MissionState[m_byStateIndex].byNpcState |= byState;
		return TRUE;
	}

	BOOL CCharMission::MisGetMissionState( DWORD dwNpcID, BYTE& byState )
	{
		int nIndex = -1;
		if( m_MissionState[m_byStateIndex].dwNpcID != dwNpcID )
		{
			for( int i = 0; i < m_byNumState; i++ )
			{
				if( m_MissionState[i].dwNpcID == dwNpcID )
				{
					nIndex = i;
					break;
				}
			}
		}
		else
		{
			nIndex = m_byStateIndex;
		}

		if( nIndex == -1 )
		{
			//m_pRoleChar->SystemNotice( "GetMissionState:NPCnpc" );
			return FALSE;
		}

		byState = m_MissionState[nIndex].byNpcState;
		return TRUE;
	}

	BOOL CCharMission::MisGetNumMission( DWORD dwNpcID, BYTE& byNum )
	{
		int nIndex = -1;
		if( m_MissionState[m_byStateIndex].dwNpcID != dwNpcID )
		{
			for( int i = 0; i < m_byNumState; i++ )
			{
				if( m_MissionState[i].dwNpcID == dwNpcID )
				{
					nIndex = i;
					break;
				}
			}
		}
		else
		{
			nIndex = m_byStateIndex;
		}

		if( nIndex == -1 )
		{
			//m_pRoleChar->SystemNotice( "GetNumMission:NPC" );
			return FALSE;
		}
		
		byNum = m_MissionState[nIndex].byMisNum;
		return TRUE;
	}

	BOOL CCharMission::MisGetMissionInfo( DWORD dwNpcID, BYTE byIndex, BYTE& byID, BYTE& byState )
	{		
		int nIndex = -1;
		if( m_MissionState[m_byStateIndex].dwNpcID != dwNpcID )
		{
			for( int i = 0; i < m_byNumState; i++ )
			{
				if( m_MissionState[i].dwNpcID == dwNpcID )
				{
					nIndex = i;
					break;
				}
			}
		}
		else
		{
			nIndex = m_byStateIndex;
		}

		if( nIndex == -1 )
		{
			//m_pRoleChar->SystemNotice( "GetMissionInfo:NPC" );
			m_pRoleChar->SystemNotice( RES_STRING(GM_MISSION_CPP_00014) );
			return FALSE;
		}

		if( byIndex >= m_MissionState[nIndex].byMisNum )
		{
			//m_pRoleChar->SystemNotice( "GetMissionInfo:NPC" );
			m_pRoleChar->SystemNotice( RES_STRING(GM_MISSION_CPP_00015) );
			return FALSE;
		}

		byID	= m_MissionState[nIndex].StateInfo[byIndex].byID;
		byState = m_MissionState[nIndex].StateInfo[byIndex].byState;
		return TRUE;
	}

	BOOL CCharMission::MisGetCharMission( DWORD dwNpcID, BYTE byID, BYTE& byState )
	{
		int nIndex = -1;
		if( m_MissionState[m_byStateIndex].dwNpcID != dwNpcID )
		{
			for( int i = 0; i < m_byNumState; i++ )
			{
				if( m_MissionState[i].dwNpcID == dwNpcID )
				{
					nIndex = i;
					break;
				}
			}
		}
		else
		{
			nIndex = m_byStateIndex;
		}

		if( nIndex == -1 )
		{
			//m_pRoleChar->SystemNotice( "GetMissionInfo:NPCdwNpcID = %d", dwNpcID );
			m_pRoleChar->SystemNotice( RES_STRING(GM_MISSION_CPP_00016), dwNpcID );
			return FALSE;
		}

		int nID = -1;
		for( int i = 0; i < m_MissionState[nIndex].byMisNum; i++ )
		{
			if( m_MissionState[nIndex].StateInfo[i].byID == byID )
			{
				nID = i;
				break;
			}
		}
		byState = m_MissionState[nIndex].StateInfo[nID].byState;
		return TRUE;
	}

	BOOL CCharMission::MisGetNextMission( DWORD dwNpcID, BYTE& byIndex, BYTE& byID, BYTE& byState )
	{
		int nIndex = -1;
		if( m_MissionState[m_byStateIndex].dwNpcID != dwNpcID )
		{
			for( int i = 0; i < m_byNumState; i++ )
			{
				if( m_MissionState[i].dwNpcID == dwNpcID )
				{
					nIndex = i;
					break;
				}
			}
		}
		else
		{
			nIndex = m_byStateIndex;
		}

		if( nIndex == -1 )
		{			
			return FALSE;
		}
		
		BYTE byNumAcp = 0;
		BYTE byAccept[ROLE_MAXNUM_MISSIONSTATE];
		for( int n = 0; n < m_MissionState[nIndex].byMisNum; n++ )
		{
			if( m_MissionState[nIndex].StateInfo[n].byState == ROLE_MIS_DELIVERY )
			{
				byIndex = n;
				byID	= m_MissionState[nIndex].StateInfo[n].byID;
				byState = m_MissionState[nIndex].StateInfo[n].byState;
				return TRUE;
			}
			else if( m_MissionState[nIndex].StateInfo[n].byState == ROLE_MIS_ACCEPT )
			{
				byAccept[byNumAcp++] = n;
			}
		}

		if( byNumAcp > 0 )
		{
			byIndex = byAccept[0];
			byID	= m_MissionState[nIndex].StateInfo[byIndex].byID;
			byState = m_MissionState[nIndex].StateInfo[byIndex].byState;
			return TRUE;
		}

		return FALSE;
	}

	BOOL CCharMission::MisClearMissionState( DWORD dwNpcID )
	{
		if( m_MissionState[m_byStateIndex].dwNpcID != dwNpcID )
		{
			for( int i = 0; i < m_byNumState; i++ )
			{
				if( m_MissionState[i].dwNpcID == dwNpcID )
				{
					m_byStateIndex = i;
					break;
				}
			}
		}

		if( m_MissionState[m_byStateIndex].dwNpcID != dwNpcID )
		{
			//m_pRoleChar->SystemNotice( "ClearMissionState:NPC" );
			return FALSE;
		}

		MISSION_STATE Info[ROLE_MAXNUM_INSIDE_NPCCOUNT];
		memset( Info, 0, sizeof(MISSION_STATE)*m_byNumState );
		memcpy( Info, m_MissionState, sizeof(MISSION_STATE)*m_byNumState );
		memset( m_MissionState, 0, sizeof(MISSION_STATE)*m_byNumState );
		memcpy( m_MissionState, Info, sizeof(MISSION_STATE)*m_byStateIndex );
		memcpy( m_MissionState + m_byStateIndex, Info + m_byStateIndex + 1, sizeof(MISSION_STATE)*( m_byNumState - m_byStateIndex - 1 ) );
		m_byNumState--;
		return TRUE;
	}

	void CCharMission::MisSetMissionPage( DWORD dwNpcID, BYTE byPrev, BYTE byNext, BYTE byState )
	{
		m_dwTalkNpcID = dwNpcID;
		m_byPrev = byPrev;
		m_byNext = byNext;
		m_byState = byState;
	}

	BOOL CCharMission::MisGetMissionPage( DWORD dwNpcID, BYTE& byPrev, BYTE& byNext, BYTE& byState )
	{
		if( dwNpcID != m_dwTalkNpcID )
			return FALSE;
		byPrev = m_byPrev;
		byNext = m_byNext;
		byState = m_byState;
		return TRUE;
	}

	void CCharMission::MisSetTempData( DWORD dwNpcID, WORD wID, BYTE byState, BYTE byMisType )
	{
		m_dwTalkNpcID = dwNpcID;
		m_wIndex  = wID;
		m_byState = byState;
		m_byMisType = byMisType;
	}

	BOOL CCharMission::MisGetTempData( DWORD dwNpcID, WORD& wID, BYTE& byState, BYTE& byMisType )
	{
		if( dwNpcID != m_dwTalkNpcID )
			return FALSE;
		byState = m_byState;
		wID		= m_wIndex;
		byMisType = m_byMisType;
		return TRUE;
	}

	BOOL CCharMission::MisAddTrigger( const TRIGGER_DATA& Data )
	{
		if( m_byNumTrigger >= ROLE_MAXNUM_CHARTRIGGER )
			return FALSE;
		
		m_Trigger[m_byNumTrigger].wTriggerID = Data.wTriggerID;
		m_Trigger[m_byNumTrigger].wMissionID = Data.wMissionID;
		m_Trigger[m_byNumTrigger].byType  = Data.byType;
		m_Trigger[m_byNumTrigger].wParam1 = Data.wParam1;
		m_Trigger[m_byNumTrigger].wParam2 = Data.wParam2;
		m_Trigger[m_byNumTrigger].wParam3 = Data.wParam3;
		m_Trigger[m_byNumTrigger].wParam4 = Data.wParam4;
		m_Trigger[m_byNumTrigger].wParam5 = Data.wParam5;
		m_Trigger[m_byNumTrigger].wParam6 = Data.wParam6;
		m_byNumTrigger++;

#ifdef ROLE_DEBUG_INFO
		printf( "AddTrigger, num=%d, wID=%d, wMisID=%d, e=%d, p1=%d, p2=%d, p3=%d, p4=%d, p5=%d, p6=%d\n", m_byNumTrigger,
			Data.wTriggerID, Data.wMissionID, Data.byType, Data.wParam1, Data.wParam2, Data.wParam3, Data.wParam4,
			Data.wParam5, Data.wParam6 );
#endif

		if( Data.byType == TE_GET_ITEM )
		{
			m_pRoleChar->RefreshNeedItem( Data.wParam1 );
		}
		else if( Data.byType == TE_GOTO_MAP )
		{
			m_byNumGotoMap++;
		}

		return TRUE;
	}

	BOOL CCharMission::MisClearTrigger( WORD wTriggerID )
	{
		for( int i = 0; i < m_byNumTrigger; i++ )
		{
			if( m_Trigger[i].wTriggerID == wTriggerID )
			{
				ClearTrigger( i );
				return TRUE;
			}
		}
		return FALSE;
	}

	BOOL CCharMission::MisDelTrigger( WORD wTriggerID )
	{
		for( int i = 0; i < m_byNumTrigger; i++ )
		{
			if( m_Trigger[i].wTriggerID == wTriggerID )
			{
				m_Trigger[i].byIsDel = TRIGGER_DELED;
				return TRUE;
			}
		}
		return FALSE;
	}

	void CCharMission::DeleteTrigger()
	{
		for( int i = 0; i < m_byNumTrigger; i++ )
		{
			if( m_Trigger[i].byIsDel == TRIGGER_DELED )
			{
				ClearTrigger( (DWORD)i );				
			}
		}
	}

	void CCharMission::ClearTrigger( DWORD dwIndex )
	{
		if( dwIndex >= m_byNumTrigger )
			return;

		// 
		if( m_Trigger[dwIndex].byType == TE_GOTO_MAP ) 
			m_byNumGotoMap--;

		memset( m_Trigger + dwIndex, 0, sizeof(TRIGGER_DATA) );
		TRIGGER_DATA Trigger[ROLE_MAXNUM_CHARTRIGGER];
		memcpy( Trigger, m_Trigger, sizeof(TRIGGER_DATA)*m_byNumTrigger );
		memset( m_Trigger + dwIndex, 0, sizeof(TRIGGER_DATA)*(m_byNumTrigger - dwIndex) );
		memcpy( m_Trigger + dwIndex, Trigger + dwIndex + 1, sizeof(TRIGGER_DATA)*( m_byNumTrigger - dwIndex - 1 ) );
		m_byNumTrigger--;
	}

	void CCharMission::MisGetMisLog()
	{
		WPACKET packet = GETWPACKET();
		WRITE_CMD(packet, CMD_MC_MISLOG );
		WRITE_CHAR(packet, m_byNumMission );
		for( BYTE i = 0; i < m_byNumMission; i++ )
		{
			WRITE_SHORT(packet, m_Mission[i].wRoleID );	
			WRITE_CHAR(packet, m_Mission[i].byState );
		}

		m_pRoleChar->ReflectINFof( m_pRoleChar, packet );
	}

	void CCharMission::MisGetMisLogInfo( WORD wMisID )
	{T_B
		int nIndex = -1;
		for( int i = 0; i < m_byNumMission; i++ )
		{
			if( m_Mission[i].wRoleID == wMisID )
			{
				nIndex = i;
				break;
			}
		}

		if( nIndex == -1 )
		{
			//m_pRoleChar->SystemNotice( "MisGetMisLogInfo:ID = %d", wMisID );
			return;
		}

		// lua
		lua_getglobal( g_pLuaState, "MissionLog" );
		if( !lua_isfunction( g_pLuaState, -1 ) )
		{
			lua_pop( g_pLuaState, 1 );
			LG( "lua_invalidfunc", "TriggerProc" );
			return;
		}

		lua_pushlightuserdata( g_pLuaState, (void*)m_pRoleChar );
		lua_pushnumber( g_pLuaState, m_Mission[nIndex].wParam1 );

		int nStatus = lua_pcall( g_pLuaState, 2, 0, 0 );
		if( nStatus )
		{
			//m_pRoleChar->SystemNotice( "CCharMission::MisGetMisLogInfo:[MissionLog]" );
			m_pRoleChar->SystemNotice( RES_STRING(GM_MISSION_CPP_00017) );
			lua_callalert( g_pLuaState, nStatus );
		}
		lua_settop(g_pLuaState, 0);
	T_E}

	void CCharMission::MisLogClear( WORD wMisID )
	{
		WPACKET packet = GETWPACKET();
		WRITE_CMD(packet, CMD_MC_MISLOG_CLEAR );	
		WRITE_SHORT(packet, wMisID );

		m_pRoleChar->ReflectINFof( m_pRoleChar, packet );
	}

	void CCharMission::MisLogAdd( WORD wMisID, BYTE byState )
	{
		WPACKET packet = GETWPACKET();
		WRITE_CMD(packet, CMD_MC_MISLOG_ADD );
		WRITE_SHORT(packet, wMisID );
		WRITE_CHAR(packet, byState );

		m_pRoleChar->ReflectINFof( m_pRoleChar, packet );
	}

	void CCharMission::ClearRoleTrigger( WORD wRoleID )
	{
		for( int i = 0; i < m_byNumTrigger; i++ )
		{
			if( m_Trigger[i].wMissionID == wRoleID )
			{
				ClearTrigger( i-- );
			}
		}
	}

	BOOL CCharMission::MisAddRole( WORD wRoleID, WORD wScriptID )
	{
		if( m_byNumMission >= ROLE_MAXNUM_FLAG )
			return FALSE;

		m_Mission[m_byNumMission].wRoleID = wRoleID;
		m_Mission[m_byNumMission].byState = ROLE_MIS_PENDING_FLAG;
		m_Mission[m_byNumMission].byMisType = MIS_TYPE_NOMAL;
		m_Mission[m_byNumMission].wParam1 = wScriptID;
		
		// 
		MisLogAdd( wRoleID, ROLE_MIS_PENDING_FLAG );

		m_byNumMission++;

		return TRUE;
	}

	BOOL CCharMission::MisHasRole( WORD wRoleID )
	{
		for( int i = 0; i < m_byNumMission; i++ )
		{
			if( m_Mission[i].wRoleID == wRoleID ) 
			{
				return m_Mission[i].byState != ROLE_MIS_FAILURE_FALG;
			}
		}

		return FALSE;
	}

	BOOL CCharMission::MisGetMisScript( WORD wRoleID, WORD& wScriptID )
	{
		for( int i = 0; i < m_byNumMission; i++ )
		{
			if( m_Mission[i].wRoleID == wRoleID )
			{
				wScriptID = m_Mission[i].wParam1;
				return TRUE;
			}
		}

		return FALSE;
	}

	BOOL CCharMission::MisSetMissionComplete( WORD wRoleID )
	{
		for( int i = 0; i < m_byNumMission; i++ )
		{
			if( m_Mission[i].wRoleID == wRoleID )
			{
				m_Mission[i].byState = ROLE_MIS_COMPLETE_FLAG;

				WPACKET packet = GETWPACKET();
				WRITE_CMD(packet, CMD_MC_MISLOG_CHANGE );
				WRITE_SHORT(packet, m_Mission[i].wRoleID );	
				WRITE_CHAR(packet, m_Mission[i].byState );	
				m_pRoleChar->ReflectINFof( m_pRoleChar, packet );
				return TRUE;
			}
		}

		return FALSE;
	}

	BOOL CCharMission::MisSetMissionPending( WORD wRoleID )
	{
		for( int i = 0; i < m_byNumMission; i++ )
		{
			if( m_Mission[i].wRoleID == wRoleID )
			{
				m_Mission[i].byState = ROLE_MIS_COMPLETE_FLAG;

				WPACKET packet = GETWPACKET();
				WRITE_CMD(packet, CMD_MC_MISLOG_CHANGE );
				WRITE_SHORT(packet, m_Mission[i].wRoleID );	
				WRITE_CHAR(packet, m_Mission[i].byState );	
				m_pRoleChar->ReflectINFof( m_pRoleChar, packet );
				return TRUE;
			}
		}

		return FALSE;
	}

	BOOL CCharMission::MisSetMissionFailure( WORD wRoleID )
	{
		for( int i = 0; i < m_byNumMission; i++ )
		{
			if( m_Mission[i].wRoleID == wRoleID )
			{
				m_Mission[i].byState = ROLE_MIS_FAILURE_FALG;

				WPACKET packet = GETWPACKET();
				WRITE_CMD(packet, CMD_MC_MISLOG_CHANGE );
				WRITE_CHAR(packet, i );
				WRITE_SHORT(packet, m_Mission[i].wRoleID );	
				WRITE_CHAR(packet, m_Mission[i].byState );	
				m_pRoleChar->ReflectINFof( m_pRoleChar, packet );
				return TRUE;
			}
		}

		return FALSE;
	}

	BOOL CCharMission::MisHasMissionFailure( WORD wRoleID )
	{
		for( int i = 0; i < m_byNumMission; i++ )
		{
			if( m_Mission[i].wRoleID == wRoleID )
			{
				return m_Mission[i].byState == ROLE_MIS_FAILURE_FALG;
			}
		}

		return FALSE;
	}

	BOOL CCharMission::CancelRole( WORD wRoleID, WORD wScriptID )
	{
		// lua
		lua_getglobal( g_pLuaState, "CancelMission" );
		if( !lua_isfunction( g_pLuaState, -1 ) )
		{
			lua_pop( g_pLuaState, 1 );
			LG( "lua_invalidfunc", "CancelMission" );
			return FALSE;
		}

		lua_pushlightuserdata( g_pLuaState, (void*)m_pRoleChar );
		lua_pushnumber( g_pLuaState, wRoleID );
		lua_pushnumber( g_pLuaState, wScriptID );

		int nStatus = lua_pcall( g_pLuaState, 3, 1, 0 );
		if( nStatus )
		{
			//m_pRoleChar->SystemNotice( "CCharMission::CancelRole:[CancelMission]" );
			m_pRoleChar->SystemNotice( RES_STRING(GM_MISSION_CPP_00018) );
			lua_callalert( g_pLuaState, nStatus );
			lua_settop(g_pLuaState, 0);
			return FALSE;
		}

		DWORD dwResult = (DWORD)lua_tonumber( g_pLuaState, -1 );
		lua_settop(g_pLuaState, 0);
		return dwResult;
	}

	BOOL CCharMission::MisCancelRole( WORD wRoleID )
	{
		for( int i = 0; i < m_byNumMission; i++ )
		{
			if( m_Mission[i].wRoleID == wRoleID )
			{
				// npc
				if( m_Mission[i].byType == MIS_RAND_CONVOY )
				{
					for( int j = 0; j < ROLE_MAXNUM_RAND_DATA; j++ )
					{
						if( m_Mission[i].RandData[j].pData && m_Mission[i].RandData[j].wParam1 > 0 )
						{
							((CCharacter*)m_Mission[i].RandData[j].pData)->Free();
							m_Mission[i].RandData[j].pData = NULL;
						}
					}
				}

				// 
				if( CancelRole( wRoleID, m_Mission[i].wParam1 ) == FALSE )
				{
					//m_pRoleChar->SystemNotice( "ID[%d], SID[%d]", wRoleID, m_Mission[i].wParam1 );
					m_pRoleChar->SystemNotice( RES_STRING(GM_MISSION_CPP_00019), wRoleID, m_Mission[i].wParam1 );
					return FALSE;
				}

				return TRUE;
			}
		}

		//m_pRoleChar->SystemNotice( "MisCancelRole:ID[%d]", wRoleID );
		m_pRoleChar->SystemNotice( RES_STRING(GM_MISSION_CPP_00020), wRoleID );
		return FALSE;
	}

	BOOL CCharMission::MisClearRole( WORD wRoleID )
	{
		for( int i = 0; i < m_byNumMission; i++ )
		{
			if( m_Mission[i].wRoleID == wRoleID )
			{
				// npc
				if( m_Mission[i].byType == MIS_RAND_CONVOY )
				{
					for( int j = 0; j < ROLE_MAXNUM_RAND_DATA; j++ )
					{
						if( m_Mission[i].RandData[j].pData && m_Mission[i].RandData[j].wParam1 > 0 )
						{
							((CCharacter*)m_Mission[i].RandData[j].pData)->Free();
							m_Mission[i].RandData[j].pData = NULL;
						}
					}
				}

				// 
				MISSION_INFO Info[ROLE_MAXNUM_MISSION];
				memset( Info, 0, sizeof(MISSION_INFO)*ROLE_MAXNUM_MISSION);
				memcpy( Info, m_Mission, sizeof(MISSION_INFO)*m_byNumMission ); 
				memset( m_Mission + i, 0, sizeof(MISSION_INFO)*( m_byNumMission - i) );
				memcpy( m_Mission + i, Info + i + 1, sizeof(MISSION_INFO)*( m_byNumMission - i - 1 ) );
				m_byNumMission--;

				// 
				ClearRoleTrigger( wRoleID );

				// 
				MisLogClear( wRoleID );
				return TRUE;
			}
		}

		//m_pRoleChar->SystemNotice( "MisClearRole:ID[%d]", wRoleID );
		m_pRoleChar->SystemNotice( RES_STRING(GM_MISSION_CPP_00021), wRoleID );
		return FALSE;
	}

	BOOL CCharMission::MisSetFlag( WORD wRoleID, WORD wFlag )
	{
		if( m_byNumMission + 1 >= ROLE_MAXNUM_FLAG )
			return FALSE;

		for( int i = 0; i < m_byNumMission; i++ )
		{
			if( m_Mission[i].wRoleID == wRoleID )
			{
				return m_Mission[i].RoleInfo.SetFlag( wFlag, TRUE );
			}
		}
		return FALSE;
	}

	BOOL CCharMission::MisClearFlag( WORD wRoleID, WORD wFlag )
	{
		if( m_byNumMission + 1 >= ROLE_MAXNUM_FLAG )
			return FALSE;

		for( int i = 0; i < m_byNumMission; i++ )
		{
			if( m_Mission[i].wRoleID == wRoleID )
			{
				return m_Mission[i].RoleInfo.SetFlag( wFlag, FALSE );
			}
		}
		return FALSE;
	}

	BOOL CCharMission::MisIsSet( WORD wRoleID, WORD wFlag )
	{
		if( m_byNumMission + 1 >= ROLE_MAXNUM_FLAG )
			return FALSE;

		for( int i = 0; i < m_byNumMission; i++ )
		{
			if( m_Mission[i].wRoleID == wRoleID )
			{
				return m_Mission[i].RoleInfo.IsSet( wFlag );	
			}
		}
		return FALSE;
	}

	BOOL CCharMission::MisIsValid( WORD wFlag )
	{
		return m_Mission[0].RoleInfo.IsValid( wFlag );	
	}

	BOOL CCharMission::MisSetRecord( WORD wRec )
	{
		return m_RoleRecord.SetID( wRec, TRUE );
	}

	BOOL CCharMission::MisClearRecord( WORD wRec )
	{
		return m_RoleRecord.SetID( wRec, FALSE );
	}

	BOOL CCharMission::MisIsRecord( WORD wRec )
	{
		return m_RoleRecord.IsSet( wRec );
	}

	BOOL CCharMission::MisIsValidRecord( WORD wRec )
	{
		return m_RoleRecord.IsValid( wRec );
	}

	
	BOOL CCharMission::MisAddFollowNpc( WORD wRoleID, BYTE byIndex, WORD wNpcCharID, CCharacter* pNpc, BYTE byAiType )
	{
		if( byIndex >= ROLE_MAXNUM_RAND_DATA )
		{
			//m_pRoleChar->SystemNotice( "MisIsFollowNpc:NPCbyInex = %d", byIndex );
			m_pRoleChar->SystemNotice( RES_STRING(GM_MISSION_CPP_00022), byIndex );
			return FALSE;
		}

		int index = -1;
		for( int i = 0; i < m_byNumMission; i++ )
		{
			if( m_Mission[i].wRoleID == wRoleID )
			{
				index = i;
				break;
			}
		}

		if( index == -1 )
		{
			//m_pRoleChar->SystemNotice( "MisAddFollowNpc:ID=%d", wRoleID );
			m_pRoleChar->SystemNotice( RES_STRING(GM_MISSION_CPP_00023), wRoleID );
			return FALSE;
		}
		
		m_Mission[index].RandData[byIndex].pData   = pNpc;
		m_Mission[index].RandData[byIndex].wParam1 = wNpcCharID;
		m_Mission[index].RandData[byIndex].wParam2 = byAiType;
		return TRUE;
	}

	BOOL CCharMission::MisClearAllFollowNpc( WORD wRoleID )
	{
		int index = -1;
		int i = 0;
		for (; i < m_byNumMission; i++)
		{
			if( m_Mission[i].wRoleID == wRoleID )
			{
				index = i;
				break;
			}
		}

		if( index == -1 )
		{
			//m_pRoleChar->SystemNotice( "MisClearFollowNpc:ID=%d", wRoleID );
			m_pRoleChar->SystemNotice( RES_STRING(GM_MISSION_CPP_00024), wRoleID );
			return FALSE;
		}

		if( m_Mission[index].byType == MIS_RAND_CONVOY )
		{
			for( int j = 0; j < ROLE_MAXNUM_RAND_DATA; j++ )
			{
				if( m_Mission[i].RandData[j].pData && m_Mission[i].RandData[j].wParam1 > 0 )
				{
					((CCharacter*)m_Mission[i].RandData[j].pData)->Free();
					m_Mission[i].RandData[j].pData = NULL;
				}
			}
		}

		return FALSE;
	}

	BOOL CCharMission::MisClearFollowNpc( WORD wRoleID, BYTE byIndex )
	{
		if( byIndex >= ROLE_MAXNUM_RAND_DATA )
		{
			//m_pRoleChar->SystemNotice( "MisClearFollowNpc:NPCbyInex = %d", byIndex );
			m_pRoleChar->SystemNotice( RES_STRING(GM_MISSION_CPP_00025), byIndex );
			return FALSE;
		}

		int index = -1;
		for( int i = 0; i < m_byNumMission; i++ )
		{
			if( m_Mission[i].wRoleID == wRoleID )
			{
				index = i;
				break;
			}
		}

		if( index == -1 )
		{
			//m_pRoleChar->SystemNotice( "MisClearFollowNpc:ID=%d", wRoleID );
			m_pRoleChar->SystemNotice( RES_STRING(GM_MISSION_CPP_00024), wRoleID );
			return FALSE;
		}

		if( m_Mission[index].RandData[byIndex].wParam1 > 0 && m_Mission[index].RandData[byIndex].pData )
		{
			((CCharacter*)m_Mission[index].RandData[byIndex].pData)->Free();
			m_Mission[index].RandData[byIndex].pData = NULL;
			m_Mission[index].RandData[byIndex].wParam1 = 0;
			return TRUE;
		}
		return FALSE;
	}

	BOOL CCharMission::MisHasFollowNpc( WORD wRoleID, BYTE byIndex )
	{
		if( byIndex >= ROLE_MAXNUM_RAND_DATA )
		{
			//m_pRoleChar->SystemNotice( "MisHasFollowNpc:NPCbyInex = %d", byIndex );
			m_pRoleChar->SystemNotice( RES_STRING(GM_MISSION_CPP_00026), byIndex );
			return FALSE;
		}

		int index = -1;
		for( int i = 0; i < m_byNumMission; i++ )
		{
			if( m_Mission[i].wRoleID == wRoleID )
			{
				index = i;
				break;
			}
		}

		if( index == -1 )
		{
			//m_pRoleChar->SystemNotice( "MisHasFollowNpc:ID=%d", wRoleID );
			m_pRoleChar->SystemNotice( RES_STRING(GM_MISSION_CPP_00027), wRoleID );
			return FALSE;
		}

		if( m_Mission[index].RandData[byIndex].wParam1 > 0 && m_Mission[index].RandData[byIndex].pData )
			return TRUE;
		
		return FALSE;
	}

	BOOL CCharMission::MisIsFollowNpc( WORD wRoleID, BYTE byIndex, WORD wNpcCharID )
	{
		if( byIndex >= ROLE_MAXNUM_RAND_DATA )
		{
			//m_pRoleChar->SystemNotice( "MisIsFollowNpc:NPCbyInex = %d", byIndex );
			m_pRoleChar->SystemNotice( RES_STRING(GM_MISSION_CPP_00022), byIndex );
			return FALSE;
		}

		int index = -1;
		for( int i = 0; i < m_byNumMission; i++ )
		{
			if( m_Mission[i].wRoleID == wRoleID )
			{
				index = i;
				break;
			}
		}

		if( index == -1 )
		{
			//m_pRoleChar->SystemNotice( "MisIsFollowNpc:ID=%d", wRoleID );
			m_pRoleChar->SystemNotice( RES_STRING(GM_MISSION_CPP_00028), wRoleID );
			return FALSE;
		}

		return m_Mission[index].RandData[byIndex].wParam1 == wNpcCharID;
	}

	BOOL CCharMission::MisLowDistFollowNpc( WORD wRoleID, BYTE byIndex )
	{
		return FALSE;
	}

	BOOL CCharMission::MisAddRandMission( WORD wRoleID, WORD wScriptID, BYTE byType, BYTE byLevel, DWORD dwExp, DWORD dwMoney, USHORT sPrizeData, USHORT sPrizeType, BYTE byNumData )
	{
		if( m_byNumMission >= ROLE_MAXNUM_RANDMISSION )
			return FALSE;
		
		m_Mission[m_byNumMission].wRoleID = wRoleID;
		m_Mission[m_byNumMission].wParam1 = wScriptID;
		m_Mission[m_byNumMission].byState = ROLE_MIS_PENDING_FLAG;
		m_Mission[m_byNumMission].byMisType = MIS_TYPE_RAND;
		m_Mission[m_byNumMission].byType = byType;
		m_Mission[m_byNumMission].byLevel = byLevel;
		m_Mission[m_byNumMission].dwExp = dwExp;
		m_Mission[m_byNumMission].dwMoney = dwMoney;
		m_Mission[m_byNumMission].wItem = sPrizeData;
		m_Mission[m_byNumMission].wParam2 = sPrizeType;
		m_Mission[m_byNumMission].byNumData = byNumData;
		m_byNumMission++;

		// 
		MisLogAdd( wRoleID, ROLE_MIS_PENDING_FLAG );

		return TRUE;
	}
	
	BOOL CCharMission::MisHasRandMission( WORD wRoleID )
	{
		for( int i = 0; i < m_byNumMission; i++ )
		{
			if( m_Mission[i].wRoleID == wRoleID && m_Mission[i].byMisType == MIS_TYPE_RAND )
				return TRUE;
		}

		return FALSE;
	}

	BOOL CCharMission::MisSetRandMissionData( WORD wRoleID, BYTE byIndex, const mission::MISSION_DATA& RandData )
	{
		if( byIndex >= ROLE_MAXNUM_RAND_DATA )
			return FALSE;
		int index = -1;
		for( int i = 0; i < m_byNumMission; i++ )
		{
			if( m_Mission[i].wRoleID == wRoleID )
			{
				index = i;
				break;
			}
		}

		if( index == -1 ) 
		{
			//m_pRoleChar->SystemNotice( "SetRandMissionData:ID=%d", wRoleID );
			m_pRoleChar->SystemNotice( RES_STRING(GM_MISSION_CPP_00029), wRoleID );
			return FALSE;
		}
		
		memcpy( m_Mission[index].RandData + byIndex, &RandData, sizeof(MISSION_DATA) );
		return TRUE;
	}

	BOOL CCharMission::MisGetRandMission( WORD wRoleID, BYTE& byType, BYTE& byLevel, DWORD& dwExp, DWORD& dwMoney, USHORT& sPrizeData, USHORT& sPrizeType, BYTE& byNumData )
	{
		int index = -1;
		for( int i = 0; i < m_byNumMission; i++ )
		{
			if( m_Mission[i].wRoleID == wRoleID )
			{
				index = i;
				break;
			}
		}

		if( index == -1 || m_Mission[index].byMisType != MIS_TYPE_RAND ) 
		{
			//m_pRoleChar->SystemNotice( "GetRandMission:ID=%d", wRoleID );
			m_pRoleChar->SystemNotice( RES_STRING(GM_MISSION_CPP_00030), wRoleID );
			return FALSE;
		}

		wRoleID = m_Mission[index].wRoleID;
		byType = m_Mission[index].byType; // 7
		byLevel = m_Mission[index].byLevel;
		dwExp = m_Mission[index].dwExp;
		dwMoney = m_Mission[index].dwMoney;
		sPrizeData = m_Mission[index].wItem;
		sPrizeType = m_Mission[index].wParam2;
		byNumData = m_Mission[index].byNumData;
		return TRUE;
	}

	BOOL CCharMission::MisGetRandMissionData( WORD wRoleID, BYTE byIndex, mission::MISSION_DATA& RandData )
	{
		if( byIndex >= ROLE_MAXNUM_RAND_DATA )
			return FALSE;
		int index = -1;
		for( int i = 0; i < m_byNumMission; i++ )
		{
			if( m_Mission[i].wRoleID == wRoleID )
			{
				index = i;
				break;
			}
		}

		if( index == -1 || m_Mission[index].byMisType != MIS_TYPE_RAND ) 
		{
			//m_pRoleChar->SystemNotice( "GetRandMissionData:ID=%d", wRoleID );
			m_pRoleChar->SystemNotice( RES_STRING(GM_MISSION_CPP_00031), wRoleID );
			return FALSE;
		}

		memcpy( &RandData, m_Mission[index].RandData + byIndex, sizeof(MISSION_DATA) );
		return TRUE;
	}

	BOOL CCharMission::MisHasSendNpcItemFlag( WORD wRoleID, WORD wNpcID )
	{
		int index = -1;
		for( int i = 0; i < m_byNumMission; i++ )
		{
			if( m_Mission[i].wRoleID == wRoleID )
			{
				index = i;
				break;
			}
		}
			
		if( index == -1 || m_Mission[index].byMisType != MIS_TYPE_RAND ) 
		{
			//m_pRoleChar->SystemNotice( "SetRandMissionNpcItemFlag:ID=%d", wRoleID );
			m_pRoleChar->SystemNotice( RES_STRING(GM_MISSION_CPP_00032), wRoleID );
			return FALSE;
		}

		for( int i = 0; i < m_Mission[index].byNumData; i++ )
		{
			if( m_Mission[index].RandData[i].wParam1 == wNpcID && m_Mission[index].RandData[i].wParam4 == 1 )
			{				
				return TRUE; // m_pRoleChar->HasItem( m_Mission[index].RandData[i].wParam2, 1 );
			}
		}
		return FALSE;
	}

	BOOL CCharMission::MisNoSendNpcItemFlag( WORD wRoleID, WORD wNpcID )
	{
		int index = -1;
		for( int i = 0; i < m_byNumMission; i++ )
		{
			if( m_Mission[i].wRoleID == wRoleID )
			{
				index = i;
				break;
			}
		}
			
		if( index == -1 || m_Mission[index].byMisType != MIS_TYPE_RAND ) 
		{
			//m_pRoleChar->SystemNotice( "SetRandMissionNpcItemFlag:ID=%d", wRoleID );
			m_pRoleChar->SystemNotice( RES_STRING(GM_MISSION_CPP_00032), wRoleID );
			return FALSE;
		}

		for( int i = 0; i < m_Mission[index].byNumData; i++ )
		{
			if( m_Mission[index].RandData[i].wParam1 == wNpcID && m_Mission[index].RandData[i].wParam4 != 1 )
			{				
				return m_pRoleChar->HasItem( m_Mission[index].RandData[i].wParam2, 1 );
			}
		}
		return FALSE;
	}

	BOOL CCharMission::MisTakeAllRandNpcItem( WORD wRoleID )
	{
		int index = -1;
		for( int i = 0; i < m_byNumMission; i++ )
		{
			if( m_Mission[i].wRoleID == wRoleID )
			{
				index = i;
				break;
			}
		}
		
		if( index == -1 || m_Mission[index].byMisType != MIS_TYPE_RAND ) 
		{
			//m_pRoleChar->SystemNotice( "GetRandMissionNpcItem:ID=%d", wRoleID );
			m_pRoleChar->SystemNotice( RES_STRING(GM_MISSION_CPP_00033), wRoleID );
			return FALSE;
		}

		for( int i = 0; i < m_Mission[index].byNumData; i++ )
		{
			if( m_Mission[index].RandData[i].wParam4 != 1 )
			{
				m_pRoleChar->GetPlyMainCha()->TakeItem( m_Mission[index].RandData[i].wParam2, 1, "" );
				m_Mission[index].RandData[i].wParam4 = 1; // 				
			}
		}
		return TRUE;
	}

	BOOL CCharMission::MisTakeRandMissionNpcItem( WORD wRoleID, WORD wNpcID, USHORT& sItemID )
	{
		int index = -1;
		for( int i = 0; i < m_byNumMission; i++ )
		{
			if( m_Mission[i].wRoleID == wRoleID )
			{
				index = i;
				break;
			}
		}
			
		if( index == -1 || m_Mission[index].byMisType != MIS_TYPE_RAND ) 
		{
			//m_pRoleChar->SystemNotice( "GetRandMissionNpcItem:ID=%d", wRoleID );
			m_pRoleChar->SystemNotice( RES_STRING(GM_MISSION_CPP_00033), wRoleID );
			return FALSE;
		}

		for( int i = 0; i < m_Mission[index].byNumData; i++ )
		{
			if( m_Mission[index].RandData[i].wParam1 == wNpcID )
			{
				sItemID = m_Mission[index].RandData[i].wParam2;
				m_Mission[index].RandData[i].wParam4 = 1; // 
				return TRUE;
			}
		}
		return FALSE;
	}

	BOOL CCharMission::MisHasRandMissionNpc( WORD wRoleID, WORD wNpcID, WORD wAreaID )
	{
		int index = -1;
		for( int i = 0; i < m_byNumMission; i++ )
		{
			if( m_Mission[i].wRoleID == wRoleID )
			{
				index = i;
				break;
			}
		}

		if( index == -1 || m_Mission[index].byMisType != MIS_TYPE_RAND ) 
		{
			//m_pRoleChar->SystemNotice( "HasRandMissionNpc:ID=%d", wRoleID );
			m_pRoleChar->SystemNotice( RES_STRING(GM_MISSION_CPP_00034), wRoleID );
			return FALSE;
		}

		for( int i = 0; i < m_Mission[index].byNumData; i++ )
		{
			if( m_Mission[index].RandData[i].wParam1 == wNpcID &&
				m_Mission[index].RandData[i].wParam3 == wAreaID )
			{
				return TRUE;
			}
		}
		return FALSE;
	}

	BOOL CCharMission::MisAddRandMissionNum( WORD wRoleID )
	{
		int nIndex = -1;
		for( int i = 0; i < m_byNumMisCount; i++ )
		{
			if( m_MissionCount[i].wRoleID == wRoleID )
			{
				nIndex = i;
				break;
			}
		}

		if( nIndex == -1 )
		{
			if( m_byNumMisCount >= ROLE_MAXNUM_MISSIONCOUNT )
			{
				//LG( "randmission", "CCharMission::CompleteRandMission:" );
				LG( "randmission", "CCharMission::CompleteRandMission:random task take count of note has fullcannot add new random task note of compelete number" );
				return FALSE;
			}
			m_MissionCount[m_byNumMisCount].wRoleID = wRoleID;
			m_MissionCount[m_byNumMisCount].wCount  = 1;
			m_byNumMisCount++;
			return TRUE;
		}

		m_MissionCount[nIndex].wCount = 0;
		m_MissionCount[nIndex].wNum++;
		return TRUE;
	}

	BOOL CCharMission::MisCompleteRandMission( WORD wRoleID )
	{
		int nIndex = -1;
		for( int i = 0; i < m_byNumMisCount; i++ )
		{
			if( m_MissionCount[i].wRoleID == wRoleID )
			{
				nIndex = i;
				break;
			}
		}

		if( nIndex == -1 )
		{
			if( m_byNumMisCount >= ROLE_MAXNUM_MISSIONCOUNT )
			{
				//LG( "randmission", "CCharMission::CompleteRandMission:" );
				LG( "randmission", "CCharMission::CompleteRandMission:random task take count of note has fullcannot add new random task compelete note " );
				return FALSE;
			}
			m_MissionCount[m_byNumMisCount].wRoleID = wRoleID;
			m_MissionCount[m_byNumMisCount].wCount  = 1;
			m_MissionCount[nIndex].wNum = 0;
			m_byNumMisCount++;
			return TRUE;
		}

		m_MissionCount[nIndex].wCount++;
		return TRUE;
	}

	BOOL CCharMission::MisFailureRandMission( WORD wRoleID )
	{
		int nIndex = -1;
		for( int i = 0; i < m_byNumMisCount; i++ )
		{
			if( m_MissionCount[i].wRoleID == wRoleID )
			{
				nIndex = i;
				break;
			}
		}

		if( nIndex == -1 )
		{
			return TRUE;
		}

		m_MissionCount[nIndex].wCount = 0;
		m_MissionCount[nIndex].wNum = 0;
		return TRUE;
	}

	BOOL CCharMission::MisResetRandMission( WORD wRoleID )
	{
		int nIndex = -1;
		for( int i = 0; i < m_byNumMisCount; i++ )
		{
			if( m_MissionCount[i].wRoleID == wRoleID )
			{
				nIndex = i;
				break;
			}
		}

		if( nIndex == -1 )
		{
			return TRUE;
		}

		m_MissionCount[nIndex].wCount = 0;
		return TRUE;
	}

	BOOL CCharMission::MisResetRandMissionNum( WORD wRoleID )
	{
		int nIndex = -1;
		for( int i = 0; i < m_byNumMisCount; i++ )
		{
			if( m_MissionCount[i].wRoleID == wRoleID )
			{
				nIndex = i;
				break;
			}
		}

		if( nIndex == -1 )
		{
			return TRUE;
		}

		m_MissionCount[nIndex].wCount = 0;
		m_MissionCount[nIndex].wNum = 0;
		return TRUE;
	}

	WORD CCharMission::MisGetRandMissionCount( WORD wRoleID )
	{
		int nIndex = -1;
		for( int i = 0; i < m_byNumMisCount; i++ )
		{
			if( m_MissionCount[i].wRoleID == wRoleID )
			{
				nIndex = i;
				break;
			}
		}

		if( nIndex == -1 )
		{
			return 0;
		}

		return m_MissionCount[nIndex].wCount;
	}

	WORD CCharMission::MisGetRandMissionNum( WORD wRoleID )
	{
		int nIndex = -1;
		for( int i = 0; i < m_byNumMisCount; i++ )
		{
			if( m_MissionCount[i].wRoleID == wRoleID )
			{
				nIndex = i;
				break;
			}
		}

		if( nIndex == -1 )
		{
			return 0;
		}

		return m_MissionCount[nIndex].wNum;
	}

	void CCharMission::MisLogout()
	{
		m_byOnline = 0;
		//TL(CHA_OUT, m_pRoleChar->GetName(), "", "");
		TL(CHA_OUT, m_pRoleChar->GetName(), "", RES_STRING(GM_MISSION_CPP_00035));
	}

	void CCharMission::MisLogin()
	{
		m_byOnline = 1;
		//TL(CHA_ENTER, m_pRoleChar->GetName(), "", "");
		TL(CHA_ENTER, m_pRoleChar->GetName(), "", RES_STRING(GM_MISSION_CPP_00036));
	}

	void CCharMission::MisEnterMap() 
	{
		if(m_pRoleChar)
		{
			CCharacter* pMain = m_pRoleChar->GetPlyCtrlCha();

			if(pMain)
			{
				//const char* pszMap = (pMain->GetSubMap()) ? pMain->GetSubMap()->GetName() : "";
				const char* pszMap = (pMain->GetSubMap()) ? pMain->GetSubMap()->GetName() : RES_STRING(GM_MISSION_CPP_00037);
				char szData[128];
				//sprintf( szData, "%sx = %d, y = %d.", pszMap, pMain->GetPos().x, pMain->GetPos().y );
				sprintf( szData, RES_STRING(GM_MISSION_CPP_00038), pszMap, pMain->GetPos().x, pMain->GetPos().y );
				TL(CHA_ENTER, m_pRoleChar->GetName(), "", szData );

				// npc
				for( int i = 0; i < m_byNumMission; i++ )
				{
					if( m_Mission[i].byType == MIS_RAND_CONVOY )
					{
						for( int j = 0; j < ROLE_MAXNUM_RAND_DATA; j++ )
						{
							if( m_Mission[i].RandData[j].wParam1 > 0 )
							{
								if( !m_pRoleChar->ConvoyNpc( m_Mission[i].wRoleID, j, m_Mission[i].RandData[0].wParam1, 
									(BYTE)m_Mission[i].RandData[0].wParam2 ) )
								{
									//m_pRoleChar->SystemNotice( "NPC	MID(%d),NID(%d)", 
									m_pRoleChar->SystemNotice( RES_STRING(GM_MISSION_CPP_00039), 
										m_Mission[i].wRoleID, m_Mission[i].RandData[0].wParam1 );					
								}
							}
						}
					}
				}
				MisGetMisLog();
			}
		}
		else
		{

		}
	}
	
	void CCharMission::MisGooutMap() 
	{
		CCharacter* pMain = m_pRoleChar->GetPlyCtrlCha();
		const char* pszMap = (pMain->GetSubMap()) ? pMain->GetSubMap()->GetName() : "";
		char szData[128];
		//sprintf( szData, "%sx = %d, y = %d.", pszMap, pMain->GetPos().x, pMain->GetPos().y );
		sprintf( szData, RES_STRING(GM_MISSION_CPP_00040), pszMap, pMain->GetPos().x, pMain->GetPos().y );
		TL(CHA_OUT, m_pRoleChar->GetName(), "", szData );

		// npc
		for( int i = 0; i < m_byNumMission; i++ )
		{
			if( m_Mission[i].byType == MIS_RAND_CONVOY )
			{
				for( int j = 0; j < ROLE_MAXNUM_RAND_DATA; j++ )
				{
					if( m_Mission[i].RandData[j].pData && m_Mission[i].RandData[j].wParam1 > 0 )
					{
						((CCharacter*)m_Mission[i].RandData[j].pData)->Free();
						m_Mission[i].RandData[j].pData = NULL;
					}
				}
			}
		}
	}
}
