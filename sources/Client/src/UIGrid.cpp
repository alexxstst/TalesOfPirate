#include "StdAfx.h"
#include "UIText.h"
#include "uigrid.h"

using namespace GUI;

CGrid::CGrid(CForm& frmOwn)
	: CCompent(frmOwn), _pImage(NULL), _nUnitHeight(32), _nUnitWidth(32), _nFirst(0), _nLast(0), _nColNum(0)
	  , _nMaxPage(0), _nPage(0), _nMargin(5), _nPageNum(0), _nSize(0), _pSelect(NULL), _nSpaceX(2), _nSpaceY(2)
	  , _pSelectImage(NULL), evtSelectChange(NULL), _clPageTextColor(0xff0000ff), _nSelectIndex(-1) {
	// _IsFocus = true;

	_pImage = new CGuiPic(this);
	_pSelectImage = new CImage(frmOwn);

	_pSizeImg = new CDragTitle(frmOwn);

	_pNextPage = new CTextButton(frmOwn);
	_pPriorPage = new CTextButton(frmOwn);

	_SetSelf();
}

CGrid::CGrid(const CGrid& rhs)
	: CCompent(rhs), _pImage(new CGuiPic(*rhs._pImage)), _pSelectImage(new CImage(*rhs._pSelectImage))
	  , _pSizeImg(new CDragTitle(*rhs._pSizeImg)), _pNextPage(new CTextButton(*rhs._pNextPage))
	  , _pPriorPage(new CTextButton(*rhs._pPriorPage)) {
	_clPageTextColor = rhs._clPageTextColor;

	_Copy(rhs);
	_SetSelf();
}

void CGrid::_Copy(const CGrid& rhs) {
	_Clear();
	for (memory::const_iterator it = rhs._memory.begin(); it != rhs._memory.end(); it++)
		_memory.push_back((*it)->Clone());

	evtSelectChange = rhs.evtSelectChange;

	_strPage = rhs._strPage;
	_nStrX = rhs._nStrX;
	_nStrY = rhs._nStrY;
	_nStrWidth = rhs._nStrWidth;
	_nUnitHeight = rhs._nUnitHeight;
	_nUnitWidth = rhs._nUnitWidth;
	_nSpaceX = rhs._nSpaceX;
	_nSpaceY = rhs._nSpaceY;

	_nSize = rhs._nSize;
	_nFirst = rhs._nFirst;
	_nLast = rhs._nLast;
	_nPage = rhs._nPage;
	_nMaxPage = rhs._nMaxPage;
	_nColNum = rhs._nColNum;
	_nRowNum = rhs._nRowNum;
	_nPageNum = rhs._nPageNum;

	_nMargin = rhs._nMargin;
	_nStartX = rhs._nStartX;
	_nStartY = rhs._nStartY;
	_nDragWidth = rhs._nDragWidth;
	_nDragHeight = rhs._nDragHeight;
	_nTotalW = rhs._nTotalW;
	_nTotalH = rhs._nTotalH;

	_nSelectIndex = rhs._nSelectIndex;
	_IsSelect = rhs._IsSelect;
	_clPageTextColor = rhs._clPageTextColor;
}

CGrid& CGrid::operator=(const CGrid& rhs) {
	CCompent::operator =(rhs);

	*_pImage = *rhs._pImage;
	*_pSelectImage = *rhs._pSelectImage;
	*_pSizeImg = *rhs._pSizeImg;
	*_pNextPage = *rhs._pNextPage;
	*_pPriorPage = *rhs._pPriorPage;

	_Copy(rhs);
	_SetSelf();
	return *this;
}

void CGrid::_Clear() {
	for (memory::iterator it = _memory.begin(); it != _memory.end(); it++) {
		//delete *it;
		SAFE_DELETE(*it); // UI
	}

	_memory.clear();
}

CGrid::~CGrid(void) {
	_Clear();
	//delete _pImage;
	SAFE_DELETE(_pImage); // UI
}

void CGrid::_SetSelf() {
	_nSelectIndex = -1;
	_pSelect = NULL;

	_pNextPage->SetParent(this);
	_pNextPage->evtMouseClick = _NextClick;

	_pPriorPage->SetParent(this);
	_pPriorPage->evtMouseClick = _PriorClick;

	_pSizeImg->SetParent(this);
	_pSizeImg->SetIsShowDrag(false);

	CDrag* pDrag = _pSizeImg->GetDrag();
	pDrag->SetDragInCursor(CCursor::stActive);
	pDrag->evtMouseDragBegin = _DragBegin;
	pDrag->evtMouseDragMove = _DragMove;
	pDrag->evtMouseDragEnd = _DragEnd;

	_pSelectImage->SetParent(this);
}

void CGrid::Render() {
	_pImage->Render(GetX(), GetY());

	int x = _nStartX;
	int y = _nStartY;
	int col = 0;
	for (int i = _nFirst; i < _nLast; i++) {
		_memory[i]->Render(x, y);
		if (++col >= _nColNum) {
			col = 0;
			y += _nTotalH;
			x = _nStartX;
		}
		else {
			x += _nTotalW;
		}
	}

	if (_IsSelect) {
		_pSelectImage->Render();
	}

	if (_pPriorPage->GetIsShow()) _pPriorPage->Render();
	ui::Render(_strPage.c_str(), _nStrX, _nStrY, _clPageTextColor);
	if (_pNextPage->GetIsShow()) _pNextPage->Render();
	if (_pSizeImg->GetIsShow()) _pSizeImg->Render();
}

