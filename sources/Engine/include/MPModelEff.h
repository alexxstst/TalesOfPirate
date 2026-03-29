#pragma once
//#include "i_effect.h"
#include "MindPowerAPI.h"



#include "mpresmanger.h"
//#include "EffectFile.h"
class   MPRender;

class CEffectCortrol
{
public:
	CEffectCortrol(void)
	{
		m_bPlay = false;

		m_fCurTime = 0.0f;
		m_wCurFrame		= 0;
		m_dwCurColor		= D3DCOLOR_ARGB(0,255,255,255);

		m_SCurSize			= D3DXVECTOR3(1.0f,1.0f,1.0f);
		m_SCurAngle			= D3DXVECTOR3(0,0,0);
		m_SCurPos			= D3DXVECTOR3(0,0,0);

		m_vecCurCoord.clear();
		m_wCurCoordIndex	= 0;
		m_fCurCoordTime		= 0;
		m_wCurTexIndex		= 0;
		m_fCurTexTime		 =0;
		m_lpCurTex.clear();

		m_iCurTimes = 0;

		m_vecCurCoord.resize(10);
		m_lpCurTex.resize(10);
	}
	//~CEffectCortrol(void);
public:
	void	Reset()
	{
		m_bPlay = false;
		m_fCurTime = 0.0f;
		m_wCurFrame		= 0;
		m_wCurCoordIndex	= 0;
		m_fCurCoordTime		= 0;
		m_wCurTexIndex		= 0;
		m_fCurTexTime		 =0;

		//m_SCurSize			= D3DXVECTOR3(1.0f,1.0f,1.0f);
		//m_SCurAngle			= D3DXVECTOR3(0,0,0);
		//m_SCurPos			= D3DXVECTOR3(0,0,0);

	}
	void    Play()
	{
		//Reset();
		m_iCurTimes = 0;
		m_bPlay = true;
	}
	void	Stop()						
	{
		Reset();
		m_bPlay = false;
	}
	bool	IsPlay()
	{
		return m_bPlay;
	}

	void GetTransformMatrix(D3DXMATRIX* pSOut,D3DXMATRIX* pRota = NULL)
	{
		D3DXMATRIX t_SMat, t_SMatRot;
		D3DXMatrixScaling(&t_SMat,m_SCurSize.x,m_SCurSize.y,m_SCurSize.z);
		if(!pRota)
		{
			D3DXMatrixRotationYawPitchRoll(&t_SMatRot,
											m_SCurAngle.y,m_SCurAngle.x,m_SCurAngle.z);
			D3DXMatrixMultiply(pSOut, &t_SMat, &t_SMatRot);
		}
		else
		{
			D3DXMATRIX t_mat;
			D3DXMatrixRotationYawPitchRoll(&t_SMatRot,
				m_SCurAngle.y,m_SCurAngle.x,m_SCurAngle.z);

			D3DXMatrixMultiply(&t_mat, &t_SMatRot, pRota);

			D3DXMatrixMultiply(pSOut, &t_SMat, &t_mat);
		}
		pSOut->_41 = m_SCurPos.x ;
		pSOut->_42 = m_SCurPos.y ;
		pSOut->_43 = m_SCurPos.z ;
	}
	void FillModelUV(CEffectModel*	pCModel);
	//{
	//	for(WORD i = 0; i < pCModel->GetVerCount(); i++)
	//	{
	//		pCModel->GetDev()->SetVertexShaderConstant(9 + i, m_vecCurCoord[i], 1);
	//	}
	//}
	void FillTextureUV(CEffectModel*	pCModel);
	//{
	//	for(WORD i = 0; i < pCModel->GetVerCount(); i++)
	//	{
	//		pCModel->GetDev()->SetVertexShaderConstant(9 + i, m_lpCurTex[i], 1);
	//	}
	//}

	void FillDefaultUV(CEffectModel*	pCModel,TEXCOORD& coord);

