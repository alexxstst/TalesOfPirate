#pragma once
#include "UIGlobalVar.h"
#include <array>
//2006-1-8 By Arcol:CCharMsg
class CCharMsg
{
public:
	enum eChannel
	{
		CHANNEL_NONE			= 0,
		CHANNEL_ALL				= 1,
		CHANNEL_SIGHT			= 2,
		CHANNEL_PRIVATE			= 4,
		CHANNEL_WORLD			= 8,
		CHANNEL_TRADE			= 16,
		CHANNEL_TEAM			= 32,
		CHANNEL_GUILD			= 64,
		CHANNEL_SYSTEM			= 128,
		CHANNEL_PUBLISH			= 256,
		CHANNEL_SIDE			= 512,	// 
	};

	struct sTextInfo
	{
		eChannel eTextChannel;
		std::string	 strWho;
		std::string	 strText;
		std::string	 strShowText;
		bool	 bSendTo;
		DWORD	dwColour;
	};

private:
	typedef std::list<sTextInfo> lstTextInfoType;
	struct sChannelInfo
	{
		eChannel					enumChannel;
		std::string						strChannelName;
		DWORD						dwChannelColor;
		std::string						strChannelCommand;
		DWORD						dwTotalMsg;
		lstTextInfoType::iterator	itFirstMsgPos;
	};


public:
	CCharMsg();
	~CCharMsg();

	static void		AddMsg(eChannel channel, std::string strWho, std::string strText, bool bSendTo = false,DWORD dwColour = 0xFFFFFFFF);
	static void		SetChannelColor(eChannel channel, DWORD dwColor);
	static DWORD	GetChannelColor(eChannel channel);
	static std::string	GetChannelCommand(WORD wChannelIndex);
	static std::string	GetChannelName(eChannel channel);
	static std::string	GetChannelShowName(eChannel channel);
	static WORD		GetChannelIndex(eChannel channel);
	static eChannel	GetChannelByIndex(WORD wChannelIndex);
	static WORD		GetTotalChannelCount();
	static void		ClearAllMsg();
	static void		InitChannelNames();

	void			SetShowChannels(DWORD ecboShowChannels);
	bool			ModifyShowChannel(eChannel eShowChannel, bool bAddOrRemove = true, bool bShowTips = true);
	bool			IsShowChannel(eChannel channel);
	DWORD			GetShowChannels();

	bool			MoveToFirstMsg();
	bool			MoveToNextMsg();
	sTextInfo		GetMsgInfo();


private:


private:

	static const DWORD			m_dwChannelBufferSize = 100;
	static std::vector<sChannelInfo> m_sChannelInfo;
	static lstTextInfoType		m_lstMsgLink;
	typedef std::list<CCharMsg*>		lstInstanceType;
	static lstInstanceType		m_lstThisInstanceLink;
	
	bool						m_bCurMsgAvailable;
	DWORD						m_ecboShowChannels;
	lstTextInfoType::iterator	m_itCurrentMsgPos;
};

class CCardCase
{
public:
	CCardCase(WORD wMaxLimit=10);
	~CCardCase();
	void	AddCard(std::string str);
	bool	RemoveCard(std::string str);
	void	ClearAll();
	int		GetTotalCount();
	bool	MoveToFirstCard();
	bool	MoveToNextCard();
	bool	MoveToLastCard();
	bool	MoveToPrevCard();
	std::string	GetCardInfo();

private:
	typedef std::list<std::string>	lstCardType;
	lstCardType				m_lstCardData;
	lstCardType::iterator	m_itCurrentCardPos;
	int						m_wMaxLimit;
};

namespace GUI
{

class CChannelSwitchForm : public CUIInterface
{
public:

	CChannelSwitchForm();
	static CChannelSwitchForm* GetInstance();
	~CChannelSwitchForm();
	void SwitchCheck();

protected:
	
	bool Init();
	static void EventLostFocus(CGuiData *pSender);
	static void EventPrivateCheckChange(CGuiData *pSender);
	static void EventSightCheckChange(CGuiData *pSender);
	static void EventSystemCheckChange(CGuiData *pSender);
	static void EventTeamCheckChange(CGuiData *pSender);
	static void EventGuildCheckChange(CGuiData *pSender);
	static void EventWorldCheckChange(CGuiData *pSender);
	static void EventTradeCheckChange(CGuiData *pSender);

private:
	CForm		*m_frmChannelSwitch;
	CCheckBox	*m_chkPrivate;
	CCheckBox	*m_chkSight;
	CCheckBox	*m_chkSystem;
	CCheckBox	*m_chkTeam;
	CCheckBox	*m_chkGuild;
	CCheckBox	*m_chkWorld;
	CCheckBox	*m_chkTrade;
};



class CCozeForm : public CUIInterface
{
	friend CChannelSwitchForm;
public:
	CCozeForm();
	~CCozeForm();
	static CCozeForm* GetInstance();

