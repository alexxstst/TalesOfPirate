#include "StdAfx.h"
#include "UIChat.h"
#include "UITeam.h"
#include "uiformmgr.h"
#include "uitreeview.h"
#include "uiitem.h"
#include "uiprogressbar.h"
#include "uilabel.h"
#include "uigraph.h"
#include "uiboatform.h"
#include "smallmap.h"
#include "ui3dcompent.h"
#include "GameApp.h"
#include "netcommand.h"

#include "teaminviteformmgr.h"
#include "frndinviteformmgr.h"
#include "Character.h"
#include "uilist.h"
#include "PacketCmd.h"
#include "UICheckBox.h"
#include "UIEdit.h"
#include "UIForm.h"
#include "NetChat.h"
#include "UIMenu.h"
#include "chaticonset.h"
#include "talksessionformmgr.h"
#include "GameConfig.h"
#include <fstream>
#include "uiboxform.h"
#include "UITemplete.h"
#include "worldscene.h"
#include "GuildMemberData.h"
#include "GuildMembersMgr.h"
#include "UIStartForm.h"
#include "GuildData.h"
#include "autoattack.h"
#include "UICozeForm.h"

using namespace std;

using namespace GUI;

int SMALL_ICON_SIZE = 16;
int BIG_ICON_SIZE = 32;

CTeamMgr* CChat::_pTeamMgr=NULL;
CForm* CChat::_frmQQ=NULL;

CForm* CChat::_frmAddFriend=NULL;

CForm* CChat::_frmChangeSytle=NULL;
CForm* CChat::_frmEditMotto=NULL;
CForm* CChat::_frmDetails=NULL;
CForm* CChat::_frmBlockPlayer=NULL;
CList* CChat::_lstFrndPlayer=NULL;
CList* CChat::_lstGroupPlayer=NULL;
CList* CChat::_lstRoadPlayer=NULL;
CList* CChat::_lstStyle=NULL;
CMember* CChat::_curSelectMember=NULL;
CEdit* CChat::_pEditFrndName=NULL;

CEdit* CChat::_pEditMotto=NULL;
DWORD CChat::_dwSelfID=0;
DWORD CChat::_dwSelfIcon=0;
string CChat::_strSelfMottoName="";

CEdit* CChat::_pEditBlockName=NULL;

CTreeGridNode* CChat::_pGuildNode    = NULL;
CTreeGridNode* CChat::_pSessionNode  = NULL;
CTreeGridNode* CChat::_pMasterNode   = NULL;
CTreeGridNode* CChat::_pPrenticeNode = NULL;
CTreeGridNode* CChat::_pGMNode = NULL;
CTreeGridNode* CChat::_pBlockedNode  = NULL;

CMenu*	CChat::_frndMouseRight    = NULL;
CMenu*	CChat::_groupMouseRight   = NULL;
CMenu*	CChat::_roadMouseRight    = NULL;
CMenu*	CChat::_sessMouseRight    = NULL;
CMenu*	CChat::_MasterMouseRight  = NULL;
CMenu*	CChat::_StudentMouseRight = NULL;
CMenu*	CChat::_BlockedMouseRight = NULL;
CMenu*	CChat::_styMenu           = NULL;

CLabelEx* CChat::_labNameOfMottoFrm=NULL;
CImage*	CChat::_imgFaceOfMottoFrm=NULL;
CCheckBox*	CChat::_chkForbidOfMottoFrm=NULL;
vector<char*>	CChat::_strFilterTxt;

bool CChat::_bForbid=false;

CCharacter2D*	CChat::_pCharacter[MAX_MEMBER] = { 0 };
CLabelEx*		CChat::labMenberName[MAX_MEMBER] = { 0 };
CForm*			CChat::frmTeamMenber[MAX_MEMBER] = { 0 };
CProgressBar*   CChat::proTeamMenberHP[MAX_MEMBER] = { 0 };
CProgressBar*   CChat::proTeamMenberSP[MAX_MEMBER] = { 0 };
CLabelEx*       CChat::labLv[MAX_MEMBER] = { 0 };
CImage*		    CChat::imgWork[MAX_MEMBER] = { 0 };
CImage*		    CChat::imgLeader[MAX_MEMBER] = { 0 };

//---------------------------------------------------------------------------
// class CChat
//---------------------------------------------------------------------------

CChat::CChat()
:m_pQQTreeView(NULL), _pGroupNode(NULL), _pFrndNode(NULL)
{
	frmChatManage = NULL;

}

