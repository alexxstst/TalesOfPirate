//----------------------------------------------------------------------
// CImeInput — оверлей IME (composition + candidate list) поверх UI.
//
// Миграция на UTF-8 (этап B плана "везде CP_UTF8"):
//   - Используются W-версии Windows IMM API (ImmGetCompositionStringW,
//     ImmGetCandidateListW, ImmGetDescriptionW) — они возвращают UTF-16
//     независимо от системной ANSI codepage.
//   - Отображаемые буферы хранятся как `std::string` в UTF-8 (результат
//     encoding::WideToUtf8). Ui::Render принимает UTF-8 (после снятия
//     костыля SetAllCodepage(CP_ACP) в этапе E).
//
// :lh 2004-07-28
//----------------------------------------------------------------------
#pragma once
#include "uiGuidata.h"

#include <string>

namespace GUI {
	class CImeInput {
		static const int InputX = 120; //
		static const int InputY = 170; //

	private:
		CImeInput& operator=(const CImeInput& rhs);
		CImeInput(const CImeInput& rhs);

		virtual bool Clone(CGuiData* rhs) {
			return false;
		}

		virtual CGuiData* Clone() {
			return NULL;
		}

		virtual void SetAlpha(BYTE alpha);

	public:
		CImeInput(void);
		~CImeInput(void);

		static CImeInput s_Ime;

		void Init();
		void Clear();
		bool HandleWindowMsg(DWORD dwMsg, WPARAM wParam, LPARAM lParam);
		void Render();
		void SetShowPos(int x, int y);
		void SetScreen(bool isFull, int w, int h);
		void OnCreate(HWND hWnd, HINSTANCE hInst);

		void SetSize(int w, int h) {
			_nWidth = w, _nHeight = h;
		}

		int GetWidth() {
			return _nWidth;
		}

		int GetHeight() {
			return _nHeight;
		}

	private:
		// Получить composition-строку (GCS_COMPSTR/GCS_RESULTSTR) через W-API,
		// сохранить как UTF-8 в out.
		bool _GetCompositionStringUtf8(std::string& out, DWORD ImeValue);
		bool _GetCandidateList();
		bool _GetConversion();

	private:
		CGuiPic* _pImage;
		DWORD _ImmNameColor;
		int _nShowX;
		int _nShowY;
		bool _bIsShow;

	private:
		bool _bIsFull;
		int _nScreenWidth;
		int _nScreenHeight;
		unsigned long _lConversion;

	private:
		// Все отображаемые буферы — UTF-8.
		std::string _immName; // "[EN]", "[RU]", и т.п.
		std::string _composition; // активная composition-строка
		std::string _candidate; // "1:选项 2:选项 ..."
		std::string _inputMode; // индикатор IME input mode
		std::string _sbcMode; // Shift/DBC mode
		std::string _interpunctionMode; // pinyin punctuation mode

		HIMC _hImc;
		HIMC _oldImc;
		LPCANDIDATELIST _pList;

		int _nWidth, _nHeight;
	};

	inline void CImeInput::OnCreate(HWND hWnd, HINSTANCE hInst) {
		HIMC hImc = ::ImmCreateContext();
		//::ImmAssociateContextEx( hWnd, hImc, IACE_IGNORENOCONTEXT );
	}
}
