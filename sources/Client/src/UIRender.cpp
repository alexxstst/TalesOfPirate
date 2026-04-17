#include "StdAfx.h"
#include "script.h"
#include "UIRender.h"
//#include <typeinfo>
#include <cstdlib>
#include <Commdlg.h>

#include "UIEdit.h"
#include "MPTextureSet.h"
#include "MPFont.h"
#include "GameApp.h"
#include "UILabel.h"
#include "uiformmgr.h"
#include "uitextbutton.h"
#include "uilabel.h"
#include "uiprogressbar.h"
#include "uiscroll.h"
#include "uilist.h"
#include "uicombo.h"
#include "uiimage.h"
#include "UICheckBox.h"
#include "uiimeinput.h"
#include "uigrid.h"
#include "Character.h"
#include "uilistview.h"
#include "uipage.h"
#include "uitreeview.h"
#include "GameConfig.h"
#include "uitextparse.h"

#include "GlobalVar.h"
#include "FontManager.h"

CUIPanel::CUIPanel()
{
	m_pDev = NULL;
	_lpVB = NULL;
}

CUIPanel::~CUIPanel()
{
	SAFE_RELEASE(_lpVB);
}

bool	CUIPanel::Create(IDirect3DDeviceX*  pDev)
{
	m_pDev = pDev;

	_w = 1;
	_h = 1;

	UI_VERTEX Vertices[] = {
		{-(float)_w/2, -(float)_h/2,0.0f,1, D3DCOLOR_ARGB(255,255,255,255), 0.0f, 1.0f}, 
		{-(float)_w/2, (float)_h/2, 0.0f,1, D3DCOLOR_ARGB(255,255,255,255), 0.0f, 0.0f}, 
		{ (float)_w/2, (float)_h/2, 0.0f,1,D3DCOLOR_ARGB(255,255,255,255), 1.0f, 0.0f}, 
		{ (float)_w/2, -(float)_h/2,0.0f,1, D3DCOLOR_ARGB(255,255,255,255), 1.0f, 1.0f}, 
	};

	if(FAILED(m_pDev->CreateVertexBuffer(
		4*sizeof(UI_VERTEX),
		D3DUSAGE_WRITEONLY| D3DUSAGE_DYNAMIC,
		0,
		D3DPOOL_DEFAULT,
		&_lpVB, NULL )))
		return false;
	BYTE* lpVertex;
	if (FAILED(_lpVB->Lock(0, 0, (void**) & lpVertex, 0)))
		return false;
	memcpy(lpVertex, Vertices, sizeof(Vertices));
	_lpVB->Unlock();
	return true;
}

void	CUIPanel::Draw( IDirect3DTextureX*  pSrcTexture, 
					 CONST RECT* pSrcRect, CONST D3DXVECTOR2* pScaling, 
					 CONST D3DXVECTOR2* pRotationCenter, FLOAT Rotation, 
					 CONST D3DXVECTOR2* pTranslation, D3DCOLOR Color)
{
	m_pDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pDev->SetRenderState(D3DRS_ALPHAREF,0x08);
	m_pDev->SetRenderState(D3DRS_ALPHAFUNC,D3DCMP_GREATEREQUAL     );
	m_pDev->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
	m_pDev->SetRenderState(D3DRS_DITHERENABLE,TRUE);


	m_pDev->SetRenderState( D3DRS_FILLMODE,   D3DFILL_SOLID );
	m_pDev->SetRenderState( D3DRS_CULLMODE,   D3DCULL_CCW );
	m_pDev->SetRenderState( D3DRS_STENCILENABLE,    FALSE );
	m_pDev->SetRenderState( D3DRS_CLIPPING,         TRUE );
#if (defined LW_USE_DX8)
	m_pDev->SetRenderState( D3DRS_EDGEANTIALIAS,    FALSE );
#endif
	m_pDev->SetRenderState( D3DRS_CLIPPLANEENABLE,  FALSE );
	m_pDev->SetRenderState( D3DRS_VERTEXBLEND,      FALSE );
	m_pDev->SetRenderState( D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE );
	m_pDev->SetRenderState( D3DRS_FOGENABLE,        FALSE );

	//m_pDev->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR );
	//m_pDev->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_POINT );
	//m_pDev->SetTextureStageState( 0, D3DTSS_MIPFILTER, D3DTEXF_NONE );
	m_pDev->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0 );
	m_pDev->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE );


	m_pDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	m_pDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);


	m_pDev->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	m_pDev->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
	m_pDev->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
	m_pDev->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	m_pDev->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	m_pDev->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

	//m_pDev->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_DISABLE );
	//m_pDev->SetTextureStageState( 1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );

	m_pDev->SetRenderState(D3DRS_ZENABLE, FALSE);
	m_pDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	D3DXMATRIX matscale;

	D3DSURFACE_DESC		BufParam;
	pSrcTexture->GetLevelDesc(0,&BufParam);

	_w = pSrcRect->right - pSrcRect->left;
	_h = pSrcRect->bottom - pSrcRect->top;

	_x = (int)pTranslation->x;
	_y = (int)pTranslation->y;

	UI_VERTEX* lpVertex;
	if (FAILED(_lpVB->Lock(0, 0, (void**)&lpVertex, D3DLOCK_DISCARD)))
		return;
	lpVertex[0].x = (float)_x;//_x -(float)_w/2;// -(float)_w/2;
	lpVertex[0].y = (float)_y;//_y -(float)_h/2;// -(float)_h/2;
	lpVertex[0].z = 0;
	lpVertex[0].tu =  (float)pSrcRect->left / (float)BufParam.Width;
	lpVertex[0].tv =  (float)pSrcRect->top / (float)BufParam.Height;
	lpVertex[0].diffuse = Color ;

	lpVertex[1].x = (float)(_x + _w  * pScaling->x);//_x -(float)_w/2;//-(float)_w/2;
	lpVertex[1].y = (float)_y ;//_y + _h - (float)_h/2;//(float)_h/2;
	lpVertex[1].z = 0;
	lpVertex[1].tu =  (float)pSrcRect->right / (float)BufParam.Width;
	lpVertex[1].tv =  (float)pSrcRect->top / (float)BufParam.Height;
	lpVertex[1].diffuse = Color ;

	lpVertex[2].x =  (float)(_x + _w  * pScaling->x);//_x + _w - (float)_w/2;//(float)_w/2;
	lpVertex[2].y = (float)(_y + _h  * pScaling->y);//_y + _h -(float)_h/2;//(float)_h/2;
	lpVertex[2].z = 0;
	lpVertex[2].tu =  (float)pSrcRect->right / (float)BufParam.Width;
	lpVertex[2].tv =  (float)pSrcRect->bottom / (float)BufParam.Height;
	lpVertex[2].diffuse = Color ;

	lpVertex[3].x = (float)_x;//_x + _w - (float)_w/2;//(float)_w/2;
	lpVertex[3].y = (float)(_y + _h * pScaling->y);//_y -(float)_h/2;//-(float)_h/2;
	lpVertex[3].z = 0;
	lpVertex[3].tu =  (float)pSrcRect->left / (float)BufParam.Width;
	lpVertex[3].tv =  (float)pSrcRect->bottom / (float)BufParam.Height;
	lpVertex[3].diffuse = Color ;

 	_lpVB->Unlock();

	m_pDev->SetTexture(0,pSrcTexture);
	//m_pDev->SetTexture(1,NULL);

	m_pDev->SetVertexShader(NULL);
	m_pDev->SetFVF(UI_FVF);
	m_pDev->SetStreamSource(0, _lpVB, 0, sizeof(UI_VERTEX));
	m_pDev->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);
}
void	CUIPanel::End()
{
	m_pDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pDev->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pDev->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
	m_pDev->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
	m_pDev->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_NONE );

	//g_Render.SetCurrentView(MPRender::VIEW_WORLD);
}

