#include "stdafx.h"
#include "netguild.h"
#include "netcommand.h"
#include "uiguildmgr.h"
#include "uiglobalvar.h"

using namespace std;

BOOL MC_GUILD_GETNAME(LPRPACKET pk)
{
	NetMC_GUILD_GETNAME();
	return TRUE;
}

void CM_GUILD_PUTNAME(bool confirm,cChar *guildname,cChar *passwd)
{
	WPacket l_wpk	=g_NetIF->GetWPacket();
	l_wpk.WriteCmd(CMD_CM_GUILD_PUTNAME);
	l_wpk.WriteUInt8(confirm?1:0);
	l_wpk.WriteString(guildname);
	l_wpk.WriteString(passwd);

	g_NetIF->SendPacketMessage(l_wpk);
}
bool	g_listguild_begin	=false;
BOOL MC_LISTGUILD(LPRPACKET pk)
{
	uChar l_num		=pk.ReverseReadUInt8();
	if(!g_listguild_begin)
	{
		g_listguild_begin	=true;
		NetMC_LISTGUILD_BEGIN();
	}
	for(uChar i =0;i<l_num;++i)
	{
		uLong	 l_id = pk.ReadUInt32();
		cChar* l_name = pk.ReadString();
		cChar* l_motto = pk.ReadString();
		cChar* l_leadername = pk.ReadString();
		uShort	 l_memtotal = pk.ReadUInt16();
		auto	 l_exp = pk.ReadInt64();
		NetMC_LISTGUILD(l_id,l_name,l_motto,l_leadername,l_memtotal,l_exp);
	}
	if(l_num <20)
	{
		NetMC_LISTGUILD_END();
		g_listguild_begin	=false;
	}
	return TRUE;
}
void CM_GUILD_TRYFOR(uLong	guildid)	//申请加入公会
{
	WPacket l_wpk	=g_NetIF->GetWPacket();
	l_wpk.WriteCmd(CMD_CM_GUILD_TRYFOR);
	l_wpk.WriteUInt32(guildid);

	g_NetIF->SendPacketMessage(l_wpk);
}
BOOL MC_GUILD_TRYFORCFM(LPRPACKET pk)
{
	NetMC_GUILD_TRYFORCFM(pk.ReadString());
	return TRUE;
}
void CM_GUILD_TRYFORCFM(bool confirm)	//确认加入confirm =true;
{
	WPacket l_wpk	=g_NetIF->GetWPacket();
	l_wpk.WriteCmd(CMD_CM_GUILD_TRYFORCFM);
	l_wpk.WriteUInt8(confirm?1:0);

	g_NetIF->SendPacketMessage(l_wpk);
}
void CM_GUILD_LISTTRYPLAYER()			//管理
{
	WPacket	l_wpk	=g_NetIF->GetWPacket();
	l_wpk.WriteCmd(CMD_CM_GUILD_LISTTRYPLAYER);

	g_NetIF->SendPacketMessage(l_wpk);
}
BOOL MC_GUILD_LISTTRYPLAYER(LPRPACKET pk)
{
	uLong	l_gldid = pk.ReadUInt32();//guild_id 1
	cChar*	l_gldname = pk.ReadString();//guild_name 2
	cChar*	l_motto = pk.ReadString();//motto 3
	uChar	l_stat = CGuildData::eState::normal;/*pk.ReadUInt8();*/
	cChar*	l_ldrname = pk.ReadString();//atorNome 4
	uShort	l_memnum = pk.ReadUInt16();//member_total 5
	uShort	l_maxmem = pk.ReadUInt16();	//guild maxnumb 6
	auto	l_exp = pk.ReadInt64();//7 exp
	uLong	l_remain = pk.ReadUInt32();//8 always 0
	const auto level = pk.ReadUInt32();//level 9
	NetMC_LISTTRYPLAYER_BEGIN(l_gldid,l_gldname,l_motto,l_stat,l_ldrname,l_memnum,l_maxmem,l_exp,l_remain);
	
	uLong	l_num = pk.ReverseReadUInt32();
	for(uLong i =0;i<l_num;i++)
	{
		uLong	l_chaid = pk.ReadUInt32();
		cChar*	l_chaname = pk.ReadString();
		cChar*	l_job = pk.ReadString();
		uShort	l_degree = pk.ReadUInt16();
		NetMC_LISTTRYPLAYER(l_chaid,l_chaname,l_job,l_degree);
	}
	NetMC_LISTTRYPLAYER_END();
	return TRUE;
}

