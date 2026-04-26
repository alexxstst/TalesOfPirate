#include "StdAfx.h"
#include "UIText.h"
#include "uiGuidata.h"
#include "uiForm.h"
#include "uicompent.h"
#include "UIRender.h"

#include "uiformmgr.h"
#include "uitextparse.h"
using namespace GUI;

CItemObj::~CItemObj() {
	if (this == CGuiData::GetHintItem()) {
		CGuiData::SetHintItem(NULL);
	}
}

//---------------------------------------------------------------------------
// class CGuiData
//---------------------------------------------------------------------------
CGuiData::vcs CGuiData::_AllGui;
int CGuiData::_nMouseX = 0;
int CGuiData::_nMouseY = 0;
CCursor::eState CGuiData::_eCursor = CCursor::stNormal;

float CGuiData::_fDrawX = 1.0;
float CGuiData::_fDrawY = 1.0;
float CGuiData::_fScreenX = 1.0;
float CGuiData::_fScreenY = 1.0;

CItemObj* CGuiData::_pHintItem = NULL;

CGuiData::CGuiData()
	: _dwID(0), _nLeft(0), _nTop(0), _nWidth(0), _nHeight(0), _nX1(0), _nX2(0), _nY1(0), _nY2(0)
	  , _bShow(true), _bEnabled(true), _pParent(NULL), _pDrag(NULL), _IsMouseIn(false), nTag(0), _pVoid(NULL) {
	_AllGui.push_back(this);
	_dwID = (DWORD)(_AllGui.size());
}

CGuiData::CGuiData(const CGuiData& rhs)
	: _pParent(rhs._pParent), _nWidth(rhs._nWidth), _nHeight(rhs._nHeight), _nLeft(rhs._nLeft)
	  , _nTop(rhs._nTop), _nX1(rhs._nX1), _nX2(rhs._nX2), _nY1(rhs._nY1), _nY2(rhs._nY2)
	  , _bShow(rhs._bShow), _bEnabled(rhs._bEnabled), _strName(rhs._strName), nTag(rhs.nTag), _pVoid(rhs._pVoid)
	  , _pDrag(NULL), _IsMouseIn(false), _strHint(rhs._strHint) {
	_AllGui.push_back(this);
	_dwID = (DWORD)(_AllGui.size());

	if (!rhs._pDrag) {
		_pDrag = NULL;
	}
	else {
		_pDrag = new CDrag(*rhs._pDrag);
	}
}

CGuiData& CGuiData::operator=(const CGuiData& rhs) {
	// _Parent
	// _dwID
	_strName = rhs._strName;
	_nWidth = rhs._nWidth;
	_nHeight = rhs._nHeight;

	_nLeft = rhs._nLeft;
	_nTop = rhs._nTop;

	_nX1 = rhs._nX1;
	_nX2 = rhs._nX2;
	_nY1 = rhs._nY1;
	_nY2 = rhs._nY2;

	_bShow = rhs._bShow;
	_bEnabled = rhs._bEnabled;
	nTag = rhs.nTag;
	_pVoid = rhs._pVoid;

	_strHint = rhs._strHint;

	if (_pDrag) {
		delete _pDrag;
		_pDrag = NULL;
	}
	if (rhs._pDrag) _pDrag = new CDrag(*rhs._pDrag);

	_IsMouseIn = rhs._IsMouseIn;
	return *this;
}

CGuiData::~CGuiData() {
	//static char buf[256] = { 0 };
	//OutputDebugString( buf );

	if (_pDrag) {
		delete _pDrag;
		_pDrag = NULL;
	}

	_AllGui[_dwID - 1] = NULL;
}

void CGuiData::ClearAllGui() {
	for (vcs::iterator it = _AllGui.begin(); it != _AllGui.end(); it++) {
		//delete (*it);
		SAFE_DELETE(*it); // UI
	}
	_AllGui.clear();
}

void CGuiData::Refresh() {
	_nX1 = _nLeft;
	_nY1 = _nTop;
	CGuiData* pParent = _pParent;
	while (pParent) {
		_nX1 += pParent->GetLeft();
		_nY1 += pParent->GetTop();

		pParent = pParent->GetParent();
	}
	_nX2 = _nX1 + _nWidth;
	_nY2 = _nY1 + _nHeight;
}

