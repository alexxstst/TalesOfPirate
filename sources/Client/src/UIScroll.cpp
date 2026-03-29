#include "StdAfx.h"
#include "UIScroll.h"

using namespace GUI;

CTextButton* CScroll::_pContinueButton = NULL;
DWORD CScroll::_dwStartTime = 0;

CScroll::CScroll(CForm& frmOwn)
: CCompent(frmOwn), _eStyle(btVertical), evtChange(NULL), _fPageNum(2.0), _IsAutoHide(true)
{
	_up = new CTextButton( frmOwn );
	_down = new CTextButton( frmOwn );
	_pImage = new CGuiPic( this );

	_pScroll = new CDragTitle(frmOwn);

	_SetSelf();
}

CScroll::CScroll( const CScroll& rhs )
: CCompent(rhs), _up( new CTextButton( *rhs._up ) ), _down( new CTextButton( *rhs._down ) )
, _pScroll(new CDragTitle(*rhs._pScroll)), _pImage(new CGuiPic(*rhs._pImage))
{
	_Copy( rhs );

	_SetSelf();
}

void CScroll::_Copy( const CScroll& rhs )
{
	evtChange = rhs.evtChange;

	_eStyle = rhs._eStyle;
	_step = rhs._step;
	_nStart = rhs._nStart;
	_fPageNum = rhs._fPageNum;

	_nMinDrag = rhs._nMinDrag;
	_nMaxDrag = rhs._nMaxDrag;

	_IsAutoHide = rhs._IsAutoHide;
}

void CScroll::_SetSelf()
{
	_pImage->SetParent( this );

	_up->SetParent( this );
	_up->evtMouseClick = _UpClick;

	_down->SetParent( this );
	_down->evtMouseClick = _DownClick;

	_pScroll->SetParent( this );

	CDrag *pDrag = _pScroll->GetDrag();
	pDrag->SetDragInCursor( CCursor::stActive );
	pDrag->SetIsMove( true );
	pDrag->SetIsUseGrid( false );
	pDrag->evtMouseDragBegin = _DragBegin;
	pDrag->evtMouseDragMove = _DragMove;
	pDrag->evtMouseDragEnd = NULL;
}

CScroll& CScroll::operator=( const CScroll& rhs )
{
	CCompent::operator =( rhs );

	*_up = *(rhs._up);
	*_down = *(rhs._down);
	*_pImage = *(rhs._pImage);
	*_pScroll = *rhs._pScroll;

	_Copy( rhs );

	_SetSelf();
	return *this;
}

CScroll::~CScroll(void)
{
	//delete _pImage;
	SAFE_DELETE(_pImage); // UI
}

void CScroll::Render()
{	
	_pImage->Render( GetX(), GetY() );

    if( _up->GetIsShow() ) _up->Render();
	if( _down->GetIsShow() ) _down->Render();
    if( _pScroll->GetIsShow() ) _pScroll->DragRender();
}

void CScroll::Init()
{
	switch( _eStyle )
	{
	case btVertical:
		_pScroll->GetDrag()->SetDragCursor( CCursor::stVertical );

		_down->SetPos( 0, GetHeight() - _down->GetHeight() + 1 );
		_step.fLen = (float)(GetHeight() - _up->GetHeight() - _down->GetHeight() - _pScroll->GetHeight());
		_nStart = _up->GetHeight();
		break;
	case btHorizontal:
		_pScroll->GetDrag()->SetDragCursor( CCursor::stLevel );

		_down->SetPos( GetWidth() - _down->GetWidth() + 1, 0 );
		_step.fLen = (float)(GetWidth() - _up->GetWidth() - _down->GetWidth() - _pScroll->GetWidth());
		_nStart = _up->GetWidth();
		break;
	}
}

void CScroll::Refresh()
{
	CCompent::Refresh();

	_pImage->Refresh();
	_up->Refresh();
	_down->Refresh();
	_pScroll->Refresh();

	_RefreshMidst();
}

bool CScroll::MouseRun( int x, int y, DWORD key )
{
	if( !IsNormal() ) return false;

    _pContinueButton = NULL;
	if( InRect( x, y ) )
	{
        if( key & Mouse_LDown )
        {
            if( _up->InRect( x, y ) )
            {
                _pContinueButton = _up;
                _dwStartTime = 0;
            }
            if( _down->InRect( x, y ) )
            {
                _pContinueButton = _down;
                _dwStartTime = 0;
            }
        }

		if( _up->MouseRun(x,y,key) ) return true;
		if( _down->MouseRun(x,y,key) ) return true;
		if( _pScroll->MouseRun(x,y,key) ) return true;

		// 
		if( key & Mouse_LDown )
		{
	        switch( _eStyle )
	        {
	        case btVertical:
		        if( y > _pScroll->GetY2() ) 
                {
                    PageUp();
                }
		        else if( y < _pScroll->GetY() ) PageDown();
	        break;
	        case btHorizontal:
		        if( x > _pScroll->GetX2() ) PageUp();
		        else if( x < _pScroll->GetX() ) PageDown();
	        break;
	        }
	        _RefreshMidst();
	        if( evtChange ) evtChange( this );
		}
		return true;
	}
	return false;
}

void CScroll::SetStyle( eStyle v )
{
	if( v == _eStyle ) return;

	_eStyle = v;
	Refresh();
}

