#include "StdAfx.h"
//#include <mindpower.h>
#include "GlobalInc.h"
#include "MPModelEff.h"
#include "lwPredefinition.h"

#include "MPMap.h"

#include ".\mpshademap.h"
#include "MPRender.h"

#define	 TILESIZE	1.0f

//#include "..\include\mpshademap.h"

#define CLAMP(A, MIN, MAX)		( (A) > (MAX) ) ? (MAX) :  ( ( (A) < (MIN) ) ? (MIN) : (A) )

extern MINDPOWER_API CMPResManger        ResMgr;

CMPShadeMap::CMPShadeMap(void)
{
	m_iType = SHADE_SINGLE;
#ifndef USE_MGR
	_lpIB = NULL;
#endif

	//_fSin = sinf(3.141592654f / 2); 
	//_fCos = cosf(3.141592654f / 2);

	_SVerPos = D3DXVECTOR3(0.0f,0.0f,0);

	_dwColor = 0x80ffffff;
	_pfDailTime = NULL;
	_bShow = true;
		
	_iIdxTech = 2;
	_pCEffectFile = NULL;

	_eSrcBlend	= D3DBLEND_SRCALPHA;
	_eDestBlend = D3DBLEND_INVSRCALPHA;

	_bUpdate = true;

	_UpSea = false;

	_pModel = NULL;
}

CMPShadeMap::~CMPShadeMap(void)
{
#ifndef USE_MGR
	SAFE_RELEASE(_lpIB);
#endif

	SAFE_DELETE(_pModel);
}
void	CMPShadeMap::setTextureName(s_string& strName)
{
	if((strstr(strName.c_str(),".dds")==NULL)&&strstr(strName.c_str(),".tga")==NULL)
	{
		_strTexName = strName;
	}else
	{
		_strTexName = strName.substr(0, strName.length() - 4);
	}
}


void	CMPShadeMap::BoundingRes(CMPResManger	*m_CResMagr)
{
	int t_iID;
		
	t_iID = m_CResMagr->GetTextureID(_strTexName);
	if(t_iID == -1)
	{
		MessageBox(NULL,_strTexName.c_str() ,"shade",MB_OK);
		_lpCurTex = NULL;
	}
	else
	{
#ifdef USE_MGR
		//m_CResMagr->GetTextureByID(t_iID);
		_lpCurTex = m_CResMagr->GetTextureByIDlw(t_iID);
#else
		_lpCurTex = m_CResMagr->GetTextureByID(t_iID);
#endif

	}

	//_pModel = m_CResMagr->GetShadeMesh();

	//SAFE_DELETE(_pModel);
	if(!_pModel)
	{
		_pModel = new CEffectModel;
		_pModel->InitDevice(m_CResMagr->m_pDev,m_CResMagr->m_pSysGraphics->GetResourceMgr());
	}
	

	_pfDailTime = m_CResMagr->GetDailTime();
		
	_pCEffectFile = m_CResMagr->GetEffectFile();

	_pMatViewProj = m_CResMagr->GetViewProjMat();

	m_bUseSoft = m_CResMagr->m_bUseSoftOrg;

#if(defined LW_USE_DX8)
	DWORD	BOL = 0;
	_pModel->GetDev()->GetRenderState( D3DRS_SOFTWAREVERTEXPROCESSING , &BOL );
	if (BOL == 1)
	{
		m_bUseSoft = TRUE;
		//LG("debug","msgUsesoft");
	}
#endif

	//m_bUseSoft = TRUE;

	//if(m_bUseSoft)
	//	LG("debug","msguse soft vertex processing");
	//else
	//	LG("debug","msguse hard vertex processing");

	m_dwVsConst = m_CResMagr->GetDevCap()->MaxVertexShaderConst;
	//m_bUseSoft = TRUE;

	_UpSea = false;
}

bool	CMPShadeMap::CreateShadeMap(float fRadius)
{
	//if(fRadius > 7.0f)
	//	UseSoft(TRUE);

	_fRadius = fRadius;
	_fGridMax      = _fRadius / TILESIZE + 1;
	_iGridCrossNum = (int)_fGridMax;
	if(_fGridMax - (float)_iGridCrossNum > 0)
		_iGridCrossNum++;

	if(!SetGridNum(_iGridCrossNum))
		return false;

	//VB
	for( int n = 0; n < _iVerNum; n++)
	{
		_SShadePos[n] = D3DXVECTOR3(0,0,0);
		_SShadeUV[n]  = D3DXVECTOR2(0,0);
	}

	return true;
}

