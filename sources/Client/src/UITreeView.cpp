#include "StdAfx.h"
#include "uitreeview.h"
#include "uiformmgr.h"

// Add by lark.li 20080805 begin
#include "uigraph.h"
// End

using namespace GUI;
//---------------------------------------------------------------------------
// class CSelectItem
//---------------------------------------------------------------------------
bool CTreeSelectItem::SetItem( CTreeNodeObj* parent, CItemObj* item )
{
	if( _pItem==item ) 
	{
		_SetParent( parent );
		return false;
	}

	if( _pItem && _pOwn->evtItemLost ) _pOwn->evtItemLost(_pOwn);

	_pItem = item;
	if( _pItem && _pOwn->evtItemChange ) _pOwn->evtItemChange(_pOwn);
	_SetParent( parent );
	return true;
}

bool CTreeSelectItem::SetDifferItem( CTreeNodeObj* parent, CItemObj* item )
{
	if( _pItem==item ) 
	{
		if( _pItem && _pOwn->evtItemLost ) _pOwn->evtItemLost(_pOwn);
		_pItem = NULL;

		_SetParent( parent );
		return false;
	}

	if( _pItem && _pOwn->evtItemLost ) _pOwn->evtItemLost(_pOwn);

	_pItem = item;
	if( _pItem && _pOwn->evtItemChange ) _pOwn->evtItemChange(_pOwn);

	_SetParent( parent );
	return true;
}

bool CTreeSelectItem::_SetParent( CTreeNodeObj *node )
{
	if( _pParent!=node )
	{
		if( _pParent && _pOwn->evtSelectLost ) _pOwn->evtSelectLost(_pOwn);
		_pParent = node;
		if( _pOwn->evtSelectChange ) _pOwn->evtSelectChange(_pOwn);
		return true;
	}
	return false;
}

//---------------------------------------------------------------------------
// class CTreeNodeObj
//---------------------------------------------------------------------------
TreeNodes CTreeNodeObj::_nodes;

CTreeNodeObj::~CTreeNodeObj() 
{
	ClearAllChild();
}

bool CTreeNodeObj::ClearAllChild()
{
	_pOwn->GetSelect()->CancelSelect();
	for( TreeNodes::iterator it=_ndChilds.begin(); it!=_ndChilds.end(); ++it )
	{
		//delete *it;
		SAFE_DELETE(*it); // UI
	}

	_ndChilds.clear();
	return true;
}

bool CTreeNodeObj::DelNode( CTreeNodeObj* pNode )
{
	for( TreeNodes::iterator it=_ndChilds.begin(); it!=_ndChilds.end(); ++it )
	{
		if( (*it)->DelNode( pNode ) )
		{
			return true;
		}
		else if( *it==pNode )
		{
			if( _pOwn->GetSelectNode()==(*it) )
			{
				_pOwn->GetSelect()->CancelSelect();
			}
			//delete *it;
			SAFE_DELETE(*it); // UI
			
			_ndChilds.erase( it );
			return true;
		}
	}

	return false;
}

void CTreeNodeObj::Render()
{
	RenderSelf();
	if( _HasPage() )
	{
		if( GetIsExpand() )
		{
			_pOwn->RenderSubImage( _nX, _nY );
		}
		else
		{
			_pOwn->RenderAddImage( _nX, _nY );
		}
	}

	if( HasChild() && GetIsExpand() )
	{
		TreeNodes::iterator end = GetChild().end();
		for( TreeNodes::iterator it=GetChild().begin(); it!=end; ++it )
		{
			(*it)->Render();
		}
	}
}

int CTreeNodeObj::GetTreeHeight( int& height, int rowheight )
{
	height += GetHeight() + rowheight;
	if( HasChild() && GetIsExpand() )
	{
		TreeNodes::iterator end = GetChild().end();
		for( TreeNodes::iterator it=GetChild().begin(); it!=end; ++it )
		{
			(*it)->GetTreeHeight( height, rowheight );
		}
	}
	return height;
}

