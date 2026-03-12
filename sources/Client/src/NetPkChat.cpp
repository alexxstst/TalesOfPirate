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
	pk.WriteUInt32(color);
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
	pk.WriteUInt8(refusetome?1:0);

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
	l_wpk.WriteUInt32(chaid);
	g_NetIF->SendPacketMessage(l_wpk);
}

void  CS_Team_Confirm(unsigned long chaid)
{
	if( !g_stUIStart.IsCanTeamAndInfo() ) return;

	WPacket l_wpk	=g_NetIF->GetWPacket();
	l_wpk.WriteCmd(CMD_CP_TEAM_ACCEPT);
	l_wpk.WriteUInt32(chaid);
	g_NetIF->SendPacketMessage(l_wpk);
}

void  CS_Team_Kick( DWORD dwKickedID )
{
	if( !g_stUIStart.IsCanTeamAndInfo() ) return;

	WPacket l_wpk	=g_NetIF->GetWPacket();
	l_wpk.WriteCmd(CMD_CP_TEAM_KICK);
	l_wpk.WriteUInt32( dwKickedID );
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
	l_wpk.WriteUInt32(chaid);
	g_NetIF->SendPacketMessage(l_wpk);
}
void  CS_Frnd_Confirm(unsigned long chaid)
{
	WPacket l_wpk	=g_NetIF->GetWPacket();
	l_wpk.WriteCmd(CMD_CP_FRND_ACCEPT);
	l_wpk.WriteUInt32(chaid);
	g_NetIF->SendPacketMessage(l_wpk);
}
void  CS_Frnd_Delete(unsigned long chaid)
{
	WPacket l_wpk	=g_NetIF->GetWPacket();
	l_wpk.WriteCmd(CMD_CP_FRND_DELETE);
	l_wpk.WriteUInt32(chaid);
	g_NetIF->SendPacketMessage(l_wpk);
}

void  CP_Frnd_Refresh_Info(unsigned long chaid)
{
	WPacket l_wpk	=g_NetIF->GetWPacket();
	l_wpk.WriteCmd(CMD_CP_FRND_REFRESH_INFO);
	l_wpk.WriteUInt32(chaid);
	g_NetIF->SendPacketMessage(l_wpk);
}

void CP_Change_PersonInfo(const char* motto, unsigned short icon, bool refuse_sess)
{
	WPacket l_wpk = g_NetIF->GetWPacket();
	l_wpk.WriteCmd(CMD_CP_CHANGE_PERSONINFO);
	l_wpk.WriteString(motto);
	l_wpk.WriteUInt16(icon);
	l_wpk.WriteUInt8(refuse_sess);
	g_NetIF->SendPacketMessage(l_wpk);
}

void  CS_Sess_Create(const char *chaname[],unsigned char chanum)
{
	WPacket	l_wpk	=g_NetIF->GetWPacket();
	l_wpk.WriteCmd(CMD_CP_SESS_CREATE);
	l_wpk.WriteUInt8(chanum);
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
	l_wpk.WriteUInt32(sessid);
	l_wpk.WriteString(chaname);

	g_NetIF->SendPacketMessage(l_wpk);
}
void  CS_Sess_Leave(unsigned long sessid)
{
	WPacket l_wpk	=g_NetIF->GetWPacket();
	l_wpk.WriteCmd(CMD_CP_SESS_LEAVE);
	l_wpk.WriteUInt32(sessid);

	g_NetIF->SendPacketMessage(l_wpk);
}
void  CS_Sess_Say(unsigned long sessid,const char *word)
{
	WPacket l_wpk	=g_NetIF->GetWPacket();
	l_wpk.WriteCmd(CMD_CP_SESS_SAY);
	l_wpk.WriteUInt32(sessid);
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
	DWORD dwColour = pk.ReadUInt32();
	NetSay2You(l_say,dwColour);
	return TRUE;
}
BOOL	PC_Say2Team(LPRPACKET pk)
{
	unsigned long l_chaid	=pk.ReadUInt32();
	const char	* l_word	=pk.ReadString();
	DWORD dwColour = pk.ReadUInt32();
	NetSay2Team(l_chaid,l_word,dwColour);
	return TRUE;
}

BOOL	PC_Say2Gud(LPRPACKET pk)
{
	const char  * l_src		=pk.ReadString();
	const char	* l_word	=pk.ReadString();
	DWORD dwColour = pk.ReadUInt32();
	NetSay2Gud(l_src,l_word,dwColour);
	return TRUE;
}

