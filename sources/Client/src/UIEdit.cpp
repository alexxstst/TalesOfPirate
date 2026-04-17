#include "Stdafx.h"
#include "UIText.h"
#include "UIEdit.h"
#include "uiimeinput.h"
#include "uitextparse.h"
#include "uitextbutton.h"
#include "InputBox.h"


using namespace GUI;

MPTexRect CEdit::_CursorImage;

int	CEdit::_nCursorFlashCount = 0;
bool CEdit::_bCursorIsShow = false;
int	CEdit::_nCursorX = 0;
int	CEdit::_nCursorY = 0;

CEdit::CEdit(CForm& frmOwn)
: CCompent(frmOwn), _bIsPassWord(false),  _bIsMulti(false), _bIsDigit(false), _bIsWrap(false), _nOffset(0)
, _nMaxNum(64),_nMaxNumVisible(64), _nFontHeight(24),_nMaxLineNum(0)
, _nCursorCol(0),_nCursorFirstCol(0),_nCursorSecondCol(0), _nCursorRow(0), _bParseText(false)
, _color(COLOR_BLACK), evtKeyDown(NULL), evtKeyChar(NULL), _nLeftMargin(0), _nTopMargin(2), evtEnter(NULL)
, evtChange(NULL), _pEnterButton(NULL), _nCursorColor(COLOR_BLACK)
{
	_IsFocus = true;
	_pImage = new CGuiPic( this );

}

CEdit::CEdit( const CEdit& rhs )
: CCompent(rhs), _pImage(new CGuiPic(*rhs._pImage) )
{
	_Copy( rhs );
}

CEdit& CEdit::operator=(const CEdit& rhs)
{
	CCompent::operator=(rhs);

	*_pImage = *rhs._pImage;

	_Copy( rhs );
	return *this;
}

CEdit::~CEdit(void)
{
	//delete _pImage;
	SAFE_DELETE(_pImage); // UI
}

void CEdit::_Copy( const CEdit& rhs )
{
	evtEnter = rhs.evtEnter;
	evtKeyDown = rhs.evtKeyDown;
	evtKeyChar = rhs.evtKeyChar;
	evtChange = rhs.evtChange;

	_color = rhs._color;
	_nLeftMargin = rhs._nLeftMargin;
	_nTopMargin = rhs._nTopMargin;
	_bParseText = rhs._bParseText;

	_str = rhs._str;
	_strVisible = rhs._strVisible;
	_nMaxNum = rhs._nMaxNum;
	_nMaxNumVisible = rhs._nMaxNumVisible;
	_bIsPassWord = rhs._bIsPassWord;
	_bIsMulti = rhs._bIsMulti;
	_bIsDigit = rhs._bIsDigit;
	_bIsWrap = rhs._bIsWrap;
	_nOffset = rhs._nOffset;

	_nFontHeight = rhs._nFontHeight;
	_nMaxLineNum = rhs._nMaxLineNum;
	_nCursorRow = rhs._nCursorRow;
	_nCursorCol = rhs._nCursorCol;
	_nCursorFirstCol = rhs._nCursorFirstCol;
	_nCursorSecondCol = rhs._nCursorSecondCol;

	_pEnterButton = rhs._pEnterButton;
}

