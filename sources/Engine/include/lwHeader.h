//
#pragma once

#define LW_NAMESPACE            MindPower
#define LW_BEGIN                namespace LW_NAMESPACE {
#define LW_END                  }
#define LW_USING                using namespace LW_NAMESPACE;


#define LW_MAX_PATH             260
#define LW_MAX_FILE             128
#define LW_MAX_NAME             64
#define LW_MAX_STRING           256
#define LW_CHAR_16              16
#define LW_CHAR_32              32

#define LW_MALLOC(obj)          malloc(obj)
#define LW_REALLOC(p, obj)         realloc(p, obj)
#define LW_FREE(obj)            free(obj)
#define LW_NEW(obj)             new obj
#define LW_DELETE(obj)          delete obj
#define LW_DELETE_A(obj)        delete [] obj
#define LW_RELEASE(obj)         (obj)->Release()

#ifndef LW_SAFE_FREE
#define LW_SAFE_FREE(obj) \
    { \
    if(obj) { \
        LW_FREE(obj); \
        obj = 0; \
    } \
    }
#endif

#ifndef LW_IF_FREE
#define LW_IF_FREE(obj) \
    if(obj) { \
        LW_FREE(obj); \
    }
#endif


#ifndef LW_SAFE_DELETE
#define LW_SAFE_DELETE(obj) \
    if( obj ) { \
        LW_DELETE(obj); \
        obj = 0; \
    }
#endif

#ifndef LW_IF_DELETE
#define LW_IF_DELETE(obj) \
    if(obj) { \
        LW_DELETE(obj); \
    }
#endif

#ifndef LW_SAFE_DELETE_A
#define LW_SAFE_DELETE_A(obj) \
    if( obj ) { \
        LW_DELETE_A(obj); \
        obj = 0; \
    }
#endif

#ifndef LW_IF_DELETE_A
#define LW_IF_DELETE_A(obj) \
    if(obj) { \
        LW_DELETE_A(obj); \
    }
#endif

#ifndef LW_SAFE_RELEASE
#define LW_SAFE_RELEASE(obj) \
    { \
    if(obj) { \
        LW_RELEASE(obj); \
        obj = 0; \
    } \
    }
#endif

#ifndef LW_IF_RELEASE
#define LW_IF_RELEASE(obj) \
    if(obj) { \
        LW_RELEASE(obj); \
    }
#endif

#define LW_SIMPLE_IF_GET(dst, src) if(dst) { *dst = src; }

#define LW_SUCCEEDED_RET(code) if(LW_SUCCEEDED(code)) { goto __ret; }
#define LW_FAILED_RET(code) if(LW_FAILED(code)) { goto __ret; }
#define LW_ZERO_RET(code) if((code) == 0) { goto __ret; }


#define LW_INVALID_INDEX		0xffffffff
#define LW_INVALID_HANDLE       LW_INVALID_INDEX

typedef void LW_VOID;
typedef char LW_CHAR;
typedef int LW_BOOL;
typedef long LW_LONG;
typedef unsigned long LW_ULONG;
typedef __int64 LW_INT64;
typedef LW_LONG LW_RESULT;
typedef LW_ULONG LW_DWORD;
typedef LW_ULONG LW_HANDLE;

// ===================
typedef char lxi8;
typedef short lxi16;
typedef int lxi32;
typedef __int64 lxi64;
typedef unsigned char lxu8;
typedef unsigned short lxu16;
typedef unsigned int lxu32;
typedef unsigned __int64 lxu64; // ???
typedef float lxf32;
typedef double lxf64;
typedef void lxvoid;
typedef lxu32 lxdword;
typedef lxi32 lxresult;
typedef lxi32 lxhandle;
// end

#define LW_INLINE __forceinline
#define LW_DECLSPEC_NOVTABLE __declspec( novtable )
#define PURE_METHOD = 0

#define LW_NULL                 0
#define LW_MAKEINT64( l, h ) ( ( ((LW_INT64)l & 0xffffffff) | (((LW_INT64)h & 0xffffffff) << 32)) )
#define LW_LODWORD( i ) ( (LW_DWORD)( ((LW_INT64)i) & 0xffffffff ) )
#define LW_HIDWORD( i ) ( (LW_DWORD)( (LW_DWORD)(((LW_INT64)i) >> 32) & 0xffffffff) )

#define LW_ALIGN(adrs, bytes) (((UINT)adrs + ((bytes)-1)) & (~((bytes)-1)))
#define LW_ARRAY_LENGTH(buf) (sizeof(buf) / sizeof(buf[0]))

#define LW_RET_OK           0
#define LW_RET_FAILED       -1
