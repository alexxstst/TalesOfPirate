#pragma once

#include "GameRecordset.h"
#include "SkillStateRecord.h"

// Хранилище состояний навыков на базе SQLite.
class SkillStateRecordStore : public GameRecordset<CSkillStateRecord> {
public:
	static SkillStateRecordStore* Instance() {
		static SkillStateRecordStore instance{};
		return &instance;
	}

	static constexpr const char* TABLE_NAME = "skill_states";

	static constexpr const char* CREATE_TABLE_SQL = R"(
		CREATE TABLE IF NOT EXISTS skill_states (
			id              INTEGER PRIMARY KEY,
			ch_id           INTEGER,
			name            TEXT,
			frequency       INTEGER,
			on_transfer     TEXT,
			add_state       TEXT,
			sub_state       TEXT,
			add_type        INTEGER,
			can_cancel      INTEGER,
			can_move        INTEGER,
			can_mskill      INTEGER,
			can_gskill      INTEGER,
			can_trade       INTEGER,
			can_item        INTEGER,
			can_unbeatable  INTEGER,
			can_itemmed     INTEGER,
			can_skilled     INTEGER,
			no_hide         INTEGER,
			no_show         INTEGER,
			opt_item        INTEGER,
			talk_to_npc     INTEGER,
			free_state_id   INTEGER,
			screen          INTEGER,
			act_behave      TEXT,
			charge_link     INTEGER,
			area_effect     INTEGER,
			is_show_center  INTEGER,
			is_dizzy        INTEGER,
			effect          INTEGER,
			dummy1          INTEGER,
			bit_effect      INTEGER,
			dummy2          INTEGER,
			icon_id         INTEGER,
			icons           TEXT,
			descriptor      TEXT,
			colour          INTEGER
		)
	)";

	static constexpr const char* SELECT_ALL_SQL =
		"SELECT * FROM skill_states ORDER BY id";

	bool Load(SqliteDatabase& db) {
		EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
		return GameRecordset::Load(db, SELECT_ALL_SQL);
	}

protected:
	RecordEntry ReadRecord(SqliteStatement& stmt) override;
};
