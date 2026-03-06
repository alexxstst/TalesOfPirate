//================================================================
// It must be permitted by Dabo.Zhang that this program is used for
// any purpose in any situation.
// Copyright (C) Dabo.Zhang 2000-2003
// All rights reserved by ZhangDabo.
// This program is written(created) by Zhang.Dabo in 2000.3
// This program is modified recently by Zhang.Dabo in 2003.7
//=================================================================
#include "CommRPC.h"
#include "RPCInfo.h"
#include "DataSocket.h"
#include <string>

_DBC_USING

static PreAllocHeapPtr<TFNCall>			__genFnCall(0,200);
static PreAllocHeapPtr<OnServeCall>		__SeveCall(0,100);
static PreAllocHeapPtr<OnAsynReturn>	__RetCall(0,100);
//================================================================================
inline void OnServeCall::Init(DataSocket * datasock,uLong sessid,RPacket &rpk)
{
	m_datasock	=datasock;
	__tca		=m_datasock->GetTcpApp();
	__rpc		=__tca->__rpc;
	m_sessid	=sessid;
	m_rpk		=rpk;
}
//--------------------------------------------------
long OnServeCall::Process()
{
	try
	{
		WPacket l_retbuf =0;

		try
		{
			l_retbuf	=__rpc->OnServeCall(m_datasock,m_rpk);
		}catch(...){}

		if(!l_retbuf.HasData()||l_retbuf.Size()<l_retbuf.GetPktLen())
		{
			l_retbuf	=__tca->GetWPacket();
		}

		m_sessid	+=SESSFLAG;									//
		l_retbuf.WriteSESS(m_sessid);

		try
		{
			long	l_retval	=__tca->_SendData(m_datasock,l_retbuf);
			if(l_retval)
			{
				__tca->_SendData(m_datasock,l_retbuf);
			}
		}catch(...){}
	}catch(...){}
	m_sessid	=0;
	m_rpk		=0;
	return 0;
}
//================================================================================
inline void OnAsynReturn::Init(DataSocket * datasock,RPacket &rpk,void *pointer)
{
	m_datasock	=datasock;
	__tca		=m_datasock->GetTcpApp();
	__rpc		=__tca->__rpc;
	m_pointer	=pointer;
	m_rpk		=rpk;
}
//--------------------------------------------------
long OnAsynReturn::Process()
{
	try
	{
		__rpc->OnAsynReturn(m_datasock,m_rpk,m_pointer);
	}catch(...){}
	m_pointer	=0;
	m_rpk		=0;
	return 0;
}
//================================================================================
uLong	RPCInfo::AddFnCall(TFNCall	*fncall){
	uLong	l_retsess =0;
	auto const l_lockFnCall = std::lock_guard{m_mtxfncall};

	l_retsess			=m_gensessid.GetSessid();
	fncall->m_sessid	=l_retsess;
	fncall->m_nextfnc	=m_fncall;
	m_fncall			=fncall;

	return l_retsess;
};
//------------------------------------------------------------------------------
bool	RPCInfo::DelFnCall(TFNCall *fncall){
	TFNCall	*l_fncall,*l_prefncall;
	l_prefncall	=0;

	{
		auto const l_lockFnCall = std::lock_guard{m_mtxfncall};
		l_fncall = m_fncall;
		while (l_fncall)
		{
			if (l_fncall == fncall)
			{
				break;
			}
			l_prefncall = l_fncall;
			l_fncall = l_fncall->m_nextfnc;
		}
		if (l_fncall)
		{
			if (!l_prefncall)
			{
				m_fncall = l_fncall->m_nextfnc;
			}
			else
			{
				l_prefncall->m_nextfnc = l_fncall->m_nextfnc;
			}
		}
	}

	if(l_fncall)
	{
		l_fncall->m_nextfnc	=0;
		return true;
	}else
	{
        return false;
	}
};
//==============================================================================
RPCMGR::RPCMGR(TcpCommApp *tca):__tca(tca){
	if(!__tca)
	{
		THROW_EXCP(excp,"");
	}
}
//==============================================================================
long	RPCMGR::SendSESS(DataSocket *datasock,WPacket &sendbuf)
{
	sendbuf.WriteSESS(SESSFLAG);
	return __tca->_SendData(datasock,sendbuf);
}
long	RPCMGR::AsynCall(DataSocket *datasock,WPacket in_parameter,void *pointer)
{
	TFNCall		*	l_fncall = {};
	long			l_retval = {};
	RPCInfo		*	l_rpcinfo = {};

	if(!datasock)return -1;
	try
	{
		l_rpcinfo		=datasock->GetRPCInfo();
	}catch(...)
	{
		printf( "AsynCall warning: datasock get rpc info failed!" );
		return -1;
	}
	if(l_rpcinfo)
	{
		try
		{
			l_fncall	=__genFnCall.Get();
			if( l_fncall )
			{
				l_fncall->m_isSync	=false;			//
				l_fncall->m_pointer	=pointer;
				l_rpcinfo->AddFnCall(l_fncall);
			}
			else
			{
				printf( "AsynCall warning: get fn call null!" );
				return -1;
			}
		}
		catch(...)
		{
			printf( "AsynCall waring: get fn call exception!" );
		}
	}else
	{
		return -1;
	}

	l_fncall->m_oldsessid	=in_parameter.BackupSESS();		//backup sessid
	in_parameter.WriteSESS(l_fncall->m_sessid);
	l_retval	=__tca->_SendData(datasock,in_parameter);
	if(l_retval)								//senddata
	{
		auto const l_lockFnCall = std::lock_guard{l_rpcinfo->m_mtxfncall};
		if(l_rpcinfo->DelFnCall(l_fncall))
		{
			l_fncall->Free();
		}
		return -5;
	}
	return 0;
}
//==============================================================================
RPacket RPCMGR::SyncCall(DataSocket *datasock,WPacket	in_parameter,uLong ulMilliseconds)
{
	TFNCall		*	l_fncall = {};
	long			l_retval = {};
	RPCInfo		*	l_rpcinfo = {};
	uLong			l_sessid = {};

	if(!datasock)return 0;
	try
	{
		l_rpcinfo	=datasock->GetRPCInfo();
	}
	catch(...)
	{
		printf( "SyncCall warning: datasock get rpc info failed!" );
		return 0;
	}

	if(l_rpcinfo)
	{
		try
		{
			l_fncall	=__genFnCall.Get();
			if( l_fncall )
			{
				l_fncall->m_isSync	=true;			//
				l_sessid	=l_rpcinfo->AddFnCall(l_fncall);
			}
			else
			{
				printf( "SyncCall warning: get fn call null!" );
				return 0;
			}
		}
		catch(...)
		{
			printf( "SyncCall waring: get fn call exception!" );
		}
	}
	else
	{
		printf( "SyncCall warning: get rpc info null!" );
		return 0;
	}

	l_fncall->m_oldsessid	=in_parameter.BackupSESS();	//backup sessid
	in_parameter.WriteSESS(l_sessid);
	l_retval	=__tca->_SendData(datasock,in_parameter);
	if(l_retval)
	{
		auto const l_lockFnCall = std::lock_guard{l_rpcinfo->m_mtxfncall};
		if(l_rpcinfo->DelFnCall(l_fncall))
		{
			l_fncall->Free();
		}
		return 0;
	}

	l_retval	=l_fncall->TimeBlock(ulMilliseconds);

	RPacket	l_retbuf =0;
	{
		auto const l_lockFnCall = std::lock_guard{l_fncall->m_mtx};
		if(l_fncall->m_sessid ==l_sessid)
		{
			l_fncall->m_sessid	=0;
			if(l_rpcinfo->DelFnCall(l_fncall))
			{
				l_retbuf		=l_fncall->m_retbuf;
			}
		}
	}

	if(l_retbuf)
	{
		l_retbuf.RestoreSESS(l_fncall->m_oldsessid);
	}
	l_fncall->Free();
	return l_retbuf;
};
//==============================================================================
bool RPCMGR::ProcessSESS(DataSocket *datasock,RPacket &recvbuf)
{
	RPCInfo * l_rpcinfo	=datasock->GetRPCInfo();
	if(!l_rpcinfo){return false;}

	uLong l_sessid	=recvbuf.ReadSESS();
	if(l_sessid	>SESSFLAG)
	{
		l_sessid -=SESSFLAG;
		l_rpcinfo->m_mtxfncall.lock();

		TFNCall		*l_fncall,*l_prefncall;
		for(l_prefncall=0,l_fncall =l_rpcinfo->m_fncall;l_fncall
			;l_prefncall=l_fncall,l_fncall=l_fncall->m_nextfnc)
		{
			if(l_fncall->m_sessid ==l_sessid)
				break;
		}
		if(l_fncall)
		{
			if(l_fncall->m_isSync)
			{
				l_fncall->m_retbuf =recvbuf;
				l_fncall->SendSignal();
				l_rpcinfo->m_mtxfncall.unlock();
			}else
			{
				if(!l_prefncall){
					l_rpcinfo->m_fncall		=l_fncall->m_nextfnc;
				}else{
					l_prefncall->m_nextfnc	=l_fncall->m_nextfnc;
				}
				l_rpcinfo->m_mtxfncall.unlock();
				l_fncall->m_nextfnc	=0;
				recvbuf.RestoreSESS(l_fncall->m_oldsessid);
				if(l_fncall->m_pointer)
				{
					class OnAsynReturn *l_onret	=__RetCall.Get();
					l_onret->Init(datasock,recvbuf,l_fncall->m_pointer);
					if(__tca->GetProcessor())
					{
						__tca->GetProcessor()->AddTask(l_onret);
					}else
					{
						try
						{
							l_onret->Process();
						}catch(...){}
						try
						{
							l_onret->Lastly();
						}catch(...){}
					}
				}
				l_fncall->Free();
			}
		}
		else
		{
			l_rpcinfo->m_mtxfncall.unlock();
		}
		return true;
	}else if(l_sessid &&(l_sessid < SESSFLAG))
	{
		class OnServeCall *l_sevecall		=__SeveCall.Get();
		l_sevecall->Init(datasock,l_sessid,recvbuf);
		if(__tca->GetProcessor())
		{
			__tca->GetProcessor()->AddTask(l_sevecall);
		}else
		{
			try
			{
				l_sevecall->Process();
			}catch(...){}
			try
			{
				l_sevecall->Lastly();
			}catch(...){}
		}
		return true;
	}else
	{
		return false;
	}
};
