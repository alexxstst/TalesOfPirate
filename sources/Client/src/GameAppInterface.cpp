#include "Stdafx.h"

#include "GameApp.h"
#include "GameConfig.h"

#include "SceneObjRecordStore.h"
#include "EffectRecordStore.h"
#include "MusicRecordStore.h"
#include "ItemRecordStore.h"
#include "ChaRecordStore.h"
#include "SkillRecordStore.h"
#include "SkillStateRecordStore.h"
#include "HairRecordStore.h"
#include "ForgeRecordStore.h"
#include "AreaRecordStore.h"
#include "ShipRecordStore.h"
#include "ShipPartRecordStore.h"
#include "SceneObjRecordStore.h"
#include "EffectRecordStore.h"
#include "ShadeRecordStore.h"
#include "lwTimer.h"
#include "EventSoundRecordStore.h"
#include "MusicRecordStore.h"
#include "PoseRecordStore.h"
#include "CharacterActionStore.h"
#include "CharacterModelStore.h"
#include "AnimatedLightStore.h"
#include "ChaCreateRecordStore.h"
#include "MapRecordStore.h"
#include "EventRecordStore.h"
#include "ServerRecordStore.h"
#include "NotifyRecordStore.h"
#include "ChatIconRecordStore.h"
#include "ItemTypeRecordStore.h"
#include "ItemPreRecordStore.h"
#include "TerrainRecordStore.h"
#include "EffParamRecordStore.h"
#include "GroupParamRecordStore.h"
#include "ItemRefineRecordStore.h"
#include "ItemRefineEffectRecordStore.h"
#include "MonsterInfoRecordStore.h"
#include "ResourceRecordStore.h"
#include "StoneRecordStore.h"
#include "ElfSkillRecordStore.h"
#include "HelpEntryStore.h"
#include "AssetDatabase.h"

#include "i_effect.h"

#include "Character.h"
#include "MPEditor.h"
#include "Scene.h"

#include "MPFont.h"
#include "FontManager.h"
#include "SmallMap.h"
#include "LoginScene.h"
#include "WorldScene.h"

#include "GlobalVar.h"
#include "PacketCmd.h"
#include "Track.h"
#include "STStateObj.h"
#include "Actor.h"
#include "UIGlobalVar.h"
#include "UIFormMgr.h"
#include "uilabel.h"
#include "uitextbutton.h"
#include "UIEditor.h"
#include "UITemplete.h"
#include "UIImage.h"
#include "uicozeform.h"
#include "uinpctalkform.h"
#include "uiboxform.h"
#include "uiprogressbar.h"

#include "SceneObjRecordStore.h"
#include "EffectRecordStore.h"
#include "Mapset.h"
#include "MusicRecordStore.h"
#include "CharacterRecord.h"
#include "ItemRecord.h"
#include "SkillRecord.h"
#include "AreaRecord.h"
#include "RenderStateMgr.h"
#include "SkillStateRecord.h"
#include "forgerecord.h"
#include "EventRecord.h"
#include "createchascene.h"
#include "SelectChaScene.h"
#include "SteadyFrame.h"
#include "uistartform.h"
#include "cameractrl.h"
#include "shipset.h"

#include "gameloading.h"

#include "StringLib.h"
#include "ItemPreSet.h"
#include "HairRecord.h"
#include "ItemRefineSet.h"
#include "ItemRefineEffectRecordStore.h"
#include "SceneItem.h"
#include "GameMovie.h"
// add by mdr
#include "NPCHelper.h"
#include "MountRecordStore.h"

using namespace std;

extern CGameMovie g_GameMovie;

#ifndef USE_DSOUND
#include "AudioThread.h"
extern CAudioThread g_AudioThread;
#endif

extern std::uint32_t g_dwCurMusicID;

using namespace Corsairs::Client::Audio;


static bool IsExistFile(const std::string& file) {
	HANDLE hFile = CreateFile(file.c_str(),GENERIC_READ, 0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if (INVALID_HANDLE_VALUE == hFile) {
		return false;
	}
	else {
		CloseHandle(hFile);
		return true;
	}
}

static void __timer_frame(DWORD time) {
	g_pGameApp->FrameMove(time);
}

static void __timer_render(DWORD time) {
	g_pGameApp->Render();
}

void CALLBACK __timer_period_proc(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2) {
	HWND hwnd = (HWND)dwUser;
	::SendMessage(hwnd, WM_USER_TIMER, 0, 0);
}

void __timer_period_render() {
	if (g_pGameApp->IsRun()) {
		DWORD Tick = timeGetTime();
		if ((Tick - g_pGameApp->GetCurTick()) > 18) {
			g_pGameApp->SetTickCount(Tick);
			g_pGameApp->FrameMove(Tick);
			g_pGameApp->Render();
		}
	}
}

int CGameApp::Run() {
#define R_FAIL - 1;
#define R_OK 0;

	_dwLoadingTick = g_pGameApp->GetCurTick();

	MSG msg;
	memset(&msg, 0, sizeof(msg));

#if(defined USE_TIMERPERIOD)

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
#else
	_dwCurTick = 0;

#if(defined USE_INDIVIDUAL_TIMER)
	MPITimer* timer = LW_NEW(lwTimer);
	timer->SetTimer(0, __timer_frame, 1.0 f / 30);
	timer->SetTimer(1, __timer_render, 1.0 f / 30);
#endif

	_isRun = true;

	if (!_pSteady->Init()) return -1;

	while (_isRun) {
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			continue;
		}

		if (_pCurScene) {
#if(defined USE_INDIVIDUAL_TIMER)
			timer->OnTimer();
#else

			g_NetIF->PollPackets(100); //

			if (_pSteady->Run()) {
				//LG("frame", "time:%u\n", GetTickCount() - _dwCurTick);

				g_Render.GetInterfaceMgr()->tp_loadres->SetPoolEvent(TRUE);
				_dwCurTick = _pSteady->GetTick();
				//_FrameMoveOnce( _dwCurTick );
				FrameMove(_dwCurTick);
				Render();
				if (_nSwitchScene > -140) {
					_ShowLoading((int)((_nSwitchScene + 140) * 100 / _total));
				}

				_pSteady->End();
				g_Render.GetInterfaceMgr()->tp_loadres->SetPoolEvent(FALSE);
			}
#endif
		}
	}

#if(defined USE_INDIVIDUAL_TIMER)
	timer->Release();
#endif
#endif
	return (int)msg.wParam;
}

void CGameApp::AutoTestUpdate() {
	if (!_pCurScene) return;
	if (!_isRun) exit(-1);

	MSG msg;
	memset(&msg, 0, sizeof(msg));
	int i = 0;
	for (;;) {
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			continue;
		}

		g_Render.GetInterfaceMgr()->tp_loadres->SetPoolEvent(TRUE);
		_dwCurTick = ::GetTickCount();
		FrameMove(_dwCurTick);
		Render();
		g_Render.GetInterfaceMgr()->tp_loadres->SetPoolEvent(FALSE);

		if (i > 1) return;
		i++;
	}
}

