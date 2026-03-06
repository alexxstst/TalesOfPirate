//
#pragma once

#include "lwHeader.h"
#include "lwExpObj.h"
#include "lwFrontAPI.h"
#include "lwITypes.h"

class CGraphicsFileData;


#ifdef USE_MINDPOWER
#include "GlobalInc.h"
#endif

LW_BEGIN


//
#define LW_RGB555_R(rgb) (BYTE)( ( rgb & 0x7c00) >> 7 )
#define LW_RGB555_G(rgb) (BYTE)( ( rgb & 0x3e0) >> 2 )
#define LW_RGB555_B(rgb) (BYTE)( ( rgb & 0x1f) << 3 )

#define LW_RGB565_R(rgb) (BYTE)( ( rgb & 0xf800) >> 8 )
#define LW_RGB565_G(rgb) (BYTE)( ( rgb & 0x7e0) >> 3 )
#define LW_RGB565_B(rgb) (BYTE)( ( rgb & 0x1f) << 3 )

#define LW_RGB555TODWORD(rgb) (DWORD)( LW_RGB555_R(rgb) | (LW_RGB555_G(rgb)<<8) | (LW_RGB555_B(rgb)<<16) )
#define LW_RGB565TODWORD(rgb) (DWORD)( LW_RGB565_R(rgb) | (LW_RGB565_G(rgb)<<8) | (LW_RGB565_B(rgb)<<16) )


#define LW_RGBDWORDTO555(color) (WORD)( ((color&0xf8)<<7) | ((color & 0xf800)>>6) | ((color & 0xf80000)>>19) )
#define LW_RGBDWORDTO565(color) (WORD)( ((color&0xf8)<<8) | ((color & 0xfC00)>>5) | ((color & 0xf80000)>>19) )

inline WORD lwGetRGB555WithDWORD(DWORD color) { return LW_RGBDWORDTO555(color); }
inline WORD lwGetRGB565WithDWORD(DWORD color) { return LW_RGBDWORDTO565(color); }



struct lwTexDataInfo
{
    void* data;
    DWORD size;
    DWORD pitch;
    DWORD width;
    DWORD height;
};

struct lwMeshDataInfo
{
    void* vb_data;
    void* ib_data;

    DWORD vb_size;
    DWORD vb_stride;

    DWORD ib_size;
    DWORD ib_stride;
};

inline void lwMeshDataInfo_Construct(lwMeshDataInfo* obj)
{
    obj->vb_data = 0;
    obj->ib_data = 0;
    obj->vb_size = 0;
    obj->vb_stride = 0;
    obj->ib_size = 0;
    obj->ib_stride = 0;
}
inline void lwMeshDataInfo_Destruct(lwMeshDataInfo* obj)
{
    LW_IF_DELETE_A(obj->vb_data);
    LW_IF_DELETE_A(obj->ib_data);
}

/////////////////
#ifndef USE_MINDPOWER
LW_FRONT_API void lwMessageBox( const char* fmt, ... );
#define LG_MSGBOX lwMessageBox
#else
#ifndef USE_LG_MSGBOX
#define USE_LG_MSGBOX
inline void __cdecl LGX(const char* format, ...)
{
    char buf[512];
    buf[0] = 'm';
    buf[1] = 's';
    buf[2] = 'g';
    buf[3] = 0;
    
    va_list args;
    va_start( args, format );
    _vsntprintf( &buf[3], 512, format, args );
    va_end( args );

    LG("default", buf);
}

#define LG_MSGBOX LGX
#endif

#endif

LW_FRONT_API float lwGetFPS();

LW_FRONT_API void lwConvertTgaDataToColorValue(lwColorValue4b* dst_data,const CGraphicsFileData* tga, DWORD colorkey_type, lwColorValue4b* colorkey );
LW_FRONT_API int lwLoadColorValue( lwColorValue4b** buf, int* width, int* height, const char* file );
LW_FRONT_API void lwFreeColorValue( lwColorValue4b* buf );
//int lwLoadTexture( IDirect3DTextureX** tex, IDirect3DDeviceX* dev, const char* file, DWORD level, DWORD usage, D3DFORMAT fmt, D3DPOOL pool );

LW_FRONT_API LW_RESULT lwLoadTexture( IDirect3DTextureX** tex, IDirect3DDeviceX* dev, const char* file, DWORD usage, D3DFORMAT fmt, D3DPOOL pool );
LW_FRONT_API LW_RESULT lwLoadTexDataInfo(lwTexDataInfo* info, const char* file, DWORD format, DWORD colorkey_type, lwColorValue4b* colorkey, int use_power_size);
LW_FRONT_API LW_RESULT lwLoadMeshDataInfo( lwMeshDataInfo* info, const lwMeshInfo* mi );
LW_FRONT_API DWORD lwGetTexFlexibleSize( DWORD size );

LW_FRONT_API void lwScreenToWorld( lwVector3* org, lwVector3* ray, int x, int y, int width, int height, const lwMatrix44* mat_proj, const lwMatrix44* mat_view );
LW_FRONT_API void lwWorldToScreen( int* x, int* y, float* z, const lwVector3* vec, int width, int height, const lwMatrix44* mat_proj, const lwMatrix44* mat_view );

LW_FRONT_API void lwGetBoxVertLineList( lwVector3* vert_seq, const lwBox* box );
LW_FRONT_API void lwGetBoxTriangleList( lwVector3* vert_seq, DWORD* index_seq, const lwBox* box );
LW_FRONT_API void lwGetBoxTriangleList(lwVector3* vert36_seq, lwVector3* normal36_seq, const lwVector3* size);
LW_FRONT_API void lwBuildVertexNormalWithTriangleList(lwVector3* normal_seq, const lwVector3* vert_seq, const DWORD* index_seq, DWORD vert_num, DWORD index_num);

LW_FRONT_API void lwGetCubeMapViewMatrix(lwMatrix44* mat, DWORD face);

LW_FRONT_API LW_RESULT lwHitTestBox( lwPickInfo* info, const lwVector3* org, const lwVector3* ray, const lwBox* box, const lwMatrix44* mat );

LW_FRONT_API LW_RESULT lwSetRenderState( lwRenderStateValue* rs_seq, DWORD rs_num, DWORD state, DWORD value );
LW_FRONT_API LW_RESULT lwClearRenderState( lwRenderStateValue* rs_seq, DWORD rs_num, DWORD state );


LW_FRONT_API LW_RESULT lwExtractMeshData(lwMeshInfo* info, void* vb_data, void* ib_data, DWORD vert_num, DWORD index_num, D3DFORMAT vb_fvf, D3DFORMAT ib_fvf);
LW_FRONT_API DWORD lwGetSurfaceSize(UINT width, UINT height, D3DFORMAT format);
LW_FRONT_API LW_RESULT lwGetDirectXVersion(char* o_buf, DWORD version);
//
LW_END
