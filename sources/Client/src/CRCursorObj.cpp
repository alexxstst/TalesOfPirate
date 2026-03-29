#include "stdafx.h"
#include "CRCursorObj.h"
#include "crcircle.h"
#include "CRCursor.h"

CursorMgr::CursorMgr()
: _pCursor(NULL)
{
	for( int i=0; i<csEndCursor; i++ )
    {
        _pCursorArray[i] = 0;
    }
}

void CursorMgr::InitMemory()
{
	_pCursorArray[csNormalCursor] = new CRCursor;
	_pCursorArray[csCircleCursor] = new CCircle3DCursor;

	_pCursor = _pCursorArray[csNormalCursor];
}

void CursorMgr::SceneInit( CGameScene * p )
{
	for( int i=0; i<csEndCursor; i++ )
	{
        if( _pCursorArray[i] ) _pCursorArray[i]->Init(p);
	}
}

void CursorMgr::SceneClear()
{
	for( int i=0; i<csEndCursor; i++ )
	{
		if( _pCursorArray[i] ) _pCursorArray[i]->Clear();
	}
}

void CursorMgr::ClearMemory()
{
	for( int i=0; i<csEndCursor; i++ )
	{
        if( _pCursorArray[i] )  
        {
		    _pCursorArray[i]->Clear();
		    delete _pCursorArray[i];
		    _pCursorArray[i] = NULL;
        }
	}
}

void CursorMgr::SetCursor( unsigned int cursor )
{
	if( cursor>=csEndCursor ) return ;

	if( _pCursor==_pCursorArray[cursor] ) 
	{
		return;
	}

	if( _pCursor ) _pCursor->SetIsShow( false );

	_pCursor=_pCursorArray[cursor];

    if( _pCursor ) _pCursor->SetIsShow( true );
}
