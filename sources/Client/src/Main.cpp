//------------------------------------------------------------------------
//	2005.3.25	Arcol	Default locale set to Chinese Mainland
//------------------------------------------------------------------------


// MPTest.cpp : Defines the entry point for the application.
//
#include "stdafx.h"

#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
//#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Wingdi.h>

#include "Main.h"
#include "GameApp.h"
#include "SceneObjFile.h"
#include "UIImeinput.h"
#include "GameConfig.h"
#include "PacketCmd.h"
#include "UIsystemform.h"
#include "EffectObj.h"
#include "Lua_Platform.h"
#include "packfile.h"
#include "loginscene.h"
#include "Character.h"
#include "gameloading.h"
#include "script.h"
#include "UICozeForm.h"
#include "CrushSystem.h"

#include "GlobalVar.h"

#include "SelectChaScene.h"

#include "LootFilter.h"
#include "AssetDatabase.h"
#include "LanguageRecordStore.h"


dbc::IniFile g_SystemIni;

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst; // current instance
TCHAR szTitle[MAX_LOADSTRING]; // The title bar text
TCHAR szWindowClass[MAX_LOADSTRING]; // the main window class name

// Forward declarations of functions included in this code module:
ATOM MyRegisterClass(HINSTANCE hInstance, HBRUSH brush);
HWND InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HBRUSH g_bk_brush;

void StdoutRedirect();
int InstallFont(const char* pszPath); // Font installation
void CenterWindow(HWND hWnd); // Center window on screen


BOOL CheckDxVersion(DWORD& ver) //
{
	BOOL ret = 1;
	MPISystemInfo* sys_info = 0;

	MPGUIDCreateObject((LW_VOID**)&sys_info, LW_GUID_SYSTEMINFO);
	if (SUCCEEDED(sys_info->CheckDirectXVersion())) //
	{
		ver = sys_info->GetDirectXVersion();

		if (ver < DX_VERSION_8_1) //
		{
			ret = 0;
		}
	}
	sys_info->Release();

	return ret;
}

bool g_IsAutoUpdate = false; // Flag: when auto-updating, wait for the process to exit before launching updater
long g_nCurrentLogNo = 0;


////////////////////////////////////////
//
//  By Jampe
//  Cooperative login encryption
//  2006/5/22
//
const unsigned char key[] = {0xda, 0x15, 0x1c, 0x10, 0x4f, 0x8c, 0x7a, 0x4a};

//////////////////////////////////////////////////////////////////////

DWORD WINAPI LoadRes(LPVOID lpvThreadParam) // Load resources
{
	g_bLoadRes = 1;

	return 0;
}

HINSTANCE g_hInstance;

