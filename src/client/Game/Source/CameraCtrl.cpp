#include "Stdafx.h"
#include "cameractrl.h"
#include "GameConfig.h"
#include "GameApp.h"
#include "Character.h"
#include "ChaClientAttr.h"
#include "UISystemForm.h"
// 
CCamDither::CCamDither()
{
    _InitFlag = 0;
    _Step = 0;
}
void CCamDither::_A()
{
    const int T = 6;
    const DWORD N[T] = { 1, 1, 1, 1, 1, 1};
    const float L[T] = { 0.27f, 0.23f, 0.29f, 0.25f, 0.23f, 0.19f};
    const int S[T] = { 1, -1, -1, 1 };

    MPVector3 dir = _Dir;

    if(_InitFlag)
    {
        float d = L[_Step] / N[_Step];

        dir *= (S[_FrameNum / N[_Step]] * d);

        _RunningMat._41 += dir.x;
        _RunningMat._42 += dir.y;
        _RunningMat._43 += dir.z;

        _FrameNum++;
        if(_FrameNum >= N[_Step] * 4)
        {
            if(!lwFloatEqual(_RunningMat._41, _InitMat._41, 1e-2f)
                || !lwFloatEqual(_RunningMat._42, _InitMat._42, 1e-2f)
                || !lwFloatEqual(_RunningMat._43, _InitMat._43, 1e-2f))
            {
                int x = 0;
            }
            _FrameNum = 0;
            if(++_Step >= T)
            {
                _RunningMat = _InitMat;
                _InitFlag = 0;
            }
        }
    }
}
void CCamDither::Init(const MPMatrix44* mat) 
{ 
    _Step = 0;
    _FrameNum = 0;
    if(mat)
    {
        _InitMat = *mat; 
        _RunningMat = _InitMat;
        _Dir = *(MPVector3*)&_RunningMat._11;
        _InitFlag = 1;
    }
    else
    {
        _InitFlag = 0;
    }
}
void CCamDither::FrameMove()
{
    _A();
}

//
CCameraCtrl::CCameraCtrl(void)
{
	m_bToDefault = false;
	m_bReset = false;

	m_listPos.clear();
	m_fStackVel = 0;

	m_listHei = 0;
	m_fHeiVel = 0;

	m_bEnableRota = false;
	m_bEnableUpdown = true;
	m_bEnableTemp = true;

	m_bChangeModel = false;

	m_iType = -1;
	//m_fxy = 0;
	_fdistshow = 6;

	m_InitAngle = 0;//INIT_CAMERA_ANGLE;
	m_fAngle = m_InitAngle;

	m_pModel = NULL;

	m_bRota = FALSE;

	m_fScale = 1.0;

	MAX_SCALE = 1.0f;
	MIN_SCALE = 0.5f;
}

