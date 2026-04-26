#include "StdAfx.h"
#include "UIText.h"
#include "smallmap.h"
#include "EffectObj.h"
#include "SceneObj.h"
#include "Scene.h"
#include "Character.h"
#include "SceneObjRecordStore.h"
#include "GameApp.h"
#include "FontManager.h"
#include "MPFont.h"
#include "NetProtocol.h"
#include "CharacterModel.h"
#include "ChaClientAttr.h"
#include "NetChat.h"
#include "uifont.h"
#include "UIMiniMapForm.h"
#include "UIBoxForm.h"

CMaskData* CMaskData::g_MaskData = NULL;

HRESULT _DbgOuts(TCHAR* strFile, DWORD dwLine, HRESULT hr, TCHAR* strMsg) {
	OutputDebugString(std::format("{}({}): ", strFile, dwLine).c_str());
	OutputDebugString(strMsg);

	if (hr) {
		OutputDebugString(std::format("(hr={:08x})\n", static_cast<unsigned long>(hr)).c_str());
	}

	OutputDebugString(_T("\n"));

	return hr;
}

BOOL PosInRange(D3DXVECTOR3& pos, D3DXVECTOR3& Org, float fRange) {
	//return ((pos.x > (Org.x - fRange)) && (pos.x < (Org.x + fRange))
	//	&& (pos.y > (Org.y - fRange)) && (pos.y > (Org.y + fRange)));
	return (fabs(pos.x - Org.x) < fRange && fabs(pos.y - Org.y) < fRange);
}

extern CGameApp* g_pGameApp;

//extern CMPResManger  ResMgr;

#define SHOWWIDTH	40

CSMallWnd::CSMallWnd(void)
	: _pCurSuf(NULL), _pDSSuf(NULL), _pRenderSurface(NULL) {
}

CSMallWnd::~CSMallWnd(void) {
	SAFE_RELEASE(_pCurSuf);
	SAFE_RELEASE(_pDSSuf);
	SAFE_RELEASE(_pRenderSurface);
}

bool CSMallWnd::Create(IDirect3DDeviceX* pDev, RECT rcwnd, CGameScene* pScene, int bksize) {
	m_pDev = pDev;
	_rcWnd = rcwnd;

	m_pScene = pScene;
	D3DXMatrixIdentity(&_matView);

	//LPDIRECT3DSURFACE8	pBackBuffer;
	//   m_pDev->GetBackBuffer( 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer );
	//   pBackBuffer->GetDesc( &_BufParam );
	//   pBackBuffer->Release();
	//D3DXMatrixOrthoLH(&_matProj, SHOWWIDTH, 
	//	SHOWWIDTH, 0.0f, 500.0f);
	int texsize = bksize;

	HRESULT hr = 0;

	//#ifdef MGR
	//	MPIResourceMgr* res_mgr = g_Render.GetInterfaceMgr()->res_mgr;
	//	lwTexInfo	info;
	//	info.stage = 0;
	//	info.type = TEX_TYPE_SIZE;
	//	info.level = 1;
	//	info.usage = D3DUSAGE_RENDERTARGET;
	//	info.format = _BufParam.Format;
	//	info.pool = D3DPOOL_DEFAULT;
	//	info.colorkey_type = COLORKEY_TYPE_NONE;
	//	info.width = texsize;
	//	info.height = texsize;
	//	if(FAILED(hr = lwLoadTex(&_pCurSuf,res_mgr,&info)))
	//#else
	//	if(FAILED(hr = m_pDev->CreateTexture( texsize, texsize, 1, D3DUSAGE_RENDERTARGET , 
	//		_BufParam.Format, D3DPOOL_DEFAULT, &_pCurSuf ) ))
	//#endif
	//	{
	//		_DbgOuts( "CreateTexture", 0, hr, "" );
	//		LG("ERROR","msgCSMallWnd::CreateTexture");
	//		return false;
	//	}

	//#ifdef MGR
	//DWORD handle;

	//   lwIDeviceObject* dev_obj = g_Render.GetInterfaceMgr()->dev_obj;
	//   IDirect3DX* d3d = dev_obj->GetDirect3D();
	//   lwD3DCreateParam* d3dcp = dev_obj->GetD3DCreateParam();
	//   D3DDISPLAYMODE* d3ddm = dev_obj->GetAdapterDisplayMode();
	//   D3DFORMAT depth_fmt = d3dcp->present_param.AutoDepthStencilFormat;
	//   lwISurfaceStreamMgr* surmgr = g_Render.GetInterfaceMgr()->res_mgr->GetSurfaceStreamMgr();
	//   surmgr->CreateDepthStencilSurface(&handle,texsize, texsize, depth_fmt, \
	//       D3DMULTISAMPLE_NONE/*d3dcp->present_param.MultiSampleType*/, 0, false, 0);
	//   _pDSSuf = surmgr->GetSurfaceStream(handle);

	//   hr = d3d->CheckDeviceFormat(
	//       D3DADAPTER_DEFAULT,
	//       D3DDEVTYPE_HAL,
	//       d3ddm->Format,
	//       D3DUSAGE_DEPTHSTENCIL,
	//       D3DRTYPE_SURFACE,
	//       depth_fmt);


	//   hr = d3d->CheckDepthStencilMatch( D3DADAPTER_DEFAULT,
	//       D3DDEVTYPE_HAL,
	//       d3ddm->Format,
	//       _BufParam.Format,
	//       depth_fmt);

	//#else
	//	if(FAILED(hr = m_pDev->CreateDepthStencilSurface( texsize, texsize, 
	//		D3DFMT_D16, _BufParam.MultiSampleType, &_pDSSuf ) ))
	//	{
	//		LG("ERROR","msgCSMallWnd::CreateDepthStencilSurface");
	//		return false;
	//	}
	//#endif


	//#ifdef MGR
	//	//_pCurSuf->GetTex()->GetLevelDesc(0,&_BufParam);
	//#else
	//	_pCurSuf->GetLevelDesc(0,&_BufParam);
	//#endif

	//D3DXCreateRenderToSurface(pDev,128,128,_BufParam.Format,FALSE,D3DFMT_UNKNOWN,&_pRenderSurface);


	_fDist = SHOWWIDTH;
	_vEyePt = D3DXVECTOR3(0, 0, _fDist);
	_vLookatPt = D3DXVECTOR3(0, 0, 0);
	_vUpVec = D3DXVECTOR3(0, -1, 0);

	D3DXMatrixLookAtLH(&_matView, &_vEyePt, &_vLookatPt, &_vUpVec);

	D3DXMatrixPerspectiveFovLH(&_matProj, 1.570796f, 1, 0.0f, 1000.0f);

	//char* pszName = "/system/game.exe";
	//int  nLen = lstrlen(pszName);
	//::GetModuleFileName(NULL,pszAppPath,256);
	//int  nLenApp = lstrlen(pszAppPath);
	//pszAppPath[nLenApp - nLen] = 0;

	InitScene();
	return true;
}

//void	CSMallWnd::FrameMove(DWORD dwDailTime)
//{
//	static DWORD	dwTime = dwDailTime;
//
//	//_bUpdate = false;
//	if(dwDailTime - dwTime > 100)
//	{
//		_bUpdate = true;
//		dwTime = dwDailTime;
//	}
//	if(_bUpdate)
//	{
//		LPDIRECT3DSURFACE8	surface = NULL;
//		_pCurSuf->GetTex()->GetSurfaceLevel(0,&surface);
//		_pRenderSurface->BeginScene(surface,NULL);
//		RenderScene();
//		_pRenderSurface->EndScene();
//	}
//	_bUpdate = false;
//	_bMove = false;
//}
//
//void		CSMallWnd::Render()
//{
//	if(!_pCurSuf /*|| !_pDSSuf*/)
//		return;
////	if(_bUpdate /*&& _bMove*/)
////	{
////
////		HRESULT hr;
////		LPDIRECT3DSURFACE8	pSaveSuf = NULL;
////		LPDIRECT3DSURFACE8	dsface = NULL;
////		LPDIRECT3DSURFACE8	surface = NULL;
////
////		m_pDev->GetRenderTarget(&pSaveSuf);
////		m_pDev->GetDepthStencilSurface(&dsface); 
////#ifdef MGR
////		_pCurSuf->GetTex()->GetSurfaceLevel(0,&surface);
////		if(FAILED(hr = m_pDev->SetRenderTarget(surface,NULL/*_pDSSuf->GetSurface()*/)))
////        {
////            int i = 0;
////        }
////#else
////		_pCurSuf->GetSurfaceLevel(0,&surface);
////		m_pDev->SetRenderTarget(surface,_pDSSuf);
////#endif
////
////		RenderScene();
////		m_pDev->SetRenderTarget(pSaveSuf,dsface);
////
////		dsface->Release();
////		surface->Release();
////		pSaveSuf->Release();
////	}
//
//	////static float fx = 2187.25f;
//	////veye = D3DXVECTOR3(fx,2778.9127f,30);
//	////vlookat = D3DXVECTOR3(fx,2778.9127f,0);
//	////D3DXMatrixLookAtLH( &_matView2, &veye, &vlookat, &_vUpVec );
//	////fx += 0.01f;
//
//	//RenderMask();
//
//	//surface->Release();
//
//	RenderMask();
//}
//
void CSMallWnd::FrameMove(DWORD dwDailTime) {
	//static DWORD	dwTime = dwDailTime;

	////_bUpdate = false;
	//if(dwDailTime - dwTime > 100)
	//{
	//	_bUpdate = true;
	//	dwTime = dwDailTime;
	//}
}

void CSMallWnd::Render() {
	//if(!_pCurSuf /*|| !_pDSSuf*/)
	//	return;
	//if(_bUpdate)
	//{
	//	
	//}
	//_bUpdate = false;
	//_bMove = false;
	if (!_bUpdate)
		return;

	D3DVIEWPORTX vp, cvp;
	cvp.MinZ = 0;
	cvp.MaxZ = 1;
	cvp.X = _rcWnd.left;
	cvp.Y = _rcWnd.top;
	cvp.Width = _rcWnd.right - _rcWnd.left;
	cvp.Height = _rcWnd.bottom - _rcWnd.top;

	//cvp.X = _rcWnd.left;
	//cvp.Y = _rcWnd.top;
	//cvp.Width = 128;
	//cvp.Height= 128;

	g_Render.GetDevice()->GetViewport(&vp);
	g_Render.GetDevice()->SetViewport(&cvp);

	RenderScene();

	g_Render.GetDevice()->SetViewport(&vp);

	//RenderMask();
}

void CSMallWnd::MoveTo(D3DXVECTOR3& vPos, D3DXVECTOR3& vCameraDir, float fCameraAngle, float fCameradDist, int iAngle) {
	if (fabs(vPos.x - _vEyePt.x) > 0.0f || fabs(vPos.y - _vEyePt.y) > 0.0f) {
		_vEyePt = vPos;
		_vLookatPt = vPos;
		_vLookatPt.z = 0;

		_bMove = true;
	}

	_vEyePt.z = _fDist;

	D3DXMatrixLookAtLH(&_matView, &_vEyePt, &_vLookatPt, &_vUpVec);

	_iAngle = iAngle;

	//_vCameraPos = vPos;
	//_vCameraDir = vCameraDir;

	//_vCameraTar1[0] = _vCameraPos + (-_vCameraDir * fCameradDist);
	//_vCameraTar1[0].z = 0.2f;
	//_vCameraTar2[0] = _vCameraTar1[0];

	//D3DXMATRIX	mat;
	//D3DXVECTOR3 vdir;

	//D3DXMatrixRotationZ(&mat,-0.5235987f);
	//D3DXVec3TransformCoord(&vdir,&_vCameraDir,&mat);
	//_vCameraTar1[1] = _vCameraTar1[0] + vdir * (fCameradDist * 2);

	//D3DXMatrixRotationZ(&mat,0.5235987f);
	//D3DXVec3TransformCoord(&vdir,&_vCameraDir,&mat);
	//_vCameraTar2[1] = _vCameraTar1[0] + vdir * (fCameradDist* 2);

	_fCameraAngle = fCameraAngle;
}


/************************************************************************/
/*                                                                      */
/************************************************************************/

CSMallMap2D::CSMallMap2D(void) {
	_pVB = NULL;
	_pTexMask = NULL;
	_pVBWnd = NULL;
	_pTexDefault = NULL;
}

CSMallMap2D::~CSMallMap2D(void) {
	SAFE_RELEASE(_pVB);
	SAFE_RELEASE(_pTexMask);
	SAFE_RELEASE(_pVBWnd);
	SAFE_RELEASE(_pTexDefault);
	//g_Render.GetDevice()->DeleteVertexShader(_dwVShader);
}

