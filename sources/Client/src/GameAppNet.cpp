#include "Stdafx.h"
#include "GameApp.h"
#include "Character.h"
#include "Scene.h"
#include "GameConfig.h"
#include "GameAppMsg.h"
#include "packetcmd.h"
#include "MPEditor.h"
#include "MapRecordStore.h"
#include "NetProtocol.h"
#include "worldscene.h"
#include "LoginScene.h"
#include "UIChat.h"
#include "UITeam.h"
#include "uiboxform.h"
#include "cameractrl.h"
#include "uisystemform.h"

static void	_Disconnect(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
{
    CLoginScene* pLogin = dynamic_cast<CLoginScene*>(CGameApp::GetCurScene());
    if( pLogin )
    {
        pLogin->ShowRegionList();
		//pLogin->ShowLoginForm();
    }
	else
	{
		g_pGameApp->LoadScriptScene( enumLoginScene );

		CLoginScene* pLogin = dynamic_cast<CLoginScene*>(CGameApp::GetCurScene());
		if( pLogin )
		{
			pLogin->ShowRegionList();
			//pLogin->ShowLoginForm();
		}
	}
}

static void	_SwitchMapFailed(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
{
    CLoginScene* pLogin = dynamic_cast<CLoginScene*>(CGameApp::GetCurScene());
    if( !pLogin )
	{
		g_pGameApp->LoadScriptScene( enumLoginScene );
		pLogin = dynamic_cast<CLoginScene*>(CGameApp::GetCurScene());
	}

	if( pLogin )
	{
		if( g_NetIF->IsConnected() )
			pLogin->ShowChaList();
		else
			pLogin->ShowRegionList();
			//pLogin->ShowLoginForm();
	}
}

void CGameApp::_HandleMsg(DWORD dwTypeID, DWORD dwParam1, DWORD dwParam2) 
{
	switch( dwTypeID )
	{
		case APP_NET_LOGINRET:
			break;

        case APP_NET_DISCONNECT:
			{
                Waiting( false );

				CGameScene* scene = CGameApp::GetCurScene();
				if( !scene ) return;

				if( dwParam1==1000 )
				{
					char szBuf[128] = { 0 };
					sprintf( szBuf, g_oLangRec.GetString(135), WSAGetLastError() );
					g_stUIBox.ShowMsgBox( _Disconnect, szBuf );
					return;
				}
				// else if(dwParam1!=DS_DISCONN && dwParam1!=DS_SHUTDOWN )
				{
					if( !dynamic_cast<CLoginScene*>(scene) )
					{
						if( !g_ChaExitOnTime.TimeArrived() )
						{
							//
							char szBuf[256] = { 0 };
							if( g_NetIF )
							{
								// modify by Philip.Wu  2006-06-09  
								if(g_Config.m_IsBill)
								{
									sprintf( szBuf, g_oLangRec.GetString(136), g_oLangRec.GetString(137), dwParam1 );
								}
								else
								{

									const auto reason = g_NetIF->GetDisconnectErrText(dwParam1);
									sprintf(szBuf, reason.c_str());
								}
							}
							else
							{
								sprintf( szBuf, g_oLangRec.GetString(139), dwParam1 );
							}

							g_stUIBox.ShowMsgBox( _Disconnect, szBuf );

							extern bool g_HaveGameMender;
							if( g_HaveGameMender )
							{
								g_pGameApp->MsgBox( g_oLangRec.GetString(140) );
							}
						}
					}
					else
					{	//
						//
						CLoginScene *pkLogin = dynamic_cast<CLoginScene*>(scene);
						if (pkLogin && pkLogin->IsPasswordError())
						{
							pkLogin->ShowLoginForm();
							return;
						}
					}
				}
			}
			break;
		case APP_SWITCH_MAP_FAILED:
			{
				g_pGameApp->Waiting(false);

				char szBuf[256] = { 0 };
				sprintf( szBuf, "%s[%d]", g_GetServerError(dwParam1), dwParam1 );
				g_stUIBox.ShowMsgBox( _SwitchMapFailed, szBuf );
			}
			break;
		case APP_SWITCH_MAP:
			{
			    extern MPEditor g_Editor;
                g_Editor.Init(dwParam1);
                CMapInfo *pInfo = GetMapInfo(dwParam1);
                CCameraCtrl *pCam = g_pGameApp->GetMainCam();
				auto v = D3DXVECTOR3((float)pInfo->nInitX, (float)pInfo->nInitY, 0);
				pCam->SetFollowObj(v);
                // EnableCameraFollow(FALSE);
                break;
			}
			break;
	}
}