int APIENTRY _tWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPTSTR lpCmdLine,
					   int nCmdShow) {
	DWORD dx_ver = DX_VERSION_X_X;


	ToLogService("common", "Define __CATCH");
	// SEHTranslator translator;
	std::string strParam = lpCmdLine;

	::SetThreadName("main");
	TalesOfPirate::Utils::Crush::SetGlobalCRTExceptionBehavior();
	TalesOfPirate::Utils::Crush::SetPerThreadCRTExceptionBehavior();
	TalesOfPirate::Utils::Crush::SetupDumpSetting("log\\game\\dumps");
	g_logManager.InitLogger("log\\game");
	g_logManager.EnableGlobalConsole(true);

	g_SystemIni = dbc::IniFile("./user/system.ini");
	g_Config.Load();

	const auto assetDbPath = g_SystemIni["Assets"].GetString("StringAssetPack", "gamedata.sqlite");
	const auto language = g_SystemIni["Main"].GetString("language", "english");
	AssetDatabase::Instance()->Open(assetDbPath);
	LanguageRecordStore::Instance()->Load(AssetDatabase::Instance()->GetDb(), language, LanguageTarget::Client);
	CCharMsg::InitChannelNames();

	if (strParam.find("editor") != std::string::npos) // Launch game editor
	{
		g_Config.m_bEditor = TRUE;
		//g_Config.m_IsShowConsole = true;
	}
	else {
		g_Config.m_bEditor = FALSE;
		g_Config.m_nCreateScene = 0;
	}

	g_hInstance = hInstance;

	GameLoading loading;
	if (!g_Config.m_bEditor) {
		loading.Create(strParam);
	}

	/*	yangyinyu	2008-10-14	close!
		//  Show loading screen
		GameLoading::Init(strParam)->Create();
	*/
	::_setmaxstdio(2048); //   Increase max open file handles

	//setlocale(LC_CTYPE, GetLanguageString(0)); // Set locale from language resource file  Add by Philip.Wu  2006-07-19
	setlocale(LC_CTYPE, GetLanguageString(0).c_str());
	// Set locale from language resource file  Add by Philip.Wu  2006-07-19

	InstallFont(".\\Font"); // Auto-install fonts  Add by Philip.Wu  2006-08-07

	if (CheckDxVersion(dx_ver) == 0) {
		MessageBox(NULL, GetLanguageString(187).c_str(), "error", MB_OK);
		return 0;
	}

	// Determine which client instance number to use for log files
	HANDLE hFile = CreateFileMapping(INVALID_HANDLE_VALUE, 0, PAGE_READWRITE, 0, sizeof(long), "KopClinetNO");
	if (!hFile) {
		return 0;
	}

	long* pClientLogNO = NULL;
	pClientLogNO = (long*)MapViewOfFile(hFile, FILE_MAP_WRITE, 0, 0, 0);
	g_nCurrentLogNo = *pClientLogNO;
	(*pClientLogNO)++;

	// Delete the auto-update program copy
	char szUpdateFileName[] = "Launcher_d.exe";
	SetFileAttributes(szUpdateFileName, FILE_ATTRIBUTE_NORMAL); //  Remove read-only from updater copy
	DeleteFile(szUpdateFileName);

	// for trim obj file
	short i = 1;
	short j = i << 14;
	if (_tcsstr(_tcslwr(lpCmdLine), _TEXT("-objtrim+backup")) != NULL) {
		g_ObjFile.TrimDirectory("map", true);
		return 0;
	}
	else if (_tcsstr(_tcslwr(lpCmdLine), _TEXT("-objtrim")) != NULL) {
		g_ObjFile.TrimDirectory("map", false);
		return 0;
	}

	MSG msg;
	HACCEL hAccelTable;

	HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_KOP, szWindowClass, MAX_LOADSTRING);

	MyRegisterClass(hInstance, brush); // Register custom window class

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_KOP); // Load accelerator table (hotkeys)

	if (strParam.find("pack") != std::string::npos) {
		char* pszPos = lpCmdLine;
		pszPos += 4;
		while (*pszPos == ' ') {
			pszPos++;
		}
		char pszpath[128];
		char pszFile[128];
		sprintf(pszpath, "texture\\minimap\\%s", pszPos);
		sprintf(pszFile, "texture\\minimap\\%s\\%s.pk", pszPos, pszPos);

		CMiniPack pkfile;
		if (pkfile.SaveToPack(pszpath, pszFile))
			g_logManager.InternalLog(LogLevel::Debug, "common", SafeVFormat(GetLanguageString(188), pszPos));
		else
			g_logManager.InternalLog(LogLevel::Debug, "common", SafeVFormat(GetLanguageString(189), pszPos));
		return 0;
	}

	auto model = strParam.find("model:");
	if (model != std::string::npos) {
		std::string strModel = strParam.substr(model + 6, strParam.length());
		auto nEnd = strModel.length();
		nEnd = strModel.find(" ");
		if (nEnd != std::string::npos) strModel = strModel.substr(0, nEnd);

		std::string strList[2];
		Util_ResolveTextLine(strModel.c_str(), strList, 2, '-');

		useModelMode = true;

		sprintf(modelLook, "%s", strList[0].c_str());
		modelMode = atoi(strList[1].c_str()) == 1;
	}

	if (strParam.find("pKcfT0PcaX") == std::string::npos) {
		//MessageBox(NULL, "Please run the launcher to play the game.", "Error", MB_OK);
		return 0;
	}

	int autoLog = (int)strParam.find("autolog:");
	if (autoLog != -1) {
		std::string strAuto = strParam.substr(autoLog + 8, strParam.length());
		int nEnd = (int)strAuto.length();
		nEnd = (int)strAuto.find(" ");
		if (nEnd != -1) strAuto = strAuto.substr(0, nEnd);

		std::string strList[5];
		Util_ResolveTextLine(strAuto.c_str(), strList, 5, ',');

		useAutoLogin = true;
		useAutoLogin2 = true;
		sprintf(autoLoginName, "%s", strList[0].c_str());
		sprintf(autoLoginPassword, "%s", strList[1].c_str());
		sprintf(autoLoginChar, "%s", strList[2].c_str());
		sprintf(Region, "%s", strList[3].c_str());
		sprintf(Server, "%s", strList[4].c_str());
		if (autoLoginChar[0] != 0) {
			canAutoLoginChar = true;
		}
	}


	InitLuaPlatform();

	MPTimer t;
	t.Begin();
	g_pGameApp = new CGameApp();
	g_lootFilter = new LootFilter();

	if (!g_stUISystem.m_isLoad) {
		g_stUISystem.LoadCustomProp();
	}

	g_Config.m_bFullScreen = g_stUISystem.m_sysProp.m_videoProp.bFullScreen;
	int nWidth(0), nHeight(0), nDepth(0);

	switch (g_stUISystem.m_sysProp.m_videoProp.bResolution) {
	case 0:
		nWidth = TINY_RES_X;
		nHeight = TINY_RES_Y;
		break;
	case 1:
		nWidth = SMALL_RES_X;
		nHeight = SMALL_RES_Y;
		break;
	case 2:
		nWidth = MID_RES_X;
		nHeight = MID_RES_Y;
		break;
	case 3:
		nWidth = LARGE_RES_X;
		nHeight = LARGE_RES_Y;
		break;
	case 4:
		nWidth = EXTRA_LARGE_RES_X;
		nHeight = EXTRA_LARGE_RES_Y;
		break;
	case 5:
		nWidth = FULL_LARGE_RES_X;
		nHeight = FULL_LARGE_RES_Y;
		break;
	default:
		nWidth = TINY_RES_X;
		nHeight = TINY_RES_Y;
	}

	nDepth = g_stUISystem.m_sysProp.m_videoProp.bDepth32 ? 32 : 16;

	MPD3DCreateParamAdjustInfo cpai;
	cpai.multi_sample_type = (D3DMULTISAMPLE_TYPE)g_Config.m_dwFullScreenAntialias;

	g_Render.SetD3DCreateParamAdjustInfo(&cpai);

	// g_Render.EnableClearTarget(FALSE);
	g_Render.SetBackgroundColor(D3DCOLOR_XRGB(10, 10, 125));

	if (g_Config.m_bFullScreen) {
		nWidth = GetSystemMetrics(SM_CXSCREEN);
		nHeight = GetSystemMetrics(SM_CYSCREEN);
	}

	if (useModelMode) {
		if (!modelMode) {
			nWidth = 280;
			nHeight = 450;
		}
		else {
			nWidth = 128;
			nHeight = 128;
		}
	}

	if (!g_pGameApp->Init(hInstance, szWindowClass, nWidth, nHeight, nDepth, FALSE /*g_Config.m_bFullScreen*/)) {
		g_logManager.InternalLog(LogLevel::Debug, "common", GetLanguageString(191));
		g_pGameApp->End();
		return 0;
	}

	if (g_Config.m_bFullScreen) {
		g_pGameApp->ChangeVideoStyle(nWidth, nHeight, D3DFMT_D16,TRUE);
	}


	//DWORD ThreadID;
	//   HANDLE hThread = ::CreateThread(NULL,0,LoadRes,
	//                             0,0,&ThreadID);

	//g_pGameApp->LoadRes2();

	if (!g_Config.m_bEditor) {
		loading.Close();
	}

	ToLogService("common", "init use time = {} ms", t.End());

	// Main message loop:
	ZeroMemory(&msg, sizeof(msg));

	DWORD st_dwtick = GetTickCount();
	DWORD st_tickcount = (GetTickCount() - st_dwtick) / unsigned long(g_NetIF->m_framedelay);

	g_NetIF->m_framedelay = 40; // Frame delay

	//string str( "Micro" );
	//string rstr( "soft" );
	//str.append( rstr, 14, 3 );


	g_pGameApp->Run();

	g_pGameApp->End();
	SAFE_DELETE(g_pGameApp);

	::DeleteObject(brush);

	UnmapViewOfFile(pClientLogNO);
	CloseHandle(hFile);
	if (g_IsAutoUpdate) // If auto-updating, launch the updater
	{
		::WinExec("Launcher.exe", SW_SHOWNORMAL);
	}
	// Auto-update exception handling

	return 0;
}


