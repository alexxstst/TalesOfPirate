#include "Stdafx.h"
#include "IMusicThread.h"

#include <time.h>
#include <audiosdl.h>

#include "CrushSystem.h"


BOOL GThreadLoopEnd = FALSE;

class CMusicThread : public IMusicThread
{
public:
	CMusicThread();
	virtual ~CMusicThread();

	virtual void	SetThreadPriority( EThreadPriority NewPriority );
	virtual bool	Begin();
	virtual bool	Resume();
	virtual bool	Suspend();
	virtual bool	Terminate();
	virtual	void	Play( unsigned long MusicID, bool Loop = false );
	
	UINT	Run();
	bool	Stop();

private:
	CRITICAL_SECTION CriticalSection;
	HANDLE			m_hThread;
	DWORD			m_dwThreadID;
	EThreadPriority	m_ThreadPriority;
	DWORD			m_dwMusicID;
	bool			m_bLoop;
	DWORD			m_dwLastTime;
};

DWORD WINAPI ThreadStartRoutine( LPVOID lpThreadParameter )
{
	TalesOfPirate::Utils::Crush::SetPerThreadCRTExceptionBehavior();
	return ( ( CMusicThread* ) lpThreadParameter )->Run();
}

CMusicThread::CMusicThread()
{
	m_hThread = NULL;
	m_dwThreadID = 0xFFFFFFFF;
	m_ThreadPriority = TPri_AboveNormal;
	m_dwMusicID = 0;
	m_bLoop = false;

//	InitializeCriticalSection( &CriticalSection );

}

CMusicThread::~CMusicThread()
{
//	Terminate();

//	DeleteCriticalSection( &CriticalSection );
}

bool CMusicThread::Begin()
{
	if( !m_hThread )
	{
		GThreadLoopEnd = FALSE;

		m_hThread = CreateThread( NULL, 0, ThreadStartRoutine, this, 0, &m_dwThreadID );
		//if( m_hThread )
		//	SetThreadIdealProcessor( m_hThread, 1 );
	}

	return NULL != m_hThread;
}


bool CMusicThread::Resume()
{
	return -1 != ResumeThread( m_hThread );
}

bool CMusicThread::Suspend()
{
	return -1 != SuspendThread( m_hThread );
}

bool CMusicThread::Terminate()
{	
//	EnterCriticalSection( &CriticalSection );

//	if( m_hThread )
	{
		Stop();

//		BOOL bOK = CloseHandle( m_hThread );
//		m_hThread = NULL;
//		GThreadLoopEnd = TRUE;

//		LeaveCriticalSection( &CriticalSection );

//		return bOK == TRUE;
	}

//	LeaveCriticalSection( &CriticalSection );

	return true;
}

void CMusicThread::SetThreadPriority( EThreadPriority NewPriority )
{
	if( NewPriority != m_ThreadPriority )
	{
		m_ThreadPriority = NewPriority;
		::SetThreadPriority( m_hThread,
			m_ThreadPriority == TPri_AboveNormal ? THREAD_PRIORITY_ABOVE_NORMAL :
		m_ThreadPriority == TPri_BelowNormal ? THREAD_PRIORITY_BELOW_NORMAL :
		THREAD_PRIORITY_NORMAL);
	}
}

void CMusicThread::Play( unsigned long MusicID, bool Loop )
{
//	EnterCriticalSection( &CriticalSection );

	if( MusicID == m_dwMusicID )
	{
//		LeaveCriticalSection( &CriticalSection );
		return;
	}

	if( m_dwMusicID )
		Terminate();

	m_dwMusicID = MusicID;
	m_bLoop = Loop;
	
	Run();

//	LeaveCriticalSection( &CriticalSection );
}

bool CMusicThread::Stop()
{
	AudioSDL::get_instance()->stop( m_dwMusicID );
	m_dwMusicID = 0;

	return true;
}

UINT CMusicThread::Run()
{
//	if( m_ThreadPriority != TPri_Normal )
//		SetThreadPriority( m_ThreadPriority );

	try
	{
		AudioSDL::get_instance()->play( m_dwMusicID, m_bLoop );
	}
	catch(...)
	{
	}

/*	bool LoopEnd = false;
	while( !GThreadLoopEnd && !LoopEnd )
	{
		Sleep( 30 );

		if( !m_dwMusicID )
		{
			int index = 0;
			index ++;
		}

		if( m_dwMusicID )
			LoopEnd = AudioSDL::get_instance()->is_stopped( m_dwMusicID ) && ( !m_bLoop );
	}*/

	return 0;
}

static CMusicThread MusicThread;
IMusicThread* GMusicThread = &MusicThread;
