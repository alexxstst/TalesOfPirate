#include "util.h"
#include "db.h"
using namespace std;

#define SQLERR_FORMAT "SQL Error State:%s, Native Error Code: %lX, ODBC Error: %s\n"
#define COLTRUNC_WARNG "Number of columns in display truncated to %u\n"
#define NULLDATASTRING ""

#define SQL_SUC(sr) (((sr == SQL_SUCCESS) || (sr == SQL_SUCCESS_WITH_INFO) || (sr == SQL_NO_DATA_FOUND)) ? true : false)
#define DBFAIL(sr) ((sr != SQL_SUCCESS) && (sr != SQL_SUCCESS_WITH_INFO))

cfl_db::cfl_db() : _henv(SQL_NULL_HENV), _hdbc(SQL_NULL_HDBC), _rslist() {
	_connected = false;
	_dump_errinfo = false;
	_connstr = "";

	_rslist.clear();
}

cfl_db::~cfl_db() {
	if (_connected) {
		disconn();
		_rslist.clear();
		_connected = false;
	}
}

void cfl_db::add(cfl_rs* rs) {
	if (rs == nullptr) return;

	vector<cfl_rs*>::iterator it = _rslist.begin();
	bool found = false;
	while (it != _rslist.end()) {
		if (rs == *(it)) {
			found = true;
			break;
		}
		++it;
	}

	if (!found) _rslist.push_back(rs);
}

bool cfl_db::handle_err(SQLHANDLE h, SQLSMALLINT t,
						RETCODE r, const std::string& sql /* = NULL */,
						bool reconn /* = false */) {
	//if (!_connected) return;

#define DBLOG printf
	SQLRETURN sqlret;
	SQLCHAR state[SQL_SQLSTATE_SIZE + 1] = {0};
	SQLINTEGER error;
	SQLCHAR msg[SQL_MAX_MESSAGE_LENGTH + 1] = {0};
	SQLSMALLINT msg_len;

	sqlret = SQLGetDiagRec(t, h, 1, state, &error, msg, SQL_MAX_MESSAGE_LENGTH, &msg_len);
	if (sqlret == SQL_ERROR || sqlret == SQL_INVALID_HANDLE) return false;

	if (_dump_errinfo) {
		//DBLOG(SQLERR_FORMAT, state, error, msg);

		ToLogService("util_db_error", "SQL Error State:{}, Native Error Code: {:X}, ODBC Error: {}", (const char*)state, error, (const char*)msg);

		if (!sql.empty()) {
			ToLogService("util_db", "[STMT:0x{:X}][SQLERR]: [{}]", (uintptr_t)h, sql);

			ToLogService("util_db_error", "[STMT:0x{:X}][SQLERR]: [{}]", (uintptr_t)h, sql);
		}
	}

	// Reconnect...
	if (reconn && _needreconn((char const*)state)) {
		return _reconnt();
	}
	return false;
}

bool cfl_db::_needreconn(char const* state) {
	bool ret = false;

#if 1
	if (strcmp(state, "HYT00") == 0) ret = true;
	else if (strcmp(state, "HYT01") == 0) ret = true;
	else if (strcmp(state, "01000") == 0) ret = true;
	else if (strcmp(state, "08S01") == 0) ret = true;
#endif

	return ret;
}

bool cfl_db::_reconnt() {
	string err_info;
	cfl_rs* tmp = nullptr;

	vector<cfl_rs*>::iterator it = _rslist.begin();
	while (it != _rslist.end()) {
		tmp = (cfl_rs*)*(it);
		ToLogService("util_db", "notify {} the disconn message", (void*)tmp);

		tmp->notify_discon(this);

		++it;
	}

	disconn();

	ToLogService("util_db_error", "ready connect database...");
	while (!_connect(err_info)) {
		Sleep(1000);
		ToLogService("util_db_error", "reconnect database...");
	}

	ToLogService("util_db_error", "reconnect database success!");

	it = _rslist.begin();
	while (it != _rslist.end()) {
		tmp = (cfl_rs*)*(it);
		tmp->notify_reconn(this);

		++it;
	}

	ToLogService("util_db_error", "reconnect database ok!");
	return true;
}

bool cfl_db::_connect(string& errinfo) {
	bool ret = true;
	SQLRETURN sqlret;
	char outbuf[2048];
	SQLSMALLINT outlen = 0;

	do {
		sqlret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &_henv);
		if (sqlret == SQL_ERROR) {
			errinfo = "Unable to allocate an environment handle\n";
			ret = false;
			break;
		}

		// Let ODBC know this is an ODBC 3.0 application
		sqlret = SQLSetEnvAttr(_henv, SQL_ATTR_ODBC_VERSION,
							   (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER);
		if (DBFAIL(sqlret)) {
			handle_err(_henv, SQL_HANDLE_ENV, sqlret, "");

			errinfo = "Error in calling SQLSetEnvAttr\n";
			SQLFreeHandle(SQL_HANDLE_ENV, _henv);
			ret = false;
			break;
		}

		sqlret = SQLAllocHandle(SQL_HANDLE_DBC, _henv, &_hdbc);
		if (DBFAIL(sqlret)) {
			handle_err(_henv, SQL_HANDLE_ENV, sqlret, "");

			errinfo = "Unable to allocate an dbc handle\n";
			SQLFreeHandle(SQL_HANDLE_DBC, _hdbc);
			SQLFreeHandle(SQL_HANDLE_ENV, _henv);
			ret = false;
			break;
		}

		sqlret = SQLDriverConnect(_hdbc, nullptr, (SQLCHAR*)_connstr.c_str(),
								  SQL_NTS, (SQLCHAR*)outbuf, sizeof outbuf,
								  &outlen, SQL_DRIVER_NOPROMPT);
		if (DBFAIL(sqlret)) {
			handle_err(_hdbc, SQL_HANDLE_DBC, sqlret, "");

			errinfo = "Unable to connect database\n";
			SQLFreeHandle(SQL_HANDLE_DBC, _hdbc);
			SQLFreeHandle(SQL_HANDLE_ENV, _henv);
			ret = false;
			break;
		}

		//char command[256];

		//sprintf(command, "use %s", DATABASE);
		//if (!SQL_SUCCEEDED(SQLExecDirect(_hdbc, (SQLCHAR *) command, SQL_NTS)))
		//{
		//	handle_err(_hdbc, SQL_HANDLE_DBC, sqlret, "");

		//          errinfo = "Unable to connect database\n";
		//	SQLFreeHandle(SQL_HANDLE_DBC, _hdbc);
		//	SQLFreeHandle(SQL_HANDLE_ENV, _henv);
		//	ret = false;
		//	break;
		//}

		//handle_err(_hdbc, SQL_HANDLE_DBC, sqlret, "");

		sqlret = SQLSetConnectAttr(_hdbc, SQL_ATTR_CONNECTION_TIMEOUT,
								   (void*)10, 0);

		if (DBFAIL(sqlret)) {
			handle_err(_hdbc, SQL_HANDLE_DBC, sqlret, "");

			errinfo = "Unable set timeout to database\n";
			SQLFreeHandle(SQL_HANDLE_DBC, _hdbc);
			SQLFreeHandle(SQL_HANDLE_ENV, _henv);
			ret = false;
			break;
		}

		//exec_sql_direct(_openDatabase.c_str());

		// Set connect flag...
		_connected = true;
	}
	while (0);

	return ret;
}

