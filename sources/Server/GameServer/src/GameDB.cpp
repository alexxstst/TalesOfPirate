#include "stdafx.h"
#include "Character.h"
#include "Player.h"
#include "GameDB.h"
#include "ChaAttrType.h"
#include "SubMap.h"
#include "Config.h"
#include "Guild.h"
#include "CommFunc.h"
#include "lua_gamectrl.h"

using namespace std;

char szDBLog[256] = "DBData";

// ============================================================================
// PlayerStorage — private хелперы
// ============================================================================

void PlayerStorage::handle_err(SQLHANDLE, SQLSMALLINT, RETCODE, const char*, bool) {
}

const char* PlayerStorage::_get_table() const {
	return "character";
}

SQLRETURN PlayerStorage::exec_sql_direct(const char* sql) {
	_db.CreateCommand(sql).ExecuteNonQuery();
	return SQL_SUCCESS;
}

int PlayerStorage::get_affected_rows() {
	auto r = _db.CreateCommand("SELECT @@ROWCOUNT").ExecuteScalar();
	return r.empty() ? 0 : std::stoi(r);
}

bool PlayerStorage::_get_row(std::string buf[], int maxCol, const char* param, const char* filter, int* affect_rows) {
	auto sql = std::format("SELECT {} FROM character WHERE {}", param, filter);
	auto reader = _db.CreateCommand(sql).ExecuteReader();
	if (!reader.Read()) {
		if (affect_rows) {
			*affect_rows = 0;
		}
		return true;
	}
	for (int i = 0; i < reader.GetColumnCount() && i < maxCol; i++) {
		buf[i] = reader.GetString(i);
	}
	if (affect_rows) {
		*affect_rows = 1;
	}
	return true;
}

bool PlayerStorage::_get_row2(const char* sql, std::string buf[], int maxCol, int* rows_got) {
	auto reader = _db.CreateCommand(sql).ExecuteReader();
	if (!reader.Read()) {
		if (rows_got) {
			*rows_got = 0;
		}
		return true;
	}
	for (int i = 0; i < reader.GetColumnCount() && i < maxCol; i++) {
		buf[i] = reader.GetString(i);
	}
	if (rows_got) {
		*rows_got = 1;
	}
	return true;
}

bool PlayerStorage::_get_row3(std::string buf[], int maxCol, const char* param, const char* filter, int* affect_rows) {
	return _get_row(buf, maxCol, param, filter, affect_rows);
}

bool PlayerStorage::begin_tran() {
	SQLSetConnectAttr(_db.GetHandle(), SQL_ATTR_AUTOCOMMIT, reinterpret_cast<SQLPOINTER>(SQL_AUTOCOMMIT_OFF),
					  SQL_IS_UINTEGER);
	return true;
}

bool PlayerStorage::commit_tran() {
	SQLEndTran(SQL_HANDLE_DBC, _db.GetHandle(), SQL_COMMIT);
	SQLSetConnectAttr(_db.GetHandle(), SQL_ATTR_AUTOCOMMIT, reinterpret_cast<SQLPOINTER>(SQL_AUTOCOMMIT_ON),
					  SQL_IS_UINTEGER);
	return true;
}

bool PlayerStorage::rollback() {
	SQLEndTran(SQL_HANDLE_DBC, _db.GetHandle(), SQL_ROLLBACK);
	SQLSetConnectAttr(_db.GetHandle(), SQL_ATTR_AUTOCOMMIT, reinterpret_cast<SQLPOINTER>(SQL_AUTOCOMMIT_ON),
					  SQL_IS_UINTEGER);
	return true;
}

bool PlayerStorage::getalldata(const char* sql, std::vector<std::vector<std::string>>& data) {
	auto reader = _db.CreateCommand(sql).ExecuteReader();
	while (reader.Read()) {
		std::vector<std::string> row;
		for (int i = 0; i < reader.GetColumnCount(); i++) {
			row.push_back(reader.GetString(i));
		}
		data.push_back(std::move(row));
	}
	return true;
}

//-------------------
//
//-------------------
BOOL PlayerStorage::VerifyName(const char* pszName) {
	string buf[1];
	char param[] = "atorNome";
	char filter[80];
	sprintf(filter, "atorNome='%s'", pszName);
	bool ret = _get_row(buf, 1, param, filter);
	int r1 = get_affected_rows();
	if (ret && r1 > 0) {
		return TRUE;
	}
	return FALSE;
}

std::string PlayerStorage::GetName(int cha_id) {
	string buf[1];
	const auto param = "atorNome";
	char filter[80];
	sprintf(filter, "atorID = %d", cha_id);
	bool ret = _get_row(buf, 1, param, filter);
	return ret ? buf[0] : "";
}

#define defKITBAG_DATA_STRING_LEN	8192
#define defSKILLBAG_DATA_STRING_LEN	1500
#define defSHORTCUT_DATA_STRING_LEN	1500
#define defSSTATE_DATE_STRING_LIN	1024

const int g_cnCol = 64;
string g_buf[g_cnCol];
char g_sql[1024 * 1024]{};
char g_kitbag[defKITBAG_DATA_STRING_LEN] = {};
char g_kitbagTmp[defKITBAG_DATA_STRING_LEN] = {};
char g_equip[defKITBAG_DATA_STRING_LEN] = {};
char g_look[defLOOK_DATA_STRING_LEN] = {};
char g_skillbag[defSKILLBAG_DATA_STRING_LEN] = {};
char g_shortcut[defSHORTCUT_DATA_STRING_LEN] = {};
char g_skillstate[defSSTATE_DATE_STRING_LIN] = {};

// Add by lark.li 20080723 begin
char g_extendAttr[ROLE_MAXSIZE_DBMISCOUNT];
// End

//
char g_szMisInfo[ROLE_MAXSIZE_DBMISSION];
char g_szRecord[ROLE_MAXSIZE_DBRECORD];
char g_szTrigger[ROLE_MAXSIZE_DBTRIGGER];
char g_szMisCount[ROLE_MAXSIZE_DBMISCOUNT];

CGameDB game_db;

bool PlayerStorage::Init(void) {
	sprintf(g_sql, "select \
				atorID, atorNome, motto, icon, version, pk_ctrl, endeMem, ato_id, guild_id, guild_stat, guild_permission, job, degree, exp, \
				hp, sp, ap, tp, bomd, str, dex, agi, con, sta, luk, sail_lv, sail_exp, sail_left_exp, live_lv, live_exp, map, main_map, map_x, map_y, radius, \
				angle, olhe, skillbag, shortcut, mission, misrecord, mistrigger, miscount, birth, login_cha, live_tp, bank, \
				delflag, operdate, skill_state, kitbag, kitbag_tmp, kb_locked, credit, store_item \
				from %s \
				(nolock) where 1=2",
			_get_table());
	if (strlen(g_sql) >= SQL_MAXLEN) {
		//FILE	*pf = fopen("log\\SQL.txt", "a+");
		FILE* pf = fopen("log\\SQLsentence_length_slopover.txt", "a+");
		if (pf) {
			fprintf(pf, "%s\n\n", g_sql);
			fclose(pf);
		}
		//LG("enter_map", "SQL!\n");
		ToLogService("map", "SQL sentence length slop over");
		return false;
	}
	short sExec = exec_sql_direct(g_sql);
	if (!DBOK(sExec)) {
		//MessageBox(0, "(character)", "", MB_OK);
		char buffer[255];
		sprintf(buffer, RES_STRING(GM_GAMEDB_CPP_00001), "character");
		MessageBox(0, buffer, RES_STRING(GM_GAMEDB_CPP_00002), MB_OK);
		return false;
	}

	return true;
}

bool PlayerStorage::ShowExpRank(CCharacter* pCha, int count) {
	bool ret = false;


	const char* sql_syntax =
		"select top %d atorNome,job,degree from %s where delflag =0 ORDER BY CASE WHEN (exp < 0) THEN (exp+4294967296) ELSE exp END DESC";
	char sql[SQL_MAXLEN];
	sprintf(sql, sql_syntax, count, _get_table());

	SQLRETURN sqlret;
	SQLHSTMT hstmt = SQL_NULL_HSTMT;
	SQLSMALLINT col_num = 0;
	bool found = true;

	try {
		sqlret = SQLAllocHandle(SQL_HANDLE_STMT, _db.GetHandle(), &hstmt);
		if ((sqlret != SQL_SUCCESS) && (sqlret != SQL_SUCCESS_WITH_INFO)) {
			handle_err(_db.GetHandle(), SQL_HANDLE_DBC, sqlret);

			throw 1;
		}

		sqlret = SQLExecDirect(hstmt, (SQLCHAR*)sql, SQL_NTS);
		if (sqlret != SQL_SUCCESS) {
			handle_err(hstmt, SQL_HANDLE_STMT, sqlret);

			if (sqlret != SQL_SUCCESS_WITH_INFO)
				throw 2;
		}

		sqlret = SQLNumResultCols(hstmt, &col_num);
		col_num = min(col_num, MAX_COL);
		col_num = min(col_num, _max_col);

		// Bind Column
		for (int i = 0; i < col_num; ++i) {
			SQLBindCol(hstmt, UWORD(i + 1), SQL_C_CHAR, _buf[i], MAX_DATALEN, &_buf_len[i]);
		}

		//      count-first
		struct RankRow {
			char name[MAX_DATALEN];
			char job[MAX_DATALEN];
			short level;
		};
		std::vector<RankRow> rows;

		for (; (sqlret = SQLFetch(hstmt)) == SQL_SUCCESS || sqlret == SQL_SUCCESS_WITH_INFO;) {
			if (sqlret != SQL_SUCCESS) {
				handle_err(hstmt, SQL_HANDLE_STMT, sqlret);
			}

			RankRow r;
			strncpy(r.name, (char const*)_buf[0], MAX_DATALEN - 1);
			r.name[MAX_DATALEN - 1] = '\0';
			strncpy(r.job, (char const*)_buf[1], MAX_DATALEN - 1);
			r.job[MAX_DATALEN - 1] = '\0';
			r.level = (short)atol((char const*)_buf[2]);
			rows.push_back(r);
		}

		//  :
		{
			net::msg::McShowRankingMessage msg;
			msg.entries.reserve(rows.size());
			for (size_t i = 0; i < rows.size(); ++i)
				msg.entries.push_back({rows[i].name, rows[i].job, static_cast<int64_t>(rows[i].level), 0, 0});
			auto l_wpk = net::msg::serialize(msg);
			pCha->ReflectINFof(pCha, l_wpk);
		}

		SQLFreeStmt(hstmt, SQL_UNBIND);
		ret = true;
	}
	catch (int& e) {
		ToLogService("common", "consult apply consortia process memeberODBC interface transfer error,position ID:{}",
					 e);
	}
	catch (...) {
		ToLogService("common", "Unknown Exception raised when list rank");
	}

	if (hstmt != SQL_NULL_HSTMT) {
		SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
		hstmt = SQL_NULL_HSTMT;
	}

	return ret;
}

//-----------------------
//
//-----------------------
bool PlayerStorage::ReadAllData(CPlayer& player, DWORD atorID) {
	CCharacter* pCha = player.GetMainCha();
	if (!pCha || (player.GetDBChaId() != atorID)) {
		ToLogService("map", "Loading database error: Main character is inexistence or not matching.");
		return false;
	}

	auto row = _characters.FindOne("atorID = ?", static_cast<int>(atorID));
	if (!row) {
		ToLogService("map", "Loading database error: no character row for atorID {}", atorID);
		return false;
	}

	// Идентификация и гильдия
	player.SetDBActId(row->ato_id);
	pCha->SetGuildState(row->guild_stat);
	pCha->SetGuildID(row->guild_id);

	// Боевые характеристики
	pCha->setAttr(ATTR_HP, row->hp, 1);
	pCha->setAttr(ATTR_SP, row->sp, 1);
	pCha->setAttr(ATTR_CEXP, row->exp, 1);

	// Позиционирование
	pCha->SetRadius(row->radius);
	pCha->SetAngle(row->angle);

	// Имя и внешность
	pCha->SetName(row->atorNome.c_str());
	auto logName = std::format("Cha-{}+{}", pCha->GetName(), pCha->GetID());
	pCha->SetLogName(logName.c_str());
	pCha->SetMotto(row->motto.c_str());
	pCha->SetIcon(row->icon);

	// Версия и PK
	long lVer = row->version;
	if (pCha->getAttr(ATTR_HP) < 0) {
		lVer = defCHA_TABLE_NEW_VER;
	}
	pCha->SetPKCtrl(row->pk_ctrl);
	strcpy(pCha->m_CChaAttr.m_szName, pCha->GetName());

	// Уровень, класс, статы
	pCha->setAttr(ATTR_LV, row->degree, 1);
	pCha->setAttr(ATTR_JOB, g_GetJobID(row->job.c_str()), 1);
	pCha->setAttr(ATTR_GD, row->bomd, 1);
	pCha->setAttr(ATTR_AP, row->ap, 1);
	pCha->setAttr(ATTR_TP, row->tp, 1);
	pCha->setAttr(ATTR_BSTR, row->str, 1);
	pCha->setAttr(ATTR_BDEX, row->dex, 1);
	pCha->setAttr(ATTR_BAGI, row->agi, 1);
	pCha->setAttr(ATTR_BCON, row->con, 1);
	pCha->setAttr(ATTR_BSTA, row->sta, 1);
	pCha->setAttr(ATTR_BLUK, row->luk, 1);

	// Мореплавание
	pCha->setAttr(ATTR_SAILLV, row->sail_lv, 1);
	pCha->setAttr(ATTR_CSAILEXP, row->sail_exp, 1);
	pCha->setAttr(ATTR_CLEFT_SAILEXP, row->sail_left_exp, 1);

	// Профессии
	pCha->setAttr(ATTR_LIFELV, row->live_lv, 1);
	pCha->setAttr(ATTR_CLIFEEXP, row->live_exp, 1);
	pCha->setAttr(ATTR_LIFETP, row->live_tp, 1);

	// Карта и позиция
	pCha->SetBirthMap(row->main_map.c_str());
	pCha->SetPos(row->map_x, row->map_y);
	pCha->SetBirthCity(row->birth.c_str());

	// Внешность (olhe)
	auto olheData = row->olhe;
	try {
		if (!pCha->String2LookDate(olheData)) {
			ToLogService("errors", LogLevel::Error,
						 "character (dbid {} name {} resid {}) appearance checksum error.",
						 atorID, pCha->GetLogName(), pCha->GetKitbagRecDBID());
			return false;
		}
		pCha->SetCat(pCha->m_SChaPart.sTypeID);
	}
	catch (...) {
		ToLogService("map", "String2LookDate error! Appearance: {}", olheData);
		throw;
	}

	// Скиллбаг и шорткаты
	auto skillbagData = row->skillbag;
	String2SkillBagData(&pCha->m_CSkillBag, skillbagData);

	auto shortcutData = row->shortcut;
	String2ShortcutData(&pCha->m_CShortcut, shortcutData);

	// Миссии
	player.MisClear();

	auto missionStr = row->mission;
	if (!player.MisInit(missionStr.data())) {
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00009));
	}

	auto misrecordStr = row->misrecord;
	if (!player.MisInitRecord(misrecordStr.data())) {
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00010));
	}

	auto mistriggerStr = row->mistrigger;
	if (!player.MisInitTrigger(mistriggerStr.data())) {
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00011));
	}

	auto miscountStr = row->miscount;
	if (!player.MisInitMissionCount(miscountStr.data())) {
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00012));
	}

	// Логин-персонаж
	std::string strList[2];
	Util_ResolveTextLine(row->login_cha.c_str(), strList, 2, ',');
	player.SetLoginCha(Str2Int(strList[0]), Str2Int(strList[1]));

	// Kitbag и ресурсы
	pCha->SetKitbagRecDBID(Str2Int(row->kitbag));
	pCha->SetKitbagTmpRecDBID(row->kitbag_tmp);

	// Карта и состояние
	player.SetMapMaskDBID(Str2Int(row->map_mask));
	g_strChaState[0] = row->skill_state;

	auto bankData = row->bank;
	player.Strin2BankDBIDData(bankData);

	// Kitbag lock, кредиты, магазин
	pCha->m_CKitbag.SetPwdLockState(row->kb_locked);
	pCha->SetCredit(row->credit);
	pCha->SetStoreItemID(row->store_item);

	// Расширенные атрибуты
	auto extendData = row->extend;
	Strin2ChaExtendAttr(pCha, extendData);

	// Гильдейские права, чат, IMP
	pCha->guildPermission = row->guild_permission;
	pCha->chatColour = row->chatColour;
	pCha->SetIMP(row->IMP);

	ToLogService("map", "Character data loaded for atorID {}", atorID);
	return true;
}

