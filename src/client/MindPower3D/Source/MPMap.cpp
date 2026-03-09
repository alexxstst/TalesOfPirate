// Map & MapSection 
// Created By Ryan
// 2004/01/31

#include "Stdafx.h"
#include "MPTile.h"
#include "MPMap.h"
#include "d3dutil.h"
#include "MPGameApp.h"
#include "MPTextureSet.h"
#include "MPTerrainSet.h"
#include "MPMath.h"
#include "MPCamera.h" 
#include "assert.h" 
#include "MPRender.h"


using namespace std;


#define FVF_SEA				D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1
#define FVF_LAND			D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX2

#define MAX_RENDER_TILE		160000
#define SEA_TILE_SIZE       4
#define SKYDOOM_TILE_SIZE   6

#define FVF_SKYDOOM          D3DFVF_XYZ|D3DFVF_TEX1 

 
MPMap::MPMap()
:_bEdit(FALSE),
 _bSeaVisible(TRUE),
 _bShowCenterPoint(FALSE),
 _fp(NULL),
 _bUseVB(FALSE),
 _bClip(TRUE),
 _bWireFrame(FALSE),
 _pfnProc(NULL),
 _bBatchRender(TRUE),
 m_dwMaxLoadingTime(0),
 _bEnableNormalLight(FALSE),
 _dwLoadingCnt(0),
 _nUpdateRender(2),
 _pLandVB(NULL),
 _bUseLandVB(TRUE),
 _nLastTileStartX(0),
 _nLastTileStartY(0),
 _nLastSectionX(0),
 _nLastSectionY(0),
 _bRenderSea(FALSE),
 m_bCullTile(FALSE),
  _pOffsetIdx(NULL),
  m_dwMapDataSize(0),
  m_dwMapPos(0),
  m_txMoveSpeed(0),
  m_bSkyDoom(TRUE),
  m_pSkyDoomVB(NULL)
{
	_fShowCenterX = 0;
	_fShowCenterY = 0;

	_pDefaultTile = new MPTile;
	
	_pDefaultTile->Init();
	
	_pDefaultTile->TexLayer[0].btTexNo	 = UNDERWATER_TEXNO;
	_pDefaultTile->TexLayer[0].btAlphaNo = 15;
	_pDefaultTile->TexLayer[1].btTexNo   = 255; // DefaultTile
	_pDefaultTile->fHeight				 = UNDERWATER_HEIGHT;
	_pDefaultTile->dwColor				 = 0xffffffff;
    // _pDefaultTile->TexLayer

	memset(&_ActiveSectionArray, 0, 512 * 512 * sizeof(MPActiveMapSection*));

	_nWaterLoopFrame = 0;

	SetShowSize(36, 30);		// Increasing to 120,120 Mdr.st May 2020 - FPO alpha
	
	SetSectionBufferSize(128);

#if(defined DRAW_SEA_USE_DYNAMIC_BUFFER)
    _pVertBuf = new MPSeaTileVertex[1024];
#endif
    
    _dwSeaDefaultColor = D3DCOLOR_ARGB(0xcf, 140, 140, 220);

	m_SkyDoomVertexShaderHandle = 0;
	m_SkyDoomPixelShaderHandle = 0;

	m_iRange = 512;
}

MPMap::~MPMap()
{
	ClearAllSection(TRUE);
	delete _pDefaultTile;
	SAFE_DELETE_ARRAY(_pOffsetIdx);
    if(_fp) fclose(_fp);
#if(defined DRAW_SEA_USE_DYNAMIC_BUFFER)
    delete[] _pVertBuf;
#endif
    SAFE_RELEASE(_pLandVB);
}

void MPMap::FrameMove(DWORD dwTimeParam)
{
	_nWaterLoopFrame++;
	if(_nWaterLoopFrame >= MAX_WATER_LOOP) _nWaterLoopFrame = 0;
}

float AlphaNo2UV[16][2] = 
{
    0.0f,  0.0f,
    
    0.0f,  0.0f,   // 1
    0.25f, 0.0f,
    0.5f,  0.0f,
    0.75f, 0.0f,
    
    0.0f,  0.25f,
    0.25f, 0.25f,
    0.5f,  0.25f,
    0.75f, 0.25f,

    0.0f,  0.5f,
    0.25f, 0.5f,
    0.5f,  0.5f,
    0.75f, 0.5f,

    0.0f,  0.75f,
    0.25f, 0.75f, // 14
    0.5f,  0.75f
};


void MPMap::Render()
{
	if(_fp==NULL) return; 

    MPTimer t;
    t.Begin();

    g_Render.EnableMipmap(FALSE);
	
    if(_bUseLandVB)
    {
       if(_pLandVB==NULL)
        {
            int nVertexCnt = MAX_RENDER_TILE * 6; // 
	        _dwLandVBSize = nVertexCnt * sizeof(MPTileVertex);
            HRESULT hr = g_Render.GetDevice()->CreateVertexBuffer(_dwLandVBSize, 
		        0, FVF_LAND, D3DPOOL_MANAGED, &_pLandVB, NULL);

	        if(FAILED(hr)) 
	        {
                ToLogService("error", "msg MPMap::Render() Land Terrain Vertex Buffer!");
                _pLandVB = NULL;
            }
        }
    }

    if(_bUseVB)
    {
#if(defined DRAW_SEA_USE_DYNAMIC_BUFFER)
#else
        if(_pVB==NULL)
        {
            int nVertexCnt = (_nShowWidth / SEA_TILE_SIZE + 1)* (_nShowHeight / SEA_TILE_SIZE + 1) * 6;
	        HRESULT hr = g_Render.GetDevice()->CreateVertexBuffer(sizeof(MPSeaTileVertex) * nVertexCnt, 
		        D3DUSAGE_WRITEONLY, FVF_SEA,  D3DPOOL_DEFAULT, &_pVB, NULL);

	        if(FAILED(hr)) 
	        {
                ToLogService("error", "msg MPMap::Render() Sea Vertex Buffer!");
                _pVB = NULL;
            }
        }
#endif
    }


    if(_bShowCenterPoint)
	{
		_RenderFocusRect();
		g_Render.AddLine(D3DXVECTOR3(_fShowCenterX, _fShowCenterY, 0.0f), D3DXVECTOR3(_fShowCenterX, _fShowCenterY, 4.0f), 0xFF0000FF);
   	}

	g_Render.ResetWorldTransform();

	g_Render.SetRenderState(D3DRS_CLIPPING, TRUE);
	
	g_Render.SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	g_Render.SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA);	 // blend the colors based on the
	g_Render.SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	 // alpha value
				
			
	g_Render.SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
	g_Render.SetRenderState(D3DRS_DITHERENABLE, FALSE);
    g_Render.SetRenderState(D3DRS_COLORVERTEX, TRUE);

    g_Render.EnableZBuffer(TRUE);
	g_Render.SetRenderState( D3DRS_LIGHTING, _bEnableNormalLight );

	
	D3DMATERIALX material;
	D3DUtil_InitMaterial( material, 1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient.r = 0.80f;
	material.Ambient.g = 0.80f;
	material.Ambient.b = 0.80f;
	material.Ambient.a = 1.0f;
	
	g_Render.GetDevice()->SetMaterial(&material);

	g_Render.SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW); // 
	
	g_Render.SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE); 
	g_Render.SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
			
	g_Render.SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE); 
	g_Render.SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	g_Render.SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_MODULATE);  
	
	g_Render.SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);  

	g_Render.SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);  
	g_Render.SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_DIFFUSE);  
	g_Render.SetTextureStageState(1, D3DTSS_COLOROP,   D3DTOP_MODULATE);  
	


	//g_Render.SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);		
	//g_Render.SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
	//g_Render.SetTextureStageState( 0, D3DTSS_MIPFILTER, D3DTEXF_LINEAR);
	g_Render.SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_Render.SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_Render.SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	
	//g_Render.SetTextureStageState( 1, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);		
	//g_Render.SetTextureStageState( 1, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
	//g_Render.SetTextureStageState( 1, D3DTSS_MIPFILTER, D3DTEXF_LINEAR);
	g_Render.SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_Render.SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_Render.SetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

    //g_Render.SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_ANISOTROPIC);
    //g_Render.SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_ANISOTROPIC);
    //g_Render.SetTextureStageState( 1, D3DTSS_MAGFILTER, D3DTEXF_ANISOTROPIC);
    //g_Render.SetTextureStageState( 1, D3DTSS_MINFILTER, D3DTEXF_ANISOTROPIC);
    //g_Render.SetTextureStageState(0, D3DTSS_MAXANISOTROPY, 16);
    //g_Render.SetTextureStageState(1, D3DTSS_MAXANISOTROPY, 16);


	g_Render.SetVertexShader(NULL);
	g_Render.SetFVF(FVF_LAND);
	
    //-------------------------------------------------------------------
    //Render Logic Cycle Begin-------------------------------------------
    //-------------------------------------------------------------------
    
    int nTileStartX = (int)(_fShowCenterX - (float)(_nShowWidth)  / 2.0f);
	int nTileStartY = (int)(_fShowCenterY - (float)(_nShowHeight) / 2.0f);

	if(nTileStartX!=_nLastTileStartX || nTileStartY!=_nLastTileStartY)
    {
       _nUpdateRender = 2;
	   _nLastTileStartX = nTileStartX;
       _nLastTileStartY = nTileStartY;
       _nLastGridStartX = nTileStartX * 2;
       _nLastGridStartY = nTileStartY * 2;
    }

	//int nCurSectionX = _fShowCenterX / _nSectionWidth;
	//int	nCurSectionY = _fShowCenterY / _nSectionHeight;

	//if( nCurSectionX != _nLastSectionX || nCurSectionY != _nLastSectionY )
	//{

	//}
    
	if(m_bCullTile)
	{
		_GenerateTerrainGroup(nTileStartX, nTileStartY);
		_FillVB(); 
	}else
	{
		if(_nUpdateRender)
		{
			if(_nUpdateRender==2)
			{
				_GenerateTerrainGroup(nTileStartX, nTileStartY);
			}
			
			_FillVB();
		}
	}
 
    _RenderVB();


    _nUpdateRender = 0;
    //-------------------------------------------------------------------
    //Render Logic Cycle End---------------------------------------------
    //-------------------------------------------------------------------
    
    
   	g_Render.SetTexture(0, NULL); 
	g_Render.SetTexture(1, NULL);
		
	if(_bWireFrame)
	{
		g_Render.SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		g_Render.EnableZBuffer(FALSE);
		g_Render.EnableAlpha(FALSE);
		g_Render.SetVertexShader(NULL);
		g_Render.SetFVF(FVF_LAND);
	    
        _RenderVB(_bWireFrame);
        
        g_Render.SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		g_Render.EnableZBuffer(TRUE);
		g_Render.EnableAlpha(TRUE);
	}
	
    g_Render.SetRenderState(D3DRS_CLIPPING, TRUE);

    g_Render.SetTextureStageState(1, D3DTSS_COLOROP,   D3DTOP_DISABLE);  

    m_dwTerrainRenderTime = t.End();

	//SetSkyDoom(true);
	//CreateSkyDoom(D3DXVECTOR3(_fShowCenterX,_fShowCenterY,0),76,"texture/terrain/clouds.jpg",false);
}

