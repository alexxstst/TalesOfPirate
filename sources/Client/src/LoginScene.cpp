#include "stdafx.h"
#include "LoginScene.h"


#include "GlobalVar.h"


#include "GameApp.h"
#include "Character.h"
#include "SceneObj.h"
#include "UiFormMgr.h"
#include "UiTextButton.h"
#include "CharacterAction.h"
#include "SceneItem.h"
#include "ItemRecord.h"
#include "PacketCmd.h"
#include "GameConfig.h"

#include "Character.h"
#include "UIRender.h"
#include "UIEdit.h"
#include "UILabel.h"
#include "uiformmgr.h"
#include "uitextbutton.h"
#include "uilabel.h"
#include "uiprogressbar.h"
#include "uiscroll.h"
#include "uilist.h"
#include "uicombo.h"
#include "uiimage.h"
#include "UICheckBox.h"
#include "uiimeinput.h"
#include "uigrid.h"
#include "uilistview.h"
#include "uipage.h"
#include "uitreeview.h"
#include "uiimage.h"
#include "UILabel.h"
#include "RenderStateMgr.h"
#include "cameractrl.h"
#include "UIListView.h"

#include "UIMemo.h"

#include "Connection.h"
#include "ServerSet.h"
#include "GameAppMsg.h"


#include "UI3DCompent.h"
#include "UIForm.h"
#include "UITemplete.h"
#include "commfunc.h"
#include "uiboxform.h"

#include "uisystemform.h"

#include <shellapi.h>

#include "xmlwriter.h"

using namespace std;

bool registerLogin = false;

char autoLoginName[32];
char autoLoginPassword[32];
bool useAutoLogin;
bool useAutoLogin2;
char Region[3];
char Server[3];

bool useModelMode = false;
bool modelMode = false;
char modelLook[8192];
int logoAlpha = 0;
int alphacount = 0;
int CLoginScene::nSelectChaType = 0; // Selected character type info
int CLoginScene::nSelectChaPart[enumEQUIP_PART_NUM] = {0};

CForm* CLoginScene::frmServer = nullptr;
CForm* CLoginScene::frmRegion = nullptr;
CForm* CLoginScene::frmAccount = nullptr;
CForm* CLoginScene::frmLOGO = nullptr;
CForm* CLoginScene::frmAnnounce = nullptr;
CForm* CLoginScene::frmKeyboard = nullptr; // add by Philip.Wu  Soft keyboard form  2006-06-05
CForm* CLoginScene::frmRegister = nullptr; // add by Philip.Wu  Soft keyboard form  2006-06-05
CForm* CLoginScene::frmPathLogo = nullptr;
CCheckBox* CLoginScene::chkID = nullptr;

CList* CLoginScene::lstRegion[NUM_REGIN_LIST];
CListView* CLoginScene::lstServer[NUM_SERVR_LIST];

CEdit* CLoginScene::edtID = nullptr;
CEdit* CLoginScene::edtPassword = nullptr;
CEdit* CLoginScene::edtFocus = nullptr; // add by Philip.Wu  Mouse-activated edit box  2006-06-06
CCheckBox* CLoginScene::chkShift = nullptr; // add by Philip.Wu  Keyboard Shift key  2006-06-09

CImage* CLoginScene::imgLogo1 = nullptr;
CImage* CLoginScene::imgLogo2 = nullptr;
CImage* CLoginScene::imgBigLogo = nullptr;
CImage* CLoginScene::imgBigLogo2 = nullptr;
CCharacter* CLoginScene::modelCha = nullptr;


//static			CCharacterBuilder* __character_scene = 0;
static CCharacter* _pCntCha[3] = {nullptr};
static CCharacter* pPxCha[2] = {nullptr}; // Login scene character models
static CCharacter* _pSelectCha = nullptr;

static int iSelectIndex = 0; // Index of selected 3D control
static int iRotateCha = 360; // Rotation when creating character, input button rotates view
static int iMiniRotateCha = 360;

static CLoginScene* g_login_scene = nullptr;


static void _GoBack(CCompent* pSender, int nMsgType, int x, int y, DWORD dwKey) {
	auto pLogin = dynamic_cast<CLoginScene*>(CGameApp::GetCurScene());
	if (pLogin) {
		pLogin->ShowLoginForm();
	}
}

#define MAX_SERVER_NUM	8

extern char* GetFlashDir(int UpLayers);

CLoginScene::CLoginScene(stSceneInitParam& param) :
	CGameScene(param),
	_eState(enumInit),
	m_bPasswordError(false),
	m_sPassport("nobill"),
	IsLoad(false) {
}

CLoginScene::~CLoginScene() {
	ToLogService("common", "CLoginScene Destroy");

	for (auto& i : _pCntCha)
		i = nullptr;
	_pSelectCha = nullptr;
}


