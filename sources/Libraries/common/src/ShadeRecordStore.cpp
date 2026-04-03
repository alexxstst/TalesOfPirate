#include <ShadeRecordStore.h>

GameRecordset<CShadeInfo>::RecordEntry ShadeRecordStore::ReadRecord(SqliteStatement& stmt) {
	CShadeInfo record{};
	int col = 0;

	record.nID    = stmt.GetInt(col++);
	record.bExist = TRUE;

	{
		auto dn = stmt.GetText(col++);
		strncpy(record.szDataName, dn.data(), sizeof(record.szDataName) - 1);
		record.szDataName[sizeof(record.szDataName) - 1] = '\0';
	}
	{
		auto name = stmt.GetText(col++);
		strncpy(record.szName, name.data(), sizeof(record.szName) - 1);
		record.szName[sizeof(record.szName) - 1] = '\0';
	}

	record.fsize         = static_cast<float>(stmt.GetDouble(col++));
	record.nAni          = stmt.GetInt(col++);
	record.nRow          = stmt.GetInt(col++);
	record.nCol          = stmt.GetInt(col++);
	record.nUseAlphaTest = stmt.GetInt(col++);
	record.nAlphaType    = stmt.GetInt(col++);
	record.nColorR       = stmt.GetInt(col++);
	record.nColorG       = stmt.GetInt(col++);
	record.nColorB       = stmt.GetInt(col++);
	record.nColorA       = stmt.GetInt(col++);
	record.nType         = stmt.GetInt(col++);

	return {record.nID, std::string(record.szDataName), std::move(record)};
}

void ShadeRecordStore::Insert(SqliteDatabase& db, const CShadeInfo& r) {
	try {
		EnsureCreated(db, TABLE_NAME, CREATE_TABLE_SQL);
		auto stmt = db.Prepare(
			"INSERT OR REPLACE INTO shades "
			"(id,data_name,name,size,ani,row,col,use_alpha_test,alpha_type,color_r,color_g,color_b,color_a,type) "
			"VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
		int p = 1;
		stmt.Bind(p++, r.nID);
		stmt.Bind(p++, std::string_view(r.szDataName));
		stmt.Bind(p++, std::string_view(r.szName));
		stmt.Bind(p++, static_cast<double>(r.fsize));
		stmt.Bind(p++, r.nAni);
		stmt.Bind(p++, r.nRow);
		stmt.Bind(p++, r.nCol);
		stmt.Bind(p++, r.nUseAlphaTest);
		stmt.Bind(p++, r.nAlphaType);
		stmt.Bind(p++, r.nColorR);
		stmt.Bind(p++, r.nColorG);
		stmt.Bind(p++, r.nColorB);
		stmt.Bind(p++, r.nColorA);
		stmt.Bind(p++, r.nType);
		stmt.Step();
	} catch (const std::exception& e) {
		ToLogService("errors", LogLevel::Error, "ShadeRecordStore::Insert(id={}) failed: {}", r.nID, e.what());
	}
}

CShadeInfo* GetShadeInfo(int nTypeID, const std::source_location& loc) {
	return ShadeRecordStore::Instance()->Get(nTypeID, loc);
}
