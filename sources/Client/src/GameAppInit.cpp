#include "Stdafx.h"

#include "UIText.h"
#include "GameApp.h"
#include "cameractrl.h"
#include "Character.h"
#include "SceneObj.h"
#include "SceneItem.h"
#include "SceneObjFile.h"

#include "MPEditor.h"
#include "Scene.h"
#include "GameConfig.h"
#include "ConsoleBridge.h"
#include "EffectObj.h"
#include "Track.h"
#include "MPFont.h"
#include "FontManager.h"
#include "SmallMap.h"
#include "UIFormMgr.h"
#include "script.h"
#include "GlobalVar.h"
#include "DrawPointList.h"
#include "PacketCmd.h"
#include "UIRender.h"
#include "UIsystemform.h"
#include "SteadyFrame.h"
#include "UICozeForm.h"
#include "GameMovie.h"

using namespace std;

#ifndef USE_DSOUND
#include "AudioThread.h"
extern CAudioThread g_AudioThread;
#endif

extern CGameMovie g_GameMovie;

#ifdef TESTDEMO
#include "TestDemo.h"
#endif


DWORD CGameApp::_dwMouseDownTime[2] = {0};
int CGameApp::_nMusicSize = 64;
char CGameApp::_szOutBuf[256] = {0};
bool CGameApp::_IsMusicSystemValid = false;
CGameScene* CGameApp::_pCurScene = NULL;
DWORD CGameApp::_dwCurTick = 0;
DWORD CGameApp::m_dwLoginTime = 0;
CAniClock* CGameApp::_AniClock = NULL;


const BYTE verifyName[] = {0xf8, 0x05, 0x1a, 0xe4, 0x98, 0x5e, 0xb8, 0x9e};

const BYTE verifyDialog[] = {0xf0, 0xdc, 0xea, 0x7b, 0x40, 0xeb, 0xc4, 0x47};

static CSteadyFrame steady;
CSteadyFrame* CGameApp::_pSteady = &steady;

extern void LimitCurrentProc();


CGameApp::CGameApp()
	: _bEnableSuperKey(FALSE),
	  _pCamTrack(NULL),
	  _IsRenderTipText(false),
	  _IsInit(false),
	  _nSwitchScene(1),
	  _IsUserEnabled(true),
	  _eSwitchMusic(enumNoMusic),
	  _nCurMusicSize(1) {
	LimitCurrentProc();
	_pDrawPoints = new CDrawPointList;
	_pMainCam = new CCameraCtrl;

	xp = SHOWRSIZE / 2;
	yp = SHOWRSIZE / 2;

	_rsm = 0;

	memset(_szBkgMusic, 0, sizeof(_szBkgMusic));

#if(defined USE_TIMERPERIOD)
	_TimerPeriod = 0;
#endif

	btest = false;
	ihei = 0;
	_pNotify = new CTextHint;

	_pNotify->SetHintIsCenter(false);
	_pNotify->SetFixWidth(430);
	_pNotify->SetBkgColor(0x60000000);
	_pNotify->SetIsHeadShadow(false);

	//Add by sunny.sun20080804
	//Begin
	_pNotify1 = new CTextScrollHint;
	_pNotify1->SetFixWidth(430);
	_pNotify1->SetBkgColor(0x60000000);
	//End
	_dwNotifyTime = 0;
	_dwNotifyTime1 = 0;

	//_ctrl = new Ninja::LinearController < D3DXVECTOR3 >;
	//_pNinjaCamera = new Ninja::Camera( _ctrl );

	// Added by CLP
	_camera_target_ctrl = new Ninja::LinearController<D3DXVECTOR3>;
	_camera_eye_ctrl = new Ninja::LinearController<Ninja::SphereCoord>;
	_pNinjaCamera = new Ninja::Camera(_camera_target_ctrl, _camera_eye_ctrl);

#ifdef USE_DSOUND
	mSoundManager = NULL;
#endif
}


CGameApp::~CGameApp() {
	delete _pNotify;

#ifdef USE_DSOUND
	delete mSoundManager;
#endif

	SAFE_DELETE(_pMainCam);
	SAFE_DELETE(_pNinjaCamera);
	SAFE_DELETE(_camera_target_ctrl);
	SAFE_DELETE(_camera_eye_ctrl);
}

