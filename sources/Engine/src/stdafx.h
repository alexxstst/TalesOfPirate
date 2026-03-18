// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "MindPowerAPI.h"

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <commdlg.h>
// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <assert.h>
#include <math.h>

// STL
#include <string>
#include <list>
#include <map>
#include <vector>

#include "GlobalInc.h"

// ── Ядро движка (PCH-оптимизация) ────────────────────────────────
// Порядок: каждый заголовок зависит только от предыдущих

#include "lwHeader.h"       // базовые макросы, типы (LW_BEGIN/LW_END, LW_SAFE_DELETE и т.д.)
#include "lwStdInc.h"       // системные includes (mmsystem.h, lmaccess.h)
#include "lwFrontAPI.h"     // LW_FRONT_API макрос
#include "lwDirectX.h"      // DirectX 8/9 типы и typedef'ы
#include "lwErrorCode.h"    // коды ошибок, LW_FAILED/LW_SUCCEEDED
#include "lwMath.h"         // lwVector3, lwMatrix44, lwQuaternion, геометрия
#include "lwITypes.h"       // lwITypes — перечисления, структуры типов движка
#include "lwClassDecl.h"    // forward-declarations классов движка
#include "lwInterfaceExt.h" // lwInterface.h (~900 строк) + расширения

