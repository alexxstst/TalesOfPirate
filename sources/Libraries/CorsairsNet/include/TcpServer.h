#pragma once

// TcpServer — TCP listener с accept потоком.
//
// Архитектура:
//   Accept Thread: accept() → pending queue (потокобезопасная)
//   Game Thread:   PollAll() → обработка pending accepts → TcpClient::Attach()
//                  → проверка pendingDisconnect → OnDisconnected
//                  → PollPackets() для каждого подключения → OnPacket
//
// Каждое принятое подключение — TcpClient в Attach-режиме.

#include "TcpClient.h"
#include <memory>
#include <string>
#include <vector>

namespace net {

// ═══════════════════════════════════════════════════════════════
//  ITcpServerHandler — callback'и серверных событий
// ═══════════════════════════════════════════════════════════════

struct ITcpServerHandler {
    virtual ~ITcpServerHandler() = default;

    // Новое подключение принято. Вернуть false чтобы отклонить (закроет сокет).
    virtual bool OnAccepted(TcpClient* client) = 0;

    // Пакет от подключения. Вызывается из PollAll → PollPackets.
    virtual void OnPacket(TcpClient* client, RPacket& pkt) = 0;

    // Подключение разорвано. После вызова client будет удалён.
    virtual void OnDisconnected(TcpClient* client, int reason) = 0;
};

// ═══════════════════════════════════════════════════════════════
//  TcpServer — TCP listener + accept thread
// ═══════════════════════════════════════════════════════════════

class TcpServer {
public:
    TcpServer();
    ~TcpServer();

    TcpServer(const TcpServer&) = delete;
    TcpServer& operator=(const TcpServer&) = delete;

    // Начать слушать порт. maxConns — максимум одновременных подключений.
    bool Listen(const std::string& ip, uint16_t port, int maxConns = 16);

    // Остановить сервер: закрыть listen socket, дождаться accept thread, отключить всех.
    void Shutdown();

    // Обработать все события: pending accepts, disconnects, incoming packets.
    // maxPktsPerConn — сколько пакетов обработать за раз для каждого подключения.
    // Возвращает общее количество обработанных пакетов.
    int PollAll(int maxPktsPerConn = 50);

    void SetHandler(ITcpServerHandler* handler) { _handler = handler; }

    // Отключить конкретного клиента
    void DisconnectClient(TcpClient* client, int reason = 0);

    // Отключить всех
    void DisconnectAll();

    // Текущее количество подключений
    int GetConnectionCount() const;

    // Слушает ли сервер
    bool IsListening() const { return _listening.load(); }

private:
    // Accept thread: блокирующий accept() → pending queue
    void AcceptThreadProc();

    SOCKET _listenSocket;
    std::thread _acceptThread;
    std::atomic<bool> _listening;
    int _maxConns;
    ITcpServerHandler* _handler;

    // Handler-адаптер: перенаправляет ITcpClientHandler::OnPacket → ITcpServerHandler::OnPacket(client, pkt)
    struct ClientHandlerAdapter : ITcpClientHandler {
        ITcpServerHandler* serverHandler = nullptr;
        TcpClient* client = nullptr;

        void OnPacket(RPacket& packet) override {
            if (serverHandler) serverHandler->OnPacket(client, packet);
        }
        void OnDisconnected(int) override {
            // Не используется: disconnect обрабатывается через HasPendingDisconnect в PollAll
        }
    };

    // Подключение = TcpClient + его handler-адаптер
    struct Connection {
        std::unique_ptr<TcpClient> client;
        std::unique_ptr<ClientHandlerAdapter> adapter;
    };

    std::vector<Connection> _connections;

    // Pending accepts (accept thread → main thread)
    struct PendingAccept {
        SOCKET sock;
        std::string peerIP;
        uint16_t peerPort;
    };
    std::mutex _pendingMtx;
    std::vector<PendingAccept> _pendingAccepts;
};

} // namespace net