//-----------------
//
//-----------------
bool PlayerStorage::SaveAllData(CPlayer* pPlayer, char chSaveType) {
	if (!pPlayer || !pPlayer->IsValid()) return false;
	CCharacter* pCha = pPlayer->GetMainCha();
	if (!pCha) return false;
	DWORD atorID = pPlayer->GetDBChaId();

	CCharacter* pCCtrlCha = pPlayer->GetCtrlCha();
	if (pPlayer->GetLoginChaType() == enumLOGIN_CHA_BOAT) //
	{
		CCharacter* pCLogCha = pPlayer->GetBoat(pPlayer->GetLoginChaID());
		if (pCLogCha != pCCtrlCha) //
		{
			pCCtrlCha->SetToMainCha();
			pCCtrlCha = pCha;
			if (pCLogCha)
				//LG("", " %s %s %s.\n", pCLogCha->GetLogName(), pCCtrlCha->GetLogName(), pCha->GetLogName());
				ToLogService("errors", LogLevel::Error, "logging character {},control character {}Main character {}.",
							 pCLogCha->GetLogName(), pCCtrlCha->GetLogName(), pCha->GetLogName());
			else
				//LG("", " %s %s %s.\n", "", pCCtrlCha->GetLogName(), pCha->GetLogName());
				ToLogService("errors", LogLevel::Error, "logging character {},control character {}Main character {}.",
							 "", pCCtrlCha->GetLogName(), pCha->GetLogName());
			return false;
		}
	}
	else {
		if (pCha != pCCtrlCha) //
		{
			pCCtrlCha = pCha;
			//LG("", " %s %s %s.\n", pCCtrlCha->GetLogName(), pCCtrlCha->GetLogName(), pCha->GetLogName());
			ToLogService("errors", LogLevel::Error, "logging character {},control character {}Main character {}.",
						 pCCtrlCha->GetLogName(), pCCtrlCha->GetLogName(), pCha->GetLogName());
			return false;
		}
	}

	if (pCha) {
		//LG("enter_map", "%s .\n", pCha->GetLogName());
		ToLogService("map", "{} start configure save data.", pCha->GetLogName());
	}

	//char	szSaveCha[256] = "";
	char szSaveCha[256];
	strncpy(szSaveCha, RES_STRING(GM_GAMEDB_CPP_00013), 256 - 1);

	//char	szSaveChaFile[256] = "log\\.log";
	char szSaveChaFile[256];
	strncpy(szSaveChaFile, RES_STRING(GM_GAMEDB_CPP_00014), 256 - 1);

	char szLogMsg[1024] = "";
	//FILE	*fp;
	//if (!(fp = fopen(szSaveChaFile, "r")))
	//	LG(szSaveCha, "\t\t\t\t\tSQL\tSQL[()]\t\t\n");
	//if (fp)
	//	fclose(fp);
	DWORD dwNowTick = GetTickCount();
	DWORD dwOldTick;
	DWORD dwTotalTick = 0;

	DWORD hp = (long)pCha->getAttr(ATTR_HP);
	DWORD sp = (long)pCha->getAttr(ATTR_SP);
	DWORD exp = (long)pCha->getAttr(ATTR_CEXP);

	const char* map = pCCtrlCha->GetBirthMap();
	const char* main_map = pCha->GetBirthMap();
	DWORD map_x = pCha->GetShape().centre.x;
	DWORD map_y = pCha->GetShape().centre.y;
	DWORD radius = pCha->GetShape().radius;
	short angle = pCha->GetAngle();
	short degree = (short)pCha->getAttr(ATTR_LV);
	const char* job = g_GetJobName((short)pCha->getAttr(ATTR_JOB));
	DWORD bomd = (long)pCha->getAttr(ATTR_GD);
	DWORD ap = (long)pCha->getAttr(ATTR_AP);
	DWORD tp = (long)pCha->getAttr(ATTR_TP);
	DWORD str = (long)pCha->getAttr(ATTR_BSTR);
	DWORD dex = (long)pCha->getAttr(ATTR_BDEX);
	DWORD agi = (long)pCha->getAttr(ATTR_BAGI);
	DWORD con = (long)pCha->getAttr(ATTR_BCON);
	DWORD sta = (long)pCha->getAttr(ATTR_BSTA);
	DWORD luk = (long)pCha->getAttr(ATTR_BLUK);

	DWORD sail_lv = (long)pCha->getAttr(ATTR_SAILLV);
	DWORD sail_exp = (long)pCha->getAttr(ATTR_CSAILEXP);
	DWORD sail_left_exp = (long)pCha->getAttr(ATTR_CLEFT_SAILEXP);
	DWORD live_lv = (long)pCha->getAttr(ATTR_LIFELV);
	DWORD live_exp = (long)pCha->getAttr(ATTR_CLIFEEXP);
	DWORD live_tp = (long)pCha->getAttr(ATTR_LIFETP);

	DWORD nLocked = pCha->m_CKitbag.GetPwdLockState();

	DWORD dwCredit = (long)pCha->GetCredit();
	DWORD dwStoreItemID = pCha->GetStoreItemID();

	int chaIMP = pCha->GetIMP();

	char pk_ctrl = pCha->IsInPK();
	dwOldTick = dwNowTick;
	dwNowTick = GetTickCount();
	dwTotalTick += dwNowTick - dwOldTick;
	sprintf(szLogMsg + strlen(szLogMsg), "%4u", dwNowTick - dwOldTick);
	//LG("enter_map", ".\n");

	std::string g_lookStr;
	if (!LookData2String(pCha->m_SChaPart, g_lookStr)) {
		ToLogService("map", "character {}\tsave data (surface) error!", pCha->GetLogName());
		return false;
	}
	strncpy(g_look, g_lookStr.c_str(), defLOOK_DATA_STRING_LEN - 1);
	g_look[defLOOK_DATA_STRING_LEN - 1] = '\0';
	//LG("enter_map", ".\n");

	dwOldTick = dwNowTick;
	dwNowTick = GetTickCount();
	dwTotalTick += dwNowTick - dwOldTick;
	sprintf(szLogMsg + strlen(szLogMsg), "\t%4u", dwNowTick - dwOldTick);

	g_skillbag[0] = 0;
	if (!SkillBagData2String(&pCha->m_CSkillBag, g_skillbag, defSKILLBAG_DATA_STRING_LEN)) {
		//LG("enter_map", "%s\t!\n", pCha->GetLogName());
		ToLogService("map", "character {}\tsave data(skill) error!", pCha->GetLogName());
		return false;
	}
	//LG("enter_map", ".\n");

	dwOldTick = dwNowTick;
	dwNowTick = GetTickCount();
	dwTotalTick += dwNowTick - dwOldTick;
	sprintf(szLogMsg + strlen(szLogMsg), "\t%4u", dwNowTick - dwOldTick);

	g_shortcut[0] = 0;
	if (!ShortcutData2String(&pCha->m_CShortcut, g_shortcut, defSHORTCUT_DATA_STRING_LEN)) {
		//LG("enter_map", "%s\t!\n", pCha->GetLogName());
		ToLogService("map", "character {}\tsave data(shortcut)error!", pCha->GetLogName());
		return false;
	}
	//LG("enter_map", ".\n");

	dwOldTick = dwNowTick;
	dwNowTick = GetTickCount();
	dwTotalTick += dwNowTick - dwOldTick;
	sprintf(szLogMsg + strlen(szLogMsg), "\t%4u", dwNowTick - dwOldTick);

	//
	memset(g_szMisInfo, 0, ROLE_MAXSIZE_DBMISSION);
	if (!pPlayer->MisGetData(g_szMisInfo, ROLE_MAXSIZE_DBMISSION - 1)) {
		//pCha->SystemNotice( "!ID = %d", pCha->GetID() );
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00015), pCha->GetID());
		//LG(szDBLog, "[ID: %d\tNAME: %s]!ID = %d\n", atorID, pCha->GetName(), pCha->GetID() );
		ToLogService("db", LogLevel::Error,
					 "save character[ID: {}\tNAME: {}]data info, Get mission data error! ID = {}", atorID,
					 pCha->GetName(), pCha->GetID());
	}
	//LG("enter_map", "1.\n");

	memset(g_szRecord, 0, ROLE_MAXSIZE_DBRECORD);
	if (!pPlayer->MisGetRecord(g_szRecord, ROLE_MAXSIZE_DBRECORD - 1)) {
		//pCha->SystemNotice( "!ID = %d", pCha->GetID() );
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00015), pCha->GetID());
		//LG(szDBLog, "[ID: %d\tNAME: %s]!ID = %d\n", atorID, pCha->GetName(), pCha->GetID() );
		ToLogService("db", LogLevel::Error,
					 "save character[ID: {}\tNAME: {}]data info, Get mission history data error! ID = {}", atorID,
					 pCha->GetName(), pCha->GetID());
	}
	//LG("enter_map", "2.\n");

	memset(g_szTrigger, 0, ROLE_MAXSIZE_DBTRIGGER);
	if (!pPlayer->MisGetTrigger(g_szTrigger, ROLE_MAXSIZE_DBTRIGGER - 1)) {
		//pCha->SystemNotice( "!ID = %d", pCha->GetID() );
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00016), pCha->GetID());
		//LG(szDBLog, "[ID: %d\tNAME: %s]!ID = %d\n", atorID, pCha->GetName(), pCha->GetID() );
		ToLogService("db", LogLevel::Error,
					 "save character[ID: {}\tNAME: {}]data info, Get mission trigger data error! ID = {}", atorID,
					 pCha->GetName(), pCha->GetID());
	}
	//LG("enter_map", "3.\n");

	memset(g_szMisCount, 0, ROLE_MAXSIZE_DBMISCOUNT);
	if (!pPlayer->MisGetMissionCount(g_szMisCount, ROLE_MAXSIZE_DBMISCOUNT - 1)) {
		//pCha->SystemNotice( "!ID = %d", pCha->GetID() );
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00017), pCha->GetID());
		//LG(szDBLog, "[ID: %d\tNAME: %s]!ID = %d\n", atorID, pCha->GetName(), pCha->GetID() );
		ToLogService("db", LogLevel::Error,
					 "save character[ID: {}\tNAME: {}]data info, Get randomicity mission take count of data error! ID = {}",
					 atorID, pCha->GetName(), pCha->GetID());
	}
	//LG("enter_map", "4.\n");

	const char* szBirthName = pCha->GetBirthCity();
	//dwOldTick = dwNowTick;
	//dwNowTick = GetTickCount();
	//dwTotalTick += dwNowTick - dwOldTick;

	char szLoginCha[50];
	sprintf(szLoginCha, "%u,%u", pPlayer->GetLoginChaType(), pPlayer->GetLoginChaID());

	if (chSaveType == enumSAVE_TYPE_OFFLINE) //
	{
		SStateData2String(pCha, g_skillstate, defSSTATE_DATE_STRING_LIN, chSaveType);
	}
	else if (!SStateData2String(pCha, g_skillstate, defSSTATE_DATE_STRING_LIN, chSaveType)) {
		ToLogService("map", "character {}\tsave data(shortcut)error!", pCha->GetLogName());
		return false;
	}

	// Add by lark.li 20080723 begin
	memset(g_extendAttr, 0, ROLE_MAXSIZE_DBMISCOUNT);
	if (!ChaExtendAttr2String(pCha, g_extendAttr, ROLE_MAXSIZE_DBMISCOUNT)) {
		ToLogService("map", "character {}\tsave data (extend attr) error!", pCha->GetLogName());
		return false;
	}

	// End
	char str_exp[32];
	_i64toa(exp, str_exp, 10); // C4996

	bool bWithPos = false;
	if (pCCtrlCha->GetSubMap())
		bWithPos = pCCtrlCha->GetSubMap()->CanSavePos();
	if (bWithPos)
		sprintf(g_sql, "update %s set \
					hp=%d, sp=%d, exp=%s, map='%s', main_map='%s', map_x=%d, map_y=%d, radius=%d, angle=%d, pk_ctrl=%d, degree=%d, job='%s', bomd=%d, ap=%d, tp=%d, str=%d, dex=%d, agi=%d, con=%d, sta=%d, luk=%d, olhe='%s', skillbag='%s', \
					shortcut='%s', mission='%s', misrecord='%s', mistrigger='%s', miscount='%s', birth='%s', login_cha='%s', \
					sail_lv=%d, sail_exp=%d, sail_left_exp=%d, live_lv=%d, live_exp=%d, live_tp=%d, kb_locked=%d, credit=%d, store_item=%d, skill_state='%s', extend ='%s', IMP = '%d' \
					where atorID=%d",
				_get_table(),
				hp, sp, str_exp, map, main_map, map_x, map_y, radius, angle, pk_ctrl, degree, job, bomd, ap, tp, str,
				dex, agi, con, sta, luk, g_look, g_skillbag, g_shortcut, g_szMisInfo, g_szRecord, g_szTrigger,
				g_szMisCount, szBirthName, szLoginCha, sail_lv, sail_exp, sail_left_exp, live_lv, live_exp, live_tp,
				nLocked, dwCredit, dwStoreItemID, g_skillstate, g_extendAttr, chaIMP,
				atorID);
	else
		sprintf(g_sql, "update %s set \
					hp=%d, sp=%d, exp=%s, radius=%d, pk_ctrl=%d, degree=%d, job='%s', bomd=%d, ap=%d, tp=%d, str=%d, dex=%d, agi=%d, con=%d, sta=%d, luk=%d, olhe='%s', skillbag='%s', \
					shortcut='%s', mission='%s', misrecord='%s', mistrigger='%s', miscount='%s', birth='%s', login_cha='%s', \
					sail_lv=%d, sail_exp=%d, sail_left_exp=%d, live_lv=%d, live_exp=%d, live_tp=%d, kb_locked=%d, credit=%d, store_item=%d, skill_state='%s', extend ='%s', IMP = '%d' \
					where atorID=%d",
				_get_table(),
				hp, sp, str_exp, radius, pk_ctrl, degree, job, bomd, ap, tp, str, dex, agi, con, sta, luk, g_look,
				g_skillbag, g_shortcut, g_szMisInfo, g_szRecord, g_szTrigger, g_szMisCount, szBirthName, szLoginCha,
				sail_lv, sail_exp, sail_left_exp, live_lv, live_exp, live_tp, nLocked, dwCredit, dwStoreItemID,
				g_skillstate, g_extendAttr, chaIMP,
				atorID);
	//LG("enter_map", "SQL.\n");
	//dwOldTick = dwNowTick;
	//dwNowTick = GetTickCount();
	//dwTotalTick += dwNowTick - dwOldTick;

	if (strlen(g_sql) >= SQL_MAXLEN) {
		//FILE	*pf = fopen("log\\SQL.txt", "a+");
		FILE* pf = fopen("log\\SQLsentence_length_slopover.txt", "a+");

		if (pf) {
			fprintf(pf, "%s\n\n", g_sql);
			fclose(pf);
		}
		//LG("enter_map", "SQL!\n");
		ToLogService("map", "SQL sentence length slop over");
		return false;
	}
	short sExec = exec_sql_direct(g_sql);
	if (!DBOK(sExec)) {
		//LG("enter_map", "%s\tSQL!\n", pCha->GetLogName());
		ToLogService("map", "character {}\tcarry out SQL sentence error!", pCha->GetLogName());
		return false;
	}
	if (DBNODATA(sExec)) {
		//LG("enter_map", "%u!\n", atorID);
		ToLogService("map", "Database couldn't find the character {}!", atorID);
		return false;
	}

	//game_db.UpdateIMP(pPlayer);

	//LG("enter_map", "SQL.\n");

	dwOldTick = dwNowTick;
	dwNowTick = GetTickCount();
	dwTotalTick += dwNowTick - dwOldTick;
	sprintf(szLogMsg + strlen(szLogMsg), "\t%7u[%10u]", dwNowTick - dwOldTick, (unsigned long)strlen(g_sql));
	sprintf(szLogMsg + strlen(szLogMsg), "\t%6u", dwTotalTick);
	sprintf(szLogMsg + strlen(szLogMsg), "\t%s\n", pCha->GetLogName());
	//LG(szSaveCha, szLogMsg);

	//pCha->SystemNotice(" %d %s [%d,%d] %s.\n", pCha->m_CChaAttr.GetAttr(ATTR_LV), pCha->GetBirthMap(), pCha->GetPos().x, pCha->GetPos().y, pCha->GetBirthCity());
	//LG("enter_map", ".\n", pCha->GetLogName());
	ToLogService("map", "save the main character whole data succeed!", pCha->GetLogName());

	return true;
}

bool PlayerStorage::SavePos(CPlayer* pPlayer) {
	if (!pPlayer || !pPlayer->IsValid()) return false;
	CCharacter* pCha = pPlayer->GetMainCha();
	CCharacter* pCCtrlCha = pPlayer->GetCtrlCha();
	if (!pCha || !pCCtrlCha) return false;
	DWORD atorID = pPlayer->GetDBChaId();

	sprintf(g_sql, "update %s set \
				map='%s', main_map='%s', map_x=%d, map_y=%d, angle=%d \
				where atorID=%d",
			_get_table(),
			pCCtrlCha->GetBirthMap(), pCha->GetBirthMap(), pCha->GetPos().x, pCha->GetPos().y, pCha->GetAngle(),
			atorID);

	if (strlen(g_sql) >= SQL_MAXLEN) {
		//FILE	*pf = fopen("log\\SQL.txt", "a+");
		FILE* pf = fopen("log\\SQLsentence_length_slopover.txt", "a+");
		if (pf) {
			fprintf(pf, "%s\n\n", g_sql);
			fclose(pf);
		}
		//LG("enter_map", "SQL!\n");
		ToLogService("map", "SQL sentence length slop over");

		return false;
	}
	short sExec = exec_sql_direct(g_sql);

	if (!DBOK(sExec)) {
		//LG("enter_map", "%s\tSQL!\n", pCha->GetLogName());
		ToLogService("map", "character {}\tcarry out save position SQL sentence error!", pCha->GetLogName());
		return false;
	}
	if (DBNODATA(sExec)) {
		//LG("enter_map", "%u!\n", atorID);
		ToLogService("map", "Database couldn't find the character{}!", atorID);
		return false;
	}

	return true;
}

bool PlayerStorage::SaveMoney(CPlayer* pPlayer) {
	if (!pPlayer || !pPlayer->IsValid()) return false;
	CCharacter* pCha = pPlayer->GetMainCha();
	if (!pCha) return false;
	DWORD atorID = pPlayer->GetDBChaId();

	sprintf(g_sql, "update %s set \
				bomd=%d \
				where atorID=%d",
			_get_table(),
			(int)pCha->getAttr(ATTR_GD),
			atorID);

	if (strlen(g_sql) >= SQL_MAXLEN) {
		//FILE	*pf = fopen("log\\SQL.txt", "a+");
		FILE* pf = fopen("log\\SQLsentence_length_slopover.txt", "a+");
		if (pf) {
			fprintf(pf, "%s\n\n", g_sql);
			fclose(pf);
		}
		//LG("enter_map", "SQL!\n");
		ToLogService("map", "SQL sentence length slop over");
		return false;
	}
	short sExec = exec_sql_direct(g_sql);

	if (!DBOK(sExec)) {
		//LG("enter_map", "%s\tSQL!\n", pCha->GetLogName());
		ToLogService("map", "character {}\tcarry out save money SQL sentence error!", pCha->GetLogName());
		return false;
	}
	if (DBNODATA(sExec)) {
		//LG("enter_map", "%u!\n", atorID);
		ToLogService("map", "Database couldn't find the character{}!", atorID);
		return false;
	}

	return true;
}

bool PlayerStorage::SaveKBagDBID(CPlayer* pPlayer) {
	if (!pPlayer || !pPlayer->IsValid()) return false;
	CCharacter* pCha = pPlayer->GetMainCha();
	if (!pCha) return false;
	DWORD atorID = pPlayer->GetDBChaId();

	sprintf(g_sql, "update %s set \
				kitbag=%d \
				where atorID=%d",
			_get_table(),
			pCha->GetKitbagRecDBID(),
			atorID);

	if (strlen(g_sql) >= SQL_MAXLEN) {
		//FILE	*pf = fopen("log\\SQL.txt", "a+");
		FILE* pf = fopen("log\\SQLsentence_length_slopover.txt", "a+");
		if (pf) {
			fprintf(pf, "%s\n\n", g_sql);
			fclose(pf);
		}
		//LG("enter_map", "SQL!\n");
		ToLogService("map", "SQL sentence length slop over");
		return false;
	}
	short sExec = exec_sql_direct(g_sql);

	if (!DBOK(sExec)) {
		//LG("enter_map", "%s\tSQL!\n", pCha->GetLogName());
		ToLogService("map", "character{}\tcarry out save kitbag indexical SQL sentence error!", pCha->GetLogName());
		return false;
	}
	if (DBNODATA(sExec)) {
		//LG("enter_map", "%u!\n", atorID);
		ToLogService("map", "Database couldn't find the character{}!", atorID);
		return false;
	}

	return true;
}

bool PlayerStorage::SaveKBagTmpDBID(CPlayer* pPlayer) {
	if (!pPlayer || !pPlayer->IsValid()) return false;
	CCharacter* pCha = pPlayer->GetMainCha();
	if (!pCha) return false;
	DWORD atorID = pPlayer->GetDBChaId();

	sprintf(g_sql, "update %s set \
				kitbag_tmp=%d \
				where atorID=%d",
			_get_table(),
			pCha->GetKitbagTmpRecDBID(),
			atorID);

	if (strlen(g_sql) >= SQL_MAXLEN) {
		//FILE	*pf = fopen("log\\SQL.txt", "a+");
		FILE* pf = fopen("log\\SQLsentence_length_slopover.txt", "a+");
		if (pf) {
			fprintf(pf, "%s\n\n", g_sql);
			fclose(pf);
		}
		//LG("enter_map", "SQL!\n");
		ToLogService("map", "SQL sentence length slop over");
		return false;
	}
	short sExec = exec_sql_direct(g_sql);

	if (!DBOK(sExec)) {
		//LG("enter_map", "%s\tSQL!\n", pCha->GetLogName());
		ToLogService("map", "character {}\tcarry out save temp kitbag indexical SQL sentence error!",
					 pCha->GetLogName());
		return false;
	}
	if (DBNODATA(sExec)) {
		//LG("enter_map", "%u!\n", atorID);
		ToLogService("map", "Database couldn't find the character{}!", atorID);
		return false;
	}

	return true;
}

bool PlayerStorage::SaveKBState(CPlayer* pPlayer) {
	if (!pPlayer || !pPlayer->IsValid()) return false;
	CCharacter* pCha = pPlayer->GetMainCha();
	if (!pCha) return false;
	DWORD atorID = pPlayer->GetDBChaId();

	int iLocked = pCha->m_CKitbag.GetPwdLockState();
	sprintf(g_sql, "update %s set \
				kb_locked=%d \
				where atorID=%d",
			_get_table(),
			iLocked,
			atorID);

	if (strlen(g_sql) >= SQL_MAXLEN) {
		//FILE	*pf = fopen("log\\SQL.txt", "a+");
		FILE* pf = fopen("log\\SQLsentence_length_slopover.txt", "a+");
		if (pf) {
			fprintf(pf, "%s\n\n", g_sql);
			fclose(pf);
		}
		//LG("enter_map", "SQL!\n");
		ToLogService("map", "SQL sentence length slop over");
		return false;
	}
	short sExec = exec_sql_direct(g_sql);

	if (!DBOK(sExec)) {
		//LG("enter_map", "%s\tSQL!\n", pCha->GetLogName());
		ToLogService("map", "character {}\tcarry out save kitbag lock state SQL sentence error!", pCha->GetLogName());
		return false;
	}
	if (DBNODATA(sExec)) {
		//LG("enter_map", "%u!\n", atorID);
		ToLogService("map", "Database couldn't find the character{}!", atorID);
		return false;
	}

	return true;
}

BOOL PlayerStorage::SaveStoreItemID(DWORD atorID, long lStoreItemID) {
	if (atorID == 0) {
		return false;
	}

	sprintf(g_sql, "update %s set \
				   store_item=%d \
				   where atorID=%d",
			_get_table(),
			lStoreItemID,
			atorID);

	if (strlen(g_sql) >= SQL_MAXLEN) {
		//FILE	*pf = fopen("log\\SQL.txt", "a+");
		FILE* pf = fopen("log\\SQLsentence_length_slopover.txt", "a+");
		if (pf) {
			fprintf(pf, "%s\n\n", g_sql);
			fclose(pf);
		}
		return false;
	}
	short sExec = exec_sql_direct(g_sql);

	if (!DBOK(sExec)) {
		return false;
	}
	if (DBNODATA(sExec)) {
		return false;
	}

	return true;
}

BOOL PlayerStorage::AddMoney(DWORD atorID, long money) {
	if (atorID == 0) {
		return false;
	}

	sprintf(g_sql, "update %s set \
				   bomd=bomd+%d \
				   where atorID=%d",
			_get_table(),
			money,
			atorID);

	if (strlen(g_sql) >= SQL_MAXLEN) {
		//FILE	*pf = fopen("log\\SQL.txt", "a+");
		FILE* pf = fopen("log\\SQLsentence_length_slopover.txt", "a+");
		if (pf) {
			fprintf(pf, "%s\n\n", g_sql);
			fclose(pf);
		}
		return false;
	}
	short sExec = exec_sql_direct(g_sql);

	if (!DBOK(sExec)) {
		return false;
	}
	if (DBNODATA(sExec)) {
		return false;
	}

	return true;
}

BOOL PlayerStorage::AddCreditByDBID(DWORD atorID, long lCredit) {
	if (atorID == 0) {
		return false;
	}

	sprintf(g_sql, "update %s set \
				   credit=credit+%d \
				   where atorID=%d",
			_get_table(),
			lCredit,
			atorID);

	if (strlen(g_sql) >= SQL_MAXLEN) {
		//FILE	*pf = fopen("log\\SQL.txt", "a+");
		FILE* pf = fopen("log\\SQLsentence_length_slopover.txt", "a+");
		if (pf) {
			fprintf(pf, "%s\n\n", g_sql);
			fclose(pf);
		}
		return false;
	}
	short sExec = exec_sql_direct(g_sql);

	if (!DBOK(sExec)) {
		return false;
	}
	if (DBNODATA(sExec)) {
		return false;
	}

	return true;
}

BOOL PlayerStorage::IsChaOnline(DWORD atorID, BOOL& bOnline) {
	if (atorID == 0) {
		return false;
	}

	BOOL ret = false;

	long lMemAddr = 0;

	const char* sql_syntax = "select endeMem from %s where atorID=%d";
	char sql[SQL_MAXLEN];
	sprintf(sql, sql_syntax, _get_table(), atorID);

	//
	SQLRETURN sqlret;
	SQLHSTMT hstmt = SQL_NULL_HSTMT;
	SQLSMALLINT col_num = 0;
	bool found = true;

	try {
		sqlret = SQLAllocHandle(SQL_HANDLE_STMT, _db.GetHandle(), &hstmt);
		if ((sqlret != SQL_SUCCESS) && (sqlret != SQL_SUCCESS_WITH_INFO)) {
			handle_err(_db.GetHandle(), SQL_HANDLE_DBC, sqlret);

			throw 1;
		}

		sqlret = SQLExecDirect(hstmt, (SQLCHAR*)sql, SQL_NTS);
		if (sqlret != SQL_SUCCESS) {
			handle_err(hstmt, SQL_HANDLE_STMT, sqlret);

			if (sqlret != SQL_SUCCESS_WITH_INFO)
				throw 2;
		}

		sqlret = SQLNumResultCols(hstmt, &col_num);
		col_num = min(col_num, MAX_COL);
		col_num = min(col_num, _max_col);

		// Bind Column
		for (int i = 0; i < col_num; ++i) {
			SQLBindCol(hstmt, UWORD(i + 1), SQL_C_CHAR, _buf[i], MAX_DATALEN, &_buf_len[i]);
		}

		// Fetch each Row	int i; //
		for (int f_row = 0; (sqlret = SQLFetch(hstmt)) == SQL_SUCCESS || sqlret == SQL_SUCCESS_WITH_INFO; ++f_row) {
			if (sqlret != SQL_SUCCESS) {
				handle_err(hstmt, SQL_HANDLE_STMT, sqlret);
			}

			lMemAddr = atol((char const*)_buf[0]);
		}
		SQLFreeStmt(hstmt, SQL_UNBIND);
		ret = true;
	}
	catch (int& e) {
		//LG("Store_msg", "IsChaOnline ODBC %d\n",e);
		ToLogService("store", "IsChaOnline ODBC interface transfer error ,position ID{}", e);
	}
	catch (...) {
		ToLogService("store", "Unknown Exception raised when IsChaOnline");
	}

	if (hstmt != SQL_NULL_HSTMT) {
		SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
		hstmt = SQL_NULL_HSTMT;
	}

	if (lMemAddr > 0) {
		bOnline = true;
	}
	else {
		bOnline = false;
	}

	return ret;
}

Long PlayerStorage::GetChaAddr(DWORD atorID) {
	if (atorID == 0)
		return false;

	long lMemAddr = 0;
	const char* sql_syntax = "select endeMem from %s where atorID=%d";
	char sql[SQL_MAXLEN];
	sprintf(sql, sql_syntax, _get_table(), atorID);

	SQLRETURN sqlret;
	SQLHSTMT hstmt = SQL_NULL_HSTMT;
	SQLSMALLINT col_num = 0;
	bool found = true;

	try {
		sqlret = SQLAllocHandle(SQL_HANDLE_STMT, _db.GetHandle(), &hstmt);
		if ((sqlret != SQL_SUCCESS) && (sqlret != SQL_SUCCESS_WITH_INFO)) {
			handle_err(_db.GetHandle(), SQL_HANDLE_DBC, sqlret);

			throw 1;
		}
		sqlret = SQLExecDirect(hstmt, (SQLCHAR*)sql, SQL_NTS);
		if (sqlret != SQL_SUCCESS) {
			handle_err(hstmt, SQL_HANDLE_STMT, sqlret);

			if (sqlret != SQL_SUCCESS_WITH_INFO)
				throw 2;
		}
		sqlret = SQLNumResultCols(hstmt, &col_num);
		col_num = min(col_num, MAX_COL);
		col_num = min(col_num, _max_col);
		for (int i = 0; i < col_num; ++i) {
			SQLBindCol(hstmt, UWORD(i + 1), SQL_C_CHAR, _buf[i], MAX_DATALEN, &_buf_len[i]);
		}
		for (int f_row = 0; (sqlret = SQLFetch(hstmt)) == SQL_SUCCESS || sqlret == SQL_SUCCESS_WITH_INFO; ++f_row) {
			if (sqlret != SQL_SUCCESS) {
				handle_err(hstmt, SQL_HANDLE_STMT, sqlret);
			}

			lMemAddr = atol((char const*)_buf[0]);
		}
		SQLFreeStmt(hstmt, SQL_UNBIND);
	}
	catch (int& e) {
		ToLogService("store", "IsChaOnline ODBC interface transfer error ,position ID{}", e);
	}
	catch (...) {
		ToLogService("store", "Unknown Exception raised when IsChaOnline");
	}
	if (hstmt != SQL_NULL_HSTMT) {
		SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
		hstmt = SQL_NULL_HSTMT;
	}
	return lMemAddr;
}

bool PlayerStorage::SaveMMaskDBID(CPlayer* pPlayer) {
	if (!pPlayer || !pPlayer->IsValid()) return false;
	DWORD atorID = pPlayer->GetDBChaId();

	sprintf(g_sql, "update %s set \
				map_mask=%d \
				where atorID=%d",
			_get_table(),
			pPlayer->GetMapMaskDBID(),
			atorID);

	if (strlen(g_sql) >= SQL_MAXLEN) {
		//FILE	*pf = fopen("log\\SQL.txt", "a+");
		FILE* pf = fopen("log\\SQLsentence_length_slopover.txt", "a+");
		if (pf) {
			fprintf(pf, "%s\n\n", g_sql);
			fclose(pf);
		}
		//LG("enter_map", "SQL!\n");
		ToLogService("map", "SQL sentence length slop over");
		return false;
	}
	short sExec = exec_sql_direct(g_sql);

	if (!DBOK(sExec)) {
		//LG("enter_map", "%d\tSQL!\n", pPlayer->GetDBChaId());
		ToLogService("map", "character {}\tcarry out save big map indexical SQL senternce error!",
					 pPlayer->GetDBChaId());
		return false;
	}
	if (DBNODATA(sExec)) {
		//LG("enter_map", "%u!\n", atorID);
		ToLogService("map", "Database couldn't find the character{}!", atorID);
		return false;
	}

	return true;
}

