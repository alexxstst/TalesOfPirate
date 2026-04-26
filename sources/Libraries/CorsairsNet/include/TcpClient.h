#pragma once

// TcpClient   TCP-    .
//
//  :
//   Client mode: Connect(host, port)   
//   Server mode: Attach(socket, peerIP, peerPort)      accept
//
// :
//   Recv Thread:  recv()       
//                  SESS|FLAG  _rpcResponseQueue (RPC-)
//                  _recvQueue ( )
//   Game Thread: PollPackets()  OnPacket callback + RPC callback dispatch
//   Game Thread: Send()    send()  
//   Game Thread: AsyncCall()   SESS  Send()     PollPackets
//
//   ICryptoProvider (  ).

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
	// 
	//  ICryptoProvider   
	// 

	struct ICryptoProvider {
		virtual ~ICryptoProvider() = default;

		//    (handshake )
		virtual bool IsActive() const = 0;

		//  . ciphertext   , ciphertext_len   .
		// plaintext   , len     plaintext,    ciphertext.
		virtual bool Encrypt(uint8_t* ciphertext, int ciphertext_len,
							 const uint8_t* plaintext, int& len) = 0;

		//   in-place. len     ciphertext,    plaintext.
		virtual bool Decrypt(uint8_t* data, int& len) = 0;
	};

	// 
	//  ITcpClientHandler  callback' 
	// 

	struct ITcpClientHandler {
		virtual ~ITcpClientHandler() = default;

		//   game thread  PollPackets
		virtual void OnPacket(RPacket& packet) = 0;

		//     ( game thread  PollPackets  Disconnect)
		virtual void OnDisconnected(int reason) = 0;
	};

	// 
	//  TcpClient   TCP-  recv 
	// 

	class TcpClient {
	public:
		TcpClient();
		~TcpClient();

		TcpClient(const TcpClient&) = delete;
		TcpClient& operator=(const TcpClient&) = delete;

		//  Lifecycle 

		// Client mode:   .  .
		bool Connect(const std::string& host, uint16_t port, uint32_t timeoutMs = 5000);

		// Server mode:     ( accept).  recv .
		bool Attach(SOCKET sock, const std::string& peerIP, uint16_t peerPort);

		// . reason: 0 = , <0 = .
		void Disconnect(int reason = 0);

		//  
		bool IsConnected() const {
			return _connected.load();
		}

		// Pending disconnect (recv thread  game thread )
		bool HasPendingDisconnect() const {
			return _pendingDisconnect.load();
		}

		//   ( game thread) 

		//  .   crypto .    send().
		bool Send(WPacket& packet);

		//   ( game thread)

		//   maxPackets   .
		// : RPC-  dispatch callback'.
		// :    handler->OnPacket().
		//    pending RPC .
		//  -   .
		//
		//   :    game thread'.
		//      _recvBatch / _rpcBatch ( ),
		// reentrant- (   OnPacket   PollPackets
		//   TcpClient,    ).
		int PollPackets(int maxPackets = 1);

		//  AsyncCall (lambda-based RPC) 

		using RpcCallback = std::function<void(RPacket& response)>;

		// Async RPC   callback.
		//   SESS  , ,  callback.
		//     SESS|FLAG   callback  PollPackets().
		//     callback   RPacket.
		bool AsyncCall(WPacket& request, uint32_t timeoutMs, RpcCallback callback);

		//   

		void SetHandler(ITcpClientHandler* handler) {
			_handler = handler;
		}

		void SetCrypto(ICryptoProvider* crypto) {
			_crypto = crypto;
		}

		// App-level pointer ( DataSocket::SetPointer/GetPointer)
		void SetPointer(void* ptr) {
			_appPtr = ptr;
		}

		void* GetPointer() const {
			return _appPtr;
		}

		//   

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
		//  
		void RecvThreadProc();

		//    len .  false  /.
		bool RecvExact(uint8_t* buf, int len);

		//   len .  false  .
		bool SendExact(const uint8_t* buf, int len);

		SOCKET _socket;
		std::atomic<bool> _connected;
		std::atomic<bool> _pendingDisconnect;
		std::thread _recvThread;

		ITcpClientHandler* _handler;
		ICryptoProvider* _crypto;
		void* _appPtr;

		//   peer
		std::string _peerIP;
		uint16_t _peerPort;

		//    
		struct PacketQueue {
			std::mutex mtx;
			std::queue<RPacket> packets;

			void Push(RPacket&& pkt) {
				std::lock_guard<std::mutex> lock(mtx);
				packets.push(std::move(pkt));
			}

			bool Pop(RPacket& out) {
				if (packets.empty()) 
					return false;
				
				std::lock_guard<std::mutex> lock(mtx);
				out = std::move(packets.front());
				packets.pop();
				return true;
			}

			//      .
			void PopAll(std::vector<RPacket>& out) {
				std::lock_guard<std::mutex> lock(mtx);
				out.reserve(out.size() + packets.size());
				while (!packets.empty()) {
					out.push_back(std::move(packets.front()));
					packets.pop();
				}
			}

			//   maxCount    .
			void PopUpTo(std::vector<RPacket>& out, int maxCount) {
				if (maxCount <= 0) {
					return;
				}
				std::lock_guard<std::mutex> lock(mtx);
				int taken = 0;
				while (taken < maxCount && !packets.empty()) {
					out.push_back(std::move(packets.front()));
					packets.pop();
					++taken;
				}
			}

			bool IsEmpty() {
				return packets.empty();
			}

			void Clear() {
				std::lock_guard<std::mutex> lock(mtx);
				while (!packets.empty()) packets.pop();
			}
		};

		PacketQueue _recvQueue; //   (SESS  FLAG)
		PacketQueue _rpcResponseQueue; // RPC- (SESS  FLAG)

		//      PollPackets,    .
		//   game thread'  PollPackets, reentrant- (. ).
		std::vector<RPacket> _recvBatch;
		std::vector<RPacket> _rpcBatch;
		static constexpr std::size_t kPollBatchReserve = 64;

		//  
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

	// 
	//  WinSock   
	// 

	//      
	bool InitWinSock();
	void CleanupWinSock();
} // namespace net