void CSMallMap2D::InitScene() {
	D3DXVECTOR2 vUV[4] = {
		D3DXVECTOR2(0, 0),
		D3DXVECTOR2(1, 0),
		D3DXVECTOR2(1, 1),
		D3DXVECTOR2(0, 1),
	};
	_dwColor = D3DXCOLOR(1, 1, 1, 1);
#ifdef MGR
	MPIResourceMgr* res_mgr = g_Render.GetInterfaceMgr()->res_mgr;
	res_mgr->CreateMesh(&_pVB);
	_pVB->SetStreamType(STREAM_LOCKABLE);
	res_mgr->CreateMesh(&_pVBWnd);
	_pVBWnd->SetStreamType(STREAM_LOCKABLE);

	MPMeshInfo mi;
	mi.fvf = D3DFVF_M2D;
	mi.pt_type = D3DPT_TRIANGLEFAN;
	mi.subset_num = 1;
	mi.vertex_num = 4;
	mi.vertex_seq = MP_NEW(MPVector3[mi.vertex_num]);
	mi.blend_seq = MP_NEW(lwBlendInfo[mi.vertex_num]);
	mi.vercol_seq = MP_NEW(DWORD[mi.vertex_num]);
	mi.texcoord0_seq = MP_NEW(MPVector2[mi.vertex_num]);
	mi.texcoord1_seq = MP_NEW(MPVector2[mi.vertex_num]);
	mi.subset_seq = LW_NEW(lwSubsetInfo[mi.subset_num]);

	mi.vertex_seq[0] = MPVector3(0, 0, 0);
	mi.vertex_seq[1] = MPVector3(0, 0, 0);
	mi.vertex_seq[2] = MPVector3(0, 0, 0);
	mi.vertex_seq[3] = MPVector3(0, 0, 0);

	mi.blend_seq[0].weight[0] = 0;
	mi.blend_seq[1].weight[0] = 1;
	mi.blend_seq[2].weight[0] = 2;
	mi.blend_seq[3].weight[0] = 3;

	mi.vercol_seq[0] = _dwColor;
	mi.vercol_seq[1] = _dwColor;
	mi.vercol_seq[2] = _dwColor;
	mi.vercol_seq[3] = _dwColor;

	mi.texcoord0_seq[0] = MPVector2(0, 0);
	mi.texcoord0_seq[1] = MPVector2(1, 0);
	mi.texcoord0_seq[2] = MPVector2(1, 1);
	mi.texcoord0_seq[3] = MPVector2(0, 1);

	mi.texcoord1_seq[0] = MPVector2(0, 0);
	mi.texcoord1_seq[1] = MPVector2(1, 0);
	mi.texcoord1_seq[2] = MPVector2(1, 1);
	mi.texcoord1_seq[3] = MPVector2(0, 1);

	lwSubsetInfo_Construct(&mi.subset_seq[0], 2, 0, 4, 0);

	_pVB->LoadSystemMemory(&mi);
	_pVB->LoadVideoMemory();

	mi.fvf = D3DFVF_M2DW;

	mi.vertex_seq[0] = MPVector3(float(_rcWnd.left), float(_rcWnd.top), 0.9f);
	mi.vertex_seq[1] = MPVector3(float(_rcWnd.right), float(_rcWnd.top), 0.9f);
	mi.vertex_seq[2] = MPVector3(float(_rcWnd.right), float(_rcWnd.bottom), 0.9f);
	mi.vertex_seq[3] = MPVector3(float(_rcWnd.left), float(_rcWnd.bottom), 0.9f);

	mi.blend_seq[0].weight[0] = 1;
	mi.blend_seq[1].weight[0] = 1;
	mi.blend_seq[2].weight[0] = 1;
	mi.blend_seq[3].weight[0] = 1;

	lwSubsetInfo_Construct(&mi.subset_seq[0], 2, 0, 4, 0);

	_pVBWnd->LoadSystemMemory(&mi);
	_pVBWnd->LoadVideoMemory();

#else
	if (FAILED(m_pDev->CreateVertexBuffer( 4 * sizeof(M2D_VER),
		D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, 0,
		D3DPOOL_DEFAULT, &_pVB ))) {
		ToLogService("errors", LogLevel::Error, "msgCSMallWnd::CreateVertexBuffer");
		return;
	}

	M2D_VER* pVertices;
	_pVB->Lock(0, 0, (BYTE**)&pVertices, D3DLOCK_DISCARD);
	for (int n = 0; n < 4; n++) {
		(*pVertices++).SetValue(D3DXVECTOR4(0, 0, 0, float(n)), _dwColor, vUV[n]);
	}
	_pVB->Unlock();


	if (FAILED(m_pDev->CreateVertexBuffer( 4 * sizeof(M2D_VER),
		D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, 0,
		D3DPOOL_DEFAULT, &_pVBWnd ))) {
		ToLogService("common", "msgERROR");
		return;
	}

	M2D_VER* pVers;
	_pVBWnd->Lock(0, 0, (BYTE**)&pVers, D3DLOCK_DISCARD);
	(*pVers++).SetValue(D3DXVECTOR4(float(_rcWnd.left), float(_rcWnd.top), 0.9f, 1), 0xffffffff, vUV[0]);
	(*pVers++).SetValue(D3DXVECTOR4(float(_rcWnd.right), float(_rcWnd.top), 0.9f, 1), 0xffffffff, vUV[1]);
	(*pVers++).SetValue(D3DXVECTOR4(float(_rcWnd.right), float(_rcWnd.bottom), 0.9f, 1), 0xffffffff, vUV[2]);
	(*pVers++).SetValue(D3DXVECTOR4(float(_rcWnd.left), float(_rcWnd.bottom), 0.9f, 1), 0xffffffff, vUV[3]);
	_pVBWnd->Unlock();
#endif

	//////////////////////////////////////////////////////////////////////////

	_Cha.Init(m_pDev);

	//#ifdef MGR
	//#else
	//	LPD3DXBUFFER pCode;
	//	char t_Path[64];
	//	DWORD dwMapDecl[] =
	//	{
	//		D3DVSD_STREAM( 0 ),
	//			D3DVSD_REG( D3DVSDE_POSITION ,		D3DVSDT_FLOAT3 ), // Position of first mesh
	//			D3DVSD_REG( D3DVSDE_BLENDWEIGHT,	D3DVSDT_FLOAT1),
	//			D3DVSD_REG( D3DVSDE_DIFFUSE,		D3DVSDT_D3DCOLOR ), // diffuse
	//			D3DVSD_REG( D3DVSDE_TEXCOORD0,		D3DVSDT_FLOAT2 ), // Tex coords
	//			D3DVSD_END()
	//	};
	//	if(_dwVShader != 0)
	//	{
	//		g_Render.GetDevice()->DeleteVertexShader(_dwVShader);
	//		_dwVShader = 0;
	//	}
	//	if(SUCCEEDED(D3DXAssembleShaderFromFile( t_Path, NULL, 0, &pCode, NULL )))
	//	{
	//		if( FAILED(m_pDev->CreateVertexShader( dwMapDecl, 
	//			(DWORD*)pCode->GetBufferPointer(),
	//			&_dwVShader , FALSE ) ) )
	//		{
	//			_dwVShader = 0L;
	//		}
	//		pCode->Release();
	//		pCode = NULL;
	//	}
	//	else
	//		_dwVShader = 0L;
	//#endif

#ifdef MGR
	//MPIResourceMgr* res_mgr = g_Render.GetInterfaceMgr()->res_mgr;
	if (FAILED(lwLoadTex(&_pTexMask,res_mgr, "texture\\minimap\\mapmask.bmp", 0, D3DFMT_A8R8G8B8))) {
		ToLogService("errors", LogLevel::Error, "msgmapmask.bmp");
	}
	if (FAILED(lwLoadTex(&_pTexDefault,res_mgr, "texture\\minimap\\mapsea.bmp", 0, D3DFMT_A8R8G8B8))) {
		ToLogService("errors", LogLevel::Error, "msgmapsea.bmp");
	}
#else
	if (FAILED(D3DXCreateTextureFromFileEx(m_pDev,
		"texture\\minimap\\mapmask.bmp", //
		0,
		0,
		1, //mipmap1
		0, //
		D3DFMT_UNKNOWN, //
		D3DPOOL_MANAGED, //DXGraphics
		D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR, //
		D3DX_FILTER_NONE, //mipmap
		0xff000000, //
		NULL, //
		NULL, //
		&_pTexMask))) //
	{
		ToLogService("errors", LogLevel::Error, "msgtexture\\minimap\\mapmask.bmp");
	}

	if (FAILED(D3DXCreateTextureFromFileEx(m_pDev,
		"texture\\minimap\\mapsea.bmp", //
		0,
		0,
		1, //mipmap1
		0, //
		D3DFMT_UNKNOWN, //
		D3DPOOL_MANAGED, //DXGraphics
		D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR, //
		D3DX_FILTER_NONE, //mipmap
		0xff000000, //
		NULL, //
		NULL, //
		&_pTexDefault))) //
	{
		ToLogService("errors", LogLevel::Error, "msgtexture\\minimap\\mapsea.bmp");
	}
#endif
}

void CSMallMap2D::RenderScene() {
	::SetCurrentDirectory(pszAppPath);

	m_pScene = g_pGameApp->GetCurScene();
	//g_Render.GetDevice()->Clear(0,0,D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,0xff000000,1,0);
	D3DXMATRIX matIdentity;
	D3DXMatrixIdentity(&matIdentity);
	//g_Render.SetRenderState( D3DRS_MULTISAMPLEANTIALIAS,FALSE); 
	g_Render.SetRenderState(D3DRS_ZENABLE, FALSE);
	g_Render.SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	g_Render.SetRenderState(D3DRS_CLIPPING, TRUE);

	g_Render.SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	g_Render.SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_Render.SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_Render.SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	g_Render.SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
	g_Render.SetRenderState(D3DRS_DITHERENABLE,FALSE);
	g_Render.SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW); // 
	//
	g_Render.SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	g_Render.SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);


	g_Render.SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_Render.SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	g_Render.SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	g_Render.SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	g_Render.SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	g_Render.SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

	g_Render.SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	g_Render.SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	//g_Render.SetTextureStageState( 0, D3DTSS_MIPFILTER, D3DTEXF_NONE);

	g_Render.SetRenderState(D3DRS_TEXTUREFACTOR, 0xffffffff);

	g_Render.SetRenderState(D3DRS_LIGHTING, FALSE);

#ifdef MGR
	_pVB->BeginSet();
#else
	m_pDev->SetStreamSource(0, _pVB, sizeof(M2D_VER));
#endif

	if (ResMgr.m_bUseSoft) {
		g_Render.SetVertexShader(NULL);
		g_Render.SetFVF(D3DFVF_M2D);
		g_Render.SetTransformWorld(&matIdentity);
		g_Render.SetTransformProj(&_matProj);
		g_Render.SetTransformView(&_matView);
	}
	else {
		D3DXMATRIX vpPose;
		const auto matrix = _matView * _matProj;
		g_Render.SetVertexShader(ResMgr.GetMinimapVS());
		g_Render.SetVertexDeclaration(ResMgr.GetMinimapVDecl());
		g_Render.SetVertexShaderConstantF(0, matIdentity, 4);
		g_Render.SetVertexShaderConstantF(4, vpPose, 4);
		g_Render.SetVertexShaderConstantF(8, _dwColor, 1);
	}

	//g_Render.SetTexture(0, NULL); 
	//g_Render.SetTexture(1, NULL); 
	int sx = (int)(_vEyePt.x / SHOWRSIZE);
	int sy = (int)(_vEyePt.y / SHOWRSIZE);
	if (sx > 0) sx -= 1;
	if (sy > 0) sy -= 1;

	if (_sx == sx && _sy == sy) {
	}
	else {
		_sx = sx;
		_sy = sy;
	}
	int texX = sx;
	int texY = sy;

	sx *= SHOWRSIZE;
	sy *= SHOWRSIZE;

	int m;
	int temp = sx;
	for (m = 0; m < 3; m++) {
		for (int n = 0; n < 3; n++) {
			const std::string filename = std::format("texture\\minimap\\{}\\sm{}{}.bmp",
					m_pScene->GetTerrainName(), _sx + n, _sy + m);

			if (_access(filename.c_str(), 0) == -1) {
#ifdef MGR
				g_Render.SetTexture(0, _pTexDefault->GetTex());
#else
				g_Render.SetTexture(0, _pTexDefault);
#endif
			}
			else {
				if ((_pTex[n][m].x != _sx + n) || (_pTex[n][m].y != _sy + m)) {
					_pTex[n][m].ReleaseTex();

#ifdef MGR
					lwLoadTex(&_pTex[n][m].pTex, g_Render.GetInterfaceMgr()->res_mgr,
							  filename.c_str(), 0, D3DFMT_A8R8G8B8);
#else
					D3DXCreateTextureFromFileEx(m_pDev,
												filename.c_str(), //
												0,
												0,
												1, //mipmap1
												0, //
												D3DFMT_UNKNOWN, //
												D3DPOOL_MANAGED, //DXGraphics
												D3DX_FILTER_LINEAR, // TRIANGLE|D3DX_FILTER_DITHER
												D3DX_FILTER_NONE, //mipmap
												0x00000000, //
												NULL, //
												NULL, //
												&_pTex[n][m].pTex); //
#endif
					if (_pTex[n][m].pTex) {
						_pTex[n][m].x = _sx + n;
						_pTex[n][m].y = _sy + m;
					}
				}
#ifdef MGR
				if (_pTex[n][m].pTex)
					g_Render.SetTexture(0, _pTex[n][m].pTex->GetTex());
				else
					g_Render.SetTexture(0, NULL);
#else
				g_Render.SetTexture(0, _pTex[n][m].pTex);
#endif
			}
			if (ResMgr.m_bUseSoft) {
				M2D_VER* pVertices;
#ifdef MGR
				lwILockableStreamVB* _lpSVB = _pVB->GetLockableStreamVB();
				_lpSVB->Lock(0, 0, (void**)&pVertices, 0);
#else
				_pVB->Lock(0, 0, (BYTE**)&pVertices, 0);
#endif
				(*pVertices++).m_vPos = D3DXVECTOR4(float(sx), float(sy), 0, 1);
				(*pVertices++).m_vPos = D3DXVECTOR4(float(sx) + SHOWRSIZE, float(sy), 0, 1);
				(*pVertices++).m_vPos = D3DXVECTOR4(float(sx) + SHOWRSIZE, float(sy) + SHOWRSIZE, 0, 1);
				(*pVertices++).m_vPos = D3DXVECTOR4(float(sx), float(sy) + SHOWRSIZE, 0, 1);
#ifdef MGR
				_lpSVB->Unlock();
#else
				_pVB->Unlock();
#endif
			}
			else {
				g_Render.SetVertexShaderConstantF(9, D3DXVECTOR4(float(sx), float(sy),
																 0, 1), 1);
				g_Render.SetVertexShaderConstantF(10, D3DXVECTOR4(float(sx) + SHOWRSIZE, float(sy),
																  0, 1), 1);

				g_Render.SetVertexShaderConstantF(11, D3DXVECTOR4(float(sx) + SHOWRSIZE, float(sy) +
																  SHOWRSIZE, 0, 1), 1);
				g_Render.SetVertexShaderConstantF(12, D3DXVECTOR4(float(sx), float(sy) + SHOWRSIZE,
																  0, 1), 1);
			}
#ifdef MGR
			_pVB->DrawSubset(0);
#else
			g_Render.DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);
#endif
			sx += SHOWRSIZE;
		}
		sx = temp;
		sy += SHOWRSIZE;
	}
