#pragma once
#include "UIGlobalVar.h"
#include "NetProtocol.h"

namespace GUI
{
	class CForm;
	class CMemoEx;
	class CTextButton;

	// 
	class CMissionMgr : public CUIInterface
	{
	public:
		CMissionMgr();
		virtual ~CMissionMgr();

		void ShowMissionPage( DWORD dwNpcID, BYTE byCmd, const NET_MISPAGE& page );

	protected:
		bool Init();
		void End();
		void CloseForm();

	private:
		// 
		static void _MouseEvent( CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey );
		static void _ItemClickEvent( std::string strItem );

		CForm*			m_pMisForm;
		CMemoEx*		m_pMisInfo;
		CTextButton*	m_pMisBtn1;
		CTextButton*	m_pMisBtn2;
		CTextButton*	m_pMisClose;

		DWORD	m_dwNpcID;
		BYTE	m_byMisCmd;
	};

}

