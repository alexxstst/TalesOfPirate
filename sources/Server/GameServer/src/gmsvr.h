
#ifndef GMSVR_H_
#define GMSVR_H_

#ifdef USE_IOCP

#include "cfl_iocp.h"
#include "cfl_mempool.h"
#define USE_NBO
#include "cfl_pkt.h"

#include "gtplayer.h"
#include "config.h"

#define DataSocket PER_SOCKET_CONTEXT
#define GameServerApp myiocpclt

class myiocpclt;
class GateServer
    {
public:
    GateServer() {Invalid();}
    ~GateServer() {Invalid();}

    void SetDataSock(DataSocket* datasock, GameServerApp* gmsvr)
        {
        sk_ctx = datasock;
        datasock->setusrdat(this);
        myclt = gmsvr;}
    DataSocket* GetDataSock() const {return sk_ctx;}

    void Invalid() {sk_ctx = NULL;}
    bool IsValid() {return (sk_ctx != NULL) ? true : false;}

    string& GetIP() {return ipaddr;}
    unsigned short& GetPort() {return port;}
    string& GetName() {return m_gtname;}

    long SendData(Packet* sendbuf)
        {
        if (sendbuf == NULL) return 0;

        long l;
        l = IsValid() ? send(sendbuf) : 0;
        sendbuf->free();
        return l;
        }

    GatePlayer*& GetPlayerList() {return m_playerlist;}

    Packet* getpkt();
    
public:
    bool send(Packet* pkt);

    PER_SOCKET_CONTEXT* sk_ctx;
    myiocpclt* myclt;

    std::string ipaddr;
    unsigned short port;
    
    // Player 
    GatePlayer* m_playerlist;
	GatePlayer* m_listcurplayer;	// 
    std::string m_gtname;
    
    };

struct mymsg : public cfl_mpelem
    {
    DWORD type;
    GateServer* gt;
    Packet* pkt;};

class myiocpclt : public cfl_iocpclt
    {
public:
    myiocpclt();
    ~myiocpclt() {}

protected:

    void postmsg(DWORD msg_type, GateServer* gt, Packet* pkt);
    mymsg* recvmsg();

    virtual int on_connect(PER_SOCKET_CONTEXT* sk_ctx);
    virtual int on_disconn(PER_SOCKET_CONTEXT* sk_ctx);
    virtual int on_recvdat(PER_SOCKET_CONTEXT* sk_ctx, DWORD size);
    virtual int on_sendfsh(PER_SOCKET_CONTEXT* sk_ctx, DWORD size);

    static DWORD WINAPI conn_thrd(LPVOID);

public:

    bool init_gtconn(CGameConfig* gmcfg);
    bool conn_gate(GateServer* gt);
    void disconnect(PER_SOCKET_CONTEXT* sk_ctx) {_disconnect(sk_ctx);}
    void peekpkt(int ms = 0);
    Packet* getpkt() {return (Packet *)wpktpool.get();}
    bool isvalidgt(int i);

    bool sendtoworld(Packet* pkt);
    bool sendtoclient(Packet* pkt, GatePlayer* playerlist);
    bool sendtoclient(Packet* pkt, int array_cnt, uplayer* uplayer_array);
    bool sendtogame(Packet* pkt, uplayer* uplyr);

    bool addplayer(GatePlayer* gtplayer, GateServer* gt, unsigned long gtaddr);
    bool delplayer(GatePlayer* gtplayer);
    bool kickplayer(GatePlayer* gtplayer, long lTimeSec = 0);
	// add by xuedong
	bool begingetplayer(GateServer* gt);
	GatePlayer* getnextplayer(GateServer* gt);

	void begingetgate(void);
	GateServer* getnextgate(void);
	//

    char const* getname() const {return gmsvr_name.c_str();}
    GateServer* find_gt(char const* gtname);

private:

    cfl_mp<Packet> rpktpool;
    cfl_mp<Packet> wpktpool;

    cfl_mp<mymsg> msgpool;

    short pkthdr_size;

    string gmsvr_name;
    GateServer gtarray[MAX_GATE];
    short gtnum;
	short listcurgt;

    std::list<mymsg *> _msgqueue;
    cfl_spinlock _mqlock;};


inline Packet* GateServer::getpkt() {return myclt->getpkt();}
inline bool GateServer::send(Packet* pkt)
{
    return sk_ctx->send((char const *)pkt->pkt_buf(), pkt->pkt_len());
}

inline bool myiocpclt::isvalidgt(int i)
{
    if (i < 0 || i >= gtnum) throw std::logic_error("index < 0 or >= gtnum");

    return gtarray[i].IsValid();
}

inline GateServer* myiocpclt::find_gt(char const* gt_name)
{
    for (int i = 0; i < gtnum; ++ i)
    {
        if (gtarray[i].IsValid())
        {
            if (gtarray[i].GetName().compare(gt_name) == 0)
                return &gtarray[i];
        }
    }

    return NULL;
}

inline bool myiocpclt::begingetplayer(GateServer* gt)
{
    if (gt == NULL) return false;
    if (!gt->IsValid()) return false;

	gt->m_listcurplayer = gt->GetPlayerList();

	return true;
}

inline GatePlayer* myiocpclt::getnextplayer(GateServer* gt)
{
	GatePlayer*	pretplayer = gt->m_listcurplayer;
	if (gt->m_listcurplayer)
		gt->m_listcurplayer = gt->m_listcurplayer->Next;

	return pretplayer;
}

inline void myiocpclt::begingetgate(void)
{
	listcurgt = 0;
}

inline GateServer* myiocpclt::getnextgate(void)
{
	while (listcurgt < gtnum)
	{
	    if (gtarray[listcurgt++].IsValid())
			return gtarray + (listcurgt - 1);
	}

	return 0;
}

extern myiocpclt* g_mygmsvr;


inline void uplayer::Init(char const* gt_name, unsigned long gt_addr, DWORD atorID)
{
    pGate = g_mygmsvr->find_gt(gt_name);
    m_dwDBChaId = atorID;
    m_ulGateAddr = gt_addr;
}

#endif

#endif // GMSVR_H_
