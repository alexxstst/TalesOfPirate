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
// PlayerStorage
// ============================================================================

//-------------------
//
//-------------------
bool PlayerStorage::VerifyName(const std::string& pszName) {
	auto row = _characters.FindOne("atorNome = ?", std::string_view(pszName));
	return row.has_value();
}

std::string PlayerStorage::GetName(std::int32_t cha_id) {
	auto row = _characters.FindOne("atorID = ?", cha_id);
	return row ? row->atorNome : "";
}

#define defKITBAG_DATA_STRING_LEN	8192
#define defSKILLBAG_DATA_STRING_LEN	1500
#define defSHORTCUT_DATA_STRING_LEN	1500
#define defSSTATE_DATE_STRING_LIN	1024

// g_sql и g_buf удалены — PlayerStorage теперь использует _characters (OdbcTable)
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
	try {
		// Проверяем доступность таблицы character
		_db.CreateCommand("SELECT TOP 0 atorID FROM character").ExecuteNonQuery();
		return true;
	}
	catch (const OdbcException& e) {
		char buffer[255];
		std::snprintf(buffer, sizeof(buffer), RES_STRING(GM_GAMEDB_CPP_00001), "character");
		MessageBox(0, buffer, RES_STRING(GM_GAMEDB_CPP_00002), MB_OK);
		ToLogService("db", LogLevel::Error, "PlayerStorage::Init failed: {}", e.what());
		return false;
	}
}

bool PlayerStorage::ShowExpRank(CCharacter& pCha, std::int32_t count) {
	try {
		auto sql = std::format(
			"SELECT TOP {} atorNome, job, degree FROM character "
			"WHERE delflag = 0 ORDER BY CASE WHEN (exp < 0) THEN (exp+4294967296) ELSE exp END DESC",
			count);
		auto reader = _db.CreateCommand(sql).ExecuteReader();

		net::msg::McShowRankingMessage msg;
		while (reader.Read()) {
			msg.entries.push_back({
				reader.GetString(0),
				reader.GetString(1),
				static_cast<int64_t>(reader.GetInt(2)),
				0, 0
			});
		}

		auto l_wpk = net::msg::serialize(msg);
		pCha.ReflectINFof(&pCha, l_wpk);
		return true;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "ShowExpRank failed: {}", e.what());
		return false;
	}
}

//-----------------------
//
//-----------------------
bool PlayerStorage::ReadAllData(CPlayer& player, std::uint32_t atorID) {
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
bool PlayerStorage::SaveAllData(CPlayer& pPlayer, char chSaveType, bool bForceWithPos) {
	if (!pPlayer.IsValid()) {
		return false;
	}
	CCharacter* pCha = pPlayer.GetMainCha();
	if (!pCha) {
		return false;
	}
	DWORD atorID = pPlayer.GetDBChaId();

	CCharacter* pCCtrlCha = pPlayer.GetCtrlCha();
	if (pPlayer.GetLoginChaType() == enumLOGIN_CHA_BOAT) {
		CCharacter* pCLogCha = pPlayer.GetBoat(pPlayer.GetLoginChaID());
		if (pCLogCha != pCCtrlCha) {
			pCCtrlCha->SetToMainCha();
			pCCtrlCha = pCha;
			ToLogService("errors", LogLevel::Error, "SaveAllData: login/ctrl character mismatch for {}",
						 pCha->GetLogName());
			return false;
		}
	} else {
		if (pCha != pCCtrlCha) {
			pCCtrlCha = pCha;
			ToLogService("errors", LogLevel::Error, "SaveAllData: main/ctrl character mismatch for {}",
						 pCha->GetLogName());
			return false;
		}
	}

	ToLogService("map", "{} saving character data...", pCha->GetLogName());

	// Сериализация внешности
	std::string lookStr;
	if (!LookData2String(pCha->m_SChaPart, lookStr)) {
		ToLogService("map", "character {} save data (appearance) error!", pCha->GetLogName());
		return false;
	}

	// Скиллбаг
	char skillbagBuf[defSKILLBAG_DATA_STRING_LEN]{};
	if (!SkillBagData2String(&pCha->m_CSkillBag, skillbagBuf, defSKILLBAG_DATA_STRING_LEN)) {
		ToLogService("map", "character {} save data (skill) error!", pCha->GetLogName());
		return false;
	}

	// Шорткаты
	char shortcutBuf[defSHORTCUT_DATA_STRING_LEN]{};
	if (!ShortcutData2String(&pCha->m_CShortcut, shortcutBuf, defSHORTCUT_DATA_STRING_LEN)) {
		ToLogService("map", "character {} save data (shortcut) error!", pCha->GetLogName());
		return false;
	}

	// Миссии
	char misInfoBuf[ROLE_MAXSIZE_DBMISSION]{};
	if (!pPlayer.MisGetData(misInfoBuf, ROLE_MAXSIZE_DBMISSION - 1)) {
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00015), pCha->GetID());
		ToLogService("db", LogLevel::Error, "SaveAllData: MisGetData failed for atorID {}", atorID);
	}

	char misRecordBuf[ROLE_MAXSIZE_DBRECORD]{};
	if (!pPlayer.MisGetRecord(misRecordBuf, ROLE_MAXSIZE_DBRECORD - 1)) {
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00015), pCha->GetID());
		ToLogService("db", LogLevel::Error, "SaveAllData: MisGetRecord failed for atorID {}", atorID);
	}

	char misTriggerBuf[ROLE_MAXSIZE_DBTRIGGER]{};
	if (!pPlayer.MisGetTrigger(misTriggerBuf, ROLE_MAXSIZE_DBTRIGGER - 1)) {
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00016), pCha->GetID());
		ToLogService("db", LogLevel::Error, "SaveAllData: MisGetTrigger failed for atorID {}", atorID);
	}

	char misCountBuf[ROLE_MAXSIZE_DBMISCOUNT]{};
	if (!pPlayer.MisGetMissionCount(misCountBuf, ROLE_MAXSIZE_DBMISCOUNT - 1)) {
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00017), pCha->GetID());
		ToLogService("db", LogLevel::Error, "SaveAllData: MisGetMissionCount failed for atorID {}", atorID);
	}

	// Skill state
	char skillstateBuf[defSSTATE_DATE_STRING_LIN]{};
	if (chSaveType == enumSAVE_TYPE_OFFLINE) {
		SStateData2String(pCha, skillstateBuf, defSSTATE_DATE_STRING_LIN, chSaveType);
	} else if (!SStateData2String(pCha, skillstateBuf, defSSTATE_DATE_STRING_LIN, chSaveType)) {
		ToLogService("map", "character {} save data (skill_state) error!", pCha->GetLogName());
		return false;
	}

	// Расширенные атрибуты
	char extendBuf[ROLE_MAXSIZE_DBMISCOUNT]{};
	if (!ChaExtendAttr2String(pCha, extendBuf, ROLE_MAXSIZE_DBMISCOUNT)) {
		ToLogService("map", "character {} save data (extend attr) error!", pCha->GetLogName());
		return false;
	}

	// Позиция — сохраняем только если карта позволяет.
	// Вызывающая сторона может запросить сохранение через bForceWithPos —
	// нужно для offline-save после GoOut, когда GetSubMap() уже nullptr,
	// но мы хотим сохранить актуальные координаты (иначе при следующем входе
	// персонаж появится в старом месте).
	bool bWithPos = bForceWithPos
		|| (pCCtrlCha->GetSubMap() && pCCtrlCha->GetSubMap()->CanSavePos());

	// Читаем текущую строку из БД, чтобы сохранить неизменяемые поля
	auto existing = _characters.FindOne("atorID = ?", static_cast<int>(atorID));
	if (!existing) {
		ToLogService("map", "SaveAllData: character {} not found in database!", atorID);
		return false;
	}
	CharacterRow row = std::move(*existing);
	row.hp = pCha->getAttr(ATTR_HP);
	row.sp = pCha->getAttr(ATTR_SP);
	row.exp = pCha->getAttr(ATTR_CEXP);
	row.radius = pCha->GetShape().radius;
	row.angle = pCha->GetAngle();
	row.pk_ctrl = pCha->IsInPK();
	row.degree = pCha->getAttr(ATTR_LV);
	row.job = g_GetJobName(static_cast<short>(pCha->getAttr(ATTR_JOB)));
	row.bomd = pCha->getAttr(ATTR_GD);
	row.ap = pCha->getAttr(ATTR_AP);
	row.tp = pCha->getAttr(ATTR_TP);
	row.str = pCha->getAttr(ATTR_BSTR);
	row.dex = pCha->getAttr(ATTR_BDEX);
	row.agi = pCha->getAttr(ATTR_BAGI);
	row.con = pCha->getAttr(ATTR_BCON);
	row.sta = pCha->getAttr(ATTR_BSTA);
	row.luk = pCha->getAttr(ATTR_BLUK);
	row.sail_lv = pCha->getAttr(ATTR_SAILLV);
	row.sail_exp = pCha->getAttr(ATTR_CSAILEXP);
	row.sail_left_exp = pCha->getAttr(ATTR_CLEFT_SAILEXP);
	row.live_lv = pCha->getAttr(ATTR_LIFELV);
	row.live_exp = pCha->getAttr(ATTR_CLIFEEXP);
	row.live_tp = pCha->getAttr(ATTR_LIFETP);
	row.olhe = lookStr;
	row.skillbag = skillbagBuf;
	row.shortcut = shortcutBuf;
	row.mission = misInfoBuf;
	row.misrecord = misRecordBuf;
	row.mistrigger = misTriggerBuf;
	row.miscount = misCountBuf;
	row.birth = pCha->GetBirthCity();
	row.login_cha = std::format("{},{}", pPlayer.GetLoginChaType(), pPlayer.GetLoginChaID());
	row.kb_locked = pCha->m_CKitbag.GetPwdLockState();
	row.credit = pCha->GetCredit();
	row.store_item = pCha->GetStoreItemID();
	row.skill_state = skillstateBuf;
	row.extend = extendBuf;
	row.IMP = pCha->GetIMP();

	if (bWithPos) {
		row.map = pCCtrlCha->GetBirthMap();
		row.main_map = pCha->GetBirthMap();
		row.map_x = pCha->GetShape().centre.x;
		row.map_y = pCha->GetShape().centre.y;
	}

	int affected = _characters.Update(row);
	if (affected == 0) {
		ToLogService("map", "SaveAllData: character {} not found in database!", atorID);
		return false;
	}

	ToLogService("map", "Character data saved for {}", pCha->GetLogName());
	return true;
}

bool PlayerStorage::SavePos(CPlayer& pPlayer) {
	if (!pPlayer.IsValid()) {
		return false;
	}
	CCharacter* pCha = pPlayer.GetMainCha();
	CCharacter* pCCtrlCha = pPlayer.GetCtrlCha();
	if (!pCha || !pCCtrlCha) {
		return false;
	}
	_characters.Execute(
		"UPDATE character SET map=?, main_map=?, map_x=?, map_y=?, angle=? WHERE atorID=?",
		std::string_view(pCCtrlCha->GetBirthMap()),
		std::string_view(pCha->GetBirthMap()),
		static_cast<int>(pCha->GetPos().x),
		static_cast<int>(pCha->GetPos().y),
		static_cast<int>(pCha->GetAngle()),
		static_cast<int>(pPlayer.GetDBChaId()));
	return true;
}

bool PlayerStorage::SaveMoney(CPlayer& pPlayer) {
	if (!pPlayer.IsValid()) {
		return false;
	}
	CCharacter* pCha = pPlayer.GetMainCha();
	if (!pCha) {
		return false;
	}
	_characters.Execute(
		"UPDATE character SET bomd=? WHERE atorID=?",
		static_cast<int>(pCha->getAttr(ATTR_GD)),
		static_cast<int>(pPlayer.GetDBChaId()));
	return true;
}

bool PlayerStorage::SaveKBagDBID(CPlayer& pPlayer) {
	if (!pPlayer.IsValid()) {
		return false;
	}
	CCharacter* pCha = pPlayer.GetMainCha();
	if (!pCha) {
		return false;
	}
	_characters.Execute(
		"UPDATE character SET kitbag=? WHERE atorID=?",
		static_cast<int>(pCha->GetKitbagRecDBID()),
		static_cast<int>(pPlayer.GetDBChaId()));
	return true;
}

bool PlayerStorage::SaveKBagTmpDBID(CPlayer& pPlayer) {
	if (!pPlayer.IsValid()) {
		return false;
	}
	CCharacter* pCha = pPlayer.GetMainCha();
	if (!pCha) {
		return false;
	}
	_characters.Execute(
		"UPDATE character SET kitbag_tmp=? WHERE atorID=?",
		static_cast<int>(pCha->GetKitbagTmpRecDBID()),
		static_cast<int>(pPlayer.GetDBChaId()));
	return true;
}

bool PlayerStorage::SaveKBState(CPlayer& pPlayer) {
	if (!pPlayer.IsValid()) {
		return false;
	}
	CCharacter* pCha = pPlayer.GetMainCha();
	if (!pCha) {
		return false;
	}
	_characters.Execute(
		"UPDATE character SET kb_locked=? WHERE atorID=?",
		pCha->m_CKitbag.GetPwdLockState(),
		static_cast<int>(pPlayer.GetDBChaId()));
	return true;
}

bool PlayerStorage::SaveStoreItemID(std::uint32_t atorID, std::int32_t lStoreItemID) {
	if (atorID == 0) {
		return false;
	}
	_characters.Execute(
		"UPDATE character SET store_item=? WHERE atorID=?",
		static_cast<int>(lStoreItemID),
		static_cast<int>(atorID));
	return true;
}

bool PlayerStorage::AddMoney(std::uint32_t atorID, std::int32_t money) {
	if (atorID == 0) {
		return false;
	}
	_characters.Execute(
		"UPDATE character SET bomd=bomd+? WHERE atorID=?",
		static_cast<int>(money),
		static_cast<int>(atorID));
	return true;
}

bool PlayerStorage::AddCreditByDBID(std::uint32_t atorID, std::int32_t lCredit) {
	if (atorID == 0) {
		return false;
	}
	_characters.Execute(
		"UPDATE character SET credit=credit+? WHERE atorID=?",
		static_cast<int>(lCredit),
		static_cast<int>(atorID));
	return true;
}

bool PlayerStorage::IsChaOnline(std::uint32_t atorID, bool& bOnline) {
	if (atorID == 0) {
		return false;
	}
	auto row = _characters.FindOne("atorID = ?", static_cast<int>(atorID));
	if (!row) {
		bOnline = false;
		return false;
	}
	bOnline = (row->endeMem > 0);
	return true;
}

Long PlayerStorage::GetChaAddr(std::uint32_t atorID) {
	if (atorID == 0) {
		return 0;
	}
	auto row = _characters.FindOne("atorID = ?", static_cast<int>(atorID));
	if (!row) {
		return 0;
	}
	return static_cast<Long>(row->endeMem);
}

bool PlayerStorage::SaveMMaskDBID(CPlayer& pPlayer) {
	if (!pPlayer.IsValid()) {
		return false;
	}
	_characters.Execute(
		"UPDATE character SET map_mask=? WHERE atorID=?",
		static_cast<int>(pPlayer.GetMapMaskDBID()),
		static_cast<int>(pPlayer.GetDBChaId()));
	return true;
}

bool PlayerStorage::SaveBankDBID(CPlayer& pPlayer) {
	if (!pPlayer.IsValid()) {
		return false;
	}
	const short csIDBufLen = 200;
	char szIDBuf[csIDBufLen];
	if (!pPlayer.BankDBIDData2String(szIDBuf, csIDBufLen)) {
		return false;
	}
	_characters.Execute(
		"UPDATE character SET bank=? WHERE atorID=?",
		std::string_view(szIDBuf),
		static_cast<int>(pPlayer.GetDBChaId()));
	return true;
}

bool PlayerStorage::SaveTableVer(std::uint32_t atorID) {
	_characters.Execute(
		"UPDATE character SET version=? WHERE atorID=?",
		defCHA_TABLE_NEW_VER,
		static_cast<int>(atorID));
	return true;
}

