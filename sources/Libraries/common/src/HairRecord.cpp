//======================================================================================================================
// FileName: HairRecord.cpp
// Creater: Jerry li
// Date: 2005.08.29
// Comment: CHairRecordSet class
//======================================================================================================================

#include "HairRecord.h"
using namespace std;
//---------------------------------------------------------------------------
// class CHairRecord
//---------------------------------------------------------------------------
CHairRecord::CHairRecord()	
{
	memset( szColor, 0, sizeof(szColor) );
	memset( dwNeedItem, 0, sizeof(dwNeedItem) );
	memset( IsChaUse, 0, sizeof(IsChaUse) );
	memset( dwFailItemID, 0, sizeof(dwFailItemID) );

	dwItemID = 0;
	dwMoney = 0;
	_nFailNum = 0;
}

void CHairRecord::RefreshPrivateData()
{
	_nFailNum = 0;
	for( int i=0; i<defHAIR_MAX_FAIL_ITEM; i++ )
	{
		if( dwFailItemID[i]!=0 )
		{
			_nFailNum++;
		}
		else
		{
			break;
		}
	}
}

//---------------------------------------------------------------------------
// class CHairRecordSet
//---------------------------------------------------------------------------
CHairRecordSet * CHairRecordSet::_Instance = NULL;

BOOL CHairRecordSet::_ReadRawDataInfo(CRawDataInfo *pRawDataInfo, vector<string> &ParamList)
{   
	if(ParamList.size()==0) return FALSE;

	CHairRecord *pInfo = (CHairRecord*)pRawDataInfo;

    string strList[80];
	string strLine;
	int m = 0, n = 0;
	_tcsncpy(pInfo->szColor, ParamList[m++].c_str(), sizeof(pInfo->szColor) );

	for (int i = 0; i < defHAIR_MAX_ITEM; i++)
	{
		strLine = ParamList[m++];
		n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ',');
		if( n>2 ) n=2;
		for( int j=0; j<n; j++ )
		{
			pInfo->dwNeedItem[i][j] = Str2Int(strList[j]);
		}
	}

	pInfo->dwMoney = Str2Int(ParamList[m++]);

	pInfo->dwItemID = Str2Int(ParamList[m++]);

	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ',');
	if( n>defHAIR_MAX_FAIL_ITEM ) n = defHAIR_MAX_FAIL_ITEM;
	for( int i=0; i<n; i++ )
	{
		pInfo->dwFailItemID[i] = Str2Int( strList[i] );
	}

	for( int i=0; i<4; i++ )
	{
		pInfo->IsChaUse[i] = Str2Int(ParamList[m++])!=0 ? true : false;
	}
	return TRUE;
}

void CHairRecordSet::_ProcessRawDataInfo(CRawDataInfo *pInfo)
{
	CHairRecord *pHair = (CHairRecord*)pInfo;

    // 
	pHair->RefreshPrivateData();	
}

void CHairRecordSet::_AfterLoad()
{
}