bool CChat::Init()
{
	//ifstream filterTxt("scripts\\table\\filter.txt",ios::in);
	//if (filterTxt.is_open())
	//{
	//	char buf[500]={0};
	//	filterTxt.getline(buf,500);
	//	while (!filterTxt.fail())
	//	{
	//		char *pText=new char[strlen(buf)+2];
	//		strcpy(pText,buf);
	//		_strFilterTxt.push_back(pText);
	//		filterTxt.getline(buf,500);
	//	}
	//	filterTxt.close();
	//}
	CFormMgr::s_Mgr.AddHotKeyEvent(CTalkSessionFormMgr::OnHotKeyShow);

    _pTeamMgr = new CTeamMgr;

    CFormMgr& mgr = CFormMgr::s_Mgr;

    // ��ʼ����ӿؼ�
    char szBuf[80] = { 0 };
    for( int i=0; i<MAX_MEMBER; i++ )
    {
        sprintf( szBuf, "frmTeamMenber%d", i+1 );
	    frmTeamMenber[i] = mgr.Find( szBuf );
	    if( !frmTeamMenber[i] ) return false;
	    frmTeamMenber[i]->SetIsShow(false);	
        frmTeamMenber[i]->Refresh();        

        sprintf( szBuf, "proTeamMenber%dHP", i+1 );
	    proTeamMenberHP[i] =  dynamic_cast<CProgressBar*> ( frmTeamMenber[i]->Find(szBuf) );
	    if( !proTeamMenberHP[i] ) return _Error( g_oLangRec.GetString(45), frmTeamMenber[i]->GetName(), szBuf );   
        proTeamMenberHP[i]->SetRange( 0.0f, 1.0f );

        sprintf( szBuf, "proTeamMenber%dSP", i+1 );
	    proTeamMenberSP[i] =  dynamic_cast<CProgressBar*> ( frmTeamMenber[i]->Find(szBuf) );
	    if( !proTeamMenberSP[i] ) return _Error( g_oLangRec.GetString(45), frmTeamMenber[i]->GetName(), szBuf );   
        proTeamMenberSP[i]->SetRange( 0.0f, 1.0f );

        sprintf( szBuf, "labMenber%dName", i+1 );
	    labMenberName[i] =  dynamic_cast<CLabelEx*> ( frmTeamMenber[i]->Find(szBuf) );
	    if( !labMenberName[i] ) return _Error( g_oLangRec.GetString(45), frmTeamMenber[i]->GetName(), szBuf );  

	    labLv[i] =  dynamic_cast<CLabelEx*> ( frmTeamMenber[i]->Find("labFLv") );
	    if( !labLv[i] ) return _Error( g_oLangRec.GetString(45), frmTeamMenber[i]->GetName(), "labFLv" );   

	    imgWork[i] =  dynamic_cast<CImage*> ( frmTeamMenber[i]->Find("imgobj") );
	    if( !imgWork[i] ) return _Error( g_oLangRec.GetString(45), frmTeamMenber[i]->GetName(), "imgobj" );
		
	    imgLeader[i] =  dynamic_cast<CImage*> ( frmTeamMenber[i]->Find("imgLeader") );
	    if( !imgLeader[i] ) return _Error( g_oLangRec.GetString(45), frmTeamMenber[i]->GetName(), "imgLeader" );

		C3DCompent* p3DDown = dynamic_cast<C3DCompent*>( frmTeamMenber[i]->Find( "d3dDown" ) );
		if( !p3DDown ) return _Error( g_oLangRec.GetString(45), frmTeamMenber[i]->GetName(), "d3dDown" ); 
		p3DDown->nTag = i;
		p3DDown->evtMouseDown = _MemberMouseDownEvent;
		p3DDown->SetMouseAction( enumMA_Skill );

		C3DCompent* p3D = dynamic_cast<C3DCompent*>( frmTeamMenber[i]->Find( "d3dHead" ) );
		if( !p3D ) return _Error( g_oLangRec.GetString(45), frmTeamMenber[i]->GetName(), "d3dHead" ); 
        p3D->SetRenderEvent( _RenderEvent );
		p3D->nTag = i;

        RECT rt;
        rt.left = p3D->GetX();
        rt.right = p3D->GetX2();
        rt.top = p3D->GetY();
        rt.bottom = p3D->GetY2();

        _pCharacter[i] = new CCharacter2D;
        _pCharacter[i]->Create( rt );
    }

	// ��ʼ��QQ����
	_frmQQ = mgr.Find("frmQQ");
	if( !_frmQQ )
	{
		g_logManager.InternalLog(LogLevel::Error, "errors", g_oLangRec.GetString(412));
		return false;
	}

	_frmQQ->SetIsShow(true);
	_frmQQ->evtShow=_evtQQMainShow;
	CGuiTime::Create(300,_OnTimerFlash);

	CTextButton* pbtnAdd = dynamic_cast<CTextButton*>(_frmQQ->Find("btnAdd"));
	if( !pbtnAdd ) return _Error(g_oLangRec.GetString(411), _frmQQ->GetName(), "btnAdd" );
	pbtnAdd->evtMouseClick=_evtQQMainAddFrnd;

	
	pbtnAdd = dynamic_cast<CTextButton*>(_frmQQ->Find("btnEdit"));
	if( !pbtnAdd ) return _Error(g_oLangRec.GetString(411), _frmQQ->GetName(), "btnEdit" );
	pbtnAdd->evtMouseClick=_evtQQMainEditMotto;


	pbtnAdd = dynamic_cast<CTextButton*>(_frmQQ->Find("btnBlock"));
	if( !pbtnAdd ) return _Error(g_oLangRec.GetString(411), _frmQQ->GetName(), "btnBlock" );
	pbtnAdd->evtMouseClick=_evtQQMainBlockPlayer;

	m_pQQTreeView = dynamic_cast<CTreeView*>(_frmQQ->Find("trvEditor"));
	if( !m_pQQTreeView ) return _Error(g_oLangRec.GetString(411), _frmQQ->GetName(), "trvEditor" );

	CLabelEx* pMineName=dynamic_cast<CLabelEx*>(_frmQQ->Find("labMineName"));
	if (!pMineName)  return _Error(g_oLangRec.GetString(411), _frmQQ->GetName(), "labMineName" );

	_frndMouseRight=CMenu::FindMenu("frndMouseRight");
	if (!_frndMouseRight)  return _Error(g_oLangRec.GetString(411), _frmQQ->GetName(), "frndMouseRight" );
	_frndMouseRight->evtListMouseDown=_OnMouseFrndMenu;

	_groupMouseRight=CMenu::FindMenu("groupMouseRight");
	if (!_groupMouseRight)  return _Error(g_oLangRec.GetString(411), _frmQQ->GetName(), "groupMouseRight" );
	_groupMouseRight->evtListMouseDown=_OnMouseGroupMenu;

	_roadMouseRight=CMenu::FindMenu("roadMouseRight");
	if (!_roadMouseRight)  return _Error(g_oLangRec.GetString(411), _frmQQ->GetName(), "roadMouseRight" );
	_roadMouseRight->evtListMouseDown=_OnMouseRoadMenu;

	_sessMouseRight=CMenu::FindMenu("ChatMouseRight");
	if (!_sessMouseRight)  return _Error(g_oLangRec.GetString(411), _frmQQ->GetName(), "ChatMouseRight" );
	_sessMouseRight->evtListMouseDown=_OnMouseSessMenu;

	_MasterMouseRight = CMenu::FindMenu("MasterRight");
	if(!_MasterMouseRight)  return _Error(g_oLangRec.GetString(411), _frmQQ->GetName(), "MasterRight" );
	_MasterMouseRight->evtListMouseDown = _OnMouseMasterMenu;

	_StudentMouseRight = CMenu::FindMenu("StudentRight");
	if(!_StudentMouseRight)  return _Error(g_oLangRec.GetString(411), _frmQQ->GetName(), "StudentRight" );
	_StudentMouseRight->evtListMouseDown = _OnMouseStudentMenu;

	_BlockedMouseRight = CMenu::FindMenu("BlockedRight");
	if(!_BlockedMouseRight)  return _Error(g_oLangRec.GetString(411), _frmQQ->GetName(), "BlockedRight" );
	_BlockedMouseRight->evtListMouseDown = _OnMouseBlockedMenu;

	m_pQQTreeView->evtMouseDown=_MainMouseClick;
	m_pQQTreeView->evtMouseDB=_MainMouseClick;
	m_pQQTreeView->evtMouseDragEnd = _OnDragEnd;

	m_pQQTreeView->SetSelectColor( 0 );
	int nWidth = m_pQQTreeView->GetWidth() - m_pQQTreeView->GetScroll()->GetWidth()-28;
	int nHeight = SMALL_ICON_SIZE+3;

	_pSessionNode = dynamic_cast<GUI::CTreeGridNode*>(m_pQQTreeView->GetRootNode()->FindNode( g_oLangRec.GetString(465) ));
	_pSessionNode->GetItem()->SetColor( COLOR_BLACK );
	_pSessionNode->GetUpImage()->UnLoadImage();
	_pSessionNode->GetDownImage()->UnLoadImage();
	_pSessionNode->SetUnitSize(nWidth, nHeight );
	_pSessionNode->SetColMaxNum(1);

	_pFrndNode = dynamic_cast<GUI::CTreeGridNode*>(m_pQQTreeView->GetRootNode()->FindNode( g_oLangRec.GetString(466) ));
	_pFrndNode->GetItem()->SetColor( COLOR_BLACK );
	_pFrndNode->GetUpImage()->UnLoadImage();
	_pFrndNode->GetDownImage()->UnLoadImage();
	_pFrndNode->SetUnitSize(nWidth, nHeight );
	_pFrndNode->SetColMaxNum(1);

	_pGroupNode = dynamic_cast<GUI::CTreeGridNode*>(m_pQQTreeView->GetRootNode()->FindNode( g_oLangRec.GetString(467) ));
	_pGroupNode->GetItem()->SetColor( COLOR_BLACK );
	_pGroupNode->GetUpImage()->UnLoadImage();
	_pGroupNode->GetDownImage()->UnLoadImage();
	_pGroupNode->SetUnitSize(nWidth, nHeight );
	_pGroupNode->SetColMaxNum(1);

	_pGuildNode = dynamic_cast<GUI::CTreeGridNode*>(m_pQQTreeView->GetRootNode()->FindNode( g_oLangRec.GetString(468) ));
	_pGuildNode->GetItem()->SetColor( COLOR_BLACK );
	_pGuildNode->GetUpImage()->UnLoadImage();
	_pGuildNode->GetDownImage()->UnLoadImage();
	_pGuildNode->SetUnitSize(nWidth, nHeight );
	_pGuildNode->SetColMaxNum(1);

	_pMasterNode = dynamic_cast<GUI::CTreeGridNode*>(m_pQQTreeView->GetRootNode()->FindNode( g_oLangRec.GetString(850) ));
	_pMasterNode->GetItem()->SetColor( COLOR_BLACK );
	_pMasterNode->GetUpImage()->UnLoadImage();
	_pMasterNode->GetDownImage()->UnLoadImage();
	_pMasterNode->SetUnitSize(nWidth, nHeight );
	_pMasterNode->SetColMaxNum(1);

	_pPrenticeNode = dynamic_cast<GUI::CTreeGridNode*>(m_pQQTreeView->GetRootNode()->FindNode( g_oLangRec.GetString(851) ));
	_pPrenticeNode->GetItem()->SetColor( COLOR_BLACK );
	_pPrenticeNode->GetUpImage()->UnLoadImage();
	_pPrenticeNode->GetDownImage()->UnLoadImage();
	_pPrenticeNode->SetUnitSize(nWidth, nHeight );
	_pPrenticeNode->SetColMaxNum(1);
	
	_pGMNode = dynamic_cast<GUI::CTreeGridNode*>(m_pQQTreeView->GetRootNode()->FindNode( "GM"));
	_pGMNode->GetItem()->SetColor( COLOR_BLACK );
	_pGMNode->GetUpImage()->UnLoadImage();
	_pGMNode->GetDownImage()->UnLoadImage();
	_pGMNode->SetUnitSize(nWidth, nHeight );
	_pGMNode->SetColMaxNum(1);

	_pBlockedNode = dynamic_cast<GUI::CTreeGridNode*>(m_pQQTreeView->GetRootNode()->FindNode( "Blocked" ));
	_pBlockedNode->GetItem()->SetColor( COLOR_BLACK );
	_pBlockedNode->GetUpImage()->UnLoadImage();
	_pBlockedNode->GetDownImage()->UnLoadImage();
	_pBlockedNode->SetUnitSize(nWidth, nHeight );
	_pBlockedNode->SetColMaxNum(1);

	_pTeamMgr->Find( enumTeamFrnd )->SetPointer( _pFrndNode );
	_pTeamMgr->Find( enumTeamGroup )->SetPointer( _pGroupNode );
	_pTeamMgr->Find( enumTeamMaster)->SetPointer( _pMasterNode );
	_pTeamMgr->Find( enumTeamPrentice )->SetPointer(_pPrenticeNode );
	_pTeamMgr->Find( enumTeamGM )->SetPointer(_pGMNode );
	_pTeamMgr->Find( enumTeamBlocked )->SetPointer( _pBlockedNode );

	_frmAddFriend = mgr.Find("frmAddFriend");
	if( !_frmAddFriend )
	{
		g_logManager.InternalLog(LogLevel::Error, "errors", g_oLangRec.GetString(470));
		return false;
	}
	_frmAddFriend->SetIsShow(true);
	pbtnAdd = dynamic_cast<CTextButton*>(_frmAddFriend->Find("btnYes"));
	if( !pbtnAdd ) return _Error(g_oLangRec.GetString(411), _frmAddFriend->GetName(), "btnYes" );
	pbtnAdd->evtMouseClick=_evtAddFrnd;
	_pEditFrndName = dynamic_cast<CEdit*>(_frmAddFriend->Find("edtTradeGold"));
	if( !_pEditFrndName ) return _Error(g_oLangRec.GetString(411), _frmAddFriend->GetName(), "edtTradeGold" );


	_styMenu = CMenu::FindMenu("styMenu");
	if( !_styMenu ) return _Error(g_oLangRec.GetString(411), _frmQQ->GetName(), "styMenu" );
	_styMenu->evtListMouseDown=_evtChangeStyle;

	_frmEditMotto = mgr.Find("frmEdit");
	if( !_frmEditMotto )
	{
		g_logManager.InternalLog(LogLevel::Error, "errors", g_oLangRec.GetString(471));
		return false;
	}
	_frmEditMotto->SetIsShow(true);
	pbtnAdd = dynamic_cast<CTextButton*>(_frmEditMotto->Find("btnYes"));
	if( !pbtnAdd ) return _Error(g_oLangRec.GetString(411), _frmEditMotto->GetName(), "btnYes" );
	pbtnAdd->evtMouseClick=_evtChangeMotto;

	_pEditMotto = dynamic_cast<CEdit*>(_frmEditMotto->Find("edtTradeGold"));
	if( !_pEditMotto) return _Error(g_oLangRec.GetString(411), _frmEditMotto->GetName(), "edtTradeGold" );
	_labNameOfMottoFrm=dynamic_cast<CLabelEx*>(_frmEditMotto->Find("labName"));
	if( !_labNameOfMottoFrm) return _Error(g_oLangRec.GetString(411), _frmEditMotto->GetName(), "labName" );
	_imgFaceOfMottoFrm=dynamic_cast<CImage*>(_frmEditMotto->Find("imgMhead"));
	if( !_imgFaceOfMottoFrm) return _Error(g_oLangRec.GetString(411), _frmEditMotto->GetName(), "imgMhead" );
	_chkForbidOfMottoFrm=dynamic_cast<CCheckBox*>(_frmEditMotto->Find("chkChat"));
	if( !_chkForbidOfMottoFrm) return _Error(g_oLangRec.GetString(411), _frmEditMotto->GetName(), "chkChat" );

	_frmDetails = mgr.Find("frmdetails");
	if( !_frmDetails )
	{
		g_logManager.InternalLog(LogLevel::Error, "errors", g_oLangRec.GetString(472));
		return false;
	}
	_frmDetails->SetIsShow(true);
	pbtnAdd = dynamic_cast<CTextButton*>(_frmDetails->Find("btnRefurbish"));
	if( !pbtnAdd ) return _Error(g_oLangRec.GetString(473), _frmDetails->GetName(), "btnRefurbish" );
	pbtnAdd->evtMouseClick=_evtRefreshInfo;

	_frmBlockPlayer = mgr.Find("frmBlockPlayer");
	if( !_frmBlockPlayer )
	{
		g_logManager.InternalLog(LogLevel::Error, "errors", g_oLangRec.GetString(470));
		return false;
	}
	_frmBlockPlayer->SetIsShow(true);
	pbtnAdd = dynamic_cast<CTextButton*>(_frmBlockPlayer->Find("btnYes"));
	if( !pbtnAdd ) return _Error(g_oLangRec.GetString(411), _frmBlockPlayer->GetName(), "btnYes" );
	pbtnAdd->evtMouseClick=_evtBlockPlayer;
	_pEditBlockName = dynamic_cast<CEdit*>(_frmBlockPlayer->Find("edtTradeGold"));
	if( !_pEditBlockName ) return _Error(g_oLangRec.GetString(411), _frmBlockPlayer->GetName(), "edtTradeGold" );

	return true;
}

