//=============================================================================
// FileName: FightAble.cpp
// Creater: ZhangXuedong
// Date: 2004.09.15
// Comment: CFightAble class
//=============================================================================
#include "stdafx.h"
#include "FightAble.h"
#include "Util.h"
#include "GameApp.h"
#include "GameAppNet.h"
#include "SubMap.h"
#include "LevelRecord.h"
#include "SailLvRecord.h"
#include "LifeLvRecord.h"
#include "CommFunc.h"
#include "Character.h"
#include "Player.h"
#include "ItemAttr.h"
#include "SkillStateRecord.h"
#include "SkillState.h"
#include "HarmRec.h"
#include "lua_gamectrl.h"
#include "LuaAPI.h"
#include "CommandMessages.h"
#include "LevelRecordStore.h"
#include "LifeLvRecordStore.h"
#include "SailLvRecordStore.h"

using namespace std;

CTimeSkillMgr	g_CTimeSkillMgr;
char	g_chItemFall[defCHA_INIT_ITEM_NUM + 1];

_DBC_USING


//=============================================================================
CFightAble::CFightAble() : m_CSkillState(SKILL_STATE_MAXID)
{
	m_SFightInit.chTarType = 0;
	m_SFightProc.sState = enumFSTATE_TARGET_NO;
	m_SFightProc.sRequestState = SFightProc::Request::None;

	m_pCChaRecord = 0;

	m_usTickInterval = 100;
	m_bOnFight = false;
	m_uchFightID = 0;
}

void CFightAble::Initially()
{
	CAttachable::Initially();

	memset(&m_SFightInit, 0, sizeof(SFightInit));
	m_SFightInit.chTarType = 0;
	memset(&m_SFightInitCache, 0, sizeof(SFightInit));
	m_SFightInitCache.chTarType = 0;
	memset(&m_SFightProc, 0, sizeof(SFightProc));
	m_SFightProc.sState = enumFSTATE_TARGET_NO;

	m_CChaAttr.Clear();
	m_pCChaRecord = 0;
	m_CSkillState.Reset();
	m_CSkillBag.Init();
	m_sDefSkillNo = 0;

	m_usTickInterval = 100;
	m_bOnFight = false;
	m_uchFightID = 0;

	m_bLookAttrChange = false;
}

void CFightAble::Finally()
{
	m_SFightInit.chTarType = 0;
	CAttachable::Finally();
}

void CFightAble::ResetFight()
{
	m_SFightInit.chTarType = 0;
	m_SFightProc.sState = enumFSTATE_TARGET_NO;
	m_SFightProc.sRequestState = SFightProc::Request::None;

	m_bOnFight = false;
}

bool CFightAble::DesireFightBegin(SFightInit *pSFightInit)
{
	m_CChaAttr.ResetChangeFlag();
	m_CSkillState.ResetChangeFlag();

	if (!IsRightSkillSrc(pSFightInit->pCSkillRecord->chHelpful))
	{
		memcpy(&m_SFightInit, pSFightInit, sizeof(SFightInit));
		m_SFightProc.sState = enumFSTATE_OFF;
		NotiSkillSrcToSelf();
		SubsequenceFight();
		return false;
	}
	if (m_SFightProc.sState == enumFSTATE_ON)
	{
		EndFight();
		return false;
	}

	// 
	SetExistState(enumEXISTS_FIGHTING);
	if (GetTickCount() - (uLong)pSFightInit->pSSkillGrid->lColdDownT > (uLong)GetSkillTime(pSFightInit->pCSkillTData))
	{
		memcpy(&m_SFightInit, pSFightInit, sizeof(SFightInit));
		m_SFightProc.sRequestState = SFightProc::Request::None;
		BeginFight();
	}
	else// if( m_SFightInit.pSSkillGrid != pSFightInit->pSSkillGrid )
	{
		memcpy(&m_SFightInitCache, pSFightInit, sizeof(SFightInit));
		m_SFightProc.sRequestState = SFightProc::Request::StartAttack;
		OnFightBegin();
		return true;
	}
	return true;
}

void CFightAble::BeginFight()
{
	// log
	//

	m_SFightProc.sState = enumFSTATE_ON;
	m_uchFightID++;

	Square	STarShape = {{0, 0}, 0};
	Long	lReqDist = 0;
	if (!GetFightTargetShape(&STarShape)) // 
	{
		m_SFightProc.sState = enumFSTATE_TARGET_NO;
		m_SFightInit.chTarType = 0;
		NotiSkillSrcToEyeshot();
		SubsequenceFight();
		
		// add by ryan wang , lColdDownT, 
		m_ulLastTick = GetTickCount();
		m_SFightInit.pSSkillGrid->lColdDownT = m_ulLastTick;
		//----------------------------------------------------------------------------
		return;
	}
	lReqDist = GetRadius() + STarShape.radius + m_SFightInit.pCSkillRecord->sApplyDistance;

	long	lDistX2 = (GetShape().centre.x - STarShape.centre.x) * (GetShape().centre.x - STarShape.centre.x);
	long	lDistY2 = (GetShape().centre.y - STarShape.centre.y) * (GetShape().centre.y - STarShape.centre.y);
	if (lDistX2 + lDistY2 <= lReqDist * lReqDist)
	{
		if (m_SFightInit.pCSkillRecord->chOperate[0] == 0) // 
		{
			//g_CParser.DoString(m_SFightInit.pCSkillRecord->szPrepare, enumSCRIPT_RETURN_NONE, 0, enumSCRIPT_PARAM_LIGHTUSERDATA, 1, this->IsCharacter(), enumSCRIPT_PARAM_NUMBER, 1, m_SFightInit.pSSkillGrid->chLv, DOSTRING_PARAM_END);
			SkillGeneral((long)sqrt((double)lDistX2 + lDistY2));
		}
		else
		{
			m_SFightProc.sState = enumFSTATE_OFF;
			NotiSkillSrcToSelf();
		}

		m_ulLastTick = GetTickCount();
		m_SFightInit.pSSkillGrid->lColdDownT = m_ulLastTick;
		if (m_SFightProc.sState == enumFSTATE_ON)
		{
			OnFightBegin();
		}
	}
	else
	{
		// add by ryan wang , lColdDownT, 
		m_ulLastTick = GetTickCount();
		m_SFightInit.pSSkillGrid->lColdDownT = m_ulLastTick;
		//----------------------------------------------------------------------------
		
		m_SFightProc.sState = enumFSTATE_TARGET_OUT;
		NotiSkillSrcToSelf();
	}
	if (m_SFightProc.sState)
	{
		SubsequenceFight();
	}
}

void CFightAble::OnFight(uLong ulCurTick)
{
	if (!m_bOnFight)
		return;

	uLong	ulTickDist = ulCurTick - m_ulLastTick;
	if (ulTickDist < m_usTickInterval)
		return;
	m_ulLastTick = ulCurTick;

	if (m_SFightProc.sState == enumFSTATE_ON)
		if (m_SFightInit.pSSkillGrid->chState != enumSUSTATE_ACTIVE // 
			|| (m_SFightInit.pCSkillTData->lResumeTime == 0 && !IsCharacter()->GetActControl(enumACTCONTROL_USE_GSKILL)) // 
			|| (m_SFightInit.pCSkillTData->lResumeTime > 0 && !IsCharacter()->GetActControl(enumACTCONTROL_USE_MSKILL))) // 
		{
			m_SFightProc.sState = enumFSTATE_CANCEL; // 
			NotiSkillSrcToEyeshot();
			EndFight();
			return;
		}

	m_CChaAttr.ResetChangeFlag();
	m_CSkillState.ResetChangeFlag();

	if (m_SFightProc.sRequestState == SFightProc::Request::StopAttack)
	{
		m_SFightProc.sState = enumFSTATE_CANCEL; // 
		m_SFightProc.sRequestState = SFightProc::Request::None;
		NotiSkillSrcToEyeshot();
		SubsequenceFight();
	}

	Square	STarShape = {{0, 0}, 0};
	Long	lReqDist = 0;
	if (m_SFightProc.sState == enumFSTATE_ON && !GetFightTargetShape(&STarShape)) // 
	{
		m_SFightProc.sState = enumFSTATE_TARGET_NO;
		m_SFightInit.chTarType = 0;
		NotiSkillSrcToEyeshot();
		SubsequenceFight();
	}

	if (m_SFightProc.sState == enumFSTATE_ON)
	{
		lReqDist = GetRadius() + STarShape.radius + m_SFightInit.pCSkillRecord->sApplyDistance;

		long	lDistX2 = (GetShape().centre.x - STarShape.centre.x) * (GetShape().centre.x - STarShape.centre.x);
		long	lDistY2 = (GetShape().centre.y - STarShape.centre.y) * (GetShape().centre.y - STarShape.centre.y);
		if (lDistX2 + lDistY2 > lReqDist * lReqDist)
		{
			m_SFightProc.sState = enumFSTATE_TARGET_OUT; // 
			NotiSkillSrcToEyeshot();
		}
		else
		{
			Long	lResumeT = GetSkillTime(m_SFightInit.pCSkillTData);
			Long	lResumeDist = ulCurTick - m_SFightInit.pSSkillGrid->lColdDownT;

			if (lResumeDist > lResumeT)
			{
				Short	sExecTime;
				if (lResumeT <= 0)
					sExecTime = 1;
				else
					sExecTime = Short(lResumeDist / lResumeT);

				// add by ryan wang, , 
				if(GetPlayer()==NULL)
				{
					if(sExecTime > 1)
					{
						//LG("skill_error", "[%s][%s], , %d ms, cooldown = %d\n", GetName(), m_SFightInit.pCSkillRecord->szName, lResumeDist, lResumeT);
						ToLogService("errors", LogLevel::Error, "[{}] use [{}] skill, interval time account error, interval last time {} ms, skill cooldown = {}", GetName(), m_SFightInit.pCSkillRecord->szName, lResumeDist, lResumeT);
						sExecTime = 1; // 1, 
						m_SFightInit.pSSkillGrid->lColdDownT = ulCurTick - lResumeT; 
					}
				}
				//-----------------------------------------------------------------------

				if (m_SFightInit.pCSkillRecord->chOperate[0] == 0) // 
				{
					short i;
					for (i = 0; i < sExecTime; i++)
					{
						if (!SkillGeneral((long)sqrt((double)lDistX2 + lDistY2)))
							break;
					}
					m_SFightInit.pSSkillGrid->lColdDownT += lResumeT * i;
				}
			}
		}

		if (m_SFightProc.sState != enumFSTATE_ON)
			SubsequenceFight();
	}

	if (m_SFightProc.sState != enumFSTATE_ON)
	{
		if (m_SFightProc.sRequestState == SFightProc::Request::StartAttack)
		{
			if (ulCurTick - (uLong)m_SFightInitCache.pSSkillGrid->lColdDownT > (uLong)GetSkillTime(m_SFightInitCache.pCSkillTData)) // 
			{
				memcpy(&m_SFightInit, &m_SFightInitCache, sizeof(SFightInit));
				m_SFightProc.sRequestState = SFightProc::Request::None;
				OnFightEnd();
				BeginFight();
				return;
			}
		}
		else
			OnFightEnd();
	}
}

void CFightAble::EndFight()
{
	m_SFightProc.sRequestState = SFightProc::Request::StopAttack;
}

