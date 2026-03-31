#pragma once

#include "EventSoundRecord.h"
#include "EventSoundRecordStore.h"
#include "AssetDatabase.h"

class CEventSoundSet : public CRawDataSet
{
public:
	static CEventSoundSet* I() { return _Instance; }

	CEventSoundSet(int nIDStart, int nIDCnt, int nCol = 8)
		:CRawDataSet(nIDStart, nIDCnt, nCol)
	{
		_Instance = this;
		_Init();
	}

protected:
	static CEventSoundSet* _Instance;

	virtual CRawDataInfo* _CreateRawDataArray(int nCnt) { return new CEventSoundInfo[nCnt]; }
	virtual void _DeleteRawDataArray() { delete[] (CEventSoundInfo*)_RawDataArray; }
	virtual int _GetRawDataInfoSize() { return sizeof(CEventSoundInfo); }
	virtual void* _CreateNewRawData(CRawDataInfo *pInfo) { return NULL; }
	virtual void _DeleteRawData(CRawDataInfo *pInfo) { SAFE_DELETE(pInfo->pData); }

	virtual BOOL _ReadRawDataInfo(CRawDataInfo *pRawDataInfo, std::vector<std::string> &ParamList)
	{
		CEventSoundInfo *pInfo = (CEventSoundInfo*)pRawDataInfo;
		pInfo->nSoundID = Str2Int(ParamList[0]);
		return TRUE;
	}

	virtual void _ProcessRawDataInfo(CRawDataInfo *pRawDataInfo)
	{
		EventSoundRecordStore::Insert(AssetDatabase::Instance()->GetDb(), *(CEventSoundInfo*)pRawDataInfo);
	}
};
