#include "StdAfx.h"

#include "UIText.h"
#include "StringLib.h"
#include "uiformmgr.h"
#include "uiedit.h"
#include "UIRender.h"
#include "UIImeInput.h"
#include "uitextbutton.h"
#include "UICommand.h"
#include "uimenu.h"

using namespace GUI;

using namespace std;

const int START_HINT = 5;

CFormMgr CFormMgr::s_Mgr;

eMouseAction CFormMgr::_eMouseAction = enumMA_Gui;

bool CFormMgr::_IsDebugMode = false; //  -added by Arcol
bool CFormMgr::_IsDrawFrameInDebugMode = true; //  -added by Arcol
bool CFormMgr::_IsDrawBackgroundInDebugMode = true; //  -added by Arcol

CFormMgr::CFormMgr()
	: _bEnabled(false), _bInit(false), _nEnableHotKey(0xFFFFFFFF), _nTempleteNo(-1), /*_nMouseHover(0),*/
	  _pHintGui(NULL) // 
{
	_forms = &_defaulttemplete;
}

CFormMgr::~CFormMgr() {
	Clear();
}

bool CFormMgr::SwitchTemplete(int n) {
	if (n >= (int)GetFormTempleteMax() || n < -1) return false;

	if (_nTempleteNo == n) return true;

	_pHintGui = NULL;

	_nTempleteNo = n;
	if (n == -1) {
		_forms = &_defaulttemplete;
		_InitFormID();
		return true;
	}

	if (_forms != _showforms[n]) {
		_forms = _showforms[n];
		_InitFormID();
		return true;
	}

	return false;
}

bool CFormMgr::SetFormTempleteMax(int n) {
	if (n >= 0) {
		for (frmtemplete::iterator it = _showforms.begin(); it != _showforms.end(); ++it) {
			//delete *it;
			SAFE_DELETE(*it); // UI
		}
		_showforms.resize(n);
		for (int i = 0; i < n; i++) {
			_showforms[i] = new vfrm;
		}
		_forms = &_defaulttemplete;

		if (CDrag::GetDrag()) CDrag::GetDrag()->Reset();
		return true;
	}
	return false;
}

void CFormMgr::_InitFormID() {
	CCompent::SetActive(NULL);

	for (vfrm::iterator it = _allForms.begin(); it != _allForms.end(); ++it) {
		(*it)->_TempleteClear();
	}

	int id = 0;
	_show.clear();
	for (vfrm::iterator it = _forms->begin(); it != _forms->end(); ++it) {
		(*it)->_TempleteInit();
		id++;

		if ((*it)->GetIsShow()) {
			_show.push_back(*it);
		}
	}

	_modal.clear();

	_SetNewActiveForm();
}

void CFormMgr::ResetAllForm() {
	for (vfrm::iterator it = _allForms.begin(); it != _allForms.end(); it++)
		(*it)->_bShow = false;

	_show.clear();

	CCompent::SetActive(NULL);
}

bool CFormMgr::_DelMemory(CForm* form) {
	_DelForm(_allForms, form);
	_DelForm(_modal, form);
	_DelForm(_show, form);
	_DelForm(_defaulttemplete, form);
	for (frmtemplete::iterator it = _showforms.begin(); it != _showforms.end(); ++it) {
		_DelForm(**it, form);
	}
	return true;
}

void CFormMgr::_DelForm(vfrm& list, CForm* frm) {
	list.remove(frm);
}

bool CFormMgr::_AddMemory(CForm* form) {
	// 
	for (vfrm::iterator it = _allForms.begin(); it != _allForms.end(); ++it) {
		if (stricmp(form->GetName(), (*it)->GetName()) == 0) {
			{
				g_logManager.InternalLog(LogLevel::Debug, "common",
										 SafeVFormat(GetLanguageString(574), form->GetName()));
			}
			return false;
		}
	}

	// 
	if (_allForms.end() == find(_allForms.begin(), _allForms.end(), form)) {
		_allForms.push_back(form);
		if (!form->GetIsModal()) {
			_defaulttemplete.push_back(form);
		}
		return true;
	}
	return false;
}

