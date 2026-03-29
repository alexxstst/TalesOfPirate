#include "stdafx.h"
#include "uihelpinfoform.h"
#include "helpinfoset.h"
#include "npchelper.h"
#include "worldscene.h"
#include "gameapp.h"
#include "mapset.h"
#include "character.h"
#include "uiboxform.h"
#include "effectobj.h"

using namespace std;
using namespace GUI;

bool CHelpInfoMgr::Init()
{
	m_pFormMain = _FindForm( "frmHelpchat" );   
	if( !m_pFormMain )
		return false;

	m_pMemoContent = dynamic_cast<CMemoEx*>( m_pFormMain->Find( "memCtrl" ) );
	if( !m_pMemoContent )
		return false;
	m_pMemoContent->evtClickItem = _ItemClickEvent;
	m_pMemoContent->Refresh();

	return true;
}

void CHelpInfoMgr::ShowHelpInfo( bool show, const char* HelpTitle )
{
	if( show )
	{
		NET_MISPAGE page;
		memset( &page, 0x0, sizeof( page ) );
		const char* HelpInfo = GetHelpInfo( HelpTitle );
		if( HelpInfo )
			strncpy( page.szDesp ,GetHelpInfo( HelpTitle ), ROLE_MAXNUM_DESPSIZE - 1 );

		m_pMemoContent->Init();
		m_pMemoContent->SetMisPage( page );
		m_pMemoContent->SetIsShow( true );

		m_pFormMain->Show();
	}
	else
		m_pFormMain->Hide();
}

bool CHelpInfoMgr::IsShown()
{
	return m_pFormMain->GetIsShow();
}

void CHelpInfoMgr::_ItemClickEvent( string strItem )
{
	const char* pStr = strItem.c_str();
	const char* p = pStr;
	const char* q;
	std::string map, x, y;

	bool bmap = false;
	bool bx = false;
	bool by = false;

	int index = 0;
	int num = 0;
	q = p;
	//	while(( (*p) != '(') && index < strItem.length() )
	while(( (*p) != '(') && index < (int)strItem.length() )
	{
		num++;
		p++;
		index++;
	}
	if( (*p) == '(' )
	{
		map.insert(0, q, num );
		p++;
		index++;
		bmap = true;
	}

	num = 0;
	q = p;
	//	while(( (*p) != ',') && index < strItem.length() )
	while(( (*p) != ',') && index < (int)strItem.length() )
	{
		num++;
		p++;
		index++;
	}
	if( (*p) == ',')
	{
		x.insert(0, q, num);
		p++;
		index++;
		bx = true;
	}

	num = 0;
	q = p;
	//while(( (*p) != ')')  && index < strItem.length() )
	while(( (*p) != ')')  && index < (int)strItem.length() )
	{
		num++;
		p++;
		index++;
	}
	if( (*p) == ')')
	{
		y.insert(0, q, num);
		p++;
		index++;
		by = true;
	}

	if(bmap && bx && by && index == strItem.length())
	{
		auto wintermap = "Winter Isle Archipelago";

		//check npc is valid or not
		std::string targetmap;
		if (map == g_oLangRec.GetString(56))
		{
			targetmap = g_oLangRec.GetString(56);
		}
		else if (map == g_oLangRec.GetString(57))
		{
			targetmap = g_oLangRec.GetString(57);
		}
		else if (map == g_oLangRec.GetString(58))
		{
			targetmap = g_oLangRec.GetString(58);
		}
		else if (map == wintermap)
		{
			targetmap = wintermap;
		}
		else
		{
			int nTotalIndex = NPCHelper::I()->GetLastID() + 1;
			for(int i = 0; i < nTotalIndex ; ++ i)
			{
				NPCData * pData = GetNPCDataInfo(i);
				if( p )
				{
					const char* npc = pData->szName;

					if(map == npc)
					{
						if (strcmp(pData->szName, "jialebi") == 0)
						{
							targetmap = "Pirate\'s Base";
						}
						else
						{
							targetmap = pData->szMapName;
						}
						break;
					}
				}
			}
		}

		CWorldScene* pScene = dynamic_cast<CWorldScene*>(CGameApp::GetCurScene());
		if( !pScene ) return;
		const char* curmap = pScene->GetCurMapInfo()->szName;

		if(pScene->GetMainCha()->IsBoat())
		{
			g_stUIBox.ShowMsgBox( NULL, "No routing on sea." );
			return;
		}

		if(targetmap != curmap)
		{
			g_stUIBox.ShowMsgBox( NULL, "Not on the same map." );	
			return;
		}

		const int cx = (int)pScene->GetMainCha()->GetPos().x;
		const int cy = (int)pScene->GetMainCha()->GetPos().y;

		//check x,y is valid or not
		const int tx = std::stoi(x);
		const int ty = std::stoi(y);
		if( tx < 0 || tx > 4096 )
			return;

		if( ty < 0 || ty > 4096 )
			return;

		//if(!g_cFindPathEx.HaveTarget())
		//{
		g_cFindPathEx.Reset();
		g_cFindPathEx.ClearDestDirection();
		g_cFindPathEx.SetDestDirection(cx,cy,tx,ty);
		g_cFindPathEx.SetTarget(cx,cy,tx,ty);
		//}
		D3DXVECTOR3 target((float)tx, (float)ty, 0);
		CNavigationBar::g_cNaviBar.SetTarget((char*)"", target);
		CNavigationBar::g_cNaviBar.Show(true);
	}
}