bool UIGetChooseColor( DWORD &c )
{
	COLORREF cf = RGB( (c & 0x00ff0000) >> 16, (c & 0x0000ff00) >> 8, c & 0x000000ff );
	CHOOSECOLOR cc;
	memset( &cc, 0, sizeof(cc) );
	cc.lStructSize = sizeof(CHOOSECOLOR);
	cc.lpCustColors = &cf;
	cc.rgbResult = cf;
	cc.hwndOwner = CGuiData::GetHWND();
	cc.Flags = CC_FULLOPEN | CC_RGBINIT | CC_SOLIDCOLOR;

	bool rt = ChooseColor( &cc )!=0 ? true: false;
	if( rt )
	{
		cf = cc.rgbResult;
		c = ( c & 0xff000000 ) |  (GetRValue( cf ) << 16) | (GetGValue( cf ) << 8) | GetBValue( cf );
	}
	return rt;
}

char* UIGetOpenFileName( char* strInitDir )
{
	char szFile [MAX_PATH] = "\0"; 
	char szFileTitle [MAX_PATH] = "\0";
	char szFilter [] = {"All Files\0*.*\0"};

	OPENFILENAME of;
	memset( &of, 0, sizeof(of) );
	of.lStructSize = sizeof(of);
	of.lpstrFile = szFile;
	of.nMaxFile = sizeof(szFile);
	of.lpstrFileTitle = szFileTitle;
	of.nMaxFileTitle = sizeof (szFileTitle);
	of.lpstrCustomFilter = (LPTSTR)NULL;
	of.nMaxCustFilter = 0L;
	of.lpstrFilter = szFilter;
	of.nFilterIndex = 1L;
	of.lpstrTitle = GetLanguageString(745).c_str();
	of.nFileOffset = 0;
	of.nFileExtension = 0;
	of.lpstrDefExt = "*.*";
	of.lCustData = 0;
	of.lpstrInitialDir = strInitDir;

	of.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	if( GetOpenFileName( &of ) )
		return of.lpstrFile;
	return NULL;
}

//---------------------------------------------------------------------------
// class CDrag
//---------------------------------------------------------------------------
DWORD CDrag::_dwMouseUpTime= 0;
int CDrag::BeginMouseRun( CGuiData* gui, bool InRect, int x, int y, DWORD key )
{
    if( InRect )
    {
        switch(_eState)
        {
        case stNone:
            if( key & Mouse_LDown )
            {
				_dwMouseUpTime = 0;
                _eState = stPress;

                _nStartX = x;
                _nStartY = y;

                _nDragX = 0;
                _nDragY = 0;

                if( _nYareLen<=0 )
                {
                    _eState = stDrag;
                    _pParent = gui;
                    _pDrag = this;
					CGuiData::SetCursor( _crDrag );

                    if( evtMouseDragBegin )  evtMouseDragBegin( _pParent, x, y, key );
                    return stDrag;
                }
                return stPress;
            }

			CGuiData::SetCursor( _crDragIn );
            break;
        case stPress:
            if( !g_pGameApp->IsMouseButtonPress(0) )
            {
                Reset();
                return stNone;
            }

            if( key & Mouse_Move )
            {
                _nDragX = x - _nStartX;
                _nDragY = y - _nStartY;

                // Begin drag
                if( (abs(_nStartX - x)>=(int)_nYareLen || abs(_nStartY - y)>=(int)_nYareLen) )
                {
                    _eState = stDrag;
                    _pParent = gui;
                    _pDrag = this;
					CGuiData::SetCursor( _crDrag );

                    if( evtMouseDragBegin )  evtMouseDragBegin( _pParent, x, y, key );
                    return stDrag;
                }
                return stPress;
            }
            break;
        }
    }
    return stNone;
}

void CDrag::Reset()				
{ 
	_eState=stNone; 
	_pDrag=NULL; 
	_pParent=NULL;	

	CGuiData::SetCursor( _crNormal );
}

bool CDrag::MouseRun( int x, int y, DWORD key )
{
    // Drag ended
    if( key & Mouse_LUp )
    {
        if( _IsUseGrid )
        {
            _nDragX = ( _pParent->GetX() + GetDragX() + _dwGridWidth / 2 ) / _dwGridWidth * _dwGridWidth - _pParent->GetX();
            _nDragY = ( _pParent->GetY() + GetDragY() + _dwGridHeight / 2 ) / _dwGridHeight * _dwGridHeight - _pParent->GetY();
        }

        if( _IsMove )
        {
            _pParent->SetPos( _pParent->GetLeft() + GetDragX(), _pParent->GetTop() + GetDragY() );
            _pParent->Refresh();
        }	

        if( evtMouseDragEnd )  evtMouseDragEnd( _pParent, x, y, key );
        Reset();
        return true;
    }

	if( _dwMouseUpTime==0  )
    {
		if( !g_pGameApp->IsMouseButtonPress(0) )
		{
			_dwMouseUpTime = CGameApp::GetCurTick();
		}
    }
	else if( _dwMouseUpTime<CGameApp::GetCurTick() )
	{
        Reset();
		return true;
	}

    if( key & Mouse_Move )
    {
		CGuiData::SetCursor( _crDrag );

        _nDragX = x - _nStartX;
        _nDragY = y - _nStartY;
        if( evtMouseDragMove )  evtMouseDragMove( _pParent, x, y, key );
        return true;
    }
    return false;
}

//---------------------------------------------------------------------------
// class CGuiData
//---------------------------------------------------------------------------
HWND CGuiData::GetHWND()
{
	return g_pGameApp->GetHWND();
}

void CGuiData::LineFrame()
{
	GetRender().LineFrame(GetX(),GetY(), GetX2(), GetY2(), 0xffffffff );
}

