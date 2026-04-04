
#include "stdafx.h"
#include "UICookingForm.h"
#include "uiformmgr.h"
#include "UIGoodsGrid.h"
#include "uiEquipForm.h"
#include "uiItemCommand.h"
#include "uinpctalkform.h"
#include "uiprogressbar.h"
#include "NetProtocol.h"
#include "packetCmd.h"
#include "GameApp.h"
using namespace std;

namespace GUI
{
    CCookingMgr::CCookingMgr()
    {
    }

    CCookingMgr::~CCookingMgr()
    {
    }

    void CCookingMgr::ShowCookingForm(bool bShow/* = true*/)
    {
        if(frmCooking->GetIsShow())
        {
            return;
        }
        if(bShow)
        {
            bLock = false;
            ClearCommand();
            btnForgeYes->SetIsEnabled(false);
            btnForgeNo->SetIsEnabled(false);
            frmCooking->Show();
        }
        else
        {
            if(!bLock)
            {
                frmCooking->Hide();
            }
        }
    }

    void CCookingMgr::CheckResult(short sRet)
    {
        CItemRecord*    pCItemRec = NULL;
        CItemCommand*   pItemCommand = NULL;
        ClearCommand(true);
        if(sRet >= 0)
        {
            pItemCommand = dynamic_cast<CItemCommand*>(g_stUIEquip.GetGoodsGrid()->GetItem(sRet));
            if(pItemCommand)
            {
                PushItem(6, *pItemCommand, true);
            }
        }
		if(g_stUIEquip.GetGoodsGrid()->GetItem(iCookingPos[1]))
		{
			for(int i = 0; i < (COOKING_COUNT - 1); i++)
			{
				if(NO_USE != iCookingPos[i])
				{
					pItemCommand = dynamic_cast<CItemCommand*>(g_stUIEquip.GetGoodsGrid()->GetItem(iCookingPos[i]));
					if(pItemCommand)
					{
						if(3 == i)
						{
							int iItem = (dynamic_cast<CItemCommand*>(g_stUICooking.cmdCooking[1]->GetCommand()))->GetData().GetInstAttr(ITEMATTR_VAL_STR);
							if(pItemCommand->GetItemInfo()->lID == iItem)
							{
								PushItem(i, *pItemCommand, true);
							}
						}
						else if(4 == i)
						{
							int iItem = (dynamic_cast<CItemCommand*>(g_stUICooking.cmdCooking[1]->GetCommand()))->GetData().GetInstAttr(ITEMATTR_VAL_CON);
							if(pItemCommand->GetItemInfo()->lID == iItem)
							{
								PushItem(i, *pItemCommand, true);
							}
						}
						else if(5 == i)
						{
							int iItem = (dynamic_cast<CItemCommand*>(g_stUICooking.cmdCooking[1]->GetCommand()))->GetData().GetInstAttr(ITEMATTR_VAL_DEX);
							if(pItemCommand->GetItemInfo()->lID == iItem)
							{
								PushItem(i, *pItemCommand, true);
							}
						}
						else
						{
							PushItem(i, *pItemCommand, true);
						}
					}
					else
					{
						iCookingPos[i] = NO_USE;
					}
				}
			}
		}
        btnForgeYes->SetIsEnabled(true);
        btnForgeNo->SetIsEnabled(true);
    }

    void CCookingMgr::StartTime(short time)
    {
        if(!time)
        {
            bLock = false;
            btnForgeYes->SetIsEnabled(true);
            btnForgeNo->SetIsEnabled(true);
            return;
        }
        sTime = time * 1000;
        proCooking->SetRange(0.0f, (float)sTime);
        dwStartTime = g_pGameApp->GetCurTick();
        proCooking->Start(sTime);
    }

    int CCookingMgr::GetComIndex(COneCommand* oneCommand)
    {
        for (int i =0; i < COOKING_COUNT; i++)
        {
		    if (cmdCooking[i] == oneCommand)
			    return i;
        }
	    return -1;
    }

    void CCookingMgr::DragToEquipGrid(int index)
    {
        PopItem(index);
    }

