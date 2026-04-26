#include "stdafx.h"
#include "MouseDown.h"
#include "SceneItem.h"
#include "WorldScene.h"
#include "GameApp.h"
#include "Character.h"
#include "uiformmgr.h"
#include "mpeditor.h"
#include "skillrecord.h"

#include "stmove.h"
#include "stattack.h"
#include "netif.h"
#include "uicommand.h"
#include "stnpctalk.h"
#include "event.h"
#include "eventrecord.h"
#include "stseat.h"
#include "stpose.h"
#include "uiCozeform.h"
#include "autoattack.h"
#include "isskilluse.h"
#include "uiminimapform.h"
#include "findpath.h"
#include "GameConfig.h"

#include "uistartform.h"


extern MPEditor g_Editor;

static D3DXVECTOR3 vPos;
static int nScrX, nScrY;

const DWORD DELAY_TIME = 100;
int MOVE_LENGTH = -1;
int ATTACT_DIS = 200;

CMouseDown::CMouseDown() {
	_pAutoAttack = new CAutoAttack(this);

	Reset();
}

CMouseDown::~CMouseDown() {
	delete _pAutoAttack;
}

void CMouseDown::Reset() {
	_pAutoAttack->Reset();

	_eLastType = eLastNone;
	_pAttackSkill = NULL;
	_pAttackCha = NULL;
	_pPickItem = NULL;
	_pEvent = NULL;
	_pEventNode = NULL;
	_pNpcCha = NULL;
	_pAreaSkill = NULL;
	_pFollow = NULL;

	_dwAttackTime = 0;
	_dwPickTime = 0;
	_IsEnabled = true;
}

bool CMouseDown::_AttackCha(CCharacter* pMain, CSkillRecord* pSkill, CCharacter* pTarget) {
	if (!_IsEnabled) return false;

	if (!pSkill) return false;

	if (pSkill->IsAutoAttack() && g_pGameApp->IsShiftPress()) {
		if (_pAutoAttack->AttackStart(pMain, pSkill, pTarget))
			return true;
	}

	bool IsForce = false;
	if (g_pGameApp->IsCtrlPress() && pMain != pTarget) {
		if (!pSkill->IsAttackTime(CGameApp::GetCurTick()))
			return false;

		IsForce = true;
	}

	if (!IsForce && !g_SkillUse.IsUse(pSkill, pMain, pTarget)) {
		return false;
	}

	return ActAttackCha(pMain, pSkill, pTarget, true, IsForce);
}

bool CMouseDown::_AttackArea(CCharacter* pMain, CSkillRecord* pSkill, int nScrX, int nScrY) {
	if (!_IsEnabled) return false;

	if (!pSkill) return false;

	if (pSkill->IsAutoAttack() && g_pGameApp->IsShiftPress()) {
		if (_pAutoAttack->AttackStart(pMain, pSkill, nScrX, nScrY))
			return true;
	}

	bool IsForce = false;
	if (g_pGameApp->IsCtrlPress()) {
		if (!pSkill->IsAttackTime(CGameApp::GetCurTick()))
			return false;

		IsForce = true;
	}

	if (!IsForce && !g_SkillUse.IsUse(pSkill, pMain, NULL)) {
		return false;
	}

	return ActAttackArea(pMain, pSkill, nScrX, nScrY, IsForce);
}

bool CMouseDown::PickItem(CCharacter* pMain) {
	if (!_IsEnabled) return false;

	int nCount = _pScene->PickItem();
	if (nCount > 0) {
		_pAutoAttack->Cancel();
		g_pGameApp->SysInfo("%s", SafeVFormat(GetLanguageString(195), nCount).c_str());
		return true;
	}
	return false;
}

