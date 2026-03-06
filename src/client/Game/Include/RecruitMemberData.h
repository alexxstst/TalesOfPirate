#pragma once

class CRecruitMemberData
{
public:
	CRecruitMemberData(void);
	~CRecruitMemberData(void);
	void	SetID(DWORD dwID)	{ m_dwID=dwID; }
	DWORD	GetID()	{ return m_dwID;}
	void	SetName(std::string strName)	{ m_strName=strName; }
	std::string	GetName()	{ return m_strName; }
	void	SetJob(std::string strJob)	{ m_strJob=strJob; }
	std::string GetJob()	{ return m_strJob; }
	void SetLevel(DWORD	dwLv)	{ m_dwLv=dwLv; }
	DWORD GetLevel()	{ return m_dwLv; }

private:
	DWORD	m_dwID;
	std::string	m_strName;
	std::string	m_strJob;
	DWORD	m_dwLv;
};