void CChat::End()
{
    m_pQQTreeView = NULL;
    //delete _pTeamMgr;
	SAFE_DELETE(_pTeamMgr); // UI��������

    for( int i=0; i<MAX_MEMBER; i++ )
    {
        //delete _pCharacter[i];
        //_pCharacter[i] = 0;
		SAFE_DELETE(_pCharacter[i]); // UI��������
    }
}

void CChat::ClearTeam()
{
	for( int i=0; i<MAX_MEMBER; i++)
    {
		frmTeamMenber[i]->SetIsShow(false);
    }
}

void CChat::RefreshTeamData( CMember* pCurMember )
{
	if( !pCurMember || !pCurMember->GetPointer() ) return;

	CMemberData *pCurMemData = pCurMember->GetData();	
	DWORD i = pCurMemData->GetIndex();

    proTeamMenberHP[i]->SetRange( 0.0f, (float)pCurMemData->GetMaxHP() );
    proTeamMenberHP[i]->SetPosition( (float)pCurMemData->GetHP() );

	static char szBuf[32] = { 0 };
	sprintf( szBuf, "%d", pCurMemData->GetLV() );
	labLv[i]->SetCaption( szBuf );

	if (pCurMemData->GetFace()) _pCharacter[i]->UpdataFace(*pCurMemData->GetFace());
}

void CChat::RefreshTeam()
{
    CTeam* pTeam = _pTeamMgr->Find(enumTeamGroup);
	if( !dynamic_cast<CWorldScene*>(CGameApp::GetCurScene()) )
	{
		for( int i=0; i<MAX_MEMBER; i++)
		{	
			frmTeamMenber[i]->SetIsShow(false);
		}
		return;
	}

	if( CGameScene::GetMainCha() && CGameScene::GetMainCha()->getHumanID()==CTeamMgr::GetTeamLeaderID() )
	{
		// �����ѵ�ͷ����ʾ�ӳ���־
		g_stUIStart.SetIsLeader( true );
	}
	else
	{
		g_stUIStart.SetIsLeader( false );
	}

	int Num = pTeam->GetCount();
	for( int i=MAX_MEMBER-1; i>=0; i--)
	{	
		if ( i<Num )
		{
			CMember* pCurMember = pTeam->GetMember( i );
			if( pCurMember && pCurMember->GetPointer())
            {
			    CMemberData *pCurMemData = pCurMember->GetData();
				pCurMemData->SetIndex( i );

				labMenberName[i]->SetCaption( pCurMember->GetName() );

				RefreshTeamData( pCurMember );
			}

			frmTeamMenber[i]->nTag = pCurMember->GetID();

			imgLeader[i]->SetIsShow( pCurMember->GetID()==CTeamMgr::GetTeamLeaderID() );
			frmTeamMenber[i]->SetIsShow( true );
		}
		else
		{
			frmTeamMenber[i]->SetIsShow( false );
		}
	}
}

