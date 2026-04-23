//################################
// MindPower 3D Engine
// class MPGameApp Implement
// Created By   : Ryan Wang 
// Last Modifed : 2004/02/06
//################################

#include "Stdafx.h"
#include "MPGameApp.h"
#include "d3dutil.h"
#include "dxutil.h"
#include "MPCamera.h"
#include "MPTextureSet.h"
#include "MPTerrainSet.h"
#include "ConsoleProcessor.h"

#include "lwInterface.h"
#include "lwIFunc.h"
#include "MPCharacter.h"

#include "MPResourceSet.h"

using namespace std;
//Del by lark.li 20080611
//#include "FontSystem.h"

LW_USING


MPGameApp::MPGameApp()
	: _dwRenderUseTime(0),
	  _dwFrameMoveUseTime(0),
	  _bCheckInputWnd(TRUE) {
	_pConsole = new ConsoleProcessor;
	//_pMainCam = new MPCamera;

	_nMousePosX = 0;
	_nMousePosY = 0;

	memset(_btButtonState, 0, 3);
	memset(_btLastButtonState, 0, 3);

	_bLastDBClick = false;
	_bCanDB = false;
	_nDBTime = 0;

	_bEnSpScreen = FALSE;
	_bEnSpAvi = FALSE;
	_bEnSpSmMap = FALSE;
	_bFullScreen = FALSE;

	_bActive = false;
}

MPGameApp::~MPGameApp() {
}


BOOL MPGameApp::Init(HINSTANCE hInst, const char* pszClassName, int nScrWidth, int nScrHeight, int nColorBit,
					 BOOL bFullScreen) {
	_hInst = hInst;

	DWORD dwWindowStyle = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;

	if (bFullScreen == 2 || bFullScreen == 1) //
	{
		dwWindowStyle = WS_VISIBLE | WS_POPUP | WS_CLIPCHILDREN;
	}

	RECT rc;
	SetRect(&rc, 0, 0, nScrWidth, nScrHeight);
	AdjustWindowRect(&rc, dwWindowStyle, FALSE);

	int nFrameSize = GetSystemMetrics(SM_CYSIZEFRAME);
	int nCaptionSize = GetSystemMetrics(SM_CXSIZE);

	int nWindowWidth = rc.right - rc.left;
	int nWindowHeight = rc.bottom - rc.top; // nScrHeight + nFrameSize + nCaptionSize;
	_hWnd = CreateWindow(pszClassName, "MindPower3D Application", dwWindowStyle,
						 CW_USEDEFAULT, CW_USEDEFAULT, nWindowWidth, nWindowHeight, NULL, NULL, hInst, NULL);

	if (!_hWnd) {
		return FALSE;
	}

	GetClientRect(_hWnd, &rc);
	_nWindowWidth = rc.right - rc.left;
	_nWindowHeight = rc.bottom - rc.top;
	_bFullScreen = bFullScreen;

	// LG_SetWnd        HWND

	int dev_width;
	int dev_height;

	if (bFullScreen) {
		dev_width = nScrWidth;
		dev_height = nScrHeight;
	}
	else {
		dev_width = _nWindowWidth;
		dev_height = _nWindowHeight;
	}

	if (!g_Render.Init(_hWnd, dev_width, dev_height, nColorBit, bFullScreen)) {
		return FALSE;
	}

	new TextureManager();

	_nLogoTexID = GetTextureID("texture/logo.BMP");

	_InitInput();

	// Del by lark.li 20080611
	// Added by clp
	//FontModule::FontSystem::getSingleton().init();

	if (_Init() == 0)
		return 0;

	ShowWindow(_hWnd, SW_SHOW);
	UpdateWindow(_hWnd);

	return 1;
}

BOOL MPGameApp::LoadResource() {
	if (!g_Render.InitResource()) {
		return FALSE;
	}
	return TRUE;
}

BOOL MPGameApp::LoadRes2() {
	if (!g_Render.InitRes2()) {
		return FALSE;
	}

	return TRUE;
}

BOOL MPGameApp::LoadRes3() {
	if (!g_Render.InitRes3()) {
		return FALSE;
	}
	return TRUE;
}

static DWORD _time;

void MPGameApp::FrameMove(DWORD dwTimeParam) {
	MPTimer tUseTime;
	tUseTime.Begin();

	_ReadKeyboardInput();

	g_Render.UpdateCullInfo(); //added by billy

	if (_pConsole->IsVisible()) {
		// Tick — анимация мигающего курсора. Отрисовка текста — в клиенте
		// (CGameApp::_RenderConsoleText) через FontManager::Get(FontSlot::Console).
		_pConsole->Tick();
	}

	lwISceneMgr* sm = g_Render.GetInterfaceMgr()->sys_graphics->GetSceneMgr();
	sm->Update();

	_FrameMove(_time = dwTimeParam);

	_dwFrameMoveUseTime = tUseTime.End();
}