void CFightAble::SkillTarEffect(SFireUnit *pSFireSrc)
{
	// 
	CCharacter	*pSrcCha = pSFireSrc->pCFightSrc->IsCharacter();
	CCharacter	*pSrcMainC = 0;
	pSrcMainC = pSrcCha->GetPlyMainCha();
	if (pSrcMainC == pSrcCha)
		pSrcMainC = 0;

	g_uchFightID = pSFireSrc->uchFightID;

	pSrcCha->m_CChaAttr.ResetChangeFlag();
	pSrcCha->m_CSkillState.ResetChangeFlag();
	if (pSrcMainC)
	{
		pSrcMainC->m_CChaAttr.ResetChangeFlag();
		pSrcMainC->m_CSkillState.ResetChangeFlag();
		pSrcMainC->SetLookChangeFlag();
		pSrcMainC->SetEspeItemChangeFlag();
	}
	else
	{
		pSrcCha->SetLookChangeFlag();
		pSrcCha->SetEspeItemChangeFlag();
	}
	m_CChaAttr.ResetChangeFlag();
	m_CSkillState.ResetChangeFlag();
	IsCharacter()->GetPlyMainCha()->SetLookChangeFlag();
	IsCharacter()->GetPlyMainCha()->SetEspeItemChangeFlag();

	// 
	m_SFightProc.bCrt = false;
	m_SFightProc.bMiss = false;

	Long	lOldHP = (long)m_CChaAttr.GetAttr(ATTR_HP);
	Long	lNowHP;
	Long	lSrcOldHP = (long)pSrcCha->m_CChaAttr.GetAttr(ATTR_HP);
	//
	for (int i = 0; i < pSFireSrc->sExecTime; i++)
		//g_CParser.DoString(pSFireSrc->pCSkillRecord->szEffect, enumSCRIPT_RETURN_NONE, 0, enumSCRIPT_PARAM_LIGHTUSERDATA, 2, pSrcCha, this->IsCharacter(), enumSCRIPT_PARAM_NUMBER, 1, pSrcCha->m_SFightInit.pSSkillGrid->chLv, DOSTRING_PARAM_END);
		g_luaAPI.Call(pSFireSrc->pCSkillRecord->szEffect.c_str(), pSrcCha, this->IsCharacter(), (int)pSrcCha->m_SFightInit.pSSkillGrid->chLv);
	lNowHP = (long)m_CChaAttr.GetAttr(ATTR_HP);
	BeUseSkill(lOldHP, lNowHP, pSrcCha, pSFireSrc->pCSkillRecord->chHelpful);

	// 
	if( m_CChaAttr.GetAttr(ATTR_CHATYPE) != enumCHACTRL_MONS_MINE && m_CChaAttr.GetAttr(ATTR_CHATYPE) != enumCHACTRL_MONS_TREE
		&& m_CChaAttr.GetAttr(ATTR_CHATYPE) != enumCHACTRL_MONS_FISH && m_CChaAttr.GetAttr(ATTR_CHATYPE) != enumCHACTRL_MONS_DBOAT)
	{
		// 
		if (lOldHP > 0 && lNowHP <= 0)
		{
			SetDie(pSrcCha);
		}
		if (lSrcOldHP > 0 && pSrcCha->m_CChaAttr.GetAttr(ATTR_HP) <= 0)
		{
			pSrcCha->SetDie(IsCharacter());
		}
	}
	else
	{
		// 
		if(lNowHP <= 0)
		{
			// 
			SetExistState(enumEXISTS_WITHERING);
			m_SFightInit.chTarType = 0;
			m_SFightProc.sState = enumFSTATE_DIE;
			m_SExistCtrl.ulTick = GetTickCount();
			m_CSkillState.Reset();
		}

		Long lNumData;
		if (lNowHP <= 0)
			lNumData = lOldHP;
		else
			lNumData = lOldHP - lNowHP;
		// 
		for( int i = 0; i < lNumData; i++ )
		{
			// 
			SpawnResource( pSrcCha, pSrcCha->m_SFightInit.pSSkillGrid->chLv );
		}

		/* 
		
		// 
		if (pSrcCha->m_CChaAttr.GetAttr(ATTR_HP) <= 0)
		{
			bSrcDie = true;
			pSrcCha->SetExistState(enumEXISTS_WITHERING);
			pSrcCha->m_SFightInit.chTarType = 0;
			pSrcCha->m_SFightProc.sState = enumFSTATE_DIE;
			pSrcCha->m_SExistCtrl.ulTick = GetTickCount();
			pSrcCha->m_CSkillState.Reset();
		}
		*/
	}

	NotiSkillTarToEyeshot(pSFireSrc);
	RectifyAttr();
	IsCharacter()->GetPlyMainCha()->SynLook(enumSYN_LOOK_CHANGE);
	IsCharacter()->GetPlyMainCha()->SynEspeItem();
	pSrcCha->RectifyAttr();
	if (pSrcMainC)
	{
		pSrcMainC->SynAttr(enumATTRSYN_ATTACK);
		pSrcMainC->SynSkillStateToEyeshot();
		pSrcMainC->RectifyAttr();
		pSrcMainC->SynLook(enumSYN_LOOK_CHANGE);
		pSrcMainC->SynEspeItem();
	}
	else
	{
		pSrcCha->SynLook(enumSYN_LOOK_CHANGE);
		pSrcCha->SynEspeItem();
	}
}

bool CFightAble::RectifyAttr()
{
	bool	bRectify = false;
	Long	lMaxHP = (long)m_CChaAttr.GetAttr(ATTR_MXHP);
	if (m_CChaAttr.GetAttr(ATTR_HP) > lMaxHP)
	{
		bRectify = true;
		m_CChaAttr.ResetChangeFlag();
		setAttr(ATTR_HP, lMaxHP);
	}
	Long	lMaxSP = (long)m_CChaAttr.GetAttr(ATTR_MXSP);
	if (m_CChaAttr.GetAttr(ATTR_SP) > lMaxSP)
	{
		if (!bRectify)
			m_CChaAttr.ResetChangeFlag();
		setAttr(ATTR_SP, lMaxSP);
	}

	if (bRectify)
		SynAttr(enumATTRSYN_RECTIFY);

	return bRectify;
}

// 012
Long CFightAble::setAttr(int nIdx, LONG32 lValue, int nType)
{
	if (nIdx == ATTR_GD && lValue < 0)
		return m_CChaAttr.SetAttr(nIdx, 0);

	LONG32	lOldVal = m_CChaAttr.GetAttr(nIdx);
	Long	lRet = m_CChaAttr.SetAttr(nIdx, lValue);
	if (lRet != 2)
		return lRet;
	if (nType != 0)
		return lRet;
	if (IsCharacter() != IsCharacter()->GetPlyMainCha())
		return lRet;
	if (nIdx == ATTR_CEXP)
		CountLevel();
	else if (nIdx == ATTR_CSAILEXP)
		CountSailLevel();
	else if (ATTR_CLIFEEXP == nIdx )
		CountLifeLevel();

	AfterAttrChange(nIdx, lOldVal, m_CChaAttr.GetAttr(nIdx));

	return lRet;
}

void CFightAble::SetDie(CCharacter *pCSkillSrcCha)
{
	SetItemHostObj(0);
	SetExistState(enumEXISTS_WITHERING);
	m_SFightInit.chTarType = 0;
	m_SFightProc.sState = enumFSTATE_DIE;
	m_SExistCtrl.ulTick = GetTickCount();
	RemoveAllSkillState();
	m_CSkillState.Reset();

	CCharacter	*pCDieCha = this->IsCharacter();
	if (pCSkillSrcCha && pCSkillSrcCha != g_pCSystemCha)
	{
		pCDieCha->JustDie(pCSkillSrcCha);
	}

	pCDieCha->m_pHate->ClearHarmRec();

	if (pCDieCha->IsPlayerOwnCha() && pCSkillSrcCha && pCSkillSrcCha->IsPlayerOwnCha())
	{
		//g_CParser.DoString("after_player_kill_player", enumSCRIPT_RETURN_NONE, 0, enumSCRIPT_PARAM_LIGHTUSERDATA, 2, pCSkillSrcCha, pCDieCha, DOSTRING_PARAM_END);
		g_luaAPI.Call("after_player_kill_player", pCSkillSrcCha, pCDieCha);
	}
}

Long CFightAble::GetSkillTime(CSkillTempData *pCSkillTData)
{
	if (pCSkillTData->lResumeTime == 0) // 
		return (long)m_CChaAttr.GetAttr(ATTR_ASPD);
	else
		return pCSkillTData->lResumeTime;
}

void CFightAble::BeUseSkill(dbc::Long lPreHp, dbc::Long lNowHp, CCharacter *pCSrcCha, Char chSkillEffType)
{
	if (!pCSrcCha || pCSrcCha == g_pCSystemCha)
		return;

	if (chSkillEffType == enumSKILL_EFF_BANEFUL)
	{
		SetMonsterFightObj(pCSrcCha->GetID(), pCSrcCha->GetHandle());
		if (lPreHp != lNowHp)
		{
			CCharacter *pCha = IsCharacter();
			if(pCha->m_HostCha!=pCSrcCha) // 
			{
				pCha->m_pHate->AddHarm(pCSrcCha, Short(lPreHp - lNowHp), pCSrcCha->GetID());
			}
		}
	}
}

void CFightAble::SetMonsterFightObj(uLong ulObjWorldID, Long lObjHandle)
{
	if (m_SFightInit.chTarType == 0 && m_CChaAttr.GetAttr(ATTR_CHATYPE) != enumCHACTRL_PLAYER)
	{
		m_SFightInit.chTarType = 1;
		m_SFightInit.lTarInfo1 = ulObjWorldID;
		m_SFightInit.lTarInfo2 = lObjHandle;
	}

}

void CFightAble::NotiSkillSrcToEyeshot(Short sExecTime)
{
	//  : SKILL_SRC   
	net::msg::McCharacterActionMessage msg;
	msg.worldId = m_ID;
	msg.packetId = m_ulPacketID;
	msg.actionType = net::msg::ActionType::SKILL_SRC;
	msg.data = net::msg::ActionSkillSrcData{};

	auto& d = std::get<net::msg::ActionSkillSrcData>(msg.data);
	d.fightId = m_uchFightID;
	d.angle = m_sAngle;
	d.state = m_SFightProc.sState;
	if (m_SFightProc.sState & enumFSTATE_DIE)
		d.stopState = enumEXISTS_WITHERING;
	else if (m_SFightProc.sState != enumFSTATE_ON)
		d.stopState = m_SFightInit.sStopState;
	d.skillId = m_SFightInit.pCSkillRecord->sID;
	d.skillSpeed = GetSkillTime(m_SFightInit.pCSkillTData);
	d.targetType = m_SFightInit.chTarType;
	if (m_SFightInit.chTarType == 1)
	{
		d.targetId = m_SFightInit.lTarInfo1;
		Entity *pEnt = g_pGameApp->GetEntity(m_SFightInit.lTarInfo2);
		if (!pEnt)
		{
			d.targetX = GetShape().centre.x;
			d.targetY = GetShape().centre.y;
		}
		else
		{
			d.targetX = pEnt->GetShape().centre.x;
			d.targetY = pEnt->GetShape().centre.y;
		}
	}
	else if (m_SFightInit.chTarType == 2)
	{
		d.targetX = m_SFightInit.lTarInfo1;
		d.targetY = m_SFightInit.lTarInfo2;
	}
	d.execTime = sExecTime;

	//   ()
	for (int i = 0; i < ATTR_CLIENT_MAX; i++)
	{
		if (m_CChaAttr.GetChangeBitFlag(i))
		{
			net::msg::ActionEffectEntry e;
			e.attrId = i;
			if (i == ATTR_NLEXP || i == ATTR_CLEXP || i == ATTR_CEXP)
				e.attrVal = m_CChaAttr.GetAttr(i);
			else
				e.attrVal = (unsigned long)m_CChaAttr.GetAttr(i);
			d.effects.push_back(e);
		}
	}

	//   ( )
	SSkillStateUnit *pSStateUnit;
	m_CSkillState.BeginGetState();
	while (pSStateUnit = m_CSkillState.GetNextState())
	{
		net::msg::ActionStateEntry s;
		s.stateId = pSStateUnit->GetStateID();
		s.stateLv = pSStateUnit->GetStateLv();
		d.states.push_back(s);
	}

	auto pk = net::msg::serialize(msg);
	NotiChgToEyeshot(pk);

}