//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance, HBRUSH brush) {
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = (WNDPROC)WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, (LPCTSTR)IDI_KOP);
	wcex.hCursor = NULL;
	wcex.hbrBackground = (HBRUSH)brush; //(HBRUSH)(COLOR_BACKGROUND+1);
	wcex.lpszMenuName = (LPCTSTR)IDC_KOP;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
HWND InitInstance(HINSTANCE hInstance, int nCmdShow) {
	HWND hWnd;

	hInst = hInstance; // Store instance handle in our global variable

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
						CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!hWnd) {
		return 0;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return hWnd;
}

//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//

extern void __timer_period_render();

static WNDPROC g_wpOrigEditProc = NULL;
// Subclass procedure
LRESULT APIENTRY EditSubclassProc(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam) {
	if (uMsg == WM_GETDLGCODE)
		return DLGC_WANTALLKEYS;

	switch (uMsg) {
	case WM_KEYDOWN:
		if (wParam == VK_UP || wParam == VK_DOWN) {
			WndProc(hwnd, uMsg, wParam, lParam);
			return 0;
		}
	case WM_CHAR:
		WndProc(hwnd, uMsg, wParam, lParam);
		if (GetKeyState(VK_CONTROL) & 0xff00) {
			return 0;
		}
		switch (wParam) {
		case VK_ESCAPE:
		case VK_RETURN:
		case VK_TAB:
			return 0;
		}
		break;
	case WM_SYSKEYDOWN:
		WndProc(hwnd, uMsg, wParam, lParam);
		break;
	case WM_SYSKEYUP:
		if (wParam == VK_MENU || wParam == VK_F10) {
			return 0;
		}
		WndProc(hwnd, uMsg, wParam, lParam);
		break;
	case WM_SYSCOMMAND:
		if (wParam == SC_KEYMENU) {
			return 0;
		}
		break;
		//case WM_INPUTLANGCHANGEREQUEST:
		//case WM_INPUTLANGCHANGE:
		//case WM_IME_NOTIFY:
		//case WM_IME_STARTCOMPOSITION:
		//case WM_IME_ENDCOMPOSITION:
		//    {
		//        WndProc( hwnd, uMsg, wParam, lParam);
		//    }
		//    break;
		//case WM_IME_COMPOSITION:
		//case WM_IME_SETCONTEXT:
		//    {
		//        WndProc( hwnd, uMsg, wParam, lParam);
		//        if(g_Config.m_bFullScreen)
		//        {
		//            return 0;
		//        }
		//    }
		//    break;
	}
	return CallWindowProc(g_wpOrigEditProc, hwnd, uMsg, wParam, lParam);
}


