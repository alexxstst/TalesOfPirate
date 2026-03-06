#include "StdAfx.h"
#include "uiimage.h"

using namespace GUI;

CImage::CImage(CForm& frmOwn, unsigned int max)
: CCompent(frmOwn), _pImage(NULL), evtMouseDown(NULL)
{
	_pImage = new CGuiPic( this, max );
}

CImage::CImage( const CImage& rhs )
: CCompent(rhs), _pImage( new CGuiPic( *rhs._pImage) )
{
}

CImage& CImage::operator=(const CImage& rhs)
{
	CCompent::operator =( rhs );

	*_pImage = *(rhs._pImage);
	return *this;
}

CImage::~CImage(void)
{
	//delete _pImage;
	SAFE_DELETE(_pImage);	// UI
}

void CImage::Render()
{	
	if( CDrag::IsDraging(this) )
	{
		//  
		_pImage->Render( GetX() + CDrag::GetDrag()->GetDragX(), GetY() + CDrag::GetDrag()->GetDragY(), DROP_ALPHA );
		return;
	}
	else
		_pImage->Render( GetX(), GetY() );
}

void CImage::Refresh()
{
	CCompent::Refresh();

	_pImage->Refresh();
}

bool CImage::MouseRun( int x, int y, DWORD key )
{
	if( !IsNormal() ) return false;

	if( IsNoDrag( x, y, key ) )
	{
		if( (key & Mouse_LDown) )
		{
			if( !_isChild && GetActive()!=this ) _SetActive();
			if( evtMouseDown ) evtMouseDown( this, x, y, key );
		}
		return true;
	}
	return _IsMouseIn;
}

//---------------------------------------------------------------------------
// class CFrameImage
//---------------------------------------------------------------------------
CFrameImage::CFrameImage(CForm& frmOwn)
: CCompent(frmOwn), _pImage(NULL), evtMouseDown(NULL)
{
	_pImage = new CFramePic( this );
}

CFrameImage::CFrameImage( const CFrameImage& rhs )
: CCompent(rhs), _pImage( new CFramePic( *rhs._pImage) )
{
}

CFrameImage& CFrameImage::operator=(const CFrameImage& rhs)
{
	CCompent::operator =( rhs );

	*_pImage = *(rhs._pImage);
	return *this;
}

CFrameImage::~CFrameImage(void)
{
	//delete _pImage;
	SAFE_DELETE(_pImage); // UI
}

void CFrameImage::Render()
{	
	if( !GetIsShow() ) return;

	if( CDrag::IsDraging(this) )
	{
		//  
		_pImage->Render( GetX() + CDrag::GetDrag()->GetDragX(), GetY() + CDrag::GetDrag()->GetDragY(), DROP_ALPHA );
		return;
	}
	else
		_pImage->Render();
}

void CFrameImage::Refresh()
{
	CCompent::Refresh();

	_pImage->Refresh();
}

bool CFrameImage::MouseRun( int x, int y, DWORD key )
{
	if( !IsNormal() ) return false;

	if( IsNoDrag( x, y, key ) )
	{		
		if( (key & Mouse_LDown) )
		{
			if( !_isChild && GetActive()!=this ) _SetActive();
			if( evtMouseDown ) evtMouseDown( this, x, y, key );
		}
		return true;
	}
	return _IsMouseIn;
}

//-------------------------------------------------------------------------
// CFlashImage
//-------------------------------------------------------------------------
CFlashImage::CFlashImage(CForm& frmOwn, unsigned int max)
: CCompent(frmOwn), _pImage(NULL), _eStyle(enumRightMove), _dwInterval(1)
{
	_pImage = new CGuiPic( this, max );
}

CFlashImage::CFlashImage( const CFlashImage& rhs )
: CCompent(rhs), _pImage( new CGuiPic( *rhs._pImage) )
{
	_Copy( rhs );
}

CFlashImage& CFlashImage::operator=(const CFlashImage& rhs)
{
	CCompent::operator =( rhs );

	*_pImage = *(rhs._pImage);

	_Copy( rhs );
	return *this;
}

CFlashImage::~CFlashImage(void)
{
	//delete _pImage;
	SAFE_DELETE(_pImage); // UI
}

void CFlashImage::Init()
{
	_dwLastTime = 0;

	if( _eStyle==enumRightMove )
	{
		_pImage->SetIsScale( false );

		_nMoveWidth = int( GetWidth() - _pImage->GetWidth() );
		_nRightMoveX = 0;
		_fPos = 0.0f;

		_nImageStart = _pImage->GetImage()->nTexSX;
	}
}

void CFlashImage::_Copy( const CFlashImage& rhs )
{
	_eStyle = rhs._eStyle;
	_dwInterval = rhs._dwInterval;
}

void CFlashImage::Render()
{	
	switch( _eStyle )
	{
	case enumRightMove:
		_pImage->Render( GetX() + _nRightMoveX, GetY() );
		break;
	default:
		_pImage->Render( GetX(), GetY() );
		break;
	}
}

void CFlashImage::FrameMove( DWORD dwTime )
{
	if( dwTime - _dwLastTime < _dwInterval ) return;

	_dwLastTime = dwTime;
	switch( _eStyle )
	{
	case enumFlash:
		_bShow = !_bShow;
		break;
	case enumCartoon:
		_pImage->Next();
		break;
	case enumRightMove:
		_fPos += 0.2f;
		if( _fPos>1.0f ) _fPos=0.0f;

		_nRightMoveX = int(_nMoveWidth * _fPos);
		 _pImage->GetImage()->nTexSX = _nImageStart + _nRightMoveX;
		break;
	}
}

void CFlashImage::Refresh()
{
	CCompent::Refresh();

	_pImage->Refresh();
}