bool CLoginScene::_Init() {
	g_login_scene = this;
	_IsUseSound = false;
	_eState = enumInit;

	if (!CGameScene::_Init()) {
		return false;
	}
	{
		// save loading res mt flag, and resume these flags in _Clear() before this scene destoried.
		lwIByteSet* res_bs = g_Render.GetInterfaceMgr()->res_mgr->GetByteSet();
		_loadtex_flag = res_bs->GetValue(OPT_RESMGR_LOADTEXTURE_MT);
		_loadmesh_flag = res_bs->GetValue(OPT_RESMGR_LOADMESH_MT);
		res_bs->SetValue(OPT_RESMGR_LOADTEXTURE_MT, 0);
		res_bs->SetValue(OPT_RESMGR_LOADMESH_MT, 0);
	}

	_bEnableCamDrag = TRUE;
	MPTimer tInit;
	tInit.Begin();

	//static bool IsLoad = false;
	static CGuiPic LoginPic;
	if (!IsLoad) {
		IsLoad = true;
		pObj = AddSceneObj(435);

		if (pObj) {
			pObj->SetCullingFlag(0);
			// position of the scene
			pObj->setPos(0, 0);
			pObj->setYaw(180);

			DWORD num = pObj->GetPrimitiveNum();
			for (DWORD i = 0; i < num; i++) {
				pObj->GetPrimitive(i)->SetState(STATE_TRANSPARENT, 0);
				pObj->GetPrimitive(i)->SetState(STATE_UPDATETRANSPSTATE, 0);
			}
			pObj->PlayDefaultAnimation(!g_stUISystem.m_sysProp.m_gameOption.bFramerate);
		}
	}

	g_Render.SetClip(g_Config.fnear, g_Config.ffar);

	CCameraCtrl* pCam = g_pGameApp->GetMainCam();
	if (pCam) {
		g_pGameApp->EnableCameraFollow(TRUE);
		pCam->m_EyePos.x = 103.749f;
		pCam->m_EyePos.y = 150.923f;
		pCam->m_EyePos.z = 320.982f;

		pCam->m_RefPos.x = 0.034f;
		pCam->m_RefPos.y = -294.137f;
		pCam->m_RefPos.z = 0.868f;
	}
	g_Render.SetWorldViewFOV(Angle2Radian(70.0f));
	g_Render.SetWorldViewAspect(1.33f);
	g_Render.SetClip(1.0f, 2000.0f);

	g_Render.LookAt(pCam->m_EyePos, pCam->m_RefPos);
	g_Render.SetCurrentView(MPRender::VIEW_WORLD);
	MPIDeviceObject* dev_obj = g_Render.GetInterfaceMgr()->dev_obj;

	//SetupVertexFog(dev_obj, 0, 0, D3DCOLOR_XRGB(28, 221, 246), D3DFOG_EXP2, 1, 0.0006f);
	//SetupVertexFog(dev_obj, 0, 0, D3DCOLOR_XRGB(86, 209, 246), D3DFOG_EXP2, 1, 0.0006f);

	g_Render.SetRenderStateForced(D3DRS_LIGHTING, 0);
	g_Render.SetRenderState(D3DRS_AMBIENT, 0xffffffff);

	/////////////////////////////////////

	m_sUsername = "player";
	m_sPassword = "";

	if (!_InitUI()) {
		g_logManager.InternalLog(LogLevel::Debug, "common", GetLanguageString(168));

		return false;
	}

	CFormMgr::s_Mgr.SetEnabled(TRUE);
	_pMainCha = nullptr;

	// Delete auto-updater from previous session
	char szUpdateFileName[] = "_Update.exe";
	SetFileAttributes(szUpdateFileName, FILE_ATTRIBUTE_NORMAL);
	DeleteFile(szUpdateFileName);

	PlayWhalePose();
	/* ShowLoginForm(); */
	if (useModelMode) {
		CForm* frmModel = CFormMgr::s_Mgr.Find("frmModel");
		frmAccount->Hide();
		frmModel->Show();
		C3DCompent* ui3dCha;
		if (!modelMode) {
			ui3dCha = dynamic_cast<C3DCompent*>(frmModel->Find("ui3dCha"));
			frmModel->Find("iconBack")->SetIsShow(false);
		}
		else {
			ui3dCha = dynamic_cast<C3DCompent*>(frmModel->Find("ui3dIcon"));
			frmModel->Find("cardBack")->SetIsShow(false);
		}
		if (ui3dCha) {
			stNetChangeChaPart pLook;
			memset(&pLook, 0, sizeof(pLook));
			string strLook(modelLook);
			if (String2LookData(pLook, strLook)) {
				modelCha = AddCharacter(pLook.sTypeID);
				modelCha->SetIsForUI(1);
				modelCha->UpdataFace(pLook);
			}
			ui3dCha->SetRenderEvent(__cha_render_event);
		}
		return true;
	}

	if (useAutoLogin) {
		//prevent issues on logout/change char.
		useAutoLogin = false;
		m_sUsername = autoLoginName;
		m_sPassword = autoLoginPassword;
		_Connect();
	}


	return true;
}