void CMouseDown::MouseButtonDown() {
	CCharacter* pMain = CGameScene::GetMainCha();
	if (!pMain) return;

	if (!_IsEnabled) return;

	_pScene->GetSign()->Hide();

	_pScene->GetPickPos(g_pGameApp->GetMouseX(), g_pGameApp->GetMouseY(), vPos);
	nScrX = (int)(vPos.x * 100.0f);
	nScrY = (int)(vPos.y * 100.0f);

	CInsertState* seat = dynamic_cast<CInsertState*>(pMain->GetActor()->GetCurState());
	if (seat && !seat->GetIsCancel()) {
		seat->SetFaceTo(nScrX, nScrY);
		seat->Start();
		return;
	}

	CIsStart start(_pAutoAttack);
	if (g_pGameApp->IsCtrlPress()) {
		CSkillRecord* pSkill = pMain->GetReadySkillInfo();
		if (!pSkill) pSkill = pMain->GetDefaultSkillInfo();

		if (pSkill && pSkill->IsAttackArea()) {
			_AttackArea(pMain, pSkill, nScrX, nScrY);
			return;
		}
	}

	switch (_eLastType) {
	case eLastAttack:
		_AttackCha(pMain, _pAttackSkill, _pAttackCha);
		break;
	case eLastPick:
		ActPickItem(pMain, _pPickItem);
		break;
	case eLastEvent:
		ActEvent(pMain, _pEventNode, _pEvent);
		break;
	case eLastNpc:
		ActTalkNpc(pMain, _pNpcCha);
		break;
	case eLastArea:
		_AttackArea(pMain, _pAreaSkill, nScrX, nScrY);
		break;
	case eLastDummy:
		ActDummyObj(pMain, _nDummyX, _nDummyY, _nDummyH, _nDummyAngle, _nDummyAction);
		break;
	case eLastFollow:
		_pAutoAttack->Follow(pMain, _pFollow);
		break;
	default:
		// lemon
		_pScene->GetPickPos(g_pGameApp->GetMouseX(), g_pGameApp->GetMouseY() + 25, vPos);
		int nAreaX = (int)(vPos.x * 100.0f);
		int nAreaY = (int)(vPos.y * 100.0f);

		if (CGameApp::GetCurTick() <= _dwAttackTime + 500) {
			CSkillRecord* pSkill = pMain->GetReadySkillInfo();
			if (!pSkill) pSkill = pMain->GetDefaultSkillInfo();

			if (_pAttackSkill == pSkill && _pAttackCha->IsValid()) {
				if (GetDistance(nAreaX, nAreaY, _pAttackCha->GetCurX(), _pAttackCha->GetCurY()) <= 230 && _AttackCha(
					pMain, _pAttackSkill, _pAttackCha)) {
					return;
				}
			}
		}

		if (CGameApp::GetCurTick() <= _dwPickTime + 500) {
			if (!pMain->GetReadySkillInfo() && _pPickItem->IsValid())
				if (GetDistance(nAreaX, nAreaY, _pPickItem->GetCurX(), _pPickItem->GetCurY()) <= 150)
					if (ActPickItem(pMain, _pPickItem))
						return;
		}

		if (eLastMove == _eLastType) {
			if (!_pAutoAttack->AttackMoveTo(nScrX, nScrY)) {
				ActMove(pMain, nScrX, nScrY, g_pGameApp->IsAltPress() ? true : false);
			}
		}
		break;
	}
}

void CMouseDown::CheckWalkContinue() {
	static DWORD dwAddTime = 0;
	if (CGameApp::GetCurTick() >= dwAddTime) {
		dwAddTime = CGameApp::GetCurTick() + 500;
		CCharacter* pMain = CGameScene::GetMainCha();
		if (_IsEnabled && pMain && !g_Editor.IsEnable() && CGameApp::IsMouseInScene()) {
			CUIInterface::MainChaMove();

			CInsertState* seat = dynamic_cast<CInsertState*>(pMain->GetActor()->GetCurState());
			if (seat) return;

			_pScene->GetSign()->Hide();

			_pScene->GetPickPos(g_pGameApp->GetMouseX(), g_pGameApp->GetMouseY(), vPos);
			nScrX = (int)(vPos.x * 100.0f);
			nScrY = (int)(vPos.y * 100.0f);
			if (pMain->getMoveSpeed() > 0) {
				int len = pMain->getMoveSpeed();
				len += GetDistance(pMain->GetCurX(), pMain->GetCurY(), pMain->GetServerX(), pMain->GetServerY());
				if (GetDistance(pMain->GetCurX(), pMain->GetCurY(), nScrX, nScrY) < len)
					GetDistancePos(pMain->GetCurX(), pMain->GetCurY(), nScrX, nScrY, len, nScrX, nScrY);
			}

			ActMove(CGameScene::GetMainCha(), nScrX, nScrY, false, true);
		}
	}
}

