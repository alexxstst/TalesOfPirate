#pragma once
class   MPRender;

#include "i_effect.h"
#include "EffectFile.h"

//#include "mpmodeleff.h"

#include "mpshademap.h"

#include "mpparticlectrl.h"

#include "MPResourceSet.h"
#if RESOURCE_SCRIPT == 1
#include <set>
#endif


template<class _Ty>	class S_FVector
{
	std::vector<_Ty>	m_VECPath;
	int m_nCount;
	int m_nPos;
public:
	S_FVector()
	{
		m_VECPath.clear();
		m_nCount =0;
		m_nPos = 0;
	}
	void resize(WORD _nSize = 100)
	{
		m_VECPath.resize(_nSize);
		m_nCount = _nSize;
		clear();
	}
	void setsize(WORD _nSize = 100)		{m_nPos -= _nSize;}
	void clear()						{m_nPos = m_nCount;}
	void push_front(_Ty &_BaseMesh)		{--m_nPos; m_VECPath[m_nPos] = _BaseMesh;}
	void pop_front()
	{
		if(m_nCount > m_nPos)
		{
			++m_nPos;
			if(m_nPos == m_nCount)
			{
				clear();
			}
		}
	}
	int size() const { return m_nCount - m_nPos; }
	bool	empty()						{return m_nPos == m_nCount ? true : false;}
	_Ty* operator[] ( int i )
	{
		if(!empty() && i >= 0 && i < size())
		{
			return &m_VECPath[m_nPos + i];
		}
		return NULL;
	}
	_Ty* front()
	{
		if(!empty())
			return &m_VECPath[m_nPos];
		return NULL;
	}
	_Ty* next()
	{
		if(!empty())
		{
			if( (m_nPos + 1) != m_nCount)
				return &m_VECPath[m_nPos + 1];
		}
		return NULL;
	}
};

inline bool fEquat( float f1, float f2)
{
	return fabs(f1 - f2) < 0.000001f; 
}
#define  MAXPART_COUNT		1500

#define  MAXMSG_COUNT		150

#define  MAXMESH_COUNT		800


//////////////////////////////////////////////////////////////////////////

class  CMPResManger
{
public:
	CMPResManger(void);
	~CMPResManger(void);
	
	void Clear();

public:
	bool					LoadTotalVShader(lwISysGraphics* sys_graphics);

#ifdef USE_RENDER
	bool					InitRes(MPRender*		pDev, D3DXMATRIX* pmat, D3DXMATRIX* pMatviewproj);
#else
	bool					InitRes(LPDIRECT3DDEVICE8		pDev, D3DXMATRIX* pmat, D3DXMATRIX* pMatviewproj);

#endif

	bool					InitRes2();
	bool					InitRes3();

	void					ReleaseTotalRes();

	BOOL					OnResetDevice();
	BOOL					OnLostDevice();



	int						GetTexNum(){ return _iTexNum;}
	int						GetMeshNum(){ return _iMeshNum;}
	int						GetEffectNum(){ return _iEffectNum;}
	int						GetSubEffectNum( int idx) { return (int)_vecEffectList[idx].size();}

	char*					GetEffectSavePath(){ return _pszEFFectPath;}

	I_Effect*			 	AddEffectToMgr(const s_string& strName);
	void					AddUniteEffectToMgr(std::vector<I_Effect>& vecEffArray);

	int						GetTextureID(const s_string &pszName);
	IDirect3DTextureX*		GetTextureByID( int iID);

	lwITex*					GetTextureByIDlw( int iID);
	lwITex*					GetTextureByNamelw(const s_string &sName);

	int						GetMeshID(const s_string &pszName);
	CEffectModel*			GetMeshByID( int iID);
	CEffectModel*			GetMeshByName(const s_string &sName);
	CEffectModel*			GetShadeMesh();


	void					DeleteMesh(CEffectModel& rEffectModel);

	int						GetEffectID(const s_string &pszName);
	std::vector<I_Effect>&	GetEffectByID( int iID);
	I_Effect*				GetSubEffectByID(int iID, int iSubIdx);

	//int						GetEffectTechByID(int iID);
	EffParameter*			GetEffectParamByID(int iID); 

	IDirect3DVertexShaderX*	GetVShaderByID(int iID);
	IDirect3DVertexDeclarationX* GetVDeclByID(int iID);
	IDirect3DVertexShaderX*	GetShadeVS();
	IDirect3DVertexDeclarationX* GetShadeVDecl();
	IDirect3DVertexShaderX*	GetFontVS();
	IDirect3DVertexDeclarationX* GetFontVDecl();
	IDirect3DVertexShaderX*	GetMinimapVS(){return _dwMinimapVS;}
	IDirect3DVertexDeclarationX* GetMinimapVDecl();


	int						GetEffPathID(const s_string& pszName);
	CEffPath*				GetEffPath(int iID);

	CMPEffectFile*			GetEffectFile()	{ return &_CEffectFile;}

	void					FrameMove(DWORD dwTime);
	void					Render();
	void					UpdateMatrix();
	float*					GetDailTime()		{ return &_fDailTime;}
	D3DXMATRIX*				GetBBoardMat()		{ return &_MatBBoard;}
	D3DXMATRIX*				GetViewProjMat()	{ return &_MatViewProjPose;}
	D3DXMATRIX*				Get2DViewProjMat()	{ return &_Mat2dViewProj;}
	int						GetBackBufferWidth(){ return m_d3dBackBuffer.Width;}
	int						GetBackBufferHeight(){ return m_d3dBackBuffer.Height;}
	int&					GetFontBkWidth()	{ return _iFontBkWidth;}
	int&					GetFontBkHeight()	{ return _iFontBkHeight;}

