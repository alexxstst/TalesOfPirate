//----------------------------------------------------------------------
// :
// :lh 2004-07-19
// :CTreeViewCTreeNodeObj
// :2004-10-09
//----------------------------------------------------------------------
#pragma once
#include "uicompent.h"
#include "uiscroll.h"

namespace GUI
{
class CTreeNodeObj;
typedef std::vector<CTreeNodeObj*>	TreeNodes;

class CTreeView;

typedef void (*GuiTreeDragEvent)(CForm* pTargetForm, CTreeView *pSourceTree, CTreeNodeObj* pNode, CItemObj* pItem, int x, int y, DWORD key );

class CTreeSelectItem
{
public:
	CTreeSelectItem(CTreeView *own) : _pOwn(own), _pItem(NULL), _pParent(NULL) {}

	void			Render();
	void			Refresh();

	// ,,true
	bool			SetItem( CTreeNodeObj* parent, CItemObj* item );			

	// ,,,,true
	bool			SetDifferItem( CTreeNodeObj* parent, CItemObj* item );		

	void			CancelSelect()			{ _pItem=NULL; _pParent=NULL;}

	CItemObj*		GetItem()				{ return _pItem;			}
	CTreeNodeObj*	GetParent()				{ return _pParent;			}
	CTreeView*		GetOwn()				{ return _pOwn;				}

private:	
	bool			_SetParent( CTreeNodeObj *node );		// false

	CTreeView*		_pOwn;
	CTreeNodeObj*	_pParent;
	CItemObj*		_pItem;

private:    // 
    CTreeSelectItem( const CTreeSelectItem& rhs );
	CTreeSelectItem& operator=(const CTreeSelectItem& rhs);

};

class CTreeNodeObj
{
public:
	CTreeNodeObj(CTreeView *own) : _pOwn(own), _IsExpand(true), _pParent(NULL), _dwTag(0)	{_nodes.push_back(this);	}
	virtual ~CTreeNodeObj();

	virtual void		RenderSelf() {}
	virtual bool		AddNode( CTreeNodeObj* obj );
	virtual int			GetHeight()							{ return 0;		}
	virtual int			GetWidth()							{ return 0;		}
	virtual void		ShowFocus();
	virtual void		MouseRunSelf( int x, int y, DWORD key ) {}
	virtual void		RefreshNode() {}

    virtual const std::string& GetCaption()                        { return "";    }
	virtual void		SetCaption( const std::string&  str)		{}

	// 
	virtual int			GetFocusX()							{ return _nX;	}		
	virtual int			GetFocusY()							{ return _nY;	}

	bool				ClearAllChild();
	bool				DelNode( CTreeNodeObj* pNode );

	// xuqin added
	void				GetNumItems( int& Items );

	// 
	int					GetChildCount()						{ return (int)_ndChilds.size();		}
	CTreeNodeObj*		GetChildNode( unsigned int v )		{ if(v<_ndChilds.size()) return _ndChilds[v]; return NULL;	}

public:
	void				Render();
	void				Refresh(int x, int& y, int colwidth, int rowheight);
	bool				MouseRun( int x, int y, DWORD key );
	int					GetTreeHeight( int& height, int rowheight );

    CTreeNodeObj*       FindNode( const std::string& str );

    CItemObj*           GetHitItem( int x, int y );
    CTreeNodeObj*       GetHitNode( int x, int y );

	TreeNodes&			GetChild()							{ return _ndChilds;				}
	bool				HasChild()							{ return !_ndChilds.empty();	}
	void				SetIsExpand( bool v )				{ _IsExpand=v;					}	

	bool				GetIsExpand() { return _IsExpand; }
	bool				InRect(int x, int y) {return x>=_nX && x<=_nX2 && y>=_nY && y<=_nY2;}

	int					GetX()								{ return _nX;					}
	int					GetY()								{ return _nY;					}

	CTreeNodeObj*		GetParent()							{ return _pParent;				}