void CTreeNodeObj::Refresh(int x, int& y, int colwidth, int rowheight ) 
{
	_nX=x; 
	_nY=y;
	_nX2 = x + GetWidth();
	_nY2 = y + GetHeight();
	y = _nY2 + rowheight;

	if( HasChild() && GetIsExpand() )
	{
		x += colwidth;
		//y += GetHeight() + rowheight;
		TreeNodes::iterator end = GetChild().end();
		for( TreeNodes::iterator it=GetChild().begin(); it!=end; ++it )
		{
			(*it)->Refresh( x, y, colwidth, rowheight );
			//y += ((*it)->GetHeight() + rowheight);
		}
	}
}

bool CTreeNodeObj::MouseRun( int x, int y, DWORD key )
{
	if( InRect( x, y ) ) 
	{
		MouseRunSelf(x,y,key);
		return true;
	}

	if( _HasPage() && _pOwn->InAddImage(x,y,_nX,_nY) )
	{
		_IsExpand = !_IsExpand;
		CWaitCursor lock;
		_pOwn->Refresh();

		//  +/- +32  +/- 
		if(_pOwn->evtMouseDown) _pOwn->evtMouseDown(_pOwn, x + 32, y, key);

		return false;
	}

	if( HasChild() && GetIsExpand() )
	{	
		TreeNodes::iterator end = GetChild().end();
		for( TreeNodes::iterator it=GetChild().begin(); it!=end; ++it )
		{
			if( (*it)->MouseRun(x,y,key) )
				return true;
		}
	}

	return false;
}

bool CTreeNodeObj::IsShow()
{
	CTreeNodeObj *p = _pParent;
	while ( p )
	{
		if( !p->GetIsExpand() ) return false;
		p = p->GetParent();
	}
	return true;
}

bool CTreeNodeObj::AddNode( CTreeNodeObj* obj ) 
{ 
	obj->_pParent = this;
	_ndChilds.push_back(obj);
	return true;
}

CTreeNodeObj* CTreeNodeObj::GetHitNode( int x, int y )
{
	if( InRect( x,y ) )
	{
		return this;
	}
	else if( HasChild() && GetIsExpand() )
    {
		CTreeNodeObj* pRV = NULL;
		TreeNodes::iterator end = GetChild().end();
		for( TreeNodes::iterator it=GetChild().begin(); it!=end; ++it )
		{
            if( pRV = (*it)->GetHitNode( x, y ) )
            {
				return pRV;
            }
		}
    }
    return NULL;
}

CItemObj* CTreeNodeObj::GetHitItem( int x, int y )
{
    if( HasChild() && GetIsExpand() )
    {
        CItemObj* pObj = NULL;
		TreeNodes::iterator end = GetChild().end();
		for( TreeNodes::iterator it=GetChild().begin(); it!=end; ++it )
		{
            if( (*it)->InRect( x, y ) && (pObj=(*it)->_GetSelfHitItem(x,y)) )
            {
				return pObj;
            }
            else if( pObj=(*it)->GetHitItem(x, y) )
            {
				return pObj;
            }
		}
    }
    return NULL;
}

CTreeNodeObj* CTreeNodeObj::FindNode( const char* str )
{
    if( strcmp( str, GetCaption() )==0 )
    {
        return this;
    }

    if( GetChild().empty() ) return NULL;

    TreeNodes::iterator end = GetChild().end();
    for( TreeNodes::iterator it=GetChild().begin(); it!=end; ++it )
    {
        if( strcmp( str, (*it)->GetCaption() )==0 )
            return *it;
    }

    return NULL;
}

//---------------------------------------------------------------------------
// class CTreeNodeRoot
//---------------------------------------------------------------------------
void CTreeNodeRoot::RootRender()
{
	if( HasChild() )
	{
		TreeNodes::iterator end = GetChild().end();
		for( TreeNodes::iterator it=GetChild().begin(); it!=end; ++it )
		{
			(*it)->Render();
		}
	}
}

