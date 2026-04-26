#include "stdafx.h"
#include "stmove.h"
#include "actor.h"
#include "GameApp.h"
#include "FontManager.h"
#include "MPFont.h"
#include "NetProtocol.h"
#include "Character.h"
#include "DrawPointList.h"
#include "CharacterAction.h"
#include "PacketCmd.h"
#include "SceneItem.h"
#include "FindPath.h"
#include "uiminimapform.h"
#include "CameraCtrl.h"
#include "STAttack.h"
#include "AreaRecord.h"
#include "MountRecord.h"
#include "GameDiagnostic.h"

using Corsairs::Client::Diagnostic::GameDiagnostic;

//---------------------------------------------------------------------------
// class CWaitMoveState
//---------------------------------------------------------------------------
const float _fNormalRate = 0.95f;

unsigned long CWaitMoveState::_ulPreMoveTime = 500;
float CWaitMoveState::_fMoveRate = 1.0f;
DWORD CWaitMoveState::_dwPreMoveDis = 240;


int CWaitMoveState::MaxPing = 0;
int CWaitMoveState::MinPing = 5000;
int CWaitMoveState::LastPing[LAST_NUM] = {0};
int CWaitMoveState::LastPingCnt = 0;
int CWaitMoveState::nTotalPing = 0;
int CWaitMoveState::nTotalPingCnt = 0;
float CWaitMoveState::fAveragePing = 0.0f;
int CWaitMoveState::LastPingShow[LAST_NUM] = {0};

void CWaitMoveState::SetPreMoveTime(long time) {
	_ulPreMoveTime = time;

	if (MaxPing < time)
		MaxPing = time;

	if (MinPing > time)
		MinPing = time;

	LastPing[LastPingCnt] = time;

	int nCnt = LastPingCnt;
	for (int i = 0; i < LAST_NUM; i++) {
		LastPingShow[i] = LastPing[nCnt];
		nCnt--;
		if (nCnt < 0) nCnt = LAST_NUM - 1;
	}

	LastPingCnt++;
	if (LastPingCnt >= LAST_NUM) LastPingCnt = 0;

	nTotalPing += time;
	nTotalPingCnt++;
	fAveragePing = (float)nTotalPing / (float)nTotalPingCnt;

	static DWORD nTotalFPS = 0;
	static DWORD nTotalFPSCnt = 0;
	static float fFPSAverage = 0.0f;
	static DWORD nMaxFPS = 0;
	static DWORD nMinFPS = 500;
	if (nMaxFPS < g_Render.GetFPS())
		nMaxFPS = g_Render.GetFPS();
	if (nMinFPS > g_Render.GetFPS())
		nMinFPS = g_Render.GetFPS();
	nTotalFPS += g_Render.GetFPS();
	nTotalFPSCnt++;
	fFPSAverage = (float)nTotalFPS / (float)nTotalFPSCnt;
	g_logManager.InternalLog(LogLevel::Debug, "common",
							 SafeVFormat(GetLanguageString(404), LastPingShow[0], LastPingShow[1], LastPingShow[2],
										 MaxPing, fAveragePing, MinPing, nMaxFPS, fFPSAverage, nMinFPS));
}

void CWaitMoveState::RenderPing() {
	//  CTRL + ALT + SHIFT + K 

	//static char szBuf[128] = { 0 };
	//FontManager::Instance().Get(FontSlot::TipText)->DrawText( szBuf, 5, 55, D3DCOLOR_ARGB(255,255,255,255));
}

void CWaitMoveState::RenderCameraInfo() {
#if 1
	CCameraCtrl* pCam = g_pGameApp->GetMainCam();

	int y(65), step(16), line(5);

	GetRender().FillFrame(0, y - 5, 160, y + 5 + step * line);

	auto drawLine = [&](const std::string& s) {
		FontManager::Instance().Get(FontSlot::TipText)->DrawText(s.c_str(), 5, y, D3DCOLOR_ARGB(255, 255, 255, 255));
		y += step;
	};
	drawLine(std::format(" CameraRangeXY  = {:.2f} ", pCam->m_fxy));
	drawLine(std::format(" CameraRangeZ   = {:.2f} ", pCam->m_fz));
	drawLine(std::format(" CameraRangeFOV = {:.2f} ", pCam->m_ffov));
	drawLine(std::format(" CameraRotate   = {:.2f} ", pCam->m_fAngle));
	drawLine(std::format(" CameraShowSize = {} ", pCam->m_iShowWidth));
#endif
}


