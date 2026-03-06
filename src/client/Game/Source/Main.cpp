//------------------------------------------------------------------------
//	2005.3.25	Arcol	
//------------------------------------------------------------------------


// MPTest.cpp : Defines the entry point for the application.
//
#include "stdafx.h"

#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
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
#include "d3des.h"
#include "Character.h"
#include "gameloading.h"
#include "lua_platform.h"
#include "enclib.h"

#include "GlobalVar.h"

#include "SelectChaScene.h"

#include "LootFilter.h"


std::string g_serverset;

#define MAX_LOADSTRING 100
 
// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
 
// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance, HBRUSH brush);
HWND				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

HBRUSH	g_bk_brush;

void StdoutRedirect(); 
int InstallFont(const char* pszPath); // 
void CenterWindow(HWND hWnd); //

#pragma init_seg( lib )
CLanguageRecord  g_oLangRec{};	//Add by lark.li 20080330

BOOL CheckDxVersion(DWORD &ver) // 
{
    BOOL ret = 1;
    MPISystemInfo* sys_info = 0;

    MPGUIDCreateObject((LW_VOID**)&sys_info, LW_GUID_SYSTEMINFO);
    if(SUCCEEDED(sys_info->CheckDirectXVersion())) // 
    {
		ver = sys_info->GetDirectXVersion();

        if(ver < DX_VERSION_8_1) //
        {
            ret = 0;
        }

    }
    sys_info->Release();

    return ret;
}

bool	g_IsAutoUpdate = false;				// ?
long	g_nCurrentLogNo = 0;


////////////////////////////////////////
//
//  By Jampe
//  
//  2006/5/22
//
const unsigned char key[] = {0xda, 0x15, 0x1c, 0x10, 0x4f, 0x8c, 0x7a, 0x4a};

//////////////////////////////////////////////////////////////////////

DWORD WINAPI LoadRes(LPVOID lpvThreadParam) // 
{
	g_bLoadRes = 1;

	return 0;
}

HINSTANCE g_hInstance;

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	DWORD dx_ver = DX_VERSION_X_X;

	std::cout << "Loading string..." << CLanguageRecordInstance().GetRecordCount() << std::endl;

T_B

	LG("init", "Define __CATCH\n");
	std::string strParam = lpCmdLine;
	
	if(strParam.find("editor")!= std::string::npos) // 
    {
        g_Config.m_bEditor = TRUE;
		//g_Config.m_IsShowConsole = true;
    }
    else
    {
        g_Config.m_bEditor      = FALSE;
        g_Config.m_nCreateScene = 0;
    }

    g_hInstance = hInstance;

	GameLoading loading;
	if (!g_Config.m_bEditor) {
		loading.Create(strParam);
	}
	
