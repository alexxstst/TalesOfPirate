#include "StdAfx.h"
#include <limits>
#include "uisystemform.h"
#include "SteadyFrameSync.h"
#include "uiform.h"
#include "uicheckbox.h"
#include "Gameapp.h"
#include "uiformmgr.h"
#include "uiprogressbar.h"
#include "uiCombo.h"
#include "packetcmd.h"
#include "loginscene.h"
#include "cameractrl.h"
#include "Character.h"
#include "GameConfig.h"
#include "UIMinimapForm.h"
#include "uistartform.h"
#include "uiheadsay.h"
#include "netchat.h"
#include "smallmap.h"

#include "uiboatform.h"
#include "uiequipform.h"
#include "GlobalVar.h"
#include "scene.h"

#ifndef USE_DSOUND
#include "AudioThread.h"
extern CAudioThread g_AudioThread;
#endif
using namespace std;
using namespace GUI;

extern bool g_IsShowStates;
extern bool g_IsCameraMode;

//---------------------------------------------------------
// class CSystemProperties
//---------------------------------------------------------

/**
* .
* @return: success Return 0.
*/

CSystemProperties::SVideo::SVideo() :
	bFullScreen(false), bResolution(0), nTexture(0), nQuality(0),
	bAnimation(false), bCameraRotate(false), bGroundMark(false), bDepth32(false) {
}

int CSystemProperties::ApplyVideo() {
	//video

	//bCameraRotate
	g_pGameApp->GetMainCam()->EnableRotation(m_videoProp.bCameraRotate);
	//bViewFar
	//g_pGameApp->GetMainCam()->EnableUpdown( m_videoProp.bViewFar ) ;//(Michael Chen 2005-04-22
	//nTexture-bGroundMark-nQuality
	g_pGameApp->GetCurScene()->SetTextureLOD(m_videoProp.nTexture);

	GetRender().SetIsChangeResolution(true);

	//
	//bAnimation

	//bDepth32-bFullScreen-bResolution
	int width(0), height(0);
	switch (m_videoProp.bResolution) {
	case 0:
		width = TINY_RES_X;
		height = TINY_RES_Y;
		break;
	case 1:
		width = SMALL_RES_X;
		height = SMALL_RES_Y;
		break;
	case 2:
		width = MID_RES_X;
		height = MID_RES_Y;
		break;
	case 3:
		width = LARGE_RES_X;
		height = LARGE_RES_Y;
		break;
	case 4:
		width = EXTRA_LARGE_RES_X;
		height = EXTRA_LARGE_RES_Y;
		break;
	case 5:
		width = FULL_LARGE_RES_X;
		height = FULL_LARGE_RES_Y;
		break;
	default:
		width = TINY_RES_X;
		height = TINY_RES_Y;
		break;
	}
	D3DFORMAT format = m_videoProp.bDepth32 ? D3DFMT_D24X8 : D3DFMT_D16;

	MPIDeviceObject* dev_obj = g_Render.GetInterfaceMgr()->dev_obj;
	if (FAILED(dev_obj->CheckCurrentDeviceFormat(BBFI_DEPTHSTENCIL, format))) {
		format = D3DFMT_D16;
	}
	g_pGameApp->ChangeVideoStyle(width, height, format, !m_videoProp.bFullScreen);

	return 0;
}

/**
* .
* @return: success Return 0.
*/
int CSystemProperties::ApplyAudio() {
	//audio
	g_pGameApp->GetCurScene()->SetSoundSize(m_audioProp.nMusicEffect / 10.0f);
	g_pGameApp->SetMusicSize(m_audioProp.nMusicSound / 10.0f);
	g_pGameApp->mSoundManager->SetVolume(m_audioProp.nMusicEffect / 10.0f);

	return 0;
}

//-----------------------------------------------------------------------------
int CSystemProperties::ApplyGameOption() {
	GlobalAppConfig.SetMoveClient(m_gameOption.bRunMode);
	g_pGameApp->SysInfo("Game settings have been updated");
	// Success
	return 0;
}


/**
* .
* @return: success Return 0.
*          video failure Return -1.
*          audio failure Return -2.
*          gameOption failureboth Return -3.
*			other failure Return -4.
*/
int CSystemProperties::Apply() {
	int nVideo = ApplyVideo();
	int nAudio = ApplyAudio();
	int nGameOption = ApplyGameOption();
	if (nVideo == 0 && nAudio == 0 && nGameOption == 0)
		return 0;
	if (nVideo != 0)
		return -1;
	if (nAudio != 0)
		return -2;
	if (nGameOption != 0)
		return -3;
	return -4;
}

namespace {
	//  Допустимый диапазон целевого FPS — нижняя граница 30 (исторический минимум,
	//  под который настроены анимации и физика), верхняя 240 (физический потолок
	//  для современных high-refresh мониторов).
	constexpr int kMinFps = 30;
	constexpr int kMaxFps = 240;

	//  Парсер ini-ключа [gameOption] framerate с backward-compat:
	//    0    → 30 (legacy «30 FPS режим»)
	//    1    → 60 (legacy «60 FPS режим»)
	//    N≥30 → N (clamp в [kMinFps, kMaxFps])
	//    иначе → 60 (разумный дефолт для современных машин)
	int ParseFramerate(std::int64_t raw) {
		if (raw == 0) return 30;
		if (raw == 1) return 60;
		if (raw < kMinFps) return 60;
		if (raw > kMaxFps) return kMaxFps;
		return static_cast<int>(raw);
	}

	//  Записывает значение только если оно отличается от текущего в секции
	//  (или ключа ещё нет). Возвращает true, если что-то реально изменилось —
	//  вызывающая сторона по этому флагу решает, нужно ли сохранять файл на диск.
	//  Sentinel заведомо отличен от value, чтобы отличить "ключа нет" от
	//  "ключ есть и равен value" одним вызовом GetInt64.
	bool SetInt64IfChanged(dbc::IniSection& sec, std::string_view key, std::int64_t value) {
		//  Sentinel — заведомо отличный от value: используется как маркер "ключа нет".
		constexpr auto kMin = std::numeric_limits<std::int64_t>::min();
		constexpr auto kMax = std::numeric_limits<std::int64_t>::max();
		const std::int64_t sentinel = (value == kMin) ? kMax : kMin;
		if (sec.GetInt64(key, sentinel) == value) {
			return false;
		}
		sec.SetInt64(key, value);
		return true;
	}
} // namespace

