#include "StdAfx.h"
#include "uilist.h"

using namespace GUI;
//---------------------------------------------------------------------------
// class CSelectItem
//---------------------------------------------------------------------------
CSelectItem::CSelectItem() 
: _pItem(NULL), _isSelect(false), _nIndex(-1), _isEnabled(true)
, _pList(NULL)
{
	_pImage = new CGuiPic;
}

CSelectItem::CSelectItem( const CSelectItem& rhs )
{
	_pImage = new CGuiPic( *rhs._pImage );
	_Copy( rhs );
}

void CSelectItem::_Copy( const CSelectItem& rhs )
{
	_pItem = NULL;
	_isSelect = false;
	_nIndex = -1;
	_isEnabled = true;
	_pList = NULL;
}

CSelectItem& CSelectItem::operator=( const CSelectItem& rhs )
{
	*_pImage = *rhs._pImage;
	_Copy( rhs );
	return *this;
}

void CSelectItem::Render( int x, int y ) 
{ 
	if(_isSelect )
	{
		_pImage->Render( x, y );
	}
}

//---------------------------------------------------------------------------
// class CListItems
//---------------------------------------------------------------------------
int CListItems::_nTmpY;

CListItems::CListItems()
: _pSelect( new CSelectItem() ), _dwCount(0), _nItemLeft(0), _nItemTop(0)
, _dwFirstShowRow(0), _pList(NULL), _dwLastShowRow(0), _nShowCount(1000), _nMaxRow(0), _IsMouseFollow(false), _pItemPic(NULL)
, _nPicLeft(0), _nPicTop(0), _nPicX(0)
{
}

CListItems::CListItems( const CListItems& rhs )
: _pSelect( new CSelectItem(*rhs._pSelect) ), _dwCount(rhs._dwCount)
, _dwFirstShowRow(rhs._dwFirstShowRow), _pList(rhs._pList), _dwLastShowRow(rhs._dwLastShowRow)
, _nShowCount( rhs._nShowCount), _nX1( rhs._nX1 ), _nX2( rhs._nX2 ), _nY1( rhs._nY1 ), _nY2( rhs._nY2 )
, _nHeight(20), _nMaxRow(rhs._nMaxRow), _IsMouseFollow(rhs._IsMouseFollow)
{
	_pItemPic = _pList->GetItemImage();

	_Copy( rhs );
}

CListItems& CListItems::operator=( const CListItems& rhs )
{
	*_pSelect = *rhs._pSelect;
	_dwCount = rhs._dwCount;
	_dwFirstShowRow = rhs._dwFirstShowRow;
	_dwLastShowRow = rhs._dwLastShowRow;
	_pList = rhs._pList;

	_Copy( rhs );
	return *this;
}

void CListItems::_Copy(const CListItems &rhs)
{
	_items.clear();
	_dwCount = 0;

	_pList = NULL;
	_pItemPic = NULL;

	_nX1 = rhs._nX1;
	_nX2= rhs._nX2;
	_nY1 = rhs._nY1;
	_nY2 = rhs._nY2;

	_nHeight = rhs._nHeight;
	_nShowCount = rhs._nShowCount;
	_dwFirstShowRow = rhs._dwFirstShowRow;
	_dwLastShowRow = rhs._dwLastShowRow;
	_nMaxRow = rhs._nMaxRow;

	_nItemLeft = rhs._nItemLeft;
	_nItemTop = rhs._nItemTop;
	_nItemX = rhs._nItemX;

	_nPicLeft = rhs._nPicLeft;
	_nPicTop = rhs._nPicTop;
	_nPicX = rhs._nPicX;

	_IsMouseFollow = rhs._IsMouseFollow;
}

void CListItems::SetParent( CList* pList )				
{
	_pList = pList; 
	_pItemPic = _pList->GetItemImage();
	_pSelect->SetList( pList );
}

CListItems::~CListItems()
{
	Clear();
}

bool CListItems::Clone( CListItems* rhs ) 
{
	CListItems *p = dynamic_cast<CListItems*>(rhs);
	if( !p ) return false;				
	*this = *p;							
	return true;						
}

void CListItems::Clear()
{
	_pSelect->SetNoSelect();
	for( vitems::iterator it=_items.begin(); it!=_items.end(); it++ )
	{
		//delete *it;
		SAFE_DELETE(*it); // UI
	}

	_items.clear();

	_dwFirstShowRow = 0;
	_dwLastShowRow = 0;
	_dwCount = 0;
}

