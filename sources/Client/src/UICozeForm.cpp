#include "StdAfx.h"
#include "UICozeForm.h"
#include "uiformmgr.h"
#include "uiform.h"
#include "uicheckbox.h"
#include "uitextbutton.h"
#include "uilist.h"
#include "uigrid.h"
#include "uiedit.h"
#include "UITeam.h"
#include "UIChat.h"
#include "UICombo.h"
#include "gameapp.h"
#include "character.h"
#include "netchat.h"
#include "packetcmd.h"
#include "uiheadsay.h"
#include "uitextparse.h"
#include "StringLib.h"
#include "UIBoatForm.h"
#include "UIGlobalVar.h"
#include "UIMiniMapForm.h"

using namespace std;


//const WORD					CCharMsg::m_wTotalChannelsCount;
//const DWORD					CCharMsg::m_dwChannelBufferSize;
CCharMsg::lstTextInfoType	CCharMsg::m_lstMsgLink;

CCharMsg::sChannelInfo		CCharMsg::m_sChannelInfo[m_wTotalChannelsCount]=
{
	{CHANNEL_NONE,		"",							0xFFFFFFFF,	"",		0,},				//CHANNEL_NONE
	{CHANNEL_ALL,		g_oLangRec.GetString(493),	0xFFFFFFFF,	"",		0,},				//CHANNEL_ALL
	{CHANNEL_SIGHT,		g_oLangRec.GetString(494),	0xFFFFFFFF,	"",		0,},				//CHANNEL_SIGHT
	{CHANNEL_PRIVATE,	g_oLangRec.GetString(495),	0xFFFFFFFF,	"@",	0,},				//CHANNEL_PRIVATE
	{CHANNEL_WORLD,		g_oLangRec.GetString(496),	0xFFFFFFFF,	"*",	0,},				//CHANNEL_WORLD
	{CHANNEL_TRADE,		g_oLangRec.GetString(497),	0xFF09bdba,	"^",	0,},				//CHANNEL_TRADE
	{CHANNEL_TEAM,		g_oLangRec.GetString(299),	0xFFFFFFFF,	"!",	0,},				//CHANNEL_TEAM
	{CHANNEL_GUILD,		g_oLangRec.GetString(468),	0xFFFFFFFF,	"%",	0,},				//CHANNEL_GUILD
	{CHANNEL_SYSTEM,	g_oLangRec.GetString(498),	0xFFFFFFFF,	"",		0,},				//CHANNEL_SYSTEM
	{CHANNEL_PUBLISH,	g_oLangRec.GetString(499),	0xFFFFFFFF,	"",		0,},				//CHANNEL_PUBLISH
	{CHANNEL_SIDE,		g_oLangRec.GetString(932),	0xFFFFFFFF,	"|",	0,},				//CHANNEL_SIDE
};

CCharMsg::lstInstanceType	CCharMsg::m_lstThisInstanceLink;

CCharMsg::CCharMsg()
{
	m_ecboShowChannels = CHANNEL_ALL | CHANNEL_SIGHT | CHANNEL_PRIVATE | CHANNEL_WORLD | CHANNEL_TRADE | CHANNEL_TEAM | CHANNEL_GUILD | CHANNEL_SYSTEM | CHANNEL_PUBLISH | CHANNEL_SIDE;
	m_bCurMsgAvailable=false;
	m_itCurrentMsgPos=m_lstMsgLink.end();
	m_lstThisInstanceLink.push_back(this);
}

CCharMsg::~CCharMsg()
{
	m_lstThisInstanceLink.remove(this);
}

WORD CCharMsg::GetChannelIndex(eChannel channel)
{
	DWORD dwChannel=(DWORD)channel;
	for (WORD i=1; i<=m_wTotalChannelsCount; i++)
	{
		if (dwChannel&1)
			return i;
		dwChannel=dwChannel>>1;
	}
	return 0;
}

void CCharMsg::SetChannelColor(eChannel channel, DWORD dwColor)
{
	m_sChannelInfo[GetChannelIndex(channel)].dwChannelColor=dwColor;
}

DWORD CCharMsg::GetChannelColor(eChannel channel)
{
	return m_sChannelInfo[GetChannelIndex(channel)].dwChannelColor;
}

string CCharMsg::GetChannelCommand(WORD wChannelIndex)
{
	return m_sChannelInfo[wChannelIndex].strChannelCommand;
}

string CCharMsg::GetChannelName(eChannel channel)
{
	return m_sChannelInfo[GetChannelIndex(channel)].strChannelName;
}

string CCharMsg::GetChannelShowName(eChannel channel)
{
	return "["+m_sChannelInfo[GetChannelIndex(channel)].strChannelName+"]";
}

CCharMsg::eChannel CCharMsg::GetChannelByIndex(WORD wChannelIndex)
{
	return m_sChannelInfo[wChannelIndex].enumChannel;
}

WORD CCharMsg::GetTotalChannelCount()
{
	return m_wTotalChannelsCount;
}

void CCharMsg::AddMsg(eChannel channel, string strWho, string strText, bool bSendTo,DWORD dwcolour)
{
	CTeam* pTeam = g_stUIChat.GetTeamMgr()->Find(enumTeamBlocked);
	for (int i = 0; i < pTeam->GetCount(); i++)
	{
		CMember* pMember = pTeam->GetMember(i);
		if (pMember->GetName() == strWho)
		{
			return;
		}
	}

	sTextInfo sText;
	if (channel == CHANNEL_PRIVATE)
	{
		if (bSendTo)
		{
			sText.strShowText=GetChannelShowName(channel)+"<To "+strWho+">:";
		}
		else
		{
			sText.strShowText=GetChannelShowName(channel)+"<From "+strWho+">:";
		}
	}
	else
	{
		sText.strShowText=GetChannelShowName(channel)+strWho;
		if (!strWho.empty())
		{
			sText.strShowText+=":";
		}
	}
	sText.strShowText+=strText;
	sText.eTextChannel=channel;
	sText.strText=strText;
	sText.bSendTo=bSendTo;
	sText.strWho=strWho;
	sText.dwColour = dwcolour;

	WORD wChannelIndex=GetChannelIndex(channel);
	if (m_sChannelInfo[wChannelIndex].dwTotalMsg >= m_dwChannelBufferSize)
	{
		//��������
		lstTextInfoType::iterator iterRemoveObject=m_sChannelInfo[wChannelIndex].itFirstMsgPos;
		while(m_sChannelInfo[wChannelIndex].itFirstMsgPos != m_lstMsgLink.end())
		{
			m_sChannelInfo[wChannelIndex].itFirstMsgPos++;
			sTextInfo sTempText=*(m_sChannelInfo[wChannelIndex].itFirstMsgPos);
			if (wChannelIndex == GetChannelIndex(sTempText.eTextChannel))
			{
				break;
			}
		}
		if (iterRemoveObject != m_lstMsgLink.end())
		{
			for (lstInstanceType::iterator it=m_lstThisInstanceLink.begin(); it!=m_lstThisInstanceLink.end(); it++)
			{
				if (iterRemoveObject == (*it)->m_itCurrentMsgPos)
				{
					(*it)->m_bCurMsgAvailable=false;
					if ((*it)->m_itCurrentMsgPos == m_lstMsgLink.begin())
					{
						(*it)->m_itCurrentMsgPos = m_lstMsgLink.end();
					}
					else
					{
						(*it)->m_itCurrentMsgPos--;
					}
				}
			}
			m_lstMsgLink.erase(iterRemoveObject);
		}
		m_sChannelInfo[wChannelIndex].dwTotalMsg--;
	}
	
	m_lstMsgLink.push_back(sText);

	if (m_sChannelInfo[wChannelIndex].dwTotalMsg++ == 0)
	{
		m_sChannelInfo[wChannelIndex].itFirstMsgPos = m_lstMsgLink.end()--;
	}
}

void CCharMsg::ClearAllMsg()
{
	m_lstMsgLink.clear();
	for (WORD i=0; i<m_wTotalChannelsCount; i++)
	{
		m_sChannelInfo[i].dwTotalMsg = 0;
		m_sChannelInfo[i].itFirstMsgPos = m_lstMsgLink.end();
	}
	for (lstInstanceType::iterator it=m_lstThisInstanceLink.begin(); it!=m_lstThisInstanceLink.end(); it++)
	{
		(*it)->m_bCurMsgAvailable=false;
		(*it)->m_itCurrentMsgPos = m_lstMsgLink.end();
	}
}

void CCharMsg::SetShowChannels(DWORD ecboShowChannels)
{
	m_ecboShowChannels=ecboShowChannels;
}

bool CCharMsg::ModifyShowChannel(eChannel eShowChannel, bool bAddOrRemove, bool bShowTips)
{
	bool bOrgShow=(m_ecboShowChannels&eShowChannel)?true:false;
	if (bOrgShow != bAddOrRemove)
	{
		if (bAddOrRemove)
		{
			SetShowChannels(m_ecboShowChannels|eShowChannel);
			if (bShowTips)
			{
				CCozeForm::GetInstance()->OnSystemMsg(GetChannelName(eShowChannel)+g_oLangRec.GetString(500));
			}
		}
		else
		{
			SetShowChannels(m_ecboShowChannels&~eShowChannel);
			if (bShowTips)
			{
				CCozeForm::GetInstance()->OnSystemMsg(GetChannelName(eShowChannel)+g_oLangRec.GetString(501));
			}
		}
		return true;
	}
	return false;
}

