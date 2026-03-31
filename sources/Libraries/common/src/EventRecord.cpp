//=============================================================================
// FileName: EventRecord.cpp
// Creater: ZhangXuedong
// Date: 2004.11.24
// Comment:
//=============================================================================

#include "EventRecord.h"
#include "AssetDatabase.h"
#include "EventRecordStore.h"
using namespace std;

CEventRecordSet * CEventRecordSet::_Instance = NULL;

BOOL CEventRecordSet::_ReadRawDataInfo(CRawDataInfo *pRawDataInfo, vector<string> &ParamList)
{   
	if(ParamList.size()==0) return FALSE;

	CEventRecord *pInfo = (CEventRecord*)pRawDataInfo;

	int m = 0, n = 0;
    string strList[80];
	string strLine;

	// 
	pInfo->lID = pInfo->nID;
	// 
	_tcsncpy(pInfo->szName, pInfo->szDataName, defEVENT_NAME_LEN);
	pInfo->szName[defEVENT_NAME_LEN - 1] = _TEXT('0');

    // 
    pInfo->sEventType = Str2Int(ParamList[m++]);
	// 
	pInfo->sArouseType = Str2Int(ParamList[m++]);
	// 
	pInfo->sArouseRadius = Str2Int(ParamList[m++]);
	// 
	pInfo->sEffect = Str2Int(ParamList[m++]);
	// 
	pInfo->sMusic = Str2Int(ParamList[m++]);
	// 
	pInfo->sBornEffect = Str2Int(ParamList[m++]);	
	// 
	pInfo->sCursor = Str2Int(ParamList[m++]);		
	// (0-,1-2-)
	pInfo->chMainChaType = Str2Int(ParamList[m++]);
	return TRUE;
}

void CEventRecordSet::_ProcessRawDataInfo(CRawDataInfo *pRawDataInfo)
{
	EventRecordStore::Insert(AssetDatabase::Instance()->GetDb(), *(CEventRecord*)pRawDataInfo);
}
