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

_DBC_USING

//----------------------------
// C->S : 
//----------------------------
void  CS_GM1Say(const char *pszContent)
{
	WPacket pk	=g_NetIF->GetWPacket();
	pk.WriteCmd(CMD_CP_GM1SAY);	//
	pk.WriteString(pszContent);

	g_NetIF->SendPacketMessage(pk);
}

//add by sunny.sun20080804
void  CS_GM1Say1(const char *pszContent, DWORD color)
{
	WPacket pk	=g_NetIF->GetWPacket();
	pk.WriteCmd(CMD_CP_GM1SAY1);	//
	pk.WriteString(pszContent);
	pk.WriteLong(color);
	g_NetIF->SendPacketMessage(pk);
}
//End
void  CS_Say2Trade(const char *pszContent)
{
	WPacket pk	=g_NetIF->GetWPacket();
	pk.WriteCmd(CMD_CP_SAY2TRADE);	//
	pk.WriteString(pszContent);

	g_NetIF->SendPacketMessage(pk);
}
void CS_Say2All(const char *pszContent)
{
	WPacket pk	=g_NetIF->GetWPacket();
	pk.WriteCmd(CMD_CP_SAY2ALL);	//
	pk.WriteString(pszContent);

	g_NetIF->SendPacketMessage(pk);
}
void  CP_RefuseToMe(bool refusetome)	//
{
	WPacket	pk	=g_NetIF->GetWPacket();
	pk.WriteCmd(CMD_CP_REFUSETOME);
	pk.WriteChar(refusetome?1:0);

	g_NetIF->SendPacketMessage(pk);
}
void  CS_Say2You(const char *you,const char *pszContent)
{
	WPacket pk	=g_NetIF->GetWPacket();
	pk.WriteCmd(CMD_CP_SAY2YOU);	//
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
	l_wpk.WriteLong(chaid);
	g_NetIF->SendPacketMessage(l_wpk);
}

void  CS_Team_Confirm(unsigned long chaid)
{
	if( !g_stUIStart.IsCanTeamAndInfo() ) return;

	WPacket l_wpk	=g_NetIF->GetWPacket();
	l_wpk.WriteCmd(CMD_CP_TEAM_ACCEPT);
	l_wpk.WriteLong(chaid);
	g_NetIF->SendPacketMessage(l_wpk);
}

void  CS_Team_Kick( DWORD dwKickedID )
{
	if( !g_stUIStart.IsCanTeamAndInfo() ) return;

	WPacket l_wpk	=g_NetIF->GetWPacket();
	l_wpk.WriteCmd(CMD_CP_TEAM_KICK);
	l_wpk.WriteLong( dwKickedID );
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
	l_wpk.WriteLong(chaid);
	g_NetIF->SendPacketMessage(l_wpk);
}
void  CS_Frnd_Confirm(unsigned long chaid)
{
	WPacket l_wpk	=g_NetIF->GetWPacket();
	l_wpk.WriteCmd(CMD_CP_FRND_ACCEPT);
	l_wpk.WriteLong(chaid);
	g_NetIF->SendPacketMessage(l_wpk);
}
void  CS_Frnd_Delete(unsigned long chaid)
{
	WPacket l_wpk	=g_NetIF->GetWPacket();
	l_wpk.WriteCmd(CMD_CP_FRND_DELETE);
	l_wpk.WriteLong(chaid);
	g_NetIF->SendPacketMessage(l_wpk);
}

void  CP_Frnd_Refresh_Info(unsigned long chaid)
{
	WPacket l_wpk	=g_NetIF->GetWPacket();
	l_wpk.WriteCmd(CMD_CP_FRND_REFRESH_INFO);
	l_wpk.WriteLong(chaid);
	g_NetIF->SendPacketMessage(l_wpk);
}

