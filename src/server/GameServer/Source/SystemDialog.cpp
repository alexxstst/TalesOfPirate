
#include "Stdafx.h"
#include "GameApp.h"
#include "resource.h"
#include "SystemDialog.h"
#include "lua_gamectrl.h"
#include "CharScript.h"
#include "GameDB.h"
#include "Config.h"

#ifdef WIN32_DLG

HWND g_SysDlg;
HWND g_ReportView;
HWND g_MapView;
long  g_lViewAtMapX  = 0;
long  g_lViewAtMapY  = 0;
long  g_lMouseAtMapX = 0;
long  g_lMouseAtMapY = 0;
BOOL  g_bShowView    = 0;

BOOL IsMouseLocation(int x, int y)
{
	int xoff = abs(x - g_lMouseAtMapX);
	int yoff = abs(y - g_lMouseAtMapY);

	if(xoff < 4 && yoff < 4)
	{
		return TRUE;
	}

	return FALSE;
}

int  UNIT_SCALE =  1;
#define UNIT_DRAW_SIZE (UNIT_SCALE * 8)


BOOL CALLBACK DlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_INITDIALOG:
        {
			HWND hCheck = GetDlgItem(hwndDlg, IDC_CHECK_CHA_LOG);
			SendMessage(hCheck, BM_SETCHECK, BST_UNCHECKED, 0);
			g_pGameApp->SetEntityEnableLog(false);


			// Add by lark.li 20080330 begin
			::SetDlgItemText(hwndDlg, IDD_DLG_MAIN, RES_STRING(GM_SYSTEMDIALOG_CPP_00002));
			::SetDlgItemText(hwndDlg, IDC_CHECK_CHA_LOG, RES_STRING(GM_SYSTEMDIALOG_CPP_00003));
			::SetDlgItemText(hwndDlg, IDC_CHECK_VIEW, RES_STRING(GM_SYSTEMDIALOG_CPP_00004));
			// End

			break;
        }
        case WM_KEYDOWN:
        {
            break;
        }
        case WM_COMMAND:
		{
			if(wParam==IDC_BUT_ZOOM1)
			{
				UNIT_SCALE*=2;
				if(UNIT_SCALE>=8) UNIT_SCALE = 8;
			}
			else if(wParam==IDC_BUT_ZOOM2)
			{
				UNIT_SCALE/=2;
				if(UNIT_SCALE<=1) UNIT_SCALE = 1;
			}
			else if(wParam==IDC_VIEWLOG)
			{
				WinExec("logvwr.exe", SW_SHOW);
				break;
			}
			else if(wParam==IDC_CHECK_VIEW)
			{
				HWND hCheck = GetDlgItem(hwndDlg, IDC_CHECK_VIEW);
				if(BST_CHECKED==SendMessage(hCheck, BM_GETCHECK, 0, 0))
				{
					g_bShowView = TRUE;	
				}
				else
				{
					g_bShowView = FALSE;	
				}
			}
			else if (wParam==IDC_CHECK_CHA_LOG)
			{
				HWND hCheck = GetDlgItem(hwndDlg, IDC_CHECK_CHA_LOG);
				if(BST_CHECKED==SendMessage(hCheck, BM_GETCHECK, 0, 0))
					g_pGameApp->SetEntityEnableLog(true);
				else
					g_pGameApp->SetEntityEnableLog(false);
			}
			break;
		}
		case WM_USER_LOG: // SendMessagelog
		{
			// g_pGameApp->AddLog((const char*)lParam);
			break;	
		}
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			break;
		}
	}
	return FALSE;
}