/**
* Чтение настроек из глобального g_SystemIni (загружен из system.ini в Main.cpp).
* @return: 0 Success.
*          -1 File is not exist.
*          -2 File is destroyed.
*			-3 Filename is null or its length is zero.
*/
int CSystemProperties::readFromFile() {
	auto& video = g_SystemIni["video"];
	m_videoProp.nTexture = static_cast<int>(video.GetInt64("texture", 0));
	m_videoProp.bAnimation = video.GetInt64("animation", 0) != 0;
	m_videoProp.bCameraRotate = video.GetInt64("cameraRotate", 0) != 0;
	m_videoProp.bGroundMark = video.GetInt64("groundMark", 0) != 0;
	m_videoProp.bDepth32 = video.GetInt64("depth32", 0) != 0;
	m_videoProp.nQuality = static_cast<int>(video.GetInt64("quality", 0));
	m_videoProp.bFullScreen = video.GetInt64("fullScreen", 0) != 0;
	m_videoProp.bResolution = static_cast<int>(video.GetInt64("resolution", 0));

	auto& audio = g_SystemIni["audio"];
	m_audioProp.nMusicSound = static_cast<int>(audio.GetInt64("musicSound", 0));
	m_audioProp.nMusicEffect = static_cast<int>(audio.GetInt64("musicEffect", 0));

	auto& game = g_SystemIni["gameOption"];
	m_gameOption.bRunMode = game.GetInt64("runMode", 0) != 0;
	m_gameOption.bHelpMode = game.GetInt64("helpMode", 1) != 0;
	m_gameOption.bCameraMode = game.GetInt64("cameraMode", 0) != 0;
	m_gameOption.bAppMode = game.GetInt64("apparel", 0) != 0;
	m_gameOption.bEffMode = game.GetInt64("effect", 0) != 0;
	m_gameOption.bStateMode = game.GetInt64("state", 0) != 0;
	m_gameOption.bEnemyNames = game.GetInt64("enemynames", 0) != 0;
	m_gameOption.bShowBars = game.GetInt64("showbars", 0) != 0;
	m_gameOption.bShowPercentages = game.GetInt64("showpercentages", 0) != 0;
	m_gameOption.bShowInfo = game.GetInt64("showinfo", 0) != 0;
	m_gameOption.nFramerate = ParseFramerate(game.GetInt64("framerate", 0));
	m_gameOption.bVsync = game.GetInt64("vsync", 0) != 0;
	m_gameOption.bShowMounts = game.GetInt64("showmounts", 0) != 0;

	auto& start = g_SystemIni["startOption"];
	m_startOption.bFirst = start.GetInt64("first", 0) != 0;

	return 0;
}

/**
* Запись настроек в глобальный g_SystemIni; если хотя бы одно значение
* отличается от текущего в ini — синхронизируется на диск, иначе нет.
* @return: 0 Success.
*          -1 Error.
*			-2 File can not be created.
*			-3 Filename is null or its length is zero.
*/
int CSystemProperties::writeToFile() {
	bool changed = false;

	auto& video = g_SystemIni["video"];
	changed |= SetInt64IfChanged(video, "texture", m_videoProp.nTexture);
	changed |= SetInt64IfChanged(video, "animation", m_videoProp.bAnimation ? 1 : 0);
	changed |= SetInt64IfChanged(video, "cameraRotate", m_videoProp.bCameraRotate ? 1 : 0);
	changed |= SetInt64IfChanged(video, "groundMark", m_videoProp.bGroundMark ? 1 : 0);
	changed |= SetInt64IfChanged(video, "depth32", m_videoProp.bDepth32 ? 1 : 0);
	changed |= SetInt64IfChanged(video, "quality", m_videoProp.nQuality);
	changed |= SetInt64IfChanged(video, "fullScreen", m_videoProp.bFullScreen ? 1 : 0);
	changed |= SetInt64IfChanged(video, "resolution", m_videoProp.bResolution);

	auto& audio = g_SystemIni["audio"];
	changed |= SetInt64IfChanged(audio, "musicSound", m_audioProp.nMusicSound);
	changed |= SetInt64IfChanged(audio, "musicEffect", m_audioProp.nMusicEffect);

	auto& game = g_SystemIni["gameOption"];
	changed |= SetInt64IfChanged(game, "runMode", 1); // всегда true (как в оригинале)
	changed |= SetInt64IfChanged(game, "helpMode", m_gameOption.bHelpMode ? 1 : 0);
	changed |= SetInt64IfChanged(game, "cameraMode", m_gameOption.bCameraMode ? 1 : 0);
	changed |= SetInt64IfChanged(game, "apparel", m_gameOption.bAppMode ? 1 : 0);
	changed |= SetInt64IfChanged(game, "effect", m_gameOption.bEffMode ? 1 : 0);
	changed |= SetInt64IfChanged(game, "state", m_gameOption.bStateMode ? 1 : 0);
	changed |= SetInt64IfChanged(game, "enemynames", m_gameOption.bEnemyNames ? 1 : 0);
	changed |= SetInt64IfChanged(game, "showbars", m_gameOption.bShowBars ? 1 : 0);
	changed |= SetInt64IfChanged(game, "showpercentages", m_gameOption.bShowPercentages ? 1 : 0);
	changed |= SetInt64IfChanged(game, "showinfo", m_gameOption.bShowInfo ? 1 : 0);
	changed |= SetInt64IfChanged(game, "framerate", m_gameOption.nFramerate);
	changed |= SetInt64IfChanged(game, "vsync", m_gameOption.bVsync ? 1 : 0);
	changed |= SetInt64IfChanged(game, "showmounts", m_gameOption.bShowMounts ? 1 : 0);

	changed |= SetInt64IfChanged(g_SystemIni["startOption"], "first", m_startOption.bFirst ? 1 : 0);

	if (changed) {
		g_SystemIni.Save();
	}
	return 0;
}


//video
static int g_nCbxTexture;
static int g_nCbxMovie;
static int g_nCbxCamera;
static int g_nCbxView;
static int g_nCbxTrail;
static int g_nCbxColor;
static int g_nCboResolution;
static int g_nCbxModel;
static int g_bCbxQuality;
static float g_fPosMusic = -1.0f; //
static float g_fPosMidi = -1.0f;
static bool g_bChangeAudio = false; //

//---------------------------------------------------------------------------
// class CVideoMgr
//---------------------------------------------------------------------------

CSystemMgr::CSystemMgr()
	: m_isLoad(false), frmSystem(0), frmAudio(0), proAudioMusic(0), proAudioMidi(0),
	  frmVideo(0), cbxTexture(0), cbxMovie(0), cbxCamera(0), cbxTrail(0),
	  cbxColor(0), cboResolution(0), cbxModel(0), cbxQuality(0), frmAskRelogin(0),
	  frmAskExit(0), frmAskOfflineMode(0), frmAskChange(0) {
}


void CSystemMgr::LoadCustomProp() {
	//(Michael Chen 2005-04-27)

	if (!m_isLoad) {
		if (m_sysProp.Load()) {
			//,
			m_sysProp.m_videoProp.nTexture = 0;
			m_sysProp.m_videoProp.bAnimation = true;
			m_sysProp.m_videoProp.bCameraRotate = true;
			//m_sysProp.m_videoProp.bViewFar=true;      //(Michael Chen 2005-04-22
			m_sysProp.m_videoProp.bGroundMark = true;
			m_sysProp.m_videoProp.bDepth32 = true;
			m_sysProp.m_videoProp.nQuality = 0;
			m_sysProp.m_videoProp.bFullScreen = false;
			m_sysProp.m_videoProp.bResolution = 0;

			m_sysProp.m_audioProp.nMusicSound = static_cast<int>(10.0f * g_pGameApp->GetMusicSize());
			m_sysProp.m_audioProp.nMusicEffect = static_cast<int>(10.0f * g_pGameApp->GetCurScene()->GetSoundSize());

			m_sysProp.m_gameOption.bRunMode = true;
			m_sysProp.m_gameOption.bLockMode = false;
			m_sysProp.m_gameOption.bCameraMode = true;
			m_sysProp.m_gameOption.bAppMode = true;
			m_sysProp.m_gameOption.bEffMode = true;
			m_sysProp.m_gameOption.bStateMode = false;
			m_sysProp.m_gameOption.bEnemyNames = true;
			m_sysProp.m_gameOption.bShowBars = false;
			m_sysProp.m_gameOption.bShowPercentages = false;
			m_sysProp.m_gameOption.bShowInfo = false;
			m_sysProp.m_gameOption.nFramerate = 60;
			m_sysProp.m_gameOption.bVsync = false;
			m_sysProp.m_gameOption.bShowMounts = true;
		}
		//	m_sysProp.m_gameOption.bRunMode = true;	//true
		m_isLoad = true;
	}
	CCharacter::SetIsShowShadow(m_sysProp.m_videoProp.bGroundMark);
	CCharacter::SetIsShowApparel(m_sysProp.m_gameOption.bAppMode);
	CCharacter::SetIsShowEffects(m_sysProp.m_gameOption.bEffMode);
	g_IsShowStates = m_sysProp.m_gameOption.bStateMode;
	g_IsCameraMode = m_sysProp.m_gameOption.bCameraMode;

	CHeadSay::SetIsShowEnemyNames(m_sysProp.m_gameOption.bEnemyNames);
	CHeadSay::SetIsShowBars(m_sysProp.m_gameOption.bShowBars);
	CHeadSay::SetIsShowPercentages(m_sysProp.m_gameOption.bShowPercentages);
	CHeadSay::SetIsShowInfo(m_sysProp.m_gameOption.bShowInfo);
	Corsairs::Client::Frame::SteadyFrameSync::Instance().SetFps(static_cast<std::uint32_t>(m_sysProp.m_gameOption.nFramerate));
}