bool CFormMgr::AddForm(CForm* form, int templete) {
	if (templete >= (int)GetFormTempleteMax() || templete < 0) return false;

	if (form->GetIsModal()) return false;

	vfrm* f = _showforms[templete];
	if (f->end() == find(f->begin(), f->end(), form)) {
		f->push_back(form);
		return true;
	}
	return false;
}

void CFormMgr::FrameMove(int x, int y, DWORD dwMouseKey, DWORD dwTime) {
	if (!_bEnabled) {
		_eMouseAction = enumMA_None;
		return;
	}

	CGuiTime::FrameMove(dwTime);

	static DWORD dwNextTime = 0;
	if (dwTime >= dwNextTime) {
		dwNextTime = dwTime + 100;
		for (vfrm::iterator it = _modal.begin(); it != _modal.end(); it++) {
			(*it)->FrameMove(dwTime);
		}
		for (vfrm::reverse_iterator rit = _show.rbegin(); rit != _show.rend(); ++rit) {
			(*rit)->FrameMove(dwTime);
		}
	}

	if (dwMouseKey) {
		_MouseRun(x, y, dwMouseKey);
	}


	_pHintGui = NULL;

	CGuiData::SetHintItem(NULL);
	CForm* frm = GetHitForm(x, y);
	if (frm) {
		_pHintGui = frm->GetHintGui(x, y);
	}
	CCompent* p = dynamic_cast<CCompent*>(_pHintGui);
	if (p) {
		if (CGuiData::GetHintItem()) {
			CGuiData::GetHintItem()->ReadyForHint(x, y, p);
		}
	}


	if (CCursor::stEnd != CGuiData::GetCursor()) {
		CCursor::I()->SetFrame(CGuiData::GetCursor());
	}
	else {
		CCursor::I()->Restore();
	}
}

void CFormMgr::RenderHint(int x, int y) {
	if (_pHintGui) {
		GetRender().ScreenConvert(x, y);

		if (CGuiData::GetHintItem()) {
			CGuiData::GetHintItem()->RenderHint(x, y);
			return;
		}

		_pHintGui->RenderHint(x, y);
	}
}

bool CFormMgr::_MouseRun(int x, int y, DWORD mouse) {
	_eMouseAction = enumMA_None;
	GetRender().ScreenConvert(x, y);

	CGuiData::SetCursor(CCursor::stEnd);

	CGuiData::SetMousePos(x, y);

	if (!_OnMouseRun.empty()) {
		for (vmouses::iterator it = _OnMouseRun.begin(); it != _OnMouseRun.end(); ++it)
			(*it)(x, y, mouse);
	}

	CCompent::_pLastMouseCompent = NULL;
	if (CDrag::GetDrag()) {
		CDrag::GetDrag()->MouseRun(x, y, mouse);
		_eMouseAction = enumMA_Gui;
		return true;
	}

	if (!_modal.empty()) {
		static CForm* p = NULL;

		// 
		p = _modal.back();
		p->MouseRun(x, y, mouse);
		if (CForm::mrNone != p->GetModalResult()) {
			p->Hide();
		}
		_eMouseAction = enumMA_Gui;
		return true;
	}

	for (vfrm::reverse_iterator rit = _show.rbegin(); rit != _show.rend(); ++rit) {
		if ((*rit)->MenuMouseRun(x, y, mouse)) {
			_eMouseAction = enumMA_Gui;
			return true;
		}
	}

	if (mouse & Mouse_Down) {
		CMenu::CloseAll();
	}

	for (vfrm::reverse_iterator rit = _show.rbegin(); rit != _show.rend(); ++rit) {
		if ((*rit)->MouseRun(x, y, mouse)) {
			_eMouseAction = enumMA_Gui;
			if (CCompent::GetLastMouseCompent()) {
				_eMouseAction = CCompent::GetLastMouseCompent()->GetMouseAction();
			}
			return _eMouseAction == enumMA_Gui;
		}
	}

	return false;
}

