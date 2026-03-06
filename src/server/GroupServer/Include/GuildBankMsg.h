#pragma once

#include "Player.h"
#include <deque>
struct GuildBankMsg{
	Player* player;
	WPacket msg;
};

std::deque<GuildBankMsg> guildBankMsgQueue[201];
