
#ifndef _GAMEAPPNET_H_
#define _GAMEAPPNET_H_

// 

#include "stdafx.h" //add by alfred.shi 20080313

#include "NetRetCode.h"

#include "point.h"
#include "gtplayer.h"

#define NETMSG_INVALID_MSG      0
#define NETMSG_GATE_CONNECTED   100
#define NETMSG_GATE_DISCONNECT  101
#define NETMSG_PACKET           200

//           
#define EXCEPTION()	ToLogService("errors", LogLevel::Error, "Exception: File {}, Line {}", __FILE__, __LINE__)

inline const char* g_GameGateConnError( int error_code )
{
    switch( error_code )
    {
    /*case ERR_TM_OVERNAME:	return "GameServer";
    case ERR_TM_OVERMAP:	return "GameServer";
    case ERR_TM_MAPERR:		return "GameServer";
    default:                return "";*/
	case ERR_TM_OVERNAME:	return RES_STRING(GM_GAMEAPPNET_H_00001);
    case ERR_TM_OVERMAP:	return RES_STRING(GM_GAMEAPPNET_H_00002);
    case ERR_TM_MAPERR:		return RES_STRING(GM_GAMEAPPNET_H_00003);
    default:                return RES_STRING(GM_GAMEAPPNET_H_00004);
    }
}



//

#include "GameServerApp.h"

#define GETGMSVRNAME() g_gmsvr->GetName()
#define ADDPLAYER(ply, pGate, gtaddr) g_gmsvr->AddPlayer(ply, pGate, gtaddr)
#define DELPLAYER(ply) g_gmsvr->DelPlayer(ply)
#define KICKPLAYER(ply, time) g_gmsvr->KickPlayer(ply, time)
#define PEEKPACKET(ms) g_gmsvr->PeekPacket(ms)
#define DISCONNECT(pGate) g_gmsvr->DisconnectGate(pGate)
#define ISVALIDGATE(i) g_gmsvr->IsValidGate(i)
#define GETPLAYERCOUNT(pGate) pGate->GetPlayerCount()

#define SENDTOWORLD(pkt) g_gmsvr->SendToWorld(pkt)
#define SENDTOGROUP(pkt) g_gmsvr->SendToGroup(pkt);
#define SENDTOCLIENT(pkt, l) g_gmsvr->SendToClient(pkt, l)
#define SENDTOCLIENT2(pkt, n, a) g_gmsvr->SendToClient(pkt, n, a)
#define SENDTOSINGLE(pkt, l) g_gmsvr->SendToClient(l, pkt)
#define SENDTOGAME(pkt, p) g_gmsvr->SendToGame(pkt, p)

#define BEGINGETGATE() g_gmsvr->BeginGetGate()
#define GETNEXTGATE() g_gmsvr->GetNextGate()





#endif // _GAMEAPPNET_H_