void MPMap::RenderSmMap()
{
	if(_fp==NULL) return; 
	
	g_Render.ResetWorldTransform();

	g_Render.SetRenderState(D3DRS_CLIPPING, TRUE);
	
	g_Render.SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	g_Render.SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA);	 // blend the colors based on the
	g_Render.SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	 // alpha value
				
			
	g_Render.SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
	g_Render.SetRenderState(D3DRS_DITHERENABLE, FALSE);
    g_Render.SetRenderState(D3DRS_COLORVERTEX, TRUE);

	//g_Render.SetTextureStageState( 1, D3DTSS_MAGFILTER, D3DTEXF_POINT);	// tile texture
	//g_Render.SetTextureStageState( 1, D3DTSS_MINFILTER, D3DTEXF_POINT);
	//g_Render.SetTextureStageState( 1, D3DTSS_MIPFILTER, D3DTEXF_POINT);
	g_Render.SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_Render.SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_Render.SetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	
	//g_Render.SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_POINT);	// tile texture
	//g_Render.SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_POINT);
	//g_Render.SetTextureStageState( 0, D3DTSS_MIPFILTER, D3DTEXF_POINT);
	g_Render.SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_Render.SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_Render.SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	
	g_Render.EnableZBuffer(FALSE);
	g_Render.SetRenderState( D3DRS_LIGHTING, TRUE );
	
	D3DMATERIALX material;
	D3DUtil_InitMaterial( material, 1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient.r = 0.50f;
	material.Ambient.g = 0.50f;
	material.Ambient.b = 0.50f;
	material.Ambient.a = 1.0f;
	
	g_Render.GetDevice()->SetMaterial(&material);

	g_Render.SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW); // 
	
	g_Render.SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE); 
	g_Render.SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);
			
	g_Render.SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE); 
	g_Render.SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	g_Render.SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_MODULATE);  
	
	g_Render.SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);  
	g_Render.SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_DIFFUSE);  
	g_Render.SetTextureStageState(1, D3DTSS_COLOROP,   D3DTOP_MODULATE);  
	
	
	//g_Render.SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);		
	//g_Render.SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
	//g_Render.SetTextureStageState( 0, D3DTSS_MIPFILTER, D3DTEXF_LINEAR);
	g_Render.SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_Render.SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_Render.SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	//
	//g_Render.SetTextureStageState( 1, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);		
	//g_Render.SetTextureStageState( 1, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
	//g_Render.SetTextureStageState( 1, D3DTSS_MIPFILTER, D3DTEXF_LINEAR);
	g_Render.SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_Render.SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_Render.SetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	g_Render.SetVertexShader(NULL);
	g_Render.SetFVF(FVF_LAND);
	
	int nTileStartX = (int)(_fShowCenterX - (float)(_nShowWidth)  / 2.0f);
	int nTileStartY = (int)(_fShowCenterY - (float)(_nShowHeight) / 2.0f);

	// if(nTileStartX < 0) nTileStartX
	
	MPTile *TileList[4];
	int x, y, i;
	for(y = 0; y < _nShowHeight; y++)
	{
		for(x = 0; x < _nShowWidth; x++)
		{
			int nX = nTileStartX + x;
			int nY = nTileStartY + y;
			
			if(nX < 0 || nX >= _nWidth || nY < 0 || nY >= _nHeight) continue;
			
			MPTile *pTile = GetTile(nX, nY);
			
			if(!pTile) continue;

			TileList[0] = pTile;
			TileList[1] = GetGroupTile(nX, nY, 1);
			TileList[2] = GetGroupTile(nX, nY, 2);
			TileList[3] = GetGroupTile(nX, nY, 3);
			
			_CalTileNormal(nX, nY);
	
			for(i = 0; i < 4; i++)
			{
				MPTileTex *pLayer = &(pTile->TexLayer[i]);
				
				if(pLayer->btTexNo==0) break;
				if(pLayer->btAlphaNo==0) continue;

				float fScale = 0.25f;
				float fU = (float)(nX % (int)(1.0f / fScale)) * fScale;
				float fV = (float)(nY % (int)(1.0f / fScale)) * fScale;
				
				if(pLayer->btAlphaNo==15)
				{
					//g_Render.SetTextureStageState(0, D3DTSS_ADDRESSU, D3DTADDRESS_WRAP);
					//g_Render.SetTextureStageState(0, D3DTSS_ADDRESSV, D3DTADDRESS_WRAP);
					g_Render.SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
					g_Render.SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
					g_Render.EnableAlpha(FALSE);
				
					g_Render.SetTexture(0, GetTextureByID(GetTerrainTextureID(pLayer->btTexNo)));
					g_Render.SetTexture(1, NULL);
					
					pTile->SetTerrainUV(0, fU, fV, fScale);
					pTile->RenderTerrain(nX, nY, TileList);
				}
				else
				{
					//g_Render.SetTextureStageState(0, D3DTSS_ADDRESSU, D3DTADDRESS_MIRROR);
					//g_Render.SetTextureStageState(0, D3DTSS_ADDRESSV, D3DTADDRESS_MIRROR);
					g_Render.SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR);
					g_Render.SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR);
					g_Render.EnableAlpha(TRUE);
					
					g_Render.SetTexture(0, GetTextureByID(GetTerrainAlphaTextureID(pLayer->btAlphaNo))); 
					g_Render.SetTexture(1, GetTextureByID(GetTerrainTextureID(pLayer->btTexNo)));

					pTile->SetTerrainUV(0, 0.0f, 0.0f, 1.0f);
					pTile->SetTerrainUV(1, fU, fV, fScale);

					pTile->RenderTerrain(nX, nY, TileList);
				}
			}
		}
	}
	
	//g_Render.SetTexture(0, NULL); 
	//g_Render.SetTexture(1, NULL);
	//
	//g_Render.SetRenderState(D3DRS_CLIPPING, TRUE);
}

