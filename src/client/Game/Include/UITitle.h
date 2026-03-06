//----------------------------------------------------------------------
// :
// :lh 2005-05-23
// :
//----------------------------------------------------------------------
#pragma once
#include "uicompent.h"

namespace GUI
{

class CTitle : public CCompent
{
	enum ePicPos
	{
		enumLeft,
		enumCenter,
		enumRight,
		enumMax,
	};
public:
	CTitle(CForm& frmOwn);
	CTitle( const CTitle& rhs );
    CTitle& operator=(const CTitle& rhs);
	virtual ~CTitle(void);
	GUI_CLONE(CTitle)

	virtual void	Init();
	virtual void	Render();
	virtual void	SetCaption( const char * str );
	const char*		GetCaption()					{ return _strCaption.c_str();	}
	virtual CGuiPic*	GetImage()					{ return _pImage;				}
	virtual void		SetTextColor( std::uint32_t color ) { _dwColor = color;				}

	void			SetFont( unsigned int nFont )	{ _nFontIndex=nFont;			}
	void			SetFontH( int v )				{ _nFontH=v;			}
	void			SetShowTime( DWORD v )			{ _dwShowTime=v;				}

private:
	void	_Copy( const CTitle& rhs );

protected:
	CGuiPic*		_pImage;

	unsigned int	_nFontIndex;
	std::string			_strCaption;		// 
	std::uint32_t			_dwColor;			// 
	DWORD			_dwShowTime;
	int				_nFontH;

private:
	BYTE			_alpha;
	DWORD			_endtime;
	int				_posx[enumMax];		// 
	int				_posy;
	int				_posfonty;

};

}
