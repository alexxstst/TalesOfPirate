#include "StdAfx.h"
//#include "../../../proj/EffectEditer.h"
//#include <mindpower.h>

#include "GlobalInc.h"
#include "lwPredefinition.h"

#include "mpmodeleff.h"
#include "MPRender.h"
#include "lwEfxTrack.h"

void CEffectCortrol::FillModelUV(CEffectModel*	pCModel)
{
	for(WORD i = 0; i < pCModel->GetVerCount(); i++)
	{
		pCModel->GetDev()->SetVertexShaderConstantF(9 + i, *m_vecCurCoord[i], 1);
	}
}
void CEffectCortrol::FillTextureUV(CEffectModel*	pCModel)
{
	for(WORD i = 0; i < pCModel->GetVerCount(); i++)
	{
		pCModel->GetDev()->SetVertexShaderConstantF(9 + i, *m_lpCurTex[i], 1);
	}
}
void CEffectCortrol::FillDefaultUV(CEffectModel*	pCModel,TEXCOORD& coord)
{
	for(WORD i = 0; i < pCModel->GetVerCount(); i++)
	{
		pCModel->GetDev()->SetVertexShaderConstantF(9 + i, coord[i], 1);
	}
}

bool	CEffPath::LoadPathFromFile(char* pszName)
{
	FILE*     stream;
	char      header[4];
	DWORD     version,num;
	D3DXVECTOR3   tvec;
	float		ftemp;
	
	header[3] = NULL;
	stream = fopen( pszName,"rb" );
	if( !stream )
		return false;

	// confirm file header.
	fread( header,sizeof( char ),4,stream );
	if( strcmp( header,"csf" ) )
	{
		fclose( stream );
		return false;
	}

	fread( &version,sizeof( DWORD ),1,stream );
	fread( &num,sizeof( DWORD ),1,stream );

	m_iFrameCount = num;

	//m_vecPath = new D3DXVECTOR3[m_iFrameCount];
	//m_vecDist = new float[m_iFrameCount-1];
	//m_vecDir = new D3DXVECTOR3[m_iFrameCount-1];

	for(DWORD n = 0; n < num; n++)
	{
		fread( &tvec,sizeof( D3DXVECTOR3 ),1,stream );
		ftemp = tvec.y;
		tvec.y = -tvec.z;
		tvec.z = ftemp;
		//tvec.y = -tvec.y;

		m_vecPath[n] = tvec;
	}
	fclose( stream );

	for( int n=0;n < num -1;n++ )
	{
		m_vecDir[n] = m_vecPath[n+1] - m_vecPath[n];
		m_vecDist[n] = D3DXVec3Length( &m_vecDir[n] );
		//D3DXVec3Normalize(&m_vecDir[n],&m_vecDir[n]);
		D3DXVec3Normalize(&m_vecDir[n], &m_vecDir[n]);
	}

	return true;
}
bool CEffPath::LoadPathFromFileLet(const char* file)
{
    int i, j;
    lwMatrix44 mat;
    lwEfxTrack et;
    lwIAnimDataMatrix* data;
    if(LW_FAILED(et.Load(file)))
        return false;
    data = et.GetData();
    j = data->GetFrameNum();
    for(i = 0; i < j; i++)
    {
        data->GetValue(&mat, (float)i);
        m_vecPath[i].x = mat._41;
        m_vecPath[i].y = mat._42;
        m_vecPath[i].z = mat._43;
    }

	for(i = 0; i < j -1; i++)
	{
		m_vecDir[i] = m_vecPath[i+1] - m_vecPath[i];
		m_vecDist[i] = D3DXVec3Length( &m_vecDir[i] );
		D3DXVec3Normalize(&m_vecDir[i],&m_vecDir[i]);
	}

    m_iFrameCount = j;

    return true;
}
void	CEffPath::SavePath(FILE* pf)
{
	fwrite( &m_iFrameCount,sizeof( int ),1,pf );
	fwrite( &m_fVel,sizeof( float ),1,pf );

	for(int n = 0; n < m_iFrameCount; n++)
	{
		fwrite( &m_vecPath[n],sizeof( D3DXVECTOR3 ),1,pf );
	}
	for( int n=0;n < m_iFrameCount -1;n++ )
	{
		fwrite( &m_vecDir[n],sizeof( D3DXVECTOR3 ),1,pf );
		fwrite( &m_vecDist[n],sizeof( D3DXVECTOR3 ),1,pf );
	}
}
void	CEffPath::LoadPath(FILE* pf)
{
	fread( &m_iFrameCount,sizeof( int ),1,pf );
	fread( &m_fVel,sizeof( float ),1,pf );

	//m_vecPath = new D3DXVECTOR3[m_iFrameCount];
	//m_vecDist = new float[m_iFrameCount-1];
	//m_vecDir = new D3DXVECTOR3[m_iFrameCount-1];

	for(int n = 0; n < m_iFrameCount; n++)
	{
		fread( &m_vecPath[n],sizeof( D3DXVECTOR3 ),1,pf );
	}
	for(int  n=0;n < m_iFrameCount -1;n++ )
	{
		fread( &m_vecDir[n],sizeof( D3DXVECTOR3 ),1,pf );
		fread( &m_vecDist[n],sizeof( D3DXVECTOR3 ),1,pf );
	}
}

void	CEffPath::LoadPathFromMemory(CMemoryBuf*	pbuf)
{
	pbuf->mread( &m_iFrameCount,sizeof( int ),1 );
	pbuf->mread( &m_fVel,sizeof( float ),1 );

	for(int n = 0; n < m_iFrameCount; n++)
	{
		pbuf->mread( &m_vecPath[n],sizeof( D3DXVECTOR3 ),1 );
	}
	for(int  n=0;n < m_iFrameCount -1;n++ )
	{
		pbuf->mread( &m_vecDir[n],sizeof( D3DXVECTOR3 ),1 );
		pbuf->mread( &m_vecDist[n],sizeof( D3DXVECTOR3 ),1 );
	}
}

CMPModelEff::CMPModelEff(void)
{
	m_iEffNum = 0;
	ReleaseAll();
	//m_vecCortrol.resize(30);
}

CMPModelEff::~CMPModelEff(void)
{
	ReleaseAll();
}

//!保存效果到文件
bool	CMPModelEff::SaveToFile(char* pszFileName)	
{
	FILE* t_pFile;
	t_pFile = fopen(pszFileName, "wb");
	if(!t_pFile)
	{
		ToLogService("errors", LogLevel::Error, " {},只读文件，打开失败",pszFileName);
		return false;
	}
	//!版本
	DWORD t_dwVersion = 7;
	fwrite(&t_dwVersion,sizeof(t_dwVersion),1,t_pFile);

	//!效果状态索引
	int t_temp;
	t_temp = m_iIdxTech;
	fwrite(&t_temp,sizeof(int),1,t_pFile);

	char t_pszName[32];

	fwrite(&m_bUsePath, sizeof(bool),1,t_pFile);
	lstrcpy(t_pszName,m_strPathName.c_str());
	fwrite(t_pszName, sizeof(char),32,t_pFile);

	fwrite(&m_bUseSound, sizeof(bool),1,t_pFile);
	lstrcpy(t_pszName,m_strSoundName.c_str());
	fwrite(t_pszName, sizeof(char),32,t_pFile);

	fwrite(&m_bRotating, sizeof(bool),1,t_pFile);
	fwrite(&m_SVerRota, sizeof(D3DXVECTOR3),1,t_pFile);
	fwrite(&m_fRotaVel, sizeof(float),1,t_pFile);

	//!效果数量
	t_temp = m_iEffNum;
	fwrite(&t_temp,sizeof(int),1,t_pFile);


	for(int n = 0; n < m_iEffNum; n++)
	{
		m_vecEffect[n]->SaveToFile(t_pFile);
	}


	fclose(t_pFile);
	return true;
}
//!装入效果从文件
bool	CMPModelEff::LoadFromFile(char* pszFileName)
{
	return true;
}