bool cfl_db::connect(const char* source, const char* userid, const char* passwd, string& err_info) {
	if (_connected) {
		err_info = "Already connected\n";
		return false;
	}

	bool ret = true;
	SQLRETURN sqlret;

	do {
		sqlret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &_henv);
		if (sqlret == SQL_ERROR) {
			err_info = "Unable to allocate an environment handle\n";
			ret = false;
			break;
		}

		// Let ODBC know this is an ODBC 3.0 application
		sqlret = SQLSetEnvAttr(_henv, SQL_ATTR_ODBC_VERSION,
							   (SQLPOINTER)SQL_OV_ODBC3,
							   SQL_IS_INTEGER);
		if (DBFAIL(sqlret)) {
			handle_err(_henv, SQL_HANDLE_ENV, sqlret, "");

			err_info = "Error in calling SQLSetEnvAttr\n";
			SQLFreeHandle(SQL_HANDLE_ENV, _henv);
			ret = false;
			break;
		}

		sqlret = SQLAllocHandle(SQL_HANDLE_DBC, _henv, &_hdbc);
		if (DBFAIL(sqlret)) {
			handle_err(_henv, SQL_HANDLE_ENV, sqlret, "");

			err_info = "Unable to allocate an dbc handle\n";
			SQLFreeHandle(SQL_HANDLE_DBC, _hdbc);
			SQLFreeHandle(SQL_HANDLE_ENV, _henv);
			ret = false;
			break;
		}

		sqlret = SQLConnect(_hdbc, (SQLCHAR*)source, SQL_NTS,
							(SQLCHAR*)userid, SQL_NTS,
							(SQLCHAR*)passwd, SQL_NTS);
		if (DBFAIL(sqlret)) {
			handle_err(_hdbc, SQL_HANDLE_DBC, sqlret, "");

			err_info = "Unable to connect database\n";
			SQLFreeHandle(SQL_HANDLE_DBC, _hdbc);
			SQLFreeHandle(SQL_HANDLE_ENV, _henv);
			ret = false;
			break;
		}

		handle_err(_hdbc, SQL_HANDLE_DBC, sqlret, "");

		// Set connect flag...
		_connected = true;
	}
	while (0);

	return ret;
}

bool cfl_db::connect(const std::string& dsn, string& err_info) {
	if (_connected) {
		err_info = "Already connected\n";
		return false;
	}

	_connstr = dsn;
	_openDatabase = string("use ") + string(dsn) + string(";");
	// End

	return _connect(err_info);
}

void cfl_db::disconn() {
	if (_connected) {
		SQLDisconnect(_hdbc);

		SQLFreeHandle(SQL_HANDLE_DBC, _hdbc);
		_hdbc = SQL_NULL_HDBC;

		SQLFreeHandle(SQL_HANDLE_ENV, _henv);
		_henv = SQL_NULL_HENV;

		_connected = false;
	}
}

SQLRETURN cfl_db::exec_sql_direct(const std::string& sql, unsigned short timeout /* = 5 */) {
	if (!_connected) return SQL_ERROR;

RECONNECT:

	SQLRETURN sqlret;
	SQLHSTMT hstmt = SQL_NULL_HSTMT;

	do {
		sqlret = SQLAllocHandle(SQL_HANDLE_STMT, _hdbc, &hstmt);
		if (DBFAIL(sqlret)) {
			handle_err(_hdbc, SQL_HANDLE_DBC, sqlret, sql);
			break;
		}

		sqlret = SQLSetStmtAttr(_hdbc, SQL_QUERY_TIMEOUT, &timeout, 0);
		if (DBFAIL(sqlret)) {
			handle_err(_hdbc, SQL_HANDLE_DBC, sqlret, sql);
			break;
		}

		sqlret = SQLExecDirect(hstmt, (SQLCHAR*)sql.c_str(), SQL_NTS);
		switch (sqlret) {
		case SQL_SUCCESS_WITH_INFO:
		//handle_err(hstmt, SQL_HANDLE_STMT, sqlret, sql);
		// fall through
		case SQL_SUCCESS:
			break;

		case SQL_ERROR: {
			if (handle_err(hstmt, SQL_HANDLE_STMT, sqlret, sql, true)) {
				goto RECONNECT;
			}
		}
			return 0;

		default:
			continue;
		}
	}
	while (false);

	if (hstmt != SQL_NULL_HSTMT) SQLFreeHandle(SQL_HANDLE_STMT, hstmt);

	return sqlret;
}

bool cfl_db::begin_tran() {
#if 1
	if (!_connected) return false;

	SQLRETURN sqlret = SQLSetConnectAttr(_hdbc, SQL_ATTR_AUTOCOMMIT,
										 (SQLPOINTER)SQL_AUTOCOMMIT_OFF,
										 SQL_IS_POINTER);
	return (DBFAIL(sqlret)) ? false : true;
#endif

	return true;
}

bool cfl_db::commit_tran() {
#if 1
	if (!_connected) return false;

	SQLEndTran(SQL_HANDLE_DBC, _hdbc, SQL_COMMIT);

	SQLRETURN sqlret = SQLSetConnectAttr(_hdbc, SQL_ATTR_AUTOCOMMIT,
										 (SQLPOINTER)SQL_AUTOCOMMIT_ON,
										 SQL_IS_POINTER);
	return (DBFAIL(sqlret)) ? false : true;
#endif

	return true;
}

bool cfl_db::rollback() {
#if 1
	if (!_connected) return false;

	SQLEndTran(SQL_HANDLE_DBC, _hdbc, SQL_ROLLBACK);

	SQLRETURN sqlret = SQLSetConnectAttr(_hdbc, SQL_ATTR_AUTOCOMMIT,
										 (SQLPOINTER)SQL_AUTOCOMMIT_ON,
										 SQL_IS_POINTER);
	return (DBFAIL(sqlret)) ? false : true;
#endif

	return true;
}


cfl_rs::cfl_rs(cfl_db* db) : _db(db), _hdbc(SQL_NULL_HDBC), _hstmt(SQL_NULL_HSTMT) {
	attach_db(db);
	db->add(this);
}

cfl_rs::cfl_rs(cfl_db* db, char const* tbl_name, int max_col)
	: _db(db), _hdbc(SQL_NULL_HDBC), _hstmt(SQL_NULL_HSTMT), _tbl_name(tbl_name) {
	attach_db(db);
	db->add(this);

	_max_col = max_col;
	_col_num = 0;
	_row_num = 0;
	_param_num = 0;
}

cfl_rs::~cfl_rs() {
	if (_hstmt != SQL_NULL_HSTMT) {
		SQLFreeHandle(SQL_HANDLE_STMT, _hstmt);
		_hstmt = SQL_NULL_HSTMT;
	}

	_hdbc = SQL_NULL_HDBC;
	_db = nullptr;
}

void cfl_rs::attach_db(cfl_db* db) {
	if (db != _db) return;

	_hdbc = db->get_dbc();
	SQLAllocHandle(SQL_HANDLE_STMT, _hdbc, &_hstmt);
}

void cfl_rs::notify_discon(cfl_db* db) {
	if (_db != db) return;

#if 1
	if (_hstmt != SQL_NULL_HSTMT) {
		SQLFreeHandle(SQL_HANDLE_STMT, _hstmt);
		_hstmt = SQL_NULL_HSTMT;
	}
#endif
}

