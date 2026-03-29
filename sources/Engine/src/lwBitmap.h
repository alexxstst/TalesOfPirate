//
#pragma once

#include "lwStdInc.h"
#include "lwGraphicsUtil.h"

LW_BEGIN


class LW_FRONT_API lwBitmap
{
public:
	BITMAPINFOHEADER _bmih;
	lwColorValue4b* _buf;

private:
	int _GetData8( BYTE* buf );
    int _GetData16( BYTE* buf );
    int _GetData24( BYTE* buf );
    int _GetData32( BYTE* buf );
	void _SetAlphaChannel( DWORD colorkey_type, lwColorValue4b* colorkey );
public:
	lwBitmap() :_buf(0) {}
	~lwBitmap() { LW_SAFE_DELETE( _buf ); }

	int Load( const char* file, DWORD colorkey_type, lwColorValue4b* colorkey );
	lwColorValue4b* Detach();
};

LW_END
