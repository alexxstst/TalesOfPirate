#pragma once
#include "uiglobalvar.h"
#include "GuildData.h"
#include "UIPage.h"
#include "PacketCmd.h"
#include "procirculate.h"
#include "CommandMessages.h"

namespace GUI
{

class CUIGuildMgr :
	public CUIInterface
{
public:
	CUIGuildMgr(void);
	~CUIGuildMgr(void);
	static void ShowForm();
	static void RefreshList();
	static void RefreshAttribute();
	static void RefreshForm();
	static void RemoveForm();

	static void UpdateGuildLogs(const net::msg::ActionUpdateGuildLogsData& data);
	static void RequestGuildLogs(const net::msg::ActionRequestGuildLogsData& data);
	static void UpdateLogList();
	static void SetActivePerm(int perm);
protected:
	virtual bool Init();

private:
	static CForm*		m_pGuildMgrForm;
	static CForm*		m_pGuildColForm;
	static CForm*		m_pGuildPermForm;
	static CLabelEx*	m_plabGuildName;
	static CLabelEx*	m_plabGuildMottoName;
	static CLabelEx*	m_plabGuildType;
	static CLabelEx*	m_plabGuildMaster;
	static CLabelEx*	m_plabGuildMemberCount;
	static CLabelEx*	m_plabGuildState;
	static CLabelEx*	m_plabGuildRemainTime;
	//static CLabelEx*	m_plabGuildRank;
	static CListView*	m_plstGuildMember;
	static CListView*	m_plstRecruitMember;
	static CTextButton*	m_pbtnGuildMottoEdit;
	static CTextButton*	m_pbtnGuildDisband;
	static CTextButton* m_pbtnGuildLeave;
	static CTextButton*	m_pbtnMemberRecruit;
	static CTextButton*	m_pbtnMemberRefuse;
	static CTextButton*	m_pbtnMemberKick;
	static CTextButton*	m_pbtnMemberPerms;
	static CTextButton*	m_pbtnMemberYesPerms;
	
	//Guild vault logs
	static CListView* m_plistBankLog;
	std::vector<BankLog> banklogs;
	int curLogPage;
	static CTextButton* m_btnNext;
	static CTextButton* m_btnPrev;
	static void _OnClickPrevLogs(CGuiData* pSender, int x, int y, DWORD key);
	static void _OnClickNextLogs(CGuiData* pSender, int x, int y, DWORD key);
	static CTreeView* m_trvPerm;
	static CPage* m_ppgeClass;
	static void _OnClickEditMottoName(CGuiData *pSender, int x, int y, DWORD key);
	static void _OnClickRecruit(CGuiData *pSender, int x, int y, DWORD key);
	static void _OnClickRefuse(CGuiData *pSender, int x, int y, DWORD key);
	static void _OnClickKick(CGuiData *pSender, int x, int y, DWORD key);
	static void _OnClickPerm(CGuiData *pSender, int x, int y, DWORD key);
	static void _OnClickConfirmPerm(CGuiData *pSender, int x, int y, DWORD key);
	static void _OnClickSelectPage(CGuiData *pSender);
	static void _OnClickLeave(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
	static void _OnPassKick(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
	static void _OnPassDismiss(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
	static void _OnClickPermText(CGuiData *pSender, int x, int y, DWORD key);
	static void _evtPermFormMouseEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
	
	static CForm* m_pGuildMottoNameEditForm;
	static CEdit* m_pedtGuildMottoName;
	static CTextButton*	m_pbtnGuildMottoFormOK;
	static void _OnClickMottoFormOK(CGuiData *pSender, int x, int y, DWORD key);
	static void OnBeforeShow(CForm* pForm,bool& IsShow);
};

}
