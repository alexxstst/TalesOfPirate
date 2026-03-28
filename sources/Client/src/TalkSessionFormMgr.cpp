//------------------------------------------------------------------------
//	2005.3.29	Arcol	create this file
//	2005.4.5	Arcol	add session node and handle the flash event in the QQ form
//------------------------------------------------------------------------

#include "StdAfx.h"
#include "UIGlobalVar.h"
#include "uiformmgr.h"
#include "uilabel.h"
#include "uicloneform.h"
#include "UITeam.h"
#include "UIForm.h"
#include "uitextbutton.h"
#include "PacketCmd.h"
#include "NetChat.h"
#include "UIChat.h"
#include "UIEdit.h"
#include "UIMemo.h"
#include "UIList.h"
#include "uilabel.h"
#include "GameApp.h"
#include "StringLib.h"
#include "Character.h"
#include "UIGraph.h"
#include "UIGrid.h"
#include "uitreeview.h"
#include "ChatIconSet.h"
#include "UITextParse.h"
#include "talksessionformmgr.h"
#include "UIMenu.h"
#include "atltypes.h"
#include "UICozeForm.h"
#include "ChatRecord.h"

using namespace std;

#define MINIMIZE_FORM_WIDTH 185
#define MINIMIZE_FORM_HEIGHT 20
#define INFORM_FORM_SHOW_SX 100
#define NORMAL_FORM_OFFSET_WIDTH 33
#define NORMAL_FORM_OFFSET_HEIGHT 20
#define NORMAL_FORM_SHOW_SX 105
#define NORMAL_FORM_SHOW_SY 90

static CCloneForm dupe1;
static CCloneForm dupe2;
static CCloneForm dupe3;
static CCloneForm dupe4;


bool _Error(const char* strInfo, const char* strFormName, const char* strCompentName) {
	{ char _buf[512]; snprintf(_buf, sizeof(_buf), strInfo, strFormName, strCompentName); g_logManager.InternalLog(LogLevel::Debug, "common", _buf); }
	return false;
}

//------------------------------------------------------------------------
//	CSessionMember��ʵ��
//------------------------------------------------------------------------

CSessionMember::CSessionMember(stNetSessCreate *pMember)
{
	_strName=pMember->szChaName;
	_strMotto=pMember->szMotto;
	_nID=pMember->lChaID;
	_nIcon_id=pMember->sIconID;
}

CSessionMember::~CSessionMember()
{
}



CMenu* CTalkSessionForm::m_playerMouseRight=NULL;

//------------------------------------------------------------------------
//	CTalkSessionForm��ʵ��
//------------------------------------------------------------------------

CTalkSessionForm::CTalkSessionForm(DWORD sessionID,eTalkSessionStyle style) : 
m_dwSessioinID(sessionID), m_strWaitForSend(""), m_bActive(true), m_pForm(NULL), m_pEdit(NULL),
m_strTalkMsg(""), /*m_pList(NULL),*/ m_pInformTextBtn(NULL), m_pInformTitle(NULL), m_nShowChannel(NULL),
m_nShowSx(NULL), m_nShowSy(NULL), m_strWaitForAdded(""), m_bFlash(NULL), m_nFlashTimes(0), m_pTimer1(NULL), m_pTimer2(NULL)
{
	//����Form
	m_bActive=(m_dwSessioinID!=0)?true:false;
	CTextButton *btn;
	static CForm *frmRemind=CFormMgr::s_Mgr.Find("frmRemind");
	if (!frmRemind)
	{
		// Форма напоминания не найдена
		if( !frmRemind ) g_logManager.InternalLog(LogLevel::Error, "common", g_oLangRec.GetString(410));
		return;
	}
	dupe1.SetSample(frmRemind);
	m_pInformForm=dupe1.Clone();	
	m_pInformTextBtn = dynamic_cast<CTextButton*>(m_pInformForm->Find("btnCheck"));
	if( !m_pInformTextBtn )
	{
		_Error(g_oLangRec.GetString(411), m_pInformForm->GetName(), "btnCheck" );
		return;
	}
	m_pInformTextBtn->evtMouseClick=_OnMouseInformButton;
	m_pInformTextBtn->SetPointer(this);
	m_pInformTitle = dynamic_cast<CLabelEx*>(m_pInformForm->Find("labTitle"));
	if( !m_pInformTitle )
	{
		_Error(g_oLangRec.GetString(411), m_pInformForm->GetName(), "labTitle" );
		return;
	}
	btn = dynamic_cast<CTextButton*>(m_pInformForm->Find("btnYes"));
	if( !btn )
	{
		_Error(g_oLangRec.GetString(411), m_pInformForm->GetName(), "btnYes" );
		return;
	}
	btn->evtMouseClick=_OnMouseInformButton;
	btn->SetPointer(this);
	btn = dynamic_cast<CTextButton*>(m_pInformForm->Find("btnNo"));
	if( !btn )
	{
		_Error(g_oLangRec.GetString(411), m_pInformForm->GetName(), "btnNo" );
		return;
	}
	btn->evtMouseClick=_OnMouseQuitButton;
	btn->SetPointer(this);


	static CForm *frmChat=CFormMgr::s_Mgr.Find("frmChat");
	if (!frmChat)
	{
		// Форма чата не найдена
		if( !frmChat ) g_logManager.InternalLog(LogLevel::Error, "common", g_oLangRec.GetString(412));
		 return;
	}
	dupe2.SetSample(frmChat);
	m_pNormalForm=dupe2.Clone();
	m_pNormalForm->SetPointer(this);
	((CGuiData*)m_pNormalForm)->SetPointer(this);
	POINT pt=CTalkSessionFormMgr::GetNormalFormPosition(sessionID);
	m_pNormalForm->SetPos(pt.x,pt.y);
	m_pNormalForm->SetHotKeyHandler(_OnHotKey);
	m_bQuitFlag=false;
	m_pNormalForm->evtEscClose=GuiFormEscCloseEvent;
	btn = dynamic_cast<CTextButton*>(m_pNormalForm->Find("btnMin"));
	if( !btn )
	{
		_Error(g_oLangRec.GetString(411), m_pNormalForm->GetName(), "btnMin" );
		return;
	}
	btn->evtMouseClick=_OnMouseMinimizeButton;
	btn->SetPointer(this);
	btn = dynamic_cast<CTextButton*>(m_pNormalForm->Find("btnClose"));
	if( !btn )
	{
		_Error(g_oLangRec.GetString(411), m_pNormalForm->GetName(), "btnClose" );
		return;
	}
	btn->evtMouseClick=_OnMouseCloseButton;
	btn->SetPointer(this);
	//btn = dynamic_cast<CTextButton*>(m_pNormalForm->Find("btnCloseD"));
	//if( !btn )
	//{
	//	_Error("chat.clu����<%s>���Ҳ����ؼ�<%s>", m_pNormalForm->GetName(), "btnCloseD" );
	//	return;
	//}
	//btn->evtMouseClick=_OnMouseQuitButton;
	//btn->pTag=this;
	btn = dynamic_cast<CTextButton*>(m_pNormalForm->Find("btnChat"));
	if( !btn )
	{
		_Error(g_oLangRec.GetString(411), m_pNormalForm->GetName(), "btnChat" );
		return;
	}
	btn->evtMouseClick=_OnMouseSaveButton;
	btn->SetPointer(this);
	btn = dynamic_cast<CTextButton*>(m_pNormalForm->Find("btnSend"));
	if( !btn )
	{
		_Error(g_oLangRec.GetString(411), m_pNormalForm->GetName(), "btnSend" );
		return;
	}
	btn->evtMouseClick=_OnMouseSendButton;
	btn->SetPointer(this);
	m_pNormalEdit = dynamic_cast<CEdit*>(m_pNormalForm->Find("edtChat"));
	if( !m_pNormalEdit )
	{
		_Error(g_oLangRec.GetString(411), m_pNormalForm->GetName(), "edtChat" );
		return;
	}
	m_pNormalEdit->SetEnterButton(btn);
	m_pNormalEdit->SetIsParseText(true);
	m_pNormalEdit->SetIsWrap(true);
	m_pNormalEdit->Init();
	//m_pNormalList = dynamic_cast<CList*>(m_pNormalForm->Find("memCtrl"));
	//if( !m_pNormalList )
	//{
	//	_Error("chat.clu����<%s>���Ҳ����ؼ�<%s>", m_pNormalForm->GetName(), "memCtrl" );
	//	return;
	//}
	//m_pNormalList->SetRowHeight(20);

	m_pNormalMemo = dynamic_cast<CRichMemo*>(m_pNormalForm->Find("memChat"));
	if( !m_pNormalMemo )
	{
		_Error(g_oLangRec.GetString(411), m_pNormalForm->GetName(), "memChat" );
		return;
	}
	m_pNormalMemo->SetAutoScroll(true);
	m_pNormalMemo->Init();
	RECT rc;
	rc.left = 1;
	rc.top = 1;
	rc.right = 15;
	rc.bottom = 10;
	m_pNormalMemo->SetClipRect(rc);

	btn = dynamic_cast<CTextButton*>(m_pNormalForm->Find("btnBrow"));
	if( !btn )
	{
		_Error( g_oLangRec.GetString(411), m_pNormalForm->GetName(), "btnBrow" );
		return;
	}
	btn->evtMouseClick=_OnMouseFaceListButton;
	btn->SetPointer(this);
	btn = dynamic_cast<CTextButton*>(m_pNormalForm->Find("btnClean"));
	if( !btn )
	{
		_Error( g_oLangRec.GetString(411), m_pNormalForm->GetName(), "btnClean" );
		return;
	}
	btn->evtMouseClick=_OnMouseCleanText;
	btn->SetPointer(this);
	m_pFaceGrid = dynamic_cast<CGrid*>(m_pNormalForm->Find("grdFace"));
	if( !m_pFaceGrid )
	{
		// Компонент сетки эмодзи не найден
		{ char _buf[512]; snprintf(_buf, sizeof(_buf), g_oLangRec.GetString(411), m_pNormalForm->GetName(), "grdFace"); g_logManager.InternalLog(LogLevel::Error, "common", _buf); }
		return;
	}
	m_pFaceGrid->evtSelectChange =_OnMouseSelectFace;
	m_pFaceGrid->evtLost=_OnFaceLostFocus;
	m_pFaceGrid->SetPointer(this);
	m_pNormalNameList = dynamic_cast<CList*>(m_pNormalForm->Find("lstChat"));
	if( !m_pNormalNameList )
	{
		_Error( g_oLangRec.GetString(411), m_pNormalForm->GetName(), "lstChat" );
		return;
	}
	m_pNormalNameList->SetRowHeight(16);
	m_pNormalNameList->evtListMouseDB=_OnMouseRightPlayerMenu;
	m_playerMouseRight=CMenu::FindMenu("ListMouseRight");
	if (!m_playerMouseRight)
	{
		_Error(g_oLangRec.GetString(411), m_pNormalForm->GetName(), "ListMouseRight" );
		return;
	}
	m_playerMouseRight->evtListMouseDown=_OnPlayerSelect;
	btn = dynamic_cast<CTextButton*>(m_pNormalForm->Find("btnQuit"));
	if( !btn )
	{
		_Error(g_oLangRec.GetString(411), m_pNormalForm->GetName(), "btnQuit" );
		return;
	}
	btn->evtMouseClick=_OnMouseQuitButton;
	btn->SetPointer(this);

	static CForm *frmChatMd=CFormMgr::s_Mgr.Find("frmChatMd");
	if (!frmChatMd)
	{
		// Форма малого чата не найдена
		if( !frmChatMd ) g_logManager.InternalLog(LogLevel::Error, "common", g_oLangRec.GetString(413));
		return;
	}
	dupe3.SetSample(frmChatMd);
	m_pSmallForm=dupe3.Clone();
	btn = dynamic_cast<CTextButton*>(m_pSmallForm->Find("btnMin"));
	if( !btn )
	{
		_Error(g_oLangRec.GetString(411), m_pSmallForm->GetName(), "btnMin" );
		return;
	}
	btn->evtMouseClick=_OnMouseMinimizeButton;
	btn->SetPointer(this);
	btn = dynamic_cast<CTextButton*>(m_pSmallForm->Find("btnReturn"));
	if( !btn )
	{
		_Error(g_oLangRec.GetString(411), m_pSmallForm->GetName(), "btnReturn" );
		return;
	}
	btn->evtMouseClick=_OnMouseNormalButton;
	btn->SetPointer(this);
	btn = dynamic_cast<CTextButton*>(m_pSmallForm->Find("btnClose"));
	if( !btn )
	{
		_Error(g_oLangRec.GetString(411), m_pSmallForm->GetName(), "btnClose" );
		return;
	}
	btn->evtMouseClick=_OnMouseQuitButton;
	btn->SetPointer(this);
	btn = dynamic_cast<CTextButton*>(m_pSmallForm->Find("btnSend"));
	if( !btn )
	{
		_Error(g_oLangRec.GetString(411), m_pSmallForm->GetName(), "btnSend" );
		return;
	}
	btn->evtMouseClick=_OnMouseSendButton;
	btn->SetPointer(this);
	m_pSmallEdit = dynamic_cast<CEdit*>(m_pSmallForm->Find("edtTradeGold"));
	if( !m_pSmallEdit )
	{
		_Error(g_oLangRec.GetString(411), m_pNormalForm->GetName(), "edtTradeGold" );
		return;
	}
	m_pSmallEdit->SetEnterButton(btn);
	//m_pSmallList = dynamic_cast<CList*>(m_pSmallForm->Find("memCtrl"));
	//if( !m_pSmallList )
	//{
	//	_Error("chat.clu����<%s>���Ҳ����ؼ�<%s>", m_pSmallForm->GetName(), "memCtrl" );
	//	return;
	//}




	static CForm *frmChatMin=CFormMgr::s_Mgr.Find("frmChatMin");
	if (!frmChatMin)
	{
		// Форма минимизированного чата не найдена
		if( !frmChatMin ) g_logManager.InternalLog(LogLevel::Error, "common", g_oLangRec.GetString(414));
		return;
	}
	dupe4.SetSample(frmChatMin);
	m_pMinimizeForm=dupe4.Clone();
	m_pMinimizeForm->SetPos(0,0);
	btn = dynamic_cast<CTextButton*>(m_pMinimizeForm->Find("btnMax"));
	if( !btn )
	{
		_Error(g_oLangRec.GetString(411), m_pMinimizeForm->GetName(), "btnMax" );
		return;
	}
	btn->evtMouseClick=_OnMouseNormalButton;
	btn->SetPointer(this);
	btn = dynamic_cast<CTextButton*>(m_pMinimizeForm->Find("btnClose"));
	if( !btn )
	{
		_Error(g_oLangRec.GetString(411), m_pMinimizeForm->GetName(), "btnClose" );
		return;
	}
	btn->evtMouseClick=_OnMouseCloseButton;
	btn->SetPointer(this);


	ChangeStyle(style);
	//if (m_pList) m_pList->GetItems()->Clear();
	if (m_pMemo) m_pMemo->Clear();
	if (m_pNameList) m_pNameList->GetItems()->Clear();
	if (m_pEdit) m_pEdit->SetCaption("");
}

