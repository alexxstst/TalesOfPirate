#pragma once

//#define MAX_SESSION_FORM 4

#include "UIMemo.h"

enum  eTalkSessionStyle
{
	enumTalkSessionHidden,			// 
	enumTalkSessionInform,			// 
	enumTalkSessionNormal,			// 
	enumTalkSessionSmall,			// 
	enumTalkSessionMinimize,		// 
};

struct stNetSessCreate;

class CSessionMember
{
public:
	CSessionMember(stNetSessCreate *pMember);
	~CSessionMember();

	DWORD		GetID()     { return _nID;              }
	DWORD		GetIconID() { return _nIcon_id;			}
	const char* GetName()   { return _strName.c_str();  }
	const char* GetMotto()  { return _strMotto.c_str();  }
	void		SetMotto(const char *szMotto)  { _strMotto=szMotto;  }
	CTeam*      GetTeam()   { return _pTeam;            }

	void        SetSessionFormPointer( void* p )   { _pPointer = p;        }
	void*       GetSessionFormPointer()            { return _pPointer;     }

private:
	std::string          _strName;       // 
	std::string          _strMotto;      // 
	DWORD			_nID;			// ID
	DWORD			_nIcon_id;
	void*           _pPointer;
	CTeam*          _pTeam;
	CMemberData*    _pData;
};

class CTalkSessionFormMgr;
class CTalkSessionForm
{
	friend CTalkSessionFormMgr;

	struct sTalkMsg
	{
		std::string name;
		std::string motto;
		std::string text;
	};

public:
	CTalkSessionForm(DWORD sessionID,eTalkSessionStyle style);
	~CTalkSessionForm(void);
	int GetTotalMembers();
	CForm* GetForm();
	bool hasMember(DWORD chaID);
	bool hasMember(std::string name);
	CSessionMember* GetMemberByIndex(int nIndex);
	CSessionMember* GetMemberByID(DWORD nID);
	void AddMembers(stNetSessCreate *pArrMembers[],int memberNum=1);
	bool DelMember(DWORD chaID);
	bool ChangeStyle(eTalkSessionStyle style);
	DWORD GetSessionID() {return m_dwSessioinID;}
	void RefreshForm();
	void SetActiveSession(bool bActive,DWORD newID=0)	{m_bActive=bActive;m_dwSessioinID=newID;}
	bool IsActiveSession()	{return m_bActive;}
	void GetMsg(DWORD chaID, std::string talkMsg);
	void SendBufferMsg();
	eTalkSessionStyle GetStyle()	{return m_Style;}
	void SetInformText(const char* name,const char* text);
	void StopFlash();
	void SetIsFlash(bool isFlash)	{ m_bFlash=isFlash;m_nFlashTimes=0;}
	bool GetIsFlash() { return m_bFlash;}
	void AddMemberToBuffer(std::string name)	{m_strWaitForAdded+=name+"\n";}
	std::string GetMemberFromBuffer()	{ std::string str=m_strWaitForAdded;m_strWaitForAdded.clear();return str;}
	void SetData(LPVOID data)	{ m_pData=data; }
	LPVOID GetData()	{ return m_pData; }
	void ClearBuffer()	{m_strWaitForSend.clear();m_strWaitForAdded.clear();}
	int GetInformShowChannel();
	void ReleaseInformShowChannel();
	void OnHotKeyShow();

	CForm*			m_pMinimizeForm;

private:
	bool			m_bActive;
	bool			m_bFlash;
	int				m_nFlashTimes;
	CGuiTime*		m_pTimer1;
	CGuiTime*		m_pTimer2;
	std::string			m_strWaitForSend;
	std::string			m_strWaitForAdded;
	CForm*			m_pForm;
	CForm*			m_pNormalForm;
	CForm*			m_pSmallForm;
	CForm*			m_pInformForm;
	CEdit*			m_pEdit;
	CEdit*			m_pNormalEdit;
	CEdit*			m_pSmallEdit;
	CRichMemo*		m_pMemo;
	CRichMemo*		m_pNormalMemo;
	//CRichMemo*		m_pSmallMemo;
	//CList*			m_pList;
	//CList*			m_pNormalList;
	//CList*			m_pSmallList;
	CList*			m_pNameList;
	CList*			m_pNormalNameList;
	CTextButton*	m_pInformTextBtn;
	CLabelEx*		m_pInformTitle;
	CGrid*			m_pFaceGrid;
	static CMenu*	m_playerMouseRight;
	DWORD			m_dwSessioinID;
	std::string			m_strTalkMsg;
	int				m_nShowChannel;
	int				m_nShowSx;
	int				m_nShowSy;
	bool			m_bQuitFlag;
	std::vector<CSessionMember*> m_Members;
	eTalkSessionStyle m_Style;
	LPVOID			m_pData;
	std::vector<sTalkMsg*> m_TalkMsg;

