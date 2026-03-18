#include "TcpServer.h"
#include <iostream>
#include <algorithm>

#pragma comment(lib, "ws2_32.lib")

namespace net {

	// ═══════════════════════════════════════════════════════════════
	//  TcpServer — реализация
	// ═══════════════════════════════════════════════════════════════

	TcpServer::TcpServer()
		: _listenSocket(INVALID_SOCKET), _listening(false), _maxConns(16), _handler(nullptr) {
	}

	TcpServer::~TcpServer() {
		Shutdown();
	}

	// ── Listen ─────────────────────────────────────────────────

	bool TcpServer::Listen(const std::string& ip, uint16_t port, int maxConns) {
		if (_listening) {
			std::cout << "[TcpServer] Listen: уже слушаем, вызов игнорируется" << std::endl;
			return false;
		}

		_maxConns = maxConns;

		// Создаём listen socket
		_listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (_listenSocket == INVALID_SOCKET) {
			int err = WSAGetLastError();
			std::cout << "[TcpServer] socket() ОШИБКА: WSA=" << err << std::endl;
			return false;
		}

		// SO_REUSEADDR
		int reuse = 1;
		setsockopt(_listenSocket, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(reuse));

		// Bind
		sockaddr_in addr{};
		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);

		if (ip == "0.0.0.0" || ip.empty()) {
			addr.sin_addr.s_addr = INADDR_ANY;
		} else {
			inet_pton(AF_INET, ip.c_str(), &addr.sin_addr);
		}

		if (bind(_listenSocket, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
			int err = WSAGetLastError();
			std::cout << "[TcpServer] bind() ОШИБКА: WSA=" << err << " на " << ip << ":" << port << std::endl;
			closesocket(_listenSocket);
			_listenSocket = INVALID_SOCKET;
			return false;
		}

		// Listen
		if (listen(_listenSocket, SOMAXCONN) == SOCKET_ERROR) {
			int err = WSAGetLastError();
			std::cout << "[TcpServer] listen() ОШИБКА: WSA=" << err << std::endl;
			closesocket(_listenSocket);
			_listenSocket = INVALID_SOCKET;
			return false;
		}

		_listening = true;
		_acceptThread = std::thread(&TcpServer::AcceptThreadProc, this);

		std::cout << "[TcpServer] Слушаем " << ip << ":" << port << " (maxConns=" << maxConns << ")" << std::endl;
		return true;
	}

	// ── Shutdown ───────────────────────────────────────────────

	void TcpServer::Shutdown() {
		if (!_listening.exchange(false)) {
			return;
		}

		std::cout << "[TcpServer] Shutdown..." << std::endl;

		// Закрыть listen socket → accept thread разблокируется
		if (_listenSocket != INVALID_SOCKET) {
			closesocket(_listenSocket);
			_listenSocket = INVALID_SOCKET;
		}

		if (_acceptThread.joinable()) {
			_acceptThread.join();
		}

		// Закрыть pending accepts
		{
			std::lock_guard<std::mutex> lock(_pendingMtx);
			for (auto& pa : _pendingAccepts) {
				closesocket(pa.sock);
			}
			_pendingAccepts.clear();
		}

		// Отключить всех
		for (auto& conn : _connections) {
			conn.client->Disconnect(0);
		}
		_connections.clear();

		std::cout << "[TcpServer] Shutdown завершён" << std::endl;
	}

	// ── DisconnectClient ───────────────────────────────────────

	void TcpServer::DisconnectClient(TcpClient* client, int reason) {
		for (auto it = _connections.begin(); it != _connections.end(); ++it) {
			if (it->client.get() == client) {
				client->Disconnect(reason);
				if (_handler) {
					_handler->OnDisconnected(client, reason);
				}
				_connections.erase(it);
				return;
			}
		}
	}

	// ── DisconnectAll ──────────────────────────────────────────

