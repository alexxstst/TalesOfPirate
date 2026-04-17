#include "StdAfx.h"
#include "UIText.h"
#include "uieditkey.h"
#include "gameapp.h"
#include "uirender.h"
#include "uifont.h"
#include "uieditstrategy.h"
#include "uieditdata.h"

using namespace GUI;
//---------------------------------------------------------------------------
// class CEditKey
//---------------------------------------------------------------------------
CEditKey::CEditKey()
: _IsReadyOnly( false ), _dwFontIndex(0)
, _nCursorX(0), _nCursorY(0), _dwCursorColor(0xffffffff)
{
	memset( _szEnter, 0, sizeof(_szEnter) );

	_dwCursorSpace = GetCaretBlinkTime();
	if( _dwCursorSpace==0 ) _dwCursorSpace=530;
	_dwCursorTime = 0;

	_pParse = new CEditParse;
}

CEditKey::~CEditKey()
{
	//delete _pParse;
	SAFE_DELETE(_pParse); // UI
}

bool CEditKey::OnKeyDown( int key )
{
	if( _IsReadyOnly ) return false;

	//_cArticle.OnKeyDown( key, g_pGameApp->IsShiftPress()!=0 );
	switch( key )
	{
	case VK_LEFT:
		break;
	case VK_RIGHT:
		break;
	case VK_UP:
		break;
	case VK_DOWN:
		break;
	case VK_HOME:
		break;
	case VK_END:
		break;
	case VK_PRIOR:	// pageup
		break;
	case VK_NEXT:	// pagedown
		break;
	case VK_DELETE:
		break;
	default:
		return false;
	}
	return false;
}

bool CEditKey::OnChar( char c )
{
	if( _IsReadyOnly ) return false;

	// 
	switch( c )
	{
	case '\r':		// 
		AddChar( new CEditControl( c ) );
		break;
	case '\b':		// 
		break;
	case '\t':
		break;
	case 3:			// copy
		break;
	case 22:		// paste
		break;
	case 24:		// cut
		break;
	case 27:		// ESC
		break;
	default:
		{
			_szEnter[_nEnterPos++] = c;
			bool	IsError = false;
			if( _nEnterPos==1 )
			{
				IsError = false;
				if( _ismbslead( (unsigned char*)_szEnter, (unsigned char*)&_szEnter[0] )==0 
					&& _ismbstrail( (unsigned char*)_szEnter, (unsigned char*)&_szEnter[0] )==0 )
				{
					// 
					_nEnterPos = 0;

					AddChar( new CEditChar( c ) );
				}
			}
			else if( _nEnterPos==2 )
			{
				if( _ismbslead( (unsigned char*)_szEnter, (unsigned char*)&_szEnter[0] )==-1 && _ismbstrail( (unsigned char*)_szEnter, (unsigned char*)&_szEnter[1] )==-1 )
				{
					// 
					AddChar( new CEditChar( _szEnter[0], _szEnter[1] ) );


					IsError = false;
					_nEnterPos = 0;
					_szEnter[1] = 0;
				}
			}

			if( IsError )
			{
				// 
				_nEnterPos = 0;
				_szEnter[1] = 0;
			}
		}
	}

	return false;
}

void CEditKey::Render()
{
	if( CGameApp::GetCurTick()>=_dwCursorTime ) 
	{
		_dwCursorTime = CGameApp::GetCurTick()+_dwCursorSpace;
		_IsShowCursor = !_IsShowCursor;
	}

	if( !_IsShowCursor ) return;

	GetRender().FillFrame( _nCursorX, _nCursorY, _nCursorX + 2, _nCursorY + _dwCursorHeight, _dwCursorColor );
}

bool CEditKey::SetFont( DWORD dwFont )
{
	if( !ui::GetFont( dwFont ) ) return false;

	_dwFontIndex = dwFont;
	Init();
	return true;
}

void CEditKey::Init()
{	
	int w, h;
	ui::GetSize( _dwFontIndex, GetLanguageString(489).c_str(), w, h );
	_dwCursorHeight = h;
}

void CEditKey::AddChar( CEditObj* pObj )
{
	int rv = _pParse->Insert( _dwCurosrIndex, pObj );
	if( rv==-1 )
	{
		//delete pObj;
		SAFE_DELETE(pObj); // UI
	}
	else
	{
		_dwCurosrIndex = rv;
	}
}