bool PlayerStorage::SaveMissionData(CPlayer& pPlayer, std::uint32_t atorID) {
	CCharacter* pCha = pPlayer.GetMainCha();
	if (!pCha) {
		return false;
	}

	char misInfoBuf[ROLE_MAXSIZE_DBMISSION]{};
	if (!pPlayer.MisGetData(misInfoBuf, ROLE_MAXSIZE_DBMISSION - 1)) {
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00018), pCha->GetID());
		ToLogService("db", LogLevel::Error, "SaveMissionData: MisGetData failed for atorID {}", atorID);
	}

	char misRecordBuf[ROLE_MAXSIZE_DBRECORD]{};
	if (!pPlayer.MisGetRecord(misRecordBuf, ROLE_MAXSIZE_DBRECORD - 1)) {
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00018), pCha->GetID());
		ToLogService("db", LogLevel::Error, "SaveMissionData: MisGetRecord failed for atorID {}", atorID);
	}

	char misTriggerBuf[ROLE_MAXSIZE_DBTRIGGER]{};
	if (!pPlayer.MisGetTrigger(misTriggerBuf, ROLE_MAXSIZE_DBTRIGGER - 1)) {
		pCha->SystemNotice(RES_STRING(GM_GAMEDB_CPP_00019), pCha->GetID());
		ToLogService("db", LogLevel::Error, "SaveMissionData: MisGetTrigger failed for atorID {}", atorID);
	}

	_characters.Execute(
		"UPDATE character SET mission=?, misrecord=?, mistrigger=? WHERE atorID=?",
		std::string_view(misInfoBuf),
		std::string_view(misRecordBuf),
		std::string_view(misTriggerBuf),
		static_cast<int>(atorID));
	return true;
}

bool PlayerStorage::SaveDaily(CPlayer& pPlayer) {
	return true;
}


// === CTableResource (OdbcDatabase) ===

bool CTableResource::Create(std::int32_t& lDBID, std::int32_t lChaId, std::int32_t lTypeId) {
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

bool CTableResource::ReadKitbagData(CCharacter& pCCha) {
	if (pCCha.GetKitbagRecDBID() == 0) {
		std::int32_t lDBID;
		if (!Create(lDBID, pCCha.GetPlayer()->GetDBChaId(), enumRESDB_TYPE_KITBAG)) return false;
		pCCha.SetKitbagRecDBID(lDBID);
	}
	try {
		auto reader = _db.CreateCommand("SELECT atorID, type_id, content FROM resource WHERE id = ?")
						 .SetParam(1, pCCha.GetKitbagRecDBID()).ExecuteReader();
		if (reader.Read()) {
			auto dwChaId = static_cast<DWORD>(reader.GetInt(0));
			auto chType = reader.GetInt(1);
			if (dwChaId != pCCha.GetPlayer()->GetDBChaId() || chType != enumRESDB_TYPE_KITBAG) {
				ToLogService("map", "ReadKitbagData: character mismatch");
				return false;
			}
			auto content = reader.GetString(2);
			if (!pCCha.String2KitbagData(content)) {
				ToLogService("errors", LogLevel::Error, "character({}) kitbag data(resource_id {}) checksum error",
							 pCCha.GetLogName(), pCCha.GetKitbagRecDBID());
				return false;
			}
		}
		else {
			ToLogService("map", "ReadKitbagData: no data for id {}", pCCha.GetKitbagRecDBID());
			return false;
		}
		return true;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "ReadKitbagData failed: {}", e.what());
		return false;
	}
}

bool CTableResource::SaveKitbagData(CCharacter& pCCha) {
	if (!pCCha.IsValid()) return false;
	g_kitbag[0] = 0;
	if (!KitbagData2String(&pCCha.m_CKitbag, g_kitbag, defKITBAG_DATA_STRING_LEN)) {
		ToLogService("map", "character {}\tsave kitbag error!", pCCha.GetLogName());
		return false;
	}
	try {
		_db.CreateCommand("UPDATE resource SET content = ? WHERE id = ?")
		   .SetParam(1, std::string_view(g_kitbag))
		   .SetParam(2, pCCha.GetKitbagRecDBID()).ExecuteNonQuery();
		return true;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "SaveKitbagData failed: {}", e.what());
		return false;
	}
}

bool CTableResource::ReadKitbagTmpData(CCharacter& pCCha) {
	if (pCCha.GetKitbagTmpRecDBID() == 0) {
		std::int32_t lDBID;
		if (!Create(lDBID, pCCha.GetPlayer()->GetDBChaId(), enumRESDB_TYPE_KITBAGTMP)) {
			return false;
		}
		pCCha.SetKitbagTmpRecDBID(lDBID);
	}
	try {
		auto reader = _db.CreateCommand("SELECT atorID, type_id, content FROM resource WHERE id = ?")
						 .SetParam(1, pCCha.GetKitbagTmpRecDBID()).ExecuteReader();
		if (reader.Read()) {
			auto dwChaId = static_cast<DWORD>(reader.GetInt(0));
			auto chType = reader.GetInt(1);
			if (dwChaId != pCCha.GetPlayer()->GetDBChaId() || chType != enumRESDB_TYPE_KITBAGTMP) {
				ToLogService("map", "ReadKitbagTmpData: character mismatch");
				return false;
			}
			auto content = reader.GetString(2);
			if (!pCCha.String2KitbagTmpData(content)) {
				ToLogService("errors", LogLevel::Error, "character({}) temp kitbag data(resource_id {}) checksum error",
							 pCCha.GetLogName(), pCCha.GetKitbagTmpRecDBID());
				return false;
			}
		}
		else {
			ToLogService("map", "ReadKitbagTmpData: no data for id {}", pCCha.GetKitbagTmpRecDBID());
			return false;
		}
		return true;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "ReadKitbagTmpData failed: {}", e.what());
		return false;
	}
}

bool CTableResource::SaveKitbagTmpData(CCharacter& pCCha) {
	if (!pCCha.IsValid()) return false;
	g_kitbagTmp[0] = 0;
	if (!KitbagData2String(pCCha.m_pCKitbagTmp, g_kitbagTmp, defKITBAG_DATA_STRING_LEN)) {
		ToLogService("map", "character {}\tsave temp kitbag error!", pCCha.GetLogName());
		return false;
	}
	try {
		_db.CreateCommand("UPDATE resource SET content = ? WHERE id = ?")
		   .SetParam(1, std::string_view(g_kitbagTmp))
		   .SetParam(2, pCCha.GetKitbagTmpRecDBID()).ExecuteNonQuery();
		return true;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "SaveKitbagTmpData failed: {}", e.what());
		return false;
	}
}

bool CTableResource::ReadBankData(CPlayer& pCPly, std::int8_t chBankNO) {
	if (pCPly.GetCurBankNum() == 0) {
		std::int32_t lDBID;
		if (!Create(lDBID, pCPly.GetDBChaId(), enumRESDB_TYPE_BANK)) {
			return false;
		}
		pCPly.AddBankDBID(lDBID);
	}
	char chStart = (chBankNO < 0) ? 0 : chBankNO;
	char chEnd = (chBankNO < 0) ? (pCPly.GetCurBankNum() - 1) : chBankNO;
	if (chBankNO >= 0 && chBankNO >= pCPly.GetCurBankNum()) return false;

	try {
		for (char i = chStart; i <= chEnd; i++) {
			auto reader = _db.CreateCommand("SELECT atorID, type_id, content FROM resource WHERE id = ?")
							 .SetParam(1, pCPly.GetBankDBID(i)).ExecuteReader();
			if (reader.Read()) {
				auto dwChaId = static_cast<DWORD>(reader.GetInt(0));
				auto chType = reader.GetInt(1);
				if (dwChaId != pCPly.GetDBChaId() || chType != enumRESDB_TYPE_BANK) {
					ToLogService("map", "ReadBankData: character mismatch");
					return false;
				}
				auto content = reader.GetString(2);
				if (!pCPly.String2BankData(i, content)) {
					ToLogService("errors", LogLevel::Error, "player ({}) bank data(resource_id {}) checksum error",
								 pCPly.GetDBChaId(), pCPly.GetBankDBID(i));
					return false;
				}
			}
			else {
				ToLogService("map", "ReadBankData: no data for id {}", pCPly.GetBankDBID(i));
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

bool CTableResource::SaveBankData(CPlayer& pCPly, std::int8_t chBankNO) {
	if (!pCPly.IsValid()) return false;
	if (pCPly.GetCurBankNum() == 0) return true;

	char chStart = (chBankNO < 0) ? 0 : chBankNO;
	char chEnd = (chBankNO < 0) ? (pCPly.GetCurBankNum() - 1) : chBankNO;
	if (chBankNO >= 0 && chBankNO >= pCPly.GetCurBankNum()) return false;

	try {
		for (char i = chStart; i <= chEnd; i++) {
			if (!pCPly.BankWillSave(i)) continue;
			pCPly.SetBankSaveFlag(i, false);
			g_kitbag[0] = 0;
			if (!KitbagData2String(pCPly.GetBank(i), g_kitbag, defKITBAG_DATA_STRING_LEN)) {
				ToLogService("map", "bank {}\tsave error!", pCPly.GetBankDBID(i));
				return false;
			}
			_db.CreateCommand("UPDATE resource SET content = ? WHERE id = ?")
			   .SetParam(1, std::string_view(g_kitbag))
			   .SetParam(2, pCPly.GetBankDBID(i)).ExecuteNonQuery();
		}
		return true;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "SaveBankData failed: {}", e.what());
		return false;
	}
}


// === CTableMapMask (OdbcDatabase) ===

bool CTableMapMask::GetColNameByMapName(const std::string& szMapName, std::string& colName) {
	if (szMapName.empty()) return false;
	if (szMapName == "garner") {
		colName = "content1";
		return true;
	}
	if (szMapName == "magicsea") {
		colName = "content2";
		return true;
	}
	if (szMapName == "darkblue") {
		colName = "content3";
		return true;
	}
	if (szMapName == "winterland") {
		colName = "content4";
		return true;
	}
	return false;
}

bool CTableMapMask::Create(std::int32_t& lDBID, std::int32_t lChaId) {
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

bool CTableMapMask::ReadData(CPlayer& pCPly) {
	if (!pCPly.IsValid()) {
		ToLogService("map", "Load map_mask error: player is null");
		return false;
	}
	if (pCPly.GetMapMaskDBID() == 0) {
		std::int32_t lDBID;
		if (!Create(lDBID, pCPly.GetDBChaId())) {
			return false;
		}
		pCPly.SetMapMaskDBID(lDBID);
	}

	std::string colName;
	if (!GetColNameByMapName(pCPly.GetMaskMapName(), colName)) {
		ToLogService("map", "map_mask: unknown map name");
		return false;
	}

	try {
		// colName из фиксированного набора (content1..4), безопасно вставлять в SQL
		auto sql = std::format("SELECT atorID, {} FROM map_mask WHERE id = ?", colName);
		auto reader = _db.CreateCommand(sql).SetParam(1, pCPly.GetMapMaskDBID()).ExecuteReader();
		if (reader.Read()) {
			auto dbChaId = static_cast<DWORD>(reader.GetInt(0));
			if (dbChaId != pCPly.GetDBChaId()) {
				ToLogService("map", "map_mask: character mismatch");
				return false;
			}
			pCPly.SetMapMaskBase64(reader.GetString(1).c_str());
		}
		else {
			ToLogService("map", "map_mask: no data for id {}", pCPly.GetMapMaskDBID());
			return false;
		}
		return true;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "CTableMapMask::ReadData failed: {}", e.what());
		return false;
	}
}

bool CTableMapMask::SaveData(CPlayer& pCPly, bool bDirect) {
	if (!pCPly.IsValid()) return false;

	std::string colName;
	if (!GetColNameByMapName(pCPly.GetMaskMapName(), colName)) {
		ToLogService("map", "map_mask: unknown map name");
		return false;
	}

	// colName из фиксированного набора (content1..4), безопасно
	auto sql = std::format("UPDATE map_mask SET {} = ? WHERE id = ?", colName);

	if (!bDirect) {
		// Отложенное сохранение — собираем SQL в очередь
		// Для отложенного нужно подставить значения сразу (очередь хранит готовые SQL)
		auto fullSql = std::format("UPDATE map_mask SET {} = '{}' WHERE id = {}",
								   colName, pCPly.GetMapMaskBase64(), pCPly.GetMapMaskDBID());
		_saveQueue.push_back(std::move(fullSql));
	}
	else {
		try {
			_db.CreateCommand(sql)
			   .SetParam(1, std::string_view(pCPly.GetMapMaskBase64()))
			   .SetParam(2, pCPly.GetMapMaskDBID()).ExecuteNonQuery();
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

bool CTableBoat::Create(std::uint32_t& dwBoatID, const BOAT_DATA& Data) {
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
		dwBoatID = static_cast<std::uint32_t>(std::stol(idStr));
		return true;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "CTableBoat::Create failed: {}", e.what());
		return false;
	}
}

bool CTableBoat::GetBoat(CCharacter& Boat) {
	DWORD dwBoatID = (DWORD)Boat.getAttr(ATTR_BOAT_DBID);
	try {
		auto reader = _db.CreateCommand(
							 "SELECT boat_name, boat_boatid, boat_berth, boat_header, boat_body, "
							 "boat_engine, boat_cannon, boat_equipment, boat_diecount, boat_isdead, "
							 "boat_ownerid, boat_isdeleted, cur_endure, mx_endure, cur_supply, mx_supply, "
							 "skill_state, map, map_x, map_y, angle, degree, exp FROM boat WHERE boat_id = ?")
						 .SetParam(1, dwBoatID).ExecuteReader();

		if (!reader.Read()) return false;

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
			return false;
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

		if (!ReadCabin(Boat)) return false;

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
		return true;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "GetBoat failed: {}", e.what());
		return false;
	}
}

bool CTableBoat::SaveBoatTempData(std::uint32_t dwBoatID, std::uint32_t dwOwnerID, std::uint8_t byIsDeleted) {
	try {
		_db.CreateCommand("UPDATE boat SET boat_ownerid = ?, boat_isdeleted = ? WHERE boat_id = ?")
		   .SetParam(1, dwOwnerID).SetParam(2, static_cast<int>(byIsDeleted)).SetParam(3, dwBoatID).ExecuteNonQuery();
		return true;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "SaveBoatTempData(id) failed: {}", e.what());
		return false;
	}
}

bool CTableBoat::SaveBoatDelTag(std::uint32_t dwBoatID, std::uint8_t byIsDeleted) {
	try {
		_db.CreateCommand("UPDATE boat SET boat_isdeleted = ? WHERE boat_id = ?")
		   .SetParam(1, static_cast<int>(byIsDeleted)).SetParam(2, dwBoatID).ExecuteNonQuery();
		return true;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "SaveBoatDelTag failed: {}", e.what());
		return false;
	}
}

bool CTableBoat::SaveBoatTempData(CCharacter& Boat, std::uint8_t byIsDeleted) {
	try {
		DWORD dwBoatID = (DWORD)Boat.getAttr(ATTR_BOAT_DBID);
		_db.CreateCommand(
			   "UPDATE boat SET boat_diecount = ?, boat_isdead = ?, boat_ownerid = ?, boat_isdeleted = ? WHERE boat_id = ?")
		   .SetParam(1, static_cast<int>(Boat.getAttr(ATTR_BOAT_DIECOUNT)))
		   .SetParam(2, static_cast<int>(Boat.getAttr(ATTR_BOAT_ISDEAD)))
		   .SetParam(3, Boat.GetPlayer()->GetDBChaId())
		   .SetParam(4, static_cast<int>(byIsDeleted))
		   .SetParam(5, dwBoatID).ExecuteNonQuery();
		return true;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "SaveBoatTempData(cha) failed: {}", e.what());
		return false;
	}
}

bool CTableBoat::SaveBoat(CCharacter& Boat, std::int8_t chSaveType) {
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
		return true;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "SaveBoat failed: {}", e.what());
		return false;
	}
}

bool CTableBoat::SaveAllData(CPlayer& pPlayer, std::int8_t chSaveType) {
	for (int i = 0; i < MAX_CHAR_BOAT; i++) {
		CCharacter* pBoat = pPlayer.GetBoat(static_cast<BYTE>(i));
		if (pBoat && (DWORD)pBoat->getAttr(ATTR_BOAT_DBID) != 0) {
			if (!SaveBoat(*pBoat, chSaveType)) return false;
		}
	}
	return true;
}