void CTreeNodeRoot::RootRefresh(int x, int& y, int colwidth, int rowheight ) 
{
	_nX = x; 
	_nY = y;
	_nX2 = x;
	_nY2 = y;

	if( HasChild() )
	{
		TreeNodes::iterator end = GetChild().end();
		for( TreeNodes::iterator it=GetChild().begin(); it!=end; ++it )
		{
			(*it)->Refresh( x, y, colwidth, rowheight );
		}
	}
}

bool CTreeNodeRoot::RootMouseRun( int x, int y, DWORD key )
{
	if( HasChild() )
	{
		TreeNodes::iterator end = GetChild().end();
		for( TreeNodes::iterator it=GetChild().begin(); it!=end; ++it )
		{
			if( (*it)->MouseRun(x,y,key) )
				return true;
		}
	}

	return false;
}

//---------------------------------------------------------------------------
// class CTreeNode
//---------------------------------------------------------------------------
void CTreeNode::RenderSelf()
{ 
	if( _pOwn->InTree( _nX, _nY, _nX2, _nY2 ) )
		_pItem->Render( _nX, _nY ); 
}

void CTreeNode::MouseRunSelf( int x, int y, DWORD key )
{
	if( !_pOwn->GetSelect()->SetItem( this, _pItem ) && _pOwn->GetIsAutoExpand() )
	{
		_IsExpand = !_IsExpand;
		_pOwn->Refresh();
	}
}

// xuqin added
void CTreeNodeObj::GetNumItems( int& Items )
{
	Items ++;
	if( HasChild() && GetIsExpand() )
	{
		TreeNodes::iterator end = GetChild().end();
		for( TreeNodes::iterator it=GetChild().begin(); it!=end; ++it )
		{
			(*it)->GetNumItems( Items );
		}
	}
}

//---------------------------------------------------------------------------
// class CTreeGridNode
//---------------------------------------------------------------------------

CTreeGridNode::CTreeGridNode(CTreeView *own, CItemObj* item) 
: CTreeNodeObj(own), _pItem(item)
, _nUnitWidth(32), _nUnitHeight(32)
, _nColMaxNum(3), _nRowNum(0) 
{
	_nFocusX1 = 0;
	_nFocusX2 = 0;
	_nFocusY1 = 0;
	_nFocusY2 = 0;
	_nFocusCol = 0;
	_nFocusRow = 0;
	_IsShowFocus = false;

	GetUpImage()->LoadImage( "texture/editor/frame01.tga", 32, 32, 0, 0, 0 );
	GetDownImage()->LoadImage( "texture/editor/frame02.tga", 32, 32, 0, 0, 0 );
}

void CTreeGridNode::SetUnitSize( int w, int h ) 
{ 
	if( w>0 && h>0 ) 
	{
		_nUnitWidth=w;
		_nUnitHeight=h;

		GetUpImage()->SetScaleW( h );	
		GetUpImage()->SetScaleH( h );	
		GetDownImage()->SetScaleW( h );	
		GetDownImage()->SetScaleH( h );	
	}
}

CTreeGridNode::~CTreeGridNode()
{
    for( vitems::iterator it=_items.begin(); it!=_items.end(); ++it )
	{
        //delete *it;
		SAFE_DELETE(*it); // UI
	}

    _items.clear();
}

void CTreeGridNode::RefreshNode()
{
	_nFocusX1 = _nFocusCol * _nUnitWidth + _nX;
	_nFocusX2 = _nFocusX1 + _nUnitWidth;
	_nFocusY1 = _nFocusRow * _nUnitHeight + _nY + _pItem->GetHeight();
	_nFocusY2 = _nFocusY1 + _nUnitHeight;
}

int CTreeGridNode::GetHeight() 
{ 
	if( _IsExpand )
		return _pItem->GetHeight() + _nRowNum * _nUnitHeight; 
	else
		return _pItem->GetHeight();
}

CItemObj* CTreeGridNode::_GetSelfHitItem( int x, int y ) 
{ 
	if( y >= _nY && y <= _nY + _pItem->GetHeight() )
    {
        return _pItem;                    
    }
    else
    {
		int col = ( x - _nX ) / _nUnitWidth;
		int row = ( y - _nY - _pItem->GetHeight() ) / _nUnitHeight;
		int index = row * _nColMaxNum + col;

		if( index < (int)_items.size() )
		{
            return _items[ index ];
		}
    }
    return NULL;
}