void CLoginScene::__cha_render_event(C3DCompent* pSender, int x, int y) {
	g_Render.GetDevice()->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	g_Render.GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	g_Render.LookAt(D3DXVECTOR3(11.0f, 36.0f, 10.0f), D3DXVECTOR3(8.70f, 12.0f, 8.0f), MPRender::VIEW_3DUI);
	y += 100;
	MPMatrix44 old_mat = *modelCha->GetMatrix();

	if (!modelMode) {
		modelCha->SetUIYaw(160);
		modelCha->SetUIScaleDis(5.0f);
	}
	else {
		modelCha->SetUIYaw(140);
		modelCha->SetUIScaleDis(1.7f);
		int chatype = modelCha->GetPart().sTypeID;
		if (chatype == 1) {
			y += 140;
		}
		else if (chatype == 2) {
			y += 240;
		}
		else if (chatype == 3) {
			y += 140;
		}
		else if (chatype == 4) {
			y += 20;
		}
	}
	modelCha->RenderForUI(x, y);
	modelCha->SetMatrix(&old_mat);
	g_Render.SetTransformView(&g_Render.GetWorldViewMatrix());


	char szMD5[33] = {0};
	md5string(modelLook, szMD5);

	char file[32];
	if (!modelMode) {
		sprintf(file, "./player/%s.png", szMD5);
	}
	else {
		sprintf(file, "./icon/%s.png", szMD5);
	}
	g_Render.CaptureScreen(file);
	g_ChaExitOnTime.ExitApp();
}

void CLoginScene::PlayWhalePose() {
}

bool CLoginScene::_Clear() {
	if (!CGameScene::_Clear()) {
		return false;
	}

	//    g_Render.SetRenderState(D3DRS_FOGENABLE, 0);
	{
		// reset loading res mt flag
		if (_loadtex_flag != 9 && _loadmesh_flag != 9) {
			lwIByteSet* res_bs = g_Render.GetInterfaceMgr()->res_mgr->GetByteSet();
			res_bs->SetValue(OPT_RESMGR_LOADTEXTURE_MT, _loadtex_flag);
			res_bs->SetValue(OPT_RESMGR_LOADMESH_MT, _loadmesh_flag);
		}
	}

	g_Render.SetClip(1.F, 1000.0f); //1000.0f orignial

	return 1;
}

void CLoginScene::ShowLoginForm() {
	//imgBigLogo->GetImage()->LoadImage("texture/ui/forsaken/logo1.png", 500, 600, 0, 0, 1.0, 1.0);


	chkID->SetIsChecked(m_bSaveAccount);
	edtID->SetCaption(m_sSaveAccount.c_str());
	edtPassword->SetCaption("");
	frmAccount->Show();

	// add by Philip.Wu  2006-07-03  Show logo when login form is displayed
	frmKeyboard->SetIsShow(false);
	imgLogo1->SetIsShow(true);
	imgLogo2->SetIsShow(true);

	if (m_sSaveAccount == "") {
		GUI::CEdit::SetActive(edtID);
	}
	else {
		GUI::CEdit::SetActive(edtPassword);
	}
}


void CLoginScene::_FrameMove(DWORD dwTimeParam) {
	//CGameScene::_FrameMove(dwTimeParam);
	int x = g_pGameApp->GetMouseX();
	int y = g_pGameApp->GetMouseY();
	GetRender().ScreenConvert(x, y);

	if (frmServer->GetIsShow()) {
	}

	if (frmRegion->GetIsShow()) {
		for (auto& index : lstRegion) {
			if (!index->InRect(x, y))
				index->GetItems()->GetSelect()->SetNoSelect();
		}
	}

	if (_eState == enumConnect) {
		switch (g_NetIF->GetConnStat()) {
		case Connection::CNST_CONNECTING:
			return;
		case Connection::CNST_INVALID:
		case Connection::CNST_FAILURE: {
			// Return to server selection screen
			_eState = enumInit;
			CGameApp::Waiting(false);

			ShowKeyboard(false);
			frmRegion->SetIsShow(false);
			//frmAccount->SetIsShow(false);
			GUI::CBoxMgr::ShowMsgBox(_GoBack, GetLanguageString(169).c_str());
		}
			return;
		case Connection::CNST_CONNECTED:
			_Login();
			return;

		case Connection::CNST_HANDSHAKE: {
			CGameApp::Waiting(true);
			return;
		}
		case Connection::CNST_TIMEOUT:
			_eState = enumInit;
			g_pGameApp->SendMessage(APP_NET_DISCONNECT, 1000);
			return;
		}
		return;
	}
}

void CLoginScene::_Render() {
	static bool IsLoad = false;
	static CGuiPic LoginPic;
	if (!IsLoad) {
		LoginPic.LoadImage("texture/ui/new_login.jpg", 1198, 768, 0, 0, 0, 1.F, 1.F);
		IsLoad = true;
	}
	LoginPic.SetScale(0, GetRender().GetScreenWidth(), GetRender().GetScreenHeight());
	LoginPic.Render(0, 0);
}


void CLoginScene::LoadingCall() {
	if (g_dwCurMusicID != 1) g_pGameApp->PlayMusic(1);
}

//-----------------
// Login callbacks Routines
//-----------------
void CLoginScene::CallbackUIEvent_LoginScene(CCompent* pSender, int state, int x, int y, DWORD key) {
	string strName = pSender->GetName();
	auto pScene = dynamic_cast<CLoginScene*>(CGameApp::GetCurScene());
	if (!pScene)
		return;

	if (stricmp("frmAnnounce", pSender->GetForm()->GetName()) == 0) {
		if (strName == "btnYes") {
			//Close announcement dialog
			pSender->GetForm()->SetIsShow(false);
		}
	}
	else if (stricmp("frmAccount", pSender->GetForm()->GetName()) == 0) {
		if (strName == "btnYes") {
			pScene->LoginFlow();
		}
		else if (strName == "btnNo") {
			if (g_NetIF->IsConnected()) {
				CS_Disconnect(DS_DISCONN);
			}
			pSender->GetForm()->Hide();
			return;
		}
	}
}