void CGrid::Refresh() {
	CCompent::Refresh();

	_pImage->Refresh();

	// _pSizeImg
	_pSizeImg->SetPos(GetWidth() - _pSizeImg->GetWidth(), GetHeight() - _pSizeImg->GetHeight());
	_pSizeImg->Refresh();

	// 
	_pPriorPage->SetPos(_nMargin, GetHeight() - _pPriorPage->GetHeight() - _nMargin);
	_pPriorPage->Refresh();

	_nStrX = _pPriorPage->GetX2() + _nMargin;
	_nStrY = _pPriorPage->GetY() + (_pPriorPage->GetHeight() - ui::GetHeight("S")) / 2;

	_pNextPage->SetPos(_pPriorPage->GetWidth() + _nMargin * 3 + _nStrWidth, _pPriorPage->GetTop());
	_pNextPage->Refresh();

	_nStartX = GetX() + _nMargin;
	_nStartY = GetY() + _nMargin;

	_nColNum = (GetWidth() - _nMargin - _nMargin) / _nTotalW;
	_nRowNum = (GetHeight() - _nMargin - _nMargin - _pNextPage->GetHeight()) / _nTotalH;

	_nPageNum = _nColNum * _nRowNum;

	if (_nPageNum <= 0) {
		_nMaxPage = 0;
	}
	else {
		_nMaxPage = _nSize / _nPageNum;
		if (_nSize > _nMaxPage * _nPageNum) {
			_nMaxPage++;
		}
		if (_nMaxPage > 0) _nMaxPage--;
	}

	_PageChange();
}

bool CGrid::MouseRun(int x, int y, DWORD key) {
	if (!IsNormal()) return false;

	if (IsNoDrag(x, y, key)) {
		if (!_isChild && GetActive() != this && (key & Mouse_LDown)) _SetActive();

		if (_pNextPage->MouseRun(x, y, key)) return true;
		if (_pPriorPage->MouseRun(x, y, key)) return true;

		if (_pSizeImg->MouseRun(x, y, key)) return true;

		// 
		int col = (x - _nStartX) / _nTotalW;
		int row = (y - _nStartY) / _nTotalH;
		if (col < _nColNum)
			x = col + row * _nColNum + _nFirst;
		_IsSelect = (x < _nSize) && (col < _nColNum) && (row < _nRowNum);
		if (_IsSelect) {
			if (_pSelect && _pSelect != _memory[x]) _pSelect->Reset();

			_pSelect = _memory[x];
			_nSelectIndex = x;
			_pSelectImage->SetPos(_nTotalW * col + _nMargin, _nTotalH * row + _nMargin);
			_pSelectImage->Refresh();

			if ((key & Mouse_LDown) && evtSelectChange) evtSelectChange(this);
		}
		else if (_pSelect) {
			_IsSelect = false;
			_pSelect->Reset();
			_pSelect = NULL;
			_nSelectIndex = -1;
		}
		return true;
	}

	if (_pSelect) {
		_IsSelect = false;
		_nSelectIndex = -1;
		_pSelect->Reset();
		_pSelect = NULL;
	}
	return _IsMouseIn;
}

bool CGrid::OnKeyDown(int key) {
	if (!IsNormal()) return false;

	switch (key) {
	case VK_LEFT:
	case VK_UP:
	case VK_PRIOR: // pageup
		_PriorClick();
		break;
	case VK_RIGHT:
	case VK_NEXT: // pagedown
	case VK_DOWN:
		_NextClick();
		break;
	case VK_HOME:
		_nPage = 0;
		Refresh();
		break;
	case VK_END:
		_nPage = _nMaxPage;
		Refresh();
		break;
	default:
		return false;
	}
	return true;
}

void CGrid::_DragBegin() {
	_nDragWidth = GetWidth();
	_nDragHeight = GetHeight();

	_RefreshSize();
}

void CGrid::_RefreshSize() {
	if ((_nDragWidth + _pSizeImg->GetDrag()->GetDragX()) > (3 * _nTotalW) && (_nDragHeight + _pSizeImg->GetDrag()->
		GetDragY()) > (3 * _nTotalH)) {
		SetSize(_nDragWidth + _pSizeImg->GetDrag()->GetDragX(), _nDragHeight + _pSizeImg->GetDrag()->GetDragY());
		Refresh();
		if (_nPage > _nMaxPage) {
			_nPage = _nMaxPage;
			_PageChange();
		}
	}
}

void CGrid::SetAlpha(BYTE alpha) {
	_pImage->SetAlpha(alpha);
	_pSizeImg->SetAlpha(alpha);
	_pNextPage->SetAlpha(alpha);
	_pPriorPage->SetAlpha(alpha);
	_clPageTextColor = (_clPageTextColor & 0x00ffffff) & (alpha << 24);
}

void CGrid::SetIsDrag(bool v) {
	_pSizeImg->SetIsEnabled(v);
}

void CGrid::FrameMove(DWORD dwTime) {
	static DWORD dwLastTime = 0;
	if (dwTime < dwLastTime) return;

	dwLastTime = dwTime + 150;
	for (int i = _nFirst; i < _nLast; i++) {
		_memory[i]->Next();
	}
}