void CGameApp::GotoScene(CGameScene* scene, bool isDelCurScene, bool IsShowLoading) {
	if (!scene) {
		MsgBox("%s", GetLanguageString(71).c_str());
		return;
	}

	if (_pCurScene && !_pCurScene->_Clear()) {
		_SceneError(GetLanguageString(72).c_str(), _pCurScene);
		SetIsRun(false);
		return;
	}

	if (isDelCurScene && _pCurScene && _pCurScene != scene) {
		delete _pCurScene;
	}
	_pCurScene = scene;

	CFormMgr::s_Mgr.SetEnabled(true);
	CFormMgr::s_Mgr.ResetAllForm();
	CFormMgr::s_Mgr.SwitchTemplete(_pCurScene->GetInitParam()->nUITemplete);

	if (!_pCurScene->_Init()) {
		_SceneError(GetLanguageString(73).c_str(), _pCurScene);
		SetIsRun(false);
		return;
	}

	if (IsShowLoading) {
		_dwLoadingTick = g_pGameApp->GetCurTick();

		static bool isFirstWorld = false;
		if (!isFirstWorld && dynamic_cast<CWorldScene*>(scene)) {
			// ,
			isFirstWorld = true;
			Loading(100);
		}
		else {
			Loading();
		}
	}
}

void CGameApp::ResetGameCamera(int type) {
	CCameraCtrl* pCam = GetMainCam();
	pCam->SetModel(type);
	//if(type==0)
	//{
	//	pCam->m_stDist = g_Config.m_fminxy;
	//	pCam->m_enDist = g_Config.m_fmaxxy;
	//	pCam->m_stHei = g_Config.m_fminHei;
	//	pCam->m_enHei = g_Config.m_fmaxHei;
	//	pCam->m_stFov = g_Config.m_fminfov;
	//	pCam->m_enFov = g_Config.m_fmaxfov;

	//	pCam->m_fxy = g_Config.m_fmaxxy;
	//	pCam->m_fz  = g_Config.m_fmaxHei;
	//	pCam->m_ffov = g_Config.m_fmaxfov;
	//}else
	//{
	//	pCam->m_stDist = g_Config.m_fminxy2;
	//	pCam->m_enDist = g_Config.m_fmaxxy2;
	//	pCam->m_stHei = g_Config.m_fminHei2;
	//	pCam->m_enHei = g_Config.m_fmaxHei2;
	//	pCam->m_stFov = g_Config.m_fminfov2;
	//	pCam->m_enFov = g_Config.m_fmaxfov2;

	//	pCam->m_fxy = g_Config.m_fmaxxy2;
	//	pCam->m_fz  = g_Config.m_fmaxHei2;
	//	pCam->m_ffov = g_Config.m_fmaxfov2;

	//}
	//pCam->m_fstep1 = 1.0f;

	//pCam->m_fResetRotatVel = g_Config.m_fCameraVel;
	//pCam->m_fResetRotatAccl = g_Config.m_fCameraAccl;

	//////pCam->SetFollowObj(_pCurScene->GetMainCha()->GetPos());
	//GetMainCam()->InitAngle(0.5235987f + 0.0872664f);
	////GetMainCam()->ResetCamera(0.5235987f + 0.0872664f);
	//g_Render.SetWorldViewFOV(Angle2Radian( GetMainCam()->m_ffov));
}

void CGameApp::ResetCamera() {
	CCameraCtrl* pCam = g_pGameApp->GetMainCam();
	GetMainCam()->ResetCamera(0.5235987f + 0.0872664f);
	//g_Render.SetWorldViewFOV(Angle2Radian( GetMainCam()->m_cameractrl.m_ffov));
}

void CGameApp::CreateCharImg() {
	static int id = 1;
	static float fTime = *ResMgr.GetDailTime();

	CCharacter* pMainCha = _pCurScene->GetMainCha();

	fTime += *ResMgr.GetDailTime();
	bool be = false;
	CCharacter* pCha = NULL;
	CChaRecord* pInfo = GetChaRecordInfo(id);

	D3DXVECTOR3 veye(pMainCha->GetPos());
	D3DXVECTOR3 vlookat = veye;

	veye.y += 10;
	veye.z += 3.5f;

	vlookat.z += 1.5f;


	if (fTime > 1.0f) {
		pCha = _pCurScene->AddCharacter(id);
		if (pCha) {
			pCha->PlayPose(1, PLAY_PAUSE);
			pCha->setYaw(180);

			pCha->setPos(pMainCha->GetCurX(), pMainCha->GetCurY());

			//if(pInfo->chTerritory == 1)
			//	pCha->SetHieght(1.6f);
			pCha->FrameMove(0);


			veye = pCha->GetPos();
			vlookat = veye;

			veye.y += 12;
			veye.z += 3.5f;

			vlookat.z += 1.5f;
		}
		else {
			id++;
			if (id > 670) {
				btest = false;
				pMainCha->SetHide(FALSE);
			}
			fTime = 0;
			return;
		}

		be = true;
		fTime = 0;
	}
	pMainCha->SetHide(TRUE);
	g_Render.LookAt(veye, vlookat);
	g_Render.SetCurrentView(MPRender::VIEW_WORLD);

	_pCurScene->_Render();

	if (be) {
		char szPath[255];
		char fileName[255];
		sprintf(szPath, "screenshot/cha");
		Util_MakeDir(szPath);

		sprintf(fileName, "%s/%s.bmp", szPath, pInfo->DataName.c_str());

		g_Render.CaptureScreen(fileName);

		if (pCha)
			pCha->SetValid(FALSE);
		id++;
	}
	if (id > 670) {
		btest = false;
		pMainCha->SetHide(FALSE);
	}
}

////////////////////////
BOOL CGameApp::_CreateSmMap(MPTerrain* pTerr) {
	static bool isnext = true;

	static float fTime = *ResMgr.GetDailTime();

	if (!_pCurScene->GetMainCha()) return FALSE;

	fTime += *ResMgr.GetDailTime();
	if (fTime > 0.5f) {
		if (isnext) {
			pTerr->SetShowSize(SHOWRSIZE + 5,SHOWRSIZE + 5);
			_pCurScene->GetMainCha()->setPos((int)xp * 100, (int)yp * 100);
			SetCameraPos(_pCurScene->GetMainCha()->GetPos());
			isnext = false;
			fTime = 0;
			return TRUE;
		}
		bool bSea = true;
		fTime = 0;


		if (!isnext) {
			MPTile* TileList;
			int se;
			int e = (int)xp - SHOWRSIZE / 2;
			se = e;
			int w = (int)yp - SHOWRSIZE / 2;
			for (; w < int(yp + SHOWRSIZE / 2); w++) {
				for (; e < int(xp + SHOWRSIZE / 2); e++) {
					TileList = pTerr->GetTile(e, w);
					if (!TileList->IsDefault()) {
						bSea = false;
						goto skip;
					}
				}
				e = se;
			}
		skip:

			if (int(xp / SHOWRSIZE) >= (destxp / SHOWRSIZE)) {
				xp = xp1;
				yp += SHOWRSIZE;
				if (int(yp / SHOWRSIZE) >= destyp / SHOWRSIZE) {
					MessageBox(NULL, GetLanguageString(74).c_str(), "INFO", 0);
					xp = xp1;
					yp = yp1;

					_pCurScene->GetMainCha()->setPos((int)xp * 100, (int)yp * 100);
					SetCameraPos(_pCurScene->GetMainCha()->GetPos());

					EnableSprintSmMap(FALSE);
				}
				isnext = true;
				return TRUE;
			}
			else {
				xp += SHOWRSIZE;
				isnext = true;
			}
			fTime = 0;
		}
		if (bSea)
			return TRUE; //

		D3DXMATRIX matView, matProj;
		D3DXVECTOR3 vEyePt = _pCurScene->GetMainCha()->GetPos();
		vEyePt.z = SHOWRSIZE;
		D3DXVECTOR3 vLookatPt = vEyePt;
		vLookatPt.z = 0;
		D3DXVECTOR3 vUpVec = D3DXVECTOR3(0, -1, 0);
		D3DXMatrixOrthoLH(&matProj, SHOWRSIZE,
						  SHOWRSIZE, 0.0f, 1000.0f);
		g_Render.SetTransformProj(&matProj);
		char fileName[64];

		D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);
		g_Render.SetTransformView(&matView);

		pTerr->SetShowCenter(vEyePt.x, vEyePt.y);
		pTerr->SetShowSize(SHOWRSIZE + 5,SHOWRSIZE + 5);
		_pCurScene->RenderSMallMap();

		char szPath[255];
		sprintf(szPath, "texture/minimap/%s", _pCurScene->GetTerrainName());
		Util_MakeDir(szPath);
		sprintf(fileName, "%s/sm_%d_%d.bmp", szPath,
				int(vEyePt.x / SHOWRSIZE), int(vEyePt.y / SHOWRSIZE));

		g_Render.CaptureScreen(fileName);

		IDirect3DTextureX* pTex = NULL;
		D3DXCreateTextureFromFileEx(g_Render.GetDevice(),
									fileName, //
									256, //
									256, //
									1, //mipmap1
									0, //
									D3DFMT_UNKNOWN, //?
									D3DPOOL_MANAGED, //DXGraphics
									D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR | D3DX_FILTER_BOX, //
									D3DX_FILTER_NONE, //mipmap
									0x00000000, //
									NULL, //?
									NULL, //?
									&pTex); //
		D3DXSaveTextureToFile(fileName, D3DXIFF_BMP, pTex,NULL);
		SAFE_RELEASE(pTex);
	}
	return TRUE;
}

