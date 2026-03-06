//
#pragma once

#include "lwHeader.h"

LW_BEGIN


class lwLinkCtrl
{
public:
    virtual LW_RESULT GetLinkCtrlMatrix( lwMatrix44* mat, DWORD link_id ) = 0;
};


LW_END