void MPMap::_RenderFocusRect()
{
	int nSectionX = (int)(_fShowCenterX / (float)_nSectionWidth)  * _nSectionWidth;
	int nSectionY = (int)(_fShowCenterY / (float)_nSectionHeight) * _nSectionHeight;

	int nTileStartX = nSectionX;
	int nTileStartY = nSectionY;
	
	VECTOR3 v[32]; int x, y, n = 0, nX, nY;

	float fHeight = SEA_LEVEL;
	float fStep   = 0.01f;
	
	for(x = 0; x < _nSectionWidth; x++)
	{
		nX = nTileStartX + x; nY = nTileStartY;
		
		MPTile *pTile = GetTile(nX, nY);
		if(pTile->IsDefault()) fHeight = SEA_LEVEL + fStep;
		else                   fHeight = pTile->fHeight + fStep;
		v[n].x = (float)nX; v[n].y = (float)nY; v[n].z = fHeight;
		n++;
	}
	for(y = 0; y < _nSectionHeight; y++)
	{
		nX = nTileStartX + _nSectionWidth; nY = nTileStartY + y;
		
		MPTile *pTile = GetTile(nX, nY);
		if(pTile->IsDefault()) fHeight = SEA_LEVEL + fStep;
		else                   fHeight = pTile->fHeight + fStep;
		v[n].x = (float)nX; v[n].y = (float)nY; v[n].z = fHeight;
		n++;
	}
	for(x = 0; x < _nSectionWidth; x++)
	{
		nX = nTileStartX + _nSectionWidth - x; nY = nTileStartY + _nSectionHeight;
		
		MPTile *pTile = GetTile(nX, nY);
		if(pTile->IsDefault()) fHeight = SEA_LEVEL + fStep;
		else                   fHeight = pTile->fHeight + fStep;
		v[n].x = (float)nX; v[n].y = (float)nY; v[n].z = fHeight;
		n++;
	}
	for(y = 0; y < _nSectionHeight; y++)
	{
		nX = nTileStartX; nY = nTileStartY + _nSectionHeight - y;
		MPTile *pTile = GetTile(nX, nY);
		if(pTile->IsDefault()) fHeight = SEA_LEVEL + fStep;
		else                   fHeight = pTile->fHeight + fStep;
		v[n].x = (float)nX; v[n].y = (float)nY; v[n].z = fHeight;
		n++;
	}
	for(int k = 0; k < n; k++)
	{
		if(k==n-1)
		{
			g_Render.AddLine(v[k], v[0] ,0xFFFFFF00);
			break;
		}
		g_Render.AddLine(v[k], v[k + 1] ,0xFFFFFF00);
	}
}

void MPMap::RenderSea()
{
    if(!_bRenderSea) return;

	MPTimer t; t.Begin();
    
    g_Render.ResetWorldTransform();
	
	int nStartX = (int)(_fShowCenterX - (float)_nShowWidth  / 2.0f);
	int nStartY = (int)(_fShowCenterY - (float)_nShowHeight / 2.0f);

	g_Render.SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE); 
	g_Render.SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);
	g_Render.SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW); // 
	
	g_Render.SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA);	 // blend the colors based on the
	g_Render.SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	 // alpha value
	
	g_Render.EnableAlpha(TRUE);
	g_Render.EnableZBuffer(TRUE);
	
	g_Render.SetRenderState( D3DRS_LIGHTING, FALSE );

	g_Render.SetTexture(0, NULL);
	g_Render.SetTexture(1, NULL);
	
	MPSeaTileVertex SVertex[4];
	SVertex[0].tu = 0;
	SVertex[0].tv = 0;
	SVertex[1].tu = 1.0;
	SVertex[1].tv = 0;
	SVertex[2].tu = 0.0;
	SVertex[2].tv = 1.0;
	SVertex[3].tu = 1.0;
	SVertex[3].tv = 1.0;

	SVertex[0].dwColor = 0xFFffffff;
	SVertex[1].dwColor = 0xFFffffff;
	SVertex[2].dwColor = 0xFFffffff;
	SVertex[3].dwColor = 0xFFffffff; 
	
	g_Render.SetTextureStageState( 0, D3DTSS_COLOROP,  D3DTOP_MODULATE); // D3DTOP_DOTPRODUCT3 );
	g_Render.SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	g_Render.SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_CURRENT );
	
	g_Render.SetTexture(0, GetTextureByID(GetWaterBumpTextureID(_nWaterLoopFrame))); 
	
	g_Render.SetVertexShader(NULL);
	g_Render.SetFVF(FVF_SEA);

	g_Render.SetTextureStageState(0, D3DTSS_ALPHAOP,  D3DTOP_SELECTARG1  );
	g_Render.SetTextureStageState(0, D3DTSS_ALPHAARG1,D3DTA_DIFFUSE );
	g_Render.SetTextureStageState(0, D3DTSS_ALPHAARG2,D3DTA_TEXTURE);
	// g_Render.SetTexture(1, NULL);

	int nSize = SEA_TILE_SIZE;
	int nSeaStartX = nStartX / nSize;
	int nSeaStartY = nStartY / nSize;

	int nSeaCntX = _nShowWidth  / nSize;
	int nSeaCntY = _nShowHeight / nSize;

	if(_nShowWidth  % nSize!=0)  nSeaCntX++;
	if(_nShowHeight % nSize!=0)  nSeaCntY++;
	
	int nTileCnt   = nSeaCntX * nSeaCntY;
	int nVertexCnt = nTileCnt * 6;
	int nTriCnt    = nTileCnt * 2;

	// LPDIRECT3DINDEXBUFFER8   _pIB = NULL;
	MPSeaTileVertex   *pCurVertex = NULL;
	
	int x, y;
	if(_bUseVB)
	{
//#if(defined DRAW_SEA_USE_DYNAMIC_BUFFER)
//        DWORD num = 0;
//
//		for(y = 0; y < nSeaCntY; y++)
//		{
//			for(x = 0; x < nSeaCntX; x++)
//			{
//				int nX = (nSeaStartX + x) * nSize;
//				int nY = (nSeaStartY + y) * nSize;
//
//				if(nX < 0 || nX >= _nWidth || nY < 0 || nY >= _nHeight) continue;
//			
//				SVertex[0].dwColor = _dwSeaDefaultColor;
//				SVertex[1].dwColor = _dwSeaDefaultColor;
//				SVertex[2].dwColor = _dwSeaDefaultColor;
//				SVertex[3].dwColor = _dwSeaDefaultColor;
//			
//				for(int i = 0; i < 4; i++)
//				{
//					int nCurX = nX + MPTile::Offset[i][0] * nSize;
//					int nCurY = nY + MPTile::Offset[i][1] * nSize;
//				
//					SVertex[i].p = VECTOR3( (float)nCurX, (float)nCurY, SEA_LEVEL);
//				
//					MPTile *pTile = GetTile(nCurX, nCurY);
//					if((SEA_LEVEL - pTile->fHeight) < 0.5f)
//					{
//						SVertex[i].dwColor = 0x00ffffff; 
//					}
//				}   
//				// 0 1 2 
//				memcpy(&_pVertBuf[num], &SVertex, 3 * sizeof(MPSeaTileVertex));
//				num += 3;  
//				
//				// 1 3 4
//                _pVertBuf[num++] = SVertex[3];
//                _pVertBuf[num++] = SVertex[2];
//                _pVertBuf[num++] = SVertex[1];
//			} 
//		}
//	
//        assert(nTriCnt == num / 3);
//
//        lwInterfaceMgr* imgr = g_Render.GetInterfaceMgr();
//        lwIDynamicStreamMgr* dsm = imgr->res_mgr->GetDynamicStreamMgr();
//        dsm->BindDataVB(0, _pVertBuf, sizeof(MPSeaTileVertex) * num, sizeof(MPSeaTileVertex));
//        if(LW_FAILED(dsm->DrawPrimitive(D3DPT_TRIANGLELIST, 0, num / 3)))
//            LG("error","msg render sea");
//
//#else
		HRESULT hr = _pVB->Lock(0, nVertexCnt * sizeof(MPSeaTileVertex), (void**)&pCurVertex, D3DLOCK_NOOVERWRITE );
	    if(FAILED(hr)) return;
		for(y = 0; y < nSeaCntY; y++)
		{
			for(x = 0; x < nSeaCntX; x++)
			{
				int nX = (nSeaStartX + x) * nSize;
				int nY = (nSeaStartY + y) * nSize;

				if(nX < 0 || nX >= _nWidth || nY < 0 || nY >= _nHeight) continue;
			
				SVertex[0].dwColor = _dwSeaDefaultColor;
				SVertex[1].dwColor = _dwSeaDefaultColor;
				SVertex[2].dwColor = _dwSeaDefaultColor;
				SVertex[3].dwColor = _dwSeaDefaultColor;
			
				for(int i = 0; i < 4; i++)
				{
					int nCurX = nX + MPTile::Offset[i][0] * nSize;
					int nCurY = nY + MPTile::Offset[i][1] * nSize;
				
					SVertex[i].p = VECTOR3( (float)nCurX, (float)nCurY, SEA_LEVEL);
				
					MPTile *pTile = GetTile(nCurX, nCurY);
					if((SEA_LEVEL - pTile->fHeight) < 0.5f)
					{
						SVertex[i].dwColor = 0x00ffffff; 
					}
				}   
				// 1 2 3 
				memcpy(pCurVertex, &SVertex, 3 * sizeof(MPSeaTileVertex));
				pCurVertex+=3;  
				
				// 1 3 4
				memcpy(pCurVertex, &SVertex[3], 1 * sizeof(MPSeaTileVertex));
				pCurVertex++;
				memcpy(pCurVertex, &SVertex[2], 1 * sizeof(MPSeaTileVertex));
				pCurVertex++;
				memcpy(pCurVertex, &SVertex[1], 1 * sizeof(MPSeaTileVertex));
				pCurVertex++;
			} 
		}
	
		_pVB->Unlock();
		g_Render.SetStreamSource(0, _pVB, 0, sizeof(MPSeaTileVertex));
		if(FAILED(g_Render.DrawPrimitive(D3DPT_TRIANGLELIST, 0, nTriCnt)))
			ToLogService("error", "msg render sea");
//#endif
	}
	else
	{
		for(y = 0; y < nSeaCntY; y++)
		{
			for(x = 0; x < nSeaCntX; x++)
			{
				int nX = (nSeaStartX + x) * nSize;
				int nY = (nSeaStartY + y) * nSize;

				if(nX < 0 || nX >= _nWidth || nY < 0 || nY >= _nHeight) continue;
			
				SVertex[0].dwColor = _dwSeaDefaultColor;
				SVertex[1].dwColor = _dwSeaDefaultColor;
				SVertex[2].dwColor = _dwSeaDefaultColor;
				SVertex[3].dwColor = _dwSeaDefaultColor;
			
				for(int i = 0; i < 4; i++)
				{
					int nCurX = nX + MPTile::Offset[i][0] * nSize;
					int nCurY = nY + MPTile::Offset[i][1] * nSize;
				
					SVertex[i].p = VECTOR3( (float)nCurX, (float)nCurY, SEA_LEVEL);
				
					MPTile *pTile = GetTile(nCurX, nCurY);
					if((SEA_LEVEL - pTile->fHeight) < 0.5f)
					{
						SVertex[i].dwColor = 0x00ffffff;
					}
				}

                // begin by lsh
#if(defined DRAW_TERRAIN_USE_DYNAMIC_BUFFER)

                lwInterfaceMgr* imgr = g_Render.GetInterfaceMgr();
                lwIDynamicStreamMgr* dsm = imgr->res_mgr->GetDynamicStreamMgr();
                dsm->BindDataVB(0, &SVertex, sizeof(MPSeaTileVertex) * 4, sizeof(MPSeaTileVertex));
				if(LW_FAILED(dsm->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2)))
					ToLogService("error", "msg render sea");
#else
				g_Render.GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, &SVertex, sizeof(MPSeaTileVertex));
#endif
                // end
			}
		}
	}

	g_Render.SetTexture(0, NULL);
	g_Render.SetTexture(1, NULL);
	
	g_Render.SetRenderState( D3DRS_LIGHTING, TRUE);

    m_dwSeaRenderTime = t.End();
}