bool CCharMsg::IsShowChannel(eChannel channel)
{
	return (m_ecboShowChannels&channel)?true:false;
}

DWORD CCharMsg::GetShowChannels()
{
	return m_ecboShowChannels;
}

bool CCharMsg::MoveToFirstMsg()
{
	m_itCurrentMsgPos = m_lstMsgLink.begin();
	m_bCurMsgAvailable = !m_lstMsgLink.empty();
	if (!m_bCurMsgAvailable) return false;
	if ((*m_itCurrentMsgPos).eTextChannel&m_ecboShowChannels)
	{
		return true;
	}
	else
	{
		return MoveToNextMsg();
	}
}

bool CCharMsg::MoveToNextMsg()
{
	if (m_itCurrentMsgPos == m_lstMsgLink.end())
	{
		if (m_lstMsgLink.empty())
		{
			return false;
		}
		m_itCurrentMsgPos = m_lstMsgLink.begin();
		if ((*m_itCurrentMsgPos).eTextChannel&m_ecboShowChannels)
		{
			m_bCurMsgAvailable=true;
			return true;
		}
	}

	lstTextInfoType::iterator iterTemp=m_itCurrentMsgPos;
	while(++iterTemp != m_lstMsgLink.end())
	{
		sTextInfo sTempText=*(iterTemp);
		if (sTempText.eTextChannel&m_ecboShowChannels)
		{
			m_bCurMsgAvailable=true;
			m_itCurrentMsgPos = iterTemp;
			return true;
		}
	}
	return false;
}

CCharMsg::sTextInfo CCharMsg::GetMsgInfo()
{
	sTextInfo sText;
	sText.eTextChannel=CHANNEL_ALL;
	sText.strWho=g_oLangRec.GetString(2);
	sText.strText=g_oLangRec.GetString(502);
	sText.strShowText=g_oLangRec.GetString(502);
	sText.bSendTo=false;
	if (m_bCurMsgAvailable && m_itCurrentMsgPos != m_lstMsgLink.end())
	{
		sText=*m_itCurrentMsgPos;
	}
	return sText;
}

CCardCase::CCardCase(WORD wMaxLimit)
{
	m_wMaxLimit=wMaxLimit;
	MoveToFirstCard();
}

CCardCase::~CCardCase()
{
}

void CCardCase::AddCard(string str)
{
	if (str.empty()) return;

	for (lstCardType::iterator it=m_lstCardData.begin();it!=m_lstCardData.end();it++)
	{
		if ((*it)==str)
		{
			m_lstCardData.erase(it);
			m_lstCardData.push_back(str);
			return;
		}
	}
	
	m_lstCardData.push_back(str);
	if ((WORD)m_lstCardData.size()>m_wMaxLimit)
	{
		m_lstCardData.erase(m_lstCardData.begin());
	}
}

bool CCardCase::RemoveCard(string str)
{
	for (lstCardType::iterator it=m_lstCardData.begin();it!=m_lstCardData.end();it++)
	{
		if ((*it)==str)
		{
			m_lstCardData.erase(it);
			return true;
		}
	}
	return false;
}

void CCardCase::ClearAll()
{
	m_lstCardData.clear();
	MoveToFirstCard();
}

int	CCardCase::GetTotalCount()
{
	return (int)m_lstCardData.size();
}

bool CCardCase::MoveToFirstCard()
{
	m_itCurrentCardPos=m_lstCardData.begin();
	if (GetTotalCount()==0)
	{
		return false;
	}
	return true;
}

bool CCardCase::MoveToNextCard()
{
	lstCardType::iterator iterTemp=m_itCurrentCardPos;
	if (++iterTemp == m_lstCardData.end())
	{
		return false;
	}
	m_itCurrentCardPos=iterTemp;
	return true;
}

bool CCardCase::MoveToLastCard()
{
	m_itCurrentCardPos=m_lstCardData.end();
	if (GetTotalCount()==0)
	{
		return false;
	}
	--m_itCurrentCardPos;
	return true;
}

bool CCardCase::MoveToPrevCard()
{
	if (m_itCurrentCardPos==m_lstCardData.begin())
	{
		return false;
	}
	--m_itCurrentCardPos;
	return true;
}

string CCardCase::GetCardInfo()
{
	if (GetTotalCount()==0)
	{
		return "";
	}

	return *m_itCurrentCardPos;
}



using namespace GUI;


CChannelSwitchForm::CChannelSwitchForm()
{
}

CChannelSwitchForm::~CChannelSwitchForm()
{
}

extern CChannelSwitchForm g_stUIChannelSwitch;
CChannelSwitchForm* CChannelSwitchForm::GetInstance()
{
	//static CChannelSwitchForm s_ChannelSwitchForm;
	return (&g_stUIChannelSwitch);
}

void CChannelSwitchForm::SwitchCheck()
{
	CCozeForm *pCozeForm=CCozeForm::GetInstance();
	if (m_frmChannelSwitch->GetIsShow())
	{
		pCozeForm->m_chkChannelSwitch->SetIsChecked(false);
		m_frmChannelSwitch->SetIsShow(false);
	}
	else
	{
		CChannelSwitchForm::GetInstance()->m_chkPrivate->SetIsChecked(pCozeForm->m_cMainMsg.IsShowChannel(CCharMsg::CHANNEL_PRIVATE));
		CChannelSwitchForm::GetInstance()->m_chkSight->SetIsChecked(pCozeForm->m_cMainMsg.IsShowChannel(CCharMsg::CHANNEL_SIGHT));
		CChannelSwitchForm::GetInstance()->m_chkSystem->SetIsChecked(pCozeForm->m_lstSystemPage->GetIsShow());
		CChannelSwitchForm::GetInstance()->m_chkTeam->SetIsChecked(pCozeForm->m_cMainMsg.IsShowChannel(CCharMsg::CHANNEL_TEAM));
		CChannelSwitchForm::GetInstance()->m_chkGuild->SetIsChecked(pCozeForm->m_cMainMsg.IsShowChannel(CCharMsg::CHANNEL_GUILD));
		CChannelSwitchForm::GetInstance()->m_chkWorld->SetIsChecked(pCozeForm->m_cMainMsg.IsShowChannel(CCharMsg::CHANNEL_WORLD));
		CChannelSwitchForm::GetInstance()->m_chkTrade->SetIsChecked(pCozeForm->m_cMainMsg.IsShowChannel(CCharMsg::CHANNEL_TRADE));
		pCozeForm->m_chkChannelSwitch->SetIsChecked(true);
		m_frmChannelSwitch->SetIsShow(true);
	}
}

bool CChannelSwitchForm::Init()
{
	FORM_LOADING_CHECK(m_frmChannelSwitch,"main.clu","frmForbid");
	m_frmChannelSwitch->evtLost=EventLostFocus;
	FORM_CONTROL_LOADING_CHECK(m_chkPrivate,m_frmChannelSwitch,CCheckBox,"main.clu","chkPersonal");
	FORM_CONTROL_LOADING_CHECK(m_chkSight,m_frmChannelSwitch,CCheckBox,"main.clu","chkNear");
	FORM_CONTROL_LOADING_CHECK(m_chkSystem,m_frmChannelSwitch,CCheckBox,"main.clu","chkSystem");
	FORM_CONTROL_LOADING_CHECK(m_chkTeam,m_frmChannelSwitch,CCheckBox,"main.clu","chkTeam");
	FORM_CONTROL_LOADING_CHECK(m_chkGuild,m_frmChannelSwitch,CCheckBox,"main.clu","chkGuild");
	FORM_CONTROL_LOADING_CHECK(m_chkWorld,m_frmChannelSwitch,CCheckBox,"main.clu","chkWorld");
	FORM_CONTROL_LOADING_CHECK(m_chkTrade,m_frmChannelSwitch,CCheckBox,"main.clu","chkTrade");
	
	m_chkPrivate->evtCheckChange=EventPrivateCheckChange;
	m_chkSight->evtCheckChange=EventSightCheckChange;
	m_chkSystem->evtCheckChange=EventSystemCheckChange;
	m_chkTeam->evtCheckChange=EventTeamCheckChange;
	m_chkGuild->evtCheckChange=EventGuildCheckChange;
	m_chkWorld->evtCheckChange=EventWorldCheckChange;
	m_chkTrade->evtCheckChange=EventTradeCheckChange;

	return true;
}

void CChannelSwitchForm::EventLostFocus(CGuiData *pSender)
{
	CCozeForm *pCozeForm=CCozeForm::GetInstance();
	pCozeForm->m_chkChannelSwitch->SetIsChecked(false);
	pCozeForm->m_chkChannelSwitch->MouseRun(pCozeForm->m_chkChannelSwitch->GetX()-1,pCozeForm->m_chkChannelSwitch->GetY()-1,0);
	CChannelSwitchForm::GetInstance()->m_frmChannelSwitch->SetIsShow(false);
}

