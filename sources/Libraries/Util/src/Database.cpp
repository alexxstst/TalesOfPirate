#include "Database.h"
#include "logutil.h"
#include <format>

// ============================================================================
// Вспомогательные функции
// ============================================================================

static std::string ToUpper(std::string_view s) {
	std::string result(s);
	std::transform(result.begin(), result.end(), result.begin(),
				   [](unsigned char c) {
					   return static_cast<char>(std::toupper(c));
				   });
	return result;
}

static std::string ExtractOdbcError(SQLSMALLINT handleType, SQLHANDLE handle) {
	SQLCHAR sqlState[6]{};
	SQLINTEGER nativeError{};
	SQLCHAR message[1024]{};
	SQLSMALLINT msgLen{};

	if (SQLGetDiagRec(handleType, handle, 1, sqlState, &nativeError, message, sizeof(message), &msgLen) ==
		SQL_SUCCESS) {
		return std::format("[{}] ({}) {}", reinterpret_cast<char*>(sqlState), nativeError,
						   reinterpret_cast<char*>(message));
	}
	return "Unknown ODBC error";
}

// ============================================================================
// OdbcException
// ============================================================================

OdbcException::OdbcException(std::string sqlState, int nativeError, std::string message)
	: std::runtime_error(std::move(message)), _sqlState(std::move(sqlState)), _nativeError(nativeError) {
}

// ============================================================================
// OdbcDatabase::ThrowIfError
// ============================================================================

void OdbcDatabase::ThrowIfError(SQLSMALLINT handleType, SQLHANDLE handle, SQLRETURN ret, std::string_view context) {
	if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
		return; 
	}

	SQLCHAR sqlState[6]{};
	SQLINTEGER nativeError{};
	SQLCHAR message[1024]{};
	SQLSMALLINT msgLen{};

	std::string fullMsg;
	if (SQLGetDiagRec(handleType, handle, 1, sqlState, &nativeError, message, sizeof(message), &msgLen) ==
		SQL_SUCCESS) {
		fullMsg = std::format("{}: [{}] ({}) {}", context, reinterpret_cast<char*>(sqlState), nativeError,
							  reinterpret_cast<char*>(message));
	}
	else {
		fullMsg = std::format("{}: ODBC error (SQLRETURN={})", context, ret);
	}

	ToLogService("errors", LogLevel::Error, "OdbcDatabase: {}", fullMsg);
	throw OdbcException(reinterpret_cast<char*>(sqlState), nativeError, fullMsg);
}

// ============================================================================
// OdbcDatabase
// ============================================================================

OdbcDatabase::~OdbcDatabase() {
	Close();
}

void OdbcDatabase::Open(std::string_view connectionString) {
	if (IsOpen()) {
		Close();
	}

	_connectionString = std::string(connectionString);

	SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &_henv);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
		throw OdbcException("", 0, "Failed to allocate ODBC environment handle");
	}

	SQLSetEnvAttr(_henv, SQL_ATTR_ODBC_VERSION, reinterpret_cast<SQLPOINTER>(SQL_OV_ODBC3), 0);

	ret = SQLAllocHandle(SQL_HANDLE_DBC, _henv, &_hdbc);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
		SQLFreeHandle(SQL_HANDLE_ENV, _henv);
		_henv = SQL_NULL_HENV;
		throw OdbcException("", 0, "Failed to allocate ODBC connection handle");
	}

	SQLSetConnectAttr(_hdbc, SQL_ATTR_LOGIN_TIMEOUT, reinterpret_cast<SQLPOINTER>(10), 0);

	SQLCHAR outStr[1024]{};
	SQLSMALLINT outLen{};
	ret = SQLDriverConnect(_hdbc, nullptr,
						   reinterpret_cast<SQLCHAR*>(_connectionString.data()),
						   static_cast<SQLSMALLINT>(_connectionString.size()),
						   outStr, sizeof(outStr), &outLen, SQL_DRIVER_NOPROMPT);

	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
		auto err = ExtractOdbcError(SQL_HANDLE_DBC, _hdbc);
		SQLFreeHandle(SQL_HANDLE_DBC, _hdbc);
		SQLFreeHandle(SQL_HANDLE_ENV, _henv);
		_hdbc = SQL_NULL_HDBC;
		_henv = SQL_NULL_HENV;
		throw OdbcException("", 0, "Failed to connect: " + err);
	}

	ToLogService("db", "OdbcDatabase: connected to {}", _connectionString.substr(0, 60));
}