CWaitMoveState::CWaitMoveState(CActor* p)
	: CActionState(p) {
	_pCha = GetActor()->GetCha();
}

CWaitMoveState::~CWaitMoveState() {
}

void CWaitMoveState::ChaRun() {
	_pCha->CheckIsFightArea();

	_pCha->PlayPose(_pCha->GetPose(POSE_RUN), PLAY_LOOP_SMOOTH);
	float vel = g_stUISystem.m_sysProp.m_gameOption.bFramerate ? 960.0f : 480.0f;

	_pCha->SetPoseVelocity((float)_pCha->getMoveSpeed() / vel);
	//480.0f, doubled it to match increased framerate mdrst@pkodev.net
	_pCha->RefreshItem();
}


bool CWaitMoveState::_Start() {
	ChaRun();
	return true;
}

void CWaitMoveState::FrameMove() {
	if (_pCha->GetIsArrive()) {
		if (!GetMoveList()->IsEmpty()) {
			GetMoveList()->GotoFront(_pCha);
		}
		else if (_IsOver) {
			PopState();
			return;
		}
	}
}

void CWaitMoveState::PushPoint(int x, int y) {
	GetMoveList()->PushPoint(x, y);
}

bool CWaitMoveState::IsAllowMove() {
	if (_pCha->GetChaState()->IsFalse(enumChaStateMove)) {
		g_pGameApp->SysInfo(GetLanguageString(406));
		return false;
	}

	//   if( _pCha->getMoveSpeed()<=0 )
	//{
	//	g_pGameApp->SysInfo( "The speed is zero and cannot be moved" );
	//       return false;
	//}

	return true;
}

//---------------------------------------------------------------------------
// class CMoveState
//---------------------------------------------------------------------------
CMoveState::CMoveState(CActor* p)
	: CWaitMoveState(p), _IsWalkLine(false), _nTargetX(0), _nTargetY(0) {
	_IsSend = true;
}

bool CMoveState::_Start() {
	if (!IsAllowMove())
		return false;

	int serverdis = 0;
	bool isServer = false;
	static stNetMoveInfo serverPath;
	if (g_cFindPath.Find(_pCha->GetScene(), _pCha, _pCha->GetServerX(), _pCha->GetServerY(), _nTargetX, _nTargetY,
						 _IsWalkLine)) {
		WriteInfo(g_cFindPath.GetResultPath(), serverPath);
		serverdis = g_cFindPath.GetLength();
	}

	if (!g_cFindPath.Find(_pCha->GetScene(), _pCha, _pCha->GetCurX(), _pCha->GetCurY(), _nTargetX, _nTargetY,
						  _IsWalkLine)) {
		return false;
	}

	WriteInfo(g_cFindPath.GetResultPath(), _stPathInfo);

	//Compare two wayfinding, whether the end point is the same, the end point is different, according to the server
	if (isServer) {
		if (memcmp(&serverPath.pos_buf[serverPath.pos_num - 1], &_stPathInfo.pos_buf[_stPathInfo.pos_num - 1],
				   sizeof(POINT)) != 0) {
			memcpy(_stPathInfo.pos_buf, serverPath.pos_buf, sizeof(_stPathInfo.pos_buf));
			_stPathInfo.pos_num = serverPath.pos_num;

			isServer = false;
		}
	}

	float fRate = 1.0f;
	if (isServer) {
		fRate = (float)g_cFindPath.GetLength() / (float)serverdis;

		if (fRate > 1.10f) fRate = 1.10f;
		if (fRate < 0.90f) fRate = 0.90f;
	}

	// The first point is its own coordinates
	int n = _stPathInfo.pos_num;
	for (int i = 2; i < n; i++) {
		GetMoveList()->PushPoint(_stPathInfo.pos_buf[i].x, _stPathInfo.pos_buf[i].y);
	}

	ChaRun();

	_pCha->MoveTo(_stPathInfo.pos_buf[1].x, _stPathInfo.pos_buf[1].y);


	if (GameDiagnostic::Instance().IsMoveEnabled()) {
		ToLogService("movie", LogLevel::Debug,
					 "CMoveState::Start SelfPos:{}, {}, ServerPos:{}, {}, Target:{}, {}, walkLine={}, Tick:{}",
					 GetActor()->GetCha()->GetCurX(), GetActor()->GetCha()->GetCurY(),
					 GetActor()->GetCha()->GetServerX(), GetActor()->GetCha()->GetServerY(),
					 _nTargetX, _nTargetY, _IsWalkLine, GetTickCount());
	}

	if (_IsSend) {
		if (isServer) {
			serverPath.dwAveragePing = _ulPreMoveTime;

			CS_BeginAction(_pCha, enumACTION_MOVE, (void*)&serverPath, this);
		}
		else {
			_stPathInfo.dwAveragePing = _ulPreMoveTime;

			CS_BeginAction(_pCha, enumACTION_MOVE, (void*)&_stPathInfo, this);
		}
	}
	return true;
}