bool PlayerStorage::SaveBankDBID(CPlayer* pPlayer) {
	if (!pPlayer || !pPlayer->IsValid()) return false;
	DWORD atorID = pPlayer->GetDBChaId();

	const short csIDBufLen = 200;
	char szIDBuf[csIDBufLen];
	if (!pPlayer->BankDBIDData2String(szIDBuf, csIDBufLen))
		return false;

	sprintf(g_sql, "update %s set \
				bank=%s \
				where atorID=%d",
			_get_table(),
			szIDBuf,
			atorID);

	if (strlen(g_sql) >= SQL_MAXLEN) {
		//FILE	*pf = fopen("log\\SQL.txt", "a+");
		FILE* pf = fopen("log\\SQLsentence_length_slopover.txt", "a+");
		if (pf) {
			fprintf(pf, "%s\n\n", g_sql);
			fclose(pf);
		}
		//LG("enter_map", "SQL!\n");
		ToLogService("map", "SQL sentence length slop over");

		return false;
	}
	short sExec = exec_sql_direct(g_sql);

	if (!DBOK(sExec)) {
		//LG("enter_map", "%d\tSQL!\n", pPlayer->GetDBChaId());
		ToLogService("map", "character {}\tcarry out save bank indexcial SQL sentence error!", pPlayer->GetDBChaId());
		return false;
	}
	if (DBNODATA(sExec)) {
		//LG("enter_map", "%u!\n", atorID);
		ToLogService("map", "Database couldn't find the character{}!", atorID);
		return false;
	}

	return true;
}

bool PlayerStorage::SaveTableVer(DWORD atorID) {
	sprintf(g_sql, "update %s set \
				version=%d \
				where atorID=%d",
			_get_table(),
			defCHA_TABLE_NEW_VER,
			atorID);

	short sExec = exec_sql_direct(g_sql);

	return DBOK(sExec) && !DBNODATA(sExec);
}

BOOL PlayerStorage::SaveMissionData(CPlayer* pPlayer, DWORD atorID) {
	if (!pPlayer) return FALSE;
	CCharacter* pCha = pPlayer->GetMainCha();
	if (!pCha) return FALSE;

	//
	memset(g_szMisInfo, 0, ROLE_MAXSIZE_DBMISSION);
	if (!pPlayer->MisGetData(g_szMisInfo, ROLE_MAXSIZE_DBMISSION - 1)) {
		//pCha->SystemNotice( "SaveMissionData:!ID = %d", pCha->GetID() );
		//LG(szDBLog, "SaveMissionData:[ID: %d\tNAME: %s]!ID = %d\n", atorID, pCha->GetName(), pCha->GetID() );
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00018), pCha->GetID());
		ToLogService("db", LogLevel::Error,
					 "SaveMissionData: save character[ID: {}\tNAME: {}]data info, Get mission data error! ID = {}",
					 atorID, pCha->GetName(), pCha->GetID());
	}

	memset(g_szRecord, 0, ROLE_MAXSIZE_DBRECORD);
	if (!pPlayer->MisGetRecord(g_szRecord, ROLE_MAXSIZE_DBRECORD - 1)) {
		//pCha->SystemNotice( "SaveMissionData:!ID = %d", pCha->GetID() );
		//LG(szDBLog, "SaveMissionData:[ID: %d\tNAME: %s]!ID = %d\n", atorID, pCha->GetName(), pCha->GetID() );
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00018), pCha->GetID());
		ToLogService("db", LogLevel::Error,
					 "SaveMissionData: save character[ID: {}\tNAME: {}]data info, Get mission history data error! ID = {}",
					 atorID, pCha->GetName(), pCha->GetID());
	}

	memset(g_szTrigger, 0, ROLE_MAXSIZE_DBTRIGGER);
	if (!pPlayer->MisGetTrigger(g_szTrigger, ROLE_MAXSIZE_DBTRIGGER - 1)) {
		//pCha->SystemNotice( "SaveMissionData:!ID = %d", pCha->GetID() );
		//LG(szDBLog, "SaveMissionData:[ID: %d\tNAME: %s]!ID = %d\n", atorID, pCha->GetName(), pCha->GetID() );
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00019), pCha->GetID());
		ToLogService("db", LogLevel::Error,
					 "SaveMissionData: save character[ID: {}\tNAME: {}]data info, Get mission trigger data error! ID = {}",
					 atorID, pCha->GetName(), pCha->GetID());
	}

	sprintf(g_sql, "update %s set mission='%s', misrecord='%s', mistrigger='%s' \
		where atorID=%d", _get_table(), g_szMisInfo, g_szRecord, g_szTrigger, atorID);

	if (strlen(g_sql) >= SQL_MAXLEN) {
		//FILE	*pf = fopen("log\\SQL.txt", "a+");
		FILE* pf = fopen("log\\SQLsentence_length_slopover.txt", "a+");
		if (pf) {
			fprintf(pf, "%s\n\n", g_sql);
			fclose(pf);
		}
		//LG("enter_map", "SQL!\n");
		ToLogService("map", "SQL sentence length slop over");
		return FALSE;
	}
	short sExec = exec_sql_direct(g_sql);
	return DBOK(sExec) && !DBNODATA(sExec);
}

// Add by lark.li 20080521 begin

// End


// === CTableResource (OdbcDatabase) ===

bool CTableResource::Create(long& lDBID, long lChaId, long lTypeId) {
	try {
		_db.CreateCommand("INSERT INTO resource (atorID, type_id) VALUES (?, ?)")
		   .SetParam(1, lChaId).SetParam(2, lTypeId).ExecuteNonQuery();
		auto idStr = _db.CreateCommand("SELECT @@IDENTITY").ExecuteScalar();
		lDBID = std::stol(idStr);
		return true;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "CTableResource::Create failed: {}", e.what());
		return false;
	}
}

bool CTableResource::ReadKitbagData(CCharacter* pCha) {
	if (!pCha) {
		ToLogService("map", "ReadKitbagData: character is null");
		return false;
	}
	if (pCha->GetKitbagRecDBID() == 0) {
		long lDBID;
		if (!Create(lDBID, pCha->GetPlayer()->GetDBChaId(), enumRESDB_TYPE_KITBAG)) return false;
		pCha->SetKitbagRecDBID(lDBID);
	}
	try {
		auto reader = _db.CreateCommand("SELECT atorID, type_id, content FROM resource WHERE id = ?")
						 .SetParam(1, pCha->GetKitbagRecDBID()).ExecuteReader();
		if (reader.Read()) {
			auto dwChaId = static_cast<DWORD>(reader.GetInt(0));
			auto chType = reader.GetInt(1);
			if (dwChaId != pCha->GetPlayer()->GetDBChaId() || chType != enumRESDB_TYPE_KITBAG) {
				ToLogService("map", "ReadKitbagData: character mismatch");
				return false;
			}
			auto content = reader.GetString(2);
			if (!pCha->String2KitbagData(content)) {
				ToLogService("errors", LogLevel::Error, "character({}) kitbag data(resource_id {}) checksum error",
							 pCha->GetLogName(), pCha->GetKitbagRecDBID());
				return false;
			}
		}
		else {
			ToLogService("map", "ReadKitbagData: no data for id {}", pCha->GetKitbagRecDBID());
			return false;
		}
		return true;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "ReadKitbagData failed: {}", e.what());
		return false;
	}
}

bool CTableResource::SaveKitbagData(CCharacter* pCha) {
	if (!pCha || !pCha->IsValid()) return false;
	g_kitbag[0] = 0;
	if (!KitbagData2String(&pCha->m_CKitbag, g_kitbag, defKITBAG_DATA_STRING_LEN)) {
		ToLogService("map", "character {}\tsave kitbag error!", pCha->GetLogName());
		return false;
	}
	try {
		_db.CreateCommand("UPDATE resource SET content = ? WHERE id = ?")
		   .SetParam(1, std::string_view(g_kitbag))
		   .SetParam(2, pCha->GetKitbagRecDBID()).ExecuteNonQuery();
		return true;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "SaveKitbagData failed: {}", e.what());
		return false;
	}
}

bool CTableResource::ReadKitbagTmpData(CCharacter* pCha) {
	if (!pCha) {
		ToLogService("map", "ReadKitbagTmpData: character is null");
		return false;
	}
	if (pCha->GetKitbagTmpRecDBID() == 0) {
		long lDBID;
		if (!Create(lDBID, pCha->GetPlayer()->GetDBChaId(), enumRESDB_TYPE_KITBAGTMP)) return false;
		pCha->SetKitbagTmpRecDBID(lDBID);
	}
	try {
		auto reader = _db.CreateCommand("SELECT atorID, type_id, content FROM resource WHERE id = ?")
						 .SetParam(1, pCha->GetKitbagTmpRecDBID()).ExecuteReader();
		if (reader.Read()) {
			auto dwChaId = static_cast<DWORD>(reader.GetInt(0));
			auto chType = reader.GetInt(1);
			if (dwChaId != pCha->GetPlayer()->GetDBChaId() || chType != enumRESDB_TYPE_KITBAGTMP) {
				ToLogService("map", "ReadKitbagTmpData: character mismatch");
				return false;
			}
			auto content = reader.GetString(2);
			if (!pCha->String2KitbagTmpData(content)) {
				ToLogService("errors", LogLevel::Error, "character({}) temp kitbag data(resource_id {}) checksum error",
							 pCha->GetLogName(), pCha->GetKitbagTmpRecDBID());
				return false;
			}
		}
		else {
			ToLogService("map", "ReadKitbagTmpData: no data for id {}", pCha->GetKitbagTmpRecDBID());
			return false;
		}
		return true;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "ReadKitbagTmpData failed: {}", e.what());
		return false;
	}
}

bool CTableResource::SaveKitbagTmpData(CCharacter* pCha) {
	if (!pCha || !pCha->IsValid()) return false;
	g_kitbagTmp[0] = 0;
	if (!KitbagData2String(pCha->m_pCKitbagTmp, g_kitbagTmp, defKITBAG_DATA_STRING_LEN)) {
		ToLogService("map", "character {}\tsave temp kitbag error!", pCha->GetLogName());
		return false;
	}
	try {
		_db.CreateCommand("UPDATE resource SET content = ? WHERE id = ?")
		   .SetParam(1, std::string_view(g_kitbagTmp))
		   .SetParam(2, pCha->GetKitbagTmpRecDBID()).ExecuteNonQuery();
		return true;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "SaveKitbagTmpData failed: {}", e.what());
		return false;
	}
}

bool CTableResource::ReadBankData(CPlayer* pCPly, char chBankNO) {
	if (!pCPly) {
		ToLogService("map", "ReadBankData: player is null");
		return false;
	}
	if (pCPly->GetCurBankNum() == 0) {
		long lDBID;
		if (!Create(lDBID, pCPly->GetDBChaId(), enumRESDB_TYPE_BANK)) return false;
		pCPly->AddBankDBID(lDBID);
	}
	char chStart = (chBankNO < 0) ? 0 : chBankNO;
	char chEnd = (chBankNO < 0) ? (pCPly->GetCurBankNum() - 1) : chBankNO;
	if (chBankNO >= 0 && chBankNO >= pCPly->GetCurBankNum()) return false;

	try {
		for (char i = chStart; i <= chEnd; i++) {
			auto reader = _db.CreateCommand("SELECT atorID, type_id, content FROM resource WHERE id = ?")
							 .SetParam(1, pCPly->GetBankDBID(i)).ExecuteReader();
			if (reader.Read()) {
				auto dwChaId = static_cast<DWORD>(reader.GetInt(0));
				auto chType = reader.GetInt(1);
				if (dwChaId != pCPly->GetDBChaId() || chType != enumRESDB_TYPE_BANK) {
					ToLogService("map", "ReadBankData: character mismatch");
					return false;
				}
				auto content = reader.GetString(2);
				if (!pCPly->String2BankData(i, content)) {
					ToLogService("errors", LogLevel::Error, "player ({}) bank data(resource_id {}) checksum error",
								 pCPly->GetDBChaId(), pCPly->GetBankDBID(i));
					return false;
				}
			}
			else {
				ToLogService("map", "ReadBankData: no data for id {}", pCPly->GetBankDBID(i));
				return false;
			}
		}
		return true;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "ReadBankData failed: {}", e.what());
		return false;
	}
}

bool CTableResource::SaveBankData(CPlayer* pCPly, char chBankNO) {
	if (!pCPly || !pCPly->IsValid()) return false;
	if (pCPly->GetCurBankNum() == 0) return true;

	char chStart = (chBankNO < 0) ? 0 : chBankNO;
	char chEnd = (chBankNO < 0) ? (pCPly->GetCurBankNum() - 1) : chBankNO;
	if (chBankNO >= 0 && chBankNO >= pCPly->GetCurBankNum()) return false;

	try {
		for (char i = chStart; i <= chEnd; i++) {
			if (!pCPly->BankWillSave(i)) continue;
			pCPly->SetBankSaveFlag(i, false);
			g_kitbag[0] = 0;
			if (!KitbagData2String(pCPly->GetBank(i), g_kitbag, defKITBAG_DATA_STRING_LEN)) {
				ToLogService("map", "bank {}\tsave error!", pCPly->GetBankDBID(i));
				return false;
			}
			_db.CreateCommand("UPDATE resource SET content = ? WHERE id = ?")
			   .SetParam(1, std::string_view(g_kitbag))
			   .SetParam(2, pCPly->GetBankDBID(i)).ExecuteNonQuery();
		}
		return true;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "SaveBankData failed: {}", e.what());
		return false;
	}
}


// === CTableMapMask (OdbcDatabase) ===

bool CTableMapMask::GetColNameByMapName(const char* szMapName, std::string& colName) {
	if (!szMapName) return false;
	if (!strcmp(szMapName, "garner")) {
		colName = "content1";
		return true;
	}
	if (!strcmp(szMapName, "magicsea")) {
		colName = "content2";
		return true;
	}
	if (!strcmp(szMapName, "darkblue")) {
		colName = "content3";
		return true;
	}
	if (!strcmp(szMapName, "winterland")) {
		colName = "content4";
		return true;
	}
	return false;
}

bool CTableMapMask::Create(long& lDBID, long lChaId) {
	try {
		_db.CreateCommand("INSERT INTO map_mask (atorID) VALUES (?)").SetParam(1, lChaId).ExecuteNonQuery();
		auto idStr = _db.CreateCommand("SELECT @@IDENTITY").ExecuteScalar();
		lDBID = std::stol(idStr);
		return true;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "CTableMapMask::Create failed: {}", e.what());
		return false;
	}
}

bool CTableMapMask::ReadData(CPlayer* pCPly) {
	if (!pCPly || !pCPly->IsValid()) {
		ToLogService("map", "Load map_mask error: player is null");
		return false;
	}
	if (pCPly->GetMapMaskDBID() == 0) {
		long lDBID;
		if (!Create(lDBID, pCPly->GetDBChaId())) return false;
		pCPly->SetMapMaskDBID(lDBID);
	}

	std::string colName;
	if (!GetColNameByMapName(pCPly->GetMaskMapName(), colName)) {
		ToLogService("map", "map_mask: unknown map name");
		return false;
	}

	try {
		// colName из фиксированного набора (content1..4), безопасно вставлять в SQL
		auto sql = std::format("SELECT atorID, {} FROM map_mask WHERE id = ?", colName);
		auto reader = _db.CreateCommand(sql).SetParam(1, pCPly->GetMapMaskDBID()).ExecuteReader();
		if (reader.Read()) {
			auto dbChaId = static_cast<DWORD>(reader.GetInt(0));
			if (dbChaId != pCPly->GetDBChaId()) {
				ToLogService("map", "map_mask: character mismatch");
				return false;
			}
			pCPly->SetMapMaskBase64(reader.GetString(1).c_str());
		}
		else {
			ToLogService("map", "map_mask: no data for id {}", pCPly->GetMapMaskDBID());
			return false;
		}
		return true;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "CTableMapMask::ReadData failed: {}", e.what());
		return false;
	}
}

bool CTableMapMask::SaveData(CPlayer* pCPly, BOOL bDirect) {
	if (!pCPly || !pCPly->IsValid()) return false;

	std::string colName;
	if (!GetColNameByMapName(pCPly->GetMaskMapName(), colName)) {
		ToLogService("map", "map_mask: unknown map name");
		return false;
	}

	// colName из фиксированного набора (content1..4), безопасно
	auto sql = std::format("UPDATE map_mask SET {} = ? WHERE id = ?", colName);

	if (!bDirect) {
		// Отложенное сохранение — собираем SQL в очередь
		// Для отложенного нужно подставить значения сразу (очередь хранит готовые SQL)
		auto fullSql = std::format("UPDATE map_mask SET {} = '{}' WHERE id = {}",
								   colName, pCPly->GetMapMaskBase64(), pCPly->GetMapMaskDBID());
		_saveQueue.push_back(std::move(fullSql));
	}
	else {
		try {
			_db.CreateCommand(sql)
			   .SetParam(1, std::string_view(pCPly->GetMapMaskBase64()))
			   .SetParam(2, pCPly->GetMapMaskDBID()).ExecuteNonQuery();
		}
		catch (const OdbcException& e) {
			ToLogService("db", LogLevel::Error, "CTableMapMask::SaveData failed: {}", e.what());
			return false;
		}
	}
	return true;
}

void CTableMapMask::HandleSaveList() {
	DWORD dwTick = GetTickCount();
	static DWORD g_dwLastSaveTick = 0;

	if ((dwTick - g_dwLastSaveTick) > 2000) {
		g_dwLastSaveTick = dwTick;
		if (_saveQueue.empty()) return;

		try {
			_db.CreateCommand(_saveQueue.front()).ExecuteNonQuery();
		}
		catch (const OdbcException& e) {
			ToLogService("db", LogLevel::Error, "CTableMapMask::HandleSaveList failed: {}", e.what());
		}
		_saveQueue.pop_front();
	}
}

void CTableMapMask::SaveAll() {
	for (auto& sql : _saveQueue) {
		try {
			_db.CreateCommand(sql).ExecuteNonQuery();
		}
		catch (const OdbcException& e) {
			ToLogService("db", LogLevel::Error, "CTableMapMask::SaveAll failed: {}", e.what());
		}
	}
	ToLogService("map", "MapMask SaveAll: {} queries executed", _saveQueue.size());
	_saveQueue.clear();
}


// === CTableBoat (OdbcDatabase) ===

BOOL CTableBoat::Create(DWORD& dwBoatID, const BOAT_DATA& Data) {
	try {
		std::string strKitbag;
		KitbagStringConv(Data.sCapacity, strKitbag);
		SYSTEMTIME st;
		GetLocalTime(&st);
		auto timeStr = std::format("{}-{}-{} {}:{}:{}", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

		_db.CreateCommand("INSERT INTO boat (boat_name, boat_berth, boat_boatid, boat_header, boat_body, "
			   "boat_engine, boat_cannon, boat_equipment, boat_bag, boat_diecount, boat_isdead, boat_ownerid, boat_createtime, boat_isdeleted) "
			   "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, 0, 0, ?, ?, 0)")
		   .SetParam(1, std::string_view(Data.szName))
		   .SetParam(2, Data.sBerth).SetParam(3, Data.sBoat)
		   .SetParam(4, Data.sHeader).SetParam(5, Data.sBody)
		   .SetParam(6, Data.sEngine).SetParam(7, Data.sCannon)
		   .SetParam(8, Data.sEquipment).SetParam(9, std::string_view(strKitbag))
		   .SetParam(10, Data.dwOwnerID).SetParam(11, std::string_view(timeStr))
		   .ExecuteNonQuery();
		auto idStr = _db.CreateCommand("SELECT @@IDENTITY").ExecuteScalar();
		dwBoatID = static_cast<DWORD>(std::stol(idStr));
		return TRUE;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "CTableBoat::Create failed: {}", e.what());
		return FALSE;
	}
}

BOOL CTableBoat::GetBoat(CCharacter& Boat) {
	DWORD dwBoatID = (DWORD)Boat.getAttr(ATTR_BOAT_DBID);
	try {
		auto reader = _db.CreateCommand(
							 "SELECT boat_name, boat_boatid, boat_berth, boat_header, boat_body, "
							 "boat_engine, boat_cannon, boat_equipment, boat_diecount, boat_isdead, "
							 "boat_ownerid, boat_isdeleted, cur_endure, mx_endure, cur_supply, mx_supply, "
							 "skill_state, map, map_x, map_y, angle, degree, exp FROM boat WHERE boat_id = ?")
						 .SetParam(1, dwBoatID).ExecuteReader();

		if (!reader.Read()) return FALSE;

		BOAT_DATA Data{};
		strncpy(Data.szName, reader.GetString(0).c_str(), BOAT_MAXSIZE_BOATNAME - 1);
		Data.sBoat = static_cast<USHORT>(reader.GetInt(1));
		Data.sBerth = static_cast<USHORT>(reader.GetInt(2));
		Data.sHeader = static_cast<USHORT>(reader.GetInt(3));
		Data.sBody = static_cast<USHORT>(reader.GetInt(4));
		Data.sEngine = static_cast<USHORT>(reader.GetInt(5));
		Data.sCannon = static_cast<USHORT>(reader.GetInt(6));
		Data.sEquipment = static_cast<USHORT>(reader.GetInt(7));
		USHORT sDieCount = static_cast<USHORT>(reader.GetInt(8));
		BYTE byIsDead = static_cast<BYTE>(reader.GetInt(9));
		DWORD dwOwnerID = static_cast<DWORD>(reader.GetInt(10));
		BYTE byIsDeleted = static_cast<BYTE>(reader.GetInt(11));

		if (byIsDeleted == 1) {
			ToLogService("errors", LogLevel::Error, "boat({}) ID[0x{:X}] deleted", Data.szName, dwBoatID);
			Boat.SystemNotice(RES_STRING(GM_GAMEDB_CPP_00020), Boat.GetName());
			return FALSE;
		}

		Boat.SetName(Data.szName);
		Boat.setAttr(ATTR_BOAT_BERTH, Data.sBerth, 1);
		Boat.setAttr(ATTR_BOAT_SHIP, Data.sBoat, 1);
		Boat.setAttr(ATTR_BOAT_HEADER, Data.sHeader, 1);
		Boat.setAttr(ATTR_BOAT_BODY, Data.sBody, 1);
		Boat.setAttr(ATTR_BOAT_ENGINE, Data.sEngine, 1);
		Boat.setAttr(ATTR_BOAT_CANNON, Data.sCannon, 1);
		Boat.setAttr(ATTR_BOAT_PART, Data.sEquipment, 1);
		Boat.setAttr(ATTR_BOAT_DIECOUNT, sDieCount, 1);
		Boat.setAttr(ATTR_BOAT_ISDEAD, byIsDead, 1);
		Boat.setAttr(ATTR_HP, reader.GetInt(12), 1);
		Boat.setAttr(ATTR_BMXHP, reader.GetInt(13), 1);
		Boat.setAttr(ATTR_SP, reader.GetInt(14), 1);
		Boat.setAttr(ATTR_BMXSP, reader.GetInt(15), 1);
		g_strChaState[1] = reader.GetString(16);
		Boat.SetBirthMap(reader.GetString(17).c_str());
		Boat.SetPos(reader.GetInt(18), reader.GetInt(19));
		Boat.SetAngle(reader.GetInt(20));
		Boat.setAttr(ATTR_LV, reader.GetInt(21), 1);
		Boat.setAttr(ATTR_CEXP, reader.GetInt(22), 1);
		reader.Close();

		if (!ReadCabin(Boat)) return FALSE;

		SItemGrid* pGridCont = NULL;
		CItemRecord* pItem = NULL;
		Short sPos = 0;
		int i = 0;
		Short sUsedNum = Boat.m_CKitbag.GetUseGridNum();
		while (i < sUsedNum) {
			pGridCont = Boat.m_CKitbag.GetGridContByNum(i);
			if (pGridCont) {
				pItem = GetItemRecordInfo(pGridCont->sID);
				if (pItem && enumITEM_PICKTO_KITBAG == pItem->chPickTo) {
					sPos = Boat.m_CKitbag.GetPosIDByNum(i);
					ToLogService("common", "Character {} Remove {}.", Boat.GetName(), pItem->szName);
					Boat.m_CKitbag.Pop(pGridCont, sPos);
					sUsedNum = Boat.m_CKitbag.GetUseGridNum();
					i = 0;
					continue;
				}
			}
			i++;
		}
		return TRUE;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "GetBoat failed: {}", e.what());
		return FALSE;
	}
}

BOOL CTableBoat::SaveBoatTempData(DWORD dwBoatID, DWORD dwOwnerID, BYTE byIsDeleted) {
	try {
		_db.CreateCommand("UPDATE boat SET boat_ownerid = ?, boat_isdeleted = ? WHERE boat_id = ?")
		   .SetParam(1, dwOwnerID).SetParam(2, static_cast<int>(byIsDeleted)).SetParam(3, dwBoatID).ExecuteNonQuery();
		return TRUE;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "SaveBoatTempData(id) failed: {}", e.what());
		return FALSE;
	}
}

