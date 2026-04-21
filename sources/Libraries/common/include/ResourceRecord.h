#pragma once

#include "TableData.h"

// Запись таблицы ресурсов (ResourceInfo)
class CResourceInfo : public EntityData {
public:
	enum {
		RT_PAR     = 0,
		RT_PATH    = 1,
		RT_EFF     = 2,
		RT_MESH    = 3,
		RT_TEXTURE = 4,
		RT_UNKNOWN = -1,
	};

	int m_iType{RT_UNKNOWN};

	int GetType() const { return m_iType; }
};
