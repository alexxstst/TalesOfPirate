#pragma once

#include "TableData.h"

// Запись таблицы поз персонажа
class CPoseInfo : public EntityData
{
public:
	short sRealPoseID[7]{};
};
