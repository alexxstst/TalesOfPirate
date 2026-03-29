/*//\\//\\//\\//\\//\\//\\//\\//\\////\\//\\//\\//\\//\\//\\//\\//\\////\\//\\//\\//\\
  CaLua Script Binding Utility
  By Justin Reynen, 2003/2004

  Main internal header file

  Where CaLua keeps track of all of it's guts...
//\\//\\//\\//\\//\\//\\//\\//\\////\\//\\//\\//\\//\\//\\//\\//\\////\\//\\//\\//\\*/

#ifndef __CLU_GUTS_H
#define __CLU_GUTS_H



#include <CaLua.h>
#include "CaLua_helper.h"
#include "CaLua_hash.h"

//////
// C data types
//////

#define CLU_POINTER_BIT		(1<<31)
#define CLU_UNSIGNED_BIT	(1<<30)
#define CLU_TYPE_FIELD 0x3FFFFF

#define CLU_UNKNOWN		0
#define CLU_CHAR		1
#define CLU_SHORT		2
#define CLU_INT			3
#define CLU_FLOAT		4
#define CLU_DOUBLE		5
#define CLU_STRING		6
#define CLU_POINTER		7

#define CLU_NUM_C_TYPES 7

//////
// Some handy defines
//////
#define CLU_IS_UNSIGNED(x)			( x&CLU_UNSIGNED_BIT )
#define CLU_IS_POINTER(x)			( x&CLU_POINTER_BIT )
#define CLU_IS_STRUCTURE_POINTER(x) ( (x&CLU_POINTER_BIT) && ( (x&CLU_TYPE_FIELD) > CLU_NUM_C_TYPES)
#define CLU_IS_STRUCTURE(x)			( !(x&CLU_POINTER_BIT) && ( (x&CLU_TYPE_FIELD) > CLU_NUM_C_TYPES) )
#define CLU_TYPE_TO_INDEX(x)			( (x&CLU_TYPE_FIELD) - CLU_NUM_C_TYPES - 1)
#define CLU_C_BASE_TYPE(x)			(x&CLU_TYPE_FIELD)

//////
// The CLU virtualMachine stack stuff
//////
#define CLU_VM_STACK_SIZE			3
#define CLU_STATE_STACK_SIZE	16


/////
// These are registered structures that are known to CaLua
// The numbers that appear here are the order in which they are registered in CLU_Init();
////
#define CLUARRAY_REG		 0


typedef struct StructForm
{
	char*	name;						// Registration name
	int		regNum;						// Registration number (Index into global array which THIS structure appears)
	char*	signature;					// Member signature form, as supplied by the user
	
	char**	memberName;					//DA of member names
	int*	memberType;					//DA of member data types (will be a C data type, or the index of a registered structure, ignore the top bit!!)
	int*	memberSize;					//DA of member data sizes
	int*	NextMemberOffset;			//DA of offsets to the next data member

	int		numMembers;					//Total number of members

	int		size;						//Total size of structure (including alignment offsets)
} StructForm;


typedef struct FuncForm
{
	int		callType;					// The calling convention, either CLU_CDECL or CLU_STDCALL
	void	(*fPtr)(void);				// The actual function pointer
	char*	name;						// Registration name.
	
	char*	passArgs;					// Signature of arguments to pass to the function
	char*	retArgs;					// Signature of the one return argument, or NULL for none

	int*	passType;					// DA of argument C data types
	int		retType;					// ONE C type of returning arguments...

	int		numRetArgs;					// Total number of return arguments (1 or 0)
	int		numPassArgs;				// Total number of passed arguments

} FuncForm;


typedef struct CStack					//Represents the C hardware stack as a series of bytes
{
	char*	data;
	int		dataLength;
} CStack;


typedef struct StackData				//Represents a list of C conversions of the Lua stack data
{
	int*	dataSize;
	void**	data;
	int		numArgs;
}StackData;


typedef struct VMSlate					//All of the values in here represent global variables that need to be stored for each state
{
	lua_State*		_virtualMachine;

	CLU_AVL*		_baseTypeLU;
	
	StructForm**	_structs;
	int				_numStructs;
	CLU_AVL*		_structsLU;

	FuncForm**		_funcs;
	int				_numFuncs;
	CLU_AVL*		_funcsLU;

	char			_used;			//Is this 
	
}VMSlate;

/////
// Lua API Structures
/////

