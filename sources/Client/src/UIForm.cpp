//------------------------------------------------------------------------
//	2005/3/26	Arcol	BUG
//	2005/3/24	Arcol	
//	2005/4/13	Arcol	,
//	2005/5/17	Arcol	
//------------------------------------------------------------------------
#include "StdAfx.h"
#include "uiform.h"
#include "uicompent.h"
#include "uiformmgr.h"
#include "gameapp.h"
#include "uimenu.h"
#include "atltypes.h"
#include "uitextbutton.h"

using namespace GUI;

CForm* CForm::_pActive = NULL;
int CForm::_nDragOffX = 0;
int CForm::_nDragOffY = 0;

CForm::CForm()
	: CGuiData(), _isModal(false), _nActiveCompentID(0), _modalResult(mrNone), _nOffWidth(0), _nOffHeight(0)
	  , evtShow(NULL), evtHide(NULL), evtActive(NULL), evtLost(NULL), evtMouseDown(NULL), evtHotkeyHandler(NULL)
	  , evtEntrustMouseEvent(NULL), evtKeyDown(NULL), evtKeyChar(NULL), _cHotKey(0), _formStyle(fsNone),
	  _IsEscClose(true)
	  , evtMouseDragEnd(NULL), evtOnSetScreen(NULL), _pPointer(NULL), _pPopMenu(NULL)
	  , evtClose(NULL), _pEnterButton(NULL), evtBeforeShow(NULL), evtEscClose(NULL) {
	CFormMgr::s_Mgr._AddMemory(this);
	_pImage = new CFramePic(this);

	_bShow = false;
}

CForm& CForm::operator=(const CForm& rhs) {
	CGuiData::operator =(rhs);

	*_pImage = *rhs._pImage;
	_Copy(&rhs);
	return *this;
}

CForm::CForm(const CForm& rhs)
	: CGuiData(rhs), _isModal(rhs._isModal), _pImage(new CFramePic(*rhs._pImage)), _cHotKey(0) {
	_strName += " Copy";
	CFormMgr::s_Mgr._AddMemory(this);

	_Copy(&rhs);
}

void CForm::_Copy(const CForm* rhs) {
	evtShow = rhs->evtShow;
	evtHide = rhs->evtHide;
	evtActive = rhs->evtActive;
	evtLost = rhs->evtLost;
	evtMouseDown = rhs->evtMouseDown;
	evtKeyDown = rhs->evtKeyDown;
	evtKeyChar = rhs->evtKeyChar;
	evtOnSetScreen = rhs->evtOnSetScreen;
	evtMouseDragEnd = rhs->evtMouseDragEnd;
	evtHotkeyHandler = rhs->evtHotkeyHandler;
	evtClose = rhs->evtClose;
	evtBeforeShow = rhs->evtBeforeShow;
	evtEscClose = rhs->evtEscClose;

	evtEntrustMouseEvent = rhs->evtEntrustMouseEvent;

	_IsEscClose = rhs->_IsEscClose;
	_nOffWidth = _nOffWidth;
	_nOffHeight = rhs->_nOffHeight;

	_pEnterButton = rhs->_pEnterButton;
	_pPointer = NULL;

	_pImage->SetParent(this);

	_modalResult = rhs->_modalResult;
	_formStyle = rhs->_formStyle;

	_bShow = false;
	if (_pDrag) {
		_pDrag->evtMouseDragBegin = _DragMouseEventBegin;
		_pDrag->evtMouseDragMove = _DragMouseEvent;
		_pDrag->evtMouseDragEnd = _DragMouseEvent;
	}

	_nActiveCompentID = 0;
	_cHotKey = 0;
	_pPointer = 0;
	_isModal = false;
	_pPopMenu = NULL;

	_CopyCompent(rhs);
}

CForm::~CForm() {
	//delete _pImage;
	SAFE_DELETE(_pImage); // UI
	_TempleteClear();

	CFormMgr::s_Mgr._DelMemory(this);
}

