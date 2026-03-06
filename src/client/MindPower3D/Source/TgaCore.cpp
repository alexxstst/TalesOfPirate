//#include "stdafx.h"
#include "stdafx.h"
#include <stdio.h>

#include "TgaCore.h"
//#include <string>
#include <assert.h>
//#include "tex.h"
#include "blk.h"
//using namespace std;

//unsigned char	m_id_length;        /* 0 */
//unsigned char	m_color_map_type;   /* 1 */
//unsigned char	m_type;				/* 2 */
//unsigned short	m_cm_index;         /* 3 4 */
//unsigned short	m_cm_length;        /* 5 6 */
//unsigned char	m_cm_entry_size;    /* 7 */
//unsigned short	m_x_org;			/* 8 9 */
//unsigned short	m_y_org;			/* 10 11 */
//unsigned char	m_desc;				/* 17 */



#define UL_TGA_LR 0x10
#define UL_TGA_BT 0x20




#define UL_GFX_SWAP_RGB     0x0001
#define UL_GFX_PAD          0x0004
#define UL_GFX_COMPRESSED   0x0008
#define UL_GFX_SIGNEDPAD    0x0010
#define UL_GFX_INVERT_VERT  0x0020
#define UL_GFX_INVERT_HORIZ 0x0040


#define UL_GFX_565          0x00020000

#ifndef bread
#define bread() ((BYTE)fgetc(f))
#endif
#ifndef wread
#define wread() ( ((BYTE)bread()) | (((BYTE)bread())<<8) )
#endif

//int s_Width,s_Height;
//bool g_IsBuilding = false;

CGraphicsFileData::CGraphicsFileData(void)
{
	m_pFileData=NULL;
	memset(&m_FileHeader,0,sizeof(m_FileHeader));
}

CGraphicsFileData::~CGraphicsFileData(void)
{
	if (m_pFileData)
        delete [] m_pFileData;
}