void CGameApp::End() {
	_IsInit = false;

	delete _pDrawPoints;

	SAFE_DELETE(_pCamTrack);

	CNavigationBar::g_cNaviBar.Clear();

	_stCursorMgr.ClearMemory();
	if (_pCurScene) {
		_pCurScene->_Clear();
		_pCurScene->_ClearMemory();
		delete _pCurScene;
		_pCurScene = NULL;
	}

	CGameScene::_ClearScene();

	// CCharacterSet *pCharSet = CCharacterSet::I();
	// SAFE_DELETE( pCharSet );

	//CSceneItemSet* pItemSet = CSceneItemSet::I();
	//SAFE_DELETE( pItemSet );

	ReleaseAllTable();

	_stScriptMgr.Clear();

	SAFE_DELETE(_pMainCam);

	FontManager::Instance().ClearFonts();

	g_CEffBox.ReleaseBox();
	CPathBox.ReleaseBox();

	SAFE_DELETE(_rsm);

	// Add by lark.li 20080923 begin
	_pSteady->Exit();
	// End

#ifdef TESTDEMO
	ReleaseTestDemo();
#endif

	MPGameApp::End();
}


BOOL CGameApp::_Init() {
	_AniClock = new CAniClock[MAX_ANI_CLOCK];
	for (int i = 0; i < MAX_ANI_CLOCK; i++) {
		_AniClock[i].Create(32, 0xa0000000);
	}

	CCozeForm::GetInstance();
	//InitAllTable();
	extern HINSTANCE g_hInstance;

	if (!LoadResource() || !LoadRes2() /*|| !LoadRes3()*/) {
		return 0;
	}

	if (g_Config.m_bEditor) {
		if (!LoadRes3())
			return 0;
	}

	_IsInit = false;

	{
		// init loading res mt flag
		lwIByteSet* res_bs = g_Render.GetInterfaceMgr()->res_mgr->GetByteSet();
		res_bs->SetValue(OPT_RESMGR_LOADTEXTURE_MT, g_Config.m_bMThreadRes);
		res_bs->SetValue(OPT_RESMGR_LOADMESH_MT, 0); //g_Config.m_bMThreadRes);

		// tex encoder
		res_bs->SetValue(OPT_RESMGR_TEXENCODE, 1);
	}

	{
		// init loading helper object instance flag
		lwIOptionMgr* opt_mgr = g_Render.GetInterfaceMgr()->sys->GetOptionMgr();
		opt_mgr->SetByteFlag(OPTION_FLAG_CREATEHELPERPRIMITIVE, g_Config.m_bEditor);
	}

	//_IsMusicSystemValid = ::mus_mgr_init( g_Config.m_bEnableMusic!=0 );	// music
#ifdef USE_DSOUND
	if (mSoundManager == NULL)
		mSoundManager = new DSoundManager(GetHWND());
#endif
	AudioSDL::get_instance()->init();

	_IsMusicSystemValid = true;
	if (!_IsMusicSystemValid && g_Config.m_bEnableMusic != 0) {
		g_logManager.InternalLog(LogLevel::Debug, "common", GetLanguageString(65));
	}

	_bConnected = FALSE;

	// Console handlers: диспатч команд и проверка разрешения — через
	// ConsoleBridge. Передаём лямбды (ConsoleProcessor принимает std::function).
	_pConsole->SetCmdHandler([](std::string_view cmd) -> std::string {
		return ConsoleBridge::Get().Dispatch(cmd);
	});
	_pConsole->SetCanOpenCheck([]() -> bool {
		return ConsoleBridge::Get().CanOpen();
	});

	g_Render.EnablePrint(INFO_GAME, TRUE);

	// Шрифты создаются в font_bootstrap.lua (вызывается из CFormMgr::Init).
	// FontSlot::TipText / MidAnnounce / BottomShadow соответствуют бывшим
	// g_CFont / _MidFont / _BottomFont. Доступ — FontManager::Instance().Get(...).

	//SIZE sizes;
	//g_CFont.GetTextSize("IDE\nokdote\n ",&sizes);

	memset(_stMidFont.szText, 0, sizeof(_stMidFont.szText));
	_stMidFont.dwBeginTime = 0;

	if (!_stScriptMgr.Init()) {
		g_logManager.InternalLog(LogLevel::Debug, "common", GetLanguageString(68));
		return FALSE;
	}

	string curr_ver = __TIME__;
	curr_ver += " ";
	curr_ver += __DATE__;
	// cout << curr_ver.c_str() << endl;

	_pCamTrack = new CPointTrack;

	InitAllTable();
	CCharacterActionCache::_cache.LoadActionDataFromStore();

	GetCursor()->InitMemory();

#if 1
	// by lsh
	// Load CSceneObjInfo model data
	extern void LoadResModelBuf(MPIResourceMgr* res_mgr);
	MPTimer t;
	t.Begin();
	LoadResModelBuf(g_Render.GetInterfaceMgr()->res_mgr);
	DWORD res_t = t.End();
#endif

	extern bool UIMainInit(CFormMgr* pSender);
	CFormMgr::s_Mgr.AddFormInit(UIMainInit); // Register script event, initialization event

	if (!CFormMgr::s_Mgr.Init(g_pGameApp->GetHWND())) // by lh Executes CLU_LoadScript and others
	{
		g_logManager.InternalLog(LogLevel::Debug, "common", GetLanguageString(69));
		return FALSE;
	}

	GetRender().RegisterFunc();

	CFormMgr::s_Mgr.SetEnabled(true);

	if (!_stScriptMgr.LoadScript()) {
		g_logManager.InternalLog(LogLevel::Debug, "common", GetLanguageString(70));
		return FALSE;
	}

	g_NetIF = new NetIF;
	g_Editor.Init(1);

	// Load initial scene
	LoadScriptScene((eSceneType)g_Config.m_nCreateScene);

	//LoadRes4();

	GetRender().Init();

#ifdef FLOAT_INVALID
	int i = _controlfp(0, 0);
	i &= ~(EM_ZERODIVIDE | EM_OVERFLOW | EM_INVALID);
	_controlfp(i, MCW_EM);
#endif

	SetFocus(GetHWND());

	if (g_Config.m_bEditor) SetIsRenderTipText(true);

	_rsm = new RenderStateMgr;
	_rsm->Init(g_Render.GetInterfaceMgr()->dev_obj);

	_IsInit = true;

	ResetCaption();

	if (!CGameScene::_InitScene()) {
		ToLogService("common", "msgCGameScene::_InitScene() return false");
		return false;
	}

#if(defined USE_TIMERPERIOD)
	extern void CALLBACK __timer_period_proc(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2);

	//DWORD fps = (DWORD)((1.0f / 30.0f) * 1000);
	DWORD fps = _dwFPSInterval;
	HWND hwnd = g_pGameApp->GetHWND();
	MPGUIDCreateObject((LW_VOID**)&_TimerPeriod, LW_GUID_TIMERPERIOD);
	_TimerPeriod->SetEvent(fps, fps, __timer_period_proc, (DWORD_PTR)hwnd, TIME_PERIODIC | TIME_CALLBACK_FUNCTION);
#endif

	lwIOptionMgr* opt_mgr = g_Render.GetInterfaceMgr()->sys->GetOptionMgr();
#if(defined OPT_CULL_1)
#pragma message("-------------Primitive Culling Opened-------------")
	opt_mgr->SetByteFlag(OPTION_FLAG_CULLPRIMITIVE_MODEL, 1);
#else
#pragma message("-------------Primitive Culling Closed-------------")
	opt_mgr->SetByteFlag(OPTION_FLAG_CULLPRIMITIVE_MODEL, 0);
#endif

	//LPD3DXEFFECT peff,peff2;
	//LPD3DXBUFFER	pbuff;
	//D3DXCreateEffectFromFile(g_Render.GetDevice(),"shader\\dx8\\eff.fx",&peff,NULL);
	//peff->GetCompiledEffect(&pbuff);
	//if(FAILED(D3DXCreateEffect(g_Render.GetDevice(),pbuff->GetBufferPointer(),pbuff->GetBufferSize(),&peff2,NULL)))
	//{
	//	INT EF = 0;
	//}

	// Load user-customized video settings (Modify by Michael Chen 2005-04-27)
	if (!g_stUISystem.m_isLoad) {
		g_stUISystem.LoadCustomProp();
	}

	auto currentScene = g_pGameApp->GetCurScene();
	if (currentScene) {
		currentScene->SetTextureLOD(g_stUISystem.m_sysProp.m_videoProp.nTexture);
	}

	GetRender().SetIsChangeResolution(true);

	g_stUISystem.m_sysProp.ApplyAudio();

	// Load user-customized game options (Modify by Michael Chen 2006-01-17)
	g_stUISystem.m_sysProp.ApplyGameOption();

#ifdef TESTDEMO
	InitTestDemo();
#endif

	//std::list<int> listtest;
	//listtest.push_back(1);
	//listtest.push_back(1);
	//listtest.push_back(1);
	//listtest.push_back(1);
	//listtest.push_back(1);

	//listtest.remove(NULL);

	// Record main thread ID for future use
	//DWORD id = GetCurrentThreadId();
	//LG( "threadid", "%d:%s\n", id, "WinMain" );

	//_pConsole->Show(TRUE);

	return TRUE;
}


