#include "DataSocket.h"
#include "PacketQueue.h"
#include <logutil.h>

_DBC_USING

void LimitCurrentProcTest()
{
    HANDLE hCurrentProcess = GetCurrentProcess();
    
    // Get the processor affinity mask for this process
    DWORD_PTR dwProcessAffinityMask = 0;
    DWORD_PTR dwSystemAffinityMask = 0;
    
    if( GetProcessAffinityMask( hCurrentProcess, &dwProcessAffinityMask, &dwSystemAffinityMask ) != 0 && dwProcessAffinityMask )
    {
        // Find the lowest processor that our process is allows to run against
        DWORD_PTR dwAffinityMask = ( dwProcessAffinityMask & ((~dwProcessAffinityMask) + 1 ) );

        // Set this as the processor that our thread must always run against
        // This must be a subset of the process affinity mask
        HANDLE hCurrentThread = GetCurrentThread();
        if( INVALID_HANDLE_VALUE != hCurrentThread )
        {
            SetThreadAffinityMask( hCurrentThread, dwAffinityMask );
            CloseHandle( hCurrentThread );
        }
    }

    CloseHandle( hCurrentProcess );
}

PKQueue::PKQueue(bool mode/* =true*/):m_head(0),m_tail(0),m_isclose(false),m_mode(mode),m_pktotal(0)
{
	//LimitCurrentProcTest();
	if(!m_mode){m_sem.Create(0,0x7FFFFFFF);}
	PKItem	*l_it	=m_heap.Get();
	l_it->Free();	
}

WPacket	PKQueue::SyncPK(DataSocket *datasock,RPacket &in_para,uLong ulMilliseconds)
{
	uLong	l_tick	=GetTickCount()	-in_para.GetTickCount();
	if(ulMilliseconds>l_tick)
	{
		ulMilliseconds =ulMilliseconds -l_tick;

		PKItem	* l_item;
		{
			auto const l = std::lock_guard{m_mut};
			if (m_isclose)
			{
				return 0;
			}
			l_item = m_heap.Get();
			if (m_tail)
			{
				m_tail->m_next = l_item;
				m_tail = l_item;
			}
			else
			{
				m_head = m_tail = l_item;
			}
			l_item->m_next = 0;
			l_item->m_datasock = datasock;
			l_item->m_inpk = in_para;
			l_item->m_iscall = 1;
			++m_pktotal;
			l_item->m_semWait.trylock();
			if (!m_mode)
			{
				m_sem.unlock();
			}
		}
		DWORD	l_sem	=l_item->m_semWait.timelock(ulMilliseconds);
		m_mut.lock();
		if((l_sem ==WAIT_OBJECT_0) || (l_item->m_semWait.trylock() ==WAIT_OBJECT_0))//(l_item->m_iscall ==2)
		{
			if(l_item->m_iscall !=2)
			{
				ToLogService("network", "PKQueue::SyncPK:[volatile char m_iscall]");
			}
			m_mut.unlock();
			WPacket	l_retpk	=l_item->m_retpk;
			if(l_retpk.HasData()==0)
			{
				ToLogService("network", "на PKQueue::SyncPK");
			}
			l_item->Free();
			return l_retpk;
		}else
		{
			l_item->m_iscall	=2;
			m_mut.unlock();
			ToLogService("network", "PKQueue::SyncPK :{}ms", ulMilliseconds);
			return 0;
		}
	}else
	{
		ToLogService("network", "PKQueue::SyncPK ():{}ms", l_tick);
		return 0;
	}
}
void	PKQueue::AddPK(DataSocket * datasock,RPacket &pk)
{
	auto const l = std::lock_guard{m_mut};
	if(m_isclose)
	{
		return;
	}
	PKItem	* l_item	=m_heap.Get();
	if(m_tail)
	{
		m_tail->m_next	=l_item;
		m_tail			=l_item;
	}else
	{
		m_head =m_tail =l_item;
	}
	l_item->m_next		=0;
	l_item->m_datasock	=datasock;
	l_item->m_inpk		=pk;
	l_item->m_iscall	=0;
	l_item->m_dwLastTime = GetTickCount();
	++m_pktotal;
	if(!m_mode)m_sem.unlock();
}
PKItem * PKQueue::GetPKItem(uLong end)
{
	PKItem		*l_item	=0;
	DWORD	l_ret =GetTickCount();
	if(m_mode || ((end >l_ret)?((l_ret =m_sem.timelock(end -l_ret)) ==WAIT_OBJECT_0):((l_ret =m_sem.trylock()) ==WAIT_OBJECT_0)))
	{
		auto const l = std::lock_guard{m_mut};
		if(!m_isclose && m_head)
		{
			//if( GetTickCount() - m_head->m_dwLastTime < 500 )
			//{
			//	m_sem.unlock();
			//	return NULL;
			//}

			--m_pktotal;
			l_item	=m_head;
			if(m_head ==m_tail)
			{
				m_head	=m_tail =0;
			}else
			{
				m_head	=m_head->m_next;
			}
		}
	} 
	return l_item;
}
void PKQueue::PeekPacket(uLong sleep)
{
	if(m_isclose)return;
	uLong l_tick	=GetTickCount() +sleep;
	for(PKItem * l_item =GetPKItem(l_tick);l_item;l_item =GetPKItem(l_tick))
	{
		switch(l_item->m_iscall)
		{
		case 0:
			{
#ifdef _SERVER
				try
				{
					ProcessData(l_item->m_datasock,l_item->m_inpk);
				}catch(...)
				{
					l_item->Free();
					throw;
				}
#else
				ProcessData(l_item->m_datasock,l_item->m_inpk);
#endif
				l_item->Free();
				break;
			}
		case 1:
			{
				try
				{
					l_item->m_retpk	=ServeCall(l_item->m_datasock,l_item->m_inpk);
				}catch(...)
				{
					ToLogService("network", "PKQueue::PeekPacket");
					auto const l = std::lock_guard{m_mut};
					if(l_item->m_iscall	==1)
					{
						l_item->m_retpk		=0;
						l_item->m_iscall	=2;
						l_item->m_semWait.unlock();
					}else
					{
						l_item->Free();
					}
					throw;
				}
				auto const l = std::lock_guard{m_mut};
				if(l_item->m_iscall	==1)
				{
					l_item->m_iscall	=2;
					l_item->m_semWait.unlock();
				}else
				{
					l_item->Free();
				}
				break;
			}
		default:
			{
				l_item->Free();
				break;
			}
		}
	}
}
void PKQueue::CloseQueue()
{
	auto const l = std::lock_guard{m_mut};
	m_isclose	=true;
	for(PKItem * l_item =GetPKItem(0);l_item;l_item =GetPKItem(0))
	{
		if(l_item->m_iscall)
		{
			l_item->m_semWait.unlock();
		}
		l_item->Free();
	}
}
