#include "stdafx.h"
#include "MPEffectMath.h"

#include <math.h>
#include <limits>
#include "MPEffectAsmMath.h"

//=============================================================================
// MPRadian
//-----------------------------------------------------------------------------
float MPRadian::valueDegrees() const
{
	return MPEffectMath::RadiansToDegrees (m_fRad);
}
//-------------------------------------------------------------------------
float MPRadian::valueAngleUnits() const
{
	return MPEffectMath::RadiansToAngleUnits (m_fRad);

}
//=============================================================================
// MPDegree
//-----------------------------------------------------------------------------
float MPDegree::valueRadians() const
{
	return MPEffectMath::DegreesToRadians(m_fDeg);
}
//-------------------------------------------------------------------------
float MPDegree::valueAngleUnits() const
{
	return MPEffectMath::DegreesToAngleUnits(m_fDeg);
}
//=============================================================================
// MPEffectMath
//-----------------------------------------------------------------------------
const float MPEffectMath::POS_INFINITY = std::numeric_limits<float>::infinity();
const float MPEffectMath::NEG_INFINITY = -std::numeric_limits<float>::infinity();
const float MPEffectMath::PI = float( 4.0 * atan( 1.0 ) );
const float MPEffectMath::TWO_PI = float( 2.0 * PI );
const float MPEffectMath::HALF_PI = float( 0.5 * PI );
const float MPEffectMath::fDeg2Rad = PI / float(180.0);
const float MPEffectMath::fRad2Deg = float(180.0) / PI;

const std::string MPEffectMath::BLANK = std::string("");


int MPEffectMath::m_TrigTableSize;
MPEffectMath::AngleUnit MPEffectMath::m_sAngleUnit;

float  MPEffectMath::m_TrigTableFactor;
float *MPEffectMath::m_SinTable = 0;
float *MPEffectMath::m_TanTable = 0;

//-----------------------------------------------------------------------------
MPEffectMath::MPEffectMath( unsigned int trigTableSize )
{
	m_sAngleUnit = AU_DEGREE;

	m_TrigTableSize = trigTableSize;
	m_TrigTableFactor = m_TrigTableSize / MPEffectMath::TWO_PI;

	m_SinTable = new float[m_TrigTableSize];
	m_TanTable = new float[m_TrigTableSize];

	buildTrigTables();
}
//-----------------------------------------------------------------------------
MPEffectMath::~MPEffectMath()
{
	delete[] m_SinTable;
	delete[] m_TanTable;
}
//-----------------------------------------------------------------------------
void MPEffectMath::buildTrigTables(void)
{
	float angle;
	for (int i = 0; i < m_TrigTableSize; ++i)
	{
		angle = MPEffectMath::TWO_PI * i / m_TrigTableSize;
		m_SinTable[i] = sin(angle);
		m_TanTable[i] = tan(angle);
	}
}
//-----------------------------------------------------------------------------
float MPEffectMath::SinTable (float fValue)
{
	// Convert range to index values, wrap if required
	int idx;
	if (fValue >= 0)
	{
		idx = int(fValue * m_TrigTableFactor) % m_TrigTableSize;
	}
	else
	{
		idx = m_TrigTableSize - (int(-fValue * m_TrigTableFactor) % m_TrigTableSize) - 1;
	}

	return m_SinTable[idx];
}
//-----------------------------------------------------------------------------
float MPEffectMath::TanTable (float fValue)
{
	int idx = int(fValue * m_TrigTableFactor) % m_TrigTableSize;
	return m_TanTable[idx];
}
//-----------------------------------------------------------------------------
void MPEffectMath::setAngleUnit(AngleUnit unit)
{
	m_sAngleUnit = unit;
}
//-----------------------------------------------------------------------------
MPEffectMath::AngleUnit MPEffectMath::getAngleUnit(void)
{
	return m_sAngleUnit;
}
//-----------------------------------------------------------------------------
float MPEffectMath::AngleUnitsToRadians(float units)
{
	if (m_sAngleUnit == AU_DEGREE)
		return DegreesToRadians(units);
	else
		return units;
}
//-----------------------------------------------------------------------------
float MPEffectMath::RadiansToAngleUnits(float radians)
{
	if (m_sAngleUnit == AU_DEGREE)
		return RadiansToDegrees(radians);
	else
		return radians;
}
//-----------------------------------------------------------------------------
float MPEffectMath::AngleUnitsToDegrees(float units)
{
	if (m_sAngleUnit == AU_DEGREE)
		return units;
	else
		return RadiansToDegrees(units);
}
//-----------------------------------------------------------------------------
float MPEffectMath::DegreesToAngleUnits(float degrees)
{
	if (m_sAngleUnit == AU_DEGREE)
		return degrees;
	else
		return DegreesToRadians(degrees);
}
//-----------------------------------------------------------------------------
float MPEffectMath::UnitRandom ()
{
	return asm_rand() / asm_rand_max();
}
//-----------------------------------------------------------------------------
float MPEffectMath::RangeRandom (float fLow, float fHigh)
{
	return (fHigh - fLow) * UnitRandom() + fLow;
}
//-----------------------------------------------------------------------------