bool CSystemMgr::Init() {
	frmSystem = _FindForm("frmSystem"); // 
	if (!frmSystem) return false;
	frmSystem->evtEntrustMouseEvent = _evtSystemFromMouseEvent;


	LoadCustomProp(); //

	///////////// Video
	frmVideo = _FindForm("frmVideo");
	if (!frmVideo) return false;
	frmVideo->evtEntrustMouseEvent = _evtVideoFormMouseEvent;

	cbxTexture = (CCheckGroup*)frmVideo->Find("cbxTexture"); //
	if (!cbxTexture) return Error("msgui.clu<%s><%s>", frmVideo->GetName(), "cbxTexture");
	cbxTexture->SetActiveIndex(m_sysProp.m_videoProp.nTexture); // Michael Chen 2005-04-22

	cbxMovie = (CCheckGroup*)frmVideo->Find("cbxMovie"); //
	if (!cbxMovie) return Error("msgui.clu<%s><%s>", frmVideo->GetName(), "cbxMovie");
	cbxMovie->SetActiveIndex(m_sysProp.m_videoProp.bAnimation ? 0 : 1); // Michael Chen 2005-04-22

	cbxCamera = (CCheckGroup*)frmVideo->Find("cbxCamera"); //
	if (!cbxCamera) return Error("msgui.clu<%s><%s>", frmVideo->GetName(), "cbxCamera");
	cbxCamera->SetActiveIndex(m_sysProp.m_videoProp.bCameraRotate ? 0 : 1); // Michael Chen 2005-04-22

	/** (Michael Chen 2005-04-22
	cbxView = ( CCheckGroup *)frmVideo->Find( "cbxView" ); //
	if (! cbxView )		return Error( "msgui.clu<%s><%s>", frmVideo->GetName(), "cbxView" );
	cbxView->SetActiveIndex(m_sysProp.m_videoProp.bViewFar ? 0 : 1);    // Michael Chen 2005-04-22
	*/

	cbxTrail = (CCheckGroup*)frmVideo->Find("cbxTrail"); //
	if (!cbxTrail) return Error(GetLanguageString(45).c_str(), frmVideo->GetName(), "cbxTrail");
	cbxTrail->SetActiveIndex(m_sysProp.m_videoProp.bGroundMark ? 0 : 1); // Michael Chen 2005-04-22

	cbxColor = (CCheckGroup*)frmVideo->Find("cbxColor"); //
	if (!cbxColor) return Error(GetLanguageString(45).c_str(), frmVideo->GetName(), "cbxColor");
	cbxColor->SetActiveIndex(m_sysProp.m_videoProp.bDepth32 ? 1 : 0); // Michael Chen 2005-04-22

	cboResolution = (CCombo*)frmVideo->Find("cboResolution"); //
	if (!cboResolution) return Error(GetLanguageString(45).c_str(), frmVideo->GetName(), "cboResolution");
	cboResolution->GetList()->GetItems()->Select(m_sysProp.m_videoProp.bResolution);

	cbxModel = (CCheckGroup*)frmVideo->Find("cbxModel"); //
	if (!cbxModel) return Error(GetLanguageString(45).c_str(), frmVideo->GetName(), "cbxModel");
	cbxModel->SetActiveIndex(m_sysProp.m_videoProp.bFullScreen ? 0 : 1); // Michael Chen 2005-04-22

	cbxQuality = (CCheckGroup*)frmVideo->Find("cbxQuality"); //
	if (!cbxQuality) return Error(GetLanguageString(45).c_str(), frmVideo->GetName(), "cbxQuality");
	cbxQuality->SetActiveIndex(m_sysProp.m_videoProp.nQuality); // Michael Chen 2005-04-22
	cbxQuality->evtSelectChange = _evtVideoChangeChange;

	//////////Audio
	frmAudio = _FindForm("frmAudio");
	if (!frmAudio) return false;
	frmAudio->evtEntrustMouseEvent = _evtAudioFormMouseEvent;

	proAudioMusic = dynamic_cast<CProgressBar*>(frmAudio->Find("proAudioMusic"));
	if (!proAudioMusic) return Error(GetLanguageString(45).c_str(), frmAudio->GetName(), "proAudioMusic");
	proAudioMusic->SetPosition(static_cast<float>(m_sysProp.m_audioProp.nMusicSound)); // Michael Chen 2005-04-22
	proAudioMusic->evtMouseDown = _evtMainMusicMouseDown;

	proAudioMidi = dynamic_cast<CProgressBar*>(frmAudio->Find("proAudioMidi"));
	if (!proAudioMidi) return Error(GetLanguageString(45).c_str(), frmAudio->GetName(), "proAudioMidi");
	proAudioMidi->SetPosition(static_cast<float>(m_sysProp.m_audioProp.nMusicEffect)); // Michael Chen 2005-04-22
	proAudioMidi->evtMouseDown = _evtMainMusicMouseDown;

	//////// GameOption
	frmGameOption = _FindForm("frmGame");
	if (!frmGameOption) return Error(GetLanguageString(45).c_str(), frmGameOption->GetName(), "frmGame");
	frmGameOption->evtEntrustMouseEvent = _evtGameOptionFormMouseDown;
	frmGameOption->evtBeforeShow = _evtGameOptionFormBeforeShow;

	cbxRunMode = (CCheckGroup*)frmGameOption->Find("cbxRunmodel");
	if (!cbxRunMode) return Error(GetLanguageString(45).c_str(), frmGameOption->GetName(), "cbxRunmodel");

	cbxLockMode = (CCheckGroup*)frmGameOption->Find("cbxLockmodel");
	if (!cbxLockMode) return Error(GetLanguageString(45).c_str(), frmGameOption->GetName(), "cbxLockmodel");

	cbxHelpMode = (CCheckGroup*)frmGameOption->Find("cbxHelpmodel");
	if (!cbxHelpMode) return Error(GetLanguageString(45).c_str(), frmGameOption->GetName(), "cbxHelpmodel");

	cbxCameraMode = static_cast<CCheckGroup*>(frmGameOption->Find("cbxCameraMode_p"));
	if (!cbxCameraMode) {
		return Error(GetLanguageString(45).c_str(), frmGameOption->GetName(), "cbxCameraMode_p");
	}

	cbxAppMode = (CCheckGroup*)frmGameOption->Find("cbxAppmodel");
	if (!cbxAppMode) return Error(GetLanguageString(45).c_str(), frmGameOption->GetName(), "cbxAppmodel");

	cbxEffMode = (CCheckGroup*)frmGameOption->Find("cbxEffmodel");
	if (!cbxEffMode) return Error(GetLanguageString(45).c_str(), frmGameOption->GetName(), "cbxEffmodel");

	cbxStateMode = (CCheckGroup*)frmGameOption->Find("cbxStatemodel");
	if (!cbxStateMode) return Error(GetLanguageString(45).c_str(), frmGameOption->GetName(), "cbxStatemodel");

	// Added by mdrst May 2020 - FPO alpha

	cbxEnemyNames = (CCheckGroup*)frmGameOption->Find("cbxEnemyNames_p");
	if (!cbxEnemyNames) return Error(GetLanguageString(45).c_str(), frmGameOption->GetName(), "cbxEnemyNames_p");

	cbxShowBars = (CCheckGroup*)frmGameOption->Find("cbxShowBars_p");
	if (!cbxShowBars) return Error(GetLanguageString(45).c_str(), frmGameOption->GetName(), "cbxShowBars_p");
	cbxShowPercentages = (CCheckGroup*)frmGameOption->Find("cbxShowPercentages_p");
	if (!cbxShowPercentages) return Error(GetLanguageString(45).c_str(), frmGameOption->GetName(),
										  "cbxShowPercentages_p");

	cbxShowInfo = (CCheckGroup*)frmGameOption->Find("cbxShowInfo_p");
	if (!cbxShowInfo) return Error(GetLanguageString(45).c_str(), frmGameOption->GetName(), "cbxShowInfo_p");
	cbxFramerate = (CCheckGroup*)frmGameOption->Find("cbxFramerate_p");
	if (!cbxFramerate) return Error(GetLanguageString(45).c_str(), frmGameOption->GetName(), "cbxFramerate_p");

	cbxShowMounts = static_cast<CCheckGroup*>(frmGameOption->Find("cbxShowMounts_p"));
	if (!cbxShowMounts) {
		return Error(GetLanguageString(45).c_str(), frmGameOption->GetName(), "cbxShowMounts_p");
	}


	//////// 
	frmAskRelogin = _FindForm("frmAskRelogin");
	if (frmAskRelogin) frmAskRelogin->evtEntrustMouseEvent = _evtAskReloginFormMouseDown;

	frmAskExit = _FindForm("frmAskExit");
	if (frmAskExit) frmAskExit->evtEntrustMouseEvent = _evtAskExitFormMouseDown;

	frmAskOfflineMode = _FindForm("frmAskOfflineMode");
	if (frmAskOfflineMode) frmAskOfflineMode->evtEntrustMouseEvent = _evtAskOfflineModeFormMouseDown;

	frmAskChange = _FindForm("frmAskChange");
	if (frmAskChange) frmAskChange->evtEntrustMouseEvent = _evtAskChangeFormMouseDown;

	return true;
}

