#include "StdAfx.h"
#include "uifont.h"
#include "gameapp.h"

#include "GlobalVar.h"


using namespace std;

using namespace GUI;
//---------------------------------------------------------------------------
// class CGuiFont
//---------------------------------------------------------------------------
CGuiFont	CGuiFont::s_Font;

int CGuiFont::CreateFont( const std::string& font, int size800, int size1024, DWORD dwStyle )
{
	stFont ft;
	ft.strFont	= font;
	ft.size800	= size800;
	ft.size1024 = size1024;
	ft.dwStyle	= dwStyle;

	_stfonts.push_back( ft );
	return (int)_stfonts.size() - 1;
}

void CGuiFont::FrameRender( const std::string& str, int x, int y )
{
	static int w, h, offx;

	GetSize( str, w, h );
	offx = GetRender().GetScreenWidth() - x - w - 10; // original: -10
	if( offx < 0 )
	{
		x += offx;
	}
	GetRender().FillFrame( x - 5, y - 3, x + w + 10, y + h + 5, 0x90000000 );
	CGuiFont::s_Font.Render( str, x - 1, y - 1, 0xffffffff );
}

void CGuiFont::TipRender( const std::string& str, int x, int y )
{
	static int w, h;

	GetSize( str, w, h );
	x -=  w / 2;
	GetRender().FillFrame( x - 2, y - 1, x + w + 2, y + h + 1, 0x90000000 );
	CGuiFont::s_Font.Render( str, x - 1, y - 1, 0xffffffff );
}

//---------------------------------------------------------------------------
// class CGuiFont
//---------------------------------------------------------------------------
CTextHint::CTextHint()
{
	_nHintW = 0; 
	_nHintH = 0;
	_nStartX = 0; 
	_nStartY = 0;
	_IsCenter = true;

	_eStyle = enumFontSize;
	_nFixWidth = 0;
	_dwBkgColor = 0x90000000;
	_IsShowFrame = true;

	_IsHeadShadow = true;
}