void CTreeGridNode::MouseRunSelf( int x, int y, DWORD key )
{
	if( y >= _nY && y <= _nY + _pItem->GetHeight() )
	{
		_IsShowFocus = false;
		if( !_pOwn->GetSelect()->SetItem( this, _pItem ) && _pOwn->GetIsAutoExpand() ) 
		{
			_IsExpand = !_IsExpand;
			_pOwn->Refresh();
		}
	}
	else
	{
		int col = ( x - _nX ) / _nUnitWidth;
		int row = ( y - _nY - _pItem->GetHeight() ) / _nUnitHeight;
		int index = row * _nColMaxNum + col;

		if( index < (int)_items.size() )
		{
			if( _pOwn->GetSelect()->SetDifferItem( this, _items[ index ] ) )
			{
				_nFocusCol = col;
				_nFocusRow = row;
				RefreshNode();
			}
		}
		_IsShowFocus = true;
	}
}

void CTreeGridNode::RenderSelf()
{ 
	if( _pOwn->InTree( _nX, _nY, _nX2, _nY + _pItem->GetHeight() ) )
		_pItem->Render( _nX, _nY ); 

	if( _IsExpand && !_items.empty() )
	{
		int col = 0;
		int row = 0;
		int x = _nX;
		int y = _nY + _pItem->GetHeight();
		for( vitems::iterator it=_items.begin(); it!=_items.end(); it++ )
		{
			if( _pOwn->InTree( x, y, x + _nUnitWidth, y + _nUnitHeight ) ) 
			{
				(*it)->Render( x, y );
				_imgUp.Render( x, y );
			}

			col++;
			x += _nUnitWidth;
			if( col>=_nColMaxNum )
			{
				++row;
				col=0;
				x = _nX;
				y += _nUnitHeight;
			}
		}
	}
}
 
bool CTreeGridNode::AddItem( CItemObj* item )
{
	vitems::iterator it = find( _items.begin(), _items.end(), item );
	if( it==_items.end() )
	{
		_items.push_back(item);
		_CaclRowNum();
		return true;
	}
	return false;
}

void CTreeGridNode::Clear()
{
    for( vitems::iterator it=_items.begin(); it!=_items.end(); ++it )
	{
        //delete *it;
		SAFE_DELETE(*it); // UI
	}

    _items.clear();

    _CaclRowNum();
}

bool CTreeGridNode::DelItem( CItemObj* item )
{
	vitems::iterator it = find( _items.begin(), _items.end(), item );
	if( it!=_items.end() )
	{
		if( _pOwn->GetSelect()->GetItem()==*it )
		{
			_pOwn->GetSelect()->CancelSelect();
		}
		//delete *it;
		SAFE_DELETE(*it); // UI

		_pOwn->DelItemCall( *it );
		_items.erase(it);

		if( _pOwn->GetSelect()->GetItem() )
		{
			int count = (int)_items.size();
			CItemObj* p = _pOwn->GetSelect()->GetItem();
			int col = 0;
			int row = 0;
			for( int i=0; i<count; i++ )
			{
				if( p==_items[i] )
				{
					_nFocusCol = i % _nColMaxNum;
					_nFocusRow = i / _nColMaxNum;
					RefreshNode();
					break;
				}
			}
		}
		_CaclRowNum();
		return true;
	}
	return false;
}

CItemObj* CTreeGridNode::FindItem( CItemObj* item )
{
	vitems::iterator it = find( _items.begin(), _items.end(), item );
	if( it!=_items.end() )
	{
		return *it;
	}
	return NULL;
}

bool CTreeGridNode::DelItem( const char* str )
{
    for( vitems::iterator it=_items.begin(); it!=_items.end(); ++it )
    {
        if( strcmp( str, (*it)->GetString() ) == 0 )
        {
			if( _pOwn->GetSelect()->GetItem()==*it )
			{
				_pOwn->GetSelect()->CancelSelect();
			}
			//delete *it;
			SAFE_DELETE(*it); // UI

			_pOwn->DelItemCall( *it );
		    _items.erase(it);
		    _CaclRowNum();
		    return true;
        }
    }
    return false;
}