#ifdef MGR
	_pVB->EndSet();
#endif

	//return;

	_Cha._vecNpc.clear();

	if (!ResMgr.m_bUseSoft) {
		_Cha.Begin(ResMgr.GetMinimapVS());
		g_Render.SetVertexDeclaration(ResMgr.GetMinimapVDecl());
		CCharacter* pCha = NULL;
		int nMainTeadID = m_pScene->GetMainCha()->GetTeamLeaderID();
		for (m = 0; m < m_pScene->GetChaCnt(); m++) {
			pCha = m_pScene->GetCha(m);
			if (pCha && pCha->IsInMiniMap() && pCha != m_pScene->GetMainCha()) {
				if (PosInRange(pCha->GetPos(), _vEyePt, SHOWRSIZE)) {
					_Cha.setTexture(0);
					_Cha.setPos(pCha->GetPos());
					_Cha.setScaling(0.5f, 0.5f, 0.5f);
					_Cha.setAngle(pCha->getYaw());
					//g_Render.SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);		
					//g_Render.SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR);

					if (pCha->IsNPC()) {
						//g_Render.SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_POINT);		
						//g_Render.SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_POINT);
						_Cha.setAngle(180);
						_Cha.setScaling(0.45f, 0.45f, 0.45f);
						if (pCha->getNpcType() > 0) {
							_Cha.setTexture(pCha->getNpcType());
							_Cha.setColor(0xffffffff);
						}
						else {
							_Cha.setTexture(10);
							_Cha.setColor(0xffffff00);
						}


						//int dx,dy;
						//GetWndPos(dx, dy, pCha->GetPos());
						//_Cha.RenderWndCha(pCha->getNpcType(),dx,dy,12);

						//int dx,dy;
						//GetWndPos(dx, dy, pCha->GetPos());
						//MNPC_PARAM param;
						//param.iType = pCha->getNpcType();
						//param.nx = dx;// + (_rcWnd.left + 64);
						//param.ny = dy;// + (_rcWnd.top + 64);

						//_Cha._vecNpc.push_back(param);
						//continue;
					}
					else if (pCha->IsMonster())
						_Cha.setColor(0xffff0000);
					else {
						if (nMainTeadID > 0) {
							if (nMainTeadID == pCha->GetTeamLeaderID())
								_Cha.setColor(D3DCOLOR_ARGB(255, 0, 255, 0));
							else
								_Cha.setColor(D3DCOLOR_ARGB(255, 0, 220, 220));
						}
						else if (pCha->IsResource()) {
							_Cha.setColor(D3DCOLOR_ARGB(255, 255, 180, 0));
						}
						else {
							_Cha.setColor(D3DCOLOR_ARGB(255, 0, 220, 220));
						}
					}
					_Cha.Render();
				}
			}
		}
		CSceneObj* pscobj = NULL;
		for (m = 0; m < m_pScene->GetSceneObjCnt(); m++) {
			pscobj = m_pScene->GetSceneObj(m);

			if (pscobj->IsValid() == FALSE) continue;
			if (pscobj->getObjType() != SCENEOBJ_TYPE_NORMAL) continue;

			int nScriptID = pscobj->getTypeID();
			CSceneObjInfo* pInfo = GetSceneObjInfo(nScriptID);
			if (pscobj /*&& pscobj->IsValid()*/ && pInfo->_flag != 0) {
				if (PosInRange(pscobj->getPos(), _vEyePt, SHOWRSIZE)) {
					_Cha.setTexture(15);
					_Cha.setAngle((int)D3DX_PI);
					_Cha.setPos(pscobj->getPos());
					_Cha.setScaling(0.5f, 0.5f, 0.5f);
					_Cha.setColor(0xffffffff);
					_Cha.Render();
				}
			}
		}
		_Cha.setTexture(14);
		_Cha.setPos(_vEyePt);
		_Cha.setAngle(_iAngle);
		_Cha.setScaling(0.5f, 1, 0.5f);
		_Cha.setColor(0xffffffff);
		_Cha.Render();
		_Cha.End();

		//int tn;
		//int tm = (int)_Cha._vecNpc.size();
		//for (tn = 0; tn < tm; ++tn)
		//{
		//	_Cha.RenderWndCha(_Cha._vecNpc[tn].iType,_Cha._vecNpc[tn].nx,_Cha._vecNpc[tn].ny);
		//}
	}
	else {
		_Cha.Begin(NULL);
		g_Render.SetFVF(D3DFVF_M2D);
		//g_Render.SetVertexShader(D3DFVF_M2D);

		g_Render.SetTransformWorld(&matIdentity);
		g_Render.SetTransformView(&_matView);
		g_Render.SetTransformProj(&_matProj);
		CCharacter* pCha = NULL;
		for (m = 1; m < m_pScene->GetChaCnt(); m++) {
			pCha = m_pScene->GetCha(m);
			if (pCha && pCha->IsInMiniMap()) {
				if (PosInRange(pCha->GetPos(), _vEyePt, SHOWRSIZE)) {
					_Cha.setPos(pCha->GetPos());
					_Cha.setAngle(pCha->getYaw());

					switch (pCha->getChaCtrlType()) {
					case enumCHACTRL_PLAYER:
						_Cha.setColor(0xff0000ff);
						break;
					case enumCHACTRL_NPC:
						_Cha.setColor(0xffffff00);
						break;
					case enumCHACTRL_MONS:
						_Cha.setColor(0xffff0000);
						break;
					}

					_Cha.RenderSoft();
				}
			}
		}
		CSceneObj* pscobj = NULL;
		for (m = 0; m < m_pScene->GetSceneObjCnt(); m++) {
			pscobj = m_pScene->GetSceneObj(m);
			if (pscobj && pscobj->IsValid() && pscobj->getObjType() != 0) {
				if (PosInRange(pscobj->getPos(), _vEyePt, SHOWRSIZE)) {
					_Cha.setTexture(2);
					_Cha.setPos(pscobj->getPos());
					_Cha.setColor(0xffffffff);
					_Cha.RenderSoft();
				}
			}
		}
		_Cha.setPos(_vEyePt);
		_Cha.setAngle(_iAngle);
		_Cha.setColor(0xffffffff);
		_Cha.RenderSoft();

		_Cha.End();
	}

	//g_Render.SetTexture(0, NULL); 
	//D3DXMATRIX matIden;

	////D3DXMatrixRotationZ(&matIden,0.1f);
	//GetMatrixRotation(&matIden,&_vCameraPos,\
	//	&D3DXVECTOR3(0,0,1),_fCameraAngle);


	//g_Render.SetTransformWorld(&matIden);
	////g_Render.SetTransformWorld(&matIdentity);

	//g_Render.SetVertexShader(D3DFVF_XYZ);
	//g_Render.SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	//g_Render.SetRenderState(D3DRS_TEXTUREFACTOR,0xffffffff);
	////g_Render.SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);		
	////g_Render.SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR);

	//g_Render.SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
	//g_Render.SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_SELECTARG2);  

	//int r = 0;
	//for (; r < 5; ++r)
	//{
	//	g_Render.GetDevice()->DrawPrimitiveUP(D3DPT_LINELIST,1,&_vCameraTar1,sizeof(D3DXVECTOR3));
	//	g_Render.GetDevice()->DrawPrimitiveUP(D3DPT_LINELIST,1,&_vCameraTar2,sizeof(D3DXVECTOR3));
	//}	

	//g_Render.SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	//g_Render.SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_MODULATE);  
	//g_Render.SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	//g_Render.LookAt(g_pGameApp->GetMainCam()->m_EyePos, g_pGameApp->GetMainCam()->m_RefPos);
	//g_Render.SetCurrentView(MPRender::VIEW_WORLD);
}

void CSMallMap2D::RenderMask() {
	D3DXMATRIX matIdentity;
	D3DXMatrixIdentity(&matIdentity);
	//g_Render.SetRenderState( D3DRS_MULTISAMPLEANTIALIAS,FALSE); 
	g_Render.SetRenderState(D3DRS_ZENABLE, FALSE);
	g_Render.SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	g_Render.SetRenderState(D3DRS_CLIPPING, TRUE);

	g_Render.SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	g_Render.SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_Render.SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_Render.SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	g_Render.SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
	g_Render.SetRenderState(D3DRS_DITHERENABLE,FALSE);
	g_Render.SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW); // 

	g_Render.SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	g_Render.SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

	g_Render.SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_Render.SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	g_Render.SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	//g_Render.SetRenderState(D3DRS_ALPHATESTENABLE,   FALSE );


	g_Render.SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	g_Render.SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	g_Render.SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

	g_Render.SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

	g_Render.SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	g_Render.SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	g_Render.SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);

	g_Render.SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_Render.SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_Render.SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	g_Render.SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_POINT);

	g_Render.SetRenderState(D3DRS_LIGHTING, FALSE);

#ifdef MGR
	g_Render.SetTexture(0, _pTexMask->GetTex());
	g_Render.SetTexture(1, _pCurSuf->GetTex());
	_pVBWnd->BeginSet();
	//g_Render.GetDevice()->SetVertexShader(D3DFVF_M2DW);
	_pVBWnd->DrawSubset(0);
	_pVBWnd->EndSet();

#else
	g_Render.SetTexture(0, _pTexMask);
	g_Render.SetTexture(1, _pCurSuf);

	m_pDev->SetStreamSource(0, _pVBWnd, sizeof(M2D_VER));
	g_Render.SetVertexShader(D3DFVF_M2DW);
	g_Render.DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);
#endif
	g_Render.SetTexture(1, NULL);

	//g_Render.SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);		
	//g_Render.SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR);

	//int n;
	//int m = (int)_Cha._vecNpc.size();
	//for (n = 0; n < m; ++n)
	//{
	//	_Cha.RenderWndCha(_Cha._vecNpc[n].iType,_Cha._vecNpc[n].nx,_Cha._vecNpc[n].ny);
	//}
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
CAniWnd::CAniWnd() {
	_bUpdate = false;
	_pVBWnd = NULL;
}

CAniWnd::~CAniWnd() {
	SAFE_RELEASE(_pVBWnd);
}

void CAniWnd::Play(DWORD dwPlayTime) {
	if (_bUpdate)
		return;
	_bUpdate = true;
	ResetTime(dwPlayTime);
}

void CAniWnd::ResetTime(DWORD dwTime) {
	_fPlayTime = (float)dwTime / 1000;
	_vSave[0].vPos = D3DXVECTOR3(0, 0, 0);
	_vSave[0].dwColor = 0xffffffff;

	_vSave[1].vPos = D3DXVECTOR3(0, -30, 0);
	_vSave[1].dwColor = 0xffffffff;

	_vSave[2].vPos = D3DXVECTOR3(30, 0, 0);
	_vSave[2].dwColor = 0xffffffff;

	_vSave[3].vPos = D3DXVECTOR3(0, 30, 0);
	_vSave[3].dwColor = 0xffffffff;

	_vSave[4].vPos = D3DXVECTOR3(-30, 0, 0);
	_vSave[4].dwColor = 0xffffffff;

	_vSave[5].vPos = D3DXVECTOR3(0, -30, 0);
	_vSave[5].dwColor = 0xffffffff;

	_vVertex[0].vPos = D3DXVECTOR3(0, 0, 0);
	_vVertex[0].dwColor = 0xffffffff;

	_vVertex[1].vPos = D3DXVECTOR3(0, -30, 0);
	_vVertex[1].dwColor = 0xffffffff;

	_vVertex[2].vPos = D3DXVECTOR3(30, 0, 0);
	_vVertex[2].dwColor = 0xffffffff;

	_vVertex[3].vPos = D3DXVECTOR3(0, 30, 0);
	_vVertex[3].dwColor = 0xffffffff;

	_vVertex[4].vPos = D3DXVECTOR3(-30, 0, 0);
	_vVertex[4].dwColor = 0xffffffff;

	_vVertex[5].vPos = D3DXVECTOR3(0, -30, 0);
	_vVertex[5].dwColor = 0xffffffff;

	_fCurAngle = 0; // 6.283185f / (float)_dwTime;
	_fCurTime = 0;
}

void CAniWnd::FrameMove(DWORD dwDailTime) {
	if (!_bUpdate)
		return;
	_fCurTime += *ResMgr.GetDailTime();
	_fCurAngle = (_fCurTime / _fPlayTime) * 6.283185f;
	//_fCurAngle = 0.12f;

	D3DXVECTOR4 ver;
	D3DXMATRIX mat;
	D3DXMatrixRotationZ(&mat, _fCurAngle);
	D3DXVec3Transform(&ver, &_vSave[1].vPos, &mat);

	_vVertex[1].vPos.x = ver.x;
	_vVertex[1].vPos.y = ver.y;
	_vVertex[1].vPos.z = ver.z;

	if (_fCurAngle >= 1.570796f && _fCurAngle < 3.141592f) {
		_vVertex[2].vPos = _vVertex[1].vPos;
	}
	else if (_fCurAngle >= 3.141592f && _fCurAngle < 3.141592f + 1.570796f) {
		_vVertex[2].vPos = _vVertex[1].vPos;
		_vVertex[3].vPos = _vVertex[1].vPos;
	}
	else if (_fCurAngle >= 3.141592f + 1.570796f && _fCurAngle < 6.283185f) {
		_vVertex[2].vPos = _vVertex[1].vPos;
		_vVertex[3].vPos = _vVertex[1].vPos;
		_vVertex[4].vPos = _vVertex[1].vPos;
	}
	else if (_fCurAngle >= 6.283185f) {
		_bUpdate = false;
	}
}

void CAniWnd::Render() {
	if (_bUpdate) {
		IDirect3DSurfaceX* pSaveSuf = NULL;
		//IDirect3DSurfaceX*	dsface = NULL;
		IDirect3DSurfaceX* surface = NULL;

		m_pDev->GetRenderTarget(0, &pSaveSuf);
		//m_pDev->GetDepthStencilSurface(&dsface);
#ifdef MGR
		_pCurSuf->GetTex()->GetSurfaceLevel(0, &surface);
		m_pDev->SetRenderTarget(0, surface);
#else
		_pCurSuf->GetSurfaceLevel(0, &surface);
		m_pDev->SetRenderTarget(surface, _pDSSuf);
#endif
		RenderScene();
		m_pDev->SetRenderTarget(0, pSaveSuf);

		//dsface->Release();
		surface->Release();
		pSaveSuf->Release();
		RenderMask();
	}
}