void CChannelSwitchForm::EventPrivateCheckChange(CGuiData *pSender)
{
	CCozeForm *pCozeForm=CCozeForm::GetInstance();
	pCozeForm->m_cMainMsg.ModifyShowChannel(CCharMsg::CHANNEL_PRIVATE, CChannelSwitchForm::GetInstance()->m_chkPrivate->GetIsChecked());
	pCozeForm->ResetPages();
}

void CChannelSwitchForm::EventSightCheckChange(CGuiData *pSender)
{
	CCozeForm *pCozeForm=CCozeForm::GetInstance();
	pCozeForm->m_cMainMsg.ModifyShowChannel(CCharMsg::CHANNEL_SIGHT, CChannelSwitchForm::GetInstance()->m_chkSight->GetIsChecked());
	pCozeForm->ResetPages();
}

void CChannelSwitchForm::EventSystemCheckChange(CGuiData *pSender)
{
	CCozeForm *pCozeForm=CCozeForm::GetInstance();
	bool bCheck=CChannelSwitchForm::GetInstance()->m_chkSystem->GetIsChecked();
	pCozeForm->m_lstSystemPage->SetIsShow(bCheck);
	pCozeForm->m_drgSystemPage->SetIsShow(bCheck);
	if (bCheck)
	{
		pCozeForm->OnSystemMsg(g_oLangRec.GetString(503));
	}
	else
	{
		pCozeForm->OnSystemMsg(g_oLangRec.GetString(504));
	}
	pCozeForm->ResetPages();
}

void CChannelSwitchForm::EventTeamCheckChange(CGuiData *pSender)
{
	CCozeForm *pCozeForm=CCozeForm::GetInstance();
	pCozeForm->m_cMainMsg.ModifyShowChannel(CCharMsg::CHANNEL_TEAM, CChannelSwitchForm::GetInstance()->m_chkTeam->GetIsChecked());
	pCozeForm->ResetPages();
}

void CChannelSwitchForm::EventGuildCheckChange(CGuiData *pSender)
{
	CCozeForm *pCozeForm=CCozeForm::GetInstance();
	pCozeForm->m_cMainMsg.ModifyShowChannel(CCharMsg::CHANNEL_GUILD, CChannelSwitchForm::GetInstance()->m_chkGuild->GetIsChecked());
	pCozeForm->ResetPages();
}

void CChannelSwitchForm::EventWorldCheckChange(CGuiData *pSender)
{
	CCozeForm *pCozeForm=CCozeForm::GetInstance();
	pCozeForm->m_cMainMsg.ModifyShowChannel(CCharMsg::CHANNEL_WORLD, CChannelSwitchForm::GetInstance()->m_chkWorld->GetIsChecked());
	pCozeForm->ResetPages();
}

void CChannelSwitchForm::EventTradeCheckChange(CGuiData *pSender)
{
	CCozeForm *pCozeForm=CCozeForm::GetInstance();
	pCozeForm->m_cMainMsg.ModifyShowChannel(CCharMsg::CHANNEL_TRADE, CChannelSwitchForm::GetInstance()->m_chkTrade->GetIsChecked());
	pCozeForm->ResetPages();
}

const int	CCozeForm::m_nMainPageMinHeight=70;
const int	CCozeForm::m_nMainPageMaxHeight=280;
const int	CCozeForm::m_nSystemPageMinHeight=55;
const int	CCozeForm::m_nSystemPageMaxHeight=240;

CCozeForm::CCozeForm() : m_cCallingCard(10), m_cSendMsgCard(10), m_bSendMsgCardSwitch(false)
{
	m_eCurSelChannel=CCharMsg::CHANNEL_SIGHT;
}

CCozeForm::~CCozeForm()
{
}


extern CCozeForm g_stUICozeForm;
CCozeForm* CCozeForm::GetInstance()
{
	//static CCozeForm s_CozeForm;
	return &g_stUICozeForm;
}

void CCozeForm::OnSightMsg(string strName, string strMsg,DWORD dwColour)
{
	CCharMsg::AddMsg(CCharMsg::CHANNEL_SIGHT, strName, strMsg,false,dwColour);	
	UpdatePages();
}

void CCozeForm::OnSightMsg(CCharacter* pChar, string strMsg,DWORD dwColour)
{
	if (!pChar) return;

	std::string sender = pChar->getHumanName();
	if (g_stUIChat.GetTeamMgr()->Find(enumTeamBlocked)->FindByName(sender.c_str()))
		return;
	
	char buf[10];
	//for (int i=0; i<=9; i++)
	for (int i=0; i<=50; i++)//Modify by sunny.sun 20080902
	{
		sprintf(buf, "***%d", i);
		if (strMsg==buf)
		{
			pChar->GetHeadSay()->SetFaceID(i);
			return;
		}
	}
	
	//Add by sunny.sun 20080902
	//Begin
	const string pcnsenderface = "#21";//�������ղ������ַ���
	const string preplaceface = "#99";//�滻���͵��ַ���
	ReplaceSpecialFace(strMsg,pcnsenderface,preplaceface);//�滻�ַ���
	//End
	CTextFilter::Filter(CTextFilter::DIALOG_TABLE, strMsg);
	OnSightMsg(pChar->IsPlayer() ? pChar->getHumanName() : pChar->getName().c_str(), strMsg,0xFF56bdfc);	//local chat color default //mothannakh player name color

	string str = string(pChar->IsPlayer() ? pChar->getHumanName() : pChar->getName().c_str())+": "+strMsg;// 需要修�? ning.yan 2008-11-07
	CItemEx *item = new CItemEx(str.c_str(), CCharMsg::GetChannelColor(CCharMsg::CHANNEL_SIGHT));
	if (strlen(str.c_str()) > 32)  //�������?32���ַ�����������?
	{
		item->SetIsMultiLine(true);
		//item->ProcessString((int)strlen(pChar->getHumanName())+1);
		item->ProcessString((int)strlen(pChar->IsPlayer() ? pChar->getHumanName() : pChar->getName().c_str())+1);
	}
	if (str.find("#") != std::string::npos)
	{
		item->SetIsParseText(true);	
	}
	pChar->GetHeadSay()->AddItem(item);
}

void CCozeForm::OnPrivateMsg(string strFromName, string strToName, string strMsg,DWORD dwColour)
{
	if( !CGameScene::GetMainCha() ) return;

	CTextFilter::Filter(CTextFilter::DIALOG_TABLE, strMsg);
	string strHumanName=CGameScene::GetMainCha()->getHumanName();
	if (strHumanName==strFromName)
	{
		if (strMsg!="{x}*")
		{
			CCharMsg::AddMsg(CCharMsg::CHANNEL_PRIVATE, strToName, strMsg, true,dwColour);
			m_cCallingCard.AddCard(strToName);
		}
	}
	else
	{
		if (strMsg=="{x}*")
		{
			OnSystemMsg(g_oLangRec.GetString(505)+strFromName+g_oLangRec.GetString(506));
		}
		else
		{
			CCharMsg::AddMsg(CCharMsg::CHANNEL_PRIVATE, strFromName, strMsg, false,dwColour);
			if (m_cMainMsg.IsShowChannel(CCharMsg::CHANNEL_PRIVATE))
			{
				m_cCallingCard.AddCard(strFromName);
			}
			else
			{
				string strContent="{x}*";
				CS_Say2You(strFromName.c_str(), strContent.c_str());
			}
		}
	}
	UpdatePages();
}

void CCozeForm::OnWorldMsg(string strName, string strMsg,DWORD dwColour)
{
	CTextFilter::Filter(CTextFilter::DIALOG_TABLE, strMsg);
	CCharMsg::AddMsg(CCharMsg::CHANNEL_WORLD, strName, strMsg,false,0xFFFFFFFF); //dwColour
	UpdatePages();
}

void CCozeForm::OnTradeMsg(string strName, string strMsg,DWORD dwColour)
{
	CTextFilter::Filter(CTextFilter::DIALOG_TABLE, strMsg);
	CCharMsg::AddMsg(CCharMsg::CHANNEL_TRADE, strName, strMsg,false,0xFF09bdba); //dwColour
	UpdatePages();
}

void CCozeForm::OnTeamMsg(string strName, string strMsg,DWORD dwColour)
{
	CTextFilter::Filter(CTextFilter::DIALOG_TABLE, strMsg);
	CCharMsg::AddMsg(CCharMsg::CHANNEL_TEAM, strName, strMsg,false,0xFF33ff00);//dwColour
	UpdatePages();
}

void CCozeForm::OnTeamMsg(DWORD dwCharID, string strMsg,DWORD dwColour)
{
	if( !CGameScene::GetMainCha() ) return;

	string strName;
	if ( dwCharID!=CGameScene::GetMainCha()->getHumanID() )
	{
		CTeam *pTeam=g_stUIChat.GetTeamMgr()->Find(enumTeamGroup);
		if (!pTeam) return;
		CMember *pMember=pTeam->Find(dwCharID);
		if (!pMember) return;
		strName=pMember->GetName();
	}
	else
	{
		strName=CGameScene::GetMainCha()->getHumanName();
	}
	OnTeamMsg(strName, strMsg,0xFF33ff00);//dwColour
}

