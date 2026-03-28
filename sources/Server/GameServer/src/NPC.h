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

	#define EN_OK						 0		// ��ȷ
	#define EN_FAILER					-1		// ����
	#define INVALID_SCRIPT_NPCHANDLE	USHORT(-1)		// ��Ч��NPC�ű�ID

	//	
	class CNpc : public CCharacter
	{
	public:
		enum NPC_TYPE { NPC, TALK, TRADE, TRADE_AGENCY, ROLE, FIGHT, EUDEMON };

		CNpc();
		virtual ~CNpc();

		// ���������Ϣ
		virtual CNpc* IsNpc() { return this; }
		virtual void SetType() { m_byType = NPC; }
		BYTE GetType() { return m_byType; }
		BYTE GetShowType() { return m_byShowType; }
		
		// װ��npc������Ϣ
		virtual BOOL Load( const CNpcRecord& recNpc, const CChaRecord& recChar );

		// ������Ϣ��������
		virtual HRESULT MsgProc( CCharacter& character, net::RPacket& packet );

		// ����״̬��������
		virtual BOOL MissionProc( CCharacter& character, BYTE& byState );

		// �ж�npc�ĵ�ͼ����
		virtual BOOL IsMapNpc( const char szMap[], USHORT sID );

		// ���Ӵ�������Ϣ
		virtual BOOL AddNpcTrigger( WORD wID, mission::TRIGGER_EVENT e, WORD wParam1, WORD wParam2, WORD wParam3, WORD wParam4 );

		// �������¼�����
		virtual BOOL EventProc( TRIGGER_EVENT e, WPARAM wParam, LPARAM lParam );

		//����npc�ű��Ի��ͽ�����Ϣ��ԴID
		virtual void	SetScriptID( USHORT sID ) { m_sScriptID = sID; }
		virtual USHORT	GetScriptID() { return m_sScriptID; }
		virtual void	SetNpcHasMission( BOOL bHasMission ) { m_bHasMission = bHasMission; }
		virtual BOOL	GetNpcHasMission() { return m_bHasMission; }
		virtual const char* GetInitFunc() { return m_szMsgProc; }

		// NPC���ٻ�
		virtual void	Summoned( USHORT sTime ) {};
		
		const char* GetNpcName() { return m_szName; }

	protected:
		//
		virtual void Clear();

		// npc������Ϣ
		BYTE	m_byType;

		// npc�ͻ�����ʾ��ͬ����
		BYTE	m_byShowType;

		// npc���ñ����
		USHORT	m_sNpcID;

		// npc��Ϣ�ṹ
		char	m_szMsgProc[ROLE_MAXSIZE_MSGPROC];

		// npc�ű���ϢID
		USHORT	m_sScriptID;

		// npc�Ƿ�Я��������Ϣ
		BOOL	m_bHasMission;

		char m_szName[128];

	};

	class CTalkNpc : public CNpc
	{
	public:
		CTalkNpc();
		virtual ~CTalkNpc();

		// ���������Ϣ
		virtual void SetType() { m_byType = TALK; }

		// װ��npc������Ϣ
		virtual BOOL Load( const CNpcRecord& recNpc, const CChaRecord& recChar );
		
		// װ�ؽű���Ϣ
		virtual BOOL InitScript( const char szFunc[], const char szName[] );

		//
		virtual HRESULT MsgProc( CCharacter& character, net::RPacket& packet );

		//
		virtual BOOL MissionProc( CCharacter& character, BYTE& byState );

		// �ж�npc�ĵ�ͼ����
		virtual BOOL IsMapNpc( const char szMap[], USHORT sID );

		// ���Ӵ�������Ϣ
		virtual BOOL AddNpcTrigger( WORD wID, mission::TRIGGER_EVENT e, WORD wParam1, WORD wParam2, WORD wParam3, WORD wParam4 );

		// �������¼�����
		virtual BOOL EventProc( TRIGGER_EVENT e, WPARAM wParam, LPARAM lParam );

		// NPC���ٻ�
		virtual void Summoned( USHORT sTime );

	protected:
		// װ��npc�ű���ʼ��npc��Ϣ
		virtual BOOL Load( const char szNpcScript[] );
		
		// 
		virtual void Clear();

		// �����������Ϣ
		void	ClearTrigger( WORD wIndex );

		// �����¼���������
		void	TimeOut( USHORT sTime );

		// npcЯ���Ĵ�������Ϣ
		BYTE				m_byNumTrigger;
		NPC_TRIGGER_DATA	m_Trigger[ROLE_MAXNUM_NPCTRIGGER];

		// ��������Я��ĳ������Ľ�ɫʱ����
		USHORT m_sTime;		// ʣ����ʾʱ��
		BOOL   m_bSummoned; // �Ƿ����ٻ���NPC
	};
	
	class CTradeNpc : public CTalkNpc
	{
	public:
		enum ITEMCOUNT_TYPE { SINGLE, MULTIPLE };
		CTradeNpc();
		virtual ~CTradeNpc();

		virtual void SetType() { m_byType = TRADE; }

		// ���׺����ӿ�
		virtual BOOL Sale( CCharacter& character, net::RPacket& packet );
		virtual BOOL Buy( CCharacter& character, net::RPacket& packet );

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

		// ��Ʒװ����Ϣ
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
		// ������Ϣ
		USHORT	m_sRoleList[ROLE_MAXNUM_CAPACITY];
		USHORT	m_sNumRole;
	};

	extern CTalkNpc* g_TalkNpc;	// ��ʼ��npc�ű�ʱ������ȫ��npc��ָ��
}

//---------------------------------------------------------

#endif // _NPC_H_
