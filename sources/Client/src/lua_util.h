#include <strstream>

// ---   (LuaBridge ) ---

inline void MsgBox(const std::string& content)
{
    MessageBox(NULL, content.c_str(), "msg", 0);
}

inline int GetTickCount_Lua()
{
    return (int)GetTickCount();
}

inline int Rand_Lua(int nRange)
{
    return rand() % nRange;
}

// --- lua_CFunction (arg  LuaBridge  ) ---

inline int LG(lua_State *L)
{
    int count = lua_gettop(L);
    if( count<=1 )
    {
        PARAM_ERROR;
    }

    const char *pszFile = lua_tostring(L, 1);
	char szBuf[2048] = { 0 };
    std::ostrstream str( szBuf, sizeof(szBuf) );
    str << lua_tostring(L, 2);
    for( int i=3; i<=count; i++ )
    {
        str << ", " << lua_tostring(L, i);
    }

    str << std::ends;
    g_logManager.InternalLog(LogLevel::Debug, "common", str.str());
    return 0;
}

inline int SysInfo(lua_State *L)
{
    int count = lua_gettop(L);
    if( count<=0 )
    {
        PARAM_ERROR;
    }

	char szBuf[2048] = { 0 };
    std::ostrstream str( szBuf, sizeof(szBuf) );
    for( int i=1; i<=count; i++ )
    {
        str << " " << lua_tostring(L, i);
    }

    str << std::ends;
	g_pGameApp->SysInfo( "luaSysInfo:%s", str.str() );
    return 0;
}
//lua_dofile
