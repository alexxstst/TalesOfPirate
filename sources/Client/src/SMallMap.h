#pragma once
#define MGR
#include "EffectObj.h"
#include "MindPower.h"
#include ".\packfile.h"

class CGameScene;
class CCharacter;

class CSMallWnd
{
public:
	CSMallWnd(void);
	virtual ~CSMallWnd(void);

	bool			Create(IDirect3DDeviceX*  pDev,RECT rcwnd, CGameScene* pScene = NULL,int bksize = 256);

	//void			Render();

	void			MoveTo(D3DXVECTOR3 &vPos,D3DXVECTOR3&	vCameraDir,float fCameraAngle,float fCameradDist,int iAngle = 0);
	
	void			SetDist(float fDist)	{_fDist = fDist;_bUpdate = true;_bMove = true;}
	float			GetDist()				{ return _fDist;}

	void			SetUpdate(bool bUpdate){ _bUpdate = bUpdate;}
	virtual	void	FrameMove(DWORD dwDailTime);
	virtual void	Render();

	void			GetWndPos(int &nOutX, int &nOutY, D3DXVECTOR3 &vWorldPos)
	{
		
		D3DXVECTOR4 vOut;
		const auto v = _matView * _matProj;
		D3DXVec3Transform(&vOut, &vWorldPos, &(v));
		nOutX = int((float)(_rcWnd.right - _rcWnd.left) * (vOut.x / vOut.w*0.5f+0.5f));
		nOutY = int((float)(_rcWnd.bottom - _rcWnd.top) * (0.5f - vOut.y / vOut.w * 0.5f));


		//float fdistx =  (vWorldPos.x - _vLookatPt.x) * 1.6f;
		//float fdisty =  (vWorldPos.y - _vLookatPt.y) * 1.6f;
		//nOutX = (int)fdistx;
		//nOutY = (int)fdisty;
	}
protected:
	virtual void	InitScene(){}
	virtual void	RenderScene(){}
	virtual void	RenderMask(){}
public:
	IDirect3DDeviceX*		m_pDev;

	CGameScene*				m_pScene;

protected:
#ifdef MGR
	MPITex*					_pCurSuf;
	lwISurfaceStream*		_pDSSuf;
#else
	LPDIRECT3DTEXTURE8		_pCurSuf;
	LPDIRECT3DSURFACE8		_pDSSuf;
#endif
	
	D3DSURFACE_DESC			_BufParam;

	RECT					_rcWnd;
	D3DXMATRIX				_matView;
	D3DXMATRIX				_matProj;

	//D3DXVECTOR3				veye,vlookat;
	//D3DXMATRIX				_matView2;

	D3DXVECTOR3				_vEyePt;
	D3DXVECTOR3				_vLookatPt;
	D3DXVECTOR3				_vUpVec;

	float					_fDist;
	int						_iAngle;

	//D3DXVECTOR3				_vCameraPos;
	//D3DXVECTOR3				_vCameraDir;
	//D3DXVECTOR3				_vCameraTar1[2];
	//D3DXVECTOR3				_vCameraTar2[2];
	float					_fCameraAngle;


	bool					_bUpdate;
	bool					_bMove;

	char					pszAppPath[256];

	LPD3DXRENDERTOSURFACE	_pRenderSurface;
};


#define D3DFVF_CLOCK (D3DFVF_XYZ | D3DFVF_DIFFUSE)
#define D3DFVF_M2DWA (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

struct M2D_AVER
{
	D3DXVECTOR4 m_vPos;
	DWORD m_color;
	D3DXVECTOR2 m_fUV;
	void SetValue(D3DXVECTOR4 &vPos,DWORD color,D3DXVECTOR2 &fUV)
	{
		m_vPos = vPos;
		m_color = color;
		m_fUV = fUV;
	}
};

struct MNPC_PARAM
{
	int iType;
	//D3DXVECTOR3 vPos;
	int nx,ny;
};

/************************************************************************/
/*                                                                      */
/************************************************************************/
#define D3DFVF_M2D (D3DFVF_XYZB1 | D3DFVF_DIFFUSE | D3DFVF_TEX2)
#define D3DFVF_M2DW (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX2)

#define TEXNUM	18
struct M2D_VER
{
	D3DXVECTOR4 m_vPos;
	DWORD m_color;
	D3DXVECTOR2 m_fUV;
	D3DXVECTOR2 m_fUV2;
	void SetValue(D3DXVECTOR4 &vPos,DWORD color,D3DXVECTOR2 &fUV)
	{
		m_vPos = vPos;
		m_color = color;
		m_fUV = fUV;
		m_fUV2 = fUV;
	}
};
class CSMCha
{
public:
	CSMCha()
	{	
		_pVB = NULL;
		_pTex[0] = NULL;
		_pTex[1] = NULL;
		_pTex[2] = NULL;

		_dwColor = 0xffff0000;
		_iCurTex = 0;
	}
	~CSMCha()
	{
		SAFE_RELEASE(_pVB);
		SAFE_RELEASE(_pTex[0]);
		SAFE_RELEASE(_pTex[1]);
		SAFE_RELEASE(_pTex[2]);
	}

