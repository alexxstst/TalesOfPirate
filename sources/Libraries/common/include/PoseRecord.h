#pragma once

#include "TableData.h"

// Запись таблицы поз персонажа
class CPoseInfo : public CRawDataInfo
{
public:
	short sRealPoseID[7]{};
};