BOOL CGameApp::_PrintScreen() {
	if (IsEnableSpAvi()) {
		_CreateAviScreen();
	}
	else {
		static int g_nScreenCap = 0;
		char fileName[64];
		Util_MakeDir("screenshot\\");

		char pszName[64];

		int nidx = 0;
		while (1) {
			sprintf(pszName, "screenshot\\%d\\", nidx);
			if (_access(pszName, 0) == -1) {
				Util_MakeDir(pszName);
				break;
			}
		}
		sprintf(fileName, "%scap%05d.bmp", pszName, g_nScreenCap);
		g_Render.CaptureScreen(fileName);

		auto _strTip = SafeVFormat(GetLanguageString(75), fileName);
		Tip(_strTip.c_str());
		g_nScreenCap++;
		EnableSprintScreen(FALSE);
	}
	return TRUE;
}

BOOL CGameApp::_CreateAviScreen() {
	/*static int g_nAviCnt = 0;
	char fileName[64];
    Util_MakeDir("screenshot/");

	sprintf(fileName,"screenshot/cap%05d.bmp",g_nAviCnt);
	g_Render.CaptureScreen(fileName);
	g_nAviCnt++;*/
	return TRUE;
}

CGameScene* CGameApp::CreateScene(stSceneInitParam* param) {
	if (!param) return NULL;

	if (param->nTypeID >= enumSceneEnd) return NULL;

	CGameScene* scene = NULL;
	switch (param->nTypeID) {
	case enumLoginScene: scene = new CLoginScene(*param);
		break;
	case enumSelectChaScene: scene = new CSelectChaScene(*param);
		break;
	case enumCreateChaScene: scene = new CCreateChaScene(*param);
		break;
	case enumWorldScene: {
		scene = new CWorldScene(*param);
		break;
	}
	}

	if (scene && scene->_CreateMemory())
		return scene;

	return NULL;
}

BOOL CGameApp::CreateCurrentScene(char* szMapName) {
	stSceneInitParam stInit;
	stInit.nTypeID = enumLoginScene;
	stInit.strName = GetLanguageString(76);
	stInit.strMapFile = szMapName;
	stInit.nMaxEff = 300;
	stInit.nMaxCha = 300;
	stInit.nMaxObj = 400;
	stInit.nMaxItem = 400;
	CGameScene* s = g_pGameApp->CreateScene(&stInit);
	if (!s)
		return FALSE;
	_pCurScene = s;
	return TRUE;
}

void CGameApp::_RenderTipText() {
	if (!_TipText.empty()) {
		//()
		int nTextY = 220;
		SIZE sz;

		for (auto const& tipText : _TipText) {
			if (GetCurTick() - tipText->dwBeginTime > 3000) {
				if (tipText->btAlpha > 10) {
					tipText->btAlpha -= 10;
				}
				else if (tipText->btAlpha < 10) {
					tipText->btAlpha = 0;
				}
			}
			DWORD const dwAlpha = tipText->btAlpha << 24;
			DWORD const dwColor = dwAlpha | 0x00f01000;
			CMPFont* pTipFont = FontManager::Instance().Get(FontSlot::TipText);
			pTipFont->GetTextSize(tipText->szText, &sz);
			pTipFont->DrawText(tipText->szText, (GetWindowWidth() - sz.cx) / 2, nTextY, dwColor);
			nTextY += 24;
		}

		if (_TipText.size() > 0) {
			if (auto& tipText = _TipText.front();
				tipText && tipText->btAlpha <= 0) {
				_TipText.pop_front();
			}
		}
	}
}

void CGameApp::PlayMusic(int nMusicNo) {
	if (!CGameApp::IsMusicSystemValid()) return;

	//SetBkMusicNotify(_hWnd, WM_MUSICEND);
	CMusicInfo* pInfo = NULL;
	//
	//if(nMusicNo==0) //
	//{
	//    static g_dwPlayMusicNo = 0;
	//    g_dwPlayMusicNo++;
	//    CMusicInfo *pInfo = GetMusicInfo(g_dwPlayMusicNo);
	//    if( !pInfo || strlen(pInfo->DataName.c_str())==0 )
	//    {
	//        g_dwPlayMusicNo = 1; //
	//    }
	//    nMusicNo = g_dwPlayMusicNo;
	//}
	if (nMusicNo >= 1) //
	{
		pInfo = GetMusicInfo(nMusicNo);
		if (pInfo && strlen(pInfo->DataName.c_str()) == 0) {
			pInfo = NULL;
		}
	}

	if (!pInfo) {
		_szBkgMusic[0] = '\0';
		_eSwitchMusic = enumOldMusic;
		return;
	}

	switch (_eSwitchMusic) {
	case enumNoMusic: {
		strncpy(_szBkgMusic, pInfo->DataName.c_str(), sizeof(_szBkgMusic));

		_eSwitchMusic = enumNewMusic;
		std::uint32_t OldMusicID = g_dwCurMusicID;
		g_dwCurMusicID = AudioSDL::Instance().GetResourceId(_szBkgMusic, AudioType::Stream);
		AudioSDL::Instance().SetVolume(g_dwCurMusicID, float(_nCurMusicSize) / 128.0f);
#ifndef USE_DSOUND
		g_AudioThread.play(g_dwCurMusicID, true);
#else
		if (g_dwCurMusicID && (OldMusicID != g_dwCurMusicID)) {
			AudioSDL::Instance().Stop(OldMusicID);
			AudioSDL::Instance().Play(g_dwCurMusicID, true);
		}
#endif
	}
	break;
	case enumOldMusic: {
		strncpy(_szBkgMusic, pInfo->DataName.c_str(), sizeof(_szBkgMusic));
	}
	break;
	case enumNewMusic:
	case enumMusicPlay:
		if (strncmp(_szBkgMusic, pInfo->DataName.c_str(), sizeof(_szBkgMusic)) != 0) {
			strncpy(_szBkgMusic, pInfo->DataName.c_str(), sizeof(_szBkgMusic));
			_eSwitchMusic = enumOldMusic;
		}
		break;
	}
}

