#include "TcpClient.h"
#include <cassert>
#include <iostream>
#include <ostream>
#include <algorithm>

#pragma comment(lib, "ws2_32.lib")

namespace net {
	// ═══════════════════════════════════════════════════════════════
	//  WsaErrorStr — текстовое описание WSA-ошибок
	// ═══════════════════════════════════════════════════════════════

	static const char* WsaErrorStr(int err) {
		switch (err) {
		case 0: return "OK (0)";
		case WSAEINTR: return "WSA=10004 WSAEINTR (прерван блокирующий вызов)";
		case WSAEACCES: return "WSA=10013 WSAEACCES (отказано в доступе)";
		case WSAEFAULT: return "WSA=10014 WSAEFAULT (неверный адрес)";
		case WSAEINVAL: return "WSA=10022 WSAEINVAL (неверный аргумент)";
		case WSAEMFILE: return "WSA=10024 WSAEMFILE (слишком много открытых сокетов)";
		case WSAEWOULDBLOCK: return "WSA=10035 WSAEWOULDBLOCK (операция заблокирована)";
		case WSAEINPROGRESS: return "WSA=10036 WSAEINPROGRESS (операция в процессе)";
		case WSAEALREADY: return "WSA=10037 WSAEALREADY (операция уже выполняется)";
		case WSAENOTSOCK: return "WSA=10038 WSAENOTSOCK (не сокет)";
		case WSAEMSGSIZE: return "WSA=10040 WSAEMSGSIZE (сообщение слишком длинное)";
		case WSAENETDOWN: return "WSA=10050 WSAENETDOWN (сеть недоступна)";
		case WSAENETRESET: return "WSA=10052 WSAENETRESET (сеть сбросила соединение)";
		case WSAECONNABORTED: return "WSA=10053 WSAECONNABORTED (соединение прервано)";
		case WSAECONNRESET: return "WSA=10054 WSAECONNRESET (соединение сброшено удалённой стороной)";
		case WSAENOBUFS: return "WSA=10055 WSAENOBUFS (нет буферного пространства)";
		case WSAENOTCONN: return "WSA=10057 WSAENOTCONN (сокет не подключён)";
		case WSAESHUTDOWN: return "WSA=10058 WSAESHUTDOWN (сокет выключен)";
		case WSAETIMEDOUT: return "WSA=10060 WSAETIMEDOUT (таймаут соединения)";
		case WSAECONNREFUSED: return "WSA=10061 WSAECONNREFUSED (соединение отклонено)";
		case WSAEHOSTDOWN: return "WSA=10064 WSAEHOSTDOWN (хост недоступен)";
		case WSAEHOSTUNREACH: return "WSA=10065 WSAEHOSTUNREACH (нет маршрута к хосту)";
		case WSANOTINITIALISED: return "WSA=10093 WSANOTINITIALISED (WSAStartup не вызван)";
		case WSAEDISCON: return "WSA=10101 WSAEDISCON (graceful disconnect)";
		default: {
			// Для неизвестных кодов — статический буфер (не thread-safe, но для логов достаточно)
			static thread_local char buf[64];
			snprintf(buf, sizeof(buf), "WSA=%d (неизвестная ошибка)", err);
			return buf;
		}
		}
	}

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
		  _handler(nullptr), _crypto(nullptr), _appPtr(nullptr), _peerPort(0) {
	}

	TcpClient::~TcpClient() {
		Disconnect(0);
	}

	// ── Connect (client mode) ─────────────────────────────────