void OdbcDatabase::Close() {
	if (_hdbc != SQL_NULL_HDBC) {
		SQLDisconnect(_hdbc);
		SQLFreeHandle(SQL_HANDLE_DBC, _hdbc);
		_hdbc = SQL_NULL_HDBC;
	}
	if (_henv != SQL_NULL_HENV) {
		SQLFreeHandle(SQL_HANDLE_ENV, _henv);
		_henv = SQL_NULL_HENV;
	}
}

void OdbcDatabase::EnsureConnected() {
	if (IsOpen()) {
		// Проверяем живость соединения через простой запрос
		SQLHSTMT hstmt{SQL_NULL_HSTMT};
		SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_STMT, _hdbc, &hstmt);
		if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
			ret = SQLExecDirect(hstmt, reinterpret_cast<SQLCHAR*>(const_cast<char*>("SELECT 1")), SQL_NTS);
			SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
			if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
				return; // Соединение живо
			}
		}

		// Соединение мертво — закрываем
		ToLogService("db", LogLevel::Warning, "OdbcDatabase: connection lost, attempting reconnect...");
		Close();
	}

	// Переподключение с повторами
	if (_connectionString.empty()) {
		throw OdbcException("", 0, "Cannot reconnect: no connection string");
	}

	for (int attempt = 1; attempt <= _maxReconnectAttempts; ++attempt) {
		try {
			Open(_connectionString);
			ToLogService("db", "OdbcDatabase: reconnected (attempt {})", attempt);
			return;
		}
		catch (const OdbcException& e) {
			ToLogService("db", LogLevel::Error, "OdbcDatabase: reconnect attempt {}/{} failed: {}",
						 attempt, _maxReconnectAttempts, e.what());
			if (attempt == _maxReconnectAttempts) {
				throw;
			}
			Sleep(1000 * attempt); // Прогрессивная задержка
		}
	}
}

OdbcCommand OdbcDatabase::CreateCommand(std::string_view sql) {
	EnsureConnected();
	return OdbcCommand(*this, sql);
}

OdbcTransaction OdbcDatabase::BeginTransaction() {
	EnsureConnected();
	return OdbcTransaction(_hdbc);
}

// ============================================================================
// OdbcReader
// ============================================================================

OdbcReader::OdbcReader(SQLHSTMT hstmt, std::string sql) : _hstmt(hstmt), _sql(std::move(sql)) {
	BuildColumnMap();
}

OdbcReader::~OdbcReader() {
	Close();
}

OdbcReader::OdbcReader(OdbcReader&& other) noexcept
	: _hstmt(other._hstmt), _closed(other._closed), _sql(std::move(other._sql)),
	  _columnNames(std::move(other._columnNames)), _columnMap(std::move(other._columnMap)) {
	other._hstmt = SQL_NULL_HSTMT;
	other._closed = true;
}

OdbcReader& OdbcReader::operator=(OdbcReader&& other) noexcept {
	if (this != &other) {
		Close();
		_hstmt = other._hstmt;
		_closed = other._closed;
		_sql = std::move(other._sql);
		_columnNames = std::move(other._columnNames);
		_columnMap = std::move(other._columnMap);
		other._hstmt = SQL_NULL_HSTMT;
		other._closed = true;
	}
	return *this;
}

void OdbcReader::BuildColumnMap() {
	if (_hstmt == SQL_NULL_HSTMT) {
		return;
	}

	SQLSMALLINT numCols{};
	SQLNumResultCols(_hstmt, &numCols);

	_columnNames.resize(numCols);
	for (SQLSMALLINT i = 0; i < numCols; ++i) {
		SQLCHAR colName[256]{};
		SQLSMALLINT nameLen{};
		SQLDescribeCol(_hstmt, static_cast<SQLUSMALLINT>(i + 1), colName, sizeof(colName), &nameLen,
					   nullptr, nullptr, nullptr, nullptr);
		_columnNames[i] = reinterpret_cast<char*>(colName);
		_columnMap[ToUpper(_columnNames[i])] = i;
	}
}