CItemRow* CListItems::GetItem( int index )
{
	if( index>=0 && index<(int)_dwCount )
	{
		return _items[index];
	}
	return NULL;
}

int CListItems::GetIndex( CItemRow *p)
{
	for ( DWORD i=0; i<_dwCount; i++ )
	{
		if ( _items[i] == p )
			return i ;
	}
	return  -1;

}

void CListItems::_AddRow( CItemRow* p )
{
	_items.push_back( p );
	_CheckMaxRow();
	_dwCount = (DWORD)_items.size();
}

CItemRow* CListItems::Add( const char* str, DWORD c  ) 
{ 
	CItemRow *p = new CItemRow( 1 );

	p->SetBegin( new CItem( str, c ) );
	_AddRow( p );
	return p;
}

CItemRow* CListItems::NewItem()								
{ 
	CItemRow* p = new CItemRow( GetColumn() ); 
	_AddRow( p );
	return p;
}

void CListItems::SetMaxRowNum( int n )
{
	if( n >= 0 )
	{
		_nMaxRow = n;
		if( n > 0 )  
		{
			int end = (int)_items.size();
			for( int i=n; i<end; i++ )
			{
				//delete _items[i];
				SAFE_DELETE(_items[i]); // UI
			}
			_items.erase( _items.begin() + n, _items.end() );		
			_dwCount = (DWORD)_items.size();

		}
	}
}

void CListItems::_CheckMaxRow()
{
	if( _nMaxRow>0 && ((int)_items.size()>=_nMaxRow) )
	{
		//delete _items.front();
		SAFE_DELETE(_items.front());
		_items.pop_front();
	}
}

bool CListItems::Del( CItemRow* item ) 
{ 
    vitems::iterator it = find( _items.begin(), _items.end(), item );
    if( it!=_items.end() )
    {
		if( _pSelect->GetItem() == *it ) _pSelect->SetNoSelect();

        //delete (*it);
		SAFE_DELETE(*it);	// UI

        _items.erase( it );
		_dwCount = (DWORD)_items.size();
        return true;
    }
    return false;
}

bool CListItems::MouseRun( int x, int y, DWORD key )
{
	if (x>=_nX1 && x<=_nX2 && y>=_nY1 && y<=_nY2)
	{	
		if( _IsMouseFollow || (key & Mouse_LDown) || (key & Mouse_LDB) || (key &Mouse_RDown) )
		{
			int h = ( (_dwLastShowRow - _dwFirstShowRow) * _nHeight + _nY1 );
			if( y >= h ) return false;

			// 
			int row = ( y - _nY1 ) / _nHeight + _dwFirstShowRow;

			if( _pSelect->GetIndex()!=row )
			{  
				_pSelect->SetSelect( _items[row], row );			
			}

			if( key & Mouse_LDown) 
			{
				if(	_pList->evtListMouseDown ) _pList->evtListMouseDown( _pList, x, y, key );
				if( _pList->evtSelectChange ) _pList->evtSelectChange( _pList );
			}
			if( key & Mouse_LDB )
			{
				if(	_pList->evtListMouseDB ) _pList->evtListMouseDB( _pList, x, y, key );
			}
			if( key & Mouse_RDown )
			{
				if(	_pList->evtListMouseDB ) _pList->evtListMouseDB( _pList, x, y, key );
			}
		}
		return true;
	}	
	return false;
}

void CListItems::Select( int n ) 
{
	if( !_items.empty() && n>=0 && n<(int)_items.size() )
	{
		_pSelect->SetSelect( _items[n], n );
		if( _pList->evtSelectChange ) _pList->evtSelectChange( _pList );
	}
}

void CListItems::Refresh()
{
	if( _nHeight > 0 ) _nShowCount = ( _nY2 - _nY1 ) / _nHeight;

	_dwLastShowRow = _dwFirstShowRow + _nShowCount;
	_dwLastShowRow = _dwLastShowRow > _dwCount ? _dwCount : _dwLastShowRow;
}

void CListItems::SetRect( int x1, int y1, int x2, int y2, int rowheight )
{
	_nX1 = x1;
	_nY1 = y1;
	_nX2 = x2;
	_nY2 = y2;

	_nPicX = _nX1 +   _nPicLeft;
	_nItemX = _nX1 +  _nItemLeft;
	
	_pSelect->SetSize( x2-x1, rowheight );
	_nHeight = rowheight;
}

