#pragma once

// TcpServer  TCP listener  accept .
//
// :
//   Accept Thread: accept()  pending queue ()
//   Game Thread:   PollAll()   pending accepts  TcpClient::Attach()
//                    pendingDisconnect  OnDisconnected
//                   PollPackets()     OnPacket
//
//     TcpClient  Attach-.

#include "TcpClient.h"
#include <memory>
#include <string>
#include <vector>

namespace net {

// 
//  ITcpServerHandler  callback'  
// 

struct ITcpServerHandler {
    virtual ~ITcpServerHandler() = default;

    //   .  false   ( ).
    virtual bool OnAccepted(TcpClient* client) = 0;

    //   .   PollAll  PollPackets.
    virtual void OnPacket(TcpClient* client, RPacket& pkt) = 0;

    //  .   client  .
    virtual void OnDisconnected(TcpClient* client, int reason) = 0;
};

// 
//  TcpServer  TCP listener + accept thread
// 

class TcpServer {
public:
    TcpServer();
    ~TcpServer();

    TcpServer(const TcpServer&) = delete;
    TcpServer& operator=(const TcpServer&) = delete;

    //   . maxConns    .
    bool Listen(const std::string& ip, uint16_t port, int maxConns = 16);

    //  :  listen socket,  accept thread,  .
    void Shutdown();

    //   : pending accepts, disconnects, incoming packets.
    // maxPktsPerConn         .
    //     .
    int PollAll(int maxPktsPerConn = 50);

    void SetHandler(ITcpServerHandler* handler) { _handler = handler; }

    //   
    void DisconnectClient(TcpClient* client, int reason = 0);

    //  
    void DisconnectAll();

    //   
    int GetConnectionCount() const;

    //   
    bool IsListening() const { return _listening.load(); }

private:
    // Accept thread:  accept()  pending queue
    void AcceptThreadProc();

    SOCKET _listenSocket;
    std::thread _acceptThread;
    std::atomic<bool> _listening;
    int _maxConns;
    ITcpServerHandler* _handler;

    // Handler-:  ITcpClientHandler::OnPacket  ITcpServerHandler::OnPacket(client, pkt)
    struct ClientHandlerAdapter : ITcpClientHandler {
        ITcpServerHandler* serverHandler = nullptr;
        TcpClient* client = nullptr;

        void OnPacket(RPacket& packet) override {
            if (serverHandler) serverHandler->OnPacket(client, packet);
        }
        void OnDisconnected(int) override {
            //  : disconnect   HasPendingDisconnect  PollAll
        }
    };

    //  = TcpClient +  handler-
    struct Connection {
        std::unique_ptr<TcpClient> client;
        std::unique_ptr<ClientHandlerAdapter> adapter;
    };

    std::vector<Connection> _connections;

    // Pending accepts (accept thread  main thread)
    struct PendingAccept {
        SOCKET sock;
        std::string peerIP;
        uint16_t peerPort;
    };
    std::mutex _pendingMtx;
    std::vector<PendingAccept> _pendingAccepts;
};

} // namespace net
