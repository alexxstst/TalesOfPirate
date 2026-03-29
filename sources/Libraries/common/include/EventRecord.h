//=============================================================================
// FileName: EventRecord.h
// Creater: ZhangXuedong
// Date: 2004.11.24
// Comment:
//=============================================================================

#ifndef EVENTRECORD_H
#define EVENTRECORD_H

#include <tchar.h>
#include "util.h"
#include "TableData.h"
#include "point.h"

// 
enum EEventTouchType
{
	enumEVENTT_RANGE	= 1,		// 
};

enum EEventExecType
{
	enumEVENTE_SMAP_ENTRY	= 1,	// 
	enumEVENTE_DMAP_ENTRY	= 2,	// 
};

enum EEventType
{
	enumEVENT_TYPE_ACTION = 1,	// 
	enumEVENT_TYPE_ENTITY,
};

// ,
enum EEventArouseType
{
	enumEVENT_AROUSE_DISTANCE,		// 
	enumEVENT_AROUSE_CLICK,			// 
};

const char cchEventRecordKeyValue = (char)0xff;

#define defEVENT_NAME_LEN	18

class CEventRecord : public CRawDataInfo
{
public:
	//CEventRecord();

	long	lID;						// 
	char	szName[defEVENT_NAME_LEN];	// 
    short   sEventType;                 // ,,EEventType
	short	sArouseType;				// 
	short	sArouseRadius;				// 
	short	sEffect;					// 
	short	sMusic;						// 
	short   sBornEffect;				// 
	short	sCursor;					// 
	char	chMainChaType;				// (0-,1-2-)

	bool	IsValid( int MainChaType )	{ return chMainChaType==0 || MainChaType==chMainChaType;	}
};

class CEventRecordSet : public CRawDataSet
{
public:

	static CEventRecordSet* I() { return _Instance; }

	CEventRecordSet(int nIDStart, int nIDCnt, int nCol = 128)
		:CRawDataSet(nIDStart, nIDCnt, nCol)
	{
		_Instance = this;
		_Init();
	}

protected:

	static CEventRecordSet* _Instance; // , 

	virtual CRawDataInfo* _CreateRawDataArray(int nCnt)
	{
		return new CEventRecord[nCnt];
	}

	virtual void _DeleteRawDataArray()
	{
		delete[] (CEventRecord*)_RawDataArray;
	}

	virtual int _GetRawDataInfoSize()
	{
		return sizeof(CEventRecord);
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

inline CEventRecord* GetEventRecordInfo( int nTypeID )
{
	return (CEventRecord*)CEventRecordSet::I()->GetRawDataInfo(nTypeID);
}

#endif // EVENTRECORD_H
