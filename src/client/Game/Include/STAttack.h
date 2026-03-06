#pragma once
#include "STStateObj.h"
#include "SkillRecord.h"
#include "effdelay.h"

class CServerMoveState;
class CStateSynchro;
class CSkillRecord;
class CSceneItem;
class CServerHarm;
class CHitRepresent;

class CWaitAttackState : public CActionState
{
public:
	CWaitAttackState(CActor* p);
    ~CWaitAttackState();

    virtual void    End();

    virtual const char* GetExplain()                { return "CWaitAttackState";        }

    virtual void    ActionBegin( DWORD pose_id );
	virtual void	ActionFrame( DWORD pose_id, int key_frame );
	virtual void	ActionEnd( DWORD pose_id );
	virtual void	FrameMove();
	virtual void	MoveEnd(int x, int y, int nState);
    virtual void    ServerEnd(int nState);
    virtual void	SetServerID( int n );

	void			SetTarget( CCharacter* p );
    CCharacter*     GetTarget()                     { return _pTarget;                  }

	void			SetAttackPoint( int x, int y );

	void			SetSkill( CSkillRecord* p );
	CSkillRecord*	GetSkill()						{ return _pSkillInfo;				}

    void            ServerYaw( int nAngle )         { _nAngle=nAngle; _IsHaveAngle=true;}

    CServerHarm*    GetServerHarm()                 { return _pHarm;                    }
	bool			IsInvalidHarm()					{ return GetServerID()==INT_MAX;	}

	void			SetSkillSpeed( int v )			{ if( v<=0 ) v=1; _nSkillSpeed = v;	}

protected:
	virtual bool	_Start();
	virtual void	_End();
    virtual bool	_IsAllowCancel()				{ return false;						}

	void	CalcSkillSpeed();		// 

protected:
	virtual void    _UseSkill();
	virtual void    _StartActor();

protected:
    bool    _IsPoseValid( DWORD pose_id );
    int     _GetRand( int nMax )        { return  ( RAND_MAX * rand() + rand() ) % nMax;}
	int		_GetPoseID();
    void    _PopThis();

protected:
	CCharacter		*_pTarget;				// 
	CSkillRecord	*_pSkillInfo;			// 
	int				_nAttackX,  _nAttackY;	// 
	CCharacter		*_pSelf;				// ,
    int             _nAngle;
    bool            _IsHaveAngle;
    int             _nKeyFrameNum;
    int             _nSkillPoseID;          // Pose,posepose
	int				_nSkillActionIndex;		// Pose,
	float			_fSkillSpeed[ defSKILL_POSE_NUM ];
	float			_fSkillRate;

	CHitRepresent	_cHit;
	int				_nActionEnd;

protected:
	enum eUseSkill
	{
		enumInit,
		enumStart,
		enumUseSkill,
	};
	eUseSkill		_eUseSkill;
    int             _nAttackState;
    CServerHarm*    _pHarm;
	int				_nSkillSpeed;

};

namespace GUI
{
class CCommandObj;
};

// 
class CAttackState : public CWaitAttackState
{
public:
	struct stLastAttack
	{
		stLastAttack() : pTarget(NULL), dwTime(0) {}
		void Reset()	{ pTarget=NULL;		}

		CCharacter* pTarget;
		DWORD		dwTime;
	};

	CAttackState(CActor* p);
	~CAttackState();

    virtual const char* GetExplain()				{ return "CAttackState";               }

	virtual void FrameMove();
	virtual void PushPoint( int x, int y );
	virtual void MoveEnd(int x, int y, int nState);
	virtual void ActionFrame( DWORD pose_id, int key_frame );
    virtual void ActionBegin( DWORD pose_id );
	virtual void ActionEnd( DWORD pose_id );
	virtual bool IsAllowUse();
	virtual void Cancel();

	void	SetCommand( CCommandObj* pCommand )		{ _pCommand=pCommand;					}
	void	SetIsForce( bool v )					{ _IsForce = v;							}

	static  stLastAttack*	GetLastTarget()			{ return &_sLastTarget;					}

