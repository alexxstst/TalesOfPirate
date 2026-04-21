#include "StdAfx.h"
#include "SkillRecordStore.h"
#include "SkillStateRecordStore.h"
#include "uiequipform.h"
#include "uiform.h"
#include "uiskilllist.h"
#include "packetcmd.h"
#include "Scene.h"
#include "Character.h"
#include "uifastcommand.h"
#include "uigoodsgrid.h"
#include "NetProtocol.h"
#include "gameapp.h"
#include "uiitemcommand.h"
#include "uilabel.h"
#include "tools.h"
#include "uitradeform.h"
#include "uiboxform.h"
#include "packetcmd.h"
#include "netprotocol.h"
#include "uiboxform.h"
#include "uiboatform.h"
#include "chastate.h"
#include "SkillStateRecord.h"
#include "ProCirculate.h"
#include "stpose.h"
#include "uiboxform.h"
#include "StringLib.h"
#include "UICheckBox.h"
#include "UIDoublePwdForm.h"
#include "UIStoreForm.h"
#include "GlobalVar.h"
#include "EffectObj.h"
#include "UIMenu.h"
#include "UINpcTradeForm.h"
#include "WorldScene.h"
#include "UICozeForm.h"

using namespace std;

using namespace GUI;

static char szBuf[32] = { 0 };

CGuiPic	CEquipMgr::_imgCharges[enumEQUIP_NUM];
CEquipMgr::SSplitItem CEquipMgr::SSplit;
int CEquipMgr::lIMP =0;

bool g_IsNumberTopBar{ false };

extern CDoublePwdMgr g_stUIDoublePwd;

void CEquipMgr::ThrowSelectedItems(CGoodsGrid* grid)
{
	if (!grid)
	{
		return;
	}

	auto pSelf = g_stUIBoat.FindCha(grid);
	if (!pSelf)
	{
		return;
	}

	stNetItemThrow sItemThrow;
	sItemThrow.lPosX = CGameApp::GetCurScene()->GetMouseMapX() * 100.0f;
	sItemThrow.lPosY = CGameApp::GetCurScene()->GetMouseMapY() * 100.0f;

	if (!g_stUIEquip._GetThrowPos(reinterpret_cast<int&>(sItemThrow.lPosX), reinterpret_cast<int&>(sItemThrow.lPosY)))
	{
		return;
	}

	for (auto i = 0, n = grid->GetMaxNum(); i < n; ++i)
	{
		if (!grid->IsItemSelected(i))
		{
			continue;
		}

		auto item = grid->GetItem(i);
		if (!item || !item->GetIsValid())
		{
			continue;
		}

		sItemThrow.lNum = item->GetTotalNum();
		sItemThrow.sGridID = i;
		CS_BeginAction(pSelf, enumACTION_ITEM_THROW, reinterpret_cast<void*>(&sItemThrow));
	}

	grid->ResetItemSelections();
}

void CEquipMgr::LockSelectedItems(CGoodsGrid* grid)
{
	if (!grid)
	{
		return;
	}

	for (auto i = 0, n = grid->GetMaxNum(); i < n; ++i)
	{
		if (!grid->IsItemSelected(i))
	{
			continue;
		}

		auto item = static_cast<CItemCommand*>(grid->GetItem(i));
		if (!item || !item->GetIsValid())
		{
			continue;
		}

		if (item->IsLocked())
		{
			continue;
		}

		CS_DropLock(i);
	}

	grid->ResetItemSelections();
}

void CEquipMgr::DeleteSelectedItems(CGoodsGrid* grid)
{
	if (!grid)
	{
		return;
	}

	for (auto i = 0, n = grid->GetMaxNum(); i < n; ++i)
	{
		if (!grid->IsItemSelected(i))
		{
			continue;
		}

		auto item = static_cast<CItemCommand*>(grid->GetItem(i));
		if (!item || !item->GetIsValid())
		{
			continue;
		}

		if (!item->GetItemInfo()->chIsDel)
		{
			continue;
		}

		stNetDelItem info;
		info.sGridID = i;
		CS_BeginAction(g_stUIBoat.GetHuman(), enumACTION_ITEM_DELETE, &info);
	}

	grid->ResetItemSelections();
}

