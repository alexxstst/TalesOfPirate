#pragma once
#include "StdAfx.h"
#include "GlobalInc.h"
#include "assert.h"

#include "MPModelEff.h"

#include "mpfont.h"
#include "MPRender.h"

#include "lwIUtil.h"
#include "lwPredefinition.h"

using namespace std;

ui::UIClip* ui::UIClip::m_pClip = NULL;

extern MINDPOWER_API CMPResManger        ResMgr;

namespace ui
{
	UIClip::UIClip()
	{
		m_byState = 0;
	}

	UIClip::~UIClip()
	{
		SAFE_DELETE( m_pClip );
	}
}

#define  HLSIZE 0
#define  ASSIZE 0

CMPFont::CMPFont() : _hDc(NULL), _hBmp(NULL), _hFont(NULL),_hBmpOld(NULL),_hFontOld(NULL),
_Max(0), _RowNum(0), _pBits(NULL), 
_TextSize(0), _TextureSize(0),
_pDev(NULL), _pVB(NULL), _pTex(NULL)
{
	_iBufWidth = 0;
	_iBufHeight= 0;

	_vecBuf.clear();
	_pTexFast = NULL;
	_vecBufFast.clear();
	_RowNumFast = 0;
	_MaFast   = 0;
	_RowNumFast = 0;
	_TextureSizeFast = 0;

	_iRenderIdx = 5;

	_vecVer = NULL;
	_vecAsh = NULL;

}

CMPFont::~CMPFont()
{
	SelectObject( _hDc, _hBmpOld );
	SelectObject( _hDc, _hFontOld );

	DeleteObject( _hFont );
	DeleteObject( _hBmp );
	DeleteDC( _hDc );

	SAFE_DELETE_ARRAY(_vecVer);
	SAFE_DELETE_ARRAY(_vecAsh);

	ReleaseFont();
}
#ifdef		USE_RENDER
bool CMPFont::CreateFont( MPRender* pd3dDevice, const std::string& szFontName, int nSize, int nLevel ,DWORD dwFlag)
#else
bool CMPFont::CreateFont( LPDIRECT3DDEVICE8 pd3dDevice, char szFontName[], int nSize, int nLevel ,DWORD dwFlag)
#endif
{
	if( nLevel< 1 || nLevel>5 )
		return false;	

	_hDc = CreateCompatibleDC(NULL);
	if(!_hDc)
	{
		MessageBox(NULL,"CreateCompatibleDC","ERROR",0);
		return false;
	}
	::SetMapMode( _hDc, MM_TEXT );

	_bWidthEqual = true;
	SIZE size1, size2;
	::GetTextExtentPoint(_hDc,"a",1,&size1);
	::GetTextExtentPoint(_hDc,"i",1,&size2);
	if( size1.cx != size2.cx )
	{
		_bWidthEqual = false;
	}

	SIZE size;
	::GetTextExtentPoint(_hDc,"",1,&size);

	_TextSize	 = /*size.cy;//*/	nSize;//
	_TextureSize = 32 << nLevel;		

	_RowNum = _TextureSize / _TextSize;	
	_Max = _RowNum * _RowNum;
	_fyu =  (float)_TextureSize / (float)_TextSize - (float)_RowNum;

	_pDev = pd3dDevice;

	DWORD dwBold  = (dwFlag&MPFONT_BOLD)   ? FW_BOLD : FW_NORMAL;
	bool dwItalic = (dwFlag&MPFONT_ITALIC) ? TRUE    : FALSE;
	bool dwUnLine = (dwFlag&MPFONT_UNLINE) ? TRUE    : FALSE;


	LOGFONT LogFont;
	ZeroMemory( &LogFont, sizeof(LogFont) );
	LogFont.lfHeight			= -_TextSize;
	LogFont.lfWidth				= 0;
	LogFont.lfEscapement		= 0;
	LogFont.lfOrientation		= 0;
	LogFont.lfWeight			= dwBold;
	LogFont.lfItalic			= dwItalic;
	LogFont.lfUnderline			= dwUnLine;
	LogFont.lfStrikeOut			= FALSE;
	LogFont.lfCharSet			= DEFAULT_CHARSET;
	LogFont.lfOutPrecision		= OUT_DEFAULT_PRECIS; 
	LogFont.lfClipPrecision		= CLIP_DEFAULT_PRECIS; 
	LogFont.lfQuality			= ANTIALIASED_QUALITY;
	LogFont.lfPitchAndFamily	= VARIABLE_PITCH;
	lstrcpy( LogFont.lfFaceName, szFontName.c_str() );

	_hFont = CreateFontIndirect( &LogFont );
	if ( NULL == _hFont )
	{
		DeleteDC( _hDc );
		MessageBox(NULL,"CreateFontIndirect","ERROR",0);
		return false;
	}

	BITMAPINFO bmi;
	ZeroMemory(&bmi.bmiHeader, sizeof(BITMAPINFOHEADER));
	bmi.bmiHeader.biSize		= sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth		= _TextSize;
	bmi.bmiHeader.biHeight		= -_TextSize/* - 1*/;
	bmi.bmiHeader.biPlanes		= 1;
	bmi.bmiHeader.biBitCount	= 32;
	bmi.bmiHeader.biCompression = BI_RGB;

	_hBmp = CreateDIBSection( _hDc, &bmi, DIB_RGB_COLORS,
		(void **) &_pBits, NULL, 0 );
	if ( NULL == _hBmp || NULL == _pBits )
	{
		DeleteObject( _hFont );
		DeleteDC( _hDc );
		MessageBox(NULL,"CreateDIBSection","ERROR",0);
		return false;
	}

	_hBmpOld  = SelectObject( _hDc, _hBmp );
	_hFontOld = (HFONT)SelectObject( _hDc, _hFont );

	SetTextColor( _hDc, RGB(255,255,255) );
	SetBkColor( _hDc, 0x00000000 );
	SetTextAlign( _hDc, TA_TOP );

	lwIResourceMgr* res_mgr = _pDev->GetInterfaceMgr()->res_mgr;

	// begin
	res_mgr->CreateMesh(&_pVB);
	_pVB->SetStreamType(STREAM_LOCKABLE);

	lwMeshInfo mi;
	mi.fvf = D3DFVF_3DFONT;
	mi.pt_type = D3DPT_TRIANGLEFAN;
	mi.subset_num = 1;
	mi.vertex_num = 4;
	mi.vertex_seq = LW_NEW(lwVector3[mi.vertex_num]);
	mi.blend_seq =  LW_NEW(lwBlendInfo[mi.vertex_num]);
	mi.vercol_seq = LW_NEW(DWORD[mi.vertex_num]);
	mi.texcoord0_seq = LW_NEW(lwVector2[mi.vertex_num]);
	mi.subset_seq = LW_NEW(lwSubsetInfo[mi.subset_num]);

	for (int w = 0; w < 4; w++)
	{
		mi.vertex_seq[w] = lwVector3(0, 0, 0);
		mi.blend_seq[w].weight[0] = (float)w * 2;
		mi.vercol_seq[w] = 0xffffffff;
		mi.texcoord0_seq[w].x = (float)w * 2;
		mi.texcoord0_seq[w].y = (float)w * 2 +1; 
	}
	lwSubsetInfo_Construct(&mi.subset_seq[0], 2, 0, 4, 0);

	if(LW_FAILED(_pVB->LoadSystemMemory(&mi)))
		return 0;

	if(LW_FAILED(_pVB->LoadVideoMemory()))
		return 0;
	lwTexInfo	info;
	info.stage = 0;
	info.type = TEX_TYPE_SIZE;
	info.level = 1;
	info.usage = 0;
	info.format = D3DFMT_A4R4G4B4;
	info.pool = D3DPOOL_MANAGED;
	info.colorkey_type = COLORKEY_TYPE_NONE;
	info.width = _TextureSize;
	info.height = _TextureSize;
	if(FAILED(lwLoadTex(&_pTex,res_mgr,&info)))
	{
		DeleteObject( _hFont );
		DeleteObject( _hBmp );
		DeleteDC( _hDc );
		SAFE_RELEASE(_pVB);

		MessageBox(NULL,"CreateTexture","ERROR",0);

		return false;
	}
	_vecBuf.resize(_Max);

	_TextureSizeFast = 256;
	_RowNumFast = _TextureSizeFast / _TextSize;
	_MaFast = _RowNumFast * _RowNumFast;

	info.stage = 0;
	info.type = TEX_TYPE_SIZE;
	info.level = 1;
	info.usage = 0;
	info.format = D3DFMT_A4R4G4B4;
	info.pool = D3DPOOL_MANAGED;
	info.colorkey_type = COLORKEY_TYPE_NONE;
	info.width = _TextureSizeFast;
	info.height = _TextureSizeFast;
	if(FAILED(lwLoadTex(&_pTexFast,res_mgr,&info)))
	{
		DeleteObject( _hFont );
		DeleteObject( _hBmp );
		DeleteDC( _hDc );
		SAFE_RELEASE(_pVB);
		MessageBox(NULL,"CreateTexture","ERROR",0);

		return false;
	}
	_vecBufFast.resize(_MaFast);
	for(UINT n = 0; n < _MaFast; n++)
		_vecBufFast[n] = 0;

	_lpCurTex = _pTexFast;
	return true;
}