bool CMouseDown::ActMove(CCharacter* pCha, int nScrX, int nScrY, bool isAdd, bool isLine, bool IsCompart) {
	if (!_IsEnabled) return false;

	if (!pCha) return false;

	if (GlobalAppConfig.IsEditor()) {
		pCha->SetServerPos(pCha->GetCurX(), pCha->GetCurY());
	}

	if (!isLine) {
		if (!g_cFindPathEx.HaveTarget())
			_pScene->GetSign()->Show(nScrX, nScrY);
	}

	static CCharacter* pLastCha = NULL;
	static int nLastScrX = 0, nLastScrY = 0;
	static bool isLastAdd = false, isLastLine = false;
	static DWORD dwLastTime = 0;

	if (CGameApp::GetCurTick() <= dwLastTime
		&& pLastCha == pCha
		&& nLastScrX == nScrX && nLastScrY == nScrY
		&& isLastAdd == isAdd && isLastLine == isLine) {
		return true;
	}

	dwLastTime = CGameApp::GetCurTick() + DELAY_TIME;
	pLastCha = pCha;
	nLastScrX = nScrX;
	nLastScrY = nScrY;
	isLastAdd = isAdd;
	isLastLine = isLine;

	if (!GlobalAppConfig.IsMoveClient()) {
		CServerMoveState* move = new CServerMoveState(pCha->GetActor());
		move->SetWalkLine(isLine);
		move->SetMoveTo(nScrX, nScrY);
		return pCha->GetActor()->SwitchState(move);

		// 
		bool isOK = false;
		if (!isLine && MOVE_LENGTH > 0) {
			isOK = g_cFindPath.GetPathFindingState()
				/*g_cFindPath.Find( pCha->GetScene(), pCha, pCha->GetCurX(), pCha->GetCurY(), nScrX, nScrY, isLine )*/;
		}
		if (isOK) {
			CMoveList cLocalList;
			COneMoveState::CompartMoveList(cLocalList, g_cFindPath.GetResultPath(), MOVE_LENGTH);
			POINT& pt = cLocalList.GetBack();
			pt.x = nScrX;
			pt.y = nScrY;

			int nCount = 0;
			while (!cLocalList.IsEmpty()) {
				POINT pt = cLocalList.GetFront();
				cLocalList.PopFront();

				CServerMoveState* move = new CServerMoveState(pCha->GetActor());
				move->SetWalkLine(isLine);
				move->SetMoveTo(pt.x, pt.y);
				if (nCount == 0)
					pCha->GetActor()->SwitchState(move);
				else
					pCha->GetActor()->AddState(move);
				nCount++;
			}
			return true;
		}
		else {
			CServerMoveState* move = new CServerMoveState(pCha->GetActor());
			move->SetWalkLine(isLine);
			move->SetMoveTo(nScrX, nScrY);
			return pCha->GetActor()->SwitchState(move);
		}
	}

	if (IsCompart) {
		COneMoveState* pMoveState = dynamic_cast<COneMoveState*>(pCha->GetActor()->GetCurState());
		if (pMoveState) {
			if (isAdd) {
				return pMoveState->AddPath(nScrX, nScrY, isLine);
			}
			else {
				pCha->GetActor()->ClearQueueState();
				return pMoveState->ContinueMove(nScrX, nScrY, isLine, true);
			}
		}
		else {
			COneMoveState* move = new COneMoveState(pCha->GetActor());
			move->SetWalkLine(isLine);
			move->SetMoveTo(nScrX, nScrY);
			return pCha->GetActor()->SwitchState(move);
		}
	}
	else {
		CServerMoveState* move = new CServerMoveState(pCha->GetActor());
		move->SetWalkLine(isLine);
		move->SetMoveTo(nScrX, nScrY);
		return pCha->GetActor()->SwitchState(move);
	}
}