BOOL CTableBoat::SaveBoatDelTag(DWORD dwBoatID, BYTE byIsDeleted) {
	try {
		_db.CreateCommand("UPDATE boat SET boat_isdeleted = ? WHERE boat_id = ?")
		   .SetParam(1, static_cast<int>(byIsDeleted)).SetParam(2, dwBoatID).ExecuteNonQuery();
		return TRUE;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "SaveBoatDelTag failed: {}", e.what());
		return FALSE;
	}
}

BOOL CTableBoat::SaveBoatTempData(CCharacter& Boat, BYTE byIsDeleted) {
	try {
		DWORD dwBoatID = (DWORD)Boat.getAttr(ATTR_BOAT_DBID);
		_db.CreateCommand(
			   "UPDATE boat SET boat_diecount = ?, boat_isdead = ?, boat_ownerid = ?, boat_isdeleted = ? WHERE boat_id = ?")
		   .SetParam(1, static_cast<int>(Boat.getAttr(ATTR_BOAT_DIECOUNT)))
		   .SetParam(2, static_cast<int>(Boat.getAttr(ATTR_BOAT_ISDEAD)))
		   .SetParam(3, Boat.GetPlayer()->GetDBChaId())
		   .SetParam(4, static_cast<int>(byIsDeleted))
		   .SetParam(5, dwBoatID).ExecuteNonQuery();
		return TRUE;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "SaveBoatTempData(cha) failed: {}", e.what());
		return FALSE;
	}
}

BOOL CTableBoat::SaveBoat(CCharacter& Boat, char chSaveType) {
	try {
		DWORD dwBoatID = (DWORD)Boat.getAttr(ATTR_BOAT_DBID);
		bool bWithPos = (chSaveType == enumSAVE_TYPE_OFFLINE || chSaveType == enumSAVE_TYPE_SWITCH);

		g_kitbag[0] = 0;
		KitbagData2String(&Boat.m_CKitbag, g_kitbag, defKITBAG_DATA_STRING_LEN);

		const auto& skillState = g_strChaState[1];

		if (bWithPos) {
			_db.CreateCommand("UPDATE boat SET boat_berth=?, boat_ownerid=?, cur_endure=?, mx_endure=?, "
				   "cur_supply=?, mx_supply=?, skill_state=?, map=?, map_x=?, map_y=?, angle=?, "
				   "degree=?, exp=?, boat_bag=? WHERE boat_id=?")
			   .SetParam(1, static_cast<int>(Boat.getAttr(ATTR_BOAT_BERTH)))
			   .SetParam(2, Boat.GetPlayer()->GetDBChaId())
			   .SetParam(3, static_cast<int>(Boat.getAttr(ATTR_HP)))
			   .SetParam(4, static_cast<int>(Boat.getAttr(ATTR_BMXHP)))
			   .SetParam(5, static_cast<int>(Boat.getAttr(ATTR_SP)))
			   .SetParam(6, static_cast<int>(Boat.getAttr(ATTR_BMXSP)))
			   .SetParam(7, std::string_view(skillState))
			   .SetParam(8, std::string_view(Boat.GetBirthMap()))
			   .SetParam(9, static_cast<int>(Boat.GetPos().x))
			   .SetParam(10, static_cast<int>(Boat.GetPos().y))
			   .SetParam(11, static_cast<int>(Boat.GetAngle()))
			   .SetParam(12, static_cast<int>(Boat.getAttr(ATTR_LV)))
			   .SetParam(13, static_cast<int>(Boat.getAttr(ATTR_CEXP)))
			   .SetParam(14, std::string_view(g_kitbag))
			   .SetParam(15, dwBoatID).ExecuteNonQuery();
		}
		else {
			_db.CreateCommand("UPDATE boat SET boat_berth=?, boat_ownerid=?, cur_endure=?, mx_endure=?, "
				   "cur_supply=?, mx_supply=?, skill_state=?, degree=?, exp=?, boat_bag=? WHERE boat_id=?")
			   .SetParam(1, static_cast<int>(Boat.getAttr(ATTR_BOAT_BERTH)))
			   .SetParam(2, Boat.GetPlayer()->GetDBChaId())
			   .SetParam(3, static_cast<int>(Boat.getAttr(ATTR_HP)))
			   .SetParam(4, static_cast<int>(Boat.getAttr(ATTR_BMXHP)))
			   .SetParam(5, static_cast<int>(Boat.getAttr(ATTR_SP)))
			   .SetParam(6, static_cast<int>(Boat.getAttr(ATTR_BMXSP)))
			   .SetParam(7, std::string_view(skillState))
			   .SetParam(8, static_cast<int>(Boat.getAttr(ATTR_LV)))
			   .SetParam(9, static_cast<int>(Boat.getAttr(ATTR_CEXP)))
			   .SetParam(10, std::string_view(g_kitbag))
			   .SetParam(11, dwBoatID).ExecuteNonQuery();
		}
		return TRUE;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "SaveBoat failed: {}", e.what());
		return FALSE;
	}
}

bool CTableBoat::SaveAllData(CPlayer* pPlayer, char chSaveType) {
	if (!pPlayer) return false;
	for (int i = 0; i < MAX_CHAR_BOAT; i++) {
		CCharacter* pBoat = pPlayer->GetBoat(static_cast<BYTE>(i));
		if (pBoat && (DWORD)pBoat->getAttr(ATTR_BOAT_DBID) != 0) {
			if (!SaveBoat(*pBoat, chSaveType)) return false;
		}
	}
	return true;
}

bool CTableBoat::SaveCabin(CCharacter& Boat, char chSaveType) {
	try {
		DWORD dwBoatID = (DWORD)Boat.getAttr(ATTR_BOAT_DBID);
		g_kitbag[0] = 0;
		KitbagData2String(&Boat.m_CKitbag, g_kitbag, defKITBAG_DATA_STRING_LEN);
		_db.CreateCommand("UPDATE boat SET boat_bag = ? WHERE boat_id = ?")
		   .SetParam(1, std::string_view(g_kitbag)).SetParam(2, dwBoatID).ExecuteNonQuery();
		return true;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "SaveCabin failed: {}", e.what());
		return false;
	}
}

bool CTableBoat::SaveAllCabin(CPlayer* pPlayer, char chSaveType) {
	if (!pPlayer) return false;
	for (int i = 0; i < MAX_CHAR_BOAT; i++) {
		CCharacter* pBoat = pPlayer->GetBoat(static_cast<BYTE>(i));
		if (pBoat && (DWORD)pBoat->getAttr(ATTR_BOAT_DBID) != 0) {
			if (!SaveCabin(*pBoat, chSaveType)) return false;
		}
	}
	return true;
}

bool CTableBoat::ReadCabin(CCharacter& Boat) {
	try {
		DWORD dwBoatID = (DWORD)Boat.getAttr(ATTR_BOAT_DBID);
		auto content = _db.CreateCommand("SELECT boat_bag FROM boat WHERE boat_id = ?")
						  .SetParam(1, dwBoatID).ExecuteScalar();
		if (!content.empty()) {
			std::string data = content;
			if (!Boat.String2KitbagData(data)) {
				ToLogService("errors", LogLevel::Error, "ReadCabin: kitbag checksum error for boat {}", dwBoatID);
				return false;
			}
		}
		return true;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "ReadCabin failed: {}", e.what());
		return false;
	}
}

BOOL CGameDB::Init() {
	m_bInitOK = FALSE;

	static const char* s_szDsn =
		"DRIVER={ODBC Driver 17 for SQL Server};SERVER=localhost;DATABASE=gamedb;Trusted_Connection=Yes;";
	ToLogService("db", "Connecting database [{}]...", s_szDsn);

	try {
		_db.Open(s_szDsn);
		ToLogService("db", "OdbcDatabase connected");
	}
	catch (const OdbcException& e) {
		MessageBox(NULL, "Database Connection Failed!", "Database Connection Error", MB_ICONERROR | MB_OK);
		ToLogService("db", LogLevel::Error, "OdbcDatabase connect failed: {}", e.what());
		return FALSE;
	}

	if (!_tab_cha.Init()) {
		return FALSE;
	}

	m_bInitOK = TRUE;
	return TRUE;
}

bool CGameDB::ReadPlayer(CPlayer* pPlayer, DWORD atorID) {
	if (!_tab_cha.ReadAllData(*pPlayer, atorID))
		return false;

	long lKbDBID = pPlayer->GetMainCha()->GetKitbagRecDBID();
	long lkbTmpDBID = pPlayer->GetMainCha()->GetKitbagTmpRecDBID(); //ID
	long lMMaskDBID = pPlayer->GetMapMaskDBID();
	long lBankNum = pPlayer->GetCurBankNum();
	if (!_tab_res.ReadKitbagData(pPlayer->GetMainCha()))
		return false;
	if (lKbDBID == 0)
		if (!SavePlayerKBagDBID(pPlayer))
			return false;

	if (!_tab_res.ReadKitbagTmpData(pPlayer->GetMainCha()))
		return false;
	if (lkbTmpDBID == 0)
		if (!SavePlayerKBagTmpDBID(pPlayer))
			return false;
	pPlayer->GetMainCha()->LogAssets(enumLASSETS_INIT);

	if (!_tab_res.ReadBankData(pPlayer))
		return false;
	if (lBankNum == 0)
		if (!_tab_cha.SaveBankDBID(pPlayer))
			return false;

	//if (g_Config.m_chMapMask > 0)
	{
		//
		_tab_mmask.ReadData(pPlayer);
		if (lMMaskDBID == 0)
			SavePlayerMMaskDBID(pPlayer);
	}

	// Чтение данных аккаунта через новый ODBC API
	try {
		auto reader = _db.CreateCommand("SELECT jmes, ato_nome, IMP FROM account WHERE ato_id = ?")
						 .SetParam(1, pPlayer->GetDBActId()).ExecuteReader();
		if (reader.Read()) {
			pPlayer->SetGMLev(reader.GetInt(0));
			pPlayer->SetActName(reader.GetString(1).c_str());
			pPlayer->SetIMP(reader.GetInt(2));
		}
		else {
			return false;
		}
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "ReadAllData(account) failed: {}", e.what());
		return false;
	}

	//
	if (pPlayer->m_lGuildID > 0) {
		_tab_gld.GetGuildInfo(pPlayer->GetMainCha(), pPlayer->m_lGuildID);
		//long	lType = _tab_gld.GetTypeByID(pPlayer->GetMainCha()->getAttr(ATTR_GUILD));
		//if (lType >= 0)
		//	pPlayer->GetMainCha()->setAttr(ATTR_GUILD_TYPE, lType, 1);
	}
	//LG("enter_map", ".\n");
	ToLogService("map", "Load the character whole data succeed.");

	//
	CKitbag* pCKb;
	CCharacter* pCMainC = pPlayer->GetMainCha();
	short sItemNum = pCMainC->m_CKitbag.GetUseGridNum();
	g_kitbag[0] = '\0';
	sprintf(g_kitbag, RES_STRING(GM_GAMEDB_CPP_00021), pCMainC->getAttr(ATTR_GD), sItemNum);
	SItemGrid* pGridCont;
	CItemRecord* pCItem;
	pCKb = &(pCMainC->m_CKitbag);
	for (short i = 0; i < sItemNum; i++) {
		pGridCont = pCKb->GetGridContByNum(i);
		if (!pGridCont)
			continue;
		pCItem = GetItemRecordInfo(pGridCont->sID);
		if (!pCItem)
			continue;
		sprintf(g_kitbag + strlen(g_kitbag), "%s[%d],%d;", pCItem->szName.c_str(), pGridCont->sID, pGridCont->sNum);
	}
	ToLogService("trade", "[CHA_ENTER] {} : {}", pCMainC->GetName(), g_kitbag);

	short sItemTmpNum = pCMainC->m_pCKitbagTmp->GetUseGridNum();
	g_kitbagTmp[0] = '\0';
	sprintf(g_kitbagTmp, RES_STRING(GM_GAMEDB_CPP_00022), sItemTmpNum);
	pCKb = pCMainC->m_pCKitbagTmp;
	for (short i = 0; i < sItemTmpNum; i++) {
		pGridCont = pCKb->GetGridContByNum(i);
		if (!pGridCont)
			continue;
		pCItem = GetItemRecordInfo(pGridCont->sID);
		if (!pCItem)
			continue;
		sprintf(g_kitbagTmp + strlen(g_kitbagTmp), "%s[%d],%d;", pCItem->szName.c_str(), pGridCont->sID,
				pGridCont->sNum);
	}
	ToLogService("trade", "[CHA_ENTER] {} : {}", pCMainC->GetName(), g_kitbagTmp);

	char chStart = 0, chEnd = pPlayer->GetCurBankNum() - 1;
	for (char i = chStart; i <= chEnd; i++) {
		sprintf(g_kitbag, RES_STRING(GM_GAMEDB_CPP_00023), i + 1);
		pCKb = pPlayer->GetBank(i);
		sItemNum = pCKb->GetUseGridNum();
		sprintf(g_kitbag + strlen(g_kitbag), "[%d]%d@;", i + 1, sItemNum);
		for (short i = 0; i < sItemNum; i++) {
			pGridCont = pCKb->GetGridContByNum(i);
			if (!pGridCont)
				continue;
			pCItem = GetItemRecordInfo(pGridCont->sID);
			if (!pCItem)
				continue;
			sprintf(g_kitbag + strlen(g_kitbag), "%s[%d],%d;", pCItem->szName.c_str(), pGridCont->sID, pGridCont->sNum);
		}
		ToLogService("trade", "[CHA_ENTER] {} : {}", pCMainC->GetName(), g_kitbag);
	}

	g_equip[0] = '\0';
	sprintf(g_equip, RES_STRING(GM_GAMEDB_CPP_00024), enumEQUIP_NUM);
	for (short i = 0; i < enumEQUIP_NUM; i++) {
		pGridCont = &pCMainC->m_SChaPart.SLink[i];
		if (!pGridCont)
			continue;
		pCItem = GetItemRecordInfo(pGridCont->sID);
		if (!pCItem)
			continue;
		sprintf(g_equip + strlen(g_equip), "%s[%d],%d;", pCItem->szName.c_str(), pGridCont->sID, pGridCont->sNum);
	}
	ToLogService("trade", "[CHA_EQUIP] {} : {}", pCMainC->GetName(), g_equip);

	//
	return true;
}

bool CGameDB::SavePlayer(CPlayer* pPlayer, char chSaveType) {
	if (!pPlayer || !pPlayer->GetMainCha())
		return FALSE;

	if (pPlayer->GetMainCha()->GetPlayer() != pPlayer) {
		//LG("", "Player %p[dbid %u] %sPlayer %p\n",
		ToLogService("errors", LogLevel::Error,
					 "save Player address {}[dbid {}], the character main player {}, the character's Player address {}",
					 static_cast<void*>(pPlayer), pPlayer->GetDBChaId(), pPlayer->GetMainCha()->GetLogName(),
					 static_cast<void*>(pPlayer->GetMainCha()->GetPlayer()));
		//pPlayer->SystemNotice("");
		pPlayer->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00025));
		return FALSE;
	}

	bool bSaveMainCha = false, bSaveBoat = false, bSaveKitBag = false, bSaveMMask = false, bSaveBank = false;
	bool bSaveKitBagTmp = false;
	bool bSaveKBState = false;
	BeginTran();
	try {
		DWORD dwStartTick = GetTickCount();

		bSaveMainCha = _tab_cha.SaveAllData(pPlayer, chSaveType); //
		DWORD dwSaveMainTick = GetTickCount();
		bSaveKitBag = _tab_res.SaveKitbagData(pPlayer->GetMainCha());
		//
		bSaveKitBagTmp = _tab_res.SaveKitbagTmpData(pPlayer->GetMainCha());
		//
		//bSaveKBState = _tab_cha.SaveKBState(pPlayer);
		DWORD dwSaveKbTick = GetTickCount();
		bSaveBank = _tab_res.SaveBankData(pPlayer);
		DWORD dwSaveBankTick = GetTickCount();
		if ((chSaveType != enumSAVE_TYPE_TIMER) && (g_Config.m_chMapMask > 0)) {
			if (pPlayer->IsMapMaskChange()) {
				bSaveMMask = _tab_mmask.SaveData(pPlayer);
				pPlayer->ResetMapMaskChange();
			}
		}
		else
			bSaveMMask = true;
		DWORD dwSaveMMaskTick = GetTickCount();
		bSaveBoat = _tab_boat.SaveAllData(pPlayer, chSaveType); //
		DWORD dwSaveBoatTick = GetTickCount();

		//LG("", "%-8d%-8d%-8d%-8d%-8d%-8d.[%d %s]\n",
		ToLogService(
			"common",
			"totalize {:8}main character {:8}main character kitbag {:8}bank {:8}big map {:8}boat {:8}.[{} {}]",
			dwSaveBoatTick - dwStartTick, dwSaveMainTick - dwStartTick, dwSaveKbTick - dwSaveMainTick,
			dwSaveBankTick - dwSaveKbTick, dwSaveMMaskTick - dwSaveBankTick, dwSaveBoatTick - dwSaveMMaskTick,
			pPlayer->GetDBChaId(), pPlayer->GetMainCha()->GetLogName());
	}
	catch (...) {
		//LG("enter_map", ".\n");
		ToLogService("map", "It's abnormity when saving the character's whole data.");
	}

	if (!bSaveMainCha || !bSaveBoat || !bSaveKitBag
	) {
		RollBack();
		return false;
	}
	CommitTran();

	//LG("enter_map", ".\n");
	ToLogService("map", "save character whole data succeed.");
	//
	if (chSaveType != enumSAVE_TYPE_TIMER) {
		CKitbag* pCKb;
		CCharacter* pCMainC = pPlayer->GetMainCha();
		short sItemNum = pCMainC->m_CKitbag.GetUseGridNum();
		g_kitbag[0] = '\0';
		sprintf(g_kitbag, RES_STRING(GM_GAMEDB_CPP_00026), pCMainC->getAttr(ATTR_GD), sItemNum);
		SItemGrid* pGridCont;
		CItemRecord* pCItem;
		pCKb = &(pCMainC->m_CKitbag);
		for (short i = 0; i < sItemNum; i++) {
			pGridCont = pCKb->GetGridContByNum(i);
			if (!pGridCont)
				continue;
			pCItem = GetItemRecordInfo(pGridCont->sID);
			if (!pCItem)
				continue;
			sprintf(g_kitbag + strlen(g_kitbag), "%s[%d],%d;", pCItem->szName.c_str(), pGridCont->sID, pGridCont->sNum);
		}
		ToLogService("trade", "[CHA_OUT] {} : {}", pCMainC->GetName(), g_kitbag);

		short sItemTmpNum = pCMainC->m_pCKitbagTmp->GetUseGridNum();
		g_kitbagTmp[0] = '\0';
		sprintf(g_kitbagTmp, RES_STRING(GM_GAMEDB_CPP_00022), sItemTmpNum);
		pCKb = pCMainC->m_pCKitbagTmp;
		for (short i = 0; i < sItemTmpNum; i++) {
			pGridCont = pCKb->GetGridContByNum(i);
			if (!pGridCont)
				continue;
			pCItem = GetItemRecordInfo(pGridCont->sID);
			if (!pCItem)
				continue;
			sprintf(g_kitbagTmp + strlen(g_kitbagTmp), "%s[%d],%d;", pCItem->szName.c_str(), pGridCont->sID,
					pGridCont->sNum);
		}
		ToLogService("trade", "[CHA_OUT] {} : {}", pCMainC->GetName(), g_kitbagTmp);

		g_equip[0] = '\0';
		sprintf(g_equip, RES_STRING(GM_GAMEDB_CPP_00024), enumEQUIP_NUM);
		for (short i = 0; i < enumEQUIP_NUM; i++) {
			pGridCont = &pCMainC->m_SChaPart.SLink[i];
			if (!pGridCont)
				continue;
			pCItem = GetItemRecordInfo(pGridCont->sID);
			if (!pCItem)
				continue;
			sprintf(g_equip + strlen(g_equip), "%s[%d],%d;", pCItem->szName.c_str(), pGridCont->sID, pGridCont->sNum);
		}
		ToLogService("trade", "[CHA_EQUIP] {} : {}", pCMainC->GetName(), g_equip);

		char chStart = 0, chEnd = pPlayer->GetCurBankNum() - 1;
		sprintf(g_kitbag, RES_STRING(GM_GAMEDB_CPP_00023), pPlayer->GetCurBankNum());
		for (char i = chStart; i <= chEnd; i++) {
			pCKb = pPlayer->GetBank(i);
			sItemNum = pCKb->GetUseGridNum();
			sprintf(g_kitbag + strlen(g_kitbag), "[%d]%d@;", i + 1, sItemNum);
			for (short i = 0; i < sItemNum; i++) {
				pGridCont = pCKb->GetGridContByNum(i);
				if (!pGridCont)
					continue;
				pCItem = GetItemRecordInfo(pGridCont->sID);
				if (!pCItem)
					continue;
				sprintf(g_kitbag + strlen(g_kitbag), "%s[%d],%d;", pCItem->szName.c_str(), pGridCont->sID,
						pGridCont->sNum);
			}
		}
		ToLogService("trade", "[CHA_BANK] {} : {}", pCMainC->GetName(), g_kitbag);
	}
	//

	return true;
}

/*
//
#include "lua_gamectrl.h"
extern char g_TradeName[][8];
#include "SystemDialog.h"

void CGameDB::Log(const char *type, const char *c1, const char *c2, const char *c3, const char *c4, const char *p, BOOL bAddToList)
{
	if(g_Config.m_bLogDB==FALSE) return;

	if(!_tab_log) return;

	char szSQL[8192];

	sprintf(szSQL, "insert gamelog (action, c1, c2, c3, c4, content) \
				   values('%s', '%s', '%s', '%s', '%s', '%s')", type, c1, c2, c3, c4, p);

	//if(bAddToList)
	{
		// SendMessage, ,
	//	extern HWND g_SysDlg;
	//	PostMessage(g_SysDlg, WM_USER_LOG, 0, 0);
	}
	//else
	{
		ExecLogSQL(szSQL);
	}
}

void CGameDB::Log1(int nType, const char *cha1, const char *cha2, const char *pszContent)
{
	Log( g_TradeName[nType], cha1, "", cha2, "", pszContent);
}


void CGameDB::Log2(int nType, CCharacter *pCha1, CCharacter *pCha2, const char *pszContent)
{
	if(!_tab_log) return;

	char szName1[32]    = "";
	char szName2[32]    = "";
	char szActName1[32] = "";
	char szActName2[32] = "";

	if(pCha1)
	{
		strcpy(szName1, pCha1->GetName());
		if(pCha1->GetPlayer()) strcpy(szActName1, pCha1->GetPlayer()->GetActName());
	}
	if(pCha2)
	{
		strcpy(szName2, pCha2->GetName());
		if(pCha2->GetPlayer()) strcpy(szActName1, pCha2->GetPlayer()->GetActName());
	}

	Log(g_TradeName[nType], szName1, szActName1, szName2, szActName2, pszContent);
}*/

// ============================================================================
// CTableGuild — private хелперы
// ============================================================================

void CTableGuild::handle_err(SQLHANDLE, SQLSMALLINT, RETCODE, const char*, bool) {
}

const char* CTableGuild::_get_table() const {
	return "guild";
}

SQLRETURN CTableGuild::exec_sql_direct(const char* sql) {
	_db.CreateCommand(sql).ExecuteNonQuery();
	return SQL_SUCCESS;
}

int CTableGuild::get_affected_rows() {
	auto r = _db.CreateCommand("SELECT @@ROWCOUNT").ExecuteScalar();
	return r.empty() ? 0 : std::stoi(r);
}

bool CTableGuild::_get_row(std::string buf[], int maxCol, const char* param, const char* filter, int* affect_rows) {
	auto sql = std::format("SELECT {} FROM guild WHERE {}", param, filter);
	auto reader = _db.CreateCommand(sql).ExecuteReader();
	if (!reader.Read()) {
		if (affect_rows) {
			*affect_rows = 0;
		}
		return true;
	}
	for (int i = 0; i < reader.GetColumnCount() && i < maxCol; i++) {
		buf[i] = reader.GetString(i);
	}
	if (affect_rows) {
		*affect_rows = 1;
	}
	return true;
}

bool CTableGuild::_get_row3(std::string buf[], int maxCol, const char* param, const char* filter) {
	return _get_row(buf, maxCol, param, filter);
}

