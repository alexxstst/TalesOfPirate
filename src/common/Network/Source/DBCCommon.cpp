#include <string>
#include "DBCCommon.h"
#include "PreAlloc.h"

_DBC_USING

//=======PreAllocHeapPtr=======================================================================
uLong PreAllocStru::Size()	//Override
{
	return __preAllocHeapPtr?__preAllocHeapPtr->m_unitsize:(__preAllocHeap?__preAllocHeap->m_unitsize:0);
}
void PreAllocStru::Free()
{
	if(__preAllocHeapPtr)
	{
		*__preAllocHeapPtr<<this;
	}else if(__preAllocHeap)
	{
		*__preAllocHeap<<this;
	};
};

//======InterLockedLong=======================================
InterLockedLong::InterLockedLong(LONG lInitVal)
{
	InterlockedExchange(&m_plVal,lInitVal);
}
InterLockedLong::InterLockedLong(const InterLockedLong &val)
{
	InterlockedExchange(&m_plVal,val);
}
LONG InterLockedLong::Increment()								//The return value is the resulting incremented value
{
	LONG	l_ret	=0;/*
	if(g_isSuportAcquire >0)
	{
		return	InterlockedIncrementAcquire(&m_plVal);
	}else if(g_isSuportAcquire <0)*/
	{
		return	InterlockedIncrement(&m_plVal);
	}/*else
	{
		return -1;
	}*/
}
LONG InterLockedLong::Decrement()								//The return value is the resulting decremented value
{/*
	if(g_isSuportAcquire >0)
	{
		return	InterlockedDecrementAcquire(&m_plVal);
	}else if(g_isSuportAcquire <0)*/
	{
		return	InterlockedDecrement(&m_plVal);
	}/*else
	{
		return -1;
	}*/
}
LONG InterLockedLong::Add(LONG Value)								//The return value is the initial value
{
	return InterlockedExchangeAdd(&m_plVal,Value);
}
LONG InterLockedLong::Assign(LONG newval)							//The return value is the initial value
{
	return InterlockedExchange(&m_plVal,newval);
}
LONG InterLockedLong::CompareAssign(LONG Comperand,LONG newval)		//,The return value is the initial value
{
	/*
	if(g_isSuportAcquire >0)
	{
		return InterlockedCompareExchangeAcquire(&m_plVal,newval,Comperand);
	}else if(g_isSuportAcquire <0)*/
	{
		return InterlockedCompareExchange(&m_plVal,newval,Comperand);
	}/*else
	{
		return -1;
	}*/
}
