#pragma once

#include <windows.h>
#include "lwFrontAPI.h"

typedef struct LW_FRONT_API _TAGFILEHEADER{
	BYTE IDLength;
    BYTE ColormapType;
    BYTE ImageType;
    BYTE ColormapSpecification[5];
    WORD XOrigin;
    WORD YOrigin;
    WORD Width;
    WORD Height;
    BYTE PixelDepth;
    BYTE ImageDescriptor;
	_TAGFILEHEADER()
	{
		memset(this,0,sizeof(*this));
		ImageType=2;
		PixelDepth=32;
	}
}TGAFILEHEADER, FAR * LPTGAFILEHEADER;

class LW_FRONT_API CGraphicsFileData
{
private:
public:
	TGAFILEHEADER m_FileHeader;
	unsigned short GetWidth(){return m_FileHeader.Width;}
	unsigned short GetHeight(){return m_FileHeader.Height;}

	typedef struct _stFileData{
		BYTE B;
		BYTE G;
		BYTE R;
		BYTE A;
	}stFileData;
 
	union {
		stFileData* m_pstFileData;
		unsigned char *m_pFileData;
	};
    //int hxw;
    //int size_in_bytes;
    //int bits_per_pixel;
    int m_BytesPerPixel;
    //unsigned char *endptr;

	CGraphicsFileData(void);
	~CGraphicsFileData(void);

	void Draw(HDC hDC, RECT * pRect, COLORREF ct);
	HBITMAP GetBitmap(COLORREF ct);

    int LoadTGAFile2(const char* );
	void LoadTGAFile(BYTE* pBuf);
	int GetImageSize(int* width,int* height,const char* file);

#ifdef _MFC

	void LoadTGAFile(const char* );
	void LoadTGAFile(CArchive& ar);
	void SaveAsTex(LPCTSTR lpszFileName, int type);
	void SaveAsTex(CArchive& ar, int type);
#endif
	
};
