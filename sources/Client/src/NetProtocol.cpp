#include "stdafx.h"
#include "NetProtocol.h"
#include "gameapp.h"
#include "Actor.h"
#include "STMove.h"
#include "STAttack.h"
#include "HMAttack.h"
#include "CharacterRecord.h"
#include "Character.h"
#include "SceneItem.h"
#include "SkillRecord.h"
#include "stseat.h"
#include "mapset.h"
#include "actor.h"
#include "WorldScene.h"
#include "LoginScene.h"

#include "PacketCmd.h"
#include "EffectObj.h"
#include "UICommand.h"
#include "uigoodsgrid.h"
#include "ItemRecord.h"
#include "uiitemcommand.h"
#include "stpose.h"
#include "UITemplete.h"
#include "ItemRecord.h"
#include <strstream>

#include "rolecommon.h"
#include "HMManage.h"
#include "effdelay.h"
#include "UISkillList.h"
#include "UITeam.h"
#include "UIChat.h"
#include "SkillStateRecord.h"
#include "notifyset.h"
#include "streadydie.h"
#include "uiheadsay.h"
#include "EventRecord.h"
#include "CommFunc.h"
#include "uiequipform.h"
#include "UIMisLogForm.h"
#include "uicozeform.h"
#include "uinpctradeform.h"
#include "uimissionform.h"
#include "uinpctalkform.h"
#include "uitradeform.h"
#include "uiforgeform.h"
#include "TalkSessionFormMgr.H"
#include "uistateform.h"
#include "uicozeform.h"
#include "shipfactory.h"
#include "SelectChaScene.h"
#include "CreateChaScene.h"
#include "uiboatform.h"
#include "UIBourseForm.h"
#include "event.h"
#include "uistartform.h"
#include "uiminimapform.h"
#include "gameappmsg.h"
#include "SMallMap.h"
#include "Scene.h"
#include "uibankform.h"
#include "UIGuildBankForm.h"
#include "gameconfig.h"
#include "UIHaircutForm.h"
#include "UIBoothForm.h"
#include "UIPKDialog.h"
#include "stnpctalk.h"
#include "UIMakeEquipForm.h"
#include "uifastcommand.h"
#include "uisystemform.h"
#include "uiDoublePwdForm.h"
#include "SelectChaScene.h"
#include "uistoreform.h"
#include "uiSpiritForm.h"
#include "uiPurifyForm.h"
#include "uiStateForm.h"
#include "UIChurchChallenge.h"

using namespace std;

static CActionState* g_state = NULL;
//CLargerMap* CGameScene::_pLargerMap			= NULL;

inline static CCharacter* GetCharacter(unsigned int nID, const char* error = NULL) {
	if (!CGameApp::GetCurScene()) return NULL;

	CCharacter* pCha = CGameScene::GetMainCha();

	if (pCha && pCha->getAttachID() == nID) {
		return pCha;
	}
	else {
		pCha = CGameApp::GetCurScene()->SearchByID(nID);
	}
	if (!pCha && error) {
		g_logManager.InternalLog(LogLevel::Debug, "network", SafeVFormat(GetLanguageString(247), nID, error));
	}
	return pCha;
}

//----------------------------------------------------------------------------
// Network functions
//----------------------------------------------------------------------------


void NetLoginSuccess(char byPassword, uint8_t maxCharacters, std::span<const NetChaBehave> characters) {
#ifdef _TEST_CLIENT
	static int i = 0;
	i++;
	//  dev-only  test_client
	return;
#endif

	// Record whether secondary password exists
	g_Config.m_IsDoublePwd = byPassword ? true : false;

	ToLogService("ui", "NetLoginSuccess - CharNum:{}", characters.size());

	//for (const auto& cha : characters)
	//{
	//	LG("select", GetLanguageString(248), cha.sCharName, cha.sJob, cha.iDegree,
	//		cha.sLook->sTypeID, cha.sLook->SLink[0].sID, cha.sLook->SLink[1].sID,
	//		cha.sLook->SLink[2].sID, cha.sLook->SLink[3].sID, cha.sLook->SLink[4].sID);
	//}

	CLoginScene* pScene = dynamic_cast<CLoginScene*>(CGameApp::GetCurScene());
	if (!pScene) {
		g_logManager.InternalLog(LogLevel::Debug, "network", GetLanguageString(249));
		return;
	}
	pScene->SetPasswordError(false);

	g_pGameApp->LoadScriptScene(enumSelectChaScene);
	CSelectChaScene::GetCurrScene().m_MaxCharacters = maxCharacters;
	CSelectChaScene::GetCurrScene().SelectCharacters(characters);
	CGameApp::Waiting(false);
}

void NetLoginFailure(unsigned short Errno) {
#ifdef _TEST_CLIENT
	static int i = 0;
	i++;
	//  dev-only  test_client
	return;
#endif

	ToLogService("ui", "NetLoginFailure - Errno:{}, Info:{}", Errno, g_GetServerError(Errno));

	CLoginScene* pScene = dynamic_cast<CLoginScene*>(CGameApp::GetCurScene());
	if (!pScene) {
		g_logManager.InternalLog(LogLevel::Debug, "network", GetLanguageString(250));
		return;
	}

	// Check if password is wrong
	switch (Errno) {
	case ERR_AP_INVALIDPWD: {
		pScene->SetPasswordError(true);
		pScene->Error(Errno, "NetLoginFailure");

		// Track consecutive wrong password attempts
		typedef vector<DWORD> times;
		static times error_time;
		error_time.push_back(CGameApp::GetCurTick());
		DWORD dwCount = (DWORD)error_time.size() - 1;
		if (dwCount >= 3) {
			DWORD dwLast = error_time[dwCount];
			DWORD dwFirst = error_time[dwCount - 3];
			if (dwLast - dwFirst <= 60 * 1000) {
				g_pGameApp->MsgBox("%s", GetLanguageString(251).c_str());
				g_pGameApp->SetIsRun(false);
			}
			return;
		}
		return;
	}
	case ERR_AP_BANUSER: {
		g_pGameApp->MsgBox(GetLanguageString(252).c_str());
		return;
	}
	case ERR_AP_INVALIDUSER: {
		g_pGameApp->MsgBox(GetLanguageString(253).c_str());
		return;
	}

	default:
		break;
	}

	pScene->Error(Errno, "NetLoginFailure");
}

void NetBeginPlay(unsigned short Errno) // Look up errno in NetRetCode.h
{
#ifdef _TEST_CLIENT
	static int i = 0;
	i++;
	//  dev-only  test_client
	return;
#endif

	ToLogService("ui", "NetBeginPlay - Errno:{}, Info:{}", Errno, g_GetServerError(Errno));

	CSelectChaScene& rkScene = CSelectChaScene::GetCurrScene();
	rkScene.SelectChaError(Errno, "NetBeginPlay");
}

void NetEndPlay(uint8_t maxCharacters, std::span<const NetChaBehave> characters) {
#ifdef _TEST_CLIENT
	static int i = 0;
	i++;
	//  dev-only  test_client
	return;
#endif

	ToLogService("ui", "NetEndPlay - CharNum:{}", characters.size());

	/* Not needed for production?
	*/

	//for (const auto& cha : characters)
	//{
	//	LG("select", GetLanguageString(248), cha.sCharName, cha.sJob, cha.iDegree, cha.sLook->sTypeID,
	//		cha.sLook->SLink[0].sID, cha.sLook->SLink[1].sID, cha.sLook->SLink[2].sID, cha.sLook->SLink[3].sID, cha.sLook->SLink[4].sID);
	//}


	// Return to character selection list
	g_pGameApp->LoadScriptScene(enumLoginScene);
	g_pGameApp->SetLoginTime(0);

	CLoginScene* pScene = dynamic_cast<CLoginScene*>(g_pGameApp->GetCurScene());
	if (pScene) {
		if (g_NetIF->IsConnected())
			pScene->ShowChaList();
		else
			pScene->ShowRegionList();
		//pScene->ShowLoginForm();
	}

	// Reset equipment lock state to default (unlocked)
	g_stUIEquip.SetIsLock(false);

	g_pGameApp->LoadScriptScene(enumSelectChaScene);
	CSelectChaScene::GetCurrScene().m_MaxCharacters = maxCharacters;
	CSelectChaScene::GetCurrScene().SelectCharacters(characters);
}

void NetNewCha(unsigned short Errno) // Look up errno in NetRetCode.h
{
#ifdef _TEST_CLIENT
	static int i = 0;
	i++;
	//  dev-only  test_client
	return;
#endif

	ToLogService("ui", "NetNewCha - Errno:{}, Info:{}", Errno, g_GetServerError(Errno));

	CCreateChaScene& rkScene = CCreateChaScene::GetCurrScene();

	if (Errno == 0) {
		// Success create new character.
		rkScene.CreateNewCha();
	}
	else {
		rkScene.NewChaError(Errno, "NetNewCha");
	}
}

void NetDelCha(unsigned short Errno) // Look up errno in NetRetCode.h
{
#ifdef _TEST_CLIENT
	static int i = 0;
	i++;
	//  dev-only  test_client
	return;
#endif

	ToLogService("ui", "NetDelCha - Errno:{}, Info:{}", Errno, g_GetServerError(Errno));

	CSelectChaScene& rkScene = CSelectChaScene::GetCurrScene();

	if (Errno == ERR_SUCCESS) {
		g_stUIDoublePwd.CloseAllForm();
		rkScene.DelCurrentSelCha();
		CGameApp::Waiting(false);
		return;
	}

	switch (Errno) {
	case ERR_PT_INVALID_PW2: {
		CGameApp::Waiting(false);
		g_pGameApp->MsgBox("%s", GetLanguageString(802).c_str());
		return;
	}
	case ERR_PT_MULTICHA:
		CGameApp::Waiting(false);
		g_pGameApp->MsgBox("You cannot delete a character still playing");
		return;
	default: {
		rkScene.SelectChaError(Errno, "NetDelCha");
		CGameApp::Waiting(false);
		return;
	}
	}
}

void NetCreatePassword2(unsigned short Errno) {
	// Restore cursor
	CCursor::I()->SetCursor(CCursor::stNormal);

	if (Errno == ERR_SUCCESS) {
		// Secondary password created successfully
		g_Config.m_IsDoublePwd = true;

		CSelectChaScene* pSelChaScene = dynamic_cast<CSelectChaScene*>(g_pGameApp->GetCurScene());
		if (pSelChaScene) {
			pSelChaScene->UpdateButton();

			if (0 == pSelChaScene->GetChaCount()) {
				// Show welcome notice
				pSelChaScene->ShowWelcomeNotice();
			}
		}

		g_stUIDoublePwd.CloseAllForm();
	}
	else if (Errno == ERR_PT_SERVERBUSY) {
		// System busy
		g_pGameApp->MsgBox("%s", SafeVFormat(GetLanguageString(172), "").c_str());
	}
	else if (Errno == ERR_PT_INVALID_PW2) {
		// Secondary password already exists
		g_pGameApp->MsgBox("%s", GetLanguageString(801).c_str());
	}
	else {
		// Unknown error
		g_pGameApp->MsgBox("%s", GetLanguageString(375).c_str());
	}
}

void NetUpdatePassword2(unsigned short Errno) {
	// Restore cursor
	CCursor::I()->SetCursor(CCursor::stNormal);

	if (Errno == ERR_SUCCESS) {
		// Secondary password updated successfully
		g_Config.m_IsDoublePwd = true;
		g_stUIDoublePwd.CloseAllForm();
	}
	else if (Errno == ERR_PT_SERVERBUSY) {
		// System busy
		g_pGameApp->MsgBox("%s", SafeVFormat(GetLanguageString(172), "").c_str());
	}
	else if (Errno == ERR_PT_INVALID_PW2) {
		// Secondary password already exists
		g_pGameApp->MsgBox("%s", GetLanguageString(801).c_str());
	}
	else {
		// Unknown error
		g_pGameApp->MsgBox("%s", GetLanguageString(375).c_str());
	}
}