//void CGraphicsFileData::LoadTGAFile(const char* filename)
//{
//	CGraphicsFileData &m = *this;
//	FILE *f;
//
//	f = fopen(filename, "rb");
//	assert(f);
//
//	f = f;
//
//	unsigned char *data;
//
//	const int flags = 0;
//
//	int i, rle;
//	int a, r, g, b;
//	int right, top;
//	unsigned short bits16;
//
//	m_id_length = bread();
//	m_color_map_type = bread();
//	m_type = bread();
//
//	assert(m_type == 2 || m_type == 10 || m_type == 3);
//
//	m_cm_index = wread();
//	m_cm_length = wread();
//
//	m_cm_entry_size = bread();
//
//	m_x_org = wread();
//	m_y_org = wread();
//	m.dwWidth = wread();
//	m.dwHeight = wread();
//
//	s_Width = m.dwWidth;
//	s_Height = m.dwHeight;
//
//	m.bits_per_pixel = bread();
//	switch(m.bits_per_pixel)
//	{
//	case 8:
//		m_BytesPerPixel = 4;
//		break;
//	case 16:
//		m_BytesPerPixel = 2;
//		break;
//	case 24:
//		m_BytesPerPixel = 3;
//		break;
//	case 32:
//		m_BytesPerPixel = 4;
//		break;
//	}
//
//	m_desc = bread();
//
//	if (m_pFileData)
//		delete m_pFileData;
//	m_pFileData = (unsigned char *) new unsigned long[m.dwWidth * m.dwHeight];
//	assert(m_pFileData);
//
//	right = m_desc & UL_TGA_LR;
//	top = m_desc & UL_TGA_BT;
//
//	fread((char *) arId, m_id_length, 1, f);
//	fread((char *) arColorPalette, m_cm_length, m_cm_entry_size >> 3, f);
//
//	data = (unsigned char *)m_pFileData;
//
//	hxw = m.dwHeight * m.dwWidth;
//	switch (m_type)
//	{
//	case 3:
//		for (i = 0; i < hxw; i++)
//		{
//			a = bread();
//
//			*data++ = a;
//			*data++ = a;
//			*data++ = a;
//			/*
//			if (a == 0)
//			{
//			*data++ = 255;
//			}
//			else
//			{
//			*data++ = a;
//			}
//			*/
//			*data++ = a;
//		}
//		break;
//
//	case 2:
//		switch(m.bits_per_pixel)
//		{
//		case 16:
//			for (i = 0; i < hxw; i++)
//			{
//				bits16 = wread();
//				b = bits16 & 0x1F;
//				g = (bits16 >> 5) & 0x1F;
//				r = (bits16 >> 10) & 0x1F;
//
//				b <<= 3;
//				g <<= 3;
//				r <<= 3;
//				if (flags & UL_GFX_SWAP_RGB)
//				{
//					*data++ = r;
//					*data++ = g;
//					*data++ = b;
//				}
//				else
//				{
//					*data++ = b;
//					*data++ = g;
//					*data++ = r;
//				}
//
//				if (flags & UL_GFX_PAD)
//					*data++ = 0xFF;
//				else if (flags & UL_GFX_SIGNEDPAD)
//				{
//					if (r & 0x80)
//						*data++ = 0xFF;
//					else
//						*data++ = 0;
//				}
//				else
//					*data++ = 0;
//
//
//			}
//			break;
//
//		case 24:
//			for (i = 0; i < hxw; i++)
//			{
//				b = bread();
//				g = bread();
//				r = bread();
//				if (flags & UL_GFX_SWAP_RGB)
//				{
//					*data++ = r;
//					*data++ = g;
//					*data++ = b;
//				}
//				else
//				{
//					*data++ = b;
//					*data++ = g;
//					*data++ = r;
//				}
//
//				if (flags & UL_GFX_PAD)
//					*data++ = 0xFF;
//				else if (flags & UL_GFX_SIGNEDPAD)
//				{
//					if (r & 0x80)
//						*data++ = 0xFF;
//					else
//						*data++ = 0;
//				}
//				else
//					*data++ = 0;
//
//			}
//			break;
//		case 32:
//			for (i = 0; i < hxw; i++)
//			{
//				b = bread();
//				g = bread();
//				r = bread();
//				a = bread();
//				if (flags & UL_GFX_SWAP_RGB)
//				{
//					*data++ = r;
//					*data++ = g;
//					*data++ = b;
//					*data++ = a;
//				}
//				else
//				{
//					*data++ = b;
//					*data++ = g;
//					*data++ = r;
//					*data++ = a;
//				}
//			}
//		}
//		break;
//
//case 10:   //RLE TRUE color
//	switch(m.bits_per_pixel)
//	{
//	case 24:
//		i = 0;
//		while (i < hxw)
//		{
//
//			rle = bread();
//			if (rle < 0x80)
//			{
//				// Raw data
//				// encoded as n - 1
//				rle++;
//				// # of pixels
//				while (rle)
//				{
//					b = bread();
//					g = bread();
//					r = bread();
//					if (flags & UL_GFX_SWAP_RGB)
//					{
//						*data++ = r;
//						*data++ = g;
//						*data++ = b;
//					}
//					else
//					{
//						*data++ = b;
//						*data++ = g;
//						*data++ = r;
//					}
//					if (flags & UL_GFX_PAD)
//						*data++ = 0xFF;
//					else if (flags & UL_GFX_SIGNEDPAD)
//					{
//						if (r & 0x80)
//							*data++ = 0xFF;
//						else
//							*data++ = 0;
//					}
//					else
//						*data++ = 0;
//
//
//					rle--;
//					i++;
//					if (i > hxw)
//					{
//						//printf("RLE overflow\n");
//						delete m_pFileData;
//
//						assert(false);
//					}
//				}
//			}
//			else
//			{
//				rle -= 127;
//				b = bread();
//				g = bread();
//				r = bread();
//
//				// # of rle
//				while (rle)
//				{
//					if (flags & UL_GFX_SWAP_RGB)
//					{
//						*data++ = r;
//						*data++ = g;
//						*data++ = b;
//					}
//					else
//					{
//						*data++ = b;
//						*data++ = g;
//						*data++ = r;
//					}
//					if (flags & UL_GFX_PAD)
//						*data++ = 0xFF;
//					else if (flags & UL_GFX_SIGNEDPAD)
//					{
//						if (r & 0x80)
//							*data++ = 0xFF;
//						else
//							*data++ = 0;
//					}
//					else
//						*data++ = 0;
//
//
//
//					i++;
//					if (i > hxw)
//					{
//						//printf("RLE overflow\n");
//						delete m_pFileData;
//
//						assert(false);
//					}
//					rle--;
//				}
//			}
//
//		}
//		break;
//
//case 32: // RLE
//	i = 0;
//	while (i < hxw)
//	{
//
//		rle = bread();
//		if (rle < 0x80)
//		{
//			rle++;
//			while (rle)
//			{
//				if (flags & UL_GFX_SWAP_RGB)
//				{
//					b = bread();
//					g = bread();
//					r = bread();
//					a = bread();
//					*data++ = r;
//					*data++ = g;
//					*data++ = b;
//					*data++ = a;
//				}
//				else
//				{
//
//					*data++ = bread();
//					*data++ = bread();
//					*data++ = bread();
//					*data++ = bread();
//				}
//				rle--;
//				i++;
//				if (i > hxw)
//				{
//					//printf("RLE overflow\n");
//					//exit(116);
//					delete m_pFileData;
//
//					assert(false);
//				}
//
//			}
//		}
//		else
//		{
//			rle -= 127;
//			b = bread();
//			g = bread();
//			r = bread();
//			a = bread();
//
//			while (rle)
//			{
//				if (flags & UL_GFX_SWAP_RGB)
//				{
//					*data++ = r;
//					*data++ = g;
//					*data++ = b;
//					*data++ = a;
//				}
//				else
//				{
//					*data++ = b;
//					*data++ = g;
//					*data++ = r;
//					*data++ = a;
//				}
//				rle--;
//				i++;
//				if (i > hxw)
//				{
//					//printf("RLE overflow\n");
//					delete m_pFileData;
//
//					assert(false);
//				}
//			}
//		}
//
//	}
//	break;
//}
//}
//
//fclose(f);
//f = f = NULL;
//
//if (!top)
//{
//	unsigned long *swap = (unsigned long *) new unsigned long[hxw];
//
//	memcpy(swap, m_pFileData, hxw * sizeof(unsigned long));
//
//	unsigned long * src, * dest;
//	unsigned long * src_data = (unsigned long *)m_pFileData;
//
//	for (i = 0; i < m.dwHeight; i++)
//	{
//		src = &swap[(m.dwHeight - i - 1) * m.dwWidth];
//		dest = &src_data[i * m.dwWidth];
//
//		memcpy(dest, src, m.dwWidth * sizeof(unsigned long) );
//	}
//
//	delete swap;
//}
//}