bool CTableBoat::SaveCabin(CCharacter& Boat, std::int8_t chSaveType) {
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

bool CTableBoat::SaveAllCabin(CPlayer& pPlayer, std::int8_t chSaveType) {
	for (int i = 0; i < MAX_CHAR_BOAT; i++) {
		CCharacter* pBoat = pPlayer.GetBoat(static_cast<BYTE>(i));
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

bool CGameDB::Init() {
	m_bInitOK = false;

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
		return false;
	}

	if (!_tab_cha.Init()) {
		return false;
	}

	m_bInitOK = true;
	return true;
}

bool CGameDB::ReadPlayer(CPlayer& pPlayer, std::uint32_t atorID) {
	if (!_tab_cha.ReadAllData(pPlayer, atorID))
		return false;

	long lKbDBID = pPlayer.GetMainCha()->GetKitbagRecDBID();
	long lkbTmpDBID = pPlayer.GetMainCha()->GetKitbagTmpRecDBID(); //ID
	long lMMaskDBID = pPlayer.GetMapMaskDBID();
	long lBankNum = pPlayer.GetCurBankNum();
	if (!_tab_res.ReadKitbagData(*pPlayer.GetMainCha()))
		return false;
	if (lKbDBID == 0)
		if (!SavePlayerKBagDBID(pPlayer))
			return false;

	if (!_tab_res.ReadKitbagTmpData(*pPlayer.GetMainCha()))
		return false;
	if (lkbTmpDBID == 0)
		if (!SavePlayerKBagTmpDBID(pPlayer))
			return false;
	pPlayer.GetMainCha()->LogAssets(enumLASSETS_INIT);

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

	// Чтение данных аккаунта через типизированную таблицу
	try {
		auto accRow = _accounts.FindOne("ato_id = ?", static_cast<int>(pPlayer.GetDBActId()));
		if (accRow) {
			pPlayer.SetGMLev(accRow->jmes);
			pPlayer.SetActName(accRow->ato_nome.c_str());
			pPlayer.SetIMP(accRow->IMP);
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
	if (pPlayer.m_lGuildID > 0) {
		_tab_gld.GetGuildInfo(*pPlayer.GetMainCha(), pPlayer.m_lGuildID);
		//long	lType = _tab_gld.GetTypeByID(pPlayer.GetMainCha()->getAttr(ATTR_GUILD));
		//if (lType >= 0)
		//	pPlayer.GetMainCha()->setAttr(ATTR_GUILD_TYPE, lType, 1);
	}
	//LG("enter_map", ".\n");
	ToLogService("map", "Load the character whole data succeed.");

	//
	CKitbag* pCKb;
	CCharacter* pCMainC = pPlayer.GetMainCha();
	short sItemNum = pCMainC->m_CKitbag.GetUseGridNum();
	g_kitbag[0] = '\0';
	std::snprintf(g_kitbag, defKITBAG_DATA_STRING_LEN, RES_STRING(GM_GAMEDB_CPP_00021), pCMainC->getAttr(ATTR_GD), sItemNum);
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
		{ size_t _n = strlen(g_kitbag); std::snprintf(g_kitbag + _n, defKITBAG_DATA_STRING_LEN > _n ? defKITBAG_DATA_STRING_LEN - _n : 0, "%s[%d],%d;", pCItem->szName.c_str(), pGridCont->sID, pGridCont->sNum); }
	}
	ToLogService("trade", "[CHA_ENTER] {} : {}", pCMainC->GetName(), g_kitbag);

	short sItemTmpNum = pCMainC->m_pCKitbagTmp->GetUseGridNum();
	g_kitbagTmp[0] = '\0';
	std::snprintf(g_kitbagTmp, defKITBAG_DATA_STRING_LEN, RES_STRING(GM_GAMEDB_CPP_00022), sItemTmpNum);
	pCKb = pCMainC->m_pCKitbagTmp;
	for (short i = 0; i < sItemTmpNum; i++) {
		pGridCont = pCKb->GetGridContByNum(i);
		if (!pGridCont)
			continue;
		pCItem = GetItemRecordInfo(pGridCont->sID);
		if (!pCItem)
			continue;
		{ size_t _n = strlen(g_kitbagTmp); std::snprintf(g_kitbagTmp + _n, defKITBAG_DATA_STRING_LEN > _n ? defKITBAG_DATA_STRING_LEN - _n : 0, "%s[%d],%d;", pCItem->szName.c_str(), pGridCont->sID,
				pGridCont->sNum); }
	}
	ToLogService("trade", "[CHA_ENTER] {} : {}", pCMainC->GetName(), g_kitbagTmp);

	char chStart = 0, chEnd = pPlayer.GetCurBankNum() - 1;
	for (char i = chStart; i <= chEnd; i++) {
		std::snprintf(g_kitbag, defKITBAG_DATA_STRING_LEN, RES_STRING(GM_GAMEDB_CPP_00023), i + 1);
		pCKb = pPlayer.GetBank(i);
		sItemNum = pCKb->GetUseGridNum();
		{ size_t _n = strlen(g_kitbag); std::snprintf(g_kitbag + _n, defKITBAG_DATA_STRING_LEN > _n ? defKITBAG_DATA_STRING_LEN - _n : 0, "[%d]%d@;", i + 1, sItemNum); }
		for (short i = 0; i < sItemNum; i++) {
			pGridCont = pCKb->GetGridContByNum(i);
			if (!pGridCont)
				continue;
			pCItem = GetItemRecordInfo(pGridCont->sID);
			if (!pCItem)
				continue;
			{ size_t _n = strlen(g_kitbag); std::snprintf(g_kitbag + _n, defKITBAG_DATA_STRING_LEN > _n ? defKITBAG_DATA_STRING_LEN - _n : 0, "%s[%d],%d;", pCItem->szName.c_str(), pGridCont->sID, pGridCont->sNum); }
		}
		ToLogService("trade", "[CHA_ENTER] {} : {}", pCMainC->GetName(), g_kitbag);
	}

	g_equip[0] = '\0';
	std::snprintf(g_equip, defKITBAG_DATA_STRING_LEN, RES_STRING(GM_GAMEDB_CPP_00024), enumEQUIP_NUM);
	for (short i = 0; i < enumEQUIP_NUM; i++) {
		pGridCont = &pCMainC->m_SChaPart.SLink[i];
		if (!pGridCont || pGridCont->sID <= 0)
			continue;
		pCItem = GetItemRecordInfo(pGridCont->sID);
		if (!pCItem)
			continue;
		{ size_t _n = strlen(g_equip); std::snprintf(g_equip + _n, defKITBAG_DATA_STRING_LEN > _n ? defKITBAG_DATA_STRING_LEN - _n : 0, "%s[%d],%d;", pCItem->szName.c_str(), pGridCont->sID, pGridCont->sNum); }
	}
	ToLogService("trade", "[CHA_EQUIP] {} : {}", pCMainC->GetName(), g_equip);

	//
	return true;
}

bool CGameDB::SavePlayer(CPlayer& pPlayer, std::int8_t chSaveType, bool bForceWithPos) {
	if (!pPlayer.GetMainCha())
		return false;

	if (pPlayer.GetMainCha()->GetPlayer() != &pPlayer) {
		//LG("", "Player %p[dbid %u] %sPlayer %p\n",
		ToLogService("errors", LogLevel::Error,
					 "save Player address {}[dbid {}], the character main player {}, the character's Player address {}",
					 static_cast<void*>(&pPlayer), pPlayer.GetDBChaId(), pPlayer.GetMainCha()->GetLogName(),
					 static_cast<void*>(pPlayer.GetMainCha()->GetPlayer()));
		//pPlayer.SystemNotice("");
		pPlayer.SystemNotice(RES_STRING(GM_GAMEDB_CPP_00025));
		return false;
	}

	bool bSaveMainCha = false, bSaveBoat = false, bSaveKitBag = false, bSaveMMask = false, bSaveBank = false;
	bool bSaveKitBagTmp = false;
	bool bSaveKBState = false;
	BeginTran();
	try {
		DWORD dwStartTick = GetTickCount();

		bSaveMainCha = _tab_cha.SaveAllData(pPlayer, chSaveType, bForceWithPos); //
		DWORD dwSaveMainTick = GetTickCount();
		bSaveKitBag = _tab_res.SaveKitbagData(*pPlayer.GetMainCha());
		//
		bSaveKitBagTmp = _tab_res.SaveKitbagTmpData(*pPlayer.GetMainCha());
		//
		//bSaveKBState = _tab_cha.SaveKBState(pPlayer);
		DWORD dwSaveKbTick = GetTickCount();
		bSaveBank = _tab_res.SaveBankData(pPlayer);
		DWORD dwSaveBankTick = GetTickCount();
		if ((chSaveType != enumSAVE_TYPE_TIMER) && (g_Config.m_chMapMask > 0)) {
			if (pPlayer.IsMapMaskChange()) {
				bSaveMMask = _tab_mmask.SaveData(pPlayer);
				pPlayer.ResetMapMaskChange();
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
			pPlayer.GetDBChaId(), pPlayer.GetMainCha()->GetLogName());
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
		CCharacter* pCMainC = pPlayer.GetMainCha();
		short sItemNum = pCMainC->m_CKitbag.GetUseGridNum();
		g_kitbag[0] = '\0';
		std::snprintf(g_kitbag, defKITBAG_DATA_STRING_LEN, RES_STRING(GM_GAMEDB_CPP_00026), pCMainC->getAttr(ATTR_GD), sItemNum);
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
			{ size_t _n = strlen(g_kitbag); std::snprintf(g_kitbag + _n, defKITBAG_DATA_STRING_LEN > _n ? defKITBAG_DATA_STRING_LEN - _n : 0, "%s[%d],%d;", pCItem->szName.c_str(), pGridCont->sID, pGridCont->sNum); }
		}
		ToLogService("trade", "[CHA_OUT] {} : {}", pCMainC->GetName(), g_kitbag);

		short sItemTmpNum = pCMainC->m_pCKitbagTmp->GetUseGridNum();
		g_kitbagTmp[0] = '\0';
		std::snprintf(g_kitbagTmp, defKITBAG_DATA_STRING_LEN, RES_STRING(GM_GAMEDB_CPP_00022), sItemTmpNum);
		pCKb = pCMainC->m_pCKitbagTmp;
		for (short i = 0; i < sItemTmpNum; i++) {
			pGridCont = pCKb->GetGridContByNum(i);
			if (!pGridCont)
				continue;
			pCItem = GetItemRecordInfo(pGridCont->sID);
			if (!pCItem)
				continue;
			{ size_t _n = strlen(g_kitbagTmp); std::snprintf(g_kitbagTmp + _n, defKITBAG_DATA_STRING_LEN > _n ? defKITBAG_DATA_STRING_LEN - _n : 0, "%s[%d],%d;", pCItem->szName.c_str(), pGridCont->sID,
					pGridCont->sNum); }
		}
		ToLogService("trade", "[CHA_OUT] {} : {}", pCMainC->GetName(), g_kitbagTmp);

		g_equip[0] = '\0';
		std::snprintf(g_equip, defKITBAG_DATA_STRING_LEN, RES_STRING(GM_GAMEDB_CPP_00024), enumEQUIP_NUM);
		for (short i = 0; i < enumEQUIP_NUM; i++) {
			pGridCont = &pCMainC->m_SChaPart.SLink[i];
			if (!pGridCont || pGridCont->sID <= 0)
				continue;
			pCItem = GetItemRecordInfo(pGridCont->sID);
			if (!pCItem)
				continue;
			{ size_t _n = strlen(g_equip); std::snprintf(g_equip + _n, defKITBAG_DATA_STRING_LEN > _n ? defKITBAG_DATA_STRING_LEN - _n : 0, "%s[%d],%d;", pCItem->szName.c_str(), pGridCont->sID, pGridCont->sNum); }
		}
		ToLogService("trade", "[CHA_EQUIP] {} : {}", pCMainC->GetName(), g_equip);

		char chStart = 0, chEnd = pPlayer.GetCurBankNum() - 1;
		std::snprintf(g_kitbag, defKITBAG_DATA_STRING_LEN, RES_STRING(GM_GAMEDB_CPP_00023), pPlayer.GetCurBankNum());
		for (char i = chStart; i <= chEnd; i++) {
			pCKb = pPlayer.GetBank(i);
			sItemNum = pCKb->GetUseGridNum();
			{ size_t _n = strlen(g_kitbag); std::snprintf(g_kitbag + _n, defKITBAG_DATA_STRING_LEN > _n ? defKITBAG_DATA_STRING_LEN - _n : 0, "[%d]%d@;", i + 1, sItemNum); }
			for (short i = 0; i < sItemNum; i++) {
				pGridCont = pCKb->GetGridContByNum(i);
				if (!pGridCont)
					continue;
				pCItem = GetItemRecordInfo(pGridCont->sID);
				if (!pCItem)
					continue;
				{ size_t _n = strlen(g_kitbag); std::snprintf(g_kitbag + _n, defKITBAG_DATA_STRING_LEN > _n ? defKITBAG_DATA_STRING_LEN - _n : 0, "%s[%d],%d;", pCItem->szName.c_str(), pGridCont->sID,
						pGridCont->sNum); }
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

	{
		auto _s = std::format("insert gamelog (action, c1, c2, c3, c4, content) "
			"values('{}', '{}', '{}', '{}', '{}', '{}')", type, c1, c2, c3, c4, p);
		std::strncpy(szSQL, _s.c_str(), sizeof(szSQL) - 1);
		szSQL[sizeof(szSQL) - 1] = 0;
	}

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
// CTableGuild — реализации методов (OdbcDatabase API, параметризованные запросы)
// ============================================================================

//===============CTableGuild Begin===========================================
std::int32_t CTableGuild::Create(CCharacter& pCha, const std::string& guildname, const std::string& passwd) {
	std::int32_t l_ret_guild_id = 0;

	while (true) {
		// Ищем свободный guild_id (leader_id == 0 означает незанятый слот)
		try {
			auto reader = _db.CreateCommand(
				"SELECT ISNULL(MIN(guild_id), 0) FROM guild WHERE guild_id > 0 AND leader_id = 0")
				.ExecuteReader();
			if (reader.Read()) {
				l_ret_guild_id = reader.GetInt(0);
			}
		} catch (const OdbcException&) {
			pCha.SystemNotice(RES_STRING(GM_GAMEDB_CPP_00027));
			ToLogService("common", "found consortia system occur SQL operator error.");
			return 0;
		}

		if (!l_ret_guild_id) {
			pCha.SystemNotice(RES_STRING(GM_GAMEDB_CPP_00030));
			return 0;
		}

		// Атомарно занимаем слот (leader_id = 0 → наш ID)
		try {
			int affected = _db.CreateCommand(
				"UPDATE guild SET leader_id = ?, passwd = ?, guild_name = ?, exp = 0, "
				"member_total = 1, try_total = 0 "
				"WHERE leader_id = 0 AND guild_id = ?")
				.SetParam(1, static_cast<int>(pCha.GetID()))
				.SetParam(2, passwd)
				.SetParam(3, guildname)
				.SetParam(4, static_cast<int>(l_ret_guild_id))
				.ExecuteNonQuery();
			if (affected != 1) {
				continue; // Кто-то занял слот раньше — пробуем снова
			}
		} catch (const OdbcException&) {
			pCha.SystemNotice(RES_STRING(GM_GAMEDB_CPP_00031));
			return 0;
		}

		break;
	}

	// Привязываем персонажа к гильдии
	_db.CreateCommand(
		"UPDATE character SET guild_id = ?, guild_stat = 0, guild_permission = ? WHERE atorID = ?")
		.SetParam(1, static_cast<int>(l_ret_guild_id))
		.SetParam(2, static_cast<int>(emGldPermMax))
		.SetParam(3, static_cast<int>(pCha.GetID()))
		.ExecuteNonQuery();

	// Уведомление GameServerGroup
	auto l_wpk = net::msg::serialize(net::msg::GmGuildCreateMessage{
		static_cast<int64_t>(l_ret_guild_id), guildname,
		g_GetJobName(uShort(pCha.getAttr(ATTR_JOB))),
		static_cast<int64_t>(uShort(pCha.getAttr(ATTR_LV)))
	});
	pCha.ReflectINFof(&pCha, l_wpk);

	return l_ret_guild_id;
}

bool CTableGuild::ListAll(CCharacter& pCha, std::int8_t disband_days) {
	if (disband_days < 1) {
		return false;
	}

	try {
		auto reader = _db.CreateCommand(
			"SELECT gld.guild_id, gld.guild_name, gld.motto, gld.leader_id, "
			"cha.atorNome AS leader_name, gld.exp, gld.member_total "
			"FROM guild AS gld, character AS cha "
			"WHERE gld.leader_id = cha.atorID")
			.ExecuteReader();

		// Отправка клиенту страницами по 20 записей
		net::msg::McListGuildMessage page;
		while (reader.Read()) {
			page.entries.push_back({
				static_cast<int64_t>(reader.GetInt(0)),      // guild_id
				reader.GetString(1),                          // guild_name
				reader.GetString(2),                          // motto
				reader.GetString(4),                          // leader_name
				static_cast<int64_t>(reader.GetInt(6)),      // member_total
				reader.GetInt64(5)                            // exp
			});
			if (page.entries.size() == 20) {
				auto l_wpk = net::msg::serialize(page);
				pCha.ReflectINFof(&pCha, l_wpk);
				page.entries.clear();
			}
		}
		// Последняя (неполная) страница
		auto l_wpk = net::msg::serialize(page);
		pCha.ReflectINFof(&pCha, l_wpk);
		return true;
	} catch (const OdbcException& e) {
		ToLogService("common", "found consortia process ODBC interfance transfer error: {}", e.what());
		return false;
	} catch (...) {
		ToLogService("common", "Unknown Exception raised when list all guilds");
		return false;
	}
}

void CTableGuild::TryFor(CCharacter& pCha, std::uint32_t guildid) {
	if (pCha.HasGuild()) {
		pCha.SystemNotice(RES_STRING(GM_GAMEDB_CPP_00032), pCha.GetGuildName());
		return;
	} else if (guildid == pCha.GetGuildID()) {
		pCha.SystemNotice(RES_STRING(GM_GAMEDB_CPP_00033), pCha.GetGuildName());
		return;
	}

	// Проверяем существование гильдии (leader_id > 0)
	{
		auto reader = _db.CreateCommand(
			"SELECT guild_id FROM guild WHERE leader_id > 0 AND guild_id = ?")
			.SetParam(1, static_cast<int>(guildid))
			.ExecuteReader();
		if (!reader.Read()) {
			pCha.SystemNotice(RES_STRING(GM_GAMEDB_CPP_00035));
			return;
		}
	}

	// Проверяем текущую гильдию персонажа (если есть — в другой гильдии)
	std::string curGuildName;
	int curGuildId = 0;
	int curGuildStat = 0;
	{
		auto reader = _db.CreateCommand(
			"SELECT c.guild_id, c.guild_stat, g.guild_name "
			"FROM character c, guild g "
			"WHERE c.guild_id = g.guild_id AND c.atorID = ? AND g.guild_id <> ?")
			.SetParam(1, static_cast<int>(pCha.GetID()))
			.SetParam(2, static_cast<int>(guildid))
			.ExecuteReader();
		if (reader.Read()) {
			curGuildId = reader.GetInt(0);
			curGuildStat = reader.GetInt(1);
			curGuildName = reader.GetString(2);
		}
	}

	// Получаем имя целевой гильдии
	std::string targetGuildName;
	{
		auto reader = _db.CreateCommand("SELECT guild_name FROM guild WHERE guild_id = ?")
			.SetParam(1, static_cast<int>(guildid))
			.ExecuteReader();
		if (!reader.Read()) {
			ToLogService("common", "TryFor: character {} apply consortia ID[0x{:X}]is inexistence!",
						 pCha.GetName(), guildid);
			pCha.SystemNotice(RES_STRING(GM_GAMEDB_CPP_00036));
			return;
		}
		targetGuildName = reader.GetString(0);
	}

	// Сохраняем имя гильдии для подтверждения
	strncpy(pCha.GetPlayer()->m_szTempGuildName, targetGuildName.c_str(), defGUILD_NAME_LEN - 1);

	if (curGuildId) {
		if (curGuildStat == emGldMembStatNormal) {
			pCha.SystemNotice(RES_STRING(GM_GAMEDB_CPP_00037), curGuildName.c_str());
			return;
		} else if (curGuildStat == emGldMembStatTry && !(pCha.GetPlayer()->m_GuildState & emGuildReplaceOldTry)) {
			pCha.GetPlayer()->m_GuildState |= emGuildReplaceOldTry;
			pCha.GetPlayer()->m_lTempGuildID = guildid;
			auto l_wpk = net::msg::serialize(net::msg::McGuildTryForCfmMessage{curGuildName.c_str()});
			pCha.ReflectINFof(&pCha, l_wpk);
			return;
		}
	} else {
		TryForConfirm(pCha, guildid);
	}
}

void CTableGuild::TryForConfirm(CCharacter& pCha, std::uint32_t guildid) {
	if (pCha.HasGuild()) {
		pCha.SystemNotice(RES_STRING(GM_GAMEDB_CPP_00038), pCha.GetGuildName());
		return;
	}

	DWORD dwOldGuildID = pCha.GetGuildID();

	try {
		auto txn = _db.BeginTransaction();

		// Устанавливаем guild_id для персонажа, только если гильдия принимает заявки
		int affected = _db.CreateCommand(
			"UPDATE character SET guild_id = ?, guild_stat = 1, guild_permission = 0 "
			"WHERE atorID = ? AND ? IN ("
			"  SELECT guild_id FROM guild "
			"  WHERE leader_id > 0 AND guild_id = ? AND try_total < ? AND member_total < ?)")
			.SetParam(1, static_cast<int>(guildid))
			.SetParam(2, static_cast<int>(pCha.GetID()))
			.SetParam(3, static_cast<int>(guildid))
			.SetParam(4, static_cast<int>(guildid))
			.SetParam(5, static_cast<int>(emMaxTryMemberNum))
			.SetParam(6, static_cast<int>(emMaxMemberNum))
			.ExecuteNonQuery();
		if (affected == 0) {
			pCha.SystemNotice(RES_STRING(GM_GAMEDB_CPP_00040));
			return; // txn auto-rollback
		}

		affected = _db.CreateCommand(
			"UPDATE guild SET try_total = try_total + 1 WHERE guild_id = ?")
			.SetParam(1, static_cast<int>(guildid))
			.ExecuteNonQuery();
		if (affected == 0) {
			pCha.SystemNotice(RES_STRING(GM_GAMEDB_CPP_00040));
			return;
		}

		// Если была заявка в другую гильдию — отменяем
		if (dwOldGuildID && (pCha.GetPlayer()->m_GuildState & emGuildReplaceOldTry)) {
			affected = _db.CreateCommand(
				"UPDATE guild SET try_total = try_total - 1 WHERE guild_id = ? AND try_total > 0")
				.SetParam(1, static_cast<int>(dwOldGuildID))
				.ExecuteNonQuery();
			if (affected == 0) {
				pCha.SystemNotice(RES_STRING(GM_GAMEDB_CPP_00041));
				return;
			}
		}

		txn.Commit();
	} catch (const OdbcException&) {
		pCha.SystemNotice(RES_STRING(GM_GAMEDB_CPP_00040));
		return;
	}

	pCha.SetGuildID(guildid);
	pCha.SetGuildState(emGldMembStatTry);
	pCha.SetGuildName(pCha.GetPlayer()->m_szTempGuildName);
	pCha.SystemNotice(RES_STRING(GM_GAMEDB_CPP_00042), pCha.GetGuildName());
}


bool CTableGuild::GetGuildBank(std::uint32_t guildid, CKitbag* bag) {
	auto reader = _db.CreateCommand("SELECT bank FROM guild WHERE guild_id = ?")
		.SetParam(1, static_cast<int>(guildid))
		.ExecuteReader();
	if (reader.Read()) {
		auto bankStr = reader.GetString(0);
		if (bankStr.empty()) {
			bag->SetCapacity(48);
			return true;
		}
		if (String2KitbagData(bag, bankStr)) {
			return true;
		}
	}
	return false;
}

std::int32_t CTableGuild::GetGuildLeaderID(std::uint32_t guildid) {
	auto reader = _db.CreateCommand("SELECT leader_id FROM guild WHERE guild_id = ?")
		.SetParam(1, static_cast<int>(guildid))
		.ExecuteReader();
	if (reader.Read()) {
		return reader.GetInt(0);
	}
	return 0;
}

bool CTableGuild::UpdateGuildBank(std::uint32_t guildid, CKitbag* bag) {
	char bagStr[defKITBAG_DATA_STRING_LEN];
	if (KitbagData2String(bag, bagStr, defKITBAG_DATA_STRING_LEN)) {
		int affected = _db.CreateCommand("UPDATE guild SET bank = ? WHERE guild_id = ?")
			.SetParam(1, std::string_view(bagStr))
			.SetParam(2, static_cast<int>(guildid))
			.ExecuteNonQuery();
		return affected > 0;
	}
	return false;
}

bool CTableGuild::UpdateGuildBankGold(std::int32_t guildID, std::int32_t money) {
	int affected = _db.CreateCommand("UPDATE guild SET gold = gold + ? WHERE guild_id = ?")
		.SetParam(1, money)
		.SetParam(2, guildID)
		.ExecuteNonQuery();
	return affected > 0;
}

std::int64_t CTableGuild::GetGuildBankGold(std::uint32_t guildid) {
	auto reader = _db.CreateCommand("SELECT gold FROM guild WHERE guild_id = ?")
		.SetParam(1, static_cast<int>(guildid))
		.ExecuteReader();
	if (reader.Read()) {
		return reader.GetInt64(0);
	}
	return 0;
}

std::vector<CTableGuild::BankLog> CTableGuild::GetGuildLog(std::uint32_t guildid) {
	std::vector<CTableGuild::BankLog> logs;

	auto reader = _db.CreateCommand("SELECT banklog FROM guild WHERE guild_id = ?")
		.SetParam(1, static_cast<int>(guildid))
		.ExecuteReader();
	if (reader.Read()) {
		auto banklogStr = reader.GetString(0);
		if (!banklogStr.empty()) {
			string logList[1024];
			int n = Util_ResolveTextLine(banklogStr.c_str(), logList, 1024, '-', ';');
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
	}

	if (logs.size() == 200) {
		logs.erase(logs.begin()); // Лимит 200 записей — удаляем самую старую
	}

	return logs;
}

bool CTableGuild::SetGuildLog(std::vector<BankLog> log, std::uint32_t guild_id) {
	std::string data;
	data.reserve(log.size() * 40);
	for (const auto& entry : log) {
		if (entry.userID == 0) {
			continue;
		}
		data += std::format("{}-{}-{}-{}-{};",
			entry.type, entry.time, entry.parameter, entry.quantity, entry.userID);
	}

	int affected = _db.CreateCommand("UPDATE guild SET banklog = ? WHERE guild_id = ?")
		.SetParam(1, data)
		.SetParam(2, static_cast<int>(guild_id))
		.ExecuteNonQuery();
	return affected > 0;
}


bool CTableGuild::GetGuildInfo(CCharacter& pCha, std::uint32_t guildid) {
	auto reader = _db.CreateCommand("SELECT guild_name, motto FROM guild WHERE guild_id = ?")
		.SetParam(1, static_cast<int>(guildid))
		.ExecuteReader();
	if (reader.Read()) {
		pCha.SetGuildName(reader.GetString(0).c_str());
		pCha.SetGuildMotto(reader.GetString(1).c_str());
		return true;
	}
	return false;
}

bool CTableGuild::ListTryPlayer(CCharacter& pCha, char disband_days) {
	if (!pCha.HasGuild()) {
		return false;
	}

	// Получаем информацию о гильдии и лидере
	net::msg::McGuildListTryPlayerMessage tryMsg;
	{
		auto reader = _db.CreateCommand(
			"SELECT g.guild_id, g.guild_name, g.motto, c.atorNome, g.member_total, g.exp, g.level "
			"FROM character c, guild g "
			"WHERE g.leader_id = c.atorID AND g.guild_id = ?")
			.SetParam(1, static_cast<int>(pCha.GetGuildID()))
			.ExecuteReader();
		if (!reader.Read()) {
			return false;
		}
		tryMsg.guildId = reader.GetInt(0);
		tryMsg.guildName = reader.GetString(1);
		tryMsg.motto = reader.GetString(2);
		tryMsg.leaderName = reader.GetString(3);
		tryMsg.memberTotal = reader.GetInt(4);
		tryMsg.maxMembers = g_Config.m_sGuildNum;
		tryMsg.exp = reader.GetInt64(5);
		tryMsg.reserved = 0;
		tryMsg.level = reader.GetInt(6);
	}

	// Получаем список игроков-кандидатов (guild_stat = 1 = TryFor)
	try {
		auto reader = _db.CreateCommand(
			"SELECT c.atorID, c.atorNome, c.job, c.degree "
			"FROM character c "
			"WHERE c.guild_stat = 1 AND c.guild_id = ? AND c.delflag = 0")
			.SetParam(1, static_cast<int>(pCha.GetGuildID()))
			.ExecuteReader();
		while (reader.Read()) {
			tryMsg.players.push_back({
				static_cast<int64_t>(reader.GetInt(0)),
				reader.GetString(1),
				reader.GetString(2),
				static_cast<int64_t>(reader.GetInt(3))
			});
		}

		auto l_wpk = net::msg::serialize(tryMsg);
		pCha.ReflectINFof(&pCha, l_wpk);
		return true;
	} catch (const OdbcException& e) {
		ToLogService("common", "consult apply consortia process memberODBC interface transfer error: {}", e.what());
		return false;
	} catch (...) {
		ToLogService("common", "Unknown Exception raised when list all guilds");
		return false;
	}
}

bool CTableGuild::Approve(CCharacter& pCha, std::uint32_t chaid) {
	if (!pCha.HasGuild()) {
		return false;
	}

	// Проверяем права на приём в гильдию
	{
		auto reader = _db.CreateCommand(
			"SELECT c.atorID FROM character c "
			"WHERE c.atorID = ? AND c.guild_id = ? AND c.guild_permission & ? = ?")
			.SetParam(1, static_cast<int>(pCha.GetID()))
			.SetParam(2, static_cast<int>(pCha.GetGuildID()))
			.SetParam(3, static_cast<int>(emGldPermRecruit))
			.SetParam(4, static_cast<int>(emGldPermRecruit))
			.ExecuteReader();
		if (!reader.Read()) {
			pCha.SystemNotice(RES_STRING(GM_GAMEDB_CPP_00044));
			return false;
		}
	}

	try {
		auto txn = _db.BeginTransaction();

		int affected = _db.CreateCommand(
			"UPDATE guild SET try_total = try_total - 1, member_total = member_total + 1 "
			"WHERE guild_id = ? AND member_total < ? AND try_total > 0")
			.SetParam(1, static_cast<int>(pCha.GetGuildID()))
			.SetParam(2, static_cast<int>(g_Config.m_sGuildNum))
			.ExecuteNonQuery();
		if (affected == 0) {
			pCha.SystemNotice(RES_STRING(GM_GAMEDB_CPP_00046));
			return false;
		}

		affected = _db.CreateCommand(
			"UPDATE character SET guild_stat = 0, guild_permission = ? "
			"WHERE atorID = ? AND guild_id = ? AND guild_stat = 1 AND delflag = 0")
			.SetParam(1, static_cast<int>(emGldPermDefault))
			.SetParam(2, static_cast<int>(chaid))
			.SetParam(3, static_cast<int>(pCha.GetGuildID()))
			.ExecuteNonQuery();
		if (affected == 0) {
			pCha.SystemNotice(RES_STRING(GM_GAMEDB_CPP_00046));
			return false;
		}

		txn.Commit();
	} catch (const OdbcException&) {
		pCha.SystemNotice(RES_STRING(GM_GAMEDB_CPP_00046));
		return false;
	}

	// Уведомление MM
	auto l_wpk = net::msg::serialize(net::msg::MmGuildApproveMessage{
		static_cast<int64_t>(chaid), pCha.GetGuildID(),
		pCha.GetValidGuildName(), pCha.GetValidGuildMotto()
	});
	pCha.ReflectINFof(&pCha, l_wpk);

	// Уведомление GameServerGroup
	l_wpk = net::msg::serialize(net::msg::GmGuildApproveMessage{static_cast<int64_t>(chaid)});
	pCha.ReflectINFof(&pCha, l_wpk);

	const std::string cha_name = game_db.GetChaNameByID(chaid);
	auto msg = std::format("{} has been accepted to the guild!", cha_name);
	DWORD guildID = pCha.GetGuildID();
	g_pGameApp->GuildNotice(guildID, msg.c_str());

	return true;
}

bool CTableGuild::Reject(CCharacter& pCha, std::uint32_t chaid) {
	if (!pCha.HasGuild()) {
		return false;
	}

	// Проверяем права на отклонение заявки
	{
		auto reader = _db.CreateCommand(
			"SELECT c.atorID FROM character c "
			"WHERE c.atorID = ? AND c.guild_id = ? AND c.guild_permission & ? = ?")
			.SetParam(1, static_cast<int>(pCha.GetID()))
			.SetParam(2, static_cast<int>(pCha.GetGuildID()))
			.SetParam(3, static_cast<int>(emGldPermRecruit))
			.SetParam(4, static_cast<int>(emGldPermRecruit))
			.ExecuteReader();
		if (!reader.Read()) {
			pCha.SystemNotice(RES_STRING(GM_GAMEDB_CPP_00048));
			return false;
		}
	}

	try {
		auto txn = _db.BeginTransaction();

		int affected = _db.CreateCommand(
			"UPDATE character SET guild_id = 0, guild_stat = 0, guild_permission = 0 "
			"WHERE atorID = ? AND guild_id = ? AND guild_stat = 1")
			.SetParam(1, static_cast<int>(chaid))
			.SetParam(2, static_cast<int>(pCha.GetGuildID()))
			.ExecuteNonQuery();
		if (affected == 0) {
			pCha.SystemNotice(RES_STRING(GM_GAMEDB_CPP_00049));
			return false;
		}

		affected = _db.CreateCommand(
			"UPDATE guild SET try_total = try_total - 1 WHERE guild_id = ? AND try_total > 0")
			.SetParam(1, static_cast<int>(pCha.GetGuildID()))
			.ExecuteNonQuery();
		if (affected == 0) {
			pCha.SystemNotice(RES_STRING(GM_GAMEDB_CPP_00049));
			return false;
		}

		txn.Commit();
	} catch (const OdbcException&) {
		pCha.SystemNotice(RES_STRING(GM_GAMEDB_CPP_00049));
		return false;
	}

	// Уведомление MM
	auto l_wpk = net::msg::serialize(net::msg::MmGuildRejectMessage{
		static_cast<int64_t>(chaid), pCha.GetGuildName()
	});
	pCha.ReflectINFof(&pCha, l_wpk);
	return true;
}

bool CTableGuild::Kick(CCharacter& pCha, std::uint32_t chaid) {
	if (!pCha.HasGuild()) {
		return false;
	}

	// Проверяем права на исключение
	{
		auto reader = _db.CreateCommand(
			"SELECT c.atorID FROM character c "
			"WHERE c.atorID = ? AND c.guild_id = ? AND c.guild_permission & ? = ?")
			.SetParam(1, static_cast<int>(pCha.GetID()))
			.SetParam(2, static_cast<int>(pCha.GetGuildID()))
			.SetParam(3, static_cast<int>(emGldPermKick))
			.SetParam(4, static_cast<int>(emGldPermKick))
			.ExecuteReader();
		if (!reader.Read()) {
			pCha.SystemNotice(RES_STRING(GM_GAMEDB_CPP_00048));
			return false;
		}
	}

	if (chaid == pCha.GetID()) {
		pCha.SystemNotice(RES_STRING(GM_GAMEDB_CPP_00051));
		return false;
	}

	try {
		auto txn = _db.BeginTransaction();

		// Исключаем участника (нельзя исключить лидера)
		int affected = _db.CreateCommand(
			"UPDATE character SET guild_id = 0, guild_stat = 0, guild_permission = 0 "
			"WHERE atorID = ? AND guild_id = ? AND guild_stat = 0 "
			"AND atorID NOT IN (SELECT leader_id FROM guild WHERE guild_id = ?)")
			.SetParam(1, static_cast<int>(chaid))
			.SetParam(2, static_cast<int>(pCha.GetGuildID()))
			.SetParam(3, static_cast<int>(pCha.GetGuildID()))
			.ExecuteNonQuery();
		if (affected == 0) {
			pCha.SystemNotice(RES_STRING(GM_GAMEDB_CPP_00053));
			return false;
		}

		affected = _db.CreateCommand(
			"UPDATE guild SET member_total = member_total - 1 WHERE guild_id = ?")
			.SetParam(1, static_cast<int>(pCha.GetGuildID()))
			.ExecuteNonQuery();
		if (affected == 0) {
			pCha.SystemNotice(RES_STRING(GM_GAMEDB_CPP_00053));
			return false;
		}

		txn.Commit();
	} catch (const OdbcException&) {
		pCha.SystemNotice(RES_STRING(GM_GAMEDB_CPP_00053));
		return false;
	}

	// Уведомление MM
	auto l_wpk = net::msg::serialize(net::msg::MmGuildKickMessage{
		static_cast<int64_t>(chaid), pCha.GetGuildName()
	});
	pCha.ReflectINFof(&pCha, l_wpk);

	// Уведомление GameServerGroup
	l_wpk = net::msg::serialize(net::msg::GmGuildKickMessage{static_cast<int64_t>(chaid)});
	pCha.ReflectINFof(&pCha, l_wpk);

	// Уведомление клиенту
	l_wpk = net::msg::serializeMcGuildKickCmd();
	pCha.ReflectINFof(&pCha, l_wpk);

	return true;
}

bool CTableGuild::Leave(CCharacter& pCha) {
	if (!pCha.HasGuild()) {
		return false;
	}

	try {
		auto txn = _db.BeginTransaction();

		// Покидаем гильдию (лидер не может выйти)
		int affected = _db.CreateCommand(
			"UPDATE character SET guild_id = 0, guild_stat = 0, guild_permission = 0 "
			"WHERE atorID = ? AND guild_id = ? AND guild_stat = 0 "
			"AND atorID NOT IN (SELECT leader_id FROM guild WHERE guild_id = ?)")
			.SetParam(1, static_cast<int>(pCha.GetID()))
			.SetParam(2, static_cast<int>(pCha.GetGuildID()))
			.SetParam(3, static_cast<int>(pCha.GetGuildID()))
			.ExecuteNonQuery();
		if (affected == 0) {
			pCha.SystemNotice(RES_STRING(GM_GAMEDB_CPP_00055));
			return false;
		}

		affected = _db.CreateCommand(
			"UPDATE guild SET member_total = member_total - 1 WHERE guild_id = ?")
			.SetParam(1, static_cast<int>(pCha.GetGuildID()))
			.ExecuteNonQuery();
		if (affected == 0) {
			pCha.SystemNotice(RES_STRING(GM_GAMEDB_CPP_00055));
			return false;
		}

		txn.Commit();
	} catch (const OdbcException&) {
		pCha.SystemNotice(RES_STRING(GM_GAMEDB_CPP_00055));
		return false;
	}

	auto msg = std::format("{} has left the guild!", pCha.GetName());
	DWORD guildID = pCha.GetGuildID();
	g_pGameApp->GuildNotice(guildID, msg.c_str());

	pCha.SetGuildID(0);
	pCha.SetGuildName("");
	pCha.SetGuildMotto("");
	pCha.SyncGuildInfo();
	pCha.SystemNotice(RES_STRING(GM_GAMEDB_CPP_00056));

	// Уведомление GameServerGroup
	auto l_wpk = net::msg::serializeGmGuildLeaveCmd();
	pCha.ReflectINFof(&pCha, l_wpk);

	// Уведомление клиенту
	l_wpk = net::msg::serializeMcGuildLeaveCmd();
	pCha.ReflectINFof(&pCha, l_wpk);
	return true;
}

bool CTableGuild::Disband(CCharacter& pCha, const std::string& passwd) {
	if (!pCha.HasGuild()) {
		return false;
	}

	// Проверяем challlevel — нельзя распустить гильдию с активным вызовом
	{
		auto reader = _db.CreateCommand("SELECT challlevel FROM guild WHERE guild_id = ?")
			.SetParam(1, static_cast<int>(pCha.GetValidGuildID()))
			.ExecuteReader();
		if (!reader.Read()) {
			pCha.SystemNotice(RES_STRING(GM_GAMEDB_CPP_00060));
			return false;
		}
		int challLevel = reader.GetInt(0);
		if (challLevel > 0) {
			pCha.SystemNotice(RES_STRING(GM_GAMEDB_CPP_00057));
			return false;
		}
	}

	// Проверяем, не является ли гильдия чьим-то challid
	{
		auto reader = _db.CreateCommand("SELECT challlevel FROM guild WHERE challid = ?")
			.SetParam(1, static_cast<int>(pCha.GetValidGuildID()))
			.ExecuteReader();
		if (reader.Read()) {
			pCha.SystemNotice(RES_STRING(GM_GAMEDB_CPP_00059));
			return false;
		}
	}

	try {
		auto txn = _db.BeginTransaction();

		// Сброс гильдии (проверяем пароль)
		int affected = _db.CreateCommand(
			"UPDATE guild SET level = 0, gold = 0, bank = '', motto = '', passwd = '', "
			"leader_id = 0, exp = 0, member_total = 0, try_total = 0 "
			"WHERE guild_id = ? AND passwd = ?")
			.SetParam(1, static_cast<int>(pCha.GetGuildID()))
			.SetParam(2, passwd)
			.ExecuteNonQuery();
		if (affected == 0) {
			pCha.SystemNotice(RES_STRING(GM_GAMEDB_CPP_00062));
			return false;
		}

		// Убираем всех членов из гильдии
		affected = _db.CreateCommand(
			"UPDATE character SET guild_id = 0, guild_stat = 0, guild_permission = 0 "
			"WHERE guild_id = ?")
			.SetParam(1, static_cast<int>(pCha.GetGuildID()))
			.ExecuteNonQuery();
		if (affected == 0) {
			pCha.SystemNotice(RES_STRING(GM_GAMEDB_CPP_00062));
			return false;
		}

		txn.Commit();
	} catch (const OdbcException&) {
		pCha.SystemNotice(RES_STRING(GM_GAMEDB_CPP_00062));
		return false;
	}

	pCha.guildPermission = 0;

	// Уведомление GameServerGroup
	auto l_wpk = net::msg::serializeGmGuildDisbandCmd();
	pCha.ReflectINFof(&pCha, l_wpk);

	int guildID = pCha.GetGuildID();

	// Уведомление MM
	l_wpk = net::msg::serialize(net::msg::MmGuildDisbandMessage{static_cast<int64_t>(guildID)});
	pCha.ReflectINFof(&pCha, l_wpk);

	return true;
}

bool CTableGuild::Motto(CCharacter& pCha, const std::string& motto) {
	if (!pCha.HasGuild()) {
		return false;
	}

	int affected = _db.CreateCommand("UPDATE guild SET motto = ? WHERE guild_id = ?")
		.SetParam(1, motto)
		.SetParam(2, static_cast<int>(pCha.GetGuildID()))
		.ExecuteNonQuery();
	if (affected != 1) {
		pCha.SystemNotice(RES_STRING(GM_GAMEDB_CPP_00064));
		return false;
	}

	// Уведомление MM
	auto l_wpk = net::msg::serialize(net::msg::MmGuildMottoMessage{
		static_cast<int64_t>(pCha.GetGuildID()), motto
	});
	pCha.ReflectINFof(&pCha, l_wpk);

	// Уведомление GameServerGroup
	l_wpk = net::msg::serialize(net::msg::GmGuildMottoMessage{motto});
	pCha.ReflectINFof(&pCha, l_wpk);

	auto msg = std::format("Guild Motto: {}", motto);
	DWORD guildID = pCha.GetGuildID();
	g_pGameApp->GuildNotice(guildID, msg.c_str());

	return true;
}

bool CTableGuild::GetGuildName(std::int32_t lGuildID, std::string& strGuildName) {
	auto reader = _db.CreateCommand("SELECT guild_name FROM guild WHERE guild_id = ?")
		.SetParam(1, lGuildID)
		.ExecuteReader();
	if (reader.Read()) {
		strGuildName = reader.GetString(0);
		return true;
	}
	return true; // Совместимость: старый код всегда возвращал true
}

bool CTableGuild::Leizhu(CCharacter& pCha, std::uint8_t byLevel, std::uint32_t dwMoney) {
	if (!pCha.HasGuild() || byLevel < 1 || byLevel > 3) {
		return false;
	}

	if (dwMoney == 0) {
		pCha.SystemNotice(RES_STRING(GM_GAMEDB_CPP_00065));
		return false;
	}

	if (pCha.GetValidGuildID() <= 0) {
		return false;
	}

	// Проверяем, что вызывающий — лидер гильдии
	{
		auto reader = _db.CreateCommand(
			"SELECT guild_id, guild_name, challid, challmoney, leader_id, challstart "
			"FROM guild WHERE guild_id = ?")
			.SetParam(1, static_cast<int>(pCha.GetValidGuildID()))
			.ExecuteReader();
		if (!reader.Read()) {
			pCha.SystemNotice(RES_STRING(GM_GAMEDB_CPP_00066));
			return false;
		}
		if (pCha.GetID() != reader.GetInt(4)) {
			return false; // Не лидер
		}
	}

	// Проверяем, что нашу гильдию никто не вызывает
	{
		auto reader = _db.CreateCommand(
			"SELECT guild_id, guild_name FROM guild WHERE challid = ?")
			.SetParam(1, static_cast<int>(pCha.GetValidGuildID()))
			.ExecuteReader();
		if (reader.Read()) {
			pCha.SystemNotice(RES_STRING(GM_GAMEDB_CPP_00067), reader.GetString(1).c_str());
			return false;
		}
	}

	// Проверяем, не занят ли уровень другой гильдией
	{
		auto reader = _db.CreateCommand(
			"SELECT guild_id, guild_name, challid, challmoney FROM guild WHERE challlevel = ?")
			.SetParam(1, static_cast<int>(byLevel))
			.ExecuteReader();
		if (reader.Read()) {
			pCha.SystemNotice(RES_STRING(GM_GAMEDB_CPP_00069), reader.GetString(1).c_str(), byLevel);
			return false;
		}
	}

	// Проверяем текущий уровень нашей гильдии
	{
		auto reader = _db.CreateCommand("SELECT challlevel FROM guild WHERE guild_id = ?")
			.SetParam(1, static_cast<int>(pCha.GetValidGuildID()))
			.ExecuteReader();
		if (reader.Read()) {
			int curLevel = reader.GetInt(0);
			if (curLevel > 0) {
				pCha.SystemNotice(RES_STRING(GM_GAMEDB_CPP_00070), curLevel);
				return false;
			}
		}
	}

	// Проверяем деньги
	DWORD dwMoneyArray[3] = {5000000, 3000000, 1000000};
	if (dwMoney < dwMoneyArray[byLevel - 1] || !pCha.HasMoney(dwMoney)) {
		pCha.SystemNotice(RES_STRING(GM_GAMEDB_CPP_00071), byLevel, dwMoneyArray[byLevel - 1]);
		return false;
	}

	try {
		auto txn = _db.BeginTransaction();

		int affected = _db.CreateCommand(
			"UPDATE guild SET challid = 0, challstart = 0, challmoney = 0, challlevel = ? "
			"WHERE guild_id = ?")
			.SetParam(1, static_cast<int>(byLevel))
			.SetParam(2, static_cast<int>(pCha.GetValidGuildID()))
			.ExecuteNonQuery();
		if (affected == 0) {
			ToLogService("common",
				"challenge consortia over,leizhu failed:update lost consortia data operater failed! "
				"consortiaID = {}.consortia level:{}",
				pCha.GetValidGuildID(), byLevel);
			return false;
		}

		txn.Commit();
	} catch (const OdbcException&) {
		pCha.SystemNotice(RES_STRING(GM_GAMEDB_CPP_00072));
		return false;
	}

	if (pCha.TakeMoney(RES_STRING(GM_GAMEDB_CPP_00073), dwMoney)) {
		pCha.SystemNotice(RES_STRING(GM_GAMEDB_CPP_00074), pCha.GetGuildName(), byLevel);
	}
	this->ListChallenge(pCha);
	return true;
}

bool CTableGuild::Challenge(CCharacter& pCha, std::uint8_t byLevel, std::uint32_t dwMoney) {
	if (!pCha.HasGuild() || byLevel < 1 || byLevel > 3) {
		return false;
	}

	if (dwMoney == 0) {
		pCha.SystemNotice(RES_STRING(GM_GAMEDB_CPP_00075));
		return false;
	}

	if (pCha.GetValidGuildID() <= 0) {
		return false;
	}

	// Проверяем, что вызывающий — лидер гильдии
	{
		auto reader = _db.CreateCommand(
			"SELECT leader_id FROM guild WHERE guild_id = ?")
			.SetParam(1, static_cast<int>(pCha.GetValidGuildID()))
			.ExecuteReader();
		if (!reader.Read()) {
			pCha.SystemNotice(RES_STRING(GM_GAMEDB_CPP_00066));
			return false;
		}
		if (pCha.GetID() != reader.GetInt(0)) {
			return false;
		}
	}

	// Проверяем, что нашу гильдию никто не вызывает
	{
		auto reader = _db.CreateCommand(
			"SELECT guild_name FROM guild WHERE challid = ?")
			.SetParam(1, static_cast<int>(pCha.GetValidGuildID()))
			.ExecuteReader();
		if (reader.Read()) {
			pCha.SystemNotice(RES_STRING(GM_GAMEDB_CPP_00067), reader.GetString(0).c_str());
			return false;
		}
	}

	// Ищем гильдию-владельца этого уровня
	DWORD dwGuildID = 0;
	DWORD dwChallID = 0;
	DWORD dwChallMoney = 0;
	std::string szGuild;
	{
		auto reader = _db.CreateCommand(
			"SELECT guild_id, guild_name, challid, challmoney FROM guild WHERE challlevel = ?")
			.SetParam(1, static_cast<int>(byLevel))
			.ExecuteReader();
		if (reader.Read()) {
			dwGuildID = reader.GetInt(0);
			szGuild = reader.GetString(1);
			dwChallID = reader.GetInt(2);
			dwChallMoney = reader.GetInt(3);
		}
	}

	// Если уровень никем не занят — занимаем (аналогично Leizhu)
	if (dwGuildID == 0) {
		DWORD dwMoneyArray[3] = {5000000, 3000000, 1000000};
		if (dwMoney < dwMoneyArray[byLevel - 1] || !pCha.HasMoney(dwMoney)) {
			pCha.SystemNotice(RES_STRING(GM_GAMEDB_CPP_00077), byLevel, dwMoneyArray[byLevel - 1]);
			return false;
		}

		try {
			auto txn = _db.BeginTransaction();

			int affected = _db.CreateCommand(
				"UPDATE guild SET challid = 0, challstart = 0, challmoney = 0, challlevel = ? "
				"WHERE guild_id = ?")
				.SetParam(1, static_cast<int>(byLevel))
				.SetParam(2, static_cast<int>(pCha.GetValidGuildID()))
				.ExecuteNonQuery();
			if (affected == 0) {
				ToLogService("common",
					"challenge consortia over,leizhu failed:update lost consortia data operater failed! "
					"consortiaID = {}.consortia level:{}",
					pCha.GetValidGuildID(), byLevel);
				return false;
			}

			txn.Commit();
		} catch (const OdbcException&) {
			pCha.SystemNotice(RES_STRING(GM_GAMEDB_CPP_00072));
			return false;
		}

		if (pCha.TakeMoney(RES_STRING(GM_GAMEDB_CPP_00073), dwMoney)) {
			pCha.SystemNotice(RES_STRING(GM_GAMEDB_CPP_00074), pCha.GetGuildName(), byLevel);
		}
		this->ListChallenge(pCha);
		return true;
	}

	// Проверяем текущий challlevel нашей гильдии
	BYTE byLvData = 0;
	{
		auto reader = _db.CreateCommand("SELECT challlevel FROM guild WHERE guild_id = ?")
			.SetParam(1, static_cast<int>(pCha.GetValidGuildID()))
			.ExecuteReader();
		if (!reader.Read()) {
			pCha.SystemNotice(RES_STRING(GM_GAMEDB_CPP_00078));
			return false;
		}
		byLvData = static_cast<BYTE>(reader.GetInt(0));
	}

	if (byLvData != 0 && byLevel > byLvData) {
		pCha.SystemNotice(RES_STRING(GM_GAMEDB_CPP_00080));
		return false;
	}

	if (pCha.GetPlayer()->GetDBChaId() == dwChallID) {
		pCha.SystemNotice(RES_STRING(GM_GAMEDB_CPP_00081));
		return false;
	} else if (pCha.GetValidGuildID() == dwGuildID) {
		pCha.SystemNotice(RES_STRING(GM_GAMEDB_CPP_00082));
		return false;
	} else if (dwMoney < dwChallMoney + 50000) {
		pCha.SystemNotice(RES_STRING(GM_GAMEDB_CPP_00083), dwMoney);
		return false;
	}

	if (!pCha.HasMoney(dwMoney)) {
		pCha.SystemNotice(RES_STRING(GM_GAMEDB_CPP_00084), dwMoney);
		return false;
	}

	// Делаем ставку (challstart == 0 гарантирует, что бой ещё не начался)
	try {
		auto txn = _db.BeginTransaction();

		int affected = _db.CreateCommand(
			"UPDATE guild SET challid = ?, challmoney = ? "
			"WHERE guild_id = ? AND challmoney < ? AND challstart = 0")
			.SetParam(1, static_cast<int>(pCha.GetGuildID()))
			.SetParam(2, static_cast<int>(dwMoney))
			.SetParam(3, static_cast<int>(dwGuildID))
			.SetParam(4, static_cast<int>(dwMoney))
			.ExecuteNonQuery();
		if (affected == 0) {
			pCha.SystemNotice(RES_STRING(GM_GAMEDB_CPP_00085));
			return false;
		}

		txn.Commit();
	} catch (const OdbcException&) {
		pCha.SystemNotice(RES_STRING(GM_GAMEDB_CPP_00085));
		return false;
	}

	pCha.TakeMoney(RES_STRING(GM_GAMEDB_CPP_00073), dwMoney);

	// Возвращаем деньги предыдущему претенденту
	if (dwChallID > 0 && dwChallMoney > 0) {
		auto l_wpk = net::msg::serialize(net::msg::GmGuildChallMoneyMessage{
			static_cast<int64_t>(dwChallID), static_cast<int64_t>(dwChallMoney),
			szGuild, pCha.GetGuildName()
		});
		pCha.ReflectINFof(&pCha, l_wpk);
	}

	ListChallenge(pCha);
	return true;
}

void CTableGuild::ListChallenge(CCharacter& pCha) {
	net::msg::McGuildListChallMessage challMsg{};

	// Определяем, является ли персонаж лидером своей гильдии
	if (pCha.GetValidGuildID() > 0) {
		auto reader = _db.CreateCommand("SELECT leader_id FROM guild WHERE guild_id = ?")
			.SetParam(1, static_cast<int>(pCha.GetValidGuildID()))
			.ExecuteReader();
		if (!reader.Read()) {
			pCha.SystemNotice(RES_STRING(GM_GAMEDB_CPP_00066));
			return;
		}
		challMsg.isLeader = (pCha.GetID() == reader.GetInt(0)) ? 1 : 0;
	}

	// Получаем информацию по каждому уровню (1–3)
	for (int i = 1; i <= 3; ++i) {
		auto reader = _db.CreateCommand(
			"SELECT guild_id, guild_name, challid, challmoney, leader_id, challstart "
			"FROM guild WHERE challlevel = ?")
			.SetParam(1, i)
			.ExecuteReader();
		if (!reader.Read()) {
			continue;
		}

		DWORD dwGuildID = reader.GetInt(0);
		std::string guildName = reader.GetString(1);
		DWORD dwChallID = reader.GetInt(2);
		DWORD dwChallMoney = reader.GetInt(3);
		BYTE byStart = static_cast<BYTE>(reader.GetInt(5));

		if (dwChallID != 0) {
			// Получаем имя гильдии-претендента
			auto reader2 = _db.CreateCommand(
				"SELECT guild_name FROM guild WHERE guild_id = ?")
				.SetParam(1, static_cast<int>(dwChallID))
				.ExecuteReader();
			std::string challGuildName;
			if (reader2.Read()) {
				challGuildName = reader2.GetString(0);
			}
			challMsg.entries[i - 1] = {
				static_cast<int64_t>(i), static_cast<int64_t>(byStart),
				guildName, challGuildName, static_cast<int64_t>(dwChallMoney)
			};
		} else {
			challMsg.entries[i - 1] = {
				static_cast<int64_t>(i), static_cast<int64_t>(byStart),
				guildName, std::string(""), static_cast<int64_t>(dwChallMoney)
			};
		}
	}

	auto l_wpk = net::msg::serialize(challMsg);
	pCha.ReflectINFof(&pCha, l_wpk);
}

bool CTableGuild::HasGuildLevel(CCharacter& pChar, std::uint8_t byLevel) {
	if (!pChar.HasGuild()) {
		return false;
	}

	auto reader = _db.CreateCommand("SELECT challlevel FROM guild WHERE guild_id = ?")
		.SetParam(1, static_cast<int>(pChar.GetValidGuildID()))
		.ExecuteReader();
	if (reader.Read()) {
		return byLevel == static_cast<BYTE>(reader.GetInt(0));
	}
	return false;
}

bool CTableGuild::HasCall(std::uint8_t byLevel) {
	auto reader = _db.CreateCommand(
		"SELECT challid, challstart FROM guild WHERE challlevel = ?")
		.SetParam(1, static_cast<int>(byLevel))
		.ExecuteReader();
	if (reader.Read()) {
		int challId = reader.GetInt(0);
		int challStart = reader.GetInt(1);
		return challId != 0 && challStart == 1;
	}
	return false;
}

bool CTableGuild::StartChall(std::uint8_t byLevel) {
	ToLogService("common", "range level {} challenge start treat with....", byLevel);

	DWORD dwGuildID = 0;
	DWORD dwChallID = 0;
	DWORD dwChallMoney = 0;

	{
		auto reader = _db.CreateCommand(
			"SELECT guild_id, guild_name, challid, challmoney FROM guild WHERE challlevel = ?")
			.SetParam(1, static_cast<int>(byLevel))
			.ExecuteReader();
		if (!reader.Read()) {
			return false;
		}
		dwGuildID = reader.GetInt(0);
		dwChallID = reader.GetInt(2);
		dwChallMoney = reader.GetInt(3);
	}

	if (dwGuildID == 0) {
		return false;
	}

	int affected = _db.CreateCommand(
		"UPDATE guild SET challstart = 1 WHERE guild_id = ? AND challstart = 0")
		.SetParam(1, static_cast<int>(dwGuildID))
		.ExecuteNonQuery();
	if (affected == 0) {
		ToLogService("common",
			"challenge consortia data operator failed!consortia battle already start or inexistence!");
		return false;
	}

	ToLogService("common", "range level {} challenge start succeed !GUILD1 = {}, GUILD2 = {}, Money = {}.",
				 byLevel, dwGuildID, dwChallID, dwChallMoney);
	return true;
}

void CTableGuild::EndChall(std::uint32_t dwGuild1, std::uint32_t dwGuild2, bool bChall) {
	ToLogService("common", "arranger level consortia game start operator finish GUILD1 = {}, GUILD2 = {}...",
				 dwGuild1, dwGuild2);

	// Проверяем guild1 → challid == guild2
	{
		auto reader = _db.CreateCommand(
			"SELECT challstart, guild_name, challid, challmoney, challlevel "
			"FROM guild WHERE guild_id = ?")
			.SetParam(1, static_cast<int>(dwGuild1))
			.ExecuteReader();
		if (reader.Read()) {
			DWORD dwChallID = reader.GetInt(2);
			DWORD dwChallMoney = reader.GetInt(3);
			BYTE byLevel = static_cast<BYTE>(reader.GetInt(4));
			if (dwChallID == dwGuild2) {
				ChallMoney(byLevel, bChall, dwGuild1, dwGuild2, dwChallMoney);
				ToLogService("common",
					"range level {} consortia challenge over!GUILD1 = {}, GUILD2 = {}, Money = {}.",
					byLevel, dwGuild1, dwGuild2, dwChallMoney);
				return;
			}
		}
	}

	// Проверяем guild2 → challid == guild1
	{
		auto reader = _db.CreateCommand(
			"SELECT challstart, guild_name, challid, challmoney, challlevel "
			"FROM guild WHERE guild_id = ?")
			.SetParam(1, static_cast<int>(dwGuild2))
			.ExecuteReader();
		if (reader.Read()) {
			DWORD dwChallID = reader.GetInt(2);
			DWORD dwChallMoney = reader.GetInt(3);
			BYTE byLevel = static_cast<BYTE>(reader.GetInt(4));
			if (dwChallID == dwGuild1) {
				ChallMoney(byLevel, !bChall, dwGuild2, dwGuild1, dwChallMoney);
				ToLogService("common",
					"range level {} consortia challenge over!GUILD1 = {}, GUILD2 = {}, Money = {}.",
					byLevel, dwGuild2, dwGuild1, dwChallMoney);
				return;
			}
		}
	}

	ToLogService("common",
		"consortia challenge result disposal failed!GUILD1 = {}, GUILD2 = {}, ChallFlag = {}.",
		dwGuild1, dwGuild2, (bChall) ? 1 : 0);
}

bool CTableGuild::ChallWin(bool bUpdate, std::uint8_t byLevel, std::uint32_t dwWinGuildID, std::uint32_t dwFailerGuildID) {
	try {
		auto txn = _db.BeginTransaction();

		if (bUpdate) {
			// Получаем текущий уровень победителя
			BYTE byLvData = 0;
			{
				auto reader = _db.CreateCommand("SELECT challlevel FROM guild WHERE guild_id = ?")
					.SetParam(1, static_cast<int>(dwWinGuildID))
					.ExecuteReader();
				if (!reader.Read()) {
					ToLogService("common",
						"finish challenge consortialeizhu failed:inquire about failed consortia level "
						"info failed!GUILDID = {}, WINID = {}.",
						dwFailerGuildID, dwWinGuildID);
					return false;
				}
				byLvData = static_cast<BYTE>(reader.GetInt(0));
			}

			if (byLvData > 0) {
				// Обмен уровней: проигравший получает меньший уровень
				if (byLvData < byLevel) {
					BYTE byTemp = byLevel;
					byLevel = byLvData;
					byLvData = byTemp;
				}

				int affected = _db.CreateCommand(
					"UPDATE guild SET challid = 0, challstart = 0, challmoney = 0, challlevel = ? "
					"WHERE guild_id = ?")
					.SetParam(1, static_cast<int>(byLvData))
					.SetParam(2, static_cast<int>(dwFailerGuildID))
					.ExecuteNonQuery();
				if (affected == 0) {
					ToLogService("common",
						"challenge consortia over,leizhu failed:update lost consortia data operater failed! "
						"consortiaID = {}.consortia level:{}.",
						dwFailerGuildID, byLevel);
					return false;
				}
			} else {
				int affected = _db.CreateCommand(
					"UPDATE guild SET challid = 0, challstart = 0, challmoney = 0, challlevel = 0 "
					"WHERE guild_id = ?")
					.SetParam(1, static_cast<int>(dwFailerGuildID))
					.ExecuteNonQuery();
				if (affected == 0) {
					ToLogService("common",
						"challenge consortia over,leizhu failed:update lost consortia data operater failed! "
						"consortiaID = {}.consortia level:{}.",
						dwFailerGuildID, byLevel);
					return false;
				}
			}
		}

		// Обновляем победителя
		int affected = _db.CreateCommand(
			"UPDATE guild SET challid = 0, challstart = 0, challmoney = 0, challlevel = ? "
			"WHERE guild_id = ?")
			.SetParam(1, static_cast<int>(byLevel))
			.SetParam(2, static_cast<int>(dwWinGuildID))
			.ExecuteNonQuery();
		if (affected == 0) {
			ToLogService("common",
				"challenge consortia over,update winner consortia data operator failed!"
				"inexistence consortia!consortiaID = {}.consortia level{}.",
				dwWinGuildID, byLevel);
			return false;
		}

		txn.Commit();
		return true;
	} catch (const OdbcException&) {
		ToLogService("common", "challenge consortia data referring failed,retry later on");
		return false;
	}
}

void CTableGuild::ChallMoney(std::uint8_t byLevel, bool bChall, std::uint32_t dwGuildID, std::uint32_t dwChallID, std::uint32_t dwMoney) {
	if (bChall) {
		ToLogService("common", "challenge failed: winner:ID = {},loser:ID = {}, money = {},level:{}.",
					 dwGuildID, dwChallID, dwMoney, byLevel);
		if (!ChallWin(FALSE, byLevel, dwGuildID, dwChallID)) {
			return;
		}

		if (dwChallID != 0) {
			dwMoney = DWORD(float(dwMoney * 80) / 100);
			auto l_wpk = net::msg::serialize(net::msg::MpGuildChallPrizeMoneyMessage{
				static_cast<int64_t>(dwGuildID), static_cast<int64_t>(dwMoney)
			});
			SENDTOGROUP(l_wpk);
		}
	} else {
		ToLogService("common", "challenge succeedwinner:ID = {},loser:ID = {}, money = {},level:{}.",
					 dwChallID, dwGuildID, dwMoney, byLevel);
		if (!ChallWin(TRUE, byLevel, dwChallID, dwGuildID)) {
			return;
		}

		dwMoney = DWORD(float(dwMoney * 80) / 100);
		auto l_wpk = net::msg::serialize(net::msg::MpGuildChallPrizeMoneyMessage{
			static_cast<int64_t>(dwChallID), static_cast<int64_t>(dwMoney)
		});
		SENDTOGROUP(l_wpk);
	}
}

bool CTableGuild::GetChallInfo(std::uint8_t byLevel, std::uint32_t& dwGuildID1, std::uint32_t& dwGuildID2, std::uint32_t& dwMoney) {
	auto reader = _db.CreateCommand(
		"SELECT guild_id, challid, challmoney FROM guild WHERE challlevel = ?")
		.SetParam(1, static_cast<int>(byLevel))
		.ExecuteReader();
	if (reader.Read()) {
		dwGuildID1 = reader.GetInt(0);
		dwGuildID2 = reader.GetInt(1);
		dwMoney = reader.GetInt(2);
		return true;
	}
	return false;
}

bool PlayerStorage::SetGuildPermission(std::int32_t atorID, std::uint32_t perm, std::int32_t guild_id) {
	_characters.Execute(
		"UPDATE character SET guild_permission=? WHERE atorID=? AND guild_id=?",
		static_cast<int>(perm),
		atorID,
		guild_id);
	return true;
}

bool PlayerStorage::SetChaAddr(std::uint32_t atorID, Long addr) {
	_characters.Execute(
		"UPDATE character SET endeMem=? WHERE atorID=?",
		static_cast<int>(addr),
		static_cast<int>(atorID));
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

bool CGameDB::SavePlayerKitbag(CPlayer& pPlayer, std::int8_t chSaveType) {
	if (!_tab_res.SaveKitbagData(*pPlayer.GetMainCha())) {
		return false;
	}
	if (!_tab_boat.SaveAllCabin(pPlayer, chSaveType)) {
		return false;
	}
	return true;
}

bool CGameDB::SaveChaAssets(CCharacter& pCCha) {
	if (!pCCha.GetPlayer()) {
		return false;
	}
	DWORD dwStartTick = GetTickCount();
	if (!_tab_cha.SaveMoney(*pCCha.GetPlayer())) {
		return false;
	}
	if (!pCCha.IsBoat()) {
		if (!_tab_res.SaveKitbagData(pCCha)) {
			return false;
		}
	}
	else {
		if (!_tab_boat.SaveCabin(pCCha, enumSAVE_TYPE_TRADE)) {
			return false;
		}
	}
	ToLogService("common", "Save assets {} in {} ms", pCCha.GetLogName(), GetTickCount() - dwStartTick);
	return true;
}

bool CGameDB::GetWinItemno(std::int32_t issue, std::string& itemno) {
	try {
		auto row = _lotterySettings.FindOne("state = 0 AND issue = ?", issue);
		if (!row) {
			return false;
		}
		itemno = row->itemno;
		return !itemno.empty();
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "GetWinItemno failed: {}", e.what());
		return false;
	}
}

bool CGameDB::GetLotteryIssue(std::int32_t& issue) {
	try {
		auto row = _lotterySettings.FindOne("state = 0");
		if (!row) {
			return false;
		}
		issue = row->issue;
		return true;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "GetLotteryIssue failed: {}", e.what());
		return false;
	}
}

bool CGameDB::AddIssue(std::int32_t issue) {
	try {
		_lotterySettings.Execute(
			"INSERT INTO LotterySetting (section, issue, state, createdate, updatetime) VALUES (1, ?, 0, getdate(), getdate())",
			issue);
		return true;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "AddIssue failed: {}", e.what());
		return false;
	}
}

bool CGameDB::DisuseIssue(std::int32_t issue, std::int32_t state) {
	try {
		return _lotterySettings.Execute(
			"UPDATE LotterySetting SET state = ?, updatetime = getdate() WHERE issue = ?",
			state, issue) > 0;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "DisuseIssue failed: {}", e.what());
		return false;
	}
}

bool CGameDB::LotteryIsExsit(std::int32_t issue, const std::string& itemno) {
	try {
		auto rows = _tickets.FindAll("issue = ? AND itemno = ?", issue, std::string_view(itemno));
		return !rows.empty();
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "LotteryIsExsit failed: {}", e.what());
		return false;
	}
}

bool CGameDB::AddLotteryTicket(CCharacter& pCCha, std::int32_t issue, char itemno[6][2]) {
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
				_tickets.Execute(
					"INSERT INTO Ticket (atorID, issue, itemno, real, buydate) VALUES (?, ?, ?, 0, getdate())",
					pCCha.m_ID, issue, std::string_view(no, 6));
			}
		}
		char mainNo[7]{};
		for (int j = 0; j < 6; j++) {
			mainNo[j] = itemno[j][0];
		}
		_tickets.Execute(
			"INSERT INTO Ticket (atorID, issue, itemno, real, buydate) VALUES (?, ?, ?, 1, getdate())",
			pCCha.m_ID, issue, std::string_view(mainNo, 6));
		return true;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "AddLotteryTicket failed: {}", e.what());
		return false;
	}
}

