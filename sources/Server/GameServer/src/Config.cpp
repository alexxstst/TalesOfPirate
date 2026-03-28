#include "stdafx.h"
#include "Config.h"
#include <fstream>
#include "Util.h"

using namespace std;

char	szConfigFileN[defCONFIG_FILE_NAME_LEN] = "GameServer00.cfg";
CGameConfig g_Config;
CGameCommand g_Command;

CGameConfig::CGameConfig()
{
	SetDefault();
}

void CGameConfig::SetDefault()
{
	m_nGateCnt = 0;
    m_nMapCnt  = 0;
	m_lSocketAlive = 1;
	memset(m_btMapOK, 0, MAX_MAP);
	strcpy(m_szDBIP,  "192.168.1.233");
	strcpy(m_szDBUsr,  "usr");
	strcpy(m_szDBPass, "22222"); 

	// Add by lark.li 20080321 begin
	memset(m_szTradeLogDBIP, 0, sizeof(m_szTradeLogDBIP));
	memset(m_szTradeLogDBName, 0, sizeof(m_szTradeLogDBName));
	memset(m_szTradeLogDBUsr, 0, sizeof(m_szTradeLogDBUsr));
	memset(m_szTradeLogDBPass, 0, sizeof(m_szTradeLogDBPass));
	// End

	memset( m_szEqument, 0, MAX_MAPNAME_LENGTH );
	m_nMaxPly = 3000;
	m_nMaxCha = 15000;
	m_nMaxItem = 10000;
	m_nMaxTNpc = 300;

	m_lItemShowTime = 300 * 1000;
	m_lItemProtTime = 30  * 1000;
	m_lSayInterval  =  3  * 1000;

	m_chMapMask = 1;
	m_lDBSave = 20 * 60 * 1000;

	strcpy(m_szResDir, "");
	strcpy(m_szLogDir, "log\\");

	strcpy(m_szInfoIP, "");
	m_nInfoPort = 0;
	strcpy(m_szInfoPwd, "");
	m_nSection = 0;

	m_bLogAI		= FALSE;	// �Ƿ��AI��log
	m_bLogCha		= FALSE;	// �Ƿ�򿪽�ɫ��log
	m_bLogCal		= FALSE;	// �Ƿ����ֵ�����log
	m_bLogMission	= FALSE;	// �Ƿ��Mission��log

	m_bSuperCmd     = FALSE;

	// Add by lark.li 20080731 begin
	m_vGMCmd.clear();
	// End

	m_bLogDB        = FALSE;

	m_bTradeLogIsConfig = FALSE;	// Add by lark.li 20080324

	m_sGuildNum = 80;
	m_sGuildTryNum = 80;
	m_bOfflineMode = FALSE;
	m_bBlindChaos = FALSE;
	strcpy(m_szChaosMap, "NONE");
	m_bInstantIGS = FALSE;
	m_lWeather = 120;
	m_dwStallTime = 48;
	m_cSaveState[32] = {0};
}