void CFormMgr::_ModalClose(CForm* form) {
	vfrm::iterator it = find(_modal.begin(), _modal.end(), form);
	if (it != _modal.end()) {
		_modal.erase(it);
		_SetNewActiveForm();
		return;
	}

	ToLogService("common", "msgCFormMgr::_ModalClose({}) not find this form", form->GetName());
}

void CFormMgr::_SetNewActiveForm() {
	_ActiveCompent();

	if (CCompent::GetActive()) {
		if (CCompent::GetActive()->GetForm() != CForm::GetActive())
			CCompent::GetActive()->GetForm()->_OnActive();
	}
	else {
		if (!_modal.empty()) {
			_modal.back()->_OnActive();
			return;
		}

		for (vfrm::iterator it = _show.begin(); it != _show.end(); ++it) {
			if ((*it)->IsNormal()) {
				(*it)->_OnActive();
				break;
			}
		}
	}
}

CForm* CFormMgr::GetHitForm(int x, int y) {
	for (vfrm::reverse_iterator it = _show.rbegin(); it != _show.rend(); ++it) {
		vfrm& childs = (*it)->_childs;
		if (!childs.empty()) {
			for (vfrm::iterator ch = childs.begin(); ch != childs.end(); ch++)
				if ((*ch)->GetIsShow() && (*ch)->InRect(x, y))
					return (*ch);
		}

		if ((*it)->GetIsShow() && (*it)->InRect(x, y)) {
			return (*it);
		}
	}

	return NULL;
}

bool CFormMgr::MouseScroll(int nScroll) {
	if (!_bEnabled) return false;

	if (!_OnMouseScroll.empty()) {
		for (vscrolls::iterator it = _OnMouseScroll.begin(); it != _OnMouseScroll.end(); ++it)
			if ((*it)(nScroll))
				return true;
	}

	if (CCompent::_pLastMouseCompent) return CCompent::_pLastMouseCompent->MouseScroll(nScroll);
	return false;
}

bool CFormMgr::OnKeyDown(int key) {
	if (!_bEnabled) return false;

	if (!_OnKeyDown.empty()) {
		vkeydowns::const_iterator end = _OnKeyDown.end();
		for (vkeydowns::const_iterator it = _OnKeyDown.begin(); it != end; ++it)
			if ((*it)(key))
				return true;
	}

	static CCompent* p = NULL;
	static CForm* f = NULL;

	f = CForm::GetActive();
	if (f && f->OnKeyDown(key)) return true;

	p = CCompent::GetActive();
	if (p && p->OnKeyDown(key)) return true;

	return true;
}

bool CFormMgr::OnKeyChar(char key) {
	if (!_bEnabled) return false;

	if (!_OnKeyChar.empty()) {
		vkeychars::const_iterator end = _OnKeyChar.end();
		for (vkeychars::const_iterator it = _OnKeyChar.begin(); it != end; ++it)
			if ((*it)(key))
				return true;
	}

	static CCompent* p = NULL;
	static CForm* f = NULL;

	f = CForm::GetActive();
	if (f && f->OnChar(key)) return true;

	p = CCompent::GetActive();
	if (p && p->OnChar(key)) return true;

	return false;
}

void CFormMgr::Render() {
	if (!_bEnabled) {
		ShowDebugInfo(); //	-added by Arcol
		return;
	}

	vfrm::iterator it;
	for (it = _show.begin(); it != _show.end(); it++)
		(*it)->Render();

	for (it = _modal.begin(); it != _modal.end(); it++)
		(*it)->Render();

	if (CDrag::GetParent()) CDrag::GetParent()->DragRender();

	CImeInput::s_Ime.Render();

	ShowDebugInfo(); //	-added by Arcol
}

