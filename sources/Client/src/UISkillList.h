//----------------------------------------------------------------------
// :
// :lh 2004-12-13
// :,,,
//      ,,Command
//      ,
//      ,
//      ,,
//      ,,,
//   :CList,CListItems,CList,
//      ,,CList
// :
//----------------------------------------------------------------------
#pragma once
#include <vector>
#include "uicompent.h"
#include "uiskillcommand.h"
#include "UITextButton.h"
#include "UIScroll.h"

namespace GUI
{

class CSkillList;
typedef void (*GuiSkillListUpgrade) (CSkillList *pSender, CSkillCommand* pSkill);

class CSkillList : public CCompent
{
public:
    CSkillList(CForm& frmOwn);
    CSkillList( const CSkillList& rhs );
    CSkillList& operator=(const CSkillList& rhs);
    virtual ~CSkillList(void);
    GUI_CLONE(CSkillList)

    virtual void	Init();
    virtual void	Render();
    virtual void	Refresh();
    virtual bool	MouseRun( int x, int y, DWORD key );
    virtual bool	MouseScroll( int nScroll );
    virtual void	SetAlpha( BYTE alpha );
    virtual bool	IsHandleMouse()				{ return true;			}
    virtual bool    OnKeyDown( int key );
    virtual void	DragRender();
    virtual CCompent* GetHintCompent( int x, int y );

    virtual void        SetMargin( int left, int top, int right, int bottom );
    virtual CGuiPic*    GetImage()              { return _pImage;       }
    
    CGuiPic*        GetButtonImage()            { return _pButton;      }
    void            Clear();
    void			SetRowHeight( int v )		{ _nRowHeight = v;	    }
    void            SetUnitSize( int w, int h ) { _nUnitHeight=h; _nUnitWidth=w;        }
    void            SetFontLeft( int x )        { _nFontStart = x;      }
    void            SetFontColor( DWORD color ) { _dwFontColor = color; }
    void            SetIsShowUpgrade( bool v )  { _IsShowUpgrade = v;   }

    CSkillCommand*  GetCommand( unsigned int  v );
    bool            AddCommand( CSkillCommand* p );
    CSkillCommand*  FindSkill( int nID );
    bool            DelSkill( int nID );

	int				GetCount()					{ return (int)_skills.size();			}

    CGuiPic*        GetSelect()                 { return _pSelect;      }
    CScroll*        GetScroll()                 { return _pScroll;      }

    int             FindCommand( CSkillCommand* p );

public:
    GuiSkillListUpgrade     evtUpgrade;

private:
    void    _SetSelf();
    void    _Copy( const CSkillList& rhs );
    void    _SetFirstShowRow( DWORD );
	int		_GetHitSkill( int x, int y );

private:
    static void			_ScrollChange(CGuiData *pSender) {
        ((CSkillList*)(pSender->GetParent()))->_OnScrollChange();
    }
    void				_OnScrollChange();
    void	_ResetPageNum();

private:
    static	void	_DragEnd(CGuiData *pSender, int x, int y, DWORD key)	{
        ((CSkillList*)(pSender))->_DragEnd( x, y, key );
    }
    void			_DragEnd( int x, int y, DWORD key );

protected:
    CGuiPic*		_pImage;		// 
    CGuiPic*		_pSelect;	    // 
    CGuiPic*        _pButton;       // 
    CScroll*		_pScroll;	    

    typedef std::vector<CSkillCommand*> skills;
    skills          _skills;

    // 
    int				_nLeftMargin;	
    int				_nTopMargin;	
    int				_nRightMargin;	
    int				_nBottomMargin;	

    int				_nRowHeight;    // 
    int             _nFontStart;    // 
    DWORD           _dwFontColor;

    int             _nUnitHeight, _nUnitWidth;   // 

    int             _nSelectIndex;  // 
    bool            _IsShowUpgrade;

private:
    int             _nShowFirst;    // 
    int             _nShowLast;     // 
    int				_nShowCount;    //     

    // 
    int             _nSX1, _nSY1, _nSX2, _nSY2;   

    int             _nButtonX1;     // X
    int             _nButtonOffY;   // Y

    int             _nRowSpace;     // 
    int             _nFontYOff;     // 

private:
    int             _nDragIndex;
    int             _nDragOffX, _nDragOffY;
    int             _nMouseUpgradeID;

};


//  
inline 	void CSkillList::_ResetPageNum() 
{ 
    _pScroll->SetPageNum( ( GetHeight() - _nTopMargin - _nBottomMargin ) / _nRowHeight - 1 );
}

inline CSkillCommand* CSkillList::GetCommand( unsigned int  v )
{
    if( v>=_skills.size() ) return NULL;
    return _skills[v];
}

inline int CSkillList::_GetHitSkill( int x, int y )
{
    if( x>=_nSX1 && x<=_nSX2 && y>=_nSY1 && y<=_nSY2 )
    {
        int h = ( _nShowCount * _nRowHeight + _nSY1 );
        if( y >= h ) return false;

        // 
        int row = ( y - _nSY1 ) / _nRowHeight + _nShowFirst;
        if( row>=0 && row<(int)_skills.size() ) 
			return row;
	}
	return -1;
}

}