CCameraCtrl::~CCameraCtrl(void)
{
}
void CCameraCtrl::InitModel(const int iType, D3DXVECTOR3* pInitPos) 
{
	m_iType = iType;
	m_pModel = &CameraMode[m_iType];

	m_fxy = m_pModel->m_fmaxxy;
	m_fz = m_pModel->m_fmaxHei;
	m_ffov = m_pModel->m_fmaxfov;

	m_fstep1 = 1.0f;

	m_stDist = m_pModel->m_fminxy;
	m_enDist = m_pModel->m_fmaxxy;
	m_stHei = m_pModel->m_fminHei;
	m_enHei = m_pModel->m_fmaxHei;
	m_stFov = m_pModel->m_fminfov;
	m_enFov = m_pModel->m_fmaxfov;

	m_iShowWidth = m_pModel->m_nShowWidth;
	m_iShowHeight = m_pModel->m_nShowHeight;
	CGameApp::GetCurScene()->GetTerrain()->SetShowSize(
		m_iShowWidth, m_iShowHeight);

	m_InitAngle = 0;
	m_fAngle = m_InitAngle;

	m_vDir = D3DXVECTOR3(0, 1, 0);
	D3DXMATRIX mat;
	D3DXMatrixRotationZ(&mat, m_fAngle);
	D3DXVec3TransformCoord(&m_vDir, &m_vDir, &mat);

	m_vDefaultDir = m_vDir;
	m_fSubVel = 0;
	if (pInitPos) {
		m_RefPos = *pInitPos;
		InitPos(m_RefPos.x, m_RefPos.y, m_RefPos.z);
	}
	GetEyePos(m_EyePos, m_RefPos);
	const auto val = m_EyePos - m_RefPos;
	D3DXVec3Normalize(&m_vDir, &val);
	m_vDir.z = 0;

	m_fResetRotatVel = g_Config.m_fCameraVel;
	m_fResetRotatAccl = g_Config.m_fCameraAccl;

	SetModel(iType, pInitPos);

	m_iState = 2;
}
void		CCameraCtrl::SetModel(int iType, D3DXVECTOR3* pInitPos)
{
	m_bChangeModel = true;

	//if(m_iType == iType)
	//{
	//	//InitModel(iType, NULL);
	//	return;
	//}else
	{
		m_iType = iType;
		m_pModel = &CameraMode[m_iType];

		m_stDist = m_pModel->m_fminxy;
		m_enDist = m_pModel->m_fmaxxy;
		m_stHei  = m_pModel->m_fminHei;
		m_enHei  = m_pModel->m_fmaxHei;
		m_stFov  = m_pModel->m_fminfov;
		m_enFov  = m_pModel->m_fmaxfov;

		m_fSubVel = 0;

		m_fxyLast = m_fxy;
		m_fzLast  = m_fz;
		m_ffovLast= m_ffov;

		m_flerp = 0;
		m_fstep1 = 1.0f;

		m_fResetRotatVel = g_Config.m_fCameraVel;
		m_fResetRotatAccl = g_Config.m_fCameraAccl;

		//if(pInitPos)
		//{
		//	m_RefPos = *pInitPos;
		//	//InitPos(m_RefPos.x,m_RefPos.y,m_RefPos.z);

		//}
		//GetEyePos(m_EyePos,m_RefPos);
		//D3DXVec3Normalize(&m_vDir, &(m_EyePos - m_RefPos));
		//m_vDir.z = 0;
	}
}

void		CCameraCtrl::ChangeModel()
{
	if(m_pModel->m_nShowWidth > m_iShowWidth && m_pModel->m_nShowHeight > m_iShowHeight)
	{
		m_iShowWidth = m_pModel->m_nShowWidth;
		m_iShowHeight = m_pModel->m_nShowHeight;
		g_pGameApp->GetCurScene()->GetTerrain()->SetShowSize(\
			m_iShowWidth,m_iShowHeight);
	}
	m_flerp += 0.02f;
	if(m_flerp >= 1.0f)
	{
		m_bChangeModel = false;
		m_flerp = 1.0f;

		m_iShowWidth = m_pModel->m_nShowWidth;
		m_iShowHeight = m_pModel->m_nShowHeight;
		g_pGameApp->GetCurScene()->GetTerrain()->SetShowSize(\
			m_iShowWidth,m_iShowHeight);

		if(m_bEnableRota != m_pModel->m_bRotate)
		{
			EnableRotation(m_bEnableRota);
		}
	}
	m_fxy = m_fxyLast * (1.0f - m_flerp) + m_enDist * m_flerp;
	m_fz = m_fzLast * (1.0f - m_flerp) + m_enHei * m_flerp;

	m_ffov = float(m_ffovLast * (1.0 - m_flerp) + m_enFov * m_flerp);
}
//void	CCameraCtrl::SetTerrainShow(bool bcull)
//{
//	if(!m_bEnableRota)
//	{
//		g_pGameApp->GetCurScene()->GetTerrain()->m_bCullTile = false;
//		if(m_iType == 0)
//		{
//			g_pGameApp->GetCurScene()->GetTerrain()->SetShowSize(26,26);
//
//		}else
//		{
//			g_pGameApp->GetCurScene()->GetTerrain()->SetShowSize(45,45);
//		}
//		_fdistshow = m_iType == 0 ? 4.0f : 3.0f;
//	}else
//	{
//		g_pGameApp->GetCurScene()->GetTerrain()->m_bCullTile = bcull;
//
//		if(m_iType == 0)
//		{
//			if(g_pGameApp->GetCurScene()->GetTerrain()->m_bCullTile)
//				g_pGameApp->GetCurScene()->GetTerrain()->SetShowSize(38,38);
//			else
//				g_pGameApp->GetCurScene()->GetTerrain()->SetShowSize(36,36);
//		}else
//		{
//			if(g_pGameApp->GetCurScene()->GetTerrain()->m_bCullTile)
//				g_pGameApp->GetCurScene()->GetTerrain()->SetShowSize(60,60);
//			else
//				g_pGameApp->GetCurScene()->GetTerrain()->SetShowSize(55,55);
//		}
//		if(g_pGameApp->GetCurScene()->GetTerrain()->m_bCullTile)
//			_fdistshow = m_iType == 0 ? 5.0f : 12.5f;
//		else
//			_fdistshow = m_iType == 0 ? 6 : 11.0f;
//	}
//}

