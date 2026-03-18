// GroupServerApp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <thread>
#include "GroupServerApp.h"
#include "Timer.h"

extern dbc::TimerMgr g_timermgr;

HANDLE hConsole = NULL;

// ��ʼ��˳��
#pragma init_seg( lib )
pi_LeakReporter pi_leakReporter("groupememleak.log");
CResourceBundleManage g_ResourceBundleManage("GroupServer.loc"); //Add by alfred.shi 20080130

// binary compatible
int _tmain(int argc, _TCHAR* argv[])
{
	setvbuf(stdout, nullptr, _IONBF, 0);
	setvbuf(stderr, nullptr, _IONBF, 0);

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	C_TITLE("GroupServer.exe")
	C_PRINT("Loading GroupServer.cfg...\n");

	//SEHTranslator translator;


	net::InitWinSock();

	try{
		g_gpsvr	=new GroupServerApp();
	}catch(...)
	{
		net::CleanupWinSock();
		Sleep(10*1000);
		return -1;
	}

	// Запуск таймеров (AddStatLog, DisableCloseButton, GMBBS) в отдельном потоке
	g_timermgr.Start();

	// stdin в отдельном потоке, чтобы не блокировать main loop
	std::thread consoleThread([&]() {
		while (!g_exit) {
			std::string str;
			str.reserve(256);
			std::cout << RES_STRING(GP_MAIN_CPP_00001);
			std::cin >> str;
			if (str == "exit" || g_exit) {
				std::cout << RES_STRING(GP_MAIN_CPP_00002) << std::endl;
				g_exit = 1;
				break;
			}
			else if (str == "logbak") {
				LogStream::Backup();
			}
			else {
				std::cout << RES_STRING(GP_MAIN_CPP_00003) << std::endl;
			}
		}
	});
	consoleThread.detach();

	// Main loop: PollNetwork + Sleep(1)
	while (!g_exit) {
		g_gpsvr->PollNetwork();
		Sleep(1);
	}

	{
		g_exit	=1;
		while(g_ref)
		{
			Sleep(1);
		}
		delete g_gpsvr;

		g_timermgr.Stop();
		net::CleanupWinSock();
		g_exit	=2;
		Sleep(2000);
	}
	while(g_exit !=2)
	{
		Sleep(1);
	}

	return 0;
}