	BOOL    Init(IDirect3DDeviceX*	pDev)
	{
		m_pDev = pDev;
		D3DXVECTOR2 vUV[4] = {
			D3DXVECTOR2(0,0),
				D3DXVECTOR2(1,0),	
				D3DXVECTOR2(1,1),
				D3DXVECTOR2(0,1),
		};
		float frad = 5.0f;
#ifdef MGR
		MPIResourceMgr* res_mgr = g_Render.GetInterfaceMgr()->res_mgr;
		res_mgr->CreateMesh(&_pVB);
		_pVB->SetStreamType(STREAM_LOCKABLE);

		MPMeshInfo mi;
		mi.fvf = D3DFVF_M2D;
		mi.pt_type = D3DPT_TRIANGLEFAN;
		mi.subset_num = 1;
		mi.vertex_num = 4;
		mi.vertex_seq = MP_NEW(MPVector3[mi.vertex_num]);
		mi.blend_seq =  MP_NEW(lwBlendInfo[mi.vertex_num]);
		mi.vercol_seq = MP_NEW(DWORD[mi.vertex_num]);
		mi.texcoord0_seq = MP_NEW(MPVector2[mi.vertex_num]);
		mi.texcoord1_seq = MP_NEW(MPVector2[mi.vertex_num]);
        mi.subset_seq = LW_NEW(lwSubsetInfo[mi.subset_num]);

		mi.vertex_seq[0] = MPVector3(-frad, -frad,  0.000001f);
		mi.vertex_seq[1] = MPVector3(frad, -frad,   0.000001f);
		mi.vertex_seq[2] = MPVector3(frad, frad,    0.000001f);
		mi.vertex_seq[3] = MPVector3(-frad, frad,   0.000001f);

		mi.blend_seq[0].weight[0] = 0;
		mi.blend_seq[1].weight[0] = 1;
		mi.blend_seq[2].weight[0] = 2;
		mi.blend_seq[3].weight[0] = 3;

		mi.vercol_seq[0] = _dwColor;
		mi.vercol_seq[1] = _dwColor;
		mi.vercol_seq[2] = _dwColor;
		mi.vercol_seq[3] = _dwColor;

		mi.texcoord0_seq[0] = MPVector2(0,0);
		mi.texcoord0_seq[1] = MPVector2(1,0);
		mi.texcoord0_seq[2] = MPVector2(1,1);
		mi.texcoord0_seq[3] = MPVector2(0,1);

		mi.texcoord1_seq[0] = MPVector2(0,0);
		mi.texcoord1_seq[1] = MPVector2(1,0);
		mi.texcoord1_seq[2] = MPVector2(1,1);
		mi.texcoord1_seq[3] = MPVector2(0,1);

		lwSubsetInfo_Construct(&mi.subset_seq[0], 2, 0, 4, 0);

		_pVB->LoadSystemMemory(&mi);
		_pVB->LoadVideoMemory();
#else
		if ( FAILED( m_pDev->CreateVertexBuffer( 4 * sizeof(M2D_VER),
			D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, 0,
			D3DPOOL_DEFAULT, &_pVB ) ) )
		{
			ToLogService("errors", LogLevel::Error, "msgCSMCha::CreateVertexBuffer");
			return FALSE;
		}

		M2D_VER * pVertices;
		_pVB->Lock(0, 0, (BYTE**)&pVertices, D3DLOCK_DISCARD);
		(*pVertices++).SetValue(D3DXVECTOR4(-frad, -frad,  0.000001f,  float(0)),_dwColor,vUV[0]);
		(*pVertices++).SetValue(D3DXVECTOR4(frad, -frad,   0.000001f,  float(1)),_dwColor,vUV[1]);
		(*pVertices++).SetValue(D3DXVECTOR4(frad, frad,    0.000001f,  float(2)),_dwColor,vUV[2]);
		(*pVertices++).SetValue(D3DXVECTOR4(-frad, frad,   0.000001f,  float(3)),_dwColor,vUV[3]);
		_pVB->Unlock();
#endif

		_vers[0] = D3DXVECTOR4(-frad, -frad,  0.000001f,  float(1));
		_vers[1] = D3DXVECTOR4(frad, -frad,   0.000001f,  float(1));
		_vers[2] = D3DXVECTOR4(frad, frad,    0.000001f, float(1));
		_vers[3] = D3DXVECTOR4(-frad, frad,   0.000001f,  float(1));

		const char* pszName[] =
		{
			"texture\\minimap\\0.tga",
			"texture\\minimap\\1.tga",
			"texture\\minimap\\2.tga",
			"texture\\minimap\\3.tga",
			"texture\\minimap\\4.tga",
			"texture\\minimap\\5.tga",
			"texture\\minimap\\6.tga",
			"texture\\minimap\\7.tga",
			"texture\\minimap\\8.tga",
			"texture\\minimap\\9.tga",
			"texture\\minimap\\10.tga",
			"texture\\minimap\\11.tga",
			"texture\\minimap\\12.tga",
			"texture\\minimap\\13.tga",
			"texture\\minimap\\arraw.tga",
			"texture\\minimap\\shop.tga"
		};
		//MPIResourceMgr* res_mgr = g_Render.GetInterfaceMgr()->res_mgr;
		for(int n = 0; n < TEXNUM; n++)
		{
#ifdef MGR
			if(FAILED(lwLoadTex(&_pTex[n],res_mgr, pszName[n], 0, D3DFMT_A4R4G4B4)))
			{
				ToLogService("errors", LogLevel::Error, "msg{}", pszName[n]);
				return FALSE;
			}
#else
			D3DXCreateTextureFromFileEx(m_pDev,
				pszName[n], //
				0, 
				0, 
				1, //mipmap1  
				0, //
				D3DFMT_UNKNOWN, //
				D3DPOOL_MANAGED, //DXGraphics
				D3DX_FILTER_LINEAR, //
				D3DX_FILTER_NONE, //mipmap
				0x00000000, //
				NULL, //
				NULL, //
				&_pTex[n]);//
			if(!_pTex[n])
			{
				ToLogService("errors", LogLevel::Error, "msgCSMCha::no found file :texture\\minimap\\arraw.tga");
				return FALSE;
			}
#endif
		}
		//////////////////////////////////////////////////////////////////////////
		RECT rc;
		rc.left = 0;
		rc.top = 0;
		rc.right = 10;
		rc.bottom = 10;

		_vWndVer[0].m_vPos = D3DXVECTOR4(float(rc.left),float(rc.top),0.9f,1);
		_vWndVer[1].m_vPos = D3DXVECTOR4(float(rc.right),float(rc.top),0.9f,1);
		_vWndVer[2].m_vPos = D3DXVECTOR4(float(rc.right),float(rc.bottom),0.9f,1);
		_vWndVer[3].m_vPos = D3DXVECTOR4(float(rc.left),float(rc.bottom),0.9f,1);

		D3DXVECTOR2 vUVwnd[4] = {
			D3DXVECTOR2(0,0),
				D3DXVECTOR2(1,0),	
				D3DXVECTOR2(1,1),
				D3DXVECTOR2(0,1),
		};
		_vWndVer[0].m_fUV = vUVwnd[0];
		_vWndVer[1].m_fUV = vUVwnd[1] ;
		_vWndVer[2].m_fUV = vUVwnd[2] ;
		_vWndVer[3].m_fUV = vUVwnd[3] ;

		_vWndVer[0].m_color = 0xffffffff;
		_vWndVer[1].m_color = 0xffffffff ;
		_vWndVer[2].m_color = 0xffffffff ;
		_vWndVer[3].m_color = 0xffffffff ;

		return TRUE;
	}