int CGraphicsFileData::GetImageSize(int* width,int* height,const char* file)
{
    FILE* fp = fopen(file,"rb");
    if( fp == 0 )
        return 0;

	TGAFILEHEADER tga_head;
	memset(&tga_head,0,sizeof(tga_head));

	fread(&tga_head,sizeof(tga_head),1,fp);

	*width = tga_head.Width;
	*height = tga_head.Height;

	fclose(fp);

	return 1;
}

int CGraphicsFileData::LoadTGAFile2(const char* filename)
{
    FILE* fp = fopen(filename,"rb");
    if( fp == 0 )
        return 0;

    fseek(fp,0,SEEK_END);
    long len = ftell(fp);

	BYTE* pBuf=new BYTE[len];
    fseek(fp,0,SEEK_SET);
    fread(pBuf,sizeof(BYTE)*len,1,fp);
	LoadTGAFile(pBuf);
	delete [] pBuf;

    fclose(fp);

    return 1;
}


void CGraphicsFileData::Draw(HDC hDC, RECT * pRect, COLORREF ct)
{
	int x,y;

	float fW = (float)GetWidth()/(pRect->right-pRect->left);
	float fH = (float)GetHeight()/(pRect->bottom-pRect->top);

	for(y=pRect->top;y<=pRect->bottom;y++)
	{
		for(x=pRect->left;x<=pRect->right;x++)
		{
			int x1,y1;
			x1=(int)( x * fW );
			y1=(int)( y * fH );
			int index=y1*GetWidth()+x1;
			float f1,f2;
			f1=m_pstFileData[index].A/255.0f;
			f2=1-f1;
			COLORREF c1=::GetPixel(hDC,x,y);
			COLORREF color=RGB(this->m_pstFileData[index].R*f1+GetRValue(c1)*f2,
				this->m_pstFileData[index].G*f1+GetGValue(c1)*f2,
				this->m_pstFileData[index].B*f1+GetBValue(c1)*f2);
			::SetPixel(hDC,x,y,color);
		}
	}
}

