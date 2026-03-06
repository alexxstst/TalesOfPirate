#pragma once

//Created By Andor.Zhang 2005.4.19
#ifndef GUILD_H
#define GUILD_H
//#include "stdafx.h"

#include "character.h"

class Guild
{
public:
	static BOOL lua_CreateGuild(CCharacter* pCha);	//,0-,1-
	static BOOL lua_ListAllGuild(CCharacter* pCha);	//NPC

	static void cmd_CreateGuild(CCharacter* pCha, bool confirm, cChar *guildname, cChar *passwd);
	static void cmd_ListAllGuild(CCharacter* pCha);	//20
	static void cmd_GuildTryFor(CCharacter* pCha, uLong guildid);	//NPC
	static void cmd_GuildTryForComfirm(CCharacter* pCha, char IsReplace);
	static void cmd_GuildListTryPlayer(CCharacter* pCha);
	static void cmd_GuildApprove(CCharacter* pCha,uLong chaid);
	static void cmd_GuildReject(CCharacter* pCha,uLong chaid);
	static void cmd_GuildKick(CCharacter* pCha,uLong chaid);
	static void	cmd_GuildLeave(CCharacter* pCha);
	static void cmd_GuildDisband(CCharacter* pCha,cChar *passwd);
	static void cmd_GuildMotto(CCharacter* pCha,cChar *motto);
	static void cmd_PMDisband(CCharacter *pCha);
	static void cmd_GuildChallenge( CCharacter* pCha, BYTE byLevel, DWORD dwMoney );
	static void cmd_GuildLeizhu( CCharacter* pCha, BYTE byLevel, DWORD dwMoney );
	static bool IsValidGuildName(const char *name,unsigned short len)
	{
		const unsigned char *l_name =reinterpret_cast<const unsigned char *>(name);
		bool l_ishan	=false;
		for(unsigned short i=0;i<len;i++)
		{
			if(!l_name[i])
			{
				return false;
			}else if(l_ishan)
			{
				if(l_name[i-1] ==0xA1 && l_name[i] ==0xA1)	//
				{
					return false;
				}
				if(l_name[i] >0x3F && l_name[i] <0xFF && l_name[i] !=0x7F)
				{
					l_ishan =false;
				}else
				{
					return false;
				}
			}else if(l_name[i]>0x80 && l_name[i] <0xFF)
			{
				l_ishan	=true;
			}else if((l_name[i] >='A' && l_name[i] <='Z') ||(l_name[i] >='a' && l_name[i] <='z') ||(l_name[i] >='0' && l_name[i] <='9'))
			{

			}
			else if( l_name[i] == ' ' )
			{
			}
			else
			{
				return false;
			}
		}
		return !l_ishan;
	}
};


#endif
