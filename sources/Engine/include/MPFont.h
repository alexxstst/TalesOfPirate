#pragma once

#include <algorithm>

inline bool	GetIntersectRect(RECT* pdest, RECT* psrc,RECT* pclip)
{
	int x1 = 0, x2 = 0, y1 = 0, y2 = 0;
	// ͼ���ڲü����⣬����ʾ
	if( psrc->left >= pclip->right || psrc->right <= pclip->left\
		||psrc->top >= pclip->bottom || psrc->bottom <= pclip->top)
	{
		return false;
	}
	// ���вü�
	if( psrc->left < pclip->left )
	{
		x1 = psrc->left - pclip->left;
	}
	if( psrc->right > pclip->right )
	{
		x2 = pclip->right - psrc->right;
	}
	if( psrc->top < pclip->top )
	{
		y1 = psrc->top - pclip->top;
	}
	if( psrc->bottom > pclip->bottom )
	{
		y2 = pclip->bottom - psrc->bottom;
	}
	pdest->left  = psrc->left - x1;
	pdest->right = psrc->right + x2;
	pdest->top   = psrc->top - y1;
 	pdest->bottom = psrc->bottom + y2;
	return true;
}
namespace ui
{
#define UI_STATE_NOCLIP		0
#define UI_STATE_CLIP		1

	class MINDPOWER_API UIClip
	{
	public:
		static UIClip* Instance() 
		{ 
			if( m_pClip ) return m_pClip;
			m_pClip = new UIClip;
			return m_pClip;
		}

		void	SetClipRect( int x, int y, int w, int h ) 
		{ 
			m_rectClip.left = x;
			m_rectClip.top  = y;
			m_rectClip.right = x + w;
			m_rectClip.bottom = y + h;
			m_byState = UI_STATE_CLIP;
		}

		void	Reset() { m_byState = UI_STATE_NOCLIP; }

		BYTE	GetClipState() { return m_byState; }
		RECT&	GetClipRect() { return m_rectClip; }

		static UIClip* GetCliper()		{ return m_pClip;}

		bool	GetIntersectRect(RECT* pdest, RECT* psrc, BYTE* byFill = NULL)
		{
			int x1 = 0, x2 = 0, y1 = 0, y2 = 0;
			RECT* prc = &GetClipRect();
			// ͼ���ڲü����⣬����ʾ
			if( psrc->left >= prc->right || psrc->right <= prc->left\
				||psrc->top >= prc->bottom || psrc->bottom <= prc->top)
			{
				return false;
			}
			if(byFill)
			{
				if( psrc->left >= prc->left && psrc->right <= prc->right\
					&&psrc->top >= prc->top && psrc->bottom <= prc->bottom)
				{
					*byFill = 1;
					return true;
				}
			}
			// ���вü�

			if( psrc->left < prc->left )
			{
				x1 = psrc->left - prc->left;
			}
			if( psrc->right > prc->right )
			{
				x2 = prc->right - psrc->right;
			}
			if( psrc->top < prc->top )
			{
				y1 = psrc->top - prc->top;
			}
			if( psrc->bottom > prc->bottom )
			{
				y2 = prc->bottom - psrc->bottom;
			}
			pdest->left  = psrc->left - x1;
			pdest->right = psrc->right + x2;
			pdest->top   = psrc->top - y1;
			pdest->bottom = psrc->bottom + y2;
			return true;
		}

	protected:
		UIClip();
		~UIClip();

	private:
		BYTE	m_byState;
		RECT	m_rectClip;
		static UIClip*	m_pClip;
	};
}


/*
	bool CreateFont( LPDIRECT3DDEVICE8 pd3dDevice, char* szFontName, int nSize = 16, int nLevel = 3 );
	\param1		3D�豸
	\param2		��������
	\param3		�����С������ʹ��16��24����
	\param4		������С�ȼ�������Ϊ1 -- 5.�ֱ�Ϊ1=64 2=128 3=256 4=512 5=1024��
				NOTE: 
				1.���ʹ��С��256��������ʱ����ʹ��֣�����뷨ʱ��ʱһ�����������������ڻ�����Ŀ��
				��������Ĭ��������СΪ256.���ھ�������Կ���˵����һ��256������Ӧ�ò�������ɣ�:)
				2.����ʹ��16�����С������,���Ҫʹ�ô�������Ըı�����ֵ
				3.��ͬ��������Сһ�ο�����Ⱦ����������Ҳ�ǲ�һ���ģ�64��С����������һ����Ⱦ������������16����
				128��С����64����256����256��,512��С��1024,1024��С��4096,����ԽС���ٶ�Խ�졣������˵��
				����16�����С�����壬�����24����Ļ�Ҫ��һЩ��


	bool DrawText( char* szText, int x, int y, D3DCOLOR color = 0xFFFFFFFF,float fScale = 1.0f );
	\param1		Ҫ��Ⱦ������
				��NOTE:����Ⱦ�����ֵĸ������ܴ��ڻ�����Ŀ��
	\param2		��Ļ�ϵ�X����
	\param3		��Ļ�ϵ�Y����
	\param4		���ֵ���ɫ
	\param5		���Զ����ִ�С��������
	return		�ɹ�����true;
 */
