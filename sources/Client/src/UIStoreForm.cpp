
#include "stdafx.h"
#include "uistoreform.h"
#include "uidoublepwdform.h"

#include "GameApp.h"
#include "uiItem.h"
#include "uilabel.h"
#include "uiformmgr.h"
#include "uigoodsgrid.h"
#include "uiboxform.h"
#include "uigraph.h"
#include "uilist.h"
#include "uiEquipForm.h"
#include "UIGoodsGrid.h"
#include "uiItemCommand.h"
#include "uiBoatForm.h"
#include "packetcmd.h"
#include "Character.h"
#include "StringLib.h"
#include "SceneItem.h"

#include <time.h>
#include <shellapi.h>

#include "GlobalVar.h"

using namespace std;


namespace GUI
{

	CStoreMgr::CStoreMgr()
	{
		frmStore = NULL;

		m_nCurClass = 0;
		m_nCurSel   = -1;
		m_nVip      = 0;

		_dwDarkScene = 0;

		m_pCurrMainCha = NULL;
	}
	
	bool CStoreMgr::Init()
	{
		CFormMgr &mgr = CFormMgr::s_Mgr;
	
		frmTempBag = mgr.Find("frmTempBag");
		if (!frmTempBag)
		{
			ToLogService("common", "frmTempBag not found.");
			return false;
		}
		frmTempBag->SetIsDrag(true);
		frmTempBag->SetIsEscClose(true);
		
		grdTempBag = dynamic_cast<CGoodsGrid*>(frmTempBag->Find("grdTempBag"));
		if (!grdTempBag)
		{
			ToLogService("common", "frmTempBag:grdTempBag not found.");
			return false;
		}
		grdTempBag->evtBeforeAccept = CUIInterface::_evtDragToGoodsEvent;
		
		//
		// 
		//
		frmStore = mgr.Find("frmStore");
		if(!frmStore)
		{
			ToLogService("common", "frmStore not found.");
			return false;
		}
		frmStore->evtEntrustMouseEvent = _evtStoreFormMouseEvent;
		frmStore->evtClose             = _evtStoreFormClose;

		// 
		trvStore = dynamic_cast<CTreeView*>(frmStore->Find("trvStore"));
		if(!trvStore)
		{
			ToLogService("common", "frmStore:trvStore not found. ");
			return false;
		}
		trvStore->SetSelectColor( 0 );
		trvStore->evtMouseDown = _evtStoreTreeMouseClick;

		// 
		labMoneyLeft = dynamic_cast<CLabelEx*>(frmStore->Find("labMoneyLeft"));
		if(!labMoneyLeft)
		{
			ToLogService("common", "frmStore:labMoneyLeft not found. ");
			return false;
		}

		// 
		labBeanLeft = dynamic_cast<CLabelEx*>(frmStore->Find("labBeanLeft"));
		if(!labBeanLeft)
		{
			ToLogService("common", "frmStore:labBeanLeft not found. ");
			return false;
		}

		// 
		labMemberStyle = dynamic_cast<CLabelEx*>(frmStore->Find("labMemberStyle"));
		if(! labMemberStyle)
		{
			ToLogService("common", "frmStore:labMemberStyle not found. ");
			return false;
		}


		labNameTitle = dynamic_cast<CLabelEx*>(frmStore->Find("labNameTitle0"));
		if(!labNameTitle)
		{
			ToLogService("common", "frmStore:labNameTitle0 not found. ");
			return false;
		}

		labPriceTitle = dynamic_cast<CLabelEx*>(frmStore->Find("labPriceTitle"));
		if(!labPriceTitle)
		{
			ToLogService("common", "frmStore:labPriceTitle not found. ");
			return false;
		}

		labLeftTimeTitle = dynamic_cast<CLabelEx*>(frmStore->Find("labLeftTimeTitle"));
		if(!labLeftTimeTitle)
		{
			ToLogService("common", "frmStore:labLeftTimeTitle not found. ");
			return false;
		}

		labLeftNumTitle = dynamic_cast<CLabelEx*>(frmStore->Find("labLeftNumTitle"));
		if(!labLeftNumTitle)
		{
			ToLogService("common", "frmStore:labLeftNumTitle not found. ");
			return false;
		}




		// 
		labListPage = dynamic_cast<CLabelEx*>(frmStore->Find("labListPage"));
		if(!labListPage)
		{
			ToLogService("common", "frmStore:labListPage not found. ");
			return false;
		}

		// 
		btnLeftPage = dynamic_cast<CTextButton*>(frmStore->Find("btnLeftPage"));
		if(! btnLeftPage)
		{
			ToLogService("common", "frmStore:btnLeftPage not found. ");
			return false;
		}

		// 
		btnRightPage = dynamic_cast<CTextButton*>(frmStore->Find("btnRightPage"));
		if(! btnRightPage)
		{
			ToLogService("common", "frmStore:btnRightPage not found. ");
			return false;
		}

		// 
		btnTrade = dynamic_cast<CTextButton*>(frmStore->Find("btnTrade"));
		if(! btnTrade)
		{
			ToLogService("common", "frmStore:btnTrade not found. ");
			return false;
		}
		btnTrade->SetIsEnabled(false);

		// 
		btnTryon = dynamic_cast<CTextButton*>(frmStore->Find("btnTryon"));
		if(! btnTryon)
		{
			ToLogService("common", "frmStore:btnTryon not found. ");
			return false;
		}
		btnTryon->SetIsEnabled(false);

		btnViewAll = dynamic_cast<CTextButton*>(frmStore->Find("btnViewAll"));
		if(! btnViewAll)
		{
			ToLogService("common", "frmStore:btnViewAll not found. ");
			return false;
		}
		btnViewAll->SetIsEnabled(false);

		// 
		labTrade = dynamic_cast<CLabelEx*>(frmStore->Find("labTrade"));
		if(! labTrade)
		{
			ToLogService("common", "frmStore:labTrade not found. ");
			return false;
		}

		// 
		labTryon = dynamic_cast<CLabelEx*>(frmStore->Find("labTryon"));
		if(! labTryon)
		{
			ToLogService("common", "frmStore:labTryon not found. ");
			return false;
		}

		// 
		labViewAll = dynamic_cast<CLabelEx*>(frmStore->Find("labViewAll"));
		if(! labViewAll)
		{
			ToLogService("common", "frmStore:labViewAll not found. ");
			return false;
		}

		// 
		memStoreHelp = dynamic_cast<CMemo*>(frmStore->Find("memStoreHelp"));
		if(! memStoreHelp)
		{
			ToLogService("common", "frmStore:memStoreHelp not found. ");
			return false;
		}


		// 
		//CLabelEx* labNotice = dynamic_cast<CLabelEx*>(frmStore->Find("labNotice"));
		//if(labNotice)
		//{
		//	labNotice->SetIsFlash(true);
		//}

		// 
		imgBackGround10 = dynamic_cast<CImage*>(frmStore->Find("imgBackGround10"));
		if(! imgBackGround10)
		{
			ToLogService("common", "frmStore:imgBackGround10 not found. ");
			return false;
		}

		// 
		char szName[32] = {0};
		for(int i = 0; i < STORE_PAGE_SIZE; ++i)
		{
			sprintf(szName, "labName_%d", i);
			m_stStoreGui[i].labName = dynamic_cast<CLabelEx*>(frmStore->Find(szName));
			if(! m_stStoreGui[i].labName)
			{
				ToLogService("common", "frmStore:{} not found. ", szName);
				return false;
			}

			sprintf(szName, "labPrice_%d", i);
			m_stStoreGui[i].labPrice = dynamic_cast<CLabelEx*>(frmStore->Find(szName));
			if(! m_stStoreGui[i].labPrice)
			{
				ToLogService("common", "frmStore:{} not found. ", szName);
				return false;
			}

			sprintf(szName, "labLeftTime_%d", i);
			m_stStoreGui[i].labLeftTime = dynamic_cast<CLabelEx*>(frmStore->Find(szName));
			if(! m_stStoreGui[i].labLeftTime)
			{
				ToLogService("common", "frmStore:{} not found. ", szName);
				return false;
			}

			sprintf(szName, "labLeftNum_%d", i);
			m_stStoreGui[i].labLeftNum = dynamic_cast<CLabelEx*>(frmStore->Find(szName));
			if(! m_stStoreGui[i].labLeftNum)
			{
				ToLogService("common", "frmStore:{} not found. ", szName);
				return false;
			}

			sprintf(szName, "labRemark_%d", i);
			m_stStoreGui[i].labRemark = dynamic_cast<CLabelEx*>(frmStore->Find(szName));
			if(! m_stStoreGui[i].labRemark)
			{
				ToLogService("common", "frmStore:{} not found. ", szName);
				return false;
			}

			sprintf(szName, "labRightClickView_%d", i);
			m_stStoreGui[i].labRightClickView = dynamic_cast<CLabelEx*>(frmStore->Find(szName));
			if(! m_stStoreGui[i].labRightClickView)
			{
				ToLogService("common", "frmStore:{} not found. ", szName);
				return false;
			}

			sprintf(szName, "cmdStore_%d", i);
			m_stStoreGui[i].cmdStore = dynamic_cast<COneCommand*>(frmStore->Find(szName));
			if(! m_stStoreGui[i].cmdStore)
			{
				ToLogService("common", "frmStore:{} not found. ", szName);
				return false;
			}
			m_stStoreGui[i].cmdStore->SetIsDrag(false);	// 

			sprintf(szName, "imgSquare_%d", i);
			m_stStoreGui[i].imgSquare = dynamic_cast<CImage*>(frmStore->Find(szName));
			if(! m_stStoreGui[i].imgSquare)
			{
				ToLogService("common", "frmStore:{} not found. ", szName);
				return false;
			}

			sprintf(szName, "btnBlue_%d", i);
			m_stStoreGui[i].btnBlue = dynamic_cast<CTextButton*>(frmStore->Find(szName));
			if(! m_stStoreGui[i].btnBlue)
			{
				ToLogService("common", "frmStore:{} not found. ", szName);
				return false;
			}
			m_stStoreGui[i].btnBlue->evtMouseRClick  = _evtStoreListMouseRClick;
			m_stStoreGui[i].btnBlue->evtMouseDBClick = _evtStoreListMouseDBClick;

			sprintf(szName, "imgCutLine_%d", i);
			m_stStoreGui[i].imgCutLine = dynamic_cast<CImage*>(frmStore->Find(szName));
			if(! m_stStoreGui[i].imgCutLine)
			{
				ToLogService("common", "frmStore:{} not found. ", szName);
				return false;
			}

			sprintf(szName, "imgHot_%d", i);
			m_stStoreGui[i].imgHot = dynamic_cast<CImage*>(frmStore->Find(szName));
			if(! m_stStoreGui[i].imgHot)
			{
				ToLogService("common", "frmStore:{} not found. ", szName);
				return false;
			}

			sprintf(szName, "imgNew_%d", i);
			m_stStoreGui[i].imgNew = dynamic_cast<CImage*>(frmStore->Find(szName));
			if(! m_stStoreGui[i].imgNew)
			{
				ToLogService("common", "frmStore:{} not found. ", szName);
				return false;
			}

			sprintf(szName, "imgBlue_%d", i);
			m_stStoreGui[i].imgBlue = dynamic_cast<CImage*>(frmStore->Find(szName));
			if(! m_stStoreGui[i].imgBlue)
			{
				ToLogService("common", "frmStore:{} not found. ", szName);
				return false;
			}
			
		} // end for(0..4)
		ClearStoreTreeNode();
		ClearStoreItemList();


		//
		// 
		//
		frmStoreLoad = CFormMgr::s_Mgr.Find("frmStoreLoad");
		if(!frmStoreLoad) return false;
		frmStoreLoad->evtClose = _evtStoreLoadFormClose;

		proStoreLoad = dynamic_cast<CProgressBar*>(frmStoreLoad->Find("proStoreLoad"));
		if(!proStoreLoad) return false;
		proStoreLoad->evtTimeArrive = _evtProTimeArriveEvt;


		//
		// 
		//
		frmViewAll = CFormMgr::s_Mgr.Find("frmViewAll");
		if(! frmViewAll)
		{
			ToLogService("common", "frmViewAll not found.");
			return false;
		}
		frmViewAll->evtLost = _evtStoreViewAllLostEvent;

		memViewAll = dynamic_cast<CMemo*>(frmViewAll->Find("memViewAll"));
		if(! memViewAll)
		{
			ToLogService("common", "frmViewAll:memViewAll not found.");
			return false;
		}

		for(int i = 0; i < STORE_ITEM_COUNT; ++i)
		{
			sprintf(szName, "imgSquareViewAll_%d", i);
			imgSquareViewAll[i] = dynamic_cast<CImage*>(frmViewAll->Find(szName));
			if(! imgSquareViewAll[i])
			{
				ToLogService("common", "frmStore:{} not found. ", szName);
				return false;
			}

			sprintf(szName, "cmdSquareIcon_%d", i);
			cmdSquareIcon[i] = dynamic_cast<COneCommand*>(frmViewAll->Find(szName));
			if(! cmdSquareIcon[i])
			{
				ToLogService("common", "frmStore:{} not found. ", szName);
				return false;
			}

			cmdSquareIcon[i]->SetIsDrag(false);
		}


		//
		// 
		//
		frmTryon = mgr.Find("frmTryon");
		if(!frmTryon)
		{
			ToLogService("common", "frmTryon not found.");
			return false;
		}
		frmTryon->evtClose = _evtTryonFormClose;

		// 3D
		ui3dplayer = dynamic_cast<C3DCompent*>(frmTryon->Find("ui3dplayer"));
		if(! ui3dplayer)
		{
			ToLogService("common", "frmStore:ui3dplayer not found. ");
			return false;
		}
		ui3dplayer->SetRenderEvent(_evtChaTryonRenderEvent);

		CTextButton* btnClearAll = dynamic_cast<CTextButton*>(frmTryon->Find("btnClearAll"));
		if(btnClearAll)
		{
			btnClearAll->evtMouseClick	= _evtChaEquipClearAll;
		}

		CTextButton* btnTurnleft3d = dynamic_cast<CTextButton*>(frmTryon->Find("btnTurnleft3d"));
		if(btnTurnleft3d)
		{
			btnTurnleft3d->evtMouseClick        = _evtChaLeftRotate;
			btnTurnleft3d->evtMouseDownContinue = _evtChaLeftContinueRotate;
		}

		CTextButton* btnTurnright3d = dynamic_cast<CTextButton*>(frmTryon->Find("btnTurnright3d"));
		if(btnTurnright3d)
		{
			btnTurnright3d->evtMouseClick        = _evtChaRightRotate;
			btnTurnright3d->evtMouseDownContinue = _evtChaRightContinueRotate;
		}

		return true;
	}


