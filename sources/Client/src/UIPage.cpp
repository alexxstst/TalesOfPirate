#include "StdAfx.h"
#include "uipage.h"

using namespace GUI;
//---------------------------------------------------------------------------
// class CPageItem
//---------------------------------------------------------------------------
CPageItem::CPageItem(CForm& frmOwn)
	: CContainer(frmOwn), _nIndex(0), _pImage(NULL), _pTitle(NULL) {
	_pImage = new CImage(frmOwn);
	_pTitle = new CImage(frmOwn, tsEnd);

	_SetSelf();
}

CPageItem::CPageItem(const CPageItem& rhs)
	: CContainer(rhs), _pImage(new CImage(*rhs._pImage)), _pTitle(new CImage(*rhs._pTitle)) {
	_SetSelf();
}

CPageItem& CPageItem::operator=(const CPageItem& rhs) {
	CContainer::operator =(rhs);
	*_pImage = *rhs._pImage;
	*_pTitle = *rhs._pTitle;
	_SetSelf();
	return *this;
}

CPageItem::~CPageItem(void) {
}

void CPageItem::_SetSelf() {
	_pImage->SetParent(this);
	_pTitle->SetParent(this);
	_pTitle->evtMouseDown = _OnTitleDown;
}

void CPageItem::Render() {
	_pImage->Render();

	CContainer::Render();
}

void CPageItem::Refresh() {
	CContainer::Refresh();

	_pImage->SetSize(GetWidth(), GetHeight());
	_pImage->Refresh();
	_pTitle->Refresh();
}

void CPageItem::_OnTitleDown() {
	((CPage*)(_pParent))->SetIndex(_nIndex);
}

bool CPageItem::MouseRun(int x, int y, DWORD key) {
	if (_pTitle->MouseRun(x, y, key)) return true;

	return CContainer::MouseRun(x, y, key);
}

void CPageItem::_InActive(bool isActive) {
	SetIsShow(isActive);
	_pTitle->GetImage()->SetFrame(isActive ? tsActive : tsNormal);
}

void CPageItem::SetAlpha(BYTE alpha) {
	_pTitle->SetAlpha(alpha);
	_pImage->SetAlpha(alpha);
}

CCompent* CPageItem::GetHintCompent(int x, int y) {
	CCompent* pHint = NULL;
	if (pHint = _pTitle->GetHintCompent(x, y)) return pHint;
	if (pHint = _pImage->GetHintCompent(x, y)) return pHint;

	return CContainer::GetHintCompent(x, y);
}

//---------------------------------------------------------------------------
// class CPage
//---------------------------------------------------------------------------
CPage::CPage(CForm& frmOwn)
	: CCompent(frmOwn), _nIndex(-1), _eButtonPos(bpLeftUp), _nButtonMargin(0)
	  , _nButtonWidth(100), _nButtonHeight(30), evtSelectPage(NULL), _nMaxPage(0) {
	_pImage = new CGuiPic(this);
}

CPage::CPage(const CPage& rhs)
	: CCompent(rhs), _nMaxPage(0) {
	_pImage = new CGuiPic(this);
}

CPage& CPage::operator=(const CPage& rhs) {
	CCompent::operator =(rhs);

	return *this;
}

CPage::~CPage(void) {
	//delete _pImage;
	SAFE_DELETE(_pImage); // UI
}

void CPage::Init() {
	for (items::iterator it = _pages.begin(); it != _pages.end(); it++) {
		(*it)->Init();
	}

	_RefreshButtonPos();

	if (_nMaxPage > 0) {
		_pages[0]->_InActive(true);
		for (unsigned int i = 1; i < _nMaxPage; i++) {
			_pages[i]->_InActive(false);
		}
	}
}

void CPage::_RefreshButtonPos() {
	if (_nMaxPage <= 0) return;

	if (bpCustom == _eButtonPos) {
		for (items::iterator it = _pages.begin(); it != _pages.end(); it++) {
			(*it)->SetSize(GetWidth(), GetHeight());
		}
		return;
	}

	int cw = GetWidth();
	int ch = GetHeight() - _nButtonHeight;
	int bw = _nButtonWidth;
	int bh = _nButtonHeight;
	int sx = _nButtonMargin;
	switch (_eButtonPos) {
	case bpUpFull:
		bw = (GetWidth() - _nButtonMargin - _nButtonMargin) / _nMaxPage;
		break;
	case bpLeftUp:
		break;
	}

	for (items::iterator it = _pages.begin(); it != _pages.end(); it++) {
		(*it)->GetTitle()->SetSize(bw, bh);
		(*it)->GetTitle()->SetPos(sx, 0);
		sx += bw;

		(*it)->SetPos(0, 0);
		(*it)->SetSize(GetWidth(), GetHeight());
	}
}

void CPage::Render() {
	_pImage->Render(GetX(), GetY());

	items::iterator it = _pages.begin();
	for (; it != _pages.end(); it++)
		if ((*it)->GetIsShow())
			(*it)->Render();

	for (it = _pages.begin(); it != _pages.end(); it++)
		(*it)->RenderTitle();
}

void CPage::Refresh() {
	CCompent::Refresh();

	_pImage->Refresh();
	for (items::iterator it = _pages.begin(); it != _pages.end(); it++)
		(*it)->Refresh();
}

bool CPage::MouseRun(int x, int y, DWORD key) {
	if (!IsNormal()) return false;

	for (items::iterator it = _pages.begin(); it != _pages.end(); it++)
		if ((*it)->MouseRun(x, y, key))
			return true;

	return false;
}

void CPage::SetIndex(unsigned int n) {
	if (_nIndex == n) return;
	if (n >= _nMaxPage) return;

	for (unsigned int i = 0; i < _nMaxPage; i++) {
		_pages[i]->_InActive(n == i);
	}

	CCompent* p = CCompent::GetActive();
	if (!p || (p && !p->IsAllowActive())) {
		_frmOwn->SetNextActiveCompent();
	}

	_nIndex = n;

	if (evtSelectPage) evtSelectPage(this);
}

CPageItem* CPage::NewPage() {
	CPageItem* tmp = new CPageItem(*_frmOwn);
	tmp->SetParent(this);
	_pages.push_back(tmp);
	tmp->SetIndex((unsigned int)_pages.size() - 1);
	_nMaxPage = (unsigned int)_pages.size();
	return tmp;
}

void CPage::SetAlpha(BYTE alpha) {
	for (unsigned int i = 0; i < _nMaxPage; i++)
		_pages[i]->SetAlpha(alpha);

	_pImage->SetAlpha(alpha);
}

CCompent* CPage::GetHintCompent(int x, int y) {
	if (GetIsShow()) {
		if (InRect(x, y) && !_strHint.empty())
			return this;

		CCompent* pHint = NULL;
		for (unsigned int i = 0; i < _nMaxPage; i++) {
			if (pHint = _pages[i]->GetHintCompent(x, y)) {
				return pHint;
			}
		}
	}
	return NULL;
}

CCompent* CPage::GetHitCommand(int x, int y) {
	CCompent* pCommand = NULL;
	for (unsigned int i = 0; i < _nMaxPage; i++) {
		if (pCommand = _pages[i]->GetHitCommand(x, y)) {
			return pCommand;
		}
	}

	return NULL;
}
