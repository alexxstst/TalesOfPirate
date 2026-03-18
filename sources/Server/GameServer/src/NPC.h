// npc.h Created by knight-gongjian 2004.11.19.
//---------------------------------------------------------
#pragma once

#ifndef _NPC_H_
#define _NPC_H_

#include "RoleData.h"
#include "Character.h"
//---------------------------------------------------------

class CNpcRecord;
class CChaRecord;

namespace mission
{
	using namespace dbc;

	#define EN_OK						 0		// 正确
	#define EN_FAILER					-1		// 错误
	#define INVALID_SCRIPT_NPCHANDLE	USHORT(-1)		// 无效的NPC脚本ID

	//	
	class CNpc : public CCharacter
	{
	public:
		enum NPC_TYPE { NPC, TALK, TRADE, TRADE_AGENCY, ROLE, FIGHT, EUDEMON };

		CNpc();
		virtual ~CNpc();

		// 类的类型信息
		virtual CNpc* IsNpc() { return this; }
		virtual void SetType() { m_byType = NPC; }
		BYTE GetType() { return m_byType; }
		BYTE GetShowType() { return m_byShowType; }
		
		// 装载npc配置信息
		virtual BOOL Load( const CNpcRecord& recNpc, const CChaRecord& recChar );

		// 网络消息处理函数
		virtual HRESULT MsgProc( CCharacter& character, RPACKET packet );

		// 任务状态处理函数
		virtual BOOL MissionProc( CCharacter& character, BYTE& byState );

		// 判定npc的地图属性
		virtual BOOL IsMapNpc( const char szMap[], USHORT sID );

		// 添加触发器信息
		virtual BOOL AddNpcTrigger( WORD wID, mission::TRIGGER_EVENT e, WORD wParam1, WORD wParam2, WORD wParam3, WORD wParam4 );

		// 触发器事件处理
		virtual BOOL EventProc( TRIGGER_EVENT e, WPARAM wParam, LPARAM lParam );

		//设置npc脚本对话和交易信息资源ID
		virtual void	SetScriptID( USHORT sID ) { m_sScriptID = sID; }
		virtual USHORT	GetScriptID() { return m_sScriptID; }
		virtual void	SetNpcHasMission( BOOL bHasMission ) { m_bHasMission = bHasMission; }
		virtual BOOL	GetNpcHasMission() { return m_bHasMission; }
		virtual const char* GetInitFunc() { return m_szMsgProc; }

		// NPC被召唤
		virtual void	Summoned( USHORT sTime ) {};
		
		const char* GetNpcName() { return m_szName; }

	protected:
		//
		virtual void Clear();

		// npc类型信息
		BYTE	m_byType;

		// npc客户端显示不同类型
		BYTE	m_byShowType;

		// npc配置表编号
		USHORT	m_sNpcID;

		// npc信息结构
		char	m_szMsgProc[ROLE_MAXSIZE_MSGPROC];

		// npc脚本信息ID
		USHORT	m_sScriptID;

		// npc是否携带任务信息
		BOOL	m_bHasMission;

		char m_szName[128];

	};

	class CTalkNpc : public CNpc
	{
	public:
		CTalkNpc();
		virtual ~CTalkNpc();

		// 类的类型信息
		virtual void SetType() { m_byType = TALK; }

		// 装载npc配置信息
		virtual BOOL Load( const CNpcRecord& recNpc, const CChaRecord& recChar );
		
		// 装载脚本信息
		virtual BOOL InitScript( const char szFunc[], const char szName[] );

		//
		virtual HRESULT MsgProc( CCharacter& character, RPACKET packet );

		//
		virtual BOOL MissionProc( CCharacter& character, BYTE& byState );

		// 判定npc的地图属性
		virtual BOOL IsMapNpc( const char szMap[], USHORT sID );

		// 添加触发器信息
		virtual BOOL AddNpcTrigger( WORD wID, mission::TRIGGER_EVENT e, WORD wParam1, WORD wParam2, WORD wParam3, WORD wParam4 );

		// 触发器事件处理
		virtual BOOL EventProc( TRIGGER_EVENT e, WPARAM wParam, LPARAM lParam );

		// NPC被召唤
		virtual void Summoned( USHORT sTime );

	protected:
		// 装载npc脚本初始化npc信息
		virtual BOOL Load( const char szNpcScript[] );
		
		// 
		virtual void Clear();

		// 清除触发器信息
		void	ClearTrigger( WORD wIndex );

		// 任务事件处理函数
		void	TimeOut( USHORT sTime );

		// npc携带的触发器信息
		BYTE				m_byNumTrigger;
		NPC_TRIGGER_DATA	m_Trigger[ROLE_MAXNUM_NPCTRIGGER];

		// 当看到有携带某个任务的角色时出现
		USHORT m_sTime;		// 剩余显示时间
		BOOL   m_bSummoned; // 是否是召唤的NPC
	};
	
	class CTradeNpc : public CTalkNpc
	{
	public:
		enum ITEMCOUNT_TYPE { SINGLE, MULTIPLE };
		CTradeNpc();
		virtual ~CTradeNpc();

		virtual void SetType() { m_byType = TRADE; }

		// 交易函数接口
		virtual BOOL Sale( CCharacter& character, RPACKET packet );
		virtual BOOL Buy( CCharacter& character, RPACKET packet );

	private:

	};

	class CTradeAgencyNpc : public CTalkNpc
	{
	public:
		CTradeAgencyNpc();
		virtual ~CTradeAgencyNpc();

		virtual void SetType() { m_byType = TRADE_AGENCY; }

	private:
		// fixed me to remove
		struct AGENCY_ITEM
		{
			USHORT sItemID;
			BYTE   byType;
			BYTE   byCount;
			DWORD  dwOwnerID;
		};

		// 物品装备信息
		AGENCY_ITEM	m_sItemList[ROLE_MAXNUM_CAPACITY];
		USHORT		m_sNumItem;
	};

	class CRoleNpc : public CTalkNpc
	{
	public:
		CRoleNpc();
		virtual ~CRoleNpc();

		virtual void SetType() { m_byType = ROLE; }
		
	private:
		// 任务信息
		USHORT	m_sRoleList[ROLE_MAXNUM_CAPACITY];
		USHORT	m_sNumRole;
	};

	extern CTalkNpc* g_TalkNpc;	// 初始化npc脚本时候设置全局npc类指针
}

//---------------------------------------------------------

#endif // _NPC_H_