//---------------------------------------------------------------------------
// class CTreeView
//---------------------------------------------------------------------------
CTreeView::CTreeView(CForm& frmOwn)
: CCompent(frmOwn), _nColSpace(4), _nRowSpace(5), _pRoot(NULL), _IsAutoExpand(true)
, evtSelectChange(NULL), evtMouseDown(NULL), evtSelectLost(NULL)
, evtItemLost(NULL), evtItemChange(NULL), evtMouseDragEnd(NULL)
, _pDragItem(NULL), _pDragNode(NULL), evtMouseDB(NULL), _dwSelectColor(COLOR_BLACK)
{
	// _IsFocus = true;

	_pSelectItem = new CTreeSelectItem(this);
	_pImage = new CGuiPic( this );
	_pAddImage = new CGuiPic;
	_pSubImage = new CGuiPic;
	_pScroll = new CScroll( frmOwn );

	_pScroll->SetParent(this);
	_pScroll->evtChange = _ScrollChange;

	_pRoot = new CTreeNodeRoot(this);

	GetAddImage()->LoadImage( "texture/ui/tree/prefix.tga", 16, 16, 0, 0, 0  );
	GetSubImage()->LoadImage( "texture/ui/tree/prefix.tga", 16, 16, 0, 16, 0  );
}

CTreeView::CTreeView( const CTreeView& rhs )
: CCompent(rhs)
{
	_pSelectItem = new CTreeSelectItem(this);
	_Copy( rhs );
}

CTreeView& CTreeView::operator=(const CTreeView& rhs)
{
	CCompent::operator =( rhs );
	_Copy( rhs );
	return *this;
}


void CTreeView::_Copy( const CTreeView& rhs )
{

}

CTreeView::~CTreeView(void)
{
	SAFE_DELETE(_pRoot);

	SAFE_DELETE(_pImage);
	SAFE_DELETE(_pAddImage);
	SAFE_DELETE(_pSubImage);
	SAFE_DELETE(_pSelectItem);
}

void CTreeView::Render()
{
	try
	{
		_pImage->Render( GetX(), GetY() );

		// ,SelectItem
		_pSelectItem->Render();

		if( _pScroll->GetIsShow() ) _pScroll->Render();
	}
	catch(...)
	{
	}
}

void CTreeView::Refresh()
{
	CCompent::Refresh();

	_nDrawX1 = _nX1;
	_nDrawY1 = _nY1;
	_nDrawX2 = _nX2;
	_nDrawY2 = _nY2;

	_nItemsHeight = 0;
	_pRoot->GetTreeHeight( _nItemsHeight, _nRowSpace );
	_nScrollMax = (float)((_nItemsHeight - GetHeight())/( _pRoot->GetHeight() + _nRowSpace) );
	if( _nScrollMax < 0.0 ) _nScrollMax = 0;
	int NumItems = 0;
	// xuqin modified
	_pRoot->GetNumItems( NumItems );
	if( _pScroll->SetRange( 0.0, NumItems ) )
	{
		if( _pScroll->GetScroll()!=CDrag::GetParent() )
			_pScroll->Refresh();
	}

	int y =  GetY() - (int)( _pScroll->GetStep().GetRate() * _nItemsHeight );
	_pRoot->RootRefresh( GetX() + _nColSpace + _nAddOffX, y, _nColSpace + _nAddOffX, _nRowSpace );

	_pSelectItem->Refresh();
}

void CTreeView::_OnScrollChange()
{
	int y =  GetY() - (int)( _pScroll->GetStep().GetRate() * _nItemsHeight );
	_pRoot->RootRefresh( GetX() + _nColSpace + _nAddOffX, y, _nColSpace + _nAddOffX, _nRowSpace );

	_pSelectItem->Refresh();
}