void CGuiData::SelectChaLineFrame()
{
	GetRender().SelectChaLineFrame(GetX(),GetY(), GetX2(), GetY2(), 0xffffffff );
}

void CGuiData::LineThinFrame()
{
	GetRender().LineThinFrame(GetX(),GetY(), GetX2(), GetY2(), 0xffffffff );
}

void CGuiData::FillFrame()
{
	GetRender().FillFrame(GetX(),GetY(), GetX2(), GetY2(), 0xffffffff );
}

//---------------------------------------------------------------------------
// class UIRender
//---------------------------------------------------------------------------
UIRender UIRender::s_Render;

UIRender::UIRender()
: _nGuiWidth(LARGE_RES_X), _nGuiHeight(768), _fScreenScaleX(1.0), _fScreenScaleY(1.0)
, _fDrawScaleX(1.0), _fDrawScaleY(1.0), _p2DSprite(NULL), _nTex(-1)
{
	_rtFrame.left = 0;
	_rtFrame.top = 0;
	_rtFrame.right = 1;
	_rtFrame.bottom = 1;

	_vesLevel.y = 2;
	_vesVertical.x = 2;

	_vesLevelThin.y = 1;
	_vesVerticalThin.x = 1;

	_fOldScreenScaleX = _fScreenScaleX;
	_fOldScreenScaleY = _fScreenScaleY;
	_fOldDrawScaleX = _fDrawScaleX; 
	_fOldDrawScaleY = _fDrawScaleY;
}

void UIRender::SetClipRect( int x, int y, int w, int h )
{
	ui::UIClip::Instance()->SetClipRect( x, y, w, h );
}

void UIRender::Reset()
{
	ui::UIClip::Instance()->Reset();
}

HRESULT  UI_OnLostDevice()
{
#ifdef USE_RENDER
    GetRender().OnLostDevice();
#endif
    return TRUE; 
}
HRESULT  UI_OnResetDevice()
{
#ifdef USE_RENDER
    GetRender().OnResetDevice();
#endif
    return TRUE; 
}

void	UIRender::OnLostDevice()
{
    if(FAILED(_p2DSprite->OnLostDevice()))
        ToLogService("errors", LogLevel::Error, "msglost");
}

void	UIRender::OnResetDevice()
{
    if(FAILED(_p2DSprite->OnResetDevice()))
        ToLogService("errors", LogLevel::Error, "msgreset");
}

void	UIRender::RegisterFunc()
{
    MPRegisterOutputLoseDeviceProc(UI_OnLostDevice);
    MPRegisterOutputResetDeviceProc(UI_OnResetDevice);
}

bool UIRender::Init()
{
	//_p2DSprite->
    D3DXCreateSprite( g_Render.GetDevice(), &_p2DSprite );
    
	_nTex = GetTextureID( "texture/ui/frame.tga" );
	_nOutLine = GetTextureID("texture/ui/outline.tga");

    return true;
}

void UIRender::RenderTextureRect(int nX, int nY, MPTexRect *pRect)
{
	LPTEXTURE pTexture = GetTextureByID(pRect->nTextureNo);
	if(!pTexture) return;
	
	DrawConvert( nX , nY );

	VECTOR2 vecDest((float)nX, (float)nY);
	VECTOR2 vecScale( DrawConvertX( pRect->fScaleX ), DrawConvertY( pRect->fScaleY ) );	

//	VECTOR2 vecScale( DrawConvertX( pRect->fScaleX ), DrawConvertY( pRect->fScaleY ) );	

	RECT TexRect = { pRect->nTexSX, pRect->nTexSY, pRect->nTexSX + pRect->nTexW, pRect->nTexSY + pRect->nTexH };

	RenderSprite(pTexture, &TexRect, &vecScale, &vecDest, pRect->dwColor);
}

void UIRender::RenderTextureAbsRect(int nX, int nY, MPTexRect *pRect)
{
	LPTEXTURE pTexture = GetTextureByID(pRect->nTextureNo);
	if(!pTexture) return;

	DrawConvert( nX, nY );
	VECTOR2 vecDest((float)nX, (float)nY);
	VECTOR2 vecScale( DrawConvertX( pRect->fScaleX ), DrawConvertY( pRect->fScaleY ) );	

	RECT TexRect = { pRect->nTexSX, pRect->nTexSY, pRect->nTexSX + pRect->nTexW, pRect->nTexSY + pRect->nTexH };

	RenderSprite(pTexture, &TexRect, &vecScale, &vecDest, pRect->dwColor);
}

void UIRender::SetIsConvert( bool v )
{
	if( v )
	{
		_fScreenScaleX = _fOldScreenScaleX;
		_fScreenScaleY = _fOldScreenScaleY;
		_fDrawScaleX = _fOldDrawScaleX; 
		_fDrawScaleY = _fOldDrawScaleY;
	}
	else
	{
		_fScreenScaleX = 1.0f;
		_fScreenScaleY = 1.0f;
		_fDrawScaleX = 1.0f; 
		_fDrawScaleY = 1.0f;
	}
}

void UIRender::RenderTextureRect(int nX, int nY, MPTexRect *pRect, DWORD dwColor)
{
	LPTEXTURE pTexture = GetTextureByID(pRect->nTextureNo);
	if(!pTexture) return;

	DrawConvert( nX, nY );
	VECTOR2 vecDest((float)nX, (float)nY);
	VECTOR2 vecScale( DrawConvertX( pRect->fScaleX ), DrawConvertY( pRect->fScaleY ) );

	RECT TexRect = { pRect->nTexSX, pRect->nTexSY, pRect->nTexSX + pRect->nTexW, pRect->nTexSY + pRect->nTexH };

	//_p2DSprite->Draw(pTexture, &TexRect, &vecScale, NULL, 0, &vecDest, dwColor);
	RenderSprite(pTexture, &TexRect, &vecScale, &vecDest, dwColor);
}

void UIRender::LineThinFrame(int left, int top, int right, int bottom, DWORD color )
{
	DrawConvert( left, top );
	DrawConvert( right, bottom );

	static VECTOR2 pos;
	pos.x = (float)left;
	pos.y = (float)top;
	_vesLevelThin.x = (float)(right - left);
	_vesVerticalThin.y = (float)(bottom - top);

    LPTEXTURE pTexture = GetTextureByID(_nTex);
    if(!pTexture) return;

	RenderSprite(pTexture, &_rtFrame, &_vesLevelThin, &pos, color);

	RenderSprite(pTexture, &_rtFrame, &_vesVerticalThin,  &pos, color);

	pos.y = (float)bottom;
	RenderSprite(pTexture, &_rtFrame, &_vesLevelThin,&pos, color);

	pos.x = (float)(right-1);
	pos.y = (float)(top+1);
	RenderSprite(pTexture, &_rtFrame, &_vesVerticalThin, &pos, color);

}

