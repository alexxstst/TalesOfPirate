#include "StdAfx.h"
#include "uimenu.h"
#include "UIForm.h"

using namespace GUI;

CMenuItem::CMenuItem( const std::string& str, DWORD v )
: _IsCheck(false), _IsEnabled(true), _IsHide(false), _strText(str), _nTag(0), _dwIndex(v)
{
	CGuiFont::s_Font.GetSize( str, _nWidth, _nHeight );	
}

void CMenuItem::Render( int x, int y )	
{
	CGuiFont::s_Font.Render( _strText.c_str(), x, y, _IsEnabled ? COLOR_BLACK : 0xffa0a0a0 );
}

//---------------------------------------------------------------------------
// class CMenu
//---------------------------------------------------------------------------
CMenu::allmenus	CMenu::_allmenus;

CMenu::CMenu( CForm* pForm ) 
: CGuiData(), evtSelectChange(NULL), evtListMouseDown(NULL), _eStyle(enumMouseClick)
{
	_pSelect = new CGuiPic;
	_pImage = new CFramePic( this );

	_nRowSpace = 2;

	_nLeftMargin = 5;
	_nTopMargin = 5;
	_nRightMargin = 5;
	_nBottomMargin = 5;

	evtSelectChange = NULL;
	evtListMouseDown = NULL;

	_pSelectMenu = NULL;

	_pParent = NULL;

	_allmenus.push_back( this );
}

CMenu::CMenu( const CMenu& rhs )
:CGuiData(rhs), _pSelect( new CGuiPic(*rhs._pSelect) ), _pImage( new CFramePic(*rhs._pImage) )
{
	_Copy(rhs);
}

CMenu& CMenu::operator=(const CMenu& rhs)
{
	CGuiData::operator =(rhs);

	*_pSelect = *rhs._pSelect;
	*_pImage = *rhs._pImage;
	
	_Copy(rhs);
	return *this;
}

void CMenu::_Copy( const CMenu& rhs )
{
	_pImage->SetParent( this );

	_nRowSpace = rhs._nRowSpace;

	_nLeftMargin = rhs._nLeftMargin;
	_nTopMargin = rhs._nTopMargin;
	_nRightMargin = rhs._nRightMargin;
	_nBottomMargin = rhs._nBottomMargin;

	_eStyle = rhs._eStyle;

	evtSelectChange = rhs.evtSelectChange;
	evtListMouseDown = rhs.evtListMouseDown;

	_MenusClear();
	for( auto it=rhs._menus.begin(); it!=rhs._menus.end(); it++ )
		_menus.push_back( (*it)->Clone() );
}

void CMenu::_MenusClear()
{
	for( auto it=_menus.begin(); it!=_menus.end(); it++ )
	{
		//delete *it;
		SAFE_DELETE(*it); // UI
	}
	_menus.clear();
	_pSelectMenu = NULL;
}

CMenu::~CMenu() 
{ 
	_MenusClear();
	//delete _pSelect;
	//delete _pImage;

	SAFE_DELETE(_pSelect); // UI
	SAFE_DELETE(_pImage);  // UI
}

CMenuItem* CMenu::AddMenu(const std::string& szText)
{
	int nLastID = 1;
	if( !_menus.empty() )
	{
		nLastID = _menus.back()->GetIndex() + 1;
	}

	CMenuItem* tmp = new CMenuItem( szText, nLastID );
	_menus.push_back( tmp );
	return tmp;
}

bool CMenu::DelMenu( CMenuItem* pMenu )
{
	auto it = find( _menus.begin(), _menus.end(), pMenu );
	if( it!=_menus.end() )
	{
		//delete pMenu;
		SAFE_DELETE(pMenu); // UI

		_menus.erase( it );
		return true;
	}
	return false;
}

bool CMenu::MouseRun( int x, int y, DWORD key )
{
	if( !IsNormal() ) return false;

	if( InRect( x, y ) )
	{
		int y0 = _nStartY;
		int y1 = 0;
		for( auto it=_menus.begin(); it!=_menus.end(); it++ )
		{
			if( (*it)->GetIsHide() ) 
				continue;
			
			y1 = y0 + (*it)->GetHeight();
			if( y>=y0 && y<=y1 )
			{
				if( _pSelectMenu!=*it )
				{
					_pSelectMenu = *it;
					_pSelect->SetScale( _pSelectMenu->GetWidth(), _pSelectMenu->GetHeight() );

					if( evtSelectChange ) evtSelectChange( this );
				}

				if( (key & Mouse_LDown) && _pSelectMenu->GetIsEnabled() )
				{
					if( evtListMouseDown ) evtListMouseDown( this, x, y, key );
				}				
				break;
			}
			y0 = y1 + _nRowSpace;
		}
	}
	else
	{
		switch( _eStyle )
		{
		case enumMouseFollow:
			_bShow = false;
			break;
		case enumMouseClick:
			if( key & Mouse_LDown )
			{
				_bShow = false;
			}
			break;
		}
	}

	return _IsMouseIn;
}

void CMenu::Render()
{
	_pImage->Render();	
	
	int x = _nStartX;
	int y0 = _nStartY;
	for( auto it=_menus.begin(); it!=_menus.end(); it++ )
	{
		if( !(*it)->GetIsHide() )
		{
			if( _pSelectMenu==(*it) )
			{
				_pSelect->Render( x, y0 );
			}

			(*it)->Render( x, y0 );
			y0 += (*it)->GetHeight() + _nRowSpace;
		}		
	}
}

void CMenu::Refresh()
{
	_nX1 = _nLeft;
	_nY1 = _nTop;
	_nX2 = _nX1 + _nWidth;

	_nStartX = _nLeftMargin + _nX1;
	_nStartY = _nTopMargin + _nY1;

	_nStartY2 = _nStartY;
	for( auto it=_menus.begin(); it!=_menus.end(); it++ )
	{
		if( !(*it)->GetIsHide() )
		{
			_nStartY2 += (*it)->GetHeight() + _nRowSpace;
		}
	}
	_nY2 = _nStartY2 + _nBottomMargin;
	_nHeight = _nY2 - _nY1;

	_pImage->Refresh();
}

CMenu* CMenu::FindMenu( const std::string& name )
{
	for(auto it=_allmenus.begin(); it!=_allmenus.end(); it++ )
		if( strcmp( (*it)->GetName(), name.c_str() )==0 )
			return (*it);

	return NULL;
}

void CMenu::SetParent( CGuiData* p )		
{ 
	if( _pParent && _pParent!=p )
	{
		CForm* frm = (CForm*)_pParent;
		frm->PopMenu( NULL );
	}

	_pParent = p;
}

void CMenu::SetAllEnabled( bool isEnabled )
{
	for( auto it=_menus.begin(); it!=_menus.end(); it++ )
	{
		(*it)->SetIsEnabled( isEnabled );
	}
}

bool CMenu::IsAllDisabled()
{
	for( auto it=_menus.begin(); it!=_menus.end(); it++ )
	{
		if( (*it)->GetIsEnabled() && !(*it)->GetIsHide() )
			return false;
	}
	return true;
}

int	CMenu::CloseAll()
{
	int i = 0;
	for(auto it=_allmenus.begin(); it!=_allmenus.end(); it++ )
	{
		if( (*it)->GetIsShow() )
		{
			(*it)->SetIsShow( false );
			i++;
		}
	}
	return i;
}