//#include "resource.h"

HBITMAP CGraphicsFileData::GetBitmap(COLORREF ct)
{
	HBITMAP ret=NULL;
	if(m_pFileData==NULL)
		return ret;
	
	HDC hdc=GetDC(NULL);
	
    int cx=GetWidth();
	int cy=GetHeight();

	int bitWidth=cx * 3;
	
	if(bitWidth % 4)
	{
		bitWidth +=( 4 - (bitWidth % 4));
	}

	BYTE* pBmp=new BYTE[bitWidth * cy];

	int x,y;
	int index=0;
	int ix=0;
	for(y=0;y<cy;y++)
	{
		int ix1=ix;
		for(x=0;x<cx;x++)
		{
			UINT f1,f2;
			f1=m_pstFileData[index].A;
			f2=255-f1;
			pBmp[ ix1 + 0] = this->m_pstFileData[index].B * f1/255 + GetBValue(ct) *f2/255;
			pBmp[ ix1 + 1] = this->m_pstFileData[index].G * f1/255 + GetGValue(ct) *f2/255;
			pBmp[ ix1 + 2] = this->m_pstFileData[index].R * f1/255 + GetRValue(ct) *f2/255;
			index++;
			ix1+=3;
		}
		ix+=bitWidth;
	}

	BITMAPINFO bmi;
	memset(&bmi,0,sizeof(bmi));
	bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth       = cx;
    bmi.bmiHeader.biHeight      = -cy; // top-down image
    bmi.bmiHeader.biPlanes      = 1;
    bmi.bmiHeader.biBitCount    = 24;
    bmi.bmiHeader.biCompression = BI_RGB;
    bmi.bmiHeader.biSizeImage   = 0;

	ret=CreateDIBitmap(hdc,&bmi.bmiHeader,CBM_INIT,(void*)pBmp,&bmi,DIB_RGB_COLORS);
	
	delete [] pBmp;
	
	::ReleaseDC(NULL,hdc);

	return ret;
}

#ifdef _MFC
void CGraphicsFileData::LoadTGAFile(const char* filename)
{
	CFile file(filename,CFile::modeRead);
	CArchive ar(&file,CArchive::load);
	LoadTGAFile(ar);
	ar.Close();
	file.Close();
}
void CGraphicsFileData::LoadTGAFile(CArchive& ar)
{
	int len=ar.GetFile()->GetLength();
	BYTE* pBuf=new BYTE[len];
	ar.Read(pBuf,len);
	LoadTGAFile(pBuf);
	delete [] pBuf;
}

void CGraphicsFileData::SaveAsTex(LPCTSTR lpszFileName, int type)
{
	CFile file(lpszFileName,CFile::modeCreate|CFile::modeWrite);
	CArchive ar(&file,CArchive::store);
	SaveAsTex(ar,type);
	ar.Close();
	file.Close();
}