void CCozeForm::OnGuildMsg(string strName, string strMsg,DWORD dwColour)
{
	CTextFilter::Filter(CTextFilter::DIALOG_TABLE, strMsg);
	CCharMsg::AddMsg(CCharMsg::CHANNEL_GUILD, strName, strMsg,false,0xFF0eb5e8);//dwColour
	UpdatePages();
}

void CCozeForm::OnSystemMsg(string strMsg)
{
	if (GuildSysInfo)
		CCharMsg::SetChannelColor(CCharMsg::CHANNEL_SYSTEM, 0xFF00FFFF);
	else
		CCharMsg::SetChannelColor(CCharMsg::CHANNEL_SYSTEM, 0xFFFFFF00); //92bbf0 0xFFFFFF00	//mothannakh

	CCharMsg::AddMsg(CCharMsg::CHANNEL_SYSTEM, "", strMsg);
	UpdatePages();
}

void CCozeForm::OnSideMsg(string strName, string strMsg,DWORD dwColour)
{
	CCharMsg::AddMsg(CCharMsg::CHANNEL_SIDE, strName, strMsg,false,dwColour);
	UpdatePages();
}

void CCozeForm::OnPublishMsg(string strName, string strMsg)
{
	string str;
	str=g_oLangRec.GetString(507)+string(strMsg);
	g_pGameApp->ShowNotify(str.c_str(), CCharMsg::GetChannelColor(CCharMsg::CHANNEL_PUBLISH));
}
//Add by sunny.sun20080804
void CCozeForm::OnPublishMsg1(string strMsg,int setnum, DWORD color)
{
	string str;
	str=string(strMsg);
	g_pGameApp->ShowNotify1(str.c_str(),setnum, color);
}
//End
void CCozeForm::OnPrivateNameSet(string strName)
{
	if (strName.empty()) return;

	string strCurCmd=CCharMsg::GetChannelCommand(CCharMsg::GetChannelIndex(CCharMsg::CHANNEL_PRIVATE))+strName+" ";
	m_edtMsg->SetCaption(strCurCmd.c_str());
	m_edtMsg->SetActive(m_edtMsg);

	m_cCallingCard.AddCard(strName);
}

void CCozeForm::OnResetAll()
{
	m_chkChannelSwitch->SetIsChecked(false);
	m_lstCallingCard->SetIsShow(false);
	m_grdFacePanel->SetIsShow(false);
	m_grdBrowPanel->SetIsShow(false);
	m_grdActionPanel->SetIsShow(false);

	m_eCurSelChannel=CCharMsg::CHANNEL_SIGHT;
	m_cmbChannel->GetList()->GetItems()->Select(0);
	m_edtMsg->SetCaption("");
	m_lstMainPage->GetItems()->Clear();
	m_lstSystemPage->GetItems()->Clear();
	m_cMainMsg.ClearAllMsg();
	m_cSystemMsg.ClearAllMsg();
	m_cCallingCard.ClearAll();
	m_cSendMsgCard.ClearAll();
	m_bSendMsgCardSwitch=false;
}

bool CCozeForm::IsMouseOnList(int x, int y)
{
	CCozeForm *pThis=CCozeForm::GetInstance();
	if (pThis->m_lstSystemPage->GetScroll()->IsNormal())
	{
		GetRender().ScreenConvert(x, y);
		if(pThis->m_lstSystemPage->GetScroll()->InRect(x, y)) return false;
	}
	if (pThis->m_lstMainPage->GetScroll()->IsNormal())
	{
		GetRender().ScreenConvert(x, y);
		if(pThis->m_lstMainPage->GetScroll()->InRect(x, y)) return false;
	}
	return true;
}

void CCozeForm::AddToEdit(string strData)
{
	CEdit* pEdit=dynamic_cast<CEdit*>(CCompent::GetActive());
	if (!pEdit)
	{
		if (!m_edtMsg) return;
		pEdit = m_edtMsg;
	}

	string strMsg=pEdit->GetCaption();
	strMsg+=strData;
	if ((int)strlen(strMsg.c_str()) > pEdit->GetMaxNum()) return;
	pEdit->SetCaption(strMsg.c_str());
}
//Add by sunny.sun 20080902
//Begin
//��strMsg�е�strFace�滻ΪstrReplace
void CCozeForm::ReplaceSpecialFace( string &strMsg, const string & strReplace, const string & strFace )
{
	auto nPos=strMsg.find(strFace);

	while (nPos!= std::string::npos)
	{
		strMsg.replace(nPos,strFace.length(),strReplace,0,strReplace.length());
		nPos=strMsg.find(strFace,nPos+(strReplace.length()));
	}
}
//End

bool CCozeForm::Init()
{
	m_frmMainChat = _FindForm("frmMain800");
	if( !m_frmMainChat ) return false;
	CFormMgr::s_Mgr.AddKeyDownEvent( EventGlobalKeyDownHandle );

	FORM_LOADING_CHECK(m_frmPublish, "main.clu", "frmGM");
	m_frmPublish->evtBeforeShow=EventPublishShowForm;

	FORM_CONTROL_LOADING_CHECK(m_edtPublishMsg, m_frmPublish, CEdit, "coze.clu", "edtGMSay");
	m_edtPublishMsg->SetIsWrap(true);
	m_edtPublishMsg->evtEnter=EventPublishSendMsg;

	FORM_CONTROL_LOADING_CHECK(m_edtMsg,m_frmMainChat,CEdit,"main.clu","edtSay");
	m_edtMsg->SetIsWrap(true);
	m_edtMsg->SetIsParseText(true);
	m_edtMsg->evtEnter=EventSendMsg;
	m_edtMsg->evtKeyDown=EventEditMsg;

	FORM_CONTROL_LOADING_CHECK(m_cmbChannel,m_frmMainChat,CCombo,"main.clu","cboChannel");
	m_cmbChannel->evtSelectChange=EventSendChannelChange;
	m_cmbChannel->evtMouseClick=EventSendChannelSwitchClick;
	m_cmbChannel->GetList()->GetItems()->Select(0); 

	FORM_CONTROL_LOADING_CHECK(m_lstMainPage,m_frmMainChat,CList,"main.clu","lstOnSay");
	m_lstMainPage->SetIsChangeColor(false);
	m_lstMainPage->SetMouseAction( enumMA_Drill );
	m_lstMainPage->SetRowHeight(16);
	m_lstMainPage->evtListMouseDB=EventMainListKeyDown;

	FORM_CONTROL_LOADING_CHECK(m_lstSystemPage,m_frmMainChat,CList,"main.clu","lstOnSaySystem");
	m_lstSystemPage->SetIsChangeColor(false);
	m_lstSystemPage->SetMouseAction( enumMA_Drill );
	m_lstSystemPage->SetRowHeight(16);
	m_cSystemMsg.SetShowChannels(CCharMsg::CHANNEL_SYSTEM);
	
	FORM_CONTROL_LOADING_CHECK(m_drgMainPage,m_frmMainChat,CDragTitle,"main.clu","drpTitle");
	m_drgMainPage->SetIsShowDrag(false);
	m_drgMainPage->GetDrag()->SetYare(0);
	m_drgMainPage->GetDrag()->SetDragInCursor(CCursor::stVertical);
	m_drgMainPage->GetDrag()->evtMouseDragBegin = EventMainPageDragBegin;
	m_drgMainPage->GetDrag()->evtMouseDragMove = EventMainPageDragging;
	m_drgMainPage->GetDrag()->evtMouseDragEnd = EventMainPageDragEnd;
	
	FORM_CONTROL_LOADING_CHECK(m_drgSystemPage,m_frmMainChat,CDragTitle,"main.clu","drpTitleSystem");
	m_drgSystemPage->SetIsShowDrag(false);
	m_drgSystemPage->GetDrag()->SetYare(0);
	m_drgSystemPage->GetDrag()->SetDragInCursor(CCursor::stVertical);
	m_drgSystemPage->GetDrag()->evtMouseDragBegin = EventSystemPageDragBegin;
	m_drgSystemPage->GetDrag()->evtMouseDragMove = EventSystemPageDragging;
	m_drgSystemPage->GetDrag()->evtMouseDragEnd = EventSystemPageDragEnd;	
	
	FORM_CONTROL_LOADING_CHECK(m_chkChannelSwitch,m_frmMainChat,CCheckBox,"main.clu","chkChatOn");
	m_chkChannelSwitch->SetIsChecked(false);
	m_chkChannelSwitch->evtCheckChange=EventChannelSwitchCheck;

	FORM_CONTROL_LOADING_CHECK(m_btnCallingCardSwitch,m_frmMainChat,CTextButton,"main.clu","btnCard");
	m_btnCallingCardSwitch->evtMouseClick=EventCallingCardSwitchClick;		//switch

	FORM_CONTROL_LOADING_CHECK(m_lstCallingCard,m_frmMainChat,CList,"main.clu","lstCard");
	//m_lstCallingCard->evtLost=EventCallingCardLostFocus; -arcol ��Ч�¼�
	m_lstCallingCard->evtSelectChange=EventCardSelected;
	m_lstCallingCard->SetIsChangeColor(false);

	FORM_CONTROL_LOADING_CHECK(m_btnFaceSwitch,m_frmMainChat,CTextButton,"main.clu","btnChatFace");
	m_btnFaceSwitch->evtMouseClick=EventFacePanelSwitchClick;		//switch

	FORM_CONTROL_LOADING_CHECK(m_grdFacePanel,m_frmMainChat,CGrid,"main.clu","grdFace");
	//m_grdFacePanel->evtLost=EventFacePanelLostFocus;
	m_grdFacePanel->evtSelectChange=EventFaceSelected;

	FORM_CONTROL_LOADING_CHECK(m_btnBrowSwitch,m_frmMainChat,CTextButton,"main.clu","btnBrow");
	m_btnBrowSwitch->evtMouseClick=EventBrowPanelSwitchClick;		//switch

	FORM_CONTROL_LOADING_CHECK(m_grdBrowPanel,m_frmMainChat,CGrid,"main.clu","grdHeart");
	m_grdBrowPanel->evtSelectChange=EventBrowSelected;

	FORM_CONTROL_LOADING_CHECK(m_btnActionSwitch,m_frmMainChat,CTextButton,"main.clu","btnAction");
	m_btnActionSwitch->evtMouseClick=EventActionPanelSwitchClick;		//switch

	FORM_CONTROL_LOADING_CHECK(m_grdActionPanel,m_frmMainChat,CGrid,"main.clu","grdAction");
	m_grdActionPanel->evtSelectChange=EventActionSelected;


	m_frmMainChat->SetIsShow(true);
	CChannelSwitchForm::GetInstance();

	m_cMainMsg.ModifyShowChannel(CCharMsg::CHANNEL_SYSTEM, false, false);
	ResetPages();
	return true;
}

