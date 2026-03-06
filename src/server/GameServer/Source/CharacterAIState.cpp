#include "stdafx.h"
#include "Character.h"
#include "SubMap.h"
#include "NPC.h"
#include "lua_gamectrl.h"
#include "TryUtil.h"
#include "HarmRec.h"

//--------------------------------------------------------
//                       AI
//--------------------------------------------------------


//-----------
// AI
//-----------
BOOL  g_bEnableAI  = TRUE;

void CCharacter::OnAI(DWORD dwCurTime)
{T_B
	
	m_pHate->UpdateHarmRec(this); // 

	if (IsPlayerCha() && getAttr(ATTR_CHATYPE) != enumCHACTRL_PLAYER_PET)	return;

	if(!g_bEnableAI) return;

	if (m_SMoveInit.STargetInfo.chType == 1)
	{
		Entity *pEnt = g_pGameApp->IsLiveingEntity(m_SMoveInit.STargetInfo.lInfo1, m_SMoveInit.STargetInfo.lInfo2);
		if (!pEnt)
			m_SMoveInit.STargetInfo.chType = 0;
	}

	if (m_SFightInit.chTarType == 1)
	{
		Entity *pEnt = g_pGameApp->IsLiveingEntity(m_SFightInit.lTarInfo1, m_SFightInit.lTarInfo2);
		if (!pEnt)
		{
			m_SFightInit.chTarType = 0;
			m_AITarget = 0;
		}
		else
			m_AITarget = pEnt->IsCharacter();
	}
	else
	{
		m_AITarget = 0;
	}

	if (!IsLiveing())         return; // 


	if (IsNpc())
		lua_NPCRun(this);
	else
	{
		DWORD	dwResumeExecTime = m_timerScripts.IsOK(dwCurTime);
		if (dwResumeExecTime > 0)
			lua_AIRun(this, dwResumeExecTime);
	}

T_E}


void CCharacter::ResetAIState()
{
	m_AITarget		= 0;
	m_btPatrolState = 0;
	m_pHate->ClearHarmRec();
}
