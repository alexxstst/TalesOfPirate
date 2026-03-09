
#pragma once


#include "UIFormMgr.h"
#include "UIForm.h"
#include "UIEdit.h"
#include "UILabel.h"
#include "UITextButton.h"

#include "UIGlobalVar.h"
#include "NetProtocol.h"

#include "LoginScene.h"
#include "SelectChaScene.h"


namespace GUI
{
	class CForm;
	class CEdit;
	class CTextButton;

	// 
	class CDoublePwdMgr : public CUIInterface	
	{
	public:

		CDoublePwdMgr();

		// 
		void ShowCreateForm();

		// 
		void ShowAlterForm();

		// 
		void ShowDoublePwdForm();

		// 
		void CloseAllForm();

		// 
		bool GetIsShowCreateForm()    { return frmDoublePwdCreate->GetIsShow(); }
		bool GetIsShowAlterForm()     { return frmDoublePwdAlter->GetIsShow();  }
		bool GetIsShowDoublePwdForm() { return frmDoublePwd->GetIsShow();       }

		// 
		void SetType(int nType) { m_nType = nType; }
		int  GetType()          { return  m_nType; }

		void SetLockGridID(int id) { lockGridID = id; }

		static const int MC_REQUEST	= -1;
		static const int DELETE_CHARACTOR	= 1;	// 
		static const int PACKAGE_UNLOCK		= 2;	// 
		static const int STORE_OPEN_ASK		= 3;	// 
		static const int ITEM_UNLOCK		= 4;	//   ning.yan 2008-11-10
		static const int MULTI_ITEM_UNLOCK = 5;
		static const int SHOW_EXCHANGEFORM	= 5;

	protected:

		virtual bool Init();
		virtual void CloseForm();

		// 
		void ShowDoublePwdKeyboardForm();

		// 
		void SetFocusEditBox(CEdit* edt) { edtFocusEditBox = edt; }

		// 
		void SendDeleteCharactor();

		// 
		void SendPackageUnlock();

		// 
		void SendItemUnlock(); //add by ning.yan 2008-11-11

		void SendUnlockSelectedItems();
		// 
		void SendPackageStoreOpen();

		void SendGameRequest();

		// 
		void RandomInputButton();

	private:

		// 
		CForm*      frmDoublePwdCreate;
		CEdit*		edtDoublePwdCreate;
		CEdit*      edtDoublePwdCreateRetry;

		// 
		CForm*      frmDoublePwdAlter;
		CEdit*		edtDoublePwdAlterOld;
		CEdit*		edtDoublePwdAlterNew;
		CEdit*		edtDoublePwdAlterNewRetry;

		// 
		CForm*      frmDoublePwd;
		CEdit*      edtDoublePwd;

		// 
		CForm*      frmDoublePwdInput;

		// 
		CForm*		frmDoublePwdInfo;

		// 
		CEdit*		edtFocusEditBox;

		// 
		int         m_nType;

		int lockGridID; //TODO: No default value

		// 
		static void _evtCreateFromMouseEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);

		// 
		static void _evtAlterFromMouseEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);

		// 
		static void _evtDoublePwdFromMouseEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);

		// 
		static void _evtInputFromMouseEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);

		// 
		static void _evtEditFocus(CGuiData* pSender);

		// 
		static void _evtFormClose(CForm* pForm, bool& IsClose );

		// 
		static bool IsPwdValid(const char* szStr);

	}; // end of class CDoublePwdMgr

} // end of namespace GUI