bool	CMPShadeMap::SetGridNum(int iNum)
{
	_iGridCrossNum = iNum;/////77777
	_iGridNum = _iGridCrossNum * _iGridCrossNum;

	_iFaceCount = _iGridNum * 2;
		
	_iVerNum		= (_iGridCrossNum + 1)*(_iGridCrossNum + 1);
	_iIndexNum			= _iGridCrossNum * _iGridCrossNum * 6;

	if(DWORD(_iVerNum * 2 + 8)>m_dwVsConst)
		m_bUseSoft = TRUE;

	//m_bUseSoft = TRUE;

	//HRESULT hr;
	//hr	= _pModel->m_pDev->CreateVertexBuffer(sizeof(SEFFECT_SHADE_VERTEX)*4, 
	//	D3DUSAGE_WRITEONLY |D3DUSAGE_DYNAMIC,
	//	EFFECT_SHADE_FVF, 
	//	D3DPOOL_DEFAULT, &_lpVB);
	//if( FAILED(hr) )
	//	return false;
	//SEFFECT_SHADE_VERTEX *pVertex;
	//_lpVB->Lock(0, 0, (BYTE**)&pVertex, D3DLOCK_NOOVERWRITE  );
	//pVertex[0].m_SPos			= D3DXVECTOR3(0, 0, 0);//*/D3DXVECTOR3(-0.5f, -0.5f, 0);
	//pVertex[0].m_dwDiffuse	= 0xffffffff;
	//pVertex[0].m_SUV			= D3DXVECTOR2(0.0f, 1.0f);
	//pVertex[0].m_SUV2			= D3DXVECTOR2(0, 1);

	//pVertex[1].m_SPos			= D3DXVECTOR3(0, 0, 0);//*/D3DXVECTOR3(-0.5f, 0.5f, 0);
	//pVertex[1].m_dwDiffuse	= 0xffffffff;
	//pVertex[1].m_SUV			= D3DXVECTOR2(0.0f, 0);
	//pVertex[1].m_SUV2			= D3DXVECTOR2(2, 3);

	//pVertex[2].m_SPos			= D3DXVECTOR3(0, 0, 0);//*/D3DXVECTOR3(0.5f, 0.5f, 0);
	//pVertex[2].m_dwDiffuse	= 0xffffffff;
	//pVertex[2].m_SUV			= D3DXVECTOR2(1.0f, 0.0f);
	//pVertex[2].m_SUV2			= D3DXVECTOR2(4, 5);

	//pVertex[3].m_SPos			= D3DXVECTOR3(0, 0, 0);//*/D3DXVECTOR3(0.5f, -0.5f, 0);
	//pVertex[3].m_dwDiffuse	= 0xffffffff;
	//pVertex[3].m_SUV			= D3DXVECTOR2(1.0f, 1.0f);
	//pVertex[3].m_SUV2			= D3DXVECTOR2(6, 7);

	//_lpVB->Unlock();
	//!IB

#ifndef USE_MGR
	SAFE_RELEASE(_lpIB);

	if(FAILED(_pModel->m_pDev->CreateIndexBuffer(sizeof(WORD) * _iIndexNum, 
		D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &_lpIB)))
		return false;

	//WORD  t_wIndex[6] = 
	//{
	//	0,1,2,0,2,3,
	//};
	WORD*  t_pwIndex;
	_lpIB->Lock(0, 0, (BYTE**)&t_pwIndex, 0); 
	int nIndex = 0;
	for( int nY = 0; nY < _iGridCrossNum; nY++ )
	{
		for( int nX = 0; nX < _iGridCrossNum; nX++ )
		{
			//t_pwIndex[nIndex++] = nX + nY * (_iGridCrossNum + 1);
			//t_pwIndex[nIndex++] = (nX+1) + nY * (_iGridCrossNum + 1);
			//t_pwIndex[nIndex++] = nX + (nY+1) * (_iGridCrossNum + 1);

			//t_pwIndex[nIndex++] = nX + nY * (_iGridCrossNum + 1);
			//t_pwIndex[nIndex++] = nX + (nY+1) * (_iGridCrossNum + 1);
			//t_pwIndex[nIndex++] = (nX+1) + (nY+1) * (_iGridCrossNum + 1);

			t_pwIndex[nIndex++] = nX + nY * (_iGridCrossNum + 1);
			t_pwIndex[nIndex++] = (nX+1) + nY * (_iGridCrossNum + 1);
			t_pwIndex[nIndex++] = nX + (nY+1) * (_iGridCrossNum + 1);

			t_pwIndex[nIndex++] = nX + (nY+1) * (_iGridCrossNum + 1);
			t_pwIndex[nIndex++] = (nX+1) + nY * (_iGridCrossNum + 1);
			t_pwIndex[nIndex++] = (nX+1) + (nY+1) * (_iGridCrossNum + 1);

			//t_pwIndex[nIndex++] = (nX+1) + nY * (_iGridCrossNum + 1);
			//t_pwIndex[nIndex++] = nX + (nY+1) * (_iGridCrossNum + 1);
			//t_pwIndex[nIndex++] = nX + nY * (_iGridCrossNum + 1);
			//t_pwIndex[nIndex++] = (nX+1) + (nY+1) * (_iGridCrossNum + 1);
			//t_pwIndex[nIndex++] = (nX+1) + nY * (_iGridCrossNum + 1);
			//t_pwIndex[nIndex++] = (nX+1) + (nY+1) * (_iGridCrossNum + 1);
			//t_pwIndex[nIndex++] = nX + (nY+1) * (_iGridCrossNum + 1);
		}
	}

	//memcpy(t_pwIndex, t_wIndex, sizeof(WORD)*6);
	_lpIB->Unlock();
#else

	_pModel->CreateShadeModel(_iVerNum,_iFaceCount,_iGridCrossNum,m_bUseSoft);

	/*int nIndex = 0;
	for( int nY = 0; nY < _iGridCrossNum; nY++ )
	{
		for( int nX = 0; nX < _iGridCrossNum; nX++ )
		{
			_wIndex[nIndex++] = nX + nY * (_iGridCrossNum + 1);
			_wIndex[nIndex++] = (nX+1) + nY * (_iGridCrossNum + 1);
			_wIndex[nIndex++] = nX + (nY+1) * (_iGridCrossNum + 1);

			_wIndex[nIndex++] = nX + (nY+1) * (_iGridCrossNum + 1);
			_wIndex[nIndex++] = (nX+1) + nY * (_iGridCrossNum + 1);
			_wIndex[nIndex++] = (nX+1) + (nY+1) * (_iGridCrossNum + 1);
		}
	}*/

	/*WORD* wIdx= NULL;
	_pModel->LockIB((BYTE**)&wIdx);
	memcpy(wIdx,_wIndex,sizeof(WORD)*_iIndexNum);
	_pModel->UnlockIB();*/

#endif

	return true;
}

