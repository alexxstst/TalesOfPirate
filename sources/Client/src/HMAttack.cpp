#include "stdafx.h"
#include "HMAttack.h"
#include "UIGuiData.h"
#include "GameApp.h"
#include "Actor.h"
#include "EffectObj.h"
#include "Character.h"
#include "ChaAttr.h"
#include "streadydie.h"
#include "STAttack.h"
#include "UIGlobalVar.h"
#include "HMManage.h"
#include "SkillStateRecord.h"
#include "UIChat.h"
#include "CommFunc.h"
#include "uistartform.h"
#include "uiteam.h"
#include "cameractrl.h"
#include "uiboatform.h"
#include "uisystemform.h"


//---------------------------------------------------------------------------
// globe fun
//---------------------------------------------------------------------------
const DWORD LEVEL80_EXP = 50;

inline bool IsHarmMain(CCharacter* pTarget, CCharacter* pAttack) {
	if (!pTarget->IsMonster()) {
		if (!(pAttack && pAttack->IsMainCha())) {
			return true;
		}
	}
	return false;
}

//---------------------------------------------------------------------------
// class CAttackEffect
//---------------------------------------------------------------------------
CAttackEffect::CAttackEffect()
	: CStateSynchro(), _isTargetDied(false), _isDoubleAttack(false), _pTarget(NULL), _pAttack(NULL)
	  , _IsMiss(false), _pSkill(NULL), _isBeatBack(false), _pRepSynchro(NULL) {
}

CAttackEffect::~CAttackEffect() {
	if (_pServerHarm) {
		_pServerHarm->DelHarm(this);
	}
	if (_pRepSynchro) {
		_pRepSynchro->SetAttackEffect(NULL);
	}
}

void CAttackEffect::_Exec() {
	if (_pRepSynchro) {
		_pRepSynchro->SetAttackEffect(NULL);
		_pRepSynchro->Exec();
		_pRepSynchro = NULL;
	}

	if (_IsMiss && _pTarget->IsEnabled()) {
		D3DXVECTOR3 VPOS = _pAttack ? _pAttack->GetPos() : _pTarget->GetPos();
		D3DXVECTOR3 VPOS2 = _pTarget->GetPos();

		VPOS.z += 3;
		VPOS2.z += 2;
		CCameraCtrl* pCam = g_pGameApp->GetMainCam();
		D3DXVECTOR3 vdir = pCam->m_vCross;
		vdir = rand() % 2 ? vdir : -vdir;
		D3DXVec3Normalize(&vdir, &vdir);

		DWORD dwDelay = 0;
		CreateEffect(enumMiss, "", VPOS2, VPOS, vdir, IsHarmMain(_pTarget, _pAttack), dwDelay);
	}

	if (_isDoubleAttack) {
		_pTarget->SelfEffect(130, -1);
	}

	if (!_HarmValue.IsEmpty()) {
		ExecHarm(_HarmValue, _pTarget, _pAttack);
	}

	if (!_HarmState.IsEmpty()) {
		_pTarget->SynchroSkillState(_HarmState.GetValue(), _HarmState.GetCount());
	}

	if (_isTargetDied) {
		ChaDied(_pTarget, _pAttack);
	}

	if (_isBeatBack) {
		_pTarget->ForceMove(_nBeatX, _nBeatY);
	}
}

void CAttackEffect::ChaDied(CCharacter* pTarget, CCharacter* pAttack) {
	if (!pTarget->IsEnabled()) return;

	if (pTarget->IsMainCha()) {
		CGameApp::GetCurScene()->GetUseLevel().SetFalse(LEVEL_CHA_RUN);
	}

	// pTarget->getGameAttr()->set( ATTR_HP, 0 );
	// pTarget->RefreshUI();

	// ,EndAction
	pTarget->GetActor()->OverAllState();

	CReadyDieState* st = new CReadyDieState(pTarget->GetActor());
	st->SetState(enumDied);
	st->SetAttack(pAttack);
	pTarget->GetActor()->InsertState(st);
}