BOOL CALLBACK ReportDlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_INITDIALOG:
        {
			char szRes[255];
			HWND hPID = GetDlgItem(hwndDlg, IDC_PID);
			sprintf(szRes, "%d", GetCurrentProcessId());
			SetWindowText(hPID, szRes);

			HWND hConfigDir = GetDlgItem(hwndDlg, IDC_RES_DIR);
			sprintf(szRes, "[%s]", szConfigFileN);
			SetWindowText(hConfigDir, szRes);

			HWND hResDir = GetDlgItem(hwndDlg, IDC_RES_DIR3);
			sprintf(szRes, "[%s]", g_Config.m_szResDir);
			SetWindowText(hResDir, szRes);

			HWND hLogDir = GetDlgItem(hwndDlg, IDC_RES_DIR2);
			sprintf(szRes, "[%s]", g_Config.m_szLogDir);
			SetWindowText(hLogDir, szRes);

			// Add by lark.li 20080330 Begin
			::SetDlgItemText(hwndDlg, IDC_STATIC_GROUP1, RES_STRING(GM_SYSTEMDIALOG_CPP_00005));
			::SetDlgItemText(hwndDlg, IDC_STATIC_LBL1, RES_STRING(GM_SYSTEMDIALOG_CPP_00006));
			::SetDlgItemText(hwndDlg, IDC_STATIC_LBL2, RES_STRING(GM_SYSTEMDIALOG_CPP_00007));
			::SetDlgItemText(hwndDlg, IDC_STATIC_LBL3, RES_STRING(GM_SYSTEMDIALOG_CPP_00008));
			::SetDlgItemText(hwndDlg, IDC_STATIC_LBL4, RES_STRING(GM_SYSTEMDIALOG_CPP_00009));
			::SetDlgItemText(hwndDlg, IDC_STATIC_LBL5, RES_STRING(GM_SYSTEMDIALOG_CPP_00010));
			::SetDlgItemText(hwndDlg, IDC_STATIC_LBL6, RES_STRING(GM_SYSTEMDIALOG_CPP_00011));
			::SetDlgItemText(hwndDlg, IDC_STATIC_LBL7, RES_STRING(GM_SYSTEMDIALOG_CPP_00012));
			::SetDlgItemText(hwndDlg, IDC_STATIC_LBL8, RES_STRING(GM_SYSTEMDIALOG_CPP_00013));
			::SetDlgItemText(hwndDlg, IDC_STATIC_LBL9, RES_STRING(GM_SYSTEMDIALOG_CPP_00014));
			::SetDlgItemText(hwndDlg, IDC_STATIC_LBL10, RES_STRING(GM_SYSTEMDIALOG_CPP_00015));
			::SetDlgItemText(hwndDlg, IDC_STATIC_LBL11, RES_STRING(GM_SYSTEMDIALOG_CPP_00016));
			::SetDlgItemText(hwndDlg, IDC_STATIC_LBL12, RES_STRING(GM_SYSTEMDIALOG_CPP_00017));
			::SetDlgItemText(hwndDlg, IDC_STATIC_LBL13, RES_STRING(GM_SYSTEMDIALOG_CPP_00019));
			::SetDlgItemText(hwndDlg, IDC_STATIC_GROUP2, RES_STRING(GM_SYSTEMDIALOG_CPP_00018));
			::SetDlgItemText(hwndDlg, IDC_CLEAR, RES_STRING(GM_SYSTEMDIALOG_CPP_00021));
			::SetDlgItemText(hwndDlg, IDC_EXEC, RES_STRING(GM_SYSTEMDIALOG_CPP_00020));

			// End

			break;
        }
        case WM_KEYDOWN:
        {
            break;
        }
        case WM_COMMAND:
        {
            HWND hEdit1 = GetDlgItem(hwndDlg, IDC_EDIT1);
                    
            switch(wParam)
            {
                case IDC_EXEC:
                {
                    FILE *fp = fopen("tmp.txt", "wt");
                    if(fp==NULL) break;
                    char szText[8192];
                    int n = GetWindowText(hEdit1, szText, 8192); 
                    fwrite(szText, n, 1, fp); 
                    fclose(fp);
					g_pGameApp->m_bExecLuaCmd = TRUE;
                    break;
                }
				case IDC_CLEAR:
				{
					SetWindowText(hEdit1, "");
					break;
				}
			}
			break;
        }
		
    }
    return FALSE;
}


