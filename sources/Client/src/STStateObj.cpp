#include "stdafx.h"
#include "STStateObj.h"
#include "GameApp.h"
#include "PacketCmd.h"
#include "Character.h"
#include "Actor.h"
#include "GameDiagnostic.h"

using Corsairs::Client::Diagnostic::GameDiagnostic;

//---------------------------------------------------------------------------
// class CActionState
//---------------------------------------------------------------------------
CActionState::CActionState(CActor* p)
	: _pActor(p), _isWait(false), _isExecEnd(false), _pParent(NULL)
	  , _IsOver(false), _IsCancel(false), _IsSend(false), _IsInit(false), _nServerID(INT_MAX) {
}

CActionState::~CActionState() {
}

void CActionState::Cancel() {
	if (!_IsAllowCancel()) return;

	_IsCancel = true;

	if (_IsSend) CS_EndAction(this);

	if (GameDiagnostic::Instance().IsMoveEnabled()
		&& GetActor()->GetCha() == g_pGameApp->GetCurScene()->GetMainCha()) {
		ToLogService("movie", LogLevel::Debug,
					 "{}{}-cancel, Tick:{}, srvID:{}",
					 _pParent ? "  \t" : "  ",
					 GetExplain(), GetTickCount(), _nServerID);
	}
}

void CActionState::FrameMove() {
	if (_IsOver) PopState();
}

void CActionState::End() {
	if (_IsInit)
		_End(); // ,

	if (GameDiagnostic::Instance().IsMoveEnabled() && GetActor()->GetCha()->IsMainCha()) {
		ToLogService("movie", LogLevel::Debug,
					 "{}{}-end, Tick:{}, srvID:{}, isOver={}, isCancel={}{}",
					 _pParent ? "\t" : "",
					 GetExplain(), GetTickCount(), _nServerID,
					 _IsOver, _IsCancel,
					 _pParent ? "" : "\n");
	}
}

void CActionState::Start() {
	if (_isWait) return;

	if (_IsSend && !GetActor()->IsEnabled()) {
		_StartFailed();
		return;
	}

	const bool moveDiag = GameDiagnostic::Instance().IsMoveEnabled();
	const bool isMain = GetActor()->GetCha()->IsMainCha();

	if (moveDiag && isMain) {
		ToLogService("movie", LogLevel::Debug,
					 "{}{}-start, Tick:{}, isSend={}",
					 _pParent ? "\t" : "",
					 GetExplain(), GetTickCount(), _IsSend);
	}

	_IsInit = _Start();

	if (!_IsInit) {
		_StartFailed();
		if (moveDiag && isMain) {
			ToLogService("movie", LogLevel::Debug,
						 "{}{}-Not Start, Tick:{}",
						 _pParent ? "\t" : "",
						 GetExplain(), GetTickCount());
		}
	}
}

bool CActionState::_IsAllowCancel() {
	return _AllowCancel();
}

void CActionState::_StartFailed() {
	_IsInit = false;
	_IsOver = true;

	PopState();
	StartFailed();
}

//---------------------------------------------------------------------------
// class CActionState
//---------------------------------------------------------------------------
void CMoveList::GotoFront(CCharacter* pCha) {
	POINT& pt = _path.front();
	if (pt.x != pCha->GetCurX() || pt.y != pCha->GetCurY()) {
		pCha->MoveTo(pt.x, pt.y);
	}
	_path.pop_front();
}

void CMoveList::Clear(unsigned int count) {
	if (count == 0) {
		_path.clear();
	}
	else {
		while (_path.size() >= count)
			_path.pop_front();
	}
}

void CMoveList::PushPoint(int x, int y) {
	if (_path.empty()) {
		POINT pt = {x, y};
		_path.push_back(pt);
	}
	else {
		if (::GetDistance(x, y, _path.back().x, _path.back().y) > 18) {
			POINT pt = {x, y};
			_path.push_back(pt);
		}
		else {
			_path.back().x = x;
			_path.back().y = y;
		}
	}
	return;
}

int CMoveList::GetListDistance(int x, int y) {
	int dis = 0;
	int nCurX = x;
	int nCurY = y;
	for (path::iterator it = _path.begin(); it != _path.end(); it++) {
		dis += ::GetDistance(nCurX, nCurY, it->x, it->y);
		nCurX = it->x;
		nCurY = it->y;
	}
	return dis;
}
