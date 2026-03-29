//--------------------------------------------------------------
// :
// :lh 2004-07-08
// :,,,
// :2004-10-09
//--------------------------------------------------------------
#pragma once
#include "uiguidata.h"
#include "uiform.h"
#include "uicursor.h"

namespace GUI
{
class CCommandObj;
typedef void (*UseComandEvent)(CCommandObj* pItem,bool& isUse);

enum eAccept
{
	enumAccept,		// 
	enumRefuse,		// 
	enumFast,		// 
};

enum eMouseAction
{
	enumMA_None,		// 

	enumMA_Gui,			// GUI
	enumMA_Skill,		// ,
	enumMA_Drill,		// 
};

class CCommandObj;
class CContainer;
class CCompent : public CGuiData
{
public:
	friend class CFormMgr;
	friend class CForm;
	friend class CContainer;
public:
	CCompent(CForm& frmOwn);
	CCompent(const CCompent& rhs);
	CCompent& operator=( const CCompent& rhs );
	virtual ~CCompent(void);
	GUI_CLONE(CCompent)

	virtual bool	MouseRun( int x, int y, DWORD key );
    virtual void    Refresh();
    virtual void    FrameMove( DWORD dwTime ) {}
	virtual bool	IsFrameMove()				{ return false;		}

	eCompentAlign	GetAlign()	{ return _align; }
	virtual void	SetAlign( eCompentAlign v );
	virtual void	SetIsDrag( bool v );
    virtual void	SetIsShow( bool v );

	virtual void	SetParent( CGuiData* p );

	void			SetForm( CForm* f );
	virtual			CForm* GetForm()	        { return _frmOwn;	}

	virtual CCompent* Find( const char* str )   { return _frmOwn->Find( str ); }

    virtual CCompent* GetHintCompent( int x, int y );                       // 

    virtual CCompent*       GetHitCommand(int x, int y)     { return NULL;  }   // ,Compent
    virtual UseComandEvent  GetUseCommantEvent()            { return NULL;  }   // itemitem
    virtual eAccept	        SetCommand( CCommandObj* p, int x, int y )	{ return enumRefuse;    }   // command

public:
	virtual void	OnActive();		// 
	virtual void	OnLost() { if ( evtLost ) evtLost(this );	}		// 

	virtual bool	OnKeyDown( int key )	{ return false;		}
	virtual bool	OnChar( char c )		{ return false;		}

	virtual bool	IsHandleMouse()			{ return false;		}		// 

	static void		SetActive(CCompent* v);
	static CCompent* GetActive()			{ return _pActive;	}

	void	SetIsFocus( bool v )			{ _IsFocus = v;		}
	bool	GetIsFocus()					{ return _IsFocus;	}

    void            AddForm();

	eMouseAction	GetMouseAction()						{ return _eMouseAction;			}
	void			SetMouseAction( eMouseAction v )		{ _eMouseAction = v;			}

	static CCompent* GetLastMouseCompent()					{ return _pLastMouseCompent;	}

public:
	GuiEvent		evtLost;		// 
	GuiEvent		evtActive;		// 

protected:
	virtual void	_AddForm();		// Form
	void			_SetActive();

private:
	void			_Copy( const CCompent& rhs );

protected:
	static CCompent*	_pActive;	// 

protected:
	CForm			*_frmOwn;		// ,

	bool			_IsFocus;		// 

	eCompentAlign	_align;
	bool			_isChild;		// 
	eMouseAction	_eMouseAction;	// 

	static CCompent*	_pLastMouseCompent;

};

// ,CCompent
class CContainer : public CCompent
{
public:
	CContainer(CForm& frmOwn);
	CContainer( const CContainer& rhs );
	CContainer& operator=(const CContainer& rhs);
	virtual ~CContainer(void);
	GUI_CLONE(CContainer)

	virtual void	Init();
	virtual void	Render();
	virtual void	Refresh();
	virtual bool	MouseRun( int x, int y, DWORD key );
	virtual void	SetAlpha( BYTE alpha );
	virtual CCompent* Find( const char* str );
    virtual CCompent* GetHitCommand(int x, int y);
    virtual CCompent* GetHintCompent( int x, int y );
    virtual void    FrameMove( DWORD dwTime );

	bool			AddCompent( CCompent* p );
	CCompent*		GetIndex( unsigned int index );

	int				GetIndex( CCompent* p );		// p,,-1
	int				GetSize() { return (int)_items.size(); }

	void			ForEach( CompentFun pFun );
	
	virtual eAccept	SetCommand( CCommandObj* p, int x, int y )	{ return enumRefuse;			}

private:
	void			_SetSelf( const CContainer& rhs );

protected:
	typedef std::vector<CCompent*>	items;
	items			_items;
	items			_mouse;				// 

};

// 
inline void CCompent::_SetActive() 
{
    if( !_isChild && _IsFocus && GetActive()!=this )
		SetActive( this );
}

inline void CCompent::SetForm( CForm* f ) 
{ 
	_frmOwn = f;
	_pParent = f;
	_isChild = false;
}

inline CCompent* CContainer::GetIndex( unsigned int index )
{
	if( index < _items.size() ) return _items[index];
	return NULL;
}

}