void NetActorMove(unsigned int id, stNetNotiMove& list) {
	if (!CGameApp::GetCurScene()) return;

	CCharacter* cha = CGameScene::GetMainCha();

	static CActionState* g_state = NULL;
	g_state = NULL;
	if (false && cha && cha->getAttachID() == id) {
		cha->SetServerPos(list.SPos[list.nPointNum - 1].x, list.SPos[list.nPointNum - 1].y);
		g_state = cha->GetActor()->GetCurState();
		if (!g_state) {
			g_logManager.InternalLog(LogLevel::Debug, "network", GetLanguageString(258));
			return;
		}
	}
	else {
		cha = CGameApp::GetCurScene()->SearchByID(id);
		if (!cha) {
			g_logManager.InternalLog(LogLevel::Debug, "network", SafeVFormat(GetLanguageString(259), id, list.nPointNum, list.SPos[0].x, list.SPos[0].y));
			return;
		}

		cha->SetServerPos(list.SPos[list.nPointNum - 1].x, list.SPos[list.nPointNum - 1].y);
		g_state = cha->GetActor()->GetServerState();
		if (!g_state) {
			g_state = new CWaitMoveState(cha->GetActor());
			if (!cha->GetActor()->SwitchState(g_state)) {
				g_logManager.InternalLog(LogLevel::Debug, "network", SafeVFormat(GetLanguageString(260), static_cast<int>(cha->GetActor()->GetState(), cha->getLogName(),
				   list.nPointNum, static_cast<int>(list.sState), list.SPos[1].x, list.SPos[1].y, GetTickCount())));
				return;
			}
		}
	}

	if (!g_state) {
		g_logManager.InternalLog(LogLevel::Debug, "network", SafeVFormat(GetLanguageString(261), cha->getLogName(), list.nPointNum, list.sState, list.SPos[1].x,
		   list.SPos[1].y, GetTickCount()));
		return;
	}

	for (int i = 1; i < list.nPointNum; i++) {
		g_state->PushPoint(list.SPos[i].x, list.SPos[i].y);
	}

	// If movement state is "stop"
	if (list.sState) {
		if (list.nPointNum > 1) {
			g_state->MoveEnd(list.SPos[list.nPointNum - 1].x, list.SPos[list.nPointNum - 1].y, list.sState);
		}
		else {
			g_logManager.InternalLog(LogLevel::Debug, "network", SafeVFormat(GetLanguageString(262), cha->getLogName(), list.nPointNum, list.sState, list.SPos[1].x,
			   list.SPos[1].y, GetTickCount()));
		}
	}
}

void stNetActorCreate::SetValue(CCharacter* pCha) {
	pCha->setAttachID(ulWorldID);
	pCha->lTag = lHandle;
	pCha->setName(szName.c_str());
	pCha->setPos(SArea.centre.x, SArea.centre.y);
	pCha->SetServerPos(SArea.centre.x, SArea.centre.y);
	pCha->setYaw(sAngle);
	pCha->setChaCtrlType(chCtrlType);
	pCha->SetTeamLeaderID(ulTLeaderID);
	pCha->setHumanID(ulCommID);
	pCha->setHumanName(szCommName.c_str());
	pCha->setGMLv(chGMLv);
	if (chGMLv) pCha->setNpcState(0);
	pCha->setSecondName(strMottoName.c_str());
	pCha->setPhotoID(sIcon);
	pCha->setShopName(strStallName.c_str());

	pCha->setGuildID(lGuildID);
	pCha->setGuildName(strGuildName.c_str());
	pCha->setGuildMotto(strGuildMotto.c_str());
	pCha->setGuildPermission(chGuildPermission);
	pCha->setSideID(SSideInfo.chSideID);
	pCha->setIsPlayerCha(chIsPlayer == 1);

#ifdef _LOG_NAME_
	char pszLogName[256];
	sprintf(pszLogName, "%s+%u", szName.c_str(), ulWorldID);
	g_logManager.InternalLog(LogLevel::Debug, "common", std::format("SeeType:{}, Create Cha:{}, Type:{}", static_cast<int>(chSeeType), pCha->getName(),
	   pCha->GetDefaultChaInfo()->szName));
	g_logManager.InternalLog(LogLevel::Debug, "common", std::format("CommID:{}, CommName:{}, MottoName:{}", ulCommID, szCommName, strMottoName));
	g_logManager.InternalLog(LogLevel::Debug, "common", std::format("SideID:[{}], sAngle:{}, GM:{}, Icon:{}", static_cast<int>(SSideInfo.chSideID), sAngle, static_cast<int>(chGMLv), sIcon));
	g_logManager.InternalLog(LogLevel::Debug, "common", std::format("GuildID:{}, Name:{}, Motto:{}", lGuildID, strGuildName, strGuildMotto));
#endif
}

CCharacter* stNetActorCreate::CreateCha() {
	ToLogService("common", "Create WorldID:{}, ChaID = {}, Pos = [{},{}]", ulWorldID, ulChaID, SArea.centre.x, SArea.centre.y);

	if (chSeeType == enumENTITY_SEEN_SWITCH) {
		CCharacter* pCha = g_stUIBoat.FindCha(ulWorldID);
		if (pCha) {
			SAppendLook.Exec(pCha);

			SetValue(pCha);
			pCha->SetHide(FALSE);
			SPKCtrl.Exec(pCha);
		}
		else {
			g_logManager.InternalLog(LogLevel::Debug, "common", GetLanguageString(263));
		}
		return pCha;
	}

	CCharacter* p = GetCharacter(ulWorldID);
	if (p) {
		g_logManager.InternalLog(LogLevel::Debug, "common", SafeVFormat(GetLanguageString(264), szName.c_str(), ulWorldID, p->getLogName()));
		p->SetValid(FALSE);
	}

	// Create character and apply look
	CChaRecord* pChaRec = GetChaRecordInfo(ulChaID);
	if (!pChaRec) return NULL;

	if (pChaRec->chModalType == enumMODAL_BOAT) {
		p = CGameApp::GetCurScene()->AddBoat(SLookInfo.SLook);
	}
	else {
		p = CGameApp::GetCurScene()->AddCharacter(ulChaID);
	}

	if (!p) {
		ToLogService("network", LogLevel::Error,
		   "msgNetCreateActor AddCharacter Failed:chType[{}], chCtrlType[{}], MainType[{}], id[{}], type[{}], x[{}], y[{}], name[{}]",
		   static_cast<int>(pChaRec->chModalType), static_cast<int>(chCtrlType), static_cast<int>(chMainCha), ulWorldID, ulChaID, SArea.centre.x, SArea.centre.y, szName);
		return 0;
	}

	SPKCtrl.Exec(p);
	p->SetMainType((eMainChaType)chMainCha); // This value is zero for enumENTITY_SEEN_SWITCH, only set on initial creation
	SetValue(p);

	g_logManager.InternalLog(LogLevel::Debug, "common", std::format("MainChaType:{}", static_cast<int>(chMainCha)));

	p->setChaModalType(pChaRec->chModalType);
	if (SEvent.usEventID) {
		SEvent.Exec(p);
	}

	if (pChaRec->chModalType != enumMODAL_BOAT) {
		// Apply look
		if (p->GetMainType() != enumMainPlayer && pChaRec->chCtrlType == 5) {
			p->setTypeID(pChaRec->lScript);

			// Apply Avatar parts
			if (pChaRec->sSkinInfo[5]) {
				SLookInfo.SLook.SLink[enumEQUIP_LHAND].sID = (short)pChaRec->sSkinInfo[5];
				SLookInfo.SLook.SLink[enumEQUIP_LHAND].sNum = 1;
			}
			if (pChaRec->sSkinInfo[6]) {
				SLookInfo.SLook.SLink[enumEQUIP_RHAND].sID = (short)pChaRec->sSkinInfo[6];
				SLookInfo.SLook.SLink[enumEQUIP_RHAND].sNum = 1;
			}
		}
		NetChangeChaPart(p, SLookInfo);
	}
	SAppendLook.Exec(p);

	if (chMainCha) {
		if (chMainCha == enumMainPlayer) {
			// Main character, set up boat
			g_stUIBoat.Clear();
			g_stUIBoat.SetHuman(p);

			g_stUIStart.RefreshMainName(szName);
		}
		else if (chMainCha == enumMainBoat) {
			g_stUIBoat.AddBoat(p);
		}
		p->SetHide(TRUE);
	}
	else if (chCtrlType == enumCHACTRL_PLAYER) {
		g_stUIChat.GetTeamMgr()->Find(enumTeamRoad)->Add(ulWorldID, szCommName.c_str(), strMottoName.c_str(), sIcon);
	}

	if (chMainCha != 2) {
		switch (sState) {
		case enumEXISTS_SLEEPING:
			p->GetActor()->SetState(enumNormal);
			if (p->GetDefaultChaInfo()->sDormancy > 0) {
				p->PlayPose(p->GetDefaultChaInfo()->sDormancy);
				p->GetActor()->SetSleep();
			}
			break;
		case enumEXISTS_DIE:
		case enumEXISTS_WITHERING: // Already dead
		case enumEXISTS_RESUMEING:
			p->GetActor()->SetState(enumDied);
			p->PlayPose(POSE_FALLDOWN);
			break;
		case enumEXISTS_NATALITY: {
			p->GetActor()->SetState(enumNormal);
			p->PlayAni(p->GetDefaultChaInfo()->nBirthBehave, defCHA_DIE_EFFECT_NUM);

			CEffectObj* pEffect = p->GetScene()->GetFirstInvalidEffObj();
			if (pEffect && pEffect->Create(p->GetDefaultChaInfo()->sBornEff)) {
				pEffect->Emission(-1, &p->GetPos(), NULL);
				pEffect->SetValid(TRUE);
			}
		}
		break;
		default:
			p->GetActor()->SetState(enumNormal);
		}
	}

	switch (chCtrlType) {
	case enumCHACTRL_MONS_TREE:
	case enumCHACTRL_MONS_MINE:
		p->GetActor()->SetSleep();
		break;
	default:
		if (SEvent.usEventID) {
			p->GetActor()->SetSleep();
		}
	}

	p->setMobID(ulChaID);

	return p;
}

void stNetNPCShow::SetNpcShow(CCharacter* pCha) {
	g_logManager.InternalLog(LogLevel::Debug, "common", std::format("NpcType:{}\tNpcState:{}", static_cast<int>(byNpcType), static_cast<int>(byNpcState)));

	if (pCha) {
		pCha->setNpcState(byNpcState);
		pCha->setNpcType(byNpcType);
	}
}

void NetActorDestroy(unsigned int nID, char chSeeType) {
	ToLogService("common", "Destroy WorldID:{}", nID);
	g_stUIBooth.CloseBoothByOther(nID);

	if (chSeeType == enumENTITY_SEEN_SWITCH) {
		CCharacter* pCha = g_stUIBoat.FindCha(nID);
		if (pCha) {
			pCha->SetHide(TRUE);
		}
		else {
			g_logManager.InternalLog(LogLevel::Debug, "common", GetLanguageString(265));
		}
		return;
	}

	// Disappear
	CCharacter* pCha = CGameApp::GetCurScene()->SearchByID(nID);
	if (pCha) {
		if (pCha->getChaCtrlType() == enumCHACTRL_PLAYER) {
			CTeam* pTeam = g_stUIChat.GetTeamMgr()->Find(enumTeamRoad);
			pTeam->Del(nID);

			if (pCha->GetMainType() && pCha->IsBoat()) {
				CBoat* pBoat = g_stUIBoat.FindBoat(nID);
				if (pBoat) {
					pBoat->UnLink();
				}
			}
		}

		pCha->GetActor()->ExecAllNet();
		pCha->SetValid(FALSE);
	}
	else {
		CTeam* pTeam = g_stUIChat.GetTeamMgr()->Find(enumTeamRoad);
		pTeam->Del(nID);

		CBoat* pBoat = g_stUIBoat.FindBoat(nID);
		if (pBoat) {
			pBoat->UnLink();
		}
	}
}

void NetSynSkillState(DWORD dwCharID, stNetSkillState* pSSkillState) {
	CCharacter* pCha = CGameApp::GetCurScene()->SearchByID(dwCharID);
	if (pCha) {
		CSkillStateSynchro* pSynchro = new CSkillStateSynchro;
		pSynchro->SetCha(pCha);
		pSynchro->SetValue(pSSkillState->SState.GetValue(), pSSkillState->SState.GetCount());
		pSynchro->SetType(pSSkillState->chType);
		pSynchro->Exec();
	}
	else {
		g_logManager.InternalLog(LogLevel::Debug, "network", SafeVFormat(GetLanguageString(266), dwCharID));
	}
}

