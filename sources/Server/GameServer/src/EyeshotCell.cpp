//=============================================================================
// FileName: EyeshotCell.cpp
// Creater: ZhangXuedong
// Date: 2005.04.29
// Comment: Map Eyeshot Cell
//=============================================================================
#include "stdafx.h"
#include "EyeshotCell.h"
#include "GamePool.h"
#include "Character.h"
#include "Item.h"

CEyeshotCell::CEyeshotCell()
{
	m_lActNum = 0;
	m_pCChaL = 0;
	m_pCItemL = 0;

	m_pCNext = 0;
	m_pCLast = 0;

	m_lChaCount = 0;
	m_lItemCount = 0;
}

CEyeshotCell::~CEyeshotCell()
{
	Entity	*pCEnt, *pCHeadEnt;

	pCHeadEnt = m_pCChaL;
	while(pCEnt = pCHeadEnt)
	{
		pCHeadEnt = pCHeadEnt->m_pCEyeshotCellNext;
		pCEnt->m_pCEyeshotCellNext = 0;
		pCEnt->m_pCEyeshotCellLast = 0;
		pCEnt->Free();
	}

	pCHeadEnt = m_pCItemL;
	while(pCEnt = pCHeadEnt)
	{
		pCHeadEnt = pCHeadEnt->m_pCEyeshotCellNext;
		pCEnt->m_pCEyeshotCellNext = 0;
		pCEnt->m_pCEyeshotCellLast = 0;
		pCEnt->Free();
	}
}

// pCEnt
void CEyeshotCell::EnterEyeshot(Entity *pCEnt)
{
	DBG_ASSERT_ENTITY(pCEnt);
	for (auto* slot : _stateCellSlots) {
		if (auto* cell = *slot) {
			cell->StateBeginSeen(pCEnt);
		}
	}

	CCharacter	*pCCha = pCEnt->IsCharacter();
	CCharacter	*pCCellCha = m_pCChaL;
	while (pCCellCha)
	{
		if (pCEnt != pCCellCha)
		{
			if (pCCha)
			{
				if (pCCellCha->CanSeen(pCCha))
					pCEnt->BeginSee(pCCellCha);
				if (pCCha->CanSeen(pCCellCha))
					pCCellCha->BeginSee(pCEnt);
			}
			else
			{
				pCEnt->BeginSee(pCCellCha);
				pCCellCha->BeginSee(pCEnt);
			}
		}
		if (pCCellCha->m_pCEyeshotCellNext)
			pCCellCha = pCCellCha->m_pCEyeshotCellNext->IsCharacter();
		else
			pCCellCha = 0;
	}
	CItem	*pCCellItem = m_pCItemL;
	while (pCCellItem)
	{
		pCEnt->BeginSee(pCCellItem);
		if (pCCellItem->m_pCEyeshotCellNext)
			pCCellItem = pCCellItem->m_pCEyeshotCellNext->IsItem();
		else
			pCCellItem = 0;
	}
}

void CEyeshotCell::OutEyeshot(Entity *pCEnt)
{
	DBG_ASSERT_ENTITY(pCEnt);
#if defined(_DEBUG)
	// Перед тем как разослать EndSee всем соседям по клетке — проверим,
	// что и сами соседи ещё живы. Раньше мы ловили краши именно здесь:
	// лут освобождался без снятия из eyeshot, и затем EndSee уходил по
	// висячему указателю.
	//
	// Если нашли stale — сразу дампим всё состояние клетки: адрес cell,
	// позицию, все указатели в linked-list (чтобы увидеть ВЕСЬ порядок
	// элементов, а не только stale).
	const auto dumpCellItems = [this]() {
		ToLogService("errors", LogLevel::Error,
			"CELL DUMP cell={} pos=[{},{}] chaCount={} itemCount={} chaHead={} itemHead={}",
			static_cast<const void*>(this), m_sPosX, m_sPosY,
			m_lChaCount, m_lItemCount,
			static_cast<const void*>(m_pCChaL),
			static_cast<const void*>(m_pCItemL));
		int idx = 0;
		for (const Entity* p = m_pCItemL; p && idx < 16;
			 p = p->m_pCEyeshotCellNext, ++idx)
		{
			const bool alive = GamePool::Instance().IsValidEntityPtr(p);
			ToLogService("errors", LogLevel::Error,
				"  item[{}] ptr={} alive={} next={} last={}",
				idx, static_cast<const void*>(p), alive,
				static_cast<const void*>(p->m_pCEyeshotCellNext),
				static_cast<const void*>(p->m_pCEyeshotCellLast));
		}
	};
	for (const CCharacter* pDbg = m_pCChaL; pDbg; )
	{
		if (pDbg && !GamePool::Instance().IsValidEntityPtr(pDbg))
		{
			ToLogService("errors", LogLevel::Error,
				"OutEyeshot stale CHARACTER in cell ptr={}", static_cast<const void*>(pDbg));
			dumpCellItems();
			assert(!"OutEyeshot: stale character in m_pCChaL");
			break;
		}
		pDbg = pDbg->m_pCEyeshotCellNext ? pDbg->m_pCEyeshotCellNext->IsCharacter() : nullptr;
	}
	for (const CItem* pDbg = m_pCItemL; pDbg; )
	{
		if (pDbg && !GamePool::Instance().IsValidEntityPtr(pDbg))
		{
			ToLogService("errors", LogLevel::Error,
				"OutEyeshot stale ITEM in cell ptr={}", static_cast<const void*>(pDbg));
			dumpCellItems();
			assert(!"OutEyeshot: stale item in m_pCItemL");
			break;
		}
		pDbg = pDbg->m_pCEyeshotCellNext ? pDbg->m_pCEyeshotCellNext->IsItem() : nullptr;
	}
#endif
	for (auto* slot : _stateCellSlots) {
		if (auto* cell = *slot) {
			cell->StateEndSeen(pCEnt);
		}
	}

	CCharacter	*pCCha = pCEnt->IsCharacter();
	CCharacter	*pCCellCha = m_pCChaL;
	while (pCCellCha)
	{
		if (pCEnt != pCCellCha)
		{
			if (pCCha)
			{
				if (pCCellCha->CanSeen(pCCha))
					pCEnt->EndSee(pCCellCha);
				if (pCCha->CanSeen(pCCellCha))
					pCCellCha->EndSee(pCEnt);
			}
			else
			{
				pCEnt->EndSee(pCCellCha);
				pCCellCha->EndSee(pCEnt);
			}
		}
		if (pCCellCha->m_pCEyeshotCellNext)
			pCCellCha = pCCellCha->m_pCEyeshotCellNext->IsCharacter();
		else
			pCCellCha = 0;
	}
	CItem	*pCCellItem = m_pCItemL;
	while (pCCellItem)
	{
		pCEnt->EndSee(pCCellItem);
		if (pCCellItem->m_pCEyeshotCellNext)
			pCCellItem = pCCellItem->m_pCEyeshotCellNext->IsItem();
		else
			pCCellItem = 0;
	}
}

