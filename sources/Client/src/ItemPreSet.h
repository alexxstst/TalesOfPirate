#include "TableData.h"
#include "ItemPreRecord.h"
#include "ItemPreRecordStore.h"
#include "AssetDatabase.h"


class CItemPreSet : public CRawDataSet
{
public:
	static CItemPreSet* I() { return _Instance; }
	
	CItemPreSet(int nIDStart, int nIDCnt)
	:CRawDataSet(nIDStart, nIDCnt)
	{
		_Instance = this;
		_Init();
    }

protected:

	static CItemPreSet* _Instance; // , 
   
	virtual CRawDataInfo* _CreateRawDataArray(int nCnt)
	{
        return new CItemPreInfo[nCnt];
	}
	
	virtual void _DeleteRawDataArray()
	{
		delete[] (CItemPreInfo*)_RawDataArray;
	}
	
	virtual int _GetRawDataInfoSize()
	{
		return sizeof(CItemPreInfo);
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
        CItemPreInfo *pInfo = (CItemPreInfo*)pRawDataInfo;
        return TRUE;
    }

	virtual void _ProcessRawDataInfo(CRawDataInfo *pRawDataInfo)
	{
		ItemPreRecordStore::Insert(AssetDatabase::Instance()->GetDb(), *(CItemPreInfo*)pRawDataInfo);
	}
};