void CAttackEffect::ExecHarm(CSizeArray<stEffect>& Value, CCharacter* pTarget, CCharacter* pAttack) {
	D3DXVECTOR3 VPOS = pAttack ? pAttack->GetPos() : pTarget->GetPos();
	D3DXVECTOR3 VPOS2 = pTarget->GetPos();

	VPOS.z += 3;
	VPOS2.z += 2;
	CCameraCtrl* pCam = g_pGameApp->GetMainCam();
	D3DXVECTOR3 vdir = pCam->m_vCross;
	vdir = rand() % 2 ? vdir : -vdir;
	D3DXVec3Normalize(&vdir, &vdir);

	bool isMain = pTarget->IsMainCha();

	char buf[30] = {0};
	int count = Value.GetCount();
	stEffect* p = Value.GetValue();
	SGameAttr* pAttr = pTarget->getGameAttr();

	int type;
	long val = 0;
	DWORD dwDelay = 0;

	static CChaAttrChange Change;
	Change.Reset();
	for (int i = 0; i < count; i++) {
		//char str[256];
		//char v1[32];
		//char v2[32];


		//::OutputDebugStr(str);

		val = p[i].lVal - pAttr->get((short)p[i].lAttrID);

		pAttr->set((short)p[i].lAttrID, p[i].lVal);
		Change.SetChangeBitFlag(p[i].lAttrID);

		switch (p[i].lAttrID) {
		case ATTR_HP: type = val < 0 ? enumSubLife : enumAddLife;
			if (pTarget->IsEnabled()) {
				sprintf(buf, "%d", abs(val));
				CreateEffect(type, buf, VPOS2, VPOS, vdir, IsHarmMain(pTarget, pAttack), dwDelay);
			}
			break;
		//case ATTR_SP: type = val<0 ? enumSubSp : enumAddSp;
		//          CreateEffect( type, buf, VPOS2, VPOS, vdir, isMain, dwDelay );
		//          break;
		case ATTR_CEXP:
			if (isMain) {
				if (pTarget->IsBoat()) {
					g_pGameApp->SysInfo("%s", SafeVFormat(GetLanguageString(144), val).c_str());
				}
				else if (val > 0) {
					//g_pGameApp->SysInfo( "%s", SafeVFormat(GetLanguageString(145), val).c_str() );

					// 80BUG
					DWORD dwLastExp = g_stUIBoat.GetHuman()->getGameAttr()->get(ATTR_CEXP);
					DWORD dwLv80Exp = 2425349810; // 
					if (dwLastExp < dwLv80Exp && dwLv80Exp <= (DWORD)p[i].lVal) {
						DWORD dwObtainExp;
						dwObtainExp = ((DWORD)p[i].lVal - dwLv80Exp) * LEVEL80_EXP; // 80 
						dwObtainExp += (dwLv80Exp - dwLastExp); // 79 

						g_pGameApp->SysInfo("%s", SafeVFormat(GetLanguageString(145), dwObtainExp).c_str());
					}
					else if (pTarget->getGameAttr()->get(ATTR_LV) >= 80) {
						g_pGameApp->SysInfo("%s", SafeVFormat(GetLanguageString(145), val * LEVEL80_EXP).c_str());
					}
					else {
						g_pGameApp->SysInfo("%s", SafeVFormat(GetLanguageString(145), val).c_str());
					}
				}
			}
			break;
		}
	}

	// pose
	if (Change.GetChangeBitFlag(ATTR_HP)) {
		pTarget->RefreshFog();

		if (pTarget->getChaCtrlType() == enumCHACTRL_MONS_MINE) {
			float f = (float)pAttr->get(ATTR_HP) / (float)pAttr->get(ATTR_MXHP);

			if (f > 0.67f)
				pTarget->PlayPose(1, PLAY_ONCE_SMOOTH);
			else if (f >= 0.33f && f <= 0.67f)
				pTarget->PlayPose(10, PLAY_ONCE_SMOOTH);
			else
				pTarget->PlayPose(11, PLAY_ONCE_SMOOTH);
		}
	}

	if (Change.GetChangeBitFlag(ATTR_CEXP) && pTarget == g_stUIBoat.GetHuman()) {
		g_stUIStart.RefreshMainExperience(pAttr->get(ATTR_CEXP), pAttr->get(ATTR_CLEXP), pAttr->get(ATTR_NLEXP));
	}

	if (Change.GetChangeBitFlag(ATTR_LV)) {
		if (isMain) CGameApp::GetCurScene()->RefreshLevel();

		if (!pTarget->IsBoat()) {
			if (isMain) {
				g_pGameApp->PlaySound(21);
				g_pGameApp->ShowBigText(
					"%s", SafeVFormat(GetLanguageString(146), pTarget->getGameAttr()->get(ATTR_LV)).c_str());
				pTarget->SelfEffect(132, -1);
			}
			else {
				pTarget->SelfEffect(132, -1);
				g_pGameApp->PlaySound(21);
			}
		}
	}

	if (Change.GetChangeBitFlag(ATTR_SAILLV) && pTarget->getGameAttr()->get(ATTR_CSAILEXP) > 0) {
		if (isMain) {
			g_pGameApp->PlaySound(21);
			g_pGameApp->ShowBigText("reborn level %d", pTarget->getGameAttr()->get(ATTR_SAILLV));
			pTarget->SelfEffect(132, -1);
		}
		else {
			//pTarget->SelfEffect( 132, -1 );
		}
	}

	pTarget->RefreshUI();
}

