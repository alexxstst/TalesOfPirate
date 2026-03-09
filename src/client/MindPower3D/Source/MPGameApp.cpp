//################################
// MindPower 3D Engine
// class MPGameApp Implement
// Created By   : Ryan Wang 
// Last Modifed : 2004/02/06
//################################

#include "MindPowerAPI.h"
#include "Stdafx.h"
#include "MPGameApp.h"
#include "d3dutil.h"
#include "dxutil.h"
#include "MPCamera.h"
#include "MPTextureSet.h"
#include "MPTerrainSet.h"
#include "MPConsole.h"

#include "lwGuidObj.h"
#include "lwInterface.h"
#include "lwIFunc.h"
#include "MPCharacter.h"

#include "MPResourceSet.h"

using namespace std;
//Del by lark.li 20080611
//#include "FontSystem.h"

LW_USING

MPTexSet*		MPTexSet::_Instance		= NULL;
MPTerrainSet*	MPTerrainSet::_Instance = NULL;
MPResourceSet*  MPResourceSet::m_pInstance = NULL;


MPGameApp::MPGameApp()
:_dwRenderUseTime(0),
 _dwFrameMoveUseTime(0),
 _bCheckInputWnd(TRUE)
{
	_pDI			 = NULL;
	_pDIKeyboard	 = NULL;
	_pDIMouse		 = NULL;

	_pConsole = new MPConsole;
	//_pMainCam = new MPCamera;

	_nMousePosX      = 0;
	_nMousePosY      = 0;

	memset(_btButtonState, 0, 3);
	memset(_btLastButtonState, 0, 3);

	// ----- Added by CLP ----- //
	memset ( mASCKeysState, KEY_FREE, sizeof ( mASCKeysState ) );
	memset ( mKeyState, KEY_FREE, sizeof ( mKeyState ) );
	// ----- Added by CLP ----- //


	_bLastDBClick = false;
	_bCanDB = false;
	_nDBTime = 0;

	_bEnSpScreen = FALSE;
	_bEnSpAvi	 = FALSE;
	_bEnSpSmMap  = FALSE;
    _bFullScreen = FALSE;

	_bActive= false;
}

MPGameApp::~MPGameApp()
{
}


BOOL MPGameApp::Init(HINSTANCE hInst,const char *pszClassName, int nScrWidth, int nScrHeight, int nColorBit, BOOL bFullScreen)
{
	_hInst			= hInst;

	DWORD dwWindowStyle = WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX; 
	
    if(bFullScreen==2 ||bFullScreen==1) // 
    {
        dwWindowStyle = WS_VISIBLE|WS_POPUP|WS_CLIPCHILDREN;
    }

    RECT rc; SetRect( &rc, 0, 0, nScrWidth, nScrHeight);
	AdjustWindowRect( &rc, dwWindowStyle, FALSE );
    
    int nFrameSize   = GetSystemMetrics(SM_CYSIZEFRAME);
	int nCaptionSize = GetSystemMetrics(SM_CXSIZE);

    int nWindowWidth = rc.right - rc.left;
    int nWindowHeight = rc.bottom - rc.top; // nScrHeight + nFrameSize + nCaptionSize;
	_hWnd = CreateWindow(pszClassName, "MindPower3D Application", dwWindowStyle, 
	CW_USEDEFAULT, CW_USEDEFAULT, nWindowWidth, nWindowHeight, NULL, NULL, hInst, NULL);

	if(!_hWnd)
	{
		return FALSE;
	}

    GetClientRect(_hWnd, &rc);
    _nWindowWidth  = rc.right  - rc.left;
    _nWindowHeight = rc.bottom - rc.top;
    _bFullScreen   = bFullScreen;
    
	int dev_width;
    int dev_height;

    if(bFullScreen)
    {
        dev_width = nScrWidth;
        dev_height = nScrHeight;
    }
    else
    {
        dev_width = _nWindowWidth;
        dev_height = _nWindowHeight;
    }

   	if(!g_Render.Init(_hWnd, dev_width, dev_height, nColorBit, bFullScreen))
    {
        return FALSE;
    }

	MPTexSet *pTextureSet = new MPTexSet(0, 8192);
	  
	_nLogoTexID = GetTextureID("texture/logo.BMP");

    pTextureSet->EnableRequest(FALSE);

	_InitInput();

	// Del by lark.li 20080611
	// Added by clp
	//FontModule::FontSystem::getSingleton().init();

    if( _Init() == 0 ) 
        return 0;

	ShowWindow(_hWnd, SW_SHOW);
	UpdateWindow(_hWnd);

    return 1;
}

