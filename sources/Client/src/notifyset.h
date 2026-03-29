#pragma once

#include "TableData.h"

class CNotifyInfo : public CRawDataInfo
{
public:
    CNotifyInfo() : chType(0)
	{
        memset(szInfo, 0, sizeof(szInfo));
    }
    
    char    chType;
    char    szInfo[64];

};


class CNotifySet : public CRawDataSet
{

public:
	
	static CNotifySet* I() { return _Instance; }
	
	CNotifySet(int nIDStart, int nIDCnt)
	:CRawDataSet(nIDStart, nIDCnt)
	{
	    _Instance = this;
		_Init();
	}

protected:

	static CNotifySet* _Instance; // , 
   
	virtual CRawDataInfo* _CreateRawDataArray(int nCnt)
	{
		return new CNotifyInfo[nCnt];
	}
	
	virtual void _DeleteRawDataArray()
	{
		delete[] (CNotifyInfo*)_RawDataArray;
	}
	
	virtual int _GetRawDataInfoSize()
	{
		return sizeof(CNotifyInfo);
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
        CNotifyInfo *pInfo = (CNotifyInfo*)pRawDataInfo;

        pInfo->chType = Str2Int( ParamList[0] );

		strncpy( pInfo->szInfo, ParamList[1].c_str(), sizeof(pInfo->szInfo) );
        
		return TRUE;
    }
};

inline CNotifyInfo* GetNotifyInfo(int nTypeID)
{
	return (CNotifyInfo*)CNotifySet::I()->GetRawDataInfo(nTypeID);
}


