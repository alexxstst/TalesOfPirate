#pragma once


#include "GuildData.h"

class CGuildListData;
class CGuildListMgr
{
public:
	CGuildListMgr(void);
	~CGuildListMgr(void);
	static void	AddGuild(CGuildListData* pGuildMember);
	static bool	DelGuild(CGuildListData* pGuildMember);
	static bool	DelGuildByID(DWORD dwID);
	static bool	DelGuildByName(std::string strName);
	static CGuildListData*	FindGuildByID(DWORD dwID);
	static CGuildListData*	FindGuildByName(std::string strName);
	static CGuildListData*	FindGuildByIndex(DWORD dwIndex);
	static void SortGuildsByName(bool bInc=true);
	static void SortGuildsByMemberCount(bool bInc=true);
	static void SortGuildsByExperience(bool bInc=true);
	static DWORD	GetTotalGuilds();
	static void	ResetAll();

private:
	static std::vector<CGuildListData*> m_pGuildList;
};