void CLoginScene::_evtServerFrm(CCompent* pSender, int state, int x, int y, DWORD key) {
	auto pkScene = dynamic_cast<CLoginScene*>(CGameApp::GetCurScene());
	if (!pkScene) return;

	string strName = pSender->GetName();
	if (strName == "btnNo") {
		pSender->GetForm()->SetIsShow(false);

		frmRegion->SetIsShow(true);
	}
}

void CLoginScene::_evtRegionFrm(CCompent* pSender, int state, int x, int y, DWORD key) {
	auto pkScene = dynamic_cast<CLoginScene*>(CGameApp::GetCurScene());
	if (!pkScene) return;

	string strName = pSender->GetName();
	if (strName == "btnNo") {
		//Close server list, show announcement dialog
		pSender->GetForm()->SetIsShow(false);

		g_pGameApp->SetIsRun(false);
	}
}

void CLoginScene::_evtLoginFrm(CCompent* pSender, int state, int x, int y, DWORD key) {
	auto pkScene = dynamic_cast<CLoginScene*>(CGameApp::GetCurScene());
	if (!pkScene) return;

	string strName = pSender->GetName();
	if (strName == "btnYes") {
		// First close soft keyboard
		if (frmKeyboard->GetIsShow()) {
			frmKeyboard->SetIsShow(false);

			imgLogo1->SetIsShow(true);
			imgLogo2->SetIsShow(true);
		}

		// Connect to server
		registerLogin = false;
		pkScene->LoginFlow();
	}
	else if (strName == "btnNo") {
		// First close soft keyboard
		if (frmKeyboard->GetIsShow()) {
			frmKeyboard->SetIsShow(false);

			imgLogo1->SetIsShow(true);
			imgLogo2->SetIsShow(true);
		}

		pSender->GetForm()->SetIsShow(false);
		frmRegion->SetIsShow(true);
	}
	else if (strName == "btnKeyboard" /*|| strName == "btnRegNo"*/) {
		string strURL = "https://nexusmmo.com";
		ShellExecute(nullptr, "open", strURL.c_str(), nullptr, nullptr, SW_SHOW);
		/*
		bool bShow = frmRegister->GetIsShow();
		frmRegister->SetIsShow(!frmRegister->GetIsShow());
		CEdit* edtRegID = dynamic_cast<CEdit*>(frmRegister->Find( "edtRegID" ));
		CEdit* edtRegPassword = dynamic_cast<CEdit*>(frmRegister->Find( "edtRegPassword" ));
		CEdit* edtRegPassword2 = dynamic_cast<CEdit*>(frmRegister->Find( "edtRegPassword2" ));
		CEdit* edtRegEmail = dynamic_cast<CEdit*>(frmRegister->Find( "edtRegEmail" ));
		edtRegID->SetCaption("");
		edtRegPassword->SetCaption("");
		edtRegPassword2->SetCaption("");
		edtRegEmail->SetCaption("");
		*/
	}
	else if (strName == "btnRegYes") {
		//registerLogin = true;
		//pkScene->LoginFlow();
	}
}

//-----------------------------------------------------------------------------
void CLoginScene::_evtServerFrmBeforeShow(CForm* pForm, bool& IsShow) {
}

//-----------------------------------------------------------------------------
void CLoginScene::_evtServerFrmOnClose(CForm* pForm, bool& IsClose) {
}

void CLoginScene::_evtServerLDBDown(CGuiData* pSender, int x, int y, DWORD key) {
	auto pkScene = dynamic_cast<CLoginScene*>(CGameApp::GetCurScene());
	if (!pkScene) return;

	auto pkServerList = dynamic_cast<CList*>(pSender);
	if (!pkServerList) return;

	if (pkServerList == lstServer[0]->GetList()) {
		pkScene->SetCurSelServerIndex(pkServerList->GetItems()->GetSelect()->GetIndex() * 2);
	}
	else if (pkServerList == lstServer[1]->GetList()) {
		pkScene->SetCurSelServerIndex(pkServerList->GetItems()->GetSelect()->GetIndex() * 2 + 1);
	}

	if (key & Mouse_LDown) {
		pSender->GetForm()->Hide();
		pkScene->ShowLoginForm();
	}
}

void CLoginScene::_evtRegionLDBDown(CGuiData* pSender, int x, int y, DWORD key) {
	auto pkScene = dynamic_cast<CLoginScene*>(CGameApp::GetCurScene());
	if (!pkScene) return;

	auto pkRegionList = dynamic_cast<CList*>(pSender);
	if (!pkRegionList) return;

	if (pkRegionList == lstRegion[0]) {
		pkScene->SetCurSelRegionIndex(pkRegionList->GetItems()->GetSelect()->GetIndex() * 2);
	}
	else if (pkRegionList == lstRegion[1]) {
		pkScene->SetCurSelRegionIndex(pkRegionList->GetItems()->GetSelect()->GetIndex() * 2 + 1);
	}

	if (key & Mouse_LDown) {
		pSender->GetForm()->SetIsShow(false);
		pkScene->InitServerList(pkScene->GetCurSelRegionIndex());
		frmServer->SetIsShow(true);
	}
}

