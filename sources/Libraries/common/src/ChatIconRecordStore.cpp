#include <ChatIconRecordStore.h>

GameRecordset<CChatIconInfo>::RecordEntry ChatIconRecordStore::ReadRecord(SqliteStatement& stmt) {
	CChatIconInfo record{};
	int col = 0;

	record.Id    = stmt.GetInt(col++);

	// name → szSmall и DataName
	{
		auto name = stmt.GetText(col++);
		strncpy(record.szSmall, name.data(), sizeof(record.szSmall) - 1);
		record.szSmall[sizeof(record.szSmall) - 1] = '\0';
		record.DataName = name;
	}

	record.nSmallX = stmt.GetInt(col++);
	record.nSmallY = stmt.GetInt(col++);

	{
		auto text = stmt.GetText(col++);
		strncpy(record.szSmallOff, text.data(), sizeof(record.szSmallOff) - 1);
		record.szSmallOff[sizeof(record.szSmallOff) - 1] = '\0';
	}

	record.nSmallOffX = stmt.GetInt(col++);
	record.nSmallOffY = stmt.GetInt(col++);

	{
		auto text = stmt.GetText(col++);
		strncpy(record.szBig, text.data(), sizeof(record.szBig) - 1);
		record.szBig[sizeof(record.szBig) - 1] = '\0';
	}

	record.nBigX = stmt.GetInt(col++);
	record.nBigY = stmt.GetInt(col++);

	{
		auto text = stmt.GetText(col++);
		strncpy(record.szHint, text.data(), sizeof(record.szHint) - 1);
		record.szHint[sizeof(record.szHint) - 1] = '\0';
	}

	std::string name(record.DataName);
	return {record.Id, std::move(name), std::move(record)};
}

void ChatIconRecordStore::Insert(SqliteDatabase& db, const CChatIconInfo& r) {
	try {
		EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
		auto stmt = db.Prepare(
			"INSERT OR REPLACE INTO chat_icons "
			"(id,name,small_x,small_y,small_off,small_off_x,small_off_y,big,big_x,big_y,hint) "
			"VALUES (?,?,?,?,?,?,?,?,?,?,?)");
		int p = 1;
		stmt.Bind(p++, r.Id);
		stmt.Bind(p++, std::string_view(r.szSmall));
		stmt.Bind(p++, r.nSmallX);
		stmt.Bind(p++, r.nSmallY);
		stmt.Bind(p++, std::string_view(r.szSmallOff));
		stmt.Bind(p++, r.nSmallOffX);
		stmt.Bind(p++, r.nSmallOffY);
		stmt.Bind(p++, std::string_view(r.szBig));
		stmt.Bind(p++, r.nBigX);
		stmt.Bind(p++, r.nBigY);
		stmt.Bind(p++, std::string_view(r.szHint));
		stmt.Step();
	} catch (const std::exception& e) {
		ToLogService("errors", LogLevel::Error, "ChatIconRecordStore::Insert(id={}) failed: {}", r.Id, e.what());
	}
}

CChatIconInfo* GetChatIconInfo(int nIconID, const std::source_location& loc) {
	return ChatIconRecordStore::Instance()->Get(nIconID, loc);
}
