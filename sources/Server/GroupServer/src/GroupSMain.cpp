// GroupServerApp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "GroupServerApp.h"

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

	T_B

	TcpCommApp::WSAStartup();
	ThreadPool	*l_proc = ThreadPool::CreatePool(8,8,2048);
	ThreadPool	*l_comm = ThreadPool::CreatePool(4,4,512,THREAD_PRIORITY_ABOVE_NORMAL);

	try{
		g_gpsvr	=new GroupServerApp(l_proc,l_comm);
	}catch(...)
	{
		l_comm->DestroyPool();
		l_proc->DestroyPool();
		TcpCommApp::WSACleanup();
		Sleep(10*1000);
		return -1;
	}
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
		else if(str =="logbak")
		{
			LogStream::Backup();
		}
		else
		{
			std::cout<<RES_STRING(GP_MAIN_CPP_00003)<<std::endl;
		}
	}
	//if(!g_exit)
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

	T_FINAL
	return 0;
}