void NetActorSkillRep(unsigned int nID, stNetNotiSkillRepresent& SSkillRep) {
	bool isRep = g_IsValidFightState(SSkillRep.sState);

	// Find attacker
	CCharacter* pCha = CGameApp::GetCurScene()->SearchByID(nID);
	if (!pCha) {
		g_logManager.InternalLog(LogLevel::Debug, "network", SafeVFormat(GetLanguageString(267), SSkillRep.lSkillID, nID));
		return;
	}

	static CWaitAttackState* state = NULL;
	static CActionState* g_state = NULL;

	if (pCha->IsMainCha()) {
		state = dynamic_cast<CAttackState*>(pCha->GetActor()->GetCurState());
		if (!state) {
			g_logManager.InternalLog(LogLevel::Debug, "network", GetLanguageString(268));
			return;
		}

		if (state->IsInvalidHarm()) {
			state->SetServerID(SSkillRep.byFightID);
		}
	}
	else {
		g_state = pCha->GetActor()->GetServerStateByID(SSkillRep.byFightID);
		if (!g_state) {
			CSkillRecord* p = GetSkillRecordInfo(SSkillRep.lSkillID);
			if (!p) {
				ToLogService("network", "NetActorSkillRep GetSkillRecordInfo({}) return NULL", SSkillRep.lSkillID);
				return;
			}

			if (!isRep) {
				if (SSkillRep.SEffect.GetCount() > 0 || SSkillRep.SState.GetCount() > 0) {
					CAttackRepSynchro* eff = new CAttackRepSynchro;
					eff->SetSkill(p);
					eff->SetAttackCha(pCha);
					eff->SetTargetCha(pCha);
					eff->SetRepValue(SSkillRep.SEffect.GetValue(), SSkillRep.SEffect.GetCount());
					eff->SetRepState(SSkillRep.SState.GetValue(), SSkillRep.SState.GetCount());
					eff->Exec();
				}
				return;
			}

			state = new CWaitAttackState(pCha->GetActor());
			state->SetSkill(p);
			state->SetServerID(SSkillRep.byFightID);
			state->SetAttackPoint(SSkillRep.STargetPoint.x, SSkillRep.STargetPoint.y);
			if (!p->IsAttackArea()) {
				CCharacter* pFace = CGameApp::GetCurScene()->SearchByID(SSkillRep.lTargetID);
				if (pFace) {
					state->SetTarget(pFace);
				}
			}

			if (!pCha->GetActor()->AddState(state)) {
				g_pGameApp->AddTipText("NetActorSkillRep AddState return false\n");
				return;
			}
		}
		else {
			state = dynamic_cast<CWaitAttackState*>(g_state);
			if (!state) {
				ToLogService("network", "msgNetActorSkillRep g_state[{}] not is CWaitAttackState", static_cast<int>(SSkillRep.byFightID));
				return;
			}
		}
	}

	state->SetSkillSpeed(SSkillRep.lSkillSpeed);
	if (SSkillRep.SEffect.GetCount() > 0 || SSkillRep.SState.GetCount() > 0) {
		CAttackRepSynchro* eff = new CAttackRepSynchro;
		eff->SetSkill(state->GetSkill());
		eff->SetTargetCha(pCha);
		eff->SetAttackCha(pCha);
		eff->SetRepValue(SSkillRep.SEffect.GetValue(), SSkillRep.SEffect.GetCount());
		eff->SetRepState(SSkillRep.SState.GetValue(), SSkillRep.SState.GetCount());

		CServerHarm* pHarm = state->GetServerHarm();
		//if( !pHarm || !pHarm->AddRep(eff) )
		{
			eff->Exec();
		}
	}

	state->ServerYaw(SSkillRep.sAngle);
	if (SSkillRep.sState) {
		state->ServerEnd(SSkillRep.sState);
	}
}

// Effect chain
/*
void NetActorSkillEff(unsigned int nID, stNetNotiSkillEffect &SkillEff)
{
    CGameScene* pScene = CGameApp::GetCurScene();
    if( !pScene ) return;

	CSkillRecord *pSkill =  GetSkillRecordInfo( SkillEff.lSkillID );
	if( !pSkill )
	{
		g_logManager.InternalLog(LogLevel::Debug, "common", SafeVFormat(GetLanguageString(269), SkillEff.lSkillID));
		return;
	}

	// Find target
	CCharacter* pTarget = pScene->SearchByID( nID );
	if( !pTarget )
	{
		ToLogService("common", LogLevel::Error, "NetActorSkillEff Error, Target[{}] is Null, SkillID[{}]", nID, SkillEff.lSkillID);
		return;
	}

	// Find attacker
	CCharacter* pAttack = pScene->SearchByID( SkillEff.lSrcID );
	if( !pAttack )
	{
		// Attacker not in view, may be null
	}

    CServerHarm* pHarm = NULL;
    if( pAttack )
    {
        pHarm = pAttack->GetActor()->FindHarm( SkillEff.byFightID );
    }
    else
    {
        // When attacker not found and skill has projectile effect, create matching attack effect
        if( pSkill->IsEffectHarm() )
        {
            CEffectObj	*pEffect = CGameApp::GetCurScene()->GetFirstInvalidEffObj();
            if( pEffect && pEffect->Create( pSkill->sSkyEffect ) )
            {
		        D3DXVECTOR3 pos, target;
                pos.x = (float)SkillEff.SSrcPos.x / 100;
		        pos.y = (float)SkillEff.SSrcPos.y / 100;

                if( pSkill->IsAttackArea() )
                {
                    target.x = (float)SkillEff.SSkillTPos.x / 100;
                    target.y = (float)SkillEff.SSkillTPos.y / 100;
                    target.z = CGameApp::GetCurScene()->GetGridHeight( target.x, target.y );
                }
                else
                {
			        lwMatrix44 mat;
			        if( pTarget->GetObjDummyRunTimeMatrix( &mat, pSkill->sTargetDummyLink ) >=0 )
			        {
				        target = *(D3DXVECTOR3*)&mat._41;
			        }
                    else
                    {
                        target = pTarget->GetPos();
                    }
                }
                pos.z = target.z + 0.5f;

                pEffect->Emission( -1, &pos, &target );
	            pEffect->SetVel( (float)pSkill->sSkySpd );
	            pEffect->SetValid(TRUE);

				if( !pHarm )
				{
					pHarm = pTarget->GetActor()->CreateHarmMgr();
					pHarm->SetFightID( SkillEff.byFightID );
					pHarm->SetSkill( pSkill );

					pHarm->ReadyExec();
                }
				CHitRepresent Hit;
				Hit.SetAttackPoint( SkillEff.SSrcPos.x, SkillEff.SSrcPos.y );
				Hit.SetSkill( pSkill );
				Hit.SetTarget( pTarget );
                pEffect->GetEffDelay()->SetServerHarm( Hit, pHarm );
            }
        }
    }

	CAttackEffect*  eff = new CAttackEffect;
    eff->SetSkill( pSkill );
	eff->SetTargetCha( pTarget );
    eff->SetAttackCha( pAttack );

    // Damage values
    eff->SetIsDoubleAttack( SkillEff.bDoubleAttack );
    eff->SetIsMiss( SkillEff.bMiss );
    eff->SetBeatPos( SkillEff.bBeatBack, SkillEff.SPos.x, SkillEff.SPos.y );
	eff->SetHarmValue( SkillEff.SEffect.GetValue(), SkillEff.SEffect.GetCount() );
	eff->SetHarmState( SkillEff.SState.GetValue(), SkillEff.SState.GetCount() );
	if( SkillEff.sState & enumFSTATE_DIE )
	{
        eff->SetTargetIsDied( true );

		if( pTarget->IsMainCha() )
		{
			CAttackEffect::ChaDied( pTarget, pAttack );
		}
	}

    // Attacker feedback

	if( SkillEff.SSrcEffect.GetCount()>0 || SkillEff.SSrcState.GetCount()>0 || (SkillEff.sSrcState & enumFSTATE_DIE) )
	{
		CAttackRepSynchro*  rep = new CAttackRepSynchro;
		rep->SetSkill( pSkill );
		rep->SetAttackEffect( eff );
		rep->SetTargetCha( pTarget );
		rep->SetAttackCha( pAttack );
		rep->SetRepValue( SkillEff.SSrcEffect.GetValue(), SkillEff.SSrcEffect.GetCount() );
		rep->SetRepState( SkillEff.SSrcState.GetValue(), SkillEff.SSrcState.GetCount() );
		if( SkillEff.sSrcState & enumFSTATE_DIE )
		{
			rep->SetAttackIsDied( true );

			if( pAttack->IsMainCha() )
			{
				CAttackEffect::ChaDied( pAttack, pTarget );
			}
		}
		eff->SetAttackRep( rep );

		// Defer execution, wait for eff to execute first
		rep->Reset();
	}


    if( !pHarm || !pHarm->AddHarm( eff, pSkill ) )
    {
		eff->Exec();
    }

	//CGameScene* pScene = CGameApp::GetCurScene();
	//CCharacter* pTarget = pScene->SearchByID( nID );

	if(pTarget->getHumanID() == g_stUIStart.targetInfoID){
		g_stUIStart.RefreshTargetLifeNum(pTarget->getHP(),pTarget->getHPMax());
	}


	return;
}*/


void NetActorSkillEff(unsigned int nID, stNetNotiSkillEffect& SkillEff) {
	CGameScene* pScene = CGameApp::GetCurScene();
	if (!pScene) return;

	CSkillRecord* pSkill = GetSkillRecordInfo(SkillEff.lSkillID);
	if (!pSkill) {
		//LG( "protocol", RES_STRING(CL_LANGUAGE_MATCH_269), SkillEff.lSkillID );
		return;
	}

	// Find target
	CCharacter* pTarget = pScene->SearchByID(nID);
	if (!pTarget) {
		//LG( "protocol", "msgNetActorSkillEff Error, Target[%u] is Null, SkillID[%u]\n", nID, SkillEff.lSkillID );
		return;
	}

	// Find attacker
	CCharacter* pAttack = pScene->SearchByID(SkillEff.lSrcID);
	if (!pAttack) {
		// Attacker not in view, may be null
	}

	CServerHarm* pHarm = NULL;
	if (pAttack) {
		pHarm = pAttack->GetActor()->FindHarm(SkillEff.byFightID);
	}
	else {
		// When attacker not found and skill has projectile effect, create matching attack effect
		if (pSkill->IsEffectHarm()) {
			CEffectObj* pEffect = CGameApp::GetCurScene()->GetFirstInvalidEffObj();
			if (pEffect && pEffect->Create(pSkill->sSkyEffect)) {
				D3DXVECTOR3 pos, target;
				pos.x = (float)SkillEff.SSrcPos.x / 100;
				pos.y = (float)SkillEff.SSrcPos.y / 100;

				if (pSkill->IsAttackArea()) {
					target.x = (float)SkillEff.SSkillTPos.x / 100;
					target.y = (float)SkillEff.SSkillTPos.y / 100;
					target.z = CGameApp::GetCurScene()->GetGridHeight(target.x, target.y);
				}
				else {
					lwMatrix44 mat;
					if (pTarget->GetObjDummyRunTimeMatrix(&mat, pSkill->sTargetDummyLink) >= 0) {
						target = *(D3DXVECTOR3*)&mat._41;
					}
					else {
						target = pTarget->GetPos();
					}
				}
				pos.z = target.z + 0.5f;

				pEffect->Emission(-1, &pos, &target);
				pEffect->SetVel((float)pSkill->sSkySpd);
				pEffect->SetValid(TRUE);

				if (!pHarm) {
					pHarm = pTarget->GetActor()->CreateHarmMgr();
					pHarm->SetFightID(SkillEff.byFightID);
					pHarm->SetSkill(pSkill);

					pHarm->ReadyExec();
				}
				CHitRepresent Hit;
				Hit.SetAttackPoint(SkillEff.SSrcPos.x, SkillEff.SSrcPos.y);
				Hit.SetSkill(pSkill);
				Hit.SetTarget(pTarget);
				pEffect->GetEffDelay()->SetServerHarm(Hit, pHarm);
			}
		}
	}

	CAttackEffect* eff = new CAttackEffect;
	eff->SetSkill(pSkill);
	eff->SetTargetCha(pTarget);
	eff->SetAttackCha(pAttack);

	// Damage values
	eff->SetIsDoubleAttack(SkillEff.bDoubleAttack);
	eff->SetIsMiss(SkillEff.bMiss);
	eff->SetBeatPos(SkillEff.bBeatBack, SkillEff.SPos.x, SkillEff.SPos.y);
	eff->SetHarmValue(SkillEff.SEffect.GetValue(), SkillEff.SEffect.GetCount());
	eff->SetHarmState(SkillEff.SState.GetValue(), SkillEff.SState.GetCount());
	if (SkillEff.sState & enumFSTATE_DIE) {
		eff->SetTargetIsDied(true);

		if (pTarget->IsMainCha()) {
			CAttackEffect::ChaDied(pTarget, pAttack);
		}
	}

	// Attacker feedback
	if (SkillEff.SSrcEffect.GetCount() > 0 || SkillEff.SSrcState.GetCount() > 0 || (SkillEff.sSrcState &
		enumFSTATE_DIE)) {
		CAttackRepSynchro* rep = new CAttackRepSynchro;
		rep->SetSkill(pSkill);
		rep->SetAttackEffect(eff);
		rep->SetTargetCha(pTarget);
		rep->SetAttackCha(pAttack);
		rep->SetRepValue(SkillEff.SSrcEffect.GetValue(), SkillEff.SSrcEffect.GetCount());
		rep->SetRepState(SkillEff.SSrcState.GetValue(), SkillEff.SSrcState.GetCount());
		if (SkillEff.sSrcState & enumFSTATE_DIE) {
			rep->SetAttackIsDied(true);

			if (pAttack->IsMainCha()) {
				CAttackEffect::ChaDied(pAttack, pTarget);
			}
		}
		eff->SetAttackRep(rep);

		// Defer execution, wait for eff to execute first
		rep->Reset();
	}

	if (!pHarm || !pHarm->AddHarm(eff, pSkill)) {
		eff->Exec();
	}
	return;
}

