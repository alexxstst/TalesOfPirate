#include "Stdafx.h"
#include "MPMath.h"

using namespace std;

BOOL EdgeIntersectsFace(D3DXVECTOR3* pEdges, D3DXVECTOR3* pFacePoints, D3DXPLANE* pPlane) {
	FLOAT fDist1 = pPlane->a * pEdges[0].x + pPlane->b * pEdges[0].y +
				   pPlane->c * pEdges[0].z + pPlane->d;
	FLOAT fDist2 = pPlane->a * pEdges[1].x + pPlane->b * pEdges[1].y +
				   pPlane->c * pEdges[1].z + pPlane->d;
	if ((fDist1 > 0 && fDist2 > 0) || (fDist1 < 0 && fDist2 < 0)) {
		return FALSE;
	}

	D3DXVECTOR3 ptIntersection;
	if (NULL == D3DXPlaneIntersectLine(&ptIntersection, pPlane, &pEdges[0], &pEdges[1])) {
		return FALSE;
	}

	FLOAT fAbsA = (pPlane->a > 0 ? pPlane->a : -pPlane->a);
	FLOAT fAbsB = (pPlane->b > 0 ? pPlane->b : -pPlane->b);
	FLOAT fAbsC = (pPlane->c > 0 ? pPlane->c : -pPlane->c);
	D3DXVECTOR2 facePoints[4];
	D3DXVECTOR2 point;
	if (fAbsA > fAbsB && fAbsA > fAbsC) {
		for (INT i = 0; i < 4; i++) {
			facePoints[i].x = pFacePoints[i].y;
			facePoints[i].y = pFacePoints[i].z;
		}
		point.x = ptIntersection.y;
		point.y = ptIntersection.z;
	}
	else if (fAbsB > fAbsA && fAbsB > fAbsC) {
		for (INT i = 0; i < 4; i++) {
			facePoints[i].x = pFacePoints[i].x;
			facePoints[i].y = pFacePoints[i].z;
		}
		point.x = ptIntersection.x;
		point.y = ptIntersection.z;
	}
	else {
		for (INT i = 0; i < 4; i++) {
			facePoints[i].x = pFacePoints[i].x;
			facePoints[i].y = pFacePoints[i].y;
		}
		point.x = ptIntersection.x;
		point.y = ptIntersection.y;
	}

	FLOAT x0 = facePoints[0].x;
	FLOAT y0 = facePoints[0].y;
	FLOAT x1 = facePoints[1].x;
	FLOAT y1 = facePoints[1].y;
	FLOAT x2 = facePoints[2].x;
	FLOAT y2 = facePoints[2].y;
	BOOL bClockwise = FALSE;
	if (x1 * y2 - y1 * x2 - x0 * y2 + y0 * x2 + x0 * y1 - y0 * x1 < 0) {
		bClockwise = TRUE;
	}
	x2 = point.x;
	y2 = point.y;
	for (INT i = 0; i < 4; i++) {
		x0 = facePoints[i].x;
		y0 = facePoints[i].y;
		if (i < 3) {
			x1 = facePoints[i + 1].x;
			y1 = facePoints[i + 1].y;
		}
		else {
			x1 = facePoints[0].x;
			y1 = facePoints[0].y;
		}
		if ((x1 * y2 - y1 * x2 - x0 * y2 + y0 * x2 + x0 * y1 - y0 * x1 > 0) == bClockwise) {
			return FALSE;
		}
	}

	return TRUE;
}

D3DXVECTOR3 ComputeNormalVector( D3DXVECTOR3 v1, D3DXVECTOR3 v2, D3DXVECTOR3 v3 )
{
	D3DXVECTOR3 vResult;
	D3DXVECTOR3 vec1 = ( v2 - v1 );
	D3DXVECTOR3 vec2 = ( v3 - v1 );
	
	D3DXVec3Cross( &vResult, &vec1, &vec2 );	
	vResult = vResult / D3DXVec3Length(&vResult);
	return vResult;
}

