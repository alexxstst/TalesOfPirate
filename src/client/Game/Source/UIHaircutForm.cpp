#include "StdAfx.h"

#include "UIHaircutForm.h"
#include "HairTool.h"
#include "Character.h"
#include "UIForm.h"
#include "UIFormMgr.h"
#include "UITextButton.h"
#include "UITemplete.h"
#include "UILabel.h"
#include "UIFastCommand.h"
#include "UIBoatForm.h"
#include "NetProtocol.h"
#include "hairrecord.h"
#include "Packetcmd.h"
#include "UI3DCompent.h"
#include "GameApp.h"
#include "UIItemCommand.h"
#include "UIEquipForm.h"

#include "GlobalVar.h"

namespace GUI
{
	//~ ==================================================================
	CHaircutMgr::CHaircutMgr()
	{
		m_pHairTools = new CHairTools;
	}

	//~ ==================================================================
	CHaircutMgr::~CHaircutMgr()
	{
		//delete m_pHairTools;
		SAFE_DELETE(m_pHairTools); // UI
	}

	//~ ==================================================================
	bool CHaircutMgr::Init()
	{
		CFormMgr &mgr = CFormMgr::s_Mgr;

		frmHaircut = mgr.Find("frmHead", enumMainForm);
		if ( !frmHaircut)
		{
			ToLogService("gui", "{}", g_oLangRec.GetString(615));
			return false;
		}
		frmHaircut->evtEntrustMouseEvent = _MainMouseHaircutEvent ;
		frmHaircut->evtClose = _MainOnCloseEvent;

		char szBuf[16];
		for (int i(0); i<defHAIR_MAX_ITEM; i++)
		{
			sprintf(szBuf, "cmdHead%d", i+1);
			cmdProp[i] = dynamic_cast<COneCommand *>(frmHaircut->Find(szBuf));
			if (!cmdProp[i])
				return Error(g_oLangRec.GetString(616),
							 frmHaircut->GetName(), szBuf);
		}

		lblHairColor = dynamic_cast<CLabel*>(frmHaircut->Find("labHairColor"));
		if (!lblHairColor)
			return Error(g_oLangRec.GetString(616),
						 frmHaircut->GetName(), "labHairColor");

		lblHairType = dynamic_cast<CLabel*>(frmHaircut->Find("labHair"));
		if (!lblHairType)
			return Error(g_oLangRec.GetString(616),
						 frmHaircut->GetName(), "labHair");

		lblHairFare = dynamic_cast<CLabel*>(frmHaircut->Find("labHairMoney"));
		if (!lblHairFare)
			return Error(g_oLangRec.GetString(616),
						 frmHaircut->GetName(), "labHairMoney");

		// 
		CTextButton* btnLeftColor = (CTextButton*)frmHaircut->Find("btnLeftColor");
		if( !btnLeftColor ) 
		{
			Error(g_oLangRec.GetString(616), 
				frmHaircut->GetName(), "btnLeftColor");
			return false;
		}
		btnLeftColor->evtMouseClick = _gui_event_left_color;

		CTextButton* btnRightColor = (CTextButton*)frmHaircut->Find("btnRightColor");
		if( !btnRightColor ) 
		{
			Error(g_oLangRec.GetString(616), 
				frmHaircut->GetName(), "btnRightColor");
			return false;
		}
		btnRightColor->evtMouseClick = _gui_event_right_color;

		CTextButton* btnLeftHair = (CTextButton*)frmHaircut->Find("btnLeftHair");
		if( !btnLeftHair ) 
		{
			Error(g_oLangRec.GetString(616), 
				frmHaircut->GetName(), "btnLeftHair");
			return false;
		}
		btnLeftHair->evtMouseClick = _gui_event_left_hair;

		CTextButton* btnRightHair = (CTextButton*)frmHaircut->Find("btnRightHair");
		if( !btnLeftHair ) 
		{
			Error(g_oLangRec.GetString(616), 
				frmHaircut->GetName(), "btnRightHair");
			return false;
		}
		btnRightHair->evtMouseClick = _gui_event_right_hair;

		C3DCompent* ui3dCreateCha = (C3DCompent*)frmHaircut->Find( "ui3dCreateCha" );
		if( ui3dCreateCha )
		{
			ui3dCreateCha->SetRenderEvent( _cha_render_event );
		}

		CTextButton* btnLeft3d = (CTextButton*)frmHaircut->Find("btnLeft3d");
		if (!btnLeft3d)
		{
			Error(g_oLangRec.GetString(45), 
				frmHaircut->GetName(), "btnLeft3d");
			return false;
		}
		btnLeft3d->evtMouseClick = _gui_event_left_rotate;
		btnLeft3d->evtMouseDownContinue = _gui_event_left_continue_rotate;

		CTextButton* btnRight3d = (CTextButton*)frmHaircut->Find("btnRight3d");
		if (!btnRight3d)
		{
			Error(g_oLangRec.GetString(45), 
				frmHaircut->GetName(), "btnRight3d");
			return false;
		}
		btnRight3d->evtMouseClick = _gui_event_right_rotate;
		btnRight3d->evtMouseDownContinue = _gui_event_right_continue_rotate;
		return true;
	}

