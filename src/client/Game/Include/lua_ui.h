#pragma once



inline int lua_uiGetForm(lua_State *L)
{
    return 1;
}

inline int lua_uiHideAll(lua_State *L)
{
    CFormMgr::s_Mgr.SetEnabled(FALSE);
    return 0;
}
