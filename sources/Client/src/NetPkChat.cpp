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
#include "CommandMessages.h"

//  uChar, uShort, uLong, cChar   NetIF.h

// =================================================================
//   CP- ( -> GroupServer  GateServer)
// =================================================================

//----------------------------
// GM- (  )
//----------------------------
void CS_GM1Say(const char* pszContent) {
	auto pk = net::msg::serializeClient(net::msg::CpGm1SayMessage{pszContent});
	g_NetIF->SendPacketMessage(pk);
}

//----------------------------
// GM-    
//----------------------------
void CS_GM1Say1(const char* pszContent, DWORD color) {
	auto pk = net::msg::serializeClient(net::msg::CpGm1Say1Message{pszContent, static_cast<int64_t>(color)});
	g_NetIF->SendPacketMessage(pk);
}

//----------------------------
//  
//----------------------------
void CS_Say2Trade(const char* pszContent) {
	auto pk = net::msg::serializeClient(net::msg::CpSay2TradeMessage{pszContent});
	g_NetIF->SendPacketMessage(pk);
}

//----------------------------
//  
//----------------------------
void CS_Say2All(const char* pszContent) {
	auto pk = net::msg::serializeClient(net::msg::CpSay2AllMessage{pszContent});
	g_NetIF->SendPacketMessage(pk);
}

//----------------------------
//      
//----------------------------
void CP_RefuseToMe(bool refusetome) {
	auto pk = net::msg::serializeClient(net::msg::CpRefuseToMeMessage{refusetome ? 1 : 0});
	g_NetIF->SendPacketMessage(pk);
}

//----------------------------
//    
//----------------------------
void CS_Say2You(const char* you, const char* pszContent) {
	auto pk = net::msg::serializeClient(net::msg::CpSay2YouMessage{you, pszContent});
	g_NetIF->SendPacketMessage(pk);
}

//----------------------------
//  
//----------------------------
void CS_Say2Team(const char* pszContent) {
	auto pk = net::msg::serializeClient(net::msg::CpSay2TemMessage{pszContent});
	g_NetIF->SendPacketMessage(pk);
}

//----------------------------
//  
//----------------------------
void CS_Say2Guild(const char* pszContent) {
	auto pk = net::msg::serializeClient(net::msg::CpSay2GudMessage{pszContent});
	g_NetIF->SendPacketMessage(pk);
}

//----------------------------
//   
//----------------------------
void CS_Team_Invite(const char* chaname) {
	if (!g_stUIStart.IsCanTeamAndInfo()) return;

	auto pk = net::msg::serializeClient(net::msg::CpTeamInviteMessage{chaname});
	g_NetIF->SendPacketMessage(pk);
}

//----------------------------
//    
//----------------------------
void CS_Team_Refuse(unsigned long chaid) {
	auto pk = net::msg::serializeClient(net::msg::CpTeamRefuseMessage{static_cast<int64_t>(chaid)});
	g_NetIF->SendPacketMessage(pk);
}

//----------------------------
//    
//----------------------------
void CS_Team_Confirm(unsigned long chaid) {
	if (!g_stUIStart.IsCanTeamAndInfo()) return;

	auto pk = net::msg::serializeClient(net::msg::CpTeamAcceptMessage{static_cast<int64_t>(chaid)});
	g_NetIF->SendPacketMessage(pk);
}

//----------------------------
//   
//----------------------------
void CS_Team_Kick(DWORD dwKickedID) {
	if (!g_stUIStart.IsCanTeamAndInfo()) return;

	auto pk = net::msg::serializeClient(net::msg::CpTeamKickMessage{static_cast<int64_t>(dwKickedID)});
	g_NetIF->SendPacketMessage(pk);
}

//----------------------------
//  
//----------------------------
void CS_Team_Leave() {
	if (!g_stUIStart.IsCanTeamAndInfo()) return;

	auto pk = net::msg::serializeClient(net::msg::CpTeamLeaveMessage{});
	g_NetIF->SendPacketMessage(pk);
}

//----------------------------
//   
//----------------------------
void CS_Frnd_Invite(const char* chaname) {
	auto pk = net::msg::serializeClient(net::msg::CpFrndInviteMessage{chaname});
	g_NetIF->SendPacketMessage(pk);
}

