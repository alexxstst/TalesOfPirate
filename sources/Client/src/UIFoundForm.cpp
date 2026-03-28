
#include "stdafx.h"
#include "UIFoundForm.h"
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
    CFoundMgr::CFoundMgr()
    {
    }

    CFoundMgr::~CFoundMgr()
    {
    }

    void CFoundMgr::ShowFoundForm(bool bShow/* = true*/)
    {
        if(frmFound->GetIsShow())
        {
            return;
        }
        if(bShow)
        {
            bLock = false;
            bRnd = false;
            ClearCommand();
            ResetDice();
            int rnd = 0;
            rnd = rand() % 6;
            Dice1[rnd]->SetIsShow(true);
            rnd = rand() % 6;
            Dice2[rnd]->SetIsShow(true);
            rnd = rand() % 6;
            Dice3[rnd]->SetIsShow(true);
            btnForgeYes->SetIsEnabled(false);
            btnForgeBig->SetIsEnabled(false);
            btnForgeSmall->SetIsEnabled(false);
            frmFound->Show();
        }
        else
        {
            if(!bLock)
            {
                frmFound->Hide();
            }
        }
    }

    void CFoundMgr::CheckResult(short sRet, const char* num1, const char* num2, const char* num3)
    {
        g_stUIFound.bRnd = false;
        g_stUIFound.bLock = false;
        int num[3] = {-1};
        num[0] = atoi(num1) - 1;
        num[1] = atoi(num2) - 1;
        num[2] = atoi(num3) - 1;
        g_stUIFound.ResetDice();
        g_stUIFound.Dice1[num[0]]->SetIsShow(true);
        g_stUIFound.Dice2[num[1]]->SetIsShow(true);
        g_stUIFound.Dice3[num[2]]->SetIsShow(true);

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
		if(g_stUIEquip.GetGoodsGrid()->GetItem(iFoundPos[1]))
		{
			for(int i = 0; i < (FOUND_COUNT - 1); i++)
			{
				if(NO_USE != iFoundPos[i])
				{
					pItemCommand = dynamic_cast<CItemCommand*>(g_stUIEquip.GetGoodsGrid()->GetItem(iFoundPos[i]));
					if(pItemCommand)
					{
						if(3 == i)
						{
							int iItem = (dynamic_cast<CItemCommand*>(g_stUIFound.cmdFound[1]->GetCommand()))->GetData().GetInstAttr(ITEMATTR_VAL_STR);
							if(pItemCommand->GetItemInfo()->lID == iItem)
							{
								PushItem(i, *pItemCommand, true);
							}
						}
						else if(4 == i)
						{
							int iItem = (dynamic_cast<CItemCommand*>(g_stUIFound.cmdFound[1]->GetCommand()))->GetData().GetInstAttr(ITEMATTR_VAL_CON);
							if(pItemCommand->GetItemInfo()->lID == iItem)
							{
								PushItem(i, *pItemCommand, true);
							}
						}
						else if(5 == i)
						{
							int iItem = (dynamic_cast<CItemCommand*>(g_stUIFound.cmdFound[1]->GetCommand()))->GetData().GetInstAttr(ITEMATTR_VAL_DEX);
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
						iFoundPos[i] = NO_USE;
					}
				}
			}
		}
        btnForgeYes->SetIsEnabled(true);
        btnForgeBig->SetIsEnabled(true);
        btnForgeSmall->SetIsEnabled(true);
    }

    void CFoundMgr::StartTime(short time)
    {
        if(!time)
        {
            bLock = false;
            btnForgeYes->SetIsEnabled(true);
            btnForgeBig->SetIsEnabled(true);
            btnForgeSmall->SetIsEnabled(true);
            return;
        }
        sTime = time * 1000;
        proFound->SetRange(0.0f, (float)sTime);
        bRnd = true;
        proFound->Start(sTime);
    }


    int CFoundMgr::GetComIndex(COneCommand* oneCommand)
    {
        for (int i =0; i < FOUND_COUNT; i++)
        {
		    if (cmdFound[i] == oneCommand)
			    return i;
        }
	    return -1;
    }

    void CFoundMgr::DragToEquipGrid(int index)
    {
        PopItem(index);
    }

    bool CFoundMgr::Init()
    {
        CFormMgr &mgr = CFormMgr::s_Mgr;

        frmFound = mgr.Find("frmFound");
        if(!frmFound)
        {
            ToLogService("common", "frmFound not found.");
            return false;
        }

        proFound = dynamic_cast<CProgressBar*>(frmFound->Find("proFound"));
        if(!proFound)
        {
            ToLogService("common", "frmFound::proFound not found.");
            return false;
        }
        proFound->evtTimeArrive = _ProTimeArriveEvt;

        btnForgeYes = dynamic_cast<CTextButton*>(frmFound->Find("btnForgeYes"));
        if(!btnForgeYes)
        {
            ToLogService("common", "frmFound::btnForgeYes not found.");
            return false;
        }
        btnForgeYes->SetIsEnabled(false);

        btnForgeBig = dynamic_cast<CTextButton*>(frmFound->Find("btnBigBig"));
        if(!btnForgeBig)
        {
            ToLogService("common", "frmFound::btnBigBig not found.");
            return false;
        }
        btnForgeBig->SetIsEnabled(false);

        btnForgeSmall = dynamic_cast<CTextButton*>(frmFound->Find("btnSmallSmall"));
        if(!btnForgeSmall)
        {
            ToLogService("common", "frmFound::btnSmallSmall not found.");
            return false;
        }
        btnForgeSmall->SetIsEnabled(false);

        char buff[32] = {0};
        for(int i = 0; i < FOUND_COUNT; i++)
        {
            sprintf(buff, "cmdItemBase%i", (i+1));
            cmdFound[i] = dynamic_cast<COneCommand*>(frmFound->Find(buff));
            if(!cmdFound[i])
            {
                ToLogService("common", "frmFound::{} not found.", buff);
                return false;
            }
            iFoundPos[i] = NO_USE;
        }

        cmdFound[0]->evtBeforeAccept = _evtDragItemBase1;
        cmdFound[1]->evtBeforeAccept = _evtDragItemBase2;
        cmdFound[2]->evtBeforeAccept = _evtDragItemBase3;
        cmdFound[3]->evtBeforeAccept = _evtDragItemBase4;
        cmdFound[4]->evtBeforeAccept = _evtDragItemBase5;
        cmdFound[5]->evtBeforeAccept = _evtDragItemBase6;

        for(int i = 0; i < 6; i++)
        {
            sprintf(buff, "imgShaiZi1_%i", (i+1));
            Dice1[i] = dynamic_cast<CImage*>(frmFound->Find(buff));
            if(!Dice1[i])
            {
                ToLogService("common", "frmFound::{} not found.", buff);
                return false;
            }
            sprintf(buff, "imgShaiZi2_%i", (i+1));
            Dice2[i] = dynamic_cast<CImage*>(frmFound->Find(buff));
            if(!Dice2[i])
            {
                ToLogService("common", "frmFound::{} not found.", buff);
                return false;
            }
            sprintf(buff, "imgShaiZi3_%i", (i+1));
            Dice3[i] = dynamic_cast<CImage*>(frmFound->Find(buff));
            if(!Dice3[i])
            {
                ToLogService("common", "frmFound::{} not found.", buff);
                return false;
            }
        }

        frmFound->evtClose = _evtCloseFound;
        frmFound->evtEntrustMouseEvent = _evtbtnForgeYes;
        frmFound->SetIsEscClose(true);

        srand(g_pGameApp->GetCurTick());

        bRnd = false;
        bLock = false;
        return true;
    }

    void CFoundMgr::CloseForm()
    {
    }

    void CFoundMgr::ClearCommand(bool bRetry/* = false*/)
    {
        if(bLock)
        {
            return;
        }
        for(int i = 0; i < FOUND_COUNT; i++)
        {
            PopItem(i, bRetry);
        }
        if(proFound->IsRuning())
        {
            proFound->Start(0);
        }
        proFound->SetPosition(0.0f);
    }

    void CFoundMgr::PopItem(int iIndex, bool bRetry/* = false*/)
    {
        //  �Ƿ�����
        if(bLock)
        {
            return;
        }
        // ɾ��Cmd�е�Item����Item����PushItem()����new����
		CItemCommand* pItemCommand =  dynamic_cast<CItemCommand*>(cmdFound[iIndex]->GetCommand());
		if (! pItemCommand)
            return;

		cmdFound[iIndex]->DelCommand();	// �ú�����ɾ��delete Item

		// ��Item��Ӧ����Ʒ���ָ��ɿ���
		CCommandObj* pItem = g_stUIEquip.GetGoodsGrid()->GetItem(iFoundPos[iIndex]);
		if (pItem && (6 != iIndex))
		{
			pItem->SetIsValid(true);
		}

		// ��¼Item����Ʒ���е�λ��
        if(!bRetry)
        {
		    iFoundPos[iIndex] = NO_USE;
        }
    }
    
    void CFoundMgr::PushItem(int iIndex, CItemCommand& rItem, bool bRetry/* = false*/)
    {
        //  �Ƿ�����
        if(bLock)
        {
            return;
        }
        // �жϵ����Ƿ������
		if(! rItem.GetIsValid())
		{
			return;
		}

		// �鿴ԭ����Cmd���Ƿ��Ѿ���Item�ˣ���������Ƴ�
		CItemCommand* pItemCommand =  dynamic_cast<CItemCommand*>(cmdFound[iIndex]->GetCommand());
		if (pItemCommand)
		{
			PopItem(iIndex);
		}

		// ��¼Item����Ʒ���е�λ��
        if(!bRetry)
        {
            iFoundPos[iIndex] = g_stUIEquip.GetGoodsGrid()->GetDragIndex();
        }

        if(6 != iIndex)
        {
		    // ��Item��Ӧ����Ʒ���ҵ�
		    rItem.SetIsValid(false);
        }

		// ��������Item����Cmd�У�������new������PopItem()��ɾ��
		CItemCommand* pItemCmd = new CItemCommand(rItem);
		pItemCmd->SetIsValid(true);
		cmdFound[iIndex]->AddCommand(pItemCmd);

		SetFoundUI();
    }

    void CFoundMgr::SetFoundUI()
    {
        btnForgeYes->SetIsEnabled(true);
        btnForgeBig->SetIsEnabled(true);
        btnForgeSmall->SetIsEnabled(true);
    }

    void CFoundMgr::FrameMove(DWORD dwTime)
    {
        if(bRnd)
        {
            static char buff[16] = {0};
            if(frmFound && frmFound->GetIsShow())
		    {
			    DWORD dwCurrTickCount = g_pGameApp->GetCurTick();

			    if(dwCurrTickCount - m_dwLastTickCount > ERNIE_SPEED)
			    {
				    m_dwLastTickCount = dwCurrTickCount;

                    int rnd = 0;
                    ResetDice();
                    rnd = rand() % 6;
                    Dice1[rnd]->SetIsShow(true);
                    rnd = rand() % 6;
                    Dice2[rnd]->SetIsShow(true);
                    rnd = rand() % 6;
                    Dice3[rnd]->SetIsShow(true);
			    }
		    }
        }
    }

    void CFoundMgr::ResetDice()
    {
        for(int i = 0; i < 6; i++)
        {
            Dice1[i]->SetIsShow(false);
            Dice2[i]->SetIsShow(false);
            Dice3[i]->SetIsShow(false);
        }
    }

    void CFoundMgr::_evtbtnForgeYes(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
    {
        string szName = pSender->GetName();
        if(szName == "btnForgeYes")
        {
            g_stUIFound.btnForgeYes->SetIsEnabled(false);
            //CS_LifeSkill(2, g_stUINpcTalk.GetNpcId());
            CS_Found(g_stUINpcTalk.GetNpcId(), g_stUIFound.iFoundPos, (FOUND_COUNT - 1), 1, true);
            g_stUIFound.bLock = true;
        }
        else if(szName == "btnBigBig")
        {
            if(g_stUIFound.proFound->IsRuning())
            {
                float pos = g_stUIFound.proFound->GetPosition();
                g_stUIFound.proFound->Start(0);
                g_stUIFound.proFound->SetPosition(pos);
                CS_Found(g_stUINpcTalk.GetNpcId(), g_stUIFound.iFoundPos, (FOUND_COUNT - 1), 1);
            }
        }
        else if(szName == "btnSmallSmall")
        {
            if(g_stUIFound.proFound->IsRuning())
            {
                float pos = g_stUIFound.proFound->GetPosition();
                g_stUIFound.proFound->Start(0);
                g_stUIFound.proFound->SetPosition(pos);
                CS_Found(g_stUINpcTalk.GetNpcId(), g_stUIFound.iFoundPos, (FOUND_COUNT - 1), 0);
            }
        }
    }

    void CFoundMgr::_evtCloseFound(CForm* pForm, bool& IsClose)
    {
        if(g_stUIFound.bLock)
        {
            IsClose = true;
            return;
        }
        g_stUIFound.ClearCommand();
        CS_UnlockCharacter();
    }

    void CFoundMgr::_ProTimeArriveEvt(CGuiData *pSender)
    {
        CS_Found(g_stUINpcTalk.GetNpcId(), g_stUIFound.iFoundPos, (FOUND_COUNT - 1), (short)(rand() % 2));
    }

    void CFoundMgr::_evtDragItemBase1(CGuiData *pSender,CCommandObj* pItem,bool& isAccept)
    {
        if(!g_stUIFound.cmdFound[1]->GetCommand())
        {
            g_pGameApp->MsgBox(g_oLangRec.GetString(896));
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
        g_stUIFound.PushItem(0, *pItemCommand);
    }

    void CFoundMgr::_evtDragItemBase2(CGuiData *pSender,CCommandObj* pItem,bool& isAccept)
    {
        CItemCommand* pItemCommand =  dynamic_cast<CItemCommand*>(pItem);
		if( !pItemCommand || !pItemCommand->GetIsValid())
            return;
        if(pItemCommand->GetItemInfo()->lID != 2301)
        {
            return;
        }

        CGoodsGrid* pGood = dynamic_cast<CGoodsGrid*>(CDrag::GetParent());
        if( pGood != g_stUIEquip.GetGoodsGrid() )
            return;
        g_stUIFound.PushItem(1, *pItemCommand);
    }

    void CFoundMgr::_evtDragItemBase3(CGuiData *pSender,CCommandObj* pItem,bool& isAccept)
    {
        if(!g_stUIFound.cmdFound[1]->GetCommand())
        {
            g_pGameApp->MsgBox(g_oLangRec.GetString(896));
            return;
        }
        CItemCommand* pItemCommand =  dynamic_cast<CItemCommand*>(pItem);
		if( !pItemCommand || !pItemCommand->GetIsValid())
            return;
        if(pItemCommand->GetItemInfo()->lID != 1069)
        {
            return;
        }

        CGoodsGrid* pGood = dynamic_cast<CGoodsGrid*>(CDrag::GetParent());
        if( pGood != g_stUIEquip.GetGoodsGrid() )
            return;
        g_stUIFound.PushItem(2, *pItemCommand);
    }

    void CFoundMgr::_evtDragItemBase4(CGuiData *pSender,CCommandObj* pItem,bool& isAccept)
    {
        //  ����1
        if(!g_stUIFound.cmdFound[1]->GetCommand())
        {
            g_pGameApp->MsgBox(g_oLangRec.GetString(896));
            return;
        }
        CItemCommand* pItemCommand =  dynamic_cast<CItemCommand*>(pItem);
		if( !pItemCommand || !pItemCommand->GetIsValid())
            return;
        int iItem = (dynamic_cast<CItemCommand*>(g_stUIFound.cmdFound[1]->GetCommand()))->GetData().GetInstAttr(ITEMATTR_VAL_STR);
        if(pItemCommand->GetItemInfo()->lID != iItem)
        {
            return;
        }

        CGoodsGrid* pGood = dynamic_cast<CGoodsGrid*>(CDrag::GetParent());
        if( pGood != g_stUIEquip.GetGoodsGrid() )
            return;
        g_stUIFound.PushItem(3, *pItemCommand);
    }

    void CFoundMgr::_evtDragItemBase5(CGuiData *pSender,CCommandObj* pItem,bool& isAccept)
    {
        //  ����2
        if(!g_stUIFound.cmdFound[1]->GetCommand())
        {
            g_pGameApp->MsgBox(g_oLangRec.GetString(896));
            return;
        }
        CItemCommand* pItemCommand =  dynamic_cast<CItemCommand*>(pItem);
		if( !pItemCommand || !pItemCommand->GetIsValid())
            return;
        int iItem = (dynamic_cast<CItemCommand*>(g_stUIFound.cmdFound[1]->GetCommand()))->GetData().GetInstAttr(ITEMATTR_VAL_CON);
        if(pItemCommand->GetItemInfo()->lID != iItem)
        {
            return;
        }

        CGoodsGrid* pGood = dynamic_cast<CGoodsGrid*>(CDrag::GetParent());
        if( pGood != g_stUIEquip.GetGoodsGrid() )
            return;
        g_stUIFound.PushItem(4, *pItemCommand);
    }

    void CFoundMgr::_evtDragItemBase6(CGuiData *pSender,CCommandObj* pItem,bool& isAccept)
    {
        //  ����3
        if(!g_stUIFound.cmdFound[1]->GetCommand())
        {
            g_pGameApp->MsgBox(g_oLangRec.GetString(896));
            return;
        }
        CItemCommand* pItemCommand =  dynamic_cast<CItemCommand*>(pItem);
		if( !pItemCommand || !pItemCommand->GetIsValid())
            return;
        int iItem = (dynamic_cast<CItemCommand*>(g_stUIFound.cmdFound[1]->GetCommand()))->GetData().GetInstAttr(ITEMATTR_VAL_DEX);
        if(pItemCommand->GetItemInfo()->lID != iItem)
        {
            return;
        }

        CGoodsGrid* pGood = dynamic_cast<CGoodsGrid*>(CDrag::GetParent());
        if( pGood != g_stUIEquip.GetGoodsGrid() )
            return;
        g_stUIFound.PushItem(5, *pItemCommand);
    }
};

