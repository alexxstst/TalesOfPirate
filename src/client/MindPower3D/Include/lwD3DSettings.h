//
#pragma once

#include "lwHeader.h"
#include "lwDirectX.h"
#include "lwFrontAPI.h"
#include "lwErrorCode.h"
#include "lwITypes.h"

LW_BEGIN

LW_RESULT LW_FRONT_API lwInitDefaultD3DCreateParam( lwD3DCreateParam* param, HWND hwnd );

LW_RESULT LW_FRONT_API lwLoadD3DSettings( lwD3DCreateParam* param, const char* file );
LW_RESULT LW_FRONT_API lwSaveD3DSettings( const char* file, const lwD3DCreateParam* param );


LW_END