void CGameApp::PlaySound(int nSoundNo) {
	if (!CGameApp::IsMusicSystemValid()) return;

	if (nSoundNo == -1) return;

	CMusicInfo* pInfo = GetMusicInfo(nSoundNo);
	if (!pInfo || pInfo->nType != 1) return;

#ifdef USE_DSOUND
	PlaySample(pInfo->DataName.c_str());
#else
	std::uint32_t musid = AudioSDL::Instance().GetResourceId(pInfo->DataName, AudioType::Sfx);
	AudioSDL::Instance().SetVolume(musid, CGameScene::_fSoundSize / 128.0f);
	g_AudioThread.play(musid, false);
#endif
}

void CGameApp::SetMusicSize(float fVol) {
	if (!CGameApp::IsMusicSystemValid()) return;

	if (fVol >= 0.0 && fVol <= 1.0) {
		_nMusicSize = (int)(fVol * 128.0f);

		//::bkg_snd_vol( _nMusicSize );
		AudioSDL::Instance().SetVolume(g_dwCurMusicID, float(_nMusicSize) / 128.0f);
	}
}

static bool WaitModalForm(CGuiData* pSender, char& key) {
	if (key == VK_RETURN) {
		CForm* frmWaiting = dynamic_cast<CForm*>(pSender);
		if (frmWaiting) {
			frmWaiting->Close();
			return true;
		}
	}
	return false;
}

void CGameApp::MsgBox(const char* pszFormat, ...) {
	va_list list;
	va_start(list, pszFormat);
	vsprintf(_szOutBuf, pszFormat, list);
	va_end(list);

	g_stUIBox.ShowMsgBox(NULL, _szOutBuf);
}

void CGameApp::ShowBigText(const char* pszFormat, ...) {
	va_list list;
	va_start(list, pszFormat);
	vsprintf(_szOutBuf, pszFormat, list);
	va_end(list);

	g_stUIStart.ShowBigText(_szOutBuf);
}

void CGameApp::ShowMidText(const char* pszFormat, ...) {
	va_list list;
	va_start(list, pszFormat);
	vsprintf(_szOutBuf, pszFormat, list);
	va_end(list);

	strncpy(_stMidFont.szText, _szOutBuf, sizeof(_stMidFont.szText));
	_stMidFont.dwBeginTime = GetCurTick() + SHOW_TEXT_TIME;
	_stMidFont.btAlpha = 255;

	//g_stUICoze.OnSystemSay( _szOutBuf );
	CCozeForm::GetInstance()->OnSystemMsg(_szOutBuf);
}

void CGameApp::ShowBottomText(unsigned int rgb, const char* pszFormat, ...) {
	va_list list;
	va_start(list, pszFormat);
	vsprintf(_szOutBuf, pszFormat, list);
	va_end(list);

	STipText* tmpSize = new STipText;

	strncpy(tmpSize->szText, _szOutBuf, sizeof(tmpSize->szText));
	tmpSize->dwBeginTime = GetCurTick() + TIME_CQUEUE;
	tmpSize->btAlpha = 255;

	if (_qCQueueStrColour.size() >= MAX_CQUEUE) {
		pair<STipText*, unsigned int> curpair;
		curpair = _qCQueueStrColour.front();
		_qCQueueStrColour.pop();
	}
	_qCQueueStrColour.push(make_pair(tmpSize, rgb));
	_IsRenderColourTest = true;
	//CCozeForm::GetInstance()->OnSystemMsg(_szOutBuf);
}

void CGameApp::RenderColourQueue() {
	if (_qCQueueStrColour.empty()) {
		return;
	}
	// peek the first one
	pair<STipText*, unsigned int> curpair;
	curpair = _qCQueueStrColour.back();

	// check if time is up
	STipText* tmp = curpair.first;
	if (GetCurTick() > tmp->dwBeginTime + TIME_CQUEUE) {
		// remove it
		//MessageBoxA(NULL, "done", "title", 0);
		_IsRenderColourTest = false;
		_qCQueueStrColour.pop();
		delete tmp;
	}
	else {
		// render
		queue<pair<STipText*, unsigned int>> tmpQueue = _qCQueueStrColour;
		int i;
		for (i = 0; i < MAX_CQUEUE; i++) {
			if (tmpQueue.empty()) {
				break;
			}

			curpair = tmpQueue.front();
			STipText* tmp = curpair.first;
			if (GetCurTick() < tmp->dwBeginTime + TIME_CQUEUE) {
				//tmp->btAlpha = (BYTE)(255.0f * (float)(tmp->dwBeginTime - GetCurTick()) / (float)TIME_CQUEUE);

				DWORD dwAlpha = tmp->btAlpha << 24;
				DWORD dwColor = dwAlpha | curpair.second;

				SIZE size;
				CMPFont* pBottomFont = FontManager::Instance().Get(FontSlot::BottomShadow);
				pBottomFont->GetTextSize(tmp->szText, &size);

				int x = (GetWindowWidth() - size.cx) / 2;
				int y = (floor(GetWindowHeight() * 2.025) - size.cy) / 3 + 18 * i;
				pBottomFont->DrawTextShadow(tmp->szText, x + 1, y + 1, x, y, D3DXCOLOR(0, 0, 0, 1), (DWORD)dwColor);
				//0xff808000
			}
			else {
				tmpQueue.pop();
				_qCQueueStrColour = tmpQueue;
			}
			tmpQueue.pop();
			//_IsRenderColourTest = false;
		}
		//_IsRenderColourTest = false;
	}
}

void CGameApp::AddTipText(const char* pszFormat, ...) {
	va_list list;
	va_start(list, pszFormat);
	vsprintf(_szOutBuf, pszFormat, list);
	va_end(list);

	auto& textTip = _TipText.emplace_back(std::make_unique<STipText>());
	strncpy(textTip->szText, _szOutBuf, TIP_TEXT_NUM);
	textTip->dwBeginTime = GetTickCount();
	textTip->btAlpha = 255;
}

void CGameApp::ShowStateHint(int x, int y, CChaStateMgr::stChaState stateData) {
	char szTitle[32];
	sprintf(szTitle, "Lv%d %s", stateData.chStateLv, stateData.pInfo->szName.c_str());
	_pNotify->SetFixWidth(0);
	_dwNotifyTime = GetCurTick() + 100;
	_pNotify->Clear();
	_pNotify->PushHint(szTitle, COLOR_WHITE);
	//char szDescHint[255];
	//_pNotify->PushHint(szDescHint, COLOR_WHITE);
	if (stateData.lTimeRemaining > 0) {
		int minutes = stateData.lTimeRemaining / 60;
		int seconds = stateData.lTimeRemaining % 60;
		char szTime[32];
		sprintf(szTime, "Time Remaining: %d:%02d", minutes, seconds);
		_pNotify->PushHint(szTime, COLOR_WHITE);
	}
	_pNotify->ReadyForHint(x, y);
}

void CGameApp::ShowHint(int x, int y, const char* szStr, DWORD dwColor) {
	_pNotify->SetFixWidth(0);

	_dwNotifyTime = GetCurTick() + 100;
	_pNotify->Clear();
	_pNotify->PushHint(szStr, dwColor);
	_pNotify->ReadyForHint(x, y);


	//disabled by billy 20/06/2018
	//Add by sunny.sun20080804
	//_pNotify1->SetFixWidth( 0 );
	//_dwNotifyTime1 = GetCurTick() + 100;
	//	_pNotify1->Clear();
	//_pNotify1->PushHint( szStr, dwColor );
	//	SetNum = 1;
	//_pNotify1->ReadyForHint( x, y,SetNum);
	//End
}

