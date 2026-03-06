#include "stdafx.h"
#include "blk.h"
#include <stdio.h>

#ifndef TRACE
#define TRACE
#endif

//#include "makegtfdlg.h"
/********************************************************************
	created:	2003/04/28
	created:	28:4:2003   18:02
	filename: 	c:\xuzhao\my_engine\tools\tgaview\blk.cpp
	file path:	c:\xuzhao\my_engine\tools\tgaview
	file base:	blk
	file ext:	cpp
	author:		xuzhao
	
	purpose:	
*********************************************************************/


bool g_bRec;

CBlk::CBlk(void)
{
	m_pFileData=NULL;
	m_iWidth=0;
	m_iHeight=0;
}

CBlk::~CBlk(void)
{
	delete m_pFileData;
}

int CBlk::UseableSize( int size )
{
	int test = 0;
	if ( size <=16 )
		test	=	16;
	else
		if ( size <= 32 )
			test	=	32;
		else
			if ( size <= 64 )
				test	=	64;
			else
				if ( size <= 128 )
					test	=	128;
				else
					if ( size <= 256 )
						test	=	256;
					else
						test	=	256;

	return test;	
}

BOOL CBlk::Compress_BLK_RLE(const char* filename, DWORD* pARGB, int width, int height , BOOL bAlpha)
{
	_RGB pal[256];
	int pal_count;
	_RGB color;

	int i,j;
	int index;
	int ix,iy;

	if(!CompressPal(pARGB,width,height,pal,pal_count))
		return FALSE;

	RECT UseRect=FoundRect(pARGB,width,height);
	
	if(UseRect.top >= UseRect.bottom)
	{
		memset(&UseRect,0,sizeof(UseRect));
		//((CmakegtfDlg*)AfxGetMainWnd())->OutMsg("\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>         <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n");
	}
	else if(UseRect.left >= UseRect.right)
	{
		memset(&UseRect,0,sizeof(UseRect));
		//((CmakegtfDlg*)AfxGetMainWnd())->OutMsg("\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>         <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n");
	}

	int w,h;
	
	w = UseRect.right-UseRect.left;
	h = UseRect.bottom-UseRect.top;

	int iTotalWidthBlock,iTotalHeightBlock;
	int iTailBlockWidth,iTailBlockHeight;

	iTotalWidthBlock	=	( w / MAX_TEXTURE_WIDTH );

	iTotalHeightBlock	=	( h / MAX_TEXTURE_WIDTH );

	iTailBlockWidth=MAX_TEXTURE_WIDTH;
	iTailBlockHeight=MAX_TEXTURE_WIDTH;

	int w2,h2;

	if( w % MAX_TEXTURE_WIDTH )
	{
		iTotalWidthBlock++;
		iTailBlockWidth=UseableSize( w % MAX_TEXTURE_WIDTH);
		w2=(iTotalWidthBlock-1) * MAX_TEXTURE_WIDTH + iTailBlockWidth;
	}
	else
	{
		w2=iTotalWidthBlock * MAX_TEXTURE_WIDTH;
	}

	if( h % MAX_TEXTURE_WIDTH )
	{
		iTotalHeightBlock++;
		iTailBlockHeight=UseableSize( h % MAX_TEXTURE_WIDTH);
		h2=(iTotalHeightBlock-1) * MAX_TEXTURE_WIDTH + iTailBlockHeight;
	}
	else
	{
		h2=iTotalHeightBlock * MAX_TEXTURE_WIDTH;
	}

	BYTE * pData = new BYTE[w2 * h2];
	BYTE * pAlpha{};
	memset(pData,0,w2*h2);

	if(bAlpha)
	{
		pAlpha=new BYTE[w2 * h2];
		memset(pAlpha,0,w2*h2);
		for(i=0; i< h ; i++)
		{
			for(j=0; j<w ; j++)
			{
				pAlpha[i*w2+j]=((pARGB[(i+UseRect.top)*width + j + UseRect.left]&0xff000000) >> 24);
			}
		}
	}

	for(i=0; i< h ; i++)
	{
		for(j=0; j<w ; j++)
		{
			index=i*w2+j;
			if(bAlpha  && pAlpha[index]==0)
			{
				pData[index]=0;
			}
			else
			{
				//color.r=(pARGB[(i+UseRect.top)*width + j + UseRect.left]&0xff0000) >> 16;
				//color.g=(pARGB[(i+UseRect.top)*width + j + UseRect.left]&0xff00) >> 8;
				//color.b=(pARGB[(i+UseRect.top)*width + j + UseRect.left]&0xff) ;
				color=pARGB[(i+UseRect.top)*width + j + UseRect.left];

				pData[index]=GetColorIndex(pal,pal_count,color);
			}
		}
	}
	
	
	
	int _width;
	int _height;

	FILE* fp=fopen(filename,"wb");

	if(fp==NULL) 
	{
		return FALSE;
	}

	BYTE* pRleData= new BYTE[ MAX_TEXTURE_WIDTH * MAX_TEXTURE_WIDTH * 100];
	int nRleDataLen=0;
	
	BYTE* pRleAlpha=new BYTE[ MAX_TEXTURE_WIDTH * MAX_TEXTURE_WIDTH * 100];
	int nRleAlphaLen=0;

	int iy3=0;
	int ix3=0;


	IMG_BLOCK blk;
	memset(&blk,0,sizeof(blk));
	blk.m_wPalCount=pal_count;
	blk.m_rClip=UseRect;
	blk.m_wHeight=height;
	blk.m_wWidth=width;
	blk.m_wTotalWidthBlock=iTotalWidthBlock;
	blk.m_wTotalHeightBlock=iTotalHeightBlock;
	blk.m_wTailBlockWidth=iTailBlockWidth;
	blk.m_wTailBlockHeight=iTailBlockHeight;
	blk.m_byPixelDepth=8;
	blk.m_byUseRle=1;
	blk.m_byUseAlpha=bAlpha;

	fwrite(&blk,sizeof(blk),1,fp);
	fwrite(pal,sizeof(_RGB)*pal_count,1,fp);

	for( iy = 0; iy < iTotalHeightBlock; iy ++ )
	{
		if ( iy == iTotalHeightBlock - 1 )
			_height	=	iTailBlockHeight;
		else
			_height	=	MAX_TEXTURE_WIDTH;
		ix3=0;
		for( ix = 0; ix < iTotalWidthBlock;  ix ++ )
		{

			int ix2, iy2;

			if ( ix == iTotalWidthBlock - 1 ) 
				_width	=	iTailBlockWidth;
			else
				_width	=	MAX_TEXTURE_WIDTH;

			nRleDataLen=0;
			nRleAlphaLen=0;
			for(iy2=0;iy2<_height;iy2++)
			{
				int ilen;
				g_bRec=false;
				ProcessLine(&pData [(iy2+iy3 ) * w2 +  ix3],_width,&pRleData[nRleDataLen],ilen);
				nRleDataLen+=ilen;	
				if(bAlpha)
				{
					ProcessLine(&pAlpha[(iy2+iy3 ) * w2 +  ix3],_width,&pRleAlpha[nRleAlphaLen],ilen);
					nRleAlphaLen+=ilen;
				}
			}
			
			//nTmp=nRleDataLen+nRleAlphaLen;
			
			//fwrite(&nTmp,4,1,fp);
			fwrite(&nRleDataLen,4,1,fp);
			fwrite(pRleData,nRleDataLen,1,fp);
			if(bAlpha)
			{
				fwrite(&nRleAlphaLen,4,1,fp);
				fwrite(pRleAlpha,nRleAlphaLen,1,fp);
			}

			ix3+=_width;
		}
		iy3+=_height;
	}

	fclose(fp);

	delete [] pRleData;
	delete [] pRleAlpha;

	delete [] pData;
	delete [] pAlpha;
	return 0;
}