bool CFormMgr::OnHotKey(char key, int control) {
	if (!_bEnabled) return false;

	if (_modal.size()) {
		CForm* frm = _modal.back();
		if (frm->evtHotkeyHandler != NULL) {
			if (frm->evtHotkeyHandler(frm, key))
				return true;
		}
	}
	else if (_show.size()) {
		CForm* frm = _show.back();
		if (frm->evtHotkeyHandler != NULL) {
			if (frm->evtHotkeyHandler(frm, key))
				return true;
		}
	}

	if (!_vhotkey.empty()) {
		for (vhotkey::iterator it = _vhotkey.begin(); it != _vhotkey.end(); ++it)
			if ((*it)(key, control))
				return true;
	}

	if (!_modal.empty()) return true;

	for (vfrm::iterator it = _forms->begin(); it != _forms->end(); ++it) {
		if ((*it)->GetHotKey() == key) {
			if ((*it)->GetIsShow())
				(*it)->Hide();
			else
				(*it)->Show();
			return true;
		}
	}

	return false;
}

bool CFormMgr::HandleWindowMsg(DWORD dwMsg, DWORD dwParam1, DWORD dwParam2) {
	if (!_bEnabled) return false;

	switch (dwMsg) {
	case WM_HOTKEY: {
		if (LOWORD(dwParam2) == MOD_ALT) {
			vfrm::const_iterator end = _forms->end();
			for (vfrm::const_iterator it = _forms->begin(); it != end; ++it) {
				if ((*it)->GetHotKey() > 0 && (*it)->GetHotKey() == (UINT)HIWORD(dwParam2)) {
					if ((*it)->GetIsShow())
						(*it)->Hide();
					else
						(*it)->Show();
				}
			}
		}
		return true;
	}
	};

	CImeInput::s_Ime.HandleWindowMsg(dwMsg, dwParam1, dwParam2);
	return false;
}


void CFormMgr::Refresh() {
	for (vfrm::iterator it = _allForms.begin(); it != _allForms.end(); it++)
		(*it)->Refresh();
}

CForm* CFormMgr::Find(const char* str, int no) {
	if (no >= (int)GetFormTempleteMax() || no < -1) return NULL;

	vfrm* form = no == -1 ? &_defaulttemplete : _showforms[no];
	for (vfrm::iterator it = form->begin(); it != form->end(); it++)
		if (strcmp((*it)->GetName(), str) == 0)
			return *it;

	return NULL;
}

void CFormMgr::_ShowModal(CForm* form) {
	if (find(_modal.begin(), _modal.end(), form) == _modal.end()) {
		_modal.push_back(form);
	}
	_ActiveCompent();
}

CForm* CFormMgr::FindAll(const char* str) {
	for (vfrm::iterator it = _allForms.begin(); it != _allForms.end(); it++)
		if (strcmp((*it)->GetName(), str) == 0)
			return *it;

	return NULL;
}

void CFormMgr::ForEach(FormFun pFun) {
	for (vfrm::iterator it = _allForms.begin(); it != _allForms.end(); it++)
		pFun(*it);
}

bool CFormMgr::AddKeyCharEvent(KeyCharEvent event) {
	if (_OnKeyChar.end() == find(_OnKeyChar.begin(), _OnKeyChar.end(), event)) {
		_OnKeyChar.push_back(event);
		return true;
	}
	return false;
}

bool CFormMgr::DelKeyCharEvent(KeyCharEvent event) {
	vkeychars::iterator it = find(_OnKeyChar.begin(), _OnKeyChar.end(), event);
	if (_OnKeyChar.end() != it) {
		_OnKeyChar.erase(it);
		return true;
	}
	return false;
}

bool CFormMgr::AddKeyDownEvent(KeyDownEvent event) {
	if (_OnKeyDown.end() == find(_OnKeyDown.begin(), _OnKeyDown.end(), event)) {
		_OnKeyDown.push_back(event);
		return true;
	}
	return false;
}

