#pragma once

#include "TableData.h"
#define NOMINMAX
#include <windows.h>
#include <array>

// Запись таблицы маунтов (mountinfo)
class CMountInfo : public EntityData
{
public:
	CMountInfo() = default;

	short mountID{};
	short boneID{};
	std::array<short, 4> height{ /* Lance, Carsise, Phyllis, Ami */};
	short offsetX{};
	short offsetY{};
	std::array<short, 4> poseID{ /* Lance, Carsise, Phyllis, Ami */};
};
