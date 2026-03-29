
/*//\\//\\//\\//\\//\\//\\//\\//\\////\\//\\//\\//\\//\\//\\//\\//\\////\\//\\//\\//\\
  CaLua Script Binding Utility
  By Justin Reynen, 2003

  Main File

  The only reason you'd need to touch this header, is if you've changed the structure
  member alignment of your compiler.

  There's no significance to the fact that I used __cdecl as the function type passed
  to CLU_RegisterFunction, it could just as easily have been __stdcall.
  
//\\//\\//\\//\\//\\//\\//\\//\\////\\//\\//\\//\\//\\//\\//\\//\\////\\//\\//\\//\\*/


#ifndef __CALUA_MAIN_H
#define __CALUA_MAIN_H

#ifdef __cplusplus
extern "C"{
#endif

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#define __STRUCTURE_MEMBER_ALIGNMENT 8

#define CLU_CAST(x) (void (__cdecl *)(void))(x)

#define CLU_CDECL		0
#define CLU_STDCALL		1


int			CLU_Init();
int			CLU_Shutdown();
int			CLU_ShutdownState(int state);
int			CLU_LoadState(int state);
int			CLU_LoadScript(const char* filename, int isObfuscate);
int			CLU_RegisterStructure(char* name, char* desc);
int			CLU_RegisterFunction(const char* name, const char* ret, const char* args, int convention, void (__cdecl *func)(void));

void*		CLU_CallScriptFunction(const char* funcName, const char* retArgs, const char* args, ...);

void		CLU_DllFree(void* memptr);

lua_State*  CLU_GetVirtualMachine();
int			CLU_GetCurrentState();

#ifdef __cplusplus
}
#endif

#endif
