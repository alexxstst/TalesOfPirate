#pragma once

#include <tchar.h>
#include "util.h"
#include "TableData.h"

class CLifeLvRecord : public CRawDataInfo
{
public:
	long	lID;			//
	short	sLevel;			//
	unsigned long	ulExp;	//
};
