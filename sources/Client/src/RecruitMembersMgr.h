#pragma once

class CRecruitMembersMgr {
public:
	CRecruitMembersMgr(void);
	~CRecruitMembersMgr(void);
	static void AddRecruitMember(CRecruitMemberData* pRecruitMember);
	static bool DelRecruitMember(CRecruitMemberData* pRecruitMember);
	static bool DelRecruitMemberByID(DWORD dwID);
	static bool DelRecruitMemberByName(std::string strName);
	static CRecruitMemberData* FindRecruitMemberByID(DWORD dwID);
	static CRecruitMemberData* FindRecruitMemberByName(std::string strName);
	static CRecruitMemberData* FindRecruitMemberByIndex(DWORD dwIndex);
	static DWORD GetTotalRecruitMembers();
	static void ResetAll();

private:
	static std::vector<CRecruitMemberData*> m_pRecruitMembers;
};
