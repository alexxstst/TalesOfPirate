#include "stdafx.h"
#include <iostream>
#include "GroupServerApp.h"
#include "GameCommon.h"

using namespace std;

const cChar*	gc_master_group = RES_STRING(GP_GROUPSERVERAPPMASTER_CPP_00018);
const cChar*	gc_prentice_group = RES_STRING(GP_GROUPSERVERAPPMASTER_CPP_00017);

void GroupServerApp::CP_MASTER_REFRESH_INFO(Player *ply,net::TcpClient *client,net::RPacket &pk)
{
	uLong	l_chaid	=pk.ReadInt64();
	m_mtxDB.lock();
	//if(m_tblmaster->HasMaster(ply->m_chaid[ply->m_currcha],l_chaid) < 1)
	if(HasMaster(ply->m_chaid[ply->m_currcha],l_chaid) < 1)
	{
		m_mtxDB.unlock();
		//ply->SendSysInfo("");
		ply->SendSysInfo(RES_STRING(GP_GROUPSERVERAPPMASTER_CPP_00001));
	}else if(m_tblcharaters->FetchRowByChaID(l_chaid) ==1)
	{
		net::WPacket	l_wpk(256);
		l_wpk.WriteCmd(CMD_PC_MASTER_REFRESH_INFO);
		l_wpk.WriteInt64(l_chaid);
		l_wpk.WriteString(m_tblcharaters->GetMotto());
		l_wpk.WriteInt64(m_tblcharaters->GetIcon());
		l_wpk.WriteInt64(m_tblcharaters->GetDegree());
		l_wpk.WriteString(m_tblcharaters->GetJob());
		l_wpk.WriteString(m_tblcharaters->GetGuildName());
		m_mtxDB.unlock();
		SendToClient(ply,l_wpk);
	}
}

void GroupServerApp::CP_PRENTICE_REFRESH_INFO(Player *ply,net::TcpClient *client,net::RPacket &pk)
{
	uLong	l_chaid	=pk.ReadInt64();
	m_mtxDB.lock();
	//if(m_tblmaster->HasMaster(l_chaid, ply->m_chaid[ply->m_currcha]) < 1)
	if(HasMaster(l_chaid, ply->m_chaid[ply->m_currcha]) < 1)
	{
		m_mtxDB.unlock();
		//ply->SendSysInfo("");
		ply->SendSysInfo(RES_STRING(GP_GROUPSERVERAPPMASTER_CPP_00001));
	}else if(m_tblcharaters->FetchRowByChaID(l_chaid) ==1)
	{
		net::WPacket	l_wpk(256);
		l_wpk.WriteCmd(CMD_PC_PRENTICE_REFRESH_INFO);
		l_wpk.WriteInt64(l_chaid);
		l_wpk.WriteString(m_tblcharaters->GetMotto());
		l_wpk.WriteInt64(m_tblcharaters->GetIcon());
		l_wpk.WriteInt64(m_tblcharaters->GetDegree());
		l_wpk.WriteString(m_tblcharaters->GetJob());
		l_wpk.WriteString(m_tblcharaters->GetGuildName());
		m_mtxDB.unlock();
		SendToClient(ply,l_wpk);
	}
}