	void FillModelUVSoft(CEffectModel*	pCModel)
	{
		SEFFECT_VERTEX *pVertex;
		pCModel->Lock((BYTE**)&pVertex);
		for(WORD i = 0; i < pCModel->GetVerCount(); ++i)
		{
			pVertex[i].m_SUV = *m_vecCurCoord[i];
		}
		pCModel->Unlock();

	}
	void FillTextureUVSoft(CEffectModel*	pCModel)
	{
		SEFFECT_VERTEX *pVertex;
		pCModel->Lock((BYTE**)&pVertex);
		for(WORD i = 0; i < pCModel->GetVerCount(); ++i)
		{
			pVertex[i].m_SUV = *m_lpCurTex[i];
		}
		pCModel->Unlock();
	}
	//////////////////////////////////////////////////////////////////////////
	//!
	float				m_fCurTime;
	//!
	WORD				m_wCurFrame;
	//!Diffuse
	D3DXCOLOR			m_dwCurColor;
	//!
	D3DXVECTOR3			m_SCurSize;
	//!
	D3DXVECTOR3		m_SCurAngle;
	//!	
	D3DXVECTOR3			m_SCurPos;

	/////////////////////////////////////////////////////////////////////////
	//!
	WORD					m_wCurCoordIndex;
	//!
	float					m_fCurCoordTime;
	//!
	S_BVECTOR<D3DXVECTOR2>	m_vecCurCoord;
	////////////////////////////////////////////////////////////////////
	//!
	WORD					m_wCurTexIndex;
	//!
	float					m_fCurTexTime;
	//!
	S_BVECTOR<D3DXVECTOR2>	m_lpCurTex;
	////////////////////////////////////////////////////////////
	int						m_iCurTimes;

	bool					m_bPlay;
	//FVF_STRUCT              buf;

	D3DXMATRIX				m_SMatResult;

	float					m_fCurRotat;			
};


class MINDPOWER_API CEffPath
{
public:
	CEffPath(){	
		//m_vecPath = NULL;
		//m_vecDist = NULL;
		//m_vecDir = NULL;
		m_fVel = 1.f;m_iCurFrame = 0;m_fCurDist = 0;
		m_bEnd = false;
	}
	~CEffPath(){		
	//	SAFE_DELETE_ARRAY(m_vecPath);
	//SAFE_DELETE_ARRAY(m_vecDist);
	//SAFE_DELETE_ARRAY(m_vecDir);
	}
	void	Copy(CEffPath* pPath)
	{
		m_iFrameCount = pPath->m_iFrameCount;
		//m_vecPath = new D3DXVECTOR3[m_iFrameCount];
		//m_vecDist = new float[m_iFrameCount-1];
		//m_vecDir = new D3DXVECTOR3[m_iFrameCount-1];
		memcpy(m_vecPath,pPath->m_vecPath,sizeof(D3DXVECTOR3) * m_iFrameCount);
		memcpy(m_vecDist,pPath->m_vecDist,sizeof(float) * (m_iFrameCount-1));
		memcpy(m_vecDir,pPath->m_vecDir,sizeof(D3DXVECTOR3) * (m_iFrameCount-1));
		m_fVel = pPath->m_fVel;
		Reset();
	}

	void				Reset()		{ m_iCurFrame = 0;m_vCurPos = m_vecPath[0];m_fCurDist = 0;}
	void				SetVel(float fvel)	{ m_fVel = fvel;}
	float				GetVel()			{ return m_fVel;}

	bool				LoadPathFromFile(char* pszName);
    bool                LoadPathFromFileLet(const char* file);
	void				SavePath(FILE* pf);
	void				LoadPath(FILE* pf);

	void				LoadPathFromMemory(CMemoryBuf*	pbuf);


	void				FrameMove(float fDailTime)
	{
		if(m_iFrameCount<=0)
			return;
		m_bEnd = false;

		float fvel = m_fVel * fDailTime;
		m_fCurDist += fvel;
		while (m_fCurDist >= m_vecDist[m_iCurFrame])
		{
			m_fCurDist -= m_vecDist[m_iCurFrame];
			m_iCurFrame++;
			if(m_iCurFrame >= m_iFrameCount-1)
			{
				m_iCurFrame = 0;
				m_bEnd = true;
			}
		}
		m_vCurPos = m_vecPath[m_iCurFrame] + (m_vecDir[m_iCurFrame] * m_fCurDist);
	}
	D3DXVECTOR3*		GetCurPos()			{ return &m_vCurPos;}
	D3DXVECTOR3*		GetNextPos()		
	{ 
		if(m_iCurFrame >= m_iFrameCount - 1)
			return &m_vCurPos;
		return &m_vecPath[m_iCurFrame - 1];
	}


