#pragma once

#include "mpcamera.h"

struct SCameraMode;

//#ifdef TESTDEMO
//////////////////////////////////////////////////////////////////////////

//class CCameraCtrl : public MPCamera
//{
//public:
//	CCameraCtrl()
//	{
//		m_stDist = 10;
//		m_enDist = 1;
//		m_fAngle = INIT_CAMERA_ANGLE;
//		m_stHei = D3DX_PI/2;
//		m_enHei = 0;
//		m_curHei = 0;
//	}
//	~CCameraCtrl(){}
//	void			InitModel(int iType = 0, D3DXVECTOR3* pInitPos = NULL)
//	{
//	}
//	void			SetModel(int iType = 0, D3DXVECTOR3* pInitPos = NULL){}
//	void			ChangeModel(){}
//
//	void			EnableRotation(BOOL	benable){}
//	void			EnableUpdown(BOOL	benable){}
//
//	bool			IsDefaultView(){}
//
//	bool			IsRotating(){ return m_bRota;}
//
//	void			FrameMove(DWORD	dwTailTime){}
//
//	void			MoveForwardBack(float	fDist){}
//	void			RotationCameraLR(float Angle){}
//
//	void			RotationCameraUD(float Angle){}
//
//	void			ResetCamera(float fAngle){}
//
//	void			SetFollowObj(D3DXVECTOR3&	vRefObj);
//
//
//	void			Reset()		{ m_iType = -1;}
//
//public:
//	void	GetEyePos(D3DXVECTOR3& vpos,D3DXVECTOR3& vRefPos){}
//	bool	ScroolFB(float fStep){}
//	void	ScroolLR(float fstep){}
//	void	ScrollUD(float fStep){}
//
//	void	ScrollToDefault(){}
//
//protected:
//	float   m_stDist;
//	float	m_enDist;
//	float	m_fAngle;
//	float   m_stHei;
//	float   m_enHei;
//	float	m_curHei;
//};
//////////////////////////////////////////////////////////////////////////

//#else
//////////////////////////////////////////////////////////////////////////

#define INIT_CAMERA_ANGLE		0 // 0.5235987f + 0.0872664f

class CCamDither
{
public:
    MPMatrix44 _InitMat;
    MPMatrix44 _RunningMat;
    MPVector3 _Dir;
    DWORD _FrameNum;
    DWORD _Step;
    BOOL _InitFlag;

private:
    void _A();

public:
    CCamDither();
    void Init(const MPMatrix44* mat);
    MPMatrix44* GetRunningMatrix() { return &_RunningMat; }
    void FrameMove();
    BOOL GetState() { return _InitFlag; }
};

class CCameraCtrl : public MPCameraNOLEECH
{
public:
	CCameraCtrl(void);
	~CCameraCtrl(void);

	void			InitModel(int iType = 0, D3DXVECTOR3* pInitPos = NULL);
	void			SetModel(int iType = 0, D3DXVECTOR3* pInitPos = NULL);
	void			ChangeModel();

	void			EnableRotation(BOOL	benable);
	void			EnableUpdown(BOOL	benable);

	bool			IsDefaultView();

	bool			IsRotating(){ return m_bRota;}

	//void			InitAngle(float	fAngle);
	void			FrameMove(DWORD	dwTailTime);

	void			MoveForwardBack(float	fDist);
	void			RotationCameraLR(float Angle);

	void			RotationCameraUD(float Angle);
	void			Scale(float fDist);

	void			ResetCamera(float fAngle);

	void			SetFollowObj(D3DXVECTOR3&	vRefObj);


	void			Reset()
	{
		m_iType = -1;
	}

void	GetEyePos(D3DXVECTOR3& vpos,D3DXVECTOR3& vRefPos);
	bool	ScroolFB(float fStep);
	void	ScroolLR(float fstep);
	void	ScrollUD(float fStep);

	void	ScrollToDefault();


    void    SetCameraDither(BOOL flag);

	//void	SetTerrainShow(bool bcull);
	int			m_iType;
	SCameraMode* m_pModel;

	BOOL		m_bEnableRota;
	BOOL		m_bEnableUpdown;
	BOOL		m_bEnableTemp;

	BOOL		m_bChangeModel;
	int			m_iShowWidth,m_iShowHeight;

