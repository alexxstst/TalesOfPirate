// MP Console Quake
// 

// Created By Ryan Wang

#pragma once

typedef const char* (*CMD_FN)(const char *pszCmd);

class MPConsole
{

public:
    
    MPConsole();
    
    BOOL			OnCharEvent(char cChar, DWORD dwParam);
    BOOL			OnKeyDownEvent( int nKeyCode );
	
    void			Exec(const char *pszCmd);
    void			Show(BOOL bShow);
    BOOL			IsVisible();
    void			FrameMove();
    void			AddText(const char *pszText);
	void			AddHelp(const char *pszText);
    void			Clear();
	void			SetCmdFN(CMD_FN fn);
    int             GetHeight()           { return _nHeight;   }
	std::list<std::string>*	GetTextList() { return &_TextList; }
	const char*		GetInputText(); 

protected:

    void                    _AddText(const char* pszText, bool bCmd);
    
    char                    _szInput[128];
	char					_szInputTmp[128];
    
	std::list<std::string>            _CmdList;
	std::list<std::string>::iterator  _itCmd;
	std::list<std::string>            _TextList;

	std::list<std::string>            _HelpList;
    
    int                     _nInputCnt;
	BOOL					_bVisible;
    int                     _nWidth;
    int                     _nHeight;
    int                     _nMaxLine;
    DWORD                   _dwCursorTick;
    BOOL                    _bShowCursor;

	CMD_FN					_pfnCmd;
};

inline BOOL MPConsole::IsVisible()
{
    return _bVisible;
}

inline void MPConsole::AddText(const char *pszText)
{
    _AddText(pszText, false);
}

inline void MPConsole::AddHelp(const char *pszText)
{
	_HelpList.push_back(pszText);
}

inline void MPConsole::SetCmdFN(CMD_FN fn)
{
	_pfnCmd = fn;
}

inline const char*	MPConsole::GetInputText()
{ 
	return _szInputTmp;
} 

