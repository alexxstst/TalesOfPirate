#include "stdafx.h"
#include "GameApp.h"
#include "Character.h"
#include "SubMap.h"
#include "NPC.h"
#include "Item.h"
#include "Script.h"
#include "CommFunc.h"
#include "Player.h"
#include "ItemAttr.h"
#include "JobInitEquip.h"
#include "GameAppNet.h"
#include "SkillStateRecord.h"
#include "lua_gamectrl.h"
#include "LuaAPI.h"
#include <fstream>
#include <iostream>

using namespace std;
#pragma warning(disable: 4355)


void CCharacter::DoCommand(cChar *cszCommand, uLong ulLen)
{
	Char	szComHead[256], szComParam[2048];
	std::string	strList[10];
	std::string strPrint = cszCommand;

	Char	*szCom = (Char *)cszCommand;
	size_t	tStart = strspn(cszCommand, " ");
	if (tStart >= strlen(cszCommand))
		return;
	szCom += tStart;
	Char	*szParam = strstr(szCom, " ");
	if (szParam)
	{
		*szParam = '\0';
		strncpy(szComHead, szCom, 256 - 1);
		if (szParam[1] != '\0')
			strncpy(szComParam, szParam + 1, 256 - 1);
		else
			szComParam[0] = '\0';
	}
	else
	{
		strncpy(szComHead, szCom, 256 - 1);
		szComParam[0] = '\0';
	}

	
	// GM
	if(DoGMCommand(szComHead, szComParam))
		//LG("DoCommand", "[]%s%s\n", GetLogName(), strPrint.c_str());
		ToLogService("commands", "[operator succeed]{}{}", GetLogName(), strPrint.c_str());
	else
		//LG("DoCommand", "[]%s%s\n", GetLogName(), strPrint.c_str());
		ToLogService("commands", "[operator succeed]{}{}", GetLogName(), strPrint.c_str());
	
}


