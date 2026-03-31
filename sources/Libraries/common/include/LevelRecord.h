#pragma once

#include <tchar.h>
#include "util.h"
#include "TableData.h"

class CLevelRecord : public CRawDataInfo {
public:
	long lID; //
	short sLevel; //
	unsigned int ulExp; //
};
