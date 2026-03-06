#include "stdafx.h"
#include <iostream>
#include "cfl_iocp.h"
#include "cfl_lock.h"
#include "util.h"
#include <exception>
using namespace std;

#pragma warning(disable : 4311 4312)

// LOG...
void cfl_printf(char const* fmt, ...)
    {
    char buf[2048];
    int len; // len  buf 

    va_list args;
    va_start(args, fmt);
    len = vsprintf(buf, fmt, args);
    va_end(args);

    if (len > sizeof buf)
        throw logic_error("cfl_printf: string is longer than 2K!");

    OutputDebugString(buf);}

// I/O Operation word
enum IO_OPERATION {CLIENT_IO_ACCEPT, CLIENT_IO_READ, CLIENT_IO_WRITE, CLIENT_IO_CONNECTED};

// special interface
bool PER_SOCKET_CONTEXT::send(char const* buf, int buf_len)
    {return app->send(this, buf, buf_len);}

// iocp app base class
bool cfl_iocpapp::startup()
    {
    // init the winsock
    WSADATA wd;
    if (WSAStartup(0x202, &wd) != 0) return false;
    else return true;}

void cfl_iocpapp::cleanup() {WSACleanup();}

// iocp client
cfl_iocpclt::cfl_iocpclt(int maxconn)
try : skpool(maxconn), iopool(100), ctxlist(NULL), ctxlist_lock(new cfl_spinlock),
    mq_lock(new cfl_spinlock)
    {
    iocpclt_end = false;
    _verbose = false;

    // determine the thread count
    SYSTEM_INFO si; GetSystemInfo(&si);
    thread_count = min(MAX_WORKER_THREAD, si.dwNumberOfProcessors * 2 + 2);

    // init IOCP
    _hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
    if (NULL == _hIOCP)
        throw bad_exception();

    // create manager thread and walker thread pool
    DWORD dwThreadId;
    mgr_thrd_handle = CreateThread(NULL, 0, mgr_thrd, this, 0, &dwThreadId);

    HANDLE handle;
    for (DWORD i = 0; i < thread_count; ++ i) 
        {
        handle = CreateThread(NULL, 0, wrk_thrd, this, 0, &dwThreadId);
        SetThreadPriority(handle, THREAD_PRIORITY_ABOVE_NORMAL);
        thread_handles[i] = handle;}
    }
catch (bad_alloc&) {throw bad_exception();}
catch (...)
    {
    cfl_printf("Unknown Exception raised from cfl_iocp()\n");};

cfl_iocpclt::~cfl_iocpclt()
    {
    // Cause worker threads to exit
    if (_hIOCP)
        {
        for (DWORD i = 0; i < thread_count; ++ i)
            PostQueuedCompletionStatus(_hIOCP, 0, 0, NULL);}

    // Make sure worker threads exits
    if (WaitForMultipleObjects(thread_count, thread_handles,
                               TRUE, 3000) != WAIT_OBJECT_0)
        {
        cfl_printf("WaitForMultipleObjects failed: %d\n", GetLastError());}
    else {
        for (DWORD i = 0; i < thread_count; ++ i)
            {
            if (thread_handles[i] != INVALID_HANDLE_VALUE)
                CloseHandle(thread_handles[i]);
            thread_handles[i] = INVALID_HANDLE_VALUE;}
        }

    if (mgr_thrd_handle != INVALID_HANDLE_VALUE)
        {
        CloseHandle(mgr_thrd_handle);
        mgr_thrd_handle = INVALID_HANDLE_VALUE;}

    // free the linked list of context
    ctxlist_free();
    delete ctxlist_lock;

    delete mq_lock;

    if (_hIOCP) {CloseHandle(_hIOCP); _hIOCP = NULL;}
    }