/*	yangyinyu	2008-10-14	close!
	//  loading
	GameLoading::Init(strParam)->Create();
*/
	::_setmaxstdio( 2048 ); //   

	//setlocale(LC_CTYPE, g_oLangRec.GetString(0)); // ?  Add by Philip.Wu  2006-07-19  
	setlocale(LC_CTYPE, g_oLangRec.GetString(0)); // ?  Add by Philip.Wu  2006-07-19  

	InstallFont(".\\Font");	//  Add by Philip.Wu  2006-08-07

	if(CheckDxVersion(dx_ver) == 0) 
    {
        MessageBox(NULL, g_oLangRec.GetString(187), "error", MB_OK);
        return 0;
    }

	// ?log
	HANDLE	hFile = CreateFileMapping(INVALID_HANDLE_VALUE, 0, PAGE_READWRITE, 0, sizeof(long), "KopClinetNO");
	if (!hFile) 
	{
		return 0;
	}

	LGInfo lg_info;
	memset( &lg_info, 0, sizeof(lg_info) );

	if( !g_Config.IsPower() ) // log
	{
		lg_info.bCloseAll = true;
	}

	long*	pClientLogNO = NULL;
	pClientLogNO = (long *)MapViewOfFile(hFile, FILE_MAP_WRITE, 0, 0, 0);
	g_nCurrentLogNo = *pClientLogNO;
	(*pClientLogNO)++;
	if (g_nCurrentLogNo > 0)
	{
		sprintf(lg_info.dir, "%s%d", "log", g_nCurrentLogNo);
	}
	else
	{
		sprintf(lg_info.dir, "%s", "log" );
	}

	lg_info.bEnableAll = g_Config.m_bEnableLG!=0;
	lg_info.bMsgBox = g_Config.m_bEnableLGMsg!=0;
	lg_info.bEraseMode = true;

	// ?
	char szUpdateFileName[] = "Launcher_d.exe";
	SetFileAttributes(szUpdateFileName, FILE_ATTRIBUTE_NORMAL); //  
  	DeleteFile(szUpdateFileName);

	// for trim obj file
	short i = 1;
	short j = i<<14;
	if (_tcsstr(_tcslwr(lpCmdLine), _TEXT("-objtrim+backup")) != NULL)
	{
		g_ObjFile.TrimDirectory("map", true);
		return 0;
	}
	else if (_tcsstr(_tcslwr(lpCmdLine), _TEXT("-objtrim")) != NULL)
	{
		g_ObjFile.TrimDirectory("map", false);
		return 0;
	}
	
	MSG msg;
	HACCEL hAccelTable;

    HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));

    // Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_KOP, szWindowClass, MAX_LOADSTRING);

	MyRegisterClass(hInstance, brush);// ?

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_KOP); // ? ?

	if(strParam.find("pack") != std::string::npos)
	{
		char* pszPos = lpCmdLine;
		pszPos += 4;
		while (*pszPos == ' ')
		{
			pszPos++;
		}
		char pszpath[128];
		char pszFile[128];
		sprintf(pszpath,"texture\\minimap\\%s",pszPos);
		sprintf(pszFile,"texture\\minimap\\%s\\%s.pk",pszPos,pszPos);

		CMiniPack pkfile;
		if(pkfile.SaveToPack(pszpath,pszFile))
			LG("ok", g_oLangRec.GetString(188),pszPos);
		else
			LG("ok", g_oLangRec.GetString(189),pszPos);
		return 0;
	}

    auto model = strParam.find("model:");
	if(model!= std::string::npos){
		std::string strModel = strParam.substr( model + 6, strParam.length() );
		auto nEnd = strModel.length();
		nEnd = strModel.find( " " );
		if( nEnd!= std::string::npos) strModel = strModel.substr( 0, nEnd );
		
		std::string strList[2];
		Util_ResolveTextLine( strModel.c_str(), strList, 2, '-' );
		
		useModelMode = true;
		
		sprintf(modelLook,"%s",strList[0].c_str());
		modelMode = atoi(strList[1].c_str())==1;		
		
	}

#ifndef _DEBUG
    if(strParam.find("pKcfT0PcaX")== std::string::npos)
	{
		//MessageBox(NULL, "Please run the launcher to play the game.", "Error", MB_OK);
		return 0;
	}
#endif

	int autoLog = (int)strParam.find("autolog:");
    if(autoLog!=-1) 
    {
        std::string strAuto = strParam.substr( autoLog + 8, strParam.length() );
        int nEnd = (int)strAuto.length();
        nEnd = (int)strAuto.find( " " );
        if( nEnd!=-1 ) strAuto = strAuto.substr( 0, nEnd );

        std::string strList[5];
        Util_ResolveTextLine( strAuto.c_str(), strList, 5, ',' );

        useAutoLogin = true;
        useAutoLogin2 = true;
        sprintf(autoLoginName,"%s",strList[0].c_str());
        sprintf(autoLoginPassword,"%s",strList[1].c_str());
        sprintf(autoLoginChar,"%s",strList[2].c_str());
        sprintf(Region,"%s",strList[3].c_str());
        sprintf(Server,"%s",strList[4].c_str());
        if (autoLoginChar[0] != 0){
            canAutoLoginChar = true;
        }
    }

//#ifdef KOP_TOM
	auto nTomPos = strParam.find("tom:");
	if(nTomPos!= std::string::npos)
	{
		// Tom,:"tom:IP,,,"
		std::string strTom = strParam.substr( nTomPos + 4, strParam.length() );
		auto nEnd = strTom.length();
		nEnd = strTom.find( " " );
		if( nEnd!= std::string::npos) strTom = strTom.substr( 0, nEnd );
		
		std::string strList[4];
		Util_ResolveTextLine( strTom.c_str(), strList, 4, ',' );

		g_TomServer.szServerIP = strList[0];
		g_TomServer.nPort = atoi( strList[1].c_str() );
		g_TomServer.szUser = strList[2];
		g_TomServer.szPassword = strList[3];
		g_TomServer.bEnable=true;
	}