//----------------------------
//    
//----------------------------
void CS_Frnd_Refuse(unsigned long chaid) {
	auto pk = net::msg::serializeClient(net::msg::CpFrndRefuseMessage{static_cast<int64_t>(chaid)});
	g_NetIF->SendPacketMessage(pk);
}

//----------------------------
//    
//----------------------------
void CS_Frnd_Confirm(unsigned long chaid) {
	auto pk = net::msg::serializeClient(net::msg::CpFrndAcceptMessage{static_cast<int64_t>(chaid)});
	g_NetIF->SendPacketMessage(pk);
}

//----------------------------
//   
//----------------------------
void CS_Frnd_Delete(unsigned long chaid) {
	auto pk = net::msg::serializeClient(net::msg::CpFrndDeleteMessage{static_cast<int64_t>(chaid)});
	g_NetIF->SendPacketMessage(pk);
}

//----------------------------
//     
//----------------------------
void CP_Frnd_Refresh_Info(unsigned long chaid) {
	auto pk = net::msg::serializeClient(net::msg::CpFrndRefreshInfoMessage{static_cast<int64_t>(chaid)});
	g_NetIF->SendPacketMessage(pk);
}

//----------------------------
//    (, ,   )
//----------------------------
void CP_Change_PersonInfo(const char* motto, unsigned short icon, bool refuse_sess) {
	auto pk = net::msg::serializeClient(net::msg::CpChangePersonInfoMessage{
		motto, static_cast<int64_t>(icon), static_cast<int64_t>(refuse_sess)
	});
	g_NetIF->SendPacketMessage(pk);
}

//----------------------------
//  -
//----------------------------
void CS_Sess_Create(const char* chaname[], unsigned char chanum) {
	std::vector<std::string> names;
	names.reserve(chanum);
	for (unsigned char i = 0; i < chanum; i++) {
		names.emplace_back(chaname[i]);
	}
	auto pk = net::msg::serializeClient(net::msg::CpSessCreateMessage{
		static_cast<int64_t>(chanum), std::move(names)
	});
	g_NetIF->SendPacketMessage(pk);
}

//----------------------------
//    
//----------------------------
void CS_Sess_Add(unsigned long sessid, const char* chaname) {
	auto pk = net::msg::serializeClient(net::msg::CpSessAddMessage{
		static_cast<int64_t>(sessid), chaname
	});
	g_NetIF->SendPacketMessage(pk);
}

//----------------------------
//  
//----------------------------
void CS_Sess_Leave(unsigned long sessid) {
	auto pk = net::msg::serializeClient(net::msg::CpSessLeaveMessage{
		static_cast<int64_t>(sessid)
	});
	g_NetIF->SendPacketMessage(pk);
}

//----------------------------
//   
//----------------------------
void CS_Sess_Say(unsigned long sessid, const char* word) {
	auto pk = net::msg::serializeClient(net::msg::CpSessSayMessage{
		static_cast<int64_t>(sessid), word
	});
	g_NetIF->SendPacketMessage(pk);
}

// =================================================================
//   PC- (GroupServer -> )
// =================================================================

//----------------------------
//  
//----------------------------
BOOL PC_Say2You(LPRPACKET pk) {
	net::msg::PcSay2YouMessage msg;
	net::msg::deserialize(pk, msg);

	stNetSay2You l_say;
	l_say.m_src = msg.sender;
	l_say.m_dst = msg.target;
	l_say.m_content = msg.content;
	NetSay2You(l_say, static_cast<DWORD>(msg.color));
	return TRUE;
}

//----------------------------
//   ()
//----------------------------
BOOL PC_Say2Team(LPRPACKET pk) {
	net::msg::PcSay2TemMessage msg;
	net::msg::deserialize(pk, msg);

	NetSay2Team(static_cast<unsigned long>(msg.chaId), msg.content.c_str(),
				static_cast<DWORD>(msg.color));
	return TRUE;
}

//----------------------------
//   ()
//----------------------------
BOOL PC_Say2Gud(LPRPACKET pk) {
	net::msg::PcSay2GudMessage msg;
	net::msg::deserialize(pk, msg);

	NetSay2Gud(msg.chaName.c_str(), msg.content.c_str(),
			   static_cast<DWORD>(msg.color));
	return TRUE;
}