BOOL MPMap::GetPickPos(int nPosX, int nPosY, D3DXVECTOR3 &vPickPos)
{
	vPickPos = D3DXVECTOR3(-1, -1, -10000);

	D3DXVECTOR3 vPickRayOrig, vPickRayDir;
	
	g_Render.GetPickRayVector(nPosX, nPosY, &vPickRayOrig, &vPickRayDir);
	
	float fDistance = 70;
    float fStep     = 0.6f;
	
    D3DXVECTOR3 vecEnd = vPickRayOrig + vPickRayDir * fDistance;
    list<D3DXVECTOR3> PointList;
    GetSamplePointList(vPickRayOrig.x, vPickRayOrig.y, vecEnd.x, vecEnd.y, fStep, 0.3f, PointList);
    
    for(list<D3DXVECTOR3>::iterator it = PointList.begin(); it!=PointList.end(); it++)
    {
        D3DXVECTOR3 *pVec = &(*it);

        int nX = (int)(pVec->x);
        int nY = (int)(pVec->y);

        if(nX < 0 || nX >=_nWidth )  continue;
		if(nY < 0 || nY >=_nHeight ) continue;
		
        int nOffX = nX;
        int nOffY = nY;

        float fX = (float)nX;
        float fY = (float)nY;
        
		float fHeight[4] = { SEA_LEVEL, SEA_LEVEL, SEA_LEVEL, SEA_LEVEL };
#if 0
		MPTile *pTile = GetTile(nX, nY);
		if(pTile) 
		{
			for(int i = 0; i < 4; i++)
			{
				MPTile *pCurTile = GetTile(nX + MPTile::Offset[i][0], nY + MPTile::Offset[i][1]);
				if(!pCurTile->IsDefault()) fHeight[i] = pCurTile->fHeight;
			}
		}

		//MPTile *pTile = GetTile(nX, nY);
		//if(pTile) 
		//{
		//	for(int i = 0; i < 4; i++)
		//	{
		//		MPTile *pCurTile = GetTile(nX + MPTile::Offset[i][0], nY + MPTile::Offset[i][1]);
		//		if(!pCurTile->IsDefault()) fHeight[i] = pCurTile->fHeight;
		//	}
		//}
#else
		for(int i = 0; i < 4; i++)
		{
			//MPTile *pCurTile = GetTile(nX + MPTile::Offset[i][0], nY + MPTile::Offset[i][1]);
			//if(!pCurTile->IsDefault()) fHeight[i] = pCurTile->fHeight;
			fHeight[i] = GetGridHeight((nX + MPTile::Offset[i][0])* 2,\
				(nY + MPTile::Offset[i][1])* 2);
		}
#endif
		float fx1 = (float)nX;
		float fx2 = (float)nX + 1;
		float fy1 = (float)nY;
		float fy2 = (float)nY + 1;
		
		D3DXVECTOR3 v0( fx1, fy1, fHeight[0]); 
		D3DXVECTOR3 v1( fx2, fy1, fHeight[1]); 
		D3DXVECTOR3 v2( fx1, fy2, fHeight[2]); 
		D3DXVECTOR3 v3( fx2, fy2, fHeight[3]); 		
		
		float u, v;
		if( D3DXIntersectTri(&v0, &v1, &v2, &vPickRayOrig, &vPickRayDir, &u, &v, NULL) == TRUE)
		{
			vPickPos = v0 + u * (v1 - v0) + v * (v2 - v0);	
			return TRUE;
		}
			
		if( D3DXIntersectTri(&v2, &v1, &v3, &vPickRayOrig, &vPickRayDir, &u, &v, NULL) == TRUE)
		{
			vPickPos = v2 + u * (v1 - v2) + v * (v3 - v2);		
			return TRUE;
		}
	}
	return FALSE; 
}