void CMPFont::BindingRes(CMPResManger* pResMagr)
{
	_pCEffectFile = pResMagr->GetEffectFile();

	_pmatBBoard	  = pResMagr->GetBBoardMat();
	_pmatViewProj = pResMagr->GetViewProjMat();
	_pmat2DViewProj = pResMagr->Get2DViewProjMat();
	_iBufWidth		= &pResMagr->GetFontBkWidth();
	_iBufHeight		= &pResMagr->GetFontBkHeight();

	_bUseSoft = pResMagr->m_bUseSoft;
	
	SAFE_DELETE_ARRAY(_vecVer);
	SAFE_DELETE_ARRAY(_vecAsh);
	_vecVer = new FONT_VER[2000];
	_vecAsh = new FONT_VER[2000];
	_vecFontRect.resize(RECTNUM);
	_vecValidID.resize(RECTNUM);
	_vecValidID.setsize(RECTNUM);
	for(INT iw = 0; iw <  RECTNUM; iw++)
	{
		*_vecValidID[iw] = iw;
	}
	FillTextToTex(pResMagr->GetDefaultText());

}
int	  CMPFont::GetHzLength(float fscale)
{
	float w = (float)_TextSize * fscale;
	w += ASSIZE;
	return (int)w;
}
int   CMPFont::GetAscLength(float fscale)
{
	float w = (float)_TextSize * fscale;
	w += HLSIZE;
	return (int)w;
}


SIZE* CMPFont::GetTextSize(const std::string& szText, SIZE* pSize ,float fScale)
{
	if(szText.empty())
		return pSize;

	float sx = 0, sy = 0,
		offset=0, w=0, h=0;
	w = h = ((float)(_TextSize) * fScale) / 2.0f;
	sy = h * 2;

	auto ch = szText.c_str();

	int sx2 = 0;
	while(*ch != '\0')
	{
		if ( *ch=='\n' )
		{
			sy+=h * 2 ;
			if(sx > sx2)
				sx2 = (int)sx;

			sx = 0;
			ch++;
			continue;
		}
		if(*ch == ' ')
		{
			sx+=w + HLSIZE;
			ch++;
			continue;
		}

		if ( *ch & 0x80 )
		{
			ch++;
			offset = w * 2 + ASSIZE;
		}
		else
		{
			SIZE size;
			::GetTextExtentPoint(_hDc,&ch[0],1,&size);
			offset = size.cx + HLSIZE ;
		}
		sx+=offset;
		ch++;
	}

	if(sx > sx2)
		sx2 = (int)sx;

	pSize->cx = (long)sx2;
	pSize->cy = (long)sy;

	return pSize;
}


