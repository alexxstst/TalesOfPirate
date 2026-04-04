#include "StdAfx.h"
#include "uiimeinput.h"
#include "GameApp.h"
#include "GameConfig.h"

#pragma comment( lib, "Imm32.lib" )

using namespace GUI;

CImeInput	CImeInput::s_Ime;

extern HINSTANCE hInst;
CImeInput::CImeInput(void)
: _pImage(NULL),_ImmNameColor(0xf00000ff), _pList(NULL), _nShowX(0), _nShowY(0), _bIsFull(false), _hImc(NULL)
{
	memset( _strImmName, 0, sizeof(_strImmName) );
    memset( _strSBC, 0, sizeof(_strSBC) );
    memset( _strInput, 0, sizeof(_strInput) );
    memset( _strInterpunction, 0, sizeof(_strInterpunction) );
	memset( _strComposition, 0, sizeof(_strComposition) );
	memset( _strCandidate, 0, sizeof(_strCandidate) );
	
	SetSize( InputX, InputY );
}

CImeInput::~CImeInput(void)
{
	Clear();
}

void CImeInput::Clear()
{
	if( _hImc )
	{
		//::ImmAssociateContextEx( GetHWND(), NULL, IACE_DEFAULT );
		_hImc = NULL;
	}
	if( _pImage ) 
	{
		delete _pImage;
		_pImage = NULL;
	}
}

void CImeInput::Init()
{

}

bool CImeInput::HandleWindowMsg(DWORD dwMsg, WPARAM wParam, LPARAM lParam)
{
	if( !_bIsFull ) return false;

	switch (dwMsg)
	{
	case WM_INPUTLANGCHANGEREQUEST:		// 
    case WM_INPUTLANGCHANGE:
		{
			int len = ImmGetDescription((HKL)lParam, 0, 0);
			if( len > 0 )
			{
				_strImmName[0] = '[';
				ImmGetDescription((HKL)lParam, &_strImmName[1], len);

				_strImmName[5] = ']';
				_strImmName[6] = 0;

				_strComposition[0] = 0;
				_bIsShow = true;
			}
			else
			{
				_bIsShow = false;
				strncpy_s( _strImmName, sizeof(_strImmName), GetLanguageString(622).c_str(), _TRUNCATE);
			}
            return true;
		}
		break;
    case WM_IME_SETCONTEXT:
        {
            return true;
        }
        break;
	case WM_IME_COMPOSITION:                // 
		{
			_GetCompositionString( _strComposition, GCS_COMPSTR);
            return true;
		}
		break;
	case WM_IME_NOTIFY:
		{
			switch(wParam)
			{
				case IMN_CHANGECANDIDATE:
				case IMN_CLOSECANDIDATE:
				case IMN_OPENCANDIDATE:
					{
					    _GetCandidateList();
					break;
					}
                case IMN_SETCONVERSIONMODE:
                case IMN_SETSENTENCEMODE:
                case IMN_OPENSTATUSWINDOW:
                    {
                        _GetConversion();
                    }break;
			}
            return true;
		break;
		}
    case WM_IME_STARTCOMPOSITION:
    case WM_IME_ENDCOMPOSITION:
        {
            return true;
        }
        break;
	default: return false;
	}
	return false;
}

void CImeInput::Render()
{
	if( !_bIsShow || !_bIsFull ) return;

    if(!strlen(_strComposition) && !strlen(_strCandidate))
    {
        return;
    }

	_pImage->Render( _nShowX, _nShowY );
	CGuiFont::s_Font.Render( _strImmName, _nShowX + 7, _nShowY + 7, _ImmNameColor );
    CGuiFont::s_Font.Render( _strInput, _nShowX + 40, _nShowY + 7, _ImmNameColor );
    CGuiFont::s_Font.Render( _strSBC, _nShowX + 61, _nShowY + 7, _ImmNameColor );
    CGuiFont::s_Font.Render( _strInterpunction, _nShowX + 82, _nShowY + 7, _ImmNameColor );
	CGuiFont::s_Font.Render( _strComposition, _nShowX + 7, _nShowY + 25, _ImmNameColor );
	CGuiFont::s_Font.Render( _strCandidate, _nShowX, _nShowY + 50, _ImmNameColor );
}

bool CImeInput::_GetCompositionString( char* str, DWORD ImeValue )
{
	DWORD dwSize = ImmGetCompositionString(_hImc, ImeValue, NULL, 0);
	::ImmGetCompositionString(_hImc, ImeValue, str, dwSize);
	str[ dwSize ] = 0;
	return true;
}