bool OdbcReader::Read() {
	if (_closed || _hstmt == SQL_NULL_HSTMT) {
		return false;
	}
	SQLRETURN ret = SQLFetch(_hstmt);
	if (ret == SQL_NO_DATA) {
		return false;
	}
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
		OdbcDatabase::ThrowIfError(SQL_HANDLE_STMT, _hstmt, ret, std::format("OdbcReader::Read [{}]", _sql));
	}
	return true;
}

std::string OdbcReader::GetString(int col) const {
	SQLCHAR buf[8192]{};
	SQLLEN indicator{};
	SQLRETURN ret = SQLGetData(_hstmt, static_cast<SQLUSMALLINT>(col + 1), SQL_C_CHAR,
							   buf, sizeof(buf), &indicator);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
		OdbcDatabase::ThrowIfError(SQL_HANDLE_STMT, _hstmt, ret, std::format("OdbcReader::GetString(col={}) [{}]", col, _sql));
	}
	if (indicator == SQL_NULL_DATA) {
		return {};
	}
	return reinterpret_cast<char*>(buf);
}

int OdbcReader::GetInt(int col) const {
	SQLINTEGER val{};
	SQLLEN indicator{};
	SQLRETURN ret = SQLGetData(_hstmt, static_cast<SQLUSMALLINT>(col + 1), SQL_C_SLONG, &val, sizeof(val), &indicator);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
		OdbcDatabase::ThrowIfError(SQL_HANDLE_STMT, _hstmt, ret, std::format("OdbcReader::GetInt(col={}) [{}]", col, _sql));
	}
	if (indicator == SQL_NULL_DATA) {
		return 0;
	}
	return val;
}

int64_t OdbcReader::GetInt64(int col) const {
	SQLBIGINT val{};
	SQLLEN indicator{};
	SQLRETURN ret = SQLGetData(_hstmt, static_cast<SQLUSMALLINT>(col + 1), SQL_C_SBIGINT, &val, sizeof(val), &indicator);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
		OdbcDatabase::ThrowIfError(SQL_HANDLE_STMT, _hstmt, ret, std::format("OdbcReader::GetInt64(col={}) [{}]", col, _sql));
	}
	if (indicator == SQL_NULL_DATA) {
		return 0;
	}
	return val;
}

double OdbcReader::GetDouble(int col) const {
	SQLDOUBLE val{};
	SQLLEN indicator{};
	SQLRETURN ret = SQLGetData(_hstmt, static_cast<SQLUSMALLINT>(col + 1), SQL_C_DOUBLE, &val, sizeof(val), &indicator);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
		OdbcDatabase::ThrowIfError(SQL_HANDLE_STMT, _hstmt, ret, std::format("OdbcReader::GetDouble(col={}) [{}]", col, _sql));
	}
	if (indicator == SQL_NULL_DATA) {
		return 0.0;
	}
	return val;
}

std::vector<uint8_t> OdbcReader::GetBinary(int col) const {
	auto colNo = static_cast<SQLUSMALLINT>(col + 1);

	// Первый вызов — узнаём размер данных
	SQLLEN indicator{};
	SQLRETURN ret = SQLGetData(_hstmt, colNo, SQL_C_BINARY, nullptr, 0, &indicator);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO && indicator != SQL_NULL_DATA) {
		OdbcDatabase::ThrowIfError(SQL_HANDLE_STMT, _hstmt, ret, std::format("OdbcReader::GetBinary(col={}) size probe [{}]", col, _sql));
	}
	if (indicator == SQL_NULL_DATA || indicator <= 0) {
		return {};
	}

	// Второй вызов — читаем данные
	std::vector<uint8_t> buf(static_cast<size_t>(indicator));
	ret = SQLGetData(_hstmt, colNo, SQL_C_BINARY, buf.data(), static_cast<SQLLEN>(buf.size()), &indicator);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
		OdbcDatabase::ThrowIfError(SQL_HANDLE_STMT, _hstmt, ret, std::format("OdbcReader::GetBinary(col={}) read [{}]", col, _sql));
	}
	return buf;
}

