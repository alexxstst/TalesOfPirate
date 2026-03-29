#include "stdafx.h"
#include <iostream>
#include "SessionChat.h"
#include "GroupServerApp.h"
#include "Player.h"

static InterLockedLong	s_sessid;
static PreAllocHeapPtr<Chat_Player> s_ChatPlyrHeap(1,100);
static PreAllocHeapPtr<Chat_Session> s_ChatSessHeap(1,100);

//==========================================================
void Chat_Player::Initially()
{
	RunBiDirectItem<Chat_Player>::Initially();
	m_ply	=0;
}
void Chat_Player::Finally()
{
	m_ply	=0;
	RunBiDirectItem<Chat_Player>::Finally();
}
Chat_Session *Chat_Player::GetSession()
{
	return static_cast<Chat_Session *>(GetChain());
}
Chat_Session *Player::FindSessByID(uLong sessid)
{
	auto const lock = std::lock_guard{m_mtxChat};
	for(uLong i=0;i<m_chatarranum;i++)
	{
		if(m_chat[i]->GetSession()->GetID() ==sessid)
		{
			return m_chat[i]->GetSession();
		}
	}
	return 0;
}
//==========================================================
void Chat_Session::Initially()
{
	RunBiDirectChain<Chat_Player>::Initially();
	RunBiDirectItem<Chat_Session>::Initially();

	m_sessid	=++s_sessid;
}
void Chat_Session::Finally()
{
	m_sessid	=0;

	RunBiDirectItem<Chat_Session>::Finally();
	RunBiDirectChain<Chat_Player>::Finally();
}

long Chat_Session::AddPlayer(Player *ply)
{
	if(!ply)return 0;
	Chat_Player	*l_chatply	=s_ChatPlyrHeap.Get();
	
	l_chatply->m_ply		=ply;
	{
		auto const lock = std::lock_guard{ply->m_mtxChat};
		ply->m_chat[ply->m_chatarranum]	=l_chatply;
		ply->m_chatarranum	++;
	}

	return l_chatply->_BeginRun(this);
}
long Chat_Session::DelPlayer(Player *ply)
{
	if(!ply)return 0;
	long	l_retval	=0;

	Chat_Player	*l_chatply =0;
	ply->m_mtxChat.lock();
	for(uLong i=0;i<ply->m_chatarranum;i++)
	{
		if(!l_chatply && ply->m_chat[i]->GetSession() ==this)
		{
			l_chatply	=ply->m_chat[i];
		}
		if(l_chatply && i<ply->m_chatarranum -1)
		{
			ply->m_chat[i] =ply->m_chat[i+1];
		}
	}
	if (l_chatply)
	{
		--ply->m_chatarranum;
		ply->m_mtxChat.unlock();
		l_retval	=l_chatply->_EndRun();
		l_chatply->Free();
	}
	else 
	{
		ply->m_mtxChat.unlock();
	}
	return l_retval;
}
//==========================================================
Chat_Session *GroupServerApp::AddSession()
{
	Chat_Session *l_sess	=s_ChatSessHeap.Get();	
	if(!l_sess->_BeginRun(&m_sesslst))
	{
		l_sess->Free();
		return 0;
	}else
	{
		return l_sess;
	}
}
void GroupServerApp::DelSession(Chat_Session * sess)
{
	sess->_EndRun();
	sess->Free();
}
//Chat_Session *ChatSession_list::FindSessByID(uLong sessid)
//{
//	Chat_Session	*l_sess	=0;
//	BEGIN_GET(*this);
//	while((l_sess =GetNextItem())&&(l_sess->m_sessid !=sessid)){}
//	END_GET(*this);
//	return l_sess;
//}