bool CGameDB::CalWinTicket(std::int32_t issue, std::int32_t max, std::string& itemno) {
	try {
		int probability = rand() % 2 + 1;
		if (issue % probability == 0) {
			// Подзапрос с агрегацией — оставляем _db.CreateCommand (JOIN/subquery)
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
				_winTickets.Execute(
					"UPDATE WinTicket SET num = num + 1 WHERE issue = ? AND itemno = ?",
					issue, std::string_view(itemno));
				_lotterySettings.Execute(
					"UPDATE LotterySetting SET itemno = ?, updatetime = getdate() WHERE issue = ?",
					std::string_view(itemno), issue);
				return true;
			}
		}
		std::string buffer;
		do {
			buffer = std::format("{:06d}", rand() % 999999 + 1);
		}
		while (LotteryIsExsit(issue, buffer));
		itemno = buffer;
		_lotterySettings.Execute(
			"UPDATE LotterySetting SET itemno = ?, updatetime = getdate() WHERE issue = ?",
			std::string_view(itemno), issue);
		return true;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "CalWinTicket failed: {}", e.what());
		return false;
	}
}

bool CGameDB::IsValidAmphitheaterTeam(std::int32_t teamID, std::int32_t captainID, std::int32_t member1, std::int32_t member2) {
	try {
		auto m1 = std::format("{},{}", member1, member2);
		auto m2 = std::format("{},{}", member2, member1);
		auto row = _amphiTeams.FindOne(
			"id = ? AND captain = ? AND (member = ? OR member = ?)",
			teamID, captainID, std::string_view(m1), std::string_view(m2));
		return row.has_value();
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "IsValidAmphitheaterTeam: {}", e.what());
		return false;
	}
}

