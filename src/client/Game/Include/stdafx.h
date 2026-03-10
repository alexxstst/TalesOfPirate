// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once


#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// TODO: reference additional headers your program requires here

#include <list>
#include <map>
#include <vector>
#include <queue>
#include <set>
#include <algorithm>
#include <span>
#include <bitset>
#include <tuple>

namespace GUI  //
{
};

using namespace GUI;

#include "Language.h"
#include "util.h"

#include "GlobalInc.h"
#include "MindPower.h"

#include "LanguageRecord.h"

extern CLanguageRecord g_oLangRec;
extern Logger animationLogger;

inline VOID D3DUtil_InitMaterialI( D3DMATERIALX& mtrl, FLOAT r, FLOAT g, FLOAT b,
                           FLOAT a )
{
    ZeroMemory( &mtrl, sizeof(D3DMATERIALX) );
    mtrl.Diffuse.r = mtrl.Ambient.r = r;
    mtrl.Diffuse.g = mtrl.Ambient.g = g;
    mtrl.Diffuse.b = mtrl.Ambient.b = b;
    mtrl.Diffuse.a = mtrl.Ambient.a = a;
}

#define WM_MUSICEND WM_USER + 0x1000

//#define APP_DEBUG

#define _LOG_NAME_		// ,

// #define FLOAT_INVALID   // ,caLua

//#define USE_TIMERPERIOD
#define WM_USER_TIMER (WM_USER+99)

#define OPT_CULL_1
//#define OPT_CULL_2

#define CLIENT_BUILD

// #define KOP_TOM			// TOM
