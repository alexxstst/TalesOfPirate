#pragma once

#include "uiglobalvar.h"
#include "uiform.h"
#include "uiformmgr.h"
#include "uilabel.h"
#include "uicommand.h"
#include "uifastcommand.h"


namespace GUI {
	class CPurifyMgr : public CUIInterface {
	public:
		CPurifyMgr(void);
		~CPurifyMgr(void);

		void ShowForm(int nType);
		void CloseAllForm();

		void ClearAllCommand();

		int GetItemComIndex(COneCommand* pCom);
		void DragItemToEquipGrid(int nIndex);

		static const int NO_USE = -1;
		static const int TYPE_COUNT = 4; // 

		static const int PURIFY_TYPE = 7; // 
		static const int ENERGY_TYPE = 8; // 
		static const int GETSTONE_TYPE = 9; // 
		static const int REPAIR_OVEN_TYPE = 10; // 

		static const int PURIFY_CELL_COUNT = 2; // 
		static const int PURIFY_ONE = 0; // 1
		static const int PURIFY_TWO = 1; // 2

		int GetType() {
			return m_nType;
		} // 

	protected:
		virtual bool Init();
		virtual void CloseForm();

	private:
		CForm* frmEquipPurify;
		CLabelEx* labMoneyShow;
		COneCommand* cmdEquipPurify[PURIFY_CELL_COUNT];
		CTextButton* btnForgeYes;

		CLabelEx* labHintleft[TYPE_COUNT];
		CLabelEx* labHintright[TYPE_COUNT];
		CLabelEx* labTitle[TYPE_COUNT];

		int m_nType;
		int m_iPurifyItemPos[PURIFY_CELL_COUNT];

		bool IsEquipItem(CItemCommand& rItem);
		bool IsMainLifeItem(CItemCommand& rItem);
		bool IsRepairLifeItem(CItemCommand& rItem);

		void PushItem(int iIndex, CItemCommand& rItem);
		void PopItem(int iIndex);
		void SetPurifyUI();
		void SendNetProtocol();


		static void _evtDragPurifyOne(CGuiData* pSender, CCommandObj* pItem,bool& isAccept); // 
		static void _evtDragPurifyTwo(CGuiData* pSender, CCommandObj* pItem,bool& isAccept); // 

		static void _evtMainMouseButton(CCompent* pSender, int nMsgType, int x, int y, DWORD dwKey); // 
		static void _evtClosePurifyForm(CForm* pForm, bool& IsClose); // 
	};
}