bool CTreeView::MouseRun( int x, int y, DWORD key )
{
	if( !IsNormal() ) return false;
	
	if( InRect( x, y ) )
	{
		if( (key & Mouse_LDown) && !_isChild && GetActive()!=this ) _SetActive();

		if( _pScroll->MouseRun(x,y,key) ) return true;

        if( _pDrag && _pDrag->BeginMouseRun(this, _IsMouseIn, x, y, key )==CDrag::stDrag )
        {			
			// Add by lark.li 20080805 begin
			CTextGraph *pSelectItem=dynamic_cast<CTextGraph*>(_pRoot->GetHitItem(x,y));
			if(!pSelectItem)
			{
				_pDrag->Reset();
				return true;
			}
			// End

			_pDragNode = _pRoot->GetHitNode( x, y );
			if( _pDragNode )
			{
				_pDragItem = _pRoot->GetHitItem(x, y);
				if( _pDragItem )
				{
					_pDragNode->RefreshNode();

					_nDragOffX = x - _pDragNode->GetFocusX();
					_nDragOffY = y - _pDragNode->GetFocusY();

					return true;
				}
			}
			_pDrag->Reset();
            return true;
        }

		if( (key & Mouse_LDB ) )
		{
			if( evtMouseDB ) evtMouseDB(this, x, y, key);
			return true;
		}
		// Add by lark.li 20080805 begin
		if( (key & Mouse_RDown ) )
		{
			if( evtMouseDB ) evtMouseDB(this, x, y, key);
			//return true;
		}
		// End

		if( (key & Mouse_Down ) && _pRoot->RootMouseRun(x, y, key) )
		{
			if( evtMouseDown ) evtMouseDown(this, x, y, key);
			return true;
		}
		return true;
	}
	return _IsMouseIn;
}

void CTreeView::DragRender()
{
	if( _pDragItem ) _pDragItem->Render( _pDrag->GetX() - _nDragOffX, _pDrag->GetY() - _nDragOffY );
}

bool CTreeView::MouseScroll( int nScroll )
{
	if( !IsNormal() ) return false;

	if( _IsMouseIn ) return _pScroll->MouseScroll( nScroll );
	return _IsMouseIn;
}

void CTreeView::Init()
{
	_nAddOffX = (int)(_pAddImage->GetWidth());
	_nAddHeight = (int)(_pAddImage->GetHeight());

	_pScroll->SetSize( _pScroll->GetWidth(), GetHeight() );
	_pScroll->SetPos( GetWidth() - _pScroll->GetWidth(), 0 );
	_pScroll->Init();

	_pScroll->SetPageNum( GetHeight()/(_nRowSpace) );

	if( _pDrag )
	{
        _pDrag->SetIsMove( false );
		_pDrag->evtMouseDragEnd = _DragEnd;
		_pDrag->evtMouseDragMove = NULL;
		_pDrag->evtMouseDragBegin = NULL;
	}
}

bool CTreeView::OnKeyDown( int key )
{
	if( !GetIsEnabled() ) return false;

	if( _IsMouseIn ) return _pScroll->OnKeyDown( key );
	return false;
}

void CTreeView::SetAlpha( BYTE alpha )
{
	_pImage->SetAlpha(alpha); 
	_pScroll->SetAlpha(alpha); 
}

void CTreeView::SetSelectNode( CTreeNodeObj* p )
{
	_pSelectItem->SetItem( p, NULL );
}

CCompent* CTreeView::GetHintCompent( int x, int y )    
{ 
    if( GetIsShow() && InRect( x, y ) )
    {
        if( !_strHint.empty() )
            return this;

        CItemObj *pObj = _pRoot->GetHitItem(x, y);
        if( pObj && SetHintItem( pObj ) )
        {
            return this;
        }
    }

    return NULL;
} 

void CTreeView::_DragEnd( int x, int y, DWORD key )
{
	CTreeNodeObj* _pDragMouseMoveNode = _pRoot->GetHitNode( x, y );	
	
	CForm * form = CFormMgr::s_Mgr.GetHitForm( x, y );
	if( form && evtMouseDragEnd )
	{
		evtMouseDragEnd( form, this, _pDragNode, _pDragItem, x, y, key );
	}
}