//---------------------------------------------------------------------------
// class CMainMgr
//---------------------------------------------------------------------------
bool CEquipMgr::Init()
{
	CForm *frmMain800 = _FindForm("frmMain800");

	/////////// 
	frmSkill = _FindForm("frmSkill");
	if(!frmSkill) return false;
	frmSkill->evtShow = _evtSkillFormShow;

    lstFightSkill = dynamic_cast<CSkillList*>( frmSkill->Find("lstSkill") );
    if( !lstFightSkill ) return Error( GetLanguageString(45).c_str(), frmSkill->GetName(), "lstSkill" );
	lstFightSkill->evtUpgrade = _evtSkillUpgrade;

    lstLifeSkill = dynamic_cast<CSkillList*>( frmSkill->Find("lstSkillW") );
    if( !lstLifeSkill ) return Error( GetLanguageString(45).c_str(), frmSkill->GetName(), "lstSkillW" );
	lstLifeSkill->evtUpgrade = _evtSkillUpgrade;

    lstSailSkill = dynamic_cast<CSkillList*>( frmSkill->Find("lstSkillS") );
    if( !lstSailSkill ) return Error( GetLanguageString(45).c_str(), frmSkill->GetName(), "lstSkillS" );
	lstSailSkill->evtUpgrade = _evtSkillUpgrade;
	lstSailSkill->SetIsShowUpgrade(false);

    labPoint = dynamic_cast<CLabel *>(frmSkill->Find("labPoint"));
    if (!labPoint) return Error( GetLanguageString(45).c_str(), frmSkill->GetName(), "labPoint");

    labPointLife = dynamic_cast<CLabel *>(frmSkill->Find("labPoint1"));
    if (!labPointLife) return Error( GetLanguageString(45).c_str(), frmSkill->GetName(), "labPoint1");

	frmInv = _FindForm("frmInv");
	
	if( !frmInv ) 		return false;  
	frmInv->evtShow = _evtEquipFormShow;   
	frmInv->evtClose = _evtEquipFormClose;
	frmInv->evtEntrustMouseEvent = _evtItemFormMouseEvent;
	
	grdItem = dynamic_cast<CGoodsGrid*>(frmInv->Find("grdItem"));
	if( !grdItem ) return Error( GetLanguageString(45).c_str(), frmInv->GetName(), "grdItem" );

	lblGold = dynamic_cast<CLabel*>(frmInv->Find("labItemgoldnumber"));
	if( !lblGold ) return Error( GetLanguageString(45).c_str(), frmInv->GetName(), "labItemgoldnumber" );
	
	GetGoodsGrid()->SetSelectEnable(true);
    GetGoodsGrid()->evtThrowItem = evtThrowItemEvent;
    GetGoodsGrid()->evtSwapItem = evtSwapItemEvent;
    GetGoodsGrid()->evtBeforeAccept = _evtDragToGoodsEvent;
	GetGoodsGrid()->evtRMouseEvent = _evtRMouseGridEvent;

	imgLock = dynamic_cast<CImage*>(frmInv->Find("imgLock"));
	if(! imgLock)return Error( GetLanguageString(45).c_str(), frmInv->GetName(), "imgLock" );

	imgUnLock = dynamic_cast<CImage*>(frmInv->Find("imgUnLock"));
	if(! imgUnLock)return Error( GetLanguageString(45).c_str(), frmInv->GetName(), "imgUnLock" );



	SetIsLock(false);	// 


	///////////////
	CForm* frmFast = _FindForm("frmFast");
	if( !frmFast ) return false;

	// 
    CTextButton* btnFastUp = dynamic_cast<CTextButton*>( frmFast->Find("btnFastUp") );
    if( !btnFastUp ) return Error( GetLanguageString(45).c_str(), frmMain800->GetName(), "btnFastUp" );
	btnFastUp->evtMouseClick = _evtButtonClickEvent;

    CTextButton* btnFastDown = dynamic_cast<CTextButton*>( frmFast->Find("btnFastDown") );
    if( !btnFastUp ) return Error( GetLanguageString(45).c_str(), frmMain800->GetName(), "btnFastDown" );
	btnFastDown->evtMouseClick = _evtButtonClickEvent;

	CForm* frmFast2 = _FindForm("frmFast2");
	if( !frmFast2 ) return false;

	_pFastCommands = new CFastCommand*[ SHORT_CUT_NUM ];   
    memset( _pFastCommands, 0, sizeof(CFastCommand*)*SHORT_CUT_NUM );
    char szName[30] = { 0 };


	for (auto i = 0; i < MAX_FAST_COL; ++i)
	{
		sprintf(szName, "fscMainF%d", i);
		_pFastCommands[i] = dynamic_cast<CFastCommand*>(frmFast->Find(szName));
		if (_pFastCommands[i])
		{
			_pFastCommands[i]->nTag = i;
			_pFastCommands[i]->evtChange = _evtFastChange;
		}
	}

	for (auto row = 1; row < MAX_FAST_ROW - 1; ++row) {
		for (auto i = 0; i < MAX_FAST_COL; ++i)
		{
			auto index = row * MAX_FAST_COL + i;
			_pFastCommands[index] = new CFastCommand(*_pFastCommands[i]);
			_pFastCommands[index]->AddForm();

			_pFastCommands[index]->nTag = index;
			_pFastCommands[index]->evtChange = _evtFastChange;
		}
	}


	for (auto i = MAX_FAST_COL * 2; i < MAX_FAST_COL * 3; ++i)
	{
		sprintf(szName, "fscMainF2%d", i - 12);
		_pFastCommands[i] = dynamic_cast<CFastCommand*>(frmFast2->Find(szName));
		if (_pFastCommands[i])
		{
			_pFastCommands[i]->nTag = i;
			_pFastCommands[i]->evtChange = _evtFastChange;
			_pFastCommands[i]->topBar = true;
		}
	}
    
	//_pActiveFastLabel = dynamic_cast<CLabel*>(frmMain800->Find( "labFast" ) );
    _ActiveFast( 0 );

	/////////////// 
	
	memset( cnmEquip, 0, sizeof(cnmEquip) );
	cnmEquip[enumEQUIP_HEAD] = dynamic_cast<COneCommand*>(frmInv->Find("cmdArmet"));
	cnmEquip[enumEQUIP_BODY] = dynamic_cast<COneCommand*>(frmInv->Find("cmdBody"));
	cnmEquip[enumEQUIP_GLOVE] = dynamic_cast<COneCommand*>(frmInv->Find("cmdGlove"));
	cnmEquip[enumEQUIP_SHOES] = dynamic_cast<COneCommand*>(frmInv->Find("cmdShoes"));
	cnmEquip[enumEQUIP_LHAND] = dynamic_cast<COneCommand*>(frmInv->Find("cmdLeftHand"));
	cnmEquip[enumEQUIP_RHAND] = dynamic_cast<COneCommand*>(frmInv->Find("cmdRightHand"));
	cnmEquip[enumEQUIP_NECK] = dynamic_cast<COneCommand*>(frmInv->Find("cmdNecklace"));
	cnmEquip[enumEQUIP_HAND1] = dynamic_cast<COneCommand*>(frmInv->Find("cmdCirclet1"));
	cnmEquip[enumEQUIP_HAND2] = dynamic_cast<COneCommand*>(frmInv->Find("cmdCirclet2"));
	cnmEquip[enumEQUIP_Jewelry1] = dynamic_cast<COneCommand*>(frmInv->Find("cmdJewelry1"));
	cnmEquip[enumEQUIP_Jewelry2] = dynamic_cast<COneCommand*>(frmInv->Find("cmdJewelry2"));
	cnmEquip[enumEQUIP_Jewelry3] = dynamic_cast<COneCommand*>(frmInv->Find("cmdJewelry3"));
	cnmEquip[enumEQUIP_Jewelry4] = dynamic_cast<COneCommand*>(frmInv->Find("cmdJewelry4"));
	cnmEquip[enumEQUIP_WING] = dynamic_cast<COneCommand*>(frmInv->Find("cmdWing"));
	
	cnmEquip[enumEQUIP_CLOAK] = dynamic_cast<COneCommand*>(frmInv->Find("cmdCloak"));
	cnmEquip[enumEQUIP_FAIRY] = dynamic_cast<COneCommand*>(frmInv->Find("cmdPet"));
	cnmEquip[enumEQUIP_REAR	] = dynamic_cast<COneCommand*>(frmInv->Find("cmdRearPet"));
	cnmEquip[enumEQUIP_MOUNT] = dynamic_cast<COneCommand*>(frmInv->Find("cmdMount"));
	
	
	cnmEquip[enumEQUIP_HEADAPP ] = dynamic_cast<COneCommand*>(frmInv->Find("cmdArmetApp"));
	cnmEquip[enumEQUIP_FACEAPP ] = dynamic_cast<COneCommand*>(frmInv->Find("cmdFaceApp"));
	cnmEquip[enumEQUIP_BODYAPP ] = dynamic_cast<COneCommand*>(frmInv->Find("cmdBodyApp"));
	cnmEquip[enumEQUIP_GLOVEAPP] = dynamic_cast<COneCommand*>(frmInv->Find("cmdGloveApp"));
	cnmEquip[enumEQUIP_SHOESAPP] = dynamic_cast<COneCommand*>(frmInv->Find("cmdShoesApp"));
	cnmEquip[enumEQUIP_FAIRYAPP] = dynamic_cast<COneCommand*>(frmInv->Find("cmdPetApp"));
	cnmEquip[enumEQUIP_GLOWAPP ] = dynamic_cast<COneCommand*>(frmInv->Find("cmdGlowApp"));
	cnmEquip[enumEQUIP_DAGGERAPP] = dynamic_cast<COneCommand*>(frmInv->Find("cmdDaggerApp"));
	cnmEquip[enumEQUIP_GUNAPP] = dynamic_cast<COneCommand*>(frmInv->Find("cmdGunApp"));
	cnmEquip[enumEQUIP_SWORD1APP] = dynamic_cast<COneCommand*>(frmInv->Find("cmdSword1App"));
	cnmEquip[enumEQUIP_GREATSWORDAPP] = dynamic_cast<COneCommand*>(frmInv->Find("cmdGreatSwordApp"));
	cnmEquip[enumEQUIP_STAFFAPP] = dynamic_cast<COneCommand*>(frmInv->Find("cmdStaffApp"));
	cnmEquip[enumEQUIP_BOWAPP] = dynamic_cast<COneCommand*>(frmInv->Find("cmdBowApp"));
	cnmEquip[enumEQUIP_SWORD2APP] = dynamic_cast<COneCommand*>(frmInv->Find("cmdSword2App"));
	cnmEquip[enumEQUIP_SHIELDAPP] = dynamic_cast<COneCommand*>(frmInv->Find("cmdShieldApp"));
	

	for( int i=0; i<enumEQUIP_NUM; i++ )
    {
        if( cnmEquip[i] ) 
        {
            cnmEquip[i]->evtBeforeAccept = _evtEquipEvent;
            //cnmEquip[i]->evtMouseClick = _UnequipPart;
			//cnmEquip[i]->evtThrowItem = _evtThrowEquipEvent;	// 

			cnmEquip[i]->SetActivePic( &_imgCharges[i] );
        }
    }

	SkillRecordStore::Instance()->ForEach([&](CSkillRecord& info) {
		if( info.nStateID ) _cancels.push_back( &info );
	});

	SkillStateRecordStore::Instance()->ForEach([&](CSkillStateRecord& state) {
		if( state.sChargeLink>=0 && state.sChargeLink<enumEQUIP_NUM )
		{
			_charges.push_back( &state );
		}
	});
	
	frmItemSpy = _FindForm("frmItemSpy");   // 
	frmItemSpy->evtShow = _evtSpyFormShow;
	frmItemSpy->evtClose = _evtSpyFormClose;
	if( !frmItemSpy ) 		return false;  
	//frmItemSpy->evtShow = _evtItemFormShow;   
	//frmItemSpy->evtClose = _evtItemFormClose;
	//frmItemSpy->evtEntrustMouseEvent = _evtItemFormMouseEvent;
	
	/////////////// 
	memset( cnmEquipSpy, 0, sizeof(cnmEquipSpy) );
	cnmEquipSpy[enumEQUIP_HEAD] = dynamic_cast<COneCommand*>(frmItemSpy->Find("cmdArmet"));
	cnmEquipSpy[enumEQUIP_BODY] = dynamic_cast<COneCommand*>(frmItemSpy->Find("cmdBody"));
	cnmEquipSpy[enumEQUIP_GLOVE] = dynamic_cast<COneCommand*>(frmItemSpy->Find("cmdGlove"));
	cnmEquipSpy[enumEQUIP_SHOES] = dynamic_cast<COneCommand*>(frmItemSpy->Find("cmdShoes"));
	cnmEquipSpy[enumEQUIP_LHAND] = dynamic_cast<COneCommand*>(frmItemSpy->Find("cmdLeftHand"));
	cnmEquipSpy[enumEQUIP_RHAND] = dynamic_cast<COneCommand*>(frmItemSpy->Find("cmdRightHand"));
	cnmEquipSpy[enumEQUIP_NECK] = dynamic_cast<COneCommand*>(frmItemSpy->Find("cmdNecklace"));
	cnmEquipSpy[enumEQUIP_HAND1] = dynamic_cast<COneCommand*>(frmItemSpy->Find("cmdCirclet1"));
	cnmEquipSpy[enumEQUIP_HAND2] = dynamic_cast<COneCommand*>(frmItemSpy->Find("cmdCirclet2"));
	cnmEquipSpy[enumEQUIP_Jewelry1] = dynamic_cast<COneCommand*>(frmItemSpy->Find("cmdJewelry1"));
	cnmEquipSpy[enumEQUIP_Jewelry2] = dynamic_cast<COneCommand*>(frmItemSpy->Find("cmdJewelry2"));
	cnmEquipSpy[enumEQUIP_Jewelry3] = dynamic_cast<COneCommand*>(frmItemSpy->Find("cmdJewelry3"));
	cnmEquipSpy[enumEQUIP_Jewelry4] = dynamic_cast<COneCommand*>(frmItemSpy->Find("cmdJewelry4"));
	cnmEquipSpy[enumEQUIP_WING] = dynamic_cast<COneCommand*>(frmItemSpy->Find("cmdWing"));
		
	cnmEquipSpy[enumEQUIP_CLOAK] = dynamic_cast<COneCommand*>(frmItemSpy->Find("cmdCloak"));
	cnmEquipSpy[enumEQUIP_FAIRY] = dynamic_cast<COneCommand*>(frmItemSpy->Find("cmdPet"));
	cnmEquipSpy[enumEQUIP_REAR	] = dynamic_cast<COneCommand*>(frmItemSpy->Find("cmdRearPet"));
	cnmEquipSpy[enumEQUIP_MOUNT] = dynamic_cast<COneCommand*>(frmItemSpy->Find("cmdMount"));
	
	
	C3DCompent* ui3dEqSpyCha = (C3DCompent*)frmItemSpy->Find( "ui3dEqSpyCha" );
	if( ui3dEqSpyCha ){
		ui3dEqSpyCha->SetRenderEvent( _EqSpyRenderEvent );
	}
	
	CTextButton* btnLeft3d = (CTextButton*)frmItemSpy->Find("btnLeft3d");
	if (!btnLeft3d){
		{ g_logManager.InternalLog(LogLevel::Error, "errors", SafeVFormat(GetLanguageString(45), frmItemSpy->GetName(), "btnLeft3d")); }
		return false;
	}
	btnLeft3d->evtMouseClick = _RotateSpyLeft;
	btnLeft3d->evtMouseDownContinue = _RotateSpyLeftContinue;

	CTextButton* btnRight3d = (CTextButton*)frmItemSpy->Find("btnRight3d");
	if (!btnRight3d){
		{ g_logManager.InternalLog(LogLevel::Error, "errors", SafeVFormat(GetLanguageString(45), frmItemSpy->GetName(), "btnRight3d")); }
		return false;
	}
	btnRight3d->evtMouseClick = _RotateSpyRight;
	btnRight3d->evtMouseDownContinue = _RotateSpyRightContinue;
	
	
	

	C3DCompent* ui3dCha = (C3DCompent*)frmInv->Find( "ui3dCha" );
	if( ui3dCha ){
		ui3dCha->SetRenderEvent( _ChaRenderEvent );
	}
	
	CTextButton* btnChaLeft3d = (CTextButton*)frmInv->Find("btnChaLeft3d");
	btnChaLeft3d->evtMouseClick = _RotateChaLeft;
	btnChaLeft3d->evtMouseDownContinue = _RotateChaLeftContinue;

	CTextButton* btnChaRight3d = (CTextButton*)frmInv->Find("btnChaRight3d");
	btnChaRight3d->evtMouseClick = _RotateChaRight;
	btnChaRight3d->evtMouseDownContinue = _RotateChaRightContinue;
	
	btnOpenTempBag = (CTextButton*)frmInv->Find("btnOpenTempBag");
	btnOpenTempBag->evtMouseClick = _ClickTempBag;
	
	if (rightClickItemMenu = CMenu::FindMenu("inventoryItemRightClick"); rightClickItemMenu) {
		rightClickItemMenu->evtListMouseDown = [](CGuiData* pSender, int x, int y, DWORD key) {
			auto menu = static_cast<CMenu*>(pSender);
			menu->SetIsShow(false);

			constexpr std::string_view menu_string[] = { "Delete Item", "Throw Item", "Lock Item", "Unlock Item", "Sell Item" };

			const std::string_view selected = menu->GetSelectMenu()->GetString();
			if (selected == menu_string[0]) {

				auto deleteConfirm = [](CCompent* pSender, int nMsgType, int x, int y, DWORD dwKey)
				{
					if (nMsgType != CForm::mrYes)
					{
						return;
					}

					auto pSelect = static_cast<stSelectBox*>(pSender->GetForm()->GetPointer());
					if (pSelect && pSelect->pointer) {
						auto grid = static_cast<CGoodsGrid*>(pSelect->pointer);
						g_stUIEquip.DeleteSelectedItems(grid);
					}
					
				};

				auto pSelectBox = g_stUIBox.ShowSelectBox(deleteConfirm, RES_STRING(CL_LANGUAGE_MATCH_556), true);
				pSelectBox->pointer = reinterpret_cast<void*>(g_stUIEquip.GetGoodsGrid());
				return;
			}
			if (selected == menu_string[1]) {
				if (CWorldScene::_IsThrowItemHint)
				{
					auto throwConfirm = [](CCompent* pSender, int nMsgType, int x, int y, DWORD dwKey)
					{
						if (nMsgType != CForm::mrYes) {
							return;
						}

						auto pSelect = static_cast<stSelectBox*>(pSender->GetForm()->GetPointer());
						if (pSelect && pSelect->pointer) {
							auto grid = static_cast<CGoodsGrid*>(pSelect->pointer);
							g_stUIEquip.ThrowSelectedItems(grid);
						}
					};

					if (g_stUIEquip.GetGoodsGrid()->GetSelectedItemCount() == 1)
					{
						//NOTE: bool is never set through parameter reference in evtThrowItemEvent...
						bool does_absolutely_nothing = false;
						evtThrowItemEvent(g_stUIEquip.GetGoodsGrid(), g_stUIEquip.rightClickItemIndex, does_absolutely_nothing);
						return;
					}

					auto pSelectBox = g_stUIBox.ShowSelectBox(throwConfirm, RES_STRING(CL_UIEQUIPFORM_CPP_00001), true);
					pSelectBox->pointer = reinterpret_cast<void*>(g_stUIEquip.GetGoodsGrid());
					return;
				}

				ThrowSelectedItems(g_stUIEquip.GetGoodsGrid());
				return;
			}
			if (selected == menu_string[2]) {
				auto lockItemsConfirm = [](CCompent* pSender, int nMsgType, int x, int y, DWORD dwKey)
				{
					if (nMsgType != CForm::mrYes)
					{
						return;
					}

					auto pSelect = static_cast<stSelectBox*>(pSender->GetForm()->GetPointer());
					if (pSelect && pSelect->pointer) {
						auto grid = static_cast<CGoodsGrid*>(pSelect->pointer);
						LockSelectedItems(grid);
					}
				};

				auto pSelectBox = g_stUIBox.ShowSelectBox(lockItemsConfirm, "Do you wish to lock the item?", true);
				pSelectBox->pointer = reinterpret_cast<void*>(g_stUIEquip.GetGoodsGrid());
				return;
			}
			if (selected == menu_string[3]) {
				if (g_stUIEquip.GetGoodsGrid()->GetSelectedItemCount() == 1)
				{
					g_stUIDoublePwd.SetLockGridID(g_stUIEquip.rightClickItemIndex);
					g_stUIDoublePwd.SetType(CDoublePwdMgr::ITEM_UNLOCK);
					g_stUIDoublePwd.ShowDoublePwdForm();
					return;
				}

				g_stUIDoublePwd.SetType(CDoublePwdMgr::MULTI_ITEM_UNLOCK);
				g_stUIDoublePwd.ShowDoublePwdForm();
				return;
			}
			if (selected == menu_string[4]) {
				auto sellConfirm = [](CCompent* pSender, int nMsgType, int x, int y, DWORD dwKey)
				{
					if (nMsgType != CForm::mrYes)
					{
						return;
					}

					g_stUINpcTrade.SellSelectedItems(g_stUIEquip.GetGoodsGrid());
				};

				if (g_stUIEquip.GetGoodsGrid()->GetSelectedItemCount() == 1)
				{
					g_stUIEquip.ShowSellPrompt();
					return;
				}
		
				auto pSelectBox = g_stUIBox.ShowSelectBox(sellConfirm, "Sell all selected items?", true);
				pSelectBox->pointer = reinterpret_cast<void*>(g_stUIEquip.GetGoodsGrid());
				return;
			}
		};
	}

	stateDrags = _FindForm("stateDrags");
	if (!stateDrags) 
	{
		return false;
	}
	stateDrags->evtMouseDragEnd = _OnDragStates;
	stateDrags->SetIsShow(true);
	stateDrags->SetIsDrag(true);
	
	return true;
}