//--------------------------------------------------------------------------------
// GM , 
//--------------------------------------------------------------------------------
//TODO(Ogge): Extract method for each GM-level
BOOL CCharacter::DoGMCommand(const char *pszCmd, const char *pszParam)
{
	CPlayer *pPlayer = GetPlayer(); 
	if(!pPlayer) return FALSE;
	
	uChar uchGMLv = pPlayer->GetGMLev();
	if (uchGMLv == 0)
	{
		//SystemNotice("!");
		SystemNotice(RES_STRING(GM_CHARACTERSUPERCMD_CPP_00001));
		return FALSE;
	}

	std::string	strList[10];
	string strCmd = pszCmd;

	C_PRINT("%s: %s %s\n", GetName(), strCmd.c_str(), pszParam);
	//-----------------------
	// GM
	//-----------------------
	if (strCmd==g_Command.m_cMove) // move x,y,
	{
		int n = Util_ResolveTextLine(pszParam, strList, 10, ',');
		Point l_aim;
		l_aim.x = Str2Int(strList[0]) * 100;
		l_aim.y = Str2Int(strList[1]) * 100;
		const char	*szMapName = 0;
		short	sMapCpyNO = 0;
		if (n == 3)
			szMapName = strList[2].c_str();
		else
			szMapName = GetSubMap()->GetName();
		if (n == 4)
			sMapCpyNO = Str2Int(strList[1]);

		SwitchMap(GetSubMap(), szMapName, l_aim.x, l_aim.y, true, enumSWITCHMAP_CARRY, sMapCpyNO);
		// Delete by lark.li 20080814 begin
		//LG("ServerRunLog", "ChaID: %i, ChaName: %s, CMD: %s, Param: %s\n", GetPlayer()->GetID(), GetName(), pszCmd, pszParam);
		// End
		return TRUE;
	}
	else if(strCmd==g_Command.m_cNotice) // 
	{
		g_pGameApp->WorldNotice(pszParam);
		ToLogService("common", "ChaID: {}, ChaName: {}, CMD: {}, Param: {}", GetPlayer()->GetID(), GetName(), pszCmd, pszParam);
		return TRUE;
	}
	else if(strCmd==g_Command.m_cHide) // 
	{
		AddSkillState(m_uchFightID, GetID(), GetHandle(), enumSKILL_TYPE_SELF, enumSKILL_TAR_LORS, enumSKILL_EFF_HELPFUL, SSTATE_HIDE, 1, -1, enumSSTATE_ADD);
		ToLogService("common", "ChaID: {}, ChaName: {}, CMD: {}, Param: {}", GetPlayer()->GetID(), GetName(), pszCmd, pszParam);
		return TRUE;
	}
	else if(strCmd==g_Command.m_cUnhide) // 
	{
		DelSkillState(SSTATE_HIDE);
		ToLogService("common", "ChaID: {}, ChaName: {}, CMD: {}, Param: {}", GetPlayer()->GetID(), GetName(), pszCmd, pszParam);
		return TRUE;
	}
	else if(strCmd==g_Command.m_cGoto) // 
	{
		int n = Util_ResolveTextLine(pszParam, strList, 10, ',');
		//  :    
		auto WtPk = net::msg::serialize(net::msg::MmGotoChaMessage{GetID(), strList[0], 1, GetName()});
		ReflectINFof(this, WtPk);
		ToLogService("common", "ChaID: {}, ChaName: {}, CMD: {}, Param: {}", GetPlayer()->GetID(), GetName(), pszCmd, pszParam);
		return TRUE;
	}

	if(uchGMLv <= 1)
	{
		//SystemNotice("!");
		SystemNotice(RES_STRING(GM_CHARACTERSUPERCMD_CPP_00001));
		return FALSE;
	}

	//-----------------------
	// 1GM
	//-----------------------
	
	if (strCmd == g_Command.m_cMute)
	{
		const auto n = Util_ResolveTextLine(pszParam, strList, 10, ',');
		if (n < 2)
		{
			return false;
		}

		//  :   (ReflectINFof  trailer )
		auto wpk = net::msg::serialize(net::msg::GmMutePlayerMessage{strList[0], static_cast<int64_t>(Str2Int(strList[1]))});
		ReflectINFof(this, wpk);
	}

	if (strCmd==g_Command.m_cKick) // 
	{
		int n = Util_ResolveTextLine(pszParam, strList, 10, ',');
		if (n < 1)
		{
			SystemNotice("You did not input a player name!");
			return FALSE;
		}
		//  :   (-)
		auto WtPk = net::msg::serialize(net::msg::MmKickChaMessage{GetID(), strList[0], (n == 2) ? (int64_t)Str2Int(strList[1]) : 0LL});
		ReflectINFof(this, WtPk);
		ToLogService("common", "ChaID: {}, ChaName: {}, CMD: {}, Param: {}", GetPlayer()->GetID(), GetName(), pszCmd, pszParam);
		return TRUE;
	}

	// Modify by lark.li 20080731 begin
	time_t t = time(0);
    tm* TM = localtime(&t);

	bool gmOK = false;

	for(vector<int>::iterator it = g_Config.m_vGMCmd.begin(); it != g_Config.m_vGMCmd.end(); it++)
	{
		if(TM->tm_wday == *it)
		{
			gmOK = true;
			break;
		}
	}

	if(!gmOK)
	{
		SystemNotice(RES_STRING(GM_CHARACTERSUPERCMD_CPP_00047));
		return FALSE;
	}
	// End

	if(uchGMLv != 99)
	{
		//SystemNotice("!");
		SystemNotice(RES_STRING(GM_CHARACTERSUPERCMD_CPP_00001));
		return FALSE;
	}

    ToLogService("common", "ChaID: {}, ChaName: {}, CMD: {}, Param: {}", GetPlayer()->GetID(), GetName(), pszCmd, pszParam);

	cChar	*szComHead = pszCmd;
	cChar	*szComParam = pszParam;
	//-----------------------
	// 99GM
	//-----------------------
	if (!strcmp(szComHead, g_Command.m_cReload)) // 
	{
		cChar *pszChaInfo = "characterinfo";
		cChar *pszSkillInfo = "skillinfo";
		cChar *pszItemInfo = "iteminfo";
		if (!strcmp(szComParam, pszChaInfo))
			g_pGameApp->LoadCharacterInfo();
		else if (!strcmp(szComParam, pszSkillInfo))
			g_pGameApp->LoadSkillInfo();
		else if (!strcmp(szComParam, pszItemInfo))
			g_pGameApp->LoadItemInfo();
		else
		{
			SystemNotice("Available argument: iteminfo, skillinfo, and characterinfo", szComParam);
			return TRUE;
		}
		SystemNotice("Reloading %s.txt success!", szComParam);
		ToLogService("common", "ChaID: {}, ChaName: {}, CMD: {}, Param: {}", GetPlayer()->GetID(), GetName(), pszCmd, pszParam);
		return TRUE;
	}
    else if(!strcmp(szComHead, g_Command.m_cRelive)) // 
	{	
		ToLogService("common", "ChaID: {}, ChaName: {}, CMD: {}, Param: {}", GetPlayer()->GetID(), GetName(), pszCmd, pszParam);
		return TRUE;
	}
	else if(!strcmp(szComHead, g_Command.m_cQcha)) // (,,ID)
	{
		int n = Util_ResolveTextLine(pszParam, strList, 10, ',');
		//  :    
		auto WtPk = net::msg::serialize(net::msg::MmQueryChaMessage{GetID(), strList[0]});
		ReflectINFof(this, WtPk);
		ToLogService("common", "ChaID: {}, ChaName: {}, CMD: {}, Param: {}", GetPlayer()->GetID(), GetName(), pszCmd, pszParam);
		return TRUE;
	}
	else if(!strcmp(szComHead, g_Command.m_cQitem)) // 
	{
		int n = Util_ResolveTextLine(pszParam, strList, 10, ',');
		//  :   
		auto WtPk = net::msg::serialize(net::msg::MmQueryChaItemMessage{GetID(), strList[0]});
		ReflectINFof(this, WtPk);
		ToLogService("common", "ChaID: {}, ChaName: {}, CMD: {}, Param: {}", GetPlayer()->GetID(), GetName(), pszCmd, pszParam);
		return TRUE;
	}
    if(!strcmp(szComHead, g_Command.m_cCall)) // 
	{
		int n = Util_ResolveTextLine(pszParam, strList, 10, ',');
		//  :    
		auto WtPk = net::msg::serialize(net::msg::MmCallChaMessage{GetID(), strList[0], IsBoat() ? 1LL : 0LL, GetSubMap()->GetName(), (int64_t)GetPos().x, (int64_t)GetPos().y, (int64_t)GetSubMap()->GetCopyNO()});
		ReflectINFof(this, WtPk);
		ToLogService("common", "ChaID: {}, ChaName: {}, CMD: {}, Param: {}", GetPlayer()->GetID(), GetName(), pszCmd, pszParam);
		return TRUE;
	}
	else if (!strcmp(szComHead, g_Command.m_cGamesvrstop)) // 
	{
		g_pGameApp->m_CTimerReset.Begin(1000);
		g_pGameApp->m_ulLeftSec = atol(szComParam);
		ToLogService("common", "ChaID: {}, ChaName: {}, CMD: {}, Param: {}", GetPlayer()->GetID(), GetName(), pszCmd, pszParam);
		return TRUE;
	}
	else if( !strcmp(szComHead, g_Command.m_cUpdateall) ) // lua
	{
		LoadScript();
		if ( g_pGameApp->ReloadNpcInfo( *this ) )
		{
			//SystemNotice( "NPClua!" );
			SystemNotice( RES_STRING(GM_CHARACTERSUPERCMD_CPP_00002) );
		}
		else
		{
			return FALSE;
		}
		ToLogService("common", "ChaID: {}, ChaName: {}, CMD: {}, Param: {}", GetPlayer()->GetID(), GetName(), pszCmd, pszParam);
		return TRUE;
	}
	else if( !strcmp(szComHead, "reloadcfg"))
	{
		if (!g_Config.Reload((char*)pszParam))
		{
			SystemNotice("Reloading %s failed!", pszParam);
		}
		else
		{
			SystemNotice("Reloading %s success!", pszParam);
		}
		return TRUE;
	}
	else if( !strcmp(szComHead, "harmlog=1") ) // Log
	{
		g_bLogHarmRec = TRUE;
		ToLogService("common", "ChaID: {}, ChaName: {}, CMD: {}, Param: {}", GetPlayer()->GetID(), GetName(), pszCmd, pszParam);
		return TRUE;
	}
	else if( !strcmp(szComHead, "harmlog=0") ) // Log
	{
		g_bLogHarmRec = FALSE;
		ToLogService("common", "ChaID: {}, ChaName: {}, CMD: {}, Param: {}", GetPlayer()->GetID(), GetName(), pszCmd, pszParam);
		return TRUE;
	}
	else if( !strcmp(szComHead, g_Command.m_cMisreload) ) // 
	{
		LoadScript();
		if( g_pGameApp->ReloadNpcInfo( *this ) )
		{
			//SystemNotice( "NPClua!" );
			SystemNotice( RES_STRING(GM_CHARACTERSUPERCMD_CPP_00002) );
		}
		else
		{
			return FALSE;
		}
		ToLogService("common", "ChaID: {}, ChaName: {}, CMD: {}, Param: {}", GetPlayer()->GetID(), GetName(), pszCmd, pszParam);
		return TRUE;
	}
	else if( !strcmp(szComHead, "reload_ai") )
	{
		ReloadAISdk();
		ToLogService("common", "ChaID: {}, ChaName: {}, CMD: {}, Param: {}", GetPlayer()->GetID(), GetName(), pszCmd, pszParam);
		return TRUE;
	}
	else if( !strcmp(szComHead, "setrecord" ) ) // 
	{
		int n = Util_ResolveTextLine(szComParam, strList, 10, ',');		
		USHORT sID   = Str2Int(strList[0]);
		if( GetPlayer()->MisSetRecord( sID ) )
		{
			//SystemNotice( "!ID[%d]", sID );
			SystemNotice( RES_STRING(GM_CHARACTERSUPERCMD_CPP_00003), sID );
			ToLogService("common", "ChaID: {}, ChaName: {}, CMD: {}, Param: {}", GetPlayer()->GetID(), GetName(), pszCmd, pszParam);
			return TRUE;
		}
		else
		{
			//SystemNotice( "!ID[%d]", sID );
			SystemNotice( RES_STRING(GM_CHARACTERSUPERCMD_CPP_00003), sID );
			return FALSE;
		}
		ToLogService("common", "ChaID: {}, ChaName: {}, CMD: {}, Param: {}", GetPlayer()->GetID(), GetName(), pszCmd, pszParam);
		return TRUE;
	}
	else if( !strcmp(szComHead, "clearrecord" ) ) // 
	{
		int n = Util_ResolveTextLine(szComParam, strList, 10, ',');		
		USHORT sID   = Str2Int(strList[0]);
		if( GetPlayer()->MisClearRecord( sID ) )
		{
			//SystemNotice( "!ID[%d]", sID );
			SystemNotice( RES_STRING(GM_CHARACTERSUPERCMD_CPP_00004), sID );
			ToLogService("common", "ChaID: {}, ChaName: {}, CMD: {}, Param: {}", GetPlayer()->GetID(), GetName(), pszCmd, pszParam);
			return TRUE;
		}
		else
		{
			//SystemNotice( "!ID[%d]", sID );
			SystemNotice( RES_STRING(GM_CHARACTERSUPERCMD_CPP_00005), sID );
			return FALSE;
		}
		return TRUE;
	}
	else if( !strcmp(szComHead, "setflag" ) ) // 
	{
		int n = Util_ResolveTextLine(szComParam, strList, 10, ',');		
		USHORT sID   = Str2Int(strList[0]);
		USHORT sFlag = Str2Int(strList[1]);
		if( GetPlayer()->MisSetFlag( sID, sFlag ) )
		{
			//SystemNotice( "!ID[%d], FLAG[%d]", sID, sFlag );
			SystemNotice( RES_STRING(GM_CHARACTERSUPERCMD_CPP_00006), sID, sFlag );
			ToLogService("common", "ChaID: {}, ChaName: {}, CMD: {}, Param: {}", GetPlayer()->GetID(), GetName(), pszCmd, pszParam);
			return TRUE;
		}
		else
		{
			//SystemNotice( "!ID[%d], FLAG[%d]", sID, sFlag );
			SystemNotice( RES_STRING(GM_CHARACTERSUPERCMD_CPP_00007), sID, sFlag );
			return FALSE;
		}
		return TRUE;
	}
	else if( !strcmp(szComHead, "clearflag" ) ) // 
	{
		int n = Util_ResolveTextLine(szComParam, strList, 10, ',');		
		USHORT sID   = Str2Int(strList[0]);
		USHORT sFlag = Str2Int(strList[1]);
		if( GetPlayer()->MisClearFlag( sID, sFlag ) )
		{
			//SystemNotice( "!ID[%d], FLAG[%d]", sID, sFlag );
			SystemNotice( RES_STRING(GM_CHARACTERSUPERCMD_CPP_00008), sID, sFlag );
			ToLogService("common", "ChaID: {}, ChaName: {}, CMD: {}, Param: {}", GetPlayer()->GetID(), GetName(), pszCmd, pszParam);
			return TRUE;
		}
		else
		{
			//SystemNotice( "!ID[%d], FLAG[%d]", sID, sFlag );
			SystemNotice( RES_STRING(GM_CHARACTERSUPERCMD_CPP_00009), sID, sFlag );
			return FALSE;
		}
		return TRUE;
	}
	else if( !strcmp(szComHead, "addmission" ) ) // 
	{
		int n = Util_ResolveTextLine(szComParam, strList, 10, ',');		
		USHORT sMID   = Str2Int(strList[0]);
		USHORT sSID   = Str2Int(strList[1]);
		if( GetPlayer()->MisAddRole( sMID, sSID ) )
		{
			//SystemNotice( "!MID[%d], SID[%d]", sMID, sSID );
			SystemNotice( RES_STRING(GM_CHARACTERSUPERCMD_CPP_00010), sMID, sSID );
			ToLogService("common", "ChaID: {}, ChaName: {}, CMD: {}, Param: {}", GetPlayer()->GetID(), GetName(), pszCmd, pszParam);
			return TRUE;
		}
		else
		{
			//SystemNotice( "!MID[%d], SID[%d]", sMID, sSID );
			SystemNotice( RES_STRING(GM_CHARACTERSUPERCMD_CPP_00011), sMID, sSID );
			return FALSE;
		}
		return TRUE;
	}
	else if( !strcmp(szComHead, "clearmission" ) ) // 
	{
		int n = Util_ResolveTextLine(szComParam, strList, 10, ',');
		USHORT sID   = Str2Int(strList[0]);
		if( GetPlayer()->MisCancelRole( sID ) )
		{
			//SystemNotice( "!MID[%d]", sID );
			SystemNotice( RES_STRING(GM_CHARACTERSUPERCMD_CPP_00012), sID );
			ToLogService("common", "ChaID: {}, ChaName: {}, CMD: {}, Param: {}", GetPlayer()->GetID(), GetName(), pszCmd, pszParam);
			return TRUE;
		}
		else
		{
			//SystemNotice( "!MID[%d]", sID );
			SystemNotice( RES_STRING(GM_CHARACTERSUPERCMD_CPP_00013), sID );
			return FALSE;
		}
		return TRUE;
	}
	else if( !strcmp(szComHead, "delmission" ) ) // 
	{
		int n = Util_ResolveTextLine(szComParam, strList, 10, ',');
		USHORT sID   = Str2Int(strList[0]);
		if( GetPlayer()->MisClearRole( sID ) )
		{
			//SystemNotice( "!MID[%d]", sID );
			SystemNotice( RES_STRING(GM_CHARACTERSUPERCMD_CPP_00014), sID );
			ToLogService("common", "ChaID: {}, ChaName: {}, CMD: {}, Param: {}", GetPlayer()->GetID(), GetName(), pszCmd, pszParam);
			return TRUE;
		}
		else
		{
			//SystemNotice( "!MID[%d]", sID );
			SystemNotice( RES_STRING(GM_CHARACTERSUPERCMD_CPP_00015), sID );
			return FALSE;
		}
		return TRUE;
	}
	else if( !strcmp(szComHead, "missdk" ) )	 // 
	{
		ReloadLuaSdk();
		ToLogService("common", "ChaID: {}, ChaName: {}, CMD: {}, Param: {}", GetPlayer()->GetID(), GetName(), pszCmd, pszParam);
		return TRUE;
	}
	else if( !strcmp( szComHead, "misclear") ) // 
	{
		GetPlayer()->MisClear();
		//SystemNotice( "!" );
		SystemNotice( RES_STRING(GM_CHARACTERSUPERCMD_CPP_00016) );
		ToLogService("common", "ChaID: {}, ChaName: {}, CMD: {}, Param: {}", GetPlayer()->GetID(), GetName(), pszCmd, pszParam);
		return TRUE;
	}
	else if (!strcmp(szComHead, "isblock")) // 
	{
		if (m_submap->IsBlock(GetPos().x / m_submap->GetBlockCellWidth(), GetPos().y / m_submap->GetBlockCellHeight()))
			//SystemNotice("");
			SystemNotice(RES_STRING(GM_CHARACTERSUPERCMD_CPP_00017));
		else
			//SystemNotice("");
			SystemNotice(RES_STRING(GM_CHARACTERSUPERCMD_CPP_00018));
		ToLogService("common", "ChaID: {}, ChaName: {}, CMD: {}, Param: {}", GetPlayer()->GetID(), GetName(), pszCmd, pszParam);
		return TRUE;
	}
	else if (!strcmp(szComHead, "pet")) // 
	{
		Long	lChaInfoID = Str2Int(strList[0]);
		Point	Pos = GetPos();
		Pos.move(rand() % 360, 3 * 100);
		CCharacter *pCha = m_submap->ChaSpawn(lChaInfoID, enumCHACTRL_PLAYER_PET, rand()%360, &Pos);
		if (pCha)
		{
			pCha->m_HostCha = this;
			pCha->SetPlayer(GetPlayer());
			pCha->m_AIType = 5;
			ToLogService("common", "ChaID: {}, ChaName: {}, CMD: {}, Param: {}", GetPlayer()->GetID(), GetName(), pszCmd, pszParam);
			return TRUE;
		}
		else
		{
			//SystemNotice( "" );
			SystemNotice( RES_STRING(GM_CHARACTERSUPERCMD_CPP_00019) );
			return FALSE;
		}
		return TRUE;
	}
	else if (!strcmp(szComHead, g_Command.m_cSummon)) // 
	{
		int n = Util_ResolveTextLine(szComParam, strList, 10, ',');
		if (n >= 1)
		{
			Long	lChaInfoID = Str2Int(strList[0]);
			Point	Pos = GetPos();
			Pos.move(rand() % 360, 3 * 100);
			CCharacter *pCha = m_submap->ChaSpawn(lChaInfoID, enumCHACTRL_NONE, rand()%360, &Pos);
			if (pCha)
			{
				if(n>=2)
				{
					DWORD dwLifeTime = Str2Int(strList[1]);
					pCha->ResetLifeTime(dwLifeTime);	
				}
				if( n==3 )
				{
					int nAIType = Str2Int(strList[2]);
					pCha->m_AIType  = (BYTE)nAIType; // AI
				}
			}
			else
			{
				//SystemNotice( "!" );
				SystemNotice( RES_STRING(GM_CHARACTERSUPERCMD_CPP_00020) );
				return FALSE;
			}
			ToLogService("common", "ChaID: {}, ChaName: {}, CMD: {}, Param: {}", GetPlayer()->GetID(), GetName(), pszCmd, pszParam);
			return TRUE;
		}
		return FALSE;
	}
	else if (!strcmp(szComHead, g_Command.m_cSummonex)) // 
	{
		int n = Util_ResolveTextLine(szComParam, strList, 10, ',');
		if (n >= 1)
		{
			Long	lChaInfoID = Str2Int(strList[0]);
			Point	Pos;
			long	lChaNum = 1;
			if (n > 1)
				lChaNum = Str2Int(strList[1]);
			bool	bActEyeshot = false;
			if( n > 2 )
				bActEyeshot = Str2Int(strList[2]) ? true : false;
			int nAIType = 0;
			if( n > 3 )
				nAIType = Str2Int(strList[3]);
			CCharacter *pCha;
			for (long i = 0; i < lChaNum; i++)
			{
				Pos = GetPos();
				Pos.move(rand() % 360, rand() % 20 * 100);
				pCha = m_submap->ChaSpawn(lChaInfoID, enumCHACTRL_NONE, rand()%360, &Pos, bActEyeshot);
				if (pCha)
				{
					if( n > 3 )
						pCha->m_AIType = (BYTE)nAIType; // AI
				}
				else
				{
					//SystemNotice( "!" );
					SystemNotice( RES_STRING(GM_CHARACTERSUPERCMD_CPP_00021 ));
				}
			}
			ToLogService("common", "ChaID: {}, ChaName: {}, CMD: {}, Param: {}", GetPlayer()->GetID(), GetName(), pszCmd, pszParam);
			return TRUE;
		}
		return FALSE;
	}
	else if (!strcmp(szComHead, g_Command.m_cKill)) // 8
	{
		int n = Util_ResolveTextLine(szComParam, strList, 10, ',');
		if (n >= 1)
		{
			const char	*szMonsName = strList[0].c_str();
			long	lRange = 8 * 100;
			long	lNum = 0;
			if (n >= 2)
				lRange = Str2Int(strList[1]) * 100;
			if (n >= 3)
				lNum = Str2Int(strList[2]);

			long	lBParam[defSKILL_RANGE_BASEP_NUM] = {GetPos().x, GetPos().y, 0};
			long	lEParam[defSKILL_RANGE_EXTEP_NUM] = {enumRANGE_TYPE_CIRCLE, lRange};
			CCharacter	*pCCha = 0, *pCFreeCha = 0;
			long	lFindNum = 0, lKillNum = 0;
			GetSubMap()->BeginSearchInRange(lBParam, lEParam);
			while (pCCha = GetSubMap()->GetNextCharacterInRange())
			{
				if (pCFreeCha)
					pCFreeCha->Free();
				pCFreeCha = 0;

				lFindNum++;
				if (pCCha->IsPlayerCha())
					continue;
				if (!strcmp(pCCha->GetName(), szMonsName))
				{
					if (lNum == 0 || lKillNum <= lNum)
					{
						pCFreeCha = pCCha;
						lKillNum++;
					}
				}
			}
			if (pCFreeCha)
				pCFreeCha->Free();

			//SystemNotice( "%u.!", lKillNum );
			SystemNotice( RES_STRING(GM_CHARACTERSUPERCMD_CPP_00022), lKillNum );
			ToLogService("common", "ChaID: {}, ChaName: {}, CMD: {}, Param: {}", GetPlayer()->GetID(), GetName(), pszCmd, pszParam);
			return TRUE;
		}
		return FALSE;
	}
	
	if(g_Config.m_bSuperCmd==FALSE)
	{
		//SystemNotice("!");
		SystemNotice(RES_STRING(GM_CHARACTERSUPERCMD_CPP_00001));
		return FALSE;
	}
	
	//-----------------------------------
	// , GM
	//-----------------------------------
	if( !strcmp( szComHead, g_Command.m_cAddmoney ) )
	{
		//AddMoney( "", atol(szComParam) );
		AddMoney( RES_STRING(GM_CHARACTERSUPERCMD_CPP_00023), atol(szComParam) );
		ToLogService("common", "ChaID: {}, ChaName: {}, CMD: {}, Param: {}", GetPlayer()->GetID(), GetName(), pszCmd, pszParam);
		return TRUE;
	}
	if (!strcmp(szComHead, g_Command.m_cAddImp))
	{
		const char* szItemScript = "GiveIMP";
		lua_getglobal(g_pLuaState, szItemScript);
		if (!lua_isfunction(g_pLuaState, -1))
		{
			lua_pop(g_pLuaState, 1);
			return FALSE;
		}
		luabridge::push(g_pLuaState, static_cast<CCharacter*>(this));
		lua_pushnumber(g_pLuaState, atol(szComParam));
		int ret = lua_pcall(g_pLuaState, 2, 0, 0);
		lua_settop(g_pLuaState, 0);
		return TRUE;
	}
	else if( !strcmp( szComHead, g_Command.m_cAddexp ) )
	{
		AddExpAndNotic( atol(szComParam) );
		ToLogService("common", "ChaID: {}, ChaName: {}, CMD: {}, Param: {}", GetPlayer()->GetID(), GetName(), pszCmd, pszParam);
		return TRUE;
	}
	else if( !strcmp( szComHead, "addlifeexp" ) )
	{
		AddAttr( ATTR_CLIFEEXP, atol(szComParam) );
		//SystemNotice( "%ld!", atol(szComParam) );
		SystemNotice( RES_STRING(GM_CHARACTERSUPERCMD_CPP_00024), atol(szComParam) );
		ToLogService("common", "ChaID: {}, ChaName: {}, CMD: {}, Param: {}", GetPlayer()->GetID(), GetName(), pszCmd, pszParam);
		return TRUE;
	}
	else if( !strcmp( szComHead, "addsailexp" ) )
	{
		AddAttr( ATTR_CSAILEXP, atol(szComParam) );
		//SystemNotice( "%ld!", atol(szComParam) );
		SystemNotice( RES_STRING(GM_CHARACTERSUPERCMD_CPP_00025), atol(szComParam) );
		ToLogService("common", "ChaID: {}, ChaName: {}, CMD: {}, Param: {}", GetPlayer()->GetID(), GetName(), pszCmd, pszParam);
		return TRUE;
	}
	else if( !strcmp( szComHead, "addcess" ) )
	{
		AdjustTradeItemCess( 60000, (USHORT)atol(szComParam) );
		//SystemNotice( "%ld!", atol(szComParam) );
		SystemNotice( RES_STRING(GM_CHARACTERSUPERCMD_CPP_00026), atol(szComParam) );
		ToLogService("common", "ChaID: {}, ChaName: {}, CMD: {}, Param: {}", GetPlayer()->GetID(), GetName(), pszCmd, pszParam);
		return TRUE;
	}
	else if( !strcmp( szComHead, "setcesslevel" ) )
	{
		SetTradeItemLevel( (BYTE)atol(szComParam) );
		//SystemNotice( "%ld!", atol(szComParam) );
		SystemNotice( RES_STRING(GM_CHARACTERSUPERCMD_CPP_00027), atol(szComParam) );
		ToLogService("common", "ChaID: {}, ChaName: {}, CMD: {}, Param: {}", GetPlayer()->GetID(), GetName(), pszCmd, pszParam);
		return TRUE;
	}
	else if (!strcmp(szComHead, g_Command.m_cMake)) // [][1.]
	{
		ToLogService("commands", "begin make");
		int n = Util_ResolveTextLine(szComParam, strList, 20, ',');
		if(n >= 2)
		{
			short	sID = Str2Int(strList[0]);
			short	sNum = Str2Int(strList[1]);
			short	sTo = 1; // 
			char	chSpawnType = enumITEM_INST_MONS;
			if (sNum < 0 || sNum > 100)
				sNum = 10;
			if (n == 3)
				chSpawnType = Str2Int(strList[2]);
			if (n == 4)
				sTo = Str2Int(strList[3]);
			
			
			ToLogService("commands", "atorNome = {},sID = {},sNum = {},sTo = {},chSpawnType = {}",
				m_name,sID,sNum,sTo,chSpawnType);

			if (sTo == 1)
			{
				if (AddItem( sID, sNum, this->GetName(), chSpawnType))
				{
					ToLogService("commands", "add to kitbag successful!");
					return TRUE;
				}
			}
			else
			{
				SItemGrid GridContent(sID, sNum);
				ItemInstance(chSpawnType, &GridContent);
				Long	lPosX, lPosY;
				CCharacter	*pCCtrlCha = GetPlyCtrlCha();
				pCCtrlCha->GetTrowItemPos(&lPosX, &lPosY);
				if (pCCtrlCha->GetSubMap()->ItemSpawn(&GridContent, lPosX, lPosY, enumITEM_APPE_THROW, pCCtrlCha->GetID()))
				{
					ToLogService("commands", "add to ground successful!");
					return TRUE;
				}
			}
			ToLogService("commands", "make failed!");
			return FALSE;
		}
		ToLogService("commands", "make failed! because the param is less than 2!");
		return FALSE;
	}
	else if (!strcmp(szComHead, g_Command.m_cAttr)) // .
	{
		int n = Util_ResolveTextLine(szComParam, strList, 10, ',');
		if (n < 2)
			return FALSE;
		CCharacter	*pCCha = this;

		long	lAttrID = Str2Int(strList[0]);


		long	lAttrVal = Str2Int(strList[1]);
		//LONG64 lAttrVal = _atoi64(strList[1].c_str());

		if (n == 3)
			pCCha = g_pGameApp->FindChaByID(Str2Int(strList[2]));
		if (!pCCha)
		{
			//SystemNotice("!");
			SystemNotice(RES_STRING(GM_CHARACTERSUPERCMD_CPP_00028));
			return FALSE;
		}
		pCCha->m_CChaAttr.ResetChangeFlag();
		pCCha->SetBoatAttrChangeFlag(false);
		pCCha->setAttr(lAttrID, lAttrVal);
		if (pCCha->IsPlayerOwnCha())
		{
			if (pCCha->IsBoat())
				g_luaAPI.Call("ShipAttrRecheck", pCCha);
			else
				g_luaAPI.Call("AttrRecheck", pCCha);
		}
		else
		{
			g_luaAPI.Call("ALLExAttrSet", pCCha);
		}
		if (pCCha->GetPlayer())
		{
			pCCha->GetPlayer()->RefreshBoatAttr();
			pCCha->SyncBoatAttr(enumATTRSYN_TASK);
		}
		pCCha->SynAttr(enumATTRSYN_TASK);
		ToLogService("common", "ChaID: {}, ChaName: {}, CMD: {}, Param: {}", GetPlayer()->GetID(), GetName(), pszCmd, pszParam);
		return TRUE;
	}
	else if (!strcmp(szComHead, g_Command.m_cItemattr)) // 1.2.
	{
		int n = Util_ResolveTextLine(szComParam, strList, 10, ',');
		if (n != 4)
			return FALSE;

		Char	chPosType = Str2Int(strList[0]);
		Long	lPosID = Str2Int(strList[1]);
		SItemGrid	*pSItem = GetItem2(chPosType, lPosID);
		if (!pSItem)
			return FALSE;
		Long	lAttrID = Str2Int(strList[2]);
		Short	sAttr = Str2Int(strList[3]);
		pSItem->SetAttr(lAttrID, sAttr);
		pSItem->SetInstAttr(lAttrID, sAttr);
		ToLogService("common", "ChaID: {}, ChaName: {}, CMD: {}, Param: {}", GetPlayer()->GetID(), GetName(), pszCmd, pszParam);
		return TRUE;
	}
	
	else if (!strcmp(szComHead, "setexpiration"))
	{
	int n = Util_ResolveTextLine(szComParam, strList, 30, ',');
	if (n != 3) return FALSE;
	Char	chPosType = Str2Int(strList[0]);
	Long	lPosID = Str2Int(strList[1]);
	SItemGrid* pSItem = GetItem2(chPosType, lPosID);
	if (!pSItem)
		return FALSE;

	int minutes = Str2Int(strList[2]);
	time_t expirationt;
	if (minutes == 0) expirationt = 0;
	else expirationt = std::time(0) + minutes;
	pSItem->expiration = expirationt;
	pSItem->SetChange(true);
	char buf[32];

	return TRUE;

	}

	else if (!strcmp(szComHead, "light")) // .
	{
		int n = Util_ResolveTextLine(szComParam, strList, 10, ',');
		if (n != 1)
			return FALSE;

		long	lLight = Str2Int(strList[0]);
		if (lLight < 0)
			return FALSE;
		GetPlayer()->SetMMaskLightSize(lLight);
		ToLogService("common", "ChaID: {}, ChaName: {}, CMD: {}, Param: {}", GetPlayer()->GetID(), GetName(), pszCmd, pszParam);
		return TRUE;
	}
	else if (!strcmp(szComHead, "seeattr")) // WorldID.
	{
		int n = Util_ResolveTextLine(szComParam, strList, 10, ',');
		if (n != 2)
			return FALSE;

		uLong	ulWorldID = Str2Int(strList[0]);
		short	sAttrID = Str2Int(strList[1]);
		CCharacter	*pCCha = g_pGameApp->FindChaByID(ulWorldID);
		if (!pCCha)
		{
			//SystemNotice("!");
			SystemNotice(RES_STRING(GM_CHARACTERSUPERCMD_CPP_00028));
			return FALSE;
		}
		//SystemNotice("[%s]%d%d!", pCCha->GetLogName(), sAttrID, pCCha->getAttr(sAttrID));
		SystemNotice(RES_STRING(GM_CHARACTERSUPERCMD_CPP_00029), pCCha->GetLogName(), sAttrID, pCCha->getAttr(sAttrID));
		ToLogService("common", "ChaID: {}, ChaName: {}, CMD: {}, Param: {}", GetPlayer()->GetID(), GetName(), pszCmd, pszParam);
		return TRUE;
	}
	else if (!strcmp(szComHead, "forge")) // 
	{
		int n = Util_ResolveTextLine(szComParam, strList, 10, ',');
		char	chAddLv = Str2Int(strList[0]);
		short	sGridID = 0;
		if (n == 2)
			sGridID = Str2Int(strList[1]);

		SItemGrid *pItemCont = m_CKitbag.GetGridContByID(sGridID);
		if (pItemCont)
		{
			CItemRecord	*pCItemRec = GetItemRecordInfo(pItemCont->sID);
			if (pCItemRec && pCItemRec->chForgeLv > 0)
			{
				m_CKitbag.SetChangeFlag(false);
				pItemCont->AddForgeLv(chAddLv);
				ItemForge(pItemCont, chAddLv);
				SynKitbagNew(enumSYN_KITBAG_FORGES);
				ToLogService("common", "ChaID: {}, ChaName: {}, CMD: {}, Param: {}", GetPlayer()->GetID(), GetName(), pszCmd, pszParam);
				return TRUE;
			}
		}
		return FALSE;
	}
	else if (!strcmp(szComHead, g_Command.m_cSkill)) // 
	{
		int n = Util_ResolveTextLine(szComParam, strList, 10, ',');
		short	sID = Str2Int(strList[0]);
		char	chLv = Str2Int(strList[1]);
		bool	bLimit = true;
		if (n == 3 && Str2Int(strList[2]) == 0)
			bLimit = false;

		if (!GetPlayer()->GetMainCha()->LearnSkill(sID, chLv, true, false, bLimit))
		{
			//SystemNotice(" %d %d.!", sID, chLv);
			SystemNotice(RES_STRING(GM_CHARACTERSUPERCMD_CPP_00030), sID, chLv);
			return FALSE;
		}
		ToLogService("common", "ChaID: {}, ChaName: {}, CMD: {}, Param: {}", GetPlayer()->GetID(), GetName(), pszCmd, pszParam);
		return TRUE;
	}
	else if (!strcmp(szComHead, g_Command.m_cDelitem))
	{
		int n = Util_ResolveTextLine(szComParam, strList, 10, ',');
		if (n == 6)
		{
			long	lItemID = Str2Int(strList[0]);
			long	lItemNum = Str2Int(strList[1]);
			char	chFromType = Str2Int(strList[2]);
			short	sFromID = Str2Int(strList[3]);
			char	chToType = Str2Int(strList[4]);
			char	chForcible = Str2Int(strList[5]);
			if (Cmd_RemoveItem(lItemID, lItemNum, chFromType, sFromID, chToType, 0, true, chForcible) != enumITEMOPT_SUCCESS)
			{
				//SystemNotice("!");
				SystemNotice(RES_STRING(GM_CHARACTERSUPERCMD_CPP_00031));
				return FALSE;
			}
			ToLogService("common", "ChaID: {}, ChaName: {}, CMD: {}, Param: {}", GetPlayer()->GetID(), GetName(), pszCmd, pszParam);
			return TRUE;
		}
		return FALSE;
	}
	else if (!strcmp(szComHead, g_Command.m_cLua)) // GameServer
	{
		luaL_dostring(g_pLuaState, szComParam);
		//C_PRINT("%s: lua %s\n", GetName(), pszParam);
		ToLogService("common", "ChaID: {}, ChaName: {}, CMD: {}, Param: {}", GetPlayer()->GetID(), GetName(), pszCmd, pszParam);
		return TRUE;
	}
	else if (!strcmp(szComHead, g_Command.m_cLuaall)) // GameServer
	{
		//  :  Lua-   GameServer
		auto WtPk = net::msg::serialize(net::msg::MmDoStringMessage{GetID(), szComParam});
		ReflectINFof(this, WtPk);
		C_PRINT("%s: lua_all %s\n", GetName(), pszParam);
		ToLogService("common", "ChaID: {}, ChaName: {}, CMD: {}, Param: {}", GetPlayer()->GetID(), GetName(), pszCmd, pszParam);
		return TRUE;
	}
	else if (!strcmp(szComHead, "setping"))
	{
		int n = Util_ResolveTextLine(szComParam, strList, 10, ',');
		if (n != 1)
		{
			//SystemNotice("");
			SystemNotice(RES_STRING(GM_CHARACTERSUPERCMD_CPP_00032));
			return FALSE;
		}
		Long	lPing = Str2Int(strList[0]);
		m_lSetPing = lPing;
		SendPreMoveTime();
		ToLogService("common", "ChaID: {}, ChaName: {}, CMD: {}, Param: {}", GetPlayer()->GetID(), GetName(), pszCmd, pszParam);
		return TRUE;
	}
	else if (!strcmp(szComHead, "getping"))
	{
		//SystemNotice("ping%d", m_SMoveInit.usPing);
		SystemNotice(RES_STRING(GM_CHARACTERSUPERCMD_CPP_00033), m_SMoveInit.usPing);
		ToLogService("common", "ChaID: {}, ChaName: {}, CMD: {}, Param: {}", GetPlayer()->GetID(), GetName(), pszCmd, pszParam);
		return TRUE;
	}
	else if (!strcmp(szComHead, "senddata"))
	{
		int n = Util_ResolveTextLine(szComParam, strList, 10, ',');
		if (n != 2)
		{
			//SystemNotice("");
			SystemNotice(RES_STRING(GM_CHARACTERSUPERCMD_CPP_00032));
			return FALSE;
		}

		m_timerNetSendFreq.SetInterval((DWORD)Str2Int(strList[0]));
		m_ulNetSendLen = (uLong)Str2Int(strList[1]);
		ToLogService("common", "ChaID: {}, ChaName: {}, CMD: {}, Param: {}", GetPlayer()->GetID(), GetName(), pszCmd, pszParam);
		return TRUE;
	}
	else if (!strcmp(szComHead, "setpinginfo"))
	{
		int n = Util_ResolveTextLine(szComParam, strList, 10, ',');
		if (n != 2)
		{
			//SystemNotice("");
			SystemNotice(RES_STRING(GM_CHARACTERSUPERCMD_CPP_00032));
			return FALSE;
		}

		m_timerPing.SetInterval((DWORD)Str2Int(strList[0]));
		m_ulPingDataLen = (uLong)Str2Int(strList[1]);
		ToLogService("common", "ChaID: {}, ChaName: {}, CMD: {}, Param: {}", GetPlayer()->GetID(), GetName(), pszCmd, pszParam);
		return TRUE;
	}
	else if (!strcmp(szComHead, g_Command.m_cAddkb)) // [,WorldID]
	{
		int n = Util_ResolveTextLine(szComParam, strList, 10, ',');
		short	sAddCap = Str2Int(strList[0]);
		CCharacter	*pCCha = this;
		if (n == 2)
			pCCha = g_pGameApp->FindChaByID(Str2Int(strList[1]));
		if (!pCCha)
		{
			//SystemNotice(".!");
			SystemNotice(RES_STRING(GM_CHARACTERSUPERCMD_CPP_00034));
			return FALSE;
		}

		if (!pCCha->AddKitbagCapacity(sAddCap))
		{
			//SystemNotice(" %s .!", pCCha->GetName());
			SystemNotice(RES_STRING(GM_CHARACTERSUPERCMD_CPP_00035), pCCha->GetName());
			return FALSE;
		}
		else
		{
			//pCCha->SystemNotice(" %d.!", pCCha->m_CKitbag.GetCapacity());
			pCCha->SystemNotice(RES_STRING(GM_CHARACTERSUPERCMD_CPP_00036), pCCha->m_CKitbag.GetCapacity());
			if (pCCha != this)
				//SystemNotice(" %s  %d.!", pCCha->GetName(), pCCha->m_CKitbag.GetCapacity());
				SystemNotice(RES_STRING(GM_CHARACTERSUPERCMD_CPP_00037), pCCha->GetName(), pCCha->m_CKitbag.GetCapacity());
			ToLogService("common", "ChaID: {}, ChaName: {}, CMD: {}, Param: {}", GetPlayer()->GetID(), GetName(), pszCmd, pszParam);
			return TRUE;
		}
	}
	else if (!strcmp(szComHead, "itemvalid")) // 
	{
		int n = Util_ResolveTextLine(szComParam, strList, 10, ',');
		if (n != 2)
		{
			//SystemNotice("!");
			SystemNotice(RES_STRING(GM_CHARACTERSUPERCMD_CPP_00038));
			return FALSE;
		}
		short	sPosID = Str2Int(strList[0]);
		bool	bValid = Str2Int(strList[1]) != 0 ? true : false;

		if (!SetKitbagItemValid(sPosID, bValid))
		{
			//SystemNotice("!");
			SystemNotice(RES_STRING(GM_CHARACTERSUPERCMD_CPP_00039));
			return FALSE;
		}
		else
			ToLogService("common", "ChaID: {}, ChaName: {}, CMD: {}, Param: {}", GetPlayer()->GetID(), GetName(), pszCmd, pszParam);
			return TRUE;
	}
	else if (!strcmp(szComHead, "scroll")) {
	g_pGameApp->ScrollNotice(szComParam, 2);
		return TRUE;
	}
	else if (!strcmp(szComHead, "generatecharbag")) {					
	const char *charname = szComParam;
	CCharacter *player = g_pGameApp->FindChaByName(charname);
	if (!player) return FALSE;
	CKitbag inventory[2];
	inventory[0] = player->m_CKitbag;
	inventory[1] = *player->GetPlayer()->GetBank();
	char buf2[50];
	char buf3[1024];
	char buf4[300];
	sprintf(buf2, "%s.txt", charname);
	ofstream myfile;
	myfile.open(buf2, ios::out | ios::app);
	if (myfile.is_open())
	{
		for (int p = 0; p < 2; p++) {
			if (p == 0) {
				myfile << "INVENTORY ITEMS: \n";
			}
			else {
				myfile << "BANK ITEMS: \n";
			}
			for (int i = 0; i < inventory[p].GetCapacity(); i++) {
				if (inventory[p].GetGridContByID(i)) {
					
					sprintf(buf3, "Item Name: %s; Item ID: %d; Position ID: %d\n;", GetItemRecordInfo(inventory[p].GetGridContByID(i)->sID)->szName.c_str(), inventory[p].GetGridContByID(i)->sID, i);
					myfile << buf3;
					sprintf(buf4, "STR (raw): %d. STR (%%): %2.2f\n", inventory[p].GetGridContByID(i)->GetInstAttr(26), inventory[p].GetGridContByID(i)->GetInstAttr(1));
					myfile << buf4;
					sprintf(buf4, "AGI (raw): %d. AGI (%%): %2.2f\n", inventory[p].GetGridContByID(i)->GetInstAttr(27), inventory[p].GetGridContByID(i)->GetInstAttr(2));
					myfile << buf4;
					sprintf(buf4, "DEX (raw): %d. DEX (%%): %2.2f\n", inventory[p].GetGridContByID(i)->GetInstAttr(28), inventory[p].GetGridContByID(i)->GetInstAttr(3));
					myfile << buf4;
					sprintf(buf4, "CON (raw): %d. CON (%%): %2.2f\n", inventory[p].GetGridContByID(i)->GetInstAttr(29), inventory[p].GetGridContByID(i)->GetInstAttr(4));
					myfile << buf4;
					sprintf(buf4, "STA (raw): %d. STA (%%): %2.2f\n", inventory[p].GetGridContByID(i)->GetInstAttr(30), inventory[p].GetGridContByID(i)->GetInstAttr(5));
					myfile << buf4;
					sprintf(buf4, "LUCK (raw): %d. LUCK (%%): %2.2f\n", inventory[p].GetGridContByID(i)->GetInstAttr(31), inventory[p].GetGridContByID(i)->GetInstAttr(6));
					myfile << buf4;
					sprintf(buf4, "ASPD (raw): %d. ASPD(%%): %2.2f\n", inventory[p].GetGridContByID(i)->GetInstAttr(32), inventory[p].GetGridContByID(i)->GetInstAttr(7));
					myfile << buf4;
					sprintf(buf4, "ADIS (raw): %d. ADIS (%%): %2.2f\n", inventory[p].GetGridContByID(i)->GetInstAttr(33), inventory[p].GetGridContByID(i)->GetInstAttr(8));
					myfile << buf4;
					sprintf(buf4, "MNATK (raw): %d. MNATK (%%): %2.2f\n", inventory[p].GetGridContByID(i)->GetInstAttr(34), inventory[p].GetGridContByID(i)->GetInstAttr(9));
					myfile << buf4;
					sprintf(buf4, "MXATK (raw): %d. MXATK (%%): %2.2f\n", inventory[p].GetGridContByID(i)->GetInstAttr(35), inventory[p].GetGridContByID(i)->GetInstAttr(10));
					myfile << buf4;
					sprintf(buf4, "DEF (raw): %d. DEF(%%): %2.2f\n", inventory[p].GetGridContByID(i)->GetInstAttr(36), inventory[p].GetGridContByID(i)->GetInstAttr(11));
					myfile << buf4;
					sprintf(buf4, "MXHP (raw): %d. MXHP(%%): %2.2f\n", inventory[p].GetGridContByID(i)->GetInstAttr(37), inventory[p].GetGridContByID(i)->GetInstAttr(12));
					myfile << buf4;
					sprintf(buf4, "MXSP (raw): %d. MXSP(%%): %2.2f\n", inventory[p].GetGridContByID(i)->GetInstAttr(38), inventory[p].GetGridContByID(i)->GetInstAttr(13));
					myfile << buf4;
					sprintf(buf4, "FLEE (raw): %d. FLEE(%%): %2.2f\n", inventory[p].GetGridContByID(i)->GetInstAttr(39), inventory[p].GetGridContByID(i)->GetInstAttr(14));
					myfile << buf4;
					sprintf(buf4, "HIT (raw): %d. HIT(%%): %2.2f\n", inventory[p].GetGridContByID(i)->GetInstAttr(40), inventory[p].GetGridContByID(i)->GetInstAttr(15));
					myfile << buf4;
					sprintf(buf4, "CRT (raw): %d. CRT(%%): %2.2f\n", inventory[p].GetGridContByID(i)->GetInstAttr(41), inventory[p].GetGridContByID(i)->GetInstAttr(16));
					myfile << buf4;
					sprintf(buf4, "MF (raw): %d. MF(%): %2.2f\n", inventory[p].GetGridContByID(i)->GetInstAttr(42), inventory[p].GetGridContByID(i)->GetInstAttr(17));
					myfile << buf4;
					sprintf(buf4, "HREC (raw): %d. HREC(%): %2.2f\n", inventory[p].GetGridContByID(i)->GetInstAttr(43), inventory[p].GetGridContByID(i)->GetInstAttr(18));
					myfile << buf4;
					sprintf(buf4, "SREC (raw): %d. SREC(%): %2.2f\n", inventory[p].GetGridContByID(i)->GetInstAttr(44), inventory[p].GetGridContByID(i)->GetInstAttr(19));
					myfile << buf4;
					sprintf(buf4, "MSPD (raw): %d. MSPD(%): %2.2f\n", inventory[p].GetGridContByID(i)->GetInstAttr(45), inventory[p].GetGridContByID(i)->GetInstAttr(20));
					myfile << buf4;
					sprintf(buf4, "COL (raw): %d. COL(%): %2.2f\n", inventory[p].GetGridContByID(i)->GetInstAttr(46), inventory[p].GetGridContByID(i)->GetInstAttr(21));
					myfile << buf4;
					sprintf(buf4, "PDEF (raw): %d. PDEF(%): %2.2f\n\n", inventory[p].GetGridContByID(i)->GetInstAttr(47), inventory[p].GetGridContByID(i)->GetInstAttr(22));
					myfile << buf4;

				}
			}
			myfile << "=========================================\n";
		}
		myfile << "EQUIPMENTS: \n";
		for (int i = 0; i < 34; i++) {
			if (player->m_SChaPart.SLink[i].sID) {
				sprintf(buf3, "Item ID: %d; SLink (position) ID: %d\n;", player->m_SChaPart.SLink[i].sID, i);
				myfile << buf3;
				sprintf(buf4, "STR (raw): %d. STR (%%): %2.2f\n", player->m_SChaPart.SLink[i].GetInstAttr(26), player->m_SChaPart.SLink[i].GetInstAttr(1));
				myfile << buf4;
				sprintf(buf4, "AGI (raw): %d. AGI (%%): %2.2f\n", player->m_SChaPart.SLink[i].GetInstAttr(27), player->m_SChaPart.SLink[i].GetInstAttr(2));
				myfile << buf4;
				sprintf(buf4, "DEX (raw): %d. DEX (%%): %2.2f\n", player->m_SChaPart.SLink[i].GetInstAttr(28), player->m_SChaPart.SLink[i].GetInstAttr(3));
				myfile << buf4;
				sprintf(buf4, "CON (raw): %d. CON (%%): %2.2f\n", player->m_SChaPart.SLink[i].GetInstAttr(29), player->m_SChaPart.SLink[i].GetInstAttr(4));
				myfile << buf4;
				sprintf(buf4, "STA (raw): %d. STA (%%): %2.2f\n", player->m_SChaPart.SLink[i].GetInstAttr(30), player->m_SChaPart.SLink[i].GetInstAttr(5));
				myfile << buf4;
				sprintf(buf4, "LUCK (raw): %d. LUCK (%%): %2.2f\n", player->m_SChaPart.SLink[i].GetInstAttr(31), player->m_SChaPart.SLink[i].GetInstAttr(6));
				myfile << buf4;
				sprintf(buf4, "ASPD (raw): %d. ASPD(%%): %2.2f\n", player->m_SChaPart.SLink[i].GetInstAttr(32), player->m_SChaPart.SLink[i].GetInstAttr(7));
				myfile << buf4;
				sprintf(buf4, "ADIS (raw): %d. ADIS (%%): %2.2f\n", player->m_SChaPart.SLink[i].GetInstAttr(33), player->m_SChaPart.SLink[i].GetInstAttr(8));
				myfile << buf4;
				sprintf(buf4, "MNATK (raw): %d. MNATK (%%): %2.2f\n", player->m_SChaPart.SLink[i].GetInstAttr(34), player->m_SChaPart.SLink[i].GetInstAttr(9));
				myfile << buf4;
				sprintf(buf4, "MXATK (raw): %d. MXATK (%%): %2.2f\n", player->m_SChaPart.SLink[i].GetInstAttr(35), player->m_SChaPart.SLink[i].GetInstAttr(10));
				myfile << buf4;
				sprintf(buf4, "DEF (raw): %d. DEF(%%): %2.2f\n", player->m_SChaPart.SLink[i].GetInstAttr(36), player->m_SChaPart.SLink[i].GetInstAttr(11));
				myfile << buf4;
				sprintf(buf4, "MXHP (raw): %d. MXHP(%%): %2.2f\n", player->m_SChaPart.SLink[i].GetInstAttr(37), player->m_SChaPart.SLink[i].GetInstAttr(12));
				myfile << buf4;
				sprintf(buf4, "MXSP (raw): %d. MXSP(%%): %2.2f\n", player->m_SChaPart.SLink[i].GetInstAttr(38), player->m_SChaPart.SLink[i].GetInstAttr(13));
				myfile << buf4;
				sprintf(buf4, "FLEE (raw): %d. FLEE(%%): %2.2f\n", player->m_SChaPart.SLink[i].GetInstAttr(39), player->m_SChaPart.SLink[i].GetInstAttr(14));
				myfile << buf4;
				sprintf(buf4, "HIT (raw): %d. HIT(%%): %2.2f\n", player->m_SChaPart.SLink[i].GetInstAttr(40), player->m_SChaPart.SLink[i].GetInstAttr(15));
				myfile << buf4;
				sprintf(buf4, "CRT (raw): %d. CRT(%%): %2.2f\n", player->m_SChaPart.SLink[i].GetInstAttr(41), player->m_SChaPart.SLink[i].GetInstAttr(16));
				myfile << buf4;
				sprintf(buf4, "MF (raw): %d. MF(%): %2.2f\n", player->m_SChaPart.SLink[i].GetInstAttr(42), player->m_SChaPart.SLink[i].GetInstAttr(17));
				myfile << buf4;
				sprintf(buf4, "HREC (raw): %d. HREC(%): %2.2f\n", player->m_SChaPart.SLink[i].GetInstAttr(43), player->m_SChaPart.SLink[i].GetInstAttr(18));
				myfile << buf4;
				sprintf(buf4, "SREC (raw): %d. SREC(%): %2.2f\n", player->m_SChaPart.SLink[i].GetInstAttr(44), player->m_SChaPart.SLink[i].GetInstAttr(19));
				myfile << buf4;
				sprintf(buf4, "MSPD (raw): %d. MSPD(%): %2.2f\n", player->m_SChaPart.SLink[i].GetInstAttr(45), player->m_SChaPart.SLink[i].GetInstAttr(20));
				myfile << buf4;
				sprintf(buf4, "COL (raw): %d. COL(%): %2.2f\n", player->m_SChaPart.SLink[i].GetInstAttr(46), player->m_SChaPart.SLink[i].GetInstAttr(21));
				myfile << buf4;
				sprintf(buf4, "PDEF (raw): %d. PDEF(%): %2.2f\n\n", player->m_SChaPart.SLink[i].GetInstAttr(47), player->m_SChaPart.SLink[i].GetInstAttr(22));
				myfile << buf4;
			}
	
		}
		myfile.close();
		}
	SystemNotice(".txt created!");
		return TRUE;
	}
	else if (!strcmp(szComHead, "editcharbag")) {
		int n = Util_ResolveTextLine(szComParam, strList, 16, ',');
		const char* charname = strList[0].c_str();
		int positionID = Str2Int(strList[1]);
		int ATTR_TYPE = Str2Int(strList[2]);
		int ATTR_VALUE = Str2Int(strList[3]);
		int TYPE = Str2Int(strList[4]);
		CCharacter* player = g_pGameApp->FindChaByName(charname);
		if (!player) return FALSE;
		char buffer[32];
		bool p;
		if (TYPE == 1) {
			p = player->m_CKitbag.GetGridContByID(positionID)->SetInstAttr(ATTR_TYPE, ATTR_VALUE);
		}
		else if (TYPE == 2) {
		 p = player->m_SChaPart.SLink[positionID].SetInstAttr(ATTR_TYPE, ATTR_VALUE);
		}
		else if (TYPE == 3) {
			p = player->GetPlayer()->GetBank()->GetGridContByID(positionID)->SetInstAttr(ATTR_TYPE, ATTR_VALUE);
		}
		
		sprintf(buffer, "Attribute updated (1 is ok, 0 is not ok) =  %d", p);
		SystemNotice(buffer);
		player->SynKitbagNew(enumSYN_KITBAG_PICK);
		return TRUE;
	}

	


	SystemNotice("Invalid command!");
	return FALSE;
}

