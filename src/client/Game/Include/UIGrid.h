//----------------------------------------------------------------------
// :
// :lh 2004-07-29
// :CGraph,,
// :2004-10-09
//----------------------------------------------------------------------
#pragma once
#include "uicompent.h"
#include "uitextbutton.h"
#include "uiimage.h"
#include "UIGraph.h"
#include "uidragtitle.h"

namespace GUI
{
class CGrid : public CCompent
{
public:
	CGrid(CForm& frmOwn);
	CGrid( const CGrid& rhs );
	CGrid& operator=(const CGrid& rhs);
	virtual ~CGrid(void);
	GUI_CLONE(CGrid)

	virtual void	Init();
	virtual void	Render();
	virtual void	Refresh();
	virtual bool	MouseRun( int x, int y, DWORD key );
	virtual bool	IsHandleMouse()                 { return true;                  }
	virtual void	SetAlpha( BYTE alpha );
	virtual void	SetMargin( int left, int top, int right, int bottom ) { _nMargin=left;	}
    virtual void	SetIsDrag( bool v );
    virtual void    FrameMove( DWORD dwTime );
	virtual bool	IsFrameMove()					{ return true;					}

void			SetUnitSize(int w, int h);
	int				GetUnitWidth()	                { return _nUnitWidth;	        }
	int				GetUnitHeight()                 { return _nUnitHeight;	        }
    void			SetSpace( int x, int y );
    void            SetIsDragSize( bool v );

	void			Add(CGraph* p);

	int				GetCount()		{ return (int)_memory.size();		}
	CGraph*			GetGraph( unsigned int v ){	return (v<_memory.size())?_memory[v]:NULL;}


virtual bool	OnKeyDown( int key );

CGuiPic*		GetImage()		{ return _pImage;		}
	CImage*			GetSelectImage(){ return _pSelectImage; }
	CTextButton*	GetNext()		{ return _pNextPage;	}
	CTextButton*	GetPrior()		{ return _pPriorPage;	}

	CGraph*			GetSelect()		{ return _pSelect;		}
    int             GetSelectIndex(){ return _nSelectIndex; }

GuiEvent		evtSelectChange;				// 

protected:
	void			_SetSelf();
	void			_Copy( const CGrid& rhs );
	void			_Clear();
	void			_PageChange();

	static	void	_NextClick(CGuiData *pSender, int x, int y, DWORD key) {
		((CGrid*)(pSender->GetParent()))->_NextClick();
	}
	void			_NextClick();

	static	void	_PriorClick(CGuiData *pSender, int x, int y, DWORD key) {
		((CGrid*)(pSender->GetParent()))->_PriorClick();
	}
	void			_PriorClick();

	static	void	_DragBegin(CGuiData *pSender, int x, int y, DWORD key)	{
		((CGrid*)(pSender->GetParent()))->_DragBegin();
	}
	void			_DragBegin();

	static	void	_DragEnd(CGuiData *pSender, int x, int y, DWORD key)	{
		((CGrid*)(pSender->GetParent()))->_RefreshSize();
	}

	static void		_DragMove(CGuiData *pSender, int x, int y, DWORD key)	{
		((CGrid*)(pSender->GetParent()))->_RefreshSize();
	}
	void			_RefreshSize();

CGuiPic*		_pImage;
	CImage*			_pSelectImage;		// 

	CDragTitle*		_pSizeImg;

	CTextButton*	_pNextPage;			// 
	CTextButton*	_pPriorPage;		// 	

	char			_strPage[10];		// 
	int				_nStrX, _nStrY;
	int				_nStrWidth;

	int				_nUnitHeight, _nUnitWidth;  // 
    int             _nSpaceX, _nSpaceY; // 

	typedef std::vector<CGraph*>		memory;
	memory		_memory;
	int			_nSize;			
	int			_nFirst, _nLast;		// IDID
	int			_nPage, _nMaxPage;		// 
	int			_nColNum;				// 
	int			_nRowNum;				// 
	int			_nPageNum;				// 

	int			_nMargin;				// 
	int			_nStartX, _nStartY;

private:
	int			_nDragWidth, _nDragHeight;
    int			_nTotalW, _nTotalH;

	CGraph*		_pSelect;
    int         _nSelectIndex;
	bool		_IsSelect;
	DWORD		_clPageTextColor;
	
};

// 
inline void CGrid::SetUnitSize(int w, int h) 
{ 
	if( w>0 ) _nUnitWidth=w; 
	if( h>0 ) _nUnitHeight=h;
	_pSelectImage->SetSize( _nUnitWidth, _nUnitHeight );
}

inline void CGrid::SetSpace( int x, int y )	
{ 
    if( x >= 0 )	_nSpaceX=x; 
    if( y >= 0 ) 	_nSpaceY=y;	
}

inline void CGrid::Add(CGraph* p) 
{ 
	_memory.push_back(p);
	_nSize = (int)_memory.size();
	_pSelect = p;
}

inline void CGrid::_PriorClick()
{
	_nPage--;
	if( _nPage<0 ) _nPage=_nMaxPage;
	Refresh();
}

inline void CGrid::_PageChange()
{
	_nFirst = _nPage * _nPageNum;
	_nLast = _nFirst + _nPageNum;
	if( _nLast > _nSize ) _nLast = _nSize;
	sprintf( _strPage, "%d/%d", _nPage+1, _nMaxPage+1 );
}

inline 	void CGrid::_NextClick()
{
	_nPage++;
	if( _nPage>_nMaxPage ) _nPage=0;
	Refresh();
}

inline 	void CGrid::SetIsDragSize( bool v )         
{ 
    _pSizeImg->SetIsShow( v );
}

}