void	CMPShadeMap::setFrameTexture(s_string& strTexName, CMPResManger	*pCResMagr)
{
	_strTexName = strTexName;
	int t_iID;
		
	t_iID = pCResMagr->GetTextureID(_strTexName);
	if(t_iID == -1)
	{
		MessageBox(NULL,_strTexName.c_str(),"",MB_OK);
		_lpCurTex = NULL;
	}
	else
	{
#ifdef USE_MGR
		//LPDIRECT3DTEXTURE8 tex=  pCResMagr->GetTextureByID(t_iID);
		_lpCurTex = pCResMagr->GetTextureByIDlw(t_iID);
#else
		_lpCurTex = pCResMagr->GetTextureByID(t_iID);
#endif
	}

	//assert(_lpCurTex);
}


void	CMPShadeMap::FrameMove(DWORD	dwDailTime)
{
	static	float dwTime = 1.5f;
	_bUpdate = false;
	dwTime += *_pfDailTime;
	if(dwTime > 1.5f)
	{
		dwTime = 0;
		_bUpdate = true;
	}

}

void	CMPShadeMap::MoveTo(D3DXVECTOR3 SVerPos, MPMap* pMap, float	fAngle)
{
	if(!_bShow)
		return;
	if(!_bUpdate)
	{
		//if(_SVerPos == SVerPos)
		//	return;
		if(fEquat(_SVerPos.x,SVerPos.x) &&fEquat(_SVerPos.y,SVerPos.y)/*&&fEquat(_SVerPos.z,SVerPos.z)*/)
		{
			return;
		}
	}
	_bUpdate = false;

	_SVerPos = SVerPos;


	//TILE
	int nX	= (int)((_SVerPos.x - _fRadius / 2 ) /*/ TILESIZE*/);
	int nY	= (int)((_SVerPos.y - _fRadius / 2 ) /*/ TILESIZE*/);

	//D3DXVECTOR2 vCenter;
	//vCenter.x = nX* TILESIZE + ((_fGridMax * TILESIZE) / 2);
	//vCenter.y = nY* TILESIZE + ((_fGridMax * TILESIZE) / 2);
	//TILE
	//float X = nX / TILESIZE + 0.5f;
	//float Y = nY / TILESIZE + 0.5f;
	
	//MPTile *TileList;
	for( int y = 0; y < _iGridCrossNum + 1; y++ )
	{
		for( int x = 0; x < _iGridCrossNum + 1; x++ )
		{
			int iIndex = x + y*(_iGridCrossNum + 1);
			//if(pMap)
			//	TileList = pMap->GetTile(int(nX) + x, int(nY) + y);

			float	fGridX = (float)(nX + x * TILESIZE)/* / TILESIZE*/;
			float	fGridY = (float)(nY + y * TILESIZE)/* / TILESIZE*/;
			_SShadePos[iIndex].x = fGridX;
			_SShadePos[iIndex].y = fGridY;
			if(pMap)
			{
				//int iUnitNo = int(fGridY) % 2 + int(fGridX ) % 2;
				//float height = pMap ? pMap->GetHeight(fGridX,fGridY) : 0.0f; // TileList->getHeight();
				float objHeight,height;
 				//float height;
	///*			for(int q =0; q < 4; q++)
	//			{
	//				objHeight = float(TileList->sObjHeight[q])/100;
	//				if(objHeight > height)
	//				{
	//					height = objHeight;
	//					height  += 0.001f;
	//				}
	//			}*/
	//			//_SShadePos[iIndex].z = height;
	//			
	//			objHeight = 0.0f; // float(TileList->sObjHeight[iUnitNo])/100;
	//			/*if(TileList->Block[iUnitNo])
	//			{
	//				objHeight += 1.0f;
	//			}*/

				//if(objHeight > height) 
				//{
				//	height = objHeight; 
				//}

	/*			if(!TileList ||TileList->IsDefault())
					height = 0.0f;
				else*/
				{
					int iGridx = (int)(fGridX * 2);
					int iGridy = (int)(fGridY* 2);
					if(y == _iGridCrossNum)
						iGridy -= 1;
					if(x == _iGridCrossNum)
						iGridx -= 1;
					height = pMap->GetGridHeight(iGridx, iGridy);
					//objHeight = pMap ? pMap->GetGridHeight((int)(fGridX * 2), (int)(fGridY* 2)) : 0.0f;
					objHeight = pMap->GetTileHeight((int)(fGridX), (int)(fGridY));

					if(_UpSea)
					{
						if(objHeight <= 0)
							objHeight = 0;
					}
					if(height > objHeight &&  !pMap->IsGridBlock(iGridx, iGridy))
						objHeight = height;
				}

				//if(objHeight > 0 && objHeight > height) 
				//{
				//	height = objHeight;
				//}
				_SShadePos[iIndex].z = objHeight  + 0.015f;
			}
			else
				_SShadePos[iIndex].z = SVerPos.z + 0.015f;

			float fU,fV;

			{
				fU = ( fGridX - _SVerPos.x ) / ( _fGridMax * TILESIZE);
				fV = ( fGridY - _SVerPos.y ) / ( _fGridMax * TILESIZE);
				_SShadeUV[iIndex].x	=  CLAMP(fU + 0.5f, 0.0f, 1.0f); //*/_fCos * fU - _fSin * fV   + 0.5f;
				_SShadeUV[iIndex].y	=  CLAMP(fV + 0.5f, 0.0f, 1.0f);//*/_fSin * fU + _fCos * fV   + 0.5f;

			}
			if(fAngle != 0)
			{
				D3DXMATRIX mat;

				D3DXVECTOR3	vpos(0.5f, 0.5f, 0);
				auto v = D3DXVECTOR3(0, 0, 1);
				GetMatrixRotation(&mat, &vpos, &v, fAngle);

				D3DXVec2TransformCoord(&_SShadeUV[iIndex], &_SShadeUV[iIndex], &mat);

				//_SShadeUV[iIndex] = vUV;

			}

		}
	}
	if(m_bUseSoft)
		FillVertex();
}

