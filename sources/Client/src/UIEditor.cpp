#include "StdAfx.h"
#include "uiEditor.h"
#include "UIForm.h"
#include "UICheckBox.h"
#include "UIFormMgr.h"
#include "UIEdit.h"
#include "uiImage.h"
#include "UILabel.h"
#include "uitextbutton.h"
#include "UITreeView.h"
#include "uigraph.h"
#include "uiitem.h"
#include "Scene.h"
#include "UITemplete.h"
#include "GameApp.h"
#include "Character.h"
#include "GameConfig.h"
#include "MPEditor.h"
#include "streadydie.h"
#include "EffectObj.h"
#include "SceneObj.h"
using namespace std;
using namespace GUI;
extern MPEditor         g_Editor;

static bool isdigit( char key )
{
    return key>='0' && key<='9';
}
//---------------------------------------------------------------------------
// calss CEditor
//---------------------------------------------------------------------------
CEditor::CEditor()
{
}

bool CEditor::Init()
{
    if( !GlobalAppConfig.IsEditor() ) return true;

    CFormMgr &mgr = CFormMgr::s_Mgr;
    frmEditor = mgr.Find("frmEditor");
    if( !frmEditor ) 
    {
        g_logManager.InternalLog(LogLevel::Debug, "common", GetLanguageString(527).c_str());
        return false;
    }
    frmEditor->evtShow = _ShowEditor;
    frmEditor->evtHide = _HideEditor;

    trvEditor = dynamic_cast<CTreeView*>(frmEditor->Find("trvEditor"));
    if( !trvEditor ) return Error(GetLanguageString(45).c_str(), frmEditor->GetName(), "trvEditor" );
    trvEditor->evtMouseDown = _TreeMouseDown;
    trvEditor->evtItemLost = _TreeSelectLost;
    trvEditor->GetScroll()->SetPageNum( 5 );

    imgEditor =  dynamic_cast<CImage*>(frmEditor->Find("imgEditor"));
    if( !imgEditor ) return Error(GetLanguageString(45).c_str(), frmEditor->GetName(), "imgEditor" );

    btnColor =  dynamic_cast<CTextButton*>(frmEditor->Find("btnColor"));
    if( !btnColor ) return Error(GetLanguageString(45).c_str(), frmEditor->GetName(), "btnColor" );
    btnColor->evtMouseClick = _ColorMouseDown;
    btnColor->GetImage()->SetColor( g_Editor.GetColor() ); 

    chkSize01 =  dynamic_cast<CCheckBox*>(frmEditor->Find("chkSize01"));
    if( !chkSize01 ) return Error(GetLanguageString(45).c_str(), frmEditor->GetName(), "chkSize01" );

    chkSize02 =  dynamic_cast<CCheckBox*>(frmEditor->Find("chkSize02"));
    if( !chkSize02 ) return Error(GetLanguageString(45).c_str(), frmEditor->GetName(), "chkSize02" );

    chkSize03 =  dynamic_cast<CCheckBox*>(frmEditor->Find("chkSize03"));
    if( !chkSize03 ) return Error(GetLanguageString(45).c_str(), frmEditor->GetName(), "chkSize03" );

    chgGroup =   dynamic_cast<CCheckGroup*>(frmEditor->Find("chgGroup"));
    if( !chgGroup ) return Error(GetLanguageString(45).c_str(), frmEditor->GetName(), "chgGroup" );
    chgGroup->evtSelectChange = _CheckSelectChange;

    chkWireFrame =  dynamic_cast<CCheckBox*>(frmEditor->Find("chkWireFrame"));
    if( !chkWireFrame ) return Error(GetLanguageString(45).c_str(), frmEditor->GetName(), "chkWireFrame" );
    chkWireFrame->evtCheckChange = _WireCheckChange;

    chkEnabledAlpha =  dynamic_cast<CCheckBox*>(frmEditor->Find("chkEnabledAlpha"));
    if( !chkEnabledAlpha ) return Error(GetLanguageString(45).c_str(), frmEditor->GetName(), "chkEnabledAlpha" );
    chkEnabledAlpha->evtCheckChange = _EnabledAlphaCheckChange;
    chkEnabledAlpha->SetIsChecked( g_Editor.m_bEnableTextureAlphaBlend == TRUE );

    chkSmooth  =  dynamic_cast<CCheckBox*>(frmEditor->Find("chkSmooth"));
    if( !chkSmooth ) return Error(GetLanguageString(45).c_str(), frmEditor->GetName(), "chkSmooth" );
    chkSmooth->evtCheckChange = _SmoothCheckChange;
    chkSmooth->SetIsChecked( g_Editor.m_bSmooth );

    chkHideTree =  dynamic_cast<CCheckBox*>(frmEditor->Find("chkHideTree"));
    if( !chkHideTree ) return Error(GetLanguageString(45).c_str(), frmEditor->GetName(), "chkHideTree" );
    chkHideTree->evtCheckChange = _HideTreeCheckChange;
    chkHideTree->SetIsChecked( true );

    imgTreeFrame =  dynamic_cast<CFrameImage*>(frmEditor->Find("imgTreeFrame"));
    if( !imgTreeFrame ) return Error(GetLanguageString(45).c_str(), frmEditor->GetName(), "imgTreeFrame" );

    edtSetMainChaPos =  dynamic_cast<CEdit*>(frmEditor->Find("edtSetMainChaPos"));
    if( !edtSetMainChaPos ) return Error(GetLanguageString(45).c_str(), frmEditor->GetName(), "edtSetMainChaPos" );
    edtSetMainChaPos->evtKeyChar = _evtSetChaPosKeyChar;

    CEdit* edtShotMiniMap =  dynamic_cast<CEdit*>(frmEditor->Find("edtShotMiniMap"));
    if( edtShotMiniMap ) edtShotMiniMap->evtKeyChar = _evtShotMiniMap;

    lblShotMiniMap =  dynamic_cast<CLabel*>(frmEditor->Find("lblShotMiniMap"));

    edtSetBrushHeight =  dynamic_cast<CEdit*>(frmEditor->Find("edtSetBrushHeight"));
    if( !edtSetBrushHeight ) return Error(GetLanguageString(45).c_str(), frmEditor->GetName(), "edtSetBrushHeight" );
    edtSetBrushHeight->evtKeyChar = _evtSetBrushHeightKeyChar;
    char buf[10] = { 0 };
    itoa(g_Editor.m_nBrushHeight, buf, 10);
    edtSetBrushHeight->SetCaption( buf );

    chkModifyHeight =  dynamic_cast<CCheckBox*>(frmEditor->Find("chkModifyHeight"));
    if( !chkModifyHeight ) return Error(GetLanguageString(45).c_str(), frmEditor->GetName(), "chkModifyHeight" );
    chkModifyHeight->evtCheckChange = _ModifyHeightCheckChange;
    chkModifyHeight->SetIsChecked( g_Editor.m_bModifyHeight == TRUE );

    chkColor  =  dynamic_cast<CCheckBox*>(frmEditor->Find("chkColor"));
    if( !chkColor ) return Error(GetLanguageString(45).c_str(), frmEditor->GetName(), "chkColor" );
    chkColor->evtCheckChange = _ColorCheckChange;
    chkColor->SetIsChecked( g_Editor.m_bIsBrushColor );

    chkShowLightBrush  =  dynamic_cast<CCheckBox*>(frmEditor->Find("chkShowLightBrush"));
    if( !chkShowLightBrush ) return Error(GetLanguageString(45).c_str(), frmEditor->GetName(), "chkShowLightBrush" );
    chkShowLightBrush->evtCheckChange = _ShowLightBrushCheckChange ;
    chkShowLightBrush->SetIsChecked( g_Editor.m_bShowLightBrush );


    ckhHideOptionEx = dynamic_cast<CCheckBox*>(frmEditor->Find("ckhHideOptionEx"));
    if( !ckhHideOptionEx ) return Error(GetLanguageString(45).c_str(), frmEditor->GetName(), "ckhHideOptionEx" );
    ckhHideOptionEx->evtCheckChange = _HideOptionExCheckChange;
    ckhHideOptionEx->SetIsChecked( false );

    imgEditorEx =  dynamic_cast<CImage*>(frmEditor->Find("imgEditorEx"));
    if( !imgEditorEx ) return Error(GetLanguageString(45).c_str(), frmEditor->GetName(), "imgEditorEx" );

    lblSetMainChaPos = dynamic_cast<CLabel*>(frmEditor->Find("lblSetMainChaPos"));
    if( !lblSetMainChaPos ) return Error(GetLanguageString(45).c_str(), frmEditor->GetName(), "lblSetMainChaPos" );

    lblSetBrushHeight = dynamic_cast<CLabel*>(frmEditor->Find("lblSetBrushHeight"));
    if( !lblSetBrushHeight ) return Error(GetLanguageString(45).c_str(), frmEditor->GetName(), "lblSetBrushHeight" );

    ckhShowCompass =  dynamic_cast<CCheckBox*>(frmEditor->Find("ckhShowCompass"));
    if( !ckhShowCompass ) return Error(GetLanguageString(45).c_str(), frmEditor->GetName(), "ckhShowCompass" );
    ckhShowCompass->SetIsChecked( false );
    ckhShowCompass->evtCheckChange = _ShowCompassCheckChange;

    edtSetIslandIndex =  dynamic_cast<CEdit*>(frmEditor->Find("edtSetIslandIndex"));
    if( !edtSetIslandIndex )
        return Error(GetLanguageString(45).c_str(), frmEditor->GetName(), "edtSetIslandIndex" );
    edtSetIslandIndex->evtKeyChar = _evtSetIslandIndex;
    edtSetIslandIndex->SetIsShow(false);

    lblSetIslandIndex = dynamic_cast<CLabel*>(frmEditor->Find("lblSetIslandIndex"));
    if( !lblSetIslandIndex ) return Error(GetLanguageString(45).c_str(), frmEditor->GetName(), "lblSetIslandIndex" );
    lblSetIslandIndex->SetIsShow(false);

    chkEraseAttrib = dynamic_cast<CCheckBox*>(frmEditor->Find("chkEraseAttrib"));
    if (!chkEraseAttrib)
        return Error(GetLanguageString(45).c_str(), frmEditor->GetName(), "chkEraseAttrib");
    chkEraseAttrib->SetIsChecked(false);
    chkEraseAttrib->evtCheckChange = _EraseAttribCheckChange;
    chkEraseAttrib->SetIsShow(false);

    lblEraseAttrib = dynamic_cast<CLabel*>(frmEditor->Find("lblEraseAttrib"));
    if (!lblEraseAttrib)
        return Error(GetLanguageString(45).c_str(), frmEditor->GetName(), "lblEraseAttrib");
    lblEraseAttrib->SetIsShow(false);


    _HideOptionExCheckChange( ckhHideOptionEx );

    //g_Editor.Enable( frmEditor->GetIsShow() );
    return true;
}

