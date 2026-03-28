#include "stdafx.h"
#include "netguild.h"
#include "netcommand.h"
#include "uiguildmgr.h"
#include "uiglobalvar.h"
#include "CommandMessages.h"

using namespace std;

BOOL MC_GUILD_GETNAME(LPRPACKET pk) {
	NetMC_GUILD_GETNAME();
	return TRUE;
}

void CM_GUILD_PUTNAME(bool confirm, cChar* guildname, cChar* passwd) {
	auto pk = net::msg::serialize(net::msg::CmGuildPutNameMessage{confirm ? 1 : 0, guildname, passwd});
	g_NetIF->SendPacketMessage(pk);
}

bool g_listguild_begin = false;

BOOL MC_LISTGUILD(LPRPACKET pk) {
	// Десериализация через McListGuildMessage (count-first)
	net::msg::McListGuildMessage msg;
	net::msg::deserialize(pk, msg);
	if (!g_listguild_begin) {
		g_listguild_begin = true;
		NetMC_LISTGUILD_BEGIN();
	}
	for (const auto& e : msg.entries) {
		NetMC_LISTGUILD(static_cast<uLong>(e.guildId), e.guildName.c_str(), e.motto.c_str(),
						e.leaderName.c_str(), static_cast<uShort>(e.memberCount), e.exp);
	}
	if (msg.entries.size() < 20) {
		NetMC_LISTGUILD_END();
		g_listguild_begin = false;
	}
	return TRUE;
}

void CM_GUILD_TRYFOR(uLong guildid) // Подать заявку в гильдию
{
	auto pk = net::msg::serialize(net::msg::CmGuildTryForMessage{(int64_t)guildid});
	g_NetIF->SendPacketMessage(pk);
}

BOOL MC_GUILD_TRYFORCFM(LPRPACKET pk) {
	net::msg::McGuildTryForCfmMessage msg;
	net::msg::deserialize(pk, msg);
	NetMC_GUILD_TRYFORCFM(msg.name.c_str());
	return TRUE;
}

void CM_GUILD_TRYFORCFM(bool confirm) // Подтвердить заявку в гильдию
{
	auto pk = net::msg::serialize(net::msg::CmGuildTryForCfmMessage{confirm ? 1 : 0});
	g_NetIF->SendPacketMessage(pk);
}

void CM_GUILD_LISTTRYPLAYER() // Запросить список заявок
{
	auto pk = net::msg::serializeCmGuildListTryPlayerCmd();
	g_NetIF->SendPacketMessage(pk);
}

BOOL MC_GUILD_LISTTRYPLAYER(LPRPACKET pk) {
	// Десериализация через McGuildListTryPlayerMessage (count-first)
	net::msg::McGuildListTryPlayerMessage msg;
	net::msg::deserialize(pk, msg);
	uChar l_stat = CGuildData::eState::normal;
	NetMC_LISTTRYPLAYER_BEGIN(static_cast<uLong>(msg.guildId), msg.guildName.c_str(), msg.motto.c_str(),
							  l_stat, msg.leaderName.c_str(), static_cast<uShort>(msg.memberTotal),
							  static_cast<uShort>(msg.maxMembers), msg.exp, static_cast<uLong>(msg.reserved));

	for (const auto& p : msg.players) {
		NetMC_LISTTRYPLAYER(static_cast<uLong>(p.chaId), p.name.c_str(), p.job.c_str(), static_cast<uShort>(p.degree));
	}
	NetMC_LISTTRYPLAYER_END();
	return TRUE;
}

struct stGuildInfo {
	bool bOnline;
	uLong nChaID;
	char szChaName[65];
	char szMotto[257];
	char szJob[33];
	uShort sDegree;
	uShort sIcon;
	uLong sPreMission;
};


//bool g_guild_start_begin	=false;
vector<stGuildInfo> g_vecGuildInfo;

BOOL PC_GUILD_PERM(LPRPACKET pk) {
	net::msg::PcGuildPermMessage msg;
	net::msg::deserialize(pk, msg);
	NetPC_GUILD_UPDATEPERM(msg.targetChaId, msg.permission);
	return true;
}

