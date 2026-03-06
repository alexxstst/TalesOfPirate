
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
			LG("gui", "main.clu   frmQuestion not found.\n");
			return false;
		}
		frmQuestion->evtEntrustMouseEvent = _evtQuestionFormEvent;

		edtQuestionTitle = dynamic_cast<CEdit*>(frmQuestion->Find("edtQuestionTitle"));
		if(! edtQuestionTitle)
		{
			LG("gui", "main.clu   frmQuestion:edtQuestionTitle not found.\n");
			return false;
		}
		edtQuestionTitle->SetIsWrap(true);

		memCentent = dynamic_cast<CMemo*>(frmQuestion->Find("memCentent"));
		if(! memCentent)
		{
			LG("gui", "main.clu   frmQuestion:memCentent not found.\n");
			return false;
		}


		frmAnswer = CFormMgr::s_Mgr.Find("frmAnswer");
		if(! frmAnswer)
		{
			LG("gui", "main.clu   frmAnswer not found.\n");
			return false;
		}

		memMiss = dynamic_cast<CMemo*>(frmAnswer->Find("memMiss"));
		if(! memMiss)
		{
			LG("gui", "main.clu   frmAnswer:memMiss not found.\n");
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
			g_pGameApp->MsgBox(g_oLangRec.GetString(914));	// 816
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


