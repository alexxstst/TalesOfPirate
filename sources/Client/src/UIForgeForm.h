#pragma once
#include "UIGlobalVar.h"

namespace GUI
{

// 
class CForgeMgr : public CUIInterface
{
public:
	void		ShowForge(bool bShow = true, bool isBurnish = false);
	int			GetForgeComIndex(COneCommand* oneCommand);
	void		DragToEquipGrid(int iIndex);
	void		Clear();
	void		ShowConfirmDialog(long lMoney);
	void		ForgeSuccess(long lChaID);
	void		ForgeFailed(long lChaID);
	void		ForgeOther(long lChaID);
	bool		IsRunning();

protected:
	virtual bool Init();
	virtual void CloseForm();
	virtual void SwitchMap();

	bool        SendForgeProtocol();

private:
	static void _MainMouseEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
	static void _DragEvtEquip(CGuiData *pSender,CCommandObj* pItem,bool& isAccept);
	static void _DragEvtGenStone(CGuiData *pSender,CCommandObj* pItem,bool& isAccept);
	static void _DragEvtForgStone(CGuiData *pSender,CCommandObj* pItem,bool& isAccept);
	static void _evtConfirmEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
	static void _OnClose(CForm* pForm, bool& IsClose);
	static void _ProTimeArriveEvt(CGuiData *pSender);
	static void _DragBeforeEvt(CGuiData *pSender, int x, int y, DWORD key);


private:
	void		ClearUI();
	void		ClearItem();

	bool		IsEquip(CItemCommand& rItem);
	bool		IsGenStone(CItemCommand& rItem);
	bool		IsForgStone(CItemCommand& rItem);
	bool		IsMillingReinforce(CItemCommand& rItem);
	bool		IsMillingKatalyst(CItemCommand& rItem);
	bool		IsValidDragSource();

	void		PushItem(int iIndex, CItemCommand& rItem);
	void		PopItem(int iIndex);

	void		SetForgeUI();

	long		CalForgeMoney();
	long		CalMillingMoney();

private:
	enum {
		EQUIP = 0,			// 
		GEN_STONE = 1,		// 
		FORGE_STONE = 2,	// 
		ITEM_NUM = 3,
	};

	CForm       *frmNPCforge ;//
	COneCommand *cmdForgeItem[ITEM_NUM];
	CLabelEx    *labForgeGold;
	CProgressBar *proNPCforge;
	CTextButton	*btnForgeYes;
	CTextButton *btnMillingYes;
	CTextButton *btnYes;
	CList		*lstForgeItemState;
	CImage		*imgMillingTitle;



	static const int NO_USE = -1;
	static const int FORGE_TYPE = 1;
	static const int MILLING_TYPE = 3;
	static const long FORGE_PER_LEVEL_MONEY = 100000;
	//static const long FORGE_PER_LEVEL_MONEY = 10000;	//	modify by alfred.shi 20080804
	static const long MILLING_PER_LEVEL_MONEY = 50000;
	static const int FORGE_PRO_TIME = 10000;	//10
	static const int FORGE_SUCCESS_EFF_ID = 345;
	static const int FORGE_FAILED_EFF_ID = 346;

	int			m_iForgeItemPos[ITEM_NUM];
	bool		m_isOldEquipFormShow;
	bool		m_isConfirm;
	bool		m_isMilling;

	static const int EQUIP_TYPE = 29;
	static const int GEN_STONE_TYPE = 49;
	static const int FORGE_STONE_TYPE = 50;
	static const int MILLING_Reinforce_TYPE = 51;
	static const int MILLING_Katalyst_TYPE = 52;
};

}

