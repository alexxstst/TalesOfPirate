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

		// 网络消息处理函数
		virtual HRESULT MsgProc( CCharacter& character, RPACKET packet );
		
		// 装载世界守护神配置信息
		virtual BOOL Load( const char szMsgProc[], const char szName[], dbc::uLong ulID );

	private:
		// 装载脚本信息
		virtual BOOL InitScript( const char szFunc[], const char szName[] );
		
	};

	// 暂时未用
	class CEudemonManager
	{
	public:
		CEudemonManager();
		~CEudemonManager();

		// 装载世界守护神列表，生成世界守护神
		BOOL	Load( const char szTable[] );

	private:
		CWorldEudemon m_EudemonList[ROLE_MAXNUM_EUDEMON];
		BYTE	m_byNumEudemon;
	};

	extern CWorldEudemon g_WorldEudemon;
}

//---------------------------------------------------------

#endif // _WORLD_EUDEMON_H_