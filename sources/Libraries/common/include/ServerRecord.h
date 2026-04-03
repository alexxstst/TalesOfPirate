#pragma once

#include "TableData.h"
#include <string>
#include <vector>

#define MAX_GROUP_GATE   5
#define MAX_REGION_GROUP 20
#define MAX_REGION       20

// Запись таблицы серверов (группа шлюзов)
class CServerGroupInfo : public CRawDataInfo {
public:
	std::string region{};
	std::vector<std::string> gateIPs{};
};
