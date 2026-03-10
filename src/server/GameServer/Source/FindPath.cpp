#include "StdAfx.h"
#include "FindPath.h"

struct NODE_SHARED   //
{
	BYTE*			buf_ptr;
	BYTE			dire;
	NODE_SHARED*	last_node_ptr;
};

struct PATH_LINK
{
	BYTE dire;
	PATH_LINK *next_path_ptr;
};

static void DeletePathLink(PATH_LINK **ppPathLink)
{
	PATH_LINK *pPathLink = *ppPathLink;
	while(pPathLink!=NULL)
	{
		PATH_LINK *pNextPath = pPathLink->next_path_ptr;
		delete pPathLink;  
		pPathLink = pNextPath;
	}
	*ppPathLink = NULL;
}
// 
// 0,1, , , 
// , 
// 1, 

PATH_LINK* SearchPath(BYTE *block_buf , short width , short height , short sx , short sy , short tx , short ty)
{
	static NODE_SHARED node_list[STEP_LIMIT];
	static PATH_LINK path_link[STEP_LIMIT];


	if(block_buf==NULL) return NULL;

	short off[8];
	{
	off[0] =  width  * (-1);
	off[1] =  width;
	off[2] =  - 1;   
	off[3] =  1;   
	off[4] =  (width - 1) * (-1);  
	off[5] =  width + 1;
	off[6] =  width - 1;  
	off[7] =  (width + 1) * -1;
	}

	BYTE prior[8]; 
	prior[0] = 0;
	prior[1] = 4;
	prior[2] = 6;
	prior[3] = 2;
	prior[4] = 1; 
	prior[5] = 3;
	prior[6] = 5;
	prior[7] = 7;

	UINT32 node_count   = 0;
	UINT32 current_node = 0;
	UINT32 current_path = 0;

	BYTE *target_ptr = &block_buf[ty * width + tx];

	node_list[0].buf_ptr       = &block_buf[sy * width + sx];

	node_list[0].last_node_ptr = NULL;

	BYTE *last_buf_ptr , *new_buf_ptr;

	PATH_LINK *path_link_ptr = NULL;
	BOOL  found_flag = FALSE;

	//BYTE *temp_buf_ptr;

	UINT32 lMaxStep = (height -1) * (height -1);
	if(lMaxStep>STEP_LIMIT) lMaxStep = STEP_LIMIT; //

	BYTE *end_block = &block_buf[STEP_LIMIT - 1];

	while(!found_flag)
	{
		if(node_count>=lMaxStep) 
			break;

		if(current_node>node_count)
			break;

		last_buf_ptr = node_list[current_node].buf_ptr;
		
		for(register BYTE d = 0 ; d < 8 ; d++)
		{	 		
			new_buf_ptr = last_buf_ptr + off[d];
			if( new_buf_ptr < block_buf || new_buf_ptr > end_block )
			{
				goto skip;
			}
	
			if(*new_buf_ptr==0)
			{
				*new_buf_ptr = 1;
				node_count++;
				node_list[node_count].buf_ptr       = new_buf_ptr;
				node_list[node_count].dire          = d;
				node_list[node_count].last_node_ptr = &node_list[current_node];
				if(new_buf_ptr==target_ptr)
				{   
					NODE_SHARED *node_ptr     = &node_list[node_count];
					while(node_ptr->last_node_ptr)   
					{
						path_link[current_path].dire = prior[node_ptr->dire];
						path_link[current_path].next_path_ptr = path_link_ptr;
						path_link_ptr           = &path_link[current_path];
						node_ptr                = node_ptr->last_node_ptr;

    					current_path++;
					}
					found_flag = TRUE; 
					break;
				}
				if(node_count >= lMaxStep)
			 {
				 if(found_flag == TRUE)
				 {
					 found_flag = FALSE; 

					 //DeletePathLink(&path_link_ptr);
				 }
				 goto skip;
			 }
			}

		}
		current_node++;
	
	}
skip:
	if(found_flag)
	{ 
		return path_link_ptr;
	}
	return NULL;
}
