#include "stdafx.h"
#include "SkillRecord.h"
#include "GameApp.h"
#include "Character.h"	
#include "CharacterAction.h"
#include "STMove.h"
#include "EffectObj.h"
#include "stpose.h"
#include "HMManage.h"
#include "SceneItem.h"
#include "chastate.h"
#include "Actor.h"

extern CLanguageRecord g_oLangRec;
//---------------------------------------------------------------------------
// class CActor
//---------------------------------------------------------------------------
CActor::CActor(CCharacter *pCha)
: _pCha(pCha), _pCurState(NULL), _eState(enumNormal), _nWaitingTime(-1)
{
}

CActor::~CActor()
{
	ClearQueueState();
	SAFE_DELETE( _pCurState );
}

bool CActor::AddState( CActionState* pState )
{
	if( !IsEnabled() ) 
	{
		//delete pState;
		SAFE_DELETE(pState); // UI
		return false;
	}

	if( _nWaitingTime>0 ) _nWaitingTime = 0;

    if( !_pCurState )
    {
        _pCurState = pState;
        _pCurState->Start();
    }
    else
    {
        _pCurState->BeforeNewState();

        pState->SetIsWait(true);
        _statelist.push_back(pState);
    }
    return true;
}

bool CActor::InsertState( CActionState* pState, bool IsFront )
{
	if( !IsEnabled() ) 
	{
		//delete pState;
		SAFE_DELETE(pState); // UI
		return false;
	}

	if( _nWaitingTime>0 ) _nWaitingTime = 0;

	if( !_pCurState )
	{
		_pCurState = pState;
        _pCurState->Start();
	}
	else
	{
        _pCurState->BeforeNewState();

		pState->SetIsWait(true);
		_statelist.push_front(pState);
	}
	return true;
}

bool CActor::SwitchState( CActionState* pState )
{
    if( !pState->IsAllowUse() )
    {
        //delete pState;
		SAFE_DELETE(pState); // UI
        return false;
    }

	if( _nWaitingTime>0 ) _nWaitingTime = 0;

	if( _pCurState )
	{
        _pCurState->BeforeNewState();
		
		if( _pCurState->GetIsCancel() ) 
		{
			// 
			ClearQueueState();		

			pState->SetIsWait(true);
			_statelist.push_back(pState);
			return true;
		}

		ClearQueueState();		// ,

        _pCurState->Cancel();

		pState->SetIsWait(true);
		_statelist.push_back(pState);
		return true;
	}
	else
	{
		ClearQueueState();		// ,

		_pCurState = pState;
        _pCurState->Start();
		return true;
	}
}

void CActor::CancelState()
{
	ClearQueueState();

	if( _pCurState )
	{
        _pCurState->Cancel();
	}
	else
	{
	    IdleState();
	}
}

void CActor::ClearQueueState()
{
	while(!_statelist.empty()) 
	{
		delete _statelist.front();
		_statelist.pop_front();
	}
}

void CActor::IdleState() 
{ 
	if( _eState==enumNormal )
	{
		_pCha->PlayPose( _pCha->GetPose(POSE_WAITING), PLAY_LOOP_SMOOTH );

		_nWaitingTime = -1;

		switch( _pCha->getChaCtrlType() )
		{
		case enumCHACTRL_PLAYER:
			if( _pCha->getChaModalType()!=enumMODAL_BOAT)
				_nWaitingTime = 30 * 32;
			break;
		case enumCHACTRL_MONS:
			if( _pCha->getChaModalType()==enumMODAL_OTHER )
				_nWaitingTime = (rand() % 15 + 3) * 32;
			break;
		}
    }
}

void CActor::ExecAllNet()
{
	ClearQueueState();
	SAFE_DELETE( _pCurState );

	for( auto it=_fights.begin(); it!=_fights.end(); it++ )
    {
        if( !(*it)->GetIsExecEnd() )
        {
            (*it)->ExecAll();
			(*it)->SetIsOuter( false );
        }
    }

	ExecDied();
}

void CActor::ExecDied()
{
	for( auto it=_dies.begin(); it!=_dies.end(); it++ )
	{
		(*it)->Exec();
		//delete (*it);
		SAFE_DELETE(*it); // UI
	}

	_dies.clear();

	_pCha->GetStateMgr()->ChaDied();
}

void CActor::FailedAction()
{
	ClearQueueState();

	if( _pCurState )
	{
		_pCurState->ServerEnd( 0 );
		_pCurState->MoveEnd( _pCha->GetCurX(), _pCha->GetCurY(), 0 );
	}
}