void NetActorLean(unsigned int nID, stNetLeanInfo& lean) {
	// Sit, stand
	if (lean.chState == 0) {
		CCharacter* cha = CGameApp::GetCurScene()->GetMainCha();
		if (cha && cha->getAttachID() == nID) {
		}
		else {
			cha = CGameApp::GetCurScene()->SearchByID(nID);
			if (!cha) {
				g_logManager.InternalLog(LogLevel::Debug, "network", SafeVFormat(GetLanguageString(270), nID));
				return;
			}


			//if( !cha->GetActor()->GetCurState() )
			{
				CSeatState* st = new CSeatState(cha->GetActor());
				st->SetIsSend(false);
				st->SetPos(lean.lPosX, lean.lPosY);
				st->SetAngle(lean.lAngle);
				st->SetPose(lean.lPose);
				st->SetHeight(lean.lHeight);

				if (!cha->GetActor()->AddState(st)) {
					g_pGameApp->AddTipText("NetActorLean NewState return NULL\n");
					return;
				}
			}
		}
	}
	else {
		CCharacter* cha = GetCharacter(nID, "NetActorLean");
		if (cha) {
			CActionState* st = cha->GetActor()->GetCurState();
			if (st) {
				st->ServerEnd(lean.chState);
			}
		}
	}
}

void NetSwitchMap(stNetSwitchMap& switchmap) {
#ifdef _TEST_CLIENT
	static int i = 0;
	i++;
	//  dev-only  test_client
	return;
#endif

	// Anti-cheat: randomize character memory layout to prevent simple memory value reading
	static char* pMemory = new char[1];
	delete [] pMemory;
	pMemory = new char[rand() % 777 + 1];

	g_ChaExitOnTime.Reset();

	if (switchmap.sEnterRet != ERR_SUCCESS) {
		if (switchmap.sEnterRet == ERR_MC_ENTER_ERROR) // Character still online, cannot enter for 15 minutes
		{
			g_pGameApp->MsgBox("%s", GetLanguageString(271).c_str());
			CGameApp::Waiting(false);
		}
		else {
			g_pGameApp->SendMessage(APP_SWITCH_MAP_FAILED, switchmap.sEnterRet);
		}
		return;
	}

	// comment by Philip.Wu  2006-07-06
	// BUG report: TEST-46  Team leader lost when members on different maps
	//g_stUIStart.SetIsLeader( false );

	g_stUIStart.SetIsNewer(switchmap.bIsNewCha);
	g_stUIChat.GetTeamMgr()->Find(enumTeamRoad)->Clear();
	g_stUIStart.SetIsCanTeam(switchmap.bCanTeam);

	CMapInfo* pInfo = GetMapInfo(switchmap.szMapName.c_str());
	if (!pInfo) {
		ToLogService("map", LogLevel::Error, "msgNetSwitchMap - GetMapInfo({}) return NULL", switchmap.szMapName);
		return;
	}

	g_stUIMap.RefreshMapName(pInfo->szName);

	CGameApp::Waiting(false);
	CWorldScene* s = dynamic_cast<CWorldScene*>(CGameApp::GetCurScene());
	if (!s) // Enter game scene
	{
		stSceneInitParam init;
		init.strMapFile = pInfo->DataName.c_str();

		init.nMaxCha = g_Config.m_dwMaxCha;
		init.nMaxEff = g_Config.m_dwMaxEff;
		init.nMaxItem = g_Config.m_dwMaxItem;
		init.nMaxObj = g_Config.m_dwMaxObj;
		init.nUITemplete = enumMainForm;
		init.strName = pInfo->szName;
		init.nTypeID = enumWorldScene;
		s = dynamic_cast<CWorldScene*>(g_pGameApp->CreateScene(&init));
		if (!s) {
			ToLogService("map", LogLevel::Error, "msgNetSwitchMap({}) CreateScene return NULL", switchmap.szMapName);
			return;
		}

		g_pGameApp->GotoScene(s, true, switchmap.chEnterType == enumENTER_MAP_CARRY);
		CTalkSessionFormMgr::ClearAll();
		g_stUIChat.GetTeamMgr()->ResetAll();
		//CCozeMgr::ResetAll();
		CCozeForm::GetInstance()->OnResetAll();
		g_stUIMap.ClearRadar();
		g_cFindPathEx.Reset();
		g_cFindPath.SetShortPathFinding(128, 38);

		if (switchmap.chEnterType == enumENTER_MAP_CARRY) {
			if (!g_Config.IsPower()) {
				g_logManager.Shutdown();
			}
		}

		// Record game login time
		CGameApp::SetLoginTime(GetTickCount());
	}
	else {
		if (switchmap.chEnterType == enumENTER_MAP_CARRY) {
			g_pGameApp->Loading();

			if (!g_Config.IsPower()) {
				g_logManager.Shutdown();
			}
		}

		if (!s->SwitchMap(pInfo->Id)) {
			ToLogService("map", LogLevel::Error, "msgNetSwitchMap({}) SwitchMap Failed", switchmap.szMapName);
			return;
		}
	}
}

void NetBickerInfo(const char szData[]) {
	g_pGameApp->ShowMidText(szData);
}

void NetColourInfo(unsigned int rgb, const char szData[]) {
	g_pGameApp->ShowBottomText(rgb, szData);
}

void NetSysInfo(stNetSysInfo& sysinfo) {
	//g_stUICoze.OnSystemSay( sysinfo.m_sysinfo );
	//CCharMsg::SetChannelColor(CCharMsg::CHANNEL_SYSTEM,sysinfo.dwColor);
	CCozeForm::GetInstance()->OnSystemMsg(sysinfo.m_sysinfo);
}

void NetSideInfo(const char szName[], const char szInfo[]) {
	CCozeForm::GetInstance()->OnSideMsg(szName, szInfo);
}

void NetSay(stNetSay& netsay, DWORD dwColour) // Nearby chat
{
	CCharacter* cha = GetCharacter(netsay.m_srcid, "NetSay");
	if (!cha) {
		ToLogService("common", "roadsay not find, ID:[{}], say:{}", netsay.m_srcid, netsay.m_content);
		return;
	}

	if (!cha->IsPlayer()) {
		g_logManager.InternalLog(LogLevel::Debug, "common", SafeVFormat(GetLanguageString(272), netsay.m_srcid, cha->getName().c_str(), cha->getLogName(),
		   netsay.m_content));
	}

	//g_stUICoze.OnRoadSay( cha, netsay.m_content );
	CCozeForm::GetInstance()->OnSightMsg(cha, netsay.m_content, dwColour);
}

CSceneItem* NetCreateItem(stNetItemCreate& info) {
#ifdef _TEST_CLIENT
	return NULL;
#endif

	ToLogService("common", "Create - ID:{}, Angle:{}, Pos:[{}, {}], WorldID:{}, EventID:{}", info.lID, info.sAngle, info.SPos.x,
	   info.SPos.y, info.lWorldID, info.SEvent.usEventID);

	CGameScene* pScene = CGameApp::GetCurScene();
	CSceneItem* pItem = pScene->SearchItemByID(info.lWorldID);
	if (pItem) {
		g_logManager.InternalLog(LogLevel::Debug, "common", SafeVFormat(GetLanguageString(273), info.lID, pItem->GetItemInfo()->szName));
		pItem->SetValid(FALSE);
	}

	pItem = pScene->AddSceneItem(info.lID, 0);
	if (pItem == NULL) {
		g_logManager.InternalLog(LogLevel::Debug, "network", SafeVFormat(GetLanguageString(274), info.lID));
		return NULL;
	}

	pItem->setIsShowName(true);
	pItem->setYaw(info.sAngle);
	pItem->setPos(info.SPos.x, info.SPos.y);
	pItem->setAttachID(info.lWorldID);
	pItem->lTag = info.lHandle;
	pItem->PlayObjImpPose(ANIM_CTRL_TYPE_MAT, 0, PLAY_LOOP, 0.0f, 2.0f);

	if (info.SEvent.usEventID) {
		info.SEvent.Exec(pItem);
		pItem->setIsShowName(false);
		return pItem;
	}

	CMonsterItem* pThrowItem = new CMonsterItem;
	pThrowItem->SetItem(pItem);

	switch (info.chAppeType) {
	case enumITEM_APPE_THROW: // Thrown by character
	case enumITEM_APPE_MONS: // Dropped by monster
	{
		if (info.lFromID) {
			CCharacter* pFromCha = CGameApp::GetCurScene()->SearchByID(info.lFromID);
			if (pFromCha) {
				pThrowItem->SetCha(pFromCha);
				if (pFromCha->IsResource())
					break;

				ToLogService("common", "\tCreateItem, Type:{} - ID:{}, Name:{}, ItemPos:[{}, {}], WorldID:{}, ChaLogName:{}, ChaPos[{}, {}]",
				   static_cast<int>(info.chAppeType), info.lID, pItem->GetItemInfo()->szName, info.SPos.x, info.SPos.y, info.lWorldID,
				   pFromCha->getLogName(), pFromCha->GetCurX(), pFromCha->GetCurY());

				if (info.chAppeType == enumITEM_APPE_MONS) {
					if (pFromCha->GetActor()->AddDieExec(pThrowItem)) {
						pItem->SetHide(TRUE);
						return pItem;
					}
				}
			}
		}
	}
	break;
	case enumITEM_APPE_NATURAL: // Natural spawn
		break;
	}

	pThrowItem->Exec();
	delete pThrowItem;
	return pItem;
}

void NetItemDisappear(unsigned int nID) {
	ToLogService("common", "Disappear - WorldID:{}", nID);

	// Item disappear
	if (!CGameApp::GetCurScene()) return;

	CSceneItem* pItem = CGameApp::GetCurScene()->SearchItemByID(nID);
	if (pItem) {
		pItem->SetValid(FALSE);
	}
}

void NetChangeChaPart(CCharacter* pCha, stNetLookInfo& SLookInfo) {
	stNetChangeChaPart& SPart = SLookInfo.SLook;
	if (pCha->getChaModalType() == enumMODAL_BOAT) {
		pCha->LoadBoat(SPart);
	}
	else {
		if (SLookInfo.chSynType == enumSYN_LOOK_SWITCH) {
			if (SPart.sTypeID != 0 && pCha->getTypeID() != SPart.sTypeID) {
				if (SPart.sTypeID != 0 && SPart.sTypeID != pCha->getTypeID())
					pCha->ReCreate(SPart.sTypeID);
			}

			pCha->UpdataFace(SPart);
		}

		if (pCha->GetMainType() == enumMainPlayer) {
			g_stUIStart.RefreshPet(SPart.SLink[16], SPart.SLink[24].sID != 0 ? SPart.SLink[24] : SPart.SLink[16]);

			std::ostrstream str;
			str << "ChangePart Type:" << SPart.sTypeID << ", Item:" << SPart.SLink[0].sID;
			for (int i = 1; i < enumEQUIP_NUM; i++) {
				str << ", " << SPart.SLink[i].sID;
			}
			str << "\r\n";
			str << '\0';
			g_logManager.InternalLog(LogLevel::Debug, "ui", str.str());

			if (SLookInfo.chSynType == enumSYN_LOOK_SWITCH) {
				g_stUIStart.RefreshMainFace(SPart);
				g_stUIEquip.UpdataEquip(SPart, pCha);
			}
			else {
				g_stUIEquip.UpdataEquipData(SPart, pCha);
			}
		}
	}
}

void NetChangeChaPart(unsigned int nID, stNetLookInfo& SLookInfo) {
	CCharacter* pCha = CGameApp::GetCurScene()->SearchByID(nID);
	if (pCha) {
		NetChangeChaPart(pCha, SLookInfo);
	}
}

void NetChangeChaLookEnergy(unsigned int nID, stLookEnergy& SLookEnergy) {
	g_logManager.InternalLog(LogLevel::Debug, "common", GetLanguageString(275));
	for (int i = 0; i < enumEQUIP_NUM; i++) {
		g_logManager.InternalLog(LogLevel::Debug, "common", std::format("{}: {}", i, SLookEnergy.sEnergy[i]));
	}

	CItemCommand* pItem = NULL;
	for (int i = 0; i < enumEQUIP_NUM; i++) {
		pItem = g_stUIEquip.GetEquipItem(i);
		if (pItem) {
			pItem->GetData().sEnergy[0] = SLookEnergy.sEnergy[i];
		}
	}
}

void NetQueryRelive(unsigned int nID, stNetQueryRelive& SQueryRelive) {
	g_logManager.InternalLog(LogLevel::Debug, "common", SafeVFormat(GetLanguageString(276), nID, GetTickCount()));
	g_stUIStart.ShowQueryReliveForm(SQueryRelive.chType, SQueryRelive.szSrcChaName.c_str());
}

void NetPreMoveTime(unsigned long ulTime) {
	CWaitMoveState::SetPreMoveTime(ulTime);
}

