#pragma once

#include "SceneNode.h"
#include "MindPower.h"
#include "mpeffectctrl.h"
#include "MPModelEff.h"
#include "MPShadeMap.h"
#include "mpparticlesys.h"
#include "mpparticlectrl.h"

enum	Effect_Type
{
	EFF_SCENE,  //	
	EFF_CHA,	//,ID
	EFF_ITEM,	//,item dummyitem id
	EFF_STRIP,	//
	EFF_SELF,	//
	EFF_HIT,	//,
	EFF_MAGIC,	//,
	EFF_FONT,	//,
};
class CCharacter;
class CGameScene;
class CEffDelay;

enum  EFFOBJ_TYPE
{
	EFFOBJ_NONE		        = 0,
	EFFOBJ_SHADE		    = 1,
	EFFOBJ_SHADEANI		    = 2,
	EFFOBJ_PARTICLE_RIPPLE	= 3,
	EFFOBJ_PARTICLE_TRACE	= 4,
	EFFOBJ_SCENE		    = 5,
	EFFOBJ_PARTICLE		    = 6,
	EFFOBJ_PART				= 7,
};


struct BoxVer
{
	float		m_vpos[3];
	DWORD		 m_dwColor;
};

class CEffectBox
{
public:
	CEffectBox(){ _pDev = NULL;_lpIB = NULL; _lpVB = NULL;
	_lpIBLine = NULL; 
	_bShow =false;D3DXMatrixScaling(&_matScale,1,1,1);
	_dwColor = 0xffffffff;_bWriteFrame = FALSE;_bShowLine = FALSE;_bShowBox = TRUE;}

	~CEffectBox(){ SAFE_RELEASE(_lpIB); SAFE_RELEASE(_lpVB);SAFE_RELEASE(_lpIBLine); }

	void		Create(IDirect3DDeviceX*	pDev,  float fRadius = 0.5f)
	{
		SAFE_RELEASE(_lpIB); SAFE_RELEASE(_lpVB);SAFE_RELEASE(_lpIBLine);
		_pDev = pDev;
		BoxVer  ver[8] = {
			{ -fRadius,-fRadius,fRadius * 2,0xffff0000},
			{ -fRadius, fRadius,fRadius * 2,0xffff0000},
			{  fRadius, fRadius,fRadius * 2,0xffff0000},
			{  fRadius,-fRadius,fRadius * 2,0xffff0000},

			{ -fRadius,-fRadius,0,0xffff0000},
			{ -fRadius, fRadius,0,0xffff0000},
			{  fRadius, fRadius,0,0xffff0000},
			{  fRadius,-fRadius,0,0xffff0000},
		};
		pDev->CreateVertexBuffer(sizeof(BoxVer) * 8, 
					D3DUSAGE_WRITEONLY |D3DUSAGE_DYNAMIC,
					D3DFVF_XYZ | D3DFVF_DIFFUSE, 
					D3DPOOL_DEFAULT, &_lpVB, NULL);
		BoxVer *pVertex;
		_lpVB->Lock(0, 0, (void**)&pVertex, D3DLOCK_NOOVERWRITE   );
		memcpy(pVertex, ver, sizeof(BoxVer) * 8);
		_lpVB->Unlock();
		WORD wIndex[24] = {
			0,1,2,3,//top
			5,4,7,6,//bottom
			5,1,0,4,//left
			7,3,2,6,//right
			6,2,1,5,//front
			4,0,3,7,//back
		};
		pDev->CreateIndexBuffer(sizeof(WORD)*24, 
		D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, 
		&_lpIB, NULL);
		WORD*  t_pwIndex;
		_lpIB->Lock(0, 0, (void**)&t_pwIndex, 0);
		memcpy(t_pwIndex, wIndex, sizeof(WORD)*24);
		_lpIB->Unlock();

		////////

		WORD wIndexLine[24] = {
			0,1,1,2,//top
				2,3,3,0,//bottom
				4,5,5,6,//left
				6,7,7,4,//right
				0,4,1,5,//front
				2,6,3,7,//back
		};
		pDev->CreateIndexBuffer(sizeof(WORD)*24, 
			D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, 
			&_lpIBLine, NULL);
		WORD*  t_Index;
		_lpIBLine->Lock(0, 0, (void**)&t_Index, 0);
		memcpy(t_Index, wIndexLine, sizeof(WORD)*24);
		_lpIBLine->Unlock();
	}
	void	setPos(D3DXVECTOR3 vPos)
	{
		D3DXMatrixTranslation(&_matWorld,vPos.x, vPos.y, vPos.z);
	}
	void	setScale(float fx,float fy, float fz)
	{
		D3DXMatrixScaling(&_matScale,fx,fy,fz);
	}
	void	setColor(DWORD	dwColor)
	{
		_dwColor = dwColor;
	}
	void	setWriteFrame(BOOL bWriteFrame)
	{
		_bWriteFrame = bWriteFrame;
	}
	void	Show(bool	bShow){ _bShow = bShow;}
	void	ShowLine(bool	bShow){ _bShowLine = bShow;}
	void	ShowBox(bool	bShow){ _bShowBox = bShow;}

