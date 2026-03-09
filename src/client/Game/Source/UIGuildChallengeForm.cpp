#include "StdAfx.h"

#include "uiGuildChallengeForm.h"
#include "uiformmgr.h"
#include "uiform.h"
#include "uilabel.h"
#include "uimemo.h"
#include "uitextbutton.h"
#include "scene.h"
#include "uiitemcommand.h"
#include "uifastcommand.h"
#include "forgerecord.h"
#include "gameapp.h"
#include "uigoodsgrid.h"
#include "uiequipform.h"
#include "packetcmd.h"
#include "character.h"
#include "UIBoxform.h"
#include "packetCmd.h"
#include "StoneSet.h"
#include "GameApp.h"
#include "UIProgressBar.h"
#include "WorldScene.h"
#include "UIList.h"
#include "StringLib.h"
#include "NetGuild.h"
#include "StringLib.h"
#include "UIEdit.h"



namespace GUI
{
	const int CGuildChallengeMgr::FIRST_CHARGE_MONEY[NUM] = {5000000, 3000000, 1000000};

	//-------------------------------------------------------------------------
	bool CGuildChallengeMgr::Init()
	{
		CFormMgr &mgr = CFormMgr::s_Mgr;
		//npc
		frmGuildPK  = mgr.Find("frmGuildPK" );
		if ( !frmGuildPK )
		{	
			ToLogService("gui", "{}", g_oLangRec.GetString(560));
			return false;
		}

		//frmNPCforge->evtEntrustMouseEvent = _MainMouseEvent;
		//frmNPCforge->evtClose = _OnClose;
		//lstGuildPK = dynamic_cast<CList*>(frmGuildPK->Find("lstGuildPK"));
		//if (!lstGuildPK)
		//	return Error("NPC.clu<%s><%s>",
		//				 frmGuildPK->GetName(), 
		//				 "lstGuildPK");
		char szBuf[32];
		for (int i(0); i<NUM; i++)
		{
			sprintf(szBuf, "labGuildName%d", i);
			labGuildName[i] = dynamic_cast<CLabel*>(frmGuildPK->Find(szBuf));
			if (!labGuildName[i]) 
				return Error(g_oLangRec.GetString(561),
				frmGuildPK->GetName(), 
				szBuf);

			sprintf(szBuf, "labChallenger%d", i);
			labChallenger[i] = dynamic_cast<CLabel*>(frmGuildPK->Find(szBuf));
			if (!labChallenger[i]) 
				return Error(g_oLangRec.GetString(561),
				frmGuildPK->GetName(), 
				szBuf);

			sprintf(szBuf, "labMoney%d", i);
			labMoney[i] = dynamic_cast<CLabel*>(frmGuildPK->Find(szBuf));
			if (!labMoney[i]) 
				return Error(g_oLangRec.GetString(561),
				frmGuildPK->GetName(), 
				szBuf);

			sprintf(szBuf, "btnCharge%d", i);
			btnCharge[i] = dynamic_cast<CTextButton*>(frmGuildPK->Find(szBuf));
			if (!btnCharge[i]) 
				return Error(g_oLangRec.GetString(561),
				frmGuildPK->GetName(), 
				szBuf);

		}
		btnCharge[FIRST]->evtMouseClick = _FirstChallengeButtonDown;
		btnCharge[SECOND]->evtMouseClick = _SecondChallengeButtonDown;
		btnCharge[THIRD]->evtMouseClick = _ThirdChallengeButtonDown;

		return true;
	}
	//-------------------------------------------------------------------------
	bool CGuildChallengeMgr::SendChallegeProtocol(int iIndex, long lMoney)
	{		
		if (m_lChargeMoney[iIndex] == -1)
		{
			CM_GUILD_LEIZHU(iIndex+1, (DWORD)lMoney);
		}
		else
		{
			CM_GUILD_CHALL(iIndex+1, (DWORD)lMoney);
		}
		return true;
	}
	//-------------------------------------------------------------------------
	void CGuildChallengeMgr::Show(bool bShow)
	{
		frmGuildPK->SetIsShow(bShow);
	}
	//-------------------------------------------------------------------------
	void CGuildChallengeMgr::SetContent(const NET_GUILD_CHALLINFO& Info)
	{
		for (int i(0); i<NUM; ++i)
		{
			btnCharge[i]->SetIsEnabled(Info.byIsLeader == 1);

			if (Info.byLevel[i] == 0)
			{
				labGuildName[i]->SetCaption("");
				labChallenger[i]->SetCaption("");
				labMoney[i]->SetCaption("");
				m_lChargeMoney[i] = -1;
				m_bStart[i] = false ;

			}
			else
			{
				labGuildName[i]->SetCaption(Info.szGuild[i]);
				labChallenger[i]->SetCaption(Info.szChall[i]);
				labMoney[i]->SetCaption(StringSplitNum(Info.dwMoney[i]));
				m_lChargeMoney[i] = Info.dwMoney[i];
				m_bStart[i] = Info.byStart[i] == 1 ? true : false ;
			}
		}

		return;
	}
	//-------------------------------------------------------------------------
	void CGuildChallengeMgr::ChallegeSuccess(long lChaID)
	{
		return;
	}
	//-------------------------------------------------------------------------
	void CGuildChallengeMgr::ChallegeFailed(long lChaID)
	{
		return;
	}
	//-------------------------------------------------------------------------
	void CGuildChallengeMgr::ChallegeOther(long lChaID)
	{
		return;
	}
	//-------------------------------------------------------------------------
	void CGuildChallengeMgr::_FirstChallengeButtonDown(CGuiData *pSender, int x, int y, DWORD key)
	{
		g_stGuildChallenge.ChargeMoney(CGuildChallengeMgr::FIRST);
	}
	//-------------------------------------------------------------------------
	void CGuildChallengeMgr::_SecondChallengeButtonDown(CGuiData *pSender, int x, int y, DWORD key)
	{
		g_stGuildChallenge.ChargeMoney(CGuildChallengeMgr::SECOND);
	}
	//-------------------------------------------------------------------------
	void CGuildChallengeMgr::_ThirdChallengeButtonDown(CGuiData *pSender, int x, int y, DWORD key)
	{
		g_stGuildChallenge.ChargeMoney(CGuildChallengeMgr::THIRD);
	}
	//-------------------------------------------------------------------------
	void CGuildChallengeMgr::_enterChargeMoney(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
	{
		if( nMsgType!=CForm::mrYes ) 
		{
			g_stGuildChallenge.m_iSelIndex = -1;
			return;
		}

		if (g_stGuildChallenge.m_iSelIndex < 0 && 
			g_stGuildChallenge.m_iSelIndex >= CGuildChallengeMgr::NUM )
			return;

		stNumBox* kItemPriceBox = (stNumBox*)pSender->GetForm()->GetPointer();
		if (!kItemPriceBox) return;

		int iChargeMoney = kItemPriceBox->GetNumber();
		if (!g_stGuildChallenge.IsValid(g_stGuildChallenge.m_iSelIndex, iChargeMoney))
		{
			return;
		}
		g_stGuildChallenge.m_iChangeMoney = iChargeMoney;

		char buf[256] = { 0 };
		sprintf(buf, g_oLangRec.GetString(583), StringSplitNum( iChargeMoney ));
		g_stUIBox.ShowSelectBox(_ChargeEvent, buf, true);
	}
	//-------------------------------------------------------------------------
	void CGuildChallengeMgr::_ChargeEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
	{
		if( nMsgType!=CForm::mrYes )	//  BUG  add by Philip.Wu  2006-07-25
		{
			g_stGuildChallenge.m_iSelIndex = -1;
			return;
		}

		if (g_stGuildChallenge.m_iSelIndex == -1 && g_stGuildChallenge.m_iChangeMoney == -1)
			return;
		g_stGuildChallenge.SendChallegeProtocol(g_stGuildChallenge.m_iSelIndex, g_stGuildChallenge.m_iChangeMoney);

		g_stGuildChallenge.m_iSelIndex = -1;
		g_stGuildChallenge.m_iChangeMoney = -1;

	}
	//-------------------------------------------------------------------------
	void CGuildChallengeMgr::ChargeMoney(int iIndex)
	{
		CCharacter* pMainCha = CGameScene::GetMainCha();
		if (!pMainCha)
			return;

		if (m_bStart[iIndex])
		{
			g_pGameApp->MsgBox(g_oLangRec.GetString(584));
			return;
		}

		if (m_lChargeMoney[iIndex] == -1)
		{
			if (GetChallengeMasterIndex(pMainCha->getGuildName()) != -1)
			{
				g_pGameApp->MsgBox(g_oLangRec.GetString(585));
				return;
			}
		}
		else
		{
			int iMasterIndex = GetChallengeMasterIndex(pMainCha->getGuildName());
			if ( iMasterIndex != -1 )
			{
				if (iIndex == iMasterIndex)
				{
					g_pGameApp->MsgBox(g_oLangRec.GetString(586));
					return;
				}
				if (iIndex > iMasterIndex)
				{
					g_pGameApp->MsgBox(g_oLangRec.GetString(587));
					return;
				}
			}

		}

		if (strcmp(labChallenger[iIndex]->GetCaption(), pMainCha->getGuildName()) == 0 )
		{
			g_pGameApp->MsgBox(g_oLangRec.GetString(588));
			return;
		}
		m_iSelIndex = iIndex;
		long iNextCharge(0);
		if (m_lChargeMoney[iIndex] == -1)
		{
			iNextCharge = FIRST_CHARGE_MONEY[iIndex];
		}
		else
		{
			iNextCharge = m_lChargeMoney[iIndex] + CHARGE_MONEY;
		}

		char szBuf[64];
		stNumBox* numBox = CBoxMgr::ShowNumberBox(_enterChargeMoney, -1, g_oLangRec.GetString(589), false);
		sprintf(szBuf, "%d", iNextCharge);
		numBox->edtNumber->SetCaption(szBuf);
	}
	//-------------------------------------------------------------------------
	void CGuildChallengeMgr::ClearUI()
	{
		for (int i(0); i<NUM; i++)
		{
			labGuildName[i]->SetCaption("");
			labChallenger[i]->SetCaption("");
			labMoney[i]->SetCaption("");
			btnCharge[i]->SetCaption("");
		}
		return;
	}
	//-------------------------------------------------------------------------
	bool CGuildChallengeMgr::IsValid(int iIndex, long lMoney)
	{
		if (m_lChargeMoney[iIndex] == -1)
		{
			if (lMoney < FIRST_CHARGE_MONEY[iIndex])
			{
				g_pGameApp->MsgBox( g_oLangRec.GetString(590) );
				return false;
			}
		}
		else
		{
			long lMinMoney = m_lChargeMoney[iIndex] + CGuildChallengeMgr::CHARGE_MONEY;
			if( lMoney < lMinMoney ) 
			{
				g_pGameApp->MsgBox( g_oLangRec.GetString(591) );
				return false;
			}
		}

		if (lMoney >= 2000000000)
		{
			g_pGameApp->MsgBox( g_oLangRec.GetString(592) );
			return false;
		}
		return true;
	}
	//-------------------------------------------------------------------------
	int CGuildChallengeMgr::GetChallengeMasterIndex(const char * szName)
	{
		for (int i(0); i<NUM; i++)
		{
			if (strcmp(labGuildName[i]->GetCaption(), szName) == 0 )
			{
				return i;
			}
		}
		return -1;
	}


}

