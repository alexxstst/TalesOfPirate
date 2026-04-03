#pragma once

#include "TableData.h"

// Запись таблицы подсказок (helpinfoset)
class CHelpInfo : public CRawDataInfo {
public:
	char m_strHelpDetail[2048]{};
};