bool CMouseDown::ActAttackCha(CCharacter* pCha, CSkillRecord* pSkill, CCharacter* pTarget, bool isTrace, bool IsForce,
							  bool IsCompart) {
	if (!_IsEnabled) return false;

	if (!pCha || !pSkill || !pTarget) {
		return false;
	}

	//  by lh
	if (!g_NetIF->IsConnected()) return false;

	static CCharacter* pLastCha = NULL;
	static CCharacter* pLastTarget = NULL;
	static CSkillRecord* pLastSkill = NULL;
	static DWORD dwLastTime = 0;

	g_stUIStart.SetTargetInfo(pTarget);

	if (CGameApp::GetCurTick() <= dwLastTime
		&& pLastCha == pCha && pLastTarget == pTarget
		&& pLastSkill == pSkill) {
		return true;
	}


	ToLogService("common", "actattackcha: self:{}, target:{}, pos[{}, {}], skill:{}\ttick:{}", pCha->getName().c_str(),
				 pTarget->getName().c_str(), pTarget->GetCurX(), pTarget->GetCurY(), pSkill->szName,
				 CGameApp::GetCurTick());

	dwLastTime = CGameApp::GetCurTick() + DELAY_TIME;
	pLastCha = pCha;
	pLastTarget = pTarget;
	pLastSkill = pSkill;

	_pScene->GetSign()->Show(pTarget);
	g_pGameApp->AddTipText(pSkill->szName.c_str());

	//// 
	//if( g_Config.m_IsMoveClient && IsCompart && pSkill->GetDistance() )
	//{
	//	int preMove = CWaitMoveState::GetPreMoveDis();
	//	if( preMove<ATTACT_DIS ) preMove = ATTACT_DIS;
	//	int nTotalDis = pCha->GetDefaultChaInfo()->sRadii + pSkill->GetDistance() + pTarget->GetDefaultChaInfo()->sRadii + preMove + ATTACT_DIS;
	//	bool isLine = false;
	//	if( g_cFindPath.Find( pCha->GetScene(), pTarget, pTarget->GetServerX(), pTarget->GetServerY(), pCha->GetServerX(), pCha->GetServerY(), isLine ) )
	//	{
	//		if( g_cFindPath.GetLength() > nTotalDis + ATTACT_DIS )
	//		{
	//			CMoveList cLocalList;
	//			COneMoveState::CompartMoveList( cLocalList, g_cFindPath.GetResultPath(), nTotalDis );
	//			int x = cLocalList.GetFront().x;
	//			int y = cLocalList.GetFront().y;

	//			pCha->GetActor()->ClearQueueState();
	//			COneMoveState* pMoveState = dynamic_cast<COneMoveState*>( pCha->GetActor()->GetCurState() );
	//			if( pMoveState )
	//			{
	//				pMoveState->ContinueMove( x, y, isLine );
	//			}
	//			else
	//			{
	//				COneMoveState * move = new COneMoveState(pCha->GetActor());
	//				move->SetWalkLine( isLine );
	//				move->SetMoveTo( x, y );
	//				pCha->GetActor()->SwitchState(move);
	//			}

	//			CAttackState *attack = isTrace ? new CTraceAttackState(pCha->GetActor()) : new CAttackState(pCha->GetActor());
	//			attack->SetIsForce( IsForce );
	//			attack->SetSkill( pSkill );
	//			attack->SetTarget( pTarget );
	//			attack->SetCommand( CCommandObj::GetReadyCommand() );
	//			return pCha->GetActor()->AddState(attack);
	//		}
	//	}
	//}

	CAttackState* attack = isTrace ? new CTraceAttackState(pCha->GetActor()) : new CAttackState(pCha->GetActor());
	attack->SetIsForce(IsForce);
	attack->SetSkill(pSkill);
	attack->SetTarget(pTarget);
	attack->SetCommand(CCommandObj::GetReadyCommand());
	return pCha->GetActor()->SwitchState(attack);
}

