#include "Stdafx.h"
#include "GameApp.h"

#include "Character.h"
#include "SceneObj.h"
#include "SceneItem.h"
#include "EffectObj.h"
#include "MPEditor.h"
#include "Scene.h"
#include "Track.h"
#include "PacketCmd.h"
#include "lua_platform.h"

#include "SmallMap.h"
#include "GlobalVar.h"
#include "UIGlobalVar.h"
#include "GameConfig.h"

#ifndef USE_DSOUND
#include "AudioThread.h"
extern CAudioThread g_AudioThread;
#endif

#ifdef TESTDEMO
#include "TestDemo.h"
#endif

#include "state_reading.h"

void CGameApp::_FrameMove(DWORD dwTimeParam, bool camMove)		//Vim
{
	for (int i = 0; i < MAX_ANI_CLOCK; i++)
	{
		_AniClock[i].Update();
	}
	
	if(camMove)
	{
		/*
		If camMove is set, we only force the computation of all matrices and
		skipping the rest.
		The rest causes problems, when calling _FrameMove inside the renderloop for
		multiple views.
		*/
		CCameraCtrl *pCam = GetMainCam();
		pCam->SetViewTransform();
		return;
	}

	// Modified by CLP
	CCameraCtrl *pCam = GetMainCam();
	Ninja::Camera *pCamera = GetNinjaCamera();

	MPTerrain *pTerr = GetCurScene()->GetTerrain();
	CCharacter *pCha = _pCurScene->GetMainCha();

	if(m_AddSceneObjList.size() > 0)
	{
		SAddSceneObj *pAdd = m_AddSceneObjList.front();
		m_AddSceneObjList.pop_front();
		CGameScene *pScene = g_pGameApp->GetCurScene();
		CSceneObj *pObj = pScene->AddSceneObj(pAdd->nTypeID);
		if(pObj)
		{
			pObj->setHeightOff(pAdd->nHeightOff);
			pObj->setPos(pAdd->nPosX, pAdd->nPosY);
			pObj->setYaw(pAdd->nAngle);
			{ char _buf[512]; snprintf(_buf, sizeof(_buf), g_oLangRec.GetString(64), pAdd->nTypeID, pAdd->nPosX, pAdd->nPosY); g_logManager.InternalLog(LogLevel::Debug, "common", _buf); }
		}
		delete pAdd;
	}

	// 
	if(_bCameraFollow)
	{
		if(pCha)
		{
			D3DXVECTOR3 vecCha = pCha->GetPos() + D3DXVECTOR3( 0, 0, 1.f );

			pCam->AddPoint(vecCha.x, vecCha.y, vecCha.z);
			pCam->Update();
			pCam->SetFollowObj(pCam->m_vCurPos);

			pCam->FrameMove(dwTimeParam);
			g_Render.SetWorldViewFOV(Angle2Radian(pCam->m_ffov));
		}
	}
	if(_bCameraFollow)
	{
		D3DXVECTOR3 vecCha = pCam->m_RefPos;
		const auto v = -pCam->m_vDir * pCam->_fdistshow;
		D3DXVec3Add(&vecCha,&vecCha,&(v));
		if (pTerr) { 
			pTerr->SetShowCenter(vecCha.x, vecCha.y);
		}
	}
	//!VIEW MATRIX
	pCam->SetViewTransform();

	//if( IsKeyDown( DIK_K ) )
	//{
	//	if( dynamic_cast < CReadingState* > ( pCha->GetActor()->GetCurState() ) == 0 )
	//	{
	//		CReadingState* readingState = new CReadingState( pCha->GetActor() );
	//		pCha->GetActor()->SwitchState( readingState );
	//	}
	//}

	//if( IsKeyDown( DIK_K ) )
	//{
	//	CSceneItem* item = _pCurScene->AddSceneItem( 1, 2 );
	//	item->setPos( pCha->GetCurX(), pCha->GetCurY() );
	//	pCha->AttachItem( enumEQUIP_LHAND, item, -1 );
	//}
	//test

	//if( IsKeyDown( DIK_K ) )
	//{
	//	CCharacter* entity = _pCurScene->AddCharacter( 2 );
	//	entity->linkTo( pCha, 1 );
	//}
	//test

	// Added by CLP
	//if ( pCha )	// 
	//{
	//	static bool initial = true;
	//	if ( initial )
	//	{
	//		initial = false;
	//		pCamera->SetTarget ( pCha->GetPos() );
	//	}
	//	pCamera->UpdateTargetPosition ( pCha->GetPos() );
	//	pCamera->UpdateEyePosition();

	//	// Update old camera
	//	pCam->m_EyePos = pCamera->GetEye();
	//	pCam->m_RefPos = pCamera->GetTarget();
	//	pCam->m_fAngle = pCamera->GetSphereCoord().theta;

	//	if( pTerr )
	//	{
	//		pTerr->SetShowCenter ( pCamera->GetTarget().x, pCamera->GetTarget().y );
	//	}

	//	g_Render.SetWorldViewFOV ( Angle2Radian ( 20.0f ) );

	//	pCamera->UpdateViewTransform();
	//}
	// Added by CLP


	ResMgr.FrameMove(dwTimeParam);

	//  1s 
	static DWORD tick = 0;
	if( GetCurTick() - tick > 1000 )
	{
		tick = GetCurTick();
		CS_SendPing();
	}

	CUIInterface::All_FrameMove( dwTimeParam );

	_pCurScene->_FrameMove( dwTimeParam );

#ifdef TESTDEMO
	g_pTestDemo->FrameMove();
#endif

	// 
#ifdef USE_DSOUND
//	if( m_pAudioPlayer )
//		m_pAudioPlayer->Update( TRUE );
#else
	g_AudioThread.FrameMove();
#endif

	// 
	switch( _eSwitchMusic )
	{
	case enumOldMusic:
		_nCurMusicSize--;
		if( _nCurMusicSize<=1 )
		{
			//::bkg_snd_stop();
			AudioSDL::get_instance()->stop(::g_dwCurMusicID);
			if( _szBkgMusic[0]=='\0' )
			{
				_eSwitchMusic=enumNoMusic;
			}
			else
			{
				//::bkg_snd_play( _szBkgMusic, true );
				DWORD OldMusicID = g_dwCurMusicID;
				g_dwCurMusicID = AudioSDL::get_instance()->get_resID(_szBkgMusic, TYPE_MP3);

				//AudioSDL::get_instance()->play(g_dwCurMusicID, true);

				// 
#ifndef USE_DSOUND
				g_AudioThread.play(g_dwCurMusicID, true);
#else
				if( g_dwCurMusicID && ( OldMusicID != g_dwCurMusicID ) )
					AudioSDL::get_instance()->play( g_dwCurMusicID, true );
#endif
				_eSwitchMusic=enumNewMusic;
			}
		}
		else
		{
			//::bkg_snd_vol( _nCurMusicSize );
			AudioSDL::get_instance()->volume(g_dwCurMusicID, _nCurMusicSize);
		}
		break;

	case enumNewMusic:
		_nCurMusicSize++;
		if( _nCurMusicSize>=_nMusicSize )
		{
			_eSwitchMusic=enumMusicPlay;
		}
		else
		{
			//::bkg_snd_vol( _nCurMusicSize );
			AudioSDL::get_instance()->volume(g_dwCurMusicID, _nCurMusicSize);
		}
		break;
	}

#ifdef _LUA_GAME
	lua_platform_framemove();
#endif

	HandleKeyContinue();

	if(g_Config.m_bEditor)
	{
		g_Editor.FrameMove(dwTimeParam);

		if(_pCurScene->GetTerrain())
		{
			g_Render.Print(INFO_DEBUG, 290, 120, "showcenter = %f,%f",_pCurScene->GetTerrain()->GetShowCenterX(),_pCurScene->GetTerrain()->GetShowCenterY() );
		}

		if(pCha)
		{
			g_Render.Print(INFO_DEBUG, 290, 410, "cha pos = %f,%f,%f",pCha->GetPos().x * 100,pCha->GetPos().y * 100 ,pCha->GetPos().z* 100 );
			g_Render.Print(INFO_DEBUG, 290, 430, "cha angle = %d",(int)(int(((float)pCha->getYaw() * 0.01745329f /*+ D3DX_PI*/) * 57.29577f) % 360));
		}
	}


	_stCursorMgr.FrameMove(dwTimeParam);

	if( CGameScene::_bShowMinimap && CGameScene::_pSmallMap )
	{
		CGameScene::_pSmallMap->FrameMove(dwTimeParam);
	}

	MPTexSet::I()->DynamicRelease();
}

bool CGameApp::_MouseInScene = false;
void CGameApp::_PreMouseRun( DWORD dwMouseKey )
{
	if( !GetConsole()->IsVisible() )
	{
		extern HWND g_InputEdit;
		if( GetFocus()!=g_InputEdit )
		{
			::SetFocus( g_InputEdit );
		}
	}
	else
	{
		::SetFocus( GetHWND() );
	}

	CFormMgr::s_Mgr.FrameMove( GetMouseX(), GetMouseY(), dwMouseKey, GetCurTick() );

	switch( CFormMgr::GetMouseAction() )
	{
	case enumMA_None:
		_MouseInScene = true;
		break;
	case enumMA_Gui:
	case enumMA_Skill:
		_MouseInScene = false;
		break;
	case enumMA_Drill:
		_MouseInScene = CCozeForm::GetInstance()->IsMouseOnList(GetMouseX(), GetMouseY());
		break;
	}	
}

CAniClock* CGameApp::AddAniClock()
{
    for (unsigned int i = 0; i < MAX_ANI_CLOCK; i++)
	{
        if (_AniClock[i].IsEnd())
            return &_AniClock[i];
    }

    ToLogService("common", "msgCGameScene::AddAniClock return NULL");
    return NULL;
}
