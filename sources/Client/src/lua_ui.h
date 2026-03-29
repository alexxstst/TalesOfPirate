

// ---   (LuaBridge ) ---

inline void uiHideAll()
{
    CFormMgr::s_Mgr.SetEnabled(FALSE);
}

// --- lua_CFunction (,  1   ) ---

inline int uiGetForm(lua_State *L)
{
    return 1;
}
