#include "stdafx.h"
#include "uifastcommand.h"
#include "UICommand.h"
#include "uigoodsgrid.h"
#include "UIFormMgr.h"
#include "UIEquipForm.h"
#include "UIBoatForm.h"
#include "character.h"
#include "uiitemcommand.h"

//---------------------------------------------------------------------------
// class CFastCommand
//---------------------------------------------------------------------------
CFastCommand::fasts CFastCommand::_fast;

CFastCommand::CFastCommand(CForm& frmOwn)
	: CCommandCompent(frmOwn), _pCommand(NULL), _pCommand2(NULL), evtChange(NULL), topBar(false) {
	_fast.push_back(this);

	_SetSelf();
}

CFastCommand::~CFastCommand() {
	for (fasts::iterator it = _fast.begin(); it != _fast.end(); ++it) {
		if ((*it) == this) {
			_fast.erase(it);
			return;
		}
	}
}

CFastCommand::CFastCommand(const CFastCommand& rhs)
	: CCommandCompent(rhs), _pCommand(rhs._pCommand), _pCommand2(rhs._pCommand2), evtChange(rhs.evtChange) {
	_fast.push_back(this);

	_SetSelf();
}

CFastCommand& CFastCommand::operator=(const CFastCommand& rhs) {
	CCommandCompent::operator =(rhs);

	_pCommand = rhs._pCommand;
	evtChange = rhs.evtChange;
	return *this;
}

void CFastCommand::Render() {
	/*
		try{
			if ( _pCommand2 !=NULL ) _pCommand2->Render( GetX()+3, GetY() );
		}catch(const std::exception &exc)
		{
			//todo - debug this.
			// catch anything thrown within try block that derives from std::exception
			//AllocConsole();
			//freopen("CONOUT$", "w", stdout);
			//cout << exc.what();
		}
		*/
	if (_pCommand) _pCommand->Render(GetX(), GetY());
}

bool CFastCommand::MouseRun(int x, int y, DWORD key) {
	if (!IsNormal()) return false;

	if (!_pCommand) {
		InRect(x, y);
	}
	else if (IsNoDrag(x, y, key)) {
		if (key & Mouse_RDown || key & Mouse_LUp) {
			Exec(key & Mouse_RDown);
		}
	}

	return _IsMouseIn;
}

void CFastCommand::Exec(bool rightclick) {
	if (_pCommand == NULL) {
		return;
	}
	char type;
	short grid;
	g_stUIEquip._GetCommandShortCutType(_pCommand, type, grid);

	if (type != defItemShortCutType) {
		_pCommand->Exec();
	}
	else {
		if (_pCommand2 != NULL) {
			_pCommand2->ExecRightClick();
			_pCommand->Exec();
		}
		else {
			CItemCommand* pFastItem = dynamic_cast<CItemCommand*>(_pCommand);
			int fastItemType = pFastItem->GetItemInfo()->sType;
			int equipItemType = 0;
			SItemGrid grdItem = g_stUIBoat.GetHuman()->GetPart().SLink[enumEQUIP_LHAND];
			if (grdItem.sID != 0) {
				CItemRecord* pEquipItem = GetItemRecordInfo(grdItem.sID);
				if (pEquipItem) {
					equipItemType = pEquipItem->sType;
				}
			}
			if ((equipItemType == 11 && fastItemType == 1) || rightclick) {
				_pCommand->ExecRightClick();
			}
			else {
				_pCommand->Exec();
			}
		}
	}
}


void CFastCommand::DelCommand(CCommandObj* p) {
	for (fasts::iterator it = _fast.begin(); it != _fast.end(); ++it) {
		if ((*it)->_pCommand && (*it)->_pCommand == p) {
			(*it)->_pCommand = NULL;
			(*it)->_pCommand2 = NULL;
		}
	}
}

void CFastCommand::FindFastCommandIndexes(CCommandObj* p, int arr[36]) {
	int arrCount = 0;
	for (fasts::iterator it = _fast.begin(); it != _fast.end(); ++it) {
		//if( (*it)->_pCommand && ((*it)->_pCommand==p || (*it)->_pCommand2==p) ){
		bool left = (*it)->_pCommand && (*it)->_pCommand == p;
		bool right = (*it)->_pCommand2 && (*it)->_pCommand2 == p;
		if (left || right) {
			arr[arrCount++] = (*it)->nTag;
		}
	}
}