void CLoginScene::_evtEnter(CGuiData* pSender) // added by billy
{
	auto pScene = dynamic_cast<CLoginScene*>(CGameApp::GetCurScene());
	if (!pScene) return;
	pScene->LoginFlow();
}

void CLoginScene::InitRegionList() {
	lstRegion[0]->GetItems()->Clear();
	lstRegion[1]->GetItems()->Clear();

	int idx = 0;
	for (auto& [name, _] : ServerRecordStore::Instance()->m_regionGroups) {
		lstRegion[idx % 2]->Add(name.c_str());
		idx++;
	}

	SetCurSelRegionIndex(0);
	CListItems* items = lstRegion[0]->GetItems();
	if (!items)
		return;
	items->Select(GetCurSelRegionIndex());
}

void CLoginScene::InitServerList(int nRegionNo) {
	lstServer[0]->GetList()->GetItems()->Clear();
	lstServer[1]->GetList()->GetItems()->Clear();

	for (int i = 0; i < GetCurServerGroupCnt(nRegionNo); i++) {
		CItemRow* item_row = lstServer[i % 2]->AddItemRow();
		if (item_row) {
			auto v7 = new CItem(GetCurServerGroupName(nRegionNo, i).c_str(), COLOR_BLACK);
			v7->SetColor(lstServer[1]->GetList()->GetFontColor());

			auto v10 = new CGraph(*imgServerIcons->GetImage());

			item_row->SetIndex(0, v10);
			item_row->SetIndex(1, v7);
		}
	}

	SetCurSelServerIndex(0);
	CListItems* items = lstServer[0]->GetList()->GetItems();
	if (!items)
		return;
	items->Select(GetCurSelServerIndex());
	lstServer[0]->Refresh();
	lstServer[1]->Refresh();
}

BOOL CLoginScene::_InitUI() {
	frmServer = CFormMgr::s_Mgr.Find("frmServer");
	if (!frmServer) return false;
	frmServer->evtEntrustMouseEvent = _evtServerFrm;
	lstServer[0] = dynamic_cast<CListView*>(frmServer->Find("lvServer0"));
	if (!lstServer[0]) return false;
	lstServer[0]->GetList()->evtListMouseDown = _evtServerLDBDown;

	lstServer[1] = dynamic_cast<CListView*>(frmServer->Find("lvServer1"));
	if (!lstServer[1]) return false;
	lstServer[1]->GetList()->evtListMouseDown = _evtServerLDBDown;

	imgServerIcons = dynamic_cast<CImage*>(frmServer->Find("imgServerIcon0"));
	if (!imgServerIcons) return false;
	imgServerIcons->SetIsShow(false);

	frmRegion = CFormMgr::s_Mgr.Find("frmArea");
	if (!frmRegion) return false;
	frmRegion->evtEntrustMouseEvent = _evtRegionFrm;

	lstRegion[0] = dynamic_cast<CList*>(frmRegion->Find("lstRegion0"));
	if (!lstRegion[0]) return false;
	lstRegion[0]->evtListMouseDown = _evtRegionLDBDown;

	lstRegion[1] = dynamic_cast<CList*>(frmRegion->Find("lstRegion1"));
	if (!lstRegion[1]) return false;
	lstRegion[1]->evtListMouseDown = _evtRegionLDBDown;

	InitRegionList();
	frmRegion->SetIsShow(true);

	frmPathLogo = CFormMgr::s_Mgr.Find("frmPathLogo");
	if (!frmPathLogo) return false;
	frmPathLogo->SetIsShow(false);

	frmAccount = CFormMgr::s_Mgr.Find("frmAccount");
	if (!frmAccount) return false;
	frmAccount->SetIsShow(false);
	frmAccount->evtEntrustMouseEvent = _evtLoginFrm;


	chkID = dynamic_cast<CCheckBox*>(frmAccount->Find("chkID"));
	m_bSaveAccount = false;
	if (!chkID) return false;

	char szChkID[128] = {0};
	char szChkTestServer[128] = {0};
	string strChkID;
	string strChkTestServer;
	ifstream inCheck("user\\checkid.txt");
	if (inCheck.is_open()) {
		inCheck.getline(szChkID, 128, '\n');
		strChkID = szChkID;
		int nCheck = Str2Int(strChkID);
		m_bSaveAccount = (nCheck == 1) ? true : false;
		chkID->SetIsChecked(m_bSaveAccount);
	}
	else {
		m_bSaveAccount = true;
		chkID->SetIsChecked(m_bSaveAccount);
	}

	edtID = dynamic_cast<CEdit*>(frmAccount->Find("edtID"));

	if (!edtID) return false;
	m_sSaveAccount = "";

	char szName[128] = {0};
	ifstream in("user\\username.txt");

	_bAutoInputAct = FALSE;
	if (in.is_open()) {
		while (!in.eof()) {
			in.getline(szName, 128);
		}
		_bAutoInputAct = TRUE;
	}
	m_sSaveAccount = string(szName);
	edtID->SetCaption(m_sSaveAccount.c_str());

	if (edtID) {
		edtID->evtEnter = _evtEnter;
		edtID->SetIsWrap(true);
	}


	edtPassword = dynamic_cast<CEdit*>(frmAccount->Find("edtPassword"));
	if (edtPassword) {
		edtPassword->SetCaption("");
		edtPassword->SetIsPassWord(true);
		edtPassword->SetIsWrap(true);
		edtPassword->evtEnter = _evtEnter;
	}

	frmKeyboard = CFormMgr::s_Mgr.Find("frmKeyboard");
	if (!frmKeyboard) return false;
	frmKeyboard->Hide();

	frmRegister = CFormMgr::s_Mgr.Find("frmRegister");
	if (!frmRegister) return false;
	frmRegister->Hide();
	frmRegister->evtEntrustMouseEvent = _evtLoginFrm;

	auto edtRegPassword = dynamic_cast<CEdit*>(frmRegister->Find("edtRegPassword"));
	auto edtRegPassword2 = dynamic_cast<CEdit*>(frmRegister->Find("edtRegPassword2"));
	edtRegPassword->SetIsPassWord(true);
	edtRegPassword2->SetIsPassWord(true);


	chkShift = dynamic_cast<CCheckBox*>(frmKeyboard->Find("chkShift"));
	if (!chkShift) return false;

	frmKeyboard->evtEntrustMouseEvent = _evtKeyboardFromMouseEvent;

	edtID->evtActive = _evtAccountFocus;
	edtPassword->evtActive = _evtAccountFocus;

	imgLogo1 = dynamic_cast<CImage*>(frmAccount->Find("imgLogo1"));
	if (!imgLogo1) return false;

	imgLogo2 = dynamic_cast<CImage*>(frmAccount->Find("imgLogo2"));
	if (!imgLogo2) return false;

	return TRUE;
}