void CAniWnd::MoveWnd(int x, int y) {
	int w = _rcWnd.right - _rcWnd.left;
	int h = _rcWnd.bottom - _rcWnd.top;

	_rcWnd.left = x;
	_rcWnd.top = y;
	_rcWnd.right = x + w;
	_rcWnd.bottom = y + h;

	D3DXVECTOR2 vUV[4] = {
		D3DXVECTOR2(0, 0),
		D3DXVECTOR2(1, 0),
		D3DXVECTOR2(1, 1),
		D3DXVECTOR2(0, 1),
	};
	M2D_AVER* pVers;
#ifdef MGR
	lwILockableStreamVB* _lpSVB = _pVBWnd->GetLockableStreamVB();
	_lpSVB->Lock(0, 0, (void**)&pVers, 0);
#else
	_pVBWnd->Lock(0, 0, (BYTE**)&pVers, D3DLOCK_DISCARD);
#endif
	D3DXVECTOR4 v[] = {
		{float(_rcWnd.left), float(_rcWnd.top), 0.9f, 1},
		{float(_rcWnd.right), float(_rcWnd.top), 0.9f, 1},
		{float(_rcWnd.right), float(_rcWnd.bottom), 0.9f, 1},
		{float(_rcWnd.left), float(_rcWnd.bottom), 0.9f, 1},
		{float(_rcWnd.left), float(_rcWnd.top), 0.9f, 1},
		{float(_rcWnd.right), float(_rcWnd.top), 0.9f, 1},
		{float(_rcWnd.right), float(_rcWnd.bottom), 0.9f, 1},
		{float(_rcWnd.left), float(_rcWnd.bottom), 0.9f, 1}
	};


	_vWndVer[0].SetValue(v[0], 0x80ffffff, vUV[0]);
	_vWndVer[0].SetValue(v[1], 0x80ffffff, vUV[1]);
	_vWndVer[0].SetValue(v[2], 0x80ffffff, vUV[2]);
	_vWndVer[0].SetValue(v[3], 0x80ffffff, vUV[3]);

	(*pVers++).SetValue(v[4], 0x80ffffff, vUV[0]);
	(*pVers++).SetValue(v[5], 0x80ffffff, vUV[1]);
	(*pVers++).SetValue(v[6], 0x80ffffff, vUV[2]);
	(*pVers++).SetValue(v[7], 0x80ffffff, vUV[3]);
#ifdef MGR
	_lpSVB->Unlock();
#else
	_pVBWnd->Unlock();
#endif
}

void CAniWnd::InitScene() {
	D3DXVECTOR2 vUV[4] = {
		D3DXVECTOR2(0, 0),
		D3DXVECTOR2(1, 0),
		D3DXVECTOR2(1, 1),
		D3DXVECTOR2(0, 1),
	};
#ifdef MGR
	MPIResourceMgr* res_mgr = g_Render.GetInterfaceMgr()->res_mgr;
	res_mgr->CreateMesh(&_pVBWnd);
	_pVBWnd->SetStreamType(STREAM_LOCKABLE);

	MPMeshInfo mi;
	mi.fvf = D3DFVF_M2DWA;
	mi.pt_type = D3DPT_TRIANGLEFAN;
	mi.subset_num = 1;
	mi.vertex_num = 4;
	mi.vertex_seq = MP_NEW(MPVector3[mi.vertex_num]);
	mi.blend_seq = MP_NEW(lwBlendInfo[mi.vertex_num]);
	mi.vercol_seq = MP_NEW(DWORD[mi.vertex_num]);
	mi.texcoord0_seq = MP_NEW(MPVector2[mi.vertex_num]);
	mi.texcoord1_seq = MP_NEW(MPVector2[mi.vertex_num]);
	mi.subset_seq = LW_NEW(lwSubsetInfo[mi.subset_num]);

	mi.vertex_seq[0] = MPVector3(float(_rcWnd.left), float(_rcWnd.top), 0.9f);
	mi.vertex_seq[1] = MPVector3(float(_rcWnd.right), float(_rcWnd.top), 0.9f);
	mi.vertex_seq[2] = MPVector3(float(_rcWnd.right), float(_rcWnd.bottom), 0.9f);
	mi.vertex_seq[3] = MPVector3(float(_rcWnd.left), float(_rcWnd.bottom), 0.9f);

	mi.blend_seq[0].weight[0] = 1;
	mi.blend_seq[1].weight[0] = 1;
	mi.blend_seq[2].weight[0] = 1;
	mi.blend_seq[3].weight[0] = 1;

	mi.vercol_seq[0] = 0x80ffffff;
	mi.vercol_seq[1] = 0x80ffffff;
	mi.vercol_seq[2] = 0x80ffffff;
	mi.vercol_seq[3] = 0x80ffffff;

	mi.texcoord0_seq[0] = MPVector2(0, 0);
	mi.texcoord0_seq[1] = MPVector2(1, 0);
	mi.texcoord0_seq[2] = MPVector2(1, 1);
	mi.texcoord0_seq[3] = MPVector2(0, 1);

	mi.texcoord1_seq[0] = MPVector2(0, 0);
	mi.texcoord1_seq[1] = MPVector2(1, 0);
	mi.texcoord1_seq[2] = MPVector2(1, 1);
	mi.texcoord1_seq[3] = MPVector2(0, 1);

	lwSubsetInfo_Construct(&mi.subset_seq[0], 2, 0, 4, 0);

	_pVBWnd->LoadSystemMemory(&mi);
	_pVBWnd->LoadVideoMemory();
#else
	if (FAILED(m_pDev->CreateVertexBuffer( 4 * sizeof(M2D_VER),
		D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, 0,
		D3DPOOL_DEFAULT, &_pVBWnd ))) {
		ToLogService("common", "msgERRORCreateVertexBuffer");
		return;
	}


	M2D_AVER* pVers;
	_pVBWnd->Lock(0, 0, (BYTE**)&pVers, D3DLOCK_DISCARD);
	_vWndVer[0].SetValue(D3DXVECTOR4(float(_rcWnd.left), float(_rcWnd.top), 0.9f, 1), 0x80ffffff, vUV[0]);
	_vWndVer[0].SetValue(D3DXVECTOR4(float(_rcWnd.right), float(_rcWnd.top), 0.9f, 1), 0x80ffffff, vUV[1]);
	_vWndVer[0].SetValue(D3DXVECTOR4(float(_rcWnd.right), float(_rcWnd.bottom), 0.9f, 1), 0x80ffffff, vUV[2]);
	_vWndVer[0].SetValue(D3DXVECTOR4(float(_rcWnd.left), float(_rcWnd.bottom), 0.9f, 1), 0x80ffffff, vUV[3]);

	(*pVers++).SetValue(D3DXVECTOR4(float(_rcWnd.left), float(_rcWnd.top), 0.9f, 1), 0x80ffffff, vUV[0]);
	(*pVers++).SetValue(D3DXVECTOR4(float(_rcWnd.right), float(_rcWnd.top), 0.9f, 1), 0x80ffffff, vUV[1]);
	(*pVers++).SetValue(D3DXVECTOR4(float(_rcWnd.right), float(_rcWnd.bottom), 0.9f, 1), 0x80ffffff, vUV[2]);
	(*pVers++).SetValue(D3DXVECTOR4(float(_rcWnd.left), float(_rcWnd.bottom), 0.9f, 1), 0x80ffffff, vUV[3]);

	_pVBWnd->Unlock();

#endif
	SetDist(10);
	D3DXVECTOR3 v[] = {D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 1, 0)};
	MoveTo(v[0], v[1], 7, 0);
}

void CAniWnd::RenderScene() {
	g_Render.GetDevice()->Clear(0, 0,D3DCLEAR_TARGET, 0x00000000, 0, 0);
	D3DXMATRIX matIdentity;
	D3DXMatrixIdentity(&matIdentity);
	g_Render.SetRenderState(D3DRS_ZENABLE, FALSE);
	g_Render.SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	g_Render.SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	g_Render.SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_Render.SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_Render.SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	g_Render.SetRenderState(D3DRS_DITHERENABLE,FALSE);
	g_Render.SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW); // 

	g_Render.SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	g_Render.SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2);

	g_Render.SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	g_Render.SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG2);

	//g_Render.SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_POINT);		
	//g_Render.SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_POINT);

	g_Render.SetRenderState(D3DRS_LIGHTING, FALSE);
	g_Render.SetTexture(0, NULL);
	//m_pDev->SetStreamSource( 0, _pVB, sizeof(M2D_VER) );
	g_Render.SetVertexShader(NULL);
	g_Render.SetFVF(D3DFVF_CLOCK);
	g_Render.SetTransformWorld(&matIdentity);
	g_Render.SetTransformProj(&_matProj);
	g_Render.SetTransformView(&_matView);

	//MPInterfaceMgr* imgr = g_Render.GetInterfaceMgr();
	//lwIDynamicStreamMgr* dsm = imgr->res_mgr->GetDynamicStreamMgr();
	//dsm->BindDataVB(0, &_vVertex, sizeof(ClockVer) * 6, sizeof(ClockVer));
	//dsm->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 4);

	g_Render.GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 4, &_vVertex, sizeof(ClockVer));
}

void CAniWnd::RenderMask() {
	g_Render.SetRenderState(D3DRS_ZENABLE, FALSE);
	g_Render.SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	g_Render.SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_Render.SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_Render.SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	g_Render.SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
	g_Render.SetRenderState(D3DRS_DITHERENABLE,FALSE);
	g_Render.SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW); // 

	g_Render.SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_Render.SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	g_Render.SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	g_Render.SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	g_Render.SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	g_Render.SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

	g_Render.SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_Render.SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_Render.SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

	g_Render.SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	g_Render.SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

	g_Render.SetRenderState(D3DRS_LIGHTING, FALSE);
	g_Render.SetTexture(1, NULL);

#ifdef MGR
	g_Render.SetTexture(0, _pCurSuf->GetTex());
	_pVBWnd->BeginSet();
	_pVBWnd->DrawSubset(0);
	_pVBWnd->EndSet();
#else
	g_Render.SetTexture(0, _pCurSuf);

	m_pDev->SetStreamSource(0, _pVBWnd, sizeof(M2D_AVER));
	g_Render.SetVertexShader(D3DFVF_M2DWA);

	g_Render.DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);
#endif

	//g_Render.GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLEFAN,2,&_vWndVer,sizeof(M2D_VER));
}

//////////////////////////////////////////////////////////////////////////
CCharacter2D::CCharacter2D(void) {
	_bLoad = false;
	_pModel = NULL;
	//_dwColor = 0xffffffff;
	_vPos = D3DXVECTOR3(0, 0, 0);

	_pChaPart = new stNetTeamChaPart;
	memset(_pChaPart, 0, sizeof(stNetTeamChaPart));

	D3DXVECTOR3 up = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	D3DXVECTOR3 eye = D3DXVECTOR3(0, 1, 0);
	D3DXVECTOR3 target = D3DXVECTOR3(0.0f, 0.0f, 0);
	D3DXMatrixLookAtLH(&_mat3DUIView, &eye, &target, &up);
	D3DXMatrixPerspectiveFovLH(&_mat3DUIProj, D3DX_PI * 0.12f, 1, 1, 1000);
}

CCharacter2D::~CCharacter2D(void) {
	SAFE_DELETE(_pModel);
	delete _pChaPart;
}

void CCharacter2D::Create(RECT rc) {
	/*
	D3DXVECTOR2 vUV[4] = {
		D3DXVECTOR2(0,0),
			D3DXVECTOR2(1,0),	
			D3DXVECTOR2(1,1),
			D3DXVECTOR2(0,1),
	};
	*/

	//_vWndVer[0].m_vPos = D3DXVECTOR4(float(_rcWnd.left),float(_rcWnd.top),0.9f,1);
	//_vWndVer[1].m_vPos = D3DXVECTOR4(float(_rcWnd.right),float(_rcWnd.top),0.9f,1);
	//_vWndVer[2].m_vPos = D3DXVECTOR4(float(_rcWnd.right),float(_rcWnd.bottom),0.9f,1);
	//_vWndVer[3].m_vPos = D3DXVECTOR4(float(_rcWnd.left),float(_rcWnd.bottom),0.9f,1);

	//_vWndVer[0].m_fUV = vUV[0];
	//_vWndVer[1].m_fUV = vUV[1] ;
	//_vWndVer[2].m_fUV = vUV[2] ;
	//_vWndVer[3].m_fUV = vUV[3] ;

	//_vWndVer[0].m_color = _dwColor;
	//_vWndVer[1].m_color = _dwColor;
	//_vWndVer[2].m_color = _dwColor ;
	//_vWndVer[3].m_color = _dwColor ;

	_vp.X = rc.left;
	_vp.Y = rc.top;

	_vp.Width = rc.right - rc.left;
	_vp.Height = rc.bottom - rc.top;
	_vp.MinZ = 0;
	_vp.MaxZ = 1;

	_pModel = new CCharacterModel;
}

void CCharacter2D::MoveWnd(RECT rc) {
	_vp.X = rc.left;
	_vp.Y = rc.top;

	_vp.Width = rc.right - rc.left;
	_vp.Height = rc.bottom - rc.top;
}