bool CEdit::OnKeyDown( int key )
{
	if( !IsNormal() ) return false;

	if( evtKeyDown)	evtKeyDown(this, key) ;



	const char* s = _str.c_str() ;
	switch( key )
	{
	case VK_LEFT:
		{
			//begin:		
			if ( _nCursorCol >=3 )
			{
				if (! (s[_nCursorCol-1] & 0x80) )
				{				
					if ( isdigit( s[_nCursorCol-1])  && _nCursorCol >=3)
					{
						if ( isdigit( s[_nCursorCol-2])  &&(s[_nCursorCol-3]=='#')) 
						{
							_nCursorCol -=3;
							break;
						}					
					}
				}
			}
			//end:

			--_nCursorCol;
			if( _str[_nCursorCol] & 0x80 )	--_nCursorCol;
			

			if(_nCursorCol%13 == 0){
				if(GetIsMulti()){
				--_nCursorRow;
				
				}
			}
			if( _nCursorCol <= 0 ) 
			{
				_nCursorCol = 0;				
				if ( (int)_str.length() >=_nMaxNumVisible  ) //
				{	
					_strVisible = GetSelfString(_str , _nMaxNumVisible , true ) ;
				}
			}
			break;
		
		}
	case VK_RIGHT:
		if( _str.length() ==0 )		return false ;

		/*
		// begin:		
		if (  (s[_nCursorCol]=='#') && _nCursorCol >=0)
		{
			if ( isdigit( s[_nCursorCol+1])  &&isdigit( s[_nCursorCol+2])) 
			{
				_nCursorCol +=3;
				break;
			}					
		}		

		++_nCursorCol;
		if( GetIsMulti() )
		{
		}
		else
		{
			if( _str[_nCursorCol] & 0x80 ) ++_nCursorCol;
			if (_nCursorCol >= (int)_str.length() )
			{
				_nCursorCol = (int)_str.length() ;
			}		

			if( _nCursorCol >=_nMaxNumVisible  )
			{
				_nCursorCol = _nMaxNumVisible ;						
				_strVisible = GetSelfString( _str, _nMaxNumVisible , false );//
			}
		}*/
		break;
	case VK_UP:
		if( !GetIsMulti() ) return false;

		break;
	case VK_DOWN:
		if( !GetIsMulti() ) return false;

		break;
	case VK_HOME:
		{
			RefreshCursor();
			/*_nCursorCol = 0;
			if ( (int)_str.length() >_nMaxNumVisible  ) //
			{	
				_strVisible = GetSelfString(_str , _nMaxNumVisible , true ) ;  //
			}*/
		}
		break;
	case VK_END:
	{
		RefreshCursor();
		/*_nCursorCol = (int)_str.length();
		if ( (int)_str.length() >_nMaxNumVisible  ) //
		{
			_nCursorCol = _nMaxNumVisible;
			_strVisible = GetSelfString(_str , _nMaxNumVisible , false ) ; //
		}*/
		break;
	}
	case VK_PRIOR:	// pageup
		if( !GetIsMulti() ) return false;

		break;
	case VK_NEXT:	// pagedown
		if( !GetIsMulti() ) return false;

		break;
	case VK_DELETE:
		_Delete();
		_nCursorRow = (_str.size() +1)/15;
		break;
	default:
		
		if(GetIsMulti()){
			_nCursorRow = (_str.size() +1)/15;
			RefreshCursor();
		}
		else {
			return FALSE;
		}
	}
	
	_GetCursorPos(_nCursorCol);
	return true;
}

#include "resource.h"


void CEdit::RefreshCursor()
{
	_nCursorCol = g_InputBox.RefreshCursor();
	_GetCursorPos(_nCursorCol );
}

void CEdit::RefreshText()
{
	_str = g_InputBox.RefreshText();
	_strVisible = _str;
}

void CEdit::ClearText()
{
	g_InputBox.ClearText();
}

