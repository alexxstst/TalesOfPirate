#include "Stdafx.h"
#include "MPCamera.h"


MPCameraNOLEECH::MPCameraNOLEECH() {
	m_fRoll = 0.0f;

	InitPosition(0.0f, 100.0f, 100.0f, 0.0f, 0.0f, 0.0f);
	sprintf(strCameraInfo, "camera : (%4.02f %4.02f %4.02f)__(%4.02f %4.02f %4.02f)",
			m_EyePos.x, m_EyePos.y, m_EyePos.z,
			m_RefPos.x, m_RefPos.y, m_RefPos.z);
}

MPCameraNOLEECH::~MPCameraNOLEECH() {
}

VOID MPCameraNOLEECH::Move(DWORD dwMoveType) {
	float fStep = 0.2f;
	D3DXVECTOR3 Move(0, 0, 0);

	switch (dwMoveType) {
	case MOVE_LEFT: {
		auto v = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		auto v2 = (m_RefPos - m_EyePos);
		D3DXVec3Cross(&Move, &v2, &v);
		D3DXVec3Normalize(&Move, &Move);
		Move *= fStep;
		m_RefPos += Move;
		m_EyePos += Move;
		break;
	}
	case MOVE_RIGHT: {
		auto v = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		auto v2 = (m_RefPos - m_EyePos);
		D3DXVec3Cross(&Move, &v2, &v);
		D3DXVec3Normalize(&Move, &Move);
		Move *= fStep;
		m_RefPos -= Move;
		m_EyePos -= Move;
		break;
	}
	case MOVE_FORWARD: {
		auto v = (m_RefPos - m_EyePos);
		D3DXVec3Normalize(&Move, &v);
		Move *= fStep;
		m_RefPos += Move;
		m_EyePos += Move;
		break;
	}
	case MOVE_BACKWARD: {
		auto v = (m_RefPos - m_EyePos);
		D3DXVec3Normalize(&Move, &v);
		Move *= fStep;
		m_RefPos -= Move;
		m_EyePos -= Move;
		break;
	}
	case MOVE_UP: {
		Move = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		Move *= fStep;
		m_RefPos += Move;
		m_EyePos += Move;
		break;
	}
	case MOVE_DOWN: {
		Move = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		Move *= fStep;
		m_RefPos -= Move;
		m_EyePos -= Move;
		break;
	}
	case ROTATE_VEER: {
		break;
	}
	case ROTATE_REVERSE: {
		break;
	}
	case ROLL1: {
		m_fRoll += 5.0f;
		break;
	}
	case ROLL2: {
		m_fRoll -= 5.0f;
		break;
	}
	}
	sprintf(strCameraInfo, "camera : (%4.02f %4.02f %4.02f)__(%4.02f %4.02f %4.02f)",
			m_EyePos.x, m_EyePos.y, m_EyePos.z,
			m_RefPos.x, m_RefPos.y, m_RefPos.z);
}

// , Hang
void MPCameraNOLEECH::MoveForward(float fStep, BOOL bHang) {
	D3DXVECTOR3 Move(0, 0, 0);
	if (bHang) {
		auto v = (D3DXVECTOR3(m_RefPos.x, m_RefPos.y, 0.0f) - D3DXVECTOR3(m_EyePos.x, m_EyePos.y, 0.0f));
		D3DXVec3Normalize(&Move, &v);
	}
	else {
		auto v = (m_RefPos - m_EyePos);
		D3DXVec3Normalize(&Move, &v);
	}
	Move *= fStep;
	m_RefPos += Move;
	m_EyePos += Move;
}

// , Hang
void MPCameraNOLEECH::MoveRight(float fStep, BOOL bHang) {
	D3DXVECTOR3 Move(0, 0, 0);

	auto v = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	auto v2 = (m_RefPos - m_EyePos);
	D3DXVec3Cross(&Move, &v2, &v);
	D3DXVec3Normalize(&Move, &Move);
	Move *= fStep;
	if (bHang) {
		Move.z = 0;
	}
	m_RefPos += Move;
	m_EyePos += Move;
}

void MPCameraNOLEECH::Turn(float fStep, D3DXVECTOR3* pFocusVec) {
	D3DXVECTOR3 Move(0, 0, 0);

	if (!pFocusVec) {
		const D3DXVECTOR3 v[] = {
			m_RefPos - m_EyePos,
			D3DXVECTOR3(0.0f, 0.0f, 1.0f)
		};
		D3DXVec3Cross(&Move, &v[0], &v[1]);
		D3DXVec3Normalize(&Move, &Move);
		Move *= fStep;
		m_RefPos += Move;
	}
	else {
		m_RefPos = *pFocusVec;
		D3DXVECTOR3 vv = m_EyePos - *pFocusVec;
		D3DXVECTOR3 const v[] = {
			D3DXVECTOR3(vv.x, vv.y, 0.0f),
			D3DXVECTOR3(0.0f, 0.0f, 1.0f)
		};
		D3DXVec3Normalize(&vv, &vv);
		D3DXVec3Cross(&Move, &v[0], &v[1]);
		D3DXVec3Normalize(&Move, &Move);
		Move *= fStep;
		m_EyePos += Move;
	}
}

void MPCameraNOLEECH::FrameMove(DWORD dwTailTime) {
}

//lemon add@2004.8.4////////////////////////////////////////////////////////////////////////
