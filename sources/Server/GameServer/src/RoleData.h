// RoleData.h Created by knight-gongjian 2004.11.22.
//---------------------------------------------------------
#pragma once

#ifndef _ROLEDATA_H_
#define _ROLEDATA_H_


#include "GameAppNet.h"
#include "GameCommon.h"
//---------------------------------------------------------

namespace mission
{

	// 
	typedef struct _ROLE_ITEM_COMMAND
	{
		USHORT usCmd;								// 
		char   szDesc[ROLE_MAXNUM_FUNCITEMSIZE];	// 	
		BYTE   byParam;								// 
		WORD   wParam;
		DWORD  dwParam;

		union
		{
			struct 
			{
				DWORD  dwParam1;
				DWORD  dwParam2;
			};
			char szParam[16];
		};

	} ROLE_ITEM_COMMAND, *PROLE_ITEM_COMMAND;

	// 
	typedef struct _ROLE_DESCRIPTION
	{
		BYTE byCmd;							// ID
		char szDesc[ROLE_MAXNUM_DESPSIZE];	// 

	} ROLE_DESCRIPTION, *PROLE_DESCRIPTION;

	//
	typedef struct _ROLE_FUNCTION
	{		
		BYTE byCount;		// 
		ROLE_ITEM_COMMAND FuncItem[ROLE_MAXNUM_FUNCITEM]; // 

	} ROLE_FUNCTION, *PROLE_FUNCTION;

	// npc
	// 
	typedef struct _ROLE_FLAGINFO
	{
		WORD  wRoleID;
		WORD  wParam;
		char  szFlag[ROLE_MAXNUM_FLAGSIZE];

		_ROLE_FLAGINFO()
		{
			Clear();	
		}

		void Clear()
		{
			wRoleID = 0;
			wParam  = 0;
			memset( szFlag, 0, ROLE_MAXNUM_FLAGSIZE*sizeof(char) );
		}

		BOOL Load( LPBYTE pBuf, DWORD dwSize )
		{
			if( dwSize >= ROLE_MAXNUM_RECORDSIZE ) return FALSE;
			memcpy( szFlag, pBuf, dwSize );
			return TRUE;
		}

		BOOL IsValid( WORD wFlag )
		{
			return wFlag < ROLE_MAXNUM_FLAGSIZE<<3;
		}

		BOOL IsSet( WORD wFlag )
		{
			if( wFlag >= ROLE_MAXNUM_FLAGSIZE<<3 ) return FALSE;
			DWORD dwIndex = wFlag>>3;// wFlag/ROLE_MAXNUM_INDEXSIZE;
			char szTag = 1<<(wFlag%ROLE_MAXNUM_INDEXSIZE);
			return szFlag[dwIndex] & szTag;
		}

		// 
		BOOL SetFlag( WORD wFlag, BOOL bValid )
		{
			if( wFlag >= ROLE_MAXNUM_FLAGSIZE<<3 ) return FALSE;
			DWORD dwIndex = wFlag>>3;// wFlag/ROLE_MAXNUM_INDEXSIZE;
			char szTag = 1<<(wFlag%ROLE_MAXNUM_INDEXSIZE);
			if( bValid )
			{
				szFlag[dwIndex] |= szTag;
			}
			else 
			{
				szFlag[dwIndex] &= ~szTag;
			}

			return TRUE;
		}

	} ROLE_FLAGINFO, *PROLE_FLAGINFO;

	typedef struct _ROLE_EXFLAGINFO
	{
		DWORD dwRoleID;
		WORD  wParam;
		char  szFlag[ROLE_MAXNUM_FLAGSIZE];
		char  szParam[ROLE_MAXNUM_FLAGSIZE*4];

		_ROLE_EXFLAGINFO()
		{
			Clear();
		}

		void Clear()
		{
			dwRoleID = 0;
			memset( szFlag, 0, ROLE_MAXNUM_FLAGSIZE*sizeof(char) );
		}

		BOOL Load( LPBYTE pBuf, DWORD dwSize )
		{
			if( dwSize >= ROLE_MAXNUM_RECORDSIZE ) return FALSE;
			return TRUE;
		}

		BOOL IsValid( DWORD dwFlag )
		{
			return dwFlag < ROLE_MAXNUM_FLAGSIZE<<3;
		}

		BOOL IsSet( DWORD dwFlag )
		{
			DWORD dwIndex = dwFlag>>3; // dwFlag/ROLE_MAXNUM_INDEXSIZE;
			char szTag = 1<<(dwFlag%ROLE_MAXNUM_INDEXSIZE);
			return szFlag[dwIndex] & szTag;
		}

		// 
		void SetFlag( DWORD dwFlag, BOOL bValid )
		{
			DWORD dwIndex = dwFlag>>3; // dwFlag/ROLE_MAXNUM_INDEXSIZE;
			char szTag = 1<<(dwFlag%ROLE_MAXNUM_INDEXSIZE);
			if( bValid )
			{
				szFlag[dwIndex] |= szTag;
			}
			else 
			{
				szFlag[dwIndex] &= ~szTag;
			}

			dwIndex = dwFlag>>1;
			DWORD dwTag = dwFlag&1;
			if( dwTag == 0 )
			{
				// 
				szParam[dwIndex] &= 0xF;
			}
			else
			{
				// 
				szParam[dwIndex] &= 0xF0;
			}
		}

		// 
		BOOL SetParam( DWORD dwFlag, BYTE byParam )
		{
			if( byParam >= ROLE_MAXVALUE_PARAM )
				return FALSE;

			DWORD dwIndex = dwFlag>>1;
			DWORD dwTag = dwFlag&1;
			if( dwTag == 0 )
			{
				// 
				szParam[dwIndex] &= 0xF;
				szParam[dwIndex] |= byParam<<4;
			}
			else
			{
				// 
				szParam[dwIndex] &= 0xF0;
				szParam[dwIndex] |= byParam;
			}
			return TRUE;
		}

		BYTE GetParam( DWORD dwFlag )
		{
			DWORD dwIndex = dwFlag>>1;
			DWORD dwTag = dwFlag&1;
			if( dwTag == 0 )
			{
				// 
				return (szParam[dwIndex]&0xF0)>>4;
			}
			else
			{
				// 
				return szParam[dwIndex]&0xF;
			}

			return TRUE;
		}
		
	} ROLE_EXFLAGINFO, *PROLE_EXFLAGINFO;

