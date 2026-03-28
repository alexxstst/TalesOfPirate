#include "StdAfx.h"
#include "uiboatform.h"
#include "uiform.h"
#include "uiedit.h"
#include "uilabel.h"
#include "tools.h"
#include "uiformmgr.h"
#include "character.h"
#include "uigoodsgrid.h"
#include "uitemplete.h"
#include "uiequipform.h"
#include "uitradeform.h"
#include "gameapp.h"
#include "uistartform.h"
#include "uibourseForm.h"
#include "uiItemCommand.h"

using namespace GUI;

//---------------------------------------------------------------------------
// class CBoat  �û�����
//---------------------------------------------------------------------------
CBoat::CBoat() 
: _chtBoat(0), _frmShipRoom(0)
, _grdHold(0), _nIndex(0) 
{
}

bool CBoat::Init( int n, CForm* ship, GuiDragInGridEvent evt )  // �û�����Ϣ��ʼ��
{
	_frmShipRoom = ship;
	if( !_frmShipRoom ) return false;

	_nIndex = n;

	_grdHold = dynamic_cast<CGoodsGrid*>(_frmShipRoom->Find("grdRoom"));
	if( !_grdHold ) 
	{
		g_logManager.InternalLog(LogLevel::Debug, "common", g_oLangRec.GetString(443));
		return false;
	}

	_grdHold->SetIsHint(false);
	_grdHold->SetParent(ship);
	_frmShipRoom->evtEscClose = _evtEscClose;	// �� ESC �ر��¼� add by Philip.Wu  2006-06-22

	// ���û���¼�����Ϊ��ʱ�����������κβ���
	if( evt )
	{
		_grdHold->evtBeforeAccept = evt;
		_grdHold->evtThrowItem = CEquipMgr::evtThrowItemEvent;
		_grdHold->evtSwapItem = CEquipMgr::evtSwapItemEvent;
		_grdHold->evtRMouseEvent = _evtHoldGridRMouse;
	}
	return true;
}

void CBoat::_evtHoldGridRMouse(CGuiData *pSender,CCommandObj* pItem,int nGridID)
{
	if (!pItem) return;  

	//�������Ҽ�
	if (g_stUIBourse.GetForm()->GetIsShow())
	{
		CItemCommand* pkItemCmd = dynamic_cast<CItemCommand*>(pItem);
		if (!pkItemCmd)	return;

		g_stUIBourse.SaleGoods(*pkItemCmd, nGridID);
	}
}

void CBoat::Reset()  //  �����ɫ�����ھ�����û�����Ϣ
{
	_chtBoat = NULL;
	_grdHold->Clear();
}


// �� ESC �رմ����¼�����������Ҳ����Ϊ����ʾ  add by Philip.Wu  2006-06-22
void CBoat::_evtEscClose( CForm* pForm )
{
	if(pForm)
	{
		pForm->SetIsShow(false);

		if(pForm->GetParent())
		{
			pForm->GetParent()->SetIsShow(false);
		}
	}
}


//---------------------------------------------------------------------------
// class CBoatMgr �û���������
//---------------------------------------------------------------------------
bool CBoatMgr::Init() // �û���������Ϣ��ʼ��
{
	CForm* frm[defMaxBoat+1] = { 0 }; 
	CForm* frmShipRoom = _FindForm("frmShipRoom"); // ���Ҵ�����
	if(!frmShipRoom) return false;
	frm[0] = frmShipRoom;

	CForm* frmClone = NULL;
	char buf[128] = { 0 };
	for( int i=1; i<defMaxBoat+1; i++ ) // 
	{
		frmClone = dynamic_cast<CForm*>( frmShipRoom->Clone() );
		if( !frmClone ) return false;

		sprintf( buf, "frmShipRoom%d", i );
		frmClone->SetName( buf );

		CFormMgr::s_Mgr.AddForm( frmClone, enumMainForm );
		frmClone->Init();

		frm[i] = frmClone;
	}

	GuiDragInGridEvent evtGrid = _evtDragToGoodsEvent;
	for( int i=0; i<defMaxBoat; i++ )
	{
		if( !_cBoats[i].Init( i, frm[i], evtGrid ) ) return false;
	}

	if( !_cOther.Init( defMaxBoat+1, frm[defMaxBoat], NULL ) )
		return false;

	_pHuman = NULL;
	return true;
}