BOOL Get2DLineIntersection(float *pv1,float *pv2, float *pv3, float *pv4, float *pv0, BOOL bline)
{
    float t1, t2;
 
    float a1, a2, b1, b2, c1, c2, d1, d2; 
 

    /*
    if( (*pv1>*pv3 && *pv1>*pv4) && (*pv2>*pv3 && *pv2>*pv4) )
        return FALSE;
    else if( *pv1<*pv3 && *pv1<*pv4 && *pv2<*pv3 && *pv2<*pv4 )
        return FALSE;
    else if( *(pv1+1)>*(pv3+1) && *(pv1+1)>*(pv4+1) && *(pv2+1)>*(pv3+1) && *(pv2+1)>*(pv4+1) )
        return FALSE;
    else if( *(pv1+1)<*(pv3+1) && *(pv1+1)<*(pv4+1) && *(pv2+1)<*(pv3+1) && *(pv2+1)<*(pv4+1) )
        return FALSE;
    */
 
    a1 = *(pv2)  - *(pv1);
    b1 = *(pv1);
    c1 = *(pv2+1)- *(pv1+1);
    d1 = *(pv1+1);
 
    a2 = *(pv4)  - *(pv3);
    b2 = *(pv3);
    c2 = *(pv4+1)- *(pv3+1);
    d2 = *(pv3+1);
 
    // t1, t2 range [0,1]
    // l1 :  x = a1 * t1 + b1;
    //       y = c1 * t1 + d1;
    // l2 :  x = a2 * t2 + b2;
    //       y = c2 * t2 + d2;
 
    if(a1*c2 == a2*c1)          return FALSE;
 

    t2 = (b1*c1 + a1*d2 - a1*d1 - b2*c1) / (a2*c1 - a1*c2);
 
    if(a1)
        t1 = (a2*t2 + b2 - b1) / a1;
    else if(c1)
        t1 = (c2*t2 + d2 - d1) / c1;
    else
        return FALSE;
 
    if(bline) {
 
        if(pv0) {
            *(pv0)  = a1*t1 + b1;
            *(pv0+1)= c1*t1 + d1;
        }
        
        return TRUE;
    }
    else {
        if( t1>=0&&t1<=1 && t2>=0&&t2<=1 ) {
 
            if(pv0) {
                *(pv0)  = a1*t1 + b1;
                *(pv0+1)= c1*t1 + d1;
            }
            
            return TRUE;
        }
        else 
            return FALSE;
    }
 
}



VOID RNNormalizeVector(float *pfVector)
{
	float m = (float)sqrt(pfVector[0] * pfVector[0] + 
		                  pfVector[1] * pfVector[1] + 
						  pfVector[2] * pfVector[2]);
	
	pfVector[0] = pfVector[0] / m;
	pfVector[1] = pfVector[1] / m;
	pfVector[2] = pfVector[2] / m;
}


int GetVectorRelativeDirection(float *fSrcDir , float *fDestDir , float *pfAngleOff)
{
	float x1 = fSrcDir[0];
	float y1 = fSrcDir[1];
	float x2 = fDestDir[0];
	float y2 = fDestDir[1];

	// Log("x1 = %.6f , y1 = %.2f ----- x2 = %.6f , y2 = %.2f\n" , x1 , y1 , x2 , y2);
    float a1 = (float)Radian2Angle( (float)(atan(fabs(y1 / x1))) );
	float a2 = (float)Radian2Angle( (float)(atan(fabs(y2 / x2))) );
	float p1 = Angle2Quadrant(a1 , x1 , y1);
	float p2 = Angle2Quadrant(a2 , x2 , y2);

	float fAngleOff = (float)fabs(p1 - p2);
	
	int nDestDir;
	if(fAngleOff < 180.0f)
	{
		if(p1 > p2) nDestDir = 1;
		else		nDestDir = 0;
	}
	else
	{
		if(p1 > p2) nDestDir = 0;
		else		nDestDir = 1;
		fAngleOff = 360.0f - fAngleOff;
	}
	
	if(pfAngleOff) 
	{
		*pfAngleOff = fAngleOff;
	}
	return nDestDir;
}

