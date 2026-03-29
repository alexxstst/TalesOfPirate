#pragma once
#include "STStateObj.h"
#include "CharacterAction.h"

enum eActorState
{
	enumNormal,		// 
	enumDied,		// 
	enumRemains,	// 
};


// 
class CActorDie
{
public:
	virtual ~CActorDie() {}
	virtual void Exec()	 {}
};

// 
class CSceneItem;
class CMonsterItem : public CActorDie
{
public:
    CMonsterItem();
    virtual ~CMonsterItem();

    void	Exec();

    void    SetCha( CCharacter* pCha )	    { _pCha=pCha;	        }
    void    SetItem( CSceneItem* pItem )    { _pItem = pItem;       }
	CSceneItem*		GetItem()				{ return _pItem;		}

private:
    CSceneItem*         _pItem;
    CCharacter*         _pCha;

};

// 
struct stNetNpcMission;
class CMissionTrigger : public CActorDie
{
public:
	CMissionTrigger();
	~CMissionTrigger();

	virtual void Exec();

	void	SetData( stNetNpcMission& v );

private:
	stNetNpcMission		*_pData;

};

class CCharacter;
class CActionState;
class CStateSynchro;
class CSkillRecord;
class CServerHarm;

class CActor
{
public:
	CActor(CCharacter *pCha);
	~CActor();

	void			InitState();

	CActionState*	GetCurState()				{ return _pCurState;			}
	CCharacter*		GetCha()					{ return _pCha;					}
	eActorState		GetState()					{ return _eState;				}
	void			SetState( eActorState v );

	bool			IsEmpty()					{ return _statelist.empty();	}

public:
    void            PlayPose( int poseid, bool isKeep=false, bool isSend=false );
	void			SetSleep()					{ _nWaitingTime = -1;			}

public:		// CActionState 
	bool	        SwitchState( CActionState* pState );						// ,,
	bool	        InsertState( CActionState* pState, bool IsFront=false );	// ,
    bool            AddState( CActionState* pState );

    CActionState*   FindStateClass( const type_info& info );            // 
    void            OverAllState();                                     // ,

	void			CancelState();										// 
	void			FrameMove(DWORD dwTimeParam);

	CActionState*   GetServerState();
	CActionState*   GetServerStateByID(int id);
	CActionState*   GetNextState();
	int				GetQueueCount()				{ return (int)_statelist.size();}
	bool			IsEnabled()					{ return GetState()==enumNormal;}

	void			FailedAction();

public:	// 
    void	        ActionBegin( DWORD pose_id );
	void			ActionKeyFrame( DWORD pose_id, int key_frame );
	void			ActionEnd( DWORD pose_id );

	void			Stop();
    void	        IdleState();

	void			ExecAllNet();			// 

	void			ExecDied();
	bool			AddDieExec( CActorDie* pDieExec );
	
	void			ClearQueueState();

protected:
    //void            _InitIdle( DWORD pose );

    typedef std::list<CStateSynchro*>  synchro;
    void            _ExecSynchro( synchro& s );
    void            _ClearSynchro( synchro& s );

protected:
	CActionState*	_pCurState;				// 
	typedef std::list<CActionState*> states;
	states			_statelist;

	CCharacter*		_pCha;					// 

public:
    CServerHarm*    CreateHarmMgr();
    CServerHarm*    FindHarm( int nFightID );
    CServerHarm*    FindHarm();				// 

private:
    typedef std::vector<CServerHarm*>    fights;
    fights          _fights;

	typedef std::vector<CActorDie*>		dies;	// 
	dies			_dies;

protected:
    int             _nWaitingTime;     // Wait,Pose
	eActorState		_eState;

};

// 
inline void	CActor::ActionKeyFrame( DWORD pose_id, int key_frame )	
{ 
	if( _pCurState ) _pCurState->ActionFrame(pose_id, key_frame);	
}

inline void	CActor::ActionBegin( DWORD pose_id )							
{ 
    if ( _pCurState ) _pCurState->ActionBegin(pose_id);				
}

inline void CActor::ActionEnd( DWORD pose_id )
{ 
    if ( _pCurState ) 
    {
        _pCurState->ActionEnd(pose_id);
    }
    else if( _eState==enumNormal && (pose_id == POSE_SHOW || pose_id == POSE_FLY_SHOW ) )
    {
        IdleState();
    }
}

inline CActionState* CActor::GetServerState()
{
    if( _pCurState )
    {
        if( !_pCurState->GetIsOver() )
            return _pCurState;

        for( states::iterator it=_statelist.begin(); it!=_statelist.end(); ++it )
            if( !(*it)->GetIsOver() )
                return *it;
    }
    return NULL;
}

inline CActionState* CActor::GetServerStateByID(int id)
{
    if( _pCurState )
    {
        if( _pCurState->GetServerID()==id )
            return _pCurState;

        for( states::iterator it=_statelist.begin(); it!=_statelist.end(); ++it )
            if( (*it)->GetServerID()==id )
                return *it;
    }
    return NULL;
}

inline void CActor::_ExecSynchro( synchro& s )
{
    for( synchro::iterator it=s.begin(); it!=s.end(); ++it )
    {
        (*it)->Exec();
        // delete *it;
    }
    s.clear();
}

inline void CActor::_ClearSynchro( synchro& s )
{
    for( synchro::iterator it=s.begin(); it!=s.end(); ++it )
    {
        //delete *it;
		SAFE_DELETE(*it);// UI
    }
    s.clear();
}

inline void CActor::InitState()
{
	ClearQueueState();
	SAFE_DELETE( _pCurState );

	_eState = enumNormal;
    IdleState();
}

inline bool CActor::AddDieExec( CActorDie* pDieExec )
{
	_dies.push_back( pDieExec );
	return true;
}