void CFightAble::NotiSkillSrcToSelf(Short sExecTime)
{
	//  : SKILL_SRC 
	net::msg::McCharacterActionMessage msg;
	msg.worldId = m_ID;
	msg.packetId = m_ulPacketID;
	msg.actionType = net::msg::ActionType::SKILL_SRC;
	msg.data = net::msg::ActionSkillSrcData{};

	auto& d = std::get<net::msg::ActionSkillSrcData>(msg.data);
	d.fightId = m_uchFightID;
	d.angle = m_sAngle;
	d.state = m_SFightProc.sState;
	if (m_SFightProc.sState & enumFSTATE_DIE)
		d.stopState = enumEXISTS_WITHERING;
	else if (m_SFightProc.sState != enumFSTATE_ON)
		d.stopState = m_SFightInit.sStopState;
	d.skillId = m_SFightInit.pCSkillRecord->sID;
	d.skillSpeed = GetSkillTime(m_SFightInit.pCSkillTData);
	d.targetType = m_SFightInit.chTarType;
	if (m_SFightInit.chTarType == 1)
	{
		d.targetId = m_SFightInit.lTarInfo1;
		Entity *pEnt = g_pGameApp->GetEntity(m_SFightInit.lTarInfo2);
		d.targetX = pEnt->GetShape().centre.x;
		d.targetY = pEnt->GetShape().centre.y;
	}
	else if (m_SFightInit.chTarType == 2)
	{
		d.targetX = m_SFightInit.lTarInfo1;
		d.targetY = m_SFightInit.lTarInfo2;
	}
	d.execTime = sExecTime;

	//   ()
	for (int i = 0; i < ATTR_CLIENT_MAX; i++)
	{
		if (m_CChaAttr.GetChangeBitFlag(i))
		{
			net::msg::ActionEffectEntry e;
			e.attrId = i;
			if (i == ATTR_NLEXP || i == ATTR_CLEXP || i == ATTR_CEXP)
				e.attrVal = m_CChaAttr.GetAttr(i);
			else
				e.attrVal = (unsigned long)m_CChaAttr.GetAttr(i);
			d.effects.push_back(e);
		}
	}

	//   ( )
	{
		SSkillStateUnit *pSStateUnit;
		m_CSkillState.BeginGetState();
		while (pSStateUnit = m_CSkillState.GetNextState())
		{
			if (m_CSkillState.GetChangeBitFlag(pSStateUnit->GetStateID()))
			{
				net::msg::ActionStateEntry s;
				s.stateId = pSStateUnit->GetStateID();
				s.stateLv = pSStateUnit->GetStateLv();
				d.states.push_back(s);
			}
		}
	}

	auto pk = net::msg::serialize(msg);
	ReflectINFof(this,pk);

}

void CFightAble::NotiSkillTarToEyeshot(SFireUnit *pSFireSrc)
{
	//  : SKILL_TAR   
	net::msg::McCharacterActionMessage msg;
	msg.worldId = m_ID;
#ifdef defPROTOCOL_HAVE_PACKETID
	msg.packetId = pSFireSrc->ulPacketID;
#endif
	msg.actionType = net::msg::ActionType::SKILL_TAR;
	msg.data = net::msg::ActionSkillTarData{};

	auto& d = std::get<net::msg::ActionSkillTarData>(msg.data);
	d.fightId = pSFireSrc->uchFightID;
	d.state = m_SFightProc.sState;
	d.doubleAttack = m_SFightProc.bCrt;
	d.miss = m_SFightProc.bMiss;
	if (g_bBeatBack)
	{
		d.beatBack = true;
		d.beatBackX = GetPos().x;
		d.beatBackY = GetPos().y;
		g_bBeatBack = false;
	}
	d.srcId = pSFireSrc->ulID;
	d.srcPosX = pSFireSrc->SSrcPos.x;
	d.srcPosY = pSFireSrc->SSrcPos.y;
	d.skillId = pSFireSrc->pCSkillRecord->sID;
	d.skillTargetX = pSFireSrc->lTarInfo1;
	d.skillTargetY = pSFireSrc->lTarInfo2;
	d.execTime = pSFireSrc->sExecTime;

	//   ()
	d.synType = enumATTRSYN_ATTACK;
	d.effects.clear();
	for (int i = 0; i < ATTR_CLIENT_MAX; i++)
	{
		if (m_CChaAttr.GetChangeBitFlag(i))
		{
			net::msg::ActionEffectEntry e;
			e.attrId = i;
			e.attrVal = m_CChaAttr.GetAttr(i);
			d.effects.push_back(e);
		}
	}

	//  
	if (m_CSkillState.GetChangeNum() > 0)
	{
		d.hasStates = true;
		d.stateTime = GetTickCount();
		SSkillStateUnit *pSStateUnit;
		m_CSkillState.BeginGetState();
		while (pSStateUnit = m_CSkillState.GetNextState())
		{
			net::msg::ActionTarStateEntry s;
			s.stateId = pSStateUnit->GetStateID();
			s.stateLv = pSStateUnit->GetStateLv();
			if (pSStateUnit->lOnTick < 1) {
				s.duration = 0;
				s.startTime = 0;
			} else {
				s.duration = pSStateUnit->lOnTick;
				s.startTime = pSStateUnit->ulStartTick;
			}
			d.states.push_back(s);
		}
	}

	//   (  != )
	if (pSFireSrc->pCFightSrc != this)
	{
		d.hasSrcEffect = true;
		d.srcState = pSFireSrc->pCFightSrc->m_SFightProc.sState;
		d.srcSynType = enumATTRSYN_ATTACK;
		for (int i = 0; i < ATTR_CLIENT_MAX; i++)
		{
			if (pSFireSrc->pCFightSrc->m_CChaAttr.GetChangeBitFlag(i))
			{
				net::msg::ActionEffectEntry e;
				e.attrId = i;
				e.attrVal = pSFireSrc->pCFightSrc->m_CChaAttr.GetAttr(i);
				d.srcEffects.push_back(e);
			}
		}
		//  
		if (pSFireSrc->pCFightSrc->m_CSkillState.GetChangeNum() > 0)
		{
			d.srcHasStates = true;
			d.srcStateTime = GetTickCount();
			SSkillStateUnit *pSStateUnit;
			pSFireSrc->pCFightSrc->m_CSkillState.BeginGetState();
			while (pSStateUnit = pSFireSrc->pCFightSrc->m_CSkillState.GetNextState())
			{
				net::msg::ActionTarStateEntry s;
				s.stateId = pSStateUnit->GetStateID();
				s.stateLv = pSStateUnit->GetStateLv();
				if (pSStateUnit->lOnTick < 1) {
					s.duration = 0;
					s.startTime = 0;
				} else {
					s.duration = pSStateUnit->lOnTick;
					s.startTime = pSStateUnit->ulStartTick;
				}
				d.srcStates.push_back(s);
			}
		}
	}

	auto pk = net::msg::serialize(msg);
	NotiChgToEyeshot(pk);

}

void CFightAble::SynAttr(Short sType)
{
	short	sAttrChangeNum = m_CChaAttr.GetChangeNumClient();
	if (sAttrChangeNum == 0)
		return;

	//  :     
	net::msg::McSynAttributeMessage msg;
	msg.worldId = m_ID;
	FillAttr(msg.attr, sType);
	auto pk = net::msg::serialize(msg);

	NotiChgToEyeshot(pk, true);
}

void CFightAble::SynAttrToSelf(Short sType)
{
	short	sAttrChangeNum = m_CChaAttr.GetChangeNumClient();
	if (sAttrChangeNum == 0)
		return;

	//  :   
	net::msg::McSynAttributeMessage msg;
	msg.worldId = m_ID;
	FillAttr(msg.attr, sType);
	auto pk = net::msg::serialize(msg);

	ReflectINFof(this,pk);
}

void CFightAble::SynAttrToEyeshot(Short sType) //    
{
	short	sAttrChangeNum = m_CChaAttr.GetChangeNumClient();
	if (sAttrChangeNum == 0)
		return;

	//  :      ( )
	net::msg::McSynAttributeMessage msg;
	msg.worldId = m_ID;
	FillAttr(msg.attr, sType);
	auto pk = net::msg::serialize(msg);

	NotiChgToEyeshot(pk, false);
}

// pCObj
void CFightAble::SynAttrToUnit(CFightAble *pCObj, Short sType)
{
	if (!pCObj)
		return;

	short	sAttrChangeNum = pCObj->m_CChaAttr.GetChangeNumClient();
	if (sAttrChangeNum == 0)
		return;

	//  :     
	net::msg::McSynAttributeMessage msg;
	msg.worldId = pCObj->GetID();
	pCObj->FillAttr(msg.attr, sType);
	auto pk = net::msg::serialize(msg);

	ReflectINFof(this,pk);
}

//      
void CFightAble::SynAttrToUnit(CFightAble *pCObj, Short sStartAttr, Short sEndAttr, Short sType)
{
	if (!pCObj)
		return;

	if (sEndAttr >= ATTR_CLIENT_MAX)
		return;

	//  :   
	net::msg::McSynAttributeMessage msg;
	msg.worldId = pCObj->GetID();
	msg.attr.synType = sType;
	for (int i = sStartAttr; i <= sEndAttr; i++)
	{
		net::msg::AttrEntry e;
		e.attrId = i;
		e.attrVal = pCObj->m_CChaAttr.GetAttr(i);
		msg.attr.attrs.push_back(e);
	}
	auto pk = net::msg::serialize(msg);

	ReflectINFof(this,pk);
}

void CFightAble::SynSkillStateToSelf()
{
	//  :    
	net::msg::McSynSkillStateMessage msg;
	msg.worldId = m_ID;
	FillSkillState(msg.skillState);
	auto pk = net::msg::serialize(msg);

	ReflectINFof(this,pk);
}

void CFightAble::SynSkillStateToEyeshot()
{
	//  :      
	net::msg::McSynSkillStateMessage msg;
	msg.worldId = m_ID;
	FillSkillState(msg.skillState);
	auto pk = net::msg::serialize(msg);

	NotiChgToEyeshot(pk, true);
}

// pCObj
void CFightAble::SynSkillStateToUnit(CFightAble *pCObj)
{
	if (!pCObj)
		return;

	//  :      
	net::msg::McSynSkillStateMessage msg;
	msg.worldId = pCObj->GetID();
	pCObj->FillSkillState(msg.skillState);
	auto pk = net::msg::serialize(msg);

	ReflectINFof(this,pk);
}

void CFightAble::SynLookEnergy(void)
{
	CCharacter	*pCMainCha = IsCharacter()->GetPlyMainCha();

	//  :    std::variant
	net::msg::McCharacterActionMessage msg;
	msg.worldId = pCMainCha->GetID();
	msg.packetId = pCMainCha->m_ulPacketID;
	msg.actionType = net::msg::ActionType::LOOK_ENERGY;
	msg.data = net::msg::ActionLookEnergyData{};

	auto& energyData = std::get<net::msg::ActionLookEnergyData>(msg.data);
	SItemGrid *pItem;
	for (int i = 0; i < enumEQUIP_NUM; i++)
	{
		pItem = &pCMainCha->m_SChaPart.SLink[i];
		if (!g_IsRealItemID(pItem->sID))
			energyData.energy[i] = 0;
		else
			energyData.energy[i] = pItem->sEnergy[0];
	}

	auto WtPk = net::msg::serialize(msg);
	pCMainCha->ReflectINFof(this,WtPk);
}

void CFightAble::WriteSkillState(net::WPacket &pk)
{
	pk.WriteInt64(GetTickCount());//current time
	pk.WriteInt64(m_CSkillState.GetStateNum());
	SSkillStateUnit	*pSStateUnit;
	m_CSkillState.BeginGetState();
	while (pSStateUnit = m_CSkillState.GetNextState())
	{
		pk.WriteInt64(pSStateUnit->GetStateID());
		pk.WriteInt64(pSStateUnit->GetStateLv());
		// end time = pSStateUnit->lOnTick + pSStateUnit->ulStartTick
		if (pSStateUnit->lOnTick < 1){
			pk.WriteInt64(0);//current time
			pk.WriteInt64(0);//current time
		}else{
			pk.WriteInt64(pSStateUnit->lOnTick);//duration
			pk.WriteInt64(pSStateUnit->ulStartTick);//start time
		}
		
	}
}

void CFightAble::WriteAttr(net::WPacket &pk, Short sSynType)
{
	short	sAttrChangeNum = m_CChaAttr.GetChangeNumClient();

	pk.WriteInt64((Char)sSynType);
	pk.WriteInt64(sAttrChangeNum);
	if (sAttrChangeNum > 0)
	{
		for (int i = 0; i < ATTR_CLIENT_MAX; i++)
		{
			if (m_CChaAttr.GetChangeBitFlag(i)) // 
			{
				pk.WriteInt64(i);
				pk.WriteInt64(m_CChaAttr.GetAttr(i)); // 1.3x
			}
		}
	}
}