void CEyeshotCell::RefreshEyeshot(Entity *pCEnt, bool bToEyeshot, bool bToNoHide, bool bToNoShow)
{
	CCharacter	*pCCha = pCEnt->IsCharacter();
	if (!pCCha)
		return;
	if (pCCha->GetActControl(enumACTCONTROL_EYESHOT) == bToEyeshot
		&& pCCha->GetActControl(enumACTCONTROL_NOHIDE) == bToNoHide
		&& pCCha->GetActControl(enumACTCONTROL_NOSHOW) == bToNoShow)
		return;
	bool	bOldSeen, bNewSeen;

	CCharacter	*pCCellCha;
	pCCellCha = m_pCChaL;
	while (pCCellCha)
	{
		if (pCCha != pCCellCha)
		{
			bOldSeen = pCCha->CanSeen(pCCellCha);
			bNewSeen = pCCha->CanSeen(pCCellCha, bToEyeshot, bToNoHide, bToNoShow);
			if (bOldSeen && !bNewSeen)
				pCCellCha->EndSee(pCCha);
			if (!bOldSeen && bNewSeen)
				pCCellCha->BeginSee(pCCha);
		}
		if (pCCellCha->m_pCEyeshotCellNext)
			pCCellCha = pCCellCha->m_pCEyeshotCellNext->IsCharacter();
		else
			pCCellCha = 0;
	}
}

CCharacter* CEyeshotCell::GetNextCha(void)
{
	CCharacter	*pRet = m_pCChaSearch;

	if (m_pCChaSearch)
		m_pCChaSearch = m_pCChaSearch->m_pCEyeshotCellNext ? m_pCChaSearch->m_pCEyeshotCellNext->IsCharacter() : NULL;

	return pRet;
}

// ============================================================================
// Ранее inline-методы из EyeshotCell.h, вынесены в .cpp 2026-04-22.
// ============================================================================

long CEyeshotCell::GetEntityNum(void) { return m_lChaCount + m_lItemCount; }
long CEyeshotCell::GetChaNum(void)    { return m_lChaCount; }
long CEyeshotCell::GetItemNum(void)   { return m_lItemCount; }

void CEyeshotCell::BeginGetCha(void)  { m_pCChaSearch = m_pCChaL; }

void CEyeshotCell::AddEntity(CCharacter *pCCha)
{
	DBG_ASSERT_ENTITY(pCCha);
	if (!pCCha)
		return;
	if (pCCha->m_pCEyeshotCellLast || pCCha->m_pCEyeshotCellNext)
	{
		ToLogService("errors", LogLevel::Error, "when add character entity to eyeshot cell {} ,find it is not break away foregone manage cell", pCCha->GetLogName());
		return;
	}

	pCCha->m_pCEyeshotCellLast = 0;
	pCCha->m_pCEyeshotCellNext = m_pCChaL;
	if (pCCha->m_pCEyeshotCellNext)
	{
		pCCha->m_pCEyeshotCellNext->m_pCEyeshotCellLast = pCCha;
	}
	m_pCChaL = pCCha;

	m_lChaCount++;
}