	void CStoreMgr::CloseForm()
	{
		//if(frmStore)
		//{
		//	frmStore->SetIsShow(false);
		//}
	}


	void CStoreMgr::SwitchMap()
	{
		m_pCurrMainCha = nullptr; // to fix when form close by ticket to avoid crash @mothannakh
		ShowStoreForm(false);
	}


	void CStoreMgr::FrameMove(DWORD dwTime)
	{
		if(frmStoreLoad->GetIsShow())
		{
			if(GetTickCount() - _dwDarkScene > STORE_OPEN_TIMEOUT * 1000)
			{
				DarkScene(false);
				_dwDarkScene = 0;

				ShowStoreLoad(false);
			}
		}
	}


	void CStoreMgr::ShowTempKitbag(bool bShow)
	{
		if(bShow)
		{
			if(frmStore->GetIsShow())
			{
				frmTempBag->SetPos(frmStore->GetX2(), frmStore->GetHeight() - frmTempBag->GetHeight());
				frmTempBag->SetIsDrag(false);
				frmTempBag->Refresh();
				frmTempBag->SetIsShow(true);
			}
			else	//	Modify by alfred.shi 20080902	begin
			{
				CForm* frmEquip = g_stUIEquip.GetItemForm();
				frmEquip->SetPos(100, 100);
				frmEquip->Refresh();
				frmEquip->SetIsShow(!frmTempBag->GetIsShow());

				frmTempBag->SetPos(frmEquip->GetX2(), frmEquip->GetY());
				frmTempBag->Refresh();
				frmTempBag->SetIsShow(!frmTempBag->GetIsShow());
				
			}	//	End
		}
		else
		{
			frmTempBag->SetIsShow(false);
		}
	}