void CMoveState::WriteInfo(S_BVECTOR<D3DXVECTOR3>& path, stNetMoveInfo& info) {
	char buf[80] = {0};

	int n = path.size();
	if (n > defMAX_POS_NUM) n = defMAX_POS_NUM;
	for (int i = 0; i < n; i++) {
		info.pos_buf[i].x = (long)(path[i]->x * 100.0f) / 50 * 50 + 25;
		info.pos_buf[i].y = (long)(path[i]->y * 100.0f) / 50 * 50 + 25;

		ToLogService("common", "{}, {}", info.pos_buf[i].x, info.pos_buf[i].y);
	}

	info.pos_num = n;

	ToLogService("common", "\n");
}

void CMoveState::PushPoint(int x, int y) {
	int dis = GetDistance(x, y, _pCha->GetCurX(), _pCha->GetCurY());
	if (dis > 500) {
		g_pGameApp->AddTipText(SafeVFormat(GetLanguageString(407), dis));
	}
}

bool CMoveState::_IsAllowCancel() {
	if (!g_NetIF->IsConnected()) return true;
	else return _AllowCancel();
}

void CMoveState::MoveEnd(int x, int y, int nState) {
	_IsOver = true;
	if (!GetParent()) {
		if (enumMSTATE_BLOCK & nState) {
			PopState();

			_SynchroServer();
			return;
		}

		if (GetDistance(_pCha->GetServerX(), _pCha->GetServerY(), _pCha->GetCurX(), _pCha->GetCurY()) > 2000) {
			_SynchroServer();
		}
	}
}

void CMoveState::_End() {
	if (!GetIsCancel()) {
		if (_pCha->GetServerX() != _pCha->GetCurX() || _pCha->GetServerY() != _pCha->GetCurY()) {
			_SynchroServer();
		}
	}
}

void CMoveState::FrameMove() {
	if (_IsCancel && _IsOver) {
		_pCha->StopMove();

		PopState();
		return;
	}

	if (_pCha->GetIsArrive()) {
		if (!GetMoveList()->IsEmpty()) {
			GetMoveList()->GotoFront(_pCha);
		}
		else if (_IsOver) {
			if (_pCha->GetServerX() != _pCha->GetCurX() || _pCha->GetServerY() != _pCha->GetCurY()) {
				_SynchroServer();
			}
			PopState();
			return;
		}
		else {
			if (!GetParent() && GetActor()->IsEmpty()) {
				GetActor()->IdleState();
			}
		}
	}
}

void CMoveState::_SynchroServer() {
	_pCha->setPos(_pCha->GetServerX(), _pCha->GetServerY());
	_pCha->MoveTo(_pCha->GetServerX(), _pCha->GetServerY());

	g_pGameApp->SetCameraPos(_pCha->GetPos());
	g_pGameApp->GetMainCam()->BeginMove();
}

void CMoveState::Cancel() {
	_pCha->StopMove();

	GetMoveList()->Clear();
	_IsOver = true;
}

//---------------------------------------------------------------------------
// class CServerMoveState
//---------------------------------------------------------------------------
DWORD CServerMoveState::_dwLastEndTime = 0;
DWORD CServerMoveState::_dwLastMoveTime = 0;

