#include "StdAfx.h"
#include "MPRender.h"
#include "MPMath.h"
#include "MPTextureSet.h"
#include "lwIFunc.h"
#include "ShaderLoad.h"
#include "lwD3DSettings.h"
#include "lwPredefinition.h"
#include "d3dutil.h"
#include "MPGameApp.h"

using namespace std;

bool bUsePixelShader = true;

MPRender::MPRender()
:_hWnd(0),
 _pD3D(NULL),
 _pD3DDevice(NULL),
 _p2DSprite(NULL),
 _dwBackgroundColor(0),
 _bClearTarget(true), 
 _bClearZBuffer(true),
 _bClearStencil(false),
 _dwFPS(0),
 _dwLastTick(0),
 _dwFrameCnt(0),
 _bEnableCaptureAVI(FALSE),
 _bCaptureScreen(FALSE)
{
	_nCurViewType = -1;
    SetWorldViewFOV(D3DX_PI /4.0f * 0.90f);
	_dwBackgroundColor = D3DCOLOR_XRGB(128, 128, 128);
	_fNearClip = 1.0f;
	_fFarClip  = 1000.0f;	// Original 1000f

    _d3dCPAdjustInfo.multi_sample_type = D3DMULTISAMPLE_NONE;

	ZeroMemory(&_Light, sizeof(_Light));	
}

MPRender::~MPRender()
{
}

void MPRender::End()
{
	SAFE_RELEASE(_p2DSprite);

	SAFE_DELETE(_pFont);
    ResMgr.ReleaseTotalRes();


    ToLogService("common", "begin release mesh lib");

    lwReleaseMeshLibSystem();

    ToLogService("common", "end release mesh lib");

    // by lsh has been released in mesh lib
	//SAFE_RELEASE(_pD3DDevice);
	//SAFE_RELEASE(_pD3D);
}

