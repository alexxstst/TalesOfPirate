#pragma once

class CChatRecord {
public:
	CChatRecord(void);
	~CChatRecord(void);
	static bool Save(const std::string name, DWORD number, const std::string chatData);
	static std::string GetLastSavePath();

private:
	static std::string m_strPath;
};