bool CGameDB::IsMasterRelation(std::int32_t masterID, std::int32_t prenticeID) {
	try {
		auto row = _masters.FindOne("cha_id1 = ? AND cha_id2 = ?", prenticeID, masterID);
		return row.has_value();
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "IsMasterRelation: {}", e.what());
		return false;
	}
}

bool CGameDB::GetAmphitheaterSeasonAndRound(std::int32_t& season, std::int32_t& round) {
	try {
		auto row = _amphiSettings.FindOne("state = 0");
		if (!row) {
			return false;
		}
		season = row->season;
		round = row->round;
		return true;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "GetAmphitheaterSeasonAndRound: {}", e.what());
		return false;
	}
}

bool CGameDB::AddAmphitheaterSeason(std::int32_t season) {
	try {
		_amphiSettings.Execute(
			"INSERT INTO AmphitheaterSetting (section, season, [round], state, createdate, updatetime, winner) VALUES (1, ?, 1, 0, getdate(), getdate(), NULL)",
			season);
		return true;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "AddAmphitheaterSeason: {}", e.what());
		return false;
	}
}

bool CGameDB::DisuseAmphitheaterSeason(std::int32_t season, std::int32_t state, const std::string& winner) {
	try {
		return _amphiSettings.Execute(
			"UPDATE AmphitheaterSetting SET state = ?, updatetime = getdate(), winner = ? WHERE season = ?",
			state, std::string_view(winner), season) > 0;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "DisuseAmphitheaterSeason: {}", e.what());
		return false;
	}
}

