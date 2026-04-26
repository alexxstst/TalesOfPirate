//
#pragma once


#include "lwHeader.h"
#include "lwStdInc.h"
#include "lwErrorCode.h"
#include "lwInterfaceExt.h"

LW_BEGIN

class lwTexEncode
{
private:
    LW_RESULT _Encode0(lwIBuffer* buf);
    LW_RESULT _Decode0(lwIBuffer* buf);
    LW_RESULT _Encode1(lwIBuffer* buf);
    LW_RESULT _Decode1(lwIBuffer* buf);
public:
    LW_RESULT Encode(lwIBuffer* buf);
    LW_RESULT Decode(lwIBuffer* buf);
};


LW_END
