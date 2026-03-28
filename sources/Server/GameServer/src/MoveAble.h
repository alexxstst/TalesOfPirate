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
		// �˽ṹ������ͬSFightInit�Ķ�Ӧ�ṹ
		struct
		{
			dbc::Char		chType;	// 0����Ŀ�ꡣ1��Ŀ����ʵ�塣2��Ŀ��������
			dbc::Long		lInfo1;
			dbc::Long		lInfo2;
		};
		//
		dbc::uLong	ulDist;		// ��Ŀ��ľ��루���ף�
	};

	struct SMoveInit
	{
		//dbc::uLong	ulSpeed;		// �ƶ��ٶȣ�����/�룩
		dbc::uShort	usPing;			// һ���������ص�ʱ��

		SPointList	SInflexionInfo;	// ��Ҫִ���ƶ��ĵ�����
		STarget		STargetInfo;	// �ƶ���Ŀ����Ϣ

		dbc::Short	sStopState;		// �ƶ�ֹͣ���״̬��enumEXISTS_WAITING, enumEXISTS_SLEEPING��
	};

	struct SMoveProc
	{
		dbc::Short	sCurInflexion;	// ��ǰת���

		dbc::uLong	ulElapse;		// Ԥ�ƶ���ʱ�����룩
		dbc::uLong	ulCacheTick;

		dbc::Short	sState;		// �μ�CompCommand.h�е�EMoveStateö������
		dbc::Char	chRequestState;	// ������ƶ�״̬��0��������1������ֹͣ�ƶ���2������ʼ�ƶ�
		dbc::Char	chLagMove;		// 0���������ƶ�����1������

		SPointList	SNoticePoint;	// ͨ����ն˵ĵ�����
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
	virtual void AfterStepMove(void){}; // ������ͼ�л����ú���û�з���AttemptMove�����У�����ΪAfterStepMove���ܽ��е�ͼ�л������µ�ǰ��ͼΪ�գ���AttemptMove֮����ܽ�����Ұͨ�棬��ͼ����Ϊ��
	virtual void SubsequenceMove(){};

	void BeginMove(dbc::uLong ulElapse = 0);
	void EndMove();
	void OnMoveBegin(void) {m_bOnMove = true;}
	void OnMoveEnd(void) {m_bOnMove = false;}

	bool AreaOverlap(long& xdist,long& ydist);

	Point NearlyPointFromPointToLine(const Point *pPort1, const Point *pPort2, const Point *pCenter);
	bool SegmentEnterCircle(Point *pSPort1, Point *pSPort2, Circle *pSCircle, Point *pResult);

	dbc::uShort	m_usHeartbeatFreq;	// �ƶ�ִ�е�������Ƶ�ʣ�����λ�����룩
	dbc::uLong	m_ulHeartbeatTick;	// ��λ�����룩
	bool		m_bOnMove;

	CTimer		m_timeRun;

};

#endif // MOVEABLE_H