	void ClearMsg();
	static void _OnMouseInformButton(CGuiData *pSender, int x, int y, DWORD key);
	static void _OnMouseNormalButton(CGuiData *pSender, int x, int y, DWORD key);
	static void _OnMouseSmallButton(CGuiData *pSender, int x, int y, DWORD key);
	static void _OnMouseMinimizeButton(CGuiData *pSender, int x, int y, DWORD key);
	static void _OnMouseQuitButton(CGuiData *pSender, int x, int y, DWORD key);
	static void _OnMouseCloseButton(CGuiData *pSender, int x, int y, DWORD key);
	static void _OnMouseSendButton(CGuiData *pSender, int x, int y, DWORD key);
	static void _OnMouseSelectFace(CGuiData *pSender);
	static void _OnTimerInformShowing(CGuiTime *pSender);
	static void _OnMouseFaceListButton(CGuiData *pSender, int x, int y, DWORD key);
	static void _OnMouseCleanText(CGuiData *pSender, int x, int y, DWORD key);
	static void _OnMouseRightPlayerMenu(CGuiData *pSender, int x, int y, DWORD key);
	static void _OnPlayerSelect(CGuiData *pSender, int x, int y, DWORD key);
	static void _OnFaceLostFocus(CGuiData *pSender);
	static void _OnMouseSaveButton(CGuiData *pSender, int x, int y, DWORD key);
	static bool _OnHotKey(CForm *pSender, char key);
	static void GuiFormEscCloseEvent(CForm *pForm);
};



class CTalkSessionFormMgr
{
public:
	struct sApplyMember
	{
		std::string name;
		static UCHAR numbers;
	};
	CTalkSessionFormMgr(void);
	~CTalkSessionFormMgr(void);

	static CTalkSessionForm* CreateSessionForm(DWORD sessionID, stNetSessCreate *pArrMembers[],int memberNum=1);
	static CTalkSessionForm* CreateHiddenSessionForm(DWORD sessionID, stNetSessCreate *pArrMembers[],int memberNum=1);
	static void ApplySession(CMember *pArrMember[],int memberNum=1);
	static void ApplySession(sApplyMember pApplyMembers[]);
	static bool AddToSession(DWORD sessionID,stNetSessCreate *pMember);
	static void LeaveSession(DWORD sessionID,DWORD chaID);
	static void CloseSessionForm(DWORD sessionID);
	static void ClearAll();
	static CTalkSessionForm* GetSessionFormByIndex(int nIndex);
	static CTalkSessionForm* GetSessionFormByID(DWORD sessionID);
	static CTalkSessionForm* GetSessionFormByName(std::string name,int index=0);
	static CTalkSessionForm* GetSessionFormByForm(CForm *form);
	static void SessionReceiveMsg(DWORD sessionID,DWORD chaID, std::string talkMsg);

	static int GetShowChannel();
	static void ReleaseShowChannel(int n);
	static POINT GetMiniFormPosition(DWORD sessionID);
	static POINT GetNormalFormPosition(DWORD sessionID);

	static void OnClickOpenSession(CItemObj* pItem);
	static void OnClickCloseSession(CTalkSessionForm* pSession);
	static void OnFlashSession();
	static bool OnHotKeyShow( char& key, int& control );

	static void RefreshSessionMember(DWORD chaID,const char* szMotto);

	static bool hasFlashSession();

	static void PrintTraceInfo();
	static DWORD GetSessionCount();


	//static void SetInformSize(RECT rectFrom,RECT rectTitle);
	//static void SetTablePosition(POINT tPosition);
	//static void TableInit();

	//static RECT TableToTitle(int tx,int ty);
	//static RECT TableToForm(int tx,int ty);

	//static void GetFormRect(DWORD sessionID,RECT &rect);
	//static void GetTitleRect(DWORD sessionID,RECT &rect);

protected:
	//static RECT INFORM_FORM;
	//static RECT INFORM_TITLE;
	//static int INFORM_TABLE_W;
	//static int INFORM_TABLE_H;
	//static POINT INFORM_TABLE_POSITION;


	//static CTalkSessionForm* CreateSessionForm(CMember *pMember,eTalkSessionStyle style=enumTalkSessionNormal);
	//static bool AddToSessionForm(CMember *pArrMembers,CTalkSessionForm *sessionForm);
	//static void LeaveSessionForm(CTalkSessionForm *sessionForm);

private:
	static int m_Channel[4];
	static DWORD m_SessionCounts;
	static std::vector<CTalkSessionForm*> m_SessionFormsLink;

	//static PS_Sessions* FindSession(CTalkSessionForm *sessionForm);

};
