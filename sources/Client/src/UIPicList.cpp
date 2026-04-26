#include "stdafx.h"
#include "UIPicList.h"

using namespace GUI;
//---------------------------------------------------------------------------
// class CFixList
//---------------------------------------------------------------------------
CFixList::CFixList(CForm& frmOwn)
	: CCompent(frmOwn), _nRowHeight(22), _nRowSpace(0), _pImage(NULL), _pSelect(NULL)
	  , _nLeftMargin(5), _nTopMargin(5), _nRightMargin(5), _nBottomMargin(5)
	  , evtSelectChange(NULL), evtListMouseDown(NULL) {
	_pImage = new CGuiPic(this);
	_pSelect = new CGuiPic;
	SetMaxNum(1);
}

CFixList::CFixList(const CFixList& rhs)
	: CCompent(rhs) {
	_SetSelf(rhs);
}

CFixList& CFixList::operator=(const CFixList& rhs) {
	CCompent::operator =(rhs);
	_SetSelf(rhs);
	return *this;
}

void CFixList::_SetSelf(const CFixList& rhs) {
	_lmtSel = rhs._lmtSel;
	_nRowHeight = rhs._nRowHeight;
	_nRowSpace = rhs._nRowSpace;
	*_pImage = *rhs._pImage;
	*_pSelect = *rhs._pSelect;
	_nLeftMargin = rhs._nLeftMargin;
	_nTopMargin = rhs._nTopMargin;
	_nRightMargin = rhs._nRightMargin;
	_nBottomMargin = rhs._nBottomMargin;
	evtSelectChange = rhs.evtSelectChange;
	evtListMouseDown = rhs.evtListMouseDown;
	_vstString = rhs._vstString;
}

CFixList::~CFixList() {
	//delete _pImage; 
	//delete _pSelect; 

	SAFE_DELETE(_pImage); // UI
	SAFE_DELETE(_pSelect); // UI
}

void CFixList::Render() {
	_pImage->Render(GetX(), GetY());
	_pSelect->Render(_nSelX, _nSelY);
}

void CFixList::Refresh() {
	CCompent::Refresh();

	_pImage->Refresh();

	_nHitX1 = GetX() + _nLeftMargin;
	_nHitX2 = GetX2() - _nRightMargin;
	_nHitY1 = GetY() + _nTopMargin;
	_nHitY2 = GetY2() - _nBottomMargin;

	_pSelect->SetScale(_nHitX2 - _nHitX1, _nRowHeight);
	_RefreshSelY();
	_nSelX = _nHitX1;
}

bool CFixList::MouseRun(int x, int y, DWORD key) {
	if (!IsNormal()) return false;

	if (IsNoDrag(x, y, key)) {
		if ((key & Mouse_LDown) && (x >= _nHitX1 && x <= _nHitX2 && y >= _nHitY1 && y <= _nHitY2)) {
			// 
			int row = (y - _nHitY1) / (_nRowHeight + _nRowSpace);
			if ((_lmtSel.GetCur() != row) && (_lmtSel.SetCur(row))) {
				_RefreshSelY();
				if (evtSelectChange) evtSelectChange(this);
			}
			if (evtListMouseDown) evtListMouseDown(this, x, y, key);
		}
		return true;
	}
	return _IsMouseIn;
}

bool CFixList::MouseScroll(int nScroll) {
	if (!IsNormal()) return false;

	if (nScroll > 0) {
		--_lmtSel;
	}
	else {
		++_lmtSel;
	}
	Refresh();
	return _IsMouseIn;
}

bool CFixList::SetMaxNum(int n) {
	if (n > 0) {
		_lmtSel.SetRange(n - 1);
		_vstString.resize(n);
		return true;
	}
	return false;
}

const char* CFixList::GetString(int n) {
	if (n > 0 && n < _lmtSel.GetMax())
		return _vstString[n].c_str();
	else
		return NULL;
}

//---------------------------------------------------------------------------
// class CCheckFixList
//---------------------------------------------------------------------------
CCheckFixList::CCheckFixList(CForm& frmOwn)
	: CFixList(frmOwn), _nCheckLeftMargin(0), _nCheckTopMargin(0) {
	_pImgCheck = new CGuiPic;
	_pImgUnCheck = new CGuiPic;
	evtListMouseDown = _OnCheckDown;
}

CCheckFixList::CCheckFixList(const CCheckFixList& rhs)
	: CFixList(rhs) {
	_SetSelf(rhs);
}

CCheckFixList& CCheckFixList::operator=(const CCheckFixList& rhs) {
	CFixList::operator=(rhs);
	_SetSelf(rhs);
	return *this;
}

void CCheckFixList::_SetSelf(const CCheckFixList& rhs) {
	*_pImgCheck = *rhs._pImgCheck;
	*_pImgUnCheck = *rhs._pImgUnCheck;
	_checks = rhs._checks;
	_nCheckLeftMargin = rhs._nCheckLeftMargin;
	_nCheckTopMargin = rhs._nCheckTopMargin;
	evtListMouseDown = _OnCheckDown;
}

CCheckFixList::~CCheckFixList() {
	//delete _pImgCheck; 
	//delete _pImgUnCheck; 

	SAFE_DELETE(_pImgCheck); // UI
	SAFE_DELETE(_pImgUnCheck); // UI
}

bool CCheckFixList::SetMaxNum(int n) {
	if (CFixList::SetMaxNum(n)) {
		_checks.resize(n);
		for (int i = 0; i < n; i++)
			_checks[i] = false;
		return true;
	}
	return false;
}

void CCheckFixList::Render() {
	_pImage->Render(GetX(), GetY());
	_pSelect->Render(_nSelX, _nSelY);

	int y = _nHitY1;
	for (int i = 0; i <= _lmtSel.GetMax(); i++) {
		if (_checks[i])
			_pImgCheck->Render(_nSelX + _nCheckLeftMargin, y + _nCheckTopMargin);
		else
			_pImgUnCheck->Render(_nSelX + _nCheckLeftMargin, y + _nCheckTopMargin);
		y = y + _nRowHeight + _nRowSpace;
	}
}
