//=============================================================================
// FileName: Attachable.h
// Creater: ZhangXuedong
// Date: 2004.10.19
// Comment: CAttachable class
//=============================================================================

#ifndef ATTACHABLE_H
#define ATTACHABLE_H

#include "Entity.h"

class	CPlayer;

/*
*	
*	Lark.li 
*/
class CAttachable : public Entity
{
	friend class CConjureMgr;
	friend class CPassengerMgr;
	friend class Entity;

public:
	CAttachable();
	void	SetPlayer(CPlayer *pCPlayer) {m_pCPlayer = pCPlayer;}
	CPlayer	*GetPlayer(void) {return m_pCPlayer;}
	void	SetShip(CPassengerMgr *pCShip) {m_pCShip = pCShip;}
	CPassengerMgr	*GetShip(void) {return m_pCShip;}
	void	SetShipMaster(CAttachable *pCShipM) {m_pCShipMaster = pCShipM;}
	CAttachable		*GetShipMaster(void) {return m_pCShipMaster;}

protected:
	void	Initially();
	void	Finally();

	CAttachable *IsAttachable() {return this;}

	CPlayer			*m_pCPlayer;
	CAttachable		*m_pCShipMaster;
	CPassengerMgr	*m_pCShip;

private:
	CAttachable	*m_pCConjureLast;
	CAttachable	*m_pCConjureNext;

	CAttachable	*m_pCPassengerLast;
	CAttachable	*m_pCPassengerNext;

};

#endif // ATTACHABLE_H