void DrawMgrUnit(HDC dc, int map_sx, int map_sy, int view_w, int view_h)
{
	int cnt_x = view_w / UNIT_DRAW_SIZE + 1;
	int cnt_y = view_h / UNIT_DRAW_SIZE + 1;

	int cx = map_sx * UNIT_SCALE;
	int cy = map_sy * UNIT_SCALE;
	
	int draw_sx = (cx / UNIT_DRAW_SIZE) * UNIT_DRAW_SIZE - map_sx * UNIT_SCALE - 1;
	int draw_sy = (cy / UNIT_DRAW_SIZE) * UNIT_DRAW_SIZE - map_sy * UNIT_SCALE - 1;

	for(int x = 0; x < cnt_x; x++)
	{
		POINT p;
		MoveToEx(dc, draw_sx + x * UNIT_DRAW_SIZE, draw_sy, &p);
		LineTo(dc,   draw_sx + x * UNIT_DRAW_SIZE, draw_sy + view_h + UNIT_DRAW_SIZE);
	}
	
	for(int y = 0; y < cnt_y; y++)
	{
		POINT p;
		MoveToEx(dc, draw_sx,                           draw_sy + y * UNIT_DRAW_SIZE, &p);
		LineTo(dc,   draw_sx + view_w + UNIT_DRAW_SIZE, draw_sy + y * UNIT_DRAW_SIZE);
	}
}

void DrawMapUnit(SubMap *pMap, HDC dc, int sx, int sy, int w, int h)
{
	if(pMap==NULL) return;


	SetWindowText(g_MapView, pMap->GetName());
	HBRUSH brMonster = CreateSolidBrush(RGB(140, 140, 140));
	SelectObject(dc, brMonster);
	SetBkMode(dc, TRANSPARENT);
	SetTextColor(dc, RGB(255, 255, 255));

	/*
	HFONT font;// = GetCurrentObject(dc, OBJ_FONT);
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	//lf.lfWeight    = 9;
	lf.lfWidth     = 14;
	lf.lfHeight    = 14;
	lf.lfUnderline = FALSE;           //
	lf.lfStrikeOut = FALSE;           //
	lf.lfItalic    = FALSE; 
	lf.lfCharSet   = DEFAULT_CHARSET; //
    strcpy(lf.lfFaceName,"");   //=@system

	font = CreateFontIndirect(&lf);
	SelectObject(dc, font);
*/
	DWORD m_color = RGB(255, 0, 0);
	DWORD p_color = RGB(0, 0, 255);
	DWORD n_color = RGB(255,255, 0);
	DWORD i_color = RGB(0, 255, 0);

	for (long i = 0; i < pMap->GetEyeshotCellLin(); i++)
	{
		for (long j = 0; j < pMap->GetEyeshotCellCol(); j++)
		{
			if (pMap->m_pCEyeshotCell[i][j].m_lActNum)
			{
				int cx = (j * 8 - sx) * UNIT_SCALE;
				int cy = (i * 8 - sy) * UNIT_SCALE;
				Rectangle(dc, cx, cy, cx + UNIT_DRAW_SIZE - 1, cy + UNIT_DRAW_SIZE - 1);
			}
		}
	}

	Entity	*pObj;
	for (long i = 0; i < pMap->GetEyeshotCellLin(); i++)
	{
		for (long j = 0; j < pMap->GetEyeshotCellCol(); j++)
		{
			pObj = pMap->m_pCEyeshotCell[i][j].m_pCChaL;
			while (pObj)
			{
				int mx = pObj->GetShape().centre.x;
				int my = pObj->GetShape().centre.y;
				int x = (mx / 100 - sx) * UNIT_SCALE + 1;
				int y = (my / 100 - sy) * UNIT_SCALE + 1;
				
				DWORD dwColor = m_color;
				if(pObj->IsCharacter()->IsPlayerCha())  
				{
					dwColor = p_color;
				}
				if(pObj->IsNpc())              
				{
					dwColor = n_color;
				}
				
				if(IsMouseLocation(mx / 100, my / 100)) TextOut(dc, x, y, pObj->GetName(), (int)strlen(pObj->GetName()));
				
				SetPixel(dc, x, y, dwColor);
				SetPixel(dc, x + 1, y, dwColor);
				SetPixel(dc, x, y + 1, dwColor);
				SetPixel(dc, x + 1, y + 1, dwColor);

				pObj = pObj->m_pCEyeshotCellNext;
			}

			pObj = pMap->m_pCEyeshotCell[i][j].m_pCItemL;
			while (pObj)
			{
				int mx = pObj->GetShape().centre.x;
				int my = pObj->GetShape().centre.y;
				int x = (mx / 100 - sx) * UNIT_SCALE + 1;
				int y = (my / 100 - sy) * UNIT_SCALE + 1;
				
				DWORD dwColor = 0;
				if(pObj->IsItem())             dwColor = i_color;
				
				if(IsMouseLocation(mx / 100, my / 100)) TextOut(dc, x, y, pObj->GetName(), (int)strlen(pObj->GetName()));
				
				SetPixel(dc, x, y, dwColor);
				SetPixel(dc, x + 1, y, dwColor);
				SetPixel(dc, x, y + 1, dwColor);
				SetPixel(dc, x + 1, y + 1, dwColor);

				pObj = pObj->m_pCEyeshotCellNext;
			}
		}
	}
	DeleteObject(brMonster);
}