BOOL MPRender::Init(HWND hWnd, int nScrWidth, int nScrHeight, int nColorBit, BOOL bFullScreen)
{

	_hWnd = hWnd;

    g_bBinaryTable = TRUE;
    ::GetClientRect(::GetDesktopWindow(), &_rcDeskTop);


    lwD3DCreateParam d3dcp;
    memset(&d3dcp, 0, sizeof(d3dcp));
    d3dcp.hwnd = hWnd;
    d3dcp.adapter = D3DADAPTER_DEFAULT;
    d3dcp.behavior_flag = D3DCREATE_HARDWARE_VERTEXPROCESSING;
    d3dcp.dev_type = D3DDEVTYPE_HAL;

    IDirect3DX* d3d = Direct3DCreateX(D3D_SDK_VERSION);
    D3DDISPLAYMODE d3ddm;
    d3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);
    d3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &_d3dCaps);
 
    d3dcp.present_param.hDeviceWindow = hWnd;
    d3dcp.present_param.Windowed = !bFullScreen;
    d3dcp.present_param.SwapEffect = D3DSWAPEFFECT_DISCARD;//bFullScreen ? D3DSWAPEFFECT_DISCARD : D3DSWAPEFFECT_COPY_VSYNC ;
	// Modified by clp 2
	d3dcp.present_param.BackBufferCount = 2;
    //d3dcp.present_param.BackBufferCount = 1;
    d3dcp.present_param.BackBufferFormat = d3ddm.Format;//D3DFMT_UNKNOWN;
    d3dcp.present_param.BackBufferWidth = nScrWidth;
    d3dcp.present_param.BackBufferHeight = nScrHeight;
	d3dcp.present_param.EnableAutoDepthStencil = 1;

	//if( FAILED( d3d->CheckDeviceFormat( D3DADAPTER_DEFAULT,
	//	D3DDEVTYPE_HAL,
	//	d3ddm.Format,
	//	D3DUSAGE_DEPTHSTENCIL,
	//	D3DRTYPE_SURFACE,
	//	D3DFMT_D24S8 ) ) )
	//{
	//	d3dcp.present_param.AutoDepthStencilFormat = D3DFMT_D16;
	//}
	//else
	//{
	//	d3dcp.present_param.AutoDepthStencilFormat = D3DFMT_D24S8;
	//}

	d3dcp.present_param.AutoDepthStencilFormat = D3DFMT_D16;

	// Modified by clp 
	d3dcp.present_param.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
    // d3dcp.present_param.FullScreen_PresentationInterval = bFullScreen ? D3DPRESENT_INTERVAL_IMMEDIATE : D3DPRESENT_INTERVAL_DEFAULT;
    // vsd3d
    if(_d3dCaps.VertexShaderVersion < D3DVS_VERSION(1,0))
    {
        d3dcp.behavior_flag = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
		d3dcp.present_param.SwapEffect = bFullScreen ? D3DSWAPEFFECT_DISCARD : D3DSWAPEFFECT_COPY;
		d3dcp.present_param.BackBufferCount = 1; //

    }

	//	add by	jze	begin!
	bUsePixelShader	=	_d3dCaps.PixelShaderVersion >= D3DPS_VERSION(1,4) ? true : false;
	//	add	by	jze	end!

    if(bFullScreen)
    {
        //d3dcp.present_param.FullScreen_RefreshRateInHz = 85;
    }

    d3dcp.behavior_flag |= D3DCREATE_MULTITHREADED;
    d3d->Release();


	// Init Mesh Lib
    LW_RESULT ret;
	lwISystem* sys;
    lwISysGraphics* sys_graphics;
    if(LW_FAILED(ret = lwInitMeshLibSystem(&sys, &sys_graphics, &d3dcp, &_d3dCPAdjustInfo)))
    {
        char err_str[260];

        switch(ret)
        {
        case INIT_ERR_CREATE_D3D:
            _tcscpy(err_str, "Create DirectX error");
            break;
        case INIT_ERR_CREATE_DEVICE:
            ToLogService("common", LogLevel::Error, "");
            _tcscpy(err_str, "Create DirectX Device error");
            break;
        case INIT_ERR_DX_VERSION:
            _tcscpy(err_str, "Invalid Installed Directx Version");
            break;
        default:
            _tcscpy(err_str, "Unknown Internal Error");

        }
        ToLogService("common", LogLevel::Error, "{}", err_str);
        return FALSE;
    }


    lwIDeviceObject* dev_obj = sys_graphics->GetDeviceObject();
    _pD3D = dev_obj->GetDirect3D();
    _pD3DDevice = dev_obj->GetDevice();
    _IMgr.sys = sys;
    _IMgr.sys_graphics = sys_graphics;
    _IMgr.dev_obj = sys_graphics->GetDeviceObject();
    _IMgr.res_mgr = sys_graphics->GetResourceMgr();
    _IMgr.tp_loadres = _IMgr.res_mgr->GetThreadPoolMgr()->GetThreadPool(THREAD_POOL_LOADRES);

    LoadShader0(sys_graphics);
    LoadShader1(sys_graphics);

    ToggleFullScreen();

    ResMgr.m_pSys = sys;
	ResMgr.m_pSysGraphics = sys_graphics;
	ResMgr.LoadTotalVShader(sys_graphics);

	D3DXCreateSprite(_pD3DDevice, &_p2DSprite);
	D3DUtil_InitLight( _Light, D3DLIGHT_DIRECTIONAL, -1.0f, -1.0f, -1.0f );
    SetDirectLIghtAmbient(0.0f, 0.0f, 0.0f, 0.0f);
	SetLight(0, &_Light);
	LightEnable(0, TRUE);

	if(!_LoadStateFromFile())
	{
		return FALSE;
	}

    // begin by lsh
    D3DXVECTOR3	up = D3DXVECTOR3( 0.0f, 0.0f, 1.0f );
    D3DXVECTOR3	eye = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
    D3DXVECTOR3	target = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	D3DXMatrixLookAtLH( &_mat3DUIView, &eye, &target, &up );	  

    _fAspect = (float)_nWorldViewWidth / ((float)(_nWorldViewHeight));
    D3DXMatrixPerspectiveFovLH(&_mat3DUIProj, D3DX_PI * 0.12f, _fAspect, _fNearClip, _fFarClip);
    SetRenderState( D3DRS_ZENABLE  , TRUE );
	SetRenderState( D3DRS_AMBIENT, 0xffffffff );
	SetRenderState(D3DRS_CULLMODE , D3DCULL_NONE);

    if(InitMPTextureSetFormat() == 0)
        return FALSE;

	return TRUE;
}
//-----------------------------------------------------------------------------
BOOL MPRender::InitResource()
{
#ifdef USE_RENDER
	if(!ResMgr.InitRes(this,(D3DXMATRIX*)_IMgr.dev_obj->GetMatView(),(D3DXMATRIX*)_IMgr.dev_obj->GetMatViewProj()))
#else
	if(!ResMgr.InitRes(_pD3DDevice,&GetWorldViewMatrix(),&GetViewProjMatrix()))
#endif
	{
		ToLogService("errors", LogLevel::Error, "ResMgr,!");
		return FALSE;
	}
	return TRUE;
}

BOOL MPRender::InitRes2()
{
	if(!ResMgr.InitRes2())
	{
		ToLogService("errors", LogLevel::Error, "ResMgr 2,!");
		return FALSE;
	}
	return TRUE;
}

BOOL MPRender::InitRes3()
{
	if(!ResMgr.InitRes3())
	{
		ToLogService("errors", LogLevel::Error, "ResMgr 3,!");
		return FALSE;
	}
	return TRUE;
}

//-----------------------------------------------------------------------------