	void	Render()
	{
		if(!_bShow)return;
		g_Render.SetTexture(0,NULL);
		g_Render.SetVertexShader(NULL);
		g_Render.SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);

		const auto matrix = _matScale * _matWorld;
		g_Render.SetTransformWorld(&matrix);
		g_Render.SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE );
		g_Render.SetRenderState(D3DRS_TEXTUREFACTOR, _dwColor );
		g_Render.SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TFACTOR); 
		g_Render.SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1); 
		g_Render.SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TFACTOR); 
		g_Render.SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1); 


		g_Render.SetRenderState(D3DRS_ZENABLE, TRUE);
		g_Render.SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

		g_Render.SetRenderState( D3DRS_LIGHTING, FALSE );
		g_Render.SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

		g_Render.SetStreamSource(0,_lpVB,0, sizeof(BoxVer));

		if(_bShowBox)
		{
			if(_bWriteFrame)
				g_Render.SetRenderState(D3DRS_FILLMODE , D3DFILL_WIREFRAME  );
			else
				g_Render.SetRenderState(D3DRS_FILLMODE , D3DFILL_SOLID   );

			g_Render.SetIndices(_lpIB,0);

			g_Render.DrawIndexedPrimitive( D3DPT_TRIANGLEFAN, 0, 0, 8, 0,  2);
			g_Render.DrawIndexedPrimitive( D3DPT_TRIANGLEFAN, 0, 0, 8, 4,  2);
			g_Render.DrawIndexedPrimitive( D3DPT_TRIANGLEFAN, 0, 0, 8, 8,  2);
			g_Render.DrawIndexedPrimitive( D3DPT_TRIANGLEFAN, 0, 0, 8, 12,  2);
			g_Render.DrawIndexedPrimitive( D3DPT_TRIANGLEFAN, 0, 0, 8, 16,  2);
			g_Render.DrawIndexedPrimitive( D3DPT_TRIANGLEFAN, 0, 0, 8, 20,  2);

			g_Render.SetRenderState(D3DRS_FILLMODE , D3DFILL_SOLID   );
			g_Render.SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW );

		}
		if(_bShowLine)
		{
			g_Render.SetRenderState(D3DRS_TEXTUREFACTOR, 0xffffffff );
			g_Render.SetIndices(_lpIBLine,0);
			g_Render.DrawIndexedPrimitive( D3DPT_LINELIST, 0, 0, 8, 0,  12);
		}
		g_Render.SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE); 
		g_Render.SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
		g_Render.SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);

		g_Render.SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);  
		g_Render.SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);  
		g_Render.SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_MODULATE);  
	}
	void	ReleaseBox()
	{
		SAFE_RELEASE(_lpIB); SAFE_RELEASE(_lpVB);SAFE_RELEASE(_lpIBLine);
	}
protected:
	bool						_bShow;
	bool						_bShowLine;
	bool						_bShowBox;

	D3DXMATRIX					_matWorld;
	D3DXMATRIX					_matScale;
	DWORD						_dwColor;
	BOOL						_bWriteFrame;

	IDirect3DDeviceX*			_pDev;
	IDirect3DIndexBufferX*		_lpIB;
	IDirect3DVertexBufferX*		_lpVB;
	IDirect3DIndexBufferX*		_lpIBLine;
};

extern CEffectBox  g_CEffBox;
extern CEffectBox  CPathBox;

//
#define CMagicEff CEffectObj

class  CMagicEff;

///************************************************************************/
///*CMagicEff*/
///************************************************************************/

//CMPEffectCtrl
#define				PARTCTRL_MSG_PLAY			1
#define				PARTCTRL_MSG_STOP			2	
#define				PARTCTRL_MSG_MOVE			3	
#define				PARTCTRL_MSG_HIT			4	
#define				PARTCTRL_MSG_RENDER			5	


//
struct Eff_Property
{
	int			m_iEffType; //0 =1,2,3,4
	s_string	m_strName;  //

