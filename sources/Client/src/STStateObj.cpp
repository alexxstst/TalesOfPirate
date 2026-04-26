#include "stdafx.h"
#include "STStateObj.h"
#include "GameApp.h"
#include "PacketCmd.h"
#include "Character.h"
#include "Actor.h"

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

#ifdef _STATE_DEBUG
	if (GetActor()->GetCha() == g_pGameApp->GetCurScene()->GetMainCha()) {
		static char buf[256] = {0};
		sprintf(buf, "state-%s", GetActor()->GetCha()->getName().c_str());

		if (_pParent)
			g_logManager.InternalLog(LogLevel::Debug, "common",
									 std::format("  \t{}-cancel, Tick:{}", GetExplain(), GetTickCount()));
		else
			g_logManager.InternalLog(LogLevel::Debug, "common",
									 std::format("  {}-cancel, Tick:{}", GetExplain(), GetTickCount()));
	}
#endif
}

void CActionState::FrameMove() {
	if (_IsOver) PopState();
}

void CActionState::End() {
	if (_IsInit)
		_End(); // ,

#ifdef _STATE_DEBUG
	if (GetActor()->GetCha()->IsMainCha()) {
		static char buf[256] = {0};
		sprintf(buf, "state-%s", GetActor()->GetCha()->getName().c_str());

		if (_pParent)
			g_logManager.InternalLog(LogLevel::Debug, "common",
									 std::format("\t{}-end, Tick:{}", GetExplain(), GetTickCount()));
		else
			g_logManager.InternalLog(LogLevel::Debug, "common",
									 std::format("{}-end, Tick:{}\n", GetExplain(), GetTickCount()));
	}
#endif
}

void CActionState::Start() {
	if (_isWait) return;

	if (_IsSend && !GetActor()->IsEnabled()) {
		_StartFailed();
		return;
	}

#ifdef _STATE_DEBUG
	static char buf[256] = {0};
	static bool isMain;
	isMain = GetActor()->GetCha()->IsMainCha();
	if (isMain) {
		sprintf(buf, "state-%s", GetActor()->GetCha()->getName().c_str());
		if (_pParent)
			g_logManager.InternalLog(LogLevel::Debug, "common",
									 std::format("\t{}-start, Tick:{}", GetExplain(), GetTickCount()));
		else
			g_logManager.InternalLog(LogLevel::Debug, "common",
									 std::format("{}-start, Tick:{}", GetExplain(), GetTickCount()));
	}
#endif

	_IsInit = _Start();

#ifdef _STATE_DEBUG
	if (!_IsInit) {
		_StartFailed();
		if (isMain) {
			if (_pParent)
				g_logManager.InternalLog(LogLevel::Debug, "common",
										 std::format("\t{}-Not Start, Tick:{}", GetExplain(), GetTickCount()));
			else
				g_logManager.InternalLog(LogLevel::Debug, "common",
										 std::format("{}-Not Start, Tick:{}", GetExplain(), GetTickCount()));
			return;
		}
	}
#else
	if (!_IsInit) {
		_StartFailed();
	}
#endif
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
