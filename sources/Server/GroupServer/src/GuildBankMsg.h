#include "Player.h"
#include <deque>
struct GuildBankMsg{
	Player* player;
	net::WPacket msg;
};

std::deque<GuildBankMsg> guildBankMsgQueue[201];