void MPGameApp::Render() {
	MPTimer tRenderUse;
	tRenderUse.Begin();

	lwISceneMgr* sm = g_Render.GetInterfaceMgr()->sys_graphics->GetSceneMgr();

	// g_Render.LookAt(_pMainCam->m_EyePos, _pMainCam->m_RefPos);

	//  g_Render.SetCurrentView(MPRender::VIEW_WORLD);


	///////////////////////////////////////////////////////////

	if (!g_Render.BeginRender(true)) return;

	// _RenderAxis();

	sm->BeginRender();
	sm->Render();

	// Фон консоли — сплошной colored quad (без текстуры) через D3D. Рисуем
	// ДО _Render(), чтобы UI и текст консоли (CGameApp::_RenderConsoleText)
	// ложились поверх. Раньше использовалась лого-текстура с tint'ом, но у
	// неё неоднородная alpha → фон казался "пропавшим" в местах prozrachnosti.
	if (_pConsole->IsVisible()) {
		const DWORD argb = _pConsole->GetBackdropColor();
		const float w = static_cast<float>(_pConsole->GetWidth());
		const float h = static_cast<float>(_pConsole->GetHeight() + 14);

		struct CV { float x, y, z, rhw; DWORD color; };
		const CV verts[6] = {
			{ 0.0f, 0.0f, 0.0f, 1.0f, argb },
			{    w, 0.0f, 0.0f, 1.0f, argb },
			{ 0.0f,    h, 0.0f, 1.0f, argb },
			{    w, 0.0f, 0.0f, 1.0f, argb },
			{    w,    h, 0.0f, 1.0f, argb },
			{ 0.0f,    h, 0.0f, 1.0f, argb },
		};

		auto* dev = g_Render.GetDevice();
		if (dev) {
			dev->SetTexture(0, nullptr);
			dev->SetVertexShader(nullptr);
			dev->SetPixelShader(nullptr);
			dev->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
			dev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			dev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			dev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			dev->SetRenderState(D3DRS_LIGHTING, FALSE);
			dev->SetRenderState(D3DRS_ZENABLE, FALSE);
			dev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
			dev->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1);
			dev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
			dev->SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
			dev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
			dev->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, verts, sizeof(CV));
		}
	}

	_Render(); //

	sm->EndRender();

	///////////////////////////////////////////////////////////

	//g_Render.EnableZBuffer(FALSE);
	//
	//   g_Render.SetCurrentView(MPRender::VIEW_WORLD);

	// INFO_CMD / INFO_FPS текст рисует клиент напрямую (через FontManager
	// в CGameApp::_Render). Прежний RenderDebugInfo использовал _pFont,
	// который никогда не инициализировался — вызывал AV.

	// Del by lark.l i20080611
	// Draw font
	// FontModule::FontSystem::getSingleton().update(0.01f);

	g_Render.EndRender(true);

	_dwRenderUseTime = tRenderUse.End();
}

// UpdateConsoleText удалён — отрисовка текста консоли перенесена в клиент:
// CGameApp::_RenderConsoleText (sources/Client/src/GameAppRender.cpp)
// использует FontManager::Get(FontSlot::Console).

void MPGameApp::_RenderUI() {
}

