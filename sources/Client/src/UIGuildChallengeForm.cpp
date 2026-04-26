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


namespace GUI {
	const int CGuildChallengeMgr::FIRST_CHARGE_MONEY[NUM] = {5000000, 3000000, 1000000};

	//-------------------------------------------------------------------------
	bool CGuildChallengeMgr::Init() {
		CFormMgr& mgr = CFormMgr::s_Mgr;
		//npc
		frmGuildPK = mgr.Find("frmGuildPK");
		if (!frmGuildPK) {
			g_logManager.InternalLog(LogLevel::Debug, "common", GetLanguageString(560).c_str());
			return false;
		}

		//frmNPCforge->evtEntrustMouseEvent = _MainMouseEvent;
		//frmNPCforge->evtClose = _OnClose;
		//lstGuildPK = dynamic_cast<CList*>(frmGuildPK->Find("lstGuildPK"));
		//if (!lstGuildPK)
		//	return Error("NPC.clu<%s><%s>",
		//				 frmGuildPK->GetName(), 
		//				 "lstGuildPK");
		for (int i(0); i < NUM; i++) {
			std::string szBuf = std::format("labGuildName{}", i);
			labGuildName[i] = dynamic_cast<CLabel*>(frmGuildPK->Find(szBuf.c_str()));
			if (!labGuildName[i])
				return Error(GetLanguageString(561).c_str(),
							 frmGuildPK->GetName(),
							 szBuf.c_str());

			szBuf = std::format("labChallenger{}", i);
			labChallenger[i] = dynamic_cast<CLabel*>(frmGuildPK->Find(szBuf.c_str()));
			if (!labChallenger[i])
				return Error(GetLanguageString(561).c_str(),
							 frmGuildPK->GetName(),
							 szBuf.c_str());

			szBuf = std::format("labMoney{}", i);
			labMoney[i] = dynamic_cast<CLabel*>(frmGuildPK->Find(szBuf.c_str()));
			if (!labMoney[i])
				return Error(GetLanguageString(561).c_str(),
							 frmGuildPK->GetName(),
							 szBuf.c_str());

			szBuf = std::format("btnCharge{}", i);
			btnCharge[i] = dynamic_cast<CTextButton*>(frmGuildPK->Find(szBuf.c_str()));
			if (!btnCharge[i])
				return Error(GetLanguageString(561).c_str(),
							 frmGuildPK->GetName(),
							 szBuf.c_str());
		}
		btnCharge[FIRST]->evtMouseClick = _FirstChallengeButtonDown;
		btnCharge[SECOND]->evtMouseClick = _SecondChallengeButtonDown;
		btnCharge[THIRD]->evtMouseClick = _ThirdChallengeButtonDown;

		return true;
	}

	//-------------------------------------------------------------------------
	bool CGuildChallengeMgr::SendChallegeProtocol(int iIndex, long lMoney) {
		if (m_lChargeMoney[iIndex] == -1) {
			CM_GUILD_LEIZHU(iIndex + 1, (DWORD)lMoney);
		}
		else {
			CM_GUILD_CHALL(iIndex + 1, (DWORD)lMoney);
		}
		return true;
	}

	//-------------------------------------------------------------------------
	void CGuildChallengeMgr::Show(bool bShow) {
		frmGuildPK->SetIsShow(bShow);
	}

	//-------------------------------------------------------------------------
	void CGuildChallengeMgr::SetContent(const NET_GUILD_CHALLINFO& Info) {
		for (int i(0); i < NUM; ++i) {
			btnCharge[i]->SetIsEnabled(Info.byIsLeader == 1);

			if (Info.byLevel[i] == 0) {
				labGuildName[i]->SetCaption("");
				labChallenger[i]->SetCaption("");
				labMoney[i]->SetCaption("");
				m_lChargeMoney[i] = -1;
				m_bStart[i] = false;
			}
			else {
				labGuildName[i]->SetCaption(Info.szGuild[i]);
				labChallenger[i]->SetCaption(Info.szChall[i]);
				labMoney[i]->SetCaption(StringSplitNum(Info.dwMoney[i]));
				m_lChargeMoney[i] = Info.dwMoney[i];
				m_bStart[i] = Info.byStart[i] == 1 ? true : false;
			}
		}

		return;
	}

	//-------------------------------------------------------------------------
	void CGuildChallengeMgr::ChallegeSuccess(long lChaID) {
		return;
	}

	//-------------------------------------------------------------------------
	void CGuildChallengeMgr::ChallegeFailed(long lChaID) {
		return;
	}

	//-------------------------------------------------------------------------
	void CGuildChallengeMgr::ChallegeOther(long lChaID) {
		return;
	}

	//-------------------------------------------------------------------------
	void CGuildChallengeMgr::_FirstChallengeButtonDown(CGuiData* pSender, int x, int y, DWORD key) {
		g_stGuildChallenge.ChargeMoney(CGuildChallengeMgr::FIRST);
	}