CTalkSessionForm::~CTalkSessionForm(void)
{
	if (m_pTimer1)
	{
		m_pTimer1->Release();
		m_pTimer1=NULL;
	}
	if (m_pTimer2)
	{
		m_pTimer2->Release();
		m_pTimer2=NULL;
	}
	//m_pInformForm->KillTimer(1);
	//m_pInformForm->KillTimer(2);
	m_pNormalMemo->Clear();
	//m_pNormalList->GetItems()->Clear();
	//m_pSmallList->GetItems()->Clear();
	//m_pSmallMemo->Clear();
	g_stUIChat.GetSessionNode()->DelItem((CItemObj*)m_pData);
	if (m_pForm) m_pForm->Close();
	dupe1.Release(m_pInformForm);
	dupe2.Release(m_pNormalForm);
	dupe3.Release(m_pSmallForm);
	dupe4.Release(m_pMinimizeForm);
	CSessionMember* pNode;
	vector <CSessionMember*>::iterator Iter;
	while (m_Members.size()>0)
	{
		Iter=m_Members.begin();
		pNode=*Iter;
		//delete pNode;
		SAFE_DELETE(pNode); // UI��������
		m_Members.erase(Iter);
	}
	ClearMsg();
	ReleaseInformShowChannel();
}

int CTalkSessionForm::GetTotalMembers()
{
	return (int)m_Members.size();
}

CForm* CTalkSessionForm::GetForm()
{
	return m_pForm;
}

bool CTalkSessionForm::hasMember(DWORD chaID)
{
	vector <CSessionMember*>::iterator Iter;
	for (Iter=m_Members.begin();Iter!=m_Members.end();Iter++)
	{
		if ((*Iter)->GetID()==chaID)
			return true;
	}
	return false;
}

bool CTalkSessionForm::hasMember(string name)
{
	vector <CSessionMember*>::iterator Iter;
	for (Iter=m_Members.begin();Iter!=m_Members.end();Iter++)
	{
		if ((*Iter)->GetName()==name)
			return true;
	}
	return false;
}

CSessionMember* CTalkSessionForm::GetMemberByIndex(int nIndex)
{
	if (nIndex>=(int)m_Members.size()) return NULL;
	return m_Members[nIndex];
}

CSessionMember* CTalkSessionForm::GetMemberByID(DWORD chrID)
{
	vector <CSessionMember*>::iterator Iter;
	for (Iter=m_Members.begin();Iter!=m_Members.end();Iter++)
	{
		if ((*Iter)->GetID()==chrID)
			return *Iter;
	}
	return NULL;
}

void CTalkSessionForm::AddMembers(stNetSessCreate *pArrMembers[],int memberNum)
{
	CSessionMember *pNewMember=NULL;
	for (int i=0;i<memberNum;i++)
	{
		if (g_stUIChat._dwSelfID==pArrMembers[i]->lChaID) continue;
		pNewMember=new CSessionMember(pArrMembers[i]);
		m_Members.push_back(pNewMember);
	}
	m_playerMouseRight->SetIsShow(false);
	RefreshForm();
	return;
}