void CEditor::_TreeSelectLost(CGuiData *pSender)
{
    CTreeView* tree = dynamic_cast<CTreeView*>(pSender);
    if( !tree ) return;

    g_stUIEditor._TreeEvent( tree, false );
}

void CEditor::_TreeMouseDown(CGuiData *pSender, int x, int y, DWORD key)
{
    CTreeView* tree = dynamic_cast<CTreeView*>(pSender);
    if( !tree ) return;

    bool press = (tree->GetSelect()->GetItem() != NULL);
    g_stUIEditor._TreeEvent( tree, press );
}

void CEditor::_TreeEvent(CTreeView* tree, bool press )
{
    if( !CGameApp::GetCurScene() ) return;

    if( press )
    {
        _ClearCheckState(NULL);
    }

    CTreeGridNode * select = dynamic_cast<CTreeGridNode*>(tree->GetSelectNode());
    if( !select ) return;

    CItem *item = dynamic_cast<CItem*>(select->GetItem());
    if( !item ) return;

    if( _stricmp( item->GetString(), GetLanguageString(528).c_str() )==0 )
    {
        CGraph *p = dynamic_cast<CGraph*>( tree->GetSelect()->GetItem() );
        if( p ) g_Editor.SetCha( press, p->nTag );
        return;
    }

    if( _stricmp( item->GetString(), GetLanguageString(529).c_str() )==0 )
    {
        CGraph *p = dynamic_cast<CGraph*>( tree->GetSelect()->GetItem() );
        if( p ) g_Editor.SetEff( press, p->nTag );
        return;
    }

    if( _stricmp( item->GetString(), GetLanguageString(530).c_str() )==0 )
    {
        CGraph *p = dynamic_cast<CGraph*>( tree->GetSelect()->GetItem() );
        if( p ) 
        {
            if( press )
            {
                g_Editor.SelectProperty( p->nTag );
                lblEraseAttrib->SetIsShow(true);
                chkEraseAttrib->SetIsChecked(false);
                chkEraseAttrib->SetIsShow(true);}
            else
            {
                g_Editor.CancelProperty();
                lblEraseAttrib->SetIsShow(false);
                chkEraseAttrib->SetIsChecked(false);
                chkEraseAttrib->SetIsShow(false);}
        }
        return;
    }

    if( _stricmp( item->GetString(), GetLanguageString(531).c_str() )==0 )
    {
        CGraph *p = dynamic_cast<CGraph*>( tree->GetSelect()->GetItem() );
        if( p ) 
        {
            if( press )
            {
                lblSetIslandIndex->SetIsShow(true);
                edtSetIslandIndex->SetCaption(""); // 
                edtSetIslandIndex->SetIsShow(true); // 
				CCompent::SetActive(edtSetIslandIndex); // 
            }
            else
            {
                edtSetIslandIndex->SetIsShow(false); // 
                lblSetIslandIndex->SetIsShow(false);
                g_Editor.cancelIsland();
            }
        }
        return;
    }

    if( _stricmp( item->GetString(), GetLanguageString(532).c_str() )==0 )
    {
        CGraph *p = dynamic_cast<CGraph*>( tree->GetSelect()->GetItem() );
        if( p && press ) 
        {
			g_pGameApp->Loading();
            g_pGameApp->GetCurScene()->SwitchMap( p->nTag );
            g_stUIEditor.SetEnabled( false );
            g_stUIEditor.trvEditor->SetSelectNode(NULL);
        }
        return;
    }

    if( _stricmp( item->GetString(), GetLanguageString(533).c_str() )==0 )
    {
        CGraph *p = dynamic_cast<CGraph*>( tree->GetSelect()->GetItem() );
        if( p ) g_Editor.SetChaPose( press, p->nTag );
        return;
    }

    if( _stricmp( item->GetString(), GetLanguageString(534).c_str() )==0 )
    {
        CGraph *p = dynamic_cast<CGraph*>( tree->GetSelect()->GetItem() );
        if( p ) 
        {
            CCursor::I()->SetCursor( (GUI::CCursor::eState)p->nTag ); 
        }
        return;
    }

    if( _stricmp(item->GetString(), GetLanguageString(535).c_str())==0 )
    {
        CGraph *p = dynamic_cast<CGraph*>( tree->GetSelect()->GetItem() );
        if( p )
        {
            CCharacter *pCha = CGameApp::GetCurScene()->GetMainCha();
            if( !pCha ) return;

            switch( p->nTag )
            {
            case 0:
                {
					pCha->PlayAni( pCha->GetDefaultChaInfo()->nBirthBehave, defCHA_DIE_EFFECT_NUM );

                    CEffectObj	*pEffect = pCha->GetScene()->GetFirstInvalidEffObj();
                    if( pEffect && pEffect->Create( pCha->GetDefaultChaInfo()->sBornEff ) )
                    {
                        pEffect->Emission( -1, &pCha->GetPos(), NULL );
                        pEffect->SetValid(TRUE);
                    }
                }
                g_pGameApp->AddTipText( "%s", GetLanguageString(536).c_str() );
                break;
            case 1:
                CReadyDieState* st = new CReadyDieState(pCha->GetActor());
                st->SetState( enumDied );
                pCha->GetActor()->InsertState(st);
                g_pGameApp->AddTipText( "%s", GetLanguageString(537).c_str() );
                break;
            }
        }
        return;
    }

    if( _stricmp( item->GetString(), GetLanguageString(538).c_str() )==0 )
    {
        CGraph *p = dynamic_cast<CGraph*>( tree->GetSelect()->GetItem() );
        if( p ) g_Editor.ChangePart( press, p->nTag );
        return;
    }

    CTreeNode * parent = dynamic_cast<CTreeNode*>(select->GetParent());
    if( !parent ) return;

    item = dynamic_cast<CItem*>( parent->GetItem() );
    if( !item ) return;

    if( _stricmp( item->GetString(), GetLanguageString(539).c_str() )==0 )
    {
        CGraph *p = dynamic_cast<CGraph*>( tree->GetSelect()->GetItem() );
        if( p ) g_Editor.SetTerr( press, p->nTag );
        return;
    }

    if( _stricmp( item->GetString(), GetLanguageString(540).c_str() )==0 )
    {
        CGraph *p = dynamic_cast<CGraph*>( tree->GetSelect()->GetItem() );
        if( p ) g_Editor.SetObj( press, p->nTag );
        return;
    }
}