void CFightAble::WriteMonsAttr(net::WPacket &pk, Short sSynType)
{
	pk.WriteInt64((Char)sSynType);
	pk.WriteInt64(5);

	pk.WriteInt64(ATTR_LV);
	pk.WriteInt64((long)m_CChaAttr.GetAttr(ATTR_LV));
	pk.WriteInt64(ATTR_HP);
	pk.WriteInt64((long)m_CChaAttr.GetAttr(ATTR_HP));
	pk.WriteInt64(ATTR_MXHP);
	pk.WriteInt64((long)m_CChaAttr.GetAttr(ATTR_MXHP));
	pk.WriteInt64(ATTR_ASPD);
	pk.WriteInt64((long)m_CChaAttr.GetAttr(ATTR_ASPD));
	pk.WriteInt64(ATTR_MSPD);
	pk.WriteInt64((long)m_CChaAttr.GetAttr(ATTR_MSPD));
}

void CFightAble::WriteAttr(net::WPacket &pk, dbc::Short sStartAttr, dbc::Short sEndAttr, Short sSynType)
{
	short	sAttrChangeNum = m_CChaAttr.GetChangeNumClient();

	pk.WriteInt64((Char)sSynType);
	pk.WriteInt64(sEndAttr - sStartAttr + 1);
	for (int i = sStartAttr; i <= sEndAttr; i++)
	{
		pk.WriteInt64(i);
		pk.WriteInt64(m_CChaAttr.GetAttr(i));
	}
}

void CFightAble::WriteLookEnergy(net::WPacket &pk)
{
	SItemGrid *pItem;
	for (int i = 0; i < enumEQUIP_NUM; i++)
	{
		pItem = &IsCharacter()->m_SChaPart.SLink[i];
		if (!g_IsRealItemID(pItem->sID))
			pk.WriteInt64(0);
		else
			pk.WriteInt64(pItem->sEnergy[0]);
	}
}

bool CFightAble::GetFightTargetShape(Square *pSTarShape)
{
	if (m_SFightInit.chTarType == 1) // 
	{
		Entity	*pTarObj = g_pGameApp->IsMapEntity(m_SFightInit.lTarInfo1, m_SFightInit.lTarInfo2);
		if (!pTarObj)
			return false;
		if (pSTarShape)
		{
			*pSTarShape = pTarObj->GetShape();
		}
	}
	else if (m_SFightInit.chTarType == 2) // 
	{
		if (pSTarShape)
		{
			pSTarShape->centre.x = m_SFightInit.lTarInfo1;
			pSTarShape->centre.y = m_SFightInit.lTarInfo2;
			pSTarShape->radius = 0;
		}
	}

	return true;
}

bool CFightAble::SkillExpend(Short sExecTime)
{
	CCharacter	*pCMainCha = this->IsCharacter();
	if (GetPlayer())
		pCMainCha = GetPlayer()->GetMainCha();
	if (pCMainCha != this->IsCharacter())
		pCMainCha->m_CChaAttr.ResetChangeFlag();
	pCMainCha->SetLookChangeFlag();
	// SP
	if (m_SFightInit.pCSkillTData->sUseSP > 0)
	{
		if (m_SFightInit.pCSkillTData->sUseSP * sExecTime > pCMainCha->m_CChaAttr.GetAttr(ATTR_SP))
		{
			m_SFightProc.sState |= enumFSTATE_NO_EXPEND;
			NotiSkillSrcToEyeshot(sExecTime);
			return false;
		}
		else
			pCMainCha->setAttr(ATTR_SP, pCMainCha->m_CChaAttr.GetAttr(ATTR_SP) - m_SFightInit.pCSkillTData->sUseSP * sExecTime);
	}

	// 
	Short	sNeedEnergy = m_SFightInit.pCSkillTData->sUseEnergy * sExecTime;
	if (sNeedEnergy > 0)
	{
		SItemGrid	*pGrid;
		for (int i = 0; i < defSKILL_ITEM_NEED_NUM; i++)
		{
			if (m_SFightInit.pCSkillRecord->sConchNeed[i][0] == cchSkillRecordKeyValue)
				break;

			pGrid = &pCMainCha->m_SChaPart.SLink[m_SFightInit.pCSkillRecord->sConchNeed[i][0]];
			if (!g_IsRealItemID(pGrid->sID))
				continue;
			sNeedEnergy -= pGrid->sEnergy[0];
			if (sNeedEnergy <= 0)
				break;
		}

		if (sNeedEnergy > 0) // 
		{
			m_SFightProc.sState |= enumFSTATE_NO_EXPEND;
			NotiSkillSrcToEyeshot(sExecTime);
			return false;
		}
		else
		{
			sNeedEnergy = m_SFightInit.pCSkillTData->sUseEnergy * sExecTime;
			SItemGrid	*pGrid;
			for (int i = 0; i < defSKILL_ITEM_NEED_NUM; i++)
			{
				pGrid = &pCMainCha->m_SChaPart.SLink[m_SFightInit.pCSkillRecord->sConchNeed[i][0]];
				if (!g_IsRealItemID(pGrid->sID))
					continue;
				sNeedEnergy -= pGrid->sEnergy[0];
				if (sNeedEnergy > 0)
					pGrid->SetInstAttr(ITEMATTR_ENERGY, 0);
				else if (sNeedEnergy == 0)
				{
					pGrid->SetInstAttr(ITEMATTR_ENERGY, 0);
					break;
				}
				else
				{
					pGrid->SetInstAttr(ITEMATTR_ENERGY, -1 * sNeedEnergy);
					break;
				}
			}
		}
	}

	//
	if (m_SFightInit.pCSkillRecord->szUse != "0")
		//g_CParser.DoString(m_SFightInit.pCSkillRecord->szUse, enumSCRIPT_RETURN_NONE, 0, enumSCRIPT_PARAM_LIGHTUSERDATA, 1, this->IsCharacter(), enumSCRIPT_PARAM_NUMBER, 1, m_SFightInit.pSSkillGrid->chLv, DOSTRING_PARAM_END);
		g_luaAPI.Call(m_SFightInit.pCSkillRecord->szUse.c_str(), this->IsCharacter(), (int)m_SFightInit.pSSkillGrid->chLv);
	if (m_SFightProc.sState == enumFSTATE_NO_EXPEND)
	{
		NotiSkillSrcToEyeshot(sExecTime);
		return false;
	}

	if (pCMainCha != this->IsCharacter())
		pCMainCha->SynAttrToSelf(enumATTRSYN_ATTACK);
	pCMainCha->SynLook(enumSYN_LOOK_CHANGE);

	return true;
}

void CFightAble::RangeEffect(SFireUnit *pSFireSrc, SubMap *pCMap, Long *plRangeBParam)
{
	CCharacter *pCFightObj;

	if (!pCMap)
		return;

	Long	lEParam[defSKILL_RANGE_EXTEP_NUM];
	for (short i = 0; i < defSKILL_RANGE_EXTEP_NUM; i++)
		lEParam[i] = pSFireSrc->pCSkillTData->sRange[i];
	pCMap->BeginSearchInRange(plRangeBParam, lEParam, true);
	while (pCFightObj = pCMap->GetNextCharacterInRange())
	{
		if (!pCFightObj->IsLiveing()) // 
			continue;

		if (!pCFightObj->IsRightSkillTar(this,
			pSFireSrc->pCSkillRecord->chApplyTarget, pSFireSrc->pCSkillRecord->chTarType, pSFireSrc->pCSkillRecord->chHelpful, true))
			continue;

		pCFightObj->SkillTarEffect(pSFireSrc);
		if (m_SFightProc.sState & enumFSTATE_DIE) // 
		{
			Die();
			return;
		}
		if (pCFightObj->m_SFightProc.sState & enumFSTATE_DIE) // 
		{
			pCFightObj->Die();

			if (pSFireSrc->pCSkillRecord->chPlayTime && pSFireSrc->pCSkillRecord->chApplyType != 2)
			{
				m_CChaAttr.ResetChangeFlag();
				m_CSkillState.ResetChangeFlag();

				m_SFightInit.chTarType = 0;
				m_SFightProc.sState = enumFSTATE_TARGET_DIE;
				NotiSkillSrcToEyeshot();
			}
		}
	}

	if (pSFireSrc->pCSkillTData->sStateParam[0] != SSTATE_NONE)
	{
		pCMap->RangeAddState(pSFireSrc->uchFightID, pSFireSrc->pCFightSrc->GetID(), pSFireSrc->pCFightSrc->GetHandle(),
			pSFireSrc->pCSkillRecord->chApplyTarget, pSFireSrc->pCSkillRecord->chTarType, pSFireSrc->pCSkillRecord->chHelpful,
			pSFireSrc->pCSkillTData->sStateParam);
	}
}

//=============================================================================
// lDist sExecTime 
// truefalse
//=============================================================================
bool CFightAble::SkillGeneral(Long lDist, Short sExecTime) // 
{
	if (!m_SFightInit.pCSkillRecord->chPlayTime) // 
		m_SFightProc.sState |= enumFSTATE_STOP;

	if (IsCharacter()->IsPlayerCha())
		if (!SkillExpend())
			return false;

	if (m_SFightInit.chTarType == 2) // 
	{
		g_SSkillPoint.x = m_SFightInit.lTarInfo1;
		g_SSkillPoint.y = m_SFightInit.lTarInfo2;

		Point SrcPos = GetPos();
		Point TarPos = {m_SFightInit.lTarInfo1, m_SFightInit.lTarInfo2};
		if (SrcPos != TarPos)
			SetAngle(arctan(SrcPos, TarPos));
		NotiSkillSrcToEyeshot(sExecTime);

		if (m_SFightInit.pCSkillTData->sRange[0] == enumRANGE_TYPE_STICK || m_SFightInit.pCSkillTData->sRange[0] == enumRANGE_TYPE_FAN)
		{
			m_SFightProc.lERangeBParam[0] = GetPos().x;
			m_SFightProc.lERangeBParam[1] = GetPos().y;
		}
		else
		{
			m_SFightProc.lERangeBParam[0] = m_SFightInit.lTarInfo1;
			m_SFightProc.lERangeBParam[1] = m_SFightInit.lTarInfo2;
		}
		m_SFightProc.lERangeBParam[2] = GetAngle();

		SFireUnit SFire;
#ifdef defPROTOCOL_HAVE_PACKETID
		SFire.ulPacketID = m_ulPacketID;
#endif
		SFire.uchFightID = m_uchFightID;
		SFire.pCFightSrc = this;
		SFire.ulID = GetID();
		SFire.SSrcPos = GetPos();
		SFire.lTarInfo1 = m_SFightInit.lTarInfo1;
		SFire.lTarInfo2 = m_SFightInit.lTarInfo2;
		SFire.pCSkillRecord = m_SFightInit.pCSkillRecord;
		SFire.pCSkillTData = m_SFightInit.pCSkillTData;
		SFire.sExecTime = sExecTime;

		if (m_SFightInit.pCSkillRecord->sSkySpd > 0)
		{
			uLong ulLeftTime = lDist * 1000 / m_SFightInit.pCSkillRecord->sSkySpd;
			g_CTimeSkillMgr.Add(&SFire, ulLeftTime, m_submap, &TarPos, m_SFightProc.lERangeBParam);
		}
		else if (m_SFightInit.pCSkillRecord->sSkySpd == 0)// 
			RangeEffect(&SFire, m_submap, m_SFightProc.lERangeBParam);
		else {} // 
	}
	else if (m_SFightInit.chTarType == 1) // ID
	{
		Entity	*pTarObj = g_pGameApp->IsMapEntity(m_SFightInit.lTarInfo1, m_SFightInit.lTarInfo2);
		if (!pTarObj) // 
		{
			m_SFightProc.sState = enumFSTATE_TARGET_NO;
			NotiSkillSrcToEyeshot();
			return false;
		}

		CCharacter	*pObjCha = pTarObj->IsCharacter();

		if (!pObjCha->IsRightSkillTar(this,
			m_SFightInit.pCSkillRecord->chApplyTarget, m_SFightInit.pCSkillRecord->chTarType, m_SFightInit.pCSkillRecord->chHelpful)) // 
		{
			m_SFightProc.sState = enumFSTATE_TARGET_IMMUNE;
			NotiSkillSrcToEyeshot();
			m_SFightInit.chTarType = 0;
			IsCharacter()->m_AITarget = 0;
			IsCharacter()->m_pHate->ClearHarmRecByCha(pObjCha);
			return false;
		}

		if (GetPos() != pTarObj->GetPos())
			SetAngle(arctan(GetPos(), pTarObj->GetPos()));
		NotiSkillSrcToEyeshot(sExecTime);

		m_SFightProc.lERangeBParam[0] = pTarObj->GetPos().x;
		m_SFightProc.lERangeBParam[1] = pTarObj->GetPos().y;
		m_SFightProc.lERangeBParam[2] = GetAngle();

		SFireUnit SFire{};
#ifdef defPROTOCOL_HAVE_PACKETID
		SFire.ulPacketID = m_ulPacketID;
#endif
		SFire.uchFightID = m_uchFightID;
		SFire.pCFightSrc = this;
		SFire.ulID = GetID();
		SFire.SSrcPos = GetPos();
		SFire.lTarInfo1 = m_SFightInit.lTarInfo1;
		SFire.lTarInfo2 = m_SFightInit.lTarInfo2;
		SFire.pCSkillRecord = m_SFightInit.pCSkillRecord;
		SFire.pCSkillTData = m_SFightInit.pCSkillTData;
		SFire.sExecTime = sExecTime;

		if (m_SFightInit.pCSkillRecord->chApplyType == 3) // 
			RangeEffect(&SFire, m_submap, m_SFightProc.lERangeBParam);
		else
		{
			bool	bTarIsLive = pObjCha->IsLiveing();
			pObjCha->SkillTarEffect(&SFire);


			if (m_SFightProc.sState & enumFSTATE_DIE) // 
			{
				Die();
				return true;
			}

			if (pObjCha->m_SFightProc.sState & enumFSTATE_DIE) // 
			{
				m_SFightInit.chTarType = 0;
				if (m_SFightProc.sState == enumFSTATE_ON)
				{
					m_CChaAttr.ResetChangeFlag();
					m_CSkillState.ResetChangeFlag();

					m_SFightProc.sState = enumFSTATE_TARGET_DIE;
					NotiSkillSrcToEyeshot();
				}

				if (bTarIsLive)
					pObjCha->Die();
			}
		}
	}

	return true;
}