void CListItems::Render() 
{	
	if( _pSelect->GetIsEnabled() && _pSelect->GetIsSelect() && _pSelect->GetIndex()>=(int)_dwFirstShowRow && _pSelect->GetIndex()<(int)_dwLastShowRow )
	{
		_pSelect->Render( _nX1, (_pSelect->GetIndex() - _dwFirstShowRow)*_nHeight+_nY1 );
	}

	_nTmpY = _nY1;
	if( _pItemPic->IsNull() )
	{
		for( unsigned int i=_dwFirstShowRow; i<_dwLastShowRow; i++ )
		{
			if(0 <= i && i < _items.size() && _items[i])
			{
				_items[i]->GetBegin()->Render( _nItemX, _nTmpY + _nItemTop );
				_nTmpY += _nHeight;
			}
		}
	}
	else
	{
		for( unsigned int i=_dwFirstShowRow; i<_dwLastShowRow; i++ )
		{
			if(0 <= i && i < _items.size() && _items[i])
			{
				_pItemPic->Render( _nPicX, _nTmpY + _nPicTop );
				_items[i]->GetBegin()->Render( _nItemX, _nTmpY + _nItemTop );
				_nTmpY += _nHeight;
			}
		}
	}
}

//---------------------------------------------------------------------------
// class CListItemsEx
//---------------------------------------------------------------------------
CListItemsEx::CListItemsEx( unsigned int nCol)
: CListItems(), _nCol(nCol)
{
	_nColWidth = new unsigned int[_nCol];
	_nColX = new int[_nCol];

	for( unsigned int i=0; i<_nCol; i++ )
		_nColWidth[i] = 100;
}

CListItemsEx::CListItemsEx( const CListItemsEx& rhs )
: CListItems( rhs ), _nCol(rhs._nCol)
{
	_nColWidth = new unsigned int[_nCol];
	_nColX = new int[_nCol];

	_SetSelf( rhs );
}

bool CListItemsEx::Clone( CListItems* rhs ) 
{
	CListItemsEx *p = dynamic_cast<CListItemsEx*>(rhs);
	if( !p ) return false;				
	*this = *p;							
	return true;						
}

CListItemsEx& CListItemsEx::operator=( const CListItemsEx& rhs )
{
	CListItems::operator=(rhs);

	if( _nCol != rhs._nCol )
	{
		//delete [] _nColWidth;
		SAFE_DELETE_ARRAY(_nColWidth); // UI
		_nCol = rhs._nCol;
		_nColWidth = new unsigned int[_nCol];

		//delete [] _nColX;
		SAFE_DELETE_ARRAY(_nColX); // UI
		_nColX = new int[_nCol];
	}

	_SetSelf( rhs );
	return *this;
}

void CListItemsEx::_SetSelf(const CListItemsEx &rhs)
{
	for( unsigned int i=0; i<_nCol; i++ )
	{
		_nColWidth[i] = rhs._nColWidth[i];
		_nColX[i] = rhs._nColX[i];
	}
}

void CListItemsEx::_RefreshColX()
{
	_nColX[0] = _nX1;
	for(unsigned int j=1; j<_nCol; j++ ) 
	{
		_nColX[j] = _nColX[j-1] + _nColWidth[j-1];
	}
}
int CListItemsEx::_GetColumnWidth( unsigned int nCol ) 
{
	if( nCol < _nCol ) return _nColWidth[nCol];
	return 0;
}

CListItemsEx::~CListItemsEx()
{
	//delete [] _nColX;
	//delete [] _nColWidth;

	SAFE_DELETE_ARRAY(_nColX);     // UI
	SAFE_DELETE_ARRAY(_nColWidth); // UI
}

void CListItemsEx::Clear()
{
	CListItems::Clear();
}

void CListItemsEx::SetRect( int x1, int y1, int x2, int y2, int rowheight )
{
	CListItems::SetRect( x1, y1, x2, y2, rowheight );
	_RefreshColX();
}

CItemRow* CListItemsEx::Add( const char* str, DWORD c ) 
{ 
	CItemRow *p = new CItemRow( _nCol );
	p->SetBegin( new CItem(str, c) );

	_AddRow( p );
	return p;
}

