#include "StdAfx.h"
#include "uicombo.h"

using namespace GUI;

CCombo::CCombo(CForm& frmOwn)
: CCompent(frmOwn), _pEdit( new CEdit(frmOwn) ), _pList( new CList(frmOwn) ), _pButton( new CTextButton(frmOwn) )
, _eListStyle(lsDown), evtSelectChange(NULL), evtMouseClick(NULL)
{
	_IsFocus = false;
	_SetSelf();
}

CCombo::CCombo( const CCombo& rhs )
: CCompent(rhs), _pEdit( new CEdit(*rhs._pEdit)), _pList( new CList(*rhs._pList) ), _pButton( new CTextButton(*rhs._pButton) )
, _eListStyle( rhs._eListStyle)
{
	_SetSelf();

	evtSelectChange = rhs.evtSelectChange;
	evtMouseClick = rhs.evtMouseClick;
}

void CCombo::_SetSelf()
{
	_pEdit->SetParent( this );
	_pList->SetParent( this );
	_pButton->SetParent( this );

	_pButton->evtMouseClick = _OnButtonClick;
	_pList->evtSelectChange = _OnListSelectChange;
	_pList->evtListMouseDown = _OnListDown;

}

CCombo& CCombo::operator=(const CCombo& rhs)
{
	CCompent::operator =( rhs );

	*_pEdit = *(rhs._pEdit);
	*_pList = *(rhs._pList);
	*_pButton = *(rhs._pButton);

	_eListStyle = rhs._eListStyle;

	_SetSelf();

	evtSelectChange = rhs.evtSelectChange;
	evtMouseClick = rhs.evtMouseClick;
	return *this;
}

CCombo::~CCombo(void)
{
}

void CCombo::Init()
{
	_SetSelf();

	//_pButton->SetSize( GetHeight(), GetHeight() );
	//_pButton->SetPos( GetWidth() - _pButton->GetWidth(), 0 );

	_pEdit->SetPos( 4, 4 );
	_pEdit->SetSize( GetWidth() - 8, GetHeight() -8 );
	//_pEdit->SetSize( GetWidth() - _pButton->GetWidth(), GetHeight() );

	_pList->SetSize( GetWidth(), _pList->GetHeight() );
	_RefreshList();
	_pList->SetIsShow( false );

	_pList->Init();
	_pEdit->Init();
	_pButton->Init();
}

void CCombo::_RefreshList()
{
	switch( _eListStyle )
	{
	case lsUp:
		_pList->SetPos( 0, 0 - _pList->GetHeight() );
	break;
	case lsDown:
		_pList->SetPos( 0, _pEdit->GetHeight() );
	break;
	}
}

void CCombo::Render()
{	
	_pEdit->Render();
	_pButton->Render();
    if( _pList->GetIsShow() )_pList->Render();
}

void CCombo::Refresh()
{
	CCompent::Refresh();

	_pEdit->Refresh();
	_pButton->Refresh();
	_pList->Refresh();
}


bool CCombo::MouseRun( int x, int y, DWORD key )
{
	if( !IsNormal() ) return false;

	if( IsNoDrag( x, y, key ) )
	{
		if( key & Mouse_LDown )
		{
			if( !_isChild && GetActive()!=this ) 
			{
				_SetActive();
				_pList->SetIsShow( !_pList->GetIsShow() );
				if( evtMouseClick ) evtMouseClick(this, x, y, key);
				return true;
			}
		}

		if( _pButton->MouseRun( x, y, key ) ) return true;
		if( key & Mouse_LDown ) 
		{
			_pList->SetIsShow( !_pList->GetIsShow() );
			return true;
		}
	}

	if( _pList->MouseRun( x, y, key ) ) return true;

	if( _pList->GetIsShow() && ( key & Mouse_LDown ) )
	{
		_pList->SetIsShow( false );
	}

	return _IsMouseIn;
}

void CCombo::_OnButtonClick(int x, int y, DWORD key)
{ 
	_pList->SetIsShow( !_pList->GetIsShow() );
	if( evtMouseClick ) evtMouseClick(this, x, y, key);
}

void CCombo::_OnListSelectChange()
{
	CItemRow* p = _pList->GetSelectItem();
	if( p ) 
	{
		_pEdit->SetCaption( p->GetBegin()->GetString() );

		if( evtSelectChange ) evtSelectChange(this);
	}
}

void CCombo::SetListStyle( eListStyle v )
{
	if( v==_eListStyle ) return;

	_eListStyle = v;
}

bool CCombo::OnKeyDown( BYTE key ) 
{ 
	if( !IsNormal() ) return false;

	return _pList->OnKeyDown(key);
}

bool CCombo::OnChar( BYTE c ) 
{ 
	if( !IsNormal() ) return false;

	switch( c )
	{
	case '\r':	
	case VK_SPACE:
	case 27:
		_pList->SetIsShow(!_pList->GetIsShow());
		break;
	default: return false;
	}
	return true;
}

bool CCombo::MouseScroll( int nScroll )
{
	if( !IsNormal() ) return false;
	
	return _pList->MouseScroll( nScroll );
}

void CCombo::SetAlpha( BYTE alpha )
{ 
	_pList->SetAlpha(alpha); 
	_pEdit->SetAlpha(alpha); 
	_pButton->SetAlpha(alpha); 
}

void CCombo::OnActive()	
{ 
	CCompent::OnActive(); 
	_pList->OnActive(); 
}

void CCombo::OnLost()	
{
	_pList->SetIsShow(false);
	_pList->OnLost();		
}