BOOL	PC_Say2All(LPRPACKET pk)
{
	stNetSay2All l_say;
	l_say.m_src		=pk.ReadString();
	l_say.m_content	=pk.ReadString();
	DWORD dwColour = pk.ReadUInt32();
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
	l_say.setnum = pk.ReadUInt32();
	l_say.color	= pk.ReadUInt32();
	NetGM1Say1(l_say);
	return TRUE;
}
//End
BOOL	PC_SAY2TRADE(LPRPACKET pk)
{
	stNetSay2All l_say;
	l_say.m_src		=pk.ReadString();
	l_say.m_content	=pk.ReadString();
	DWORD dwColour = pk.ReadUInt32();
	NetSay2Trade(l_say,dwColour);
	return TRUE;
}
BOOL	PC_SESS_CREATE(LPRPACKET pk)
{
	uLong	l_newsessid	=pk.ReadUInt32();
	if(!l_newsessid)
	{
		NetSessCreate(pk.ReadString());
	}else
	{
		uShort	l_chanum	=pk.ReverseReadUInt16();
		if(!l_chanum && l_chanum >100) return FALSE;

		stNetSessCreate l_nsc[100];
		for(uShort i=0;i<l_chanum;i++)
		{
			l_nsc[i].lChaID		=pk.ReadUInt32();
			l_nsc[i].szChaName	=pk.ReadString();
			l_nsc[i].szMotto	=pk.ReadString();
			l_nsc[i].sIconID	=pk.ReadUInt16();
		}
		NetSessCreate(l_newsessid,l_nsc,l_chanum);
	}
	return TRUE;
}
BOOL	PC_SESS_ADD(LPRPACKET pk)
{
	stNetSessCreate l_nsc;
	uLong	l_sessid=pk.ReadUInt32();
	l_nsc.lChaID	=pk.ReadUInt32();
	l_nsc.szChaName	=pk.ReadString();
	l_nsc.szMotto	=pk.ReadString();
	l_nsc.sIconID	=pk.ReadUInt16();
	NetSessAdd(l_sessid,&l_nsc);
	return TRUE;
}
BOOL	PC_SESS_LEAVE(LPRPACKET pk)
{
	uLong l_sessid	=pk.ReadUInt32();
	uLong l_chaid	=pk.ReadUInt32();
	NetSessLeave(l_sessid,l_chaid);
	return TRUE;
}
BOOL	PC_SESS_SAY(LPRPACKET pk)
{
	uLong	l_sessid	=pk.ReadUInt32();
	uLong	l_chaid		=pk.ReadUInt32();
	cChar*	l_word		=pk.ReadString();
	NetSessSay(l_sessid,l_chaid,l_word);
	return TRUE;
}
BOOL	PC_TEAM_INVITE(LPRPACKET pk)
{
	const char * l_inviter_name =pk.ReadString();
	uLong		 l_inviter_chaid=pk.ReadUInt32();
	uShort		 l_inviter_icon	=pk.ReadUInt16();
	NetTeamInvite(l_inviter_name,l_inviter_chaid,l_inviter_icon);
	return TRUE;
}
BOOL	PC_TEAM_CANCEL(LPRPACKET pk)
{
	unsigned char reason =pk.ReadUInt8();
	NetTeamCancel(pk.ReadUInt32(),reason);
	return TRUE;
}
//��ӳ�Ա�仯��Ϣˢ��
BOOL	PC_TEAM_REFRESH(LPRPACKET pk)
{
	stNetPCTeam	l_pcteam;
	l_pcteam.kind	=pk.ReadUInt8();
	l_pcteam.count	=pk.ReadUInt8();  

    LG("Team", "Kind:[%u], Count[%u]\n", l_pcteam.kind, l_pcteam.count );
	for(unsigned char i=0;i<l_pcteam.count;i++)
	{
		l_pcteam.cha_dbid[i]	=pk.ReadUInt32();
		strcpy( l_pcteam.cha_name[i], pk.ReadString() );
		strcpy( l_pcteam.motto[i], pk.ReadString() );
		l_pcteam.cha_icon[i]		=pk.ReadUInt16();

        LG("Team", "    DB_ID:[%u], Name[%s]\n", l_pcteam.cha_dbid[i], l_pcteam.cha_name[i] );
	}    

	NetPCTeam(l_pcteam);
	return TRUE;
}
BOOL PC_FRND_INVITE(LPRPACKET pk)
{
	const char * l_inviter_name =pk.ReadString();
	uLong		 l_inviter_chaid=pk.ReadUInt32();
	uShort		 l_inviter_icon	=pk.ReadUInt16();
	NetFrndInvite(l_inviter_name,l_inviter_chaid,l_inviter_icon);
	return TRUE;
}
BOOL PC_FRND_CANCEL(LPRPACKET pk)
{
	unsigned char reason =pk.ReadUInt8();
	NetFrndCancel(pk.ReadUInt32(),reason);
	return TRUE;
}

