//=============================================================================
// FileName: Action.h
// Creater: ZhangXuedong
// Date: 2004.10.08
// Comment: CAction class
//=============================================================================

#ifndef ACTION_H
#define ACTION_H


#include "GameAppNet.h"
#include "Entity.h"
#include "MoveAble.h"
#include "FightAble.h"

#define defMAX_ACTION_NUM	2

class CAction
{
public:
	struct SActionQueue
	{
		dbc::Short	sType;
		void		*pInit;
	};

public:
	CAction(Entity *);

	bool Add(dbc::Short sActionType, void *pActionData);
	bool DoNext(dbc::Short sActionType = 0, dbc::Short sActionState = 0);
	void End();
	void Interrupt();

	dbc::Short GetActionNum() {return m_sActionNum;}
	dbc::Short GetCurActionNo() {return m_sCurAction;}
	bool Has(dbc::Short sActionType, void *pActionData);

protected:

private:
	Entity			*m_pCEntity;
	SActionQueue	m_SAction[defMAX_ACTION_NUM];
	dbc::Short		m_sActionNum;
	dbc::Short		m_sCurAction;

	CMoveAble::SMoveInit m_SMoveInit;
	SFightInit			 m_SFightInit;

};

#define defMAX_CACHE_ACTION_PARAM_LEN	30

enum ECacheActionType
{
	enumCACHEACTION_MOVE,
	enumCACHEACTION_SKILL,
	enumCACHEACTION_SKILL2,
};

class CCharacter;
class CActionCache
{
public:
	struct SAction
	{
		dbc::Short	sCommand;
		dbc::Char	szParam[defMAX_CACHE_ACTION_PARAM_LEN];
		dbc::Char	chParamPos;

		SAction	*pSNext;
	};

	CActionCache(CCharacter *pCOwn);
	~CActionCache();

	void	AddCommand(dbc::Short sCommand);
	void	PushParam(void *pParam, dbc::Char chSize);
	void	Run(void);
	void	ExecAction(SAction *pSCarrier);

protected:

private:
	CCharacter	*m_pCOwn;

	SAction	*m_pSExecQueue;	// 
	SAction	*m_pSFreeQueue;	// 

};

#endif // ACTION_H
