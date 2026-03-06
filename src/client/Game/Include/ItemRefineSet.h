#pragma once
//----------------------------------------------------------------------
// :
// :lh 2005-11-03
// :
//----------------------------------------------------------------------
#include "TableData.h"

#define ITEM_REFINE_NUM  14
class CItemRefineInfo : public CRawDataInfo
{
public:
	CItemRefineInfo()
	{
		memset( Value, 0, sizeof(Value) );
		memset( fChaEffectScale, 0, sizeof(fChaEffectScale) );
    }
	short Value[ITEM_REFINE_NUM];
	float fChaEffectScale[4];			// 
	
};

class CItemRefineSet : public CRawDataSet
{
public:
	static CItemRefineSet* I() { return _Instance; }
	
	CItemRefineSet(int nIDStart, int nIDCnt)
	:CRawDataSet(nIDStart, nIDCnt)
	{
	    _Instance = this;
		_Init();
	}

protected:
	static CItemRefineSet* _Instance; // , 
   
	virtual CRawDataInfo* _CreateRawDataArray(int nCnt)
	{
		return new CItemRefineInfo[nCnt];
	}
	
	virtual void _DeleteRawDataArray()
	{
		delete[] (CItemRefineInfo*)_RawDataArray;
	}
	
	virtual int _GetRawDataInfoSize()
	{
		return sizeof(CItemRefineInfo);
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
		
        CItemRefineInfo *pInfo = (CItemRefineInfo*)pRawDataInfo;
		for( int i=0; i<ITEM_REFINE_NUM; i++ )
			pInfo->Value[i] = Str2Int(ParamList[i]);

		for( int i=0; i<4; i++ )
			pInfo->fChaEffectScale[i] = Str2Float(ParamList[i+ITEM_REFINE_NUM]);		
        return TRUE;
    }
};

inline CItemRefineInfo* GetItemRefineInfo(int nRefineID)
{
	return (CItemRefineInfo*)CItemRefineSet::I()->GetRawDataInfo(nRefineID);
}