void CEquipMgr::_ClickTempBag(CGuiData* pSender, int x, int y, DWORD key) {
	string name = pSender->GetName();
	g_stUIStore.ShowTempKitbag();
}

void CEquipMgr::_UnequipPart(CGuiData *pSender, int x, int y, DWORD key){
	if( !CGameScene::GetMainCha() ) return;

	string name = pSender->GetName();
	int linkID = 0;
	if(name == "eqhelm"){
		linkID = 0;
	}else if(name == "eqboot"){
		linkID = 4;
	}else if(name == "eqglove"){
		linkID = 3;
	}else if(name == "eqbody"){
		linkID = 2;
	}else{
		return;
	}
	
    stNetItemUnfix item;
	item.chLinkID = linkID;
	item.sGridID = 0;

	if( g_stUIBoat.GetHuman()==CGameScene::GetMainCha() )
	{
		CActor* pActor = g_stUIBoat.GetHuman()->GetActor();
		CEquipState* pState = new CEquipState( pActor );
		pState->SetUnfixData( item );
		pActor->SwitchState( pState );
	}
	else
	{
	    CS_BeginAction( g_stUIBoat.GetHuman(), enumACTION_ITEM_UNFIX, (void*)&item );
	}
}


void CEquipMgr::End()
{
	//RefreshServerShortCut(); Human
}

void CEquipMgr::_evtSkillUpgrade(CSkillList *pSender, CSkillCommand* pSkill)
{
	if(!pSkill) return;

    int nSkillID = pSkill->GetSkillID();
	g_NetIF->GetProCir()->SkillUpgrade( nSkillID, 1 );
	return;
}

void CEquipMgr::LoadingCall()
{
	CCharacter* pMain = CGameScene::GetMainCha();
	if ( pMain )
	{
		int nJob = pMain->getGameAttr()->get(ATTR_JOB);

		for( int i=0; i<4; i++ )
		{
			CSkillList* pSkillList = GetSkillList( i );
			if(!pSkillList) continue;

			int nCount = pSkillList->GetCount();
			CSkillCommand* pObj = NULL;
			for( int j=0; j<nCount; j++ )
			{
				pObj = pSkillList->GetCommand( j );
				if( pObj )
				{
					pObj->GetSkillRecord()->Refresh( nJob );
				}
			}
		}
	}
}