//----------------------------
//   ()
//----------------------------
BOOL PC_Say2All(LPRPACKET pk) {
	net::msg::PcSay2AllMessage msg;
	net::msg::deserialize(pk, msg);

	stNetSay2All l_say;
	l_say.m_src = msg.chaName;
	l_say.m_content = msg.content;
	NetSay2All(l_say, static_cast<DWORD>(msg.color));
	return TRUE;
}

//----------------------------
// GM- ()
//----------------------------
BOOL PC_GM1SAY(LPRPACKET pk) {
	net::msg::PcGm1SayMessage msg;
	net::msg::deserialize(pk, msg);

	stNetSay2All l_say;
	l_say.m_src = msg.chaName;
	l_say.m_content = msg.content;
	NetGM1Say(l_say);
	return TRUE;
}

//----------------------------
// GM-   ()
//----------------------------
BOOL PC_GM1SAY1(LPRPACKET pk) {
	net::msg::PcGm1Say1Message msg;
	net::msg::deserialize(pk, msg);

	stNetScrollSay l_say;
	l_say.m_content = msg.content;
	l_say.setnum = static_cast<int>(msg.setNum);
	l_say.color = static_cast<DWORD>(msg.color);
	NetGM1Say1(l_say);
	return TRUE;
}

//----------------------------
//   ()
//----------------------------
BOOL PC_SAY2TRADE(LPRPACKET pk) {
	net::msg::PcSay2TradeMessage msg;
	net::msg::deserialize(pk, msg);

	stNetSay2All l_say;
	l_say.m_src = msg.chaName;
	l_say.m_content = msg.content;
	NetSay2Trade(l_say, static_cast<DWORD>(msg.color));
	return TRUE;
}

//----------------------------
//   ( / )
//----------------------------
BOOL PC_SESS_CREATE(LPRPACKET pk) {
	net::msg::PcSessCreateMessage msg;
	net::msg::deserialize(pk, msg);

	if (!msg.sessId) {
		NetSessCreate(msg.errorMsg.c_str());
	}
	else {
		uShort l_chanum = static_cast<uShort>(msg.members.size());
		if (!l_chanum || l_chanum > 100) return FALSE;

		stNetSessCreate l_nsc[100];
		for (uShort i = 0; i < l_chanum; i++) {
			l_nsc[i].lChaID = static_cast<uLong>(msg.members[i].chaId);
			l_nsc[i].szChaName = msg.members[i].chaName;
			l_nsc[i].szMotto = msg.members[i].motto;
			l_nsc[i].sIconID = static_cast<uShort>(msg.members[i].icon);
		}
		NetSessCreate(static_cast<uLong>(msg.sessId), l_nsc, l_chanum);
	}
	return TRUE;
}

//----------------------------
//    
//----------------------------
BOOL PC_SESS_ADD(LPRPACKET pk) {
	net::msg::PcSessAddMessage msg;
	net::msg::deserialize(pk, msg);

	stNetSessCreate l_nsc;
	l_nsc.lChaID = static_cast<unsigned long>(msg.chaId);
	l_nsc.szChaName = msg.chaName;
	l_nsc.szMotto = msg.motto;
	l_nsc.sIconID = static_cast<unsigned short>(msg.icon);
	NetSessAdd(static_cast<unsigned long>(msg.sessId), &l_nsc);
	return TRUE;
}

//----------------------------
//   
//----------------------------
BOOL PC_SESS_LEAVE(LPRPACKET pk) {
	net::msg::PcSessLeaveMessage msg;
	net::msg::deserialize(pk, msg);

	NetSessLeave(static_cast<unsigned long>(msg.sessId),
				 static_cast<unsigned long>(msg.chaId));
	return TRUE;
}

//----------------------------
//   
//----------------------------
BOOL PC_SESS_SAY(LPRPACKET pk) {
	net::msg::PcSessSayMessage msg;
	net::msg::deserialize(pk, msg);

	NetSessSay(static_cast<unsigned long>(msg.sessId),
			   static_cast<unsigned long>(msg.chaId), msg.content.c_str());
	return TRUE;
}

//----------------------------
//    ()
//----------------------------
BOOL PC_TEAM_INVITE(LPRPACKET pk) {
	net::msg::PcTeamInviteMessage msg;
	net::msg::deserialize(pk, msg);

	NetTeamInvite(msg.inviterName.c_str(),
				  static_cast<unsigned long>(msg.chaId),
				  static_cast<unsigned short>(msg.icon));
	return TRUE;
}

