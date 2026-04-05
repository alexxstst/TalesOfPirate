#pragma once

#include "GameRecordset.h"
#include "ItemRecord.h"

// Хранилище предметов на базе SQLite.
class ItemRecordStore : public GameRecordset<CItemRecord> {
public:
	static ItemRecordStore* Instance() {
		static ItemRecordStore instance{};
		return &instance;
	}

	static constexpr const char* TABLE_NAME = "items";

	bool Load(SqliteDatabase& db) {
		EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
		return GameRecordset::Load(db, SELECT_ALL_SQL);
	}

	static constexpr const char* CREATE_TABLE_SQL = R"(
		CREATE TABLE IF NOT EXISTS items (
			id              INTEGER PRIMARY KEY,
			name            TEXT NOT NULL,
			icon            TEXT,
			module_0 TEXT, module_1 TEXT, module_2 TEXT, module_3 TEXT, module_4 TEXT,
			ship_flag       INTEGER,
			ship_type       INTEGER,
			type            INTEGER,
			forge_lv        INTEGER,
			forge_steady    INTEGER,
			exclusive_id    INTEGER,
			is_trade        INTEGER,
			is_pick         INTEGER,
			is_throw        INTEGER,
			is_del          INTEGER,
			price           INTEGER,
			body            TEXT,
			need_lv         INTEGER,
			work            TEXT,
			pile_max        INTEGER,
			instance        INTEGER,
			able_link       TEXT,
			need_link       TEXT,
			pick_to         INTEGER,

			str_coef INTEGER, agi_coef INTEGER, dex_coef INTEGER, con_coef INTEGER,
			sta_coef INTEGER, luk_coef INTEGER, aspd_coef INTEGER, adis_coef INTEGER,
			mn_atk_coef INTEGER, mx_atk_coef INTEGER, def_coef INTEGER,
			mx_hp_coef INTEGER, mx_sp_coef INTEGER, flee_coef INTEGER, hit_coef INTEGER,
			crt_coef INTEGER, mf_coef INTEGER, hrec_coef INTEGER, srec_coef INTEGER,
			mspd_coef INTEGER, col_coef INTEGER,

			str_valu TEXT, agi_valu TEXT, dex_valu TEXT, con_valu TEXT,
			sta_valu TEXT, luk_valu TEXT, aspd_valu TEXT, adis_valu TEXT,
			mn_atk_valu TEXT, mx_atk_valu TEXT, def_valu TEXT,
			mx_hp_valu TEXT, mx_sp_valu TEXT, flee_valu TEXT, hit_valu TEXT,
			crt_valu TEXT, mf_valu TEXT, hrec_valu TEXT, srec_valu TEXT,
			mspd_valu TEXT, col_valu TEXT,

			p_def           TEXT,
			l_hand_valu     INTEGER,
			endure          TEXT,
			energy          TEXT,
			hole            INTEGER,
			attr_effect     TEXT,
			drap            INTEGER,
			effects         TEXT,
			effects_dummy   TEXT,
			item_effect     TEXT,
			area_effect     TEXT,
			use_item_effect TEXT,
			descriptor      TEXT,
			cooldown        REAL
		)
	)";

	static constexpr const char* SELECT_ALL_SQL =
		"SELECT * FROM items ORDER BY id";

protected:
	RecordEntry ReadRecord(SqliteStatement& stmt) override;

private:
	static void ParsePair(std::string_view text, short& out0, short& out1);
	static void ParseCharArray(std::string_view text, std::int8_t* out, int maxLen, std::int8_t defaultVal);
	static void ParseShortArray(std::string_view text, short* out, int maxLen);
};
