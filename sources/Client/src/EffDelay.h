#pragma once
#include "SkillRecord.h"

#define ACTION_BEGIN_HIT	-1
#define ACTION_END_HIT		-2

class CCharacter;
class CEffectObj;
class CActor;
class CSkillRecord;
class CServerHarm;

// һ���Ե��ܻ���Ч,�������Ч,�ܻ���Ч,�����ܻ���Ч��
class CHitRepresent 
{
public:
	CHitRepresent() : _pTarget(NULL), _pSkill(NULL), _nAttackX(0), _nAttackY(0), _pAttack(NULL) {}

	void	SetAttack( CCharacter*	p )		{ _pAttack=p;				}
	void	SetTarget( CCharacter*	p )		{ _pTarget=p;				}
	void	SetSkill( CSkillRecord* p )		{ _pSkill=p;				}
	void	SetAttackPoint( int x, int y )	{ _nAttackX=x; _nAttackY=y;	}

	void	ActionExec( CServerHarm* pHarm, int nActionKey );
	void    EffectExec( CServerHarm* pHarm );

private:
	CSkillRecord*	_pSkill;
	CCharacter*		_pTarget;				// ��������ʱ��Ŀ��
	int				_nAttackX,  _nAttackY;	// ����ʱ�������ĵ�
	CCharacter*		_pAttack;				// ������, ������ʾ�ܻ��ķ���,����Ϊ��

private:
	void	Exec( CServerHarm* pHarm );

};

// ���ڷ�����ЧʧЧ�󲥷��ӳٵ��ܻ���Ч,�ӳ���Ч,�����˺�����
class CEffDelay
{
public:
    enum ePlayStyle
    {
        enumNone,       // ʲô�¶����� 
        enumPos,        // ��ĳ�ص㲥��
        enumHitEffect,  // �ܻ���Ч
    };

public:
    CEffDelay(CEffectObj* pOwn);
    ~CEffDelay();

    void Reset()        { _eStyle=enumNone; }
    void Exec();

    void SetPosEffect( int nEffID, const D3DXVECTOR3& pos );
    void SetServerHarm( CHitRepresent& Hit, CServerHarm* pHarm );

private:
    CEffectObj* _pOwn;
    ePlayStyle  _eStyle;
    int         _nEffectID;

    // enumPos
    D3DXVECTOR3 _Pos;

    // enumActor
	CHitRepresent	_cHit;
    CServerHarm*    _pHarm;

};

// ��������
inline void CHitRepresent::EffectExec( CServerHarm* pHarm )
{
	if( !_pSkill->IsEffectHarm() ) return;

	Exec( pHarm );
}

inline void CHitRepresent::ActionExec( CServerHarm* pHarm, int nActionKey )
{
	if( !_pSkill->IsActKeyHarm() ) return;

	if( _pSkill->chTargetEffectTime!=nActionKey ) return;

	Exec( pHarm );
}

inline void CEffDelay::SetServerHarm( CHitRepresent& Hit, CServerHarm* pHarm )
{
    if( _eStyle!=enumNone ) 
    {
        ToLogService("common", "msgCEffDelay::SetServerHarm() Style[{}] is Valid", static_cast<int>(_eStyle));
    }

    _eStyle = enumHitEffect;

	_cHit = Hit;
	_pHarm = pHarm;
}

inline void CEffDelay::SetPosEffect( int nEffID, const D3DXVECTOR3& pos )
{
    if( _eStyle!=enumNone ) 
    {
        ToLogService("common", "msgCEffDelay::SetPosEffect(nEffID[{}],pos[{},{},{}]) Style[{}] is Valid", nEffID, pos.x, pos.y, pos.z, static_cast<int>(_eStyle));
    }
    _eStyle = enumPos;

    _nEffectID = nEffID;
    _Pos = pos;
}