void cfl_rs::notify_reconn(cfl_db* db) {
	attach_db(db);
}

int cfl_rs::get_affected_rows() {
	string l_buf[1];
	int l_affected_rows = 0;
	const char* param = " @@ROWCOUNT ";
	string l_tbl_name = _tbl_name;
	_tbl_name = "";
	bool l_ret = _get_row(l_buf, 1, param, nullptr, &l_affected_rows);
	_tbl_name = l_tbl_name;
	if (!l_ret) {
		return -1; //SQL
	}
	else if (l_affected_rows != 1) {
		return -2; //
	}
	return atoi(l_buf[0].c_str());
}

int cfl_rs::get_identity() {
	string l_buf[1];
	int l_affected_rows = 0;
	const char* param = " ISNULL(@@IDENTITY,0) ";
	string l_tbl_name = _tbl_name;
	_tbl_name = "";
	bool l_ret = _get_row(l_buf, 1, param, nullptr, &l_affected_rows);
	_tbl_name = l_tbl_name;
	if (!l_ret) {
		return -1; //SQL
	}
	else if (l_affected_rows != 1) {
		return -2; //
	}
	return atoi(l_buf[0].c_str());
}

// Add bynlark.li 20080808 begin
bool cfl_rs::_get_bin_field(char* field_text, int& len, char* param, char* filter, int* affect_rows) {
	bool ret = false;
	std::string sql;
	int i = 0;

	if (param == nullptr) {
		sql = std::format("select * from {}", _tbl_name);
	}
	else {
		if (_tbl_name.length() != 0) {
			sql = std::format("select {} from {}", param, _tbl_name);
		}
		else {
			sql = std::format("select {}", param);
		}
	}

	if ((strstr(param, "@@ROWCOUNT") == nullptr) &&
		(strstr(param, "@@rowcount") == nullptr) &&
		(strstr(_tbl_name.c_str(), "(nolock)") == nullptr) &&
		(strstr(_tbl_name.c_str(), "(NOLOCK)") == nullptr)) {
		sql += " (nolock) ";
	}

	if (filter == nullptr) {
	}
	else {
		sql += " where ";
		sql += filter;
	}

RECONNECT:

	//
	SQLRETURN sqlret;
	SQLHSTMT hstmt = SQL_NULL_HSTMT;
	SQLSMALLINT col_num = 0;
	bool found = true;

	do {
		sqlret = SQLAllocHandle(SQL_HANDLE_STMT, _hdbc, &hstmt);
		if (DBFAIL(sqlret)) {
			handle_err(_hdbc, SQL_HANDLE_DBC, sqlret, "");
			break;
		}

		std::uint16_t timeout = 50;
		sqlret = SQLSetStmtAttr(hstmt, SQL_QUERY_TIMEOUT, &timeout, 0);
		if (DBFAIL(sqlret)) {
			handle_err(_hdbc, SQL_HANDLE_DBC, sqlret, "");
			break;
		}

		sqlret = SQLExecDirect(hstmt, (SQLCHAR*)sql.c_str(), SQL_NTS);
		if (DBFAIL(sqlret)) {
			if (handle_err(hstmt, SQL_HANDLE_STMT, sqlret, sql, true)) {
				goto RECONNECT;
			}
			return false;
		}

		sqlret = SQLNumResultCols(hstmt, &col_num);

		col_num = min(col_num, 1);

		if (col_num <= 0) {
			ToLogService("util_db", "col_num = {} (<=0)", col_num);
			break;
		}

		SQLBindCol(hstmt, UWORD(1), SQL_C_BINARY, _buf[0], len, &_buf_len[0]);

		sqlret = SQLFetch(hstmt); // only fetch the next row
		if (DBNODATA(sqlret)) {
			ToLogService("util_db", "SQL didn't fetch any data [{}]", sql);
			found = false;
		}
		else if (sqlret != SQL_SUCCESS) {
			handle_err(hstmt, SQL_HANDLE_STMT, sqlret, sql);
			if (sqlret != SQL_SUCCESS_WITH_INFO) {
				break;
			}
		}
		//
		if (found) {
			//
			len = _buf_len[0];
			if (len == SQL_NULL_DATA) {
				field_text[0] = 0x0;
			}
			else {
				memcpy(field_text, _buf[0], len);
			}

			if (affect_rows != nullptr)
				*affect_rows = 1;
		}
		else {
			//
			if (affect_rows != nullptr)
				*affect_rows = 0;
		}

		ret = true;
	}
	while (false);

	if (hstmt != SQL_NULL_HSTMT) {
		SQLFreeStmt(hstmt, SQL_CLOSE);
		SQLFreeStmt(hstmt, SQL_RESET_PARAMS);
		SQLFreeStmt(hstmt, SQL_UNBIND);
		SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
	}

	return ret;
}

// End

bool cfl_rs::_get_row(string field_text[], int field_max_cnt, const char* param,
					  const char* filter, int* affect_rows /* = NULL */) {
	bool ret = false;
	std::string sql;
	int i = 0;

	if (param == nullptr) {
		sql = std::format("select * from {}", _tbl_name);
	}
	else {
		if (_tbl_name.length() != 0) {
			sql = std::format("select {} from {}", param, _tbl_name);
		}
		else {
			sql = std::format("select {}", param);
		}
	}

	if ((strstr(param, "@@ROWCOUNT") == nullptr) &&
		(strstr(param, "@@rowcount") == nullptr) &&
		(strstr(_tbl_name.c_str(), "(nolock)") == nullptr) &&
		(strstr(_tbl_name.c_str(), "(NOLOCK)") == nullptr)) {
		sql += " (nolock) ";
	}

	if (filter == nullptr) {
	}
	else {
		sql += " where ";
		sql += filter;
	}


	ToLogService("util_db", "_get_row [SQL]: [{}]", sql);

RECONNECT:

	//
	SQLRETURN sqlret;
	SQLHSTMT hstmt = SQL_NULL_HSTMT;
	SQLSMALLINT col_num = 0;
	bool found = true;

	do {
		sqlret = SQLAllocHandle(SQL_HANDLE_STMT, _hdbc, &hstmt);
		if (DBFAIL(sqlret)) {
			handle_err(_hdbc, SQL_HANDLE_DBC, sqlret, "");
			break;
		}


		std::uint16_t timeout = 50;
		if (DBFAIL(sqlret)) {
			handle_err(_hdbc, SQL_HANDLE_DBC, sqlret, "");
			break;
		}

		sqlret = SQLExecDirect(hstmt, (SQLCHAR*)sql.c_str(), SQL_NTS);
		if (DBFAIL(sqlret)) {
			if (handle_err(hstmt, SQL_HANDLE_STMT, sqlret, sql, true)) {
				goto RECONNECT;
			}
			return false;
		}

		sqlret = SQLNumResultCols(hstmt, &col_num);

		col_num = min(col_num, field_max_cnt);
		col_num = min(col_num, MAX_COL);
		col_num = min(col_num, _max_col);

		if (col_num <= 0) {

				ToLogService("util_db", "col_num = 0 (<=0)");
			break;
		}

		for (i = 0; i < col_num; ++i) {
			SQLBindCol(hstmt, UWORD(i + 1), SQL_C_CHAR, _buf[i],
					   MAX_DATALEN, &_buf_len[i]);
		}


		sqlret = SQLFetch(hstmt); // only fetch the next row
		if (DBNODATA(sqlret)) {

				ToLogService("util_db", "SQL didn't fetch any data [{}]", sql);
			found = false;
		}
		else if (sqlret != SQL_SUCCESS) {
			handle_err(hstmt, SQL_HANDLE_STMT, sqlret, sql);
			if (sqlret != SQL_SUCCESS_WITH_INFO) {
				break;
			}
		}


		//
		if (found) {
			//
			for (i = 0; i < col_num; ++i) {
				if (_buf_len[i] == SQL_NULL_DATA) {
					field_text[i] = NULLDATASTRING;
				}
				else {
					field_text[i] = (char*)(_buf[i]);
				}
			}

			if (affect_rows != nullptr)
				*affect_rows = 1;
		}
		else {
			//
			if (affect_rows != nullptr)
				*affect_rows = 0;
		}


		ret = true;
	}
	while (0);

	if (hstmt != SQL_NULL_HSTMT) {
		SQLFreeStmt(hstmt, SQL_CLOSE);
		SQLFreeStmt(hstmt, SQL_RESET_PARAMS);
		SQLFreeStmt(hstmt, SQL_UNBIND);
		SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
	}

	return ret;
}

