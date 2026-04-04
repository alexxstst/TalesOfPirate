
#include "stdafx.h"
#include "gameapp.h"
#include "UIMailForm.h"
#include "UIFormMgr.h"
#include "UINpcTalkForm.h"
#include "PacketCmd.h"

using namespace std;

namespace GUI
{
	CMailMgr::CMailMgr(void)
	{
	}

	CMailMgr::~CMailMgr(void)
	{
	}


	bool CMailMgr::Init()
	{
		frmQuestion = CFormMgr::s_Mgr.Find("frmQuestion");
		if(! frmQuestion)
		{
			ToLogService("common", "main.clu   frmQuestion not found.");
			return false;
		}
		frmQuestion->evtEntrustMouseEvent = _evtQuestionFormEvent;

		edtQuestionTitle = dynamic_cast<CEdit*>(frmQuestion->Find("edtQuestionTitle"));
		if(! edtQuestionTitle)
		{
			ToLogService("common", "main.clu   frmQuestion:edtQuestionTitle not found.");
			return false;
		}
		edtQuestionTitle->SetIsWrap(true);

		memCentent = dynamic_cast<CMemo*>(frmQuestion->Find("memCentent"));
		if(! memCentent)
		{
			ToLogService("common", "main.clu   frmQuestion:memCentent not found.");
			return false;
		}


		frmAnswer = CFormMgr::s_Mgr.Find("frmAnswer");
		if(! frmAnswer)
		{
			ToLogService("common", "main.clu   frmAnswer not found.");
			return false;
		}

		memMiss = dynamic_cast<CMemo*>(frmAnswer->Find("memMiss"));
		if(! memMiss)
		{
			ToLogService("common", "main.clu   frmAnswer:memMiss not found.");
			return false;
		}

		return true;
	}


	void CMailMgr::CloseForm()
	{
	}


	void CMailMgr::FrameMove(DWORD dwTime)
	{
		if(frmQuestion && frmQuestion->GetIsShow())
		{
			memCentent->SetCaption(edtQuestionTitle->GetCaption());
			memCentent->ProcessCaption();
			memCentent->Refresh();
		}
	}


	void CMailMgr::ShowQuestionForm()
	{
		memCentent->SetCaption("");
		memCentent->ProcessCaption();
		memCentent->Refresh();

		frmQuestion->SetIsShow(true);
	}


	void CMailMgr::ShowAnswerForm(const char* szTitle, const char* szContent)
	{
		string strAnswer("");
		if(strlen(szTitle) > 0)
		{
			strAnswer += szTitle;
			strAnswer += "\n";
		}
		strAnswer += szContent;

		memMiss->SetCaption(strAnswer.c_str());
		memMiss->ProcessCaption();
		memMiss->Refresh();

		frmAnswer->SetIsShow(true);
	}


	void CMailMgr::SubmitQuestion()
	{
		if(strlen(edtQuestionTitle->GetCaption()) < 16)
		{
			g_pGameApp->MsgBox("%s", GetLanguageString(914).c_str());	// 816
			return;
		}

		CS_GMSend(g_stUINpcTalk.GetNpcId(), "", edtQuestionTitle->GetCaption());

		edtQuestionTitle->SetCaption("");
		memCentent->SetCaption("");
		frmQuestion->SetIsShow(false);
	}


	void CMailMgr::_evtQuestionFormEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
	{
		string strName = pSender->GetName();
		if(strName == "btnSubmit")	// 
		{
			g_stUIMail.SubmitQuestion();
		}
	}

}