void UIRender::LineFrame(int left, int top, int right, int bottom, DWORD color )
{
	DrawConvert( left, top );
	DrawConvert( right, bottom );
    
	
    LPTEXTURE pTexture = GetTextureByID(_nTex);
    if(!pTexture) return;

	static VECTOR2 pos;
	pos.x = (float)left;
	pos.y = (float)top;
	_vesLevel.x = (float)(right - left);
	_vesVertical.y = (float)(bottom - top);
	RenderSprite(pTexture, &_rtFrame, &_vesLevel,&pos, color);

	RenderSprite(pTexture, &_rtFrame, &_vesVertical,&pos, color);

	pos.y = (float)bottom;
	RenderSprite(pTexture, &_rtFrame, &_vesLevel, &pos, color);

	pos.x = (float)(right - 2);
	pos.y = (float)(top + 2);
	RenderSprite(pTexture, &_rtFrame, &_vesVertical, &pos, color);
}



void UIRender::SelectChaLineFrame(int left, int top, int right, int bottom, DWORD color )
{
	DrawConvert( left, top );
	DrawConvert( right, bottom );
    
	
    LPTEXTURE pTexture = GetTextureByID(_nOutLine);
    if(!pTexture) return;

	static VECTOR2 pos;
	pos.x = (float)left;
	pos.y = (float)top;
	_vesLevel.x = (float)(right - left);
	_vesVertical.y = (float)(bottom - top);
	RenderSprite(pTexture, &_rtFrame, &_vesLevel,&pos, color);

	RenderSprite(pTexture, &_rtFrame, &_vesVertical,&pos, color);

	pos.y = (float)bottom;
	RenderSprite(pTexture, &_rtFrame, &_vesLevel, &pos, color);

	pos.x = (float)(right - 2);
	pos.y = (float)(top + 2);
	RenderSprite(pTexture, &_rtFrame, &_vesVertical, &pos, color);
}


void UIRender::FillFrame(int left, int top, int right, int bottom, DWORD color )
{
	DrawConvert( left, top );
	DrawConvert( right, bottom );

	static VECTOR2 pos;
	pos.x = (float)left;
	pos.y = (float)top;

	static VECTOR2 vesScale;
	vesScale.x =  (float)(right - left);
	vesScale.y =  (float)(bottom - top);

    LPTEXTURE pTexture = GetTextureByID(_nTex);
    if(!pTexture) return;

	RenderSprite(pTexture, &_rtFrame, &vesScale,&pos, color);

}

void UIRender::SetScreen( int w, int h, bool isFull )
{
	_IsFullScreen = isFull;

    int sw, sh;     // Actual display dimensions
    RECT rc;
    ::GetClientRect( g_pGameApp->GetHWND(), &rc );
    sw = rc.right  - rc.left;	
    sh = rc.bottom - rc.top;

    _nScrWidth = sw;
    _nScrHeight = sh;

	_fDrawScaleX = (float)((float)w / 1024.0);
	_fDrawScaleY = (float)((float)h / 768.0);

	if( !isFull )
	{
		_fScreenScaleX = _fDrawScaleX;
		_fScreenScaleY = (float)( _fDrawScaleY - (float)(h - sh)/(float)(h) );
	}
	else
	{
		_fScreenScaleX = _fDrawScaleX;
		_fScreenScaleY = _fDrawScaleY;
	}

    _fScreenScaleY = 1.0f - ( _fDrawScaleY - _fScreenScaleY );
    _fScreenScaleX = 1.0f;
    _fDrawScaleX = 1.0f;
    _fDrawScaleY = 1.0f;

    ToLogService("common", "Screen:{}, {} Draw:{}, {} ScreenSize: {}, {}  ClientSize: {}, {}  IsFull: {}",
        _fScreenScaleX, _fScreenScaleY, _fDrawScaleX, _fDrawScaleY, w, h, sw, sh, isFull);

	_fOldScreenScaleX = _fScreenScaleX;
	_fOldScreenScaleY = _fScreenScaleY;
	_fOldDrawScaleX = _fDrawScaleX; 
	_fOldDrawScaleY = _fDrawScaleY;

	CGuiFont::s_Font.SetScreen( w, h );
	CGuiData::SetScreen( _fDrawScaleX, _fDrawScaleY, _fScreenScaleX, _fScreenScaleY );
	CImeInput::s_Ime.SetScreen( isFull, _nGuiWidth, _nGuiHeight );


    CFormMgr::s_Mgr.SetScreen();
}

//---------------------------------------------------------------------------
// class CGuiPic
//---------------------------------------------------------------------------
bool CGuiPic::LoadImage( MPTexRect* pTex, const char* file, int w, int h, int tx, int ty, float scale_x, float scale_y )
{
	if( !pTex ) return false;

	pTex->nTexSX = tx;
	pTex->nTexSY = ty;
	pTex->nTexW	= w;
	pTex->nTexH	= h;
	pTex->fScaleX = scale_x;
	pTex->fScaleY = scale_y;
	pTex->nTextureNo = GetTextureID( file );
	return pTex->nTextureNo!=-1;
}

bool CGuiPic::LoadAllImage( const char * file, int w, int h, int tx, int ty )
{
	int id = GetTextureID( file );
	for( unsigned int i=0; i<_max; i++ )
	{
		LoadImage( i, id, tx, ty, w, h );
		tx += w;
	}
	return id!=-1;
}

bool CGuiPic::LoadImage( const char * file, int w, int h, int frame, int tx, int ty, float scalex, float scaley )
{
	int id = GetTextureID( file );
	if( id == -1 ) return false;

	LoadImage( frame, id, tx, ty, w, h, scalex, scaley );
	return true;
}

bool CGuiPic::LoadImage( const char * file )
{
	int id = GetTextureID( file );
	if( id == -1 ) return false;
	
	SIZE sz;
	sz = GetTextureSizeByID( id );
	LoadImage( 0, id, 0, 0, sz.cx, sz.cy );
	return true;
}

bool CGuiPic::LoadImage( int frame, int nTextureID, int tx, int ty, int tw, int th, float scale_x, float scale_y )
{
	MPTexRect* pTex = GetImage( frame );
	if( !pTex ) return false;

	if( tw == 0 || th == 0 ) 
	{
		pTex->nTextureNo = -1;
		_bIsScale = false;
		return false;
	}

	pTex->nTexSX	= tx;
	pTex->nTexSY	= ty;
	pTex->nTexW	= tw;
	pTex->nTexH	= th;
	if( scale_x<=0.000001f ) // Scale texture to fit parent dimensions
	{
		if( _pParent )
		{
			_bIsScale = true;
			pTex->fScaleX  = (float)_pParent->GetWidth() / (float)pTex->nTexW;
			pTex->fScaleY  = (float)_pParent->GetHeight() / (float)pTex->nTexH;
		}
		else
		{
			_bIsScale = false;
			pTex->fScaleX	= 1.0;
			pTex->fScaleY	= 1.0;
		}
	}
	else
	{
		_bIsScale = false;
		pTex->fScaleX	= scale_x;
		pTex->fScaleY	= scale_y;
	}
	pTex->nTextureNo = nTextureID;

	if( nTextureID==-1 )
	{
		_bIsScale = false;
	}
	return true;
}