void CGuiData::InitMemory() {
	for (vcs::iterator it = _AllGui.begin(); it != _AllGui.end(); it++) {
		(*it)->_AddForm();
	}
}

bool CGuiData::MouseRun(int x, int y, DWORD key) {
	if (!IsNormal()) return false;

	IsNoDrag(x, y, key);
	return _IsMouseIn;
}

void CGuiData::SetIsDrag(bool v) {
	if (v) {
		if (!_pDrag) _pDrag = new CDrag;
	}
	else {
		if (_pDrag) {
			delete _pDrag;
			_pDrag = NULL;
		}
	}
}

bool CGuiData::IsAllowActive() {
	if (!IsNormal()) return false;

	CGuiData* pParent = _pParent;
	while (pParent) {
		if (!pParent->IsNormal())
			return false;

		pParent = pParent->GetParent();
	}
	return true;
}

void CGuiData::RenderHint(int x, int y) {
	ui::FrameRender(_strHint.c_str(), x + 40, y);
}

CItemObj* pLastObj = NULL;
DWORD dwLastObjColor = 0;

bool CGuiData::SetHintItem(CItemObj* pObj) {
	if (pObj && pObj->HasHint()) {
		pLastObj = pObj;
		if (pObj->GetColor() != 4294901760) {
			dwLastObjColor = pObj->GetColor();
			pObj->SetColor((DWORD)0xffaaaaaa);
		}
		_pHintItem = pObj;
		return true;
	}
	else {
		if (pLastObj && pLastObj->GetColor() != 4294901760) {
			pLastObj->SetColor(dwLastObjColor);
			pLastObj = NULL;
		}
		pLastObj = NULL; //fix npcs crashing -- mothanankh 
		_pHintItem = NULL;
		return false;
	}
}

//---------------------------------------------------------------------------
// class CDrag
//---------------------------------------------------------------------------
CDrag* CDrag::_pDrag = NULL;
CGuiData* CDrag::_pParent = NULL;

DWORD CDrag::_dwGridWidth = 4;
DWORD CDrag::_dwGridHeight = 4;

int CDrag::_nDragX = 0;
int CDrag::_nDragY = 0;
int CDrag::_nStartX = 0;
int CDrag::_nStartY = 0;

CCursor::eState CDrag::_crNormal = CCursor::stNormal;

CDrag::CDrag()
	: evtMouseDragBegin(NULL), evtMouseDragMove(NULL), evtMouseDragEnd(NULL)
	  , _eState(stNone), _nYareLen(2), _IsMove(true), _IsUseGrid(true), _crDragIn(CCursor::stEnd),
	  _crDrag(CCursor::stDrag) {
}

void CDrag::SetSnapToGrid(DWORD dwWidth, DWORD dwHeight) {
	if (dwWidth > 0) {
		_dwGridWidth = dwWidth;
	}
	if (dwHeight > 0) {
		_dwGridHeight = dwHeight;
	}
}

//---------------------------------------------------------------------------
// class CGuiTime
//---------------------------------------------------------------------------
CGuiTime::times CGuiTime::_times;

CGuiTime* CGuiTime::Find(DWORD id) {
	for (times::iterator it = _times.begin(); it != _times.end(); it++)
		if (!(*it)->_IsRelease && (*it)->GetID() == id)
			return (*it);

	return NULL;
}

CGuiTime* CGuiTime::Create(DWORD dwInterval, GuiTimerEvent evt) {
	CGuiTime* p = NULL;
	for (times::iterator it = _times.begin(); it != _times.end(); it++) {
		if ((*it)->_IsRelease) {
			p = (*it);
		}
	}
	if (!p) {
		p = new CGuiTime;
		_times.push_back(p);
	}
	p->SetInterval(dwInterval);
	p->evtTime = evt;
	p->_dwLastTime = GetTickCount();
	p->_IsRelease = false;
	return p;
}

bool CGuiTime::Release() {
	_IsRelease = true;
	return true;
}

CGuiTime::CGuiTime(DWORD dwInterval, GuiTimerEvent evt)
	: _nEventID(0), _lpData(0), evtTime(evt), _dwInterval(dwInterval) {
	static int n = 0;
	n++;
	_dwIndex = n;
}

CGuiTime::~CGuiTime() {
}

void CGuiTime::FrameMove(DWORD dwTime) {
	for (times::iterator it = _times.begin(); it != _times.end(); it++) {
		(*it)->OnTime(dwTime);
	}
}