bool CFormMgr::DelKeyDownEvent(KeyDownEvent event) {
	vkeydowns::iterator it = find(_OnKeyDown.begin(), _OnKeyDown.end(), event);
	if (_OnKeyDown.end() != it) {
		_OnKeyDown.erase(it);
		return true;
	}
	return false;
}

bool CFormMgr::AddMouseEvent(MouseEvent event) {
	vmouses::iterator it = find(_OnMouseRun.begin(), _OnMouseRun.end(), event);
	if (_OnMouseRun.end() == it) {
		_OnMouseRun.push_back(event);
		return true;
	}
	return false;
}

bool CFormMgr::DelMouseEvent(MouseEvent event) {
	vmouses::iterator it = find(_OnMouseRun.begin(), _OnMouseRun.end(), event);
	if (_OnMouseRun.end() != it) {
		_OnMouseRun.erase(it);
		return true;
	}
	return false;
}

bool CFormMgr::AddHotKeyEvent(HotKeyEvent event) {
	vhotkey::iterator it = find(_vhotkey.begin(), _vhotkey.end(), event);
	if (_vhotkey.end() == it) {
		_vhotkey.push_back(event);
		return true;
	}
	return false;
}

bool CFormMgr::DelHotKeyEvent(HotKeyEvent event) {
	vhotkey::iterator it = find(_vhotkey.begin(), _vhotkey.end(), event);
	if (_vhotkey.end() != it) {
		_vhotkey.erase(it);
		return true;
	}
	return false;
}

bool CFormMgr::AddMouseScrollEvent(MouseScrollEvent event) {
	vscrolls::iterator it = find(_OnMouseScroll.begin(), _OnMouseScroll.end(), event);
	if (_OnMouseScroll.end() == it) {
		_OnMouseScroll.push_back(event);
		return true;
	}
	return false;
}

bool CFormMgr::DelMouseScrollEvent(MouseScrollEvent event) {
	vscrolls::iterator it = find(_OnMouseScroll.begin(), _OnMouseScroll.end(), event);
	if (_OnMouseScroll.end() != it) {
		_OnMouseScroll.erase(it);
		return true;
	}
	return false;
}

bool CFormMgr::AddFormInit(FormMgrEvent pInitFun) {
	vinits::iterator it = find(_vinits.begin(), _vinits.end(), pInitFun);
	if (_vinits.end() == it) {
		_vinits.push_back(pInitFun);
		return true;
	}
	return false;
}

void CFormMgr::SetScreen() {
	for (vfrm::iterator it = _allForms.begin(); it != _allForms.end(); ++it)
		(*it)->OnSetScreen();

	Refresh();
}

void CFormMgr::_UpdataShowForm(CForm* frm) {
	if (_show.empty()) return;

	if (frm == _show.back()) return;

	vfrm::iterator it = find(_show.begin(), _show.end(), frm);
	if (it != _show.end()) {
		_show.erase(it);
		_show.push_back(frm);
	}
	_ActiveCompent();
}

void CFormMgr::_ActiveCompent() {
	CCompent* pCompent = NULL;
	if (!_modal.empty()) {
		pCompent = _modal.back()->FindActiveCompent();
		CCompent::SetActive(pCompent);
		return;
	}
	else {
		for (vfrm::reverse_iterator rit = _show.rbegin(); rit != _show.rend(); ++rit) {
			if (pCompent == (*rit)->FindActiveCompent()) {
				continue;
			}
		}
	}

	if (pCompent) {
		CCompent::SetActive(pCompent);
	}
	else {
		if (CCompent::GetActive()) {
			if (!CCompent::GetActive()->GetForm()->IsNormal())
				CCompent::SetActive(NULL);
		}
	}
}