bool CForm::_AddFrameMove(CCompent* c, bool isCheck) {
	if (isCheck) {
		vcs::iterator it = find(_frames.begin(), _frames.end(), c);
		if (_frames.end() == it) {
			_frames.push_back(c);
			return true;
		}
		return false;
	}
	else {
		_frames.push_back(c);
	}
	return true;
}

bool CForm::_AddCompent(CCompent* c, bool isCheck) {
	if (isCheck) {
		vcs::iterator it = find(_allCompents.begin(), _allCompents.end(), c);
		if (_allCompents.end() == it) {
			_allCompents.push_back(c);
		}

		if (c->GetParent() == c->GetForm()) {
			vcs::iterator it = find(_compents.begin(), _compents.end(), c);
			if (_compents.end() == it) {
				_compents.push_back(c);

				if (c->IsHandleMouse() || c->_IsFocus) _mouse.push_back(c);
				if (c->_IsFocus) _actives.push_back(c);
			}
		}
	}
	else {
		_allCompents.push_back(c);
		if (c->GetParent() == c->GetForm()) {
			_compents.push_back(c);
			if (c->IsHandleMouse() || c->_IsFocus) _mouse.push_back(c);
			if (c->_IsFocus) _actives.push_back(c);
			return true;
		}
	}
	return false;
}

CCompent* CForm::FindActiveCompent() {
	if (_actives.empty()) return NULL;

	if (_actives[_nActiveCompentID]->IsAllowActive()) {
		return _actives[_nActiveCompentID];
	}

	for (vcs::iterator it = _actives.begin(); it != _actives.end(); ++it) {
		if ((*it)->IsNormal()) {
			return *it;
		}
	}
	return NULL;
}

void CForm::_CopyCompent(const CForm* rhs) {
	CCompent* p = NULL;
	for (vcs::const_iterator it = rhs->_allCompents.begin(); it != rhs->_allCompents.end(); it++) {
		p = dynamic_cast<CCompent*>((*it)->Clone());
		if (p) {
			p->SetForm(this);
			p->SetParent(this);

			p->_AddForm();
		}
	}
}

void CForm::Init() {
	if (_pParent) {
		CForm* rhs = dynamic_cast<CForm*>(_pParent);
		_pParent = NULL;

		if (!rhs) return;
		_CopyCompent(rhs);
	}

	OnSetScreen();

	for (vcs::iterator it = _compents.begin(); it != _compents.end(); it++) {
		(*it)->Init();
	}

	if (_pEnterButton && _pEnterButton->GetForm() != this) {
		_pEnterButton = dynamic_cast<CTextButton*>(Find(_pEnterButton->GetName()));
	}
}

