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
		// 此结构及含义同SFightInit的对应结构
		struct
		{
			dbc::Char		chType;	// 0，无目标。1，目标是实体。2，目标是坐标
			dbc::Long		lInfo1;
			dbc::Long		lInfo2;
		};
		//
		dbc::uLong	ulDist;		// 对目标的距离（厘米）
	};

	struct SMoveInit
	{
		//dbc::uLong	ulSpeed;		// 移动速度（厘米/秒）
		dbc::uShort	usPing;			// 一个网络来回的时间

		SPointList	SInflexionInfo;	// 需要执行移动的点序列
		STarget		STargetInfo;	// 移动的目标信息

		dbc::Short	sStopState;		// 移动停止后的状态（enumEXISTS_WAITING, enumEXISTS_SLEEPING）
	};

	struct SMoveProc
	{
		dbc::Short	sCurInflexion;	// 当前转弯点

		dbc::uLong	ulElapse;		// 预移动耗时（毫秒）
		dbc::uLong	ulCacheTick;

		dbc::Short	sState;		// 参见CompCommand.h中的EMoveState枚举类型
		dbc::Char	chRequestState;	// 请求的移动状态：0，无请求。1，请求停止移动。2，请求开始移动
		dbc::Char	chLagMove;		// 0，不缓存移动请求。1，缓存

		SPointList	SNoticePoint;	// 通告给终端的点序列
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
	void	ReadPK(RPACKET rpk);

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
	virtual void AfterStepMove(void){}; // 处理地图切换，该函数没有放在AttemptMove例程中，是因为AfterStepMove可能进行地图切换而导致当前地图为空，而AttemptMove之后可能进行视野通告，地图不能为空
	virtual void SubsequenceMove(){};

	void BeginMove(dbc::uLong ulElapse = 0);
	void EndMove();
	void OnMoveBegin(void) {m_bOnMove = true;}
	void OnMoveEnd(void) {m_bOnMove = false;}

	bool AreaOverlap(long& xdist,long& ydist);

	Point NearlyPointFromPointToLine(const Point *pPort1, const Point *pPort2, const Point *pCenter);
	bool SegmentEnterCircle(Point *pSPort1, Point *pSPort2, Circle *pSCircle, Point *pResult);

	dbc::uShort	m_usHeartbeatFreq;	// 移动执行的心跳（频率），单位（毫秒）
	dbc::uLong	m_ulHeartbeatTick;	// 单位（毫秒）
	bool		m_bOnMove;

	CTimer		m_timeRun;

};

#endif // MOVEABLE_H
