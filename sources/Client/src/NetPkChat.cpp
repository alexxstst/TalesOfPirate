#include "StdAfx.h"
#include "NetChat.h"

#include "Character.h"
#include "Scene.h"
#include "GameApp.h"
#include "actor.h"
#include "NetProtocol.h"
#include "PacketCmd.h"
#include "GameAppMsg.h"
#include "CharacterRecord.h"
#include "DrawPointList.h"
#include "Algo.h"
#include "CommFunc.h"
#include "ShipSet.h"
#include "uistartform.h"
#include "UIGuildMgr.h"

// Типы uChar, uShort, uLong, cChar определены в NetIF.h

//----------------------------
// Э��C->S : ����ȫ��������Ϣ
//----------------------------
void  CS_GM1Say(const char *pszContent)
{
	WPacket pk	=g_NetIF->GetWPacket();
	pk.WriteCmd(CMD_CP_GM1SAY);	//����
	pk.WriteString(pszContent);

	g_NetIF->SendPacketMessage(pk);
}

//add by sunny.sun20080804
void  CS_GM1Say1(const char *pszContent, DWORD color)
{
	WPacket pk	=g_NetIF->GetWPacket();
	pk.WriteCmd(CMD_CP_GM1SAY1);	//����
	pk.WriteString(pszContent);
	pk.WriteInt64(color);
	g_NetIF->SendPacketMessage(pk);
}
//End
void  CS_Say2Trade(const char *pszContent)
{
	WPacket pk	=g_NetIF->GetWPacket();
	pk.WriteCmd(CMD_CP_SAY2TRADE);	//����
	pk.WriteString(pszContent);

	g_NetIF->SendPacketMessage(pk);
}
void CS_Say2All(const char *pszContent)
{
	WPacket pk	=g_NetIF->GetWPacket();
	pk.WriteCmd(CMD_CP_SAY2ALL);	//����
	pk.WriteString(pszContent);

	g_NetIF->SendPacketMessage(pk);
}
void  CP_RefuseToMe(bool refusetome)	//���þܾ�˽�ı�־
{
	WPacket	pk	=g_NetIF->GetWPacket();
	pk.WriteCmd(CMD_CP_REFUSETOME);
	pk.WriteInt64(refusetome?1:0);

	g_NetIF->SendPacketMessage(pk);
}
void  CS_Say2You(const char *you,const char *pszContent)
{
	WPacket pk	=g_NetIF->GetWPacket();
	pk.WriteCmd(CMD_CP_SAY2YOU);	//����
	pk.WriteString(you);
	pk.WriteString(pszContent);

	g_NetIF->SendPacketMessage(pk);
}
void  CS_Say2Team(const char *pszContent)
{
	WPacket l_wpk	=g_NetIF->GetWPacket();
	l_wpk.WriteCmd(CMD_CP_SAY2TEM);
	l_wpk.WriteString(pszContent);
	g_NetIF->SendPacketMessage(l_wpk);
}

void  CS_Say2Guild( const char* pszContent)
{
	WPacket l_wpk	=g_NetIF->GetWPacket();
	l_wpk.WriteCmd(CMD_CP_SAY2GUD);
	l_wpk.WriteString(pszContent);
	g_NetIF->SendPacketMessage(l_wpk);
}

void  CS_Team_Invite(const char *chaname)
{
	if( !g_stUIStart.IsCanTeamAndInfo() ) return;

	WPacket l_wpk	=g_NetIF->GetWPacket();
	l_wpk.WriteCmd(CMD_CP_TEAM_INVITE);
	l_wpk.WriteString(chaname);
	g_NetIF->SendPacketMessage(l_wpk);
}

void  CS_Team_Refuse(unsigned long chaid)
{
	WPacket l_wpk	=g_NetIF->GetWPacket();
	l_wpk.WriteCmd(CMD_CP_TEAM_REFUSE);
	l_wpk.WriteInt64(chaid);
	g_NetIF->SendPacketMessage(l_wpk);
}