void CP_Change_PersonInfo(const char* motto, unsigned short icon, bool refuse_sess)
{
	WPacket l_wpk = g_NetIF->GetWPacket();
	l_wpk.WriteCmd(CMD_CP_CHANGE_PERSONINFO);
	l_wpk.WriteString(motto);
	l_wpk.WriteShort(icon);
	l_wpk.WriteChar(refuse_sess);
	g_NetIF->SendPacketMessage(l_wpk);
}

void  CS_Sess_Create(const char *chaname[],unsigned char chanum)
{
	WPacket	l_wpk	=g_NetIF->GetWPacket();
	l_wpk.WriteCmd(CMD_CP_SESS_CREATE);
	l_wpk.WriteChar(chanum);
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
	l_wpk.WriteLong(sessid);
	l_wpk.WriteString(chaname);

	g_NetIF->SendPacketMessage(l_wpk);
}
void  CS_Sess_Leave(unsigned long sessid)
{
	WPacket l_wpk	=g_NetIF->GetWPacket();
	l_wpk.WriteCmd(CMD_CP_SESS_LEAVE);
	l_wpk.WriteLong(sessid);

	g_NetIF->SendPacketMessage(l_wpk);
}
void  CS_Sess_Say(unsigned long sessid,const char *word)
{
	WPacket l_wpk	=g_NetIF->GetWPacket();
	l_wpk.WriteCmd(CMD_CP_SESS_SAY);
	l_wpk.WriteLong(sessid);
	l_wpk.WriteString(word);

	g_NetIF->SendPacketMessage(l_wpk);
}
//--------------------
// S->C : 
//--------------------
BOOL	PC_Say2You(LPRPACKET pk)
{
	stNetSay2You l_say;
	l_say.m_src		=pk.ReadString();
	l_say.m_dst		=pk.ReadString();
	l_say.m_content	=pk.ReadString();
	DWORD dwColour = pk.ReadLong();
	NetSay2You(l_say,dwColour);
	return TRUE;
}
BOOL	PC_Say2Team(LPRPACKET pk)
{
	unsigned long l_chaid	=pk.ReadLong();
	const char	* l_word	=pk.ReadString();
	DWORD dwColour = pk.ReadLong();
	NetSay2Team(l_chaid,l_word,dwColour);
	return TRUE;
}

BOOL	PC_Say2Gud(LPRPACKET pk)
{
	const char  * l_src		=pk.ReadString();
	const char	* l_word	=pk.ReadString();
	DWORD dwColour = pk.ReadLong();
	NetSay2Gud(l_src,l_word,dwColour);
	return TRUE;
}