void CMPShadeMap::RenderVS()
{
	if (!_bShow)
		return;
	D3DXMATRIX t_mat;
	D3DXMatrixIdentity(&t_mat);
	if (_lpCurTex && _lpCurTex->IsLoadingOK())
	{
		// Here SetTexture is your method, inside which pD3DDevice->SetTexture(0, _lpCurTex->GetTex()) is probably called
		_pModel->m_pDev->SetTexture(0, _lpCurTex->GetTex());
	}
	else
	{
		_pCEffectFile->End();
		return;
	}

	_pModel->Begin();

	if (!_pCEffectFile->SetTechnique(_iIdxTech))
	{
		return;
	}

	_pCEffectFile->Begin(D3DXFX_DONOTSAVESHADERSTATE);//D3DXFX_DONOTSAVESTATE
	_pCEffectFile->Pass(0);
	//to fix overlay of models
	_pCEffectFile->m_pDev->SetRenderState(D3DRS_ZENABLE, FALSE);
	_pModel->m_pDev->SetRenderState(D3DRS_ZENABLE, TRUE);

	_pModel->m_pDev->SetRenderStateForced(D3DRS_SRCBLEND, _eSrcBlend);
	_pModel->m_pDev->SetRenderStateForced(D3DRS_DESTBLEND, _eDestBlend);

	_pModel->m_pDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	_pModel->m_pDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

	_pModel->m_pDev->SetVertexShader(ResMgr.GetShadeVS());
	_pModel->m_pDev->SetVertexDeclaration(ResMgr.GetShadeVDecl());

	_pModel->m_pDev->SetVertexShaderConstantF(0, t_mat, 4);
	_pModel->m_pDev->SetVertexShaderConstantF(4, *_pMatViewProj, 4);
	_pModel->m_pDev->SetVertexShaderConstantF(8, _dwColor, 1);

	D3DXVECTOR4 tv(0, 0, 0, 0);
	int nIndex = 9;
	for (int n = 0; n < _iVerNum; n++)
	{

		tv.x = _SShadeUV[n].x;
		tv.y = _SShadeUV[n].y;
		_pModel->m_pDev->SetVertexShaderConstantF(nIndex++, tv, 1);

		tv.x = _SShadePos[n].x;
		tv.y = _SShadePos[n].y;
		tv.z = _SShadePos[n].z;
		tv.w = 1;
		_pModel->m_pDev->SetVertexShaderConstantF(nIndex++, tv, 1);
	}

	_pModel->RenderModel();
	_pModel->End();

	_pCEffectFile->End();
}

