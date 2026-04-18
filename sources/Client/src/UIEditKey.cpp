#include "StdAfx.h"
#include "UIText.h"
#include "uieditkey.h"
#include "gameapp.h"
#include "uirender.h"
#include "uifont.h"
#include "uieditstrategy.h"
#include "uieditdata.h"
#include "EncodingUtil.h"

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

	switch( c )
	{
	case '\r':
		AddChar( new CEditControl( c ) );
		break;
	case '\b':
	case '\t':
	case 3:   // copy
	case 22:  // paste
	case 24:  // cut
	case 27:  // ESC
		break;
	default:
		{
			// WM_CHAR отдаёт байт CP_ACP. Переводим в UTF-8 (1-2 байта для
			// ASCII/кириллицы). Для DBCS lead-байта (CJK) накапливаем пару
			// байт, затем конвертируем два байта CP_ACP → UTF-8. Хранение
			// в CEditChar остаётся побайтовое (1-2 байт на символ UTF-8).
			const unsigned char byte = static_cast<unsigned char>(c);
			std::string utf8;
			if (_nEnterPos == 1) {
				_szEnter[1] = c;
				const std::string_view pair(_szEnter, 2);
				utf8 = encoding::AnsiToUtf8(pair);
				_nEnterPos = 0;
				_szEnter[0] = _szEnter[1] = 0;
			}
			else if (byte >= 0x80 && ::IsDBCSLeadByteEx(CP_ACP, byte)) {
				_szEnter[0] = c;
				_nEnterPos = 1;
				return false;  // ждём trail-байт
			}
			else {
				encoding::AppendAnsiByteAsUtf8(byte, utf8);
			}
			// Порционно разложить UTF-8 в CEditChar: 1 byte → CEditChar(b0),
			// 2 byte → CEditChar(b0, b1). 3+ byte (CJK UTF-8) пока не
			// влезает в CEditChar — ограничение побайтного хранения.
			if (utf8.size() == 1) {
				AddChar( new CEditChar( utf8[0] ) );
			}
			else if (utf8.size() == 2) {
				AddChar( new CEditChar( utf8[0], utf8[1] ) );
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