int MPRender::ToggleFullScreen(int width, int height, D3DFORMAT depth_fmt, BOOL be_windowed)
{
	lwIResourceMgr* res_mgr = _IMgr.res_mgr;
	lwIDeviceObject* dev_obj = _IMgr.dev_obj;
    IDirect3DX* dev = dev_obj->GetDirect3D();

	HWND hwnd = _hWnd;

	RECT wnd_rc = { 0, 0, width, height };
	DWORD style;

	if(be_windowed)
	{
		if(wnd_rc.right == _rcDeskTop.right && wnd_rc.bottom == _rcDeskTop.bottom)
		{
			style = WS_POPUP;
		}
		else
		{
			style = WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX|WS_VISIBLE;
			::AdjustWindowRectEx(&wnd_rc, style, 0, 0);
		}
	}
    else
	{
        style = WS_POPUP;
	}

	lwWndInfo wnd_info;
	wnd_info.hwnd = hwnd;
	wnd_info.left = 0;
	wnd_info.top = 0;
	wnd_info.width =  wnd_rc.right - wnd_rc.left;
	wnd_info.height = wnd_rc.bottom - wnd_rc.top;
	wnd_info.windowed_style = style;

    lwD3DCreateParam d3dcp = *dev_obj->GetD3DCreateParam();
    d3dcp.present_param.Windowed = be_windowed;
    //d3dcp.present_param.BackBufferFormat = back_fmt;
    d3dcp.present_param.BackBufferWidth = width;
    d3dcp.present_param.BackBufferHeight = height;
    d3dcp.present_param.AutoDepthStencilFormat = depth_fmt;


    if(LW_FAILED(lwAdjustD3DCreateParam(dev, &d3dcp, &_d3dCPAdjustInfo)))
    {
		ToLogService("errors", LogLevel::Error, "ToggleFullScreen error");
        return 0;
    }

    if(ToggleFullScreen(&d3dcp.present_param, &wnd_info) == 0)
	{
		ToLogService("errors", LogLevel::Error, "ToggleFullScreen error");
		return 0;
	}

    return 1;

}
int MPRender::ToggleFullScreen(D3DPRESENT_PARAMETERS* d3dpp, lwWndInfo* wnd_info)
{
    SAFE_RELEASE(_p2DSprite);
    if(LW_FAILED(_IMgr.sys_graphics->ToggleFullScreen(d3dpp, wnd_info)))
        return 0;

    D3DXCreateSprite(_pD3DDevice, &_p2DSprite);
    return ToggleFullScreen();
}
int MPRender::ToggleFullScreen()
{
    lwD3DCreateParam* d3dcp = _IMgr.dev_obj->GetD3DCreateParam();

    _bFullScreen    = !d3dcp->present_param.Windowed;
	//char ws[16];
	//sprintf( ws, "fullscreen %d",(int)_bFullScreen);
	// OutputDebugString( ws );
	
    RECT rc;
    GetClientRect(d3dcp->present_param.hDeviceWindow, &rc);
    _nScrWidth  = rc.right  - rc.left;
    _nScrHeight = rc.bottom - rc.top;

    SetWorldView(0, 0, _nScrWidth, _nScrHeight);

    SetViewport(0, 0, d3dcp->present_param.BackBufferWidth, d3dcp->present_param.BackBufferHeight);

    //LightEnable(0, TRUE);
    //UpdateLight();

    if(InitMPTextureSetFormat() == 0)
        return 0;

    return 1;
}

int MPRender::InitMPTextureSetFormat()
{
    // Форматы текстур для TextureManager:
    //   _TexSetFmt[0] — без альфы (сцена/мир, .tga без альфа-канала)
    //   _TexSetFmt[1] — с альфой (UI, .tga с альфой)
    //
    // На всех современных GPU A8R8G8B8/X8R8G8B8 поддерживаются нативно.
    // Раньше был fallback на A4R4G4B4/R5G6B5 для старых видеокарт —
    // удалён, так как 16-bit форматы дают ступенчатую альфу и, как правило,
    // всё равно эмулируются через 32-bit драйвером.
    if(LW_FAILED(_IMgr.dev_obj->CheckCurrentDeviceFormat(BBFI_TEXTURE, D3DFMT_A8R8G8B8)))
    {
        return 0;
    }
    if(LW_FAILED(_IMgr.dev_obj->CheckCurrentDeviceFormat(BBFI_TEXTURE, D3DFMT_X8R8G8B8)))
    {
        return 0;
    }

    _TexSetFmt[0] = D3DFMT_X8R8G8B8; // 32-bit без альфы
    _TexSetFmt[1] = D3DFMT_A8R8G8B8; // 32-bit с альфой
    return 1;
}

void MPRender::SetViewport(int nStartX, int nStartY, int nWidth, int nHeight)
{
    _view.X	     = nStartX;
	_view.Y	     = nStartY;
	_view.Width  = nWidth;
	_view.Height = nHeight;
	_view.MinZ	 = 0.0f;
	_view.MaxZ	 = 1.0f;
    HRESULT hr = _pD3DDevice->SetViewport(&_view);
	if (FAILED(hr))
	{
		ToLogService("common", "Error when SetViewport(), [{}].", hr);
	}
    // LG("render", "Set View Port [x = %d, y = %d , w = %d, h = %d\n", nStartX, nStartY, nWidth, nHeight);
}

BOOL MPRender::IsLineInWorldView(float fX1, float fY1, float fX2, float fY2)
{
    float v1[2] = { fX1, fY1 };
    float v2[2] = { fX2, fY2 };
    float v0[2];
    float x1 = (float)_nWorldViewStartX;
    float y1 = (float)_nWorldViewStartY;
    float x2 = (float)(_nWorldViewStartX + _nWorldViewWidth);
    float y2 = (float)(_nWorldViewStartY + _nWorldViewHeight);
    for(int i = 0; i < 4; i++)
    {
        float v3[2], v4[2];
        switch(i)
        {
            case 0:
            {   
                v3[0] = x1; v3[1] = y1;
                v4[0] = x2; v4[1] = y1;
                break;
            }
            case 1:
            {   
                v3[0] = x2; v3[1] = y1;
                v4[0] = x2; v4[1] = y2;
                break;
            }
            case 2:
            {
                v3[0] = x1; v3[1] = y2;
                v4[0] = x2; v4[1] = y2;
                break;
            }
            case 3:
            {
                v3[0] = x1; v3[1] = y1;
                v4[0] = x1; v4[1] = y2;
                break;
            }
        }
        if(Get2DLineIntersection(v1, v2, v3, v4, v0, FALSE)) return TRUE;
    }
    return FALSE;
}