	int			m_iIdxRender;//
};

//!
inline  void	Part_bind(CMagicEff* pEffCtrl);
inline  void	Part_follow(CMagicEff* pEffCtrl);
inline  void	Part_foldir(CMagicEff* pEffCtrl);


inline  void	Part_trace(CMagicCtrl* pEffCtrl, void*	pParam);
inline  void	Part_drop(CMagicCtrl* pEffCtrl, void*	pParam);
inline  void	Part_fly(CMagicCtrl* pEffCtrl, void*	pParam);
inline  void	Part_fshade(CMagicCtrl* pEffCtrl, void*	pParam);
inline  void	Part_arc(CMagicCtrl* pEffCtrl, void*	pParam);
inline  void	Part_dirlight(CMagicCtrl* pEffCtrl, void*	pParam);
inline  void	Part_dist(CMagicCtrl* pEffCtrl, void*	pParam);
inline  void	Part_dist2(CMagicCtrl* pEffCtrl, void*	pParam);


//
inline  void	Part_fan(CMagicEff* pEffCtrl,D3DXVECTOR3* pStart,D3DXVECTOR3* pEnd);
inline  void	Part_sequence(CMagicEff* pEffCtrl,D3DXVECTOR3* pStart,D3DXVECTOR3* pEnd);





class CMagicEff : /*public CMagicCtrl ,*/ public CSceneNode
{
	virtual		BOOL	_Create(int iIdxID,int nType) { return TRUE;}

public:
	CMagicEff();
	~CMagicEff();
	BOOL			Create(int iIdxID);
	BOOL			CreateMagic(int iIdxID);
	BOOL			CreateGroupMagic(int iIdxID);


	BOOL			Create(Eff_Property* pProperty,CMPResManger	*pCResMagr);

	void			SetScene(CGameScene* pScene);

	void			Clear();

	void			FrameMove(DWORD dwDailTime);
	void			Render();

	void			SetInvalidByTime(DWORD dwDailTime);

protected:
	void			RenderMagic();
public:
	//
	void				(*RenderUpdate)(CMagicEff* pEffCtrl);

	friend	void		Part_bind(CMagicEff* pEffCtrl);
	friend  void		Part_follow(CMagicEff* pEffCtrl);
	friend  void		Part_foldir(CMagicEff* pEffCtrl);

	//friend	void		Part_trace(CMagicEff* pEffCtrl);
	//friend  void		Part_drop(CMagicEff* pEffCtrl);
	//friend  void		Part_fly(CMagicEff* pEffCtrl);
	//friend  void		Part_fshade(CMagicEff* pEffCtrl);
	//friend  void		Part_arc(CMagicEff* pEffCtrl);
	//friend  void		Part_dirlight(CMagicEff* pEffCtrl);

	friend  void		Part_fan(CMagicEff* pEffCtrl,D3DXVECTOR3* pStart,D3DXVECTOR3* pEnd);
	friend  void		Part_sequence(CMagicEff* pEffCtrl,D3DXVECTOR3* pStart,D3DXVECTOR3* pEnd);

	//////////////////////////////////////////////////////////////////////////
	void				(*GroupEmission)(CMagicEff* pEffCtrl,D3DXVECTOR3* pStart,D3DXVECTOR3* pEnd);


void			setIdxID(int iIdx)	 { _iIdxID = iIdx;}
	int				getIdxID()			 { return _iIdxID;}


	void			setOwerID(int iIdx)	 { _iOwnerID = iIdx;}
	int				getOwerID()			 { return _iOwnerID;}


	//iType  0:  1Item
	void			setFollowObj(CSceneNode* pObj, NODE_TYPE eType = NODE_CHA, int iDummy = -1, int iDummy2 = -1);

	BOOL            IsSceneEffect()             { return getTypeID() == 0; }
	BOOL            IsChaEffect()				{ return getTypeID() == 1; }
	BOOL            IsObjEffect()				{ return getTypeID() == 2; }
	BOOL            IsArmEffect()				{ return getTypeID() == 3; }
	BOOL            IsMagicEffect()				{ return getTypeID() == 4; }

	BOOL			IsShowBox()					{ return _bShowBox;}
	void			ShowBox(BOOL bshow)			{ _bShowBox = bshow;}

	void			Emission(int iID=-1, D3DXVECTOR3* vBegin=NULL, D3DXVECTOR3* vEnd=NULL, int iTime = 0);

	void			Stop();
	void			End();

	void			MoveTo(const D3DXVECTOR3* vPos);

