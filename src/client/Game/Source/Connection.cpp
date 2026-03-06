#include "StdAfx.h"
#pragma warning(disable: 4018)
#include "Connection.h"
#include "PacketCmd.h"
#pragma warning(default: 4018)

_DBC_USING

bool Connection::Connect(dbc::cChar *hostname,dbc::uShort port,dbc::uLong timeout)
{
    LG( "connect", "Connect: %s, %d\n", hostname, port );

	if(m_datasock)
	{
		return false;
	}
	if((m_status ==CNST_CONNECTING)||(m_status ==CNST_CONNECTED) || (m_status == CNST_HANDSHAKE))
	{
		return false;
	}
	m_timeout	=max(timeout,1000);
	if(m_timeout)
	{
		m_tick	=GetTickCount();
	}else
	{
		m_tick	=0;
	}
	m_status	=CNST_CONNECTING;
	strcpy(m_hostname,hostname);
	m_port		=port;
	if(m_netif->GetProcessor())
	{
		m_netif->GetProcessor()->AddTask(this);
	}else
	{
		m_netif->GetCommunicator()->AddTask(this);
	}
	return true;
}
void Connection::Disconnect(int reason)
{
    LG( "connect", "Disconnect\n" );

	m_netif->Disconnect(m_datasock,0,reason);
}
void Connection::OnDisconnect()
{
	m_datasock	=0;
	m_status	=CNST_INVALID;
}

long Connection::Process()
{
	if(m_timeout)
	{
		LG( "connect", g_oLangRec.GetString(34), m_hostname );
		m_datasock	=m_netif->Connect(m_hostname,m_port,&m_sock);
	}else
	{
		LG( "connect", g_oLangRec.GetString(34), m_hostname );
        m_datasock	=m_netif->Connect(m_hostname,m_port);
	}
	auto const l_lock = std::lock_guard{m_mtx};
	if(m_datasock)
	{
		if(m_status.Assign(CNST_CONNECTING) ==CNST_TIMEOUT)
		{
			m_status =CNST_TIMEOUT;
			if(m_datasock)
			{
				m_datasock->Free();
				m_datasock	=0;
			}
		}
	}else if(m_status.Assign(CNST_FAILURE) ==CNST_TIMEOUT)
	{
		m_status	=CNST_TIMEOUT;
	}
	return 0;
}
void Connection::CHAPSTR(bool handshake)
{
	LONG	l_stat;
	auto const l_lock = std::lock_guard{m_mtx};
	LONG newval = handshake == true ? CNST_HANDSHAKE : CNST_CONNECTED;

	if((l_stat =m_status.Assign(newval)) ==CNST_TIMEOUT)
	{
		m_status =CNST_TIMEOUT;
		if(m_datasock)
		{
			m_datasock->Free();
			m_datasock	=0;
		}
	}else if(l_stat == CNST_FAILURE)
	{
		l_stat =CNST_FAILURE;
		if(m_datasock)
		{
			m_datasock->Free();
			m_datasock	=0;
		}
	}
	else{}

}

int  Connection::GetConnStat()
{
	if(m_status ==CNST_CONNECTING && m_timeout && (GetTickCount() -m_tick) >m_timeout)
	{
		auto const l_lock = std::lock_guard{m_mtx};
		if(m_status.Assign(CNST_TIMEOUT) ==CNST_CONNECTED)
		{
			m_status	=CNST_CONNECTED;
		}else if(m_datasock)
		{
			m_datasock->Free();
			m_datasock	=0;
		}else
		{
			closesocket(m_sock);
		}
	}

	return m_status;
}

/*
	//xuedong 2004.08.18
	cChar chAppendName[512] = {0};

	if (cbNameAppendInfo)
	{
		cChar chIp[16] = {0}, chPort[16] = {0}, chHostName[64] = {0};

		gethostname((char *)chHostName, 64);
		sprintf((char *)chIp, "%s", m_datasock->GetLocalIP());
		sprintf((char *)chPort, "%d", m_datasock->GetLocalPort());
		strcat((char *)chAppendName, chHostName);
		strcat((char *)chAppendName, ".");
		strcat((char *)chAppendName, (char *)chPort);
	}
	//end

	WPacket pk	=g_NetIF->GetWPacket();;
	pk.WriteCmd(CMD_CM_SELROLE);			//
	pk.WriteLong(m_cat);
	pk.WriteSequence(m_name, uShort(strlen(m_name) + 1));
	if (cbNameAppendInfo)
		pk.WriteSequence(chAppendName, uShort(strlen(chAppendName) + 1));
	m_nt->SendData(m_datasock,pk);
	return true;
*/
