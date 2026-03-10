#include "stdafx.h"
#include "TradeLogDB.h"
#include "Config.h"

BOOL CTradeLogDB::Init()
{
	m_bInitOK = FALSE;

	// if(g_Config.m_bTradeLogIsConfig)
	// {
	// 	const char* buf = g_Config.m_szTradeLogDBPass;
	// 	if(strcmp(buf,"\"\"") == 0 || strcmp(buf,"''") == 0 || strcmp(buf,"22222") == 0)
	// 	{
	// 		return FALSE;
	// 	}
	//
	// 	_connect.enable_errinfo();
	//
	//
	//
	// 	std::string err_info;
	// 	//if(!pswd.c_str() || pswd.length() == 0)
	// 	//{
	// 	//	LG("gamedb", "Database  Password Error!");
	// 	//	return FALSE;
	// 	//}
	// 	bool r = _connect.connect(g_Config.m_dsn, err_info);
	// 	if(!r)
	// 	{
	// 		() );
	// 		return FALSE;
	// 	}
	//
	//
	// 	_tab_log   = new CTradeTableLog(&_connect);
	//
	// 	if (!_tab_log )
	// 		return FALSE;
	// }

	m_bInitOK = TRUE;

	return TRUE;
}


CTradeLogDB tradeLog_db;
