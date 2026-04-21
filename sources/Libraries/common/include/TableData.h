#pragma once

// EntityData — общая DTO-база для всех Record-типов (CNpcRecord,
// CMonRefRecord, CSwitchMapRecord, CItemRecord, ...).
// После сноса CRawDataSet остались только поля, читаемые потребителями:
//   nID       — числовой ID записи
//   DataName  — строковое имя записи (std::string)

#include <string>

class EntityData {
public:
	int Id{0};
	std::string DataName{};
};