void CEquipMgr::SynSkillBag(DWORD dwCharID, stNetSkillBag *pSSkillBag)
{
    CCharacter * pCha = g_stUIBoat.GetHuman();
	if( !pCha || pCha->getAttachID()!=dwCharID) 
	{
        g_logManager.InternalLog(LogLevel::Debug, "network", GetLanguageString(547).c_str());
        return;
	}

	int nJob = pCha->getGameAttr()->get(ATTR_JOB);
	int nCount = pSSkillBag->SBag.GetCount();
	SSkillGridEx* pSBag = pSSkillBag->SBag.GetValue();
    switch( pSSkillBag->chType )
    {
    case enumSYN_SKILLBAG_INIT: // 
        {
			lstSailSkill->Clear();
			lstFightSkill->Clear();
			lstLifeSkill->Clear();

			_skills.clear();

            CSkillRecord* pInfo = NULL;
            CSkillCommand* tmp = NULL;
			for( int i=0; i<nCount; i++ )
            {
                pInfo = GetSkillRecordInfo( pSBag[i].sID );
                if( !pInfo )
                {
                    { g_logManager.InternalLog(LogLevel::Debug, "network", SafeVFormat(GetLanguageString(548), pSBag[i].sID)); }
                    continue;
                }
                pInfo->GetSkillGrid() = pSBag[i];
				pInfo->Refresh( nJob );
				if( !pInfo->IsShow() ) continue;

                tmp = new CSkillCommand( pInfo );
                GetSkillList( pInfo->chFightType )->AddCommand( tmp );

				_skills.push_back( pInfo );
            }
        }
        break;
    case enumSYN_SKILLBAG_ADD:  // 
        {
            CSkillRecord* pInfo = NULL;
            CSkillCommand* tmp = NULL;
            for( int i=0; i<nCount; i++ )
            {
                pInfo = GetSkillRecordInfo( pSBag[i].sID );
                if( !pInfo )
                {
                    { g_logManager.InternalLog(LogLevel::Debug, "network", SafeVFormat(GetLanguageString(549), pSBag[i].sID)); }
                    continue;
                }
                pInfo->GetSkillGrid() = pSBag[i];
				pInfo->Refresh( nJob );
				if( !pInfo->IsShow() ) continue;

                tmp = new CSkillCommand( pInfo );

                GetSkillList( pInfo->chFightType )->AddCommand( tmp );
				_skills.push_back( pInfo );
            }
        }
        break;
    case enumSYN_SKILLBAG_MODI: // 
        {
            CSkillRecord* pInfo = NULL;
            CSkillCommand* tmp = NULL;
            for( int i=0; i<nCount; i++ )
            {
                pInfo = GetSkillRecordInfo( pSBag[i].sID );
                if( !pInfo )
                {
                    { g_logManager.InternalLog(LogLevel::Debug, "network", SafeVFormat(GetLanguageString(550), pSBag[i].sID)); }
                    continue;
                }
                pInfo->GetSkillGrid() = pSBag[i];
				pInfo->Refresh( nJob );
				if( !pInfo->IsShow() ) continue;

                if( pSBag[i].chLv==0 && !GetSkillList( pInfo->chFightType )->DelSkill( pSBag[i].sID ) )
                {
                    { g_logManager.InternalLog(LogLevel::Debug, "network", SafeVFormat(GetLanguageString(551), pSBag[i].sID)); }
                    continue;
                }
            }        
        }
        break;
    default:
        { g_logManager.InternalLog(LogLevel::Debug, "network", SafeVFormat(GetLanguageString(552), pSSkillBag->chType)); }
        return;
    }


	lstSailSkill->Refresh();
	lstFightSkill->Refresh();
	lstLifeSkill->Refresh();
}

CSkillRecord* CEquipMgr::FindSkill( int nID )
{
	for( vskill::iterator it=_skills.begin(); it!=_skills.end(); it++ )
		if( (*it)->Id==nID )
			return *it;

	return NULL;
}

//defItemShortCutType
void CEquipMgr::FastChange(int nIndex, short sGridID, char chType,bool update){
	if(update){
		_pFastCommands[ nIndex ]->AddCommand( GetGoodsGrid()->GetItem(sGridID) );
	}else{
		stNetShortCutChange param;
		memset( &param, 0, sizeof(param) );
		param.chIndex = nIndex;
		param.chType = chType;
		param.shyGrid = sGridID;
		
		/*
		if(_pFastCommands[ nIndex ]->GetCommand2()){
			param.shyGrid2 = _pFastCommands[ nIndex ]->GetCommand2()->nTag;
		}
		*/
		
		CS_BeginAction( g_stUIBoat.GetHuman(), enumACTION_SHORTCUT, (void*)&param );
		stNetShortCut& SCut = _stShortCut;
		SCut.chType[nIndex] = chType;
		SCut.byGridID[nIndex] = sGridID;
	}
}

void CEquipMgr::_evtFastChange(CGuiData *pSender, CCommandObj* pItem, bool& isAccept)
{	
	CGoodsGrid* pGrid = dynamic_cast<CGoodsGrid*>(CDrag::GetParent());
	if( pGrid && pGrid!=g_stUIEquip.GetGoodsGrid() ) return;

    CFastCommand* pFast = dynamic_cast<CFastCommand*>(pSender);
    if( !pFast ) return;
   
    isAccept = true;

    char chType = 0;
    short sGridID = 0;
    int nIndex = pFast->nTag;
	if( !g_stUIEquip._GetCommandShortCutType( pItem, chType, sGridID ) )
	{
		isAccept = false;
		g_pGameApp->SysInfo("%s", SafeVFormat(GetLanguageString(553), pItem->GetName()).c_str() );
		return;
	}
	g_stUIEquip.FastChange( nIndex,  sGridID,  chType);
}


void CEquipMgr::UpdataEquipData( const stNetChangeChaPart& SPart, CCharacter* pCha )
{
	// ID,
	CItemCommand* pItem = NULL;
	for( int i=0; i<enumEQUIP_NUM; i++ )
	{
		// Modify by lark.li 20080818 begin
		//if( SPart.SLink[i].sID==0 ) continue;
		if( cnmEquip[i] == NULL || SPart.SLink[i].sID==0 ) continue;
		// End

        pItem = dynamic_cast<CItemCommand*>(cnmEquip[i]->GetCommand());
        if( pItem && pItem->GetItemInfo()->lID==SPart.SLink[i].sID )
        {
			SItemGrid& Item = pItem->GetData();
			Item.bValid = SPart.SLink[i].bValid;
			Item.sEndure[0] = SPart.SLink[i].sEndure[0];
			Item.sEnergy[0] = SPart.SLink[i].sEnergy[0];
			Item.bItemTradable = SPart.SLink[i].bItemTradable;
			Item.expiration = SPart.SLink[i].expiration;

        }
		else
		{
			{ g_logManager.InternalLog(LogLevel::Error, "errors", SafeVFormat(GetLanguageString(554), ( pItem ? pItem->GetItemInfo()->lID : 0), SPart.SLink[i].sID)); }
		}
	}
}

void CEquipMgr::UpdataEquip( const stNetChangeChaPart& SPart, CCharacter* pCha )
{     
    // 
    memcpy( &stEquip, &SPart, sizeof(SPart) );
    for( int i=0; i<enumEQUIP_PART_NUM; i++ )
    {
        if( stEquip.SLink[i].sID==pCha->GetDefaultChaInfo()->sSkinInfo[i] )
            stEquip.SLink[i].sID = 0;
    }
    //for( int i=enumEQUIP_PART_NUM; i<enumEQUIP_NUM; i++ )
    //{
    //    if( stEquip.SLink[i].sID==enumEQUIP_BOTH_HAND )
    //        stEquip.SLink[i].sID = 0;
    //}

    // UI
    for( int i=0; i<enumEQUIP_NUM; i++ )
    {
        _UpdataEquip( stEquip.SLink[i], i );
    }
}

void CEquipMgr::UpdataEquipSpy( const stNetChangeChaPart& SPart, CCharacter* pCha )
{     
    // 
	eqSpyTarget = pCha;
	
	CLabel* labTitle = (CLabel*)frmItemSpy->Find( "labTitle" );
	char buf[64];
	sprintf(buf,"Stalk - %s",pCha->getName().c_str());
	labTitle->SetCaption(buf);
	
	
    memcpy( &stEquipSpy, &SPart, sizeof(SPart) );
    for( int i=0; i<enumEQUIP_PART_NUM; i++ )
    {
        if( stEquipSpy.SLink[i].sID==pCha->GetDefaultChaInfo()->sSkinInfo[i] )
            stEquipSpy.SLink[i].sID = 0;
    }
    for( int i=0; i<enumEQUIP_NUM; i++ )
    {
        _UpdataEquipSpy( stEquipSpy.SLink[i], i );
    }
	
	frmItemSpy->Show();
	
	
}

bool CEquipMgr::_UpdataEquipSpy( SItemGrid& Item, int nLink )
{
    if( !cnmEquipSpy[nLink] ) return false;

    int nItemID = Item.sID;
    if( nItemID>0 && nItemID!=enumEQUIP_BOTH_HAND )
    {        
        CItemCommand* pItem = dynamic_cast<CItemCommand*>(cnmEquipSpy[nLink]->GetCommand());
        if( pItem && pItem->GetItemInfo()->lID==nItemID )
        {
	        pItem->SetData( Item );
            return true;
        }

        CItemRecord* pInfo = GetItemRecordInfo( nItemID );
        if( !pInfo )
        {
            return false;
        }

        pItem = new CItemCommand( pInfo );
        pItem->SetData( Item );
        cnmEquipSpy[nLink]->AddCommand( pItem );
    }
    else
    {
        cnmEquipSpy[nLink]->DelCommand();
    }
    return true;
}


void CEquipMgr::RenderSpy(int x,int y){
	if( !eqSpyTarget || !spyModel ) {
		return;
	}
	RenderModel(x,y,eqSpyTarget, spyModel,eqSpyTargetRotate,refreshSpyModel);
	refreshSpyModel = false;
}

void CEquipMgr::SwitchMap(){
	chaModel = 0;
	eqSpyTarget = 0;
	spyModel = 0;
	refreshChaModel = true;
	refreshSpyModel = true;
}
	
