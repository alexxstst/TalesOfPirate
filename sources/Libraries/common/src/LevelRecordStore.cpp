#include <LevelRecordStore.h>

GameRecordset<CLevelRecord>::RecordEntry LevelRecordStore::ReadRecord(SqliteStatement& stmt) {
	CLevelRecord record{};
	int col = 0;

	record.lID    = stmt.GetInt(col++);
	record.nID    = static_cast<int>(record.lID);
	record.bExist = TRUE;
	record.sLevel = static_cast<short>(stmt.GetInt(col++));
	record.ulExp  = static_cast<unsigned int>(stmt.GetInt64(col++));

	return {record.nID, {}, std::move(record)};
}
