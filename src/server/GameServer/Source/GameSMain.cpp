// GameServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"                           

#include "GameAppNet.h"
#include "GameApp.h"
#include "SystemDialog.h"
#include "Config.h"
#include "GameDB.h"

extern BOOL GameServer_Begin();
extern void GameServer_End();
BOOL       g_bGameEnd = FALSE;
CGameApp*  g_pGameApp = NULL;
std::string g_strLogName = "GameServerLog";
HANDLE     hGameT;
HANDLE hConsole = NULL;

void DisableCloseButton();
void AppExit(void);


//#pragma init_seg(user)
#pragma init_seg( lib )
CLanguageRecord  g_ResourceBundleManage{};	//Add by lark.li 20080330

CLanguageRecord& CLanguageRecordInstance() {
	if (g_ResourceBundleManage.GetRecordStringCount() == 0) {
		g_ResourceBundleManage.LoadFromTxtStringFile("server-localization.txt");
	}
	return g_ResourceBundleManage;
}
#include <signal.h>

void sigintHandler(int sig_num)
{
	signal(SIGINT, sigintHandler);
	C_PRINT("Notify Logout to GameServer...\n");
	g_pGameApp->m_CTimerReset.Begin(1000);
	g_pGameApp->m_ulLeftSec = 3;
	fflush(stdout);
}

int main(int argc, char* argv[])
{
	setvbuf(stdout, nullptr, _IONBF, 0);
	//CreateConsole();
	std::cout << "Console" << std::endl;
	printf("Console-printf\n");
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	C_TITLE("GameServer.exe")
	C_PRINT("Loading %s\n", szConfigFileN);

	DisableCloseButton();

	T_B
	if (argc >= 2)
	{
		strncpy(szConfigFileN, argv[1], defCONFIG_FILE_NAME_LEN - 1);
		szConfigFileN[defCONFIG_FILE_NAME_LEN - 1] = '\0';
	}
	if (!g_Config.Load(szConfigFileN))
	{
		LG("init", "config init...Fail!\n");
		return FALSE;		
	}
	if (!g_Command.Load("cmd.cfg"))
	{
		g_Command.SetDefault();
	}

#ifdef __CATCH	
    LG("init", "Define __CATCH\n");
#endif

	atexit(AppExit);
	if(!GameServer_Begin()) 
	{
		return 0;
	}
	signal(SIGINT, sigintHandler);

	MSG msg;
	while(!g_bGameEnd)
	{
		if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
			if(msg.message==WM_QUIT)
			{
				if (!g_bGameEnd)
					g_pGameApp->SaveAllPlayer();
				g_bGameEnd = TRUE;
				break;
			}
		}
		else
		{
			SystemReport(GetTickCount());
			Sleep(50);
		}
	}
	GameServer_End();
	LG("init", "game map server succeed to exit\n"); 
	T_FINAL
	return 0;
}
 



//_DBC_USING

#ifdef USE_IOCP

#else
ThreadPool	*l_proc = NULL;
ThreadPool	*l_comm = NULL;
#endif


extern DWORD WINAPI g_GameLogicProcess(LPVOID lpParameter);
BOOL GameServer_Begin()
{T_B
	_setmaxstdio(2048);

	//LG("init", "[%s]...\n", g_Config.m_szName);
LG("init", "game map server [%s] startup...\n", g_Config.m_szName);

	g_pGameApp = new CGameApp();
	if(!g_pGameApp->Init())
	{
		//LG("init", "GameApp , !\n");
		LG("init", "GameApp initialization failed, exit!\n");
		return FALSE;
	}
	
#ifdef USE_IOCP
    cfl_iocpapp::startup();

    g_mygmsvr = new myiocpclt();
    g_mygmsvr->init_gtconn(&g_Config);

#else
    TcpCommApp::WSAStartup();
    //l_proc = ThreadPool::CreatePool(1, 60, 512);
    l_comm = ThreadPool::CreatePool(8, 8, 512);//,THREAD_PRIORITY_ABOVE_NORMAL);

	g_gmsvr	= new GameServerApp(0, l_comm);

    //  GateServer 
   // LG("init", "Gate...\n");
	 LG("init", "startup Gate server connect thread...\n");
    l_comm->AddTask(new ConnectGateServer(g_gmsvr));
#endif

    //InfoServer
	//LG("init", "...\n");
	//LG("init", "startup information server connect thread...\n");
    //l_comm->AddTask(new ToInfoServer(g_gmsvr));
	
	// 
	//LG("init", "...\n");
	LG("init", "startup game thread...\n");
	DWORD	dwThreadID;
	hGameT = CreateThread(NULL, 0, g_GameLogicProcess, 0, 0, &dwThreadID);
	LG("init", "Game Thread ID = %d\n", dwThreadID);
	//

	//LG("init",  "Win32 \n");
	LG("init",  "start create Win32 control dialog box\n");
	HINSTANCE hInst = GetModuleHandle(0);
	CreateMainDialog(hInst, NULL);

	//Log("", "GameServer", g_Config.m_szMapList[0], "", "", "");
	Log("restart", "GameServer restart", g_Config.m_szMapList[0], "", "", "");
	
	return TRUE;
T_E}


void GameServer_End()
{T_B
	//LG("init", "\n");
	LG("init", "start to exit game map server\n");
	CloseHandle(hGameT);

	HWND hConsole = GetConsoleWindow();
	if(hConsole)
	{
		SendMessage(hConsole, WM_CLOSE, 0, 0);
	}
	
	Sleep(400);

	SAFE_DELETE(g_pGameApp);

#ifdef USE_IOCP
    if (g_mygmsvr != NULL)
    {
        g_mygmsvr->exit();
        Sleep(3000);
        delete g_mygmsvr;
    }

    cfl_iocpapp::cleanup();
#else
	delete g_gmsvr;

	// l_comm->DestroyPool();
	// l_proc->DestroyPool();

	TcpCommApp::WSACleanup();
#endif

T_E}

typedef HWND (*LPGETCONSOLEWINDOW)(void);
void DisableCloseButton()
{
	HMODULE hMod = LoadLibrary("kernel32.dll");

	LPGETCONSOLEWINDOW lpfnGetConsoleWindow =
		(LPGETCONSOLEWINDOW)GetProcAddress(hMod, "GetConsoleWindow");

	HWND hWnd = lpfnGetConsoleWindow();
	HMENU hMenu = GetSystemMenu(hWnd, FALSE);
	if (hMenu != NULL)
	{
		EnableMenuItem(hMenu, SC_CLOSE, MF_BYCOMMAND | MF_GRAYED);
	}

	FreeLibrary(hMod);
}

void AppExit(void)
{
	//int	*p = NULL;
	// *p = 1;
}

/*
 GameServer

 GameServer

  
 
[GameData]
 
Map         
MgrUnit    
Player     
Character  
Item       
Skill      
SkillState 
Mission    

 
GameData

[GameControl]
App        
TimerMgr  
AI        AI

[EventHandler] 

GameServer 

GameControl, AI, 

GameControl  EventHandler Event, AI, 
  EventHandler Event, , , 

EventHandlerEvent, , 

EventHandlerEvent, Modify GameData

EventHandler , 
Modify GameData


Control -> Event 
 





*/


