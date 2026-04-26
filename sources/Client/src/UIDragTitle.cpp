#include "StdAfx.h"
#include "uidragtitle.h"

using namespace GUI;
//---------------------------------------------------------------------------
// class CDragTitle
//---------------------------------------------------------------------------
CDragTitle::CDragTitle(CForm& frmOwn)
	: CCompent(frmOwn), _pImage(NULL), _IsShowDrag(true) {
	_pImage = new CGuiPic(this);
	SetIsDrag(true);
	_pDrag->SetYare(0);
}

CDragTitle::CDragTitle(const CDragTitle& rhs)
	: CCompent(rhs), _pImage(new CGuiPic(*rhs._pImage)), _IsShowDrag(rhs._IsShowDrag) {
	_SetSelf(rhs);
}

CDragTitle& CDragTitle::operator=(const CDragTitle& rhs) {
	CCompent::operator =(rhs);

	*_pImage = *(rhs._pImage);
	_IsShowDrag = rhs._IsShowDrag;
	_SetSelf(rhs);
	return *this;
}

void CDragTitle::_SetSelf(const CDragTitle& rhs) {
}

CDragTitle::~CDragTitle(void) {
	//delete _pImage;
	SAFE_DELETE(_pImage); // UI
}

void CDragTitle::DragRender() {
	if (!_IsShowDrag)
		return;

	if (CDrag::IsDraging(this)) {
		//  
		_pImage->Render(GetX() + CDrag::GetDrag()->GetDragX(), GetY() + CDrag::GetDrag()->GetDragY(), DROP_ALPHA);
	}
	else {
		_pImage->Render(GetX(), GetY());
	}
}

void CDragTitle::Render() {
	if (!GetIsShow()) return;

	_pImage->Render(GetX(), GetY());
}

void CDragTitle::Refresh() {
	CCompent::Refresh();

	_pImage->Refresh();
}

bool CDragTitle::MouseRun(int x, int y, DWORD key) {
	if (!IsNormal()) return false;

	if (InRect(x, y)) {
		if (_pDrag && _pDrag->BeginMouseRun(this, _IsMouseIn, x, y, key)) return true;
		return true;
	}
	return _IsMouseIn;
}
