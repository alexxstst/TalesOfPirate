#pragma once

#include "TableData.h"

// Запись таблицы карт
class CMapInfo : public CRawDataInfo
{
public:
	char  szName[16]{};
	int   nInitX{0};
	int   nInitY{0};
	float fLightDir[3]{1.0f, 1.0f, -1.0f};
	BYTE  btLightColor[3]{255, 255, 255};
	bool  IsShowSwitch{true};
};
