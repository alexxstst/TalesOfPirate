#include <PoseDataStore.h>
#include <sstream>
#include <chrono>

void PoseDataStore::EnsureCreated(SqliteDatabase& db) {
	try {
		auto stmt = db.Prepare("SELECT 1 FROM sqlite_master WHERE type='table' AND name=?");
		stmt.Bind(1, std::string_view(TABLE_NAME));
		if (!stmt.Step()) {
			db.Execute(CREATE_TABLE_SQL);
			ToLogService("common", "PoseDataStore: created table '{}'", TABLE_NAME);
		}
	} catch (const SqliteException& e) {
		ToLogService("errors", LogLevel::Error, "PoseDataStore::EnsureCreated failed: {}", e.what());
	}
}

bool PoseDataStore::Load(SqliteDatabase& db) {
	try {
		auto start = std::chrono::high_resolution_clock::now();

		EnsureCreated(db);
		auto stmt = db.Prepare(SELECT_ALL_SQL);

		_records.clear();
		_charTypes.clear();
		_maxCharType = 0;

		while (stmt.Step()) {
			PoseDataRecord rec;
			rec.characterType = stmt.GetInt(0);
			rec.actionId      = stmt.GetInt(1);
			rec.startFrame    = stmt.GetInt(2);
			rec.endFrame      = stmt.GetInt(3);
			rec.keyframeCount = 0;
			memset(rec.keyframes, 0, sizeof(rec.keyframes));

			// Парсинг keyframes: "f0,f1,f2,..."
			std::string kfText(stmt.GetText(4));
			if (!kfText.empty()) {
				std::istringstream ss(kfText);
				std::string token;
				while (std::getline(ss, token, ',') && rec.keyframeCount < 8) {
					if (!token.empty()) {
						rec.keyframes[rec.keyframeCount++] = std::stoi(token);
					}
				}
			}

			if (rec.characterType > _maxCharType) {
				_maxCharType = rec.characterType;
			}
			_records.push_back(rec);
		}

		if (_records.empty()) {
			ToLogService("common", "PoseDataStore: no data loaded");
			return true;
		}

		// Построить индекс по character_type
		_charTypes.resize(_maxCharType);

		// Данные отсортированы по character_type, action_id — считаем offset/count
		int currentType = 0;
		int currentOffset = 0;
		for (int i = 0; i < static_cast<int>(_records.size()); i++) {
			int ct = _records[i].characterType;
			if (ct != currentType) {
				currentType = ct;
				currentOffset = i;
			}
			auto& idx = _charTypes[ct - 1];
			if (idx.count == 0) {
				idx.offset = currentOffset;
			}
			idx.count++;
			if (_records[i].actionId > idx.maxActionId) {
				idx.maxActionId = _records[i].actionId;
			}
		}

		auto elapsed = std::chrono::high_resolution_clock::now() - start;
		auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
		ToLogService("common", "PoseDataStore loaded {} records, {} char types in {} ms",
			_records.size(), _maxCharType, ms);

		return true;
	} catch (const SqliteException& e) {
		ToLogService("errors", LogLevel::Error, "PoseDataStore::Load failed: {}", e.what());
		return false;
	}
}

const PoseDataRecord* PoseDataStore::GetActions(int charType, int* outCount) const {
	if (charType < 1 || charType > _maxCharType) {
		if (outCount) {
			*outCount = 0;
		}
		return nullptr;
	}

	const auto& idx = _charTypes[charType - 1];
	if (idx.count == 0) {
		if (outCount) {
			*outCount = 0;
		}
		return nullptr;
	}

	if (outCount) {
		*outCount = idx.count;
	}
	return &_records[idx.offset];
}

const PoseDataRecord* PoseDataStore::GetAction(int charType, int actionId) const {
	if (charType < 1 || charType > _maxCharType) {
		return nullptr;
	}

	const auto& idx = _charTypes[charType - 1];
	if (idx.count == 0 || actionId < 1 || actionId > idx.maxActionId) {
		return nullptr;
	}

	// Линейный поиск в пределах charType (обычно до 54 записей)
	for (int i = idx.offset; i < idx.offset + idx.count; i++) {
		if (_records[i].actionId == actionId) {
			return &_records[i];
		}
	}
	return nullptr;
}

void PoseDataStore::Insert(SqliteDatabase& db, const PoseDataRecord& rec) {
	try {
		auto stmt = db.Prepare(
			"INSERT OR REPLACE INTO pose_data "
			"(character_type, action_id, start_frame, end_frame, keyframes) "
			"VALUES (?, ?, ?, ?, ?)");

		int p = 1;
		stmt.Bind(p++, rec.characterType);
		stmt.Bind(p++, rec.actionId);
		stmt.Bind(p++, rec.startFrame);
		stmt.Bind(p++, rec.endFrame);

		std::string kf;
		for (int k = 0; k < rec.keyframeCount && k < 8; k++) {
			if (k > 0) {
				kf += ',';
			}
			kf += std::to_string(rec.keyframes[k]);
		}
		stmt.Bind(p++, std::string_view(kf));

		stmt.Step();
	} catch (const std::exception& e) {
		ToLogService("errors", LogLevel::Error,
			"PoseDataStore::Insert(ct={}, act={}) failed: {}",
			rec.characterType, rec.actionId, e.what());
	}
}

void PoseDataStore::Clear(SqliteDatabase& db) {
	try {
		EnsureCreated(db);
		db.Execute("DELETE FROM pose_data");
	} catch (const SqliteException& e) {
		ToLogService("errors", LogLevel::Error, "PoseDataStore::Clear failed: {}", e.what());
	}
}