BOOL MPRender::IsRectIntersectWorldView(int *pnPosX, int *pnPosY)
{
    int x1, y1, x2, y2;
    for(int i = 0; i < 4; i++)
    {
        switch(i)
        {
            case 0: { x1 = *(pnPosX + 0); y1 = *(pnPosY + 0); x2 = *(pnPosX + 1); y2 = *(pnPosY + 1); break; }
            case 1: { x1 = *(pnPosX + 1); y1 = *(pnPosY + 1); x2 = *(pnPosX + 2); y2 = *(pnPosY + 2); break; }
            case 2: { x1 = *(pnPosX + 2); y1 = *(pnPosY + 2); x2 = *(pnPosX + 3); y2 = *(pnPosY + 3); break; }
            case 3: { x1 = *(pnPosX + 3); y1 = *(pnPosY + 3); x2 = *(pnPosX + 0); y2 = *(pnPosY + 0); break; }
        }
        if(IsLineInWorldView((float)x1, (float)y1, (float)x2, (float)y2))
        {
            return TRUE;
        }
    }
    return FALSE;
}

void MPRender::SetCurrentView(int nType, BOOL bReset)
{
	if(nType==_nCurViewType && !bReset) return;


	
    switch(nType)
	{
	    case VIEW_WORLD:
        {
			D3DXMatrixIdentity(&_matProjWorld);
			D3DXMatrixPerspectiveFovLH(&_matProjWorld, _fWorldViewFOV, _fAspect, _fNearClip, _fFarClip);
			

#if(defined USE_MANAGED_RES)
            SetTransformProj(&_matProjWorld);
#else
			_pD3DDevice->SetTransform( D3DTS_PROJECTION, (D3DMATRIX*)&_matProjWorld );
#endif
            _matViewProj = _matViewWorld * _matProjWorld;
			break;
        }
	 case VIEW_UI:
        {
            D3DXMatrixIdentity(&_matUIView);
			D3DXMatrixIdentity(&_matUIProj);

			D3DXMatrixPerspectiveFovLH(&_matUIProj, 0.1f, _fAspect, 10, 100);


#if(defined USE_MANAGED_RES)
            SetTransformProj(&_matUIProj);
#else
            _pD3DDevice->SetTransform( D3DTS_PROJECTION, (D3DMATRIX*)&_matUIProj );
#endif

	        D3DXVECTOR3	upVector = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );	
	        D3DXVECTOR3	vecCam(0, 0, -50);
	        D3DXVECTOR3	vecLookAt(0, 0, 0);
	        D3DXMatrixLookAtLH( &_matUIView, &vecCam, &vecLookAt, &upVector );
#if(defined USE_MANAGED_RES)
            SetTransformView(&_matUIView);
#else
	        _pD3DDevice->SetTransform( D3DTS_VIEW, &_matUIView );
#endif
			break;
        }
        case VIEW_SCREEN:
            break;
        case VIEW_3DUI:
#if(defined USE_MANAGED_RES)
            SetTransformProj(&_mat3DUIProj);
            SetTransformView(&_mat3DUIView);
#else
            _pD3DDevice->SetTransform( D3DTS_PROJECTION, (D3DMATRIX*)&_mat3DUIProj );
            _pD3DDevice->SetTransform( D3DTS_VIEW, &_mat3DUIView ); 
#endif
            _matViewProj = _mat3DUIView * _mat3DUIProj;
            break;
        default:
        {
            break;
        }
    }
}

/*
void MPRender::SetCurrentView(int nType, BOOL bReset)
{
	if(nType==_nCurViewType && !bReset) return;

    //_nCurViewType = nType;

	switch(nType)
	{
	    case VIEW_WORLD:
        {
            D3DXMatrixIdentity(&_matProjWorld);
            // SetViewport(_nWorldViewStartX, _nWorldViewStartY, _nWorldViewWidth, _nWorldViewHeight);
            //float fAspect = (float)_nWorldViewWidth / ((float)(_nWorldViewHeight));
	        D3DXMatrixPerspectiveFovLH(&_matProjWorld, _fWorldViewFOV, _fAspect, _fNearClip, _fFarClip);

#if(defined USE_MANAGED_RES)
            SetTransformProj(&_matProjWorld);
#else
            _pD3DDevice->SetTransform( D3DTS_PROJECTION, (D3DMATRIX*)&_matProjWorld);
#endif
            _matViewProj = _matViewWorld * _matProjWorld;
			break;
        }
        case VIEW_UI:
        {
            D3DXMatrixIdentity(&_matUIView);
			D3DXMatrixIdentity(&_matUIProj);

			//SetViewport(0, 0, _nScrWidth, _nScrHeight);
            //FLOAT fAspect = (float)_nScrWidth / (float)_nScrHeight;
            D3DXMatrixPerspectiveFovLH(&_matUIProj, 0.1f, _fAspect, 10, 100);

#if(defined USE_MANAGED_RES)
            SetTransformProj(&_matUIProj);
#else
            _pD3DDevice->SetTransform( D3DTS_PROJECTION, (D3DMATRIX*)&_matUIProj );
#endif

	        D3DXVECTOR3	upVector = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );	
	        D3DXVECTOR3	vecCam(0, 0, -50);
	        D3DXVECTOR3	vecLookAt(0, 0, 0);
	        D3DXMatrixLookAtLH( &_matUIView, &vecCam, &vecLookAt, &upVector );
#if(defined USE_MANAGED_RES)
            SetTransformView(&_matUIView);
#else
	        _pD3DDevice->SetTransform( D3DTS_VIEW, &_matUIView );
#endif
			break;
        }
        case VIEW_SCREEN:
            break;
        case VIEW_3DUI:
#if(defined USE_MANAGED_RES)
            SetTransformProj(&_mat3DUIProj);
            SetTransformView(&_mat3DUIView);
#else
            _pD3DDevice->SetTransform( D3DTS_PROJECTION, (D3DMATRIX*)&_mat3DUIProj );
            _pD3DDevice->SetTransform( D3DTS_VIEW, &_mat3DUIView ); 
#endif
            _matViewProj = _mat3DUIView * _mat3DUIProj;
            break;
        default:
        {
            //SetViewport(0, 0, _nScrWidth, _nScrHeight);
            break;
        }
    }
}
*/
////////////////////////////////////////////////////////////////
// VIM END


