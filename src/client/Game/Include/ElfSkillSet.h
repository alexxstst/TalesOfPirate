#pragma once

#include "TableData.h"


class CElfSkillInfo : public CRawDataInfo
{
public:
	CElfSkillInfo()
	{
		nTypeID = 0;
		nIndex = 0;
    }

	int		nIndex;		// 
	int		nTypeID;	// 
};


class CElfSkillSet : public CRawDataSet
{
public:	
	static CElfSkillSet* I() { return _Instance; }
	
	CElfSkillSet(int nIDStart, int nIDCnt)
	:CRawDataSet(nIDStart, nIDCnt)
	{
	    _Instance = this;
		_Init();
	}

protected:
	static CElfSkillSet* _Instance; // , 
   
	virtual CRawDataInfo* _CreateRawDataArray(int nCnt)
	{
		return new CElfSkillInfo[nCnt];
	}
	
	virtual void _DeleteRawDataArray()
	{
		delete[] (CElfSkillInfo*)_RawDataArray;
	}
	
	virtual int _GetRawDataInfoSize()
	{
		return sizeof(CElfSkillInfo);
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
		
        CElfSkillInfo *pInfo = (CElfSkillInfo*)pRawDataInfo;
        
		pInfo->nIndex = Str2Int(ParamList[1]);            
		pInfo->nTypeID = Str2Int(ParamList[0]);
        return TRUE;
    }
};

inline CElfSkillInfo* GetElfSkillInfo(int nIndex, int nTypeID)
{
	int nCount = CElfSkillSet::I()->GetLastID() + 1;
	CElfSkillInfo* pInfo = NULL;
	for( int i=1; i<nCount; i++ )
	{
		pInfo = (CElfSkillInfo*)CElfSkillSet::I()->GetRawDataInfo(i);
		if( pInfo && pInfo->nIndex==nIndex && pInfo->nTypeID==nTypeID )
		{
			return pInfo;
		}
	}
	return NULL;
}

