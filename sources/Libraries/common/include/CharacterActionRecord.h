#pragma once

#include "TableData.h"
#include <vector>

// Запись таблицы действий персонажа.
// Одна строка = одно действие (ActionNo) внутри типа персонажа (CharacterType).
// _keyFrames — массив ключевых кадров (может быть пустым).
class CCharacterActionInfo : public CRawDataInfo
{
public:
	short _characterType{};
	short _actionNo{};
	short _startFrame{};
	short _endFrame{};
	std::vector<short> _keyFrames;
};