//transform 3d world position into screen space
void MPRender::GetScreenPos(int &nOutX, int &nOutY, D3DXVECTOR3 &vWorldPos)
{
	D3DXVECTOR4 vOut;
	D3DXVec3Transform(&vOut, &vWorldPos, &_matViewProj);
	nOutX = _nWorldViewStartX + int((float)_nWorldViewWidth * (vOut.x / vOut.w*0.5f+0.5f));
	nOutY = _nWorldViewStartY + int(((float)(_nWorldViewHeight))*(0.5f - vOut.y / vOut.w * 0.5f));
}


void MPRender::GetRay(int nScreenX, int nScreenY, D3DXVECTOR3 &vRayStart, D3DXVECTOR3 &vRayEnd)
{
	D3DXMATRIX matInverse;

	D3DXMatrixInverse(&matInverse, NULL, &_matViewProj);

	D3DXVECTOR4 vOut;
	D3DXVECTOR3 v;
	v.x = float(nScreenX - _nWorldViewStartX) / (float)_nWorldViewWidth - 0.5f;
	v.x+= v.x;
	v.y = 0.5f - float(nScreenY - _nWorldViewStartY)/((float)(_nWorldViewHeight));
	v.y+= v.y;
	v.z = 0.0f;
	D3DXVec3Transform(&vOut, &v, &matInverse);
	vRayStart = D3DXVECTOR3(vOut.x/vOut.w, vOut.y/vOut.w, vOut.z/vOut.w);
	v.z = 1.0f;
	D3DXVec3Transform(&vOut,&v, &matInverse);
	vRayEnd = D3DXVECTOR3(vOut.x/vOut.w, vOut.y/vOut.w, vOut.z/vOut.w);
}

void MPRender::GetPickRayVector(int nScrPosX, int nScrPosY, D3DXVECTOR3* pPickRayOrig, D3DXVECTOR3* pPickRayDir)
{	
	GetRay(nScrPosX, nScrPosY, *pPickRayOrig, *pPickRayDir);
	(*pPickRayDir) = (*pPickRayDir) - (*pPickRayOrig);
	D3DXVec3Normalize(pPickRayDir, pPickRayDir);
}

void MPRender::LookAt(D3DXVECTOR3 vecPos, D3DXVECTOR3 vecLookAt,DWORD dwViewType)
{
    D3DXMATRIX* mat = NULL;

	switch( dwViewType)
	{
    case VIEW_WORLD:
        mat = &_matViewWorld;
        break;
    case VIEW_3DUI:
        mat = &_mat3DUIView;
        break;
    default:
        return;
    }

    D3DXVECTOR3	upVector = D3DXVECTOR3( 0.0f, 0.0f, 1.0f );
	D3DXMatrixLookAtLH( mat, &vecPos, &vecLookAt, &upVector );	

#if(defined USE_MANAGED_RES)
    SetTransformView(mat);
#else
    _pD3DDevice->SetTransform( D3DTS_VIEW, mat );
#endif


	//D3DXMATRIX matProj;
    //D3DXMatrixPerspectiveFovLH( &_matProjWorld, D3DX_PI/4, 1.0f, 1.0f, 1000.0f );
    //_pD3DDevice->SetTransform( D3DTS_PROJECTION, &_matProjWorld );

	// _matViewProj = _matViewWorld * _matProjWorld;
}

BOOL MPRender::BeginRender(bool clear)//vim
{

	HRESULT hr;
    if(FAILED(hr = _IMgr.sys_graphics->TestCooperativeLevel()))
    {
        return 0;
    }
    if(hr == LW_RET_OK_1)
    {
        LightEnable(0, TRUE);
        UpdateLight();
    }

#if 0
	HRESULT hr;
    if( FAILED( hr = _pD3DDevice->TestCooperativeLevel() ) ) // ...
    {
        // If the device was lost, do not render until we get it back
        if (D3DERR_DEVICELOST == hr) return FALSE;
		
        if (D3DERR_DEVICENOTRESET == hr)
        {
			return FALSE;
			// if (FAILED(_pD3DDevice->Reset(&g_pDevice->m_d3dpp))) return false;
		}
    }
#endif
    
    _dwClearFlag = 0;
    if(_bClearTarget)	_dwClearFlag|=D3DCLEAR_TARGET;
    if(_bClearZBuffer)  _dwClearFlag|=D3DCLEAR_ZBUFFER;
    if(_bClearStencil)  _dwClearFlag|=D3DCLEAR_STENCIL;
    
    
    if(clear)//vim
	{
		if(FAILED( _pD3DDevice->Clear( 0L, NULL, _dwClearFlag, _dwBackgroundColor, 1.0f, 0L ) ) )
		{
			ToLogService("errors", LogLevel::Error, "D3D Device Clear Failed!");
			return false;
		}
	}
    
	if ( FAILED( _pD3DDevice->BeginScene() ) ) 
    {
		return false;
	}
	
	return true;
}

