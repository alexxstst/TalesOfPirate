//=============================================================================
// FileName: SailLvRecord.cpp
// Creater: ZhangXuedong
// Date: 2005.05.28
// Comment: CSailLvRecord class
//=============================================================================

#include "SailLvRecord.h"
using namespace std;

CSailLvRecordSet * CSailLvRecordSet::_Instance = NULL;

BOOL CSailLvRecordSet::_ReadRawDataInfo(CRawDataInfo *pRawDataInfo, vector<string> &ParamList)
{   
	if(ParamList.size()==0) return FALSE;

	CSailLvRecord *pInfo = (CSailLvRecord*)pRawDataInfo;

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
