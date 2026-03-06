#pragma once

/*--

    cfl_iocp.h

     Be sure to use this class on Windows NT

 Author:

     clauDe @2005-1-15 10:30

--*/

#ifndef _CFL_IOCP_H_
#define _CFL_IOCP_H_

#include <winsock2.h>
#include <string>
#include <list>
#include "cfl_mempool.h"

#define MAX_BUFF_SIZE (8 * 1024)
#define MAX_WORKER_THREAD 16

enum IO_OPERATION;

struct PER_IO_CONTEXT : public cfl_mpelem
    {
    WSAOVERLAPPED overlapped;
    char buffer[MAX_BUFF_SIZE];
    WSABUF wsabuf;
    int total_bytes;
    int sent_bytes;
    IO_OPERATION io_op;
    SOCKET accept_sk;
    PER_IO_CONTEXT* forward;
        
    void on_get()
        {
        overlapped.Internal = 0;
        overlapped.InternalHigh = 0;
        overlapped.Offset = 0;
        overlapped.OffsetHigh = 0;
        overlapped.hEvent = NULL;

        forward = NULL;
        total_bytes = 0;
        sent_bytes = 0;
        wsabuf.buf = buffer;
        wsabuf.len = sizeof buffer;}
    };

class cfl_iocpapp;
struct PER_SOCKET_CONTEXT : public cfl_mpelem
    {
    SOCKET sk;
    cfl_iocpapp* app;
    bool connected;

    std::string remote_ip;
    unsigned short remote_port;
    void* usrdat;

    PER_IO_CONTEXT* io_ctx;

    PER_SOCKET_CONTEXT* back;
    PER_SOCKET_CONTEXT* forward;

    bool send(char const* buf, int buf_len);
    void setusrdat(void* p) {usrdat = p;}
    void* getusrdat() const {return usrdat;}

    void on_get()
        {
        connected = false;

        back = NULL;
        forward = NULL;}

    void on_ret()
        {
        connected = false;}
    };

class cfl_spinlock;
class cfl_iocpapp
    {
protected:
    cfl_iocpapp() {}
    virtual ~cfl_iocpapp() {}

public:
    static bool startup();
    static void cleanup();
    virtual bool send(PER_SOCKET_CONTEXT* sk_ctx, char const* buf, int buf_len) = 0;};

class cfl_iocpclt : public cfl_iocpapp
    {
public:
    cfl_iocpclt(int maxconn = 10);
    ~cfl_iocpclt();

public:
    PER_SOCKET_CONTEXT* connect(char const* ipaddr, unsigned short port);
    bool send(PER_SOCKET_CONTEXT* sk_ctx, char const* buf, int buf_len);
    void exit() {iocpclt_end = true;}
    bool get_exit_flag() const {return iocpclt_end;}

protected:
    void _recv(PER_SOCKET_CONTEXT* sk_ctx, DWORD start_pos = 0);
    void _postconnmsg(PER_SOCKET_CONTEXT* sk_ctx);
    void _disconnect(PER_SOCKET_CONTEXT* sk_ctx);

    virtual int on_connect(PER_SOCKET_CONTEXT* sk_ctx);
    virtual int on_sendfsh(PER_SOCKET_CONTEXT* sk_ctx, DWORD size);
    virtual int on_recvdat(PER_SOCKET_CONTEXT* sk_ctx, DWORD size);
    virtual int on_disconn(PER_SOCKET_CONTEXT* sk_ctx);    

private:
    
    // manager thread
    HANDLE mgr_thrd_handle;
    static DWORD WINAPI mgr_thrd(LPVOID);
    std::list<int> mgr_queue;
    cfl_spinlock* mq_lock;

    // walker thread pool
    DWORD thread_count;
    HANDLE thread_handles[MAX_WORKER_THREAD];
    static DWORD WINAPI wrk_thrd(LPVOID);

    // iocp model
    HANDLE _hIOCP;
    bool update_iocp(PER_SOCKET_CONTEXT* sk_ctx, IO_OPERATION client_io);

    // socket and i/o context pool
    cfl_mp<PER_SOCKET_CONTEXT> skpool;
    cfl_mp<PER_IO_CONTEXT> iopool;

    // linked list of socket context
    cfl_spinlock* ctxlist_lock;
    PER_SOCKET_CONTEXT* ctxlist;
    PER_SOCKET_CONTEXT* ctxlist_alloc(SOCKET sk, IO_OPERATION client_io);
    void ctxlist_add(PER_SOCKET_CONTEXT* sk_ctx);
    void ctxlist_del(PER_SOCKET_CONTEXT* sk_ctx);
    void ctxlist_free();

    // server configuration
    bool volatile iocpclt_end;
    bool _verbose;
    std::string _ip;
    unsigned short _port;

    void close_client(PER_SOCKET_CONTEXT* sk_ctx, bool graceful);
    };

void cfl_printf(char const* fmt, ...);

#endif