void  CS_Team_Confirm(unsigned long chaid)
{
	if( !g_stUIStart.IsCanTeamAndInfo() ) return;

	WPacket l_wpk	=g_NetIF->GetWPacket();
	l_wpk.WriteCmd(CMD_CP_TEAM_ACCEPT);
	l_wpk.WriteInt64(chaid);
	g_NetIF->SendPacketMessage(l_wpk);
}

void  CS_Team_Kick( DWORD dwKickedID )
{
	if( !g_stUIStart.IsCanTeamAndInfo() ) return;

	WPacket l_wpk	=g_NetIF->GetWPacket();
	l_wpk.WriteCmd(CMD_CP_TEAM_KICK);
	l_wpk.WriteInt64( dwKickedID );
	g_NetIF->SendPacketMessage(l_wpk);
}

void  CS_Team_Leave()
{
	if( !g_stUIStart.IsCanTeamAndInfo() ) return;

	WPacket l_wpk	=g_NetIF->GetWPacket();
	l_wpk.WriteCmd(CMD_CP_TEAM_LEAVE);
	g_NetIF->SendPacketMessage(l_wpk);
}

void  CS_Frnd_Invite(const char *chaname)
{
	WPacket l_wpk	=g_NetIF->GetWPacket();
	l_wpk.WriteCmd(CMD_CP_FRND_INVITE);
	l_wpk.WriteString(chaname);
	g_NetIF->SendPacketMessage(l_wpk);
}
void  CS_Frnd_Refuse(unsigned long chaid)
{
	WPacket l_wpk	=g_NetIF->GetWPacket();
	l_wpk.WriteCmd(CMD_CP_FRND_REFUSE);
	l_wpk.WriteInt64(chaid);
	g_NetIF->SendPacketMessage(l_wpk);
}
void  CS_Frnd_Confirm(unsigned long chaid)
{
	WPacket l_wpk	=g_NetIF->GetWPacket();
	l_wpk.WriteCmd(CMD_CP_FRND_ACCEPT);
	l_wpk.WriteInt64(chaid);
	g_NetIF->SendPacketMessage(l_wpk);
}
void  CS_Frnd_Delete(unsigned long chaid)
{
	WPacket l_wpk	=g_NetIF->GetWPacket();
	l_wpk.WriteCmd(CMD_CP_FRND_DELETE);
	l_wpk.WriteInt64(chaid);
	g_NetIF->SendPacketMessage(l_wpk);
}

void  CP_Frnd_Refresh_Info(unsigned long chaid)
{
	WPacket l_wpk	=g_NetIF->GetWPacket();
	l_wpk.WriteCmd(CMD_CP_FRND_REFRESH_INFO);
	l_wpk.WriteInt64(chaid);
	g_NetIF->SendPacketMessage(l_wpk);
}

void CP_Change_PersonInfo(const char* motto, unsigned short icon, bool refuse_sess)
{
	WPacket l_wpk = g_NetIF->GetWPacket();
	l_wpk.WriteCmd(CMD_CP_CHANGE_PERSONINFO);
	l_wpk.WriteString(motto);
	l_wpk.WriteInt64(icon);
	l_wpk.WriteInt64(refuse_sess);
	g_NetIF->SendPacketMessage(l_wpk);
}