bool CTalkSessionForm::DelMember(DWORD chaID)
{
	if (!hasMember(chaID)) return false;
	vector <CSessionMember*>::iterator Iter;
	for (Iter=m_Members.begin();Iter!=m_Members.end();Iter++)
	{
		if (chaID==(*Iter)->GetID())
		{
			//delete (*Iter);
			SAFE_DELETE(*Iter); // UI��������
			m_Members.erase(Iter);
			m_playerMouseRight->SetIsShow(false);
			RefreshForm();
			return true;
		}
	}
	return false;
}


bool CTalkSessionForm::ChangeStyle(eTalkSessionStyle style)
{
	if (m_pForm) m_pForm->Close();
	if (m_pTimer1)
	{
		m_pTimer1->Release();
		m_pTimer1=NULL;
	}
	if (m_pTimer2)
	{
		m_pTimer2->Release();
		m_pTimer2=NULL;
	}
	//m_pInformForm->KillTimer(1);
	//m_pInformForm->KillTimer(2);
	//if (m_nShowChannel)
	//{
	//	CTalkSessionFormMgr::ReleaseShowChannel(m_nShowChannel);
	//	m_nShowChannel=0;
	//}
	ReleaseInformShowChannel();
	switch(style)
	{
	case enumTalkSessionHidden:
		{
			m_pForm=NULL;
			m_pMemo=NULL;
			m_pEdit=NULL;
			m_pNameList=NULL;
			//m_pList=NULL;
		}
		break;
	case enumTalkSessionInform:
		{
			//m_nShowChannel=CTalkSessionFormMgr::GetShowChannel();
			GetInformShowChannel();
			m_pForm=m_pInformForm;
			m_pMemo=NULL;
			m_pEdit=NULL;
			m_pNameList=NULL;
			//m_pList=NULL;
			m_pTimer1=CGuiTime::Create(100,_OnTimerInformShowing);
			m_pTimer1->SetUserID(1);
			m_pTimer1->SetUserPoint(this);
			//m_pForm->SetTimer(1,100,this);
			//m_pForm->evtOnTimer=_OnTimerInformShowing;
			m_nShowSx=INFORM_FORM_SHOW_SX+(m_nShowChannel-1)*m_pInformForm->GetWidth();
			m_nShowSy=-m_pInformForm->GetHeight();
			m_pForm->SetPos(m_nShowSx,m_nShowSy);
			m_Style=style;
			m_pForm->Show();
			return true;
		}
		break;
	case enumTalkSessionNormal:
		{
			m_pForm=m_pNormalForm;
			m_pMemo=m_pNormalMemo;
			if (m_pEdit)
			{
				m_pNormalEdit->SetCaption(m_pEdit->GetCaption());
			}
			m_pEdit=m_pNormalEdit;
			m_pNameList=m_pNormalNameList;
			//m_pList=m_pNormalList;
		}
		break;
	case enumTalkSessionSmall:
		{
			/*
			m_pForm=m_pSmallForm;
			//m_pMemo=m_pSmallMemo;
			if (m_pEdit)
			{
				m_pSmallEdit->SetCaption(m_pEdit->GetCaption());
			}
			m_pEdit=m_pSmallEdit;
			m_pNameList=NULL;
			//m_pList=m_pSmallList;
			*/
		}
		break;
	case enumTalkSessionMinimize:
		{
			if (m_pMinimizeForm->GetTop() == 0 && m_pMinimizeForm->GetLeft() == 0)
			{
				POINT p=CTalkSessionFormMgr::GetMiniFormPosition(GetSessionID());
				m_pMinimizeForm->SetPos(p.x,p.y);
			}
			m_pForm=m_pMinimizeForm;
			m_pMemo=NULL;
			m_pEdit=NULL;
			m_pNameList=NULL;
			//m_pList=NULL;
		}
		break;
	default:
		return false;
	}
	m_Style=style;
	if (m_Style==enumTalkSessionHidden) return true;
	RefreshForm();
	m_pForm->Show();
	return true;
}

int CTalkSessionForm::GetInformShowChannel()
{
	if (!m_nShowChannel)
	{
		m_nShowChannel=CTalkSessionFormMgr::GetShowChannel();
	}
	return m_nShowChannel;
}

void CTalkSessionForm::ReleaseInformShowChannel()
{
	if (m_nShowChannel)
	{
		CTalkSessionFormMgr::ReleaseShowChannel(m_nShowChannel);
		m_nShowChannel=0;
	}
}

void CTalkSessionForm::RefreshForm()
{
	if (m_Style==enumTalkSessionHidden) return;
	int total=GetTotalMembers();
	char buf[100];
	string str;
	if (total==1)
	{
		str=string(g_oLangRec.GetString(415))+GetMemberByIndex(0)->GetName()+g_oLangRec.GetString(416);
	}
	else
	{
		sprintf(buf,g_oLangRec.GetString(417),total);
		str=buf;
	}

	CLabelEx* pText=dynamic_cast<CLabelEx*>(m_pMinimizeForm->Find("labName"));
	if (pText)
	{
		pText->SetCaption(str.c_str());
	}
	else
	{
		_Error(g_oLangRec.GetString(411), m_pMinimizeForm->GetName(), "labName" );
	}
	if (total==1)
	{
		str=string(g_oLangRec.GetString(415))+GetMemberByIndex(0)->GetName();
		if (strlen(GetMemberByIndex(0)->GetMotto())>0)
		{
			str+=string("(")+GetMemberByIndex(0)->GetMotto()+string(")");
		}
		str=StringLimit(str,31);
		str+=g_oLangRec.GetString(416);
	}
	else
	{
		sprintf(buf,g_oLangRec.GetString(418),total);
		str=buf;
		for (int i=0;i<total;i++)
		{
			str+=GetMemberByIndex(i)->GetName();
			str+=" ";
		}
		str=StringLimit(str,36);
	}

	pText=dynamic_cast<CLabelEx*>(m_pNormalForm->Find("labName1"));
	if (pText)
	{
		pText->SetCaption(str.c_str());
	}
	else
	{
		_Error(g_oLangRec.GetString(411), m_pNormalForm->GetName(), "labName1" );
	}
	sprintf(buf,g_oLangRec.GetString(419),total);
	str=buf;
	pText=dynamic_cast<CLabelEx*>(m_pNormalForm->Find("labParty"));
	if (pText)
	{
		pText->SetCaption(str.c_str());
	}
	else
	{
		_Error(g_oLangRec.GetString(411), m_pNormalForm->GetName(), "labParty" );
	}

	m_pNormalNameList->GetItems()->Clear();
	for (int i=0;i<GetTotalMembers();i++)
	{
		CItemRow* pItemRow=m_pNormalNameList->NewItem();
		CTextGraph* pItem = new CTextGraph(1);
		pItem->SetString( StringLimit(GetMemberByIndex(i)->GetName(),11).c_str() );
		pItem->SetHint( GetMemberByIndex(i)->GetName() );
		pItem->SetColor(0xef5c3cd3);
		pItem->SetPointer(GetMemberByIndex(i));
		CChatIconInfo *pIconInfo=GetChatIconInfo(GetMemberByIndex(i)->GetIconID());
		if (pIconInfo)
		{
			CGuiPic* pPic=pItem->GetImage();
			string strPath="texture/ui/HEAD/";	
			pPic->LoadImage((strPath+pIconInfo->szSmall).c_str(),16,16,0,pIconInfo->nSmallX,pIconInfo->nSmallY);
		}
		pItemRow->SetIndex(0,pItem);
		//m_pNormalNameList->Add(GetMemberByIndex(i)->GetName());
	}

	if (m_pMemo)
	{
		m_pMemo->SetAutoScroll( FALSE );
		m_pMemo->Clear();
		for (const auto& iter: m_TalkMsg)
		{
			string strName= iter->name;
			if (iter->motto.length()==0)
			{
				strName+=":";
			}
			else
			{
				strName+=+"("+iter->motto+")"+":";
			}
			m_pMemo->AddText(strName.c_str(),iter->text.c_str(),TEXT_COLOR_BLACK,TEXT_FONT_9);
		}
		m_pMemo->SetAutoScroll( TRUE );
		m_pMemo->AutoScroll();
	}


/*
	if (m_pMemo&&!m_TalkMsg.empty())
	{
		//m_pList->GetItems()->Clear();
		m_pMemo->Clear();
		char *buf=new char[m_strTalkMsg.length()+10];
		strcpy(buf,m_strTalkMsg.c_str());
		char *token=strtok(buf, "\n");
		while( token != NULL )
		{
			string str=token;
			DWORD color=0xFF000000;
			m_pMemo->AddText("abcd",str.c_str(),TEXT_COLOR_BLACK,TEXT_FONT_9);
			//while (!str.empty())
			//{
			//	string text;
			//	text=CutFaceText(str,41);
			//	CItemRow* pItemRow=m_pList->NewItem();
			//	CItemEx* pItem=new CItemEx(text.c_str(),color);
			//	pItem->SetAlignment(eAlignCenter);
			//	pItem->SetHeight(18);
			//	if (text.find ("#") !=-1)	
			//	{
			//		pItem->SetIsParseText(true);
			//	}
			//	pItemRow->SetBegin(pItem);
			//}
			//m_pList->Add(token);
			token = strtok(NULL, "\n");
		}
		//m_pList->GetScroll()->OnKeyDown(VK_END);
		//m_pMemo->GetScroll()->OnKeyDown(VK_END);
		delete buf;
	}
*/
	m_pForm->Refresh();
}