BOOL CBlk::Compress_BLK(const char* filename, DWORD* pARGB, int width, int height)
{
	_RGB pal[256];
	int pal_count;
	_RGB color;

	int i,j;
	int index;
	int ix,iy;
	int nTmp;

	if(!CompressPal(pARGB,width,height,pal,pal_count)) return FALSE;

	RECT UseRect=FoundRect(pARGB,width,height);
	
	if(UseRect.top >= UseRect.bottom)
	{
		memset(&UseRect,0,sizeof(UseRect));
		//((CmakegtfDlg*)AfxGetMainWnd())->OutMsg("\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>         <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n");
	}
	else if(UseRect.left >= UseRect.right)
	{
		memset(&UseRect,0,sizeof(UseRect));
		//((CmakegtfDlg*)AfxGetMainWnd())->OutMsg("\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>         <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n");
	}

	int w,h;

	w = UseRect.right-UseRect.left;
	h = UseRect.bottom-UseRect.top;

	int iTotalWidthBlock,iTotalHeightBlock;
	int iTailBlockWidth,iTailBlockHeight;

	iTotalWidthBlock	=	( w / MAX_TEXTURE_WIDTH );

	iTotalHeightBlock	=	( h / MAX_TEXTURE_WIDTH );

	iTailBlockWidth=MAX_TEXTURE_WIDTH;
	iTailBlockHeight=MAX_TEXTURE_WIDTH;

	int w2,h2;

	if( w % MAX_TEXTURE_WIDTH )
	{
		iTotalWidthBlock++;
		iTailBlockWidth=UseableSize( w % MAX_TEXTURE_WIDTH);
		w2=(iTotalWidthBlock-1) * MAX_TEXTURE_WIDTH + iTailBlockWidth;
	}
	else
	{
		w2=iTotalWidthBlock * MAX_TEXTURE_WIDTH;
	}

	if( h % MAX_TEXTURE_WIDTH )
	{
		iTotalHeightBlock++;
		iTailBlockHeight=UseableSize( h % MAX_TEXTURE_WIDTH);
		h2=(iTotalHeightBlock-1) * MAX_TEXTURE_WIDTH + iTailBlockHeight;
	}
	else
	{
		h2=iTotalHeightBlock * MAX_TEXTURE_WIDTH;
	}

	BYTE * pData = new BYTE[w2 * h2];
	BYTE * pAlpha= new BYTE[w2 * h2];

	memset(pData,0,w2*h2);
	memset(pAlpha,0,w2*h2);

	for(i=0; i< h ; i++)
	{
		for(j=0; j<w ; j++)
		{
			pAlpha[i*w2+j]=((pARGB[(i+UseRect.top)*width + j + UseRect.left]&0xff000000) >> 24);
		}
	}

	for(i=0; i< h ; i++)
	{
		for(j=0; j<w ; j++)
		{
			index=i*w2+j;
			if(pAlpha[index]==0)
			{
				pData[index]=0;
			}
			else
			{
				//color.r=(pARGB[(i+UseRect.top)*width + j + UseRect.left]&0xff0000) >> 16;
				//color.g=(pARGB[(i+UseRect.top)*width + j + UseRect.left]&0xff00) >> 8;
				//color.b=(pARGB[(i+UseRect.top)*width + j + UseRect.left]&0xff) ;
				color=pARGB[(i+UseRect.top)*width + j + UseRect.left];

				pData[index]=GetColorIndex(pal,pal_count,color);
			}
		}
	}



	int _width;
	int _height;

	FILE* fp=fopen(filename,"wb");

	if(fp==NULL) 
	{
		return FALSE;
	}

	BYTE* pNewData= new BYTE[ MAX_TEXTURE_WIDTH * MAX_TEXTURE_WIDTH *2];
	int nNewDataLen=0;

	int iy3=0;
	int ix3=0;


	IMG_BLOCK blk;
	memset(&blk,0,sizeof(blk));
	blk.m_wPalCount=pal_count;
	blk.m_rClip=UseRect;
	blk.m_wHeight=height;
	blk.m_wWidth=width;
	blk.m_wTotalWidthBlock=iTotalWidthBlock;
	blk.m_wTotalHeightBlock=iTotalHeightBlock;
	blk.m_wTailBlockWidth=iTailBlockWidth;
	blk.m_wTailBlockHeight=iTailBlockHeight;
	blk.m_byPixelDepth=8;
	blk.m_byUseRle=0;
	blk.m_byUseAlpha=1;

	fwrite(&blk,sizeof(blk),1,fp);
	fwrite(pal,sizeof(_RGB)*pal_count,1,fp);

	for( iy = 0; iy < iTotalHeightBlock; iy ++ )
	{
		if ( iy == iTotalHeightBlock - 1 )
			_height	=	iTailBlockHeight;
		else
			_height	=	MAX_TEXTURE_WIDTH;
		ix3=0;
		for( ix = 0; ix < iTotalWidthBlock;  ix ++ )
		{

			int ix2, iy2;

			if ( ix == iTotalWidthBlock - 1 ) 
				_width	=	iTailBlockWidth;
			else
				_width	=	MAX_TEXTURE_WIDTH;

			nNewDataLen=0;
			for(iy2=0;iy2<_height;iy2++)
			{
				int ilen;
				g_bRec=false;

				for(ix2=0; ix2<_width;ix2++)
				{
					pNewData[nNewDataLen]=pData[(iy2+iy3 ) * w2 +  ix3 + ix2];
					pNewData[nNewDataLen+1]=pAlpha[(iy2+iy3 ) * w2 +  ix3 + ix2];

					nNewDataLen+=2;
				}
				//ProcessLine(&pData [(iy2+iy3 ) * w2 +  ix3],_width,&pRleData[nRleDataLen],ilen);
				//nRleDataLen+=ilen;	

				//ProcessLine(&pAlpha[(iy2+iy3 ) * w2 +  ix3],_width,&pRleAlpha[nRleAlphaLen],ilen);

				//nRleAlphaLen+=ilen;
			}

			//nTmp=nRleDataLen+nRleAlphaLen;

			fwrite(pNewData,nNewDataLen,1,fp);

			ix3+=_width;
		}
		iy3+=_height;
	}

	fclose(fp);

	delete [] pNewData;

	delete [] pData;
	delete [] pAlpha;
	return 0;
}

