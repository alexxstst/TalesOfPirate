#pragma once
#include "TableData.h"

class EFF_Param : public EntityData {
public:
	char szName[32]{};
	int  nModelNum{0};
	char strModel[8][24]{};
	int  nVel{0};
	int  nParNum{0};
	char strPart[8][24]{};
	int  nDummy[8]{-1,-1,-1,-1,-1,-1,-1,-1};
	int  nRenderIdx{-1};
	int  nLightID{-1};
	char strResult[24]{};
};