bool CServerMoveState::_Start() {
	_fMoveRate = _fNormalRate;

	if (_nTargetX < 0 || _nTargetY < 0)
		return false;

	if (!IsAllowMove())
		return false;

	bool isServer = false;
	if (_pCha->GetServerX() != _pCha->GetCurX() || _pCha->GetServerY() != _pCha->GetCurY()) {
		if (g_cFindPath.Find(_pCha->GetScene(), _pCha, _pCha->GetServerX(), _pCha->GetServerY(), _nTargetX, _nTargetY,
							 _IsWalkLine)) {
			WriteInfo(g_cFindPath.GetResultPath(), _stPathInfo);
			isServer = true;
		}
		else {
			return false;
		}
	}

	if (!isServer) {
		//LG( "test", "msgtest" );
		if (g_cFindPath.Find(_pCha->GetScene(), _pCha, _pCha->GetCurX(), _pCha->GetCurY(), _nTargetX, _nTargetY,
							 _IsWalkLine)) {
			WriteInfo(g_cFindPath.GetResultPath(), _stPathInfo);
		}
		else {
			return false;
		}
	}

	_stPathInfo.dwAveragePing = _ulPreMoveTime;

	if (!GetParent()) {
		PreMove();
	}

	if (_IsSend) {
		CS_BeginAction(_pCha, enumACTION_MOVE, (void*)&_stPathInfo, this);
		_dwLastMoveTime = CGameApp::GetCurTick();
	}
	return true;
}

void CServerMoveState::PreMove() {
	ChaRun();

	POINT SPos[defMOVE_LIST_POINT_NUM];
	int nPointNum = defMOVE_LIST_POINT_NUM;
	int dis = int(_ulPreMoveTime * (float)_pCha->getMoveSpeed() / 1000.0f);
	int num = GetMovePoint(_stPathInfo.pos_buf, _stPathInfo.pos_num, dis, SPos, nPointNum);
	if (num > 1) {
		_pCha->MoveTo(SPos[1].x, SPos[1].y);

		for (int i = 2; i < num; i++) {
			GetMoveList()->PushPoint(SPos[i].x, SPos[i].y);
		}

		//  Визуальная отладка: точка PreMove на карте (зелёная, маленькая).
		//  Отображается только при включённой move-диагностике.
		if (GameDiagnostic::Instance().IsMoveEnabled()) {
			g_pGameApp->GetDrawPoints()->Add(SPos[num - 1].x, SPos[num - 1].y, 0xff00ff00, 0.1f);
		}
	}
}

void CServerMoveState::Cancel() {
	CActionState::Cancel();

	if (!g_NetIF->IsConnected()) {
		GetMoveList()->Clear();
	}
}

void CServerMoveState::FrameMove() {
	CWaitMoveState::FrameMove();

	// Adjust speed according to distance
	//DWORD dis = 0;
	//if( !_IsClientMove && _pCha->getMoveSpeed()>0 )
	//{
	//	dis = GetMoveList()->GetListDistance( _pCha->GetCurX(), _pCha->GetCurY() );
	//	//if( dis > _dwPreMoveDis )
	//	//{
	//		//_fMoveRate = 1.0f + float( dis - _dwPreMoveDis ) / (float)dis / 4.0f;
	//	//}

	//	float fRate = 1.0f + float( dis ) / (float)_pCha->getMoveSpeed(); 
	//	if( _fMoveRate > fRate )
	//		_fMoveRate -= 0.02f;
	//	else
	//		_fMoveRate += 0.02f;
	//}
	//LG( "moverate", "%d, %f\n", dis, _fMoveRate );
}

void CServerMoveState::_End() {
	_dwLastEndTime = CGameApp::GetCurTick();

	DWORD dwSpace = _dwLastEndTime - _dwLastMoveTime;
	g_logManager.InternalLog(LogLevel::Debug, "common",
							 SafeVFormat(GetLanguageString(408), dwSpace, _dwLastMoveTime, _dwLastEndTime,
										 (float)dwSpace * (float)GetActor()->GetCha()->getMoveSpeed() / 1000.0f));
}

void CServerMoveState::MoveEnd(int x, int y, int nState) {
	_IsOver = true;
}