	bool				IsShow() const;

	void				SetTag( DWORD v )					{ _dwTag = v;					}
	DWORD				GetTag()							{ return _dwTag;				}

public:	// 
	int							GetID()						{ return (int)_nodes.size() - 1;}
	static CTreeNodeObj*		GetNode( unsigned int id )  { 
		if( id < _nodes.size() ) return _nodes[id];
		return NULL;
	}

protected:
	virtual bool		_HasPage()							{ return !_ndChilds.empty();	}
    virtual CItemObj*   _GetSelfHitItem( int x, int y )     { return NULL;                  }

protected:
	void			_RenderTree();

protected:
	int					_nX, _nY, _nX2, _nY2;
	bool				_IsExpand;		// 
	TreeNodes			_ndChilds;

	CTreeView*			_pOwn;
	CTreeNodeObj*		_pParent;

	DWORD				_dwTag;

	static TreeNodes	_nodes;

private:    // 
    CTreeNodeObj( const CTreeNodeObj& rhs );
	CTreeNodeObj& operator=(const CTreeNodeObj& rhs);

};

class CTreeNodeRoot : public CTreeNodeObj
{
public:
	CTreeNodeRoot( CTreeView *own ) : CTreeNodeObj(own) {}

	void	RootRender();
	void	RootRefresh(int x, int& y, int colwidth, int rowheight);
	bool	RootMouseRun( int x, int y, DWORD key );

private:    // 
    CTreeNodeRoot( const CTreeNodeRoot& rhs );
	CTreeNodeRoot& operator=(const CTreeNodeRoot& rhs);

};

// node
class CTreeNode : public CTreeNodeObj
{
public:
	CTreeNode( CTreeView *own, CItemObj* item ) : CTreeNodeObj(own), _pItem(item) {}
	~CTreeNode() { SAFE_DELETE(_pItem); } // UI //delete _pItem; }

	virtual void	RenderSelf();
	virtual void	MouseRunSelf( int x, int y, DWORD key );

    virtual const std::string& GetCaption()                    { return _pItem->GetString();           }

	int				GetHeight()							{ return _pItem->GetHeight();			}
	int				GetWidth()							{ return _pItem->GetWidth();			}
	CItemObj*		GetItem()							{ return _pItem;						}

	virtual void	SetCaption( const std::string& str )		{ _pItem->SetString( str );				}

protected:
    virtual CItemObj*   _GetSelfHitItem( int x, int y ) { return _pItem;                        }

protected:
	CItemObj*		_pItem;

private:    // 
    CTreeNode( const CTreeNode& rhs );
	CTreeNode& operator=(const CTreeNode& rhs);

};

// node,
class CTreeGridNode : public CTreeNodeObj
{
public:
	CTreeGridNode(CTreeView *own, CItemObj* item);
	~CTreeGridNode();

	virtual bool	AddNode( CTreeNode* obj )			{ return false;						}
	virtual void	RenderSelf();
	virtual void	ShowFocus();
	virtual void	MouseRunSelf( int x, int y, DWORD key );
	virtual void	RefreshNode();
	virtual int		GetFocusX()							{ return _nFocusX1;					}
	virtual int		GetFocusY()							{ return _nFocusY1;					}

    virtual const std::string& GetCaption()                    { return _pItem->GetString();       }
	virtual void	SetCaption( const std::string& str )		{ _pItem->SetString( str );			}

	bool			AddItem( CItemObj* item );
	bool			DelItem( CItemObj* item );
    bool            DelItem( const std::string& str );
    CItemObj*       FindItem( CItemObj* item );

	int				GetHeight();
	int				GetWidth()							{ return _nColMaxNum * _nUnitWidth; }

	void			SetColMaxNum( int n )				{ if(n>0) _nColMaxNum=n;			}
	int				GetColMaxNum()						{ return _nColMaxNum;				}

	int				GetRowNum()							{ return _nRowNum;					}

