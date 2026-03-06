#pragma once

//=============================================================================
// FileName: Expand.h
// Creater: 
// Date: 2004.11.22
// Comment: 
//=============================================================================

#ifndef TIMER_H
#define TIMER_H

class CTimer
{
public:

    void Begin(DWORD dwInterval)
    {
        _dwLastTick = GetTickCount();
        _dwInterval = dwInterval;
    }

    //BOOL IsOK(DWORD dwCurTime)
    //{
    //    if((dwCurTime - _dwLastTick) > _dwInterval)
    //    {
    //        _dwLastTick = dwCurTime;
    //        return TRUE;
    //    }
    //    return FALSE;
    //}
    DWORD IsOK(DWORD dwCurTime)
    {
		if (dwCurTime < _dwLastTick)
		{
			_dwLastTick = dwCurTime;
			return 0;
		}
		DWORD	dwExecTime = (dwCurTime - _dwLastTick) / _dwInterval;
		if (dwExecTime)
			_dwLastTick = dwCurTime - ((dwCurTime - _dwLastTick) % _dwInterval);

		return dwExecTime;
    }

	void Reset(void) {_dwLastTick = GetTickCount();}
	DWORD GetInterval(void) {return _dwInterval;}
	void SetInterval(DWORD dwInterval) {_dwInterval = dwInterval;}

protected:

    DWORD _dwLastTick;
    DWORD _dwInterval;
};

#endif // TIMER_H