bool cfl_rs::_get_row3(string field_text[], int field_max_cnt, const char* param,
					   const char* filter, int* affect_rows /* = NULL */) {
	bool ret = false;
	std::string sql;
	int i = 0;

	if (param == nullptr) {
		sql = std::format("select * from {}", _tbl_name);
	}
	else {
		if (_tbl_name.length() != 0) {
			sql = std::format("select {} from {}", param, _tbl_name);
		}
		else {
			sql = std::format("select {}", param);
		}
	}

	if (filter == nullptr) {
	}
	else {
		sql += " where ";
		sql += filter;
	}

	ToLogService("util_db", "_get_row3 [SQL]: [{}]", sql);

RECONNECT:

	//
	SQLRETURN sqlret;
	SQLHSTMT hstmt = SQL_NULL_HSTMT;
	SQLSMALLINT col_num = 0;
	bool found = true;

	do {
		sqlret = SQLAllocHandle(SQL_HANDLE_STMT, _hdbc, &hstmt);
		if (DBFAIL(sqlret)) {
			handle_err(_hdbc, SQL_HANDLE_DBC, sqlret, "");
			break;
		}


		std::uint16_t timeout = 50;
		sqlret = SQLSetStmtAttr(hstmt, SQL_QUERY_TIMEOUT, &timeout, 0);
		if (DBFAIL(sqlret)) {
			handle_err(_hdbc, SQL_HANDLE_DBC, sqlret, "");
			break;
		}

		sqlret = SQLExecDirect(hstmt, (SQLCHAR*)sql.c_str(), SQL_NTS);
		if (DBFAIL(sqlret)) {
			if (handle_err(hstmt, SQL_HANDLE_STMT, sqlret, sql, true)) {
				goto RECONNECT;
			}
			return false;
		}


		sqlret = SQLNumResultCols(hstmt, &col_num);

		col_num = min(col_num, field_max_cnt);
		col_num = min(col_num, MAX_COL);
		col_num = min(col_num, _max_col);

		if (col_num <= 0) {

				ToLogService("util_db", "col_num = 0 (<=0)");
			break;
		}

		for (i = 0; i < col_num; ++i) {
			SQLBindCol(hstmt, UWORD(i + 1), SQL_C_CHAR, _buf[i],
					   MAX_DATALEN, &_buf_len[i]);
		}


		sqlret = SQLFetch(hstmt); // only fetch the next row
		if (DBNODATA(sqlret)) {

				ToLogService("util_db", "SQL didn't fetch any data [{}]", sql);
			found = false;
		}
		else if (sqlret != SQL_SUCCESS) {
			handle_err(hstmt, SQL_HANDLE_STMT, sqlret, sql);
			if (sqlret != SQL_SUCCESS_WITH_INFO) {
				break;
			}
		}

		//
		if (found) {
			//
			for (i = 0; i < col_num; ++i) {
				if (_buf_len[i] == SQL_NULL_DATA) {
					field_text[i] = NULLDATASTRING;
				}
				else {
					char* p = strchr((char*)_buf[i], ' ');
					if (p != nullptr) {
						*p = '\0';
					}

					field_text[i] = (char*)(_buf[i]);
				}
			}

			if (affect_rows != nullptr)
				*affect_rows = 1;
		}
		else {
			//
			if (affect_rows != nullptr)
				*affect_rows = 0;
		}


		ret = true;
	}
	while (0);


	if (hstmt != SQL_NULL_HSTMT) {
		SQLFreeStmt(hstmt, SQL_CLOSE);
		SQLFreeStmt(hstmt, SQL_RESET_PARAMS);
		SQLFreeStmt(hstmt, SQL_UNBIND);
		SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
	}


	return ret;
}

bool cfl_rs::_get_rowOderby(string field_text[], int field_max_cnt, const char* param,
							const char* filter, int* affect_rows/* = NULL*/) {
	bool ret = false;
	std::string sql;
	int i = 0;

	if (param == nullptr) {
		sql = std::format("select * from {}", _tbl_name);
	}
	else {
		if (!_tbl_name.empty()) {
			sql = std::format("select {} from {}", param, _tbl_name);
		}
		else {
			sql = std::format("select {}", param);
		}
	}

	if (filter == nullptr) {
	}
	else {
		sql += " ";
		sql += filter;
	}


	ToLogService("util_db", "_get_row3 [SQL]: [{}]", sql);

RECONNECT:

	//
	SQLRETURN sqlret;
	SQLHSTMT hstmt = SQL_NULL_HSTMT;
	SQLSMALLINT col_num = 0;
	bool found = true;

	do {
		sqlret = SQLAllocHandle(SQL_HANDLE_STMT, _hdbc, &hstmt);
		if (DBFAIL(sqlret)) {
			handle_err(_hdbc, SQL_HANDLE_DBC, sqlret, "");
			break;
		}

		std::uint16_t timeout = 50;
		if (DBFAIL(sqlret)) {
			handle_err(_hdbc, SQL_HANDLE_DBC, sqlret, "");
			break;
		}

		sqlret = SQLExecDirect(hstmt, (SQLCHAR*)sql.c_str(), SQL_NTS);
		if (DBFAIL(sqlret)) {
			if (handle_err(hstmt, SQL_HANDLE_STMT, sqlret, sql, true)) {
				goto RECONNECT;
			}
			return false;
		}


		sqlret = SQLNumResultCols(hstmt, &col_num);

		col_num = min(col_num, field_max_cnt);
		col_num = min(col_num, MAX_COL);
		col_num = min(col_num, _max_col);

		if (col_num <= 0) {
			ToLogService("util_db", "col_num = 0 (<=0)");
			break;
		}

		for (i = 0; i < col_num; ++i) {
			SQLBindCol(hstmt, UWORD(i + 1), SQL_C_CHAR, _buf[i],
					   MAX_DATALEN, &_buf_len[i]);
		}


		sqlret = SQLFetch(hstmt); // only fetch the next row
		if (DBNODATA(sqlret)) {
			ToLogService("util_db", "SQL didn't fetch any data [{}]", sql);
			found = false;
		}
		else if (sqlret != SQL_SUCCESS) {
			handle_err(hstmt, SQL_HANDLE_STMT, sqlret, sql);
			if (sqlret != SQL_SUCCESS_WITH_INFO) {
				break;
			}
		}

		//
		if (found) {
			//
			for (i = 0; i < col_num; ++i) {
				if (_buf_len[i] == SQL_NULL_DATA) {
					field_text[i] = NULLDATASTRING;
				}
				else {
					char* p = strchr((char*)_buf[i], ' ');
					if (p != nullptr) {
						*p = '\0';
					}

					field_text[i] = (char*)(_buf[i]);
				}
			}

			if (affect_rows != nullptr)
				*affect_rows = 1;
		}
		else {
			//
			if (affect_rows != nullptr)
				*affect_rows = 0;
		}


		ret = true;
	}
	while (0);


	if (hstmt != SQL_NULL_HSTMT) {
		SQLFreeStmt(hstmt, SQL_CLOSE);
		SQLFreeStmt(hstmt, SQL_RESET_PARAMS);
		SQLFreeStmt(hstmt, SQL_UNBIND);
		SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
	}


	return ret;
}