    bool CCookingMgr::Init()
    {
        CFormMgr &mgr = CFormMgr::s_Mgr;

        frmCooking = mgr.Find("frmCooking");
        if(!frmCooking)
        {
            ToLogService("common", "frmCooking not found.");
            return false;
        }

        proCooking = dynamic_cast<CProgressBar*>(frmCooking->Find("proCooking"));
        if(!proCooking)
        {
            ToLogService("common", "frmCooking::proCooking not found.");
            return false;
        }
        proCooking->evtTimeArrive = _ProTimeArriveEvt;

        btnForgeYes = dynamic_cast<CTextButton*>(frmCooking->Find("btnForgeYes"));
        if(!btnForgeYes)
        {
            ToLogService("common", "frmCooking::btnForgeYes not found.");
            return false;
        }
        btnForgeYes->SetIsEnabled(false);

        btnForgeNo = dynamic_cast<CTextButton*>(frmCooking->Find("btnForgeNo"));
        if(!btnForgeNo)
        {
            ToLogService("common", "frmCooking::btnForgeNo not found.");
            return false;
        }
        btnForgeNo->SetIsEnabled(false);

        char buff[32] = {0};
        for(int i = 0; i < COOKING_COUNT; i++)
        {
            sprintf(buff, "cmdItemBase%i", (i+1));
            cmdCooking[i] = dynamic_cast<COneCommand*>(frmCooking->Find(buff));
            if(!cmdCooking[i])
            {
                ToLogService("common", "frmCooking::{} not found.", buff);
                return false;
            }
            iCookingPos[i] = NO_USE;
        }

        cmdCooking[0]->evtBeforeAccept = _evtDragItemBase1;
        cmdCooking[1]->evtBeforeAccept = _evtDragItemBase2;
        cmdCooking[2]->evtBeforeAccept = _evtDragItemBase3;
        cmdCooking[3]->evtBeforeAccept = _evtDragItemBase4;
        cmdCooking[4]->evtBeforeAccept = _evtDragItemBase5;
        cmdCooking[5]->evtBeforeAccept = _evtDragItemBase6;


        frmCooking->evtClose = _evtCloseCooking;
        frmCooking->evtEntrustMouseEvent = _evtbtnForgeYes;
        dwStartTime = 0;
        bLock = false;
        return true;
    }

    void CCookingMgr::CloseForm()
    {
    }

    void CCookingMgr::ClearCommand(bool bRetry/* = false*/)
    {
        if(bLock)
        {
            return;
        }
        for(int i = 0; i < COOKING_COUNT; i++)
        {
            PopItem(i, bRetry);
        }
        if(proCooking->IsRuning())
        {
            proCooking->Start(0);
        }
        proCooking->SetPosition(0.0f);
    }

    void CCookingMgr::PopItem(int iIndex, bool bRetry/* = false*/)
    {
        //  
        if(bLock)
        {
            return;
        }
        // CmdItemItemPushItem()new
		CItemCommand* pItemCommand =  dynamic_cast<CItemCommand*>(cmdCooking[iIndex]->GetCommand());
		if (! pItemCommand)
            return;

		cmdCooking[iIndex]->DelCommand();	// delete Item

		// Item
		CCommandObj* pItem = g_stUIEquip.GetGoodsGrid()->GetItem(iCookingPos[iIndex]);
		if (pItem && (6 != iIndex))
		{
			pItem->SetIsValid(true);
		}

		// Item
        if(!bRetry)
        {
		    iCookingPos[iIndex] = NO_USE;
        }
    }