void CGraphicsFileData::SaveAsTex(CArchive& ar, int type)
{
	TEXFILEHEADER th;
	th.lSize=sizeof(TEXFILEHEADER);
	th.lHeader=GET_HEADER(' ','T','E','X');
	th.lType=type;
	th.lColorKey=0;
	th.lWidth=this->m_FileHeader.Width;
	th.lHeight=this->m_FileHeader.Height;
	th.lBytesPreLine=th.lWidth*2;
	int nTailNum=0;
	if(th.lBytesPreLine % 8)
	{
		nTailNum=8-th.lBytesPreLine%8;
		th.lBytesPreLine+=nTailNum;
	}
	ar.Write(&th,sizeof(th));
	
	const unsigned int nR=4;
	const unsigned int nR1=3;
	DWORD * p=(DWORD*)this->m_pFileData;
	DWORD * p1;
	for(int i=0;i<this->m_FileHeader.Height;i++)
	{
		p1=p;
		for(int j=0;j<this->m_FileHeader.Width;j++)
		{
			WORD wColor;
			if(type==1)
			{
				//A4R4G4B4
				wColor=(((*p1)&0xf0000000)>>16) | (((*p1)&0xf00000)>>12) | (((*p1)&0xf000)>>8) | (((*p1)&0xf0)>>4);
			}
			else if(type==2)
			{
				//R5G6B5
				wColor=(((*p1)&0xf80000)>>8) | (((*p1)&0xFC00)>>5) | (((*p1)&0xf8)>>3);
			}
			else if(type==3)
			{
				//R5G5B5
				wColor=(0x8000) | (((*p1)&0xf80000)>>9) | (((*p1)&0xF800)>>6) | (((*p1)&0xf8)>>3);
			}
			ar<<wColor;
			p1++;
		}
		for(j=0;j<nTailNum;j++)
			ar<<(BYTE)0;
		p+=this->m_FileHeader.Width;
	}
}
#endif