bool CMPFont::TextToTexture( char c1, char c2, float & tX, float & tY )
{
	WORD w = MAKEWORD(c1, c2);
	vector<WORD>::iterator it = std::find( _vecBuf.begin(), _vecBuf.end(), w );
	if ( it == _vecBuf.end() )
	{
		it = find( _vecBuf.begin(), _vecBuf.end(), 0 );
		if ( it == _vecBuf.end() )
		{
			memset(&_vecBuf.front(),0,sizeof(WORD)*_Max );

			it = _vecBuf.begin();
		}
		int at = (int)(it - _vecBuf.begin());

		tX = (float)(at % _RowNum) * _TextSize;
		tY = (float)(at / _RowNum) * _TextSize;

		(*it) = w;

		RECT rect = {0, 0, _TextSize, _TextSize/* + 1*/};
		char sz[3] = {c1, c2, '\0'};


		FillRect( _hDc, &rect, (HBRUSH)GetStockObject(BLACK_BRUSH) );

		::TextOut( _hDc, 0, 0, sz, c1 & 0x80 ? 2 : 1 );

		D3DLOCKED_RECT d3dlr;

		_pTex->GetTex()->LockRect(0, &d3dlr, NULL, 0);
		BYTE * pDstRow = (BYTE*)( (WORD *)d3dlr.pBits + (int)tY * _TextureSize + (int)tX );

		for (WORD y=0; y < _TextSize; y++)
		{
			WORD * pDst16 = (WORD*)pDstRow;

			for (WORD x=0; x<_TextSize; x++)
			{
				BYTE bAlpha = (BYTE)((_pBits[_TextSize * y + x] & 0xff) >> 4);
				if (bAlpha > 0)
				{
					*pDst16 = (bAlpha << 12) | 0x0fff;
				}
				else
					*pDst16 = 0x0000;

				pDst16++;
			}
			pDstRow += d3dlr.Pitch;
		}
		_pTex->GetTex()->UnlockRect( NULL );
	}
	else
	{
		int at = (int)(it - _vecBuf.begin());
		tX = (float)(at % _RowNum) * _TextSize;
		tY = (float)(at / _RowNum) * _TextSize;
	}
	return true;
}
void CMPFont::DrawTextClipOnce(const std::string& szText, int nLen, LPRECT psrc, LPRECT pclip,D3DXCOLOR color)
{
	int x,y;
	float sx = 0, sy = 0,
		offset=0, w=0, h=0, tx1=0, ty1=0, tx2=0, ty2=0;

	float tx = 0, ty = 0;
	float tu1 = 0,tv1 = 0,tu2 = 0,tv2 = 0;

	int idxHsl = 0, idxAsh = 0;
	x = psrc->left;
	y = psrc->top;

	w = h = (float)(_TextSize);
	h = (float)(_TextSize);
	_pDev->SetVertexShader(NULL);
	_pDev->SetFVF(D3DFVF_FONT);

	bool	bExit = false;
	sx = float(x); sy = float(y);
	char ch[3] = {0,0,0};
	FONT_VER * pVertices = NULL;

	for (int n=0; n<nLen; n++ )
	{
		ch[0] = szText[n];
		if ( ch[0]=='\n' )
		{
			sy = sy + h;
			sx=float(x);
			continue;
		}

		if(ch[0] == ' ')
		{
			sx+= w/2 + HLSIZE;
			continue;
		}

		if ( ch[0] & 0x80 )
		{
			n++;
			ch[1] = szText[n];
			offset = w + ASSIZE;
		}
		else
		{
			ch[1] = '\0';
			if( !_bWidthEqual )
			{
				SIZE size;
				::GetTextExtentPoint(_hDc,&ch[0],1,&size);
				offset = size.cx + HLSIZE ;
			}
			else
			{
				offset = w/2 + HLSIZE ;
			}			
		}

		float a, b;

		if(!(ch[0] & 0x80) &&FindTextFromTex(ch[0], ch[1], tx1, ty1 ,tx2,ty2))
		{
			bExit = true;
		}
		else
		{
			TextToTexture( ch[0], ch[1], a, b );
			//!  0.0-1.0
			tx1 = (float)a / (float)((float)_TextureSize - _fyu/*- (ch[0] & 0x80 ? _fyu/2: _fyu/4)*/);
			ty1 = (float)b / (float)((float)_TextureSize - _fyu/*- (ch[0] & 0x80 ? _fyu/2 : _fyu/4)*/);
			//!  0.0-1.0
			tx2 = tx1 + (float)_TextSize / (float)((float)_TextureSize -_fyu/* - (ch[0] & 0x80 ? _fyu/2 : _fyu/4)*/);
			ty2 = ty1 + (float)_TextSize / (float)((float)_TextureSize -_fyu/* - (ch[0] & 0x80 ? _fyu/2 : _fyu/4)*/);

			bExit = false;
		}

		{

			RECT rc, dest;
			rc.left = (LONG)sx;
			rc.top = (LONG)sy;
			rc.right = (LONG)sx + (LONG)w;
			rc.bottom = (LONG)sy + (LONG)h;
			if(!GetIntersectRect(&dest, &rc,pclip))
			{
				sx+=offset;
				continue;//
			}else if( rc.left >= pclip->left && rc.right <= pclip->right\
				&&rc.top >= pclip->top && rc.bottom <= pclip->bottom)
			{
				if(bExit)
				{
					_vecAsh[idxAsh++].SetValue(D3DXVECTOR4(sx,    sy+h, 0.0f,1),color,D3DXVECTOR2(tx1,ty2));
					_vecAsh[idxAsh++].SetValue(D3DXVECTOR4(sx,	  sy,    0.0f,1),color,D3DXVECTOR2(tx1,ty1));
					_vecAsh[idxAsh++].SetValue(D3DXVECTOR4(sx+w,  sy+h, 0.0f,1),color,D3DXVECTOR2(tx2,ty2));

					_vecAsh[idxAsh++].SetValue(D3DXVECTOR4(sx+w,  sy,   0.0f,1),color,D3DXVECTOR2(tx2,ty1));
					_vecAsh[idxAsh++].SetValue(D3DXVECTOR4(sx+w,  sy+h, 0.0f,1),color,D3DXVECTOR2(tx2,ty2));
					_vecAsh[idxAsh++].SetValue(D3DXVECTOR4(sx,    sy,   0.0f,1),color,D3DXVECTOR2(tx1,ty1));

				}else
				{
					_vecVer[idxHsl++].SetValue(D3DXVECTOR4(sx,    sy+h, 0.0f,1),color,D3DXVECTOR2(tx1,ty2));
					_vecVer[idxHsl++].SetValue(D3DXVECTOR4(sx,	  sy,    0.0f,1),color,D3DXVECTOR2(tx1,ty1));
					_vecVer[idxHsl++].SetValue(D3DXVECTOR4(sx+w,  sy+h, 0.0f,1),color,D3DXVECTOR2(tx2,ty2));

					_vecVer[idxHsl++].SetValue(D3DXVECTOR4(sx+w,  sy,   0.0f,1),color,D3DXVECTOR2(tx2,ty1));
					_vecVer[idxHsl++].SetValue(D3DXVECTOR4(sx+w,  sy+h, 0.0f,1),color,D3DXVECTOR2(tx2,ty2));
					_vecVer[idxHsl++].SetValue(D3DXVECTOR4(sx,    sy,   0.0f,1),color,D3DXVECTOR2(tx1,ty1));
				}

				//
			}else 
			{
				// 
				tx = (float)dest.left;
				ty = (float)dest.top;
				float tw = (float)dest.right - (float)dest.left;
				float th = (float)dest.bottom - (float)dest.top;
				{
					float flerpx = fabs((float)(dest.left - rc.left) / w);
					float flerpy = fabs((float)(dest.top - rc.top)   / h);

					tu1 = (tx1 + (float)((tx2 - tx1) * flerpx));
					tv1 = (ty1 + (float)((ty2 - ty1) * flerpy));

					flerpx = fabs((float)(dest.right - rc.right)	/ w);
					flerpy = fabs((float)(dest.bottom - rc.bottom)  / h);

					tu2 = tx2 - ((tx2 - tx1) * flerpx);
					tv2 = ty2 - ((ty2 - ty1) * flerpy);
				}
				if(bExit)
				{
					_vecAsh[idxAsh++].SetValue(D3DXVECTOR4(tx,    ty+th, 0.0f,1),color,D3DXVECTOR2(tu1,tv2));
					_vecAsh[idxAsh++].SetValue(D3DXVECTOR4(tx,	  ty,    0.0f,1),color,D3DXVECTOR2(tu1,tv1));
					_vecAsh[idxAsh++].SetValue(D3DXVECTOR4(tx+tw,  ty+th, 0.0f,1),color,D3DXVECTOR2(tu2,tv2));

					_vecAsh[idxAsh++].SetValue(D3DXVECTOR4(tx+tw,  ty,   0.0f,1),color,D3DXVECTOR2(tu2,tv1));
					_vecAsh[idxAsh++].SetValue(D3DXVECTOR4(tx+tw,  ty+th, 0.0f,1),color,D3DXVECTOR2(tu2,tv2));
					_vecAsh[idxAsh++].SetValue(D3DXVECTOR4(tx,    ty,   0.0f,1),color,D3DXVECTOR2(tu1,tv1));

				}else
				{
					_vecVer[idxHsl++].SetValue(D3DXVECTOR4(tx,    ty+th, 0.0f,1),color,D3DXVECTOR2(tu1,tv2));
					_vecVer[idxHsl++].SetValue(D3DXVECTOR4(tx,	  ty,    0.0f,1),color,D3DXVECTOR2(tu1,tv1));
					_vecVer[idxHsl++].SetValue(D3DXVECTOR4(tx+tw,  ty+th, 0.0f,1),color,D3DXVECTOR2(tu2,tv2));

					_vecVer[idxHsl++].SetValue(D3DXVECTOR4(tx+tw,  ty,   0.0f,1),color,D3DXVECTOR2(tu2,tv1));
					_vecVer[idxHsl++].SetValue(D3DXVECTOR4(tx+tw,  ty+th, 0.0f,1),color,D3DXVECTOR2(tu2,tv2));
					_vecVer[idxHsl++].SetValue(D3DXVECTOR4(tx,    ty,   0.0f,1),color,D3DXVECTOR2(tu1,tv1));
				}
			}
		}
		sx+=offset;
	}
	if(idxAsh)
	{
		_pDev->SetTexture( 0, _pTexFast->GetTex() );
		_pDev->GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, idxAsh/3, _vecAsh,sizeof(FONT_VER));
	}
	if(idxHsl)
	{	
		_pDev->SetTexture( 0, _pTex->GetTex() );
		_pDev->GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, idxHsl/3, _vecVer,sizeof(FONT_VER));
	}
}

