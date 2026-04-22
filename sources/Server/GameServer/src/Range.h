// Геометрические примитивы для проверки попадания в область.
// 0 — направление по вертикали (север).

#pragma once

#ifndef _RANGE_H_
#define _RANGE_H_

#define RADIAN  57.296f  //  = 180.0 / 3.1415926

// 0 ~ 360
float Tan2Angle(int nOffX, int nOffY, float fTan);

BOOL  LineIntersection(float* pv1, float* pv2, float* pv3, float* pv4, BOOL bline);

// --------------------------------------------------------------------------------------------

class CRangeBase
{
public:
	CRangeBase();

	void setPos(int x, int y);
	void setRadius(int nRadius);
	void setAngle(int nAngle);

	int  getX();
	int  getY();
	int  getRadius();
	int  getAngle();

	virtual BOOL PointHitTest(int x, int y);
	virtual BOOL RectHitTest(int x1, int y1, int x2, int y2);

protected:
	virtual void _UpdatePos();
	virtual void _UpdateAngle();
	virtual void _UpdateRadius();

	int _x;
	int _y;
	int _nRadius;
	int _nAngle;
};


class CRangeLine : public CRangeBase
{
public:
	CRangeLine();

	int  getEndX();
	int  getEndY();

	BOOL LineHitTest(int x1, int y1, int x2, int y2); //
	BOOL RectHitTest(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);

protected:
	void         _CalEndPos();

	virtual void _UpdatePos() override;
	virtual void _UpdateAngle() override;
	virtual void _UpdateRadius() override;

	int _x2;
	int _y2;
};


class CRangeFan : public CRangeBase // ( : 0, )
{
public:
	CRangeFan();

	virtual BOOL PointHitTest(int x, int y) override;                                                    //
	virtual BOOL RectHitTest(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
	virtual BOOL RectHitTest(int x1, int y1, int x2, int y2) override;

	void CalEndPos(int& x1, int& y1, int& x2, int& y2);

	void setAngleStep(int nAngleStep); //
	int  getAngleStep();

protected:
	virtual void _UpdateRadius() override;

	int _nAngleStep;
	int _nTempRadius;
};


class CRangeStick : public CRangeBase // ()
{
public:
	CRangeStick();

	virtual BOOL PointHitTest(int x, int y) override;
	virtual BOOL RectHitTest(int x1, int y1, int x2, int y2) override;

	void setWidth(int nWidth);
	int  getWidth();

	void getPoint(int& x1, int& y1, int& x2, int& y2, int& x3, int& y3, int& x4, int& y4);

protected:
	float _Point2Angle(int x, int y);

	int _nWidth;
};


class CRangeCircle : public CRangeBase //
{
public:
	virtual BOOL PointHitTest(int x, int y) override;
	virtual BOOL RectHitTest(int x1, int y1, int x2, int y2) override;
};

class CRangeSquare : public CRangeBase //
{
public:
	virtual BOOL PointHitTest(int x, int y) override;
	virtual BOOL RectHitTest(int x1, int y1, int x2, int y2) override;
};

#endif // _RANGE_H_