bool CMouseDown::ActAttackArea(CCharacter* pCha, CSkillRecord* pSkill, int nScrX, int nScrY, bool IsForce,
							   bool IsCompart) {
	if (!_IsEnabled) return false;

	if (!pCha || !pSkill) return false;

	if (!pSkill->IsAttackArea()) {
		g_pGameApp->SysInfo("%s", GetLanguageString(196).c_str());
		return false;
	}

	static CCharacter* pLastCha = NULL;
	static CSkillRecord* pLastSkill = NULL;
	static int nLastScrX = 0, nLastScrY = 0;
	static DWORD dwLastTime = 0;

	if (CGameApp::GetCurTick() <= dwLastTime
		&& pLastCha == pCha
		&& pLastSkill == pSkill
		&& nLastScrX == nScrX && nLastScrY == nScrY) {
		return true;
	}

	ToLogService("common", "actattackarea: self:{}, skill:{}, pos[{}, {}]\ttick:{}", pCha->getName().c_str(),
				 pSkill->szName, nScrX, nScrY, CGameApp::GetCurTick());

	dwLastTime = CGameApp::GetCurTick() + DELAY_TIME;
	pLastCha = pCha;
	pLastSkill = pSkill;
	nLastScrX = nScrX;
	nLastScrY = nScrY;

	//// 
	//if( g_Config.m_IsMoveClient && IsCompart && pSkill->GetDistance() )
	//{
	//	int preMove = CWaitMoveState::GetPreMoveDis();
	//	if( preMove<ATTACT_DIS ) preMove = ATTACT_DIS;
	//	int nTotalDis = pCha->GetDefaultChaInfo()->sRadii + pSkill->GetDistance() + preMove + ATTACT_DIS;
	//	bool isLine = false;
	//	if( g_cFindPath.Find( pCha->GetScene(), pCha, nScrX, nScrY, pCha->GetServerX(), pCha->GetServerY(), isLine ) )
	//	{
	//		if( g_cFindPath.GetLength() > nTotalDis + ATTACT_DIS )
	//		{
	//			CMoveList cLocalList;
	//			COneMoveState::CompartMoveList( cLocalList, g_cFindPath.GetResultPath(), nTotalDis );
	//			int x = cLocalList.GetFront().x;
	//			int y = cLocalList.GetFront().y;

	//			pCha->GetActor()->ClearQueueState();
	//			COneMoveState* pMoveState = dynamic_cast<COneMoveState*>( pCha->GetActor()->GetCurState() );
	//			if( pMoveState )
	//			{
	//				pMoveState->ContinueMove( x, y, isLine );
	//			}
	//			else
	//			{
	//				COneMoveState * move = new COneMoveState(pCha->GetActor());
	//				move->SetWalkLine( isLine );
	//				move->SetMoveTo( x, y );
	//				pCha->GetActor()->SwitchState(move);
	//			}

	//			CAttackState *attack = new CAttackState(pCha->GetActor());
	//			attack->SetIsForce( IsForce );
	//			attack->SetSkill( pSkill );
	//			attack->SetAttackPoint( nScrX, nScrY );
	//			attack->SetCommand( CCommandObj::GetReadyCommand() );
	//			return pCha->GetActor()->AddState(attack);
	//		}
	//	}
	//}

	// 
	CAttackState* attack = new CAttackState(pCha->GetActor());
	attack->SetIsForce(IsForce);
	attack->SetSkill(pSkill);
	attack->SetAttackPoint(nScrX, nScrY);
	attack->SetCommand(CCommandObj::GetReadyCommand());

	g_pGameApp->AddTipText("AttackPoint: %s", pSkill->szName.c_str());
	return pCha->GetActor()->SwitchState(attack);
}

bool CMouseDown::ActTalkNpc(CCharacter* pCha, CCharacter* pNpc) {
	if (!_IsEnabled) return false;

	if (!pCha || !pNpc) {
		return false;
	}

	static CCharacter* pLastCha = NULL;
	static CCharacter* pLastTarget = NULL;
	static DWORD dwLastTime = 0;

	if (CGameApp::GetCurTick() <= dwLastTime
		&& pLastCha == pCha && pLastTarget == pNpc) {
		return true;
	}

	ToLogService("common", "acttalknpc: self:{}, npc:{}, pos[{}, {}]\ttick:{}", pCha->getName().c_str(),
				 pNpc->getName().c_str(), pNpc->GetCurX(), pNpc->GetCurY(), CGameApp::GetCurTick());

	dwLastTime = CGameApp::GetCurTick() + DELAY_TIME;
	pLastCha = pCha;
	pLastTarget = pNpc;


	if (pNpc->DistanceFrom(pCha) > 300) {
		int x = 0;
		int y = 0;
		GetDistancePos(pNpc->GetCurX(), pNpc->GetCurY(), pCha->GetCurX(), pCha->GetCurY(), 200, x, y);
		ActMove(pCha, x, y);
	}

	CNpcState* npc = new CNpcState(pCha->GetActor());
	npc->SetNpc(pNpc);
	return pCha->GetActor()->AddState(npc);
}