void CCharacter2D::Render() {
	if (!_pModel)
		return;


	D3DVIEWPORTX vp;
	g_Render.GetDevice()->GetViewport(&vp);

	_vp.MinZ = vp.MinZ;
	_vp.MaxZ = vp.MaxZ;

	g_Render.GetDevice()->SetViewport(&_vp);

	g_Render.GetDevice()->Clear(0, 0,D3DCLEAR_ZBUFFER, 0, 1, 0);

	g_Render.SetTransformProj(&_mat3DUIProj);
	g_Render.SetTransformView(&_mat3DUIView);

	//g_Render.SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA);	 
	//g_Render.SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//g_Render.SetTransformWorld(&matIdentity);
	//g_Render.SetTransformProj(&_matProj);
	//g_Render.SetTransformView(&_matView);

	//g_Render.SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	//g_Render.SetRenderState(D3DRS_ZENABLE,      FALSE);
	//g_Render.SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	////g_Render.SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL );
	//g_Render.SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );
	//g_Render.SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR );
	//g_Render.SetRenderState(D3DRS_TEXTUREFACTOR, 0xffffffff );

	//DWORD rs_amb;
	//g_Render.GetRenderState(D3DRS_AMBIENT, &rs_amb);
	//g_Render.SetRenderState(D3DRS_AMBIENT, 0xffffffff);

	//DWORD rs_light;
	//g_Render.GetRenderState(D3DRS_LIGHTING, &rs_light);
	//g_Render.SetRenderState(D3DRS_LIGHTING, 0);


	//_pModel->SetLightColor(255, 255,255);
	////lwMatrix44Scale( _pModel->GetMatrix(), 1.0f / (float)1 );

	//_pModel->SetPos((float*)&D3DXVECTOR3(0,0,0));

	//_pModel->FrameMove();
	//_pModel->SetUIScaleDis(1);

	_pModel->SetPos(&_vPos.x);
	_pModel->SetUIYaw(180);
	//_pModel->SetUIScaleDis(1);
	_pModel->RenderUI(400, 300);
	//_pModel->SetPoseVelocity(0.25f);			// Mdr May 2020 FPO Beta

	g_Render.GetDevice()->SetViewport(&vp);
}

void CCharacter2D::LoadCha(DWORD dwID, bool IsMonster) {
	if (!_pModel) {
		return;
	}

	CChaRecord* pInfo = GetChaRecordInfo(dwID);
	if (!pInfo) {
		return;
	}

	CCharacterModel* pCha = _pModel;
	if (pCha == NULL) {
		return;
	}

	pCha->Destroy();


	if (pInfo->chModalType == enumMODAL_MAIN_CHA) {
		DWORD part_buf[5] =
		{
			pInfo->sSkinInfo[0],
			pInfo->sSkinInfo[1],
			pInfo->sSkinInfo[2],
			pInfo->sSkinInfo[3],
			pInfo->sSkinInfo[4],
		};

		if (((CCharacterModel*)pCha)->LoadCha(pInfo->chModalType, pInfo->sModel, part_buf) == 0) {
			return;
		}
	}
	else if (pInfo->chModalType == enumMODAL_BOAT) {
		DWORD part_buf[3] =
		{
			pInfo->sSkinInfo[0],
			pInfo->sSkinInfo[1],
			pInfo->sSkinInfo[2],
		};

		if (((CCharacterModel*)pCha)->LoadShip(pInfo->chModalType, pInfo->sModel, part_buf) == 0) {
			return;
		}
	}
	else if (pInfo->chModalType == enumMODAL_EMPL) {
		DWORD part_buf[5] =
		{
			pInfo->sSkinInfo[0],
			pInfo->sSkinInfo[1],
			pInfo->sSkinInfo[2],
			pInfo->sSkinInfo[3],
		};

		if (((CCharacterModel*)pCha)->LoadTower(pInfo->chModalType, part_buf) == 0) {
			return;
		}
	}
	else {
		MPChaLoadInfo load_info;

		{
			auto r = std::format_to_n(load_info.bone, sizeof(load_info.bone) - 1, "{:04}.lab", pInfo->sModel);
			*r.out = 0;
		}

		for (DWORD i = 0; i < 5; i++) {
			if (pInfo->sSkinInfo[i] == 0)
				continue;

			DWORD file_id = pInfo->sModel * 1000000 + pInfo->sSuitID * 10000 + i;
			auto r = std::format_to_n(load_info.part[i], sizeof(load_info.part[i]) - 1, "{:010}.lgo", file_id);
			*r.out = 0;
		}

		if (((CCharacterModel*)pCha)->LoadCha(&load_info) == 0) {
			return;
		}
	}

	if (((CCharacterModel*)pCha)->LoadPose(pInfo->sActionID) == 0) {
		return;
	}
	_vPos.z = -pInfo->fHeight;
	//pCha->SetYaw( D3DX_PI );
	if (pInfo->sModel == 139) pCha->PlayPose(1, PLAY_LOOP, -1, 32, 1, 5, true);
	else
		pCha->PlayPose(1, PLAY_LOOP);

	/*
	float fz[] = 
	{
		0.4f,0.3f,0.2f,0.3f,
	};

	int w = _vp.Width;
	int h = _vp.Height;
	*/


	SClientAttr* pChaAttr = GetClientAttr(dwID);

	if (!pChaAttr->sTeamAngle) {
		pChaAttr = GetClientAttr(0);
	}

	D3DXVECTOR3 up = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	//D3DXVECTOR3	eye = D3DXVECTOR3( -0.6f,1.2f, -fz[dwID - 1] +0.3f);
	//D3DXVECTOR3	target = D3DXVECTOR3( 0.0f, 0.0f,-fz[dwID - 1] -0.1f);

	D3DXVECTOR3 eye(0, 1, 0);
	D3DXMATRIX mat;
	D3DXMatrixRotationZ(&mat, float(pChaAttr->sTeamAngle) * 0.01745329f);
	D3DXVec3TransformCoord(&eye, &eye, &mat);
	eye *= pChaAttr->fTeamDis;
	eye.z = 0.4f;
	_vPos.z += pChaAttr->fTeamHei;
	D3DXVECTOR3 target = D3DXVECTOR3(0.0f, 0.0f, 0);

	D3DXMatrixLookAtLH(&_mat3DUIView, &eye, &target, &up);
	if (IsMonster) {
		D3DXMatrixPerspectiveFovLH(&_mat3DUIProj, D3DX_PI * 0.14f, 1, 1, 1000);
	}
	else {
		D3DXMatrixPerspectiveFovLH(&_mat3DUIProj, D3DX_PI * 0.12f, 1, 1, 1000);
	}
	_bLoad = true;
}

void CCharacter2D::UpdataFace(stNetTeamChaPart& stPart, bool IsPlayer) {
	if (stPart.sTypeID != _pChaPart->sTypeID)
		LoadCha(stPart.sTypeID);

	// Mdr May 2020 - FPO Beta
	// This is a mess but seems to work, so we'll keep it that way until something burns down
	int nHairID;
	if (stPart.SLink[enumEQUIP_HEADAPP].sID != 0 && CCharacter::GetIsShowApparel()) nHairID = stPart.SLink[
		enumEQUIP_HEADAPP].sID;
	else nHairID = stPart.SLink[enumEQUIP_HEAD].sID != 0 ? stPart.SLink[enumEQUIP_HEAD].sID : stPart.sHairID;

	int nSelfID;
	if (_pChaPart->SLink[enumEQUIP_HEADAPP].sID != 0 && CCharacter::GetIsShowApparel()) nSelfID = _pChaPart->SLink[
		enumEQUIP_HEADAPP].sID;
	else nSelfID = _pChaPart->SLink[enumEQUIP_HEAD].sID != 0
					   ? _pChaPart->SLink[enumEQUIP_HEAD].sID
					   : _pChaPart->sHairID;

	if (nHairID != nSelfID || (_pChaPart->SLink[enumEQUIP_HEAD].sID == 0 && !CCharacter::GetIsShowApparel())) {
		_pModel->ChangePart(0, nHairID);
	}

	const int nMaxPart = 5;
	for (int i = 0; i < nMaxPart; i++) {
		int ID = stPart.SLink[i].sID;
		if (ID == 0 && i && (!stPart.SLink[i + 19].sID || !CCharacter::GetIsShowApparel()) && IsPlayer) {
			if (CGameScene::GetMainCha())
				ID = CGameScene::GetMainCha()->GetDefaultChaInfo()->sSkinInfo[i];
		}
		else if (CCharacter::GetIsShowApparel()) {
			if (stPart.SLink[i + 19].sID != 0 && ID != 0) {
				ID = stPart.SLink[i + 19].sID;
			}
		}
		_pModel->ChangePart(i, ID);
	}

	int nForgeLv = 0;
	for (int i = 0; i < nMaxPart; i++) {
		if (stPart.SLink[i].chForgeLv == _pChaPart->SLink[i].chForgeLv) continue;

		_pModel->Unlit(i);
		if (stPart.SLink[i].sID > 0) {
			nForgeLv = GetItemForgeLv(stPart.SLink[i].chForgeLv);
			if (nForgeLv >= 0) {
				_pModel->Lit(i, nForgeLv);
			}
		}
	}

	memcpy(_pChaPart, &stPart, sizeof(stNetTeamChaPart));
}

//////////////////////////////////////////////////////////////////////////
CBigMap::CBigMap() {
	_dwColor = 0xffffffff;

	D3DXVECTOR2 vUV[4] = {
		D3DXVECTOR2(0, 0),
		D3DXVECTOR2(1, 0),
		D3DXVECTOR2(1, 1),
		D3DXVECTOR2(0, 1),
	};
	_vWndVer[0].m_fUV = vUV[0];
	_vWndVer[1].m_fUV = vUV[1];
	_vWndVer[2].m_fUV = vUV[2];
	_vWndVer[3].m_fUV = vUV[3];

	_vWndVer[0].m_color = _dwColor;
	_vWndVer[1].m_color = _dwColor;
	_vWndVer[2].m_color = _dwColor;
	_vWndVer[3].m_color = _dwColor;

	for (int e = 0; e < 4; ++e) {
		_vWndVer[e].m_vPos.z = 0.9f;
		_vWndVer[e].m_vPos.w = 1;
	}
	_pTex = NULL;
}

CBigMap::~CBigMap() {
	Destory();
}

void CBigMap::Create() {
	CGameScene* pScene = g_pGameApp->GetCurScene();
	MPTerrain* pTerr = pScene->GetTerrain();
	if (!pTerr)
		return;
	//int w = pTerr->GetWidth();
	//int h = pTerr->GetHeight();

	//int srcw = g_Render.GetScrWidth();
	//int srch = g_Render.GetScrHeight();

	//float flerpx = (float)srcw / (float)w;
	//float flerpy = (float)srch / (float)h;

	//_rcW = (int)(256 * flerpx);
	//_rcH = (int)(256 * flerpy);


	_rcW = g_Render.GetScrWidth();
	_rcH = g_Render.GetScrHeight();
	wc = _rcW / 4;
	hc = _rcH / 3;


	_pTex = new MPITex*[4 * 3];

	for (int m = 0; m < 3; ++m) {
		for (int n = 0; n < 4; ++n) {
			const std::string filename = std::format("texture\\bigmap\\{}{}.bmp", m, n + 1);
			lwLoadTex(&_pTex[m * 4 + n], g_Render.GetInterfaceMgr()->res_mgr,
					  filename.c_str(), 0, D3DFMT_A8R8G8B8);
		}
	}
}

void CBigMap::Destory() {
	for (int n = 0; n < 4 * 3; n++) {
		SAFE_RELEASE(_pTex[n]);
	}
	SAFE_DELETE_ARRAY(_pTex);
}

void CBigMap::Render() {
	if (!_pTex)
		return;
	RECT rc;
	rc.left = 0;
	rc.top = 0;
	rc.right = wc;
	rc.bottom = hc;

	M2D_AVER vVer[4];

	vVer[0].m_vPos = D3DXVECTOR4(float(rc.left), float(rc.top), 0.9f, 1);
	vVer[1].m_vPos = D3DXVECTOR4(float(rc.right), float(rc.top), 0.9f, 1);
	vVer[2].m_vPos = D3DXVECTOR4(float(rc.right), float(rc.bottom), 0.9f, 1);
	vVer[3].m_vPos = D3DXVECTOR4(float(rc.left), float(rc.bottom), 0.9f, 1);


	g_Render.SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	g_Render.SetRenderState(D3DRS_ZENABLE, FALSE);
	g_Render.SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	g_Render.SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	g_Render.SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	g_Render.SetRenderState(D3DRS_TEXTUREFACTOR, 0xffffffff);

	g_Render.SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW); // 

	g_Render.SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	g_Render.SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

	for (int m = 0; m < 3; ++m) {
		for (int n = 0; n < 4; ++n) {
			for (int e = 0; e < 4; ++e) {
				_vWndVer[e].m_vPos.x = vVer[e].m_vPos.x + wc * n;
				_vWndVer[e].m_vPos.y = vVer[e].m_vPos.y + hc * m;
			}
			g_Render.SetVertexShader(NULL);
			g_Render.SetFVF(D3DFVF_M2DWA);
			g_Render.SetTexture(0, _pTex[m * 4 + n]->GetTex());
			g_Render.GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, &_vWndVer, sizeof(M2D_AVER));
		}
	}
	g_Render.SetRenderState(D3DRS_ZENABLE, TRUE);
	g_Render.SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	g_Render.SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_Render.SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

	g_Render.SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	g_Render.SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
}

//////////////////////////////////////////////////////////////////////////
Ctemp::Ctemp() {
	RECT rc;
	rc.left = 10;
	rc.top = 10;
	rc.right = 70;
	rc.bottom = 70;

	_vWndVer[0].m_vPos = D3DXVECTOR4(float(rc.left), float(rc.top), 0.9f, 1);
	_vWndVer[1].m_vPos = D3DXVECTOR4(float(rc.right), float(rc.top), 0.9f, 1);
	_vWndVer[2].m_vPos = D3DXVECTOR4(float(rc.right), float(rc.bottom), 0.9f, 1);
	_vWndVer[3].m_vPos = D3DXVECTOR4(float(rc.left), float(rc.bottom), 0.9f, 1);

	D3DXVECTOR2 vUV[4] = {
		D3DXVECTOR2(0, 0),
		D3DXVECTOR2(1, 0),
		D3DXVECTOR2(1, 1),
		D3DXVECTOR2(0, 1),
	};
	_vWndVer[0].m_fUV = vUV[0];
	_vWndVer[1].m_fUV = vUV[1];
	_vWndVer[2].m_fUV = vUV[2];
	_vWndVer[3].m_fUV = vUV[3];

	_vWndVer[0].m_color = 0xff000000;
	_vWndVer[1].m_color = 0xff000000;
	_vWndVer[2].m_color = 0xff000000;
	_vWndVer[3].m_color = 0xff000000;

	_pTex = NULL;
}

