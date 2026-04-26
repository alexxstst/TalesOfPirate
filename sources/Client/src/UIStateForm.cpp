#include "StdAfx.h"
#include "uistateform.h"
#include "uiformmgr.h"
#include "uilabel.h"
#include "uitextbutton.h"
#include "gameapp.h"
#include "character.h"
#include "uiprogressbar.h"
#include "commfunc.h"
#include "ChaAttr.h"
#include "procirculate.h"
#include "packetcmd.h"
#include "tools.h"
#include "GuildData.h"
#include "uiboatform.h"
using namespace std;
using namespace GUI;

//  :   UI-   false   return
inline bool Error(const char* strInfo, const char* strFormName, const char* strCompentName) {
	char _buf[512];
	snprintf(_buf, sizeof(_buf), strInfo, strFormName, strCompentName);
	g_logManager.InternalLog(LogLevel::Error, "errors", _buf);
	return false;
}

//---------------------------------------------------------------------------
// class CStateMgr
//---------------------------------------------------------------------------
bool CStateMgr::Init() {
	CFormMgr& mgr = CFormMgr::s_Mgr;

	frmState = _FindForm("frmState"); // 
	if (!frmState) return false;
	frmState->evtShow = _evtMainShow;

	//frmState 
	labStateName = dynamic_cast<CLabelEx*>(frmState->Find("labStateName"));
	if (!labStateName) return Error(GetLanguageString(45).c_str(), frmState->GetName(), "labStateName");
	labStateName->SetIsCenter(true);

	FORM_CONTROL_LOADING_CHECK(labGuildName, frmState, CLabelEx, "preperty.clu", "labStateGuid");

	labStateJob = dynamic_cast<CLabelEx*>(frmState->Find("labStateJob"));
	if (!labStateJob) return Error(GetLanguageString(45).c_str(), frmState->GetName(), "labStateJob");

	labStateLevel = dynamic_cast<CLabelEx*>(frmState->Find("labStateLevel"));
	if (!labStateLevel) return Error(GetLanguageString(45).c_str(), frmState->GetName(), "labStateLevel");

	labStatePoint = dynamic_cast<CLabelEx*>(frmState->Find("labStatePoint"));
	if (!labStatePoint) return Error(GetLanguageString(45).c_str(), frmState->GetName(), "labStatePoint");

	labSkillPoint = dynamic_cast<CLabelEx*>(frmState->Find("labSkillPoint"));
	if (!labSkillPoint) return Error(GetLanguageString(45).c_str(), frmState->GetName(), "labSkillPoint");

	labFameShow = dynamic_cast<CLabelEx*>(frmState->Find("labFameShow"));
	if (!labFameShow) return Error(GetLanguageString(45).c_str(), frmState->GetName(), "labFameShow");

	//6
	btnStr = dynamic_cast<CTextButton*>(frmState->Find("btnStr"));
	if (!btnStr) return Error(GetLanguageString(45).c_str(), frmState->GetName(), "btnStr");
	btnStr->evtMouseClick = MainMouseDown;

	btnAgi = dynamic_cast<CTextButton*>(frmState->Find("btnAgi"));
	if (!btnAgi) return Error(GetLanguageString(45).c_str(), frmState->GetName(), "btnAgi");
	btnAgi->evtMouseClick = MainMouseDown;

	btnCon = dynamic_cast<CTextButton*>(frmState->Find("btnCon"));
	if (!btnCon) return Error(GetLanguageString(45).c_str(), frmState->GetName(), "btnCon");
	btnCon->evtMouseClick = MainMouseDown;

	btnSta = dynamic_cast<CTextButton*>(frmState->Find("btnSta"));
	if (!btnSta) return Error(GetLanguageString(45).c_str(), frmState->GetName(), "btnSta");
	btnSta->evtMouseClick = MainMouseDown;

	//btnLuk  = dynamic_cast<CTextButton *>(frmState->Find("btnLuk"));    
	//if( !btnLuk )			return Error( "msgui.clu<%s><%s>", frmState->GetName(), "btnLuk" );
	//btnLuk->evtMouseClick = MainMouseDown;

	btnDex = dynamic_cast<CTextButton*>(frmState->Find("btnDex"));
	if (!btnDex) return Error(GetLanguageString(45).c_str(), frmState->GetName(), "btnDex");
	btnDex->evtMouseClick = MainMouseDown;

	//frmState
	labStateEXP = dynamic_cast<CLabelEx*>(frmState->Find("labStateEXP"));
	if (!labStateEXP)
		return Error(GetLanguageString(45).c_str(), frmState->GetName(), "labStateEXP");
	labStateEXP->SetIsCenter(true);

	labStateHP = dynamic_cast<CLabelEx*>(frmState->Find("labStateHP"));
	if (!labStateHP)
		return Error(GetLanguageString(45).c_str(), frmState->GetName(), "labStateHP");
	labStateHP->SetIsCenter(true);

	labStateSP = dynamic_cast<CLabelEx*>(frmState->Find("labStateSP"));
	if (!labStateSP)
		return Error(GetLanguageString(45).c_str(), frmState->GetName(), "labStateSP");
	labStateSP->SetIsCenter(true);


	//6
	labStrshow = (CLabelEx*)frmState->Find("labStrshow");
	labDexshow = (CLabelEx*)frmState->Find("labDexshow");
	labAgishow = (CLabelEx*)frmState->Find("labAgishow");
	labConshow = (CLabelEx*)frmState->Find("labConshow");
	labStashow = (CLabelEx*)frmState->Find("labStashow");
	//labLukshow  = 	( CLabelEx *)frmState->Find( "labLukshow" );
	labSailLevel = (CLabelEx*)frmState->Find("labSailLevel");
	labSailEXP = (CLabelEx*)frmState->Find("labSailEXP");

	//8
	labMinAtackShow = (CLabelEx*)frmState->Find("labMinAtackShow");
	labMaxAtackShow = (CLabelEx*)frmState->Find("labMaxAtackShow");
	labFleeShow = (CLabelEx*)frmState->Find("labFleeShow");
	labAspeedShow = (CLabelEx*)frmState->Find("labAspeedShow");
	labMspeedShow = (CLabelEx*)frmState->Find("labMspeedShow");
	labHitShow = (CLabelEx*)frmState->Find("labHitShow");
	labDefenceShow = (CLabelEx*)frmState->Find("labDefenceShow");
	//labCriticalShow  = 	( CLabelEx *)frmState->Find( "labCriticalShow" );
	//labMfShow        = 	( CLabelEx *)frmState->Find( "labMfShow" );
	labPhysDefineShow = (CLabelEx*)frmState->Find("labPhysDefineShow");
	return true;
}