	// 
	typedef struct _ROLE_RECORDINFO
	{
		char  szID[ROLE_MAXNUM_RECORDSIZE];

		_ROLE_RECORDINFO()
		{
			Clear();
		}

		BOOL Load( LPBYTE pBuf, DWORD dwSize )
		{
			if( dwSize >= ROLE_MAXNUM_RECORDSIZE<<3 ) return FALSE;
			memcpy( szID, pBuf, dwSize );
			return TRUE;
		}

		void Clear()
		{
			memset( szID, 0, ROLE_MAXNUM_RECORDSIZE*sizeof(char) );
		}

		BOOL IsValid( DWORD dwID )
		{
			return dwID < ROLE_MAXNUM_RECORDSIZE<<3;
		}

		BOOL IsSet( DWORD dwID )
		{
			if( dwID >= ROLE_MAXNUM_RECORDSIZE<<3 ) return FALSE;
			DWORD dwIndex = dwID>>3; // dwID/ROLE_MAXNUM_INDEXSIZE;
			char szTag = 1<<(dwID%ROLE_MAXNUM_INDEXSIZE);
			return szID[dwIndex] & szTag;
		}

		// 
		BOOL SetID( DWORD dwID, BOOL bValid )
		{
			if( dwID >= ROLE_MAXNUM_RECORDSIZE<<3 ) return FALSE;
			DWORD dwIndex = dwID>>3; // dwID/ROLE_MAXNUM_INDEXSIZE;
			char szTag = 1<<(dwID%ROLE_MAXNUM_INDEXSIZE);
			if( bValid )
			{
				szID[dwIndex] |= szTag;
			}
			else 
			{
				szID[dwIndex] &= ~szTag;
			}

			return TRUE;
		}

	} ROLE_RECORDINFO, *PROLE_RECORDINFO;


	// 
	typedef struct _TRIGGER_DATA
	{
		WORD wTriggerID;	// ID
		WORD wMissionID;	// ID()
		BYTE byType;		// 
		WORD wParam1;		// 3
		WORD wParam2;		// 4,
		WORD wParam3;		// ()
		WORD wParam4;		// 
		WORD wParam5;
		WORD wParam6;

		BYTE byIsDel : 1;	// 
		BYTE byParam : 7;

	} TRIGGER_DATA, *PTRIGGER_DATA;

	enum { TRIGGER_VALID, TRIGGER_DELED };

	// 
	typedef struct _NPC_TRIGGER_DATA
	{
		WORD wTID;			// ID
		WORD wParam;
		BYTE byType;		// 
		WORD wParam1;		// 
		WORD wParam2;
		WORD wParam3;
		WORD wParam4;

	} NPC_TRIGGER_DATA, *PNPC_TRIGGER_DATA;

	// 
	typedef struct _MISSION_DATA
	{
		WORD	wParam1;				// 
		WORD	wParam2;
		WORD	wParam3;
		WORD	wParam4;
		WORD	wParam5;
		WORD	wParam6;

		void*	pData;					// 
	} MISSION_DATA, *PMISSION_DATA;

	typedef struct _MISSION_INFO
	{
		WORD	wRoleID;				// ID
		BYTE	byState : 3;			// 
		BYTE	byMisType : 5;			// 
		BYTE	byType;					// 
		BYTE	byLevel;				// 
		BYTE	byNumData;				// 
		DWORD	dwExp;					// 
		DWORD	dwMoney;				// 
		WORD	wItem;					// 01
		WORD	wParam1;				// 
		WORD	wParam2;
		ROLE_FLAGINFO RoleInfo;			// ID
		MISSION_DATA RandData[ROLE_MAXNUM_RAND_DATA];	// 
		
	} MISSION_INFO, *PMISSION_INFO;

	// 
	typedef struct _RAND_MISSION_COUNT
	{
		WORD	wRoleID;				// ID
		WORD	wCount;					// 
		WORD	wNum;					// 

	} RAND_MISSION_COUNT, *PRAND_MISSION_COUNT;

	// NPC
	typedef struct _STATE_DATA
	{
		BYTE byID : 5;		// npc
		BYTE byState : 3;	// 
	
	} STATE_DATA, *PSTATE_DATA;

	typedef struct _MISSION_STATE
	{
		DWORD dwNpcID;
		BYTE  byNpcState;
		BYTE  byMisNum;			// 
		STATE_DATA StateInfo[ROLE_MAXNUM_MISSIONSTATE];	// 
	
	} MISSION_STATE, *PMISSION_STATE;
}

//---------------------------------------------------------

#endif // _ROLEDATA_H_
