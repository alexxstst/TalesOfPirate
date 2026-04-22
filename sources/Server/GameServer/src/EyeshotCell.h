//=============================================================================
// FileName: EyeshotCell.h
// Creater: ZhangXuedong
// Date: 2005.04.29
// Comment: Map Eyeshot Cell
//=============================================================================

#ifndef EYESHOTCELL_H
#define EYESHOTCELL_H

#include "Character.h"
#include "Entity.h"
#include "Item.h"
#include "StateCell.h"
#include <vector>

//
class CEyeshotCell
{
public:
	CEyeshotCell();
	~CEyeshotCell();

	void	AddEntity(CCharacter *pCCha);
	void	AddEntity(CItem *pCItem);
	void	DelEntity(Entity *pCEnt);
	long	GetEntityNum(void);
	long	GetChaNum(void);
	long	GetItemNum(void);

	void	EnterEyeshot(Entity *pCEnt);
	void	OutEyeshot(Entity *pCEnt);
	void	RefreshEyeshot(Entity *pCEnt, bool bToEyeshot, bool bToNoHide, bool bToNoShow);

	void	BeginGetCha(void);
	CCharacter*	GetNextCha(void); //

public:
	short			m_sPosX;	//
	short			m_sPosY;
	long	 	    m_lActNum;	//
	CCharacter		*m_pCChaL;	//
	CItem			*m_pCItemL;	//

	CEyeshotCell	*m_pCNext;	//
	CEyeshotCell	*m_pCLast;

	std::vector<CStateCell**> _stateCellSlots;	// указатели на слоты в StateCellGrid

private:
	long			m_lChaCount;
	long			m_lItemCount;

	CCharacter		*m_pCChaSearch;

};

//=============================================================================
//
class CActEyeshotCell
{
public:
	void			Add(CEyeshotCell *pObj);
	void			Del(CEyeshotCell *pObj);

	void			BeginGet(void); //
	CEyeshotCell*	GetNext(void); //
	CEyeshotCell*	GetCurrent(void);

	long			GetActiveNum(void);

private:
	CEyeshotCell* m_pHead{};
	CEyeshotCell* m_pCur{};
	long          m_lCount{};
};

#endif // EYESHOTCELL_H