//#endif


    ////////////////////////////////////////
    //
    //  By Jampe
    //  
    //  2006/5/19
    //
    memset(&g_cooperate, 0, sizeof(g_cooperate));
	auto nCopPos = strParam.find("/login:");
    //  /login:&/&&
    if(std::string::npos != nCopPos)
    {
        SetEncKey(key);
        std::string strCop = strParam.substr(nCopPos + 7, strParam.length());
        std::string tmp[4];
        Util_ResolveTextLine(strCop.c_str(), tmp, 4, '&');

        g_cooperate.code = atol(tmp[0].c_str());
        g_cooperate.serv = tmp[1];
        g_cooperate.uid = tmp[2];
        unsigned char buff[128] = {0};
        Decrypt(buff, 128, (unsigned char*)tmp[3].c_str(), (int)tmp[3].length());
        g_cooperate.pwd = (char*)buff;
    }
    switch(g_cooperate.code) // 
    {
    case COP_OURGAME:
        {
            g_serverset = "scripts/txt/server2.tx";
        }  break;
    case COP_SINA:
        {
            g_serverset = "scripts/txt/server3.tx";
        }  break;
    case COP_CGA:
        {
            g_serverset = "scripts/txt/server4.tx";
        }  break;
    default:
        {
            g_serverset = "scripts/txt/server.tx";
        }  break;
    }

#ifdef _LUA_GAME
    InitLuaPlatform(); 