void CStateMgr::End() {
}

void CStateMgr::FrameMove(DWORD dwTime) {
	if (frmState->GetIsShow()) {
		static CTimeWork time(100);
		if (time.IsTimeOut(dwTime))
			RefreshStateFrm();
	}
}

void CStateMgr::_evtMainShow(CGuiData* pSender) {
	g_stUIState.RefreshStateFrm();
}

void CStateMgr::RefreshStateFrm() {
	CForm* f = g_stUIState.frmState;
	if (!f->GetIsShow()) return;

	CCharacter* pCha = g_stUIBoat.GetHuman();
	if (!pCha) return;

	SGameAttr* pCChaAttr = pCha->getGameAttr();
	if (!pCChaAttr) return;

	if (labStateHP) {
		labStateHP->SetCaption(std::format("{}/{}", pCChaAttr->get(ATTR_HP), pCChaAttr->get(ATTR_MXHP)).c_str());
	}

	int num = pCChaAttr->get(ATTR_CEXP);
	int curlev = pCChaAttr->get(ATTR_CLEXP);
	int nextlev = pCChaAttr->get(ATTR_NLEXP);

	int max = nextlev - curlev;
	num = num - curlev;
	if (num < 0) num = 0;


	if (labStateEXP) {
		if (max != 0)
			labStateEXP->SetCaption(std::format("{:.2f}%", num * 100.0f / max).c_str());
		else
			labStateEXP->SetCaption("0.00%");
	}

	num = pCChaAttr->get(ATTR_SP);
	max = pCChaAttr->get(ATTR_MXSP);
	if (labStateSP) {
		labStateSP->SetCaption(std::format("{}/{}", num, max).c_str());
	}

	if (labStateName) {
		labStateName->SetCaption(pCha->getName().c_str());
	}

	if (labGuildName) {
		if (CGuildData::GetGuildID()) {
			labGuildName->SetCaption(CGuildData::GetGuildName().c_str());
		}
		else {
			labGuildName->SetCaption("");
		}
	}

	if (labStateJob) {
		labStateJob->SetCaption(g_GetJobName((short)pCChaAttr->get(ATTR_JOB)));
	}

	if (labStateLevel) {
		labStateLevel->SetCaption(std::format("{}", pCChaAttr->get(ATTR_LV)).c_str());
	}

	if (labStatePoint) {
		labStatePoint->SetCaption(std::format("{}", pCChaAttr->get(ATTR_AP)).c_str());
	}

	if (labSkillPoint) {
		labSkillPoint->SetCaption(std::format("{}", pCChaAttr->get(ATTR_TP)).c_str());
	}

	//06
	if (pCChaAttr->get(ATTR_AP) > 0) {
		btnStr->SetIsShow(true);
		btnAgi->SetIsShow(true);
		btnCon->SetIsShow(true);
		btnSta->SetIsShow(true);
		//btnLuk->SetIsShow(true); 
		btnDex->SetIsShow(true);
	}
	else {
		btnStr->SetIsShow(false);
		btnAgi->SetIsShow(false);
		btnCon->SetIsShow(false);
		btnSta->SetIsShow(false);
		//btnLuk->SetIsShow(false); 
		btnDex->SetIsShow(false);
	}
	//6
	if (labStrshow) {
		labStrshow->SetCaption(std::format("{}", pCChaAttr->get(ATTR_STR)).c_str());
	}

	if (labDexshow) {
		labDexshow->SetCaption(std::format("{}", pCChaAttr->get(ATTR_DEX)).c_str());
	}

	if (labAgishow) {
		labAgishow->SetCaption(std::format("{}", pCChaAttr->get(ATTR_AGI)).c_str());
	}

	if (labConshow) {
		labConshow->SetCaption(std::format("{}", pCChaAttr->get(ATTR_CON)).c_str());
	}

	if (labStashow) {
		labStashow->SetCaption(std::format("{}", pCChaAttr->get(ATTR_STA)).c_str());
	}

	if (labSailLevel) {
		labSailLevel->SetCaption(std::format("{}", pCChaAttr->get(ATTR_SAILLV)).c_str());
	}

	if (labSailEXP) {
		labSailEXP->SetCaption(std::format("{}", pCChaAttr->get(ATTR_CSAILEXP)).c_str());
	}

	if (labMinAtackShow) {
		labMinAtackShow->SetCaption(std::format("{}", pCChaAttr->get(ATTR_MNATK)).c_str());
	}

	if (labMaxAtackShow) {
		labMaxAtackShow->SetCaption(std::format("{}", pCChaAttr->get(ATTR_MXATK)).c_str());
	}

	if (labFleeShow) {
		labFleeShow->SetCaption(std::format("{}", pCChaAttr->get(ATTR_FLEE)).c_str());
	}

	if (labAspeedShow) {
		int v = pCChaAttr->get(ATTR_ASPD);
		if (v == 0)
			labAspeedShow->SetCaption("-1");
		else
			labAspeedShow->SetCaption(std::format("{}", 100000 / v).c_str());
	}

	if (labMspeedShow) {
		labMspeedShow->SetCaption(std::format("{}", pCChaAttr->get(ATTR_MSPD)).c_str());
	}

	if (labHitShow) {
		labHitShow->SetCaption(std::format("{}", pCChaAttr->get(ATTR_HIT)).c_str());
	}

	if (labDefenceShow) {
		labDefenceShow->SetCaption(std::format("{}", pCChaAttr->get(ATTR_DEF)).c_str());
	}

	if (labPhysDefineShow) {
		labPhysDefineShow->SetCaption(std::format("{}", pCChaAttr->get(ATTR_PDEF)).c_str());
	}

	if (labFameShow) {
		labFameShow->SetCaption(std::format("{}", pCChaAttr->get(ATTR_FAME)).c_str());
	}
}