	void CHaircutMgr::CloseForm()
	{
		if (frmHaircut->GetIsShow())
		{
			frmHaircut->SetIsShow(false);
		}
	}

	void CHaircutMgr::SwitchMap()
	{
		m_pCurrMainCha = 0;
	}



	//~ ==================================================================
	void CHaircutMgr::ShowHaircutForm()
	{
		//CCharacter* pCha = g_pGameApp->GetCurScene()->GetGoodsCha();
		//if (!pCha) return;

		CCharacter* pMainCha = g_stUIBoat.GetHuman();
		if (!pMainCha){ ToLogService("haircut", "{}", g_oLangRec.GetString(617)); return; }

		m_pHairTools->RefreshCha( pMainCha->GetDefaultChaInfo()->lID );
		//
		
		if( (m_dwHairTypeMaxNum = m_pHairTools->GetHairMax()) <= 0 ) 
		{
			g_pGameApp->MsgBox( g_oLangRec.GetString(618) );
			return;
		}

		//
		m_nChaRotate = 0;

		//
		frmHaircut->Show();

		CCharacter* pCha = CGameApp::GetCurScene()->AddCharacter( pMainCha->getTypeID() );
		if( !pCha ) { ToLogService("haircut", "{}", g_oLangRec.GetString(619)); return; }
		pCha->GetActor()->SetSleep();
		pCha->UpdataFace(pMainCha->GetPart());

		m_pCurrMainCha = pCha;
		Refresh(0, 0);
		m_pCurrMainCha->PlayPose( 1, PLAY_LOOP_SMOOTH );
	}

