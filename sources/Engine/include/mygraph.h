#include "lwDirectX.h"
#define LPSURFACE IDirect3DSurfaceX*
#define LPTEXTURE IDirect3DTextureX*
#define LPD3D	  IDirect3DX*
#define LPDEVICE  IDirect3DDeviceX*


LPTEXTURE LoadTextureFromRawFile(char* strFileName);

extern LPDEVICE g_pd3dDevice;
