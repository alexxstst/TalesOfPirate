#include "StdAfx.h"
#include "uiprogressbar.h"
#include "GameApp.h"

using namespace GUI;

//---------------------------------------------------------------------------
// class CProgressBar
//---------------------------------------------------------------------------
CProgressBar::CProgressBar(CForm& frmOwn, eStyle style)
	: CCompent(frmOwn), _bActiveMouse(false), _style(style), _pTex(NULL), _nFlash(1), _eHintStyle(hsHintPercent),
	  evtMouseDown(NULL), evtTimeArrive(NULL), _dwEndTime(0), _dwStartTime(0) {
	_pImage = new CGuiPic(this, pcEnd);
}

CProgressBar::CProgressBar(const CProgressBar& rhs)
	: CCompent(rhs), _pImage(new CGuiPic(*rhs._pImage)) {
	_Copy(rhs);
}

CProgressBar& CProgressBar::operator=(const CProgressBar& rhs) {
	CCompent::operator =(rhs);

	*_pImage = *rhs._pImage;

	_Copy(rhs);
	return *this;
}

void CProgressBar::_Copy(const CProgressBar& rhs) {
	_step = rhs._step;
	_style = rhs._style;
	_fEnd = rhs._fEnd;
	_bActiveMouse = rhs._bActiveMouse;
	evtTimeArrive = rhs.evtTimeArrive;
	evtMouseDown = rhs.evtMouseDown;
	_dwEndTime = rhs._dwEndTime;
	_dwStartTime = rhs._dwStartTime;

	_nFlash = rhs._nFlash;
	_eHintStyle = rhs._eHintStyle;

	_pTex = _pImage->GetImage(pcProgressbar);
	_nStart = rhs._nStart;
}

CProgressBar::~CProgressBar() {
	//delete _pImage;
	SAFE_DELETE(_pImage); // UI
}

void CProgressBar::Refresh() {
	CCompent::Refresh();

	_RefreshPos();
}

void CProgressBar::RenderHint(int x, int y) {
	char buf[80] = {0};
	if (_eHintStyle == hsHintNum) {
		sprintf(buf, "%s:%d/%d", _strHint.c_str(), (int)_step.GetPosition(), (int)_step.GetRange());
	}
	else {
		sprintf(buf, "%s:%.02f%%", _strHint.c_str(), _step.GetPosition() / _step.GetRange() * 100.0f);
	}
	_RenderHint(buf, x, y);
}

bool CProgressBar::MouseRun(int x, int y, DWORD key) {
	if (!_bActiveMouse) return false;
	if (!IsNormal()) return false;

	if (InRect(x, y)) {
		if (key & Mouse_LDown) {
			if (evtMouseDown) evtMouseDown(this, x, y, key);
		}
		return true;
	}
	return false;
}
