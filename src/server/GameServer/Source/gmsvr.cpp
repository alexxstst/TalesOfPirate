#include "stdafx.h"

#ifdef USE_IOCP

#include "gmsvr.h"
#include "gtplayer.h"
#include "gameapp.h"
#pragma warning(disable: 4311)

myiocpclt::myiocpclt()
    : cfl_iocpclt(), _mqlock(), rpktpool(1000), wpktpool(1000), msgpool(1000), gtnum(0)
    {
    pkthdr_size = Packet::hdr_sz();
    _msgqueue.clear();}

bool myiocpclt::init_gtconn(CGameConfig* gmcfg)
{
    gmsvr_name = gmcfg->m_szName;

    int num = min(MAX_GATE, gmcfg->m_nGateCnt);

    for (int i = 0; i < num; ++ i)
    {
        gtarray[i].GetIP() = gmcfg->m_szGateIP[i];
        gtarray[i].GetPort() = gmcfg->m_nGatePort[i];
        gtarray[i].Invalid();
    }

    gtnum = num;

    DWORD dwThreadId;
    CreateThread(NULL, 0, conn_thrd, this, 0, &dwThreadId);
    return true;
}

DWORD WINAPI myiocpclt::conn_thrd(LPVOID conn_thrd_ctx)
{
    myiocpclt* that = (myiocpclt *)conn_thrd_ctx;
    PER_SOCKET_CONTEXT* sk_ctx;
    int i;
    
    while (!that->get_exit_flag())
    {
        for (i = 0; i < that->gtnum; ++ i)
        {
            if (that->gtarray[i].IsValid()) {}
            else
            {
                sk_ctx = that->connect(that->gtarray[i].GetIP().c_str(), that->gtarray[i].GetPort());
                if (sk_ctx == NULL)
                {
                   // LG("iocp_conn", " Gate %s:%d \n", that->gtarray[i].GetIP().c_str(),
                     //   that->gtarray[i].GetPort());
					 ToLogService("iocp_conn", "connect Gate {}:{} failed", that->gtarray[i].GetIP().c_str(),
						 that->gtarray[i].GetPort());
                }
                else
                {
                    that->gtarray[i].SetDataSock(sk_ctx, that);
                    that->_postconnmsg(sk_ctx); // notify walker thread to update the socket
                }
            }
            
        }

        Sleep(1000);
    }

    ToLogService("iocp_conn", "conn_thrd thread exit");
    return 0;
}

void myiocpclt::postmsg(DWORD msg_type, GateServer* gt, Packet* pkt)
    {
    if (gt == NULL) return;

    mymsg* msg = msgpool.get();
    if (msg == NULL)
        {
        cfl_printf("message pool is empty! drop a packet\n");
        pkt->free();
        return;}

    msg->type = msg_type;
    msg->gt = gt;
    msg->pkt = pkt;

    _mqlock.lock();
    try {
        _msgqueue.push_back(msg);}
    catch (...)
        {
        cfl_printf("Exception raised from postmsg\n");}
    _mqlock.unlock();}

int myiocpclt::on_connect(PER_SOCKET_CONTEXT* sk_ctx)
    {
    GateServer* gt = (GateServer *)sk_ctx->getusrdat();
    if (gt == NULL)
        {
        cfl_printf("gtsvr pool is empty!\n");
        return 0;}

    // Clear the player list
    gt->GetPlayerList() = NULL;

    // Post the connect message to app
    postmsg(NETMSG_GATE_CONNECTED, gt, NULL);

    // Post the first asynchronized read quest on this socket
    _recv(sk_ctx);

    return 0;}

int myiocpclt::on_disconn(PER_SOCKET_CONTEXT* sk_ctx)
    {
    GateServer* gt = (GateServer *)sk_ctx->getusrdat();

    // Post the disconnect message to app
    Packet* pkt = (Packet *)rpktpool.get();
    if (pkt != NULL)
    {
        pkt->WriteLong((long)gt->GetPlayerList());
        pkt->ResetOffset();
    }
    postmsg(NETMSG_GATE_DISCONNECT, gt, pkt);

    // Reset the user data of socket
    sk_ctx->setusrdat(NULL);
    gt->Invalid();

    return 0;}