PER_SOCKET_CONTEXT* cfl_iocpclt::connect(char const* ipaddr, unsigned short port)
{
    int ret;
    DWORD err;
    SOCKET s;
    SOCKADDR_IN rmt_addr;
    hostent* host = NULL;

    s = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_IP, NULL, 0, WSA_FLAG_OVERLAPPED);
    if (s == INVALID_SOCKET)
    {
        LG("iocp_connect", "WSASocket: %d\n", GetLastError());
        return NULL;
    }

    memset(&rmt_addr, 0, sizeof rmt_addr);
    rmt_addr.sin_family = AF_INET;
    rmt_addr.sin_port = htons(port);
    rmt_addr.sin_addr.s_addr = inet_addr(ipaddr);
    if (rmt_addr.sin_addr.s_addr == INADDR_NONE)
    {
        if (ipaddr == NULL) return NULL;
        host = gethostbyname(ipaddr);

        if (host == NULL) return NULL;
        memcpy(&rmt_addr.sin_addr, host->h_addr_list[0], host->h_length);
    }

    if (::connect(s, (SOCKADDR *)&rmt_addr, sizeof rmt_addr) == SOCKET_ERROR)
    {
        err = WSAGetLastError();
        if (err != WSAEWOULDBLOCK)
        {
            LG("iocp_connect", "connect error: %d\n", err);
            return NULL;
        }
    }

    // set non-block mode
    unsigned long no_block = 1;
    if(ioctlsocket(s, FIONBIO, &no_block))
    {
        LG("iocp_connect", "ioctlsocket error: %d\n", WSAGetLastError());
        closesocket(s);
        return NULL;}

    // set send/recv buffer
    int zero = 0;
    ret = setsockopt(s, SOL_SOCKET, SO_SNDBUF, (char *)&zero, sizeof(zero));
    if (SOCKET_ERROR == ret) 
    {
        LG("iocp_connect", "setsockopt(SNDBUF): %d\n", WSAGetLastError());
        closesocket(s);
        return NULL;
    }

    zero = 0;
    ret = setsockopt(s, SOL_SOCKET, SO_RCVBUF, (char *)&zero, sizeof(zero));
    if (SOCKET_ERROR == ret) 
    {
        LG("iocp_connect", "setsockopt(SO_RCVBUF): %d\n", WSAGetLastError());
        closesocket(s);
        return NULL;
    }

    // set linger
    LINGER mylinger;
    mylinger.l_onoff = 1;
    mylinger.l_linger = 0;
    ret = setsockopt(s, SOL_SOCKET, SO_LINGER, (char *)&mylinger, sizeof(mylinger) );
    if (SOCKET_ERROR == ret) 
    {
        LG("iocp_connect", "setsockopt(SO_LINGER): %d\n", WSAGetLastError());
        closesocket(s);
        return NULL;
    }

    // allocate socket context
    PER_SOCKET_CONTEXT* sk_ctx = ctxlist_alloc(s, CLIENT_IO_CONNECTED);
    if (sk_ctx != NULL)
    {
        sk_ctx->remote_ip = ipaddr;
        sk_ctx->remote_port = port;
    }

    return sk_ctx;
}

void cfl_iocpclt::_postconnmsg(PER_SOCKET_CONTEXT* sk_ctx)
{
    sk_ctx->io_ctx->io_op = CLIENT_IO_CONNECTED;
    PostQueuedCompletionStatus(_hIOCP, DWORD(1), reinterpret_cast<ULONG_PTR>(sk_ctx),
                               &sk_ctx->io_ctx->overlapped);
}

void cfl_iocpclt::_disconnect(PER_SOCKET_CONTEXT* sk_ctx)
{
    ::closesocket(sk_ctx->sk);
}

DWORD WINAPI cfl_iocpclt::mgr_thrd(LPVOID mgr_thrd_ctx)
    {
    cfl_iocpclt* that = (cfl_iocpclt *)mgr_thrd_ctx;
    unsigned short ping_pkt = htons(2);

    while (true)
        {
        if (that->iocpclt_end) break;

        // ping everyone socket
        Sleep(1000);}

    cfl_printf("mgr_thrd exit!\n");
    return 0;}