	bool TcpClient::Connect(const std::string& host, uint16_t port, uint32_t timeoutMs) {
		if (_connected) {
			std::cout << "[TcpClient] Connect: уже подключён, повторный вызов игнорируется" << std::endl;
			return false;
		}

		std::cout << "[TcpClient] Подключение к " << host << ":" << port << " (timeout=" << timeoutMs << "ms)..." <<
			std::endl;

		// Резолв адреса
		addrinfo hints{};
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;

		char portStr[8];
		snprintf(portStr, sizeof(portStr), "%u", port);

		addrinfo* result = nullptr;
		int gaiErr = getaddrinfo(host.c_str(), portStr, &hints, &result);
		if (gaiErr != 0) {
			std::cout << "[TcpClient] getaddrinfo ОШИБКА: " << gaiErr << " для " << host << ":" << port << std::endl;
			return false;
		}

		// Создание сокета
		_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
		if (_socket == INVALID_SOCKET) {
			int err = WSAGetLastError();
			std::cout << "[TcpClient] socket() ОШИБКА: " << WsaErrorStr(err) << std::endl;
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
				std::cout << "[TcpClient] connect() ОШИБКА: " << WsaErrorStr(err) << std::endl;
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
				if (selResult == 0) {
					std::cout << "[TcpClient] connect() ТАЙМАУТ (" << timeoutMs << "ms) к " << host << ":" << port <<
						std::endl;
				}
				else if (selResult < 0) {
					std::cout << "[TcpClient] select() ОШИБКА: " << WsaErrorStr(WSAGetLastError()) << std::endl;
				}
				else {
					int sockErr = 0;
					int optLen = sizeof(sockErr);
					getsockopt(_socket, SOL_SOCKET, SO_ERROR, (char*)&sockErr, &optLen);
					std::cout << "[TcpClient] connect() ОТКЛОНЁН к " << host << ":" << port << ": " <<
						WsaErrorStr(sockErr) << std::endl;
				}
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

		_peerIP = host;
		_peerPort = port;
		_connected = true;
		_pendingDisconnect = false;

		// Запуск recv потока
		_recvThread = std::thread(&TcpClient::RecvThreadProc, this);

		std::cout << "[TcpClient] Подключён к " << host << ":" << port << " (socket=" << _socket << ")" << std::endl;
		return true;
	}

	// ── Attach (server mode) ──────────────────────────────────

	bool TcpClient::Attach(SOCKET sock, const std::string& peerIP, uint16_t peerPort) {
		if (_connected) {
			std::cout << "[TcpClient] Attach: уже подключён, вызов игнорируется" << std::endl;
			return false;
		}

		_socket = sock;
		_peerIP = peerIP;
		_peerPort = peerPort;

		// Отключаем Nagle
		int noDelay = 1;
		setsockopt(_socket, IPPROTO_TCP, TCP_NODELAY, (const char*)&noDelay, sizeof(noDelay));

		_connected = true;
		_pendingDisconnect = false;

		// Запуск recv потока
		_recvThread = std::thread(&TcpClient::RecvThreadProc, this);

		std::cout << "[TcpClient] Attached от " << peerIP << ":" << peerPort << " (socket=" << _socket << ")" <<
			std::endl;
		return true;
	}

	// ── Disconnect ─────────────────────────────────────────────

	void TcpClient::Disconnect(int reason) {
		bool expected = true;
		if (!_connected.compare_exchange_strong(expected, false)) {
			return; // Уже отключены
		}

		std::cout << "[TcpClient] Disconnect: reason=" << reason
			<< " peer=" << _peerIP << ":" << _peerPort
			<< " (socket=" << _socket << ")" << std::endl;

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

		// Очищаем очереди
		_recvQueue.Clear();
		_rpcResponseQueue.Clear();

		// Отменяем все pending RPC с пустым ответом
		{
			std::lock_guard<std::mutex> lock(_callsMtx);
			for (auto& call : _pendingCalls) {
				RPacket empty;
				call.callback(empty);
			}
			_pendingCalls.clear();
		}

		// Уведомляем handler
		if (_handler) {
			_handler->OnDisconnected(reason);
		}
	}

	// ── Send ───────────────────────────────────────────────────

	bool TcpClient::Send(WPacket& packet) {
		if (!_connected) {
			std::cout << "[TcpClient] Send: не подключён, пакет отброшен" << std::endl;
			return false;
		}

		std::cout << "[TcpClient] " << packet.PrintCommand() << std::endl;

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
				std::cout << "[TcpClient] Send: ошибка шифрования, пакет отброшен" << std::endl;
				return false;
			}

			int newSize = 6 + encLen;
			encrypted.SetPacketSize(newSize);

			return SendExact(encrypted.Data(), newSize);
		}