void NetMapMask(unsigned int nID, BYTE* pMask, long lLen) {
	// If pMask is empty, there is no large map
	if (pMask) {
		if (CMaskData::g_MaskData) {
			CMaskData::g_MaskData->InitMaskData(pMask, lLen);
		}

		if (CGameApp::GetCurScene() && CGameApp::GetCurScene()->GetLargerMap()) {
			CGameApp::GetCurScene()->GetLargerMap()->Show(true);
		}
	}
	else {
		g_stUIMap.GetBigmapForm()->Hide();
		g_pGameApp->SysInfo("%s", GetLanguageString(277).c_str());
	}

	g_pGameApp->Waiting(false);
}

void NetTempChangeChaPart(unsigned int nID, stTempChangeChaPart& SPart) {
	// Temporary protocol, equip part
	CCharacter* cha = GetCharacter(nID, "NetTempChangeChaPart");
	if (!cha) return;

	cha->LoadPart(SPart.dwPartID, SPart.dwItemID);
}

void NetActorChangeCha(unsigned int nID, stNetChangeCha& SChangeCha) {
	// log
	g_logManager.InternalLog(LogLevel::Debug, "common", SafeVFormat(GetLanguageString(278), GetTickCount()));
	g_logManager.InternalLog(LogLevel::Debug, "common", std::format("New Character ID: {}\tOld Character ID: {}", SChangeCha.ulMainChaID, nID));
	//

	g_stUIBoat.ChangeMainCha(SChangeCha.ulMainChaID);
}

void NetShowTalk(const char szTalk[], BYTE byCmd, DWORD dwNpcID) {
	g_stUINpcTalk.ShowTalkPage(szTalk, byCmd, dwNpcID);
}

void NetShowHelp(const NET_HELPINFO& Info) {
	g_stUINpcTalk.AddHelpInfo(Info);
}

void NetShowMapCrash(const char szTalk[]) {
	g_pGameApp->MsgBox(szTalk);
}

void NetShowFunction(BYTE byFuncPage, BYTE byFuncNum, BYTE byMisNum, const NET_FUNCPAGE& FuncArray, DWORD dwNpcID) {
	g_stUINpcTalk.ShowFuncPage(byFuncPage, byFuncNum, byMisNum, FuncArray, dwNpcID);
}

void NetShowMissionList(DWORD dwNpcID, const NET_MISSIONLIST& MisList) {
	//g_stUINpcTalk.ShowMissionList( dwNpcID, MisList );
}

void NetShowMisPage(DWORD dwNpcID, BYTE byCmd, const NET_MISPAGE& page) {
	g_stUIMission.ShowMissionPage(dwNpcID, byCmd, page);
}

void NetMisLogList(NET_MISLOG_LIST& List) {
	g_stUIMisLog.MisLogList(List);
}

void NetShowMisLog(WORD wMisID, const NET_MISPAGE& page) {
	g_stUIMisLog.MissionLog(wMisID, page);
}

void NetMisLogClear(WORD wMisID) {
	g_stUIMisLog.MisClear(wMisID);
}

void NetMisLogAdd(WORD wMisID, BYTE byState) {
	g_stUIMisLog.MisAddLog(wMisID, byState);
}

void NetMisLogState(WORD wID, BYTE byState) {
	g_stUIMisLog.MisLogState(wID, byState);
}

void NetCloseTalk(DWORD dwNpcID) {
	g_stUINpcTalk.CloseTalk(dwNpcID);
}

void NetCreateBoat(const xShipBuildInfo& Info) {
	xShipFactory* pShip =
		((CWorldScene*)g_pGameApp->GetCurScene())->GetShipMgr()->_factory;
	if (!pShip)
		return;

	// If shipyard window is already open, close it first
	if (pShip->sbf.wnd->GetIsShow())
		pShip->sbf.wnd->SetIsShow(false);

	pShip->SetState(xShipFactory::STATE_CREATE);
	pShip->SetBoatID(-1);

	pShip->UpdateBoatCreate((xShipBuildInfo*)&Info, 1, 0);
}

void NetUpdateBoat(const xShipBuildInfo& Info) {
	((CWorldScene*)g_pGameApp->GetCurScene())->GetShipMgr()->_factory->Update((xShipBuildInfo*)&Info, 1, 0);
}

void NetBoatInfo(DWORD dwBoatID, const char szName[], const xShipBuildInfo& Info) {
	xShipFactory* pShip =
		((CWorldScene*)g_pGameApp->GetCurScene())->GetShipMgr()->_factory;
	if (!pShip)
		return;

	// If shipyard window is already open, close it first
	if (pShip->sbf.wnd->GetIsShow())
		pShip->sbf.wnd->SetIsShow(false);

	xShipFactory::eState oldState = pShip->GetState();
	DWORD oldBoatID = pShip->GetBoatID();

	pShip->SetState(xShipFactory::STATE_INFO);
	pShip->SetBoatID(dwBoatID);
	if (!pShip->UpdateBoatInfo((xShipBuildInfo*)&Info, 0, szName)) {
		pShip->SetState(oldState);
		pShip->SetBoatID(oldBoatID);
	}
}

void NetShowBoatList(DWORD dwNpcID, BYTE byNumBoat, const BOAT_BERTH_DATA& Data, BYTE byType) {
	((CWorldScene*)g_pGameApp->GetCurScene())->GetShipMgr()->
											   _launch_list->SetType(byType);
	((CWorldScene*)g_pGameApp->GetCurScene())->GetShipMgr()->
											   _launch_list->SetNpcID(dwNpcID);

	// ShowBoatList( byNumBoat, Data );
	if (byType == mission::BERTH_LUANCH_LIST) {
		((CWorldScene*)g_pGameApp->GetCurScene())->GetShipMgr()->
												   _launch_list->Update(byNumBoat, &Data);
	}
	else if (byType == mission::BERTH_TRADE_LIST) {
		((CWorldScene*)g_pGameApp->GetCurScene())->GetShipMgr()->
												   _launch_list->Update(byNumBoat, &Data, xShipLaunchList::eTrade);
	}
	else if (byType == mission::BERTH_BAG_LIST) {
		((CWorldScene*)g_pGameApp->GetCurScene())->GetShipMgr()->
												   _launch_list->Update(byNumBoat, &Data, xShipLaunchList::eBag);
	}
	else if (byType == mission::BERTH_REPAIR_LIST) {
		((CWorldScene*)g_pGameApp->GetCurScene())->GetShipMgr()->
												   _launch_list->Update(byNumBoat, &Data, xShipLaunchList::eRepair);
	}
	else if (byType == mission::BERTH_SALVAGE_LIST) {
		((CWorldScene*)g_pGameApp->GetCurScene())->GetShipMgr()->
												   _launch_list->Update(byNumBoat, &Data, xShipLaunchList::eSalvage);
	}
	else if (byType == mission::BERTH_SUPPLY_LIST) {
		((CWorldScene*)g_pGameApp->GetCurScene())->GetShipMgr()->
												   _launch_list->Update(byNumBoat, &Data, xShipLaunchList::eSupply);
	}
	else if (byType == mission::BERTH_BOATLEVEL_LIST) {
		((CWorldScene*)g_pGameApp->GetCurScene())->GetShipMgr()->
												   _launch_list->Update(byNumBoat, &Data, xShipLaunchList::eUpgrade);
	}
}

void NetShowTrade(const NET_TRADEINFO& TradeInfo, BYTE byCmd, DWORD dwNpcID, DWORD dwParam) {
	if (byCmd == mission::TRADE_GOODS) {
		// Goods exchange trading (dwParam is first boat ID)
		g_stUIBourse.ShowBourse(TradeInfo, byCmd, dwNpcID, dwParam);
	}
	else if (byCmd == mission::TRADE_SALE || byCmd == mission::TRADE_BUY) {
		// Normal trading
		g_stUINpcTrade.ShowTradePage(TradeInfo, byCmd, dwNpcID);
	}
}

void NetUpdateTradeAllData(const NET_TRADEINFO& TradeInfo, BYTE byCmd, DWORD dwNpcID, DWORD dwParam) {
	if (byCmd == mission::TRADE_GOODS && dwNpcID == g_stUIBourse.GetNpcId() && g_stUIBourse.IsShow()) {
		if (0 == dwParam)
			dwParam = g_stUIBourse.GetBoatId();
		// Refresh all data
		g_stUIBourse.ShowBourse(TradeInfo, byCmd, dwNpcID, dwParam);
	}
}

void NetUpdateTradeData(DWORD dwNpcID, BYTE byPage, BYTE byIndex, USHORT sItemID, USHORT sCount, DWORD dwPrice) {
	if (dwNpcID == g_stUIBourse.GetNpcId() && g_stUIBourse.IsShow()) {
		// Update single item
		g_stUIBourse.UpdateOneGood(byPage, byIndex, sItemID, sCount, dwPrice);
	}
}

void NetTradeResult(BYTE byCmd, BYTE byIndex, BYTE byCount, USHORT sItemID, DWORD dwMoney) {
	if (byCmd == mission::TRADE_SALE) {
		g_stUINpcTrade.SaleToNpc(byIndex, byCount, sItemID, dwMoney);
	}
	else if (byCmd == mission::TRADE_BUY) {
		g_stUINpcTrade.BuyFromNpc(byIndex, byCount, sItemID, dwMoney);
	}
	return;
}

void NetShowCharTradeRequest(BYTE byType, DWORD dwRequestID) {
	g_stUITrade.ShowCharTradeRequest(byType, dwRequestID);
}

void NetShowCharTradeInfo(BYTE byType, DWORD dwAcceptID, DWORD dwRequestID) {
	g_stUITrade.ShowCharTrade(byType, dwAcceptID, dwRequestID);
}

void NetCancelCharTrade(DWORD dwCharID) {
	g_stUITrade.CancelCharTrade(dwCharID);
}

void NetTradeAddBoat(DWORD dwCharID, BYTE byOpType, USHORT sItemID, BYTE byIndex,
					 BYTE byCount, BYTE byItemIndex, const NET_CHARTRADE_BOATDATA& Data) {
	if (byOpType == mission::TRADE_DRAGTO_ITEM) {
		//
		g_stUITrade.DragTradeToItem(dwCharID, byIndex, byItemIndex);
	}
	else {
		// Update boat info
		g_stUITrade.DragItemToTrade(dwCharID, sItemID, byIndex, byCount, byItemIndex, NULL, &Data);
	}
}

void NetTradeAddItem(DWORD dwCharID, BYTE byOpType, USHORT sItemID, BYTE byIndex,
					 BYTE byCount, BYTE byItemIndex, const NET_CHARTRADE_ITEMDATA& Data) {
	if (byOpType == mission::TRADE_DRAGTO_ITEM) {
		g_stUITrade.DragTradeToItem(dwCharID, byIndex, byItemIndex);
	}
	else {
		SItemGrid sGrid;
		memset(&sGrid, 0, sizeof(sGrid));
		sGrid.sID = sItemID;
		sGrid.sNum = byCount;
		sGrid.chForgeLv = Data.byForgeLv;
		sGrid.sEndure[0] = Data.sEndure[0];
		sGrid.sEndure[1] = Data.sEndure[1];
		sGrid.sEnergy[0] = Data.sEnergy[0];
		sGrid.sEnergy[1] = Data.sEnergy[1];
		sGrid.bValid = Data.bValid;
		sGrid.bItemTradable = Data.bItemTradable;
		sGrid.expiration = Data.expiration;

		if (Data.byHasAttr) {
			sGrid.sInstAttr = Data.sInstAttr;
		}

		sGrid.lDBParam = Data.lDBParam;
		g_stUITrade.DragItemToTrade(dwCharID, sItemID, byIndex, byCount, byItemIndex, &sGrid, NULL);
	}
}

void NetTradeShowMoney(DWORD dwCharID, DWORD dwMoney) {
	g_stUITrade.ShowCharTradeMoney(dwCharID, dwMoney);
}

void NetTradeShowIMP(DWORD dwCharID, DWORD dwMoney) {
	g_stUITrade.ShowCharTradeIMP(dwCharID, dwMoney);
}

void NetValidateTradeData(DWORD dwCharID) {
	g_stUITrade.ValidateTradeData(dwCharID);
}

void NetValidateTrade(DWORD dwCharID) {
	g_stUITrade.ValidateTrade(dwCharID);
}

void NetTradeSuccess() {
	g_stUITrade.ShowTradeSuccess();
}

void NetTradeFailed() {
	g_stUITrade.ShowTradeFailed();
}

void NetStallInfo(DWORD dwCharID, BYTE byNum, const char szName[]) {
	g_stUIBooth.ShowTradeBoothForm(dwCharID, szName, byNum);
}

void NetStallAddBoat(BYTE byGrid, USHORT sItemID, BYTE byCount, DWORD dwMoney, NET_CHARTRADE_BOATDATA& Data) {
	g_stUIBooth.AddTradeBoothBoat(byGrid, sItemID, byCount, dwMoney, Data);
}