void CGuiPic::Refresh()
{
	if( _bIsScale )
	{
		for( unsigned int i=0; i<_max; i++ )
		{
			if( _pImage[i].nTextureNo==-1 ) continue;
			_pImage[i].fScaleX  = (float)_pParent->GetWidth() / (float)_pImage[i].nTexW;
			_pImage[i].fScaleY  = (float)_pParent->GetHeight() / (float)_pImage[i].nTexH;
		}
	}
}

void CGuiPic::RenderAll( int x, int y )
{
	for( unsigned int i=0; i<_max; i++ )
	{
		GetRender().RenderTextureRect( x, y, &_pImage[i] );
	}
}

void CGuiPic::RenderAll( int x, int y ,BYTE bAlpha )
{

	for( unsigned int i=0; i<_max; i++ )
	{ 
		GetRender().RenderTextureRect( x, y, &_pImage[i], ( bAlpha << 24 ) | 0x00FFFFFF );
    }
}

void CGuiPic::RenderAll( int x, int y ,DWORD dwColor )
{

	for( unsigned int i=0; i<_max; i++ )
	{ 
		GetRender().RenderTextureRect( x, y, &_pImage[i], dwColor );
    }
}

//---------------------------------------------------------------------------
// class CFramePic
//---------------------------------------------------------------------------
void CFramePic::Refresh()
{
	int nUnitW = _pImage[ppLeft].nTexW;
	int nUnitH = _pImage[ppLeft].nTexH;

	float fScaleW = 1.0f;
	if( nUnitW>0 && _pOwn->GetWidth() > nUnitW ) fScaleW = (float)(_pOwn->GetWidth() - nUnitW) / (float)nUnitW;
	float fScaleH = 1.0f;
	if( nUnitH>0 && _pOwn->GetHeight() > nUnitH ) fScaleH = (float)(_pOwn->GetHeight() - nUnitH) / (float)nUnitH;

	int nLeftX = nUnitW / 2;
	int nTopY = nUnitH / 2;

	if( _bIsShowFrame )
	{
		_nX[ppLeftUp] = _pOwn->GetX();
		_nY[ppLeftUp] = _pOwn->GetY();

		_nX[ppUp] = _pOwn->GetX() + nLeftX;
		_nY[ppUp] = _pOwn->GetY();
		_pImage[ppUp].fScaleX = fScaleW;

		_nX[ppRightUp] = _pOwn->GetX2() - nUnitW;
		_nY[ppRightUp] = _pOwn->GetY();

		_nX[ppRight] = _pOwn->GetX2() - nUnitW;
		_nY[ppRight] = _pOwn->GetY() + nTopY;
		_pImage[ppRight].fScaleY = fScaleH;

		_nX[ppRightBottom] = _pOwn->GetX2() - nUnitW;
		_nY[ppRightBottom] = _pOwn->GetY2() - nUnitH;

		_nX[ppBottom] = _pOwn->GetX() + nLeftX;
		_nY[ppBottom] = _pOwn->GetY2() - nUnitH;
		_pImage[ppBottom].fScaleX = fScaleW;

		_nX[ppLeftBottom] = _pOwn->GetX();
		_nY[ppLeftBottom] = _pOwn->GetY2() - nUnitH;

		_nX[ppLeft] = _pOwn->GetX();
		_nY[ppLeft] = _pOwn->GetY() + nTopY;
		_pImage[ppLeft].fScaleY = fScaleH;
	}

	if( !_bIsTitle )
	{
		_nX[ppClient] = _pOwn->GetX() + nLeftX;
		_nY[ppClient] = _pOwn->GetY() + nTopY;

		_pImage[ppClient].fScaleX  = fScaleW;
		_pImage[ppClient].fScaleY  = fScaleH;
	}
	else
	{
		_nX[ppClient] = _pOwn->GetX();
		_nY[ppClient] = _pOwn->GetY();
	}
}

void CFramePic::Render( int x, int y, BYTE alpha )
{
	DWORD color = ( alpha << 24 ) | 0x00FFFFFF;
	_ClientShow( x, y, color );

	if( _bIsShowFrame )
	{
		for( unsigned int i=0; i<ppClient; i++ )
		{
			GetRender().RenderTextureRect( _nX[i], _nY[i], &_pImage[i], color );
		}
	}
}

void CFramePic::_ClientShow( int x, int y, DWORD color)
{
	if( _bIsTitle )
	{
		MPTexRect* p = &_pImage[ppClient];
		int nLeft = x;
		int nTop = y;
		int nRight = x + _pOwn->GetWidth();
		int nBottom = y + _pOwn->GetHeight();

		int nWidth = (int)(p->nTexW * p->fScaleX);
		int nHeight = (int)(p->nTexH * p->fScaleY);
		if( nWidth <= 0 || nHeight <= 0 ) return;

		while( nTop < nBottom )
		{
			GetRender().RenderTextureRect( nLeft, nTop, p, color );
			nLeft += nWidth;
			if( nLeft >= nRight )
			{
				nTop += nHeight;
				nLeft = x;
			}
		}
	}
	else
	{
		GetRender().RenderTextureRect( x, y, &_pImage[ppClient], color );
	}
}

void CFramePic::Render()
{
	_ClientShow( _nX[ppClient], _nY[ppClient], _pImage[ppClient].dwColor );

	if( _bIsShowFrame )
	{
		for( unsigned int i=0; i<ppClient; i++ )
		{
			GetRender().RenderTextureRect( _nX[i], _nY[i], &_pImage[i] );
		}
	}
}

void CFramePic::Render( BYTE bAplha )
{
	DWORD color = ( bAplha << 24 ) | 0x00FFFFFF;
	_ClientShow( _nX[ppClient], _nY[ppClient], color );

	if( _bIsShowFrame )
	{
		for( unsigned int i=0; i<ppClient; i++ )
		{
			GetRender().RenderTextureRect( _nX[i], _nY[i], &_pImage[i], color );
		}
	}
}

