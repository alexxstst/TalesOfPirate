#include "StdAfx.h"
#include <cstdint>
#include "character.h"
#include "HMSynchroObj.h"
#include "GameApp.h"

using namespace std;

static const std::uint32_t DELAY_TIME = 1500;
extern CLanguageRecord g_oLangRec;

CStateSynchro::CStateSynchro()
	: _isExec(true), _pServerHarm(nullptr), _dwExecTime(CGameApp::GetCurTick() + DELAY_TIME), _dwCreateTime(_dwExecTime) {
	_nID = CSynchroManage::I()->_AddState(this);
}

CStateSynchro::~CStateSynchro() {
	// CSynchroManage::I()->_DelState( this );
}

//---------------------------------------------------------------------------
// class CSynchroManage
//---------------------------------------------------------------------------
static CSynchroManage g_Mange;
CSynchroManage* CSynchroManage::_pInstance = &g_Mange;

CSynchroManage::CSynchroManage()
	: _dwHead(0), _dwTail(0), _nSynchroNum(0) {
	memset(_All, 0, sizeof(_All));
}

CSynchroManage::~CSynchroManage() {
	Reset();
}

void CSynchroManage::Reset() {
	for (int i = 0; i < MAX_SIZE; i++) {
		if (_All[i]) {
			delete _All[i];
			_All[i] = nullptr;
		}
	}

	_dwHead = 0;
	_dwTail = 0;
	_nSynchroNum = 0;
}

void CSynchroManage::FrameMove(std::uint32_t dwTime) {
	static CStateSynchro* p = nullptr;
	static std::uint32_t i;

	if (_nSynchroNum == 0) return;

	typedef vector<CCharacter*> vchs;
	static vchs noexec;
	noexec.clear();
	static bool isFirst;
	isFirst = true;
	for (i = _dwHead; i < _dwTail; i++) {
		p = _All[i];
		if (p) {
			static CCharacter* pCha = nullptr;
			if (p->_isExec || dwTime >= p->_dwExecTime) {
				//
				pCha = p->GetHarmCha();
				if (pCha && find(noexec.begin(), noexec.end(), pCha) != noexec.end())
					continue;

				LG("CSynchroManage", "Del (%s ID:%d, Exec:%d), Num[%d], Head[%d], Tail[%d], Time[%d]\n",
				   p->GetClassName(), p->_nID, p->_isExec, _nSynchroNum, _dwHead, _dwTail,
				   DELAY_TIME - (p->_dwCreateTime - dwTime));
				p->_Exec();
				delete p;

				_All[i] = nullptr;
				_nSynchroNum--;
			}
			else {
				if (isFirst) {
					_dwHead = i;
					isFirst = false;
				}

				pCha = p->GetHarmCha();
				if (pCha && find(noexec.begin(), noexec.end(), pCha) == noexec.end()) {
					noexec.push_back(pCha);
				}
			}
		}
	}

	if (_nSynchroNum == 0) {
		_dwHead = 0;
		_dwTail = 0;
	}
}

int CSynchroManage::_AddState(CStateSynchro* pState) {
	if (_nSynchroNum >= MAX_SIZE) {
		LG("CSynchroManage", "msgAddState() false, SynchroNum[%d], Head[%d], Tail[%d]\n", _nSynchroNum, _dwHead,
		   _dwTail);
		return ERROR_ID;
	}

	if (_dwTail >= MAX_SIZE) {
		_CollectEmpty();
	}

	if (_All[_dwTail]) {
		LG("CSynchroManage", "msgAddState() false, Has Value SynchroNum[%d], Head[%d], Tail[%d], ID[%d]\n",
		   _nSynchroNum, _dwHead, _dwTail + 1, _dwTail);
		return ERROR_ID;
	}

	_All[_dwTail] = pState;
	pState->_nID = _dwTail;
	++_nSynchroNum;

	LG("CSynchroManage", "Add(%s), Num[%d], Head[%d], Tail[%d], ID[%d]\n", pState->GetClassName(), _nSynchroNum,
	   _dwHead, _dwTail + 1, _dwTail);
	return _dwTail++;
}

bool CSynchroManage::_DelState(CStateSynchro* pState) {
	int nID = pState->_nID;
	if (nID == ERROR_ID) {
		LG("CSynchroManage", "DelState() false, SynchroNum[%d], Head[%d], Tail[%d]\n", _nSynchroNum, _dwHead, _dwTail);
		return false;
	}

	if (_All[nID] == pState) {
		_All[nID] = nullptr;
		--_nSynchroNum;

		if (_nSynchroNum == 0) {
			_dwTail = 0;
			_dwHead = 0;
		}
		LG("CSynchroManage", "DelState(), SynchroNum[%d], Head[%d], Tail[%d], ID[%d]\n", _nSynchroNum, _dwHead, _dwTail,
		   nID);
		return true;
	}
	LG("CSynchroManage", g_oLangRec.GetString(148), _nSynchroNum, _dwHead, _dwTail, nID);
	return false;
}

void CSynchroManage::_CollectEmpty() {
	CStateSynchro* tmp[MAX_SIZE] = {nullptr};

	std::uint32_t ok = 0;
	std::uint32_t i = _dwHead;
	for (; i < _dwTail; i++) {
		if (_All[i]) {
			tmp[ok++] = _All[i];
		}
	}

	memcpy(_All, tmp, sizeof(_All));
	for (i = 0; i < ok; i++)
		_All[i]->_nID = i;

	_dwHead = 0;
	_dwTail = ok;
	_nSynchroNum = ok;
}
