#ifndef UI_MAKE_EQUIP_FORM_H
#define UI_MAKE_EQUIP_FORM_H

#include "UIGlobalVar.h"
#include <vector>

class CItemRecord;

namespace GUI {
	class CLabel;

	class CMakeEquipMgr : public CUIInterface {
	public:
		CMakeEquipMgr() {
		}

		virtual ~CMakeEquipMgr() {
		}

		void ShowMakeEquipForm(bool bShow = true);
		void SendMakeEquipProtocol();
		void ShowConfirmDialog(long lMoney);

		void Clear();

		bool IsRouleauCommand(COneCommand* oneCommand);
		bool IsAllCommand(COneCommand* oneCommand);
		int GetItemComIndex(COneCommand* oneCommand);

		void DragRouleauToEquipGrid();
		void DragItemToEquipGrid(int iIndex);

		void MakeEquipSuccess(long lChaID);
		void MakeEquipFailed(long lChaID);
		void MakeEquipOther(long lChaID);

		void SetType(int type) {
			m_iType = type;
		}

		int GetType() const {
			return m_iType;
		}

		static const int MAKE_EQUIP_TYPE = 2; //
		static const int EQUIP_FUSION_TYPE = 4; //
		static const int EQUIP_UPGRADE_TYPE = 5; //
		static const int ELF_SHIFT_TYPE = 6; //


		void PushEquipUpgradeItem(int iIndex, CItemCommand& rItem);
		void PushRouleau(CItemCommand& rItem);
		void SetMakeEquipUI();
		void DragEvtEquipItem(int index, CGuiData* pSender, CCommandObj* pItem, bool& isAccept);
		void PushItem(int iIndex, CItemCommand& rItem, int iItemNum);

	protected: /*@See CUIInterface Methods */
		virtual bool Init();
		virtual void CloseForm();
		virtual void SwitchMap();

	private: /*Call back Functions*/
		static void _MainMouseEvent(CCompent* pSender, int nMsgType, int x, int y, DWORD dwKey);
		static void _DragEvtRouleau(CGuiData* pSender, CCommandObj* pItem,bool& isAccept);
		static void _DragEvtEquipItem0(CGuiData* pSender, CCommandObj* pItem,bool& isAccept);
		static void _DragEvtEquipItem1(CGuiData* pSender, CCommandObj* pItem,bool& isAccept);
		static void _DragEvtEquipItem2(CGuiData* pSender, CCommandObj* pItem,bool& isAccept);
		static void _DragEvtEquipItem3(CGuiData* pSender, CCommandObj* pItem,bool& isAccept);
		static void _OnClose(CForm* pForm, bool& IsClose);
		static void _evtConfirmEvent(CCompent* pSender, int nMsgType, int x, int y, DWORD dwKey);
		static void _evtConfirmCancelEvent(CForm* pForm);
		static void _evtFusionNoCatalyzerConfirmEvent(CCompent* pSender, int nMsgType, int x, int y, DWORD dwKey);

	private: /*Help Functions:*/

		void ClearEquips();
		void ClearEquipList(int iIndex);

		/*
		 *	Help Function:
		 */
		//void		DragEvtEquipItem(int index, CGuiData *pSender, CCommandObj* pItem, bool& isAccept);

		/*
		 *	
		 */
		bool IsMakeGem();
		bool CanPushStone(int iIndex, CItemCommand& rItem);
		bool CanPushEquip(int iIndex, CItemCommand& rItem);

		//void		SetMakeEquipUI();

		/*
		 *	
		 */
		//void		PushRouleau(CItemCommand& rItem);
		void PopRouleau();
		void PushNewGems();
		void PushNewEquips(CItemRecord& rRouleauRecord);

		/*
		 *	
		 */
		void PushGemItem(int iIndex, CItemCommand& rItem);
		void PopGemItem(int iIndex);

		/*
		 *	
		 */
		void PushEquipItem(int iIndex, CItemCommand& rItem);
		void PopEquipItem(int iIndex);

		/*
		 *	
		 */
		void PushEquipFusionItem(int iIndex, CItemCommand& rItem);
		void PopEquipFusionItem(int iIndex);

		/*
		 *	
		 */
		//void		PushEquipUpgradeItem(int iIndex, CItemCommand& rItem);
		void PopEquipUpgradeItem(int iIndex);

		/*
		 *	
		 */
		void PushLastEquip(CItemCommand& rItem);
		void PopLastEquip();

		/*
		 *  
		 */
		void PushElfShiftItem(int iIndex, CItemCommand& rItem);
		void PopElfShiftItem(int iIndex);

		/*
		 *	
		 */
		//void		PushItem(int iIndex, CItemCommand& rItem, int iItemNum);
		void PopItem(int iIndex);

		long CalMakeEquipMoney();

		bool IsEquipMakeRouleau(CItemCommand& rItem);

		bool IsEquipFusionRouleau(CItemCommand& rItem);
		bool IsAppearanceEquip(CItemCommand& rItem);
		bool IsEquipFusionCatalyzer(CItemCommand& rItem);
		bool IsSameAppearEquip(CItemCommand& rEquipItem, CItemCommand& rAppearItem);

		bool IsEquipUpgradeRouleau(CItemCommand& rItem);
		bool IsEquipUpgradeSpar(CItemCommand& rItem);
		bool IsFusionEquip(CItemCommand& rItem);

		bool IsElfShiftStone(CItemCommand& rItem); // 
		bool IsElfShiftItem(CItemCommand& rItem); // 


	private:
		static const int STONE_ITEM_NUM = 2; //
		static const int ITEM_NUM = 4; //
		static const int FUSION_NUM = 3; //Item
		static const int UPGRADE_NUM = 2; //Item
		static const int SHIFT_NUM = 2; //Item

		static const int GEM_ROULEAU_TYPE = 47; //
		static const int EQUIP_ROULEAU_TYPE = 48; //D
		static const int GEN_STONE_TYPE = 49;
		static const int FORGE_STONE_TYPE = 50;
		static const int EQUIP_FUSION_ROULEAU_TYPE = 60;
		static const int EQUIP_FUSION_CATALYZER_TYPE = 61;
		static const int APPEAR_EQUIP_BASE_ID = 5000;
		static const int EQUIP_UPGRADE_ROULEAU_TYPE = 62;
		static const int EQUIP_UPGRADE_SPAR = 63;
		static const int FORGE_SUCCESS_EFF_ID = 345;
		static const int FORGE_FAILED_EFF_ID = 346;


		static const long MAKE_EQUIP_MONEY = 50000;
		static const long EQUIP_FUSION_MONEY = 1000; // 
		static const long EQUIP_UPGRADE_MONEY = 10000;

		CForm* frmMakeEquip;

		COneCommand* cmdRouleau;
		COneCommand* cmdLastEquip;
		COneCommand* cmdItem[ITEM_NUM];
		CLabel* labForgeGold;
		CMemo* memForgeItemState;
		CTextButton* btnYes;

		struct EquipInfo {
			int iPos; // 
			int iNum; // 
		};

		typedef std::vector<EquipInfo*> EquipList;
		typedef EquipList::iterator EquipListIter;
		EquipList equipItems[ITEM_NUM];

		int m_iRouleauPos;

		bool m_isOldEquipFormShow;

		int m_iType;
	}; // end of CMakeEquipMgr class
} // end of GUI namespace

#endif	// end of UI_MAKE_EQUIP_FORM_H
