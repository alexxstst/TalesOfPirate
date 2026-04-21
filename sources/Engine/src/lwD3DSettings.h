//
#pragma once

#include "lwHeader.h"
#include "lwDirectX.h"
#include "lwErrorCode.h"
#include "lwITypes.h"

LW_BEGIN

LW_RESULT lwInitDefaultD3DCreateParam( lwD3DCreateParam* param, HWND hwnd );

LW_RESULT lwLoadD3DSettings( lwD3DCreateParam* param, const char* file );
LW_RESULT lwSaveD3DSettings( const char* file, const lwD3DCreateParam* param );


LW_END