void	CMPShadeMap::FillVertex()
{
	SEFFECT_SHADE_VERTEX *pVertex(0);
	if (!_pModel)
		return;

	_pModel->SetRenderNum( _iVerNum, _iFaceCount);

	_pModel->Lock((BYTE**)&pVertex);
	if(pVertex == 0)
	{
		MessageBox(NULL, "msgLockFailed lock error msglock error CMPShadeMap::FillVertex() line 552", "error", 0);
		//LG("error","msgLockFailed CMPShadeMap::FillVertex() %s\n ", _pModel->m_strName.c_str());
		return;
	}

	int nIndex = 9;//!VS9
	for( int n = 0; n < _iVerNum; n++)
	{
		pVertex[n].m_dwDiffuse = _dwColor;
		pVertex[n].m_SPos.x = _SShadePos[n].x;
		pVertex[n].m_SPos.y = _SShadePos[n].y;
		pVertex[n].m_SPos.z = _SShadePos[n].z;

		pVertex[n].m_SUV.x  = _SShadeUV[n].x;
		pVertex[n].m_SUV.y  = _SShadeUV[n].y;

		pVertex[n].m_SUV2.x = (float)nIndex;
		nIndex++;
		pVertex[n].m_SUV2.y = (float)nIndex;
		nIndex++;
	}
	_pModel->Unlock();
}

