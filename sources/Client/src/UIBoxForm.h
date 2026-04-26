#pragma once
#include "UIGlobalVar.h"
#include "uicloneform.h"
#include "uiform.h"

namespace GUI {
	struct stTradeBox {
		stTradeBox();
		void Refresh();
		int GetTradeNum();

		CForm* frmDialog;
		CLabel* labPrice;
		CEdit* edtNumber;
		CLabel* labName;

		// 
		float fUnitMoney;
		int nTotalNum;
	};

	struct stSelectBox {
		stSelectBox();

		CForm* frmDialog;
		CLabel* labInfo;

		// 
		DWORD dwTag;
		DWORD dwParam;
		void* pointer;
	};

	struct stNumBox {
		stNumBox();
		void Refresh();
		int GetNumber();
		void GetString(const char*& out);

		CForm* frmDialog;
		CEdit* edtNumber;
		CLabel* labInfo;

		// 
		int nTotalNum;
		void* pointer;
	};

	struct stPasswordBox {
		stPasswordBox() : frmDialog(NULL), edtPassword(NULL), labInfo(NULL) {
		}

		CForm* frmDialog;
		CEdit* edtPassword;
		CLabel* labInfo;
	};

	struct stMsgBox {
		CForm* frmDialog;
		CLabel* labInfo;

		// 
	};

	struct stMsgTimeBox {
		CForm* frmDialog;
		CLabel* labInfo;

		// 
		short teamID;
	};

	//
	class CBoxMgr : public CUIInterface {
	public:
		//
		static stTradeBox* ShowTradeBox(FormMouseEvent evtForm = NULL, float fUnitMoney = 0, int nTotalNum = -1,
										std::string_view szName = {});

		//
		static stSelectBox* ShowSelectBox(FormMouseEvent evtForm = NULL, std::string_view szTitle = {},
										  bool bModal = false);

		//
		static stNumBox* ShowNumberBox(FormMouseEvent evtForm = NULL, int nTotalNum = -1,
									   std::string_view szTitle = {},
									   bool IsMax = true, bool bModal = true);

		//
		static stPasswordBox* ShowPasswordBox(FormMouseEvent evtForm = NULL, std::string_view szTitle = {},
											  std::string_view szPassword = {}, bool bModal = true);

		//
		static stMsgBox* ShowMsgBox(FormMouseEvent evtForm = NULL, std::string_view szTitle = {}, bool bModal = true);

		static stMsgTimeBox* ShowMsgTime(FormMouseEvent evtForm = NULL, std::string_view szTitle = {},
										 int iSeconds = 60);

		static void CloseAllBox();

	protected:
		virtual bool Init();
		virtual void End();
		virtual void FrameMove(DWORD dwTime);

	private:
		static void _ClearBox(CHideForm& list);

		CForm* _FindForm(std::string_view frmName);

	private:
		// :,
		static CHideForm _cTrade;

		// ,
		static CHideForm _cSelect;

		// 
		static CHideForm _cNumber;

		// 
		static CHideForm _cPassword;

		// 
		static CHideForm _cMsg;

		// Tick
		static CForm* _cfrmError;
		static DWORD _cTick;
		static int _cSeconds;
		static bool _cStart;
	};
}
