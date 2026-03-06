#include "stdafx.h"
#include "steadyframe.h"

DWORD CSteadyFrame::_dwFPS = 0;
bool CSteadyFrame::_bFramerate =false;

bool CSteadyFrame::Init()
{
	DWORD dwThread = 0;

	_dwRefreshFPS = _bFramerate ? 60 : 30;

	_dwTotalTime=0; 
	_dwRunCount=0;	

	hThread = CreateThread( NULL, 0, _SleepThreadProc, this, 0, &dwThread);

	if( hThread != NULL )
	{
		LG( "threadid", "%d:%s\n", dwThread, "_SleepThreadProc" );
		return true;
	}
	return false;
}

// Add by lark.li 20080923 begin
void CSteadyFrame::Exit()
{
	DWORD exitCode;
	if(GetExitCodeThread(hThread, &exitCode))
	{
		if(exitCode == STILL_ACTIVE)
		{
			if(SuspendThread(hThread) >= 0)
			{
				if(TerminateThread(hThread, 0))
				{
					LG( "threadid", "_SleepThreadProc end\n");
					::CloseHandle(hThread);
				}
			}
			else
			{
				DWORD error = ::GetLastError();
				LG( "threadid", "_SleepThreadProc error %d\n", error);
			}
		}
	}
}
// End

void CSteadyFrame::_Sleep()
{
	int nCount=0;
	DWORD dwTime = GetTickCount();
	DWORD dwCurTime = dwTime;
	float fRate = 0.0;
	while( true )
	{
		Sleep( _dwTimeSpace );

		LG( "_Sleep", "%d %d\n", dwTime, nCount);

		InterlockedIncrement( &_lRun );

		nCount++;
		if( nCount >= 60 )
		{
			dwCurTime = GetTickCount();
			fRate = (float)_dwTotalTime / (float)(dwCurTime - dwTime);
			if( fRate < 0.5f )
			{
				_dwRefreshFPS = _dwFPS + 3;
			}
			else if( fRate < 0.6f )
			{
				_dwRefreshFPS = _dwFPS + 2;
			}
			else if( fRate < 0.7f )
			{
				_dwRefreshFPS = _dwFPS + 1;
			}
			else if( fRate > 0.98f )
			{
				_dwRefreshFPS = _dwFPS - 3;
			}
			else if( fRate > 0.95f )
			{
				_dwRefreshFPS = _dwFPS - 2;
			}
			else if( fRate > 0.9f )
			{
				_dwRefreshFPS = _dwFPS - 1;
			}
			else
			{
				_dwRefreshFPS = _dwFPS;
			}
			if(GetIsFramerate60()){
			if( _dwRefreshFPS>60 )
				_dwRefreshFPS = 60;
			else if( _dwRefreshFPS<8 )
				_dwRefreshFPS = 8;
			}
			else{
				if( _dwRefreshFPS>30 )
					_dwRefreshFPS = 30;
				else if( _dwRefreshFPS<8 )
				_dwRefreshFPS = 8;


			}


			nCount=0;
			dwTime = dwCurTime;
			_dwTotalTime=0; 
			_dwRunCount=0;
		}
	}
}

