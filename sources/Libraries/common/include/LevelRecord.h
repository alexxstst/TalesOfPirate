#pragma once

#include <tchar.h>
#include "util.h"
#include "TableData.h"

class CLevelRecord : public EntityData {
public:
	long lID; //
	short sLevel; //
	unsigned int ulExp; //
};
