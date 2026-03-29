#include "StdAfx.h"
#include "STNpcTalk.h"
#include "actor.h"
#include "Character.h"
#include "PacketCmd.h"
#include "NetProtocol.h"
#include "CharacterRecord.h"
#include "stmove.h"
#include "uinpctalkform.h"
#include "SceneItem.h"
#include "eventrecord.h"
#include "gameapp.h"
#include "event.h"
#include "uicursor.h"
#include "uiequipform.h"
#include "uiform.h"

//---------------------------------------------------------------------------
// class CNpcState
//---------------------------------------------------------------------------
CNpcState::CNpcState(CActor* p)
: CActionState(p), _pNpc(NULL)
{
    _IsSend = true;
    _IsOver = true;
}

bool CNpcState::_Start()
{
    if( !_pNpc ) return false;

    if( !_pNpc->IsNPC() ) return false;

    if( _pNpc->getAttachID()==0 ) return false;

    CCharacter* pSelf = GetActor()->GetCha();
    if( _pNpc->DistanceFrom(pSelf)>300 )
        return false;

    pSelf->FaceTo( _pNpc->GetCurX(), _pNpc->GetCurY() );

    CS_RequestTalk( _pNpc->getAttachID(), 0 );
	CNpcTalkMgr::SetTalkStyle( 0) ;
	return true;
}

//---------------------------------------------------------------------------
// class CNpcState
//---------------------------------------------------------------------------
CShopState::CShopState(CActor* p)
: CActionState(p), _pShop(NULL)
{
    _IsSend = true;
    _IsOver = true;
}

bool CShopState::_Start()
{
    if( !_pShop ) return false;

    if( !_pShop->IsPlayer() ) return false;

	if( !_pShop->IsShop() ) return false;

    if( _pShop->getAttachID()==0 ) return false;

    CCharacter* pSelf = GetActor()->GetCha();
    if( _pShop->DistanceFrom(pSelf)>500 )
        return false;

    pSelf->FaceTo( _pShop->GetCurX(), _pShop->GetCurY() );

	CS_StallOpen( _pShop->getAttachID() );
	return true;
}

//---------------------------------------------------------------------------
// CPickState
//---------------------------------------------------------------------------
CPickState::CPickState(CActor* p)
: CActionState(p), _pItem(NULL)
{
	_IsOver = true;		// 
    _IsSend = true;
}

CPickState::~CPickState()
{
}

bool CPickState::_Start()
{
	if( !_pItem || !_pItem->IsValid() || !_pItem->IsPick() ) return false;

    if( GetDistance( GetActor()->GetCha()->GetCurX(), GetActor()->GetCha()->GetCurY(), _pItem->GetCurX(), _pItem->GetCurY() ) > defPICKUP_DISTANCE )
        return false;

    stNetItemPick info;
    info.lWorldID	= _pItem->getAttachID();
    info.lHandle    = _pItem->lTag;
    CS_BeginAction( GetActor()->GetCha(), enumACTION_ITEM_PICK, &info );
	return false;
}

//---------------------------------------------------------------------------
// CEventState
//---------------------------------------------------------------------------
CEventState::CEventState(CActor* p)
: CActionState(p), _pNode(NULL), _pEvent(0)
{
	_IsOver = true;		// 
}

bool CEventState::_Start()
{
	if( !_pNode || !_pNode->IsValid() ) return false;

	CCharacter* pSelf = GetActor()->GetCha();
    if( GetDistance( pSelf->GetCurX(), pSelf->GetCurY(), _pNode->GetCurX(), _pNode->GetCurY() ) > 200 )
        return false;

	if( _pEvent->IsNormal() )
	{
		_pEvent->ExecEvent( pSelf );
		return true;
	}

	return false;
}

//---------------------------------------------------------------------------
// CRepairState
//---------------------------------------------------------------------------
CRepairState::CRepairState(CActor* p)
: CActionState(p)
{
}

bool CRepairState::_Start()
{
	_IsBeforeShow = g_stUIEquip.GetItemForm()->GetIsShow();
	if( !_IsBeforeShow ) g_stUIEquip.GetItemForm()->Show();
	CCursor::I()->SetCursor( CCursor::stRepair );
	return true;
}

void CRepairState::End()
{
	if( !_IsBeforeShow ) g_stUIEquip.GetItemForm()->Hide();
	CCursor::I()->SetCursor( CCursor::stNormal );
}

//---------------------------------------------------------------------------
// CFeedState
//---------------------------------------------------------------------------
CFeedState::CFeedState(CActor* p)
: CActionState(p)
{
}

bool CFeedState::_Start()
{
	_IsBeforeShow = g_stUIEquip.GetItemForm()->GetIsShow();
	if( !_IsBeforeShow ) g_stUIEquip.GetItemForm()->Show();
	CCursor::I()->SetCursor( CCursor::stFeed );
	return true;
}

void CFeedState::End()
{
	if( !_IsBeforeShow ) g_stUIEquip.GetItemForm()->Hide();
	CCursor::I()->SetCursor( CCursor::stNormal );
}



//---------------------------------------------------------------------------
// CFeteState		 add by Philip.Wu  2006-06-20  
//---------------------------------------------------------------------------
CFeteState::CFeteState(CActor* p)
: CActionState(p)
{
}

bool CFeteState::_Start()
{
	_IsBeforeShow = g_stUIEquip.GetItemForm()->GetIsShow();
	if( !_IsBeforeShow ) g_stUIEquip.GetItemForm()->Show();
	CCursor::I()->SetCursor( CCursor::stRepair );	// 
	return true;
}

void CFeteState::End()
{
	if( !_IsBeforeShow ) g_stUIEquip.GetItemForm()->Hide();
	CCursor::I()->SetCursor( CCursor::stNormal );
}

