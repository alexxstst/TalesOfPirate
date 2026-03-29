//------------------------------------------------------------------------
//	2005/3/24	Arcol	
//------------------------------------------------------------------------
#include "StdAfx.h"
#include "uicompent.h"
#include "UIEdit.h"

using namespace GUI;

CCompent* CCompent::_pActive = NULL;
CCompent* CCompent::_pLastMouseCompent = NULL;
//---------------------------------------------------------------------------
// class CCompent
//---------------------------------------------------------------------------
CCompent::CCompent(CForm& frmOwn)
: CGuiData(), _frmOwn(&frmOwn), _align(caNone), _isChild(false)
, _eMouseAction(enumMA_Gui), _IsFocus(false)
, evtLost(NULL), evtActive(NULL)
{
	_pParent = _frmOwn;
}

CCompent::CCompent(const CCompent& rhs)
: CGuiData( rhs )
{
	_Copy( rhs );
}

CCompent& CCompent::operator=( const CCompent& rhs )
{
	CGuiData::operator =( rhs );
	_Copy( rhs );
	return *this;
}

void CCompent::_Copy( const CCompent& rhs )
{
	_frmOwn		= rhs._frmOwn; 

	evtLost		= rhs.evtLost;
	evtActive	= rhs.evtActive;

	_align		= rhs._align;
	_isChild	= rhs._isChild;
	_IsFocus	= rhs._IsFocus;

	_eMouseAction = rhs._eMouseAction;
}

void CCompent::_AddForm()
{
	if( !_frmOwn ) return;
	_frmOwn->_AddFrameMove( this, false );

	if( _isChild ) return;
	_frmOwn->_AddCompent( this, false );
}

void CCompent::AddForm()
{
	if( !_frmOwn ) return;
	_frmOwn->_AddFrameMove( this, true );

	if( _isChild ) return;
    _frmOwn->_AddCompent( this, true );
}

CCompent::~CCompent(void)
{
}

void CCompent::SetAlign( eCompentAlign v )
{
    if( !GetParent() ) return;

	_align = v;
}

void CCompent::SetIsShow( bool v )			    
{ 
	_bShow = v;
	if( v && _IsFocus )	
	{
		SetActive( this );
	}
}

void CCompent::Refresh()
{
    switch( _align )
    {
    case caLeft: 
        {
            SetPos( 0, 0 );
            SetSize( GetWidth(), GetParent()->GetHeight() );
        }
        break;
    case caLeftUp: 
        {
            SetPos( 0, 0 );			
        }
        break;
    case caUp: 
        {
            SetPos( 0, 0 );
            SetSize( GetParent()->GetWidth(), GetHeight() );
        }
        break;
    case caRightUp: 
        {
            SetPos( GetParent()->GetWidth() - GetWidth(), 0 );
        }
        break;
    case caRight: 
        {
            SetPos( GetParent()->GetWidth() - GetWidth(), 0 );
            SetSize( GetWidth(), GetParent()->GetHeight() );
        }
        break;
    case caRightBottom: 
        {
            SetPos( GetParent()->GetWidth() - GetWidth(), GetParent()->GetHeight() -  GetHeight() );
        }
        break;
    case caBottom: 
        {
            SetPos( 0, GetParent()->GetHeight() -  GetHeight() );
            SetSize( GetParent()->GetWidth(), GetHeight() );
        }
        break;
    case caLeftBottom: 
        {
            SetPos( 0, GetParent()->GetHeight() - GetHeight() );
        }
        break;
    case caClient: 
        {
            SetPos( 0, 0 );
            SetSize( GetParent()->GetWidth(), GetParent()->GetHeight() );
        }
        break;
    case caWidthCenter:
        SetPos( (GetParent()->GetWidth() - GetWidth())/2, GetTop() );
        break;
    case caHeightCenter:
        SetPos( GetLeft(), (GetParent()->GetHeight()-GetHeight())/2 );
        break;
    case caCenter:
        SetPos( (GetParent()->GetWidth() - GetWidth())/2, (GetParent()->GetHeight()-GetHeight())/2 );
        break;
    }
    CGuiData::Refresh();
}

 bool CCompent::MouseRun( int x, int y, DWORD key )
{
	if( !IsNormal() ) return false;

	if( IsNoDrag( x, y, key ) )
	{
		if( !_isChild && (GetActive()!=this) && ( key & Mouse_LDown ) )
		{
			_SetActive();
		}
		return true;
	}
	return _IsMouseIn;
}

void CCompent::OnActive() 
{
	GetForm()->SetActiveCompent( this );
	if( evtActive ) evtActive(this);
}

void CCompent::SetActive(CCompent* v) 
{ 
    // 
    if(_pActive==v ) return;

    if( _pActive ) _pActive->OnLost();

    _pActive = v;

    if( _pActive ) _pActive->OnActive();
    return;
}

