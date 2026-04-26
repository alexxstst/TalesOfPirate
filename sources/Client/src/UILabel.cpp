#include "StdAfx.h"
#include "UIText.h"
#include "uilabel.h"

using namespace GUI;
//---------------------------------------------------------------------------
// class CLabelEx
//---------------------------------------------------------------------------
CLabel::CLabel(CForm& frmOwn)
	: CCompent(frmOwn), _color(COLOR_BLACK) {
}

CLabel::CLabel(const CLabel& rhs)
	: CCompent(rhs), _color(rhs._color), _caption(rhs._caption) {
}

CLabel::~CLabel(void) {
}

void CLabel::Render() {
	ui::Render((char*)_caption.c_str(), GetX(), GetY(), _color);
}

//---------------------------------------------------------------------------
// class CLabelEx
//---------------------------------------------------------------------------
CLabelEx::CLabelEx(CForm& frmOwn)
	: CLabel(frmOwn), _IsShadow(true), _clShadow(COLOR_BLACK), _nFontIndex(0), _IsCenter(false), _IsFlash(false) {
}

CLabelEx::CLabelEx(const CLabelEx& rhs)
	: CLabel(rhs) {
	_Copy(rhs);
}

void CLabelEx::_Copy(const CLabelEx& rhs) {
	_IsShadow = rhs._IsShadow;
	_clShadow = rhs._clShadow;
	_IsCenter = rhs._IsCenter;
	_nFontIndex = rhs._nFontIndex;
	_IsFlash = rhs._IsFlash;
}

void CLabelEx::Render() {
	DWORD dwOldShadow = _clShadow; // 
	DWORD dwOldColor = _color; // 

	if (_IsFlash) // 
	{
		DWORD dwColorID = (GetTickCount() & 1023) >> 8;
		DWORD dwColor = 0;
		switch (dwColorID) {
		case 0:
			dwColor = 0xFFFF0000;
			break;
		case 1:
			dwColor = 0xFF800000;
			break;
		case 2:
			dwColor = 0xFF0000FF;
			break;
		case 3:
			dwColor = 0xFF000080;
			break;
		}

		_color = dwColor;
		//_clShadow = dwColor;
	}

	if (_IsShadow) {
		if (_IsCenter == 0) {
			//if(_caption == "")
			//	ui::BRender( _nFontIndex, "", GetX(), GetY(), _color, _clShadow );
			//else
			ui::BRender(_nFontIndex, (const char*)_caption.c_str(), GetX(), GetY(), _color, _clShadow);
		}
		else if (_IsCenter == 2) {
			int textWidth = ui::GetWidth(_caption.c_str());
			int posX = GetWidth() - textWidth;
			ui::BRender(_nFontIndex, (const char*)_caption.c_str(), GetX() + posX, GetY(), _color, _clShadow);
		}
		else {
			int posX = (GetWidth() - ui::GetWidth(_caption.c_str())) / 2;
			ui::BRender(_nFontIndex, (const char*)_caption.c_str(), GetX() + posX, GetY(), _color, _clShadow);
		}
	}
	else {
		if (_IsCenter == 0) {
			ui::Render(_nFontIndex, (char*)_caption.c_str(), GetX(), GetY(), _color);
		}
		else if (_IsCenter == 2) {
			int posX = (GetWidth() - ui::GetWidth(_caption.c_str()) * 2);
			ui::Render(_nFontIndex, (char*)_caption.c_str(), GetX() + posX, GetY(), _color);
		}
		else {
			int posX = (GetWidth() - ui::GetWidth(_caption.c_str())) / 2;
			ui::Render(_nFontIndex, (char*)_caption.c_str(), GetX() + posX, GetY(), _color);
		}
	}

	_clShadow = dwOldShadow; // 
	_color = dwOldColor; //?
}
