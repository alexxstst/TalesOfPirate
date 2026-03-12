#ifndef NETIF_H
#define NETIF_H

#include "CorsairsNet.h"
#include "Connection.h"
#include <bcrypt.h>
#include <wincrypt.h>
#pragma comment(lib, "bcrypt.lib")
#pragma comment(lib, "crypt32.lib")

class CProCirculate;

// Типы совместимости (ранее из dbc:: namespace)
using uChar  = uint8_t;
using uShort = uint16_t;
using uLong  = unsigned long;
using cChar  = const char;
using LLong  = __int64;

// Константы совместимости
#ifndef DS_DISCONN
#define DS_DISCONN  (0xFFFE)
#endif

// Типы пакетов (ранее dbc::RPacket& / dbc::WPacket&)
typedef net::RPacket&  LPRPACKET;
typedef net::WPacket&  LPWPACKET;
// Глобальные имена без квалификации
using WPacket = net::WPacket;
using RPacket = net::RPacket;

struct lua_State;

extern inline int lua_HandleNetMessage(lua_State* L);

// Класс записи логов
class CLogName
{
public:
	CLogName();
	void Init();

	const char* SetLogName(DWORD dwWorlID, const char* szName);
	const char* GetLogName(DWORD dwWorlID);
	const char* GetMainLogName();

	bool IsMainCha(DWORD dwWorlID);

private:
	enum
	{
		LOG_NAME = 256,
		LOG_MAX = 1000,
	};

	DWORD _dwWorldArray[LOG_MAX];
	char  _szLogName[LOG_MAX][LOG_NAME];
	char  _szNoFind[LOG_NAME];
};

class NetIF : public net::ITcpClientHandler, public net::ICryptoProvider {
public:
	// Обработка входящего пакета (Server → Client)
	BOOL HandlePacketMessage(LPRPACKET pk);
	// Отправка пакета (Client → Server)
	void SendPacketMessage(LPWPACKET pk);

	NetIF();
	virtual ~NetIF();

	// ── ITcpClientHandler ────────────────────────────────────
	void OnPacket(net::RPacket& packet) override;
	void OnDisconnected(int reason) override;

	// ── ICryptoProvider ──────────────────────────────────────
	bool IsActive() const override;
	bool Encrypt(uint8_t* ciphertext, int ciphertext_len,
	             const uint8_t* plaintext, int& len) override;
	bool Decrypt(uint8_t* data, int& len) override;

	// ── Публичный API ────────────────────────────────────────

	bool IsConnected() { return m_connect.IsConnected(); }
	int  GetConnStat() { return m_connect.GetConnStat(); }
	std::string GetDisconnectErrText(int reason) const;

	// Создание WPacket для отправки
	net::WPacket GetWPacket() { return net::WPacket(256); }

	// Обработка входящих пакетов из очереди (вызывать в game loop)
	void PollPackets(int maxPackets = 1) { _client.PollPackets(maxPackets); }

	unsigned long GetAveragePing();
	CProCirculate* GetProCir() { return m_pCProCir; }
	void SwitchNet(bool isConnected);

	// Доступ к TcpClient (для Connection)
	net::TcpClient& GetClient() { return _client; }

	Connection       m_connect;

	struct
	{
		unsigned long m_pingid;
		unsigned long m_maxdelay, m_curdelay, m_mindelay;
		DWORD dwLatencyTime[20];

		unsigned long m_ulCurStatistic;
		unsigned long m_ulDelayTime[4];
	};
	unsigned long    m_ulPacketCount;
	long             m_framedelay;

	CProCirculate*   m_pCProCir;
	std::recursive_mutex m_mutmov;
	char             m_accounts[100];
	char             m_passwd[100];

	// RSA-AES Network encryption (BCrypt)
	BCRYPT_KEY_HANDLE hRsaPubKey;
	BYTE              cliAesKey[32];
	BCRYPT_ALG_HANDLE hAesAlg;
	BCRYPT_KEY_HANDLE hAesKey;
	bool              handshakeDone;

	bool InitAesKey();
	void CleanupCrypto();

	bool _enc;
	int  _comm_enc;
	char _key[12];
	int  _key_len;

	lua_State* g_rLvm;
	lua_State* g_sLvm;

private:
	bool EncryptAES(char* ciphertext, unsigned long ciphertext_len,
	                const char* plaintext, unsigned long& ciphersize);
	bool DecryptAES(char* ciphertext, unsigned long& len);

	net::TcpClient _client;
};

extern NetIF*    g_NetIF;
extern CLogName  g_LogName;

#endif