static DWORD g_dwLastMouseX    = 0;
static DWORD g_dwLastMouseY    = 0;
static long  g_lLastViewAtMapX = 0;
static long  g_lLastViewAtMapY = 0;
static BOOL	 g_bDragMap         = FALSE;

BOOL CALLBACK MapDlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
		case WM_INITDIALOG:
		{
			break;
		}
		case WM_LBUTTONDOWN:
		{
			POINT p; GetCursorPos(&p);
			g_dwLastMouseX     = p.x;
			g_dwLastMouseY     = p.y;
			g_lLastViewAtMapX = g_lViewAtMapX;
			g_lLastViewAtMapY = g_lViewAtMapY;
			g_bDragMap         = TRUE;
			break;
		}
		case WM_LBUTTONUP:
		{
			g_bDragMap = FALSE;
			break;
		}
		case WM_MOUSEMOVE:
		{
			if(g_bDragMap)
			{
				POINT p; GetCursorPos(&p);
				g_lViewAtMapX = g_lLastViewAtMapX - (p.x - g_dwLastMouseX);
				g_lViewAtMapY = g_lLastViewAtMapY - (p.y - g_dwLastMouseY);
				if(g_lViewAtMapX < 0) g_lViewAtMapX = 0;
				if(g_lViewAtMapY < 0) g_lViewAtMapY = 0;
				if(g_bShowView) InvalidateRect(g_MapView, NULL, FALSE);
			}
			break;
		}
		case WM_PAINT:
        {
            HDC hdc; PAINTSTRUCT ps;
			hdc = BeginPaint(hwndDlg, &ps);
			
			if(g_bShowView)
			{
				HBRUSH brMonster = CreateSolidBrush(RGB(100, 100, 88));
				RECT rc; GetClientRect(hwndDlg, &rc);
			
				FillRect(hdc, &rc, brMonster);
				int w = rc.right - rc.left;
				int h = rc.bottom - rc.top;
			
				DrawMgrUnit(hdc, g_lViewAtMapX, g_lViewAtMapY, w, h);
				DrawMapUnit(g_pGameApp->GetMap(0)->GetCopy(0), hdc, g_lViewAtMapX, g_lViewAtMapY, w, h);
			
				DeleteObject(brMonster);
			}
			EndPaint(hwndDlg, &ps);
			break;
        }
	}
	return FALSE;
}

