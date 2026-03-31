#pragma once

#include "GameRecordset.h"
#include "SkillRecord.h"

// Хранилище навыков на базе SQLite.
class SkillRecordStore : public GameRecordset<CSkillRecord> {
public:
	static SkillRecordStore* Instance() {
		static SkillRecordStore instance{};
		return &instance;
	}

	static constexpr const char* TABLE_NAME = "skills";

	static constexpr const char* CREATE_TABLE_SQL = R"(
		CREATE TABLE IF NOT EXISTS skills (
			id                  INTEGER PRIMARY KEY,
			name                TEXT NOT NULL,
			fight_type          INTEGER,
			job_select          TEXT,
			item_need_1         TEXT,
			item_need_2         TEXT,
			item_need_3         TEXT,
			conch_need          TEXT,
			phase               INTEGER,
			type                INTEGER,
			helpful             INTEGER,
			level_demand        INTEGER,
			premiss_skill       TEXT,
			point_expend        INTEGER,
			src_type            INTEGER,
			tar_type            INTEGER,
			apply_distance      INTEGER,
			apply_target        INTEGER,
			apply_type          INTEGER,
			angle               INTEGER,
			radii               INTEGER,
			range_val           INTEGER,
			prepare             TEXT,
			range_state         TEXT,
			use_sp              TEXT,
			use_endure          TEXT,
			use_energy          TEXT,
			set_range           TEXT,
			use_script          TEXT,
			effect              TEXT,
			active              TEXT,
			inactive            TEXT,
			state_id            INTEGER,
			splash_para         INTEGER,
			target_effect       INTEGER,
			splash_effect       INTEGER,
			variation           INTEGER,
			summon              INTEGER,
			fire_speed          TEXT,
			action_harm         INTEGER,
			action_play_type    INTEGER,
			action_pose         TEXT,
			action_key_frame    INTEGER,
			whop                INTEGER,
			action_dummy_link   TEXT,
			action_effect       TEXT,
			action_effect_type  TEXT,
			item_dummy_link     INTEGER,
			item_effect_1       TEXT,
			item_effect_2       TEXT,
			sky_eff_key_frame   INTEGER,
			sky_eff_dummy_link  INTEGER,
			sky_eff_item_dummy  INTEGER,
			sky_effect          INTEGER,
			sky_spd             INTEGER,
			whoped              INTEGER,
			target_dummy_link   INTEGER,
			target_effect_id    INTEGER,
			target_effect_time  INTEGER,
			aground_effect_id   INTEGER,
			water_effect_id     INTEGER,
			icon                TEXT,
			play_time           INTEGER,
			operate             TEXT,
			describe_hint       TEXT,
			effect_hint         TEXT,
			expend_hint         TEXT
		)
	)";

	static constexpr const char* SELECT_ALL_SQL =
		"SELECT * FROM skills ORDER BY id";

	bool Load(SqliteDatabase& db) {
		EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
		return GameRecordset::Load(db, SELECT_ALL_SQL);
	}

protected:
	RecordEntry ReadRecord(SqliteStatement& stmt) override;

private:
	// --- Вспомогательные функции сериализации ---

	// short[] -> "v1,v2,v3"
	static std::string SerializeShortArray(const short* arr, int count) {
		std::string result;
		for (int i = 0; i < count; i++) {
			if (i > 0) result += ',';
			result += std::to_string(arr[i]);
		}
		return result;
	}

	// char[] -> "v1,v2,v3"
	static std::string SerializeCharArray(const char* arr, int count) {
		std::string result;
		for (int i = 0; i < count; i++) {
			if (i > 0) result += ',';
			result += std::to_string(static_cast<int>(arr[i]));
		}
		return result;
	}

	// char[][2] -> "a,b;a,b;..." (до sentinel)
	static std::string SerializeCharPairs(const char* arr, int rows, int cols, char sentinel) {
		std::string result;
		for (int i = 0; i < rows; i++) {
			char v0 = arr[i * cols + 0];
			char v1 = arr[i * cols + 1];
			if (v0 == sentinel) break;
			if (i > 0) result += ';';
			result += std::to_string(static_cast<int>(v0));
			result += ',';
			result += std::to_string(static_cast<int>(v1));
		}
		return result;
	}

	// short[][2] -> "a,b;a,b;..." (до sentinel)
	static std::string SerializeShortPairs(const short* arr, int rows, int cols, short sentinel) {
		std::string result;
		for (int i = 0; i < rows; i++) {
			short v0 = arr[i * cols + 0];
			short v1 = arr[i * cols + 1];
			if (v0 == sentinel) break;
			if (i > 0) result += ';';
			result += std::to_string(v0);
			result += ',';
			result += std::to_string(v1);
		}
		return result;
	}

	// short[][3] -> "a,b,c;a,b,c;..." (до sentinel)
	static std::string SerializeShortTriples(const short* arr, int rows, int cols, short sentinel) {
		std::string result;
		for (int i = 0; i < rows; i++) {
			short v0 = arr[i * cols + 0];
			short v1 = arr[i * cols + 1];
			short v2 = arr[i * cols + 2];
			if (v0 == sentinel) break;
			if (i > 0) result += ';';
			result += std::to_string(v0);
			result += ',';
			result += std::to_string(v1);
			result += ',';
			result += std::to_string(v2);
		}
		return result;
	}

	// --- Вспомогательные функции десериализации ---

	// "v1,v2,v3" -> short[]
	static void ParseShortArray(std::string_view text, short* out, int maxLen);

	// "v1,v2,v3" -> char[]
	static void ParseCharArray(std::string_view text, char* out, int maxLen);

	// "a,b;a,b;..." -> char[][2] (заполняет sentinel по умолчанию)
	static void ParseCharPairs(std::string_view text, char* out, int rows, int cols, char sentinel);

	// "a,b;a,b;..." -> short[][2] (заполняет sentinel по умолчанию)
	static void ParseShortPairs(std::string_view text, short* out, int rows, int cols, short sentinel);

	// "a,b,c;a,b,c;..." -> short[][3] (заполняет sentinel по умолчанию)
	static void ParseShortTriples(std::string_view text, short* out, int rows, int cols, short sentinel);
};