void CSystemMgr::End() {
	const std::string_view Value{cboResolution->GetText()};
	int setResolution;
	if (Value == "800x600") {
		setResolution = 0;
	}
	if (Value == "1152x648") {
		setResolution = 1;
	}
	else if (Value == "1280x720") {
		setResolution = 2;
	}
	else if (Value == "1366x768") {
		setResolution = 3;
	}
	else if (Value == "1600x900") {
		setResolution = 4;
	}
	else if (Value == "1920x1080") {
		setResolution = 5;
	}

	//(Michael Chen 2005-04-19)
	if (cbxTexture)
		m_sysProp.m_videoProp.nTexture = cbxTexture->GetActiveIndex();
	if (cbxMovie)
		m_sysProp.m_videoProp.bAnimation = cbxMovie->GetActiveIndex() == 0 ? true : false;
	if (cbxCamera)
		m_sysProp.m_videoProp.bCameraRotate = cbxCamera->GetActiveIndex() == 0 ? true : false;
	//m_sysProp.m_videoProp.bViewFar = cbxView->GetActiveIndex() == 0 ? true : false;(Michael Chen 2005-04-22
	if (cbxTrail)
		m_sysProp.m_videoProp.bGroundMark = cbxTrail->GetActiveIndex() == 0 ? true : false;
	if (cbxColor)
		m_sysProp.m_videoProp.bDepth32 = cbxColor->GetActiveIndex() == 0 ? false : true;
	if (cbxQuality)
		m_sysProp.m_videoProp.nQuality = cbxQuality->GetActiveIndex();
	if (cbxModel)
		m_sysProp.m_videoProp.bFullScreen = cbxModel->GetActiveIndex() == 0 ? true : false;
	if (cboResolution)
		m_sysProp.m_videoProp.bResolution = setResolution;
	if (proAudioMusic)
		m_sysProp.m_audioProp.nMusicSound = static_cast<int>(proAudioMusic->GetPosition());
	if (proAudioMidi)
		m_sysProp.m_audioProp.nMusicEffect = static_cast<int>(proAudioMidi->GetPosition());

	if (cbxRunMode)
		m_sysProp.m_gameOption.bRunMode = cbxRunMode->GetActiveIndex() == 0 ? false : true;

	//if (cbxAppMode)
	//	m_sysProp.m_gameOption.bAppMode = cbxAppMode->GetActiveIndex() == 0 ? false : true;

	if (m_sysProp.Save()) {
		// error when save the system properties.
	}
	//end of modifying by Michael Chen
}

void CSystemMgr::_evtVideoChangeChange(CGuiData* pSender) {
	CCheckGroup* g = dynamic_cast<CCheckGroup*>(pSender);
	if (!g) return;

	if (g->GetActiveIndex() == 0) {
		g_stUISystem.cbxTexture->SetActiveIndex(0);
		g_stUISystem.cbxMovie->SetActiveIndex(0);
		g_stUISystem.cbxCamera->SetActiveIndex(0);
		//g_stUISystem.cbxView->SetActiveIndex(0);//(Michael Chen 2005-04-22
		g_stUISystem.cbxTrail->SetActiveIndex(0);
		g_stUISystem.cbxColor->SetActiveIndex(0);
	}
	else if (g->GetActiveIndex() == 1) {
		g_stUISystem.cbxTexture->SetActiveIndex(1);
		g_stUISystem.cbxMovie->SetActiveIndex(0);
		g_stUISystem.cbxCamera->SetActiveIndex(1);
		//g_stUISystem.cbxView->SetActiveIndex(0);//(Michael Chen 2005-04-22
		g_stUISystem.cbxTrail->SetActiveIndex(0);
		g_stUISystem.cbxColor->SetActiveIndex(1);
	}
	else if (g->GetActiveIndex() == 2) {
		g_stUISystem.cbxTexture->SetActiveIndex(2);
		g_stUISystem.cbxMovie->SetActiveIndex(1);
		g_stUISystem.cbxCamera->SetActiveIndex(1);
		//g_stUISystem.cbxView->SetActiveIndex(1);//(Michael Chen 2005-04-22
		g_stUISystem.cbxTrail->SetActiveIndex(1);
		g_stUISystem.cbxColor->SetActiveIndex(1);
	}
}

