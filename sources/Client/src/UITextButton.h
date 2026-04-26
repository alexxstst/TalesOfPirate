//----------------------------------------------------------------------
// :
// :lh 2004-07-19
// :
// :2004-10-09
//----------------------------------------------------------------------
#pragma once
#include "uicompent.h"
#include "UIForm.h"

namespace GUI {
	class CTextButton : public CCompent {
	public:
		enum eButtonState // 
		{
			csNormal = 0,
			csHover,
			csDown,
			csDisable,
			csEnd,
		};

	public:
		CTextButton(CForm& frmOwn);
		CTextButton(const CTextButton& rhs);
		CTextButton& operator=(const CTextButton& rhs);
		~CTextButton(void);
		GUI_CLONE(CTextButton)

		virtual void Render();
		virtual void Refresh();

		virtual void SetAlpha(BYTE alpha) {
			_pImage->SetAlpha(alpha);
		}

		virtual bool IsHandleMouse() {
			return true;
		}

		virtual bool MouseRun(int x, int y, DWORD key);
		virtual void FrameMove(DWORD dwTime);

		virtual bool IsFrameMove() {
			return true;
		}

		virtual CGuiPic* GetImage() {
			return _pImage;
		}

		const char* GetCaption() {
			return _strCaption.c_str();
		}

		void SetCaption(const char* str) {
			_strCaption = str;
		}

		virtual void SetIsEnabled(bool v);

		// csNormal,csHover,csDown,csDisablewh
		bool LoadImage(std::string_view file, int w = 32, int h = 32, int tx = 0, int ty = 0, bool isHorizontal = true);

		DWORD GetTextColor() {
			return _textColor;
		}

		void SetTextColor(DWORD color) {
			_textColor = color;
		}

		bool GetState() {
			return _isDown;
		}

		void SetFormModal(CForm::eModalResult v);

		CForm::eModalResult GetFormModal() {
			return _eFormModal;
		}

		void DoClick(eMouseState state = Mouse_LUp); // 

		//  0 
		void SetFlashCycle(DWORD dwCycle = 1000) {
			_dwFlashCycle = dwCycle;
		}

		DWORD GetFlashCycle() {
			return _dwFlashCycle;
		}

	public: // 
		GuiMouseEvent evtMouseClick; // 
		GuiMouseEvent evtMouseRClick; //   add by Philip.Wu 2006/01/23
		GuiMouseEvent evtMouseDBClick; //   add by Philip.Wu 2006/01/23
		GuiEvent evtMouseDownContinue; // 

	protected:
		void _SetState(eButtonState v);
		void _ClearOldState();

	protected:
		std::string _strCaption; // 
		DWORD _textColor; // 

		CGuiPic* _pImage; // ,bsNormal,bsHover,bsDown,bsDisable
		bool _isDown; // evtMouseClick

		CForm::eModalResult _eFormModal; // 

		DWORD _dwFlashCycle; // =0 
		DWORD _dwLastClick; // 

		static CTextButton* m_pCurButton;
	};

	inline void CTextButton::SetFormModal(CForm::eModalResult v) {
		if (v >= CForm::mrNone && v < CForm::mrEnd)
			_eFormModal = v;
	}
}
