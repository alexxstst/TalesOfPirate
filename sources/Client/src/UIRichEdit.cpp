#include "StdAfx.h"
#include "uirichedit.h"
#include "gameapp.h"
#include "uieditkey.h"
#include "EncodingUtil.h"

using namespace GUI;
//---------------------------------------------------------------------------
// class CRichEdit
//---------------------------------------------------------------------------
CRichEdit::CRichEdit(CForm& frmOwn)
	: CCompent(frmOwn), _IsReadyOnly(false) {
	_IsFocus = true;

	_pEditKey = new CEditKey;

	_SetSelf();
}

CRichEdit::CRichEdit(const CRichEdit& rhs)
	: CCompent(rhs) {
	_pEditKey = new CEditKey;

	_Copy(rhs);
}

CRichEdit& CRichEdit::operator=(const CRichEdit& rhs) {
	_Copy(rhs);
	return *this;
}

void CRichEdit::_SetSelf() {
	_cArticle.SetEdit(this);

	memset(_szEnter, 0, sizeof(_szEnter));
	_nEnterPos = 0;
}

void CRichEdit::_Copy(const CRichEdit& rhs) {
	_SetSelf();
}

CRichEdit::~CRichEdit(void) {
}

void CRichEdit::Init() {
	_pEditKey->Init();
}

void CRichEdit::Refresh() {
}

void CRichEdit::OnActive() {
}

void CRichEdit::OnLost() {
}

bool CRichEdit::OnKeyDown(int key) {
	if (_IsReadyOnly) return false;

	_cArticle.OnKeyDown(key, g_pGameApp->IsShiftPress() != 0);
	switch (key) {
	case VK_LEFT:
		break;
	case VK_RIGHT:
		break;
	case VK_UP:
		break;
	case VK_DOWN:
		break;
	case VK_HOME:
		break;
	case VK_END:
		break;
	case VK_PRIOR: // pageup
		break;
	case VK_NEXT: // pagedown
		break;
	case VK_DELETE:
		break;
	default:
		return false;
	}
	return false;
}

bool CRichEdit::OnChar(char c) {
	if (_IsReadyOnly) return false;

	switch (c) {
	case '\r':
		_cArticle.AddControl(c);
		break;
	case '\b':
	case '\t':
	case 3: // copy
	case 22: // paste
	case 24: // cut
	case 27: // ESC
		break;
	default: {
		// Аналогично CEditKey::OnChar: CP_ACP → UTF-8 через EncodingUtil,
		// порционно в CArticle::AddChar (1 или 2 байта UTF-8).
		const unsigned char byte = static_cast<unsigned char>(c);
		std::string utf8;
		if (_nEnterPos == 1) {
			_szEnter[1] = c;
			utf8 = encoding::AnsiToUtf8(std::string_view(_szEnter, 2));
			_nEnterPos = 0;
			_szEnter[0] = _szEnter[1] = 0;
		}
		else if (byte >= 0x80 && ::IsDBCSLeadByteEx(CP_ACP, byte)) {
			_szEnter[0] = c;
			_nEnterPos = 1;
			return false;
		}
		else {
			encoding::AppendAnsiByteAsUtf8(byte, utf8);
		}
		if (utf8.size() == 1) {
			_cArticle.AddChar(utf8[0]);
		}
		else if (utf8.size() == 2) {
			_cArticle.AddChar(utf8[0], utf8[1]);
		}
	}
	}

	return false;
}

void CRichEdit::Render() {
	_cArticle.Render();

	_pEditKey->Render();
}