void CServerMoveState::PushPoint(int x, int y) {
	if (_IsClientMove) {
		if (GetMoveList()->IsEmpty()) {
			_pCha->MoveTo(x, y);
		}
		else {
			POINT& pt = GetMoveList()->GetBack();
			pt.x = x;
			pt.y = y;
		}

		_IsClientMove = false;
		return;
	}

	GetMoveList()->PushPoint(x, y);
	return;
}

//---------------------------------------------------------------------------
// class CBackMoveState
//---------------------------------------------------------------------------
bool CBackMoveState::_Start() {
	ChaRun();
	_pCha->ForceMove(_nTargetX, _nTargetY);
	return true;
}

void CBackMoveState::FrameMove() {
	if (_pCha->GetIsArrive()) {
		PopState();
	}
}

//---------------------------------------------------------------------------
// class COneMoveState
//---------------------------------------------------------------------------
CMoveList COneMoveState::_List;

COneMoveState::COneMoveState(CActor* p)
	: CWaitMoveState(p), _IsWalkLine(false), _nTargetX(0), _nTargetY(0) {
	_nStartX = _nStartY = 0;
	_IsStartWalkLine = false;

	_nMoveCount = _nStartCount = _nEndCount = _nSendCount = 0;
	_dwSendTime = _dwEndTime = 0;
	_IsSendCancel = false;
	_fRate = _fNormalRate;
}

// Write down the new movement path
bool COneMoveState::ContinueMove(int nTargetX, int nTargetY, bool isWalkLine, bool IsCheckTime) {
	// Check if the coordinates are legal or illegal, and pull back directly
	if (GetDistance(_pCha->GetCurX(), _pCha->GetCurY(), _pCha->GetServerX(), _pCha->GetServerY()) > 2000) {
		SynchroPos(_pCha->GetServerX(), _pCha->GetServerY());
		if (!g_cFindPath.Find(_pCha->GetScene(), _pCha, _pCha->GetCurX(), _pCha->GetCurY(), nTargetX, nTargetY,
							  isWalkLine)) {
			return false;
		}
	}
	else if (!g_cFindPath.Find(_pCha->GetScene(), _pCha, _pCha->GetCurX(), _pCha->GetCurY(), nTargetX, nTargetY,
							   isWalkLine)) {
		if (GetDistance(_pCha->GetCurX(), _pCha->GetCurY(), _pCha->GetServerX(), _pCha->GetServerY()) <
			GetSyschroDistance())
			return false;

		SynchroPos(_pCha->GetServerX(), _pCha->GetServerY());
		if (!g_cFindPath.Find(_pCha->GetScene(), _pCha, _pCha->GetCurX(), _pCha->GetCurY(), nTargetX, nTargetY,
							  isWalkLine)) {
			return false;
		}
	}

	bool rv = true;

	//According to the parameters, the new moving path can be segmented into multiple moving paths
	int len = -1; // No segment
	extern int MOVE_LENGTH;
	if (MOVE_LENGTH > 0) {
		len = _pCha->getMoveSpeed() / 2;
		if (len < (int)_dwPreMoveDis) len = _dwPreMoveDis;
		if (len < 350) len = 350;
		if (len < MOVE_LENGTH) len = MOVE_LENGTH;
	}

	CompartMoveList(_List, g_cFindPath.GetResultPath(), len);

	const DWORD MOUSE_TIME = 300;
	static DWORD dwTime = 0;
	if (!IsCheckTime || (IsCheckTime && CGameApp::GetCurTick() > dwTime)) {
		if (IsCheckTime) {
			dwTime = CGameApp::GetCurTick() + MOUSE_TIME;
		}

		POINT cur_pt = _List.GetFront();
		_List.PopFront();
		if (!StartMove(cur_pt.x, cur_pt.y, isWalkLine))
			rv = false;
	}

	// Cache the movement path requested by the user
	_cNeedList.clear();
	while (!_List.IsEmpty()) {
		POINT pt = _List.GetFront();
		_List.PopFront();
		AddPath(pt.x, pt.y, isWalkLine);
	}
	return rv;
}

bool COneMoveState::AddPath(int x, int y, bool IsWalkLine) {
	ToLogService("common", "AddPath( {}, {}, {} )", x, y, IsWalkLine);

	_cNeedList.push_back(NeedPath(x, y, IsWalkLine));
	return true;
}

