//----------------------------------------------------------------------
// :
// :lh 2004-07-08
// :,,
// :2004-10-09
//----------------------------------------------------------------------
#pragma once
#include <chrono>
#include "uiCompent.h"
#include "uiForm.h"

namespace GUI {
	class CTextButton;

	class CEdit : public CCompent {
	public:
		CEdit(CForm& frmOwn);
		CEdit(const CEdit& rhs);
		CEdit& operator=(const CEdit& rhs);
		virtual ~CEdit(void);
		GUI_CLONE(CEdit)

		virtual void Init();
		virtual void Refresh();
		virtual void SetAlpha(BYTE alpha);
		virtual void OnActive();
		virtual void OnLost();

		// 
		bool OnKeyDown(int key);
		bool OnChar(char c);

	public: // Get,Set
		static MPTexRect* GetCursorImage() {
			return &_CursorImage;
		}

		CGuiPic* GetImage() {
			return _pImage;
		}

		bool GetIsPassWord() {
			return _bIsPassWord;
		}

		bool GetIsParseText() {
			return _bParseText;
		}

		bool GetIsDigit() {
			return _bIsDigit;
		}

		bool GetIsMulti() {
			return _bIsMulti;
		}

		bool GetIsWrap() {
			return _bIsWrap;
		}

		void SetIsMulti(bool v) {
			_bIsMulti = v;
		}

		void SetIsPassWord(bool v) {
			_bIsPassWord = v;
		}

		void SetIsParseText(bool v) {
			_bParseText = v;
		}

		void SetIsDigit(bool v) {
			_bIsDigit = v;
		}

		void SetIsWrap(bool v) {
			_bIsWrap = v;
		}

		int GetFontHeight() {
			return _nFontHeight;
		}

		void SetFontHeight(bool v) {
			_nFontHeight = v;
		}

		int GetMaxNum() {
			return _nMaxNum;
		}

		void SetMaxNum(int v) {
			_nMaxNum = v;
		}

		int GetMaxVisible() {
			return _nMaxNumVisible;
		}

		void SetMaxNumVisible(int v) {
			_nMaxNumVisible = v;
		}

		int GetMaxLineNum() {
			return _nMaxLineNum;
		}

		void SetMaxLineNum(int v);
		void SetCaption(std::string_view str);

		const char* GetCaption() {
			return _str.c_str();
		}

		void ReplaceSel(std::string_view str, BOOL bCanUndo = TRUE);

		virtual void SetTextColor(DWORD color);

		DWORD GetTextColor() {
			return _color;
		}

		void SetCursorColor(DWORD v) {
			_nCursorColor = v;
		}

		void SetEnterButton(CTextButton* pButton) {
			_pEnterButton = pButton;
		}

	public:
		GuiEvent evtEnter; // 
		GuiKeyDownEvent evtKeyDown;
		GuiKeyCharEvent evtKeyChar;
		GuiEvent evtChange; // caption

	public:
		void Render();
		static bool InitCursor(const char* szFile);

		static int GetCursorX() {
			return _nCursorX;
		}

		static int GetCursorY() {
			return _nCursorY;
		}

		void RefreshText();
		void RefreshCursor();
		void ClearText();

	protected:
		bool _IsCursorInHZ(long l, char* s); // 
		void ShowFocus(); // 
		void CorrectCursor();

		// 
		void _Copy();
		void _Paste();

		void _Cut();
		void _Delete(); // 

		void _UpdataLines(); // 

		bool _isdigit(char c) {
			return (c >= '0' && c <= '9') || c == VK_BACK || c == VK_RETURN || c == VK_DELETE;
		}

	private:
		// void		_RefreshCursorPos();	// 
		void _GetCursorPos(int nCurPos);
		void _Copy(const CEdit& rhs);

	private:
		static MPTexRect _CursorImage; // 

		//  Caret blink: момент последнего переключения видимости (steady_clock).
		//  Раньше тут был frame counter (`++count; if count >= 10`) — на 30 FPS
		//  blink был ~333 мс, на 144 FPS уходил в ~69 мс (≈14 Гц, мерцание).
		//  Теперь по абсолютному времени, независимо от FPS.
		static std::chrono::steady_clock::time_point _lastCursorBlink;
		static bool _bCursorIsShow;
		static int _nCursorX, _nCursorY; // 

		CGuiPic* _pImage;

		DWORD _color;

		int _nLeftMargin;
		int _nTopMargin;

		bool _bParseText; // 

		CTextButton* _pEnterButton; // 

	protected:
		std::string _str;
		std::string _strVisible;

		int _nMaxNum; // 
		int _nMaxNumVisible; //

		bool _bIsPassWord; // 
		bool _bIsMulti; // 
		bool _bIsDigit; // 
		bool _bIsWrap;
		int _nOffset;

	protected: // 	
		int _nFontHeight; // 
		int _nMaxLineNum; // 

	protected: // 
		int _nCursorRow; // 
		int _nCursorCol; // ,0length
		int _nCursorFirstCol;
		int _nCursorSecondCol;

		DWORD _nCursorColor;
		DWORD _nCursorHeight;
	};

	// 
	inline void CEdit::SetMaxLineNum(int v) {
		if (v > 1) {
			_nMaxLineNum = v;
			SetIsMulti(true);
		}
	}
}
