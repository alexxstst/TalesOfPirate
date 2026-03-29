//=============================================================================
// FileName: ToolClass.h
// Creater: ZhangXuedong
// Date: 2005.10.19
// Comment: ToolClass class
//=============================================================================

#ifndef TOOLCLASS_H
#define TOOLCLASS_H

template <class T>
class CListArray
{
public:
	CListArray();
	~CListArray();

	bool	Init(long lSize);
	void	SetCapacity(long lCapacity);
	long	GetCapacity(void) {return m_lCapacity;}
	void	Reset(void);
	void	Free(void);

	T*		Add(T* pCObj);
	bool	Del(T* pCObj);
	bool	Del(long lPosID);
	long	GetUsedNum(void) {return m_lUsed;}
	long	GetSize(void) {return m_lSize;}

	T*		Get(long lPosID);
	void	BeginGet(void) {m_lSearchID = m_lUsed - 1;}
	T*		GetNext(void);

protected:

private:
	struct SUnit
	{
		T		CContent;
		long	lPosReverse;
		long	lPosID;
	};

	long	m_lSize;
	long	m_lCapacity;
	long	m_lUsed;
	SUnit**	m_ppArray;
	SUnit*	m_pArray;

	long	m_lSearchID;

};

template<class T>
CListArray<T>::CListArray()
{
	m_lSize = 0;
	m_lCapacity = 0;
	m_lUsed = 0;
	m_ppArray = NULL;
	m_pArray = NULL;
}

template<class T>
CListArray<T>::~CListArray()
{
	Free();
}

template<class T>
bool CListArray<T>::Init(long lSize)
{
	if (lSize < 0)
		return false;

	m_lUsed = 0;
	m_lSize = lSize;
	m_lCapacity = m_lSize;
	m_ppArray = new SUnit*[m_lSize];
	m_pArray = new SUnit[m_lSize];
	if (!m_ppArray || !m_pArray)
	{
		Free();
		return false;
	}

	for (long i = 0; i < m_lSize; i++)
	{
		m_pArray[i].CContent.SetPosID(i);
		m_pArray[i].lPosReverse = i;
		m_pArray[i].lPosID = i;
		m_ppArray[i] = m_pArray + i;
	}

	return true;
}

template<class T>
void CListArray<T>::SetCapacity(long lCapacity)
{
	if (lCapacity < 0)
		lCapacity = 0;
	if (lCapacity > m_lSize)
		lCapacity = m_lSize;

	m_lCapacity = lCapacity;
}

template<class T>
void CListArray<T>::Reset(void)
{
	m_lUsed = 0;
	for (long i = 0; i < m_lSize; i++)
	{
		m_pArray[i].CContent.SetPosID(i);
		m_pArray[i].lPosReverse = i;
		m_pArray[i].lPosID = i;
		m_ppArray[i] = m_pArray + i;
	}
}

template<class T>
void CListArray<T>::Free(void)
{
	m_lSize = 0;
	m_lCapacity = 0;
	m_lUsed = 0;
	if (m_ppArray)
	{
		delete [] m_ppArray;
		m_ppArray = NULL;
	}
	if (m_pArray)
	{
		delete [] m_pArray;
		m_pArray = NULL;
	}
}

template<class T>
T* CListArray<T>::Add(T* pCObj)
{
	if (m_lCapacity <= 0 || m_lUsed >= m_lCapacity)
		return NULL;

	long	lPosID = pCObj->GetPosID();
	if (lPosID < 0 || lPosID >= m_lCapacity)
		lPosID = m_ppArray[m_lUsed]->lPosID;
	if (m_pArray[lPosID].lPosReverse < m_lUsed) // 
		return NULL;

	pCObj->SetPosID(lPosID);
	m_pArray[lPosID].CContent = *pCObj;
	m_ppArray[m_lUsed]->lPosReverse = m_pArray[lPosID].lPosReverse;
	m_ppArray[m_pArray[lPosID].lPosReverse] = m_ppArray[m_lUsed];
	m_ppArray[m_lUsed] = m_pArray + lPosID;
	m_pArray[lPosID].lPosReverse = m_lUsed;
	//long	lNextUsed = m_lUsed + 1;
	//if (lNextUsed >= m_lCapacity - 1)
	//	lNextUsed = m_lUsed;
	//m_ppArray[m_lUsed]->lPosReverse = m_ppArray[lNextUsed]->lPosReverse;
	//m_ppArray[lNextUsed]->lPosReverse = m_pArray[lPosID].lPosReverse;
	//m_pArray[lPosID].lPosReverse = m_lUsed;
	//m_ppArray[m_pArray[lNextUsed].lPosReverse] = m_ppArray[lNextUsed];
	//m_ppArray[lNextUsed] = m_ppArray[m_lUsed];
	//m_ppArray[m_lUsed] = m_pArray + lPosID;
	m_lUsed++;

	return &m_pArray[lPosID].CContent;
}

template<class T>
bool CListArray<T>::Del(T* pCObj)
{
	return Del(pCObj->GetPosID());
}