	void			SetUnitSize( int w, int h );

	CGuiPic*		GetUpImage()						{ return &_imgUp;					}
	CGuiPic*		GetDownImage()						{ return &_imgDown;					}

	CItemObj*		GetItem()							{ return _pItem;					}
	DWORD			GetItemCount()						{ return (DWORD)_items.size();}
	CItemObj*		GetItemByIndex(DWORD index)			{ return (_items.size()>=index)?_items[index]:NULL;}

    void            Clear();

protected:
	//virtual bool	    _HasPage()					    { return !_items.empty();			}
	virtual bool	    _HasPage()					    { return true;						}
    virtual CItemObj*   _GetSelfHitItem( int x, int y );

protected:
	CItemObj*		_pItem;					        // ,

	typedef std::vector<CItemObj*> vitems;
	vitems			_items;

	int				_nUnitWidth,	_nUnitHeight;	// 
	int				_nColMaxNum;					// 
	int				_nRowNum;						// 

private:
	void			_CaclRowNum();

private:
	CGuiPic	_imgUp,  _imgDown;
	int		_nFocusX1,  _nFocusX2,  _nFocusY1,   _nFocusY2;
	int		_nFocusCol,	_nFocusRow;
	bool		_IsShowFocus;

private:    // 
    CTreeGridNode( const CTreeGridNode& rhs );
	CTreeGridNode& operator=(const CTreeGridNode& rhs);

};

class CTreeView : public CCompent
{
public:
	struct NodeOptions
	{
		int TextX{ 0 };
		int TextY{ 0 };
	};

	CTreeView(CForm& frmOwn);
	CTreeView( const CTreeView& rhs );
	CTreeView& operator=(const CTreeView& rhs);
	virtual ~CTreeView(void);
	GUI_CLONE(CTreeView)

	virtual void		Init();
	virtual void		Render();
	virtual void		Refresh();
	virtual bool		MouseRun( int x, int y, DWORD key );
	virtual bool		MouseScroll( int nScroll );

	virtual bool		IsHandleMouse()				{ return true;					}
	virtual bool		OnKeyDown( int key );
	virtual void		SetAlpha( BYTE alpha );
    virtual CCompent*   GetHintCompent( int x, int y );                       // 
	virtual void		DragRender();

public:
	bool				ClearAllNode()				{ return _pRoot->ClearAllChild();		}
	bool				DelNode( CTreeNodeObj* p )	{ return _pRoot->DelNode( p );	}

public:
	CItemObj*			GetHitItem( int x, int y )	{ return _pRoot->GetHitItem(x, y);	}
	CTreeNodeObj*		GetHitNode( int x, int y )	{ return _pRoot->GetHitNode(x, y);	}
	void				SetColSpace( int n )		{ if(n>0) _nColSpace=n;			}
	void				SetRowSpace( int n )		{ if(n>0) _nRowSpace=n;			}	

	CTreeNodeRoot*		GetRootNode()				{ return _pRoot;				}
	CTreeNodeObj*		GetSelectNode()			{ return _pSelectItem->GetParent(); }
	CTreeSelectItem*	GetSelect()					{ return _pSelectItem;			}

	CGuiPic*			GetImage()					{ return _pImage;				}

	CGuiPic*			GetAddImage()				{ return _pAddImage;			}
	CGuiPic*			GetSubImage()				{ return _pSubImage;			}
	CGuiPic* GetNodeImage() { return _nodeImage.get(); }
	CTreeView::NodeOptions& GetNodeOptions() { return nodeOptions; }
	CScroll*			GetScroll()					{ return _pScroll;				}

	void				SetIsAutoExpand( bool v )	{ _IsAutoExpand=v;				}
	bool				GetIsAutoExpand()			{ return _IsAutoExpand;			}
	
