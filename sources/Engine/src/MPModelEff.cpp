#include "StdAfx.h"

#include "GlobalInc.h"

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


	for(DWORD n = 0; n < num; n++)
	{
		fread( &tvec,sizeof( D3DXVECTOR3 ),1,stream );
		ftemp = tvec.y;
		tvec.y = -tvec.z;
		tvec.z = ftemp;

		m_vecPath[n] = tvec;
	}
	fclose( stream );

	for( int n=0;n < num -1;n++ )
	{
		m_vecDir[n] = m_vecPath[n+1] - m_vecPath[n];
		m_vecDist[n] = D3DXVec3Length( &m_vecDir[n] );
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
    if(LW_RESULT r = et.Load(file); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
                     "[{}] lwEfxTrack::Load failed: file='{}', ret={}",
                     __FUNCTION__, file ? file : "(null)", static_cast<long long>(r));
        return false;
    }
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

CMPModelEff::CMPModelEff(void)
{
	m_iEffNum = 0;
	ReleaseAll();
}

CMPModelEff::~CMPModelEff(void)
{
	ReleaseAll();
}

//!
bool	CMPModelEff::SaveToFile(char* pszFileName)	
{
	FILE* t_pFile;
	t_pFile = fopen(pszFileName, "wb");
	if(!t_pFile)
	{
		ToLogService("errors", LogLevel::Error, " {},",pszFileName);
		return false;
	}
	//!
	DWORD t_dwVersion = 7;
	fwrite(&t_dwVersion,sizeof(t_dwVersion),1,t_pFile);

	//!
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

	//!
	t_temp = m_iEffNum;
	fwrite(&t_temp,sizeof(int),1,t_pFile);


	for(int n = 0; n < m_iEffNum; n++)
	{
		m_vecEffect[n]->SaveToFile(t_pFile);
	}


	fclose(t_pFile);
	return true;
}
//!
bool	CMPModelEff::LoadFromFile(char* pszFileName)
{
	return true;
}

/************************************************************************/
/*!	*/
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
/*!*/
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
///*/*/
///************************************************************************/

