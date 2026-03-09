//----------------------------------------------------------------------
// :
// :lh 2004-07-21
// :CListItems,CListItemsCItemRow
// :2004-10-09
//----------------------------------------------------------------------
#pragma once
#include "uicompent.h"
#include "uiscroll.h"
#include "uiimage.h"
#include "uiitem.h"

namespace GUI
{

class CList;

// 
class CSelectItem
{
public:
	CSelectItem();
	CSelectItem( const CSelectItem& rhs );
	CSelectItem& operator=( const CSelectItem& rhs );

	void		Render( int x, int y );

	void		SetSelect( CItemRow* pItem, DWORD dwIndex );
	void		SetMouseMoveSelect( CItemRow* pItem, DWORD dwIndex );
	void		SetNoSelect();
	int			GetIndex()				{ return _nIndex;			}
	bool		GetIsSelect()			{ return _isSelect;			}

	void		SetSize( int w, int h ) { _pImage->SetScale( w, h );}

	void		SetList( CList* pList ) { _pList = pList;			}

CItemRow*	GetItem()				{ return _pItem;			}
	void		SetIsEnabled( bool v )	{ _isEnabled=v;				}
	bool		GetIsEnabled()			{ return _isEnabled;		}
	CGuiPic*	GetImage()				{ return _pImage;			}

private:
	void		_Copy( const CSelectItem& rhs );

CItemRow*	_pItem;				// Item
	bool		_isEnabled;			// 
	bool		_isSelect;
	int			_nIndex;			// -1
	CGuiPic*	_pImage;

CList*		_pList;

};

class CList;
class CForm;
class CListItems
{
public:
	explicit CListItems();
	explicit CListItems( const CListItems& rhs );
	CListItems& operator=( const CListItems& rhs );
	virtual ~CListItems();
	virtual bool		Clone( CListItems* rhs );

bool				MouseRun( int x, int y, DWORD key );
	void				Refresh();
	virtual void		Render();
	virtual void		Clear();
	virtual bool		SetColumnWidth( unsigned int nCol, unsigned int width ) { return false; }
	virtual unsigned int GetColumn() { return 1; }
	virtual CItemRow*	Add( const char* str, DWORD c );

	virtual void		SetRect( int x1, int y1, int x2, int y2, int rowheight ); // 
	
	int					GetColumnWidth( unsigned int nCol=0 )	{ return _GetColumnWidth(nCol); }
	void				SetParent( CList* pList );
	CSelectItem*		GetSelect()								{ return _pSelect;				}

	CItemRow*			NewItem();
	bool				Del( CItemRow* item );

	void				SetFirstShowRow( DWORD row );
	CItemRow*			GetItem( int index );
	int					GetIndex( CItemRow *p) ;
	DWORD				GetCount()								{ return _dwCount;				}

	int					GetShowCount()							{ return _nShowCount;			}
	int					GetCurShowCount()						{ return _dwLastShowRow -_dwFirstShowRow;}//Add by zcj 20080801

	int					GetMaxRowNum()							{ return _nMaxRow;				}
	void				SetMaxRowNum( int n );

	void				SetIsMouseFollow( bool v )				{ _IsMouseFollow=v;				}
	bool				GetIsMouseFollow()						{ return _IsMouseFollow;		}	

	void				SetItemMargin( int left, int top );
	void				SetImageMargin( int left, int top );

		// 	
	void				SelectPrior();			// 	
	void				SelectNext();			// 
	void				SelectFirst();			// 
	void				SelectLast();			// 
	void				SelectAdd( int page );	// 
	void				Select( int n );		// 

protected:
	void				_CheckMaxRow();			// ,,
	void				_AddRow( CItemRow* p );

private:
	void				_Copy(const CListItems &rhs);
	virtual int			_GetColumnWidth( unsigned int nCol )	{ return 0;						}

protected:
	typedef		std::deque<CItemRow*>	vitems;
	vitems				_items;
	CSelectItem*		_pSelect;			// Item

	CGuiPic*			_pItemPic;			// 
	CList*				_pList;

	int					_nX1, _nX2, _nY1, _nY2;
	int					_nHeight;
	int					_nShowCount;

	DWORD				_dwFirstShowRow;	// 
	DWORD				_dwLastShowRow;		// 
	DWORD				_dwCount;			// 
	int					_nMaxRow;			// 

	// 
	int					_nItemLeft, _nItemTop;
	int					_nItemX;

	int					_nPicLeft,  _nPicTop;
	int					_nPicX;
	bool				_IsMouseFollow;		// 

static int			_nTmpY;

};

class CListItemsEx : public CListItems
{
public:
	explicit CListItemsEx(unsigned int nCol );
	explicit CListItemsEx( const CListItemsEx& rhs );
	CListItemsEx& operator=( const CListItemsEx& rhs );
	~CListItemsEx();

	virtual bool		Clone( CListItems* rhs );
	virtual void		Clear();
	virtual void		SetRect( int x1, int y1, int x2, int y2, int rowheight );
	virtual void		Render();

virtual CItemRow*	Add( const char* str, DWORD c );
	virtual bool		SetColumnWidth( unsigned int nCol, unsigned int width );
	virtual unsigned int GetColumn() { return _nCol; }

private:
	virtual int			_GetColumnWidth( unsigned int nCol );