BOOL MPMap::GetPickPosEditor(int nPosX, int nPosY, D3DXVECTOR3 &vPickPos)
{
	vPickPos = D3DXVECTOR3(-1, -1, -10000);

	D3DXVECTOR3 vPickRayOrig, vPickRayDir;
	
	g_Render.GetPickRayVector(nPosX, nPosY, &vPickRayOrig, &vPickRayDir);
	
	float fDistance = 70;
    float fStep     = 0.6f;
	
    D3DXVECTOR3 vecEnd = vPickRayOrig + vPickRayDir * fDistance;
    list<D3DXVECTOR3> PointList;
    GetSamplePointList(vPickRayOrig.x, vPickRayOrig.y, vecEnd.x, vecEnd.y, fStep, 0.3f, PointList);
    
    for(list<D3DXVECTOR3>::iterator it = PointList.begin(); it!=PointList.end(); it++)
    {
        D3DXVECTOR3 *pVec = &(*it);

        int nX = (int)(pVec->x);
        int nY = (int)(pVec->y);

        if(nX < 0 || nX >=_nWidth )  continue;
		if(nY < 0 || nY >=_nHeight ) continue;
		
        int nOffX = nX;
        int nOffY = nY;

        float fX = (float)nX;
        float fY = (float)nY;
        
		float fHeight[4] = { SEA_LEVEL, SEA_LEVEL, SEA_LEVEL, SEA_LEVEL };
#if 1
		MPTile *pTile = GetTile(nX, nY);
		if(pTile) 
		{
			for(int i = 0; i < 4; i++)
			{
				MPTile *pCurTile = GetTile(nX + MPTile::Offset[i][0], nY + MPTile::Offset[i][1]);
				if(!pCurTile->IsDefault()) fHeight[i] = pCurTile->fHeight;
			}
		}

		//MPTile *pTile = GetTile(nX, nY);
		//if(pTile) 
		//{
		//	for(int i = 0; i < 4; i++)
		//	{
		//		MPTile *pCurTile = GetTile(nX + MPTile::Offset[i][0], nY + MPTile::Offset[i][1]);
		//		if(!pCurTile->IsDefault()) fHeight[i] = pCurTile->fHeight;
		//	}
		//}
#else
		for(int i = 0; i < 4; i++)
		{
			//MPTile *pCurTile = GetTile(nX + MPTile::Offset[i][0], nY + MPTile::Offset[i][1]);
			//if(!pCurTile->IsDefault()) fHeight[i] = pCurTile->fHeight;
			fHeight[i] = GetGridHeight((nX + MPTile::Offset[i][0])* 2,\
				(nY + MPTile::Offset[i][1])* 2);
		}
#endif
		float fx1 = (float)nX;
		float fx2 = (float)nX + 1;
		float fy1 = (float)nY;
		float fy2 = (float)nY + 1;
		
		D3DXVECTOR3 v0( fx1, fy1, fHeight[0]); 
		D3DXVECTOR3 v1( fx2, fy1, fHeight[1]); 
		D3DXVECTOR3 v2( fx1, fy2, fHeight[2]); 
		D3DXVECTOR3 v3( fx2, fy2, fHeight[3]); 		
		
		float u, v;
		if( D3DXIntersectTri(&v0, &v1, &v2, &vPickRayOrig, &vPickRayDir, &u, &v, NULL) == TRUE)
		{
			vPickPos = v0 + u * (v1 - v0) + v * (v2 - v0);	
			return TRUE;
		}
			
		if( D3DXIntersectTri(&v2, &v1, &v3, &vPickRayOrig, &vPickRayDir, &u, &v, NULL) == TRUE)
		{
			vPickPos = v2 + u * (v1 - v2) + v * (v3 - v2);		
			return TRUE;
		}
	}
	return FALSE; 
}


float MPMap::GetHeight(float fX, float fY)  // ?  
{
	int nX = (int)fX;
	int nY = (int)fY;
	
	float fx1 = (float)nX;
	float fx2 = (float)nX + 1;
	float fy1 = (float)nY;
	float fy2 = (float)nY + 1;
		
	float fHeight[4] = { SEA_LEVEL, SEA_LEVEL, SEA_LEVEL, SEA_LEVEL };
	
	MPTile *pTile = GetTile(nX, nY);
	for(int i = 0; i < 4; i++)
	{
		MPTile *pCurTile = GetGroupTile(nX, nY, i);
		fHeight[i] = pCurTile->fHeight;
	}
	
	D3DXVECTOR3 v0( fx1, fy1, fHeight[0]); 
	D3DXVECTOR3 v1( fx2, fy1, fHeight[1]); 
	D3DXVECTOR3 v2( fx1, fy2, fHeight[2]); 
	D3DXVECTOR3 v3( fx2, fy2, fHeight[3]); 		
		
	VECTOR3 vOrig(fX, fY, 20.0f);
	VECTOR3 vDir(0, 0, -1);
	float u, v;
	
	VECTOR3 vPickPos;
	if( D3DXIntersectTri(&v0, &v1, &v2, &vOrig, &vDir, &u, &v, NULL) == TRUE)
	{
		vPickPos = v0 + u * (v1 - v0) + v * (v2 - v0);	
		return vPickPos.z;
	}
			
	if( D3DXIntersectTri(&v2, &v1, &v3, &vOrig, &vDir, &u, &v, NULL) == TRUE)
	{
		vPickPos = v2 + u * (v1 - v2) + v * (v3 - v2);		
		return vPickPos.z;
	}

	return 0.0f;
}

void MPMap::_CalTileNormal(int nX, int nY)
{
	VECTOR3 vAround[4], vNormal[4];

	float f1 = (float)(rand()%20) / 100.0f;
	float f2 = (float)(rand()%20) / 100.0f;
	float f3 = (float)(rand()%10) / 100.0f;
	
	vAround[ 0 ] = D3DXVECTOR3( -1,  0, GetTile(nX - 1, nY)->fHeight);
	vAround[ 1 ] = D3DXVECTOR3(  0,  1, GetTile(nX, nY + 1)->fHeight);
	vAround[ 2 ] = D3DXVECTOR3(  1,  0, GetTile(nX + 1, nY)->fHeight);
	vAround[ 3 ] = D3DXVECTOR3(  0, -1, GetTile(nX, nY - 1)->fHeight);	
	
	float fHeight = GetTile(nX, nY)->fHeight;

	VECTOR3 v = VECTOR3(0, 0, fHeight);
	vNormal[ 0 ] = ComputeNormalVector( v, vAround[1], vAround[0]);	
	vNormal[ 1 ] = ComputeNormalVector( v, vAround[2], vAround[1]);	
	vNormal[ 2 ] = ComputeNormalVector( v, vAround[3], vAround[2]);	
	vNormal[ 3 ] = ComputeNormalVector( v, vAround[0], vAround[3]);
	VECTOR3 vR = ( vNormal[ 0 ] + vNormal[ 1 ]  + vNormal[ 2 ] + vNormal[ 3 ] ) / 4;
	
	// vR = VECTOR3(0, 0, 1.0f);
	D3DXVec3Normalize(&vR, &vR);
	
	// GetTile(nX, nY)->vN = vR;
}


struct SRenderTileInfo
{
    MPTile *pTile;
    short sx;
    short sy;
    MPTile *pNeighbor[3];
};

struct STileGroup
{
    short               sTile[MAX_RENDER_TILE];
    short               sTileCnt;
    BYTE                btLayer;
    BYTE                btTextureID;
    STileGroup(): btTextureID(0), sTileCnt(0), btLayer(0){}
};

#define MAX_RENDER_GROUP 160
static SRenderTileInfo TilePool[MAX_RENDER_TILE];
static STileGroup      TileGroup[4][MAX_RENDER_GROUP];
static STileGroup*     RenderGroup[MAX_RENDER_GROUP];
static int             nRenderGroupCnt = 0;
static int             nLockVertex     = 0;
static int             nRenderTileCnt  = 0;

BOOL MPMap::_AddRenderGroup(BYTE btLayer, int nTexNo, MPTile *pTile, short sTileX, short sTileY)
{
   if(nRenderTileCnt>=MAX_RENDER_TILE) 
   {
        return FALSE;
   }

   if(nTexNo>=MAX_RENDER_GROUP) 
   {
		ToLogService("error", "msgAddRenderGroup{}", MAX_RENDER_GROUP);
		return FALSE;
   }
   if(btLayer>=4)
   {
		ToLogService("error", "msgAddRenderGroup{}", 3);
		return FALSE;
   }
   
   STileGroup *pGroup = &TileGroup[btLayer][nTexNo];
   pGroup->btLayer = btLayer;
   pGroup->btTextureID = nTexNo;
   pGroup->sTile[pGroup->sTileCnt] = nRenderTileCnt;
   pGroup->sTileCnt++;
   
   SRenderTileInfo *pInfo = &TilePool[nRenderTileCnt];
   pInfo->pTile = pTile;
   pInfo->sx    = sTileX;
   pInfo->sy    = sTileY;
   pInfo->pNeighbor[0] = GetGroupTile(sTileX, sTileY, 1);
   pInfo->pNeighbor[1] = GetGroupTile(sTileX, sTileY, 2);
   pInfo->pNeighbor[2] = GetGroupTile(sTileX, sTileY, 3);
   
   nRenderTileCnt++;
   return TRUE;
}