void MPRender::EndRender(const bool present) // vim
{
	if (FAILED(_pD3DDevice->EndScene())) {
		ToLogService("errors", LogLevel::Error, "D3D End Scene Fail!");
		return;
	}

	if(present)
	{
		DWORD dwTick = GetTickCount();
		if((dwTick - _dwLastTick) >= 1000)
		{
			_dwFPS      = _dwFrameCnt;
			_dwFrameCnt = 0;
			_dwLastTick = dwTick;

			Print(INFO_FPS, 5, 5, "FPS : %d", _dwFPS);
		}

		_dwFrameCnt++;

		static int g_nAviCnt = 0;
		static int g_nCapCnt = 0;

		if(_bCaptureScreen) // 
		{
			static int g_nScreenCap = 0;
			char fileName[64];
			Util_MakeDir("screenshot\\");

			char pszName[64];

			/*int nidx = 0;
			while(1)
			{
				sprintf(pszName,"screenshot\\%d\\",nidx);
				if(_access(pszName,0)== -1)
				{
					Util_MakeDir(pszName);
					break;
				}
				nidx++;
			}*/
			struct tm *newtime;
			__int64 ltime;
			char buff[80];
			_time64( &ltime );
			newtime = _gmtime64( &ltime );
			sprintf(pszName, "screenshot/%04d-%02d-%02d/",newtime->tm_year+1900,newtime->tm_mon+1,newtime->tm_mday);
			Util_MakeDir(pszName);
			
			sprintf(fileName,"%scap%05d.png",pszName, g_nScreenCap);
			g_Render.CaptureScreen(fileName);
			g_nScreenCap++;
			_bCaptureScreen = FALSE;
		}

		if (_bEnableCaptureAVI) // 
		{
			static int g_nAviCnt = 0;
			char szFileName[64];
			Util_MakeDir("screenshot/");
			_snprintf_s(szFileName, _TRUNCATE, "avi%06d.bmp", g_nAviCnt);
			CaptureScreen(szFileName);
			g_nAviCnt++;
		}

		if (_bFullScreen || true) {
			_pD3DDevice->Present(nullptr, nullptr, nullptr, nullptr);
		}
		else {
			RECT rc{};
			rc.left = 0;
			rc.top = 0;
			rc.right = this->_nScrWidth;
			rc.bottom = this->_nScrHeight;
			_pD3DDevice->Present(&rc, nullptr, nullptr, nullptr);
		}
	}
}


void MPRender::RenderAllLines()
{
	for(vector<MPLine*>::iterator it = _LineList.begin(); it!=_LineList.end(); it++)
	{
		MPLine *pLine = (*it);
		RenderLine(pLine->v1.x, pLine->v1.y, pLine->v1.z, pLine->v2.x, pLine->v2.y, pLine->v2.z, pLine->dwColor);
		delete pLine;
	}
	_LineList.clear();
}


void MPRender::RenderDebugInfo()
{
    for(int i = 0; i < MAX_INFO_TYPE; i++)
    {
        if(!_PrintInfoMask[i]) continue;
        for(map<int, string>::iterator it = _InfoIdx[i].begin(); it!=_InfoIdx[i].end(); it++)
        {
            int  nIdx = (*it).first;
            string &str = (*it).second;
            int nPosY = nIdx / 2000;
            int nPosX = nIdx % 2000;
			_pFont->DrawText((TCHAR*)str.c_str(),nPosX, nPosY, 0xffffffff);
    	}
	}
}

void MPRender::Print(int nInfoType, int x, int y, const char *szFormat, ...)
{           
    va_list list;
	va_start(list, szFormat);           
	vsprintf(_szInfo, szFormat, list);
	va_end( list );

    int nIdx = y * 2000 + x; // 
    _InfoIdx[nInfoType][nIdx] = _szInfo;
}

// Render States Routines
void MPRender::EnableMipmap(BOOL bEnable)
{
    if(bEnable)
    {
        SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR );
    }
    else // mipmap
    {
		SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE );
    }
}

void MPRender::RenderTextureRect(int nX, int nY, MPTexRect *pRect)
{
	VECTOR3 vecDest((float)nX, (float)nY, 0.0f);
	LPTEXTURE pTexture = GetTextureByID(pRect->nTextureNo);
	if(!pTexture) return;
	
	RECT *pTexRect = NULL;
	RECT TexRect = { pRect->nTexSX, pRect->nTexSY, pRect->nTexSX + pRect->nTexW, pRect->nTexSY + pRect->nTexH };
	if(pRect->nTexW!=0) pTexRect = &TexRect;
	// _p2DSprite->Draw(NULL, NULL, NULL , NULL, 0, &vecDest, pRect->dwColor);
	_p2DSprite->Begin(D3DXSPRITE_ALPHABLEND);
	D3DXMATRIX scaleMat;
	D3DXMatrixIdentity(&scaleMat);
	scaleMat = *D3DXMatrixScaling(&scaleMat, pRect->fScaleX, pRect->fScaleY, 1.0f);
	_p2DSprite->SetTransform(&scaleMat);
	_p2DSprite->Draw(pTexture, pTexRect, NULL, &vecDest, pRect->dwColor);
	_p2DSprite->End();
}