	void			BindingBone(D3DXMATRIX* pMatBone);

	void			SetVel(float fVel)	
	{
		_fVel = fVel/100;
		for (INT n = 0; n < (INT)_pMagicCtrl.size(); ++n)
		{
			_pMagicCtrl[n]->SetVel(_fVel);
		}
	}

	void			SetEffectDir(int iAngle);
	void			SetEffectMatrix(MPMatrix44* pmat);


	BOOL			HitTestPrimitive( lwVector3& org, lwVector3& ray);
	BOOL			HitTestMap();
	BOOL			HitTestMap(D3DXVECTOR3* vPos);


	void			HitObj();

	void			SetFontEffect(const char* pszText, CMPFont* pFont)
	{
		if(_pEffCtrl)
		{
			_pEffCtrl->GetPartCtrl()->setFontEffect(pszText,pFont);
			_bFoneEff = true;
		}
	}
	void			SetFontEffectCom(VEC_string& vecText, int num,
		CMPResManger *pCResMagr,D3DXVECTOR3* pvDir,int iTexID = 0,D3DXCOLOR dwColor= 0xffffffff,bool bUseBack = false,bool bmain= false)
	{
		if(_pEffCtrl)
		{
			_pEffCtrl->GetPartCtrl()->setFontEffectCom(vecText,num,pCResMagr,pvDir,iTexID,dwColor,bUseBack,bmain);
		}
	}
	BOOL			IsFlyEff(){return _bMagic;}

	void			SetDailTime(DWORD	dwTime)
	{
		_bDail = dwTime > 0 ? TRUE : FALSE;
		_fsDailTime = (float)dwTime /1000;
	}

    CEffDelay*      GetEffDelay()					{ return _pEffDelay;    }

	long			getTag()						{ return _nTag;			}
	void			setTag( long v )				{ _nTag = v;			}
	void			setLoop(bool	bLoop)			{ _bloop=  bLoop;}
	float			GetBaseSize()					{ return _fBaseSize;}
	void	SetSkillCtrl(SkillCtrl* pCtrl)
	{
		if(_bMagic) 
		{
			for (INT n = 0; n < (INT)_pMagicCtrl.size(); ++n)
			{
				_pMagicCtrl[n]->SetSkillCtrl(pCtrl);
			}
		}else if(!_bGroupMagic)
		{
			_pEffCtrl->SetSkillCtrl(pCtrl);
		}
	}
	int				GetLightID()	
	{ 
		if(_bMagic) 
			return _pMagicCtrl[0]->GetLightID();
		return -1;
	}
	void	SetFanAngle(int iAngle)
	{
		_fFanAngle = iAngle * 0.01745329f;
	}
	void	SetMagicDist(float fDist)
	{
		for (WORD n = 0; n < (WORD)_pMagicCtrl.size(); ++n)
		{
			_pMagicCtrl[n]->SetTargetDist(fDist);
		}
	}

	void	SetUpdateHieght(bool bUpdate)	{ _bUpdateHei = bUpdate;	}

	DWORD	GetStartTime()					{ return _dwStartTime;		}

	//  Michael 2005.12.8
	void	SetScale(float fX, float fY, float fZ) { _UpdateScale(fX, fY, fZ); }
	void	SetAlpha(float fAlpha) { 	_pEffCtrl->GetPartCtrl()->SetAlpha(fAlpha); }

	void	SetLoop(bool bLoop) { this->_bloop = bLoop; }
	bool	IsLoop() { return _bloop; }

protected:
	
    virtual void	_UpdateYaw();
	virtual void	_UpdatePitch();
	virtual void	_UpdateRoll();
    virtual void    _UpdatePos();
    virtual void    _UpdateHeight();
	virtual void    _UpdateValid(BOOL bValid);

    virtual void	_UpdateScale(float fx,float fy,float fz);

Effect_Type			_eType;
    CEffDelay*          _pEffDelay;
	CSceneNode*			_pObj;//

	MPTerrain*			_pTerrain;
	long				_nTag;

	float*				_pDailTime;

	bool				_bPlay;


	int					_iDummy;
	float				_fVel;
	float				_fCurDist;

	int					_iIdxID;
	int				_iTargetID;

	int				_iOwnerID;
	BOOL			_bGroupMagic;
	int				_iGroupIdx;

	BOOL			_bMagic;
	BOOL			_bFoneEff;

	BOOL			_bMagicEm;


	CMPEffectCtrl*	_pEffCtrl;
	std::vector<CMagicCtrl*>	_pMagicCtrl;

	float			_fFanAngle;