void CForm::OnSetScreen() {
	int sw, sh; // 
	RECT rc;
	::GetClientRect(g_pGameApp->GetHWND(), &rc);
	sw = rc.right - rc.left;
	sh = rc.bottom - rc.top;


	if (GetLeft() > sw) {
		SetPos(sw - GetWidth(), GetTop());
		_nOffWidth = 0;
	}
	if (GetTop() > sh) {
		SetPos(GetLeft(), sh - GetHeight());
		_nOffHeight = 0;
	}

	//
	switch (_formStyle) {
	case fsNone:
		SetPos(GetLeft(), GetTop());
		_nOffWidth = 0;
		_nOffHeight = 0;
		break;
	case fsAllCenter:
		SetPos(sw / 2 - GetWidth() / 2, sh / 2 - GetHeight() / 2);
		_nOffWidth = 0;
		_nOffHeight = 0;
		break;
	case fsXCenter:
		SetPos(sw / 2 - GetWidth() / 2, GetTop());
		_nOffWidth = 0;
		_nOffHeight = 0;
		break;
	case fsYCenter:
		SetPos(GetLeft(), sh / 2 - GetHeight() / 2);
		_nOffWidth = 0;
		_nOffHeight = 0;
		break;
	case fsLeft:
		SetPos(_nOffWidth, GetTop()); //OK
		break;
	case fsRight:
		SetPos(sw - GetWidth() - _nOffWidth, GetTop()); //OK
		break;
	case fsTop:
		SetPos(GetLeft(), _nOffHeight); //OK
		break;
	case fsBottom:
		SetPos(GetLeft(), sh - GetHeight() - _nOffHeight); //OK
		break;
	case fsLeftTop:
		SetPos(_nOffWidth, _nOffHeight); //OK
		break;
	case fsRightTop:
		SetPos(sw - GetWidth() - _nOffWidth, _nOffHeight); //OK
		break;
	case fsLeftBottom:
		SetPos(_nOffWidth, sh - GetHeight() - _nOffHeight); //OK
		break;
	case fsRightBottom:
		SetPos(sw - GetWidth(), sh - GetHeight());
		SetPos(sw - GetWidth() - _nOffWidth, sh - GetHeight() - _nOffHeight); //OK
		break;
	default:
		SetPos(GetLeft(), GetTop());
		break;
	}

	if (evtOnSetScreen) {
		evtOnSetScreen(this);
	}
}

//void CForm::OnSetScreen()
//{
//	int sw, sh;     // 
//	RECT rc;
//	::GetClientRect( g_pGameApp->GetHWND(), &rc );
//	sw = rc.right  - rc.left;
//	sh = rc.bottom - rc.top;
//
//	//
//	switch( _formStyle)
//	{
//	case fsNone:
//		SetPos( GetLeft(), GetTop() );
//		break;
//	case fsAllCenter:
//		SetPos( sw / 2 - GetWidth()/2 ,sh / 2 - GetHeight()/2 );		
//		break;
//	case fsXCenter:
//		SetPos(  sw /2 - GetWidth()/2,GetTop() );
//		break;
//	case	fsYCenter:
//		SetPos(  GetLeft() ,sh /2 - GetHeight()/2 );
//		break;
//	case	fsLeft:
//		SetPos( 0 , GetTop() );
//		break;
//	case	fsRight:
//		SetPos(  sw - GetWidth(),GetTop() );
//		break;
//	case	fsTop:
//		SetPos(  GetLeft(),0  );
//		break;
//	case	fsBottom:
//		SetPos( GetLeft(), sh  -GetHeight() );
//		break;
//	case	fsLeftTop:
//		SetPos(  0 ,0 );
//		break;
//	case    fsRightTop:
//		SetPos( sw - GetWidth() , 0  );
//		break;
//	case	fsLeftBottom:
//		SetPos( 0 , sh  - GetHeight() );
//		break;
//	case	fsRightBottom:
//		SetPos(  sw - GetWidth() ,sh  -GetHeight() );
//		break;
//	default:
//		SetPos( GetLeft(), GetTop() );
//		break;
//	}
//
//	if( evtOnSetScreen )
//	{
//		evtOnSetScreen( this );		
//	}
//}

void CForm::_TempleteInit() {
	if (_cHotKey > 0) {
		//RegisterHotKey( GetHWND(), _cHotKey, MOD_ALT, _cHotKey );
	}
}

void CForm::_TempleteClear() {
	if (_cHotKey > 0) {
		//UnregisterHotKey( GetHWND(), _cHotKey );
	}
}