	// 
	void CStoreMgr::OpenStoreAsk()
	{
		//CBoxMgr::ShowSelectBox(_evtStoreOpenCheckEvent, GetLanguageString(858), true);
		CS_StoreOpenAsk("");
	}


	void CStoreMgr::ShowStoreForm(bool bShow)
	{
		g_stUIDoublePwd.CloseAllForm();
		CFormMgr::s_Mgr.SetEnableHotKey(HOTKEY_STORE, ! bShow);		// 
		_SetIsShowCozeForm(! bShow);

		// Loading...
		ShowStoreLoad(false);

		if(bShow)
		{
			StoreTreeRefresh();
			_SetIsShowUserInfo(false);

			//  ESC 
			//frmTempBag->SetIsEscClose(false);
			frmStore->SetIsEscClose(true);
			frmStore->SetPos(0, 0);
			frmStore->Refresh();
			frmStore->SetIsShow(true);

			if(! frmTryon->GetIsShow())
			{
				// 
				ShowTryonForm(true);
			}

			// 
			ShowTempKitbag(true);

			CUIInterface::MainChaMove();
		}
		else
		{
			frmStore->SetIsShow(false);

			ClearStoreTreeNode();
			ClearStoreItemList();
		}
	}


	void CStoreMgr::ShowStoreLoad(bool bShow)
	{
		if(bShow)
		{
			proStoreLoad->SetRange(0.0f, STORE_OPEN_TIMEOUT * 1.0f);
			proStoreLoad->SetPosition(0.0f);

			frmStoreLoad->SetPos((g_pGameApp->GetWindowWidth()  - frmStoreLoad->GetWidth())  >> 1,
								 (g_pGameApp->GetWindowHeight() - frmStoreLoad->GetHeight()) >> 1);
			frmStoreLoad->Refresh();
			frmStoreLoad->ShowModal();

			_dwDarkScene = GetTickCount();
		}
		else
		{
			frmStoreLoad->SetIsShow(false);
		}

		DarkScene(bShow);
	}


	void CStoreMgr::ShowViewAllForm(bool bShow)
	{
		if(! bShow)
		{
			frmViewAll->SetIsShow(false);
			return;
		}

		if(m_nCurSel < 0 || m_nCurSel >= STORE_PAGE_SIZE)
		{
			return;
		}

		for(int i = 0; i < STORE_ITEM_COUNT; ++i)
		{
			imgSquareViewAll[i]->SetIsShow(false);
			cmdSquareIcon[i]->SetIsShow(false);
			cmdSquareIcon[i]->DelCommand();

			if(m_stStoreInfo[m_nCurSel].comItemInfo[i].itemID > 0)
			{
				CItemRecord* pInfo = GetItemRecordInfo(m_stStoreInfo[m_nCurSel].comItemInfo[i].itemID);
				if(! pInfo) continue;

				CItemCommand* pItem = new CItemCommand(pInfo);
				SItemGrid& oItemGrid = pItem->GetData();

				oItemGrid.sNum = m_stStoreInfo[m_nCurSel].comItemInfo[i].itemNum;
				for(int j = 0; j < defITEM_INSTANCE_ATTR_NUM; ++j)
				{
					oItemGrid.sInstAttr[j][0] = m_stStoreInfo[m_nCurSel].comItemInfo[i].itemAttrID[j];
					oItemGrid.sInstAttr[j][1] = m_stStoreInfo[m_nCurSel].comItemInfo[i].itemAttrVal[j];
				}

				// 
				unsigned long ulForgeP = oItemGrid.GetDBParam(enumITEMDBP_FORGE);
				short sFlute = (short)(m_stStoreInfo[m_nCurSel].comItemInfo[i].itemFlute);
				short sHole  = (short)(ulForgeP / 1000000000);
				ulForgeP = ulForgeP + (sFlute - sHole) * 1000000000;
				oItemGrid.SetDBParam(enumITEMDBP_FORGE, (long)ulForgeP);

				cmdSquareIcon[i]->AddCommand(pItem);
				cmdSquareIcon[i]->SetIsShow(true);
				imgSquareViewAll[i]->SetIsShow(true);
			}
		}

		memViewAll->SetCaption(m_stStoreInfo[m_nCurSel].comRemark);
		memViewAll->ProcessCaption();
		memViewAll->Refresh();

		// 
		frmViewAll->SetPos((g_pGameApp->GetWindowWidth()  - frmViewAll->GetWidth())  >> 1 , 
						   (g_pGameApp->GetWindowHeight() - frmViewAll->GetHeight()) >> 1);
		frmViewAll->Refresh();
		frmViewAll->SetIsShow(true);
	}


	// 
	void CStoreMgr::ShowStoreWebPage()
	{
	}


