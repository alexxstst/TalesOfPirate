// WorldEudemon.h Created by knight-gongjian 2005.3.9.
//---------------------------------------------------------
#pragma once

#ifndef _WORLD_EUDEMON_H_
#define _WORLD_EUDEMON_H_

#include "Npc.h"
//---------------------------------------------------------

namespace mission
{
	class CWorldEudemon : public CNpc
	{
	public:
		CWorldEudemon();
		virtual ~CWorldEudemon();

		virtual void SetType() { m_byType = EUDEMON; }

		// ������Ϣ��������
		virtual HRESULT MsgProc( CCharacter& character, net::RPacket& packet );
		
		// װ�������ػ���������Ϣ
		virtual BOOL Load( const char szMsgProc[], const char szName[], dbc::uLong ulID );

	private:
		// װ�ؽű���Ϣ
		virtual BOOL InitScript( const char szFunc[], const char szName[] );
		
	};

	// ��ʱδ��
	class CEudemonManager
	{
	public:
		CEudemonManager();
		~CEudemonManager();

		// װ�������ػ����б������������ػ���
		BOOL	Load( const char szTable[] );

	private:
		CWorldEudemon m_EudemonList[ROLE_MAXNUM_EUDEMON];
		BYTE	m_byNumEudemon;
	};

	extern CWorldEudemon g_WorldEudemon;
}

//---------------------------------------------------------

#endif // _WORLD_EUDEMON_H_