	float			_fBaseSize;
	D3DXVECTOR3			_vMapTarget;
	float				_fHei;

private:
	bool				_bloop;
	BOOL				_bShowBox;
	D3DXVECTOR3			_vMin;
	D3DXVECTOR3			_vMax;

	bool				_bUpdateHei;

protected:
	BOOL				_bDail;
	float				_fsCurTime;
	float				_fsDailTime;
	int					_isID;
	D3DXVECTOR3			_vsBegin;
	D3DXVECTOR3			_vsEnd;
	DWORD				_dwStartTime;		// 

};

//////////////////////////////////////////////////////////////////////////
/************************************************************************/
/*class CMagicCreate*/
/************************************************************************/
//////////////////////////////////////////////////////////////////////////
/************************************************************************/
/*class CShadeEff*/
/************************************************************************/
class CShadeInfo;
class CShadeEff : public CMPShadeCtrl , public CSceneNode
{
public:
	CShadeEff();
	~CShadeEff();
private:
	virtual		BOOL	_Create(int iIdxID,int nType){return TRUE;}

public:
	bool			Create(int iIdxID);
	bool			Create( CShadeInfo* pInfo);
	bool			Create(s_string strTexName, float fSize, bool bAni,int iRow, int iColnum);

	bool			CreateAttachLight(int iIdxID, float fRange,D3DXCOLOR dwcolor);

	void			SetScene(CGameScene* pScene);
	void			Clear();

	void			FrameMove(DWORD dwDailTime);
	void			Render();

    void			setIdxID(int iIdx)	 { _iIdxID = iIdx;}
	int				getIdxID()			 { return _iIdxID;}

	void			setChaID(int iIdx)	 { _iChaID = iIdx;}
	int				getChaID()			 { return _iChaID;}

	void			setUpSea(bool bsea)	 { _bUpSea = bsea;_pShadeMap->SetUpSea(_bUpSea);}
	bool			IsUpSea()			 { return _bUpSea;}

	void			Emission(WORD wID, const D3DXVECTOR3* vBegin, D3DXVECTOR3* vEnd);

	void			MoveTo (D3DXVECTOR3* SVerPos);

protected:
	
    virtual void	_UpdateYaw()  {}
	virtual void	_UpdatePitch(){}
	virtual void	_UpdateRoll() {}

	virtual void    _UpdatePos()	{}
	virtual void    _UpdateHeight()	{}

    //CGameScene*		_pScene;
	MPTerrain*      _pTerrain;
	int				_iChaID;//ID

	int				_iIdxID;

	bool			_bUpSea;
};

class CPug
{
public:
	CPug();
	~CPug();

	bool		Create(D3DXVECTOR3* pvPos, float fangle, MPMap* pMap);

	void		MoveTo(MPMap* pMap);
	void		FrameMove(DWORD dwTime);
	void		Render();

	bool		IsValid()	{ return _bValid; }
protected:
	CMPShadeCtrl	_cShadeEff;
	float			_fAngle;
	bool			_bValid;
	D3DXCOLOR		_dwColor;
	float			_fCurTime;
	D3DXVECTOR3		_vPos;
};

#define		MAXPUG_COUNT	100
class CPugMgr
{
public:
	CPugMgr();
	~CPugMgr();

	void	InitMemory(MPMap* pMap);
	void	ClearMemory();

	void	BeginPug()	{ _bPug = true; }
	void	EndPug()	{ _bPug = false; }

	void	NewPug(D3DXVECTOR3* pvPos, float fangle);

	void	FrameMove(DWORD dwTime);
	void	Render();
protected:
	MPMap*					_pMap;
	std::vector<CPug*>		_vecPugArray;
	S_FVector<WORD>			_vecValidID;

	bool					_bPug;

	bool					_bLeft;
};

class CNavigationBar
{
public:
	CNavigationBar()
	{
		_bShow = false;
		_pShadeEff = NULL;
		_vTarget = D3DXVECTOR3(0,0,0);
		_strName = "";
	}
	~CNavigationBar()
	{
	}

	void		Show(bool bshow)	{ _bShow = bshow; }
	bool		IsShow()			{ return _bShow;}
	void SetTarget(const char* pszName, D3DXVECTOR3& pTarget);

	void		Render();

	void		Clear()
	{
		SAFE_DELETE(_pShadeEff);
	}
	static	CNavigationBar	g_cNaviBar;
protected:

	CMPShadeCtrl*	_pShadeEff;

	bool			_bShow;
	D3DXVECTOR3		_vTarget;

	s_string		_strName;
};
