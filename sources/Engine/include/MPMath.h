//#########################
// Mind Power Math Routines
// Created By Ryan Wang
//#########################

#ifndef MODUDLE_RNMath
#define MODUDLE_RNMath


D3DXVECTOR3 ComputeNormalVector( D3DXVECTOR3 v1, D3DXVECTOR3 v2, D3DXVECTOR3 v3 );

BOOL Get2DLineIntersection(float *pv1,float *pv2, float *pv3, float *pv4, float *pv0, BOOL bline);

VOID RNNormalizeVector(float *pfVector);

inline float GetM(float *pfVector)
{
    return (pfVector[0] * pfVector[0] + pfVector[1] * pfVector[1] + pfVector[2] * pfVector[2]);
}

inline float GetM2D(float *pfVector)
{
    return (pfVector[0] * pfVector[0] + pfVector[1] * pfVector[1]);
}


// p1, p2 ,p p1,p2p1,p2: added by billy
inline bool IsPoint3Intersect(D3DXVECTOR3 *p, D3DXVECTOR3* p1, D3DXVECTOR3  *p2  )
{

	if ( (( p->x - p1->x) *( p->x -p2->x)<0)  || (( p->y - p1->y) *( p->y -p2->y)<0 ) || (( p->z - p1->z) *( p->z -p2->z)<0 ) )
		return true ;
	return false ;
}

BOOL EdgeIntersectsFace(D3DXVECTOR3* pEdges, D3DXVECTOR3* pFacePoints, D3DXPLANE* pPlane);


inline float GetVectorAngle2D(float *pfVector1 , float *pfVector2)
{
    return 0.0f;
}



inline float DistanceFrom(const D3DXVECTOR3& v1, const D3DXVECTOR3& v2)
{
	D3DXVECTOR3 v3 = v1 - v2;
	return D3DXVec3Length(&v3);
}

inline int GetVectorRelativePos(float *pfVector1 , float *pfVector2)
{
    float f = pfVector1[0] * pfVector2[1] - pfVector1[1] * pfVector2[0];
    if(f > 0.0f) return 1;
    else return 0;
}

int GetVectorRelativeDirection(float *fSrcDir , float *fDestDir , float *pfAngleOff);

inline void GetPosByDistance(float *fSrcPos , float *fSrcFace , float fDis , float *fDestPos)
{
    RNNormalizeVector(fSrcFace);
    fDestPos[0] = fSrcPos[0] + fDis * fSrcFace[0];
    fDestPos[1] = fSrcPos[1] + fDis * fSrcFace[1];
    fDestPos[2] = fSrcPos[2];
}

inline float Radian2Angle(float fRadian)
{
	return (float)(fRadian * 180.0f / 3.1415926f);
}

inline float Angle2Radian(float fAngle)
{
	return (float)(fAngle * 3.1415926f / 180.0f);
}

inline float Angle2Quadrant(float fAngle, float x, float y)
{
	if(x >= 0 && y >= 0)
	{
		return fAngle;
	}
	else if(x < 0 && y >= 0)
	{
		return 180.0f - fAngle;
	}
	else if(x < 0 && y < 0)
	{
		return 180.0f + fAngle;
	}
	else if(x >= 0 && y < 0)
	{
		return 360.0f - fAngle;
	}
	return 0;
}

#define MAX(a,b)				( ((a) > (b)) ? (a) : (b) )
#define MIN(a,b)				( ((a) > (b)) ? (b) : (a) )
#define ABS(value)				(((value) > 0) ? (value) : -(value) )
#define EPS						0.000001f
#define FZERO(A)				(ABS(A) > (EPS) ? false : true)
#define FEQUAL(A, B)			(FZERO((A) - (B)))
#define CLAMP(A, MIN, MAX)		( (A) > (MAX) ) ? (MAX) :  ( ( (A) < (MIN) ) ? (MIN) : (A) )

inline DWORD FLOAT2DWORD( FLOAT f ) { return *((DWORD*)&f); }

#define SG_PI					3.1415926f
#define SG_HALF_PI				1.5707963f

#define RAD2DEG(A)				(57.295780f * (A))
#define DEG2RAD(A)				(0.0174533f * (A))

int		BSplineCurveCV(int numCV, D3DXVECTOR3* vCtrl, D3DXVECTOR3* vertices, int *numVert, int nPrecise, int nStrideInFloat = 3);

inline float CalculateCircleRate( float fValue )
{
	fValue = max( 0.0f, min( fValue, 1.0f ) );
	return ( sinf( ( fValue - 0.5f ) * D3DX_PI ) + 1.0f ) / 2;
}


inline DWORD F2DW( FLOAT f ) { return *((DWORD*)&f); }

int	GetSamplePointList(float fStartX, float fStartY, float fEndX, float fEndY, float fStep, float fStepShift, std::list<D3DXVECTOR3> &PointList);

#endif
