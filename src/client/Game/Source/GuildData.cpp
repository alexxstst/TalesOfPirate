//------------------------------------------------------------------------
//	2005.4.25	Arcol	create this file
//------------------------------------------------------------------------

#include "stdafx.h"
#include ".\guilddata.h"


DWORD	CGuildData::m_dwID=0;
std::string	CGuildData::m_strName="";
std::string	CGuildData::m_strMottoName="";
DWORD	CGuildData::m_dwLv=0;
DWORD	CGuildData::m_dwMasterID=0;
std::string	CGuildData::m_strMasterName="";
DWORD	CGuildData::m_dwMemberCount=0;
DWORD	CGuildData::m_dwMaxMembers=0;
CGuildData::eState	CGuildData::m_eState=CGuildData::normal;
__int64	CGuildData::m_i64Exp=0;
__int64	CGuildData::m_i64RemainTime=0;

CGuildData::CGuildData(void)
{
}

CGuildData::~CGuildData(void)
{
}

void CGuildData::LoadData(sGuildData data)
{
	m_dwID=data.guild_id;
	m_strName=data.guild_name;
	m_strMottoName=data.guild_motto_name;
	m_strMasterName=data.guild_master_name;
	m_i64Exp=data.guild_experience;
	m_dwMemberCount=data.guild_member_count;
	m_dwMaxMembers=data.guild_member_max;
	m_eState=data.guild_state;
	m_i64RemainTime=data.guild_remain_time;
	m_dwLv=1;
}

void CGuildData::Reset()
{
	m_dwID=0;
	m_strName="";
	m_strMottoName="";
	m_dwLv=0;
	m_dwMasterID=0;
	m_strMasterName="";
	m_dwMemberCount=0;
	m_dwMaxMembers=0;
	m_eState=CGuildData::normal;
	m_i64Exp=0;
	m_i64RemainTime=0;
}