//----------------------------
// /   
//----------------------------
BOOL PC_TEAM_CANCEL(LPRPACKET pk) {
	net::msg::PcTeamCancelMessage msg;
	net::msg::deserialize(pk, msg);

	NetTeamCancel(static_cast<unsigned long>(msg.chaId),
				  static_cast<char>(msg.reason));
	return TRUE;
}

//----------------------------
//   
//----------------------------
BOOL PC_TEAM_REFRESH(LPRPACKET pk) {
	net::msg::PcTeamRefreshMessage msg;
	net::msg::deserialize(pk, msg);

	stNetPCTeam l_pcteam;
	l_pcteam.kind = static_cast<unsigned char>(msg.msg);
	l_pcteam.count = static_cast<unsigned char>(msg.count);

	ToLogService("players", "Kind:[{}], Count[{}]", l_pcteam.kind, l_pcteam.count);

	for (unsigned char i = 0; i < l_pcteam.count && i < 10; i++) {
		l_pcteam.cha_dbid[i] = static_cast<unsigned long>(msg.members[i].chaId);
		strncpy(l_pcteam.cha_name[i], msg.members[i].chaName.c_str(),
				sizeof(l_pcteam.cha_name[i]) - 1);
		l_pcteam.cha_name[i][sizeof(l_pcteam.cha_name[i]) - 1] = '\0';
		strncpy(l_pcteam.motto[i], msg.members[i].motto.c_str(),
				sizeof(l_pcteam.motto[i]) - 1);
		l_pcteam.motto[i][sizeof(l_pcteam.motto[i]) - 1] = '\0';
		l_pcteam.cha_icon[i] = static_cast<short>(msg.members[i].icon);

		ToLogService("players", "    DB_ID:[{}], Name[{}]",
					 l_pcteam.cha_dbid[i], l_pcteam.cha_name[i]);
	}

	NetPCTeam(l_pcteam);
	return TRUE;
}

//----------------------------
//    ()
//----------------------------
BOOL PC_FRND_INVITE(LPRPACKET pk) {
	net::msg::PcFrndInviteMessage msg;
	net::msg::deserialize(pk, msg);

	NetFrndInvite(msg.inviterName.c_str(),
				  static_cast<unsigned long>(msg.chaId),
				  static_cast<unsigned short>(msg.icon));
	return TRUE;
}

//----------------------------
// /   
//----------------------------
BOOL PC_FRND_CANCEL(LPRPACKET pk) {
	net::msg::PcFrndCancelMessage msg;
	net::msg::deserialize(pk, msg);

	NetFrndCancel(static_cast<unsigned long>(msg.chaId),
				  static_cast<char>(msg.reason));
	return TRUE;
}

//----------------------------
// GM- (   )
//----------------------------
BOOL PC_GM_INFO(LPRPACKET pk) {
	net::msg::PcGmInfoMessage msg;
	net::msg::deserialize(pk, msg);

	switch (msg.type) {
	case MSG_FRND_REFRESH_START: {
		auto& startData = std::get<net::msg::GmInfoStartData>(msg.data);
		uShort l_count = static_cast<uShort>(startData.entries.size());
		stNetFrndStart l_nfs[100];
		for (uShort i = 0; i < l_count && i < 100; i++) {
			l_nfs[i].szGroup = "GM";
			l_nfs[i].lChaid = static_cast<uLong>(startData.entries[i].chaId);
			l_nfs[i].szChaname = startData.entries[i].chaName;
			l_nfs[i].szMotto = startData.entries[i].motto;
			l_nfs[i].sIconID = static_cast<uShort>(startData.entries[i].icon);
			l_nfs[i].cStatus = static_cast<unsigned char>(startData.entries[i].status);
		}
		NetGMStart(l_nfs, l_count);
		break;
	}
	case MSG_FRND_REFRESH_OFFLINE:
		NetGMOffline(static_cast<uLong>(std::get<net::msg::GmInfoChaIdData>(msg.data).chaId));
		break;
	case MSG_FRND_REFRESH_ONLINE:
		NetGMOnline(static_cast<uLong>(std::get<net::msg::GmInfoChaIdData>(msg.data).chaId));
		break;
	case MSG_FRND_REFRESH_DEL:
		NetGMDel(static_cast<uLong>(std::get<net::msg::GmInfoChaIdData>(msg.data).chaId));
		break;
	case MSG_FRND_REFRESH_ADD: {
		auto& addEntry = std::get<net::msg::GmFrndAddEntry>(msg.data);
		NetGMAdd(
			static_cast<uLong>(addEntry.chaId),
			addEntry.chaName.c_str(),
			addEntry.motto.c_str(),
			static_cast<uShort>(addEntry.icon),
			addEntry.group.c_str());
		break;
	}
	}
	return TRUE;
}

