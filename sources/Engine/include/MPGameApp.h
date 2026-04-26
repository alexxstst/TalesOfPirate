//#################################
// MindPower GameApp Header File
// Render & GameApp Routines
// Created By Ryan Wang
// Last Modified : 2004/02/04 
//#################################
#pragma once

#include "InputSystem.h"

#define KEYBOARD_BUFFERSIZE		10
#define MOUSE_BUFFERSIZE		10


#define DI_KEY_NOACTION 0
#define DI_KEY_PRESSED  1
#define DI_KEY_RELEASED 2

//  Совместимость: phase-маска клавиши (was KEY_FREE/PUSH/HOLD/POP).
//  Значения зеркалят Corsairs::Engine::Input::KeyPhaseBits.
#define KEY_FREE 0x0001
#define KEY_PUSH 0x0002
#define KEY_HOLD 0x0004
#define KEY_POP  0x0008

class MPCameraNOLEECH;
class MPRender;
class ConsoleProcessor;

#define M_LDown		0x0001
#define M_MDown		0x0002
#define M_RDown		0x0004
#define M_Down		0x0008	// 
#define M_LUp		0x0010
#define M_MUp		0x0020
#define M_RUp		0x0040
#define M_Move		0x0080
#define M_LDB		0x0100
#define M_MDB		0x0200
#define M_RDB		0x0400
#define M_LClick	0x0800 // 
#define M_MClick	0x1000
#define M_RClick	0x2000

class MPGameApp {
public:
	MPGameApp();
	~MPGameApp();

	virtual void _PreMouseRun(DWORD dwMouseKey) {
	}

	// camMove Vim 3D Vision
	virtual void _FrameMove(DWORD dwTimeParam, bool camMove = false) {
	}

	virtual void _Render() {
	}

	virtual BOOL _Init() {
		return TRUE;
	}

	virtual void _End() {
	}

	BOOL Init(HINSTANCE hInst, const char* pszClassName, int nScrWidth = 800, int nScrHeight = 600, int nColorBit = 16,
			  BOOL bFullScreen = FALSE);
	void FrameMove(DWORD dwTimeParam);
	void Render();
	virtual void End();

	virtual void MouseButtonDown(int nButton) {
	}

	virtual void MouseButtonUp(int nButton) {
	}

	virtual void MouseButtonDB(int nButton) {
	}

	virtual void MouseMove(int nOffsetX, int nOffsetY) {
	}

	virtual void MouseScroll(int nOffset) {
	}

	virtual void HandleKeyDown(DWORD dwKey) {
	}

	virtual void HandleKeyUp() {
	}

	virtual void MouseContinue(int nButton) {
	}


	DWORD GetMouseKey() {
		return _dwMouseKey;
	}

	// input interface
	BOOL IsKeyContinue(BYTE btDIKey); // 
	BOOL IsKeyDown(BYTE btDIKey);
	BOOL IsMouseButtonPress(int nButtonNo); // 
	BOOL IsCtrlPress() {
		return (GetKeyState(VK_CONTROL) & 0xff00);
	}

	BOOL IsAltPress() {
		return (GetKeyState(VK_MENU) & 0xff00);
	}

	BOOL IsShiftPress() {
		return (GetKeyState(VK_SHIFT) & 0xff00);
	}

	int GetMouseX() {
		return _nMousePosX;
	}

	int GetMouseY() {
		return _nMousePosY;
	}

	void SetMouseXY(int nPosX, int nPosY) {
		_nMousePosX = nPosX;
		_nMousePosY = nPosY;
	}

	HWND GetHWND() {
		return _hWnd;
	}

	int GetWindowWidth() {
		return _nWindowWidth;
	}

	int GetWindowHeight() {
		return _nWindowHeight;
	}

	BOOL IsFullScreen() {
		return _bFullScreen;
	}

	void EnableCheckInputWnd(BOOL bEnable) {
		_bCheckInputWnd = bEnable;
	}

	// Console
	ConsoleProcessor* GetConsole() {
		return _pConsole;
	}


	void SetCaption(const char* pszCaption);

	void EnableSprintScreen(BOOL bEnable) {
		_bEnSpScreen = bEnable;
	}

	BOOL IsEnableSpScreen() {
		return _bEnSpScreen;
	}

	void EnableSprintAvi(BOOL bEnable) {
		_bEnSpAvi = bEnable;
	}

