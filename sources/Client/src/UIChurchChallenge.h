//add by ALLEN 2007-10-19
#pragma once


#include "GameApp.h"
#include "Scene.h"
#include "Character.h"
#include "UIForm.h"
#include "UIFormMgr.h"
#include "UIEdit.h"
#include "UILabel.h"
#include "UIBoxForm.h"
#include "UITextButton.h"
#include "UIGlobalVar.h"
#include "PacketCmd.h"
#include "StringLib.h"


#define ENABLE_CHURCHCHALLENGE   1


namespace GUI
{

// 
class CChurchChallengeMgr : public CUIInterface
{
public:
	CChurchChallengeMgr(void);
	virtual ~CChurchChallengeMgr(void);

	void SetChallenge(const stChurchChallenge* pInfo);
	void ShowForm(bool bShow = true);

	void ChallengePriceAsk(void);
	void CheckChallengePrice(void);
	void NetSendChallenge(void);

protected:
	virtual bool Init();
	virtual void CloseForm();

private:

	CForm*			frmChurchSale;
	CTextButton*	btnPriceUp;

	CLabelEx*		labInfoShow1;
	CLabelEx*		labInfoShow2;
	CLabelEx*		labInfoShow3;

	stNumBox*		_pNumberBox;
	stChurchChallenge	_stInfo;

	int				_nPrice;

	static void _evtChallengeButtonDown(CGuiData *pSender, int x, int y, DWORD key);
	static void _evtChallengeNumInput(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
	static void _evtChallengeNumCheck(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);

};


}
