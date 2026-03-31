#pragma once

#include "PoseRecord.h"
#include "PoseRecordStore.h"
#include "AssetDatabase.h"

class CPoseSet : public CRawDataSet
{
public:
	static CPoseSet* I() { return _Instance; }

	CPoseSet(int nIDStart, int nIDCnt)
	:CRawDataSet(nIDStart, nIDCnt)
	{
		_Instance = this;
		_Init();
	}

protected:
	static CPoseSet* _Instance;

	virtual CRawDataInfo* _CreateRawDataArray(int nCnt) { return new CPoseInfo[nCnt]; }
	virtual void _DeleteRawDataArray() { delete[] (CPoseInfo*)_RawDataArray; }
	virtual int _GetRawDataInfoSize() { return sizeof(CPoseInfo); }
	virtual void* _CreateNewRawData(CRawDataInfo *pInfo) { return NULL; }
	virtual void _DeleteRawData(CRawDataInfo *pInfo) { SAFE_DELETE(pInfo->pData); }

	virtual BOOL _ReadRawDataInfo(CRawDataInfo *pRawDataInfo, std::vector<std::string> &ParamList)
	{
		if(ParamList.size()==0) return FALSE;

		CPoseInfo *pInfo = (CPoseInfo*)pRawDataInfo;
		for (int i = 0; i < 7; i++)
			pInfo->sRealPoseID[i] = (short)Str2Int(ParamList[i]);

		return TRUE;
	}

	virtual void _ProcessRawDataInfo(CRawDataInfo *pRawDataInfo)
	{
		PoseRecordStore::Insert(AssetDatabase::Instance()->GetDb(), *(CPoseInfo*)pRawDataInfo);
	}
};

inline CPoseInfo* GetPoseInfo(short sPoseID)
{
	return (CPoseInfo*)CPoseSet::I()->GetRawDataInfo(sPoseID);
}

inline short GetRealPoseID(short sPoseID, short sPoseType)
{
	CPoseInfo *pInfo = GetPoseInfo(sPoseID);
	return pInfo->sRealPoseID[sPoseType];
}
