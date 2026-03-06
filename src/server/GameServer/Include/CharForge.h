// CharForge.h Created by knight-gongjian 2005.1.24.
//---------------------------------------------------------
#pragma once

#ifndef _CHARFORGE_H_
#define _CHARFORGE_H_


#include "Character.h"

//---------------------------------------------------------
_DBC_USING

class CForgeRecordSet;

namespace mission
{
	class CForgeSystem
	{
	public:
		CForgeSystem();
		virtual ~CForgeSystem();

		void	Clear();

		// 
		BOOL	LoadForgeData( char szName[] );
		
		// 
		void	ForgeItem( CCharacter& character, BYTE byIndex );

	private:

		// 
		CForgeRecordSet* m_pRecordSet;
	};

}

extern mission::CForgeSystem g_ForgeSystem;

//---------------------------------------------------------

#endif // _CHARFORGE_H_