void GroupServerApp::MP_MASTER_CREATE(Player *ply,net::TcpClient *client,net::RPacket &pk)
{
	auto szPrenticeName = pk.ReadString();
	uLong l_prentice_chaid = pk.ReadInt64();
	auto szMasterName = pk.ReadString();
	uLong l_master_chaid = pk.ReadInt64();

	Player *pPrentice = FindPlayerByChaName(szPrenticeName.c_str());
	//Player *pPrentice = GetPlayerByChaID(l_prentice_chaid);
	Player *pMaster = FindPlayerByChaName(szMasterName.c_str());
	//Player *pMaster = GetPlayerByChaID(l_master_chaid);

	if(!pPrentice || !pMaster)
	{
		LogLine l_line(g_LogMaster);
		//l_line<<newln<<"MP_MASTER_CREATE()!";
		l_line<<newln<<"MP_MASTER_CREATE() member is offline!";
		return;
	}

	//
	bool bInvited = false;
	if(pPrentice->m_CurrMasterNum >= const_master.MasterMax)
	{
		//pMaster->SendSysInfo("!");
		pMaster->SendSysInfo(RES_STRING(GP_GROUPSERVERAPPMASTER_CPP_00003));
		//pPrentice->SendSysInfo("!");
		pPrentice->SendSysInfo(RES_STRING(GP_GROUPSERVERAPPMASTER_CPP_00004));
	}
	else if(m_tblmaster->GetPrenticeCount(l_master_chaid) >= const_master.PrenticeMax)
	{
		//pMaster->SendSysInfo("!");
		pMaster->SendSysInfo(RES_STRING(GP_GROUPSERVERAPPMASTER_CPP_00005));
		//pPrentice->SendSysInfo("!");
		pPrentice->SendSysInfo(RES_STRING(GP_GROUPSERVERAPPMASTER_CPP_00006));
	}
	else
	{
		Invited			*	l_invited	=0;
		uShort				l_len = (uShort)szMasterName.size();
		cChar			*	l_invited_name	= szMasterName.c_str();
		if(!l_invited_name || l_len >16)
		{
			return;
		}
		Player			*	l_invited_ply	= pMaster;
		auto const l_lockDB = std::lock_guard{m_mtxDB};
		if(!l_invited_ply || l_invited_ply->m_currcha <0 || l_invited_ply == pPrentice)
		{
			char l_buf[256];
			sprintf(l_buf,RES_STRING(GP_GROUPSERVERAPPMASTER_CPP_00007),l_invited_name);
			pPrentice->SendSysInfo(l_buf);
		}else if(l_invited = l_invited_ply->MasterFindInvitedByInviterChaID(pPrentice->m_chaid[pPrentice->m_currcha]))
		{
			//pPrentice->SendSysInfo(dstring("")<<l_invited_name<<"");
			char l_buf[256];
			sprintf(l_buf,RES_STRING(GP_GROUPSERVERAPPMASTER_CPP_00008),l_invited_name);
			pPrentice->SendSysInfo(l_buf);
		}else if(l_invited = pPrentice->MasterFindInvitedByInviterChaID(l_invited_ply->m_chaid[l_invited_ply->m_currcha]))
		{
			//pPrentice->SendSysInfo(dstring("")<<l_invited_name<<"");
			char l_buf[256];
			sprintf(l_buf,RES_STRING(GP_GROUPSERVERAPPMASTER_CPP_00009),l_invited_name);
			pPrentice->SendSysInfo(l_buf);
		//}else if(m_tblmaster->HasMaster(pPrentice->m_chaid[pPrentice->m_currcha], l_invited_ply->m_chaid[l_invited_ply->m_currcha]) > 0)
		}else if(HasMaster(pPrentice->m_chaid[pPrentice->m_currcha], l_invited_ply->m_chaid[l_invited_ply->m_currcha]) > 0)
		{
			//pPrentice->SendSysInfo("!");
			pPrentice->SendSysInfo(RES_STRING(GP_GROUPSERVERAPPMASTER_CPP_00011));
			//pMaster->SendSysInfo("!");
			pMaster->SendSysInfo(RES_STRING(GP_GROUPSERVERAPPMASTER_CPP_00011));
		}else
		{
			PtInviter l_ptinviter = l_invited_ply->MasterBeginInvited(pPrentice);
			if(l_ptinviter )
			{
				char l_buf[256];
				sprintf(l_buf,RES_STRING(GP_GROUPSERVERAPPMASTER_CPP_00012),l_invited_name);
				l_ptinviter->SendSysInfo(l_buf);
			}

			bInvited = true;
		}
	}

	//
	if(bInvited)
	{
		uLong		l_inviter_chaid	= l_prentice_chaid;
		PtInviter	l_inviter		= pMaster->MasterEndInvited(l_inviter_chaid);
		if(l_inviter && l_inviter->m_currcha >=0 && l_inviter.m_chaid == l_inviter->m_chaid[l_inviter->m_currcha])
		{
			auto const l_lockDB = std::lock_guard{m_mtxDB};
			++(pMaster->m_CurrPrenticeNum);
			if((++(pPrentice->m_CurrMasterNum)) >const_master.MasterMax)
			{
				--(pMaster->m_CurrPrenticeNum);
				--(pPrentice->m_CurrMasterNum);
				//pMaster->SendSysInfo(dstring("")<<l_inviter->m_chaname[l_inviter->m_currcha].c_str()<<"!");
				char l_buf[256];
				sprintf(l_buf,RES_STRING(GP_GROUPSERVERAPPMASTER_CPP_00013),l_inviter->m_chaname[l_inviter->m_currcha].c_str());
				pMaster->SendSysInfo(l_buf);
			}else
			{
				LogLine	l_line(g_LogMaster);
				/*
				l_line<<newln<<""<<pMaster->m_chaname[pMaster->m_currcha]<<"("<<pMaster->m_chaid[pMaster->m_currcha]
				<<")"<<l_inviter->m_chaname[l_inviter->m_currcha]<<"("<<l_inviter_chaid<<")!"
					<<endln;
				*/
				l_line<<newln<<"player"<<pMaster->m_chaname[pMaster->m_currcha]<<"("<<pMaster->m_chaid[pMaster->m_currcha]
				<<")and player"<<l_inviter->m_chaname[l_inviter->m_currcha]<<"("<<l_inviter_chaid<<")become Master!"
					<<endln;
				m_tblmaster->AddMaster(l_prentice_chaid, l_master_chaid);
				AddMaster(l_prentice_chaid, l_master_chaid);
				//pMaster->SendSysInfo("!");
				pMaster->SendSysInfo(RES_STRING(GP_GROUPSERVERAPPMASTER_CPP_00014));
				//pPrentice->SendSysInfo("!");
				pPrentice->SendSysInfo(RES_STRING(GP_GROUPSERVERAPPMASTER_CPP_00015));
				net::WPacket	l_wpk(256);
				l_wpk.WriteCmd(CMD_PC_MASTER_REFRESH);
				net::WPacket l_wpk2=l_wpk;
				l_wpk.WriteInt64(MSG_MASTER_REFRESH_ADD);
				l_wpk.WriteString(gc_master_group);
				l_wpk.WriteInt64(pMaster->m_chaid[pMaster->m_currcha]);
				l_wpk.WriteString(pMaster->m_chaname[pMaster->m_currcha].c_str());
				l_wpk.WriteString(pMaster->m_motto[pMaster->m_currcha].c_str());
				l_wpk.WriteInt64(pMaster->m_icon[pMaster->m_currcha]);
				SendToClient(l_inviter.m_ply,l_wpk);
				l_wpk2.WriteInt64(MSG_PRENTICE_REFRESH_ADD);
				l_wpk2.WriteString(gc_prentice_group);
				l_wpk2.WriteInt64(l_inviter->m_chaid[l_inviter->m_currcha]);
				l_wpk2.WriteString(l_inviter->m_chaname[l_inviter->m_currcha].c_str());
				l_wpk2.WriteString(l_inviter->m_motto[l_inviter->m_currcha].c_str());
				l_wpk2.WriteInt64(l_inviter->m_icon[l_inviter->m_currcha]);
				SendToClient(pMaster,l_wpk2);
			}
		}
	}
	else
	{
		LogLine l_line(g_LogMaster);
		//l_line<<newln<<"MP_MASTER_CREATE()";
		l_line<<newln<<"MP_MASTER_CREATE() invite failed";
	}
}