bool CEdit::OnChar( char c )
{	
	if( !IsNormal() ) return false;
	if( evtKeyChar && !evtKeyChar(this, c) ) return true;

	switch( c )
	{
	case  0: return true;
	case '\r':		// 
		if( _pEnterButton )
		{
			_pEnterButton->DoClick();
			return false;
		}

		if( !GetIsMulti() )
		{			
			if( evtEnter && !_str.empty() ) evtEnter(this);  // Edit,
		}
		else
		{
			//_nCursorCol = 0;
			//_nCursorRow++;
		}
		//ClearText();
		break;
	/*
	case '\b':		// 
		{
			/*try
			{

				const char* s = _str.c_str() ;
				int len = (int)_str.length() - 1;
				if ( _nCursorCol <=0 )
					return false;

				bool bHasPic = false;
				if( (int) _str.length() <=_nMaxNumVisible )
				{
					//begin:
					if ( _nCursorCol >=3 )
					{
						if (! (s[_nCursorCol-1] & 0x80) )
						{	

							if ( isdigit( s[_nCursorCol-1])  && _nCursorCol >=3)
							{
								if ( isdigit( s[_nCursorCol-2])  &&(s[_nCursorCol-3]=='#')) 
								{
									bHasPic =true;
								}					
							}
							if ( bHasPic )
							{
								string right =   GetSelfString( _str, (int)_str.length() - _nCursorCol , false  );
								_nCursorCol = _nCursorCol-3;
								string left =  GetSelfString (  _str, _nCursorCol , true  );
								_str = left +right ;
								_strVisible = _str ;
								_GetCursorPos(_nCursorCol );
								return false;
							}
						}
					}
					//end
					
					string right =   GetSelfString( _str, (int)_str.length() - _nCursorCol , false  );
					-- _nCursorCol ;

					if( _nCursorCol >= 0 )
					{
						if( (s[_nCursorCol] & 0x80) )
						{
							-- _nCursorCol ;					
						}				
					}
					string left =  GetSelfString (  _str, _nCursorCol , true  );
					
					_str = left +right ;
					_strVisible = _str ;
				}
				else if ( (int) _str.length()>=_nMaxNumVisible )
				{						
					if (_nCursorCol == _nMaxNumVisible)   //8
					{
						if( (s[len] & 0x80) )
						{
							if( _str.length() >= 2 )
								_str.erase( _str.end() - 2, _str.end() );
							else
								_str.erase( _str.end() - 1 );
						}
						else
						{
							_str.erase( _str.end() - 1 );
						}
						_strVisible = _str.substr( _str.length() -_nMaxNumVisible  ,_nMaxNumVisible );  
					}
					else 	//added by billy 
					{
						//
						string right = _str.substr( _nCursorCol + _str.length() - _nMaxNumVisible, _nMaxNumVisible - _nCursorCol ); 
						string left = _str.substr( 0 , _nCursorCol + _str.length() - _nMaxNumVisible  -1 ); 
						_str = left + right;               
						_strVisible = _str.substr( _str.length() - _nMaxNumVisible ,_nMaxNumVisible ); 
					}		
				}
			}
			catch(...)
			{
				MessageBox( 0, "CGameApp exception!", "APP_DEBUG", 0 );
			}

			break;
		}*/
	case '\t':		
		break;
	case 3:			// copy
		if( !GetIsPassWord() )
			_Copy();
			break;
	case 22:		// paste
		_Paste();
		break;
	case 24:		// cut
		if( !GetIsPassWord() )
			_Cut();
		break;
	case 27:		// ESC
		break ;
	//case '':
	case 'ff':
	case '{':
	case '}':
	case '[':
	case ']':
	//case '\\':
	case '~':	
	case '|':
	case '@':{
		DWORD dwThreadID = GetCurrentThreadId();
		HKL hCurKeyboard = GetKeyboardLayout(dwThreadID);
		unsigned int keyboard = reinterpret_cast<int> (hCurKeyboard);
		unsigned int layout = keyboard>>16;
		unsigned int lang = keyboard&0xFFFF;
		if(layout == 0x0407 || layout == 0xf012 ){
			_str+=c;
			SetCaption(_str.c_str());
			return true;
		}	
		break;
	}
	case '\b':
	default:
		
		break;/*
		if( GetIsMulti() )
		{

		}
		else
		{
			RefreshText();
			
			// 
			
			if( GetMaxNum()-1 <= (int)_str.length() ) return false;		
			if( GetIsDigit() && !isdigit( c ) ) return false;		
			if( GetIsPassWord() && !isprint( c ) ) return false;	// 
			

			if( (int)_str.length()  < _nMaxNumVisible )
			{
				string left =  _str.substr(0 , _nCursorCol);
				string right =  _str.substr( _nCursorCol ,(int) _str.length() - _nCursorCol );
                _str = left + c +right ;
				_strVisible = _str;
                ++_nCursorCol;
			}
			else if ((int) _str.length() >= _nMaxNumVisible  ) 
			{
				_str +=c ;
				++ _nCursorCol ; 
				_strVisible = _str;
				if ( _nCursorCol  > _nMaxNumVisible )
				{	
					_nCursorCol = _nMaxNumVisible ;
					_strVisible = _str.substr( _str.length() -_nMaxNumVisible ,_nMaxNumVisible );
				}
			}  	
		}*/
	}

	
    _GetCursorPos(_nCursorCol );
	if( GetIsDigit() && !_isdigit(c) )
	{
		return true;
	}
	//SetCaption(_str.c_str());	
    if( evtChange ) evtChange(this);
	return false;
}