bool COneMoveState::SendInfo() {
	// Send Mobile Agreement
	if (!g_cFindPath.Find(_pCha->GetScene(), _pCha, _pCha->GetServerX(), _pCha->GetServerY(), _nTargetX, _nTargetY,
						  _IsWalkLine)) {
		return true;
	}

	ChaRun();

	_nSendCount++;

	stNetMoveInfo stPathInfo;
	WriteInfo(g_cFindPath.GetResultPath(), stPathInfo);
	CS_BeginAction(_pCha, enumACTION_MOVE, (void*)&stPathInfo, this);
	_IsSendCancel = true;
	_nServerDis = g_cFindPath.GetLength();
	_nServerStepDis = 0;

	_dwSendTime = CGameApp::GetCurTick();
	DWORD len = (DWORD)((float)(_nServerDis) / (float)_pCha->getMoveSpeed() * 1000.0f);
	_dwEndTime = CGameApp::GetCurTick() + len;

	int nLocal = 0;
	if (_cLocalList.IsEmpty()) {
		nLocal = GetDistance(_pCha->GetCurX(), _pCha->GetCurY(), _nTargetX, _nTargetY);
	}
	else {
		nLocal = _cLocalList.GetListDistance(_pCha->GetCurX(), _pCha->GetCurY()) + _pCha->GetTargetDistance();
	}

	// Adjust the protagonist rate, and if the distance is too far, pull back directly to the protagonist
	if (abs(nLocal - _nServerDis) > GetSyschroDistance()) {
		//The distance is too large, pull back
		_fMoveRate = _fNormalRate;
		_fRate = _fNormalRate;
		SynchroPos(_pCha->GetServerX(), _pCha->GetServerY());

		_nLocalDis = g_cFindPath.GetLength();

		_cLocalList.Clear();

		S_BVECTOR<D3DXVECTOR3>& path = g_cFindPath.GetResultPath();
		int n = path.size();
		for (int i = 1; i < n; i++) {
			_cLocalList.PushPoint((long)(path[i]->x * 100.0f), (long)(path[i]->y * 100.0f));
		}
		return true;
	}

	_fRate = RefreshRate(nLocal, _nServerDis);
	return true;
}

int COneMoveState::GetSyschroDistance() {
	// The distance that needs to be synchronized, that is, how much distance is exceeded, you should directly pull back to the protagonist
	int len = _pCha->getMoveSpeed() + _pCha->getMoveSpeed() + (int)_dwPreMoveDis;
	if (len < 1000) len = 1000;
	return len;
}

void COneMoveState::FrameMove() {
	// Dynamically adjust the moving speed of the protagonist
	const float fAddStep = 0.005f;
	const float fDownStep = 0.05f;
	if (_fMoveRate > _fRate) {
		_fMoveRate -= fDownStep;
	}
	else if (_fMoveRate < _fRate - fDownStep) {
		_fMoveRate += fAddStep;
	}
	//fix stun stuck second fix @mothannakh
	if (_nSendCount == _nEndCount + 1 && _nStartCount > _nSendCount) {
		if (_dwSendTime + 3000 < CGameApp::GetCurTick()) {
			//LG("SynchroPos","[SynchroPos] SendElapsed %u\n", CGameApp::GetCurTick() - _dwSendTime);
			PopState();
			SynchroPos(_pCha->GetServerX(), _pCha->GetServerY());
		}
	}

	if (_nSendCount == _nEndCount && _nStartCount > _nSendCount) {
		// The server has returned to the end of the move, check if there is a cache move, and send a new move protocol
		if (!SendInfo()) {
			PopState();
			return;
		}
	}

	if (_pCha->GetIsArrive()) {
		if (_cLocalList.IsEmpty() && _cNeedList.empty() && _nSendCount == _nEndCount) {
			// The server has returned to the end of the move, and there is no cache move, the state is over
			PopState();
			return;
		}

		if (!_cLocalList.IsEmpty()) {
			// Perform cache move
			if (!(_pCha->GetCurPoseType() == POSE_RUN || _pCha->GetCurPoseType() == POSE_RUN2)) {
				ChaRun();
			}
			_cLocalList.GotoFront(_pCha);
		}
		else if (GetActor()->IsEmpty() && _cNeedList.empty()) {
			// The protagonist has moved to the destination before the server and is waiting for the server's end mark
			// In order to prevent people from standing still, call Idle directly
			GetActor()->IdleState();
		}
	}

	if (_nSendCount == _nEndCount && CGameApp::GetCurTick() > _dwEndTime) {
		if (dynamic_cast<CAttackState*>(GetActor()->GetNextState())) {
			// Check if the next state is the attack state and the server has returned to the end
			// Regardless of whether there is a cache move, this move state is ended, ready to attack
			PopState();
			return;
		}

		if (!_cNeedList.empty()) {
			// The server has returned to the end, perform a new cache move
			const NeedPath& pt = _cNeedList.front();
			StartMove(pt.x, pt.y, pt.IsLine);
			_cNeedList.pop_front();
		}
	}
}