	static  DWORD			GetLastTime()			{ return _dwLastAttackTime;				}

protected:
	virtual bool _Start();
    virtual bool _IsAllowCancel();
	virtual void _End();

protected:
	virtual void    _UseSkill();

	static stLastAttack  _sLastTarget;	// 
	static DWORD		 _dwLastAttackTime;

protected:
	CServerMoveState*	_pMove;
    int					_nTotalDis;     // 
	bool				_IsForce;

    bool				_IsMoveOver;
	CCommandObj*		_pCommand;
	bool				_IsLocalCancel;

	DWORD				_dwEndTime;
	int					_nActionKeyCount;
	int					_nActionEndCount;

};

// 
class CTraceAttackState : public CAttackState
{
public:
	CTraceAttackState(CActor* p);
	~CTraceAttackState();

	virtual void MoveEnd(int x, int y, int nState);
	virtual void ServerEnd(int nState);
	virtual void BeforeNewState()		{ _IsTrace = false;		}
    virtual const char* GetExplain()    { return "CTraceAttackState";   }

protected:
	virtual void _End();
	virtual void StartFailed();

	void	CheckTrace();

protected:
	bool		_IsTrace;
	bool		_IsSuccess;		// 

};

// 
class CHitAttackState : public CActionState
{
public:
	CHitAttackState(CActor* p);
    ~CHitAttackState();

    virtual const char* GetExplain()     { return "CHitAttackState";}

    virtual void    ActionBegin( DWORD pose_id );
	virtual void	ActionFrame( DWORD pose_id, int key_frame );
	virtual void	ActionEnd( DWORD pose_id );

	void	SetPose( int* pose, int num ){
		_nPoseNum = num;
		memcpy( _nPose, pose, sizeof(int) * num );
	}
	void	SetSpeed( float fSpeed )	{ _fSpeed = fSpeed;		}

protected:
	virtual bool	_Start();
	virtual void	_End();

private:
	int				_nPose[100];
	int				_nPoseNum;
	CCharacter*		_pSelf;
	float			_fSpeed;

	int				_nIndex;		// 
	int				_nCurPose;

};

// pose,
class CAllPoseState : public CActionState
{
public:
	CAllPoseState(CActor* p);
    ~CAllPoseState();

    virtual const char* GetExplain()     { return "CAllPoseState";}

    virtual void    ActionBegin( DWORD pose_id );
	virtual void	ActionFrame( DWORD pose_id, int key_frame );
	virtual void	ActionEnd( DWORD pose_id );
	void	SetSpeed( float fSpeed )	{ _fSpeed = fSpeed;		}
	void	SetChaID( int nChaID )		{ _nCurChaID = nChaID;	}
	void	SetChaRange( int nStart, int nEnd ) {
		_nChaStart = nStart, _nChaEnd = nEnd;
	}

protected:
	virtual bool	_Start();
	virtual void	_End();

private:
	int				_nPoseStart, _nPoseEnd;
	int				_nCurPose;
	CCharacter*		_pSelf;
	float			_fSpeed;

	int				_nCurChaID;
	int				_nChaStart, _nChaEnd;

};

// 
inline bool CWaitAttackState::_IsPoseValid( DWORD pose_id )
{
    return _nSkillPoseID==pose_id;
}

inline void CWaitAttackState::SetAttackPoint( int x, int y )	
{ 
	_nAttackX = x;	
	_nAttackY = y;	
	_cHit.SetAttackPoint( x, y );
}

inline int CWaitAttackState::_GetPoseID()				
{ 
	_fSkillRate = 1.0f;
	if( _pSkillInfo->GetPoseNum()>1 )
	{
		if( _pSkillInfo->IsPlayRand() )
		{
			return _pSkillInfo->sActionPose[ _GetRand( _pSkillInfo->GetPoseNum() ) ];
		}
		else
		{
			int pose = _pSkillInfo->sActionPose[ _nSkillActionIndex ];
			_fSkillRate = _fSkillSpeed[ _nSkillActionIndex ];
			_nSkillActionIndex++;
			if( _nSkillActionIndex >= _pSkillInfo->GetPoseNum() )
				_nSkillActionIndex = 0;
			return pose;
		}
	}
	else
	{
		return _pSkillInfo->sActionPose[0];
	}
}
