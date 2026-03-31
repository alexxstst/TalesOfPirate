#pragma once
#include "TableData.h"
#include "ItemTypeRecord.h"
#include "ItemTypeRecordStore.h"
#include "AssetDatabase.h"


class CItemTypeSet : public CRawDataSet
{
public:
	static CItemTypeSet* I() { return _Instance; }
	
	CItemTypeSet(int nIDStart, int nIDCnt)
	:CRawDataSet(nIDStart, nIDCnt)
	{
		_Instance = this;
		_Init();
    }

protected:

	static CItemTypeSet* _Instance; // , 
   
	virtual CRawDataInfo* _CreateRawDataArray(int nCnt)
	{
        return new CItemTypeInfo[nCnt];
	}
	
	virtual void _DeleteRawDataArray()
	{
		delete[] (CItemTypeInfo*)_RawDataArray;
	}
	
	virtual int _GetRawDataInfoSize()
	{
		return sizeof(CItemTypeInfo);
	}

	virtual void*	_CreateNewRawData(CRawDataInfo *pInfo)
	{
		return NULL;
	}

	virtual void  _DeleteRawData(CRawDataInfo *pInfo)
	{
		SAFE_DELETE(pInfo->pData);
	}
	
	virtual BOOL _ReadRawDataInfo(CRawDataInfo *pRawDataInfo, std::vector<std::string> &ParamList)
	{   
        // if(ParamList.size()==0) return FALSE;
		
        CItemTypeInfo *pInfo = (CItemTypeInfo*)pRawDataInfo;
        return TRUE;
    }

	virtual void _ProcessRawDataInfo(CRawDataInfo *pRawDataInfo)
	{
		ItemTypeRecordStore::Insert(AssetDatabase::Instance()->GetDb(), *(CItemTypeInfo*)pRawDataInfo);
	}
};