		return SendExact(packet.Data(), pktSize);
	}

	// ── AsyncCall ──────────────────────────────────────────────

	bool TcpClient::AsyncCall(WPacket& request, uint32_t timeoutMs, RpcCallback callback) {
		if (!_connected) {
			std::cout << "[TcpClient] AsyncCall: не подключён" << std::endl;
			return false;
		}

		// Генерируем уникальный SESS (1..0x7FFFFFFE)
		uint32_t sess = _sessCounter.fetch_add(1);
		if (sess >= 0x7FFFFFFF) {
			_sessCounter.store(1);
			sess = _sessCounter.fetch_add(1);
		}

		// Записываем SESS в пакет
		request.WriteSess(sess);

		// Запоминаем callback
		{
			std::lock_guard<std::mutex> lock(_callsMtx);
			PendingCall pc;
			pc.sess = sess;
			pc.callback = std::move(callback);
			pc.deadline = GetTickCount() + timeoutMs;
			_pendingCalls.push_back(std::move(pc));
		}

		// Отправляем
		if (!Send(request)) {
			// Откатываем pending call
			std::lock_guard<std::mutex> lock(_callsMtx);
			_pendingCalls.erase(
				std::remove_if(_pendingCalls.begin(), _pendingCalls.end(),
							   [sess](const PendingCall& c) {
								   return c.sess == sess;
							   }),
				_pendingCalls.end());
			return false;
		}

		return true;
	}

	// ── PollPackets ────────────────────────────────────────────

	int TcpClient::PollPackets(int maxPackets) {
		// 1. Обработать RPC-ответы (dispatch callbacks)
		RPacket rpcPkt;
		while (_rpcResponseQueue.Pop(rpcPkt)) {
			uint32_t sess = rpcPkt.GetSess() & ~SESS_FLAG;
			std::lock_guard<std::mutex> lock(_callsMtx);
			auto it = std::find_if(_pendingCalls.begin(), _pendingCalls.end(),
								   [sess](const PendingCall& c) {
									   return c.sess == sess;
								   });
			if (it != _pendingCalls.end()) {
				auto cb = std::move(it->callback);
				_pendingCalls.erase(it);
				cb(rpcPkt);
			}
		}

		// 2. Проверить таймауты pending RPC
		{
			std::lock_guard<std::mutex> lock(_callsMtx);
			uint32_t now = GetTickCount();
			for (auto it = _pendingCalls.begin(); it != _pendingCalls.end();) {
				if (now >= it->deadline) {
					auto cb = std::move(it->callback);
					it = _pendingCalls.erase(it);
					RPacket empty;
					cb(empty);
				}
				else {
					++it;
				}
			}
		}

		// 3. Обработать обычные пакеты
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
			_rpcResponseQueue.Clear();
			if (_recvThread.joinable()) {
				_recvThread.join();
			}

			// Отменяем все pending RPC
			{
				std::lock_guard<std::mutex> lock(_callsMtx);
				for (auto& call : _pendingCalls) {
					RPacket empty;
					call.callback(empty);
				}
				_pendingCalls.clear();
			}

			_handler->OnDisconnected(-1);
		}

		return processed;
	}

	// ── RecvThreadProc ─────────────────────────────────────────

	void TcpClient::RecvThreadProc() {
		// Сброс системных ошибок, чтобы WSAGetLastError/GetLastError возвращали актуальные значения
		WSASetLastError(0);
		SetLastError(0);
		std::cout << "[TcpClient] RecvThread: запущен (socket=" << _socket
			<< " peer=" << _peerIP << ":" << _peerPort << ")" << std::endl;
		uint8_t sizeHeader[2];

		while (_connected) {
			// 1. Читаем 2 байта размера пакета
			if (!RecvExact(sizeHeader, 2)) {
				std::cout << "[TcpClient] RecvThread: выход — ошибка чтения заголовка размера" << std::endl;
				break;
			}

			int pktSize = static_cast<int>(readUInt16(sizeHeader));
			if (pktSize == 2) {
				std::cout << "[TcpClient] RecvThread: пришел пинг: " << pktSize << std::endl;
				continue;
			}

			if (pktSize < 8 || pktSize > 65535) {
				std::cout << "[TcpClient] RecvThread: невалидный размер кадра: " << pktSize << std::endl;
				break;
			}

			// 2. Аллоцируем буфер из пула
			int bucketSize = PacketPool::Shared().BucketSize(pktSize);
			uint8_t* buf = PacketPool::Shared().Allocate(pktSize);

			// Копируем size header
			writeUInt16(buf, static_cast<uint16_t>(pktSize));

			// 3. Дочитываем остаток кадра [SESS][CMD][payload]
			if (!RecvExact(buf + 2, pktSize - 2)) {
				std::cout << "[TcpClient] RecvThread: выход — ошибка чтения тела пакета (size=" << pktSize << ")" <<
					std::endl;
				PacketPool::Shared().Free(buf, bucketSize);
				break;
			}

			// 4. Дешифровка CMD+payload (offset 6)
			if (_crypto && _crypto->IsActive() && pktSize > 6) {
				int encLen = pktSize - 6;

				if (!_crypto->Decrypt(buf + 6, encLen)) {
					std::cout << "[TcpClient] RecvThread: ошибка дешифровки (size=" << pktSize << "), разрыв" <<
						std::endl;
					PacketPool::Shared().Free(buf, bucketSize);
					break;
				}

				// Обновляем размер после дешифровки
				pktSize = 6 + encLen;
				writeUInt16(buf, static_cast<uint16_t>(pktSize));
			}

			// 5. Маршрутизация по SESS: FLAG → RPC-ответ, иначе → обычный пакет
			uint32_t sess = readUInt32(buf + 2);

			{
				RPacket tmp(buf, pktSize, /*ownsBuffer=*/false);
				std::cout << "[TcpClient] " << tmp.PrintCommand() << std::endl;
			}

			if (sess & SESS_FLAG) {
				_rpcResponseQueue.Push(RPacket(buf, bucketSize, /*ownsBuffer=*/true));
			}
			else {
				_recvQueue.Push(RPacket(buf, bucketSize, /*ownsBuffer=*/true));
			}
		}

		// Соединение потеряно — сигнализируем game thread через _pendingDisconnect
		if (_connected) {
			std::cout << "[TcpClient] RecvThread: соединение потеряно, ставим pendingDisconnect" << std::endl;
			_connected = false;
			_pendingDisconnect = true;

			if (_socket != INVALID_SOCKET) {
				closesocket(_socket);
				_socket = INVALID_SOCKET;
			}
		}
		else {
			std::cout << "[TcpClient] RecvThread: завершён (Disconnect вызван извне)" << std::endl;
		}
	}

	// ── RecvExact ──────────────────────────────────────────────

	bool TcpClient::RecvExact(uint8_t* buf, int len) {
		int received = 0;
		while (received < len) {
			int n = recv(_socket, reinterpret_cast<char*>(buf + received), len - received, 0);
			if (n <= 0) {
				int err = WSAGetLastError();
				if (n == 0) {
					std::cout << "[TcpClient] RecvExact: соединение закрыто удалённой стороной" << std::endl;
				}
				else {
					std::string error = WsaErrorStr(err);
					std::cout << "[TcpClient] RecvExact: recv() = " << n << ", " << error << std::endl;
				}
				return false;
			}
			received += n;
		}
		return true;
	}

	// ── SendExact ──────────────────────────────────────────────

	bool TcpClient::SendExact(const uint8_t* buf, int len) {
		int sent = 0;
		while (sent < len) {
			int n = send(_socket, reinterpret_cast<const char*>(buf + sent), len - sent, 0);
			if (n <= 0) {
				int err = WSAGetLastError();
				std::cout << "[TcpClient] SendExact: send() = " << n << ", " << WsaErrorStr(err) << std::endl;
				return false;
			}
			sent += n;
		}
		return true;
	}
} // namespace net
