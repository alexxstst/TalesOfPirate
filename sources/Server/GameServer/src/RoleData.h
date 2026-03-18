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

	// 任务功能项命令结构
	typedef struct _ROLE_ITEM_COMMAND
	{
		USHORT usCmd;								// 功能项目命令
		char   szDesc[ROLE_MAXNUM_FUNCITEMSIZE];	// 描述信息	
		BYTE   byParam;								// 参数信息
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

	// 显示任务对话描述信息
	typedef struct _ROLE_DESCRIPTION
	{
		BYTE byCmd;							// 描述对话栏按钮命令ID
		char szDesc[ROLE_MAXNUM_DESPSIZE];	// 描述信息

	} ROLE_DESCRIPTION, *PROLE_DESCRIPTION;

	//　显示任务功能描述信息
	typedef struct _ROLE_FUNCTION
	{		
		BYTE byCount;		// 功能项计数
		ROLE_ITEM_COMMAND FuncItem[ROLE_MAXNUM_FUNCITEM]; // 对应的功能项目

	} ROLE_FUNCTION, *PROLE_FUNCTION;

	// npc信息存储结构
	// 历史任务单个标记结构
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

		// 设置或者清除设定标记
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

		// 设置或者清除设定标记
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
				// 高四位
				szParam[dwIndex] &= 0xF;
			}
			else
			{
				// 低四位
				szParam[dwIndex] &= 0xF0;
			}
		}

		// 设置标记相应参数信息
		BOOL SetParam( DWORD dwFlag, BYTE byParam )
		{
			if( byParam >= ROLE_MAXVALUE_PARAM )
				return FALSE;

			DWORD dwIndex = dwFlag>>1;
			DWORD dwTag = dwFlag&1;
			if( dwTag == 0 )
			{
				// 高四位
				szParam[dwIndex] &= 0xF;
				szParam[dwIndex] |= byParam<<4;
			}
			else
			{
				// 低四位
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
				// 高四位
				return (szParam[dwIndex]&0xF0)>>4;
			}
			else
			{
				// 低四位
				return szParam[dwIndex]&0xF;
			}

			return TRUE;
		}
		
	} ROLE_EXFLAGINFO, *PROLE_EXFLAGINFO;

	// 所有角色最大可执行历史任务标记结构
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

		// 设置或者清除设定标记
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


	// 触发器信息记录
	typedef struct _TRIGGER_DATA
	{
		WORD wTriggerID;	// 触发器ID
		WORD wMissionID;	// 任务ID(任务取消时需要检测，清除相应任务的触发器)
		BYTE byType;		// 事件类型
		WORD wParam1;		// 触发器参数3
		WORD wParam2;		// 触发器参数4,动作计数
		WORD wParam3;		// (如果是随机任务，则用作开始记录标签的起始位置，传递到触发器脚本中使用)
		WORD wParam4;		// 
		WORD wParam5;
		WORD wParam6;

		BYTE byIsDel : 1;	// 标志触发器是否应该删除
		BYTE byParam : 7;

	} TRIGGER_DATA, *PTRIGGER_DATA;

	enum { TRIGGER_VALID, TRIGGER_DELED };

	// 触发器信息记录
	typedef struct _NPC_TRIGGER_DATA
	{
		WORD wTID;			// 触发器ID
		WORD wParam;
		BYTE byType;		// 事件类型
		WORD wParam1;		// 触发器参数
		WORD wParam2;
		WORD wParam3;
		WORD wParam4;

	} NPC_TRIGGER_DATA, *PNPC_TRIGGER_DATA;

	// 随机任务数据信息
	typedef struct _MISSION_DATA
	{
		WORD	wParam1;				// 随机任务的执行内容信息参数
		WORD	wParam2;
		WORD	wParam3;
		WORD	wParam4;
		WORD	wParam5;
		WORD	wParam6;

		void*	pData;					// 运行时临时数据
	} MISSION_DATA, *PMISSION_DATA;

	typedef struct _MISSION_INFO
	{
		WORD	wRoleID;				// 任务ID
		BYTE	byState : 3;			// 任务进行状态
		BYTE	byMisType : 5;			// 任务类形
		BYTE	byType;					// 随机任务信息类型
		BYTE	byLevel;				// 等级信息
		BYTE	byNumData;				// 执行内容计数
		DWORD	dwExp;					// 执行该任务可得的经验
		DWORD	dwMoney;				// 执行该任务的金钱奖励
		WORD	wItem;					// 随机任务的高级物品奖励（值为0表示无，数量都为1）
		WORD	wParam1;				// 参数
		WORD	wParam2;
		ROLE_FLAGINFO RoleInfo;			// 标志和任务ID信息
		MISSION_DATA RandData[ROLE_MAXNUM_RAND_DATA];	// 随机任务数据信息
		
	} MISSION_INFO, *PMISSION_INFO;

	// 完成随机任务计数结构
	typedef struct _RAND_MISSION_COUNT
	{
		WORD	wRoleID;				// 任务ID
		WORD	wCount;					// 任务完成计数
		WORD	wNum;					// 完成任务的圈数

	} RAND_MISSION_COUNT, *PRAND_MISSION_COUNT;

	// NPC任务信息状态记录
	typedef struct _STATE_DATA
	{
		BYTE byID : 5;		// 每个npc自带的任务信息列表的索引
		BYTE byState : 3;	// 单个任务状态
	
	} STATE_DATA, *PSTATE_DATA;

	typedef struct _MISSION_STATE
	{
		DWORD dwNpcID;
		BYTE  byNpcState;
		BYTE  byMisNum;			// 已完成任务计数
		STATE_DATA StateInfo[ROLE_MAXNUM_MISSIONSTATE];	// 任务数据信息
	
	} MISSION_STATE, *PMISSION_STATE;
}

//---------------------------------------------------------

#endif // _ROLEDATA_H_