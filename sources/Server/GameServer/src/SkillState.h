//=============================================================================
// FileName: SkillState.h
// Creater: ZhangXuedong
// Date: 2005.01.13
// Comment: Skill State
//=============================================================================

#ifndef SKILLSTATE_H
#define SKILLSTATE_H

#include "GameAppNet.h"
#include "SkillStateType.h"
#include "CompCommand.h"
#include "SkillStateRecord.h"

struct SSkillStateUnit
{
	unsigned char	uchReverseID;
	unsigned char	uchStateID;
	unsigned char	uchStateLv;

	char			chCenter;		//
	unsigned char	uchFightID;		//
	char			chObjType;		//
	char			chObjHabitat;	//
	char			chEffType;		//
	//
	unsigned long	ulSrcWorldID;
	long			lSrcHandle;
	//

	long			lOnTick;		// -1>00
	unsigned long	ulStartTick;	//
	unsigned long	ulLastTick;		//

	unsigned char   GetStateID();
	unsigned char   GetStateLv();
};

class CSkillState
{
public:
	CSkillState(unsigned char uchMaxState = AREA_STATE_MAXID);

	void	Init(unsigned char uchMaxState = AREA_STATE_MAXID);
	bool	Add(unsigned char uchFightID, unsigned long ulSrcWorldID, long lSrcHandle, char chObjType, char chObjHabitat, char chEffType,
			unsigned char uchStateID, unsigned char uchStateLv, unsigned long ulStartTick, long lOnTick, char chType, char chWithCenter = 0);
	bool	Del(unsigned char uchStateID);
	void	Reset(void);
	bool	NeedResetState( unsigned char uchStateID );
	bool	HasState(unsigned char uchStateID, unsigned char uchStateLv);
	bool	HasState(unsigned char uchStateID);
	SSkillStateUnit*	GetSStateByID(unsigned char uchStateID);
	SSkillStateUnit*	GetSStateByNum(unsigned char uchNum);
	unsigned char       GetStateNum(void);
	unsigned char       GetReverseID(unsigned char uchStateID);

	void	SetChangeFlag();
	void	ResetChangeFlag();
	void	SetChangeBitFlag(long lBit);
	bool	GetChangeBitFlag(long lBit);
	unsigned char   GetChangeNum(void);

	void	BeginGetState(void);
	SSkillStateUnit*	GetNextState(void);

	bool	WriteState(net::WPacket &pk);

	///       .
	std::vector<net::msg::AStateBeginSeeEntry> BuildStateEntries();

	SSkillStateUnit	*m_pSState[SKILL_STATE_MAXID + 1];
	unsigned char	m_uchStateNum;

private:
	unsigned char	m_uchMaxState;
	SSkillStateUnit	m_SState[SKILL_STATE_MAXID + 1];

	char			m_szChangeFlag[SSTATE_SIGN_BYTE_NUM]; //
	unsigned char	m_uchChangeNum; //
	unsigned char	m_uchCurGetNo;
};

#endif // SKILLSTATE_H