void CGraphicsFileData::LoadTGAFile(BYTE* pBuf)
{
	if( NULL == pBuf )
	{
		return ;
	}

	int i;

	TGAFILEHEADER * ptga=(TGAFILEHEADER*)pBuf;
	pBuf+=sizeof(TGAFILEHEADER);
	memcpy(&m_FileHeader,ptga,sizeof(m_FileHeader));

	// Only TRUE color, non-mapped images are supported
	if( ( 0 != ptga->ColormapType && ptga->ImageType!=1) ||
		( ptga->ImageType != 10 && ptga->ImageType != 2 && ptga->ImageType!=1) )
	{
		return ;
	}
	


	m_BytesPerPixel=ptga->PixelDepth/8;

	// Skip the ID field. The first byte of the header is the length of this field
	if( ptga->IDLength )
	{
		pBuf+=ptga->IDLength;
	}

	_RGB aPal[256];
	int pal_count=*(WORD*)&ptga->ColormapSpecification[2];

	if(ptga->ImageType==1)
	{
		for(i=0; i< pal_count; i++)
		{
			aPal[i].b=pBuf[i*3 + 0];
			aPal[i].g=pBuf[i*3 + 1];
			aPal[i].r=pBuf[i*3 + 2];
			
		}
	}

	pBuf += pal_count *3;

	DWORD m_dwBPP = ptga->PixelDepth;

	delete m_pFileData;

	m_pFileData = (unsigned char *) new unsigned long[m_FileHeader.Width * m_FileHeader.Height];
	assert(m_pFileData);

	BYTE* p1=m_pFileData;
	BYTE* p2;


	if( 0 == ( ptga->ImageDescriptor & 0x0020 ) )
	{
		p1+=m_FileHeader.Height * m_FileHeader.Width*4;

		for( DWORD y=0; y<m_FileHeader.Height; y++ )
		{
			p1-=m_FileHeader.Width*4;
			DWORD dwOffset = y*m_FileHeader.Width;

			if( 0 == ( ptga->ImageDescriptor & 0x0020 ) )
				dwOffset = (m_FileHeader.Height-y-1)*m_FileHeader.Width;

			p2=p1;
			for( DWORD x=0; x<m_FileHeader.Width; x )
			{
				if( ptga->ImageType == 10 )
				{
					BYTE PacketInfo = *pBuf;
					++pBuf;

					WORD PacketType = 0x80 & PacketInfo;
					WORD PixelCount = ( 0x007f & PacketInfo ) + 1;

					if( PacketType )
					{
						DWORD dwTemp;
						if( m_dwBPP == 32 )
						{
							dwTemp=*(DWORD*)pBuf;
							pBuf+=4;
						}
						else if( m_dwBPP == 8)
						{
							dwTemp=aPal[pBuf[0]] | 0xff000000;
							pBuf+=1;
						}
						else 
						{
							dwTemp = 0xff000000 | (pBuf[2] << 16) | (pBuf[1] << 8) | (pBuf[0]);
							pBuf+=3;
						}

						while( PixelCount-- )
						{
							*(DWORD*)p2=dwTemp;
							p2+=4;
							x++;
						}
					}
					else
					{
						while( PixelCount-- )
						{
							if( m_dwBPP == 32 )
							{
								*(DWORD*)p2=*(DWORD*)pBuf;
								pBuf+=4;
							}
							else if(m_dwBPP == 8)
							{
								*(DWORD*)p2=aPal[pBuf[0]] | 0xff000000;
								pBuf++;
							}
							else
							{
								*(DWORD*)p2 = 0xff000000 | (pBuf[2] << 16) | (pBuf[1] << 8) | (pBuf[0]);
								pBuf+=3;
							}
							p2+=4;

							x++;

						}
					}
				}
				else
				{

					if( m_dwBPP == 32 )
					{
						*(DWORD*)p2=*(DWORD*)pBuf;
						pBuf+=4;
					}
					else if(m_dwBPP == 8)
					{
						*(DWORD*)p2=aPal[pBuf[0]] | 0xff000000;
						pBuf++;
					}
					else
					{
						*(DWORD*)p2 = 0xff000000 | (pBuf[2] << 16) | (pBuf[1] << 8) | (pBuf[0]);
						pBuf+=3;
					}
					p2+=4;
					x++;
				}
			}
		}
	}
	else
	{
		for( DWORD y=0; y<m_FileHeader.Height; y++ )
		{
			p2=p1;
			for( DWORD x=0; x<m_FileHeader.Width; x )
			{
				if( ptga->ImageType == 10 )
				{
					BYTE PacketInfo = *pBuf;
					++pBuf;

					WORD PacketType = 0x80 & PacketInfo;
					WORD PixelCount = ( 0x007f & PacketInfo ) + 1;

					if( PacketType )
					{
						DWORD dwTemp;
						if( m_dwBPP == 32 )
						{
							dwTemp=*(DWORD*)pBuf ;
							pBuf+=4;
						}
						else if( m_dwBPP == 8)
						{
							dwTemp=aPal[pBuf[0]] | 0xff000000;
							pBuf+=1;
						}
						else
						{
							dwTemp = 0xff000000 | (pBuf[2] << 16) | (pBuf[1] << 8) | (pBuf[0]);
							pBuf+=3;
						}

						while( PixelCount-- )
						{
							*(DWORD*)p2=dwTemp;
							p2+=4;
							x++;
						}
					}
					else
					{
						while( PixelCount-- )
						{
							if( m_dwBPP == 32 )
							{
								*(DWORD*)p2=*(DWORD*)pBuf ;
								pBuf+=4;
							}
							else if(m_dwBPP == 8)
							{
								*(DWORD*)p2=aPal[pBuf[0]] | 0xff000000;
								pBuf++;
							}
							else
							{
								*(DWORD*)p2 = 0xff000000 | (pBuf[2] << 16) | (pBuf[1] << 8) | (pBuf[0]);
								pBuf+=3;
							}
							p2+=4;
							x++;

						}
					}
				}
				else
				{
					if( m_dwBPP == 32 )
					{
						*(DWORD*)p2=*(DWORD*)pBuf ;
						pBuf+=4;
					}
					else if(m_dwBPP == 8)
					{
						*(DWORD*)p2=aPal[pBuf[0]] | 0xff000000;
						pBuf++;
					}
					else
					{
						*(DWORD*)p2 = 0xff000000 | (pBuf[2] << 16) | (pBuf[1] << 8) | (pBuf[0]);
						pBuf+=3;
					}
					p2+=4;
					x++;

				}
			}
			p1+=m_FileHeader.Width*4;
		}
	}
}