void CSystemMgr::_evtVideoFormMouseEvent(CCompent* pSender, int nMsgType, int x, int y, DWORD dwKey) {
	std::string name = pSender->GetName();
	if (name == "btnYes") {
		int nTextureHigh = g_stUISystem.cbxTexture->GetActiveIndex();
		bool bMovieOn = g_stUISystem.cbxMovie->GetActiveIndex() == 0 ? true : false;
		bool bCameraOn = g_stUISystem.cbxCamera->GetActiveIndex() == 0 ? true : false;
		//bool bViewFar     = g_stUISystem.cbxView->GetActiveIndex()==0?true:false;//(Michael Chen 2005-04-22
		bool bTrailOn = g_stUISystem.cbxTrail->GetActiveIndex() == 0 ? true : false;
		CCharacter::SetIsShowShadow(bTrailOn);
		D3DFORMAT format = g_stUISystem.cbxColor->GetActiveIndex() == 0 ? D3DFMT_D24X8 : D3DFMT_D16;

		int width;
		int height;

		const std::string_view getValue{g_stUISystem.cboResolution->GetText()};
		if (getValue == "800x600") {
			width = TINY_RES_X;
			height = TINY_RES_Y;
		}
		if (getValue == "1152x648") {
			width = SMALL_RES_X;
			height = SMALL_RES_Y;
		}
		else if (getValue == "1280x720") {
			width = MID_RES_X;
			height = MID_RES_Y;
		}
		else if (getValue == "1366x768") {
			width = LARGE_RES_X;
			height = LARGE_RES_Y;
		}
		else if (getValue == "1600x900") {
			width = EXTRA_LARGE_RES_X;
			height = EXTRA_LARGE_RES_Y;
		}
		else if (getValue == "1920x1080") {
			width = FULL_LARGE_RES_X;
			height = FULL_LARGE_RES_Y;
		}

		bool bWindowed = g_stUISystem.cbxModel->GetActiveIndex() == 0 ? false : true;

		g_pGameApp->GetMainCam()->EnableRotation(bCameraOn);
		g_stUISystem.m_sysProp.m_videoProp.bCameraRotate = bCameraOn;

		if (bCameraOn == false) {
			g_Render.EnableClearTarget(FALSE);
		}
		else {
			g_Render.EnableClearTarget(TRUE);
		}
		//g_pGameApp->GetMainCam()->EnableUpdown( bViewFar ) ;//(Michael Chen 2005-04-22
		g_pGameApp->GetCurScene()->SetTextureLOD(nTextureHigh);

		GlobalAppConfig.SetFullScreen(false);
		if (!bWindowed) {
			width = GetSystemMetrics(SM_CXSCREEN);
			height = GetSystemMetrics(SM_CYSCREEN);
			bWindowed = TRUE;
			GlobalAppConfig.SetFullScreen(true);
		}

		GetRender().SetIsChangeResolution(true);

		MPIDeviceObject* dev_obj = g_Render.GetInterfaceMgr()->dev_obj;
		if (FAILED(dev_obj->CheckCurrentDeviceFormat(BBFI_DEPTHSTENCIL, format))) {
			format = D3DFMT_D16;
		}
		g_pGameApp->ChangeVideoStyle(width, height, format, bWindowed);
		pSender->GetForm()->SetIsShow(false);

		g_stUISystem.frmSystem->SetIsShow(false);
		return;
	}
	else if (name == "btnNo" || name == "btnClose") {
		g_stUISystem.cbxTexture->SetActiveIndex(g_nCbxTexture);
		g_stUISystem.cbxMovie->SetActiveIndex(g_nCbxMovie);
		g_stUISystem.cbxCamera->SetActiveIndex(g_nCbxCamera);
		//g_stUISystem.cbxView->SetActiveIndex(g_nCbxView);//(Michael Chen 2005-04-22
		g_stUISystem.cbxTrail->SetActiveIndex(g_nCbxTrail);
		g_stUISystem.cbxColor->SetActiveIndex(g_nCbxColor);
		g_stUISystem.cbxModel->SetActiveIndex(g_nCbxModel);
		g_stUISystem.cbxQuality->SetActiveIndex(g_bCbxQuality);

		pSender->GetForm()->SetIsShow(false);
		return;
	}
}

void CSystemMgr::_evtSystemFromMouseEvent(CCompent* pSender, int nMsgType, int x, int y, DWORD dwKey) {
	string name = pSender->GetName();

	//frmSystem
	if (name == "btnClose" || name == "btnNo") {
		pSender->GetForm()->Close();
		return;
	}
	if (name == "btnChange") {
		CForm* f = CFormMgr::s_Mgr.Find("frmAskChange");
		if (f) f->SetIsShow(true);
		g_stUISystem.frmSystem->SetIsShow(false);
		return;
	}
	else if (name == "btnGame") {
		CForm* f = CFormMgr::s_Mgr.Find("frmGame");
		if (f) f->SetIsShow(true);
		g_stUISystem.frmSystem->SetIsShow(false);
		return;
	}
	else if (name == "btnRelogin") {
		CForm* f = CFormMgr::s_Mgr.Find("frmAskRelogin");
		if (f) f->SetIsShow(true);
		g_stUISystem.frmSystem->SetIsShow(false);
		return;
	}
	else if (name == "btnExit") {
		CForm* f = CFormMgr::s_Mgr.Find("frmAskExit");
		if (f) f->SetIsShow(true);
		g_stUISystem.frmSystem->SetIsShow(false);
		return;
	}
	else if (name == "btnOfflineMode") {
		CForm* f = CFormMgr::s_Mgr.Find("frmAskOfflineMode");
		if (f) f->SetIsShow(true);
		g_stUISystem.frmSystem->SetIsShow(false);
		return;
	}
	else if (name == "btnAudio") {
		CForm* f = CFormMgr::s_Mgr.Find("frmAudio");
		if (!f) return;

		f->SetIsShow(true);
		if (g_fPosMusic < 0.0f && g_fPosMusic < 0.0f) {
			g_fPosMusic = g_stUISystem.proAudioMusic->GetPosition();
			g_fPosMidi = g_stUISystem.proAudioMidi->GetPosition();
			g_pGameApp->SetMusicSize(g_fPosMusic / 10.0f);
			g_pGameApp->GetCurScene()->SetSoundSize(g_fPosMidi / 10.0f);
			g_pGameApp->mSoundManager->SetVolume(g_fPosMidi / 10.0f);
		}
		if (g_bChangeAudio) {
			g_fPosMusic = g_stUISystem.proAudioMusic->GetPosition();
			g_fPosMidi = g_stUISystem.proAudioMidi->GetPosition();
			g_pGameApp->SetMusicSize(g_fPosMusic / 10.0f);
			g_pGameApp->GetCurScene()->SetSoundSize(g_fPosMidi / 10.0f);
			g_pGameApp->mSoundManager->SetVolume(g_fPosMidi / 10.0f);
		}
		g_stUISystem.frmSystem->SetIsShow(false);
		return;
	}
	else if (name == "btnVideo") {
		CForm* f = CFormMgr::s_Mgr.Find("frmVideo");
		if (!f) return;

		f->SetIsShow(true);
		g_nCbxTexture = g_stUISystem.cbxTexture->GetActiveIndex();
		g_nCbxMovie = g_stUISystem.cbxMovie->GetActiveIndex();
		g_nCbxCamera = g_stUISystem.cbxCamera->GetActiveIndex();
		//g_nCbxView    = g_stUISystem.cbxView->GetActiveIndex();//(Michael Chen 2005-04-22
		g_nCbxTrail = g_stUISystem.cbxTrail->GetActiveIndex();
		g_nCbxColor = g_stUISystem.cbxColor->GetActiveIndex();
		g_nCboResolution = g_stUISystem.cboResolution->GetList()->GetItems()->GetSelect()->GetIndex();
		g_nCbxModel = g_stUISystem.cbxModel->GetActiveIndex();
		g_bCbxQuality = g_stUISystem.cbxQuality->GetActiveIndex();
		g_stUISystem.frmSystem->SetIsShow(false);
		return;
	}
}