	void				RenderAddImage( int x, int y );
	void				RenderSubImage( int x, int y );
	bool				InAddImage( int x, int y, int ux, int uy );
	bool				InTree( int x, int y, int x2, int y2 ) { return y>=_nDrawY1 && y2<=_nDrawY2; }
	void				SetSelectNode( CTreeNodeObj* p );

	void				DelItemCall( CItemObj* pItem );

	void				SetSelectColor( DWORD v )	{ _dwSelectColor = v;			}
	DWORD				GetSelectColor()			{ return _dwSelectColor;		}

public:
	GuiEvent			evtSelectChange;		// 
	GuiEvent			evtSelectLost;			// 
	GuiEvent			evtItemLost;			// Item
	GuiEvent			evtItemChange;			// Item
	GuiMouseEvent		evtMouseDown;			// 
	GuiMouseEvent		evtMouseDB;				//
	GuiTreeDragEvent	evtMouseDragEnd;		// 

protected:
	static void			_ScrollChange(CGuiData *pSender) {
		((CTreeView*)(pSender->GetParent()))->_OnScrollChange();
	}
	void				_OnScrollChange();

protected:
    static	void	_DragEnd(CGuiData *pSender, int x, int y, DWORD key)	{
        ((CTreeView*)(pSender))->_DragEnd( x, y, key );
    }
    void			_DragEnd( int x, int y, DWORD key );

private:
	void			_Copy( const CTreeView& rhs );

protected:
	CScroll*			_pScroll;		// 
	CGuiPic*			_pImage;		// 

	CGuiPic*			_pAddImage;		// 
	CGuiPic*			_pSubImage;		// 
	std::unique_ptr<CGuiPic> _nodeImage = std::make_unique<CGuiPic>();
	NodeOptions nodeOptions{};

	int					_nColSpace, _nRowSpace;		// ,
	bool				_IsAutoExpand;	// 

	CTreeNodeRoot*		_pRoot;			// 
	CTreeSelectItem*	_pSelectItem;	// item

	CItemObj*			_pDragItem;		// item
	CTreeNodeObj*		_pDragNode;
	int					_nDragOffX, _nDragOffY;

	int					_nDrawX1,	_nDrawY1;
	int					_nDrawX2,   _nDrawY2;

private:
	int					_nAddOffX;
	int					_nAddHeight;

	int					_nItemsHeight;
	float				_nScrollMax;

	DWORD				_dwSelectColor;	// 

};

// 
inline void CTreeSelectItem::Render()
{
	if( _pItem ) 
	{
		if(_pParent)
			_pParent->ShowFocus();
	}
	else
	{
		_pOwn->GetRootNode()->RootRender();
	}
}

inline void CTreeSelectItem::Refresh()
{
	if( _pParent ) _pParent->RefreshNode();
}

inline void	CTreeNodeObj::_RenderTree()
{
	_pOwn->GetRootNode()->RootRender();
}

inline void CTreeGridNode::_CaclRowNum()
{
    if( _nColMaxNum==1 )
    {
	    _nRowNum = (int)_items.size();
    }
    else
    {
	    _nRowNum = (int)(_items.size() / _nColMaxNum);
	    if( _items.size() % _nColMaxNum ) _nRowNum++;
    }
}

inline void CTreeView::RenderAddImage( int x, int y )
{ 
	if( InTree(x-_nAddOffX, y, x , y+_nAddHeight ) )
		_pAddImage->Render( x - _nAddOffX, y ); 
}

inline void CTreeView::RenderSubImage( int x, int y )
{ 
	if( InTree(x-_nAddOffX, y, x , y+_nAddHeight ) )
		_pSubImage->Render( x - _nAddOffX, y ); 
}

inline bool CTreeView::InAddImage( int x, int y, int ux, int uy )
{ 
	return x>=ux-_nAddOffX && x<=ux && y>=uy && y<=uy+_nAddHeight;
}

inline void	CTreeView::DelItemCall( CItemObj* pItem )	
{ 
	if( _pDragItem && _pDragItem==pItem ) 
		_pDragItem=NULL;			
}

}
