#include "stdafx.h"
#include <iostream>
#include "GroupServerApp.h"
#include "GameCommon.h"

void GroupServerApp::CP_CHANGE_PERSONINFO(Player *ply,net::TcpClient *client,net::RPacket &pk)
{
	auto	l_motto	=pk.ReadString();
	if(l_motto.empty() ||l_motto.size() >16 || !IsValidName(l_motto.c_str(),l_motto.size()))
	{
		return;
	}
	uShort		l_icon	=pk.ReadInt64();
	if(l_icon >const_cha.MaxIconVal)
	{
		ply->SendSysInfo(RES_STRING(GP_GROUPSERVERAPPPRSN_CPP_00001));
	}else if(strchr(l_motto.c_str(),'\'') || !CTextFilter::IsLegalText(CTextFilter::NAME_TABLE,l_motto.c_str()))
	{
		ply->SendSysInfo(RES_STRING(GP_GROUPSERVERAPPPRSN_CPP_00002));
	}else
	{
		{
			auto const l_lockDB = std::lock_guard{m_mtxDB};
			ply->m_refuse_sess	=pk.ReadInt64()?true:false;
			m_tblcharaters->UpdateInfo(ply->m_chaid[ply->m_currcha],l_icon,l_motto.c_str());
		}

		net::WPacket l_wpk(256);
		l_wpk.WriteCmd(CMD_PC_CHANGE_PERSONINFO);
		l_wpk.WriteString(l_motto);
		l_wpk.WriteInt64(l_icon);
		l_wpk.WriteInt64(ply->m_refuse_sess?1:0);
		SendToClient(ply,l_wpk);
	}
}

void GroupServerApp::CP_FRND_REFRESH_INFO(Player *ply,net::TcpClient *client,net::RPacket &pk)
{
	uLong	l_chaid	=pk.ReadInt64();
	m_mtxDB.lock();
	if(m_tblfriends->GetFriendsCount(ply->m_chaid[ply->m_currcha],l_chaid) !=2)
	{
		m_mtxDB.unlock();
		ply->SendSysInfo(RES_STRING(GP_GROUPSERVERAPPPRSN_CPP_00003));
	}else if(m_tblcharaters->FetchRowByChaID(l_chaid) ==1)
	{
		net::WPacket l_wpk(256);
		l_wpk.WriteCmd(CMD_PC_FRND_REFRESH_INFO);
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
void GroupServerApp::CP_REFUSETOME(Player *ply,net::TcpClient *client,net::RPacket &pk)
{
	ply->m_refuse_tome	=pk.ReadInt64()?true:false;
}