void CEquipMgr::RenderModel(int x, int y, CCharacter* original, CCharacter* model, int rotation, bool refresh)
{
	g_Render.LookAt(D3DXVECTOR3(11.0f, 36.0f, 10.0f), D3DXVECTOR3(8.70f, 12.0f, 8.0f), MPRender::VIEW_3DUI);
	
	MPMatrix44 matrix = *model->GetMatrix();
	
	model->SetUIYaw(180 + rotation);

	int typeID = model->getTypeID();
	if (typeID == 3) {
	    model->SetUIScaleDis(13.0f * g_Render.GetScrWidth() / TINY_RES_X);
	} else if (typeID == 1) {
	    model->SetUIScaleDis(13.5f * g_Render.GetScrWidth() / TINY_RES_X);
	} else if (typeID == 4) {
	    model->SetUIScaleDis(12.0f * g_Render.GetScrWidth() / TINY_RES_X);
	} else if (typeID == 2) {
	    model->SetUIScaleDis(14.5f * g_Render.GetScrWidth() / TINY_RES_X);
	}

	if (refresh) {
		model->UpdataFace(original->GetPart());
	}

	model->SetMatrix(&matrix);

	g_Render.SetTransformView(&g_Render.GetWorldViewMatrix());

	model->RenderForUI(x, y, true);

	original->CheckIsFightArea();
	model->FightSwitch(original->GetIsFight());
	DWORD pose = original->GetCurPoseType();
	float vel = original->GetPoseVelocity();

	switch (pose) {
	case POSE_FLY_RUN:
	    pose = POSE_RUN;
	    break;
	case POSE_FLY_SEAT:
	    pose = POSE_SEAT;
	    break;
	case POSE_FLY_SHOW:
	    pose = POSE_SHOW;
	    break;
	case POSE_FLY_WAITING:
	    pose = POSE_WAITING;
	    break;
	case POSE_SEAT2:
	    pose = POSE_WAITING;
	    break;
	case POSE_LEAN:
	    pose = POSE_WAITING;
	    break;
	}
	model->PlayPose(pose, PLAY_LOOP_SMOOTH);
}

void CEquipMgr::RenderCha(int x,int y){
	if(!chaModel) {
		refreshChaModel = false;
		return;
	}
	RenderModel(x,y, g_pGameApp->GetCurScene()->GetMainCha(), chaModel,chaModelRotate,refreshChaModel);
	refreshChaModel = false;
}




void CEquipMgr::RotateCha(eDirectType enumDirect){
	chaModelRotate += 180;
	chaModelRotate += -((int)(enumDirect)) * 15;
	chaModelRotate = (chaModelRotate + 360) % 360;
	chaModelRotate -= 180;
}

void CEquipMgr::_RotateChaLeft(CGuiData *sender, int x, int y, DWORD key ){
	g_stUIEquip.RotateCha(LEFT);
}
	
	//~ ==================================================================
void CEquipMgr::_RotateChaRight(CGuiData *sender, int x, int y, DWORD key ){
	g_stUIEquip.RotateCha(RIGHT);
}

void CEquipMgr::_RotateChaLeftContinue(CGuiData *sender ){
	g_stUIEquip.RotateCha(LEFT);
}

void CEquipMgr::_RotateChaRightContinue(CGuiData *sender ){
	g_stUIEquip.RotateCha(RIGHT);
}



void CEquipMgr::RotateSpy(eDirectType enumDirect){
	eqSpyTargetRotate += 180;
	eqSpyTargetRotate += -((int)(enumDirect)) * 15;
	eqSpyTargetRotate = (eqSpyTargetRotate + 360) % 360;
	eqSpyTargetRotate -= 180;
}

void CEquipMgr::_EqSpyRenderEvent( C3DCompent *pSender, int x, int y){
	g_stUIEquip.RenderSpy(x, y);
}
void CEquipMgr::_ChaRenderEvent( C3DCompent *pSender, int x, int y){
	g_stUIEquip.RenderCha(x, y);
}

void CEquipMgr::_RotateSpyLeft(CGuiData *sender, int x, int y, DWORD key ){
	g_stUIEquip.RotateSpy(LEFT);
}
	
	//~ ==================================================================
void CEquipMgr::_RotateSpyRight(CGuiData *sender, int x, int y, DWORD key ){
	g_stUIEquip.RotateSpy(RIGHT);
}

//~ ==================================================================
void CEquipMgr::_RotateSpyLeftContinue(CGuiData *sender ){
	g_stUIEquip.RotateSpy(LEFT);
}

//~ ==================================================================
void CEquipMgr::_RotateSpyRightContinue(CGuiData *sender ){
	g_stUIEquip.RotateSpy(RIGHT);
}

bool CEquipMgr::_UpdataEquip( SItemGrid& Item, int nLink )
{
    if( !cnmEquip[nLink] ) return false;

    int nItemID = Item.sID;
    if( nItemID>0 && nItemID!=enumEQUIP_BOTH_HAND )
    {        
        CItemCommand* pItem = dynamic_cast<CItemCommand*>(cnmEquip[nLink]->GetCommand());
        if( pItem && pItem->GetItemInfo()->lID==nItemID )
        {
	        pItem->SetData( Item );
            return true;
        }

        CItemRecord* pInfo = GetItemRecordInfo( nItemID );
        if( !pInfo )
        {
            { g_logManager.InternalLog(LogLevel::Debug, "common", SafeVFormat(GetLanguageString(555), nItemID)); }
            return false;
        }

        pItem = new CItemCommand( pInfo );
        pItem->SetData( Item );
        cnmEquip[nLink]->AddCommand( pItem );
    }
    else
    {
        cnmEquip[nLink]->DelCommand();
    }
	
    return true;
}

bool CEquipMgr::_GetCommandShortCutType( CCommandObj* pItem, char& chType, short& sGridID )
{
	chType = 0;
	sGridID = 0;

	if( !pItem ) return true;

    if( g_stUIEquip.GetGoodsGrid() && pItem->GetParent()==g_stUIEquip.GetGoodsGrid() )
    {
        chType = defItemShortCutType;
        sGridID = pItem->GetIndex();
    }
    else if( g_stUIEquip.lstFightSkill && g_stUIEquip.lstFightSkill==pItem->GetParent() )
    {
        CSkillCommand* pSkill = dynamic_cast<CSkillCommand*>(pItem);

		if( pSkill->GetSkillRecord()->chType==2 )
		{
			return false;
		}

        if( pSkill ) 
        {
            chType = defSkillFightShortCutType;
            sGridID = pSkill->GetSkillID();
        }
    }
    else if( g_stUIEquip.lstLifeSkill && g_stUIEquip.lstLifeSkill==pItem->GetParent() )
    {
        CSkillCommand* pSkill = dynamic_cast<CSkillCommand*>(pItem);
        if( pSkill ) 
        {
            chType = defSkillLifeShortCutType;
            sGridID = pSkill->GetSkillID();
		}
    }
	else if(g_stUIEquip.lstSailSkill && g_stUIEquip.lstSailSkill == pItem->GetParent())
	{
        CSkillCommand* pSkill = dynamic_cast<CSkillCommand*>(pItem);
        if( pSkill ) 
        {
            chType = defSkillSailShortCutType;
            sGridID = pSkill->GetSkillID();
		}
	}

	return true;
}

int CEquipMgr::RefreshServerShortCut()
{
	int nCount = 0;
	stNetShortCut tmp;
	memset( &tmp, 0, sizeof(tmp) );
	for( int i=0; i<SHORT_CUT_NUM; i++ )
	{
		if( _pFastCommands[i] )
		{
			_GetCommandShortCutType( _pFastCommands[i]->GetCommand(), tmp.chType[i], tmp.byGridID[i] );
		}
	}

	stNetShortCutChange param;
	for( int i=0; i<SHORT_CUT_NUM; i++ )
	{
		if( (tmp.chType[i]!=_stShortCut.chType[i]) || (tmp.byGridID[i]!=_stShortCut.byGridID[i]) )
		{
			param.chIndex = i;
			param.chType = tmp.chType[i];
			param.shyGrid = tmp.byGridID[i];
			CS_BeginAction( g_stUIBoat.GetHuman(), enumACTION_SHORTCUT, (void*)&param );
			nCount++;

			ToLogService("common", "Index:{}, Type:{}, GridID:{}", param.chIndex, param.chType, param.shyGrid);
		}
	}
	memcpy( &_stShortCut, &tmp, sizeof(tmp) );
	ToLogService("common", "Total:{}\n", nCount);
	return nCount;
}

void CEquipMgr::UpdateShortCut( stNetShortCut& stShortCut ){
    memcpy( &_stShortCut, &stShortCut, sizeof(_stShortCut) );
   if( _pFastCommands )
   {
       for( unsigned int i=0; i<SHORT_CUT_NUM; i++ )
       {
           if( _pFastCommands[i] ) _pFastCommands[i]->DelCommand();
       }
   }

	int nIndex = 0;
	CGoodsGrid* getGoods = GetGoodsGrid();
	for( DWORD i=0; i<SHORT_CUT_NUM; i++ ) { 
		if( _pFastCommands[ i ] ){	
			if( getGoods && stShortCut.chType[i]==defItemShortCutType ){
				_pFastCommands[ i ]->AddCommand( getGoods->GetItem(stShortCut.byGridID[i]) );
			}
			else if (lstFightSkill && stShortCut.chType[i]==defSkillFightShortCutType ){
				_pFastCommands[ i ]->AddCommand( lstFightSkill->FindSkill( stShortCut.byGridID[i] ) );
			}
			else if (lstLifeSkill && stShortCut.chType[i]==defSkillLifeShortCutType ){
				_pFastCommands[ i ]->AddCommand( lstLifeSkill->FindSkill( stShortCut.byGridID[i] ) );				
			}
			else if (lstSailSkill && stShortCut.chType[i]==defSkillSailShortCutType ){
				_pFastCommands[ i ]->AddCommand( lstSailSkill->FindSkill( stShortCut.byGridID[i] ) );		
			}
		}
	}
}

void CEquipMgr::DelFastCommand( CCommandObj* pObj )
{
	if( pObj && pObj->GetIsFast() )
	{
		CFastCommand* pFast = CFastCommand::FintFastCommand( pObj ,true);
		if( pFast )
		{
			int nIndex = pFast->nTag;
			stNetShortCutChange param;
			memset( &param, 0, sizeof(param) );
			param.chIndex = nIndex;
			CS_BeginAction( g_stUIBoat.GetHuman(), enumACTION_SHORTCUT, (void*)&param );

			// ,
			stNetShortCut& SCut = _stShortCut;
			SCut.chType[nIndex] = 0;
			SCut.byGridID[nIndex] = 0;	
			pFast->DelCommand();			
		}				
	}
}

