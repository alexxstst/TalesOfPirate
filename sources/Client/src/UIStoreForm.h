
#pragma once

#include "uiglobalvar.h"
#include "uiguidata.h"
#include "uiform.h"
#include "uilabel.h"
#include "uimemo.h"
#include "uiboxform.h"
#include "uifastcommand.h"
#include "uitreeview.h"
#include "uiprogressbar.h"
#include "UI3DCompent.h"
#include "NetProtocol.h"

#include <map>


namespace GUI
{

	// 
	struct stItemInfo
	{
		short       itemID;         //	
		int         itemNum;        //	
		int         itemFlute;      //	
		short       itemAttrID[5];  //	
		short       itemAttrVal[5]; //	

		void Clear() { memset(this, 0, sizeof(stItemInfo)); }
	};

	// 
	struct stStoreInfo
	{
		long        comID;          //	
		char        comName[21];    //	
		long        comClass;       //	
		long        comTime;        //	
		long        comPrice;       //	
		char        comRemark[129]; //	
		int         comNumber;      //  -10
		time_t      comExpire;      //  
		int         itemNum;        //  
		int         isHot;          //  
		stItemInfo  comItemInfo[6]; //  

		stStoreInfo() { memset(this, 0, sizeof(stStoreInfo)); }
		void Clear()  { memset(this, 0, sizeof(stStoreInfo)); }
	};

	//  GUI
	struct stStoreGui
	{
		CLabelEx*		labName;			// 
		CLabelEx*		labPrice;			// 
		CLabelEx*		labLeftTime;		// 
		CLabelEx*		labLeftNum;			// 
		CLabelEx*		labRemark;			// 
		CLabelEx*		labRightClickView;	// 
		CImage*			imgCutLine;			// 
		CImage*			imgHot;				// 
		CImage*			imgNew;				// 
		CImage*			imgBlue;			// 
		CImage*			imgSquare;			// 
		COneCommand*	cmdStore;			// 
		CTextButton*	btnBlue;			// 
		
		CImage*			imgWhite;

		void SetIsShow(bool v)
		{
			labName->SetIsShow(v);
			labPrice->SetIsShow(v);
			labRemark->SetIsShow(v);
			labLeftTime->SetIsShow(v);
			labLeftNum->SetIsShow(v);
			labRightClickView->SetIsShow(v);
			imgCutLine->SetIsShow(v);
			imgHot->SetIsShow(v);
			imgNew->SetIsShow(v);
			imgBlue->SetIsShow(v);
			imgSquare->SetIsShow(v);
			cmdStore->SetIsShow(v);
			btnBlue->SetIsShow(v);

			if(!v) cmdStore->DelCommand();
		}
	};


	class CStoreMgr : public CUIInterface
	{
	public:
		CStoreMgr();

		void ShowTempKitbag(bool bShow = true);
		void ShowStoreForm(bool bShow = true);
		void ShowStoreLoad(bool bShow = true);
		void ShowViewAllForm(bool bShow = true);
		void ShowTryonForm(bool bShow = true);
		void ShowAlphaMatteForm(bool bShow = true);
		void ShowStoreWebPage();

		CGoodsGrid* GetTempKitbagGrid()		{  return grdTempBag;	}
		CForm*		GetStoreForm()			{  return frmStore;		}
		static int	GetPageSize()			{  return STORE_PAGE_SIZE;  }

		// 
		void DarkScene(bool bDark);

		// 
		bool PopFromTempKitbag(CGoodsGrid& rkDrag, CGoodsGrid& rkSelf, int nGridID, CCommandObj& rkItem);
		
		// 
		void OpenStoreAsk();

		// 
		void AddStoreTreeNode(long nParentID, long nID, const char* szCaption);

		// 
		void AddStoreItemInfo(long nSeq, long nID, const char* szName, long nPrice, const char* szRemark, bool isHot, long nTime, long nRemainNum, long nRemainTime);

		// 
		void AddStoreItemDetail(long nSeq, long nSubSeq, short sItemID, short sItemNum, short sFlute, short pItemAttrID[], short pItemAttrVal[]);

		// 
		void AddStoreUserTreeNode(void);

		// 
		void SetStoreItemPage(long nCurPage, long nMaxPage);

		// 
		void SetStoreMoney(long nMoBean, long nRplMoney);

		// VIP
		void SetStoreVip(long nVip);

		// 
		//void AddStoreCommunique(long nID, const char* szTitle, 

		// 
		void StoreTreeRefresh();

		// 
		void ClearStoreTreeNode();

		// 
		void ClearStoreItemList();

		// 
		void SetStoreBuyButtonEnable(bool b);

		// ID
		int GetCurSelItemID();

		// 10 
		bool ResetLastOperate(bool bSilent = false);

		// 
		static const long USER_NODEID = 0x7FFFFFFE;
		static const long HELP_NODEID = 0x7FFFFFFF;

	protected:
		virtual bool Init();
		virtual void CloseForm();
		virtual void SwitchMap();
		virtual void FrameMove(DWORD dwTime);

