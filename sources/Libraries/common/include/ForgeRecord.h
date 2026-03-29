// ForgeRecord.h Created by knight-gongjian 2005.1.24.
//---------------------------------------------------------
#pragma once

#ifndef _FORGERECORD_H_
#define _FORGERECORD_H_

#include <tchar.h>
#include "util.h"
#include "TableData.h"

//---------------------------------------------------------
#define FORGE_MAXNUM_ITEM				6 // 

class CForgeRecord : public CRawDataInfo
{
public:
	BYTE byLevel;	// 
	BYTE byFailure; // 
	BYTE byRate;	// 	
	BYTE byParam;	// 
	DWORD dwMoney;  // 

	// 
	struct FORGE_ITEM
	{
		USHORT sItem;	// ID
		BYTE   byNum;	// 
		BYTE   byParam; // 
	};	
	FORGE_ITEM ForgeItem[FORGE_MAXNUM_ITEM];
};

class CForgeRecordSet : public CRawDataSet
{
public:

	static CForgeRecordSet* I() { return _Instance; }

	CForgeRecordSet(int nIDStart, int nIDCnt, int nCol = 128)
		: CRawDataSet(nIDStart, nIDCnt, nCol)
	{
		_Instance = this;
		_Init();
	}

protected:
	static CForgeRecordSet* _Instance; // , 

	virtual CRawDataInfo* _CreateRawDataArray(int nCnt)
	{
		return new CForgeRecord[nCnt];
	}

	virtual void _DeleteRawDataArray()
	{
		delete[] (CForgeRecord*)_RawDataArray;
	}

	virtual int _GetRawDataInfoSize()
	{
		return sizeof(CForgeRecord);
	}

	virtual void*	_CreateNewRawData(CRawDataInfo *pInfo)
	{
		return NULL;
	}

	virtual void  _DeleteRawData(CRawDataInfo *pInfo)
	{
		SAFE_DELETE(pInfo->pData);
	}

	virtual BOOL _ReadRawDataInfo(CRawDataInfo *pRawDataInfo, std::vector<std::string> &ParamList);

};


inline CForgeRecord* GetForgeRecordInfo(const char *pszName)
{
    return (CForgeRecord*)CForgeRecordSet::I()->GetRawDataInfo(pszName);
}

inline CForgeRecord* GetForgeRecordInfo(int nIndex)
{
    return (CForgeRecord*)CForgeRecordSet::I()->GetRawDataInfo(nIndex);
}
//---------------------------------------------------------
#endif // _FORGERECORD_H_