int CChat::TeamSend( DWORD dwMsg, void* pData, DWORD dwParam )
{
    CTreeView* pTree = GetTeamView();
    if( !pTree ) return -1;

    switch( dwMsg )
    {
    case enumSTM_ADD_GROUP:
        {
            CTeam* pTeam = (CTeam*)pData;
            
            CTreeGridNode* pNode = (CTreeGridNode*)pTeam->GetPointer();
            if( !pNode ) return -1;

            pNode->Clear();
            if( pNode->GetIsExpand() )
                pTree->Refresh();

            if( enumTeamGroup==dwParam )
                ClearTeam();
        }
        break;
    case enumSTM_DEL_GROUP:
        {
            CTeam* pTeam = (CTeam*)pData;
            CTreeGridNode* pNode = (CTreeGridNode*)pTeam->GetPointer();
            if( !pNode ) return -1;

            pNode->Clear();
            if( pNode->GetIsExpand() )
                pTree->Refresh();

            if( enumTeamGroup==dwParam )
                ClearTeam();
        }
        break;
    case enumSTM_ADD_MEMBER:
        {
            CMember* pMember = (CMember*)pData;
            CTreeGridNode* pNode = (CTreeGridNode*)pMember->GetTeam()->GetPointer();
            if( !pNode ) return -1;

			CTextGraph* pItem = new CTextGraph(2);
            pItem->SetString( pMember->GetShowName() );
			pItem->SetPointer(pMember);
            pNode->AddItem( pItem );
            pMember->SetPointer( pItem );
			CChatIconInfo *pIconInfo=GetChatIconInfo(pMember->GetIconID());
			if (pIconInfo)
			{
				CGuiPic* pPic=pItem->GetImage();
				string strPath="texture/ui/HEAD/";
				if (pMember->IsOnline())
				{
					pPic->LoadImage((strPath+pIconInfo->szSmall).c_str(),SMALL_ICON_SIZE,SMALL_ICON_SIZE,0,pIconInfo->nSmallX,pIconInfo->nSmallY);
					pPic->SetFrame( 0 );
					pItem->SetColor(0xfffc2f20);
				}
				else
				{
					pPic->LoadImage((strPath+pIconInfo->szSmallOff).c_str(),SMALL_ICON_SIZE,SMALL_ICON_SIZE,0,pIconInfo->nSmallX,pIconInfo->nSmallY);
					pPic->SetFrame( 0 );
					pItem->SetColor(0xff000000);
				}
			}
			string _strHint=pMember->GetName();
			string strMotto=pMember->GetMotto();
			if (!strMotto.empty())
			{
				_strHint+="("+strMotto+")";
			}
			pItem->SetHint(_strHint.c_str());

			SortOnlineFrnd();

			if( pNode->GetIsExpand() )
				pTree->Refresh();
            if( enumTeamGroup==dwParam )
                RefreshTeam();
        }
        break;
    case enumSTM_DEL_MEMBER:
        {
            CMember* pMember = (CMember*)pData;
			if (pMember==_curSelectMember)
			{
				_curSelectMember=NULL;
				_frmQQ->PopMenu(NULL);
			}
            CTreeGridNode* pNode = (CTreeGridNode*)pMember->GetTeam()->GetPointer();
            if( !pNode ) return -1;

			pNode->DelItem((CItemObj*)pMember->GetPointer());
			pMember->SetPointer(NULL);
            if( pNode->GetIsExpand() )
                pTree->Refresh();

            if( dwParam==enumTeamGroup )
                RefreshTeam();
        }
        break;
    case enumSTM_NODE_CHANGE:
        {
            CMember* pMember = (CMember*)pData;
            CTextGraph* pItem = (CTextGraph*)(pMember->GetPointer());
			CChatIconInfo *pIconInfo=GetChatIconInfo(pMember->GetIconID());
			if (pIconInfo && pItem)	// modify by Philip.Wu  2006-08-13  �޸�ȥ��·�˺�ĵ���
			{
				CGuiPic* pPic=pItem->GetImage();
				string strPath="texture/ui/HEAD/";
				if (pMember->IsOnline())
				{
					pPic->LoadImage((strPath+pIconInfo->szSmall).c_str(),SMALL_ICON_SIZE,SMALL_ICON_SIZE,0,pIconInfo->nSmallX,pIconInfo->nSmallY);
					pPic->SetFrame( 0 );
					pItem->SetColor(0xfffc2f20);
				}
				else
				{
					pPic->LoadImage((strPath+pIconInfo->szSmallOff).c_str(),SMALL_ICON_SIZE,SMALL_ICON_SIZE,0,pIconInfo->nSmallX,pIconInfo->nSmallY);
					pPic->SetFrame( 0 );
					pItem->SetColor(0xff000000);
				}
			}
			if( pItem )
            {
				pItem->SetString( pMember->GetShowName() );
				string _strHint=pMember->GetName();
				string strMotto=pMember->GetMotto();
				if (!strMotto.empty())
				{
					_strHint+="("+strMotto+")";
				}
				pItem->SetHint(_strHint.c_str());
			}
			
			SortOnlineFrnd();
			pTree->Refresh();

            if( dwParam==enumTeamGroup )
                RefreshTeam();
        }
        break;
    case enumSTM_NODE_DATA_CHANGE:
        {
            if( dwParam==enumTeamGroup )
                RefreshTeamData( (CMember*)pData );
        }
        break;
	case enumSTM_AFTER_DEL_MEMBER:
        {
            if( dwParam==enumTeamGroup )
				RefreshTeam();
        }
        break;
    }
    return 0;
}

// Add by lark.li 20080806 begin
void CChat::SortOnlineFrnd(CTreeGridNode* pNode)
{
	DWORD nCount=pNode->GetItemCount();
	DWORD i=0;
	DWORD j=0;
	CTextGraph *pItem1,*pItem2;
	CMember *pMember1,*pMember2;
	for (;i<nCount;i++)
	{
		pItem1=dynamic_cast<CTextGraph*>(pNode->GetItemByIndex(i));
		pMember1=(CMember*)(pItem1->GetPointer());
		if (pMember1->IsOnline())
		{
			continue;
		}
		if (j<=i)
		{
			j=i+1;
		}
		for (;j<nCount;j++)
		{
			pItem2=dynamic_cast<CTextGraph*>(pNode->GetItemByIndex(j));
			pMember2=(CMember*)(pItem2->GetPointer());
			if (pMember2->IsOnline())
			{
				//1��2��Ի�
				pMember1->SetPointer(pItem2);
				pMember2->SetPointer(pItem1);
				pItem1->SetPointer(pMember2);
				pItem2->SetPointer(pMember1);
				CChatIconInfo *pIconInfo1=GetChatIconInfo(pMember1->GetIconID());
				CChatIconInfo *pIconInfo2=GetChatIconInfo(pMember2->GetIconID());
				if (!pIconInfo1 || !pIconInfo2)
				{
					continue;
				}
				CGuiPic* pPic2=pItem2->GetImage();
				string strPath="texture/ui/HEAD/";
				pPic2->LoadImage((strPath+pIconInfo1->szSmallOff).c_str(),SMALL_ICON_SIZE,SMALL_ICON_SIZE,0,pIconInfo1->nSmallX,pIconInfo1->nSmallY);
				pPic2->SetFrame( 0 );
				pItem2->SetColor(0xff000000);
				CGuiPic* pPic1=pItem1->GetImage();
				pPic1->LoadImage((strPath+pIconInfo2->szSmall).c_str(),SMALL_ICON_SIZE,SMALL_ICON_SIZE,0,pIconInfo2->nSmallX,pIconInfo2->nSmallY);
				pPic1->SetFrame( 0 );
				pItem1->SetColor(0xfffc2f20);
				string _strHint1=pMember1->GetName();
				string strMotto1=pMember1->GetMotto();
				if (!strMotto1.empty())
				{
					_strHint1+="("+strMotto1+")";
				}
				pItem2->SetString(pMember1->GetShowName());
				pItem2->SetHint(_strHint1.c_str());
				string _strHint2=pMember2->GetName();
				string strMotto2=pMember2->GetMotto();
				if (!strMotto2.empty())
				{
					_strHint2+="("+strMotto2+")";
				}
				pItem1->SetString(pMember2->GetShowName());
				pItem1->SetHint(_strHint2.c_str());
				break;
			}
		}
	}
}
// End

void CChat::SortOnlineFrnd()
{
	DWORD nCount=_pFrndNode->GetItemCount();
	DWORD i=0;
	DWORD j=0;
	CTextGraph *pItem1,*pItem2;
	CMember *pMember1,*pMember2;
	for (;i<nCount;i++)
	{
		pItem1=dynamic_cast<CTextGraph*>(_pFrndNode->GetItemByIndex(i));
		pMember1=(CMember*)(pItem1->GetPointer());
		if (pMember1->IsOnline())
		{
			continue;
		}
		if (j<=i)
		{
			j=i+1;
		}
		for (;j<nCount;j++)
		{
			pItem2=dynamic_cast<CTextGraph*>(_pFrndNode->GetItemByIndex(j));
			pMember2=(CMember*)(pItem2->GetPointer());
			if (pMember2->IsOnline())
			{
				//1��2��Ի�
				pMember1->SetPointer(pItem2);
				pMember2->SetPointer(pItem1);
				pItem1->SetPointer(pMember2);
				pItem2->SetPointer(pMember1);
				CChatIconInfo *pIconInfo1=GetChatIconInfo(pMember1->GetIconID());
				CChatIconInfo *pIconInfo2=GetChatIconInfo(pMember2->GetIconID());
				if (!pIconInfo1 || !pIconInfo2)
				{
					continue;
				}
				CGuiPic* pPic2=pItem2->GetImage();
				string strPath="texture/ui/HEAD/";
				pPic2->LoadImage((strPath+pIconInfo1->szSmallOff).c_str(),SMALL_ICON_SIZE,SMALL_ICON_SIZE,0,pIconInfo1->nSmallX,pIconInfo1->nSmallY);
				pPic2->SetFrame( 0 );
				pItem2->SetColor(0xff000000);
				CGuiPic* pPic1=pItem1->GetImage();
				pPic1->LoadImage((strPath+pIconInfo2->szSmall).c_str(),SMALL_ICON_SIZE,SMALL_ICON_SIZE,0,pIconInfo2->nSmallX,pIconInfo2->nSmallY);
				pPic1->SetFrame( 0 );
				pItem1->SetColor(0xfffc2f20);
				string _strHint1=pMember1->GetName();
				string strMotto1=pMember1->GetMotto();
				if (!strMotto1.empty())
				{
					_strHint1+="("+strMotto1+")";
				}
				pItem2->SetString(pMember1->GetShowName());
				pItem2->SetHint(_strHint1.c_str());
				string _strHint2=pMember2->GetName();
				string strMotto2=pMember2->GetMotto();
				if (!strMotto2.empty())
				{
					_strHint2+="("+strMotto2+")";
				}
				pItem1->SetString(pMember2->GetShowName());
				pItem1->SetHint(_strHint2.c_str());
				break;
			}
		}
	}
}

