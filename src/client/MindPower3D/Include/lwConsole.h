//
#pragma once

#include "lwHeader.h"
#include "lwStdInc.h"
#include "lwErrorCode.h"
#include "lwInterfaceExt.h"

LW_BEGIN

class lwConsole : public lwIConsole
{
    LW_STD_DECLARATION()

private:
    HANDLE _handle;

public:
    lwConsole();
    ~lwConsole();
    
    LW_RESULT Alloc();
    LW_RESULT Create(DWORD desired_access = GENERIC_READ|GENERIC_WRITE, DWORD share_mode = 0, const SECURITY_ATTRIBUTES* security_attr = 0);
    LW_RESULT Destroy();
    LW_RESULT Write(const char* str, ...);
    LW_RESULT SetBufferSize(DWORD row, DWORD column);
    BOOL SetActive() { return ::SetConsoleActiveScreenBuffer(_handle); }
    HANDLE GetHandle() const { return _handle; }
};

LW_END
