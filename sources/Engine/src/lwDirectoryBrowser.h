//
#pragma once

#include "lwHeader.h"
#include "lwStdInc.h"
#include "lwErrorCode.h"
#include "lwInterfaceExt.h"


LW_BEGIN

class lwDirectoryBrowser : public lwIDirectoryBrowser
{ 
    LW_STD_DECLARATION()

private: 
    lwDirectoryBrowserProc _proc;
    void* _param;

private:
    LW_RESULT _Go(const char* file, DWORD flag);

public: 
    lwDirectoryBrowser(); 

    void SetBrowseProc(lwDirectoryBrowserProc proc, void* param) { _proc = proc; _param = param; }
    LW_RESULT Browse(const char *file, DWORD flag);

}; 


LW_END