void CListItemsEx::Render()
{
	if( _pSelect->GetIsEnabled() && _pSelect->GetIsSelect() && _pSelect->GetIndex()>=(int)_dwFirstShowRow && _pSelect->GetIndex()<(int)_dwLastShowRow )
	{
		_pSelect->Render( _nX1, (_pSelect->GetIndex() - _dwFirstShowRow)*_nHeight+_nY1 );
	}

	_nTmpY = _nY1;
	CItemRow* r = NULL;
	if( _pItemPic->IsNull() )
	{
		for( unsigned int i=_dwFirstShowRow; i<_dwLastShowRow; i++ )
		{
			r = _items[i];
			for(unsigned int j=0; j<_nCol; j++ )
			{
				r->GetIndex(j)->Render( _nColX[j] + _nItemLeft, _nTmpY + _nItemTop );
			}
			_nTmpY += _nHeight;
		}
	}
	else
	{
		for( unsigned int i=_dwFirstShowRow; i<_dwLastShowRow; i++ )
		{
			_pItemPic->Render( _nX1, _nTmpY );
			r = _items[i];
			for(unsigned int j=0; j<_nCol; j++ )
			{
				r->GetIndex(j)->Render( _nColX[j] + _nItemLeft, _nTmpY + _nItemTop );
			}
			_nTmpY += _nHeight;
		}
	}
}

bool CListItemsEx::SetColumnWidth( unsigned int nCol, unsigned int width )
{
	if( nCol < _nCol )
	{
		_nColWidth[nCol] = width;
		return true;
	}
	return false;
}

//---------------------------------------------------------------------------
// class CList
//---------------------------------------------------------------------------
CList::CList(CForm& frmOwn)
: CCompent(frmOwn), _nRowHeight(22), _nSpaceBottom(0), _dwFontColor(COLOR_BLACK), _dwSelectColor(0xFF00FF71)
, _nLeftMargin(5), _nTopMargin(5), _nRightMargin(5), _nBottomMargin(5), _IsChangeColor(true)
, evtSelectChange(NULL), evtListMouseDown(NULL), evtListMouseDB(NULL)
{
	_IsFocus = false;

	_pImage = new CGuiPic( this );
	_pItemImage = new CGuiPic( this );
	_pScroll = new CScroll( frmOwn );
	_pItems = new CListItems();

	_SetSelf();
}

CList::CList(CForm& frmOwn, int nCol)
: CCompent(frmOwn), _nRowHeight(22), _nSpaceBottom(0), _dwFontColor(COLOR_BLACK), _dwSelectColor(0xFF00FF71)
, _nLeftMargin(5), _nTopMargin(5), _nRightMargin(5), _nBottomMargin(5), _IsChangeColor(true)
, evtSelectChange(NULL), evtListMouseDown(NULL), evtListMouseDB(NULL)
{
	_pImage = new CGuiPic( this );
	_pItemImage = new CGuiPic;

	_pScroll = new CScroll( frmOwn );

	if( nCol > 1 )
		_pItems = new CListItemsEx( nCol );
	else
		_pItems = new CListItems();

	_SetSelf();
}

CList::CList( const CList& rhs )
: CCompent(rhs), _pImage( new CGuiPic( *rhs._pImage ) ), _pScroll( new CScroll( *rhs._pScroll ) )
, _pItems( new CListItems( *rhs._pItems ) ), _pItemImage( new CGuiPic( *rhs._pItemImage ) )
{
	_Copy( rhs );

	_SetSelf();
}

void CList::_SetSelf()
{
	_pScroll->SetIsShow( false );
	_pScroll->SetParent( this );
	_pScroll->evtChange = _ScrollChange;

	_pItems->SetParent( this );
	_pImage->SetParent( this );
}

CList& CList::operator=(const CList& rhs)
{
	CCompent::operator =( rhs );

	*_pScroll = *(rhs._pScroll);
	_pItems->Clone( rhs._pItems );
	*_pImage = *(rhs._pImage);
	*_pItemImage = *(rhs._pItemImage);

	_Copy( rhs );
	_SetSelf();
	return *this;
}

void CList::_Copy( const CList& rhs )
{
	_nSpaceBottom = rhs._nSpaceBottom;
	_nRowHeight = rhs._nRowHeight;

	_nLeftMargin = rhs._nLeftMargin;
	_nTopMargin = rhs._nTopMargin;
	_nRightMargin = rhs._nRightMargin;
	_nBottomMargin = rhs._nBottomMargin;

	_dwFontColor = rhs._dwFontColor;
	_dwSelectColor = rhs._dwSelectColor;
	_IsChangeColor = rhs._IsChangeColor;

	evtSelectChange = rhs.evtSelectChange;
	evtListMouseDown = rhs.evtListMouseDown;
	evtListMouseDB = rhs.evtListMouseDB;	
}

CList::~CList(void)
{
	//delete _pItems;
	//delete _pImage;
	//delete _pItemImage;

	SAFE_DELETE(_pItems);     // UI
	SAFE_DELETE(_pImage);     // UI
	SAFE_DELETE(_pItemImage); // UI
}

