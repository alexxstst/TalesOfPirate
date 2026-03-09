//----------------------------------------------------------------------
// :
// :lh 2004-06-15
// :
//----------------------------------------------------------------------

#pragma once

class CCharacter;
class CSkillRecord;
class CMouseDown;
class CIsStart;

class CAutoAttack
{
public:
	CAutoAttack( CMouseDown* pMouseDown );
	void			Reset();

	void			FrameMove();
	void			Cancel()				{ _eStyle=eNone;	}

	void			SetIsStart( bool v )	{ _IsStart=v;		}
	bool			GetIsStart()			{ return _IsStart;	}

	// 
	bool			AttackStart( CCharacter* pMain, CSkillRecord* pSkill, CCharacter* pCha );
	bool			AttackStart( CCharacter* pMain, CSkillRecord* pSkill, int nScrX, int nScrY );
	bool			AttackMoveTo( int nScrX, int nScrY );

	// 
	bool			Follow( CCharacter* pMain, CCharacter* pTarget );

private:
	void			_CaleDist();

enum eStyle
	{
		eNone,
		eFollow,
		eAttack,
	};

	eStyle			_eStyle;
	bool			_IsStart;

	CCharacter*		_pTarget;
	CCharacter*		_pMain;
	int				_nAttackX, _nAttackY;
	CSkillRecord*	_pSkill;

	CMouseDown*		_pMouseDown;

	bool			_IsMove;
	int				_nMoveX, _nMoveY;

	int				_nTotalDis;
			
};

class CIsStart
{
public:
	CIsStart( CAutoAttack* pAuto ) : _pAuto(pAuto) {	_pAuto->SetIsStart( false );	}
	~CIsStart()					{ if( !_pAuto->GetIsStart() )	_pAuto->Cancel(); 		}

private:
	CAutoAttack*	_pAuto;

};
