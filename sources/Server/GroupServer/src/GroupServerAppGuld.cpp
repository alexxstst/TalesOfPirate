#include "stdafx.h"
#include <iostream>
#include "GroupServerApp.h"
#include "GameCommon.h"

void GroupServerApp::PC_GULD_INIT(Player *ply)
{
	if(ply->m_guild[ply->m_currcha] >0)
	{
		Guild *l_guild =FindGuildByGldID(ply->m_guild[ply->m_currcha]);
		if(l_guild)
		{
			ply->JoinGuild(l_guild);
			if(l_guild->m_leaderID ==ply->m_chaid[ply->m_currcha])
			{
				l_guild->m_leader	=ply;
			}
		}else
		{
			LogLine	l_line(g_LogGuild);
			//l_line<<newln<<"���["<<ply->m_chaname[ply->m_currcha]<<"]���߳�ʼ��ʱû�ҵ�����ṹ,����ID:"<<ply->m_guild[ply->m_currcha]<<endln;
			l_line<<newln<<"player ["<<ply->m_chaname[ply->m_currcha]<<"] can't get guild struct ,guild ID:"<<ply->m_guild[ply->m_currcha]<<endln;
		}
	}
	auto const l_lockDB = std::lock_guard{m_mtxDB};
	m_tblguilds->InitGuildMember(ply,ply->m_chaid[ply->m_currcha],ply->m_guild[ply->m_currcha],0);
}
void GroupServerApp::MP_GUILD_CREATE(Player *ply,net::TcpClient *client,net::RPacket &pk)
{
	ply->m_guildPermission[ply->m_currcha] = emGldPermMax;
	ply->m_guild[ply->m_currcha]=pk.ReadInt64();
	Guild *l_gld				=FindGuildByGldID(ply->m_guild[ply->m_currcha]);
	l_gld->m_id					=ply->m_guild[ply->m_currcha];	//����ID
	strcpy(l_gld->m_name, pk.ReadString().c_str());						//������
	strcpy(l_gld->m_motto,"");									//����������
	l_gld->m_leaderID			=ply->m_chaid[ply->m_currcha];	//�᳤ID
	l_gld->m_stat				=0;								//����״̬
	l_gld->m_remain_minute		=0;								//�����ɢʣ�������
	l_gld->m_tick				=GetTickCount();

	ply->JoinGuild(l_gld);
	net::WPacket	l_wpk(256);
	l_wpk.WriteCmd(CMD_PC_GUILD);
	l_wpk.WriteInt64(MSG_GUILD_START);
	l_wpk.WriteInt64(ply->m_guild[ply->m_currcha]);	//����ID
	l_wpk.WriteString(ply->GetGuild()->m_name);		//����name
	l_wpk.WriteInt64(ply->GetGuild()->m_leaderID);	//�᳤ID

	l_wpk.WriteInt64(1);									//online
	l_wpk.WriteInt64(ply->m_chaid[ply->m_currcha]);		//chaid
	l_wpk.WriteString(ply->m_chaname[ply->m_currcha].c_str());	//chaname
	l_wpk.WriteString(ply->m_motto[ply->m_currcha].c_str());	//motto
	l_wpk.WriteString(pk.ReadString());					//job
	l_wpk.WriteInt64(pk.ReadInt64());					//degree
	l_wpk.WriteInt64(ply->m_icon[ply->m_currcha]);		//icon
	l_wpk.WriteInt64(emGldPermMax);							//permission

	l_wpk.WriteInt64(0);
	l_wpk.WriteInt64(1);
	g_gpsvr->SendToClient(ply,l_wpk);
}
void GroupServerApp::MP_GUILD_APPROVE(Player *ply,net::TcpClient *client,net::RPacket &pk)
{
	uLong	l_chaid	=pk.ReadInt64();
	Player	*l_ply	=FindPlayerByChaID(l_chaid);
	if(!ply->GetGuild())
	{
		LogLine	l_line(g_LogGuild);
		//l_line<<newln<<"GroupServer�ϵĹ��������쳣���뿪���߽��...";
		l_line<<newln<<"GroupServer guild data exception, please contact developer...";
		return;
	}
	if(l_ply)
	{
		l_ply->m_guild[l_ply->m_currcha]	=ply->GetGuild()->m_id;
		l_ply->m_guildPermission[l_ply->m_currcha] = emGldPermDefault;
		l_ply->JoinGuild(ply->GetGuild());
	}
	auto const l_lockDB = std::lock_guard{m_mtxDB};
	m_tblguilds->InitGuildMember(l_ply,l_chaid,ply->GetGuild()->m_id,1);
}
void GroupServerApp::MP_GUILD_KICK(Player *ply,net::TcpClient *client,net::RPacket &pk)
{
	uLong	 l_chaid	=pk.ReadInt64();
	Guild	*l_guild	=ply->GetGuild();
	if(!l_guild)
	{
		LogLine	l_line(g_LogGuild);
		//l_line<<newln<<"GroupServer�ϵĹ��������쳣���뿪���߽��...";
		l_line<<newln<<"GroupServer guild data exception, please contact developer...";
		return;
	}
	Player	*l_ply	=l_guild->FindGuildMemByChaID(l_chaid);

	if(l_ply && l_ply->m_currcha >=0)
	{
		l_ply->m_guild[l_ply->m_currcha]	=0;
		ply->m_guildPermission[ply->m_currcha] = 0;
		l_ply->LeaveGuild();

		net::WPacket	l_wpk(256);
		l_wpk.WriteCmd(CMD_PC_GUILD);
		l_wpk.WriteInt64(MSG_GUILD_STOP);
		SendToClient(l_ply,l_wpk);
	}
	Player *l_plylst[10240];
	short	l_plynum	=0;

	net::WPacket	l_wpk(256);
	l_wpk.WriteCmd(CMD_PC_GUILD);
	l_wpk.WriteInt64(MSG_GUILD_DEL);
	l_wpk.WriteInt64(l_chaid);
	RunChainGetArmor<GuildMember> l(*l_guild);
	while(l_ply	=static_cast<Player	*>(l_guild->GetNextItem()))
	{
		l_plylst[l_plynum]	=l_ply;
		l_plynum	++;
	}
	l.unlock();

	SendToClient(l_plylst,l_plynum,l_wpk);
}
void GroupServerApp::MP_GUILD_LEAVE(Player *ply,net::TcpClient *client,net::RPacket &pk)
{
	uLong	 l_chaid	=ply->m_chaid[ply->m_currcha];
	Guild	*l_guild	=ply->GetGuild();
	if(!l_guild)
	{
		LogLine	l_line(g_LogGuild);
		//l_line<<newln<<"GroupServer�ϵĹ��������쳣���뿪���߽��...";
		l_line<<newln<<"GroupServer guild data exception, please contact developer...";
		return;
	}
	{
		ply->m_guildPermission[ply->m_currcha] = 0;
		ply->m_guild[ply->m_currcha]	=0;
		ply->LeaveGuild();

		net::WPacket	l_wpk(256);
		l_wpk.WriteCmd(CMD_PC_GUILD);
		l_wpk.WriteInt64(MSG_GUILD_STOP);
		SendToClient(ply,l_wpk);
	}
	Player *l_plylst[10240];
	short	l_plynum	=0;

	net::WPacket	l_wpk(256);
	l_wpk.WriteCmd(CMD_PC_GUILD);
	l_wpk.WriteInt64(MSG_GUILD_DEL);
	l_wpk.WriteInt64(l_chaid);
	RunChainGetArmor<GuildMember> l(*l_guild);
	while(ply	=static_cast<Player	*>(l_guild->GetNextItem()))
	{
		l_plylst[l_plynum]	=ply;
		l_plynum	++;
	}
	l.unlock();

	SendToClient(l_plylst,l_plynum,l_wpk);
}
void GroupServerApp::MP_GUILD_DISBAND(Player *ply,net::TcpClient *client,net::RPacket &pk)
{
	Guild	*l_guild	=ply->GetGuild();
	if(!l_guild)
	{
		LogLine	l_line(g_LogGuild);
		//l_line<<newln<<"GroupServer�ϵĹ��������쳣���뿪���߽��...";
		l_line<<newln<<"GroupServer guild data exception, please contact developer...";
		return;
	}
	l_guild->m_leader	= nullptr;
	l_guild->m_leaderID	= 0;

	Player *l_plylst[10240];
	short	l_plynum	=0;

	net::WPacket	l_wpk(256);
	l_wpk.WriteCmd(CMD_PC_GUILD);
	l_wpk.WriteInt64(MSG_GUILD_STOP);
	RunChainGetArmor<GuildMember> l(*l_guild);
	while(ply	=static_cast<Player	*>(l_guild->GetFirstItem()))
	{
		ply->m_guildPermission[ply->m_currcha] = 0;
		ply->m_guild[ply->m_currcha]	=0;
		ply->LeaveGuild();

		l_plylst[l_plynum]	=ply;
		l_plynum	++;
	}
	l.unlock();

	SendToClient(l_plylst,l_plynum,l_wpk);

}
void GroupServerApp::MP_GUILD_MOTTO(Player *ply,net::TcpClient *client,net::RPacket &pk)
{
	Guild	*l_guild	=ply->GetGuild();
	if(!l_guild)
	{
		LogLine	l_line(g_LogGuild);
		//l_line<<newln<<"GroupServer�ϵĹ��������쳣���뿪���߽��...";
		l_line<<newln<<"GroupServer guild data exception, please contact developer...";
		return;
	}
	strcpy(l_guild->m_motto,pk.ReadString().c_str());
}