typedef struct CLUArray					//C version of Lua array structures
{
	void* data;							//Actual structure data
	unsigned short size;				//array size (number of elements)
	int type;							//CaLua type of the array elements
	int elementSize;					//The size of each array element (in bytes)

	char* valid;						//Points to either a 1 or 0, indicating whether this particular array structure has been invalidated or not...
} CLUArray;


extern char* CLUArraySignature;					//Defined in CaLua_luafunc.c, the data signature for struct Lua Array

//Possible values for a VMSlate

extern CLU_AVL*		baseTypeLU;					//A Quick way to look up base types in GetDataType

extern StructForm** structs;					//List of registered structures
extern int			numStructs;
extern CLU_AVL*		structsLU;					//Quick Name Lookup Table

extern FuncForm**	funcs;						//List of registered functions
extern int			numFuncs;
extern CLU_AVL*		funcsLU;					//Quick Name Lookup Table

extern lua_State*	virtualMachine;				//Main virtual machine

extern lua_State*	vmStack[CLU_VM_STACK_SIZE];	// virtualMachine stack, used only by CaLua calls
extern int			vmStackPos;					// How deep we are into the stack..

extern VMSlate		stateStack[CLU_STATE_STACK_SIZE];		//The stack that will be used to push whole CaLua states
extern int			currentState;							//How deep we're in this stack

/////
//Prototypes for CaLua functions exposed to Lua
/////
int		Handler					(lua_State* vm);		//Prototype for our Lua->C handler
int		LuaFree					(lua_State* vm);		//Frees a pointer created by CaLua
//Array Stuff
int		Array_Create			(lua_State* vm);		//Creates a new array of a given type
int		Array_Destroy			(lua_State* vm);		//Destroys an array
int		Array_Get				(lua_State* vm);		//Retrieves an indicie from an index
int		Array_Insert			(lua_State* vm);		//Inserts data in a given array location
int		Array_InsertArray		(lua_State* vm);		//Insert the contents of another array into an array
int		Array_Append			(lua_State* vm);		//Adds some data onto the end of an array
int		Array_CharToString		(lua_State* vm);		//For turning an array of chars into a string
int		Array_Delete			(lua_State* vm);		//Deletes data from an array index
//Conversion Stuff
int		LUD_To_Number			(lua_State* vm);		//Pointer to a number
int		LUD_To_String			(lua_State* vm);		//Pointer to a string
int		LUD_To_Table			(lua_State* vm);		//Turn LUD into a registered table
int		Table_To_LUD			(lua_State* vm);		//Turn a table into LUD, which must then be freed
int		String_To_LUD			(lua_State* vm);		//Turns a table into LUD
int		Number_To_LUD			(lua_State* vm);		//Turns a table into LUD
//Misc
int		UpdateLUD				(lua_State*	vm);		//Takes in a LUD and updates it with C data converted from Lua type data


/////
//Guts functions that other parts of CaLua might use
/////

/////
// CaLua_function.c
/////
int		ParseReturnArgument	(int cType, int ea, int ed, double fs);
CStack* PrepareStack		(int rFunc);
int		FreeCStack			(CStack* in);
int		FreeStackData		(StackData* in);
int		To4ByteBoundary		(int s);

/////
// CaLua_parser.c
/////
int		GetFunctionIndex	(char* func);
int		GetTableIndex		(char* tab);
int		GetDataSize			(int t);
int		GetDataType			(char* in);
int		GetNumberOfTokens	(char* in);
char*	GetToken			(char* str, int n);
char*	ParseDataName		(char* in);
char*	ParseDataType		(char* in);

/////
// CaLua_stack.c
/////
void*	PopRegTable			(int regStruct);
void*	PopValuesBySig		(char* desc);
int		PushStructure		(int regStruct, void* data);
void*	ConvertLuaStackData	(int pos, int cType, int* size);
int		CToLuaData			(int type, void* data);

/////
// CaLua_structure.c
/////
int		ComputeStructureSize		(StructForm* s);
int		ComputeStructureAlignment	(StructForm* s);
void	FreeStructure				(void* s, int rStruct);
void*	Pack						(int nElements, void** data, int* type);
int		NextAlignment				(int offset, int type);

/////
// CaLua_vmstack.c
/////
void		PushVM	(lua_State* vm);
lua_State*	PopVM	(void);
int			SaveNewState();
void UpdateState(int state);
int			ClearState(int state);
#endif
