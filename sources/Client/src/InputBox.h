// CInputBox — обёртка над скрытым Windows Edit control, используемая
// формой чата (и другими CEdit'ами) для ввода текста.
//
// Миграция на UTF-8 (этап A плана):
//   Edit Windows control хранит текст в Unicode независимо от того, был ли
//   он создан через CreateWindow(A) или (W). Мы используем GetWindowTextW /
//   SetWindowTextW и конвертируем wide ↔ UTF-8 на границе — буфер _szText
//   теперь хранит UTF-8 байты, которые напрямую идут в ui::Render (шрифты
//   настроены на CP_UTF8) и в чат-пакеты.
//
// WM_CHAR от главного окна пересылается в Edit через SendMessage без
// конвертации — Edit сам распакует ANSI-байт в Unicode согласно system ACP.
#pragma once

#include "EncodingUtil.h"

class CInputBox {
public:
	CInputBox()
		: _bAccountMode(FALSE), _nCursorPos(0), _hEdit(0) {
		_szText[0] = '\0';
	}

	HWND GetEditWindow() {
		return _hEdit;
	}

	void SetEditWindow(HWND hEdit) {
		_hEdit = hEdit;
	}

	void HandleWindowMsg(DWORD dwMsgType, DWORD dwParam1, DWORD dwParam2);

	void SetAccountMode(BOOL bAccount);
	void SetPasswordMode(BOOL bPassword);
	void SetDigitalMode(BOOL bDigitalMode);
	void SetMaxNum(int nMaxNum);
	void SetMultiMode(BOOL bMultiLine);
	void SetSel(int nStartChar, int nEndChar);

	void SetCursorTail();
	char* RefreshText();
	int RefreshCursor();
	void ClearText();
	char* SetText(const char* pszText);
	void ReplaceSel(const char* pszRplText, BOOL bCanUndo = TRUE);

protected:
	char _szText[1024]; // UTF-8
	int _nCursorPos;
	BOOL _bAccountMode;
	HWND _hEdit;
};


inline void CInputBox::SetMaxNum(int nMaxNum) {
	::SendMessage(_hEdit, EM_LIMITTEXT, nMaxNum, 0);
}

inline char* CInputBox::RefreshText() {
	// UTF-16 из Edit → UTF-8 в _szText. Длина wide в символах, буфер
	// берём с запасом (_szText/1024 byte достаточно для 256 BMP-символов
	// в UTF-8 ≈ 768 байт + NUL).
	wchar_t wbuf[512] = {0};
	const int wlen = ::GetWindowTextW(_hEdit, wbuf, static_cast<int>(std::size(wbuf)));
	const std::string utf8 = encoding::WideToUtf8(std::wstring_view(wbuf, static_cast<std::size_t>(wlen)));
	const std::size_t n = utf8.size() < sizeof(_szText) - 1 ? utf8.size() : sizeof(_szText) - 1;
	std::memcpy(_szText, utf8.data(), n);
	_szText[n] = '\0';
	return _szText;
}

inline char* CInputBox::SetText(const char* pszText) {
	if (!pszText) pszText = "";
	const std::wstring wide = encoding::Utf8ToWide(pszText);
	::SetWindowTextW(_hEdit, wide.c_str());
	// Синхронизировать кеш _szText с входящей UTF-8 строкой.
	const std::size_t n = std::strlen(pszText);
	const std::size_t k = n < sizeof(_szText) - 1 ? n : sizeof(_szText) - 1;
	std::memcpy(_szText, pszText, k);
	_szText[k] = '\0';
	return _szText;
}

inline void CInputBox::ClearText() {
	_szText[0] = '\0';
	::SetWindowTextW(_hEdit, L"");
}

inline int CInputBox::RefreshCursor() {
	::SendMessage(_hEdit, EM_GETSEL, (WPARAM)&_nCursorPos, 0);
	return _nCursorPos;
}

inline void CInputBox::SetCursorTail() {
	// EM_SETSEL принимает wide-индексы (количество codepoint/wchar).
	wchar_t wbuf[512] = {0};
	const int wlen = ::GetWindowTextW(_hEdit, wbuf, static_cast<int>(std::size(wbuf)));
	::SendMessage(_hEdit, EM_SETSEL, wlen, wlen);
}

inline void CInputBox::SetSel(int nStartChar, int nEndChar) {
	::SendMessage(_hEdit, EM_SETSEL, nStartChar, nEndChar);
}

inline void CInputBox::HandleWindowMsg(DWORD dwMsgType, DWORD dwParam1, DWORD dwParam2) {
	if (dwMsgType == WM_KEYDOWN || dwMsgType == WM_KEYUP) {
		if (dwParam1 == VK_UP) return;
	}
	else if (dwMsgType == WM_CHAR) {
		if (_bAccountMode) {
			if (dwParam1 == ' ' || dwParam1 == ',') return;
		}
	}
	// Edit Windows control — окно создано через CreateWindow(A), но сам
	// хранит текст в Unicode. WM_CHAR байт в ANSI распакуется в Unicode
	// согласно system ACP (CP1251 для русской локали).
	::SendMessage(_hEdit, dwMsgType, dwParam1, dwParam2);
}

inline void CInputBox::ReplaceSel(const char* pszRplText, BOOL bCanUndo) {
	if (!pszRplText) pszRplText = "";
	const std::wstring wide = encoding::Utf8ToWide(pszRplText);
	::SendMessageW(_hEdit, EM_REPLACESEL, (WPARAM)bCanUndo, (LPARAM)wide.c_str());
}

extern CInputBox g_InputBox;
