#include "stdafx.h"

#include "MPEffQuaternion.h"

#include "MPEffectMath.h"
#include "MPEffVector3.h"
//-----------------------------------------------------------------------------
void MPEffQuaternion::FromAngleAxis(const MPRadian& rfAngle, const MPEffVector3& rkAxis) {
	// :  

	MPRadian fHalfAngle(rfAngle * 0.5);
	float fSin = MPEffectMath::Sin(fHalfAngle);
	m_qSelf.w = MPEffectMath::Cos(fHalfAngle);
	m_qSelf.x = fSin * rkAxis.m_vSelf.x;
	m_qSelf.y = fSin * rkAxis.m_vSelf.y;
	m_qSelf.z = fSin * rkAxis.m_vSelf.z;
}

//-----------------------------------------------------------------------------
MPEffVector3 MPEffQuaternion::operator*(const MPEffVector3& v) const {
	// nVidia SDK 
	MPEffVector3 uv, uuv;
	MPEffVector3 qvec(m_qSelf.x, m_qSelf.y, m_qSelf.z);
	uv = qvec.crossProduct(v);
	uuv = qvec.crossProduct(uv);
	uv *= (2.0f * m_qSelf.w);
	uuv *= 2.0f;

	return v + uv + uuv;
}