	// 
	void CStoreMgr::ShowTryonForm(bool bShow)
	{
		if(bShow)
		{
			CCharacter* pMainCha = g_stUIBoat.GetHuman();
			if(! pMainCha)
				return;

			if(false == frmTryon->GetIsShow())
			{
				// 
				m_nChaRotate = 0;

				m_pCurrMainCha = g_pGameApp->GetCurScene()->AddCharacter(pMainCha->getTypeID());
				m_pCurrMainCha->SetIsForUI(true);
				m_pCurrMainCha->SetIsShowShadow(false);

				m_sLookInfo.SLook = pMainCha->GetPart();
				m_sLookInfo.chSynType = enumSYN_LOOK_SWITCH;

				m_sCurLookInfo = m_sLookInfo;

				m_isFight = m_pCurrMainCha->GetIsFight();
				m_pCurrMainCha->FightSwitch(true);	// 
				_ChangeChaPart(m_sCurLookInfo);

				frmTryon->SetPos(frmStore->GetX2(), frmStore->GetY());
				frmTryon->Refresh();
				frmTryon->SetIsShow(true);
				return;
			}

			//
			// 
			bool bAllowEquip = false;
			DWORD dwBodyType = m_pCurrMainCha->GetDefaultChaInfo()->lID;

			if(0 <= m_nCurSel && m_nCurSel < STORE_PAGE_SIZE)
			{
				bool leftHand = true;
				for(int i = 5; i >= 0; --i){
					if(m_stStoreInfo[m_nCurSel].comItemInfo[i].itemID <= 0)
						continue;

					//CItemRecord* pItem = pItemCmd->GetItemInfo();
					CItemRecord* pItem = GetItemRecordInfo(m_stStoreInfo[m_nCurSel].comItemInfo[i].itemID);
					if(! pItem)
						continue;

					short sType = pItem->sType;
					if(1 <= sType && sType <= 11)	// 
					{
						
						if(! pItem->IsAllowEquip(dwBodyType)){
							continue;
						}
						
						int slot{};
						int unequip{};
						int appslot{};
						
						if (sType == enumItemTypeSword){
							if (m_sCurLookInfo.SLook.SLink[enumEQUIP_LHAND].sID == enumEQUIP_BOTH_HAND)
							{
								unequip = enumEQUIP_LHAND;
							}

							if(!leftHand){
								appslot = enumEQUIP_SWORD2APP;
								slot = enumEQUIP_LHAND;
							}
							else
							{

								appslot = enumEQUIP_SWORD1APP;
								slot = enumEQUIP_RHAND;
								leftHand = false;
								if (m_sCurLookInfo.SLook.SLink[enumEQUIP_LHAND].sID != 0)
								{
									CItemRecord* pItem2 = GetItemRecordInfo(m_sCurLookInfo.SLook.SLink[enumEQUIP_LHAND].sID);
									if (pItem2)
									{
										if (pItem2->sType != enumItemTypeSword && pItem2->sType != enumItemTypeShield) {
											unequip = enumEQUIP_LHAND;
										}
									}
								}
							}
						}else if (sType == enumItemTypeGlave){
							appslot = enumEQUIP_GREATSWORDAPP;
							slot = enumEQUIP_LHAND;
							unequip = enumEQUIP_RHAND;
						}else if (sType == enumItemTypeBow){
							appslot = enumEQUIP_BOWAPP;
							slot = enumEQUIP_LHAND;
							unequip = enumEQUIP_RHAND;
						}else if (sType == enumItemTypeHarquebus){
							appslot = enumEQUIP_GUNAPP;
							slot = enumEQUIP_RHAND;
							unequip = enumEQUIP_LHAND;
						}else if (sType == enumItemTypeStylet){
							appslot = enumEQUIP_DAGGERAPP;
							slot = enumEQUIP_LHAND;
							unequip = enumEQUIP_RHAND;
						}else if (sType == enumItemTypeCosh){
							appslot = enumEQUIP_STAFFAPP;
							slot = enumEQUIP_LHAND;
							unequip = enumEQUIP_RHAND;
						}else if (sType == enumItemTypeShield){
							appslot = enumEQUIP_SHIELDAPP;
							slot = enumEQUIP_LHAND;
							if(m_sCurLookInfo.SLook.SLink[enumEQUIP_RHAND].sID != 0){
								CItemRecord* pItem2 = GetItemRecordInfo(m_sCurLookInfo.SLook.SLink[enumEQUIP_RHAND].sID);
								if(pItem2->sType != enumItemTypeSword){
									unequip = enumEQUIP_RHAND;
								}
							}
						}
						
						if(appslot){
							m_sCurLookInfo.SLook.SLink[appslot].sID  = 0;
							m_sCurLookInfo.SLook.SLink[appslot].sNum = 0;
						}
						
						if(unequip){
							m_sCurLookInfo.SLook.SLink[unequip].sID  = 0;
							m_sCurLookInfo.SLook.SLink[unequip].sNum = 0;
						}
							
						if(slot){
							m_sCurLookInfo.SLook.SLink[slot].sID  = (short)pItem->lID;
							m_sCurLookInfo.SLook.SLink[slot].sNum = 1;
							bAllowEquip = true;
						}
						
					}
					else if(sType == 20)	// 
					{
						if(! pItem->IsAllowEquip(dwBodyType))
						{
							continue;
						}
						m_sCurLookInfo.SLook.SLink[enumEQUIP_HEAD] = (short)pItem->lID;
						m_sCurLookInfo.SLook.SLink[enumEQUIP_HEAD].sNum = 1;
						m_sCurLookInfo.SLook.SLink[enumEQUIP_HEADAPP] = 0;
						m_sCurLookInfo.SLook.SLink[enumEQUIP_HEADAPP].sNum = 0;
						bAllowEquip = true;
					}			
					else if(sType == 22 || sType == 27)	// 
					{
						if(! pItem->IsAllowEquip(dwBodyType))
						{
							continue;
						}
						m_sCurLookInfo.SLook.SLink[enumEQUIP_BODY] = (short)pItem->lID;
						m_sCurLookInfo.SLook.SLink[enumEQUIP_BODY].sNum = 1;
						m_sCurLookInfo.SLook.SLink[enumEQUIP_BODYAPP] = 0;
						m_sCurLookInfo.SLook.SLink[enumEQUIP_BODYAPP].sNum = 0;
						bAllowEquip = true;
					}
					else if(sType == 23)	// 
					{
						if(! pItem->IsAllowEquip(dwBodyType))
						{
							continue;
						}
						m_sCurLookInfo.SLook.SLink[enumEQUIP_GLOVE] = (short)pItem->lID;
						m_sCurLookInfo.SLook.SLink[enumEQUIP_GLOVE].sNum = 1;
						m_sCurLookInfo.SLook.SLink[enumEQUIP_GLOVEAPP] = 0;
						m_sCurLookInfo.SLook.SLink[enumEQUIP_GLOVEAPP].sNum = 0;
						bAllowEquip = true;
					}
					else if(sType == 24)	// 
					{
						if(! pItem->IsAllowEquip(dwBodyType))
						{
							continue;
						}
						m_sCurLookInfo.SLook.SLink[enumEQUIP_SHOES] = (short)pItem->lID;
						m_sCurLookInfo.SLook.SLink[enumEQUIP_SHOES].sNum = 1;
						m_sCurLookInfo.SLook.SLink[enumEQUIP_SHOESAPP] = 0;
						m_sCurLookInfo.SLook.SLink[enumEQUIP_SHOESAPP].sNum = 0;
						bAllowEquip = true;
					}
				}
			}

			if(bAllowEquip)
			{
				//NetChangeChaPart(m_pCurrMainCha, m_sCurLookInfo);
				_ChangeChaPart(g_stUIStore.m_sCurLookInfo);

				m_pCurrMainCha->PlayPose(1, PLAY_LOOP_SMOOTH );
				m_pCurrMainCha->FightSwitch(true);	// 
			}
			else
			{
				g_pGameApp->MsgBox("%s", GetLanguageString(882).c_str());
			}
		}
		else
		{
			frmTryon->SetIsShow(false);
		}
	}


	// 
	void CStoreMgr::ShowAlphaMatteForm(bool bShow)
	{
		CForm* frmAlphaMatte = CFormMgr::s_Mgr.Find("frmAlphaMatte");
		if(frmAlphaMatte)
		{
			frmAlphaMatte->SetSize(g_pGameApp->GetWindowWidth(), g_pGameApp->GetWindowHeight());
			frmAlphaMatte->SetPos(0, 0);
			frmAlphaMatte->Refresh();

			frmAlphaMatte->SetIsShow(bShow);
		}
	}


	// 
	void CStoreMgr::DarkScene(bool bDark)
	{
		CForm* frmStoreDark = CFormMgr::s_Mgr.Find("frmStoreDark");
		if(frmStoreDark) frmStoreDark->SetIsShow(bDark);

		_dwDarkScene = bDark ? GetTickCount() : 0;
	}


	// 
	bool CStoreMgr::PopFromTempKitbag(CGoodsGrid& rkDrag, CGoodsGrid& rkSelf, int nGridID, CCommandObj& rkItem)
	{
		g_stUIStore.m_NetTempKitbag.sSrcGridID = rkDrag.GetDragIndex();
		g_stUIStore.m_NetTempKitbag.sTarGridID = nGridID;

		// 
		CItemCommand* pkItemCmd = dynamic_cast<CItemCommand*>(&rkItem);
		if (!pkItemCmd)	return false;

		if ( pkItemCmd->GetItemInfo()->GetIsPile() && pkItemCmd->GetTotalNum() > 1 )
		{
			// 
			m_pkNumberBox = 
				g_stUIBox.ShowNumberBox( _evtDragItemsEvent, pkItemCmd->GetTotalNum(), GetLanguageString(442).c_str(), false);

			if (m_pkNumberBox->GetNumber() < pkItemCmd->GetTotalNum())
				return false;
			else
				return true;
		}
		else
		{
			// 
			g_stUIStore.m_NetTempKitbag.sSrcNum = 1;
			CS_BeginAction(g_stUIBoat.GetHuman(), enumACTION_KITBAGTMP_DRAG, (void*)&(g_stUIStore.m_NetTempKitbag));
			return true;
		}

		return true;
	}


