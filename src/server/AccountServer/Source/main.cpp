// ,,, - by Arcol

// main.cpp : Defines the entry point for the console application.
//

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "stdafx.h"
#include "resource.h"
#include "AccountServer2.h"
#include <signal.h>
#include <CommCtrl.h>

#include "CrushSystem.h"
#include "GlobalVariable.h"

#include "inifile.h"
#include "LanguageRecord.h"

#define AUTHUPDATE_TIMER 1
#define GROUPUPDATE_TIMER 2

ThreadPool* comm = NULL;
ThreadPool* proc = NULL;
AuthThreadPool atp;
bool bExit = false;

#pragma init_seg( lib )
CLanguageRecord  g_ResourceBundleManage{};	//Add by lark.li 20080330

CLanguageRecord& CLanguageRecordInstance() {
    if (g_ResourceBundleManage.GetRecordStringCount() == 0) {
        g_ResourceBundleManage.LoadFromTxtStringFile("server-localization.txt");
    }
    return g_ResourceBundleManage;
}


//std::string g_BTSAddr[2] = {"61.152.115.79:7243", "61.152.115.79:7243"}; BTS
//std::string g_BTSAddr[2] = {"61.152.115.172:7243", "61.152.115.173:7243"};

void __cdecl Ctrlc_Dispatch(int sig)
{
    if (!bExit) {
		C_PRINT(RES_STRING(AS_MAIN_CPP_00007));
	    PostMessage(g_hMainWnd, WM_CLOSE, 0, 0);
        bExit = TRUE;
	}
}

BOOL UpdateBTS(int index, char const* strStat)
{
    HWND hBTSList = GetDlgItem(g_hMainWnd, IDC_BILLLIST);
    ListView_SetItemText(hBTSList, index, 2, (LPSTR)strStat);
    return TRUE;
}
LRESULT OnTimer(HWND hwnd, UINT idEvent)
{
    if (idEvent == AUTHUPDATE_TIMER) {
        LVITEM item;
        item.mask = LVIF_TEXT;
        char buf[80] = {0};
        HWND hAuthList = GetDlgItem(hwnd, IDC_AUTHLIST);
        for (char i = 0; i < AuthThreadPool::AT_MAXNUM; ++ i) {
            item.iItem = i;

            item.iSubItem = 1;
            sprintf(buf, "%02d", AuthThreadPool::RunLabel[i]);
            item.pszText = (LPSTR)buf;
            ListView_SetItem(hAuthList, &item);

            item.iSubItem = 2;
            sprintf(buf, "%04d/%04d", AuthThreadPool::RunLast[i], AuthThreadPool::RunConsume[i]);
            item.pszText = (LPSTR)buf;
            ListView_SetItem(hAuthList, &item);
        }

        HWND hQueueCap = GetDlgItem(hwnd, IDC_QUEUECAP);
        //sprintf(buf, " %d", g_Auth.GetPkTotal());
        sprintf(buf, RES_STRING(AS_MAIN_CPP_00024), g_Auth.GetPkTotal());
        SetWindowText(hQueueCap, (LPCTSTR)buf);

        HWND hTaskCnt = GetDlgItem(hwnd, IDC_TASKCNT);
        //sprintf(buf, " %d", proc->GetTaskCount());
        sprintf(buf, RES_STRING(AS_MAIN_CPP_00025), proc->GetTaskCount());
        SetWindowText(hTaskCnt, (LPCTSTR)buf);
    }

    return 0;
}
//#include <string.h>

HANDLE hConsole = NULL;

int main(int argc, char* argv[])
{
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	setvbuf(stdout, nullptr, _IONBF, 0);
	setvbuf(stderr, nullptr, _IONBF, 0);

	C_TITLE("AccountServer.exe")
	C_PRINT("Loading AccountServer.cfg...\n");

	std::cout << "Loaded string: " << CLanguageRecordInstance().GetRecordCount() << std::endl;

	::SetThreadName("main");
	TalesOfPirate::Utils::Crush::SetGlobalCRTExceptionBehavior();
	TalesOfPirate::Utils::Crush::SetPerThreadCRTExceptionBehavior();
	TalesOfPirate::Utils::Crush::SetupDumpSetting("log\\account\\dumps");
	g_logManager.InitLogger("log\\account");


    // 
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );		//Add by Arcol (2005-12-2)

    signal(SIGINT, Ctrlc_Dispatch);

	if (!g_MainDBHandle.CreateObject())
	{
		C_PRINT("failed\n");
		printf(RES_STRING(AS_MAIN_CPP_00003));
		system("pause");
		return -1;
	}


    // 
	//bt.Launch();
	//g_BillThread.Launch();
    atp.Launch();

    // 
    TcpCommApp::WSAStartup();
    comm = ThreadPool::CreatePool(10, 10, 256);

    int nProcCnt = 2 * AuthThreadPool::AT_MAXNUM;
    proc = ThreadPool::CreatePool(nProcCnt, nProcCnt, 2048);
    try {
        g_As2 = new AccountServer2(proc, comm);
    } catch (excp& e) {
        printf("%s\n", e.what());
        comm->DestroyPool();
        TcpCommApp::WSACleanup();
        Sleep(10 * 1000);
        return -1;
    } catch (...) {
   		printf(RES_STRING(AS_MAIN_CPP_00006));
		comm->DestroyPool();
        TcpCommApp::WSACleanup();
        Sleep(10 * 1000);
        return -2;
    }

    // 
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // 
    delete g_As2;
    if (comm != NULL) comm->DestroyPool();
    if (proc != NULL) proc->DestroyPool();
    TcpCommApp::WSACleanup();

    // 
    //bt.NotifyToExit();
	//g_BillThread.NotifyToExit();
    atp.NotifyToExit();

    atp.WaitForExit();
	//g_BillThread.WaitForExit(-1);

    // 
    ToLogService("RunLabel", "");
    for (char i = 0; i < AuthThreadPool::AT_MAXNUM; ++ i) {
        ToLogService("RunLabel", "{:02} {:04}", (int)AuthThreadPool::RunLabel[i],
            (DWORD)AuthThreadPool::RunConsume[i]);
    }
    ToLogService("RunLabel", "");

	g_MainDBHandle.ReleaseObject();



    return 0;
}


