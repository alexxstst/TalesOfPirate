#pragma once
#include "HMSynchroObj.h"
#include "Tools.h"
#include "NetProtocol.h"

enum eAttackEffectType   // 
{    
	enumAddLife=0,		// ".tga";
	enumSubLife,		// ".tga";
	enumAddSp,			// "sp.tga";
	enumSubSp,			// "sp.tga";
	enumAddLifeMonster,	// ".tga";
	enumSubLifeMonster, // ".tga";
	enumDoubleAttack,	// ".tga";
	enumMiss,			// "Miss.tga";
};

struct stEffect;
struct stSkillState;
class CCharacter;
class CSkillRecord;
class CChaAttr;
class CAttackRepSynchro;

class CChaAttrChange
{
public:
	CChaAttrChange()	{ memset( _szAttr, 0, sizeof(_szAttr) );	}

	void Reset()		{ memset( _szAttr, 0, sizeof(_szAttr) );	}
	void SetChangeBitFlag( int nFlag )	{ _szAttr[nFlag] = 1;			}
	bool GetChangeBitFlag( int nFlag )	{ return _szAttr[nFlag]!=0;		}

private:
	char  _szAttr[MAX_ATTR_CLIENT];

};

class CAttackEffect : public CStateSynchro
{
public:
	CAttackEffect();
    virtual ~CAttackEffect();

    virtual CStateSynchro*  Gouge( float fRate );
	virtual CCharacter*		GetHarmCha()	{ return _pTarget;	}
	virtual const char*		GetClassName()	{ return "CAttackEffect";  }

    CCharacter*     GetTarget()             { return _pTarget;  }

    void SetSkill( CSkillRecord* p )        { _pSkill=p;        }
	void SetAttackCha( CCharacter* pCha )	{ _pAttack=pCha;	}
	void SetTargetCha( CCharacter* pCha )	{ _pTarget=pCha;	}

	void SetIsDoubleAttack( bool v )		{ _isDoubleAttack=v;}
    void SetIsMiss( bool v )                { _IsMiss=v;        }

    void SetBeatPos( bool v, int x, int y ) { _nBeatX=x; _nBeatY=y;  _isBeatBack=v; }

	void SetTargetIsDied( bool v )			{ _isTargetDied=v;  }
    void SetHarmValue( stEffect* pChange, unsigned int nMax ) { _HarmValue.SetValue( pChange, nMax ); }
    void SetHarmState( stSkillState* pState, int nCount )     { _HarmState.SetValue( pState, nCount); }

    static void    CreateEffect( int eType, const char* str, D3DXVECTOR3& start, D3DXVECTOR3& target, D3DXVECTOR3& dir, bool isMain, DWORD& dwDelay );
    static void    ExecHarm( CSizeArray<stEffect>& Value, CCharacter* pTarget, CCharacter* pAttack=NULL );
    static void    ChaDied( CCharacter* pTarget, CCharacter* pAttack=NULL );

	void	SetAttackRep( CAttackRepSynchro* p )			{ _pRepSynchro = p;		}

protected:
	virtual void _Exec();

protected:

protected:
	CAttackRepSynchro*	_pRepSynchro;	// 
	CCharacter*     _pTarget;	// 
	CCharacter*     _pAttack;	// 
    CSkillRecord*   _pSkill;
    
    // 
	bool	_isDoubleAttack;
    bool    _IsMiss;
    bool	_isTargetDied;
    bool    _isBeatBack;        // 
    int     _nBeatX, _nBeatY;   // 
    CSizeArray<stEffect>		_HarmValue;
    CSizeArray<stSkillState>	_HarmState;

};

class CAttackRepSynchro : public CStateSynchro
{
public:
	CAttackRepSynchro();
    virtual ~CAttackRepSynchro();
	virtual CCharacter*		GetHarmCha()						{ return _pAttack;			}

	void	SetAttackEffect( CAttackEffect* p );
    void	SetSkill( CSkillRecord* p )							{ _pSkill=p;				}
	void	SetAttackCha( CCharacter* pCha )					{ _pAttack=pCha;			}
	void	SetTargetCha( CCharacter* pCha )					{ _pTarget=pCha;			}
	void	SetAttackIsDied( bool v )							{ _IsAttackDied = v;		}

    void	SetRepValue( stEffect* pChange, unsigned int nMax )	{ _RepValue.SetValue( pChange, nMax ); }
    void	SetRepState( stSkillState* pState, int nCount )		{ _RepState.SetValue( pState, nCount); }
	virtual const char* GetClassName()							{ return "CAttackRepSynchro";		   }

protected:
	virtual void _Exec();

private:
	CAttackEffect*	_pAttackEffect;		// 
	CCharacter*     _pTarget;			// 
	CCharacter*     _pAttack;			// 
    CSkillRecord*   _pSkill;

    // 
    bool    _IsAttackDied;
    CSizeArray<stSkillState>	_RepState;
    CSizeArray<stEffect>		_RepValue;

};

class CAttribSynchro : public CStateSynchro
{
public:
	CAttribSynchro();
	virtual ~CAttribSynchro();

    void	SetValue( stEffect* pChange, unsigned int nMax )  { _Value.SetValue( pChange, nMax ); }
	void	SetType( int nType )		{ _nType = nType;	}

	void	SetCha( CCharacter* pCha )							{ _pCha = pCha;						}
	virtual CCharacter*	GetHarmCha()							{ return _pCha;						}
	virtual const char* GetClassName()							{ return "CAttribSynchro";			}

	void	Start();

protected:
	virtual void _Exec();

protected:
    CSizeArray<stEffect>  _Value;
	int				_nType;	
	CCharacter*		_pCha;

	bool			_IsAlreadyExec;

};

class CSkillStateSynchro : public CStateSynchro
{
public:
	CSkillStateSynchro();
	virtual ~CSkillStateSynchro();

    void	SetValue( stSkillState* pState, int nCount )		{ _SState.SetValue( pState, nCount);}
	void	SetType( char chType )								{ _chType = chType;					}

	void	SetCha( CCharacter* pCha )							{ _pCha = pCha;						}
	virtual CCharacter*	GetHarmCha()							{ return _pCha;						}
	virtual const char* GetClassName()							{ return "CSkillStateSynchro";		}

protected:
	virtual void _Exec();

private:
	CCharacter*		_pCha;
	char			_chType;
	CSizeArray<stSkillState> _SState;

};

// 
inline void CAttackRepSynchro::SetAttackEffect( CAttackEffect* p )					
{ 
	_pAttackEffect = p;
}