bool CGameConfig::Load(char *pszFileName)
{
	ToLogService("common", "Load Game Config File(Text Mode) [{}]", pszFileName);
	
	ifstream in(pszFileName);
	if(in.is_open()==0)
	{
		ToLogService("common", "msgLoad Game Config File(Text Mode) [{}] error! ", pszFileName);
		return false;
	}
	string strPair[2];
	string strComment;
	string strLine;
	char szLine[255];
	while(!in.eof())
	{
		in.getline(szLine, 255);
		strLine = szLine;
		auto p = strLine.find("//");
		if(p!= std::string::npos)
		{
			string strLeft = strLine.substr(0, p);
			strComment = strLine.substr(p + 2, strLine.size() - p - 2);
			strLine = strLeft;
		}
		else
		{
			strComment = "";
		}
		Util_TrimString(strLine);
		if(strLine.size()==0) continue;
		if(strLine[0]=='[') 
		{
			// Секция конфигурационного файла
			ToLogService("common", "{}", strLine);
			continue;
		}
		
		int n = Util_ResolveTextLine(strLine.c_str(), strPair, 2, '=');
		if(n < 2) continue;
        string strKey   = strPair[0];
		string strValue = strPair[1];
		
		if(strKey=="gate")
		{
			string strList[2];
            int nCnt = Util_ResolveTextLine(strValue.c_str(), strList, 2, ',');
		    if(nCnt==2)
            {
                strcpy(m_szGateIP[m_nGateCnt], strList[0].c_str());
                m_nGatePort[m_nGateCnt] = Str2Int(strList[1]);
				if (m_nGateCnt < MAX_GATE)
					m_nGateCnt++;
            }
        }
        else if(strKey=="info") // ����InfoServer IP��port
        {
            string strList[4];
            int nCnt = Util_ResolveTextLine(strValue.c_str(), strList, 4, ',');
            if(nCnt==4)
            {
                strcpy(m_szInfoIP, strList[0].c_str());
                m_nInfoPort = Str2Int(strList[1]);
				strcpy(m_szInfoPwd, strList[2].c_str());
				m_nSection = Str2Int(strList[3].c_str());
            }
        }
		else if(strKey == "persist_state")
		{
			string strList[32];
			int nCnt = Util_ResolveTextLine(strValue.c_str(), strList, 32, ',');
			for (int i = 0; i < nCnt; i++)
			{
				m_cSaveState[i] = Str2Int(strList[i]);
			}
		}
		else if(strKey=="map") 
		{
		    strcpy(m_szMapList[m_nMapCnt], strValue.c_str());
            m_nMapCnt++;
        }
		else if(strKey=="equment" )
		{
			strncpy( m_szEqument, strValue.c_str(), MAX_MAPNAME_LENGTH - 1 );
		}
		else if(strKey=="name")
		{
            strcpy(m_szName, strValue.c_str());
		}
		else if (strKey == "BaseID")
		{
			size_t stPos = 0;
			if ((stPos = strValue.find("0x")) != std::string::npos || (stPos = strValue.find("0X")) != std::string::npos) // ʮ������ֵ
				sscanf(strValue.c_str(), "%x", &m_ulBaseID);
			else // ʮ����ֵ
				sscanf(strValue.c_str(), "%d", &m_ulBaseID);
		}
		else if(strKey=="max_ply")
		{
			m_nMaxPly = Str2Int(strValue);
		}
		else if(strKey=="max_cha")
		{
			m_nMaxCha = Str2Int(strValue);
		}
		else if(strKey=="max_item")
		{
			m_nMaxItem = Str2Int(strValue);
		}
		else if(strKey=="max_tnpc")
		{
			m_nMaxTNpc = Str2Int(strValue);
		}
		else if(strKey=="db_ip")
		{
			strcpy(m_szDBIP, strValue.c_str());
		}
		else if(strKey=="db_usr")
		{
			strcpy(m_szDBUsr, strValue.c_str());
		}
		else if(strKey=="db_pass")
		{
			strcpy(m_szDBPass, strValue.c_str());
		}
		else if(strKey=="log_cha")
		{
			m_bLogCha = Str2Int(strValue);
		}
		else if(strKey=="db_name")
		{
			strncpy_s( m_szDBName, sizeof(m_szDBName), strValue.c_str(), _TRUNCATE );
		}
		else if(strKey=="log_ai")
		{
			m_bLogAI = Str2Int(strValue);
		}
		else if(strKey=="log_cal")
		{
			m_bLogCal = Str2Int(strValue);
		}
		else if(strKey=="log_mission")
		{
			m_bLogMission = Str2Int(strValue);
		}
		else if (strKey=="keep_alive")
		{
			m_lSocketAlive = Str2Int(strValue);
		}	
		// Add by lark.li 20080731 begin
		if(strKey=="gmcmd")
		{
			string strList[7];
            int nCnt = Util_ResolveTextLine(strValue.c_str(), strList, 7, ',');

			for(int i=0;i<nCnt;i++)
			{
                m_vGMCmd.push_back(Str2Int(strList[i]));
			}
       }
		// End
		else if(strKey=="supercmd")
		{
			m_bSuperCmd = Str2Int(strValue);
		}
		else if(strKey=="item_show_time")
		{
			m_lItemShowTime = Str2Int(strValue);
		}
		else if(strKey=="item_prot_time")
		{
			m_lItemProtTime = Str2Int(strValue);
		}
		else if(strKey=="say_interval")
		{
			m_lSayInterval = Str2Int(strValue) * 1000;
		}
		else if(strKey=="res_dir")
		{
			strcpy(m_szResDir, strValue.c_str());
		}
		else if(strKey=="log_dir")
		{
			strcpy(m_szLogDir, strValue.c_str());
		}
		else if(strKey=="db_mapmask")
		{
			m_chMapMask = Str2Int(strValue);
		}
		else if(strKey=="save_db")
		{
			m_lDBSave = Str2Int(strValue) * 60 * 1000;
		}
		else if(strKey=="log_db")
		{
			m_bLogDB = Str2Int(strValue);
		} // Add by lark.li 20080324 begin
		else if(strKey=="tradelog_db_ip")
		{
			strcpy(m_szTradeLogDBIP, strValue.c_str());
		}
		else if(strKey=="tradelog_db_name")
		{
			strcpy(m_szTradeLogDBName, strValue.c_str());
		}
		else if(strKey=="tradelog_db_usr")
		{
			strcpy(m_szTradeLogDBUsr, strValue.c_str());
		}
		else if(strKey=="tradelog_db_pass")
		{
			strcpy(m_szTradeLogDBPass, strValue.c_str());
		}
		else if(strKey=="guild_num")
		{
			m_sGuildNum = Str2Int(strValue);
		}
		else if(strKey=="guild_try_num")
		{
			m_sGuildTryNum = Str2Int(strValue);
		}
		else if(strKey=="stall_offline")
		{
			m_bOfflineMode = Str2Int(strValue);
		}
		else if(strKey=="igs_instant")
		{
			m_bInstantIGS = Str2Int(strValue);
		}
		else if(strKey=="stall_empty_dc")
		{
			m_bDiscStall = Str2Int(strValue);
		}
		else if(strKey=="chaos_blind")
		{
			m_bBlindChaos = Str2Int(strValue);
		}
		else if(strKey=="weather_interval")
		{
			m_lWeather = Str2Int(strValue);
		}
		else if(strKey=="chaos_map")
		{
			strncpy_s( m_szChaosMap, sizeof(m_szChaosMap), strValue.c_str(), _TRUNCATE );
		}
		else if(strKey=="stall_interval")
		{
			m_dwStallTime = Str2Int(strValue);
		}
	}
	in.close();

	// Add by lark.li 20080324 begin
	if( strlen(g_Config.m_szTradeLogDBIP) > 0 && strlen(g_Config.m_szTradeLogDBName) > 0 && strlen(g_Config.m_szTradeLogDBUsr) > 0 && strlen(g_Config.m_szTradeLogDBPass) > 0 )
		m_bTradeLogIsConfig = TRUE;
	// End

	return true;
}