void CTalkSessionForm::StopFlash()
{
	SetIsFlash(false);
	//CMember* pMember=NULL;
	//for (int i=0;i<GetTotalMembers();i++)
	//{
	//	string str=GetMemberByIndex(i)->GetName();
	//	CTeam* pTeam=g_stUIChat.GetTeamMgr()->Find(enumTeamFrnd);
	//	if (pTeam)
	//	{
	//		for (DWORD i=0;i<pTeam->GetCount();i++)
	//		{
	//			pMember=pTeam->GetMember(i);
	//			if (str==pMember->GetName())
	//			{
	//				pMember->SetIsFlash(false);
	//			}
	//		}
	//	}
	//	pTeam=g_stUIChat.GetTeamMgr()->Find(enumTeamGroup);
	//	if (pTeam)
	//	{
	//		for (DWORD i=0;i<pTeam->GetCount();i++)
	//		{
	//			pMember=pTeam->GetMember(i);
	//			if (str==pMember->GetName())
	//			{
	//				pMember->SetIsFlash(false);
	//			}
	//		}
	//	}
	//	pTeam=g_stUIChat.GetTeamMgr()->Find(enumTeamRoad);
	//	if (pTeam)
	//	{
	//		for (DWORD i=0;i<pTeam->GetCount();i++)
	//		{
	//			pMember=pTeam->GetMember(i);
	//			if (str==pMember->GetName())
	//			{
	//				pMember->SetIsFlash(false);
	//			}
	//		}
	//	}
	//}
}

void CTalkSessionForm::OnHotKeyShow()
{
	StopFlash();
	ChangeStyle(enumTalkSessionNormal);
}

void CTalkSessionForm::SetInformText(const char* name,const char* text)
{
	char buf[100];
	sprintf(buf,"New message from %s",name);
	m_pInformTitle->SetCaption(buf);
	string str=StringLimit(text,22);
	m_pInformTextBtn->SetCaption(str.c_str());
}

void CTalkSessionForm::_OnMouseInformButton(CGuiData *pSender, int x, int y, DWORD key)
{
	CTalkSessionForm *pThis=((CTalkSessionForm*)pSender->GetPointer());
	if (!pThis) return;
	pThis->StopFlash();
	pThis->ChangeStyle(enumTalkSessionNormal);
}

void CTalkSessionForm::_OnMouseSmallButton(CGuiData *pSender, int x, int y, DWORD key)
{
	CTalkSessionForm *pThis=((CTalkSessionForm*)pSender->GetPointer());
	if (!pThis) return;
	pThis->ChangeStyle(enumTalkSessionSmall);
}

void CTalkSessionForm::_OnMouseMinimizeButton(CGuiData *pSender, int x, int y, DWORD key)
{
	CTalkSessionForm *pThis=((CTalkSessionForm*)pSender->GetPointer());
	if (!pThis) return;
	pThis->ChangeStyle(enumTalkSessionMinimize);
}

void CTalkSessionForm::_OnMouseNormalButton(CGuiData *pSender, int x, int y, DWORD key)
{
	CTalkSessionForm *pThis=((CTalkSessionForm*)pSender->GetPointer());
	if (!pThis) return;
	pThis->StopFlash();
	pThis->ChangeStyle(enumTalkSessionNormal);
}

void CTalkSessionForm::_OnMouseQuitButton(CGuiData *pSender, int x, int y, DWORD key)
{
	CTalkSessionForm *pThis=((CTalkSessionForm*)pSender->GetPointer());
	if (!pThis) return;
	CS_Sess_Leave(pThis->m_dwSessioinID);
	//if (pThis->m_nShowChannel)
	//{
	//	CTalkSessionFormMgr::ReleaseShowChannel(pThis->m_nShowChannel);
	//	pThis->m_nShowChannel=0;
	//}
	pThis->ReleaseInformShowChannel();
	pThis->StopFlash();
	CTalkSessionFormMgr::CloseSessionForm(pThis->m_dwSessioinID);
}

void CTalkSessionForm::_OnMouseCloseButton(CGuiData *pSender, int x, int y, DWORD key)
{
	CTalkSessionForm *pThis=((CTalkSessionForm*)pSender->GetPointer());
	if (!pThis) return;
	pThis->StopFlash();
	pThis->ChangeStyle(enumTalkSessionHidden);
}

void CTalkSessionForm::_OnMouseSendButton(CGuiData *pSender, int x, int y, DWORD key)
{
	CTalkSessionForm *pThis=((CTalkSessionForm*)pSender->GetPointer());
	if (!pThis) return;
	string sendText=pThis->m_pEdit->GetCaption();
	if (sendText.length()<1) return;
	ChangeParseSymbol(sendText,g_TextParse.GetFaceCount());
	//FilterText(sendText,&g_stUIChat._strFilterTxt);
	CTextFilter::Filter(CTextFilter::DIALOG_TABLE,sendText);
	if (pThis->IsActiveSession())
	{
		CS_Sess_Say(pThis->m_dwSessioinID,sendText.c_str());
		pThis->m_pEdit->SetCaption("");
	}
	else
	{
		const char **pArrName=new const char *[1];
		pArrName[0]=pThis->GetMemberByIndex(0)->GetName();
		CS_Sess_Create(pArrName,1);
		SAFE_DELETE_ARRAY(pArrName);
		pThis->m_strWaitForSend+=sendText.c_str()+string("\n");
		pThis->m_pEdit->SetCaption("");
	}
	
	// Add by ning.yan  20080726  ��ʱ���淢�͵���������  Begin
	string strName = CGameScene::GetMainCha()->getName();
	string strMotto = CGameScene::GetMainCha()->getSecondName();
	__time64_t t;
	_time64(&t);
	tm *ltime=_gmtime64(&t);
	char ch[12];
	sprintf(ch,"  %d:%d:%d",ltime->tm_hour+8,ltime->tm_min,ltime->tm_sec);
	string time = ch;
	if ( strMotto.size() == 0 )
	{
		strName +=":" + time + "\r\n  ";
	}
	else
	{
		strName += +"(" + strMotto + "):" +time + "\r\n  ";
	}
	string saveText;
	saveText += strName + sendText + "\r\n\r\n";

	for(DWORD i=0; i< pThis->GetTotalMembers(); i++)
	{ // forѭ��������������
		if (CChatRecord::Save(pThis->GetMemberByIndex(i)->GetName(),pThis->GetTotalMembers(),saveText))
		{
			//string str="�ѳɹ����浽�ļ�:\""+CChatRecord::GetLastSavePath()+"\"��";
			//CCozeForm::GetInstance()->OnSystemMsg(str.c_str());
		}
		else
		{
			//CCozeForm::GetInstance()->OnSystemMsg("����ʧ�ܣ����ݿհף����̿ռ䲻����ļ�Ŀ¼û�п�дȨ�ޣ�");
		}
	}
	// End
}

void CTalkSessionForm::_OnTimerInformShowing(CGuiTime *pSender)
{
	CTalkSessionForm *pThis=(CTalkSessionForm*)pSender->GetUserPoint();
	if (!pThis) return;
	if (pSender->GetUserID()==1)
	{
		if (pThis->m_nShowSy<0)
		{
			pThis->m_nShowSy+=pThis->m_pInformForm->GetHeight()/10;
			if (pThis->m_nShowSy>0) pThis->m_nShowSy=0;
			pThis->m_pInformForm->SetPos(pThis->m_nShowSx,pThis->m_nShowSy);
			pThis->m_pInformForm->Refresh();
			if (!pThis->m_pInformForm->GetIsShow()) pThis->m_pInformForm->Show();
		}
		else
		{
			pThis->m_pTimer1->Release();
			pThis->m_pTimer1=NULL;
			pThis->m_pTimer2=CGuiTime::Create(2000,_OnTimerInformShowing);
			pThis->m_pTimer2->SetUserID(2);
			pThis->m_pTimer2->SetUserPoint(pThis);
			//pThis->m_pInformForm->KillTimer(1);
			//pThis->m_pInformForm->SetTimer(2,2000,pThis);
		}
	}
	else if (pSender->GetUserID()==2)
	{
		pThis->ChangeStyle(enumTalkSessionMinimize);
	}
}

void CTalkSessionForm::_OnMouseFaceListButton(CGuiData *pSender, int x, int y, DWORD key)
{
	CTalkSessionForm *pThis=((CTalkSessionForm*)pSender->GetPointer());
	if (!pThis) return;
	int sx=x;
	int sy=y;
	if (x+pThis->m_pFaceGrid->GetWidth()>g_pGameApp->GetWindowWidth())
		sx=g_pGameApp->GetWindowWidth()-pThis->m_pFaceGrid->GetWidth();
	if (y+pThis->m_pFaceGrid->GetHeight()>g_pGameApp->GetWindowHeight())
		sy=g_pGameApp->GetWindowHeight()-pThis->m_pFaceGrid->GetHeight();
	sx-=pThis->m_pNormalForm->GetLeft();
	sy-=pThis->m_pNormalForm->GetTop();
	pThis->m_pFaceGrid->SetPos(sx,sy);
	pThis->m_pFaceGrid->Refresh();
	pThis->m_pFaceGrid->SetIsShow(true);
}

