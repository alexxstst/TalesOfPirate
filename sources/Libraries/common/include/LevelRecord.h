//=============================================================================
// FileName: LevelRecord.h
// Creater: ZhangXuedong
// Date: 2004.12.10
// Comment: CLevelRecord class
//=============================================================================

#ifndef LEVELRECORD_H
#define LEVELRECORD_H

#include <tchar.h>
#include "util.h"
#include "TableData.h"

const char cchLevelRecordKeyValue = (char)0xff;

class CLevelRecord : public CRawDataInfo
{
public:
	//CLevelRecord();

	long	lID;			// 
	short	sLevel;			// 
	unsigned int	ulExp;	// 
};

class CLevelRecordSet : public CRawDataSet
{
public:

	static CLevelRecordSet* I() { return _Instance; }

	CLevelRecordSet(int nIDStart, int nIDCnt, int nCol = 128)
		:CRawDataSet(nIDStart, nIDCnt, nCol)
	{
		_Instance = this;
		_Init();
	}

protected:

	static CLevelRecordSet* _Instance; // , 

	virtual CRawDataInfo* _CreateRawDataArray(int nCnt)
	{
		return new CLevelRecord[nCnt];
	}

	virtual void _DeleteRawDataArray()
	{
		delete[] (CLevelRecord*)_RawDataArray;
	}

	virtual int _GetRawDataInfoSize()
	{
		return sizeof(CLevelRecord);
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

inline CLevelRecord* GetLevelRecordInfo( int nTypeID )
{
	return (CLevelRecord*)CLevelRecordSet::I()->GetRawDataInfo(nTypeID);
}

#endif // LEVELRECORD_H
