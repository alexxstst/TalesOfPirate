// GroupServerApp.cpp : Defines the entry point for the console application.
//

#include "CrushSystem.h"
#include "stdafx.h"
#include "GroupServerApp.h"

HANDLE hConsole = NULL;

// 
#pragma init_seg( lib )
CLanguageRecord g_ResourceBundleManage{}; //Add by lark.li 20080130
CLanguageRecord& CLanguageRecordInstance() {
	if (g_ResourceBundleManage.GetRecordStringCount() == 0) {
		g_ResourceBundleManage.LoadFromTxtStringFile("server-localization.txt");
	}
	return g_ResourceBundleManage;
}

// binary compatible
int _tmain(int argc, _TCHAR* argv[])
{
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	setvbuf(stdout, nullptr, _IONBF, 0);
	setvbuf(stderr, nullptr, _IONBF, 0);

	C_TITLE("GroupServer.exe")
	C_PRINT("Loading GroupServer.cfg...\n");

	std::cout << "Loaded string: " << CLanguageRecordInstance().GetRecordCount() << std::endl;

	::SetThreadName("main");
	TalesOfPirate::Utils::Crush::SetGlobalCRTExceptionBehavior();
	TalesOfPirate::Utils::Crush::SetPerThreadCRTExceptionBehavior();
	TalesOfPirate::Utils::Crush::SetupDumpSetting("log\\group\\dumps");
	g_logManager.InitLogger("log\\group");




	TcpCommApp::WSAStartup();
	ThreadPool	*l_proc = ThreadPool::CreatePool(8,8,2048);
	ThreadPool	*l_comm = ThreadPool::CreatePool(4,4,512,THREAD_PRIORITY_ABOVE_NORMAL);

	g_gpsvr	=new GroupServerApp(l_proc,l_comm);

	while(!g_exit)
	{
		std::string str;
		str.reserve(256);

        std::cout<<RES_STRING(GP_MAIN_CPP_00001);
		std::cin >> str;
		if(str =="exit" ||g_exit)
		{
			std::cout<<RES_STRING(GP_MAIN_CPP_00002)<<std::endl;
			break;
		}
		else
		{
			std::cout<<RES_STRING(GP_MAIN_CPP_00003)<<std::endl;
		}
	}

	{
		g_exit	=1;
		while(g_ref)
		{
			Sleep(1);
		}
		delete g_gpsvr;

		l_comm->DestroyPool();
		l_proc->DestroyPool();
		TcpCommApp::WSACleanup();
		g_exit	=2;
		Sleep(2000);
	}
	while(g_exit !=2)
	{
		Sleep(1);
	}


	return 0;
}
