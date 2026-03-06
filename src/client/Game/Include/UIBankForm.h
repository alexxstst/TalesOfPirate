#pragma once

//--------------------------------------------------------------
// :
// :
//--------------------------------------------------------------

#ifndef UI_BANK_FORM_H
#define UI_BANK_FORM_H

#include "UIGlobalVar.h"
#include "NetProtocol.h"

namespace GUI
{
	class CForm;
	class CGoodsGrid;
	class CLabel;

	struct stNumBox;
     
	class CBankMgr : public CUIInterface
	{
	public:
		void		ShowBank(); // 
		CGoodsGrid* GetBankGoodsGrid() { return grdBank; } // 

		bool		PushToBank(CGoodsGrid& rkDrag, CGoodsGrid& rkSelf, int nGridID, CCommandObj& rkItem); // 
		bool		PopFromBank(CGoodsGrid& rkDrag, CGoodsGrid& rkSelf, int nGridID, CCommandObj& rkItem); //
		bool		BankToBank(CGoodsGrid& rkDrag, CGoodsGrid& rkSelf, int nGridID, CCommandObj& rkItem);  //

	protected:
		virtual bool Init(); // 
		virtual void CloseForm(); // 

	private:
		static void _MoveItemsEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);  // 
		static void _MoveAItemEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);  //
		static void _evtBankToBank(CGuiData *pSender,int nFirst, int nSecond, bool& isSwap);  //
		static void _evtOnClose( CForm* pForm, bool& IsClose );//

	private:
		stNumBox*		m_pkNumberBox; // 
		stNetBank		m_kNetBank;

		// 
		CForm*			frmBank; // 
		CGoodsGrid*		grdBank; // 
		CLabel*			labCharName;

	}; // end of class CBankMgr

} // end of namespace GUI

#endif // end of UI_BANK_FORM_H
