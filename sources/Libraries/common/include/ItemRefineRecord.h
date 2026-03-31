#pragma once
#include "TableData.h"

#define ITEM_REFINE_NUM 14

class CItemRefineInfo : public CRawDataInfo {
public:
	short Value[ITEM_REFINE_NUM]{};
	float fChaEffectScale[4]{};
};