int myiocpclt::on_recvdat(PER_SOCKET_CONTEXT* sk_ctx, DWORD size)
    {
    //cfl_printf("Receive %dB from %s:%d\n", size, sk_ctx->remote_ip.c_str(), sk_ctx->remote_port);

    char* buf = sk_ctx->io_ctx->buffer;
    int& sent_bytes = sk_ctx->io_ctx->sent_bytes; // data last received
    short len = (short)size;
    short remain_len = 0;
    short pkt_size;
    Packet* pkt;

    len += sent_bytes;
    if (len == 0) return 0;

    do  {

        if (len < pkthdr_size) break;

        // receive a packet header
        pkt_size = Packet::pkt_len(buf);
        if (len < pkt_size)
            {
            remain_len = pkt_size - len;
            break;}

        //cfl_printf("packet size = %d\n", pkt_size);

        // receive a packet
        pkt = (Packet *)rpktpool.get();
        if (pkt != NULL)
            {
            memcpy(pkt->pkt_buf(), buf, pkt_size);

            // post it up
            postmsg(NETMSG_PACKET, (GateServer *)sk_ctx->getusrdat(), pkt);}

        // find whether there is another packet or not
        buf += pkt_size;
        len -= pkt_size;

        } while (true);

    if (len > 0)
        {
        //cfl_printf("Remain length = %d, enter recv state again!\n", remain_len);
        
        // need to move data to the start position
        //  will be optimized in the future
        char* buf2 = sk_ctx->io_ctx->buffer;
        char tmpbuf[MAX_BUFF_SIZE];
        memcpy(tmpbuf, buf, len);
        memcpy(buf2, tmpbuf, len);

        // receive the remain data
        sent_bytes = len;
        _recv(sk_ctx, len);}
    else {
        //cfl_printf("Enter recv state normally!\n");
        sent_bytes = 0;
        _recv(sk_ctx);}

    return 0;}

int myiocpclt::on_sendfsh(PER_SOCKET_CONTEXT* sk_ctx, DWORD size)
    {
    //cfl_printf("Send %dB to %s:%d\n", size, sk_ctx->remote_ip.c_str(), sk_ctx->remote_port);

    return 0;}

mymsg* myiocpclt::recvmsg()
    {
    mymsg* msg;

    _mqlock.lock();
    try {
        msg = (_msgqueue.empty()) ? NULL : _msgqueue.front();
        if (msg != NULL) _msgqueue.pop_front();}
    catch (...) {}
    _mqlock.unlock();

    return msg;}

void myiocpclt::peekpkt(int ms /* = 0 */)
    {
    mymsg* msg;
	int cnt = 0;

    while (true)
        {
        // peek a packet
        msg = recvmsg();
        if (msg == NULL) break;

        // dispatch packet
        ::g_pGameApp->ProcessNetMsg(msg->type, msg->gt, msg->pkt);
        if (msg->pkt != NULL) msg->pkt->free();
        msg->free();
		
		if (cnt >= ms)
			{
			Sleep(0);
			++ cnt;}
		}
    }

bool myiocpclt::addplayer(GatePlayer* gtplayer, GateServer* gt, unsigned long gtaddr)
{
    if (gt == NULL || gtplayer == NULL) return false;
    if (!gt->IsValid()) return false;

    //  GatePlayer 
    gtplayer->SetGate(gt);
    gtplayer->SetGateAddr(gtaddr);

    //  gtplayer 
    gtplayer->Prev = NULL;
    gtplayer->Next = gt->GetPlayerList();

    if (gtplayer->Next != NULL)
        gtplayer->Next->Prev = gtplayer;

    // 
    gt->GetPlayerList() = gtplayer;

    return true;
}