// 
CCharacter* CFightAble::SkillPopBoat(Long lPosX, Long lPosY, Short sDir) // 
{
	CCharacter	*pCCha = 0;

	Short	sUnitWidth, sUnitHeight;
	Short	sUnitX, sUnitY;
	uShort	usAreaAttr;

	m_submap->GetTerrainCellSize(&sUnitWidth, &sUnitHeight);
	sUnitX = static_cast<Short>(lPosX / sUnitWidth);
	sUnitY = static_cast<Short>(lPosY / sUnitHeight);
	m_submap->GetTerrainCellAttr(sUnitX, sUnitY, usAreaAttr);

	if (g_IsSea(usAreaAttr)) // 
	{
		Point		SPos = {lPosX, lPosY};
		if (sDir == -1)
			sDir = GetAngle();
		pCCha = GetSubMap()->ChaSpawn(302, enumCHACTRL_PLAYER, sDir, &SPos, true, GetName(), 0);
		if (pCCha)
		{
			pCCha->SetShip(g_pGameApp->m_CabinPool.Get());

			SSkillGrid	SSkillCont;
			SSkillCont.chState = enumSUSTATE_ACTIVE;
			SSkillCont.sID = 39;	// 
			SSkillCont.chLv = 1;
			pCCha->m_CSkillBag.Add(&SSkillCont);

			pCCha->SetShipMaster(pCCha->IsAttachable());
		}
	}

	return pCCha;
}

// 
bool CFightAble::SkillPopBoat(CCharacter *pCBoat, Long lPosX, Long lPosY, Short sDir) // 
{
	if (GetSubMap())
	{
		if (sDir == -1)
			sDir = GetAngle();
		pCBoat->SetAngle(sDir);

		Square	SEntShape = {{lPosX, lPosY}, pCBoat->GetRadius()};

		SubMap	*pCTempMap = pCBoat->GetSubMap();
		pCBoat->SetSubMap(GetSubMap());
		if (!pCBoat->GetSubMap()->EnsurePos(&SEntShape, pCBoat))
		{
			pCBoat->SetSubMap(pCTempMap);
			return false;
		}
		pCBoat->SetSideID(IsCharacter()->GetSideID());
		SEntShape.centre = pCBoat->GetPos();
		if (!pCBoat->GetSubMap()->Enter(&SEntShape, pCBoat))
			return false;
		pCBoat->SetBirthMap(pCBoat->GetSubMap()->GetName());
	}

	pCBoat->SetShipMaster(pCBoat->IsAttachable());

	return true;
}

// 
bool CFightAble::SkillInBoat(CCharacter *pCBoat) // 
{
	// 
	RemoveOtherSkillState();

	// 
	Point	SUpPos = GetPos();
	if (GetSubMap())
	{
		GetSubMap()->MoveTo(this, pCBoat->GetPos());
		NotiChangeMainCha(pCBoat->GetID());
	}
	if (m_pCPlayer == pCBoat->GetPlayer())
		m_pCPlayer->SetCtrlCha(pCBoat);
	pCBoat->GetShip()->Add(this);
	SetShipMaster(pCBoat->IsAttachable());
	if (GetSubMap())
	{
		BreakAction();
		m_CSkillState.Reset();
		m_submap->GoOut(this);
		SetPos(SUpPos), m_lastpos = SUpPos;

		m_CSkillBag.SetChangeFlag(false);
		pCBoat->SkillRefresh();
		IsCharacter()->SynSkillBag(enumSYN_SKILLBAG_MODI);
		m_pCPlayer->SetLoginCha(enumLOGIN_CHA_BOAT, (long)pCBoat->getAttr(ATTR_BOAT_DBID));
		pCBoat->SynPKCtrl();
	}

	return true;
}

// 
bool CFightAble::SkillOutBoat(Long lPosX, Long lPosY, Short sDir) // 
{
	// 
	RemoveOtherSkillState();

	CAttachable	*pOutObj = this;
	CAttachable	*pCShipM = GetShipMaster();
	if (!pCShipM)
		return false;

	if (pCShipM == this) // 
	{
		if (!(pOutObj = GetShip()->GetLeader()))
			return false;
	}

	SubMap	*pCMap = pCShipM->GetSubMap();
	Point	STarPos = {lPosX, lPosY};
	Square	SShape = {STarPos, GetRadius()};

	SubMap	*pCTempMap = pOutObj->GetSubMap();
	pOutObj->SetSubMap(pCMap);
	if (!pOutObj->GetSubMap()->EnsurePos(&SShape, pOutObj))
	{
		pOutObj->SetSubMap(pCTempMap);
		return false;
	}

	pOutObj->IsCharacter()->SetSideID(IsCharacter()->GetSideID());
	CPlayer	*pCPlayer = pOutObj->GetPlayer();
	if (sDir == -1)
		sDir = GetAngle();
	pOutObj->SetAngle(sDir);
	bool	bEntSuc = pCMap->Enter(&SShape, pOutObj);
	if (!bEntSuc)
		return false;
	else
	{
		// 
		pCMap->MoveTo(this, pOutObj->GetPos());
		NotiChangeMainCha(pOutObj->GetID());
		if (pCPlayer == pCShipM->GetPlayer())
			m_pCPlayer->SetCtrlCha(pOutObj->IsCharacter());
		pOutObj->SetShipMaster(0);
		//

		CCharacter	*pCCha = pOutObj->IsCharacter();
		pCCha->m_CSkillBag.SetChangeFlag(false);
		pCCha->SkillRefresh();
		pCCha->SynSkillBag(enumSYN_SKILLBAG_MODI);
		pCCha->SetBirthMap(pCMap->GetName());
		pCPlayer->SetLoginCha(enumLOGIN_CHA_MAIN, 0);

		pCCha->SynPKCtrl();
	}

	return true;
}

// 
bool CFightAble::SkillPushBoat(CCharacter* pCBoat, bool bFree) // 
{
	if (bFree)
		g_pGameApp->m_CabinPool.Release(pCBoat->GetShip());

	pCBoat->BreakAction();
	pCBoat->m_CSkillState.Reset();
	pCBoat->GetSubMap()->GoOut(pCBoat);
	pCBoat->SetBirthMap("");

	if (bFree)
		pCBoat->Free();

	return true;
}

void CFightAble::NotiChangeMainCha(uLong ulTargetID)
{
	//  :     std::variant
	net::msg::McCharacterActionMessage msg;
	msg.worldId = m_ID;
	msg.packetId = m_ulPacketID;
	msg.actionType = net::msg::ActionType::CHANGE_CHA;
	msg.data = net::msg::ActionChangeChaData{ static_cast<int64_t>(ulTargetID) };
	auto pk = net::msg::serialize(msg);

	ReflectINFof(this, pk);

	// log
	//
}

bool CFightAble::IsRightSkill(CSkillRecord *pSkill)
{
	if (IsCharacter()->IsPlayerCha())
	{
		if (IsCharacter()->IsBoat())
		{
			if (pSkill->chSrcType == enumSKILL_SRC_BOAT)
				return true;
			return false;
		}
		else
		{
			if (pSkill->chSrcType == enumSKILL_SRC_HUMAN)
				return true;
			return false;
		}
	}
	return true;
}

bool CFightAble::IsRightSkillSrc(Char chSkillEffType)
{
	if ((GetAreaAttr() & enumAREA_TYPE_NOT_FIGHT) && (chSkillEffType != enumSKILL_EFF_HELPFUL)) // 
		return false;
	else
		return true;
}

bool CFightAble::IsRightSkillTar(CFightAble *pSkillSrc, Char chSkillObjType, Char chSkillObjHabitat, Char chSkillEffType, bool bIncHider)
{
	//if (GetPlayer() && GetPlayer()->GetGMLev() > 0) // GM
	//	return false;
	if (!bIncHider)
		if (IsCharacter()->IsHide())
			return false;
	if (!IsCharacter()->GetActControl(enumACTCONTROL_INVINCIBLE))
		return false;

	bool	bIsTeammate = pSkillSrc->IsTeammate(this);
	bool	bIsFriend = pSkillSrc->IsFriend(this);

	int nCheckRet = g_IsRightSkillTar((long)m_CChaAttr.GetAttr(ATTR_CHATYPE), !IsLiveing(), IsCharacter()->GetActControl(enumACTCONTROL_BEUSE_SKILL), GetAreaAttr(),
						(long)pSkillSrc->m_CChaAttr.GetAttr(ATTR_CHATYPE), chSkillObjType, chSkillObjHabitat, chSkillEffType, bIsTeammate, bIsFriend, pSkillSrc == this);
	if (nCheckRet != enumESKILL_SUCCESS)
		return false;

	return true;
}

inline bool CFightAble::IsTeammate(CFightAble *pCTar)
{
	CPlayer	*pCPly1 = GetPlayer();
	CPlayer	*pCPly2 = 0;
	if (pCTar)
		pCPly2 = pCTar->GetPlayer();
	CCharacter	*pCCha1 = IsCharacter();
	CCharacter	*pCCha2 = 0;
	if (pCTar)
		pCCha2 = pCTar->IsCharacter();

	//if (g_CParser.DoString("is_teammate", enumSCRIPT_RETURN_NUMBER, 1, enumSCRIPT_PARAM_LIGHTUSERDATA, 2, pCCha1, pCCha2, DOSTRING_PARAM_END))
	//	return g_CParser.GetReturnNumber(0) != 0 ? true : false;
	//else
	//	return false;
	auto val = g_luaAPI.CallR<int>("is_teammate", pCCha1, pCCha2);
	return val.value_or(0) != 0;
}

bool CFightAble::IsFriend(CFightAble *pCTar)
{
	CPlayer	*pCPly1 = GetPlayer();
	CPlayer	*pCPly2 = 0;
	if (pCTar)
		pCPly2 = pCTar->GetPlayer();
	CCharacter	*pCCha1 = IsCharacter();
	CCharacter	*pCCha2 = 0;
	if (pCTar)
		pCCha2 = pCTar->IsCharacter();

	//if (g_CParser.DoString("is_friend", enumSCRIPT_RETURN_NUMBER, 1, enumSCRIPT_PARAM_LIGHTUSERDATA, 2, pCCha1, pCCha2, DOSTRING_PARAM_END))
	//	return g_CParser.GetReturnNumber(0) != 0 ? true : false;
	//else
	//	return false;
	auto val = g_luaAPI.CallR<int>("is_friend", pCCha1, pCCha2);
	return val.value_or(0) != 0;
}

