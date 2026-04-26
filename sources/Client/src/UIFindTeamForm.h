#pragma once

#include "uiglobalvar.h"
#include "uiform.h"
#include "uiformmgr.h"
#include "uilabel.h"
#include "uitextbutton.h"
#include "uifastcommand.h"
#include <string>


namespace GUI {
	class CFindTeamMgr : public CUIInterface {
	public:
		CFindTeamMgr();
		~CFindTeamMgr();

		void ShowFindTeamForm(bool bShow = true);
		bool IsShowFom() {
			return frmFindTeam->GetIsShow();
		} //	Add by alfred.shi 20080902
		void SetFindTeamPage(int nCurPage, int nPageNum);
		void AddFindTeamInfo(int nSeq, const char* szName, long nLevel, long nJob, const char* szPlace);
		void RemoveTeamInfo();
		void SetOwnFindTeamState(bool bState);
		void FindTeamAsk(const char* szName);

		static const int FINDTEAM_PAGE_SIZE = 10; // 
		static const int FINDTEAM_INTERVAL = 60; // 60

	protected:
		virtual bool Init();
		virtual void CloseForm();

	private:
		//
		// 
		//
		CForm* frmFindTeam;

		CLabelEx* labName[FINDTEAM_PAGE_SIZE]; // 
		CLabelEx* labLevel[FINDTEAM_PAGE_SIZE]; // 
		CLabelEx* labJob[FINDTEAM_PAGE_SIZE]; // 
		CLabelEx* labPlace[FINDTEAM_PAGE_SIZE]; // 
		CTextButton* btnSubmit[FINDTEAM_PAGE_SIZE]; // 

		CTextButton* btnAddme;
		CTextButton* btnDelme;
		CLabelEx* labListPage; // 

		int m_nCurPage; // 
		int m_nPageNum; // 
		DWORD m_dwLastTick; // 
		std::string m_strTeamLeader; // 

		static void _evtFindTeamMouseButton(CCompent* pSender, int nMsgType, int x, int y, DWORD dwKey); // 
		static void _evtFindTeamCheckEvent(CCompent* pSender, int nMsgType, int x, int y, DWORD dwKey); // 
	};
}