void CTextHint::Render()
{
	if( _hint.empty() )
		return;

	unsigned int sh = -1;

	static RECT rt;
	rt.left = _nStartX - 5;
	rt.top = _nStartY - 7;
	rt.right = _nStartX + _nHintW +5;
	rt.bottom = _nStartY + _nHintH + 2;
	GetRender().FillFrame( rt.left, rt.top, rt.right, rt.bottom, _dwBkgColor );

    int yy = _nStartY;

	auto it=_hint.begin();
	if( _IsHeadShadow )
	{
		if ( !(*it)->shadow )
		{
			DWORD color = (*it)->color;
			color = ( 0xff000000 & color ) | ~color;
			CGuiFont::s_Font.BRender( (*it)->font, (*it)->hint.c_str(), _nStartX + (*it)->offx, yy, (*it)->color, color );
			yy += (*it)->height;
			++it;
		}
		else
		{
			sh = (*it)->color;
			if ( (*it)->scolor)
				sh = (*it)->scolor;
			else
				sh = ( sh & 0xFF000000 ) | ~sh;

			CGuiFont::s_Font.BRender( (*it)->font, (*it)->hint.c_str(), _nStartX + (*it)->offx, yy, (*it)->color, sh );
		}
	}
    for( ; it!=_hint.end(); ++it )
    {
		if ( (*it)->shadow )
		{
			sh = (*it)->color;
			if ( (*it)->scolor)
				sh = (*it)->scolor;
			else
				sh = ( sh & 0xFF000000 ) | ~sh;

			CGuiFont::s_Font.BRender( (*it)->font, (*it)->hint.c_str(), _nStartX + (*it)->offx, yy, (*it)->color, sh );
		}
		else
		{
			CGuiFont::s_Font.Render( (*it)->font, (*it)->hint.c_str(), _nStartX + (*it)->offx, yy, (*it)->color );
		}
        yy += (*it)->height;
    }

	if( _IsShowFrame )
	{
		extern BOOL RenderHintFrame(const RECT* rc, DWORD color);
		RenderHintFrame( &rt, D3DCOLOR_ARGB(255,123,218,229) );
		rt.left--;
		rt.top--;
		rt.right++;
		rt.bottom++;
		RenderHintFrame( &rt, COLOR_BLACK );
	}

	if( _hint_related[0].empty() )
		return;

	int nStartX = _nStartX + _nHintW + 12;

	for( INT i = 0; i < enumEQUIP_NUM; ++ i )
	{
		if( _hint_related[i].empty() )
			break;

		static RECT rt;
		rt.left = nStartX - 5;
		rt.top = _nStartY - 7;
		rt.right = nStartX + _nRelatedHintW[i] + 5;
		rt.bottom = _nStartY + _nRelatedHintH[i] + 2;

		GetRender().FillFrame( rt.left, rt.top, rt.right, rt.bottom, _dwBkgColor );

		int yy = _nStartY;

		auto it=_hint_related[i].begin();

		if( _IsHeadShadow )
		{
			DWORD color = ( *it )->color;
			color = ( 0xff000000 & color ) | ~color;
			CGuiFont::s_Font.BRender( (*it)->font, (*it)->hint.c_str(), nStartX + (*it)->offx, yy, (*it)->color, color );
			yy += (*it)->height;
			++it;
		}

		for( ; it!=_hint_related[i].end(); ++it )
		{
			if ( (*it)->shadow)
			{
				sh = (*it)->color;
				if ( (*it)->scolor)
					sh = (*it)->scolor;
				else
					sh = ~sh | sh & 0xFF000000;

				CGuiFont::s_Font.BRender( (*it)->font, (*it)->hint.c_str(), _nStartX + (*it)->offx, yy, (*it)->color, sh );
			}
			else
			{
				CGuiFont::s_Font.Render( (*it)->font, (*it)->hint.c_str(), _nStartX + (*it)->offx, yy, (*it)->color );
			}
			yy += (*it)->height;
		}

		if( _IsShowFrame )
		{
			extern BOOL RenderHintFrame(const RECT* rc, DWORD color);
			RenderHintFrame( &rt, D3DCOLOR_ARGB(255,123,218,229) );
			rt.left--;
			rt.top--;
			rt.right++;
			rt.bottom++;
			RenderHintFrame( &rt, COLOR_BLACK );
		}

		nStartX += ( _nRelatedHintW[i] + 12 );
	}
}

void CTextHint::ReadyForHint( int x, int y )
{
    if( _hint.empty() ) return;

    // 
    _nStartX = x + 40;
    _nStartY = y;
    _nHintW = 0;
    _nHintH = 0;
    static int w, h;

    for( auto it=_hint.begin(); it!=_hint.end(); ++it )
    {
		CGuiFont::s_Font.GetSize( (*it)->font, (*it)->hint.c_str(), w, h );
        if( _nHintW<w ) _nHintW=w;
        (*it)->width = w;
        (*it)->height += h;
        _nHintH += (*it)->height;
    }

	if( _eStyle==enumFixWidth )
	{
		_nHintW = _nFixWidth;
	}

    if( _hint.size()>1 && _IsCenter )
    {
        for( auto it=_hint.begin(); it!=_hint.end(); ++it )
        {
            (*it)->offx = ( _nHintW - (*it)->width ) / 2;
        }        
    }

    // hint
    int off = GetRender().GetScreenWidth() - _nStartX - _nHintW - 10;
    if( off<0 )
    {
        _nStartX += off;
    }

    off = GetRender().GetScreenHeight() - _nStartY - _nHintH - 10;
    if( off<0 )
    {
        _nStartY += off;
    }
}