	private:

		// 
		CForm*			frmTempBag;			// 
		CGoodsGrid*		grdTempBag;			// 
		
		
		stNumBox*		m_pkNumberBox;		// 
		stNetTempKitbag m_NetTempKitbag;	// 

		static void _evtDragItemsEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
		static void _MoveItemsEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);

	private:
		// 
		CForm*			frmStore;			// 
		CTreeView*		trvStore;			// 
		CLabelEx*		labMoneyLeft;		// 
		CLabelEx*		labBeanLeft;		// 
		CLabelEx*		labMemberStyle;		// 

		CLabelEx*		labNameTitle;
		CLabelEx*		labPriceTitle;
		CLabelEx*		labLeftTimeTitle;
		CLabelEx*		labLeftNumTitle;

		CLabelEx*		labListPage;		// 
		CTextButton*	btnLeftPage;		// 
		CTextButton*	btnRightPage;		// 
		CTextButton*	btnTrade;			// 
		CTextButton*	btnTryon;			// 
		CTextButton*	btnViewAll;			// 
		CLabelEx*		labTrade;			// 
		CLabelEx*		labTryon;			// 
		CLabelEx*		labViewAll;			// 
		CMemo*			memStoreHelp;		// 
		CImage*			imgBackGround10;	// 
		stNumBox*		m_pkExchangeNum;	// 

		static const int STORE_PAGE_SIZE = 9;		  //  9 
		stStoreGui		m_stStoreGui [STORE_PAGE_SIZE];
		stStoreInfo		m_stStoreInfo[STORE_PAGE_SIZE];

		long			m_nCurClass;		// 
		long			m_nCurSel;			// 
		long			m_nCurPage;			// 
		long			m_nMaxPage;			// 
		long			m_nExchangeNum;		// 
		long			m_nVip;				// VIP


		typedef std::map<long, std::string> MapNode;
		typedef std::map<long, std::string>::iterator MapNodeIter;
		MapNode			m_mapNode;		// 

		void _SetIsShowUserInfo(bool bShow);
		void _SetIsShowHelpInfo(bool bShow);
		void _SetIsShowCozeForm(bool bShow);
		void _RefreshStoreListHighLight();
		void _ShowTradeSelectBox();
		void _LoadStoreHelpInfo();
		void _ChangeChaPart(stNetLookInfo& SLookInfo);
		bool _IsCurSelVipNode(void);

		static void	_evtStoreTreeMouseClick(CGuiData *pSender, int x, int y, DWORD key);
		static void	_evtStoreOpenCheckEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
		static void	_evtStoreToVipEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
		static void _evtExchangeEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
		static void	_evtExchangeCheckEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
		static void	_evtTradeCheckEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
		static void _evtStoreFormClose(CForm* pForm, bool& IsClose);

		static void _evtStoreFormMouseEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
		static void	_evtStoreListMouseRClick(CGuiData *pSender, int x, int y, DWORD key);
		static void	_evtStoreListMouseDBClick(CGuiData *pSender, int x, int y, DWORD key);

	private:

		// 
		CForm*			frmStoreLoad;
		CProgressBar*	proStoreLoad;
		static void _evtStoreLoadFormClose(CForm* pForm, bool& IsClose);
		static void _evtProTimeArriveEvt(CGuiData *pSender);
		DWORD			_dwDarkScene;

		static const int STORE_OPEN_TIMEOUT = 15;	// 

	private:

		// 
		static const int STORE_ITEM_COUNT = 6;	// 
		CForm*			frmViewAll;
		CMemo*			memViewAll;
		CImage*			imgSquareViewAll[STORE_ITEM_COUNT];
		COneCommand*	cmdSquareIcon[STORE_ITEM_COUNT];

		static void _evtStoreViewAllMouseEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
		static void _evtStoreViewAllLostEvent(CGuiData *pSender);

	private:

		// (Avata)
		CForm*			frmTryon;		// 
		C3DCompent*		ui3dplayer;		// 3D

		int				m_nChaRotate;	// 
		CCharacter*		m_pCurrMainCha;
		bool			m_isFight;
		stNetLookInfo	m_sLookInfo;
		stNetLookInfo	m_sCurLookInfo;

		void			ChaEquipClearAll();
		void			ChaLeftRotate();
		void			ChaRightRotate();
		void			RenderChaTryon(int x, int y);	//  3D 

		static void	_evtChaTryonRenderEvent(C3DCompent *pSender, int x, int y);
		static void _evtChaEquipClearAll(CGuiData *sender, int x, int y, DWORD key);
		static void _evtChaLeftRotate(CGuiData *sender, int x, int y, DWORD key);
		static void _evtChaRightRotate(CGuiData *sender, int x, int y, DWORD key);
		static void _evtChaLeftContinueRotate(CGuiData *sender);
		static void _evtChaRightContinueRotate(CGuiData *sender);
		static void _evtTryonFormClose(CForm* pForm, bool& IsClose);

	};

}

