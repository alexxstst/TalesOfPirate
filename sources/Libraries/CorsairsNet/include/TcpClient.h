#pragma once

// TcpClient — TCP клиент с выделенным потоком приёма.
//
// Архитектура:
//   Recv Thread: блокирующий recv() → сборка кадра → дешифровка → очередь
//   Game Thread: PollPackets() → OnPacket callback
//   Game Thread: Send() → шифровка → send() в сокет
//
// Шифрование через ICryptoProvider (реализуется в клиенте).

#include "Packet.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <cstdint>
#include <atomic>
#include <mutex>
#include <queue>
#include <thread>
#include <functional>

namespace net {

// ═══════════════════════════════════════════════════════════════
//  ICryptoProvider — интерфейс шифрования
// ═══════════════════════════════════════════════════════════════

struct ICryptoProvider {
    virtual ~ICryptoProvider() = default;

    // Активно ли шифрование (handshake завершён)
    virtual bool IsActive() const = 0;

    // Шифрует данные. ciphertext — выходной буфер, ciphertext_len — его размер.
    // plaintext — входные данные, len — на входе размер plaintext, на выходе размер ciphertext.
    virtual bool Encrypt(uint8_t* ciphertext, int ciphertext_len,
                         const uint8_t* plaintext, int& len) = 0;

    // Дешифрует данные in-place. len — на входе размер ciphertext, на выходе размер plaintext.
    virtual bool Decrypt(uint8_t* data, int& len) = 0;
};

// ═══════════════════════════════════════════════════════════════
//  ITcpClientHandler — callback'и событий
// ═══════════════════════════════════════════════════════════════

struct ITcpClientHandler {
    virtual ~ITcpClientHandler() = default;

    // Вызывается из game thread при PollPackets
    virtual void OnPacket(RPacket& packet) = 0;

    // Вызывается при разрыве соединения (из recv thread)
    virtual void OnDisconnected(int reason) = 0;
};

// ═══════════════════════════════════════════════════════════════
//  TcpClient — TCP клиент с recv потоком
// ═══════════════════════════════════════════════════════════════

class TcpClient {
public:
    TcpClient();
    ~TcpClient();

    TcpClient(const TcpClient&) = delete;
    TcpClient& operator=(const TcpClient&) = delete;

    // ── Lifecycle ───────────────────────────────────────────

    // Подключение к серверу. Блокирующий вызов.
    bool Connect(const char* host, uint16_t port, uint32_t timeoutMs = 5000);

    // Отключение. reason: 0 = нормальное, <0 = ошибка.
    void Disconnect(int reason = 0);

    // Статус соединения
    bool IsConnected() const { return _connected.load(); }

    // ── Отправка (из game thread) ───────────────────────────

    // Отправить пакет. Шифрует если crypto активен. Блокирует до завершения send().
    bool Send(WPacket& packet);

    // ── Приём (из game thread) ──────────────────────────────

    // Обработать до maxPackets пакетов из очереди.
    // Для каждого вызывает handler->OnPacket(). Возвращает кол-во обработанных.
    int PollPackets(int maxPackets = 1);

    // ── Настройка ───────────────────────────────────────────

    void SetHandler(ITcpClientHandler* handler) { _handler = handler; }
    void SetCrypto(ICryptoProvider* crypto) { _crypto = crypto; }

    // ── Информация ──────────────────────────────────────────

    SOCKET GetSocket() const { return _socket; }

private:
    // Поток приёма
    void RecvThreadProc();

    // Блокирующее чтение ровно len байт. Возвращает false при ошибке/закрытии.
    bool RecvExact(uint8_t* buf, int len);

    // Отправка ровно len байт. Возвращает false при ошибке.
    bool SendExact(const uint8_t* buf, int len);

    SOCKET _socket;
    std::atomic<bool> _connected;
    std::atomic<bool> _pendingDisconnect;  // recv thread → game thread уведомление
    std::thread _recvThread;

    ITcpClientHandler* _handler;
    ICryptoProvider* _crypto;

    // Потокобезопасная очередь входящих пакетов
    struct PacketQueue {
        std::mutex mtx;
        std::queue<RPacket> packets;

        void Push(RPacket&& pkt) {
            std::lock_guard<std::mutex> lock(mtx);
            packets.push(std::move(pkt));
        }

        bool Pop(RPacket& out) {
            std::lock_guard<std::mutex> lock(mtx);
            if (packets.empty()) return false;
            out = std::move(packets.front());
            packets.pop();
            return true;
        }

        void Clear() {
            std::lock_guard<std::mutex> lock(mtx);
            while (!packets.empty()) packets.pop();
        }
    };

    PacketQueue _recvQueue;

    // Мьютекс отправки (на случай вызова Send из разных мест)
    std::mutex _sendMtx;
};

// ═══════════════════════════════════════════════════════════════
//  WinSock — глобальная инициализация
// ═══════════════════════════════════════════════════════════════

// Вызвать один раз при старте приложения
bool InitWinSock();
void CleanupWinSock();

} // namespace net
