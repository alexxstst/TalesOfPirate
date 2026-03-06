#pragma once

#include <strstream>

inline int lua_MsgBox(lua_State *L)
{
    const char *pszContent = lua_tostring(L, 1);
    MessageBox(NULL, pszContent, "msg", 0);
    return 0;
}

inline int lua_GetTickCount(lua_State *L)
{
    lua_pushnumber(L, GetTickCount());
    return 1;
}

inline int lua_Rand(lua_State *L)
{
    BOOL bValid = (lua_gettop(L)==1 && lua_isnumber(L, 1));
    if(!bValid)
    {
        PARAM_ERROR;
    }
    int nRange = (int)lua_tonumber(L, 1);
    int nRand = rand()%nRange;
    lua_pushnumber(L, nRand);
    return 1;
}

inline int lua_LG(lua_State *L)
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
    LG( (char*)pszFile, str.str() );
    return 0;
}

inline int lua_SysInfo(lua_State *L)
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