void CActor::FrameMove(DWORD dwTimeParam) 
{
	if( _pCurState ) 
	{
		if ( !_pCurState->GetIsExecEnd() )
		{
			_pCurState->FrameMove();
		}

		if ( _pCurState->GetIsExecEnd() )
		{
			_pCurState->End();

			if( _statelist.empty() )
			{
                if( !_pCurState->IsKeepPose() )
				    IdleState();

				SAFE_DELETE( _pCurState );
			}
			else
			{
				SAFE_DELETE( _pCurState );

				_pCurState = _statelist.front();
				_statelist.pop_front();

				_pCurState->SetIsWait(false);
				_pCurState->Start();
			}
		}
	}
    else if( _eState==enumNormal )
    {
		if( _pCha->GetStateMgr()->GetSkillStateNum()==0 && _nWaitingTime>0 )
        {
            _nWaitingTime--;
            if( _nWaitingTime==0 )
            {
				switch( _pCha->getChaCtrlType() )
				{
				case enumCHACTRL_PLAYER:
					if( _pCha->getChaModalType()!=enumMODAL_BOAT && !_pCha->GetIsOnMount())
		                _pCha->PlayPose( POSE_SHOW, PLAY_ONCE );
					break;
				case enumCHACTRL_MONS:
					if(_pCha->getMobID() == 71)
					{
						_pCha->PlayPose(POSE_SHOW, PLAY_ONCE, -1, 60, false, true);
					}else{
						_pCha->PlayPose( POSE_SHOW, PLAY_ONCE );
					}
					break;
				}
            }
        }
    }
}

void CActor::Stop()
{
	ClearQueueState();

	if( _pCurState ) 
	{
		_pCurState->Cancel();
	}
}

void CActor::SetState( eActorState v )	
{ 
	_eState = v;

	GetCha()->RefreshShadow();

    if( _eState==enumNormal )
    {
        IdleState();
    }
}

void CActor::PlayPose( int poseid, bool isKeep, bool isSend )
{
    CPoseState * state = new CPoseState(this);
    state->SetKeepPose( isKeep );
    state->SetPose( poseid );
    state->SetIsSend( isSend );

    SwitchState( state );
}

CActionState* CActor::FindStateClass( const type_info& info )
{
    if( _pCurState )
    {
        if( typeid(*_pCurState)==info )
            return _pCurState;

        for( auto it=_statelist.begin(); it!=_statelist.end(); ++it )
            if( typeid(**it)==info )
                return *it;
    }
    return NULL;
}

CServerHarm* CActor::CreateHarmMgr()
{
    for( auto it=_fights.begin(); it!=_fights.end(); it++ )
    {
        if( (*it)->GetIsExecEnd() )
        {
            (*it)->InitMemory();
            return *it;
        }
    }

    CServerHarm* tmp = new CServerHarm( this );
    _fights.push_back( tmp );
	ToLogService(_pCha->getLogName(), "CServerHarm CreateHarmMgr, Size[{}]", _fights.size());
    return tmp;
}

CServerHarm* CActor::FindHarm( int nFightID )
{
    for( auto it=_fights.begin(); it!=_fights.end(); it++ )
    {
        if( (*it)->GetFightID()==nFightID )
        {
            return *it;
        }
    }
    return NULL;
}

CServerHarm* CActor::FindHarm()
{
    for( auto it=_fights.begin(); it!=_fights.end(); it++ )
    {
        if( !(*it)->GetIsExecEnd() )
        {
            return *it;
        }
    }
    return NULL;
}

void CActor::OverAllState()
{
	ClearQueueState();
    if( _pCurState )
    {
		_pCurState->PopState();
    }
}

CActionState* CActor::GetNextState()
{
	if( _statelist.empty() ) return NULL;

	return _statelist.front();
}

//---------------------------------------------------------------------------
// class CMonsterItemSynchro
//---------------------------------------------------------------------------
CMonsterItem::CMonsterItem()
: _pCha(NULL), _pItem(NULL)
{
}

CMonsterItem::~CMonsterItem()
{
}

void CMonsterItem::Exec()
{
    if( !_pItem ) return;

    if( _pCha )
    {
        _pItem->PlayArcAni( _pCha->GetPos(), _pItem->getPos(), 3.5f, 5.0f );
    }
    _pItem->SetHide( FALSE );

	int nEffectID = _pItem->GetItemInfo()->sAreaEffect[0];
	if( nEffectID<=0 )
		return;

	int nDummy = _pItem->GetItemInfo()->sAreaEffect[1];

	// 
	// 
    CEffectObj	*pEffect = CGameApp::GetCurScene()->GetFirstInvalidEffObj();
    if( !pEffect ) return;            

    if( !pEffect->Create( nEffectID ) )
    {
		ToLogService("protocol", "{} {}", g_oLangRec.GetString(1), nEffectID);
        return;
    }
	pEffect->setFollowObj((CSceneNode*)_pItem, NODE_ITEM);
	pEffect->Emission(-1, &_pItem->getPos(), NULL);
	_pItem->AddEffect( pEffect->getID() );
    pEffect->SetValid(TRUE);
}

//---------------------------------------------------------------------------
// class CMissionTrigger
//---------------------------------------------------------------------------
CMissionTrigger::CMissionTrigger()
{
	_pData = new stNetNpcMission;
}

CMissionTrigger::~CMissionTrigger()
{
	//delete _pData;
	SAFE_DELETE(_pData); // UI
}

void CMissionTrigger::SetData( stNetNpcMission& v )	
{
	memcpy( _pData, &v, sizeof(stNetNpcMission) );	
}

void CMissionTrigger::Exec()
{
	char szData[64] = {0};
	strcpy(szData, g_oLangRec.GetString(2));

	CChaRecord* pCharRecord = GetChaRecordInfo( _pData->sID );
	if( pCharRecord )
	{
		strncpy( szData, pCharRecord->szName, sizeof(szData) );
	}
	g_pGameApp->ShowMidText( g_oLangRec.GetString(3), szData, _pData->sCount, _pData->sNum );
}