BOOL CBlk::CompressPal(const DWORD* pARGB, int width, int height, _RGB* pPal, int & nCount)
{
	//

	_RGB* pPalTmp=new _RGB[width * height];
	int	* pColorCount= new int [width * height];
	
	int	nPalTmpCount=0;
	_RGB crTmp;
	int	nTmp;
	int i,j;
	i=sizeof(crTmp);
	for(i=0; i<height; i++)
	{
		for(j=0; j<width; j++)
		{
			if((pARGB[i*width + j] & 0xff000000) == 0 ) continue;
			
			crTmp=pARGB[i*width + j];
			//crTmp.r=(pARGB[i*width + j] & 0xff0000) >> 16;
			//crTmp.g=(pARGB[i*width + j] & 0xff00) >> 8;
			//crTmp.b=(pARGB[i*width + j] & 0xff)  ;
			int index;
			if((index = SearchPal(pPalTmp,nPalTmpCount,crTmp)) == -1)
			{
				//
				pPalTmp[nPalTmpCount]=crTmp;
				pColorCount[nPalTmpCount]=1;
				nPalTmpCount++;
			}
			else
			{
				//
				++pColorCount[index];
			}
		}
	}

	if(nPalTmpCount > 256)
	{
		// 256 , 256 
		for(i=1; i<nPalTmpCount; i++)
		{
			crTmp=pPalTmp[i];
			nTmp=pColorCount[i];
			j=i-1;
			while( j>=0 && nTmp > pColorCount[j] )
			{
				pColorCount[j+1]=pColorCount[j];
				pPalTmp[j+1]=pPalTmp[j];
				--j;
			}
			pColorCount[j+1]=nTmp;
			pPalTmp[j+1]=crTmp;
		}
		memcpy(pPal,pPalTmp,256*sizeof(_RGB));
		nCount=256;
	}
	else
	{
		memcpy(pPal,pPalTmp,nPalTmpCount*sizeof(_RGB));
		nCount=nPalTmpCount;
	}

	//for(i=0;i<nPalTmpCount;i++)
	//{
	//	TRACE("%d\n",pColorCount[i]);
	//}
	delete [] pPalTmp;
	delete [] pColorCount;

	return TRUE;
}