	D3DXVECTOR3*		GetCurDir()			{ return &m_vecDir[m_iCurFrame];}
	float				GetCurDist()		{ return m_vecDist[m_iCurFrame];}

	D3DXVECTOR3*		GetStart()			{ return &m_vecPath[0];}
	D3DXVECTOR3*		GetEnd()			{ return &m_vecPath[m_iFrameCount - 1];}

	bool				IsEnd()				{ return m_bEnd; }

public:
	//D3DXVECTOR3*				m_vecPath;
	//float*						m_vecDist;
	//D3DXVECTOR3*			    m_vecDir;
	bool					m_bEnd;

	D3DXVECTOR3				m_vecPath[200];
	float					m_vecDist[200];
	D3DXVECTOR3			    m_vecDir[200];

	int							m_iFrameCount;
	float						m_fVel;

	int							m_iCurFrame;
	float						m_fCurDist;
	D3DXVECTOR3					m_vCurPos;
};

class CEffPathCtrl
{
public:
	CEffPathCtrl(){m_fCurTime  = 0;m_fFrameTime = 0.1f;}
	~CEffPathCtrl(){}

	void	Update(float dwDailTime,CEffPath* pPath )
	{
		m_fCurTime += dwDailTime;
		if(m_fCurTime >= m_fFrameTime)
		{
			m_iCurFrame++;
			if(m_iCurFrame >= pPath->m_iFrameCount)
				m_iCurFrame = 0;
			m_fCurTime = 0;
		}
		if(m_iCurFrame  == pPath->m_iFrameCount -1)
			m_iNextFrame = m_iCurFrame;
		else
			m_iNextFrame = m_iCurFrame + 1;
		m_fLerp = m_fCurTime / m_fFrameTime;

		//pPath->GetCurPos(&m_SCurPath,m_iCurFrame,m_iNextFrame,m_fLerp);
	}
public:
	int			m_iCurFrame;
	int			m_iNextFrame;
	float		m_fLerp;

	float		m_fCurTime;
	float		m_fFrameTime;

	D3DXVECTOR3	m_SCurPath;
};


class	EffParameter
{
public:
	EffParameter()
	{
		m_bUsePath = false;
		m_szPathName = "";
		m_bUseSound = false;
		m_szSoundName =  "";
		m_iIdxTech = 0;
		m_bRotating = false;
		m_fRotaVel = 1;
		m_SVerRota = D3DXVECTOR3(0,0,0);
	}
	~EffParameter(){}

	bool	m_bUsePath;
	s_string	m_szPathName;
	bool	m_bUseSound;
	s_string	m_szSoundName;
	int		m_iIdxTech;
		
	bool	m_bRotating;
	float	m_fRotaVel;
	D3DXVECTOR3	m_SVerRota;
};

class  MINDPOWER_API  CMPModelEff// : public CEffectBase
{
public:
	CMPModelEff(void);
	~CMPModelEff(void);
public:
	//!	
	virtual void ReleaseAll();
	//!
	virtual void Reset();
	//!
	virtual void FrameMove(DWORD	dwDailTime);
	//!
	virtual void Render();
	virtual void RenderVS();
	virtual void RenderSoft();

	void FrameMoveAccel(float fDail);
	void Begin();
	void RenderAccel(float &fTime);
	void End();

	void		 ShowCurFrame(int iCurSubEff, int iCurFrame);

	void		 ShowTempFrame(int iCurSubEff,
		D3DXVECTOR3& pScale, D3DXVECTOR3& pRotating, D3DXVECTOR3& pTranslate,
		D3DXCOLOR& pColor,TEXCOORD& vecCoord, IDirect3DTextureX* lpTex);

	//!
	bool		 SaveToFile(char* pszFileName);
	//!
	bool		 LoadFromFile(char* pszFileName);

	bool		 IsLoop()					{ return m_bLoop; }
	bool		 IsPlaying()				{ return m_bPlay;}
	bool		 IsPlay()				
	{ 
		for(int n = 0; n < m_iEffNum; n++)
		{
			if(m_vecCortrol[n]->IsPlay())
				return true;
		}
		return false;
	}