//gmnotice
void CGameApp::ShowNotify(const char* szStr, DWORD dwColor) {
	//gmnotice fix
	_pNotify->SetFixWidth(430);

	_dwNotifyTime = GetCurTick() + 120000;

	_pNotify->Clear();
	//Add by sunny.sun 20080820
	//Begin
	int textlength = 0;
	if (GetRender().GetScreenWidth() == 1198) {
		textlength = 80;
	}
	else if (GetRender().GetScreenWidth() == 936) {
		textlength = 63;
	}
	else
		textlength = 140;
	//End
	static bool titleFlag;
	titleFlag = false; //
	std::string text = szStr;
	while (!text.empty()) {
		int n = 1;
		std::string strSub = text.substr(0, 1);
		for (; n < textlength; n++) {
			if (n >= (int)text.length()) {
				break;
			}
			if (text[n] == ':' && titleFlag) {
				titleFlag = false;
				n++;
				break;
			}
			strSub = text.substr(0, n);
		}
		std::string str = CutFaceText(text, n);
		_pNotify->PushHint(str.c_str(), dwColor);
	}
	//_pNotify->ReadyForHint( 170, 5 );
	_pNotify->ReadyForHintGM(170, 5);
}

//Add by sunny.sun20080804
//Begin
void CGameApp::ShowNotify1(const char* szStr, int setnum, DWORD dwColor) {
	_pNotify1->SetFixWidth(430);

	_dwNotifyTime1 = GetCurTick() + 240000; //120000

	_pNotify1->Clear();

	static bool titleFlag;
	titleFlag = false; //
	string text = szStr;

	_pNotify1->PushHint(text.c_str(), dwColor);


	_pNotify1->ReadyForHint(170, 100, setnum);
}

//End

void CGameApp::AutoTestInfo(const char* pszFormat, ...) {
	va_list list;
	va_start(list, pszFormat);
	vsprintf(_szOutBuf, pszFormat, list);
	va_end(list);

	string info = _szOutBuf;
	ShowMidText(info.c_str());
	info += "\n";
	g_logManager.InternalLog(LogLevel::Debug, "common", info.c_str());

	FrameMove(GetTickCount());
	Render();
}

void CGameApp::SysInfo(const char* pszFormat, ...) {
	va_list list;
	va_start(list, pszFormat);
	vsprintf(_szOutBuf, pszFormat, list);
	va_end(list);

	string info = _szOutBuf;
	//g_stUICoze.OnSystemSay( info.c_str() );
	CCozeForm::GetInstance()->OnSystemMsg(info.c_str());
}

void CGameApp::Waiting(bool isWaiting) {
	static CForm* frmWaiting = NULL;
	if (!frmWaiting) {
		frmWaiting = CFormMgr::s_Mgr.Find("frmConnect", enumSwitchSceneForm);
		if (frmWaiting) {
			frmWaiting->SetIsEnabled(false);
		}
	}

	if (frmWaiting) {
		if (isWaiting) {
			CCursor::I()->SetCursor(CCursor::stWait);
			frmWaiting->ShowModal();
		}
		else {
			CCursor::I()->SetCursor(CCursor::stNormal);
			frmWaiting->Close();
		}
	}
}

void CGameApp::SetCameraPos(D3DXVECTOR3& pos, bool bRestoreCustom) {
	CCameraCtrl* pCam = GetMainCam();

	//GetCameraTrack()->Reset(pos.x,pos.y,pos.z);

	pCam->InitPos(pos.x, pos.y, pos.z, bRestoreCustom);

	pCam->Update();

	pCam->SetFollowObj(pCam->m_vCurPos);

	pCam->FrameMove(0);

	g_Render.SetWorldViewFOV(Angle2Radian(pCam->m_ffov));
	g_Render.LookAt(pCam->m_EyePos, pCam->m_RefPos);
	g_Render.SetCurrentView(MPRender::VIEW_WORLD);
}

static CImage* flashProgress[32] = {0};
static CForm* frmLoading = nullptr;
static bool bChangeFormLoginToSelCha = false;

void CGameApp::_ShowLoading(int percent) {
	_nSwitchScene--;

	const int LOAD_DELAY = 10;
	if (GetCurScene()->GetTerrain() && _nSwitchScene > LOAD_DELAY) {
		CCharacter* pMain = CGameScene::GetMainCha();
		if (pMain) {
			float fX = (float)pMain->GetCurX() / 100.0f;
			float fY = (float)pMain->GetCurY() / 100.0f;
			float fHeight = GetCurScene()->GetGridHeight(fX, fY);
			bool isOK = false;
			if (fHeight > 0.001f) {
				isOK = true;
			}
			else {
				MPTerrain* pTer = GetCurScene()->GetTerrain();
				if (pTer && pTer->GetTileRegionAttr((int)fX, (int)fY) != 0) {
					isOK = true;
				}
			}
			if (isOK) {
				_nSwitchScene = LOAD_DELAY;
				_total = _nSwitchScene + 140;
				pMain->setPos(pMain->GetCurX(), pMain->GetCurY());
			}
		}
	}

	if (frmLoading) {
		for (int i = 0; i < 32; i++) {
			if (!flashProgress[i]) continue;
			flashProgress[i]->SetIsShow(false);
			if (i == 32 - _nSwitchScene / 4) {
				flashProgress[i]->SetPos(0, (int)(GetRender().GetScreenHeight() * 0.9f));
				flashProgress[i]->Refresh();
				flashProgress[i]->SetIsShow(true);
			}
		}

		int w_loadingbar = (int)(GetRender().GetScreenWidth() / 5 * 3) - 5;
		CForm* frmLoading = CFormMgr::s_Mgr.Find("frmLoading", enumSwitchSceneForm);
		if (frmLoading) {
			CImage* imgProgressLoad = dynamic_cast<CImage*>(frmLoading->Find("imgProgressLoad"));
			if (imgProgressLoad) {
				int wX = (int)(w_loadingbar - w_loadingbar * percent / 100);
				imgProgressLoad->SetPos((int)(GetRender().GetScreenWidth() / 5) + 1,
										(int)(GetRender().GetScreenHeight() * 0.935f));
				imgProgressLoad->SetSize(wX, 50);
				imgProgressLoad->Refresh();
			}
		}
	}

	if (_nSwitchScene <= -140) {
		_IsUserEnabled = true;
		SetInputActive(true);

		GetCurScene()->LoadingCall();
		CUIInterface::All_LoadingCall();

		if (frmLoading) {
			frmLoading->Close();
			GameLoading::Init()->Active();
			GameLoading::Init()->Close();
		}
	}
}

void CGameApp::RefreshLoadingProgress() {
	const int PROGRESS_CRYCLE = 1000;
	CForm* frmLoading = CFormMgr::s_Mgr.Find("frmLoading", enumSwitchSceneForm);
	if (frmLoading) {
		CImage* imgLoadingBar = dynamic_cast<CImage*>(frmLoading->Find("imgProgressLoad"));
		if (imgLoadingBar) {
			float fPrecent = ((GetCurTick() - _dwLoadingTick) % PROGRESS_CRYCLE) / (float)(PROGRESS_CRYCLE);
			int wX = (int)(TINY_RES_X * fPrecent);
			imgLoadingBar->SetSize(wX, 3);
			imgLoadingBar->Refresh();
		}
	}
}