//=============================================================================
// 
//=============================================================================
void CFightAble::CountLevel()
{
	if (!IsLiveing())
		return;

	LONG32	lOldLevel, lCurLevel;
	unsigned int	lCurExp = (unsigned int)m_CChaAttr.GetAttr(ATTR_CEXP);

	lOldLevel = lCurLevel = m_CChaAttr.GetAttr(ATTR_LV);
	CLevelRecord	*pCLvRec = 0, *pNLvRec = 0;
	while (1)
	{
		pCLvRec = GetLevelRecordInfo((int)lCurLevel + 1);
		if (!pCLvRec)
		{
			ToLogService("common", "Unable to find Lv{} record", lCurLevel + 1);
			break;
		}
		if (lCurExp >= pCLvRec->ulExp)
		{
			lCurLevel++;
			setAttr(ATTR_LV, lCurLevel);
			setAttr(ATTR_CLEXP, pCLvRec->ulExp);
			pNLvRec = GetLevelRecordInfo((int)lCurLevel + 1);
			if (pNLvRec)
			{
				setAttr(ATTR_NLEXP, pNLvRec->ulExp);
			}
			//g_CParser.DoString("Shengji_Shuxingchengzhang", enumSCRIPT_RETURN_NONE, 0, enumSCRIPT_PARAM_LIGHTUSERDATA, 1, this->IsCharacter(), DOSTRING_PARAM_END); //
			g_luaAPI.Call("Shengji_Shuxingchengzhang", this->IsCharacter());
			OnLevelUp( (USHORT)lCurLevel );
		}
		else
			break;
	}
}

//=============================================================================
// 
//=============================================================================
void CFightAble::CountSailLevel()
{
	if (!IsLiveing())
		return;
	Long	lOldLevel, lCurLevel;
	Long	lCurExp = (long)m_CChaAttr.GetAttr(ATTR_CSAILEXP);

	lOldLevel = lCurLevel = (long)m_CChaAttr.GetAttr(ATTR_SAILLV);
	CSailLvRecord	*pCLvRec = 0, *pNLvRec = 0;
	while (1)
	{
		pCLvRec = GetSailLvRecordInfo(lCurLevel + 1);
		if (!pCLvRec)
		{
			break;
		}
		if ((uLong)lCurExp >= pCLvRec->ulExp)
		{
			lCurLevel++;
			setAttr(ATTR_SAILLV, lCurLevel);
			setAttr(ATTR_CLV_SAILEXP, pCLvRec->ulExp);
			pNLvRec = GetSailLvRecordInfo(lCurLevel + 1);
			if (pNLvRec)
			{
				setAttr(ATTR_NLV_SAILEXP, pNLvRec->ulExp);
			}
			//g_CParser.DoString("Saillv_Up", enumSCRIPT_RETURN_NONE, 0, enumSCRIPT_PARAM_LIGHTUSERDATA, 1, this->IsCharacter(), DOSTRING_PARAM_END); //
			g_luaAPI.Call("Saillv_Up", this->IsCharacter());
			OnSailLvUp( (USHORT)lCurLevel );
		}
		else
			break;
	}
}

//=============================================================================
// 
//=============================================================================
void CFightAble::CountLifeLevel()
{
	if (!IsLiveing())
		return;
	Long	lOldLevel, lCurLevel;
	Long	lCurExp = m_CChaAttr.GetAttr(ATTR_CLIFEEXP);

	lOldLevel = lCurLevel = (long)m_CChaAttr.GetAttr(ATTR_LIFELV);
	CLifeLvRecord	*pCLvRec = 0, *pNLvRec = 0;
	while (1)
	{
		pCLvRec = GetLifeLvRecordInfo(lCurLevel + 1);
		if (!pCLvRec)
		{
			break;
		}
		if (lCurExp >= pCLvRec->ulExp)
		{
			lCurLevel++;
			setAttr(ATTR_LIFELV, lCurLevel);
			setAttr(ATTR_CLV_LIFEEXP, pCLvRec->ulExp);
			pNLvRec = GetLifeLvRecordInfo(lCurLevel + 1);
			if (pNLvRec)
			{
				setAttr(ATTR_NLV_LIFEEXP, pNLvRec->ulExp);
			}
			//g_CParser.DoString("Lifelv_Up", enumSCRIPT_RETURN_NONE, 0, enumSCRIPT_PARAM_LIGHTUSERDATA, 1, this->IsCharacter(), DOSTRING_PARAM_END); //
			g_luaAPI.Call("Lifelv_Up", this->IsCharacter());
			OnLifeLvUp( (USHORT)lCurLevel );
		}
		else
			break;
	}
}

Long CalculateLevelByExp(Long lretLv, uLong t) /* by value */
{
	CLevelRecord* pCLvRec = 0;
	while (true)
	{
		pCLvRec = GetLevelRecordInfo((int)lretLv + 1);
		if (!pCLvRec)
			break;
		if (t < pCLvRec->ulExp)
			break;
		lretLv++;
		t -= pCLvRec->ulExp;
	}
	return lretLv;
}

void CFightAble::AddExp(dbc::uLong ulAddExp)
{
	//g_CParser.DoString("EightyLv_ExpAdd", enumSCRIPT_RETURN_NONE, 0, enumSCRIPT_PARAM_LIGHTUSERDATA, 1, this->IsCharacter(), enumSCRIPT_PARAM_NUMBER_UNSIGNED, 1, ulAddExp, DOSTRING_PARAM_END); // 
	if (!this || !GetPlayer())
		return;

	uLong lCurExp = (uLong)m_CChaAttr.GetAttr(ATTR_CEXP);
	Long lCurLevel = m_CChaAttr.GetAttr(ATTR_LV);

	CLevelRecord* pCLvRec = GetLevelRecordInfo((int)lCurLevel + 1);
	if (!pCLvRec)
		return;

	if (!GetPlayer()->GetCtrlCha()->IsBoat())
	{
		if (lCurLevel < 80)
		{
			uLong lTotalExp = lCurExp + ulAddExp;
			Long lTotalLevel = CalculateLevelByExp(lCurLevel, lTotalExp);
			if (lTotalLevel >= 80)
			{
				uLong ulNeed = GetLevelRecordInfo(80)->ulExp - lCurExp;
				ulAddExp = /* needed for 80 */ ulNeed + /* remaining / 50 */((ulAddExp-ulNeed) / 50);
			}
		}
		else
		{
			ulAddExp = floor(ulAddExp/50);
		}
	}
	lCurExp = floor(lCurExp+ulAddExp);
	setAttr(ATTR_CEXP, lCurExp);
	//m_CChaAttr.SetAttr(ATTR_CEXP, lCurExp);
}

bool CFightAble::AddExpAndNotic(dbc::Long lAddExp, Short sNotiType)
{
	m_CChaAttr.ResetChangeFlag();
	AddExp(lAddExp);
	SynAttr(sNotiType);

	return true;
}

void CFightAble::SpawnResource(CCharacter* pCAtk, dbc::Long lSkillLv)
{
		Long i = 0;
	for( ; i < defCHA_INIT_ITEM_NUM; i++ )
	{
		if( m_pCChaRecord->lItem[i][1] <= 0 )
			break;
	}

	// 
	if( i < 1 ) return;

	g_chItemFall[0] = 0;
	// MFMFdefCHA_INIT_ITEM_NUMCSetItemFall()
	lua_getglobal( g_pLuaState, "Check_SpawnResource" );
	if( !lua_isfunction( g_pLuaState, -1 ) )
	{
		lua_pop(g_pLuaState, 1);
		//LG( "", "Check_SpawnResource" );
		return;
	}

	luabridge::push( g_pLuaState, static_cast<CCharacter*>(pCAtk) );
	luabridge::push( g_pLuaState, this->IsCharacter() );
	lua_pushnumber( g_pLuaState, lSkillLv );
	lua_pushnumber( g_pLuaState, i ); // 
	for( int n = 0; n < i; n++ )
	{
		lua_pushnumber( g_pLuaState, m_pCChaRecord->lItem[n][1] );
	}

	int nStatus = lua_pcall( g_pLuaState, 4 + i, 0, 0 );
	if( nStatus )
	{
		//LG( "", "Check_SpawnResource" );
		lua_callalert(g_pLuaState, nStatus);
		lua_settop(g_pLuaState, 0);
		return;
	}
	lua_settop(g_pLuaState, 0);

	CItem	*pCItem;
	for (int i = 0; i < g_chItemFall[0]; i++)
	{
		//LG("", "\t%d\n", m_pCChaRecord->lItem[g_chItemFall[i + 1] - 1][0]);
		// 
		SItemGrid GridContent((Short)m_pCChaRecord->lItem[g_chItemFall[i + 1] - 1][0], 1);
		ItemInstance(enumITEM_INST_MONS, &GridContent);
		// 
		CCharacter	*pCCtrlCha = IsCharacter()->GetPlyCtrlCha(), *pCAtkMainCha = pCAtk->GetPlyMainCha();
		Long	lPosX, lPosY;
		pCCtrlCha->GetTrowItemPos(&lPosX, &lPosY);
		pCItem = pCCtrlCha->GetSubMap()->ItemSpawn(&GridContent, lPosX, lPosY, enumITEM_APPE_MONS, pCCtrlCha->GetID(), pCAtkMainCha->GetID(), pCAtkMainCha->GetHandle());
		if (pCItem)
			pCItem->SetProtType(enumITEM_PROT_TEAM);
	}
}

bool CFightAble::GetTrowItemPos(Long *plPosX, Long *plPosY)
{
	Point	Pos;
	CCharacter	*pCCtrlCha = IsCharacter()->GetPlyCtrlCha();
	SubMap	*pCMap = pCCtrlCha->GetSubMapFar();
	if (!pCMap)
		return false;

	// 
	Pos = pCCtrlCha->GetShape().centre;
	Pos.move(rand() % 360, 150);
	if (!pCMap->IsValidPos(Pos.x, Pos.y))
	{
		*plPosX = pCCtrlCha->GetPos().x;
		*plPosY = pCCtrlCha->GetPos().y;
		return false;
	}

	uShort	usAreaAttr = pCMap->GetAreaAttr(Pos);

	if (g_IsLand(usAreaAttr) != g_IsLand(GetAreaAttr()))
		Pos = pCCtrlCha->GetPos();
	else
	{
		if (pCMap->IsBlock(Short(Pos.x / pCMap->GetBlockCellWidth()), Short(Pos.y / pCMap->GetBlockCellHeight())))
			Pos = pCCtrlCha->GetPos();
	}
	//

	*plPosX = Pos.x;
	*plPosY = Pos.y;

	return true;
}

