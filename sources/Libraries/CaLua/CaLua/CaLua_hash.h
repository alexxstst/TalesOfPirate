/*
	Serpent Engine
	Dan Carter, 2003
*/

#ifndef __CLU_HASH_H
#define __CLU_HASH_H

#include "CaLua_helper.h"
#include "CaLua_avltree.h"

#define CLU_HASH_STRING_COPY(a) CLU_String_Copy(a);

typedef char CLU_Hash_Key;

typedef struct CLU_LUNode {
	int index;
} CLU_LUNode;

CLU_Hash_Key *CLU_Hash_Key_Create(char *a);
CLU_Hash_Key *CLU_Hash_Key_Destroy(CLU_Hash_Key *a);
char CLU_Hash_Key_Cmp(CLU_Hash_Key *a, CLU_Hash_Key *b);

CLU_LUNode * CLU_LUNode_Create();
CLU_LUNode * CLU_LUNode_Destroy(CLU_LUNode *n);

#endif
