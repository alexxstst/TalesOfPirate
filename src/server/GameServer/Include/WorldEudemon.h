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

		// 
		virtual HRESULT MsgProc( CCharacter& character, RPACKET& packet );
		
		// 
		virtual BOOL Load( const char szMsgProc[], const char szName[], dbc::uLong ulID );

	private:
		// 
		virtual BOOL InitScript( const char szFunc[], const char szName[] );
		
	};

	// 
	class CEudemonManager
	{
	public:
		CEudemonManager();
		~CEudemonManager();

		// 
		BOOL	Load( const char szTable[] );

	private:
		CWorldEudemon m_EudemonList[ROLE_MAXNUM_EUDEMON];
		BYTE	m_byNumEudemon;
	};

	extern CWorldEudemon g_WorldEudemon;
}

//---------------------------------------------------------

#endif // _WORLD_EUDEMON_H_