void CForm::SetStyle(eFormStyle index, int offWidth, int offHeight, bool bRedraw) {
	_nOffWidth = offWidth;
	_nOffHeight = offHeight;
	switch (index) {
	case 0:
		_formStyle = fsNone;
	case 1:
		_formStyle = fsAllCenter;
		break;
	case 2:
		_formStyle = fsXCenter;
		break;
	case 3:
		_formStyle = fsYCenter;
		break;
	case 4:
		_formStyle = fsLeft;
		break;
	case 5:
		_formStyle = fsRight;
		break;
	case 6:
		_formStyle = fsTop;
		break;
	case 7:
		_formStyle = fsBottom;
		break;
	case 8:
		_formStyle = fsLeftTop;
		break;
	case 9:
		_formStyle = fsRightTop;
		break;
	case 10:
		_formStyle = fsLeftBottom;
		break;
	case 11:
		_formStyle = fsRightBottom;
		break;
	default:
		_formStyle = fsNone;
		break;
	}
	if (bRedraw) {
		OnSetScreen();
		Refresh();
	}
}

void CForm::FrameMove(int nCount) {
	for (vcs::iterator it = _frames.begin(); it != _frames.end(); it++) {
		if ((*it)->GetIsShow()) {
			(*it)->FrameMove(nCount);
		}
	}
}

void CForm::Render() {
	_pImage->Render();

	for (vcs::iterator it = _compents.begin(); it != _compents.end(); it++) {
		if ((*it)->GetIsShow())
			(*it)->Render();
	}

	for (vfrm::iterator it = _childs.begin(); it != _childs.end(); it++) {
		if ((*it)->GetIsShow())
			(*it)->Render();
	}

	if (_pPopMenu && _pPopMenu->GetIsShow()) {
		_pPopMenu->Render();
	}
}

void CForm::Refresh() {
	CGuiData::Refresh();

	_pImage->Refresh();

	for (vcs::iterator it = _compents.begin(); it != _compents.end(); it++)
		(*it)->Refresh();

	for (vfrm::iterator it = _childs.begin(); it != _childs.end(); it++) {
		(*it)->Refresh();
	}
}

void CForm::SetActiveCompent(CCompent* pActive) {
	int nCount = (int)_actives.size();
	for (int i = 0; i < nCount; i++) {
		if (_actives[i] == pActive) {
			if (pActive->IsAllowActive())
				_nActiveCompentID = i;
			return;
		}
	}
}

bool CForm::SetNextActiveCompent(bool isNext) {
	if (_actives.empty()) {
		CCompent::SetActive(NULL);
		return false;
	}

	int n = 0;
	int max = (int)_actives.size();

	// 
	if (isNext) {
		do {
			_nActiveCompentID++;
			if (_nActiveCompentID >= max)
				_nActiveCompentID = 0;
			n++;
		}
		while (!_actives[_nActiveCompentID]->IsAllowActive() && n < max);
	}
	else {
		do {
			_nActiveCompentID--;
			if (_nActiveCompentID < 0)
				_nActiveCompentID = max - 1;
			n++;
		}
		while (!_actives[_nActiveCompentID]->IsAllowActive() && n < max);
	}

	if (n < max) {
		CCompent::SetActive(_actives[_nActiveCompentID]);
		return true;
	}
	else {
		CCompent::SetActive(NULL);
		return false;
	}
}

bool CForm::OnChar(char key) {
	if (!IsNormal()) return false;

	if (evtKeyChar && evtKeyChar(this, key)) return false;

	switch (key) {
	case '\t':
		SetNextActiveCompent(GetAsyncKeyState(VK_LSHIFT) != 0);
		return true;
	case '\r':
		if (_pEnterButton && _pEnterButton->IsNormal()) {
			_pEnterButton->DoClick();
			return true;
		}
		break;
	case VK_ESCAPE:
		if (_IsEscClose) {
			Close();
		}
		break;
	}

	return false;
}

bool CForm::OnKeyDown(int key) {
	if (!IsNormal()) return false;

	if (evtKeyDown && evtKeyDown(this, key)) return false;

	return false;
}

bool CForm::MenuMouseRun(int x, int y, DWORD key) {
	if (!_pPopMenu || !GetIsShow()) return false;

	if (_pPopMenu->MouseRun(x, y, key)) {
		return true;
	}
	return false;
}