//Add by sunny.sun 20080912GM
//Begin
void CTextHint::ReadyForHintGM(int x, int y)
{
	    if( _hint.empty() ) return;

    // 
    _nStartX = x + 40;
    _nStartY = y;
    _nHintW = 0;
    _nHintH = 0;
    static int w, h;

    for( auto it=_hint.begin(); it!=_hint.end(); ++it )
    {
		CGuiFont::s_Font.GetSize( (*it)->font, (*it)->hint.c_str(), w, h );
        if( _nHintW<w ) _nHintW=w;
        (*it)->width = w;
        (*it)->height += h;
        _nHintH += (*it)->height;
    }

	if( _eStyle==enumFixWidth )
	{
		if(GetRender().GetScreenWidth() == TINY_RES_X)
			_nHintW = _nFixWidth - 50;
		else if(GetRender().GetScreenWidth() == FULL_LARGE_RES_X)
				_nHintW = _nFixWidth+170;
		else
			 _nHintW = _nFixWidth+420;
			
	}

    if( _hint.size()>1 && _IsCenter )
    {
        for( auto it=_hint.begin(); it!=_hint.end(); ++it )
        {
            (*it)->offx = ( _nHintW - (*it)->width ) / 2;
        }        
    }

    // hint
    int off = GetRender().GetScreenWidth() - _nStartX - _nHintW - 10;
    if( off<0 )
    {
        _nStartX += off;
    }

    off = GetRender().GetScreenHeight() - _nStartY - _nHintH - 10;
    if( off<0 )
    {
        _nStartY += off;
    }
	
}
//End

void CTextHint::Clear()
{
    auto it=_hint.begin();
    for( ; it!=_hint.end(); ++it )
    {
        //delete *it;
		SAFE_DELETE(*it); // UI
    }
    _hint.clear();
}

#include <fstream>
int CTextHint::WriteText( const std::string& file )
{
	ofstream out( file, ios::app );

	int nLines = 0;
    auto it=_hint.begin();
	stHint* pHint = NULL;
    for( ; it!=_hint.end(); ++it )
	{
		nLines++;
		out << (*it)->hint << endl;
	}
	if( nLines>0 ) out << "\n======================================\n" << endl;
	return nLines;
}

//-----------------------------------------------------------------------------
//	class CTextScrollHint
//Add by sunny.sun20080804
//-----------------------------------------------------------------------------

CTextScrollHint::CTextScrollHint()
{
	_nHintW = 0; 
	_nHintH = 0;
	_nStartX = 0; 
	_nStartY = 0;
	_IsCenter = true;
	index = 0;
	num = 0;
	SetScrollNum = 1;
	m = 0;
	_eStyle = enumFontSize;
	_nFixWidth = 0;
	_dwBkgColor = 0x90000000;
	_IsShowFrame = true;

	_IsHeadShadow = true;
}