void CGameApp::_End() {
	CFormMgr::s_Mgr.SetEnabled(false);

	extern bool UIClear();
	UIClear();

	SAFE_DELETE(_pCurScene);

	ToLogService("common", "NetIF release start");
	SAFE_DELETE(g_NetIF);
	ToLogService("common", "NetIF release end");

	CFormMgr::s_Mgr.Clear();

	//::mus_mgr_exit();	// music
#ifdef USE_DSOUND
	if (g_dwCurMusicID) {
		AudioSDL::get_instance()->stop(g_dwCurMusicID);
		g_dwCurMusicID = 0;
		Sleep(60);
	}
#endif

	AudioSDL::get_instance()->release();

#if(defined USE_TIMERPERIOD)
	if (_TimerPeriod) {
		_TimerPeriod->KillEvent();
		SAFE_RELEASE(_TimerPeriod);
	}
#endif
}

void CGameApp::OnLostDevice() {
	_pDrawPoints->OnLostDevice();
}

void CGameApp::OnResetDevice() {
	_pDrawPoints->OnResetDevice();
}

#ifdef USE_DSOUND

void CGameApp::PlaySample(string SoundName) {
	int SoundChannel = mSoundManager->LoadSound(SoundName);
	if (SoundChannel == -1)
		return;

	SoundInstance* aSoundInstance = mSoundManager->GetSoundInstance(SoundChannel);
	if (aSoundInstance)
		aSoundInstance->Play(false, true);
}