void CStateMgr::MainMouseDown(CGuiData* pSender, int x, int y, DWORD key) {
	CCharacter* pCha = g_stUIBoat.GetHuman();
	if (!pCha) return;

	if (pCha->getGameAttr()->get(ATTR_AP) <= 0) return;

	CChaAttr attr;
	attr.ResetChangeFlag();

	std::string name = pSender->GetName();
	if (name == "btnStr") {
		attr.DirectSetAttr(ATTR_STR, 1);
		attr.SetChangeBitFlag(ATTR_STR);
		CProCirculateCS* proCir = (CProCirculateCS*)g_NetIF->GetProCir();
		proCir->SynBaseAttribute(&attr);
	}
	else if (name == "btnAgi") {
		attr.DirectSetAttr(ATTR_AGI, 1);
		attr.SetChangeBitFlag(ATTR_AGI);
		CProCirculateCS* proCir = (CProCirculateCS*)g_NetIF->GetProCir();
		proCir->SynBaseAttribute(&attr);
	}
	else if (name == "btnCon") {
		attr.DirectSetAttr(ATTR_CON, 1);
		attr.SetChangeBitFlag(ATTR_CON);
		CProCirculateCS* proCir = (CProCirculateCS*)g_NetIF->GetProCir();
		proCir->SynBaseAttribute(&attr);
	}
	else if (name == "btnSta") {
		attr.DirectSetAttr(ATTR_STA, 1);
		attr.SetChangeBitFlag(ATTR_STA);
		CProCirculateCS* proCir = (CProCirculateCS*)g_NetIF->GetProCir();
		proCir->SynBaseAttribute(&attr);
	}
	//else if (name =="btnLuk")
	//{
	//	attr.DirectSetAttr( ATTR_LUK, 1);
	//	attr.SetChangeBitFlag( ATTR_LUK );		
	//	CProCirculateCS* proCir = (CProCirculateCS *)g_NetIF->GetProCir();
	//	proCir->SynBaseAttribute( &attr );

	//}
	else if (name == "btnDex") {
		attr.DirectSetAttr(ATTR_DEX, 1);
		attr.SetChangeBitFlag(ATTR_DEX);
		CProCirculateCS* proCir = (CProCirculateCS*)g_NetIF->GetProCir();
		proCir->SynBaseAttribute(&attr);
	}
}
