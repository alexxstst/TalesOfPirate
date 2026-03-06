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

	#define EN_OK						 0		// 
	#define EN_FAILER					-1		// 
	#define INVALID_SCRIPT_NPCHANDLE	USHORT(-1)		// NPCID

	//	
	class CNpc : public CCharacter
	{
	public:
		enum NPC_TYPE { NPC, TALK, TRADE, TRADE_AGENCY, ROLE, FIGHT, EUDEMON };

		CNpc();
		virtual ~CNpc();

		// 
		virtual CNpc* IsNpc() { return this; }
		virtual void SetType() { m_byType = NPC; }
		BYTE GetType() { return m_byType; }
		BYTE GetShowType() { return m_byShowType; }
		
		// npc
		virtual BOOL Load( const CNpcRecord& recNpc, const CChaRecord& recChar );

		// 
		virtual HRESULT MsgProc( CCharacter& character, RPACKET& packet );

		// 
		virtual BOOL MissionProc( CCharacter& character, BYTE& byState );

		// npc
		virtual BOOL IsMapNpc( const char szMap[], USHORT sID );

		// 
		virtual BOOL AddNpcTrigger( WORD wID, mission::TRIGGER_EVENT e, WORD wParam1, WORD wParam2, WORD wParam3, WORD wParam4 );

		// 
		virtual BOOL EventProc( TRIGGER_EVENT e, WPARAM wParam, LPARAM lParam );

		//npcID
		virtual void	SetScriptID( USHORT sID ) { m_sScriptID = sID; }
		virtual USHORT	GetScriptID() { return m_sScriptID; }
		virtual void	SetNpcHasMission( BOOL bHasMission ) { m_bHasMission = bHasMission; }
		virtual BOOL	GetNpcHasMission() { return m_bHasMission; }
		virtual const char* GetInitFunc() { return m_szMsgProc; }

		// NPC
		virtual void	Summoned( USHORT sTime ) {};
		
		const char* GetNpcName() { return m_szName; }

	protected:
		//
		virtual void Clear();

		// npc
		BYTE	m_byType;

		// npc
		BYTE	m_byShowType;

		// npc
		USHORT	m_sNpcID;

		// npc
		char	m_szMsgProc[ROLE_MAXSIZE_MSGPROC];

		// npcID
		USHORT	m_sScriptID;

		// npc
		BOOL	m_bHasMission;

		char m_szName[128];

	};

	class CTalkNpc : public CNpc
	{
	public:
		CTalkNpc();
		virtual ~CTalkNpc();

		// 
		virtual void SetType() { m_byType = TALK; }

		// npc
		virtual BOOL Load( const CNpcRecord& recNpc, const CChaRecord& recChar );
		
		// 
		virtual BOOL InitScript( const char szFunc[], const char szName[] );

		//
		virtual HRESULT MsgProc( CCharacter& character, RPACKET& packet );

		//
		virtual BOOL MissionProc( CCharacter& character, BYTE& byState );

		// npc
		virtual BOOL IsMapNpc( const char szMap[], USHORT sID );

		// 
		virtual BOOL AddNpcTrigger( WORD wID, mission::TRIGGER_EVENT e, WORD wParam1, WORD wParam2, WORD wParam3, WORD wParam4 );

		// 
		virtual BOOL EventProc( TRIGGER_EVENT e, WPARAM wParam, LPARAM lParam );

		// NPC
		virtual void Summoned( USHORT sTime );

	protected:
		// npcnpc
		virtual BOOL Load( const char szNpcScript[] );
		
		// 
		virtual void Clear();

		// 
		void	ClearTrigger( WORD wIndex );

		// 
		void	TimeOut( USHORT sTime );

		// npc
		BYTE				m_byNumTrigger;
		NPC_TRIGGER_DATA	m_Trigger[ROLE_MAXNUM_NPCTRIGGER];

		// 
		USHORT m_sTime;		// 
		BOOL   m_bSummoned; // NPC
	};
	
	class CTradeNpc : public CTalkNpc
	{
	public:
		enum ITEMCOUNT_TYPE { SINGLE, MULTIPLE };
		CTradeNpc();
		virtual ~CTradeNpc();

		virtual void SetType() { m_byType = TRADE; }

		// 
		virtual BOOL Sale( CCharacter& character, RPACKET& packet );
		virtual BOOL Buy( CCharacter& character, RPACKET& packet );

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

		// 
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
		// 
		USHORT	m_sRoleList[ROLE_MAXNUM_CAPACITY];
		USHORT	m_sNumRole;
	};

	extern CTalkNpc* g_TalkNpc;	// npcnpc
}

//---------------------------------------------------------

#endif // _NPC_H_