void MPGameApp::_RenderAxis() {
	struct AXIS_VERTEX {
		D3DVECTOR pos;
		DWORD diffuse;
	};

	D3DXMATRIX mat;
	D3DXMatrixIdentity(&mat);

#if(defined USE_MANAGED_RES)
	g_Render.SetTransformWorld(&mat);
#else
	g_Render.GetDevice()->SetTransform(D3DTS_WORLD, &mat);
#endif


	AXIS_VERTEX pVertices[6];

	// z
	pVertices[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVertices[1].pos = D3DXVECTOR3(0.0f, 0.0f, 50.0f);
	pVertices[0].diffuse = 0xffffffff;
	pVertices[1].diffuse = 0xffffffff;

	// x
	pVertices[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVertices[3].pos = D3DXVECTOR3(100.0f, 0.0f, 0.0f);
	pVertices[2].diffuse = 0xffff0000;
	pVertices[3].diffuse = 0xffff0000;

	// y
	pVertices[4].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVertices[5].pos = D3DXVECTOR3(0.0f, 100.0f, 0.0f);
	pVertices[4].diffuse = 0xFF00ffff;
	pVertices[5].diffuse = 0xFF00ffff;

	g_Render.SetRenderState(D3DRS_LIGHTING, FALSE);
	g_Render.SetTexture(0, 0);
	g_Render.SetVertexShader(NULL);
	g_Render.SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
	g_Render.GetDevice()->DrawPrimitiveUP(D3DPT_LINELIST, 3, &pVertices, sizeof(AXIS_VERTEX));
	// _pD3DDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
}

BOOL MPGameApp::_InitInput() {
	if (!Corsairs::Engine::Input::InputSystem::Instance().Init(_hWnd)) {
		ToLogService("common", "Init InputSystem failed!");
		return FALSE;
	}

	_nDBClickTime = ::GetDoubleClickTime();
	_nLastClickTime = timeGetTime();

	return TRUE;
}

void MPGameApp::SetInputActive(bool bActive) {
	Corsairs::Engine::Input::InputSystem::Instance().Reset();
	_bActive = bActive;
}

void MPGameApp::_ReadKeyboardInput() {
	using Corsairs::Engine::Input::InputSystem;
	using Corsairs::Engine::Input::MouseButton;

	ZeroMemory(_btButtonState, sizeof(_btButtonState));
	_dwMouseKey = 0;

	if (!_bActive) {
		return;
	}
	if (!_CanInput()) {
		return;
	}

	//  Обновить edge-фазы клавиш/мыши. Оконные сообщения уже прокинуты в InputSystem из WndProc.
	auto& input = InputSystem::Instance();
	input.Update();

	//  HandleKeyDown — для клавиш, только что нажатых в этом кадре.
	for (DWORD codeScan = 0; codeScan < 256; ++codeScan) {
		if (input.GetKeyPhase(static_cast<std::uint8_t>(codeScan)) == Corsairs::Engine::Input::KeyPush) {
			HandleKeyDown(codeScan);
		}
	}

	const int nOffsetX = input.GetMouseDeltaX();
	const int nOffsetY = input.GetMouseDeltaY();
	const int nScroll  = input.GetMouseWheelDelta();

	_btButtonState[0] = input.IsMouseButtonDown(MouseButton::Left)   ? 0x80 : 0;
	_btButtonState[1] = input.IsMouseButtonDown(MouseButton::Right)  ? 0x80 : 0;
	_btButtonState[2] = input.IsMouseButtonDown(MouseButton::Middle) ? 0x80 : 0;

	if (nOffsetX || nOffsetY) //
	{
		//MouseMove(nOffsetX, nOffsetY);
		_bCanDB = false;

		_dwMouseKey |= M_Move;
	}

	if (nScroll) {
		MouseScroll(nScroll);
	}
	UINT time = timeGetTime();

	for (int i = 0; i < 3; i++) {
		if (_btButtonState[i] && !_btLastButtonState[i]) {
			//MouseButtonDown(i);
			_dwMouseKey |= M_Down;
			if (i == 0)
				_dwMouseKey |= M_LDown;
			else if (i == 1)
				_dwMouseKey |= M_RDown;
			else
				_dwMouseKey |= M_MDown;


			if ((time - _nLastClickTime < _nDBClickTime)) {
				if (!nOffsetX && !nOffsetY && _bCanDB) {
					if (_bLastDBClick) {
						if (time - _nDBTime > _nDBClickTime)
							_bLastDBClick = false;
					}
					if (!_bLastDBClick) {
						//MouseButtonDB(i);
						_bLastDBClick = true;
						_nDBTime = time;

						if (i == 0)
							_dwMouseKey |= M_LDB;
						else if (i == 1)
							_dwMouseKey |= M_RDB;
						else
							_dwMouseKey |= M_MDB;
					}
				}
			}
			else {
				_bLastDBClick = false;
			}
			_nLastClickTime = time;
			_bCanDB = true;
		}

		if (!_btButtonState[i] && _btLastButtonState[i]) {
			//MouseButtonUp(i);
			if (i == 0)
				_dwMouseKey |= M_LUp;
			else if (i == 1)
				_dwMouseKey |= M_RUp;
			else
				_dwMouseKey |= M_MUp;
		}
		if (_btButtonState[i] && _btLastButtonState[i]) {
			MouseContinue(i);
		}

		_btLastButtonState[i] = _btButtonState[i];
	}

	_PreMouseRun(_dwMouseKey);

	if (_dwMouseKey & M_Move) {
		MouseMove(nOffsetX, nOffsetY);
	}

	if (_dwMouseKey & M_Down) {
		if (_dwMouseKey & M_LDown)
			MouseButtonDown(0);
		if (_dwMouseKey & M_RDown)
			MouseButtonDown(1);
		if (_dwMouseKey & M_MDown)
			MouseButtonDown(2);
	}

	if (_dwMouseKey & M_LDB)
		MouseButtonDB(0);
	if (_dwMouseKey & M_RDB)
		MouseButtonDB(1);
	if (_dwMouseKey & M_MDB)
		MouseButtonDB(2);

	if (_dwMouseKey & M_LUp)
		MouseButtonUp(0);
	if (_dwMouseKey & M_RUp)
		MouseButtonUp(1);
	if (_dwMouseKey & M_MUp)
		MouseButtonUp(2);
}

void MPGameApp::SetCaption(const char* pszCaption) {
	if (_hWnd == 0) return;
	SetWindowText(_hWnd, pszCaption);
}

bool MPGameApp::_CanInput() {
	return true;
	if (::GetFocus() != _hWnd)
		return false;

	POINT p;
	RECT rc;
	::GetCursorPos(&p);
	::GetWindowRect(_hWnd, &rc);
	if (!PtInRect(&rc, p)) {
		return false;
	}
	return true;
}

void MPGameApp::End() {
	_End();

	SAFE_DELETE(_pConsole);


	Corsairs::Engine::Input::InputSystem::Instance().Release();

	g_Render.End();

	ToLogService("common", "exit game appliaction successful!");
}