bool CGameConfig::Reload(char *pszFileName)
{
	ToLogService("common", "Load Game Config File(Text Mode) [{}]", pszFileName);
	
	ifstream in(pszFileName);
	if(in.is_open()==0)
	{
		ToLogService("common", "msgLoad Game Config File(Text Mode) [{}] error! ", pszFileName);
		return false;
	}
	string strPair[2];
	string strComment;
	string strLine;
	char szLine[255];
	while(!in.eof())
	{
		in.getline(szLine, 255);
		strLine = szLine;
		auto p = strLine.find("//");
		if(p!= std::string::npos)
		{
			string strLeft = strLine.substr(0, p);
			strComment = strLine.substr(p + 2, strLine.size() - p - 2);
			strLine = strLeft;
		}
		else
		{
			strComment = "";
		}
		Util_TrimString(strLine);
		if(strLine.size()==0) continue;
		if(strLine[0]=='[') 
		{
			// Секция конфигурационного файла
			ToLogService("common", "{}", strLine);
			continue;
		}
		
		int n = Util_ResolveTextLine(strLine.c_str(), strPair, 2, '=');
		if(n < 2) continue;
        string strKey   = strPair[0];
		string strValue = strPair[1];
		if(strKey=="guild_num")
		{
			m_sGuildNum = Str2Int(strValue);
		}
		else if(strKey=="guild_try_num")
		{
			m_sGuildTryNum = Str2Int(strValue);
		}
		else if(strKey=="offline_stall")
		{
			m_bOfflineMode = Str2Int(strValue);
		}
		else if(strKey=="instant_igs")
		{
			m_bInstantIGS = Str2Int(strValue);
		}
		else if(strKey=="empty_disconnect")
		{
			m_bDiscStall = Str2Int(strValue);
		}
		else if(strKey=="chaos_blind")
		{
			m_bBlindChaos = Str2Int(strValue);
		}
	}
	in.close();
	return true;
}

CGameCommand::CGameCommand()
{
	SetDefault();
}

void CGameCommand::SetDefault()
{
	strcpy(m_cMove, "move");
	strcpy(m_cMake, "make");
	strcpy(m_cNotice, "notice");
	strcpy(m_cHide, "hide");
	strcpy(m_cUnhide, "unhide");
	strcpy(m_cGoto, "goto");
	strcpy(m_cKick, "kick");
	strcpy(m_cMute, "mute");
	strcpy(m_cReload, "reload");
	strcpy(m_cRelive, "relive");
	strcpy(m_cQcha, "qcha");
	strcpy(m_cQitem, "qitem");
	strcpy(m_cCall, "call");
	strcpy(m_cMove, "move");
	strcpy(m_cGamesvrstop, "gamesvrstop");
	strcpy(m_cUpdateall, "updateall");
	strcpy(m_cMisreload, "misreload");
	strcpy(m_cSummon, "summon");
	strcpy(m_cSummonex, "summonex");
	strcpy(m_cKill, "kill");
	strcpy(m_cAddmoney, "addmoney");
	strcpy(m_cAddexp, "addexp");
	strcpy(m_cAttr, "attr");
	strcpy(m_cItemattr, "itemattr");
	strcpy(m_cSkill, "skill");
	strcpy(m_cDelitem, "delitem");
	strcpy(m_cLuaall, "lua_all");
	strcpy(m_cAddkb, "addkb");
	strcpy(m_cLua, "lua");
	strcpy(m_cAddImp, "addimp");
	strcpy(m_cScrollNotice, "scroll");
	strcpy(m_cgenCharBag, "gencharbag");
	strcpy(m_cDistance, "distance");
}

