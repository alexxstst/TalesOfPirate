
#include "stdafx.h"
#include "UIPKSilverForm.h"
#include "uiformmgr.h"
#include "UIGoodsGrid.h"
#include "uiEquipForm.h"
#include "uiItemCommand.h"
#include "uinpctalkform.h"
#include "NetProtocol.h"
#include "packetcmd.h"
#include "GameApp.h"

namespace GUI
{

    CPKSilverMgr::CPKSilverMgr()
    {
    }


    CPKSilverMgr::~CPKSilverMgr()
    {
    }


    void CPKSilverMgr::ShowPKSilverForm(bool bShow/* = true*/)
    {
        if(bShow)
        {
            frmPKSilver->Show();
        }
        else
        {
            frmPKSilver->Hide();
        }
    }


    bool CPKSilverMgr::AddFormAttribute(int idx, const std::string& szName, long sLevel, const std::string& szJob, long lPkval)
    {
        if((idx < 0) || (idx >= MAX_PKSILVER_PLAYER))
        {
            return false;
        }

        if((!labName[idx]) || (!labLevel[idx]) || (!labJob[idx]) || (!labDate[idx]))
        {
            return false;
        }

        if(sLevel < 0)
        {
            labName[idx]->SetCaption("");
            labLevel[idx]->SetCaption("");
            labJob[idx]->SetCaption("");
            labDate[idx]->SetCaption("");
            return true;
        }

        char buff[32] = {0};
        labName[idx]->SetCaption(szName.c_str());

        sprintf(buff, "%li", sLevel);
        labLevel[idx]->SetCaption(buff);

        labJob[idx]->SetCaption(szJob.c_str());

        sprintf(buff, "%li", lPkval);
        labDate[idx]->SetCaption(buff);
        return true;
    }


    bool CPKSilverMgr::Init()
    {
        CFormMgr &mgr = CFormMgr::s_Mgr;

        frmPKSilver = mgr.Find("frmPKSilver");
        if(!frmPKSilver)
        {
            ToLogService("common", "frmPKSilver not found.");
            return false;
        }

        char szTitle[32] = {0};
        for(int i = 0; i < MAX_PKSILVER_PLAYER; i++)
        {
            sprintf(szTitle, "labName_%i", i);
            labName[i] = dynamic_cast<CLabelEx*>(frmPKSilver->Find(szTitle));
            if(! labName[i])
			{
                ToLogService("common", "frmPKSilver:{} not found. ", szTitle);
				return false;
			}

            sprintf(szTitle, "labLevel_%i", i);
            labLevel[i] = dynamic_cast<CLabelEx*>(frmPKSilver->Find(szTitle));
            if(! labLevel[i])
			{
				ToLogService("common", "frmPKSilver:{} not found. ", szTitle);
				return false;
			}

            sprintf(szTitle, "labJob_%i", i);
            labJob[i] = dynamic_cast<CLabelEx*>(frmPKSilver->Find(szTitle));
            if(! labJob[i])
			{
				ToLogService("common", "frmPKSilver:{} not found. ", szTitle);
				return false;
			}

            sprintf(szTitle, "labData_%i", i);
            labDate[i] = dynamic_cast<CLabelEx*>(frmPKSilver->Find(szTitle));
            if(! labDate[i])
			{
				ToLogService("common", "frmPKSilver:{} not found. ", szTitle);
				return false;
			}
        }
        //frmPKSilver->evtEntrustMouseEvent = _evtPKSilverSortBtn;
        return true;
    }


    void CPKSilverMgr::CloseForm()
    {
    }

   // void CPKSilverMgr::_evtPKSilverSortBtn(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
   // {
   //     string szName = pSender->GetName();
   //     if(szName == "btnSort")
   //     {
   //         //  ��������
   //         CGoodsGrid* pGrid  = g_stUIEquip.GetGoodsGrid();
			//CItemCommand* pCmd = NULL;
   //         CItemRecord* pItemRecord = NULL;

			//int nCount = pGrid->GetMaxNum();
			//for(int i = 0; i < nCount; ++i)
			//{
			//	pCmd = dynamic_cast<CItemCommand*>(pGrid->GetItem(i));
			//	if(pCmd)
			//	{
			//		pItemRecord = pCmd->GetItemInfo();

			//		if(pItemRecord && pItemRecord->lID == 1123)
			//		{
   //                     CS_PKSilverSort(g_stUINpcTalk.GetNpcId(), i);
			//			return;
			//		}
			//	}
			//}
   //         g_pGameApp->MsgBox(g_oLangRec.GetString(849));
   //     }
   // }

};