void CChat::_RenderEvent(C3DCompent *pSender, int x, int y)
{
	_pCharacter[pSender->nTag]->Render();
}

void CChat::_MemberMouseDownEvent(CGuiData *pSender, int x, int y, DWORD key)
{	
	if( key & Mouse_LDown )
	{
		CWorldScene* pScene = dynamic_cast<CWorldScene*>(CGameApp::GetCurScene());
		if( !pScene ) return;

		CCharacter* pCha = pScene->SearchByHumanName( labMenberName[pSender->nTag]->GetCaption() );
		if( !pCha )
		{
			g_pGameApp->SysInfo( g_oLangRec.GetString(474), labMenberName[pSender->nTag]->GetCaption() );
			return;
		}

		if( g_pGameApp->IsAltPress() )
			pScene->GetMouseDown().GetAutoAttack()->Follow( CGameScene::GetMainCha(), pCha );
		else
			pScene->GetMouseDown().ActCha( CGameScene::GetMainCha(), pCha );
		return;
	}
	
	if( key & Mouse_RDown )
	{
		if( _groupMouseRight )
		{
			CTeam *pTeam = _pTeamMgr->Find( enumTeamGroup );
			CMember *pMember = pTeam->Find( frmTeamMenber[pSender->nTag]->nTag );
			if( pMember )
			{
				_curSelectMember = pMember;
				CMenuItem* pItem = _groupMouseRight->GetMenuItem(3);
				if (pItem && stricmp( pItem->GetString(), g_oLangRec.GetString(475) ) == 0)
				{
					if (g_stUIStart.GetIsLeader())
					{
						pItem->SetIsEnabled(true);
					}
					else
					{
						pItem->SetIsEnabled(false);
					}
				}
				pSender->GetForm()->PopMenu(_groupMouseRight,x,y);
				return;
			}
		}
		g_pGameApp->SysInfo( g_oLangRec.GetString(476), labMenberName[pSender->nTag]->GetCaption() );
	}
}

void CChat::_MainMouseClick(CGuiData *pSender, int x, int y, DWORD key)
{
	CTreeView* pTree = dynamic_cast<CTreeView*>(pSender);
	if( !pTree ) return;

	CTextGraph *pSelectItem=dynamic_cast<CTextGraph*>(pTree->GetHitItem(x,y));
	if (!pSelectItem)
		return;

	CTreeGridNode *pSelectNode=dynamic_cast<CTreeGridNode*>(pTree->GetSelectNode());
	if (!pSelectNode)
		return;
		
	int sx=x;
	int sy=y;

	if (pSelectNode==(_pTeamMgr->Find( enumTeamFrnd ))->GetPointer())
	{
		CTeam *pTeam=_pTeamMgr->Find( enumTeamFrnd );
		for (DWORD i=0;i<pTeam->GetCount();i++)
		{
			CMember *pMember=pTeam->GetMember(i);
			if (pMember->GetPointer()==pSelectItem)
			{
				_curSelectMember=pMember;
				if (key & Mouse_LDB)
				{
					CTalkSessionFormMgr::ApplySession( &_curSelectMember );
				}
				else if ( key & Mouse_RDown )
				{
					if (x+_frndMouseRight->GetWidth()>g_pGameApp->GetWindowWidth())
						sx=g_pGameApp->GetWindowWidth()-_frndMouseRight->GetWidth();
					if (y+_frndMouseRight->GetHeight()>g_pGameApp->GetWindowHeight())
						sy=g_pGameApp->GetWindowHeight()-_frndMouseRight->GetHeight();
					_frmQQ->PopMenu(_frndMouseRight,sx,sy);
				}
				break;
			}
		}
	}
	else if (pSelectNode==_pTeamMgr->Find( enumTeamGroup )->GetPointer())
	{
		CTeam *pTeam=_pTeamMgr->Find( enumTeamGroup );
		for (DWORD i=0;i<pTeam->GetCount();i++)
		{
			CMember *pMember=pTeam->GetMember(i);
			if (pMember->GetPointer()==pSelectItem)
			{
				_curSelectMember=pMember;
				if (key & Mouse_LDB)
				{
					CTalkSessionFormMgr::ApplySession(&_curSelectMember);
				}
				else if (key & Mouse_RDown)
				{
					if (x+_groupMouseRight->GetWidth()>g_pGameApp->GetWindowWidth())
						sx=g_pGameApp->GetWindowWidth()-_groupMouseRight->GetWidth();
					if (y+_groupMouseRight->GetHeight()>g_pGameApp->GetWindowHeight())
						sy=g_pGameApp->GetWindowHeight()-_groupMouseRight->GetHeight();
					_frmQQ->PopMenu(_groupMouseRight,sx,sy);
				}
				break;
			}
		}
	}
	else if (pSelectNode==_pTeamMgr->Find( enumTeamRoad )->GetPointer())
	{
		CTeam *pTeam=_pTeamMgr->Find( enumTeamRoad );
		for (DWORD i=0;i<pTeam->GetCount();i++)
		{
			CMember *pMember=pTeam->GetMember(i);
			if (pMember->GetPointer()==pSelectItem)
			{
				_curSelectMember=pMember;
				if (key & Mouse_LDB)
				{
					CTalkSessionFormMgr::ApplySession(&_curSelectMember);
				}
				else if (key & Mouse_RDown)
				{
					if (x+_roadMouseRight->GetWidth()>g_pGameApp->GetWindowWidth())
						sx=g_pGameApp->GetWindowWidth()-_roadMouseRight->GetWidth();
					if (y+_roadMouseRight->GetHeight()>g_pGameApp->GetWindowHeight())
						sy=g_pGameApp->GetWindowHeight()-_roadMouseRight->GetHeight();
					_frmQQ->PopMenu(_roadMouseRight,sx,sy);
				}
				break;
			}
		}
	}
	else if (pSelectNode == _pTeamMgr->Find( enumTeamMaster )->GetPointer())	// ��ʦ
	{
		CTeam *pTeam=_pTeamMgr->Find( enumTeamMaster );
		for (DWORD i=0;i<pTeam->GetCount();i++)
		{
			CMember *pMember=pTeam->GetMember(i);
			if (pMember->GetPointer()==pSelectItem)
			{
				_curSelectMember=pMember;
				if (key & Mouse_LDB)
				{
					CTalkSessionFormMgr::ApplySession(&_curSelectMember);
				}
				else if (key & Mouse_RDown)
				{
					if (x+_roadMouseRight->GetWidth()>g_pGameApp->GetWindowWidth())
						sx=g_pGameApp->GetWindowWidth()-_roadMouseRight->GetWidth();
					if (y+_roadMouseRight->GetHeight()>g_pGameApp->GetWindowHeight())
						sy=g_pGameApp->GetWindowHeight()-_roadMouseRight->GetHeight();
					_frmQQ->PopMenu(_MasterMouseRight,sx,sy);
				}
				break;
			}
		}
	}
	else if (pSelectNode == _pTeamMgr->Find( enumTeamPrentice )->GetPointer())		// ѧͽ
	{
		CTeam *pTeam=_pTeamMgr->Find( enumTeamPrentice );
		for (DWORD i=0;i<pTeam->GetCount();i++)
		{
			CMember *pMember=pTeam->GetMember(i);
			if (pMember->GetPointer()==pSelectItem)
			{
				_curSelectMember=pMember;
				if (key & Mouse_LDB)
				{
					CTalkSessionFormMgr::ApplySession(&_curSelectMember);
				}
				else if (key & Mouse_RDown)
				{
					if (x+_roadMouseRight->GetWidth()>g_pGameApp->GetWindowWidth())
						sx=g_pGameApp->GetWindowWidth()-_roadMouseRight->GetWidth();
					if (y+_roadMouseRight->GetHeight()>g_pGameApp->GetWindowHeight())
						sy=g_pGameApp->GetWindowHeight()-_roadMouseRight->GetHeight();
					_frmQQ->PopMenu(_StudentMouseRight,sx,sy);
				}
				break;
			}
		}
	}
	else if (pSelectNode==_pSessionNode)
	{
		if (key & Mouse_LDB)
		{
			CTalkSessionFormMgr::OnClickOpenSession(pSelectItem);
		}
		else if (key & Mouse_RDown)
		{
			CTalkSessionForm* sessForm=static_cast<CTalkSessionForm*>(pSelectItem->GetPointer());
			if (sessForm)
			{
				_sessMouseRight->SetPointer(sessForm);
				if (x+_sessMouseRight->GetWidth()>g_pGameApp->GetWindowWidth())
					sx=g_pGameApp->GetWindowWidth()-_sessMouseRight->GetWidth();
				if (y+_sessMouseRight->GetHeight()>g_pGameApp->GetWindowHeight())
					sy=g_pGameApp->GetWindowHeight()-_sessMouseRight->GetHeight();
				_frmQQ->PopMenu(_sessMouseRight,sx,sy);
			}
		}
	}
	else if (pSelectNode==_pGuildNode)
	{
		int i=0;
		CGuildMemberData* pMemberData=NULL;
		while(pMemberData=CGuildMembersMgr::FindGuildMemberByIndex(i))
		{
			if (pMemberData->GetPointer()==pSelectItem)
			{
				if (key & Mouse_LDB)
				{
					CTalkSessionFormMgr::sApplyMember *pMember=new CTalkSessionFormMgr::sApplyMember[1];
					pMember[0].name=pMemberData->GetName();
					pMember[0].numbers=1;
					CTalkSessionFormMgr::ApplySession(pMember);
				}
				else if (key & Mouse_RDown)
				{
				}
			}
			i++;
		}
	}
	else if (pSelectNode==(_pTeamMgr->Find( enumTeamGM ))->GetPointer())
	{
		CTeam *pTeam=_pTeamMgr->Find( enumTeamGM );
		for (DWORD i=0;i<pTeam->GetCount();i++)
		{
			CMember *pMember=pTeam->GetMember(i);
			if (pMember->GetPointer()==pSelectItem)
			{
				_curSelectMember=pMember;
				CTalkSessionFormMgr::ApplySession( &_curSelectMember );
				break;
			}
		}
	}
	else if (pSelectNode==_pTeamMgr->Find( enumTeamBlocked )->GetPointer())
	{
		CTeam *pTeam=_pTeamMgr->Find(enumTeamBlocked);
		for (DWORD i=0;i<pTeam->GetCount();i++)
		{
			CMember *pMember=pTeam->GetMember(i);
			if (pMember->GetPointer()==pSelectItem)
			{
				_curSelectMember=pMember;
				if (key & Mouse_RDown)
				{
					if (x+_roadMouseRight->GetWidth()>g_pGameApp->GetWindowWidth())
						sx=g_pGameApp->GetWindowWidth()-_roadMouseRight->GetWidth();
					if (y+_roadMouseRight->GetHeight()>g_pGameApp->GetWindowHeight())
						sx=g_pGameApp->GetWindowHeight()-_roadMouseRight->GetHeight();
					_frmQQ->PopMenu(_BlockedMouseRight,sx,sy);
				}
				break;
			}
		}
	}
	else
	{
		g_pGameApp->SysInfo("Unknow Hit! File: UIChat.cpp   Class: CChat   Code: Arcol");
	}
}