bool CGameDB::UpdateAmphitheaterRound(std::int32_t season, std::int32_t round) {
	try {
		return _amphiSettings.Execute(
			"UPDATE AmphitheaterSetting SET [round] = ?, updatetime = getdate() WHERE season = ?",
			round, season) > 0;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "UpdateAmphitheaterRound: {}", e.what());
		return false;
	}
}

bool CGameDB::GetAmphitheaterTeamCount(std::int32_t& count) {
	try {
		auto rows = _amphiTeams.FindAll("state > ?", static_cast<int>(AmphitheaterTeam::enumNotUse));
		count = static_cast<int>(rows.size());
		return true;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "GetAmphitheaterTeamCount: {}", e.what());
		return false;
	}
}

bool CGameDB::GetAmphitheaterNoUseTeamID(std::int32_t& teamID) {
	try {
		auto row = _amphiTeams.FindOne("state = ?", static_cast<int>(AmphitheaterTeam::enumNotUse));
		if (!row) {
			return false;
		}
		teamID = row->id;
		return true;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "GetAmphitheaterNoUseTeamID: {}", e.what());
		return false;
	}
}

bool CGameDB::AmphitheaterTeamSignUP(std::int32_t& teamID, std::int32_t captain, std::int32_t member1, std::int32_t member2) {
	try {
		if (teamID < 0 && !GetAmphitheaterNoUseTeamID(teamID)) {
			return false;
		}
		auto memberStr = std::format("{},{}", member1, member2);
		return _amphiTeams.Execute(
			std::format(
				"UPDATE AmphitheaterTeam SET captain = ?, member = ?, state = {}, updatetime = getdate() WHERE id = ?",
				static_cast<int>(AmphitheaterTeam::enumUse)),
			captain, std::string_view(memberStr), teamID) > 0;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "AmphitheaterTeamSignUP: {}", e.what());
		return false;
	}
}