void MPRender::RenderLine(float x1, float y1, float z1, float x2, float y2, float z2, DWORD dwColor)
{
    D3DXMATRIXA16 mat;
	D3DXMatrixIdentity(&mat);
	
	D3DXMatrixTranslation(&mat, 0.0f, 0.0f, 0.0f);
#if(defined USE_MANAGED_RES)
    SetTransformWorld(&mat);
#else
	_pD3DDevice->SetTransform(D3DTS_WORLD, &mat);
#endif

	struct TMP_VERTEX
    {
	    D3DVECTOR pos;
	    DWORD diffuse;
    };

    TMP_VERTEX pVertices[2];

	// line 1
	pVertices[0].pos = D3DXVECTOR3(x1 , y1 , z1);
	pVertices[1].pos = D3DXVECTOR3(x2 , y2 , z2);
	pVertices[0].diffuse = dwColor;
	pVertices[1].diffuse = dwColor;

	SetRenderState( D3DRS_LIGHTING, FALSE );
	SetTexture(0, 0);
	SetVertexShader(NULL);
	SetFVF(D3DFVF_XYZ|D3DFVF_DIFFUSE);
	_pD3DDevice->DrawPrimitiveUP(D3DPT_LINELIST, 1 , pVertices , sizeof(TMP_VERTEX));
	SetRenderState( D3DRS_LIGHTING, TRUE );
	//SetRenderState( D3DRS_ZENABLE,  TRUE );
}

void MPRender::AddLine(const D3DXVECTOR3 &v1, const D3DXVECTOR3& v2,DWORD dwColor)
{
	MPLine *pLine = new MPLine;
	pLine->v1		= v1;
	pLine->v2		= v2;
	pLine->dwColor	= dwColor;

	_LineList.push_back(pLine);
}

void MPRender::_AddText(const char *pszText, DWORD dwColor)
{

}

MPRender g_Render;
int g_nTemp = 0;

/************************************************************************/
/* state*/
/************************************************************************/
bool	MPRender::_LoadStateFromFile()
{
	return true;
}
void	MPRender::BeginState(int iIdx)
{
}
void	MPRender::EndState()
{
}

///////////////////////////////////////////////
////////rework screenshot to remove asm and use smart pointer @mothannakh///////////
bool SurfaceToBMP(IDirect3DSurfaceX* pSurface, const char* strName) {
	D3DSURFACE_DESC Desc;
	D3DLOCKED_RECT Rect;
	if (!pSurface) {
		ToLogService("common", "Invalid surface pointer");
		return false;
	}
	pSurface->GetDesc(&Desc);

	const int nPitch = (Desc.Width * 3 + 3) & ~0x3;
	if (pSurface->LockRect(&Rect, nullptr, D3DLOCK_READONLY) != D3D_OK) {
		ToLogService("common", "Failed to lock surface rect");
		return false;
	}
	//somehow we need wider buffer if screen reso is 2k so we add buffer *4
	const auto pbtTmp = std::make_unique<BYTE[]>(sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + nPitch * Desc.Height * 4);
	BYTE* pbtTmpRaw = pbtTmp.get();

	BITMAPFILEHEADER bithead;
	BITMAPINFOHEADER bitinfo;

	std::ofstream file(strName, std::ios::binary);
	if (!file) {
		ToLogService("common", "Failed to open file");
		pSurface->UnlockRect();
		return false;
	}

	constexpr int headsize = sizeof(bithead) + sizeof(bitinfo);
	bithead.bfType = 0x4D42; // 'BM' in little-endian order
	bithead.bfSize = headsize + nPitch * Desc.Height;
	bithead.bfReserved1 = bithead.bfReserved2 = 0;
	bithead.bfOffBits = headsize;
	bitinfo.biSize = sizeof(bitinfo);
	bitinfo.biWidth = Desc.Width;
	bitinfo.biHeight = Desc.Height;
	bitinfo.biPlanes = 1;
	bitinfo.biBitCount = 24;
	bitinfo.biCompression = BI_RGB;
	bitinfo.biSizeImage = 0;
	//we use 72 dpi screenshot if want use 300 use:
	//bitinfo.biXPelsPerMeter = 11811;
	//bitinfo.biYPelsPerMeter = 11811;
	bitinfo.biXPelsPerMeter = 72;
	bitinfo.biYPelsPerMeter = 72;
	bitinfo.biClrUsed = 0;
	bitinfo.biClrImportant = 0;

	file.write(reinterpret_cast<char*>(&bithead), sizeof(BITMAPFILEHEADER));
	file.write(reinterpret_cast<char*>(&bitinfo), sizeof(BITMAPINFOHEADER));

	const auto pSrc = static_cast<BYTE*>(Rect.pBits);
	for (int y = 0; y < Desc.Height; ++y) {
		for (int x = 0; x < Desc.Width; ++x) {
			const BYTE* pPixel = pSrc + y * Rect.Pitch + x * 4;
			const int dstIndex = (Desc.Height - 1 - y) * nPitch + x * 3;
			if (dstIndex + 2 >= sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + nPitch * Desc.Height * 4) {
				ToLogService("common", "Out of bounds write detected at y= {} x={}", y, x);
				pSurface->UnlockRect();
				file.close();
				return false;
			}
			pbtTmpRaw[dstIndex + 0] = pPixel[0]; // Blue channel
			pbtTmpRaw[dstIndex + 1] = pPixel[1]; // Green channel
			pbtTmpRaw[dstIndex + 2] = pPixel[2]; // Red channel
		}
	}

	file.write(reinterpret_cast<char*>(pbtTmpRaw), static_cast<std::streamsize>(Desc.Height) * nPitch);
	file.close();

	pSurface->UnlockRect();
	return true;
}