void CLoginScene::CloseNewChaFrm() {
}

bool CLoginScene::IsValidCheckChaName(const std::string& name) {
	if (!::IsValidName(name)) {
		CGameApp::MsgBox("%s", GetLanguageString(51).c_str());
		return false;
	}
	return true;
}

bool CLoginScene::_CheckAccount() {
	// Additional keyboard check for validity
	if (registerLogin) {
		auto edtRegPassword = dynamic_cast<CEdit*>(frmRegister->Find("edtRegPassword"));
		auto edtRegPassword2 = dynamic_cast<CEdit*>(frmRegister->Find("edtRegPassword2"));
		auto edtRegEmail = dynamic_cast<CEdit*>(frmRegister->Find("edtRegEmail"));

		if (strcmp(edtRegPassword->GetCaption(), edtRegPassword2->GetCaption()) != 0) {
			CGameApp::MsgBox("Two passwords do not match.");
			return false;
		}
		else {
			return true;
		}
	}

	if (strlen(edtID->GetCaption()) == 0) {
		CGameApp::MsgBox("%s", GetLanguageString(174).c_str());
		return false;
	}
	if (!IsValidCheckChaName(edtID->GetCaption()))
		return false;

	if (strlen(edtPassword->GetCaption()) <= 4) {
		CGameApp::MsgBox("%s", GetLanguageString(175).c_str());
		return false;
	}

	// Save username
	SaveUserName(*chkID, *edtID);

	m_sUsername = edtID->GetCaption();
	m_sPassword = edtPassword->GetCaption();

	return true;
}

void CLoginScene::_Connect() {
	CGameApp::Waiting(true);

	//PlayWhalePose();	// Animation disabled since it does not play at init (Michael Chen 2005-06-03)

	_eState = enumConnect;

	g_logManager.InternalLog(LogLevel::Debug, "connections", SafeVFormat(GetLanguageString(179), m_iCurSelRegionIndex, m_iCurSelServerIndex));
	//int nSelRegionNo = 0;
	//int nNO = lstServer->GetItems()->GetSelect()->GetIndex();

	std::string selectGateIP;

	if (useAutoLogin2) {
		int region = atoi(Region);
		int server = atoi(Server);

		if (selectGateIP.empty()) selectGateIP = SelectGroupIP(region - 1, server - 1);
		useAutoLogin2 = false;
	}
	else {
		if (selectGateIP.empty()) selectGateIP = SelectGroupIP(m_iCurSelRegionIndex, m_iCurSelServerIndex);
	}

	if (selectGateIP.empty()) {
		g_logManager.InternalLog(LogLevel::Debug, "connections", SafeVFormat(GetLanguageString(180), 0, 0));
	}
	else {
		CS_Connect(selectGateIP.c_str(), 1973, g_Config.m_nConnectTimeOut);
	}
	//#endif
}


void CLoginScene::LoginFlow() {
	if (!_CheckAccount()) {
		return;
	}
	m_sPassport = "nobill";
	_Connect();
}

void CLoginScene::_Login() {
	_eState = enumAccount;
	if (registerLogin) {
		auto edtRegID = dynamic_cast<CEdit*>(frmRegister->Find("edtRegID"));
		auto edtRegPassword = dynamic_cast<CEdit*>(frmRegister->Find("edtRegPassword"));
		auto edtRegEmail = dynamic_cast<CEdit*>(frmRegister->Find("edtRegEmail"));
		CS_Register(edtRegID->GetCaption(), edtRegPassword->GetCaption(), edtRegEmail->GetCaption());
		registerLogin = false;
		CGameApp::Waiting();
	}
	else if (m_sUsername.size() != 0 && m_sPassword.size() != 0) {
		CS_Login(m_sUsername.c_str(), m_sPassword.c_str(), m_sPassport.c_str());
		//CGameApp::Waiting();
	}
}

