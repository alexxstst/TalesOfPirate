#include "stdafx.h"
#include "autoattack.h"
#include "isskilluse.h"
#include "gameapp.h"
#include "skillrecord.h"
#include "mousedown.h"
#include "character.h"
#include "uistartform.h"
#include "uiminimapform.h"

#include "stmove.h"
#include "stattack.h"
#include "findpath.h"

CAutoAttack::CAutoAttack( CMouseDown* pMouseDown )
: _pMouseDown( pMouseDown )
{
	Reset();
}

void CAutoAttack::Reset()
{
	_pTarget = NULL;

	_pMain = NULL;
	_pSkill = NULL;
	_IsMove = false;

	_eStyle = eNone;
	_IsStart = false;
}

bool CAutoAttack::AttackStart( CCharacter* pMain, CSkillRecord* pSkill, CCharacter* pCha )
{
	if( !pMain || !pSkill || !pCha )
	{
		_eStyle = eNone;
		return false;
	}

	if( !g_SkillUse.IsUse( pSkill, pMain, pCha ) )
	{
		g_pGameApp->SysInfo( "%s", g_SkillUse.GetError() );
		return false;
	}

	g_stUIStart.SysLabel( g_oLangRec.GetString(4), g_stUIMap.IsPKSilver() ? "??????" : pCha->getName().c_str() );

	_pMain = pMain;
	_pSkill = pSkill;
	_pTarget = pCha;
	_IsMove = false;
	_eStyle = eAttack;

	_CaleDist();

	_IsStart = true;
	return true;
}

bool CAutoAttack::AttackStart( CCharacter* pMain, CSkillRecord* pSkill, int nScrX, int nScrY )
{
	if( !pMain || !pSkill )
	{
		_eStyle = eNone;
		return false;
	}

	if( !g_SkillUse.IsUse( pSkill, pMain, NULL ) )
	{
		g_pGameApp->SysInfo( "%s", g_SkillUse.GetError() );
		return false;
	}

	g_stUIStart.SysLabel( g_oLangRec.GetString(5), nScrX/100, nScrY/100 );

	_pTarget = NULL;

	_eStyle = eAttack;
	_pMain = pMain;
	_pSkill = pSkill;
	_nAttackX = nScrX;
	_nAttackY = nScrY;
	_IsMove = false;

	_CaleDist();

	_IsStart = true;
	return true;
}

void CAutoAttack::_CaleDist()
{
	if( _pSkill->GetDistance()>0 )
	{
		_nTotalDis = _pMain->GetDefaultChaInfo()->sRadii + _pSkill->GetDistance();
		if( _pTarget ) _nTotalDis += _pTarget->GetDefaultChaInfo()->sRadii;
	}
	else
	{
		_eStyle = eNone;
	}

}

void CAutoAttack::FrameMove()
{
	switch ( _eStyle )
	{
	case eAttack:
		{
			if( !_pSkill->IsAttackTime( CGameApp::GetCurTick() ) )
				return;
			
			static DWORD time = 0;
			if( CGameApp::GetCurTick()<time )
				return;

			time = CGameApp::GetCurTick() + 500;
			if( !g_SkillUse.IsUse( _pSkill, _pMain, _pTarget ) ) 
			{
				_eStyle = eNone;

				if( _IsMove ) 
				{
					_pMouseDown->ActMove( _pMain, _nMoveX, _nMoveY, true, false, false );
				}
				return;
			}

			if( _IsMove )
			{
				if( GetDistance( _nMoveX, _nMoveY, _pMain->GetCurX(), _pMain->GetCurY() ) < 50 )
				{
					_IsMove = false;
				}

				if( CAttackState::GetLastTime()>CServerMoveState::GetLastTime() )
				{
					_pMouseDown->ActMove( _pMain, _nMoveX, _nMoveY, false, false, false );
					return;
				}
			}
			
			if( _pTarget )
			{
				if( !_pTarget->IsValid() || !_pTarget->IsEnabled() || _pTarget->IsHide() )					
				{
					_eStyle = eNone;
				}
				else
				{
					_pMouseDown->ActAttackCha( _pMain, _pSkill, _pTarget, false, false, false );
				}
			}
			else
			{		
				if( GetDistance( _pMain->GetCurX(), _pMain->GetCurY(), _nAttackX, _nAttackY )>_nTotalDis )
				{
					bool IsWalkLine = false;
					if( !g_cFindPath.GetPathFindingState()/*g_cFindPath.Find( _pMain->GetScene(), _pMain, _pMain->GetCurX(), _pMain->GetCurY(), _nAttackX, _nAttackY, IsWalkLine )*/ )
					{
						_eStyle = eNone;
						return;
					}
				}

				_pMouseDown->ActAttackArea( _pMain, _pSkill, _nAttackX, _nAttackY, false );
			}
		}
		break;
	case eFollow:
		{
			if( !_pMain->IsValid() || !_pMain->IsEnabled() || !_pTarget->IsValid() || !_pTarget->IsPlayer() ) 
			{
				_eStyle = eNone;
			}
			else
			{
				static DWORD time = 0;
				if( CGameApp::GetCurTick()>time || (_pMain->GetIsArrive() && _pMain->GetActor()->IsEmpty()) )
				{
					time = CGameApp::GetCurTick() + 1000;

					const int dis = 200;
					if( GetDistance( _pMain->GetServerX(), _pMain->GetServerY(), _pTarget->GetServerX(), _pTarget->GetServerY() )>dis )
					{
						int x, y;
						GetDistancePos( _pTarget->GetServerX(), _pTarget->GetServerY(), _pMain->GetServerX(), _pMain->GetServerY(), dis - 100, x, y );
						_pMouseDown->ActMove( _pMain, x, y, false, false, false );
					}
				}
			}
		}
		break;
	default:
		g_stUIStart.SysHide();
		break;
	}
}

bool CAutoAttack::AttackMoveTo( int nScrX, int nScrY )
{
	if( _eStyle!=eAttack ) return false;

	//if( !_pTarget && GetDistance( _nAttackX, _nAttackY, nScrX, nScrY ) > _nTotalDis )
	//{		
	//	_eStyle = eNone;
	//	return false;
	//}

	_IsMove = true;
	_nMoveX = nScrX;
	_nMoveY = nScrY;

	_pMouseDown->ActMove( _pMain, _nMoveX, _nMoveY, true, false, false );

	_IsStart = true;
	return true;
}

bool CAutoAttack::Follow( CCharacter* pMain, CCharacter* pTarget )
{
	_eStyle = eNone;
	if( !pMain || !pTarget )
		return false;

	if( !pMain->IsValid() || !pTarget->IsValid() ) 
		return false;

	if( !pTarget->IsPlayer() ) 
		return false;

	if( pMain==pTarget ) 
		return false;

	g_stUIStart.SysLabel( g_oLangRec.GetString(6), g_stUIMap.IsPKSilver() ? "??????" : pTarget->getName().c_str() );

	_pTarget = pTarget;
	_pMain = pMain;
	_eStyle = eFollow;

	_IsStart = true;
	return true;
}