DWORD WINAPI cfl_iocpclt::wrk_thrd(LPVOID wrk_thrd_ctx)
    {
    cfl_iocpclt* that = (cfl_iocpclt *)wrk_thrd_ctx;
    int nRet = 0;
    BOOL ret = FALSE;
    LPOVERLAPPED overlapped = NULL;
    PER_SOCKET_CONTEXT* sk_ctx = NULL;
    PER_IO_CONTEXT* io_ctx = NULL;
    DWORD io_size = 0, send_bytes = 0;
    WSABUF buffSend;
    DWORD flags = 0;
    DWORD* ptr = NULL;

    while (true)
        {
        // continually loop to service io completion packets
        ret = GetQueuedCompletionStatus(that->_hIOCP, &io_size, (LPDWORD)&sk_ctx,
                                        &overlapped, INFINITE);
        if (!ret)
            cfl_printf("Failed to GetQueuedCompletionStatus: %d\n", GetLastError());

        if (that->iocpclt_end)
            {
            cfl_printf("wrk_thrd: it's time to end thread\n");
            break;}

        if (sk_ctx == NULL)
            {
            cfl_printf("wrk_thrd: sk_ctx = NULL\n");
            continue;}

        if (!ret || (ret && (0 == io_size)))
            {
            // the connection dropped
            if (sk_ctx->connected)
                {
                try {
                    that->on_disconn(sk_ctx);}
                catch (...)
                    {
                    cfl_printf("Exception raised from before_disconn()\n");}

                that->close_client(sk_ctx, false);
                }

            continue;}

        // determine what type of IO packet has completed by checking the PER_IO_CONTEXT 
        // associated with this socket.  This will determine what action to take.
        io_ctx = CONTAINING_RECORD(overlapped, PER_IO_CONTEXT, overlapped);

        switch (io_ctx->io_op)
            {
            case CLIENT_IO_CONNECTED:
                if (that->update_iocp(sk_ctx, CLIENT_IO_READ))
                    {
                    sk_ctx->connected = true;
                    that->ctxlist_add(sk_ctx);

                    try {
                        that->on_connect(sk_ctx);}
                    catch (...)
                        {
                        cfl_printf("Exception raised from on_connect()\n");}
                    }

                break;

            case CLIENT_IO_READ:
                try {
                    that->on_recvdat(sk_ctx, io_size);}
                catch (...)
                    {
                    cfl_printf("Exception raised from on_recvdat\n");}
                break;

            case CLIENT_IO_WRITE:
                io_ctx->io_op = CLIENT_IO_WRITE;
                io_ctx->sent_bytes += io_size;
                flags = 0;
                if (io_ctx->sent_bytes < io_ctx->total_bytes)
                    {
                    // the previous write operation didn't send all the data,
                    // post another send to complete the operation
                    buffSend.buf = io_ctx->buffer + io_ctx->sent_bytes;
                    buffSend.len = io_ctx->total_bytes - io_ctx->sent_bytes;
                    nRet = WSASend(sk_ctx->sk, &buffSend, 1, &send_bytes,
                                   flags, &(io_ctx->overlapped), NULL);
                    if (SOCKET_ERROR == nRet && (ERROR_IO_PENDING != WSAGetLastError()))
                        {
                        cfl_printf("WSASend: %d\n", WSAGetLastError());
                        that->close_client(sk_ctx, false);}
                    }
                else { // previous write operation completed for this socket
                    try {
                        that->on_sendfsh(sk_ctx, io_size);}
                    catch (...)
                        {
                        cfl_printf("Exception raised from on_sendfsh\n");}

                    io_ctx->free();}
                break;

            default:
                break;
            }
        }

    return 0;} 

int cfl_iocpclt::on_connect(PER_SOCKET_CONTEXT* sk_ctx)
    {return 0;}

int cfl_iocpclt::on_recvdat(PER_SOCKET_CONTEXT* sk_ctx, DWORD io_size)
    {return 0;}

int cfl_iocpclt::on_sendfsh(PER_SOCKET_CONTEXT* sk_ctx, DWORD io_size)
    {return 0;}

int cfl_iocpclt::on_disconn(PER_SOCKET_CONTEXT* sk_ctx)
    {return 0;}