void CLoginScene::SaveUserName(CCheckBox& chkID, CEdit& edtID) {
	//Create directory for file
	if (!CreateDirectory("user", nullptr)) {
	}

	m_bSaveAccount = chkID.GetIsChecked();
	m_sSaveAccount = string(edtID.GetCaption());

	//Write to file
	FILE* fchk = fopen("user\\checkid.txt", "wb");
	if (fchk) {
		fwrite(m_bSaveAccount ? "1" : "0", strlen("1"), 1, fchk);
		fwrite("\n", strlen("\n"), 1, fchk);
		fwrite(m_bTestServer ? "1" : "0", strlen("1"), 1, fchk);
		fclose(fchk);
	}

	FILE* fp = fopen("user\\username.txt", "wb");
	if (fp) {
		if (m_bSaveAccount)
			fwrite(m_sSaveAccount.c_str(), m_sSaveAccount.size(), 1, fp);
		else
			fwrite("", 0, 1, fp);

		fclose(fp);
	}
}


void CLoginScene::BeginPlay() {
}

void CLoginScene::_evtVerErrorFrm(CCompent* pSender, int nMsgType, int x, int y, DWORD key) {
	g_pGameApp->SetIsRun(false);

	if (nMsgType != CForm::mrYes) {
		// Open a web page
		if (strlen(g_Config.m_szVerErrorHTTP) == 0)
			return;

		/*	2008-10-15	close!
		::ShellExecute( NULL, "open",
			g_Config.m_szVerErrorHTTP,
			NULL, NULL, SW_SHOW);
		*/
		return;
	}

	// Auto update
	extern bool g_IsAutoUpdate;
	g_IsAutoUpdate = true;
}

void CLoginScene::Error(int error_no, const std::string& error_info) {
	CGameApp::Waiting(false);
	ToLogService("errors", LogLevel::Error, "{} Error, Code:{}, Info: {}", error_info, error_no,
				 g_GetServerError(error_no));

	if (ERR_MC_VER_ERROR == error_no) {
		CBoxMgr::ShowSelectBox(_evtVerErrorFrm, GetLanguageString(181).c_str(), true);
		return;
	}

	CGameApp::MsgBox("%s", g_GetServerError(error_no));
}

void CLoginScene::ReSetNewCha() {
	_pSelectCha = CGameApp::GetCurScene()->AddCharacter(CLoginScene::nSelectChaType);
	for (int i = 0; i < enumEQUIP_PART_NUM; i++) {
		_pSelectCha->ChangePart(i, CLoginScene::nSelectChaPart[i]);
	}
}

void CLoginScene::ShowChaList() {
	if (frmAccount) {
		frmAccount->Hide();
	}
	if (frmAnnounce) {
		frmAnnounce->Hide();
	}
	if (frmServer) {
		frmServer->Hide();
	}
}

void CLoginScene::ShowServerList() {
	CS_Disconnect(DS_DISCONN);

	if (frmKeyboard) // add by Philip.Wu  2006-07-21
		ShowKeyboard(false);

	if (frmAccount) {
		frmAccount->Hide();
	}
	if (frmAnnounce) {
		frmAnnounce->Hide();
	}
	if (frmServer) {
		frmServer->Show();
	}
}

void CLoginScene::ShowRegionList() {
	CS_Disconnect(DS_DISCONN);

	if (frmKeyboard) // add by Philip.Wu  2006-06-05
		ShowKeyboard(false);

	if (frmAccount)
		frmAccount->SetIsShow(false);

	if (frmAnnounce)
		frmAnnounce->SetIsShow(false);

	if (frmServer)
		frmServer->SetIsShow(false);

	InitRegionList();

	if (frmRegion)
		frmRegion->SetIsShow(true);
}

int CLoginScene::GetServIconIndex(int iNum) {
	if (iNum < 0) return 0;
	if (iNum > ServIconNum) return ServIconNum;
	return iNum;
}