void GroupServerApp::MP_GUILD_CHALLMONEY(Player *ply,net::TcpClient *client,net::RPacket &pk)
{
	DWORD dwChallID = pk.ReadInt64();
	DWORD dwMoney  = pk.ReadInt64();
	Guild* pGuild = FindGuildByGldID( dwChallID );
	if( !pGuild || pGuild->m_leaderID == 0 )
	{
		LogLine	l_line(g_LogGuild);
		//l_line<<newln<<"GroupServer�ϵĹ��������쳣��δ�ҵ�����,���߹���û�л᳤���˻���ս�����Ǯʧ�ܣ�guildid = "<<dwChallID<<"money = "<<dwMoney;
		l_line<<newln<<"GroupServer guild data exception, find guild nothing, or guild has no leader! withdrawal challenging money! guildid = "<<dwChallID<<"money = "<<dwMoney;
		return;
	}

	auto pszGuild1 = pk.ReadString();
	auto pszGuild2 = pk.ReadString();

	Player	*l_ply = pGuild->m_leader;
	if( !l_ply || l_ply->m_currcha == -1 || pGuild->m_leaderID != l_ply->m_chaid[l_ply->m_currcha] )
	{
		// ������,�������ݿ�
		LogLine	l_line(g_LogGuild);
		//l_line<<newln<<"��Ҳ�����ͨ�����ݿ�������˻���ս���ᡶ"<<pszGuild1<<"����Ǯ��chaid = "<<pGuild->m_leaderID<<"money = "<<dwMoney;
		l_line<<newln<<"player is offline, withdrawal challenging��"<<pszGuild1<<"��money!chaid = "<<pGuild->m_leaderID<<"money = "<<dwMoney;

		auto const l_lockDB = std::lock_guard{g_gpsvr->m_mtxDB};
		if( !g_gpsvr->m_tblcharaters->AddMoney( pGuild->m_leaderID, dwMoney ) )
		{
			LogLine	l_line(g_LogGuild);
			//l_line<<newln<<"��ս���ᣬ�˻���ս���ᡶ"<<pszGuild1<<"����Ǯ���ݿ����ʧ�ܣ�chaid = "<<pGuild->m_leaderID<<"money = "<<dwMoney;
			l_line<<newln<<"challenge guild, withdrawal challenging��"<<pszGuild1<<"��money failed!chaid = "<<pGuild->m_leaderID<<"money = "<<dwMoney;
		}
	}
	else
	{
		// ������֪ͨ���ڷ�����
		LogLine	l_line(g_LogGuild);
		//l_line<<newln<<"����֪ͨ��ս���ᣬ�˻���ս���ᡶ"<<pszGuild1<<"����Ǯ��chaid = "<<pGuild->m_leaderID<<"money = "<<dwMoney;
		l_line<<newln<<"online guild, withdrawal challenging��"<<pszGuild1<<"��money!chaid = "<<pGuild->m_leaderID<<"money = "<<dwMoney;

		net::WPacket	l_wpk(256);
		l_wpk.WriteCmd(CMD_PM_GUILD_CHALLMONEY);
		l_wpk.WriteInt64( pGuild->m_leaderID );
		l_wpk.WriteInt64( dwMoney );
		l_wpk.WriteString( pszGuild1 );
		l_wpk.WriteString( pszGuild2 );
		l_wpk.WriteInt64( 0 );
		SendToClient( l_ply, l_wpk );
	}
}

