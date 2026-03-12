#include "TcpClient.h"
#include <cassert>

#pragma comment(lib, "ws2_32.lib")

namespace net {

// ═══════════════════════════════════════════════════════════════
//  WinSock init/cleanup
// ═══════════════════════════════════════════════════════════════

bool InitWinSock() {
    WSADATA wsa;
    return WSAStartup(MAKEWORD(2, 2), &wsa) == 0;
}

void CleanupWinSock() {
    WSACleanup();
}

// ═══════════════════════════════════════════════════════════════
//  TcpClient — реализация
// ═══════════════════════════════════════════════════════════════

TcpClient::TcpClient()
    : _socket(INVALID_SOCKET), _connected(false), _pendingDisconnect(false),
      _handler(nullptr), _crypto(nullptr) {}

TcpClient::~TcpClient() {
    Disconnect(0);
}

// ── Connect ────────────────────────────────────────────────

bool TcpClient::Connect(const char* host, uint16_t port, uint32_t timeoutMs) {
    if (_connected) return false;

    // Резолв адреса
    addrinfo hints{};
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    char portStr[8];
    snprintf(portStr, sizeof(portStr), "%u", port);

    addrinfo* result = nullptr;
    if (getaddrinfo(host, portStr, &hints, &result) != 0) {
        return false;
    }

    // Создание сокета
    _socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (_socket == INVALID_SOCKET) {
        freeaddrinfo(result);
        return false;
    }

    // Non-blocking connect с timeout
    u_long nonBlocking = 1;
    ioctlsocket(_socket, FIONBIO, &nonBlocking);

    int connectResult = connect(_socket, result->ai_addr, (int)result->ai_addrlen);
    freeaddrinfo(result);

    if (connectResult == SOCKET_ERROR) {
        int err = WSAGetLastError();
        if (err != WSAEWOULDBLOCK) {
            closesocket(_socket);
            _socket = INVALID_SOCKET;
            return false;
        }

        // Ожидание подключения с timeout
        fd_set writeSet;
        FD_ZERO(&writeSet);
        FD_SET(_socket, &writeSet);

        fd_set exceptSet;
        FD_ZERO(&exceptSet);
        FD_SET(_socket, &exceptSet);

        timeval tv;
        tv.tv_sec = timeoutMs / 1000;
        tv.tv_usec = (timeoutMs % 1000) * 1000;

        int selResult = select(0, nullptr, &writeSet, &exceptSet, &tv);
        if (selResult <= 0 || FD_ISSET(_socket, &exceptSet)) {
            closesocket(_socket);
            _socket = INVALID_SOCKET;
            return false;
        }
    }

    // Обратно в blocking mode для recv thread
    nonBlocking = 0;
    ioctlsocket(_socket, FIONBIO, &nonBlocking);

    // Отключаем Nagle для минимальной задержки
    int noDelay = 1;
    setsockopt(_socket, IPPROTO_TCP, TCP_NODELAY, (const char*)&noDelay, sizeof(noDelay));

    _connected = true;
    _pendingDisconnect = false;

    // Запуск recv потока
    _recvThread = std::thread(&TcpClient::RecvThreadProc, this);

    return true;
}

// ── Disconnect ─────────────────────────────────────────────

void TcpClient::Disconnect(int reason) {
    bool expected = true;
    if (!_connected.compare_exchange_strong(expected, false)) {
        return; // Уже отключены
    }

    // Закрыть сокет — это разблокирует recv() в потоке
    if (_socket != INVALID_SOCKET) {
        shutdown(_socket, SD_BOTH);
        closesocket(_socket);
        _socket = INVALID_SOCKET;
    }

    // Ждём завершения recv потока
    if (_recvThread.joinable()) {
        _recvThread.join();
    }

    // Очищаем очередь
    _recvQueue.Clear();

    // Уведомляем handler
    if (_handler) {
        _handler->OnDisconnected(reason);
    }
}

// ── Send ───────────────────────────────────────────────────

bool TcpClient::Send(WPacket& packet) {
    if (!_connected) return false;

    std::lock_guard<std::mutex> lock(_sendMtx);

    int pktSize = packet.GetPacketSize();

    // Шифрование CMD+payload (offset 6)
    if (_crypto && _crypto->IsActive() && pktSize > 6) {
        int dataLen = pktSize - 6;
        // AES-GCM overhead: nonce(12) + tag(16) = 28 байт
        int maxEncLen = dataLen + 28;

        // Временный буфер для шифрованных данных
        WPacket encrypted(maxEncLen);

        // Копируем SESS [2..5]
        std::memcpy(encrypted.Data() + 2, packet.Data() + 2, 4);

        int encLen = dataLen;
        if (!_crypto->Encrypt(encrypted.Data() + 6, maxEncLen,
                              packet.Data() + 6, encLen)) {
            return false;
        }

        int newSize = 6 + encLen;
        encrypted.SetPacketSize(newSize);

        return SendExact(encrypted.Data(), newSize);
    }

    return SendExact(packet.Data(), pktSize);
}

// ── PollPackets ────────────────────────────────────────────

int TcpClient::PollPackets(int maxPackets) {
    if (!_handler) return 0;

    int processed = 0;
    RPacket pkt;

    while (processed < maxPackets && _recvQueue.Pop(pkt)) {
        _handler->OnPacket(pkt);
        ++processed;
    }

    // Проверяем отложенное уведомление о разрыве от recv thread
    bool expected = true;
    if (_pendingDisconnect.compare_exchange_strong(expected, false)) {
        _recvQueue.Clear();
        if (_recvThread.joinable()) {
            _recvThread.join();
        }
        _handler->OnDisconnected(-1);
    }

    return processed;
}

// ── RecvThreadProc ─────────────────────────────────────────

void TcpClient::RecvThreadProc() {
    uint8_t sizeHeader[2];

    while (_connected) {
        // 1. Читаем 2 байта размера пакета
        if (!RecvExact(sizeHeader, 2)) break;

        int pktSize = static_cast<int>(readUInt16(sizeHeader));
        if (pktSize < 8 || pktSize > 65535) break; // Невалидный кадр

        // 2. Аллоцируем буфер из пула
        int bucketSize = PacketPool::Shared().BucketSize(pktSize);
        uint8_t* buf = PacketPool::Shared().Allocate(pktSize);

        // Копируем size header
        writeUInt16(buf, static_cast<uint16_t>(pktSize));

        // 3. Дочитываем остаток кадра [SESS][CMD][payload]
        if (!RecvExact(buf + 2, pktSize - 2)) {
            PacketPool::Shared().Free(buf, bucketSize);
            break;
        }

        // 4. Дешифровка CMD+payload (offset 6)
        if (_crypto && _crypto->IsActive() && pktSize > 6) {
            int encLen = pktSize - 6;

            if (!_crypto->Decrypt(buf + 6, encLen)) {
                // Ошибка дешифровки — разрыв соединения
                PacketPool::Shared().Free(buf, bucketSize);
                break;
            }

            // Обновляем размер после дешифровки
            pktSize = 6 + encLen;
            writeUInt16(buf, static_cast<uint16_t>(pktSize));
        }

        // 5. Кладём в очередь (move semantics, буфер передаётся RPacket)
        _recvQueue.Push(RPacket(buf, bucketSize, /*ownsBuffer=*/true));
    }

    // Соединение потеряно — сигнализируем game thread через _pendingDisconnect
    if (_connected) {
        _connected = false;
        _pendingDisconnect = true;

        if (_socket != INVALID_SOCKET) {
            closesocket(_socket);
            _socket = INVALID_SOCKET;
        }
    }
}

// ── RecvExact ──────────────────────────────────────────────

bool TcpClient::RecvExact(uint8_t* buf, int len) {
    int received = 0;
    while (received < len) {
        int n = recv(_socket, reinterpret_cast<char*>(buf + received), len - received, 0);
        if (n <= 0) return false; // Ошибка или закрытие
        received += n;
    }
    return true;
}

// ── SendExact ──────────────────────────────────────────────

bool TcpClient::SendExact(const uint8_t* buf, int len) {
    int sent = 0;
    while (sent < len) {
        int n = send(_socket, reinterpret_cast<const char*>(buf + sent), len - sent, 0);
        if (n <= 0) return false; // Ошибка
        sent += n;
    }
    return true;
}

} // namespace net
