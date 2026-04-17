#include "stdafx.h"
#include "publicfun.h"

int GetAngleDistance(int nAngle1, int nAngle2) {
	if (nAngle1 >= 0 && nAngle1 < 90) {
		if (nAngle2 >= 0 && nAngle2 < 90) {
			return nAngle2 - nAngle1;
		}
		else if (nAngle2 >= 90 && nAngle2 < 180) {
			return nAngle2 - nAngle1;
		}
		else if (nAngle2 >= 180 && nAngle2 < 270) {
			if (nAngle2 - nAngle1 > 180) {
				return -1 * (360 - nAngle2 + nAngle1);
			}
			return nAngle2 - nAngle1;
		}
		else if (nAngle2 >= 270 && nAngle2 < 360) {
			return -1 * (360 - nAngle2 + nAngle1);
		}
	}
	else if (nAngle1 >= 90 && nAngle1 < 180) {
		if (nAngle2 >= 0 && nAngle2 < 90) {
			return nAngle2 - nAngle1;
		}
		else if (nAngle2 >= 90 && nAngle2 < 180) {
			return nAngle2 - nAngle1;
		}
		else if (nAngle2 >= 180 && nAngle2 < 270) {
			return nAngle2 - nAngle1;
		}
		else if (nAngle2 >= 270 && nAngle2 < 360) {
			if (nAngle2 - nAngle1 > 180) {
				return -1 * (360 - nAngle2 + nAngle1);
			}
			return nAngle2 - nAngle1;
		}
	}
	else if (nAngle1 >= 180 && nAngle1 < 270) {
		if (nAngle2 >= 0 && nAngle2 < 90) {
			if (nAngle1 - nAngle2 > 180) {
				return 360 - nAngle1 + nAngle2;
			}
			return nAngle2 - nAngle1;
		}
		else if (nAngle2 >= 90 && nAngle2 < 180) {
			return nAngle2 - nAngle1;
		}
		else if (nAngle2 >= 180 && nAngle2 < 270) {
			return nAngle2 - nAngle1;
		}
		else if (nAngle2 >= 270 && nAngle2 < 360) {
			return nAngle2 - nAngle1;
		}
	}
	else if (nAngle1 >= 270 && nAngle1 < 360) {
		if (nAngle2 >= 0 && nAngle2 < 90) {
			return 360 - nAngle1 + nAngle2;
		}
		else if (nAngle2 >= 90 && nAngle2 < 180) {
			if (nAngle1 - nAngle2 > 180) {
				return 360 - nAngle1 + nAngle2;
			}
			return nAngle2 - nAngle1;
		}
		else if (nAngle2 >= 180 && nAngle2 < 270) {
			return nAngle2 - nAngle1;
		}
		else if (nAngle2 >= 270 && nAngle2 < 360) {
			return nAngle2 - nAngle1;
		}
	}
	return 0;
}

int FixAngle(int nAngle) {
	if (nAngle < 0) {
		nAngle += (((nAngle / 360) + 1) * 360);
	}
	else if (nAngle >= 360) {
		nAngle -= ((nAngle / 360) * 360);
	}
	return nAngle;
}

int GetLineAngle(int x1, int y1, int x2, int y2) {
	D3DXVECTOR2 dPosition = D3DXVECTOR2((float)x2, (float)y2) - D3DXVECTOR2((float)x1, (float)y1);
	int nAngle = (int)(180.0f - Radian2Angle(atan2f(dPosition.x, dPosition.y)));
	return FixAngle(nAngle);
}

int GetDistance(__int64 x1, __int64 y1, __int64 x2, __int64 y2) {
	return (int)(sqrt((double)((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2))));
}

float GetDistance(float x1, float y1, float z1, float x2, float y2, float z2) {
	return (float)(sqrt((double)((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) + (z1 - z2) * (z1 - z2))));
}

BOOL IsValidHeight(DWORD dwCharType, float fHeight) {
	BOOL ret = TRUE;
	switch (dwCharType) {
	case 201:
	case 202:
		if (fHeight > SEA_LEVEL) {
			ret = FALSE;
		}
		break;
	case 31:
		break;
	default:
		if (fHeight < (SEA_LEVEL - 0.2f)) {
			ret = FALSE;
		}
		break;
	}
	return ret;
}

void GetDistancePos(int x1, int y1, int x2, int y2, int dis, int& x, int& y) {
	D3DXVECTOR2 v1((float)x1, (float)y1);
	D3DXVECTOR2 v2((float)x2, (float)y2);
	D3DXVECTOR2 v3 = v2 - v1;
	D3DXVec2Normalize(&v3, &v3);
	v2 = v1 + v3 * (float)dis;
	x = (int)v2.x;
	y = (int)v2.y;
}

void GetInDistancePos(int x1, int y1, int x2, int y2, int dis, int& x, int& y) {
	D3DXVECTOR2 v1((float)x1, (float)y1);
	D3DXVECTOR2 v2((float)x2, (float)y2);
	D3DXVECTOR2 v3 = v1 - v2;
	D3DXVec2Normalize(&v3, &v3);
	v2 = v1 + v3 * (float)dis;
	x = (int)v2.x;
	y = (int)v2.y;
}

void GetAnglePos(int x1, int y1, int Dist, int angle, int& x, int& y) {
	D3DXVECTOR2 v1((float)x1, (float)y1);
	D3DXVECTOR2 v2(0, 1);
	D3DXVECTOR4 ver;
	D3DXMATRIX mat;
	D3DXMatrixRotationZ(&mat, (float(angle + 180)) * 0.01745329f);
	D3DXVec2Transform(&ver, &v2, &mat);

	v2.x = ver.x;
	v2.y = ver.y;

	const auto v3 = v2 * float(Dist);
	D3DXVec2Add(&v1, &v1, &v3);
	x = (int)v1.x;
	y = (int)v1.y;
}

int GetItemForgeLv(int chForgeLv) {
	if (chForgeLv < 4) {
		return -1;
	}
	if (chForgeLv < 8) {
		return 0;
	}
	if (chForgeLv < 12) {
		return 1;
	}
	return 2;
}

int GetMovePoint(POINT* pInPoint, int nInLen, int nDis, POINT* pOutPoint, int nOutLen) {
	if (!pInPoint || !pOutPoint || nInLen <= 1 || nOutLen <= 1) {
		return -1;
	}
	int nCurDist = 0;
	int nPointer = 1;
	while (nPointer < nInLen) {
		POINT* pPoint = &pInPoint[nPointer - 1];
		POINT* pPoint2 = &pInPoint[nPointer];
		POINT* pPoint3 = &pOutPoint[nPointer];
		int nTDist = GetDistance(pPoint->x, pPoint->y, pPoint2->x, pPoint2->y);
		if (nCurDist + nTDist >= nDis) {
			int tx = 0;
			int ty = 0;
			GetDistancePos(pPoint->x, pPoint->y, pPoint2->x, pPoint2->y, nDis - nCurDist, tx, ty);
			pPoint3->x = tx;
			pPoint3->y = ty;
			return nPointer + 1;
		}
		pOutPoint[nPointer] = *pPoint;
		nCurDist += nTDist;
		nPointer++;
	}
	return -1;
}