void CTalkSessionForm::_OnMouseCleanText(CGuiData *pSender, int x, int y, DWORD key)
{
	CTalkSessionForm *pThis=((CTalkSessionForm*)pSender->GetPointer());
	if (!pThis) return;
	//pThis->m_pNormalList->GetItems()->Clear();
	pThis->m_pMemo->Clear();
	//pThis->m_strTalkMsg.clear();
	pThis->ClearMsg();
}

void CTalkSessionForm::_OnMouseSelectFace(CGuiData *pSender)
{
	pSender->SetIsShow( false );
	CTalkSessionForm *pThis=((CTalkSessionForm*)pSender->GetPointer());
	if (!pThis) return;
	if (strlen(pThis->m_pNormalEdit->GetCaption())>39) return;

	CGraph *pGraph=pThis->m_pFaceGrid->GetSelect();
	if (pGraph)
	{
		char buf[20];
		int index=pThis->m_pFaceGrid->GetSelectIndex();
		//pThis->m_pNormalEdit->SetIsParseText(true);
		pThis->m_pNormalEdit->SetActive(pThis->m_pNormalEdit);
		string str=pThis->m_pNormalEdit->GetCaption();
		sprintf(buf,"#%.2d",index);
		str+=buf;
		pThis->m_pNormalEdit->SetCaption(str.c_str());	
	}
}

void CTalkSessionForm::_OnFaceLostFocus(CGuiData *pSender)
{
	pSender->SetIsShow( false );
}

void CTalkSessionForm::_OnMouseRightPlayerMenu(CGuiData *pSender, int x, int y, DWORD key)
{
	if( key & Mouse_RDown )
	{
		CList* pList = dynamic_cast<CList*>(pSender);
		if( !pList ) return;
		CTextGraph *pSelectItem=dynamic_cast<CTextGraph*>(pList->GetSelectItem()->GetBegin());
		if (!pSelectItem) return;

		int sx=x;
		int sy=y;
		if (x+m_playerMouseRight->GetWidth()>g_pGameApp->GetWindowWidth())
			sx=g_pGameApp->GetWindowWidth()-m_playerMouseRight->GetWidth();
		if (y+m_playerMouseRight->GetHeight()>g_pGameApp->GetWindowHeight())
			sy=g_pGameApp->GetWindowHeight()-m_playerMouseRight->GetHeight();
		m_playerMouseRight->SetPointer(pSelectItem->GetPointer());
		pList->GetForm()->PopMenu(m_playerMouseRight,sx,sy);
		//m_playerMouseRight->SetPos(sx,sy);
		//m_playerMouseRight->SetIsShow(true);
		//m_playerMouseRight->Refresh();
	}
}

void CTalkSessionForm::_OnPlayerSelect(CGuiData *pSender, int x, int y, DWORD key)
{
	m_playerMouseRight->SetIsShow(false);
	CMenuItem* pItem=m_playerMouseRight->GetSelectMenu();
	if (!pItem) return;
	string str=pItem->GetString();
	if (str=="���Ӻ���" )
	{
		CSessionMember* pSessionMember=static_cast<CSessionMember*>(m_playerMouseRight->GetPointer());
		CS_Frnd_Invite(pSessionMember->GetName());
	}
}

void CTalkSessionForm::_OnMouseSaveButton(CGuiData *pSender, int x, int y, DWORD key)
{   //delete by ning.yan 2008-07-29 ȥ���ֶ����������¼���ܣ���Ϊ�Զ����� Begin
	//CTalkSessionForm *pThis=((CTalkSessionForm*)pSender->GetPointer());
	//if (!pThis || !pThis->GetMemberByIndex(0)) return;
	//string saveText;
	//vector <sTalkMsg*>::iterator Iter;
	//for (Iter=pThis->m_TalkMsg.begin();Iter!=pThis->m_TalkMsg.end();Iter++)
	//{
	//	string strName=(*Iter)->name;
	//	if ((*Iter)->motto.length()==0)
	//	{
	//		strName+=":\r\n  ";
	//	}
	//	else
	//	{
	//		strName+=+"("+(*Iter)->motto+")"+":\r\n  ";
	//	}
	//	saveText+=strName+(*Iter)->text+"\r\n\r\n";
	//}
	//if (CChatRecord::Save(pThis->GetMemberByIndex(0)->GetName(),pThis->GetTotalMembers(),saveText))
	//{
	//	string str="�ѳɹ����浽�ļ�:\""+CChatRecord::GetLastSavePath()+"\"��";
	//	//g_stUICoze.OnSystemSay( str.c_str() );
	//	CCozeForm::GetInstance()->OnSystemMsg(str.c_str());
	//}
	//else
	//{
	//	//g_stUICoze.OnSystemSay( "����ʧ�ܣ����ݿհף����̿ռ䲻����ļ�Ŀ¼û�п�дȨ�ޣ�" );
	//	CCozeForm::GetInstance()->OnSystemMsg("����ʧ�ܣ����ݿհף����̿ռ䲻����ļ�Ŀ¼û�п�дȨ�ޣ�");
	//}
	//End

}


void CTalkSessionForm::GetMsg(DWORD chaID,string talkMsg)
{
	string nameBuf="";
	string secondNameBuf="";
	CSessionMember *pMember=GetMemberByID(chaID);
	if (pMember)
	{
		nameBuf=pMember->GetName();
		secondNameBuf=pMember->GetMotto();
	}
	else
	{
		if (CGameScene::GetMainCha())
		{
			nameBuf=CGameScene::GetMainCha()->getHumanName();
			secondNameBuf=CGameScene::GetMainCha()->getSecondName();
		}
		else
		{
			nameBuf="�Լ�";
			secondNameBuf="";
		}
	}
	sTalkMsg *pMsg=new sTalkMsg;
	pMsg->name=nameBuf;
	pMsg->motto=secondNameBuf;
	pMsg->text=talkMsg;
	m_TalkMsg.push_back(pMsg);
	//m_strTalkMsg+=buf+string(" : ")+talkMsg+"\n";
	RefreshForm();

	// Add by ning.yan  20080727  ��ʱ������յ�����������  Begin
	if( pMsg->name != CGameScene::GetMainCha()->getName())
	{
		string strName =pMsg->name;
		__time64_t t;
		_time64(&t);
		tm *ltime=_gmtime64(&t);
		char ch[12];
		sprintf(ch,"  %d:%d:%d",ltime->tm_hour+8,ltime->tm_min,ltime->tm_sec);
		string time = ch;
		if ( pMsg->motto.size() == 0 )
		{
			strName += ":" + time + "\r\n  ";
		}
		else
		{
			strName += +"(" + pMsg->motto + ");" + time + "\r\n  ";
		}
		string saveText;
		saveText += strName + pMsg->text + "\r\n\r\n";

		for( DWORD i=0; i< GetTotalMembers(); i++)
		{
			if (CChatRecord::Save( GetMemberByIndex(i)->GetName(), GetTotalMembers(), saveText))
			{
				//string str="�ѳɹ����浽�ļ�:\""+CChatRecord::GetLastSavePath()+"\"��";
				//CCozeForm::GetInstance()->OnSystemMsg(str.c_str());
			}
			else
			{
				//CCozeForm::GetInstance()->OnSystemMsg("����ʧ�ܣ����ݿհף����̿ռ䲻����ļ�Ŀ¼û�п�дȨ�ޣ�");
			}
		}
	}
	// End
}

void CTalkSessionForm::SendBufferMsg()
{
	if (!m_strWaitForSend.empty())
	{
		char *buf=new char[m_strWaitForSend.length()+10];
		strcpy(buf,m_strWaitForSend.c_str());
		char *token=strtok(buf, "\n");
		while( token != NULL )
		{
			CS_Sess_Say(m_dwSessioinID,token);
			token = strtok(NULL, "\n");
		}
		m_strWaitForSend.clear();
		//delete buf;
		SAFE_DELETE_ARRAY(buf); // UI��������
	}
	if (!m_strWaitForAdded.empty())
	{
		char *buf=new char[m_strWaitForAdded.length()+10];
		strcpy(buf,m_strWaitForAdded.c_str());
		char *token=strtok(buf, "\n");
		while( token != NULL )
		{
			CS_Sess_Add(GetSessionID(),token);
			token = strtok(NULL, "\n");
		}
		m_strWaitForAdded.clear();
		//delete buf;
		SAFE_DELETE_ARRAY(buf); // UI��������
		//CS_Sess_Add(GetSessionID(),m_strWaitForAdded.c_str());
		//m_strWaitForAdded.clear();
	}
}

void CTalkSessionForm::ClearMsg()
{
	vector<sTalkMsg*>::iterator Iter;
	while (m_TalkMsg.size()>0)
	{
		Iter=m_TalkMsg.begin();
		sTalkMsg* pNode=*Iter;
		//delete pNode;
		SAFE_DELETE(pNode); // UI��������
		m_TalkMsg.erase(Iter);
	}
}

bool CTalkSessionForm::_OnHotKey(CForm *pSender, char key)
{
	CTalkSessionForm *pThis=((CTalkSessionForm*)pSender->GetPointer());
	if (pThis && (key=='X'|| key=='x'))
	{
		pThis->_OnMouseSendButton(pSender,0,0,key);
		return true;
	}
	return false;
}