CCharMsg::eChannel CCozeForm::GetCmdFromMsg(string strMsg)
{
	string strCmd;
	strCmd.push_back(strMsg[0]);
	for (WORD i=0; i<CCharMsg::GetTotalChannelCount(); i++)
	{
		if (strCmd==CCharMsg::GetChannelCommand(i))
		{
			return CCharMsg::GetChannelByIndex(i);
		}
	}
	return CCharMsg::CHANNEL_NONE;
}

void CCozeForm::SendMsg()
{
	CCharacter* pChar = CGameScene::GetMainCha();
	if( !pChar ) return;

	string strMsg = m_edtMsg->GetCaption();

	if (strMsg.empty() || strMsg.length() == count( strMsg.begin( ), strMsg.end( ), ' '))
	{
		m_lstSystemPage->OnKeyDown(VK_END);
		m_lstMainPage->OnKeyDown(VK_END);
		return;
	}

	if(g_stUIMap.IsPKSilver() && pChar->getGMLv() <= 0)
	{
		g_pGameApp->SysInfo(g_oLangRec.GetString(901)); // �Ҷ��������н�ֹ������Ϣ
		return;
	}

	string strCurCmd="";
	CCharMsg::eChannel enumChannel=GetCmdFromMsg(strMsg);
	if (enumChannel == CCharMsg::CHANNEL_PRIVATE)
	{
		auto p = strMsg.find(" ") ;
		if (p != std::string::npos)
		{	
			string strPersonName=strMsg.substr(1, p-1) ;//˽�Ķ���
			string strContent=strMsg.substr(p+1);
			if (!strContent.empty())
			{
				//CTextFilter::Filter(CTextFilter::DIALOG_TABLE, strContent);
				if (!strPersonName.empty() && strPersonName.length()<=37)
				{
					m_cSendMsgCard.AddCard(strContent);
					m_bSendMsgCardSwitch=false;
					if (m_cMainMsg.ModifyShowChannel(CCharMsg::CHANNEL_PRIVATE))
					{
						ResetPages();
					}
					strCurCmd=CCharMsg::GetChannelCommand(CCharMsg::GetChannelIndex(CCharMsg::CHANNEL_PRIVATE))+strPersonName+" ";

					CCharacter * pMain = CGameScene::GetMainCha();
					if(pMain && (pMain->IsBoat() || pMain->getGameAttr()->get(ATTR_LV) >= 9))
					{
						CS_Say2You(strPersonName.c_str(), strContent.c_str());
					}
					else
					{
						// ��ɫ�ȼ�9�����½�ֹ��������˽��
						g_pGameApp->SysInfo(g_oLangRec.GetString(868));
					}

					m_edtMsg->SetCaption(strCurCmd.c_str());
				}
			}
		}
		return;
	}
	else if (enumChannel == CCharMsg::CHANNEL_NONE)
	{
		enumChannel=m_eCurSelChannel;
	}
	else
	{
		strMsg.erase(0, 1);
		if (strMsg.empty()) return;
	}
	m_cSendMsgCard.AddCard(strMsg);
	m_bSendMsgCardSwitch=false;

	switch(enumChannel)
	{
	case CCharMsg::CHANNEL_SIGHT:
		{
			static string preStr="";
			static DWORD preTime=0;
			if (preStr==strMsg && GetTickCount()-preTime<1000)
			{
				OnSystemMsg(g_oLangRec.GetString(508));
			}
			else
			{
				if (m_cMainMsg.ModifyShowChannel(CCharMsg::CHANNEL_SIGHT))
				{
					ResetPages();
				}
				CTextFilter::Filter(CTextFilter::DIALOG_TABLE, strMsg);
				//Add by sunny.sun 20080902
				//Begin
				const string pcnsenderface = "#21";
				const string preplaceface = "#99";
				ReplaceSpecialFace(strMsg,preplaceface,pcnsenderface);
				//End
				CS_Say(strMsg.c_str());
				preStr=strMsg;
				preTime=GetTickCount();
			}
			break;
		}
	case CCharMsg::CHANNEL_WORLD:
		{
			CCharacter* pChar=g_stUIBoat.GetHuman();
			if (!pChar || pChar->getGameAttr()->get(ATTR_LV)<10)
			{
				OnSystemMsg(g_oLangRec.GetString(509));
			}
			else	// modify by Philip.Wu  2006-06-09  ���� 10 ������ʹ������Ƶ��
			{
				if (pChar->getGMLv() == 99 || GetTickCount()-g_pGameApp->GetLoginTime()>60000)
				{
					if (m_cMainMsg.ModifyShowChannel(CCharMsg::CHANNEL_WORLD))
					{
						ResetPages();
					}
					CTextFilter::Filter(CTextFilter::DIALOG_TABLE, strMsg);
					CS_Say2All(strMsg.c_str());
				}
				else
				{
					OnSystemMsg(g_oLangRec.GetString(510));
				}
			}
			break;
		}
	case CCharMsg::CHANNEL_TRADE:
		{
			CCharacter* pChar=g_stUIBoat.GetHuman();
			if (!pChar || pChar->getGameAttr()->get(ATTR_LV)<10)
			{
				OnSystemMsg(g_oLangRec.GetString(511));
			}
			else	// modify by Philip.Wu  2006-06-09  ���� 10 ������ʹ��ó��Ƶ��
			{
				if (pChar->getGMLv() == 99 ||  GetTickCount()-g_pGameApp->GetLoginTime()>60000)
				{
					if (m_cMainMsg.ModifyShowChannel(CCharMsg::CHANNEL_TRADE))
					{
						ResetPages();
					}
					CTextFilter::Filter(CTextFilter::DIALOG_TABLE, strMsg);
					CS_Say2Trade(strMsg.c_str());
				}
				else
				{
					OnSystemMsg(g_oLangRec.GetString(512));
				}
			}
			break;
		}
	case CCharMsg::CHANNEL_TEAM:
		{
			CTextFilter::Filter(CTextFilter::DIALOG_TABLE, strMsg);
			if (!strMsg.empty())
			{
				CTeam *pTeam=g_stUIChat.GetTeamMgr()->Find(enumTeamGroup);
				if (!pTeam || pTeam->GetCount()==0)
				{
					OnSystemMsg(g_oLangRec.GetString(513));
				}
				else
				{
					if (m_cMainMsg.ModifyShowChannel(CCharMsg::CHANNEL_TEAM))
					{
						ResetPages();
					}
					CS_Say2Team(strMsg.c_str());
				}
			}
			break;
		}
	case CCharMsg::CHANNEL_GUILD:
		{
			if (m_cMainMsg.ModifyShowChannel(CCharMsg::CHANNEL_GUILD))
			{
				ResetPages();
			}
			CTextFilter::Filter(CTextFilter::DIALOG_TABLE, strMsg);
			CS_Say2Guild(strMsg.c_str());
			break;
		}
	case CCharMsg::CHANNEL_SYSTEM:
		{
			break;
		}
	case CCharMsg::CHANNEL_PUBLISH:
		{
			break;
		}
	case CCharMsg::CHANNEL_SIDE:
		{
			CS_Say2Camp(strMsg.c_str());
			break;
		}
	default:
		{
			OnSystemMsg(g_oLangRec.GetString(514));
			return;
		}
	}
	strCurCmd=CCharMsg::GetChannelCommand(CCharMsg::GetChannelIndex(enumChannel));
	m_edtMsg->SetCaption(strCurCmd.c_str());
}