#endif

CAniClock::CAniClock() {
	_bUpdate = false;
	_pVBWnd = NULL;
}

CAniClock::~CAniClock() {
	SAFE_RELEASE(_pVBWnd);
}

bool CAniClock::Create(int width, DWORD dwColor) {
	_rcWnd.left = 0;
	_rcWnd.top = 0;
	_rcWnd.right = width;
	_rcWnd.bottom = width;

	int len = _rcWnd.right - _rcWnd.left;
	int hei = _rcWnd.bottom - _rcWnd.top;

	_vVertex[0].vPos = D3DXVECTOR4(float(_rcWnd.left + len / 2), float(_rcWnd.top + hei / 2), 0.9f, 1);
	_vVertex[1].vPos = D3DXVECTOR4(_vVertex[0].vPos.x, float(_rcWnd.top), 0.9f, 1);
	_vVertex[2].vPos = D3DXVECTOR4(float(_rcWnd.right), float(_rcWnd.top), 0.9f, 1);
	_vVertex[3].vPos = D3DXVECTOR4(float(_rcWnd.right), float(_rcWnd.top + hei / 2), 0.9f, 1);
	_vVertex[4].vPos = D3DXVECTOR4(float(_rcWnd.right), float(_rcWnd.bottom), 0.9f, 1);
	_vVertex[5].vPos = D3DXVECTOR4(_vVertex[0].vPos.x, float(_rcWnd.bottom), 0.9f, 1);
	_vVertex[6].vPos = D3DXVECTOR4(float(_rcWnd.left), float(_rcWnd.bottom), 0.9f, 1);
	_vVertex[7].vPos = D3DXVECTOR4(float(_rcWnd.left), float(_rcWnd.top + hei / 2), 0.9f, 1);
	_vVertex[8].vPos = D3DXVECTOR4(float(_rcWnd.left), float(_rcWnd.top), 0.9f, 1);
	_vVertex[9].vPos = D3DXVECTOR4(_vVertex[0].vPos.x, float(_rcWnd.top), 0.9f, 1);

	_vVertex[0].dwColor = dwColor;
	_vVertex[1].dwColor = dwColor;
	_vVertex[2].dwColor = dwColor;
	_vVertex[3].dwColor = dwColor;
	_vVertex[4].dwColor = dwColor;
	_vVertex[5].dwColor = dwColor;
	_vVertex[6].dwColor = dwColor;
	_vVertex[7].dwColor = dwColor;
	_vVertex[8].dwColor = dwColor;
	_vVertex[9].dwColor = dwColor;

	for (int n = 0; n < 10; n++) {
		_vSave[n] = _vVertex[n].vPos;
		_vTempVer[n] = _vVertex[n];
	}
	return true;
	//_vVertex[362]
}

