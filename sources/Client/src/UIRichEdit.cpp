#include "StdAfx.h"
#include "uirichedit.h"
#include "gameapp.h"
#include "uieditkey.h"

using namespace GUI;
//---------------------------------------------------------------------------
// class CRichEdit
//---------------------------------------------------------------------------
CRichEdit::CRichEdit(CForm& frmOwn)
: CCompent(frmOwn), _IsReadyOnly(false)
{
	_IsFocus = true;

	_pEditKey = new CEditKey;

	_SetSelf();
}

CRichEdit::CRichEdit( const CRichEdit& rhs )
: CCompent(rhs)
{
	_pEditKey = new CEditKey;

	_Copy( rhs );
}

CRichEdit& CRichEdit::operator=(const CRichEdit& rhs)
{
	_Copy( rhs );
	return *this;
}

void CRichEdit::_SetSelf()
{
	_cArticle.SetEdit( this );

	memset( _szEnter, 0, sizeof(_szEnter) );
	_nEnterPos = 0;
}

void CRichEdit::_Copy( const CRichEdit& rhs )
{
	_SetSelf();
}

CRichEdit::~CRichEdit(void)
{
}

void CRichEdit::Init()
{
	_pEditKey->Init();
}

void CRichEdit::Refresh()
{
}

void CRichEdit::OnActive()
{
}

void CRichEdit::OnLost()
{
}

bool CRichEdit::OnKeyDown( int key )
{
	if( _IsReadyOnly ) return false;

	_cArticle.OnKeyDown( key, g_pGameApp->IsShiftPress()!=0 );
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

bool CRichEdit::OnChar( char c )
{
	if( _IsReadyOnly ) return false;

	// 
	switch( c )
	{
	case '\r':		// 
		_cArticle.AddControl( c );
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

					_cArticle.AddChar( c );
				}
			}
			else if( _nEnterPos==2 )
			{
				if( _ismbslead( (unsigned char*)_szEnter, (unsigned char*)&_szEnter[0] )==-1 && _ismbstrail( (unsigned char*)_szEnter, (unsigned char*)&_szEnter[1] )==-1 )
				{
					// 
					_cArticle.AddChar(_szEnter[0], _szEnter[1] );

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

void CRichEdit::Render()
{
	_cArticle.Render();

	_pEditKey->Render();
}