HWND g_InputEdit = NULL;
#include "inputbox.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	//switch (message)
	//{
	//case WM_ACTIVATE:
	//	// Add by lark.li 20080903 begin
	//	if(GameLoading::Init()->Active())
	//		return 0;
	//	// End
	//
	//	break;
	//case WM_NCACTIVATE:
	//case WM_ACTIVATEAPP:
	//	// Add by lark.li 20080903 begin
	//	if(GameLoading::Init()->Active())
	//		return 0;
	//	// End

	//	if( g_pGameApp && g_pGameApp->IsInit() )
	//		g_pGameApp->SetInputActive( LOWORD(wParam)!=WA_INACTIVE );
	//	break;
	//}

	if (g_pGameApp) {
		g_pGameApp->HandleWindowMsg(message, (DWORD)wParam, (DWORD)lParam);
	}

	switch (message) {
	case WM_USER_TIMER:
		__timer_period_render();
		return 0;
		break;

	case WM_ACTIVATE:
		// Add by lark.li 20080903 begin
		//if(GameLoading::Init()->Active())
		//	return 0;
		// End

		break;
	case WM_NCACTIVATE:
	case WM_ACTIVATEAPP:
		// Add by lark.li 20080903 begin
		//if(GameLoading::Init()->Active())
		//	return 0;
		// End

		if (g_pGameApp && g_pGameApp->IsInit())
			g_pGameApp->SetInputActive(LOWORD(wParam) != WA_INACTIVE);
		break;
	case WM_CREATE: {
		CenterWindow(hWnd);

		g_InputEdit = CreateWindow(TEXT("EDIT"),
								   GetLanguageString(192).c_str(),
								   WS_CHILD | WS_VISIBLE,
								   0, 0,
								   0, 0,
								   hWnd,
								   (HMENU)IDC_EDIT1,
								   ((LPCREATESTRUCT) lParam)->hInstance,
								   NULL);

		g_wpOrigEditProc = (WNDPROC)(LONG64)SetWindowLong(g_InputEdit, GWL_WNDPROC, (LONG)(LONG64)EditSubclassProc);

		extern CInputBox g_InputBox;
		g_InputBox.SetEditWindow(g_InputEdit);

		CImeInput::s_Ime.OnCreate(hWnd, hInst);
	}
	break;
	case WM_KEYDOWN:
	case WM_CHAR:
		break;
	case WM_LBUTTONDOWN:
	case WM_MOUSEMOVE:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP: {
		break;
	}
	case WM_COMMAND: {
		int wmId = LOWORD(wParam);
		int wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId) {
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
		break;
	case WM_CLOSE:
		//{
		//	if( g_Config.m_bEditor )
		//		break;

		//	if( !g_NetIF->IsConnected() )
		//		break;

		//	if( !dynamic_cast<CWorldScene*>( CGameApp::GetCurScene() ) )
		//		break;

		//	CForm* f = CFormMgr::s_Mgr.Find( "frmAskExit" );
		//	if( f )
		//	{
		//		long lState = GetWindowLong(hWnd, GWL_STYLE);
		//		if( lState & WS_MINIMIZE )
		//		{
		//			ShowWindow(hWnd, SW_SHOWNORMAL);
		//		}

		//		f->SetIsShow(true);
		//		g_stUISystem.GetSystemForm()->SetIsShow(false);
		//		return 0;
		//	}
		//}
		break;
	case WM_QUIT:
		break;
	case WM_DESTROY: {
		CS_Logout();
		CS_Disconnect(DS_DISCONN);

		SetWindowLong(g_InputEdit, GWL_WNDPROC, (LONG)(LONG64)g_wpOrigEditProc);

		PostQuitMessage(0);
		if (g_pGameApp) g_pGameApp->SetIsRun(false);
		break;
	}
	case WM_MUSICEND: // Music playback system
	{
		if (g_pGameApp) g_pGameApp->PlayMusic(0);
		break;
	}
	case WM_SYSKEYUP:
		if (wParam == VK_MENU || wParam == VK_F10) {
			return 0;
		}
	//case WM_IME_STARTCOMPOSITION:
	//case WM_IME_ENDCOMPOSITION:
	//case WM_IME_NOTIFY:
	//case WM_IME_COMPOSITION:
	//case WM_IME_SETCONTEXT:
	//case WM_INPUTLANGCHANGEREQUEST:
	//case WM_INPUTLANGCHANGE:
	//    {
	//        return 0;
	//    }
	//    break;

	case WM_ERASEBKGND:
		return 0;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

// Display error message to debug output (formerly printf to edit control)
void DisplayError(const char* szMsg) {
	OutputDebugString(szMsg);
	//TRACE(szMsg);
	//TRACE(" ");
}

std::list<CCharacter*> g_Loading;

HANDLE hOutputReadTmp = 0;
HANDLE hOutputWrite = 0;

DWORD WINAPI ReadStdout(LPVOID lpvThreadParam) {
	CHAR lpBuffer[256];
	DWORD nBytesRead;

	//extern CMyIDEApp theApp;

	while (TRUE) {
		if (g_Loading.size() > 0) {
		}
		else {
			Sleep(1);
		}


		//  if (!ReadFile(hOutputReadTmp, lpBuffer,sizeof(lpBuffer),
		//                                   &nBytesRead,NULL) || !nBytesRead)
		//  {
		//     if (GetLastError() == ERROR_BROKEN_PIPE)
		//        break; // pipe done - normal exit path.
		//     else
		//        DisplayError("ReadFile"); // Something bad happened.
		//  }
		//
		//  if (nBytesRead >0 )
		//  {
		//      // Read printf output
		//      lpBuffer[nBytesRead] = 0;
		////if(theApp.m_pMainWnd)
		////{
		   ////CMyIDEDlg *pDlg = (CMyIDEDlg*)theApp.m_pMainWnd; // Write pipe string to Edit control
		   ////pDlg->m_edit1.ReplaceSel(lpBuffer);
		//   //}
		//if( g_pGameApp )
		//{
		// g_pGameApp->SysInfo( "lua printf:%s", lpBuffer );
		//}
		//  }
	}

	return 0;
}

#include  <io.h>
#include <FCNTL.h>

void StdoutRedirect() {
	if (!CreatePipe(&hOutputReadTmp, &hOutputWrite, 0, 0))
		DisplayError("CreatePipe");

	int hCrt;
	FILE* hf;
	//AllocConsole();
	hCrt = _open_osfhandle((long)hOutputWrite, _O_TEXT);
	hf = _fdopen(hCrt, "w");
	*stdout = *hf;
	int i = setvbuf(stdout, NULL, _IONBF, 0);

	// Launch the thread that gets the input and sends it to the child.
	DWORD ThreadID;
	HANDLE hThread = ::CreateThread(NULL, 0, ReadStdout, // Create reader thread
									0, 0, &ThreadID);
}


// Install fonts
int InstallFont(const char* pszPath) {
	int nRet = 0;
	char szWindow[256] = {0};
	char szBuffer[256] = {0};

	// Check if the font is already installed; if not, install automatically
	GetWindowsDirectory(szWindow, sizeof(szWindow) / sizeof(szWindow[0])); // Get Windows directory full path
	sprintf(szBuffer, "%s\\fonts\\simsun.ttc", szWindow);
	if (-1 != access(szBuffer, 0)) {
		return nRet;
	}
	else {
		sprintf(szBuffer, "%s\\simsun.ttc", pszPath);
		nRet += AddFontResource(szBuffer);
	}


	//WIN32_FIND_DATA oFinder;

	//HANDLE hFind = FindFirstFile(szBuffer, &oFinder);

	//if(hFind == INVALID_HANDLE_VALUE)
	//{
	//	return 0;
	//}

	//// Iterate over font directory
	//while(FindNextFile(hFind, &oFinder) != 0)
	//{
	//	if(oFinder.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
	//	{
	//		continue;
	//	}

	//	nRet += AddFontResource(szBuffer);	//AddFontResourceEx(szBuffer, FR_NOT_ENUM, 0);
	//}

	//FindClose(hFind);

	if (nRet) {
		SendMessage(HWND_BROADCAST,WM_FONTCHANGE, 0, 0); // Broadcast font change to the system
	}

	return nRet;
}


void CenterWindow(HWND hWnd) // Center window display position on screen
{
	RECT rc;
	GetWindowRect(hWnd, &rc); // Get window rect to center on screen

	int x = (GetSystemMetrics(SM_CXSCREEN) - (rc.right - rc.left)) / 2; // Calculate left edge
	int y = (GetSystemMetrics(SM_CYSCREEN) - (rc.bottom - rc.top)) / 2; // Calculate top edge
	MoveWindow(hWnd, x, y, rc.right - rc.left, rc.bottom - rc.top, TRUE); // Set window position and size
}
