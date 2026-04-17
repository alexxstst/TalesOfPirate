#include "StdAfx.h"
#include "MPConsole.h"
#include "MPRender.h"

using namespace std;

MPConsole::MPConsole()
	: _pfnCmd(NULL), _dwCursorTick(0), _bVisible(FALSE) {
	strcpy(_szInput, "");
	strcpy(_szInputTmp, "");
	_nHeight = 140;
	_nWidth = 450;
	_nMaxLine = (_nHeight - 18) / 16;

	for (int i = 0; i < _nMaxLine; i++) {
		_TextList.push_back("");
	}

	AddText("Welcome To MindPower3D Engine");

	_itCmd = _CmdList.begin();

	for (int y = 0; y < 13; y++) {
		g_Render.Print(INFO_CMD, _nWidth + 2, y * 12, "|");
	}
	g_Render.Print(INFO_CMD, 0, _nHeight + 4,
				   "____________________________________________________________________________");
	// g_Render.Print(INFO_CMD, 3, _nHeight - 2, "]");
	// g_Render.Print(INFO_CMD, _nWidth - 98, _nHeight + 14, "MindPower Engine Console");
}

BOOL MPConsole::OnKeyDownEvent(int nKeyCode) {
	if (!IsVisible()) return FALSE;

	switch (nKeyCode) {
	case VK_UP: {
		if (_CmdList.size() == 0) break;
		_itCmd--;
		strcpy(_szInput, (*_itCmd).c_str());
		if (_itCmd == _CmdList.begin()) {
			_itCmd = _CmdList.end();
		}
		if (_bShowCursor) sprintf(_szInputTmp, "]%s_", _szInput);
		else sprintf(_szInputTmp, "]%s", _szInput);
		break;
	}
	}
	return TRUE;
}


BOOL MPConsole::OnCharEvent(TCHAR iChar, DWORD dwParam) {
#ifdef DEBUG
	if (iChar == '`') {
		Show(!IsVisible());
		return TRUE;
	}
#endif
	if (!IsVisible()) return FALSE;

	int nLen = (int)(strlen(_szInput));
	if (iChar == '\r') {
		if (nLen == 0) return TRUE;

		string str = ">";
		str += _szInput;
		_AddText(str.c_str(), false);

		if (_pfnCmd) {
			_AddText(_pfnCmd(_szInput), false);
		}

		if (strcmp(_szInput, "programmer") == 0) {
			_AddText("                MindPower3D Stars                       ", false);
			_AddText("                                                        ", false);
			_AddText("ModelSystem & ResourceManage & Lighting ....... JackLi", false);
			_AddText("EffectSystem & Camera & Font .................. LemonWang", false);
			_AddText("TerrainSystem & SceneEditor ................... RyanWang", false);
			_AddText("                                                        ", false);
			_AddText("Project Leader ................................ RyanWang", false);
		}

		_AddText(_szInput, true);

		strcpy(_szInput, "");
	}
	else if (iChar == '\t') {
	}
	else if (iChar == 0x08) {
		if (nLen == 0) return TRUE;

		BYTE cFirst = _szInput[nLen - 1];
		if (cFirst >> 7) {
			_szInput[nLen - 2] = '\0';
			_szInput[nLen - 1] = '\0';
		}
		else {
			_szInput[nLen - 1] = '\0';
		}
	}
	else if (nLen < 48) {
		_szInput[nLen] = iChar;
		_szInput[nLen + 1] = '\0';
	}

	if (_bShowCursor) sprintf(_szInputTmp, "]%s_", _szInput);
	else sprintf(_szInputTmp, "]%s", _szInput);

	return TRUE;
}

void MPConsole::_AddText(const char* pszText, bool bCmd) {
	if (strlen(pszText) == 0) return;
	list<string>* pList = &_TextList;
	if (bCmd) {
		pList = &_CmdList;
	}
	int nSize = (int)pList->size();
	if (nSize > _nMaxLine) {
		pList->pop_front();
	}
	pList->push_back(pszText);

	if (bCmd) {
		_itCmd = pList->end();
		return;
	}
}

void MPConsole::Show(BOOL bShow) {
	_bVisible = bShow;
}

void MPConsole::FrameMove() //
{
	DWORD dwTick = GetTickCount();
	if (_dwCursorTick == 0) _dwCursorTick = dwTick;
	if ((dwTick - _dwCursorTick) < 500) return;

	_dwCursorTick = dwTick;

	_bShowCursor = !_bShowCursor;

	if (_bShowCursor) sprintf(_szInputTmp, "]%s_", _szInput);
	else sprintf(_szInputTmp, "]%s", _szInput);
}

void MPConsole::Clear() {
	_TextList.clear();
	for (int i = 0; i < _nMaxLine; i++) {
		_TextList.push_back("");
	}
}