// 
void CCharacter::DoCommand_CheckStatus(cChar *pszCommand, uLong ulLen)
{
	Char szComHead[256], szComParam[256];
	std::string	strList[10];

	int n = Util_ResolveTextLine(pszCommand, strList, 10, ' ');
	strncpy(szComHead, strlwr((char *)strList[0].c_str()), 256 - 1);
	strncpy(szComParam, strList[1].c_str(), 256 - 1);

	string strCmd   = szComHead;
	string strParam = szComParam;

	if(strCmd=="game_status")	 // gameserver
	{
		char szInfo[255];
		sprintf(szInfo, "fps:%d tick:%d player:%d mgr:%d\n", g_pGameApp->m_dwFPS, 
			                     g_pGameApp->m_dwRunCnt, g_pGameApp->m_dwPlayerCnt,
								 g_pGameApp->m_dwActiveMgrUnit);
		SystemNotice(szInfo);
	}
	else if (strCmd=="ping_game") // GameServerping
	{
		int n = Util_ResolveTextLine(szComParam, strList, 10, ',');
		//  :   
		auto WtPk = net::msg::serialize(net::msg::MmQueryChaPingMessage{GetID(), strList[0]});
		ReflectINFof(this, WtPk);
	}
}

	
// NPC 
void NPC_PrivateTalk(CCharacter *pCha, CCharacter *pNPC, const char *pszText)
{
	//  :   NPC
	auto wpk = net::msg::serialize(net::msg::McSayMessage{pNPC->m_ID, pszText, 0});
	pCha->ReflectINFof(pCha, wpk);
}		