void NetStallAddItem(BYTE byGrid, USHORT sItemID, BYTE byCount, DWORD dwMoney, NET_CHARTRADE_ITEMDATA& Data) {
	SItemGrid sGrid;
	memset(&sGrid, 0, sizeof(sGrid));
	sGrid.sID = sItemID;
	sGrid.sNum = byCount;
	sGrid.chForgeLv = Data.byForgeLv;
	sGrid.sEndure[0] = Data.sEndure[0];
	sGrid.sEndure[1] = Data.sEndure[1];
	sGrid.sEnergy[0] = Data.sEnergy[0];
	sGrid.sEnergy[1] = Data.sEnergy[1];
	sGrid.bValid = Data.bValid;
	sGrid.bItemTradable = Data.bItemTradable;
	sGrid.expiration = Data.expiration;


	sGrid.lDBParam = Data.lDBParam;
	if (Data.byHasAttr) {
		sGrid.sInstAttr = Data.sInstAttr;
	}

	g_stUIBooth.AddTradeBoothGood(byGrid, sItemID, byCount, dwMoney, sGrid);
}

void NetStallDelGoods(DWORD dwCharID, BYTE byGrid, BYTE byCount) {
	if (g_stUIBooth.IsOpen()) {
		g_stUIBooth.RemoveTradeBoothItem(dwCharID, byGrid, byCount);
	}
}

void NetStallClose(DWORD dwCharID) {
	g_stUIBooth.PullBoothSuccess();
}

void NetStallSuccess(DWORD dwCharID) {
	g_stUIBooth.SetupBoothSuccess();
}

void NetStallName(DWORD dwCharID, const char* szStallName) {
	CCharacter* pCha = GetCharacter(dwCharID, "NetStallName");
	if (pCha) {
		pCha->setShopName(szStallName);
	}
}

void NetSynAttr(DWORD dwWorldID, char chType, short sNum, stEffect* pEffect) {
#ifdef _TEST_CLIENT
	return;
#endif

	//if( enumATTRSYN_INIT==chType )
	//{
	//	CCharacter* pCha = GetCharacter( dwCharID, "NetSynAttr" );
	//	if( !pCha ) return;

	//	int count = sNum;
	//	SGameAttr* pAttr = pCha->getGameAttr();
	//	for( int i=0; i<count; i++ )
	//	{
	//		pAttr->set( (short)pEffect[i].lAttrID, pEffect[i].lVal );
	//	}

	//	pCha->RefreshUI();
	//	return;
	//}

	CCharacter* pCha = CGameApp::GetCurScene()->SearchByID(dwWorldID);
	if (!pCha) {
		if (enumATTRSYN_INIT == chType)
			g_logManager.InternalLog(LogLevel::Debug, "network", SafeVFormat(GetLanguageString(279), dwWorldID));
		return;
	}

	CAttribSynchro* pSynchro = new CAttribSynchro;
	pSynchro->SetCha(pCha);
	pSynchro->SetValue(pEffect, sNum);
	pSynchro->SetType(chType);
	pSynchro->Start();

	g_stUIState.RefreshStateFrm();

	// Silver coin zone special handling
	if (g_stUIMap.IsPKSilver() && pCha->IsPlayer() && pCha->GetMainType() != enumMainPlayer && pCha->getGameAttr()) {
		SGameAttr* pAttr = pCha->getGameAttr();
		long nJob = pAttr->get(ATTR_JOB);

		stNetChangeChaPart SLook = pCha->GetPart();
		memset(&SLook.SLink, 0, sizeof(SItemGrid) * 10);

		SLook.SLink[enumEQUIP_BODY] = 289;
		SLook.SLink[enumEQUIP_BODY].sNum = 1;

		SLook.sHairID = 0; // Unify hair style

		switch (nJob) {
		case JOB_TYPE_JUJS: // Lance, Crusader
			SLook.SLink[enumEQUIP_BODY] = 1933;
			SLook.SLink[enumEQUIP_BODY].sNum = 1;
			SLook.SLink[enumEQUIP_GLOVE] = 477;
			SLook.SLink[enumEQUIP_GLOVE].sNum = 1;
			SLook.SLink[enumEQUIP_SHOES] = 653;
			SLook.SLink[enumEQUIP_SHOES].sNum = 1;
			SLook.SLink[enumEQUIP_RHAND] = 3803;
			SLook.SLink[enumEQUIP_RHAND].sNum = 1;
			break;

		case JOB_TYPE_SHUANGJS: // Carsise, Dual-Blade Warrior
			SLook.SLink[enumEQUIP_BODY] = 1930;
			SLook.SLink[enumEQUIP_BODY].sNum = 1;
			SLook.SLink[enumEQUIP_GLOVE] = 1937;
			SLook.SLink[enumEQUIP_GLOVE].sNum = 1;
			SLook.SLink[enumEQUIP_SHOES] = 1941;
			SLook.SLink[enumEQUIP_SHOES].sNum = 1;
			SLook.SLink[enumEQUIP_LHAND] = 3800;
			SLook.SLink[enumEQUIP_LHAND].sNum = 1;
			SLook.SLink[enumEQUIP_RHAND] = 3800;
			SLook.SLink[enumEQUIP_RHAND].sNum = 1;
			break;

		case JOB_TYPE_JUJISHOU: // Ami/Phyllis, Sharpshooter
			SLook.SLink[enumEQUIP_BODY] = 1945;
			SLook.SLink[enumEQUIP_BODY].sNum = 1;
			SLook.SLink[enumEQUIP_GLOVE] = 1949;
			SLook.SLink[enumEQUIP_GLOVE].sNum = 1;
			SLook.SLink[enumEQUIP_SHOES] = 1953;
			SLook.SLink[enumEQUIP_SHOES].sNum = 1;
			SLook.SLink[enumEQUIP_RHAND] = 3807;
			SLook.SLink[enumEQUIP_RHAND].sNum = 1;
			break;

		case JOB_TYPE_FENGYINSHI: // Ami/Phyllis, Seal Master
			SLook.SLink[enumEQUIP_BODY] = 1957;
			SLook.SLink[enumEQUIP_BODY].sNum = 1;
			SLook.SLink[enumEQUIP_GLOVE] = 1964;
			SLook.SLink[enumEQUIP_GLOVE].sNum = 1;
			SLook.SLink[enumEQUIP_SHOES] = 1971;
			SLook.SLink[enumEQUIP_SHOES].sNum = 1;
			SLook.SLink[enumEQUIP_RHAND] = 3811;
			SLook.SLink[enumEQUIP_RHAND].sNum = 1;
			break;

		case JOB_TYPE_HANGHAISHI: // Lance/Carsise, Voyager
			SLook.SLink[enumEQUIP_BODY] = 1978;
			SLook.SLink[enumEQUIP_BODY].sNum = 1;
			SLook.SLink[enumEQUIP_GLOVE] = 1982;
			SLook.SLink[enumEQUIP_GLOVE].sNum = 1;
			SLook.SLink[enumEQUIP_SHOES] = 1986;
			SLook.SLink[enumEQUIP_SHOES].sNum = 1;
			SLook.SLink[enumEQUIP_HEAD] = 2107;
			SLook.SLink[enumEQUIP_HEAD].sNum = 1;
			SLook.SLink[enumEQUIP_RHAND] = 3818;
			SLook.SLink[enumEQUIP_RHAND].sNum = 1;
			break;

		case JOB_TYPE_SHENGZHIZHE: // Lance/Carsise, Cleric
			SLook.SLink[enumEQUIP_BODY] = 1960;
			SLook.SLink[enumEQUIP_BODY].sNum = 1;
			SLook.SLink[enumEQUIP_GLOVE] = 1967;
			SLook.SLink[enumEQUIP_GLOVE].sNum = 1;
			SLook.SLink[enumEQUIP_SHOES] = 1974;
			SLook.SLink[enumEQUIP_SHOES].sNum = 1;
			SLook.SLink[enumEQUIP_HEAD] = 2207;
			SLook.SLink[enumEQUIP_HEAD].sNum = 1;
			SLook.SLink[enumEQUIP_RHAND] = 3814;
			SLook.SLink[enumEQUIP_RHAND].sNum = 1;
			break;
		}

		//if(! g_stUIEquip.GetEquipItem(enumEQUIP_LHAND))
		//{
		//	SLook.SLink[enumEQUIP_LHAND]      = 0;
		//	SLook.SLink[enumEQUIP_LHAND].sNum = 0;
		//}
		//if(! g_stUIEquip.GetEquipItem(enumEQUIP_RHAND))
		//{
		//	SLook.SLink[enumEQUIP_RHAND]      = 0;
		//	SLook.SLink[enumEQUIP_RHAND].sNum = 0;
		//}
		//if(! g_stUIEquip.GetEquipItem(enumEQUIP_BODY))
		//{
		//	SLook.SLink[enumEQUIP_BODY]      = 0;
		//	SLook.SLink[enumEQUIP_BODY].sNum = 0;
		//}
		//if(! g_stUIEquip.GetEquipItem(enumEQUIP_GLOVE))
		//{
		//	SLook.SLink[enumEQUIP_GLOVE]      = 0;
		//	SLook.SLink[enumEQUIP_GLOVE].sNum = 0;
		//}
		//if(! g_stUIEquip.GetEquipItem(enumEQUIP_HEAD))
		//{
		//	SLook.SLink[enumEQUIP_HEAD]      = 0;
		//	SLook.SLink[enumEQUIP_HEAD].sNum = 0;
		//}
		//if(! g_stUIEquip.GetEquipItem(enumEQUIP_SHOES))
		//{
		//	SLook.SLink[enumEQUIP_SHOES]      = 0;
		//	SLook.SLink[enumEQUIP_SHOES].sNum = 0;
		//}

		// Update character
		if (SLook.sTypeID != 0 && pCha->getTypeID() != SLook.sTypeID) {
			if (SLook.sTypeID != 0 && SLook.sTypeID != pCha->getTypeID())
				pCha->ReCreate(SLook.sTypeID);
		}
		pCha->UpdataFace(SLook);
	}

	if (pCha->getHumanID() == g_stUIStart.targetInfoID) {
		g_stUIStart.RefreshTargetLifeNum(pCha->getHP(), pCha->getHPMax());
	}
}

void NetFace(DWORD dwCharID, stNetFace& netface, char chType) {
	if (enumACTION_SKILL_POSE == chType) {
		CCharacter* pCha = GetCharacter(dwCharID, "NetSkillFace");
		if (!pCha) return;

		if (netface.sPose == POSE_WAITING) {
			CInsertState* st = dynamic_cast<CInsertState*>(pCha->GetActor()->GetCurState());
			if (st) {
				st->ServerEnd(0);
				if (pCha->GetIsMountEquipped() && !pCha->GetIsOnMount() && g_stUISystem.m_sysProp.m_gameOption.
					bShowMounts && !pCha->GetIsPK()) {
					pCha->RespawnMount();
				}
			}
			else {
				ToLogService("network", LogLevel::Error, "msgNetSkillFace Error not InsertState!");
			}
		}
		else {
			if (pCha->IsMainCha()) {
				return;
			}
			else {
				CActor* pActor = pCha->GetActor();
				CInsertState* st = dynamic_cast<CInsertState*>(pCha->GetActor()->GetCurState());
				if (st) {
					//pCha->FaceTo( netface.sAngle );
					st->SetAngle(netface.sAngle);
					st->Start();
				}
				else {
					st = new CInsertState(pActor);
					st->SetAngle(netface.sAngle);
					st->SetIsSend(false);
					pActor->SwitchState(st);
				}
			}
		}
	}
	else {
		CCharacter* pCha = GetCharacter(dwCharID, "NetFace");
		if (!pCha) return;

		if (pCha == CGameApp::GetCurScene()->GetMainCha()) return;

		pCha->FaceTo(netface.sAngle);

		CPoseState* st = new CPoseState(pCha->GetActor());
		st->SetIsSend(false);
		st->SetPose(netface.sPose);
		st->SetKeepPose(true);
		pCha->GetActor()->SwitchState(st);
	}
}