bool myiocpclt::delplayer(GatePlayer* gtplayer)
{
    if (gtplayer == NULL) return false;

    GateServer* gt = gtplayer->GetGate();
    if (gt == NULL || !gt->IsValid()) return false;

	if (gt->m_listcurplayer == gtplayer)
		gt->m_listcurplayer = gtplayer->Next;
    // 
    GatePlayer*& playerlist = gt->GetPlayerList();    
    if ((gtplayer->Prev == NULL) && (gtplayer->Next == NULL))
    {
        if (gtplayer == playerlist)
        { // 
            playerlist = NULL;
            return true;
        }
        else
        { // gtplayer
            return false;
        }
    }
    else if ((gtplayer->Prev == NULL) && (gtplayer->Next != NULL))
    { // 
        playerlist = gtplayer->Next;
        playerlist->Prev = NULL;

        gtplayer->Next = NULL;
        return true;
    }    
    else if ((gtplayer->Prev != NULL) && (gtplayer->Next == NULL))
    { // 
        gtplayer->Prev->Next = NULL;

        gtplayer->Prev = NULL;
        return true;
    }
    else
    { // 
        gtplayer->Prev->Next = gtplayer->Next;
        gtplayer->Next->Prev = gtplayer->Prev;

        gtplayer->Prev = NULL;
        gtplayer->Next = NULL;
        return true;
    }
}

bool myiocpclt::kickplayer(GatePlayer* gtplayer, long lTimeSec)
{
    Packet* pkt = (Packet *)wpktpool.get();
    pkt->WriteCmd(CMD_MT_KICKUSER); 
	pkt->WriteLong(lTimeSec);
    return sendtoclient(pkt, gtplayer);
}

bool myiocpclt::sendtoworld(Packet* pkt)
{
    if (pkt == NULL) return false;

    Packet* tmp;
    short cnt;

    GatePlayer* pPlayer;
    GateServer* pGate;

    //  Gate 
    for (int i = 0; i < gtnum; ++ i)
    {
        pGate = &gtarray[i];

        if (pGate->IsValid())
        {
            cnt = 0;
            tmp = (Packet *)wpktpool.get();
            if (tmp == NULL)
            {
                continue;
            }

            tmp->clone_from(pkt);

            pPlayer = pGate->GetPlayerList();
            while (pPlayer != NULL)
            {
                tmp->WriteLong(pPlayer->GetDBChaId());
                tmp->WriteLong(pPlayer->GetGateAddr());
                cnt++;

                pPlayer = pPlayer->Next;
            }

            if (cnt > 0)
            {
                tmp->WriteShort(cnt);
                pGate->SendData(tmp);
            }
        }
    }

    pkt->free();
    return true;
}

bool myiocpclt::sendtoclient(Packet* pkt, GatePlayer* playerlist)
{
    if (playerlist == NULL || pkt == NULL) return false;

    //  Gate
    unsigned short usCount[MAX_GATE];
    Packet* CChginf[MAX_GATE];
    Packet* tmppkt;
    GateServer* pValidGate[MAX_GATE];
    short sValidGateNum = 0;    
    for (int i = 0; i < gtnum; ++ i)
    {
        if (gtarray[i].IsValid())
        {
            pValidGate[sValidGateNum] = &gtarray[i];
            usCount[sValidGateNum] = 0;

            tmppkt = (Packet *)wpktpool.get();
            if (tmppkt != NULL) tmppkt->clone_from(pkt);
            CChginf[sValidGateNum] = tmppkt;
            
            sValidGateNum++;
        }
    }

    //  playerlist  Gate 
    GatePlayer* tmp = playerlist;
    while (tmp != NULL)
    {
        if (tmp->GetGate() == NULL)
        {      
#ifdef defCOMMU_LOG
            ToLogService("SendToClient", "WARNING! pGate = NULL, atorID={}, gt_addr={:x}",
                tmp->GetDBChaId(), tmp->GetGateAddr());
#endif
        }
        else
        {
            for (i = 0; i < sValidGateNum; ++ i)
            {
                if ((tmp->GetGate() == pValidGate[i]) && (CChginf[i] != NULL))
                {
                    usCount[i]++;
                    CChginf[i]->WriteLong(tmp->GetDBChaId());
                    CChginf[i]->WriteLong(tmp->GetGateAddr());
                    break;
                }
            }
        }

        tmp = tmp->GetNextPlayer();
    }

    //  
    for (i = 0; i < sValidGateNum; i++)
    {
        if (usCount[i] > 0)
        {
            CChginf[i]->WriteShort(usCount[i]);
            pValidGate[i]->SendData(CChginf[i]);
        }
    }

    pkt->free();
    return true;
}