void CTalkSessionForm::GuiFormEscCloseEvent(CForm *pForm)
{
	CTalkSessionForm *pThis=((CTalkSessionForm*)pForm->GetPointer());
	if (pThis)
	{
		_OnMouseCloseButton(pForm,0,0,VK_ESCAPE);
	}
}

//------------------------------------------------------------------------
//	CTalkSessionFormMgr��ʵ��
//------------------------------------------------------------------------

//RECT CTalkSessionFormMgr::INFORM_FORM;
//RECT CTalkSessionFormMgr::INFORM_TITLE;
//int CTalkSessionFormMgr::INFORM_TABLE_W;
//int CTalkSessionFormMgr::INFORM_TABLE_H;
//POINT CTalkSessionFormMgr::INFORM_TABLE_POSITION;
int CTalkSessionFormMgr::m_Channel[4]={0,0,0,0};
DWORD CTalkSessionFormMgr::m_SessionCounts=0;
UCHAR CTalkSessionFormMgr::sApplyMember::numbers=0;
vector<CTalkSessionForm*> CTalkSessionFormMgr::m_SessionFormsLink;

CTalkSessionFormMgr::CTalkSessionFormMgr(void)
{
}

CTalkSessionFormMgr::~CTalkSessionFormMgr(void)
{
	vector <CTalkSessionForm*>::iterator Iter;
	while (m_SessionFormsLink.size()>0)
	{
		Iter=m_SessionFormsLink.begin();
		//delete (*Iter);
		SAFE_DELETE(*Iter); // UI��������
		m_SessionFormsLink.erase(Iter);
	}
}

//CTalkSessionForm* CTalkSessionFormMgr::CreateSessionForm(CMember *pMember,eTalkSessionStyle style)
//{
//	vector <PS_Sessions>::iterator Iter;
//	for (Iter=m_SessionFormsLink.begin();Iter!=m_SessionFormsLink.end();Iter++)
//	{
//		if ((*Iter)->session->hasMember(pMember))
//		{
//			if ((*Iter)->session->GetTotalMembers()==1)
//			{
//				(*Iter)->session->ChangeStyle(enumTalkSessionNormal);
//				return (*Iter)->session;
//			}
//		}
//	}
//	if (m_SessionCounts>MAX_SESSION_FORM) return NULL;
//	CTalkSessionForm *pSessionForm=new CTalkSessionForm(enumTalkSessionNormal);
//	if (pSessionForm->AddMember(pMember)==false)
//	{
//		delete pSessionForm;
//		return NULL;
//	}
//	_sSessions *psSession=new _sSessions;
//	psSession->sessionID=++m_SessionIDCount;
//	psSession->session=pSessionForm;
//	m_SessionFormsLink.push_back(psSession);
//	return NULL;
//}

//bool CTalkSessionFormMgr::AddToSessionForm(CMember *pMember,CTalkSessionForm *session)
//{
//	if (!pMember||!session) return false;
//	return session->AddMember(pMember);
//}

//void CTalkSessionFormMgr::LeaveSessionForm(CTalkSessionForm *session)
//{
//	vector <PS_Sessions>::iterator Iter;
//	for (Iter=m_SessionFormsLink.begin();Iter!=m_SessionFormsLink.end();Iter++)
//	{
//		if ((*Iter)->session==session)
//		{
//			delete (*Iter)->session;
//			delete (*Iter);
//			m_SessionFormsLink.erase(Iter);
//			return;
//		}
//	}
//}

bool CTalkSessionFormMgr::AddToSession(DWORD sessionID,stNetSessCreate *pMember)
{
	if (!pMember||sessionID==0) return false;
	CTalkSessionForm *pSessionForm=GetSessionFormByID(sessionID);
	if (!pSessionForm) return false;
	pSessionForm->AddMembers(&pMember);
	//pSessionForm->SetInformText("ϵͳ��Ϣ",pMember.  );
	return true;
}

void CTalkSessionFormMgr::CloseSessionForm(DWORD sessionID)
{
	vector <CTalkSessionForm*>::iterator Iter;
	for (Iter=m_SessionFormsLink.begin();Iter!=m_SessionFormsLink.end();Iter++)
	{
		if ((*Iter)->GetSessionID()==sessionID)
		{
			g_stUIChat.GetSessionNode()->DelItem((CItemObj*)((*Iter)->GetData()));
			g_stUIChat.GetTeamView()->Refresh();
			//delete (*Iter);
			SAFE_DELETE(*Iter); // UI��������
			m_SessionFormsLink.erase(Iter);
			--m_SessionCounts;
			return;
		}
	}
}

void CTalkSessionFormMgr::ClearAll()
{
	vector <CTalkSessionForm*>::iterator Iter;
	while (m_SessionFormsLink.size()>0)
	{
		Iter=m_SessionFormsLink.begin();
		//delete (*Iter);
		SAFE_DELETE(*Iter); // UI��������
		m_SessionFormsLink.erase(Iter);
	}
	m_SessionCounts=0;
}

void CTalkSessionFormMgr::LeaveSession(DWORD sessionID,DWORD chaID)
{
	vector <CTalkSessionForm*>::iterator Iter;
	for (Iter=m_SessionFormsLink.begin();Iter!=m_SessionFormsLink.end();Iter++)
	{
		if ((*Iter)->GetSessionID()==sessionID)
		{
			if ((*Iter)->GetTotalMembers()==1 && chaID==(*Iter)->GetMemberByIndex(0)->GetID())
			{
				(*Iter)->SetActiveSession(false);
			}
			else
			{
				(*Iter)->DelMember(chaID);
			}
			return;
		}
	}
}

CTalkSessionForm* CTalkSessionFormMgr::GetSessionFormByIndex(int nIndex)
{
	if (nIndex>=(int)m_SessionFormsLink.size()) return NULL;
	return m_SessionFormsLink[nIndex];
}

CTalkSessionForm* CTalkSessionFormMgr::GetSessionFormByID(DWORD sessionID)
{
	vector <CTalkSessionForm*>::iterator Iter;
	for (Iter=m_SessionFormsLink.begin();Iter!=m_SessionFormsLink.end();Iter++)
	{
		if ((*Iter)->GetSessionID()==sessionID)
		{
			return *Iter;
		}
	}
	return NULL;
}

CTalkSessionForm* CTalkSessionFormMgr::GetSessionFormByName(string name,int index)
{
	vector <CTalkSessionForm*>::iterator Iter;
	for (Iter=m_SessionFormsLink.begin();Iter!=m_SessionFormsLink.end();Iter++)
	{
		if ((*Iter)->GetTotalMembers()==1&&(*Iter)->hasMember(name))
		{
			if (index==0)
				return *Iter;
			--index;
		}
	}
	return NULL;
}

CTalkSessionForm* CTalkSessionFormMgr::GetSessionFormByForm(CForm *form)
{
	vector <CTalkSessionForm*>::iterator Iter;
	for (Iter=m_SessionFormsLink.begin();Iter!=m_SessionFormsLink.end();Iter++)
	{
		if ((*Iter)->m_pForm==form)
		{
			return *Iter;
		}
	}
	return NULL;
}

void CTalkSessionFormMgr::ApplySession(sApplyMember pApplyMembers[])
{
	CTalkSessionForm *pSessionForm=NULL;
	for (DWORD i=0;i<m_SessionCounts;i++)
	{
		pSessionForm=GetSessionFormByIndex(i);
		if (pSessionForm)
		{
			if (pSessionForm->GetTotalMembers()==sApplyMember::numbers)
			{
				int j=0;
				for (;j<sApplyMember::numbers;j++)
				{
					if (!pSessionForm->hasMember(pApplyMembers[j].name))
					{
						break;
					}
				}
				if (j==sApplyMember::numbers)
				{
					eTalkSessionStyle style=pSessionForm->GetStyle();
					if (style==enumTalkSessionHidden||style==enumTalkSessionInform||style==enumTalkSessionMinimize)
					{
						pSessionForm->ChangeStyle(enumTalkSessionNormal);
					}
					if (pSessionForm->m_pForm) pSessionForm->m_pForm->Show();
					return;
				}
			}
		}
	}
	const char **pArrName=new const char *[sApplyMember::numbers];
	for (int i=0;i<sApplyMember::numbers;i++)
	{
		pArrName[i]=pApplyMembers[i].name.c_str();
	}
	CS_Sess_Create(pArrName,sApplyMember::numbers);
	SAFE_DELETE_ARRAY(pArrName);
}