void COneMoveState::PushPoint(int x, int y) {
	// Adjust the speed of the protagonist according to the distance between the protagonist and the server
	int nLocal = 0;
	if (_cLocalList.IsEmpty()) {
		nLocal = GetDistance(_pCha->GetCurX(), _pCha->GetCurY(), _nTargetX, _nTargetY);
	}
	else {
		nLocal = _cLocalList.GetListDistance(_pCha->GetCurX(), _pCha->GetCurY()) + _pCha->GetTargetDistance();
	}

	if (!g_cFindPathEx.HaveTarget()) {
		_fRate = _fNormalRate;
		if (GetDistance(_pCha->GetServerX(), _pCha->GetServerY(), _nTargetX, _nTargetY) > 80) {
			//for fix stun walks bugs form clint stmovement  @mothannakh
			//if (g_cFindPath.Find(_pCha->GetScene(), _pCha, _pCha->GetServerX(), _pCha->GetServerY(), _nTargetX, _nTargetY, _IsWalkLine)) {
			if (g_cFindPath.GetPathFindingState()) {
				_fRate = RefreshRate(nLocal, g_cFindPath.GetLength());
			}
		}
	}
}


bool COneMoveState::StartMove(int nTargetX, int nTargetY, bool isWalkLine) {
	// There is a new movement path, send a protocol to terminate the current movement, and the protagonist walks along the new path in advance
	if (!IsAllowMove())
		return false;

	if (!g_cFindPath.Find(_pCha->GetScene(), _pCha, _pCha->GetCurX(), _pCha->GetCurY(), nTargetX, nTargetY, isWalkLine))
		return false;

	_nMoveCount++;
	if (_nStartCount == _nSendCount) {
		_nStartCount++;
		if (_nSendCount == _nEndCount + 1) {
			CS_EndAction(this);

			_dwSendTime = CGameApp::GetCurTick();
			_dwEndTime = CGameApp::GetCurTick();
			_IsSendCancel = false;
		}
	}

	_cLocalList.Clear();
	S_BVECTOR<D3DXVECTOR3>& path = g_cFindPath.GetResultPath();
	int n = path.size();
	for (int i = 1; i < n; i++) {
		_cLocalList.PushPoint((long)(path[i]->x * 100.0f), (long)(path[i]->y * 100.0f));
	}
	if (_cLocalList.GetCount() > 0) {
		_pCha->StopMove();
	}

	_nLocalDis = g_cFindPath.GetLength();
	_nTargetX = _cLocalList.GetBack().x;
	_nTargetY = _cLocalList.GetBack().y;
	_IsWalkLine = isWalkLine;

	// Adjust the moving speed of the protagonist according to the distance ratio between the local and the server to move
	if (g_cFindPath.Find(_pCha->GetScene(), _pCha, _pCha->GetServerX(), _pCha->GetServerY(), nTargetX, nTargetY,
						 isWalkLine)) {
		_fRate = RefreshRate(_nLocalDis, g_cFindPath.GetLength());
	}
	return true;
}

float COneMoveState::RefreshRate(int nLocalLen, int nServerLen) {
	float fRate = (float)nLocalLen / (float)(nServerLen) / 0.95f;
	if (fRate > 10.30f) fRate = 10.30f;
	if (fRate < 0.85f) fRate = 0.85f;
	return fRate;
}