void CEditor::_ColorMouseDown(CGuiData *pSender, int x, int y, DWORD key)
{
    DWORD color = g_stUIEditor.btnColor->GetImage()->GetImage()->dwColor;

    if( UIGetChooseColor( color ) ) g_stUIEditor.btnColor->GetImage()->SetColor( color );

    g_Editor.SetColor( color );
    g_stUIEditor.trvEditor->SetSelectNode(NULL);
}

void CEditor::_CheckSelectChange(CGuiData *pSender)
{
    CCheckGroup * g = dynamic_cast<CCheckGroup*>(pSender);
    if( !g ) return;

    g_Editor.m_nBrushNo = g->GetActiveIndex() + 1;
}

void CEditor::_HideTreeCheckChange(CGuiData *pSender)
{
    CCheckBox * g = dynamic_cast<CCheckBox*>(pSender);
    if( !g ) return;

    g_stUIEditor.trvEditor->SetIsShow( g->GetIsChecked() );
    g_stUIEditor.imgTreeFrame->SetIsShow( g->GetIsChecked() );
}

void CEditor::_WireCheckChange(CGuiData *pSender)
{
    CCheckBox * g = dynamic_cast<CCheckBox*>(pSender);
    if( !g ) return;

    g_Editor.SetWireFrame( g->GetIsChecked() );
}