	void	setPos(D3DXVECTOR3& vPos)
	{
		D3DXMatrixTranslation(&_matWorld,vPos.x, vPos.y, 1);
	}
	void	setAngle(int z)
	{
		D3DXMatrixRotationZ(&_matRotat,Angle2Radian(float(z)) + D3DX_PI);
	}
	void	setScaling(float x,float y, float z)
	{
		D3DXMatrixScaling(&_matScal,x,y,z);
	}
	void	setColor(DWORD	dwColor)
	{
		_dwColor = dwColor;
	}
	void	setTexture(int iID)
	{
		_iCurTex = iID;
	}
	void	Begin(IDirect3DVertexShaderX* dwVs)
	{
#ifdef MGR
		_pVB->BeginSet();
		g_Render.SetVertexShader(dwVs);
#endif
	}
	void	Render()
	{
		g_Render.SetVertexShaderConstantF(8, _dwColor, 1);

		D3DXVECTOR4 tv;
		D3DXMATRIX	 tm = _matScal * _matRotat;
		for(int i = 0; i < 4; ++i)
		{
			tv = _vers[i];
			const auto v = tm * _matWorld;
			D3DXVec4Transform(&tv,&_vers[i],&(v));
			g_Render.SetVertexShaderConstantF(9+i, tv, 1);
		}
#ifdef MGR
		g_Render.SetTexture(0,_pTex[_iCurTex]->GetTex());
		_pVB->DrawSubset(0);
#else
		g_Render.SetTexture(0,_pTex[_iCurTex]);
		m_pDev->SetStreamSource( 0, _pVB, sizeof(M2D_VER) );
		g_Render.DrawPrimitive( D3DPT_TRIANGLEFAN, 0, 2 );
#endif
	}
	void	RenderSoft()
	{
		g_Render.SetRenderState(D3DRS_TEXTUREFACTOR, _dwColor );
		g_Render.SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE); 
		g_Render.SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR); 
		g_Render.SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_MODULATE);  
		const auto v = _matRotat * _matWorld;
		g_Render.SetTransformWorld(&(v));

