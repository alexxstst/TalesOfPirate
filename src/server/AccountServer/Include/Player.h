#pragma once

#include "map"
#include <string>
//#include "atltime.h"

class CPlayer
{
public:
	CPlayer(void);
	~CPlayer(void);
	void Initial();
	void AddPlayer(long nTask,std::string strPlayerName);
	std::string GetPlayer(long nTask);
	void RemovePlayer(long nTask);

private:
	typedef std::map<long,std::string> StringMap;
	StringMap m_mapPlayers;
};
