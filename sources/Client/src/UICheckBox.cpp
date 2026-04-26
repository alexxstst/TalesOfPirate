#include "StdAfx.h"
#include "UIText.h"
#include "UICheckBox.h"
using namespace GUI;

int CCheckBox::s_nMarginLeft = 5;

CCheckBox::CCheckBox(CForm& frmOwn)
	: CCompent(frmOwn), evtCheckChange(NULL), _pImage(NULL), _TextColor(0xffff0000), _isDown(false), _isChecked(false) {
	_pImage = new CGuiPic(this, stEnd);
	_SetSelf();
}

CCheckBox::CCheckBox(const CCheckBox& rhs)
	: CCompent(rhs), _pImage(rhs._pImage), _isChecked(rhs._isChecked), _TextColor(rhs._TextColor),
	  _isDown(rhs._isDown) {
	_SetSelf();
}

CCheckBox& CCheckBox::operator=(const CCheckBox& rhs) {
	CCompent::operator =(rhs);

	_isChecked = rhs._isChecked;
	*_pImage = *rhs._pImage;
	_TextColor = rhs._TextColor;
	_isDown = rhs._isDown;

	_SetSelf();
	return *this;
}

void CCheckBox::_SetSelf() {
	_nTextOffsetX = 0;
	_pImage->SetFrame(_isChecked);
}

void CCheckBox::Render() {
	_pImage->Render(GetX(), GetY());

	if (!_strCaption.empty()) ui::Render(_strCaption.c_str(), GetX() + _nTextOffsetX, GetY() + _nTextOffsetY,
										 _TextColor);
}

void CCheckBox::Refresh() {
	CCompent::Refresh();

	_nTextOffsetY = (int)((_pImage->GetImage()->nTexH * _pImage->GetImage()->fScaleY - ui::GetHeight("S")) / 2.0);

	if (!_strCaption.empty()) {
		_nTextOffsetX = (int)(_pImage->GetImage()->nTexW * _pImage->GetImage()->fScaleX + s_nMarginLeft);
		_nX2 = _nX1 + _nTextOffsetX + ui::GetWidth(_strCaption.c_str());
	}
	else {
		_nTextOffsetX = (int)(_pImage->GetImage()->nTexW * _pImage->GetImage()->fScaleX);
		_nX2 = _nX1 + _nTextOffsetX;
	}
}

bool CCheckBox::MouseRun(int x, int y, DWORD key) {
	if (!IsNormal()) return false;

	if (IsNoDrag(x, y, key)) {
		if (_isDown && (key & Mouse_LUp)) {
			_isDown = false;
			_isChecked = !_isChecked;
			_pImage->SetFrame(_isChecked);
			if (evtCheckChange) evtCheckChange(this);
			return true;
		}
		if (key & Mouse_LDown) {
			_isDown = true;
			return true;
		}
		return true;
	}
	else {
		_isDown = false;
	}
	return _IsMouseIn;
}

//---------------------------------------------------------------------------
// class CCheckGroup
//---------------------------------------------------------------------------
CCheckGroup::CCheckGroup(CForm& frmOwn)
	: CContainer(frmOwn), _nSize(0), _pActive(NULL), _nActive(-1), evtSelectChange(NULL) {
}

CCheckGroup::CCheckGroup(const CCheckGroup& rhs)
	: CContainer(rhs) {
	_SetSelf();
}

CCheckGroup& CCheckGroup::operator=(const CCheckGroup& rhs) {
	CContainer::operator =(rhs);
	_SetSelf();
	return *this;
}

void CCheckGroup::Init() {
	CContainer::Init();

	if (!_items.empty()) _UpdataActive((CCheckBox*)_items[0]);
}

void CCheckGroup::SetActiveIndex(int n) {
	_nActive = n;
	if (!_items.empty()) _UpdataActive((CCheckBox*)_items[_nActive]);
}

void CCheckGroup::_UpdataActive(CCheckBox* active) {
	if (_pActive == active) {
		_pActive->SetIsChecked(true);
		return;
	}

	for (int i = 0; i < _nSize; i++) {
		if (_items[i] == active) {
			_pActive = active;
			_pActive->SetIsChecked(true);

			_nActive = i;

			if (evtSelectChange) evtSelectChange(this);
		}
		else {
			((CCheckBox*)_items[i])->SetIsChecked(false);
		}
	}
}

void CCheckGroup::_SetSelf() {
	for (items::iterator it = _items.begin(); it != _items.end(); it++) {
		((CCheckBox*)(*it))->evtCheckChange = _CheckChange;
		((CCheckBox*)(*it))->SetIsChecked(false);
	}

	if (_nActive >= 0 && _nActive < _nSize)
		_pActive->SetIsChecked(true);
}