#ifdef MGR
		g_Render.SetTexture(0,_pTex[_iCurTex]->GetTex());
		_pVB->DrawSubset(0);
#else
		g_Render.SetTexture(0,_pTex[_iCurTex]);
		m_pDev->SetStreamSource( 0, _pVB, sizeof(M2D_VER) );
		g_Render.DrawPrimitive( D3DPT_TRIANGLEFAN, 0, 2 );
#endif

	}

	void	End()
	{
#ifdef MGR
		_pVB->EndSet();
#endif
	}

	void	RenderWndCha(int iType,int nx,int ny,int wh = 8)
	{
		RECT rc;
		//int nx,ny;
		//g_Render.GetScreenPos(nx,ny,*vPos);

		//float ferpx,ferpy;
		//ferpx = (float)nx / (rcwnd.right  - rcwnd.left);//g_Render.GetScrWidth();
		//ferpy = (float)ny / (rcwnd.bottom  - rcwnd.top);//g_Render.GetScrHeight();

		//nx = rcwnd.left + ( ferpx *(rcwnd.right  - rcwnd.left));
		//ny = rcwnd.top + ( ferpy *(rcwnd.bottom  - rcwnd.top));

		rc.left = (LONG)nx;
		rc.top = (LONG)ny;
		rc.right = rc.left + wh;
		rc.bottom = rc.top + wh;

		_vWndVer[0].m_vPos = D3DXVECTOR4(float(rc.left),float(rc.top),0.9f,1);
		_vWndVer[1].m_vPos = D3DXVECTOR4(float(rc.right),float(rc.top),0.9f,1);
		_vWndVer[2].m_vPos = D3DXVECTOR4(float(rc.right),float(rc.bottom),0.9f,1);
		_vWndVer[3].m_vPos = D3DXVECTOR4(float(rc.left),float(rc.bottom),0.9f,1);

		if(iType> 0)
		{
			g_Render.SetTexture(0, _pTex[iType]->GetTex());
		}else
		{
			g_Render.SetTexture(0, _pTex[10]->GetTex());
		}
		g_Render.SetRenderState(D3DRS_ZENABLE,      FALSE);
		g_Render.SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		//g_Render.SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_POINT );
		//g_Render.SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_POINT );
		//g_Render.SetRenderState(D3DRS_TEXTUREFACTOR, 0xffffffff );
		g_Render.SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW); // 
		//g_Render.SetTextureStageState( 0, D3DTSS_ADDRESSU , D3DTADDRESS_CLAMP);		
		//g_Render.SetTextureStageState( 0, D3DTSS_ADDRESSV , D3DTADDRESS_CLAMP);
		g_Render.SetVertexShader(NULL);
		g_Render.SetFVF(D3DFVF_M2DWA);
		g_Render.GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLEFAN,2,&_vWndVer,sizeof(M2D_AVER));
	}
public:
	IDirect3DDeviceX*		m_pDev;

	std::vector<MNPC_PARAM>		_vecNpc;

protected:
#ifdef MGR
	MPITex*						_pTex[TEXNUM];

	MPIMesh*					_pVB;
#else
	LPDIRECT3DTEXTURE8			_pTex[TEXNUM];

	LPDIRECT3DVERTEXBUFFER8		_pVB;
#endif

	D3DXMATRIX					_matWorld;
	D3DXMATRIX					_matRotat;
	D3DXMATRIX					_matScal;

	D3DXCOLOR					_dwColor;
	D3DXVECTOR4					_vers[4];
	int							_iCurTex;


	M2D_AVER					_vWndVer[4];


private:
};

class CSMallMap2D: public CSMallWnd
{
public:
	CSMallMap2D(void);
	~CSMallMap2D(void);