void MPMap::_GenerateTerrainGroup(int nTileStartX, int nTileStartY)
{
    // Terrain Texture GroupVB
    for(int i = 0; i < nRenderGroupCnt; i++)
    {
        RenderGroup[i]->sTileCnt = 0;
    }
    nRenderGroupCnt = 0;
    
    MPTimer t;
    t.Begin();

    nRenderTileCnt = 0;

    BOOL bFull = FALSE;

    // memset(_fHeightBuffer, 0, 4 * 200 * 200);
    // memset(_btBlockBuffer, 1, 200 * 200);
	// memset(_fTileHeightBuffer, 0, 100 * 100 * 4);
    float fh[4];
	_bRenderSea = FALSE;

    for(short y = 0; y < _nShowHeight; y++)
	{
		for(short x = 0; x < _nShowWidth; x++)
		{
			short sX = nTileStartX + x;
			short sY = nTileStartY + y;
			if(sX < 0 || sX >= _nWidth || sY < 0 || sY >= _nHeight) continue;
		
			MPTile *pTile = GetTile(sX, sY);
            if(!pTile) continue;

			if(pTile->fHeight < (SEA_LEVEL - 0.05f)) 
			{
				_bRenderSea = TRUE; // 
			}

			_fTileHeightBuffer[y][x] = pTile->fHeight;
			_sTileRegionAttr[y][x]   = pTile->sRegion;
            
			int gx = x * 2;
            int gy = y * 2;
			
			_fHeightBuffer[gy][gx]         = pTile->getObjHeight(0);
            _fHeightBuffer[gy][gx + 1]     = pTile->getObjHeight(1);
            _fHeightBuffer[gy + 1][gx]     = pTile->getObjHeight(2);
            _fHeightBuffer[gy + 1][gx + 1] = pTile->getObjHeight(3);
            
            _btBlockBuffer[gy][gx]         = pTile->IsBlock(0);
            _btBlockBuffer[gy][gx + 1]     = pTile->IsBlock(1);
            _btBlockBuffer[gy + 1][gx]     = pTile->IsBlock(2);
            _btBlockBuffer[gy + 1][gx + 1] = pTile->IsBlock(3);
            
            //lemon add for cull
			if(m_bCullTile)
			{
				fh[0] = pTile->fHeight;
				fh[1] = GetGroupTile(sX, sY, 1)->fHeight;
				fh[2] = GetGroupTile(sX, sY, 2)->fHeight;
				fh[3] = GetGroupTile(sX, sY, 3)->fHeight;

				if(!pTile->IsVisibale(sX, sY, fh))
					continue;
			}
			
			if(pTile->TexLayer[0].btTexNo==0) continue;
            
            if(pTile->TexLayer[0].btAlphaNo==15) 
            {
                if(!_AddRenderGroup(0, pTile->TexLayer[0].btTexNo, pTile, sX, sY))
                {
                    bFull = TRUE;
                    goto groupend;
                }
            }
            
            for(int i = 1; i < 4; i++)
			{
				MPTileTex *pLayer = &(pTile->TexLayer[i]);
			
				if(pLayer->btTexNo==0)   break;
				if(pLayer->btAlphaNo==0) continue;

                if(!_AddRenderGroup(i, pLayer->btTexNo, pTile, sX, sY))
                {
                    bFull = TRUE;
                    goto groupend;
                }
            }
        }
	}

	{
		int nBlockStartX = (int)(_fShowCenterX - (float)(m_iRange) / 2.0f);
		int nBlockStartY = (int)(_fShowCenterY - (float)(m_iRange) / 2.0f);

		m_pBlock->SetGrid(nBlockStartX * 2, nBlockStartY * 2);

		for (short y = 0; y < m_iRange; y++)
		{
			for (short x = 0; x < m_iRange; x++)
			{
				short sX = nBlockStartX + x;
				short sY = nBlockStartY + y;
				if (sX < 0 || sX >= _nWidth || sY < 0 || sY >= _nHeight) continue;

				ZRBlockData* pBlockData = m_pBlock->GetBlock(sX, sY);
				if (!pBlockData) continue;

				m_pBlock->m_sTileRegionAttr[y][x] = pBlockData->sRegion;

				int gx = x * 2;
				int gy = y * 2;

				m_pBlock->m_btBlockBuffer[gy][gx] = pBlockData->IsBlock(0);
				m_pBlock->m_btBlockBuffer[gy][gx + 1] = pBlockData->IsBlock(1);
				m_pBlock->m_btBlockBuffer[gy + 1][gx] = pBlockData->IsBlock(2);
				m_pBlock->m_btBlockBuffer[gy + 1][gx + 1] = pBlockData->IsBlock(3);
			}
		}
	}
groupend:

    if(bFull) ToLogService("error", "msg(>{})", MAX_RENDER_TILE * 2);

    nLockVertex = 0;

    for(int n = 0; n < 4; n++)
    {
        for(int i = 0; i < MAX_RENDER_GROUP; i++)
        {
            STileGroup *pGroup = &TileGroup[n][i];
            if(pGroup->sTileCnt)
            {
                RenderGroup[nRenderGroupCnt] = pGroup;
                nRenderGroupCnt++;
                nLockVertex+=(pGroup->sTileCnt * 6);
            }
        }
    }
    DWORD dwGroupTime = t.End();

    // if(rand()%10==5) LG("group", "G : %d A : %d L : %d F : %d \n", dwGroupTime, dwArrangeTime, dwLockTime, dwFillTime);
}

void MPMap::_FillVB()
{
    if(_pLandVB==NULL) return;
    
    MPTimer t; t.Begin();
        
    MPTileVertex *pCurVertex = NULL;
    HRESULT hr = _pLandVB->Lock(0, nLockVertex * sizeof(MPTileVertex), (void**)&pCurVertex, 0 );
	if(FAILED(hr)) 
    {
        ToLogService("terrain", "Terrain Land VB Lock Fail!");
        return;
    }

    MPTile *TileList[4];
    
    int nSize = sizeof(MPTileVertex);
            
    for(int j = 0; j < nRenderGroupCnt; j++)
    {
        STileGroup *pGroup = RenderGroup[j];
            
        int nLayer = pGroup->btLayer;
        for(int i = 0; i < pGroup->sTileCnt; i++)
        {
            SRenderTileInfo *pInfo = &TilePool[pGroup->sTile[i]];
            MPTile *pTile = pInfo->pTile;
			if( !pTile ) 
			{
				ToLogService("render", "msgTerrain _FillVB pTile = null!, {}", pGroup->sTile[i]);
				continue;
			}
            int nX = pInfo->sx;
            int nY = pInfo->sy;
            TileList[0] = pTile;
            TileList[1] = pInfo->pNeighbor[0];
			TileList[2] = pInfo->pNeighbor[1];
			TileList[3] = pInfo->pNeighbor[2];
		
			float fU = (float)(nX % 4) * .25f;
            float fV = (float)(nY % 4) * .25f;
				
            if(nLayer==0)
            {
                pTile->SetTerrainUV(0, fU, fV, .25f);
			}
            else
            {
                MPTileTex *pLayer = &pTile->TexLayer[nLayer];
                float fU1 = AlphaNo2UV[pLayer->btAlphaNo][0];
                float fV1 = AlphaNo2UV[pLayer->btAlphaNo][1];
                pTile->SetTerrainUV_Inter(0, fU1, fV1, 0.25f);
                pTile->SetTerrainUV(1, fU, fV, 0.25f);
            }
            
            pTile->RenderTerrain(nX, nY, TileList);
                
            memcpy(pCurVertex, &(MPTile::_TVertex[0]), nSize * 3);
            pCurVertex+=3;				   
            

            // 1 3 4
            memcpy(pCurVertex, &(MPTile::_TVertex[3]), nSize);
            pCurVertex++;
            memcpy(pCurVertex, &(MPTile::_TVertex[2]), nSize);
            pCurVertex++;
            memcpy(pCurVertex, &(MPTile::_TVertex[1]), nSize);
            pCurVertex++;
            
        }
    }

    DWORD dwFillTime = t.End();

    _pLandVB->Unlock();
}

void MPMap::_RenderVB(BOOL bWireframe)
{
    if(_pLandVB==NULL) return;
    
    g_Render.GetDevice()->SetStreamSource(0, _pLandVB, 0, sizeof(MPTileVertex));
    //g_Render.SetTexture(0, NULL);
        
    int nStartVertex = 0;
    for(int j = 0; j < nRenderGroupCnt; j++)
    {
        STileGroup *pGroup = RenderGroup[j];
        if(pGroup->sTileCnt==0) continue;

        if(pGroup->btLayer==0)
        {
            g_Render.EnableAlpha(FALSE);
		    //g_Render.SetTextureStageState(0, D3DTSS_ADDRESSU, D3DTADDRESS_WRAP);
		    //g_Render.SetTextureStageState(0, D3DTSS_ADDRESSV, D3DTADDRESS_WRAP);
			g_Render.SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
			g_Render.SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
            g_Render.SetTextureStageState(1, D3DTSS_COLOROP,   D3DTOP_DISABLE);
            //g_Render.SetTexture(1, NULL);
            if(!bWireframe) 
            {
                g_Render.SetTexture(0, GetTextureByID(GetTerrainTextureID(pGroup->btTextureID)));
            }
        }
        else
        {
            if(bWireframe) continue;
            g_Render.SetTexture(0, GetTextureByID(GetTerrainAlphaTextureID_I()));
            g_Render.EnableAlpha(TRUE);
			//g_Render.SetTextureStageState(0, D3DTSS_ADDRESSU, D3DTADDRESS_MIRROR);
            //g_Render.SetTextureStageState(0, D3DTSS_ADDRESSV, D3DTADDRESS_MIRROR);
			g_Render.SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR);
			g_Render.SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR);
            g_Render.SetTextureStageState(1, D3DTSS_COLOROP,   D3DTOP_MODULATE);
            g_Render.SetTexture(1, GetTextureByID(GetTerrainTextureID(pGroup->btTextureID)));
	    }
        g_Render.DrawPrimitive(D3DPT_TRIANGLELIST, nStartVertex, pGroup->sTileCnt * 2);
        nStartVertex+=(pGroup->sTileCnt * 6);

        if(pGroup->btLayer && !bWireframe)
        {
            g_Render.SetTexture(1, NULL);
        }
    }
	g_Render.EnableAlpha(TRUE);
	//g_Render.SetTextureStageState(0, D3DTSS_ADDRESSU, D3DTADDRESS_MIRROR);
	//g_Render.SetTextureStageState(0, D3DTSS_ADDRESSV, D3DTADDRESS_MIRROR);
	//g_Render.SetTextureStageState(1, D3DTSS_COLOROP,   D3DTOP_MODULATE);
}

