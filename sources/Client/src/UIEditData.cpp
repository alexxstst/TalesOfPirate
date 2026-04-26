#include "StdAfx.h"
#include "UIText.h"
#include "uieditdata.h"
#include "uieditstrategy.h"
#include "uifont.h"

using namespace GUI;

//---------------------------------------------------------------------------
// class CEditObj
//---------------------------------------------------------------------------
void CEditObj::Parse(CEditStrategy* pParse) {
	pParse->ParseObj(this);
}

//---------------------------------------------------------------------------
// class CEditTextObj
//---------------------------------------------------------------------------
CEditTextObj::CEditTextObj()
	: _dwColor(COLOR_BLACK), _dwFont(0) {
}

void CEditTextObj::Parse(CEditStrategy* pParse) {
	pParse->ParseText(this);
}

//---------------------------------------------------------------------------
// class CEditTextObj
//---------------------------------------------------------------------------
void CEditChar::InvertRedner(int x, int y) {
	ui::Render(GetFont(), _szChar, x, y, COLOR_RED);
}

void CEditChar::SetWord(char c1, char c2) {
	_szChar[0] = c1;
	_szChar[1] = c2;
	int nHeight;
	ui::GetSize(GetFont(), _szChar, _nWidth, nHeight);
}

//---------------------------------------------------------------------------
// class CEditControl
//---------------------------------------------------------------------------
void CEditControl::Parse(CEditStrategy* pParse) {
	pParse->ParseControl(this);
}

//---------------------------------------------------------------------------
// class CEditSentence
//---------------------------------------------------------------------------
CEditSentence::CEditSentence()
	: _nX(0), _nY(0) {
}

void CEditSentence::Render() {
	ui::Render(GetFont(), _szString.c_str(), _nX, _nY, GetColor());
}

void CEditSentence::GetSize(int& w, int& h) {
	ui::GetSize(GetFont(), _szString.c_str(), w, h);
}

//---------------------------------------------------------------------------
// class CEditArticle
//---------------------------------------------------------------------------
CEditArticle::CEditArticle()
	: _eRunType(enumNormal), _dwFontColor(COLOR_BLACK), _dwFontIndex(0), _dwWordMax(0) {
	_pStrategy = new CEditStrategy(this);

	Clear();
}

CEditArticle::~CEditArticle() {
	//delete _pStrategy;
	SAFE_DELETE(_pStrategy); // UI
}

void CEditArticle::Init() {
}

void CEditArticle::Clear() {
	for (memorys::iterator it = _memorys.begin(); it != _memorys.end(); it++) {
		//delete (*it);
		SAFE_DELETE(*it); // UI
	}
	_memorys.clear();

	_dwWordMax = 0;
	_dwWordNum = 0;

	_nCursor = 0;
	_nSelStart = _nSelEnd = 0;
	_eRunType = enumNormal;

	_pStrategy->Clear();
}

void CEditArticle::Render() {
	_pStrategy->Render();

	const std::string szBuf = std::format("{}, {}, {}", _nCursor, _nSelStart, _nSelEnd);
	ui::Render(szBuf.c_str(), 100, 100, COLOR_BLACK);
}

bool CEditArticle::_AddObj(CEditObj* pObj) {
	DelSelect();

	if (_dwWordMax > 0 && _dwWordNum + pObj->GetWordCount() >= _dwWordMax)
		return false;

	if (_nCursor >= _memorys.size()) {
		// 
		if (_pStrategy->Append(pObj)) {
			_memorys.push_back(pObj);
			_nCursor = (int)_memorys.size();
			_dwWordNum += pObj->GetWordCount();

			_pStrategy->RefreshPos(100, 200);
		}
		else {
			return false;
		}
	}
	else {
		_memorys.insert(_memorys.begin() + _nCursor, pObj);
		_dwWordNum += pObj->GetWordCount();

		RefreshText();
		_nCursor++;
	}
	return true;
}

bool CEditArticle::DelChar(unsigned int nIndex) {
	if (_memorys.empty()) return false;

	CEditObj* pObj = NULL;
	if (nIndex < _memorys.size()) {
		pObj = _memorys[nIndex];
		_memorys.erase(_memorys.begin() + nIndex);

		RefreshText();
		return true;
	}
	return false;
}

void CEditArticle::RefreshText() {
	_pStrategy->Clear();
	for (memorys::iterator it = _memorys.begin(); it != _memorys.end(); it++)
		_pStrategy->Append(*it);

	_pStrategy->RefreshPos(100, 200);
}

void CEditArticle::DelSelect() {
	if (_eRunType == enumSelect) {
		if (_nSelEnd <= _memorys.size() && _nSelStart <= _memorys.size()) {
			unsigned int start = _nSelStart > _nSelEnd ? _nSelEnd : _nSelStart;
			unsigned int end = _nSelStart < _nSelEnd ? _nSelEnd : _nSelStart;
			for (unsigned int i = start; i < end; i++) {
				//delete _memorys[i];
				SAFE_DELETE(_memorys[i]); // UI
			}
			_memorys.erase(_memorys.begin() + start, _memorys.begin() + end);
			RefreshText();
			_nCursor = start;
		}
		else {
			ToLogService("common", "msgSize:{}, Start:{}, End:{}", _memorys.size(), _nSelStart, _nSelEnd);
		}
		_eRunType = enumNormal;
	}
}

void CEditArticle::OnKeyDown(int key, bool IsShiftPress) {
	switch (key) {
	case VK_LEFT:
		if (IsShiftPress) {
			_ToSelectMode();
			if (_nSelEnd > 0)
				_nSelEnd--;
		}
		else if (_eRunType == enumNormal) {
			// 
			if (_nCursor > 0) _nCursor--;
		}
		else {
			// 
			_eRunType = enumNormal;
			_nCursor = _nSelStart;
		}
		break;
	case VK_RIGHT:
		if (IsShiftPress) {
			_ToSelectMode();
			_nSelEnd++;
			if (_nSelEnd > (unsigned int)_memorys.size())
				_nSelEnd = (unsigned int)_memorys.size();
		}
		else if (_eRunType == enumNormal) {
			// 
			if (_nCursor < _memorys.size()) _nCursor++;
		}
		else {
			// 
			_eRunType = enumNormal;
			_nCursor = _nSelEnd;
		}
		break;
	case VK_UP:
		break;
	case VK_DOWN:
		break;
	case VK_HOME:
		if (IsShiftPress) {
			_ToSelectMode();
			_nSelEnd = 0;
		}
		else {
			_eRunType = enumNormal;
			_nCursor = 0;
		}
		break;
	case VK_END:
		if (IsShiftPress) {
			_ToSelectMode();
			_nSelEnd = (int)_memorys.size();
		}
		else {
			_eRunType = enumNormal;
			_nCursor = (int)_memorys.size();
		}
		break;
	case VK_PRIOR: // pageup
		break;
	case VK_NEXT: // pagedown
		break;
	case VK_DELETE:
		if (_eRunType == enumNormal) {
			DelChar(_nCursor);
		}
		else {
			DelSelect();
		}
		break;
	case VK_BACK:
		if (_eRunType == enumNormal) {
			if (_nCursor > 0 && DelChar(_nCursor - 1))
				_nCursor--;
		}
		else {
			DelSelect();
		}
		break;
	case VK_RETURN:
		if (_eRunType == enumSelect) {
			DelSelect();
		}
		break;
	default:
		break;
	}
	return;
}
