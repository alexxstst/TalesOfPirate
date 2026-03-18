#include "StdAfx.h"
#include "UIPKDialog.h"
#include "NetProtocol.h"
#include "UIFormMgr.h"
#include "UILabel.h"
#include "UIForm.h"
#include "PacketCmd.h"
#include "UIListView.h"
#include "UIList.h"
#include "UIItem.h"
#include "GameApp.h"

using namespace std;

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
bool CPkDialog::Init()
{
	CFormMgr &mgr = CFormMgr::s_Mgr;

	{	// pk ��ʼǰ�ĶԻ���
		frmTeamPkStart = mgr.Find("frmTeamPK");
		if ( !frmTeamPkStart)
		{
			LG("gui", g_oLangRec.GetString(744));
			return false;
		}
		frmTeamPkStart->evtEntrustMouseEvent = _MainMousePkStartEvent;
		frmTeamPkStart->SetIsEscClose(false);

		char szBuf[32];
		for (int i(0); i<TEAM_NUM; i++)
		{
			sprintf(szBuf, "lstTeam%d", i);
			lvStartTeams[i] = dynamic_cast<CListView*>(frmTeamPkStart->Find(szBuf));
			if (!lvStartTeams[i]) 
				return Error(g_oLangRec.GetString(616),
							 frmTeamPkStart->GetName(), 
							 szBuf);

			CItemRow* pRow(0);
			CItem* pItem(0);
			lvStartTeams[i]->GetList()->GetItems()->Clear();
			for (int j(0); j<MEM_NUM; j++)
			{
				pRow = lvStartTeams[i]->GetList()->GetItems()->NewItem();
				for( int k(0); k<S_ITEM_NUM; k++)
				{
					pItem = new CItem();
					pItem->SetColor(COLOR_BLACK);
					pRow->SetIndex(k, pItem);
				}
			}
		}
	}

	{
		 // pk ������ĶԻ���
		//frmTeamPkEnd = mgr.Find("frmfrmTeamPkStart");
		//if ( !frmTeamPkEnd)
		//{
		//	LG("gui", "npc.clu��û�ж������frmfrmTeamPk");
		//	return false;
		//}
		//frmTeamPkEnd->evtEntrustMouseEvent = _MainMousePkEndEvent;

		//char szBuf[32];
		//for (int i(0); i<TEAM_NUM; i++)
		//{
		//	sprintf(szBuf, "lvEndTeam%d", i);
		//	lvEndTeams[i] = dynamic_cast<CListView*>(frmTeamPkEnd->Find(szBuf));
		//	if (!lvEndTeams[i]) 
		//		return Error("npc.clu����<%s>���Ҳ����ؼ�<%s>",
		//					 frmTeamPkEnd->GetName(), 
		//					 szBuf);
		//}

	}

	return true;
}

//-----------------------------------------------------------------------------
void CPkDialog::SetStartDialogContent(const stNetTeamFightAsk & sNetTeamFightAsk)
{

	ClearStartDialog();

	int iLeftNum = static_cast<int>(sNetTeamFightAsk.chSideNum1);
	int iRightNum = static_cast<int>(sNetTeamFightAsk.chSideNum2);

	char szBuf[128];
	for (int i(0); i<iLeftNum; i++)
	{
		lvStartTeams[TEAM_LEFT]->GetList()->GetItems()->GetItem(i)->GetIndex(0)->SetString(sNetTeamFightAsk.Info[i].szName.c_str());
		lvStartTeams[TEAM_LEFT]->GetList()->GetItems()->GetItem(i)->GetIndex(1)->SetString(sNetTeamFightAsk.Info[i].szJob.c_str());
		sprintf(szBuf, "%d", static_cast<int>(sNetTeamFightAsk.Info[i].chLv));
		lvStartTeams[TEAM_LEFT]->GetList()->GetItems()->GetItem(i)->GetIndex(2)->SetString(szBuf);
		sprintf(szBuf, "%d", static_cast<int>(sNetTeamFightAsk.Info[i].usVictoryNum));
		lvStartTeams[TEAM_LEFT]->GetList()->GetItems()->GetItem(i)->GetIndex(3)->SetString(szBuf);
		sprintf(szBuf, "%d", static_cast<int>(sNetTeamFightAsk.Info[i].usFightNum));
		lvStartTeams[TEAM_LEFT]->GetList()->GetItems()->GetItem(i)->GetIndex(4)->SetString(szBuf);

	}

	iRightNum = static_cast<int>(sNetTeamFightAsk.chSideNum2);
	iLeftNum = static_cast<int>(sNetTeamFightAsk.chSideNum1);
	for (int i(0); i<iRightNum; i++)
	{
		lvStartTeams[TEAM_RIGHT]->GetList()->GetItems()->GetItem(i)->GetIndex(0)->SetString(sNetTeamFightAsk.Info[i+iLeftNum].szName.c_str());
		lvStartTeams[TEAM_RIGHT]->GetList()->GetItems()->GetItem(i)->GetIndex(1)->SetString(sNetTeamFightAsk.Info[i+iLeftNum].szJob.c_str());
		sprintf(szBuf, "%d", static_cast<int>(sNetTeamFightAsk.Info[i+iLeftNum].chLv));
		lvStartTeams[TEAM_RIGHT]->GetList()->GetItems()->GetItem(i)->GetIndex(2)->SetString(szBuf);
		sprintf(szBuf, "%d", static_cast<int>(sNetTeamFightAsk.Info[i+iLeftNum].usVictoryNum));
		lvStartTeams[TEAM_RIGHT]->GetList()->GetItems()->GetItem(i)->GetIndex(3)->SetString(szBuf);
		sprintf(szBuf, "%d", static_cast<int>(sNetTeamFightAsk.Info[i+iLeftNum].usFightNum));
		lvStartTeams[TEAM_RIGHT]->GetList()->GetItems()->GetItem(i)->GetIndex(4)->SetString(szBuf);
	}


}

