#pragma once


#include "MPRender.h"

#define DRAW_TERRAIN_USE_DYNAMIC_BUFFER

#define SEA_LEVEL 0.0f

struct MPTileVertex
{
	D3DXVECTOR3	p;
	// D3DXVECTOR3 n;
	DWORD		dwColor;
	float		tu1, tv1;
	float		tu2, tv2;
};

struct MPSeaTileVertex
{
	D3DXVECTOR3	p;
	DWORD		dwColor;
	float		tu, tv;
};

struct MPSkyDoomVertex
{
	D3DXVECTOR3 p;
	float       tu,tv;
};

struct MPLineVertex
{
	D3DXVECTOR3	p;
	DWORD		dwColor;
};

struct MPTileTex  //   
{
	BYTE	btTexNo;	 // 
	BYTE	btAlphaNo;	 // Alpha
	MPTileTex()
		:btTexNo(0), btAlphaNo(0)	{}
	
	BYTE SetAlphaNo(BYTE btAlphaNo_, BOOL bReset = FALSE)
	{
		if(bReset) 
		{
			btAlphaNo = btAlphaNo_;
		}
		else
		{
			btAlphaNo = btAlphaNo | btAlphaNo_;
		}
		return btAlphaNo;
	}
};

inline float _getObjHeight(BYTE btValue, int no)
{
    BYTE btLevel = btValue & 63;
    float f = (float)btLevel;
    f*=5.0f; // 5
    if(btValue & 64) // 
    {
        return f / 100.0f * -1.0f;
    }
    return f / 100.0f;
}


inline BYTE _setObjHeight(BYTE btOldValue, float fHeight, int no)
{
     if(fHeight>3.1f)  fHeight = 3.1f;
	 if(fHeight<-3.1f) fHeight = -3.1f;
	 if(fHeight < 0.0f) 
     {
         fHeight*=-1.0f;
         btOldValue|=64;
     }
     else
     {
         btOldValue&=191;
     }
     BYTE btLevel = (BYTE)(fHeight / 0.05f);
     btOldValue&=192; // 6
     btOldValue|=btLevel;
     return btOldValue;
}


struct MPTile
{
	static	int		Offset[4][2];

    MPTileTex		TexLayer[4]; // 4
	DWORD			dwColor;	 // 
    short           sRegion;     // 
    BYTE            btIsland;    // 
    BYTE            btBlock[4];  // 4
    
	float			fHeight;     // 
	
    //lemon add@2004.10.18
	DWORD			dwTColor;	 // 
    DWORD           dwXColor;    // 

public:
    
    void Init()
	{
		fHeight     = 0.0f;
        dwColor     = 0xffffffff;
        dwTColor    = 0;

		for(int i = 0; i < 4; i++)
		{
			TexLayer[i].btTexNo = TexLayer[i].btAlphaNo = 0;
		    btBlock[i] = 0;
        }
		sRegion  = 0; // ,  = 0
        btIsland = 0;
    }

	BOOL	IsDefault()
	{
		if(TexLayer[1].btTexNo==255) return TRUE;
		return FALSE;
	}
	
	void	ResetTexLayer(BYTE btTexNo)
	{
		TexLayer[0].btTexNo   = btTexNo;
		TexLayer[0].btAlphaNo = 15;
		TexLayer[1].btTexNo   = 0;
	}
	
	void	AddHeight(float fAddHeight)
	{
		if(IsDefault()) return;
		fHeight+=fAddHeight;
	}

	void	setHeight(float fSetHeight)
	{
		if(IsDefault()) return;
		fHeight = fSetHeight;
	}

    float getHeight() { return fHeight; }

    BYTE    IsBlock(BYTE no)
    {
        if(btBlock[no] & 128) return 1;
        return 0;
    }

    void setBlock(BYTE no, BOOL bSet)
    {
        if(bSet)
        {
            btBlock[no]|=128;
        }
        else
        {
            btBlock[no]&=127;
        }
    }
    
    float   getObjHeight(BYTE no)
    {
        return _getObjHeight(btBlock[no], no);
    }

    void setObjHeight(BYTE no, float fHeight)
    {
        btBlock[no] = _setObjHeight(btBlock[no], fHeight, no);
    }
     
    void    setRegion(int nRegionNo, BOOL bSet)
    {
        if(IsDefault()) return;
        short s = 1;
        s<<=(nRegionNo - 1);
        if(bSet)              sRegion|=s;
        else if(sRegion & s)  sRegion^=s;
    }

    void    setIsland(BOOL bSet)
    {
        btIsland = bSet;
    }

    BYTE    getIsland()
    {
        return btIsland;
    }
    
    BOOL    IsRegion(int nRegionNo)
    {
        short s = 1;
        s<<=(nRegionNo - 1);
        return sRegion & s; 
    }

    short  getRegionValue() { return sRegion; }

	static void	RenderSea(int nX, int nY, int nTileSize);

	void	AddTexLayer(BYTE btTexNo, BYTE btAlphaNo);
	
    void	SetTerrainUV(int nStage, float fU, float fV, float fUVSize);
	
    void	SetTerrainUV_Inter(int nStage, float fU, float fV, float fUVSize);
	
    void	SetBumpUV(int nStage, float fU, float fV, float fUVSize);
	void	RenderTerrain(int nX, int nY, MPTile *TileList[]);
	void	RenderBump(int nX, int nY, int nTileSize);

	void	setTempColor(DWORD dwcolor)		{dwTColor = dwcolor;}

	bool	IsVisibale(int sx, int sy,float* hei);
public:

	static	MPTileVertex		_TVertex[4];
	static  MPSeaTileVertex		_SVertex[4];
};