bool OdbcReader::IsNull(int col) const {
	SQLCHAR buf[1]{};
	SQLLEN indicator{};
	SQLRETURN ret = SQLGetData(_hstmt, static_cast<SQLUSMALLINT>(col + 1), SQL_C_CHAR, buf, 0, &indicator);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO && indicator != SQL_NULL_DATA) {
		OdbcDatabase::ThrowIfError(SQL_HANDLE_STMT, _hstmt, ret, std::format("OdbcReader::IsNull(col={}) [{}]", col, _sql));
	}
	return indicator == SQL_NULL_DATA;
}

int OdbcReader::ResolveColumn(std::string_view name) const {
	auto it = _columnMap.find(ToUpper(name));
	if (it == _columnMap.end()) {
		throw OdbcException("", 0, std::format("Column '{}' not found in result set", name));
	}
	return it->second;
}

std::string OdbcReader::GetString(std::string_view name) const {
	return GetString(ResolveColumn(name));
}

int OdbcReader::GetInt(std::string_view name) const {
	return GetInt(ResolveColumn(name));
}

int64_t OdbcReader::GetInt64(std::string_view name) const {
	return GetInt64(ResolveColumn(name));
}

double OdbcReader::GetDouble(std::string_view name) const {
	return GetDouble(ResolveColumn(name));
}

std::vector<uint8_t> OdbcReader::GetBinary(std::string_view name) const {
	return GetBinary(ResolveColumn(name));
}

bool OdbcReader::IsNull(std::string_view name) const {
	return IsNull(ResolveColumn(name));
}

int OdbcReader::GetColumnIndex(std::string_view name) const {
	return ResolveColumn(name);
}

const std::string& OdbcReader::GetColumnName(int col) const {
	return _columnNames.at(col);
}

void OdbcReader::Close() {
	if (!_closed && _hstmt != SQL_NULL_HSTMT) {
		SQLCloseCursor(_hstmt);
		SQLFreeHandle(SQL_HANDLE_STMT, _hstmt);
		_hstmt = SQL_NULL_HSTMT;
		_closed = true;
	}
}

// ============================================================================
// OdbcCommand
// ============================================================================

OdbcCommand::OdbcCommand(OdbcDatabase& db, std::string_view sql)
	: _db(db), _sql(sql) {
	if (!_sql.empty()) {
		ParseNamedParameters();
	}
}

OdbcCommand::~OdbcCommand() {
	if (_hstmt != SQL_NULL_HSTMT) {
		SQLFreeHandle(SQL_HANDLE_STMT, _hstmt);
	}
}

OdbcCommand::OdbcCommand(OdbcCommand&& other) noexcept
	: _db(other._db), _hstmt(other._hstmt), _sql(std::move(other._sql)),
	  _processedSql(std::move(other._processedSql)), _timeout(other._timeout),
	  _params(std::move(other._params)), _namedParams(std::move(other._namedParams)),
	  _indicators(std::move(other._indicators)) {
	other._hstmt = SQL_NULL_HSTMT;
}

OdbcCommand& OdbcCommand::operator=(OdbcCommand&& other) noexcept {
	if (this != &other) {
		if (_hstmt != SQL_NULL_HSTMT) SQLFreeHandle(SQL_HANDLE_STMT, _hstmt);
		_hstmt = other._hstmt;
		_sql = std::move(other._sql);
		_processedSql = std::move(other._processedSql);
		_timeout = other._timeout;
		_params = std::move(other._params);
		_namedParams = std::move(other._namedParams);
		_indicators = std::move(other._indicators);
		other._hstmt = SQL_NULL_HSTMT;
	}
	return *this;
}

void OdbcCommand::SetCommandText(std::string_view sql) {
	_sql = std::string(sql);
	_params.clear();
	_namedParams.clear();
	ParseNamedParameters();
}