void CGameApp::ResetCaption() {
	extern short g_sClientVer;
	char szSpace[250] = "";
	int nSpace = 40;
	int nWidth = GetRender().GetScreenWidth();
	if (nWidth == LARGE_RES_X) {
		nSpace = 140;
	}

	for (int i = 0; i < nSpace; i++) szSpace[i] = ' ';
	szSpace[nSpace] = '\0';

	char szCaption[350];
	sprintf(szCaption, CLIENT_NAME, (float)(g_sClientVer % 1000) / 100, szSpace);
	SetCaption(szCaption);
}

void CGameApp::Loading(int nFrame) {
	_IsUserEnabled = false;

	static std::string tiptext;

	auto tips = HelpEntryStore::Instance()->GetCategory(HelpEntryStore::CATEGORY_TIP);
	const int total_lines = static_cast<int>(tips.size());
	const int random_number = total_lines > 0 ? (rand() % total_lines) : 0;

	if (total_lines > 0) {
		tiptext = tips[random_number]->_content;
	}
	else {
		tiptext.clear();
	}

	//snd_enable( false );
	_nSwitchScene = nFrame;
	_total = _nSwitchScene + 140;
	SetInputActive(false);

	if (!frmLoading) {
		frmLoading = CFormMgr::s_Mgr.Find("frmLoading", enumSwitchSceneForm);
	}

	if (!frmLoading) {
		return;
	}

	CLabelEx* labTip = dynamic_cast<CLabelEx*>(frmLoading->Find("labTip"));

	switch (random_number) {
	case 0:
		labTip->SetPos((int)(GetRender().GetScreenWidth() / 2) - 550, (int)(GetRender().GetScreenHeight() * 0.97f));
		break;
	case 1:
		labTip->SetPos((int)(GetRender().GetScreenWidth() / 2) - 530, (int)(GetRender().GetScreenHeight() * 0.97f));
		break;
	case 2:
		labTip->SetPos((int)(GetRender().GetScreenWidth() / 2) - 170, (int)(GetRender().GetScreenHeight() * 0.97f));
		break;
	case 3:
		labTip->SetPos((int)(GetRender().GetScreenWidth() / 2) - 180, (int)(GetRender().GetScreenHeight() * 0.97f));
		break;
	case 4:
		labTip->SetPos((int)(GetRender().GetScreenWidth() / 2) - 310, (int)(GetRender().GetScreenHeight() * 0.97f));
		break;
	case 5:
		labTip->SetPos((int)(GetRender().GetScreenWidth() / 2) - 170, (int)(GetRender().GetScreenHeight() * 0.97f));
		break;
	case 6:
		labTip->SetPos((int)(GetRender().GetScreenWidth() / 2) - 290, (int)(GetRender().GetScreenHeight() * 0.97f));
		break;
	case 7:
		labTip->SetPos((int)(GetRender().GetScreenWidth() / 2) - 330, (int)(GetRender().GetScreenHeight() * 0.97f));
		break;
	case 8:
		labTip->SetPos((int)(GetRender().GetScreenWidth() / 2) - 330, (int)(GetRender().GetScreenHeight() * 0.97f));
		break;
	case 9:
		labTip->SetPos((int)(GetRender().GetScreenWidth() / 2) - 240, (int)(GetRender().GetScreenHeight() * 0.97f));
		break;
	case 10:
		labTip->SetPos((int)(GetRender().GetScreenWidth() / 2) - 240, (int)(GetRender().GetScreenHeight() * 0.97f));
		break;
	case 11:
		labTip->SetPos((int)(GetRender().GetScreenWidth() / 2) - 230, (int)(GetRender().GetScreenHeight() * 0.97f));
		break;
	case 12:
		labTip->SetPos((int)(GetRender().GetScreenWidth() / 2) - 265, (int)(GetRender().GetScreenHeight() * 0.97f));
		break;
	case 13:
		labTip->SetPos((int)(GetRender().GetScreenWidth() / 2) - 275, (int)(GetRender().GetScreenHeight() * 0.97f));
		break;
	case 14:
		labTip->SetPos((int)(GetRender().GetScreenWidth() / 2) - 410, (int)(GetRender().GetScreenHeight() * 0.97f));
		break;
	case 15:
		labTip->SetPos((int)(GetRender().GetScreenWidth() / 2) - 300, (int)(GetRender().GetScreenHeight() * 0.97f));
		break;
	case 16:
		labTip->SetPos((int)(GetRender().GetScreenWidth() / 2) - 210, (int)(GetRender().GetScreenHeight() * 0.97f));
		break;
	case 17:
		labTip->SetPos((int)(GetRender().GetScreenWidth() / 2) - 310, (int)(GetRender().GetScreenHeight() * 0.97f));
		break;
	case 18:
		labTip->SetPos((int)(GetRender().GetScreenWidth() / 2) - 220, (int)(GetRender().GetScreenHeight() * 0.97f));
		break;
	case 19:
		labTip->SetPos((int)(GetRender().GetScreenWidth() / 2) - 210, (int)(GetRender().GetScreenHeight() * 0.97f));
		break;
	case 20:
		labTip->SetPos((int)(GetRender().GetScreenWidth() / 2) - 440, (int)(GetRender().GetScreenHeight() * 0.97f));
		break;
	case 21:
		labTip->SetPos((int)(GetRender().GetScreenWidth() / 2) - 120, (int)(GetRender().GetScreenHeight() * 0.97f));
		break;
	case 22:
		labTip->SetPos((int)(GetRender().GetScreenWidth() / 2) - 260, (int)(GetRender().GetScreenHeight() * 0.97f));
		break;
	case 23:
		labTip->SetPos((int)(GetRender().GetScreenWidth() / 2) - 220, (int)(GetRender().GetScreenHeight() * 0.97f));
		break;
	}
	labTip->SetCaption(tiptext.c_str());

	int width = (GetRender().GetScreenWidth() == LARGE_RES_X) ? (LARGE_RES_X / 4) : (TINY_RES_X / 4);
	int height = (GetRender().GetScreenHeight() == LARGE_RES_Y) ? (LARGE_RES_Y / 3) : (TINY_RES_Y / 3);

	if (g_Config.m_bFullScreen) {
		width = GetSystemMetrics(SM_CXSCREEN) / 4;
		height = GetSystemMetrics(SM_CYSCREEN) / 3;
		height += 32;
	}

	frmLoading->SetPos(0, 0);
	frmLoading->SetSize(GetRender().GetScreenWidth(), GetRender().GetScreenHeight());
	frmLoading->Refresh();
	frmLoading->ShowModal();

	CImage* imgLoadingBg = nullptr;
	imgLoadingBg = dynamic_cast<CImage*>(frmLoading->Find("imgLoadingBg"));
	imgLoadingBg->_pImage->LoadImage("./texture/ui/LOADING/LoadBgScene.jpg");
	imgLoadingBg->SetSize(GetRender().GetScreenWidth(), GetRender().GetScreenHeight());
	imgLoadingBg->Refresh();

	CImage* imgProgressBar = dynamic_cast<CImage*>(frmLoading->Find("imgProgressBar"));
	if (imgProgressBar) {
		imgProgressBar->SetPos((int)(GetRender().GetScreenWidth() / 5), (int)(GetRender().GetScreenHeight() * 0.935f));
		imgProgressBar->SetSize((int)(GetRender().GetScreenWidth() / 5 * 3), 50);
		imgProgressBar->Refresh();
	}
}