	struct TEXIDX
	{
		TEXIDX()
		{
			x = -1;
			y = -1;
			pTex = NULL;
		}
		TEXIDX(int tx, int ty)
		{
			x = tx;
			y = ty;
			pTex = NULL;
		}
		void	ReleaseTex()
		{
			SAFE_RELEASE(pTex);
		}
		~TEXIDX()
		{
			SAFE_RELEASE(pTex);
		}
		BOOL operator == ( CONST TEXIDX& tes )
		{
			return ((x == tes.x) && (y == tes.y));
		}
		int	x,y;
#ifdef MGR
		MPITex*					pTex;			
#else
		LPDIRECT3DTEXTURE8		pTex;
#endif

	};
protected:
	void	InitScene();

	void	RenderScene();

	void	RenderMask();
private:
#ifdef MGR
	MPIMesh*				_pVB;			
#else
	LPDIRECT3DVERTEXBUFFER8 _pVB;
#endif

	TEXIDX					_pTex[3][3];

	int						_sx;
	int						_sy;			
	//CEffectBox  box;
	CSMCha					_Cha;

#ifdef MGR
	MPITex*					_pTexMask;			
#else
	LPDIRECT3DTEXTURE8		_pTexMask;
#endif

#ifdef MGR
	MPIMesh*				_pVBWnd;			
#else
	LPDIRECT3DVERTEXBUFFER8 _pVBWnd;
#endif



#ifdef MGR
	MPITex*					_pTexDefault;			
#else
	LPDIRECT3DTEXTURE8		_pTexDefault;
#endif

	D3DXCOLOR				_dwColor;
};




/************************************************************************/
/**/
/************************************************************************/
class CAniWnd: public CSMallWnd
{
public:
	CAniWnd();
	~CAniWnd();

	void			MoveWnd(int x, int y);

	bool			IsEnd()		{return !_bUpdate;}
	void			Play(DWORD dwPlayTime);
	virtual	void	FrameMove(DWORD dwDailTime);
	virtual void	Render();

	struct ClockVer
	{
		D3DXVECTOR3 vPos;
		DWORD dwColor;
	};
protected:
	void	InitScene();

	void	RenderScene();

	void	RenderMask();

	void	ResetTime(DWORD	dwTime);
private:
#ifdef MGR
	MPIMesh*				_pVBWnd;	
#else
	LPDIRECT3DVERTEXBUFFER8 _pVBWnd;
#endif


	float					_fPlayTime;
	float					_fCurTime;
	float					_fCurAngle;

	ClockVer				_vVertex[6];
	ClockVer				_vSave[6];

	M2D_AVER				_vWndVer[4];
};

/************************************************************************/
/**/
/************************************************************************/
#define D3DFVF_CLOCK2 (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)

struct stNetTeamChaPart;
class CCharacterModel;

//struct SClientAttr
//{
//	short sTeamAngle; // 
//	float fTeamDis;   // 
//	SClientAttr()
//		:sTeamAngle(0),
//		fTeamDis(0)
//	{
//	}
//};
//
//inline SClientAttr* GetClientAttr(int nScriptID)
//characterinfo

// >dofile("scripts/cameraconf.clu")reload

class CCharacter2D//: public CSMallWnd
{
public:
	CCharacter2D(void);
	~CCharacter2D(void);

	void	Create(RECT rc);

	void    UpdataFace(stNetTeamChaPart& stPart, bool IsPlayer = true);

	void	MoveWnd(RECT rc);

	CCharacterModel* GetCha()   { return _pModel;   }

	void	Render();


	void	LoadCha(DWORD dwID, bool IsMonster = false);

	bool	IsLoad(){ return _bLoad; }
public:
	bool					_bLoad;
	D3DVIEWPORTX			_vp;
	D3DXVECTOR3				_vPos;
	D3DXMATRIXA16           _mat3DUIView;
	D3DXMATRIXA16           _mat3DUIProj;

	CCharacterModel*		_pModel;
    stNetTeamChaPart*       _pChaPart;

};

class CBigMap
{
public:
	CBigMap();
	~CBigMap();

	void			Create();
	void			Destory();
	void			Render();
	bool			IsLoad(){ return _pTex ? true : false; }
protected:
	M2D_AVER		_vWndVer[4];

	int				wc,hc;
	int				_rcW, _rcH;

	D3DXCOLOR		_dwColor;

	MPITex**		_pTex;

};

class Ctemp
{
public:
	Ctemp();
	~Ctemp();

	void	Render();

	M2D_AVER	_vWndVer[4];

	MPITex*		_pTex;

};

//////////////////////////////////////////////////////////////////////////
#define  NPCSIZE	12
class CSMNpc
{
public:
	CSMNpc()
	{	
		int n = 0;
		for (; n < TEXNUM; n++)
		{
			_pTex[n] = NULL;
		}

		_iCurTex = 0;
		nw = NPCSIZE;
		nh = NPCSIZE;
		mposx = 0;
		mposy = 0;
	}
	~CSMNpc()
	{
		int n = 0;
		for (; n < TEXNUM; n++)
		{
			SAFE_RELEASE(_pTex[n]);
		}
	}

