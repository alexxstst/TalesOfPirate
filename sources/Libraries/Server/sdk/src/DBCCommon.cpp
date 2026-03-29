#include <string>
#include "rstring.h"
#include "DBCCommon.h"
#include "PreAlloc.h"

_DBC_USING

char g_isSuportAcquire		=0;

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
bool isSuportAcquire()
{
	OSVERSIONINFOEX osvi;
	BOOL bOsVersionInfoEx;

	// Try calling GetVersionEx using the OSVERSIONINFOEX structure.
	// If that fails, try using the OSVERSIONINFO structure.

	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

	if( !(bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO *) &osvi)) )
	{
		osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
		if (! GetVersionEx ( (OSVERSIONINFO *) &osvi) ) 
		{
			return false;
		}
	}
	uLong l_ver	=osvi.dwMajorVersion *0x10000 +osvi.dwMinorVersion;
	if (osvi.dwPlatformId ==VER_PLATFORM_WIN32_NT && l_ver >=0x50002)	//Microsoft Windows Server&nbsp;2003 family
	{
		g_isSuportAcquire	=1;
	}else{
		g_isSuportAcquire	=-1;
	}
	return true;
}
//======InterLockedLong=======================================
InterLockedLong::InterLockedLong(LONG lInitVal)
{
	if(!g_isSuportAcquire)
	{
		isSuportAcquire();
	}
	InterlockedExchange(&m_plVal,lInitVal);
}
InterLockedLong::InterLockedLong(const InterLockedLong &val)
{
	if(!g_isSuportAcquire)
	{
		isSuportAcquire();
	}
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
