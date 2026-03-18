
#ifndef _GAMEAPPNET_H_
#define _GAMEAPPNET_H_

// ͨ������

#include "stdafx.h" //add by alfred.shi 20080313

#include "NetRetCode.h"
#include "PreAlloc.h"
#include "point.h"
#include "gtplayer.h"

#define NETMSG_INVALID_MSG      0
#define NETMSG_GATE_CONNECTED   100
#define NETMSG_GATE_DISCONNECT  101
#define NETMSG_PACKET           200

#define EXCEPTION()	LG("EXCEPTION", "Exception��File %s��Line %d��\n", __FILE__, __LINE__)

inline const char* g_GameGateConnError( int error_code )
{
    switch( error_code )
    {
    /*case ERR_TM_OVERNAME:	return "GameServer���ظ�";
    case ERR_TM_OVERMAP:	return "GameServer�ϵ�ͼ���ظ�";
    case ERR_TM_MAPERR:		return "GameServer��ͼ�����﷨����";
    default:                return "δ֪����";*/
	case ERR_TM_OVERNAME:	return RES_STRING(GM_GAMEAPPNET_H_00001);
    case ERR_TM_OVERMAP:	return RES_STRING(GM_GAMEAPPNET_H_00002);
    case ERR_TM_MAPERR:		return RES_STRING(GM_GAMEAPPNET_H_00003);
    default:                return RES_STRING(GM_GAMEAPPNET_H_00004);
    }
}



// ��ͬ����ײ����ݽṹ�����ͳ���

#ifdef USE_IOCP

#define USE_NBO
#include "cfl_pkt.h"
#include "gmsvr.h"

#define GETGMSVRNAME() g_mygmsvr->getname()
#define ADDPLAYER(ply, pGate, gtaddr) g_mygmsvr->addplayer(ply, pGate, gtaddr)
#define DELPLAYER(ply) g_mygmsvr->delplayer(ply)
#define KICKPLAYER(ply, time) g_mygmsvr->kickplayer(ply, time)
#define PEEKPACKET(ms) g_mygmsvr->peekpkt(ms)
#define DISCONNECT(sk) g_mygmsvr->disconnect(sk)
#define ISVALIDGATE(i) g_mygmsvr->isvalidgt(i)
#define BEGINGETPLAYER(pGate) g_mygmsvr->begingetplayer(pGate)
#define GETNEXTPLAYER(pGate) g_mygmsvr->getnextplayer(pGate)

#define SENDTOWORLD(pkt) g_mygmsvr->sendtoworld(pkt)
#define SENDTOCLIENT(pkt, l) g_mygmsvr->sendtoclient(pkt, l)
#define SENDTOCLIENT2(pkt, n, a) g_mygmsvr->sendtoclient(pkt, n, a)
#define SENDTOGAME(pkt, p) g_gmsvr->sendtogame(pkt, p)

#define WPACKET Packet*
#define RPACKET Packet*
#define VALIDRPACKET(pkt) (pkt != NULL) ? true : false

#define GETWPACKET() g_mygmsvr->getpkt()
#define RETRPACKET(pkt) pkt->free()
#define WRITE_CMD(pkt, cmd) pkt->WriteCmd(cmd)
#define WRITE_CHAR(pkt, c) pkt->WriteInt64(c)
#define WRITE_SHORT(pkt, s) pkt->WriteInt64(s)
#define WRITE_LONG(pkt, l) pkt->WriteInt64(l)
#define WRITE_LONGLONG(pkt, l) pkt->WriteInt64(l)
#define WRITE_SEQ(pkt, seq, len) pkt->WriteSequence(seq, len)
#define WRITE_STRING(pkt, str) pkt->WriteString(str)
#define READ_CMD(pkt) pkt->ReadCmd()
#define READ_CHAR(pkt) pkt->ReadInt64()
#define READ_SHORT(pkt) pkt->ReadInt64()
#define READ_LONG(pkt) pkt->ReadInt64()
#define READ_LONGLONG(pkt) pkt->ReadInt64()
#define READ_SEQ(pkt, len) pkt->ReadSequence(len)
#define READ_STRING(pkt) pkt->ReadString()

#define READ_LONG_R(pkt) pkt->ReverseReadInt64()
#define READ_SHORT_R(pkt) pkt->ReverseReadInt64()

#define BEGINGETGATE() g_mygmsvr->begingetgate()
#define GETNEXTGATE() g_mygmsvr->getnextgate()


#else


#include "GameServerApp.h"

#define GETGMSVRNAME() g_gmsvr->GetName()
#define ADDPLAYER(ply, pGate, gtaddr) g_gmsvr->AddPlayer(ply, pGate, gtaddr)
#define DELPLAYER(ply) g_gmsvr->DelPlayer(ply)
#define KICKPLAYER(ply, time) g_gmsvr->KickPlayer(ply, time)
#define PEEKPACKET(ms) g_gmsvr->PeekPacket(ms)
#define DISCONNECT(pGate) g_gmsvr->DisconnectGate(pGate)
#define ISVALIDGATE(i) g_gmsvr->IsValidGate(i)
#define BEGINGETPLAYER(pGate) g_gmsvr->BeginGetplayer(pGate)
#define GETNEXTPLAYER(pGate) g_gmsvr->GetNextPlayer(pGate)
#define GETPLAYERCOUNT(pGate) pGate->GetPlayerCount()

#define SENDTOWORLD(pkt) g_gmsvr->SendToWorld(pkt)
#define SENDTOGROUP(pkt) g_gmsvr->SendToGroup(pkt);
#define SENDTOCLIENT(pkt, l) g_gmsvr->SendToClient(pkt, l)
#define SENDTOCLIENT2(pkt, n, a) g_gmsvr->SendToClient(pkt, n, a)
#define SENDTOSINGLE(pkt, l) g_gmsvr->SendToClient(l, pkt)
#define SENDTOGAME(pkt, p) g_gmsvr->SendToGame(pkt, p)

#define WPACKET net::WPacket
#define RPACKET net::RPacket&
#define VALIDRPACKET(pkt) (static_cast<bool>(pkt))

#define GETWPACKET() g_gmsvr->GetWPacket()
#define RETRPACKET(pkt)
#define WRITE_CMD(pkt, cmd) pkt.WriteCmd(cmd)
#define WRITE_CHAR(pkt, c) pkt.WriteInt64(c)
#define WRITE_SHORT(pkt, s) pkt.WriteInt64(s)
#define WRITE_LONG(pkt, l) pkt.WriteInt64(l)
#define WRITE_LONGLONG(pkt, l) pkt.WriteInt64(l)
#define WRITE_SEQ(pkt, seq, len) pkt.WriteSequence(seq, len)
#define WRITE_STRING(pkt, str) pkt.WriteString(str)
#define READ_CMD(pkt) pkt.ReadCmd()
#define READ_CHAR(pkt) pkt.ReadInt64()
#define READ_SHORT(pkt) pkt.ReadInt64()
#define READ_LONG(pkt) pkt.ReadInt64()
#define READ_LONGLONG(pkt) pkt.ReadInt64()
#define READ_SEQ(pkt, len) pkt.ReadSequence(len)
#define READ_STRING(pkt) pkt.ReadString()

#define READ_LONG_R(pkt) pkt.ReverseReadInt64()
#define READ_SHORT_R(pkt) pkt.ReverseReadInt64()

#define BEGINGETGATE() g_gmsvr->BeginGetGate()
#define GETNEXTGATE() g_gmsvr->GetNextGate()


#endif // USE_IOCP





#endif // _GAMEAPPNET_H_
