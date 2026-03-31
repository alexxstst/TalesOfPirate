#pragma once

#include "GameRecordset.h"
#include "CharacterRecord.h"

#include <format>
#include <sstream>

// Хранилище таблицы персонажей (NPC/монстров/игроков) на базе SQLite.
class ChaRecordStore : public GameRecordset<CChaRecord> {
public:
	static ChaRecordStore* Instance() {
		static ChaRecordStore instance{};
		return &instance;
	}

	static constexpr const char* TABLE_NAME = "characters";

	static constexpr const char* CREATE_TABLE_SQL = R"(
		CREATE TABLE IF NOT EXISTS characters (
			id                INTEGER PRIMARY KEY,
			name              TEXT NOT NULL,
			icon_name         TEXT,
			modal_type        INTEGER,
			ctrl_type         INTEGER,
			model             INTEGER,
			suit_id           INTEGER,
			suit_num          INTEGER,
			skin_info         TEXT,
			feff_id           TEXT,
			eeff_id           INTEGER,
			effect_action_id  TEXT,
			shadow            INTEGER,
			action_id         INTEGER,
			diaphaneity       INTEGER,
			footfall          INTEGER,
			whoop             INTEGER,
			dirge             INTEGER,
			control_able      INTEGER,
			territory         INTEGER,
			sea_height        INTEGER,
			item_type         TEXT,
			lengh             REAL,
			width             REAL,
			height            REAL,
			radii             INTEGER,
			birth_behave      TEXT,
			died_behave       TEXT,
			born_eff          INTEGER,
			die_eff           INTEGER,
			dormancy          INTEGER,
			die_action        INTEGER,
			hp_effect         TEXT,
			is_face           INTEGER,
			is_cyclone        INTEGER,
			script            INTEGER,
			weapon            INTEGER,
			skill_ids         TEXT,
			skill_lvs         TEXT,
			item_ids          TEXT,
			item_counts       TEXT,
			max_show_item     INTEGER,
			all_show          REAL,
			prefix            INTEGER,
			task_item_ids     TEXT,
			task_item_counts  TEXT,
			ai_no             INTEGER,
			can_turn          INTEGER,
			vision            INTEGER,
			noise             INTEGER,
			get_exp           INTEGER,
			light             INTEGER,
			mobexp            INTEGER,
			lv                INTEGER,
			mx_hp             INTEGER,
			hp                INTEGER,
			mx_sp             INTEGER,
			sp                INTEGER,
			mn_atk            INTEGER,
			mx_atk            INTEGER,
			p_def             INTEGER,
			def               INTEGER,
			hit               INTEGER,
			flee              INTEGER,
			crt               INTEGER,
			mf                INTEGER,
			h_rec             INTEGER,
			s_rec             INTEGER,
			a_spd             INTEGER,
			a_dis             INTEGER,
			c_dis             INTEGER,
			m_spd             INTEGER,
			col               INTEGER,
			str               INTEGER,
			agi               INTEGER,
			dex               INTEGER,
			con               INTEGER,
			sta               INTEGER,
			luk               INTEGER,
			l_hand_val        INTEGER,
			guild             TEXT,
			title             TEXT,
			job               TEXT,
			c_exp             INTEGER,
			n_exp             INTEGER,
			fame              INTEGER,
			ap                INTEGER,
			tp                INTEGER,
			gd                INTEGER,
			spri              INTEGER,
			stor              INTEGER,
			mx_sail           INTEGER,
			sail              INTEGER,
			stasa             INTEGER,
			scsm              INTEGER,
			t_str             INTEGER,
			t_agi             INTEGER,
			t_dex             INTEGER,
			t_con             INTEGER,
			t_sta             INTEGER,
			t_luk             INTEGER,
			t_mx_hp           INTEGER,
			t_mx_sp           INTEGER,
			t_atk             INTEGER,
			t_def             INTEGER,
			t_hit             INTEGER,
			t_flee            INTEGER,
			t_mf              INTEGER,
			t_crt             INTEGER,
			t_h_rec           INTEGER,
			t_s_rec           INTEGER,
			t_a_spd           INTEGER,
			t_a_dis           INTEGER,
			t_spd             INTEGER,
			t_spri            INTEGER,
			t_scsm            INTEGER,
			scaling           TEXT
		)
	)";

	static constexpr const char* SELECT_ALL_SQL =
		"SELECT * FROM characters ORDER BY id";

	bool Load(SqliteDatabase& db) {
		EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
		return GameRecordset::Load(db, SELECT_ALL_SQL);
	}

protected:
	RecordEntry ReadRecord(SqliteStatement& stmt) override;

private:
	// Вспомогательные функции для парсинга TEXT-массивов
	static void ParseShortArray(std::string_view text, short* out, int maxLen, short defaultVal = 0);
	static void ParseLongArray(std::string_view text, long* out, int maxLen, long defaultVal = 0);
	static void ParseCharArray(std::string_view text, char* out, int maxLen);
	static void ParseIntArray(std::string_view text, int* out, int maxLen);
	static void ParseFloatArray(std::string_view text, float* out, int maxLen);

	// Сериализация массивов в строку через запятую
	template <typename T>
	static std::string JoinArray(const T* data, int count) {
		std::string result;
		for (int i = 0; i < count; i++) {
			if (i > 0) result += ',';
			if constexpr (std::is_floating_point_v<T>)
				result += std::format("{}", data[i]);
			else
				result += std::to_string(static_cast<long long>(data[i]));
		}
		return result;
	}
};
