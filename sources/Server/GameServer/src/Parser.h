//=============================================================================
// FileName: Parser.h
// Creater: ZhangXuedong
// Date: 2004.11.22
// Comment: scripts interface
//=============================================================================

#ifndef PARSER_H
#define PARSER_H

#include "tchar.h"

#include "lua.hpp"

#define DOSTRING_PARAM_END	999999999
#define DOSTRING_RETURN_NUM	20
#define DOSTRING_RETURN_STRING_LEN	512

enum EScriptParamType
{
	enumSCRIPT_PARAM_NUMBER = 1,
	enumSCRIPT_PARAM_NUMBER_UNSIGNED,
	enumSCRIPT_PARAM_LIGHTUSERDATA,
	enumSCRIPT_PARAM_STRING,
	enumSCRIPT_PARAM_NUMBER_LONG64,
};

enum EScriptReturnType
{
	enumSCRIPT_RETURN_NONE,
	enumSCRIPT_RETURN_NUMBER,
	enumSCRIPT_RETURN_STRING,
};

class CParser
{
public:
	void	Init(lua_State *pLS);
	void	Free();

	int		DoString(const char *csString, char chRetType, int nRetNum, ...);
	bool	StringIsFunction(const char *csString) {lua_getglobal(m_pSLua, csString); return lua_isfunction(m_pSLua, -1);}
	int		GetReturnNumber(char chID) {if (chID >= DOSTRING_RETURN_NUM) return 0; return m_nDoStringRet[chID];}
	char*	GetReturnString(char chID) {if (chID >= DOSTRING_RETURN_NUM) return 0; return m_szDoStringRet[chID];}

protected:

private:
	bool	SetRetString(char chID, const char *cszString)
	{
		if (chID >= DOSTRING_RETURN_NUM) return false;
		strncpy(m_szDoStringRet[chID], cszString, DOSTRING_RETURN_STRING_LEN - 1);
		m_szDoStringRet[chID][DOSTRING_RETURN_STRING_LEN - 1] = '\0';
		return true;
	}

	lua_State	*m_pSLua; // lua

	int			m_nDoStringRet[DOSTRING_RETURN_NUM];
	char		m_szDoStringRet[DOSTRING_RETURN_NUM][DOSTRING_RETURN_STRING_LEN];
};

extern CParser	g_CParser;

#endif // PARSER_H