bool CForm::MouseRun(int x, int y, DWORD key) {
	if (!IsNormal()) return false;

	for (vfrm::reverse_iterator rit = _childs.rbegin(); rit != _childs.rend(); rit++) {
		if ((*rit)->MouseRun(x, y, key))
			return true;
	}

	for (vcs::reverse_iterator it = _mouse.rbegin(); it != _mouse.rend(); it++) {
		if ((*it)->MouseRun(x, y, key)) {
			if (!CCompent::_pLastMouseCompent) CCompent::_pLastMouseCompent = *it;
			if (IsNormal() && (key & Mouse_LDown))
				_ActiveForm(key);
			return true;
		}
	}

	if (InRect(x, y) && (key & Mouse_LDown) && IsNormal()) {
		_ActiveForm(key);
	}

	if (!GetParent() && _pDrag && _pDrag->BeginMouseRun(this, _IsMouseIn, x, y, key)) {
		return true;
	}
	return _IsMouseIn;
}

CCompent* CForm::GetHitCommand(int x, int y) {
	vcs::iterator it = _mouse.begin();
	vcs::const_iterator end = _mouse.end();
	CCompent* pCommand = NULL;
	for (it = _mouse.begin(); it != end; it++) {
		if (pCommand = (*it)->GetHitCommand(x, y)) {
			return pCommand;
		}
	}

	return NULL;
}

CGuiData* CForm::GetHintGui(int x, int y) {
	vcs::iterator it = _compents.begin();
	vcs::const_iterator end = _compents.end();
	CCompent* pHint = NULL;
	for (it = _compents.begin(); it != end; it++) {
		if (pHint = (*it)->GetHintCompent(x, y))
			return pHint;
	}

	if (!_strHint.empty())
		return this;

	return NULL;
}

void CForm::_OnActive() {
	if (_pActive == this) return;

	if (!_actives.empty() && _actives[_nActiveCompentID]->IsAllowActive()) {
		CCompent::SetActive(_actives[_nActiveCompentID]);
	}
	else {
		if (CCompent::GetActive()) {
			if (!CCompent::GetActive()->GetIsShow())
				CCompent::SetActive(NULL);
		}
	}

	if (_pActive && _pActive != this) {
		if (_pActive->evtLost) _pActive->evtLost(this);

		// 
		if (!_pActive->GetIsModal() && !GetIsModal()) {
			CFormMgr::s_Mgr._UpdataShowForm(this);
		}
	}

	if (evtActive) evtActive(this);

	_pActive = this;
}

void CForm::ShowModal() {
	if (GetIsShow()) return;

	if (GetParent()) {
		_bShow = true;
		return;
	}

	if (evtBeforeShow) {
		bool IsShow = true;
		evtBeforeShow(this, IsShow);
		if (!IsShow) return;
	}

	_isModal = true;
	_modalResult = mrNone;
	CFormMgr::s_Mgr._ShowModal(this);

	_bShow = true;

	_OnActive();

	if (evtShow) evtShow(this);
}

void CForm::Show() {
	if (GetParent()) {
		_bShow = true;
		return;
	}

	if (evtBeforeShow) {
		bool IsShow = true;
		evtBeforeShow(this, IsShow);
		if (!IsShow) return;
	}

	if (GetIsShow()) {
		CFormMgr::s_Mgr._UpdataShowForm(this);
		return;
	}

	_isModal = false;
	_bShow = true;

	CFormMgr::s_Mgr._AddShowForm(this);

	_OnActive();
	if (evtShow) evtShow(this);
}

void CForm::Hide() {
	if (GetParent()) {
		_bShow = false;
		return;
	}

	if (!GetIsShow()) return;

	if (evtClose) {
		bool IsClose = false;
		evtClose(this, IsClose);
		if (IsClose) return;
	}

	_bShow = false;

	if (_isModal) {
		CFormMgr::s_Mgr._ModalClose(this);
		_modalResult = mrNone;
	}
	else {
		CFormMgr::s_Mgr._DelShowForm(this);

		CForm* f = NULL;
		if (CCompent::GetActive()) {
			f = CCompent::GetActive()->GetForm();
			if (f == this) {
				CCompent::SetActive(NULL);
			}
		}

		if (_pActive == this) {
			CFormMgr::s_Mgr._SetNewActiveForm();
		}
	}

	if (evtHide) evtHide(this);
}