SQLRETURN cfl_rs::exec_sql_direct(const std::string& sql, unsigned short timeout /* = 5 */) {
RECONNECT:

	SQLRETURN sqlret;
	SQLHSTMT hstmt = SQL_NULL_HSTMT;
	//ToLogService("util_db", "SQL Statement Length: {}", strlen(sql));

	do {
		sqlret = SQLAllocHandle(SQL_HANDLE_STMT, _hdbc, &hstmt);
		if (DBFAIL(sqlret)) {
			handle_err(_hdbc, SQL_HANDLE_DBC, sqlret, "");
			break;
		}
		sqlret = SQLSetStmtAttr(hstmt, SQL_QUERY_TIMEOUT, &timeout, 0);
		if (DBFAIL(sqlret)) {
			handle_err(_hdbc, SQL_HANDLE_DBC, sqlret, "");
			break;
		}

		sqlret = SQLExecDirect(hstmt, (SQLCHAR*)sql.c_str(), SQL_NTS);
		switch (sqlret) {
		case SQL_SUCCESS_WITH_INFO:
		//handle_err(hstmt, SQL_HANDLE_STMT, sqlret, sql);
		// fall through
		case SQL_SUCCESS:
			break;

		case SQL_ERROR: {
			if (handle_err(hstmt, SQL_HANDLE_STMT, sqlret, sql, true)) {
				goto RECONNECT;
			}
		}
		break;

		case SQL_NO_DATA: // update or delete
			break;
		}
	}
	while (0);

	if (hstmt != SQL_NULL_HSTMT) {
		SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
		hstmt = SQL_NULL_HSTMT;
	}

	return sqlret;
}

SQLRETURN cfl_rs::exec_sql(const std::string& sql, char const* pdata, int len, unsigned short timeout /* = 30 */) {
RECONNECT:

	SQLRETURN sqlret;
	SQLHSTMT hstmt = SQL_NULL_HSTMT;
	SQLINTEGER data_len = SQL_NTS;

	do {
		sqlret = SQLAllocHandle(SQL_HANDLE_STMT, _hdbc, &hstmt);
		if (DBFAIL(sqlret)) {
			handle_err(_hdbc, SQL_HANDLE_DBC, sqlret, "");
			break;
		}

		sqlret = SQLSetStmtAttr(hstmt, SQL_QUERY_TIMEOUT, &timeout, 0);
		if (DBFAIL(sqlret)) {
			handle_err(_hdbc, SQL_HANDLE_DBC, sqlret, "");
			break;
		}

		sqlret = SQLPrepare(hstmt, (SQLCHAR*)sql.c_str(), SQL_NTS);
		if (DBFAIL(sqlret)) {
			handle_err(hstmt, SQL_HANDLE_STMT, sqlret, "");
			break;
		}

		sqlret = SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, len - 1, 0,
								  (SQLPOINTER)pdata, len, &data_len);
		if (DBFAIL(sqlret)) {
			handle_err(hstmt, SQL_HANDLE_STMT, sqlret, "");
			break;
		}

		sqlret = SQLExecute(hstmt);
		switch (sqlret) {
		case SQL_SUCCESS_WITH_INFO:
		//handle_err(hstmt, SQL_HANDLE_STMT, sqlret, sql);
		// fall through
		case SQL_SUCCESS:
			break;

		case SQL_ERROR: {
			if (handle_err(hstmt, SQL_HANDLE_STMT, sqlret, sql, true)) {
				goto RECONNECT;
			}
		}
		break;

		case SQL_NO_DATA: // update or delete
			break;

		case SQL_NEED_DATA:
			handle_err(hstmt, SQL_HANDLE_STMT, sqlret, sql);
			break;
		}
	}
	while (0);

	if (hstmt != SQL_NULL_HSTMT) {
		SQLFreeStmt(hstmt, SQL_CLOSE);
		SQLFreeStmt(hstmt, SQL_RESET_PARAMS);
		SQLFreeStmt(hstmt, SQL_UNBIND);
		SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
		hstmt = SQL_NULL_HSTMT;
	}

	return sqlret;
}

// Add by lark.li 20080808 begin
SQLRETURN cfl_rs::exec_param_sql(const std::string& sql, char const* pdata, int len, unsigned short timeout /* = 30 */) {
RECONNECT:

	SQLRETURN sqlret;
	SQLHSTMT hstmt = SQL_NULL_HSTMT;
	SQLINTEGER data_len = len;

	do {
		sqlret = SQLAllocHandle(SQL_HANDLE_STMT, _hdbc, &hstmt);
		if (DBFAIL(sqlret)) {
			handle_err(_hdbc, SQL_HANDLE_DBC, sqlret, "");
			break;
		}

		sqlret = SQLSetStmtAttr(hstmt, SQL_QUERY_TIMEOUT, &timeout, 0);
		if (DBFAIL(sqlret)) {
			handle_err(_hdbc, SQL_HANDLE_DBC, sqlret, "");
			break;
		}

		sqlret = SQLPrepare(hstmt, (SQLCHAR*)sql.c_str(), SQL_NTS);
		if (DBFAIL(sqlret)) {
			handle_err(hstmt, SQL_HANDLE_STMT, sqlret, "");
			break;
		}

		sqlret = SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_BINARY, len, 0,
								  (SQLPOINTER)pdata, 0, &data_len);
		if (DBFAIL(sqlret)) {
			handle_err(hstmt, SQL_HANDLE_STMT, sqlret, "");
			break;
		}

		sqlret = SQLExecute(hstmt);
		switch (sqlret) {
		case SQL_SUCCESS_WITH_INFO:
		//handle_err(hstmt, SQL_HANDLE_STMT, sqlret, sql);
		// fall through
		case SQL_SUCCESS:
			break;

		case SQL_ERROR: {
			if (handle_err(hstmt, SQL_HANDLE_STMT, sqlret, sql, true)) {
				goto RECONNECT;
			}
		}
		break;

		case SQL_NO_DATA: // update or delete
			break;

		case SQL_NEED_DATA:
			handle_err(hstmt, SQL_HANDLE_STMT, sqlret, sql);
			break;
		}
	}
	while (0);

	if (hstmt != SQL_NULL_HSTMT) {
		SQLFreeStmt(hstmt, SQL_CLOSE);
		SQLFreeStmt(hstmt, SQL_RESET_PARAMS);
		SQLFreeStmt(hstmt, SQL_UNBIND);
		SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
		hstmt = SQL_NULL_HSTMT;
	}

	return sqlret;
}