bool CTableGuild::begin_tran() {
	SQLSetConnectAttr(_db.GetHandle(), SQL_ATTR_AUTOCOMMIT, reinterpret_cast<SQLPOINTER>(SQL_AUTOCOMMIT_OFF),
					  SQL_IS_UINTEGER);
	return true;
}

bool CTableGuild::commit_tran() {
	SQLEndTran(SQL_HANDLE_DBC, _db.GetHandle(), SQL_COMMIT);
	SQLSetConnectAttr(_db.GetHandle(), SQL_ATTR_AUTOCOMMIT, reinterpret_cast<SQLPOINTER>(SQL_AUTOCOMMIT_ON),
					  SQL_IS_UINTEGER);
	return true;
}

bool CTableGuild::rollback() {
	SQLEndTran(SQL_HANDLE_DBC, _db.GetHandle(), SQL_ROLLBACK);
	SQLSetConnectAttr(_db.GetHandle(), SQL_ATTR_AUTOCOMMIT, reinterpret_cast<SQLPOINTER>(SQL_AUTOCOMMIT_ON),
					  SQL_IS_UINTEGER);
	return true;
}

bool CTableGuild::getalldata(const char* sql, std::vector<std::vector<std::string>>& data) {
	auto reader = _db.CreateCommand(sql).ExecuteReader();
	while (reader.Read()) {
		std::vector<std::string> row;
		for (int i = 0; i < reader.GetColumnCount(); i++) {
			row.push_back(reader.GetString(i));
		}
		data.push_back(std::move(row));
	}
	return true;
}

//===============CTableGuild Begin===========================================
long CTableGuild::Create(CCharacter* pCha, char* guildname, cChar* passwd) {
	long l_ret_guild_id = 0;
	string buf[1];
	char sql[SQL_MAXLEN];

	while (true) {
		//ID
		const char* param = "isnull(min(guild_id),0)";
		char filter[80];
		sprintf(filter, "guild_id >0 and leader_id =0");
		bool ret = _get_row(buf, 1, param, filter);
		if (!ret) {
			pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00027));
			ToLogService("common", "found consortia system occur SQL operator error.");
			return 0;
		}
		l_ret_guild_id = atol(buf[0].c_str());
		if (!l_ret_guild_id) {
			pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00030));
			return 0;
		}


		sprintf(sql, "update %s set leader_id =%d ,passwd ='%s', guild_name ='%s', exp =0,\
										member_total =1,try_total =0\
								where leader_id =0 and guild_id =%d",
				_get_table(), pCha->GetID(), passwd, guildname, l_ret_guild_id);
		SQLRETURN l_sqlret = exec_sql_direct(sql);
		if (!DBOK(l_sqlret)) //
		{
			//pCha->SystemNotice("");
			pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00031));
			return 0;
		}
		if (get_affected_rows() != 1) {
			continue;
		}


		break;
	}
	sprintf(sql, "update character set guild_id =%d,guild_stat =0,guild_permission =%d \
								where atorID =%d", l_ret_guild_id, emGldPermMax, pCha->GetID());
	exec_sql_direct(sql);

	//  :   (GameServerGroup)
	auto l_wpk = net::msg::serialize(net::msg::GmGuildCreateMessage{
		(int64_t)l_ret_guild_id, guildname, g_GetJobName(uShort(pCha->getAttr(ATTR_JOB))),
		(int64_t)uShort(pCha->getAttr(ATTR_LV))
	});
	pCha->ReflectINFof(pCha, l_wpk);

	return l_ret_guild_id; //,ID
}

bool CTableGuild::ListAll(CCharacter* pCha, char disband_days) {
	const char* sql_syntax = 0;
	if (!pCha || disband_days < 1) {
		return false;
	}
	else {
		sql_syntax =
			"select gld.guild_id, gld.guild_name, gld.motto, gld.leader_id, cha.atorNome leader_name, gld.exp, gld.member_total "
			"from guild As gld, character As cha where gld.leader_id =cha.atorID";
	}
	bool ret = false;
	char sql[SQL_MAXLEN];
	sprintf(sql, sql_syntax, disband_days);

	//
	SQLRETURN sqlret;
	SQLHSTMT hstmt = SQL_NULL_HSTMT;
	SQLSMALLINT col_num = 0;
	bool found = true;

	try {
		sqlret = SQLAllocHandle(SQL_HANDLE_STMT, _db.GetHandle(), &hstmt);
		if ((sqlret != SQL_SUCCESS) && (sqlret != SQL_SUCCESS_WITH_INFO)) {
			handle_err(_db.GetHandle(), SQL_HANDLE_DBC, sqlret);

			throw 1;
		}

		sqlret = SQLExecDirect(hstmt, (SQLCHAR*)sql, SQL_NTS);
		if (sqlret != SQL_SUCCESS) {
			handle_err(hstmt, SQL_HANDLE_STMT, sqlret);

			if (sqlret != SQL_SUCCESS_WITH_INFO)
				throw 2;
		}

		sqlret = SQLNumResultCols(hstmt, &col_num);
		col_num = min(col_num, MAX_COL);
		col_num = min(col_num, _max_col);

		// Bind Column
		for (int i = 0; i < col_num; ++i) {
			SQLBindCol(hstmt, UWORD(i + 1), SQL_C_CHAR, _buf[i], MAX_DATALEN, &_buf_len[i]);
		}

		//  :     (20   )
		{
			net::msg::McListGuildMessage page;
			for (; (sqlret = SQLFetch(hstmt)) == SQL_SUCCESS || sqlret == SQL_SUCCESS_WITH_INFO;) {
				if (sqlret != SQL_SUCCESS)
					handle_err(hstmt, SQL_HANDLE_STMT, sqlret);
				page.entries.push_back({
					static_cast<int64_t>(atol((char const*)_buf[0])),
					std::string((char const*)_buf[1]),
					std::string((char const*)_buf[2]),
					std::string((char const*)_buf[4]),
					static_cast<int64_t>(atoi((const char*)_buf[6])),
					std::atoll((const char*)_buf[5])
				});
				if (page.entries.size() == 20) {
					auto l_wpk = net::msg::serialize(page);
					pCha->ReflectINFof(pCha, l_wpk);
					page.entries.clear();
				}
			}
			//    (  )
			auto l_wpk = net::msg::serialize(page);
			pCha->ReflectINFof(pCha, l_wpk);
		}

		SQLFreeStmt(hstmt, SQL_UNBIND);
		ret = true;
	}
	catch (int& e) {
		//LG("", "ODBC %d\n",e);
		ToLogService("common", "found consortia process ODBC interfance transfer error,position ID:{}", e);
	}
	catch (...) {
		//LG("", "Unknown Exception raised when list all guilds\n");
		ToLogService("common", "Unknown Exception raised when list all guilds");
	}

	if (hstmt != SQL_NULL_HSTMT) {
		SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
		hstmt = SQL_NULL_HSTMT;
	}

	return ret;
}

void CTableGuild::TryFor(CCharacter* pCha, uLong guildid) {
	if (pCha->HasGuild()) {
		//pCha->SystemNotice( "%s,!", pCha->GetGuildName() );
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00032), pCha->GetGuildName());
		return;
	}
	else if (guildid == pCha->GetGuildID()) {
		//pCha->SystemNotice( "%s!", pCha->GetGuildName() );
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00033), pCha->GetGuildName());
		return;
	}

	string buf[3];
	char filter[80];
	const char* param = "guild_id";
	sprintf(filter, "leader_id >0 and guild_id =%d", guildid);
	int l_ret = _get_row(buf, 3, param, filter);
	if (!DBOK(l_ret)) {
		//pCha->SystemNotice(".");
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00034));
		//LG("","[%s][ID=%d]SQL.\n",pCha->GetName(),guildid);
		ToLogService("common", "character[{}]apply join in consortia [ID={}] carry out SQL failed.", pCha->GetName(),
					 guildid);
		return;
	}
	else if (get_affected_rows() != 1) {
		//pCha->SystemNotice(".");
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00035));
		return;
	}
	param = "c.guild_id, c.guild_stat, g.guild_name";
	string l_tbl_name = _tbl_name;
	_tbl_name = "character c,guild g";
	sprintf(filter, "c.guild_id =g.guild_id and c.atorID =%d and g.guild_id <>%d", pCha->GetID(), guildid);
	l_ret = _get_row(buf, 3, param, filter);
	_tbl_name = l_tbl_name;
	if (!DBOK(l_ret) || get_affected_rows() != 1) {
		//pCha->SystemNotice(".");
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00034));
		//LG("","[%s][ID=%d]SQL.\n",pCha->GetName(),guildid);
		ToLogService("common", "character[{}]apply join in consortia [ID={}] carry out SQL failed.", pCha->GetName(),
					 guildid);
		return;
	}

	//
	string bufnew[1];
	param = "guild_name";
	sprintf(filter, "guild_id =%d", guildid);
	int l_retrow = 0;
	l_ret = _get_row(bufnew, 1, param, filter, &l_retrow);
	if (l_retrow == 1) {
	}
	else {
		//LG( "", "TryFor%sID[0x%X]!", pCha->GetName(), guildid );
		ToLogService("common", "TryFor: character {} apply consortia ID[0x{:X}]is inexistence!", pCha->GetName(),
					 guildid);
		//pCha->SystemNotice( "!" );
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00036));
		return;
	}

	//
	strncpy(pCha->GetPlayer()->m_szTempGuildName, bufnew[0].c_str(), defGUILD_NAME_LEN - 1);

	if (const auto guild_id = std::stoi(buf[0]); guild_id) {
		if (const auto status = std::stoi(buf[1]); status == emGldMembStatNormal) {
			pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00037), buf[2].c_str());
			return;
		}
		else if (status == emGldMembStatTry && !(pCha->GetPlayer()->m_GuildState & emGuildReplaceOldTry)) {
			pCha->GetPlayer()->m_GuildState |= emGuildReplaceOldTry;
			pCha->GetPlayer()->m_lTempGuildID = guildid;
			//  :
			auto l_wpk = net::msg::serialize(net::msg::McGuildTryForCfmMessage{buf[2].c_str()});
			pCha->ReflectINFof(pCha, l_wpk);
			return;
		}
	}
	else {
		TryForConfirm(pCha, guildid);
	}
}

void CTableGuild::TryForConfirm(CCharacter* pCha, uLong guildid) {
	char sql[SQL_MAXLEN];

	if (pCha->HasGuild()) {
		//pCha->SystemNotice( "%s,!", pCha->GetGuildName() );
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00038), pCha->GetGuildName());
		return;
	}

	DWORD dwOldGuildID = pCha->GetGuildID();

	//
	if (!begin_tran()) {
		//pCha->SystemNotice( "!" );
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00039));
		return;
	}

	sprintf(sql, "update character set guild_id =%d ,guild_stat =1,guild_permission =0\
						where atorID =%d and\
								%d in (select guild_id from guild where leader_id >0 and guild_id =%d and try_total <%d and member_total <%d)",
			guildid, pCha->GetID(), guildid, guildid, emMaxTryMemberNum, emMaxMemberNum);
	SQLRETURN l_sqlret = exec_sql_direct(sql);
	if (!DBOK(l_sqlret) || get_affected_rows() == 0) {
		this->rollback();
		//pCha->SystemNotice( "!" );
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00040));
		return;
	}

	sprintf(sql, "update guild set try_total =try_total +1 where guild_id =%d", guildid);
	l_sqlret = exec_sql_direct(sql);
	if (!DBOK(l_sqlret) || get_affected_rows() == 0) {
		this->rollback();
		//pCha->SystemNotice( "!" );
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00040));
		return;
	}

	//
	if (dwOldGuildID && (pCha->GetPlayer()->m_GuildState & emGuildReplaceOldTry)) {
		sprintf(sql, "update guild set try_total =try_total -1 where guild_id =%d and try_total > 0"
				, dwOldGuildID);
		SQLRETURN l_sqlret = exec_sql_direct(sql);
		if (!DBOK(l_sqlret) || get_affected_rows() == 0) {
			this->rollback();
			//pCha->SystemNotice( "!" );
			pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00041));
			return;
		}
	}

	if (!commit_tran()) {
		this->rollback();
		//pCha->SystemNotice( "!" );
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00040));
		return;
	}

	//
	pCha->SetGuildID(guildid);
	pCha->SetGuildState(emGldMembStatTry);

	pCha->SetGuildName(pCha->GetPlayer()->m_szTempGuildName);
	//pCha->SystemNotice( "!%s,.", pCha->GetGuildName() );
	pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00042), pCha->GetGuildName());
}


bool CTableGuild::GetGuildBank(uLong guildid, CKitbag* bag) {
	string buf[3];
	char filter[80];
	const char* param = "bank";
	sprintf(filter, "guild_id =%d", guildid);
	int l_retrow = 0;
	bool l_ret = _get_row(buf, 1, param, filter, &l_retrow);
	if (l_retrow == 1) {
		if (buf[0].length() == 0) {
			bag->SetCapacity(48);
			return true;
		}
		if (String2KitbagData(bag, buf[0])) {
			return true;
		}
	}
	return false;
}

int CTableGuild::GetGuildLeaderID(uLong guildid) {
	string buf[3];
	char filter[80];
	const char* param = "leader_id";
	sprintf(filter, "guild_id =%d", guildid);
	int l_retrow = 0;
	bool l_ret = _get_row(buf, 1, param, filter, &l_retrow);
	if (l_retrow == 1) {
		return atoi(buf[0].c_str());
	}
	return 0;
}

bool CTableGuild::UpdateGuildBank(uLong guildid, CKitbag* bag) {
	char sql[SQL_MAXLEN];
	char bagStr[defKITBAG_DATA_STRING_LEN];
	if (KitbagData2String(bag, bagStr, defKITBAG_DATA_STRING_LEN)) {
		sprintf(sql, "update guild set bank = '%s' where guild_id =%d", bagStr, guildid);
		SQLRETURN l_sqlret = exec_sql_direct(sql);
		if (!DBOK(l_sqlret) || get_affected_rows() == 0) {
			//this->rollback(); // dont think we need to rollback??
			return false;
		}
		return true;
	}
	return false;
}

bool CTableGuild::UpdateGuildBankGold(int guildID, int money) {
	char sql[SQL_MAXLEN];
	sprintf(sql, "update guild set gold = gold + %d where guild_id =%d", money, guildID);
	SQLRETURN l_sqlret = exec_sql_direct(sql);
	if (!DBOK(l_sqlret) || get_affected_rows() == 0) {
		return false;
	}
	return true;
}

unsigned long long CTableGuild::GetGuildBankGold(uLong guildid) {
	string buf[1];
	char filter[80];
	const char* param = "gold";
	sprintf(filter, "guild_id =%d", guildid);
	int l_retrow = 0;
	bool l_ret = _get_row(buf, 1, param, filter, &l_retrow);
	if (l_retrow == 1) {
		return stoll(buf[0]);
	}
	return false;
}

std::vector<CTableGuild::BankLog> CTableGuild::GetGuildLog(uLong guildid) {
	string buf[1];
	string logList[1024]; //Max number of strings (1 log = 5 strings)
	std::vector<CTableGuild::BankLog> logs;

	char filter[SQL_MAXLEN];
	const char* param = "banklog";
	sprintf(filter, "guild_id =%d", guildid);
	int l_retrow = 0;
	bool l_ret = _get_row(buf, 1, param, filter, &l_retrow);
	if (l_retrow == 1) {
		int n = Util_ResolveTextLine(buf[0].c_str(), logList, 1024, '-', ';');
		int i = 0;
		while (i < n) {
			BankLog p;
			p.type = Str2Int(logList[i].c_str());
			p.time = Str2Int(logList[i + 1].c_str());
			p.parameter = Str2Int(logList[i + 2].c_str());
			p.quantity = Str2Int(logList[i + 3].c_str());
			p.userID = Str2Int(logList[i + 4].c_str());
			logs.push_back(p);
			i += 5;
		}
	}

	if (logs.size() == 200) {
		logs.erase(logs.begin()); // size is 200, let's erase the first log
	}

	return logs;
}

bool CTableGuild::SetGuildLog(std::vector<BankLog> log, uLong guild_id) {
	char sql[SQL_MAXLEN];
	char data[8000];
	data[0] = '\0';
	for (int i = 0; i < log.size(); i++) {
		if (log.at(i).userID == 0) {
			continue;
		}
		char buf[100];
		//if (i != log.size() - 1) {
		if (true) {
			sprintf(buf, "%d-%lld-%lld-%d-%d;", log.at(i).type, log.at(i).time, log.at(i).parameter, log.at(i).quantity,
					log.at(i).userID);
		}
		strcat(data, buf);
	}

	sprintf(sql, "update guild set banklog = '%s' where guild_id =%d", data, guild_id);
	SQLRETURN l_sqlret = exec_sql_direct(sql);
	if (!DBOK(l_sqlret) || get_affected_rows() == 0) {
		return false;
	}
	return true;
}


bool CTableGuild::GetGuildInfo(CCharacter* pCha, uLong guildid) {
	string buf[4];
	char filter[80];

	const char* param = "guild_name, motto";
	sprintf(filter, "guild_id =%d", guildid);
	int l_retrow = 0;
	bool l_ret = _get_row(buf, 2, param, filter, &l_retrow);
	if (l_retrow == 1) {
		pCha->SetGuildName(buf[0].c_str());
		pCha->SetGuildMotto(buf[1].c_str());
		return true;
	}
	else {
		return false;
	}
}

bool CTableGuild::ListTryPlayer(CCharacter* pCha, char disband_days) {
	bool ret = false;

	if (!pCha || !pCha->HasGuild()) {
		return ret;
	}

	string buf[10];
	char filter[80];

	const char* sql_syntax = "g.guild_id, g.guild_name,g.motto, c.atorNome, g.member_total,g.exp, g.level";

	char param[500];
	sprintf(param, sql_syntax);

	string l_tbl_name = _tbl_name;
	_tbl_name = "character c,guild g";
	sprintf(filter, "g.leader_id =c.atorID and g.guild_id =%d", pCha->GetGuildID());
	int l_retrow = 0;

	bool l_ret = _get_row(buf, 10, param, filter, &l_retrow);
	_tbl_name = l_tbl_name;
	if (!l_ret || !l_retrow || this->get_affected_rows() != 1) {
		return ret;
	}
	//  :
	net::msg::McGuildListTryPlayerMessage tryMsg;
	tryMsg.guildId = atol(buf[0].c_str());
	tryMsg.guildName = buf[1].c_str();
	tryMsg.motto = buf[2].c_str();
	tryMsg.leaderName = buf[3].c_str();
	tryMsg.memberTotal = atoi(buf[4].c_str());
	tryMsg.maxMembers = g_Config.m_sGuildNum;
	tryMsg.exp = _atoi64(buf[5].c_str());
	tryMsg.reserved = 0;
	tryMsg.level = atol(buf[6].c_str());

	sql_syntax =
		"select c.atorID,c.atorNome,c.job,c.degree\
			from character c\
			where (c.guild_stat =1 and c.guild_id =%d and c.delflag =0)\
		";
	char sql[SQL_MAXLEN];
	sprintf(sql, sql_syntax, pCha->GetGuildID());

	SQLRETURN sqlret;
	SQLHSTMT hstmt = SQL_NULL_HSTMT;
	SQLSMALLINT col_num = 0;
	bool found = true;

	try {
		sqlret = SQLAllocHandle(SQL_HANDLE_STMT, _db.GetHandle(), &hstmt);
		if ((sqlret != SQL_SUCCESS) && (sqlret != SQL_SUCCESS_WITH_INFO)) {
			handle_err(_db.GetHandle(), SQL_HANDLE_DBC, sqlret);
			throw 1;
		}

		sqlret = SQLExecDirect(hstmt, (SQLCHAR*)sql, SQL_NTS);
		if (sqlret != SQL_SUCCESS) {
			handle_err(hstmt, SQL_HANDLE_STMT, sqlret);
			if (sqlret != SQL_SUCCESS_WITH_INFO)
				throw 2;
		}

		sqlret = SQLNumResultCols(hstmt, &col_num);
		col_num = min(col_num, MAX_COL);
		col_num = min(col_num, _max_col);

		for (int i = 0; i < col_num; ++i)
			SQLBindCol(hstmt, UWORD(i + 1), SQL_C_CHAR, _buf[i], MAX_DATALEN, &_buf_len[i]);

		for (; (sqlret = SQLFetch(hstmt)) == SQL_SUCCESS || sqlret == SQL_SUCCESS_WITH_INFO;) {
			if (sqlret != SQL_SUCCESS)
				handle_err(hstmt, SQL_HANDLE_STMT, sqlret);
			tryMsg.players.push_back({
				static_cast<int64_t>(atol(reinterpret_cast<char const*>(_buf[0]))),
				std::string(reinterpret_cast<char const*>(_buf[1])),
				std::string(reinterpret_cast<char const*>(_buf[2])),
				static_cast<int64_t>(Str2Int(reinterpret_cast<char const*>(_buf[3])))
			});
		}

		auto l_wpk = net::msg::serialize(tryMsg);
		pCha->ReflectINFof(pCha, l_wpk);

		SQLFreeStmt(hstmt, SQL_UNBIND);
		ret = true;
	}
	catch (int& e) {
		//LG("", "ODBC %d\n",e);
		ToLogService("common", "consult apply consortia process memeberODBC interface transfer error,position ID:{}",
					 e);
	}
	catch (...) {
		//LG("", "Unknown Exception raised when list all guilds\n");
		ToLogService("common", "Unknown Exception raised when list all guilds");
	}

	if (hstmt != SQL_NULL_HSTMT) {
		SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
		hstmt = SQL_NULL_HSTMT;
	}

	return ret;
}

bool CTableGuild::Approve(CCharacter* pCha, uLong chaid) {
	if (!pCha || !pCha->HasGuild()) {
		return false;
	}

	string buf[3];
	char filter[80];

	const char* param = "c.atorID";
	string l_tbl_name = _tbl_name;
	_tbl_name = "character c";
	sprintf(filter, "c.atorID =%d and c.guild_id =%d and c.guild_permission & %d =%d", pCha->GetID(),
			pCha->GetGuildID(), emGldPermRecruit, emGldPermRecruit);
	int retrow = 0;
	bool l_ret = _get_row(buf, 3, param, filter, &retrow);
	_tbl_name = l_tbl_name;
	if (!l_ret) {
		//pCha->SystemNotice(".");
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00043));
		return false;
	}
	if (!retrow) {
		//pCha->SystemNotice("");
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00044));
		return false;
	}

	//
	if (!begin_tran()) {
		//pCha->SystemNotice( "!" );
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00045));
		return false;
	}

	char sql[SQL_MAXLEN];
	sprintf(sql, "update guild\
					set try_total =try_total -1,\
						member_total =member_total +1\
						where guild_id =%d and member_total <%d and try_total > 0"
			, pCha->GetGuildID(), g_Config.m_sGuildNum);
	SQLRETURN l_sqlret = exec_sql_direct(sql);
	if (!DBOK(l_sqlret) || get_affected_rows() == 0) {
		this->rollback();
		//pCha->SystemNotice( "!" );
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00046));
		return false;
	}

	sprintf(sql, "update character set guild_stat =0,guild_permission =%d\
						where atorID =%d and guild_id =%d and guild_stat =1 and delflag =0",
			emGldPermDefault, chaid, pCha->GetGuildID());
	l_sqlret = exec_sql_direct(sql);
	if (!DBOK(l_sqlret) || get_affected_rows() == 0) {
		this->rollback();
		//pCha->SystemNotice( "!" );
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00046));
		return false;
	}

	if (!commit_tran()) {
		this->rollback();
		//pCha->SystemNotice( "!" );
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00046));
		return false;
	}

	//  :   (- MM)
	auto l_wpk = net::msg::serialize(net::msg::MmGuildApproveMessage{
		(int64_t)chaid, pCha->GetGuildID(), pCha->GetValidGuildName(), pCha->GetValidGuildMotto()
	});
	pCha->ReflectINFof(pCha, l_wpk);

	//  :   (GameServerGroup)
	l_wpk = net::msg::serialize(net::msg::GmGuildApproveMessage{(int64_t)chaid});
	pCha->ReflectINFof(pCha, l_wpk);

	const std::string cha_name = game_db.GetChaNameByID(chaid);

	char msg[SQL_MAXLEN];
	sprintf(msg, "%s has been accepted to the guild!", cha_name.c_str());
	DWORD guildID = pCha->GetGuildID();
	g_pGameApp->GuildNotice(guildID, msg);

	return true;
}

