#define _CRTDBG_MAP_ALLOC
#include "stdafx.h"
#include <stdlib.h>
#include <crtdbg.h>

#include "MyThread.h"


MyThread::MyThread()
{
	m_hThread = NULL;
	m_dwThreadId = 0;
	m_bExitFlag = false;
	m_bExitThrd = true;
}

MyThread::~MyThread()
{
	NotifyToExit();
	WaitForExit(-1);
}

bool MyThread::Launch()
{
	m_hThread = CreateThread(NULL, 0, MyThread::ThreadProc, this, CREATE_SUSPENDED, &m_dwThreadId);
	if (m_hThread == NULL) return false;

	// 
	m_bExitFlag = false;
	m_bExitThrd = false;

	// 
	ResumeThread(m_hThread);
	return true;
}

void MyThread::NotifyToExit()
{
	m_bExitFlag = true;
}

int MyThread::WaitForExit(int ms)
{
	while (!m_bExitThrd) {
		Sleep(1000);
	}

	// 
	Sleep(5);
	DWORD dwThreadExitCode = 0;
	GetExitCodeThread(m_hThread, &dwThreadExitCode);
	return (int)dwThreadExitCode;
}

DWORD WINAPI MyThread::ThreadProc(LPVOID lpParameter)
{
	MyThread* This = (MyThread *)lpParameter;
	return (DWORD)This->Run();
}

int MyThread::Run()
{
	ExitThread();
	return 0;
}
