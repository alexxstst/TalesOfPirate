#include "stdafx.h"
#include ".\chatrecord.h"

using namespace std;

string CChatRecord::m_strPath = "";


CChatRecord::CChatRecord(void) {
}

CChatRecord::~CChatRecord(void) {
}

bool CChatRecord::Save(const string name, DWORD number, const string chatData) {
	if (name.length() == 0 || chatData.length() == 0) return false;
	__time64_t t;
	_time64(&t);
	tm* ltime = _gmtime64(&t);
	const std::string folder = std::format("chats\\{}-{}-{}", ltime->tm_year + 1900, ltime->tm_mon + 1, ltime->tm_mday);
	CreateDirectory("chats", NULL);
	CreateDirectory(folder.c_str(), NULL);
	ofstream chatLog;
	m_strPath = folder + std::format("\\{}.txt", name);
	chatLog.open(m_strPath.c_str(), ios_base::app); // 
	// End
	chatLog << chatData.c_str();
	chatLog.close();
	return true;
}

string CChatRecord::GetLastSavePath() {
	return m_strPath;
}
