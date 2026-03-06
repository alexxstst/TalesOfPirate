//=============================================================================
// FileName: JobInitEquip.cpp
// Creater: ZhangXuedong
// Date: 2005.01.08
// Comment: 
//=============================================================================

#include "JobInitEquip.h"
#include "CommFunc.h"

using namespace std;
CJobEquipRecordSet * CJobEquipRecordSet::_Instance = NULL;

BOOL CJobEquipRecordSet::_ReadRawDataInfo(CRawDataInfo *pRawDataInfo, vector<string> &ParamList)
{   
	if(ParamList.size()==0) return FALSE;

	CJobEquipRecord *pInfo = (CJobEquipRecord*)pRawDataInfo;

	int m = 0, n = 0;
    string strList[80];
	string strLine;

	// 
	pInfo->chID = (char)pInfo->nID;
	// 
	pInfo->chJob = (char)g_GetJobID(pInfo->szDataName);
	// 
	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ',');
	n = n > defJOB_INIT_EQUIP_MAX ? defJOB_INIT_EQUIP_MAX : n;
	for (int i = 0; i < defJOB_INIT_EQUIP_MAX; i++)
	{
		if (i < n)
			pInfo->sItemID[i] = Str2Int(strList[i]);
		else
			pInfo->sItemID[i] = 0;
	}

	return TRUE;
}