BOOL MPGameApp::LoadResource()
{
	if (!g_Render.InitResource())
	{
		return FALSE;
	}
	return TRUE;
}

BOOL MPGameApp::LoadRes2()
{
	if (!g_Render.InitRes2())
	{
		return FALSE;
	}

	return TRUE;
}

BOOL MPGameApp::LoadRes3()
{
	if (!g_Render.InitRes3())
	{
		return FALSE;
	}
	return TRUE;
}

static DWORD _time;
void MPGameApp::FrameMove(DWORD dwTimeParam)
{
	
    MPTimer tUseTime; tUseTime.Begin();

    _ReadKeyboardInput();
	
	g_Render.UpdateCullInfo() ; //added by billy
	
	if(_pConsole->IsVisible())
	{
		g_Render.EnablePrint(INFO_CMD, TRUE);
		_pConsole->FrameMove();
		UpdateConsoleText(FALSE);
	}
	else
	{
		g_Render.EnablePrint(INFO_CMD, FALSE);
	}

     lwISceneMgr* sm = g_Render.GetInterfaceMgr()->sys_graphics->GetSceneMgr();
     sm->Update();

    _FrameMove(_time = dwTimeParam);

    _dwFrameMoveUseTime = tUseTime.End();
}

void MPGameApp::Render()
{
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

	_Render(); // 

	sm->EndRender();

	///////////////////////////////////////////////////////////

//g_Render.EnableZBuffer(FALSE);
//
//   g_Render.SetCurrentView(MPRender::VIEW_WORLD);

#ifdef DEBUG	
    g_Render.RenderDebugInfo();
#endif

    if(_pConsole->IsVisible())
	{
		MPTexInfo *pTexInfo = GetTextureInfo(_nLogoTexID);
		if(pTexInfo)
		{
			MPTexRect TexRect;
			TexRect.nTexW      = 128;
			TexRect.nTexH      = 64;
			TexRect.fScaleX    = (float)g_Render.GetScrWidth() / 128.0f / 1.76f;
			TexRect.fScaleY    = 2.4f;
			TexRect.dwColor    = 0x2FFFFFFF;
			TexRect.nTextureNo = _nLogoTexID;
			g_Render.RenderTextureRect(0, 0, &TexRect);
		}
	}

	// Del by lark.l i20080611
	// Draw font
	// FontModule::FontSystem::getSingleton().update(0.01f);

	g_Render.EndRender(true);

    _dwRenderUseTime = tRenderUse.End();
}

void MPGameApp::UpdateConsoleText(BOOL bClear)
{
	list<string> *pTextList = _pConsole->GetTextList(); 
    int y = 4; // g_Render.GetScrHeight() - _pConsole->GetHeight() - 40;
    for(list<string>::iterator it = pTextList->begin(); it!=pTextList->end(); it++,y+=16)
    {
        string &str = (*it);
        if(str.size() > 0 || bClear)
        {
            g_Render.Print(INFO_CMD, 2, y, "%s", str.c_str());
        }
    }
	g_Render.Print(INFO_CMD, 4, _pConsole->GetHeight() - 10, "%s", _pConsole->GetInputText());
}

void MPGameApp::_RenderUI()
{
	
}

void MPGameApp::_RenderAxis()
{
	struct AXIS_VERTEX
	{
		D3DVECTOR pos;
		DWORD diffuse;
	};

	D3DXMATRIX mat;
	D3DXMatrixIdentity(&mat);

#if(defined USE_MANAGED_RES)
    g_Render.SetTransformWorld(&mat);
#else
	g_Render.GetDevice()->SetTransform( D3DTS_WORLD , &mat);
#endif


	AXIS_VERTEX pVertices[6];

	// z 
	pVertices[0].pos = D3DXVECTOR3(0.0f , 0.0f , 0.0f);
	pVertices[1].pos = D3DXVECTOR3(0.0f , 0.0f , 50.0f);
	pVertices[0].diffuse = 0xffffffff;
	pVertices[1].diffuse = 0xffffffff;

	// x 
	pVertices[2].pos = D3DXVECTOR3(0.0f ,   0.0f , 0.0f);
	pVertices[3].pos = D3DXVECTOR3(100.0f , 0.0f , 0.0f);
	pVertices[2].diffuse = 0xffff0000;
	pVertices[3].diffuse = 0xffff0000;

	// y 
	pVertices[4].pos = D3DXVECTOR3(0.0f , 0.0f ,   0.0f);
	pVertices[5].pos = D3DXVECTOR3(0.0f , 100.0f , 0.0f);
	pVertices[4].diffuse = 0xFF00ffff;
	pVertices[5].diffuse = 0xFF00ffff;
	
	g_Render.SetRenderState( D3DRS_LIGHTING , FALSE );
	g_Render.SetTexture(0, 0);
	g_Render.SetVertexShader(NULL);
	g_Render.SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
	g_Render.GetDevice()->DrawPrimitiveUP(D3DPT_LINELIST, 3, &pVertices , sizeof(AXIS_VERTEX));
	// _pD3DDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
}