void CGameApp::InitAllTable() {
	auto& db = AssetDatabase::Instance()->GetDb();

	ShadeRecordStore::Instance()->Load(db);
	EventSoundRecordStore::Instance()->Load(db);
	MusicRecordStore::Instance()->Load(db);
	PoseRecordStore::Instance()->Load(db);
	CharacterActionStore::Instance()->Load(db);
	CharacterModelStore::Instance()->Load(db);
	AnimatedLightStore::Instance()->Load(db);
	ChaCreateRecordStore::Instance()->Load(db);
	MapRecordStore::Instance()->Load(db);
	EventRecordStore::Instance()->Load(db);
	ServerRecordStore::Instance()->Load(db);
	ItemRecordStore::Instance()->Load(db);
	AreaRecordStore::Instance()->Load(db);
	NotifyRecordStore::Instance()->Load(db);
	ChatIconRecordStore::Instance()->Load(db);
	ItemTypeRecordStore::Instance()->Load(db);
	ItemPreRecordStore::Instance()->Load(db);
	EffParamRecordStore::Instance()->Load(db);
	GroupParamRecordStore::Instance()->Load(db);
	ItemRefineRecordStore::Instance()->Load(db);
	ItemRefineEffectRecordStore::Instance()->Load(db);
	ForgeRecordStore::Instance()->Load(db);
	ShipRecordStore::Instance()->Load(db);
	ShipPartRecordStore::Instance()->Load(db);
	ChaRecordStore::Instance()->Load(db);
	SkillRecordStore::Instance()->Load(db);
	SkillStateRecordStore::Instance()->Load(db);
	HairRecordStore::Instance()->Load(db);
	TerrainRecordStore::Instance()->Load(db, GetTextureID);
	SceneObjRecordStore::Instance()->Load(db, GetTextureID);
	EffectRecordStore::Instance()->Load(db, GetTextureID);
	MonsterListRecordStore::Instance()->Load(db);
	NPCListRecordStore::Instance()->Load(db);
	MonsterInfoRecordStore::Instance()->Load(db);
	MountRecordStore::Instance()->Load(db);
	ResourceRecordStore::Instance()->Load(db);
	StoneRecordStore::Instance()->Load(db);
	ElfSkillRecordStore::Instance()->Load(db);
	HelpEntryStore::Instance()->Load(db);
}

void CGameApp::ReleaseAllTable() {
}

bool CGameApp::LoadRes4() {
	int test = 0;
	char szBone[128];
	CCharacter* pChar = this->GetCurScene()->_GetFirstInvalidCha();
	for (int i = 1; i < 1200; i++) {
		CChaRecord* pInfo = GetChaRecordInfo(i);
		if (pInfo && (pInfo->chCtrlType == 1 || pInfo->chCtrlType == 2)) {
			sprintf(szBone, "%04d.lab", pInfo->sModel);
			pChar->InitBone(szBone);
			test++;
		}
	}
	return true;
}

void LoadResModelBuf(MPIResourceMgr* res_mgr) {
	auto* buf_mgr = res_mgr->GetResBufMgr();
	auto* path_info = res_mgr->GetSysGraphics()->GetSystem()->GetPathInfo();
	std::string model_path = path_info->GetPath(PATH_TYPE_MODEL_SCENE);

	SceneObjRecordStore::Instance()->ForEach([&](const CSceneObjInfo& info) {
		auto path = model_path + info._dataName;
		if (LW_FAILED(buf_mgr->RegisterModelObjInfo(info._id, path.c_str()))) {
			ToLogService("common", "cannot find model file: {}", path);
		}
	});
}

void CGameApp::SetFPSInterval(DWORD v) {
	if (v > 0) _pSteady->SetFPS(v);
}

void CGameApp::_SceneError(const char* info, CGameScene* p) {
	if (p) {
		MsgBox("%s", SafeVFormat(GetLanguageString(78), p->GetInitParam()->strName, info).c_str());
	}
	else {
		MsgBox("%s", SafeVFormat(GetLanguageString(79), info).c_str());
	}
}

bool CGameApp::HasLogFile(const char* log_file, bool isOpen) {
	// gui
	string file;
	file = g_logManager.GetLogDirectory();
	file += "/";
	file += log_file;
	file += ".log";
	FILE* fp = fopen(file.c_str(), "r");
	if (fp) {
		fclose(fp);

		if (isOpen) {
			char buf[256] = {0};
			sprintf(buf, "notepad %s", file.c_str());
			::WinExec(buf, SW_SHOWNORMAL);
		}
		return true;
	}
	return false;
}

void LimitCurrentProc() {
	HANDLE hCurrentProcess = GetCurrentProcess();

	// Get the processor affinity mask for this process
	DWORD_PTR dwProcessAffinityMask = 0;
	DWORD_PTR dwSystemAffinityMask = 0;

	if (GetProcessAffinityMask(hCurrentProcess, &dwProcessAffinityMask, &dwSystemAffinityMask) != 0 &&
		dwProcessAffinityMask) {
		// Find the lowest processor that our process is allows to run against
		DWORD_PTR dwAffinityMask = (dwProcessAffinityMask & ((~dwProcessAffinityMask) + 1));

		// Set this as the processor that our thread must always run against
		// This must be a subset of the process affinity mask
		HANDLE hCurrentThread = GetCurrentThread();
		if (INVALID_HANDLE_VALUE != hCurrentThread) {
			SetThreadAffinityMask(hCurrentThread, dwAffinityMask);
			CloseHandle(hCurrentThread);
		}
	}

	CloseHandle(hCurrentProcess);
}

extern const char* ConsoleCallback(const char*);

