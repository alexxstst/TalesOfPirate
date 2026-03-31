#pragma once

#include <tchar.h>
#include "util.h"
#include "TableData.h"

class CSailLvRecord : public CRawDataInfo
{
public:
	long	lID;			//
	short	sLevel;			//
	unsigned long	ulExp;	//
};
