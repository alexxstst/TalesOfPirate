// LuaFunc.cpp Created by knight-gongjian 2006.9.20.
//---------------------------------------------------------
#include "stdafx.h"
#include "luafunc.h"
#include "Parser.h"
#include <strstream>
#include <algorithm>
using namespace std;
//---------------------------------------------------------
extern std::list<std::string> g_luaFNList;
extern lua_State* g_pLuaState;
extern void lua_callalert(lua_State* L, int status);

BOOL GetOnlineCount( DWORD& dwLoginNum, DWORD& dwPlayerNum )
{
	// ��ʼ��NPC�ű�ȫ�ֱ�����Ϣ
	lua_getglobal( g_pLuaState, "GetOnlineCount" );
	if( !lua_isfunction( g_pLuaState, -1 ) )
	{
		lua_pop( g_pLuaState, 1 );
		// Lua-функция не найдена
		ToLogService("lua", LogLevel::Error, "GetOnlineCount");
		return FALSE;
	}

	SYSTEMTIME st;
	GetSystemTime( &st );

	lua_pushnumber( g_pLuaState, st.wYear );
	lua_pushnumber( g_pLuaState, st.wMonth );
	lua_pushnumber( g_pLuaState, st.wDay );
	lua_pushnumber( g_pLuaState, st.wDayOfWeek );
	lua_pushnumber( g_pLuaState, st.wHour );
	lua_pushnumber( g_pLuaState, st.wMinute );

	lua_pushnumber( g_pLuaState, dwLoginNum );
	lua_pushnumber( g_pLuaState, dwPlayerNum );		

	int nStatus = lua_pcall( g_pLuaState, 8, 3, 0 );
	if( nStatus )
	{
		// Ошибка вызова lua-функции
		ToLogService("lua", LogLevel::Error, "call [GetOnlineCount] failed!");
		lua_callalert( g_pLuaState, nStatus );
		lua_settop(g_pLuaState, 0);
		return FALSE;
	}
	
	DWORD dwResult = (DWORD)lua_tonumber( g_pLuaState, -3 );
	dwLoginNum = (DWORD)lua_tonumber( g_pLuaState, -2 );
	dwPlayerNum = (DWORD)lua_tonumber( g_pLuaState, -1 );

	lua_settop(g_pLuaState, 0);

	return dwResult != 0;
}

// Add by lark.li 20080306 begin
inline int lua_GetResString( lua_State *L )
{
	BOOL bValid = lua_gettop( L ) == 1 && lua_isstring( L, 1 );
	if( !bValid )
	{
		PARAM_ERROR;
		return 0;
	}

	const char* pszID = lua_tostring( L, 1 );
	if( pszID == NULL )
	{
		PARAM_ERROR;
		return 0;
	}
	char* text = const_cast<char*>(g_ResourceBundleManage.LoadResString(pszID));

	lua_pushstring(L, text);

	return 1;
}
// End

inline int lua_LG(lua_State *L)
{
    int count = lua_gettop(L);
    if( count<=1 ) 
    {
        PARAM_ERROR;
		return 0;
    }

    const char *pszFile = lua_tostring(L, 1);	
	char szBuf[1024 * 2] = { 0 };
	std::ostrstream str( szBuf, sizeof(szBuf) );
    str << lua_tostring(L, 2);
	str << " ";
    for( int i=3; i<=count; i++ )
    {
		switch( lua_type( L, i ) )
		{
		case LUA_TNIL:
			{
				str << "nil"; 
			}
			break;
		case LUA_TBOOLEAN:
			{
				( lua_toboolean( L, i ) == 0 ) ? str << "FALSE" : str << "TRUE";
			}
			break;
		case LUA_TLIGHTUSERDATA:
		case LUA_TUSERDATA:
			{
				str << "userdata:";
				const void* p = lua_touserdata( L, i );
				( p ) ? str << p : str << "nil";
			}
			break;
		case LUA_TNUMBER:
		case LUA_TSTRING:
			{			
				const char* pszData = lua_tostring( L, i );		
				str << ( pszData ) ? pszData : "nil"; 
			}
			break;
		case LUA_TTABLE:
			{
				str << "table:";
				const void* p = lua_topointer( L, i );
				( p ) ? str << p : str << "nil";
			}
			break;
		case LUA_TFUNCTION:
			{
				str << "function:";
				const void* p = lua_topointer( L, i );
				( p ) ? str << p : str << "nil";
			}
			break;		
		case LUA_TTHREAD:
			{			
				str << "thread:";
				str << lua_tothread( L, i );
			}
			break;
		}
		str << "  ";
    }
    
	str << "\n";
	str << ends;

    // Лог из Lua-скрипта — канал определяется в рантайме
    g_logManager.InternalLog(LogLevel::Debug, (const char*)pszFile, str.str());
    return 0;
}

inline int lua_EXLG(lua_State *L)
{
	int nNumParam = lua_gettop(L);
	if( nNumParam <=1 ) 
	{
		PARAM_ERROR;
		return 0;
	}
	
	const char* pszFile = lua_tostring(L, 1);
	const char* pszTemp = lua_tostring(L, 2);
	if( !pszFile || !pszTemp )
	{
		PARAM_ERROR;
		return 0;
	}
	char  szData[1024] = {0};

	std::ostrstream str;
	USHORT sPos1 = 0, sNum = 0;
	for( int i = 3; i <= nNumParam; i++ )
	{
		char* pszPos = (char*)strstr( pszTemp + sPos1, "%" );
		if( pszPos == NULL )
		{
			str << pszTemp + sPos1;
			break;
		}

		sNum = USHORT(pszPos - (pszTemp + sPos1));
		strncpy( szData, pszTemp + sPos1, ( sNum > 1020 ) ? 1020 : sNum );
		szData[( sNum > 1020 ) ? 1020 : sNum] = 0;
		if( sNum > 1020 ) strcat( szData, "..." );
		str << szData;
		switch( *(pszPos + 1) )
		{
		case 'd':
		case 's':
			{
				const char* pszData = lua_tostring( L, i );		
				( pszData ) ? str << pszData : str << "nil"; 
			}
			break;		
		case 'b':
			{
				( lua_toboolean( L, i ) == 0 ) ? str << "FALSE" : str << "TRUE";
			}
			break;
		case 'u':
			{
				str << "userdata:";
				const void* p = lua_touserdata( L, i );
				( p ) ? str << p : str << "nil";
			}
			break;
		case 'f':
			{
				str << "function:";
				const void* p = lua_topointer( L, i );
				( p ) ? str << p : str << "nil";
			}
			break;
		case 't':
			{
				str << "table:";
				const void* p = lua_topointer( L, i );
				( p ) ? str << p : str << "nil";
			}
			break;
		default:
			{
				//str << "[��Ч��ʶ(" << *(pszPos + 1) << ")]";
				str << "[invilid identifier(" << *(pszPos + 1) << ")]";
			}
			break;
		}
		sPos1 += sNum + 2;
	}

	str << "\r\n";
	str << '\0';
	
	// Расширенный лог из Lua-скрипта — канал определяется в рантайме
	g_logManager.InternalLog(LogLevel::Debug, (const char*)pszFile, str.str());
	return 0;
}

BOOL RegisterLuaFunc()
{
	lua_State* L = g_pLuaState;

	g_CParser.Init(L);

	REGFN_INIT;
	REGFN(GetResString);
	REGFN(LG);
	REGFN(EXLG);

	return TRUE;
}