void CCameraCtrl::EnableRotation(BOOL	benable)
{
	m_bEnableRota = benable;

	if(!benable)
	{
		m_InitAngle = 0;
		ResetCamera(m_InitAngle);

		if(g_pGameApp->GetCurScene())
		{
			if(g_pGameApp->GetCurScene()->GetMainCha())
				if(g_pGameApp->GetCurScene()->GetMainCha()->IsBoat())
					SetModel(2, NULL);
				else
					SetModel(1, NULL);
		}

		// SetModel(2, NULL);
	}else
	{
		m_InitAngle = INIT_CAMERA_ANGLE;
		//SetTerrainShow(g_pGameApp->GetCurScene()->GetTerrain()->m_bCullTile);
		ResetCamera(m_InitAngle);

		if(g_pGameApp->GetCurScene())
		{
			if(g_pGameApp->GetCurScene()->GetMainCha())
				if(g_pGameApp->GetCurScene()->GetMainCha()->IsBoat())
					SetModel(3, NULL);
				else
					SetModel(0, NULL);
		}
	}
}
void CCameraCtrl::EnableUpdown(BOOL	benable)
{
	//ScrollToDefault();
	if(benable)
	{
		m_bEnableTemp = benable;
		m_bEnableUpdown = benable;
	}else
	{
		m_bEnableTemp = benable;
		ResetCamera(m_InitAngle);
	}

}


bool		CCameraCtrl::IsDefaultView()
{
	return ((m_fstep1 == 1.0f)&&fabs(m_fAngle - m_InitAngle) <=0)  ?  true : false;
}

//void		CCameraCtrl::InitAngle(float	fAngle)
//{
//	m_InitAngle = fAngle;
//	m_vDir = D3DXVECTOR3(0,1,0);
//	m_fAngle = fAngle;
//	D3DXMATRIX		mat;
//	D3DXVECTOR4		ver;
//	D3DXMatrixRotationZ(&mat,m_fAngle);
//	D3DXVec3Transform(&ver, &m_vDir, &mat);
//	m_vDir.x = ver.x;
//	m_vDir.y = ver.y;
//	m_vDir.z = ver.z;
//
//	m_vDefaultDir = m_vDir;
//
//	m_fxy = g_Config.m_fmaxxy;
//	m_fz  = g_Config.m_fmaxHei;
//	m_ffov = g_Config.m_fmaxfov;
//	//SetModel(0);
//	m_fSubVel = 0;
//	GetEyePos(m_EyePos,m_RefPos);
//	D3DXVec3Normalize(&m_vDir, &(m_EyePos - m_RefPos));
//	m_vDir.z = 0;
//}

void		CCameraCtrl::ResetCamera(float fAngle)
{
	if(!m_bEnableRota)
		m_InitAngle = 0;
	else
		m_InitAngle = fAngle;
	m_bReset = true;
}

