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

//
enum EEventArouseType
{
	enumEVENT_AROUSE_DISTANCE,		//
	enumEVENT_AROUSE_CLICK,			//
};

const char cchEventRecordKeyValue = (char)0xff;

#define defEVENT_NAME_LEN	18

class CEventRecord : public EntityData
{
public:
	long	lID;						//
	char	szName[defEVENT_NAME_LEN];	//
    short   sEventType;                 //
	short	sArouseType;				//
	short	sArouseRadius;				//
	short	sEffect;					//
	short	sMusic;						//
	short   sBornEffect;				//
	short	sCursor;					//
	char	chMainChaType;				//

	bool	IsValid( int MainChaType )	{ return chMainChaType==0 || MainChaType==chMainChaType;	}
};

#endif // EVENTRECORD_H