void OdbcCommand::ParseNamedParameters() {
	_namedParams.clear();
	_processedSql.clear();
	_processedSql.reserve(_sql.size());

	int paramIndex = 0;
	bool inString = false;

	for (size_t i = 0; i < _sql.size(); ++i) {
		char c = _sql[i];

		// Отслеживание строковых литералов
		if (c == '\'') {
			inString = !inString;
			_processedSql += c;
			continue;
		}

		if (inString) {
			_processedSql += c;
			continue;
		}

		if (c == '?') {
			++paramIndex;
			_processedSql += '?';
		}
		else if (c == '@') {
			// Именованный параметр: @identifier
			size_t start = i;
			++i;
			while (i < _sql.size() && (std::isalnum(static_cast<unsigned char>(_sql[i])) || _sql[i] == '_')) {
				++i;
			}
			std::string name = _sql.substr(start, i - start);
			--i; // цикл сделает ++i

			auto it = _namedParams.find(name);
			if (it == _namedParams.end()) {
				++paramIndex;
				_namedParams[name] = paramIndex;
			}
			_processedSql += '?';
		}
		else {
			_processedSql += c;
		}
	}
}

int OdbcCommand::ResolveNamedParam(std::string_view name) {
	auto it = _namedParams.find(std::string(name));
	if (it == _namedParams.end()) {
		throw OdbcException("", 0, std::format("Named parameter '{}' not found in SQL", name));
	}
	return it->second;
}

void OdbcCommand::AllocateStatement() {
	if (_hstmt != SQL_NULL_HSTMT) {
		SQLFreeHandle(SQL_HANDLE_STMT, _hstmt);
		_hstmt = SQL_NULL_HSTMT;
	}

	SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_STMT, _db._hdbc, &_hstmt);
	OdbcDatabase::ThrowIfError(SQL_HANDLE_DBC, _db._hdbc, ret, "AllocHandle(STMT)");

	SQLSetStmtAttr(_hstmt, SQL_ATTR_QUERY_TIMEOUT, reinterpret_cast<SQLPOINTER>(static_cast<SQLLEN>(_timeout)), 0);
}

// Вспомогательная функция для ensure capacity
static void EnsureParamSlot(std::vector<OdbcCommand::ParamValue>& params, int index) {
	if (index < 1) {
		throw OdbcException("", 0, "Parameter index must be >= 1");
	}
	if (static_cast<size_t>(index) > params.size()) {
		params.resize(index);
	}
}

// SetParam по индексу
OdbcCommand& OdbcCommand::SetParam(int index, int value) {
	EnsureParamSlot(_params, index);
	_params[index - 1].value = value;
	return *this;
}

OdbcCommand& OdbcCommand::SetParam(int index, int64_t value) {
	EnsureParamSlot(_params, index);
	_params[index - 1].value = value;
	return *this;
}

OdbcCommand& OdbcCommand::SetParam(int index, std::string_view value) {
	EnsureParamSlot(_params, index);
	_params[index - 1].value = std::string(value);
	return *this;
}

OdbcCommand& OdbcCommand::SetParam(int index, double value) {
	EnsureParamSlot(_params, index);
	_params[index - 1].value = value;
	return *this;
}

OdbcCommand& OdbcCommand::SetParam(int index, std::nullptr_t) {
	EnsureParamSlot(_params, index);
	_params[index - 1].value = std::monostate{};
	return *this;
}

OdbcCommand& OdbcCommand::SetParam(int index, const void* data, size_t len) {
	EnsureParamSlot(_params, index);
	auto* bytes = static_cast<const char*>(data);
	_params[index - 1].value = std::vector<char>(bytes, bytes + len);
	return *this;
}

OdbcCommand& OdbcCommand::SetTimestampParam(int index, std::string_view value) {
	EnsureParamSlot(_params, index);
	_params[index - 1].value = TimestampString{std::string(value)};
	return *this;
}

// SetParam по имени
OdbcCommand& OdbcCommand::SetParam(std::string_view name, int value) {
	return SetParam(ResolveNamedParam(name), value);
}

OdbcCommand& OdbcCommand::SetParam(std::string_view name, int64_t value) {
	return SetParam(ResolveNamedParam(name), value);
}

OdbcCommand& OdbcCommand::SetParam(std::string_view name, std::string_view value) {
	return SetParam(ResolveNamedParam(name), value);
}

OdbcCommand& OdbcCommand::SetParam(std::string_view name, double value) {
	return SetParam(ResolveNamedParam(name), value);
}

OdbcCommand& OdbcCommand::SetParam(std::string_view name, std::nullptr_t) {
	return SetParam(ResolveNamedParam(name), nullptr);
}

void OdbcCommand::ClearParameters() {
	_params.clear();
	_indicators.clear();
}

