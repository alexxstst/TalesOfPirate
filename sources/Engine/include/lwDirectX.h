//
#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <d3d9types.h>

typedef IDirect3D9                 IDirect3DX;
typedef IDirect3DDevice9           IDirect3DDeviceX;
typedef IDirect3DTexture9          IDirect3DTextureX;
typedef IDirect3DVertexBuffer9     IDirect3DVertexBufferX;
typedef IDirect3DIndexBuffer9      IDirect3DIndexBufferX;
typedef IDirect3DSurface9          IDirect3DSurfaceX;
typedef IDirect3DVolume9           IDirect3DVolumeX;
typedef IDirect3DBaseTexture9      IDirect3DBaseTextureX;
typedef IDirect3DVolumeTexture9    IDirect3DVolumeTextureX;
typedef IDirect3DCubeTexture9      IDirect3DCubeTextureX;

typedef IDirect3DVertexShader9      IDirect3DVertexShaderX;
typedef IDirect3DVertexDeclaration9 IDirect3DVertexDeclarationX;
typedef IDirect3DVertexShaderX*     SHADER_TYPE;
typedef IDirect3DPixelShader9       IDirect3DPixelShaderX;

typedef D3DLIGHT9         D3DLIGHTX;
typedef D3DMATERIAL9      D3DMATERIALX;
typedef D3DVERTEXELEMENT9 D3DVERTEXELEMENTX;
typedef D3DVIEWPORT9      D3DVIEWPORTX;
typedef void              D3DLOCK_TYPE;

typedef D3DCAPS9 D3DCAPSX;


#define Direct3DCreateX         Direct3DCreate9

#define CreateVertexBufferX(Length, Usage, FVF, Pool, ppVertexBuffer, pHandle) \
    CreateVertexBuffer(Length, Usage, FVF, Pool, ppVertexBuffer, pHandle)

#define CreateIndexBufferX(Length, Usage, Format, Pool, ppIndexBuffer, pHandle) \
    CreateIndexBuffer(Length, Usage, Format, Pool, ppIndexBuffer, pHandle)

#define CreateTextureX(Width, Height, Level, Usage, Format, Pool, ppTexture, Handle) \
    CreateTexture(Width, Height, Level, Usage, Format, Pool, ppTexture, Handle)

#define CreateVolumeTextureX(Width, Height, Depth, Levels, Usage, Format, Pool, ppVolumeTexture, Handle) \
    CreateVolumeTexture(Width, Height, Depth, Levels, Usage, Format, Pool, ppVolumeTexture, Handle)

#define CreateCubeTextureX(EdgeLength, Levels, Usage, Format, Pool, ppCubeTexture, pHandle) \
    CreateCubeTexture(EdgeLength, Levels, Usage, Format, Pool, ppCubeTexture, pHandle)

#define CreateRenderTargetX(Width, Height, Format, MultiSample, MultisampleQuality, Lockable, ppSurface, pHandle) \
    CreateRenderTarget(Width, Height, Format, MultiSample, MultisampleQuality, Lockable, ppSurface, pHandle)

#define CreateDepthStencilSurfaceX(Width, Height, Format, MultiSample, MultisampleQuality, Discard, ppSurface, pHandle) \
    CreateDepthStencilSurface(Width, Height, Format, MultiSample, MultisampleQuality, Discard, ppSurface, pHandle)

#define SetStreamSourceX(StreamNum, StreamData, Offset, Stride) \
    SetStreamSource(StreamNum, StreamData, Offset, Stride)

#define SetIndicesX(pIndexData, BaseVertexIndex) \
    SetIndices(pIndexData)

#define DrawPrimitiveX(Type, StartVertex, PrimitiveCount) \
    DrawPrimitive(Type, StartVertex, PrimitiveCount)

#define DrawIndexedPrimitiveX(Type, BaseVertexIndex, MinIndex, NumVertices, StartIndex, PrimitiveCount) \
    DrawIndexedPrimitive(Type, BaseVertexIndex, MinIndex, NumVertices, StartIndex, PrimitiveCount)

#define GetDisplayModeX(SwapChain, pMode) \
    GetDisplayMode(SwapChain, pMode)

#define CheckDeviceMultiSampleTypeX(Adapter, DeviceType, SurfaceFormat, Windowed, MultiSampleType, pQualityLevels) \
    CheckDeviceMultiSampleType(Adapter, DeviceType, SurfaceFormat, Windowed, MultiSampleType, pQualityLevels)