int CBlk::SearchPal(_RGB* pPal, int nCount,_RGB color)
{
	for(int i=0; i<nCount; i++)
	{
		if(pPal[i]==color)
		{
			return i;
		}
	}
	/*for(int i=0; i<nCount; i++)
	{
		if(pPal[i].Difference(color)<5)
		{
			return i;
		}
	}*/
	return -1;
}

int CBlk::GetColorIndex(_RGB* pPal, int nCount, _RGB color)
{
	//
	unsigned int min_diff=100000;
	int min_index=-1;
	unsigned int diff;
	for(int i=0;i<nCount;i++)
	{
		if(pPal[i]==color) return i;
		diff=pPal[i].Difference(color);

		if(diff < min_diff)
		{
			min_diff=diff;
			min_index=i;
		}
	}
	if(min_index==-1) return 0;
	return min_index;
}

RECT CBlk::FoundRect(DWORD* pARGB, int width, int height)
{
	RECT rect;

	int ix,iy;
	bool bBreak;

	bBreak=false;
	for(iy=0; iy<height; iy++)
	{
		for(ix=0; ix<width; ix++)
		{
			if((pARGB[iy*width + ix] & 0xff000000) != 0)
			{
				bBreak=true;
				break;					
			}
		}
		if(bBreak) break;
	}
	
	rect.top=iy;

	bBreak=false;
	for(iy=height-1; iy>=0; iy--)
	{
		for(ix=0; ix<width; ix++)
		{
			if((pARGB[iy*width + ix] & 0xff000000) != 0)
			{
				bBreak=true;
				break;					
			}
		}
		if(bBreak) break;
	}

	rect.bottom=iy+1;

	bBreak=false;
	for(ix=0; ix<width; ix++)
	{
		for(iy=0; iy<height; iy++)
		{
			if((pARGB[iy*width + ix] & 0xff000000) != 0)
			{
				bBreak=true;
				break;					
			}
		}
		if(bBreak) break;
	}

	rect.left=ix;

	bBreak=false;
	for(ix=width-1; ix>=0; ix--)
	{
		for(iy=0; iy<height; iy++)
		{
			if((pARGB[iy*width + ix] & 0xff000000) != 0)
			{
				bBreak=true;
				break;					
			}
		}
		if(bBreak) break;
	}

	rect.right=ix+1;

	return rect;
}