bool CMPFont::DrawTextClip( const std::string& szText, int nLen, LPRECT psrc, LPRECT pclip,D3DXCOLOR color)
{
	int x,y,rcw,rch;
	float sx = 0, sy = 0,
		offset=0, w=0, h=0, tx1=0, ty1=0, tx2=0, ty2=0;

	float tx = 0, ty = 0;
	float tu1 = 0,tv1 = 0,tu2 = 0,tv2 = 0;

	x = psrc->left;
	y = psrc->top;

	int nX = pclip->left - x;
	int nY = pclip->top - y;

	w = h = (float)(_TextSize);
	{
		_pDev->SetVertexShader(ResMgr.GetFontVS());
		_pDev->SetVertexDeclaration(ResMgr.GetFontVDecl());

		x -= *_iBufWidth;
		y -= (*_iBufHeight  - (int)(h));
		y = -y;

		rcw = pclip->right - pclip->left;
		rch = pclip->bottom - pclip->top;

		pclip->left = x + nX;
		pclip->top = y + nY;

		pclip->right = pclip->left + rcw;
		pclip->bottom = pclip->top + rch;

		_pDev->SetVertexShaderConstantF(8, color, 1);
	}

	sx = float(x); sy = float(y);
	char ch[3] = {0,0,0};
	FONT_VER * pVertices = NULL;

	for (int n=0; n<nLen; n++ )
	{
		ch[0] = szText[n];
		if ( ch[0]=='\n' )
		{
			sy = _bUseSoft ? sy + (h + 1) : sy - (h);//
			sx=float(x);
			continue;
		}

		if(ch[0] == ' ')
		{
			sx+= w/2 + HLSIZE;
			continue;
		}

		if ( ch[0] & 0x80 )
		{
			n++;
			ch[1] = szText[n];
			offset = w + ASSIZE;
		}
		else
		{
			ch[1] = '\0';
			offset = w/2 + HLSIZE ;
		}

		float a, b;

		if(!(ch[0] & 0x80) &&FindTextFromTex(ch[0], ch[1], tx1, ty1 ,tx2,ty2))
		{
			if(_lpCurTex  != _pTexFast)
			{
				_lpCurTex  = _pTexFast;
			}
		}
		else
		{
			TextToTexture( ch[0], ch[1], a, b );
			if(_lpCurTex  != _pTex)
			{
				_lpCurTex = _pTex;
			}
			//!  0.0-1.0
			tx1 = (float)a / (float)((float)_TextureSize - _fyu/*- (ch[0] & 0x80 ? _fyu/2: _fyu/4)*/);
			ty1 = (float)b / (float)((float)_TextureSize - _fyu/*- (ch[0] & 0x80 ? _fyu/2 : _fyu/4)*/);
			//!  0.0-1.0
			tx2 = tx1 + (float)_TextSize / (float)((float)_TextureSize -_fyu/* - (ch[0] & 0x80 ? _fyu/2 : _fyu/4)*/);
			ty2 = ty1 + (float)_TextSize / (float)((float)_TextureSize -_fyu/* - (ch[0] & 0x80 ? _fyu/2 : _fyu/4)*/);

		}
		if( _lpCurTex )
		{
			_pDev->SetTexture( 0, _lpCurTex->GetTex() );
		}

		{

			RECT rc, dest;
			rc.left = (LONG)sx;
			rc.top = (LONG)sy;
			rc.right = (LONG)sx + (LONG)w;
			rc.bottom = (LONG)sy + (LONG)h;
			if(!GetIntersectRect(&dest, &rc,pclip))
			{
				sx+=offset;
				continue;//
			}else if( rc.left >= pclip->left && rc.right <= pclip->right\
				&&rc.top >= pclip->top && rc.bottom <= pclip->bottom)
			{

				const D3DXVECTOR4 v[] = {
				{sx, sy, 0.9f, 1},
				{tx1, ty2, 0, 1},
				{sx, sy + h, 0.9f, 1},
				{tx1, ty1, 0, 1},
				{sx + w, sy + h, 0.9f, 1},
				{tx2, ty1, 0, 1},
				{sx + w, sy, 0.9f, 1},
				{tx2, ty2, 0, 1}
				};

				_pDev->SetVertexShaderConstantF(9,  v[0], 1);
				_pDev->SetVertexShaderConstantF(10, v[1], 1);
				_pDev->SetVertexShaderConstantF(11, v[2], 1);
				_pDev->SetVertexShaderConstantF(12, v[3], 1);
				_pDev->SetVertexShaderConstantF(13, v[4], 1);
				_pDev->SetVertexShaderConstantF(14, v[5], 1);
				_pDev->SetVertexShaderConstantF(15, v[6], 1);
				_pDev->SetVertexShaderConstantF(16, v[7], 1);
				//
			}else 
			{
				// 
				tx = (float)dest.left;
				ty = sy;//(float)dest.top;
				float tw = (float)dest.right - (float)dest.left;
				float th = (float)dest.bottom - (float)dest.top;
				if(rc.bottom > pclip->bottom)
				{
					ty += h - th;
				}

				float flerpx = fabs((float)(dest.left - rc.left) / w);
				float flerpy = fabs((float)(dest.top - rc.top)   / h);

				tu1 = (tx1 + (float)((tx2 - tx1) * flerpx));
				tv1 = (ty1 + (float)((ty2 - ty1) * flerpy));

				flerpx = fabs((float)(dest.right - rc.right)	/ w);
				flerpy = fabs((float)(dest.bottom - rc.bottom)  / h);

				tu2 = tx2 - ((tx2 - tx1) * flerpx);
				tv2 = ty2 - ((ty2 - ty1) * flerpy);

				const D3DXVECTOR4 v[] = {
				{tx, ty, 0.9f, 1},
				{tu1, tv2, 0, 1},
				{tx, (ty + th), 0.9f, 1},
				{tu1, tv1, 0, 1},
				{tx + tw, (ty + th), 0.9f, 1},
				{tu2, tv1, 0, 1},
				{tx + tw, ty, 0.9f, 1},
				{tu2, tv2, 0, 1}
				};

				_pDev->SetVertexShaderConstantF(9,  v[0], 1);
				_pDev->SetVertexShaderConstantF(10, v[1], 1);
				_pDev->SetVertexShaderConstantF(11, v[2], 1);
				_pDev->SetVertexShaderConstantF(12, v[3], 1);
				_pDev->SetVertexShaderConstantF(13, v[4], 1);
				_pDev->SetVertexShaderConstantF(14, v[5], 1);
				_pDev->SetVertexShaderConstantF(15, v[6], 1);
				_pDev->SetVertexShaderConstantF(16, v[7], 1);

			}
		}
		sx+=offset;
		_pVB->DrawSubset(0);
	}
	return true;
}


bool CMPFont::DrawText( int iNumber, int x, int y, D3DXCOLOR color,float fScale)
{
	char psz[64];
	sprintf(psz,"%d",iNumber);
	return DrawText( psz, x, y, color, fScale );
}

bool CMPFont::DrawText( const std::string& szText, int x, int y, D3DXCOLOR color,  float fScale ,DWORD* dwTime)
{
	if( szText.empty())
		return false;

	if(!_pTex || !_pTex->IsLoadingOK())
		return false;
	if(!_pTexFast || !_pTexFast->IsLoadingOK())
		return false;

	ui::UIClip* pCliper = ui::UIClip::Instance();
	if(pCliper->GetClipState())
	{
		SIZE size;
		GetTextSize(szText, &size,1.0f);

		RECT vdest,vsrc;
		vsrc.left = x;
		vsrc.top = y;
		vsrc.right = x + size.cx;
		vsrc.bottom = y +  size.cy;

		BYTE byFill = 0;
		if(!pCliper->GetIntersectRect(&vdest, &vsrc,&byFill))
			return true;
		else
		{
			if(byFill == 1)
				goto __ret;
		}
		BeginClip();
		_pCEffectFile->SetTechnique(_iRenderIdx);
		_pCEffectFile->Begin(D3DXFX_DONOTSAVESTATE);
		_pCEffectFile->Pass(0);
		DrawTextClipOnce(szText,szText.size(),&vsrc, &vdest,color);
		_pCEffectFile->End();
		EndClip();
		return true;
	}
__ret:

	int iAshNum = 0;
	int iHslNum = 0;
#ifdef DRAWONCE
#else
	if(rcFont->_vecAsh.size() != nLen * 4)
	{
		rcFont->_vecAsh.resize(nLen * 4);
		rcFont->_vecHsl.resize(nLen * 4);
	}
#endif

	float sx = 0, sy = 0,
		offset=0, w=0, h=0, tx1=0, ty1=0, tx2=0, ty2=0;
	w = h = (float)(_TextSize) * fScale;

#ifdef DRAWONCE
#else
	x -= *_iBufWidth;
	y -= *_iBufHeight  - (int)(h);
	y = -y;
#endif

	sx = float(x); sy = float(y);
	char ch[3] = {0,0,0};

	for ( int n=0; n<szText.size(); n++ )
	{
		ch[0] = szText[n];
		if ( ch[0]=='\n' )
		{
#ifdef DRAWONCE
			sy = sy + (h);
#else
			sy = sy - (h);
#endif
			sx=float(x);
			continue;
		}
		if(ch[0] == ' ')
		{
			sx+= w/2 + HLSIZE;
			continue;
		}

		if ( ch[0] & 0x80 )
		{
			n++;
			ch[1] = szText[n];
			offset = w + ASSIZE;
		}
		else
		{
			ch[1] = '\0';
			if( !_bWidthEqual )
			{
				SIZE size;
				::GetTextExtentPoint(_hDc,&ch[0],1,&size);
				offset = size.cx + HLSIZE ;
			}
			else
			{
				offset = w/2 + HLSIZE ;
			}
		}

		float a, b;
		bool bExit;


		if(!(ch[0] & 0x80) && FindTextFromTex(ch[0], ch[1], tx1, ty1 ,tx2,ty2))
		{
			bExit = true;
		}
		else
		{
			TextToTexture( ch[0], ch[1], a, b );
			bExit = false;

			//!  0.0-1.0
			tx1 = (float)a / (float)((float)_TextureSize - _fyu/*- (ch[0] & 0x80 ? _fyu/2: _fyu/4)*/);
			ty1 = (float)b / (float)((float)_TextureSize - _fyu/*- (ch[0] & 0x80 ? _fyu/2 : _fyu/4)*/);
			//!  0.0-1.0
			tx2 = tx1 + (float)_TextSize / (float)((float)_TextureSize -_fyu/* - (ch[0] & 0x80 ? _fyu/2 : _fyu/4)*/);
			ty2 = ty1 + (float)_TextSize / (float)((float)_TextureSize -_fyu/* - (ch[0] & 0x80 ? _fyu/2 : _fyu/4)*/);
		}
#ifdef DRAWONCE
		if(bExit)
		{
			_vecAsh[iAshNum++].SetValue(D3DXVECTOR4(sx,    sy+h, 0.0f,1),color,D3DXVECTOR2(tx1,ty2));
			_vecAsh[iAshNum++].SetValue(D3DXVECTOR4(sx,	  sy,    0.0f,1),color,D3DXVECTOR2(tx1,ty1));
			_vecAsh[iAshNum++].SetValue(D3DXVECTOR4(sx+w,  sy+h, 0.0f,1),color,D3DXVECTOR2(tx2,ty2));

			_vecAsh[iAshNum++].SetValue(D3DXVECTOR4(sx+w,  sy,   0.0f,1),color,D3DXVECTOR2(tx2,ty1));
			_vecAsh[iAshNum++].SetValue(D3DXVECTOR4(sx+w,  sy+h, 0.0f,1),color,D3DXVECTOR2(tx2,ty2));
			_vecAsh[iAshNum++].SetValue(D3DXVECTOR4(sx,    sy,   0.0f,1),color,D3DXVECTOR2(tx1,ty1));

		}else
		{
			_vecVer[iHslNum++].SetValue(D3DXVECTOR4(sx,    sy+h, 0.0f,1),color,D3DXVECTOR2(tx1,ty2));
			_vecVer[iHslNum++].SetValue(D3DXVECTOR4(sx,	  sy,    0.0f,1),color,D3DXVECTOR2(tx1,ty1));
			_vecVer[iHslNum++].SetValue(D3DXVECTOR4(sx+w,  sy+h, 0.0f,1),color,D3DXVECTOR2(tx2,ty2));

			_vecVer[iHslNum++].SetValue(D3DXVECTOR4(sx+w,  sy,   0.0f,1),color,D3DXVECTOR2(tx2,ty1));
			_vecVer[iHslNum++].SetValue(D3DXVECTOR4(sx+w,  sy+h, 0.0f,1),color,D3DXVECTOR2(tx2,ty2));
			_vecVer[iHslNum++].SetValue(D3DXVECTOR4(sx,    sy,   0.0f,1),color,D3DXVECTOR2(tx1,ty1));

		}
#else
		if(bExit)
		{
			rcFont->_vecAsh[iAshNum++] = D3DXVECTOR4(sx,	  sy,    tx1,   ty2);
			rcFont->_vecAsh[iAshNum++] = D3DXVECTOR4(sx,    sy+h,    tx1,   ty1);
			rcFont->_vecAsh[iAshNum++] = D3DXVECTOR4(sx+w,  sy+h,    tx2,   ty1);
			rcFont->_vecAsh[iAshNum++] = D3DXVECTOR4(sx+w,  sy,		 tx2,  ty2);

		}else
		{
			rcFont->_vecHsl[iHslNum++] = D3DXVECTOR4(sx,	  sy,    tx1,   ty2);
			rcFont->_vecHsl[iHslNum++] = D3DXVECTOR4(sx,    sy+h,    tx1,   ty1);
			rcFont->_vecHsl[iHslNum++] = D3DXVECTOR4(sx+w,  sy+h,    tx2,   ty1);
			rcFont->_vecHsl[iHslNum++] = D3DXVECTOR4(sx+w,  sy,		 tx2,  ty2);
		}
#endif
		sx+=offset;
	}
#ifdef DRAWONCE
	iAshNum = iAshNum/3;
	iHslNum = iHslNum/3;

#else
	rcFont->iAshNum = iAshNum/4;
	rcFont->iHslNum = iHslNum/4;
#endif

	_pCEffectFile->SetTechnique(_iRenderIdx);
	_pCEffectFile->Begin(D3DXFX_DONOTSAVESTATE);
	_pCEffectFile->Pass(0);

	_pDev->SetVertexShader(NULL);
	_pDev->SetFVF( D3DFVF_FONT );
	if(iAshNum)
	{
		_pDev->SetTexture( 0, _pTexFast->GetTex() );
		_pDev->GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, iAshNum, _vecAsh,sizeof(FONT_VER));
	}
	if(iHslNum)
	{	
		_pDev->SetTexture( 0, _pTex->GetTex() );
		_pDev->GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, iHslNum, _vecVer,sizeof(FONT_VER));
	}
	_pCEffectFile->End();

	return true;
}


