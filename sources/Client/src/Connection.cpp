#include "StdAfx.h"
#include "Connection.h"
#include "NetIF.h"

Connection::Connection(NetIF* netif)
	: m_netif(netif), m_status(CNST_INVALID),
	  m_timeout(0), m_tick(0), m_port(0)
{
	memset(m_hostname, 0, sizeof(m_hostname));
}

Connection::~Connection()
{
	if (m_connectThread.joinable())
		m_connectThread.join();
}

bool Connection::Connect(const char* hostname, uint16_t port, uint32_t timeout)
{
	ToLogService("connections", "Connect: {}, {}", hostname, port);

	if (m_status == CNST_CONNECTING || m_status == CNST_CONNECTED || m_status == CNST_HANDSHAKE)
		return false;

	m_timeout = max(timeout, (uint32_t)1000);
	m_tick = GetTickCount();
	m_status = CNST_CONNECTING;

	strncpy(m_hostname, hostname, sizeof(m_hostname) - 1);
	m_hostname[sizeof(m_hostname) - 1] = '\0';
	m_port = port;

	// Ждём завершения предыдущего потока подключения
	if (m_connectThread.joinable())
		m_connectThread.join();

	// Подключение в отдельном потоке (TcpClient::Connect блокирующий)
	m_connectThread = std::thread([this]() {
		bool ok = m_netif->GetClient().Connect(m_hostname, m_port, m_timeout);

		std::lock_guard<std::mutex> lock(m_mtx);
		if (ok) {
			// TCP подключён. Статус остаётся CNST_CONNECTING
			// до вызова CHAPSTR() из SC_SendPublicKey/CS_SendPrivateKey.
			int cur = m_status.load();
			if (cur == CNST_TIMEOUT) {
				// Таймаут уже сработал — закрываем только что открытое соединение
				m_netif->GetClient().Disconnect(0);
			}
		} else {
			int expected = CNST_CONNECTING;
			if (!m_status.compare_exchange_strong(expected, CNST_FAILURE)) {
				// Статус уже изменился (например, CNST_TIMEOUT)
			}
		}
	});

	return true;
}

void Connection::Disconnect(int reason)
{
	ToLogService("connections", "Disconnect");
	m_netif->GetClient().Disconnect(reason);
}

void Connection::OnDisconnect()
{
	m_status = CNST_INVALID;
}

void Connection::CHAPSTR(bool handshake)
{
	std::lock_guard<std::mutex> lock(m_mtx);
	int newval = handshake ? CNST_HANDSHAKE : CNST_CONNECTED;
	int old = m_status.exchange(newval);

	if (old == CNST_TIMEOUT || old == CNST_FAILURE) {
		m_status = old;
		m_netif->GetClient().Disconnect(0);
	}
}

int Connection::GetConnStat()
{
	if (m_status == CNST_CONNECTING && m_timeout &&
	    (GetTickCount() - m_tick) > m_timeout)
	{
		std::lock_guard<std::mutex> lock(m_mtx);
		int expected = CNST_CONNECTING;
		if (m_status.compare_exchange_strong(expected, CNST_TIMEOUT)) {
			// Таймаут — поток подключения либо ещё работает, либо уже завершился.
			// Если соединение установлено, оно будет закрыто в Process().
		} else if (expected == CNST_CONNECTED || expected == CNST_HANDSHAKE) {
			// Подключились как раз вовремя
			m_status = expected;
		}
	}

	return m_status;
}
