#pragma once

#include "GameRecordset.h"
#include "AnimatedLightRecord.h"

// Хранилище ключевых кадров анимированных источников света (таблица animated_lights).
// Каждая строка = один ключевой кадр; группа записей с общим light_no описывает одну
// анимацию AnimCtrlLight. Потребитель: CWorldScene::_LoadAnimLight.
class AnimatedLightStore : public GameRecordset<CAnimatedLightInfo>
{
public:
	static AnimatedLightStore* Instance() {
		static AnimatedLightStore instance{};
		return &instance;
	}

	static constexpr const char* TABLE_NAME = "animated_lights";

	static constexpr const char* CREATE_TABLE_SQL = R"(
		CREATE TABLE IF NOT EXISTS animated_lights (
			light_no      INTEGER NOT NULL,
			key_no        INTEGER NOT NULL,
			frame_id      INTEGER NOT NULL,
			type          INTEGER NOT NULL,
			r             INTEGER NOT NULL,
			g             INTEGER NOT NULL,
			b             INTEGER NOT NULL,
			range_        REAL    NOT NULL,
			attenuation0  REAL    NOT NULL,
			attenuation1  REAL    NOT NULL,
			attenuation2  REAL    NOT NULL,
			PRIMARY KEY (light_no, key_no)
		)
	)";

	static constexpr const char* SELECT_ALL_SQL =
		"SELECT light_no, key_no, frame_id, type, r, g, b, "
		"range_, attenuation0, attenuation1, attenuation2 "
		"FROM animated_lights ORDER BY light_no, key_no";

	// Загрузить записи из SQLite.
	bool Load(SqliteDatabase& db);

	// Вставить одну запись (INSERT OR REPLACE).
	static void Insert(SqliteDatabase& db, const CAnimatedLightInfo& record);

	// Максимальный light_no среди загруженных записей (= число анимаций - 1). -1 если пусто.
	short GetMaxLightNo() const { return _maxLightNo; }

protected:
	RecordEntry ReadRecord(SqliteStatement& stmt) override;

private:
	short _maxLightNo{-1};
};