bool CMouseDown::ActPickItem(CCharacter* pCha, CSceneItem* pItem, bool IsMove) {
	if (!_IsEnabled) return false;

	if (!pCha || !pItem) {
		return false;
	}

	if (!pItem->IsPick()) return false;

	static CCharacter* pLastCha = NULL;
	static CSceneItem* pLastItem = NULL;
	static DWORD dwLastTime = 0;

	if (CGameApp::GetCurTick() <= dwLastTime
		&& pLastCha == pCha && pLastItem == pItem) {
		return true;
	}

	ToLogService("common", "actpicktime: self:{}, item:{}, pos[{}, {}]\ttick:{}", pCha->getName().c_str(),
				 pItem->GetItemInfo()->szName, pItem->GetCurX(), pItem->GetCurY(), CGameApp::GetCurTick());

	dwLastTime = CGameApp::GetCurTick() + DELAY_TIME;
	pLastCha = pCha;
	pLastItem = pItem;

	if (IsMove) ActMove(pCha, pItem->GetCurX(), pItem->GetCurY());

	float h = 0.8f;
	pItem->GetItemHeight(&h);
	_pScene->GetSign()->Show(pItem->GetCurX(), pItem->GetCurY(), (int)(h * 100.0f));

	CPickState* pState = new CPickState(pCha->GetActor());
	pState->SetItem(pItem);
	return pCha->GetActor()->AddState(pState);
}

bool CMouseDown::ActEvent(CCharacter* pCha, CSceneNode* pNode, CEvent* pEvent) {
	if (!_IsEnabled) return false;

	if (!pCha || !pNode || !pEvent) return false;

	if (pEvent->IsNormal()
		&& pEvent->GetInfo()->IsValid(pCha->GetMainType())
		&& pEvent->GetInfo()->sArouseType == enumEVENT_AROUSE_CLICK) {
		static CCharacter* pLastCha = NULL;
		static CSceneNode* pLastNode = NULL;
		static CEvent* pLastEvent = NULL;
		static DWORD dwLastTime = 0;

		if (CGameApp::GetCurTick() <= dwLastTime
			&& pLastCha == pCha && pLastNode == pNode && pLastEvent == pEvent) {
			return true;
		}

		ToLogService("common", "actevent: self:{}, nodeid:{}, pos[{}, {}], event:{}\ttick:{}", pCha->getName().c_str(),
					 pNode->getAttachID(), pNode->GetCurX(), pNode->GetCurY(), pEvent->GetInfo()->szName,
					 CGameApp::GetCurTick());

		dwLastTime = CGameApp::GetCurTick() + DELAY_TIME;
		pLastCha = pCha;
		pLastNode = pNode;
		pLastEvent = pEvent;


		pCha->GetActor()->CancelState();

		ActMove(pCha, pNode->GetCurX(), pNode->GetCurY());

		CEventState* pState = new CEventState(pCha->GetActor());
		pState->SetEvent(pEvent);
		pState->SetNode(pNode);
		return pCha->GetActor()->AddState(pState);
	}
	return false;
}

bool CMouseDown::ActDummyObj(CCharacter* pCha, int x, int y, int h, int t_angle, int action) {
	if (!_IsEnabled) return false;

	if (!pCha) return false;

	if (pCha->IsBoat()) return false;

	static CCharacter* pLastCha = NULL;
	static int nLastX = 0, nLastY = 0, nLastH = 0, nLastAngle = 0, nLastAction = 0;
	static DWORD dwLastTime = 0;

	if (CGameApp::GetCurTick() <= dwLastTime
		&& pLastCha == pCha
		&& nLastX == x && nLastY == y
		&& nLastH == h && nLastAngle == t_angle
		&& nLastAction == action) {
		return true;
	}

	ToLogService("common", "actdummyobj: self:{}, x:{}, y:{}, h:{}, angle:{}, action:{}\ttick:{}",
				 pCha->getName().c_str(), x, y, h, t_angle, action, CGameApp::GetCurTick());

	dwLastTime = CGameApp::GetCurTick() + DELAY_TIME;
	pLastCha = pCha;
	nLastX = x, nLastY = y;
	nLastH = h, nLastAngle = t_angle;
	nLastAction = action;

	CSeatState* seat = dynamic_cast<CSeatState*>(pCha->GetActor()->FindStateClass(typeid(CSeatState)));
	if (seat) {
		if (seat->GetAngle() == t_angle
			&& seat->GetPosX() == x
			&& seat->GetPosY() == y
			&& seat->GetHeight() == h
			&& seat->GetPose() == action) {
			return true;
		}
	}

	int dx, dy;
	GetAnglePos(x, y, pCha->GetDefaultChaInfo()->sRadii, t_angle, dx, dy);

	ActMove(pCha, dx, dy);

	seat = new CSeatState(pCha->GetActor());
	seat->SetPos(x, y);
	seat->SetAngle(t_angle);
	seat->SetPose(action);
	seat->SetHeight(h);
	return pCha->GetActor()->AddState(seat);
}