	void CStoreMgr::_evtDragItemsEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
	{
		if(nMsgType != CForm::mrYes) 
			return;

		int num =  g_stUIStore.m_pkNumberBox->GetNumber();
		if ( num > 0 )
		{
			g_stUIStore.m_NetTempKitbag.sSrcNum = num;
			CS_BeginAction(g_stUIBoat.GetHuman(), enumACTION_KITBAGTMP_DRAG, (void*)&(g_stUIStore.m_NetTempKitbag));
		}
	}


	// 
	void CStoreMgr::AddStoreTreeNode(long nParentID, long nID, const char* szCaption)
	{
		m_mapNode[nID] = string(szCaption);

		if (0 == nParentID)
		{
			constexpr auto frame{ 0 };
			CNoteGraph* item = new CNoteGraph(frame);
			*item->GetImage() = *trvStore->GetNodeImage();
			item->SetString(szCaption);
			item->SetTextX(0);
			item->SetTextY(3);

			const int nWidth = trvStore->GetWidth() - trvStore->GetScroll()->GetWidth() - 28;
			constexpr auto nHeight{16 + 3};
			auto obj = new CTreeGridNode(trvStore, item);
			obj->SetIsExpand(false);
			obj->SetColMaxNum(1);
			obj->GetItem()->SetColor(COLOR_BLACK);
			obj->GetUpImage()->UnLoadImage();
			obj->GetDownImage()->UnLoadImage();
			obj->SetUnitSize(nWidth, nHeight);

			trvStore->GetRootNode()->AddNode(obj);
			return;
		}

		MapNodeIter iterNode = m_mapNode.find(nParentID);
		if (iterNode != m_mapNode.end())
		{
			CTreeGridNode* pGridNode = dynamic_cast<GUI::CTreeGridNode*>(trvStore->GetRootNode()->FindNode(iterNode->second.c_str()));
			if (pGridNode)
			{
				CItem* pItem = new CItem();
				pItem->SetColor(COLOR_BLACK);
				pItem->SetString(szCaption);

				pGridNode->AddItem(pItem);
			}
		}
	}


	// 
	void CStoreMgr::AddStoreItemInfo(long nSeq, long nID, const char* szName, long nPrice, const char* szRemark, bool isHot, long nTime, long nRemainNum, long nRemainTime)
	{
		if(0 > nSeq || nSeq >= STORE_PAGE_SIZE)
		{
			return;
		}

		char szTemp[128] = {0};
		m_stStoreGui[nSeq].labName->SetCaption(szName);		// 
		m_stStoreGui[nSeq].labName->SetIsShow(true);

		m_stStoreGui[nSeq].labPrice->SetCaption(StringSplitNum(nPrice, 3, ','));	// 
		m_stStoreGui[nSeq].labPrice->SetIsShow(true);

		sprintf(szTemp, "%s", StringLimit(szRemark, 25).c_str());
		m_stStoreGui[nSeq].labRemark->SetCaption(strlen(szRemark) > 0 ? szTemp : szRemark);  // 
		m_stStoreGui[nSeq].labRemark->SetIsShow(true);

		sprintf(szTemp, "Quantity: %d", nRemainNum); // "%d"
		m_stStoreGui[nSeq].labLeftNum->SetCaption(nRemainNum >= 0 ? szTemp : "No Limit");
		m_stStoreGui[nSeq].labLeftNum->SetIsShow(true);

		strncpy_s(szTemp, sizeof(szTemp), SafeVFormat(GetLanguageString(911), nRemainTime).c_str(), _TRUNCATE);// "%d"
		m_stStoreGui[nSeq].labLeftTime->SetCaption(nRemainTime >= 0 ? szTemp : "No Limit");
		m_stStoreGui[nSeq].labLeftTime->SetIsShow(true);

		m_stStoreGui[nSeq].btnBlue->SetIsShow(true);			// 
		m_stStoreGui[nSeq].labRightClickView->SetIsShow(true);
		
		//m_stStoreGui[nSeq].labLeftNum->SetIsShow(false);
		//m_stStoreGui[nSeq].labLeftTime->SetIsShow(false);

		if(isHot)
		{
			// 
			m_stStoreGui[nSeq].imgHot->SetIsShow(true);
		}
		else
		{
			time_t now;
			time(& now);
			int nDay = ((long)(now) - nTime) / (24 * 3600 * 1000);

			if(nDay < 15)
			{
				// 
				m_stStoreGui[nSeq].imgNew->SetIsShow(true);
			}
		}

		m_stStoreInfo[nSeq].comID     = nID;
		m_stStoreInfo[nSeq].comExpire = nTime;
		m_stStoreInfo[nSeq].comPrice  = nPrice;
		m_stStoreInfo[nSeq].comTime   = nTime;
		m_stStoreInfo[nSeq].isHot     = isHot;

		m_stStoreInfo[nSeq].comExpire = nRemainNum;	// 
		m_stStoreInfo[nSeq].itemNum   = nRemainTime;// 

		strncpy(m_stStoreInfo[nSeq].comName, szName, 20);	// 
		m_stStoreInfo[nSeq].comName[21] = 0;

		strncpy(m_stStoreInfo[nSeq].comRemark, szRemark, 128);	// 
		m_stStoreInfo[nSeq].comRemark[129] = 0;
	}


	// 
	void CStoreMgr::AddStoreItemDetail(long nSeq, long nSubSeq, short sItemID, short sItemNum, short sFlute, short pItemAttrID[], short pItemAttrVal[])
	{
		if(0 > nSeq || nSeq >= STORE_PAGE_SIZE)
		{
			return;
		}

		if(0 > nSubSeq || nSubSeq >= 6)
		{
			return;
		}

		CItemRecord*  pInfo(NULL);
		CItemCommand* pItem(NULL);

		pInfo = GetItemRecordInfo(sItemID);
		if (!pInfo) return;

		// 
		m_stStoreInfo[nSeq].comItemInfo[nSubSeq].itemID = sItemID;
		m_stStoreInfo[nSeq].comItemInfo[nSubSeq].itemNum = sItemNum;
		m_stStoreInfo[nSeq].comItemInfo[nSubSeq].itemFlute = sFlute;
		memcpy(m_stStoreInfo[nSeq].comItemInfo[nSubSeq].itemAttrID,  pItemAttrID,  sizeof(short) * 5);
		memcpy(m_stStoreInfo[nSeq].comItemInfo[nSubSeq].itemAttrVal, pItemAttrVal, sizeof(short) * 5);

		if(nSubSeq != 0)	// 
		{
			return;
		}

		pItem = new CItemCommand(pInfo);
		//pItem->GetData().sNum = sItemNum;	// 
		pItem->SetOwnDefText("");			// 

		// 
		SItemGrid& oItemGrid = pItem->GetData();
		for(int i = 0; i < defITEM_INSTANCE_ATTR_NUM; ++i)
		{
			oItemGrid.sInstAttr[i][0] = pItemAttrID[i];
			oItemGrid.sInstAttr[i][1] = pItemAttrVal[i];
		}

		// 
		unsigned long ulForgeP = oItemGrid.GetDBParam(enumITEMDBP_FORGE);
		short sHole = (short)(ulForgeP / 1000000000);
		ulForgeP = ulForgeP + (sFlute - sHole) * 1000000000;
		oItemGrid.SetDBParam(enumITEMDBP_FORGE, (long)ulForgeP);

		m_stStoreGui[nSeq].cmdStore->AddCommand(pItem);
		m_stStoreGui[nSeq].cmdStore->SetIsShow(true);
		m_stStoreGui[nSeq].imgSquare->SetIsShow(true);
	}


