#include "StdAfx.h"
#include "UICommand.h"
#include "uifastcommand.h"
#include "GameApp.h"
#include "Scene.h"

using namespace GUI;

//---------------------------------------------------------------------------
// class CCommandObj
//--------------------------------------------------------------------------
CCommandObj* CCommandObj::_pCommand = NULL;
CTextHint	 CCommandObj::_hints;

CCommandObj::~CCommandObj()
{
	if( _nFast>0 ) CFastCommand::DelCommand( this );
}

bool CCommandObj::ExecRightClick(){
	if( !CGameApp::GetCurScene() || !CGameScene::GetMainCha() ){
		return false;
	}
    _pCommand = NULL;
    if ( IsAllowUse() ){
		if ( IsAtOnce() ){
            bool isUse = true;
			if( GetParent() ){
				UseComandEvent pEvent = GetParent()->GetUseCommantEvent();
				if( pEvent ){
					pEvent( this, isUse );
				}
			}
			if( isUse ){
				
			
				return UseCommand(true);
			}else{
				return false;
			}
        }else{
            if( ReadyUse() ){
                _pCommand = this;
                return true;
            }
        }
    }
    Error();
    return false;     
}  




bool CCommandObj::Exec()         
{ 
	if( !CGameApp::GetCurScene() || !CGameScene::GetMainCha() ) return false;

    _pCommand = NULL;
    if ( IsAllowUse() )
    {
        if ( IsAtOnce() )
        {
            return _Exec();
        }
        else
        {
            if( ReadyUse() )
            {
                _pCommand = this;
                return true;
            }
        }
    }

    Error();
    return false;     
}

bool CCommandObj::_Exec()
{
    bool isUse = true;
    if( GetParent() )
    {
        UseComandEvent pEvent = GetParent()->GetUseCommantEvent();
        if( pEvent )
        {
            pEvent( this, isUse );
        }
    }
    if( isUse )
    {
        return UseCommand();
    }
    else
    {
        return false;
    }
}

bool CCommandObj::UserExec()
{
    if( _pCommand )
    {
        bool rv = _pCommand->_Exec();
        _pCommand = NULL;
        return rv;
    }

    return false;
}

bool CCommandObj::UseCommand(bool value)
{
    LG( "CCommandObj", "msgTest CCommandObj" );
    return true;
}

void CCommandObj::SetIsFast( bool v )
{
    if( v )
    {
        _nFast++;
    }
    else
    {
        _nFast--;
    }
}

bool CCommandObj::GetCanDrag( ){
	return true;
}

void CCommandObj::ReadyForHint( int x, int y, CCompent* pCompent )
{
	_hints.Clear();

    SetHintIsCenter( false );
    AddHint( x, y );

    if( GetIsFast() && pCompent )
    {
        CFastCommand* pFast = dynamic_cast<CFastCommand*>(pCompent);
        if( pFast )
        {
            AddHintHeight();

            int n = pFast->nTag % 12;

            char buf[32] = { 0 };
			if(pFast->topBar == true){
				sprintf( buf, "Shortcut: Shift + F%d", n + 1 );
			}else{
				sprintf( buf, g_oLangRec.GetString(492), n + 1 );
			}
            PushHint( buf );
        }
    }

	_hints.ReadyForHint( x, y );
}

void CCommandObj::RenderHint( int x, int y )
{
	_hints.Render();
}