bool CTableGuild::Reject(CCharacter* pCha, uLong chaid) {
	if (!pCha || !pCha->HasGuild()) {
		return false;
	}
	//printf_s("Reject %lu", chaid);
	string buf[3];
	char filter[80];

	const char* param = "c.atorID";
	string l_tbl_name = _tbl_name;
	_tbl_name = "character c";
	sprintf(filter, "c.atorID =%d and c.guild_id =%d and c.guild_permission & %d =%d", pCha->GetID(),
			pCha->GetGuildID(), emGldPermRecruit, emGldPermRecruit);
	int retrow = 0;
	bool l_ret = _get_row(buf, 3, param, filter, &retrow);
	_tbl_name = l_tbl_name;
	if (!l_ret) {
		//pCha->SystemNotice(".");
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00047));
		return false;
	}
	if (!retrow) {
		//pCha->SystemNotice(".");
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00048));
		return false;
	}

	//
	if (!begin_tran()) {
		//pCha->SystemNotice( "!" );
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00045));
		return false;
	}

	char sql[SQL_MAXLEN];
	sprintf(sql, "update character set guild_id =0 ,guild_stat =0,guild_permission =0\
						where atorID =%d and guild_id =%d and guild_stat =1",
			chaid, pCha->GetGuildID());
	SQLRETURN l_sqlret = exec_sql_direct(sql);
	if (!DBOK(l_sqlret) || get_affected_rows() == 0) {
		this->rollback();
		//pCha->SystemNotice( "!!" );
		//printf_s("Reject %lu failed ", chaid);
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00049));
		return false;
	}

	sprintf(sql, "update guild set try_total =try_total -1 where guild_id =%d and try_total > 0"
			, pCha->GetGuildID());
	l_sqlret = exec_sql_direct(sql);
	if (!DBOK(l_sqlret) || get_affected_rows() == 0) {
		this->rollback();
		//pCha->SystemNotice( "!!" );
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00049));
		return false;
	}

	if (!commit_tran()) {
		this->rollback();
		//pCha->SystemNotice( "!!" );
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00049));
		return false;
	}

	//  :   (- MM)
	auto l_wpk = net::msg::serialize(net::msg::MmGuildRejectMessage{(int64_t)chaid, pCha->GetGuildName()});
	pCha->ReflectINFof(pCha, l_wpk);
	return true;
}

bool CTableGuild::Kick(CCharacter* pCha, uLong chaid) {
	if (!pCha || !pCha->HasGuild()) {
		return false;
	}

	string buf[3];
	char filter[80];

	const char* param = "c.atorID";
	string l_tbl_name = _tbl_name;
	_tbl_name = "character c";
	sprintf(filter, "c.atorID =%d and c.guild_id =%d and c.guild_permission & %d =%d", pCha->GetID(),
			pCha->GetGuildID(), emGldPermKick, emGldPermKick);
	int retrow = 0;
	bool l_ret = _get_row(buf, 3, param, filter, &retrow);
	_tbl_name = l_tbl_name;
	if (!l_ret) {
		//pCha->SystemNotice(".");
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00050));
		return false;
	}
	if (!retrow) {
		//pCha->SystemNotice(".");
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00048));
		return false;
	}

	if (chaid == pCha->GetID()) {
		//pCha->SystemNotice( "!" );
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00051));
		return false;
	}

	//
	if (!begin_tran()) {
		//pCha->SystemNotice( "!" );
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00052)
		);
		return false;
	}

	char sql[SQL_MAXLEN];
	sprintf(sql, "update character set guild_id =0 ,guild_stat =0,guild_permission =0\
						where atorID =%d and guild_id =%d and guild_stat =0 and\
								atorID not in(select leader_id from guild where guild_id =%d)",
			chaid, pCha->GetGuildID(), pCha->GetGuildID());
	SQLRETURN l_sqlret = exec_sql_direct(sql);
	if (!DBOK(l_sqlret) || get_affected_rows() == 0) {
		this->rollback();
		//pCha->SystemNotice( "!" );
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00053));
		return false;
	}

	sprintf(sql, "update guild set member_total =member_total -1 where guild_id =%d", pCha->GetGuildID());
	l_sqlret = exec_sql_direct(sql);
	if (!DBOK(l_sqlret) || get_affected_rows() == 0) {
		this->rollback();
		//pCha->SystemNotice( "!" );
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00053));
		return false;
	}

	if (!commit_tran()) {
		this->rollback();
		//pCha->SystemNotice( "!" );
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00053));
		return false;
	}

	//  :    (- MM)
	auto l_wpk = net::msg::serialize(net::msg::MmGuildKickMessage{(int64_t)chaid, pCha->GetGuildName()});
	pCha->ReflectINFof(pCha, l_wpk);

	//  :    (GameServerGroup)
	l_wpk = net::msg::serialize(net::msg::GmGuildKickMessage{(int64_t)chaid});
	pCha->ReflectINFof(pCha, l_wpk);

	//  :
	l_wpk = net::msg::serializeMcGuildKickCmd();
	pCha->ReflectINFof(pCha, l_wpk);

	return true;
}

bool CTableGuild::Leave(CCharacter* pCha) {
	if (!pCha || !pCha->HasGuild()) {
		return false;
	}

	//
	if (!begin_tran()) {
		//pCha->SystemNotice( "!" );
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00054));
		return false;
	}

	char sql[SQL_MAXLEN];
	sprintf(sql, "update character set guild_id =0 ,guild_stat =0,guild_permission =0\
						where atorID =%d and guild_id =%d and guild_stat =0 and\
								atorID not in(select leader_id from guild where guild_id =%d)",
			pCha->GetID(), pCha->GetGuildID(), pCha->GetGuildID());
	SQLRETURN l_sqlret = exec_sql_direct(sql);
	if (!DBOK(l_sqlret) || get_affected_rows() == 0) {
		this->rollback();
		//pCha->SystemNotice( "!" );
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00055));
		return false;
	}

	sprintf(sql, "update guild set member_total =member_total -1 where guild_id =%d", pCha->GetGuildID());
	l_sqlret = exec_sql_direct(sql);
	if (!DBOK(l_sqlret) || get_affected_rows() == 0) {
		this->rollback();
		//pCha->SystemNotice( "!" );
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00055));
		return false;
	}

	if (!commit_tran()) {
		this->rollback();
		//pCha->SystemNotice( "!" );
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00055));
		return false;
	}

	char msg[SQL_MAXLEN];
	sprintf(msg, "%s has left the guild!", pCha->GetName());
	DWORD guildID = pCha->GetGuildID();
	g_pGameApp->GuildNotice(guildID, msg);

	pCha->SetGuildID(0);
	pCha->SetGuildName("");
	pCha->SetGuildMotto("");
	pCha->SyncGuildInfo();
	//pCha->SystemNotice("!");
	pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00056));

	//  :    (GameServerGroup)
	auto l_wpk = net::msg::serializeGmGuildLeaveCmd();
	pCha->ReflectINFof(pCha, l_wpk);

	//  :
	l_wpk = net::msg::serializeMcGuildLeaveCmd();
	pCha->ReflectINFof(pCha, l_wpk);
	return true;
}

bool CTableGuild::Disband(CCharacter* pCha, cChar* passwd) {
	if (!pCha || !pCha->HasGuild()) {
		return false;
	}

	string buf[6];
	char filter[80];
	const char* param = "challlevel";
	sprintf(filter, "guild_id =%d", pCha->GetValidGuildID());
	int l_retrow = 0;
	bool l_ret = _get_row(buf, 6, param, filter, &l_retrow);
	if (l_retrow == 1) {
		if (atoi(buf[0].c_str()) > 0) {
			//pCha->SystemNotice( "!" );
			pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00057));
			return false;
		}
		else {
			l_retrow = 0;
			sprintf(filter, "challid =%d", pCha->GetValidGuildID());
			bool l_ret = _get_row(buf, 6, param, filter, &l_retrow);
			if (!l_ret) {
				//pCha->SystemNotice( "!" );
				pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00058));
				return false;
			}
			if (l_retrow >= 1) {
				//pCha->SystemNotice( "!" );
				pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00059));
				return false;
			}
		}
	}
	else {
		//pCha->SystemNotice( "!" );
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00060));
		return false;
	}

	//
	if (!begin_tran()) {
		//pCha->SystemNotice( "!" );
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00061));
		return false;
	}

	char sql[SQL_MAXLEN];
	sprintf(
		sql,
		"update guild set level = 0, gold = 0,bank = '', motto ='',passwd ='',leader_id =0, exp =0,member_total =0,try_total =0 \
						where guild_id =%d and passwd ='%s' ",
		pCha->GetGuildID(), passwd);

	SQLRETURN l_sqlret = exec_sql_direct(sql);
	if (!DBOK(l_sqlret) || get_affected_rows() == 0) {
		this->rollback();
		//pCha->SystemNotice( "!" );
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00062));
		return false;
	}

	sprintf(sql, "update character set guild_id =0 ,guild_stat =0,guild_permission =0\
						where guild_id =%d",
			pCha->GetGuildID());
	l_sqlret = exec_sql_direct(sql);
	if (!DBOK(l_sqlret) || get_affected_rows() == 0) {
		this->rollback();
		//pCha->SystemNotice( "!" );
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00062));
		return false;
	}

	if (!commit_tran()) {
		this->rollback();
		//pCha->SystemNotice( "!" );
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00062));
		return false;
	}
	pCha->guildPermission = 0;
	//  :   (GameServerGroup)
	auto l_wpk = net::msg::serializeGmGuildDisbandCmd();
	pCha->ReflectINFof(pCha, l_wpk);

	int guildID = pCha->GetGuildID();

	//  :   (- MM)
	l_wpk = net::msg::serialize(net::msg::MmGuildDisbandMessage{(int64_t)guildID});
	pCha->ReflectINFof(pCha, l_wpk);

	return true;
}

bool CTableGuild::Motto(CCharacter* pCha, cChar* motto) {
	if (!pCha || !pCha->HasGuild()) {
		return false;
	}

	char sql[SQL_MAXLEN];
	sprintf(sql, "update guild set motto ='%s'\
						where guild_id =%d",
			motto, pCha->GetGuildID());
	SQLRETURN l_sqlret = exec_sql_direct(sql);
	if (!DBOK(l_sqlret)) {
		//pCha->SystemNotice(".");
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00063));
		return false; //SQL
	}
	if (get_affected_rows() != 1) {
		//pCha->SystemNotice(".");
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00064));
		return false;
	}

	//  :    (- MM)
	auto l_wpk = net::msg::serialize(net::msg::MmGuildMottoMessage{(int64_t)pCha->GetGuildID(), motto});
	pCha->ReflectINFof(pCha, l_wpk);

	//  :    (GameServerGroup)
	l_wpk = net::msg::serialize(net::msg::GmGuildMottoMessage{motto});
	pCha->ReflectINFof(pCha, l_wpk);

	char msg[SQL_MAXLEN];
	sprintf(msg, "Guild Motto: %s", motto);

	DWORD guildID = pCha->GetGuildID();
	g_pGameApp->GuildNotice(guildID, msg);

	return true;
}

bool CTableGuild::GetGuildName(long lGuildID, std::string& strGuildName) {
	char filter[80];

	const char* param = "guild_name";
	sprintf(filter, "guild_id =%d", lGuildID);
	int l_retrow = 0;
	return _get_row(&strGuildName, 1, param, filter, &l_retrow);
}

bool CTableGuild::Leizhu(CCharacter* pCha, BYTE byLevel, DWORD dwMoney) {
	if (!pCha || !pCha->HasGuild() || byLevel < 1 || byLevel > 3) {
		return false;
	}

	if (dwMoney == 0) {
		//pCha->SystemNotice( "0!" );
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00065));
		return false;
	}

	string buf[6];
	char filter[80];
	const char* param1 = "guild_id, guild_name, challid, challmoney, leader_id, challstart";
	if (pCha->GetValidGuildID() > 0) {
		sprintf(filter, "guild_id =%d", pCha->GetValidGuildID());
		int l_retrow = 0;
		bool l_ret = _get_row(buf, 6, param1, filter, &l_retrow);
		if (l_retrow == 1) {
			if (pCha->GetID() == atoi(buf[4].c_str())) {
				//
			}
			else {
				return false;
			}
		}
		else {
			//pCha->SystemNotice( "!!" );
			pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00066));
			return false;
		}

		sprintf(filter, "challid =%d", pCha->GetValidGuildID());
		l_ret = _get_row(buf, 6, param1, filter, &l_retrow);
		if (l_retrow >= 1) {
			//pCha->SystemNotice( "%s^_^!", buf[1].c_str() );
			pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00067), buf[1].c_str());
			return false;
		}
	}
	else {
		return false;
	}

	//
	if (!begin_tran()) {
		//pCha->SystemNotice( "!" );
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00068));
		return false;
	}

	char sql[SQL_MAXLEN];
	char szGuild[64];
	memset(szGuild, 0, 64);
	DWORD dwGuildID = 0;
	DWORD dwChallID = 0;
	DWORD dwChallMoney = 0;
	const char* param = "guild_id, guild_name, challid, challmoney";
	sprintf(filter, "challlevel =%d", byLevel);
	int l_retrow = 0;
	bool l_ret = _get_row(buf, 4, param, filter, &l_retrow);
	if (l_retrow == 1) {
		//pCha->SystemNotice( "%s%d!", buf[1].c_str(), byLevel );
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00069), buf[1].c_str(), byLevel);
		return false;
	}
	else {
		const char* param1 = "challlevel";
		sprintf(filter, "guild_id =%d", pCha->GetValidGuildID());
		bool l_ret = _get_row(buf, 4, param1, filter, &l_retrow);
		if (l_retrow == 1) {
			if (atoi(buf[0].c_str()) > 0) {
				//pCha->SystemNotice( "%d!", atoi(buf[0].c_str()) );
				pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00070), atoi(buf[0].c_str()));
				return false;
			}
		}

		DWORD dwMoneyArray[3] = {5000000, 3000000, 1000000};
		if (dwMoney < dwMoneyArray[byLevel - 1] || !pCha->HasMoney(dwMoney)) {
			//pCha->SystemNotice( "%d%uG!", byLevel, dwMoneyArray[byLevel-1] );
			pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00071), byLevel, dwMoneyArray[byLevel - 1]);
			return false;
		}

		sprintf(sql, "update guild set challid = 0, challstart = 0, challmoney = 0, challlevel = %d where guild_id =%d",
				byLevel, pCha->GetValidGuildID());
		SQLRETURN l_sqlret = exec_sql_direct(sql);
		if (!DBOK(l_sqlret) || get_affected_rows() == 0) {
			this->rollback();
			//LG( "", "!!ID = %d.%d", pCha->GetValidGuildID(), byLevel );
			ToLogService(
				"common",
				"challenge consortia over,leizhu failed:update lost consortia data operater failed! consortiaID = {}.consortia level:{}",
				pCha->GetValidGuildID(), byLevel);
			return false;
		}

		if (!commit_tran()) {
			this->rollback();
			//pCha->SystemNotice( "!" );
			pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00072));
			return false;
		}
		//if( pCha->TakeMoney( "", dwMoney ) )
		if (pCha->TakeMoney(RES_STRING(GM_GAMEDB_CPP_00073), dwMoney)) {
			//pCha->SystemNotice( "%s%d!", pCha->GetGuildName(), byLevel );
			pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00074), pCha->GetGuildName(), byLevel);
		}
		this->ListChallenge(pCha);
	}
	return true;
}

bool CTableGuild::Challenge(CCharacter* pCha, BYTE byLevel, DWORD dwMoney) {
	if (!pCha || !pCha->HasGuild() || byLevel < 1 || byLevel > 3) {
		return false;
	}

	if (dwMoney == 0) {
		//pCha->SystemNotice( "0!" );
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00075));
		return false;
	}

	string buf[6];
	char filter[80];
	const char* param1 = "guild_id, guild_name, challid, challmoney, leader_id, challstart";
	if (pCha->GetValidGuildID() > 0) {
		sprintf(filter, "guild_id =%d", pCha->GetValidGuildID());
		int l_retrow = 0;
		bool l_ret = _get_row(buf, 6, param1, filter, &l_retrow);
		if (l_retrow == 1) {
			if (pCha->GetID() == atoi(buf[4].c_str())) {
				//
			}
			else {
				return false;
			}
		}
		else {
			//pCha->SystemNotice( "!!" );
			pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00066));
			return false;
		}

		sprintf(filter, "challid =%d", pCha->GetValidGuildID());
		l_ret = _get_row(buf, 6, param1, filter, &l_retrow);
		if (l_retrow >= 1) {
			//pCha->SystemNotice( "%s^_^!", buf[1].c_str() );
			pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00067), buf[1].c_str());
			return false;
		}
	}
	else {
		return false;
	}

	//
	if (!begin_tran()) {
		//pCha->SystemNotice( "!" );
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00068));
		return false;
	}

	char sql[SQL_MAXLEN];
	char szGuild[64];
	memset(szGuild, 0, 64);
	DWORD dwGuildID = 0;
	DWORD dwChallID = 0;
	DWORD dwChallMoney = 0;
	const char* param = "guild_id, guild_name, challid, challmoney";
	sprintf(filter, "challlevel =%d", byLevel);
	int l_retrow = 0;
	bool l_ret = _get_row(buf, 4, param, filter, &l_retrow);
	if (l_retrow == 1) {
		dwGuildID = atoi(buf[0].c_str());
		strncpy(szGuild, buf[1].c_str(), 63);
		dwChallID = atoi(buf[2].c_str());
		dwChallMoney = atoi(buf[3].c_str());
	}
	else {
		DWORD dwMoneyArray[3] = {5000000, 3000000, 1000000};
		if (dwMoney < dwMoneyArray[byLevel - 1] || !pCha->HasMoney(dwMoney)) {
			//pCha->SystemNotice( "%d%uG!", byLevel, dwMoneyArray[byLevel-1] );
			pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00077), byLevel, dwMoneyArray[byLevel - 1]);
			return false;
		}

		sprintf(sql, "update guild set challid = 0, challstart = 0, challmoney = 0, challlevel = %d where guild_id =%d",
				byLevel, pCha->GetValidGuildID());
		SQLRETURN l_sqlret = exec_sql_direct(sql);
		if (!DBOK(l_sqlret) || get_affected_rows() == 0) {
			this->rollback();
			//LG( "", "!!ID = %d.%d", pCha->GetValidGuildID(), byLevel );
			ToLogService(
				"common",
				"challenge consortia over,leizhu failed:update lost consortia data operater failed! consortiaID = {}.consortia level:{}",
				pCha->GetValidGuildID(), byLevel);
			return false;
		}

		if (!commit_tran()) {
			this->rollback();
			//pCha->SystemNotice( "!" );
			pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00072));
			return false;
		}
		//if( pCha->TakeMoney( "", dwMoney ) )
		if (pCha->TakeMoney(RES_STRING(GM_GAMEDB_CPP_00073), dwMoney)) {
			//pCha->SystemNotice( "%s%d!", pCha->GetGuildName(), byLevel );
			pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00074), pCha->GetGuildName(), byLevel);
		}
		this->ListChallenge(pCha);
		return true;
	}

	BYTE byLvData = 0;
	const char* param2 = "challlevel";
	sprintf(filter, "guild_id =%d", pCha->GetValidGuildID());
	l_ret = _get_row(buf, 4, param2, filter, &l_retrow);
	if (l_retrow == 1) {
		byLvData = (BYTE)atoi(buf[0].c_str());
	}
	else {
		//pCha->SystemNotice( "!" );
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00078));
		return false;
	}

	if (dwGuildID == 0) {
		//pCha->SystemNotice( "!GID = %d, LV = %d", dwGuildID, byLevel );
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00079), dwGuildID, byLevel);
		return false;
	}

	if (byLvData != 0 && byLevel > byLvData) {
		//pCha->SystemNotice( "!" );
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00080));
		return false;
	}

	if (pCha->GetPlayer()->GetDBChaId() == dwChallID) {
		//pCha->SystemNotice( "!" );
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00081));
		return false;
	}
	else if (pCha->GetValidGuildID() == dwGuildID) {
		//pCha->SystemNotice( "!" );
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00082));
		return false;
	}
	else if (dwMoney < dwChallMoney + 50000) {
		//pCha->SystemNotice( "!%u", dwMoney );
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00083), dwMoney);
		return false;
	}

	if (!pCha->HasMoney(dwMoney)) {
		//pCha->SystemNotice( "!%u", dwMoney );
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00084), dwMoney);
		return false;
	}

	//
	sprintf(sql, "update guild set challid =%d,challmoney =%d where guild_id =%d \
					and challmoney < %d and challstart = 0",
			pCha->GetGuildID(), dwMoney, dwGuildID, dwMoney);
	SQLRETURN l_sqlret = exec_sql_direct(sql);
	if (!DBOK(l_sqlret) || get_affected_rows() == 0) {
		this->rollback();
		//pCha->SystemNotice( "!" );
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00085));
		return false;
	}

	if (!commit_tran()) {
		this->rollback();
		//pCha->SystemNotice( "!" );
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00085));
		return false;
	}

	//
	//pCha->TakeMoney( "", dwMoney );
	pCha->TakeMoney(RES_STRING(GM_GAMEDB_CPP_00073), dwMoney);
	//
	if (dwChallID > 0 && dwChallMoney > 0) {
		//  :     (GameServerGroup)
		auto l_wpk = net::msg::serialize(net::msg::GmGuildChallMoneyMessage{
			(int64_t)dwChallID, (int64_t)dwChallMoney, szGuild, pCha->GetGuildName()
		});
		pCha->ReflectINFof(pCha, l_wpk);
	}

	ListChallenge(pCha);
	return true;
}

void CTableGuild::ListChallenge(CCharacter* pCha) {
	string buf1[6];
	string buf2[6];
	char filter[80];

	DWORD dwGuildID = 0;
	DWORD dwChallID = 0;
	DWORD dwChallMoney = 0;
	DWORD dwLeaderID = 0;
	BYTE byStart = 0;

	//  :
	net::msg::McGuildListChallMessage challMsg{};

	const char* param = "guild_id, guild_name, challid, challmoney, leader_id, challstart";
	if (pCha->GetValidGuildID() > 0) {
		sprintf(filter, "guild_id =%d", pCha->GetValidGuildID());
		int l_retrow = 0;
		bool l_ret = _get_row(buf1, 6, param, filter, &l_retrow);
		if (l_retrow == 1) {
			challMsg.isLeader = (pCha->GetID() == atoi(buf1[4].c_str())) ? 1 : 0;
		}
		else {
			pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00066));
			return;
		}
	}

	for (int i = 1; i <= 3; ++i) {
		sprintf(filter, "challlevel =%d", i);
		int l_retrow = 0;
		bool l_ret = _get_row(buf1, 6, param, filter, &l_retrow);
		if (l_retrow == 1) {
			dwGuildID = atoi(buf1[0].c_str());
			dwChallID = atoi(buf1[2].c_str());
			dwChallMoney = atoi(buf1[3].c_str());
			byStart = (BYTE)atoi(buf1[5].c_str());

			if (dwChallID != 0) {
				sprintf(filter, "guild_id =%d", dwChallID);
				bool l_ret2 = _get_row(buf2, 6, param, filter, &l_retrow);
				if (l_retrow == 1)
					challMsg.entries[i - 1] = {
						static_cast<int64_t>(i), static_cast<int64_t>(byStart), std::string(buf1[1].c_str()),
						std::string(buf2[1].c_str()), static_cast<int64_t>(dwChallMoney)
					};
			}
			else {
				challMsg.entries[i - 1] = {
					static_cast<int64_t>(i), static_cast<int64_t>(byStart), std::string(buf1[1].c_str()),
					std::string(""), static_cast<int64_t>(dwChallMoney)
				};
			}
		}
	}
	auto l_wpk = net::msg::serialize(challMsg);
	pCha->ReflectINFof(pCha, l_wpk);
}

bool CTableGuild::HasGuildLevel(CCharacter* pChar, BYTE byLevel) {
	if (!pChar->HasGuild()) {
		return false;
	}

	string buf[1];
	char filter[80];
	BYTE byData = 0;
	const char* param = "challlevel";
	sprintf(filter, "guild_id =%d", pChar->GetValidGuildID());
	int l_retrow = 0;
	bool l_ret = _get_row(buf, 1, param, filter, &l_retrow);
	if (l_retrow == 1) {
		byData = (BYTE)atoi(buf[0].c_str());
		return byLevel == byData;
	}
	return false;
}

bool CTableGuild::HasCall(BYTE byLevel) {
	string buf[5];
	char filter[80];

	char szGuild[64];
	memset(szGuild, 0, 64);
	DWORD dwGuildID = 0;
	DWORD dwChallID = 0;
	DWORD dwChallMoney = 0;
	const char* param = "guild_id, guild_name, challid, challmoney, challstart";
	sprintf(filter, "challlevel =%d", byLevel);
	int l_retrow = 0;
	BYTE byStart = 0;
	bool l_ret = _get_row(buf, 5, param, filter, &l_retrow);
	if (l_retrow == 1) {
		dwGuildID = atoi(buf[0].c_str());
		strncpy(szGuild, buf[1].c_str(), 63);
		dwChallID = atoi(buf[2].c_str());
		dwChallMoney = atoi(buf[3].c_str());
		byStart = (BYTE)atoi(buf[4].c_str());
		return dwChallID != 0 && byStart == 1;
	}
	return false;
}