bool CFramePic::LoadImage( ePicPos p, int nTextureID, int tx, int ty, int tw, int th )
{
	if( p<0 || p>=ppEnd ) return false;

	MPTexRect* pTex = &_pImage[p];
	pTex->nTexSX	= tx;
	pTex->nTexSY	= ty;
	pTex->nTexW	= tw;
	pTex->nTexH	= th;
	pTex->nTextureNo = nTextureID;

	pTex->fScaleX  = 1.0;
	pTex->fScaleY  = 1.0;
	return true;
}

bool CFramePic::LoadImage( const char* client, int cw, int ch, int tx, int ty, const char * file, int w, int h )
{
	int id = GetTextureID( client );
	if( id!=-1 ) 
	{
		LoadImage( ppClient, id, tx, ty, cw, ch );
	}

	if( !_bIsShowFrame ) return true;

	id = GetTextureID( file );
	if( id == -1 ) return false;

	int w2 = w + w, h2 = h + h;
	LoadImage( ppLeft, id, w2, h, w, h );
	LoadImage( ppLeftUp, id, 0, 0, w, h );
	LoadImage( ppUp, id, w, h, w, h );
	LoadImage( ppRightUp, id, 0, h, w, h );
	LoadImage( ppRight, id, w, h2, w, h );
	LoadImage( ppRightBottom, id, w2, 0, w, h );
	LoadImage( ppBottom, id, 0, h2, w, h );
	LoadImage( ppLeftBottom, id, w, 0, w, h );
	return true;
}

//---------------------------------------------------------------------------
// class CGuiFont
//---------------------------------------------------------------------------
CGuiFont::CGuiFont()
: _pFont(NULL)
{
}

CGuiFont::~CGuiFont()
{
	Clear();
}

bool CGuiFont::Clear()
{
	for( fonts::iterator it=_fonts.begin(); it!=_fonts.end(); ++it ) 
	{
		(*it)->ReleaseFont();
		//delete (*it);
		SAFE_DELETE(*it); // UI font cleanup
	}

	_fonts.clear();

	_pFont		= NULL;
	_nMaxFont	= 0;
	return true;
}

void CGuiFont::SetScreen( int nScrWidth, int nScrHeight )
{
	Clear();

	CMPFont* p = NULL;
	if( nScrWidth==TINY_RES_X )
	{
		for( stfont::iterator it=_stfonts.begin(); it!=_stfonts.end(); ++it )
		{
			p =  new CMPFont;
#ifdef USE_RENDER
			if(!p->CreateFont(&g_Render, (char*)it->strFont.c_str(), it->size800, FONTLEVEN, it->dwStyle ))
				ToLogService("errors", LogLevel::Error, "msgCreateFont");
#else
			p->CreateFont(g_Render.GetDevice(), (char*)it->strFont.c_str(), it->size800, FONTLEVEN, it->dwStyle );
#endif

			p->BindingRes( &ResMgr );

			_fonts.push_back(p);
			_nMaxFont++;
		}		
	}
	else
	{
		for( stfont::iterator it=_stfonts.begin(); it!=_stfonts.end(); ++it )
		{
			p =  new CMPFont;
#ifdef USE_RENDER
			if(!p->CreateFont(&g_Render, (char*)it->strFont.c_str(), it->size1024, FONTLEVEN, it->dwStyle ))
				ToLogService("errors", LogLevel::Error, "msgCreateFont2");

#else
			p->CreateFont(g_Render.GetDevice(), (char*)it->strFont.c_str(), it->size1024, FONTLEVEN, it->dwStyle );
#endif

			p->BindingRes( &ResMgr );

			_fonts.push_back(p);
			_nMaxFont++;
		}

	}

    _pFont = _fonts[0];
}
//---------------------------------------------------------------------------
// class CTextButton
//---------------------------------------------------------------------------
bool CTextButton::LoadImage( const char* file, int w, int h, int tx, int ty, bool isHorizontal )
{
	if( !_pImage ) return false;

	int id = GetTextureID( file );
	if( id == -1 ) return false;

	if( isHorizontal )
	{
		_pImage->LoadImage( csNormal, id, tx, ty, w, h );
		_pImage->LoadImage( csHover, id, tx+w, ty, w, h );
		_pImage->LoadImage( csDown, id, tx+w*2, ty, w, h );
		_pImage->LoadImage( csDisable,id, tx+w*3, ty, w, h );
	}
	else
	{
		_pImage->LoadImage( csNormal, id, tx, ty, w, h );
		_pImage->LoadImage( csHover, id, tx, ty+h, w, h );
		_pImage->LoadImage( csDown, id, tx, ty+h*2, w, h );
		_pImage->LoadImage( csDisable, id, tx, ty+h*3, w, h );
	}
	return true;
}

//---------------------------------------------------------------------------
// class CScroll
//---------------------------------------------------------------------------
bool CScroll::LoadImage( const char* bkg, const char* up, const char* scroll, const char* down, int w, int h )
{
	if( btHorizontal == _eStyle )
	{
		_up->SetSize( GetHeight(), GetHeight() );
		_down->SetSize( GetHeight(), GetHeight() );
		_pScroll->SetSize( GetHeight(), GetHeight() );

		_up->LoadImage( up, h, h );
		_down->LoadImage( down, h, h );
		_pScroll->GetImage()->LoadImage( scroll, h, h );
	}
	else
	{
		_up->SetSize( GetWidth(), GetWidth() );
		_down->SetSize( GetWidth(), GetWidth() );
		_pScroll->SetSize( GetWidth(), GetWidth() );

		_up->LoadImage( up, w, w );
		_down->LoadImage( down, w, w );
		_pScroll->GetImage()->LoadImage( scroll, w, w );
	}

	if( !_pImage ) return false;

	int id = GetTextureID( bkg );
	if( id == -1 ) return false;

	_pImage->LoadImage( 0, id, 0, 0, w, h );

	return false;
}

//---------------------------------------------------------------------------
// class CEdit
//---------------------------------------------------------------------------
bool CEdit::InitCursor( const char* szFile ) 
{
	MPTexRect* pTex = &_CursorImage;

	int nTextureNo = GetTextureID( szFile );
	auto* pInfo = GetTextureInfo(nTextureNo);
	if( pInfo )
	{
		pTex->nTextureNo = nTextureNo;
 		pTex->nTexSX	= 0;
		pTex->nTexSY	= 0;
		//pTex->nTexW	= pInfo->btWidth;
		//pTex->nTexH	= pInfo->btHeight;

		pTex->dwColor = 0xffffffff;
		pTex->nTexW	=  2;
		pTex->nTexH	=  10;
		pTex->fScaleX	= 1.0;
		pTex->fScaleY	= 1.0;
		return true;
	}
	return false;
}

