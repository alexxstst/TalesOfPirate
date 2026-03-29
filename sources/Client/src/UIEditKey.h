//----------------------------------------------------------------------
// :Edit
// :lh 2005-05-23
// :
//----------------------------------------------------------------------
#pragma once

namespace GUI
{

class CEditParse;
class CEditObj;
class CEditKey
{
public:		
	CEditKey();
	~CEditKey();

	void	Init();
	bool	SetFont( DWORD dwFont );
	void	SetColor( DWORD dwColor )		{ _dwFontColor=dwColor;		}

public:		// 
	bool	OnChar( char c );
	bool	OnKeyDown( int key );

	void	Render();	

private:
	void	AddChar( CEditObj* pObj );

private:
	CEditParse*		_pParse;

	char			_szEnter[16];				// buf
	int				_nEnterPos;

	bool			_IsReadyOnly;				// 

	DWORD			_dwFontColor;				// 
	int				_dwFontIndex;				// 

private:
	DWORD			_dwCurosrIndex;				// 

	DWORD			_dwCursorHeight;			// 
	DWORD			_dwCursorColor;				//  
	int				_nCursorX, _nCursorY;		// 
	bool			_IsShowCursor;
	DWORD			_dwCursorTime;
	DWORD			_dwCursorSpace;

};

}