bool CTableGuild::StartChall(BYTE byLevel) {
	//LG( "", "%d...\n", byLevel );
	ToLogService("common", "range level {} challenge start treat with....", byLevel);
	string buf[4];
	char filter[80];

	char szGuild[64];
	memset(szGuild, 0, 64);
	DWORD dwGuildID = 0;
	DWORD dwChallID = 0;
	DWORD dwChallMoney = 0;
	const char* param = "guild_id, guild_name, challid, challmoney";
	sprintf(filter, "challlevel =%d", byLevel);
	int l_retrow = 0;
	bool l_ret = _get_row(buf, 4, param, filter, &l_retrow);
	if (l_retrow == 1) {
		dwGuildID = atoi(buf[0].c_str());
		strncpy(szGuild, buf[1].c_str(), 63);
		dwChallID = atoi(buf[2].c_str());
		dwChallMoney = atoi(buf[3].c_str());
	}
	else {
		return false;
	}

	if (dwGuildID == 0) {
		return false;
	}

	//
	char sql[SQL_MAXLEN];
	sprintf(sql, "update guild set challstart = 1 where guild_id =%d and challstart = 0",
			dwGuildID);
	SQLRETURN l_sqlret = exec_sql_direct(sql);
	if (!DBOK(l_sqlret) || get_affected_rows() == 0) {
		//LG( "", "!!" );
		ToLogService(
			"common", "challenge consortia data operator failed!consortia battle already start or inexistence!");
		return false;
	}

	//LG( "", "%d!GUILD1 = %d, GUILD2 = %d, Money = %u.\n", byLevel, dwGuildID, dwChallID, dwChallMoney );
	ToLogService("common", "range level {} challenge start succeed !GUILD1 = {}, GUILD2 = {}, Money = {}.", byLevel,
				 dwGuildID, dwChallID, dwChallMoney);
	return true;
}

void CTableGuild::EndChall(DWORD dwGuild1, DWORD dwGuild2, BOOL bChall) {
	//LG( "", "GUILD1 = %d, GUILD2 = %d...\n", dwGuild1, dwGuild2 );
	ToLogService("common", "arranger level consortia game start operator finish GUILD1 = {}, GUILD2 = {}...", dwGuild1,
				 dwGuild2);
	string buf[5];
	char filter[80];

	char szGuild[64];
	memset(szGuild, 0, 64);
	DWORD dwGuildID = 0;
	DWORD dwChallID = 0;
	DWORD dwChallMoney = 0;
	BYTE byLevel = 0;
	BYTE byStart = 0;
	const char* param = "challstart, guild_name, challid, challmoney, challlevel";
	sprintf(filter, "guild_id =%d", dwGuild1);
	int l_retrow = 0;
	bool l_ret = _get_row(buf, 5, param, filter, &l_retrow);
	if (l_retrow == 1) {
		byStart = (BYTE)atoi(buf[0].c_str());
		strncpy(szGuild, buf[1].c_str(), 63);
		dwChallID = atoi(buf[2].c_str());
		dwChallMoney = atoi(buf[3].c_str());
		byLevel = (BYTE)atoi(buf[4].c_str());
		if (dwChallID == dwGuild2) {
			ChallMoney(byLevel, bChall, dwGuild1, dwGuild2, dwChallMoney);
			//LG( "", "%d!GUILD1 = %d, GUILD2 = %d, Money = %u.\n", byLevel, dwGuild1, dwGuild2, dwChallMoney );
			ToLogService("common", "range level {} consortia challenge over!GUILD1 = {}, GUILD2 = {}, Money = {}.",
						 byLevel, dwGuild1, dwGuild2, dwChallMoney);
			return;
		}
	}

	sprintf(filter, "guild_id =%d", dwGuild2);
	l_ret = _get_row(buf, 5, param, filter, &l_retrow);
	if (l_retrow == 1) {
		byStart = (BYTE)atoi(buf[0].c_str());
		strncpy(szGuild, buf[1].c_str(), 63);
		dwChallID = atoi(buf[2].c_str());
		dwChallMoney = atoi(buf[3].c_str());
		byLevel = (BYTE)atoi(buf[4].c_str());
		if (dwChallID == dwGuild1) {
			ChallMoney(byLevel, !bChall, dwGuild2, dwGuild1, dwChallMoney);
			//LG( "", "%d!GUILD1 = %d, GUILD2 = %d, Money = %u.\n", byLevel, dwGuild2, dwGuild1, dwChallMoney );
			ToLogService("common", "range level {} consortia challenge over!GUILD1 = {}, GUILD2 = {}, Money = {}.",
						 byLevel, dwGuild2, dwGuild1, dwChallMoney);
			return;
		}
	}

	//LG( "", "!GUILD1 = %d, GUILD2 = %d, ChallFlag = %d.\n", dwGuild1, dwGuild2, ( bChall ) ? 1 : 0 );
	ToLogService("common", "consortia challenge result disposal failed!GUILD1 = {}, GUILD2 = {}, ChallFlag = {}.",
				 dwGuild1, dwGuild2, (bChall) ? 1 : 0);
}

bool CTableGuild::ChallWin(BOOL bUpdate, BYTE byLevel, DWORD dwWinGuildID, DWORD dwFailerGuildID) {
	//
	if (!begin_tran()) {
		//LG( "", "!" );
		ToLogService("common", "challenge consortia finish,update consortia data start affair failed!");
		return false;
	}

	//
	char sql[SQL_MAXLEN];
	SQLRETURN l_sqlret;
	if (!bUpdate) {
		//if( !DBOK( l_sqlret ) || get_affected_rows() == 0 )
		//{
		//	this->rollback();
		//	LG( "", "!!ID = %d.%d", dwFailerGuildID, byLevel );
		//	return false;
		//}
	}
	else {
		string buf[5];
		char filter[80];

		BYTE byLvData = 0;
		const char* param = "challlevel";
		sprintf(filter, "guild_id =%d", dwWinGuildID);
		int l_retrow = 0;
		bool l_ret = _get_row(buf, 5, param, filter, &l_retrow);
		if (l_retrow == 1) {
			byLvData = (BYTE)atoi(buf[0].c_str());
		}
		else {
			//LG( "", "!GUILDID = %d, WINID = %d.\n", dwFailerGuildID, dwWinGuildID );
			ToLogService(
				"common",
				"finish challenge consortialeizhu failed:inquire about failed consortia level info failed!GUILDID = {}, WINID = {}.",
				dwFailerGuildID, dwWinGuildID);
			return false;
		}

		if (byLvData > 0) {
			//
			if (byLvData < byLevel) {
				BYTE byTemp = byLevel;
				byLevel = byLvData;
				byLvData = byTemp;
			}

			sprintf(
				sql, "update guild set challid = 0, challstart = 0, challmoney = 0, challlevel = %d where guild_id =%d",
				byLvData, dwFailerGuildID);
			l_sqlret = exec_sql_direct(sql);
			if (!DBOK(l_sqlret) || get_affected_rows() == 0) {
				this->rollback();
				//LG( "", "!!ID = %d.%d.\n", dwFailerGuildID, byLevel );
				ToLogService(
					"common",
					"challenge consortia over,leizhu failed:update lost consortia data operater failed! consortiaID = {}.consortia level:{}.",
					dwFailerGuildID, byLevel);
				return false;
			}
		}
		else {
			sprintf(
				sql, "update guild set challid = 0, challstart = 0, challmoney = 0, challlevel = 0 where guild_id =%d",
				dwFailerGuildID);
			l_sqlret = exec_sql_direct(sql);
			if (!DBOK(l_sqlret) || get_affected_rows() == 0) {
				this->rollback();
				//LG( "", "!!ID = %d.%d.\n", dwFailerGuildID, byLevel );
				ToLogService(
					"common",
					"challenge consortia over,leizhu failed:update lost consortia data operater failed! consortiaID = {}.consortia level:{}.",
					dwFailerGuildID, byLevel);
				return false;
			}
		}
	}

	sprintf(sql, "update guild set challid = 0, challstart = 0, challmoney = 0, challlevel = %d where guild_id =%d",
			byLevel, dwWinGuildID);
	l_sqlret = exec_sql_direct(sql);
	if (!DBOK(l_sqlret) || get_affected_rows() == 0) {
		this->rollback();
		//LG( "", "!!ID = %d.%d.\n", dwWinGuildID, byLevel );
		ToLogService(
			"common",
			"challenge consortia over,update winner consortia data operator failed!inexistence consortia!consortiaID = {}.consortia level{}.",
			dwWinGuildID, byLevel);
		return false;
	}

	if (!commit_tran()) {
		this->rollback();
		//LG( "", "!.\n" );
		ToLogService("common", "challenge consortia data referring failed,retry later on");
		return false;
	}
	return true;
}

void CTableGuild::ChallMoney(BYTE byLevel, BOOL bChall, DWORD dwGuildID, DWORD dwChallID, DWORD dwMoney) {
	if (bChall) {
		//LG( "", "ID = %d, ID = %d, %u, %d.\n", dwGuildID, dwChallID, dwMoney, byLevel  );
		ToLogService("common", "challenge failed: winner:ID = {},loser:ID = {}, money = {},level:{}.", dwGuildID,
					 dwChallID, dwMoney, byLevel);
		if (!ChallWin(FALSE, byLevel, dwGuildID, dwChallID)) {
			return;
		}

		if (dwChallID != 0) {
			dwMoney = DWORD(float(dwMoney * 80) / 100);
			//  :    (GameServerGroup)
			auto l_wpk = net::msg::serialize(net::msg::MpGuildChallPrizeMoneyMessage{
				(int64_t)dwGuildID, (int64_t)dwMoney
			});
			SENDTOGROUP(l_wpk);
		}
	}
	else {
		//LG( "", "ID = %d, ID = %d, %u, %d.\n", dwChallID, dwGuildID, dwMoney, byLevel  );
		ToLogService("common", "challenge succeedwinner:ID = {},loser:ID = {}, money = {},level:{}.", dwChallID,
					 dwGuildID, dwMoney, byLevel);
		if (!ChallWin(TRUE, byLevel, dwChallID, dwGuildID)) {
			return;
		}

		dwMoney = DWORD(float(dwMoney * 80) / 100);
		//  :    (GameServerGroup)
		auto l_wpk = net::msg::serialize(net::msg::MpGuildChallPrizeMoneyMessage{(int64_t)dwChallID, (int64_t)dwMoney});
		SENDTOGROUP(l_wpk);
	}
}

bool CTableGuild::GetChallInfo(BYTE byLevel, DWORD& dwGuildID1, DWORD& dwGuildID2, DWORD& dwMoney) {
	string buf[3];
	char filter[80];

	DWORD dwGuildID = 0;
	DWORD dwChallID = 0;
	DWORD dwChallMoney = 0;
	const char* param = "guild_id, challid, challmoney";
	sprintf(filter, "challlevel =%d", byLevel);
	int l_retrow = 0;
	bool l_ret = _get_row(buf, 3, param, filter, &l_retrow);
	if (l_retrow == 1) {
		dwGuildID1 = atoi(buf[0].c_str());
		dwGuildID2 = atoi(buf[1].c_str());
		dwMoney = atoi(buf[2].c_str());

		return true;
	}
	return false;
}

bool PlayerStorage::SetGuildPermission(int atorID, unsigned long permission, int guild_id) {
	_snprintf_s(g_sql, sizeof(g_sql), _TRUNCATE,
				"update %s set guild_permission='%d' where atorID=%d and guild_id = %d",
				_get_table(), permission, atorID, guild_id);

	if (strlen(g_sql) >= SQL_MAXLEN)
		return false;

	short sExec = exec_sql_direct(g_sql);
	if (!DBOK(sExec))
		return false;

	if (DBNODATA(sExec))
		return false;

	return true;
}

bool PlayerStorage::SetChaAddr(DWORD atorID, Long addr) {
	_snprintf_s(g_sql, sizeof(g_sql), _TRUNCATE, "update %s set \
		endeMem='%d'\
		where atorID=%d",
				_get_table(), -1, atorID);

	if (strlen(g_sql) >= SQL_MAXLEN)
		return false;

	short sExec = exec_sql_direct(g_sql);
	if (!DBOK(sExec))
		return false;

	if (DBNODATA(sExec))
		return false;

	return true;
}

//===============CTableGuild End===========================================
//	2008-7-28	yyy	add	function	begin!


// CGameDB — методы вынесенные из GameDB.h
// ============================================================================

CGameDB::CGameDB()
	: _tab_cha(_db, _characters)
	  , _tab_res(_db)
	  , _tab_mmask(_db)
	  , _tab_gld(_db)
	  , _tab_boat(_db)
	  , _accounts(_db)
	  , _amphiSettings(_db)
	  , _amphiTeams(_db)
	  , _boats(_db)
	  , _characters(_db)
	  , _characterLogs(_db)
	  , _friends(_db)
	  , _guilds(_db)
	  , _lotterySettings(_db)
	  , _mapMasks(_db)
	  , _masters(_db)
	  , _params(_db)
	  , _personAvatars(_db)
	  , _personInfo(_db)
	  , _properties(_db)
	  , _resources(_db)
	  , _statLogs(_db)
	  , _statDegrees(_db)
	  , _statGenders(_db)
	  , _statJobs(_db)
	  , _statLogins(_db)
	  , _statMaps(_db)
	  , _tickets(_db)
	  , _tradeLogs(_db)
	  , _weekReports(_db)
	  , _winTickets(_db) {
}

CGameDB::~CGameDB() = default;

OdbcTransaction CGameDB::BeginTransaction() {
	return _db.BeginTransaction();
}

bool CGameDB::BeginTran() {
	SQLSetConnectAttr(_db.GetHandle(), SQL_ATTR_AUTOCOMMIT, reinterpret_cast<SQLPOINTER>(SQL_AUTOCOMMIT_OFF),
					  SQL_IS_UINTEGER);
	return true;
}

bool CGameDB::RollBack() {
	SQLEndTran(SQL_HANDLE_DBC, _db.GetHandle(), SQL_ROLLBACK);
	SQLSetConnectAttr(_db.GetHandle(), SQL_ATTR_AUTOCOMMIT, reinterpret_cast<SQLPOINTER>(SQL_AUTOCOMMIT_ON),
					  SQL_IS_UINTEGER);
	return true;
}

bool CGameDB::CommitTran() {
	SQLEndTran(SQL_HANDLE_DBC, _db.GetHandle(), SQL_COMMIT);
	SQLSetConnectAttr(_db.GetHandle(), SQL_ATTR_AUTOCOMMIT, reinterpret_cast<SQLPOINTER>(SQL_AUTOCOMMIT_ON),
					  SQL_IS_UINTEGER);
	return true;
}

bool CGameDB::SavePlayerKitbag(CPlayer* pPlayer, char chSaveType) {
	if (!_tab_res.SaveKitbagData(pPlayer->GetMainCha())) {
		return false;
	}
	if (!_tab_boat.SaveAllCabin(pPlayer, chSaveType)) {
		return false;
	}
	return true;
}

bool CGameDB::SaveChaAssets(CCharacter* pCCha) {
	if (!pCCha || !pCCha->GetPlayer()) {
		return false;
	}
	DWORD dwStartTick = GetTickCount();
	if (!_tab_cha.SaveMoney(pCCha->GetPlayer())) {
		return false;
	}
	if (!pCCha->IsBoat()) {
		if (!_tab_res.SaveKitbagData(pCCha)) {
			return false;
		}
	}
	else {
		if (!_tab_boat.SaveCabin(*pCCha, enumSAVE_TYPE_TRADE)) {
			return false;
		}
	}
	ToLogService("common", "Save assets {} in {} ms", pCCha->GetLogName(), GetTickCount() - dwStartTick);
	return true;
}

bool CGameDB::GetWinItemno(int issue, std::string& itemno) {
	try {
		itemno = _db.CreateCommand("SELECT itemno FROM LotterySetting WHERE state = 0 AND issue = ?")
					.SetParam(1, issue)
					.ExecuteScalar();
		return !itemno.empty();
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "GetWinItemno failed: {}", e.what());
		return false;
	}
}

bool CGameDB::GetLotteryIssue(int& issue) {
	try {
		auto result = _db.CreateCommand("SELECT issue FROM LotterySetting WHERE state = 0").ExecuteScalar();
		if (result.empty()) {
			return false;
		}
		issue = std::stoi(result);
		return true;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "GetLotteryIssue failed: {}", e.what());
		return false;
	}
}

bool CGameDB::AddIssue(int issue) {
	try {
		_db.CreateCommand(
			   "INSERT INTO LotterySetting (section, issue, state, createdate, updatetime) VALUES (1, ?, 0, getdate(), getdate())")
		   .SetParam(1, issue)
		   .ExecuteNonQuery();
		return true;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "AddIssue failed: {}", e.what());
		return false;
	}
}

bool CGameDB::DisuseIssue(int issue, int state) {
	try {
		return _db.CreateCommand("UPDATE LotterySetting SET state = ?, updatetime = getdate() WHERE issue = ?")
				  .SetParam(1, state)
				  .SetParam(2, issue)
				  .ExecuteNonQuery() > 0;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "DisuseIssue failed: {}", e.what());
		return false;
	}
}

bool CGameDB::LotteryIsExsit(int issue, char* itemno) {
	try {
		auto result = _db.CreateCommand("SELECT COUNT(*) FROM Ticket WHERE issue = ? AND itemno = ?")
						 .SetParam(1, issue)
						 .SetParam(2, std::string_view(itemno))
						 .ExecuteScalar();
		return !result.empty() && std::stoi(result) > 0;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "LotteryIsExsit failed: {}", e.what());
		return false;
	}
}

bool CGameDB::AddLotteryTicket(CCharacter* pCCha, int issue, char itemno[6][2]) {
	try {
		int xIndex = -1;
		for (int i = 0; i < 6; i++) {
			if (itemno[i][0] == 'X') {
				xIndex = i;
				break;
			}
		}
		if (xIndex >= 0) {
			for (int d = 0; d < 10; d++) {
				char no[7]{};
				for (int j = 0; j < 6; j++) {
					no[j] = (j == xIndex) ? ('0' + d) : itemno[j][0];
				}
				_db.CreateCommand(
					   "INSERT INTO Ticket (atorID, issue, itemno, real, buydate) VALUES (?, ?, ?, 0, getdate())")
				   .SetParam(1, pCCha->m_ID)
				   .SetParam(2, issue)
				   .SetParam(3, std::string_view(no, 6))
				   .ExecuteNonQuery();
			}
		}
		char mainNo[7]{};
		for (int j = 0; j < 6; j++) {
			mainNo[j] = itemno[j][0];
		}
		_db.CreateCommand("INSERT INTO Ticket (atorID, issue, itemno, real, buydate) VALUES (?, ?, ?, 1, getdate())")
		   .SetParam(1, pCCha->m_ID)
		   .SetParam(2, issue)
		   .SetParam(3, std::string_view(mainNo, 6))
		   .ExecuteNonQuery();
		return true;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "AddLotteryTicket failed: {}", e.what());
		return false;
	}
}

bool CGameDB::CalWinTicket(int issue, int max, std::string& itemno) {
	try {
		int probability = rand() % 2 + 1;
		if (issue % probability == 0) {
			auto reader = _db.CreateCommand(
								 "SELECT TOP 10 itemno, num FROM ("
								 "  SELECT itemno, COUNT(*) AS num FROM Ticket WHERE issue = ? AND real = 0 GROUP BY itemno"
								 ") AS A WHERE num <= ? ORDER BY num")
							 .SetParam(1, issue)
							 .SetParam(2, max)
							 .ExecuteReader();
			std::vector<std::string> candidates;
			while (reader.Read()) {
				candidates.push_back(reader.GetString(0));
			}
			if (!candidates.empty()) {
				itemno = candidates[rand() % candidates.size()];
				_db.CreateCommand("UPDATE WinTicket SET num = num + 1 WHERE issue = ? AND itemno = ?")
				   .SetParam(1, issue)
				   .SetParam(2, std::string_view(itemno))
				   .ExecuteNonQuery();
				_db.CreateCommand("UPDATE LotterySetting SET itemno = ?, updatetime = getdate() WHERE issue = ?")
				   .SetParam(1, std::string_view(itemno))
				   .SetParam(2, issue)
				   .ExecuteNonQuery();
				return true;
			}
		}
		std::string buffer;
		do {
			buffer = std::format("{:06d}", rand() % 999999 + 1);
		}
		while (LotteryIsExsit(issue, buffer.data()));
		itemno = buffer;
		_db.CreateCommand("UPDATE LotterySetting SET itemno = ?, updatetime = getdate() WHERE issue = ?")
		   .SetParam(1, std::string_view(itemno))
		   .SetParam(2, issue)
		   .ExecuteNonQuery();
		return true;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "CalWinTicket failed: {}", e.what());
		return false;
	}
}

bool CGameDB::IsValidAmphitheaterTeam(int teamID, int captainID, int member1, int member2) {
	try {
		auto m1 = std::format("{},{}", member1, member2);
		auto m2 = std::format("{},{}", member2, member1);
		auto r = _db.CreateCommand(
						"SELECT COUNT(*) FROM AmphitheaterTeam WHERE id = ? AND captain = ? AND (member = ? OR member = ?)")
					.SetParam(1, teamID)
					.SetParam(2, captainID)
					.SetParam(3, std::string_view(m1))
					.SetParam(4, std::string_view(m2))
					.ExecuteScalar();
		return !r.empty() && std::stoi(r) > 0;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "IsValidAmphitheaterTeam: {}", e.what());
		return false;
	}
}

bool CGameDB::IsMasterRelation(int masterID, int prenticeID) {
	try {
		auto r = _db.CreateCommand("SELECT COUNT(*) FROM master WHERE cha_id1 = ? AND cha_id2 = ?")
					.SetParam(1, prenticeID)
					.SetParam(2, masterID)
					.ExecuteScalar();
		return !r.empty() && std::stoi(r) > 0;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "IsMasterRelation: {}", e.what());
		return false;
	}
}

bool CGameDB::GetAmphitheaterSeasonAndRound(int& season, int& round) {
	try {
		auto reader = _db.CreateCommand("SELECT season, [round] FROM AmphitheaterSetting WHERE state = 0").
						  ExecuteReader();
		if (reader.Read()) {
			season = reader.GetInt(0);
			round = reader.GetInt(1);
			return true;
		}
		return false;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "GetAmphitheaterSeasonAndRound: {}", e.what());
		return false;
	}
}

bool CGameDB::AddAmphitheaterSeason(int season) {
	try {
		_db.CreateCommand(
			   "INSERT INTO AmphitheaterSetting (section, season, [round], state, createdate, updatetime, winner) VALUES (1, ?, 1, 0, getdate(), getdate(), NULL)")
		   .SetParam(1, season)
		   .ExecuteNonQuery();
		return true;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "AddAmphitheaterSeason: {}", e.what());
		return false;
	}
}

bool CGameDB::DisuseAmphitheaterSeason(int season, int state, const char* winner) {
	try {
		return _db.CreateCommand(
					  "UPDATE AmphitheaterSetting SET state = ?, updatetime = getdate(), winner = ? WHERE season = ?")
				  .SetParam(1, state)
				  .SetParam(2, std::string_view(winner ? winner : ""))
				  .SetParam(3, season)
				  .ExecuteNonQuery() > 0;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "DisuseAmphitheaterSeason: {}", e.what());
		return false;
	}
}

bool CGameDB::UpdateAmphitheaterRound(int season, int round) {
	try {
		return _db.CreateCommand("UPDATE AmphitheaterSetting SET [round] = ?, updatetime = getdate() WHERE season = ?")
				  .SetParam(1, round)
				  .SetParam(2, season)
				  .ExecuteNonQuery() > 0;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "UpdateAmphitheaterRound: {}", e.what());
		return false;
	}
}

bool CGameDB::GetAmphitheaterTeamCount(int& count) {
	try {
		auto r = _db.CreateCommand(std::format("SELECT COUNT(*) FROM AmphitheaterTeam WHERE state > {}",
											   static_cast<int>(AmphitheaterTeam::enumNotUse))).ExecuteScalar();
		count = r.empty() ? -1 : std::stoi(r);
		return count >= 0;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "GetAmphitheaterTeamCount: {}", e.what());
		return false;
	}
}

