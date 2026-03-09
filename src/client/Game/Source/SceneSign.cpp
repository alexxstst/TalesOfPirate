#include "stdafx.h"
#include "scenesign.h"
#include "SceneItem.h"
#include "Scene.h"
#include "GameApp.h"
#include "Character.h"
#include "EffectObj.h"

CSceneSign::CSceneSign(void)
 : _pAttack(NULL), _pMove(NULL), _pScene(NULL), _eStyle(enumNone)
{
}

CSceneSign::~CSceneSign(void)
{
}

bool CSceneSign::Init( CGameScene* pScene )
{
	_pScene = pScene;
	_eStyle = enumNone;
	_pCha = NULL;

	_pAttack = _pScene->AddSceneItem( "targeta.lgo" );
	if( _pAttack ) 
	{
		_pAttack->setIsSystem( true );
		_pAttack->PlayObjImpPose( ANIM_CTRL_TYPE_MAT, 0, PLAY_LOOP, 0.0f, 2.0f );
	}
    else
    {
        ToLogService("init", "{}", g_oLangRec.GetString(377));
        return false;
    }

    _pAttackShade = _pScene->GetFirstInvalidShadeObj();
    if( _pAttackShade )
    {
        _pAttackShade->Create(2);
		D3DXVECTOR3 d( 0, 0, 0 );
        _pAttackShade->Emission( 0, &d, NULL);    
        _pAttackShade->SetValid(TRUE);
        _pAttackShade->SetHide(TRUE);
	}
    else
    {
        ToLogService("init", "{}", g_oLangRec.GetString(378));
        return false;
    }

    _pMove = _pScene->AddSceneItem( "target.lgo" );
    if( _pMove ) 
    {
        _pMove->setIsSystem( true );
        _pMove->PlayObjImpPose( ANIM_CTRL_TYPE_MAT, 0, PLAY_LOOP, 0.0f, 2.0f );
    }
    else
    {
        ToLogService("init", "{}", g_oLangRec.GetString(379));
        return false;
    }
	return true;
}

void CSceneSign::Show( CCharacter* pCha )
{
	if( pCha )
	{
		Reset( _pMove );

		_pCha = pCha;
		_eStyle = enumAttack;
		if( _pAttack )
		{
			_pAttackShade->SetHide( FALSE );
			D3DXVECTOR3 d;
			d.x = (float)_pCha->GetCurX() / 100.0f;
			d.y = (float)_pCha->GetCurY() / 100.0f;
			d.z = _pScene->GetGridHeight( d.x, d.y );
			_pAttackShade->MoveTo( &d );		

			_pAttack->setHeightOff( (int)(_pCha->GetDefaultChaInfo()->fHeight * 100.0f) );
			_pAttack->setPos( _pCha->GetCurX(), _pCha->GetCurY() );
			_pAttack->SetHide( FALSE );
			_pAttack->FrameMove(CGameApp::GetCurTick());
		}
	}
	else
	{
		Hide();
	}
}

void CSceneSign::Show( int x, int y, int h )
{
	_eStyle = enumMove;
	_nX = x;
	nY = y;
	_nH = h;

	Reset( _pAttack );

	if( _pMove )
	{
        _pMove->setHeightOff( h );
        _pMove->setPos( x, y );
        _pMove->SetHide( FALSE );
        _pMove->FrameMove(CGameApp::GetCurTick());
	}
}

void CSceneSign::Reset( CSceneItem* pItem )
{
	if( pItem && !pItem->IsHide() )
	{
		if( pItem==_pAttack ) 
		{
			_pAttackShade->SetHide( TRUE );
		}

		pItem->SetHide( TRUE );
		pItem->FrameMove(CGameApp::GetCurTick());
	}
}

void CSceneSign::Hide()
{
	_eStyle = enumNone;

	Reset( _pMove );
	Reset( _pAttack );
}

void CSceneSign::FrameMove( DWORD dwTime )
{
	if( _pAttack && _eStyle==enumAttack )
	{
		_pAttack->setPos( _pCha->GetCurX(), _pCha->GetCurY() );		

		static D3DXVECTOR3 d;
		d.x = (float)_pCha->GetCurX() / 100.0f;
		d.y = (float)_pCha->GetCurY() / 100.0f;
		d.z = _pScene->GetGridHeight( d.x, d.y );
		_pAttackShade->MoveTo( &d );		
	}
}