//===================================================================
//!���������fvf
#define D3DFVF_FONT (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define D3DFVF_3DFONT (D3DFVF_XYZB1 | D3DFVF_DIFFUSE | D3DFVF_TEX1)


#define MPFONT_BOLD        0x0001
#define MPFONT_ITALIC      0x0002
#define MPFONT_UNLINE      0x0004

#define  RECTNUM	60
#define  DRAWONCE	
class   MPRender;

class MINDPOWER_API CMPFont
{
public:
	CMPFont();
	~CMPFont();
	struct FONT_VER
	{
		D3DXVECTOR4 m_vPos;
		//float		m_fIdx;
		DWORD m_color;
		D3DXVECTOR2 m_fUV;
		void SetValue(const D3DXVECTOR4 &_vPos,DWORD _color,const D3DXVECTOR2 &_fUV)
		{
			const float offs = -0.5f;
			m_vPos = _vPos;
			m_vPos.x = floorf(m_vPos.x) + offs;
			m_vPos.y = floorf(m_vPos.y) + offs;
			m_color = _color;
			m_fUV = _fUV;
		}
	};

	struct MPFontRect
	{
		void	Reset()
		{
			pstr = "";
			bUsed = false;
			fscale = 0;
		}
		bool operator == (MPFontRect& sRect)
		{
			if(pstr != sRect.pstr)
				return false;
			if(dwColor != sRect.dwColor)
				return false;
			if(rcPos.left != sRect.rcPos.left || rcPos.top != sRect.rcPos.top)
				return false;

			return true;
		}
		bool IsEqual(char* pszText, int x,int y, DWORD color,float tfscale)
		{
			if(pstr != pszText || fscale != tfscale)
				return false;
			else if( rcPos.left  != x || rcPos.top != y )
			{
#ifdef DRAWONCE
				FONT_VER* pVer;
				int w =  x - rcPos.left;
				int h = y - rcPos.top;
				WORD n;
				if(iHslNum > 0)
				{
					int lh = iHslNum * 3;
					for ( n = 0; n < lh; ++n)
					{
						pVer = &_vecHsl[n];
						pVer->m_vPos.x += w;
						pVer->m_vPos.y += h;
						pVer->m_color = color;
					}
				}
				if(iAshNum > 0)
				{
					int la = iAshNum * 3;
					for ( n = 0; n < la; ++n)
					{
						pVer = &_vecAsh[n];
						pVer->m_vPos.x += w;
						pVer->m_vPos.y += h;
						pVer->m_color = color;
					}
				}
#else
				int w =  rcPos.left - x;
				int h = rcPos.top - y;
				WORD n;
				if(iHslNum > 0)
				{
					int lh = iHslNum * 4;
					for ( n = 0; n < lh; ++n)
					{
						_vecHsl[n].x -= w;
						_vecHsl[n].y += h;
					}
				}
				if(iAshNum > 0)
				{
					int la = iAshNum * 4;
					for ( n = 0; n < la; ++n)
					{
						_vecAsh[n].x -= w;
						_vecAsh[n].y += h;
					}
				}
#endif
				rcPos.left = x ;
				rcPos.top = y;
			}if(dwColor != color)
			{
				dwColor = color;
#ifdef DRAWONCE
				WORD n;
				if(iHslNum > 0)
				{
					int lh = iHslNum * 3;
					for ( n = 0; n < lh; ++n)
						_vecHsl[n].m_color = color;
				}
				if(iAshNum > 0)
				{
					int la = iAshNum * 3;
					for ( n = 0; n < la; ++n)
						_vecAsh[n].m_color = color;
				}
#endif
			}
			return true;
		}
		s_string	pstr;
		RECT rcPos;
		DWORD dwColor;
		float	fscale;
#ifdef DRAWONCE
		std::vector<FONT_VER>	_vecHsl;
		std::vector<FONT_VER>	_vecAsh;
#else
		std::vector<D3DXVECTOR4>	_vecHsl;
		std::vector<D3DXVECTOR4>	_vecAsh;
#endif

		int iAshNum;
		int iHslNum;
		bool bUsed;
	};
#ifdef		USE_RENDER
	bool CreateFont( MPRender* pd3dDevice, char* szFontName, int nSize = 16, int nLevel = 3 ,DWORD dwFlag = 0);
#else
	bool CreateFont( LPDIRECT3DDEVICE8 pd3dDevice, char* szFontName, int nSize = 16, int nLevel = 3 ,DWORD dwFlag = 0);
#endif
	void BindingRes(CMPResManger* pResMagr);

	void Begin();
	void Draw(char* szText, int x, int y, D3DXCOLOR color);
	void End();

