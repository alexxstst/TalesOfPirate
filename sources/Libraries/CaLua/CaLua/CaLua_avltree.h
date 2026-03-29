/*
	Serpent Engine
	Dan Carter, 2003
*/

#ifndef __CLU_AVLTREE_H
#define __CLU_AVLTREE_H

#include "CaLua_helper.h"

#define CLU_AVL_MALLOC(x) CLU_Calloc(x);
#define CLU_AVL_FREE(x) SAFE_FREE(x);


typedef struct CLU_AVL_Node {
	struct CLU_AVL_Node *left, *right;
	void *d;
	void *key;
	char skew;
} CLU_AVL_Node;

typedef struct CLU_AVL {
	CLU_AVL_Node *root;
	int num;							//Number of nodes in tree
	char (*cmpKeys)(void *a, void *b);	//0 if equal, 1 if a > b, -1 if a < b
	void *(*destroyData)(void * d);
	void *(*destroyKey)(void * key);
} CLU_AVL;

CLU_AVL * CLU_AVL_Create(void * destroyData, void * destroyKey, void * cmpKeys);
CLU_AVL * CLU_AVL_Destroy(CLU_AVL * avl);

//Returns 0 on error, non-zero on success
int CLU_AVL_Insert(CLU_AVL * avl, void *d, void *key);
int CLU_AVL_Remove(CLU_AVL * avl, void *key);
void * CLU_AVL_Access(CLU_AVL * avl, void *key);

//Returns list of data, smallest key first
void ** CLU_AVL_MinList(CLU_AVL * avl);

//Returns list of data, largest key first
void ** CLU_AVL_MaxList(CLU_AVL * avl);

#endif

