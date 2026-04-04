#pragma once

#include "Database.h"
#include <ctime>

// Trade Log — запись торговых операций в БД через новый ODBC API.
class CTradeLogDB {
public:
	CTradeLogDB() = default;

	BOOL Init();

	void ExecLogSQL(const char* gameServerName, const char* action,
					const char* pszChaFrom, const char* pszChaTo, const char* pszTrade);

	BOOL m_bInitOK{FALSE};

private:
	OdbcDatabase _db;
};

extern CTradeLogDB tradeLog_db;