Ctemp::~Ctemp() {
	SAFE_RELEASE(_pTex);
}

void Ctemp::Render() {
	if (!_pTex) {
		//_pTex = ui::GetFont(0)->GetTexture(); //FontManager::Instance().Get(FontSlot::TipText)->GetTexture();

		lwLoadTex(&_pTex, g_Render.GetInterfaceMgr()->res_mgr,
				  "texture\\ui\\minimap\\mapmask.bmp", 0, D3DFMT_A8R8G8B8);
	}


	g_Render.SetRenderStateForced(D3DRS_ALPHABLENDENABLE, FALSE);

	g_Render.SetRenderState(D3DRS_ZENABLE, FALSE);
	g_Render.SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	g_Render.SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	g_Render.SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	g_Render.SetRenderState(D3DRS_TEXTUREFACTOR, 0xffffffff);

	g_Render.SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW); // 

	g_Render.SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	g_Render.SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

	g_Render.SetVertexShader(NULL);
	g_Render.SetFVF(D3DFVF_M2DWA);

	g_Render.SetTexture(0, _pTex->GetTex());


	g_Render.GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, &_vWndVer, sizeof(M2D_AVER));

	g_Render.SetRenderState(D3DRS_ZENABLE, TRUE);
	g_Render.SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	g_Render.SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_Render.SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

	g_Render.SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	g_Render.SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
}

//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
void CMinimap::InitScene() {
	MPIResourceMgr* res_mgr = g_Render.GetInterfaceMgr()->res_mgr;

	//if(FAILED(lwLoadTex(&_pTexMask,res_mgr, "texture\\minimap\\mapmask.bmp", 0, D3DFMT_A8R8G8B8)))
	//{
	//	LG("ERROR","msgmapmask.bmp");
	//}
	if (FAILED(lwLoadTex(&_pTexDefault,res_mgr, "texture\\ui\\minimap\\mapsea.bmp", 0, D3DFMT_A8R8G8B8))) {
		ToLogService("errors", LogLevel::Error, "msgmapsea.bmp");
	}

	RECT rc;
	rc.left = _rcWnd.left;
	rc.top = _rcWnd.top;
	rc.right = _rcWnd.right;
	rc.bottom = _rcWnd.bottom;
	_vWndVer[0].m_vPos = D3DXVECTOR4(float(rc.left), float(rc.top), 0.9f, 1);
	_vWndVer[1].m_vPos = D3DXVECTOR4(float(rc.right), float(rc.top), 0.9f, 1);
	_vWndVer[2].m_vPos = D3DXVECTOR4(float(rc.right), float(rc.bottom), 0.9f, 1);
	_vWndVer[3].m_vPos = D3DXVECTOR4(float(rc.left), float(rc.bottom), 0.9f, 1);

	rc.left = 0;
	rc.top = 0;
	rc.right = 64;
	rc.bottom = 64;
	_vPicVer[0].m_vPos = D3DXVECTOR4(float(rc.left), float(rc.top), 0.9f, 1);
	_vPicVer[1].m_vPos = D3DXVECTOR4(float(rc.right), float(rc.top), 0.9f, 1);
	_vPicVer[2].m_vPos = D3DXVECTOR4(float(rc.right), float(rc.bottom), 0.9f, 1);
	_vPicVer[3].m_vPos = D3DXVECTOR4(float(rc.left), float(rc.bottom), 0.9f, 1);

	D3DXVECTOR2 vUV[4] = {
		D3DXVECTOR2(0, 0),
		D3DXVECTOR2(1, 0),
		D3DXVECTOR2(1, 1),
		D3DXVECTOR2(0, 1),
	};

	for (int n = 0; n < 4; n++) {
		_vWndVer[n].m_fUV = vUV[n];
		_vWndVer[n].m_color = 0xffffffff;

		_vPicVer[n].m_fUV = vUV[n];
		_vPicVer[n].m_color = 0xffffffff;
	}

	_cSmNpc.Init();

	m_pScene = g_pGameApp->GetCurScene();
	if (m_pScene) {
		const std::string filename = std::format("texture\\minimap\\{}\\{}.pk",
				m_pScene->GetTerrainName(), m_pScene->GetTerrainName());
		SAFE_DELETE(_pMiniPack);

		if (_access(filename.c_str(), 0) != -1) {
			_pMiniPack = new CMiniPack;
			if (!_pMiniPack->Init((char*)m_pScene->GetTerrainName()))
				delete _pMiniPack;
		}
	}
	//if(!CMaskData::g_MaskData && m_pScene && m_pScene->GetTerrain())
	//{
	//	CMaskData::g_MaskData = new CMaskData;
	//	if(!CMaskData::g_MaskData->InitMaskData((char*)m_pScene->GetTerrainName(),\
	//		m_pScene->GetTerrain()->GetWidth(),m_pScene->GetTerrain()->GetHeight()))
	//		SAFE_DELETE(CMaskData::g_MaskData);
	//}
	_bUpdate = TRUE;
}

void CMinimap::RenderScene() {
	if (!m_pScene->GetMainCha())
		return;
	//::SetCurrentDirectory(pszAppPath);
	//g_Render.GetDevice()->Clear(0,0,D3DCLEAR_TARGET,0x00000000,0,0);

	m_pScene = g_pGameApp->GetCurScene();
	g_Render.SetRenderState(D3DRS_ZENABLE, FALSE);
	g_Render.SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	g_Render.SetRenderState(D3DRS_CLIPPING, TRUE);

	g_Render.SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	g_Render.SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	g_Render.SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_Render.SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	g_Render.SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
	g_Render.SetRenderState(D3DRS_DITHERENABLE,FALSE);
	g_Render.SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW); // 
	//
	g_Render.SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	g_Render.SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

	g_Render.SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_Render.SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	g_Render.SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	g_Render.SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	g_Render.SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	g_Render.SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

	g_Render.SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	g_Render.SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);

	g_Render.SetRenderState(D3DRS_TEXTUREFACTOR, 0xffffffff);
	g_Render.SetRenderState(D3DRS_LIGHTING, FALSE);

	g_Render.SetVertexShader(NULL);
	g_Render.SetFVF(D3DFVF_M2DWA);

	int tnx = m_pScene->GetMainCha()->GetCurX();
	int tny = m_pScene->GetMainCha()->GetCurY();

	int sx = (int)(tnx / (SHOWRSIZE * 100));
	int sy = (int)(tny / (SHOWRSIZE * 100));

	int irange = 0;
	int rx = (tnx / 100) - sx * (SHOWRSIZE);
	int ry = (tny / 100) - sy * (SHOWRSIZE);

	//if(rx < irange)
	//{
	//	CMaskData::g_MaskData->SetMask(sx - 1,sy);
	//	if(ry < irange)
	//		CMaskData::g_MaskData->SetMask(sx - 1,sy - 1);
	//	if(ry > SHOWRSIZE - irange)
	//		CMaskData::g_MaskData->SetMask(sx - 1,sy + 1);
	//}else if(rx > SHOWRSIZE - irange)
	//{
	//	CMaskData::g_MaskData->SetMask(sx + 1,sy);
	//	if(ry > SHOWRSIZE - irange)
	//		CMaskData::g_MaskData->SetMask(sx + 1,sy + 1);
	//	if(ry < irange)
	//		CMaskData::g_MaskData->SetMask(sx + 1,sy - 1);
	//}else if(ry < irange)
	//{
	//	CMaskData::g_MaskData->SetMask(sx,sy - 1);
	//}else if(ry > SHOWRSIZE - irange)
	//{
	//	CMaskData::g_MaskData->SetMask(sx,sy + 1);
	//}
	//if(CMaskData::g_MaskData)
	//	CMaskData::g_MaskData->SetMask(sx,sy);

	int sxue = (int)(tnx - sx * (SHOWRSIZE * 100)) - 32 * 100;
	int syue = (int)(tny - sy * (SHOWRSIZE * 100)) - 32 * 100;

	/*if(sx > 0) */
	sx -= VIEWRANGE / 2;
	/*if(sy > 0) */
	sy -= VIEWRANGE / 2;

	int lenw = (_rcWnd.right - _rcWnd.left) / 2;

	_sx = sx;
	_sy = sy;
	/////
	//	int tempx =  -32;
	//	int tempy =  -32;
	//
	//	RECT rc;
	//	int m;
	//
	//	D3DXVECTOR2 vUVwnd[4] = {
	//		D3DXVECTOR2(0,0),
	//			D3DXVECTOR2(1,0),	
	//			D3DXVECTOR2(1,1),
	//			D3DXVECTOR2(0,1),
	//	};
	//	_vWndVer[0].m_fUV = vUVwnd[0];
	//	_vWndVer[1].m_fUV = vUVwnd[1] ;
	//	_vWndVer[2].m_fUV = vUVwnd[2] ;
	//	_vWndVer[3].m_fUV = vUVwnd[3] ;
	//
	//	for( m = 0; m < 3; m++)
	//	{
	//		rc.top =  tempy + m * 64;
	//		rc.bottom =  rc.top + 64 - 1;
	//
	//		rc.left = tempx;
	//		rc.right = rc.left + 64;
	//
	//		for(int n = 0; n < 3; n++)
	//		{
	//
	//			if(_access(filename,0)== -1)
	//			{
	//				g_Render.SetTexture(0,_pTexDefault->GetTex());
	//			}else
	//			{
	//				if((_pTex[n][m].x != _sx + n) || (_pTex[n][m].y != _sy + m))
	//				{
	//					_pTex[n][m].ReleaseTex();
	//
	//					lwLoadTex(&_pTex[n][m].pTex,g_Render.GetInterfaceMgr()->res_mgr, 
	//						filename, 0, D3DFMT_A8R8G8B8);
	//					if(_pTex[n][m].pTex)
	//					{
	//						_pTex[n][m].x	= _sx + n;
	//						_pTex[n][m].y	= _sy + m;
	//					}
	//				}
	//				if(_pTex[n][m].pTex)
	//					g_Render.SetTexture(0, _pTex[n][m].pTex->GetTex()); 
	//				else
	//					g_Render.SetTexture(0, NULL); 
	//			}
	//			rc.left =  tempx + n * 64;
	//			rc.right =  rc.left + 64 - 1;
	//
	//			_vPicVer[0].m_vPos = D3DXVECTOR4(float(rc.left),float(rc.top),0.9f,1);
	//			_vPicVer[1].m_vPos = D3DXVECTOR4(float(rc.right),float(rc.top),0.9f,1);
	//			_vPicVer[2].m_vPos = D3DXVECTOR4(float(rc.right),float(rc.bottom),0.9f,1);
	//			_vPicVer[3].m_vPos = D3DXVECTOR4(float(rc.left),float(rc.bottom),0.9f,1);
	//
	//			g_Render.GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLEFAN,2,&_vPicVer,sizeof(M2D_AVER));
	//		}
	//	}

	//int tempx =  int((-32 * 100 - sxue) / 100 * 1.6f);
	//int tempy =  int((-32 * 100 - syue) / 100 * 1.6f);
	int tempx = int((-32 * 100 - sxue) / 100 * 1.6f);
	int tempy = int((-32 * 100 - syue) / 100 * 1.6f);

	tempx -= (64 * (VIEWRANGE / 2 - 1));
	tempy -= (64 * (VIEWRANGE / 2 - 1));


	tempx += (lenw * 2 - 128) / 2;
	tempy += (lenw * 2 - 128) / 2;


	RECT rc, rct;
	int m;

	for (m = 0; m < VIEWRANGE; m++) {
		rc.top = tempy + m * 64;
		rc.bottom = rc.top + 64;

		rc.left = tempx;
		rc.right = rc.left + 64;

		for (int n = 0; n < VIEWRANGE; n++) {
			if (_sx + n < 0 || _sy + m < 0) {
				g_Render.SetTexture(0, _pTexDefault->GetTex());
			}
			else {
				if (!_pMiniPack) {
					const std::string filename = std::format("texture\\minimap\\{}\\sm_{}_{}.bmp",
							m_pScene->GetTerrainName(), _sx + n, _sy + m);

					if (_access(filename.c_str(), 0) == -1) {
						g_Render.SetTexture(0, _pTexDefault->GetTex());
					}
					else {
						if ((_pTex[n][m].x != _sx + n) || (_pTex[n][m].y != _sy + m)) {
							_pTex[n][m].ReleaseTex();

							lwLoadTex(&_pTex[n][m].pTex, g_Render.GetInterfaceMgr()->res_mgr,
									  filename.c_str(), 0, D3DFMT_A8R8G8B8);
							if (_pTex[n][m].pTex) {
								_pTex[n][m].x = _sx + n;
								_pTex[n][m].y = _sy + m;
							}
						}
						if (_pTex[n][m].pTex)
							g_Render.SetTexture(0, _pTex[n][m].pTex->GetTex());
						else
							g_Render.SetTexture(0, NULL);
					}
				}
				else {
					if (_pMiniPack->IsExit(_sx + n, _sy + m)) {
						if ((_pTex[n][m].x != _sx + n) || (_pTex[n][m].y != _sy + m)) {
							_pTex[n][m].ReleaseTex();

							_pMiniPack->GetTex(&_pTex[n][m].pTex, _sx + n, _sy + m);

							if (_pTex[n][m].pTex) {
								_pTex[n][m].x = _sx + n;
								_pTex[n][m].y = _sy + m;
							}
						}
						if (_pTex[n][m].pTex)
							g_Render.SetTexture(0, _pTex[n][m].pTex->GetTex());
						else
							g_Render.SetTexture(0, NULL);
					}
					else
						g_Render.SetTexture(0, _pTexDefault->GetTex());
				}
			}
			rc.left = tempx + n * 64;
			rc.right = rc.left + 64;

			rct.left = rc.left + _rcWnd.left;
			rct.top = rc.top + _rcWnd.top;
			rct.right = rct.left + 64;
			rct.bottom = rct.top + 64;

			_vPicVer[0].m_vPos = D3DXVECTOR4(float(rct.left), float(rct.top), 0.9f, 1);
			_vPicVer[1].m_vPos = D3DXVECTOR4(float(rct.right), float(rct.top), 0.9f, 1);
			_vPicVer[2].m_vPos = D3DXVECTOR4(float(rct.right), float(rct.bottom), 0.9f, 1);
			_vPicVer[3].m_vPos = D3DXVECTOR4(float(rct.left), float(rct.bottom), 0.9f, 1);

			g_Render.GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, &_vPicVer, sizeof(M2D_AVER));
		}
	}

	//////////////////////////////////////////////////////////////////////////
	g_Render.SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	//g_Render.SetTextureStageState(0,D3DTSS_ADDRESSU, D3DTADDRESS_WRAP);
	//g_Render.SetTextureStageState(0,D3DTSS_ADDRESSV, D3DTADDRESS_WRAP);
	g_Render.SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_Render.SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

	_cSmNpc.Begin();
	CCharacter* pCha = NULL;
	CCharacter* pMainCha = m_pScene->GetMainCha();

	int nMainTeadID = pMainCha->GetTeamLeaderID();
	long lSideID = pMainCha->getSideID();
	int nChaCnt = m_pScene->GetChaCnt();
	for (m = 0; m < nChaCnt; m++) {
		pCha = m_pScene->GetCha(m);
		if (pCha->getEvent())
			continue;

		if (pCha->GetIsMount())
			continue;

		if (pCha && pCha->IsInMiniMap() && pCha != pMainCha) {
			if (PosInRange(pCha->GetPos(), _vEyePt, SHOWRSIZE)) {
				_cSmNpc.setTexture(0);
				_cSmNpc.setPos(pCha->GetCurX(), pCha->GetCurY(), tnx, tny,
							   _rcWnd.left, _rcWnd.top, lenw);

				if (pCha->IsNPC()) {
					_cSmNpc.setScaling(NPCSIZE,NPCSIZE);
					_cSmNpc.setAngle(180);
					_cSmNpc.setColor(0xffffffff);

					if (pCha->getNpcType() > 0) {
						_cSmNpc.setTexture(pCha->getNpcType());
					}
					else {
						_cSmNpc.setTexture(10);
					}
				}
				else {
					_cSmNpc.setScaling(10, 10);
					_cSmNpc.setAngle(0);

					if (pCha->IsPlayer()) {
						if (lSideID != 0) {
							if (pCha->getSideID() == lSideID)
								_cSmNpc.setColor(D3DCOLOR_ARGB(255, 0, 255, 0));
							else
								_cSmNpc.setColor(0xffff0000);
						}
						else if (nMainTeadID != 0) {
							if (nMainTeadID == pCha->GetTeamLeaderID())
								_cSmNpc.setColor(D3DCOLOR_ARGB(255, 0, 255, 0));
							else
								_cSmNpc.setColor(D3DCOLOR_ARGB(255, 0, 220, 220));
						}
						else if (pCha->GetIsPet()) {
							_cSmNpc.setColor(D3DCOLOR_ARGB(255, 255, 0, 255));
						}
						else {
							_cSmNpc.setColor(D3DCOLOR_ARGB(255, 0, 220, 220));
						}
					}
					else if (pCha->IsResource()) {
						_cSmNpc.setColor(D3DCOLOR_ARGB(255, 255, 180, 0));
					}
					else {
						_cSmNpc.setColor(0xffff0000);
					}
				}

				_cSmNpc.Render();
			}
		}
	}

	_cSmNpc.setTexture(TEXNUM - 2);
	_cSmNpc.setPos(lenw, lenw, _rcWnd.left, _rcWnd.top);
	_cSmNpc.setScaling(10, 20);
	_cSmNpc.setAngle(_iAngle);
	_cSmNpc.setColor(0xffffffff);
	_cSmNpc.Render();

	//////////////////////////////////////////////////////////////////////////
	_vCameraTar[0] = D3DXVECTOR4(float(lenw + _rcWnd.left), float(lenw + 24 + _rcWnd.top), 0, 1);

	g_Render.SetTexture(0, NULL);
	D3DXMATRIX matIden;

	D3DXVECTOR3 v[] = {
		D3DXVECTOR3(float(lenw + _rcWnd.left), float(lenw + _rcWnd.top), 0),
		D3DXVECTOR3(0, 0, 1)
	};

	GetMatrixRotation(&matIden, &v[0], &v[1], _fCameraAngle);
	D3DXVec4Transform(&_vCameraTar[0], &_vCameraTar[0], &matIden);

	_vCameraTar[1] = D3DXVECTOR4(float(lenw / 2 + 6 + _rcWnd.left), float(lenw / 2 + 6 + _rcWnd.top), 0, 1);
	D3DXVec4Transform(&_vCameraTar[1], &_vCameraTar[1], &matIden);

	g_Render.SetVertexShader(NULL);
	g_Render.SetFVF(D3DFVF_XYZRHW);
	g_Render.SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	g_Render.SetRenderState(D3DRS_TEXTUREFACTOR, 0xffffffff);
	//g_Render.SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
	//g_Render.SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR);

	g_Render.SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
	g_Render.SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG2);

	g_Render.GetDevice()->DrawPrimitiveUP(D3DPT_LINELIST, 1, &_vCameraTar, sizeof(D3DXVECTOR4));

	_vCameraTar[1] = D3DXVECTOR4(float(lenw + 26 + _rcWnd.left), float(lenw / 2 + 6 + _rcWnd.top), 0, 1);
	D3DXVec4Transform(&_vCameraTar[1], &_vCameraTar[1], &matIden);

	g_Render.GetDevice()->DrawPrimitiveUP(D3DPT_LINELIST, 1, &_vCameraTar, sizeof(D3DXVECTOR4));

	g_Render.SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	g_Render.SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	g_Render.SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
}