void CFightAble::ItemCount(CCharacter* pAtk)
{
		CCharacter* pCItemHCha = pAtk;
	if (m_pCItemHostObj)
		pCItemHCha = m_pCItemHostObj->IsCharacter();

	CCharacter* pThis = this->IsCharacter();
	CCharacter* pCCtrlCha = pThis->GetPlyCtrlCha(), * pCItemHMainCha = pCItemHCha->GetPlyMainCha();

	if (pThis->IsBoat() && pThis->IsPlayerCha()) // 
	{
		//Short	sItemNum = pThis->m_CKitbag.GetUseGridNum();
		//SItemGrid	*pCThrow;
		//Long	lPosX, lPosY;
		//for (Short	sNo = 0; sNo < sItemNum; sNo++)
		//{
		//	pCThrow = pThis->m_CKitbag.GetGridContByNum(sNo);
		//	if (!pCThrow)
		//		continue;

		//	GetTrowItemPos(&lPosX, &lPosY);
		//	GetSubMap()->ItemSpawn(pCThrow, lPosX, lPosY, enumITEM_APPE_MONS, GetID(), pCItemHMainCha->GetID(), pCItemHMainCha->GetHandle(), -1);
		//}
		//pThis->m_CKitbag.Reset();
		return;
	}

	CItem* pCItem;
	Long	lItem[defCHA_INIT_ITEM_NUM], lIndex[defCHA_INIT_ITEM_NUM];
	Long	lItemNum;
	const char* szItemScript = "Check_Baoliao";

	// 
	g_chItemFall[0] = 0;
	MPTimer t; t.Begin();
	lua_getglobal(g_pLuaState, szItemScript);
	if (!lua_isfunction(g_pLuaState, -1)) // 
	{
		lua_pop(g_pLuaState, 1);
		return;
	}
	luabridge::push(g_pLuaState, static_cast<CCharacter*>(pCItemHCha));
	luabridge::push(g_pLuaState, static_cast<CCharacter*>(pThis));
	lItemNum = 0;
	for (Long i = 0; i < defCHA_INIT_ITEM_NUM; i++)
	{
		if (m_pCChaRecord->lItem[i][1] == cchChaRecordKeyValue)
			break;
		lua_pushnumber(g_pLuaState, m_pCChaRecord->lItem[i][1]);
		lItemNum++;
	}
	if (lItemNum < 1)
	{
		lua_settop(g_pLuaState, 0);
		return;
	}
	int nState = lua_pcall(g_pLuaState, 2 + lItemNum, LUA_MULTRET, 0);
	if (nState != 0)
	{
		ToLogService("lua", LogLevel::Error, "DoString {}", szItemScript);
		lua_callalert(g_pLuaState, nState);
		lua_settop(g_pLuaState, 0);
		return;
	}
	lua_settop(g_pLuaState, 0);
	DWORD dwEndTime = t.End();
	if (dwEndTime > 20)
		//LG("script_time", "[%s] time = %d\n", szItemScript, dwEndTime);
		ToLogService("lua", LogLevel::Trace, "script [{}]cost time too long, time = {}", szItemScript, dwEndTime);

	Long	lFallNum = g_chItemFall[0];
	if (lFallNum > lItemNum)
		//LG("", " %s (%u)", GetName(), lFallNum);
		ToLogService("errors", LogLevel::Error, "character {} fall res number ({}) error", GetName(), lFallNum);
	else
	{
		for (int i = 0; i < lFallNum; i++)
			lItem[i] = g_chItemFall[i + 1];
		for (int i = 0; i < lFallNum; i++)
		{
			long itemID = m_pCChaRecord->lItem[lItem[i] - 1][0];
			// for now, ignore all drops associated with fusion scrolls.
			// Later, will handle all drops/possession from LUA
			if (itemID == 453)
				continue;

			// 
			SItemGrid GridContent((Short)m_pCChaRecord->lItem[lItem[i] - 1][0], 1);
			ItemInstance(enumITEM_INST_MONS, &GridContent);
			// 
			Long	lPosX, lPosY;
			pCCtrlCha->GetTrowItemPos(&lPosX, &lPosY);
			pCItem = pCCtrlCha->GetSubMap()->ItemSpawn(&GridContent, lPosX, lPosY, enumITEM_APPE_MONS, pCCtrlCha->GetID(), pCItemHMainCha->GetID(), pCItemHMainCha->GetHandle());
			if (pCItem)
				pCItem->SetProtType(enumITEM_PROT_TEAM);
		}
	}

	// 
	g_chItemFall[0] = 0;
	t.Begin();
	lua_getglobal(g_pLuaState, szItemScript);
	if (!lua_isfunction(g_pLuaState, -1)) // 
	{
		lua_pop(g_pLuaState, 1);
		return;
	}
	luabridge::push(g_pLuaState, static_cast<CCharacter*>(pCItemHCha));
	luabridge::push(g_pLuaState, static_cast<CCharacter*>(pThis));
	lItemNum = 0;
	for (Long i = 0; i < defCHA_INIT_ITEM_NUM; i++)
	{
		if (m_pCChaRecord->lTaskItem[i][1] == cchChaRecordKeyValue)
			break;
		if (pCItemHCha->IsMisNeedItem((uShort)m_pCChaRecord->lTaskItem[i][0]))
		{
			lua_pushnumber(g_pLuaState, m_pCChaRecord->lTaskItem[i][1]);
			lIndex[lItemNum] = i;
			lItemNum++;
		}
	}
	if (lItemNum < 1)
	{
		lua_settop(g_pLuaState, 0);
		return;
	}
	nState = lua_pcall(g_pLuaState, 2 + lItemNum, LUA_MULTRET, 0);
	if (nState != 0)
	{
		ToLogService("lua", LogLevel::Error, "DoString {}", szItemScript);
		lua_callalert(g_pLuaState, nState);
		lua_settop(g_pLuaState, 0);
		return;
	}
	lua_settop(g_pLuaState, 0);
	dwEndTime = t.End();
	if (dwEndTime > 20)
		//LG("script_time", "[%s] time = %d\n", szItemScript, dwEndTime);
		ToLogService("lua", LogLevel::Trace, "script[{}]cost time too long, time = {}", szItemScript, dwEndTime);

	lFallNum = g_chItemFall[0];
	if (lFallNum > lItemNum)
		//LG("", " %s (%u)", GetName(), lFallNum);
		ToLogService("errors", LogLevel::Error, "roll {} fall task res number ({})error", GetName(), lFallNum);
	else
	{
		for (int i = 0; i < lFallNum; i++)
			lItem[i] = g_chItemFall[i + 1];
		for (int i = 0; i < lFallNum; i++)
		{
			// 
			SItemGrid GridContent((Short)m_pCChaRecord->lTaskItem[lIndex[lItem[i] - 1]][0], 1);
			ItemInstance(enumITEM_INST_MONS, &GridContent);
			// 
			Long	lPosX, lPosY;
			pCCtrlCha->GetTrowItemPos(&lPosX, &lPosY);
			pCItem = pCCtrlCha->GetSubMap()->ItemSpawn(&GridContent, lPosX, lPosY, enumITEM_APPE_MONS, pCCtrlCha->GetID(), pCItemHMainCha->GetID(), pCItemHMainCha->GetHandle(), -1);
		}
	}
}

void CFightAble::ItemInstance(Char chType, SItemGrid* pGridContent, BOOL isTradable, LONG expiration)
{
		if (!pGridContent)
			return;
	CItemRecord* pCItemRec;
	pCItemRec = GetItemRecordInfo(pGridContent->sID);
	if (!pCItemRec)
		return;

	//char szItemInstLog[256] = "";

	char szItemInstLog[256];
	strncpy(szItemInstLog, RES_STRING(GM_FIGHTABLE_CPP_00003), 256 - 1);

	pGridContent->sEndure[1] = g_itemAttrMap[pGridContent->sID].GetAttr(ITEMATTR_MAXURE, false);
	pGridContent->sEndure[0] = pGridContent->sEndure[1];
	pGridContent->sEnergy[1] = g_itemAttrMap[pGridContent->sID].GetAttr(ITEMATTR_MAXENERGY, false);
	pGridContent->sEnergy[0] = pGridContent->sEnergy[1];
	pGridContent->bItemTradable = isTradable == 1 ? true : false;
	pGridContent->expiration = expiration;

	pGridContent->SetInstAttrInvalid();

	if (pCItemRec->chInstance == 0)
		goto ItemInstanceEnd;
	{
		int	nAttrPos = 0;
		//int nRetNum = 15;
		//if (!g_CParser.DoString("Creat_Item", enumSCRIPT_RETURN_NUMBER, nRetNum, enumSCRIPT_PARAM_ITEMGRID_PTR, 1, pGridContent, enumSCRIPT_PARAM_NUMBER, 3, pCItemRec->sType, pCItemRec->sNeedLv, chType, DOSTRING_PARAM_END))
		//	goto ItemInstanceEnd;
		auto creatResult = g_luaAPI.CallMulti("Creat_Item", pGridContent, (int)pCItemRec->sType, (int)pCItemRec->sNeedLv, (int)chType);
		if (creatResult.hasFailed() || creatResult.size() == 0)
			goto ItemInstanceEnd;

		short sMin, sMax;
		int nRetID = 0;
		//int nAttrNum = g_CParser.GetReturnNumber(0);
		int nAttrNum = creatResult[0].cast<int>().value();
		int	nAttrID, nAttr;
		for (int i = 0; i < nAttrNum; i++)
		{
			nRetID = i * 2 + 1;
			//nAttrID = g_CParser.GetReturnNumber(nRetID);
			//nAttr = g_CParser.GetReturnNumber(nRetID + 1);
			nAttrID = creatResult[nRetID].cast<int>().value();
			nAttr = creatResult[nRetID + 1].cast<int>().value();
			sMin = g_itemAttrMap[pGridContent->sID].GetAttr(nAttrID, false);
			sMax = g_itemAttrMap[pGridContent->sID].GetAttr(nAttrID, true);
			if (nAttrID == ITEMATTR_MAXURE)
			{
				if (nAttr < 0 || nAttr > 100) // 
				{
					ToLogService("errors", LogLevel::Error, "instantiation item: number {}, name {}, type {}, requirement grade {}, instantiation type {}, attribute error, attribute number {}, value {}",
						pCItemRec->lID, pCItemRec->szName, static_cast<int>(pCItemRec->sType), static_cast<int>(pCItemRec->sNeedLv), static_cast<int>(chType), nAttrID, nAttr);
					continue;
				}
				pGridContent->sEndure[1] = sMin + (sMax - sMin) * nAttr / 100;
				pGridContent->sEndure[0] = pGridContent->sEndure[1];
			}
			else if (nAttrID == ITEMATTR_MAXENERGY) // 
			{
				if (nAttr < 0 || nAttr > 1000) // 
				{
					ToLogService("errors", LogLevel::Error, "instantiation item: number {}, name {}, type {}, requirement grade {}, instantiation type {}, attribute error, attribute number {}, value {}",
						pCItemRec->lID, pCItemRec->szName, static_cast<int>(pCItemRec->sType), static_cast<int>(pCItemRec->sNeedLv), static_cast<int>(chType), nAttrID, nAttr);
					continue;
				}
				pGridContent->sEnergy[1] = sMin + (sMax - sMin) * nAttr / 100; // 1000
				pGridContent->sEnergy[0] = pGridContent->sEnergy[1];
			}
			else
			{
				if (nAttrPos < defITEM_INSTANCE_ATTR_NUM)
				{
					if (nAttr < 0 || nAttr > 100) // 
					{
						ToLogService("errors", LogLevel::Error, "instantiation item: number {}, name {}, type {}, requirement grade {}, instantiation type {}, attribute error, attribute number {}, value {}",
							pCItemRec->lID, pCItemRec->szName, static_cast<int>(pCItemRec->sType), static_cast<int>(pCItemRec->sNeedLv), static_cast<int>(chType), nAttrID, nAttr);
						continue;
					}
					pGridContent->sInstAttr[nAttrPos][0] = nAttrID;
					pGridContent->sInstAttr[nAttrPos][1] = sMin + (sMax - sMin) * nAttr / 100;
					nAttrPos++;
				}
				else
					break;
			}
		}
		if (nAttrPos < defITEM_INSTANCE_ATTR_NUM)
			pGridContent->sInstAttr[nAttrPos][0] = 0;

	}

ItemInstanceEnd:
	return;
}