/************************************************************************/
/*/*/
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
			m_pCEffect->Begin();

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


			m_pCEffect->m_pDev->SetTransformWorld( &m_pCurCortrol->m_SMatResult);

			m_pCEffect->Render();
			m_pCEffect->End();
			m_pCEffect->m_pDev->SetTextureStageStateForced(0, D3DTSS_COLORARG2, D3DTA_TEXTURE);
			m_pCEffect->m_pDev->SetTextureStageStateForced(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
			continue;
		}
		m_pCEffect->SetTexture();
		m_pCEffect->Begin();
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
		m_pCEffect->End();
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
		//!
		if(m_pCurCortrol->m_fCurTime >= m_pCEffect->getFrameTime(m_pCurCortrol->m_wCurFrame))
		{
			m_pCurCortrol->m_wCurFrame++;
			if(m_pCurCortrol->m_wCurFrame >= m_pCEffect->getFrameCount())
			{
				m_pCurCortrol->Stop();//
				if(m_bLoop)//
				{
					if(!IsPlay())//!
					{
						Play2(0);
					}
					continue;
				}
				else//!
				{
					if(!IsPlay())//!
					{
						Stop();
						return;
					}
				}
			}

			m_pCurCortrol->m_fCurTime = 0.0f;
		}
		//!
		if( m_pCurCortrol->m_wCurFrame == (/*_wFrameCount*/m_pCEffect->getFrameCount() - 1) )
			t_iNextFrame = 0;
		else
			t_iNextFrame = m_pCurCortrol->m_wCurFrame+1;

		//!
		m_fLerp = m_pCurCortrol->m_fCurTime / m_pCEffect->getFrameTime(m_pCurCortrol->m_wCurFrame);

		m_pCEffect->m_ilast = m_pCurCortrol->m_wCurFrame;
		m_pCEffect->m_inext = t_iNextFrame;
		m_pCEffect->m_flerp = m_fLerp;

		//!
		m_pCEffect->GetLerpSize(&m_pCurCortrol->m_SCurSize,m_pCurCortrol->m_wCurFrame,t_iNextFrame,m_fLerp);

		if(!m_pCEffect->IsBillBoard())
		{
				m_pCEffect->GetLerpAngle(&m_pCurCortrol->m_SCurAngle,m_pCurCortrol->m_wCurFrame,t_iNextFrame, m_fLerp);
		}
		//!
		m_pCEffect->GetLerpPos(&m_pCurCortrol->m_SCurPos,m_pCurCortrol->m_wCurFrame, t_iNextFrame, m_fLerp);
		//!
		m_pCEffect->GetLerpColor(&m_pCurCortrol->m_dwCurColor,m_pCurCortrol->m_wCurFrame,t_iNextFrame,m_fLerp);

		if(!m_pCEffect->IsItem())
		{
			if(m_pCEffect->getType() == EFFECT_MODELUV)
			{
				//!
				m_pCEffect->GetLerpCoord(m_pCurCortrol->m_vecCurCoord, m_pCurCortrol->m_wCurCoordIndex,m_pCurCortrol->m_fCurCoordTime,*m_pfDailTime);
			}else
			{
				if(m_pCEffect->getType() == EFFECT_MODELTEXTURE)
				{
					//!
					m_pCEffect->GetLerpTexture(m_pCurCortrol->m_lpCurTex, m_pCurCortrol->m_wCurTexIndex,m_pCurCortrol->m_fCurTexTime,*m_pfDailTime);
				}else if(m_pCEffect->getType() == EFFECT_FRAMETEX)
				{
					//!
					m_pCEffect->GetLerpFrame(m_pCurCortrol->m_wCurTexIndex,m_pCurCortrol->m_fCurTexTime,*m_pfDailTime);
				}
			}
		}else if(m_pCEffect->getType() == EFFECT_FRAMETEX)
		{
			//!
			m_pCEffect->GetLerpFrame(m_pCurCortrol->m_wCurTexIndex,m_pCurCortrol->m_fCurTexTime,*m_pfDailTime);
		}
	}
}
/************************************************************************/
/*/*/
/************************************************************************/
void CMPModelEff::Render()
{
	if (!m_bPlay)
		return;

	if(!m_bUseSoft)
	{
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


			if(!m_pCEffect->IsAlpah())
			{
				m_pCEffect->m_pDev->SetRenderStateForced(D3DRS_ALPHABLENDENABLE, FALSE);
				m_pCEffect->m_pDev->SetRenderStateForced(D3DRS_ZWRITEENABLE, TRUE);
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
				D3DXMatrixIdentity(&vertexShaderMat);
			}
			m_pCEffect->SetVertexShader();
			m_pCEffect->m_pDev->SetVertexShaderConstantF(0, vertexShaderMat, 4);
			m_pCEffect->m_pDev->SetVertexShaderConstantF(4, *m_pMatViewProj, 4);
			m_pCEffect->m_pDev->SetVertexShaderConstantF(8, m_pCurCortrol->m_dwCurColor, 1);
			
            // begin by lsh
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
			m_pCEffect->Begin();

			m_pCEffectFile->SetTechnique(3);
			m_pCEffectFile->Begin(D3DXFX_DONOTSAVESTATE);
			m_pCEffectFile->Pass(0);
			m_pCEffect->m_pDev->SetSamplerStateForced(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
			m_pCEffect->m_pDev->SetSamplerStateForced(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);		
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
				D3DXMatrixTranspose(&vertexShaderMat, &m_pCurCortrol->m_SMatResult);
				m_pCEffect->m_pDev->SetTransformWorld( &m_pCurCortrol->m_SMatResult);
			}else
			{
				D3DXMatrixTranspose(&vertexShaderMat, &m_SMatResult);
				m_pCEffect->m_pDev->SetTransformWorld( &m_SMatResult);
			}
			m_pCEffect->m_pDev->SetRenderState(D3DRS_TEXTUREFACTOR, m_pCurCortrol->m_dwCurColor);
			m_pCEffect->m_pDev->SetTextureStageStateForced(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
			m_pCEffect->m_pDev->SetTextureStageStateForced(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
			m_pCEffect->m_pDev->SetVertexShaderConstantF(0, vertexShaderMat, 4);

			m_pCEffect->Render();
			m_pCEffect->End();
			m_pCEffectFile->End();
			m_pCEffect->m_pDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			m_pCEffect->m_pDev->SetTextureStageStateForced(0, D3DTSS_COLORARG2, D3DTA_TEXTURE);
			m_pCEffect->m_pDev->SetTextureStageStateForced(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
			continue;
		}
		m_pCEffect->Begin();
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

		D3DXMatrixTranspose(&m_pCurCortrol->m_SMatResult, &m_SMatResult);

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
			}

			m_pCEffect->m_pDev->SetVertexShaderConstantF(0, m_pCurCortrol->m_SMatResult, 4);
			m_pCEffect->m_pDev->SetVertexShaderConstantF(8, m_pCurCortrol->m_dwCurColor, 1);
			m_pCEffect->m_pDev->SetVertexShaderConstantF(4, *m_pMatViewProj, 4);

		m_pCEffect->Render();
		m_pCEffect->End();
		m_pCEffectFile->End();
	}

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



	for(int n = 0; n < m_iEffNum; n++)
	{
		m_pCurCortrol = m_vecCortrol[n];
		if(!m_pCurCortrol->IsPlay())
			continue;

		m_pCEffect  = m_vecEffect[n];

		if(!m_pCEffect->m_CTextruelist.m_pTex->IsLoadingOK())
			return;

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
			m_pCEffect->Begin();

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

			m_pCEffect->m_pDev->SetTransformWorld(&m_SMatResult);

			m_pCEffect->Render();
			m_pCEffect->End();
			m_pCEffect->m_pDev->SetTextureStageStateForced(0, D3DTSS_COLORARG2, D3DTA_TEXTURE);
			m_pCEffect->m_pDev->SetTextureStageStateForced(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
			continue;
		}
		m_pCEffect->SetTexture();
		m_pCEffect->Begin();
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
		m_pCEffect->m_pDev->SetRenderState(D3DRS_TEXTUREFACTOR, m_pCurCortrol->m_dwCurColor);
		m_pCEffect->m_pDev->SetTextureStageStateForced(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
		m_pCEffect->m_pDev->SetTextureStageStateForced(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
		m_pCEffect->m_pDev->SetTransformWorld(&m_SMatResult);

		m_pCEffect->Render();
		m_pCEffect->End();
		m_pCEffect->m_pDev->SetTextureStageStateForced(0, D3DTSS_COLORARG2, D3DTA_TEXTURE);
		m_pCEffect->m_pDev->SetTextureStageStateForced(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	}
	m_pCEffectFile->End();
	m_pCEffect->m_pDev->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pCEffect->m_pDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

}


void	CMPModelEff::ShowCurFrame(int iCurSubEff, int iCurFrame)
{
	{
		m_pCEffect  = m_vecEffect[iCurSubEff];

	//!
	D3DXVECTOR3 t_sVerSize = m_pCEffect->getFrameSize(iCurFrame);

	D3DXVECTOR3 t_sVerAngle = m_pCEffect->getFrameAngle(iCurFrame);

	//!
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


	//!
	D3DXCOLOR   t_sColor = m_pCEffect->getFrameColor(iCurFrame);

	//!

	//!
		m_pCEffect->SetTexture();
		m_pCEffect->m_pDev->SetTransformWorld(&m_SMatResult);

	m_pCEffect->Begin();

	m_pCEffect->m_pDev->SetVertexShader(NULL);
	m_pCEffect->m_pDev->SetFVF(EFFECT_VER_FVF);

	m_pCEffect->Render();
	m_pCEffect->End();

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

			

	//!
	m_pCEffect->SetTexture();
	m_pCEffect->m_pDev->SetTransformWorld(&m_SMatResult);

	m_pCEffect->m_pDev->SetVertexShader(NULL);
	m_pCEffect->m_pDev->SetFVF(EFFECT_VER_FVF);

	m_pCEffect->Begin();

	m_pCEffect->Render();
	m_pCEffect->End();
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
	_strTexName = "";
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
		_pItem->GetObjDummyRunTimeMatrix(&mat1,_iDummy[0]);
		_pItem->GetObjDummyRunTimeMatrix(&mat2,_iDummy[1]);
	}else if(_pCha)
	{
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

	m_pDev->SetVertexShader(NULL);
	m_pDev->SetFVF(STRIP_FVF);

	if(_pTex && _pTex->IsLoadingOK())
		m_pDev->SetTexture(0, _pTex->GetTex());
	else
	{
		_pCEffFile->End();
		return;
	}

	m_pDev->SetTransformWorld(&mat);

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
		m_pDev->GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, _vecPath.size()-2, _vecPath.front(), sizeof(Strip_Vertex));
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

CEffectCortrol::CEffectCortrol()
{
	m_bPlay = false;

	m_fCurTime = 0.0f;
	m_wCurFrame = 0;
	m_dwCurColor = D3DCOLOR_ARGB(0, 255, 255, 255);

	m_SCurSize = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_SCurAngle = D3DXVECTOR3(0, 0, 0);
	m_SCurPos = D3DXVECTOR3(0, 0, 0);

	m_vecCurCoord.clear();
	m_wCurCoordIndex = 0;
	m_fCurCoordTime = 0;
	m_wCurTexIndex = 0;
	m_fCurTexTime = 0;
	m_lpCurTex.clear();

	m_iCurTimes = 0;

	m_vecCurCoord.resize(10);
	m_lpCurTex.resize(10);
}

void CEffectCortrol::Reset()
{
	m_bPlay = false;
	m_fCurTime = 0.0f;
	m_wCurFrame = 0;
	m_wCurCoordIndex = 0;
	m_fCurCoordTime = 0;
	m_wCurTexIndex = 0;
	m_fCurTexTime = 0;
}

void CEffectCortrol::Play()
{
	m_iCurTimes = 0;
	m_bPlay = true;
}

void CEffectCortrol::Stop()
{
	Reset();
	m_bPlay = false;
}

void CEffectCortrol::GetTransformMatrix(D3DXMATRIX* pSOut, D3DXMATRIX* pRota)
{
	D3DXMATRIX t_SMat, t_SMatRot;
	D3DXMatrixScaling(&t_SMat, m_SCurSize.x, m_SCurSize.y, m_SCurSize.z);
	if (!pRota)
	{
		D3DXMatrixRotationYawPitchRoll(&t_SMatRot,
			m_SCurAngle.y, m_SCurAngle.x, m_SCurAngle.z);
		D3DXMatrixMultiply(pSOut, &t_SMat, &t_SMatRot);
	}
	else
	{
		D3DXMATRIX t_mat;
		D3DXMatrixRotationYawPitchRoll(&t_SMatRot,
			m_SCurAngle.y, m_SCurAngle.x, m_SCurAngle.z);

		D3DXMatrixMultiply(&t_mat, &t_SMatRot, pRota);
		D3DXMatrixMultiply(pSOut, &t_SMat, &t_mat);
	}
	pSOut->_41 = m_SCurPos.x;
	pSOut->_42 = m_SCurPos.y;
	pSOut->_43 = m_SCurPos.z;
}

void CEffectCortrol::FillModelUVSoft(CEffectModel* pCModel)
{
	SEFFECT_VERTEX* pVertex;
	pCModel->Lock((BYTE**)&pVertex);
	for (WORD i = 0; i < pCModel->GetVerCount(); ++i)
	{
		pVertex[i].m_SUV = *m_vecCurCoord[i];
	}
	pCModel->Unlock();
}

void CEffectCortrol::FillTextureUVSoft(CEffectModel* pCModel)
{
	SEFFECT_VERTEX* pVertex;
	pCModel->Lock((BYTE**)&pVertex);
	for (WORD i = 0; i < pCModel->GetVerCount(); ++i)
	{
		pVertex[i].m_SUV = *m_lpCurTex[i];
	}
	pCModel->Unlock();
}

void CEffPath::Copy(CEffPath* pPath)
{
	m_iFrameCount = pPath->m_iFrameCount;
	memcpy(m_vecPath, pPath->m_vecPath, sizeof(D3DXVECTOR3) * m_iFrameCount);
	memcpy(m_vecDist, pPath->m_vecDist, sizeof(float) * (m_iFrameCount - 1));
	memcpy(m_vecDir, pPath->m_vecDir, sizeof(D3DXVECTOR3) * (m_iFrameCount - 1));
	m_fVel = pPath->m_fVel;
	Reset();
}

void CEffPath::FrameMove(float fDailTime)
{
	if (m_iFrameCount <= 0)
	{
		return;
	}
	m_bEnd = false;

	float fvel = m_fVel * fDailTime;
	m_fCurDist += fvel;
	while (m_fCurDist >= m_vecDist[m_iCurFrame])
	{
		m_fCurDist -= m_vecDist[m_iCurFrame];
		m_iCurFrame++;
		if (m_iCurFrame >= m_iFrameCount - 1)
		{
			m_iCurFrame = 0;
			m_bEnd = true;
		}
	}
	m_vCurPos = m_vecPath[m_iCurFrame] + (m_vecDir[m_iCurFrame] * m_fCurDist);
}

D3DXVECTOR3* CEffPath::GetNextPos()
{
	if (m_iCurFrame >= m_iFrameCount - 1)
	{
		return &m_vCurPos;
	}
	return &m_vecPath[m_iCurFrame - 1];
}

bool CMPModelEff::IsPlay()
{
	for (int n = 0; n < m_iEffNum; n++)
	{
		if (m_vecCortrol[n]->IsPlay())
		{
			return true;
		}
	}
	return false;
}

void CMPModelEff::Play(int iTime)
{
	m_bPlay = true;
	if (iTime > 0)
	{
		m_bLoop = false;
		m_iTimes = iTime;
	}
	else
	{
		m_bLoop = true;
		m_iTimes = 0;
	}
	for (int n = 0; n < m_iEffNum; n++)
	{
		m_vecCortrol[n]->Play();
		m_vecEffect[n]->PlayModel();
	}
}

void CMPModelEff::Play2(int iTime)
{
	m_bPlay = true;
	if (iTime > 0)
	{
		m_bLoop = false;
		m_iTimes = iTime;
	}
	else
	{
		m_bLoop = true;
		m_iTimes = 0;
	}
	for (int n = 0; n < m_iEffNum; n++)
	{
		m_vecCortrol[n]->Play();
	}
}

void CMPModelEff::Stop()
{
	Reset();
	m_bPlay = false;
	m_iTimes = 0;
}

void CMPModelEff::FreeEffect()
{
	m_vecEffect.clear();
	m_iEffNum = 0;
}

void CMPModelEff::BindingEffect(I_Effect* pCEffect)
{
	m_pCEffect = pCEffect;

	m_iEffNum++;

	m_vecEffect.resize(m_iEffNum);
	m_vecEffect[m_iEffNum - 1] = pCEffect;

	m_vecCortrol.setsize(m_iEffNum);
	m_vecCortrol[m_iEffNum - 1]->m_vecCurCoord.setsize((WORD)pCEffect->m_pCModel->GetVerCount());
	m_vecCortrol[m_iEffNum - 1]->m_lpCurTex.setsize((WORD)pCEffect->m_pCModel->GetVerCount());
}

void CMPModelEff::BindingEffect(std::vector<I_Effect>& CEffectArray)
{
	ClearEffect();
	int n;
	m_bPlay = false;

	m_iEffNum = (int)CEffectArray.size();
	m_vecEffect.clear();

	m_vecEffect.resize(m_iEffNum);
	for (n = 0; n < m_iEffNum; n++)
	{
		m_vecEffect[n] = new I_Effect;
		m_vecEffect[n]->CopyEffect(&CEffectArray[n]);
	}

	m_vecCortrol.resize(m_iEffNum);
	m_vecCortrol.setsize(m_iEffNum);
	for (n = 0; n < m_iEffNum; n++)
	{
		m_vecCortrol[n]->m_vecCurCoord.setsize(CEffectArray[n].m_CTexCoordlist.m_wVerCount);
		m_vecCortrol[n]->m_lpCurTex.setsize(CEffectArray[n].m_CTexCoordlist.m_wVerCount);
	}
}

void CMPModelEff::BindingRes(CMPResManger* pResMagr)
{
	m_bPlay = false;

	m_pResMgr = pResMagr;
	int n;
	for (n = 0; n < m_iEffNum; n++)
	{
		m_vecEffect[n]->BoundingRes(pResMagr);
	}

	m_pfDailTime = pResMagr->GetDailTime();
	m_pCEffectFile = pResMagr->GetEffectFile();
	int idx = pResMagr->GetEffectID(m_vecEffect[0]->getEffectName());

	if (idx == -1)
	{
		char szData[128];
		sprintf(szData, "(ID%d)", idx);
		MessageBox(NULL, szData, "Error", MB_OK);
	}

	EffParameter* pParam = pResMagr->GetEffectParamByID(idx);
	m_iIdxTech = pParam->m_iIdxTech;

	m_bUsePath = pParam->m_bUsePath;
	m_bUseSound = pParam->m_bUseSound;
	m_strPathName = pParam->m_szPathName;
	m_strSoundName = pParam->m_szSoundName;

	m_bRotating = pParam->m_bRotating;
	m_fRotaVel = pParam->m_fRotaVel;
	m_SVerRota = pParam->m_SVerRota;

	if (m_bUsePath)
	{
		m_pPath = pResMagr->GetEffPath(pResMagr->GetEffPathID(m_strPathName));
	}

	m_pMatViewProj = pResMagr->GetViewProjMat();
	m_bUseSoft = pResMagr->m_bUseSoft;

	D3DXMatrixIdentity(&m_SMatTempRota);
}
