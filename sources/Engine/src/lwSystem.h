//
#pragma once

#include "lwHeader.h"
#include "lwInterfaceExt.h"
#include "lwClassDecl.h"

LW_BEGIN

class lwSystem : public lwISystem
{
public:
    static lwISystem* __system;
    static void SetActiveISystem( lwISystem* sys ) { __system = sys; }
    static lwISystem* GetActiveISystem() { return __system; }

private:
    lwIPathInfo* _path_info;
    lwIOptionMgr* _option_mgr;
    lwITimer* _internal_timer;
    lwISystemInfo* _system_info;

LW_STD_DECLARATION()

public:
    lwSystem();
    ~lwSystem();

    LW_RESULT Initialize();
    LW_RESULT CreateGraphicsSystem( lwISysGraphics** sys );

    lwIPathInfo* GetPathInfo() { return _path_info; }
    lwIOptionMgr* GetOptionMgr() { return _option_mgr; }
    lwISystemInfo* GetSystemInfo() { return _system_info; }
};

LW_END