void CCameraCtrl::Scale(float fDist)
{
	m_fScale += fDist / 4; 

	if(m_fScale>MAX_SCALE)
	{
		m_fScale = MAX_SCALE;
	}

	if(m_fScale<MIN_SCALE)
	{
		m_fScale = MIN_SCALE;
	}

	m_ffov = m_stFov *(m_fScale - m_fstep1) + m_enFov * m_fstep1;
}

void		CCameraCtrl::FrameMove(DWORD	dwTailTime)
{
	static DWORD	dwTime = dwTailTime;

	if (g_stUISystem.m_sysProp.m_videoProp.bResolution > 0)
	{
		MAX_SCALE = 1.0f + m_fstep1 * 0.3f;
	}
	else
	{
		MAX_SCALE = 0.8f + m_fstep1 * 0.2f;	
	}

	dwTime = dwTailTime - dwTime;
    if(m_CamDither.GetState())
    {
        m_CamDither.FrameMove();
        return;
    }

	if(m_bChangeModel)
	{
		ChangeModel();
	}else if(m_bFB)
	{
		if(ScroolFB(m_fvel * m_fasscs))
		{
			m_fasscs -= (float)dwTime * 0.0005f;
			if(m_fasscs <= 0)
			{
				m_bFB = false;
				m_fvel = 0;
				if(!m_bEnableTemp)
					m_bEnableUpdown = false;
			}
		}
		else
		{
			m_bFB = false;
			m_fvel = 0;
			if(!m_bEnableTemp)
				m_bEnableUpdown = false;
		}
	}

	if(m_bToDefault)
	{
		//m_bRota = false;

		float angle  = m_fAngle * (1.0f - m_fSubVel) + m_InitAngle * m_fSubVel;

		if(m_fSubVel >= 1.0f )
		{
			m_bToDefault = false;
			m_fAngle = m_InitAngle;

			//SetTerrainShow(g_pGameApp->GetCurScene()->GetTerrain()->m_bCullTile);

		}else
		{
			m_fAngle = angle;

			//if(m_fSubVel  < 0.5f)
			m_fSubVel += 0.015f;
		}

		D3DXMATRIX		mat;
		D3DXVECTOR4		ver;
		D3DXMatrixRotationZ(&mat,angle  - m_fLastAngle);
		D3DXVec3Transform(&ver, &m_vDir, &mat);
		m_vDir.x = ver.x;
		m_vDir.y = ver.y;
		m_vDir.z = 0;

		m_fLastAngle = angle;
	}
	if( m_bRota)
	{
		ScroolLR(m_fRotaVel * m_fRotaAss);

		m_fRotaAss -= (float)dwTime * m_fResetRotatAccl;
		if(m_fRotaAss <= 0)
		{
			m_bRota = false;
			m_fRotaVel = 0;
		}
	}
	
	if(m_bReset)
	{
		ScrollToDefault();
		MoveForwardBack(0.1f);
	}
	//m_EyePos = m_RefPos + vDistFrom;
	GetEyePos(m_EyePos,m_RefPos);

	const auto v = m_RefPos - m_EyePos;
	D3DXVec3Normalize(&m_vViewDir, &v);
	//m_vDir.z = 0;

	const auto v2 = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	D3DXVec3Cross( &m_vCross, &m_vDir, &v2 );

	dwTime = dwTailTime;

}



void	CCameraCtrl::ScrollToDefault()
{
	m_bReset = false;
	m_bToDefault = true;

	int num = int(m_fAngle / (D3DX_PI * 2));
	float fyue = m_fAngle - ((D3DX_PI * 2)* num);
	if(fabs(fyue) > D3DX_PI + m_InitAngle)
	{
		if(m_fAngle > 0)
			fyue = -((D3DX_PI * 2) - fabs(fyue));
		if(m_fAngle < 0)
			fyue = ((D3DX_PI * 2) - fabs(fyue));
	}
	m_fAngle = fyue;

	m_fSubVel = 0.0f;
	m_fLastAngle = m_fAngle;
}