void CSystemMgr::_evtAudioFormMouseEvent(CCompent* pSender, int nMsgType, int x, int y, DWORD dwKey) {
	string name = pSender->GetName();

	//frmAudio
	if (name == "btnYes") //
	{
		g_fPosMusic = g_stUISystem.proAudioMusic->GetPosition();
		g_fPosMidi = g_stUISystem.proAudioMidi->GetPosition();
		g_pGameApp->SetMusicSize(g_fPosMusic / 10.0f);
		g_pGameApp->GetCurScene()->SetSoundSize(g_fPosMidi / 10.0f);
		g_pGameApp->mSoundManager->SetVolume(g_fPosMidi / 10.0f);

		g_bChangeAudio = true;
		pSender->GetForm()->SetIsShow(false);
		return;
	}
	else if (name == "btnNo") // 
	{
		g_stUISystem.proAudioMusic->SetPosition(g_fPosMusic);
		g_stUISystem.proAudioMidi->SetPosition(g_fPosMidi);
		g_pGameApp->SetMusicSize(g_fPosMusic / 10.0f);
		g_pGameApp->GetCurScene()->SetSoundSize(g_fPosMidi / 10.0f);
		g_pGameApp->mSoundManager->SetVolume(g_fPosMidi / 10.0f);
		g_bChangeAudio = false;
		pSender->GetForm()->SetIsShow(false);
		return;
	}
}

void CSystemMgr::_evtMainMusicMouseDown(CGuiData* pSender, int x, int y, DWORD key) {
	CProgressBar* proAudioMidi = g_stUISystem.proAudioMidi;
	CProgressBar* proAudioMusic = g_stUISystem.proAudioMusic;

	string name = pSender->GetName();
	float fPos;

	if (stricmp("frmAudio", pSender->GetForm()->GetName()) == 0) {
		if (name == "proAudioMusic") {
			fPos = 10.0f * (float)(x - proAudioMusic->GetLeft() - proAudioMusic->GetForm()->GetLeft()) / (float)
				proAudioMusic->GetWidth();
			proAudioMusic->SetPosition(fPos);
			proAudioMusic->Refresh();
			g_pGameApp->SetMusicSize(fPos / 10.0f);
		}
		else if (name == "proAudioMidi") {
			fPos = 10.0f * (float)(x - proAudioMidi->GetLeft() - proAudioMidi->GetForm()->GetLeft()) / (float)
				proAudioMidi->GetWidth();
			proAudioMidi->SetPosition(fPos);
			proAudioMidi->Refresh();
			g_pGameApp->mSoundManager->SetVolume(fPos / 10.0f);
			g_pGameApp->GetCurScene()->SetSoundSize(fPos / 10.0f);
		}
	}
}

void CSystemMgr::_evtAskReloginFormMouseDown(CCompent* pSender, int nMsgType, int x, int y, DWORD dwKey) {
	string name = pSender->GetName();
	pSender->GetForm()->SetIsShow(false);

	if (name == "btnYes") {
		g_ChaExitOnTime.Relogin();
		return;
	}
}

void CSystemMgr::_evtAskExitFormMouseDown(CCompent* pSender, int nMsgType, int x, int y, DWORD dwKey) {
	string name = pSender->GetName();
	pSender->GetForm()->SetIsShow(false);

	if (name == "btnYes") {
		g_ChaExitOnTime.ExitApp();
	}
}

void CSystemMgr::_evtAskOfflineModeFormMouseDown(CCompent* pSender, int nMsgType, int x, int y, DWORD dwKey) {
	string name = pSender->GetName();
	pSender->GetForm()->SetIsShow(false);

	if (name == "btnYes") {
		g_ChaExitOnTime.OfflineMode();
	}
}

void CSystemMgr::_evtAskChangeFormMouseDown(CCompent* pSender, int nMsgType, int x, int y, DWORD dwKey) {
	string name = pSender->GetName();
	pSender->GetForm()->SetIsShow(false);

	if (name == "btnYes") // 
	{
		g_ChaExitOnTime.ChangeCha();
	}
}

