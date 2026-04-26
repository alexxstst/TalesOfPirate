#include "stdafx.h"
#include <process.h>
#include "gameloading.h"
#include "gameapp.h"
#include "resource.h"
#include "GameConfig.h"
#include "UIsystemform.h"
#include "GlobalVar.h"
#include "CrushSystem.h"

using namespace std;

extern HINSTANCE g_hInstance;

bool DrawBMPFile(RECT rect, LPTSTR pszFile, HDC hDC) {
	HDC hdcCompatible = CreateCompatibleDC(hDC);

	HBITMAP hBitmap;

	hBitmap = (HBITMAP)LoadImage(NULL, pszFile, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	if (!hBitmap)
		return false;

	BITMAP bm;
	GetObject(hBitmap, sizeof(BITMAP), &bm);

	HBITMAP old = (HBITMAP)SelectObject(hdcCompatible, hBitmap);

	BitBlt(hDC, rect.right - bm.bmWidth, rect.bottom - bm.bmHeight, bm.bmWidth, bm.bmHeight, hdcCompatible, 0, 0,
		   SRCCOPY);

	SelectObject(hdcCompatible, old);

	DeleteObject(hBitmap);
	DeleteObject(hdcCompatible);

	return true;
}

LRESULT CALLBACK LoadingProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	static int i = 0;
	switch (uMsg) {
	case WM_INITDIALOG: break;
	case WM_CHAR: {
		if (wParam == 27)
			PostQuitMessage(0);
	}
	break;
	case WM_GRAPHNOTIFY: break;
	case WM_CLOSE: {
		PostQuitMessage(0);
	}
	break;
	case WM_PAINT: {
		RECT rc;
		GetClientRect(hwndDlg, &rc);
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwndDlg, &ps);

		char buff[MAX_PATH] = {0};
		extern long g_nCurrentLogNo;
		srand((unsigned)time(0) * (g_nCurrentLogNo + 1));

		int random = rand();
		sprintf(buff, "texture\\ui\\loading_%i.bmp", random % 9 + 1);

		DrawBMPFile(rc, buff, hdc);

		EndPaint(hwndDlg, &ps);

		return FALSE;
	}
	case WM_ERASEBKGND: {
		return FALSE;
	}
	default:
		return DefWindowProc(hwndDlg, uMsg, wParam, lParam);
	}
	return FALSE;
}


ATOM MyRegisterClass(HINSTANCE hInstance) {
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = LoadingProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_KOP));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOWTEXT);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	wcex.lpszClassName = "Loading...";
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

unsigned __stdcall LoadingThread(void* param) {
	::SetThreadName("loading");
	TalesOfPirate::Utils::Crush::SetPerThreadCRTExceptionBehavior();
	GameLoading* load = (GameLoading*)param;
	ATOM ret = MyRegisterClass(g_hInstance);

	// Get the users screen width & height
	int display_width = GetSystemMetrics(SM_CXFULLSCREEN);
	int display_height = GetSystemMetrics(SM_CYFULLSCREEN);
	// Width & height of loading image
	int width = 400;
	int height = 200;
	// Get the center
	int left = (display_width / 2) - (width / 2);
	int top = (display_height / 2) - (height / 2);

	RECT rc;
	SetRect(&rc, 0, 0, width, height);

	load->m_hLoading = CreateWindowEx(WS_OVERLAPPED, "Loading...", "Loading...", WS_POPUP,
									  left, top, width, height, NULL, NULL, g_hInstance, NULL);

	DWORD error = ::GetLastError();

	if (!load->m_hLoading) {
		std::string strfile = "loading_err.txt";
		FILE* fp = fopen(strfile.c_str(), "a+");
		if (fp) {
			char buffer[] = "Could not create loading window.\n";
			fwrite(buffer, sizeof(buffer), 1, fp);
			fclose(fp);
		}
		return 1;
	}

	ShowWindow(load->m_hLoading, SW_SHOW);
	UpdateWindow(load->m_hLoading);


	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}

GameLoading* GameLoading::_instance = NULL;

GameLoading* GameLoading::Init() {
	if (_instance == NULL)
		_instance = new GameLoading();
	return _instance;
}

GameLoading::GameLoading() {
}

GameLoading::~GameLoading() {
}

void GameLoading::Create(string param) {
	if (param.find("table_bin") != -1) {
		szParam = param;
		return;
	}
	unsigned tid;
	_beginthreadex(0, 0, LoadingThread, this, 0, &tid);
}

void GameLoading::Close() {
	if (szParam.size() > 0) return;
	RECT rc;
	GetClientRect(m_hLoading, &rc);


	int height = rc.bottom - rc.top - (rc.right - rc.left) * 9.0 / 16.0;

	RECT rect;
	rect.left = rc.left;
	rect.right = rc.right;
	rect.top = rc.bottom - height / 2.0 - 1;
	rect.bottom = rc.bottom;

	::InvalidateRect(m_hLoading, &rect, TRUE);

	::UpdateWindow(m_hLoading);

	PostMessage(m_hLoading, WM_CLOSE, 0, 0);

	HWND hwnd = g_pGameApp->GetHWND();
	SetForegroundWindow(hwnd);
}

bool GameLoading::Active() {
	if (szParam.size() > 0) return true;
	::SendMessage(m_hLoading, WM_ACTIVATE, TRUE, TRUE);
	::UpdateWindow(m_hLoading);
	return true;
}