#ifdef DRAWONCE
void  CMPFont::RenderOptimize(int iIdx)
{
	MPFontRect *prc = &_vecFontRect[iIdx];
	_pCEffectFile->SetTechnique(_iRenderIdx);
	_pCEffectFile->Begin(D3DXFX_DONOTSAVESTATE);
	_pCEffectFile->Pass(0);

	_pDev->SetVertexShader(NULL);
	_pDev->SetFVF( D3DFVF_FONT );
	if(prc->iAshNum)
	{
		_pDev->SetTexture( 0, _pTexFast->GetTex() );
		_pDev->GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, prc->iAshNum, &prc->_vecAsh.front(),sizeof(FONT_VER));
	}
	if(prc->iHslNum)
	{	
		_pDev->SetTexture( 0, _pTex->GetTex() );
		_pDev->GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, prc->iHslNum, &prc->_vecHsl.front(),sizeof(FONT_VER));
	}
	_pCEffectFile->End();
	_pDev->SetRenderStateForced(D3DRS_ZENABLE,TRUE);
	_pDev->SetRenderStateForced(D3DRS_ZWRITEENABLE,TRUE);
}
void CMPFont::RenderDrawOptimize(int iIdx)
{
	MPFontRect *prc = &_vecFontRect[iIdx];
	_pDev->SetVertexShader(NULL);
	_pDev->SetFVF( D3DFVF_FONT );

	if(prc->iAshNum)
	{
		_pDev->SetTexture( 0, _pTexFast->GetTex() );
		_pDev->GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, prc->iAshNum, &prc->_vecAsh.front(),sizeof(FONT_VER));
	}
	if(prc->iHslNum)
	{	
		_pDev->SetTexture( 0, _pTex->GetTex() );
		_pDev->GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, prc->iHslNum, &prc->_vecHsl.front(),sizeof(FONT_VER));
	}
}
#else
void  CMPFont::RenderOptimize(int iIdx)
{
	MPFontRect *prc = &_vecFontRect[iIdx];

	_pVB->BeginSet();

	_pCEffectFile->SetTechnique(_iRenderIdx);
	_pCEffectFile->Begin();
	_pCEffectFile->Pass(0);
	_pDev->SetVertexShader( *_pdwVS );

	D3DXMATRIX matIdentity;
	D3DXMatrixIdentity(&matIdentity);

	_pDev->SetVertexShaderConstant(0, &matIdentity, 4);
	_pDev->SetVertexShaderConstant(4, _pmat2DViewProj, 4);

	D3DXCOLOR color = prc->dwColor;
	_pDev->SetVertexShaderConstant(8, &color, 1);

	D3DXVECTOR4* pData;
	if(prc->iAshNum)
	{
		_pDev->SetTexture( 0, _pTexFast->GetTex() );
		for ( INT n = 0; n <prc->iAshNum; ++n)
		{
			pData = &prc->_vecAsh[n * 4];
			_pDev->SetVertexShaderConstant(9,  &D3DXVECTOR4(pData->x,	  pData->y,    0.9f, 1), 1);
			_pDev->SetVertexShaderConstant(10, &D3DXVECTOR4(pData->z,   pData->w,   0, 1), 1);

			pData++;
			_pDev->SetVertexShaderConstant(11, &D3DXVECTOR4(pData->x,	  pData->y,  0.9f, 1), 1);
			_pDev->SetVertexShaderConstant(12, &D3DXVECTOR4(pData->z,   pData->w,   0, 1), 1);

			pData++;
			_pDev->SetVertexShaderConstant(13, &D3DXVECTOR4(pData->x,	  pData->y, 0.9f, 1), 1);
			_pDev->SetVertexShaderConstant(14, &D3DXVECTOR4(pData->z,   pData->w,   0, 1), 1);

			pData++;
			_pDev->SetVertexShaderConstant(15, &D3DXVECTOR4(pData->x,	  pData->y,    0.9f, 1), 1);
			_pDev->SetVertexShaderConstant(16, &D3DXVECTOR4(pData->z,   pData->w,   0, 1), 1);

			_pVB->DrawSubset(0);
		}
	}
	if(prc->iHslNum)
	{	
		_pDev->SetTexture( 0, _pTex->GetTex() );
		for ( INT n = 0; n < prc->iHslNum; ++n)
		{
			pData = &prc->_vecHsl[n * 4];
			_pDev->SetVertexShaderConstant(9,  &D3DXVECTOR4(pData->x,	  pData->y,    0.9f, 1), 1);
			_pDev->SetVertexShaderConstant(10, &D3DXVECTOR4(pData->z,   pData->w,   0, 1), 1);

			pData++;
			_pDev->SetVertexShaderConstant(11, &D3DXVECTOR4(pData->x,	  pData->y,  0.9f, 1), 1);
			_pDev->SetVertexShaderConstant(12, &D3DXVECTOR4(pData->z,   pData->w,   0, 1), 1);

			pData++;
			_pDev->SetVertexShaderConstant(13, &D3DXVECTOR4(pData->x,	  pData->y, 0.9f, 1), 1);
			_pDev->SetVertexShaderConstant(14, &D3DXVECTOR4(pData->z,   pData->w,   0, 1), 1);

			pData++;
			_pDev->SetVertexShaderConstant(15, &D3DXVECTOR4(pData->x,	  pData->y,    0.9f, 1), 1);
			_pDev->SetVertexShaderConstant(16, &D3DXVECTOR4(pData->z,   pData->w,   0, 1), 1);

			_pVB->DrawSubset(0);
		}
	}

	_pCEffectFile->End();

	_pVB->EndSet();
	_pDev->SetVertexShader( D3DFVF_FONT);

	_pDev->SetRenderStateForced(D3DRS_ZENABLE,TRUE);
	_pDev->SetRenderStateForced(D3DRS_ZWRITEENABLE,TRUE);
}