void CEquipMgr::_evtButtonClickEvent( CGuiData *pSender, int x, int y, DWORD key)
{
	string name=pSender->GetName();
    if( name=="btnFastUp" )
    {
        g_stUIEquip._ActiveFast( g_stUIEquip._nFastCur-1 );
        return;
    }
    else if( name=="btnFastDown" )
    {
        g_stUIEquip._ActiveFast( g_stUIEquip._nFastCur+1 );
        return;
    }
}

void CEquipMgr::_evtEquipEvent(CGuiData *pSender, CCommandObj* pItem, bool& isAccept)
{
    isAccept = false;

    CGoodsGrid* pGood = dynamic_cast<CGoodsGrid*>(CDrag::GetParent());
    if( pGood!=g_stUIEquip.GetGoodsGrid() ) return;

    CItemCommand* pItemCommand =  dynamic_cast<CItemCommand*>(pItem);
    if( !pItemCommand ) return;

    CGameScene* pScene = g_pGameApp->GetCurScene();
    if( !pScene ) return;

    CCharacter* pCha = pScene->GetMainCha();
    if( !pCha ) return;

    COneCommand* pCom = dynamic_cast<COneCommand*>(pSender);
    if( !pCom ) return;

	stNetUseItem info;
	info.sGridID = g_stUIEquip.grdItem->GetDragIndex();
	CS_BeginAction(g_stUIBoat.GetHuman(), enumACTION_ITEM_USE, &info);
}

void CEquipMgr::_evtDeleteItemYesNoEvent(CCompent* pSender, int nMsgType, int x, int y, DWORD dwKey)
{
	if( nMsgType!=CForm::mrYes ) return;

	stNetDelItem info;
	info.sGridID = g_stUIEquip.rightClickItemIndex;
	CS_BeginAction( g_stUIBoat.GetHuman(), enumACTION_ITEM_DELETE, &info );
}

void CEquipMgr::_evtLockItemYesNoEvent(CCompent* pSender, int nMsgType, int x, int y, DWORD dwKey)
{
	if (nMsgType != CForm::mrYes)
	{
		return;
	}
	CS_DropLock(g_stUIEquip.rightClickItemIndex);
}

void CEquipMgr::ShowSellPrompt() {
	grdItem->SetDragIndex(rightClickItemIndex);
	g_stUINpcTrade.LocalSaleToNpc(g_stUINpcTrade.grdNPCtradeWeapon,
		grdItem,
		rightClickItemIndex,
		grdItem->GetItem(g_stUIEquip.rightClickItemIndex));
}


void CEquipMgr::_evtThrowEquipEvent(CGuiData *pSender, CCommandObj* pItem, bool& isThrow)
{
    isThrow = false;

    CGameScene* pScene = CGameApp::GetCurScene();
    if( !pScene ) return;

    CCharacter* pMain = CGameScene::GetMainCha();
    if( !pMain ) return;

    COneCommand* pCom = dynamic_cast<COneCommand*>(CDrag::GetParent());
    if( pCom && pCom->nTag>0 )
    {
		int x = (int)(pScene->GetMouseMapX() * 100.0f);
		int y = (int)(pScene->GetMouseMapY() * 100.0f);

		if( !g_stUIEquip._GetThrowPos( x, y ) ) return;

		stUnfix& unfix = g_stUIEquip._sUnfix;
		unfix.Reset();

		unfix.nGridID = -2;
		unfix.nLink = pCom->nTag;
		unfix.nX = x;
		unfix.nY = y;
		unfix.pItem = pItem;
		g_stUIEquip._StartUnfix( unfix );
    }
}

bool CEquipMgr::ExecFastKey( int key ){ 
    if( key==VK_TAB || ( key==VK_OEM_6 && ::GetAsyncKeyState(VK_CONTROL) ) )
    {
        _ActiveFast( _nFastCur+1 );
        return true;
    }

	bool g_FKeysInUse{ false };
	
	if (key >= 112 && key <= 123)
		g_FKeysInUse = true;

	if(key == VK_RETURN && g_IsNumberTopBar && !g_FKeysInUse && !CCozeForm::GetInstance()->IsChatBoxActive())
		CCozeForm::GetInstance()->ActivateChatBox();
	
	if (g_IsNumberTopBar && !g_FKeysInUse && !g_pGameApp->IsCtrlPress() && key != 8 && !CCozeForm::GetInstance()->IsChatBoxActive())
	{
		auto numberID = key - '0';
		if (numberID >= 0 && numberID <= 9)
		{
			if (numberID == 0)
			{
				numberID = 10;
			}
			numberID -= 1;
			key = numberID;
		}
	}
	else
	{
		key -= VK_F1;
	}

    if ( key<0 || key>(int)11 ) {
		return false;
	}
	
	if (! CFormMgr::s_Mgr.GetEnableHotKey()) {
		return false;
	}

	//check if we are using the top bar
	if (g_pGameApp->IsShiftPress() && !g_IsNumberTopBar)
	{
		key = 2 * MAX_FAST_COL + key;
	} 
	else if (g_IsNumberTopBar && !g_FKeysInUse)
	{
		key = 2 * MAX_FAST_COL + key;
	} 
	else {
		key = _nFastCur * MAX_FAST_COL + key;
	}
	
	if( _pFastCommands[key]){
		_pFastCommands[key]->Exec();
		return true;
	}
	return false;
}

void CEquipMgr::_ActiveFast( int num )
{
    if( num>=2 ) 
    {
        num = 0;
    }

    if( num<0 ) 
    {
        num = 2 - 1;
    }

    int count = (int)SHORT_CUT_NUM;
    for( int i=0; i<count; i++ )
    {
        if( _pFastCommands[i] )
        {
            _pFastCommands[i]->SetIsShow( false );
        }
    }

	count = (num + 1) * MAX_FAST_COL;
	for (int i = num * MAX_FAST_COL; i < count; i++)
	{
		if (_pFastCommands[i])
		{
			_pFastCommands[i]->SetIsShow(true);
		}
	}
	
	count =3*MAX_FAST_COL;
    for( int i=2*MAX_FAST_COL; i<count; i++ )
    {
        if( _pFastCommands[i] )
        {
            _pFastCommands[i]->SetIsShow( true );
        }
    }
    _nFastCur = num;

	//_pActiveFastLabel->SetCaption( szBuf );
}

void CEquipMgr::evtSwapItemEvent(CGuiData *pSender,int nFirst, int nSecond, bool& isSwap)
{
    isSwap = false;
    CGoodsGrid* pGood = dynamic_cast<CGoodsGrid*>(CDrag::GetParent());

	CCharacter* pSelf = g_stUIBoat.FindCha( pGood );
	if( !pSelf ) return;

	if( pSelf->IsBoat() && pSelf!=CGameScene::GetMainCha() )
	{
		g_pGameApp->SysInfo("%s", GetLanguageString(557).c_str() );
		return;
	}

	CItemCommand* pItem = dynamic_cast<CItemCommand*>( pGood->GetItem( nSecond ) );
	CItemCommand* pTarget = dynamic_cast<CItemCommand*>( pGood->GetItem( nFirst ) );
	if (pItem && !pItem->GetIsValid())
	{
		return;
	}
	if (pTarget && !pTarget->GetIsValid())
	{
		return;
	}


	if( g_pGameApp->IsShiftPress() )
	{
		if( pItem && pItem->GetTotalNum()>1 )
		{
			if( !pTarget || pTarget->GetItemInfo()->Id==pItem->GetItemInfo()->Id )
			{
				SSplit.nFirst = nFirst;
				SSplit.nSecond = nSecond;
				SSplit.pSelf = pSelf;
				CBoxMgr::ShowNumberBox( SSplitItem::_evtSplitItemEvent, pItem->GetTotalNum() );
				return;
			}
		}
	}

	stNetItemPos info;
	info.sSrcGridID = nSecond;
	info.sSrcNum = 0;
	info.sTarGridID = nFirst;
	CS_BeginAction( pSelf, enumACTION_ITEM_POS, (void*)&info );
}

void CEquipMgr::SSplitItem::_evtSplitItemEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
{
	if( nMsgType!=CForm::mrYes ) return;

	stNumBox* pBox = (stNumBox*)pSender->GetForm()->GetPointer();
	if( !pBox ) return;

	stNetItemPos info;
	info.sSrcGridID = SSplit.nSecond;
	info.sSrcNum = pBox->GetNumber();
	info.sTarGridID = SSplit.nFirst;
	CS_BeginAction( SSplit.pSelf, enumACTION_ITEM_POS, (void*)&info );
}

bool CEquipMgr::_GetThrowPos( int& x, int& y )
{
    CCharacter* pCha = CGameScene::GetMainCha();
    if( !pCha ) return false;

    GetDistancePos( pCha->GetCurX(), pCha->GetCurY(), x, y, 100, x, y );
    if( CGameApp::GetCurScene()->GetIsBlockWalk(pCha, x, y) )
    {
        x = pCha->GetCurX();
        y = pCha->GetCurY();
    }
    return true;
}

void CEquipMgr::evtThrowItemEvent(CGuiData *pSender,int id,bool& isThrow)
{
    isThrow = false;

    if( !CGameApp::GetCurScene() )  return;

    CCharacter* pMain = CGameScene::GetMainCha();
    if( !pMain ) return;

    auto pGood = dynamic_cast<CGoodsGrid*>(pSender);

    CCommandObj* obj = pGood->GetItem(id); 

	CCharacter* pSelf = g_stUIBoat.FindCha( pGood );
	if( !pSelf ) return;

	// add by Philip.Wu  2006-07-05
	//  BUG
	if(!obj->GetIsValid()) return;

    int x = (int)(CGameApp::GetCurScene()->GetMouseMapX() * 100.0f);
    int y = (int)(CGameApp::GetCurScene()->GetMouseMapY() * 100.0f);

	if( !g_stUIEquip._GetThrowPos( x, y ) ) return;

	stThrow& sthrow = g_stUIEquip._sThrow;
	sthrow.nX = x;
	sthrow.nY = y;
	sthrow.nGridID = id;
	sthrow.pSelf = pSelf;
	
	//
	CItemCommand* pItem = dynamic_cast<CItemCommand*>(obj);
	if ( pItem && pItem->GetItemInfo()->sType==43 )
	{
		stSelectBox* pBox = g_stUIBox.ShowSelectBox(_evtThrowBoatDialogEvent,
			GetLanguageString(558).c_str(),
			true);
		if (pBox)
		{
			pBox->pointer = &sthrow;
			return;
		}
	}

    if( obj->GetIsPile() && obj->GetTotalNum()>1 )
    {
		stNumBox* pBox = g_stUIBox.ShowNumberBox( _evtThrowDialogEvent, obj->GetTotalNum() );
		if( pBox )
		{
			pBox->pointer = &sthrow;
	        return;
		}
    }

	g_stUIEquip._SendThrowData( sthrow );
}

