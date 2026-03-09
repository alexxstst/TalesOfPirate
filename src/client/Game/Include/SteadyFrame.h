#pragma once
#include "CrushSystem.h"
#include "UISystemForm.h"
// ,
// :
class CSteadyFrame
{
public:
	CSteadyFrame() {
		g_stUISystem.m_sysProp.Load("user\\system.ini");
		SetFPS(g_stUISystem.m_sysProp.m_gameOption.bFramerate ? 60 : 30);
	}
	
	static bool GetIsFramerate60()		{return _bFramerate;}
	static void SetFramerate60(bool v)	{_bFramerate = v;}
	bool	Init();

	static DWORD	GetFPS()	{ return _dwFPS;		}
	void	SetFPS( DWORD v )	{ 
		_dwFPS = v;	
		_dwTimeSpace = (int)(1000.0f/(float)_dwFPS);
	}

	bool	Run(){
		if( _lRun>0 && _lRun>0 )
		{
			_lRun=0;
			_dwCurTime = GetTickCount();

			_dwRunCount++;
			return true;
		}
		return false;
	}
	
	// Add by lark.li 20080923 begin
	void	Exit();
	// End

	DWORD	GetTick()		{ return _dwCurTime;		}
	void	End()			{ 
		_dwTotalTime += GetTickCount() - _dwCurTime;
	}

	void	RefreshFPS()	{ if(_dwFPS!=_dwRefreshFPS) SetFPS(_dwRefreshFPS);	}

private:
	static DWORD WINAPI _SleepThreadProc( LPVOID lpParameter ){
		TalesOfPirate::Utils::Crush::SetPerThreadCRTExceptionBehavior();
		((CSteadyFrame*)lpParameter)->_Sleep();
		return 0;
	}

	void	_Sleep();

static DWORD	_dwFPS;			// FPS,

	long	_lRun;

	DWORD	_dwCurTime;	
	DWORD	_dwTimeSpace;
	static bool	_bFramerate;
	DWORD	_dwTotalTime;
	DWORD	_dwRunCount;

	DWORD	_dwRefreshFPS;

	// Add by lark.li 20080923 begin
	HANDLE hThread;
	// End
};