BOOL MPGameApp::_InitInput()
{
	if ( FAILED(DirectInput8Create(_hInst,	DIRECTINPUT_VERSION,
										    IID_IDirectInput8,
										    (VOID**)&_pDI,
										    NULL)))
	{
		ToLogService("init", "Create DirectInput 8 Error!");
		return FALSE;
	}

	 _KeyboardLayout = GetKeyboardLayout(0);	

	if(FAILED(_pDI->CreateDevice(GUID_SysKeyboard, &_pDIKeyboard, NULL)))
	{
		if(_pDI) SAFE_RELEASE(_pDI);
		ToLogService("init", "Create Keyboard Device Error");
		return FALSE;
	}

	DIPROPDWORD		dipdw; 
	
	// Create buffer to hold keyboard data
	ZeroMemory(&dipdw,sizeof(DIPROPDWORD));
	dipdw.diph.dwSize		= sizeof(DIPROPDWORD); 
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER); 
	dipdw.diph.dwObj		= 0; 
	dipdw.diph.dwHow		= DIPH_DEVICE; 
	dipdw.dwData			= KEYBOARD_BUFFERSIZE;	// Buffer Size
	
	HRESULT hr;
	// Set the format of the keyboard
	if (FAILED(hr = _pDIKeyboard->SetDataFormat(&c_dfDIKeyboard)))
		return FALSE; //DisplayErrorMsg(hr, MSGERR_APPMUSTEXIT);

	// Set the co-operative level to exclusive access
	if (FAILED(hr = _pDIKeyboard->SetCooperativeLevel(_hWnd, DISCL_NONEXCLUSIVE|DISCL_FOREGROUND)))
		return FALSE; //DisplayErrorMsg(hr, MSGERR_APPMUSTEXIT);

	// Set the size of the buffer
	if (FAILED(hr = _pDIKeyboard->SetProperty( DIPROP_BUFFERSIZE, &dipdw.diph))) 
		return FALSE; // DisplayErrorMsg(hr, MSGERR_APPMUSTEXIT);
	
	
	
	// Acquire the keyboard device
	_pDIKeyboard->Acquire();

    if ( FAILED( hr = _pDI->CreateDevice( GUID_SysMouse, &_pDIMouse, NULL ) ) )return FALSE;

    if ( FAILED( hr = _pDIMouse->SetDataFormat( &c_dfDIMouse2 ) ) ) return FALSE;

    _pDIMouse->SetCooperativeLevel( _hWnd, DISCL_NONEXCLUSIVE|DISCL_BACKGROUND );

    _pDIMouse->Acquire();

	_nDBClickTime = ::GetDoubleClickTime();
	_nLastClickTime = timeGetTime();

	return TRUE;
}

void	MPGameApp::SetInputActive(bool bActive)
{
	// Added by CLP
	memset ( mASCKeysState, KEY_FREE, sizeof ( mASCKeysState ) );
	memset ( mKeyState, KEY_FREE, sizeof ( mKeyState ) );
	// Added by CLP

	if(!bActive)
	{
		_pDIKeyboard->Unacquire();
		_pDIMouse->Unacquire();
	}else
	{
		_pDIKeyboard->Acquire();
		_pDIMouse->Acquire();
	}
	_bActive = bActive;
}
BOOL MPGameApp::LoadTerrainSet(const char* file, BOOL bBinary)
{
	MPTerrainSet *pTerrainSet = new MPTerrainSet(0, 100);
	//pTerrainSet->LoadRawDataInfo("scripts/table/TerrainInfo", FALSE);
    return pTerrainSet->LoadRawDataInfo(file, bBinary);
}
BOOL MPGameApp::UnloadTerrainSet()
{
    MPTerrainSet *pTerrainSet = MPTerrainSet::I();
    //SAFE_DELETE(pTerrainSet);
    return TRUE;
}
//-----------------------------------------------------------------------------
BOOL MPGameApp::LoadResourceSet(const char* file, int iMaxIndex, BOOL bBinary)
{
//#if RESOURCE_SCRIPT == 2 || RESOURCE_SCRIPT == 3
	MPResourceSet *pResourceSet = new MPResourceSet(0, iMaxIndex);
	return pResourceSet->LoadRawDataInfo(file, bBinary);
//#endif
	return TRUE;
}
//-----------------------------------------------------------------------------
BOOL MPGameApp::UnloadResourceSet()
{
	//MPResourceSet *pResourceSet = MPResourceSet::GetInstancePtr();
	//SAFE_DELETE(pResourceSet);

	// 
	MPResourceSet* pResourceSet = MPResourceSet::GetInstancePtr();
	if( pResourceSet )
	{
		pResourceSet->Release();
		delete pResourceSet;
		MPResourceSet::m_pInstance = NULL;
	}

	return TRUE;
}
//-----------------------------------------------------------------------------

