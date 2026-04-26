#include "stdafx.h"
#include "UICursor.h"
#include "uiguidata.h"

//---------------------------------------------------------------------------
// class CCursor
//---------------------------------------------------------------------------
using namespace GUI;
CCursor CCursor::s_Cursor;

CCursor::CCursor()
	: _eState(stEnd), _eFrame(stEnd), _eActive(stEnd), _IsInit(false), _IsShowFrame(false) {
	memset(_hCursor, 0, sizeof(_hCursor));
}

CCursor::~CCursor() {
}

void CCursor::_ShowCursor() {
	if (_hCursor[_eActive]) {
		::SetClassLongPtr(CGuiData::GetHWND(), GCLP_HCURSOR, (LONG_PTR)(_hCursor[_eActive]));
		::SetCursor(_hCursor[_eActive]);
	}
}
