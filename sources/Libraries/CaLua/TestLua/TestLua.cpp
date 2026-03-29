// TestLua.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <math.h>
#include <iostream>

#include "CaLua.h"

using namespace std;

const char buffer[] = "AA";

const char* UI_ResString(char* id)
{
	return buffer;
}
const int AAddMisData(int, char* id)
{
	CLU_CallScriptFunction( "AddMisData", "int", "ushort, char*", 100, id );
	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
	int CLU_State = CLU_Init(); 
	CLU_LoadState(CLU_State);

	char ret[] = "char*";
	CLU_RegisterFunction("UI_ResString", ret, "char*", CLU_CDECL, CLU_CAST(UI_ResString));
	CLU_RegisterFunction("AAddMisData", "ushort", "int,char*", CLU_CDECL, CLU_CAST(AAddMisData));

	CLU_LoadScript("E:\\Item\\Test\\First\\First\\entry.lua");

	typedef struct RetType
	{
		char* pszName;
	} RetType;

	RetType* pRet = (RetType*)CLU_CallScriptFunction( "GetMisData", "char*", "ushort", 100 );

	CLU_Shutdown();

	return 0;
}