void CChat::_evtQQMainShow(CGuiData *pSender)
{
	CCharacter* pMain = CGameScene::GetMainCha();
	if( pMain )
	{
		CLabelEx* pMineName=dynamic_cast<CLabelEx*>(CFormMgr::s_Mgr.Find("frmQQ")->Find("labMineName"));
		if (pMineName)
		{
			pMineName->SetCaption(pMain->getHumanName());
		}
		else
		{
			_Error(g_oLangRec.GetString(473), _frmQQ->GetName(), "labMineName" );
		}
	}
}

void CChat::_OnMouseFrndMenu(CGuiData *pSender, int x, int y, DWORD key)
{
	_frndMouseRight->SetIsShow(false);
	if (!_curSelectMember) return;
	CMenuItem* pItem=_frndMouseRight->GetSelectMenu();
	if (!pItem) return;
	string str=pItem->GetString();
	if (str==g_oLangRec.GetString(477))
	{
		_UpdateFrndInfo(_curSelectMember);
		CP_Frnd_Refresh_Info(_curSelectMember->GetID());
		_frmDetails->Show();
	}
	else if (str==g_oLangRec.GetString(478))
	{
	 	CTalkSessionFormMgr::ApplySession(&_curSelectMember);
	}
	else if (str==g_oLangRec.GetString(479))
	{
		string str=g_oLangRec.GetString(480);
		str+=_curSelectMember->GetName();
		stSelectBox *pSelectBox=g_stUIBox.ShowSelectBox(_OnFrndDeleteConfirm,str.c_str(),true);
		pSelectBox->dwTag=_curSelectMember->GetID();
	}
	else if (str==g_oLangRec.GetString(481))
	{
		CCozeForm::GetInstance()->OnPrivateNameSet(_curSelectMember->GetName());
	}
	_frmQQ->Refresh();
}

void CChat::_OnMouseGroupMenu(CGuiData *pSender, int x, int y, DWORD key)
{
	_groupMouseRight->SetIsShow(false);
	if (!_curSelectMember) return;
	CMenuItem* pItem=_groupMouseRight->GetSelectMenu();
	if (!pItem) return;
	string str=pItem->GetString();
	if (str==g_oLangRec.GetString(482))
	{
		CS_Frnd_Invite(_curSelectMember->GetName());
	}
	else if (str==g_oLangRec.GetString(478))
	{
		CTalkSessionFormMgr::ApplySession(&_curSelectMember);
	}
	else if (str==g_oLangRec.GetString(483))
	{
		CS_Team_Leave();
	}
	else if (str==g_oLangRec.GetString(481))
	{
		CCozeForm::GetInstance()->OnPrivateNameSet(_curSelectMember->GetName());
	}
	else if (str==g_oLangRec.GetString(475))
	{
		CS_Team_Kick( _curSelectMember->GetID() );
	}
	else if (str == "Follow")
	{
		CWorldScene* pScene = dynamic_cast<CWorldScene*>(CGameApp::GetCurScene());
		CCharacter* pCha = pScene->SearchByHumanName( labMenberName[pSender->nTag]->GetCaption() );
		if(pCha){
		pScene->GetMouseDown().GetAutoAttack()->Follow( CGameScene::GetMainCha(), pCha );
		}
	}
	_frmQQ->Refresh();

}

void CChat::_OnMouseRoadMenu(CGuiData *pSender, int x, int y, DWORD key)
{
	_roadMouseRight->SetIsShow(false);
	if (!_curSelectMember) return;

	CMenuItem* pItem=_roadMouseRight->GetSelectMenu();
	if (!pItem) return;
	string str=pItem->GetString();
	if (str==g_oLangRec.GetString(482))
	{
		CS_Frnd_Invite(_curSelectMember->GetName());
	}
	else if (str==g_oLangRec.GetString(484))
	{
		CS_Team_Invite(_curSelectMember->GetName());
	}
	else if (str==g_oLangRec.GetString(478))
	{
		CTalkSessionFormMgr::ApplySession(&_curSelectMember);
	}
	else if (str==g_oLangRec.GetString(481))
	{
		CCozeForm::GetInstance()->OnPrivateNameSet(_curSelectMember->GetName());
	}
	_frmQQ->Refresh();
}

void CChat::_OnMouseSessMenu(CGuiData *pSender, int x, int y, DWORD key)
{
	_frmQQ->PopMenu(NULL);
	CMenuItem* pItem=_sessMouseRight->GetSelectMenu();
	if (!pItem) return;
	string str=pItem->GetString();
	if (str== g_oLangRec.GetString(485))	// ����Է�
	{
		CTalkSessionFormMgr::OnClickCloseSession( static_cast<CTalkSessionForm*>(_sessMouseRight->GetPointer()) );
	}

	_frmQQ->Refresh();
}

// ��ʦ�˵�
void CChat::_OnMouseMasterMenu(CGuiData *pSender, int x, int y, DWORD key)
{
	_frmQQ->PopMenu(FALSE);

	CMenuItem* pItem = _MasterMouseRight->GetSelectMenu();
	if(! pItem) return;

	string strCommand = pItem->GetString();

	if(strCommand == g_oLangRec.GetString(482))			// ���Ӻ���
	{
		CCharacter* pMainCha = CGameScene::GetMainCha();
		if(pMainCha && pMainCha->getGameAttr() && pMainCha->getGameAttr()->get(ATTR_LV) >= 7)
		{
			CS_Frnd_Invite(_curSelectMember->GetName());
		}
		else
		{
			// �߼����½�ֹ���Ӻ���
			g_pGameApp->SysInfo(g_oLangRec.GetString(865));
		}
	}
	else if(strCommand == g_oLangRec.GetString(484))	// �������
	{
		CCharacter* pMainCha = CGameScene::GetMainCha();
		if(pMainCha && (pMainCha->IsBoat() || (pMainCha->getGameAttr() && pMainCha->getGameAttr()->get(ATTR_LV) >= 8)))
		{
			CS_Team_Invite(_curSelectMember->GetName());
		}
		else
		{
			// �˼����½�ֹ���
			g_pGameApp->SysInfo(g_oLangRec.GetString(866));
		}
	}
	else if(strCommand == g_oLangRec.GetString(481))	// ����Է�
	{
		CCozeForm::GetInstance()->OnPrivateNameSet(_curSelectMember->GetName());
	}
	else if(strCommand == g_oLangRec.GetString(478))	// ������Ϣ
	{
		CTalkSessionFormMgr::ApplySession(&_curSelectMember);
	}
	else if(strCommand == g_oLangRec.GetString(854))	// �����ϵ��ͽ�ܽ��ʦ����
	{
		CCharacter* pMain = CGameScene::GetMainCha();
		if( pMain && pMain->getGameAttr() )
		{
			long nLevel = pMain->getGameAttr()->get(ATTR_LV);

			char szBuffer[256] = {0};
			sprintf(szBuffer, g_oLangRec.GetString(889), _curSelectMember->GetName(), nLevel * 500); // "���ʦ�� %s �����۳�\n��Ǯ:%d\nȷ����"

			stSelectBox* pSelectBox = g_stUIBox.ShowSelectBox(_OnMasterDeleteConfirm, szBuffer, true);
			pSelectBox->dwTag = _curSelectMember->GetID();
			pSelectBox->pointer = (void*)_curSelectMember->GetName();
		}
	}

	_frmQQ->Refresh();
}

