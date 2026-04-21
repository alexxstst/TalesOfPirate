#include <LevelRecordStore.h>

GameRecordset<CLevelRecord>::RecordEntry LevelRecordStore::ReadRecord(SqliteStatement& stmt) {
	CLevelRecord record{};
	int col = 0;

	record.lID    = stmt.GetInt(col++);
	record.Id    = static_cast<int>(record.lID);
	record.sLevel = static_cast<short>(stmt.GetInt(col++));
	record.ulExp  = static_cast<unsigned int>(stmt.GetInt64(col++));

	return {record.Id, {}, std::move(record)};
}

CLevelRecord* GetLevelRecordInfo(int nTypeID, const std::source_location& loc) {
	return LevelRecordStore::Instance()->Get(nTypeID, loc);
}