// End

bool cfl_rs::getalldata(const std::string& sql, vector<vector<string>>& data, unsigned short timeout) {
	bool ret;

	//
	SQLRETURN sqlret;
	SQLHSTMT hstmt = SQL_NULL_HSTMT;
	SQLSMALLINT col_num = 0;
	bool found = true;

		do {
			sqlret = SQLAllocHandle(SQL_HANDLE_STMT, _hdbc, &hstmt);
			if ((sqlret != SQL_SUCCESS) && (sqlret != SQL_SUCCESS_WITH_INFO)) {
				handle_err(_hdbc, SQL_HANDLE_DBC, sqlret, "");
				break;
			}

			sqlret = SQLSetStmtAttr(hstmt, SQL_QUERY_TIMEOUT, &timeout, 0);
			if ((sqlret != SQL_SUCCESS) && (sqlret != SQL_SUCCESS_WITH_INFO)) {
				handle_err(_hdbc, SQL_HANDLE_DBC, sqlret, "");
				break;
			}

			sqlret = SQLExecDirect(hstmt, (SQLCHAR*)sql.c_str(), SQL_NTS);
			if (sqlret != SQL_SUCCESS) {
				handle_err(hstmt, SQL_HANDLE_STMT, sqlret, sql);

				if (sqlret != SQL_SUCCESS_WITH_INFO) break;
			}

			sqlret = SQLNumResultCols(hstmt, &col_num);
			col_num = min(col_num, MAX_COL);
			col_num = min(col_num, _max_col);

			// Bind Column
			for (int i = 0; i < col_num; ++i) {
				SQLBindCol(hstmt, UWORD(i + 1), SQL_C_CHAR, _buf[i], MAX_DATALEN, &_buf_len[i]);
			}

			// Fetch each Row
			for (int i = 0; ((sqlret = SQLFetch(hstmt)) == SQL_SUCCESS) || (sqlret == SQL_SUCCESS_WITH_INFO); ++i) {
				vector<string> rowV;

				if (sqlret != SQL_SUCCESS) handle_err(hstmt, SQL_HANDLE_STMT, sqlret, sql);

				for (int j = 0; j < col_num; j++) {
					rowV.push_back((char const*)_buf[j]);
				}

				data.push_back(rowV);
			}

			SQLFreeStmt(hstmt, SQL_CLOSE);
			SQLFreeStmt(hstmt, SQL_RESET_PARAMS);
			SQLFreeStmt(hstmt, SQL_UNBIND);
			ret = true;
		}
		while (0);

	if (hstmt != SQL_NULL_HSTMT) {
		SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
		hstmt = SQL_NULL_HSTMT;
	}

	return ret;
}

// Add by lark.li 20080809 begin
bool cfl_rs::get_page_data(char* tablename, char* param, int pagesize, int pageindex, char* filter, char* sort,
						   int sorttype, int& totalpage, int& totalrecord,
						   vector<vector<string>>& data, unsigned short timeout) {
	bool ret;

	//
	SQLRETURN sqlret;
	SQLHSTMT hstmt = SQL_NULL_HSTMT;
	SQLSMALLINT col_num = 0;
	bool found = true;

		do {
			SQLINTEGER sql_nts = SQL_NTS;
			SQLINTEGER num[2];

			//char tablename[32];
			//strncpy(tablename, _tbl_name.c_str(), sizeof(tablename));

			sqlret = SQLAllocHandle(SQL_HANDLE_STMT, _hdbc, &hstmt);
			if ((sqlret != SQL_SUCCESS) && (sqlret != SQL_SUCCESS_WITH_INFO)) {
				handle_err(_hdbc, SQL_HANDLE_DBC, sqlret, "");
				break;
			}

			sqlret = SQLSetStmtAttr(hstmt, SQL_QUERY_TIMEOUT, &timeout, 0);
			if ((sqlret != SQL_SUCCESS) && (sqlret != SQL_SUCCESS_WITH_INFO)) {
				handle_err(_hdbc, SQL_HANDLE_DBC, sqlret, "");
				break;
			}

			SQLFreeStmt(hstmt, SQL_UNBIND);
			SQLFreeStmt(hstmt, SQL_RESET_PARAMS);

			if ((SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, strlen(tablename), 0, tablename,
								  0, &sql_nts) != SQL_SUCCESS)
				|| (SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, strlen(param), 0, param, 0,
									 &sql_nts) != SQL_SUCCESS)
				|| (SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, sizeof(pagesize), 0,
									 &pagesize, 0, &sql_nts) != SQL_SUCCESS)
				|| (SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, sizeof(pageindex), 0,
									 &pageindex, 0, &sql_nts) != SQL_SUCCESS)
				|| (SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, strlen(filter), 0, filter, 0,
									 &sql_nts) != SQL_SUCCESS)
				|| (SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, strlen(sort), 0, sort, 0,
									 &sql_nts) != SQL_SUCCESS)
				|| (SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, sizeof(sorttype), 0,
									 &sorttype, 0, &sql_nts) != SQL_SUCCESS)
				/*||	(SQLBindParameter(hstmt, 8, SQL_PARAM_OUTPUT, SQL_C_SLONG, SQL_INTEGER, 10, 0, &num[0], sizeof(num[0]), &sql_nts) != SQL_SUCCESS)
				||	(SQLBindParameter(hstmt, 9, SQL_PARAM_OUTPUT, SQL_C_SLONG, SQL_INTEGER, 10, 0, &num[1], sizeof(num[1]),  &sql_nts) != SQL_SUCCESS)*/) {
				handle_err(_hdbc, SQL_HANDLE_DBC, sqlret, "");
				break;
			}

			sqlret = SQLExecDirect(hstmt, (SQLCHAR*)"{call sys_Paging(?,?,?,?,?,?,?)}", SQL_NTS);
			if (sqlret != SQL_SUCCESS) {
				handle_err(hstmt, SQL_HANDLE_STMT, sqlret, "{call sys_Paging(?,?,?,?,?,?,?)}");

				if (sqlret != SQL_SUCCESS_WITH_INFO) break;
			}

			sqlret = SQLNumResultCols(hstmt, &col_num);
			col_num = min(col_num, MAX_COL);
			col_num = min(col_num, _max_col);

			// Clear
			memset(_buf, 0, MAX_COL * MAX_DATALEN * sizeof(UCHAR));

			// Bind Column
			for (int i = 0; i < col_num; ++i) {
				SQLBindCol(hstmt, UWORD(i + 1), SQL_C_CHAR, _buf[i], MAX_DATALEN, &_buf_len[i]);
			}

			// Fetch each Row
			for (int i = 0; ((sqlret = SQLFetch(hstmt)) == SQL_SUCCESS) || (sqlret == SQL_SUCCESS_WITH_INFO); ++i) {
				vector<string> rowV;

				if (sqlret != SQL_SUCCESS)
					handle_err(hstmt, SQL_HANDLE_STMT, sqlret,
							   "{call sys_Paging2(?,?,?,?,?,?,?,?,?)}");

				for (int j = 0; j < col_num; j++) {
					rowV.push_back((char const*)_buf[j]);
				}

				data.push_back(rowV);
			}

			if (SQLMoreResults(hstmt) == SQL_SUCCESS) {
				sqlret = SQLNumResultCols(hstmt, &col_num);
				col_num = min(col_num, MAX_COL);
				col_num = min(col_num, _max_col);

				// Bind Column
				for (int i = 0; i < col_num; ++i) {
					SQLBindCol(hstmt, UWORD(i + 1), SQL_C_CHAR, _buf[i], MAX_DATALEN, &_buf_len[i]);
				}

				// Fetch each Row
				for (int i = 0; ((sqlret = SQLFetch(hstmt)) == SQL_SUCCESS) || (sqlret == SQL_SUCCESS_WITH_INFO); ++i) {
					totalpage = atoi((const char*)_buf[0]);
					totalrecord = atoi((const char*)_buf[1]);
				}
			}
			SQLFreeStmt(hstmt, SQL_CLOSE);
			SQLFreeStmt(hstmt, SQL_RESET_PARAMS);
			SQLFreeStmt(hstmt, SQL_UNBIND);
			ret = true;
		}
		while (0);

	if (hstmt != SQL_NULL_HSTMT) {
		SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
		hstmt = SQL_NULL_HSTMT;
	}

	return ret;
}

