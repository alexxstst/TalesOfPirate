#ifndef MPEffVector3_H
#define MPEffVector3_H

#include "MindPowerAPI.h"

#include "lwDirectX.h"
#include "MPEffectMath.h"
#include "MPEffQuaternion.h"

class MPEffVector3
{
public:
	D3DXVECTOR3 m_vSelf;
public:
	MPEffVector3() : m_vSelf(0, 0, 0) {}
	MPEffVector3(float x, float y, float z) : m_vSelf(x, y, z) {}
	MPEffVector3(const MPEffVector3& v) : m_vSelf(v.m_vSelf) {}
	MPEffVector3(const D3DXVECTOR3& dxV) : m_vSelf(dxV) {}

	D3DXVECTOR3& GetDXValue()	{ return m_vSelf; }

	// 
	inline MPEffVector3 operator + ( const MPEffVector3& rkVector ) const
	{
		MPEffVector3 kSum;

		D3DXVec3Add(&(kSum.m_vSelf), &m_vSelf, &(rkVector.m_vSelf));

		return kSum;
	}


	inline MPEffVector3& operator *= ( float fScalar )
	{
		D3DXVec3Scale(&m_vSelf, &m_vSelf, fScalar);
		return *this;
	}

	inline MPEffVector3& operator *= ( const MPEffVector3& rkVector )
	{
		m_vSelf.x *= rkVector.m_vSelf.x;
		m_vSelf.y *= rkVector.m_vSelf.y;
		m_vSelf.z *= rkVector.m_vSelf.z;

		return *this;
	}


	inline MPEffVector3 crossProduct( const MPEffVector3& rkVector ) const
	{
		D3DXVECTOR3 kCross;
		
		D3DXVec3Cross( &kCross, &m_vSelf, &(rkVector.m_vSelf));

		return kCross;
	}

	inline float squaredLength () const
	{
		return D3DXVec3LengthSq(&m_vSelf);
	}

	/** .
	@note
		
	@returns .
	*/
	inline float normalise()
	{
		float fLength = D3DXVec3Length(&m_vSelf);

		// 
		if ( fLength > 1e-06f )
		{
			D3DXVec3Normalize(&m_vSelf, &m_vSelf);
		}

		return fLength;
	}


	/** .
	@remarks
	This method will return a vector which is perpendicular to this
	vector. There are an infinite number of possibilities but this 
	method will guarantee to generate one of them. If you need more 
	control you should use the Quaternion class.
	*/
	inline MPEffVector3 perpendicular(void) const
	{
		static const float fSquareZero = float(1e-06 * 1e-06);

		MPEffVector3 perp = this->crossProduct( MPEffVector3::UNIT_X );

		//  length
		if( perp.squaredLength() < fSquareZero )
		{
			/* Y.
			*/
			perp = this->crossProduct( MPEffVector3::UNIT_Y );
		}

		return perp;
	}

	inline bool operator == ( const MPEffVector3& rkVector ) const
	{
		return ( m_vSelf.x == rkVector.m_vSelf.x 
			&& m_vSelf.y == rkVector.m_vSelf.y 
			&& m_vSelf.z == rkVector.m_vSelf.z );
	}


	/** .
	@remarks
		
	@param 
	angle 
	@param 
	up 
	@returns 
		
	*/
	inline MPEffVector3 randomDeviant( const MPRadian& angle,
		const MPEffVector3& up = MPEffVector3::ZERO ) const
	{
		MPEffVector3 newUp;

		if (up == MPEffVector3::ZERO)
		{
			// 
			newUp = this->perpendicular();
		}
		else
		{
			newUp = up;
		}

		// up
		MPEffQuaternion q;
		q.FromAngleAxis( MPRadian(MPEffectMath::UnitRandom() * MPEffectMath::TWO_PI), *this );
		newUp = q * newUp;

		// Finally rotate this by given angle around randomised up
		q.FromAngleAxis( angle, newUp );
		return q * (*this);
	}

	// 
	static const MPEffVector3 ZERO;
	static const MPEffVector3 UNIT_X;
	static const MPEffVector3 UNIT_Y;
	static const MPEffVector3 UNIT_Z;
	static const MPEffVector3 NEGATIVE_UNIT_X;
	static const MPEffVector3 NEGATIVE_UNIT_Y;
	static const MPEffVector3 NEGATIVE_UNIT_Z;
	static const MPEffVector3 UNIT_SCALE;

};
#endif