BOOL	PC_Say2All(LPRPACKET pk)
{
	stNetSay2All l_say;
	l_say.m_src		=pk.ReadString();
	l_say.m_content	=pk.ReadString();
	DWORD dwColour = pk.ReadLong();
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
	l_say.setnum = pk.ReadLong();
	l_say.color	= pk.ReadLong();
	NetGM1Say1(l_say);
	return TRUE;
}
//End
BOOL	PC_SAY2TRADE(LPRPACKET pk)
{
	stNetSay2All l_say;
	l_say.m_src		=pk.ReadString();
	l_say.m_content	=pk.ReadString();
	DWORD dwColour = pk.ReadLong();
	NetSay2Trade(l_say,dwColour);
	return TRUE;
}
BOOL	PC_SESS_CREATE(LPRPACKET pk)
{
	uLong	l_newsessid	=pk.ReadLong();
	if(!l_newsessid)
	{
		NetSessCreate(pk.ReadString());
	}else
	{
		uShort	l_chanum	=pk.ReverseReadShort();
		if(!l_chanum && l_chanum >100) return FALSE;

		stNetSessCreate l_nsc[100];
		for(uShort i=0;i<l_chanum;i++)
		{
			l_nsc[i].lChaID		=pk.ReadLong();
			l_nsc[i].szChaName	=pk.ReadString();
			l_nsc[i].szMotto	=pk.ReadString();
			l_nsc[i].sIconID	=pk.ReadShort();
		}
		NetSessCreate(l_newsessid,l_nsc,l_chanum);
	}
	return TRUE;
}
BOOL	PC_SESS_ADD(LPRPACKET pk)
{
	stNetSessCreate l_nsc;
	uLong	l_sessid=pk.ReadLong();
	l_nsc.lChaID	=pk.ReadLong();
	l_nsc.szChaName	=pk.ReadString();
	l_nsc.szMotto	=pk.ReadString();
	l_nsc.sIconID	=pk.ReadShort();
	NetSessAdd(l_sessid,&l_nsc);
	return TRUE;
}
BOOL	PC_SESS_LEAVE(LPRPACKET pk)
{
	uLong l_sessid	=pk.ReadLong();
	uLong l_chaid	=pk.ReadLong();
	NetSessLeave(l_sessid,l_chaid);
	return TRUE;
}
BOOL	PC_SESS_SAY(LPRPACKET pk)
{
	uLong	l_sessid	=pk.ReadLong();
	uLong	l_chaid		=pk.ReadLong();
	cChar*	l_word		=pk.ReadString();
	NetSessSay(l_sessid,l_chaid,l_word);
	return TRUE;
}
BOOL	PC_TEAM_INVITE(LPRPACKET pk)
{
	const char * l_inviter_name =pk.ReadString();
	uLong		 l_inviter_chaid=pk.ReadLong();
	uShort		 l_inviter_icon	=pk.ReadShort();
	NetTeamInvite(l_inviter_name,l_inviter_chaid,l_inviter_icon);
	return TRUE;
}
BOOL	PC_TEAM_CANCEL(LPRPACKET pk)
{
	unsigned char reason =pk.ReadChar();
	NetTeamCancel(pk.ReadLong(),reason);
	return TRUE;
}
//
BOOL	PC_TEAM_REFRESH(LPRPACKET pk)
{
	stNetPCTeam	l_pcteam;
	l_pcteam.kind	=pk.ReadChar();
	l_pcteam.count	=pk.ReadChar();  

    ToLogService("Team", "Kind:[{}], Count[{}]", l_pcteam.kind, l_pcteam.count);
	for(unsigned char i=0;i<l_pcteam.count;i++)
	{
		l_pcteam.cha_dbid[i]	=pk.ReadLong();
		strcpy( l_pcteam.cha_name[i], pk.ReadString() );
		strcpy( l_pcteam.motto[i], pk.ReadString() );
		l_pcteam.cha_icon[i]		=pk.ReadShort();

        ToLogService("Team", "    DB_ID:[{}], Name[{}]", l_pcteam.cha_dbid[i], l_pcteam.cha_name[i]);
	}    

	NetPCTeam(l_pcteam);
	return TRUE;
}
BOOL PC_FRND_INVITE(LPRPACKET pk)
{
	const char * l_inviter_name =pk.ReadString();
	uLong		 l_inviter_chaid=pk.ReadLong();
	uShort		 l_inviter_icon	=pk.ReadShort();
	NetFrndInvite(l_inviter_name,l_inviter_chaid,l_inviter_icon);
	return TRUE;
}
BOOL PC_FRND_CANCEL(LPRPACKET pk)
{
	unsigned char reason =pk.ReadChar();
	NetFrndCancel(pk.ReadLong(),reason);
	return TRUE;
}

