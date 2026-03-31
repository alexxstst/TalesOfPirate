#pragma once
#include "TableData.h"

#define REFINE_EFFECT_NUM     4
#define REFINE_EFFECT_CHA_NUM 4

class CItemRefineEffectInfo : public CRawDataInfo {
	friend class CItemRefineEffectSet;
	friend class ItemRefineEffectRecordStore;
public:
	int   nLightID{0};
	short sEffectID[REFINE_EFFECT_CHA_NUM][REFINE_EFFECT_NUM]{};
	char  chDummy[REFINE_EFFECT_NUM]{};

	int GetEffectNum(int nCharID) { return _sEffectNum[nCharID]; }

private:
	int _sEffectNum[REFINE_EFFECT_CHA_NUM]{};
};
