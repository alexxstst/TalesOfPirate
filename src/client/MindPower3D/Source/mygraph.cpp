#include "stdafx.h"
#include "mygraph.h"

IDirect3DDeviceX* g_pd3dDevice     = NULL; // Our rendering device


LPTEXTURE LoadTextureFromRawFile(char *strFileName)
{
	FILE *fp = fopen(strFileName , "rb");
	if(fp==NULL)
	{
		ToLogService("default", "Open File {} Error!(LoadTextureFromRawFile())", strFileName);
		return NULL;
	}

	int w , h , d;

	fread(&w , 4 , 1 , fp);
	fread(&h , 4 , 1 , fp);
	fread(&d , 4 , 1 , fp);

	ToLogService("default", "load raw file( w = {:3}  h = {:3}  d = {:3} )", w, h, d);

	int iImageSize = w * h * d;
	LPBYTE pbImageBuf = new BYTE[iImageSize];
	fread(pbImageBuf , iImageSize , 1 , fp);
	fclose(fp);
	
	IDirect3DTextureX* pTexture;
	if(D3DXCreateTexture(g_pd3dDevice , w , h , D3DX_DEFAULT , 0 , D3DFMT_A8R8G8B8 ,  D3DPOOL_MANAGED , &pTexture)!=D3D_OK)                              
	{
		ToLogService("default", "ERR : {}", "CreateTexture Failed");
		delete[] pbImageBuf;
		return NULL;
	}
	
	
	int x = 0 , y = 0;
	
	LPBYTE pbData = pbImageBuf + w * d * (h - 1);
	D3DLOCKED_RECT rcLock;
	if((pTexture->LockRect(0 , &rcLock , 0 , 0))==D3D_OK)
	{
		LPDWORD pdwTex = (LPDWORD)rcLock.pBits;
		int iPitch = rcLock.Pitch / 4;
		for(y = 0; y < h ; y++)
		{
			for(x = 0 ; x < w ; x++)
			{
				DWORD dwR = 0 , dwG = 0 , dwB = 0;
				dwR = *(pbData + 0); dwR<<=16;
				dwG = *(pbData + 1); dwG<<=8;
				dwB = *(pbData + 2); 
				*(pdwTex + x) = dwR|dwG|dwB;
				pbData+=d;
			}
			pbData-=(w * d * 2);
			pdwTex+=iPitch;
		}
		pTexture->UnlockRect(0);
	}
	else
	{
		ToLogService("default", "ERR : {}", "Lock Texture Failed!");
		SAFE_RELEASE(pTexture);
	}
	
	delete[] pbImageBuf;
	return pTexture;
}


