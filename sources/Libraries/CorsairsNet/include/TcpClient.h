#pragma once

// TcpClient — универсальное TCP-соединение с выделенным потоком приёма.
//
// Режимы работы:
//   Client mode: Connect(host, port) — исходящее подключение
//   Server mode: Attach(socket, peerIP, peerPort) — от готового сокета после accept
//
// Архитектура:
//   Recv Thread: блокирующий recv() → сборка кадра → дешифровка → очередь
//                Пакеты с SESS|FLAG → _rpcResponseQueue (RPC-ответы)
//                Остальные → _recvQueue (обычные пакеты)
//   Game Thread: PollPackets() → OnPacket callback + RPC callback dispatch
//   Game Thread: Send() → шифровка → send() в сокет
//   Game Thread: AsyncCall() → запись SESS → Send() → ожидание ответа в PollPackets
//
// Шифрование через ICryptoProvider (реализуется в клиенте).

#include "Packet.h"
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <atomic>
#include <mutex>
#include <queue>
#include <thread>
#include <functional>
#include <string>
#include <vector>

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

		// Вызывается при разрыве соединения (из game thread через PollPackets или Disconnect)
		virtual void OnDisconnected(int reason) = 0;
	};

	// ═══════════════════════════════════════════════════════════════
	//  TcpClient — универсальное TCP-соединение с recv потоком
	// ═══════════════════════════════════════════════════════════════

	class TcpClient {
	public:
		TcpClient();
		~TcpClient();

		TcpClient(const TcpClient&) = delete;
		TcpClient& operator=(const TcpClient&) = delete;

		// ── Lifecycle ───────────────────────────────────────────

		// Client mode: подключение к серверу. Блокирующий вызов.
		bool Connect(const std::string& host, uint16_t port, uint32_t timeoutMs = 5000);

		// Server mode: инициализация из готового сокета (после accept). Запускает recv поток.
		bool Attach(SOCKET sock, const std::string& peerIP, uint16_t peerPort);

		// Отключение. reason: 0 = нормальное, <0 = ошибка.
		void Disconnect(int reason = 0);

		// Статус соединения
		bool IsConnected() const {
			return _connected.load();
		}

		// Pending disconnect (recv thread → game thread уведомление)
		bool HasPendingDisconnect() const {
			return _pendingDisconnect.load();
		}

		// ── Отправка (из game thread) ───────────────────────────

		// Отправить пакет. Шифрует если crypto активен. Блокирует до завершения send().
		bool Send(WPacket& packet);

		// ── Приём (из game thread) ──────────────────────────────

		// Обработать до maxPackets пакетов из очереди.
		// Сначала: RPC-ответы → dispatch callback'ов.
		// Затем: обычные пакеты → handler->OnPacket().
		// Также проверяет таймауты pending RPC вызовов.
		// Возвращает кол-во обработанных обычных пакетов.
		int PollPackets(int maxPackets = 1);

		// ── AsyncCall (lambda-based RPC) ────────────────────────

		using RpcCallback = std::function<void(RPacket& response)>;

		// Async RPC вызов с callback.
		// Записывает уникальный SESS в пакет, отправляет, запоминает callback.
		// Когда придёт ответ с SESS|FLAG — вызовет callback из PollPackets().
		// При таймауте — вызовет callback с пустым RPacket.
		bool AsyncCall(WPacket& request, uint32_t timeoutMs, RpcCallback callback);

		// ── Настройка ───────────────────────────────────────────

		void SetHandler(ITcpClientHandler* handler) {
			_handler = handler;
		}

		void SetCrypto(ICryptoProvider* crypto) {
			_crypto = crypto;
		}

		// App-level pointer (аналог DataSocket::SetPointer/GetPointer)
		void SetPointer(void* ptr) {
			_appPtr = ptr;
		}

		void* GetPointer() const {
			return _appPtr;
		}

		// ── Информация ──────────────────────────────────────────

		SOCKET GetSocket() const {
			return _socket;
		}

		const std::string& GetPeerIP() const {
			return _peerIP;
		}

		uint16_t GetPeerPort() const {
			return _peerPort;
		}

	private:
		// Поток приёма
		void RecvThreadProc();

		// Блокирующее чтение ровно len байт. Возвращает false при ошибке/закрытии.
		bool RecvExact(uint8_t* buf, int len);

		// Отправка ровно len байт. Возвращает false при ошибке.
		bool SendExact(const uint8_t* buf, int len);

		SOCKET _socket;
		std::atomic<bool> _connected;
		std::atomic<bool> _pendingDisconnect;
		std::thread _recvThread;

		ITcpClientHandler* _handler;
		ICryptoProvider* _crypto;
		void* _appPtr;

		// Информация о peer
		std::string _peerIP;
		uint16_t _peerPort;

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

		PacketQueue _recvQueue; // Обычные пакеты (SESS без FLAG)
		PacketQueue _rpcResponseQueue; // RPC-ответы (SESS с FLAG)

		// Мьютекс отправки
		std::mutex _sendMtx;

		// AsyncCall state
		static constexpr uint32_t SESS_FLAG = 0x80000000;
		std::atomic<uint32_t> _sessCounter{1};

		struct PendingCall {
			uint32_t sess;
			RpcCallback callback;
			uint32_t deadline; // GetTickCount() + timeoutMs
		};

		std::mutex _callsMtx;
		std::vector<PendingCall> _pendingCalls;
	};

	// ═══════════════════════════════════════════════════════════════
	//  WinSock — глобальная инициализация
	// ═══════════════════════════════════════════════════════════════

	// Вызвать один раз при старте приложения
	bool InitWinSock();
	void CleanupWinSock();
} // namespace net
