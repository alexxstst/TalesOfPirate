#pragma once

#pragma warning(disable : 4251)
#pragma warning(disable : 4786)


#include "lwDirectX.h"

#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION         0x0800
#endif

#include <dinput.h>
#include <mmsystem.h>
#include <stdio.h>
#include <tchar.h>
#include <stdarg.h>

#include "Util.h"

struct MPTexRect {
	int nTexSX;
	int nTexSY;
	int nTexW;
	int nTexH;
	float fScaleX;
	float fScaleY;
	DWORD dwColor;
	int nTextureNo;

	MPTexRect()
		: nTexSX(0), nTexSY(0), nTexW(0), nTexH(0),
		  fScaleX(1.0f), fScaleY(1.0f), nTextureNo(-1), dwColor(0xFFFFFFFF) {
	}
};

inline void __cdecl LGX(const char* format, ...) {
	char buf[512]{};
	va_list args;
	va_start(args, format);
	_vsntprintf(buf, 512, format, args);
	va_end(args);

	g_logManager.InternalLog(LogLevel::Error, "errors", buf);
}

#ifndef USE_LG_MSGBOX
#define USE_LG_MSGBOX
#define LG_MSGBOX LGX
#endif