bool CGameDB::AmphitheaterTeamCancel(std::int32_t teamID) {
	try {
		return _amphiTeams.Execute(
			std::format(
				"UPDATE AmphitheaterTeam SET captain = null, member = null, matchno = 0, state = {}, updatetime = getdate() WHERE id = ?",
				static_cast<int>(AmphitheaterTeam::enumNotUse)),
			teamID) > 0;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "AmphitheaterTeamCancel: {}", e.what());
		return false;
	}
}

bool CGameDB::IsAmphitheaterLogin(std::int32_t pActorID) {
	try {
		auto idStr = std::to_string(pActorID);
		auto like1 = idStr + ",%";
		auto like2 = "%," + idStr;
		auto rows = _amphiTeams.FindAll(
			"captain = ? OR member LIKE ? OR member LIKE ?",
			pActorID, std::string_view(like1), std::string_view(like2));
		return rows.empty();
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "IsAmphitheaterLogin: {}", e.what());
		return false;
	}
}

bool CGameDB::IsMapFull(std::int32_t MapID, std::int32_t& PActorIDNum) {
	try {
		auto rows = _amphiTeams.FindAll("map = ?", MapID);
		PActorIDNum = static_cast<int>(rows.size());
		return PActorIDNum <= 2;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "IsMapFull: {}", e.what());
		return false;
	}
}

bool CGameDB::UpdateMapNum(std::int32_t Teamid, std::int32_t Mapid, std::int32_t MapFlag) {
	try {
		return _amphiTeams.Execute(
			"UPDATE AmphitheaterTeam SET mapflag = ? WHERE id = ? AND map = ?",
			MapFlag, Teamid, Mapid) > 0;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "UpdateMapNum: {}", e.what());
		return false;
	}
}

bool CGameDB::GetMapFlag(std::int32_t Teamid, std::int32_t& Mapflag) {
	try {
		auto row = _amphiTeams.FindOne("id = ?", Teamid);
		if (!row) {
			return false;
		}
		Mapflag = row->mapflag;
		return Mapflag < 2;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "GetMapFlag: {}", e.what());
		return false;
	}
}

bool CGameDB::SetMaxBallotTeamRelive() {
	try {
		auto promotionRows = _amphiTeams.FindAll("state = ?", static_cast<int>(AmphitheaterTeam::enumPromotion));
		int count = static_cast<int>(promotionRows.size());
		int oddOrEven = (count % 2 == 0) ? 2 : 1;
		// Подзапрос с TOP + ORDER BY — оставляем форматированный SQL через Execute
		_amphiTeams.Execute(std::format(
			"UPDATE AmphitheaterTeam SET state = {}, relivenum = 0 WHERE id IN "
			"(SELECT TOP {} id FROM AmphitheaterTeam WHERE state = {} ORDER BY relivenum DESC)",
			static_cast<int>(AmphitheaterTeam::enumPromotion), oddOrEven,
			static_cast<int>(AmphitheaterTeam::enumRelive)));
		_amphiTeams.Execute(std::format(
			"UPDATE AmphitheaterTeam SET state = {} WHERE state = {} OR state = {}",
			static_cast<int>(AmphitheaterTeam::enumOut), static_cast<int>(AmphitheaterTeam::enumRelive),
			static_cast<int>(AmphitheaterTeam::enumUse)));
		return true;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "SetMaxBallotTeamRelive: {}", e.what());
		return false;
	}
}

bool CGameDB::SetMatchResult(std::int32_t Teamid1, std::int32_t Teamid2, std::int32_t Id1state, std::int32_t Id2state) {
	try {
		_amphiTeams.Execute(
			"UPDATE AmphitheaterTeam SET state = ? WHERE id = ?",
			Id1state, Teamid1);
		_amphiTeams.Execute(
			"UPDATE AmphitheaterTeam SET state = ? WHERE id = ?",
			Id2state, Teamid2);
		return true;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "SetMatchResult: {}", e.what());
		return false;
	}
}

bool CGameDB::GetCaptainByMapId(std::int32_t Mapid, std::string& Captainid1, std::string& Captainid2) {
	try {
		auto rows = _amphiTeams.FindAll("map = ?", Mapid);
		if (rows.empty() || rows.size() > 2) {
			return false;
		}
		Captainid1 = std::to_string(rows[0].captain);
		Captainid2 = rows.size() > 1 ? std::to_string(rows[1].captain) : "";
		return true;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "GetCaptainByMapId: {}", e.what());
		return false;
	}
}

