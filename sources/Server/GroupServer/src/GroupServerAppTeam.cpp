#include "stdafx.h"
#include <iostream>
#include "GroupServerApp.h"
#include "GameCommon.h"

#include "Parser.h"

void GroupServerApp::CP_TEAM_INVITE(Player *ply,net::TcpClient *client,net::RPacket &pk)
{
	//// Add by lark.li 20080715 begin
	//if(!CheckFunction("garner", "Team_Invite"))
	//{
	//	ply->SendSysInfo("You can't invite!");
	//	return;
	//}
	//// End

	if(ply->GetTeam() && ply->GetLeader() !=ply)
	{
		//ply->SendSysInfo("ֻ�жӳ����ܷ���������롣");
		ply->SendSysInfo(RES_STRING(GP_GROUPSERVERAPPTEAM_CPP_00001));
	}else if(ply->GetTeam() && ply->GetTeam()->GetTotal() >=const_team.MemberMax)
	{
		//ply->SendSysInfo("��Ķ����Ա���Ѿ��ﵽ��ϵͳ�����������ˡ�");
		ply->SendSysInfo(RES_STRING(GP_GROUPSERVERAPPTEAM_CPP_00002));
	}else
	{
		Invited			*	l_invited	=0;
		auto				l_invited_name	=pk.ReadString();
		if(l_invited_name.empty() ||l_invited_name.size() >16)
		{
			return;
		}
		Player			*	l_invited_ply	=FindPlayerByChaName(l_invited_name.c_str());
		if(!l_invited_ply || l_invited_ply->m_currcha <0 ||l_invited_ply ==ply)
		{
			char l_buf[256];
			sprintf(l_buf,RES_STRING(GP_GROUPSERVERAPPTEAM_CPP_00003),l_invited_name.c_str());
			ply->SendSysInfo(l_buf);
		}else if(l_invited_ply->GetTeam())
		{
			char l_buf[256];
			sprintf(l_buf,RES_STRING(GP_GROUPSERVERAPPTEAM_CPP_00004),l_invited_name.c_str());
			ply->SendSysInfo(l_buf);
		}
		else if (l_invited = l_invited_ply->TeamFindInvitedByInviterChaID(ply->m_chaid[ply->m_currcha]))
		{
			//ply->SendSysInfo(dstring("����ǰ�ԡ�")<<l_invited_name<<"���Ѿ���һ��δ����������룬���԰����ꡣ");
			char l_buf[256];
			sprintf(l_buf, RES_STRING(GP_GROUPSERVERAPPTEAM_CPP_00005), l_invited_name.c_str());
			ply->SendSysInfo(l_buf);

		}
		else if (!l_invited_ply->CanReceiveRequests()) {
			char l_buf[256];
			sprintf(l_buf, "%s is currently offline. Unable to send request!", l_invited_name.c_str());
			ply->SendSysInfo(l_buf);
		}else
		{
			PtInviter l_ptinviter	=l_invited_ply->TeamBeginInvited(ply);
			if(l_ptinviter )
			{
				char l_buf[256];
				sprintf(l_buf,RES_STRING(GP_GROUPSERVERAPPTEAM_CPP_00007),l_invited_name.c_str());
				l_ptinviter->SendSysInfo(l_buf);

				net::WPacket	l_wpk	=net::WPacket(256);
				l_wpk.WriteCmd(CMD_PC_TEAM_CANCEL);
				l_wpk.WriteInt64(MSG_TEAM_CANCLE_BUSY);
				l_wpk.WriteInt64(l_ptinviter.m_chaid);
				SendToClient(l_invited_ply,l_wpk);
			}
			net::WPacket	l_wpk	=net::WPacket(256);
			l_wpk.WriteCmd(CMD_PC_TEAM_INVITE);
			l_wpk.WriteString(ply->m_chaname[ply->m_currcha].c_str());
			l_wpk.WriteInt64(ply->m_chaid[ply->m_currcha]);
			l_wpk.WriteInt64(ply->m_icon[ply->m_currcha]);
			SendToClient(l_invited_ply,l_wpk);
		}
	}
}
void GroupServerApp::CP_TEAM_REFUSE(Player *ply,net::TcpClient *client,net::RPacket &pk)
{
	uLong		l_inviter_chaid	=pk.ReadInt64();
	PtInviter	l_inviter		=ply->TeamEndInvited(l_inviter_chaid);
	if(l_inviter && l_inviter->m_currcha >=0 && l_inviter.m_chaid ==l_inviter->m_chaid[l_inviter->m_currcha])
	{
		char l_buf[256];
		sprintf(l_buf,RES_STRING(GP_GROUPSERVERAPPTEAM_CPP_00008),ply->m_chaname[ply->m_currcha].c_str());
		l_inviter->SendSysInfo(l_buf);
	}
}
void GroupServerApp::MP_TEAM_CREATE(Player *ply,net::TcpClient *client,net::RPacket &pk)
{
	bool bInvited = false;
	auto szName1 = pk.ReadString(); // ��Ա
	auto szName2 = pk.ReadString(); // �ӳ�

	Player *pPly = FindPlayerByChaName(szName2.c_str());
	Player *pPly2 = FindPlayerByChaName(szName1.c_str());

	if(!pPly || !pPly2)
	{
		LogLine	l_line(g_LogMaster);
		//l_line<<newln<<"MP_TEAM_CREATE()��Ա������!";
		l_line<<newln<<"MP_TEAM_CREATE() member is offline!";
		return;
	}

	//����
	if(pPly->GetTeam() && pPly->GetLeader() !=pPly)
	{
		//pPly->SendSysInfo("�����Ƕӳ�!");
		pPly->SendSysInfo(RES_STRING(GP_GROUPSERVERAPPTEAM_CPP_00010));
		//pPly2->SendSysInfo("�Է����Ƕӳ�!");
		pPly2->SendSysInfo(RES_STRING(GP_GROUPSERVERAPPTEAM_CPP_00011));
	}else if(pPly->GetTeam() && pPly->GetTeam()->GetTotal() >=const_team.MemberMax)
	{
		//pPly->SendSysInfo("��Ķ����Ա���Ѿ��ﵽ��ϵͳ������������!");
		pPly->SendSysInfo(RES_STRING(GP_GROUPSERVERAPPTEAM_CPP_00002));
		pPly2->SendSysInfo(RES_STRING(GP_GROUPSERVERAPPTEAM_CPP_00012));
	}else
	{
		Invited			*	l_invited	=0;
		uShort				l_len = (uShort)szName1.size();
		cChar			*	l_invited_name	= szName1.c_str();
		if(!l_invited_name ||l_len >16)
		{
			LogLine	l_line(g_LogMaster);
			//l_line<<newln<<"MP_TEAM_CREATE()�������ȷǷ�!";
			l_line<<newln<<"MP_TEAM_CREATE() name length is invalid!";
			return;
		}
		Player			*	l_invited_ply	= pPly2;
		if(!l_invited_ply || l_invited_ply->m_currcha <0 ||l_invited_ply == pPly)
		{
			char l_buf[256];
			sprintf(l_buf,RES_STRING(GP_GROUPSERVERAPPTEAM_CPP_00014),l_invited_name);
			pPly->SendSysInfo(l_buf);
		}else if(l_invited_ply->GetTeam())
		{
			char l_buf[256];
			sprintf(l_buf,RES_STRING(GP_GROUPSERVERAPPTEAM_CPP_00015),l_invited_name);
			pPly->SendSysInfo(l_buf);
		}else if(l_invited =l_invited_ply->TeamFindInvitedByInviterChaID(pPly->m_chaid[pPly->m_currcha]))
		{
			//pPly->SendSysInfo(dstring("����ǰ�ԡ�")<<l_invited_name<<"���Ѿ���һ��δ����������룬���԰����ꡣ");
			char l_buf[256];
			sprintf(l_buf,RES_STRING(GP_GROUPSERVERAPPTEAM_CPP_00005),l_invited_name);
			pPly->SendSysInfo(l_buf);
		}else
		{
			PtInviter l_ptinviter	=l_invited_ply->TeamBeginInvited(pPly);
			if(l_ptinviter )
			{
				char l_buf[256];
				sprintf(l_buf,RES_STRING(GP_GROUPSERVERAPPTEAM_CPP_00016),l_invited_name);
				l_ptinviter->SendSysInfo(l_buf);
			}

			bInvited = true;
		}
	}

	//��������
	if(bInvited)
	{
		cChar *szInviterName = szName2.c_str();
		long	l_count		=pPly2->JoinTeam(szInviterName);
		if(l_count &&(l_count >const_team.MemberMax))
		{
			pPly2->LeaveTeam();
			//pPly2->SendSysInfo("��Ҫ�������ӳ�Ա���Ѵﵽ���������ޡ�");
			pPly2->SendSysInfo(RES_STRING(GP_GROUPSERVERAPPTEAM_CPP_00017));
		}else if(l_count)
		{
			LogLine	l_line(g_LogMaster);
			//l_line<<newln<<"���"<<pPly2->m_chaname[pPly2->m_currcha]<<"���������"<<pPly2->GetLeader()->m_chaname[pPly2->GetLeader()->m_currcha]<<endln;
			l_line<<newln<<"player "<<pPly2->m_chaname[pPly2->m_currcha]<<" add team "<<pPly2->GetLeader()->m_chaname[pPly2->GetLeader()->m_currcha]<<endln;
			//֪ͨClient���Ա�仯
			{
				Team		*	l_team	=pPly2->GetTeam();

				net::WPacket	l_wpk	=net::WPacket(256);
				l_wpk.WriteCmd(CMD_PC_TEAM_REFRESH);
				l_wpk.WriteInt64(TEAM_MSG_ADD);
				l_wpk.WriteInt64(uChar(l_count));

				Player *l_plylst[10240];
				short	l_plynum	=0;

				Player	*	l_ply1;char	l_currcha;
				RunChainGetArmor<TeamMember> l(*l_team);
				for(int i =0;i<l_count &&(l_ply1=static_cast<Player*>(l_team->GetNextItem()));i++)
				{
					if((l_currcha =l_ply1->m_currcha) >=0)
					{
						l_wpk.WriteInt64(l_ply1->m_chaid[l_currcha]);
						l_wpk.WriteString(l_ply1->m_chaname[l_currcha].c_str());
						l_wpk.WriteString(l_ply1->m_motto[l_currcha].c_str());
						l_wpk.WriteInt64(l_ply1->m_icon[l_currcha]);

						l_plylst[l_plynum]	=l_ply1;
						l_plynum ++;
					}
				}
				l.unlock();

				SendToClient(l_plylst,l_plynum,l_wpk);
			}
			//֪ͨGameServer���Ա�仯
			{
				Team		*	l_team	=pPly2->GetTeam();
				Player		*	l_plyr;
				GateServer	*	l_gate[30];for(int i=0;i<30;i++)l_gate[i]=0;
				char			l_gtnum	=0;

				net::WPacket	l_wpk	=net::WPacket(256);
				l_wpk.WriteCmd(CMD_PM_TEAM);
				l_wpk.WriteInt64(TEAM_MSG_ADD);
				l_wpk.WriteInt64((l_count));
				RunChainGetArmor<TeamMember> l(*l_team);
				for(int i =0;i<l_count &&(l_plyr=static_cast<Player*>(l_team->GetNextItem()));i++)
				{
					l_wpk.WriteString(l_plyr->m_gate->m_name.c_str());
					l_wpk.WriteInt64(l_plyr->m_gtAddr);
					l_wpk.WriteInt64(l_plyr->m_chaid[l_plyr->m_currcha]);
					for(int j=0;j<30;j++)
					{
						if(l_gate[j] ==l_plyr->m_gate)
						{
							break;
						}
						if(!l_gate[j])
						{
							l_gate[j]	=l_plyr->m_gate;
							l_gtnum	++;
							break;
						}
					}
				}
				l.unlock();
#if 1
				l_gtnum	=l_gtnum?1:0;
#endif
				for(int j=0;j<l_gtnum;j++)
				{
					if (l_gate[j]->IsValid())
					{
						l_gate[j]->SendData(l_wpk);
					}
					LogLine	l_line(g_LogMaster);
					//l_line<<newln<<"MP_TEAM_CREATE()ȷ��ToGameServer�ļ�����Ӳ������������ݵ�GateServer";
					l_line<<newln<<"MP_TEAM_CREATE() send ToGameServer data to GateServer";
				}
			}
		}
	}
	else
	{
		LogLine l_line(g_LogMaster);
		//l_line<<newln<<"MP_TEAM_CREATE()����ʧ��";
		l_line<<newln<<"MP_TEAM_CREATE() invite failed";
	}
}
void GroupServerApp::CP_TEAM_ACCEPT(Player *ply,net::TcpClient *client,net::RPacket &pk)
{
	uLong	l_inviter_chaid	=pk.ReadInt64();
	long	l_count		=ply->JoinTeam(l_inviter_chaid);
	if(l_count &&(l_count >const_team.MemberMax))
	{
		ply->LeaveTeam();
		//ply->SendSysInfo("��Ҫ�������ӳ�Ա���Ѵﵽ���������ޡ�");
		ply->SendSysInfo(RES_STRING(GP_GROUPSERVERAPPTEAM_CPP_00017));
	}else if(l_count)
	{
		LogLine	l_line(g_LogTeam);
		//l_line<<newln<<"���"<<ply->m_chaname[ply->m_currcha]<<"���������"<<ply->GetLeader()->m_chaname[ply->GetLeader()->m_currcha]<<endln;
		l_line<<newln<<"player "<<ply->m_chaname[ply->m_currcha]<<"add team"<<ply->GetLeader()->m_chaname[ply->GetLeader()->m_currcha]<<endln;
		//֪ͨClient���Ա�仯
		{
			Team		*	l_team	=ply->GetTeam();

			net::WPacket	l_wpk	=net::WPacket(256);
			l_wpk.WriteCmd(CMD_PC_TEAM_REFRESH);
			l_wpk.WriteInt64(TEAM_MSG_ADD);
			l_wpk.WriteInt64(uChar(l_count));

			Player *l_plylst[10240];
			short	l_plynum	=0;

			Player	*	l_ply1;char	l_currcha;
			RunChainGetArmor<TeamMember> l(*l_team);
			for(int i =0;i<l_count &&(l_ply1=static_cast<Player*>(l_team->GetNextItem()));i++)
			{
				if((l_currcha =l_ply1->m_currcha) >=0)
				{
					l_wpk.WriteInt64(l_ply1->m_chaid[l_currcha]);
					l_wpk.WriteString(l_ply1->m_chaname[l_currcha].c_str());
					l_wpk.WriteString(l_ply1->m_motto[l_currcha].c_str());
					l_wpk.WriteInt64(l_ply1->m_icon[l_currcha]);

					l_plylst[l_plynum]	=l_ply1;
					l_plynum ++;
				}
			}
			l.unlock();

			SendToClient(l_plylst,l_plynum,l_wpk);
		}
		//֪ͨGameServer���Ա�仯
		{
			Team		*	l_team	=ply->GetTeam();
			Player		*	l_plyr;
			GateServer	*	l_gate[30];for(int i=0;i<30;i++)l_gate[i]=0;
			char			l_gtnum	=0;

			net::WPacket	l_wpk	=net::WPacket(256);
			l_wpk.WriteCmd(CMD_PM_TEAM);
			l_wpk.WriteInt64(TEAM_MSG_ADD);
			l_wpk.WriteInt64((l_count));
			RunChainGetArmor<TeamMember> l(*l_team);
			for(int i =0;i<l_count &&(l_plyr=static_cast<Player*>(l_team->GetNextItem()));i++)
			{
				l_wpk.WriteString(l_plyr->m_gate->m_name.c_str());
				l_wpk.WriteInt64(l_plyr->m_gtAddr);
				l_wpk.WriteInt64(l_plyr->m_chaid[l_plyr->m_currcha]);
				for(int j=0;j<30;j++)
				{
					if(l_gate[j] ==l_plyr->m_gate)
					{
						break;
					}
					if(!l_gate[j])
					{
						l_gate[j]	=l_plyr->m_gate;
						l_gtnum	++;
						break;
					}
				}
			}
			l.unlock();
#if 1
			l_gtnum	=l_gtnum?1:0;
#endif
			for(int j=0;j<l_gtnum;j++)
			{
				if (l_gate[j]->IsValid())
				{
					l_gate[j]->SendData(l_wpk);
				}
				LogLine	l_line(g_LogTeam);
				//l_line<<newln<<"ȷ��ToGameServer�ļ�����Ӳ������������ݵ�GateServer";
				l_line<<newln<<"MP_TEAM_CREATE() send ToGameServer data to GateServer";
			}
		}
	}
}
void GroupServerApp::CP_TEAM_LEAVE(Player *ply,net::TcpClient *client,net::RPacket &pk)
{
	Team	*l_team		=ply->GetTeam();
	if(!l_team)return;
	Player	*l_leader	=l_team->GetLeader();
	long	l_count		=ply->LeaveTeam();
	if(l_count)
	{
		//֪ͨClient���Ա�仯
		{
			net::WPacket	l_wpk	=net::WPacket(256);
			l_wpk.WriteCmd(CMD_PC_TEAM_REFRESH);
			l_wpk.WriteInt64(TEAM_MSG_LEAVE);
			l_wpk.WriteInt64(uChar(l_count));

			Player *l_plylst[10240];
			short	l_plynum	=0;

			Player	*	l_ply1;char	l_currcha;
			RunChainGetArmor<TeamMember> l(*l_team);
			for(int i =0;i<l_count &&((l_ply1=static_cast<Player*>(l_team->GetNextItem()))||(l_ply1 =ply));i++)
			{
				if((l_currcha =l_ply1->m_currcha) >=0)
				{
					l_wpk.WriteInt64(l_ply1->m_chaid[l_currcha]);
					l_wpk.WriteString(l_ply1->m_chaname[l_currcha].c_str());
					l_wpk.WriteString(l_ply1->m_motto[l_currcha].c_str());
					l_wpk.WriteInt64(l_ply1->m_icon[l_currcha]);

					l_plylst[l_plynum]	=l_ply1;
					l_plynum ++;
				}
			}
			l.unlock();
			SendToClient(l_plylst,l_plynum,l_wpk);
		}
		//֪ͨGameServer���Ա�仯
		{
			Player		*	l_plyr;
			GateServer	*	l_gate[30];for(int i=0;i<30;i++)l_gate[i]=0;
			char			l_gtnum	=0;

			net::WPacket	l_wpk	=net::WPacket(256);
			l_wpk.WriteCmd(CMD_PM_TEAM);
			l_wpk.WriteInt64(TEAM_MSG_LEAVE);
			l_wpk.WriteInt64((l_count));
			RunChainGetArmor<TeamMember> l(*l_team);
			for(int i =0;i<l_count &&((l_plyr=static_cast<Player*>(l_team->GetNextItem()))||(l_plyr =ply));i++)
			{
				l_wpk.WriteString(l_plyr->m_gate->m_name.c_str());
				l_wpk.WriteInt64(l_plyr->m_gtAddr);
				l_wpk.WriteInt64(l_plyr->m_chaid[l_plyr->m_currcha]);
				for(int j=0;j<30;j++)
				{
					if(l_gate[j] ==l_plyr->m_gate)
					{
						break;
					}
					if(!l_gate[j])
					{
						l_gate[j]	=l_plyr->m_gate;
						l_gtnum	++;
						break;
					}
				}
			}
			l.unlock();
#if 1
			l_gtnum	=l_gtnum?1:0;
#endif
			for(int j=0;j<l_gtnum;j++)
			{
				if (l_gate[j]->IsValid())
				{
					l_gate[j]->SendData(l_wpk);
				}
			}

			if(l_count ==2)
			{
				l_team->GetLeader()->LeaveTeam();
			}
			LogLine	l_line(g_LogTeam);
			/*l_line<<newln<<"���"<<ply->m_chaname[ply->m_currcha]<<"�뿪�����"
				<<l_leader->m_chaname[l_leader->m_currcha]<<(l_count ==2?",�����ɢ��":"��")
				<<endln;
			*/
			l_line<<newln<<"player "<<ply->m_chaname[ply->m_currcha]<<"leave team "
				<<l_leader->m_chaname[l_leader->m_currcha]<<(l_count ==2?",free team.":".")
				<<endln;
		}
	}
}
void GroupServerApp::CP_TEAM_KICK(Player *ply,net::TcpClient *client,net::RPacket &pk)
{
	Team	*l_team		=ply->GetTeam();
	if(!l_team)return;
	Player	*l_leader	=l_team->GetLeader();	
	if( ply != l_leader ) return;
	DWORD dwKickedID = pk.ReadInt64();
	if( dwKickedID == (ply->m_currcha >= 0 ? ply->m_chaid[ply->m_currcha] : 0) )
	{
		return;
	}
	Player* pKicker = l_team->GetMember( dwKickedID );
	if( !pKicker ) 
	{
		LogLine	l_line(g_LogTeam);
		//l_line<<newln<<"�ӳ�"<<ply->m_chaname[ply->m_currcha]<<"�߳���Ա�����ڣ�ID["
		//	<<dwKickedID<<"]"<<endln;
		l_line<<newln<<"captain "<<ply->m_chaname[ply->m_currcha]<<"killed member not exsit! ID["
			<<dwKickedID<<"]"<<endln;
		return;
	}
	long	l_count		=pKicker->LeaveTeam();
	if(l_count)
	{
		//֪ͨClient���Ա�仯
		{
			net::WPacket	l_wpk	=net::WPacket(256);
			l_wpk.WriteCmd(CMD_PC_TEAM_REFRESH);
			l_wpk.WriteInt64(TEAM_MSG_KICK);
			l_wpk.WriteInt64((l_count));

			Player *l_plylst[10240];
			short	l_plynum	=0;

			Player	*	l_ply1;char	l_currcha;
			RunChainGetArmor<TeamMember> l(*l_team);
			for(int i =0;i<l_count &&((l_ply1=static_cast<Player*>(l_team->GetNextItem()))||(l_ply1 =pKicker));i++)
			{
				if((l_currcha =l_ply1->m_currcha) >=0)
				{
					l_wpk.WriteInt64(l_ply1->m_chaid[l_currcha]);
					l_wpk.WriteString(l_ply1->m_chaname[l_currcha].c_str());
					l_wpk.WriteString(l_ply1->m_motto[l_currcha].c_str());
					l_wpk.WriteInt64(l_ply1->m_icon[l_currcha]);

					l_plylst[l_plynum]	=l_ply1;
					l_plynum ++;
				}
			}
			l.unlock();
			SendToClient(l_plylst,l_plynum,l_wpk);
		}
		//֪ͨGameServer���Ա�仯
		{
			Player		*	l_plyr;
			GateServer	*	l_gate[30];for(int i=0;i<30;i++)l_gate[i]=0;
			char			l_gtnum	=0;

			net::WPacket	l_wpk	=net::WPacket(256);
			l_wpk.WriteCmd(CMD_PM_TEAM);
			l_wpk.WriteInt64(TEAM_MSG_LEAVE);
			l_wpk.WriteInt64((l_count));
			RunChainGetArmor<TeamMember> l(*l_team);
			for(int i =0;i<l_count &&((l_plyr=static_cast<Player*>(l_team->GetNextItem()))||(l_plyr =pKicker));i++)
			{
				l_wpk.WriteString(l_plyr->m_gate->m_name.c_str());
				l_wpk.WriteInt64(l_plyr->m_gtAddr);
				l_wpk.WriteInt64(l_plyr->m_chaid[l_plyr->m_currcha]);
				for(int j=0;j<30;j++)
				{
					if(l_gate[j] ==l_plyr->m_gate)
					{
						break;
					}
					if(!l_gate[j])
					{
						l_gate[j]	=l_plyr->m_gate;
						l_gtnum	++;
						break;
					}
				}
			}
			l.unlock();
#if 1
			l_gtnum	=l_gtnum?1:0;
#endif
			for (int j = 0; j < l_gtnum; j++)
			{
				if (l_gate[j]->IsValid())
				{
					l_gate[j]->SendData(l_wpk);
				}
			}

			if(l_count ==2)
			{
				l_team->GetLeader()->LeaveTeam();
			}
			LogLine	l_line(g_LogTeam);
			/*l_line<<newln<<"���"<<pKicker->m_chaname[ply->m_currcha]<<"���ӳ��߳������"
				<<l_leader->m_chaname[l_leader->m_currcha]<<(l_count ==2?",�����ɢ��":"��")
				<<endln;
			*/
			l_line<<newln<<"player"<<pKicker->m_chaname[ply->m_currcha]<<"killed by captain"
				<<l_leader->m_chaname[l_leader->m_currcha]<<(l_count ==2?",free team.":".")
				<<endln;
		}
	}
}
void Player::TeamInvitedCheck(Invited	*invited)
{
	Player *l_inviter	=invited->m_ptinviter.m_ply;
	if(m_currcha<0)
	{
		TeamEndInvited(l_inviter);
	}else if(l_inviter->m_currcha <0 || l_inviter->m_chaid[l_inviter->m_currcha] !=invited->m_ptinviter.m_chaid)
	{
		net::WPacket l_wpk	=net::WPacket(256);
		l_wpk.WriteCmd(CMD_PC_TEAM_CANCEL);
		l_wpk.WriteInt64(MSG_TEAM_CANCLE_OFFLINE);
		l_wpk.WriteInt64(invited->m_ptinviter.m_chaid);
		g_gpsvr->SendToClient(this,l_wpk);
		TeamEndInvited(l_inviter);
	}else if(l_inviter->GetTeam() && l_inviter->GetTeam()->GetTotal()	>=g_gpsvr->const_team.MemberMax)
	{
		net::WPacket l_wpk	=net::WPacket(256);
		l_wpk.WriteCmd(CMD_PC_TEAM_CANCEL);
		l_wpk.WriteInt64(MSG_TEAM_CANCLE_ISFULL);
		l_wpk.WriteInt64(invited->m_ptinviter.m_chaid);
		g_gpsvr->SendToClient(this,l_wpk);
		TeamEndInvited(l_inviter);
	}else if(l_inviter->GetTeam() && l_inviter->GetLeader() !=l_inviter)
	{
		net::WPacket l_wpk	=net::WPacket(256);
		l_wpk.WriteCmd(CMD_PC_TEAM_CANCEL);
		l_wpk.WriteInt64(MSG_TEAM_CANCLE_CANCEL);
		l_wpk.WriteInt64(invited->m_ptinviter.m_chaid);
		g_gpsvr->SendToClient(this,l_wpk);
		TeamEndInvited(l_inviter);
	}else if(GetTeam() && GetTeam()->GetTotal()>1)
	{
		char l_buf[256];
		sprintf(l_buf,RES_STRING(GP_GROUPSERVERAPPTEAM_CPP_00028),m_chaname[m_currcha].c_str());
		l_inviter->SendSysInfo(l_buf);

		net::WPacket l_wpk	=net::WPacket(256);
		l_wpk.WriteCmd(CMD_PC_TEAM_CANCEL);
		l_wpk.WriteInt64(MSG_TEAM_CANCLE_CANCEL);
		l_wpk.WriteInt64(invited->m_ptinviter.m_chaid);
		g_gpsvr->SendToClient(this,l_wpk);
		TeamEndInvited(l_inviter);
	}else if(GetTickCount() -invited->m_tick	>=g_gpsvr->const_team.PendTimeOut)
	{
		char l_buf[256];
		sprintf(l_buf,RES_STRING(GP_GROUPSERVERAPPTEAM_CPP_00029),m_chaname[m_currcha].c_str(),g_gpsvr->const_team.PendTimeOut/1000);
		l_inviter->SendSysInfo(l_buf);

		net::WPacket l_wpk	=net::WPacket(256);
		l_wpk.WriteCmd(CMD_PC_TEAM_CANCEL);
		l_wpk.WriteInt64(MSG_TEAM_CANCLE_TIMEOUT);
		l_wpk.WriteInt64(invited->m_ptinviter.m_chaid);
		g_gpsvr->SendToClient(this,l_wpk);
		TeamEndInvited(l_inviter);
	}
}
void GroupServerApp::MP_SWITCH(Player *ply)
{
	Team	*l_team		=ply->GetTeam();
	if(!l_team)return;
	Player	*l_leader	=l_team->GetLeader();
	long	l_count		=l_team->GetTotal();
	{
		Player		*	l_plyr;
		GateServer	*	l_gate[3];for(int i=0;i<3;i++)l_gate[i]=0;
		char			l_gtnum	=0;

		net::WPacket	l_wpk	=net::WPacket(256);
		l_wpk.WriteCmd(CMD_PM_TEAM);
		l_wpk.WriteInt64(TEAM_MSG_UPDATE);
		l_wpk.WriteInt64(uChar(l_count));
		RunChainGetArmor<TeamMember> l(*l_team);
		for(int i =0;i<l_count &&((l_plyr=static_cast<Player*>(l_team->GetNextItem()))||(l_plyr =ply));i++)
		{
			l_wpk.WriteString(l_plyr->m_gate->m_name.c_str());
			l_wpk.WriteInt64(l_plyr->m_gtAddr);
			l_wpk.WriteInt64(l_plyr->m_chaid[l_plyr->m_currcha]);
			for(int j=0;j<3;j++)
			{
				if(l_gate[j] ==l_plyr->m_gate)
				{
					break;
				}
				if(!l_gate[j])
				{
					l_gate[j]	=l_plyr->m_gate;
					l_gtnum	++;
					break;
				}
			}
		}
		l.unlock();
#if 1
		l_gtnum	=l_gtnum?1:0;
#endif
		for (int j = 0; j < l_gtnum; j++)
		{
			if (l_gate[j]->IsValid())
			{
				l_gate[j]->SendData(l_wpk);
			}
		}

		LogLine	l_line(g_LogTeam);
		//l_line<<newln<<"���"<<ply->m_chaname[ply->m_currcha]<<"��Ϊ��ת��ͼˢ�������"<<l_leader->m_chaname[l_leader->m_currcha];
		l_line<<newln<<"player"<<ply->m_chaname[ply->m_currcha]<<"refresh team by switch map"<<l_leader->m_chaname[l_leader->m_currcha];
	}
}