//
void CCameraCtrl::RotationCameraLR(float Angle)
{
	if(!m_bEnableRota)
		return;

	m_bToDefault = false;

	m_bReset = false;

	m_fRotaVel = Angle/10;

	if(!m_bRota)
	{
		m_bRota = true;

		m_fRotaAss = m_fResetRotatVel;

	}else
	{
		if((Angle < 0 && m_fRotaVel >0) || (Angle > 0 && m_fRotaVel <0))
		{
			//m_cameractrl.m_fRotaVel = -m_cameractrl.m_fRotaVel;
		}
		m_fRotaAss = m_fResetRotatVel;

	}

}
//
void CCameraCtrl::RotationCameraUD(float Angle)
{
}

void	CCameraCtrl::MoveForwardBack(float	fDist)
{
	if(!m_bEnableUpdown)
		return;

	m_bReset = false;

	m_fvel = fDist;

	if(!m_bFB )
	{
		m_bFB = true;

		m_fasscs = 0.7f;
	}
	else
	{
		if((fDist < 0 && m_fvel >0) || (fDist > 0 && m_fvel <0))
		{
			m_fvel = -m_fvel;
		}
		m_fasscs = 0.7f;

	}
}
void	CCameraCtrl::GetEyePos(D3DXVECTOR3& vpos,D3DXVECTOR3& vRefPos)
{
	m_vDir.z = 0;
	const auto v = m_vDir * m_fxy;
	D3DXVec3Add(&vpos,&vRefPos,&v);
	vpos.z = m_fz+ vRefPos.z;
}

void	CCameraCtrl::ScrollUD(float fStep)
{
}
void	CCameraCtrl::ScroolLR(float fstep)
{
	float angle = fstep > D3DX_PI ? fstep - D3DX_PI : fstep;
	m_fAngle += angle;
	D3DXMATRIX		mat;
	D3DXVECTOR4		ver;
	D3DXMatrixRotationZ(&mat,angle);
	D3DXVec3Transform(&ver, &m_vDir, &mat);
	m_vDir.x = ver.x;
	m_vDir.y = ver.y;
	m_vDir.z = 0;
}
bool	CCameraCtrl::ScroolFB(float fStep)
{
	m_fstep1 += fStep;
	if(m_fstep1 > 1.0f)
	{
		m_fstep1 = 1.0f;
		return false;
	}
	if(m_fstep1 < 0)
	{
		m_fstep1 = 0;
		return false;
	}
	m_fxy = m_stDist *(1.0f - m_fstep1) + m_enDist * m_fstep1;

	m_fz = m_stHei *(1.0f - m_fstep1) + m_enHei * m_fstep1;

	if(m_fScale > MAX_SCALE)
	{
		m_fScale = MAX_SCALE;
	}
	m_ffov = m_stFov *(m_fScale - m_fstep1) + m_enFov * m_fstep1;
	//m_ffov = m_stFov *(1.0f - m_fstep1) + m_enFov * m_fstep1;
	return true;
}

void	CCameraCtrl::SetFollowObj(D3DXVECTOR3&	vRefObj)
{
	m_RefPos  = vRefObj;
}


//////////////////////////////////////////////////////////////////////////
void CCameraCtrl::InitPos(float fX, float fY, float fZ, bool bRestoreCustom)
{
	m_listPos.clear();
	m_fStackVel = 0;

	m_fHeiVel = 0.05f;

	//m_listHei = fZ;
	//m_fLastHei = fZ;
	
	//m_vCurPos = D3DXVECTOR3(fX, fY, fZ);

	m_fCurDist = 0;
	m_listDir.clear();
	m_listDist.clear();

	_vLastNode.x = fX;
	_vLastNode.y = fY;

	_vNextNode = _vLastNode;
	_vCurNode  = _vLastNode;
	_fProgressRate = 0.0f;

	m_iState = 2;

	m_fmaxDown = 0;

	m_pModel = &CameraMode[m_iType];

	if(bRestoreCustom)
	{
		//m_fxy = m_pModel->m_fmaxxy;
		//m_fz = m_pModel->m_fmaxHei;

		m_listHei = fZ;
		m_fLastHei = fZ;

		m_vCurPos = D3DXVECTOR3(fX, fY, fZ);

		m_pModel = &CameraMode[m_iType];
		m_fxy = m_pModel->m_fmaxxy;
		m_fz  = m_pModel->m_fmaxHei;
		m_ffov   = m_pModel->m_fmaxfov;
	}
	//m_ffov = m_pModel->m_fmaxfov;
}

