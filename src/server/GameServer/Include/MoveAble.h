#pragma once

//=============================================================================
// FileName: MoveAble.h
// Creater: ZhangXuedong
// Date: 2004.11.03
// Comment: CMoveAble class
//=============================================================================

#ifndef MOVEABLE_H
#define MOVEABLE_H

#include "FightAble.h"

#define defMOVE_INFLEXION_NUM	32
#define defPOS_QUEUE_MEMBER_NUM	32

class	CMoveAble : public CFightAble
{
public:
	struct SPointList
	{
		Point		SList[defMOVE_INFLEXION_NUM];
		dbc::Short	sNum;
	};

	struct STarget
	{
		// SFightInit
		struct
		{
			dbc::Char		chType;	// 012
			dbc::Long		lInfo1;
			dbc::Long		lInfo2;
		};
		//
		dbc::uLong	ulDist;		// 
	};

	struct SMoveInit
	{
		//dbc::uLong	ulSpeed;		// /
		dbc::uShort	usPing;			// 

		SPointList	SInflexionInfo;	// 
		STarget		STargetInfo;	// 

		dbc::Short	sStopState;		// enumEXISTS_WAITING, enumEXISTS_SLEEPING
	};

	struct SMoveProc
	{
		dbc::Short	sCurInflexion;	// 

		dbc::uLong	ulElapse;		// 
		dbc::uLong	ulCacheTick;

		dbc::Short	sState;		// CompCommand.hEMoveState
		dbc::Char	chRequestState;	// 012
		dbc::Char	chLagMove;		// 01

		SPointList	SNoticePoint;	// 
	};

	struct SMoveRedundance
	{
		dbc::uLong	ulStartTick;
		dbc::uLong	ulLeftTime;
	};

	SMoveInit	m_SMoveInit;
	SMoveInit	m_SMoveInitCache;
	SMoveProc	m_SMoveProc;
	SMoveRedundance	m_SMoveRedu;
	dbc::Long	m_lSetPing;

	CMoveAble();

	bool	DesireMoveBegin(SMoveInit *pSMove);
	void	DesireMoveStop() {EndMove();};
	void	OnMove(dbc::uLong ulTimePrecision);

	dbc::Char AttemptMove(double dDistance, bool bNotiInflexion = true);
	dbc::Char LinearAttemptMove(Point STar, double distance, dbc::uLong *ulElapse);

	void	ResetMove();

protected:
	void	Initially();
	void	Finally();
	void	WritePK(WPACKET& wpk);
	void	ReadPK(RPACKET& rpk);

	virtual bool	overlap(long& xdist,long& ydist);
	virtual CMoveAble* IsMoveAble() override { return this; }

	void	NotiSelfMov();
	void	NotiMovToEyeshot();

	bool		GetMoveTargetShape(Square *pSTarShape);
	dbc::Short	GetMoveState() {return m_SMoveProc.sState;}
	void		SetMoveState(dbc::Short sState) {m_SMoveProc.sState = sState;}
	dbc::Short	GetMoveStopState(void) {return m_SMoveInit.sStopState;}
	const Point	&GetMoveEndPos(void) {return m_SMoveInit.SInflexionInfo.SList[m_SMoveInit.SInflexionInfo.sNum - 1];}
	bool		SetMoveOnInfo(SMoveInit* pSMoveI);

private:
	virtual void AfterStepMove(void){}; // AttemptMoveAfterStepMoveAttemptMove
	virtual void SubsequenceMove(){};

	void BeginMove(dbc::uLong ulElapse = 0);
	void EndMove();
	void OnMoveBegin(void) {m_bOnMove = true;}
	void OnMoveEnd(void) {m_bOnMove = false;}

	bool AreaOverlap(long& xdist,long& ydist);

	Point NearlyPointFromPointToLine(const Point *pPort1, const Point *pPort2, const Point *pCenter);
	bool SegmentEnterCircle(Point *pSPort1, Point *pSPort2, Circle *pSCircle, Point *pResult);

	dbc::uShort	m_usHeartbeatFreq;	// 
	dbc::uLong	m_ulHeartbeatTick;	// 
	bool		m_bOnMove;

	CTimer		m_timeRun;

};

#endif // MOVEABLE_H