/************************************************************************/
/*!	释放全部资源*/
/************************************************************************/
void CMPModelEff::ReleaseAll()
{
	int n;
	for( n = 0; n < m_iEffNum; n++)
	{
		m_vecEffect[n]->DeleteItem(m_pResMgr);
		delete m_vecEffect[n];
		m_vecEffect[n] = NULL;
	}
	m_bLoop				= true;
	m_bPlay				= false;

	m_pCEffect = NULL;
	m_iEffNum = 0;
	m_vecEffect.clear();
	m_vecCortrol.clear();
	m_pCurCortrol = NULL;

	m_iTimes = 0;
	m_pfDailTime = NULL;

	D3DXMatrixIdentity(&m_SpmatBone);
	D3DXMatrixIdentity(&m_SMatTempRota);
	//m_SpmatBone = NULL;

	D3DXMatrixScaling(&m_SmatScale,1.0f,1.0f,1.0f);
	D3DXMatrixRotationYawPitchRoll(&m_SmatRota, 0, 0, 0);
	D3DXMatrixTranslation(&m_SmatTrans,0, 0, 0);

	m_SVerScale = D3DXVECTOR3(1,1,1);
	m_SVerRota  =  D3DXVECTOR3(0,0,0);
	m_SVerTrans =  D3DXVECTOR3(0,0,0);
		
	m_iIdxTech = 0;
	m_pCEffectFile = NULL;
	m_pMatViewProj = NULL;

	m_bBindbone=  false;
		
	m_bUsePath = false;
	m_strPathName = "";
	m_pPath		 = NULL;
	m_bUseSound	 = false;
	m_strSoundName = "";

	m_bRotating = false;
	m_fRotaVel = 0.01f;
	m_fCurRotat = 0;

	m_bUseZ = true;
	m_fCurRotat = 0;
	m_pResMgr = NULL;
}
void	CMPModelEff::ClearEffect()
{
	int n;
	for( n = 0; n < m_iEffNum; n++)
	{
		m_vecEffect[n]->DeleteItem(m_pResMgr);
		delete m_vecEffect[n];
		m_vecEffect[n] = NULL;
	}
	m_vecEffect.clear();
	m_iEffNum = 0;
}

/************************************************************************/
/*!重设*/
/************************************************************************/
void CMPModelEff::Reset()
{
	for(int n = 0; n < m_iEffNum; n++)
	{
		m_vecCortrol[n]->Reset();
	}
	D3DXMatrixIdentity(&m_SpmatBone);
	D3DXMatrixIdentity(&m_SMatTempRota);

	D3DXMatrixScaling(&m_SmatScale,1.0f,1.0f,1.0f);
	D3DXMatrixRotationYawPitchRoll(&m_SmatRota, 0, 0, 0);
	D3DXMatrixTranslation(&m_SmatTrans,0, 0, 0);

	m_SVerPartRota = D3DXVECTOR3(0,0,0);
	m_bBindbone=  false;
	m_fCurRotat = 0;
}
///************************************************************************/
///*/！初始化*/
///************************************************************************/

/************************************************************************/
/*/！更新桢*/
/************************************************************************/

