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
	l_wpk.WriteInt64(confirm?1:0);
	l_wpk.WriteString(guildname);
	l_wpk.WriteString(passwd);

	g_NetIF->SendPacketMessage(l_wpk);
}
bool	g_listguild_begin	=false;
BOOL MC_LISTGUILD(LPRPACKET pk)
{
	uChar l_num		=pk.ReverseReadInt64();
	if(!g_listguild_begin)
	{
		g_listguild_begin	=true;
		NetMC_LISTGUILD_BEGIN();
	}
	for(uChar i =0;i<l_num;++i)
	{
		uLong	 l_id = pk.ReadInt64();
		std::string l_name = pk.ReadString();
		std::string l_motto = pk.ReadString();
		std::string l_leadername = pk.ReadString();
		uShort	 l_memtotal = pk.ReadInt64();
		auto	 l_exp = pk.ReadInt64();
		NetMC_LISTGUILD(l_id,l_name.c_str(),l_motto.c_str(),l_leadername.c_str(),l_memtotal,l_exp);
	}
	if(l_num <20)
	{
		NetMC_LISTGUILD_END();
		g_listguild_begin	=false;
	}
	return TRUE;
}
void CM_GUILD_TRYFOR(uLong	guildid)	//������빫��
{
	WPacket l_wpk	=g_NetIF->GetWPacket();
	l_wpk.WriteCmd(CMD_CM_GUILD_TRYFOR);
	l_wpk.WriteInt64(guildid);

	g_NetIF->SendPacketMessage(l_wpk);
}
BOOL MC_GUILD_TRYFORCFM(LPRPACKET pk)
{
	NetMC_GUILD_TRYFORCFM(pk.ReadString().c_str());
	return TRUE;
}
void CM_GUILD_TRYFORCFM(bool confirm)	//ȷ�ϼ���confirm =true;
{
	WPacket l_wpk	=g_NetIF->GetWPacket();
	l_wpk.WriteCmd(CMD_CM_GUILD_TRYFORCFM);
	l_wpk.WriteInt64(confirm?1:0);

	g_NetIF->SendPacketMessage(l_wpk);
}
void CM_GUILD_LISTTRYPLAYER()			//����
{
	WPacket	l_wpk	=g_NetIF->GetWPacket();
	l_wpk.WriteCmd(CMD_CM_GUILD_LISTTRYPLAYER);

	g_NetIF->SendPacketMessage(l_wpk);
}
BOOL MC_GUILD_LISTTRYPLAYER(LPRPACKET pk)
{
	uLong	l_gldid = pk.ReadInt64();//guild_id 1
	std::string	l_gldname = pk.ReadString();//guild_name 2
	std::string	l_motto = pk.ReadString();//motto 3
	uChar	l_stat = CGuildData::eState::normal;/*pk.ReadInt64();*/
	std::string	l_ldrname = pk.ReadString();//atorNome 4
	uShort	l_memnum = pk.ReadInt64();//member_total 5
	uShort	l_maxmem = pk.ReadInt64();	//guild maxnumb 6
	auto	l_exp = pk.ReadInt64();//7 exp
	uLong	l_remain = pk.ReadInt64();//8 always 0
	const auto level = pk.ReadInt64();//level 9
	NetMC_LISTTRYPLAYER_BEGIN(l_gldid,l_gldname.c_str(),l_motto.c_str(),l_stat,l_ldrname.c_str(),l_memnum,l_maxmem,l_exp,l_remain);

	uLong	l_num = pk.ReverseReadInt64();
	for(uLong i =0;i<l_num;i++)
	{
		uLong	l_chaid = pk.ReadInt64();
		std::string	l_chaname = pk.ReadString();
		std::string	l_job = pk.ReadString();
		uShort	l_degree = pk.ReadInt64();
		NetMC_LISTTRYPLAYER(l_chaid,l_chaname.c_str(),l_job.c_str(),l_degree);
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
	uLong	chaid	=pk.ReadInt64();
	uLong	perm	=pk.ReadInt64();
	NetPC_GUILD_UPDATEPERM(chaid,perm);
	return true;
}

BOOL PC_GUILD(LPRPACKET pk)
{
	uChar	l_msg =pk.ReadInt64();
	switch(l_msg)
	{
	case MSG_GUILD_ONLINE:
		{
			NetPC_GUILD_ONLINE(pk.ReadInt64());
		}
		break;
	case MSG_GUILD_OFFLINE:
		{
			NetPC_GUILD_OFFLINE(pk.ReadInt64());
		}
		break;
	case MSG_GUILD_START:
		{
			uChar l_num		=pk.ReverseReadInt64();
			uLong lPacketIndex = pk.ReverseReadInt64(); // ���ı��(��0��ʼ)
			static int nGuildCount = 0;

			//if(!g_guild_start_begin)
			if(lPacketIndex == 0 && l_num > 0)	// ��һ�����ݰ�
			{
				//g_guild_start_begin	=true;
				uLong	l_guildid	=pk.ReadInt64();
				std::string	l_guildname	=pk.ReadString();
				uLong	l_leader	=pk.ReadInt64();
				NetPC_GUILD_START_BEGIN(l_guildid,l_guildname.c_str(),l_leader);
			}
			for(uChar i =0;i<l_num;++i)
			{
				bool	l_online	=pk.ReadInt64()?true:false;
				uLong	l_chaid		=pk.ReadInt64();
				std::string	l_chaname	=pk.ReadString();
				std::string	l_motto		=pk.ReadString();
				std::string	l_job		=pk.ReadString();
				uShort	l_degree	=pk.ReadInt64();
				uShort	l_icon		=pk.ReadInt64();
				uLong	l_permission=pk.ReadInt64();
				
				stGuildInfo info;
				memset(&info, 0, sizeof(stGuildInfo));
				info.bOnline = l_online;
				info.nChaID  = l_chaid;
				info.sDegree = l_degree;
				info.sIcon   = l_icon;
				info.sPreMission = l_permission;
				strncpy(info.szChaName, l_chaname.c_str(), sizeof(info.szChaName) - 1);
				strncpy(info.szJob, l_job.c_str(), sizeof(info.szJob) - 1);
				strncpy(info.szMotto, l_motto.c_str(), sizeof(info.szMotto) - 1);

				g_vecGuildInfo.push_back(info);

				//NetPC_GUILD_START(l_online,l_chaid,l_chaname,l_motto,l_job,l_degree,l_icon,l_permission);
			}
			if(l_num <20)	// ���һ�����ݰ�
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
			bool	l_online	=pk.ReadInt64()?true:false;
			uLong	l_chaid		=pk.ReadInt64();
			std::string	l_chaname	=pk.ReadString();
			std::string	l_motto		=pk.ReadString();
			std::string	l_job		=pk.ReadString();
			uShort	l_degree	=pk.ReadInt64();
			uShort	l_icon		=pk.ReadInt64();
			uLong	l_permission=pk.ReadInt64();

			NetPC_GUILD_ADD(l_online,l_chaid,l_chaname.c_str(),l_motto.c_str(),l_job.c_str(),l_degree,l_icon,l_permission);
		}
		break;
	case MSG_GUILD_DEL:
		{
			NetPC_GUILD_DEL(pk.ReadInt64());
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
	l_wpk.WriteInt64(chaid);

	g_NetIF->SendPacketMessage(l_wpk);
}
void CM_GUILD_REJECT(uLong	chaid)
{
	WPacket l_wpk	=g_NetIF->GetWPacket();
	l_wpk.WriteCmd(CMD_CM_GUILD_REJECT);
	l_wpk.WriteInt64(chaid);

	g_NetIF->SendPacketMessage(l_wpk);
}
void CM_GUILD_KICK(uLong	chaid)
{
	WPacket l_wpk	=g_NetIF->GetWPacket();
	l_wpk.WriteCmd(CMD_CM_GUILD_KICK);
	l_wpk.WriteInt64(chaid);

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
	l_wpk.WriteInt64( byLevel );
	l_wpk.WriteInt64( dwMoney );

	g_NetIF->SendPacketMessage( l_wpk );
}
void CM_GUILD_LEIZHU( BYTE byLevel, DWORD dwMoney )
{
	WPacket l_wpk = g_NetIF->GetWPacket();
	l_wpk.WriteCmd( CMD_CM_GUILD_LEIZHU );
	l_wpk.WriteInt64( byLevel );
	l_wpk.WriteInt64( dwMoney );

	g_NetIF->SendPacketMessage( l_wpk );
}
BOOL MC_GUILD_MOTTO(LPRPACKET pk)
{
	NetMC_GUILD_MOTTO(pk.ReadString().c_str());

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
	DWORD dwCharID = pk.ReadInt64();
	DWORD dwGuildID = pk.ReadInt64();
	std::string pszGuildName = pk.ReadString();
	std::string pszGuildMotto = pk.ReadString();
	uLong chGuildPermission = pk.ReadInt64();
	NetMC_GUILD_INFO( dwCharID, dwGuildID, pszGuildName.c_str(), pszGuildMotto.c_str(),chGuildPermission );
	return TRUE;
}

BOOL MC_GUILD_LISTCHALL(LPRPACKET pk)
{
	NET_GUILD_CHALLINFO Info;
	memset( &Info, 0, sizeof(Info) );

	Info.byIsLeader = pk.ReadInt64();
	for( int i = 0; i < MAX_GUILD_CHALLLEVEL; i++ )
	{
		Info.byLevel[i] = pk.ReadInt64();
		if( Info.byLevel[i] )
		{
			Info.byStart[i] = pk.ReadInt64();
			strncpy( Info.szGuild[i], pk.ReadString().c_str(), 64 - 1 );
			strncpy( Info.szChall[i], pk.ReadString().c_str(), 64 - 1 );
			Info.dwMoney[i] = pk.ReadInt64();
		}
	}

	NetMC_GUILD_CHALLINFO( Info );
	return TRUE;
}