bool CGameCommand::Load(const char *pszFileName)
{

	ToLogService("common", "Load Game Config File(Text Mode) [{}]", pszFileName);
	ifstream in(pszFileName);
	if(in.is_open()==0)
	{
		ToLogService("common", "msgLoad Game Config File(Text Mode) [{}] error! ", pszFileName);
		return false;
	}
	string strPair[2];
	string strComment;
	string strLine;
	char szLine[255];
	while(!in.eof()) {
		in.getline(szLine, 255);
		strLine = szLine;
		auto p = strLine.find("//");
		if(p!= std::string::npos)
		{
			string strLeft = strLine.substr(0, p);
			strComment = strLine.substr(p + 2, strLine.size() - p - 2);
			strLine = strLeft;
		}
		else
		{
			strComment = "";
		}
		Util_TrimString(strLine);
		if(strLine.size()==0)
			continue;
		if(strLine[0]=='[')
		{
			// Секция конфигурационного файла
			ToLogService("common", "{}", strLine);
			continue;
		}
		int n = Util_ResolveTextLine(strLine.c_str(), strPair, 2, '=');
		if(n < 2)
			continue;

		string strKey = strPair[0];
		string strValue = strPair[1];
		if (strKey == "cmd_move")
			strcpy(m_cMove, strValue.c_str());
		else if (strKey == "cmd_make")
			strcpy(m_cMake, strValue.c_str());
		else if (strKey == "cmd_notice")
			strcpy(m_cNotice, strValue.c_str());
		else if (strKey == "cmd_hide")
			strcpy(m_cHide, strValue.c_str());
		else if (strKey == "cmd_unhide")
			strcpy(m_cUnhide, strValue.c_str());
		else if (strKey == "cmd_goto")
			strcpy(m_cGoto, strValue.c_str());
		else if (strKey == "cmd_kick")
			strcpy(m_cKick, strValue.c_str());
		else if (strKey == "cmd_kick")
			strcpy(m_cKick, strValue.c_str());
		else if(strKey=="cmd_reload")
			strcpy(m_cReload, strValue.c_str());
		else if(strKey=="cmd_relive")
			strcpy(m_cRelive, strValue.c_str());
		else if(strKey=="cmd_qcha")
			strcpy(m_cQcha, strValue.c_str());
		else if(strKey=="cmd_qitem")
			strcpy(m_cQitem, strValue.c_str());
		else if(strKey=="cmd_call")
			strcpy(m_cCall, strValue.c_str());
		else if(strKey=="cmd_move")
			strcpy(m_cMove, strValue.c_str());
		else if(strKey=="cmd_gamesvrstop")
			strcpy(m_cGamesvrstop, strValue.c_str());
		else if(strKey=="cmd_updateall")
			strcpy(m_cUpdateall, strValue.c_str());
		else if(strKey=="cmd_misreload")
			strcpy(m_cMisreload, strValue.c_str());
		else if(strKey=="cmd_summon")
			strcpy(m_cSummon, strValue.c_str());
		else if(strKey=="cmd_summonex")
			strcpy(m_cSummonex, strValue.c_str());
		else if(strKey=="cmd_kill")
			strcpy(m_cKill, strValue.c_str());
		else if(strKey=="cmd_addmoney")
			strcpy(m_cAddmoney, strValue.c_str());
		else if(strKey=="cmd_addexp")
			strcpy(m_cAddexp, strValue.c_str());
		else if(strKey=="cmd_attr")
			strcpy(m_cAttr, strValue.c_str());
		else if(strKey=="cmd_itemattr")
			strcpy(m_cItemattr, strValue.c_str());
		else if(strKey=="cmd_skill")
			strcpy(m_cSkill, strValue.c_str());
		else if(strKey=="cmd_delitem")
			strcpy(m_cDelitem, strValue.c_str());
		else if(strKey=="cmd_lua_all")
			strcpy(m_cLuaall, strValue.c_str());
		else if(strKey=="cmd_addkb")
			strcpy(m_cAddkb, strValue.c_str());
		else if(strKey=="cmd_lua")
			strcpy(m_cLua, strValue.c_str());
		else if (strKey == "cmd_addimp")
			strcpy(m_cAddImp, strValue.c_str());
	}
	return true;
}