void CEdit::ShowFocus()
{
	if( !IsNormal() ) return;

	RefreshText();   // Refresh text display
	RefreshCursor(); // Refresh cursor display

	// Show blinking cursor
	++_nCursorFlashCount;
	if( _nCursorFlashCount>=10 )
	{
		_nCursorFlashCount=0;
		_bCursorIsShow = !_bCursorIsShow;
	}
	if( _bCursorIsShow )
	{
		// Use rectangle to accurately set cursor position
		//GetRender().RenderTextureAbsRect( _nCursorX, _nCursorY, &_CursorImage );
		GetRender().FillFrame( _nCursorX, _nCursorY, _nCursorX+2, _nCursorY+_nCursorHeight, _nCursorColor );
	}
}
//---------------------------------------------------------------------------
// class CProgressBar
//---------------------------------------------------------------------------
void CProgressBar::Init()
{
	CCompent::Init();

	_pTex = _pImage->GetImage( pcProgressbar );
    if( _style==btHorizontal ) 
	{
		_step.fLen = (float)_pTex->nTexW;
    }
    else 
	{
		_nStart = _pTex->nTexSY;
		_step.fLen = (float)_pTex->nTexH;
    }
	_pImage->Refresh();
}

bool CProgressBar::LoadImage( const char* file, int w, int h, bool isHorizontal )
{
	if( !_pImage ) return false;

	int id = GetTextureID( file );
	if( id == -1 ) return false;

	if( isHorizontal )
	{
		_pImage->LoadImage( CProgressBar::pcBackground, id, 0, 0, w, h );
		_pImage->LoadImage( CProgressBar::pcProgressbar, id, w, 0, w, h );
	}
	else
	{
		_pImage->LoadImage( CProgressBar::pcBackground, id, 0, 0, w, h );
		_pImage->LoadImage( CProgressBar::pcProgressbar, id, 0, h, w, h );
	}
	return true;
}

void CProgressBar::_RefreshPos() 
{
    if( _style==btHorizontal ) 
	{
		_pTex->nTexW = _step.GetShowPosition();
    }
    else 
	{
		_pTex->nTexH = _step.GetShowPosition();
		_pTex->nTexSY =  (int)(_nStart + _step.fLen - _pTex->nTexH);
    }
}

void CProgressBar::Start( DWORD time )
{
	if( time==0 )
	{
		_dwEndTime = 0;
	}
	else
	{
		_dwStartTime = CGameApp::GetCurTick();
		_dwEndTime = CGameApp::GetCurTick() + time;
	}
}

void CProgressBar::Render()
{	
	if( _dwEndTime > CGameApp::GetCurTick() )
	{	
		float fLen = (float)_dwEndTime - (float)_dwStartTime;
		float fPos = (float)CGameApp::GetCurTick() - (float)_dwStartTime;
		float fRate = _step.GetRange() / fLen * fPos - _step.GetMin();
		SetPosition( fRate );
	}
	else if( _dwEndTime!=0 )
	{
		if( evtTimeArrive ) evtTimeArrive( this );
		_dwEndTime = 0;
	}

	if( _style == btHorizontal ) 
	{
		if ( _nFlash >1 )
		{
			GetRender().RenderTextureRect( GetX(), GetY(), _pImage->GetImage() );
			static int i =0;
					
			_pTex->nTexSX =  0 ;
			_pTex->nTexSY =  i/4 * 15 ;
			_pTex->nTexW  =  123 ;
			_pTex->nTexH  =  15;
			i ++;
			if ( i>80 ) i=0;
			
			if ( _step.GetRate() >0.8f && _step.GetRate() <=1.0f )
				_pTex->fScaleX =  _step.GetRate() * 1.0f ;
			else if ( _step.GetRate() > 0.4f && _step.GetRate() <=0.8f )
				_pTex->fScaleX =  _step.GetRate() * 0.9f ;
			else if (_step.GetRate() > 0.1f && _step.GetRate() <=0.4f)
				_pTex->fScaleX =  _step.GetRate() * 0.5f ;
			else 
				_pTex->fScaleX = _step.GetRate() * 0.0f ;


			//GetWidth() *(1 -_step.GetRate());
			GetRender().RenderTextureRect( GetX() , GetY(), _pTex );
		}
		else
			_pImage->RenderAll( GetX(), GetY() );
	}
	else
	{
		GetRender().RenderTextureRect( GetX(), GetY(), _pImage->GetImage() );
		GetRender().RenderTextureRect( GetX(), int(GetY() + GetHeight() * ( 1 - _step.GetRate() )), _pTex );
		// GetRender().RenderTextureRect( GetX(), (int)(GetY() + GetHeight() * (1 - _step.GetRate())), _pTex );
	}
}

//---------------------------------------------------------------------------
// class CGrid
//---------------------------------------------------------------------------
void CGrid::Init()
{
	_pNextPage->LoadImage( "texture/ui/grid/button.tga", 16, 16, 0, 16 );
	_pNextPage->SetSize( 16, 16 );

	_pPriorPage->LoadImage( "texture/ui/grid/button.tga", 16, 16, 0, 0 );
	_pPriorPage->SetSize( 16, 16 );

	_pSizeImg->GetImage()->LoadImage( "texture/ui/grid/size.tga", 12, 12 );
	_pSizeImg->SetSize(12, 12);

	_pSelectImage->SetSize( _nUnitWidth, _nUnitHeight );

	sprintf( _strPage, "0/0" );
	_nStrWidth = CGuiFont::s_Font.GetWidth(_strPage);

    _nTotalW = _nSpaceX + _nUnitWidth;
    _nTotalH = _nSpaceY + _nUnitHeight;
}

//---------------------------------------------------------------------------
// class CCursor
//---------------------------------------------------------------------------
void CCursor::Init()
{
	_hCursor[stNormal] = LoadCursorFromFile("cursor/normal.ani"); 
	_hCursor[stActive] = LoadCursorFromFile("cursor/mouseon.ani"); 
	_hCursor[stDrag] = LoadCursorFromFile("cursor/drag.ani"); 
	_hCursor[stSize] = LoadCursorFromFile("cursor/drag.ani"); 	
	_hCursor[stVertical] = LoadCursorFromFile("cursor/drag.ani"); 	
	_hCursor[stLevel] = LoadCursorFromFile("cursor/drag.ani"); 	
	_hCursor[stWait] = LoadCursorFromFile("cursor/wait.ani"); 	

	_hCursor[stAttack] = LoadCursorFromFile("cursor/attack.ani"); 	
	_hCursor[stUpBank] = LoadCursorFromFile("cursor/land.ani"); 	
	_hCursor[stUpBoat] = LoadCursorFromFile("cursor/sail.ani"); 	
	_hCursor[stStop] = LoadCursorFromFile("cursor/forbid.ani"); 	
	_hCursor[stHover] = LoadCursorFromFile("cursor/mouseon.ani"); 	
	_hCursor[stPick] = LoadCursorFromFile("cursor/pick.ani"); 	
    _hCursor[stCamera] = LoadCursorFromFile("cursor/camera.ani"); 	
    _hCursor[stChat] = LoadCursorFromFile("cursor/chat.ani"); 	
    _hCursor[stMouse] = LoadCursorFromFile("cursor/mouseright.ani"); 	        
	_hCursor[stSkillAttack] = LoadCursorFromFile("cursor/skill.ani"); 	
    _hCursor[stButtonClick] = LoadCursorFromFile("cursor/mouseclick.ani"); 	
    _hCursor[stHide] = LoadCursorFromFile("cursor/hide.ani"); 	
    _hCursor[stSearch] = LoadCursorFromFile("cursor/search.ani"); 	
    _hCursor[stBlock] = LoadCursorFromFile("cursor/block.ani"); 	
    _hCursor[stRepair] = LoadCursorFromFile("cursor/repair.ani"); 	
    _hCursor[stFeed] = LoadCursorFromFile("cursor/feed.ani"); 	
	
    for( int i=0; i<CCursor::stEnd; i++ )
    {
        if( !_hCursor[i] )
        {
            g_logManager.InternalLog(LogLevel::Error, "errors", SafeVFormat(GetLanguageString(746), i));
        }
    }
    
	SetCursor( stNormal );

	_IsInit = true;
}

