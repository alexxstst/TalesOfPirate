
/*//\\//\\//\\//\\//\\//\\//\\//\\////\\//\\//\\//\\//\\//\\//\\//\\////\\//\\//\\//\\
  CaLua Script Binding Utility
  By Justin Reynen, 2003

  Helper Functions Stuff

  I kept all the memory stuff and string stuff as seperate functions, so you can
  change the function calls if you're using a memory management or string library.
//\\//\\//\\//\\//\\//\\//\\//\\////\\//\\//\\//\\//\\//\\//\\//\\////\\//\\//\\//\\*/



#ifndef __CALUA_HELPER_H
#define __CALUA_HELPER_H

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <malloc.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <crtdbg.h>

#undef NULL
#define NULL ((void*)0)

#undef SAFE_FREE
#define SAFE_FREE(x) if (x != NULL) {CLU_Free(x);x=NULL;}

#ifdef _DEBUG
#define CLU_NOISY_ERROR
#endif


#define CLU_MEMCHECK _CrtCheckMemory();


#define CLU_ERR_PREFIX "CaLua Library : "
#define CLU_ERR_FILENAME "calua_err.txt"

/////
//Exported Functions
/////

//Error Reporting stuff
void CLU_InitError();
void CLU_ShutdownError();
void CLU_Error(char* d, ...);

//Memory and buffer stuff
void* CLU_Malloc(int size);
void* CLU_Calloc(int size);
void CLU_Free(void* memptr);
void CLU_ExpandBuffer(void** in, int type, int data, int amount);
void CLU_InsertBuffer(void** in, int type, int sz, void* data, int where, int len);
void CLU_SnipBuffer(void** in, int type, int sz, int loc, int len);

//String stuff
char* CLU_String_Copy(char * string);
int CLU_String_Concat(char** dst, char* src);

#endif