void CEditor::_ClearCheckState( CCheckBox* pCheckBox )
{
    if( pCheckBox!=chkSmooth )
    {
        g_Editor.m_bSmooth = false;
        g_stUIEditor.chkSmooth->SetIsChecked(false);
    }

    /*  if (pCheckBox!= chkShowLightBrush)
    {
    g_Editor.m_bShowLightBrush = false ;
    g_stUIEditor.chkShowLightBrush->SetIsChecked(false );
    }*/

    if( pCheckBox!=chkColor )
    {
        g_stUIEditor.chkColor->SetIsChecked( false );
        g_Editor.m_bIsBrushColor = false;
    }
    if( pCheckBox!=chkModifyHeight )
    {
        g_Editor.m_bModifyHeight = false;
        g_stUIEditor.chkModifyHeight->SetIsChecked( false );
    }
}

void CEditor::_ModifyHeightCheckChange(CGuiData *pSender)
{
    CCheckBox * g = dynamic_cast<CCheckBox*>(pSender);
    if( !g ) return;

    g_Editor.m_bModifyHeight = g->GetIsChecked();
    g_stUIEditor.trvEditor->SetSelectNode(NULL);

    if( g->GetIsChecked() )
    {
        g_stUIEditor._ClearCheckState( g );
        g_stUIEditor.trvEditor->SetSelectNode(NULL);
    }
}