bool cfl_iocpclt::update_iocp(PER_SOCKET_CONTEXT* sk_ctx, IO_OPERATION client_io)
    {
    sk_ctx->io_ctx->io_op = client_io;

    _hIOCP= CreateIoCompletionPort((HANDLE)sk_ctx->sk, _hIOCP, DWORD(sk_ctx), 0);
    if (NULL == _hIOCP)
        {
        cfl_printf("CreateIocompletionPort: %d\n", GetLastError());
        return false;}
    return true;}

void cfl_iocpclt::_recv(PER_SOCKET_CONTEXT* sk_ctx, DWORD start_pos /* = 0 */)
    {
    int ret = 0;
    DWORD send_bytes = 0;
    DWORD flags = 0;
    WSABUF buffRecv;
    PER_IO_CONTEXT* io_ctx = sk_ctx->io_ctx;

    io_ctx->io_op = CLIENT_IO_READ;
    buffRecv.buf = io_ctx->buffer + start_pos;
    buffRecv.len = MAX_BUFF_SIZE - start_pos;
    ret = WSARecv(sk_ctx->sk, &buffRecv, 1, &send_bytes, &flags,
                  &io_ctx->overlapped, NULL);
    if ((SOCKET_ERROR == ret) && (WSA_IO_PENDING != WSAGetLastError())) 
        {
        cfl_printf("WSARecv: %d\n", WSAGetLastError());
        close_client(sk_ctx, false);}
    return;}

bool cfl_iocpclt::send(PER_SOCKET_CONTEXT* sk_ctx, char const* buf, int buf_len)
    {
    if (!sk_ctx->connected || (buf == NULL) || (buf_len > MAX_BUFF_SIZE))
        {
        cfl_printf("socket not connect or buffer is NULL or length too long: [%dBytes]\n", buf_len);
        return false;}

    int ret = 0;
    DWORD send_bytes = 0;
    PER_IO_CONTEXT* io_ctx = iopool.get();
    if (io_ctx == NULL)
        {
        cfl_printf("Warning! iopool is empty!!!\n");
        return false;}

    io_ctx->io_op = CLIENT_IO_WRITE;
    io_ctx->total_bytes = buf_len;
    io_ctx->sent_bytes = 0;
    io_ctx->wsabuf.len = buf_len;
    //io_ctx->wsabuf.buf = buf;
    memcpy(io_ctx->buffer, buf, buf_len);
    io_ctx->wsabuf.buf = io_ctx->buffer;
    ret = WSASend(sk_ctx->sk, &io_ctx->wsabuf, 1, &send_bytes, 0,
                  &(io_ctx->overlapped), NULL);
    if (SOCKET_ERROR == ret && (ERROR_IO_PENDING != WSAGetLastError())) 
        {
        cfl_printf("WSASend: %d\n", WSAGetLastError());
        close_client(sk_ctx, false); return false;}
    else return true;}

void cfl_iocpclt::close_client(PER_SOCKET_CONTEXT* sk_ctx, bool graceful)
    {
    ctxlist_lock->lock();
    try {
        if (sk_ctx == NULL) {cfl_printf("close_client: sk_ctx is NULL\n"); return;}
        if (_verbose)
            {
            cfl_printf("close_client: sk(%d) connection closing (graceful=%s)\n",
                     sk_ctx->sk, (graceful ? "TRUE" : "FALSE"));}

        if (!graceful)
            { // force the subsequent closesocket to be abortative.
            LINGER lingerStruct;
            lingerStruct.l_onoff = 1;
            lingerStruct.l_linger = 0;
            setsockopt(sk_ctx->sk, SOL_SOCKET, SO_LINGER, (char *)&lingerStruct,
                       sizeof(lingerStruct));}

        closesocket(sk_ctx->sk);
        sk_ctx->connected = false;
        ctxlist_del(sk_ctx);}
    catch (...) {}
    ctxlist_lock->unlock();} 