void MPGameApp::_ReadKeyboardInput()
{
	// set the single key strokes to 0 each time, so it just records changes between frames
	ZeroMemory(_btButtonState, sizeof(_btButtonState));			// 0 is no action
	_dwMouseKey = 0;

	if(!_bActive)
		return;

	if(!_CanInput())
		return;

	// Don't read the keyboard if the devices are invalid
	if (!_pDIKeyboard || !_pDI)
	{
		ToLogService("error", "Keyboard interface is NULL!");
		return;
	}

	HRESULT hr = S_OK;

//getdata:

	// GetDeviceData version
	static DIDEVICEOBJECTDATA keyDataBuffer[ KEYBOARD_BUFFERSIZE ];
	DWORD dwItems = KEYBOARD_BUFFERSIZE;

	// Read the buffered data
	hr = _pDIKeyboard->GetDeviceData (
		sizeof ( DIDEVICEOBJECTDATA ), keyDataBuffer, &dwItems, 0 );
	if ( FAILED ( hr ) )
	{
		// Keyboard may have been lost, reacquire it
		_pDIKeyboard->Acquire();
		return;
	}
	
	// Added by CLP
	// Proces the data if there is any
	if ( dwItems )
	{
		// Process the buffered data
		for ( DWORD i = 0; i < dwItems; ++i )
		{
			// Map scan-code to ASCII code
			UINT codeScan = keyDataBuffer[ i ].dwOfs;
			UINT codeASCII = MapVirtualKeyEx( codeScan, 1, _KeyboardLayout );

			if ( keyDataBuffer[ i ].dwData & 0x80 )
			{
				// The key was pressed
				switch ( mKeyState[ codeScan ] )
				{
				case KEY_POP :
				case KEY_FREE :
					mKeyState[ codeScan ] = mASCKeysState[ codeASCII ] = KEY_PUSH;
					HandleKeyDown ( codeScan );
					break;
				}
			}
			else
			{
				// The key was released
				switch ( mKeyState[ codeScan ] )
				{
				case KEY_HOLD :
				case KEY_PUSH :
					mKeyState[ codeScan ] = mASCKeysState[ codeASCII ] = KEY_POP;
					break;
				}
			}
		}
	}
	else
	{
		// There isn't any buffered data, so keys state were not changed.
		for ( DWORD i = 0; i < 256; ++i )
		{
			switch ( mKeyState[ i ] )
			{
			case KEY_PUSH :
				mKeyState[ i ] = mASCKeysState[ i ] = KEY_HOLD;
				break;
			case KEY_POP :
				mKeyState[ i ] = mASCKeysState[ i ] = KEY_FREE;
				break;
			}
		}
	}

/*
	// GetDeviceState version
	memcpy ( _lastKeyState, _curKeyState, sizeof ( _lastKeyState ) );

	// Get the immediate state of the keyboard
	hr = _pDIKeyboard->GetDeviceState ( sizeof ( _curKeyState ), _curKeyState );
	if ( FAILED ( hr ) )
	{
		// Keyboard may have been lost, reacquire it
		_pDIKeyboard->Acquire();
		return;
	}

	for ( size_t i = 0; i < 256; ++i )
	{
		if ( _curKeyState[ i ] & 0x80 && _lastKeyState[ i ] & 0x80 )
		{
			// The key has been hold yet.
			mKeyState[ i ] = keyHold;
			continue;
		} else if ( _curKeyState[ i ] & 0x80 ) {
			// The key was just pressed.
			mKeyState[ i ] = keyDown;
			continue;
		} else if ( _lastKeyState[ i ] & 0x80 ) {
			// The key was just released.
			mKeyState[ i ] = keyUp;
			continue;
		} else {
			// The key has been released yet.
			mKeyState[ i ] = keyReleased;
		}
	}
	// ----- Added by CLP ----- //
*/

	DIMOUSESTATE2 dims2;      
    
    // Get the input's device state, and put the state in dims
    ZeroMemory( &dims2, sizeof(dims2) );
     hr = _pDIMouse->GetDeviceState( sizeof(DIMOUSESTATE2), &dims2 );
    if( FAILED(hr) )
	{
        _pDIMouse->Acquire();
		return;
    }
    
	int nOffsetX = dims2.lX;
	int nOffsetY = dims2.lY;	
	int nScroll  = dims2.lZ; // 

	_btButtonState[0] = dims2.rgbButtons[0] & 0x80;
	_btButtonState[1] = dims2.rgbButtons[1] & 0x80;
	_btButtonState[2] = dims2.rgbButtons[2] & 0x80;

	if(nOffsetX || nOffsetY) //  
	{
		//MouseMove(nOffsetX, nOffsetY);
		_bCanDB = false;

		_dwMouseKey |= M_Move;
	}

	if(nScroll) 
	{
		MouseScroll(nScroll);
	}
	UINT	time = timeGetTime();

	for(int i = 0; i < 3; i++)
	{
		if(_btButtonState[i] && !_btLastButtonState[i]) 
		{
				//MouseButtonDown(i);
				_dwMouseKey |= M_Down;
				if(i == 0)
					_dwMouseKey |= M_LDown;
				else if(i == 1)
					_dwMouseKey |= M_RDown;
				else
					_dwMouseKey |= M_MDown;


				if((time - _nLastClickTime <_nDBClickTime))
				{
					if(!nOffsetX && !nOffsetY && _bCanDB)
					{
						if(_bLastDBClick)
						{
							if(time - _nDBTime > _nDBClickTime)
								_bLastDBClick =false;
						}
						if(!_bLastDBClick)
						{
							//MouseButtonDB(i);
							_bLastDBClick = true;
							_nDBTime = time;

							if(i == 0)
								_dwMouseKey |= M_LDB;
							else if(i == 1)
								_dwMouseKey |= M_RDB;
							else
								_dwMouseKey |= M_MDB;

						}
					}
				}else
				{
					_bLastDBClick = false;
				}
				_nLastClickTime = time;
				_bCanDB = true;
		}
	
		if(!_btButtonState[i] && _btLastButtonState[i]) 
		{
			//MouseButtonUp(i);
			if(i == 0)
				_dwMouseKey |= M_LUp;
			else if(i == 1)
				_dwMouseKey |= M_RUp;
			else
				_dwMouseKey |= M_MUp;

		}
		if(_btButtonState[i] && _btLastButtonState[i]) 
		{
			MouseContinue(i);
		}

		_btLastButtonState[i] = _btButtonState[i];
	}

	_PreMouseRun( _dwMouseKey );

	if(_dwMouseKey & M_Move)
	{
		MouseMove(nOffsetX, nOffsetY);
	}

	if(_dwMouseKey & M_Down)
	{
		if(_dwMouseKey & M_LDown)
			MouseButtonDown(0);
		if(_dwMouseKey & M_RDown)
			MouseButtonDown(1);
		if(_dwMouseKey & M_MDown)
			MouseButtonDown(2);
	}

	if(_dwMouseKey & M_LDB)
		MouseButtonDB(0);
	if(_dwMouseKey & M_RDB)
		MouseButtonDB(1);
	if(_dwMouseKey & M_MDB)
		MouseButtonDB(2);

	if(_dwMouseKey & M_LUp)
		MouseButtonUp(0);
	if(_dwMouseKey & M_RUp)
		MouseButtonUp(1);
	if(_dwMouseKey & M_MUp)
		MouseButtonUp(2);
}

void MPGameApp::SetCaption(const char *pszCaption)
{
	if(_hWnd==0) return;
	SetWindowText(_hWnd, pszCaption);
}

bool MPGameApp::_CanInput()
{
	return true;
    if(::GetFocus() != _hWnd)
		return false;

	POINT  p;
	RECT   rc;
	::GetCursorPos(&p);
	::GetWindowRect(_hWnd,&rc);
	if(!PtInRect(&rc,p))
	{
		return false;
	}
	return true;
}

void MPGameApp::End()
{
    _End();
  
    SAFE_DELETE(_pConsole);
	

	SAFE_RELEASE(_pDIKeyboard);
	SAFE_RELEASE(_pDIMouse);

	SAFE_RELEASE(_pDI);

    g_Render.End();

   	ToLogService("end", "exit game appliaction successful!");
}

void MPGameApp::LG_Config(const LGInfo& info)
{
    _lgInfo = info;
}