void	CCameraCtrl::AddPoint(float fX,float fY,float fZ)
{
	static float ftime  = 0;
	static float ftime2  = 0;

	ftime +=  *ResMgr.GetDailTime();
	if(ftime > 1.5f)
	{
		float d = fZ - m_fmaxDown;

		if(d > 1.5f)
			d = 1.5f;
		else
			d = fZ;

		m_fLastHei = d;

		ftime = 0;
	}

	D3DXVECTOR2 p(fX,fY);
	D3DXVECTOR2 d;
	D3DXVECTOR2 Dir;
	float fDis;
	D3DXVECTOR2 last;
	float fstep = 0.2f;

	if (!m_listPos.empty())
	{
		last = m_listPos.back();

		Dir = p - last;
		fDis = D3DXVec2Length(&Dir);
		D3DXVec2Normalize(&Dir, &Dir);

		if (fDis < 0.04f)
		{
			return;
		}
	}

	m_listPos.push_back(p);
}
void	CCameraCtrl::Update()
{
	//
	if(m_fLastHei != m_listHei)
	{
		float dist = m_fLastHei - m_listHei;
		if(fabs(dist) < m_fHeiVel)
			m_listHei += dist;
		else
			m_listHei += dist < 0 ? -m_fHeiVel : m_fHeiVel;
	}



	////
	//if(m_fTotalLen >= 1.0f) 
	//{
	//		//m_fStackAccel += 0.4f;
	//		//if(m_fStackAccel > 1.4f) 
	//		//	m_fStackAccel = 1.4f;

	//	m_fStackVel += m_maxVel/8;//dwTime   * 0.0001f;
	//	if(m_fStackVel > m_maxVel - m_maxVel/8)
	//		m_fStackVel = m_maxVel -m_maxVel/8;

	//	//m_iState = 1;
	//}
	//
	//if(m_iState == 2)
	//{
	//	//m_fStackAccel += 0.2f;
	//	//if(m_fStackAccel > 0.6f) 
	//	//	m_fStackAccel = 0.6f;

	//	m_fStackVel-=  m_maxVel/8 ;//dwTime   * 0.0001f;
	//	if(m_fStackVel <  m_maxVel/10) 
	//		m_fStackVel =   m_maxVel/10;

	//}
	////
	//D3DXVECTOR2 vpos, vnext;
	//if(m_listDir.size() > 0)
	//{
	//	m_fCurDist += m_fStackVel * *ResMgr.GetDailTime();

	//	vpos = m_listPos.front();

	//	//int nes = 0;
	//	while (m_fCurDist >= m_listDist.front())
	//	{
	//		m_fCurDist -= m_listDist.front();
	//		m_listPos.pop_front();
	//		vpos = m_listPos.front();

	//		m_fTotalLen -= m_listDist.front();

	//		//if(m_iState == 1)
	//		//{
	//		//	//if (m_fTotalLen < 1.0f)
	//		//	//{
	//		//	//	m_fCurDist = 0;
	//		//	//	m_fStackVel-=  m_maxVel/8 ;//dwTime   * 0.0001f;
	//		//	//	if(m_fStackVel <  m_maxVel/8) 
	//		//	//		m_fStackVel =   m_maxVel/8;
	//		//	//}
	//		//}

	//		m_listDist.pop_front();
	//		m_listDir.pop_front();


	//		if(m_listDir.size() == 0)
	//		{
	//			m_vCurPos.x = m_listPos.front().x;
	//			m_vCurPos.y = m_listPos.front().y;
	//			m_vCurPos.z = m_listHei; 
	//			m_fCurDist = 0;
	//			m_fStackVel = 0;
	//			m_fStackAccel = 0;
	//			m_iState = 0;
	//			return;
	//		}
	//	}


	//	vpos += m_listDir.front() * m_fCurDist;

	//	m_vCurPos.x = vpos.x;
	//	m_vCurPos.y = vpos.y;
	//	m_vCurPos.z = m_listHei;

	//}else
	//{
	//	m_vCurPos.x = m_listPos.front().x;
	//	m_vCurPos.y = m_listPos.front().y;
	//	m_fStackVel = 0;
	//	m_fStackAccel = 0;

	//	//m_iState = 0;

	//	//return;
	//}
	//int nPointSize = (int)(m_listPos.size());
	

	D3DXVECTOR2 vpos, vnext;


	while(_fProgressRate > 1.0f)
	{
		_vCurNode  = _vNextNode;
		_vLastNode = _vNextNode;

		//m_listDist.pop_front();
		//m_listDir.pop_front();

		if(m_listPos.size() > 0)  
		{
			_vNextNode = m_listPos.front();
			m_listPos.pop_front();
			_fProgressRate = _fProgressRate - 1.0f;
		}
		else
		{
			_vNextNode = _vCurNode;
			_vLastNode = _vCurNode;
			_fProgressRate = 0.0f;
			m_fStackVel = 0;
		}
	}

	_vCurNode.x =  _vLastNode.x * (1.0f - _fProgressRate) + _vNextNode.x *_fProgressRate;
	_vCurNode.y =  _vLastNode.y * (1.0f - _fProgressRate) + _vNextNode.y *_fProgressRate;

	if(m_iState == 1)
	{
		if(m_listPos.size() > 3) 
			m_fStackVel += 0.1f;
		if(m_fStackVel > 1.0f) m_fStackVel = 1.0f;
	}
	if(m_iState == 2)
	{
		m_fStackVel -= 0.1f;
		if(m_fStackVel < 0.1f) m_fStackVel = 0.1f;
	}

	_fProgressRate += m_fStackVel;

	m_vCurPos.x = _vCurNode.x;
	m_vCurPos.y = _vCurNode.y;

	m_vCurPos.z = m_listHei;

}


