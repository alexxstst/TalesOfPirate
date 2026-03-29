// GameServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"                           

#include "GameAppNet.h"
#include "GameApp.h"
#include "SystemDialog.h"
#include "Config.h"
#include "CrushSystem.h"
#include "GameDB.h"


// #pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" ) //  

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
pi_LeakReporter pi_leakReporter("gamememleak.log");
CResourceBundleManage g_ResourceBundleManage("GameServer.loc"); //Add by lark.li 20080304

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
	setvbuf(stderr, nullptr, _IONBF, 0);

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	C_TITLE("GameServer.exe")
	C_PRINT("Loading %s\n", szConfigFileN);

	::SetThreadName("main");
	TalesOfPirate::Utils::Crush::SetGlobalCRTExceptionBehavior();
	TalesOfPirate::Utils::Crush::SetPerThreadCRTExceptionBehavior();
	TalesOfPirate::Utils::Crush::SetupDumpSetting("log\\game_server\\dumps");
	g_logManager.InitLogger("log\\game_server");
	g_logManager.EnableGlobalConsole(true);

	DisableCloseButton();

	// SEHTranslator translator;

	if (argc >= 2)
	{
		strncpy(szConfigFileN, argv[1], defCONFIG_FILE_NAME_LEN - 1);
		szConfigFileN[defCONFIG_FILE_NAME_LEN - 1] = '\0';
	}
	if (!g_Config.Load(szConfigFileN))
	{
		ToLogService("common", "config init...Fail!");
		return FALSE;		
	}
	if (!g_Command.Load("cmd.cfg"))
	{
		g_Command.SetDefault();
	}

#ifdef __CATCH	
    ToLogService("common", "Define __CATCH");
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
	ToLogService("common", "game map server succeed to exit"); 
	return 0;
}
 



//_DBC_USING

#ifdef USE_IOCP

#else
// CorsairsNet: ThreadPool     TcpClient   
#endif


extern DWORD WINAPI g_GameLogicProcess(LPVOID lpParameter);
BOOL GameServer_Begin()
{
	_setmaxstdio(2048);

	//LG("init", "[%s]...\n", g_Config.m_szName);
ToLogService("common", "game map server [{}] startup...", g_Config.m_szName);

	g_pGameApp = new CGameApp();
	if(!g_pGameApp->Init())
	{
		//LG("init", "GameApp , !\n");
		ToLogService("common", "GameApp initialization failed, exit!");
		return FALSE;
	}
	
#ifdef USE_IOCP
    cfl_iocpapp::startup();

    g_mygmsvr = new myiocpclt();
    g_mygmsvr->init_gtconn(&g_Config);

#else
    net::InitWinSock();

	g_gmsvr	= new GameServerApp();
	// connect thread    GameServerApp
	ToLogService("common", "startup Gate server connect thread...");
#endif

	// 
	//LG("init", "...\n");
	ToLogService("common", "startup game thread...");
	DWORD	dwThreadID;
	hGameT = CreateThread(NULL, 0, g_GameLogicProcess, 0, 0, &dwThreadID);
	ToLogService("common", "Game Thread ID = {}", dwThreadID);
	//

	//LG("init",  "Win32 \n");
	ToLogService("common", "start create Win32 control dialog box");
	HINSTANCE hInst = GetModuleHandle(0);
	CreateMainDialog(hInst, NULL);

	//       CGameApp::Log
	g_pGameApp->Log("restart", "GameServer restart", g_Config.m_szMapList[0], "", "", "");
	
	return TRUE;
}


void GameServer_End()
{
	//LG("init", "\n");
	ToLogService("common", "start to exit game map server");
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

	net::CleanupWinSock();
#endif

}

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


