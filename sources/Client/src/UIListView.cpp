#include "StdAfx.h"
#include "uilistview.h"

using namespace GUI;


int CListView::_nTmpX = 0;
int CListView::_nTmpY = 0;
int CListView::_nTmpRow = 0;
int CListView::_nTmpCol = 0;
//---------------------------------------------------------------------------
// class CListView
//---------------------------------------------------------------------------
CListView::CListView(CForm& frmOwn, int nCol, eStyle eTitle)
	: CCompent(frmOwn), _nColumnHeight(16), _eTitle(eTitle), _pTitle(NULL) {
	_IsFocus = false;
	_IsShowHint = false;
	_pList = new CList(frmOwn, nCol);
	_pList->SetParent(this);

	switch (_eTitle) {
	case eWindowTitle: _pTitle = new CWindowsTitle(this);
		break;
	case eSimpleTitle: _pTitle = new CImageTitle(this);
		break;
	default: _pTitle = new CListTitle(this);
		break;
	}
	_pTitle->SetParent(this);
}

CListView::CListView(const CListView& rhs)
	: CCompent(rhs), _pList(new CList(*rhs._pList)) {
	_SetSelf(rhs);
}

CListView& CListView::operator=(const CListView& rhs) {
	CCompent::operator =(rhs);

	_SetSelf(rhs);
	return *this;
}

void CListView::_SetSelf(const CListView& rhs) {
	*_pList = *rhs._pList;
	_pList->SetParent(this);

	_pTitle->SetParent(this);

	_nColumnHeight = rhs._nColumnHeight;
	evtColumnClick = rhs.evtColumnClick;
}

CListView::~CListView(void) {
}

void CListView::Init() {
	_pTitle->Init();
	_nTotalW = _nSpaceX + _nUnitWidth;
	_nTotalH = _nSpaceY + _nUnitHeight;
	if (_eTitle != eNoTitle) {
		_pList->SetPos(0, _nColumnHeight);
		_pList->SetSize(GetWidth(), GetHeight() - _nColumnHeight);
	}
	else {
		_pList->SetPos(0, 0);
		_pList->SetSize(GetWidth(), GetHeight());
	}
	_pList->Init();
	_pList->SetName(GetName());
}

void CListView::Render() {
	_pTitle->Render();
	_pList->Render();
}

void CListView::Refresh() {
	CCompent::Refresh();

	if (_eTitle != eNoTitle) {
		_pList->SetSize(GetWidth(), GetHeight() - _nColumnHeight);
	}
	else {
		_pList->SetSize(GetWidth(), GetHeight());
	}
	_pTitle->Refresh();
	_pList->Refresh();
	if (_nHeight > 0) _nShowCount = (right - left) / _nHeight;
}

bool CListView::MouseRun(int x, int y, DWORD key) {
	if (!IsNormal()) return false;

	if (InRect(x, y)) {
		if ((key & Mouse_LDown) && !_isChild && GetActive() != this) _SetActive();

		if (_pList->MouseRun(x, y, key)) return true;
		if (_pTitle->MouseRun(x, y, key)) return true;
		return true;
	}

	return _IsMouseIn;
}

bool CListView::MouseScroll(int nScroll) {
	if (!IsNormal()) return false;

	return _pList->MouseScroll(nScroll);
}


bool CListView::SetShowRow(int n) {
	if (_pList->SetShowRow(n)) {
		if (_eTitle != eNoTitle) {
			SetSize(_pList->GetWidth(), _pList->GetHeight() + _nColumnHeight);
		}
		else {
			SetSize(_pList->GetWidth(), _pList->GetHeight());
		}
		return true;
	}
	return true;
}

bool CListView::OnKeyDown(int key) {
	if (IsNormal()) return _pList->OnKeyDown(key);

	return false;
}

void CListView::SetAlpha(BYTE alpha) {
	_pTitle->SetAlpha(alpha);
	_pList->SetAlpha(alpha);
}

void CListView::SetMargin(int left, int top, int right, int bottom) {
	_pList->SetMargin(left, top, right, bottom);
}

CItemRow* CListView::AddItemRow() {
	return GetList()->GetItems()->NewItem();
}

bool CListView::UpdateItemObj(int nRow, int nCol, CItemObj* pObj) {
	CItemRow* pRow = GetList()->GetItems()->GetItem(nRow);
	if (!pRow) return false;

	if (nCol >= (int)pRow->GetMax()) return false;

	pRow->SetIndex(nCol, pObj);
	return true;
}

CItemObj* CListView::GetItemObj(int nRow, int nCol) {
	CItemRow* pRow = GetList()->GetItems()->GetItem(nRow);
	if (!pRow) return NULL;

	if (nCol >= (int)pRow->GetMax()) return NULL;

	return pRow->GetIndex(nCol);
}

void CListView::RenderHint(int x, int y) {
	if (nTemp != -1 && pItem[nTemp]) {
		pItem[nTemp]->ReadyForHint(x, y, GetHitCommand(x, y));
		pItem[nTemp]->RenderHint(x, y);
	}
}

CCompent* CListView::GetHintCompent(int x, int y) {
	if (GetIsShow() && InRect(x, y)) {
		if (!_strHint.empty())
			return this;

		if (_IsShowHint) {
			if (_pList->GetItems()->GetSelect()->GetIsSelect() == true) {
				nTemp = _pList->GetItems()->GetSelect()->GetIndex();
			}
			else {
				nTemp = -1;
			}
			if (nTemp != -1 && pItem[nTemp] && SetHintItem(pItem[nTemp]))
			//nTemp!=-1 && pItem[nTemp] && SetHintItem( pItem[nTemp]
			{
				return this;
			}
		}
	}
	return NULL;
}


//---------------------------------------------------------------------------
// class CListView::CWindowsTitle
//---------------------------------------------------------------------------
CListView::CWindowsTitle::CWindowsTitle(CListView* pList)
	: CListTitle(pList) {
	_pColumn = new CContainer(*pList->GetForm());
	_pColumn->SetParent(pList);
	CImage* p = NULL;
	int nCol = pList->GetList()->GetItems()->GetColumn();
	for (int i = 0; i < nCol; i++) {
		p = new CImage(*pList->GetForm());
		_pColumn->AddCompent(p);
	}
}

void CListView::CWindowsTitle::Init() {
	int nCol = _pList->GetList()->GetItems()->GetColumn();
	int colHeight = _pList->GetColumnHeight();
	CImage* p = NULL;
	for (int i = 0; i < nCol; i++) {
		p = dynamic_cast<CImage*>(_pColumn->GetIndex(i));
		if (p) {
			p->evtMouseDown = CListView::OnColumnClick;
			p->SetSize(_pList->GetList()->GetItems()->GetColumnWidth(i), colHeight);
		}
	}
	_pColumn->ForEach(_InitColumnPos);
}

void CListView::CWindowsTitle::_InitColumnPos(CCompent* pThis, unsigned int index) {
	CContainer* p = (CContainer*)pThis->GetParent();
	CCompent* prior = p->GetIndex(index - 1);
	if (prior && (index > 0)) {
		pThis->SetPos(prior->GetWidth() + prior->GetLeft(), 0);
	}
}

void CListView::CWindowsTitle::SetColumnWidth(unsigned int nCol, unsigned int width) {
	if (_pList->GetList()->GetItems()->SetColumnWidth(nCol, width)) {
		CCompent* p = _pColumn->GetIndex(nCol);
		if (p) p->SetSize(width, p->GetHeight());
	}
}
