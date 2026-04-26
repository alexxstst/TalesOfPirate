// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently

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
#include <string_view>
#include <list>
#include <map>
#include <vector>
#include <format>
#include <charconv>

#include "GlobalInc.h"

//    (PCH-) 
// :      

#include "lwHeader.h"       //  ,  (LW_BEGIN/LW_END, LW_SAFE_DELETE  ..)
#include "lwStdInc.h"       //  includes (mmsystem.h, lmaccess.h)
#include "lwDirectX.h"      // DirectX 8/9   typedef'
#include "lwErrorCode.h"    //  , LW_FAILED/LW_SUCCEEDED
#include "lwMath.h"         // lwVector3, lwMatrix44, lwQuaternion, 
#include "lwITypes.h"       // lwITypes  ,   
#include "lwClassDecl.h"    // forward-declarations  
#include "lwInterfaceExt.h" // lwInterface.h (~900 ) + 