	D3DCAPSX*				GetDevCap()			{ return &m_caps;}

	char*					GetDefaultText()	{ return _psDefault;}


	void					BeginEffect(int iIdx);
	void					EndEffect();

	void					RestoreEffect();

	//!
	VEC_string& GetTotalTexName(){ return _vecTexName;}
	VEC_string& GetTotalMeshName(){ return _vecMeshName;}
	VEC_string& GetTotalEffectName(){ return _vecEffectName;}

	//void		UpdateTotalModel();

	bool		IsCanFrame()	{ return m_bCanFrame; }
	int 		GetCanFrame()	{ return m_iCurFrame; }



	int						GetPartCtrlID(const s_string& pszName);
//#ifdef USE_GAME
//	CMemoryBuf*				GetPartCtrlByID(int iID);
//#else
	CMPPartCtrl*			GetPartCtrlByID(int iID);
//#endif
	int						GetPartCtrlNum(){return _iPartCtrlNum;}

	CEffectModel*			NewTobMesh();
	bool					DeleteTobMesh(CEffectModel& rEffectModel);
	int						GetTobMeshNum(){return _iTobMeshNum;}

	CMPPartCtrl*			NewPartCtrl(const s_string& strName);
	void					DeletePartCtrl(int iID);


	void					SendResMessage(const s_string& strPartName, D3DXVECTOR3 vPos, MPMap* pMap);
public:
	//!3D
#ifdef		USE_RENDER
	MPRender*					m_pDev;
#else
	LPDIRECT3DDEVICE8			m_pDev;
#endif

	D3DCAPSX					m_caps;
	D3DSURFACE_DESC				m_d3dBackBuffer;

	bool						m_bUseSoft;
	bool						m_bUseSoftOrg;

	bool						m_bCanFrame;
	int							m_iCurFrame;


	lwISystem*					m_pSys;
    lwISysGraphics*             m_pSysGraphics;
protected:
	//!
	bool					LoadEffectFromFile(int idx, char* pszFileName);

	void					LoadTotalRes();
	void					LoadTotalData();
	bool					LoadTotalTexture();
	bool					LoadTotalMesh();
	bool					LoadTotalEffect();
	bool					LoadTotalVShader();
	bool					LoadTotalPath();

	bool					LoadDefaultText(const char* pszFileName);

	void					LoadTotalPartCtrl();
protected:
	char				_pszTexPath[MAX_PATH];
	char				_pszMeshPath[MAX_PATH];
	char				_pszEFFectPath[MAX_PATH];

	int					_iTexNum;
	int					_iMeshNum;
	int					_iEffectNum;

	int					_iVShaderNum;

	VEC_string				_vecTexName;
	VEC_string				_vecMeshName;
	VEC_string				_vecEffectName;

	typedef std::map<std::string, int> MESH_MAP;
		MESH_MAP _mapMesh;

	typedef std::map<std::string, int> EFFECT_MAP;
		EFFECT_MAP _mapEffect;

	typedef std::map<std::string, int> TEXTURE_MAP;
		TEXTURE_MAP _mapTexture;

	std::vector<lwITex*>			_vecTexList;
	std::vector<CEffectModel*>				_vecMeshList;
	CEffectModel*							_CShadeModel;
	std::vector< std::vector<I_Effect> >	_vecEffectList;

	std::vector<EffParameter>				_vecEffectParam;

	std::vector<IDirect3DVertexShaderX*>	_vecVShader;
	std::vector<IDirect3DVertexDeclarationX*>	_vecVDecl;
	IDirect3DVertexShaderX*					_dwShadeMapVS;
	IDirect3DVertexShaderX*					_dwFontVS;

	IDirect3DVertexShaderX*					_dwMinimapVS;
	bool									_bMagr;//

	CMPEffectFile						_CEffectFile;
	//VEC_int								_vecEffTech;

	float								_fSaveTime;
	float								_fCurTime;
	float								_fDailTime;
	bool								_bInitTime;

	D3DXMATRIX*							_pMatView;
	D3DXMATRIX							_MatBBoard;
	D3DXMATRIX*							_pMatViewProj;
	D3DXMATRIX							_MatViewProjPose;
	D3DXMATRIX							_Mat2dViewProj;

	int									_iFontBkWidth;
	int									_iFontBkHeight;

//////////////////////////////////////////////////////////

	int									_iPathNum;
	VEC_string							_vecPathName;
	std::vector<CEffPath>				_vecPath;
/////////////////////////////////////////////////////////
	char								_psDefault[418];
//////////////////////////////////////////////////////////////////////////
	int									_iPartCtrlNum;
	VEC_string							_vecPartName;
//#ifdef USE_GAME
//	std::vector<CMemoryBuf>				_vecPartCtrl;
//#else
	S_BVECTOR<CMPPartCtrl*>				_vecPartCtrl;
//#endif

//////////////////////////////////////////////////////////////////////////

	int									_iTobMeshNum;
	std::list<CEffectModel*>			_lstTobMeshs;

//////////////////////////////////////////////////////////////////////////
	std::vector<CMPPartCtrl*>			_vecPartArray;
	S_FVector<WORD>						_vecValidID;

#if RESOURCE_SCRIPT == 1
	typedef std::set<string> StringMap;
	typedef std::set<string>::iterator StrMapIter;
	StringMap							_mapParticle;
	StringMap							_mapPath;
	StringMap					_mapEffect;
	StringMap					_mapMesh;
	StringMap					_mapTexture;
#endif
};

extern CMPResManger     ResMgr;
extern LW_RESULT	g_OnLostDevice();
extern LW_RESULT	g_OnResetDevice();
