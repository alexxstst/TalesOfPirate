#pragma once

#include "GuildData.h"

class CGuildListData
{
public:
	CGuildListData(void);
	CGuildListData(DWORD dwID, std::string strName, std::string strMottoName, std::string strMasterName,DWORD dwMemberCount,__int64 i64Exp);
	~CGuildListData(void);
	void	SetGuildID(DWORD dwID)	{ m_dwID=dwID; }
	DWORD	GetGuildID()	{ return m_dwID; }
	void	SetGuildName(std::string strName)	{ m_strName=strName; }
	std::string	GetGuildName()	{ return m_strName; }
	void	SetGuildMottoName(std::string strMottoName)	{ m_strMottoName=strMottoName; }
	std::string	GetGuildMottoName()	{ return m_strMottoName; }
	void	SetGuildMasterName(std::string strMasterName)	{ m_strMasterName=strMasterName; }
	std::string	GetGuildMasterName()	{ return m_strMasterName; }
	void	SetExperence(__int64 i64Exp)	{ m_i64Exp=i64Exp; }
	__int64	GetExperence()	{ return m_i64Exp; }
	void	SetMembers(DWORD dwMemberCount)	{ m_dwMembers=dwMemberCount; }
	DWORD	GetMembers()	{ return m_dwMembers; }

private:
	DWORD	m_dwID;
	std::string	m_strName;
	std::string	m_strMottoName;
	std::string	m_strMasterName;
	DWORD	m_dwMembers;
	__int64	m_i64Exp;
};