void CAniClock::MoveTo(int x, int y) {
	int len = _rcWnd.right - _rcWnd.left;
	int hei = _rcWnd.bottom - _rcWnd.top;

	_rcWnd.left = x;
	_rcWnd.top = y;
	_rcWnd.right = _rcWnd.left + len;
	_rcWnd.bottom = _rcWnd.top + hei;

	_vVertex[0].vPos = D3DXVECTOR4(float(_rcWnd.left + len / 2), float(_rcWnd.top + hei / 2), 0.9f, 1);
	_vVertex[1].vPos = D3DXVECTOR4(_vVertex[0].vPos.x, float(_rcWnd.top), 0.9f, 1);
	_vVertex[2].vPos = D3DXVECTOR4(float(_rcWnd.right), float(_rcWnd.top), 0.9f, 1);
	_vVertex[3].vPos = D3DXVECTOR4(float(_rcWnd.right), float(_rcWnd.top + hei / 2), 0.9f, 1);
	_vVertex[4].vPos = D3DXVECTOR4(float(_rcWnd.right), float(_rcWnd.bottom), 0.9f, 1);
	_vVertex[5].vPos = D3DXVECTOR4(_vVertex[0].vPos.x, float(_rcWnd.bottom), 0.9f, 1);
	_vVertex[6].vPos = D3DXVECTOR4(float(_rcWnd.left), float(_rcWnd.bottom), 0.9f, 1);
	_vVertex[7].vPos = D3DXVECTOR4(float(_rcWnd.left), float(_rcWnd.top + hei / 2), 0.9f, 1);
	_vVertex[8].vPos = D3DXVECTOR4(float(_rcWnd.left), float(_rcWnd.top), 0.9f, 1);
	_vVertex[9].vPos = D3DXVECTOR4(_vVertex[0].vPos.x, float(_rcWnd.top), 0.9f, 1);

	for (int n = 0; n < 10; n++) {
		_vSave[n] = _vVertex[n].vPos;
	}
}

float CAniClock::RemainingTime() const {
	if (!_bUpdate) return {};
	return _fPlayTime - _fCurTime;
}

void CAniClock::Play(DWORD dwPlayTime) {
	if (_bUpdate)
		return;

	_bUpdate = true;
	ResetTime(dwPlayTime);
}

void CAniClock::ResetTime(DWORD dwTime) {
	_fPlayTime = (float)dwTime / 1000;

	int len = _rcWnd.right - _rcWnd.left;
	int hei = _rcWnd.bottom - _rcWnd.top;

	_vVertex[0].vPos = D3DXVECTOR4(float(_rcWnd.left + len / 2), float(_rcWnd.top + hei / 2), 0.9f, 1);
	_vVertex[1].vPos = D3DXVECTOR4(_vVertex[0].vPos.x, float(_rcWnd.top), 0.9f, 1);
	_vVertex[2].vPos = D3DXVECTOR4(float(_rcWnd.right), float(_rcWnd.top), 0.9f, 1);
	_vVertex[3].vPos = D3DXVECTOR4(float(_rcWnd.right), float(_rcWnd.top + hei / 2), 0.9f, 1);
	_vVertex[4].vPos = D3DXVECTOR4(float(_rcWnd.right), float(_rcWnd.bottom), 0.9f, 1);
	_vVertex[5].vPos = D3DXVECTOR4(_vVertex[0].vPos.x, float(_rcWnd.bottom), 0.9f, 1);
	_vVertex[6].vPos = D3DXVECTOR4(float(_rcWnd.left), float(_rcWnd.bottom), 0.9f, 1);
	_vVertex[7].vPos = D3DXVECTOR4(float(_rcWnd.left), float(_rcWnd.top + hei / 2), 0.9f, 1);
	_vVertex[8].vPos = D3DXVECTOR4(float(_rcWnd.left), float(_rcWnd.top), 0.9f, 1);
	_vVertex[9].vPos = D3DXVECTOR4(_vVertex[0].vPos.x, float(_rcWnd.top), 0.9f, 1);

	for (int n = 0; n < 10; n++) {
		_vSave[n] = _vVertex[n].vPos;
	}
	_fCurAngle = 0; // 6.283185f / (float)_dwTime;
	_fCurTime = 0;
}