	//float   m_fxyyue ;
	//float	m_fzyue  ;
	//float	m_ffovyue;
	float   m_fxyLast ;
	float	m_fzLast  ;
	float	m_ffovLast;
	float	m_flerp;
	float   m_fScale;

	D3DXVECTOR3	m_vDir;
	D3DXVECTOR3	m_vCross;
	D3DXVECTOR3 m_vViewDir;

	float	m_fxy;
	float	m_fz;
	float	m_fAngle;
	float	m_ffov;

	float   m_fstep1;

	float   m_stDist;
	float	m_enDist;
	float	m_stHei;
	float	m_enHei;
	float	m_stFov;
	float	m_enFov;

	float	m_fvel;
	float	m_fasscs;
	bool	m_bFB;

	float	m_fRotaVel;
	float	m_fRotaAss;
	bool	m_bRota;

	bool	m_bReset;
	float	m_InitAngle;
	float	m_fResetRotatVel;
	float	m_fResetRotatAccl;		

	bool	m_bToDefault;
	D3DXVECTOR3	m_vDefaultDir;
	float   m_fSubVel;
	float   m_fLastAngle;

	CCamDither m_CamDither;
	float	MAX_SCALE;
	float	MIN_SCALE;

//
	void	SetBufVel(DWORD	dwVel, int iCha)
	{
		//m_maxVel = ((float)dwVel ) / 100;
		//m_minVel = 0;//m_maxVel/4;

		m_iCha = iCha;
	}

	void	BeginMove();
	void	EndMove();

	void	InitPos(float fX,float fY,float fZ,bool bRestoreCustom=true);	// bRestoreCustomtrue
	void	AddPoint(float fX,float fY,float fZ);
	void	Update();

	void	SetViewTransform();
	INT							m_iCha;
	D3DXVECTOR3					m_vCurPos;

	std::list<D3DXVECTOR2>		m_listDir;
	std::list<float>			m_listDist;
	float						m_fCurDist;

	std::list<D3DXVECTOR2>		m_listPos;
	float	m_fStackVel;

	float	m_listHei;
	float	m_fHeiVel;
	float	m_fLastHei;


	int		m_iState;

	float	m_fmaxDown;

	D3DXVECTOR2		 _vCurNode;
	D3DXVECTOR2		 _vNextNode;
	D3DXVECTOR2		 _vLastNode;
	float        _fProgressRate;
	float		 _fdistshow;

	D3DXVECTOR2		 _vLastPos;
};

//#endif




namespace Ninja
{

	//
	//  Ninja_Math.h
	//
	const float pi ( 3.14159265358979323846f );
	const float piOver2 ( pi / 2.0f );
	const float squareRootOf2 = 1.4142135623730950488f;

	inline float Angle2Radian ( float angle )	{ return angle * pi / 180.0f; }
	inline float Radian2Angle ( float radian )	{ return radian * 180.0f / pi; }
	inline float sign ( float value )			{ return value > 0 ? 1.f : -1.f; }

	inline float Distance ( float x1, float y1, float x2, float y2 )
	{
		return sqrt ( ( x2 - x1 ) * ( x2 - x1 ) + ( y2 - y1 ) * ( y2 - y1 ) );
	}

	inline BOOL IsDistanceLessThanValue ( const D3DXVECTOR3 &v1,
										  const D3DXVECTOR3 &v2,
										  float value )
	{
		return ( ( v1.x - v2.x ) * ( v1.x - v2.x ) +
				 ( v1.y - v2.y ) * ( v1.y - v2.y ) +
				 ( v1.z - v2.z ) * ( v1.z - v2.z ) < value * value );
	}

	inline BOOL IsDistanceLessThanValue ( const D3DXVECTOR2 &v1,
										  const D3DXVECTOR2 &v2,
										  float value )
	{
		return ( ( v1.x - v2.x ) * ( v1.x - v2.x ) +
				 ( v1.y - v2.y ) * ( v1.y - v2.y ) < value * value );
	}

	inline BOOL IsLengthLessThanValue ( const D3DXVECTOR3 &v,
										float value )
	{
		return ( v.x * v.x + v.y * v.y + v.z * v.z < value * value );
	}