void CScroll::_UpClick() 
{
	if( _step.Sub() )
	{
		_RefreshMidst();
		if( evtChange ) evtChange( this );
	}
}

void CScroll::_DownClick() 
{
	if( _step.Add() )
	{
		_RefreshMidst();
		if( evtChange ) evtChange( this );
	}
}

void CScroll::FrameMove( DWORD dwTime )
{
    if( !GetIsEnabled() ) return;

	if(  _pContinueButton )
    {
        if( _dwStartTime==0 )
        {
            _dwStartTime = dwTime + 800;        // 
            return;
        }
        if( dwTime - _dwStartTime > 30 )
        {
            _dwStartTime = dwTime;
            if( _pContinueButton==_up )
            {
                _UpClick();
                return;
            }
            if( _pContinueButton==_down )
            {
                _DownClick();
                return;
            }
        }
        return;
    }
}

void CScroll::_RefreshMidst()
{
	switch( _eStyle )
	{
	case btVertical:
		_pScroll->SetPos( 0, _step.GetShowPosition() + _nStart );
	break;
	case btHorizontal:
		_pScroll->SetPos( _step.GetShowPosition() + _nStart, 0 );
	break;
	}
	_pScroll->Refresh();
}

void CScroll::_DragBegin()
{
	switch( _eStyle )
	{
	case btVertical:
		_nMinDrag = _up->GetY2() - _pScroll->GetY();
		_nMaxDrag = _down->GetY() - _pScroll->GetY2();
	break;
	case btHorizontal:
		_nMinDrag = _up->GetX2() - _pScroll->GetX();
		_nMaxDrag = _down->GetX() - _pScroll->GetX2();
	break;
	}
	_CheckScrollDrag();
}

void CScroll::_CheckScrollDrag()
{
	CDrag *pDrag = _pScroll->GetDrag();
	switch( _eStyle )
	{
	case btVertical:
		_pScroll->GetDrag()->SetDragX( 0 );
		if( pDrag->GetDragY() < _nMinDrag ) pDrag->SetDragY( _nMinDrag );
		if( pDrag->GetDragY() > _nMaxDrag ) pDrag->SetDragY( _nMaxDrag );
		_UpdataStep( pDrag->GetDragY() + _pScroll->GetY() );
	break;
	case btHorizontal:
		pDrag->SetDragY( 0 );
		if( pDrag->GetDragX() < _nMinDrag ) pDrag->SetDragX( _nMinDrag );
		if( pDrag->GetDragX() > _nMaxDrag ) pDrag->SetDragX( _nMaxDrag );
		_UpdataStep( pDrag->GetDragX() + _pScroll->GetX() );
	break;
	}
}

void CScroll::_UpdataStep( int v )
{
	switch( _eStyle )
	{
	case btVertical:
		_step.SetShowPosition( (float)( v - _nStart - GetY() ) );
	break;
	case btHorizontal:
		_step.SetShowPosition( (float)( v - _nStart - GetX() ) );			
	break;
	}
	if( evtChange ) evtChange( this );
}

bool CScroll::OnKeyDown( int key )	
{
	if( !GetIsEnabled() ) return false;

	switch( key )
	{
	case VK_HOME:
		_step.SetPosition( _step.GetMin() );
		break;
	case VK_END:
		_step.SetPosition( _step.GetMax() );
		break;
	case VK_PRIOR:	// pageup
		PageDown();
		break;
	case VK_NEXT:	// pagedown
		PageUp();
		break;
	case VK_UP:
		_step.Sub();
		break;
	case VK_DOWN:
		_step.Add();
		break;
	default:
		return false;
	}
	_RefreshMidst();
	if( evtChange ) evtChange( this );
	return true;
}

bool CScroll::MouseScroll( int nScroll )
{
	if( !IsNormal() ) return false;

	bool rt = false;
	if( nScroll < 0 ) rt = _step.Add( 1 );
	else if( nScroll > 0 ) rt = _step.Sub( 1 );
	if( rt )
	{
		_RefreshMidst();
		if( evtChange ) evtChange( this );
	}
	return true;
}

bool CScroll::SetRange( float min, float max )
{
    if( _step.SetRange( min, max ) )
    {
	    if( _IsAutoHide )
	    {
		    if( _step.GetRange()<=0 )
			    SetIsShow( false );
		    else
			    SetIsShow( true );
	    }

        return true;
    }
	return false;
}

void CScroll::SetAlpha( BYTE alpha )
{ 
	_pImage->SetAlpha(alpha); 
	_pScroll->SetAlpha(alpha); 
	_up->SetAlpha(alpha); 
	_down->SetAlpha(alpha); 	
}

void CScroll::Reset()
{
	_step.SetShowPosition( 0 );		
	_RefreshMidst();
}

void CScroll::RenderHint( int x, int y )
{
    char buf[80] = { 0 };
	if( _strHint.empty() )
	{
		sprintf( buf, "%.02f/%.02f/%.02f", _step.GetMin(), _step.GetPosition(), _step.GetMax() );
	}
	else
	{
		sprintf( buf, "%s:%.02f/%.02f/%.02f", _strHint.c_str(), _step.GetMin(), _step.GetPosition(), _step.GetMax() );
	}
    _RenderHint( buf, x, y );
}

CCompent* CScroll::GetHintCompent( int x, int y )
{
    if( GetIsShow() && InRect( x, y ) )
        return this;

    return NULL;
}

bool CScroll::StepMove(int val)
{


	return true;
}
