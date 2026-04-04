#include "stdafx.h"
#include "TradeLogDB.h"
#include "Config.h"

BOOL CTradeLogDB::Init()
{
	m_bInitOK = FALSE;

	if (g_Config.m_bTradeLogIsConfig)
	{
		const char* buf = g_Config.m_szTradeLogDBPass;
		if (strcmp(buf, "\"\"") == 0 || strcmp(buf, "''") == 0 || strcmp(buf, "22222") == 0)
		{
			ToLogService("common", "Database Password Error!");
			return FALSE;
		}

		try {
			static const char* s_szDsn = "DRIVER={ODBC Driver 17 for SQL Server};SERVER=localhost;DATABASE=gamedb;Trusted_Connection=Yes;";
			ToLogService("common", "TradeLogDB: connecting [{}]...", s_szDsn);
			_db.Open(s_szDsn);
			ToLogService("common", "TradeLogDB: connected");
		}
		catch (const OdbcException& e) {
			ToLogService("common", LogLevel::Error, "TradeLogDB connect failed: {}", e.what());
			return FALSE;
		}
	}

	m_bInitOK = TRUE;
	return TRUE;
}

void CTradeLogDB::ExecLogSQL(const char* gameServerName, const char* action,
							 const char* pszChaFrom, const char* pszChaTo, const char* pszTrade)
{
	time_t ltime;
	time(&ltime);
	tm* ttm = localtime(&ltime);

	char timeBuf[20];
	sprintf(timeBuf, "%04i/%02i/%02i %02i:%02i:%02i",
		ttm->tm_year + 1900, ttm->tm_mon + 1, ttm->tm_mday,
		ttm->tm_hour, ttm->tm_min, ttm->tm_sec);

	try {
		auto cmd = _db.CreateCommand(
			"INSERT INTO Trade_Log (ExecuteTime, GameServer, [Action], [From], [To], Memo) "
			"VALUES (@time, @server, @action, @from, @to, @memo)");
		cmd.SetParam("@time", std::string_view(timeBuf));
		cmd.SetParam("@server", std::string_view(gameServerName));
		cmd.SetParam("@action", std::string_view(action));
		cmd.SetParam("@from", std::string_view(pszChaFrom));
		cmd.SetParam("@to", std::string_view(pszChaTo));
		cmd.SetParam("@memo", std::string_view(pszTrade));
		cmd.ExecuteNonQuery();
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "TradeLogDB insert failed: {}", e.what());
	}
}

CTradeLogDB tradeLog_db;
