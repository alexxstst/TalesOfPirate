
#include "stdafx.h"
#include "GameWG.h"
#include "PacketCmd.h"
#include "CrushSystem.h"

#include <windows.h>
#include <process.h>
#include <tlhelp32.h>


CGameWG::CGameWG(void)
	: m_hThread(0)
{
}


CGameWG::~CGameWG(void)
{
	SafeTerminateThread();

	m_lstModule.clear();
}


// 
bool CGameWG::RefreshModule(void)
{
    bool          bRet        = false;

	try
	{
		MODULEENTRY32 me32        = {0};
		std::string   strModule;

		std::unique_ptr<void, decltype(&CloseHandle)> hModuleSnap(CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, ::GetCurrentProcessId()), &CloseHandle);
		if (hModuleSnap.get() == INVALID_HANDLE_VALUE)
			return false;

		me32.dwSize = sizeof(MODULEENTRY32); 

		if(Module32First(hModuleSnap.get(), &me32)) 
		{
			// 
			do
			{
				strModule = me32.szModule;
				m_lstModule.insert(strModule);
			}
			while(Module32Next(hModuleSnap.get(), &me32));

			bRet = true;
		}
		else
		{
			// 
			bRet = false;
		}
	}
	catch(...)
	{
		
	}

    return bRet;
}


// 
bool CGameWG::IsUseHdts(void)
{
	if (m_lstModule.contains("hookit.dll"))
	{
		return true;
	}

	return false;
}


// 
void CGameWG::BeginThread(void)
{
    m_hThread = (HANDLE)_beginthreadex(0, 0, Run, this, 0, 0);
}


// 
void CGameWG::SafeTerminateThread()
{
	if(m_hThread)
	{
		TerminateThread(m_hThread, 0);
		CloseHandle(m_hThread);

		m_hThread = 0;
	}
}


//
UINT CALLBACK CGameWG::Run(void* param)
{
	::SetThreadName("game-wg");
	TalesOfPirate::Utils::Crush::SetPerThreadCRTExceptionBehavior();
	CGameWG* pGameWG = (CGameWG*)(param);

	for(;;)
	{
		Sleep(60 * 1000);	//

		if(! g_NetIF || ! g_NetIF->IsConnected())
		{
			// 
			continue;
		}

		if(! pGameWG->RefreshModule())
		{
			// 
			continue;
		}

		if(pGameWG->IsUseHdts())
		{
			// 

			CS_ReportWG(GetLanguageString(143).c_str());
			break;
		}

	}

	return 0;
}

