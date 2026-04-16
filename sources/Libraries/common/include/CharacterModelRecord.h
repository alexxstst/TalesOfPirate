#pragma once

#include "TableData.h"
#include <array>
#include <string>

// Запись таблицы моделей персонажей (character_models).
// Одна строка = описание ассетов одного character_type: файл скелета + до 5 частей (скинов).
// Источник: `effect/model.txt` (INI-секции вида `[N] / bone=... / skin1..5=...`).
class CCharacterModelInfo : public CRawDataInfo
{
public:
	short _characterType{};
	std::string _bone;
	std::array<std::string, 5> _skins;
};
