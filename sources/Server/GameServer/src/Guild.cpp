#include "stdafx.h"
#include "Guild.h"
#include "GameDB.h"
#include "util.h"
#include "GameCommon.h"
#include "GameApp.h"
#include "LuaAPI.h"


BOOL Guild::lua_CreateGuild(CCharacter* pCha)//,1-,2-
{
	if(!(pCha->GetPlayer()->m_GuildState & emGuildGetName))//
	{
		pCha->GetPlayer()->m_GuildState |= emGuildGetName;

		//  :     
		auto l_wpk = net::msg::serializeMcGuildGetNameCmd();
		pCha->ReflectINFof(pCha,l_wpk);
	}
	return TRUE;
}
void Guild::cmd_CreateGuild(CCharacter* pCha, bool confirm, cChar *guildname, cChar *passwd)
{
	if(!(pCha->GetPlayer()->m_GuildState & emGuildGetName))
	{
		return;
	}else
	{
		pCha->GetPlayer()->m_GuildState &= ~emGuildGetName;
		if(!guildname || !passwd || !confirm)
		{
			return;
		}

        if(pCha->m_CKitbag.IsPwdLocked())
        {
            //pCha->SystemNotice(",");
			pCha->SystemNotice(RES_STRING(GM_GUILD_CPP_00001));
			return;
        }

		//add by ALLEN 2007-10-16
		if(pCha->IsReadBook())
        {
            //pCha->SystemNotice(",");
			pCha->SystemNotice(RES_STRING(GM_GUILD_CPP_00002));
			return;
        }
        
		short l_len	=short(strlen(guildname));
		if(l_len <1)
		{
			//pCha->SystemNotice("");
			pCha->SystemNotice(RES_STRING(GM_GUILD_CPP_00003));
			return;
		}else if(l_len >16)
		{
			//pCha->SystemNotice("");
			pCha->SystemNotice(RES_STRING(GM_GUILD_CPP_00004));
			return;
		}else if(strlen(passwd) >16)
		{
			//pCha->SystemNotice("");
			pCha->SystemNotice(RES_STRING(GM_GUILD_CPP_00005));
			return;
		}
		//else if(!IsValidGuildName(guildname,l_len))
		//{
		//	pCha->SystemNotice("Guild name includes illegal character");
		//	return;
		//}
	}
	{
		auto result = g_luaAPI.CallR<int>("AskGuildItem", pCha);
		if (!result.value_or(0))
			return;
	}
	char l_guildname[32];
	strcpy(l_guildname,guildname);
	long l_guildid =game_db.CreateGuild(*pCha,l_guildname,passwd);
	if(!l_guildid)
	{
		return;
	}

	//
	pCha->SetGuildName( l_guildname );//
	pCha->SetGuildID( l_guildid	);		//ID
	pCha->SetGuildState( 0 );
	pCha->guildPermission = emGldPermMax;
	pCha->SyncGuildInfo();

	//pCha->setAttr(ATTR_GUILD,l_guildid);		
	//pCha->setAttr(ATTR_GUILD_TYPE,pCha->GetPlayer()->m_cGuildType);//
	//pCha->setAttr(ATTR_GUILD_STATE,0);			//

	//luaBegin
	g_luaAPI.Call("DeductGuildItem", pCha);
	//luaEnd
	
	//pCha->SystemNotice(".");
	pCha->SystemNotice(RES_STRING(GM_GUILD_CPP_00006));

	char l_str[512];
	{
		auto _s = std::format("Player [{}] has founded [{}] guild. All Pirates may now apply!",
			pCha->GetName(), l_guildname);
		std::strncpy(l_str, _s.c_str(), sizeof(l_str) - 1);
		l_str[sizeof(l_str) - 1] = 0;
	}
	
	g_pGameApp->ScrollNotice(l_str, 2);
}
BOOL Guild::lua_ListAllGuild(CCharacter* pCha)			//NPC
{
	cmd_ListAllGuild(pCha);
	return TRUE;
}
void Guild::cmd_ListAllGuild(CCharacter* pCha)			//20
{
	game_db.ListAllGuild(*pCha,7);
}
void Guild::cmd_GuildTryFor(CCharacter* pCha, uLong guildid)			//
{
	if(!guildid)
	{
		pCha->SystemNotice(RES_STRING(GM_GUILD_CPP_00007));
	}else if(guildid >199)
	{
		pCha->SystemNotice(RES_STRING(GM_GUILD_CPP_00008));
	}else
	{
		{
			auto result = g_luaAPI.CallR<int>("AskJoinGuild", pCha);
			if (!result.value_or(0))
				return;
		}
		game_db.GuildTryFor(*pCha,guildid);
	}
}
void Guild::cmd_GuildTryForComfirm(CCharacter* pCha, char IsReplace)
{
	if(pCha->GetPlayer()->m_GuildState & emGuildReplaceOldTry)
	{
		if(IsReplace ==1)
		{
			game_db.GuildTryForConfirm(*pCha,pCha->GetPlayer()->m_lTempGuildID);
		}
		pCha->GetPlayer()->m_GuildState &= ~emGuildReplaceOldTry;
	}
}
void Guild::cmd_GuildListTryPlayer(CCharacter* pCha)
{

	game_db.GuildListTryPlayer(*pCha,7);
}
void Guild::cmd_GuildApprove(CCharacter* pCha,uLong chaid)
{

	game_db.GuildApprove(*pCha,chaid);
}
void Guild::cmd_GuildReject(CCharacter* pCha,uLong chaid)
{

	game_db.GuildReject(*pCha,chaid);
}
void Guild::cmd_GuildKick(CCharacter* pCha,uLong chaid)
{

	game_db.GuildKick(*pCha,chaid);
}
void Guild::cmd_GuildLeave(CCharacter* pCha)
{

	game_db.GuildLeave(*pCha);
}
void Guild::cmd_GuildDisband(CCharacter* pCha,cChar *passwd)
{

	game_db.GuildDisband(*pCha,passwd);
}
void Guild::cmd_GuildMotto(CCharacter* pCha,cChar *motto)
{

	game_db.GuildMotto(*pCha,motto);
}
void Guild::cmd_GuildChallenge( CCharacter* pCha, BYTE byLevel, DWORD dwMoney )
{
    if(pCha->m_CKitbag.IsPwdLocked())
    {
        //pCha->SystemNotice(",");
		pCha->SystemNotice(RES_STRING(GM_GUILD_CPP_00009));
        return;
    }

	//add by ALLEN 2007-10-16
	if(pCha->IsReadBook())
    {
       // pCha->SystemNotice(",");
		 pCha->SystemNotice(RES_STRING(GM_GUILD_CPP_00010));
        return;
    }
    
	game_db.Challenge( *pCha, byLevel, dwMoney );
}
void Guild::cmd_GuildLeizhu( CCharacter* pCha, BYTE byLevel, DWORD dwMoney )
{
	game_db.Leizhu( *pCha, byLevel, dwMoney );
}
void Guild::cmd_PMDisband(CCharacter *pCha)
{
	//pCha->m_CChaAttr.ResetChangeFlag();

	//pCha->setAttr(ATTR_GUILD,0);			//ID
	//pCha->setAttr(ATTR_GUILD_STATE,0);		//

	//pCha->SynAttr(enumATTRSYN_TRADE);

	//pCha->SetGuildName("");
}