void CMPFont::RenderDrawOptimize(int iIdx)
{
	MPFontRect *prc = &_vecFontRect[iIdx];
	D3DXCOLOR color = prc->dwColor;
	_pDev->SetVertexShaderConstant(8, &color, 1);

	D3DXVECTOR4* pData;
	if(prc->iAshNum)
	{
		_pDev->SetTexture( 0, _pTexFast->GetTex() );
		for ( INT n = 0; n <prc->iAshNum; ++n)
		{
			pData = &prc->_vecAsh[n * 4];
			_pDev->SetVertexShaderConstant(9,  &D3DXVECTOR4(pData->x,	  pData->y,    0.9f, 1), 1);
			_pDev->SetVertexShaderConstant(10, &D3DXVECTOR4(pData->z,   pData->w,   0, 1), 1);

			pData++;
			_pDev->SetVertexShaderConstant(11, &D3DXVECTOR4(pData->x,	  pData->y,  0.9f, 1), 1);
			_pDev->SetVertexShaderConstant(12, &D3DXVECTOR4(pData->z,   pData->w,   0, 1), 1);

			pData++;
			_pDev->SetVertexShaderConstant(13, &D3DXVECTOR4(pData->x,	  pData->y, 0.9f, 1), 1);
			_pDev->SetVertexShaderConstant(14, &D3DXVECTOR4(pData->z,   pData->w,   0, 1), 1);

			pData++;
			_pDev->SetVertexShaderConstant(15, &D3DXVECTOR4(pData->x,	  pData->y,    0.9f, 1), 1);
			_pDev->SetVertexShaderConstant(16, &D3DXVECTOR4(pData->z,   pData->w,   0, 1), 1);

			_pVB->DrawSubset(0);
		}
	}
	if(prc->iHslNum)
	{	
		_pDev->SetTexture( 0, _pTex->GetTex() );
		for ( INT n = 0; n < prc->iHslNum; ++n)
		{
			pData = &prc->_vecHsl[n * 4];
			_pDev->SetVertexShaderConstant(9,  &D3DXVECTOR4(pData->x,	  pData->y,    0.9f, 1), 1);
			_pDev->SetVertexShaderConstant(10, &D3DXVECTOR4(pData->z,   pData->w,   0, 1), 1);

			pData++;
			_pDev->SetVertexShaderConstant(11, &D3DXVECTOR4(pData->x,	  pData->y,  0.9f, 1), 1);
			_pDev->SetVertexShaderConstant(12, &D3DXVECTOR4(pData->z,   pData->w,   0, 1), 1);

			pData++;
			_pDev->SetVertexShaderConstant(13, &D3DXVECTOR4(pData->x,	  pData->y, 0.9f, 1), 1);
			_pDev->SetVertexShaderConstant(14, &D3DXVECTOR4(pData->z,   pData->w,   0, 1), 1);

			pData++;
			_pDev->SetVertexShaderConstant(15, &D3DXVECTOR4(pData->x,	  pData->y,    0.9f, 1), 1);
			_pDev->SetVertexShaderConstant(16, &D3DXVECTOR4(pData->z,   pData->w,   0, 1), 1);

			_pVB->DrawSubset(0);
		}
	}
}
#endif

inline void CMPFont::Begin()
{
	_lpCurTex  = _pTex;
#ifdef DRAWONCE
	_pCEffectFile->SetTechnique(_iRenderIdx);
	_pCEffectFile->Begin(D3DXFX_DONOTSAVESTATE);
	_pCEffectFile->Pass(0);

#else
	_pVB->BeginSet();
	_pCEffectFile->SetTechnique(_iRenderIdx);
	_pCEffectFile->Begin();
	_pCEffectFile->Pass(0);
	{
		_pDev->SetVertexShader( *_pdwVS );
		D3DXMATRIX matIdentity;
		D3DXMatrixIdentity(&matIdentity);
		_pDev->SetVertexShaderConstant(0, &matIdentity, 4);
		_pDev->SetVertexShaderConstant(4, _pmat2DViewProj, 4);
	}
#endif
}
inline void CMPFont::End()
{
#ifdef DRAWONCE
	_pCEffectFile->End();
#else
	_pCEffectFile->End();
	_pVB->EndSet();
	_pDev->SetVertexShader( D3DFVF_FONT);
#endif
	_pDev->SetRenderStateForced(D3DRS_ZENABLE,TRUE);
	_pDev->SetRenderStateForced(D3DRS_ZWRITEENABLE,TRUE);
}


void CMPFont::BeginClip()
{
}
void CMPFont::EndClip()
{
}

