#include "StdAfx.h"

#include "UIBoothForm.h"

#include <vector>
#include "uiform.h"
#include "uiedit.h"
#include "uilabel.h"
#include "tools.h"
#include "uiformmgr.h"
#include "character.h"
#include "uigoodsgrid.h"
#include "UIBoxForm.h"
#include "UITradeForm.h"
#include "UIFastCommand.h"
#include "itemrecord.h"
#include "NetProtocol.h"
#include "UIItemCommand.h"
#include "UITemplete.h"
#include "UIBoatForm.h"
#include "characterrecord.h"
#include "gameapp.h"
#include "PacketCmd.h"
#include "shipset.h"
#include "UIMemo.h"
#include "worldscene.h"
#include "ShipFactory.h"
#include "UIGoodsGrid.h"
#include "UIEquipForm.h"
#include "UIGlobalVar.h"
#include "rolecommon.h"
#include "itemrecord.h"
#include "UIEdit.h"
#include "UIBoxForm.h"
#include "StringLib.h"

using namespace std;

//#define SAFE_DELETE(x) if ( (x) ) delete (x), (x) = 0;
int stallGridID = 0;
//CItemCommand selectedItem = NULL;

namespace GUI
{

	struct CBoothMgr::SBoothItem
	{
		long lId;   // ����ID
		int  iPrice; // ������ʾ�۸�
		int  iNum;   // ���߸���
		int	 iTotal; // ����Ŀ
		int  iEquipIndex; // װ������
		int	 iBoothIndex; // ��̯����
		CGoodsGrid* pkEquipGrid;	//����Ҫ���
		CGoodsGrid* pkBoothGrid;	//����Ҫ���

		int itemTotalNum;
		int itemGetIsPile;
		
		SBoothItem() 
			: lId(0), iPrice(0), iNum(0), iEquipIndex(0), 
			  iBoothIndex(0), pkEquipGrid(0), pkBoothGrid(0),itemTotalNum(0),itemGetIsPile(0)
		{}
	};

	//~ ------------------------------------------------------------------
	CBoothMgr::CBoothMgr() : m_pkCurrSetupBooth(0),  m_iBoothItemMaxNum(0), 
		m_bSetupedBooth(false), m_NumBox(0), m_TradeBox(0), m_SelectBox(0)
	{
	}

	//~ ------------------------------------------------------------------
	CBoothMgr::~CBoothMgr()
	{
		ClearBoothItems();
	}


	//~ ------------------------------------------------------------------
	bool CBoothMgr::Init() // ��̯��Ϣ��ʼ��
	{
		CFormMgr &mgr = CFormMgr::s_Mgr;

		frmBooth = mgr.Find("frmBooth", enumMainForm); // ���Ұ�̯����
		if ( !frmBooth)
		{
			g_logManager.InternalLog(LogLevel::Debug, "common", g_oLangRec.GetString(445));
			return false;
		}
		frmBooth->evtEntrustMouseEvent = _MainMouseBoothEvent ; // ����Ϣ�¼��Ĵ���
		frmBooth->evtClose = _MainBoothOnCloseEvent;

		lblOwnerName = dynamic_cast<CLabel*>(frmBooth->Find("lblOwnerName"));
		if (!lblOwnerName)
			return Error(g_oLangRec.GetString(446),
			frmBooth->GetName(), "lblOwnerName");

		edtBoothName = dynamic_cast<CEdit*>(frmBooth->Find("edtBoothName"));
		if (!lblOwnerName)
			return Error(g_oLangRec.GetString(446),
			frmBooth->GetName(), "edtBoothName");

		btnSetupBooth = dynamic_cast<CTextButton*>(frmBooth->Find("btnSetupBooth"));
		if (!btnSetupBooth)
			return Error(g_oLangRec.GetString(446),
			frmBooth->GetName(), "btnSetupBooth");

		btnPullStakes = dynamic_cast<CTextButton*>(frmBooth->Find("btnPullStakes"));
		if (!btnSetupBooth)
			return Error(g_oLangRec.GetString(446),
			frmBooth->GetName(), "btnPullStakes");


		grdBoothItem = dynamic_cast<CGoodsGrid*>(frmBooth->Find("grdBoothItem"));
		if (!grdBoothItem) 
			return Error(g_oLangRec.GetString(446),
			frmBooth->GetName(), "grdBoothItem");
		grdBoothItem->evtBeforeAccept = CUIInterface::_evtDragToGoodsEvent;
		grdBoothItem;
		grdBoothItem->SetShowStyle(CGoodsGrid::enumSale);
		grdBoothItem->SetIsHint(true);


		return true;

	}

	void CBoothMgr::End()
	{
	}

	void CBoothMgr::CloseForm() // �رհ�̯����
	{
		CCharacter* pkCha = g_stUIBoat.GetHuman();
		if (!pkCha)
			return;
		if (pkCha->getAttachID() == m_dwOwnerId && pkCha->IsShop())
			return;

		if (frmBooth->GetIsShow())
		{
			CloseBoothUI();
		}
	}


	
	void CBoothMgr::SearchAllStalls() {
		g_stUIBox.ShowNumberBox(_SearchStallID, -1,  "Enter Item Name", false);
	}
	