	inline BOOL IsLengthLessThanValue ( const D3DXVECTOR2 &v,
										float value )
	{
		return ( v.x * v.x + v.y * v.y < value * value );
	}

	inline BOOL IsLengthLessThanVector ( const D3DXVECTOR3 &v1,
										 const D3DXVECTOR3 &v2 )
	{
		return ( v1.x * v1.x + v1.y * v1.y + v1.z * v1.z <
				 v2.x * v2.x + v2.y * v2.y + v2.z * v2.z );
	}

	inline BOOL IsLengthLessThanVector ( const D3DXVECTOR2 &v1,
										 const D3DXVECTOR2 &v2 )
	{
		return ( v1.x * v1.x + v1.y * v1.y <
				 v2.x * v2.x + v2.y * v2.y );
	}

	float AngleDistance ( float angle1, float angle2 );
	float DistancePoint2Segment ( const D3DXVECTOR3 &point,
								  const D3DXVECTOR3 &segPoint1,
								  const D3DXVECTOR3 &segPoint2 );


	//
	//  Ninja_Controller.h
	//
	template < class T >
	class Controller
	{
	public :
		virtual T GetValue ( T start, T end, float controll_value ) = 0;
	};

	template < class T >
	class LinearController : public Controller < T >
	{
	public :
		virtual T GetValue ( T start, T end, float controll_value )
		{
			return start + ( end - start ) * controll_value;
		}
	};

	template < class T >
	class SegmentController_Start : public Controller < T >
	{
	public :
		virtual T GetValue ( T start, T end, float controll_value )
		{
			return start;
		}
	};

	template < class T >
	class SegmentController_End : public Controller < T >
	{
	public :
		virtual T GetValue ( T start, T end, float controll_value )
		{
			return end;
		}
	};



	//
	//  Ninja_Camera.h
	//
	typedef struct SphereCoord
	{
		float radius;
		float gamma;
		float theta;
		SphereCoord() 
			: radius ( 0 ),
			gamma ( 0 ),
			theta ( 0 )
		{}

		SphereCoord ( float r, float g, float t )
			: radius ( r ),
			gamma ( g ),
			theta ( t )
		{}

		SphereCoord operator + ( const SphereCoord &sc )
		{
			return SphereCoord ( radius + sc.radius,
								 gamma + sc.gamma,
								 theta + sc.theta );
		}

		SphereCoord operator - ( const SphereCoord &sc )
		{
			return SphereCoord ( radius - sc.radius,
								 gamma - sc.gamma,
								 theta - sc.theta );
		}

		SphereCoord operator * ( float f )
		{
			return SphereCoord ( radius * f,
								 gamma * f,
								 theta * f );
		}
	} SphereCoord;

	class Camera
	{
	public :
		Camera ( Controller < D3DXVECTOR3 > *_tgt_ctrl,
				 Controller < SphereCoord > *_eye_ctrl );

		D3DXVECTOR3& GetEye()
		{
			return _myEye;
		}

		D3DXVECTOR3& GetTarget()
		{
			return _myTarget;
		}

		SphereCoord& GetSphereCoord()
		{
			return _mySC;
		}

		void SetEye ( const D3DXVECTOR3 &v )
		{
			_myEye = v;
		}

		void SetTarget ( const D3DXVECTOR3 &v )
		{
			_myTarget = v;
		}

		void SetSphereCoord ( const SphereCoord &sc )
		{
			_mySC = sc;
		}

		void Rotate_Vertical ( float radian )
		{
			_mySC_End.theta += radian;
		}

		void Rotate_Horizontal ( float radian )
		{
			_mySC_End.gamma += radian;
		}

		void Range ( float radius )
		{
			_mySC_End.radius += radius;
		}

		void UpdateTargetPosition ( const D3DXVECTOR3 &v );
		void UpdateEyePosition();
		void UpdateViewTransform();

	private :
		Controller < SphereCoord > *_my_eyeController;
		Controller < D3DXVECTOR3 > *_my_targetController;
		D3DXVECTOR3 _myEye;
		D3DXVECTOR3 _myTarget;
		SphereCoord _mySC;
		SphereCoord _mySC_End;

		std::pair < float, float > _myTheta_range;
		std::pair < float, float > _myGamma_range;
		std::pair < float, float > _myRadius_range;
	};
}