	//!
	void		 Play(int iTime = 0)	/*iTime: 0 == loop, */	
	{
		//Reset();
		m_bPlay = true;
		if(iTime > 0)
		{
			m_bLoop = false;
			m_iTimes = iTime;
		}
		else
		{
			m_bLoop = true;
			m_iTimes = 0;
		}
		for(int n = 0; n < m_iEffNum; n++)
		{
			m_vecCortrol[n]->Play();
			m_vecEffect[n]->PlayModel();
		}
	}
	void		 Play2(int iTime = 0)	/*iTime: 0 == loop, */	
	{
		//Reset();
		m_bPlay = true;
		if(iTime > 0)
		{
			m_bLoop = false;
			m_iTimes = iTime;
		}
		else
		{
			m_bLoop = true;
			m_iTimes = 0;
		}
		for(int n = 0; n < m_iEffNum; n++)
		{
			m_vecCortrol[n]->Play();
		}
	}
	void		 Stop()						
	{
		Reset();
		m_bPlay = false;	
		m_iTimes = 0;
		//m_iCurTimes = 0;
	}

	void		FreeEffect()
	{
		m_vecEffect.clear();
		m_iEffNum = 0;
	}
	void		BindingEffect(I_Effect*	pCEffect)			
	{
		m_pCEffect = pCEffect;

		m_iEffNum++;

		m_vecEffect.resize(m_iEffNum);
		m_vecEffect[m_iEffNum - 1] = pCEffect;

		m_vecCortrol.setsize(m_iEffNum);
		m_vecCortrol[m_iEffNum - 1]->m_vecCurCoord.setsize((WORD)pCEffect->m_pCModel->GetVerCount());
		m_vecCortrol[m_iEffNum - 1]->m_lpCurTex.setsize((WORD)pCEffect->m_pCModel->GetVerCount());

	}
	
	void		BindingEffect(std::vector<I_Effect>&	CEffectArray)			
	{
		ClearEffect();
		int n;
		m_bPlay = false;

		m_iEffNum = (int)CEffectArray.size();
		m_vecEffect.clear();

		m_vecEffect.resize(m_iEffNum);
		for( n = 0; n < m_iEffNum; n++)
		{
				m_vecEffect[n] = new I_Effect;
				m_vecEffect[n]->CopyEffect(&CEffectArray[n]); 
		}

		m_vecCortrol.resize(m_iEffNum);
		m_vecCortrol.setsize(m_iEffNum);
		for(n = 0; n < m_iEffNum; n++)
		{
			m_vecCortrol[n]->m_vecCurCoord.setsize(CEffectArray[n].m_CTexCoordlist.m_wVerCount);
			//m_vecCortrol[n]->m_lpCurTex.clear();
			m_vecCortrol[n]->m_lpCurTex.setsize(CEffectArray[n].m_CTexCoordlist.m_wVerCount);
		}
	}
	void	    BindingRes(CMPResManger* pResMagr)			
	{ 
		m_bPlay = false;	

		m_pResMgr = pResMagr;
		int n;
		for( n = 0; n < m_iEffNum; n++)
			m_vecEffect[n]->BoundingRes(pResMagr);

		m_pfDailTime = pResMagr->GetDailTime();
		m_pCEffectFile = pResMagr->GetEffectFile();
		int idx = pResMagr->GetEffectID(m_vecEffect[0]->getEffectName());

		if( idx == -1 )
		{
			char szData[128];
			sprintf( szData, "(ID%d)", idx );
			MessageBox( NULL, szData, "Error", MB_OK );
		}

		EffParameter *pParam = pResMagr->GetEffectParamByID(idx);
		m_iIdxTech = pParam->m_iIdxTech;/*pResMagr->GetEffectTechByID(idx);*/

		m_bUsePath = pParam->m_bUsePath;
		m_bUseSound = pParam->m_bUseSound;
		m_strPathName = pParam->m_szPathName;
		m_strSoundName = pParam->m_szSoundName;

		m_bRotating = pParam->m_bRotating;
		m_fRotaVel  = pParam->m_fRotaVel;
		m_SVerRota	= pParam->m_SVerRota;

		if(m_bUsePath)
		{
			m_pPath = pResMagr->GetEffPath(pResMagr->GetEffPathID(m_strPathName));
		}

		m_pMatViewProj = pResMagr->GetViewProjMat();
		m_bUseSoft = pResMagr->m_bUseSoft;

		D3DXMatrixIdentity(&m_SMatTempRota);
	}

