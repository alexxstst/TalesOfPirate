//
#include "stdafx.h"
#include "lwConsole.h"

LW_BEGIN

// lwConsole
LW_STD_IMPLEMENTATION(lwConsole)

lwConsole::lwConsole()
: _handle(INVALID_HANDLE_VALUE)
{
}
lwConsole::~lwConsole()
{
    Destroy();
}

LW_RESULT lwConsole::Alloc()
{
    return ::AllocConsole() ? LW_RET_OK : LW_RET_FAILED;
}

LW_RESULT lwConsole::Create(DWORD desired_access, DWORD share_mode, const SECURITY_ATTRIBUTES* security_attr)
{
    _handle = ::CreateConsoleScreenBuffer(desired_access, share_mode, security_attr, CONSOLE_TEXTMODE_BUFFER, 0);

    return (_handle == INVALID_HANDLE_VALUE) ? LW_RET_FAILED : LW_RET_OK;
}

LW_RESULT lwConsole::Destroy()
{
    if(_handle != INVALID_HANDLE_VALUE)
    {
        ::CloseHandle(_handle);
        _handle = INVALID_HANDLE_VALUE;
    }

    return LW_RET_OK;
}
LW_RESULT lwConsole::Write(const char* str, ...)
{
    char buf[1024];
    
    va_list args;
    va_start(args, str);
    _vsntprintf(buf, 1024, str, args);
    va_end(args);

    DWORD str_size = (DWORD)strlen(buf);

    BOOL b = ::WriteConsole(_handle, buf, str_size, &str_size, 0);

    return b ? LW_RET_OK : LW_RET_FAILED;
}
LW_RESULT lwConsole::SetBufferSize(DWORD row, DWORD column)
{
    COORD d;
    d.X = (SHORT)row;
    d.Y = (SHORT)column;

    return ::SetConsoleScreenBufferSize(_handle, d) ? LW_RET_OK : LW_RET_FAILED;
}

LW_END