void CAttackEffect::CreateEffect(int eType, const char* str, D3DXVECTOR3& start, D3DXVECTOR3& target, D3DXVECTOR3& dir,
								 bool isMain, DWORD& dwDelay) {
	CEffectObj* pEffect = CGameApp::GetCurScene()->GetFirstInvalidEffObj();
	if (pEffect == NULL) {
		g_logManager.InternalLog(LogLevel::Error, "errors", GetLanguageString(147));
		return;
	}

	if (!pEffect->Create(99)) return;

	VEC_string strs;
	strs.push_back(str);
	if (strs.begin()->size() > 15) {
		strs.begin()->resize(15);
	}
	pEffect->SetFontEffectCom(strs, 1, &ResMgr, &dir, eType, 0xffffffff, false, isMain);
	pEffect->Emission(-1, &start, &target);
	pEffect->SetDailTime(dwDelay);
	pEffect->SetValid(TRUE);

	dwDelay += 100;
	start.z += 0.5f;
	target.z += 0.5f;
}

CStateSynchro* CAttackEffect::Gouge(float fRate) {
	if (!_IsMiss) {
		int count = _HarmValue.GetCount();
		if (count <= 0) return NULL;

		stEffect* pOld = _HarmValue.GetValue();

		bool isFindHp = false;
		long hp = 0;
		for (int i = 0; i < count; i++) {
			if (ATTR_HP == pOld[i].lAttrID) {
				isFindHp = true;

				SGameAttr* pAttr = _pTarget->getGameAttr();
				long val = pAttr->get((short)pOld[i].lAttrID);
				hp = (pOld[i].lVal - val) * fRate + val;
			}
		}

		if (!isFindHp) return NULL;

		CAttackEffect* pAttack = new CAttackEffect;
		pAttack->_pSkill = _pSkill;
		pAttack->_pTarget = _pTarget;
		pAttack->_pAttack = _pAttack;
		pAttack->_HarmValue.Resize(1);

		// 
		stEffect* pNew = pAttack->_HarmValue.GetValue();
		pNew[0].lAttrID = ATTR_HP;
		pNew[0].lVal = hp;
		return pAttack;
	}
	return NULL;
}

//---------------------------------------------------------------------------
// class CAttackEffect
//---------------------------------------------------------------------------
CAttackRepSynchro::CAttackRepSynchro()
	: CStateSynchro(), _pTarget(NULL), _pSkill(NULL), _pAttack(NULL), _pAttackEffect(NULL), _IsAttackDied(false) {
}

CAttackRepSynchro::~CAttackRepSynchro() {
	if (_pAttackEffect) {
		_pAttackEffect->SetAttackRep(NULL);
	}
}