void GroupServerApp::MP_MASTER_DEL(Player *ply,net::TcpClient *client,net::RPacket &pk)
{
	auto szPrenticeName = pk.ReadString();
	uLong l_prentice_chaid = pk.ReadInt64();
	auto szMasterName = pk.ReadString();
	uLong l_master_chaid = pk.ReadInt64();

	Player *pPrentice = FindPlayerByChaName(szPrenticeName.c_str());
	//Player *pPrentice = GetPlayerByChaID(l_prentice_chaid);
	Player *pMaster = FindPlayerByChaName(szMasterName.c_str());
	//Player *pMaster = GetPlayerByChaID(l_master_chaid);

	auto const l_lockDB = std::lock_guard{m_mtxDB};
	//if(m_tblmaster->HasMaster(l_prentice_chaid,l_master_chaid) < 1)
	if(HasMaster(l_prentice_chaid,l_master_chaid) < 1)
	{
		return;
	}else
	{
		if(pPrentice)
		{
			net::WPacket	l_wpk(256);
			l_wpk.WriteCmd(CMD_PC_MASTER_REFRESH);
			l_wpk.WriteInt64(MSG_MASTER_REFRESH_DEL);
			l_wpk.WriteInt64(l_master_chaid);
			SendToClient(pPrentice,l_wpk);
			--(pPrentice->m_CurrMasterNum);
		}
		
		if(pMaster)
		{
			net::WPacket	l_wpk(256);
			l_wpk.WriteCmd(CMD_PC_MASTER_REFRESH);
			l_wpk.WriteInt64(MSG_PRENTICE_REFRESH_DEL);
			l_wpk.WriteInt64(l_prentice_chaid);
			SendToClient(pMaster,l_wpk);
			--(pMaster->m_CurrPrenticeNum);
		}

		m_tblmaster->DelMaster(l_prentice_chaid,l_master_chaid);
		DelMaster(l_prentice_chaid,l_master_chaid);
		LogLine	l_line(g_LogMaster);
		/*
		l_line<<newln<<""<<szMasterName<<"("<<l_master_chaid
		<<")<<szPrenticeName<<("<<l_prentice_chaid<<")";
		*/
		l_line<<newln<<"player"<<szMasterName<<"("<<l_master_chaid
		<<")and <<szPrenticeName<<("<<l_prentice_chaid<<"free master relation";
	}
}