void CEditor::_SmoothCheckChange(CGuiData *pSender)
{
    CCheckBox * g = dynamic_cast<CCheckBox*>(pSender);
    if( !g ) return;

    g_Editor.m_bSmooth = g->GetIsChecked();

    if( g->GetIsChecked() )
    {
        g_stUIEditor._ClearCheckState( g );
        g_stUIEditor.trvEditor->SetSelectNode(NULL);
    }
}


void CEditor::_ShowLightBrushCheckChange(CGuiData *pSender)
{
    CCheckBox * g = dynamic_cast<CCheckBox*>(pSender);
    if( !g ) return;

    g_Editor.m_bShowLightBrush = g->GetIsChecked();

    if( g->GetIsChecked() )
    {
        //	g_stUIEditor._ClearCheckState( g );
        g_stUIEditor.trvEditor->SetSelectNode(NULL);
    }
}




void CEditor::_ColorCheckChange(CGuiData *pSender)
{
    CCheckBox * g = dynamic_cast<CCheckBox*>(pSender);
    if( !g ) return;

    g_Editor.m_bIsBrushColor = g->GetIsChecked();
    g_stUIEditor.trvEditor->SetSelectNode(NULL);

    if( g->GetIsChecked() )
    {
        g_stUIEditor._ClearCheckState( g );
        g_stUIEditor.trvEditor->SetSelectNode(NULL);
    }
}

