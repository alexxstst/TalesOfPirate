#pragma once

#include "MindPowerAPI.h"
#include "TableData.h"

//
//#define USE_RESOURCE_SCRIPT
#define RESOURCE_SCRIPT 0		// 0-1-2- 3-

#pragma warning(disable: 4275)

class MINDPOWER_API MPResourceInfo : public CRawDataInfo
{
public:
	enum {	// ResourceType (.Abbr RT)
		RT_PAR = 0,
		RT_PATH = 1,
		RT_EFF = 2,
		RT_MESH = 3,
		RT_TEXTURE = 4,
		RT_UNKNOWN = -1,
	};
public:

	MPResourceInfo() : m_iType(RT_UNKNOWN) {}

	int GetType() const { return m_iType; }

public:	

	int    m_iType;
};

class MINDPOWER_API MPResourceSet : public CRawDataSet
{
public:

	static MPResourceSet& GetInstance()
	{
		assert(m_pInstance); 
		return *m_pInstance; 
	}

	static MPResourceSet* GetInstancePtr()
	{
		return m_pInstance; 
	}

	static MPResourceSet* m_pInstance; // , 

public:
	MPResourceSet(int nIDStart, int nIDCnt) : CRawDataSet(nIDStart, nIDCnt)
	{
		m_pInstance = this;
		_Init();
	}

	MPResourceInfo* GetResourceInfoByID(int nID)
	{
		return (MPResourceInfo*)GetRawDataInfo(nID);
	}

protected:
	virtual CRawDataInfo* _CreateRawDataArray(int nCnt)
	{
		return new MPResourceInfo[nCnt];
	}

	virtual void _DeleteRawDataArray()
	{
		delete[] (MPResourceInfo*)_RawDataArray;
	}

	virtual int _GetRawDataInfoSize()
	{
		return sizeof(MPResourceInfo);
	}

	virtual void* _CreateNewRawData(CRawDataInfo *pInfo)
	{
		return NULL;
	}

	virtual void  _DeleteRawData(CRawDataInfo *pInfo)
	{
		SAFE_DELETE(pInfo->pData);
	}

	virtual BOOL _ReadRawDataInfo(CRawDataInfo *pRawDataInfo, std::vector<std::string> &ParamList)
	{   
		if(ParamList.size()==0) return FALSE;

		MPResourceInfo *pInfo = (MPResourceInfo*)pRawDataInfo;

		pInfo->m_iType = Str2Int(ParamList[0]);

		return TRUE;
	}

};

#pragma warning(default: 4275)
