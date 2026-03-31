#include "AreaRecord.h"

#include "AssetDatabase.h"
#include "AreaRecordStore.h"

BOOL CAreaSet::_ReadRawDataInfo(CRawDataInfo *pRawDataInfo, std::vector<std::string> &ParamList)
{
	CAreaInfo *pArea = (CAreaInfo*)pRawDataInfo;
	std::string strList[3];
	int m=0;
	int n = Util_ResolveTextLine(ParamList[m++].c_str(), strList, 3, ',');
	DWORD r = Str2Int(strList[0]);
	DWORD g = Str2Int(strList[1]);
	DWORD b = Str2Int(strList[2]);
	pArea->dwColor = RGB(r, g, b);

	pArea->nMusic = Str2Int(ParamList[m++]);

	// environment color
	n = Util_ResolveTextLine(ParamList[m++].c_str(), strList, 3, ',');
	BYTE br = Str2Int(strList[0]);
	BYTE bg = Str2Int(strList[1]);
	BYTE bb = Str2Int(strList[2]);
	pArea->dwEnvColor = (DWORD)(((BYTE)0xff << 24) | ((BYTE)br << 16) | ((BYTE)bg << 8) | (BYTE)bb);

	// light color
	n = Util_ResolveTextLine(ParamList[m++].c_str(), strList, 3, ',');
	br = Str2Int(strList[0]);
	bg = Str2Int(strList[1]);
	bb = Str2Int(strList[2]);
	pArea->dwLightColor = (DWORD)(((BYTE)0xff << 24) | ((BYTE)br << 16) | ((BYTE)bg << 8) | (BYTE)bb);

	// light direction
	n = Util_ResolveTextLine(ParamList[m++].c_str(), strList, 3, ',');
	pArea->fLightDir[0] = (float)atof(strList[0].c_str());
	pArea->fLightDir[1] = (float)atof(strList[1].c_str());
	pArea->fLightDir[2] = (float)atof(strList[2].c_str());

	pArea->chType = Str2Int(ParamList[m++]);

	AreaRecordStore::Insert(AssetDatabase::Instance()->GetDb(), *pArea);
	return TRUE;
}