void CList::Init()
{
	_SetSelf();

	_pScroll->SetSize( _pScroll->GetWidth(), GetHeight() );
	_pScroll->SetPos( GetWidth() - _pScroll->GetWidth(), 0 );
	_pScroll->Init();

	_ResetPageNum();
	_pItems->SelectFirst();
}

void CList::Render()
{	
	_pImage->Render( GetX(), GetY() );
	if( _pScroll->IsNormal() ) _pScroll->Render();
	_pItems->Render();
}

void CList::Refresh()
{
	CCompent::Refresh();

    _pScroll->SetSize( _pScroll->GetWidth(), GetHeight() );
	
    _ResetPageNum();
	_pScroll->Refresh();
	_pImage->Refresh();

	_pItems->SetRect( GetX() + _nLeftMargin, GetY() + _nTopMargin, GetX2() - _pScroll->GetWidth() - _nRightMargin, GetY2() - _nBottomMargin, _nRowHeight );
	_pItems->Refresh();

	_SetScrollRange();
}

bool CList::MouseRun( int x, int y, DWORD key )
{
	if( !IsNormal() ) return false;
	
	if( InRect( x, y ) )
	{
		if( (key & Mouse_LDown) && !_isChild && GetActive()!=this ) _SetActive();

		if( _pItems->MouseRun( x, y, key ) )  return true;		
		if( _pScroll->MouseRun( x, y, key ) ) return true;
		
		return true;
	}

	return _IsMouseIn;
}

CItemRow* CList::NewItem()
{
	CItemRow* item = _pItems->NewItem();
	_SetScrollRange();
	return item;
}

CItemRow* CList::Add( const char* str )
{
	CItemRow* item = _pItems->Add( str, _dwFontColor );

	_SetScrollRange();
	return item;
}

bool CList::Del( CItemRow* item )
{
	if( _pItems->Del( item ) )
	{
		_SetScrollRange();
		return true;
	}
	return false;
}
void CList::Clear()
{
	if(_pItems)
	{
		_pItems->Clear();
	}
}

void CList::_OnScrollChange()
{
	_pItems->SetFirstShowRow( (DWORD)_pScroll->GetStep().GetPosition() );
}

void CList::ResetScrollChange()
{
	_pItems->SetFirstShowRow(0);
}

bool CList::OnKeyDown( int key )	
{
	if( !GetIsEnabled() ) return false;	

	_pScroll->OnKeyDown( key );

	switch( key )
	{
	case VK_LEFT:
	case VK_UP:
		_pItems->SelectPrior();
		break;
	case VK_RIGHT:
	case VK_DOWN:
		_pItems->SelectNext();
		break;
	case VK_HOME:
		_pItems->SelectFirst();
		break;
	case VK_END:
		_pItems->SelectLast();
		break;
	case VK_PRIOR:	// pageup
		_pItems->SelectAdd( 0 - _pScroll->GetPageNum() );
		break;
	case VK_NEXT:	// pagedown
		_pItems->SelectAdd( _pScroll->GetPageNum() );
		break;
	default:
		return false;
	}
	return true;
}

bool CList::MouseScroll( int nScroll )
{
	if( !IsNormal() ) return false;
	
	if( _IsMouseIn ) _pScroll->MouseScroll( nScroll );
	return _IsMouseIn;
}

void CList::SetAlpha( BYTE alpha )
{
	_pImage->SetAlpha(alpha); 
	_pScroll->SetAlpha(alpha); 
}

bool CList::SetShowRow( int n )
{
	if( n > 0 )
	{
		if( _nSpaceBottom>=n ) _nSpaceBottom = n-1;
		
		SetSize( GetWidth(), n * _nRowHeight + _nTopMargin + _nBottomMargin );
		_pScroll->SetSize( _pScroll->GetWidth(), GetHeight() );
		_ResetPageNum();
		_pScroll->Init();
		return true;
	}

	return false;
}

void CList::_SetScrollRange()
{	
	_pScroll->SetRange( 0.0, (float)_pItems->GetCount() - (float)_pItems->GetShowCount() + (float)_nSpaceBottom );
	_pScroll->Refresh();
	_pItems->Refresh();
}

void CList::OnActive() 
{ 
	CCompent::OnActive(); 
}

void CList::OnLost() 
{ 
	CCompent::OnLost(); 
}

void CList::SetMargin( int left, int top, int right, int bottom ) 
{ 
	_nLeftMargin = left;
	_nTopMargin = top;
	_nRightMargin= right;
	_nBottomMargin = bottom; 
}
