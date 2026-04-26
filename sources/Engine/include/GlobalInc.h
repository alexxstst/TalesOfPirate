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

// Логгирование ошибок движка через std::format (без variadic ...).
// Формат-строки в стиле std::format ({} вместо %s/%d), формат проверяется
// компилятором. Поддерживается передача только литералов или constexpr-строк.
template <class... Args>
inline void LGX(std::format_string<Args...> fmt, Args&&... args) {
	g_logManager.InternalLog(LogLevel::Error, "errors", std::format(fmt, std::forward<Args>(args)...));
}

#ifndef USE_LG_MSGBOX
#define USE_LG_MSGBOX
#define LG_MSGBOX LGX
#endif