	// 
	void CStoreMgr::AddStoreUserTreeNode(void)
	{
		//disabled help / management
		//AddStoreTreeNode(0, USER_NODEID, GetLanguageString(906));
		//AddStoreTreeNode(0, HELP_NODEID, GetLanguageString(921));
	}


	// 
	void CStoreMgr::SetStoreItemPage(long nCurPage, long nMaxPage)
	{
		m_nCurPage = nCurPage;
		m_nMaxPage = nMaxPage;

		if(m_nMaxPage > 0)
		{
			char szBuffer[32] = {0};
			sprintf(szBuffer, "%d/%d", nCurPage, nMaxPage);
			labListPage->SetCaption(szBuffer);

			btnLeftPage->SetIsShow(true);
			btnRightPage->SetIsShow(true);
			labListPage->SetIsShow(true);
		}
		else
		{
			btnLeftPage->SetIsShow(false);
			btnRightPage->SetIsShow(false);
			labListPage->SetIsShow(false);
		}
	}


	// 
	void CStoreMgr::SetStoreMoney(long nMoBean, long nRplMoney)
	{
		char szBuffer[32] = {0};

		if(nMoBean >= 0)
		{
			sprintf(szBuffer, "%ld", nMoBean);
			labBeanLeft->SetCaption(szBuffer);
		}

		if(nRplMoney >= 0)
		{
			sprintf(szBuffer, "%ld", nRplMoney);
			labMoneyLeft->SetCaption(szBuffer);
		}
	}


	// VIP
	void CStoreMgr::SetStoreVip(long nVip)
	{
		//CTextButton* btnToVip = dynamic_cast<CTextButton*>(frmStore->Find("btnToVIP"));
		if(nVip)
		{
			//if(btnToVip) btnToVip->SetIsEnabled(false);
			labMemberStyle->SetCaption(GetLanguageString(902).c_str()); //
		}
		else
		{
			//if(btnToVip) btnToVip->SetIsEnabled(true);
			labMemberStyle->SetCaption(GetLanguageString(903).c_str()); //
		}

		m_nVip = nVip;
	}


	// 
	void CStoreMgr::ClearStoreTreeNode()
	{
		m_mapNode.clear();
		trvStore->ClearAllNode();

		m_nCurClass = -1;
	}


	// 
	void CStoreMgr::ClearStoreItemList()
	{
		for(int i = 0; i < STORE_PAGE_SIZE; ++i)
		{
			m_stStoreGui[i].SetIsShow(false);
			m_stStoreInfo[i].Clear();
		}

		btnTrade->SetIsEnabled(false);
		btnTryon->SetIsEnabled(false);
		btnViewAll->SetIsEnabled(false);

		this->_SetIsShowUserInfo(false);

		m_nCurSel = -1;
	}


	// 
	void CStoreMgr::SetStoreBuyButtonEnable(bool b)
	{
		btnTrade->SetIsEnabled(b);
	}


	// ID
	int CStoreMgr::GetCurSelItemID()
	{
		if(0 <= m_nCurSel && m_nCurSel < STORE_PAGE_SIZE)
		{
			return m_stStoreInfo[m_nCurSel].comID;
		}

		return -1;
	}


	// 10 
	bool CStoreMgr::ResetLastOperate(bool bSilent)
	{
		static DWORD dwLast = 0;
		DWORD dwCurr = GetTickCount();
		if(dwCurr - dwLast < 10000)
		{
			if(! bSilent)
			{
				g_pGameApp->MsgBox("%s", GetLanguageString(895).c_str());	//
			}

			return false;
		}

		dwLast = dwCurr;
		return true;
	}


	// 
	void CStoreMgr::StoreTreeRefresh()
	{


		trvStore->Refresh();
	}


	// 
	void CStoreMgr::_SetIsShowUserInfo(bool bShow)
	{
		if(bShow)
		{
			labListPage->SetIsShow(! bShow);	// 
			btnLeftPage->SetIsShow(! bShow);	// 
			btnRightPage->SetIsShow(! bShow);	// 

			for(int i = 0; i < STORE_PAGE_SIZE; ++i)
			{
				m_stStoreGui[i].SetIsShow(! bShow);
			}
		}

		btnTrade->SetIsShow(! bShow);		// 
		btnTryon->SetIsShow(! bShow);		// 
		btnViewAll->SetIsShow(! bShow);
		labTrade->SetIsShow(! bShow);		// 
		labTryon->SetIsShow(! bShow);		// 
		labViewAll->SetIsShow(! bShow);

		labNameTitle->SetIsShow(! bShow);
		labPriceTitle->SetIsShow(! bShow);
		labLeftTimeTitle->SetIsShow(! bShow);
		labLeftNumTitle->SetIsShow(! bShow);

		memStoreHelp->SetIsShow(false);

		imgBackGround10->SetIsShow(! bShow);	// 
	}

	void CStoreMgr::_SetIsShowHelpInfo(bool bShow)
	{
		if(bShow)
		{
			labListPage->SetIsShow(! bShow);	// 
			btnLeftPage->SetIsShow(! bShow);	// 
			btnRightPage->SetIsShow(! bShow);	// 

			for(int i = 0; i < STORE_PAGE_SIZE; ++i)
			{
				m_stStoreGui[i].SetIsShow(! bShow);
			}
		}

		btnTrade->SetIsShow(! bShow);		// 
		btnTryon->SetIsShow(! bShow);		// 
		btnViewAll->SetIsShow(! bShow);
		labTrade->SetIsShow(! bShow);		// 
		labTryon->SetIsShow(! bShow);		// 
		labViewAll->SetIsShow(! bShow);

		labNameTitle->SetIsShow(! bShow);
		labPriceTitle->SetIsShow(! bShow);
		labLeftTimeTitle->SetIsShow(! bShow);
		labLeftNumTitle->SetIsShow(! bShow);

		_LoadStoreHelpInfo();
		memStoreHelp->SetIsShow(bShow);

		imgBackGround10->SetIsShow(bShow);	// 
	}

	void CStoreMgr::_SetIsShowCozeForm(bool bShow)
	{
		CForm* frmMainChat = _FindForm("frmMain800");
		if( !frmMainChat ) return;

		CList* lstOnSay = dynamic_cast<CList*>(frmMainChat->Find("lstOnSay"));
		if(lstOnSay) lstOnSay->SetIsShow(bShow);

		CList* lstOnSaySystem = dynamic_cast<CList*>(frmMainChat->Find("lstOnSaySystem"));
		if(lstOnSaySystem) lstOnSaySystem->SetIsShow(bShow);

		CDragTitle* drpTitle = dynamic_cast<CDragTitle*>(frmMainChat->Find("drpTitle"));
		if(drpTitle) drpTitle->SetIsShow(bShow);

		CDragTitle* drpTitleSystem = dynamic_cast<CDragTitle*>(frmMainChat->Find("drpTitleSystem"));
		if(drpTitleSystem) drpTitleSystem->SetIsShow(bShow);
	}

	void CStoreMgr::_RefreshStoreListHighLight()
	{
		for(int i = 0; i < CStoreMgr::STORE_PAGE_SIZE; ++i)
		{
			g_stUIStore.m_stStoreGui[i].imgBlue->SetIsShow(i == g_stUIStore.m_nCurSel ? true : false);
		}

		if(0 <= m_nCurSel && m_nCurSel < STORE_PAGE_SIZE)
		{
			g_stUIStore.btnTrade->SetIsEnabled(true);
			g_stUIStore.btnTryon->SetIsEnabled(true);
			g_stUIStore.btnViewAll->SetIsEnabled(true);
		}
		else
		{
			g_stUIStore.btnTrade->SetIsEnabled(false);
			g_stUIStore.btnTryon->SetIsEnabled(false);
			g_stUIStore.btnViewAll->SetIsEnabled(false);
		}
	}


