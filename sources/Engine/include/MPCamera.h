#include "mygraph.h"
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
//		CameraModel()
//				return false;
//				return false;
//			return true;
};

#endif
