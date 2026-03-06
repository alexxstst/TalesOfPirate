#pragma once

#include "UIGlobalVar.h"

struct stNetTeamFightAsk;

namespace GUI
{
	class CForm;
	class CListView;

	class CPkDialog : public CUIInterface	
	{
	public:
		CPkDialog() { }
		virtual ~CPkDialog() { }

		void		SetStartDialogContent(const stNetTeamFightAsk & sNetTeamFightAsk);
		void		SetEndDialogContent();
		void		ShowStartDialog(bool bShow = true);
		void		ShowEndDialog(bool bShow = true);

		std::string		ShowStartDialogDebug(const stNetTeamFightAsk & sNetTeamFightAsk);

	protected:
		virtual bool Init();

	private:
		static void _MainMousePkStartEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
		static void _MainMousePkEndEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);

		void		ClearStartDialog();
		void		ClearEndDialog();

	private:

		CForm*			frmTeamPkStart;
		CForm*			frmTeamPkEnd;

		static const int TEAM_NUM = 2;
		static const int MEM_NUM = 5;
		static const int S_ITEM_NUM = 5;
		static const int E_ITEM_NUM = 3;

		static const int TEAM_LEFT = 0;
		static const int TEAM_RIGHT = 1;

		static const int ITEM_NAME = 0;
		static const int ITEM_LEVEL = 1;
		static const int ITEM_JOB = 2;

		CListView*		lvStartTeams[TEAM_NUM];
		CListView*		lvEndTeams[TEAM_NUM];

		//CLabel*			labStartItems[TEAM_NUM][MEM_NUM][S_ITEM_NUM];

		//CLabel*			labEndItems[TEAM_NUM][MEM_NUM][E_ITEM_NUM];


	}; // end of CPkDialog class

} // end of GUI namespace
