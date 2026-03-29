//=============================================================================
// FileName: Action.cpp
// Creater: ZhangXuedong
// Date: 2004.10.08
// Comment: CAction class
//=============================================================================
#include "stdafx.h"
#include "Action.h"
#include "GameCommon.h"
#include "MoveAble.h"
#include "Character.h"

_DBC_USING;

CAction::CAction(Entity *pCEntity)
{
	m_pCEntity = pCEntity;
	memset(m_SAction, 0, sizeof(m_SAction));
	m_sActionNum = 0;
	m_sCurAction = -1;
}

bool CAction::Add(dbc::Short sActionType, void *pActionData)
{
	if (m_sCurAction != -1 || m_sActionNum >= defMAX_ACTION_NUM)
	{
		return false;
	}

	m_SAction[m_sActionNum].sType = sActionType;
	switch (m_SAction[m_sActionNum].sType)
	{
	case	enumACTION_MOVE:
		{
			memcpy(&m_SMoveInit, pActionData, sizeof(m_SMoveInit));
			m_SAction[m_sActionNum].pInit = &m_SMoveInit;
			m_sActionNum ++;

			break;
		}
	case	enumACTION_SKILL:
		{
			memcpy(&m_SFightInit, pActionData, sizeof(m_SFightInit));
			m_SAction[m_sActionNum].pInit = &m_SFightInit;
			m_sActionNum ++;

			break;
		}
	default:
		{
			break;
		}
	}

	return true;
}

//=============================================================================
// sActionType .0
// sActionState 
//=============================================================================
bool CAction::DoNext(Short sActionType, Short sActionState)
{
	if (m_sActionNum == 0 || m_pCEntity == NULL)
	{
		return false;
	}
	if (m_sCurAction >= m_sActionNum - 1)
	{
		m_sCurAction = -1;
		m_sActionNum = 0;
		return false;
	}

	switch (m_SAction[m_sCurAction + 1].sType)
	{
	case	enumACTION_MOVE:
		{
			CMoveAble *pCMoveAble = m_pCEntity->IsMoveAble();
			if (pCMoveAble)
			{
				m_sCurAction ++;

				if (pCMoveAble->DesireMoveBegin((CMoveAble::SMoveInit *)m_SAction[m_sCurAction].pInit) == false)
				{
					m_sCurAction = -1;
					m_sActionNum = 0;
				}
			}

			break;
		}
	case	enumACTION_SKILL:
		{
			if (sActionType == enumACTION_MOVE && sActionState & enumMSTATE_INRANGE)
			{
				CFightAble *pCFightAble = m_pCEntity->IsFightAble();
				if (pCFightAble)
				{
					m_sCurAction ++;

					if (pCFightAble->DesireFightBegin((SFightInit *)m_SAction[m_sCurAction].pInit) == false)
					{
						m_sCurAction = -1;
						m_sActionNum = 0;
					}
				}
			}
			else
				Interrupt();

			break;
		}
	default:
		{
			return false;

			break;
		}
	}

	return true;
}

void CAction::End()
{
	if (m_sActionNum == 0 || m_sCurAction == -1 || m_pCEntity == NULL)
	{
		return;
	}

	switch (m_SAction[m_sCurAction].sType)
	{
	case	enumACTION_MOVE:
		{
			CMoveAble *pCMoveAble = m_pCEntity->IsMoveAble();
			if (pCMoveAble)
			{
				pCMoveAble->DesireMoveStop();
			}

			break;
		}
	case	enumACTION_SKILL:
		{
			CFightAble *pCFightAble = m_pCEntity->IsFightAble();
			if (pCFightAble)
			{
				pCFightAble->DesireFightEnd();
			}

			break;
		}
	default:
		{
			break;
		}
	}

	m_sCurAction = -1;
	m_sActionNum = 0;

	return;
}

void CAction::Interrupt()
{
	m_sCurAction = -1;
	m_sActionNum = 0;

	return;
}

bool CAction::Has(Short sActionType, void *pActionData)
{
	//if (m_sActionNum == 0 || m_pCEntity == NULL)
	//	return false;
	//if (m_sCurAction >= m_sActionNum - 1)
	//	return false;

	//for (Short i = m_sCurAction + 1; i < m_sActionNum; i++)
	//{
	//	if (m_SAction[i].sType == sActionType)
	//	{
	//	}
	//}

	return false;
}

