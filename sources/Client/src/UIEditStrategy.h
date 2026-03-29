//----------------------------------------------------------------------
// :Edit
// :lh 2004-07-12
// :
//----------------------------------------------------------------------
#pragma once

namespace GUI
{

class CEditArticle;
class CEditChar;
class CEditRow;
class CEditSentence;
class CEditObj;

// ,
class CEditRow
{
public:
	CEditRow();

	void	SetPos( int x, int y );
	void	PushUnit( CEditObj* pObj, CEditObj* pAtom );
	void	Clear();
	void	Render();
	DWORD	GetWordCount()					{ return _dwWordCount;				}

	CEditObj*	GetObj( DWORD dwIndex )		{ if( dwIndex<_units.size() ) return _units[dwIndex]; return NULL;	}
	DWORD		GetObjNum()					{ return (DWORD)_units.size();		}

	DWORD		GetHeight()					{ return _dwHeight;					}
	DWORD		GetWidth()					{ return _dwWidth;					}

private:
	typedef std::vector<CEditObj*> units;
	units			_units;				// 
	DWORD			_dwWordCount;
	units			_atoms;				// 

	DWORD			_dwHeight;
	DWORD			_dwWidth;

};

class CEditTextObj;
class CEditControl;
class CEditStrategy;

class CEditParse
{
public:
	CEditParse()	{	_items.push_back( new CEditRow );	}

	// ,-1
	int		Insert( DWORD nIndex, CEditObj* pObj )			{ return -1;			}

	// 
	bool	Del( DWORD nStart, DWORD nEnd );

	// ,
	bool	GetPos( DWORD nIndex, int& x, int& y, DWORD& dwFont, DWORD& dwColor );

	// 
	bool	GetSelectRect( DWORD dwStart, DWORD dwEnd, std::vector<POINT>& pt );

public:	
	// 
	bool	InsertText( CEditTextObj* pText );
	bool	InsertControl( CEditControl* pControl );
	bool	InsertObj( CEditObj* pObj );

	// 
	int		CursorText( CEditTextObj* pText );
	int		CursorControl( CEditControl* pControl );
	int		CursorObj( CEditControl* pControl );

private:
	typedef std::vector<CEditRow*> items;
	items		_items;					// 

	DWORD		_dwRowLimit;			// 
	DWORD		_dwMaxWordLimit;		// 

};

// 
class CEditStrategy
{
public:
	CEditStrategy( CEditArticle* pActicle );
	~CEditStrategy();

	void	Init();
	bool	Append( CEditObj *pChar );
	void	Clear();
	void	Render();
	void	RefreshPos( int x, int y );

public:		// vistor
	void	ParseText( CEditTextObj* pText );
	void	ParseControl( CEditControl* pControl );
	void	ParseObj( CEditObj* pObj );

private:
	CEditRow*	_AppendToBackRow( CEditObj *pChar );

private:
	CEditArticle*	_pActicle;	

	typedef std::vector<CEditRow*> items;
	items			_items;			// 

};

inline void CEditStrategy::ParseObj( CEditObj* pObj )
{
	// 
	CEditRow* pRow = _AppendToBackRow( pObj );
	pRow->PushUnit( pObj, pObj );
}

inline void CEditStrategy::ParseControl( CEditControl* pControl )
{
	_items.push_back( new CEditRow );	
}

}