#endif
    
    MPTimer t;
    t.Begin();
    g_pGameApp = new CGameApp();
	g_pGameApp->LG_Config( lg_info );
	g_lootFilter = new LootFilter();

	if(strParam.find("table_bin") != std::string::npos) //table_bin 
	{
		extern void MakeBinTable();
		MakeBinTable();
		return FALSE;
	}

	if(strParam.find("clu_bin")!= std::string::npos){
		// CLU/LUA obfuscation
		extern lua_State* L;

		luaL_dofile(L,"scripts/lua/CameraConf.lua");
		luaL_dofile(L,"scripts/lua/scene.lua");
		luaL_dofile(L,"scripts/lua/scene/face.lua");
		luaL_dofile(L,"scripts/lua/CharacterConf.lua");
		luaL_dofile(L,"scripts/lua/mission/mission.lua");
		luaL_dofile(L,"scripts/lua/mission/missioninfo.lua");
		luaL_dofile(L,"scripts/lua/font.lua");
		luaL_dofile(L,"scripts/lua/gui.lua");

		luaL_dofile(L,"scripts/lua/scene/loginscene.lua");
		luaL_dofile(L,"scripts/lua/scene/selectchascene.lua");
		luaL_dofile(L,"scripts/lua/scene/createchascene.lua");
		luaL_dofile(L,"scripts/lua/scene/mainscene.lua");
		luaL_dofile(L,"scripts/lua/table/scripts.lua");

		luaL_dofile(L,"scripts/lua/forms/main.lua");
		luaL_dofile(L,"scripts/lua/forms/login.lua");
		luaL_dofile(L,"scripts/lua/forms/loading.lua");
		luaL_dofile(L,"scripts/lua/forms/NPC.lua");
		luaL_dofile(L,"scripts/lua/forms/Player.lua");
		luaL_dofile(L,"scripts/lua/forms/minimap.lua");
		luaL_dofile(L,"scripts/lua/forms/help.lua");
		luaL_dofile(L,"scripts/lua/forms/system.lua");
		luaL_dofile(L,"scripts/lua/forms/equip.lua");
		luaL_dofile(L,"scripts/lua/forms/chat.lua");
		luaL_dofile(L,"scripts/lua/forms/preperty.lua");
		luaL_dofile(L,"scripts/lua/forms/mission.lua");
		luaL_dofile(L,"scripts/lua/forms/coze.lua");
		luaL_dofile(L,"scripts/lua/forms/dialog.lua");
		luaL_dofile(L,"scripts/lua/forms/ship.lua");
		luaL_dofile(L,"scripts/lua/forms/traderoom.lua");
		luaL_dofile(L,"scripts/lua/forms/manage.lua");
		luaL_dofile(L,"scripts/lua/forms/select.lua");
		luaL_dofile(L,"scripts/lua/filter.lua");
		luaL_dofile(L,"scripts/lua/forms/editor.lua");
		return FALSE;
	}

	if (!g_stUISystem.m_isLoad)
	{
		g_stUISystem.LoadCustomProp();
	}

	g_Config.m_bFullScreen = g_stUISystem.m_sysProp.m_videoProp.bFullScreen; 
    int nWidth(0), nHeight(0), nDepth(0);

	switch (g_stUISystem.m_sysProp.m_videoProp.bResolution) {
		case 0:
			nWidth  = TINY_RES_X;
			nHeight = TINY_RES_Y;
			break;
		case 1:
			nWidth  = SMALL_RES_X;
			nHeight = SMALL_RES_Y;
			break;
		case 2:
			nWidth  = MID_RES_X;
			nHeight = MID_RES_Y;
			break;
		case 3:
			nWidth  = LARGE_RES_X;
			nHeight = LARGE_RES_Y;
			break;
		case 4:
			nWidth  = EXTRA_LARGE_RES_X;
			nHeight = EXTRA_LARGE_RES_Y;
			break;
		case 5:
			nWidth  = FULL_LARGE_RES_X;
			nHeight = FULL_LARGE_RES_Y;
			break;
		default:
			nWidth  = TINY_RES_X;
			nHeight = TINY_RES_Y;			
	}
	
    nDepth = g_stUISystem.m_sysProp.m_videoProp.bDepth32 ? 32 : 16;
    
    MPD3DCreateParamAdjustInfo cpai;
    cpai.multi_sample_type = (D3DMULTISAMPLE_TYPE)g_Config.m_dwFullScreenAntialias;

	g_Render.SetD3DCreateParamAdjustInfo(&cpai);

	// g_Render.EnableClearTarget(FALSE);
	g_Render.SetBackgroundColor(D3DCOLOR_XRGB(10, 10, 125));

	if(g_Config.m_bFullScreen)
	{
		nWidth  = GetSystemMetrics(SM_CXSCREEN);
		nHeight = GetSystemMetrics(SM_CYSCREEN);
	}
	
	if(useModelMode){
		if(!modelMode){
			nWidth = 280;
			nHeight = 450;
		}else{
			nWidth = 128;
			nHeight = 128;
		}
	}

	if(!g_pGameApp->Init(hInstance, szWindowClass, nWidth, nHeight, nDepth, FALSE /*g_Config.m_bFullScreen*/))
    {
        LG("init", g_oLangRec.GetString(191));
        g_pGameApp->End();
        return 0;
    }

	if(g_Config.m_bFullScreen)
	{
		g_pGameApp->ChangeVideoStyle( nWidth ,nHeight ,D3DFMT_D16 ,TRUE );
	}

	
	//DWORD ThreadID;
 //   HANDLE hThread = ::CreateThread(NULL,0,LoadRes,
 //                             0,0,&ThreadID);

	//g_pGameApp->LoadRes2();

	if(!g_Config.m_bEditor) {
		loading.Close();
	}

	LG("init", "init use time = %d ms\n", t.End());

	// Main message loop:
	ZeroMemory( &msg, sizeof(msg) );
	
	DWORD st_dwtick	   = GetTickCount();
	DWORD st_tickcount = (GetTickCount()-st_dwtick)/unsigned long(g_NetIF->m_framedelay);

	g_NetIF->m_framedelay = 40; // 

    //string str( "Micro" );
    //string rstr( "soft" );
    //str.append( rstr, 14, 3 );
	
	
	g_pGameApp->Run();

	g_pGameApp->End();
    SAFE_DELETE(g_pGameApp);

    ::DeleteObject(brush);

	UnmapViewOfFile(pClientLogNO);
	CloseHandle(hFile);

	if( g_IsAutoUpdate )  	// ?  kop.exe
	{
		::WinExec( "Launcher.exe", SW_SHOWNORMAL );
	}
	//  ?
	}
	catch(std::exception& e) 
	{ 
		std::string	strfile;
		LG_GetDir(strfile);
		strfile += "\\exception.txt";

		FILE * fp = fopen(strfile.c_str(),"a+");

		if(fp) 
		{ 
			SYSTEMTIME st;
			char tim[100] = {0};
			GetLocalTime(&st);
			sprintf(tim, "%02d-%02d %02d:%02d:%02d\r\n", st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

			fwrite(tim,strlen(tim),1,fp);

			try
			{
				// RtlGetVersion returns the true Windows version even on Windows 8.1+,
				// unlike deprecated GetVersionEx which lies without an app manifest.
				typedef LONG (WINAPI* RtlGetVersionPtr)(OSVERSIONINFOW*);
				HMODULE hNtdll = GetModuleHandleW(L"ntdll.dll");

				std::string mOSStringSimple = "Unknown Windows Ver";

				if (hNtdll)
				{
					auto pfnRtlGetVersion = reinterpret_cast<RtlGetVersionPtr>(
						GetProcAddress(hNtdll, "RtlGetVersion"));

					if (pfnRtlGetVersion)
					{
						OSVERSIONINFOEXW osvi = {};
						osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEXW);
						pfnRtlGetVersion(reinterpret_cast<OSVERSIONINFOW*>(&osvi));

						if (osvi.dwMajorVersion <= 4)
							mOSStringSimple = "Microsoft Windows NT";
						else if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0)
							mOSStringSimple = "Microsoft Windows 2000";
						else if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1)
							mOSStringSimple = "Microsoft Windows XP";
						else if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2)
							mOSStringSimple = (osvi.wProductType == VER_NT_WORKSTATION)
								? "Microsoft Windows XP x64 Edition"
								: "Microsoft Windows Server 2003";
						else if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 0)
							mOSStringSimple = (osvi.wProductType == VER_NT_WORKSTATION)
								? "Microsoft Windows Vista"
								: "Microsoft Windows Server 2008";
						else if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 1)
							mOSStringSimple = (osvi.wProductType == VER_NT_WORKSTATION)
								? "Microsoft Windows 7"
								: "Microsoft Windows Server 2008 R2";
						else if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 2)
							mOSStringSimple = (osvi.wProductType == VER_NT_WORKSTATION)
								? "Microsoft Windows 8"
								: "Microsoft Windows Server 2012";
						else if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 3)
							mOSStringSimple = (osvi.wProductType == VER_NT_WORKSTATION)
								? "Microsoft Windows 8.1"
								: "Microsoft Windows Server 2012 R2";
						else if (osvi.dwMajorVersion == 10 && osvi.dwMinorVersion == 0)
							mOSStringSimple = (osvi.dwBuildNumber >= 22000)
								? "Microsoft Windows 11"
								: "Microsoft Windows 10";
						else
							mOSStringSimple = "Microsoft Windows (Unknown)";

						char buildStr[32];
						sprintf(buildStr, " (Build %lu)", osvi.dwBuildNumber);
						mOSStringSimple += buildStr;
					}
				}

				fprintf(fp, "%s\r\n", mOSStringSimple.c_str());

				fprintf(fp, "DirectX Ver %X\r\n", dx_ver);
			
				D3DCAPSX caps;
				g_Render.GetDevice()->GetDeviceCaps(&caps);

				fprintf(fp, "DeviceType %X\r\n", caps.DeviceType);
				fprintf(fp, "AdapterOrdinal %X\r\n", caps.AdapterOrdinal);
				fprintf(fp, "Caps %X\r\n", caps.Caps);
				fprintf(fp, "Caps2 %X\r\n", caps.Caps2);
				fprintf(fp, "Caps3 %X\r\n", caps.Caps3);
				fprintf(fp, "PresentationIntervals %X\r\n", caps.PresentationIntervals);

				fprintf(fp, "CursorCaps %X\r\n", caps.CursorCaps);
				fprintf(fp, "DevCaps %X\r\n", caps.DevCaps);

				fprintf(fp, "PrimitiveMiscCaps %X\r\n", caps.PrimitiveMiscCaps);
				fprintf(fp, "RasterCaps %X\r\n", caps.RasterCaps);
				fprintf(fp, "ZCmpCaps %X\r\n", caps.ZCmpCaps);
				fprintf(fp, "SrcBlendCaps %X\r\n", caps.SrcBlendCaps);
				fprintf(fp, "DestBlendCaps %X\r\n", caps.DestBlendCaps);
				fprintf(fp, "AlphaCmpCaps %X\r\n", caps.AlphaCmpCaps);
				fprintf(fp, "ShadeCaps %X\r\n", caps.ShadeCaps);
				fprintf(fp, "TextureCaps %X\r\n", caps.TextureCaps);
				fprintf(fp, "TextureFilterCaps %X\r\n", caps.TextureFilterCaps);

				fprintf(fp, "CubeTextureFilterCaps %X\r\n", caps.CubeTextureFilterCaps);
				fprintf(fp, "VolumeTextureFilterCaps %X\r\n", caps.VolumeTextureFilterCaps);
				fprintf(fp, "TextureAddressCaps %X\r\n", caps.TextureAddressCaps);
				fprintf(fp, "VolumeTextureAddressCaps %X\r\n", caps.VolumeTextureAddressCaps);

				fprintf(fp, "LineCaps %X\r\n", caps.LineCaps);
				fprintf(fp, "MaxTextureWidth %X\r\n", caps.MaxTextureWidth);
				fprintf(fp, "MaxTextureHeight %X\r\n", caps.MaxTextureHeight);

				fprintf(fp, "MaxVolumeExtent %X\r\n", caps.MaxVolumeExtent);
				fprintf(fp, "MaxTextureRepeat %X\r\n", caps.MaxTextureRepeat);
				fprintf(fp, "MaxTextureAspectRatio %X\r\n", caps.MaxTextureAspectRatio);
				fprintf(fp, "MaxAnisotropy %X\r\n", caps.MaxAnisotropy);
				fprintf(fp, "MaxVertexW %f\r\n", caps.MaxVertexW);
				fprintf(fp, "GuardBandLeft %f\r\n", caps.GuardBandLeft);
				fprintf(fp, "GuardBandTop %f\r\n", caps.GuardBandTop);
				fprintf(fp, "GuardBandRight %f\r\n", caps.GuardBandRight);
				fprintf(fp, "GuardBandBottom %f\r\n", caps.GuardBandBottom);
				fprintf(fp, "ExtentsAdjust %f\r\n", caps.ExtentsAdjust);
				fprintf(fp, "StencilCaps %X\r\n", caps.StencilCaps);
				fprintf(fp, "FVFCaps %X\r\n", caps.FVFCaps);
				fprintf(fp, "TextureOpCaps %X\r\n", caps.TextureOpCaps);
				fprintf(fp, "MaxTextureBlendStages %X\r\n", caps.MaxTextureBlendStages);
				fprintf(fp, "MaxSimultaneousTextures %X\r\n", caps.MaxSimultaneousTextures);
				fprintf(fp, "VertexProcessingCaps %X\r\n", caps.VertexProcessingCaps);
				fprintf(fp, "MaxActiveLights %X\r\n", caps.MaxActiveLights);
				fprintf(fp, "MaxUserClipPlanes %X\r\n", caps.MaxUserClipPlanes);
				fprintf(fp, "MaxVertexBlendMatrices %X\r\n", caps.MaxVertexBlendMatrices);
				fprintf(fp, "MaxVertexBlendMatrixIndex %X\r\n", caps.MaxVertexBlendMatrixIndex);
				fprintf(fp, "MaxPointSize %f\r\n", caps.MaxPointSize);
				fprintf(fp, "MaxPrimitiveCount %X\r\n", caps.MaxPrimitiveCount);
				fprintf(fp, "MaxVertexIndex %X\r\n", caps.MaxVertexIndex);
				fprintf(fp, "MaxStreams %X\r\n", caps.MaxStreams);
				fprintf(fp, "MaxStreamStride %X\r\n", caps.MaxStreamStride);
				fprintf(fp, "VertexShaderVersion %X\r\n", caps.VertexShaderVersion);
				fprintf(fp, "MaxVertexShaderConst %X\r\n", caps.MaxVertexShaderConst);
				fprintf(fp, "PixelShaderVersion %X\r\n", caps.PixelShaderVersion);
				fprintf(fp, "MaxPixelShaderValue %f\r\n", caps.PixelShader1xMaxValue);
			}
			catch(...)
			{
				fwrite("GetDeviceCaps error\r\n",strlen("GetDeviceCaps error\r\n") - 1,1,fp); 
			}

			fwrite(e.what(),strlen(e.what()) - 1,1,fp); 
			fclose(fp); 
		} 
		WinExec("system/errorreport.exe", SW_SHOW);
	}
	catch( ... )
	{
		MessageBox( NULL, g_oLangRec.GetString(186), g_oLangRec.GetString(185), 0 );
	}

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
ATOM MyRegisterClass(HINSTANCE hInstance, HBRUSH brush) 
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_KOP);
	wcex.hCursor		= NULL;
	wcex.hbrBackground	= (HBRUSH)brush;//(HBRUSH)(COLOR_BACKGROUND+1);
	wcex.lpszMenuName	= (LPCTSTR)IDC_KOP;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

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
HWND InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
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
    LPARAM lParam) 
{
    if (uMsg == WM_GETDLGCODE) 
        return DLGC_WANTALLKEYS; 
 
	switch( uMsg )
	{
	case WM_KEYDOWN:
		if ( wParam==VK_UP || wParam==VK_DOWN )
		{
			WndProc( hwnd, uMsg, wParam, lParam);
			return 0;
		}
	case WM_CHAR:
		WndProc( hwnd, uMsg, wParam, lParam);
		if (GetKeyState(VK_CONTROL) & 0xff00)
		{
			return 0;
		}
		switch (wParam)
		{
		case VK_ESCAPE:
		case VK_RETURN:
		case VK_TAB:
			return 0;
		}
		break;
	case WM_SYSKEYDOWN:
		WndProc( hwnd, uMsg, wParam, lParam);
		break;
	case WM_SYSKEYUP:
		if ( wParam==VK_MENU || wParam==VK_F10 )
		{
			return 0;
		}
		WndProc( hwnd, uMsg, wParam, lParam);
		break;
	case WM_SYSCOMMAND:
		if (wParam == SC_KEYMENU)		{
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
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
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

	if( g_pGameApp ) 
	{
		g_pGameApp->HandleWindowMsg(message, (DWORD)wParam, (DWORD)lParam);
	}

	switch (message) 
	{
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

		if( g_pGameApp && g_pGameApp->IsInit() )
			g_pGameApp->SetInputActive( LOWORD(wParam)!=WA_INACTIVE );
		break;
	case WM_CREATE:
		{
			CenterWindow(hWnd);

			g_InputEdit = CreateWindow(TEXT("EDIT"),
			TEXT(g_oLangRec.GetString(192)),
			WS_CHILD | WS_VISIBLE,
			0,0,
			0,0,
			hWnd,
			(HMENU)IDC_EDIT1,
			((LPCREATESTRUCT) lParam)->hInstance,
			NULL);

            g_wpOrigEditProc = (WNDPROC)(LONG64)SetWindowLong(g_InputEdit, GWL_WNDPROC,(LONG)(LONG64)EditSubclassProc ); 

			extern CInputBox g_InputBox;
			g_InputBox.SetEditWindow(g_InputEdit);

			CImeInput::s_Ime.OnCreate( hWnd, hInst );
		}
		break;
	case WM_KEYDOWN: 
	case WM_CHAR:
		break;
	case WM_LBUTTONDOWN:
	case WM_MOUSEMOVE:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:  
		{
			break;
		}
	case WM_COMMAND:
		{
			int wmId    = LOWORD(wParam); 
			int wmEvent = HIWORD(wParam); 
			// Parse the menu selections: 
			switch (wmId)
			{
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
	case WM_DESTROY: 
		{
			CS_Logout();
			CS_Disconnect( DS_DISCONN );

            SetWindowLong(g_InputEdit, GWL_WNDPROC, (LONG)(LONG64)g_wpOrigEditProc); 

			PostQuitMessage(0);
			if( g_pGameApp ) g_pGameApp->SetIsRun( false );
			break; 
		}
	case WM_MUSICEND: // 
		{
			if( g_pGameApp ) g_pGameApp->PlayMusic(0);
			break;
		}
	case WM_SYSKEYUP:
		if (wParam==VK_MENU || wParam==VK_F10)
		{
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

void MakeBinTable()  
{
	g_bBinaryTable = FALSE; 		
	LGInfo lg_info;
	lg_info = *g_pGameApp->GetLGConfig();
	lg_info.bMsgBox = true;
	lg_info.bEnableAll  = true;
	g_pGameApp->LG_Config( lg_info );

	g_pGameApp->InitAllTable();

	MPResourceSet* pResourceSet = new MPResourceSet(0, g_Config.m_nMaxResourceNum);
	pResourceSet->LoadRawDataInfo("scripts/table/ResourceInfo", g_bBinaryTable);

//	MessageBox( NULL, g_oLangRec.GetString(193), "Info", 0 );
}

// printfedit
void DisplayError(const char* szMsg)
{
	OutputDebugString( szMsg );
    //TRACE(szMsg);
    //TRACE(" ");
}

std::list<CCharacter*> g_Loading;

HANDLE hOutputReadTmp = 0;
HANDLE hOutputWrite = 0;
DWORD WINAPI ReadStdout(LPVOID lpvThreadParam)
{
  CHAR lpBuffer[256];
  DWORD nBytesRead;

  //extern CMyIDEApp theApp;

  while(TRUE)
  {
	  if( g_Loading.size() > 0 )
	  {
		
	  }
	  else
	  {
		  Sleep( 1 );
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
   //      //printf?
   //      lpBuffer[nBytesRead] = 0;
		 ////if(theApp.m_pMainWnd)
		 ////{
			////CMyIDEDlg *pDlg = (CMyIDEDlg*)theApp.m_pMainWnd; //Edit
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
void StdoutRedirect()
{
    if (!CreatePipe(&hOutputReadTmp,&hOutputWrite,0,0))
         DisplayError("CreatePipe");

   int hCrt;
   FILE *hf;
   //AllocConsole();
   hCrt = _open_osfhandle((long)hOutputWrite, _O_TEXT);
   hf = _fdopen( hCrt, "w" );
   *stdout = *hf;
   int i = setvbuf( stdout, NULL, _IONBF, 0 );

      // Launch the thread that gets the input and sends it to the child.
    DWORD ThreadID;
    HANDLE hThread = ::CreateThread(NULL,0,ReadStdout, //
                              0,0,&ThreadID);
}


// 
int InstallFont(const char* pszPath)
{
	int nRet = 0;
	char szWindow[256] = {0};
	char szBuffer[256] = {0};

	// ?
	GetWindowsDirectory(szWindow, sizeof(szWindow) / sizeof(szWindow[0]));// Windows
	sprintf(szBuffer, "%s\\fonts\\simsun.ttc", szWindow); 
	if(-1 !=_access(szBuffer, 0))
	{
		return nRet;
	}
	else
	{
		sprintf(szBuffer, "%s\\simsun.ttc", pszPath);
		nRet += AddFontResource(szBuffer);
	}


	//WIN32_FIND_DATA oFinder;

	//sprintf(szBuffer, "%s%s", pszPath, "*.*");
	//HANDLE hFind = FindFirstFile(szBuffer, &oFinder);

	//if(hFind == INVALID_HANDLE_VALUE)
	//{
	//	return 0;
	//}

	//// ?
	//while(FindNextFile(hFind, &oFinder) != 0) 
	//{
	//	if(oFinder.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
	//	{
	//		continue;
	//	}

	//	sprintf(szBuffer, "%s%s", pszPath, oFinder.cFileName);
	//	nRet += AddFontResource(szBuffer);	//AddFontResourceEx(szBuffer, FR_NOT_ENUM, 0);
	//}

	//FindClose(hFind);

	if(nRet)
	{
		SendMessage(HWND_BROADCAST,WM_FONTCHANGE,0,0);  // 
	}

	return nRet;
}


void CenterWindow(HWND hWnd)  // 
{
	RECT rc;
	GetWindowRect(hWnd, &rc); // 

	int x = (GetSystemMetrics(SM_CXSCREEN) - (rc.right - rc.left)) / 2; // ?
	int y = (GetSystemMetrics(SM_CYSCREEN) - (rc.bottom - rc.top)) / 2; // 
	MoveWindow(hWnd, x, y, rc.right - rc.left, rc.bottom - rc.top, TRUE); // 
}