bool CGameDB::GetAmphitheaterNoUseTeamID(int& teamID) {
	try {
		auto r = _db.CreateCommand(std::format("SELECT TOP(1) id FROM AmphitheaterTeam WHERE state = {}",
											   static_cast<int>(AmphitheaterTeam::enumNotUse))).ExecuteScalar();
		if (r.empty()) {
			return false;
		}
		teamID = std::stoi(r);
		return true;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "GetAmphitheaterNoUseTeamID: {}", e.what());
		return false;
	}
}

bool CGameDB::AmphitheaterTeamSignUP(int& teamID, int captain, int member1, int member2) {
	try {
		if (teamID < 0 && !GetAmphitheaterNoUseTeamID(teamID)) {
			return false;
		}
		auto memberStr = std::format("{},{}", member1, member2);
		return _db.CreateCommand(std::format(
					  "UPDATE AmphitheaterTeam SET captain = ?, member = ?, state = {}, updatetime = getdate() WHERE id = ?",
					  static_cast<int>(AmphitheaterTeam::enumUse)))
				  .SetParam(1, captain)
				  .SetParam(2, std::string_view(memberStr))
				  .SetParam(3, teamID)
				  .ExecuteNonQuery() > 0;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "AmphitheaterTeamSignUP: {}", e.what());
		return false;
	}
}

bool CGameDB::AmphitheaterTeamCancel(int teamID) {
	try {
		return _db.CreateCommand(std::format(
					  "UPDATE AmphitheaterTeam SET captain = null, member = null, matchno = 0, state = {}, updatetime = getdate() WHERE id = ?",
					  static_cast<int>(AmphitheaterTeam::enumNotUse)))
				  .SetParam(1, teamID)
				  .ExecuteNonQuery() > 0;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "AmphitheaterTeamCancel: {}", e.what());
		return false;
	}
}

bool CGameDB::IsAmphitheaterLogin(int pActorID) {
	try {
		auto idStr = std::to_string(pActorID);
		auto like1 = idStr + ",%";
		auto like2 = "%," + idStr;
		auto r = _db.CreateCommand(
						"SELECT COUNT(*) FROM AmphitheaterTeam WHERE captain = ? OR member LIKE ? OR member LIKE ?")
					.SetParam(1, pActorID)
					.SetParam(2, std::string_view(like1))
					.SetParam(3, std::string_view(like2))
					.ExecuteScalar();
		return r.empty() || std::stoi(r) == 0;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "IsAmphitheaterLogin: {}", e.what());
		return false;
	}
}

bool CGameDB::IsMapFull(int MapID, int& PActorIDNum) {
	try {
		auto r = _db.CreateCommand("SELECT COUNT(*) FROM AmphitheaterTeam WHERE map = ?")
					.SetParam(1, MapID)
					.ExecuteScalar();
		PActorIDNum = r.empty() ? 0 : std::stoi(r);
		return PActorIDNum <= 2;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "IsMapFull: {}", e.what());
		return false;
	}
}

bool CGameDB::UpdateMapNum(int Teamid, int Mapid, int MapFlag) {
	try {
		return _db.CreateCommand("UPDATE AmphitheaterTeam SET mapflag = ? WHERE id = ? AND map = ?")
				  .SetParam(1, MapFlag)
				  .SetParam(2, Teamid)
				  .SetParam(3, Mapid)
				  .ExecuteNonQuery() > 0;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "UpdateMapNum: {}", e.what());
		return false;
	}
}

bool CGameDB::GetMapFlag(int Teamid, int& Mapflag) {
	try {
		auto r = _db.CreateCommand("SELECT mapflag FROM AmphitheaterTeam WHERE id = ?")
					.SetParam(1, Teamid)
					.ExecuteScalar();
		if (r.empty()) {
			return false;
		}
		Mapflag = std::stoi(r);
		return Mapflag < 2;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "GetMapFlag: {}", e.what());
		return false;
	}
}

bool CGameDB::SetMaxBallotTeamRelive() {
	try {
		auto countStr = _db.CreateCommand(std::format("SELECT COUNT(*) FROM AmphitheaterTeam WHERE state = {}",
													  static_cast<int>(AmphitheaterTeam::enumPromotion))).
							ExecuteScalar();
		int count = countStr.empty() ? 0 : std::stoi(countStr);
		int oddOrEven = (count % 2 == 0) ? 2 : 1;
		_db.CreateCommand(std::format(
			"UPDATE AmphitheaterTeam SET state = {}, relivenum = 0 WHERE id IN "
			"(SELECT TOP {} id FROM AmphitheaterTeam WHERE state = {} ORDER BY relivenum DESC)",
			static_cast<int>(AmphitheaterTeam::enumPromotion), oddOrEven,
			static_cast<int>(AmphitheaterTeam::enumRelive))).ExecuteNonQuery();
		_db.CreateCommand(std::format(
			"UPDATE AmphitheaterTeam SET state = {} WHERE state = {} OR state = {}",
			static_cast<int>(AmphitheaterTeam::enumOut), static_cast<int>(AmphitheaterTeam::enumRelive),
			static_cast<int>(AmphitheaterTeam::enumUse))).ExecuteNonQuery();
		return true;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "SetMaxBallotTeamRelive: {}", e.what());
		return false;
	}
}

bool CGameDB::SetMatchResult(int Teamid1, int Teamid2, int Id1state, int Id2state) {
	try {
		_db.CreateCommand("UPDATE AmphitheaterTeam SET state = ? WHERE id = ?")
		   .SetParam(1, Id1state)
		   .SetParam(2, Teamid1)
		   .ExecuteNonQuery();
		_db.CreateCommand("UPDATE AmphitheaterTeam SET state = ? WHERE id = ?")
		   .SetParam(1, Id2state)
		   .SetParam(2, Teamid2)
		   .ExecuteNonQuery();
		return true;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "SetMatchResult: {}", e.what());
		return false;
	}
}

bool CGameDB::GetCaptainByMapId(int Mapid, std::string& c1, std::string& c2) {
	try {
		auto reader = _db.CreateCommand("SELECT captain FROM AmphitheaterTeam WHERE map = ?")
						 .SetParam(1, Mapid)
						 .ExecuteReader();
		std::vector<std::string> caps;
		while (reader.Read()) {
			caps.push_back(reader.GetString(0));
		}
		if (caps.empty() || caps.size() > 2) {
			return false;
		}
		c1 = caps[0];
		c2 = caps.size() > 1 ? caps[1] : "";
		return true;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "GetCaptainByMapId: {}", e.what());
		return false;
	}
}

bool CGameDB::UpdateMap(int Mapid) {
	try {
		return _db.CreateCommand("UPDATE AmphitheaterTeam SET map = null WHERE map = ?")
				  .SetParam(1, Mapid)
				  .ExecuteNonQuery() > 0;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "UpdateMap: {}", e.what());
		return false;
	}
}

bool CGameDB::UpdateMapAfterEnter(int CaptainID, int MapID) {
	try {
		return _db.CreateCommand("UPDATE AmphitheaterTeam SET map = ? WHERE captain = ?")
				  .SetParam(1, MapID)
				  .SetParam(2, CaptainID)
				  .ExecuteNonQuery() > 0;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "UpdateMapAfterEnter: {}", e.what());
		return false;
	}
}

bool CGameDB::GetPromotionAndReliveTeam(std::vector<std::vector<std::string>>& dataP,
										std::vector<std::vector<std::string>>& dataR) {
	try {
		auto r1 = _db.CreateCommand(std::format(
			"SELECT B.atorNome, A.id, A.winnum FROM AmphitheaterTeam A, character B WHERE B.atorID = A.captain AND A.state = {} ORDER BY A.winnum DESC",
			static_cast<int>(AmphitheaterTeam::enumPromotion))).ExecuteReader();
		while (r1.Read()) {
			dataP.push_back({r1.GetString(0), r1.GetString(1), r1.GetString(2)});
		}
		auto r2 = _db.CreateCommand(std::format(
			"SELECT B.atorNome, A.relivenum, A.id FROM AmphitheaterTeam A, character B WHERE B.atorID = A.captain AND A.state = {} ORDER BY A.relivenum DESC",
			static_cast<int>(AmphitheaterTeam::enumRelive))).ExecuteReader();
		while (r2.Read()) {
			dataR.push_back({r2.GetString(0), r2.GetString(1), r2.GetString(2)});
		}
		return true;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "GetPromotionAndReliveTeam: {}", e.what());
		return false;
	}
}

bool CGameDB::UpdatReliveNum(int ReID) {
	try {
		auto r = _db.CreateCommand("SELECT relivenum FROM AmphitheaterTeam WHERE id = ?")
					.SetParam(1, ReID)
					.ExecuteScalar();
		if (r.empty()) {
			return false;
		}
		return _db.CreateCommand("UPDATE AmphitheaterTeam SET relivenum = ? WHERE id = ?")
				  .SetParam(1, std::stoi(r) + 1)
				  .SetParam(2, ReID)
				  .ExecuteNonQuery() > 0;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "UpdatReliveNum: {}", e.what());
		return false;
	}
}

bool CGameDB::UpdateAbsentTeamRelive() {
	try {
		return _db.CreateCommand(std::format("UPDATE AmphitheaterTeam SET state = {} WHERE state = {}",
											 static_cast<int>(AmphitheaterTeam::enumRelive),
											 static_cast<int>(AmphitheaterTeam::enumUse))).ExecuteNonQuery() > 0;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "UpdateAbsentTeamRelive: {}", e.what());
		return false;
	}
}

bool CGameDB::UpdateWinnum(int teamid) {
	try {
		return _db.CreateCommand("UPDATE AmphitheaterTeam SET winnum = winnum + 1 WHERE id = ?")
				  .SetParam(1, teamid)
				  .ExecuteNonQuery() > 0;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "UpdateWinnum: {}", e.what());
		return false;
	}
}

bool CGameDB::GetUniqueMaxWinnum(int& teamid) {
	try {
		auto reader = _db.CreateCommand(
							 "SELECT id FROM AmphitheaterTeam WHERE winnum IN (SELECT MAX(winnum) FROM AmphitheaterTeam)")
						 .
						 ExecuteReader();
		std::vector<int> ids;
		while (reader.Read()) {
			ids.push_back(reader.GetInt(0));
		}
		if (ids.size() != 1) {
			return false;
		}
		teamid = ids[0];
		return true;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "GetUniqueMaxWinnum: {}", e.what());
		return false;
	}
}

bool CGameDB::SetMatchnoState(int teamid) {
	try {
		return _db.CreateCommand("UPDATE AmphitheaterTeam SET matchno = 1 WHERE id = ?")
				  .SetParam(1, teamid)
				  .ExecuteNonQuery() > 0;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "SetMatchnoState: {}", e.what());
		return false;
	}
}

bool CGameDB::UpdateState() {
	try {
		return _db.CreateCommand(std::format("UPDATE AmphitheaterTeam SET state = {} WHERE state = {}",
											 static_cast<int>(AmphitheaterTeam::enumUse),
											 static_cast<int>(AmphitheaterTeam::enumPromotion))).ExecuteNonQuery() > 0;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "UpdateState: {}", e.what());
		return false;
	}
}

bool CGameDB::CloseReliveByState(int& statenum) {
	try {
		auto r = _db.CreateCommand(std::format("SELECT COUNT(*) FROM AmphitheaterTeam WHERE state = {}",
											   static_cast<int>(AmphitheaterTeam::enumUse))).ExecuteScalar();
		statenum = r.empty() ? 0 : std::stoi(r);
		return true;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "CloseReliveByState: {}", e.what());
		return false;
	}
}

bool CGameDB::CleanMapFlag(int teamid1, int teamid2) {
	try {
		return _db.CreateCommand("UPDATE AmphitheaterTeam SET mapflag = null WHERE id = ? OR id = ?")
				  .SetParam(1, teamid1)
				  .SetParam(2, teamid2)
				  .ExecuteNonQuery() > 0;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "CleanMapFlag: {}", e.what());
		return false;
	}
}

bool CGameDB::GetStateByTeamid(int teamid, int& state) {
	try {
		auto r = _db.CreateCommand("SELECT state FROM AmphitheaterTeam WHERE id = ?")
					.SetParam(1, teamid)
					.ExecuteScalar();
		if (r.empty()) {
			return false;
		}
		state = std::stoi(r);
		return true;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "GetStateByTeamid: {}", e.what());
		return false;
	}
}

bool CGameDB::UpdateIMP(CPlayer* ply) {
	try {
		return _db.CreateCommand("UPDATE character SET IMP = ? WHERE atorID = ?")
				  .SetParam(1, ply->GetMainCha()->GetIMP())
				  .SetParam(2, ply->GetMainCha()->GetID())
				  .ExecuteNonQuery() > 0;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "UpdateIMP: {}", e.what());
		return false;
	}
}

bool CGameDB::SaveGmLv(CPlayer* ply) {
	try {
		return _db.CreateCommand("UPDATE account SET jmes = ? WHERE ato_id = ?")
				  .SetParam(1, ply->GetGMLev())
				  .SetParam(2, ply->GetDBActId())
				  .ExecuteNonQuery() > 0;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "SaveGmLv: {}", e.what());
		return false;
	}
}

unsigned long CGameDB::GetPlayerMasterDBID(CPlayer* pPlayer) {
	if (!pPlayer || !pPlayer->GetMainCha()) {
		return 0;
	}
	try {
		auto r = _db.CreateCommand("SELECT cha_id2 FROM master WHERE cha_id1 = ?")
					.SetParam(1, pPlayer->GetDBChaId())
					.ExecuteScalar();
		return r.empty() ? 0 : static_cast<unsigned long>(std::stol(r));
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "GetPlayerMasterDBID: {}", e.what());
		return 0;
	}
}

bool CGameDB::CreatePlyBank(CPlayer* pCPly) {
	if (pCPly->GetCurBankNum() >= MAX_BANK_NUM) {
		return false;
	}
	try {
		_db.CreateCommand("INSERT INTO resource (atorID, type_id) VALUES (?, ?)")
		   .SetParam(1, pCPly->GetDBChaId())
		   .SetParam(2, static_cast<int>(enumRESDB_TYPE_BANK))
		   .ExecuteNonQuery();
		auto idStr = _db.CreateCommand("SELECT @@IDENTITY").ExecuteScalar();
		long lBankDBID = std::stol(idStr);
		pCPly->AddBankDBID(lBankDBID);
		if (!_tab_cha.SaveBankDBID(pCPly)) {
			return false;
		}
		return true;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "CreatePlyBank: {}", e.what());
		return false;
	}
}

bool CGameDB::SavePlyBank(CPlayer* pCPly, char chBankNO) {
	return _tab_res.SaveBankData(pCPly, chBankNO);
}

BOOL CGameDB::ReadKitbagTmpData(DWORD res_id, std::string& strData) {
	if (res_id == 0) {
		return FALSE;
	}
	try {
		strData = _db.CreateCommand("SELECT content FROM resource WHERE id = ?")
					 .SetParam(1, res_id)
					 .ExecuteScalar();
		return TRUE;
	}
	catch (const OdbcException& e) {
		ToLogService("store", LogLevel::Error, "ReadKitbagTmpData: {}", e.what());
		return FALSE;
	}
}

BOOL CGameDB::SaveKitbagTmpData(DWORD res_id, const std::string& strData) {
	if (res_id == 0) {
		return FALSE;
	}
	try {
		int affected = _db.CreateCommand("UPDATE resource SET content = ? WHERE id = ?")
						  .SetParam(1, std::string_view(strData))
						  .SetParam(2, res_id)
						  .ExecuteNonQuery();
		if (affected == 0) {
			ToLogService("store", "Database couldn't find temp kitbag resource {}!", res_id);
			return FALSE;
		}
		return TRUE;
	}
	catch (const OdbcException& e) {
		ToLogService("store", LogLevel::Error, "SaveKitbagTmpData: {}", e.what());
		return FALSE;
	}
}

bool CGameDB::StartChall(BYTE byLevel) {
	for (int i = 0; i < 100; i++) {
		if (_tab_gld.StartChall(byLevel)) {
			return true;
		}
	}
	return false;
}

bool CGameDB::GetChall(BYTE byLevel, DWORD& dwGuildID1, DWORD& dwGuildID2, DWORD& dwMoney) {
	for (int i = 0; i < 100; i++) {
		if (_tab_gld.GetChallInfo(byLevel, dwGuildID1, dwGuildID2, dwMoney)) {
			return true;
		}
	}
	return false;
}

void CGameDB::ExecLogSQL(const char* pszSQL) {
	try {
		_db.CreateCommand(pszSQL).ExecuteNonQuery();
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "ExecLogSQL failed: {}", e.what());
	}
}

void CGameDB::ExecTradeLogSQL(const char* gameServerName, const char* action,
							  const char* pszChaFrom, const char* pszChaTo, const char* pszTrade) {
	try {
		SYSTEMTIME st;
		GetLocalTime(&st);
		auto timeStr = std::format("{:04}/{:02}/{:02} {:02}:{:02}:{:02}",
								   st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
		auto cmd = _db.CreateCommand(
			"INSERT INTO Trade_Log (ExecuteTime, GameServer, [Action], [From], [To], Memo) "
			"VALUES (@time, @server, @action, @from, @to, @memo)");
		cmd.SetParam("@time", std::string_view(timeStr));
		cmd.SetParam("@server", std::string_view(gameServerName));
		cmd.SetParam("@action", std::string_view(action));
		cmd.SetParam("@from", std::string_view(pszChaFrom));
		cmd.SetParam("@to", std::string_view(pszChaTo));
		cmd.SetParam("@memo", std::string_view(pszTrade));
		cmd.ExecuteNonQuery();
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "ExecTradeLogSQL failed: {}", e.what());
	}
}

// ============================================================================
// CGameDB — делегаторы к PlayerStorage
// ============================================================================

std::string CGameDB::GetChaNameByID(int cha_id) {
	return _tab_cha.GetName(cha_id);
}

void CGameDB::ShowExpRank(CCharacter* pCha, int top) {
	_tab_cha.ShowExpRank(pCha, top);
}

bool CGameDB::SavePlayerPos(CPlayer* pPlayer) {
	return _tab_cha.SavePos(pPlayer);
}

bool CGameDB::SavePlayerKBagDBID(CPlayer* pPlayer) {
	return _tab_cha.SaveKBagDBID(pPlayer);
}

bool CGameDB::SavePlayerKBagTmpDBID(CPlayer* pPlayer) {
	return _tab_cha.SaveKBagTmpDBID(pPlayer);
}

bool CGameDB::SavePlayerMMaskDBID(CPlayer* pPlayer) {
	return _tab_cha.SaveMMaskDBID(pPlayer);
}

BOOL CGameDB::AddCreditByDBID(DWORD atorID, long lCredit) {
	return _tab_cha.AddCreditByDBID(atorID, lCredit);
}

BOOL CGameDB::SaveStoreItemID(DWORD atorID, long lStoreItemID) {
	return _tab_cha.SaveStoreItemID(atorID, lStoreItemID);
}

BOOL CGameDB::AddMoney(DWORD atorID, long money) {
	return _tab_cha.AddMoney(atorID, money);
}

BOOL CGameDB::IsChaOnline(DWORD atorID, BOOL& bOnline) {
	return _tab_cha.IsChaOnline(atorID, bOnline);
}

Long CGameDB::GetChaAddr(DWORD atorID) {
	return _tab_cha.GetChaAddr(atorID);
}

Long CGameDB::SetGuildPermission(int atorID, unsigned long perm, int guild_id) {
	return _tab_cha.SetGuildPermission(atorID, perm, guild_id);
}

Long CGameDB::SetChaAddr(DWORD atorID, Long addr) {
	return _tab_cha.SetChaAddr(atorID, addr);
}

BOOL CGameDB::SaveMissionData(CPlayer* pPlayer, DWORD atorID) {
	return _tab_cha.SaveMissionData(pPlayer, atorID);
}

// ============================================================================
// CGameDB — делегаторы к CTableBoat
// ============================================================================

BOOL CGameDB::Create(DWORD& dwBoatID, const BOAT_DATA& Data) {
	return _tab_boat.Create(dwBoatID, Data);
}

BOOL CGameDB::GetBoat(CCharacter& Boat) {
	return _tab_boat.GetBoat(Boat);
}

BOOL CGameDB::SaveBoat(CCharacter& Boat, char chSaveType) {
	return _tab_boat.SaveBoat(Boat, chSaveType);
}

BOOL CGameDB::SaveBoatDelTag(DWORD dwBoatID, BYTE byIsDeleted) {
	return _tab_boat.SaveBoatDelTag(dwBoatID, byIsDeleted);
}

BOOL CGameDB::SaveBoatTempData(CCharacter& Boat, BYTE byIsDeleted) {
	return _tab_boat.SaveBoatTempData(Boat, byIsDeleted);
}

BOOL CGameDB::SaveBoatTempData(DWORD dwBoatID, DWORD dwOwnerID, BYTE byIsDeleted) {
	return _tab_boat.SaveBoatTempData(dwBoatID, dwOwnerID, byIsDeleted);
}

// ============================================================================
// CGameDB — делегаторы к CTableGuild
// ============================================================================

long CGameDB::CreateGuild(CCharacter* pCha, char* guildname, cChar* passwd) {
	return _tab_gld.Create(pCha, guildname, passwd);
}

long CGameDB::GetGuildBank(uLong guildid, CKitbag* bag) {
	return _tab_gld.GetGuildBank(guildid, bag);
}

long CGameDB::UpdateGuildBank(uLong guildid, CKitbag* bag) {
	return _tab_gld.UpdateGuildBank(guildid, bag);
}

bool CGameDB::SetGuildLog(std::vector<CTableGuild::BankLog> log, uLong guildid) {
	return _tab_gld.SetGuildLog(log, guildid);
}

std::vector<CTableGuild::BankLog> CGameDB::GetGuildLog(uLong guildid) {
	return _tab_gld.GetGuildLog(guildid);
}

unsigned long long CGameDB::GetGuildBankGold(uLong guildid) {
	return _tab_gld.GetGuildBankGold(guildid);
}

bool CGameDB::UpdateGuildBankGold(int guildID, int money) {
	return _tab_gld.UpdateGuildBankGold(guildID, money);
}

int CGameDB::GetGuildLeaderID(uLong guildid) {
	return _tab_gld.GetGuildLeaderID(guildid);
}

bool CGameDB::ListAllGuild(CCharacter* pCha, char disband_days) {
	return _tab_gld.ListAll(pCha, disband_days);
}

void CGameDB::GuildTryFor(CCharacter* pCha, uLong guildid) {
	_tab_gld.TryFor(pCha, guildid);
}

void CGameDB::GuildTryForConfirm(CCharacter* pCha, uLong guildid) {
	_tab_gld.TryForConfirm(pCha, guildid);
}

bool CGameDB::GuildListTryPlayer(CCharacter* pCha, char disband_days) {
	return _tab_gld.ListTryPlayer(pCha, disband_days);
}

bool CGameDB::GuildApprove(CCharacter* pCha, uLong chaid) {
	return _tab_gld.Approve(pCha, chaid);
}

bool CGameDB::GuildReject(CCharacter* pCha, uLong chaid) {
	return _tab_gld.Reject(pCha, chaid);
}

bool CGameDB::GuildKick(CCharacter* pCha, uLong chaid) {
	return _tab_gld.Kick(pCha, chaid);
}

bool CGameDB::GuildLeave(CCharacter* pCha) {
	return _tab_gld.Leave(pCha);
}

bool CGameDB::GuildDisband(CCharacter* pCha, cChar* passwd) {
	return _tab_gld.Disband(pCha, passwd);
}

bool CGameDB::GuildMotto(CCharacter* pCha, cChar* motto) {
	return _tab_gld.Motto(pCha, motto);
}

CTableMapMask* CGameDB::GetMapMaskTable() {
	return &_tab_mmask;
}

bool CGameDB::GetGuildName(long lGuildID, std::string& strGuildName) {
	return _tab_gld.GetGuildName(lGuildID, strGuildName);
}

bool CGameDB::Challenge(CCharacter* pCha, BYTE byLevel, DWORD dwMoney) {
	return _tab_gld.Challenge(pCha, byLevel, dwMoney);
}

bool CGameDB::Leizhu(CCharacter* pCha, BYTE byLevel, DWORD dwMoney) {
	return _tab_gld.Leizhu(pCha, byLevel, dwMoney);
}

void CGameDB::ListChallenge(CCharacter* pCha) {
	_tab_gld.ListChallenge(pCha);
}

void CGameDB::EndChall(DWORD dwGuild1, DWORD dwGuild2, BOOL bChall) {
	_tab_gld.EndChall(dwGuild1, dwGuild2, bChall);
}

bool CGameDB::HasChall(BYTE byLevel) {
	return _tab_gld.HasCall(byLevel);
}

bool CGameDB::HasGuildLevel(CCharacter* pChar, BYTE byLevel) {
	return _tab_gld.HasGuildLevel(pChar, byLevel);
}
