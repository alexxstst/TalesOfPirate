#pragma once

#include "TableData.h"
#include <windows.h>

#define MONSTER_LIST_MAX 8

// Запись таблицы зон монстров (MonsterInfo)
class CMonsterInfo : public CRawDataInfo {
public:
	char  szName[32]{};
	POINT ptStart{};
	POINT ptEnd{};
	int   nMonsterList[MONSTER_LIST_MAX]{};
	char  szArea[32]{};
};