void CEquipMgr::_evtThrowDialogEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
{
	if( nMsgType!=CForm::mrYes ) return;

	stNumBox* pBox = (stNumBox*)pSender->GetForm()->GetPointer();
	if( !pBox ) return;

	stThrow* p = (stThrow*)pBox->pointer;
	if( !p ) return;

	g_stUIEquip._SendThrowData( *p, pBox->GetNumber() );
}

void CEquipMgr::_evtThrowBoatDialogEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
{
	if( nMsgType!=CForm::mrYes ) return;

	stSelectBox* pBox = (stSelectBox*)pSender->GetForm()->GetPointer();
	if( !pBox ) return;

	stThrow* p = (stThrow*)pBox->pointer;
	if( !p ) return;

	g_stUIEquip._SendThrowData( *p, 1 );
}

void CEquipMgr::_SendThrowData( const stThrow& sthrow, int nThrowNum )
{
	if( sthrow.pSelf->IsBoat() && sthrow.pSelf!=CGameScene::GetMainCha() )
	{
		g_pGameApp->SysInfo("%s", GetLanguageString(557).c_str() );
		return;
	}

	auto throwDeleteConfirm = [](CCompent* pSender, int nMsgType, int x, int y, DWORD dwKey)
	{
		if (nMsgType != CForm::mrYes) {
			return;
		}
		stSelectBox* pSelect = static_cast<stSelectBox*>(pSender->GetForm()->GetPointer());
		stThrow* pItem = (stThrow*)pSelect->pointer;

		if (pSelect && pSelect->pointer) {
			stNetItemThrow item;
			item.lNum = (int)pSelect->dwTag;
			item.sGridID = pItem->nGridID;
			item.lPosX = pItem->nX;
			item.lPosY = pItem->nY;

			CS_BeginAction(pItem->pSelf, enumACTION_ITEM_THROW, (void*)&item);
		}
	};

	if (CWorldScene::_IsThrowItemHint) {
		stSelectBox* pSelectBox = g_stUIBox.ShowSelectBox(throwDeleteConfirm, RES_STRING(CL_UIEQUIPFORM_CPP_00001), true);

		pSelectBox->dwTag = nThrowNum;
		pSelectBox->pointer = (void*)&sthrow;
	}
	else {
		stNetItemThrow item;
		item.lNum = nThrowNum;
		item.sGridID = sthrow.nGridID;
		item.lPosX = sthrow.nX;
		item.lPosY = sthrow.nY;

		CS_BeginAction(sthrow.pSelf, enumACTION_ITEM_THROW, (void*)&item);
	}
}

void CEquipMgr::_evtSkillFormShow(CGuiData *pSender)
{
	g_stUIEquip.RefreshUpgrade();
}

void CEquipMgr::UpdateIMP(int IMP){
	lIMP = IMP;
	//CForm* frmInv = _FindForm("frmInv");
	CLabel* lblIMP = dynamic_cast<CLabel*>(frmInv->Find("labItemIMPnumber"));
	char szBuf[16];
	sprintf( szBuf, "%s", StringSplitNum( IMP )  );
	lblIMP->SetCaption( szBuf );
}

void CEquipMgr::FrameMove(DWORD dwTime)
{
	static CTimeWork time(100);
	if( time.IsTimeOut( dwTime ) )
	{
		CCharacter* pCha = g_stUIBoat.GetHuman();
		if( !pCha ) return;

		SGameAttr* pGameAttr = pCha->getGameAttr();
		if( frmSkill->GetIsShow() )
		{
			// 
			sprintf( szBuf, "%d", pGameAttr->get(ATTR_TP) );
			labPoint->SetCaption( szBuf );

			sprintf( szBuf, "%d", pGameAttr->get(ATTR_LIFETP) );
			labPointLife->SetCaption( szBuf );

			RefreshUpgrade();
		}


		CCharacter* pMainCha = CGameScene::GetMainCha();
		if( pMainCha )
		{
			// 
			CSkillCommand::GetActiveImage()->Next();

			CChaStateMgr* pState = pMainCha->GetStateMgr();

			for( vskill::iterator it=_cancels.begin(); it!=_cancels.end(); it++ )
				(*it)->SetIsActive( pState->HasSkillState( (*it)->nStateID ) );


			if(frmInv->GetIsShow()){
				sprintf( szBuf, "%s", StringSplitNum( pGameAttr->get(ATTR_GD) )  );
				lblGold->SetCaption( szBuf );
				
				for( int i=0; i<enumEQUIP_NUM; i++ ) 
					_imgCharges[i].Next();
				
				CItemCommand* pItem = NULL;
				for( states::iterator it=_charges.begin(); it!=_charges.end(); it++ )
				{
					if( COneCommand* pCmd = cnmEquip[(*it)->sChargeLink] )
					{
						pItem = dynamic_cast<CItemCommand*>( pCmd->GetCommand() );
						if( pItem && pItem->GetItemInfo()->sType==29 )
						{
							pCmd->SetIsShowActive( pState->HasSkillState( (*it)->chID ) );
						}
						else
						{
							pCmd->SetIsShowActive( false );
						}
					}
				}
			}
			
		}
	}
}

void CEquipMgr::RefreshUpgrade()
{
	CForm * f = frmSkill;
	if( !f->GetIsShow() ) return;

	CCharacter* pCha = CGameScene::GetMainCha();
	if( !pCha )			return;

	SGameAttr* pAttr = pCha->getGameAttr();

	lstFightSkill->SetIsShowUpgrade(pAttr->get(ATTR_TP)>0);
	lstLifeSkill->SetIsShowUpgrade(pAttr->get(ATTR_LIFETP)>0);

	RefreshSkillJob( pAttr->get(ATTR_JOB) );
}

void CEquipMgr::RefreshSkillJob( int nJob )
{
	int count = lstFightSkill->GetCount();
	CSkillCommand* tmp = NULL;
	CSkillList* pList = NULL;
	for( int j=0; j<2; j++ )
	{
		pList = GetSkillList( j );
		for( int i=0; i<count; i++ )
		{
			tmp = pList->GetCommand(i);
			if( tmp )
			{
				tmp->GetSkillRecord()->Refresh( nJob );
			}
		}
	}
}

void CEquipMgr::UnfixToGrid( CCommandObj* pItem, int nGridID, int nLink )
{

	_sUnfix.Reset();

	_sUnfix.nLink = nLink;
	_sUnfix.nGridID = nGridID;
	_sUnfix.pItem = pItem;

	_StartUnfix( _sUnfix );
}
/*
void CEquipMgr::_evtItemFormClose( CForm *pForm, bool& IsClose )
{
	if ( g_stUITrade.IsTrading() ){
		IsClose = false;
	}
	
	if(g_stUIEquip.chkLinkEqForm->GetIsChecked()&& g_stUIEquip.frmEquip->GetIsShow()){
		g_stUIEquip.frmEquip->Hide();
	}
}*/

void CEquipMgr::_evtEquipFormClose( CForm *pForm, bool& IsClose ){
	if ( g_stUITrade.IsTrading() ){
		IsClose = false;
	}
	if (g_stUIEquip.chaModel){
		g_stUIEquip.chaModel->SetValid(false);
		g_stUIEquip.refreshChaModel = true;
	}

	g_stUIEquip.GetGoodsGrid()->ResetItemSelections();
}

void CEquipMgr::_evtSpyFormClose( CForm *pForm, bool& IsClose ){
	if (g_stUIEquip.spyModel){
		g_stUIEquip.spyModel->SetValid(false);
	}
}

void CEquipMgr::_evtSpyFormShow(CGuiData *pSender){
	int typeID = g_stUIEquip.eqSpyTarget->getTypeID();
	
	g_stUIEquip.spyModel = CGameApp::GetCurScene()->AddCharacter(typeID );

	lwIByteSet* res_bs = g_Render.GetInterfaceMgr()->res_mgr->GetByteSet();
	BYTE loadtex_flag = res_bs->GetValue(OPT_RESMGR_LOADTEXTURE_MT);
	BYTE loadmesh_flag = res_bs->GetValue(OPT_RESMGR_LOADMESH_MT);
	res_bs->SetValue(OPT_RESMGR_LOADTEXTURE_MT, 0);
	res_bs->SetValue(OPT_RESMGR_LOADMESH_MT, 0);
	res_bs->SetValue(OPT_RESMGR_LOADTEXTURE_MT, loadtex_flag);
	res_bs->SetValue(OPT_RESMGR_LOADMESH_MT, loadmesh_flag);
	g_stUIEquip.spyModel->SetIsForUI(true);
    g_stUIEquip.spyModel->EnableAI(false);
	g_stUIEquip.spyModel->SetHide(true);
	g_stUIEquip.spyModel->GetActor()->SetSleep();
	g_stUIEquip.refreshSpyModel = true;
	
}