void CFightAble::OnSkillState(DWORD dwCurTick)
{
	if (!IsLiveing())
		return;

	if (m_CSkillState.GetStateNum() > 0)
	{
		Entity	*pCEnt;
		CCharacter	*pCCha;
		CCharacter	*pSrcMainC = 0;

		IsCharacter()->GetPlyMainCha()->SetLookChangeFlag();
		m_CChaAttr.ResetChangeFlag();
		m_CSkillState.ResetChangeFlag();
		CSkillStateRecord	*pCSStateRec = 0;
		SSkillStateUnit		*pSStateUnit;
		Short	sExecTime = 0;
		Long	lOldHP;
		bool	bIsDie;
		m_CSkillState.BeginGetState();
		while (pSStateUnit = m_CSkillState.GetNextState())
		{
			pCSStateRec = GetCSkillStateRecordInfo(pSStateUnit->GetStateID());

			pCCha = 0;
			pSrcMainC = 0;
			pCEnt = g_pGameApp->IsLiveingEntity(pSStateUnit->ulSrcWorldID, pSStateUnit->lSrcHandle);
			if (pCEnt)
			{
				pCCha = pCEnt->IsCharacter();
				if (pCCha != g_pCSystemCha && pCCha != this)
				{
					pCCha->GetPlyMainCha()->SetLookChangeFlag();
					pCCha->m_CChaAttr.ResetChangeFlag();
					pCCha->m_CSkillState.ResetChangeFlag();
				}
				pSrcMainC = pCCha->GetPlyMainCha();
				if (pSrcMainC == pCCha || pSrcMainC == this)
					pSrcMainC = 0;
				if (pSrcMainC)
				{
					pSrcMainC->m_CChaAttr.ResetChangeFlag();
				}
			}

			lOldHP = (long)m_CChaAttr.GetAttr(ATTR_HP);
			if (pSStateUnit->lOnTick > 0) // 
			{
				if (dwCurTick - pSStateUnit->ulStartTick > (unsigned long)pSStateUnit->lOnTick * 1000)
				{
					DelSkillState(pSStateUnit->GetStateID(), false);
				}
				else
				{
					if (pCSStateRec->sFrequency > 0) // 
					{
						sExecTime = Short(dwCurTick - pSStateUnit->ulLastTick) / (pCSStateRec->sFrequency * 1000);
						for (int j = 0; j < sExecTime; j++)
						{
							//g_CParser.DoString(pCSStateRec->szAddState, enumSCRIPT_RETURN_NONE, 0, enumSCRIPT_PARAM_LIGHTUSERDATA, 1, this->IsCharacter(), enumSCRIPT_PARAM_NUMBER, 1, pSStateUnit->GetStateLv(), DOSTRING_PARAM_END);
							g_luaAPI.Call(pCSStateRec->szAddState.c_str(), this->IsCharacter(), (int)pSStateUnit->GetStateLv());
						}
						pSStateUnit->ulLastTick += pCSStateRec->sFrequency * sExecTime * 1000;
					}
					else if (pCSStateRec->sFrequency < 0) //
					{
						pSStateUnit->ulLastTick = dwCurTick;
					}
				}
			}
			else if (pSStateUnit->lOnTick < 0) //
			{
				if (pCSStateRec->sFrequency > 0) // 
				{
					sExecTime = Short(dwCurTick - pSStateUnit->ulLastTick) / (pCSStateRec->sFrequency * 1000);
					for (int j = 0; j < sExecTime; j++)
					{
						//g_CParser.DoString(pCSStateRec->szAddState, enumSCRIPT_RETURN_NONE, 0, enumSCRIPT_PARAM_LIGHTUSERDATA, 1, this->IsCharacter(), enumSCRIPT_PARAM_NUMBER, 1, pSStateUnit->GetStateLv(), DOSTRING_PARAM_END);
						g_luaAPI.Call(pCSStateRec->szAddState.c_str(), this->IsCharacter(), (int)pSStateUnit->GetStateLv());
					}
					pSStateUnit->ulLastTick += pCSStateRec->sFrequency * sExecTime * 1000;
				}
				else if (pCSStateRec->sFrequency < 0) //
				{
					pSStateUnit->ulLastTick = dwCurTick;
				}
			}

			BeUseSkill(lOldHP, (long)m_CChaAttr.GetAttr(ATTR_HP), pCCha, pSStateUnit->chEffType);
			if (lOldHP > 0 && m_CChaAttr.GetAttr(ATTR_HP) <= 0) bIsDie = true;
			else bIsDie = false;
			if (bIsDie) // 
			{
				SetDie(pCCha);
			}
			if (pCCha && pCCha != g_pCSystemCha && pCCha != this)
			{
				pCCha->GetPlyMainCha()->SynLook(enumSYN_LOOK_CHANGE);
				pCCha->SynSkillStateToEyeshot();
				pCCha->SynAttr(enumATTRSYN_ATTACK);
				pCCha->RectifyAttr();
			}
			if (pSrcMainC)
			{
				pSrcMainC->SynAttr(enumATTRSYN_ATTACK);
			}
			if (bIsDie) // 
			{
				Die();
				break;
			}
		}
		// log
		//
		SynSkillStateToEyeshot();
		SynAttr(enumATTRSYN_SKILL_STATE);
		IsCharacter()->GetPlyMainCha()->SynLook(enumSYN_LOOK_CHANGE);
		RectifyAttr();
	}
}

void CFightAble::RemoveOtherSkillState()
{
	if (m_CSkillState.GetStateNum() > 0)
	{
		IsCharacter()->GetPlyMainCha()->SetLookChangeFlag();
		m_CChaAttr.ResetChangeFlag();
		m_CSkillState.ResetChangeFlag();
		SSkillStateUnit		*pSStateUnit;
		m_CSkillState.BeginGetState();
		while (pSStateUnit = m_CSkillState.GetNextState())
		{
			if (pSStateUnit->lOnTick > 0) // 
			{
				DelSkillState(pSStateUnit->GetStateID(), false);
			}
		}

		SynSkillStateToEyeshot();
		SynAttr(enumATTRSYN_SKILL_STATE);
		IsCharacter()->GetPlyMainCha()->SynLook(enumSYN_LOOK_CHANGE);
	}
}

void CFightAble::RemoveAllSkillState()
{
	if (m_CSkillState.GetStateNum() > 0)
	{
		SSkillStateUnit		*pSStateUnit;
		m_CSkillState.BeginGetState();
		while (pSStateUnit = m_CSkillState.GetNextState())
		{
			DelSkillState(pSStateUnit->GetStateID(), false);
		}
	}
}

// 
void CFightAble::EnrichSkillBag(bool bActive)
{
	SSkillGrid	SSkillCont;
	if (bActive)
		SSkillCont.chState = enumSUSTATE_ACTIVE;
	else
		SSkillCont.chState = enumSUSTATE_INACTIVE;
	for (int i = 0; i < defCHA_INIT_SKILL_NUM; i++)
	{
		if (m_pCChaRecord->lSkill[i][0] > 0)
		{
			SSkillCont.sID = (Short)m_pCChaRecord->lSkill[i][0];
			SSkillCont.chLv = 1;
			m_CSkillBag.Add(&SSkillCont);
		}
	}
}

//=============================================================================
CTimeSkillMgr::CTimeSkillMgr(unsigned short usFreq)
{
	m_ulTick = GetTickCount();
	m_usFreq = usFreq;
	m_pSExecQueue = NULL;
	m_pSFreeQueue = NULL;
}

CTimeSkillMgr::~CTimeSkillMgr()
{
	SMgrUnit	*pSCarrier;

	pSCarrier = m_pSExecQueue;
	while (pSCarrier)
	{
		m_pSExecQueue = pSCarrier->pSNext;
		delete pSCarrier;
		pSCarrier = m_pSExecQueue;
	}

	pSCarrier = m_pSFreeQueue;
	while (pSCarrier)
	{
		m_pSFreeQueue = pSCarrier->pSNext;
		delete pSCarrier;
		pSCarrier = m_pSFreeQueue;
	}
}

void CTimeSkillMgr::Add(SFireUnit *pSFireSrc, uLong ulLeftTick, SubMap *pCMap, Point *pStarget, Long *plRangeBParam)
{
	SMgrUnit	*pSCarrier = NULL;

	if (m_pSFreeQueue) // 
	{
		pSCarrier = m_pSFreeQueue;
		m_pSFreeQueue = pSCarrier->pSNext;
	}
	else // 
	{
		pSCarrier = new SMgrUnit;
		if (!pSCarrier)
		{
			//THROW_EXCP(excpMem, "");
			THROW_EXCP(excpMem, RES_STRING(GM_FIGHTALBE_CPP_00004));
		}
	}

	// 
	pSCarrier->SFireSrc = *pSFireSrc;
	pSCarrier->ulLeftTick = ulLeftTick;
	pSCarrier->pCMap = pCMap;
	pSCarrier->STargetPos = *pStarget;

	memcpy(pSCarrier->lERangeBParam, plRangeBParam, sizeof(Long) * defSKILL_RANGE_BASEP_NUM);

	pSCarrier->pSNext = m_pSExecQueue;
	m_pSExecQueue = pSCarrier;
}

void CTimeSkillMgr::Run(unsigned long ulCurTick)
{
	unsigned long	ulTickDist = ulCurTick - m_ulTick;

	if (ulTickDist < m_usFreq)
		return;
	m_ulTick = ulCurTick;

	SMgrUnit	*pSCarrier, *pSLastCarrier;
	pSCarrier = pSLastCarrier = m_pSExecQueue;
	while (pSCarrier)
	{
		if (pSCarrier->ulLeftTick > ulTickDist)

		{
			pSCarrier->ulLeftTick -= ulTickDist;
			pSLastCarrier = pSCarrier;
			pSCarrier = pSCarrier->pSNext;

		}
		else // 
		{
			ExecTimeSkill(pSCarrier);
			// 
			if (pSCarrier == m_pSExecQueue)
			{
				m_pSExecQueue = pSCarrier->pSNext;

				pSCarrier->pSNext = m_pSFreeQueue;
				m_pSFreeQueue = pSCarrier;

				pSLastCarrier = m_pSExecQueue;
				pSCarrier = pSLastCarrier;
			}
			else
			{
				pSLastCarrier->pSNext = pSCarrier->pSNext;

				pSCarrier->pSNext = m_pSFreeQueue;
				m_pSFreeQueue = pSCarrier;

				pSCarrier = pSLastCarrier->pSNext;
			}
		}
	}
}

void CTimeSkillMgr::ExecTimeSkill(SMgrUnit *pFireInfo)
{
	// 
	CCharacter	*pSrcCha;
	Entity	*pSrcEnt = g_pGameApp->IsLiveingEntity(pFireInfo->SFireSrc.ulID, pFireInfo->SFireSrc.pCFightSrc->GetHandle());
	if (!pSrcEnt || !(pSrcCha = pSrcEnt->IsCharacter())) // 
		return;

	g_ulCurID = pSrcCha->GetID();
	g_lCurHandle = pSrcCha->GetHandle();

	g_SSkillPoint = pFireInfo->STargetPos;
	pSrcCha->RangeEffect(&pFireInfo->SFireSrc, pFireInfo->pCMap, pFireInfo->lERangeBParam);

	g_ulCurID = defINVALID_CHA_ID;
	g_lCurHandle = defINVALID_CHA_HANDLE;
}

// =====================================================================
//  Fill*     (CommandMessages.h)
// =====================================================================

void CFightAble::FillSkillState(net::msg::ChaSkillStateInfo &s)
{
	s.currentTime = GetTickCount();
	s.states.clear();

	SSkillStateUnit *pSStateUnit;
	m_CSkillState.BeginGetState();
	while (pSStateUnit = m_CSkillState.GetNextState())
	{
		net::msg::SkillStateEntry e;
		e.stateId = pSStateUnit->GetStateID();
		e.stateLv = pSStateUnit->GetStateLv();
		if (pSStateUnit->lOnTick < 1)
		{
			e.duration = 0;
			e.startTime = 0;
		}
		else
		{
			e.duration = pSStateUnit->lOnTick;
			e.startTime = pSStateUnit->ulStartTick;
		}
		s.states.push_back(e);
	}
}

void CFightAble::FillAttr(net::msg::ChaAttrInfo &a, Short sSynType)
{
	a.synType = sSynType;
	a.attrs.clear();

	short sAttrChangeNum = m_CChaAttr.GetChangeNumClient();
	if (sAttrChangeNum > 0)
	{
		for (int i = 0; i < ATTR_CLIENT_MAX; i++)
		{
			if (m_CChaAttr.GetChangeBitFlag(i))
			{
				net::msg::AttrEntry e;
				e.attrId = i;
				e.attrVal = m_CChaAttr.GetAttr(i);
				a.attrs.push_back(e);
			}
		}
	}
}

//   0..ATTR_CLIENT_MAX-1 ( INIT-)
void CFightAble::FillAttrAll(net::msg::ChaAttrInfo &a, Short sSynType)
{
	a.synType = sSynType;
	a.attrs.clear();
	a.attrs.reserve(ATTR_CLIENT_MAX);
	for (int i = 0; i < ATTR_CLIENT_MAX; i++)
	{
		net::msg::AttrEntry e;
		e.attrId = i;
		e.attrVal = m_CChaAttr.GetAttr(i);
		a.attrs.push_back(e);
	}
}

// 5   (LV, HP, MXHP, ASPD, MSPD)
void CFightAble::FillMonsAttr(net::msg::ChaAttrInfo &a, Short sSynType)
{
	a.synType = sSynType;
	a.attrs.clear();
	a.attrs.reserve(5);
	int monsAttrs[] = { ATTR_LV, ATTR_HP, ATTR_MXHP, ATTR_ASPD, ATTR_MSPD };
	for (int id : monsAttrs)
	{
		net::msg::AttrEntry e;
		e.attrId = id;
		e.attrVal = (long)m_CChaAttr.GetAttr(id);
		a.attrs.push_back(e);
	}
}

