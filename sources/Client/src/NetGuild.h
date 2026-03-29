#pragma once

#include "NetIF.h"
#include "PacketCmd.h"

/****************************************************************
		Packet
		:
		:
		:
		"NetPkXXX.cpp"(:Pk,cpp)
*****************************************************************/
extern void	CM_GUILD_PUTNAME(bool confirm,cChar *guildname,cChar *passwd);	//
extern void CM_GUILD_TRYFOR(uLong	guildid);	//
extern void CM_GUILD_TRYFORCFM(bool confirm);	//confirm =true;
extern void CM_GUILD_LISTTRYPLAYER();			//CMD_CM_GUILD_LISTTRYMEMBER
extern void CM_GUILD_APPROVE(uLong	chaid);		//
extern void CM_GUILD_REJECT(uLong	chaid);
extern void CM_GUILD_KICK(uLong		chaid);
extern void CM_GUILD_LEAVE();
extern void CM_GUILD_DISBAND(cChar *passwd);
extern void CM_GUILD_MOTTO(cChar *motto);
extern void CM_GUILD_CHALL( BYTE byLevel, DWORD dwMoney );
extern void CM_GUILD_LEIZHU( BYTE byLevel, DWORD dwMoney );



/****************************************************************
		Packet
		:
		:
		:Packet
		"NetXXX.cpp"(:Pk,NetPkXXX,)
*****************************************************************/
extern void NetMC_GUILD_GETNAME();
extern void NetMC_LISTGUILD_BEGIN();
extern void NetMC_LISTGUILD_END();
extern void NetMC_LISTGUILD(uLong id,cChar *name,cChar *motto,cChar *leadername,uShort memtotal,LLong exp);
extern void NetMC_GUILD_TRYFORCFM(cChar *oldgldname);
extern void NetMC_LISTTRYPLAYER_BEGIN(uLong gldid,cChar *gldname,cChar *motto,char stat,cChar *ldrname,uShort memnum,uShort maxmem,LLong exp,uLong remain);
extern void NetMC_LISTTRYPLAYER_END();
extern void NetMC_LISTTRYPLAYER(uLong chaid,cChar *chaname,cChar *job,uShort degree);

extern void NetPC_GUILD_ONLINE(uLong chaid);
extern void NetPC_GUILD_OFFLINE(uLong chaid);
extern void NetPC_GUILD_UPDATEPERM(uLong chaid,uLong perm);
extern void NetPC_GUILD_START_BEGIN(uLong guildid,cChar *guildname,uLong leaderid);
extern void NetPC_GUILD_START_END();
extern void NetPC_GUILD_START(bool online,uLong chaid,cChar *chaname,cChar *motto,cChar *job,uShort degree,uShort icon,uLong permission);
extern void NetPC_GUILD_STOP();

extern void NetPC_GUILD_ADD(bool online,uLong chaid,cChar *chaname,cChar *motto,cChar *job,uShort degree,uShort icon,uLong permission);
extern void NetPC_GUILD_DEL(uLong chaid);

extern void NetMC_GUILD_MOTTO(cChar *motto);
extern void NetMC_GUILD_INFO( DWORD dwCharID, DWORD dwGuildID, const char szGuildName[], const char szGuildMotto[],uLong chGuildPermission);
extern void NetMC_GUILD_CHALLINFO( const NET_GUILD_CHALLINFO& Info );





/****************************************************************
		Packet
		:
		:
		:NetIF::HandlePacketMessage
		"NetPkXXX.cpp"(:Pk,cpp)
*****************************************************************/
extern BOOL	MC_GUILD_GETNAME(LPRPACKET pk);
extern bool	g_listguild_begin;
extern BOOL MC_LISTGUILD(LPRPACKET pk);
extern BOOL MC_GUILD_TRYFORCFM(LPRPACKET pk);
extern BOOL MC_GUILD_LISTTRYPLAYER(LPRPACKET pk);
extern bool g_guild_start_begin;
extern BOOL PC_GUILD(LPRPACKET pk);
extern BOOL PC_GUILD_PERM(LPRPACKET pk);
extern BOOL MC_GUILD_MOTTO(LPRPACKET pk);
extern BOOL MC_GUILD_LEAVE(LPRPACKET pk);
extern BOOL MC_GUILD_KICK(LPRPACKET pk);
extern BOOL	MC_GUILD_INFO(LPRPACKET pk);
extern BOOL MC_GUILD_LISTCHALL(LPRPACKET pk);