void CSystemMgr::_evtGameOptionFormMouseDown(CCompent* pSender, int nMsgType, int x, int y, DWORD dwKey) {
	string name = pSender->GetName();
	if (name != "btnYes") return;

	CCheckGroup* pGroup = g_stUISystem.cbxRunMode;
	if (pGroup) {
		const auto v = pGroup->GetActiveIndex() == 0 ? false : true;
		if (v != g_stUISystem.m_sysProp.m_gameOption.bRunMode) {
			g_stUISystem.m_sysProp.m_gameOption.bRunMode = v;
			g_stUISystem.m_sysProp.ApplyGameOption();
		}
	}

	// 
	pGroup = g_stUISystem.cbxLockMode;
	if (pGroup) {
		const auto v = pGroup->GetActiveIndex() == 1 ? true : false;
		if (v != g_stUISystem.m_sysProp.m_gameOption.bLockMode) {
			g_stUISystem.m_sysProp.m_gameOption.bLockMode = v;
			CS_AutoKitbagLock(g_stUISystem.m_sysProp.m_gameOption.bLockMode);
		}
	}

	// 
	pGroup = g_stUISystem.cbxHelpMode;
	if (pGroup) {
		const bool bHelpMode = pGroup->GetActiveIndex() == 1 ? true : false;
		if (bHelpMode != g_stUISystem.m_sysProp.m_gameOption.bHelpMode) {
			g_stUISystem.m_sysProp.m_gameOption.bHelpMode = bHelpMode;
			if (!bHelpMode) g_stUIStart.ShowLevelUpHelpButton(bHelpMode);
			g_stUIStart.ShowInfoCenterButton(bHelpMode);
			g_SystemIni["gameOption"].SetInt64("helpMode", bHelpMode ? 1 : 0);
			g_SystemIni.Save();
		}
	}

	pGroup = g_stUISystem.cbxCameraMode;
	if (pGroup) {
		const bool bCameraMode = pGroup->GetActiveIndex() == 1 ? true : false;
		if (bCameraMode != g_stUISystem.m_sysProp.m_gameOption.bCameraMode) {
			g_stUISystem.m_sysProp.m_gameOption.bCameraMode = bCameraMode;
			g_IsCameraMode = bCameraMode;
			g_SystemIni["gameOption"].SetInt64("cameraMode", bCameraMode ? 1 : 0);
			g_SystemIni.Save();
		}
	}

	pGroup = g_stUISystem.cbxAppMode;
	if (pGroup) {
		const bool bAppMode = pGroup->GetActiveIndex() == 1 ? true : false;
		if (bAppMode != g_stUISystem.m_sysProp.m_gameOption.bAppMode) {
			g_stUISystem.m_sysProp.m_gameOption.bAppMode = bAppMode;
			CCharacter::SetIsShowApparel(bAppMode);
			g_stUIStart.RefreshPet();
			//for each player in scene, re-render them.
			CGameScene* curScene = g_pGameApp->GetCurScene();
			CCharacter* pCha = curScene->_pChaArray;
			for (int i = 0; i < curScene->_nChaCnt; i++) {
				if (pCha->IsValid() && !pCha->IsNPC() && pCha->IsEnabled()) {
					pCha->UpdataFace(pCha->GetPart());
				}
				pCha++;
			}

			//update player portrait
			pCha = g_stUIBoat.GetHuman();
			static stNetTeamChaPart stTeamPart;
			stTeamPart.Convert(pCha->GetPart());
			g_stUIStart.GetMainCha()->UpdataFace(stTeamPart);

			g_SystemIni["gameOption"].SetInt64("apparel", bAppMode ? 1 : 0);
			g_SystemIni.Save();
		}
	}

	pGroup = g_stUISystem.cbxEffMode;
	if (pGroup) {
		const bool bEffMode = pGroup->GetActiveIndex() == 1 ? true : false;
		if (bEffMode != g_stUISystem.m_sysProp.m_gameOption.bEffMode) {
			g_stUISystem.m_sysProp.m_gameOption.bEffMode = bEffMode;
			CCharacter::SetIsShowEffects(bEffMode);
			g_stUIStart.RefreshPet();
			//for each player in scene, re-render them.
			CGameScene* curScene = g_pGameApp->GetCurScene();
			CCharacter* pCha = curScene->_pChaArray;
			for (int i = 0; i < curScene->_nChaCnt; i++) {
				if (pCha->IsValid() && !pCha->IsNPC() && pCha->IsEnabled()) {
					pCha->UpdataFace(pCha->GetPart());
					pCha->RefreshSelfEffects();
				}
				pCha++;
			}

			g_SystemIni["gameOption"].SetInt64("effect", bEffMode ? 1 : 0);
			g_SystemIni.Save();
		}
	}

	pGroup = g_stUISystem.cbxStateMode;
	if (pGroup) {
		const bool bStateMode = pGroup->GetActiveIndex() == 1 ? true : false;
		if (bStateMode != g_stUISystem.m_sysProp.m_gameOption.bStateMode) {
			g_stUISystem.m_sysProp.m_gameOption.bStateMode = bStateMode;
			g_IsShowStates = bStateMode;
			g_SystemIni["gameOption"].SetInt64("state", bStateMode ? 1 : 0);
			g_SystemIni.Save();
		}
	}

	pGroup = g_stUISystem.cbxEnemyNames; // Add by mdr.st May 2020 - FPO alpha
	if (pGroup) {
		const bool bEnemyNames = pGroup->GetActiveIndex() == 1 ? true : false;
		if (bEnemyNames != g_stUISystem.m_sysProp.m_gameOption.bEnemyNames) {
			g_stUISystem.m_sysProp.m_gameOption.bEnemyNames = bEnemyNames;
			CHeadSay::SetIsShowEnemyNames(bEnemyNames); //Put this in an if statement inside UIHeadSay.cpp
			//g_stUIStart.RefreshPet();
			//CHeadSay::Render();
		}
	}
	pGroup = g_stUISystem.cbxShowBars; // Add by mdr.st May 2020 - FPO alpha
	if (pGroup) {
		const bool bShowBars = pGroup->GetActiveIndex() == 1 ? true : false;
		if (bShowBars != g_stUISystem.m_sysProp.m_gameOption.bShowBars) {
			g_stUISystem.m_sysProp.m_gameOption.bShowBars = bShowBars;
			CHeadSay::SetIsShowBars(bShowBars); //Put this in an if statement inside UIHeadSay.cpp
			//g_stUIStart.RefreshPet();
			//CHeadSay::Render();
		}
	}

	pGroup = g_stUISystem.cbxShowPercentages; // Add by mdr.st May 2020 - FPO alpha
	if (pGroup) {
		const bool bShowPercentages = pGroup->GetActiveIndex() == 1 ? true : false;
		if (bShowPercentages != g_stUISystem.m_sysProp.m_gameOption.bShowPercentages) {
			g_stUISystem.m_sysProp.m_gameOption.bShowPercentages = bShowPercentages;
			CHeadSay::SetIsShowPercentages(bShowPercentages); //Put this in an if statement inside UIHeadSay.cpp
			//g_stUIStart.RefreshPet();
			//CHeadSay::Render();
		}
	}

	pGroup = g_stUISystem.cbxShowInfo; // Add by mdr.st May 2020 - FPO alpha
	if (pGroup) {
		const bool bShowInfo = pGroup->GetActiveIndex() == 1 ? true : false;
		if (bShowInfo != g_stUISystem.m_sysProp.m_gameOption.bShowInfo) {
			g_stUISystem.m_sysProp.m_gameOption.bShowInfo = bShowInfo;
			CHeadSay::SetIsShowInfo(bShowInfo); //Put this in an if statement inside UIHeadSay.cpp
			//g_stUIStart.RefreshPet();
			//CHeadSay::Render();
		}
	}
	pGroup = g_stUISystem.cbxFramerate;
	if (pGroup) {
		//  UI чекбокс — переключатель между 30 и 60. Произвольный FPS (90/120/144)
		//  можно выставить только через ini, чекбокс этого не отображает.
		const int nFramerate = pGroup->GetActiveIndex() == 1 ? 60 : 30;
		if (nFramerate != g_stUISystem.m_sysProp.m_gameOption.nFramerate) {
			g_stUISystem.m_sysProp.m_gameOption.nFramerate = nFramerate;
			Corsairs::Client::Frame::SteadyFrameSync::Instance().SetFps(static_cast<std::uint32_t>(nFramerate));
			g_pGameApp->MsgBox("Please switch character to update framerate");
		}
	}

	pGroup = g_stUISystem.cbxShowMounts;
	if (pGroup) {
		const bool showMounts = pGroup->GetActiveIndex() == 1 ? true : false;
		if (showMounts != g_stUISystem.m_sysProp.m_gameOption.bShowMounts) {
			g_stUISystem.m_sysProp.m_gameOption.bShowMounts = showMounts;
			showMounts ? RespawnAllPlayerMounts() : DespawnAllPlayerMounts();
		}
	}
}