// ѧͽ�˵�
void CChat::_OnMouseStudentMenu(CGuiData *pSender, int x, int y, DWORD key)
{
	_frmQQ->PopMenu(FALSE);

	CMenuItem* pItem = _StudentMouseRight->GetSelectMenu();
	if(! pItem) return;

	string strCommand = pItem->GetString();

	if(strCommand == g_oLangRec.GetString(482))			// ���Ӻ���
	{
		CS_Frnd_Invite(_curSelectMember->GetName());
	}
	else if(strCommand == g_oLangRec.GetString(484))	// �������
	{
		if(_curSelectMember->GetLevel() >= 8)
		{
			CS_Team_Invite(_curSelectMember->GetName());
		}
		else
		{
			// �˼����½�ֹ���
			g_pGameApp->SysInfo(g_oLangRec.GetString(866));
		}
	}
	else if(strCommand == g_oLangRec.GetString(481))	// ����Է�
	{
		CCozeForm::GetInstance()->OnPrivateNameSet(_curSelectMember->GetName());
	}
	else if(strCommand == g_oLangRec.GetString(478))	// ������Ϣ
	{
		CTalkSessionFormMgr::ApplySession(&_curSelectMember);
	}
	else if(strCommand == g_oLangRec.GetString(854))	// �����ϵ��ʦ�����ͽ�ܣ�
	{
		CCharacter* pMain = CGameScene::GetMainCha();
		if( pMain && pMain->getGameAttr() )
		{
			long nLevel = pMain->getGameAttr()->get(ATTR_LV);

			char szBuffer[256] = {0};
			sprintf(szBuffer, g_oLangRec.GetString(856), _curSelectMember->GetName(),nLevel * 5); // ���ͽ�� %s �����۳�\n��Ǯ:%d  ����:%d\nȷ����

			stSelectBox* pSelectBox = g_stUIBox.ShowSelectBox(_OnStudentDeleteConfirm, szBuffer, true);
			pSelectBox->dwTag = _curSelectMember->GetID();
			pSelectBox->pointer = (void*)_curSelectMember->GetName();
		}
	}

	_frmQQ->Refresh();
}

void CChat::_OnMouseBlockedMenu(CGuiData *pSender, int x, int y, DWORD key)
{
	_frmQQ->PopMenu(FALSE);

	CMenuItem* pItem = _BlockedMouseRight->GetSelectMenu();
	if(! pItem) return;

	string strCommand = pItem->GetString();

	if(strCommand == "Delete")
	{
		_pTeamMgr->Find(enumTeamBlocked)->DelByName(_curSelectMember->GetName());
	}

	_frmQQ->Refresh();
}

void CChat::_evtQQMainAddFrnd(CGuiData *pSender, int x, int y, DWORD key)
{
	_frmAddFriend->Refresh();
	_frmAddFriend->Show();
}

void CChat::_evtAddFrnd(CGuiData *pSender, int x, int y, DWORD key)
{
	string name=_pEditFrndName->GetCaption();
	if (!name.empty())
	{
		if ( IsValidName(name.c_str(), (unsigned short)name.length() )
			)
		{
			_frmAddFriend->Close();
			CS_Frnd_Invite(name.c_str());
		}
		else
		{
			g_pGameApp->MsgBox( g_oLangRec.GetString(51) );
		}
	}
	else
	{
		_frmAddFriend->Close();
	}
}

void CChat::_evtQQMainChangeStyle(CGuiData *pSender, int x, int y, DWORD key)
{
	int sx=x;
	int sy=y;
	if (x+_styMenu->GetWidth()>g_pGameApp->GetWindowWidth())
		sx=g_pGameApp->GetWindowWidth()-_styMenu->GetWidth();
	if (y+_styMenu->GetHeight()>g_pGameApp->GetWindowHeight())
		sy=g_pGameApp->GetWindowHeight()-_styMenu->GetHeight();
	_frmQQ->PopMenu(_styMenu,sx,sy);
}

void CChat::_evtChangeStyle(CGuiData *pSender, int x, int y, DWORD key)
{
	_frmQQ->PopMenu(NULL);

	CMenuItem* pItem=_styMenu->GetSelectMenu();
	if (!pItem) return;
	string str=pItem->GetString();
	if (str==g_oLangRec.GetString(486))
	{
		_pTeamMgr->ChangeStyle(enumShowQQName);
	}
	else if (str==g_oLangRec.GetString(487))
	{
		_pTeamMgr->ChangeStyle(enumShowQQMotto);
	}
	_frmQQ->Refresh();
}

void CChat::_evtQQMainEditMotto(CGuiData *pSender, int x, int y, DWORD key)
{
	if (!g_pGameApp->GetCurScene()) return;
	if (!g_pGameApp->GetCurScene()->GetMainCha()) return;

	_pEditMotto->SetCaption(_strSelfMottoName.c_str());
	_labNameOfMottoFrm->SetCaption(g_pGameApp->GetCurScene()->GetMainCha()->getHumanName());

	CChatIconInfo* pIconInfo = GetChatIconInfo(_dwSelfIcon);
	if (pIconInfo)
	{
		CGuiPic* pPic = _imgFaceOfMottoFrm->GetImage();
		std::string strPath = "texture/ui/HEAD/";
		pPic->LoadImage((strPath+pIconInfo->szBig).c_str(), BIG_ICON_SIZE, BIG_ICON_SIZE, 0, pIconInfo->nBigX, pIconInfo->nBigY);
	}

	_chkForbidOfMottoFrm->SetIsChecked(_bForbid);
	_frmEditMotto->Refresh();
	_frmEditMotto->Show();
}

void CChat::_evtChangeMotto(CGuiData *pSender, int x, int y, DWORD key)
{
	std::string name = _pEditMotto->GetCaption();

	if (!CTextFilter::IsLegalText(CTextFilter::NAME_TABLE, name) || !IsValidName(name.c_str(), name.length()))
	{
		g_pGameApp->MsgBox(g_oLangRec.GetString(51));
		return;
	}

	CP_Change_PersonInfo(name.c_str(), _dwSelfIcon, _chkForbidOfMottoFrm->GetIsChecked());
	_frmEditMotto->Close();
}

void CChat::_evtRefreshInfo(CGuiData *pSender, int x, int y, DWORD key)
{
	CP_Frnd_Refresh_Info(_frmDetails->nTag);
}

void CChat::_evtQQMainBlockPlayer(CGuiData *pSender, int x, int y, DWORD key)
{
	_pEditBlockName->SetCaption("");
	_frmBlockPlayer->Refresh();
	_frmBlockPlayer->Show();
}

void CChat::_evtBlockPlayer(CGuiData *pSender, int x, int y, DWORD key)
{
	string name=_pEditBlockName->GetCaption();
	if (!name.empty())
	{
		_frmBlockPlayer->Close();

		CTeam* pTeam = g_stUIChat.GetTeamMgr()->Find(enumTeamBlocked);
		if(!pTeam->FindByName(name.c_str()))
			pTeam->Add(-1, name.c_str(), "", 9);
	}
	else
	{
		_frmBlockPlayer->Close();
	}
}