void CCozeForm::UpdatePages()
{
	CList	*page = m_lstMainPage;
	
	bool bIndentFlag;
	CListItems* pListItems;
	while (m_cMainMsg.MoveToNextMsg())
	{
		
		
		CCharMsg::sTextInfo msg=m_cMainMsg.GetMsgInfo();
		string strMsg=msg.strShowText;
		bIndentFlag=false;
		
		bool highlight = false;
		auto nPos = strMsg.find("]");
		auto nameEnd = strMsg.find(":");
		if(nPos != std::string::npos && msg.strWho != "" && nameEnd != std::string::npos){
			CCharacter* main =  g_stUIBoat.GetHuman();
			auto namePos = strMsg.find(main->getName());
			if(namePos!= std::string::npos && namePos>nameEnd){
				highlight= true;
			}
		}
					
					
		while (!strMsg.empty())
		{
			if (bIndentFlag)
			{
				strMsg="      "+strMsg;
			}
			string strShowText=CutFaceText(strMsg,44);
			
			CItemEx* pItem=new CItemEx(strShowText.c_str(), CCharMsg::GetChannelColor(msg.eTextChannel));
			pItem->SetAlignment(eAlignCenter);
			pItem->SetHeight(18);
			pItem->SetIsParseText(true);
			pItem->SetItemName(msg.strWho.c_str());
			if (!bIndentFlag){
				auto nPos = strShowText.find("]");
				auto nameEnd = strShowText.find(":");
				if(nPos!= std::string::npos){
					pItem->AddHighlightText(0, nPos+1,0xFFFFFF00);	//b0e0eb 0xFFFFFF00	mothannakh
					if(msg.strWho != "" && nameEnd != std::string::npos){
						if((int)strShowText.find("[GM]")==nPos+1){
							pItem->AddHighlightText(nPos+1,4,0xFFa89525);	//1ac8f0 0xFFa89525
						}
						
						pItem->AddHighlightText(nPos+1, nameEnd-nPos,msg.dwColour);
					}
				}
			}
			if(highlight){
				pItem->SetHighlighted(0x80607CA0);
			}
			
			page->NewItem()->SetBegin(pItem);
			bIndentFlag=true;
		}
	};
	pListItems=page->GetItems();
	for (int i=pListItems->GetCount()-200; i>0; i--)
	{
		pListItems->Del(pListItems->GetItem(0));
	}

	static bool IsPressScroll = false;
	IsPressScroll = false;
	if( g_pGameApp->IsMouseButtonPress( 0 ) )
	{
		if( page->GetScroll()->IsNormal() )
		{
			int x = g_pGameApp->GetMouseX();
			int y = g_pGameApp->GetMouseY();
			GetRender().ScreenConvert(x, y);
			if( page->GetScroll()->InRect(x, y) ) 
			{
				IsPressScroll = true;
			}
		}
	}
	else
	{
		if( page->GetScroll()->IsNormal() )
		{
			CStep &step=page->GetScroll()->GetStep();
			if (step.GetPosition() > step.GetMin() && step.GetPosition()+5 < step.GetMax())
			{
				IsPressScroll = true;
			}
		}
	}

	if( IsPressScroll )
		page->Refresh();	
	else
		page->OnKeyDown(VK_END);
	
	while (m_cSystemMsg.MoveToNextMsg())
	{
		CCharMsg::sTextInfo msg=m_cSystemMsg.GetMsgInfo();
		string strMsg=msg.strShowText;
		bIndentFlag=false;
		while (!strMsg.empty())
		{
			if (bIndentFlag)
			{
				strMsg="      "+strMsg;
			}
			string strShowText=CutFaceText(strMsg,44);
			CItemEx* pItem=new CItemEx(strShowText.c_str(), CCharMsg::GetChannelColor(msg.eTextChannel));
			pItem->SetAlignment(eAlignCenter);
			pItem->SetHeight(18);
			pItem->SetIsParseText(true);
			pItem->SetItemName(msg.strWho.c_str());
			if (!bIndentFlag)
			{
				//0xFFFFFF00 // system box mothannakh 
				pItem->SetHasHeadText(8, 0xFFFFFF00);
			}
			m_lstSystemPage->NewItem()->SetBegin(pItem);
			bIndentFlag=true;
		}
	};
	pListItems=m_lstSystemPage->GetItems();
	for (int i=pListItems->GetCount()-200; i>0; i--)
	{
		pListItems->Del(pListItems->GetItem(0));
	}
	m_lstSystemPage->Refresh();

	IsPressScroll = false;
	if( g_pGameApp->IsMouseButtonPress( 0 ) )
	{
		if( m_lstSystemPage->GetScroll()->IsNormal() )
		{
			int x = g_pGameApp->GetMouseX();
			int y = g_pGameApp->GetMouseY();
			GetRender().ScreenConvert(x, y);
			if( !m_lstSystemPage->GetScroll()->InRect(x, y) ) 
			{
				IsPressScroll = true;
			}
		}
	}
	else
	{
		if( page->GetScroll()->IsNormal() )
		{
			CStep &step=page->GetScroll()->GetStep();
			if (step.GetPosition() > step.GetMin() && step.GetPosition()+5 < step.GetMax())
			{
				IsPressScroll = true;
			}
		}
	}


	if( IsPressScroll )
		m_lstSystemPage->Refresh();	
	else
		m_lstSystemPage->OnKeyDown(VK_END);

	if (GuildSysInfo)
		GuildSysInfo = !GuildSysInfo;
}

void CCozeForm::ResetPages()
{
	bool bIndentFlag;
	m_lstMainPage->GetItems()->Clear();
	if (m_cMainMsg.MoveToFirstMsg())
	{
		CCharMsg::sTextInfo msg=m_cMainMsg.GetMsgInfo();
		string strMsg=msg.strShowText;
		bIndentFlag=false;
		
		CList* page = m_lstMainPage;
		while (!strMsg.empty())
		{
			if (bIndentFlag)
			{
				strMsg="      "+strMsg;
			}
			string strShowText=CutFaceText(strMsg,44);
			CItemEx* pItem=new CItemEx(strShowText.c_str(), CCharMsg::GetChannelColor(msg.eTextChannel));
			pItem->SetAlignment(eAlignCenter);
			pItem->SetHeight(18);
			pItem->SetIsParseText(true);
			pItem->SetItemName(msg.strWho.c_str());
			if (!bIndentFlag)
			{
				//discord channel c435cd mothannakh 0xFFFFFF00
				pItem->SetHasHeadText(6, 0xFFFFFF00);
			}
			page->NewItem()->SetBegin(pItem);
			bIndentFlag=true;
		}
	}

	m_lstSystemPage->GetItems()->Clear();
	if (m_cSystemMsg.MoveToFirstMsg())
	{
		CCharMsg::sTextInfo msg=m_cSystemMsg.GetMsgInfo();
		string strMsg=msg.strShowText;
		bIndentFlag=false;
		while (!strMsg.empty())
		{
			if (bIndentFlag)
			{
				strMsg="      "+strMsg;
			}
			string strShowText=CutFaceText(strMsg,44);
			CItemEx* pItem=new CItemEx(strShowText.c_str(), CCharMsg::GetChannelColor(msg.eTextChannel));
			pItem->SetAlignment(eAlignCenter);
			pItem->SetHeight(18);
			pItem->SetIsParseText(true);
			pItem->SetItemName(msg.strWho.c_str());
			if (!bIndentFlag)
			{
				//mothannakh  0xFFFFFF00
				pItem->SetHasHeadText(6, 0xFFFFFF00);
			}
			m_lstSystemPage->NewItem()->SetBegin(pItem);
			bIndentFlag=true;
		}
	}
	UpdatePages();
}

void CCozeForm::ChangePrivatePlayerName(string strName)
{
	if (strName.empty())
	{
		return;
	}
	
	string strChat=m_edtMsg->GetCaption();
	string strPrivateCmd=CCharMsg::GetChannelCommand(CCharMsg::GetChannelIndex(CCharMsg::CHANNEL_PRIVATE));
	int nLTrim=0;
	if (strChat.find(CCharMsg::GetChannelCommand(CCharMsg::GetChannelIndex(CCharMsg::CHANNEL_WORLD)))==0 ||
		strChat.find(CCharMsg::GetChannelCommand(CCharMsg::GetChannelIndex(CCharMsg::CHANNEL_TRADE)))==0 ||
		strChat.find(CCharMsg::GetChannelCommand(CCharMsg::GetChannelIndex(CCharMsg::CHANNEL_TEAM)))==0  || 
		strChat.find(CCharMsg::GetChannelCommand(CCharMsg::GetChannelIndex(CCharMsg::CHANNEL_GUILD)))==0
		)
	{
		nLTrim=1;
	}
	else if (strChat.find(strPrivateCmd)==0)
	{
		auto nPos=strChat.find(" ");
		if (nPos!=std::string::npos)
			nLTrim = nPos + 1;
		else
			nLTrim = (int) strChat.length();
	}
	if (nLTrim>0 && nLTrim<=(int)strChat.length())
	{
		strChat=strChat.substr(nLTrim,strChat.length()-nLTrim);
	}
	strChat=strPrivateCmd+strName+" "+strChat;
	if ((int)strChat.length()>m_edtMsg->GetMaxNum())
	{
		strChat=strPrivateCmd+strName+" ";
	}
	m_edtMsg->SetCaption(strChat.c_str());
}

