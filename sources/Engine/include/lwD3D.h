//
#pragma once

#include "lwHeader.h"
#include "lwStdInc.h"

LW_BEGIN
	struct lwFVFStruct_XyzNormal {
		lwVector3 pos;
		lwVector3 normal;
	};

	struct lwFVFStruct_XyzwDiffuseTex1 {
		lwVector4 pos;
		DWORD dif;
		lwVector2 texcoord0;
	};

	struct lwFVFStruct_XyzwDiffuseTex2 {
		lwVector4 pos;
		DWORD dif;
		lwVector2 texcoord0;
		lwVector2 texcoord1;
	};

	struct lwFVFStruct_XyzDiffuse {
		lwVector3 pos;
		DWORD dif;
	};

	struct lwFVFStruct_XyzNormalTex1 {
		lwVector3 pos;
		lwVector3 normal;
		lwVector2 texcoord0;
	};

	struct lwFVFStruct_XyzNormalTex2 {
		lwVector3 pos;
		lwVector3 normal;
		lwVector2 texcoord0;
		lwVector2 texcoord1;
	};

	struct lwFVFStruct_XyzDiffuseTex1 {
		lwVector3 pos;
		DWORD dif;
		lwVector2 texcoord0;
	};

	struct lwFVFStruct_XyzNormalDiffuseTex1 {
		lwVector3 pos;
		lwVector3 normal;
		DWORD dif;
		lwVector2 texcoord0;
	};

	struct lwFVFStruct_XyzNormalDiffuseTex2 {
		lwVector3 pos;
		lwVector3 normal;
		DWORD dif;
		lwVector2 texcoord0;
		lwVector2 texcoord1;
	};

	struct lwFVFStruct_XyzUbyte4NormalTex1 {
		lwVector3 pos;
		DWORD index;
		lwVector3 normal;
		lwVector2 texcoord0;
	};

	struct lwFVFStruct_XyzUbyte4NormalTex2 {
		lwVector3 pos;
		DWORD index;
		lwVector3 normal;
		lwVector2 texcoord0;
		lwVector2 texcoord1;
	};

	struct lwFVFStruct_Xyzb2Ubyte4NormalTex1 {
		lwVector3 pos;
		float weight;
		DWORD index;
		lwVector3 normal;
		lwVector2 texcoord0;
	};

	struct lwFVFStruct_Xyzb2Ubyte4NormalTex2 {
		lwVector3 pos;
		float weight;
		DWORD index;
		lwVector3 normal;
		lwVector2 texcoord0;
		lwVector2 texcoord1;
	};

	struct lwFVFStruct_Xyzb3Ubyte4NormalTex1 {
		lwVector3 pos;
		float weight[2];
		DWORD index;
		lwVector3 normal;
		lwVector2 texcoord0;
	};

	struct lwFVFStruct_Xyzb3Ubyte4NormalTex2 {
		lwVector3 pos;
		float weight[2];
		DWORD index;
		lwVector3 normal;
		lwVector2 texcoord0;
		lwVector2 texcoord1;
	};

	struct lwFVFStruct_Xyzb4Ubyte4NormalTex1 {
		lwVector3 pos;
		float weight[3];
		DWORD index;
		lwVector3 normal;
		lwVector2 texcoord0;
	};

	struct lwFVFStruct_Xyzb4Ubyte4NormalTex2 {
		lwVector3 pos;
		float weight[3];
		DWORD index;
		lwVector3 normal;
		lwVector2 texcoord0;
		lwVector2 texcoord1;
	};

	struct lwFVFStruct_Xyzb1DiffuseTex1 {
		lwVector3 pos;
		float weight;
		DWORD dif;
		lwVector2 texcoord0;
	};

	struct lwFVFStruct_Xyzb1DiffuseTex2 {
		lwVector3 pos;
		float weight;
		DWORD dif;
		lwVector2 texcoord0;
		lwVector2 texcoord1;
	};

	struct lwFVFStruct_XyzDiffuseTex2 {
		lwVector3 pos;
		DWORD dif;
		lwVector2 texcoord0;
		lwVector2 texcoord1;
	};

#define FVF_XYZNORMAL (D3DFVF_XYZ|D3DFVF_NORMAL)
#define FVF_XYZRHWDIFFUSETEX1 (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define FVF_XYZRHWDIFFUSETEX2 (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX2)
#define FVF_XYZDIFFUSETEX2 (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX2)
#define FVF_XYZDIFFUSE ( D3DFVF_XYZ | D3DFVF_DIFFUSE )
#define FVF_XYZDIFFUSETEX1 ( D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 )
#define FVF_XYZNORMALTEX1 (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 )
#define FVF_XYZNORMALTEX2 (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX2 )
#define FVF_XYZNORMALDIFFUSETEX1 ( D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1 )
#define FVF_XYZNORMALDIFFUSETEX2 ( D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX2 )
#define FVF_XYZB1DIFFUSETEX1 (D3DFVF_XYZB1 | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define FVF_XYZB1DIFFUSETEX2 (D3DFVF_XYZB1 | D3DFVF_DIFFUSE | D3DFVF_TEX2)
#define FVF_XYZUBYTE4NORMALTEX1 (D3DFVF_XYZ | D3DFVF_LASTBETA_UBYTE4 | D3DFVF_NORMAL | D3DFVF_TEX1 )
#define FVF_XYZB2UBYTE4NORMALTEX1 (D3DFVF_XYZB2 | D3DFVF_LASTBETA_UBYTE4 | D3DFVF_NORMAL | D3DFVF_TEX1 )
#define FVF_XYZB3UBYTE4NORMALTEX1 (D3DFVF_XYZB3 | D3DFVF_LASTBETA_UBYTE4 | D3DFVF_NORMAL | D3DFVF_TEX1 )
#define FVF_XYZB4UBYTE4NORMALTEX1 (D3DFVF_XYZB4 | D3DFVF_LASTBETA_UBYTE4 | D3DFVF_NORMAL | D3DFVF_TEX1 )
#define FVF_XYZUBYTE4NORMALTEX2 (D3DFVF_XYZ | D3DFVF_LASTBETA_UBYTE4 | D3DFVF_NORMAL | D3DFVF_TEX2 )
#define FVF_XYZB2UBYTE4NORMALTEX2 (D3DFVF_XYZB2 | D3DFVF_LASTBETA_UBYTE4 | D3DFVF_NORMAL | D3DFVF_TEX2 )
#define FVF_XYZB3UBYTE4NORMALTEX2 (D3DFVF_XYZB3 | D3DFVF_LASTBETA_UBYTE4 | D3DFVF_NORMAL | D3DFVF_TEX2 )
#define FVF_XYZB4UBYTE4NORMALTEX2 (D3DFVF_XYZB4 | D3DFVF_LASTBETA_UBYTE4 | D3DFVF_NORMAL | D3DFVF_TEX2 )


LW_END