CFastCommand* CFastCommand::GetFastCommand(int index) {
	return _fast[index] ? _fast[index] : NULL;
}

CFastCommand* CFastCommand::FintFastCommand(CCommandObj* p, bool checkSecondary) {
	for (fasts::iterator it = _fast.begin(); it != _fast.end(); ++it) {
		if ((*it)->_pCommand && ((*it)->_pCommand == p || (checkSecondary && (*it)->_pCommand2 == p))) {
			return *it;
		}
	}
	return NULL;
}

eAccept CFastCommand::SetCommand(CCommandObj* p, int x, int y) {
	if (!p || (p && p->IsDragFast())) {
		bool isAccept = false;
		if (evtChange) evtChange(this, p, isAccept);
		if (isAccept) {
			if (p) p->SetIsFast(true);

			if (_pCommand && _pCommand != p) {
				_pCommand->SetIsFast(false);
			}
			_pCommand2 = NULL;
			_pCommand = p;
		}
		return enumFast;
	}
	return enumRefuse;
}

void CFastCommand::AddCommand2(CCommandObj* p) {
	if (_pCommand2 && _pCommand2 != p) {
		_pCommand2->SetIsFast(false);
	}
	_pCommand2 = p;
	if (p) {
		p->SetIsFast(true);
	}
}

void CFastCommand::AddCommand(CCommandObj* p) {
	if (_pCommand && _pCommand != p) {
		_pCommand->SetIsFast(false);
	}
	/*
	 if( _pCommand2 && _pCommand2!=p ) 
    {
        _pCommand2->SetIsFast(false);
		_pCommand2 = NULL;
    }*/
	_pCommand = p;
	_pCommand2 = NULL;
	if (p) {
		p->SetIsFast(true);
	}
}

void CFastCommand::RenderHint(int x, int y) {
	if (_pCommand) {
		_pCommand->RenderHint(x, y);
	}
	else {
		_RenderHint(_strHint.c_str(), x, y);
	}
}

void CFastCommand::DragRender() {
	if (_pCommand) _pCommand->Render(GetX() + CDrag::GetDrag()->GetDragX(), GetY() + CDrag::GetDrag()->GetDragY());
}

void CFastCommand::_DragEnd(int x, int y, DWORD key) {
	CForm* form = CFormMgr::s_Mgr.GetHitForm(x, y);
	if (form) {
		CCompent* p = form->GetHitCommand(x, y);
		if (!p) return;

		CFastCommand* fast = dynamic_cast<CFastCommand*>(p);
		CCommandObj* oldp = NULL;
		if (fast) oldp = fast->GetCommand();

		switch (p->SetCommand(_pCommand, x, y)) {
		case enumFast:
			SetCommand(oldp, x, y);
			break;
		case enumAccept:
			break;
		case enumRefuse:
			break;
		}
	}
	else {
		SetCommand(NULL, x, y);
	}
}

void CFastCommand::_SetSelf() {
	if (!_pDrag) _pDrag = new CDrag;

	_pDrag->SetIsMove(false);
	_pDrag->evtMouseDragEnd = _DragEnd;
}

CCompent* CFastCommand::GetHintCompent(int x, int y) {
	if (GetIsShow() && InRect(x, y)) {
		if (_pCommand) {
			SetHintItem(_pCommand);
			return this;
		}
		else if (!_strHint.empty()) {
			return this;
		}
	}
	return NULL;
}

//---------------------------------------------------------------------------
// class COneCommand
//---------------------------------------------------------------------------
COneCommand::COneCommand(CForm& frmOwn)
	: CCommandCompent(frmOwn), _pCommand(NULL), evtBeforeAccept(NULL), evtThrowItem(NULL)
	  , evtUseCommand(NULL), _pActive(NULL), _IsShowActive(false), _eShowStyle(enumSmall) {
	_SetSelf();
}

COneCommand::COneCommand(const COneCommand& rhs)
	: CCommandCompent(rhs), _pCommand(NULL) {
	_SetSelf();
	if (rhs._pCommand) _pCommand = rhs._pCommand->Clone();
	_Copy(rhs);
}