void GroupServerApp::MP_MASTER_FINISH(Player *ply,net::TcpClient *client,net::RPacket &pk)
{
	uLong l_prentice_chaid = pk.ReadInt64();
	//if(m_tblmaster->GetMasterCount(l_prentice_chaid) > 0)
	if(GetMasterCount(l_prentice_chaid) > 0)
	{
		m_tblmaster->FinishMaster(l_prentice_chaid);
		//FinishMaster(l_prentice_chaid);
	}
}

void Player::MasterInvitedCheck(Invited	*invited)
{
	Player *l_inviter	=invited->m_ptinviter.m_ply;
	if(m_currcha <0)
	{
		MasterEndInvited(l_inviter);
	}else if(l_inviter->m_currcha <0 || l_inviter->m_chaid[l_inviter->m_currcha] !=invited->m_ptinviter.m_chaid)
	{
		net::WPacket l_wpk(256);
		l_wpk.WriteCmd(CMD_PC_MASTER_CANCEL);
		l_wpk.WriteInt64(MSG_MASTER_CANCLE_OFFLINE);
		l_wpk.WriteInt64(invited->m_ptinviter.m_chaid);
		g_gpsvr->SendToClient(this,l_wpk);
		MasterEndInvited(l_inviter);
	}else if(l_inviter->m_CurrMasterNum >= g_gpsvr->const_master.MasterMax)
	{
		net::WPacket l_wpk(256);
		l_wpk.WriteCmd(CMD_PC_MASTER_CANCEL);
		l_wpk.WriteInt64(MSG_MASTER_CANCLE_INVITER_ISFULL);
		l_wpk.WriteInt64(invited->m_ptinviter.m_chaid);
		g_gpsvr->SendToClient(this,l_wpk);
		MasterEndInvited(l_inviter);
	}/*else if(m_CurrPrenticeNum >= g_gpsvr->const_master.PrenticeMax)
	{
		net::WPacket l_wpk(256);
		l_wpk.WriteCmd(CMD_PC_MASTER_CANCEL);
		l_wpk.WriteInt64(MSG_MASTER_CANCLE_SELF_ISFULL);
		l_wpk.WriteInt64(invited->m_ptinviter.m_chaid);
		g_gpsvr->SendToClient(this,l_wpk);
		MasterEndInvited(l_inviter);
	}*/else if(GetTickCount() -invited->m_tick	>= g_gpsvr->const_master.PendTimeOut)
	{
		char l_buf[256];
		sprintf(l_buf,RES_STRING(GP_GROUPSERVERAPPMASTER_CPP_00016),m_chaname[m_currcha].c_str(),g_gpsvr->const_master.PendTimeOut/1000);
		l_inviter->SendSysInfo(l_buf);

		net::WPacket l_wpk(256);
		l_wpk.WriteCmd(CMD_PC_MASTER_CANCEL);
		l_wpk.WriteInt64(MSG_MASTER_CANCLE_TIMEOUT);
		l_wpk.WriteInt64(invited->m_ptinviter.m_chaid);
		g_gpsvr->SendToClient(this,l_wpk);
		MasterEndInvited(l_inviter);
	}
}

