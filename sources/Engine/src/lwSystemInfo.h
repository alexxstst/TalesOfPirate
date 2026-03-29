//
#pragma once

#include "lwHeader.h"
#include "lwInterfaceExt.h"
#include "lwClassDecl.h"

LW_BEGIN

class lwSystemInfo : public lwISystemInfo
{
    LW_STD_DECLARATION();

private:
    lwDxVerInfo _dx_ver_info;

public:
    lwSystemInfo();
    ~lwSystemInfo();

    LW_RESULT CheckDirectXVersion();
    LW_RESULT GetDirectXVersion(lwDxVerInfo* o_info) { *o_info = _dx_ver_info; return LW_RET_OK; }
    DWORD GetDirectXVersion();
};

LW_END
