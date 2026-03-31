#pragma once

#include "TableData.h"

// Запись таблицы шейдовых эффектов
class CShadeInfo : public CRawDataInfo
{
public:
	char    szName[16]{};
	int     nPhotoTexID{0};

	float   fsize{0};
	int     nAni{0};
	int     nRow{0};
	int     nCol{0};

	int     nUseAlphaTest{0};
	int     nAlphaType{0};

	int     nColorR{0};
	int     nColorG{0};
	int     nColorB{0};
	int     nColorA{0};

	int     nType{0};
};