void CEquipMgr::_evtEquipFormShow(CGuiData *pSender){
	CCharacter* pCha = g_stUIBoat.GetHuman();
	int typeID = pCha->getTypeID();
	g_stUIEquip.chaModel = CGameApp::GetCurScene()->AddCharacter(typeID);

	lwIByteSet* res_bs = g_Render.GetInterfaceMgr()->res_mgr->GetByteSet();
	BYTE loadtex_flag = res_bs->GetValue(OPT_RESMGR_LOADTEXTURE_MT);
	BYTE loadmesh_flag = res_bs->GetValue(OPT_RESMGR_LOADMESH_MT);
	res_bs->SetValue(OPT_RESMGR_LOADTEXTURE_MT, 0);
	res_bs->SetValue(OPT_RESMGR_LOADMESH_MT, 0);
	res_bs->SetValue(OPT_RESMGR_LOADTEXTURE_MT, loadtex_flag);
	res_bs->SetValue(OPT_RESMGR_LOADMESH_MT, loadmesh_flag);
	if(!g_stUIEquip.chaModel) {
		return;
	}
	g_stUIEquip.chaModel->SetIsForUI(true);
    g_stUIEquip.chaModel->EnableAI(false);
	g_stUIEquip.chaModel->SetHide(true);
	g_stUIEquip.chaModel->GetActor()->SetSleep();
	g_stUIEquip.refreshChaModel = true;
}

/*
void CEquipMgr::_evtItemFormShow(CGuiData *pSender)
{
	if(g_stUIStore.GetStoreForm()->GetIsShow())
	{
		g_stUIEquip.frmItem->SetIsShow(false);
	}
	if(g_stUIEquip.chkLinkEqForm->GetIsChecked()&& !g_stUIEquip.frmEquip->GetIsShow()){
		g_stUIEquip.frmEquip->Show();
	}		
}*/

void CEquipMgr::_SendUnfixData(const stUnfix& unfix, int nUnfixNum)
{
	if( !CGameScene::GetMainCha() ) return;

	if( unfix.nLink==-1 ) return;

    stNetItemUnfix item;
	item.chLinkID = unfix.nLink;
	item.sGridID = unfix.nGridID;
	item.lPosX = unfix.nX;
	item.lPosY = unfix.nY;
	
	if( g_stUIBoat.GetHuman()==CGameScene::GetMainCha() )
	{
		CActor* pActor = g_stUIBoat.GetHuman()->GetActor();
		CEquipState* pState = new CEquipState( pActor );
		pState->SetUnfixData( item );
		pActor->SwitchState( pState );
	}
	else
	{
	    CS_BeginAction( g_stUIBoat.GetHuman(), enumACTION_ITEM_UNFIX, (void*)&item );
	}
}

void CEquipMgr::_evtUnfixDialogEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
{
	if( nMsgType!=CForm::mrYes ) return;

	stNumBox* pBox = (stNumBox*)pSender->GetForm()->GetPointer();
	if( !pBox ) return;

	stUnfix* p = (stUnfix*)pBox->pointer;
	if( !p ) return;

	g_stUIEquip._SendUnfixData( *p, pBox->GetNumber() );
}

void CEquipMgr::_StartUnfix( stUnfix& unfix )
{
	CCommandObj* pItem = unfix.pItem;
	if( !pItem ) return;
	
	if( unfix.nGridID==-2 )
	{
        //if( !pItem->IsAllowThrow() ) 
        //{
        //    g_pGameApp->SysInfo( "" );
        //    return;
        //}

		if( !_GetThrowPos( unfix.nX, unfix.nY ) ) return;		
	}


	if( pItem->GetIsPile() && pItem->GetTotalNum()>1 )
	{
		stNumBox* pBox = g_stUIBox.ShowNumberBox( _evtUnfixDialogEvent, pItem->GetTotalNum() );
		if( pBox )
		{
			pBox->pointer = &unfix;
	        return;
		}
	}

	_SendUnfixData( unfix );
}

CItemCommand* CEquipMgr::GetEquipItem( unsigned int nLink )
{ 
	if( nLink<enumEQUIP_NUM && cnmEquip[nLink] )
		return dynamic_cast<CItemCommand*>(cnmEquip[nLink]->GetCommand());
	return NULL;
}

bool CEquipMgr::IsEquipCom( COneCommand* pCom )
{
	return frmInv==pCom->GetForm() ;
}

CGuiPic* CEquipMgr::GetChargePic( unsigned int n )	
{ 
	if( n<enumEQUIP_NUM )
		return &_imgCharges[n];		
	return NULL;
}

//TODO(Ogge): 
// Implement filter for multi-selecting
void CEquipMgr::_evtRMouseGridEvent(CGuiData *pSender,CCommandObj* pItem,int nGridID)
{
	if( !g_stUIBoat.GetHuman() ) return;

	CItemCommand* pItemCommand = dynamic_cast<CItemCommand*>(pItem);
	if( !pItemCommand)
	{
		return;
	}
	
	if (g_pGameApp->IsShiftPress())
	{
		pItem->ExecRightClick();
		return;
	}

	{
		auto ShowOnlyRelevevantMenuItems = [&](CItemCommand* item, CMenu* menu) {
			for (auto beg = 0, end = menu->GetCount(); beg < end; ++beg) {
				menu->GetMenuItem(beg)->SetIsHide(true);
			}

			// The only option available for locked items is unlock
			if (item->IsLocked()) {
				menu->FindMenuItem("Unlock Item")->SetIsHide(false);
				return;
			}

			const auto allowThrow = static_cast<bool>(item->GetItemInfo()->chIsThrow);
			const auto allowTrade = static_cast<bool>(item->GetItemInfo()->chIsTrade);
			const auto allowDelete = static_cast<bool>(item->GetItemInfo()->chIsDel);

			// Lock option
			if ((allowThrow || allowTrade || allowDelete) && item->GetItemInfo()->IsLockable()) {
				menu->FindMenuItem("Lock Item")->SetIsHide(false);
			}

			if (allowThrow) {
				menu->FindMenuItem("Throw Item")->SetIsHide(false);
			}

			if (allowDelete) {
				menu->FindMenuItem("Delete Item")->SetIsHide(false);
			}

			if (g_stUINpcTrade.GetIsShow() && !item->IsLocked() && allowTrade) {
				menu->FindMenuItem("Sell Item")->SetIsHide(false);
			}
		};

		g_stUIEquip.rightClickItemIndex = pItemCommand->GetIndex();
		ShowOnlyRelevevantMenuItems(pItemCommand, g_stUIEquip.rightClickItemMenu);
		g_stUIEquip.GetItemForm()->PopMenu(g_stUIEquip.rightClickItemMenu, CForm::GetMouseX(), CForm::GetMouseY());
	}
	//TODO: Possible collision with new right click menu method with boat information
	stNetItemInfo info;
	info.chType = mission::VIEW_CHAR_BAG;
	info.sGridID = nGridID;
	CS_BeginAction( g_stUIBoat.GetHuman(), enumACTION_ITEM_INFO, &info );	
}

void CEquipMgr::_evtRepairEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
{
	CS_ItemRepairAnswer( nMsgType==CForm::mrYes );
}

void CEquipMgr::ShowRepairMsg( const char* pItemName, long lMoney )
{
	char szBuf[255] = { 0 };
	FmtLang(szBuf, sizeof(szBuf), GetLanguageString(559), pItemName, lMoney);
	g_stUIBox.ShowSelectBox( _evtRepairEvent, szBuf, true );	
}


void CEquipMgr::CloseAllForm()
{
	if(frmInv && frmInv->GetIsShow())
	{
		frmInv->SetIsShow(false);
	}

	if(frmSkill && frmSkill->GetIsShow())
	{
		frmSkill->SetIsShow(false);
	}
}


// 
int CEquipMgr::GetItemCount(int nID)
{
	int nRet = 0;
	for(int i = 0; i < grdItem->GetMaxNum(); ++i)
	{
		CItemCommand* pItem = dynamic_cast<CItemCommand*>(grdItem->GetItem(i));
		if(pItem && pItem->GetItemInfo())
		{
			if(pItem->GetItemInfo()->lID == nID)
			{
				nRet += pItem->GetTotalNum();
			}
		}
	}

	return nRet;
}

void CEquipMgr::_evtItemFormMouseEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
{
	string strName = pSender->GetName();
	
	if(strName == "btnLock")
	{
		if(g_stUIEquip.GetIsLock())
		{
			// 
			g_stUIDoublePwd.SetType(CDoublePwdMgr::PACKAGE_UNLOCK);
			g_stUIDoublePwd.ShowDoublePwdForm();
		}
		else
		{
			CBoxMgr::ShowSelectBox(_CheckLockMouseEvent, GetLanguageString(824).c_str(), true);
		}
	}
}


void CEquipMgr::SetIsLock(bool bLock)
{
	imgLock->SetIsShow(bLock);
	imgUnLock->SetIsShow(! bLock);
}


bool CEquipMgr::GetIsLock()
{
	return imgLock->GetIsShow();
}


//  MSGBOX 
void CEquipMgr::_CheckLockMouseEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
{
    if( nMsgType == CForm::mrYes ) 
    {
		CS_LockKitbag();
    }
}

void CEquipMgr::_OnDragStates(CGuiData * pSender, int x, int y, DWORD key) {
    if (g_stUIEquip.stateDrags->GetTop() <= 0) {
        g_stUIEquip.stateDrags->SetPos(g_stUIEquip.stateDrags->GetLeft(), 0);
        g_stUIEquip.stateDrags->Refresh();
    }

    if (g_stUIEquip.stateDrags->GetLeft() <= 0) {
        g_stUIEquip.stateDrags->SetPos(0, g_stUIEquip.stateDrags->GetTop());
        g_stUIEquip.stateDrags->Refresh();
    }

    if (g_stUIEquip.stateDrags->GetBottom() >= GetRender().GetScreenHeight()) {
        g_stUIEquip.stateDrags->SetPos(g_stUIEquip.stateDrags->GetLeft(), GetRender().GetScreenHeight() - g_stUIEquip.stateDrags->GetHeight());
        g_stUIEquip.stateDrags->Refresh();
    }

    if (g_stUIEquip.stateDrags->GetRight() >= GetRender().GetScreenWidth()) {
        g_stUIEquip.stateDrags->SetPos(GetRender().GetScreenWidth() - g_stUIEquip.stateDrags->GetWidth() + 20, g_stUIEquip.stateDrags->GetTop());
        g_stUIEquip.stateDrags->Refresh();
    }
}