// End

bool cfl_rs::get(const std::string& sql, char const* pdata, int len, unsigned short timeout /* = 30 */) {
RECONNECT:

	bool ret = false;
	//std::string sql;
	int i = 0;

		ToLogService("util_db", "get() [SQL]: [{}]", sql);

	//
	SQLRETURN sqlret;
	SQLHSTMT hstmt = SQL_NULL_HSTMT;
	SQLSMALLINT col_num = 0;
	bool found = true;

	do {
			sqlret = SQLAllocHandle(SQL_HANDLE_STMT, _hdbc, &hstmt);
			if (DBFAIL(sqlret)) {
				handle_err(_hdbc, SQL_HANDLE_DBC, sqlret, "");
				break;
			}

			std::uint16_t timeout = 50;
			if (DBFAIL(sqlret)) {
				handle_err(_hdbc, SQL_HANDLE_DBC, sqlret, "");
				break;
			}

			sqlret = SQLExecDirect(hstmt, (SQLCHAR*)sql.c_str(), SQL_NTS);
			if (DBFAIL(sqlret)) {
				if (handle_err(hstmt, SQL_HANDLE_STMT, sqlret, sql, true)) {
					goto RECONNECT;
				}
				return false;
			}



			sqlret = SQLNumResultCols(hstmt, &col_num);
			if (col_num <= 0) {

					ToLogService("util_db", "col_num = 0 (<=0)");
				break;
			}

			for (i = 0; i < col_num; ++i) {
				SQLBindCol(hstmt, UWORD(i + 1), SQL_C_CHAR, _buf[i],
						   MAX_DATALEN, &_buf_len[i]);
			}

			sqlret = SQLFetch(hstmt); // only fetch the next row
			if (DBNODATA(sqlret)) {

					ToLogService("util_db", "SQL didn't fetch any data [{}]", sql);
				found = false;
			}
			else if (sqlret != SQL_SUCCESS) {
				handle_err(hstmt, SQL_HANDLE_STMT, sqlret, sql);
				if (sqlret != SQL_SUCCESS_WITH_INFO) {
					break;
				}
			}

			//
			if (found) {
				//
				memcpy((void*)pdata, _buf[0], len);
			}
			else {
			}


		ret = true;
	}
	while (0);


		if (hstmt != SQL_NULL_HSTMT) {
			SQLFreeStmt(hstmt, SQL_CLOSE);
			SQLFreeStmt(hstmt, SQL_RESET_PARAMS);
			SQLFreeStmt(hstmt, SQL_UNBIND);
			SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
		}

	return ret;
}


#if 0
SQLRETURN cfl_rs::exec_sql_direct(char const* sql, bool closecur /* = true */,
								  unsigned short timeout /* = 5 */) {
RECONNECT:

	SQLRETURN sqlret;
	if (g_cchLogUtilDb == 1)
		ToLogService("util_db", "SQL Statement Length: {}", strlen(sql));

	do {
		sqlret = SQLSetStmtOption(_hstmt, SQL_QUERY_TIMEOUT, timeout);
		if (DBFAIL(sqlret)) {
			handle_err(_hdbc, SQL_HANDLE_DBC, sqlret, "");
			break;
		}

		sqlret = SQLExecDirect(_hstmt, (SQLCHAR*)sql.c_str(), SQL_NTS);
		switch (sqlret) {
		case SQL_SUCCESS_WITH_INFO:
			handle_err(_hstmt, SQL_HANDLE_STMT, sqlret, sql);
		// fall through
		case SQL_SUCCESS:
			break;

		case SQL_ERROR: {
			if (handle_err(_hstmt, SQL_HANDLE_STMT, sqlret, sql, true)) {
				goto RECONNECT;
			}
		}
		break;

		case SQL_NO_DATA: // update or delete
			break;
		}
	}
	while (0);

	if (closecur) {
		SQLFreeStmt(_hstmt, SQL_CLOSE);
	}
	return sqlret;
}

SQLRETURN cfl_rs::getdata(int col, string& v) {
	SQLRETURN sqlret;
	SQLLEN dat_len = 0;

	_dat[0] = 0;
	sqlret = SQLGetData(_hstmt, col, SQL_C_CHAR, _dat, sizeof _dat, &dat_len);
	if (DBOK(sqlret)) {
		v = _dat;
	}
	else {
		v = "";
	}

	return sqlret;
}

void cfl_rs::finish() {
	SQLFreeStmt(_hstmt, SQL_CLOSE);
}
#endif

// friend table
//NOTE(Ogge): Might want to check how this query,
// its very similar to the one used in TBLMaster::GetMasterData().
// The first and second SELECT would cause bad values for addr.
// addr stores a pointer address.

constexpr const char query_friend_format[] =
	"select '' relation,count(*) addr,0 atorID,'' atorNome,0 icon,'' motto from ( \
select distinct friends.relation relation from character INNER JOIN \
friends ON character.atorID = friends.cha_id2 where friends.cha_id1 = {}) cc	\
union select '' atorNome,0 addr, -1 atorID,friends.relation relation,0 icon,'' motto from friends	\
where friends.cha_id1 = {} and friends.cha_id2 = -1	\
union select friends.relation relation,count(character.endeMem) addr,0 \
atorID,'' atorNome,1 icon,'' motto from character INNER JOIN friends ON \
character.atorID = friends.cha_id2 where friends.cha_id1 = {} group by relation \
union select friends.relation relation,character.endeMem addr,character.atorID \
atorID,character.atorNome atorNome,character.icon icon,character.motto motto \
from character INNER JOIN friends ON character.atorID = friends.cha_id2 \
where friends.cha_id1 = {} order by relation,atorID,icon";

