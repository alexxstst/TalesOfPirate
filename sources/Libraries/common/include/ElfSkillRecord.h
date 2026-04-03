#pragma once

#include "TableData.h"

// Запись таблицы навыков эльфов (ElfSkillInfo)
class CElfSkillInfo : public CRawDataInfo {
public:
	int nIndex{};
	int nTypeID{};
};