void OdbcCommand::BindAndExecute() {
	AllocateStatement();

	const std::string& sql = _processedSql.empty() ? _sql : _processedSql;

	if (_params.empty()) {
		// Прямое выполнение без параметров
		SQLRETURN ret = SQLExecDirect(_hstmt, reinterpret_cast<SQLCHAR*>(const_cast<char*>(sql.c_str())),
									  static_cast<SQLINTEGER>(sql.size()));
		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA) {
			OdbcDatabase::ThrowIfError(SQL_HANDLE_STMT, _hstmt, ret, sql);
		}
		return;
	}

	// Prepared execution с параметрами
	SQLRETURN ret = SQLPrepare(_hstmt, reinterpret_cast<SQLCHAR*>(const_cast<char*>(sql.c_str())),
							   static_cast<SQLINTEGER>(sql.size()));
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
		OdbcDatabase::ThrowIfError(SQL_HANDLE_STMT, _hstmt, ret, "SQLPrepare");
	}

	_indicators.resize(_params.size());

	for (size_t i = 0; i < _params.size(); ++i) {
		SQLUSMALLINT paramNo = static_cast<SQLUSMALLINT>(i + 1);
		auto& pv = _params[i];

		std::visit([&](auto&& val) {
			using V = std::decay_t<decltype(val)>;

			if constexpr (std::is_same_v<V, int>) {
				_indicators[i] = 0;
				SQLBindParameter(_hstmt, paramNo, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,
								 0, 0, const_cast<int*>(&std::get<int>(pv.value)), 0, &_indicators[i]);
			}
			else if constexpr (std::is_same_v<V, int64_t>) {
				_indicators[i] = 0;
				SQLBindParameter(_hstmt, paramNo, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT,
								 0, 0, const_cast<int64_t*>(&std::get<int64_t>(pv.value)), 0, &_indicators[i]);
			}
			else if constexpr (std::is_same_v<V, std::string>) {
				auto& s = std::get<std::string>(pv.value);
				_indicators[i] = static_cast<SQLLEN>(s.size());
				SQLBindParameter(_hstmt, paramNo, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
								 s.size(), 0, const_cast<char*>(s.c_str()), static_cast<SQLLEN>(s.size()),
								 &_indicators[i]);
			}
			else if constexpr (std::is_same_v<V, double>) {
				_indicators[i] = 0;
				SQLBindParameter(_hstmt, paramNo, SQL_PARAM_INPUT, SQL_C_DOUBLE, SQL_DOUBLE,
								 0, 0, const_cast<double*>(&std::get<double>(pv.value)), 0, &_indicators[i]);
			}
			else if constexpr (std::is_same_v<V, std::vector<char>>) {
				auto& bin = std::get<std::vector<char>>(pv.value);
				_indicators[i] = static_cast<SQLLEN>(bin.size());
				SQLBindParameter(_hstmt, paramNo, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_LONGVARBINARY,
								 bin.size(), 0, const_cast<char*>(bin.data()), static_cast<SQLLEN>(bin.size()),
								 &_indicators[i]);
			}
			else if constexpr (std::is_same_v<V, std::monostate>) {
				_indicators[i] = SQL_NULL_DATA;
				SQLBindParameter(_hstmt, paramNo, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
								 1, 0, nullptr, 0, &_indicators[i]);
			}
			else if constexpr (std::is_same_v<V, TimestampString>) {
				auto& ts = std::get<TimestampString>(pv.value);
				_indicators[i] = static_cast<SQLLEN>(ts.value.size());
				SQLBindParameter(_hstmt, paramNo, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_TYPE_TIMESTAMP,
								 23, 3, const_cast<char*>(ts.value.c_str()),
								 static_cast<SQLLEN>(ts.value.size()), &_indicators[i]);
			}
		}, pv.value);
	}

	ret = SQLExecute(_hstmt);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA) {
		// Дамп параметров для диагностики
		std::string paramDump;
		for (size_t i = 0; i < _params.size(); ++i) {
			if (!paramDump.empty()) {
				paramDump += ", ";
			}
			paramDump += std::format("[{}]=", i + 1);
			std::visit([&](auto&& val) {
				using V = std::decay_t<decltype(val)>;
				if constexpr (std::is_same_v<V, int>) {
					paramDump += std::to_string(val);
				} else if constexpr (std::is_same_v<V, int64_t>) {
					paramDump += std::to_string(val);
				} else if constexpr (std::is_same_v<V, std::string>) {
					if (val.size() <= 64) {
						paramDump += "'" + val + "'";
					} else {
						paramDump += std::format("'{}...' ({}b)", val.substr(0, 64), val.size());
					}
				} else if constexpr (std::is_same_v<V, double>) {
					paramDump += std::to_string(val);
				} else if constexpr (std::is_same_v<V, std::vector<char>>) {
					paramDump += std::format("<binary {}b>", val.size());
				} else if constexpr (std::is_same_v<V, std::monostate>) {
					paramDump += "NULL";
				} else if constexpr (std::is_same_v<V, TimestampString>) {
					paramDump += "TS'" + val.value + "'";
				}
			}, _params[i].value);
		}
		ToLogService("errors", LogLevel::Error, "OdbcCommand params: {}", paramDump);
		OdbcDatabase::ThrowIfError(SQL_HANDLE_STMT, _hstmt, ret, sql);
	}
}