void MPRender::CaptureScreen(const char* szFilename) const {
	D3DSURFACE_DESC sDesc;
	IDirect3DSurfaceX* pBackBuffer;
	_pD3DDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);
	pBackBuffer->GetDesc(&sDesc);

	IDirect3DSurfaceX* pCapSurface;
	_pD3DDevice->CreateOffscreenPlainSurface(sDesc.Width, sDesc.Height, sDesc.Format, D3DPOOL_SYSTEMMEM, &pCapSurface, nullptr);
	_pD3DDevice->GetRenderTargetData(pBackBuffer, pCapSurface);
	SurfaceToBMP(pCapSurface, szFilename);

	pBackBuffer->Release();
	pCapSurface->Release();
}

void MPRender::IgnoreModelTexture(BOOL bIgnore)
{
     lwHelperSetForceIgnoreTexFlag(bIgnore);
}

BOOL MPRender::WorldToScreen(float fX, float fY, float fZ, int* pnX, int* pnY)
{
	D3DXVECTOR3 vTemp, vPosTrans, vecPos;
	vecPos.x = fX;
	vecPos.y = fY;
	vecPos.z = fZ;
	D3DXVec3TransformCoord(&vTemp, &vecPos, &_matViewWorld);
	D3DXVec3TransformCoord(&vPosTrans, &vTemp, &_matProjWorld);
	if (vPosTrans.z >= 0.0f && vPosTrans.z < 1.0f)
	{
		int nPosX = (INT)((vPosTrans.x + 1) * (float)(_nWorldViewWidth) / 2.0f + 0.5f);
		int nPosY = (INT)((-vPosTrans.y + 1) * (float)(_nWorldViewHeight) / 2.0f + 0.5f);

		if (pnX)
		{
			*pnX = nPosX - _nWorldViewStartX;
		}
		if (pnY)
		{
			*pnY = nPosY - _nWorldViewStartY;
		}

		if (nPosX >= 0 && nPosX < _nWorldViewWidth && nPosY >= 0 && nPosY < _nWorldViewHeight)
		{
			return TRUE;
		}
	}
	return FALSE;
}

void MPRender::UpdateCullInfo()
{
	D3DXMATRIX mat;
	D3DXMatrixMultiply(&mat, &_matViewWorld, &_matProjWorld);
	D3DXMatrixInverse(&mat, NULL, &mat);

	_CullInfo.vecFrustum[0] = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
	_CullInfo.vecFrustum[1] = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
	_CullInfo.vecFrustum[2] = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
	_CullInfo.vecFrustum[3] = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	_CullInfo.vecFrustum[4] = D3DXVECTOR3(-1.0f, -1.0f, 1.0f);
	_CullInfo.vecFrustum[5] = D3DXVECTOR3(1.0f, -1.0f, 1.0f);
	_CullInfo.vecFrustum[6] = D3DXVECTOR3(-1.0f, 1.0f, 1.0f);
	_CullInfo.vecFrustum[7] = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	for (INT i = 0; i < 8; i++)
	{
		D3DXVec3TransformCoord(&_CullInfo.vecFrustum[i], &_CullInfo.vecFrustum[i], &mat);
	}

	D3DXPlaneFromPoints(&_CullInfo.planeFrustum[0], &_CullInfo.vecFrustum[0],
		&_CullInfo.vecFrustum[1], &_CullInfo.vecFrustum[2]);
	D3DXPlaneFromPoints(&_CullInfo.planeFrustum[1], &_CullInfo.vecFrustum[6],
		&_CullInfo.vecFrustum[7], &_CullInfo.vecFrustum[5]);
	D3DXPlaneFromPoints(&_CullInfo.planeFrustum[2], &_CullInfo.vecFrustum[2],
		&_CullInfo.vecFrustum[6], &_CullInfo.vecFrustum[4]);
	D3DXPlaneFromPoints(&_CullInfo.planeFrustum[3], &_CullInfo.vecFrustum[7],
		&_CullInfo.vecFrustum[3], &_CullInfo.vecFrustum[5]);
	D3DXPlaneFromPoints(&_CullInfo.planeFrustum[4], &_CullInfo.vecFrustum[2],
		&_CullInfo.vecFrustum[3], &_CullInfo.vecFrustum[6]);
	D3DXPlaneFromPoints(&_CullInfo.planeFrustum[5], &_CullInfo.vecFrustum[1],
		&_CullInfo.vecFrustum[0], &_CullInfo.vecFrustum[4]);
}