CCompent* CForm::Find(const char* str) {
	for (vcs::iterator it = _allCompents.begin(); it != _allCompents.end(); it++) {
		if (strcmp((*it)->GetName(), str) == 0) {
			return *it;
		}
	}

	return NULL;
}

void CForm::ForEach(CompentFun pFun) {
	int nIndex = 0;
	for (vcs::iterator it = _compents.begin(); it != _compents.end(); it++)
		pFun(*it, nIndex++);
}

void CForm::SetAlpha(BYTE alpha) {
	_pImage->SetAlpha(alpha);
	for (vcs::iterator it = _compents.begin(); it != _compents.end(); it++)
		(*it)->SetAlpha(alpha);
}

void CForm::SetIsDrag(bool v) {
	CGuiData::SetIsDrag(v);

	if (_pDrag) {
		_pDrag->evtMouseDragBegin = _DragMouseEventBegin;
		_pDrag->evtMouseDragMove = _DragMouseEvent;
		_pDrag->evtMouseDragEnd = _DragMouseEvent;
	}
}

void CForm::_DragMouseEvent(CGuiData* pSender, int x, int y, DWORD key) {
	CForm* frm = dynamic_cast<CForm*>(pSender);
	frm->SetPos(CDrag::GetStartX() + CDrag::GetDragX() + _nDragOffX,
				CDrag::GetStartY() + CDrag::GetDragY() + _nDragOffY);

	if (frm) frm->Refresh();

	if (frm && frm->evtMouseDragEnd) {
		frm->evtMouseDragEnd(pSender, x, y, key);
	}
}

void CForm::_DragMouseEventBegin(CGuiData* pSender, int x, int y, DWORD key) {
	_nDragOffX = pSender->GetX() - CDrag::GetStartX();
	_nDragOffY = pSender->GetY() - CDrag::GetStartY();

	_DragMouseEvent(pSender, x, y, key);
}

void CForm::PopMenu(CMenu* pMenu, int x, int y) {
	_pPopMenu = pMenu;
	if (_pPopMenu) {
		if (_pPopMenu->GetParent() && _pPopMenu->GetParent() != this) {
			CForm* old_form = (CForm*)_pPopMenu->GetParent();
			old_form->PopMenu(NULL);
		}

		// Hide disabled menu items 
		for (int i = 0; i < _pPopMenu->GetCount(); ++i) {
			CMenuItem* item = _pPopMenu->GetMenuItem(i);
			if (!item->GetIsEnabled()) {
				item->SetIsHide(true);
			}
		}

		_pPopMenu->SetParent(this);
		_pPopMenu->SetPos(x, y);
		_pPopMenu->SetIsShow(true);
		_pPopMenu->Refresh();
	}
}

void CForm::SetParent(CGuiData* p) {
	if (p == this) return;

	Close();

	if (!p || dynamic_cast<CForm*>(p)) {
		CForm* frm = dynamic_cast<CForm*>(_pParent);
		if (frm) {
			frm->_childs.remove(this);
		}

		_pParent = p;

		frm = dynamic_cast<CForm*>(_pParent);
		if (frm) {
			frm->_childs.push_back(this);
		}
	}
}

int CForm::ClearChild() {
	int i = 0;
	while (!_childs.empty()) {
		_childs.back()->SetParent(NULL);
		i++;
	}
	return i;
}

void CForm::Reset() {
	for (vcs::iterator it = _compents.begin(); it != _compents.end(); it++)
		(*it)->Reset();
}