// 
void CCharacter::HandleHelp(cChar *pszCommand, uLong ulLen)
{
	if(!pszCommand)           return;
	
	if(ulLen==0 || strlen(pszCommand)==0) 
	{
		//SystemNotice( "?" );
		SystemNotice( RES_STRING(GM_CHARACTERSUPERCMD_CPP_00040) );
		return;
	}

	// npc, 
	if(GetSubMap()==NULL) return;
	if(strcmp(GetSubMap()->GetName(), "garner")!=0) return;

	int x = this->GetPos().x / 100;
	int y = this->GetPos().y / 100;

	if(g_HelpNPCList.size()==0) return;

	CCharacter *pNPC1 = g_HelpNPCList.front();
	if(pNPC1==NULL)
	{
		//LG("error", "NPC\n");
		ToLogService("errors", LogLevel::Error, "inquire NPC is empty");
		return;
	}

	if(! ( abs(x - 2222) < 4 && abs(y - 2888) < 4) )
	{
		//SystemNotice( "!" );
		SystemNotice( RES_STRING(GM_CHARACTERSUPERCMD_CPP_00041) );
		return;
	}
	
	std::string	strList[3];
	int n = Util_ResolveTextLine(pszCommand, strList, 3, ' ');
	
	const char *pszHelp = FindHelpInfo(strList[0].c_str());
	
	
	char szTip[128]; 
	sprintf(szTip, RES_STRING(GM_CHARACTERSUPERCMD_CPP_00042), strList[0].c_str()); 
	if(strList[0]=="time")	// 
	{
		//SystemNotice( szTip );
		//GetCurrentTime()
		//SystemNotice( "force is strong with this one!");
	}
	//else if(strList[0]=="ryan" || strList[0]=="")
	else if(strList[0]=="ryan" || strList[0]==RES_STRING(GM_CHARACTERSUPERCMD_CPP_00043))
	{
		SystemNotice( szTip );
		NPC_PrivateTalk(this, pNPC1, "force is strong with this one!");
		return;
	}
	
	if(pszHelp==NULL)
	{
		SystemNotice( szTip );
		//NPC_PrivateTalk(this, pNPC1, ",,!");
		NPC_PrivateTalk(this, pNPC1, RES_STRING(GM_CHARACTERSUPERCMD_CPP_00044));
	}
	else
	{
		//if(strcmp(GetName(), "")==0) // ,
		if(strcmp(GetName(), RES_STRING(GM_CHARACTERSUPERCMD_CPP_00043))==0) // ,
		{
			SystemNotice( szTip );
			NPC_PrivateTalk(this, pNPC1, pszHelp);
		}
		//else if(TakeMoney("", 100))
		else if(TakeMoney(RES_STRING(GM_CHARACTERSUPERCMD_CPP_00045), 100))
		{
			SystemNotice( szTip );
			NPC_PrivateTalk(this, pNPC1, pszHelp);
		}
		else
		{
			SystemNotice( szTip );
			//NPC_PrivateTalk(this, pNPC1, ", Money Talk!" );
			NPC_PrivateTalk(this, pNPC1, RES_STRING(GM_CHARACTERSUPERCMD_CPP_00046) );
		}
	}
}