//---------------------------------------------------------------------------
// class CTreeView
//---------------------------------------------------------------------------
void CTreeNodeObj::ShowFocus()
{
	if( IsShow() && _pOwn->InTree( _nX, _nY, _nX2, _nY2 ) )
		GetRender().FillFrame( _nX-1, _nY-1, _nX2+1, _nY2+1, _pOwn->GetSelectColor() );

	_RenderTree();
}

void CTreeGridNode::ShowFocus()
{
	if( _IsShowFocus )
	{
		if( _IsExpand && IsShow() && _pOwn->InTree( _nFocusX1, _nFocusY1, _nFocusX2, _nFocusY2 ) )
		{
			_RenderTree();

			_pOwn->GetSelect()->GetItem()->Render( _nFocusX1, _nFocusY1 );
			_imgDown.Render( _nFocusX1, _nFocusY1 );
			return;
		}
		_RenderTree();
	}
	else
	{
		if( IsShow() && _pOwn->InTree( _nX, _nY, _nX + _pItem->GetWidth(), _nY + _pItem->GetHeight() ) )
			GetRender().FillFrame( _nX-1, _nY-1, _nX + _pItem->GetWidth()+1, _nY + _pItem->GetHeight()+1, _pOwn->GetSelectColor() );

		_RenderTree();
	}
}

//---------------------------------------------------------------------------
// class CFormMgr
//---------------------------------------------------------------------------
bool CFormMgr::Init(HWND hWnd)
{
	if( !_bInit )
	{
		CEdit::InitCursor( "texture/ui/editcursor.tga" );

		FontManager::Instance().PushToLua(g_LuaState);
		LoadLuaScript(g_LuaState, "scripts/lua/font.lua");
		if( !CGuiFont::s_Font.Init() )
		{
			g_logManager.InternalLog(LogLevel::Debug, "common", GetLanguageString(747).c_str());
			return false;
		}

		GetRender().SetScreen( g_Render.GetScrWidth(), g_Render.GetScrHeight(), (g_Render.IsFullScreen()!=0 ? true: false) );

		LoadLuaScript(g_LuaState, "scripts/lua/gui.lua");

		if( g_Config.m_bEditor ) LoadLuaScript(g_LuaState, "scripts/lua/forms/editor.lua"); 

		CImeInput::s_Ime.Init();
		CCursor::I()->Init();
		g_TextParse.Init();

		CGuiData::InitMemory();

		for( vfrm::iterator it=_allForms.begin(); it!=_allForms.end(); it++ )
			(*it)->Init();

		_InitFormID();

		for( vinits::iterator it=_vinits.begin(); it!=_vinits.end(); it++ )
		{
			if( !(*it)(this) )
				return false;
		}

		Refresh();
		_bInit = true;
	}

	_bEnabled = false;
	return _bInit;
}

CForm* CFormMgr::Find( const char *str )
{
    if( CGameApp::GetCurScene() )
        return Find( str, CGameApp::GetCurScene()->GetInitParam()->nUITemplete );
    else
        return Find( str, _nTempleteNo );
}

void CFormMgr::Clear()
{
	_bEnabled = false;

	g_TextParse.Clear();
	if( _bInit )
	{
		CGuiData::ClearAllGui();
	}

	_allForms.clear();
	_modal.clear();

	CImeInput::s_Ime.Clear();
	CGuiFont::s_Font.Clear();
	_bInit = false;
}

inline void UIRender::RenderSprite(LPTEXTURE tex, RECT* rc, VECTOR2* vscale, VECTOR2* vdest, DWORD dwColor)
{
	ui::UIClip* pCliper = ui::UIClip::Instance();
	if (pCliper->GetClipState())
	{
		int x1 = 0, x2 = 0, y1 = 0, y2 = 0;
		int w = rc->right - rc->left;
		int h = rc->bottom - rc->top;

		RECT* prc = &pCliper->GetClipRect();

		// Skip rendering if completely outside clip rect
		if (vdest->x > prc->right || vdest->x < prc->left - w)
		{
			return;
		}

		// ??DD2???
		if (vdest->x < prc->left)
		{
			x1 = (int)vdest->x - prc->left;
			vdest->x = (float)prc->left;
		}
		if (vdest->x + w > prc->right)
		{
			x2 = prc->right - (int)(vdest->x + w);
		}
		if (vdest->y < prc->top)
		{
			y1 = (int)vdest->y - prc->top;
			vdest->y = (float)prc->top;
		}
		if (vdest->y + h > prc->bottom)
		{
			y2 = prc->bottom - ((int)vdest->y + h);
		}
		rc->left = max(rc->left - x1, 0);
		rc->right = max(rc->right + x2, rc->left);
		rc->top = max(rc->top - y1, 0);
		rc->bottom = max(rc->bottom + y2, rc->top);
	}
	if (_p2DSprite)
	{
		_p2DSprite->Begin(D3DXSPRITE_ALPHABLEND);
		D3DXMATRIX m;
		D3DXMatrixTransformation2D(&m, NULL, 0, vscale, NULL, 0, vdest);
		_p2DSprite->SetTransform(&m);
		switch (_p2DSprite->Draw(tex, rc, NULL, NULL, dwColor)) {
		case D3DERR_INVALIDCALL:
			ToLogService("errors", LogLevel::Error, "drawing texture failed - invalid call");
			break;
		case D3DXERR_INVALIDDATA:
			ToLogService("errors", LogLevel::Error, "drawing texture failed - invalid data");
			break;
		case S_OK:
			break;
		}
		_p2DSprite->End();
		_p2DSprite->Flush();
	}
}