void CMPShadeMap::RenderSoft() {
	if (!_bShow)
		return;


	_pModel->Begin();
	//for fontand circle shadow onflict 
	_pCEffectFile->m_pDev->SetRenderState(D3DRS_TEXTUREFACTOR, _dwColor);
	_pCEffectFile->m_pDev->SetTextureStageStateForced(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
	_pCEffectFile->m_pDev->SetTextureStageStateForced(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
	_pCEffectFile->m_pDev->SetRenderStateForced(D3DRS_ALPHABLENDENABLE, TRUE);
	_pCEffectFile->m_pDev->SetRenderStateForced(D3DRS_ZWRITEENABLE, FALSE);
	_pCEffectFile->m_pDev->SetRenderStateForced(D3DRS_CULLMODE, D3DCULL_CCW);
	if (_iIdxTech != 4)
	{
		if (!_pCEffectFile->SetTechnique(_iIdxTech))
			return;
		_pCEffectFile->Begin(D3DXFX_DONOTSAVESTATE);
		_pCEffectFile->Pass(0);
	}

	_pModel->GetDev()->SetRenderStateForced(D3DRS_SRCBLEND, _eSrcBlend);
	_pModel->GetDev()->SetRenderStateForced(D3DRS_DESTBLEND, _eDestBlend);

	D3DXMATRIX t_mat;
	D3DXMatrixIdentity(&t_mat);

	if (_lpCurTex && _lpCurTex->IsLoadingOK())
		_pModel->m_pDev->SetTexture(0, _lpCurTex->GetTex());
	else {
		if (_iIdxTech != 4)
		{
			_pCEffectFile->End();
		}
		return;
	}
	//to fix models get front of each other when render wings/pets
	if (_iIdxTech != 4)
	{
		_pModel->GetDev()->SetTransformWorld(&t_mat);
	}

	_pModel->RenderModel();
	_pModel->End();
	_pCEffectFile->GetDev()->SetRenderState(D3DRS_TEXTUREFACTOR, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	_pCEffectFile->GetDev()->SetTextureStageStateForced(0, D3DTSS_COLORARG2, D3DTA_TEXTURE);
	_pCEffectFile->GetDev()->SetTextureStageStateForced(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	_pModel->m_pDev->SetVertexShader(nullptr);
	_pModel->m_pDev->SetFVF(EFFECT_SHADE_FVF);
	//this part not 100% sure of it yet need testing
	if (_iIdxTech != 4)
	{
		_pCEffectFile->End();
	}
}

void	CMPShadeMap::Render()
{
	if(!m_bUseSoft)
		RenderVS();
	else
		RenderSoft();
}

bool	CMPShadeMap::SaveToFile(FILE* pFile)
{
	char t_pszName[32];

	int t_temp;
	//!
	fwrite(&_fRadius,sizeof(float),1,pFile);

	lstrcpy(t_pszName,_strTexName.c_str());
	fwrite(t_pszName, sizeof(char),32,pFile);

	//!
	t_temp = (int)_eSrcBlend;
	fwrite(&t_temp,sizeof(int),1,pFile);
	t_temp = (int)_eDestBlend;
	fwrite(&t_temp,sizeof(int),1,pFile);


	fwrite(&_dwColor, sizeof(D3DXCOLOR),1,pFile);
	fwrite(&_iIdxTech, sizeof(int),1,pFile);

	return true;
}
bool	CMPShadeMap::LoadFromFile(FILE* pFile)
{
	//char t_pszName[32];

	int t_temp;
	////!
	//fread(&_fRadius,sizeof(float),1,pFile);

	//fread(t_pszName, sizeof(char),32,pFile);
	//_strTexName = t_pszName;

	//!
	fread(&t_temp,sizeof(int),1,pFile);
	_eSrcBlend = (D3DBLEND)t_temp;
	fread(&t_temp,sizeof(int),1,pFile);
	_eDestBlend = (D3DBLEND)t_temp;

	fread(&_dwColor, sizeof(D3DXCOLOR),1,pFile);
	fread(&_iIdxTech, sizeof(int),1,pFile);
	return true;
}

///////////////////////////////////////////////////////////
//class CMPShadeEX
///////////////////////////////////////////////////////////
CMPShadeEX::CMPShadeEX(int iFrameCount)
{
	m_iType = SHADE_ANI;

	_iFrameCount = iFrameCount;
	//_fLife = 0.1f * _iFrameCount;
	_iCurFrame =0;
	_fCurTime = 0;
	_vecFrameColor.resize(_iFrameCount);
	for(int n = 0; n < _iFrameCount; n++)
	{
		_vecFrameColor[n] = 0xffffffff;
	}
	_fFrameTime = 0.1f;
	_bLoop = false;
	_bShow = false;

	_iCurTex = 0;
	_vecTexOffset.clear();
	_iRow = 1;
	_iCol = 1;
	_fTexFrameTime = 0.1f;
	_fTexCurTime = 0;

	_iNumTex = 1;
	_vecTexOffset.resize(_iNumTex);
	for(int n = 0; n < _iNumTex; n++)
	{
		_vecTexOffset[n] = D3DXVECTOR2(0,0);
	}
	_vecTexSave.clear();
}

CMPShadeEX::~CMPShadeEX(void)
{
}

bool	CMPShadeEX::CreateSpliteTexture(int iRow, int iColnum)
{
	_iRow = iRow;
	_iCol = iColnum;

	_iNumTex = iRow * iColnum;

	float fsizew = 1.0f / iRow;
	float fsizeh = 1.0f / iColnum;

	_vecTexOffset.resize(_iNumTex);
	for(int h = 0;h < iColnum; h++)
	{
		for(int  w = 0; w < iRow; w++)
		{
			int idx = w + h * iRow;
			_vecTexOffset[idx].x = w * fsizew;
			_vecTexOffset[idx].y = h * fsizeh ;
		}
	}
	_vecTexSave.resize(_iVerNum);

	return true;
}

void	CMPShadeEX::setFrameCount(int iCount)
{
	_iFrameCount = iCount;
}

void	CMPShadeEX::setFrameTime(float fTime)
{
	_fFrameTime = fTime;
}
void	CMPShadeEX::setTexFrameTime(float fTime)
{
	_fTexFrameTime = fTime;
}

void	CMPShadeEX::setFrameColor(int iIdx, D3DCOLOR SColor)
{
	_vecFrameColor[iIdx] = SColor;
}
void	CMPShadeEX::setColor(D3DXCOLOR SColor)
{
	for(int n = 0; n < _iFrameCount; n++)
	{
		_vecFrameColor[n] = SColor;
	}
}


void	CMPShadeEX::FrameMove(DWORD	dwDailTime)
{
	if(!_bShow)
		return;

	int iNextFrame;
	_fCurTime += *_pfDailTime;
	if(_fCurTime >=_fFrameTime)
	{
		_iCurFrame++;
		if(_iCurFrame >= _iFrameCount)
		{
			if(_bLoop)
				_iCurFrame = 0;
			else
			{
				_bLoop = false;
				_bShow = false;
				return;
			}
		}
		_fCurTime = 0;
	}
	if(_iCurFrame == _iFrameCount - 1)
		iNextFrame = _iCurFrame;
	else
		iNextFrame = _iCurFrame + 1;
	_fLerp = _fCurTime / _fFrameTime;
	D3DXColorLerp( &_dwColor, &_vecFrameColor[_iCurFrame], &_vecFrameColor[iNextFrame], _fLerp );

	_fTexCurTime += *_pfDailTime;
	if(_fTexCurTime >= _fTexFrameTime)
	{
		_iCurTex++;
		if(_iCurTex == _iNumTex)
		{
			_iCurTex = 0;
		}
		_fTexCurTime = 0;
	}
}

void	CMPShadeEX::MoveTo(D3DXVECTOR3 SVerPos, MPMap* pMap, float	fAngle)
{
	if(!_bShow)
		return;
	if(_SVerPos == SVerPos)
	{
		if(_iNumTex > 1)
		{
			for(int n = 0; n < _iVerNum; n++)
			{
				_SShadeUV[n].x =	_vecTexSave[n].x / _iRow;
				_SShadeUV[n].y =	_vecTexSave[n].y / _iCol;
				_SShadeUV[n].x +=	_vecTexOffset[_iCurTex].x;
				_SShadeUV[n].y +=	_vecTexOffset[_iCurTex].y;
			}
		}
		return;
	}
	_SVerPos = SVerPos;
	//TILE
	int nX	= (int)((_SVerPos.x - _fRadius / 2 ) /*/ TILESIZE*/);
	int nY	= (int)((_SVerPos.y - _fRadius / 2 ) /*/ TILESIZE*/);
	
	//MPTile *TileList;
	for( int y = 0; y < _iGridCrossNum + 1; y++ )
	{
		for( int x = 0; x < _iGridCrossNum + 1; x++ )
		{
			int iIndex = x + y*(_iGridCrossNum + 1);
			//if(pMap)
			//	TileList = pMap->GetTile(nX + x, nY + y);

			float	fGridX = (float)(nX + x * TILESIZE) /*/ TILESIZE*/;
			float	fGridY = (float)(nY + y * TILESIZE) /*/ TILESIZE*/;
			_SShadePos[iIndex].x = fGridX;
			_SShadePos[iIndex].y = fGridY;
			if(pMap)
			{
				float height,objheight;
        //        if(!TileList->IsDefault())
        //        {
        //            int iUnitNo     = int(fGridY) % 2 + int(fGridX) % 2;
        //            height    = TileList->fHeight; // getHeight();
				    //float objHeight = TileList->getObjHeight(iUnitNo);
				    //if(objHeight > height) 
				    //{
					   // height = objHeight;  
				    //}
        //        }
				int iGridx = (int)(fGridX * 2);
				int iGridy = (int)(fGridY* 2);
				if(y == _iGridCrossNum)
					iGridy -= 1;
				if(x == _iGridCrossNum)
					iGridx -= 1;
				objheight = pMap->GetGridHeight(iGridx, iGridy);
				height = pMap->GetTileHeight((int)(fGridX), (int)(fGridY));

				if(_UpSea)
				{
					if(height <= 0)
						height = 0;
				}
				if( objheight > height )
					height = objheight;

				_SShadePos[iIndex].z = height  + 0.01f;
			}
			else
				_SShadePos[iIndex].z = SVerPos.z + 0.01f;

			float fU,fV;
			{
				fU = ( fGridX - _SVerPos.x ) / ( _fGridMax * TILESIZE);
				fV = ( fGridY - _SVerPos.y ) / ( _fGridMax * TILESIZE);
				_SShadeUV[iIndex].x	=  CLAMP(fU + 0.5f, 0.0f, 1.0f); 
				_SShadeUV[iIndex].y	=  CLAMP(fV + 0.5f, 0.0f, 1.0f);
			}
			if(fAngle != 0)
			{
				D3DXMATRIX mat;

				D3DXVECTOR3	vpos(0.5f, 0.5f, 0);
				const auto v = D3DXVECTOR3(0, 0, 1);
				GetMatrixRotation(&mat, &vpos, &v, fAngle);

				D3DXVec2TransformCoord(&_SShadeUV[iIndex], &_SShadeUV[iIndex], &mat);
			}
		}
	}
	if(_iNumTex > 1)
	{
		for(int n = 0; n < _iVerNum; n++)
		{
			_vecTexSave[n]		 = _SShadeUV[n];
			_SShadeUV[n].x		/=  _iRow;
			_SShadeUV[n].y		/=  _iCol;
			_SShadeUV[n]		+=  _vecTexOffset[_iCurTex];
		}
	}
}

bool	CMPShadeEX::SaveToFile(FILE* pFile)
{
	char t_pszName[32];

	int t_temp;
	//!
	fwrite(&_fRadius,sizeof(float),1,pFile);

	lstrcpy(t_pszName,_strTexName.c_str());
	fwrite(t_pszName, sizeof(char),32,pFile);

	fwrite(&_iRow,sizeof(int),1,pFile);
	fwrite(&_iCol,sizeof(int),1,pFile);

	//!
	t_temp = (int)_eSrcBlend;
	fwrite(&t_temp,sizeof(int),1,pFile);
	t_temp = (int)_eDestBlend;
	fwrite(&t_temp,sizeof(int),1,pFile);


	//!
	//fwrite(&_fLife,sizeof(float),1,pFile);
	//!
	fwrite(&_iFrameCount,sizeof(int),1,pFile);
	//!
	fwrite(&_fFrameTime,sizeof(float),1,pFile);
	//!
	for( int n = 0; n < _iFrameCount; n++)
	{
		fwrite(&_vecFrameColor[n],sizeof(D3DXCOLOR),1,pFile);
	}
	///////////////!
	//!
	fwrite(&_fTexFrameTime,sizeof(float),1,pFile);

	fwrite(&_iIdxTech, sizeof(int),1,pFile);

	return true;
}

bool	CMPShadeEX::LoadFromFile(FILE* pFile)
{
	//char t_pszName[32];

	int t_temp;
	////!
	//fread(&_fRadius,sizeof(float),1,pFile);

	//fread(t_pszName, sizeof(char),32,pFile);
	//_strTexName = t_pszName;

	//fread(&_iRow,sizeof(int),1,pFile);
	//fread(&_iCol,sizeof(int),1,pFile);

	//!
	fread(&t_temp,sizeof(int),1,pFile);
	_eSrcBlend = (D3DBLEND)t_temp;
	fread(&t_temp,sizeof(int),1,pFile);
	_eDestBlend = (D3DBLEND)t_temp;

	//!
	fread(&_iFrameCount,sizeof(int),1,pFile);
	//!
	fread(&_fFrameTime,sizeof(float),1,pFile);

	//!
	_vecFrameColor.resize(_iFrameCount);
	for( int n = 0; n < _iFrameCount; n++)
	{
		fread(&_vecFrameColor[n],sizeof(D3DXCOLOR),1,pFile);
	}
	///////////////!
	//!
	fread(&_fTexFrameTime,sizeof(float),1,pFile);

	fread(&_iIdxTech, sizeof(int),1,pFile);

	return true;
}

///////////////////////////////////////////////////////////
//class CMPShadeCtrl
///////////////////////////////////////////////////////////
CMPShadeCtrl::CMPShadeCtrl(void)
{
	_pShadeMap = NULL;
}

CMPShadeCtrl::~CMPShadeCtrl(void)
{
	SAFE_DELETE(_pShadeMap);
}

bool	CMPShadeCtrl::Create(s_string& strTexName,  CMPResManger	*pCResMagr, float fSize, 
							 bool bAni,int iRow, int iColnum)
{
	SAFE_DELETE(_pShadeMap);
	if(!bAni)
		_pShadeMap = new CMPShadeMap;
	else
		_pShadeMap = new CMPShadeEX;

	_pShadeMap->setTextureName(strTexName);
	_pShadeMap->BoundingRes(pCResMagr);

	_pShadeMap->CreateShadeMap(fSize);
	if(bAni)
		((CMPShadeEX*)_pShadeMap)->CreateSpliteTexture(iRow,iColnum);

	return true;
}
void	CMPShadeCtrl::SetAlphaType(D3DBLEND eSrcBlend, D3DBLEND eDestBlend)
{ 
	_pShadeMap->SetAlphaType(eSrcBlend,eDestBlend);
}
/////////////////////
void	CMPShadeCtrl::setFrameCount(int iCount)
{
	if(_pShadeMap->m_iType == SHADE_ANI)
		((CMPShadeEX*)_pShadeMap)->setFrameCount(iCount);
}

void	CMPShadeCtrl::setFrameTime(float fTime)
{
	if(_pShadeMap->m_iType == SHADE_ANI)
		((CMPShadeEX*)_pShadeMap)->setFrameTime(fTime);
}
void	CMPShadeCtrl::setTexFrameTime(float fTime)
{
	if(_pShadeMap->m_iType == SHADE_ANI)
		((CMPShadeEX*)_pShadeMap)->setTexFrameTime(fTime);
}

void	CMPShadeCtrl::setFrameColor(int iIdx, D3DCOLOR SColor)
{
	if(_pShadeMap->m_iType == SHADE_ANI)
		((CMPShadeEX*)_pShadeMap)->setFrameColor(iIdx,SColor);

}
void	CMPShadeCtrl::setColor(D3DXCOLOR SColor)
{
	if(_pShadeMap->m_iType == SHADE_SINGLE)
		_pShadeMap->SetColor(SColor);
	else
		((CMPShadeEX*)_pShadeMap)->SetColor(SColor);
}
	
int     CMPShadeCtrl::getFrameCount()
{
	if(_pShadeMap->m_iType == SHADE_ANI)
		return ((CMPShadeEX*)_pShadeMap)->getFrameCount();
	return 0;
}

float	CMPShadeCtrl::getFrameTime()
{
	if(_pShadeMap->m_iType == SHADE_ANI)
		return ((CMPShadeEX*)_pShadeMap)->getFrameTime();
	return 0;
}
float	CMPShadeCtrl::getTexFrameTime()
{
	if(_pShadeMap->m_iType == SHADE_ANI)
		return ((CMPShadeEX*)_pShadeMap)->getTexFrameTime();
	return 0;
}
void	CMPShadeCtrl::getFrameColor(int iIdx, D3DCOLOR* pSColor)
{
	if(_pShadeMap->m_iType == SHADE_ANI)
		return ((CMPShadeEX*)_pShadeMap)->getFrameColor(iIdx, pSColor);
}
void	CMPShadeCtrl::getColor(D3DXCOLOR* pSColor)
{
	//if(_pShadeMap->m_iType == SHADE_SINGLE)
		_pShadeMap->getColor(pSColor);
}
/////////////////////
void	CMPShadeCtrl::setFrameTexture(s_string& strTexName, CMPResManger	*pCResMagr)
{
	_pShadeMap->setFrameTexture(strTexName,pCResMagr);
}

void	CMPShadeCtrl::FrameMove(DWORD	dwDailTime)
{
	_pShadeMap->FrameMove(dwDailTime);
}

void	CMPShadeCtrl::MoveTo(D3DXVECTOR3 SVerPos, MPMap* pMap, float	fAngle)
{
	_pShadeMap->MoveTo(SVerPos,pMap,-fAngle);
}
void	CMPShadeCtrl::Render()
{
	_pShadeMap->Render();
}

bool	CMPShadeCtrl::SaveToFile(char* pchName)
{
	FILE* pFile;
	pFile = fopen(pchName, "wb");
	if(!pFile)
		return false;
	fwrite(&_pShadeMap->m_iType, sizeof(int),1,pFile);
	

	if(_pShadeMap->m_iType == SHADE_ANI)
		return ((CMPShadeEX*)_pShadeMap)->SaveToFile(pFile);
	else
		_pShadeMap->SaveToFile(pFile);

	fclose(pFile);
	return true;
}
bool	CMPShadeCtrl::LoadFromFile(char* pchName)
{
	return true;
}