void CBlk::ProcessLine(BYTE* pData,int width, BYTE* pRetData, int& nRetLen)
{
	int i;
	BYTE flag;
	nRetLen=0;
	int i1,i2;


	int k;

	if(g_bRec)
	{
		for(i=0;i<width;i++)
		{
			TRACE("%d ",(int)pData[i]);
			
		}
		TRACE("\n");
	}

	i1=0;
	i2=-1;
	for(i=0; i<width; i++)
	{
		int rl=1;
		while(i+1 < width && pData[i]==pData[i+1] && rl < 129)
		{
			rl++;
			i++;
		}
		
		if(rl>1)
		{
			if(i1 <= i2)
			{
				flag=i2 - i1 + 1;
				
				if(g_bRec)
				{
					TRACE("~");
					TRACE("%d ",flag);
					for(k=i1;k<=i2;k++)
					{
						TRACE("%d ",(int)pData[k]);
					}
				}

				pRetData[nRetLen++]=flag;
				memcpy(&pRetData[nRetLen],&pData[i1],flag);
				nRetLen+=(flag);
			}
			flag=0x80 | (rl-2);
			
			if(g_bRec)
			{
				TRACE("!");
				TRACE("%d ",rl);
				TRACE("%d ",(int)pData[i]);
			}

			pRetData[nRetLen++]=flag;
			pRetData[nRetLen++]=pData[i];
			i1=i+1;
		}
		else
		{
			i2=i;
			if(i2-i1 == 126)
			{
				flag=i2-i1 +1;

				if(g_bRec)
				{
					TRACE("%d ",flag);
					for(k=i1;k<=i2;k++)
					{
						TRACE("%d ",(int)pData[k]);
					}
				}

				pRetData[nRetLen++]=flag;
				memcpy(&pRetData[nRetLen],&pData[i1],flag);
				nRetLen+=(flag);
				i1=i+1;
			}
		}
	}
	if(i2>=i1)
	{
		flag=i2-i1+1;

		if(g_bRec)
		{
			TRACE("%d ",flag);
			for(k=i1;k<=i2;k++)
			{
				TRACE("%d ",(int)pData[k]);
			}
		}

		pRetData[nRetLen++]=flag;
		memcpy(&pRetData[nRetLen],&pData[i1],flag);
		nRetLen+=(flag);
	}
	
	if(g_bRec)
	{
		TRACE("\n");
		TRACE("len=%d",nRetLen);
		TRACE("\n");

	}
}