void CCompent::SetParent( CGuiData* p )
{ 
	if( p!=_frmOwn ) _isChild=true;
	_pParent = p; 
}

void CCompent::SetIsDrag( bool v )
{
	// 
	if( _isChild ) return;

	CGuiData::SetIsDrag(v);
}

CCompent* CCompent::GetHintCompent( int x, int y )    
{ 
    if( GetIsShow() && InRect( x, y ) && !_strHint.empty() )
        return this;

    return NULL;
} 


//---------------------------------------------------------------------------
// class CContainer
//---------------------------------------------------------------------------
CContainer::CContainer(CForm& frmOwn)
: CCompent(frmOwn)
{
}

CContainer::CContainer( const CContainer& rhs )
: CCompent(rhs)
{
	_SetSelf(rhs);
}

CContainer& CContainer::operator=(const CContainer& rhs)
{
	CCompent::operator =( rhs );

	_SetSelf(rhs);
	return *this;
}

CContainer::~CContainer(void)
{
}

void CContainer::Init()
{
	for( items::iterator it=_items.begin(); it!=_items.end(); it++ )
		(*it)->Init();
}

void CContainer::_SetSelf( const CContainer& rhs )
{
	CGuiData* p=NULL;
	CCompent* c=NULL;
	for( items::const_iterator it=rhs._items.begin(); it!=rhs._items.end(); it++ )
	{
		p = (*it)->Clone();

		c = dynamic_cast<CCompent*>(p);
		if( c )
		{
			_items.push_back(c);
			c->SetParent(this);
		}
	}
}

void CContainer::Render()
{	
	for( items::iterator it=_items.begin(); it!=_items.end(); it++ )
        if( (*it)->GetIsShow() )
		    (*it)->Render();
}

void CContainer::Refresh()
{
	CCompent::Refresh();

	for( items::iterator it=_items.begin(); it!=_items.end(); it++ )
		(*it)->Refresh();
}

bool CContainer::MouseRun( int x, int y, DWORD key )
{
	if( !IsNormal() ) return false;

	for( items::iterator it=_mouse.begin(); it!=_mouse.end(); it++ )
	{
		if( (*it)->MouseRun( x, y, key ) )
		{
			_pLastMouseCompent = *it;
			return true;
		}
	}

	return false;
}

bool CContainer::AddCompent( CCompent* p )
{
	if( p==this || p==_pParent ) return false;

	items::iterator it = find( _items.begin(), _items.end(), p );
	if( _items.end() == it )
	{
		_items.push_back(p);

		// _pParnet,_isChild,SetParent
		p->_pParent = this;		

		if( p->IsHandleMouse() || p->GetIsFocus() ) _mouse.push_back( p );
		return true;
	}
	return false;
}

void CContainer::ForEach( CompentFun pFun )
{
	int nIndex = 0;
	for( items::iterator it = _items.begin(); it!=_items.end(); it++ )
		pFun( *it, nIndex++ );
}

int CContainer::GetIndex( CCompent* p )
{
	int nIndex = 0;
	for( items::iterator it = _items.begin(); it!=_items.end(); it++ )
	{
		nIndex++;
		if( (*it) == p )
			return nIndex;
	}
	return -1;
}

void CContainer::SetAlpha( BYTE alpha )
{
	for( items::iterator it = _items.begin(); it!=_items.end(); it++ )
		(*it)->SetAlpha( alpha );
}

CCompent* CContainer::Find( const char* str )
{
	for( items::iterator it = _items.begin(); it!=_items.end(); it++ )
	{
		if( strcmp( (*it)->GetName(), str ) == 0 )
			return *it;
	}

	return NULL;
}

CCompent* CContainer::GetHitCommand(int x,int y)
{
    CCompent* pCommand = NULL;
    for( items::iterator it = _items.begin(); it!=_items.end(); it++ )
    {
        if( pCommand=(*it)->GetHitCommand( x, y ) )
        {
            return pCommand;
        }
    }

    return NULL;
}

CCompent* CContainer::GetHintCompent( int x, int y )
{
    if( GetIsShow() )
    {
        if( InRect( x, y ) && !_strHint.empty() )
            return this;

        CCompent* pHint = NULL;
        for( items::iterator it = _items.begin(); it!=_items.end(); it++ )
        {
            if( pHint = (*it)->GetHintCompent( x, y ) )
            {
                return pHint;
            }
        }
    }
    return NULL;
}

void CContainer::FrameMove( DWORD dwTime )
{
    if( !GetIsShow() ) return;

	for( items::iterator it = _items.begin(); it!=_items.end(); it++ )
		(*it)->FrameMove( dwTime );
}