bool myiocpclt::sendtoclient(Packet* pkt, int array_cnt, uplayer* uplayer_array)
{
    if (uplayer_array == NULL || pkt == NULL) return false;

#ifdef defCOMMU_LOG
    //LG("SendToClient", "\nSendToClient called to notify %d players\n", array_cnt);
#endif

    //  Gate
    unsigned short usCount[MAX_GATE];
    Packet* CChginf[MAX_GATE];
    Packet* tmppkt;
    GateServer* pValidGate[MAX_GATE];
    short sValidGateNum = 0;    
    for (int i = 0; i < gtnum; ++ i)
    {
        if (gtarray[i].IsValid())
        {
            pValidGate[sValidGateNum] = &gtarray[i];
#ifdef defCOMMU_LOG
            //LG("SendToClient", "Valid Gate 0x%x\n", pValidGate[sValidGateNum]);
#endif
            usCount[sValidGateNum] = 0;

            tmppkt = (Packet *)wpktpool.get();
            if (tmppkt != NULL)
            {
                tmppkt->clone_from(pkt);
            }
            CChginf[sValidGateNum] = tmppkt;

            sValidGateNum++;
        }
    }
#ifdef defCOMMU_LOG
    //LG("SendToClient", "Valid Gate num = %d\n", sValidGateNum);
#endif

    //  uplayer_array  Gate 
    int j;
    for (i = 0; i < array_cnt; ++ i)
    {
#ifdef defCOMMU_LOG
        //LG("SendToClient", "atorID = %d, pGate = 0x%x, gt_addr = 0x%x\n", uplayer_array[i].m_dwDBChaId, uplayer_array[i].pGate, uplayer_array[i].m_ulGateAddr);
#endif

        if (uplayer_array[i].pGate == NULL)
        {
#ifdef defCOMMU_LOG
            ToLogService("SendToClient", "WARNING! pGate = NULL, atorID={}, gt_addr={:x}",
                uplayer_array[i].m_dwDBChaId, uplayer_array[i].m_ulGateAddr);
#endif
            continue;
        }

        for (j = 0; j < sValidGateNum; ++ j)
        {
            if ((uplayer_array[i].pGate == pValidGate[j]) && (CChginf[j] != NULL))
            {
                usCount[j]++;
                CChginf[j]->WriteLong(uplayer_array[i].m_dwDBChaId);
                CChginf[j]->WriteLong(uplayer_array[i].m_ulGateAddr);
                break;
            }
        }
    }

    //  
    for (i = 0; i < sValidGateNum; i++)
    {
#ifdef defCOMMU_LOG
        //LG("SendToClient", "send to Gate: %s for %d players\n", pValidGate[i]->GetIP().c_str(), usCount[i]);
#endif
        if (usCount[i] > 0)
        {
            CChginf[i]->WriteShort(usCount[i]);
            pValidGate[i]->SendData(CChginf[i]);
        }
    }

    pkt->free();
    return true;
}

bool myiocpclt::sendtogame(Packet* pkt, uplayer* uplyr)
{
    long l = uplyr->pGate->SendData(pkt);
    pkt->free();
    return (l > 0) ? true : false;
}

myiocpclt* g_mygmsvr = NULL;

#endif