	int			GetSubEffectFrameCount(int idx)				
	{ 
		if(m_iEffNum <=0)
			return 0;

		return m_vecEffect[idx]->getFrameCount();
	}
	float		GetSubEffectFrameTime(int idx, int iframe)	
	{ 
		if(m_iEffNum <=0)
			return 0;

		return m_vecEffect[idx]->getFrameTime(iframe);
	}


	void		BindingBone(D3DXMATRIX pmatBone, bool bFollow = false)			
	{
		m_bBindbone = bFollow;
		if(m_bBindbone)
			m_SpmatBone = pmatBone;
		else
			D3DXMatrixIdentity(& m_SpmatBone);
	}

	void		Scaling(float x, float y, float z)
	{
		D3DXMatrixScaling(&m_SmatScale,x,y,z);
		m_SVerScale.x = x;
		m_SVerScale.y = y;
		m_SVerScale.z = z;
	}
	float		GetScalingX()		{ return m_SVerScale.x;}
	float		GetScalingY()		{ return m_SVerScale.y;}
	float		GetScalingZ()		{ return m_SVerScale.z;}

	void		RotatingYaw(float fDeg)//
	{
		if(m_bRotating)
			return;
		m_SVerRota.z = fDeg;

		D3DXMatrixRotationYawPitchRoll(&m_SMatTempRota,
			m_SVerRota.y,m_SVerRota.x,m_SVerRota.z);
	}
	float		GetRotaingYaw()		{ return m_SVerRota.z;}

	void		RotatingPitch(float fDeg)//!
	{
		if(m_bRotating)
			return;
		m_SVerRota.x = fDeg;

		D3DXMatrixRotationYawPitchRoll(&m_SMatTempRota,
			m_SVerRota.y,m_SVerRota.x,m_SVerRota.z);

	}
	float		GetRotaingPitch()		{ return m_SVerRota.x;}

	void		RotatingRoll(float fDeg)//!
	{
		if(m_bRotating)
			return;
		m_SVerRota.y = fDeg;

		D3DXMatrixRotationYawPitchRoll(&m_SMatTempRota,
			m_SVerRota.y,m_SVerRota.x,m_SVerRota.z);
	}
	float		GetRotaingRoll()		{ return m_SVerRota.y;}


	void		RotatingAxis(D3DXVECTOR3* pAxis, float fAngle)
	{
		D3DXMatrixRotationAxis(&m_SmatRota, pAxis,fAngle);
	}
	void		MoveTo(float x, float y, float z)
	{
		D3DXMatrixTranslation(&m_SmatTrans,x, y, z);
		m_SVerTrans.x = x;
		m_SVerTrans.y = y;
		m_SVerTrans.z = z;
	}
	//
	//FrameMove,Render
	void		SetAlpha(float fAlpha)
	{
		for(int n = 0; n < m_iEffNum; ++n)
		{
			m_pCurCortrol = m_vecCortrol[n];
			m_pCurCortrol->m_dwCurColor.a = fAlpha;
		}
	}

	void	GetRunningDummyMatrix(D3DXMATRIX* pmat, int idx)
	{
		if(m_vecEffect[0]->IsItem())
		{
			m_vecEffect[0]->m_pCModel->GetObjDummyRunTimeMatrix((lwMatrix44*)pmat,idx);
		}
	}

	void	SetRenderIdx(int idx)
	{
		m_iIdxTech = idx;
	}
	void	UseZBuffer(bool bused)
	{
		m_bUseZ = bused;
	}

//////////////////////////////////////////////////////////////////////////
	void		RotatingXZ(float fAngleX, float fAngleZ)
	{
		D3DXMATRIX mat;
		D3DXMatrixRotationX(&mat, fAngleX);
		D3DXMatrixRotationZ(&m_SMatTempRota, fAngleZ);
		D3DXMatrixMultiply(&m_SMatTempRota, &mat, &m_SMatTempRota);
	}