	void CBoothMgr::_SearchStallID(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
	{
		if( nMsgType!=CForm::mrYes ) 
		{
			return;
		}

		stNumBox* kItemPriceBox = (stNumBox*)pSender->GetForm()->GetPointer();
		if (!kItemPriceBox) return;
		
		const char*  strName;
		
		kItemPriceBox->GetString(strName);
				
		CItemRecord* pInfo= GetItemRecordInfo( strName );
		
		if (!pInfo){
			g_pGameApp->MsgBox( "Invalid Item");
			return;
		}
		
		
		//send to server.
		CS_StallSearch(pInfo->lID);
	}

	
	//~ ------------------------------------------------------------------
	void CBoothMgr::ShowSetupBoothForm(int iLevel)  // ��ʾ���ð�̯����
	{
		frmBooth->SetIsShow(!frmBooth->GetIsShow());

		//�����ǰ����Ʒ
		ClearBoothItems();

		//���ݵȼ�ȡ�ð�̯��λ��
		m_iBoothItemMaxNum = GetItemNumByLevel(iLevel);
		m_kBoothItems.reserve(m_iBoothItemMaxNum);
		for (int i(0); i<m_iBoothItemMaxNum; i++)
		{
			m_kBoothItems[i] = 0;
		}
		//���ð�̯�����UI�ؼ�
		int col = grdBoothItem->GetCol();
		int row = m_iBoothItemMaxNum / col;
		if( m_iBoothItemMaxNum % col ) row++;

		grdBoothItem->Clear();
		grdBoothItem->SetContent( row, col );
		grdBoothItem->Init();
		grdBoothItem->Refresh();

		//����̯��ID
		CCharacter* pkCha = g_stUIBoat.GetHuman();
		m_dwOwnerId = pkCha->getAttachID();

		btnSetupBooth->SetIsShow(true);
		btnPullStakes->SetIsShow(false);
		lblOwnerName->SetCaption("");
		edtBoothName->SetCaption("");
		edtBoothName->SetIsEnabled(true);

		//�����ð�̯�������ҵ���Ʒ��
		OpenBoothUI();
	}

	//~ ------------------------------------------------------------------
	void CBoothMgr::ShowTradeBoothForm(DWORD dwOwnerId, const char *szBoothName, int nItemNum)
	{
		m_dwOwnerId = dwOwnerId;
		m_iBoothItemMaxNum = nItemNum;

		ClearBoothItems();

		//ֱ�ӵ���󼶣�����Ҫ֪���ȼ�
		m_iBoothItemMaxNum = GetItemNumByLevel(3);
		m_kBoothItems.resize(m_iBoothItemMaxNum, nullptr);
		for (auto* item : m_kBoothItems)
		{
			item = nullptr;
		}
		//���ð�̯�����UI�ؼ�
		int col = grdBoothItem->GetCol();
		int row = m_iBoothItemMaxNum / col;
		if( m_iBoothItemMaxNum % col ) row++;

		grdBoothItem->Clear();
		grdBoothItem->SetContent( row, col );
		grdBoothItem->Init();
		grdBoothItem->Refresh();

		// ���ý��׽����UI�ؼ�
		btnSetupBooth->SetIsShow(false);
		btnPullStakes->SetIsShow(false);
		CGameScene* pScene = CGameApp::GetCurScene();
		if (!pScene) return;
		CCharacter* pCha = pScene->SearchByID(dwOwnerId);
		if (!pCha) return;
		lblOwnerName->SetCaption(pCha->getHumanName());
		edtBoothName->SetCaption(szBoothName);
		edtBoothName->SetIsEnabled(false);

		//�򿪽��װ�̯�������ҵ���Ʒ��
		OpenBoothUI();
	}

	void CBoothMgr::AddTradeBoothItem(int iGrid, DWORD dwItemID, int iCount, DWORD dwMoney)
	{
		if (iCount > 0)
		{
			//assert(!m_pkCurrSetupBooth);
			m_pkCurrSetupBooth = new SBoothItem;
			m_pkCurrSetupBooth->pkBoothGrid = grdBoothItem;
			m_pkCurrSetupBooth->iBoothIndex = iGrid;
			m_pkCurrSetupBooth->lId = dwItemID;
			m_pkCurrSetupBooth->iNum = iCount;
			m_pkCurrSetupBooth->iPrice = dwMoney;

			AddBoothItem(m_pkCurrSetupBooth);

			m_pkCurrSetupBooth = 0;
		}
	}

	void CBoothMgr::AddTradeBoothBoat(int iGrid, DWORD dwItemID, int iCount, DWORD dwMoney, NET_CHARTRADE_BOATDATA& Data)
	{
		AddTradeBoothItem(iGrid, dwItemID, iCount, dwMoney);

		CItemCommand* pItemCommand = dynamic_cast<CItemCommand*>(grdBoothItem->GetItem(iGrid));
		if (!pItemCommand)
			return;

		pItemCommand->SetBoatHint(&Data);
	}

	void CBoothMgr::AddTradeBoothGood(int iGrid, DWORD dwItemID, int iCount, DWORD dwMoney, SItemGrid& rSItemGrid)
	{
		AddTradeBoothItem(iGrid, dwItemID, iCount, dwMoney);

		CItemCommand* pItemCommand = dynamic_cast<CItemCommand*>(grdBoothItem->GetItem(iGrid));
		if (!pItemCommand)
			return;

		pItemCommand->SetData(rSItemGrid);
	}


	void CBoothMgr::RemoveTradeBoothItem( DWORD dwCharID, int iGrid, int iCount)
	{
		if (dwCharID == m_dwOwnerId)
		{
			//assert(!m_pkCurrSetupBooth);
			m_pkCurrSetupBooth = m_kBoothItems[iGrid];
			RemoveBoothItemByNum(m_pkCurrSetupBooth, iCount);
			m_pkCurrSetupBooth = 0;
		}
	}


	//~ ------------------------------------------------------------------
	int CBoothMgr::GetItemNumByLevel(int iLevel)
	{
		//���ݰ�̯���ܵȼ��õ����԰�̯����λ��,����ĵ�
		return iLevel * 6;
	}

	//~ ------------------------------------------------------------------
	void CBoothMgr::ClearBoothItems()
	{
		for (size_t i(0); i<m_kBoothItems.size(); ++i)
		{
			//delete m_kBoothItems[i];
			SAFE_DELETE(m_kBoothItems[i]); // UI��������
			m_kBoothItems[i] = 0;
		}
	}

	//~ ------------------------------------------------------------------
	void CBoothMgr::AddBoothItem(SBoothItem* pBoothItem)
	{
		if (!pBoothItem)
			return ;

		//�����ǰʱ̯��,����Ҫ��������װ����
		CCharacter* pkCha = g_stUIBoat.GetHuman();
		if (!pkCha)
			return ;

		if (pkCha->getAttachID() == GetOwnerId())
		{
			//��������ӵ�����λ�Ѿ�������λ��,��ô�����Ʒ���滻��ǰ����Ʒ
			SBoothItem* pSourceBoothItem = m_kBoothItems[pBoothItem->iBoothIndex];
			if (pSourceBoothItem)
			{
				RemoveBoothItemByNum(pSourceBoothItem, pSourceBoothItem->iNum);
			}

			// װ������λ������Ϊ������״̬
			pBoothItem->pkEquipGrid->GetItem(pBoothItem->iEquipIndex)->SetIsValid(false);
		}

		m_kBoothItems[pBoothItem->iBoothIndex] = pBoothItem;

		// grdBoothItem������ʾ
		CItemRecord* pInfo = GetItemRecordInfo( pBoothItem->lId);
		if (pBoothItem->pkEquipGrid)
		{
			CItemCommand* pOldItem = dynamic_cast<CItemCommand*>(pBoothItem->pkEquipGrid->GetItem(pBoothItem->iEquipIndex));
			if (pOldItem)
			{
				CItemCommand* pNewItem = new CItemCommand(*pOldItem);
				pNewItem->SetTotalNum(pBoothItem->iNum);
				pNewItem->SetPrice(pBoothItem->iPrice);
				pNewItem->SetIsValid(true);
				grdBoothItem->SetItem(pBoothItem->iBoothIndex, pNewItem);
			}
		}
		else
		{
			CItemCommand* pItem = new CItemCommand( pInfo );
			pItem->SetTotalNum(pBoothItem->iNum);
			pItem->SetPrice(pBoothItem->iPrice);
			grdBoothItem->SetItem(pBoothItem->iBoothIndex, pItem);
		}
	}

	//~ ------------------------------------------------------------------
	void CBoothMgr::RemoveBoothItemByNum(SBoothItem* pBoothItem, int iNum)
	{
		if (!pBoothItem)
			return;

		int iBoothIndex = pBoothItem->iBoothIndex;
		if (iNum == pBoothItem->iNum)
		{	//��ȥ���еĸ���Ʒ
			//�����ǰʱ̯��,����Ҫ��������װ����
			CCharacter* pkCha = g_stUIBoat.GetHuman();
			if (!pkCha)
				return ;

			if (pkCha->getAttachID() == GetOwnerId())
			{
				CCommandObj* pItem = pBoothItem->pkEquipGrid->GetItem(pBoothItem->iEquipIndex);
				if (pItem)
				{
					pItem->SetIsValid(true);
					//int num = pItem->GetTotalNum();
					//num = num - pBoothItem->iTotal + iNum;
					//pItem->SetTotalNum(pItem->GetTotalNum() - pBoothItem->iTotal + pBoothItem->iNum);
				}
			}
			
			pBoothItem->pkBoothGrid->DelItem(iBoothIndex);

			if (m_kBoothItems[iBoothIndex])
			{
				//delete m_kBoothItems[iBoothIndex];
				SAFE_DELETE(m_kBoothItems[iBoothIndex]); // UI��������
				m_kBoothItems[iBoothIndex] = 0;
			}
		}
		else
		{	//�����ֵĸ���Ʒ�϶���װ����
			pBoothItem->iNum -= iNum;

			//����UI�е�����
			pBoothItem->pkBoothGrid->GetItem(iBoothIndex)->SetTotalNum(pBoothItem->iNum);

		}
	}

	//~ ------------------------------------------------------------------
	void CBoothMgr::SetupBoothSuccess()
	{
		//�޸�UI
		btnSetupBooth->SetIsShow(false);
		btnPullStakes->SetIsShow(true);
		edtBoothName->SetIsEnabled(false);

		SetSetupedBooth(true);

		g_stUIEquip.GetItemForm()->SetIsShow(m_isOldEquipFormShow);

		//���Ű�̯����
	}

	void CBoothMgr::PullBoothSuccess() const {
		//终止摆摊动画

		for (int i(0); i < g_stUIBooth.m_iBoothItemMaxNum; i++) {
			if (g_stUIBooth.m_kBoothItems[i]) {
				g_stUIBooth.RemoveBoothItemByNum(g_stUIBooth.m_kBoothItems[i],
					g_stUIBooth.m_kBoothItems[i]->iNum);
			}
		}
		g_stUIBooth.ClearBoothItems();

		//关闭表单
		g_stUIBooth.CloseBoothUI();
	}


	void CBoothMgr::OpenBoothUI()
	{
		frmBooth->SetPos(150, 150);
		frmBooth->Reset();
		frmBooth->Refresh();
		frmBooth->Show();

		//ͬʱ����ҵ�װ����
		int x = frmBooth->GetX() + frmBooth->GetWidth();
		int y = frmBooth->GetY();
		g_stUIEquip.GetItemForm()->SetPos(x, y);
		g_stUIEquip.GetItemForm()->Refresh();

		if (!(m_isOldEquipFormShow = g_stUIEquip.GetItemForm()->GetIsShow()))
		{
			g_stUIEquip.GetItemForm()->Show();
		}

	}

	void CBoothMgr::CloseBoothUI()
	{
		frmBooth->Close();
		g_stUIEquip.GetItemForm()->SetIsShow(m_isOldEquipFormShow);

		if (m_NumBox)
			m_NumBox->frmDialog->Close();

		if (m_TradeBox)
			m_TradeBox->frmDialog->Close();

		if (m_SelectBox)
			m_SelectBox->frmDialog->Close();

	}

	//~ ------------------------------------------------------------------
	void CBoothMgr::_MainMouseBoothEvent(CCompent *pSender, int nMsgType, 
										 int x, int y, DWORD dwKey)
	{
		string name = pSender->GetName();
		if( name=="btnNo"  || name == "btnClose" )  
		{ ///�رձ���
			return;
		}
		else if ( name == "btnSetupBooth")
		{	/// ���¡����ð�̯����

			if (strlen(g_stUIBooth.edtBoothName->GetCaption()) == 0)
			{
				g_pGameApp->MsgBox(g_oLangRec.GetString(447));
				return;
			}
			
			//����̯���Ƿ��зǷ��ַ�
			string sName(g_stUIBooth.edtBoothName->GetCaption());
			if (!CTextFilter::IsLegalText(CTextFilter::NAME_TABLE, sName))
			{
				g_pGameApp->MsgBox(g_oLangRec.GetString(448));
				return ;
			}

			//������Ϣ����
			mission::NET_STALL_ALLDATA netCreateBoothData;
			int iNum(0);	//̯λ�ڵ�����
			for (int i(0); i<g_stUIBooth.m_iBoothItemMaxNum; i++)
			{
				if (g_stUIBooth.m_kBoothItems[i])
				{
					netCreateBoothData.Info[iNum].dwMoney = DWORD(g_stUIBooth.m_kBoothItems[i]->iPrice);
					netCreateBoothData.Info[iNum].byCount = BYTE(g_stUIBooth.m_kBoothItems[i]->iNum);
					netCreateBoothData.Info[iNum].byIndex = BYTE(g_stUIBooth.m_kBoothItems[i]->iEquipIndex);
					netCreateBoothData.Info[iNum].byGrid  = BYTE(g_stUIBooth.m_kBoothItems[i]->iBoothIndex);
					iNum++;
				}
			}
			netCreateBoothData.byNum = BYTE(iNum);
			if (netCreateBoothData.byNum > 0)
			{
				//���ʹ���̯λЭ��
				if( CCharacter* pCha = CGameScene::GetMainCha() )
				{
					pCha->GetActor()->Stop();
				}

				CS_StallInfo( g_stUIBooth.edtBoothName->GetCaption(), netCreateBoothData );
			}

			return ;			
		}
		else if (name == "btnPullStakes")
		{	/// ���¡���̯����
			g_stUIBooth.CloseBoothUI();
		}

	}

	void CBoothMgr::_MainBoothOnCloseEvent(CForm* pForm, bool& IsClose)
	{

		if (g_stUIBoat.GetHuman()->IsShop() && g_stUIBooth.IsSetupedBooth())
		{
			CCharacter* pMainCha = g_stUIBoat.GetHuman();
			if (pMainCha && pMainCha->getAttachID() == g_stUIBooth.m_dwOwnerId)
			{
				pMainCha->PlayPose(POSE_WAITING, PLAY_LOOP_SMOOTH);
				CS_StallClose();
			}
		}

		for (int i(0); i<g_stUIBooth.m_iBoothItemMaxNum; i++)
		{
			if (g_stUIBooth.m_kBoothItems[i])
			{
				g_stUIBooth.RemoveBoothItemByNum(g_stUIBooth.m_kBoothItems[i], 
					g_stUIBooth.m_kBoothItems[i]->iNum);
			}
		}

		CCharacter* pkCha = g_stUIBoat.GetHuman();
		if (!pkCha)
			return ;

		if (pkCha->getAttachID() == g_stUIBooth.GetOwnerId())
		{
			CGoodsGrid* pEquipGrid = g_stUIEquip.GetGoodsGrid();
			if (pEquipGrid)
			{
				pEquipGrid->SetItemValid(true);

				//CCommandObj* pItem;
				//for (int i(0); i<pEquipGrid->GetMaxNum(); i++)
				//{
				//	pItem = pEquipGrid->GetItem(i);
				//	if (pItem)
    //                    pItem->SetIsValid(true);
				//}

			}

		}

		g_stUIBooth.ClearBoothItems();

		g_stUIEquip.GetItemForm()->SetIsShow(g_stUIBooth.m_isOldEquipFormShow);
	}


	//~ ------------------------------------------------------------------
	void CBoothMgr::_InquireSetupPushItemNumEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
	{
		if( nMsgType!=CForm::mrYes ) 
		{
			SAFE_DELETE( g_stUIBooth.m_pkCurrSetupBooth );
			return;
		}

		if (!g_stUIBooth.m_pkCurrSetupBooth)
			return;

		stNumBox* kItemNumBox = (stNumBox*)pSender->GetForm()->GetPointer();
		if (!kItemNumBox) return;

		if( kItemNumBox->GetNumber()<=0 ) 
		{
			g_pGameApp->MsgBox( g_oLangRec.GetString(449) );
			return;
		}

		g_stUIBooth.m_pkCurrSetupBooth->iNum = kItemNumBox->GetNumber();	// ������������
		g_stUIBooth.m_pkCurrSetupBooth->iTotal = kItemNumBox->GetNumber();
		

		//ѯ�ʼ۸�
		g_stUIBooth.m_NumBox = g_stUIBox.ShowNumberBox(_InquireSetupPushItemPriceEvent, -1, g_oLangRec.GetString(450), false);

	}
	
	//~ ------------------------------------------------------------------
	void CBoothMgr::_InquireSetupPushItemPriceEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
	{
		if( nMsgType!=CForm::mrYes ) 
		{
			SAFE_DELETE( g_stUIBooth.m_pkCurrSetupBooth );
			return;
		}

		if (!g_stUIBooth.m_pkCurrSetupBooth)
			return;

		stNumBox* kItemPriceBox = (stNumBox*)pSender->GetForm()->GetPointer();
		if (!kItemPriceBox) return;

		if( kItemPriceBox->GetNumber()<=0 ) 
		{
			g_pGameApp->MsgBox( g_oLangRec.GetString(451) );
			return;
		}

		int iTotal = g_stUIBooth.m_pkCurrSetupBooth->iNum * kItemPriceBox->GetNumber();
		if (iTotal >= 1000000000)
		{
			g_pGameApp->MsgBox( g_oLangRec.GetString(452) );
			return;
		}

		g_stUIBooth.m_pkCurrSetupBooth->iPrice = kItemPriceBox->GetNumber();	// �������ĵ���

		g_stUIBooth.AddBoothItem(g_stUIBooth.m_pkCurrSetupBooth);

		g_stUIBooth.m_pkCurrSetupBooth = 0;

	}

	//~ ------------------------------------------------------------------
	void CBoothMgr::_InquireSetupPopItemNumEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
	{
		if( nMsgType!=CForm::mrYes ) 
		{
			g_stUIBooth.m_pkCurrSetupBooth = 0;
			return;
		}

		if (!g_stUIBooth.m_pkCurrSetupBooth)
			return;

		stNumBox* kItemNumBox = (stNumBox*)pSender->GetForm()->GetPointer();
		if (!kItemNumBox) return;

		g_stUIBooth.RemoveBoothItemByNum(g_stUIBooth.m_pkCurrSetupBooth, kItemNumBox->GetNumber());
	}

	//~ ------------------------------------------------------------------
	void CBoothMgr::_InquireTradeItemNumEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
	{
		if( nMsgType!=CForm::mrYes ) 
		{
			return;
		}
		stNumBox* kItemNumBox = (stNumBox*)pSender->GetForm()->GetPointer();
		if (!kItemNumBox) return;
	}

	//~ ------------------------------------------------------------------
	void CBoothMgr::_BuyGoodsEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
	{
		if(nMsgType != CForm::mrYes) 
		{
			g_stUIBooth.m_pkCurrSetupBooth = 0;
			return;
		}

		if (!g_stUIBooth.m_pkCurrSetupBooth)
		{
			g_pGameApp->MsgBox(g_oLangRec.GetString(453));
			return;
		}

		stTradeBox* pItemNumBox = (stTradeBox*)pSender->GetForm()->GetPointer();
		if (!pItemNumBox) return;

		CS_StallBuy( g_stUIBooth.m_dwOwnerId, 
					 g_stUIBooth.m_pkCurrSetupBooth->iBoothIndex, 
					 pItemNumBox->GetTradeNum(),
					 stallGridID );

	}

	//~ ------------------------------------------------------------------
	void CBoothMgr::_BuyAGoodEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
	{
		if( nMsgType!=CForm::mrYes ) 
		{
			g_stUIBooth.m_pkCurrSetupBooth = 0;
			return;
		}

		if (!g_stUIBooth.m_pkCurrSetupBooth)
		{
			g_pGameApp->MsgBox(g_oLangRec.GetString(453));
			return;
		}

		CS_StallBuy( g_stUIBooth.m_dwOwnerId, g_stUIBooth.m_pkCurrSetupBooth->iBoothIndex, 1 ,stallGridID);
	}

	//~ ------------------------------------------------------------------
	bool CBoothMgr::PushToBooth(CGoodsGrid& rkDrag, CGoodsGrid& rkSelf, int nGridID, CCommandObj& rkItem)
	{
		if (!rkItem.GetIsValid())
			return false;

		CItemCommand* pkItemCmd = dynamic_cast<CItemCommand*>(&rkItem);
		if (!pkItemCmd)	
			return false;

		CCharacter* pkCha = g_stUIBoat.GetHuman();
		if (!pkCha)
			return false;

		if (pkCha->getAttachID() == GetOwnerId())
		{	///��ǰ�򿪽���ľ���̯��

			//�ж���Ʒ�Ƿ�ɽ���
			CItemRecord* pItemRecord = pkItemCmd->GetItemInfo();
			if (!pItemRecord)
				return false;

			if (pItemRecord->chIsTrade)
				return PushToBoothSetup(rkDrag, rkSelf, nGridID, *pkItemCmd);
		}
		else
		{	///��ǰ�򿪽���Ĳ���̯���������Ϊ�ǽ���
			return PushToBoothTrade(rkDrag, rkSelf, nGridID, *pkItemCmd);
		}
		return true;

	}

	//~ ------------------------------------------------------------------
	bool CBoothMgr::PopFromBooth(CGoodsGrid& rkDrag, CGoodsGrid& rkSelf, int nGridID, CCommandObj& rkItem)
	{
		if (!rkItem.GetIsValid())
			return false;

		CItemCommand* pkItemCmd = dynamic_cast<CItemCommand*>(&rkItem);
		if (!pkItemCmd)	
			return false;

		CCharacter* pkCha = g_stUIBoat.GetHuman();
		if (!pkCha)
			return false;

		if (pkCha->getAttachID() == GetOwnerId())
		{	///��ǰ�򿪽���ľ���̯���������Ϊ������̯λ
			return PopFromBoothSetup(rkDrag, rkSelf, nGridID, *pkItemCmd);
		}
		else
		{	///��ǰ�򿪽���Ĳ���̯���������Ϊ�ǽ���
			return PopFromBoothTrade(rkDrag, rkSelf, nGridID, *pkItemCmd);
		}

		return true;
	}

	//~ ------------------------------------------------------------------
	bool CBoothMgr::BoothToBooth(CGoodsGrid& rkDrag, CGoodsGrid& rkSelf, int nGridID, CCommandObj& rkItem)
	{
		CCharacter* pkCha = g_stUIBoat.GetHuman();
		if (!pkCha)
			return false;

		if (pkCha->getAttachID() == GetOwnerId())
		{	///��ǰ�򿪽���ľ���̯���������Ϊ������̯λ
		}
		else
		{	///��ǰ�򿪽���Ĳ���̯���������Ϊ�ǽ���

		}

		return true;
	}

	//~ ------------------------------------------------------------------
	bool CBoothMgr::PushToBoothSetup(CGoodsGrid& rkDrag, CGoodsGrid& rkSelf, int nGridID, CItemCommand& rkItemCmd)
	{
		// ����Ѿ��ڰ�̯״̬,���ܼ��������϶�
		CCharacter *pMainCha = g_stUIBoat.GetHuman();
		if (pMainCha && pMainCha->IsShop())
		{
			return false;
		}

		m_pkCurrSetupBooth = new SBoothItem;
		m_pkCurrSetupBooth->lId = rkItemCmd.GetItemInfo()->lID;
		m_pkCurrSetupBooth->iBoothIndex = nGridID;
		m_pkCurrSetupBooth->iEquipIndex = rkDrag.GetDragIndex();
		m_pkCurrSetupBooth->pkEquipGrid = &rkDrag;
		m_pkCurrSetupBooth->pkBoothGrid = &rkSelf;
		
		m_pkCurrSetupBooth->itemTotalNum = rkItemCmd.GetTotalNum();
		m_pkCurrSetupBooth->itemGetIsPile = rkItemCmd.GetIsPile();

		//�ж��϶���Item�Ƿ���ص�����������һ��
		
		//selectedItem = rkItemCmd;
		//g_stUIBox.ShowSelectBox(_PushItemCurrencyType,"Use gold as currency?");
		if (g_stUIBooth.m_pkCurrSetupBooth->itemGetIsPile&& g_stUIBooth.m_pkCurrSetupBooth->itemTotalNum > 1)
			{
				g_stUIBooth.m_NumBox = g_stUIBox.ShowNumberBox(_InquireSetupPushItemNumEvent, g_stUIBooth.m_pkCurrSetupBooth->itemTotalNum, g_oLangRec.GetString(454), false);
			}
			else
			{
				g_stUIBooth.m_pkCurrSetupBooth->iNum = 1;	//����Ϊ1
				g_stUIBooth.m_pkCurrSetupBooth->iTotal = 1;
				g_stUIBooth.m_NumBox = g_stUIBox.ShowNumberBox(_InquireSetupPushItemPriceEvent, -1, g_oLangRec.GetString(450), false);
			}
		return true;
	}

	
	void CBoothMgr::_PushItemCurrencyType(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
	{
		if( true ) //nMsgType==CForm::mrYes
		{
			//normal stall item flow.
			if (g_stUIBooth.m_pkCurrSetupBooth->itemGetIsPile&& g_stUIBooth.m_pkCurrSetupBooth->itemTotalNum > 1)
			{
				g_stUIBooth.m_NumBox = g_stUIBox.ShowNumberBox(_InquireSetupPushItemNumEvent, g_stUIBooth.m_pkCurrSetupBooth->itemTotalNum, g_oLangRec.GetString(454), false);
			}
			else
			{
				g_stUIBooth.m_pkCurrSetupBooth->iNum = 1;	//����Ϊ1
				g_stUIBooth.m_pkCurrSetupBooth->iTotal = 1;
				g_stUIBooth.m_NumBox = g_stUIBox.ShowNumberBox(_InquireSetupPushItemPriceEvent, -1, g_oLangRec.GetString(450), false);
			}
			return;
		}else{
			/*
			//chad item stall flow.
			if (g_stUIBooth.m_pkCurrSetupBooth->itemGetIsPile&& g_stUIBooth.m_pkCurrSetupBooth->itemTotalNum > 1)
			{
				g_stUIBooth.m_NumBox = g_stUIBox.ShowNumberBox(_PushItemTradeNumEvent, g_stUIBooth.m_pkCurrSetupBooth->itemTotalNum, g_oLangRec.GetString(454), false);
			}
			else
			{
				g_stUIBooth.m_pkCurrSetupBooth->iNum = 1;	//����Ϊ1
				g_stUIBooth.m_pkCurrSetupBooth->iTotal = 1;
				g_stUIBooth.m_NumBox = g_stUIBox.ShowNumberBox(_PushItemTradeID, -1, "Enter Item Name", false);
			}
			return;
			*/
		}
	}
	
	void CBoothMgr::_PushItemTradeNumEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
	{
		if( nMsgType!=CForm::mrYes ) 
		{
			SAFE_DELETE( g_stUIBooth.m_pkCurrSetupBooth );
			return;
		}

		if (!g_stUIBooth.m_pkCurrSetupBooth)
			return;

		stNumBox* kItemNumBox = (stNumBox*)pSender->GetForm()->GetPointer();
		if (!kItemNumBox) return;

		if( kItemNumBox->GetNumber()<=0 ) 
		{
			g_pGameApp->MsgBox( g_oLangRec.GetString(449) );
			return;
		}

		g_stUIBooth.m_pkCurrSetupBooth->iNum = kItemNumBox->GetNumber();	// ������������
		g_stUIBooth.m_pkCurrSetupBooth->iTotal = kItemNumBox->GetNumber();
		

		//ѯ�ʼ۸�
		g_stUIBooth.m_NumBox = g_stUIBox.ShowNumberBox(_PushItemTradeID, -1,  "Enter Item Name", false);

	}
	
	
	void CBoothMgr::_PushItemTradeQuantity(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey){
		if( nMsgType!=CForm::mrYes ) 
		{
			SAFE_DELETE( g_stUIBooth.m_pkCurrSetupBooth );
			return;
		}
		
		if (!g_stUIBooth.m_pkCurrSetupBooth)
			return;
		
		stNumBox* kItemPriceBox = (stNumBox*)pSender->GetForm()->GetPointer();
		if (!kItemPriceBox) return;

		int quantity = kItemPriceBox->GetNumber();
		g_stUIBooth.m_pkCurrSetupBooth->iPrice += quantity * 100000;
		
		g_stUIBooth.AddBoothItem(g_stUIBooth.m_pkCurrSetupBooth);
		g_stUIBooth.m_pkCurrSetupBooth = 0;
	}
	
	void CBoothMgr::_PushItemTradeID(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
	{
		if( nMsgType!=CForm::mrYes ) 
		{
			SAFE_DELETE( g_stUIBooth.m_pkCurrSetupBooth );
			return;
		}

		if (!g_stUIBooth.m_pkCurrSetupBooth)
			return;

		stNumBox* kItemPriceBox = (stNumBox*)pSender->GetForm()->GetPointer();
		if (!kItemPriceBox) return;

		int itemID = kItemPriceBox->GetNumber();
		
		const char*  strName;
		
		kItemPriceBox->GetString(strName);

		CItemRecord* pInfo= GetItemRecordInfo( strName );

		
		if (!pInfo || pInfo->sType == 43 || !pInfo->chIsTrade){
			g_pGameApp->MsgBox( "Invalid Item");
			return;
		}
		
		g_stUIBooth.m_pkCurrSetupBooth->iPrice = 2000000000 + pInfo->lID;
		g_stUIBooth.m_NumBox = g_stUIBox.ShowNumberBox(_PushItemTradeQuantity, pInfo->nPileMax, "Enter Required Item Quantity", false);
		
	}
	
	//~ ------------------------------------------------------------------
	bool CBoothMgr::PopFromBoothSetup(CGoodsGrid& rkDrag, CGoodsGrid& rkSelf, int nGridID, CItemCommand& rkItemCmd)
	{
		CCharacter* pMainCha = g_stUIBoat.GetHuman();
		if (pMainCha && pMainCha->IsShop())
		{
			return false;
		}

		m_pkCurrSetupBooth = m_kBoothItems[rkDrag.GetDragIndex()];
		if (!m_pkCurrSetupBooth)
			return false;

		RemoveBoothItemByNum(m_pkCurrSetupBooth, rkItemCmd.GetTotalNum());

		m_pkCurrSetupBooth = 0;

		return true;
	}

	//~ ------------------------------------------------------------------
	bool CBoothMgr::PushToBoothTrade(CGoodsGrid& rkDrag, CGoodsGrid& rkSelf, int nGridID, CItemCommand& rkItemCmd)
	{
		// ����ʱ�����ܽ�Item�϶���Booth��
		return false;
	}

	//~ ------------------------------------------------------------------
	bool CBoothMgr::PopFromBoothTrade(CGoodsGrid& rkDrag, CGoodsGrid& rkSelf, int nGridID, CItemCommand& rkItemCmd)
	{
		stallGridID = nGridID;
		m_pkCurrSetupBooth = m_kBoothItems[rkDrag.GetDragIndex()];
		if (!m_pkCurrSetupBooth)
			return false;

		//�ж��϶���Item�Ƿ���ص�����������һ��
		if (rkItemCmd.GetIsPile() && rkItemCmd.GetTotalNum() > 1)
		{/// ѯ��������Ȼ����ִ�н��׵Ĳ���
			m_TradeBox = g_stUIBox.ShowTradeBox(_BuyGoodsEvent, 
								   (float)m_pkCurrSetupBooth->iPrice, 
								   m_pkCurrSetupBooth->iNum, 
								   rkItemCmd.GetItemInfo()->szName );

		}
		else
		{/// ֱ��ִ�н��ײ���
			char buf[256] = { 0 };

			/*sprintf(buf, g_oLangRec.GetString(455),
				StringSplitNum( rkItemCmd.GetPrice() ),
				ConvertNumToChinese(rkItemCmd.GetPrice()).c_str(),
				rkItemCmd.GetName());*/
			int price = rkItemCmd.GetPrice();
			if (price > 2000000000){
				price -= 2000000000;
				int num = price / 100000;
				int ID = price - (num * 100000);
				
				CItemRecord* pInfo= GetItemRecordInfo( ID );
				
				if(pInfo){
					sprintf(buf, "Do you wish to trade\n%dx %s\nfor %s",
					num,
					pInfo->szName,
					rkItemCmd.GetName());
				}else{
					sprintf(buf, "Do you wish to trade\n%dx Undefined\nfor %s",
					num,
					rkItemCmd.GetName());
				}
			}else{
				sprintf(buf, "Do you wish to spend\n%sg\nto purchase [%s]",
				StringSplitNum( rkItemCmd.GetPrice() ),
				//ConvertNumToChinese(rkItemCmd.GetPrice()).c_str(),
				rkItemCmd.GetName());
			}
			
			m_SelectBox = g_stUIBox.ShowSelectBox(_BuyAGoodEvent, buf, true);
		}

		return true;
	}

	//add by ALLEN 2007-10-16
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "state_reading.h"
#include "Actor.h"


	CCharacter* CReadBookMgr::_pCha = 0;

	bool CReadBookMgr::IsCanReadBook(CCharacter* pCha)
	{
		_pCha = pCha;

		// �ж϶��������
		CItemCommand* pRHand = g_stUIEquip.GetEquipItem(enumEQUIP_RHAND);
		CItemCommand* pNeck  = g_stUIEquip.GetEquipItem(enumEQUIP_NECK);
		if(!pRHand || !pNeck)
		{
			g_pGameApp->MsgBox(g_oLangRec.GetString(941));	// "����װ��ѧ��֤����"
			return false;
		}

		long nRHandID = pRHand->GetItemInfo()->lID;
		long nNeckID  = pNeck->GetItemInfo()->lID;
		if( (3243 <= nRHandID && nRHandID <= 3278) && nNeckID == 3289)
		{
			return true;
		}
		
		return false;
		
	}


	bool CReadBookMgr::ShowReadBookForm()
	{
		CBoxMgr::ShowSelectBox(_evtSelectBox, g_oLangRec.GetString(942), true);	// "ȷ���Ƿ�Ҫ���飿"
		return true;
	}


	void CReadBookMgr::_evtSelectBox(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
	{
		if( nMsgType != CForm::mrYes ) 
		{
			if(_pCha)
			{
				_pCha->GetActor()->GetCurState()->PopState();
				_pCha = 0;
			}

			return;
		}

		stMsgBox* pMsgBox = CBoxMgr::ShowMsgBox(_evtMsgBox, g_oLangRec.GetString(943), false);	// "�� \"ȷ��\" ֹͣ���顣"
		pMsgBox->frmDialog->SetIsEscClose(false);

		CS_ReadBookStart();
	}


	void CReadBookMgr::_evtMsgBox(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
	{
		if(_pCha && _pCha->GetActor()->GetCurState())
		{
			_pCha->GetActor()->GetCurState()->PopState();
			_pCha = 0;
		}

		CS_ReadBookClose();
	}

}	// end of namespace GUI