// add by Philip.Wu  2006-06-05
// Soft keyboard button click event handler
void CLoginScene::_evtKeyboardFromMouseEvent(CCompent* pSender, int nMsgType, int x, int y, DWORD dwKey) {
	if (!edtFocus) return;

	auto pLoginScene = dynamic_cast<CLoginScene*>(CGameApp::GetCurScene());
	if (!pLoginScene) return;

	string strText = edtFocus->GetCaption();
	string strName = pSender->GetName();
	if (strName.size() <= 0) return;

	// Handle message info
	if (strName == "btnClose" || strName == "btnYes") // Close keyboard
	{
		if (frmKeyboard->GetIsShow()) {
			ShowKeyboard(false);
		}
	}
	else if (strName == "btnDel") // Delete last character
	{
		if (strText.size() > 0) {
			strText.resize(strText.size() - 1);
			edtFocus->SetCaption(strText.c_str());
		}
	}
	else if (strName == "chkShift") // Toggle case
	{
	}
	else if (strName == "btnOther101") // First row special characters
	{
		strText += '~';
		edtFocus->SetCaption(strText.c_str());
	}
	else if (strName == "btnOther102") {
		strText += '!';
		edtFocus->SetCaption(strText.c_str());
	}
	else if (strName == "btnOther103") {
		strText += '@';
		edtFocus->SetCaption(strText.c_str());
	}
	else if (strName == "btnOther104") {
		strText += '#';
		edtFocus->SetCaption(strText.c_str());
	}
	else if (strName == "btnOther105") {
		strText += '$';
		edtFocus->SetCaption(strText.c_str());
	}
	else if (strName == "btnOther106") {
		strText += '%';
		edtFocus->SetCaption(strText.c_str());
	}
	else if (strName == "btnOther107") {
		strText += '^';
		edtFocus->SetCaption(strText.c_str());
	}
	else if (strName == "btnOther108") {
		strText += '&';
		edtFocus->SetCaption(strText.c_str());
	}
	else if (strName == "btnOther109") {
		strText += '*';
		edtFocus->SetCaption(strText.c_str());
	}
	else if (strName == "btnOther110") {
		strText += '(';
		edtFocus->SetCaption(strText.c_str());
	}
	else if (strName == "btnOther111") {
		strText += ')';
		edtFocus->SetCaption(strText.c_str());
	}
	else if (strName == "btnOther112") {
		strText += '_';
		edtFocus->SetCaption(strText.c_str());
	}
	else if (strName == "btnOther113") {
		strText += '+';
		edtFocus->SetCaption(strText.c_str());
	}
	else if (strName == "btnOther114") {
		strText += '|';
		edtFocus->SetCaption(strText.c_str());
	}

	else if (strName == "btnOther201") // Second row special characters
	{
		strText += '`';
		edtFocus->SetCaption(strText.c_str());
	}
	else if (strName == "btnOther202") {
		strText += '-';
		edtFocus->SetCaption(strText.c_str());
	}
	else if (strName == "btnOther203") {
		strText += '=';
		edtFocus->SetCaption(strText.c_str());
	}
	else if (strName == "btnOther204") {
		strText += '\\';
		edtFocus->SetCaption(strText.c_str());
	}

	else if (strName == "btnOther301") // Third row special characters
	{
		strText += '{';
		edtFocus->SetCaption(strText.c_str());
	}
	else if (strName == "btnOther302") {
		strText += '}';
		edtFocus->SetCaption(strText.c_str());
	}
	else if (strName == "btnOther303") {
		strText += '[';
		edtFocus->SetCaption(strText.c_str());
	}
	else if (strName == "btnOther304") {
		strText += ']';
		edtFocus->SetCaption(strText.c_str());
	}

	else if (strName == "btnOther401") // Fourth row special characters
	{
		strText += ':';
		edtFocus->SetCaption(strText.c_str());
	}
	else if (strName == "btnOther402") {
		strText += '\"';
		edtFocus->SetCaption(strText.c_str());
	}
	else if (strName == "btnOther403") {
		strText += ';';
		edtFocus->SetCaption(strText.c_str());
	}
	else if (strName == "btnOther404") {
		strText += '\'';
		edtFocus->SetCaption(strText.c_str());
	}

	else if (strName == "btnOther501") // Fifth row special characters
	{
		strText += '<';
		edtFocus->SetCaption(strText.c_str());
	}
	else if (strName == "btnOther502") {
		strText += '>';
		edtFocus->SetCaption(strText.c_str());
	}
	else if (strName == "btnOther503") {
		strText += '?';
		edtFocus->SetCaption(strText.c_str());
	}
	else if (strName == "btnOther504") {
		strText += ',';
		edtFocus->SetCaption(strText.c_str());
	}
	else if (strName == "btnOther505") {
		strText += '.';
		edtFocus->SetCaption(strText.c_str());
	}
	else if (strName == "btnOther506") {
		strText += '/';
		edtFocus->SetCaption(strText.c_str());
	}

	else // After excluding all special characters, handle remaining letter/digit input
	{
		char cAdd = strName.at(strName.size() - 1);

		// Check if it is a digit or letter, steal input mode at that time
		if (('0' <= cAdd && cAdd <= '9')) {
			strText += cAdd;
			edtFocus->SetCaption(strText.c_str());
		}
		else if ('A' <= cAdd && cAdd <= 'Z') {
			if (chkShift->GetIsChecked()) {
				// Uppercase
				strText += cAdd;
			}
			else {
				// Lowercase
				strText += cAdd + 32;
			}
			edtFocus->SetCaption(strText.c_str());
		}
	}
}


// add by Philip.Wu  2006-06-07
// Edit box activation event, record the activated edit box
void CLoginScene::_evtAccountFocus(CGuiData* pSender) {
	auto edtTemp = dynamic_cast<CEdit*>(pSender);

	if (edtTemp) {
		edtFocus = edtTemp;
	}
}


void CLoginScene::ShowKeyboard(bool bShow) {
	frmKeyboard->SetIsShow(bShow);

	imgLogo1->SetIsShow(!bShow);
	imgLogo2->SetIsShow(!bShow);
}


void CLoginScene::ShowPathLogo(int isShow) {
	frmPathLogo->SetIsShow(isShow ? true : false);
}
