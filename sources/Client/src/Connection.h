#ifndef CONNECTION_H
#define CONNECTION_H

#include <cstdint>
#include <atomic>
#include <thread>
#include <mutex>

class NetIF;

class Connection
{
public:
	enum { CNST_INVALID = 0, CNST_CONNECTING = 1, CNST_FAILURE = 2,
	       CNST_CONNECTED = 3, CNST_TIMEOUT = 4, CNST_HANDSHAKE = 5 };

	Connection(NetIF* netif);
	~Connection();

	void Clear() { m_status = CNST_INVALID; }

	// Асинхронный запуск подключения (возвращает сразу, подключение идёт в потоке)
	bool Connect(const char* hostname, uint16_t port, uint32_t timeout = 0);

	// Отключение
	void Disconnect(int reason);

	// Вызывается при потере соединения (из OnDisconnected)
	void OnDisconnect();

	bool IsConnected() { return (m_status == CNST_CONNECTED || m_status == CNST_HANDSHAKE); }
	int  GetConnStat();
	const char* GetPeerHost() const { return m_hostname; }

	// Переход состояния: handshake=true → CNST_HANDSHAKE, false → CNST_CONNECTED
	void CHAPSTR(bool handshake = true);

private:
	std::mutex       m_mtx;
	NetIF* const     m_netif;
	std::atomic<int> m_status;
	uint32_t         m_timeout;
	uint32_t         m_tick;
	std::thread      m_connectThread;
	char             m_hostname[129];
	uint16_t         m_port;
};

#endif // CONNECTION_H