	BOOL    Init()
	{
		const char* pszName[] = 
		{
			"texture\\ui\\minimap\\0.tga",
			"texture\\ui\\minimap\\1.tga",
			"texture\\ui\\minimap\\2.tga",
			"texture\\ui\\minimap\\3.tga",
			"texture\\ui\\minimap\\4.tga",
			"texture\\ui\\minimap\\5.tga",
			"texture\\ui\\minimap\\6.tga",
			"texture\\ui\\minimap\\7.tga",
			"texture\\ui\\minimap\\8.tga",
			"texture\\ui\\minimap\\9.tga",
			"texture\\ui\\minimap\\10.tga",
			"texture\\ui\\minimap\\11.tga",
			"texture\\ui\\minimap\\12.tga",
			"texture\\ui\\minimap\\13.tga",
			"texture\\ui\\minimap\\14.tga",
			"texture\\ui\\minimap\\15.tga",
			"texture\\ui\\minimap\\arraw.tga",
			"texture\\ui\\minimap\\shop.tga"
		};
		MPIResourceMgr* res_mgr = g_Render.GetInterfaceMgr()->res_mgr;
		for(int n = 0; n < TEXNUM; n++)
		{
			if(FAILED(lwLoadTex(&_pTex[n],res_mgr, pszName[n], 0, D3DFMT_A4R4G4B4)))
			{
				{ char _buf[512]; snprintf(_buf, sizeof(_buf), g_oLangRec.GetString(395), pszName[n]); g_logManager.InternalLog(LogLevel::Error, "errors", _buf); }
				//return FALSE;
				_pTex[n] = NULL;
			}
		}
		//////////////////////////////////////////////////////////////////////////
		rc.left = 0;
		rc.top = 0;
		rc.right = NPCSIZE;
		rc.bottom = NPCSIZE;

		_vWndVer[0].m_vPos = D3DXVECTOR4(float(rc.left),float(rc.top),0.9f,1);
		_vWndVer[1].m_vPos = D3DXVECTOR4(float(rc.right),float(rc.top),0.9f,1);
		_vWndVer[2].m_vPos = D3DXVECTOR4(float(rc.right),float(rc.bottom),0.9f,1);
		_vWndVer[3].m_vPos = D3DXVECTOR4(float(rc.left),float(rc.bottom),0.9f,1);

		D3DXVECTOR2 vUVwnd[4] = {
			D3DXVECTOR2(0,0),
				D3DXVECTOR2(1,0),	
				D3DXVECTOR2(1,1),
				D3DXVECTOR2(0,1),
		};
		_vWndVer[0].m_fUV = vUVwnd[0];
		_vWndVer[1].m_fUV = vUVwnd[1] ;
		_vWndVer[2].m_fUV = vUVwnd[2] ;
		_vWndVer[3].m_fUV = vUVwnd[3] ;

		_vWndVer[0].m_color = 0xffffffff;
		_vWndVer[1].m_color = 0xffffffff ;
		_vWndVer[2].m_color = 0xffffffff ;
		_vWndVer[3].m_color = 0xffffffff ;

		return TRUE;
	}