    void CCookingMgr::PushItem(int iIndex, CItemCommand& rItem, bool bRetry/* = false*/)
    {
        //  
        if(bLock)
        {
            return;
        }
        // 
		if(! rItem.GetIsValid())
		{
			return;
		}

		// CmdItem
		CItemCommand* pItemCommand =  dynamic_cast<CItemCommand*>(cmdCooking[iIndex]->GetCommand());
		if (pItemCommand)
		{
			PopItem(iIndex);
		}

		// Item
        if(!bRetry)
        {
            iCookingPos[iIndex] = g_stUIEquip.GetGoodsGrid()->GetDragIndex();
        }

        if(6 != iIndex)
        {
		    // Item
		    rItem.SetIsValid(false);
        }

		// ItemCmdnewPopItem()
		CItemCommand* pItemCmd = new CItemCommand(rItem);
		pItemCmd->SetIsValid(true);
		cmdCooking[iIndex]->AddCommand(pItemCmd);

		SetCookingUI();
    }

    void CCookingMgr::SetCookingUI()
    {
        btnForgeYes->SetIsEnabled(true);
        btnForgeNo->SetIsEnabled(true);
    }

    void CCookingMgr::_evtbtnForgeYes(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
    {
        string szName = pSender->GetName();
        if(szName == "btnForgeYes")
        {
            g_stUICooking.btnForgeYes->SetIsEnabled(false);
            //CS_LifeSkill(3, g_stUINpcTalk.GetNpcId());
            CS_Cooking(g_stUINpcTalk.GetNpcId(), g_stUICooking.iCookingPos, (COOKING_COUNT - 1), 0, true);
            g_stUICooking.bLock = true;
        }
        else if(szName == "btnForgeNo")
        {
            if(g_stUICooking.proCooking->IsRuning())
            {
                g_stUICooking.proCooking->Start(0);
                g_stUICooking.dwEndTime = g_pGameApp->GetCurTick();
                DWORD Percent = g_stUICooking.dwEndTime - g_stUICooking.dwStartTime;
                Percent = (DWORD)((float)((float)Percent / (float)g_stUICooking.sTime) * 100);
                CS_Cooking(g_stUINpcTalk.GetNpcId(), g_stUICooking.iCookingPos, (COOKING_COUNT - 1), (short)Percent);
                g_stUICooking.bLock = false;
            }
        }
    }

    void CCookingMgr::_evtCloseCooking(CForm* pForm, bool& IsClose)
    {
        if(g_stUICooking.bLock)
        {
            IsClose = true;
            return;
        }
        g_stUICooking.ClearCommand();
        CS_UnlockCharacter();
    }

    void CCookingMgr::_ProTimeArriveEvt(CGuiData *pSender)
    {
        g_stUICooking.proCooking->SetPosition((float)g_stUICooking.sTime);
        CS_Cooking(g_stUINpcTalk.GetNpcId(), g_stUICooking.iCookingPos, (COOKING_COUNT - 1), 100);
        g_stUICooking.bLock = false;
    }

    void CCookingMgr::_evtDragItemBase1(CGuiData *pSender,CCommandObj* pItem,bool& isAccept)
    {
        if(!g_stUICooking.cmdCooking[1]->GetCommand())
        {
            g_pGameApp->MsgBox("%s", GetLanguageString(896).c_str());
            return;
        }
        CItemCommand* pItemCommand =  dynamic_cast<CItemCommand*>(pItem);
		if( !pItemCommand || !pItemCommand->GetIsValid())
            return;
        if(pItemCommand->GetItemInfo()->sType != 59)
        {
            return;
        }

        CGoodsGrid* pGood = dynamic_cast<CGoodsGrid*>(CDrag::GetParent());
        if( pGood != g_stUIEquip.GetGoodsGrid() )
            return;
        g_stUICooking.PushItem(0, *pItemCommand);
    }

    void CCookingMgr::_evtDragItemBase2(CGuiData *pSender,CCommandObj* pItem,bool& isAccept)
    {
        CItemCommand* pItemCommand =  dynamic_cast<CItemCommand*>(pItem);
		if( !pItemCommand || !pItemCommand->GetIsValid())
            return;
        if(pItemCommand->GetItemInfo()->lID != 2302)
        {
            return;
        }

        CGoodsGrid* pGood = dynamic_cast<CGoodsGrid*>(CDrag::GetParent());
        if( pGood != g_stUIEquip.GetGoodsGrid() )
            return;
        g_stUICooking.PushItem(1, *pItemCommand);
    }

    void CCookingMgr::_evtDragItemBase3(CGuiData *pSender,CCommandObj* pItem,bool& isAccept)
    {
        if(!g_stUICooking.cmdCooking[1]->GetCommand())
        {
            g_pGameApp->MsgBox("%s", GetLanguageString(896).c_str());
            return;
        }
        CItemCommand* pItemCommand =  dynamic_cast<CItemCommand*>(pItem);
		if( !pItemCommand || !pItemCommand->GetIsValid())
            return;
        if(pItemCommand->GetItemInfo()->lID != 1067)
        {
            return;
        }

        CGoodsGrid* pGood = dynamic_cast<CGoodsGrid*>(CDrag::GetParent());
        if( pGood != g_stUIEquip.GetGoodsGrid() )
            return;
        g_stUICooking.PushItem(2, *pItemCommand);
    }

    void CCookingMgr::_evtDragItemBase4(CGuiData *pSender,CCommandObj* pItem,bool& isAccept)
    {
        //  1
        if(!g_stUICooking.cmdCooking[1]->GetCommand())
        {
            g_pGameApp->MsgBox("%s", GetLanguageString(896).c_str());
            return;
        }
        CItemCommand* pItemCommand =  dynamic_cast<CItemCommand*>(pItem);
		if( !pItemCommand || !pItemCommand->GetIsValid())
            return;
        int iItem = (dynamic_cast<CItemCommand*>(g_stUICooking.cmdCooking[1]->GetCommand()))->GetData().GetInstAttr(ITEMATTR_VAL_STR);
        if(pItemCommand->GetItemInfo()->lID != iItem)
        {
            return;
        }

        CGoodsGrid* pGood = dynamic_cast<CGoodsGrid*>(CDrag::GetParent());
        if( pGood != g_stUIEquip.GetGoodsGrid() )
            return;
        g_stUICooking.PushItem(3, *pItemCommand);
    }

    void CCookingMgr::_evtDragItemBase5(CGuiData *pSender,CCommandObj* pItem,bool& isAccept)
    {
        //  2
        if(!g_stUICooking.cmdCooking[1]->GetCommand())
        {
            g_pGameApp->MsgBox("%s", GetLanguageString(896).c_str());
            return;
        }
        CItemCommand* pItemCommand =  dynamic_cast<CItemCommand*>(pItem);
		if( !pItemCommand || !pItemCommand->GetIsValid())
            return;
        int iItem = (dynamic_cast<CItemCommand*>(g_stUICooking.cmdCooking[1]->GetCommand()))->GetData().GetInstAttr(ITEMATTR_VAL_CON);
        if(pItemCommand->GetItemInfo()->lID != iItem)
        {
            return;
        }

        CGoodsGrid* pGood = dynamic_cast<CGoodsGrid*>(CDrag::GetParent());
        if( pGood != g_stUIEquip.GetGoodsGrid() )
            return;
        g_stUICooking.PushItem(4, *pItemCommand);
    }

    void CCookingMgr::_evtDragItemBase6(CGuiData *pSender,CCommandObj* pItem,bool& isAccept)
    {
        //  3
        if(!g_stUICooking.cmdCooking[1]->GetCommand())
        {
            g_pGameApp->MsgBox("%s", GetLanguageString(896).c_str());
            return;
        }
        CItemCommand* pItemCommand =  dynamic_cast<CItemCommand*>(pItem);
		if( !pItemCommand || !pItemCommand->GetIsValid())
            return;
        int iItem = (dynamic_cast<CItemCommand*>(g_stUICooking.cmdCooking[1]->GetCommand()))->GetData().GetInstAttr(ITEMATTR_VAL_DEX);
        if(pItemCommand->GetItemInfo()->lID != iItem)
        {
            return;
        }

        CGoodsGrid* pGood = dynamic_cast<CGoodsGrid*>(CDrag::GetParent());
        if( pGood != g_stUIEquip.GetGoodsGrid() )
            return;
        g_stUICooking.PushItem(5, *pItemCommand);
    }
};