void CBlk::LoadBlk(const char* filename)
{
	FILE* fp=fopen(filename,"rb");
	if(!fp) return ;

	long nCur=ftell(fp);
	fseek(fp,0,SEEK_END);
	long nEnd=ftell(fp);
	fseek(fp,nCur,SEEK_SET);
	long nLen=nEnd-nCur;

	BYTE* pBuf=new BYTE[nLen];

	fread(pBuf,nLen,1,fp);

	fclose(fp);
	LoadBlk(pBuf);
	delete [] pBuf;

	

}

void CBlk::LoadBlk(BYTE* pBuf)
{
	IMG_BLOCK * pbh;

	pbh=(IMG_BLOCK*)pBuf;
	pBuf+=sizeof(IMG_BLOCK);

	m_iWidth=pbh->m_wWidth;
	m_iHeight=pbh->m_wHeight;

	
	int pal_count=pbh->m_wPalCount;
	
	_RGB* pal;//=new _RGB[pal_count];
	
	//WORD* p565_Pal=(WORD*)pBuf;

	//pBuf+=sizeof(WORD)*pal_count;

	//_565ToPal(p565_Pal,pal_count,pal);

	pal=(_RGB*)pBuf;
	pBuf+=sizeof(_RGB)*pal_count;

	delete  m_pstFileData;

	m_pstFileData =new stFileData[pbh->m_wWidth * pbh->m_wHeight];
	memset(m_pstFileData,0,pbh->m_wWidth * pbh->m_wHeight * sizeof(stFileData));

	int i,j,i2,j2,i3,j3,i4,j4;

	int iTailWidth,iTailHeight;
	int w,h,w2,h2;
	w=pbh->m_rClip.right-pbh->m_rClip.left;
	h=pbh->m_rClip.bottom-pbh->m_rClip.top;

	if(w % MAX_TEXTURE_WIDTH)
	{
		iTailWidth=UseableSize(w % MAX_TEXTURE_WIDTH);
	}
	else
	{
		iTailWidth=MAX_TEXTURE_WIDTH;
	}

	if(h % MAX_TEXTURE_WIDTH)
	{
		iTailHeight=UseableSize(h % MAX_TEXTURE_WIDTH);
	}
	else
	{
		iTailHeight=MAX_TEXTURE_WIDTH;
	}

	w2=(pbh->m_wTotalWidthBlock-1) * MAX_TEXTURE_WIDTH + iTailWidth;
	h2=(pbh->m_wTotalHeightBlock-1) * MAX_TEXTURE_WIDTH + iTailHeight;

	i2=pbh->m_rClip.top;

	//pBuf+=4;

	for(i=0;i<pbh->m_wTotalHeightBlock;i++)
	{
		j2=pbh->m_rClip.left;
		int _height,_width;
		if(i==pbh->m_wTotalHeightBlock-1)
			_height=iTailHeight;
		else
			_height=MAX_TEXTURE_WIDTH;
		for(j=0;j<pbh->m_wTotalWidthBlock;j++)
		{

			if(i==pbh->m_wTotalWidthBlock-1)
				_width=iTailWidth;
			else
				_width=MAX_TEXTURE_WIDTH;
			i4=i2;
			
			DWORD dwDataLen=*(DWORD*)pBuf;
			pBuf+=4;
			BYTE* pData2=pBuf;
			pBuf+=dwDataLen;
			DWORD dwAlphaLen=*(DWORD*)pBuf;
			BYTE* pAlpha2=pBuf;
			pBuf+=dwAlphaLen;
			for(i3=0;i<_height;i++)
			{
				int ilen;
				
				DecodeLine(pData2,_width,&m_pstFileData[i4 * pbh->m_wWidth + j2],ilen,pal);
				pData2+=ilen;
				DecodeAlphaLine(pAlpha2,_width,&m_pstFileData[i4 * pbh->m_wWidth + j2],ilen);
				pAlpha2+=ilen;
				

				i4++;
			}
			j2+=_width;
		}
		i2+=_height;
	}

	
}

