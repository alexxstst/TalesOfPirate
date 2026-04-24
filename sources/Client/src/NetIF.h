#ifndef NETIF_H
#define NETIF_H

#include "CorsairsNet.h"
#include "Connection.h"
#include <bcrypt.h>
#include <wincrypt.h>

class CProCirculate;

//   (  dbc:: namespace)
using uChar  = uint8_t;
using uShort = uint16_t;
using uLong  = unsigned long;
using cChar  = const char;
using LLong  = __int64;

//  
#ifndef DS_DISCONN
#define DS_DISCONN  (0xFFFE)
#endif

//   ( dbc::RPacket& / dbc::WPacket&)
typedef net::RPacket&  LPRPACKET;
typedef net::WPacket&  LPWPACKET;
//    
using WPacket = net::WPacket;
using RPacket = net::RPacket;

struct lua_State;

extern inline int lua_HandleNetMessage(lua_State* L);

class NetIF : public net::ITcpClientHandler, public net::ICryptoProvider {
public:
	//    (Server  Client)
	BOOL HandlePacketMessage(LPRPACKET pk);
	//   (Client  Server)
	void SendPacketMessage(LPWPACKET pk);

	NetIF();
	virtual ~NetIF();

	//  ITcpClientHandler 
	void OnPacket(net::RPacket& packet) override;
	void OnDisconnected(int reason) override;

	//  ICryptoProvider 
	bool IsActive() const override;
	bool Encrypt(uint8_t* ciphertext, int ciphertext_len,
	             const uint8_t* plaintext, int& len) override;
	bool Decrypt(uint8_t* data, int& len) override;

	//   API 

	bool IsConnected() { return m_connect.IsConnected(); }
	int  GetConnStat() { return m_connect.GetConnStat(); }
	std::string GetDisconnectErrText(int reason) const;

	//  WPacket  
	net::WPacket GetWPacket() { return net::WPacket(256); }

	//      (  game loop)
	void PollPackets(int maxPackets = 1) { _client.PollPackets(maxPackets); }

	unsigned long GetAveragePing();
	CProCirculate* GetProCir() { return m_pCProCir; }
	void SwitchNet(bool isConnected);

	//   TcpClient ( Connection)
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

private:
	bool EncryptAES(char* ciphertext, unsigned long ciphertext_len,
	                const char* plaintext, unsigned long& ciphersize);
	bool DecryptAES(char* ciphertext, unsigned long& len);

	net::TcpClient _client;
};

extern NetIF*    g_NetIF;

#endif