void CEyeshotCell::AddEntity(CItem *pCItem)
{
	DBG_ASSERT_ENTITY(pCItem);
	if (!pCItem)
		return;
	if (pCItem->m_pCEyeshotCellLast || pCItem->m_pCEyeshotCellNext)
	{
		ToLogService("errors", LogLevel::Error, "when add item entity to {}, find it is not break away foregone manage cell", pCItem->GetLogName());
		return;
	}

#if defined(_DEBUG)
	ToLogService("common", LogLevel::Debug,
		"CEyeshotCell::AddEntity(Item) cell={} item={} name='{}' prevHead={}",
		static_cast<const void*>(this),
		static_cast<const void*>(pCItem), pCItem->GetLogName(),
		static_cast<const void*>(m_pCItemL));
#endif

	pCItem->m_pCEyeshotCellLast = 0;
	pCItem->m_pCEyeshotCellNext = m_pCItemL;
	if (pCItem->m_pCEyeshotCellNext)
	{
		pCItem->m_pCEyeshotCellNext->m_pCEyeshotCellLast = pCItem;
	}
	m_pCItemL = pCItem;

	m_lItemCount++;
}

void CEyeshotCell::DelEntity(Entity *pCEnt)
{
	// ВАЖНО: Del может вызываться и во время Free() entity, когда GamePool
	// его уже могли снять. Не ассертим pCEnt здесь — только в Add/путях,
	// куда entity должен приходить живым.
	if (!pCEnt)
		return;

#if defined(_DEBUG)
	// Логируем только items (характеры шумят слишком сильно).
	if (pCEnt->IsItem())
	{
		ToLogService("common", LogLevel::Debug,
			"CEyeshotCell::DelEntity(Item) cell={} item={} name='{}' head_before={} next={} last={}",
			static_cast<const void*>(this),
			static_cast<const void*>(pCEnt), pCEnt->GetLogName(),
			static_cast<const void*>(m_pCItemL),
			static_cast<const void*>(pCEnt->m_pCEyeshotCellNext),
			static_cast<const void*>(pCEnt->m_pCEyeshotCellLast));
	}
#endif

	if (pCEnt->IsCharacter())
	{
		if (m_pCChaSearch == pCEnt)
			m_pCChaSearch = pCEnt->m_pCEyeshotCellNext ? pCEnt->m_pCEyeshotCellNext->IsCharacter() : NULL;
	}

	if (pCEnt->m_pCEyeshotCellLast)
		pCEnt->m_pCEyeshotCellLast->m_pCEyeshotCellNext = pCEnt->m_pCEyeshotCellNext;
	if (pCEnt->m_pCEyeshotCellNext)
		pCEnt->m_pCEyeshotCellNext->m_pCEyeshotCellLast = pCEnt->m_pCEyeshotCellLast;

	if (m_pCChaL == pCEnt)
	{
		if (pCEnt->m_pCEyeshotCellNext)
		{
			m_pCChaL = pCEnt->m_pCEyeshotCellNext->IsCharacter();
			m_pCChaL->m_pCEyeshotCellLast = 0;
		}
		else
			m_pCChaL = 0;
	}
	else if (m_pCItemL == pCEnt)
	{
		if (pCEnt->m_pCEyeshotCellNext)
		{
			m_pCItemL = pCEnt->m_pCEyeshotCellNext->IsItem();
			m_pCItemL->m_pCEyeshotCellLast = 0;
		}
		else
			m_pCItemL = 0;
	}

	pCEnt->m_pCEyeshotCellLast = 0;
	pCEnt->m_pCEyeshotCellNext = 0;

	if (pCEnt->IsCharacter())
		m_lChaCount--;
	else
		m_lItemCount--;
}

// --- CActEyeshotCell ---

void CActEyeshotCell::Add(CEyeshotCell *pObj)
{
	pObj->m_pCLast = 0;
	if (pObj->m_pCNext = m_pHead)
		m_pHead->m_pCLast = pObj;
	m_pHead = pObj;

	m_lCount++;
}

void CActEyeshotCell::Del(CEyeshotCell *pObj)
{
	if (!pObj)
		return;
	if (m_pCur == pObj)
		m_pCur = pObj->m_pCNext;

	if (pObj->m_pCLast)
		pObj->m_pCLast->m_pCNext = pObj->m_pCNext;
	if (pObj->m_pCNext)
		pObj->m_pCNext->m_pCLast = pObj->m_pCLast;
	if (m_pHead == pObj)
	{
		if (m_pHead = pObj->m_pCNext)
			m_pHead->m_pCLast = 0;
	}
	pObj->m_pCNext = 0;
	pObj->m_pCLast = 0;

	m_lCount--;
}

void CActEyeshotCell::BeginGet()
{
	m_pCur = m_pHead;
}

CEyeshotCell* CActEyeshotCell::GetNext()
{
	CEyeshotCell *pRet = m_pCur;

	if (m_pCur)
		m_pCur = m_pCur->m_pCNext;

	return pRet;
}

CEyeshotCell* CActEyeshotCell::GetCurrent()
{
	return m_pCur;
}

long CActEyeshotCell::GetActiveNum(void)
{
	return m_lCount;
}