	//-------------------------------------------------------------------------
	void CGuildChallengeMgr::_SecondChallengeButtonDown(CGuiData* pSender, int x, int y, DWORD key) {
		g_stGuildChallenge.ChargeMoney(CGuildChallengeMgr::SECOND);
	}

	//-------------------------------------------------------------------------
	void CGuildChallengeMgr::_ThirdChallengeButtonDown(CGuiData* pSender, int x, int y, DWORD key) {
		g_stGuildChallenge.ChargeMoney(CGuildChallengeMgr::THIRD);
	}

	//-------------------------------------------------------------------------
	void CGuildChallengeMgr::_enterChargeMoney(CCompent* pSender, int nMsgType, int x, int y, DWORD dwKey) {
		if (nMsgType != CForm::mrYes) {
			g_stGuildChallenge.m_iSelIndex = -1;
			return;
		}

		if (g_stGuildChallenge.m_iSelIndex < 0 &&
			g_stGuildChallenge.m_iSelIndex >= CGuildChallengeMgr::NUM)
			return;

		stNumBox* kItemPriceBox = (stNumBox*)pSender->GetForm()->GetPointer();
		if (!kItemPriceBox) return;

		int iChargeMoney = kItemPriceBox->GetNumber();
		if (!g_stGuildChallenge.IsValid(g_stGuildChallenge.m_iSelIndex, iChargeMoney)) {
			return;
		}
		g_stGuildChallenge.m_iChangeMoney = iChargeMoney;

		char buf[256] = {0};
		FmtLang(buf, sizeof(buf), GetLanguageString(583), StringSplitNum(iChargeMoney));
		g_stUIBox.ShowSelectBox(_ChargeEvent, buf, true);
	}

	//-------------------------------------------------------------------------
	void CGuildChallengeMgr::_ChargeEvent(CCompent* pSender, int nMsgType, int x, int y, DWORD dwKey) {
		if (nMsgType != CForm::mrYes) //  BUG  add by Philip.Wu  2006-07-25
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
	void CGuildChallengeMgr::ChargeMoney(int iIndex) {
		CCharacter* pMainCha = CGameScene::GetMainCha();
		if (!pMainCha)
			return;

		if (m_bStart[iIndex]) {
			g_pGameApp->MsgBox(GetLanguageString(584));
			return;
		}

		if (m_lChargeMoney[iIndex] == -1) {
			if (GetChallengeMasterIndex(pMainCha->getGuildName().c_str()) != -1) {
				g_pGameApp->MsgBox(GetLanguageString(585));
				return;
			}
		}
		else {
			int iMasterIndex = GetChallengeMasterIndex(pMainCha->getGuildName().c_str());
			if (iMasterIndex != -1) {
				if (iIndex == iMasterIndex) {
					g_pGameApp->MsgBox(GetLanguageString(586));
					return;
				}
				if (iIndex > iMasterIndex) {
					g_pGameApp->MsgBox(GetLanguageString(587));
					return;
				}
			}
		}

		if (std::string_view{labChallenger[iIndex]->GetCaption()} == pMainCha->getGuildName()) {
			g_pGameApp->MsgBox(GetLanguageString(588));
			return;
		}
		m_iSelIndex = iIndex;
		long iNextCharge(0);
		if (m_lChargeMoney[iIndex] == -1) {
			iNextCharge = FIRST_CHARGE_MONEY[iIndex];
		}
		else {
			iNextCharge = m_lChargeMoney[iIndex] + CHARGE_MONEY;
		}

		stNumBox* numBox = CBoxMgr::ShowNumberBox(_enterChargeMoney, -1, GetLanguageString(589).c_str(), false);
		numBox->edtNumber->SetCaption(std::format("{}", iNextCharge).c_str());
	}

	//-------------------------------------------------------------------------
	void CGuildChallengeMgr::ClearUI() {
		for (int i(0); i < NUM; i++) {
			labGuildName[i]->SetCaption("");
			labChallenger[i]->SetCaption("");
			labMoney[i]->SetCaption("");
			btnCharge[i]->SetCaption("");
		}
		return;
	}

	//-------------------------------------------------------------------------
	bool CGuildChallengeMgr::IsValid(int iIndex, long lMoney) {
		if (m_lChargeMoney[iIndex] == -1) {
			if (lMoney < FIRST_CHARGE_MONEY[iIndex]) {
				g_pGameApp->MsgBox(GetLanguageString(590));
				return false;
			}
		}
		else {
			long lMinMoney = m_lChargeMoney[iIndex] + CGuildChallengeMgr::CHARGE_MONEY;
			if (lMoney < lMinMoney) {
				g_pGameApp->MsgBox(GetLanguageString(591));
				return false;
			}
		}

		if (lMoney >= 2000000000) {
			g_pGameApp->MsgBox(GetLanguageString(592));
			return false;
		}
		return true;
	}

	//-------------------------------------------------------------------------
	int CGuildChallengeMgr::GetChallengeMasterIndex(const char* szName) {
		for (int i(0); i < NUM; i++) {
			if (std::string_view{labGuildName[i]->GetCaption()} == szName) {
				return i;
			}
		}
		return -1;
	}
}