	void CStoreMgr::_ShowTradeSelectBox()
	{
		if(0 <= m_nCurSel && m_nCurSel < STORE_PAGE_SIZE &&
			GetCurSelItemID() > 0)
		{
			if(0 == m_nVip && _IsCurSelVipNode())
			{
				g_pGameApp->MsgBox("%s", GetLanguageString(913).c_str()); // VIPVIP
				return;
			}

			char szTitle[256] = {0};
			sprintf(szTitle, "%s%s: %s\n%s: %s",
					GetLanguageString(857).c_str(),
					GetLanguageString(845).c_str(), g_stUIStore.m_stStoreGui[g_stUIStore.m_nCurSel].labName->GetCaption(),
					GetLanguageString(846).c_str(), g_stUIStore.m_stStoreGui[g_stUIStore.m_nCurSel].labPrice->GetCaption());

			CBoxMgr::ShowSelectBox(_evtTradeCheckEvent, szTitle, true);
		}
	}

	void CStoreMgr::_LoadStoreHelpInfo()
	{
		// 
		static bool IsFirst = true;
		if( IsFirst )
		{
			IsFirst = false;
			FILE* fp = fopen( "./scripts/txt/StoreHelp.tx", "rt" );
			if( fp )
			{
				if( fseek(fp, 0, SEEK_END) == 0 )
				{
					long size = ftell( fp );
					fseek(fp, 0, SEEK_SET);

					NET_HELPINFO Info;
					memset( &Info, 0, sizeof(NET_HELPINFO) );
					fread(Info.szDesp, size > HELPINFO_DESPSIZE - 1 ? HELPINFO_DESPSIZE - 1 : size, 1, fp);

					memStoreHelp->SetCaption(Info.szDesp);
					memStoreHelp->ProcessCaption();
					memStoreHelp->Refresh();
				}
				fclose( fp );
			}
			return;
		}
	}

	void CStoreMgr::_ChangeChaPart(stNetLookInfo& SLookInfo)
	{
		if( m_pCurrMainCha && SLookInfo.chSynType==enumSYN_LOOK_SWITCH )
		{
			if ( SLookInfo.SLook.sTypeID!=0 && m_pCurrMainCha->getTypeID()!=SLookInfo.SLook.sTypeID )
			{		
				if( SLookInfo.SLook.sTypeID!=0 && SLookInfo.SLook.sTypeID!=m_pCurrMainCha->getTypeID() ) 
					m_pCurrMainCha->ReCreate( SLookInfo.SLook.sTypeID );
			}

			m_pCurrMainCha->UpdataFace( SLookInfo.SLook );
		}
	}

	bool CStoreMgr::_IsCurSelVipNode(void)
	{
		MapNodeIter it = g_stUIStore.m_mapNode.find(m_nCurClass);
		if(it != m_mapNode.end())
		{
			string strName = it->second;
			if(strName.size() > 3)
			{
				strName = strName.substr(0, 3);
				if(0 == _stricmp(strName.c_str(), GetLanguageString(902).c_str())) //
				{
					return true;
				}
			}
		}

		return false;
	}


	// 
	void CStoreMgr::_evtStoreTreeMouseClick(CGuiData *pSender, int x, int y, DWORD key)
	{
		CItemObj* pSelItem = g_stUIStore.trvStore->GetHitItem(x, y);
		if(pSelItem)
		{
			string strItem = pSelItem->GetString();
			for(MapNodeIter it = g_stUIStore.m_mapNode.begin(); it != g_stUIStore.m_mapNode.end(); ++it)
			{
				if(it->second == strItem)
				{
					g_stUIStore.m_nCurClass = it->first;
					g_stUIStore.m_nCurPage  = 1;

					if(strItem == GetLanguageString(906))	// 
					{
						g_stUIStore._SetIsShowUserInfo(true);
					}
					else if(strItem == GetLanguageString(921))	// 
					{
						g_stUIStore._SetIsShowHelpInfo(true);
					}
					else	// 
					{
						g_stUIStore._SetIsShowUserInfo(false);
						CS_StoreListAsk(g_stUIStore.m_nCurClass, (short)g_stUIStore.m_nCurPage, STORE_PAGE_SIZE);
					}
					break;
				}
			}

			bool bRootNode = false;
			CTreeNodeRoot* pRoot = g_stUIStore.trvStore->GetRootNode();
			if(pRoot)
			{
				for(int i = 0; i < pRoot->GetChildCount(); ++i)
				{
					CTreeNodeObj* pChild = pRoot->GetChildNode(i);
					if(0 == strcmp(pChild->GetCaption(), strItem.c_str()))
					{
						bRootNode = true;
						break;
					}
				}

				if(bRootNode)
				{
					for(int i = 0; i < pRoot->GetChildCount(); ++i)
					{
						CTreeNodeObj* pChild = pRoot->GetChildNode(i);
						if(strcmp(pChild->GetCaption(), strItem.c_str()))
						{
							pChild->SetIsExpand(false);	// 
						}
						else
						{
							pChild->SetIsExpand(true);
						}
					}
					g_stUIStore.trvStore->Refresh();
				}
			}
		}
	}


	// 
	void CStoreMgr::_evtStoreFormMouseEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
	{
		string strName = pSender->GetName();
		if(strName == "btnLeftPage")	// 
		{
			if(g_stUIStore.m_nCurPage > 1 && g_stUIStore.m_nCurClass > 0)
			{
				CS_StoreListAsk(g_stUIStore.m_nCurClass, (short)g_stUIStore.m_nCurPage - 1, STORE_PAGE_SIZE);
			}
		}
		else if(strName == "btnRightPage")	// 
		{
			if(g_stUIStore.m_nCurPage < g_stUIStore.m_nMaxPage && g_stUIStore.m_nCurClass > 0)
			{
				CS_StoreListAsk(g_stUIStore.m_nCurClass, (short)g_stUIStore.m_nCurPage + 1, STORE_PAGE_SIZE);
			}
		}
		else if(strName == "btnTrade")	// 
		{
			g_stUIStore._ShowTradeSelectBox();
		}
		else if(strName == "btnTryon")	// 
		{
			g_stUIStore.ShowTryonForm(true);
		}
		else if(strName == "btnViewAll")	// 
		{
			g_stUIStore.ShowViewAllForm(true);
		}
		else if(strName == "btnReceiveMoney")	// 
		{
			if(g_stUIStore.imgBackGround10->GetIsShow()) return;
			g_stUIStore.m_pkExchangeNum = 
				g_stUIBox.ShowNumberBox( _evtExchangeEvent, -1, GetLanguageString(904).c_str(), false); //
		}
		//else if(strName == "btnToVIP")	// VIP
		//{
		//	if(g_stUIStore.imgBackGround10->GetIsShow()) return;
		//	CBoxMgr::ShowSelectBox(_evtStoreToVipEvent, GetLanguageString(915), true); // VIP
		//}
		else if(strName == "btnReceiveMoDou") //
		{
			if(g_stUIStore.imgBackGround10->GetIsShow()) return;

			std::string strURL = GetLanguageString(939);
			ShellExecute(0, "open", strURL.c_str(), NULL, NULL, SW_SHOW);
		}
		else if(strName.size() > 0 && strName.substr(0, 8) == "btnBlue_" &&
				'0' <= strName[strName.size() - 1] && strName[strName.size() - 1] <= '0' + CStoreMgr::STORE_PAGE_SIZE)	// 
		{
			g_stUIStore.m_nCurSel = strName[strName.size() - 1] - '0';
			g_stUIStore._RefreshStoreListHighLight();
		}
	}

	void CStoreMgr::_evtStoreListMouseRClick(CGuiData *pSender, int x, int y, DWORD key)
	{
		string strName = pSender->GetName();
		if(strName.size() > 0 && strName.substr(0, 8) == "btnBlue_" &&
				'0' <= strName[strName.size() - 1] && strName[strName.size() - 1] <= '0' + CStoreMgr::STORE_PAGE_SIZE)	// 
		{
			g_stUIStore.m_nCurSel = strName[strName.size() - 1] - '0';
			g_stUIStore._RefreshStoreListHighLight();

			g_stUIStore.ShowViewAllForm();		// 
		}
	}


