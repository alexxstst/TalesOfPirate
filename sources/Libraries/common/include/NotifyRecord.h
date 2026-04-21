#pragma once
#include "TableData.h"

class CNotifyInfo : public EntityData {
public:
	char chType{0};
	char szInfo[64]{};
};