void CMinimap::RenderMask() {
	g_Render.SetRenderState(D3DRS_ZENABLE, FALSE);
	g_Render.SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	g_Render.SetRenderState(D3DRS_CLIPPING, TRUE);

	g_Render.SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	g_Render.SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	//g_Render.SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA);	 
	//g_Render.SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	g_Render.SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
	g_Render.SetRenderState(D3DRS_DITHERENABLE,FALSE);
	g_Render.SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW); // 

	g_Render.SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	g_Render.SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

	g_Render.SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_Render.SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	g_Render.SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	g_Render.SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	g_Render.SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	g_Render.SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

	//g_Render.SetTextureStageState(1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE);

	//g_Render.SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);  
	//g_Render.SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_DIFFUSE);  
	//g_Render.SetTextureStageState(1, D3DTSS_COLOROP,   D3DTOP_MODULATE);  

	g_Render.SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	g_Render.SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	//g_Render.SetTextureStageState( 1, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);		
	//g_Render.SetTextureStageState( 1, D3DTSS_MINFILTER, D3DTEXF_LINEAR);

	g_Render.SetRenderState(D3DRS_LIGHTING, FALSE);

	//g_Render.SetTexture(0, _pTexMask->GetTex()); 
	g_Render.SetTexture(0, _pCurSuf->GetTex());
	g_Render.SetVertexShader(NULL);
	g_Render.SetFVF(D3DFVF_M2DWA);

	g_Render.GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, &_vWndVer, sizeof(M2D_AVER));

	//g_Render.SetTexture(1, NULL); 
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void CLargerMap::InitScene() {
	MPIResourceMgr* res_mgr = g_Render.GetInterfaceMgr()->res_mgr;

	if (FAILED(lwLoadTex(&_pTexDefault,res_mgr, "texture\\ui\\minimap\\mapsea.bmp", 0, D3DFMT_A8R8G8B8))) {
		ToLogService("errors", LogLevel::Error, "msgmapsea.bmp");
	}
	//if(FAILED(lwLoadTex(&_pTexMask,res_mgr, "texture\\ui\\minimap\\mask.tga", 0, D3DFMT_A8R8G8B8)))
	//{
	//	LG("ERROR","msgmask.tga");
	//}
	RECT rc;
	rc.left = _rcWnd.left;
	rc.top = _rcWnd.top;
	rc.right = _rcWnd.right;
	rc.bottom = _rcWnd.bottom;
	_vWndVer[0].m_vPos = D3DXVECTOR4(float(rc.left), float(rc.top), 0.9f, 1);
	_vWndVer[1].m_vPos = D3DXVECTOR4(float(rc.right), float(rc.top), 0.9f, 1);
	_vWndVer[2].m_vPos = D3DXVECTOR4(float(rc.right), float(rc.bottom), 0.9f, 1);
	_vWndVer[3].m_vPos = D3DXVECTOR4(float(rc.left), float(rc.bottom), 0.9f, 1);

	rc.left = 0;
	rc.top = 0;
	rc.right = 64;
	rc.bottom = 64;
	_vPicVer[0].m_vPos = D3DXVECTOR4(float(rc.left), float(rc.top), 0.9f, 1);
	_vPicVer[1].m_vPos = D3DXVECTOR4(float(rc.right), float(rc.top), 0.9f, 1);
	_vPicVer[2].m_vPos = D3DXVECTOR4(float(rc.right), float(rc.bottom), 0.9f, 1);
	_vPicVer[3].m_vPos = D3DXVECTOR4(float(rc.left), float(rc.bottom), 0.9f, 1);

	D3DXVECTOR2 vUV[4] = {
		D3DXVECTOR2(0, 0),
		D3DXVECTOR2(1, 0),
		D3DXVECTOR2(1, 1),
		D3DXVECTOR2(0, 1),
	};

	for (int n = 0; n < 4; n++) {
		_vWndVer[n].m_fUV = vUV[n];
		_vWndVer[n].m_color = 0xffffffff;

		_vPicVer[n].m_fUV = vUV[n];
		_vPicVer[n].m_color = 0xffffffff;
	}

	_cSmNpc.Init();


	_TexWnum = (_rcWnd.right - _rcWnd.left) / TILE_SIZE;
	_TexHnum = (_rcWnd.bottom - _rcWnd.top) / TILE_SIZE;

	/*if(!(_TexWnum & 2))*/
	_TexWnum += 2;
	/*if(!(_TexHnum & 2))*/
	_TexHnum += 2;

	//_pTex = new MPITex*[_TexWnum * _TexHnum];
	_pTex.resize(_TexWnum * _TexHnum);
	int m;
	for (m = 0; m < _TexHnum * _TexWnum; m++) {
		_pTex[m] = NULL;
	}

	vecRect.resize(_TexWnum * _TexHnum);
	vecColor.resize((_TexWnum + 1) * (_TexHnum + 1));
	memset(&vecColor.front(), 0, sizeof(DWORD) * (_TexWnum + 1) * (_TexHnum + 1));

	m_pScene = g_pGameApp->GetCurScene();
	if (m_pScene) {
		const std::string filename = std::format("texture\\minimap\\{}\\{}.pk",
				m_pScene->GetTerrainName(), m_pScene->GetTerrainName());

		SAFE_DELETE(_pMiniPack);
		if (_access(filename.c_str(), 0) != -1) {
			_pMiniPack = new CMiniPack;
			if (!_pMiniPack->Init((char*)m_pScene->GetTerrainName()))
				delete _pMiniPack;
		}
	}
	if (!CMaskData::g_MaskData && m_pScene && m_pScene->GetTerrain()) {
		CMaskData::g_MaskData = new CMaskData;
	}
	_bUpdate = FALSE;
}

void CLargerMap::Show(bool bShow) {
	if (bShow == _bUpdate)
		return;

	if (!bShow) {
		_bUpdate = bShow;
		int m;
		for (m = 0; m < _TexHnum * _TexWnum; m++) {
			if (_pTex[m] == _pTexDefault || _pTex[m] == _pTexMask) {
				_pTex[m] = NULL;
			}
			else {
				SAFE_RELEASE(_pTex[m]);
			}
		}
		memset(&vecColor.front(), 0, sizeof(DWORD) * (_TexWnum + 1) * (_TexHnum + 1));

		/*	for( m = 0; m < _TexHnum; m++)
			{
				for(int n = 0; n < _TexWnum; n++)
				{
					idx = m * _TexHnum + n;
					if(_pTex[idx] == _pTexDefault)
					{
						_pTex[idx] = NULL;
					}else
					{
						SAFE_RELEASE(_pTex[idx]);
					}
				}
			}*/
		//_pTex.clear();

		return;
	}

	if (!CMaskData::g_MaskData)
		return;

	m_pScene = CGameApp::GetCurScene();
	if (!m_pScene || !CGameScene::GetMainCha())
		return;

	_nInitX = m_pScene->GetMainCha()->GetCurX();
	_nInitY = m_pScene->GetMainCha()->GetCurY();
	Update(_nInitX, _nInitY);
	_bUpdate = (bool)bShow;
}

