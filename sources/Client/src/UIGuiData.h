//--------------------------------------------------------------
// ����:GUI�ӿ�
// ����:lh 2004-07-15
// ���˼��:CGuiDat�����ڴ棬�ṩ�ؼ��ӿ�
//		     CForm��ʾ�������¼����л��ؼ����㣬װ�ش洢��Դ�ļ�
// ע:����ʵ��ֻ�����ڶ������ɣ��ɳ���ʼʱ��̬����
//    �˳�ʱ�ͷ���Դ
// �޸�����:2004-10-26 by lh
// �޸�����:��CItemObj�Ľű����ܷ���ΪCUIScript,CItemRow֧�ֶ�̬
//--------------------------------------------------------------

#pragma once
#include <assert.h>
#include <vector>
#include <deque>
#include <map>
#include <string>
#include <algorithm>
#include "UICursor.h"
#include "UIFont.h"
#include "UIPicture.h"

// ���º�����֧��Gui��¡
#define GUI_CLONE(TName)	\
public:							\
	virtual CGuiData* Clone() { return new (##TName)(*this); }	\
	virtual bool Clone( CGuiData* rhs ) {						\
		##TName *p = dynamic_cast<##TName*>(rhs);				\
		if( !p ) return false;									\
		*this = *p;												\
		return true;											\
	}

namespace GUI
{
// ���ں���MouseRun�Ĳ���key��,�����Ϊ�ļ���
enum eMouseState
{
	Mouse_LDown		= 0x0001,
	Mouse_MDown		= 0x0002,
	Mouse_RDown		= 0x0004,
	Mouse_Down		= 0x0008,	// �м�����
	Mouse_LUp		= 0x0010,
	Mouse_MUp		= 0x0020,
	Mouse_RUp		= 0x0040,
	Mouse_Move		= 0x0080,
	Mouse_LDB		= 0x0100,
	Mouse_MDB		= 0x0200,
	Mouse_RDB		= 0x0400,
	Mouse_LClick	= 0x0800,	// ���������
	Mouse_MClick	= 0x1000,	
	Mouse_RClick	= 0x2000,	
};

enum eCompentAlign
{
	caNone,				// ���ı�λ��
	caLeft,				// �������
	caLeftUp,			// �����������Ͻ�
	caUp,
	caRightUp,
	caRight,
	caRightBottom,
	caBottom,
	caLeftBottom,	
	caClient,			// ����ȫ��
    caCenter,           // ��ȫ����
    caWidthCenter,      // ˮƽ����
    caHeightCenter,     // ���¾���
	caEnd,				// ��������������
};

const BYTE DROP_ALPHA = 0xA0;

class CCompent;
class CGuiData;
class CForm;

typedef void (*GuiMouseEvent) (CGuiData *pSender, int x, int y, DWORD key);
typedef bool (*GuiKeyDownEvent)(CGuiData *pSender, int& key );
typedef bool (*GuiKeyCharEvent)(CGuiData *pSender, char& key );
typedef void (*GuiEvent)(CGuiData *pSender);
typedef void (*GuiOnTimerEvent)(CGuiData *pSender,DWORD nEventID,PVOID lpData);
typedef void (*CompentFun)(CCompent* pSender, unsigned int index );
typedef bool (*GuiHotKeyEvent)(CForm *pSender, char key );
typedef void (*GuiItemClickEvent)(std::string strItem);

// GUI�����϶�
class CDrag
{
public:
	enum eState
	{
		stNone=0,
		stPress,
		stDrag,
		stEnd,
	};

public:
	CDrag();

	void			SetYare( unsigned int n )	{ if(n<30) _nYareLen = n; }

	bool			MouseRun( int x, int y, DWORD key );
	int				BeginMouseRun( CGuiData* gui, bool InRect, int x, int y, DWORD key );	// ȷ�����״̬,����0�޶���

	static CDrag*		GetDrag()		{ return _pDrag;	}
	static CGuiData*	GetParent()		{ return _pParent;	}

	static bool		IsDraging(CGuiData *p)	{ return _pParent==p;	}
	static void		SetSnapToGrid( DWORD dwWidth, DWORD dwHeight );

	static void		SetDragX( int x )	{ _nDragX = x;		}
	static void		SetDragY( int y )	{ _nDragY = y;		}

	static int		GetDragX()			{ return _nDragX;	}
	static int		GetDragY()			{ return _nDragY;	}

	static int		GetStartX()			{ return _nStartX;	}
	static int		GetStartY()			{ return _nStartY;	}

	static int		GetX()				{ return _nStartX + _nDragX;					}
	static int		GetY()				{ return _nStartY + _nDragY;					}

	void			Reset();

	void			SetIsMove( bool v ) { _IsMove = v;		}
	bool			GetIsMove()			{ return _IsMove;	}
	
	void			SetIsUseGrid(bool v){ _IsUseGrid = v;	}
	bool			GetIsUseGrid()		{ return _IsUseGrid;}

	int				GetState()			{ return _eState;	}

	void			SetDragInCursor( CCursor::eState v )	{ _crDragIn = v;	}
	CCursor::eState	GetDragInCursor()						{ return _crDragIn;	}
	void			SetDragCursor( CCursor::eState v )		{ _crDrag = v;		}
	CCursor::eState	GetDragCursor()							{ return _crDrag;	}

public:
	GuiMouseEvent	evtMouseDragBegin;			// ��ʼ�϶�
	GuiMouseEvent	evtMouseDragMove;			// �����϶��¼�
	GuiMouseEvent	evtMouseDragEnd;			// �϶�����

private:
	static CDrag*		_pDrag;					// �����϶��еĶ���
	static CGuiData*	_pParent;				// �����϶��е�gui

	static int			_nDragX, _nDragY;
	static int			_nStartX, _nStartY;

	int				_nYareLen;
	eState			_eState;

	bool			_IsMove;					// ���ƶ�������,ֱ�ӽ��ؼ��ƶ�
	bool			_IsUseGrid;					// �Ƿ�ʹ�ö������

	CCursor::eState	_crDragIn;					// �����϶������ڵĹ��
	CCursor::eState	_crDrag;					// �϶�ʱ�Ĺ��
    static 	CCursor::eState	_crNormal;			// ��������µĹ��

	static DWORD    _dwGridWidth, _dwGridHeight;// �������Ŀ���
	static DWORD	_dwMouseUpTime;				// ���ڿ�ʼ�϶���,�Ѿ�û��MouseUP��Ϣ,�����ȴ���ɿ��ﵽһ��ʱ���ȡ���϶�

};

// ���º�����֧��Gui��¡
#define ITEM_CLONE(TName)	\
public:							\
	virtual TName* Clone() { return new (##TName)(*this); }	\
	virtual bool Clone( CItemObj* rhs ) {					\
	##TName *p = dynamic_cast<##TName*>(rhs);				\
	if( !p ) return false;									\
	*this = *p;												\
	return true;											\
	}


// ��ʾ��GUI�����е�С����
class CItemObj		
{
public:
	CItemObj()							{}
	virtual ~CItemObj();
	ITEM_CLONE(CItemObj)

	virtual void	Render( int x, int y ){}

	virtual int		GetWidth()			{ return 0;						}
	virtual int		GetHeight()			{ return 0;						}

    virtual void    ReadyForHint( int x, int y, CCompent* pCompent ){}
    virtual void    RenderHint( int x, int y ){}

    virtual bool    HasHint()           { return false;                 }

	virtual const char*	GetString()		{ return "";					}
	virtual void		SetString( const char* str ){}

	virtual void        SetWidth(int v) {}

	virtual DWORD		GetColor()		{ return 0;						}
	virtual void		SetColor( DWORD c ){}

};

// GUI������
class CForm;
class CGuiData
{

public:
	CGuiData();
	CGuiData( const CGuiData& rhs );
	CGuiData& operator=( const CGuiData& rhs );
	virtual ~CGuiData();
	GUI_CLONE(CGuiData)

	virtual void		Init() {}			// �����ⲿ��ʼ������װ��ͼƬ��

	virtual void		Render(){}			// ��ʾ
	virtual void		Refresh();			// ����ˢ����ʾ�õľ�������

	// ������괦�����̣��Ѵ���������true
	virtual bool		MouseRun( int x, int y, DWORD key );
	virtual bool		MouseScroll( int nScroll )		{ return _IsMouseIn;}

	virtual void		SetParent( CGuiData* p )		{ _pParent = p;		}
	virtual void		SetAlign( eCompentAlign v ) {}
	virtual void		SetAlpha( BYTE alpha ) {}
	virtual	void		SetIsEnabled( bool v )			{ _bEnabled = v;	 }
	virtual void		SetMargin( int left, int top, int right, int bottom ) {}

	virtual CGuiPic*	GetImage()						{ return NULL;		}

	virtual CForm*		GetForm()						{ return NULL;		}
	virtual void		SetCaption( const char * str) {}	// ������ʾ���ı�
	virtual void		SetTextColor( DWORD color ) {}
	virtual void		SetIsDrag( bool v );

	virtual CCompent*	Find( const char* str ) { return NULL; }

	virtual void		DragRender(){}
    virtual void		SetIsShow( bool v )			    { _bShow = v;		}

    virtual void        RenderHint( int x, int y );

    virtual bool        IsAllowActive();            // �ж��Ƿ���Լ���

	virtual void		Reset()					{};	// �����ⲿ����,�ù���������ȹ���

public:
	static HWND		GetHWND();						// �õ�windows handle
	static CGuiData* GetGui( DWORD dwID ) {			// ���������õ��ؼ�����
        if( dwID==0 )
        {
            ToLogService("common", "NULL GUI");
            return NULL;
        }
		if( dwID > _AllGui.size() ) 
        {
            ToLogService("common", "BIG[{}] GUI", dwID);
            return NULL;
        }
		return _AllGui[dwID-1]; 
	}
	static void		ClearAllGui();					// ������пؼ�
	static void		InitMemory();					// ���������пؼ������

    static bool         SetHintItem( CItemObj* pObj );
    static CItemObj*    GetHintItem()                   { return _pHintItem;    }

public:
	DWORD		GetID()						{ return _dwID;					}

	const char* GetName()					{ return _strName.c_str();		}
	void		SetName( const char * name ){ _strName = name;				}

    void		SetHint( const char * str )	{ _strHint = str;			    }
    std::string		GetHint()	{ return _strHint;			    }

	void		SetPos(int nLeft, int nTop) { _nLeft = nLeft; _nTop = nTop; }
	int			GetLeft()					{ return _nLeft;				}
	int			GetTop()					{ return _nTop;					}
	int         GetRight()                  { return _nLeft + _nWidth ;     }
	int         GetBottom()                 { return  _nTop + _nHeight;     }

	int			GetX()						{ return _nX1;					}
	int			GetY()						{ return _nY1;					}
	int			GetX2()						{ return _nX2;					}
	int			GetY2()						{ return _nY2;					}

	void		SetSize( int w, int h )		{ _nWidth = w, _nHeight = h;	}
	int			GetWidth()					{ return _nWidth;				}
	int			GetHeight()					{ return _nHeight;				}

	bool		GetIsShow()					{ return _bShow;				}

	bool		GetIsEnabled()				{ return _bEnabled;				}

	bool		IsNormal()					{ return _bShow && _bEnabled;	}	

	CGuiData*	GetParent()					{ return _pParent;				}

	bool		GetIsDrag()					{ return _pDrag != NULL;		}
	CDrag*		GetDrag()					{ return _pDrag;				}
	
	void		SetPointer( void* v )		{ _pVoid = v;					}
	void*		GetPointer()				{ return _pVoid;				}

	void		ScriptSetShow( bool v )		{ _bShow = v;					}	// �ⲿ�ű���

	int			nTag;

public:
	
	bool		InRect(int x, int y)		{ return _IsMouseIn = ( x >= _nX1 && x<= _nX2 && y >=_nY1 && y <=_nY2 );	}
	bool        IsNoDrag(int x, int y, DWORD key);		// ���û�п�ʼ�϶�,���ҵ���������,����true

	static void SetScreen( float dx, float dy, float sx, float sy ) {
		_fDrawX = dx, _fDrawY = dy,  _fScreenX = sx,  _fScreenY = sy;
	}
	void		LineFrame();					// ��һ���߿�
	void        SelectChaLineFrame() ;          //��һ�����˿�

	void		LineThinFrame();				// ��һ��ϸ��
	void		FillFrame();					// ���

	static void	SetMousePos( int x, int y )	{ _nMouseX=x; _nMouseY=y;		}
	static int  GetMouseX()					{ return _nMouseX;				}
	static int  GetMouseY()					{ return _nMouseY;				}
	
	static void SetCursor( CCursor::eState v )	{ _eCursor = v;				}		// ������MouseRun��
	static CCursor::eState GetCursor()		{ return _eCursor;				}

protected:
	virtual void _AddForm() {}			// ���Լ����뵽������Form��

protected:
	typedef std::vector<CGuiData*> vcs;
	static vcs	_AllGui;		// ������еĿؼ���Դ

	DWORD		_dwID;			// ע��ؼ���ID-���������ⲿ���ɸı�

	CGuiData*	_pParent;

	std::string		_strName;				// ��ʶ
	int			_nWidth, _nHeight;		// ����
	int			_nLeft, _nTop;			// �������

	int			_nX1,_nX2, _nY1, _nY2;	// ��ʾ�õľ�������

	bool		_bShow;			// �Ƿ���ʾ
	bool		_bEnabled;		// �Ƿ����
	CDrag*		_pDrag;

	bool		_IsMouseIn;		// ����Ƿ���������
    std::string		_strHint;		// �ؼ���ʾ

	void*		_pVoid;

protected:
	int			_ScreenX( int x ) { return (int)(((float)(x)) / _fScreenX); }
	int			_ScreenY( int y ) { return (int)(((float)(y)) / _fScreenY); }
    void        _RenderHint( const char* str, int x, int y );

private:
	static		float		_fDrawX, _fDrawY,  _fScreenX,  _fScreenY;
	static		CGuiPic		_imgLevel,	_imgVertical;	// ���ڻ��߿��ˮƽ�ߺʹ�ֱ��
    static      CItemObj*   _pHintItem;

	static		int			_nMouseX,	_nMouseY;		// Ŀǰ������ڵ�λ��
	static		CCursor::eState		_eCursor;			// ��ʾ��gui�ϵĹ��
};

class CGuiTime;
typedef void (*GuiTimerEvent)(CGuiTime *pSender);

class CGuiTime
{
	friend class CFormMgr;
public:
	static  CGuiTime*	Create( DWORD dwInterval=0, GuiTimerEvent evt=NULL );
	bool	Release();					// �ⲿɾ������

	static void			FrameMove(DWORD dwTime);
	static  CGuiTime*	Find( DWORD id ); 

	void	SetInterval( DWORD v )	{ _dwInterval = v;		}
	DWORD	GetInterval()			{ return _dwInterval;	}

	DWORD	GetID()					{ return _dwIndex;		}

	void	SetUserID( DWORD v )	{ _nEventID=v;			}
	DWORD	GetUserID()				{ return _nEventID;		}
	
	void	SetUserPoint( PVOID v )	{ _lpData = v;			}
	PVOID	GetUserPoint()			{ return _lpData;		}
	
	bool	IsTime()				{	// �Ƿ񵽴�ʱ���,��������
		if( _IsTime ) {
			_IsTime = false;
			return true;
		}
	}

public: // event
	GuiTimerEvent	evtTime;

private:
	DWORD	_dwIndex;			// ΨһID
	DWORD 	_dwInterval;		// ������,������;

	bool	_IsTime;
	DWORD	_dwLastTime;

	// �û�����
	DWORD	_nEventID;
	PVOID	_lpData;

	bool	_IsRelease;

private:
	CGuiTime( DWORD dwInterval=0, GuiTimerEvent evt=NULL );
	~CGuiTime();
	void	OnTime( DWORD dwTime );

	typedef std::vector<CGuiTime*>	times;
	static times				_times;

};

// ��������
inline bool	CGuiData::IsNoDrag(int x, int y, DWORD key)
{ 
	InRect( x, y );
    if( _pDrag && _pDrag->BeginMouseRun(this, _IsMouseIn, x, y, key )==CDrag::stDrag ) return false;

	return _IsMouseIn; 
}

inline void CGuiData::_RenderHint( const char* str, int x, int y )
{
    CGuiFont::s_Font.FrameRender( str, x + 40, y );
}

inline void	CGuiTime::OnTime( DWORD dwTime )
{
	if( !_IsRelease && _dwInterval>0 && dwTime>_dwLastTime && (dwTime-_dwLastTime>_dwInterval) )
	{
		if( evtTime )
		{
			evtTime( this );
		}
		else
		{
			_IsTime = true;
		}

		_dwLastTime = dwTime;
	}
}
}