bool COneMoveState::_Start() {
	// Start moving
	if (!IsAllowMove())
		return false;

	if (!ContinueMove(_nStartX, _nStartY, _IsStartWalkLine)) {
		return false;
	}

	_dwPreMoveDis = int(_ulPreMoveTime * (float)_pCha->getMoveSpeed() / 1000.0f);

	//_fMoveRate = _fNormalRate;
	//_fRate = _fNormalRate;
	ChaRun();
	return true;
}

void COneMoveState::_End() {
	ToLogService("common", "COneMoveState::_End");
}

void COneMoveState::SynchroPos(int x, int y) {
	// Pull the protagonist directly to the new coordinate	
	_pCha->setPos(x, y);
	_pCha->MoveTo(x, y);

	g_pGameApp->SetCameraPos(_pCha->GetPos(), false);
	g_pGameApp->GetMainCam()->BeginMove();
}

void COneMoveState::MoveEnd(int x, int y, int nState) {
	// End of server move
	_nEndCount++;

	_IsSendCancel = false;

	// If you encounter an obstacle to stop, then stop directly
	if ((nState & enumMSTATE_BLOCK) || (nState & enumMSTATE_CANTMOVE)) {
		PopState();
		SynchroPos(x, y);
		return;
	}

	if ((_nStartCount == _nSendCount) && (nState & enumMSTATE_CANCEL)) {
		_cLocalList.Clear();
		_cLocalList.PushPoint(x, y);
		_pCha->StopMove();
		return;
	}
}

void COneMoveState::Cancel() {
	// _IsCancel = true;

	// Cancel when not moving
	if (_IsSendCancel) {
		ToLogService("common", "Canlce");
		CS_EndAction(this);
		_cNeedList.clear();

		_dwSendTime = CGameApp::GetCurTick();
		_dwEndTime = CGameApp::GetCurTick();
		_IsSendCancel = false;
	}
}

void COneMoveState::WriteInfo(S_BVECTOR<D3DXVECTOR3>& path, stNetMoveInfo& info) {
	// Turn the wayfinding point into the protocol content sent to the server
	int n = path.size();
	if (n > defMAX_POS_NUM) n = defMAX_POS_NUM;
	for (int i = 0; i < n; i++) {
		info.pos_buf[i].x = (long)(path[i]->x * 100.0f) / 50 * 50 + 25;
		info.pos_buf[i].y = (long)(path[i]->y * 100.0f) / 50 * 50 + 25;
	}
	info.pos_num = n;
}

bool COneMoveState::IsSameServerPos(int x, int y) {
	return _nMoveCount <= 1;
}

void COneMoveState::CompartMoveList(CMoveList& outlist, S_BVECTOR<D3DXVECTOR3>& path, int length) {
	//For a wayfinding point, the length is divided into multiple segments, less than 0 does not segment
	CMoveList& cLocalList = outlist;
	cLocalList.Clear();
	if (length <= 0) {
		int n = path.size();
		cLocalList.PushPoint((long)(path[n - 1]->x * 100.0f), (long)(path[n - 1]->y * 100.0f));
		return;

		//for( int i=1; i<n; i++ ) 
		//{
		//	cLocalList.PushPoint( (long)(path[i]->x * 100.0f), (long)(path[i]->y * 100.0f) );
		//}
		//return;
	}

	int n = path.size();
	int x, y;
	int oldx = (long)(path[0]->x * 100.0f);
	int oldy = (long)(path[0]->y * 100.0f);
	int dis = 0;
	int segment = 0;
	int dx, dy;
	for (int i = 1; i < n; i++) {
		x = (long)(path[i]->x * 100.0f);
		y = (long)(path[i]->y * 100.0f);

		do {
			segment = ::GetDistance(oldx, oldy, x, y);
			if (segment + dis > (int)length) {
				GetDistancePos(x, y, oldx, oldy, dis + segment - length, dx, dy);
				cLocalList.PushPoint(dx, dy);
				if (cLocalList.GetCount() > 10)
					break;

				dis = 0;
				oldx = dx;
				oldy = dy;
			}
			else {
				dis += segment;

				oldx = x;
				oldy = y;
				break;
			}
		}
		while (true);

		if (i == n - 1) {
			POINT& pt = cLocalList.GetBack();
			if (GetDistance(pt.x, pt.y, x, y) > (int)length / 2) {
				cLocalList.PushPoint(x, y);
			}
			else {
				pt.x = x;
				pt.y = y;
			}
		}
	}
}