bool CChat::_UpdateFrndInfo(CMember *pMember)
{
	if (!pMember) return false;
	
	CLabelEx* pDeName = dynamic_cast<CLabelEx*>(_frmDetails->Find("labDeName"));
	if (pDeName)
	{
		pDeName->SetCaption(pMember->GetName());
	}
	else
	{
		_Error(g_oLangRec.GetString(473), _frmDetails->GetName(), "labDeName");
	}

	CLabelEx* pJob = dynamic_cast<CLabelEx*>(_frmDetails->Find("labJob"));
	if (pJob)
	{
		pJob->SetCaption(pMember->GetJobName());
	}
	else
	{
		_Error(g_oLangRec.GetString(473), _frmDetails->GetName(), "labJob");
	}

	CLabelEx* pLv = dynamic_cast<CLabelEx*>(_frmDetails->Find("labLv"));
	if (pLv)
	{
		char buf[32] = {0};
		sprintf(buf, "%d", pMember->GetLevel());
		pLv->SetCaption(buf);
	}
	else
	{
		_Error(g_oLangRec.GetString(473), _frmDetails->GetName(), "labLv");
	}

	CLabelEx* pConsortia = dynamic_cast<CLabelEx*>(_frmDetails->Find("labConsortia"));
	if (pConsortia)
	{
		pConsortia->SetCaption(pMember->GetGuildName());
	}

	CLabelEx* pName = dynamic_cast<CLabelEx*>(_frmDetails->Find("labName"));
	if (!pName)
	{
		_Error(g_oLangRec.GetString(473), _frmDetails->GetName(), "labName");
	}

	CImage* pDeHead = dynamic_cast<CImage*>(_frmDetails->Find("imgDeHead"));
	if (pDeHead)
	{
		CChatIconInfo* pIconInfo = GetChatIconInfo(pMember->GetIconID());
		if (pIconInfo)
		{
			CGuiPic* pPic = _imgFaceOfMottoFrm->GetImage();
			std::string strPath = "texture/ui/HEAD/";
			pPic->LoadImage((strPath+pIconInfo->szBig).c_str(), BIG_ICON_SIZE, BIG_ICON_SIZE, 0, pIconInfo->nBigX, pIconInfo->nBigY);
		}
	}
	else
	{
		_Error(g_oLangRec.GetString(473), _frmDetails->GetName(), "imgDeHead");
	}

	_frmDetails->nTag = pMember->GetID();
	_frmDetails->Refresh();

	return true;
}

bool CChat::_UpdateSelfInfo()
{
	CForm* frmQQ = CFormMgr::s_Mgr.Find("frmQQ", 1);
	CLabelEx* pMineName = dynamic_cast<CLabelEx*>(frmQQ->Find("labMineName"));
	if (pMineName)
	{
		pMineName->SetHint(_strSelfMottoName.c_str());
	}
	else
	{
		_Error(g_oLangRec.GetString(473), _frmQQ->GetName(), "labMineName");
	}

	CImage* pImage = dynamic_cast<CImage*>(_frmQQ->Find("imgHead"));
	if (pImage)
	{
		CChatIconInfo* pIconInfo = GetChatIconInfo(_dwSelfIcon);
		if (pIconInfo)
		{
			CGuiPic* pPic = pImage->GetImage();
			std::string strPath = "texture/ui/HEAD/";
			pPic->LoadImage((strPath+pIconInfo->szBig).c_str(), BIG_ICON_SIZE, BIG_ICON_SIZE, 0, pIconInfo->nBigX, pIconInfo->nBigY);
		}
	}
	else
	{
		_Error(g_oLangRec.GetString(473), _frmQQ->GetName(), "imgHead");
	}

	return true;

}

void CChat::_OnTimerFlash(CGuiTime *pSender)
{
	CTextButton *pBtn=CStartMgr::GetShowQQButton();
	if (!pBtn) return;
	if (CTalkSessionFormMgr::hasFlashSession())
	{
		if (!_frmQQ->GetIsShow())
		{
			static int i=0;
			pBtn->GetImage()->SetFrame(i);
			i=(i==0)?3:0;
		}
		else
		{
			pBtn->GetImage()->SetFrame(0);
		}
		if (!_pSessionNode->GetIsExpand())
		{
			static bool bColor=false;
			_pSessionNode->GetItem()->SetColor((bColor)?0xff00ff00:COLOR_BLACK);
			bColor=!bColor;
		}
		else
		{
			_pSessionNode->GetItem()->SetColor(COLOR_BLACK);
		}
	}
	else
	{
		pBtn->GetImage()->SetFrame(0);
		_pSessionNode->GetItem()->SetColor(COLOR_BLACK);
	}
	CTalkSessionFormMgr::OnFlashSession();
}

void CChat::_OnDragEnd(CForm* pTargetForm, CTreeView *pTree, CTreeNodeObj* pNode, CItemObj* pItem, int x, int y, DWORD key  )
{
	CTalkSessionForm* pSessForm;
	pSessForm=CTalkSessionFormMgr::GetSessionFormByForm(pTargetForm);
	if (!pSessForm) return;
	string strCaption=pNode->GetCaption();
	if (strCaption==g_oLangRec.GetString(465)) return;
	CTextGraph* pText=dynamic_cast<CTextGraph*>(pItem);
	if (pText)
	{
		CMember* pMember=static_cast<CMember*>(pText->GetPointer());
		if (!pMember) return;
		if (pSessForm->IsActiveSession())
		{
			CS_Sess_Add(pSessForm->GetSessionID(),pMember->GetName());
		}
		else
		{
			if (string(pMember->GetName())==pSessForm->GetMemberByIndex(0)->GetName())
			{
				g_pGameApp->SysInfo(g_oLangRec.GetString(488));
				return;
			}
			pSessForm->AddMemberToBuffer(pMember->GetName());
			const char **pArrName=new const char *[1];
			pArrName[0]=pSessForm->GetMemberByIndex(0)->GetName();
			CS_Sess_Create(pArrName,1);
			SAFE_DELETE_ARRAY(pArrName);
		}
	}
	else
	{
		CTreeGridNode* pGridNode = dynamic_cast<CTreeGridNode*>(pNode);
		if (!pGridNode) return;
		int nCount=0;
		for (DWORD i=0;i<pGridNode->GetItemCount();i++)
		{
			pText=dynamic_cast<CTextGraph*>(pGridNode->GetItemByIndex(i));
			if (!pText) continue;
			CMember* pMember=static_cast<CMember*>(pText->GetPointer());
			if (!pMember) continue;
			string strName=pMember->GetName();
			if (pSessForm->hasMember(strName)) continue;
			nCount++;
			if (pSessForm->IsActiveSession())
			{
				CS_Sess_Add(pSessForm->GetSessionID(),strName.c_str());
			}
			else
			{
				pSessForm->AddMemberToBuffer(strName);
			}
		}
		if (nCount>0 && !pSessForm->IsActiveSession())
		{
			const char **pArrName=new const char *[1];
			pArrName[0]=pSessForm->GetMemberByIndex(0)->GetName();
			CS_Sess_Create(pArrName,1);
			SAFE_DELETE_ARRAY(pArrName);
		}
	}
}

void CChat::_OnFrndDeleteConfirm(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
{
	if( nMsgType!=CForm::mrYes ) return;

	stSelectBox* pSelect=static_cast<stSelectBox*>(pSender->GetForm()->GetPointer());
	if (pSelect)
	{
		CS_Frnd_Delete(pSelect->dwTag);
	}
}


void CChat::_OnMasterDeleteConfirm(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
{
	if( nMsgType != CForm::mrYes ) return;

	stSelectBox* pSelect=static_cast<stSelectBox*>(pSender->GetForm()->GetPointer());
	if (pSelect && pSelect->pointer)
	{
		CS_MasterDel((const char*)pSelect->pointer, pSelect->dwTag);
	}
}

void CChat::_OnStudentDeleteConfirm(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
{
	if( nMsgType != CForm::mrYes ) return;

	stSelectBox* pSelect = static_cast<stSelectBox*>(pSender->GetForm()->GetPointer());
	if (pSelect && pSelect->pointer)
	{
		CS_PrenticeDel((const char*)pSelect->pointer, pSelect->dwTag);
	}
}


void CChat::MasterAsk(const char* szName, DWORD dwCharID)
{
	char szBuffer[512] = {0};
	sprintf(szBuffer, g_oLangRec.GetString(860), szName);

	stSelectBox *pSelectBox = g_stUIBox.ShowSelectBox(_OnStudentAskConfirm, szBuffer, true);
	pSelectBox->pointer = (void*)szName;
	pSelectBox->dwTag	= dwCharID;
}

void CChat::_OnStudentAskConfirm(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
{
	stSelectBox* pSelect = static_cast<stSelectBox*>(pSender->GetForm()->GetPointer());
	if (pSelect && pSelect->pointer)
	{
		CS_MasterAsr(nMsgType == CForm::mrYes, (const char*)pSelect->pointer, pSelect->dwTag);
	}
}


void CChat::PrenticeAsk(const char* szName, DWORD dwCharID)
{
	char szBuffer[512] = {0};
	sprintf(szBuffer, g_oLangRec.GetString(861), szName);

	stSelectBox *pSelectBox = g_stUIBox.ShowSelectBox(_OnMasterAskConfirm, szBuffer, true);
	pSelectBox->pointer = (void*)szName;
	pSelectBox->dwTag	= dwCharID;
}

void CChat::_OnMasterAskConfirm(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
{
	stSelectBox* pSelect = static_cast<stSelectBox*>(pSender->GetForm()->GetPointer());
	if (pSelect && pSelect->pointer)
	{
		CS_PrenticeAsr(nMsgType == CForm::mrYes, (const char*)pSelect->pointer, pSelect->dwTag);
	}
}
