#pragma once

#include "MapRecord.h"
#include "MapRecordStore.h"
#include "AssetDatabase.h"

class CMapSet : public CRawDataSet
{
public:
	static CMapSet* I() { return _Instance; }

	CMapSet(int nIDStart, int nIDCnt)
	:CRawDataSet(nIDStart, nIDCnt)
	{
		_Instance = this;
		_Init();
	}

protected:
	static CMapSet* _Instance;

	virtual CRawDataInfo* _CreateRawDataArray(int nCnt) { return new CMapInfo[nCnt]; }
	virtual void _DeleteRawDataArray() { delete[] (CMapInfo*)_RawDataArray; }
	virtual int _GetRawDataInfoSize() { return sizeof(CMapInfo); }
	virtual void* _CreateNewRawData(CRawDataInfo *pInfo) { return NULL; }
	virtual void _DeleteRawData(CRawDataInfo *pInfo) { SAFE_DELETE(pInfo->pData); }

	virtual BOOL _ReadRawDataInfo(CRawDataInfo *pRawDataInfo, std::vector<std::string> &ParamList)
	{
		CMapInfo *pInfo = (CMapInfo*)pRawDataInfo;

		strncpy(pInfo->szName, ParamList[0].c_str(), sizeof(pInfo->szName));
		pInfo->IsShowSwitch = Str2Int(ParamList[1]) != 0;

		std::string strList[3];
		int n = Util_ResolveTextLine(ParamList[2].c_str(), strList, 2, ',');
		if(n==2) {
			pInfo->nInitX = Str2Int(strList[0]);
			pInfo->nInitY = Str2Int(strList[1]);
		}

		return TRUE;
	}

	virtual void _ProcessRawDataInfo(CRawDataInfo *pRawDataInfo)
	{
		MapRecordStore::Insert(AssetDatabase::Instance()->GetDb(), *(CMapInfo*)pRawDataInfo);
	}
};