bool CImeInput::_GetCandidateList()
{
	DWORD  dwSize = MAX_CHAR; 
	SAFE_DELETE( _pList );

	memset( _strCandidate, 0, sizeof(_strCandidate) );
	{
		if(dwSize = ImmGetCandidateList(_hImc,0x0,NULL,0))
		{
			_pList = (LPCANDIDATELIST)new char[dwSize];
			if(_pList)
			{
				ImmGetCandidateList(_hImc,0x0,_pList,dwSize);

				// 
				static char temp[256] = { 0 };
				_strCandidate[0]=0;
				if( !_pList->dwPageStart )
				{
					for(unsigned int i=_pList->dwPageStart+_pList->dwSelection; ( i < _pList->dwCount && i < _pList->dwPageStart + _pList->dwPageSize + _pList->dwSelection); i++)
					{
						sprintf(temp, " %d:%s\n", i+1-_pList->dwSelection, (char *)_pList + _pList->dwOffset[i]);
						strcat(_strCandidate, temp);
					}
				}
				else
				{
					for(unsigned int i=_pList->dwSelection; ( i < _pList->dwCount && i < _pList->dwPageStart + _pList->dwPageSize); i++)
					{
						sprintf(temp, " %d:%s\n", i+1-_pList->dwSelection, (char *)_pList + _pList->dwOffset[i]);
						strcat(_strCandidate, temp);
					}
				}
			}
		}
		return true;
	}

	return false;
}

bool CImeInput::_GetConversion()
{
    unsigned long sentence = 0;
    ImmGetConversionStatus(_hImc, &_lConversion, &sentence);
    if(_lConversion & 0x01)
    {
        _strInput[0] = '[';
        strncpy(&_strInput[1], "", 2);
        _strInput[3] = ']';
        _strInput[4] = 0;
    }
    else
    {
        _strInput[0] = '[';
        strncpy(&_strInput[1], "", 2);
        _strInput[3] = ']';
        _strInput[4] = 0;
    }
    if(_lConversion & 0x08)
    {
        _strSBC[0] = '[';
        strncpy(&_strSBC[1], "", 2);
        _strSBC[3] = ']';
        _strSBC[4] = 0;
    }
    else
    {
        _strSBC[0] = '[';
        strncpy(&_strSBC[1], "", 2);
        _strSBC[3] = ']';
        _strSBC[4] = 0;
    }
    if(_lConversion & 0x400)
    {
        _strInterpunction[0] = '[';
        strncpy(&_strInterpunction[1], "", 4);
        _strInterpunction[5] = ']';
        _strInterpunction[6] = 0;
    }
    else
    {
        _strInterpunction[0] = '[';
        strncpy(&_strInterpunction[1], ".,", 2);
        _strInterpunction[3] = ']';
        _strInterpunction[4] = 0;
    }
    return true;
}

void CImeInput::SetShowPos( int x, int y ) 
{
	_nShowX = x + 10;
	_nShowY = y;

	if( _nShowX >  _nScreenWidth )
	{
		_nShowX = _nScreenWidth;
	}

	if( _nShowY > _nScreenHeight )
	{
		//_nShowY = _nScreenHeight - InputY;
        _nShowY = GetRender().GetScreenHeight() - InputY;
	}
}

void CImeInput::SetScreen( bool isFull, int w, int h )
{
	//_bIsFull = isFull;
    _bIsFull = (TRUE == g_Config.m_bFullScreen);

	if( _bIsFull )
	{
		_nScreenWidth = w - GetWidth();
		_nScreenHeight = h - GetHeight();

		if( !_hImc )
		{
			_hImc = ImmGetContext( g_pGameApp->GetHWND() );
		}

		if( !_pImage )
		{
			_pImage = new CGuiPic(NULL);
			_pImage->LoadImage( "texture/ui/ime/background.tga", 122, 172 );
			//_pImage->SetScale( GetWidth(), GetHeight() );
			//_pImage->GetImage()->dwColor = 0xa0ffffff;
		}
	}
	else
	{
		::ImmAssociateContextEx( g_pGameApp->GetHWND(), NULL, IACE_DEFAULT );
	}
}

void CImeInput::SetAlpha( BYTE alpha )
{ 
	_pImage->SetAlpha(alpha); 
	_ImmNameColor = (_ImmNameColor & 0x00ffffff) & (alpha << 24);
}