bool CEdit::_IsCursorInHZ( long l, char * s )
{
	return false;
}

void CEdit::_Copy()
{
	RefreshCursor();
	
	int len = (int)_str.length();	// 
	// 

	if( !OpenClipboard( GetHWND() ) ) return;

	EmptyClipboard();

	HGLOBAL hData = GlobalAlloc( GMEM_MOVEABLE | GMEM_DDESHARE, len + 1 );
	if( !hData ) goto error;

	{
		char* pData = (char*)GlobalLock(hData);
		if (!pData) goto error;

		// 
		strncpy(pData, _str.c_str(), len);
		pData[len] = 0;

		if (evtChange) evtChange(this);

		GlobalUnlock(hData);
		SetClipboardData(CF_TEXT, hData);
	}
error:
	CloseClipboard();

}


void CEdit::_Paste() {
	RefreshText();

	if (!IsClipboardFormatAvailable(CF_TEXT)) {
		return;
	}

	if (!OpenClipboard(GetHWND())) {
		return;
	}

	HANDLE hData = GetClipboardData(CF_TEXT);
	if (!hData) goto error;

	{
		char* pData = (char*)GlobalLock(hData);

		if (!pData) goto error;

		if ((int)strlen(pData) + (int)_str.length() < GetMaxNum())
			_str += pData;

		if ((int)_str.length() <= _nMaxNumVisible)
		{
			_nCursorCol = (int)_str.length();
			_strVisible = _str;

		}
		else if ((int)_str.length() > _nMaxNumVisible)
		{
			_nCursorCol = _nMaxNumVisible;
			_strVisible = GetSelfString(_str, _nMaxNumVisible, false);
		}
		GlobalUnlock(hData);

		//added by billy 25/07/18
		SetCaption(_str.c_str());

		if (evtChange) evtChange(this);

	}

error:
	CloseClipboard();

}

void CEdit::_Cut()
{
	//RefreshText();
	//_Paste();
	//_Delete();
}

void CEdit::_Delete()    //
{
	//RefreshText();
	/*int len = (int)_str.length() ;
	const char* s = _str.c_str() ;
	string left,right ;

	//begin:
	bool bHasPic = false;
	if ( (s[_nCursorCol]=='#') && (_nCursorCol+3 <= len) )
	{
		if ( isdigit( s[_nCursorCol+1])  &&isdigit( s[_nCursorCol+2]) ) 
		{
			bHasPic = true;
		}
	}
	if( bHasPic)
	{		
		string left  = GetSelfString( _str,  _nCursorCol , true  );	
		string right = GetSelfString( _str ,(int)_str.length()- _nCursorCol -3 ,false );
		_str = left +right ;
		_strVisible = _str ;		
		return;
	}

	if(  (int)_str.length() <=_nMaxNumVisible  )
	{
		//
		if ( _nCursorCol  >0 )
			left = _str.substr (0, _nCursorCol) ;
		else
			left = "";

		if  (_nCursorCol   == len)
			right = "";
		else 
			if ( s[_nCursorCol] & 0x80 )
			{
				right = GetSelfString( _str ,(int)_str.length()- _nCursorCol -2 ,false );
			}
			else if ( isdigit( s[_nCursorCol]))
			{
				right = GetSelfString( _str ,(int)_str.length()- _nCursorCol -1 ,false );
			}
			else
			{
				right = GetSelfString( _str ,(int)_str.length()- _nCursorCol -1 ,false );
			}


		_str = left +right ;
	}
	else if ( (int)_str.length() > _nMaxNumVisible )
	{	
		left =GetSelfString( _str ,_nCursorCol , true );
		if ( s[_nCursorCol] & 0x80 )
			 right = GetSelfString( _str ,(int)_str.length()- _nCursorCol -2 ,false );
		else
			 right = GetSelfString( _str ,(int)_str.length()- _nCursorCol -1 ,false );	

		_str = left +right ;
		_strVisible = GetSelfString( _str, _nMaxNumVisible ,false ); 
	}	

    if( evtChange ) evtChange(this);
	*/
}