void CCozeForm::EventPublishShowForm(CForm* pForm, bool& IsShow)
{
	CCharacter* pCharacter=CGameScene::GetMainCha();
	if(!pCharacter) return;

	if(pCharacter->getGMLv()==0)
	{
		IsShow=false;
	}
}

void CCozeForm::EventPublishSendMsg(CGuiData *pSender)
{
	CCharacter* pCharacter=CGameScene::GetMainCha();
	if(!pCharacter) return;

	//����?�ǿո�����ʾ
	CCozeForm *pThis=CCozeForm::GetInstance();
	string strMsg=pThis->m_edtPublishMsg->GetCaption() ;
	if (strMsg.empty() || strMsg.length() == count( strMsg.begin( ), strMsg.end( ), ' '))
	{
		return;
	}

	CS_GM1Say(strMsg.c_str());
	pThis->m_edtPublishMsg->SetCaption("");
}

bool CCozeForm::EventGlobalKeyDownHandle(int& key)
{
	if(g_pGameApp->IsCtrlPress())
	{
		constexpr auto delay{ 100 };
		static auto t = g_pGameApp->GetCurTick();
		if ((g_pGameApp->GetCurTick() - t) > delay)
		{
			t = g_pGameApp->GetCurTick();

			WORD wFaceID = key - '0';
			if (wFaceID >= 0 && wFaceID <= 9)
			{
				CCharacter* pCharacter = CGameScene::GetMainCha();
				if (pCharacter)
				{
					char lpszBuf[20];
					sprintf(lpszBuf, "***%d", wFaceID);
					pCharacter->GetHeadSay()->SetFaceID(wFaceID);
					CS_Say(lpszBuf);
				}
			}
		}
	}
	return false;
}

void CCozeForm::EventSendMsg(CGuiData *pSender)
{
	CCozeForm::GetInstance()->SendMsg();
	extern bool g_IsNumberTopBar;
	if (g_IsNumberTopBar)
	{
		CCompent::SetActive(nullptr);
	}
}

bool CCozeForm::EventEditMsg(CGuiData *pSender, int& key)
{
	CCozeForm *pThis=CCozeForm::GetInstance();
	if (key == VK_PRIOR || key == VK_NEXT)
	{
		pThis->m_lstMainPage->OnKeyDown(key);
		return true;
	}
	else if (key == VK_UP || key == VK_DOWN)
	{
		if (!pThis->m_bSendMsgCardSwitch)
		{
			if (pThis->m_cSendMsgCard.MoveToLastCard())
			{
				pThis->m_bSendMsgCardSwitch=true;
				pThis->m_edtMsg->SetCaption(pThis->m_cSendMsgCard.GetCardInfo().c_str());
				return true;
			}
		}
		else
		{
			if (key == VK_UP)
			{
				if (pThis->m_cSendMsgCard.MoveToPrevCard())
				{
					pThis->m_edtMsg->SetCaption(pThis->m_cSendMsgCard.GetCardInfo().c_str());
					return true;
				}
			}
			else
			{
				if (pThis->m_cSendMsgCard.MoveToNextCard())
				{
					pThis->m_edtMsg->SetCaption(pThis->m_cSendMsgCard.GetCardInfo().c_str());
					return true;
				}
			}
		}
	}
	return false;
}

void CCozeForm::EventMainListKeyDown(CGuiData *pSender, int x, int y, DWORD key)
{
	CCozeForm *pThis=CCozeForm::GetInstance();
	if( key & Mouse_RDown )
	{
		CItemRow* pItemRow=pThis->m_lstMainPage->GetSelectItem();
		if (!pItemRow) return;
		CItemEx* pItemEx=dynamic_cast<CItemEx*>(pItemRow->GetBegin());
		if (!pItemEx) return;
		string strName=pItemEx->GetItemName();
		CCharacter* pCharacter=CGameScene::GetMainCha();
		if (pCharacter)
		{
			if (strName==pCharacter->getHumanName())
			{
				return;
			}
		}
		pThis->ChangePrivatePlayerName(strName);
	}
}

void CCozeForm::EventSendChannelSwitchClick(CGuiData *pSender, int x, int y, DWORD key)
{
	CCozeForm *pThis=CCozeForm::GetInstance();
	pThis->m_lstCallingCard->SetIsShow(false);
	pThis->m_grdFacePanel->SetIsShow(false);
	pThis->m_grdBrowPanel->SetIsShow(false);
	pThis->m_grdActionPanel->SetIsShow(false);
}

void CCozeForm::EventSendChannelChange(CGuiData *pSender)
{
	CCozeForm *pThis=CCozeForm::GetInstance();
	string str=pThis->m_cmbChannel->GetText();
	if (str==CCharMsg::GetChannelName(CCharMsg::CHANNEL_SIGHT))
	{
		pThis->m_eCurSelChannel=CCharMsg::CHANNEL_SIGHT;
	}
	else if (str==CCharMsg::GetChannelName(CCharMsg::CHANNEL_TEAM))
	{
		pThis->m_eCurSelChannel=CCharMsg::CHANNEL_TEAM;
	}
	else if (str==CCharMsg::GetChannelName(CCharMsg::CHANNEL_GUILD))
	{
		pThis->m_eCurSelChannel=CCharMsg::CHANNEL_GUILD;
	}
	else if (str==CCharMsg::GetChannelName(CCharMsg::CHANNEL_TRADE))
	{
		pThis->m_eCurSelChannel=CCharMsg::CHANNEL_TRADE;
	}
	else if (str==CCharMsg::GetChannelName(CCharMsg::CHANNEL_WORLD))
	{
		pThis->m_eCurSelChannel=CCharMsg::CHANNEL_WORLD;
	}
	else if (str==CCharMsg::GetChannelName(CCharMsg::CHANNEL_SIDE))
	{
		pThis->m_eCurSelChannel=CCharMsg::CHANNEL_SIDE;
	}
	else
	{
		pThis->m_eCurSelChannel=CCharMsg::CHANNEL_NONE;
	}
	pThis->m_edtMsg->SetCaption(CCharMsg::GetChannelCommand(CCharMsg::GetChannelIndex(pThis->m_eCurSelChannel)).c_str());
}

void CCozeForm::EventMainPageDragBegin(CGuiData *pSender, int x, int y, DWORD key)
{
	CCozeForm *pThis=CCozeForm::GetInstance();
	pThis->m_nHeightBeforeDrag=pThis->m_lstMainPage->GetHeight();
	pThis->m_nTopBeforeDrag=pThis->m_lstMainPage->GetTop();
}

void CCozeForm::EventMainPageDragging(CGuiData *pSender, int x, int y, DWORD key)
{
	CCozeForm *pThis=CCozeForm::GetInstance();
	int nDragTop=pThis->m_lstMainPage->GetDrag()->GetDragY();
	int nHeight=pThis->m_nHeightBeforeDrag - nDragTop;
	if (m_nMainPageMinHeight <= nHeight && nHeight <= m_nMainPageMaxHeight)
	{
		CList *pList=pThis->m_lstMainPage;
		pList->SetPos(pList->GetLeft(), pThis->m_nTopBeforeDrag + nDragTop);
		pList->SetSize(pList->GetWidth(), nHeight);
		pList->Init();
		pList->Refresh();
		pList->GetItems()->SetFirstShowRow(pList->GetItems()->GetCount() - pList->GetItems()->GetShowCount());
		pList->Refresh();

		pThis->m_drgMainPage->SetPos(pList->GetLeft(), pList->GetTop() - 4);
		pThis->m_drgMainPage->Refresh();

		pList=pThis->m_lstSystemPage;
		pList->SetPos(pList->GetLeft(), pThis->m_nTopBeforeDrag + nDragTop - pList->GetHeight());
		pList->Init();
		pList->Refresh();
		pList->GetItems()->SetFirstShowRow(pList->GetItems()->GetCount() - pList->GetItems()->GetShowCount());
		pList->Refresh();

		
		
		pThis->m_drgSystemPage->SetPos(pList->GetLeft(), pList->GetTop() - 4);
		pThis->m_drgSystemPage->Refresh();
	}
}

void CCozeForm::EventMainPageDragEnd(CGuiData *pSender, int x, int y, DWORD key)
{
	EventMainPageDragging(pSender, x, y, key);
	CCozeForm *pThis=CCozeForm::GetInstance();
	CList *pList=pThis->m_lstMainPage;
	pThis->m_drgMainPage->SetPos(pList->GetLeft(), pList->GetTop() - 4);
	pThis->m_drgMainPage->Refresh();

	pList=pThis->m_lstSystemPage;
	pThis->m_drgSystemPage->SetPos(pList->GetLeft(), pList->GetTop() - 4);
	pThis->m_drgSystemPage->Refresh();
}