BOOL PC_GUILD(LPRPACKET pk) {
	// Десериализация через PcGuildMessage (std::variant)
	net::msg::PcGuildMessage msg;
	net::msg::deserialize(pk, msg);

	switch (msg.msg) {
	case MSG_GUILD_ONLINE: {
		NetPC_GUILD_ONLINE(std::get<net::msg::GuildChaIdData>(msg.data).chaId);
	}
	break;
	case MSG_GUILD_OFFLINE: {
		NetPC_GUILD_OFFLINE(std::get<net::msg::GuildChaIdData>(msg.data).chaId);
	}
	break;
	case MSG_GUILD_START: {
		auto& startData = std::get<net::msg::GuildStartData>(msg.data);
		uChar l_num = static_cast<uChar>(startData.members.size());
		uLong lPacketIndex = static_cast<uLong>(startData.packetIndex);
		static int nGuildCount = 0;

		if (lPacketIndex == 0 && l_num > 0) // первый пакет
		{
			NetPC_GUILD_START_BEGIN(static_cast<uLong>(startData.guildId),
								   startData.guildName.c_str(),
								   static_cast<uLong>(startData.leaderId));
		}
		for (uChar i = 0; i < l_num; ++i) {
			const auto& m = startData.members[i];

			stGuildInfo info;
			memset(&info, 0, sizeof(stGuildInfo));
			info.bOnline = m.online ? true : false;
			info.nChaID = static_cast<uLong>(m.chaId);
			info.sDegree = static_cast<uShort>(m.degree);
			info.sIcon = static_cast<uShort>(m.icon);
			info.sPreMission = static_cast<uLong>(m.permission);
			strncpy(info.szChaName, m.chaName.c_str(), sizeof(info.szChaName) - 1);
			strncpy(info.szJob, m.job.c_str(), sizeof(info.szJob) - 1);
			strncpy(info.szMotto, m.motto.c_str(), sizeof(info.szMotto) - 1);

			g_vecGuildInfo.push_back(info);
		}
		if (l_num < 20) // последний пакет данных
		{
			nGuildCount = 20 * lPacketIndex + l_num;
		}

		if (nGuildCount == (int)g_vecGuildInfo.size()) {
			for (int i = 0; i < nGuildCount; ++i) {
				NetPC_GUILD_START(g_vecGuildInfo[i].bOnline,
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
	case MSG_GUILD_STOP: {
		NetPC_GUILD_STOP();
	}
	break;
	case MSG_GUILD_ADD: {
		const auto& m = std::get<net::msg::GuildAddData>(msg.data).member;
		NetPC_GUILD_ADD(m.online ? true : false,
						static_cast<uLong>(m.chaId),
						m.chaName.c_str(), m.motto.c_str(), m.job.c_str(),
						static_cast<uShort>(m.degree), static_cast<uShort>(m.icon),
						static_cast<uLong>(m.permission));
	}
	break;
	case MSG_GUILD_DEL: {
		NetPC_GUILD_DEL(std::get<net::msg::GuildChaIdData>(msg.data).chaId);
	}
	break;
	default:
		break;
	}
	return TRUE;
}

void CM_GUILD_APPROVE(uLong chaid) {
	auto pk = net::msg::serialize(net::msg::CmGuildApproveMessage{(int64_t)chaid});
	g_NetIF->SendPacketMessage(pk);
}

void CM_GUILD_REJECT(uLong chaid) {
	auto pk = net::msg::serialize(net::msg::CmGuildRejectMessage{(int64_t)chaid});
	g_NetIF->SendPacketMessage(pk);
}

void CM_GUILD_KICK(uLong chaid) {
	auto pk = net::msg::serialize(net::msg::CmGuildKickMessage{(int64_t)chaid});
	g_NetIF->SendPacketMessage(pk);
}

void CM_GUILD_LEAVE() {
	auto pk = net::msg::serializeCmGuildLeaveCmd();
	g_NetIF->SendPacketMessage(pk);
}

void CM_GUILD_DISBAND(cChar* passwd) {
	auto pk = net::msg::serialize(net::msg::CmGuildDisbandMessage{passwd});
	g_NetIF->SendPacketMessage(pk);
}

void CM_GUILD_MOTTO(cChar* motto) {
	auto pk = net::msg::serialize(net::msg::CmGuildMottoMessage{motto});
	g_NetIF->SendPacketMessage(pk);
}

void CM_GUILD_CHALL(BYTE byLevel, DWORD dwMoney) {
	auto pk = net::msg::serialize(net::msg::CmGuildChallMessage{(int64_t)byLevel, (int64_t)dwMoney});
	g_NetIF->SendPacketMessage(pk);
}

void CM_GUILD_LEIZHU(BYTE byLevel, DWORD dwMoney) {
	auto pk = net::msg::serialize(net::msg::CmGuildLeizhuMessage{(int64_t)byLevel, (int64_t)dwMoney});
	g_NetIF->SendPacketMessage(pk);
}

BOOL MC_GUILD_MOTTO(LPRPACKET pk) {
	net::msg::McGuildMottoMessage msg;
	net::msg::deserialize(pk, msg);
	NetMC_GUILD_MOTTO(msg.motto.c_str());
	return TRUE;
}

BOOL MC_GUILD_LEAVE(LPRPACKET pk) {
	return TRUE;
}

BOOL MC_GUILD_KICK(LPRPACKET pk) {
	return TRUE;
}

BOOL MC_GUILD_INFO(LPRPACKET pk) {
	net::msg::McGuildInfoMessage msg;
	net::msg::deserialize(pk, msg);
	NetMC_GUILD_INFO(msg.charId, msg.guildId, msg.guildName.c_str(), msg.guildMotto.c_str(), msg.guildPermission);
	return TRUE;
}

BOOL MC_GUILD_LISTCHALL(LPRPACKET pk) {
	net::msg::McGuildListChallMessage msg;
	net::msg::deserialize(pk, msg);

	// Конвертируем в legacy-структуру для UI
	NET_GUILD_CHALLINFO Info;
	memset(&Info, 0, sizeof(Info));
	Info.byIsLeader = (BYTE)msg.isLeader;
	for (int i = 0; i < MAX_GUILD_CHALLLEVEL; i++) {
		Info.byLevel[i] = (BYTE)msg.entries[i].level;
		if (Info.byLevel[i]) {
			Info.byStart[i] = (BYTE)msg.entries[i].start;
			strncpy(Info.szGuild[i], msg.entries[i].guildName.c_str(), 64 - 1);
			strncpy(Info.szChall[i], msg.entries[i].challName.c_str(), 64 - 1);
			Info.dwMoney[i] = (DWORD)msg.entries[i].money;
		}
	}

	NetMC_GUILD_CHALLINFO(Info);
	return TRUE;
}
