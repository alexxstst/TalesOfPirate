#include "StdAfx.h"
#include "ui3dcompent.h"

C3DCompent::C3DCompent(CForm& frmOwn)
	: CCompent(frmOwn), _pRenderEvent(NULL), evtMouseDown(NULL), evtMouseLDDown(NULL), _pPointer(NULL) {
}

C3DCompent::C3DCompent(const C3DCompent& rhs)
	: CCompent(rhs) {
	_Copy(rhs);
}

C3DCompent& C3DCompent::operator=(const C3DCompent& rhs) {
	CCompent::operator =(rhs);

	_Copy(rhs);
	return *this;
}

void C3DCompent::_Copy(const C3DCompent& rhs) {
	_pRenderEvent = rhs._pRenderEvent;

	evtMouseDown = rhs.evtMouseDown;
	evtMouseLDDown = rhs.evtMouseLDDown;

	_pPointer = rhs._pPointer;
}

C3DCompent::~C3DCompent() {
}

void C3DCompent::Refresh() {
	CCompent::Refresh();

	float dx = GetRender().DrawConvertX(1.0);
	float dy = GetRender().DrawConvertY(1.0);

	_nCenterX = int((float)(GetX() + GetWidth() / 2.0) * dx);
	_nCenterY = int((float)(GetY() + GetHeight() / 2.0) * dy);
}

void C3DCompent::Render() {
	if (_pRenderEvent) _pRenderEvent(this, _nCenterX, _nCenterY);
}

bool C3DCompent::MouseRun(int x, int y, DWORD key) {
	if (!IsNormal()) return false;

	if (IsNoDrag(x, y, key)) {
		if ((key & Mouse_Down)) {
			if (!_isChild && GetActive() != this) _SetActive();
			if (evtMouseDown) evtMouseDown(this, x, y, key);
		}

		if ((key & Mouse_LDB)) {
			if (evtMouseLDDown) evtMouseLDDown(this, x, y, key);
		}
		return true;
	}
	return _IsMouseIn;
}
