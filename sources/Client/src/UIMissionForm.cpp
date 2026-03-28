#include "StdAfx.h"
#include "uimissionform.h"
#include "uiform.h"
#include "uimemo.h"
#include "PacketCmd.h"
#include "gameapp.h"

using namespace std;

using namespace GUI;
//---------------------------------------------------------------------------
// class CMissionMgr  �û����������
//---------------------------------------------------------------------------

CMissionMgr::CMissionMgr()
{
	m_pMisForm = NULL;
	m_pMisInfo = NULL;
	m_pMisClose = NULL;
	m_pMisBtn1 = NULL;
	m_pMisBtn2 = NULL;

	m_dwNpcID = -1;
	m_byMisCmd = -1;
}

CMissionMgr::~CMissionMgr()
{
}

bool CMissionMgr::Init() // �û�������Ϣ��ʼ��
{
	//npc����
	m_pMisForm = _FindForm("frmNPCMission" ); 
	if( !m_pMisForm )
	{	
		g_logManager.InternalLog(LogLevel::Debug, "common", g_oLangRec.GetString(740));
		return false;
	}
	
	m_pMisForm->evtEntrustMouseEvent = _MouseEvent;
	m_pMisInfo = dynamic_cast<CMemoEx*>(m_pMisForm->Find( "memMission" ) );
	//m_pMisInfo->evtClickItem = _ItemClickEvent;

	if( !m_pMisInfo )
	{
		{ char _buf[512]; snprintf(_buf, sizeof(_buf), g_oLangRec.GetString(45), m_pMisForm->GetName(), "memMission"); g_logManager.InternalLog(LogLevel::Error, "errors", _buf); }
		return false;
	}

	m_pMisBtn1 = dynamic_cast<CTextButton*>(m_pMisForm->Find( "btnYes" ) );
	if( !m_pMisBtn1 )
	{
		{ char _buf[512]; snprintf(_buf, sizeof(_buf), g_oLangRec.GetString(45), m_pMisForm->GetName(), "btnYes"); g_logManager.InternalLog(LogLevel::Error, "errors", _buf); }
		return false;
	}

	m_pMisBtn2 = dynamic_cast<CTextButton*>(m_pMisForm->Find( "btnComplete" ) );
	if( !m_pMisBtn2 )
	{
		{ char _buf[512]; snprintf(_buf, sizeof(_buf), g_oLangRec.GetString(45), m_pMisForm->GetName(), "btnComplete"); g_logManager.InternalLog(LogLevel::Error, "errors", _buf); }
		return false;
	}

	m_pMisClose = dynamic_cast<CTextButton*>(m_pMisForm->Find( "btnClose" ) );
	if( !m_pMisClose )
	{
		{ char _buf[512]; snprintf(_buf, sizeof(_buf), g_oLangRec.GetString(45), m_pMisForm->GetName(), "btnClose"); g_logManager.InternalLog(LogLevel::Error, "errors", _buf); }
		return false;
	}

	return true;
}

void CMissionMgr::End()
{
}

void CMissionMgr::_ItemClickEvent( string strItem )
{
	const char* pStr = strItem.c_str();
}

void CMissionMgr::_MouseEvent( CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey )
{
	string strName = pSender->GetName();
	if( stricmp( "frmNPCMission", pSender->GetForm()->GetName() ) ==  0 )
	{
		// ������˳���ť,��رոñ���
		if( strName == "btnNo" || strName == "btnClose" )
		{
			pSender->GetForm()->Close();
		}
		else if( strName == "btnYes" || strName == "btnComplete" )
		{
			BYTE bySel = 0;
			if( g_stUIMission.m_pMisInfo->IsSelPrize() )
			{
				bySel = g_stUIMission.m_pMisInfo->GetSelPrize();
				if( bySel == (BYTE)-1 )
				{
					g_pGameApp->MsgBox( g_oLangRec.GetString(741) );
					return;
				}
			}
			pSender->GetForm()->Close();
			CS_MissionPage( g_stUIMission.m_dwNpcID, g_stUIMission.m_byMisCmd, bySel );
		}
	}
}

void CMissionMgr::CloseForm()
{
	if( m_pMisForm->GetIsShow() )
		m_pMisForm->Close();
}

void CMissionMgr::ShowMissionPage( DWORD dwNpcID, BYTE byCmd, const NET_MISPAGE& page )
{	
	m_pMisBtn1->SetIsShow( false );
	m_pMisBtn2->SetIsShow( false );

	if (byCmd == ROLE_MIS_BTNACCEPT )  //���ܰ�ť
	{
		m_pMisBtn1->SetIsShow( true );		
	}
	else if (byCmd == ROLE_MIS_BTNDELIVERY )  //��ɰ�ť
	{		
		m_pMisBtn2->SetIsShow( true );
		m_pMisBtn2->SetIsEnabled( true );
		m_pMisInfo->SetIsSelect( TRUE );
	}
	else if ( byCmd == ROLE_MIS_BTNPENDING )//�ѽ��ܣ���δ���
	{
		m_pMisBtn2->SetIsShow( true );
		m_pMisBtn2->SetIsEnabled( false );
	}

	m_dwNpcID  = dwNpcID;
	m_byMisCmd = byCmd;

	m_pMisInfo->Init();
	m_pMisInfo->SetMisPage( page );
	m_pMisForm->SetIsShow(true);
}