COneCommand& COneCommand::operator=(const COneCommand& rhs) {
	CCommandCompent::operator =(rhs);

	if (_pCommand && _pCommand != rhs._pCommand) delete _pCommand;
	if (rhs._pCommand) _pCommand = rhs._pCommand->Clone();
	_Copy(rhs);
	return *this;
}

COneCommand::~COneCommand() {
	//if( _pCommand ) delete _pCommand;
	SAFE_DELETE(_pCommand); // UI
}

void COneCommand::_SetSelf() {
	if (!_pDrag) _pDrag = new CDrag;

	_pDrag->SetIsMove(false);
	_pDrag->evtMouseDragEnd = _DragEnd;
}

void COneCommand::_Copy(const COneCommand& rhs) {
	evtThrowItem = rhs.evtThrowItem;
	evtBeforeAccept = rhs.evtBeforeAccept;
	evtUseCommand = rhs.evtUseCommand;

	_IsShowActive = rhs._IsShowActive;
	_eShowStyle = rhs._eShowStyle;
}

void COneCommand::Render() {
	if (_pCommand) {
		if (_eShowStyle == enumSmall) {
			_pCommand->Render(GetX(), GetY());
			if (_pActive && _IsShowActive) {
				_pCommand->RenderEnergy(GetX(), GetY());
				_pActive->Render(GetX(), GetY());
			}
		}
		else {
			_pCommand->SaleRender(GetX(), GetY(), GetWidth(), GetHeight());
		}
	}
}

bool COneCommand::MouseRun(int x, int y, DWORD key) {
	if (!IsNormal()) return false;

	if (!_pCommand) {
		InRect(x, y);
	}
	else if (IsNoDrag(x, y, key)) {
		if (key & Mouse_LDown) {
			if (_pCommand->MouseDown())
				return true;
		}

		if (key & Mouse_LDB) {
			_pCommand->Exec();
		}
	}
	return _IsMouseIn;
}

void COneCommand::DragRender() {
	if (_pCommand) _pCommand->Render(GetX() + CDrag::GetDrag()->GetDragX(), GetY() + CDrag::GetDrag()->GetDragY());
}

void COneCommand::_DragEnd(int x, int y, DWORD key) {
	CForm* form = CFormMgr::s_Mgr.GetHitForm(x, y);
	if (form) {
		CCompent* p = form->GetHitCommand(x, y);
		if (!p) return;

		switch (p->SetCommand(_pCommand, x, y)) {
		case enumFast:
			break;
		case enumAccept:
			_pCommand = NULL;
			break;
		case enumRefuse:
			break;
		}
	}
	else {
		if (_pCommand) {
			bool isThrow = false;

			if (evtThrowItem) evtThrowItem(this, _pCommand, isThrow);
			if (isThrow) {
				delete _pCommand;
				_pCommand = NULL;
			}
		}
	}
}

eAccept COneCommand::SetCommand(CCommandObj* p, int x, int y) {
	bool isAccept = false;
	if (evtBeforeAccept) evtBeforeAccept(this, p, isAccept);

	if (isAccept) {
		AddCommand(p);
		return enumAccept;
	}
	return enumRefuse;
}

void COneCommand::DelCommand() {
	if (_pCommand) {
		delete _pCommand;
		_pCommand = NULL;
	}
}

void COneCommand::RenderHint(int x, int y) {
	if (_pCommand) {
		_pCommand->RenderHint(x, y);
	}
	else {
		_RenderHint(_strHint.c_str(), x, y);
	}
}

void COneCommand::AddCommand(CCommandObj* p) {
	if (_pCommand && _pCommand != p) {
		delete _pCommand;
		_pCommand = 0;
	}

	p->SetParent(this);
	p->SetIndex(defCommandDefaultIndex);
	_pCommand = p;
}

CCompent* COneCommand::GetHintCompent(int x, int y) {
	if (GetIsShow() && InRect(x, y)) {
		if (_pCommand) {
			if (_eShowStyle == enumSmall) {
				SetHintItem(_pCommand);
			}
			return this;
		}
		else if (!_strHint.empty()) {
			return this;
		}
	}
	return NULL;
}
