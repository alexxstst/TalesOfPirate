#pragma once

#include "TableData.h"

// Запись таблицы создания персонажа
class CChaCreateInfo : public EntityData
{
public:
	DWORD type{0};
	DWORD bone{0};
	DWORD hair[64]{};
	DWORD face[64]{};
	DWORD body[64]{};
	DWORD hand[64]{};
	DWORD foot[64]{};
	DWORD hair_num{0};
	DWORD face_num{0};
	DWORD body_num{0};
	DWORD hand_num{0};
	DWORD foot_num{0};
	DWORD profession{0};
	char description[1024]{};
};
