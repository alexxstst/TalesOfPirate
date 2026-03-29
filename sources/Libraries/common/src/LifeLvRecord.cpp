//=============================================================================
// FileName: LifeLvRecord.cpp
// Creater: ZhangXuedong
// Date: 2005.05.28
// Comment: CLifeLvRecord class
//=============================================================================

#include "LifeLvRecord.h"
using namespace std;

CLifeLvRecordSet * CLifeLvRecordSet::_Instance = NULL;

BOOL CLifeLvRecordSet::_ReadRawDataInfo(CRawDataInfo *pRawDataInfo, vector<string> &ParamList)
{   
	if(ParamList.size()==0) return FALSE;

	CLifeLvRecord *pInfo = (CLifeLvRecord*)pRawDataInfo;

	int m = 0, n = 0;
    string strList[80];
	string strLine;

	// 
	pInfo->lID = pInfo->nID;
	// 
	pInfo->sLevel = Str2Int(pInfo->szDataName);
	// 
	pInfo->ulExp = Str2Int(ParamList[m++]);

	return TRUE;
}