	void CStoreMgr::_evtStoreListMouseDBClick(CGuiData *pSender, int x, int y, DWORD key)
	{
		string strName = pSender->GetName();
		if(strName.size() > 0 && strName.substr(0, 8) == "btnBlue_" &&
				'0' <= strName[strName.size() - 1] && strName[strName.size() - 1] <= '0' + CStoreMgr::STORE_PAGE_SIZE)	// 
		{
			g_stUIStore.m_nCurSel = strName[strName.size() - 1] - '0';
			g_stUIStore._RefreshStoreListHighLight();

			//g_stUIStore.ShowTryonForm();		// 
			g_stUIStore._ShowTradeSelectBox();	// 
		}
	}


	// 
	void CStoreMgr::_evtStoreOpenCheckEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
	{
		if(nMsgType != CForm::mrYes)
		{
			return;
		}

		g_stUIDoublePwd.SetType(CDoublePwdMgr::STORE_OPEN_ASK);
		g_stUIDoublePwd.ShowDoublePwdForm();
	}


	// VIP 
	void CStoreMgr::_evtStoreToVipEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
	{
		//if(nMsgType != CForm::mrYes)
		//{
		//	return;
		//}

		//if(! g_stUIStore.ResetLastOperate())
		//	return;

		//CS_StoreVIP(20, 1);
	}


	// 
	void CStoreMgr::_evtExchangeEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
	{
		if(nMsgType != CForm::mrYes)
		{
			return;
		}

		g_stUIStore.m_nExchangeNum = 0;
		int num =  g_stUIStore.m_pkExchangeNum->GetNumber();
		if ( num > 0 )
		{
			g_stUIStore.m_nExchangeNum = num;

			char szBuffer[MAX_PATH] ={0};
			strncpy_s(szBuffer, sizeof(szBuffer), SafeVFormat(GetLanguageString(905), g_stUIStore.m_nExchangeNum).c_str(), _TRUNCATE); // : %d
			CBoxMgr::ShowSelectBox(_evtExchangeCheckEvent, szBuffer, true);
		}
	}

	// 
	void CStoreMgr::_evtExchangeCheckEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
	{
		if(nMsgType != CForm::mrYes)
		{
			return;
		}

		if(! g_stUIStore.ResetLastOperate())
			return;

		CS_StoreChangeAsk(g_stUIStore.m_nExchangeNum);	// 
	}

	// 
	void CStoreMgr::_evtTradeCheckEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
	{
		if(nMsgType != CForm::mrYes)
		{
			return;
		}

		if(0 <= g_stUIStore.m_nCurSel && g_stUIStore.m_nCurSel < CStoreMgr::STORE_PAGE_SIZE &&
			g_stUIStore.GetCurSelItemID() > 0)
		{
			g_stUIStore.SetStoreBuyButtonEnable(false);
			CS_StoreBuyAsk(g_stUIStore.GetCurSelItemID());
		}
	}


	void CStoreMgr::_evtStoreFormClose(CForm* pForm, bool& IsClose)
	{
		// 
		CFormMgr::s_Mgr.SetEnableHotKey(HOTKEY_STORE, true);	// 

		//  ESC 
		//g_stUIStore.frmTempBag->SetIsEscClose(true);

		// 
		g_stUIStore.m_nCurSel = -1;

		// 
		g_stUIStore.ShowTryonForm(false);

		// 
		//g_stUIStore.frmTempBag->SetIsDrag(true);

		// 
		g_stUIStore._SetIsShowCozeForm(true);

		// 
		
		if(! g_stUIEquip.GetItemForm()->GetIsShow())
		{
			g_stUIStore.ShowTempKitbag(false);
		}

		CS_StoreClose();
	}


////////////////////////////////////////////////////////////////////////////////////////////////////


	void CStoreMgr::_evtStoreLoadFormClose(CForm* pForm, bool& IsClose)
	{
		g_stUIStore.DarkScene(false);
	}

	void CStoreMgr::_evtProTimeArriveEvt(CGuiData *pSender)
	{
		g_stUIStore.DarkScene(false);
	}


////////////////////////////////////////////////////////////////////////////////////////////////////


	void CStoreMgr::_evtStoreViewAllMouseEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
	{
		string strName = pSender->GetName();

		if(strName == "btnReBack")
		{
			g_stUIStore.ShowViewAllForm(false);
		}
	}

	
	void CStoreMgr::_evtStoreViewAllLostEvent(CGuiData *pSender)
	{
		g_stUIStore.frmViewAll->SetIsShow(false);
	}


////////////////////////////////////////////////////////////////////////////////////////////////////


	// 
	void CStoreMgr::ChaEquipClearAll()
	{
		m_sCurLookInfo = m_sLookInfo;
		_ChangeChaPart(m_sCurLookInfo);
		m_pCurrMainCha->FightSwitch(true);	// 
	}

	//  3D 
	void CStoreMgr::ChaLeftRotate()
	{
		m_nChaRotate += 180;
		m_nChaRotate += -(-1) * 15;
		m_nChaRotate = (g_stUIStore.m_nChaRotate + 360) % 360;
		m_nChaRotate -= 180;
	}

	//  3D 
	void CStoreMgr::ChaRightRotate()
	{
		m_nChaRotate += 180;
		m_nChaRotate += -(1) * 15;
		m_nChaRotate = (g_stUIStore.m_nChaRotate + 360) % 360;
		m_nChaRotate -= 180;
	}

	//  3D 
	void CStoreMgr::RenderChaTryon(int x,int y)
	{
		if( !m_pCurrMainCha ) return;

		g_Render.LookAt( D3DXVECTOR3( 11.0f, 36.0f, 10.0f ), D3DXVECTOR3( 8.70f, 12.0f, 8.0f ), MPRender::VIEW_3DUI );
		y += 100;

		MPMatrix44 matCha = *m_pCurrMainCha->GetMatrix();	// 

		m_pCurrMainCha->SetScale( D3DXVECTOR3( 0.8f, 0.8f, 0.8f ) );
		m_pCurrMainCha->SetUIYaw(180 + m_nChaRotate);
		m_pCurrMainCha->SetUIScaleDis(9.0f * g_Render.GetScrWidth()/SMALL_RES_X );
		m_pCurrMainCha->RenderForUI(x, y, true);

		m_pCurrMainCha->SetMatrix(& matCha);	// 

		g_Render.SetTransformView(&g_Render.GetWorldViewMatrix());
	}


	// 3D
	void CStoreMgr::_evtChaTryonRenderEvent(C3DCompent *pSender, int x, int y)
	{
		g_stUIStore.RenderChaTryon(x, y);
	}

	void CStoreMgr::_evtChaEquipClearAll(CGuiData *sender, int x, int y, DWORD key)
	{
		g_stUIStore.ChaEquipClearAll();
	}

	void CStoreMgr::_evtChaLeftRotate(CGuiData *sender, int x, int y, DWORD key )
	{
		g_stUIStore.ChaLeftRotate();
	}

	void CStoreMgr::_evtChaRightRotate(CGuiData *sender, int x, int y, DWORD key )
	{
		g_stUIStore.ChaRightRotate();
	}

	void CStoreMgr::_evtChaLeftContinueRotate(CGuiData *sender)
	{
		g_stUIStore.ChaLeftRotate();
	}

	void CStoreMgr::_evtChaRightContinueRotate(CGuiData *sender)
	{
		g_stUIStore.ChaRightRotate();
	}

	void CStoreMgr::_evtTryonFormClose(CForm* pForm, bool& IsClose)
	{
		if(g_stUIStore.m_pCurrMainCha)
		{
			g_stUIStore.m_pCurrMainCha->SetValid(false);
			g_stUIStore.m_pCurrMainCha = nullptr;
		}
	}
}

