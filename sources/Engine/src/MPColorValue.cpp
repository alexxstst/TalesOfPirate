#include "stdafx.h"
#include "MPColorValue.h"

//-----------------------------------------------------------------------------
MPColorValue MPColorValue::Black = MPColorValue(0.0, 0.0, 0.0);
MPColorValue MPColorValue::White = MPColorValue(1.0, 1.0, 1.0);
MPColorValue MPColorValue::Red = MPColorValue(1.0, 0.0, 0.0);
MPColorValue MPColorValue::Green = MPColorValue(0.0, 1.0, 0.0);
MPColorValue MPColorValue::Blue = MPColorValue(0.0, 0.0, 1.0);
//-----------------------------------------------------------------------------
RGBA MPColorValue::getAsRGBA(void) const {
	unsigned char val8;
	unsigned int val32 = 0;

	// 32
	// (RGBA = 8888)

	// Red
	val8 = static_cast<unsigned char>(r * 255);
	val32 = val8 << 24;

	// Green
	val8 = static_cast<unsigned char>(g * 255);
	val32 += val8 << 16;

	// Blue
	val8 = static_cast<unsigned char>(b * 255);
	val32 += val8 << 8;

	// Alpha
	val8 = static_cast<unsigned char>(a * 255);
	val32 += val8;

	return val32;
}

//-----------------------------------------------------------------------------
ARGB MPColorValue::getAsARGB(void) const {
	unsigned char val8;
	unsigned int val32 = 0;

	// 32
	// (ARGB = 8888)

	// Alpha
	val8 = static_cast<unsigned char>(a * 255);
	val32 = val8 << 24;

	// Red
	val8 = static_cast<unsigned char>(r * 255);
	val32 += val8 << 16;

	// Green
	val8 = static_cast<unsigned char>(g * 255);
	val32 += val8 << 8;

	// Blue
	val8 = static_cast<unsigned char>(b * 255);
	val32 += val8;


	return val32;
}

//-----------------------------------------------------------------------------
ABGR MPColorValue::getAsABGR(void) const {
	unsigned char val8;
	unsigned int val32 = 0;

	// 32
	// (ABRG = 8888)

	// Alpha
	val8 = static_cast<unsigned char>(a * 255);
	val32 = val8 << 24;

	// Blue
	val8 = static_cast<unsigned char>(b * 255);
	val32 += val8 << 16;

	// Green
	val8 = static_cast<unsigned char>(g * 255);
	val32 += val8 << 8;

	// Red
	val8 = static_cast<unsigned char>(r * 255);
	val32 += val8;


	return val32;
}

//-----------------------------------------------------------------------------
void MPColorValue::setAsRGBA(const RGBA val) {
	unsigned int val32 = val;

	// 32
	// (RGBA = 8888)

	// Red
	r = static_cast<unsigned char>(val32 >> 24) / 255.0f;

	// Green
	g = static_cast<unsigned char>(val32 >> 16) / 255.0f;

	// Blue
	b = static_cast<unsigned char>(val32 >> 8) / 255.0f;

	// Alpha
	a = static_cast<unsigned char>(val32) / 255.0f;
}

//-----------------------------------------------------------------------------
void MPColorValue::setAsARGB(const ARGB val) {
	unsigned int val32 = val;

	// 32
	// (ARGB = 8888)

	// Alpha
	a = static_cast<unsigned char>(val32 >> 24) / 255.0f;

	// Red
	r = static_cast<unsigned char>(val32 >> 16) / 255.0f;

	// Green
	g = static_cast<unsigned char>(val32 >> 8) / 255.0f;

	// Blue
	b = static_cast<unsigned char>(val32) / 255.0f;
}

//-----------------------------------------------------------------------------
void MPColorValue::setAsABGR(const ABGR val) {
	unsigned int val32 = val;

	// 32
	// (ABGR = 8888)

	// Alpha
	a = static_cast<unsigned char>(val32 >> 24) / 255.0f;

	// Blue
	b = static_cast<unsigned char>(val32 >> 16) / 255.0f;

	// Green
	g = static_cast<unsigned char>(val32 >> 8) / 255.0f;

	// Red
	r = static_cast<unsigned char>(val32) / 255.0f;
}

//-----------------------------------------------------------------------------
bool MPColorValue::operator==(const MPColorValue& rhs) const {
	return (r == rhs.r &&
		g == rhs.g &&
		b == rhs.b &&
		a == rhs.a);
}

//-----------------------------------------------------------------------------
bool MPColorValue::operator!=(const MPColorValue& rhs) const {
	return !(*this == rhs);
}
