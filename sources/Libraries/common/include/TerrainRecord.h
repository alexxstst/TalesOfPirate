#pragma once
#include "TableData.h"

#define TERRAIN_TYPE_NORMAL     0
#define TERRAIN_TYPE_UNDERWATER 1
#define MAX_WATER_LOOP          30

class MPTerrainInfo : public CRawDataInfo {
public:
	BYTE btType{TERRAIN_TYPE_NORMAL};
	int  nTextureID{0};
	BYTE btAttr{0};
};