	void OnSightMsg(std::string strName, std::string strMsg,DWORD dwColour=0xFFFFFFFF);
	void OnSightMsg(CCharacter* pChar, std::string strMsg,DWORD dwColour=0xFFFFFFFF);
	void OnPrivateMsg(std::string strFromName, std::string strToName, std::string strMsg,DWORD dwColour=0xFFFFFFFF);
	void OnWorldMsg(std::string strName, std::string strMsg,DWORD dwColour=0xFFFFFFFF);
	void OnTradeMsg(std::string strName, std::string strMsg,DWORD dwColour=0xFFFFFFFF);
	void OnTeamMsg(std::string strName, std::string strMsg,DWORD dwColour=0xFFFFFFFF);
	void OnTeamMsg(DWORD dwCharID, std::string strMsg,DWORD dwColour=0xFFFFFFFF);
	void OnGuildMsg(std::string strName, std::string strMsg,DWORD dwColour=0xFF63f7f5);
	void OnSystemMsg(std::string strMsg);
	void OnSideMsg(std::string strName, std::string strMsg,DWORD dwColour=0xFFFFFFFF);
	void OnPublishMsg(std::string strName, std::string strMsg);
	void OnPublishMsg1(std::string strMsg,int setnum, DWORD color);//Add by sunny.sun20080804
	void OnPrivateNameSet(std::string strName);
	void OnResetAll();
	bool IsMouseOnList(int x, int y);
	void AddToEdit(std::string strData);

	bool IsChatBoxActive() const;
	void ActivateChatBox();
	void DisableChatBox();


protected:
	bool Init();

	void SendMsg();
	void UpdatePages();
	void ResetPages();
	void ChangePrivatePlayerName(std::string strName);

	CCharMsg::eChannel	GetCmdFromMsg(std::string strMsg);

protected:
	static void EventPublishShowForm(CForm* pForm, bool& IsShow);
	static void EventPublishSendMsg(CGuiData *pSender);
	static bool EventGlobalKeyDownHandle(int& key);
	static void EventSendMsg(CGuiData *pSender);
	static bool EventEditMsg(CGuiData *pSender, int& key);
	static void EventMainListKeyDown(CGuiData *pSender, int x, int y, DWORD key);
	static void EventSendChannelSwitchClick(CGuiData *pSender, int x, int y, DWORD key);
	static void EventSendChannelChange(CGuiData *pSender);
	static void EventMainPageDragBegin(CGuiData *pSender, int x, int y, DWORD key);
	static void EventMainPageDragging(CGuiData *pSender, int x, int y, DWORD key);
	static void EventMainPageDragEnd(CGuiData *pSender, int x, int y, DWORD key);
	static void EventSystemPageDragBegin(CGuiData *pSender, int x, int y, DWORD key);
	static void EventSystemPageDragging(CGuiData *pSender, int x, int y, DWORD key);
	static void EventSystemPageDragEnd(CGuiData *pSender, int x, int y, DWORD key);
	
	static void EventChannelSwitchCheck(CGuiData *pSender);
	static void EventCallingCardSwitchClick(CGuiData *pSender, int x, int y, DWORD key);
	//static void EventCallingCardLostFocus(CGuiData *pSender);
	static void EventCardSelected(CGuiData *pSender);
	static void EventFacePanelSwitchClick(CGuiData *pSender, int x, int y, DWORD key);
	//static void EventFacePanelLostFocus(CGuiData *pSender);
	static void EventFaceSelected(CGuiData *pSender);
	static void EventBrowPanelSwitchClick(CGuiData *pSender, int x, int y, DWORD key);
	static void EventBrowSelected(CGuiData *pSender);
	static void EventActionPanelSwitchClick(CGuiData *pSender, int x, int y, DWORD key);
	static void EventActionSelected(CGuiData *pSender);
    void ReplaceSpecialFace(std::string &strMsg ,const std::string & strReplace ,const std::string & strFace );//Add by sunny.sun 20080902

private:
	CForm			*m_frmMainChat;
	CForm			*m_frmPublish;

	CCombo			*m_cmbChannel;
	CEdit			*m_edtPublishMsg;
	CEdit			*m_edtMsg;

	CList			*m_lstMainPage;
	CList			*m_lstSystemPage;
	CDragTitle		*m_drgMainPage;
	CDragTitle 		*m_drgSystemPage;
	CCheckBox		*m_chkChannelSwitch;
	CTextButton		*m_btnCallingCardSwitch;
	CList			*m_lstCallingCard;
	CTextButton		*m_btnFaceSwitch;
	CGrid			*m_grdFacePanel;
	CTextButton		*m_btnBrowSwitch;
	CGrid			*m_grdBrowPanel;
	CTextButton		*m_btnActionSwitch;
	CGrid			*m_grdActionPanel;

	CCharMsg::eChannel	m_eCurSelChannel;
	CCharMsg			m_cMainMsg;
	CCharMsg			m_cSystemMsg;
	CCardCase			m_cCallingCard;
	CCardCase			m_cSendMsgCard;

	bool				m_bSendMsgCardSwitch;
	int					m_nHeightBeforeDrag;
	int					m_nTopBeforeDrag;
	static const int	m_nMainPageMinHeight;
	static const int	m_nMainPageMaxHeight;
	static const int	m_nSystemPageMinHeight;
	static const int	m_nSystemPageMaxHeight;
};

}