	//~ ==================================================================
	void CHaircutMgr::Refresh(DWORD dwHairType, DWORD dwHairColor)
	{
		m_dwHairTypeIndex = dwHairType;
		m_dwHairColorIndex = dwHairColor;

		CHairName* pHairName = m_pHairTools->GetHair(m_dwHairTypeIndex);
		if (!pHairName)
		{
			ToLogService("haircut", "{}", g_oLangRec.GetString(620));
			return;
		}


		if ( (m_dwHairColorMaxNum = pHairName->GetMax()) <= 0)
		{
			ToLogService("haircut", "{}", g_oLangRec.GetString(621));
			return;
		}

		CHairRecord* pHairRecord = pHairName->GetInfo(m_dwHairColorIndex);
		if (!pHairRecord) return;

		char szBuf[16];
		lblHairColor->SetCaption(pHairRecord->szColor);
		lblHairType->SetCaption(pHairRecord->szDataName);
		sprintf(szBuf, "%d", pHairRecord->dwMoney);
		lblHairFare->SetCaption(szBuf);

		CGoodsGrid* pGrid = g_stUIEquip.GetGoodsGrid();
		if (!pGrid) return;

		m_sScriptID = pHairRecord->nID;
		m_dwHairTypeIndex = dwHairType;
		m_dwHairColorIndex = dwHairColor;

		for ( int j(0); j<pGrid->GetMaxNum(); j++)
		{
			CCommandObj* pObj = pGrid->GetItem(j);
			if(pObj){
				CItemCommand* pItem = dynamic_cast<CItemCommand*>(pObj);
				if( !pItem ) {
					continue;
				}
				pItem->SetIsValid( true );
			}
		}

		// 
		CItemCommand* propItem = NULL;
		for (int i(0); i<defHAIR_MAX_ITEM; i++)
		{
			CItemRecord *pItemRecord = GetItemRecordInfo(pHairRecord->dwNeedItem[i][0]);
			if (!pItemRecord) continue;

			propItem = new CItemCommand( pItemRecord );
			cmdProp[i]->AddCommand( propItem );
			propItem->SetTotalNum( pHairRecord->dwNeedItem[i][1] );

			CItemCommand* pItem = NULL;
			int j=0;
			for (j=0; j<pGrid->GetMaxNum(); j++)
			{
				pItem = dynamic_cast<CItemCommand*>(pGrid->GetItem(j));
				if( !pItem ) continue;

				if( !pItem->GetIsValid() ) continue;

				if( pItem->GetItemInfo()->nID==pHairRecord->dwNeedItem[i][0])
				{
					if( pItem->GetTotalNum()>=(int)pHairRecord->dwNeedItem[i][1] )
						break;
				}
			}
			
			if (j < pGrid->GetMaxNum())
			{	//
				m_iGoodsIndex[i] = j;
				pItem->SetIsValid( false );
			}
			else
			{	// 
				m_iGoodsIndex[i] = -1;
				propItem->SetIsValid( false );
			}
		}
	
		// 
		if (m_pCurrMainCha)
		{
			// ,
			lwIByteSet* res_bs = g_Render.GetInterfaceMgr()->res_mgr->GetByteSet();
			BYTE loadtex_flag = res_bs->GetValue(OPT_RESMGR_LOADTEXTURE_MT);
			BYTE loadmesh_flag = res_bs->GetValue(OPT_RESMGR_LOADMESH_MT);
			res_bs->SetValue(OPT_RESMGR_LOADTEXTURE_MT, 0);
			res_bs->SetValue(OPT_RESMGR_LOADMESH_MT, 0);

			m_pCurrMainCha->ChangePart( enumEQUIP_HEAD, pHairRecord->dwItemID );

            res_bs->SetValue(OPT_RESMGR_LOADTEXTURE_MT, loadtex_flag);
            res_bs->SetValue(OPT_RESMGR_LOADMESH_MT, loadmesh_flag);

			m_pCurrMainCha->PlayPose( 1, PLAY_LOOP_SMOOTH );
		}
	}

	//~ ==================================================================
	void CHaircutMgr::RenderCha(int x,int y)
	{
		if( !m_pCurrMainCha ) return;

		g_Render.LookAt( D3DXVECTOR3( 11.0f, 36.0f, 10.0f ), D3DXVECTOR3( 8.70f, 12.0f, 8.0f ), MPRender::VIEW_3DUI );
		y +=100;

		MPMatrix44 old_mat = *m_pCurrMainCha->GetMatrix();
		m_pCurrMainCha->SetUIYaw(180 + m_nChaRotate);
		m_pCurrMainCha->SetUIScaleDis(9.0f * g_Render.GetScrWidth()/TINY_RES_X );
		m_pCurrMainCha->RenderForUI(x, y);
		m_pCurrMainCha->SetMatrix(&old_mat);

		g_Render.SetTransformView(&g_Render.GetWorldViewMatrix());
	}

	//~ ==================================================================
	void CHaircutMgr::RotateCha(eDirectType enumDirect)
	{
		m_nChaRotate += 180;
		m_nChaRotate += -((int)(enumDirect)) * 15;
		m_nChaRotate = (m_nChaRotate + 360) % 360;
		m_nChaRotate -= 180;

	}

	//~ ==================================================================
	void CHaircutMgr::ChangeHairColor(eDirectType enumDirect)
	{
		m_dwHairColorIndex += ((int)(enumDirect));

		// 
		m_dwHairColorIndex = (m_dwHairColorIndex + m_dwHairColorMaxNum) % m_dwHairColorMaxNum;

		Refresh(m_dwHairTypeIndex, m_dwHairColorIndex);
	}

