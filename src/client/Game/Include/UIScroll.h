//----------------------------------------------------------------------
// :
// :lh 2004-07-20
// :,
// :2004-10-09
//----------------------------------------------------------------------
#pragma once
#include "uicompent.h"
#include "uitextbutton.h"
#include "uiimage.h"
#include "UIStep.h"
#include "uidragtitle.h"

namespace GUI
{

class CScroll : public CCompent
{
public:
	enum eStyle
	{
		btHorizontal=0, // ,
		btVertical,     //       
	};

CScroll(CForm& frmOwn);
	CScroll( const CScroll& rhs );
	CScroll& operator=( const CScroll& rhs );
	virtual ~CScroll(void);
	GUI_CLONE(CScroll)

	virtual void	Render();
	virtual void	Refresh();
	virtual bool	MouseRun( int x, int y, DWORD key );
	virtual bool	MouseScroll( int nScroll );
	virtual bool	OnKeyDown( int key );
	virtual void	Init();
    virtual void    FrameMove( DWORD dwTime );
	virtual bool	IsFrameMove()		{ return true;		}
	virtual void	SetAlpha( BYTE alpha );
    virtual void    RenderHint( int x, int y );	
    virtual CCompent* GetHintCompent( int x, int y );                       // 

	virtual bool    IsHandleMouse()		{ return true;		}

	bool			LoadImage( const std::string& bkg, const std::string& up, const std::string& scroll, const std::string& down, int w, int h );
	CTextButton*	GetUp()             { return _up;       }
	CTextButton*	GetDown()           { return _down;     }
	CDragTitle*		GetScroll()         { return _pScroll;  }
	CGuiPic*		GetImage()          { return _pImage;   }

	CStep&			GetStep()           { return _step;     }
	bool			SetRange( float min, float max );

	void			SetStyle( eStyle v );
	void			SetPageNum( unsigned int v )    { _fPageNum = (float)v;     }
	unsigned int	GetPageNum()        { return (unsigned int)_fPageNum;       }

	void			PageUp()            {_step.Add( _fPageNum );    }
	void			PageDown()          {_step.Sub( _fPageNum );    }
	void			Reset();
	void			SetAutoHide(bool v)	{ _IsAutoHide=v; }
	bool			GetAutoHide()		{ return _IsAutoHide;}

	// Add by lark.li 20080805 begin
	bool			StepMove(int val);
	// End

GuiEvent		evtChange;		// 

protected:
	void			_RefreshMidst();
	void			_SetSelf();
	void			_Copy( const CScroll& rhs );

static	void	_UpClick(CGuiData *pSender, int x, int y, DWORD key) {
		((CScroll*)(pSender->GetParent()))->_UpClick();
	}
	void _UpClick();

	static	void	_DownClick(CGuiData *pSender, int x, int y, DWORD key)	{
		((CScroll*)(pSender->GetParent()))->_DownClick();
	}
	void _DownClick();

	static	void	_DragBegin(CGuiData *pSender, int x, int y, DWORD key)	{
		((CScroll*)(pSender->GetParent()))->_DragBegin();
	}
	void			_DragBegin();

	static	void	_DragMove(CGuiData *pSender, int x, int y, DWORD key)	{
		((CScroll*)(pSender->GetParent()))->_CheckScrollDrag();
	}
	void			_CheckScrollDrag();

	void			_UpdataStep( int v );

CGuiPic*		_pImage;		// 

	CTextButton*	_up;
	CTextButton*	_down;
	CDragTitle*     _pScroll;

	eStyle			_eStyle;

	CStep			_step;
	int				_nStart;

	float			_fPageNum;		// 
	bool			_IsAutoHide;	// 

private:
	int				_nMinDrag, _nMaxDrag;

    static CTextButton*     _pContinueButton;
    static DWORD            _dwStartTime;

};

}
