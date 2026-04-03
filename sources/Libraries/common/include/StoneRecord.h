#pragma once

#include "TableData.h"

#define STONE_EQUIP_MAX 3

// Запись таблицы камней (StoneInfo)
class CStoneInfo : public CRawDataInfo {
public:
	int   nItemID{};
	int   nEquipPos[STONE_EQUIP_MAX]{};
	int   nType{};
	char  szHintFunc[64]{};
	DWORD nItemRgb{};

	bool IsEquip(int pos) {
		if (pos <= 1 || pos >= 3) return false;
		for (int i = 0; i < STONE_EQUIP_MAX; i++)
			if (nEquipPos[i] == pos)
				return true;
		return false;
	}
};