//-----------------------------------------------------------------------------
void CPkDialog::SetEndDialogContent()
{
}

//-----------------------------------------------------------------------------
void CPkDialog::ClearStartDialog()
{
	for (int i(0); i<TEAM_NUM; i++)
	{
		for (int j(0); j<MEM_NUM; j++)
			for (int k(0); k<S_ITEM_NUM; k++)
				lvStartTeams[i]->GetList()->GetItems()->GetItem(j)->GetIndex(k)->SetString("");
	}
}

//-----------------------------------------------------------------------------
string CPkDialog::ShowStartDialogDebug(const stNetTeamFightAsk & sNetTeamFightAsk)
{
	int iLeftNum = static_cast<int>(sNetTeamFightAsk.chSideNum1);
	int iRightNum = static_cast<int>(sNetTeamFightAsk.chSideNum2);

	string sShow;
	char szBuf[1024];
	for (int i(0); i<iLeftNum; i++)
	{
		sprintf(szBuf, "%d\t%s\t%s\n", sNetTeamFightAsk.Info[i].chLv, sNetTeamFightAsk.Info[i].szJob.c_str(), sNetTeamFightAsk.Info[i].szName.c_str());
		sShow += szBuf;
	}

	for (int i(0); i<iRightNum; i++)
	{
		sprintf(szBuf, "%s\t%s\t%d\n", sNetTeamFightAsk.Info[i+iLeftNum].szName.c_str(), sNetTeamFightAsk.Info[i+iLeftNum].szJob.c_str(), sNetTeamFightAsk.Info[i+iLeftNum].chLv);
		sShow += szBuf;
	}

	return sShow;
}

//-----------------------------------------------------------------------------
void CPkDialog::ClearEndDialog()
{
	for (int i(0); i<TEAM_NUM; i++)
	{
		for (int j(0); j<MEM_NUM; j++)
			for (int k(0); k<E_ITEM_NUM; k++)
				lvEndTeams[i]->GetList()->GetItems()->GetItem(j)->GetIndex(k)->SetString("");
	}
}

//-----------------------------------------------------------------------------
void CPkDialog::ShowStartDialog(bool bShow)
{
	frmTeamPkStart->SetIsShow(bShow);
}

//-----------------------------------------------------------------------------
void CPkDialog::ShowEndDialog(bool bShow)
{
	frmTeamPkEnd->SetIsShow(bShow);
}

//-----------------------------------------------------------------------------
void CPkDialog::_MainMousePkStartEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
{
	string name = pSender->GetName();
	if ( stricmp ("frmTeamPk", pSender->GetForm()->GetName() ) == 0 )
	{
		CS_TeamFightAnswer( name=="btnYes" );

		g_stUIPKDialog.frmTeamPkStart->Close();
	}

}

//-----------------------------------------------------------------------------
void CPkDialog::_MainMousePkEndEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
{
	string name = pSender->GetName();
	if ( stricmp ("frmTeamPk", pSender->GetForm()->GetName() ) == 0 )
	{
		g_stUIPKDialog.frmTeamPkEnd->Close();
	}
}

