//
#pragma once

#include "lwHeader.h"
#include "lwStdInc.h"
#include "lwDirectX.h"



LW_BEGIN



#define D3DFVF_XYZ_DIF			( D3DFVF_XYZ | D3DFVF_DIFFUSE )
#define D3DFVF_XYZ_NORMAL_TEX1		( D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 )
#define D3DFVF_XYZ_DIF_TEX1		( D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 )
#define D3DFVF_XYZRHW_DIF_TEX1        ( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 )



struct D3DFVF_XyzNormalTex1
{
    float x, y, z;
    float nx, ny, nz;
    float tu, tv;
};

struct D3DFVF_XyzDif
{
	float x, y, z;
	DWORD dif;
};


struct D3DFVF_XyzDifTex1
{
	float x, y, z;
	DWORD dif;
	float tu, tv;
};

struct D3DFVF_XyzwDifTex1
{
	float x, y, z, w;
	DWORD dif;
	float tu, tv;
};



//    return c;
//    return c;

struct lwTexParam
{
	DWORD usage;
	D3DFORMAT fmt;
	D3DPOOL pool;
};

struct lwVerParam
{
    DWORD usage;
    D3DFORMAT ind_fmt;
    D3DPOOL pool;
};

class lwSprite;
class lwD3DSprite;

LW_END