template<class T>
bool CListArray<T>::Del(long lPosID)
{
	if (m_lUsed <= 0)
		return false;
	if (lPosID < 0 || lPosID >= m_lCapacity)
		return false;
	if (m_pArray[lPosID].lPosReverse >= m_lUsed) // 
		return true;

	m_lUsed--;
	m_ppArray[m_lUsed]->lPosReverse = m_pArray[lPosID].lPosReverse;
	m_pArray[lPosID].lPosReverse = m_lUsed;
	m_ppArray[m_ppArray[m_lUsed]->lPosReverse] = m_ppArray[m_lUsed];
	m_ppArray[m_lUsed] = m_pArray + lPosID;

	if (m_lSearchID == m_lUsed) m_lSearchID--;

	return true;
}

template<class T>
T* CListArray<T>::Get(long lPosID)
{
	if (lPosID < 0 || lPosID >= m_lCapacity)
		return NULL;
	if (m_pArray[lPosID].lPosReverse >= m_lUsed) // 
		return NULL;
	return &m_pArray[lPosID].CContent;
}

template<class T>
T* CListArray<T>::GetNext(void)
{
	if (m_lSearchID < 0)
		return NULL;
	return &m_ppArray[m_lSearchID--]->CContent;
}

//=============================================================================
template <class T>
class CResidentList
{
public:
	CResidentList();
	~CResidentList();

	bool	Init(long lPreNum = 0);
	void	Free(void);

	T*		Add(T* pCObj);
	bool	Del(T* pCObj);

	void	BeginGet(void) {m_pSSearchPos = m_pSUsedQueue;}
	T*		GetNext(void);

protected:

private:
	struct SUnit
	{
		T		CContent;
		SUnit*	pSNext;
		SUnit*	pSLast;
	};

	SUnit*	m_pSUsedQueue;
	SUnit*	m_pSFreeQueue;

	SUnit*	m_pSSearchPos;

};

template<class T>
CResidentList<T>::CResidentList()
{
	m_pSUsedQueue = NULL;
	m_pSFreeQueue = NULL;
}

template<class T>
CResidentList<T>::~CResidentList()
{
	Free();
}

template<class T>
bool CResidentList<T>::Init(long lPreNum)
{
	m_pSUsedQueue = NULL;
	m_pSFreeQueue = NULL;

	if (lPreNum < 0)
		return false;
	if (lPreNum == 0)
		return true;
	SUnit	*pSCarrier;
	for (long i = 0; i < lPreNum; i++)
	{
		pSCarrier = new SUnit;
		if (!pSCarrier)
			return false;
		pSCarrier->pSNext = m_pSFreeQueue;
		pSCarrier->pSLast = 0;
		if (m_pSFreeQueue)
			m_pSFreeQueue->pSLast = pSCarrier;
		m_pSFreeQueue = pSCarrier;
	}

	return true;
}

template<class T>
void CResidentList<T>::Free(void)
{
	SUnit	*pSCarrier;

	pSCarrier = m_pSUsedQueue;
	while (pSCarrier)
	{
		m_pSUsedQueue = pSCarrier->pSNext;
		delete pSCarrier;
		pSCarrier = m_pSUsedQueue;
	}

	pSCarrier = m_pSFreeQueue;
	while (pSCarrier)
	{
		m_pSFreeQueue = pSCarrier->pSNext;
		delete pSCarrier;
		pSCarrier = m_pSFreeQueue;
	}
}

template<class T>
T* CResidentList<T>::Add(T* pCObj)
{
	if (!pCObj)
		return NULL;

	SUnit	*pSCarrier;
	if (m_pSFreeQueue) // 
	{
		pSCarrier = m_pSFreeQueue;
		m_pSFreeQueue = pSCarrier->pSNext;
		if (m_pSFreeQueue)
			m_pSFreeQueue->pSLast = 0;
	}
	else // 
	{
		pSCarrier = new SUnit;
		if (!pSCarrier)
			return NULL;
	}

	pSCarrier->pSNext = m_pSUsedQueue;
	pSCarrier->pSLast = NULL;
	if (m_pSUsedQueue)
		m_pSUsedQueue->pSLast = pSCarrier;
	m_pSUsedQueue = pSCarrier;

	pCObj->SetPos(pSCarrier);
	pSCarrier->CContent = *pCObj;

	return &pSCarrier->CContent;
}

template<class T>
bool CResidentList<T>::Del(T* pCObj)
{
	if (!pCObj)
		return false;
	SUnit*	pSUnit = (SUnit*)pCObj->GetPos();
	if (!pSUnit)
		return false;

	if (m_pSSearchPos == pSUnit)
		m_pSSearchPos = pSUnit->pSNext;

	if (pSUnit->pSLast)
		pSUnit->pSLast->pSNext = pSUnit->pSNext;
	if (pSUnit->pSNext)
		pSUnit->pSNext->pSLast = pSUnit->pSLast;
	if (m_pSUsedQueue == pSUnit)
		if (m_pSUsedQueue = pSUnit->pSNext)
			m_pSUsedQueue->pSLast = 0;
	pSUnit->pSNext = m_pSFreeQueue;
	pSUnit->pSLast = NULL;
	if (m_pSFreeQueue)
		m_pSFreeQueue->pSLast = pSUnit;
	m_pSFreeQueue = pSUnit;

	return true;
}

template<class T>
T* CResidentList<T>::GetNext(void)
{
	SUnit	*pSRet = m_pSSearchPos;

	if (m_pSSearchPos)
		m_pSSearchPos = m_pSSearchPos->pSNext;

	if (pSRet)
		return &pSRet->CContent;
	else
		return NULL;
}

#endif // TOOLCLASS_H
