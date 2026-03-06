#pragma once

#ifndef MPQuaternion_H
#define MPQuaternion_H

#include "MindPowerAPI.h"
#include "lwDirectX.h"

class MPEffVector3;
class MPRadian;

class MINDPOWER_API MPEffQuaternion
{
public:
	D3DXQUATERNION m_qSelf;
public:
	MPEffQuaternion() : m_qSelf(0, 0, 0, 0) {}
	MPEffQuaternion(float w, float x, float y, float z) 
		: m_qSelf(w, x, y, z) {}
	MPEffQuaternion(const D3DXQUATERNION& dxQ) : m_qSelf(dxQ) {}
	MPEffQuaternion(const MPEffQuaternion& q) : m_qSelf(q.m_qSelf) {}

	D3DXQUATERNION& GetDXValue() { return m_qSelf; }

	// 
	MPEffVector3 operator* (const MPEffVector3& rkVector) const;


	void FromAngleAxis (const MPRadian& rfAngle, const MPEffVector3& rkAxis);

};
#endif 
