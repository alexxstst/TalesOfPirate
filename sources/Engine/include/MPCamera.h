#include "mygraph.h"
// #include "pos_state.h"
#include "i_effect.h"


#ifndef TAG_CAMERA
#define TAG_CAMERA

#define MOVE_LEFT			1
#define MOVE_RIGHT			2
#define MOVE_UP				3	
#define MOVE_DOWN			4
#define ROTATE_VEER			5
#define ROTATE_REVERSE		6
#define MOVE_FORWARD		7
#define MOVE_BACKWARD		8
#define ROLL1				9
#define ROLL2				10


class MPCameraNOLEECH
{ 
public:
	
	// 
	D3DXVECTOR3	m_EyePos;
	D3DXVECTOR3	m_RefPos;
	
	D3DXVECTOR3 m_vRoll;

	
	float m_fRoll;
	
	TCHAR	strCameraInfo[64];
	
	VOID	InitPosition(float ex , float ey , float ez , float rx , float ry , float rz)
	{
		m_EyePos.x = ex;
		m_EyePos.y = ey;
		m_EyePos.z = ez;
		m_RefPos.x = rx;
		m_RefPos.y = ry;
		m_RefPos.z = rz;
	}
	VOID	Move(DWORD dwMoveType);
	D3DXVECTOR3 GetRollVector()
	{
		D3DXMATRIX mat;
		D3DXMatrixRotationY(&mat , D3DXToRadian(m_fRoll));
	
		D3DXVECTOR4 v;
		const auto v2 = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		D3DXVec3Transform(&v , &v2 , &mat);
		
		m_vRoll = (D3DXVECTOR3)v;
		return m_vRoll;
	}

	void	MoveForward(float fStep, BOOL bHang = TRUE);
	void	MoveRight(float fStep, BOOL bHang = TRUE);
	void	Turn(float fStep, D3DXVECTOR3 *pFocusVec = NULL);
	

	virtual void			FrameMove(DWORD	dwTailTime);
	MPCameraNOLEECH();
	~MPCameraNOLEECH();

////lemon add@ 2004.8.4////////////////////////////////////////////////////////////////////////
//
//	struct CameraModel
//	{
//		CameraModel()
//		{
//			//m_fminxy = 14.0f;
//			//m_fmaxxy = 35.0f;
//			//m_fminHei = 14.0f;
//			//m_fmaxHei = 27.0f;
//			//m_fminfov = 15.0f;
//			//m_fmaxfov = 30.0f;
//
//			m_fstep1 = 0;
//
//			m_stDist = 0;
//			m_enDist = 0;
//
//			m_stHei = 0;
//			m_enHei = 0;
//
//			m_stFov = 0;
//			m_enFov = 0;
//
//			m_fxy = 0;
//			m_fz = 0;
//			m_fAngle = 0;
//			m_ffov = 0;
//
//			m_fvel = 0.000001f;
//			m_fasscs = 0;
//			m_bFB = false;
//
//			m_fRotaVel = 0.000001f;
//			m_fRotaAss = 0;
//			m_bRota    = false;
//
//			m_bReset = false;
//
//			m_InitAngle = 0;
//
//			m_fResetRotatVel = 2.7f;
//			m_fResetRotatAccl = 0.005f;
//		}
//		D3DXVECTOR3	m_vDir;
//		D3DXVECTOR3	m_vCross;
//
//		float	m_fxy;
//		float	m_fz;
//		float	m_fAngle;
//		float	m_ffov;
//
//		float   m_fstep1;
//
//
//		float   m_stDist;
//		float	m_enDist;
//		float	m_stHei;
//		float	m_enHei;
//		float	m_stFov;
//		float	m_enFov;
//
//		//float	m_fminxy ;
//		//float   m_fmaxxy ;
//		//float	m_fminHei;
//		//float   m_fmaxHei;
//		//float   m_fminfov;
//		//float   m_fmaxfov;
//
//		float					m_fvel;
//		float					m_fasscs;
//		bool					m_bFB;
//
//		float					m_fRotaVel;
//		float					m_fRotaAss;
//		bool					m_bRota;
//
//		bool					m_bReset;
//		float					m_InitAngle;
//		float					m_fResetRotatVel;
//		float					m_fResetRotatAccl;		
//
//
//		void	ScrollUD(float fStep)
//		{
//		}
//		void	ScroolLR(float fstep)
//		{
//			m_fAngle = fstep > D3DX_PI ? fstep - D3DX_PI : fstep;
//			D3DXMATRIX		mat;
//			D3DXVECTOR4		ver;
//			D3DXMatrixRotationZ(&mat,m_fAngle);
//			D3DXVec3Transform(&ver, &m_vDir, &mat);
//			m_vDir.x = ver.x;
//			m_vDir.y = ver.y;
//			m_vDir.z = 0;
//		}
//		bool	ScroolFB(float fStep)
//		{
//			m_fstep1 += fStep;
//			if(m_fstep1 > 1.0f)
//			{
//				m_fstep1 = 1.0f;
//				return false;
//			}
//			if(m_fstep1 < 0.0f)
//			{
//				m_fstep1 = 0.0f;
//				return false;
//			}
//			m_fxy = m_stDist *(1.0f - m_fstep1) + m_enDist * m_fstep1;
//
//			m_fz = m_stHei *(1.0f - m_fstep1) + m_enHei * m_fstep1;
//
//			m_ffov = m_stFov *(1.0f - m_fstep1) + m_enFov * m_fstep1;
//			return true;
//		}
//		void	GetEyePos(D3DXVECTOR3& vpos,D3DXVECTOR3& vRefPos)
//		{
//			m_vDir.z = 0;
//			D3DXVec3Add(&vpos,&vRefPos,&(m_vDir * m_fxy));
//			vpos.z = m_fz+ vRefPos.z;
//		}
//	}m_cameractrl;
//
//	void			SetFollowObj(D3DXVECTOR3&	vRefObj)
//	{
//		m_RefPos  = vRefObj;
//	}
//	void			InitAngle(float	fAngle);
//	void			FrameMove(DWORD	dwTailTime);
//
//	void			MoveForwardBack(float	fDist);
//	void			RotationCameraLR(float Angle);
//
//	void			RotationCameraUD(float Angle);
//
//	void			ResetCamera(float fAngle);
};

#endif