void CTextScrollHint::Render()
{
	if( _hint.empty() ) return;
	try
	{
		int scrollneedspace = 0;
		static RECT rt;
		int yy = _nStartY;
		//Modify by sunny.sun20080820
		if( GetRender().GetScreenWidth()==LARGE_RES_X )
		{
			rt.left = _nStartX - 5;
			rt.top = _nStartY - 7;
			rt.right = _nStartX + _nHintW + 160;
			rt.bottom = _nStartY + _nHintH + 2;
			scrollneedspace = SCROLLNEEDSPACE + 39;
		}
		else
		{
			rt.left = _nStartX - 5;
			rt.top = _nStartY - 7;
			rt.right = _nStartX + _nHintW - 55;
			rt.bottom = _nStartY + _nHintH + 2;
			scrollneedspace = SCROLLNEEDSPACE + 3 ;
		}

		if( RenderScrollNum < SetScrollNum )
		{
			auto it=_hint.begin();
			GetRender().FillFrame( rt.left, rt.top, rt.right, rt.bottom, _dwBkgColor );
			SpaceLength = "";
			if(num == 0)
			{	
				SpaceLength = "";
				for(int n = 0 ; n<scrollneedspace;n++)
					SpaceLength = SpaceLength+" ";
				CopyHints = SpaceLength + (*it)->hint;
				(*it)->hint = SpaceLength;
				m = 0;	
			}
			if(( index%5 ) == 0)			// Original: %10
			{
				(*it)->hint = CopyHints;

				if( m == CopyHints.length() )
				{
					m= 0;
					(*it)->hint="";
					RenderScrollNum++;
				}
				else if( m>(int)(CopyHints.length()-scrollneedspace-1) )
				{
					char c = (*it)->hint.at(m);
					if((unsigned char)c >=(unsigned char)0x80)
					{
						if(m>=(int)CopyHints.length())  m = 0;
						(*it)->hint = (*it)->hint.erase(0,m);
						m+=1;
					}
					else
						(*it)->hint = (*it)->hint.substr(m,CopyHints.length()-m);
				}
				else 
				{
					char c = (*it)->hint.at(m);
					char d = (*it)->hint.at(m+scrollneedspace-1);
					if((unsigned char)c >=(unsigned char)0x80||(unsigned char)d >=(unsigned char)0x80)
					{
						(*it)->hint = (*it)->hint.substr(m,scrollneedspace-1);
						m+=1;
					}
					else
						(*it)->hint = (*it)->hint.substr(m,scrollneedspace-1);
				}
				++m;	
				index = 0;
			}
			num++;
			CGuiFont::s_Font.Render( (*it)->font, (*it)->hint.c_str(), _nStartX + (*it)->offx, yy, (*it)->color );
			index++;	
			//End
			if( _IsShowFrame )
			{
				extern BOOL RenderHintFrame(const RECT* rc, DWORD color);
				RenderHintFrame( &rt, D3DCOLOR_ARGB(255,123,218,229) );
				rt.left--;
				rt.top--;
				rt.right++;
				rt.bottom++;
				RenderHintFrame( &rt, COLOR_BLACK );
			}
		}
	}
	catch(...)
	{
		try
		{
			for(auto it = _hint.begin();it!=_hint.end();it++)
			{
				stHint* st= *it;
				LG("sunny.txt", "hint error !%s\n", st->hint.c_str());
			}
			_hint.clear();
		}
		catch(...)
		{

		}
	}
}
void CTextScrollHint::ReadyForHint( int x, int y ,int SetNum )
{
	if( _hint.empty() ) return;

	// 
	_nStartX = x + 40;			// Was 40
	_nStartY = y;
	_nHintW = 0;
	_nHintH = 0;
	static int w, h;

	for( auto it=_hint.begin(); it!=_hint.end(); ++it )
	{
		CGuiFont::s_Font.GetSize( (*it)->font, (*it)->hint.c_str(), w, h );
		if( _nHintW<w ) _nHintW=w;
		(*it)->width = w;
		(*it)->height += h;
		_nHintH += (*it)->height;
	}

	if( _eStyle==enumFixWidth )		//enumFixWidth
	{
		_nHintW = _nFixWidth;
	}

	if( _hint.size()>1 && _IsCenter )
	{
		for( auto it=_hint.begin(); it!=_hint.end(); ++it )
		{
			(*it)->offx = ( _nHintW - (*it)->width ) / 2; //original :( _nHintW - (*it)->width ) / 2
		}        
	}

	// hint
	int off = GetRender().GetScreenWidth() - _nStartX - _nHintW - 10;
	if( off<0 )
	{
		_nStartX += off;
	}

	off = GetRender().GetScreenHeight() - _nStartY - _nHintH - 10;
	if( off<0 )
	{
		_nStartY += off;
	}
	num = 0;
	RenderScrollNum = 0;
	SetScrollNum = SetNum; //previously, SetNum
	CopyHints.clear();
}

void CTextScrollHint::Clear()
{
	auto it=_hint.begin();
	for( ; it!=_hint.end(); ++it )
	{
		//delete *it;
		SAFE_DELETE(*it); // UI
	}
	_hint.clear();
}