void CTalkSessionFormMgr::ApplySession(CMember *pArrMembers[],int memberNum)
{
	CTalkSessionForm *pSessionForm=NULL;
	for (DWORD i=0;i<m_SessionCounts;i++)
	{
		pSessionForm=GetSessionFormByIndex(i);
		if (pSessionForm)
		{
			if (pSessionForm->GetTotalMembers()==memberNum)
			{
				int j=0;
				for (;j<memberNum;j++)
				{
					if (!pSessionForm->hasMember(pArrMembers[j]->GetName()))
					{
						break;
						//pSessionForm->SetActiveSession(true,sessionID);
						//pSessionForm->SendBufferMsg();
						//return pSessionForm;
					}
				}
				if (j==memberNum)
				{
					eTalkSessionStyle style=pSessionForm->GetStyle();
					if (style==enumTalkSessionHidden||style==enumTalkSessionInform||style==enumTalkSessionMinimize)
					{
						pSessionForm->ChangeStyle(enumTalkSessionNormal);
					}
					pSessionForm->SetIsFlash(false);
					if (pSessionForm->m_pForm) pSessionForm->m_pForm->Show();
					return;
				}
			}
		}
	}

	const char **pArrName=new const char *[memberNum];
	for (int i=0;i<memberNum;i++)
	{
		pArrName[i]=pArrMembers[i]->GetName();
	}
	CS_Sess_Create(pArrName,memberNum);
	SAFE_DELETE_ARRAY(pArrName);
}

CTalkSessionForm* CTalkSessionFormMgr::CreateSessionForm(DWORD sessionID, stNetSessCreate *pArrMembers[],int memberNum)
{
	CTalkSessionForm *pSessionForm=GetSessionFormByID(sessionID);
	if (pSessionForm) return pSessionForm;

	if (memberNum==1)
	{
		for (DWORD i=0;i<m_SessionCounts;i++)
		{
			if (!(pSessionForm=GetSessionFormByIndex(i))->IsActiveSession())
			{
				if (pSessionForm->GetTotalMembers()==1)
				{
					if (pSessionForm->hasMember(pArrMembers[0]->szChaName))
					{
						pSessionForm->SetActiveSession(true,sessionID);
						pSessionForm->SendBufferMsg();
						return pSessionForm;
					}
				}
			}
		}
	}
	pSessionForm=new CTalkSessionForm(sessionID,enumTalkSessionNormal);
	pSessionForm->AddMembers(pArrMembers,memberNum);
	m_SessionFormsLink.push_back(pSessionForm);
	++m_SessionCounts;

	char buf[20];
	string str=pArrMembers[0]->szChaName;
	str+="("+string(itoa(memberNum,buf,10))+")";
	CTextGraph* pItem = new CTextGraph(2);
	pItem->SetHint(str.c_str());
	str=StringLimit(str,14);
	pItem->SetString(str.c_str());
	pItem->SetColor(0xef5c3cd3);
	pItem->SetPointer(pSessionForm);
	g_stUIChat.GetSessionNode()->AddItem(pItem);
	pSessionForm->SetData( pItem );
	CChatIconInfo *pIconInfo=GetChatIconInfo(pArrMembers[0]->sIconID);
	if (pIconInfo)
	{
		CGuiPic* pPic=pItem->GetImage();
		string strPath="texture/ui/HEAD/";	
		pPic->LoadImage((strPath+pIconInfo->szSmall).c_str(),16,16,0,pIconInfo->nSmallX,pIconInfo->nSmallY);
		pPic->LoadImage("texture/ui/system/slide.tga",16,16,1,pIconInfo->nSmallX,pIconInfo->nSmallY);
		pPic->SetFrame(0);
	}
	g_stUIChat.GetTeamView()->Refresh();

	return pSessionForm;
}

CTalkSessionForm* CTalkSessionFormMgr::CreateHiddenSessionForm(DWORD sessionID, stNetSessCreate *pArrMembers[],int memberNum)
{
	CTalkSessionForm *pSessionForm=GetSessionFormByID(sessionID);
	if (pSessionForm) return pSessionForm;

	if (memberNum==1)
	{
		for (DWORD i=0;i<m_SessionCounts;i++)
		{
			if (!(pSessionForm=GetSessionFormByIndex(i))->IsActiveSession())
			{
				if (pSessionForm->GetTotalMembers()==1)
				{
					if (pSessionForm->hasMember(pArrMembers[0]->szChaName))
					{
						pSessionForm->SetActiveSession(true,sessionID);
						pSessionForm->SendBufferMsg();
						return pSessionForm;
					}
				}
			}
		}
	}
	pSessionForm=new CTalkSessionForm(sessionID,enumTalkSessionHidden);
	pSessionForm->AddMembers(pArrMembers,memberNum);
	m_SessionFormsLink.push_back(pSessionForm);
	++m_SessionCounts;

	char buf[20];
	string str=pArrMembers[0]->szChaName;
	str+="("+string(itoa(memberNum,buf,10))+")";
	CTextGraph* pItem = new CTextGraph(2);
	pItem->SetString(str.c_str());
	pItem->SetColor(0xef5c3cd3);
	pItem->SetPointer(pSessionForm);
	g_stUIChat.GetSessionNode()->AddItem(pItem);
	pSessionForm->SetData( pItem );
	CChatIconInfo *pIconInfo=GetChatIconInfo(pArrMembers[0]->sIconID);
	if (pIconInfo)
	{
		CGuiPic* pPic=pItem->GetImage();
		string strPath="texture/ui/HEAD/";	
		pPic->LoadImage((strPath+pIconInfo->szSmall).c_str(),16,16,0,pIconInfo->nSmallX,pIconInfo->nSmallY);
	}
	g_stUIChat.GetTeamView()->Refresh();

	return pSessionForm;
}

void CTalkSessionFormMgr::SessionReceiveMsg(DWORD sessionID,DWORD chaID,string talkMsg)
{
	CTalkSessionForm* pSessionForm=GetSessionFormByID(sessionID);
	if (!pSessionForm) return;
	//if (!pSessionForm->hasMember(chaID)) return;
	if (pSessionForm->GetStyle()==enumTalkSessionHidden)
	{
		string strMsg="";
		CSessionMember *pMember=pSessionForm->GetMemberByID(chaID);
		if (pMember)
		{
			//strMsg=pMember->GetName()+string(":");
			strMsg=pMember->GetName();
		}
		//strMsg+=talkMsg;
		//pSessionForm->SetInformText(strMsg.c_str());
		pSessionForm->SetInformText(strMsg.c_str(),talkMsg.c_str());
		pSessionForm->ChangeStyle(enumTalkSessionInform);
	}
	pSessionForm->GetMsg(chaID,talkMsg);
}

//void CTalkSessionFormMgr::TableInit()
//{
//	RECT rect1,rect2;
//	SetRect(&rect1,0,0,184,19);
//	SetRect(&rect2,0,0,184,100);
//	INFORM_TABLE_W=3;
//	INFORM_TABLE_H=10;
//	POINT p={580,440};
//	SetTablePosition(p);
//}
//
//void CTalkSessionFormMgr::SetInformSize(RECT rectFrom,RECT rectTitle)
//{
//	//���滯���ο�
//	LONG tmp;
//	if (rectFrom.top>rectFrom.bottom)
//	{
//		tmp=rectFrom.bottom;
//		rectFrom.bottom=rectFrom.top;
//		rectFrom.top=tmp;
//	}
//	if (rectFrom.left>rectFrom.right)
//	{
//		tmp=rectFrom.left;
//		rectFrom.left=rectFrom.right;
//		rectFrom.right=tmp;
//	}
//	if (rectTitle.top>rectTitle.bottom)
//	{
//		tmp=rectTitle.bottom;
//		rectTitle.bottom=rectTitle.top;
//		rectTitle.top=tmp;
//	}
//	if (rectTitle.left>rectTitle.right)
//	{
//		tmp=rectTitle.left;
//		rectTitle.left=rectTitle.right;
//		rectTitle.right=tmp;
//	}
//	INFORM_FORM=rectFrom;
//	INFORM_TITLE=rectTitle;
//}
//
//void CTalkSessionFormMgr::SetTablePosition(POINT tPosition)
//{
//	INFORM_TABLE_POSITION=tPosition;
//}
//
//RECT CTalkSessionFormMgr::TableToTitle(int tx,int ty)
//{
//	RECT ret;
//	int h=INFORM_TITLE.bottom-INFORM_TITLE.top+1;
//	int w=INFORM_TITLE.right-INFORM_TITLE.left+1;
//	if (tx>INFORM_TABLE_W)
//	{
//		tx=tx % INFORM_TABLE_W;
//	}
//	if (ty>INFORM_TABLE_H)
//	{
//		ty=ty % INFORM_TABLE_H;
//	}
//	ret.top=INFORM_TABLE_POSITION.x-(tx*w+w);
//	ret.left=INFORM_TABLE_POSITION.y-(ty*h+h);
//	ret.bottom=ret.top+h-1;
//	ret.right=ret.left+w-1;
//	return ret;
//}
//
//RECT CTalkSessionFormMgr::TableToForm(int tx,int ty)
//{
//	RECT ret=TableToTitle(tx,ty);
//	ret.top-=(INFORM_FORM.bottom-INFORM_FORM.top+1)-(INFORM_TITLE.bottom-INFORM_TITLE.top+1);
//	return ret;
//}

int CTalkSessionFormMgr::GetShowChannel()
{
	if (m_Channel[1]<=m_Channel[2])
	{
		m_Channel[1]++;
		return 1;
	}
	if (m_Channel[2]<=m_Channel[3])
	{
		m_Channel[2]++;
		return 2;
	}
	if (m_Channel[3]<=m_Channel[4])
	{
		m_Channel[3]++;
		return 3;
	}
	m_Channel[1]++;
	return 1;
}

void CTalkSessionFormMgr::ReleaseShowChannel(int n)
{
	if (n>0&&n<4)
	{
		m_Channel[n]--;
	}
}