void CMPFont::Draw(const std::string& szText, int x, int y, D3DXCOLOR color)
{
	
	if( szText.empty())
		return;
	if(!_pTex || !_pTex->IsLoadingOK())
		return;
	if(!_pTexFast || !_pTexFast->IsLoadingOK())
		return;
	ui::UIClip* pCliper = ui::UIClip::Instance();
	if(pCliper->GetClipState())
	{
		SIZE size;
		GetTextSize(szText, &size, 1.0f);
		RECT vdest,vsrc;
		vsrc.left = x;
		vsrc.top = y;
		vsrc.right = x + size.cx;
		vsrc.bottom = y +  size.cy;

		RECT* prc = &pCliper->GetClipRect();

		BYTE byFill = 0;
		if(!pCliper->GetIntersectRect(&vdest, &vsrc,&byFill))
			return ;
		else
		{
			if(byFill == 1)
				goto __ret;
		}
#ifdef DRAWONCE
		BeginClip();
		DrawTextClipOnce(szText,szText.size(),&vsrc, &vdest,color);
		EndClip();
#else
		DrawTextClip(szText,nLen,&vsrc, &vdest,color);
#endif
		return;
	}
__ret:

	int iAshNum = 0;
	int iHslNum = 0;
#ifdef DRAWONCE
#else
	if (rcFont->_vecAsh.size() != nLen * 4)
	{
		rcFont->_vecAsh.resize(nLen * 4);
		rcFont->_vecHsl.resize(nLen * 4);
	}
#endif

	float sx = 0, sy = 0,
		offset=0, w=0, h=0, tx1=0, ty1=0, tx2=0, ty2=0;
	w = h = (float)(_TextSize);

#ifdef DRAWONCE
#else
	x -= *_iBufWidth;
	y -= *_iBufHeight  - (int)(h);
	y = -y;
#endif

	sx = float(x); sy = float(y);
	char ch[3] = {0,0,0};

	FONT_VER * pVertices = NULL;

	for ( int n=0; n< szText.size(); n++ )
	{
		ch[0] = szText[n];
		if ( ch[0]=='\n' )
		{
#ifdef DRAWONCE
			sy = sy + (h);
#else
			sy = sy - (h);
#endif
			sx=float(x);
			continue;
		}
		if(ch[0] == ' ')
		{
			sx+= w/2 + HLSIZE;
			continue;
		}

		if ( ch[0] & 0x80 )
		{
			n++;
			ch[1] = szText[n];
			offset = w + ASSIZE;
		}
		else
		{
			ch[1] = '\0';
			offset = w/2 + HLSIZE ;
		}

		float a, b;
		bool bExit;
		if(!(ch[0] & 0x80) &&FindTextFromTex(ch[0], ch[1], tx1, ty1 ,tx2,ty2))
		{
			bExit = true;
		}
		else
		{
			TextToTexture( ch[0], ch[1], a, b );
			bExit = false;


			//!  0.0-1.0
			tx1 = (float)a / (float)((float)_TextureSize -_fyu);
			ty1 = (float)b / (float)((float)_TextureSize -_fyu);
			//!  0.0-1.0
			tx2 = tx1 + (float)_TextSize / (float)((float)_TextureSize -_fyu);
			ty2 = ty1 + (float)_TextSize / (float)((float)_TextureSize -_fyu);
		}

#ifdef DRAWONCE
		if(bExit)
		{
			_vecAsh[iAshNum++].SetValue(D3DXVECTOR4(sx,    sy+h, 0.0f,1),color,D3DXVECTOR2(tx1,ty2));
			_vecAsh[iAshNum++].SetValue(D3DXVECTOR4(sx,	  sy,    0.0f,1),color,D3DXVECTOR2(tx1,ty1));
			_vecAsh[iAshNum++].SetValue(D3DXVECTOR4(sx+w,  sy+h, 0.0f,1),color,D3DXVECTOR2(tx2,ty2));

			_vecAsh[iAshNum++].SetValue(D3DXVECTOR4(sx+w,  sy,   0.0f,1),color,D3DXVECTOR2(tx2,ty1));
			_vecAsh[iAshNum++].SetValue(D3DXVECTOR4(sx+w,  sy+h, 0.0f,1),color,D3DXVECTOR2(tx2,ty2));
			_vecAsh[iAshNum++].SetValue(D3DXVECTOR4(sx,    sy,   0.0f,1),color,D3DXVECTOR2(tx1,ty1));

		}else
		{
			_vecVer[iHslNum++].SetValue(D3DXVECTOR4(sx,    sy+h, 0.0f,1),color,D3DXVECTOR2(tx1,ty2));
			_vecVer[iHslNum++].SetValue(D3DXVECTOR4(sx,	  sy,    0.0f,1),color,D3DXVECTOR2(tx1,ty1));
			_vecVer[iHslNum++].SetValue(D3DXVECTOR4(sx+w,  sy+h, 0.0f,1),color,D3DXVECTOR2(tx2,ty2));

			_vecVer[iHslNum++].SetValue(D3DXVECTOR4(sx+w,  sy,   0.0f,1),color,D3DXVECTOR2(tx2,ty1));
			_vecVer[iHslNum++].SetValue(D3DXVECTOR4(sx+w,  sy+h, 0.0f,1),color,D3DXVECTOR2(tx2,ty2));
			_vecVer[iHslNum++].SetValue(D3DXVECTOR4(sx,    sy,   0.0f,1),color,D3DXVECTOR2(tx1,ty1));
		}

#else
		if(bExit)
		{
			rcFont->_vecAsh[iAshNum++] = D3DXVECTOR4(sx,	  sy,    tx1,   ty2);
			rcFont->_vecAsh[iAshNum++] = D3DXVECTOR4(sx,    sy+h,    tx1,   ty1);
			rcFont->_vecAsh[iAshNum++] = D3DXVECTOR4(sx+w,  sy+h,    tx2,   ty1);
			rcFont->_vecAsh[iAshNum++] = D3DXVECTOR4(sx+w,  sy,		 tx2,  ty2);

		}else
		{
			rcFont->_vecHsl[iHslNum++] = D3DXVECTOR4(sx,	  sy,    tx1,   ty2);
			rcFont->_vecHsl[iHslNum++] = D3DXVECTOR4(sx,    sy+h,    tx1,   ty1);
			rcFont->_vecHsl[iHslNum++] = D3DXVECTOR4(sx+w,  sy+h,    tx2,   ty1);
			rcFont->_vecHsl[iHslNum++] = D3DXVECTOR4(sx+w,  sy,		 tx2,  ty2);
		}
#endif
		sx+=offset;
	}
#ifdef DRAWONCE
	iAshNum = iAshNum/3;
	iHslNum = iHslNum/3;

#else
	rcFont->iAshNum = iAshNum/4;
	rcFont->iHslNum = iHslNum/4;
#endif
	_pDev->SetVertexShader(NULL);
	_pDev->SetFVF( D3DFVF_FONT );

	if(iAshNum)
	{
		_pDev->SetTexture( 0, _pTexFast->GetTex() );
		_pDev->GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, iAshNum, _vecAsh,sizeof(FONT_VER));
	}
	if(iHslNum)
	{	
		_pDev->SetTexture( 0, _pTex->GetTex() );
		_pDev->GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, iHslNum, _vecVer,sizeof(FONT_VER));
	}

}

bool CMPFont::DrawTextShadow( const std::string& szText, int x1, int y1, int x2, int y2,
							 D3DXCOLOR color1,D3DXCOLOR color2)
{
	if(szText.empty())
		return false;
	Begin();
	Draw(szText, x1, y1, color1);			//Add by mdr.st May 2020 - FPO alpha
	Draw(szText, x2, y2, color2);
	End();
	return true;
}

bool CMPFont::Draw3DText(const std::string& szText,D3DXVECTOR3& vPos, D3DXCOLOR color,float fScale)
{
	if( szText.empty())
		return false;
	if(!_pTex || !_pTex->IsLoadingOK())
		return false;
	if(!_pTexFast || !_pTexFast->IsLoadingOK())
		return false;

#ifdef USE_RENDER
	if( _lpCurTex )
	{
		_pDev->SetTexture( 0, _lpCurTex->GetTex() );
	}
	_pVB->BeginSet();
#else
	if( _lpCurTex )
	{
		_pDev->SetTexture( 0, _lpCurTex );
	}
	_pDev->SetStreamSource( 0, _pVB, sizeof(FONT_VER) );
#endif

	_pCEffectFile->SetTechnique(_iRenderIdx);
	_pCEffectFile->Begin(D3DXFX_DONOTSAVESTATE);
	_pCEffectFile->Pass(0);
	D3DXMATRIX matIdentity;
	D3DXMatrixScaling(&matIdentity,fScale,fScale,fScale );
	D3DXMatrixMultiply(&matIdentity, _pmatBBoard, &matIdentity);
	matIdentity._41 = vPos.x;
	matIdentity._42 = vPos.y;
	matIdentity._43 = vPos.z;
	if(!_bUseSoft)
	{
		D3DXMatrixTranspose(&matIdentity, &matIdentity);
		_pDev->SetVertexShader(ResMgr.GetFontVS());
		_pDev->SetVertexDeclaration(ResMgr.GetFontVDecl());
		_pDev->SetVertexShaderConstantF(0, matIdentity, 4);
		_pDev->SetVertexShaderConstantF(4, *_pmatViewProj, 4);
		_pDev->SetVertexShaderConstantF(8, color, 1);
	}
	else
	{
		_pDev->SetVertexShader(NULL);
		_pDev->SetFVF( D3DFVF_3DFONT );
#ifdef USE_RENDER
		_pDev->SetTransformWorld(&matIdentity);
#else
		_pDev->SetTransform(D3DTS_WORLDMATRIX(0), &matIdentity);
#endif

	}

	float sx =0, sy = 0,offset=0, w=0, h=0, tx1=0, ty1=0, tx2=0, ty2=0;
	w = h = 1.0f;


	char ch[3] = {0,0,0};
	FONT_VER * pVertices = NULL;
	int nLen = szText.size();

	for (int n=0; n<nLen; n++ )
	{
		ch[0] = szText[n];
		if ( ch[0]=='\n' )
		{
			sy -= h;
			sx = 0;
			continue;
		}
		if(ch[0] == ' ')
		{
			sx += w/2 /*+ HLSIZE*/;
			continue;
		}

		if ( ch[0] & 0x80 )
		{
			n++;
			ch[1] = szText[n];
			offset = w /*+ ASSIZE*/;
		}
		else
		{
			ch[1] = '\0';
			offset =  w/2 /*+HLSIZE*/ ;
		}

		float a, b;
		if(!(ch[0] & 0x80) &&FindTextFromTex(ch[0], ch[1], tx1, ty1 ,tx2,ty2))
		{
			if(_lpCurTex && _lpCurTex  != _pTexFast)
			{
				_lpCurTex  = _pTexFast;
#ifdef USE_RENDER
				_pDev->SetTexture( 0, _lpCurTex->GetTex() );
#else
				_pDev->SetTexture( 0, _lpCurTex );
#endif
			}
		}
		else
		{
			TextToTexture( ch[0], ch[1], a, b );
			if(_lpCurTex && _lpCurTex  != _pTex)
			{
				_lpCurTex = _pTex;
#ifdef USE_RENDER
				_pDev->SetTexture( 0, _lpCurTex->GetTex() );
#else
				_pDev->SetTexture( 0, _lpCurTex );
#endif
			}
			//!  0.0-1.0
			tx1 = (float)(a) / (float)(_TextureSize -_fyu/*  - (ch[0] & 0x80 ? _fyu /2: _fyu/4)*/);
			ty1 = (float)(b) / (float)(_TextureSize -_fyu/*  - (ch[0] & 0x80 ? _fyu /2: _fyu/4)*/);
			//!  0.0-1.0
			tx2 = tx1 + (float)_TextSize / (float)(_TextureSize -_fyu/* - (ch[0] & 0x80 ? _fyu/2 : _fyu/4)*/);
			ty2 = ty1 + (float)_TextSize / (float)(_TextureSize -_fyu/* - (ch[0] & 0x80 ? _fyu/2 : _fyu/4)*/);
		}

		if(!_bUseSoft)
		{
			const D3DXVECTOR4 v[] = {
			{sx, sy, 0, 1},
			{tx1, ty2, 0, 1},
			{sx, sy + h, 0, 1},
			{tx1, ty1, 0, 1},
			{sx + w, sy, 0, 1},
			{tx2, ty2, 0, 1},
			{sx + w, sy + h, 0, 1},
			{tx2, ty1, 0, 1},
			{sx + w, sy, 0, 1},
			{tx2, ty2, 0, 1},
			{sx, sy + h, 0, 1},
			{tx1, ty1, 0, 1}
			};

			_pDev->SetVertexShaderConstantF(9,  v[0], 1);
			_pDev->SetVertexShaderConstantF(10, v[1], 1);
			_pDev->SetVertexShaderConstantF(11, v[2], 1);
			_pDev->SetVertexShaderConstantF(12, v[3], 1);
			_pDev->SetVertexShaderConstantF(13, v[4], 1);
			_pDev->SetVertexShaderConstantF(14, v[5], 1);
			_pDev->SetVertexShaderConstantF(15, v[6], 1);
			_pDev->SetVertexShaderConstantF(16, v[7], 1);
			_pDev->SetVertexShaderConstantF(17, v[8], 1);
			_pDev->SetVertexShaderConstantF(18, v[9], 1);
			_pDev->SetVertexShaderConstantF(19, v[10], 1);
			_pDev->SetVertexShaderConstantF(20, v[11], 1);
		}
		else
		{
#ifdef USE_RENDER
			lwILockableStreamVB*	lpSVB = _pVB->GetLockableStreamVB();
			lpSVB->Lock(0, 0, (void**)&pVertices, D3DLOCK_DISCARD);
#else
			_pVB->Lock(0, 0, (BYTE**)&pVertices, D3DLOCK_DISCARD);
#endif
		


			(*pVertices++).SetValue(D3DXVECTOR4(sx,	   sy,   0,1),color,D3DXVECTOR2(tx1,ty2));
			(*pVertices++).SetValue(D3DXVECTOR4(sx,    sy+h, 0,1),color,D3DXVECTOR2(tx1,ty1));
			(*pVertices++).SetValue(D3DXVECTOR4(sx+w,  sy,   0,1),color,D3DXVECTOR2(tx2,ty2));

			(*pVertices++).SetValue(D3DXVECTOR4(sx+w,  sy+h, 0,1),color,D3DXVECTOR2(tx2,ty1));
			(*pVertices++).SetValue(D3DXVECTOR4(sx+w,  sy,   0,1),color,D3DXVECTOR2(tx2,ty2));
			(*pVertices++).SetValue(D3DXVECTOR4(sx,    sy+h, 0,1),color,D3DXVECTOR2(tx1,ty1));

#ifdef USE_RENDER
			lpSVB->Unlock();
#else
			_pVB->Unlock();
#endif

		}
		sx+=offset;
#ifdef USE_RENDER
		if(LW_FAILED(_pVB->DrawSubset(0)))
			ToLogService("error", "msgCMPFont draw");
#else
		_pDev->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 2 );
#endif
	}
