#include "stdafx.h"
#include "CRCursor.h"
#include "GameApp.h"
#include "effectobj.h"

CRCursor::CRCursor()
{
	_pCursorEff = NULL;
}

CRCursor::~CRCursor()
{
    SAFE_DELETE( _pCursorEff );
}

void CRCursor::Init(CGameScene * p)
{
    _pCursorEff = new CShadeEff;
    _pCursorEff->SetScene( p );
    _pCursorEff->Create("sel1.tga",1.0f,true,4,1);
    _pCursorEff->SetValid(TRUE);
    _pCursorEff->SetAlphaType(D3DBLEND_SRCALPHA,D3DBLEND_INVSRCCOLOR);
    _pCursorEff->setColor(0xff0000ff);
    _pCursorEff->setFrameTime(0.03f);
    _pCursorEff->setTexFrameTime(0.03f);

}

void CRCursor::Clear()
{
    if( _pCursorEff )
    {
        delete _pCursorEff;
        _pCursorEff = NULL;
    }
}

void CRCursor::FrameMove( DWORD dwTime )
{
 	_pCursorEff->FrameMove(dwTime);
}

void CRCursor::MoveTo(D3DXVECTOR3 &vPos) 
{ 
	_vPos = vPos; 
}

void CRCursor::Render()
{
	_pCursorEff->MoveTo(&_vPos);
	_pCursorEff->Render();
}

void CRCursor::MouseDown( int nButton )
{
	if(nButton==0)
	{
		_pCursorEff->Emission(1,&_vPos,NULL);
	}
}

void CRCursor::MouseUp( int nButton )
{
}