//=============================================================================
CActionCache::CActionCache(CCharacter *pCOwn)
{
	m_pCOwn = pCOwn;
	m_pSExecQueue = NULL;
	m_pSFreeQueue = NULL;
}

CActionCache::~CActionCache()
{
	SAction	*pSCarrier;

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

void CActionCache::AddCommand(Short sCommand)
{
	SAction	*pSCarrier = NULL;

	if (m_pSFreeQueue)
	{
		pSCarrier = m_pSFreeQueue;
		m_pSFreeQueue = pSCarrier->pSNext;
	}
	else
	{
		pSCarrier = new SAction;
		if (!pSCarrier)
		{
			//THROW_EXCP(excpMem, "");
			THROW_EXCP(excpMem, RES_STRING(GM_ACTION_CPP_00001));
		}
	}

	pSCarrier->sCommand = sCommand;
	pSCarrier->chParamPos = 0;

	pSCarrier->pSNext = m_pSExecQueue;
	m_pSExecQueue = pSCarrier;
}

void CActionCache::PushParam(void *pParam, Char chSize)
{
	if (m_pSExecQueue && (m_pSExecQueue->chParamPos + chSize <= defMAX_CACHE_ACTION_PARAM_LEN))
	{
		memcpy(m_pSExecQueue->szParam + m_pSExecQueue->chParamPos, pParam, chSize);
		m_pSExecQueue->chParamPos += chSize;
	}
}

void CActionCache::Run()
{
	SAction	*pSCarrier, *pSLastCarrier;
	pSCarrier = pSLastCarrier = m_pSExecQueue;
	while (pSCarrier)
	{
		ExecAction(pSCarrier);

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

void CActionCache::ExecAction(SAction *pSCarrier)
{
	Char	chCurParamPos = 0;
	switch (pSCarrier->sCommand)
	{
	case	enumCACHEACTION_MOVE:
		{
			Short sPing = *((Short *)(pSCarrier->szParam + chCurParamPos));
			chCurParamPos += sizeof(Short);
			Char chPointNum = pSCarrier->szParam[chCurParamPos];
			chCurParamPos += sizeof(Char);
			Point *pPath = (Point *)(pSCarrier->szParam + chCurParamPos);
			chCurParamPos += sizeof(Point) * chPointNum;
			if (chCurParamPos == pSCarrier->chParamPos)
				m_pCOwn->Cmd_BeginMove(sPing, pPath, chPointNum);
			else if (chCurParamPos < pSCarrier->chParamPos)
			{
				Char chStopState = pSCarrier->szParam[chCurParamPos];
				chCurParamPos += sizeof(Char);
				if (chCurParamPos == pSCarrier->chParamPos)
					m_pCOwn->Cmd_BeginMove(sPing, pPath, chPointNum, chStopState);
			}
		}
		break;
	case	enumCACHEACTION_SKILL:
		{
			Long lSkillID = *((Long *)(pSCarrier->szParam + chCurParamPos));
			chCurParamPos += sizeof(Long);
			CCharacter *pCTar = *((CCharacter **)(pSCarrier->szParam + chCurParamPos));
			m_pCOwn->Cmd_BeginSkillDirect(lSkillID, pCTar);
		}
		break;
	case	enumCACHEACTION_SKILL2:
		{
			Long lSkillID = *((Long *)(pSCarrier->szParam + chCurParamPos));
			chCurParamPos += sizeof(Long);
			Long lSkillLv = *((Long *)(pSCarrier->szParam + chCurParamPos));
			chCurParamPos += sizeof(Long);
			Long lPosX = *((Long *)(pSCarrier->szParam + chCurParamPos));
			chCurParamPos += sizeof(Long);
			Long lPosY = *((Long *)(pSCarrier->szParam + chCurParamPos));
			chCurParamPos += sizeof(Long);
			m_pCOwn->Cmd_BeginSkillDirect2(lSkillID, lSkillLv, lPosX, lPosY);
		}
		break;
	}
}