void CAttackRepSynchro::_Exec() {
	// 
	if (_pAttack) {
		if (!_RepState.IsEmpty()) {
			_pAttack->SynchroSkillState(_RepState.GetValue(), _RepState.GetCount());
		}
		if (!_RepValue.IsEmpty()) {
			CAttackEffect::ExecHarm(_RepValue, _pAttack, _pTarget);
		}
		if (_IsAttackDied) {
			CAttackEffect::ChaDied(_pAttack, _pTarget);
		}
	}
}

//---------------------------------------------------------------------------
// class CAttribSynchro
//---------------------------------------------------------------------------
CAttribSynchro::CAttribSynchro()
	: CStateSynchro(), _nType(0), _IsAlreadyExec(false), _pCha(NULL) {
}

CAttribSynchro::~CAttribSynchro() {
}

void CAttribSynchro::Start() {
	Exec();
	if (enumATTRSYN_INIT == _nType) {
		_Exec();
	}
}

void CAttribSynchro::_Exec() {
	if (_IsAlreadyExec) return;
	_IsAlreadyExec = true;

	CCharacter* pCha = _pCha;
	if (!pCha) return;

	if (enumATTRSYN_SKILL_STATE == _nType) {
		// 
		if (!_Value.IsEmpty()) CAttackEffect::ExecHarm(_Value, pCha);

		if (pCha->getGameAttr()->get(ATTR_HP) <= 0)
			CAttackEffect::ChaDied(pCha);
		return;
	}

	int count = _Value.GetCount();
	stEffect* p = _Value.GetValue();
	SGameAttr* pAttr = pCha->getGameAttr();

	static CChaAttrChange Change;
	Change.Reset();

	bool isMain = pCha == g_stUIBoat.GetHuman();
	static long val = 0;
	if (isMain && enumATTRSYN_INIT != _nType) {
		for (int i = 0; i < count; i++) {
			if (p[i].lAttrID == ATTR_CEXP) {
				val = p[i].lVal - pAttr->get((short)p[i].lAttrID);

				if (pCha->IsBoat()) {
					g_pGameApp->SysInfo("%s", SafeVFormat(GetLanguageString(144), val).c_str());
				}
				else if (val > 0) {
					//g_pGameApp->SysInfo( "%s", SafeVFormat(GetLanguageString(145), val).c_str() );

					// 80BUG
					DWORD dwLastExp = g_stUIBoat.GetHuman()->getGameAttr()->get(ATTR_CEXP);
					DWORD dwLv80Exp = 2425349810; // 
					if (dwLastExp < dwLv80Exp && dwLv80Exp <= (DWORD)p[i].lVal) {
						DWORD dwObtainExp;
						dwObtainExp = ((DWORD)p[i].lVal - dwLv80Exp) * LEVEL80_EXP; // 80 
						dwObtainExp += (dwLv80Exp - dwLastExp); // 79 

						g_pGameApp->SysInfo("%s", SafeVFormat(GetLanguageString(145), dwObtainExp).c_str());
					}
					else if (g_stUIBoat.GetHuman()->getGameAttr()->get(ATTR_LV) >= 80) {
						g_pGameApp->SysInfo("%s", SafeVFormat(GetLanguageString(145), val * LEVEL80_EXP).c_str());
					}
					else {
						g_pGameApp->SysInfo("%s", SafeVFormat(GetLanguageString(145), val).c_str());
					}
				}
			}

			pAttr->set((short)p[i].lAttrID, p[i].lVal);
			Change.SetChangeBitFlag(p[i].lAttrID);
		}
	}
	else {
		for (int i = 0; i < count; i++) {
			pAttr->set((short)p[i].lAttrID, p[i].lVal);
			Change.SetChangeBitFlag(p[i].lAttrID);
		}

		if (g_stUIBoat.GetHuman()) {
			pCha->RefreshLevel(g_stUIBoat.GetHuman()->getGameAttr()->get(ATTR_LV));
		}
	}
	pCha->RefreshUI();

	if (pCha->getChaCtrlType() == enumCHACTRL_PLAYER && Change.GetChangeBitFlag(ATTR_JOB)) {
		CTeam* pTeam = g_stUIChat.GetTeamMgr()->Find(enumTeamRoad);
		if (pTeam) {
			CMember* pMember = pTeam->Find(pCha->getAttachID());
			if (pMember) {
				pMember->SetName(pCha->getHumanName());
				pMember->SetJobName(g_GetJobName((short)pCha->getGameAttr()->get(ATTR_JOB)));
			}
		}
	}

	if (enumATTRSYN_INIT != _nType && pCha->getGameAttr()->get(ATTR_HP) <= 0) {
		CAttackEffect::ChaDied(pCha);
	}

	// 3D,
	if (Change.GetChangeBitFlag(ATTR_MSPD) && pCha == CGameScene::GetMainCha()) {
		g_pGameApp->GetMainCam()->SetBufVel(pCha->getMoveSpeed(), pCha->getID());
	}

	if (isMain) {
		g_stUIStart.RefreshMainLifeNum(pCha->getHP(), pCha->getHPMax());
		g_stUIStart.RefreshMainSP(pCha->getGameAttr()->get(ATTR_SP), pCha->getGameAttr()->get(ATTR_MXSP));

		if (_nType == enumATTRSYN_ITEM_MEDICINE) {
			// 
			if (Change.GetChangeBitFlag(ATTR_HP) || Change.GetChangeBitFlag(ATTR_SP)) {
				pCha->SelfEffect(133);
				g_pGameApp->PlaySound(23);
			}
		}
	}

	if (Change.GetChangeBitFlag(ATTR_LV)) {
		if (isMain) CGameApp::GetCurScene()->RefreshLevel();

		if (!pCha->IsBoat()) {
			if (pCha->IsMainCha()) {
				if (enumATTRSYN_INIT != _nType) {
					// 
					g_pGameApp->PlaySound(21);
					g_pGameApp->ShowBigText(
						"%s", SafeVFormat(GetLanguageString(146), pCha->getGameAttr()->get(ATTR_LV)).c_str());

					if (g_stUISystem.m_sysProp.m_gameOption.bHelpMode && pCha->getGameAttr()->get(ATTR_LV) <= 50)
					//	Modify by alfred.shi 20080905
					{
						// 
						g_stUIStart.ShowLevelUpHelpButton(true);
					}

					pCha->SelfEffect(132, -1);
				}
			}
			else {
				if (enumATTRSYN_INIT != _nType) {
					pCha->SelfEffect(132, -1);
				}
			}
		}
	}

	if (Change.GetChangeBitFlag(ATTR_SAILLV) && pCha->getGameAttr()->get(ATTR_CSAILEXP) > 0) {
		if (!pCha->IsBoat()) {
			if (isMain) {
				if (enumATTRSYN_INIT != _nType) {
					g_pGameApp->PlaySound(21);
					//g_pGameApp->ShowBigText( "%d", pCha->getGameAttr()->get(ATTR_SAILLV) );
					g_pGameApp->ShowBigText("reborn level %d", pCha->getGameAttr()->get(ATTR_SAILLV));
					pCha->SelfEffect(132, -1);
				}
			}
		}
		else {
			if (enumATTRSYN_INIT != _nType) {
				pCha->SelfEffect(132, -1);
			}
		}
	}

	pCha->RefreshFog();

	if (pCha->getChaCtrlType() == enumCHACTRL_MONS_MINE) {
		// 
		float f = (float)pCha->getGameAttr()->get(ATTR_HP) / (float)pCha->getGameAttr()->get(ATTR_MXHP);
		if (f > 0.67f)
			pCha->PlayPose(1, PLAY_ONCE_SMOOTH);
		else if (f >= 0.33f && f <= 0.67f)
			pCha->PlayPose(10, PLAY_ONCE_SMOOTH);
		else
			pCha->PlayPose(11, PLAY_ONCE_SMOOTH);
	}
}

//---------------------------------------------------------------------------
// class CSkillStateSynchro
//---------------------------------------------------------------------------
CSkillStateSynchro::CSkillStateSynchro()
	: CStateSynchro(), _chType(0), _pCha(NULL) {
}

CSkillStateSynchro::~CSkillStateSynchro() {
}

void CSkillStateSynchro::_Exec() {
	if (_pCha) {
		_pCha->SynchroSkillState(_SState.GetValue(), _SState.GetCount());
	}
}