HBITMAP CBlk::GetBitmap(COLORREF ct)
{
	HBITMAP ret=NULL;
	if(m_pFileData==NULL)
		return ret;

	HDC hdc=GetDC(NULL);

	int cx=m_iWidth;
	int cy=m_iHeight;

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

void CBlk::DecodeLine(BYTE* pData, int width, _stFileData* pRetData,int & nRetLen,_RGB* pal)
{
	int j=0;
	nRetLen=0;
	for(int i=0;i<width;i)
	{
		int pixelCount=0;
		if((pData[nRetLen]) & 0x80)
		{
			pixelCount=((pData[nRetLen]) & 0x7f) +2;
			nRetLen++;
			_RGB v=pal[pData[nRetLen]];
			nRetLen++;
			while(pixelCount--)
			{
				pRetData[j].R=v.r;
				pRetData[j].G=v.g;
				pRetData[j].B=v.b;
				j++;
				i++;
			}
		}
		else
		{
			pixelCount=(pData[nRetLen]);
			nRetLen++;

			while(pixelCount--)
			{
				pRetData[j].R=pal[pData[nRetLen]].r;
				pRetData[j].G=pal[pData[nRetLen]].g;
				pRetData[j].B=pal[pData[nRetLen]].b;
				j++;
				nRetLen++;
				i++;
			}
		}
	}
}

int CBlk::DecodeLine(BYTE* pData, int width, BYTE* pRetData,int & nRetLen)
{
	int j=0;
	nRetLen=0;
	TRACE("c=== ");
	for(int i=0;i<width;i)
	{
		int pixelCount=0;
		if((pData[nRetLen]) & 0x80)
		{
			pixelCount=((pData[nRetLen]) & 0x7f) +2;
			nRetLen++;
			BYTE v=pData[nRetLen];
			nRetLen++;
			while(pixelCount--)
			{
				pRetData[j++]=v;
				i++;
			}
		}
		else
		{
			pixelCount=(pData[nRetLen]);
			nRetLen++;

			while(pixelCount--)
			{
				pRetData[j++]=pData[nRetLen];
				nRetLen++;
				i++;
			}
		}
	}

	return j;
}

void CBlk::DecodeAlphaLine(BYTE* pData, int width, _stFileData* pRetData,int & nRetLen)
{

	//for(int i=0;i<width;i++)
	//{
	//	pRetData[i].A=pData[i];
	//}
	//nRetLen=width;

	int j=0;
	nRetLen=0;
	for(int i=0;i<width;i)
	{
		int pixelCount=0;
		if((pData[nRetLen]) & 0x80)
		{
			pixelCount=((pData[nRetLen]) & 0x7f) +2;
			nRetLen++;
			
			BYTE a=pData[nRetLen];

			nRetLen++;
			while(pixelCount--)
			{
				pRetData[j].A=a;
				j++;
				i++;
			}
		}
		else
		{
			pixelCount=(pData[nRetLen]);
			nRetLen++;

			while(pixelCount--)
			{
				pRetData[j].A=pData[nRetLen];
				j++;
				nRetLen++;
				i++;
			}
		}
	}
}

void CBlk::PalTo565(_RGB* pal, int pal_count, WORD* pRet)
{

	for(int i=0;i<pal_count;i++)
	{
		pRet[i]=((pal[i].r & 0xf8) <<8) | ((pal[i].g & 0xfc) << 3) | ((pal[i].b & 0xf8)>>3);
	}
}

void CBlk::_565ToPal(WORD* p565, int pal_count, _RGB* pal)
{
	for(int i=0;i<pal_count;i++)
	{
		pal[i]=(DWORD)( ((p565[i]&0xf800) << 8) | ((p565[i]&0x7e0) << 5) | ((p565[i]&0x1f) << 3) );
	}
}
