#pragma once
#include "GuildData.h"
#include "uiglobalvar.h"


namespace GUI
{

class CUIGuildApply :
	public CUIInterface
{
public:
	CUIGuildApply(void);
	~CUIGuildApply(void);
	static void ShowForm();

protected:
	virtual bool Init();

private:

	static void OnConfirm(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
	static void OnShowForm(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
	static void OnEscClose(CForm* pForm);
	

	static CForm*	m_pGuildNameInputForm;
	static CEdit*	m_pGuildNameEdit;
	static CEdit*	m_pGuildPasswordEdit;
	static CEdit*	m_pGuildConfirmEdit;
};


}
