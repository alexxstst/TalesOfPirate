#pragma once

#include "TableData.h"

#define BT_
#define _ET
class CMusicInfo : public CRawDataInfo
{

public:
    
	CMusicInfo()
	{
    }

    int nType;
};


class CMusicSet : public CRawDataSet
{

public:
	
	static CMusicSet* I() { return _Instance; }
	
	CMusicSet(int nIDStart, int nIDCnt)
	:CRawDataSet(nIDStart, nIDCnt)
	{
BT_
_Instance = this;
		_Init();
	
    _ET}

protected:

	static CMusicSet* _Instance; // , 
   
	virtual CRawDataInfo* _CreateRawDataArray(int nCnt)
	{
BT_        return new CMusicInfo[nCnt];
	_ET}
	
	virtual void _DeleteRawDataArray()
	{
BT_		delete[] (CMusicInfo*)_RawDataArray;
	_ET}
	
	virtual int _GetRawDataInfoSize()
	{
		return sizeof(CMusicInfo);
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
		
        CMusicInfo *pInfo = (CMusicInfo*)pRawDataInfo;

        pInfo->nType = Str2Int(ParamList[0]);
		
        LG("music", "Read Music List [%d][%s]\n", pInfo->nID, pInfo->szDataName);
            
        return TRUE;
    }
};

inline CMusicInfo* GetMusicInfo(int nTypeID)
{
	return (CMusicInfo*)CMusicSet::I()->GetRawDataInfo(nTypeID);
}