#ifdef USE_RENDER
	_pVB->EndSet();
#endif
	_pCEffectFile->End();
	_pDev->SetRenderStateForced(D3DRS_ZENABLE,TRUE);
	_pDev->SetRenderStateForced(D3DRS_ZWRITEENABLE,TRUE);
	_pDev->SetVertexShader(NULL);
	_pDev->SetFVF( D3DFVF_FONT);
	return true;
}

void CMPFont::ReleaseFont()
{

	SAFE_RELEASE(_pVB);
	SAFE_RELEASE(_pTex);
	_vecBuf.clear();
	SAFE_RELEASE(_pTexFast );
	_vecBufFast.clear();
}

bool CMPFont::FindTextFromTex( char c1, char c2, float & tX1, float & tY1 , float & tX2, float & tY2)
{
	WORD w = MAKEWORD(c1, c2);
	if(_mapUV.count(w)==0)
	{
		return false;
	}
	D3DXVECTOR4 ver =  _mapUV[w];
	tX1 = ver.x;
	tY1 = ver.y;
	tX2 = ver.z;
	tY2 = ver.w;

	return true;
}

void CMPFont::FillTextToTex( const std::string& szText )
{
skip:

	if(!_pTexFast->IsLoadingOK())
	{ 
		Sleep(10);
		goto skip;
	}
	float single =  float(_TextSize)/float(_TextureSizeFast);
	float yue = 0;// (float)_TextureSizeFast / (float)_TextSize - (float)((int)(_TextureSizeFast / _TextSize));
	char ch[3] = {0,0,0};
	int nLen = szText.size();

	for (int n=0; n<nLen; n++ )
	{
		ch[0] = szText[n];
		if ( ch[0]=='\n' )
		{
			continue;
		}
		if(ch[0] == '\t')
		{
			continue;
		}
		if(ch[0] == ' ')
		{
			continue;
		}

		if ( ch[0] & 0x80 )
		{
			n++;
			ch[1] = szText[n];
		}
		else
		{
			ch[1] = '\0';
		}

		WORD w = MAKEWORD(ch[0], ch[1]);
		vector<WORD>::iterator it = std::find( _vecBufFast.begin(), _vecBufFast.end(), w );
		if ( it == _vecBufFast.end() )
		{
			it = find( _vecBufFast.begin(), _vecBufFast.end(), 0 );
			if ( it == _vecBufFast.end() )
			{
				return;
			}
			int at = (int)(it - _vecBufFast.begin());

			float tX = (float)(at % _RowNumFast) * _TextSize;
			float tY = (float)(at / _RowNumFast) * _TextSize;

			(*it) = w;

			if(_mapUV.count(w)==0)
			{
				D3DXVECTOR4 sUV;
				sUV.x = (float)(tX) / ((float)(float)_TextureSizeFast -yue /*- (ch[0] & 0x80 ? yue /2: yue/4*/);
				sUV.y = (float)(tY) / ((float)(float)_TextureSizeFast -yue /* - (ch[0] & 0x80 ? yue /2: yue/4*/);
				sUV.z = sUV.x + (float)_TextSize / ((float)_TextureSizeFast -yue/* - (ch[0] & 0x80 ? yue /2: yue/4)*/);
				sUV.w = sUV.y + (float)_TextSize / ((float)_TextureSizeFast -yue/*  - (ch[0] & 0x80 ? yue /2: yue/4)*/);
				_mapUV[w] = sUV; 
			}

			RECT rect = {0, 0, _TextSize, _TextSize};
			char sz[3] = {ch[0], ch[1], '\0'};


			FillRect( _hDc, &rect, (HBRUSH)GetStockObject(BLACK_BRUSH) );

			::TextOut( _hDc, 0, 0, sz, ch[0] & 0x80 ? 2 : 1 );

			D3DLOCKED_RECT d3dlr;
#ifdef USE_RENDER
			_pTexFast->GetTex()->LockRect(0, &d3dlr, NULL, D3DLOCK_NOSYSLOCK);
#else
			_pTexFast->LockRect(0, &d3dlr, NULL, D3DLOCK_NOSYSLOCK);
#endif
			BYTE * pDstRow = (BYTE*)( (WORD *)d3dlr.pBits + (int)tY * _TextureSizeFast + (int)tX );

			for (WORD y=0; y < _TextSize; y++)
			{
				WORD * pDst16 = (WORD*)pDstRow;
				for (WORD x=0; x<_TextSize; x++)
				{
					BYTE bAlpha = (BYTE)((_pBits[_TextSize * y + x] & 0xff) >> 4);
					if (bAlpha > 0)
						*pDst16++ = (bAlpha << 12) | 0x0fff;
					else
						*pDst16++ = 0x0000;
				}
				pDstRow += d3dlr.Pitch;
			}
#ifdef USE_RENDER
			_pTexFast->GetTex()->UnlockRect( NULL );
#else
			_pTexFast->UnlockRect( NULL );
#endif
		}
	}
}
