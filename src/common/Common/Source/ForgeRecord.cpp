// ForgeRecord.cpp Created by knight-gongjian 2005.1.24.
//---------------------------------------------------------

#include "ForgeRecord.h"
using namespace std;

//---------------------------------------------------------
CForgeRecordSet * CForgeRecordSet::_Instance = NULL;

BOOL CForgeRecordSet::_ReadRawDataInfo(CRawDataInfo *pRawDataInfo, vector<string> &ParamList)
{   
	if(ParamList.size()==0) return FALSE;

	CForgeRecord *pInfo = (CForgeRecord*)pRawDataInfo;

	int m = 0;
	string strList[8];

	// 
	pInfo->byLevel = pInfo->nID;
	
	// 
	pInfo->byFailure = Str2Int(pInfo->szDataName);
	
	// 
	pInfo->byRate = Str2Int(ParamList[m++]);
	if( pInfo->byRate > 100 )
		pInfo->byRate = 100;

	// 
	for( int i = 0; i < FORGE_MAXNUM_ITEM; i++ )
	{
		Util_ResolveTextLine( ParamList[m++].c_str(), strList, 8, ',' ); 
		pInfo->ForgeItem[i].sItem = Str2Int(strList[0]);
		pInfo->ForgeItem[i].byNum = Str2Int(strList[1]);
	}

	// 
	pInfo->dwMoney = Str2Int(ParamList[m++]);

	return TRUE;
}