void CEditor::_EnabledAlphaCheckChange(CGuiData *pSender)
{
    CCheckBox * g = dynamic_cast<CCheckBox*>(pSender);
    if( !g ) return;

    g_Editor.m_bEnableTextureAlphaBlend = g->GetIsChecked();
}

void CEditor::_ShowEditor(CGuiData *pSender)
{
    g_Editor.Enable( true );
}

void CEditor::_HideEditor(CGuiData *pSender)
{
    g_Editor.Enable( false );
}

void CEditor::SetEnabled( bool v )
{
    static int nOldTemplete = -2;
    if( v )
    {
        //nOldTemplete = CFormMgr::s_Mgr.GetFormTempletetNum();
        nOldTemplete = enumMainForm;

        CFormMgr::s_Mgr.SwitchTemplete( enumEditorForm );
        g_stUIEditor.frmEditor->Show();
    }
    else
    {
        if(g_stUIEditor.frmEditor)
        {
            g_stUIEditor.frmEditor->Hide();
            _ClearCheckState( NULL );
            g_stUIEditor.trvEditor->SetSelectNode(NULL);
        }
        if( nOldTemplete!=enumEditorForm )
        {
            CFormMgr::s_Mgr.SwitchTemplete( nOldTemplete );
        }
    }
    CFormMgr::s_Mgr.SetEnabled(v);
}

bool CEditor::_evtSetChaPosKeyChar(CGuiData *pSender, char& key)
{
    if( !CGameApp::GetCurScene() || !CGameApp::GetCurScene()->GetMainCha() ) return false;

    if( key ==',' || isdigit( key ) || key==VK_RETURN || key==VK_BACK || key=='-' )
    {
        if( key==VK_RETURN )
        {
            string str = g_stUIEditor.edtSetMainChaPos->GetCaption();
            size_t pos = str.find( ',' );
            if( pos!=string::npos )
            {
                int p[] = { 0, 0 };
                p[0] = atoi( str.substr(0,pos).c_str() );
                p[1] = atoi( str.substr(pos+1,str.length()).c_str() );
                CGameApp::GetCurScene()->GetMainCha()->setPos( p[0] * 100, p[1] * 100 );
                CGameApp::GetCurScene()->SetMainCha( CGameApp::GetCurScene()->GetMainCha()->getID() );
                g_pGameApp->AddTipText( "%s", GetLanguageString(541).c_str() );
            }
            else
            {
                g_pGameApp->AddTipText( "%s", GetLanguageString(542).c_str() );
            }
        }
        return true;
    }
    return false;
}

