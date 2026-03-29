#include "StdAfx.h"
#include "uieditstrategy.h"
#include "uieditdata.h"
#include "uirichedit.h"

using namespace GUI;
//---------------------------------------------------------------------------
// class CEditRow
//---------------------------------------------------------------------------
CEditRow::CEditRow() 
: _dwWordCount(0), _dwHeight(0), _dwWidth(0)
{
}

void CEditRow::PushUnit( CEditObj* pObj, CEditObj* pAtom )	
{ 
	int w, h;
	pObj->GetSize( w, h );
	_dwWidth += w;
	if( (DWORD)h > _dwHeight ) 
	{
		_dwHeight = (DWORD)h;
	}

	_units.push_back(pObj);	
	_dwWordCount+=pObj->GetWordCount();	

	_atoms.push_back( pAtom );
}

void CEditRow::Clear()
{
	for( units::iterator it=_units.begin(); it!=_units.end(); it++ )
	{
		//delete (*it);
		SAFE_DELETE(*it); // UI
	}
	_units.clear();
	_atoms.clear();

	_dwWordCount = 0;
	_dwHeight = 0;
	_dwWidth = 0;
}

void CEditRow::Render()
{
	for( units::iterator it=_units.begin(); it!=_units.end(); it++ )
		(*it)->Render();
}

void CEditRow::SetPos( int x, int y )
{
	int w, h;
	for( units::iterator it=_units.begin(); it!=_units.end(); it++ )
	{
		(*it)->SetPos( x, y );
		(*it)->GetSize( w, h );
		x += w;
	}
}

//---------------------------------------------------------------------------
// class CEditStrategy
//---------------------------------------------------------------------------
CEditStrategy::CEditStrategy( CEditArticle* pActicle ) 
: _pActicle(pActicle)
{
}

CEditStrategy::~CEditStrategy()	
{ 
	Clear();	
}

bool CEditStrategy::Append( CEditObj *pChar )
{
	pChar->Parse( this );
	return true;
}

void CEditStrategy::Clear()
{
	for( items::iterator it=_items.begin(); it!=_items.end(); it++ )
	{
		//delete (*it);
		SAFE_DELETE(*it); // UI
	}
	_items.clear();
}

void CEditStrategy::Render()
{
	for( items::iterator it=_items.begin(); it!=_items.end(); it++ )
	{
		(*it)->Render();
	}
}

void CEditStrategy::Init()
{
}

void CEditStrategy::RefreshPos( int x, int y )
{
	for( items::iterator it=_items.begin(); it!=_items.end(); it++ )
	{
		(*it)->SetPos( x, y );
		y += (*it)->GetHeight();
	}
}

void CEditStrategy::ParseText( CEditTextObj* pText )
{
	// 
	CEditRow* pRow = _AppendToBackRow( pText );

	CEditSentence*	pSentence = NULL;	// 
	if( pRow->GetObjNum()>0 ) 
	{
		pSentence = dynamic_cast<CEditSentence*>( pRow->GetObj( pRow->GetObjNum()-1 ) );
		if( pSentence && pSentence->IsSameType( pText ) )
		{
			pSentence->AddCaption( pText->GetCaption() );
			return;
		}
	}

	pSentence = new CEditSentence;
	pRow->PushUnit( pSentence, pText );
	pSentence->AddCaption( pText->GetCaption() );
	return;
}

CEditRow* CEditStrategy::_AppendToBackRow( CEditObj *pChar )
{
	CEditRow* pRow = NULL;
	if( _items.empty() )
	{
		pRow = new CEditRow;
		_items.push_back( pRow );		
	}
	else
	{
		pRow = _items.back();
	}
	return pRow;
}