void	CCameraCtrl::BeginMove()
{
	m_iState = 1;
}

void	CCameraCtrl::EndMove()
{
	m_iState = 2;
	//m_listPos.push_back(_vLastPos);
}
//////////////////////////////////////////////////////////////////////////

void	CCameraCtrl::SetViewTransform()
{
	//g_Render.SetWorldViewFOV(Angle2Radian(m_ffov));
	D3DXVECTOR3 vRef(m_RefPos);
#ifdef TESTDEMO

	if(g_pGameApp->GetCurScene())
	{
		if(g_pGameApp->GetCurScene()->GetMainCha())
		{
			//CChaRecord* pInfo = GetChaRecordInfo( g_pGameApp->GetCurScene()->GetMainCha()->GetTypeID() );
			//if( pInfo ) 
			{
				vRef.z += 1.2f;//pInfo->fHeight / 2;
			}
		}
	}
#endif

    if(m_CamDither.GetState())
    {
        MPMatrix44 mat = *m_CamDither.GetRunningMatrix();
        lwMatrix44Inverse(&mat, NULL, &mat);
        g_Render.SetTransformView(&mat);
    }
    else
    {
        g_Render.LookAt(m_EyePos, vRef);
        g_Render.SetCurrentView(MPRender::VIEW_WORLD);
    }
}

void CCameraCtrl::SetCameraDither(BOOL flag)
{
    if(flag)
    {
        MPMatrix44 mat;
        mat = *g_Render.GetInterfaceMgr()->dev_obj->GetMatView();
        lwMatrix44Inverse(&mat, NULL, &mat);
        m_CamDither.Init(&mat);
    }
    else
    {
        m_CamDither.Init(0);
    }
}

/************************************************************************/
/*                                                                      */
/************************************************************************/




namespace Ninja
{

	//
	//  Ninja_Math.cpp
	//
	float AngleDistance ( float angle1, float angle2 )
	{
		float angleDistance = angle1 - angle2;

		while ( angleDistance > 180.0f )
		{
			angleDistance -= 360.0f;
		}

		while ( angleDistance < -180.0f )
		{
			angleDistance += 360.0f;
		}

		return angleDistance;
	}