bool CEditor::_evtShotMiniMap(CGuiData *pSender, char& key)
{
    if( !CGameApp::GetCurScene() || !CGameApp::GetCurScene()->GetMainCha() ) return false;

    if( key ==',' || isdigit( key ) || key==VK_RETURN || key==VK_BACK || key=='-' )
    {
        if( key==VK_RETURN )
        {
            CEdit* pEdit = dynamic_cast<CEdit*>( pSender );
            if( !pEdit ) return false;

            const int SIZE = 4;
            int value[SIZE] = { 0, 0, 10, 10 };

            string strList[SIZE];
            int n = Util_ResolveTextLine( pEdit->GetCaption(), strList, SIZE, ',');            
            if( n!=4 )
            {
                g_pGameApp->MsgBox( "%s", GetLanguageString(543).c_str() );
                return true;
            }

            for (int i = 0; i<SIZE; i++)
            {
                value[i] = Str2Int(strList[i]);
            }

            g_pGameApp->SetStartMinimap( value[0], value[1], value[2], value[3] );
            g_pGameApp->EnableSprintSmMap(TRUE);
            g_pGameApp->MsgBox( "%s", SafeVFormat(GetLanguageString(544), value[0], value[1], value[2], value[3]).c_str() );
        }
        return true;
    }
    return false;
}

bool CEditor::_evtSetBrushHeightKeyChar(CGuiData *pSender, char& key)
{
    if( isdigit( key ) || key==VK_RETURN || key==VK_BACK || key=='-')
    {		
        if( key==VK_RETURN )
        {
            string str = g_stUIEditor.edtSetBrushHeight->GetCaption();
            if( str.empty() )
            {
                g_pGameApp->AddTipText( "%s", GetLanguageString(545).c_str() );
            }
            else
            {
                g_Editor.m_nBrushHeight = atoi( str.c_str() );
                g_pGameApp->AddTipText( "%s", GetLanguageString(546).c_str() );

				CEffectObj* pEffect = dynamic_cast<CEffectObj*>(g_Editor.GetSelectSceneObj());
				if (!pEffect)
				{
					g_pGameApp->MsgBox("null");
				}
            }
        }
        return true;
    }
    return false;
}

bool CEditor::_evtSetIslandIndex(CGuiData *pSender, char& key)
{
    if (key ==',' || isdigit( key ) || key==VK_RETURN || key==VK_BACK || key=='-')
    {
        if (key == VK_RETURN)
        {
            string str = g_stUIEditor.edtSetIslandIndex->GetCaption();
            int index = Str2Int(str);
            g_Editor.selectIsland(index);}
        return true;}
    return false;}


void CEditor::_HideOptionExCheckChange(CGuiData *pSender)
{
    CCheckBox * g = dynamic_cast<CCheckBox*>(pSender);
    if( !g ) return;

    bool isShow = g->GetIsChecked();

    g_stUIEditor.imgEditorEx->SetIsShow( isShow );
    g_stUIEditor.edtSetMainChaPos->SetIsShow( isShow );
    g_stUIEditor.edtSetBrushHeight->SetIsShow( isShow );
    g_stUIEditor.lblSetMainChaPos->SetIsShow( isShow );
    g_stUIEditor.lblSetBrushHeight->SetIsShow( isShow );

    if( g_stUIEditor.lblShotMiniMap ) g_stUIEditor.lblShotMiniMap->SetIsShow( isShow );
}

bool CEditor::GetIsShowCompass()
{
    return g_Editor.IsEnable() && ckhShowCompass->GetIsChecked();
}

void CEditor::_ShowCompassCheckChange(CGuiData *pSender)
{
    CGameScene* s = g_pGameApp->GetCurScene();
    if( !s ) return;

    s->ShowMinimap( !s->GetIsShowMinimap() );
}

void CEditor::_EraseAttribCheckChange(CGuiData* pSender)
{
    CCheckBox* cb = dynamic_cast<CCheckBox *>(pSender);
    bool chk = cb->GetIsChecked();
    if (chk) g_Editor.enable_erase();
    else g_Editor.enable_erase(false);
}