//Modify by sunny.sun 20080910 for bigmap
//Begin
void CLargerMap::Update(int x, int y) {
	x = std::clamp(x, 0, defMAP_GARNER_WIDTH * 100);
	y = std::clamp(y, 0, defMAP_GARNER_HEIGHT * 100);

	_nCenterX = x;
	_nCenterY = y;

	const int tnx = x;
	const int tny = y;

	const int sx = (int)(tnx / (SHOWRSIZE * 100));
	const int sy = (int)(tny / (SHOWRSIZE * 100));

	const float sxue = ((float)(tnx - sx * (SHOWRSIZE * 100))) / (float(SHOWRSIZE * 100));
	const float syue = ((float)(tny - sy * (SHOWRSIZE * 100))) / (float(SHOWRSIZE * 100));

	//if(sx > _TexWnum / 2) sx -= _TexWnum / 2;
	//if(sy > _TexHnum / 2) sy -= _TexHnum / 2;
	_sx = sx;
	_sy = sy;

	_sx -= _TexWnum / 2;
	_sy -= _TexHnum / 2;


	const int lenw = (_rcWnd.right - _rcWnd.left) / 2;
	const int lenh = (_rcWnd.bottom - _rcWnd.top) / 2;


	const auto SCALED_TILE_SIZE = TILE_SIZE * map_scale;

	const int tempx = (lenw) - ((_TexWnum / 2 * SCALED_TILE_SIZE) + int(sxue * float(SCALED_TILE_SIZE)));
	const int tempy = (lenh) - ((_TexHnum / 2 * SCALED_TILE_SIZE) + int(syue * float(SCALED_TILE_SIZE)));


	RECT rc, rct;

	constexpr DWORD iColor = 0xffffffff;
	//if(CMaskData::g_MaskData)
	//{
	//sMask* pmask = (sMask*)CMaskData::g_MaskData->g_MaskData;
	//if(pmask)
	GetChangeNameText((char*)m_pScene->GetTerrainName());
	//}

	//int posx = _sx / 2;
	//int posy = _sy / 2;

	ZeroMemory(&vecColor.front(), sizeof(DWORD)*vecColor.size());
	for (int m = 0; m < _TexHnum; m++) {
		rc.top = tempy + m * SCALED_TILE_SIZE;
		rc.bottom = rc.top + SCALED_TILE_SIZE;

		rc.left = tempx;
		rc.right = rc.left + SCALED_TILE_SIZE;

		for (int n = 0; n < _TexWnum; n++) {
			int idx = m * _TexWnum + n;

			if (_sx + n < 0 || _sy + m < 0) {
				// Fix the video memory exhausted bug
				if (_pTex[idx] != _pTexDefault) {
					SAFE_RELEASE(_pTex[idx]);
				}

				_pTex[idx] = _pTexMask;
			}
			else {
				if (CMaskData::g_MaskData) {
					if (CMaskData::g_MaskData->GetMask((_sx + n) / 2, (_sy + m) / 2)) {
						vecColor[idx + 1] = iColor;
						vecColor[idx + 1 + 1] = iColor;

						vecColor[idx + _TexWnum + 1] = iColor;
						vecColor[idx + _TexWnum + 1 + 1] = iColor;

						if (m < _TexHnum - 1 && n < _TexWnum - 1) {
							vecColor[idx + 2 + 1] = iColor;
							vecColor[idx + _TexWnum + 2 + 1] = iColor;
						}

						if (idx + _TexWnum * 2 < _TexHnum * _TexWnum) {
							vecColor[idx + _TexWnum * 2 + 1] = iColor;
							vecColor[idx + _TexWnum * 2 + 1 + 1] = iColor;
							if (m < _TexHnum - 1 && n < _TexWnum - 1)
								vecColor[idx + _TexWnum * 2 + 2 + 1] = iColor;
						}
					}
				}
				else {
					vecColor[idx + 1] = 0;
					vecColor[idx + 1 + 1] = 0;
					vecColor[idx + 2 + 1] = 0;

					vecColor[idx + _TexWnum + 1] = 0;
					vecColor[idx + _TexWnum + 1 + 1] = 0;
					vecColor[idx + _TexWnum + 2 + 1] = 0;

					//if(idx + _TexWnum * 2 <  _TexHnum * _TexWnum)
					if (m < _TexHnum && n < _TexWnum) {
						vecColor[idx + _TexWnum * 2 + 1] = 0;
						vecColor[idx + _TexWnum * 2 + 1 + 1] = 0;
						vecColor[idx + _TexWnum * 2 + 2 + 1] = 0;
					}
				}

				if (!_pMiniPack) {
					const std::string filename = std::format("texture\\minimap\\{}\\sm_{}_{}.bmp",
							m_pScene->GetTerrainName(), _sx + n, _sy + m);

					// If this texture not in video memory, load it.
					if (!_pTex[idx] || _pTex[idx]->GetFileName() != filename) {
						if (_access(filename.c_str(), 0) == -1) {
							if (_pTex[idx] != _pTexDefault) {
								SAFE_RELEASE(_pTex[idx]);
								_pTex[idx] = _pTexDefault;
							}
						}
						else {
							if (_pTex[idx] != _pTexDefault) {
								SAFE_RELEASE(_pTex[idx]);
							}
							lwLoadTex(&_pTex[idx], g_Render.GetInterfaceMgr()->res_mgr,
									  filename.c_str(), 0, D3DFMT_A8R8G8B8);
						}
					}
				}
				else {
					if (_pMiniPack->IsExit(_sx + n, _sy + m)) {
						if (_pTex[idx] != _pTexDefault) {
							SAFE_RELEASE(_pTex[idx]);
						}
						_pMiniPack->GetTex(&_pTex[idx], _sx + n, _sy + m);
					}
					else {
						if (_pTex[idx] != _pTexDefault) {
							SAFE_RELEASE(_pTex[idx]);
							_pTex[idx] = _pTexDefault;
						}
					}
				}
			}
			rc.left = tempx + n * SCALED_TILE_SIZE;
			rc.right = rc.left + SCALED_TILE_SIZE;

			rct.left = rc.left + _rcWnd.left;
			rct.top = rc.top + _rcWnd.top;
			rct.right = rct.left + SCALED_TILE_SIZE;
			rct.bottom = rct.top + SCALED_TILE_SIZE;


			RECT* prc = &vecRect[idx];
			::SetRect(prc, rct.left, rct.top, rct.right, rct.bottom);
		}
	}

	if (CMaskData::g_MaskData) {
		fInf = 0;
		int ty = CMaskData::g_MaskData->iNumY;
		int tx = CMaskData::g_MaskData->iNumX;

		for (int m = 0; m < ty; m++) {
			for (int n = 0; n < tx; n++) {
				if (CMaskData::g_MaskData->GetMask(n, m)) {
					fInf += 1;
				}
			}
		}
		fInf /= float((CMaskData::g_MaskData->iNumX) * (CMaskData::g_MaskData->iNumY));
		fInf *= 100;
		//id1 = (int)fInf * 100;
		//id2 = (int)(fInf * 10000) - id1;

		//fInf = (int)fInf;
		//fInf/=100;
	}
	else
		fInf = 0;
}

//End
void CSMNpc::Render(/*int iType,int nx,int ny,int wh = 8*/) {
	if (_pTex[_iCurTex])
		g_Render.SetTexture(0, _pTex[_iCurTex]->GetTex());
	else
		g_Render.SetTexture(0, NULL);

	g_Render.GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, &_vWndVer, sizeof(M2D_AVER));
}

void CLargerMap::RenderScene() {
	if (!CGameScene::GetMainCha())
		return;

	const bool mouseInMap = g_pGameApp->GetMouseX() > _rcWnd.left &&
		g_pGameApp->GetMouseX() < _rcWnd.right &&
		g_pGameApp->GetMouseY() > _rcWnd.top &&
		g_pGameApp->GetMouseY() < _rcWnd.bottom;

	if (mouseInMap && g_pGameApp->GetMouseKey() & M_RDown) {
		if (CGameScene::GetMainCha()->IsBoat()) {
			g_stUIBox.ShowMsgBox(NULL, "No routing on sea.");
			return;
		}
		/* Resets to initial location when map was opened?
 Update( _nInitX, _nInitY );
*/
		const POINT mouse_coord = g_stUIMap.GetMouseCoordinate();
		g_cFindPathEx.Reset();
		g_cFindPathEx.ClearDestDirection();
		g_cFindPathEx.SetDestDirection((int)CGameScene::GetMainCha()->GetPos().x,
									   (int)CGameScene::GetMainCha()->GetPos().y, mouse_coord.x, mouse_coord.y);
		g_cFindPathEx.SetTarget((int)CGameScene::GetMainCha()->GetPos().x, (int)CGameScene::GetMainCha()->GetPos().y,
								mouse_coord.x, mouse_coord.y);


		D3DXVECTOR3 v(mouse_coord.x, mouse_coord.y, 0);
		CNavigationBar::g_cNaviBar.SetTarget("", v);
		CNavigationBar::g_cNaviBar.Show(true);
	}
	else if (g_pGameApp->IsMouseContinue(0) || (g_pGameApp->GetMouseKey() & M_LDown)) {
		static DWORD dwTime = 0;
		const int nInitX = 400, nInitY = 300;
		if (CGameApp::GetCurTick() > dwTime) {
			dwTime = CGameApp::GetCurTick() + 100;

			if (mouseInMap) {
				constexpr auto magic{50};
				_nCenterX += (magic / map_scale) * (g_pGameApp->GetMouseX() - nInitX);
				_nCenterY += (magic / map_scale) * (g_pGameApp->GetMouseY() - nInitY);

				Update(_nCenterX, _nCenterY);
			}
			else if (follow_player) {
				constexpr auto distance = 4 * 100;
				const auto x_dist = std::abs(CGameScene::GetMainCha()->GetCurX() - _nCenterX);
				const auto y_dist = std::abs(CGameScene::GetMainCha()->GetCurY() - _nCenterY);
				if (x_dist > distance || y_dist > distance) {
					_nCenterX = CGameScene::GetMainCha()->GetCurX();
					_nCenterY = CGameScene::GetMainCha()->GetCurY();
					Update(_nCenterX, _nCenterY);
				}
			}
		}
	}

	m_pScene = g_pGameApp->GetCurScene();
	g_Render.SetRenderState(D3DRS_ZENABLE, FALSE);
	g_Render.SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	g_Render.SetRenderState(D3DRS_CLIPPING, TRUE);

	g_Render.SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	g_Render.SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_Render.SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_Render.SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	g_Render.SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
	g_Render.SetRenderState(D3DRS_DITHERENABLE,FALSE);
	g_Render.SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW); // 
	//
	g_Render.SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	g_Render.SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

	g_Render.SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_Render.SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	g_Render.SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	g_Render.SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	g_Render.SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	g_Render.SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

	g_Render.SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	g_Render.SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);

	g_Render.SetRenderState(D3DRS_TEXTUREFACTOR, 0xffffffff);
	g_Render.SetRenderState(D3DRS_LIGHTING, FALSE);

	g_Render.SetVertexShader(NULL);
	g_Render.SetFVF(D3DFVF_M2DWA);

	//_vPicVer[0].m_color = 0xffffffff;
	//_vPicVer[1].m_color = 0xffffffff;
	//_vPicVer[2].m_color = 0xffffffff;
	//_vPicVer[3].m_color = 0xffffffff;

	//RECT rc;
	//rc.left = 0;
	//rc.right = 1024;
	//rc.top = 0;
	//rc.bottom = 768;
	//_vPicVer[0].m_vPos = D3DXVECTOR4(float(rc.left),float(rc.top),0.9f,1);
	//_vPicVer[1].m_vPos = D3DXVECTOR4(float(rc.right),float(rc.top),0.9f,1);
	//_vPicVer[2].m_vPos = D3DXVECTOR4(float(rc.right),float(rc.bottom),0.9f,1);
	//_vPicVer[3].m_vPos = D3DXVECTOR4(float(rc.left),float(rc.bottom),0.9f,1);
	//g_Render.SetTexture(0, _pTexMask->GetTex()); 

	//g_Render.GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLEFAN,2,&_vPicVer,sizeof(M2D_AVER));


	g_Render.SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);

	for (int m = 0; m < _TexHnum; m++) {
		for (int n = 0; n < _TexWnum; n++) {
			int idx = m * _TexWnum + n;
			if (_pTex[idx] == _pTexMask)
				continue;
			if (vecColor[idx + 1] == 0 && vecColor[idx + 1 + 1] == 0
				&& vecColor[idx + _TexWnum + 1 + 1] == 0 && vecColor[idx + _TexWnum + 1] == 0)
				continue;


			{
				_vPicVer[0].m_color = vecColor[idx + 1];
				_vPicVer[1].m_color = vecColor[idx + 1 + 1];
				_vPicVer[2].m_color = vecColor[idx + _TexWnum + 1 + 1];
				_vPicVer[3].m_color = vecColor[idx + _TexWnum + 1];
			}
			if (_pTex[idx]) {
				g_Render.SetTexture(0, _pTex[idx]->GetTex());
			}
			else
				g_Render.SetTexture(0,NULL);
			RECT* prc = &vecRect[idx];

			_vPicVer[0].m_vPos = D3DXVECTOR4(float(prc->left), float(prc->top), 0.9f, 1);
			_vPicVer[1].m_vPos = D3DXVECTOR4(float(prc->right), float(prc->top), 0.9f, 1);
			_vPicVer[2].m_vPos = D3DXVECTOR4(float(prc->right), float(prc->bottom), 0.9f, 1);
			_vPicVer[3].m_vPos = D3DXVECTOR4(float(prc->left), float(prc->bottom), 0.9f, 1);

			g_Render.GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, &_vPicVer, sizeof(M2D_AVER));
		}
	}
	//g_Render.SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);


	//g_Render.SetTexture(1,NULL); 

	//////////////////////////////////////////////////////////////////////////
	g_Render.SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	g_Render.SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_Render.SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

	_cSmNpc.Begin();
	_cSmNpc.setTexture(TEXNUM - 2);
	//_cSmNpc.setPos( lenw,lenh,_rcWnd.left,_rcWnd.top );
	int lenw = (_rcWnd.right - _rcWnd.left) / 2;
	int lenh = (_rcWnd.bottom - _rcWnd.top) / 2;
	{
		constexpr auto magic{125};
		lenw += (CGameScene::GetMainCha()->GetCurX() - _nCenterX) / (magic / map_scale);
		lenh += (CGameScene::GetMainCha()->GetCurY() - _nCenterY) / (magic / map_scale);
	}
	_cSmNpc.setPos(lenw, lenh, _rcWnd.left, _rcWnd.top);
	_cSmNpc.setScaling(10, 20);
	_cSmNpc.setAngle(CGameScene::GetMainCha()->getYaw());
	_cSmNpc.setColor(0xffffffff);
	_cSmNpc.Render();

	CMPFont* pfont = FontManager::Instance().Get(FontSlot::TipText);
	if (pfont) {
		pfont->DrawTextShadow((char*)strMapName.c_str(), _rcWnd.left + 60, _rcWnd.top + 30,
							  _rcWnd.left + 61, _rcWnd.top + 31, 0xff808000, 0xffffff00);

		// Modify by sunny.sun 20080904
		//mothannakh fix compass in minimap
		auto _strCoord = SafeVFormat(GetLanguageString(394), _nCenterX / 100, _nCenterY / 100, fInf);
		pfont->DrawTextShadow(const_cast<char*>(_strCoord.c_str()), _rcWnd.left + 350, _rcWnd.top + 30,
							  _rcWnd.left + 351, _rcWnd.top + 31, 0xff808000, 0xffffff00);
	}
}

void CLargerMap::RenderMask() {
}