	void		RotatingYawPart(float fDeg)//
	{
		m_SVerPartRota.z = fDeg;
		D3DXMatrixRotationYawPitchRoll(&m_SMatTempRota,
			m_SVerPartRota.y,m_SVerPartRota.x,m_SVerPartRota.z);
	}
	void		RotatingPitchPart(float fDeg)//!
	{
		m_SVerPartRota.x = fDeg;
		D3DXMatrixRotationYawPitchRoll(&m_SMatTempRota,
			m_SVerPartRota.y,m_SVerPartRota.x,m_SVerPartRota.z);
	}

	void		RotatingRollPart(float fDeg)//!
	{
		m_SVerPartRota.y = fDeg;
		D3DXMatrixRotationYawPitchRoll(&m_SMatTempRota,
			m_SVerPartRota.y,m_SVerPartRota.x,m_SVerPartRota.z);
	}
	//////////////////////////////////////////////////////////////////////////
	
	void	GetTransMatrix(D3DXMATRIX&	mat)
	{
		if(m_bRotating)
		{
			m_fCurRotat += m_fRotaVel * *m_pfDailTime;
			if(m_fCurRotat >= 6.283185f)
				m_fCurRotat = m_fCurRotat - 6.283185f;
			D3DXMatrixRotationAxis(&m_SmatRota,
				&m_SVerRota, m_fCurRotat);
		}

		D3DXMatrixMultiply(&mat, &m_SmatRota, &m_SMatTempRota);

		D3DXMatrixMultiply(&mat, &m_SmatScale, &mat);
		//D3DXMatrixMultiply(&mat, &mat, &m_SMatTempRota);
		D3DXMatrixMultiply(&mat, &mat, &m_SmatTrans);
	}

	void	ClearEffect();
public:
	CMPResManger*					m_pResMgr;
	bool							m_bUseSoft;
	int								m_iEffNum;

	std::vector<I_Effect*>			m_vecEffect;
	I_Effect*						m_pCEffect;

	S_BVECTOR<CEffectCortrol>		m_vecCortrol;
	CEffectCortrol*					m_pCurCortrol;


	float								m_fLerp;
	//////////////////////////////////////////////////////////////////////////
	bool								m_bLoop;

	bool								m_bPlay;

	int									m_iTimes;

	float*								m_pfDailTime;

	/////////////////////////////////////////////////////////
	D3DXMATRIX							m_SmatScale;
	D3DXVECTOR3							m_SVerScale;

	D3DXMATRIX							m_SmatRota;
	D3DXVECTOR3							m_SVerRota;

	D3DXVECTOR3							m_SVerPartRota;

	D3DXMATRIX							m_SmatTrans;
	D3DXVECTOR3							m_SVerTrans;

	bool								m_bBindbone;
	D3DXMATRIX							m_SpmatBone;
	D3DXMATRIX							m_SMatResult;

	D3DXMATRIX							m_SMatTempRota;

	int									m_iIdxTech;
	CMPEffectFile*						m_pCEffectFile;

	D3DXMATRIX*							m_pMatViewProj;
	////////////////////////////////////////////////////////////////////////
	bool								m_bUsePath;
	s_string							m_strPathName;
	CEffPath*							m_pPath;
	CEffPathCtrl						m_CPathCtrl;
	///////////////////////////////////////////////////////////////////////
	bool								m_bUseSound;
	s_string							m_strSoundName;
	///////////////////////////////////////////////////////////////////////
	bool								m_bRotating;
	float								m_fRotaVel;
	float								m_fCurRotat;

	bool								m_bUseZ;
};


/************************************************************************/
/*                                                                      */
/************************************************************************/
struct Strip_Vertex
{
	D3DXVECTOR3		m_SPos;
	DWORD			m_dwDiffuse;
	D3DXVECTOR2		m_SUV;
};
#define		STRIP_FVF	(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)