void CFormMgr::_DelShowForm(CForm* frm) {
	vfrm::iterator it = find(_show.begin(), _show.end(), frm);
	if (it != _show.end()) {
		_show.erase(it);
	}
	return;
}

void CFormMgr::_AddShowForm(CForm* frm) {
	vfrm::iterator it = find(_show.begin(), _show.end(), frm);
	if (it == _show.end()) {
		_show.push_back(frm);
	}
}

CForm* CFormMgr::FindESCForm() {
	CForm* frm = CForm::GetActive();
	if (frm->GetIsEscClose())
		return frm;

	for (vfrm::iterator it = _modal.begin(); it != _modal.end(); it++)
		if ((*it)->GetIsEscClose())
			return (*it);

	for (vfrm::reverse_iterator rit = _show.rbegin(); rit != _show.rend(); ++rit)
		if ((*rit)->GetIsEscClose())
			return (*rit);

	return NULL;
}

struct sUIDebugInfo {
	CForm* form;
	string formName;
	string isShow;
	string isEnable;
	string hasPopMenu;
	string coordinate;
	int totalCompents;
	int layer;
	string renderTime;
	string hotkey;
};

void CFormMgr::ShowDebugInfo() {
	if (!_IsDebugMode) return;
	int i = 0;
	char temp[25];
	char buf[10240];

	int data0 = 0, data1 = 0, data2 = 0, data3 = 0, data4 = 0, data5 = 0, data6 = 0, data7 = 0, totalTick = 0;
	sUIDebugInfo* pInfo = new sUIDebugInfo[_forms->size()];
	i = 0;
	CForm* pForm = NULL;
	vfrm::iterator it;
	for (it = _forms->begin(); it != _forms->end(); it++, i++) {
		pForm = *it;
		pInfo[i].form = (*it);
		pInfo[i].formName = pForm->GetName();
		pInfo[i].isShow = pForm->GetIsShow() ? "Y" : "N";
		pInfo[i].isEnable = pForm->GetIsEnabled() ? "Y" : "N";
		sprintf(buf, "(%4d,%4d)-(%4d,%4d)", pForm->GetLeft(), pForm->GetTop(), pForm->GetRight(), pForm->GetBottom());
		pInfo[i].coordinate = buf;
		pInfo[i].totalCompents = (int)pForm->_allCompents.size();
		sprintf(temp, "%c", pForm->GetHotKey());
		pInfo[i].hotkey = (pForm->GetHotKey() == '\0') ? "" : temp;
		pInfo[i].hasPopMenu = (pForm->_pPopMenu) ? "Y" : "N";
		pInfo[i].layer = -1;
		pInfo[i].renderTime = "N/A";
	}

	DWORD tick;
	int j = (int)_show.size() + (int)_modal.size() - 1;
	static DWORD frameColor = 0x90FFFFFF;
	static int changeColor = 0;
	if (changeColor % 7 == 1) frameColor = (frameColor == 0x90FFFFFF) ? 0x90000000 : 0x90FFFFFF;
	changeColor = (changeColor + 1) % 7;
	for (it = _show.begin(); it != _show.end(); it++) {
		for (i = 0; i < (int)_forms->size(); i++) {
			if (pInfo[i].form == (*it)) {
				tick = GetTickCount();
				(*it)->Render();
				tick = GetTickCount() - tick;
				totalTick += tick;
				sprintf(buf, "%d", tick);
				pInfo[i].renderTime = buf;
				pInfo[i].layer = j;
				j--;
			}
		}
		if (_IsDrawFrameInDebugMode) {
			GetRender().LineFrame((*it)->GetLeft(), (*it)->GetTop(), (*it)->GetRight(), (*it)->GetBottom(), frameColor);
			ui::Render((*it)->GetName(), (*it)->GetLeft(), (*it)->GetTop(), ~frameColor);
		}
	}

	for (it = _modal.begin(); it != _modal.end(); it++) {
		for (i = 0; i < (int)_forms->size(); i++) {
			if (pInfo[i].form == (*it)) {
				tick = GetTickCount();
				(*it)->Render();
				tick = GetTickCount() - tick;
				totalTick += tick;
				sprintf(buf, "%d", tick);
				pInfo[i].renderTime = buf;
				pInfo[i].layer = j;
				j--;
			}
		}
	}

	GetRender().FillFrame(0, 16, 600, 27);
	//ui::Render("ID                                                 ",0,16,COLOR_WHITE);
	ui::Render(GetLanguageString(575).c_str(), 0, 16, COLOR_WHITE);
	for (i = 0; i < (int)_forms->size(); i++) {
		//pForm = *it;
		//pInfo[i].formName=pForm->GetName();
		//pInfo[i].isShow=pForm->GetIsShow()?"Y":"N";
		//pInfo[i].isEnable=pForm->GetIsEnabled()?"N":"Y";
		//pInfo[i].coordinate=buf;
		//pInfo[i].parent=pForm->GetParent()?pForm->GetParent()->GetName():"";
		//pInfo[i].totalCompents=pForm->_allCompents.size();
		//pInfo[i].hotkey=pForm->GetHotKey();
		//pInfo[i].hasPopMenu=(pForm->_pPopMenu)"Y":"N";

		//DWORD color=COLOR_WHITE;

		DWORD color = COLOR_WHITE;
		//"ID                                                "
		sprintf(buf, "%-6d %-20s %s   %s     %s     %s     %3s    %3d    %2s     %4s", i, pInfo[i].formName.c_str(),
				pInfo[i].coordinate.c_str(), pInfo[i].isShow.c_str(), pInfo[i].isEnable.c_str(),
				pInfo[i].hasPopMenu.c_str(), (pInfo[i].layer >= 0) ? itoa(pInfo[i].layer, temp, 10) : "N/A",
				pInfo[i].totalCompents, pInfo[i].hotkey.c_str(), pInfo[i].renderTime.c_str());

		if (pInfo[i].form->GetIsShow()) {
			color = 0xFFFFFF7F;
		}
		if (pInfo[i].layer == 0) {
			color = 0xFFA0A0FF;
		}
		if (_IsDrawBackgroundInDebugMode) {
			GetRender().FillFrame(0, 30 + i * 11, 600, 41 + i * 11);
		}
		ui::Render(buf, 0, 30 + i * 11, color);
	}
	//delete[] pInfo;
	SAFE_DELETE_ARRAY(pInfo); // UI


	data0 = GetFormTempleteMax(); //
	for (i = 0; i < (int)GetFormTempleteMax(); i++) {
		if (_showforms[i]->size() > 0) {
			data1++; //
		}
	}
	data2 = GetFormTempletetNum(); //
	data3 = (int)_allForms.size(); //
	data4 = (int)_forms->size(); //
	data5 = (int)_show.size() + (int)_modal.size(); //
	data6 = (int)_modal.size(); //
	data7 = (int)CGuiTime::_times.size();
	FmtLang(buf, sizeof(buf), GetLanguageString(576), data0, data1, data2, data3, data4, data5, data6, data7,
			totalTick);
	GetRender().FillFrame(0, 0, ui::GetWidth(buf), 11);
	ui::Render(buf, 0, 0, COLOR_WHITE);
	int help_sx = 620;
	int help_sy = 160;
	if (_IsDrawBackgroundInDebugMode) {
		GetRender().FillFrame(help_sx, help_sy, 785, 511);
	}
	ui::Render(
		"      UI\n1.:\n  show \n2.:\n  hide \n3./\n  frame 1 //\n  frame 0 //\n3.\n  background 1 //\n  background 0 //",
		help_sx, help_sy, COLOR_WHITE);
}


void CFormMgr::SetEnableHotKey(int flag, bool v) {
	// 
	if (v) {
		_nEnableHotKey |= flag;
	}
	else {
		_nEnableHotKey &= ~flag;
	}
}
