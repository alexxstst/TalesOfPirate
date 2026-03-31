#pragma once
#include "TableData.h"

class CNotifyInfo : public CRawDataInfo {
public:
	char chType{0};
	char szInfo[64]{};
};