void CEdit::_UpdataLines()
{
	if( GetIsMulti() )
	{
	}
}

void CEdit::CorrectCursor()
{
	if (_bIsWrap)
	{
		int cursorPos=ui::GetWidth("S")*_nCursorCol;
		if (_nOffset+GetWidth()-5<cursorPos)
		{
			_nOffset=cursorPos-GetWidth()+5;
		}
		else if ( _nOffset>cursorPos )
		{
			_nOffset=cursorPos;
		}
		else if ( _nOffset>0 && _nOffset+GetWidth()-5>ui::GetWidth("S")*(int)(_str.length()) )
		{
			_nOffset=ui::GetWidth("S")*(int)(_str.length()) - GetWidth() +5;
			if ( _nOffset< 0 ) _nOffset=0;
		}
	}
	else
	{
		_nOffset=0;
	}
}


void CEdit::Render()
{
	if( GetActive()==this )
	{
		CorrectCursor();
		ShowFocus();
	}
	GetRender().SetClipRect( GetX(),GetY(), GetWidth(), GetHeight() );

	if( !GetIsPassWord() )
	{
		if( _bParseText ) 
		{
			if ((int) _str.size() > _nMaxNumVisible )
			{
				g_TextParse.Render( _strVisible, GetX() - _nOffset ,GetY() ,  _color );		
			}
			else
			{
				g_TextParse.Render( _str, GetX()  - _nOffset ,GetY(),  _color );		
			}
		}
		else
		{

			if ( (int) _str.size() >_nMaxNumVisible ) 
			{
				ui::Render( (char*)_strVisible.c_str(), GetX() - _nOffset ,GetY(), _color);
			}
			else
			{
				if(GetIsMulti()){
				try{
				int maxRows =  _str.size()/15 ;
				//_nCursorRow = maxRows;
				for(int i = 0; i<=maxRows; i++){
					char temp[15] = "";
					
					_str.copy(temp, 15, i*15);
					ui::Render(temp, GetX() - _nOffset, GetY() + ui::GetHeight("a")*i, _color);
					
				}}
				catch(...){
				}
				}else{
				ui::Render( (char*)_str.c_str(), GetX()- _nOffset ,GetY(), _color);		
				}
			}
		}	
	}
	else
	{
		// *  added by billy 
		char *cPassWord = new char[_nMaxNum + 1];
		memset( cPassWord, 0, _nMaxNum + 1 );
		int  length = (int)strlen ( (char*)_str.c_str() ) ;

		for (int i = 0 ; i<length ; i++)
			cPassWord[i] =	'*';

		ui::Render( (char*)cPassWord, GetX() - _nOffset, GetY(), _color );
	//	_pImage->Render( GetX(), GetY() );

		//delete [] cPassWord;
		SAFE_DELETE_ARRAY(cPassWord); // UI
	}

	GetRender().Reset();

}

void CEdit::Init()
{
	_nCursorHeight = ui::GetHeight( "" ) - 2;

	if( _pEnterButton && _pEnterButton->GetForm()!=GetForm() )
	{
		_pEnterButton = dynamic_cast<CTextButton*>( GetForm()->Find( _pEnterButton->GetName() ) );
	}
}