void CGameApp::AutoTest() {
	char szBuf[256] = {0};
	sprintf(szBuf, "testeffect 0 5000 1");
	ConsoleCallback(szBuf);
	sprintf(szBuf, "testskilleffect 0 5000 1");
	ConsoleCallback(szBuf);

	CGameScene* pScene = GetCurScene();
	if (!pScene) return;

	D3DXVECTOR3 TestPos;
	int nTestX = 0;
	int nTestY = 0;
	if (CGameScene::GetMainCha()) {
		TestPos = CGameScene::GetMainCha()->GetPos();
		TestPos.x -= 3.0f;

		nTestX = CGameScene::GetMainCha()->GetCurX() - 300;
		nTestY = CGameScene::GetMainCha()->GetCurY();
	}

	for (int i(0); i < 1; i++) {
		//
		{
			AutoTestInfo("%s", GetLanguageString(80).c_str());
			EffectRecordStore::Instance()->ForEach([&](const CMagicInfo& info) {
				auto* pEffect = pScene->GetFirstInvalidEffObj();
				if (!pEffect) return;

				if (!pEffect->Create(info.Id)) {
					AutoTestInfo("%s", SafeVFormat(GetLanguageString(81), info.Id).c_str());
					return;
				}

				pEffect->Emission(-1, &TestPos, NULL);
				pEffect->SetValid(TRUE);
				AutoTestUpdate();
				pEffect->SetValid(FALSE);
			});
		}
	}
	// ,,,
	{
		AutoTestInfo("%s", GetLanguageString(82).c_str());

		ChaRecordStore::Instance()->ForEach([&](const CChaRecord& info) {
			auto* pCha = pScene->AddCharacter(info.Id);
			if (!pCha) {
				AutoTestInfo("%s", SafeVFormat(GetLanguageString(83), info.Id).c_str());
				return;
			}

			pCha->setPos(nTestX, nTestY);
			AutoTestUpdate();
			pCha->SetValid(FALSE);
		});
	}

	// begin
	{
		AutoTestInfo("%s", GetLanguageString(84).c_str());

		CCharacter* pHairCha[4] = {NULL};
		int nMax = 4;
		for (int i = 0; i < nMax; i++) {
			pHairCha[i] = pScene->AddCharacter(i + 1);
			pHairCha[i]->setPos(nTestX, nTestY + i * 100 - 200);
		}
		HairRecordStore::Instance()->ForEach([&](CHairRecord& hair) {
			for (int j = 0; j < nMax; j++) {
				if (hair.IsChaUse[j]) {
					if (!pHairCha[j]->ChangePart(enumEQUIP_HEAD, hair.dwItemID))
						SysInfo("%s", SafeVFormat(GetLanguageString(85), hair.Id, j + 1, hair.dwItemID).c_str());

					for (int k = 0; k < hair.GetFailItemNum(); k++) {
						if (!pHairCha[j]->ChangePart(enumEQUIP_HEAD, hair.dwFailItemID[k]))
							SysInfo("%s", SafeVFormat(GetLanguageString(86), hair.Id, j + 1, hair.dwFailItemID[k]).c_str());
					}
					AutoTestUpdate();
				}
			}
		});
		for (int i = 0; i < nMax; i++) {
			pHairCha[i]->SetValid(FALSE);
		}
		// end
	}

	//
	{
		AutoTestInfo("%s", GetLanguageString(80).c_str());
		EffectRecordStore::Instance()->ForEach([&](const CMagicInfo& info) {
			auto* pEffect = pScene->GetFirstInvalidEffObj();
			if (!pEffect) return;

			if (!pEffect->Create(info.Id)) {
				AutoTestInfo("%s", SafeVFormat(GetLanguageString(81), info.Id).c_str());
				return;
			}

			pEffect->Emission(-1, &TestPos, NULL);
			pEffect->SetValid(TRUE);
			AutoTestUpdate();
			pEffect->SetValid(FALSE);
		});
	}

	//
	{
		int x = 0;
		int y = 0;
		string file;
		if (CGameScene::GetMainCha()) {
			x = CGameScene::GetMainCha()->GetCurX() - 300;
			y = CGameScene::GetMainCha()->GetCurY();
		}

		AutoTestInfo("%s", GetLanguageString(87).c_str());
		CItemRecord* pInfo = NULL;
		CSceneItem* pItem = NULL;
		CMagicInfo* pEffectInfo = NULL;
		int nEffectID = 0;
		auto checkEffect = [&](int effectID, int msgIdx, int id, const char* name) {
			if (effectID > 0 && !GetMagicInfo(effectID))
				AutoTestInfo("%s", SafeVFormat(GetLanguageString(msgIdx), id, name, effectID).c_str());
		};

		ItemRecordStore::Instance()->ForEach([&](CItemRecord& item) {
			auto id = static_cast<int>(item.lID);

			for (int j = 0; j < defITEM_MODULE_NUM; j++) {
				std::string_view module(item.chModule[j]);
				if (module == "0" || module.empty()) continue;

				auto itemPath = std::format("model/item/{}.lgo", module);
				auto chaPath = std::format("model/character/{}.lgo", module);
				if (!IsExistFile(itemPath.c_str()) && !IsExistFile(chaPath.c_str())) {
					AutoTestInfo("%s", SafeVFormat(GetLanguageString(j == 0 ? 88 : 89), id, item.szName, j, item.chModule[j]).c_str());
				}
			}

			if (std::string_view(item.szICON) != "0") {
				if (!IsExistFile(item.GetIconFile()))
					AutoTestInfo("%s", SafeVFormat(GetLanguageString(90), id, item.szName, item.szICON).c_str());
			}

			checkEffect(item.sDrap, 91, id, item.szName.c_str());
			for (int j = 0; j < item.sEffNum; j++)
				checkEffect(item.sEffect[j][0], 92, id, item.szName.c_str());
			checkEffect(item.sItemEffect[0], 93, id, item.szName.c_str());
			checkEffect(item.sAreaEffect[0], 94, id, item.szName.c_str());

			auto* sceneItem = pScene->AddSceneItem(id, 0);
			if (!sceneItem) {
				AutoTestInfo("%s", SafeVFormat(GetLanguageString(95), id, item.szName).c_str());
				return;
			}

			sceneItem->setPos(nTestX, nTestY);
			AutoTestUpdate();
			sceneItem->SetValid(FALSE);
		});
	}

	// ...
	{
		AutoTestInfo("%s", GetLanguageString(96).c_str());
		ItemRefineRecordStore::Instance()->ForEach([&](const CItemRefineInfo& refine) {
			for (int k = 0; k < ITEM_REFINE_NUM; k++) {
				int effectID = refine.Value[k];
				if (effectID > 0 && !GetItemRefineEffectInfo(effectID)) {
					AutoTestInfo("%s", SafeVFormat(GetLanguageString(97), refine.Id, std::string_view(refine.DataName.c_str()), effectID).c_str());
				}
			}
		});
	}

	// ...
	{
		AutoTestInfo("%s", GetLanguageString(98).c_str());
		ItemRefineEffectRecordStore::Instance()->ForEach([&](CItemRefineEffectInfo& info) {
			for (int k = 0; k < 4; k++) {
				int nEffectNum = info.GetEffectNum(k);
				for (int j = 0; j < nEffectNum; j++) {
					if (info.sEffectID[k][j] <= 0) continue;

					for (int level = 0; level < 4; level++) {
						int nEffectID = info.sEffectID[k][j] * 10 + level;
						if (!GetMagicInfo(nEffectID)) {
							AutoTestInfo("%s", SafeVFormat(GetLanguageString(99), info.Id, std::string_view(info.DataName.c_str()), nEffectID).c_str());
						}
					}
				}
			}
		});
	}

	//
	{
		AutoTestInfo("%s", GetLanguageString(100).c_str());

		g_pGameApp->HasLogFile("iteminfoerror");
	}

	g_pGameApp->HasLogFile("autotest");
}

void CGameApp::SendMessage(DWORD dwTypeID, DWORD dwParam1, DWORD dwParam2)
{
	int i = 0;
	while (!::PostMessage(GetHWND(), dwTypeID, dwParam1, dwParam2) && i <= 10)
	{
		Sleep(50);
		i++;
	}
}

void CGameApp::SetStartMinimap(int ix, int iy, int destx, int desty)
{
	xp = (float)((ix / SHOWRSIZE) * SHOWRSIZE) + (SHOWRSIZE / 2);
	yp = (float)((iy / SHOWRSIZE) * SHOWRSIZE) + (SHOWRSIZE / 2);

	destxp = (float)xp + destx * SHOWRSIZE;
	destyp = (float)yp + desty * SHOWRSIZE;

	xp1 = xp;
	yp1 = yp;
}

DWORD CGameApp::GetSkillClock(int skill_id)
{
	std::map<int, DWORD>::iterator it = m_mapSkillClock.find(skill_id);
	if (it != m_mapSkillClock.end())
	{
		return m_mapSkillClock[skill_id];
	}
	return NULL;
}

void CGameApp::SetSkillClock(int skill_id, DWORD dwSkillTime)
{
	if (!GetSkillClock(skill_id))
	{
		m_mapSkillClock[skill_id] = dwSkillTime;
	}
}

void CGameApp::DeleteSkillClock(int skill_id)
{
	std::map<int, DWORD>::iterator it = m_mapSkillClock.find(skill_id);
	if (it != m_mapSkillClock.end())
	{
		m_mapSkillClock.erase(skill_id);
	}
}
