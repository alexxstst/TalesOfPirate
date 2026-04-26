#pragma once
#include "UIGlobalVar.h"


#define bpHP 10
#define bpSP 10
#define bpMN 10
#define bpMX 10
#define bpASPD 10
#define bpDEF 10
#define bpPR 10
#define bpHR 10
#define bpFLEE 10
#define bpMSPD 10


namespace GUI {
	// 
	class CStateMgr : public CUIInterface {
	public:
		void RefreshStateFrm();

	protected:
		virtual bool Init();
		virtual void End();
		virtual void FrameMove(DWORD dwTime);

	private:
		static void _evtMainShow(CGuiData* pSender);
		static void MainMouseDown(CGuiData* pSender, int x, int y, DWORD key);

	private:
		//frmState
		CForm* frmState;
		CLabelEx* labName; //
		CLabelEx* labGuildName; //
		CLabelEx* labStateLevel; //
		CLabelEx* labStatePoint; // 
		CLabelEx* labSkillPoint; //
		CLabelEx* labJobShow; //
		CLabelEx* labFameShow; //

		//6
		CLabelEx* labStrshow;
		CLabelEx* labDexshow;
		CLabelEx* labAgishow;
		CLabelEx* labConshow;
		CLabelEx* labStashow;
		CLabelEx* labLukshow;
		CLabelEx* labSailLevel;
		CLabelEx* labSailEXP;

		//8
		CLabelEx* labMinAtackShow;
		CLabelEx* labMaxAtackShow;
		CLabelEx* labFleeShow;
		CLabelEx* labAspeedShow;
		CLabelEx* labMspeedShow;
		CLabelEx* labHitShow;
		CLabelEx* labDefenceShow;
		//CLabelEx*		labCriticalShow;
		//CLabelEx*		labMfShow;
		CLabelEx* labPhysDefineShow;

		//6 
		CTextButton* btnStr; //
		CTextButton* btnAgi; //
		CTextButton* btnCon; //
		CTextButton* btnSta; //
		//CTextButton*   btnLuk;						//
		CTextButton* btnDex; //     

		CLabelEx* labStateEXP;
		CLabelEx* labStateHP;
		CLabelEx* labStateSP;

		CLabelEx* labFameSho; //
		CLabelEx* labStateName;
		CLabelEx* labStateJob;
	};
}