void CEdit::Refresh()
{
	CCompent::Refresh();
	_pImage->Refresh();

	if( GetActive()==this )
	{
		_GetCursorPos(_nCursorCol);
		OnActive();
	}
}

//void CEdit::_RefreshCursorPos()
//{
//	int cx = 0;
//	if( GetIsMulti() )
//	{
//		_nCursorY = GetY() + _nCursorRow * _nFontHeight;	// 
//	}
//	else
//	{
//		_nCursorY = GetY() + _nTopMargin;
//		cx = ui::GetWidth( _str.c_str() );	
//	}
//	_nCursorX =  (int)GetRender().DrawConvertX2((float)cx) + GetX() ;  //added by billy 
//
//	CImeInput::s_Ime.SetShowPos( _nCursorX, _nCursorY + _nFontHeight );
//}

void CEdit::_GetCursorPos(int nCurPos)
{
	int cx= 0;	
	//_nCursorRow =  _str.size()/15 ;
	if( GetIsMulti() )
	{
		
		_nCursorY = GetY() + _nCursorRow*ui::GetHeight("a");
		cx = ui::GetWidth( _str.substr(0, nCurPos%15).c_str() );	
		cx-=_nOffset;
	}
	else
	{
		_nCursorY = GetY() + _nTopMargin;
		cx = ui::GetWidth( _str.substr(0, nCurPos).c_str() );	
		cx-=_nOffset;
	}
	_nCursorX =  (int)GetRender().DrawConvertX2((float)cx) + GetX();  //added by billy

	CImeInput::s_Ime.SetShowPos( _nCursorX, _nCursorY + _nFontHeight );
}

//void CEdit::SetFocusEdit( CEdit* v ) 
//{ 
//    if( _pFocusEdit==v ) return;
//
//    if( v )
//    {
//        if( !v->GetForm()->GetIsShow() || !v->IsNormal() ) return;
//
//        v->_GetCursorPos(v->_nCursorCol);
//    }
//
//	_pFocusEdit=v; 
//}

void CEdit::SetCaption( const char * str) 
{ 
	if( GetActive()==this )
	{
		if(strlen(str) <= this->GetMaxNum()) // Add by lark.li 20080820
		{
			g_InputBox.SetText(str);
			g_InputBox.SetCursorTail();
			RefreshText();
			RefreshCursor();
		}
	}
	else
	{
		_str = str;
		_nCursorCol=(int)_str.length();
	}
	if( evtChange ) evtChange(this);
}

void CEdit::OnLost()
{
	RefreshCursor();
}

void CEdit::OnActive()
{
	CCompent::OnActive();

	::SetWindowPos( g_InputBox.GetEditWindow(), 0, GetX(), GetY(), 10000, 50, SWP_NOOWNERZORDER | SWP_NOREDRAW | SWP_HIDEWINDOW );

    //SetFocusEdit( this );
	g_InputBox.SetMaxNum(_nMaxNum);	
	g_InputBox.SetText(_str.c_str());

	if (!GetIsMulti() && _nCursorCol>=0 && _nCursorCol<=(int)(strlen(_str.c_str()))) //!GetIsMulti()
	{
		g_InputBox.SetSel(_nCursorCol,_nCursorCol);
	}

	//::SendMessage( g_InputEdit, EM_SETLIMITTEXT, 10000, 0 );
	
	//g_InputBox.SetCursorTail();
}

void CEdit::SetAlpha( BYTE alpha )
{ 
	_pImage->SetAlpha(alpha); 
	_color = (_color & 0x00ffffff) & (alpha << 24);
}

void CEdit::SetTextColor( DWORD color ) 
{ 
	_color = color;		
}

void CEdit::ReplaceSel( const char * str, BOOL bCanUndo )
{
	if( GetActive()==this )
	{
		g_InputBox.ReplaceSel(str);
	}
}