void  CS_Sess_Create(const char *chaname[],unsigned char chanum)
{
	WPacket	l_wpk	=g_NetIF->GetWPacket();
	l_wpk.WriteCmd(CMD_CP_SESS_CREATE);
	l_wpk.WriteInt64(chanum);
	for(char i=0;i<chanum; i++)
	{
		l_wpk.WriteString(chaname[i]);
	}

	g_NetIF->SendPacketMessage(l_wpk);
}
void  CS_Sess_Add(unsigned long sessid,const char *chaname)
{
	WPacket l_wpk	=g_NetIF->GetWPacket();
	l_wpk.WriteCmd(CMD_CP_SESS_ADD);
	l_wpk.WriteInt64(sessid);
	l_wpk.WriteString(chaname);

	g_NetIF->SendPacketMessage(l_wpk);
}
void  CS_Sess_Leave(unsigned long sessid)
{
	WPacket l_wpk	=g_NetIF->GetWPacket();
	l_wpk.WriteCmd(CMD_CP_SESS_LEAVE);
	l_wpk.WriteInt64(sessid);

	g_NetIF->SendPacketMessage(l_wpk);
}
void  CS_Sess_Say(unsigned long sessid,const char *word)
{
	WPacket l_wpk	=g_NetIF->GetWPacket();
	l_wpk.WriteCmd(CMD_CP_SESS_SAY);
	l_wpk.WriteInt64(sessid);
	l_wpk.WriteString(word);

	g_NetIF->SendPacketMessage(l_wpk);
}
//--------------------
// Э��S->C : ˽��
//--------------------
BOOL	PC_Say2You(LPRPACKET pk)
{
	stNetSay2You l_say;
	l_say.m_src		=pk.ReadString();
	l_say.m_dst		=pk.ReadString();
	l_say.m_content	=pk.ReadString();
	DWORD dwColour = pk.ReadInt64();
	NetSay2You(l_say,dwColour);
	return TRUE;
}
BOOL	PC_Say2Team(LPRPACKET pk)
{
	unsigned long l_chaid	=pk.ReadInt64();
	std::string	l_word	=pk.ReadString();
	DWORD dwColour = pk.ReadInt64();
	NetSay2Team(l_chaid,l_word.c_str(),dwColour);
	return TRUE;
}

BOOL	PC_Say2Gud(LPRPACKET pk)
{
	std::string l_src	=pk.ReadString();
	std::string	l_word	=pk.ReadString();
	DWORD dwColour = pk.ReadInt64();
	NetSay2Gud(l_src.c_str(),l_word.c_str(),dwColour);
	return TRUE;
}

