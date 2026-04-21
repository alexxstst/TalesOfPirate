#include <LifeLvRecordStore.h>

GameRecordset<CLifeLvRecord>::RecordEntry LifeLvRecordStore::ReadRecord(SqliteStatement& stmt) {
	CLifeLvRecord record{};
	int col = 0;

	record.lID    = stmt.GetInt(col++);
	record.Id    = static_cast<int>(record.lID);
	record.sLevel = static_cast<short>(stmt.GetInt(col++));
	record.ulExp  = static_cast<unsigned long>(stmt.GetInt64(col++));

	return {record.Id, {}, std::move(record)};
}

CLifeLvRecord* GetLifeLvRecordInfo(int nTypeID, const std::source_location& loc) {
	return LifeLvRecordStore::Instance()->Get(nTypeID, loc);
}