void GroupServerApp::PC_MASTER_INIT(Player *ply)
{
	TBLMaster::master_dat l_farray1[200];
	TBLMaster::master_dat l_farray2[200];
	int l_num1{ std::size(l_farray1) };
	int l_num2{ std::size(l_farray2) };

	//
	m_tblmaster->GetPrenticeData(l_farray1,l_num1,ply->m_chaid[ply->m_currcha]);

	net::WPacket	l_toPrentice(256);
	l_toPrentice.WriteCmd(CMD_PC_MASTER_REFRESH);
	l_toPrentice.WriteInt64(MSG_MASTER_REFRESH_ONLINE);
	l_toPrentice.WriteInt64(ply->m_chaid[ply->m_currcha]);

	net::WPacket	l_toSelf1(256);
	l_toSelf1.WriteCmd(CMD_PC_MASTER_REFRESH);
	l_toSelf1.WriteInt64(MSG_PRENTICE_REFRESH_START);

	l_toSelf1.WriteInt64(ply->m_chaid[ply->m_currcha]);
	l_toSelf1.WriteString(ply->m_chaname[ply->m_currcha].c_str());
	l_toSelf1.WriteString(ply->m_motto[ply->m_currcha].c_str());
	l_toSelf1.WriteInt64(ply->m_icon[ply->m_currcha]);

	ply->m_CurrPrenticeNum	= 0;

	std::array<Player*, std::size(l_farray1)> l_plylst1;
	short	l_plynum1	=0;

	Player	*	l_ply11;char	l_currcha1;
	for (int i = 0; i < l_num1; i++)
	{
		if (l_farray1[i].atorID == 0)
		{
			if (l_farray1[i].icon_id == 0)
			{
				l_toSelf1.WriteInt64(uShort(l_farray1[i].memaddr));
			}
			else
			{
				//l_toSelf1.WriteString(l_farray1[i].relation.c_str());
				//l_toSelf1.WriteString("");
				l_toSelf1.WriteString(RES_STRING(GP_GROUPSERVERAPPMASTER_CPP_00017));
				l_toSelf1.WriteInt64(uShort(l_farray1[i].memaddr));
				ply->m_CurrPrenticeNum += l_farray1[i].memaddr;
			}
		}else if((l_ply11 =ToPointer<Player>(l_farray1[i].memaddr))
			&&((l_currcha1 =l_ply11->m_currcha)>=0)
			&&(l_ply11->m_chaid[l_currcha1] ==l_farray1[i].atorID))
		{
			l_plylst1[l_plynum1] = l_ply11;
			++l_plynum1;

			l_toSelf1.WriteInt64(l_farray1[i].atorID);
			l_toSelf1.WriteString(l_farray1[i].atorNome.c_str());
			l_toSelf1.WriteString(l_farray1[i].motto.c_str());
			l_toSelf1.WriteInt64(l_farray1[i].icon_id);
			l_toSelf1.WriteInt64(1);
		}
		else
		{
			l_toSelf1.WriteInt64(l_farray1[i].atorID);
			l_toSelf1.WriteString(l_farray1[i].atorNome.c_str());
			l_toSelf1.WriteString(l_farray1[i].motto.c_str());
			l_toSelf1.WriteInt64(l_farray1[i].icon_id);
			l_toSelf1.WriteInt64(0);
		}
	}
	SendToClient(ply,l_toSelf1);
	LogLine	l_line1(g_LogMaster);
	//l_line1<<newln<<""<<l_plynum1<<endln;
	l_line1<<newln<<"online notice apprentice num:"<<l_plynum1<<endln;
	SendToClient(l_plylst1.data(),l_plynum1,l_toPrentice);

	//
	m_tblmaster->GetMasterData(l_farray2,l_num2,ply->m_chaid[ply->m_currcha]);

	net::WPacket	l_toMaster(256);
	l_toMaster.WriteCmd(CMD_PC_MASTER_REFRESH);
	l_toMaster.WriteInt64(MSG_PRENTICE_REFRESH_ONLINE);
	l_toMaster.WriteInt64(ply->m_chaid[ply->m_currcha]);

	net::WPacket	l_toSelf2(256);
	l_toSelf2.WriteCmd(CMD_PC_MASTER_REFRESH);
	l_toSelf2.WriteInt64(MSG_MASTER_REFRESH_START);

	l_toSelf2.WriteInt64(ply->m_chaid[ply->m_currcha]);
	l_toSelf2.WriteString(ply->m_chaname[ply->m_currcha].c_str());
	l_toSelf2.WriteString(ply->m_motto[ply->m_currcha].c_str());
	l_toSelf2.WriteInt64(ply->m_icon[ply->m_currcha]);

	ply->m_CurrPrenticeNum	= 0;

	std::array<Player*, std::size(l_farray2)> l_plylst2;
	short	l_plynum2	=0;

	Player* l_ply12; char	l_currcha2;
	for (auto i = 0; i < l_num2; ++i)
	{
		if (l_farray2[i].atorID == 0)
		{
			if (l_farray2[i].icon_id == 0)
			{
				l_toSelf2.WriteInt64(uShort(l_farray2[i].memaddr));
			}
			else
			{
				l_toSelf2.WriteString(RES_STRING(GP_GROUPSERVERAPPMASTER_CPP_00018));
				l_toSelf2.WriteInt64(uShort(l_farray2[i].memaddr));
				ply->m_CurrMasterNum += l_farray2[i].memaddr;
			}
		}
		else if ((l_ply12 = ToPointer<Player>(l_farray2[i].memaddr))
			&&((l_currcha2 =l_ply12->m_currcha)>=0)
			&&(l_ply12->m_chaid[l_currcha2] ==l_farray2[i].atorID))
		{
			l_plylst2[l_plynum2] = l_ply12;
			l_plynum2++;

			l_toSelf2.WriteInt64(l_farray2[i].atorID);
			l_toSelf2.WriteString(l_farray2[i].atorNome.c_str());
			l_toSelf2.WriteString(l_farray2[i].motto.c_str());
			l_toSelf2.WriteInt64(l_farray2[i].icon_id);
			l_toSelf2.WriteInt64(1);
		}
		else
		{
			l_toSelf2.WriteInt64(l_farray2[i].atorID);
			l_toSelf2.WriteString(l_farray2[i].atorNome.c_str());
			l_toSelf2.WriteString(l_farray2[i].motto.c_str());
			l_toSelf2.WriteInt64(l_farray2[i].icon_id);
			l_toSelf2.WriteInt64(0);
		}
	}
	SendToClient(ply,l_toSelf2);
	LogLine	l_line2(g_LogMaster);
	//l_line2<<newln<<""<<l_plynum2<<endln;
	l_line2<<newln<<"online notice master num:"<<l_plynum2<<endln;
	SendToClient(l_plylst2.data(), l_plynum2, l_toMaster);
}

