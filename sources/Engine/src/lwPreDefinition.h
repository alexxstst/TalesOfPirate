//
#pragma once

// all user preprocesser definition

// watch device video memory using(texture, vertexbuffer, indexbuffer allocation)
#define WATCH_VM_FLAG

// D3DPOOL_DEFAULT texture resource can only use device.updatetexture or device.updaterect method
#define CREATETEXTURE_SYSMEM_UPDATE_FLAG

//#define LOAD_TEXTURE_RESOURCE_DIRECTLY
//#define LOAD_VERTEX_RESOURCE_DIRECTLY

// use rsa struction
#define MTLTEXAGENT_OPACITY_RSA_FLAG

#define USE_D3DX_LOAD_TEXTURE

#define USE_MODEL_INFO_LOAD

// only defined macro USE_MODEL_INFO_LOAD can be available
#define USE_RES_BUF_MGR

#define LOAD_RES_BUF_RUNTIME

//#define USE_D3DX_LOAD_TEXTURE_UPDATETEXTURE
#define USE_D3DX_LOAD_TEXTURE_CREATEFROMMEMORY

#if(defined USE_D3DX_LOAD_TEXTURE_UPDATETEXTURE && defined USE_D3DX_LOAD_TEXTURE_CREATEFROMMEMORY)
#error "cannot defined USE_D3DX_LOAD_TEXTURE_UPDATETEXTURE and USE_D3DX_LOAD_TEXTURE_CREATEFROMMEMORY together"
#endif

//#define USE_STENCILBUFFER_FILTER_COLORKEY


#define USE_DDS_FILE

#define MULTITHREAD_LOADING_TEXTURE
#define MULTITHREAD_LOADING_TEXTURE_RT
#define MULTITHREAD_LOADING_MESH

//#if(!defined MULTITHREAD_LOADING_TEXTURE)
//#define RESTRICT_LOADING_TEXTURE
//#endif

#define USE_TEXLOG_MGR

#define OPT_CULLPRIMITIVE
