#pragma once

#include <windef.h>

int GetAngleDistance(int nAngle1, int nAngle2);
int FixAngle(int nAngle);
int GetLineAngle(int x1, int y1, int x2, int y2);
int GetDistance(__int64 x1, __int64 y1, __int64 x2, __int64 y2);
float GetDistance(float x1, float y1, float z1, float x2, float y2, float z2);

inline BOOL IsArrivePos(int nCurPosX, int nCurPosY, int nTargetX, int nTargetY, int nDis = 1) {
	return (abs(nCurPosX - nTargetX) < nDis && abs(nCurPosY - nTargetY) < nDis);
}

BOOL IsValidHeight(DWORD dwCharType, float fHeight);
void GetDistancePos(int x1, int y1, int x2, int y2, int dis, int& x, int& y);
void GetInDistancePos(int x1, int y1, int x2, int y2, int dis, int& x, int& y);
void GetAnglePos(int x1, int y1, int Dist, int angle, int& x, int& y);
int GetItemForgeLv(int chForgeLv);
int GetMovePoint(POINT* pInPoint, int nInLen, int nDis, POINT* pOutPoint, int nOutLen);
