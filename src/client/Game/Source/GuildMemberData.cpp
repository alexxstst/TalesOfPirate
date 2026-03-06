//------------------------------------------------------------------------
//	2005.4.25	Arcol	create this file
//------------------------------------------------------------------------

#include "stdafx.h"
#include ".\guildmemberdata.h"

CGuildMemberData::CGuildMemberData(void)
{
	m_dwID=0;
	m_strName="";
	m_strMottoName="";
	m_dwIcon=0;
	m_strJob="";
	m_dwLv=0;
	m_bOnline=false;
	m_bMgr=false;
	m_pPointer=NULL;
}

CGuildMemberData::~CGuildMemberData(void)
{
}
