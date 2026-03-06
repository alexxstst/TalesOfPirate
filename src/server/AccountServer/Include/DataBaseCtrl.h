#pragma once

#include <map>
#include <string>

#include "atltime.h"

class CSQLDatabase;

//
class CDataBaseCtrl
{
public:
	CDataBaseCtrl(void);
	~CDataBaseCtrl(void);
	bool CreateObject();
	void ReleaseObject();
	bool KickUser(std::string strUserName);

    void SetExpScale(std::string strUserName, long time);

	bool InsertUser(std::string username, std::string password, std::string email);
	bool UpdatePassword(std::string username, std::string password);

	bool UserLogin(int nUserID, std::string strUserName, std::string strIP);
	bool UserLogout(int nUserID);
	bool UserLoginMap(std::string strUserName, std::string strPassport);
	bool UserLogoutMap(std::string strUserName);
	bool OperAccountBan(std::string strActName, int iban );//Add by sunny.sun 20090828

private:
	struct sPlayerData
	{
		CTime ctLoginTime;
	};
	typedef std::map<std::string, CDataBaseCtrl::sPlayerData> StringMap;

private:
	bool Connect();
	bool IsConnect();
	void Disconnect();

private:
	std::string m_strDsn;
	CSQLDatabase* m_pDataBase;
	StringMap m_mapUsers;
};
