#pragma once
#include "UIGlobalVar.h"
#include "NetProtocol.h"

namespace GUI
{

// NPC
class CNpcTalkMgr : public CUIInterface
{
public:
	void			ShowFuncPage( BYTE byFuncPage , BYTE byCount,BYTE byMisNum ,const NET_FUNCPAGE& FuncArray, DWORD dwNpcID ) ;      // NPC

	void			AddHelpInfo( const NET_HELPINFO &Info );
	void			ShowTalkPage( const char *content, BYTE command, DWORD npcID );
	void			CloseTalk( DWORD dwNpcID );

	DWORD			GetNpcId();

	static void     SetTalkStyle( BYTE bit )	{ _byTalkStyle = bit ;  }

protected:
	virtual bool Init();
    virtual void End();
	virtual void FrameMove(DWORD dwTime);
	virtual void LoadingCall();
	virtual void SwitchMap();

	void	CloseForm();
	void    ShowHelpInfo( const NET_HELPINFO& Info );

	static void _MainMouseNPCEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
	static void _evtMemSelectChange(CGuiData *pSender);

private:
	CForm*		frmNPCchat;
	CMemo*		memCtrl;
	
private:
	static BYTE		_byTalkStyle;		// 

	bool		m_bIsNpcTalk;

	typedef std::list<NET_HELPINFO> HELP_LIST;
	HELP_LIST	m_HelpInfoList;

};

}