//jze
BOOL MPMap::UseShader()
{
	if( (g_Render.GetOrgCap().VertexShaderVersion < D3DVS_VERSION(1,1)) || (g_Render.GetOrgCap().PixelShaderVersion < D3DPS_VERSION(1,1)) )
	{
		return false;
	}
	else
	{
		return true;
	}
}

void MPMap::CreateSkyDoom(D3DXVECTOR3 center, float radius, char* txPath, bool hemisphere)
{
	if(!m_bSkyDoom) return;

	int phi_begin = 0;
	int phi_end = 90;
	int theta_begin = 0;
	int theta_end = 360;

	int dphi = 15;
	int dtheta = 15;

	int VertNum;
	if(hemisphere)
	{
		VertNum = 6*abs(((phi_end-phi_begin)/dphi)*((theta_end-theta_begin)/dtheta));
	}
	else
	{
		VertNum = 12*abs(((phi_end-phi_begin)/dphi)*((theta_end-theta_begin)/dtheta));	
	}

	if(m_pSkyDoomVB == NULL)
	{
		m_pSkyDoomVB = new MPSkyDoomVertex[VertNum];
	}

	MPSkyDoomVertex* ptVB = m_pSkyDoomVB;	

	for(int i=phi_begin; i<phi_end; i+=dphi)
	{
		for(int j=theta_begin; j<theta_end; j+=dtheta)
		{
				ptVB->p.x = center.x + float(radius) * sinf(float(i)/180*PI) * cosf(float(j)/180*PI);
				ptVB->p.y = center.y + float(radius) * sinf(float(i)/180*PI) * sinf(float(j)/180*PI);
				ptVB->p.z = float(radius) * cosf(float(i)/180*PI);
				ptVB->tu = SKYDOOM_TILE_SIZE*float(i)/abs(phi_end-phi_begin);
				ptVB->tv = SKYDOOM_TILE_SIZE*float(j)/abs(theta_end-theta_begin);
				ptVB++;

				ptVB->p.x = center.x + float(radius) * sinf(float(i)/180*PI) * cosf(float(j+dtheta)/180*PI);
				ptVB->p.y = center.y + float(radius) * sinf(float(i)/180*PI) * sinf(float(j+dtheta)/180*PI);
				ptVB->p.z = float(radius) * cosf(float(i)/180*PI);
				ptVB->tu = SKYDOOM_TILE_SIZE*float(i)/abs(phi_end-phi_begin);
				ptVB->tv = SKYDOOM_TILE_SIZE*float(j+dtheta)/abs(theta_end-theta_begin);
				ptVB++;

				ptVB->p.x = center.x + float(radius) * sinf(float(i+dphi)/180*PI) * cosf(float(j+dtheta)/180*PI);
				ptVB->p.y = center.y + float(radius) * sinf(float(i+dphi)/180*PI) * sinf(float(j+dtheta)/180*PI);
				ptVB->p.z = float(radius) * cosf(float(i+dphi)/180*PI);
				ptVB->tu = SKYDOOM_TILE_SIZE*float(i+dphi)/abs(phi_end-phi_begin);
				ptVB->tv = SKYDOOM_TILE_SIZE*float(j+dtheta)/abs(theta_end-theta_begin);
				ptVB++;

				ptVB->p.x = center.x + float(radius) * sinf(float(i)/180*PI) * cosf(float(j)/180*PI);
				ptVB->p.y = center.y + float(radius) * sinf(float(i)/180*PI) * sinf(float(j)/180*PI);
				ptVB->p.z = float(radius) * cosf(float(i)/180*PI);
				ptVB->tu = SKYDOOM_TILE_SIZE*float(i)/abs(phi_end-phi_begin);
				ptVB->tv = SKYDOOM_TILE_SIZE*float(j)/abs(theta_end-theta_begin);
				ptVB++;

				ptVB->p.x = center.x + float(radius) * sinf(float(i+dphi)/180*PI) * cosf(float(j+dtheta)/180*PI);
				ptVB->p.y = center.y + float(radius) * sinf(float(i+dphi)/180*PI) * sinf(float(j+dtheta)/180*PI);
				ptVB->p.z = float(radius) * cosf(float(i+dphi)/180*PI);
				ptVB->tu = SKYDOOM_TILE_SIZE*float(i+dphi)/abs(phi_end-phi_begin);
				ptVB->tv = SKYDOOM_TILE_SIZE*float(j+dtheta)/abs(theta_end-theta_begin);
				ptVB++;

				ptVB->p.x = center.x + float(radius) * sinf(float(i+dphi)/180*PI) * cosf(float(j)/180*PI);
				ptVB->p.y = center.y + float(radius) * sinf(float(i+dphi)/180*PI) * sinf(float(j)/180*PI);
				ptVB->p.z = float(radius) * cosf(float(i+dphi)/180*PI);
				ptVB->tu = SKYDOOM_TILE_SIZE*float(i+dphi)/abs(phi_end-phi_begin);
				ptVB->tv = SKYDOOM_TILE_SIZE*float(j)/abs(theta_end-theta_begin);
				ptVB++;
		}
	} 

	if(!hemisphere)
	{
		for(int i=phi_begin; i<phi_end; i+=dphi)
		{
			for(int j=theta_begin; j<theta_end; j+=dtheta)
			{
					ptVB->p.x = center.x + float(radius) * sinf(float(i)/180*PI) * cosf(float(j)/180*PI);
					ptVB->p.y = center.y + float(radius) * sinf(float(i)/180*PI) * sinf(float(j)/180*PI);
					ptVB->p.z = -float(radius) * cosf(float(i)/180*PI);
					ptVB->tu = SKYDOOM_TILE_SIZE*float(i)/abs(phi_end-phi_begin);
					ptVB->tv = SKYDOOM_TILE_SIZE*float(j)/abs(theta_end-theta_begin);
					ptVB++;

					ptVB->p.x = center.x + float(radius) * sinf(float(i)/180*PI) * cosf(float(j+dtheta)/180*PI);
					ptVB->p.y = center.y + float(radius) * sinf(float(i)/180*PI) * sinf(float(j+dtheta)/180*PI);
					ptVB->p.z = -float(radius) * cosf(float(i)/180*PI);
					ptVB->tu = SKYDOOM_TILE_SIZE*float(i)/abs(phi_end-phi_begin);
					ptVB->tv = SKYDOOM_TILE_SIZE*float(j+dtheta)/abs(theta_end-theta_begin);
					ptVB++;

					ptVB->p.x = center.x + float(radius) * sinf(float(i+dphi)/180*PI) * cosf(float(j+dtheta)/180*PI);
					ptVB->p.y = center.y + float(radius) * sinf(float(i+dphi)/180*PI) * sinf(float(j+dtheta)/180*PI);
					ptVB->p.z = -float(radius) * cosf(float(i+dphi)/180*PI);
					ptVB->tu = SKYDOOM_TILE_SIZE*float(i+dphi)/abs(phi_end-phi_begin);
					ptVB->tv = SKYDOOM_TILE_SIZE*float(j+dtheta)/abs(theta_end-theta_begin);
					ptVB++;

					ptVB->p.x = center.x + float(radius) * sinf(float(i)/180*PI) * cosf(float(j)/180*PI);
					ptVB->p.y = center.y + float(radius) * sinf(float(i)/180*PI) * sinf(float(j)/180*PI);
					ptVB->p.z = -float(radius) * cosf(float(i)/180*PI);
					ptVB->tu = SKYDOOM_TILE_SIZE*float(i)/abs(phi_end-phi_begin);
					ptVB->tv = SKYDOOM_TILE_SIZE*float(j)/abs(theta_end-theta_begin);
					ptVB++;

					ptVB->p.x = center.x + float(radius) * sinf(float(i+dphi)/180*PI) * cosf(float(j+dtheta)/180*PI);
					ptVB->p.y = center.y + float(radius) * sinf(float(i+dphi)/180*PI) * sinf(float(j+dtheta)/180*PI);
					ptVB->p.z = -float(radius) * cosf(float(i+dphi)/180*PI);
					ptVB->tu = SKYDOOM_TILE_SIZE*float(i+dphi)/abs(phi_end-phi_begin);
					ptVB->tv = SKYDOOM_TILE_SIZE*float(j+dtheta)/abs(theta_end-theta_begin);
					ptVB++;

					ptVB->p.x = center.x + float(radius) * sinf(float(i+dphi)/180*PI) * cosf(float(j)/180*PI);
					ptVB->p.y = center.y + float(radius) * sinf(float(i+dphi)/180*PI) * sinf(float(j)/180*PI);
					ptVB->p.z = -float(radius) * cosf(float(i+dphi)/180*PI);
					ptVB->tu = SKYDOOM_TILE_SIZE*float(i+dphi)/abs(phi_end-phi_begin);
					ptVB->tv = SKYDOOM_TILE_SIZE*float(j)/abs(theta_end-theta_begin);
					ptVB++;
			}
		} 			
	}


	if(UseShader())
	{
		lwInterfaceMgr* imgr = g_Render.GetInterfaceMgr();
		IDirect3DDeviceX* dev = imgr->dev_obj->GetDevice();

		/* DWORD dwDecl[] =
		{
			D3DVSD_STREAM(0),
			D3DVSD_REG(D3DVSDE_POSITION,  D3DVSDT_FLOAT3),
			D3DVSD_REG(D3DVSDE_TEXCOORD1, D3DVSDT_FLOAT2),
			D3DVSD_END()
		}; */

		char Path[MAX_PATH];
		LPD3DXBUFFER pCode = 0;   
		LPD3DXBUFFER pError = 0;
		if(m_SkyDoomVertexShaderHandle == NULL)
		{
			sprintf(Path,"shader\\vs_skydoom.vsh");
			if(SUCCEEDED(D3DXAssembleShaderFromFile( Path, NULL, NULL, 0, &pCode, &pError )))
			{
				if(FAILED(dev->CreateVertexShader((DWORD*)pCode->GetBufferPointer(),(IDirect3DVertexShaderX**)&m_SkyDoomVertexShaderHandle)))
				{
					m_SkyDoomVertexShaderHandle = 0;			
					MessageBox( 0,"Failed To Create Vertex Shader","D3DXAssembleShaderFromFile For Vertex Shader Failed",0);
				}
			}
			else
			{		
				m_SkyDoomVertexShaderHandle = 0;
				if(pError != NULL)
				{
					const char* str = (const char*)pError->GetBufferPointer();
					MessageBox( 0,str,"D3DXAssembleShaderFromFile For Vertex Shader Failed", 0 );
				}
				else
				{
					MessageBox( 0, "File Of Vertex Shader Not Exist","D3DXAssembleShaderFromFile For Vertex Shader Failed",0);			
				}
			}
			if( pError )
			{
				pError->Release();
				pError = 0;
			}
			if( pCode )
			{
				pCode->Release();
				pCode = 0;
			}
		}
		//create pixel shader
		if(m_SkyDoomPixelShaderHandle == NULL)
		{
			sprintf(Path,"shader\\ps_skydoom.psh");
			if(SUCCEEDED(D3DXAssembleShaderFromFile(Path, NULL, NULL, 0, &pCode, &pError )))
			{
				if(FAILED(dev->CreatePixelShader((DWORD*)pCode->GetBufferPointer(),(IDirect3DPixelShaderX**)&m_SkyDoomPixelShaderHandle)))
				{
					m_SkyDoomPixelShaderHandle = 0;			
					MessageBox( 0,"Failed To Create Pixel Shader","D3DXAssembleShaderFromFile For Pixel Shader Failed",0);
				}
			}
			else
			{
				m_SkyDoomPixelShaderHandle = 0;
				if(pError != NULL)
				{
					const char* str = (const char*)pError->GetBufferPointer();
					MessageBox( 0, str,"D3DXAssembleShaderFromFile For Pixel Shader Failed",0);
				}
				else
				{
					MessageBox( 0, "File Of Pixel Shader Not Exist","D3DXAssembleShaderFromFile For Pixel Shader Failed",0);
				}
			}
			if( pError )
			{
				pError->Release();
				pError = 0;
			}
			if( pCode )
			{
				pCode->Release();
				pCode = 0;
			}
		}	

		g_Render.ResetWorldTransform();
		g_Render.SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);
		g_Render.SetTexture(0, GetTextureByID(GetTextureID(txPath))); /*"texture/terrain/xingkong01.bmp"*/ 
		//g_Render.SetTextureStageState(0, D3DTSS_ADDRESSU, D3DTADDRESS_WRAP);
		//g_Render.SetTextureStageState(0, D3DTSS_ADDRESSV, D3DTADDRESS_WRAP);
		g_Render.SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
		g_Render.SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
		//
		//render 
		// i remark these code for close the shader
		D3DXMATRIX mat;  
		D3DXMatrixMultiply(&mat,&g_Render.GetWorldViewMatrix(),&g_Render.GetWorldProjMatrix());
		D3DXMatrixTranspose( &mat, &mat );
		dev->SetVertexShaderConstantF(0,mat,4);
		dev->SetVertexShaderConstantF(4,D3DXVECTOR4(0,(m_txMoveSpeed-=1.0f)/10000*SKYDOOM_TEXTURE_MOVESPEED,0,0),1); //add by jze pass current frame number to vertex shader 
		dev->SetVertexShader((IDirect3DVertexShaderX*)m_SkyDoomVertexShaderHandle);
		dev->SetPixelShader((IDirect3DPixelShaderX*)m_SkyDoomPixelShaderHandle);

		lwIDynamicStreamMgr* dsm = g_Render.GetInterfaceMgr()->res_mgr->GetDynamicStreamMgr();
		dsm->BindDataVB(0, m_pSkyDoomVB, sizeof(MPSkyDoomVertex)*VertNum, sizeof(MPSkyDoomVertex));
		if(LW_FAILED(dsm->DrawPrimitive(D3DPT_TRIANGLELIST, 0, VertNum/3)))
			ToLogService("error", "msg render sky box");

		g_Render.SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);
		dev->SetPixelShader(0);
	}
	else
	{
		g_Render.ResetWorldTransform();
		g_Render.SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);
		g_Render.SetTexture(0, GetTextureByID(GetTextureID(txPath))); /*"texture/terrain/xingkong01.bmp"*/
		//g_Render.SetTextureStageState(0, D3DTSS_ADDRESSU, D3DTADDRESS_WRAP);
		//g_Render.SetTextureStageState(0, D3DTSS_ADDRESSV, D3DTADDRESS_WRAP);
		g_Render.SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
		g_Render.SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
		//
		//render
		// i remark these code for close the shader
		g_Render.SetVertexShader(NULL);
		g_Render.SetFVF(FVF_SKYDOOM);


		lwIDynamicStreamMgr* dsm = g_Render.GetInterfaceMgr()->res_mgr->GetDynamicStreamMgr();
		dsm->BindDataVB(0, m_pSkyDoomVB, sizeof(MPSkyDoomVertex)*VertNum, sizeof(MPSkyDoomVertex));
		if(LW_FAILED(dsm->DrawPrimitive(D3DPT_TRIANGLELIST, 0, VertNum/3)))
			ToLogService("error", "msg render sky box");

		g_Render.SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);
	}
}

void MPMap::SetupPixelFog(DWORD Color, DWORD Mode,float Start, float End, float Density)
{
    // Enable fog blending.
    g_Render.GetDevice()->SetRenderState(D3DRS_FOGENABLE, TRUE);
 
    // Set the fog color.
    g_Render.GetDevice()->SetRenderState(D3DRS_FOGCOLOR, Color);
    
    // Set fog parameters.
    if(D3DFOG_LINEAR == Mode)
    {
        g_Render.GetDevice()->SetRenderState(D3DRS_FOGTABLEMODE, Mode);
        g_Render.GetDevice()->SetRenderState(D3DRS_FOGSTART, *(DWORD *)(&Start));
        g_Render.GetDevice()->SetRenderState(D3DRS_FOGEND,   *(DWORD *)(&End));
    }
    else
    {
        g_Render.GetDevice()->SetRenderState(D3DRS_FOGTABLEMODE, Mode);
        g_Render.GetDevice()->SetRenderState(D3DRS_FOGDENSITY, *(DWORD *)(&Density));
    }
}