void NetChangeKitbag(DWORD dwChaID, stNetKitbag& SKitbag) {
#ifdef _TEST_CLIENT
	return;
#endif

	CGoodsGrid* grd(0);

	switch (SKitbag.chBagType) {
	case 1:
		grd = g_stUIBank.GetBankGoodsGrid();
		break;
	case 2:
		grd = g_stUIStore.GetTempKitbagGrid();
		break;
	case 3:
		grd = g_stUIGuildBank.GetBankGoodsGrid();
		break;

	case 0:
	default:
		grd = g_stUIBoat.FindGoodsGrid(dwChaID);
		break;
	}

	if (!grd) return;

	char chType = SKitbag.chType;

	switch (chType) {
	case enumSYN_KITBAG_INIT: {
		//todo
		if (SKitbag.chBagType == 0) {
			NetKitbagLockedSpaces(SKitbag.nKeybagNum, grd);
		}
		else {
			int col = grd->GetCol();
			int row = SKitbag.nKeybagNum / col;
			if (SKitbag.nKeybagNum % col) row++;
			grd->Clear();
			grd->SetContent(row, col);
			grd->Init();
			grd->Refresh();
		}
	}
	break;
	case enumSYN_KITBAG_SWITCH:
		if (grd == g_stUIEquip.GetGoodsGrid()) {
			if (SKitbag.nGridNum == 2 && grd->SwapItem(SKitbag.Grid[0].sGridID, SKitbag.Grid[1].sGridID)) {
				break;
			}
		}
		break;
	}

	stNetKitbag::stGrid* pGrid = SKitbag.Grid;
	int count = SKitbag.nGridNum;
	if (count > grd->GetMaxNum()) {
		count = grd->GetMaxNum();
		g_logManager.InternalLog(LogLevel::Debug, "network", SafeVFormat(GetLanguageString(280), SKitbag.nGridNum, grd->GetMaxNum()));
	}

	CItemRecord* item = NULL;
	int nMarginNum = 0;

	int fastItemCount = 0;
	int fastCommandCount = 0;
	int fastItemSlot[2] = {-1, -1};
	int fastCommandSlot[36];
	fill_n(fastCommandSlot, 36, -1);

	for (int i = 0; i < count; i++) {
		ToLogService("common", "ID:{}, GridID:{}, Num:{}", pGrid[i].SGridContent.sID, pGrid[i].sGridID,
		   pGrid[i].SGridContent.sNum);

		if (pGrid[i].SGridContent.sID > 0) {
			if (chType == enumSYN_KITBAG_EQUIP) {
				if (fastItemCount < 2) {
					fastItemSlot[fastItemCount++] = pGrid[i].sGridID;
				}
				else {
					fastItemSlot[0] = -1;
					fastItemSlot[1] = -1;
				}
			}
			item = GetItemRecordInfo(pGrid[i].SGridContent.sID);
			if (!item) {
				g_logManager.InternalLog(LogLevel::Debug, "network", SafeVFormat(GetLanguageString(281), pGrid[i].SGridContent.sID));
				continue;
			}

			CItemCommand* pObj = dynamic_cast<CItemCommand*>(grd->GetItem(pGrid[i].sGridID));
			if (pObj && pObj->GetItemInfo() == item) {
				nMarginNum = pObj->GetTotalNum();
			}
			else {
				nMarginNum = 0;
				pObj = new CItemCommand(item);
				if (!grd->SetItem(pGrid[i].sGridID, pObj)) {
					g_logManager.InternalLog(LogLevel::Debug, "network", SafeVFormat(GetLanguageString(282), item->szName, pGrid[i].sGridID));
					continue;
				}
			}

			pObj->nTag = pGrid[i].SGridContent.sID;
			pObj->SetData(pGrid[i].SGridContent);
			//pObj->SetPrice(pGrid[i].SGridContent)
			nMarginNum = pObj->GetTotalNum() - nMarginNum;


		if (SKitbag.chBagType != 1 && nMarginNum > 0) // modify by Philip.Wu  2006-06-21  Picking 0 items BUG fix
			{
				switch (chType) {
				case enumSYN_KITBAG_PICK: g_pGameApp->SysInfo("%s", SafeVFormat(GetLanguageString(283), item->szName, nMarginNum).c_str());
					break;
			//case enumSYN_KITBAG_FROM_NPC:	g_pGameApp->SysInfo("NPC gave [%s x %d]!", item->szName, nMarginNum );	break;
				case enumSYN_KITBAG_SYSTEM: g_pGameApp->SysInfo("%s", SafeVFormat(GetLanguageString(284), item->szName, nMarginNum).c_str());
					break;
				case enumSYN_KITBAG_TRADE: g_pGameApp->SysInfo("%s", SafeVFormat(GetLanguageString(285), item->szName, nMarginNum).c_str());
					break;
				case enumSYN_KITBAG_FORGES: g_pGameApp->SysInfo("%s", SafeVFormat(GetLanguageString(286), item->szName, nMarginNum).c_str());
					break;
				case enumSYN_KITBAG_FORGEF: g_pGameApp->SysInfo("%s", SafeVFormat(GetLanguageString(287), item->szName, nMarginNum).c_str());
					break;
				}
			}
		}
		else {
			// When deleting items, if it is a shortcut, update the shortcut bar
			if (chType != enumSYN_KITBAG_INIT && grd == g_stUIEquip.GetGoodsGrid()) {
				CCommandObj* pObj = grd->GetItem(pGrid[i].sGridID);

				if (chType == enumSYN_KITBAG_EQUIP) {
					CFastCommand::FindFastCommandIndexes(pObj, fastCommandSlot);
				}


				g_stUIEquip.DelFastCommand(pObj);
			}

			if (!grd->DelItem(pGrid[i].sGridID)) {
				switch (chType) {
				case enumSYN_KITBAG_INIT:
				case enumSYN_KITBAG_SWITCH:
					break;
				default:
					g_logManager.InternalLog(LogLevel::Debug, "network", SafeVFormat(GetLanguageString(288), pGrid[i].sGridID));
				}
				continue;
			}
		}
	}

	// Post-sync processing
	switch (chType) {
	case enumSYN_KITBAG_INIT:
		// If it is a bank bag, show bank window
		if (SKitbag.chBagType == 1) {
			g_stUIBank.ShowBank();
		}
		break;
	case enumSYN_KITBAG_ATTR: // Attribute update
		break;
	case enumSYN_KITBAG_PICK:
	case enumSYN_KITBAG_THROW:
		g_pGameApp->PlaySound(31);
		break;
	case enumSYN_KITBAG_SWITCH:
		g_pGameApp->PlaySound(22);
		if (grd == g_stUIEquip.GetGoodsGrid()) {
			g_stUIEquip.RefreshServerShortCut();
		}
		break;


	case enumSYN_KITBAG_EQUIP: {
		if (fastItemSlot[0] != -1) {
			CItemCommand* pObj1 = dynamic_cast<CItemCommand*>(grd->GetItem(fastItemSlot[0]));
			CItemCommand* pObj2 = NULL;
			if (fastItemSlot[1] != -1) {
				pObj2 = dynamic_cast<CItemCommand*>(grd->GetItem(fastItemSlot[1]));
			}
			for (int i = 0; i < 36; i++) {
				if (fastCommandSlot[i] == -1) {
					break;
				}

				g_stUIEquip.FastChange(fastCommandSlot[i], fastItemSlot[0], defItemShortCutType,true);

				if (pObj2) {
					CFastCommand* pFast = CFastCommand::GetFastCommand(fastCommandSlot[i]);
					if (pFast) {
						pFast->AddCommand2(pObj2);
					}
				}
			}
		}
		break;
	}


	default:
		g_pGameApp->PlaySound(22);
	}
}

// Sync kitbag capacity
void NetKitbagCapacity(unsigned int nID, short sKbCap) {
	CGoodsGrid* grd = g_stUIBoat.FindGoodsGrid(nID);
	if (!grd) return;

	int col = grd->GetCol();
	int row = sKbCap / col;
	if (sKbCap % col) row++;


	NetKitbagLockedSpaces(sKbCap, grd);
}

void NetKitbagLockedSpaces(short slots, CGoodsGrid* grd) {
	int lockedID = 32767;
	CItemRecord* blankItem = GetItemRecordInfo(lockedID);
	//CItemCommand* pObj = new CItemCommand( blankItem );

	//CBoat* pBoat = g_stUIBoat.FindBoat( _ItemData.GetDBParam(enumITEMDBP_INST_ID) );
	if (grd != g_stUIEquip.GetGoodsGrid()) {
		return;
	}
	/*
	else{
		grd->SetContent( 4,8 );
	}*/

	grd->SetContent(8, 6);

	grd->Init();
	grd->Refresh();

	for (int index = 0; index < slots; index++) {
		CItemCommand* item = dynamic_cast<CItemCommand*>(grd->GetItem(index));
		if (item && item->GetItemInfo() == blankItem) {
			grd->DelItem(index);
		}
	}

	for (int index = slots; index < 48; index++) {
		CItemCommand* pObj = dynamic_cast<CItemCommand*>(grd->GetItem(index));
		pObj = new CItemCommand(blankItem);
		pObj->SetCanDrag(false);
		grd->SetItem(index, pObj);
	}
}

void NetEspeItem(unsigned int nID, stNetEspeItem& SEspeItem) {
	CGoodsGrid* grd = g_stUIBoat.FindGoodsGrid(nID);
	if (!grd) return;

	CItemCommand* pItem = NULL;
	for (int i = 0; i < 1; i++) {
		pItem = dynamic_cast<CItemCommand*>(grd->GetItem(SEspeItem.SContent[i].sPos));
		if (pItem) {
			SItemGrid& Grid = pItem->GetData();
			Grid.sEndure[0] = SEspeItem.SContent[i].sEndure;
			Grid.sEnergy[0] = SEspeItem.SContent[i].sEnergy;
			Grid.bItemTradable = SEspeItem.SContent[i].bItemTradable;
			Grid.expiration = SEspeItem.SContent[i].expiration;
		}
	}
}

void NetNpcStateChange(DWORD dwChaID, BYTE byState) {
	CCharacter* pCha = GetCharacter(dwChaID, "NetNpcStateChange");
	if (!pCha) return;

	pCha->setNpcState(byState);
}

void NetEntityStateChange(DWORD dwEntityID, BYTE byState) {
	CCharacter* pCha = GetCharacter(dwEntityID, "NetEntityStateChange");
	if (!pCha) return;

	CEvent* pEvent = pCha->getEvent();
	if (pEvent) {
		pEvent->SetIsEnabled(byState == mission::ENTITY_ENABLE ? true : false);
	}
}

void NetShortCut(DWORD dwChaID, stNetShortCut& stShortCut) {
	g_stUIEquip.UpdateShortCut(stShortCut);
}

void NetTriggerAction(stNetNpcMission& info) {
	char szData[64] = {0};
	strncpy_s(szData, sizeof(szData), GetLanguageString(2).c_str(), _TRUNCATE);

	switch (info.byType) {
	case mission::TE_KILL: {
		CMissionTrigger* pMission = new CMissionTrigger;
		pMission->SetData(info);

		CCharacter* pTarget = NULL;
		CCharacter* pMain = CGameScene::GetMainCha();
		if (pMain) {
			CWaitAttackState* pState = dynamic_cast<CWaitAttackState*>(pMain->GetActor()->GetCurState());
			if (pState) {
				pTarget = pState->GetTarget();
			}
		}

		if (pTarget && pTarget->IsEnabled()) {
			pTarget->GetActor()->AddDieExec(pMission);
		}
		else {
			pMission->Exec();
			delete pMission;
		}
	}
	break;
	case mission::TE_GET_ITEM: {
		CItemRecord* pItem = GetItemRecordInfo(info.sID);
		if (pItem) {
			strncpy(szData, pItem->szName.c_str(), sizeof(szData));
		}
		g_pGameApp->ShowMidText("%s", SafeVFormat(GetLanguageString(289), szData, info.sCount, info.sNum).c_str());
	}
	break;
	case mission::TE_GAME_TIME:
	case mission::TE_CHAT:
	case mission::TE_EQUIP_ITEM:
	case mission::TE_GOTO_MAP:
	case mission::TE_LEVEL_UP:
	case mission::TE_MAP_INIT: {
	}
	break;
	default: {
		g_pGameApp->ShowMidText("%s", SafeVFormat(GetLanguageString(290), info.sID, info.sCount, info.sNum).c_str());
	}
	break;
	}
}

void NetShowForge() {
	g_stUIForge.ShowForge();
}

void NetShowUnite() {
	g_stUIMakeEquip.SetType(CMakeEquipMgr::MAKE_EQUIP_TYPE);
	g_stUIMakeEquip.ShowMakeEquipForm(true);
}

void NetShowFusion() {
	g_stUIMakeEquip.SetType(CMakeEquipMgr::EQUIP_FUSION_TYPE);
	g_stUIMakeEquip.ShowMakeEquipForm(true);
}

void NetShowUpgrade() {
	g_stUIMakeEquip.SetType(CMakeEquipMgr::EQUIP_UPGRADE_TYPE);
	g_stUIMakeEquip.ShowMakeEquipForm(true);
}

void NetShowMilling() {
	g_stUIForge.ShowForge(true, true);
}

void NetShowEidolonMetempsychosis() {
	g_stUIMakeEquip.SetType(CMakeEquipMgr::ELF_SHIFT_TYPE);
	g_stUIMakeEquip.ShowMakeEquipForm();
}

void NetShowEidolonFusion() {
	g_stUISpirit.ShowMarryForm();
}

void NetShowPurify() {
	g_stUIPurify.ShowForm(CPurifyMgr::PURIFY_TYPE);
}

void NetShowEnergy() {
	g_stUIPurify.ShowForm(CPurifyMgr::ENERGY_TYPE);
}

void NetShowGetStone() {
	g_stUIPurify.ShowForm(CPurifyMgr::GETSTONE_TYPE);
}

void NetShowRepairOven() {
	g_stUIPurify.ShowForm(CPurifyMgr::REPAIR_OVEN_TYPE);
}

void NetShowTiger() {
	g_stUISpirit.ShowErnieForm();
}

void NetSynSkillBag(DWORD dwCharID, stNetSkillBag* pSSkillBag) {
#ifdef _TEST_CLIENT
	return;
#endif

	g_stUIEquip.SynSkillBag(dwCharID, pSSkillBag);
}

