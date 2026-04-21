#pragma once

#include "TableData.h"

// Запись таблицы навыков эльфов (ElfSkillInfo)
class CElfSkillInfo : public EntityData {
public:
	int nIndex{};
	int nTypeID{};
};