void CSystemMgr::_evtGameOptionFormBeforeShow(CForm* pForm, bool& IsShow) {
	CCheckGroup* pGroup = g_stUISystem.cbxRunMode;
	if (pGroup)
		pGroup->SetActiveIndex(g_stUISystem.m_sysProp.m_gameOption.bRunMode ? 1 : 0);

	pGroup = g_stUISystem.cbxLockMode;
	if (pGroup)
		pGroup->SetActiveIndex(g_stUISystem.m_sysProp.m_gameOption.bLockMode ? 1 : 0);

	pGroup = g_stUISystem.cbxHelpMode;
	if (pGroup)
		pGroup->SetActiveIndex(g_stUISystem.m_sysProp.m_gameOption.bHelpMode ? 1 : 0);

	pGroup = g_stUISystem.cbxCameraMode;
	if (pGroup)
		pGroup->SetActiveIndex(g_stUISystem.m_sysProp.m_gameOption.bCameraMode ? 1 : 0);

	pGroup = g_stUISystem.cbxAppMode;
	if (pGroup)
		pGroup->SetActiveIndex(g_stUISystem.m_sysProp.m_gameOption.bAppMode ? 1 : 0);

	pGroup = g_stUISystem.cbxEffMode;
	if (pGroup)
		pGroup->SetActiveIndex(g_stUISystem.m_sysProp.m_gameOption.bEffMode ? 1 : 0);
	pGroup = g_stUISystem.cbxStateMode;
	if (pGroup)
		pGroup->SetActiveIndex(g_stUISystem.m_sysProp.m_gameOption.bStateMode ? 1 : 0);
	//Add by Mdr.st May 2020 - FPO alpha
	pGroup = g_stUISystem.cbxEnemyNames;
	if (pGroup)
		pGroup->SetActiveIndex(g_stUISystem.m_sysProp.m_gameOption.bEnemyNames ? 1 : 0);

	pGroup = g_stUISystem.cbxShowBars;
	if (pGroup)
		pGroup->SetActiveIndex(g_stUISystem.m_sysProp.m_gameOption.bShowBars ? 1 : 0);

	pGroup = g_stUISystem.cbxShowPercentages;
	if (pGroup)
		pGroup->SetActiveIndex(g_stUISystem.m_sysProp.m_gameOption.bShowPercentages ? 1 : 0);
	pGroup = g_stUISystem.cbxShowInfo;
	if (pGroup)
		pGroup->SetActiveIndex(g_stUISystem.m_sysProp.m_gameOption.bShowInfo ? 1 : 0);
	pGroup = g_stUISystem.cbxFramerate;
	if (pGroup)
		//  Чекбокс показывает «60 FPS включён» если nFramerate >= 60. Произвольное
		//  значение из ini (например 144) тоже отображается как «вкл».
		pGroup->SetActiveIndex(g_stUISystem.m_sysProp.m_gameOption.nFramerate >= 60 ? 1 : 0);

	pGroup = g_stUISystem.cbxShowMounts;
	if (pGroup)
		pGroup->SetActiveIndex(g_stUISystem.m_sysProp.m_gameOption.bShowMounts ? 1 : 0);
}


void CSystemMgr::CloseForm() {
	g_ChaExitOnTime.Cancel();
}

void CSystemMgr::FrameMove(DWORD dwTime) {
	g_ChaExitOnTime.FrameMove(dwTime);
}

//---------------------------------------------------------------------------
// class CChaExitOnTime
//---------------------------------------------------------------------------
namespace GUI {
	CChaExitOnTime g_ChaExitOnTime;
};

CChaExitOnTime::CChaExitOnTime()
	: _eOptionType(enumInit), _dwStartTime(0), _dwEndTime(0), _IsEnabled(false) {
}

bool CChaExitOnTime::_IsTime() {
	if (_eOptionType != enumInit) {
		if (CGameApp::GetCurTick() > _dwStartTime + 60 * 1000 * 5) {
			_eOptionType = enumInit;
			return false;
		}

		g_pGameApp->SysInfo(GetLanguageString(774));
		return true;
	}

	return false;
}

void CChaExitOnTime::ChangeCha() {
	if (_IsTime()) return;

	if (g_pGameApp->GetCurScene()->GetMainCha()->IsShop()) {
		g_pGameApp->MsgBox("Please close your stall before switching characters.");
		return;
	}

	_eOptionType = enumChangeCha;
	_dwStartTime = CGameApp::GetCurTick();

	_dwEndTime = 0;

	g_stUIMap.CloseRadar(); //   add by Philip.Wu  2006-06-21

	g_pGameApp->ClearAllSkillClocks();
	CS_EndPlay();

#ifdef USE_DSOUND
	if (g_dwCurMusicID) {
		Corsairs::Client::Audio::AudioSDL::Instance().Stop(g_dwCurMusicID);
		g_dwCurMusicID = 0;
		Sleep(60);
	}
#endif

	if (!_IsEnabled) {
		TimeArrived();
	}
}

void CChaExitOnTime::ExitApp() {
	if (_IsTime()) return;

	_eOptionType = enumExitApp;
	_dwStartTime = CGameApp::GetCurTick();

	_dwEndTime = 0;
	CS_Logout();

	if (!_IsEnabled) {
		TimeArrived();
	}
}

void CChaExitOnTime::OfflineMode() {
	if (_IsTime()) return;

	_eOptionType = enumOfflineMode;
	_dwStartTime = CGameApp::GetCurTick();

	_dwEndTime = 0;
	CS_OfflineMode();

	if (!_IsEnabled) {
		TimeArrived();
	}
}

void CChaExitOnTime::Relogin() {
	if (_IsTime()) return;

	_eOptionType = enumRelogin;
	_dwStartTime = CGameApp::GetCurTick();

	_dwEndTime = 0;

	g_stUIMap.CloseRadar(); //   add by Philip.Wu  2006-06-21
	g_pGameApp->ClearAllSkillClocks();
	CS_Logout();

#ifdef USE_DSOUND
	if (g_dwCurMusicID) {
		Corsairs::Client::Audio::AudioSDL::Instance().Stop(g_dwCurMusicID);
		g_dwCurMusicID = 0;
		Sleep(60);
	}
#endif

	if (!_IsEnabled) {
		TimeArrived();
	}
}

void CChaExitOnTime::Cancel() {
	if (!_IsEnabled) return;

	if (_eOptionType == enumInit) return;

	extern void CS_CancelExit();
	CS_CancelExit();

	_eOptionType = enumInit;
}

void CChaExitOnTime::FrameMove(DWORD dwTime) {
	if (!_IsEnabled) return;

	if (_eOptionType == enumInit) return;

	if (_dwEndTime == 0) return;

	if (dwTime < _dwEndTime) {
		static CTimeWork time(1000);
		if (time.IsTimeOut(dwTime)) {
			g_pGameApp->ShowBigText(SafeVFormat(GetLanguageString(775), (_dwEndTime - dwTime) / 1000));
			return;
		}
	}
}

bool CChaExitOnTime::TimeArrived() {
	switch (_eOptionType) {
	case enumChangeCha: {
		g_pGameApp->LoadScriptScene(enumLoginScene);
		g_pGameApp->SetLoginTime(0);

		CLoginScene* pScene = dynamic_cast<CLoginScene*>(g_pGameApp->GetCurScene());
		if (pScene) {
			if (g_NetIF->IsConnected())
				pScene->ShowChaList();
			else
				pScene->ShowRegionList();
			//pScene->ShowLoginForm();
		}
	}
	break;
	case enumExitApp: {
		CS_Disconnect(DS_DISCONN);
		g_pGameApp->SetLoginTime(0);

		g_pGameApp->SetIsRun(false);
	}
	break;
	case enumRelogin: {
		CS_Disconnect(DS_DISCONN);
		g_pGameApp->SetLoginTime(0);

		g_pGameApp->LoadScriptScene(enumLoginScene);
		CLoginScene* pScene = dynamic_cast<CLoginScene*>(g_pGameApp->GetCurScene());
		if (pScene) {
			pScene->ShowRegionList();
			//pScene->ShowLoginForm();
		}
	}
	break;
	};

	if (_eOptionType != enumInit) {
		_eOptionType = enumInit;
		return true;
	}
	return false;
}

void CChaExitOnTime::NetStartExit(DWORD dwExitTime) {
	_dwEndTime = CGameApp::GetCurTick() + dwExitTime;

	g_pGameApp->SysInfo(SafeVFormat(GetLanguageString(776), dwExitTime / 1000));
}

void CChaExitOnTime::NetCancelExit() {
	_eOptionType = enumInit;

	g_pGameApp->SysInfo(GetLanguageString(777));
}

void CChaExitOnTime::Reset() {
	_eOptionType = enumInit;
}