class MINDPOWER_API CMPStrip
{
public:
	CMPStrip();
	~CMPStrip();
#ifdef		USE_RENDER
	void	CreateStrip(MPRender*	pDev, const s_string& strTexName,CMPResManger* pResMagr)
#else
	void	CreateStrip(LPDIRECT3DDEVICE8	pDev,const s_string& strTexName,CMPResManger* pResMagr)
#endif
	{
		m_pDev = pDev;
		_strTexName = strTexName;
		BindingRes(pResMagr);
	}
	void	BindingRes(CMPResManger* pResMagr)
	{
		m_pDev= pResMagr->m_pDev;
		_pCEffFile = pResMagr->GetEffectFile();
		int id = pResMagr->GetTextureID(_strTexName);
		if(id <0)
		{
			_pTex = NULL;
		}else
		{
#ifdef USE_RENDER
			//pResMagr->GetTextureByID(id);
			_pTex = pResMagr->GetTextureByIDlw(id);
#else
			_pTex = pResMagr->GetTextureByID(id);
#endif
		}
		_pfDailTime = pResMagr->GetDailTime();

		_vecPath.resize(m_iMaxLen);
		_vecCtrl.resize(m_iMaxLen/2);
	}

	struct track
	{
		track(){ m_fCurTime = 0;}
		void	FrameMove(float fDailTime,D3DXCOLOR& dwColor,float fLife)
		{
			if(m_fCurTime >= fLife)
			{
				dwColor.a = 0;
				return;
			}
			dwColor.a = 1.0f + ((-1.0f)* (m_fCurTime/fLife));
			//D3DXColorLerp(&dwColor,&D3DXCOLOR(1,1,1,1),&D3DXCOLOR(1,1,1,0),m_fCurTime/1.0f);
			m_fCurTime += fDailTime;
		}
		float	m_fCurTime;
	};

	void	Play();
	bool	IsPlaying()							{return _bPlay;}

	void	UpdateFrame();
	void	FrameMove();
	void	Render();

public:
	void	AttachCharacter(MPCharacter* pCha)	{ _pCha = pCha;}
	void	AttachItem(MPSceneItem* pItem)	{ _pItem = pItem;}

	void	SetDumy(int iIdx, int iDummy)	{ _iDummy[iIdx] = iDummy; }
	int 	GetDumy(int iIdx)				{ return _iDummy[iIdx]; }


	void	GetTrack(lwMatrix44* dummy1,lwMatrix44* dummy2)
	{
		Strip_Vertex path;
		track tul;
		int count = _vecPath.size();
		float d = (float)1/*(m_iMaxLen/2)*/;
		if(count >=m_iMaxLen)
		{
			if(!m_bLoop)
			{
				_bPlay = false;
				return;
			}else
			{
				m_iMaxLen += 100;
				_vecPath.addsize(100);
				_vecCtrl.addsize(50);
				if(m_iMaxLen > 2000)
				{
					_bPlay = false;
					return;
				}
			}
		}
		if(count<=0 )
		{
			//_vecPath.clear();
			//_vecCtrl.clear();
			//path.m_SPos = *(D3DXVECTOR3*)&dummy1->_41;
			path.m_SPos.x = dummy1->_41;
			path.m_SPos.y = dummy1->_42;
			path.m_SPos.z = dummy1->_43;

			path.m_dwDiffuse = _dwColor;
			path.m_SUV.x = 0;
			path.m_SUV.y = 1; 
			_vecPath.push_back(path);

			//path.m_SPos = *(D3DXVECTOR3*)&dummy2->_41;
			path.m_SPos.x = dummy2->_41;
			path.m_SPos.y = dummy2->_42;
			path.m_SPos.z = dummy2->_43;

			path.m_dwDiffuse = _dwColor;
			path.m_SUV.x = 0;
			path.m_SUV.y = 0; 
			_vecPath.push_back(path);
			_vecCtrl.push_back(tul);


			//path.m_SPos = *(D3DXVECTOR3*)&dummy1->_41;
			//path.m_dwDiffuse = 0xffffffff;
			//path.m_SUV.x = (float)1/m_iMaxLen;
			//path.m_SUV.y = 1; 
			//_vecPath.push_back(path);

			//path.m_SPos = *(D3DXVECTOR3*)&dummy2->_41;
			//path.m_dwDiffuse = 0xffffffff;
			//path.m_SUV.x = (float)1/m_iMaxLen;
			//path.m_SUV.y = 0; 
			//_vecPath.push_back(path);
			//_vecCtrl.push_back(tul);

		}else
		{

			//path.m_sVer[0] = _vecPath.end()->m_sVer[2];

			//path.m_sVer[1] = _vecPath.end()->m_sVer[3];

			//path.m_sVer[0].m_SUV.x = 0;
			//path.m_sVer[0].m_SUV.y = 1; 

			//path.m_sVer[1].m_SUV.x = (float)1/count;
			//path.m_sVer[1].m_SUV.y = 0; 

			//path.m_SPos = *(D3DXVECTOR3*)&dummy1->_41;
			path.m_SPos.x = dummy1->_41;
			path.m_SPos.y = dummy1->_42;
			path.m_SPos.z = dummy1->_43;

			path.m_dwDiffuse = _dwColor;
			path.m_SUV.x = _vecPath[count-2]->m_SUV.x +d;
			path.m_SUV.y =  1; 
			_vecPath.push_back(path);

			//path.m_SPos = *(D3DXVECTOR3*)&dummy2->_41;
			path.m_SPos.x = dummy2->_41;
			path.m_SPos.y = dummy2->_42;
			path.m_SPos.z = dummy2->_43;

			path.m_dwDiffuse = _dwColor;
			path.m_SUV.x = _vecPath[count-1]->m_SUV.x +d;
			path.m_SUV.y = 0; 
			_vecPath.push_back(path);
			_vecCtrl.push_back(tul);

		}
	}
	float				GetLife()						{return  _fLife;}
	void				SetLife(float flife)			{_fLife = flife;}
	float				GetStep()						{return _fStep;}
	void				SetStep(float fstep)			{_fStep = fstep;}
	D3DXCOLOR			GetColor()						{return _dwColor;}
	void				SetColor(D3DXCOLOR color)		{_dwColor = color;}