bool CGameDB::UpdateMap(std::int32_t Mapid) {
	try {
		return _amphiTeams.Execute(
			"UPDATE AmphitheaterTeam SET map = null WHERE map = ?",
			Mapid) > 0;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "UpdateMap: {}", e.what());
		return false;
	}
}

bool CGameDB::UpdateMapAfterEnter(std::int32_t CaptainID, std::int32_t MapID) {
	try {
		return _amphiTeams.Execute(
			"UPDATE AmphitheaterTeam SET map = ? WHERE captain = ?",
			MapID, CaptainID) > 0;
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

bool CGameDB::UpdatReliveNum(std::int32_t ReID) {
	try {
		auto row = _amphiTeams.FindOne("id = ?", ReID);
		if (!row) {
			return false;
		}
		return _amphiTeams.Execute(
			"UPDATE AmphitheaterTeam SET relivenum = ? WHERE id = ?",
			row->relivenum + 1, ReID) > 0;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "UpdatReliveNum: {}", e.what());
		return false;
	}
}

bool CGameDB::UpdateAbsentTeamRelive() {
	try {
		return _amphiTeams.Execute(
			"UPDATE AmphitheaterTeam SET state = ? WHERE state = ?",
			static_cast<int>(AmphitheaterTeam::enumRelive),
			static_cast<int>(AmphitheaterTeam::enumUse)) > 0;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "UpdateAbsentTeamRelive: {}", e.what());
		return false;
	}
}

bool CGameDB::UpdateWinnum(std::int32_t teamid) {
	try {
		return _amphiTeams.Execute(
			"UPDATE AmphitheaterTeam SET winnum = winnum + 1 WHERE id = ?",
			teamid) > 0;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "UpdateWinnum: {}", e.what());
		return false;
	}
}

bool CGameDB::GetUniqueMaxWinnum(std::int32_t& teamid) {
	try {
		auto rows = _amphiTeams.FindAll(
			"winnum IN (SELECT MAX(winnum) FROM AmphitheaterTeam)");
		if (rows.size() != 1) {
			return false;
		}
		teamid = rows[0].id;
		return true;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "GetUniqueMaxWinnum: {}", e.what());
		return false;
	}
}

bool CGameDB::SetMatchnoState(std::int32_t teamid) {
	try {
		return _amphiTeams.Execute(
			"UPDATE AmphitheaterTeam SET matchno = 1 WHERE id = ?",
			teamid) > 0;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "SetMatchnoState: {}", e.what());
		return false;
	}
}

bool CGameDB::UpdateState() {
	try {
		return _amphiTeams.Execute(
			"UPDATE AmphitheaterTeam SET state = ? WHERE state = ?",
			static_cast<int>(AmphitheaterTeam::enumUse),
			static_cast<int>(AmphitheaterTeam::enumPromotion)) > 0;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "UpdateState: {}", e.what());
		return false;
	}
}

bool CGameDB::CloseReliveByState(std::int32_t& statenum) {
	try {
		auto rows = _amphiTeams.FindAll("state = ?", static_cast<int>(AmphitheaterTeam::enumUse));
		statenum = static_cast<int>(rows.size());
		return true;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "CloseReliveByState: {}", e.what());
		return false;
	}
}

bool CGameDB::CleanMapFlag(std::int32_t teamid1, std::int32_t teamid2) {
	try {
		return _amphiTeams.Execute(
			"UPDATE AmphitheaterTeam SET mapflag = null WHERE id = ? OR id = ?",
			teamid1, teamid2) > 0;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "CleanMapFlag: {}", e.what());
		return false;
	}
}

bool CGameDB::GetStateByTeamid(std::int32_t teamid, std::int32_t& state) {
	try {
		auto row = _amphiTeams.FindOne("id = ?", teamid);
		if (!row) {
			return false;
		}
		state = row->state;
		return true;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "GetStateByTeamid: {}", e.what());
		return false;
	}
}

bool CGameDB::UpdateIMP(CPlayer& ply) {
	try {
		return _characters.Execute(
			"UPDATE character SET IMP = ? WHERE atorID = ?",
			ply.GetMainCha()->GetIMP(),
			ply.GetMainCha()->GetID()) > 0;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "UpdateIMP: {}", e.what());
		return false;
	}
}

bool CGameDB::SaveGmLv(CPlayer& ply) {
	try {
		return _accounts.Execute(
			"UPDATE account SET jmes = ? WHERE ato_id = ?",
			ply.GetGMLev(),
			ply.GetDBActId()) > 0;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "SaveGmLv: {}", e.what());
		return false;
	}
}

std::uint32_t CGameDB::GetPlayerMasterDBID(CPlayer& pPlayer) {
	if (!pPlayer.GetMainCha()) {
		return 0;
	}
	try {
		auto row = _masters.FindOne("cha_id1 = ?", static_cast<int>(pPlayer.GetDBChaId()));
		return row ? static_cast<unsigned long>(row->cha_id2) : 0;
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "GetPlayerMasterDBID: {}", e.what());
		return 0;
	}
}

bool CGameDB::CreatePlyBank(CPlayer& pCPly) {
	if (pCPly.GetCurBankNum() >= MAX_BANK_NUM) {
		return false;
	}
	try {
		_resources.Execute(
			"INSERT INTO Resource (atorID, type_id) VALUES (?, ?)",
			static_cast<int>(pCPly.GetDBChaId()), static_cast<int>(enumRESDB_TYPE_BANK));
		auto idStr = _db.CreateCommand("SELECT @@IDENTITY").ExecuteScalar();
		long lBankDBID = std::stol(idStr);
		pCPly.AddBankDBID(lBankDBID);
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

bool CGameDB::SavePlyBank(CPlayer& pCPly, char chBankNO) {
	return _tab_res.SaveBankData(pCPly, chBankNO);
}

bool CGameDB::ReadKitbagTmpData(std::uint32_t res_id, std::string& strData) {
	if (res_id == 0) {
		return false;
	}
	try {
		auto row = _resources.FindOne("id = ?", static_cast<int>(res_id));
		strData = row ? row->content : "";
		return true;
	}
	catch (const OdbcException& e) {
		ToLogService("store", LogLevel::Error, "ReadKitbagTmpData: {}", e.what());
		return false;
	}
}

bool CGameDB::SaveKitbagTmpData(std::uint32_t res_id, const std::string& strData) {
	if (res_id == 0) {
		return false;
	}
	try {
		int affected = _resources.Execute(
			"UPDATE Resource SET content = ? WHERE id = ?",
			std::string_view(strData), static_cast<int>(res_id));
		if (affected == 0) {
			ToLogService("store", "Database couldn't find temp kitbag resource {}!", res_id);
			return false;
		}
		return true;
	}
	catch (const OdbcException& e) {
		ToLogService("store", LogLevel::Error, "SaveKitbagTmpData: {}", e.what());
		return false;
	}
}

bool CGameDB::StartChall(std::uint8_t byLevel) {
	for (int i = 0; i < 100; i++) {
		if (_tab_gld.StartChall(byLevel)) {
			return true;
		}
	}
	return false;
}

bool CGameDB::GetChall(std::uint8_t byLevel, std::uint32_t& dwGuildID1, std::uint32_t& dwGuildID2, std::uint32_t& dwMoney) {
	for (int i = 0; i < 100; i++) {
		if (_tab_gld.GetChallInfo(byLevel, dwGuildID1, dwGuildID2, dwMoney)) {
			return true;
		}
	}
	return false;
}

void CGameDB::ExecLogSQL(const std::string& pszSQL) {
	try {
		_db.CreateCommand(pszSQL).ExecuteNonQuery();
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "ExecLogSQL failed: {}", e.what());
	}
}

void CGameDB::ExecTradeLogSQL(const std::string& gameServerName, const std::string& action,
							 const std::string& pszChaFrom, const std::string& pszChaTo, const std::string& pszTrade) {
	try {
		SYSTEMTIME st;
		GetLocalTime(&st);
		auto timeStr = std::format("{:04}/{:02}/{:02} {:02}:{:02}:{:02}",
								   st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
		_tradeLogs.Execute(
			"INSERT INTO Trade_Log (ExecuteTime, GameServer, [Action], [From], [To], Memo) "
			"VALUES (?, ?, ?, ?, ?, ?)",
			std::string_view(timeStr),
			std::string_view(gameServerName),
			std::string_view(action),
			std::string_view(pszChaFrom),
			std::string_view(pszChaTo),
			std::string_view(pszTrade));
	}
	catch (const OdbcException& e) {
		ToLogService("db", LogLevel::Error, "ExecTradeLogSQL failed: {}", e.what());
	}
}

// ============================================================================
// CGameDB — делегаторы к PlayerStorage
// ============================================================================

std::string CGameDB::GetChaNameByID(std::int32_t cha_id) {
	return _tab_cha.GetName(cha_id);
}

void CGameDB::ShowExpRank(CCharacter& pCha, std::int32_t top) {
	_tab_cha.ShowExpRank(pCha, top);
}

bool CGameDB::SavePlayerPos(CPlayer& pPlayer) {
	return _tab_cha.SavePos(pPlayer);
}

bool CGameDB::SavePlayerKBagDBID(CPlayer& pPlayer) {
	return _tab_cha.SaveKBagDBID(pPlayer);
}

bool CGameDB::SavePlayerKBagTmpDBID(CPlayer& pPlayer) {
	return _tab_cha.SaveKBagTmpDBID(pPlayer);
}

bool CGameDB::SavePlayerMMaskDBID(CPlayer& pPlayer) {
	return _tab_cha.SaveMMaskDBID(pPlayer);
}

bool CGameDB::AddCreditByDBID(std::uint32_t atorID, std::int32_t lCredit) {
	return _tab_cha.AddCreditByDBID(atorID, lCredit);
}

bool CGameDB::SaveStoreItemID(std::uint32_t atorID, std::int32_t lStoreItemID) {
	return _tab_cha.SaveStoreItemID(atorID, lStoreItemID);
}

bool CGameDB::AddMoney(std::uint32_t atorID, std::int32_t money) {
	return _tab_cha.AddMoney(atorID, money);
}

bool CGameDB::IsChaOnline(std::uint32_t atorID, bool& bOnline) {
	return _tab_cha.IsChaOnline(atorID, bOnline);
}

Long CGameDB::GetChaAddr(std::uint32_t atorID) {
	return _tab_cha.GetChaAddr(atorID);
}

Long CGameDB::SetGuildPermission(std::int32_t atorID, std::uint32_t perm, std::int32_t guild_id) {
	return _tab_cha.SetGuildPermission(atorID, perm, guild_id);
}

Long CGameDB::SetChaAddr(std::uint32_t atorID, Long addr) {
	return _tab_cha.SetChaAddr(atorID, addr);
}

bool CGameDB::SaveMissionData(CPlayer& pPlayer, std::uint32_t atorID) {
	return _tab_cha.SaveMissionData(pPlayer, atorID);
}

// ============================================================================
// CGameDB — делегаторы к CTableBoat
// ============================================================================

bool CGameDB::Create(std::uint32_t& dwBoatID, const BOAT_DATA& Data) {
	return _tab_boat.Create(dwBoatID, Data);
}

bool CGameDB::GetBoat(CCharacter& Boat) {
	return _tab_boat.GetBoat(Boat);
}

bool CGameDB::SaveBoat(CCharacter& Boat, char chSaveType) {
	return _tab_boat.SaveBoat(Boat, chSaveType);
}

bool CGameDB::SaveBoatDelTag(std::uint32_t dwBoatID, std::uint8_t byIsDeleted) {
	return _tab_boat.SaveBoatDelTag(dwBoatID, byIsDeleted);
}

bool CGameDB::SaveBoatTempData(CCharacter& Boat, std::uint8_t byIsDeleted) {
	return _tab_boat.SaveBoatTempData(Boat, byIsDeleted);
}

bool CGameDB::SaveBoatTempData(std::uint32_t dwBoatID, std::uint32_t dwOwnerID, std::uint8_t byIsDeleted) {
	return _tab_boat.SaveBoatTempData(dwBoatID, dwOwnerID, byIsDeleted);
}

// ============================================================================
// CGameDB — делегаторы к CTableGuild
// ============================================================================

std::int32_t CGameDB::CreateGuild(CCharacter& pCha, const std::string& guildname, const std::string& passwd) {
	return _tab_gld.Create(pCha, guildname, passwd);
}

std::int32_t CGameDB::GetGuildBank(std::uint32_t guildid, CKitbag* bag) {
	return _tab_gld.GetGuildBank(guildid, bag);
}

std::int32_t CGameDB::UpdateGuildBank(std::uint32_t guildid, CKitbag* bag) {
	return _tab_gld.UpdateGuildBank(guildid, bag);
}

bool CGameDB::SetGuildLog(std::vector<CTableGuild::BankLog> log, std::uint32_t guildid) {
	return _tab_gld.SetGuildLog(log, guildid);
}

std::vector<CTableGuild::BankLog> CGameDB::GetGuildLog(std::uint32_t guildid) {
	return _tab_gld.GetGuildLog(guildid);
}

std::int64_t CGameDB::GetGuildBankGold(std::uint32_t guildid) {
	return _tab_gld.GetGuildBankGold(guildid);
}

bool CGameDB::UpdateGuildBankGold(std::int32_t guildID, std::int32_t money) {
	return _tab_gld.UpdateGuildBankGold(guildID, money);
}

std::int32_t CGameDB::GetGuildLeaderID(std::uint32_t guildid) {
	return _tab_gld.GetGuildLeaderID(guildid);
}

bool CGameDB::ListAllGuild(CCharacter& pCha, char disband_days) {
	return _tab_gld.ListAll(pCha, disband_days);
}

void CGameDB::GuildTryFor(CCharacter& pCha, std::uint32_t guildid) {
	_tab_gld.TryFor(pCha, guildid);
}

void CGameDB::GuildTryForConfirm(CCharacter& pCha, std::uint32_t guildid) {
	_tab_gld.TryForConfirm(pCha, guildid);
}

bool CGameDB::GuildListTryPlayer(CCharacter& pCha, char disband_days) {
	return _tab_gld.ListTryPlayer(pCha, disband_days);
}

bool CGameDB::GuildApprove(CCharacter& pCha, std::uint32_t chaid) {
	return _tab_gld.Approve(pCha, chaid);
}

bool CGameDB::GuildReject(CCharacter& pCha, std::uint32_t chaid) {
	return _tab_gld.Reject(pCha, chaid);
}

bool CGameDB::GuildKick(CCharacter& pCha, std::uint32_t chaid) {
	return _tab_gld.Kick(pCha, chaid);
}

bool CGameDB::GuildLeave(CCharacter& pCha) {
	return _tab_gld.Leave(pCha);
}

bool CGameDB::GuildDisband(CCharacter& pCha, const std::string& passwd) {
	return _tab_gld.Disband(pCha, passwd);
}

bool CGameDB::GuildMotto(CCharacter& pCha, const std::string& motto) {
	return _tab_gld.Motto(pCha, motto);
}

CTableMapMask* CGameDB::GetMapMaskTable() {
	return &_tab_mmask;
}

bool CGameDB::GetGuildName(std::int32_t lGuildID, std::string& strGuildName) {
	return _tab_gld.GetGuildName(lGuildID, strGuildName);
}

bool CGameDB::Challenge(CCharacter& pCha, std::uint8_t byLevel, std::uint32_t dwMoney) {
	return _tab_gld.Challenge(pCha, byLevel, dwMoney);
}

bool CGameDB::Leizhu(CCharacter& pCha, std::uint8_t byLevel, std::uint32_t dwMoney) {
	return _tab_gld.Leizhu(pCha, byLevel, dwMoney);
}

void CGameDB::ListChallenge(CCharacter& pCha) {
	_tab_gld.ListChallenge(pCha);
}

void CGameDB::EndChall(std::uint32_t dwGuild1, std::uint32_t dwGuild2, bool bChall) {
	_tab_gld.EndChall(dwGuild1, dwGuild2, bChall);
}

bool CGameDB::HasChall(std::uint8_t byLevel) {
	return _tab_gld.HasCall(byLevel);
}

bool CGameDB::HasGuildLevel(CCharacter& pChar, std::uint8_t byLevel) {
	return _tab_gld.HasGuildLevel(pChar, byLevel);
}
