#pragma once

#include "uiform.h"
#include "uimemo.h"
#include "uiglobalvar.h"

namespace GUI
{
	class CHelpInfoMgr : public CUIInterface
	{
		CForm*		m_pFormMain;
		CMemoEx*	m_pMemoContent;

	public:
		CHelpInfoMgr() : m_pFormMain( NULL ), m_pMemoContent( NULL ) {}
		virtual ~CHelpInfoMgr() {}

		void ShowHelpInfo( bool show, const char* HelpTitle );
		bool IsShown();

	private:
		virtual bool Init();

		static void _ItemClickEvent( std::string strItem );
	};
}
