#include "stdafx.h"
#include "GameConfig.h"

#ifdef _LUA_GAME


#include "resource.h"
#include "lua_platform.h"

using namespace std;

lua_State *L     = NULL;
HWND g_dlgScript = NULL;
list<string> g_luaFNList;

void print_error(lua_State* state) {
    const char* message = lua_tostring(state, -1);
    ToLogService("error", "{}", message);
    lua_pop(state, 1);
}

int lua_sysAddHelpString(lua_State *L)
{
    const char *pszHelp = lua_tostring(L, 1);
    g_luaFNList.push_back(pszHelp);
    return 0;
}

void InitLuaPlatform()
{
    ToLogService("init", "{}", g_oLangRec.GetString(182));
    
    L = lua_open ();
    luaL_openlibs(L);
    
    #define REGFN(fn) (lua_pushstring(L, "" #fn ""), \
        lua_pushcfunction(L, lua_##fn), \
	 lua_settable(L, LUA_GLOBALSINDEX), \
     g_luaFNList.push_back("" #fn ""))

    lua_register(L, "sysAddHelpString", lua_sysAddHelpString);
    
    // util
    g_luaFNList.push_back("[util]");
    REGFN(MsgBox);
    REGFN(GetTickCount);
    REGFN(Rand);
    REGFN(LG);    
    REGFN(SysInfo);
    g_luaFNList.push_back("\r");

    // app
    g_luaFNList.push_back("[app]");
    REGFN(appGetCurScene);
    REGFN(appSetCaption);
    REGFN(appPlaySound);
    REGFN(appUpdateRender);
    //lua_dofile(L, "scripts/gamesdk/app.lua");
    g_luaFNList.push_back("\r");
    
    // scene
    g_luaFNList.push_back("[scene]");
    REGFN(sceAddObj);
    REGFN(sceRemoveObj);
    REGFN(sceGetObj);
    REGFN(sceSetMainCha);
    REGFN(sceGetMainCha);
    REGFN(sceGetHoverCha);
    REGFN(sceEnableDefaultMouse);
    //lua_dofile(L, "scripts/gamesdk/scene.lua");
    g_luaFNList.push_back("\r");
    
    // object
    g_luaFNList.push_back("[object]");
    REGFN(objSetPos);
    REGFN(objGetPos);
    REGFN(objSetFaceAngle);
    REGFN(objGetFaceAngle);
    REGFN(objSetAttr);
    REGFN(objGetAttr);
    REGFN(objIsValid);
    REGFN(objGetID);
    REGFN(chaMoveTo);
    REGFN(chaSay);
    REGFN(chaChangePart);
    REGFN(chaPlayPose);
    REGFN(chaStop);
    //lua_dofile(L, "scripts/gamesdk/object.lua");
    g_luaFNList.push_back("\r");

    // 
    g_luaFNList.push_back("[camera]");
    REGFN(camGetCenter);
    REGFN(camSetCenter);
    REGFN(camFollow);
    REGFN(camMoveForward);
    REGFN(camMoveLeft);
    REGFN(camMoveUp);
    REGFN(camSetAngle);
    g_luaFNList.push_back("\r");

    // Input
    g_luaFNList.push_back("[input]");
    REGFN(IsKeyDown);
    //lua_dofile(L, "scripts/gamesdk/input.lua");
    g_luaFNList.push_back("\r");

    // UI
    g_luaFNList.push_back("[UI]");
    REGFN(uiHideAll);
    // lua_dofile(L, "scripts/gamesdk/input.lua");
    g_luaFNList.push_back("\r");
}

BOOL CALLBACK ScriptDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_INITDIALOG:
        {
             HWND hEdit1  = GetDlgItem(hwndDlg, IDC_EDIT1);
			 //extern CInputBox g_InputBox;
			 //g_InputBox.SetEditWindow(hEdit1);
			
			
			HWND hEdit = GetDlgItem(hwndDlg, IDC_EDIT2);
            for(auto it = g_luaFNList.begin(); it!=g_luaFNList.end(); it++)
            {
                SendMessage(hEdit, EM_REPLACESEL, 0, (LPARAM)(*it).c_str()); 
                SendMessage(hEdit, EM_REPLACESEL, 0, (LPARAM)"\r\n");
            }
            ShowWindow(hEdit, SW_HIDE);
            break;
        }
        case WM_KEYDOWN:
        {
            break;
        }
        case WM_COMMAND:
        {
            HWND hEdit1   = GetDlgItem(hwndDlg, IDC_EDIT1);
            HWND hEdit2   = GetDlgItem(hwndDlg, IDC_EDIT2);
            HWND hButHelp = GetDlgItem(hwndDlg, IDFNLIST); 
                    
            switch(wParam)
            {
                case IDOK:
                {
                    FILE *fp = fopen("tmp.txt", "wt");
                    if(fp==NULL) break;
                    char szText[8192];
                    int n = GetWindowText(hEdit1, szText, 8192); 
                    fwrite(szText, n, 1, fp); 
                    fclose(fp);
                    luaL_dofile(L, "tmp.txt");
                    break;
                }
                case IDFNLIST:
                {
                    if(SendMessage(hButHelp, BM_GETCHECK, 0,0)==BST_CHECKED)
                    {
                        ShowWindow(hEdit1, SW_HIDE);
                        ShowWindow(hEdit2, SW_SHOW);
                    }
                    else
                    {
                        ShowWindow(hEdit2, SW_HIDE);
                        ShowWindow(hEdit1, SW_SHOW);
                    }
                    break;
                }
                case IDCLEAR:
                { 
                    SetWindowText(hEdit1, "");
                    break;
                }
                case IDCANCEL:
                {
                    ShowWindow(hwndDlg, SW_HIDE);
                }
            }
            break;
        }
    }
    return FALSE;
}


void CreateScriptDebugWindow(HINSTANCE hInst, HWND hParent)
{
    g_dlgScript = CreateDialog(hInst, MAKEINTRESOURCE(IDD_DLG_SCRIPT), hParent, ScriptDialogProc);
    if(g_Config.m_bEditor)
    {
        ShowWindow(g_dlgScript, SW_SHOW);
    }
    else
    {
        ShowWindow(g_dlgScript, SW_HIDE);
    }
    RECT rc;
    GetWindowRect(hParent, &rc);
    SetWindowPos(g_dlgScript, NULL, rc.right - 300, rc.bottom - 300, 0, 0, SWP_NOSIZE);
}

void ToggleScriptDebugWindow()
{
    if(IsWindowVisible(g_dlgScript))
    {
        ShowWindow(g_dlgScript, SW_HIDE);
    }
    else
    {
        ShowWindow(g_dlgScript, SW_SHOW);
    }
}

#endif