	void	setPos(int posx,int posy, int mainx,int mainy, int wndx,int wndy,int wndw = 64)
	{
		int nx, ny;
		int tx,ty;
		//nx =  int((posx - mainx) / 100.0f  * 1.6f);
		//ny =  int((posy - mainy) / 100.0f  * 1.6f);
		nx =  int( float(posx  / 100 ) * 1.6f);
		ny =  int( float(posy  / 100 ) * 1.6f);
		tx =  int( float(mainx  / 100 ) * 1.6f);
		ty =  int( float(mainy  / 100 ) * 1.6f);

		rc.left = (LONG)((nx - tx)  + wndw);
		rc.top = (LONG)((ny - ty)  + wndw);
		rc.right = rc.left + nw;
		rc.bottom = rc.top + nh;

		mposx = rc.left + wndx;
		mposy = rc.top + wndy;

	}
	void    setPos(int posx,int posy, int wndx,int wndy)
	{
		mposx = posx + wndx;
		mposy = posy + wndy;

		rc.left = (LONG)posx - nw / 2;
		rc.top = (LONG)posy - nh / 2;
		rc.right = rc.left + nw;
		rc.bottom = rc.top + nh;
	}
	void	setScaling(int x,int y)
	{
		nw = x;
		nh = y;

		rc.left = (LONG)mposx - nw / 2;
		rc.top = (LONG)mposy - nh / 2;
		rc.right = rc.left + nw;
		rc.bottom = rc.top + nh;
	}
	void	setAngle(int z)
	{
		_vWndVer[0].m_vPos = D3DXVECTOR4(float(rc.left),float(rc.top),0.9f,1);
		_vWndVer[1].m_vPos = D3DXVECTOR4(float(rc.right),float(rc.top),0.9f,1);
		_vWndVer[2].m_vPos = D3DXVECTOR4(float(rc.right),float(rc.bottom),0.9f,1);
		_vWndVer[3].m_vPos = D3DXVECTOR4(float(rc.left),float(rc.bottom),0.9f,1);

		//if(z != 0)
		{
			D3DXVECTOR2 vpos[4];
			vpos[0] = D3DXVECTOR2(float(rc.left),float(rc.top));
			vpos[1] = D3DXVECTOR2(float(rc.right),float(rc.top));
			vpos[2] = D3DXVECTOR2(float(rc.right),float(rc.bottom));
			vpos[3] = D3DXVECTOR2(float(rc.left),float(rc.bottom));

			int n;
			D3DXMATRIX mat;
			//D3DXMatrixRotationZ(&mat,Angle2Radian(float(z)));
			D3DXVECTOR3	tvpos(float(mposx),float(mposy),0);
			 auto v = D3DXVECTOR3(0, 0, 1);
			GetMatrixRotation(&mat, &tvpos, &v, Angle2Radian(float(z)) + D3DX_PI);

			for ( n = 0; n < 4; n++)
			{
				D3DXVec2TransformCoord(&vpos[n],&vpos[n],&mat);
				_vWndVer[n].m_vPos.x = vpos[n].x;
				_vWndVer[n].m_vPos.y = vpos[n].y;
			}
		}/*else
		{

		}*/
	}

	void	setColor(DWORD	dwColor)
	{
		_vWndVer[0].m_color = dwColor;
		_vWndVer[1].m_color = dwColor ;
		_vWndVer[2].m_color = dwColor ;
		_vWndVer[3].m_color = dwColor ;
	}
	void	setTexture(int iID)
	{
		_iCurTex = iID;
	}
	void	Begin()
	{
		g_Render.SetVertexShader(NULL);
		g_Render.SetFVF(D3DFVF_M2DWA);
	}

	void	Render(/*int iType,int nx,int ny,int wh = 8*/);

protected:
	MPITex*						_pTex[TEXNUM];

	D3DXVECTOR4					_vers[4];
	int							_iCurTex;

	M2D_AVER					_vWndVer[4];

	RECT						rc;
	int							nw;
	int							nh;

	int							mposx;
	int							mposy;
};

struct sMask
{
	char pszName[32];
	int lenx,leny;
	long lpos;
	long llen;
};
class CMaskData
{
public:
	CMaskData()
	{
		pData = NULL;
		iLength = 0;
		iNumX = iNumY = 0;
	}
	~CMaskData()
	{
		SAFE_DELETE_ARRAY(pData);
	}


	bool	InitMaskData(BYTE* pRecData, long lLen)
	{
		sMask* pmask = (sMask*)pRecData;

		iNumX = pmask->lenx;
		iNumY = pmask->leny;

		if(iLength != pmask->llen )
		{
			SAFE_DELETE_ARRAY(pData);
			iLength = pmask->llen;
			pData = new BYTE[iLength];
		}
		memcpy(pData,pRecData, iLength);

		return true;
	}
	bool	GetMask(int x, int y)
	{
		//int x  = posx;//((posx / 100) / 40 );
		//int y  = posy;//((posy / 100) / 40 );
		if(x < 0 || y < 0 ||x >= iNumX || y >= iNumY)
			return false;

		BYTE* p = pData;
		p += sizeof(sMask);
		int pos = y * iNumX + x;
		int potion = pos / 8;

		pos = pos - potion * 8;

		if(p[potion] & (1 << pos))
			return true;
		//Add by sunny.sun 20080903
        return true;    // 
		//return false;
	}
	//void	SetMask(int x, int y)
	//{
	//	if(x < 0 || y < 0 ||x >= iNumX || y >= iNumY)
	//		return;
	//	//int x  = posx;//((posx / 100) / 40 );
	//	//int y  = posy;//((posy / 100) / 40 );

	//	int pos = y * iNumX + x;
	//	int potion = pos / 8;

	//	pos = pos - potion * 8;

	//	int value = (1 << pos);
	//	if(!(pData[potion] & value))
	//		pData[potion] |= value;
	//}

public:
	static CMaskData* g_MaskData;

	long iLength;
	int iNumX,iNumY;
	BYTE* pData;

};