	int					GetMaxLength()					{return m_iMaxLen;}
	void				SetMaxLength(int len)
	{
		m_iMaxLen = len;
		_vecPath.resize(m_iMaxLen);
		_vecCtrl.resize(m_iMaxLen/2);
	}

	s_string			GetTexName()					{return _strTexName;}
	void				SetTexName(const s_string& str ,CMPResManger* pResMagr)
	{
		_strTexName = str;
		int id = pResMagr->GetTextureID(_strTexName);
		if(id <0)
			_pTex = NULL;
		else
		{
#ifdef USE_RENDER
			//pResMagr->GetTextureByID(id);
			_pTex = pResMagr->GetTextureByIDlw(id);
#else
			_pTex = pResMagr->GetTextureByID(id);
#endif
		}
	}

	D3DBLEND			GetSrcBlend(){return _eSrcBlend;}
	void				SetSrcBlend(D3DBLEND srcblend){ _eSrcBlend = srcblend;}
	D3DBLEND			GetDestBlend(){return _eDestBlend;}
	void				SetDestBlend(D3DBLEND destblend){ _eDestBlend = destblend;}

	bool		 SaveToFile(FILE* t_pFile);
	bool		 LoadFromFile(FILE* t_pFile, DWORD dwVersion);

	bool		 LoadFromMemory(CMemoryBuf* pbuf, DWORD dwVersion);


	void		CopyStrip(CMPStrip* pstrip);

	void		SetLoop(bool	bloop)					{ m_bLoop = bloop; }
protected:

#ifdef		USE_RENDER
	MPRender*					m_pDev;
#else
	LPDIRECT3DDEVICE8	m_pDev;
#endif

	bool				m_bLoop;

	int					m_iMaxLen;

	MPCharacter*		_pCha;
	MPSceneItem*		_pItem;
	int					_iDummy[2];//DUMY

	D3DXCOLOR			_dwColor;
	float				_fLife;
	float				_fStep;

	S_BVECTOR<Strip_Vertex>	_vecPath;
	S_BVECTOR<track>	_vecCtrl;

#ifdef USE_RENDER
	lwITex*				_pTex;
#else
	LPDIRECT3DTEXTURE8	_pTex;
#endif
	s_string			_strTexName;

	CMPEffectFile*		_pCEffFile;

	float*				_pfDailTime;
	float				_fCurTime;
	bool				_bPlay;

	D3DBLEND			_eSrcBlend;
	D3DBLEND			_eDestBlend;

};

