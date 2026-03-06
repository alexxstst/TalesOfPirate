#pragma once


// Window Edit
// windows,,,,,
class CInputBox 
{

public:


	CInputBox()
	:_bAccountMode(FALSE), _hEdit(0)
	{

	}

	HWND    GetEditWindow()						{ return _hEdit;  }
	void	SetEditWindow(HWND hEdit)			{ _hEdit = hEdit; }
	void	HandleWindowMsg(DWORD dwMsgType, DWORD dwParam1, DWORD dwParam2);
	
	
	void	SetAccountMode(BOOL bAccount);		// , , 
	void	SetPasswordMode(BOOL bPassword);	// , *
	void	SetDigitalMode(BOOL bDigitalMode);	// , 
	void	SetMaxNum(int nMaxNum);				// 
	void	SetMultiMode(BOOL bMultiLine);		// 	
	void	SetSel(int nStartChar, int nEndChar);


	void	SetCursorTail();					// 
	char*	RefreshText();
	int		RefreshCursor();
	void	ClearText();
	char*	SetText(const std::string& pszText);
	void	ReplaceSel(const std::string& pszRplText, BOOL bCanUndo = TRUE);	// - added by Arcol

protected:

	char	_szText[1024]{};	//
	int		_nCursorPos;	// 
	BOOL	_bAccountMode;	// 
	HWND	_hEdit;
};


inline void CInputBox::SetMaxNum(int nMaxNum)
{
	::SendMessage(_hEdit, EM_LIMITTEXT, nMaxNum, 0);
}

inline char* CInputBox::RefreshText()
{
	GetWindowText(_hEdit, _szText, 1000);
	return _szText;
}

inline char* CInputBox::SetText(const std::string& pszText)
{
	SetWindowText(_hEdit, pszText.c_str());
	strcpy(_szText, pszText.c_str());
	return _szText;
}

inline void CInputBox::ClearText()
{
	strcpy(_szText, "");
	SetWindowText(_hEdit, "");
}

inline int CInputBox::RefreshCursor()
{
	::SendMessage(_hEdit, EM_GETSEL, (WPARAM)&_nCursorPos, 0);
	return _nCursorPos;
}

inline void CInputBox::SetCursorTail()
{
	int nLen = (int)(strlen(_szText));
	::SendMessage(_hEdit, EM_SETSEL, nLen, nLen);
}

inline void CInputBox::SetSel(int nStartChar, int nEndChar)
{
	::SendMessage(_hEdit, EM_SETSEL, nStartChar, nEndChar);
}

inline void CInputBox::HandleWindowMsg(DWORD dwMsgType, DWORD dwParam1, DWORD dwParam2)
{
	// windowsedit
	// if(!(dwMsgType==WM_KEYDOWN || dwMsgType==WM_CHAR)) return;

	// if(dwMsgType==WM_IME_NOTIFY || dwMsgType==WM_IME_STARTCOMPOSITION) return;

	if(dwMsgType==WM_KEYDOWN || dwMsgType==WM_KEYUP)
	{
		if(dwParam1==VK_UP) return; // UP, Edit
		// ::SendMessage(_hEdit, WM_KEYDOWN, dwParam1, dwParam2);
	}
	else if(dwMsgType==WM_CHAR)
	{
		if(_bAccountMode)
		{
			if(dwParam1==' ' || dwParam1==',') return;
		}
	}
	::SendMessage(_hEdit, dwMsgType, dwParam1, dwParam2);
}

inline void CInputBox::ReplaceSel(const std::string&pszRplText, BOOL bCanUndo)
{
	::SendMessage(_hEdit, EM_REPLACESEL, (WPARAM)bCanUndo, (LPARAM)(LPCTSTR)pszRplText.c_str());
}

extern CInputBox g_InputBox;