int OdbcCommand::ExecuteNonQuery() {
	BindAndExecute();

	SQLLEN rowCount{};
	SQLRowCount(_hstmt, &rowCount);

	SQLFreeHandle(SQL_HANDLE_STMT, _hstmt);
	_hstmt = SQL_NULL_HSTMT;

	return static_cast<int>(rowCount);
}

OdbcReader OdbcCommand::ExecuteReader() {
	BindAndExecute();

	// Передаём владение HSTMT и SQL в OdbcReader
	SQLHSTMT stmt = _hstmt;
	_hstmt = SQL_NULL_HSTMT;
	const std::string& sql = _processedSql.empty() ? _sql : _processedSql;
	return OdbcReader(stmt, sql);
}

std::string OdbcCommand::ExecuteScalar() {
	BindAndExecute();

	std::string result;
	SQLRETURN ret = SQLFetch(_hstmt);
	if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
		SQLCHAR buf[8192]{};
		SQLLEN indicator{};
		SQLGetData(_hstmt, 1, SQL_C_CHAR, buf, sizeof(buf), &indicator);
		if (indicator != SQL_NULL_DATA) {
			result = reinterpret_cast<char*>(buf);
		}
	}

	SQLFreeHandle(SQL_HANDLE_STMT, _hstmt);
	_hstmt = SQL_NULL_HSTMT;
	return result;
}

// ============================================================================
// OdbcTransaction
// ============================================================================

OdbcTransaction::OdbcTransaction(SQLHDBC hdbc) : _hdbc(hdbc) {
	SQLSetConnectAttr(_hdbc, SQL_ATTR_AUTOCOMMIT,
					  reinterpret_cast<SQLPOINTER>(SQL_AUTOCOMMIT_OFF), SQL_IS_UINTEGER);
}

OdbcTransaction::~OdbcTransaction() {
	if (!_finished && _hdbc != SQL_NULL_HDBC) {
		Rollback();
	}
}

OdbcTransaction::OdbcTransaction(OdbcTransaction&& other) noexcept
	: _hdbc(other._hdbc), _finished(other._finished) {
	other._hdbc = SQL_NULL_HDBC;
	other._finished = true;
}

void OdbcTransaction::Commit() {
	if (_finished) {
		return;
	}
	SQLEndTran(SQL_HANDLE_DBC, _hdbc, SQL_COMMIT);
	SQLSetConnectAttr(_hdbc, SQL_ATTR_AUTOCOMMIT,
					  reinterpret_cast<SQLPOINTER>(SQL_AUTOCOMMIT_ON), SQL_IS_UINTEGER);
	_finished = true;
}

void OdbcTransaction::Rollback() {
	if (_finished) {
		return;
	}
	SQLEndTran(SQL_HANDLE_DBC, _hdbc, SQL_ROLLBACK);
	SQLSetConnectAttr(_hdbc, SQL_ATTR_AUTOCOMMIT,
					  reinterpret_cast<SQLPOINTER>(SQL_AUTOCOMMIT_ON), SQL_IS_UINTEGER);
	_finished = true;
}
