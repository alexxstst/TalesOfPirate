#pragma once

class CGuildMembersMgr {
public:
	CGuildMembersMgr(void);
	~CGuildMembersMgr(void);
	static void AddGuildMember(CGuildMemberData* pGuildMember);
	static bool DelGuildMember(CGuildMemberData* pGuildMember);
	static bool DelGuildMemberByID(DWORD dwID);
	static bool DelGuildMemberByName(std::string strName);
	static CGuildMemberData* GetSelfData();
	static CGuildMemberData* FindGuildMemberByID(DWORD dwID);
	static CGuildMemberData* FindGuildMemberByName(std::string strName);
	static CGuildMemberData* FindGuildMemberByIndex(DWORD dwIndex);
	static DWORD GetTotalGuildMembers();
	static void ResetAll();

private:
	static std::vector<CGuildMemberData*> m_pGuildMembers;
};