	void				_SetSelf(const CListItemsEx &rhs);
	void				_RefreshColX();

protected:
	unsigned int		_nCol;			// 
	unsigned int*		_nColWidth;		// 
	int*				_nColX;			// 

}; 

class CList : public CCompent
{
public:
	CList(CForm& frmOwn);
	CList(CForm& frmOwn, int nCol);
	CList( const CList& rhs );
	CList& operator=(const CList& rhs);
	virtual ~CList(void);
	GUI_CLONE(CList)

	virtual void		Init();
	virtual void		Render();
	virtual void		Refresh();
	virtual bool		MouseRun( int x, int y, DWORD key );
	virtual bool		MouseScroll( int nScroll );

	virtual bool		IsHandleMouse()				{ return true;	}
	virtual void		SetAlpha( BYTE alpha );

		// 
	virtual void		OnActive();
	virtual void		OnLost();
	virtual bool		OnKeyDown( int key );

	virtual bool		SetShowRow( int n );			// ,List

CItemRow*			GetSelectItem()						{ return  _pItems->GetSelect()->GetItem();	}

	CScroll*			GetScroll()							{ return _pScroll;							}
	CGuiPic*			GetImage()							{ return _pImage;							}
	CGuiPic*			GetItemImage()						{ return _pItemImage;						}
	CListItems*			GetItems()							{ return _pItems;							}

	void				SetRowHeight( int v )				{ if( v>0) _nRowHeight = v;					}
	int					GetRowHeight()						{ return _nRowHeight;						}
	void				SetSpaceBottom( int n )				{ if(n>=0) _nSpaceBottom=n;					}

	void				SetMargin( int left, int top, int right, int bottom );

	void                SetFrameSelectAtrribute(int width ,int height ,int x, int y);

	void                ResetScrollChange();

CItemRow*			Add( const char* str );
	bool				Del( CItemRow* item );
	void				Clear();
	CItemRow*			NewItem();

	void				SetFontColor( std::uint32_t c )		{ _dwFontColor = c;			}
	DWORD				GetFontColor()				{ return _dwFontColor;		}

	void				SetSelectColor( std::uint32_t c )	{ _dwSelectColor = c;		}
	DWORD				GetSelectColor()			{ return _dwSelectColor;	}		

	void				SetIsChangeColor( bool v )	{ _IsChangeColor = v;		}
	bool				GetIsChangeColor()			{ return _IsChangeColor;	}

GuiEvent			evtSelectChange;			// 
	GuiMouseEvent		evtListMouseDown;			// 
	GuiMouseEvent		evtListMouseDB;

protected:
	static void			_ScrollChange(CGuiData *pSender) {
		((CList*)(pSender->GetParent()))->_OnScrollChange();
	}
	void				_OnScrollChange();
	void				_ResetPageNum();

void				_SetScrollRange();
	void				_SetSelf();
	void				_Copy( const CList& rhs );

CScroll*			_pScroll;	
	CListItems*			_pItems;
	CGuiPic*			_pImage;		// 
	CGuiPic*			_pItemImage;	

	int					_nLeftMargin;	// 
	int					_nTopMargin;	// 
	int					_nRightMargin;	// 
	int					_nBottomMargin;	// 

	int					_nRowHeight;	// 
	int					_nSpaceBottom;	// 

	std::uint32_t				_dwFontColor;
	std::uint32_t				_dwSelectColor;
	bool				_IsChangeColor;	// 


};

// 
inline 	void CSelectItem::SetSelect( CItemRow* pItem, DWORD dwIndex ) 
{
	if( _pList->GetIsChangeColor() )
	{
		if ( _pItem )
		{
			_pItem->SetColor( _pList->GetFontColor() );
			_pItem=NULL; 
		}

		_pItem = pItem;
		if (_pItem)
		{
			_pItem->SetColor( _pList->GetSelectColor() );
		}
	}
	else
	{
		_pItem = pItem;
	}

	_isSelect = true;
	_nIndex = dwIndex;	
}

inline 	void CSelectItem::SetNoSelect()	
{ 
	if( _pItem )
	{
		_pItem->SetColor( _pList->GetFontColor() );
	}

	_isSelect=false;
	_pItem=NULL; 
	_nIndex=-1; 
}

inline 	void CListItems::SetFirstShowRow( DWORD row ) 
{
	if( row <= _dwCount ) 
	{
		_dwFirstShowRow = row;
		_dwLastShowRow = _dwFirstShowRow + _nShowCount;
		_dwLastShowRow = _dwLastShowRow > _dwCount ? _dwCount : _dwLastShowRow;
	}
}

inline void	CListItems::SetItemMargin( int left, int top )
{
	_nItemLeft = left;
	_nItemTop = top;
}

inline void	CListItems::SetImageMargin( int left, int top )
{
	_nPicLeft = left;
	_nPicTop = top;
}

inline 	void CList::_ResetPageNum() 
{ 
	_pScroll->SetPageNum( ( GetHeight() - _nTopMargin - _nBottomMargin ) / _nRowHeight - 1 );
}

inline void CListItems::SelectPrior()
{	
	if( _pSelect->GetIsSelect() ) Select( _pSelect->GetIndex() - 1 ); 
	else Select( (int)_items.size() );
}

inline void CListItems::SelectNext() 
{	
	if( _pSelect->GetIsSelect() ) Select( _pSelect->GetIndex() + 1 ); 
	else Select( 0 );
}

inline void CListItems::SelectFirst()
{
	Select( 0 );
}

inline void CListItems::SelectLast()
{
	Select( _dwCount - 1 );
}

inline void CListItems::SelectAdd( int page )
{
	if( _pSelect->GetIsSelect() ) 
	{
		Select( _pSelect->GetIndex() + page ); 
	}
}

}