	void TcpServer::DisconnectAll() {
		for (auto& conn : _connections) {
			conn.client->Disconnect(0);
		}
		_connections.clear();
	}

	// ── GetConnectionCount ─────────────────────────────────────

	int TcpServer::GetConnectionCount() const {
		return static_cast<int>(_connections.size());
	}

	// ── PollAll ────────────────────────────────────────────────

	int TcpServer::PollAll(int maxPktsPerConn) {
		int totalProcessed = 0;

		// 1. Обработать pending accepts
		{
			std::vector<PendingAccept> accepts;
			{
				std::lock_guard<std::mutex> lock(_pendingMtx);
				accepts.swap(_pendingAccepts);
			}

			for (auto& pa : accepts) {
				if (_maxConns > 0 && static_cast<int>(_connections.size()) >= _maxConns) {
					std::cout << "[TcpServer] PollAll: лимит подключений ("
						<< _maxConns << "), отклоняем " << pa.peerIP << ":" << pa.peerPort << std::endl;
					closesocket(pa.sock);
					continue;
				}

				Connection conn;
				conn.client = std::make_unique<TcpClient>();
				conn.adapter = std::make_unique<ClientHandlerAdapter>();
				conn.adapter->serverHandler = _handler;
				conn.adapter->client = conn.client.get();

				// Устанавливаем handler-адаптер: PollPackets → adapter->OnPacket → serverHandler->OnPacket(client, pkt)
				conn.client->SetHandler(conn.adapter.get());

				if (!conn.client->Attach(pa.sock, pa.peerIP, pa.peerPort)) {
					std::cout << "[TcpServer] PollAll: Attach неудачен для "
						<< pa.peerIP << ":" << pa.peerPort << std::endl;
					closesocket(pa.sock);
					continue;
				}

				bool accepted = true;
				if (_handler) {
					accepted = _handler->OnAccepted(conn.client.get());
				}

				if (accepted) {
					_connections.push_back(std::move(conn));
				} else {
					conn.client->Disconnect(0);
				}
			}
		}

		// 2. Проверить disconnects + обработать пакеты
		for (auto it = _connections.begin(); it != _connections.end(); ) {
			TcpClient* client = it->client.get();

			// Проверить pending disconnect
			if (client->HasPendingDisconnect() || !client->IsConnected()) {
				if (_handler) {
					_handler->OnDisconnected(client, -1);
				}
				it = _connections.erase(it);
				continue;
			}

			// PollPackets: RPC dispatch + таймауты + обычные пакеты → adapter → serverHandler
			int processed = client->PollPackets(maxPktsPerConn);
			totalProcessed += processed;

			++it;
		}

		return totalProcessed;
	}

	// ── AcceptThreadProc ───────────────────────────────────────

	void TcpServer::AcceptThreadProc() {
		std::cout << "[TcpServer] AcceptThread: запущен" << std::endl;

		while (_listening) {
			sockaddr_in clientAddr{};
			int addrLen = sizeof(clientAddr);

			SOCKET clientSock = accept(_listenSocket, (sockaddr*)&clientAddr, &addrLen);

			if (clientSock == INVALID_SOCKET) {
				if (_listening) {
					int err = WSAGetLastError();
					std::cout << "[TcpServer] AcceptThread: accept() ОШИБКА: WSA=" << err << std::endl;
				}
				break;
			}

			char ipBuf[INET_ADDRSTRLEN];
			inet_ntop(AF_INET, &clientAddr.sin_addr, ipBuf, sizeof(ipBuf));
			uint16_t port = ntohs(clientAddr.sin_port);

			std::cout << "[TcpServer] AcceptThread: подключение от "
				<< ipBuf << ":" << port << std::endl;

			{
				std::lock_guard<std::mutex> lock(_pendingMtx);
				_pendingAccepts.push_back({ clientSock, std::string(ipBuf), port });
			}
		}

		std::cout << "[TcpServer] AcceptThread: завершён" << std::endl;
	}

} // namespace net