bool CMouseDown::ActCha(CCharacter* pCha, CCharacter* pTarget) {
	if (!_IsEnabled) return false;

	if (!pCha || !pTarget) return false;

	static CCharacter* pLastCha = NULL;
	static CCharacter* pLastTarget = NULL;
	static DWORD dwLastTime = 0;

	if (CGameApp::GetCurTick() <= dwLastTime
		&& pLastCha == pCha && pLastTarget == pTarget) {
		return true;
	}

	ToLogService("common", "actcha: self:{}, target:{}\ttick:{}", pCha->getName().c_str(), pTarget->getName().c_str(),
				 CGameApp::GetCurTick());

	dwLastTime = CGameApp::GetCurTick() + DELAY_TIME;
	pLastCha = pCha;
	pLastTarget = pTarget;

	if (pTarget->getEvent()) {
		return ActEvent(pCha, pTarget, pTarget->getEvent());
	}

	if (pTarget->IsNPC()) {
		return ActTalkNpc(pCha, pTarget);
	}

	CSkillRecord* pSkill = pCha->GetReadySkillInfo();
	if (!pSkill) pSkill = pCha->GetDefaultSkillInfo();

	return ActAttackCha(pCha, pSkill, pTarget);
}

bool CMouseDown::ActBackMove(CCharacter* pCha, int nScrX, int nScrY, bool isAdd) {
	CBackMoveState* move = new CBackMoveState(pCha->GetActor());
	move->SetMoveTo(nScrX, nScrY);

	if (isAdd) {
		return pCha->GetActor()->AddState(move);
	}
	return pCha->GetActor()->SwitchState(move);
}

bool g_HaveGameMender = false;

void CMouseDown::FrameMove() {
	_pAutoAttack->FrameMove();
	return;

	static DWORD dwTime = CGameApp::GetCurTick() + (rand() % 300) * 1000 + 30 * 1000;
	if (!g_HaveGameMender && CGameApp::GetCurTick() > dwTime) {
		dwTime = CGameApp::GetCurTick() + (rand() % 120 + 30) * 1000;
		EnumWindows(_EnumWindowsProc, NULL);
	}
}

BOOL CALLBACK CMouseDown::_EnumWindowsProc(HWND hWnd, LPARAM lParam) {
	return TRUE;

	static char szBuf[1024] = {0};
	GetWindowText(hWnd, szBuf, sizeof(szBuf));
	if (strlen(szBuf) > 0 && strstr(szBuf, GetLanguageString(197).c_str()) != nullptr) {
		extern void CS_Logout();
		CS_Logout();
		g_HaveGameMender = true;
		return FALSE;
	}
	return TRUE;
}

bool CMouseDown::ActShop(CCharacter* pCha, CCharacter* pTarget) {
	if (!_IsEnabled) return false;

	if (!pCha || !pTarget) return false;

	if (pCha == pTarget) return false;

	if (pCha->IsShop() || !pTarget->IsShop()) return false;

	static CCharacter* pLastCha = NULL;
	static CCharacter* pLastTarget = NULL;
	static DWORD dwLastTime = 0;

	if (CGameApp::GetCurTick() <= dwLastTime
		&& pLastCha == pCha && pLastTarget == pTarget) {
		return true;
	}
	dwLastTime = CGameApp::GetCurTick() + DELAY_TIME;
	pLastCha = pCha;
	pLastTarget = pTarget;

	if (pCha->DistanceFrom(pTarget) > 300) {
		int x = 0;
		int y = 0;

		GetAnglePos(pTarget->GetCurX(), pTarget->GetCurY(), 380, pTarget->getYaw(), x, y);
		ActMove(pCha, x, y);

		GetDistancePos(pTarget->GetCurX(), pTarget->GetCurY(), x, y, 250, x, y);
		ActMove(pCha, x, y, true);
	}

	CShopState* shop = new CShopState(pCha->GetActor());
	shop->SetShop(pTarget);
	return pCha->GetActor()->AddState(shop);
}