struct stGuildInfo
{
	bool   bOnline;
	uLong  nChaID;
	char   szChaName[65];
	char   szMotto[257];
	char   szJob[33];
	uShort sDegree;
	uShort sIcon;
	uLong sPreMission;
};


//bool g_guild_start_begin	=false;
vector<stGuildInfo> g_vecGuildInfo;

BOOL PC_GUILD_PERM(LPRPACKET pk){
	uLong	chaid	=pk.ReadUInt32();
	uLong	perm	=pk.ReadUInt32();
	NetPC_GUILD_UPDATEPERM(chaid,perm);
	return true;
}

BOOL PC_GUILD(LPRPACKET pk)
{
	uChar	l_msg =pk.ReadUInt8();
	switch(l_msg)
	{
	case MSG_GUILD_ONLINE:
		{
			NetPC_GUILD_ONLINE(pk.ReadUInt32());
		}
		break;
	case MSG_GUILD_OFFLINE:
		{
			NetPC_GUILD_OFFLINE(pk.ReadUInt32());
		}
		break;
	case MSG_GUILD_START:
		{
			uChar l_num		=pk.ReverseReadUInt8();
			uLong lPacketIndex = pk.ReverseReadUInt32(); // 报文编号(从0开始)
			static int nGuildCount = 0;

			//if(!g_guild_start_begin)
			if(lPacketIndex == 0 && l_num > 0)	// 第一个数据包
			{
				//g_guild_start_begin	=true;
				uLong	l_guildid	=pk.ReadUInt32();
				cChar*	l_guildname	=pk.ReadString();
				uLong	l_leader	=pk.ReadUInt32();
				NetPC_GUILD_START_BEGIN(l_guildid,l_guildname,l_leader);
			}
			for(uChar i =0;i<l_num;++i)
			{
				bool	l_online	=pk.ReadUInt8()?true:false;
				uLong	l_chaid		=pk.ReadUInt32();
				cChar*	l_chaname	=pk.ReadString();
				cChar*	l_motto		=pk.ReadString();
				cChar*	l_job		=pk.ReadString();
				uShort	l_degree	=pk.ReadUInt16();
				uShort	l_icon		=pk.ReadUInt16();
				uLong	l_permission=pk.ReadUInt32();
				
				stGuildInfo info;
				memset(&info, 0, sizeof(stGuildInfo));
				info.bOnline = l_online;
				info.nChaID  = l_chaid;
				info.sDegree = l_degree;
				info.sIcon   = l_icon;
				info.sPreMission = l_permission;
				strcpy(info.szChaName, l_chaname);
				strcpy(info.szJob, l_job);
				strcpy(info.szMotto, l_motto);

				g_vecGuildInfo.push_back(info);

				//NetPC_GUILD_START(l_online,l_chaid,l_chaname,l_motto,l_job,l_degree,l_icon,l_permission);
			}
			if(l_num <20)	// 最后一个数据包
			{
				nGuildCount = 20 * lPacketIndex + l_num;
			//	NetPC_GUILD_START_END();
			//	g_guild_start_begin	=false;
			}

			if(nGuildCount == (int)g_vecGuildInfo.size())
			{
				for(int i = 0; i < nGuildCount; ++i)
				{
					NetPC_GUILD_START(	g_vecGuildInfo[i].bOnline,
										g_vecGuildInfo[i].nChaID,
										g_vecGuildInfo[i].szChaName,
										g_vecGuildInfo[i].szMotto,
										g_vecGuildInfo[i].szJob,
										g_vecGuildInfo[i].sDegree,
										g_vecGuildInfo[i].sIcon,
										g_vecGuildInfo[i].sPreMission);
				}
				NetPC_GUILD_START_END();
				g_vecGuildInfo.clear();
				nGuildCount = 0;
			}

		}
		break;
	case MSG_GUILD_STOP:
		{
			NetPC_GUILD_STOP();
		}
		break;
	case MSG_GUILD_ADD:
		{
			bool	l_online	=pk.ReadUInt8()?true:false;
			uLong	l_chaid		=pk.ReadUInt32();
			cChar*	l_chaname	=pk.ReadString();
			cChar*	l_motto		=pk.ReadString();
			cChar*	l_job		=pk.ReadString();
			uShort	l_degree	=pk.ReadUInt16();
			uShort	l_icon		=pk.ReadUInt16();
			uLong	l_permission=pk.ReadUInt32();

			NetPC_GUILD_ADD(l_online,l_chaid,l_chaname,l_motto,l_job,l_degree,l_icon,l_permission);
		}
		break;
	case MSG_GUILD_DEL:
		{
			NetPC_GUILD_DEL(pk.ReadUInt32());
		}
		break;
	default:
		break;
	}
	return TRUE;
}
void CM_GUILD_APPROVE(uLong	chaid)
{
	WPacket	l_wpk	=g_NetIF->GetWPacket();
	l_wpk.WriteCmd(CMD_CM_GUILD_APPROVE);
	l_wpk.WriteUInt32(chaid);

	g_NetIF->SendPacketMessage(l_wpk);
}
void CM_GUILD_REJECT(uLong	chaid)
{
	WPacket l_wpk	=g_NetIF->GetWPacket();
	l_wpk.WriteCmd(CMD_CM_GUILD_REJECT);
	l_wpk.WriteUInt32(chaid);

	g_NetIF->SendPacketMessage(l_wpk);
}
void CM_GUILD_KICK(uLong	chaid)
{
	WPacket l_wpk	=g_NetIF->GetWPacket();
	l_wpk.WriteCmd(CMD_CM_GUILD_KICK);
	l_wpk.WriteUInt32(chaid);

	g_NetIF->SendPacketMessage(l_wpk);
}
void CM_GUILD_LEAVE()
{
	WPacket l_wpk	=g_NetIF->GetWPacket();
	l_wpk.WriteCmd(CMD_CM_GUILD_LEAVE);

	g_NetIF->SendPacketMessage(l_wpk);
}
void CM_GUILD_DISBAND(cChar *passwd)
{
	WPacket l_wpk	=g_NetIF->GetWPacket();
	l_wpk.WriteCmd(CMD_CM_GUILD_DISBAND);
	l_wpk.WriteString(passwd);

	g_NetIF->SendPacketMessage(l_wpk);
}
void CM_GUILD_MOTTO(cChar *motto)
{
	WPacket l_wpk	=g_NetIF->GetWPacket();
	l_wpk.WriteCmd(CMD_CM_GUILD_MOTTO);
	l_wpk.WriteString(motto);

	g_NetIF->SendPacketMessage(l_wpk);
}
void CM_GUILD_CHALL( BYTE byLevel, DWORD dwMoney )
{
	WPacket l_wpk = g_NetIF->GetWPacket();
	l_wpk.WriteCmd( CMD_CM_GUILD_CHALLENGE );
	l_wpk.WriteUInt8( byLevel );
	l_wpk.WriteUInt32( dwMoney );

	g_NetIF->SendPacketMessage( l_wpk );
}
void CM_GUILD_LEIZHU( BYTE byLevel, DWORD dwMoney )
{
	WPacket l_wpk = g_NetIF->GetWPacket();
	l_wpk.WriteCmd( CMD_CM_GUILD_LEIZHU );
	l_wpk.WriteUInt8( byLevel );
	l_wpk.WriteUInt32( dwMoney );

	g_NetIF->SendPacketMessage( l_wpk );
}
BOOL MC_GUILD_MOTTO(LPRPACKET pk)
{
	NetMC_GUILD_MOTTO(pk.ReadString());

	return TRUE;
}