void CBoatMgr::End()
{

}

bool CBoatMgr::AddBoat( CCharacter* pBoat )
{
	CBoat* p = GetFreeBoat();
	if( !p ) return false; //�Ѿ��õ�������� �����ٻ����ѵĴ�ֻ

	p->Link( pBoat );
	return true;
}

CBoat*	CBoatMgr::GetFreeBoat()   // �����Ѵ�ֻ
{ 
	for( int i=0; i<defMaxBoat; i++ ) // ������ô�ֻ
		if( !_cBoats[i].GetIsValid() )
			return &_cBoats[i];

	return NULL;
}

CBoat* CBoatMgr::FindBoat( unsigned int ulWorldID )  // �����û��е�ȫ����
{
	for( int i=0; i<defMaxBoat; i++ )
	{
		if( _cBoats[i].GetIsValid() && _cBoats[i].GetCha()->getAttachID()==ulWorldID )
		{
			return &_cBoats[i];
		}
	}

	return NULL;
}

CGoodsGrid*	CBoatMgr::FindGoodsGrid( unsigned int ulWorldID ) // ���Ҵ���Ʒ
{
	if( _pHuman && _pHuman->getAttachID()==ulWorldID )
		return g_stUIEquip.GetGoodsGrid();

	CBoat* pBoat = FindBoat( ulWorldID );
	if( pBoat ) return pBoat->GetGoodsGrid();

	return NULL;
}

CCharacter*	CBoatMgr::FindCha( unsigned int ulWorldID ) // ���Ҵ�ֻ����
{
	if( _pHuman && _pHuman->getAttachID()==ulWorldID ) return _pHuman;

	CBoat* pBoat = FindBoat( ulWorldID );
	if( pBoat ) return pBoat->GetCha();

	return NULL;
}

CCharacter* CBoatMgr::ChangeMainCha( unsigned int ulWorldID ) // �л�����
{
	CCharacter* pCha = FindCha( ulWorldID );
	if( pCha ) 
	{
		pCha->SetHide( FALSE );

		CGameScene* pScene = CGameApp::GetCurScene();
		if( pScene )
		{
			pScene->SetMainCha( pCha->getID() );
			g_stUIStart.ShowShipSailForm( pCha->IsBoat() );

			// add by Philip.Wu  2006-07-03  �л���ͼʱ�رս��׺�װ�����壨BUG������
			g_stUITrade.CloseAllForm();
			g_stUIEquip.CloseAllForm();
		}
	}
	else
	{
		{ char _buf[512]; snprintf(_buf, sizeof(_buf), g_oLangRec.GetString(444), ulWorldID); g_logManager.InternalLog(LogLevel::Error, "errors", _buf); }
	}
	return pCha;
}

void CBoatMgr::Clear()  //  ����û����д���Ϣ
{
	_pHuman = NULL;
	for( int i=0; i<defMaxBoat; i++ )
	{
		_cBoats[i].Reset();
	}
}

CCharacter* CBoatMgr::FindCha( CGoodsGrid* pGoods ) // ��װ��ƥ����Ҵ�ֻ������
{
	if( pGoods==g_stUIEquip.GetGoodsGrid() )
		return _pHuman;

	for( int i=0; i<defMaxBoat; i++ )
	{
		if( _cBoats[i].GetGoodsGrid()==pGoods )
			return _cBoats[i].GetCha();
	}
	return NULL;
}