// linked list of socket context
PER_SOCKET_CONTEXT* cfl_iocpclt::ctxlist_alloc(SOCKET sk, IO_OPERATION client_io)
    {
    PER_SOCKET_CONTEXT* sk_ctx{};

    ctxlist_lock->lock();
    try {
        sk_ctx = skpool.get();
        if (NULL == sk_ctx)
            {
            cfl_printf("Warning! socket pool is empty!!!");
            ctxlist_lock->unlock();
            return NULL;}

        sk_ctx->io_ctx = iopool.get();
        if (NULL == sk_ctx->io_ctx)
            {
            sk_ctx->free();
            cfl_printf("Warning! i/o pool is empty!!!");
            ctxlist_lock->unlock();
            return NULL;}

        sk_ctx->sk = sk;
        sk_ctx->app = this;

        sk_ctx->io_ctx->io_op = client_io;}
    catch (...) {}
    ctxlist_lock->unlock();

    return sk_ctx;}

void cfl_iocpclt::ctxlist_add(PER_SOCKET_CONTEXT* sk_ctx)
    {
    PER_SOCKET_CONTEXT* pTemp;

    ctxlist_lock->lock();
    try {
        if (ctxlist == NULL) 
            {  
            // add the first node to the linked list
            sk_ctx->back = NULL;
            sk_ctx->forward = NULL;
            ctxlist = sk_ctx;}
        else{
            // add node to head of list
            pTemp = ctxlist;

            ctxlist = sk_ctx;
            sk_ctx->back = pTemp;
            sk_ctx->forward = NULL;

            pTemp->forward = sk_ctx;}
        } catch (...) {}
    ctxlist_lock->unlock();}

void cfl_iocpclt::ctxlist_del(PER_SOCKET_CONTEXT* sk_ctx)
    {
    PER_SOCKET_CONTEXT* pBack;
    PER_SOCKET_CONTEXT* pForward;
    PER_IO_CONTEXT* pNextIO = NULL;
    PER_IO_CONTEXT* pTempIO = NULL;

    ctxlist_lock->lock();
    try {
        if (!sk_ctx) 
            {
            cfl_printf("ctxlist_del: sk_ctx is NULL\n");
            return;}

        pBack = sk_ctx->back;
        pForward = sk_ctx->forward;

        if (pBack == NULL && pForward == NULL) 
            { // This is the only node in the list to delete
            ctxlist = NULL;}
        else if (pBack == NULL && pForward != NULL) 
            { // This is the end node in the list to delete
            pForward->back = NULL;}
        else if (pBack != NULL && pForward == NULL) 
            { // This is the start node in the list to delete
            pBack->forward = NULL;
            ctxlist = pBack;}
        else if (pBack && pForward) 
            { // Neither start node nor end node in the list
            pBack->forward = pForward;
            pForward->back = pBack;}

        // Free all i/o context structures per socket
        pTempIO = (PER_IO_CONTEXT *)(sk_ctx->io_ctx);
        do 
            {
            pNextIO = (PER_IO_CONTEXT *)(pTempIO->forward);
            if (pTempIO) 
                {
                //The overlapped structure is safe to free when only the posted i/o has
                //completed. Here we only need to test those posted but not yet received 
                //by PQCS in the shutdown process.
                if (iocpclt_end) 
                    {
                    while (!HasOverlappedIoCompleted((LPOVERLAPPED)pTempIO))
                        Sleep(0);}

                pTempIO->free();
                pTempIO = NULL;}

            pTempIO = pNextIO;
            } while (pNextIO);

        sk_ctx->free();
        sk_ctx = NULL;}
    catch (...) {}
    ctxlist_lock->unlock();}

void cfl_iocpclt::ctxlist_free()
    {
    PER_SOCKET_CONTEXT* pTemp1;
    PER_SOCKET_CONTEXT* pTemp2;

    ctxlist_lock->lock();
    try {
        pTemp1 = ctxlist;
        while (pTemp1) 
            {
            pTemp2 = pTemp1->back;
            close_client(pTemp1, false);
            pTemp1 = pTemp2;}
        }
    catch (...) {}
    ctxlist_lock->unlock();}