BOOL MC_GUILD_LEAVE(LPRPACKET pk)
{
	return TRUE;
}

BOOL MC_GUILD_KICK(LPRPACKET pk)
{
	return TRUE;
}

BOOL MC_GUILD_INFO(LPRPACKET pk)
{
	DWORD dwCharID = pk.ReadUInt32();
	DWORD dwGuildID = pk.ReadUInt32();
	const char* pszGuildName = pk.ReadString();
	const char* pszGuildMotto = pk.ReadString();
	uLong chGuildPermission = pk.ReadUInt32();
	NetMC_GUILD_INFO( dwCharID, dwGuildID, pszGuildName, pszGuildMotto,chGuildPermission );
	return TRUE;
}

BOOL MC_GUILD_LISTCHALL(LPRPACKET pk)
{
	NET_GUILD_CHALLINFO Info;
	memset( &Info, 0, sizeof(Info) );

	Info.byIsLeader = pk.ReadUInt8();
	for( int i = 0; i < MAX_GUILD_CHALLLEVEL; i++ )
	{
		Info.byLevel[i] = pk.ReadUInt8();
		if( Info.byLevel[i] )
		{
			Info.byStart[i] = pk.ReadUInt8();
			strncpy( Info.szGuild[i], pk.ReadString(), 64 - 1 );
			strncpy( Info.szChall[i], pk.ReadString(), 64 - 1 );
			Info.dwMoney[i] = pk.ReadUInt32();
		}
	}

	NetMC_GUILD_CHALLINFO( Info );
	return TRUE;
}