int		BSplineCurveCV(int numCV, D3DXVECTOR3* vCtrl, D3DXVECTOR3* pVertices, int *numVert, int nPrecise, int nStrideInFloat)
{
	// number of CV must lager than 4, precision must lager than 1
	if (numCV < 4 || nPrecise < 1)
		return -1;

	// calculate span number
	int nSpan = numCV - 3;	
	if (numVert) *numVert = nSpan * (nPrecise - 1);

	// return if vertex buffer is empty
	if (pVertices == NULL)
		return 1;

	// Bezier equation matrix 
	float		M16[] = { -1, 3, -3, 1, 3, -6, 3, 0, -3, 0, 3, 0, 1, 4, 1, 0 } ; 
	D3DXMATRIXA16	MBSpline(M16), MCtrl;
	D3DXMATRIXA16	M12;

	D3DXVECTOR3 *pCV = vCtrl;
	float *pOutV = (float*)pVertices;
	float *pV = (float*)pVertices;

	float h, h2, h3;
	h	= 1.f / (float)(nPrecise - 1);
	h2	= h * h;
	h3  = h2 * h;

	for (int span = 0;span < nSpan;span ++)
	{
		// set CV
		for (int idx = 0;idx < 4;idx ++)
			memcpy(MCtrl.m[idx], pCV + idx, sizeof(D3DXVECTOR3));
		
		pCV += 1;
		
		// calculate the coefficient equation
		M12 = 0.166667f * MBSpline * MCtrl;
		
		// use difference to calculate dynamic part of equation
		float ah3, bh2, ch, r, p, q, S;		

		// x, y, z
		for (int j = 0;j < 3;j ++)
		{			
			pV	  = (float*)pOutV + j;

			ah3 = M12.m[0][j] * h3;
			bh2 = M12.m[1][j] * h2;
			ch  = M12.m[2][j] * h;
			
			S   = 6 * ah3;
			r   = S + 2 * bh2;
			q   = ah3 + bh2 + ch;
			p   = M12.m[3][j];
			
			for (int i = 0;i < nPrecise - 1;i ++)
			{
				// save to output array
				*pV = p;
				pV += nStrideInFloat;
				
				// move forward to next difference
				p += q;
				q += r;
				r += S;								
			}
		}

		// move to next span
		pOutV += (nPrecise - 1) * nStrideInFloat;
		pV	  = pOutV;

	}
	return 0;
}

int GetSamplePointList(float fStartX, float fStartY, float fEndX, float fEndY, float fStep, float fStepShift, list<D3DXVECTOR3> &PointList)
{
    D3DXVECTOR3 vecStart(fStartX, fStartY, 0.0f);
    D3DXVECTOR3 vecEnd(fEndX, fEndY, 0.0f);

    D3DXVECTOR3 vecTmp = vecEnd - vecStart;
    
    D3DXVECTOR3 vecDir; D3DXVec3Normalize(&vecDir, &vecTmp);

    D3DXVECTOR3 vM = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	D3DXVECTOR3 vShift;
	D3DXVec3Cross(&vShift, &vecDir, &vM); // 
	D3DXVec3Normalize(&vShift, &vShift);
			
    float fDis = DistanceFrom(vecStart, vecEnd);

    int nStep = (int)(fDis / fStep);
    
    D3DXVECTOR3 vecShift1 = (vShift * fStepShift);
    D3DXVECTOR3 vecShift2 = (vShift * fStepShift * -1.0f);
    
    for(int i = 0; i < nStep; i++)
    {
        D3DXVECTOR3 v  = vecStart + vecDir * (float)i * fStep;
        D3DXVECTOR3 v1 = v + vecShift1;
        D3DXVECTOR3 v2 = v + vecShift2;
        
        PointList.push_back(v1); 
        PointList.push_back(v2);        
  	}
    return nStep;
}