BOOL PC_GM_INFO(LPRPACKET pk){
	unsigned char l_type =pk.ReadChar();
	switch (l_type){
		case MSG_FRND_REFRESH_START:{
			stNetFrndStart l_nfs[100];
			uShort	l_grpnum	=pk.ReadChar();
			for(uShort l_grpi =0;l_grpi<l_grpnum;l_grpi++){
				l_nfs[l_grpi].szGroup	= "GM";
				l_nfs[l_grpi].lChaid	=pk.ReadLong();
				l_nfs[l_grpi].szChaname=pk.ReadString();
				l_nfs[l_grpi].szMotto	=pk.ReadString();
				l_nfs[l_grpi].sIconID	=pk.ReadShort();
				l_nfs[l_grpi].cStatus	=pk.ReadChar();
			}
			NetGMStart(l_nfs,l_grpnum);
			break;
		}
		case MSG_FRND_REFRESH_OFFLINE:{
			NetGMOffline(pk.ReadLong());
			break;
		}
		case MSG_FRND_REFRESH_ONLINE:{
			NetGMOnline(pk.ReadLong());
			break;
		}
		case MSG_FRND_REFRESH_DEL:{
			NetGMDel(pk.ReadLong());
			break;
		}
		case MSG_FRND_REFRESH_ADD:
		{
			cChar	*l_grp		=pk.ReadString();
			uLong	l_chaid		=pk.ReadLong();
			cChar	*l_chaname	=pk.ReadString();
			cChar	*l_motto	=pk.ReadString();
			uShort	l_icon		=pk.ReadShort();
			NetGMAdd(l_chaid,l_chaname,l_motto,l_icon,l_grp);
		}
		break;
		
	}
	return true;
}

BOOL PC_FRND_REFRESH(LPRPACKET pk)
{
	unsigned char l_type =pk.ReadChar();
	switch (l_type)
	{
	case MSG_FRND_REFRESH_ONLINE:
		{
			NetFrndOnline(pk.ReadLong());
		}
		break;
	case MSG_FRND_REFRESH_OFFLINE:
		{
			NetFrndOffline(pk.ReadLong());
		}
		break;
	case MSG_FRND_REFRESH_DEL:
		{
			NetFrndDel(pk.ReadLong());
		}
		break;
	case MSG_FRND_REFRESH_ADD:
		{
			cChar	*l_grp		=pk.ReadString();
			uLong	l_chaid		=pk.ReadLong();
			cChar	*l_chaname	=pk.ReadString();
			cChar	*l_motto	=pk.ReadString();
			uShort	l_icon		=pk.ReadShort();
			NetFrndAdd(l_chaid,l_chaname,l_motto,l_icon,l_grp);
		}
		break;
	case MSG_FRND_REFRESH_START:
		{
			stNetFrndStart l_self;
			l_self.lChaid	=pk.ReadLong();
			l_self.szChaname=pk.ReadString();
			l_self.szMotto	=pk.ReadString();
			l_self.sIconID	=pk.ReadShort();

			stNetFrndStart l_nfs[100];

			uShort	l_nfnum=0,l_grpnum	=pk.ReadShort();
			for(uShort l_grpi =0;l_grpi<l_grpnum;l_grpi++)
			{
				cChar*	l_grp		=pk.ReadString();
				uShort	l_grpmnum	=pk.ReadShort();
				for(uShort l_grpmi =0;l_grpmi<l_grpmnum;l_grpmi++)
				{
					l_nfs[l_nfnum].szGroup	=l_grp;
					l_nfs[l_nfnum].lChaid	=pk.ReadLong();
					l_nfs[l_nfnum].szChaname=pk.ReadString();
					l_nfs[l_nfnum].szMotto	=pk.ReadString();
					l_nfs[l_nfnum].sIconID	=pk.ReadShort();
					l_nfs[l_nfnum].cStatus	=pk.ReadChar();
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
	unsigned long l_chaid	=pk.ReadLong();
	const char	* l_motto	=pk.ReadString();
	unsigned short l_icon	=pk.ReadShort();
	unsigned short l_degr	=pk.ReadShort();
	if(l_degr==0)
		l_degr=1;
	const char	* l_job		=pk.ReadString();
	const char	* l_guild	=pk.ReadString();

	NetFrndRefreshInfo(l_chaid,l_motto,l_icon,l_degr,l_job,l_guild);

	return TRUE;
}
// 
BOOL	PC_CHANGE_PERSONINFO(LPRPACKET pk)
{
	const char *l_motto	=pk.ReadString();
	unsigned short	l_icon	=pk.ReadShort();
	bool		l_refuse_sess =pk.ReadChar()?true:false;
	NetChangePersonInfo(l_motto,l_icon,l_refuse_sess);
	return TRUE;
}