BOOL	PC_Say2All(LPRPACKET pk)
{
	stNetSay2All l_say;
	l_say.m_src		=pk.ReadString();
	l_say.m_content	=pk.ReadString();
	DWORD dwColour = pk.ReadInt64();
	NetSay2All(l_say,dwColour);
	return TRUE;
}
BOOL	PC_GM1SAY(LPRPACKET pk)
{
	stNetSay2All l_say;
	l_say.m_src		=pk.ReadString();
	l_say.m_content	=pk.ReadString();
	NetGM1Say(l_say);
	return TRUE;
}
//Add by sunny.sun20080804
BOOL	PC_GM1SAY1(LPRPACKET pk)
{
	stNetScrollSay l_say;
	l_say.m_content	=pk.ReadString();
	l_say.setnum = pk.ReadInt64();
	l_say.color	= pk.ReadInt64();
	NetGM1Say1(l_say);
	return TRUE;
}
//End
BOOL	PC_SAY2TRADE(LPRPACKET pk)
{
	stNetSay2All l_say;
	l_say.m_src		=pk.ReadString();
	l_say.m_content	=pk.ReadString();
	DWORD dwColour = pk.ReadInt64();
	NetSay2Trade(l_say,dwColour);
	return TRUE;
}
BOOL	PC_SESS_CREATE(LPRPACKET pk)
{
	uLong	l_newsessid	=pk.ReadInt64();
	if(!l_newsessid)
	{
		NetSessCreate(pk.ReadString().c_str());
	}else
	{
		uShort	l_chanum	=pk.ReverseReadInt64();
		if(!l_chanum && l_chanum >100) return FALSE;

		stNetSessCreate l_nsc[100];
		for(uShort i=0;i<l_chanum;i++)
		{
			l_nsc[i].lChaID		=pk.ReadInt64();
			l_nsc[i].szChaName	=pk.ReadString();
			l_nsc[i].szMotto	=pk.ReadString();
			l_nsc[i].sIconID	=pk.ReadInt64();
		}
		NetSessCreate(l_newsessid,l_nsc,l_chanum);
	}
	return TRUE;
}
BOOL	PC_SESS_ADD(LPRPACKET pk)
{
	stNetSessCreate l_nsc;
	uLong	l_sessid=pk.ReadInt64();
	l_nsc.lChaID	=pk.ReadInt64();
	l_nsc.szChaName	=pk.ReadString();
	l_nsc.szMotto	=pk.ReadString();
	l_nsc.sIconID	=pk.ReadInt64();
	NetSessAdd(l_sessid,&l_nsc);
	return TRUE;
}
BOOL	PC_SESS_LEAVE(LPRPACKET pk)
{
	uLong l_sessid	=pk.ReadInt64();
	uLong l_chaid	=pk.ReadInt64();
	NetSessLeave(l_sessid,l_chaid);
	return TRUE;
}
BOOL	PC_SESS_SAY(LPRPACKET pk)
{
	uLong	l_sessid	=pk.ReadInt64();
	uLong	l_chaid		=pk.ReadInt64();
	std::string	l_word	=pk.ReadString();
	NetSessSay(l_sessid,l_chaid,l_word.c_str());
	return TRUE;
}
BOOL	PC_TEAM_INVITE(LPRPACKET pk)
{
	std::string l_inviter_name =pk.ReadString();
	uLong		 l_inviter_chaid=pk.ReadInt64();
	uShort		 l_inviter_icon	=pk.ReadInt64();
	NetTeamInvite(l_inviter_name.c_str(),l_inviter_chaid,l_inviter_icon);
	return TRUE;
}
BOOL	PC_TEAM_CANCEL(LPRPACKET pk)
{
	unsigned char reason =pk.ReadInt64();
	NetTeamCancel(pk.ReadInt64(),reason);
	return TRUE;
}
//��ӳ�Ա�仯��Ϣˢ��
BOOL	PC_TEAM_REFRESH(LPRPACKET pk)
{
	stNetPCTeam	l_pcteam;
	l_pcteam.kind	=pk.ReadInt64();
	l_pcteam.count	=pk.ReadInt64();

    LG("Team", "Kind:[%u], Count[%u]\n", l_pcteam.kind, l_pcteam.count );
	for(unsigned char i=0;i<l_pcteam.count;i++)
	{
		l_pcteam.cha_dbid[i]	=pk.ReadInt64();
		strncpy( l_pcteam.cha_name[i], pk.ReadString().c_str(), sizeof(l_pcteam.cha_name[i]) - 1 );
		strncpy( l_pcteam.motto[i], pk.ReadString().c_str(), sizeof(l_pcteam.motto[i]) - 1 );
		l_pcteam.cha_icon[i]		=pk.ReadInt64();

        LG("Team", "    DB_ID:[%u], Name[%s]\n", l_pcteam.cha_dbid[i], l_pcteam.cha_name[i] );
	}    

	NetPCTeam(l_pcteam);
	return TRUE;
}
BOOL PC_FRND_INVITE(LPRPACKET pk)
{
	std::string l_inviter_name =pk.ReadString();
	uLong		 l_inviter_chaid=pk.ReadInt64();
	uShort		 l_inviter_icon	=pk.ReadInt64();
	NetFrndInvite(l_inviter_name.c_str(),l_inviter_chaid,l_inviter_icon);
	return TRUE;
}
BOOL PC_FRND_CANCEL(LPRPACKET pk)
{
	unsigned char reason =pk.ReadInt64();
	NetFrndCancel(pk.ReadInt64(),reason);
	return TRUE;
}

BOOL PC_GM_INFO(LPRPACKET pk){
	unsigned char l_type =pk.ReadInt64();
	switch (l_type){
		case MSG_FRND_REFRESH_START:{
			stNetFrndStart l_nfs[100];
			uShort	l_grpnum	=pk.ReadInt64();
			for(uShort l_grpi =0;l_grpi<l_grpnum;l_grpi++){
				l_nfs[l_grpi].szGroup	= "GM";
				l_nfs[l_grpi].lChaid	=pk.ReadInt64();
				l_nfs[l_grpi].szChaname=pk.ReadString();
				l_nfs[l_grpi].szMotto	=pk.ReadString();
				l_nfs[l_grpi].sIconID	=pk.ReadInt64();
				l_nfs[l_grpi].cStatus	=pk.ReadInt64();
			}
			NetGMStart(l_nfs,l_grpnum);
			break;
		}
		case MSG_FRND_REFRESH_OFFLINE:{
			NetGMOffline(pk.ReadInt64());
			break;
		}
		case MSG_FRND_REFRESH_ONLINE:{
			NetGMOnline(pk.ReadInt64());
			break;
		}
		case MSG_FRND_REFRESH_DEL:{
			NetGMDel(pk.ReadInt64());
			break;
		}
		case MSG_FRND_REFRESH_ADD:
		{
			std::string	l_grp		=pk.ReadString();
			uLong	l_chaid		=pk.ReadInt64();
			std::string	l_chaname	=pk.ReadString();
			std::string	l_motto	=pk.ReadString();
			uShort	l_icon		=pk.ReadInt64();
			NetGMAdd(l_chaid,l_chaname.c_str(),l_motto.c_str(),l_icon,l_grp.c_str());
		}
		break;

	}
	return true;
}