BOOL PC_GM_INFO(LPRPACKET pk){
	unsigned char l_type =pk.ReadUInt8();
	switch (l_type){
		case MSG_FRND_REFRESH_START:{
			stNetFrndStart l_nfs[100];
			uShort	l_grpnum	=pk.ReadUInt8();
			for(uShort l_grpi =0;l_grpi<l_grpnum;l_grpi++){
				l_nfs[l_grpi].szGroup	= "GM";
				l_nfs[l_grpi].lChaid	=pk.ReadUInt32();
				l_nfs[l_grpi].szChaname=pk.ReadString();
				l_nfs[l_grpi].szMotto	=pk.ReadString();
				l_nfs[l_grpi].sIconID	=pk.ReadUInt16();
				l_nfs[l_grpi].cStatus	=pk.ReadUInt8();
			}
			NetGMStart(l_nfs,l_grpnum);
			break;
		}
		case MSG_FRND_REFRESH_OFFLINE:{
			NetGMOffline(pk.ReadUInt32());
			break;
		}
		case MSG_FRND_REFRESH_ONLINE:{
			NetGMOnline(pk.ReadUInt32());
			break;
		}
		case MSG_FRND_REFRESH_DEL:{
			NetGMDel(pk.ReadUInt32());
			break;
		}
		case MSG_FRND_REFRESH_ADD:
		{
			cChar	*l_grp		=pk.ReadString();
			uLong	l_chaid		=pk.ReadUInt32();
			cChar	*l_chaname	=pk.ReadString();
			cChar	*l_motto	=pk.ReadString();
			uShort	l_icon		=pk.ReadUInt16();
			NetGMAdd(l_chaid,l_chaname,l_motto,l_icon,l_grp);
		}
		break;
		
	}
	return true;
}

BOOL PC_FRND_REFRESH(LPRPACKET pk)
{
	unsigned char l_type =pk.ReadUInt8();
	switch (l_type)
	{
	case MSG_FRND_REFRESH_ONLINE:
		{
			NetFrndOnline(pk.ReadUInt32());
		}
		break;
	case MSG_FRND_REFRESH_OFFLINE:
		{
			NetFrndOffline(pk.ReadUInt32());
		}
		break;
	case MSG_FRND_REFRESH_DEL:
		{
			NetFrndDel(pk.ReadUInt32());
		}
		break;
	case MSG_FRND_REFRESH_ADD:
		{
			cChar	*l_grp		=pk.ReadString();
			uLong	l_chaid		=pk.ReadUInt32();
			cChar	*l_chaname	=pk.ReadString();
			cChar	*l_motto	=pk.ReadString();
			uShort	l_icon		=pk.ReadUInt16();
			NetFrndAdd(l_chaid,l_chaname,l_motto,l_icon,l_grp);
		}
		break;
	case MSG_FRND_REFRESH_START:
		{
			stNetFrndStart l_self;
			l_self.lChaid	=pk.ReadUInt32();
			l_self.szChaname=pk.ReadString();
			l_self.szMotto	=pk.ReadString();
			l_self.sIconID	=pk.ReadUInt16();

			stNetFrndStart l_nfs[100];

			uShort	l_nfnum=0,l_grpnum	=pk.ReadUInt16();
			for(uShort l_grpi =0;l_grpi<l_grpnum;l_grpi++)
			{
				cChar*	l_grp		=pk.ReadString();
				uShort	l_grpmnum	=pk.ReadUInt16();
				for(uShort l_grpmi =0;l_grpmi<l_grpmnum;l_grpmi++)
				{
					l_nfs[l_nfnum].szGroup	=l_grp;
					l_nfs[l_nfnum].lChaid	=pk.ReadUInt32();
					l_nfs[l_nfnum].szChaname=pk.ReadString();
					l_nfs[l_nfnum].szMotto	=pk.ReadString();
					l_nfs[l_nfnum].sIconID	=pk.ReadUInt16();
					l_nfs[l_nfnum].cStatus	=pk.ReadUInt8();
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
	unsigned long l_chaid	=pk.ReadUInt32();
	const char	* l_motto	=pk.ReadString();
	unsigned short l_icon	=pk.ReadUInt16();
	unsigned short l_degr	=pk.ReadUInt16();
	if(l_degr==0)
		l_degr=1;
	const char	* l_job		=pk.ReadString();
	const char	* l_guild	=pk.ReadString();

	NetFrndRefreshInfo(l_chaid,l_motto,l_icon,l_degr,l_job,l_guild);

	return TRUE;
}
// �����Լ���Ϣ
BOOL	PC_CHANGE_PERSONINFO(LPRPACKET pk)
{
	const char *l_motto	=pk.ReadString();
	unsigned short	l_icon	=pk.ReadUInt16();
	bool		l_refuse_sess =pk.ReadUInt8()?true:false;
	NetChangePersonInfo(l_motto,l_icon,l_refuse_sess);
	return TRUE;
}