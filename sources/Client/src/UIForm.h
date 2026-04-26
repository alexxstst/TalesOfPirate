//--------------------------------------------------------------
// :
// :lh 2004-07-08
// :
// :,
// :2004-10-09
//--------------------------------------------------------------
#pragma once
#include "uiguidata.h"

namespace GUI {
	typedef void (*GuiFormCloseEvent)(CForm* pForm, bool& IsClose);
	typedef void (*GuiFormBeforeShowEvent)(CForm* pForm, bool& IsShow);
	typedef void (*GuiFormEscCloseEvent)(CForm* pForm);

	// ,
	// pSender,nMsgTypeeModalResult
	typedef void (*FormMouseEvent)(CCompent* pSender, int nMsgType, int x, int y, DWORD dwKey);


	class CTextButton;
	class CMenu;

	class CForm : public CGuiData {
		friend class CCompent;
		friend class CContainer;
		friend class CFormMgr;

	public:
		enum eModalResult {
			mrNone,
			mrClose,
			mrYes,
			mrNo,
			mrOk,
			mrCancel,
			mrEnd,
		};

		enum eFormStyle //: ,,,,, ,,
		{
			fsNone,
			fsAllCenter,
			fsXCenter,
			fsYCenter,
			fsLeft,
			fsRight,
			fsTop,
			fsBottom,
			fsLeftTop,
			fsRightTop,
			fsLeftBottom,
			fsRightBottom,
		};

		CForm();
		CForm(const CForm& rhs);
		CForm& operator=(const CForm& rhs);
		~CForm();
		GUI_CLONE(CForm)

		virtual void Init();
		virtual void Render();
		virtual void Refresh();

		virtual bool MouseRun(int x, int y, DWORD key);

		virtual void SetIsShow(bool v) {
			if (v) Show();
			else Close();
		}

		virtual void SetParent(CGuiData* p);
		virtual void Reset();

		bool MenuMouseRun(int x, int y, DWORD key);

	public:
		void ForEach(CompentFun pFun); // FormCompentpFun
		void FrameMove(int nCount);

	public:
		void Show();
		void ShowModal();
		void Hide();
		void Close();
		CCompent* GetHitCommand(int x, int y);
		CGuiData* GetHintGui(int x, int y);
		void SetStyle(eFormStyle index, int offWidth = 0, int offHeight = 0, bool bRedraw = false);

		int GetFormStyle() {
			return _formStyle;
		}

		void SetIsEscClose(bool v) {
			_IsEscClose = v;
		}

		bool GetIsEscClose() {
			return _IsEscClose;
		}

		virtual CCompent* Find(const char*);
		virtual void SetAlpha(BYTE alpha);
		virtual void SetIsDrag(bool v);

		bool GetIsModal() {
			return _isModal;
		}

		eModalResult GetModalResult() {
			return _modalResult;
		}

		void SetModalResult(eModalResult v) {
			_modalResult = v;
		}

		static CForm* GetActive() {
			return _pActive;
		}

		CFramePic* GetFrameImage() {
			return _pImage;
		}

		void SetHotKey(char key) {
			_cHotKey = key;
		}

		char GetHotKey() {
			return _cHotKey;
		}

		void SetHotKeyHandler(GuiHotKeyEvent handler) {
			evtHotkeyHandler = handler;
		}

		bool SetNextActiveCompent(bool isNext = true); // ,isNext=false

		void SetActiveCompent(CCompent* pActive);

		// 
		void PopMenu(CMenu* pMenu, int x = 0, int y = 0);

		void SetPointer(void* v) {
			_pPointer = v;
		}

		void* GetPointer() {
			return _pPointer;
		}

		void SetEnterButton(CTextButton* v) {
			_pEnterButton = v;
		}

	public:
		bool OnChar(char key);
		bool OnKeyDown(int key);
		void OnSetScreen();

		CCompent* FindActiveCompent();

		int ClearChild(); // ,

	public: // 
		GuiEvent evtShow; // 
		GuiEvent evtHide; // 
		GuiEvent evtActive; // 
		GuiEvent evtLost; // 
		GuiMouseEvent evtMouseDown; // 
		GuiKeyDownEvent evtKeyDown;
		GuiKeyCharEvent evtKeyChar;
		GuiEvent evtOnSetScreen;
		GuiMouseEvent evtMouseDragEnd; // 
		GuiHotKeyEvent evtHotkeyHandler; // truefalse

		GuiFormEscCloseEvent evtEscClose;
		GuiFormBeforeShowEvent evtBeforeShow;
		GuiFormCloseEvent evtClose; // 

	public: // 
		FormMouseEvent evtEntrustMouseEvent;

	protected:
		void _OnActive();
		bool _AddCompent(CCompent* c, bool isCheck);
		bool _AddFrameMove(CCompent* c, bool isCheck);

		void _TempleteInit(); // 
		void _TempleteClear(); // 

	protected:
		void* _pPointer;

		typedef std::vector<CCompent*> vcs;
		vcs _compents; // ,
		vcs _mouse; // 
		vcs _actives; // 
		vcs _allCompents; // 
		vcs _frames;

		typedef std::list<CForm*> vfrm;
		vfrm _childs; // 

		int _nActiveCompentID; // Compent

		bool _isModal; // 

		static CForm* _pActive;

		eModalResult _modalResult; // 
		eFormStyle _formStyle;

		bool _IsEscClose; // true,esc

		CFramePic* _pImage; // ,
		CTextButton* _pEnterButton;

	protected:
		char _cHotKey; // 
		CMenu* _pPopMenu; // 

	private:
		void _Copy(const CForm* rhs);
		void _CopyCompent(const CForm* rhs);
		void _ActiveForm(DWORD key);

	private:
		static void _DragMouseEvent(CGuiData* pSender, int x, int y, DWORD key);
		static void _DragMouseEventBegin(CGuiData* pSender, int x, int y, DWORD key);
		static int _nDragOffX, _nDragOffY;
		int _nOffWidth; //-Added by Arcol
		int _nOffHeight; //-Added by Arcol
	};

	inline void CForm::_ActiveForm(DWORD key) {
		if ((key & Mouse_Down) && !GetIsModal() && _pActive != this) {
			_OnActive();
		}
	}

	inline void CForm::Close() {
		_modalResult = mrClose;
		Hide();
	}
}