void CMPModelEff::FrameMoveAccel(float fDail)
{

}
void CMPModelEff::Begin()
{
	m_pCEffectFile->SetTechnique(m_iIdxTech);
	m_pCEffectFile->Begin(D3DXFX_DONOTSAVESTATE);
	m_pCEffectFile->Pass(0);
	m_vecEffect[0]->m_pDev->SetVertexShaderConstantF(4, *m_pMatViewProj, 4);

}
void CMPModelEff::End()  {
	//this been added when we fixed circle shadow not sure 100% of it but so far it works
	//@moth
	m_pCEffect->m_pDev->SetVertexShader(nullptr);
	m_pCEffect->m_pDev->SetFVF(EFFECT_VER_FVF);
	//edit end 
	m_pCEffectFile->End();
	m_pCEffect->m_pDev->SetRenderState(D3DRS_ZENABLE, TRUE);

	m_pCEffect->m_pDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pCEffect->m_pDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void CMPModelEff::RenderAccel(float &fTime)
{
	m_pCEffectFile->SetTechnique(m_iIdxTech);
	m_pCEffectFile->Begin(D3DXFX_DONOTSAVESTATE);
	m_pCEffectFile->Pass(0);

	D3DXMATRIX	t_STemp;

	GetTransMatrix(t_STemp);

	m_vecEffect[0]->m_pDev->SetVertexShaderConstantF(4, *m_pMatViewProj, 4);

	for(int n = 0; n < m_iEffNum; n++)
	{
		m_pCurCortrol = m_vecCortrol[n];
		if(!m_pCurCortrol->IsPlay())
			continue;

		m_pCEffect  = m_vecEffect[n];

		m_pCurCortrol->GetTransformMatrix(&m_SMatResult);

		D3DXMatrixMultiply(&m_SMatResult, &m_SMatResult, &t_STemp);		
		if(m_pCEffect->IsItem())
		{
			m_pCEffectFile->End();

			m_pCEffectFile->SetTechnique(3);
			m_pCEffectFile->Begin(D3DXFX_DONOTSAVESTATE);
			m_pCEffectFile->Pass(0);

			if(m_bBindbone)
			{
				D3DXMatrixMultiply(&m_pCurCortrol->m_SMatResult, &m_SMatResult, &m_SpmatBone);
			}
			m_pCEffect->m_pDev->SetRenderState(D3DRS_TEXTUREFACTOR, m_pCurCortrol->m_dwCurColor);
			m_pCEffect->m_pDev->SetTextureStageStateForced(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
			m_pCEffect->m_pDev->SetTextureStageStateForced(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
			m_pCEffect->m_pCModel->SetMatrix((lwMatrix44*)&m_pCurCortrol->m_SMatResult);
			m_pCEffect->m_pCModel->FrameMove(0);
			m_pCEffect->SetTexture();
			m_pCEffect->Render();
			m_pCEffect->m_pDev->SetTextureStageStateForced(0, D3DTSS_COLORARG2, D3DTA_TEXTURE);
			m_pCEffect->m_pDev->SetTextureStageStateForced(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
			continue;
		}
		if(m_pCEffect->IsChangeably())
		{
			m_pCEffectFile->End();
#ifdef USE_MGR
			m_pCEffect->Begin();
#else
			m_pCEffect->m_pDev->SetVertexShader(EFFECT_VER_FVF);
#endif

			m_pCEffectFile->SetTechnique(3);
			m_pCEffectFile->Begin(D3DXFX_DONOTSAVESTATE);
			m_pCEffectFile->Pass(0);

			m_pCEffect->SetTexture();

			if(m_bBindbone)
			{
				D3DXMatrixMultiply(&m_pCurCortrol->m_SMatResult, &m_SMatResult, &m_SpmatBone);
			}
			m_pCEffect->m_pDev->SetRenderState(D3DRS_TEXTUREFACTOR, m_pCurCortrol->m_dwCurColor);
			m_pCEffect->m_pDev->SetTextureStageStateForced(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
			m_pCEffect->m_pDev->SetTextureStageStateForced(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);


#ifdef USE_RENDER
			m_pCEffect->m_pDev->SetTransformWorld( &m_pCurCortrol->m_SMatResult);
#else
			m_pCEffect->m_pDev->SetTransform(D3DTS_WORLDMATRIX(0), &m_SMatResult);
#endif

			m_pCEffect->Render();
#ifdef USE_MGR
			m_pCEffect->End();
#endif
			m_pCEffect->m_pDev->SetTextureStageStateForced(0, D3DTSS_COLORARG2, D3DTA_TEXTURE);
			m_pCEffect->m_pDev->SetTextureStageStateForced(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
			continue;
		}
		m_pCEffect->SetTexture();
#ifdef USE_MGR
		m_pCEffect->Begin();
#endif
		m_pCEffect->SetVertexShader();

		if(m_pCEffect->IsBillBoard())
		{
			if(!m_bUseZ)
				m_pCEffect->m_pDev->SetRenderState(D3DRS_ZENABLE, FALSE);
			D3DXVECTOR3 vtpos(&m_SMatResult._41);
			D3DXMatrixMultiply(&m_SMatResult,&m_SMatResult,m_pCEffect->getBillBoardMatrix());

			if(m_bBindbone)
			{
				m_SMatResult._41 = m_SpmatBone._41;
				m_SMatResult._42 = m_SpmatBone._42;
				m_SMatResult._43 = m_SpmatBone._43;

			}
			else
			{
				m_SMatResult._41 = vtpos.x;
				m_SMatResult._42 = vtpos.y;
				m_SMatResult._43 = vtpos.z;
			}

		}else
		{
			if(m_bBindbone)
				D3DXMatrixMultiply(&m_SMatResult, &m_SMatResult, &m_SpmatBone);
		}
		Transpose(m_pCurCortrol->m_SMatResult,m_SMatResult);

		if(m_pCEffect->getType() != EFFECT_MODEL )
		{

			if(m_pCEffect->getType() == EFFECT_MODELUV)
			{
				m_pCurCortrol->FillModelUV(m_pCEffect->m_pCModel);
			}
			else
			{
				m_pCurCortrol->FillTextureUV(m_pCEffect->m_pCModel);
			}
		}
		m_pCEffect->m_pDev->SetVertexShaderConstantF(0, m_pCurCortrol->m_SMatResult, 4);
		m_pCEffect->m_pDev->SetVertexShaderConstantF(8, m_pCurCortrol->m_dwCurColor, 1);


		m_pCEffect->Render();
#ifdef USE_MGR
		m_pCEffect->End();
#endif
		m_pCEffect->m_pDev->SetRenderState(D3DRS_ZENABLE, TRUE);
	}
	m_pCEffectFile->End();
	m_pCEffect->m_pDev->SetRenderState(D3DRS_ZENABLE, TRUE);

	m_pCEffect->m_pDev->SetRenderState( D3DRS_SRCBLEND,D3DBLEND_SRCALPHA );
	m_pCEffect->m_pDev->SetRenderState( D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
}

void CMPModelEff::FrameMove(DWORD	dwDailTime)
{

	if (!m_bPlay)
		return;
	for(int n = 0; n < m_iEffNum; n++)
	{
		m_pCurCortrol = m_vecCortrol[n];
		if(!m_pCurCortrol->IsPlay())
			continue;
		m_pCEffect  = m_vecEffect[n];
		
		int   t_iNextFrame;

		m_pCurCortrol->m_fCurTime += *m_pfDailTime;
		//!得出当前播放的是哪一帧
		if(m_pCurCortrol->m_fCurTime >= m_pCEffect->getFrameTime(m_pCurCortrol->m_wCurFrame))
		{
			m_pCurCortrol->m_wCurFrame++;
			if(m_pCurCortrol->m_wCurFrame >= m_pCEffect->getFrameCount())
			{
				m_pCurCortrol->Stop();//重设到第一帧
				if(m_bLoop)//！循环播放
				{
					if(!IsPlay())//!如果所有的效果都停止，重新播放全部效果
					{
						Play2(0);
					}
					continue;
				}
				else//!按次数播放
				{
					if(!IsPlay())//!如果所有的效果都停止，停止
					{
						Stop();
						return;
					}
				}
			}

			m_pCurCortrol->m_fCurTime = 0.0f;
		}
		//!得到下一帧
		if( m_pCurCortrol->m_wCurFrame == (/*_wFrameCount*/m_pCEffect->getFrameCount() - 1) )
			t_iNextFrame = 0;
		else
			t_iNextFrame = m_pCurCortrol->m_wCurFrame+1;

		//!得到插值系数
		m_fLerp = m_pCurCortrol->m_fCurTime / m_pCEffect->getFrameTime(m_pCurCortrol->m_wCurFrame);

		m_pCEffect->m_ilast = m_pCurCortrol->m_wCurFrame;
		m_pCEffect->m_inext = t_iNextFrame;
		m_pCEffect->m_flerp = m_fLerp;

		//!得到当前大小
		m_pCEffect->GetLerpSize(&m_pCurCortrol->m_SCurSize,m_pCurCortrol->m_wCurFrame,t_iNextFrame,m_fLerp);

		if(!m_pCEffect->IsBillBoard())
		{
			//if(!m_pCEffect->IsRotaLoop())
				m_pCEffect->GetLerpAngle(&m_pCurCortrol->m_SCurAngle,m_pCurCortrol->m_wCurFrame,t_iNextFrame, m_fLerp);
		}
		//!得到当前位置
		m_pCEffect->GetLerpPos(&m_pCurCortrol->m_SCurPos,m_pCurCortrol->m_wCurFrame, t_iNextFrame, m_fLerp);
		//!得到当前颜色
		m_pCEffect->GetLerpColor(&m_pCurCortrol->m_dwCurColor,m_pCurCortrol->m_wCurFrame,t_iNextFrame,m_fLerp);

		if(!m_pCEffect->IsItem())
		{
			if(m_pCEffect->getType() == EFFECT_MODELUV)
			{
				//!得到当前的纹理坐标
				m_pCEffect->GetLerpCoord(m_pCurCortrol->m_vecCurCoord, m_pCurCortrol->m_wCurCoordIndex,m_pCurCortrol->m_fCurCoordTime,*m_pfDailTime);
			}else
			{
				if(m_pCEffect->getType() == EFFECT_MODELTEXTURE)
				{
					//!得到当前纹理
					m_pCEffect->GetLerpTexture(m_pCurCortrol->m_lpCurTex, m_pCurCortrol->m_wCurTexIndex,m_pCurCortrol->m_fCurTexTime,*m_pfDailTime);
				}else if(m_pCEffect->getType() == EFFECT_FRAMETEX)
				{
					//!得到当前纹理
					m_pCEffect->GetLerpFrame(m_pCurCortrol->m_wCurTexIndex,m_pCurCortrol->m_fCurTexTime,*m_pfDailTime);
				}
			}
		}else if(m_pCEffect->getType() == EFFECT_FRAMETEX)
		{
			//!得到当前纹理
			m_pCEffect->GetLerpFrame(m_pCurCortrol->m_wCurTexIndex,m_pCurCortrol->m_fCurTexTime,*m_pfDailTime);
		}
	}
	/*if(	m_bUsePath)
	{
		m_CPathCtrl.Update(*m_pfDailTime,m_pPath);
	}*/
}
/************************************************************************/
/*/！渲染*/
/************************************************************************/
void CMPModelEff::Render()
{
	if (!m_bPlay)
		return;

	if(!m_bUseSoft)
	{
		//Begin();
		//RenderAccel();
		//End();
		RenderVS();
	}
	else
		RenderSoft();
}
void CMPModelEff::RenderVS()
{

	D3DXMATRIX	t_STemp;

	GetTransMatrix(t_STemp);


	for(int n = 0; n < m_iEffNum; n++)
	{
		m_pCurCortrol = m_vecCortrol[n];
		if(!m_pCurCortrol->IsPlay())
			continue;

		m_pCEffect  = m_vecEffect[n];
//#ifdef	MULTITHREAD_LOADING_TEXTURE

		if(m_pCEffect->getType() == EFFECT_FRAMETEX)
		{
			if(!m_pCEffect->m_CTexFrame.m_lpCurTex || !m_pCEffect->m_CTexFrame.m_lpCurTex->IsLoadingOK())
				continue;
		}
		else
		{
			if(!m_pCEffect->m_CTextruelist.m_pTex || !m_pCEffect->m_CTextruelist.m_pTex->IsLoadingOK())
				continue;
		}
//#else
//		if(m_pCEffect->getType() == EFFECT_FRAMETEX)
//		{
//			if(!m_pCEffect->m_CTexFrame.m_lpCurTex)
//				continue;
//		}else if(!m_pCEffect->m_CTextruelist.m_pTex)
//					continue;
//
//#endif
		if(m_pCEffect->IsRotaLoop())
		{
			D3DXMATRIX sMat;
			m_pCEffect->GetRotaLoopMatrix(&sMat, m_pCurCortrol->m_fCurRotat, *m_pfDailTime);
			m_pCurCortrol->GetTransformMatrix(&m_SMatResult,&sMat);
		}
		else
			m_pCurCortrol->GetTransformMatrix(&m_SMatResult);

		D3DXMATRIX vertexShaderMat;
		D3DXMatrixMultiply(&m_SMatResult, &m_SMatResult, &t_STemp);		
		if(m_pCEffect->IsItem())
		{
			m_pCEffectFile->SetTechnique(3);
			m_pCEffectFile->Begin(D3DXFX_DONOTSAVESTATE);
			m_pCEffectFile->Pass(0);
			m_pCEffect->m_pDev->SetSamplerStateForced(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
			m_pCEffect->m_pDev->SetSamplerStateForced(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
			m_pCEffect->m_pDev->SetSamplerStateForced(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP );
			m_pCEffect->m_pDev->SetSamplerStateForced(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP );	
			//m_pCEffect->m_pDev->SetRenderStateForced(D3DRS_ZWRITEENABLE, TRUE);

			//m_pCEffect->m_pDev->SetRenderState(D3DRS_AMBIENT,0xffffffff);
			//m_pCEffect->m_pDev->SetRenderStateForced(D3DRS_CULLMODE,D3DCULL_CCW);

			if(!m_pCEffect->IsAlpah())
			{
				m_pCEffect->m_pDev->SetRenderStateForced(D3DRS_ALPHABLENDENABLE, FALSE);
				m_pCEffect->m_pDev->SetRenderStateForced(D3DRS_ZWRITEENABLE, TRUE);
				//m_pCEffect->m_pDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
				//m_pCEffect->m_pDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
				//m_pCEffect->m_pDev->SetRenderStateForced(D3DRS_ALPHATESTENABLE, TRUE);
				//m_pCEffect->m_pDev->SetRenderStateForced(D3DRS_ALPHAREF, 129);
				//m_pCEffect->m_pDev->SetRenderStateForced(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
			}
			else
			{
				m_pCEffect->m_pDev->SetRenderStateForced(D3DRS_ALPHABLENDENABLE, TRUE);
				m_pCEffect->m_pDev->SetRenderStateForced(D3DRS_ZWRITEENABLE, FALSE);
				m_pCEffect->m_pDev->SetRenderStateForced(D3DRS_CULLMODE, D3DCULL_CCW);
				//move this here
				m_pCEffect->m_pDev->SetTextureStageStateForced(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
			}

			m_pCEffect->m_pDev->SetRenderState(D3DRS_TEXTUREFACTOR, m_pCurCortrol->m_dwCurColor);
			m_pCEffect->m_pDev->SetTextureStageStateForced(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
			//if(m_pCEffect->GetTextureName() != "shadow")
			//	m_pCEffect->m_pDev->SetTextureStageStateForced(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
			if( m_pCEffect->m_pCModel )
			{
				if(m_bBindbone)
				{
					D3DXMatrixMultiply(&m_pCurCortrol->m_SMatResult, &m_SMatResult, &m_SpmatBone);

					m_pCEffect->m_pCModel->SetMatrix((lwMatrix44*)&m_pCurCortrol->m_SMatResult);
				}
				else
				{
					m_pCEffect->m_pCModel->SetMatrix((lwMatrix44*)&m_SMatResult);
				}

				m_pCEffect->m_pCModel->FrameMove(0);
				m_pCEffect->SetTexture();
				D3DXMatrixTranspose(&vertexShaderMat, m_pCEffect->m_pCModel->GetPrimitive()->GetRenderCtrlAgent()->GetGlobalMatrix());
			}
			else
			{
				//char szData[128];
				//sprintf( szData, "No model file. name = %s.", m_pCEffect->m_strModelName.c_str() );
				//MessageBox( NULL, szData, "error", MB_OK );
				D3DXMatrixIdentity(&vertexShaderMat);
			}
			m_pCEffect->SetVertexShader();
			m_pCEffect->m_pDev->SetVertexShaderConstantF(0, vertexShaderMat, 4);
			//for fix missing model
			m_pCEffect->m_pDev->SetVertexShaderConstantF(4, *m_pMatViewProj, 4);
			m_pCEffect->m_pDev->SetVertexShaderConstantF(8, m_pCurCortrol->m_dwCurColor, 1);
			
            // begin by lsh
			//m_pCEffect->m_pCModel->SetExternalTexture(0,m_pCEffect->m_CTextruelist.m_lpCurTex);
            // end			

			m_pCEffect->Render();

			m_pCEffectFile->End();
			m_pCEffect->m_pDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			m_pCEffect->m_pDev->SetTextureStageStateForced(0, D3DTSS_COLORARG2, D3DTA_TEXTURE);
			m_pCEffect->m_pDev->SetTextureStageStateForced(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
			continue;
		}
		if(m_pCEffect->IsChangeably())
		{
#ifdef USE_MGR
			m_pCEffect->Begin();
#else
			m_pCEffect->m_pDev->SetVertexShader(EFFECT_VER_FVF);
#endif

			m_pCEffectFile->SetTechnique(3);
			m_pCEffectFile->Begin(D3DXFX_DONOTSAVESTATE);
			m_pCEffectFile->Pass(0);
			m_pCEffect->m_pDev->SetSamplerStateForced(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
			m_pCEffect->m_pDev->SetSamplerStateForced(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);		
			//m_pCEffect->m_pDev->SetRenderStateForced(D3DRS_ZWRITEENABLE, TRUE);
			if(!m_pCEffect->IsAlpah())
			{
				m_pCEffect->m_pDev->SetRenderStateForced(D3DRS_ALPHABLENDENABLE, FALSE);
				m_pCEffect->m_pDev->SetRenderStateForced(D3DRS_ZWRITEENABLE, TRUE);
			}
			else
			{
				m_pCEffect->m_pDev->SetRenderStateForced(D3DRS_ZWRITEENABLE, FALSE);
				m_pCEffect->m_pDev->SetRenderStateForced(D3DRS_ALPHABLENDENABLE, TRUE);
			}
			m_pCEffect->SetVertexShader();
			m_pCEffect->SetTexture();

			if(m_bBindbone)
			{
				D3DXMatrixMultiply(&m_pCurCortrol->m_SMatResult, &m_SMatResult, &m_SpmatBone);
#ifdef USE_RENDER
				D3DXMatrixTranspose(&vertexShaderMat, &m_pCurCortrol->m_SMatResult);
				m_pCEffect->m_pDev->SetTransformWorld( &m_pCurCortrol->m_SMatResult);
#else
				m_pCEffect->m_pDev->SetTransform(D3DTS_WORLDMATRIX(0), &m_pCurCortrol->m_SMatResult);
#endif
			}else
			{
#ifdef USE_RENDER
				D3DXMatrixTranspose(&vertexShaderMat, &m_SMatResult);
				m_pCEffect->m_pDev->SetTransformWorld( &m_SMatResult);
#else
				m_pCEffect->m_pDev->SetTransform(D3DTS_WORLDMATRIX(0), &m_SMatResult);
#endif
			}
			m_pCEffect->m_pDev->SetRenderState(D3DRS_TEXTUREFACTOR, m_pCurCortrol->m_dwCurColor);
			m_pCEffect->m_pDev->SetTextureStageStateForced(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
			m_pCEffect->m_pDev->SetTextureStageStateForced(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
			m_pCEffect->m_pDev->SetVertexShaderConstantF(0, vertexShaderMat, 4);

			m_pCEffect->Render();
#ifdef USE_MGR
			m_pCEffect->End();
#endif
			m_pCEffectFile->End();
			m_pCEffect->m_pDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			m_pCEffect->m_pDev->SetTextureStageStateForced(0, D3DTSS_COLORARG2, D3DTA_TEXTURE);
			m_pCEffect->m_pDev->SetTextureStageStateForced(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
			continue;
		}
#ifdef USE_MGR
		m_pCEffect->Begin();
#endif
		m_pCEffectFile->SetTechnique(m_iIdxTech);
		m_pCEffectFile->Begin(D3DXFX_DONOTSAVESTATE);
		m_pCEffectFile->Pass(0);
		if(!m_pCEffect->IsAlpah())
		{
			m_pCEffect->m_pDev->SetRenderStateForced(D3DRS_ALPHABLENDENABLE, FALSE);
			m_pCEffect->m_pDev->SetRenderStateForced(D3DRS_ZWRITEENABLE, TRUE);
		}
		else
		{
			m_pCEffect->m_pDev->SetRenderStateForced(D3DRS_ZWRITEENABLE, FALSE);
			m_pCEffect->m_pDev->SetRenderStateForced(D3DRS_ALPHABLENDENABLE, TRUE);
		}
		m_pCEffect->SetVertexShader();
		m_pCEffect->SetTexture();

		if(m_pCEffect->IsBillBoard())
		{
			if(!m_bUseZ)
				m_pCEffect->m_pDev->SetRenderState(D3DRS_ZENABLE, FALSE);
			D3DXVECTOR3 vtpos(&m_SMatResult._41);
			if(!m_pCEffect->IsRotaBoard())
				D3DXMatrixIdentity(&m_SMatResult);
			D3DXMatrixMultiply(&m_SMatResult,&m_SMatResult,m_pCEffect->getBillBoardMatrix());

			if(m_bBindbone)
			{
				m_SMatResult._41 = m_SpmatBone._41;
				m_SMatResult._42 = m_SpmatBone._42;
				m_SMatResult._43 = m_SpmatBone._43;

			}
			else
			{
				m_SMatResult._41 = vtpos.x;
				m_SMatResult._42 = vtpos.y;
				m_SMatResult._43 = vtpos.z;
			}

		}else
		{
			if(m_bBindbone)
				D3DXMatrixMultiply(&m_SMatResult, &m_SMatResult, &m_SpmatBone);
		}

		//m_pCEffect->m_pDev->SetTextureStageStateForced(0,  D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP );
		//m_pCEffect->m_pDev->SetTextureStageStateForced(0,  D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP );		

		/*if(	m_bUsePath)
		{
			m_SMatResult._41 += m_CPathCtrl.m_SCurPath.x;
			m_SMatResult._42 += m_CPathCtrl.m_SCurPath.y;
			m_SMatResult._43 += m_CPathCtrl.m_SCurPath.z;
		}*/
		D3DXMatrixTranspose(&m_pCurCortrol->m_SMatResult, &m_SMatResult);
	
		//D3DXMATRIX tm;
		//Transpose(m_pCurCortrol->m_SMatResult,m_SMatResult);

			if(m_pCEffect->getType() != EFFECT_MODEL )
			{

				if(m_pCEffect->getType() == EFFECT_MODELUV)
				{
					m_pCurCortrol->FillModelUV(m_pCEffect->m_pCModel);
				}else if(m_pCEffect->getType() != EFFECT_FRAMETEX)
				{
					m_pCurCortrol->FillTextureUV(m_pCEffect->m_pCModel);
				}else
					m_pCurCortrol->FillDefaultUV(m_pCEffect->m_pCModel,m_pCEffect->m_CTexFrame.m_vecCoord);
			}/*else
				m_pCurCortrol->FillTextureUV(m_pCEffect->m_pCModel);*/

			m_pCEffect->m_pDev->SetVertexShaderConstantF(0, m_pCurCortrol->m_SMatResult, 4);
			m_pCEffect->m_pDev->SetVertexShaderConstantF(8, m_pCurCortrol->m_dwCurColor, 1);
			m_pCEffect->m_pDev->SetVertexShaderConstantF(4, *m_pMatViewProj, 4);

		m_pCEffect->Render();
#ifdef USE_MGR
		m_pCEffect->End();
#endif
		//m_pCEffect->m_pDev->SetRenderStateForced(D3DRS_ALPHABLENDENABLE, FALSE);
		m_pCEffectFile->End();
	}
	//m_pCEffectFile->End();
	//m_pCEffect->m_pDev->SetRenderState(D3DRS_ZENABLE, TRUE);

	//m_pCEffect->m_pDev->SetRenderState( D3DRS_SRCBLEND,D3DBLEND_SRCALPHA );
	//m_pCEffect->m_pDev->SetRenderState( D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
}
void CMPModelEff::RenderSoft()
{
	if (!m_bPlay)
		return;

	m_pCEffectFile->SetTechnique(m_iIdxTech);
	m_pCEffectFile->Begin(D3DXFX_DONOTSAVESTATE);
	m_pCEffectFile->Pass(0);
	if(!m_bUseZ)
	{
		m_pCEffect->m_pDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	}
	else
	{
		m_pCEffect->m_pDev->SetRenderState(D3DRS_ZENABLE, TRUE);
		m_pCEffect->m_pDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	}

	m_pCEffectFile->m_pDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);  
	m_pCEffectFile->m_pDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);  
	m_pCEffectFile->m_pDev->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_MODULATE);  
	m_pCEffectFile->m_pDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE); 
	m_pCEffectFile->m_pDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
	m_pCEffectFile->m_pDev->SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);

	D3DXMATRIX	t_STemp;

	if(m_bRotating)
	{
		m_fCurRotat += m_fRotaVel * *m_pfDailTime;
		if(m_fCurRotat >= 6.283185f)
			m_fCurRotat = m_fCurRotat - 6.283185f;
		D3DXMatrixRotationAxis(&m_SmatRota,
			&m_SVerRota, m_fCurRotat);
	}
	D3DXMatrixMultiply(&t_STemp, &m_SmatScale, &m_SmatRota);
	D3DXMatrixMultiply(&t_STemp, &t_STemp, &m_SmatTrans);

	//m_vecEffect[0]->m_pDev->SetVertexShaderConstant(4, m_pMatViewProj, 4);


	for(int n = 0; n < m_iEffNum; n++)
	{
		m_pCurCortrol = m_vecCortrol[n];
		if(!m_pCurCortrol->IsPlay())
			continue;

		m_pCEffect  = m_vecEffect[n];

//#ifdef	MULTITHREAD_LOADING_TEXTURE
		if(!m_pCEffect->m_CTextruelist.m_pTex->IsLoadingOK())
			return;
//#endif

		m_pCurCortrol->GetTransformMatrix(&m_SMatResult);
		D3DXMatrixMultiply(&m_SMatResult, &m_SMatResult, &t_STemp);		

		if(m_pCEffect->IsItem())
		{
			m_pCEffectFile->End();

			m_pCEffectFile->SetTechnique(3);
			m_pCEffectFile->Begin(D3DXFX_DONOTSAVESTATE);
			m_pCEffectFile->Pass(0);
			if(m_bBindbone)
			{
				D3DXMatrixMultiply(&m_SMatResult, &m_SMatResult, &m_SpmatBone);
			}
			m_pCEffect->m_pDev->SetRenderState(D3DRS_TEXTUREFACTOR, m_pCurCortrol->m_dwCurColor);
			m_pCEffect->m_pDev->SetTextureStageStateForced(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
			m_pCEffect->m_pDev->SetTextureStageStateForced(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
			m_pCEffect->m_pCModel->SetMatrix((lwMatrix44*)&m_SMatResult);
            // begin by lsh
			//m_pCEffect->m_pCModel->SetExternalTexture(0,m_pCEffect->m_CTextruelist.m_lpCurTex);
            // end

			m_pCEffect->m_pCModel->FrameMove(0);

			m_pCEffect->Render();
			m_pCEffect->m_pDev->SetTextureStageStateForced(0, D3DTSS_COLORARG2, D3DTA_TEXTURE);
			m_pCEffect->m_pDev->SetTextureStageStateForced(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
			continue;
		}
		if(m_pCEffect->IsChangeably())
		{
			m_pCEffectFile->End();
#ifdef USE_MGR
			m_pCEffect->Begin();
#else
			m_pCEffect->m_pDev->SetVertexShader(EFFECT_VER_FVF);
#endif

			m_pCEffectFile->SetTechnique(3);
			m_pCEffectFile->Begin(D3DXFX_DONOTSAVESTATE);
			m_pCEffectFile->Pass(0);

			m_pCEffect->SetTexture();

			if(m_bBindbone)
			{
				D3DXMatrixMultiply(&m_SMatResult, &m_SMatResult, &m_SpmatBone);
			}
			m_pCEffect->m_pDev->SetRenderState(D3DRS_TEXTUREFACTOR, m_pCurCortrol->m_dwCurColor);
			m_pCEffect->m_pDev->SetTextureStageStateForced(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
			m_pCEffect->m_pDev->SetTextureStageStateForced(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

#ifdef USE_RENDER
			m_pCEffect->m_pDev->SetTransformWorld(&m_SMatResult);
#else
			m_pCEffect->m_pDev->SetTransform(D3DTS_WORLDMATRIX(0), &m_SMatResult);
#endif

			m_pCEffect->Render();
#ifdef USE_MGR
			m_pCEffect->End();
#endif
			m_pCEffect->m_pDev->SetTextureStageStateForced(0, D3DTSS_COLORARG2, D3DTA_TEXTURE);
			m_pCEffect->m_pDev->SetTextureStageStateForced(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
			continue;
		}
		m_pCEffect->SetTexture();
#ifdef USE_MGR
		m_pCEffect->Begin();
#endif
		m_pCEffect->m_pDev->SetVertexShader(NULL);
		m_pCEffect->m_pDev->SetFVF(EFFECT_VER_FVF);


		if(m_pCEffect->IsBillBoard())
		{
			if(!m_bUseZ)
				m_pCEffect->m_pDev->SetRenderState(D3DRS_ZENABLE, FALSE);

			D3DXVECTOR3 vtpos(&m_SMatResult._41);
			D3DXMatrixMultiply(&m_SMatResult,&m_SMatResult,m_pCEffect->getBillBoardMatrix());

			if(m_bBindbone)
			{
				m_SMatResult._41 = m_SpmatBone._41;
				m_SMatResult._42 = m_SpmatBone._42;
				m_SMatResult._43 = m_SpmatBone._43;

			}
			else
			{
				m_SMatResult._41 = vtpos.x;
				m_SMatResult._42 = vtpos.y;
				m_SMatResult._43 = vtpos.z;
			}
		}else
			D3DXMatrixMultiply(&m_SMatResult, &m_SMatResult, &m_SpmatBone);

	/*	if(	m_bUsePath)
		{
			m_SMatResult._41 += m_CPathCtrl.m_SCurPath.x;
			m_SMatResult._42 += m_CPathCtrl.m_SCurPath.y;
			m_SMatResult._43 += m_CPathCtrl.m_SCurPath.z;
		}*/
		//D3DXMATRIX tm;
		//Transpose(tm,m_SMatResult);
			if(m_pCEffect->getType() != EFFECT_MODEL)
			{

				if(m_pCEffect->getType() == EFFECT_MODELUV)
				{
					m_pCurCortrol->FillModelUVSoft(m_pCEffect->m_pCModel);
				}
				else
				{
					m_pCurCortrol->FillTextureUVSoft(m_pCEffect->m_pCModel);
				}
			}
		//m_pCEffect->m_pDev->SetVertexShaderConstant(0, &tm, 4);
		//m_pCEffect->m_pDev->SetVertexShaderConstant(8, &m_pCurCortrol->m_dwCurColor, 1);
		m_pCEffect->m_pDev->SetRenderState(D3DRS_TEXTUREFACTOR, m_pCurCortrol->m_dwCurColor);
		m_pCEffect->m_pDev->SetTextureStageStateForced(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
		m_pCEffect->m_pDev->SetTextureStageStateForced(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
#ifdef USE_RENDER
		m_pCEffect->m_pDev->SetTransformWorld(&m_SMatResult);
#else
		m_pCEffect->m_pDev->SetTransform(D3DTS_WORLDMATRIX(0), &m_SMatResult);
#endif

		m_pCEffect->Render();
#ifdef USE_MGR
		m_pCEffect->End();
#endif
		m_pCEffect->m_pDev->SetTextureStageStateForced(0, D3DTSS_COLORARG2, D3DTA_TEXTURE);
		m_pCEffect->m_pDev->SetTextureStageStateForced(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	}
	m_pCEffectFile->End();
	m_pCEffect->m_pDev->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pCEffect->m_pDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	//m_pCEffect->m_pDev->SetRenderState(D3DRS_ZENABLE, TRUE);

	//m_pCEffect->m_pDev->SetRenderState( D3DRS_SRCBLEND,D3DBLEND_SRCALPHA );
	//m_pCEffect->m_pDev->SetRenderState( D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
}


void	CMPModelEff::ShowCurFrame(int iCurSubEff, int iCurFrame)
{
	//for(int n = 0; n < m_iEffNum; n++)
	{
		//m_pCurCortrol = &m_vecCortrol[iCurSubEff];
		m_pCEffect  = m_vecEffect[iCurSubEff];

	//!得到当前大小
	D3DXVECTOR3 t_sVerSize = m_pCEffect->getFrameSize(iCurFrame);

	D3DXVECTOR3 t_sVerAngle = m_pCEffect->getFrameAngle(iCurFrame);

	//!得到当前位置
	D3DXVECTOR3 t_sVerPos = m_pCEffect->getFramePos(iCurFrame);

	D3DXMATRIX t_SMat, t_SMatRot;
		D3DXMatrixScaling(&t_SMat,t_sVerSize.x,t_sVerSize.y,t_sVerSize.z);
		D3DXMatrixRotationYawPitchRoll(&t_SMatRot,
			t_sVerAngle.y,t_sVerAngle.x,t_sVerAngle.z);
		D3DXMatrixMultiply(&m_SMatResult, &t_SMat, &t_SMatRot);

		m_SMatResult._41 = t_sVerPos.x ;
		m_SMatResult._42 = t_sVerPos.y ;
		m_SMatResult._43 = t_sVerPos.z ;

		D3DXMatrixMultiply(&t_SMat, &m_SmatScale, &m_SmatRota);
		D3DXMatrixMultiply(&t_SMat, &t_SMat, &m_SmatTrans);
	D3DXMatrixMultiply(&m_SMatResult, &m_SMatResult, &t_SMat);


	//!得到当前颜色
	D3DXCOLOR   t_sColor = m_pCEffect->getFrameColor(iCurFrame);

	//!得到当前的纹理坐标
	//TEXCOORD	t_SCoord;
	//m_pCEffect->getFrameCoord(t_SCoord,0);
	//		
	//SEFFECT_VERTEX *pVertex;
	//	m_pCEffect->m_pCModel->GetVertexBuffer()->Lock(0, 0, (BYTE**)&pVertex, D3DLOCK_NOOVERWRITE );

	//	for(WORD i = 0; i < m_pCEffect->m_pCModel->GetVerCount(); ++i)
	//	{
	//		pVertex[i].m_SUV = t_SCoord[i];
	//		pVertex[i].m_dwDiffuse = 0xffffffff;
	//	}
	//	m_pCEffect->m_pCModel->GetVertexBuffer()->Unlock();
	//!得到当前纹理
		m_pCEffect->SetTexture();
#ifdef USE_RENDER
		m_pCEffect->m_pDev->SetTransformWorld(&m_SMatResult);
#else
		m_pCEffect->m_pDev->SetTransform(D3DTS_WORLDMATRIX(0), &m_SMatResult);
#endif

#ifdef USE_MGR
	m_pCEffect->Begin();
#endif

	m_pCEffect->m_pDev->SetVertexShader(NULL);
	m_pCEffect->m_pDev->SetFVF(EFFECT_VER_FVF);

	m_pCEffect->Render();
#ifdef USE_MGR
	m_pCEffect->End();
#endif

	}
}

void	CMPModelEff::ShowTempFrame(int iCurSubEff,
		D3DXVECTOR3& pScale, D3DXVECTOR3& pRotating, D3DXVECTOR3& pTranslate,
		D3DXCOLOR& pColor, TEXCOORD& vecCoord, IDirect3DTextureX* lpTex)
{
	m_pCEffect  = m_vecEffect[iCurSubEff];

	D3DXMATRIX t_SMat, t_SMatRot;
		D3DXMatrixScaling(&t_SMat,pScale.x,pScale.y,pScale.z);
		D3DXMatrixRotationYawPitchRoll(&t_SMatRot,
			pRotating.y,pRotating.x,pRotating.z);
		D3DXMatrixMultiply(&m_SMatResult, &t_SMat, &t_SMatRot);

		m_SMatResult._41 = pTranslate.x ;
		m_SMatResult._42 = pTranslate.y ;
		m_SMatResult._43 = pTranslate.z ;

		D3DXMatrixMultiply(&t_SMat, &m_SmatScale, &m_SmatRota);
		D3DXMatrixMultiply(&t_SMat, &t_SMat, &m_SmatTrans);
	D3DXMatrixMultiply(&m_SMatResult, &m_SMatResult, &t_SMat);

			
	//SEFFECT_VERTEX *pVertex;
	//	m_pCEffect->m_pCModel->GetVertexBuffer()->Lock(0, 0, (BYTE**)&pVertex, D3DLOCK_NOOVERWRITE );

	//	for(WORD i = 0; i < m_pCEffect->m_pCModel->GetVerCount(); ++i)
	//	{
	//		pVertex[i].m_SUV = vecCoord[i];
	//		pVertex[i].m_dwDiffuse = 0xffffffff;//(DWORD)pColor;
	//	}
	//	m_pCEffect->m_pCModel->GetVertexBuffer()->Unlock();
	//!得到当前纹理
	m_pCEffect->SetTexture();
#ifdef USE_RENDER
	m_pCEffect->m_pDev->SetTransformWorld(&m_SMatResult);
#else
	m_pCEffect->m_pDev->SetTransform(D3DTS_WORLDMATRIX(0), &m_SMatResult);
#endif

	m_pCEffect->m_pDev->SetVertexShader(NULL);
	m_pCEffect->m_pDev->SetFVF(EFFECT_VER_FVF);

#ifdef USE_MGR
	m_pCEffect->Begin();
#endif

	m_pCEffect->Render();
#ifdef USE_MGR
	m_pCEffect->End();
#endif
}


/************************************************************************/
/*                                                                      */
/************************************************************************/


CMPStrip::CMPStrip()
{
	_pCha = NULL;
	_pItem = NULL;
	_iDummy[0] = -1;
	_iDummy[1] = -1;

	m_iMaxLen = 256;

	_vecPath.resize(m_iMaxLen);
	_vecCtrl.resize(m_iMaxLen/2);
	_pTex = NULL;
	_pCEffFile = NULL;
	_strTexName = "无名";
	_pfDailTime=  NULL;
	_fCurTime = 0;
	_bPlay = false;

	_dwColor = D3DXCOLOR(1,1,1,1);
	_fLife = 1.0f;
	_fStep = 0.05f;
	_eSrcBlend		= D3DBLEND_SRCALPHA;
	_eDestBlend		= D3DBLEND_INVSRCALPHA;

	m_bLoop = false;
}
CMPStrip::~CMPStrip()
{
}

void	CMPStrip::Play()
{
	_vecPath.clear();
	_vecCtrl.clear();

	lwMatrix44 mat1,mat2;
	if (_pItem)
	{
		//_pItem->FrameMove();
		_pItem->GetObjDummyRunTimeMatrix(&mat1,_iDummy[0]);
		_pItem->GetObjDummyRunTimeMatrix(&mat2,_iDummy[1]);
	}else if(_pCha)
	{
		//_pCha->FrameMove();
		_pCha->GetObjDummyRunTimeMatrix(&mat1,_iDummy[0]);
		_pCha->GetObjDummyRunTimeMatrix(&mat2,_iDummy[1]);
	}else
		return;

	GetTrack(&mat1,&mat2);
	_fCurTime = 0;
	_bPlay = true;
}

void	CMPStrip::UpdateFrame()
{
	if(!_bPlay)
		return;
	lwMatrix44 mat1,mat2;
	if (_pItem)
	{
		_pItem->GetObjDummyRunTimeMatrix(&mat1,_iDummy[0]);
		_pItem->GetObjDummyRunTimeMatrix(&mat2,_iDummy[1]);
	}else if(_pCha)
	{
		_pCha->GetObjDummyRunTimeMatrix(&mat1,_iDummy[0]);
		_pCha->GetObjDummyRunTimeMatrix(&mat2,_iDummy[1]);
	}
	GetTrack(&mat1,&mat2);
}

void	CMPStrip::FrameMove()
{
	if(!_bPlay)
		return;
	_fCurTime +=*_pfDailTime;
	if(_fCurTime >_fStep)
	{
		lwMatrix44 mat1,mat2;
		if (_pItem)
		{
			_pItem->GetObjDummyRunTimeMatrix(&mat1,_iDummy[0]);
			_pItem->GetObjDummyRunTimeMatrix(&mat2,_iDummy[1]);
		}else if(_pCha)
		{
			_pCha->GetObjDummyRunTimeMatrix(&mat1,_iDummy[0]);
			_pCha->GetObjDummyRunTimeMatrix(&mat2,_iDummy[1]);
		}
		GetTrack(&mat1,&mat2);
		_fCurTime = 0;
	}
}
void	CMPStrip::Render()
{
	if(!_bPlay)
		return;
	_pCEffFile->SetTechnique(3);
	_pCEffFile->Begin(D3DXFX_DONOTSAVESTATE);
	_pCEffFile->Pass(0);
	m_pDev->SetRenderState( D3DRS_SRCBLEND,_eSrcBlend );
	m_pDev->SetRenderState( D3DRS_DESTBLEND,_eDestBlend);
	D3DXMATRIX mat;
	D3DXMatrixIdentity(&mat);

#ifdef		USE_RENDER
	m_pDev->SetVertexShader(NULL);
	m_pDev->SetFVF(STRIP_FVF);

#ifdef	MULTITHREAD_LOADING_TEXTURE
	if(_pTex && _pTex->IsLoadingOK())
		m_pDev->SetTexture(0, _pTex->GetTex());
	else
	{
		_pCEffFile->End();
		return;
	}
#else
	if(_pTex)
		m_pDev->SetTexture(0,_pTex->GetTex());
	else
		m_pDev->SetTexture(0,NULL);
#endif

	m_pDev->SetTransformWorld(&mat);
#else
	m_pDev->SetVertexShader(STRIP_FVF);
	m_pDev->SetTexture(0,_pTex);
	m_pDev->SetTransform(D3DTS_WORLDMATRIX(0), &mat);
#endif

	track* ptrack;
	D3DXCOLOR color = _dwColor;
	if(_vecCtrl.size()>1)
	{
		for (int n = 0; n< _vecCtrl.size(); ++n)
		{
			ptrack = _vecCtrl[n];

			ptrack->FrameMove(*_pfDailTime,color,_fLife);
			_vecPath[n*2]->m_dwDiffuse = color;
			_vecPath[n*2 +1]->m_dwDiffuse = color;
		}
	}
	if(_vecCtrl.size()>1)
#ifdef		USE_RENDER
		m_pDev->GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, _vecPath.size()-2, _vecPath.front(), sizeof(Strip_Vertex));
#else
		m_pDev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, _vecPath.size()-2, _vecPath.front(), sizeof(Strip_Vertex));
#endif
	_pCEffFile->End();
}

bool	CMPStrip::SaveToFile(FILE* t_pFile)
{
	fwrite(&m_iMaxLen,sizeof(int),1,t_pFile);
	fwrite(&_iDummy,sizeof(int),2,t_pFile);
	fwrite(&_dwColor,sizeof(D3DXCOLOR),1,t_pFile);
	fwrite(&_fLife,sizeof(float),1,t_pFile);
	fwrite(&_fStep,sizeof(float),1,t_pFile);
	char pszName[32];
	lstrcpy(pszName,_strTexName.c_str());
	fwrite(pszName,sizeof(char),32,t_pFile);
	int te = (int)_eSrcBlend;
	fwrite(&te,sizeof(int),1,t_pFile);
	te = (int)_eDestBlend;
	fwrite(&te,sizeof(int),1,t_pFile);
	return true;
}
bool	CMPStrip::LoadFromFile(FILE* t_pFile, DWORD dwVersion)
{
	fread(&m_iMaxLen,sizeof(int),1,t_pFile);
	fread(&_iDummy,sizeof(int),2,t_pFile);
	fread(&_dwColor,sizeof(D3DXCOLOR),1,t_pFile);
	fread(&_fLife,sizeof(float),1,t_pFile);
	fread(&_fStep,sizeof(float),1,t_pFile);
	char pszName[32];
	fread(pszName,sizeof(char),32,t_pFile);

	char psname[64];
	memset(psname,0,64);

	if((strstr(pszName,".dds")==NULL)&&strstr(pszName,".tga")==NULL)
	{
		_strTexName = pszName;
	}else
	{
		int len = lstrlen(pszName);
		memcpy(psname, pszName,len - 4); 
		_strTexName = psname;
	}
	int te ;
	fread(&te,sizeof(int),1,t_pFile);
	_eSrcBlend = (D3DBLEND)te;
	fread(&te,sizeof(int),1,t_pFile);
	_eDestBlend = (D3DBLEND)te;


	return true;
}

bool	CMPStrip::LoadFromMemory(CMemoryBuf* pbuf, DWORD dwVersion)
{
	pbuf->mread(&m_iMaxLen,sizeof(int),1);
	pbuf->mread(&_iDummy,sizeof(int),2);
	pbuf->mread(&_dwColor,sizeof(D3DXCOLOR),1);
	pbuf->mread(&_fLife,sizeof(float),1);
	pbuf->mread(&_fStep,sizeof(float),1);
	char pszName[32];
	pbuf->mread(pszName,sizeof(char),32);

	char psname[64];
	memset(psname,0,64);

	if((strstr(pszName,".dds")==NULL)&&strstr(pszName,".tga")==NULL)
	{
		_strTexName = pszName;
	}else
	{
		int len = lstrlen(pszName);
		memcpy(psname, pszName,len - 4); 
		_strTexName = psname;
	}
	int te ;
	pbuf->mread(&te,sizeof(int),1);
	_eSrcBlend = (D3DBLEND)te;
	pbuf->mread(&te,sizeof(int),1);
	_eDestBlend = (D3DBLEND)te;
	return true;
}


void	CMPStrip::CopyStrip(CMPStrip* pstrip)
{
	m_iMaxLen = pstrip->m_iMaxLen;
	_iDummy[0] = pstrip->_iDummy[0];
	_iDummy[1] = pstrip->_iDummy[1];
	_dwColor = pstrip->_dwColor;
	_fLife = pstrip->_fLife;
	_fStep = pstrip->_fStep;
	_strTexName = pstrip->_strTexName;
	_eSrcBlend =  pstrip->_eSrcBlend;
	_eDestBlend =  pstrip->_eDestBlend;
}