	void BeginClip();
	void DrawTextClipOnce(char* szText, int nLen, LPRECT psrc, LPRECT pclip,D3DXCOLOR color);
	void EndClip();

	bool DrawText( char* szText, int x, int y, D3DXCOLOR color = 0xFFFFFFFF,float fScale = 1.0f ,DWORD* dwTime = NULL);
	bool DrawText( int iNumber, int x, int y, D3DXCOLOR color = 0xFFFFFFFF,float fScale = 1.0f );
	
	bool DrawTextShadow( char* szText, int x1, int y1, int x2, int y2,
		D3DXCOLOR color1,D3DXCOLOR color2);


	bool Draw3DText(char* szText,D3DXVECTOR3& vPos, D3DXCOLOR color = 0xFFFFFFFF,float fScale = 0.3f );

	SIZE* GetTextSize(const std::string& szText, SIZE* pSize,float fScale = 1.0f);
	int	  GetHzLength(float fscale = 1.0f);
	int   GetAscLength(float fscale = 1.0f);

	void ReleaseFont();

	void UseSoft(bool bUseSoft){_bUseSoft = bUseSoft;}
	bool IsUseSoft()		   {return _bUseSoft;}
#ifdef USE_RENDER
	lwITex*				GetTexture(){return _pTex;}
#else
	LPDIRECT3DTEXTURE8	GetTexture(){return _pTexFast;}
#endif

	void SetRenderIdx(int iIdx)	{ _iRenderIdx = iIdx; }

private:
	bool  TextToTexture( char c1, char c2, float & tX, float & tY );

	void FillTextToTex( char* szText );

	bool FindTextFromTex( char c1, char c2, float & tX1, float & tY1 , float & tX2, float & tY2);

	bool DrawTextClip( char* szText, int nLen,LPRECT psrc, LPRECT pclip,D3DXCOLOR color = 0xFFFFFFFF);

	int FindRect( char* szText, int x,int y, DWORD color,float tfscale)
	{
		if(_vecValidID.size() == RECTNUM)
			return -1;
		for (INT n = 0; n < RECTNUM; ++n)
		{
			if(_vecFontRect[n].bUsed)
				if(_vecFontRect[n].IsEqual(szText,x,y,color,tfscale))
					return n;
		}
		return -1;
	}
	int GetNextInValidRect(MPFontRect** pRect)
	{
		if(_vecValidID.empty())
		{
			ResetRect();
		}
		int rcdx = *_vecValidID.front();

		_vecValidID.pop_front();

		*pRect = &_vecFontRect[rcdx];
		(*pRect)->bUsed = true;
		return rcdx;
	}
	void ResetRect()
	{
		_vecValidID.setsize(RECTNUM);
		for ( INT n = 0; n < RECTNUM; ++n)
		{
			_vecFontRect[n].Reset();
			*_vecValidID[n] = n;
		}
	}
	void RenderOptimize(int iIdx);
	void RenderDrawOptimize(int iIdx);

private:
	HDC			_hDc;			
	HBITMAP		_hBmp;
	HFONT		_hFont;
	HGDIOBJ		_hBmpOld;
	HFONT		_hFontOld;

	UINT		_Max;
	int			_RowNum;
	DWORD *		_pBits;

	int			_TextSize;
	int			_TextureSize;

	bool		_bWidthEqual;

	std::vector<WORD> _vecBuf;	

#ifdef		USE_RENDER
	MPRender*				_pDev;
#else
	LPDIRECT3DDEVICE8		_pDev;
#endif


#ifndef USE_RENDER
	LPDIRECT3DVERTEXBUFFER8 _pVB;	

	LPDIRECT3DTEXTURE8		_pTex;
#else
	lwIMesh*				_pVB;
	lwITex*					_pTex;


#endif

	int						_iRenderIdx;
	CMPEffectFile*			_pCEffectFile;
	bool					_bUseSoft;
	

	D3DXMATRIX*				_pmatBBoard;
	D3DXMATRIX*				_pmatViewProj;
	D3DXMATRIX*				_pmat2DViewProj;
	int*					_iBufWidth;
	int*					_iBufHeight;

#ifndef USE_RENDER
	LPDIRECT3DTEXTURE8		_pTexFast;
#else
	lwITex*					_pTexFast;
#endif

	std::vector<WORD>		_vecBufFast;
	std::map<WORD, D3DXVECTOR4>	_mapUV;
	int			_RowNumFast;		
	UINT		_MaFast;
	int			_TextureSizeFast;

#ifndef USE_RENDER
	LPDIRECT3DTEXTURE8		_lpCurTex;
#else
	lwITex*					_lpCurTex;
#endif

	float					_fyu;

///////�Ż���Ⱦ///////////////////////////////////////////////////////////////////
	S_FVector<WORD>			_vecValidID;
	std::vector<MPFontRect>	_vecFontRect;
//////////////////////////////////////////////////////////////////////////
	FONT_VER*				_vecVer;
	FONT_VER*				_vecAsh;

};