void CreateMainDialog(HINSTANCE hInst, HWND hParent)
{
    g_SysDlg = CreateDialog(hInst, MAKEINTRESOURCE(IDD_DLG_MAIN), hParent, DlgProc);
    ShowWindow(g_SysDlg, SW_SHOW);
	/*std::string	strCaption = "KOPOL-GS(Ver1.30-7)";
	strCaption += "[";
	strCaption += __DATE__;
	strCaption += " ";
	strCaption += __TIME__;
	strCaption += "]"; */
	SetWindowText(g_SysDlg, "GameServer");
	
	g_ReportView = CreateDialog(hInst, MAKEINTRESOURCE(IDD_REPORT_VIEW), g_SysDlg, ReportDlgProc);
	ShowWindow(g_ReportView, SW_SHOW);

	g_MapView = CreateDialog(hInst, MAKEINTRESOURCE(IDD_MAP_VIEW), g_SysDlg, MapDlgProc);
	ShowWindow(g_MapView, SW_SHOW);

	SetWindowPos(g_MapView, NULL, 65, 340, 0,0, SWP_NOSIZE);
}

void SystemReport(DWORD dwTimeParam)
{
	g_pGameApp->HandleLogList();

	static DWORD dwLastReportTime = 0;

	HWND hRunFlag   = GetDlgItem(g_ReportView, IDC_RUNFLAG);
	char szInfo[64];
	sprintf(szInfo, "%d", g_pGameApp->m_dwRunStep);
	if(hRunFlag) SetWindowText(hRunFlag, szInfo);

	if(dwTimeParam - dwLastReportTime < 1000) return;	

	if(g_bShowView) InvalidateRect(g_MapView, NULL, FALSE);
	
	if(dwLastReportTime==0) // 
	{
		HWND hDB = GetDlgItem(g_ReportView, IDC_GAMEDB);
		if(game_db.m_bInitOK)	SetWindowText(hDB, "ok");
		else					SetWindowText(hDB, "fail");
		
		HWND hMapList = GetDlgItem(g_ReportView, IDC_MAP_LIST);
		char szText[128];
		for(int i = 0; i < g_Config.m_nMapCnt; i++)
		{
			if(g_Config.m_btMapOK[i])
			{
				sprintf(szText, "%-12s   ok", g_Config.m_szMapList[i]);
			}
			else
			{
				sprintf(szText, "%-12s fail", g_Config.m_szMapList[i]);
			}
			SendMessage(hMapList, LB_ADDSTRING, 0, (LPARAM)szText);  
		}
	}
	
	dwLastReportTime = dwTimeParam;

	HWND hFPS       = GetDlgItem(g_ReportView, IDC_FPS);
	HWND hLoop      = GetDlgItem(g_ReportView, IDC_LOOP);
	HWND hChaCnt    = GetDlgItem(g_ReportView, IDC_ACTIVEOBJ);
	HWND hPlayerCnt = GetDlgItem(g_ReportView, IDC_PLAYER);
	HWND hActiveUnit= GetDlgItem(g_ReportView, IDC_ACTIVEOUNIT);
	HWND hPKCnt     = GetDlgItem(g_ReportView, IDC_PK_CNT);
	HWND hDBLogLeft = GetDlgItem(g_ReportView, IDC_DBLOG_LEFT);
	HWND hPerLogCnt = GetDlgItem(g_ReportView, IDC_PERLOGCNT);

	char szFPS[64]; 
	sprintf(szFPS, "%d", g_pGameApp->m_dwFPS);
	if(hFPS) SetWindowText(hFPS, szFPS);
	sprintf(szFPS, "%d", g_pGameApp->m_dwRunCnt);
	if(hLoop) SetWindowText(hLoop, szFPS);
	sprintf(szFPS, "%d", g_pGameApp->m_dwChaCnt);
	if(hChaCnt) SetWindowText(hChaCnt, szFPS);
	sprintf(szFPS, "%d", g_pGameApp->m_dwPlayerCnt);
	if(hPlayerCnt) SetWindowText(hPlayerCnt, szFPS);
	sprintf(szFPS, "%d", g_pGameApp->m_dwActiveMgrUnit);
	if(hActiveUnit) SetWindowText(hActiveUnit, szFPS);
	sprintf(szFPS, "%d", g_pGameApp->m_dwRunStep);
	if(hRunFlag) SetWindowText(hRunFlag, szFPS);
	sprintf(szFPS, "%d", g_pGameApp->GetLogLeft());
	if(hDBLogLeft) SetWindowText(hDBLogLeft, szFPS);
	sprintf(szFPS, "%d", g_pGameApp->GetPerLogCnt());
	if(hPerLogCnt) SetWindowText(hPerLogCnt, szFPS);

	
	char szText[128];
	
	if(rand()%2==0)
	{
		CMapRes *pCMap = g_pGameApp->FindMapByName("teampk");
		if(pCMap)
		{
			// 
			HWND hPKList = GetDlgItem(g_ReportView, IDC_PK_LIST);
			SendMessage(hPKList, LB_RESETCONTENT, 0, 0);
			//sprintf(szText, "    ");
			sprintf(szText, RES_STRING(GM_SYSTEMDIALOG_CPP_00001));
			SendMessage(hPKList, LB_ADDSTRING, 0, (LPARAM)szText);  
			
			// 
			pCMap->BeginGetUsedCopy();
			SubMap *pCMapCopy;
			int nPKCnt = 0;
			int nPlayerCnt = 0;
			while (pCMapCopy = pCMap->GetNextUsedCopy())
			{
				int nNum = pCMapCopy->GetPlayerNum();
				sprintf(szText, "[%d]     %d", pCMapCopy->GetCopyNO(), nNum);
				SendMessage(hPKList, LB_ADDSTRING, 0, (LPARAM)szText);  
				nPKCnt++;
				nPlayerCnt+=nNum;
			}
			sprintf(szFPS, "%d :%d", nPKCnt, nPlayerCnt);
			if(hPKCnt) SetWindowText(hPKCnt, szFPS); // 
		}
	}

	HWND hGateList = GetDlgItem(g_ReportView, IDC_GATE_LIST);
	SendMessage(hGateList, LB_RESETCONTENT, 0, 0);
	for(int i = 0; i < g_Config.m_nGateCnt; i++)
	{
		if(ISVALIDGATE(i))
		{
			sprintf(szText, "%-16sconnected", g_Config.m_szGateIP[i]);
		}
		else
		{
			sprintf(szText, "%-16stry......", g_Config.m_szGateIP[i]);
		}
		SendMessage(hGateList, LB_ADDSTRING, 0, (LPARAM)szText);  
	}

	HWND hMapXY = GetDlgItem(g_SysDlg, IDC_MAPXY);
	sprintf(szFPS, "%d  %d", g_lViewAtMapX, g_lViewAtMapY);
	SetWindowText(hMapXY, szFPS);
	
	POINT p; GetCursorPos(&p);
	RECT rc; GetWindowRect(g_MapView, &rc);
	g_lMouseAtMapX = (p.x - rc.left) / UNIT_SCALE + g_lViewAtMapX;
	g_lMouseAtMapY = (p.y - rc.top)  / UNIT_SCALE + g_lViewAtMapY;
	
	if(g_lMouseAtMapX < 0) g_lMouseAtMapX = 0;
	if(g_lMouseAtMapY < 0) g_lMouseAtMapY = 0;

	HWND hMouseXY = GetDlgItem(g_SysDlg, IDC_MOUSEXY);
	sprintf(szFPS, "%d  %d", g_lMouseAtMapX, g_lMouseAtMapY);
	SetWindowText(hMouseXY, szFPS);

}

#endif
