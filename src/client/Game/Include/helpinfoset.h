#pragma once
#include "TableData.h"

class CHelpInfo : public CRawDataInfo
{
public:
    CHelpInfo()
    {
		memset( m_strHelpDetail, 0x0, sizeof( m_strHelpDetail ) );
	}

	char m_strHelpDetail[2048];
};

class CHelpInfoSet : public CRawDataSet
{
public:
    static CHelpInfoSet* I() { return _Instance; }

    CHelpInfoSet(int nIDStart, int nIDCnt)
        :CRawDataSet(nIDStart, nIDCnt)
    {
        _Instance = this;
        _Init();
	}

protected:
    
	static CHelpInfoSet* _Instance; //

    virtual CRawDataInfo* _CreateRawDataArray(int nCnt)
    {
        return new CHelpInfo[nCnt];
    }

    virtual void _DeleteRawDataArray()
    {
        delete[] (CHelpInfo*)_RawDataArray;
    }

    virtual int _GetRawDataInfoSize()
    {
        return sizeof(CHelpInfo);
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
        CHelpInfo *pInfo = (CHelpInfo*)pRawDataInfo;

		//strncpy_s(pInfo->m_strHelpDetail, sizeof(pInfo->m_strHelpDetail),ParamList[0].c_str(),_TRUNCATE);
		strncpy(pInfo->m_strHelpDetail, ParamList[0].c_str(), sizeof(pInfo->m_strHelpDetail));
		return TRUE;
    }

	virtual void _ProcessRawDataInfo(CRawDataInfo *pRawDataInfo)
	{
	}
};

inline const char* GetHelpInfo(const char* pszName)
{
	int dataID = 1;

	CRawDataInfo* DataInfo = CHelpInfoSet::I()->GetRawDataInfo( dataID ++ );
	while( DataInfo )
	{
		if( strcmp( DataInfo->szDataName, pszName ) == 0 )
			return static_cast<CHelpInfo*>( DataInfo )->m_strHelpDetail;

		DataInfo = CHelpInfoSet::I()->GetRawDataInfo( dataID ++ );
	}

	return NULL;
}