#if 1
bool friend_tbl::get_friend_dat(friend_dat* farray, int& array_num, unsigned int atorID, bool* drop /* = NULL */) {
	if (!farray || array_num <= 0 || atorID == 0) {
		return false;
	}

	//NOTE(Ogge): Remember the size of farray before resetting array_num to be used as a return value
	const auto array_size{array_num};
	array_num = 0;

	bool ret = false;
	std::string sql;
	sql = std::vformat(query_friend_format, std::make_format_args(atorID, atorID, atorID, atorID));

	//
	SQLRETURN sqlret;
	SQLHSTMT hstmt = SQL_NULL_HSTMT;
	SQLSMALLINT col_num = 0;
	bool found = true;

		do {
			sqlret = SQLAllocHandle(SQL_HANDLE_STMT, _hdbc, &hstmt);
			if ((sqlret != SQL_SUCCESS) && (sqlret != SQL_SUCCESS_WITH_INFO)) {
				handle_err(_hdbc, SQL_HANDLE_DBC, sqlret, "");
				break;
			}

			std::uint16_t timeout = 50;
			if ((sqlret != SQL_SUCCESS) && (sqlret != SQL_SUCCESS_WITH_INFO)) {
				handle_err(_hdbc, SQL_HANDLE_DBC, sqlret, "");
				break;
			}

			sqlret = SQLExecDirect(hstmt, (SQLCHAR*)sql.c_str(), SQL_NTS);
			if (sqlret != SQL_SUCCESS) {
				handle_err(hstmt, SQL_HANDLE_STMT, sqlret, sql);

				if (sqlret != SQL_SUCCESS_WITH_INFO) break;
			}

			sqlret = SQLNumResultCols(hstmt, &col_num);
			col_num = min(col_num, MAX_COL);
			col_num = min(col_num, _max_col);

			// Bind Column
			for (int i = 0; i < col_num; ++i) {
				SQLBindCol(hstmt, UWORD(i + 1), SQL_C_CHAR, _buf[i], MAX_DATALEN, &_buf_len[i]);
			}

			// Fetch each Row
			int i = 0;
			for (; ((sqlret = SQLFetch(hstmt)) == SQL_SUCCESS) || (sqlret == SQL_SUCCESS_WITH_INFO); ++i) {
				if (i >= array_size) {
					if (drop) {
						*drop = true;
					}
					break;
				}

				if (sqlret != SQL_SUCCESS) {
					handle_err(hstmt, SQL_HANDLE_STMT, sqlret, sql);
				}

				farray[i].relation = (char const*)_buf[0];
				farray[i].memaddr = atoi((char const*)_buf[1]);
				farray[i].atorID = atoi((char const*)_buf[2]);
				farray[i].atorNome = (char const*)_buf[3];
				farray[i].icon_id = atoi((char const*)_buf[4]);
				farray[i].motto = (char const*)_buf[5];
			}

			array_num = i; //NOTE(Ogge): Return number of rows read through referenced argument

			SQLFreeStmt(hstmt, SQL_CLOSE);
			SQLFreeStmt(hstmt, SQL_RESET_PARAMS);
			SQLFreeStmt(hstmt, SQL_UNBIND);
			ret = true;
		}
		while (0);

	if (hstmt != SQL_NULL_HSTMT) {
		SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
		hstmt = SQL_NULL_HSTMT;
	}

	return ret;
}
#endif

#if 0
bool friend_tbl::get_friend_dat(friend_dat* farray, int& array_num, unsigned int atorID, bool* drop /* = NULL */) {
	if (farray == NULL || array_num <= 0 || atorID == 0) return false;

	bool ret = false;
	std::string sql;
	sql = std::vformat(query_friend_format, std::make_format_args(atorID, atorID, atorID));

	//
	SQLRETURN sqlret = exec_sql_direct(sql, false);
	if (DBOK(sqlret)) {
		string tmp[6];
		int n, i = 0;

		while (!DBNODATA(fetch())) {
			n = 1;
			getdata(n++, tmp[0]);
			getdata(n++, tmp[1]);
			getdata(n++, tmp[2]);
			getdata(n++, tmp[3]);
			getdata(n++, tmp[4]);
			getdata(n++, tmp[5]);

			if (i < array_num) {
				farray[i].relation = tmp[0];
				farray[i].memaddr = atoi(tmp[1].c_str());
				farray[i].atorID = atoi(tmp[2].c_str());
				farray[i].atorNome = tmp[3];
				farray[i].icon_id = atoi(tmp[4].c_str());
				farray[i].motto = tmp[5];

				array_num = i + 1;
			}
			else {
				if (drop != NULL) *drop = true;
			}

			i++;
		}

		finish();
	}
	else {
		stmt_err(sqlret, sql, true);
		ret = false;
	}

	return ret;
}
#endif

bool friend_tbl::get_gm_dat(friend_dat* farray, int& array_num, bool* drop) {
	if (!farray || array_num <= 0) {
		return false;
	}

	//NOTE(Ogge): Remember the size of farray before resetting array_num to be used as a return value
	const auto array_size{array_num};
	array_num = 0;

	bool ret = false;
	std::string sql =
		"select endeMem,atorID,atorNome,icon,motto from character join account on character.ato_id = account.ato_id where jmes > 0 and delflag = 0 and endeMem>0;";

	SQLRETURN sqlret;
	SQLHSTMT hstmt = SQL_NULL_HSTMT;
	SQLSMALLINT col_num = 0;
	bool found = true;

	do {
		sqlret = SQLAllocHandle(SQL_HANDLE_STMT, _hdbc, &hstmt);
		if ((sqlret != SQL_SUCCESS) && (sqlret != SQL_SUCCESS_WITH_INFO)) {
			handle_err(_hdbc, SQL_HANDLE_DBC, sqlret, "");
			break;
		}

		std::uint16_t timeout = 50;
		if ((sqlret != SQL_SUCCESS) && (sqlret != SQL_SUCCESS_WITH_INFO)) {
			handle_err(_hdbc, SQL_HANDLE_DBC, sqlret, "");
			break;
		}

		sqlret = SQLExecDirect(hstmt, (SQLCHAR*)sql.c_str(), SQL_NTS);
		if (sqlret != SQL_SUCCESS) {
			handle_err(hstmt, SQL_HANDLE_STMT, sqlret, sql);

			if (sqlret != SQL_SUCCESS_WITH_INFO) break;
		}

		sqlret = SQLNumResultCols(hstmt, &col_num);
		col_num = min(col_num, MAX_COL);
		col_num = min(col_num, _max_col);

		// Bind Column
		for (int i = 0; i < col_num; ++i) {
			SQLBindCol(hstmt, UWORD(i + 1), SQL_C_CHAR, _buf[i], MAX_DATALEN, &_buf_len[i]);
		}

		// Fetch each Row
		int i = 0;
		for (; ((sqlret = SQLFetch(hstmt)) == SQL_SUCCESS) || (sqlret == SQL_SUCCESS_WITH_INFO); ++i) {
			if (i >= array_size) {
				if (drop != nullptr)
					*drop = true;

				break;
			}

			if (sqlret != SQL_SUCCESS) handle_err(hstmt, SQL_HANDLE_STMT, sqlret, sql);

			farray[i].relation = "GM";
			farray[i].memaddr = atoi((char const*)_buf[0]);
			farray[i].atorID = atoi((char const*)_buf[1]);
			farray[i].atorNome = (char const*)_buf[2];
			farray[i].icon_id = atoi((char const*)_buf[3]);
			farray[i].motto = (char const*)_buf[4];
		}

		array_num = i; //

		SQLFreeStmt(hstmt, SQL_CLOSE);
		SQLFreeStmt(hstmt, SQL_RESET_PARAMS);
		SQLFreeStmt(hstmt, SQL_UNBIND);
		ret = true;
	}
	while (0);


	if (hstmt != SQL_NULL_HSTMT) {
		SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
		hstmt = SQL_NULL_HSTMT;
	}

	return ret;
}
