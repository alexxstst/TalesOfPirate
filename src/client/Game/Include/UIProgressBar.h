//----------------------------------------------------------------------
// :
// :lh 2004-07-19
// :
//	 :
// :2004-10-09
//----------------------------------------------------------------------
#pragma once
#include "uicompent.h"
#include "UIStep.h"

namespace GUI
{

class CProgressBar : public CCompent  
{
public:
	enum eStyle
	{
		btHorizontal=0, // ,
		btVertical,     // 
		btEnd,
	};
	enum ePic           // 
	{
		pcBackground=0,
		pcProgressbar,
		pcEnd,
	};
    enum eHintStyle     // hint:,
    {
        hsHintNum,
        hsHintPercent,
    };

public:
	CProgressBar(CForm& frmOwn, eStyle style=btHorizontal);
	CProgressBar(const CProgressBar& rhs);
	CProgressBar& operator=(const CProgressBar& rhs);
	virtual ~CProgressBar();
	GUI_CLONE(CProgressBar)

	virtual void		Init();
	virtual void		Render();
	virtual void		Refresh();
    virtual void        RenderHint( int x, int y );
	virtual bool	    MouseRun( int x, int y, DWORD key );
    virtual bool		IsHandleMouse()             {    return true;               }

	void				Start( DWORD time );

	// 
	bool				LoadImage( const std::string& file, int w=32, int h=32, bool isHorizontal=true );
	virtual void		SetAlpha( BYTE alpha )		{ _pImage->SetAlpha(alpha);		}

	virtual CGuiPic*	GetImage()					{ return _pImage;				}
	
  
	int                 GetFlashNum()               { return _nFlash;               }
	void                SetFlashNum(int v)          {  _nFlash = v ;                }   
	void                SetActiveMouse(bool v)      { _bActiveMouse = v ;   }       
	 
public:	// 
	GuiMouseEvent		evtMouseDown;
	GuiEvent			evtTimeArrive;

public:
	void				StepIt()					{ _step.Add(); _RefreshPos();	}
	void				StepBy( float v )			{ _step.Add(v); _RefreshPos();	}
	void				SetPosition( float v )		{ _step.SetPosition(v); _RefreshPos();	}
	float				GetPosition()				{ return _step.GetPosition();	}
    void                SetRange( float min, float max )    { _step.SetRange( min, max ); _RefreshPos();    }

	eStyle				GetStyle()					{ return _style;				}
	void				SetStyle( eStyle v )		{ if( v>=btHorizontal  && v<btEnd ) _style = v;         }
    void                SetHintStyle( eHintStyle v ){ _eHintStyle=v;                }
	bool				IsRuning()					{ return _dwEndTime!=0;			}

private:
	void				_Copy(const CProgressBar& rhs);		
    void				_RefreshPos();

private:
	CStep				_step;

	eStyle				_style;

	CGuiPic*			_pImage;					// +
	int                 _nFlash;                    //
	bool                _bActiveMouse;                 //() 
    eHintStyle          _eHintStyle;

	int					_nStart;

	DWORD				_dwEndTime;
	DWORD				_dwStartTime;

private:
	::MPTexRect*		_pTex;
	float				_fEnd;

};


}
