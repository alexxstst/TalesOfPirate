#include "stdafx.h"
#include <iostream>
#include "GroupServerApp.h"
#include "GameCommon.h"

void GroupServerApp::CP_CHANGE_PERSONINFO(Player *ply,DataSocket *datasock,RPacket &pk)
{
	uShort	l_len;
	cChar *l_motto	=pk.ReadString(&l_len);
	if(!l_motto ||l_len >16 || !IsValidName(l_motto,l_len))
	{
		return;
	}
	uShort		l_icon	=pk.ReadShort();
	if(l_icon >const_cha.MaxIconVal)
	{
		ply->SendSysInfo(RES_STRING(GP_GROUPSERVERAPPPRSN_CPP_00001));
	}else if(strchr(l_motto,'\'') || strlen(l_motto) !=l_len || !CTextFilter::IsLegalText(CTextFilter::NAME_TABLE,l_motto))
	{
		ply->SendSysInfo(RES_STRING(GP_GROUPSERVERAPPPRSN_CPP_00002));
	}else
	{
		{
			auto const l_lockDB = std::lock_guard{m_mtxDB};
			ply->m_refuse_sess	=pk.ReadChar()?true:false;
			m_tblcharaters->UpdateInfo(ply->m_chaid[ply->m_currcha],l_icon,l_motto);
		}

		WPacket	l_wpk	=GetWPacket();
		l_wpk.WriteCmd(CMD_PC_CHANGE_PERSONINFO);
		l_wpk.WriteString(l_motto);
		l_wpk.WriteShort(l_icon);
		l_wpk.WriteChar(ply->m_refuse_sess?1:0);
		SendToClient(ply,l_wpk);
	}
}

void GroupServerApp::CP_FRND_REFRESH_INFO(Player *ply,DataSocket *datasock,RPacket &pk)
{
	uLong	l_chaid	=pk.ReadLong();
	m_mtxDB.lock();
	if(m_tblfriends->GetFriendsCount(ply->m_chaid[ply->m_currcha],l_chaid) !=2)
	{
		m_mtxDB.unlock();
		ply->SendSysInfo(RES_STRING(GP_GROUPSERVERAPPPRSN_CPP_00003));
	}else if(m_tblcharaters->FetchRowByChaID(l_chaid) ==1)
	{
		WPacket	l_wpk	=GetWPacket();
		l_wpk.WriteCmd(CMD_PC_FRND_REFRESH_INFO);
		l_wpk.WriteLong(l_chaid);
		l_wpk.WriteString(m_tblcharaters->GetMotto());
		l_wpk.WriteShort(m_tblcharaters->GetIcon());
		l_wpk.WriteShort(m_tblcharaters->GetDegree());
		l_wpk.WriteString(m_tblcharaters->GetJob());
		l_wpk.WriteString(m_tblcharaters->GetGuildName());
		m_mtxDB.unlock();
		SendToClient(ply,l_wpk);
	}
}
void GroupServerApp::CP_REFUSETOME(Player *ply,DataSocket *datasock,RPacket &pk)
{
	ply->m_refuse_tome	=pk.ReadChar()?true:false;
}