	BOOL IsEnableSpAvi() {
		return _bEnSpAvi;
	}

	void EnableSprintSmMap(BOOL bEnable) {
		_bEnSpSmMap = bEnable;
	}

	BOOL IsEnableSpSmMap() {
		return _bEnSpSmMap;
	}

	DWORD GetFrameMoveUseTime() {
		return _dwFrameMoveUseTime;
	}

	DWORD GetRenderUseTime() {
		return _dwRenderUseTime;
	}

	void SetInputActive(bool bActive);

	BOOL LoadResource();
	BOOL LoadRes2();
	BOOL LoadRes3();

protected:
	BOOL _InitInput();
	void _RenderAxis();
	void _ReadKeyboardInput();
	void _SetupView(MPCameraNOLEECH* pCamera); // D3D ViewMatrix

	// UI
	void _RenderUI();

	bool _CanInput();

protected:
	HINSTANCE _hInst;
	HWND _hWnd;

	bool _bActive;

	// Texture Management

	BYTE _bCanDB;

	BYTE _btButtonState[3];
	BYTE _btLastButtonState[3];

	UINT _nDBClickTime;
	UINT _nLastClickTime;
	bool _bLastDBClick;

	UINT _nDBTime;


	BOOL _bDrag;
	int _nLastDragX;
	int _nLastDragY;
	int _nMousePosX;
	int _nMousePosY;

	int _nLogoTexID;

	// Console
	ConsoleProcessor* _pConsole;


	BOOL _bEnSpScreen;
	BOOL _bEnSpAvi;
	BOOL _bEnSpSmMap;
	int _nWindowWidth;
	int _nWindowHeight;
	BOOL _bFullScreen;
	BOOL _bCheckInputWnd;

	// performance routines
	DWORD _dwRenderUseTime;
	DWORD _dwFrameMoveUseTime;

	DWORD _dwMouseKey;

	DIMOUSESTATE2 _sDims2;

public:
	//  Phase-аксессоры — тонкие редиректы на InputSystem (совместимость с legacy API).
	inline BYTE getKeyState(BYTE dikey);
	inline BYTE getASCIIKeyState(BYTE codeASCII);
	inline BOOL isKeyFree(BYTE dikey);
	inline BOOL isKeyPush(BYTE dikey);
	inline BOOL isKeyHold(BYTE dikey);
	inline BOOL isKeyPop(BYTE dikey);
	inline BOOL isKeyChange(BYTE dikey);

	inline BOOL isKeyStateDown(BYTE dikey);
	inline BOOL isKeyStateUp(BYTE dikey);
};

inline BOOL MPGameApp::IsKeyContinue(BYTE dikey) {
	return isKeyHold(dikey);
}

inline BOOL MPGameApp::IsKeyDown(BYTE dikey) {
	return isKeyPush(dikey);
}

inline BOOL MPGameApp::IsMouseButtonPress(int nButtonNo) {
	return _btButtonState[nButtonNo];
}

inline BYTE MPGameApp::getKeyState(BYTE dikey) {
	return Corsairs::Engine::Input::InputSystem::Instance().GetKeyPhase(dikey);
}

inline BYTE MPGameApp::getASCIIKeyState(BYTE codeASCII) {
	return Corsairs::Engine::Input::InputSystem::Instance().GetAsciiPhase(codeASCII);
}

inline BOOL MPGameApp::isKeyFree(BYTE dikey) {
	return getKeyState(dikey) & KEY_FREE;
}

inline BOOL MPGameApp::isKeyPush(BYTE dikey) {
	return getKeyState(dikey) & KEY_PUSH;
}

inline BOOL MPGameApp::isKeyHold(BYTE dikey) {
	return getKeyState(dikey) & KEY_HOLD;
}

inline BOOL MPGameApp::isKeyPop(BYTE dikey) {
	return getKeyState(dikey) & KEY_POP;
}

inline BOOL MPGameApp::isKeyChange(BYTE dikey) {
	return (isKeyPush(dikey) || isKeyPop(dikey));
}

inline BOOL MPGameApp::isKeyStateDown(BYTE dikey) {
	return (isKeyPush(dikey) || isKeyHold(dikey));
}

inline BOOL MPGameApp::isKeyStateUp(BYTE dikey) {
	return (isKeyFree(dikey) || isKeyPop(dikey));
}