void CCozeForm::EventSystemPageDragBegin(CGuiData *pSender, int x, int y, DWORD key)
{
	CCozeForm *pThis=CCozeForm::GetInstance();
	pThis->m_nHeightBeforeDrag=pThis->m_lstSystemPage->GetHeight();
	pThis->m_nTopBeforeDrag=pThis->m_lstSystemPage->GetTop();
}

void CCozeForm::EventSystemPageDragging(CGuiData *pSender, int x, int y, DWORD key)
{
	CCozeForm *pThis=CCozeForm::GetInstance();
	int nDragTop=pThis->m_lstSystemPage->GetDrag()->GetDragY();
	int nHeight=pThis->m_nHeightBeforeDrag - nDragTop;
	if (m_nSystemPageMinHeight <= nHeight && nHeight <= m_nSystemPageMaxHeight)
	{
		CList *pList=pThis->m_lstSystemPage;
		pList->SetPos(pList->GetLeft(), pThis->m_nTopBeforeDrag + nDragTop);
		pList->SetSize(pList->GetWidth(), nHeight);
		pList->Init();
		pList->Refresh();
		pList->GetItems()->SetFirstShowRow(pList->GetItems()->GetCount() - pList->GetItems()->GetShowCount());
		pList->Refresh();

		pThis->m_drgSystemPage->SetPos(pList->GetLeft(), pList->GetTop() - 4);
		pThis->m_drgSystemPage->Refresh();
	}
}

void CCozeForm::EventSystemPageDragEnd(CGuiData *pSender, int x, int y, DWORD key)
{
	EventSystemPageDragging(pSender, x, y, key);
	CCozeForm *pThis=CCozeForm::GetInstance();
	CList *pList=pThis->m_lstSystemPage;
	pThis->m_drgSystemPage->SetPos(pList->GetLeft(), pList->GetTop() - 4);
	pThis->m_drgSystemPage->Refresh();
}

void CCozeForm::EventChannelSwitchCheck(CGuiData *pSender)
{
	CCozeForm *pThis=CCozeForm::GetInstance();
	pThis->m_lstCallingCard->SetIsShow(false);
	pThis->m_grdFacePanel->SetIsShow(false);
	pThis->m_grdBrowPanel->SetIsShow(false);
	pThis->m_grdActionPanel->SetIsShow(false);
	pThis->m_cmbChannel->GetList()->SetIsShow(false);

	CChannelSwitchForm::GetInstance()->SwitchCheck();
}

void CCozeForm::EventCallingCardSwitchClick(CGuiData *pSender, int x, int y, DWORD key)
{
	CCozeForm *pThis=CCozeForm::GetInstance();

	pThis->m_lstCallingCard->GetItems()->Clear();
	if (pThis->m_cCallingCard.MoveToFirstCard())
	{
		do
		{
			pThis->m_lstCallingCard->Add(pThis->m_cCallingCard.GetCardInfo().c_str());
		} while(pThis->m_cCallingCard.MoveToNextCard());
	}

	CItemRow *pRow=pThis->m_lstCallingCard->Add(g_oLangRec.GetString(515));
	pRow->GetBegin()->SetColor(0xFF7F7F3F);
	pThis->m_lstCallingCard->SetPointer(pRow);
	pThis->m_lstCallingCard->Refresh();

	pThis->m_lstCallingCard->SetIsShow(!pThis->m_lstCallingCard->GetIsShow());
	pThis->m_grdFacePanel->SetIsShow(false);
	pThis->m_grdBrowPanel->SetIsShow(false);
	pThis->m_grdActionPanel->SetIsShow(false);
	pThis->m_cmbChannel->GetList()->SetIsShow(false);

}

//void CCozeForm::EventCallingCardLostFocus(CGuiData *pSender)
//{
//	CCozeForm *pThis=CCozeForm::GetInstance();
//	pThis->m_lstCallingCard->SetIsShow(false);
//	pThis->m_lstCallingCard->MouseRun(pThis->m_lstCallingCard->GetX()-1,pThis->m_lstCallingCard->GetY()-1,0);
//}

void CCozeForm::EventCardSelected(CGuiData *pSender)
{
	CCozeForm *pThis=CCozeForm::GetInstance();
	pThis->m_lstCallingCard->SetIsShow(false);
	CItemRow *pRow=pThis->m_lstCallingCard->GetSelectItem();
	if (!pRow)
	{
		return;
	}
	string strName=pRow->GetBegin()->GetString();
	if (strName.empty())
	{
		return;
	}
	if (pThis->m_lstCallingCard->GetPointer()==pRow)
	{
		pThis->m_cCallingCard.ClearAll();
		return;
	}

	pThis->ChangePrivatePlayerName(strName);
}

void CCozeForm::EventFacePanelSwitchClick(CGuiData *pSender, int x, int y, DWORD key)
{
	CCozeForm *pThis=CCozeForm::GetInstance();
	pThis->m_grdFacePanel->SetIsShow(!pThis->m_grdFacePanel->GetIsShow());
	pThis->m_lstCallingCard->SetIsShow(false);
	pThis->m_grdBrowPanel->SetIsShow(false);
	pThis->m_grdActionPanel->SetIsShow(false);
	pThis->m_cmbChannel->GetList()->SetIsShow(false);
}

//void CCozeForm::EventFacePanelLostFocus(CGuiData *pSender)
//{
//	CCozeForm *pThis=CCozeForm::GetInstance();
//	pThis->m_grdFacePanel->SetIsShow(false);
//	pThis->m_grdFacePanel->MouseRun(pThis->m_grdFacePanel->GetX()-1, pThis->m_grdFacePanel->GetY()-1, 0);
//}

void CCozeForm::EventFaceSelected(CGuiData *pSender)
{
	CCozeForm *pThis=CCozeForm::GetInstance();
	pThis->m_grdFacePanel->SetIsShow(false);

	if ((int)strlen(pThis->m_edtMsg->GetCaption())>pThis->m_edtMsg->GetMaxNum()-3)
	{
		return;
	}

	if (pThis->m_grdFacePanel->GetSelect())
	{
		pThis->m_edtMsg->SetActive(pThis->m_edtMsg);
		char lpszFace[10];
		sprintf(lpszFace, "#%02d", pThis->m_grdFacePanel->GetSelectIndex());
		pThis->m_edtMsg->ReplaceSel(lpszFace);
	}
}

void CCozeForm::EventBrowPanelSwitchClick(CGuiData *pSender, int x, int y, DWORD key)
{
	CCozeForm *pThis=CCozeForm::GetInstance();
	pThis->m_grdBrowPanel->SetIsShow(!pThis->m_grdBrowPanel->GetIsShow());
	pThis->m_lstCallingCard->SetIsShow(false);
	pThis->m_grdFacePanel->SetIsShow(false);
	pThis->m_grdActionPanel->SetIsShow(false);
	pThis->m_cmbChannel->GetList()->SetIsShow(false);
}

void CCozeForm::EventBrowSelected(CGuiData *pSender)
{
	CCozeForm *pThis=CCozeForm::GetInstance();
	CCharacter* pCharacter=CGameScene::GetMainCha();
	if(!pCharacter) return;
	pThis->m_grdBrowPanel->SetIsShow(false);

	if (pThis->m_grdBrowPanel->GetSelect())
	{
		int nIndex = pThis->m_grdBrowPanel->GetSelectIndex();
		pCharacter->GetHeadSay()->SetFaceID(nIndex);
		char lpszFaceID[10] = {0} ;
		sprintf(lpszFaceID , "***%d" ,nIndex);	
		CS_Say(lpszFaceID);		
	}
}

void CCozeForm::EventActionPanelSwitchClick(CGuiData *pSender, int x, int y, DWORD key)
{
	CCozeForm *pThis=CCozeForm::GetInstance();
	pThis->m_grdActionPanel->SetIsShow(!pThis->m_grdActionPanel->GetIsShow());
	pThis->m_lstCallingCard->SetIsShow(false);
	pThis->m_grdFacePanel->SetIsShow(false);
	pThis->m_grdBrowPanel->SetIsShow(false);
	pThis->m_cmbChannel->GetList()->SetIsShow(false);
}

void CCozeForm::EventActionSelected(CGuiData *pSender)
{
	CCozeForm *pThis=CCozeForm::GetInstance();
	CCharacter* pCharacter=CGameScene::GetMainCha();
	if(!pCharacter) return;
	pThis->m_grdActionPanel->SetIsShow(false);

	CGraph * graph=pThis->m_grdActionPanel->GetSelect();
	//int nIndex = pThis->m_grdActionPanel->GetSelectIndex();
	if(graph)
	{		
		//pCharacter->GetActor()->PlayPose(graph->nTag, true, true);
		pCharacter->GetActor()->PlayPose(graph->nTag, true, true);
	}
}

bool GUI::CCozeForm::IsChatBoxActive() const
{
	if (m_edtMsg && m_edtMsg == CCompent::GetActive())
	{
		return true;
	}
	return false;
}

void GUI::CCozeForm::ActivateChatBox()
{
	CCompent::SetActive(m_edtMsg);
}

void GUI::CCozeForm::DisableChatBox()
{
	if (IsChatBoxActive())
	{
		CCompent::SetActive(nullptr);
	}
}