POINT CTalkSessionFormMgr::GetMiniFormPosition(DWORD sessionID)
{
	POINT pt;
	RECT rect;
	int Flag[30];
	ZeroMemory(Flag,sizeof(int)*30);
	vector <CTalkSessionForm*>::iterator Iter;
	for (Iter=m_SessionFormsLink.begin();Iter!=m_SessionFormsLink.end();Iter++)
	{
		if ((*Iter)->GetSessionID()!=sessionID)
		{
			CForm *pForm=(*Iter)->m_pMinimizeForm;
			SetRect(&rect,pForm->GetLeft(),pForm->GetTop(),pForm->GetRight(),pForm->GetBottom());
			pt.x=(rect.right+rect.left)/2;
			pt.x=(pt.x-INFORM_FORM_SHOW_SX)/MINIMIZE_FORM_WIDTH;
			pt.y=(rect.top+rect.bottom)/2;
			pt.y=pt.y/MINIMIZE_FORM_HEIGHT;
			if (pt.x<=2 && pt.x>=0 && pt.y>=0 && pt.y<=9)
			{
				Flag[pt.y*3+pt.x]++;
			}
		}
	}
	int least=-1;
	for (int i=0;i<30;i++)
	{
		if (Flag[i]<least || least==-1)
		{
			least=Flag[i];
			pt.x=INFORM_FORM_SHOW_SX+MINIMIZE_FORM_WIDTH*(i%3);
			pt.y=MINIMIZE_FORM_HEIGHT*(i/3);
		}
	}
	return pt;

	//for (int i=0;i<30;i++)
	//{
	//	if (Flag[i]==0)
	//	{
	//		pt.x=INFORM_FORM_SHOW_SX+MINIMIZE_FORM_WIDTH*(i%3);
	//		pt.y=MINIMIZE_FORM_HEIGHT*(i/3);
	//		return pt;
	//	}
	//}
	//pt.x=INFORM_FORM_SHOW_SX;
	//pt.y=0;
	//return pt;
}

POINT CTalkSessionFormMgr::GetNormalFormPosition(DWORD sessionID)
{
	POINT pt;
	int Flag[36];
	POINT point;
	ZeroMemory(Flag,sizeof(int)*36);
	vector <CTalkSessionForm*>::iterator Iter;
	for (Iter=m_SessionFormsLink.begin();Iter!=m_SessionFormsLink.end();Iter++)
	{
		if ((*Iter)->GetSessionID()!=sessionID)
		{
			CForm *pForm=(*Iter)->m_pNormalForm;
			//point.SetPoint(pForm->GetLeft(),pForm->GetTop());
			point.x = pForm->GetLeft();
			point.y = pForm->GetTop();
			pt.x=(point.x-NORMAL_FORM_SHOW_SX)/NORMAL_FORM_OFFSET_WIDTH;
			if ( pt.x < 0 || pt.x> 5) continue;
			pt.y=(point.y-NORMAL_FORM_SHOW_SY-NORMAL_FORM_OFFSET_HEIGHT*pt.x)/(NORMAL_FORM_OFFSET_HEIGHT+NORMAL_FORM_OFFSET_HEIGHT/2);
			if ( pt.y >= 0 || pt.y<= 5)
			{
				Flag[pt.y*6+pt.x]++;
			}
		}
	}
	int least=-1;
	for (int i=0;i<36;i++)
	{
		if (Flag[i]<least || least==-1)
		{
			least=Flag[i];
			pt.x=NORMAL_FORM_SHOW_SX+NORMAL_FORM_OFFSET_WIDTH*(i%6);
			pt.y=NORMAL_FORM_SHOW_SY+(NORMAL_FORM_OFFSET_HEIGHT+NORMAL_FORM_OFFSET_HEIGHT/2)*(i/6)+NORMAL_FORM_OFFSET_HEIGHT*(i%6);
		}
	}
	return pt;
}

void CTalkSessionFormMgr::OnClickOpenSession(CItemObj* pItem)
{
	vector <CTalkSessionForm*>::iterator Iter;
	for (Iter=m_SessionFormsLink.begin();Iter!=m_SessionFormsLink.end();Iter++)
	{
		if ((*Iter)->GetData()==(void*)pItem)
		{
			(*Iter)->StopFlash();
			(*Iter)->ChangeStyle(enumTalkSessionNormal);
		}
	}
}

void CTalkSessionFormMgr::OnClickCloseSession(CTalkSessionForm* pSession)
{
	if (!pSession) return;
	CS_Sess_Leave(pSession->m_dwSessioinID);
	//if (pSession->m_nShowChannel)
	//{
	//	ReleaseShowChannel(pSession->m_nShowChannel);
	//	pSession->m_nShowChannel=0;
	//}
	pSession->ReleaseInformShowChannel();
	pSession->StopFlash();
	CloseSessionForm(pSession->m_dwSessioinID);
}

void CTalkSessionFormMgr::OnFlashSession()
{
	vector <CTalkSessionForm*>::iterator Iter;
	for (Iter=m_SessionFormsLink.begin();Iter!=m_SessionFormsLink.end();Iter++)
	{
		CTextGraph* pItem = (CTextGraph*)((*Iter)->GetData());
		char buf[20];
		string str=(*Iter)->GetMemberByIndex(0)->GetName();
		str+="("+string(itoa((*Iter)->GetTotalMembers(),buf,10))+")";
		pItem->SetHint(str.c_str());
		str=StringLimit(str,14);
		pItem->SetString(str.c_str());
		CGuiPic* pPic=pItem->GetImage();
		if ((*Iter)->GetIsFlash())
		{
			pPic->Next();
			if ((*Iter)->GetStyle()==enumTalkSessionMinimize)
			{
				CLabelEx* pText=dynamic_cast<CLabelEx*>((*Iter)->m_pMinimizeForm->Find("labName"));
				if (pText)
				{
					pText->SetTextColor(((*Iter)->m_nFlashTimes&1)?0xffa0a030:COLOR_BLACK);
					if ((*Iter)->m_nFlashTimes<13) (*Iter)->m_nFlashTimes++;
				}
			}
		}
		else
		{
			pPic->SetFrame(0);
			CLabelEx* pText=dynamic_cast<CLabelEx*>((*Iter)->m_pMinimizeForm->Find("labName"));
			if (pText)
			{
				pText->SetTextColor(COLOR_BLACK);
			}
			(*Iter)->m_nFlashTimes=0;
		}
		g_stUIChat.GetTeamView()->Refresh();
	}
}

void CTalkSessionFormMgr::RefreshSessionMember(DWORD chaID,const char* szMotto)
{
	if (szMotto==NULL) return;
	vector <CTalkSessionForm*>::iterator Iter;
	for (Iter=m_SessionFormsLink.begin();Iter!=m_SessionFormsLink.end();Iter++)
	{
		CSessionMember *pSessMember=(*Iter)->GetMemberByID(chaID);
		if (pSessMember)
		{
			pSessMember->SetMotto(szMotto);
			(*Iter)->RefreshForm();
		}
	}
}

bool CTalkSessionFormMgr::hasFlashSession()
{
	int i=0;
	CTalkSessionForm* pSess;
	while (pSess=GetSessionFormByIndex(i))
	{
		if (pSess->GetIsFlash()) return true;
		i++;
	}
	return false;
}

bool CTalkSessionFormMgr::OnHotKeyShow( char& key, int& control )
{
	char qqKey=g_stUIChat.GetQQFrom()->GetHotKey();
	if (key!=qqKey)
	{
		return false;
	}
	int i=0;
	CTalkSessionForm* pSessionForm;
	for (pSessionForm=GetSessionFormByIndex(i);pSessionForm;pSessionForm=GetSessionFormByIndex(++i))
	{
		eTalkSessionStyle eTalkStyle=pSessionForm->GetStyle();
		if (eTalkStyle==enumTalkSessionInform)
		{
			pSessionForm->OnHotKeyShow();
			return true;
		}
		else if (eTalkStyle==enumTalkSessionMinimize)
		{
			pSessionForm->OnHotKeyShow();
			return true;
		}
	}
	return false;
}

DWORD CTalkSessionFormMgr::GetSessionCount()
{
	return m_SessionCounts;
}

void CTalkSessionFormMgr::PrintTraceInfo()
{
	string strInfo="";
	char szBuf[2048]={0};
	vector <CTalkSessionForm*>::iterator Iter;
	for (Iter=m_SessionFormsLink.begin();Iter!=m_SessionFormsLink.end();Iter++)
	{
		CTalkSessionForm* pSessionForm=(*Iter);
		DWORD dwSessionID=pSessionForm->GetSessionID();
		int nMemberCount=pSessionForm->GetTotalMembers();
		sprintf(szBuf,"�ỰID=%d ����=%d ��Ա:",nMemberCount,dwSessionID);
		strInfo+=szBuf;
		for (int i=0;i<nMemberCount;i++)
		{
			CSessionMember* pMember=pSessionForm->GetMemberByIndex(i);
			strInfo+=string(pMember->GetName())+"|";
		}
		strInfo+="|\n";
	}
	MessageBox(g_pGameApp->GetHWND(), "error", strInfo.c_str(), MB_OK);
}
