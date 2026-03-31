#pragma once

#include "TableData.h"

#define MAX_GROUP_GATE   5
#define MAX_REGION_GROUP 20
#define MAX_REGION       20

// Запись таблицы серверов (группа шлюзов)
class CServerGroupInfo : public CRawDataInfo
{
public:
	char  szGateIP[MAX_GROUP_GATE][16]{};
	char  szRegion[16]{};
	char  cValidGateCnt{0};
};