	float DistancePoint2Segment ( const D3DXVECTOR3 &point,
								  const D3DXVECTOR3 &segPoint1,
								  const D3DXVECTOR3 &segPoint2 )
	{
		const auto point2seg1 ( segPoint1 - point );
		const auto point2seg2 ( segPoint2 - point );
		const auto segment ( segPoint2 - segPoint1 );
		const auto nsegment(-segment);

		if ( D3DXVec3Dot ( &point2seg1, &nsegment ) < 0.f )
		{
			return D3DXVec3Length ( &point2seg1 );
		}
		else if ( D3DXVec3Dot ( &point2seg2, &( segment ) ) < 0.f )
		{
			return D3DXVec3Length ( &point2seg2 );
		}
		else
		{
			D3DXVECTOR3 crossProduct;
			D3DXVec3Cross ( &crossProduct, &point2seg1, &point2seg2 );
			return D3DXVec3Length ( &crossProduct ) * 2.f / D3DXVec3Length ( &segment );
		}
	}



	//
	//  Ninja_Camara.cpp
	//
	Camera::Camera ( Controller < D3DXVECTOR3 > *_tgt_ctrl,
					 Controller < SphereCoord > *_eye_ctrl )
	: 
	_my_targetController ( _tgt_ctrl ),
	_my_eyeController ( _eye_ctrl ),
	_mySC_End ( 45.f, 0.985398f, 0.0f ),
	_myTheta_range ( -FLT_MAX, FLT_MAX ),
	_myGamma_range ( Ninja::pi * 0.1f, Ninja::pi * 0.25f ),
	_myRadius_range ( 20.f, 45.f )
	{
	}

	void Camera::UpdateViewTransform()
	{
		// Engine interface
		g_Render.LookAt ( _myEye, _myTarget );
		g_Render.SetCurrentView ( MPRender::VIEW_WORLD );
	}

	void Camera::UpdateTargetPosition ( const D3DXVECTOR3 &v )
	{
		// Get value from controller
		//_myTarget = _my_targetController->GetValue ( _myTarget, v, 0.15f );

		const auto v2 = _myTarget - v;
		if (D3DXVec3Length(&v2) < 50.f)
		{
			_myTarget = _my_targetController->GetValue(_myTarget, v, 0.15f);
		}
		else	// 
		{
			_myTarget = v;
		}
	}

	void Camera::UpdateEyePosition()
	{
		// Check data validity
		if ( _mySC_End.theta > _myTheta_range.second )
			_mySC_End.theta = _myTheta_range.second;
		if ( _mySC_End.theta < _myTheta_range.first )
			_mySC_End.theta = _myTheta_range.first;

		if ( _mySC_End.gamma > _myGamma_range.second )
			_mySC_End.gamma = _myGamma_range.second;
		if ( _mySC_End.gamma < _myGamma_range.first )
			_mySC_End.gamma = _myGamma_range.first;

		if ( _mySC_End.radius > _myRadius_range.second )
			_mySC_End.radius = _myRadius_range.second;
		if ( _mySC_End.radius < _myRadius_range.first )
			_mySC_End.radius = _myRadius_range.first;

		if ( _mySC_End.gamma > Ninja::pi || _mySC_End.gamma < 0.f )
		{
			if ( _mySC_End.gamma > Ninja::pi )
			{
				_mySC_End.gamma = Ninja::pi * 2.f - _mySC_End.gamma;
			}
			if ( _mySC_End.gamma < 0.f )
			{
				_mySC_End.gamma = -_mySC_End.gamma;
			}
			_mySC_End.theta += Ninja::pi;
		}

		// Get value from controller
		_mySC = _my_eyeController->GetValue ( _mySC, _mySC_End, 0.3f );

		// Compute final sperical coordinate
		_myEye = D3DXVECTOR3 ( _mySC.radius * sin ( _mySC.gamma ) * sin ( _mySC.theta ),
			_mySC.radius * sin ( _mySC.gamma ) * cos ( _mySC.theta ),
			_mySC.radius * cos ( _mySC.gamma ) ) + _myTarget;
	}

}