BOOL PC_FRND_REFRESH(LPRPACKET pk)
{
	unsigned char l_type =pk.ReadInt64();
	switch (l_type)
	{
	case MSG_FRND_REFRESH_ONLINE:
		{
			NetFrndOnline(pk.ReadInt64());
		}
		break;
	case MSG_FRND_REFRESH_OFFLINE:
		{
			NetFrndOffline(pk.ReadInt64());
		}
		break;
	case MSG_FRND_REFRESH_DEL:
		{
			NetFrndDel(pk.ReadInt64());
		}
		break;
	case MSG_FRND_REFRESH_ADD:
		{
			std::string	l_grp		=pk.ReadString();
			uLong	l_chaid		=pk.ReadInt64();
			std::string	l_chaname	=pk.ReadString();
			std::string	l_motto	=pk.ReadString();
			uShort	l_icon		=pk.ReadInt64();
			NetFrndAdd(l_chaid,l_chaname.c_str(),l_motto.c_str(),l_icon,l_grp.c_str());
		}
		break;
	case MSG_FRND_REFRESH_START:
		{
			stNetFrndStart l_self;
			l_self.lChaid	=pk.ReadInt64();
			l_self.szChaname=pk.ReadString();
			l_self.szMotto	=pk.ReadString();
			l_self.sIconID	=pk.ReadInt64();

			stNetFrndStart l_nfs[100];

			uShort	l_nfnum=0,l_grpnum	=pk.ReadInt64();
			for(uShort l_grpi =0;l_grpi<l_grpnum;l_grpi++)
			{
				std::string	l_grp	=pk.ReadString();
				uShort	l_grpmnum	=pk.ReadInt64();
				for(uShort l_grpmi =0;l_grpmi<l_grpmnum;l_grpmi++)
				{
					l_nfs[l_nfnum].szGroup	=l_grp;
					l_nfs[l_nfnum].lChaid	=pk.ReadInt64();
					l_nfs[l_nfnum].szChaname=pk.ReadString();
					l_nfs[l_nfnum].szMotto	=pk.ReadString();
					l_nfs[l_nfnum].sIconID	=pk.ReadInt64();
					l_nfs[l_nfnum].cStatus	=pk.ReadInt64();
					l_nfnum	++;
				}
			}

			NetFrndStart(l_self,l_nfs,l_nfnum);
		}
		break;
	}
	return TRUE;
}

BOOL	PC_FRND_REFRESH_INFO(LPRPACKET pk)
{
	unsigned long l_chaid	=pk.ReadInt64();
	std::string	l_motto	=pk.ReadString();
	unsigned short l_icon	=pk.ReadInt64();
	unsigned short l_degr	=pk.ReadInt64();
	if(l_degr==0)
		l_degr=1;
	std::string	l_job	=pk.ReadString();
	std::string	l_guild	=pk.ReadString();

	NetFrndRefreshInfo(l_chaid,l_motto.c_str(),l_icon,l_degr,l_job.c_str(),l_guild.c_str());

	return TRUE;
}
// �����Լ���Ϣ
BOOL	PC_CHANGE_PERSONINFO(LPRPACKET pk)
{
	std::string l_motto	=pk.ReadString();
	unsigned short	l_icon	=pk.ReadInt64();
	bool		l_refuse_sess =pk.ReadInt64()?true:false;
	NetChangePersonInfo(l_motto.c_str(),l_icon,l_refuse_sess);
	return TRUE;
}