void NetAreaStateBeginSee(stNetAreaState* pState) {
	if (!CGameApp::GetCurScene()) return;

	MPTerrain* pTerrain = CGameApp::GetCurScene()->GetTerrain();
	if (!pTerrain) return;

	int nMapWidth = pTerrain->GetWidth();
	int x = pState->sAreaX;
	int y = pState->sAreaY;
	long lAreaID = y * nMapWidth + x;

	x = x * 200 + 50;
	y = y * 200 + 50;

	int nCount = pState->chStateNum;
	if (nCount == 0) {
		CGameApp::GetCurScene()->DelAreaEff(lAreaID);
	}
	else {
		stAreaSkillState* p = NULL;
		CSkillStateRecord* pInfo = NULL;
		CEffectObj* pEffect = NULL;

		p = pState->State;
		for (int i = 0; i < nCount; i++) {
			pInfo = GetCSkillStateRecordInfo(p[i].chID);
			if (pInfo && pInfo->sAreaEffect > 0) {
				pEffect = CGameApp::GetCurScene()->CreateEffect(pInfo->sAreaEffect, x, y, true);
				if (pEffect) {
					pEffect->setTag(lAreaID);

					CGameApp::GetCurScene()->AddAreaEff(pEffect);
				}
			}
		}
	}
}

void NetAreaStateEndSee(stNetAreaState* pState) {
	if (!CGameApp::GetCurScene()) return;

	MPTerrain* pTerrain = CGameApp::GetCurScene()->GetTerrain();
	if (!pTerrain) return;

	int nMapWidth = pTerrain->GetWidth();

	int nCount = pState->chStateNum;
	long nAreaID = pState->sAreaY * nMapWidth + pState->sAreaX;
	if (nCount == 0) {
		CGameApp::GetCurScene()->DelAreaEff(nAreaID);
	}
	else {
		stAreaSkillState* p = NULL;
		CSkillStateRecord* pInfo = NULL;

		p = pState->State;
		for (int i = 0; i < nCount; i++) {
			pInfo = GetCSkillStateRecordInfo(p[i].chID);
			if (pInfo && pInfo->sAreaEffect > 0) {
				CGameApp::GetCurScene()->DelAreaEff(nAreaID, pInfo->sAreaEffect);
			}
		}
	}
}

void NetFailedAction(char chState) {
	CCharacter* pCha = CGameScene::GetMainCha();
	if (pCha) {
		pCha->GetActor()->FailedAction();
		g_logManager.InternalLog(LogLevel::Debug, "common", std::format("FaliedAction[{}]", static_cast<int>(chState)));
	}
	else {
		g_logManager.InternalLog(LogLevel::Debug, "network", GetLanguageString(291));
	}
}

void NetShowMessage(long lMes) {
	CNotifyInfo* pInfo = GetNotifyInfo(lMes);
	if (!pInfo) return;

	switch (pInfo->chType) {
	case 0:
		g_pGameApp->SysInfo(pInfo->szInfo);
		break;
	case 1:
		g_pGameApp->ShowMidText(pInfo->szInfo);
		break;
	case 2:
		g_pGameApp->ShowBigText(pInfo->szInfo);
		break;
	default:
		g_pGameApp->MsgBox(pInfo->szInfo);
	}
}

void NetChaTLeaderID(long lID, long lLeaderID) {
	CCharacter* pCha = GetCharacter(lID);
	if (pCha) {
		pCha->SetTeamLeaderID(lLeaderID);
	}
}

void NetChaEmotion(long lID, short sEmotion) {
	CCharacter* pCha = GetCharacter(lID, "NetChaEmotion");
	if (pCha && pCha->IsEnabled()) {
		pCha->GetHeadSay()->SetFaceID(sEmotion);
	}
}

void stNetPKCtrl::Exec(CCharacter* pCha) {
	CBoolSet& set = pCha->GetPK();
	set.Set(enumChaPkSelf, bInPK);
	set.Set(enumChaPkScene, bInGymkhana);
	set.Set(enumChaPkGuild, pkGuild);
}

void stNetPKCtrl::Exec(unsigned long ulWorldID) {
	CCharacter* pCha = GetCharacter(ulWorldID, "stNetPKCtrl");
	if (pCha) Exec(pCha);
}

void stNetDefaultSkill::Exec(void) {
	// log
	g_logManager.InternalLog(LogLevel::Debug, "common", SafeVFormat(GetLanguageString(292), GetTickCount()));
	g_logManager.InternalLog(LogLevel::Debug, "common", std::format("Skill ID: {}", sSkillID));
	//

	CSkillRecord* pSkill = GetSkillRecordInfo(sSkillID);
	if (!pSkill) {
		g_logManager.InternalLog(LogLevel::Debug, "network", SafeVFormat(GetLanguageString(293), sSkillID));
		return;
	}

	CCharacter::SetDefaultSkill(pSkill);
}

void stNetUpdateHairRes::Exec() {
	CCharacter* pCha = GetCharacter(ulWorldID, "stNetUpdateHairRes");
	if (!pCha) return;

	if (_stricmp(szReason.c_str(), "ok") == 0) {
		pCha->SelfEffect(334);
		pCha->PlayPose(POSE_JUMP);
	}
	else if (_stricmp(szReason.c_str(), "fail") == 0) {
		pCha->SelfEffect(335);
		pCha->PlayPose(POSE_CRY);
	}
	else {
		g_pGameApp->MsgBox(szReason.c_str());
	}
}

void stNetOpenHair::Exec() {
	g_stUIHaircut.ShowHaircutForm();
}

CEvent* stNetEvent::Exec(CSceneNode* pNode) {
	CEvent* pEvent = CGameApp::GetCurScene()->GetEventMgr()->CreateEvent(usEventID);
	if (pEvent) {
		pEvent->SetNode(pNode);
		pEvent->SetIsValid(true);
		pEvent->SetName(cszEventName.c_str());

		CEventRecord* pInfo = pEvent->GetInfo();
		if (pInfo->sBornEffect > 0) {
			CEffectObj* pEffect = CGameApp::GetCurScene()->CreateEffect(pInfo->sBornEffect, pNode->GetCurX(),
																		pNode->GetCurY(), true);
			if (pEffect) {
				pNode->AddEffect(pEffect->getID());
			}
		}

		pNode->setEvent(pEvent);
	}
	return pEvent;
}

CEvent* stNetEvent::ChangeEvent() {
	CEvent* pEvent = CGameApp::GetCurScene()->GetEventMgr()->Search(lEntityID);
	if (!pEvent) return NULL;

	pEvent->SetName(cszEventName.c_str());
	return pEvent;
}

void NetChaSideInfo(long lID, stNetChaSideInfo& SNetSideInfo) {
	CCharacter* pCha = GetCharacter(lID, "NetChaSideInfo");
	if (!pCha) return;

	pCha->setSideID(SNetSideInfo.chSideID);
}

void stNetTeamFightAsk::Exec() {
	CCharacter* pMain = CGameScene::GetMainCha();
	CGameScene* pScene = CGameApp::GetCurScene();
	if (pMain && pScene) {
		//MPTerrain* pTerrain = pScene->GetTerrain();
		//if( pTerrain )
		//{
		//	int x = pMain->GetCurX() / 100;
		//	int y = pMain->GetCurY() / 100;
		//	short mask = pTerrain->GetTile(x, y)->sRegion;
		//	if( !(mask & enumAREA_TYPE_FIGHT_ASK) )
		//	{
		// Player not in PK zone
		//		return;
		//	}
		//}

		CMapInfo* pInfo = pScene->GetCurMapInfo();
		if (pInfo && (stricmp(pInfo->DataName.c_str(), "garner") == 0)) {
			int x = pMain->GetCurX() / 100;
			int y = pMain->GetCurY() / 100;
			if (x < 2194 || x > 2239 || y < 2872 || y > 2902)
				return;
		}
	}

	g_stUIPKDialog.SetStartDialogContent(*this);
	g_stUIPKDialog.ShowStartDialog();
	//string sInfo = g_stUIPKDialog.ShowStartDialogDebug(*this);

	//g_stUIStart.AskTeamFight( sInfo.c_str() );
}

void stNetItemRepairAsk::Exec() {
	g_stUIEquip.ShowRepairMsg(cszItemName.c_str(), lRepairMoney);
}

void stSCNetItemForgeAsk::Exec() {
	if (chType == 1)
		g_stUIForge.ShowConfirmDialog(lMoney);
	else if (chType == 2)
		g_stUIMakeEquip.ShowConfirmDialog(lMoney);
	else /*if (chType == 3)*/
		g_stUIMakeEquip.ShowConfirmDialog(lMoney);
}

void stNetItemForgeAnswer::Exec() {
	if (chType == 1 || chType == 3) {
		if (chResult == 0) {
			g_stUIForge.ForgeOther(lChaID);
		}
		else if (chResult == 1)
			g_stUIForge.ForgeSuccess(lChaID);
		else if (chResult == 2)
			g_stUIForge.ForgeFailed(lChaID);
	}
	else if (chType == 2 || chType == 4 || chType == 5) {
		if (chResult == 0) {
			g_stUIMakeEquip.MakeEquipOther(lChaID);
		}
		else if (chResult == 1)
			g_stUIMakeEquip.MakeEquipSuccess(lChaID);
		else if (chResult == 2)
			g_stUIMakeEquip.MakeEquipFailed(lChaID);
	}
}

void stNetAppendLook::Exec(unsigned long ulWorldID) {
	// Currently only the first one
	CCharacter* pCha = GetCharacter(ulWorldID, "stNetAppendLook");
	if (!pCha) return;

	Exec(pCha);
}

void stNetAppendLook::Exec(CCharacter* pCha) {
	if (!pCha) return;

	pCha->SetItemFace(0, sLookID[0]);
	if (bValid[1]) pCha->SetItemFace(1, sLookID[1]);
	else pCha->SetItemFace(1, 0);
}

void NetBeginRepairItem(void) {
	CCharacter* pCha = CGameScene::GetMainCha();
	if (!pCha) return;

	CRepairState* pState = new CRepairState(pCha->GetActor());
	pCha->GetActor()->SwitchState(pState);

	if (g_stUIEquip.GetItemForm()) {
		g_stUIEquip.GetItemForm()->Show();
	}
}

void NetItemUseSuccess(unsigned int nID, short sItemID) {
	CItemRecord* pInfo = GetItemRecordInfo(sItemID);
	if (!pInfo) return;

	if (pInfo->sUseItemEffect[0] <= 0) return;

	CCharacter* pCha = GetCharacter(nID);
	if (!pCha) return;

	int nEffectID = pInfo->sUseItemEffect[0];
	int nDummy = pInfo->sUseItemEffect[1];

	if (g_stUIMap.IsPKSilver() && (200 <= nEffectID && nEffectID <= 205)) return;
	// added by Philip.Wu  2008-01-15 Silver coin zone ignores certain visual effects
	if ((nEffectID >= 361 && nEffectID <= 369) || (nEffectID >= 3354 && nEffectID <= 3359)
		|| (nEffectID >= 564 && nEffectID < 600)) // added by Philip.Wu  2007-12-07  Petrify effects
	{
		CEffectObj* pEffect = CGameApp::GetCurScene()->GetFirstInvalidEffObj();
		if (!pEffect) return;

		if (!pEffect->Create(nEffectID))
			return;

		MPMatrix44 mat;
		if (nDummy >= 0 && pCha->GetObjDummyRunTimeMatrix(&mat, nDummy) >= 0) {
			pEffect->Emission(-1, (D3DXVECTOR3*)&mat._41, NULL);
		}
		else {
			pEffect->Emission(-1, &pCha->GetPos(), NULL);
		}
		pEffect->SetValid(TRUE);
		return;
	}
	pCha->SelfEffect(nEffectID, nDummy);
}

void NetStartExit(DWORD dwExitTime) {
	g_ChaExitOnTime.NetStartExit(dwExitTime);
}

void NetCancelExit() {
	g_ChaExitOnTime.NetCancelExit();
}


void NetKitbagCheckAnswer(bool bLock) {
	if (g_stUIEquip.GetIsLock() && bLock) {
		// Forge failed
		g_pGameApp->MsgBox("%s", GetLanguageString(802).c_str());
	}
	else if (g_stUIEquip.GetIsLock() && !bLock) {
		// Forge succeeded
		g_stUIDoublePwd.CloseAllForm();
	}

	g_stUIEquip.SetIsLock(bLock);
}


void NetChaPlayEffect(unsigned int uiWorldID, int nEffectID) {
	CCharacter* pCha = CGameApp::GetCurScene()->SearchByHumanID(uiWorldID);

	if (pCha) {
		pCha->SelfEffect(nEffectID, -1);
	}
}


void NetChurchChallenge(const stChurchChallenge* pInfo) {
	g_stChurchChallenge.SetChallenge(pInfo);
	g_stChurchChallenge.ShowForm();
}
