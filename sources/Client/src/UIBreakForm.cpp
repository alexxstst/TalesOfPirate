
#include "stdafx.h"
#include "UIBreakForm.h"
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
    CBreakMgr::CBreakMgr()
    {
    }

    CBreakMgr::~CBreakMgr()
    {
    }

    void CBreakMgr::ShowBreakForm(bool bShow/* = true*/)
    {
        if(frmBreak->GetIsShow())
        {
            return;
        }
        if(bShow)
        {
            bLock = false;
            bRnd = false;
            ClearCommand();
            btnForgeYes->SetIsEnabled(false);
            btnForgeNo->SetIsEnabled(false);
            frmBreak->Show();
        }
        else
        {
            if(!bLock)
            {
                frmBreak->Hide();
            }
        }
    }

    void CBreakMgr::CheckResult(short sRet, const char* num)
    {
        bRnd = false;
        bLock = false;
        proBreak->Start(0);
        labUsrInput->SetCaption(num);
        if(sRet < 0)
        {
            btnForgeYes->SetIsEnabled(true);
            btnForgeNo->SetIsEnabled(true);
            return;
        }

        ClearCommand(true);
        CItemCommand* pItemCommand = NULL;
        for(int i = 0; i < BREAK_COUNT; i++)
        {
            pItemCommand = NULL;
            if(NO_USE != iBreakPos[i])
            {
                pItemCommand = dynamic_cast<CItemCommand*>(g_stUIEquip.GetGoodsGrid()->GetItem(iBreakPos[i]));
                if(pItemCommand)
                {
                    if(3 == i)
                    {
                        if((pItemCommand->GetItemInfo()->lID >= 2625) && (pItemCommand->GetItemInfo()->lID <= 2639))
                        {
                            PushItem(i, *pItemCommand, true);
                        }
                    }
                    else if(2 == i)
                    {
                        switch(pItemCommand->GetItemInfo()->sType)
                        {
                        case 1:     //  ��
                        case 2:     //  �޽�
                        case 3:     //  ��
                        case 4:     //  ��ǹ
                        case 7:     //  ذ��
                        case 9:     //  ����
                        case 11:     //  ����
                        case 20:     //  ñ��
                        case 22:     //  �·�
                        case 23:     //  ����
                        case 24:     //  Ь��
                        case 27:     //  ����
                            {
                                PushItem(i, *pItemCommand, true);
                            }  break;
                        default:
                            {
                            }   break;
                        }
                    }
                    else
                    {
                        PushItem(i, *pItemCommand, true);
                    }
                }
                else
                {
                    iBreakPos[i] = NO_USE;
                }
            }
        }
        btnForgeYes->SetIsEnabled(true);
        btnForgeNo->SetIsEnabled(true);
    }

    void CBreakMgr::StartTime(short time, const char* num)
    {
        if(!time)
        {
            bLock = false;
            btnForgeYes->SetIsEnabled(true);
            btnForgeNo->SetIsEnabled(true);
            return;
        }
        sTime = time * 1000;
        proBreak->SetRange(0.0f, (float)sTime);
        labNumInput->SetCaption(num);
        proBreak->Start(sTime);
        bRnd = true;
    }

    int CBreakMgr::GetComIndex(COneCommand* oneCommand)
    {
        for (int i =0; i < BREAK_COUNT; i++)
        {
		    if (cmdBreak[i] == oneCommand)
			    return i;
        }
	    return -1;
    }

    void CBreakMgr::DragToEquipGrid(int index)
    {
        PopItem(index);
    }

    bool CBreakMgr::Init()
    {
        CFormMgr &mgr = CFormMgr::s_Mgr;

        frmBreak = mgr.Find("frmBreak");
        if(!frmBreak)
        {
            ToLogService("common", "frmBreak not found.");
            return false;
        }

        labNumInput = dynamic_cast<CLabelEx*>(frmBreak->Find("labNumOutputLeft"));
        if(!labNumInput)
        {
            ToLogService("common", "frmCompose::labNumOutputLeft not found.");
            return false;
        }

        labUsrInput = dynamic_cast<CLabelEx*>(frmBreak->Find("labNumOutputRight"));
        if(!labUsrInput)
        {
            ToLogService("common", "frmCompose::labNumOutputRight not found.");
            return false;
        }

        proBreak = dynamic_cast<CProgressBar*>(frmBreak->Find("proBreak"));
        if(!proBreak)
        {
            ToLogService("common", "frmBreak::proBreak not found.");
            return false;
        }
        proBreak->evtTimeArrive = _ProTimeArriveEvt;

        btnForgeYes = dynamic_cast<CTextButton*>(frmBreak->Find("btnForgeYes"));
        if(!btnForgeYes)
        {
            ToLogService("common", "frmBreak::btnForgeYes not found.");
            return false;
        }
        btnForgeYes->SetIsEnabled(false);

        btnForgeNo = dynamic_cast<CTextButton*>(frmBreak->Find("btnForgeNo"));
        if(!btnForgeNo)
        {
            ToLogService("common", "frmBreak::btnForgeNo not found.");
            return false;
        }
        btnForgeNo->SetIsEnabled(false);

        char buff[32] = {0};
        for(int i = 0; i < BREAK_COUNT; i++)
        {
            sprintf(buff, "cmdItemBase%i", (i+1));
            cmdBreak[i] = dynamic_cast<COneCommand*>(frmBreak->Find(buff));
            if(!cmdBreak[i])
            {
                ToLogService("common", "frmBreak::{} not found.", buff);
                return false;
            }
            iBreakPos[i] = NO_USE;
        }

        cmdBreak[0]->evtBeforeAccept = _evtDragItemBase1;
        cmdBreak[1]->evtBeforeAccept = _evtDragItemBase2;
        cmdBreak[2]->evtBeforeAccept = _evtDragItemBase3;
        cmdBreak[3]->evtBeforeAccept = _evtDragItemBase4;

        frmBreak->evtClose = _evtCloseBreak;
        frmBreak->evtEntrustMouseEvent = _evtbtnForgeYes;

        srand(g_pGameApp->GetCurTick());
        bRnd = false;
        bLock = false;
        return true;
    }

    void CBreakMgr::CloseForm()
    {
    }

    void CBreakMgr::ClearCommand(bool bRetry/* = false*/)
    {
        if(bLock)
        {
            return;
        }
        for(int i = 0; i < BREAK_COUNT; i++)
        {
            PopItem(i, bRetry);
        }
        if(proBreak->IsRuning())
        {
            proBreak->Start(0);
        }
        proBreak->SetPosition(0.0f);
    }

    void CBreakMgr::PopItem(int iIndex, bool bRetry/* = false*/)
    {
        //  �Ƿ�����
        if(bLock)
        {
            return;
        }
        // ɾ��Cmd�е�Item����Item����PushItem()����new����
		CItemCommand* pItemCommand =  dynamic_cast<CItemCommand*>(cmdBreak[iIndex]->GetCommand());
		if (! pItemCommand)
            return;

		cmdBreak[iIndex]->DelCommand();	// �ú�����ɾ��delete Item

		// ��Item��Ӧ����Ʒ���ָ��ɿ���
		CCommandObj* pItem = g_stUIEquip.GetGoodsGrid()->GetItem(iBreakPos[iIndex]);
		if (pItem)
		{
			pItem->SetIsValid(true);
		}

		// ��¼Item����Ʒ���е�λ��
        if(!bRetry)
        {
            iBreakPos[iIndex] = NO_USE;
        }
    }

    void CBreakMgr::PushItem(int iIndex, CItemCommand& rItem, bool bRetry/* = false*/)
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
		CItemCommand* pItemCommand =  dynamic_cast<CItemCommand*>(cmdBreak[iIndex]->GetCommand());
		if (pItemCommand)
		{
			PopItem(iIndex);
		}

		// ��¼Item����Ʒ���е�λ��
        if(!bRetry)
        {
            iBreakPos[iIndex] = g_stUIEquip.GetGoodsGrid()->GetDragIndex();
        }

		// ��Item��Ӧ����Ʒ���ҵ�
		rItem.SetIsValid(false);

		// ��������Item����Cmd�У�������new������PopItem()��ɾ��
		CItemCommand* pItemCmd = new CItemCommand(rItem);
		pItemCmd->SetIsValid(true);
		cmdBreak[iIndex]->AddCommand(pItemCmd);

		SetBreakUI();
    }

    void CBreakMgr::SetBreakUI()
    {
        btnForgeYes->SetIsEnabled(true);
        btnForgeNo->SetIsEnabled(true);
    }

    void CBreakMgr::FrameMove(DWORD dwTime)
    {
        if(bRnd)
        {
            static char buff[16] = {0};
            if(frmBreak && frmBreak->GetIsShow())
		    {
			    DWORD dwCurrTickCount = g_pGameApp->GetCurTick();

			    if(dwCurrTickCount - m_dwLastTickCount > ERNIE_SPEED)
			    {
				    m_dwLastTickCount = dwCurrTickCount;

                    int rnd = (rand() % (99999 - 10000 + 1)) + 10000;
                    sprintf(buff, "%i", rnd);
                    labUsrInput->SetCaption(buff);
			    }
		    }
        }
    }

    void CBreakMgr::_evtbtnForgeYes(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
    {
        string szName = pSender->GetName();
        if(szName == "btnForgeYes")
        {
            g_stUIBreak.btnForgeYes->SetIsEnabled(false);
            //CS_LifeSkill(1, g_stUINpcTalk.GetNpcId());
            CS_Break(g_stUINpcTalk.GetNpcId(), g_stUIBreak.iBreakPos, BREAK_COUNT, true);
            g_stUIBreak.bLock = true;
        }
        else if(szName == "btnForgeNo")
        {
            if(g_stUIBreak.proBreak->IsRuning())
            {
                float pos = g_stUIBreak.proBreak->GetPosition();
                g_stUIBreak.proBreak->Start(0);
                g_stUIBreak.proBreak->SetPosition(pos);
                CS_Break(g_stUINpcTalk.GetNpcId(), g_stUIBreak.iBreakPos, BREAK_COUNT);
            }
        }
    }

    void CBreakMgr::_evtCloseBreak(CForm* pForm, bool& IsClose)
    {
        if(g_stUIBreak.bLock)
        {
            IsClose = true;
            return;
        }
        g_stUIBreak.ClearCommand();
        CS_UnlockCharacter();
    }

    void CBreakMgr::_ProTimeArriveEvt(CGuiData *pSender)
    {
        CS_Break(g_stUINpcTalk.GetNpcId(), g_stUIBreak.iBreakPos, BREAK_COUNT);
    }

    void CBreakMgr::_evtDragItemBase1(CGuiData *pSender,CCommandObj* pItem,bool& isAccept)
    {
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
        g_stUIBreak.PushItem(0, *pItemCommand);
    }

    void CBreakMgr::_evtDragItemBase2(CGuiData *pSender,CCommandObj* pItem,bool& isAccept)
    {
        CItemCommand* pItemCommand =  dynamic_cast<CItemCommand*>(pItem);
		if( !pItemCommand || !pItemCommand->GetIsValid())
            return;
        if(pItemCommand->GetItemInfo()->lID != 1070)
        {
            return;
        }

        CGoodsGrid* pGood = dynamic_cast<CGoodsGrid*>(CDrag::GetParent());
        if( pGood != g_stUIEquip.GetGoodsGrid() )
            return;
        g_stUIBreak.PushItem(1, *pItemCommand);
    }

    void CBreakMgr::_evtDragItemBase3(CGuiData *pSender,CCommandObj* pItem,bool& isAccept)
    {
        CItemCommand* pItemCommand =  dynamic_cast<CItemCommand*>(pItem);
		if( !pItemCommand || !pItemCommand->GetIsValid())
            return;
        switch(pItemCommand->GetItemInfo()->sType)
        {
        case 1:     //  ��
        case 2:     //  �޽�
        case 3:     //  ��
        case 4:     //  ��ǹ
        case 7:     //  ذ��
        case 9:     //  ����
        case 11:     //  ����
        case 20:     //  ñ��
        case 22:     //  �·�
        case 23:     //  ����
        case 24:     //  Ь��
        case 27:     //  ����
            {}  break;
        default:
            {
                return;
            }   break;
        }

        CGoodsGrid* pGood = dynamic_cast<CGoodsGrid*>(CDrag::GetParent());
        if( pGood != g_stUIEquip.GetGoodsGrid() )
            return;
        g_stUIBreak.PushItem(2, *pItemCommand);
    }

    void CBreakMgr::_evtDragItemBase4(CGuiData *pSender,CCommandObj* pItem,bool& isAccept)
    {
        CItemCommand* pItemCommand =  dynamic_cast<CItemCommand*>(pItem);
		if( !pItemCommand || !pItemCommand->GetIsValid())
            return;
        if((pItemCommand->GetItemInfo()->lID < 2625) || (pItemCommand->GetItemInfo()->lID > 2639))
        {
            return;
        }

        CGoodsGrid* pGood = dynamic_cast<CGoodsGrid*>(CDrag::GetParent());
        if( pGood != g_stUIEquip.GetGoodsGrid() )
            return;
        g_stUIBreak.PushItem(3, *pItemCommand);
    }
};