#define		VIEWRANGE	3
class CMinimap : public CSMallWnd
{
public:
	CMinimap(){
		_pTexMask = NULL;
		_pTexDefault = NULL;
		_pMiniPack = NULL; 
	}
	~CMinimap()
	{
		SAFE_RELEASE(_pTexMask);
		SAFE_RELEASE(_pTexDefault);
		SAFE_DELETE(_pMiniPack); 

		SAFE_DELETE(CMaskData::g_MaskData);
	}
	struct TEXIDX
	{
		TEXIDX()
		{
			x = -1;
			y = -1;
			pTex = NULL;
		}
		TEXIDX(int tx, int ty)
		{
			x = tx;
			y = ty;
			pTex = NULL;
		}
		void	ReleaseTex()
		{
			SAFE_RELEASE(pTex);
		}
		~TEXIDX()
		{
			SAFE_RELEASE(pTex);
		}
		BOOL operator == ( CONST TEXIDX& tes )
		{
			return ((x == tes.x) && (y == tes.y));
		}
		int	x,y;
		MPITex*					pTex;
	};
protected:
	void	InitScene();

	void	RenderScene();

	void	RenderMask();
private:
	TEXIDX					_pTex[VIEWRANGE][VIEWRANGE];
	M2D_AVER				_vWndVer[4];
	M2D_AVER				_vPicVer[4];


	int						_sx;
	int						_sy;			
	MPITex*					_pTexMask;			

	MPITex*					_pTexDefault;

	D3DXCOLOR				_dwColor;

	CSMNpc					_cSmNpc;

	D3DXVECTOR4				_vCameraTar[2];

	CMiniPack*				_pMiniPack;
};



//#define		VIEW_RANGE		15
#define     TILE_SIZE		32
#define		HALF_TILE		16
class CLargerMap : public CSMallWnd
{
public:
	CLargerMap()
	{
		_TexWnum = 0;
		_TexHnum = 0;
		//_pTex = NULL;
		//_pTexMask = NULL;
		_pTexDefault = NULL;
		_pMiniPack = NULL; 
	}
	~CLargerMap()
	{
		//if(_pTex)
		{
			int m;
			for (m = 0; m < _TexHnum * _TexWnum; m++)
			{
				if(_pTex[m] != _pTexDefault && _pTex[m] != _pTexMask)
					SAFE_RELEASE(_pTex[m]);
			}
			//SAFE_DELETE_ARRAY(_pTex);
		}
		SAFE_RELEASE(_pTexMask);
		SAFE_RELEASE(_pTexDefault);
		SAFE_DELETE(_pMiniPack); 
		SAFE_DELETE(CMaskData::g_MaskData);
	}

	void	Show(bool bShow);
	bool	IsShow() const { return _bUpdate; }

	void	Update( int x, int y );
	int		GetCenterX()const { return _nCenterX; }
	int		GetCenterY()const { return _nCenterY; }
	
	void SetScale(float s)
	{
		constexpr auto MIN_ZOOM{ 1.0f }, MAX_ZOOM{ 8.0f };
		if (s >= MIN_ZOOM && s <= MAX_ZOOM)
		{
			map_scale = s;
			_bUpdate = false;
			Show(true);
		}
	}
	float GetScale() const { return map_scale; }

	void SetFollow(bool v) { follow_player = v; }
	

protected:
	void	InitScene();

	void	RenderScene();

	void	RenderMask();

	void	GetChangeNameText(char* pszName)
	{
		if (strcmp("garner",pszName) == 0)
			strMapName = g_oLangRec.GetString(56);
		else if (strcmp("darkblue",pszName) == 0)
			strMapName = g_oLangRec.GetString(58);
		else if (strcmp("eastgoaf",pszName) == 0)
			strMapName = g_oLangRec.GetString(396);
		else if (strcmp("lonetower",pszName) == 0)
			strMapName = g_oLangRec.GetString(397);
		else if (strcmp("magicsea",pszName) == 0)
			strMapName = g_oLangRec.GetString(57);

	}
private:
	std::vector<MPITex*>	_pTex;
	int						_TexWnum{}, _TexHnum{};
	M2D_AVER				_vWndVer[4];
	M2D_AVER				_vPicVer[4];


	std::vector<RECT>		vecRect;

	std::vector<DWORD>		vecColor;
	int						_sx;
	int						_sy;			
	MPITex* _pTexMask{};

	MPITex* _pTexDefault{};

	D3DXCOLOR				_dwColor;

	CSMNpc					_cSmNpc;

	D3DXVECTOR4				_vCameraTar[2];

	CMiniPack* _pMiniPack{};

	s_string				strMapName;

	float 					fInf;

	int						_nCenterX, _nCenterY;
	int						_nInitX, _nInitY;

	float map_scale{ 1.0f };
	bool follow_player{ true };
};