bool GroupServerApp::InitMasterRelation()
{
	if(m_tblmaster->InitMasterRelation(m_mapMasterRelation))
		return true;
	return false;
}

int GroupServerApp::GetMasterCount(uLong atorID)
{
	map<uLong, uLong>::iterator it = m_mapMasterRelation.find(atorID);
	if(it != m_mapMasterRelation.end())
	{
		return 1;
	}
	return 0;
}

int GroupServerApp::GetPrenticeCount(uLong atorID)
{
	return 0;
}

int GroupServerApp::HasMaster(uLong cha_id1,uLong cha_id2)
{
	map<uLong, uLong>::iterator it = m_mapMasterRelation.find(cha_id1);
	if(it != m_mapMasterRelation.end())
	{
		if(m_mapMasterRelation[cha_id1] == cha_id2)
			return 1;
	}
	return 0;
}

bool GroupServerApp::AddMaster(uLong cha_id1,uLong cha_id2)
{
	map<uLong, uLong>::iterator it = m_mapMasterRelation.find(cha_id1);
	if(it != m_mapMasterRelation.end())
	{
		return false;
	}

	m_mapMasterRelation[cha_id1] = cha_id2;
	return true;
}

bool GroupServerApp::DelMaster(uLong cha_id1,uLong cha_id2)
{
	map<uLong, uLong>::iterator it = m_mapMasterRelation.find(cha_id1);
	if(it != m_mapMasterRelation.end())
	{
		if(m_mapMasterRelation[cha_id1] == cha_id2)
		{
			m_mapMasterRelation.erase(it);
			return true;
		}
	}
	return false;
}

bool GroupServerApp::FinishMaster(uLong atorID)
{
	map<uLong, uLong>::iterator it = m_mapMasterRelation.find(atorID);
	if(it != m_mapMasterRelation.end())
	{
		m_mapMasterRelation.erase(it);
		return true;
	}
	return false;
}

