#pragma once

//=============================================================================
// FileName: SwitchMapRecord.h
// Creater: ZhangXuedong
// Date: 2004.11.23
// Comment:
//=============================================================================

#ifndef SWITCHMAPRECORD_H
#define SWITCHMAPRECORD_H

#include <tchar.h>
#include "util.h"
#include "TableData.h"
#include "point.h"

const char cchSwitchMapRecordKeyValue = (char)0xff;

#define defMAP_NAME_LEN		16

class CSwitchMapRecord : public CRawDataInfo
{
public:
	//CSwitchMapRecord();

	long	lID;							// 
	long	lEntityID;						// ID
	long	lEventID;						// 
	Point	SEntityPos;						// 
	short	sAngle;							// 
	_TCHAR	szTarMapName[defMAP_NAME_LEN];	// 
	Point	STarPos;						// 
};

class CSwitchMapRecordSet : public CRawDataSet
{
public:

	static CSwitchMapRecordSet* I() { return _Instance; }

	CSwitchMapRecordSet(int nIDStart, int nIDCnt, int nCol = 128)
		:CRawDataSet(nIDStart, nIDCnt, nCol)
	{
		_Instance = this;
		_Init();
	}

protected:

	static CSwitchMapRecordSet* _Instance; // , 

	virtual CRawDataInfo* _CreateRawDataArray(int nCnt)
	{
		return new CSwitchMapRecord[nCnt];
	}

	virtual void _DeleteRawDataArray()
	{
		delete[] (CSwitchMapRecord*)_RawDataArray;
	}

	virtual int _GetRawDataInfoSize()
	{
		return sizeof(CSwitchMapRecord);
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

inline CSwitchMapRecord* GetSwitchMapRecordInfo( int nTypeID )
{
	return (CSwitchMapRecord*)CSwitchMapRecordSet::I()->GetRawDataInfo(nTypeID);
}

#endif
