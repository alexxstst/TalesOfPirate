#pragma once

#include "HMSynchroObj.h"

class CStateSynchro;
class CSkillRecord;
class CActor;
class CCharater;
class CAttackEffect;
class CCharacter;
class CAttackRepSynchro;

const int ERROR_FIGHT_ID = INT_MAX;

class CServerHarm
{
    typedef std::list<CAttackEffect*> synchros;
    friend CActor;
public:
    CServerHarm( CActor* pActor );
    ~CServerHarm();

	void	InitMemory();

    void    Exec();             
    void    Gouge( int nGouge );                // 

    bool    AddHarm( CAttackEffect* s, CSkillRecord *pSkill );        // ,
    bool    AddRep( CAttackRepSynchro* s );         // ,
    bool    IsAllowStateOver();                 // 

    void    ReadyExec()             { _nReadyExec++;            }
	void    SetIsOuter( bool v );

	void	ExecAll(int nCount);
	void	ExecAll();

	void	SetFightID( int v )		{ _nFightID = v;			}
	void	SetSkill( CSkillRecord* p )	{ _pSkill = p;			}
	
	int		GetCount()				{ return (int)_harm.size();	}

	void	DelHarm( CAttackEffect* p )	{ _harm.remove( p );	}

	void 	HitRepresent( CSkillRecord* pSkill, int nAngle );

public:
    CSkillRecord*   GetSkill()      { return _pSkill;           }

    int     GetFightID()            { return _nFightID;         }
	bool	GetIsExecEnd()		    { return _nFightID==ERROR_FIGHT_ID; }

private:

private:
    int         _nFightID;
    CActor*     _pActor;            // Actor
	CCharacter* _pCha;

    synchros    _harm;              // ,
    synchros    _rep;               // ,

    CSkillRecord*   _pSkill;

    int         _nReadyExec;        // 

	bool		_IsOuter;			// 

};

// 
