#include "StdAfx.h"
#include "UIText.h"
#include "uititle.h"
#include "gameapp.h"
using namespace std;
using namespace GUI;
//---------------------------------------------------------------------------
// class CTitle
//---------------------------------------------------------------------------
CTitle::CTitle(CForm& frmOwn)
: CCompent(frmOwn), _dwColor(0x00f01000), _nFontIndex(0), _dwShowTime(3000), _nFontH(5)
, _alpha(0), _endtime(0)
{
	_pImage = new CGuiPic( NULL, enumMax );
}

CTitle::CTitle( const CTitle& rhs )
: CCompent(rhs)
{
	_Copy( rhs );
}

CTitle& CTitle::operator=(const CTitle& rhs)
{
	_Copy( rhs );
	return *this;
}

CTitle::~CTitle(void)
{
}

void CTitle::_Copy( const CTitle& rhs )
{
	_nFontIndex = rhs._nFontIndex;
	_strCaption = rhs._strCaption;
	_dwColor = rhs._dwColor;
	_dwShowTime = rhs._dwShowTime;
	_nFontH = rhs._nFontH;

	_alpha = rhs._alpha;
	_endtime = rhs._endtime;
}

void CTitle::Render()
{	
	if( _endtime>=CGameApp::GetCurTick() )
	{
        _alpha = (BYTE)(255.0f * (float)(_endtime - CGameApp::GetCurTick()) / (float)_dwShowTime);

		_pImage->SetAlpha( _alpha );
		_pImage->FrameRender( enumCenter, _posx[enumCenter], _posy );
		_pImage->FrameRender( enumLeft, _posx[enumLeft], _posy );
		_pImage->FrameRender( enumRight, _posx[enumRight], _posy );

	    ui::Render( _nFontIndex, (char*)_strCaption.c_str(), _posx[enumCenter], _posfonty, ( _alpha << 24 ) | (_dwColor &  0x00FFFFFF) );
	}
}

void CTitle::SetCaption( const char * str )	
{
	_strCaption = str;	

	_endtime = CGameApp::GetCurTick() + _dwShowTime;
	_alpha = 255;

	int w, h;
	ui::GetSize( _nFontIndex, _strCaption.c_str(), w, h );   

	int picWidth = _pImage->GetWidth();
	_posx[enumLeft] = ( g_pGameApp->GetWindowWidth() - w ) / 2 - picWidth;
	_posx[enumCenter] = _posx[enumLeft] + picWidth;
	_pImage->SetScale( enumCenter, w, _pImage->GetHeight(enumCenter) );
	_posx[enumRight] = (g_pGameApp->GetWindowWidth() + w) / 2;

	_posy = ( g_pGameApp->GetWindowHeight() - _pImage->GetHeight() ) / 3;
	_posfonty = _posy + _nFontH;
}

void CTitle::Init()
{
	_dwColor &= 0x00FFFFFF;
	_alpha = 0;
	_endtime = 0;
}

