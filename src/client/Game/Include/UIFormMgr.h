//--------------------------------------------------------------
// :
// :lh 2004-07-08
// :
// :,,,,
// :2004-10-09
//--------------------------------------------------------------
#pragma once
#include "uicompent.h"
#include "uiform.h"

namespace GUI
{
class CFormMgr;
typedef bool (*FormMgrEvent) ( CFormMgr* pSender );

typedef bool (*KeyDownEvent) (int& key);
typedef bool (*KeyCharEvent) (char& key);
typedef bool (*MouseEvent) (int& x, int& y, DWORD& mouse);
typedef bool (*MouseScrollEvent) (int& nScroll);
typedef bool (*HotKeyEvent) ( char& key, int& control );	// true


class CFormMgr
{
	friend class CForm;
public:	
	CFormMgr();
	~CFormMgr();

	static void		SetDebugMode(bool v) { _IsDebugMode=v;}		//-added by Arcol
	void			ShowDebugInfo();							// -added by Arcol
	static void		SetDrawFrameInDebugMode(bool v)	{_IsDrawFrameInDebugMode=v;}	// -added by Arcol
	static void		SetDrawBackGroundInDebugMode(bool v) {_IsDrawBackgroundInDebugMode=v;}	// -added by Arcol

	bool			AddForm( CForm* form, int templete=0 );

	bool			SetFormTempleteMax(int n);	
	unsigned int	GetFormTempleteMax()			{ return (unsigned int)_showforms.size();	}
	int				GetFormTempletetNum()			{ return _nTempleteNo;						}

	bool			SwitchTemplete( int n );		// ,-1,

public:
	bool			Init(HWND hWnd);
	void			Clear();

    void            FrameMove( int x, int y, DWORD dwMouseKey, DWORD dwTime );
	void			Render();
	void            RenderHint( int x, int y );
	bool			HandleWindowMsg(DWORD dwMsg, DWORD dwParam1, DWORD dwParam2);

	bool			OnKeyDown(int key);
	bool			OnKeyChar(char key);
	bool			OnHotKey( char key, int control );

	bool			MouseScroll( int nScroll );
	void			Refresh();

    void            MouseReset();

	CForm*			GetHitForm( int x, int y );

	CForm*			Find( const char * );			// Form
	CForm*			Find( const char *str, int no );// noForm
	CForm*			FindAll( const char* );			// Form

	CForm*			FindESCForm();					// ESCForm
	int				ModalFormNum()					{ return (int)_modal.size();		}

	typedef void (*FormFun)(CForm* pSender);
	void			ForEach( FormFun pFun );		// FormpFun

public:
	void			SetEnabled( bool v )			{ _bEnabled = v;	}
	bool			GetEnabled()					{ return _bEnabled; }
    void            ResetAllForm();
    void            SetScreen();

	void			SetEnableHotKey(int flag, bool v);		// 
	bool			GetEnableHotKey()				{ return _nEnableHotKey == 0xFFFFFFFF; }// 

	static bool		IsMouseInGui()					{ return _eMouseAction==enumMA_Gui;	}
	static eMouseAction	GetMouseAction()			{ return _eMouseAction;	}

public:	// 
	bool			AddFormInit( FormMgrEvent pInitFun );	// ,

	bool			AddKeyDownEvent( KeyDownEvent event );
	bool			DelKeyDownEvent( KeyDownEvent event );

	bool			AddKeyCharEvent( KeyCharEvent event );
	bool			DelKeyCharEvent( KeyCharEvent event );

	bool			AddMouseEvent( MouseEvent event );
	bool			DelMouseEvent( MouseEvent event );

	bool			AddMouseScrollEvent( MouseScrollEvent event );
	bool			DelMouseScrollEvent( MouseScrollEvent event );

	bool			AddHotKeyEvent( HotKeyEvent event );
	bool			DelHotKeyEvent( HotKeyEvent event );

public:
	static CFormMgr	s_Mgr;

private:
	bool			_AddMemory( CForm* form );
	bool			_DelMemory( CForm* form );

	bool			_MouseRun( int x, int y, DWORD mouse );

	void			_ShowModal( CForm* form );
    void            _ModalClose( CForm* form );

	void			_SetNewActiveForm();		// formform

	void			_InitFormID();

    void            _DelShowForm( CForm* frm );
    void            _AddShowForm( CForm* frm );
    void            _UpdataShowForm( CForm* frm );

    void            _ActiveCompent();

private:
	typedef std::list<CForm*> vfrm;
	vfrm			_allForms;
	vfrm*			_forms;						// ,
	vfrm			_modal;						// 
    vfrm            _show;                      // 

	typedef std::vector<vfrm*> frmtemplete;
	frmtemplete		_showforms;
	vfrm			_defaulttemplete;

	bool			_bEnabled;		
	bool			_bInit;
	int				_nEnableHotKey;		// 

	typedef std::vector<KeyDownEvent>	vkeydowns;
	vkeydowns		_OnKeyDown;

	typedef std::vector<KeyCharEvent>	vkeychars;
	vkeychars		_OnKeyChar;

	typedef std::vector<MouseEvent>		vmouses;
	vmouses			_OnMouseRun;
	
	typedef std::vector<MouseScrollEvent> vscrolls;
	vscrolls		_OnMouseScroll;

	typedef std::vector<FormMgrEvent>	vinits;
	vinits			_vinits;

	typedef std::vector<HotKeyEvent>		vhotkey;
	vhotkey			_vhotkey;

	int				_nTempleteNo;				// 

    int             _nMouseHover;               // 
    
    CGuiData*       _pHintGui;

	static eMouseAction		_eMouseAction;
	static bool		_IsDebugMode;				//  -added by Arcol
	static bool		_IsDrawFrameInDebugMode;	//  -added by Arcol
	static bool		_IsDrawBackgroundInDebugMode;	//  -added by Arcol

private:
	void			_DelForm( vfrm& list, CForm* frm );

};

inline void CFormMgr::MouseReset()                    
{ 
    _nMouseHover=0;     
}

}