void CAniClock::FrameMove(DWORD dwDailTime) {
	_fCurTime += *ResMgr.GetDailTime();
	_fCurAngle = (_fCurTime / _fPlayTime) * 6.283185f;

	//_fCurAngle = 0.02f;

	float flerp = 0;

	int len = (_rcWnd.right - _rcWnd.left) / 2;
	if (_fCurAngle < 0.7853981f) {
		flerp = _fCurAngle / 0.7853981f;
		_vVertex[1].vPos.x = _vSave[1].x + (float)len * flerp;
	}
	else if (_fCurAngle >= 0.7853981f && _fCurAngle < 1.570796f) {
		flerp = (_fCurAngle - 0.7853981f) / 0.7853981f;
		_vVertex[1].vPos.y = _vSave[2].y + (float)len * flerp;

		_vVertex[2].vPos = _vVertex[1].vPos;
	}
	else if (_fCurAngle >= 1.570796f && _fCurAngle < 3.141592f - 0.7853981f) {
		flerp = (_fCurAngle - 1.570796f) / 0.7853981f;
		_vVertex[1].vPos.y = _vSave[3].y + (float)len * flerp;

		_vVertex[2].vPos = _vVertex[1].vPos;
		_vVertex[3].vPos = _vVertex[1].vPos;
	}
	else if (_fCurAngle >= 3.141592f - 0.7853981f && _fCurAngle < 3.141592f) {
		flerp = (_fCurAngle - (3.141592f - 0.7853981f)) / 0.7853981f;
		_vVertex[1].vPos.x = _vSave[4].x - (float)len * flerp;

		_vVertex[2].vPos = _vVertex[1].vPos;
		_vVertex[3].vPos = _vVertex[1].vPos;
		_vVertex[4].vPos = _vVertex[1].vPos;
	}
	else if (_fCurAngle >= 3.141592f && _fCurAngle < 3.141592f + 0.7853981f) {
		flerp = (_fCurAngle - 3.141592f) / 0.7853981f;
		_vVertex[1].vPos.x = _vSave[5].x - (float)len * flerp;

		_vVertex[2].vPos = _vVertex[1].vPos;
		_vVertex[3].vPos = _vVertex[1].vPos;
		_vVertex[4].vPos = _vVertex[1].vPos;
		_vVertex[5].vPos = _vVertex[1].vPos;
	}
	else if (_fCurAngle >= 3.141592f + 0.7853981f && _fCurAngle < 3.141592f + 1.570796f) {
		flerp = (_fCurAngle - (3.141592f + 0.7853981f)) / 0.7853981f;

		_vVertex[1].vPos.x = _vSave[6].x;
		_vVertex[1].vPos.y = _vSave[6].y - (float)len * flerp;

		_vVertex[2].vPos = _vVertex[1].vPos;
		_vVertex[3].vPos = _vVertex[1].vPos;
		_vVertex[4].vPos = _vVertex[1].vPos;
		_vVertex[5].vPos = _vVertex[1].vPos;
		_vVertex[6].vPos = _vVertex[1].vPos;
	}
	else if (_fCurAngle >= 3.141592f + 1.570796f && _fCurAngle < 6.283185f - 0.7853981f) {
		flerp = (_fCurAngle - (3.141592f + 1.570796f)) / 0.7853981f;
		_vVertex[1].vPos.y = _vSave[7].y - (float)len * flerp;

		_vVertex[2].vPos = _vVertex[1].vPos;
		_vVertex[3].vPos = _vVertex[1].vPos;
		_vVertex[4].vPos = _vVertex[1].vPos;
		_vVertex[5].vPos = _vVertex[1].vPos;
		_vVertex[6].vPos = _vVertex[1].vPos;
		_vVertex[7].vPos = _vVertex[1].vPos;
	}
	else if (_fCurAngle >= 6.283185f - 0.7853981f && _fCurAngle < 6.283185f) {
		flerp = (_fCurAngle - (6.283185f - 0.7853981f)) / 0.7853981f;

		_vVertex[1].vPos.y = _vSave[9].y;
		_vVertex[1].vPos.x = _vSave[8].x + (float)len * flerp;

		_vVertex[2].vPos = _vVertex[1].vPos;
		_vVertex[3].vPos = _vVertex[1].vPos;
		_vVertex[4].vPos = _vVertex[1].vPos;
		_vVertex[5].vPos = _vVertex[1].vPos;
		_vVertex[6].vPos = _vVertex[1].vPos;
		_vVertex[7].vPos = _vVertex[1].vPos;
		_vVertex[8].vPos = _vVertex[1].vPos;
	}
	else if (_fCurAngle >= 6.283185f) {
		_bUpdate = false;
	}
}