	//~ ==================================================================
	void CHaircutMgr::ChangeHairType(eDirectType enumDirect)
	{
		m_dwHairTypeIndex += ((int)(enumDirect));

		// 
		m_dwHairTypeIndex = (m_dwHairTypeIndex + m_dwHairTypeMaxNum) % m_dwHairTypeMaxNum;

		Refresh(m_dwHairTypeIndex, 0);
	}

	//~ ==================================================================
	void CHaircutMgr::_MainMouseHaircutEvent(CCompent *pSender, int nMsgType, 
		int x, int y, DWORD dwKey)
	{
		if( nMsgType!=CForm::mrYes ) return;

		//
		stNetUpdateHair sNetUpdateHair;
		memset( &sNetUpdateHair, 0, sizeof(sNetUpdateHair) );
		sNetUpdateHair.sScriptID = g_stUIHaircut.m_sScriptID;
		for( int i=0; i<defHAIR_MAX_ITEM; i++ )
			sNetUpdateHair.sGridLoc[i] = g_stUIHaircut.m_iGoodsIndex[i];
		CS_UpdateHair( sNetUpdateHair );
	}

	//~ ==================================================================
	void CHaircutMgr::_MainOnCloseEvent( CForm* pForm, bool& IsClose )
	{
		if (!g_stUIHaircut.m_pCurrMainCha)
			return;

		g_stUIHaircut.m_pCurrMainCha->SetValid(false);
		g_stUIHaircut.Clear();

		stNetUpdateHair sNetUpdateHair;
		memset( &sNetUpdateHair, 0, sizeof(sNetUpdateHair) );
		CS_UpdateHair( sNetUpdateHair );
	}

	//~ ==================================================================
	void CHaircutMgr::_gui_event_left_color(CGuiData *sender, int x, int y, DWORD key )
	{
		g_stUIHaircut.ChangeHairColor(LEFT);
	}

	//~ ==================================================================
	void CHaircutMgr::_gui_event_right_color(CGuiData *sender, int x, int y, DWORD key )
	{
		g_stUIHaircut.ChangeHairColor(RIGHT);
	}

	//~ ==================================================================
	void CHaircutMgr::_gui_event_left_hair(CGuiData *sender, int x, int y, DWORD key )
	{
		g_stUIHaircut.ChangeHairType(LEFT);
	}

	//~ ==================================================================
	void CHaircutMgr::_gui_event_right_hair(CGuiData *sender, int x, int y, DWORD key )
	{
		g_stUIHaircut.ChangeHairType(RIGHT);
	}

	//~ ==================================================================
	void CHaircutMgr::_gui_event_left_rotate(CGuiData *sender, int x, int y, DWORD key )
	{
		g_stUIHaircut.RotateCha(LEFT);
	}
	
	//~ ==================================================================
	void CHaircutMgr::_gui_event_right_rotate(CGuiData *sender, int x, int y, DWORD key )
	{
		g_stUIHaircut.RotateCha(RIGHT);
	}
	
	//~ ==================================================================
	void CHaircutMgr::_gui_event_left_continue_rotate(CGuiData *sender )
	{
		g_stUIHaircut.RotateCha(LEFT);
	}
	
	//~ ==================================================================
	void CHaircutMgr::_gui_event_right_continue_rotate(CGuiData *sender )
	{
		g_stUIHaircut.RotateCha(RIGHT);
	}

	//~ ==================================================================
	void CHaircutMgr::_cha_render_event( C3DCompent *pSender, int x, int y)
	{
		g_stUIHaircut.RenderCha(x, y);
	}

	void CHaircutMgr::Clear()
	{
		CGoodsGrid* pGrid = g_stUIEquip.GetGoodsGrid();
		if (!pGrid) return;

		for ( int j(0); j<pGrid->GetMaxNum(); j++)
		{
			CItemCommand* pItem = dynamic_cast<CItemCommand*>(pGrid->GetItem(j));
			if( !pItem ) continue;

			pItem->SetIsValid( true );
		}
	}
}