//----------------------------
//    (   )
//----------------------------
BOOL PC_FRND_REFRESH(LPRPACKET pk) {
	net::msg::PcFrndRefreshFullMessage msg;
	net::msg::deserialize(pk, msg);

	switch (msg.type) {
	case MSG_FRND_REFRESH_ONLINE:
		NetFrndOnline(static_cast<uLong>(std::get<net::msg::FrndRefreshChaIdData>(msg.data).chaId));
		break;
	case MSG_FRND_REFRESH_OFFLINE:
		NetFrndOffline(static_cast<uLong>(std::get<net::msg::FrndRefreshChaIdData>(msg.data).chaId));
		break;
	case MSG_FRND_REFRESH_DEL:
		NetFrndDel(static_cast<uLong>(std::get<net::msg::FrndRefreshChaIdData>(msg.data).chaId));
		break;
	case MSG_FRND_REFRESH_ADD: {
		auto& addEntry = std::get<net::msg::GmFrndAddEntry>(msg.data);
		NetFrndAdd(
			static_cast<uLong>(addEntry.chaId),
			addEntry.chaName.c_str(),
			addEntry.motto.c_str(),
			static_cast<uShort>(addEntry.icon),
			addEntry.group.c_str());
		break;
	}
	case MSG_FRND_REFRESH_START: {
		auto& startData = std::get<net::msg::FrndRefreshStartData>(msg.data);
		stNetFrndStart l_self;
		l_self.lChaid = static_cast<uLong>(startData.self.chaId);
		l_self.szChaname = startData.self.chaName;
		l_self.szMotto = startData.self.motto;
		l_self.sIconID = static_cast<uShort>(startData.self.icon);

		stNetFrndStart l_nfs[100];
		uShort l_nfnum = 0;

		for (const auto& grp : startData.groups) {
			for (const auto& m : grp.members) {
				if (l_nfnum >= 100) break;
				l_nfs[l_nfnum].szGroup = grp.groupName;
				l_nfs[l_nfnum].lChaid = static_cast<uLong>(m.chaId);
				l_nfs[l_nfnum].szChaname = m.chaName;
				l_nfs[l_nfnum].szMotto = m.motto;
				l_nfs[l_nfnum].sIconID = static_cast<uShort>(m.icon);
				l_nfs[l_nfnum].cStatus = static_cast<unsigned char>(m.status);
				l_nfnum++;
			}
			if (l_nfnum >= 100) break;
		}

		NetFrndStart(l_self, l_nfs, l_nfnum);
	}
	break;
	}
	return TRUE;
}

//----------------------------
//    
//----------------------------
BOOL PC_FRND_REFRESH_INFO(LPRPACKET pk) {
	net::msg::PcFrndRefreshInfoMessage msg;
	net::msg::deserialize(pk, msg);

	unsigned short l_degr = static_cast<unsigned short>(msg.degree);
	if (l_degr == 0)
		l_degr = 1;

	NetFrndRefreshInfo(
		static_cast<unsigned long>(msg.chaId),
		msg.motto.c_str(),
		static_cast<unsigned short>(msg.icon),
		l_degr,
		msg.job.c_str(),
		msg.guildName.c_str());

	return TRUE;
}

//----------------------------
//    (, ,   )
//----------------------------
BOOL PC_CHANGE_PERSONINFO(LPRPACKET pk) {
	net::msg::PcChangePersonInfoMessage msg;
	net::msg::deserialize(pk, msg);

	NetChangePersonInfo(msg.motto.c_str(),
						static_cast<unsigned short>(msg.icon),
						msg.refuseSess ? true : false);
	return TRUE;
}