void CAniClock::Update() {
	if (!_bUpdate)
		return;

	//if(	_rcWnd.left != x || _rcWnd.top	!= y)
	//	MoveTo(x, y);
	FrameMove(0);
}

void CAniClock::Render(int x, int y) {
	if (!_bUpdate)
		return;

	g_Render.SetRenderState(D3DRS_ZENABLE, FALSE);
	g_Render.SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	g_Render.SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_Render.SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_Render.SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	g_Render.SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
	g_Render.SetRenderState(D3DRS_DITHERENABLE, FALSE);
	g_Render.SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	//commenting these out seems to fix model texture issues.

	//g_Render.SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	//g_Render.SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG2);
	//g_Render.SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	//g_Render.SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_SELECTARG2);

	g_Render.SetRenderState(D3DRS_LIGHTING, FALSE);
	g_Render.SetTexture(0, NULL);

	g_Render.SetVertexShader(NULL);
	g_Render.SetFVF(D3DFVF_CLOCK2);
	for (int n = 0; n < 10; ++n) {
		_vTempVer[n].vPos.x = _vVertex[n].vPos.x + x;
		_vTempVer[n].vPos.y = _vVertex[n].vPos.y + y;
	}

	g_Render.GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 8, &_vTempVer, sizeof(ClockVer));

	char txt[3];
	sprintf(txt, "%.0f%", RemainingTime());
	const int check = int(RemainingTime());
	if (check < 10) {
		ui::BRender(2, txt, x + 10, y + 10, COLOR_WHITE, COLOR_BLACK);
	}
	else if (check > 100) {
		ui::BRender(2, txt, x + 5, y + 10, COLOR_WHITE, COLOR_BLACK);
	}
	else {
		ui::BRender(2, txt, x + 9, y + 10, COLOR_WHITE, COLOR_BLACK);
	}
}

void CAniClock::Resume(DWORD dwStartTime, DWORD dwPlayTime) {
	if (_bUpdate) return;
	_bUpdate = true;

	_fPlayTime = (float)dwStartTime / 1000;

	int len = _rcWnd.right - _rcWnd.left;
	int hei = _rcWnd.bottom - _rcWnd.top;

	_vVertex[0].vPos = D3DXVECTOR4(float(_rcWnd.left + len / 2), float(_rcWnd.top + hei / 2), 0.9f, 1);
	_vVertex[1].vPos = D3DXVECTOR4(_vVertex[0].vPos.x, float(_rcWnd.top), 0.9f, 1);
	_vVertex[2].vPos = D3DXVECTOR4(float(_rcWnd.right), float(_rcWnd.top), 0.9f, 1);
	_vVertex[3].vPos = D3DXVECTOR4(float(_rcWnd.right), float(_rcWnd.top + hei / 2), 0.9f, 1);
	_vVertex[4].vPos = D3DXVECTOR4(float(_rcWnd.right), float(_rcWnd.bottom), 0.9f, 1);
	_vVertex[5].vPos = D3DXVECTOR4(_vVertex[0].vPos.x, float(_rcWnd.bottom), 0.9f, 1);
	_vVertex[6].vPos = D3DXVECTOR4(float(_rcWnd.left), float(_rcWnd.bottom), 0.9f, 1);
	_vVertex[7].vPos = D3DXVECTOR4(float(_rcWnd.left), float(_rcWnd.top + hei / 2), 0.9f, 1);
	_vVertex[8].vPos = D3DXVECTOR4(float(_rcWnd.left), float(_rcWnd.top), 0.9f, 1);
	_vVertex[9].vPos = D3DXVECTOR4(_vVertex[0].vPos.x, float(_rcWnd.top), 0.9f, 1);

	for (int n = 0; n < 10; n++) {
		_vSave[n] = _vVertex[n].vPos;
	}
	_fCurTime = (float)dwPlayTime / 1000;
	_fCurAngle = (_fCurTime / _fPlayTime) * 6.283185f;
}