void GroupServerApp::MP_GUILD_CHALL_PRIZEMONEY(Player *ply,net::TcpClient *client,net::RPacket &pk)
{
	DWORD dwChallID = pk.ReadInt64();
	DWORD dwMoney  = pk.ReadInt64();
	Guild* pGuild = FindGuildByGldID( dwChallID );
	if( !pGuild || pGuild->m_leaderID == 0 )
	{
		LogLine	l_line(g_LogGuild);
		//l_line<<newln<<"GroupServer�ϵĹ��������쳣��δ�ҵ�����,���߹���û�л᳤���˻���ս����ʤ������ʧ�ܣ�guildid = "<<dwChallID<<"money = "<<dwMoney;
		l_line<<newln<<"GroupServer guild data exception, can't find leader, or has no leader! withdrawal challenging money failed!guildid = "<<dwChallID<<"money = "<<dwMoney;
		return;
	}

	Player	*l_ply = pGuild->m_leader;
	if( !l_ply || l_ply->m_currcha == -1 || pGuild->m_leaderID != l_ply->m_chaid[l_ply->m_currcha] )
	{
		// ������,�������ݿ�
		LogLine	l_line(g_LogGuild);
		//l_line<<newln<<"��Ҳ�����ͨ�����ݿ�������˻���ս���ᡶ"<<pGuild->m_name<<"����Ǯ��chaid = "<<pGuild->m_leaderID<<"money = "<<dwMoney;
		l_line<<newln<<"player is offline, withdrawal challenging guild��"<<pGuild->m_name<<"��money! chaid = "<<pGuild->m_leaderID<<"money = "<<dwMoney;

		auto const l_lockDB = std::lock_guard{g_gpsvr->m_mtxDB};
		if( !g_gpsvr->m_tblcharaters->AddMoney( pGuild->m_leaderID, dwMoney ) )
		{
			LogLine	l_line(g_LogGuild);
			//l_line<<newln<<"��ս���ᣬ�˻���ս���ᡶ"<<pGuild->m_name<<"��ʤ���������ݿ����ʧ�ܣ�chaid = "<<pGuild->m_leaderID<<"money = "<<dwMoney;
			l_line<<newln<<"challenging guild, withdrawal challenging guild��"<<pGuild->m_name<<"��money failed! chaid = "<<pGuild->m_leaderID<<"money = "<<dwMoney;
		}
	}
	else
	{
		LogLine	l_line(g_LogGuild);
		//l_line<<newln<<"����֪ͨ��ս���ᣬ�˻���ս���ᡶ"<<pGuild->m_name<<"��ʤ������chaid = "<<pGuild->m_leaderID<<"money = "<<dwMoney;
		l_line<<newln<<"online challenging guild, withdrawal challenging guild��"<<pGuild->m_name<<"��moeny!chaid = "<<pGuild->m_leaderID<<"money = "<<dwMoney;

		// ������֪ͨ���ڷ�����
		net::WPacket	l_wpk(256);
		l_wpk.WriteCmd(CMD_PM_GUILD_CHALL_PRIZEMONEY);
		l_wpk.WriteInt64( pGuild->m_leaderID );
		l_wpk.WriteInt64( dwMoney );
		l_wpk.WriteInt64( 0 );
		SendToClient( l_ply, l_wpk );
	}
}
