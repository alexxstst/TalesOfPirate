#include "stdafx.h"

#include "UIText.h"
#include "SelectChaScene.h"

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
#include "uiDoublePwdForm.h"

#include "UIMemo.h"
#include "cameractrl.h"

#include "Connection.h"
#include "ServerSet.h"
#include "GameAppMsg.h"


#include "UI3DCompent.h"
#include "UIForm.h"
#include "UITemplete.h"
#include "commfunc.h"
#include "uiboxform.h"
#include "CreateChaScene.h"
#include "loginscene.h"
#include "UIItemCommand.h"
#include "GuildData.h"
#include "UIChat.h"
#include "NetGuild.h"

using namespace std;


static constexpr auto cha_per_page{ 3 };


char autoLoginChar[32] ;
bool canAutoLoginChar = false;

//~ Constructors ==============================================================
CSelectChaScene::CSelectChaScene(stSceneInitParam& param)
	: CGameScene(param), m_isInit(false), m_isCreateCha(false),
	frmSelectCha(NULL), btnDel(NULL), btnYes(NULL), btnCreate(NULL), btnExit(NULL), btnChangePassConf(NULL)
{
	ToLogService("common", "CSelectChaScene Create");

	// distance between each char
    m_XPositions.push_back(2164);           
    m_XPositions.push_back(2410);           
    m_XPositions.push_back(2614);  

    m_YPositions.push_back(1657);
    m_YPositions.push_back(1389);
    m_YPositions.push_back(1579);

	m_ZPositions.push_back(0);
	m_ZPositions.push_back(0);
	m_ZPositions.push_back(0);

	// angle
    m_Yaws.push_back(140);
    m_Yaws.push_back(180);
    m_Yaws.push_back(220);
}

//~ Destructors ===============================================================
CSelectChaScene::~CSelectChaScene()
{
	ToLogService("common", "CSelectChaScene Destroy");
}

//~ Scene-related callbacks ===============================================================

//-----------------------------------------------------------------------
bool CSelectChaScene::_Init()
{
	if (!CGameScene::_Init())
	{
		// If parent _Init() fails, simply return false.
		return false;
	}


	{ // save loading res mt flag, and resume these flags in _Clear() before this scene destoried.
		lwIByteSet* res_bs = g_Render.GetInterfaceMgr()->res_mgr->GetByteSet();
		_loadtex_flag = res_bs->GetValue(OPT_RESMGR_LOADTEXTURE_MT);
		_loadmesh_flag = res_bs->GetValue(OPT_RESMGR_LOADMESH_MT);
		res_bs->SetValue(OPT_RESMGR_LOADTEXTURE_MT, 0);
		res_bs->SetValue(OPT_RESMGR_LOADMESH_MT, 0);
	}

	_bEnableCamDrag = TRUE;
	MPTimer tInit;
	tInit.Begin();

	// Init Once
	if (!m_isInit)
	{
		m_isInit = true;

		// Add background scene object
		pObj = AddSceneObj(512);

		if (pObj)
		{
			pObj->SetCullingFlag(0);
			// position of the scene
			pObj->setPos(0, 0);
			pObj->setYaw(180);

			DWORD num = pObj->GetPrimitiveNum();
			for (DWORD i = 0; i < num; i++)
			{
				pObj->GetPrimitive(i)->SetState(STATE_TRANSPARENT, 0);
				pObj->GetPrimitive(i)->SetState(STATE_UPDATETRANSPSTATE, 0);
			}

			{
				pObj->PlayDefaultAnimation(!g_stUISystem.m_sysProp.m_gameOption.bFramerate);

				const DWORD p_id_num = 6;
				DWORD p_id[p_id_num][5] =
				{
					{ 0, -1, -1, ANIM_CTRL_TYPE_BONE, PLAY_ONCE },
					{ 1, -1, -1, ANIM_CTRL_TYPE_BONE, PLAY_ONCE },
					{ 2, -1, -1, ANIM_CTRL_TYPE_BONE, PLAY_ONCE },
					{ 3, -1, -1, ANIM_CTRL_TYPE_BONE, PLAY_ONCE },
					{ 4, -1, -1, ANIM_CTRL_TYPE_BONE, PLAY_ONCE },
					{ 26, 0,  0, ANIM_CTRL_TYPE_TEXIMG, PLAY_LOOP },
				};

				lwIPrimitive* p = 0;

				lwPlayPoseInfo ppi;
				memset(&ppi, 0, sizeof(ppi));
				ppi.bit_mask = PPI_MASK_DEFAULT;
				ppi.pose = 0;
				ppi.frame = 0.0f;
				ppi.type = PLAY_ONCE;
				ppi.velocity = 0.5f;

				lwAnimCtrlObjTypeInfo type_info;

				for (DWORD i = 0; i < num; i++)
				{
					p = pObj->GetPrimitive(i);

					for (DWORD j = 0; j < p_id_num; j++)
					{
						if (p->GetID() == p_id[j][0])
						{
							lwIAnimCtrlAgent* anim_agent = p->GetAnimAgent();
							if (anim_agent == 0)
								continue;

							type_info.type = p_id[j][3];
							type_info.data[0] = p_id[j][1];
							type_info.data[1] = p_id[j][2];
							lwIAnimCtrlObj* ctrl_obj = anim_agent->GetAnimCtrlObj(&type_info);
							if (ctrl_obj == 0)
								continue;

							ppi.type = p_id[j][4];
							ctrl_obj->PlayPose(&ppi);
						}
					}
				}

			}

			{
				const DWORD id_num = 3;
				const DWORD id_buf[id_num] =
				{ 46, 47, 48 };

				lwIPrimitive* pri;
				DWORD pri_num = pObj->GetPrimitiveNum();

				for (DWORD j = 0; j < pri_num; j++)
				{
					pri = pObj->GetPrimitive(j);

					for (DWORD i = 0; i < id_num; i++)
					{
						if (pri->GetID() == id_buf[i])
						{
							pAure[i] = pri;
							pAure[i]->SetState(STATE_VISIBLE, 0);
						}
					}
				}
			}
		}
	}


	g_Render.SetClip(g_Config.fnear, g_Config.ffar);

	CCameraCtrl* pCam = g_pGameApp->GetMainCam();
	if (pCam)
	{
		g_pGameApp->EnableCameraFollow(TRUE);
        pCam->m_EyePos.x = 23.749f;
        pCam->m_EyePos.y = 20.923f;
        pCam->m_EyePos.z = 1.982f;

        pCam->m_RefPos.x = 20.034f;
        pCam->m_RefPos.y = -194.137f;
        pCam->m_RefPos.z = 0.868f;

	}
	g_Render.SetWorldViewFOV(Angle2Radian(70.0f));
	g_Render.SetWorldViewAspect(1.33f);
	g_Render.SetClip(1.0f, 2000.0f);

	g_Render.LookAt(pCam->m_EyePos, pCam->m_RefPos);
	g_Render.SetCurrentView(MPRender::VIEW_WORLD);
	MPIDeviceObject* dev_obj = g_Render.GetInterfaceMgr()->dev_obj;

	//SetupVertexFog(dev_obj, 0, 0, D3DCOLOR_XRGB(28, 221, 246), D3DFOG_EXP2, 1, 0.0006f);

	g_Render.SetRenderStateForced(D3DRS_LIGHTING, 0);
	g_Render.SetRenderState(D3DRS_AMBIENT, 0xffffffff);
	m_iCurPage = 0;
	//Initialize UI
	if (!_InitUI())
	{
		return false;
	}

	/* if(canAutoLoginChar){
		CS_BeginPlay( autoLoginChar );
		canAutoLoginChar = false;
	} */

	return true;
}

//-----------------------------------------------------------------------
bool CSelectChaScene::_Clear()
{
	if (frmSelectCha)
		frmSelectCha->SetIsShow(false);

	if (!CGameScene::_Clear())
	{
		// If Clear fails, simply return false.
		return false;
	}

	{ // reset loading res mt flag
		if (_loadtex_flag != 9 && _loadmesh_flag != 9)
		{
			lwIByteSet* res_bs = g_Render.GetInterfaceMgr()->res_mgr->GetByteSet();
			res_bs->SetValue(OPT_RESMGR_LOADTEXTURE_MT, _loadtex_flag);
			res_bs->SetValue(OPT_RESMGR_LOADMESH_MT, _loadmesh_flag);
		}
	}

	g_Render.SetClip(1.0f, 1000.0f);			//1000 ORIGINAL

	return true;

}
//-----------------------------------------------------------------------
void CSelectChaScene::_Render()
{
	//CGameScene::_Render();
/*
	if(pObj == 0)
		return;
*/
	MPTimer mpt;
	mpt.Begin();
	//CGameScene::_Render();
	MPIDeviceObject* dev_obj = g_Render.GetInterfaceMgr()->dev_obj;
	RenderStateMgr* rsm = g_pGameApp->GetRenderStateMgr();
	DWORD dwOldState;

	rsm->BeginScene();

	//Render characters
	rsm->BeginCharacter();


	D3DLIGHTX env_light;
	D3DLIGHTX env_light_old;
	memset(&env_light, 0, sizeof(env_light));
	env_light.Type = D3DLIGHT_DIRECTIONAL;

	env_light.Direction.x = -1.0f;
	env_light.Direction.y = -1.0f;
	env_light.Direction.z = -0.5f;
	D3DXVec3Normalize((D3DXVECTOR3*)&env_light.Direction, (D3DXVECTOR3*)&env_light.Direction);

	MPDwordByte4 c;
	c.b[3] = 0xff;
	c.b[2] = 185;
	c.b[1] = 36;
	c.b[0] = 54;
	env_light.Diffuse.r = (float)(c.b[2] / 255.0f);
	env_light.Diffuse.g = (float)(c.b[1] / 255.0f);
	env_light.Diffuse.b = (float)(c.b[0] / 255.0f);

	g_Render.GetLight(0, &env_light_old);
	g_Render.SetLight(0, &env_light);

	dev_obj->GetRenderState(D3DRS_LIGHTING, &dwOldState);
	/*for (int i(0); i<3; i++)
	{
		pAure[i]->SetState(STATE_VISIBLE, 0);
	}*/

	for (auto it = std::begin(m_CharactorPtrs); it != std::end(m_CharactorPtrs); ++it)
	{
		auto& chaFont = *it;
		if (!chaFont.pCha)
		{
			continue;
		}

		if (!chaFont.isOnCurPage)
		{
			continue;
		}

		dev_obj->SetRenderState(D3DRS_LIGHTING, 0);
		if (const auto it_index = std::distance(std::begin(m_CharactorPtrs), it);
			it_index == m_nCurChaIndex)
		{
			dev_obj->SetRenderState(D3DRS_LIGHTING, 1);
			//pAure[(*iter)->iPos]->SetState(STATE_VISIBLE, 1);
			const auto index = static_cast<int>(m_nCurChaIndex % cha_per_page);
			chaFont.pCha->setYaw(m_Yaws[index]);
		}

		chaFont.pCha->Render();
	}

	dev_obj->SetRenderState(D3DRS_LIGHTING, dwOldState);

	g_Render.SetLight(0, &env_light_old);

	rsm->EndCharacter();

	//Render scene objects
	rsm->BeginSceneObject();

	if (pObj)
		pObj->FrameMove(0);

	dev_obj->GetRenderState(D3DRS_LIGHTING, &dwOldState);
	dev_obj->SetRenderState(D3DRS_LIGHTING, FALSE);

	// orange background
	//SetupVertexFog(dev_obj, 0, 0, D3DCOLOR_XRGB(255, 104, 13), D3DFOG_EXP2, 1, 0.0025f);

	if (pObj)
		pObj->Render();

	rsm->EndSceneObject();

	rsm->BeginTranspObject();
	lwUpdateSceneTransparentObject();
	rsm->EndTranspObject();

	rsm->EndScene();

	dev_obj->SetRenderState(D3DRS_FOGENABLE, FALSE);

}

void CSelectChaScene::_RenderUI()
{
	for (auto& cha : m_CharactorPtrs)
	{
		if (!cha.pCha)
		{
			continue;
		}

		if (!cha.isOnCurPage)
		{
			continue;
		}

		if (cha.iFontX == -1 || cha.iFontY == -1)
		{
			const auto [nScreenX, nScreenY] = [&]
			{
				int nScreenX, nScreenY;

				lwMatrix44 mat;
				cha.pCha->GetRunTimeMatrix(&mat, 1);

				D3DXVECTOR3 pos(mat._41, mat._42, mat._43);

				g_Render.WorldToScreen(pos.x, pos.y, pos.z, &nScreenX, &nScreenY);

				nScreenY -= 80;
				return std::tuple{ nScreenX, nScreenY };
			}();
			cha.iFontX = nScreenX;
			cha.iFontY = nScreenY;
		}

		auto name = std::string(cha.pCha->getName());
		auto description = std::format("Lv{} {}", cha.iLevel, cha.sProfession);

		if (name.length() < description.length())
		{
			const auto blanks = (description.length() - name.length()) / 2;
			name = std::string(blanks, ' ') + name;
		}
		else
		{
			const auto blanks = (name.length() - description.length()) / 2;
			name = std::string(blanks, ' ') + name;
		}

		ui::TipRender((name + "\n" + description).c_str(),
			cha.iFontX, cha.iFontY);
	}
}

//-----------------------------------------------------------------------
void CSelectChaScene::_FrameMove(DWORD dwTimeParam)
{
	CGameScene::_FrameMove(dwTimeParam);
}

//-----------------------------------------------------------------------
bool CSelectChaScene::_MouseButtonDown(int nButton)
{
	// Ignore mouse clicks when password dialog is shown  add by Philip.Wu  2006-07-20
	if (g_stUIDoublePwd.GetIsShowCreateForm() || g_stUIDoublePwd.GetIsShowAlterForm() || g_stUIDoublePwd.GetIsShowDoublePwdForm())
	{
		return false;
	}
	//Check whether a character was clicked
	CCharacter* pCha = this->HitTestCharacter(g_pGameApp->GetMouseX(),
		g_pGameApp->GetMouseY());
	if (!pCha)
	{
		return false;
	}
	//Determine the clicked character position

	const auto index = [&]
	{
		auto it = std::find_if(std::begin(m_CharactorPtrs), std::end(m_CharactorPtrs),
			[&pCha](const ChaFont& cha)
			{
				return cha.pCha == pCha && cha.isOnCurPage;
			});

		return std::distance(std::begin(m_CharactorPtrs), it);
	}();

	if (index >= m_CharactorPtrs.size())
	{
		// The character selected was found out of bounds.
		return false;
	}

	if (m_nCurChaIndex != index && m_nCurChaIndex != static_cast<decltype(m_nCurChaIndex)>(-1))
	{
		m_CharactorPtrs[m_nCurChaIndex].pCha->PlayPose(1, PLAY_LOOP, -1,
			CGameApp::GetFrameFPS(), true);
		SetChaDark(m_CharactorPtrs[m_nCurChaIndex].pCha);
	}

	m_nCurChaIndex = index;


	m_CharactorPtrs[m_nCurChaIndex].pCha->PlayPose(2, PLAY_LOOP, -1,
		CGameApp::GetFrameFPS(), true);
	//m_CharactorPtrs[m_nCurChaIndex]->pCha->SetColor(m_ChaColors[m_nCurChaIndex][0],
	//    m_ChaColors[m_nCurChaIndex][1], m_ChaColors[m_nCurChaIndex][2]);
	m_CharactorPtrs[m_nCurChaIndex].pCha->SetColor(255, 255, 255);

	//Update button state on the panel
	UpdateButton();

	return true;

}

//-----------------------------------------------------------------------
bool CSelectChaScene::_MouseButtonDB(int nButton)
{
	if (!_MouseButtonDown(nButton))
		return false;

	SendBeginPlayToServer();
	g_pGameApp->Waiting();
	return true;
}


//-----------------------------------------------------------------------
void CSelectChaScene::_KeyDownEvent(int key)
{

	if (m_nCurChaIndex != static_cast<decltype(m_nCurChaIndex)>(-1))
	{ /*A character is selected*/      
		int iRotate = 0;		// left:-1	right:1
		if (VK_LEFT == key)
		{
			iRotate = -1;
		}
		else if (VK_RIGHT == key)
		{
			iRotate = 1;
		}

		const auto index = m_nCurChaIndex % cha_per_page;
		m_Yaws[index] += -(iRotate) * 15;
		m_Yaws[index] = (m_Yaws[index] + 360) % 360;
	}
}

//-----------------------------------------------------------------------
void CSelectChaScene::LoadingCall()          // Called during loading, refresh
{
	CGameScene::LoadingCall();

	// Each time entering the game through character selection, clear item COOLDOWN data
	CItemCommand::ClearCoolDown();

	// Extended guild display data; if not refreshed, client will not be notified; so preserve the previous guild data
	//CGuildData::SetGuildMasterID(NULL);
	//CGuildData::SetGuildName("");
	//if(g_stUIChat.GetGuildNode()) g_stUIChat.GetGuildNode()->Clear();

	NetPC_GUILD_START_BEGIN(0, 0, 0);
	NetPC_GUILD_START_END();

	static bool bLoadRes2 = false;
	if (!bLoadRes2)
	{
		bLoadRes2 = true;
		//g_pGameApp->LoadRes2();
		g_pGameApp->LoadRes3();
		//g_pGameApp->LoadRes4();
	}

	if (!g_Config.m_IsDoublePwd)
	{
		// Show create password dialog
		g_stUIDoublePwd.ShowCreateForm();

		//CBoxMgr::ShowSelectBox(_evtCreateDoublePwdEvent, GetLanguageString(800), true);//"\n\n?"
	}
	else if (GetChaCount() == 0 && frmWelcomeNotice)
	{
		// No characters currently, show welcome notice
		frmWelcomeNotice->ShowModal();
	}
	else if (m_isCreateCha)
	{
		m_isCreateCha = false;

		if (GetChaCount() == 1 && frmCreateOKNotice)
		{
			// Just created first character
			frmCreateOKNotice->ShowModal();
		}
	}

	if (g_dwCurMusicID != 1)	g_pGameApp->PlayMusic(1);

	if(canAutoLoginChar){
		int slot = atoi(autoLoginChar);
		CS_BeginPlay( slot );
		canAutoLoginChar = false;
	}
}


//-----------------------------------------------------------------------
void CSelectChaScene::SetMainCha(int nChaID)
{
	CGameScene::SetMainCha(nChaID);
}

//~ UI-related functions =============================================================

//-----------------------------------------------------------------------
bool CSelectChaScene::_InitUI()
{
	//Character selection scene form
	frmSelectCha = CFormMgr::s_Mgr.Find("frmSelect", GetInitParam()->nUITemplete);
	if (!frmSelectCha)		return false;

	btnDel = dynamic_cast<CTextButton*>(frmSelectCha->Find("btnDel"));
	if (!btnDel)            return false;
	btnYes = dynamic_cast<CTextButton*>(frmSelectCha->Find("btnYes"));
	if (!btnYes)            return false;
	btnCreate = dynamic_cast<CTextButton*>(frmSelectCha->Find("btnCreate"));
	if (!btnCreate)         return false;
	btnExit = dynamic_cast<CTextButton*>(frmSelectCha->Find("btnNo"));
	if (!btnExit)            return false;
	btnAlter = dynamic_cast<CTextButton*>(frmSelectCha->Find("btnAlter"));
	if (!btnAlter)           return false;
	btnPrevPage = dynamic_cast<CTextButton*>(frmSelectCha->Find("btnPrevPage"));
	if (!btnPrevPage)           return false;
	btnNextPage = dynamic_cast<CTextButton*>(frmSelectCha->Find("btnNextPage"));
	if (!btnNextPage)           return false;
	labPage = dynamic_cast<CLabel*>(frmSelectCha->Find("labPage"));
	if (!labPage)           return false;
	labPage->SetCaption("1/2");

	CForm* frmUpdPas = CFormMgr::s_Mgr.Find("frmUpdPas");
	btnChangePassConf = dynamic_cast<CTextButton*>(frmUpdPas->Find("btnChangePassConf"));
	btnChangePassConf->evtMouseClick = _OnClickUpdatePass;
	btnChangePassConf->SetIsEnabled(true);
	CEdit* edtPassword = dynamic_cast<CEdit*>(frmUpdPas->Find("edtPassword"));
	CEdit* edtPassword2 = dynamic_cast<CEdit*>(frmUpdPas->Find("edtPassword2"));
	CEdit* confirmPIN = dynamic_cast<CEdit*>(frmUpdPas->Find("confirmPIN"));

	edtPassword->SetIsPassWord(true);
	edtPassword2->SetIsPassWord(true);
	confirmPIN->SetIsPassWord(true);

	// Set create button flashing
	btnCreate->SetFlashCycle();

	frmSelectCha->SetPos(
		(g_pGameApp->GetWindowWidth() - frmSelectCha->GetWidth()) / 2,
		g_pGameApp->GetWindowHeight() - frmSelectCha->GetHeight() - 20);
	frmSelectCha->Refresh();
	frmSelectCha->Show();

	frmSelectCha->evtEntrustMouseEvent = _SelChaFrmMouseEvent;

	// Welcome notice form   Shown when current account has no characters
	frmWelcomeNotice = CFormMgr::s_Mgr.Find("frmWelcomeNotice");
	if (!frmWelcomeNotice)		return false;
	frmWelcomeNotice->evtEntrustMouseEvent = _evtWelcomeNoticeEvent;

	// First character creation success notice   Shown after first character is created for this account
	frmCreateOKNotice = CFormMgr::s_Mgr.Find("frmCreateOKNotice");
	if (!frmCreateOKNotice)		return false;
	frmCreateOKNotice->evtEntrustMouseEvent = _evtCreateOKNoticeEvent;

	// Update button enabled state
	UpdateButton();

	frmChaNameAlter = CFormMgr::s_Mgr.Find("frmChaNameAlter");
	if (!frmChaNameAlter)       return false;
	frmChaNameAlter->evtEntrustMouseEvent = _evtChaNameAlterMouseEvent;

	return true;
}

void CSelectChaScene::_OnClickUpdatePass(CGuiData* pSender, int x, int y, DWORD key) {
	CForm* frmUpdPas = CFormMgr::s_Mgr.Find("frmUpdPas");

	CEdit* edtPassword = dynamic_cast<CEdit*>(frmUpdPas->Find("edtPassword"));
	CEdit* edtPassword2 = dynamic_cast<CEdit*>(frmUpdPas->Find("edtPassword2"));
	CEdit* confirmPIN = dynamic_cast<CEdit*>(frmUpdPas->Find("confirmPIN"));

	if (strlen(edtPassword->GetCaption()) == 0 || strlen(edtPassword2->GetCaption()) == 0 || strlen(confirmPIN->GetCaption()) == 0) {
		NetShowMapCrash("Please fill in all fields.");
		return;
	}

	if (strcmp(edtPassword->GetCaption(), edtPassword2->GetCaption()) != 0) {
		NetShowMapCrash("Passwords do not match.");
		return;
	}

	CS_ChangePass(edtPassword->GetCaption(), confirmPIN->GetCaption());

}

//-----------------------------------------------------------------------
void CSelectChaScene::_SelChaFrmMouseEvent(CCompent* pSender, int nMsgType,
	int x, int y, DWORD dwKey)
{
	string strName = pSender->GetName();

	if (stricmp("frmSelect", pSender->GetForm()->GetName()) != 0)
	{
		return;
	}

	if (strName == "btnCreate")
	{
		//Switch to character creation scene
		stSceneInitParam param;
		param.nTypeID = enumCreateChaScene;
		param.strName = "";
		param.strMapFile = "";
		param.nUITemplete = enumCreateChaForm;
		param.nMaxCha = 20;
		param.nMaxObj = 20;
		param.nMaxItem = 20;
		param.nMaxEff = 20;

		CCreateChaScene* pkScene =
			dynamic_cast<CCreateChaScene*>(g_pGameApp->CreateScene(&param));
		if (!pkScene) return;
		CSelectChaScene& rkSelectChaScene = CSelectChaScene::GetCurrScene();

		g_pGameApp->GotoScene(pkScene, false);
		pkScene->setLastScene(&rkSelectChaScene);
	}
	else if (strName == "btnYes")
	{
		//Enter game
		//Send begin play message to server
		GetCurrScene().SendBeginPlayToServer();
		CGameApp::Waiting();
	}
	else if (strName == "btnDel")
	{
		if (g_Config.m_IsDoublePwd)
		{
			// Deleting character requires password  modify by Philip.Wu  2006-07-19
			g_stUIDoublePwd.SetType(CDoublePwdMgr::DELETE_CHARACTOR);
			g_stUIDoublePwd.ShowDoublePwdForm();
		}
		else
		{
			// Delete account
			//CBoxMgr::ShowSelectBox(_CheckFrmMouseEvent, GetLanguageString(384), true);
		}
	}
	else if (strName == "btnNo")
	{

		// Exit character selection scene
		CS_Logout();
		CS_Disconnect(DS_DISCONN);
		g_pGameApp->LoadScriptScene(enumLoginScene);
	}
	else if (strName == "btnAlter")
	{
		// Update double password
		g_stUIDoublePwd.ShowAlterForm();

	}
	else if (strName == "btnChangePass")
	{

		CForm* frmUpdPas = CFormMgr::s_Mgr.Find("frmUpdPas");
		if (frmUpdPas) {
			frmUpdPas->Show();
		}



	}
	else if (strName == "btnPrevPage")
	{

		auto createChaScene = static_cast<CSelectChaScene*>(CGameApp::GetCurScene());
		if (createChaScene->m_iCurPage <= 0)
		{
			return;
		}
		--createChaScene->m_iCurPage;
		createChaScene->UpdatePageLabel();

		createChaScene->UpdateCharacterPositions();

	}
	else if (strName == "btnNextPage")
	{

		auto createChaScene = static_cast<CSelectChaScene*>(CGameApp::GetCurScene());
		if (createChaScene->m_iCurPage + 1 >= createChaScene->GetMaxPage())
		{
			return;
		}
		++createChaScene->m_iCurPage;
		createChaScene->UpdatePageLabel();
		createChaScene->UpdateCharacterPositions();
	}
	//else if(strName == "btnChangePassConf")
	//{
	//	//send packet etc
	//}
	//else if(strName == "btnCancel")
	//{
	//	CForm* frmUpdPas = CFormMgr::s_Mgr.Find( "frmUpdPas");
	//	if(frmUpdPas){
	//		frmUpdPas->Hide();
	//	}
	//}

	return;
}

//-----------------------------------------------------------------------
// Account verification callback
//void CSelectChaScene::_CheckFrmMouseEvent(CCompent *pSender, int nMsgType, 
//                                          int x, int y, DWORD dwKey)
//{
//    if( nMsgType == CForm::mrYes ) 
//    {
//        // Send delete character message to server
//        GetCurrScene().SendDelChaToServer();
//        CGameApp::Waiting();
//        return;
//    }
//    return;
//}


// Ask whether to create double password  add by Philip.Wu  2006-07-20
void CSelectChaScene::_evtCreateDoublePwdEvent(CCompent* pSender, int nMsgType, int x, int y, DWORD dwKey)
{
	if (nMsgType == CForm::mrYes)
	{
		// Show create password dialog
		g_stUIDoublePwd.ShowCreateForm();
	}
	else
	{
		// Exit character selection scene
		CS_Logout();
		CS_Disconnect(DS_DISCONN);
		g_pGameApp->LoadScriptScene(enumLoginScene);
	}
}



//~ Logic-related functions ==========================================================

//-----------------------------------------------------------------------
void CSelectChaScene::DelCurrentSelCha()
{
	//Remove character from scene
	m_CharactorPtrs[m_nCurChaIndex].pCha->SetValid(false);


	//Clear the slot
	m_CharactorPtrs[m_nCurChaIndex].pCha = nullptr;

	m_FreePositions[m_nCurChaIndex] = 0;    //Mark this slot as empty

	m_nCurChaIndex = static_cast<decltype(m_nCurChaIndex)>(-1);
	UpdateCharacterPositions();

	//Update UI buttons
	UpdateButton();
	return;
}


//-----------------------------------------------------------------------
bool CSelectChaScene::CreateCha(const string& sName, int nChaIndex, stNetChangeChaPart* part)
{
	if (m_nCurChaIndex < m_CharactorPtrs.size())
	{
		SetChaDark(m_CharactorPtrs[m_nCurChaIndex].pCha);
	}

	CCharacter* pCha = this->AddCharacter(part->sTypeID);
	if (!pCha) return false;
	pCha->setName(sName.c_str());
	pCha->UpdataFace(*part);
	pCha->SetScale(pCha->GetScale() * 1.0f);

	//Find next available slot

	const auto it = std::find(std::cbegin(m_FreePositions), std::cend(m_FreePositions), 0);
	if (it == std::cend(m_FreePositions))
	{
		return false;
	}
	m_nCurChaIndex = std::distance(std::cbegin(m_FreePositions), it);
	//pCha->setPos(m_XPositions[i], m_YPositions[i], m_ZPositions[i]);
	//pCha->setYaw(m_Yaws[i]);
	//m_FreePositions[i] = 1;
	//pCha->GetColor(m_ChaColors[i]);
	//SetChaDark(pCha);

	if (m_nCurChaIndex >= m_CharactorPtrs.size())
	{
		m_CharactorPtrs.emplace_back();
	}

	auto& chaFont = m_CharactorPtrs.back();

	chaFont.pCha = pCha;
	chaFont.iLevel = 1;
	chaFont.sProfession = GetLanguageString(385);
	chaFont.iPos = m_nCurChaIndex;
	chaFont.iFontX = -1;
	chaFont.iFontY = -1;
	m_isCreateCha = true;
	UpdateCharacterPositions();
	m_FreePositions[m_nCurChaIndex] = 1;
	UpdateButton();
	return true;
}

//-----------------------------------------------------------------------
void CSelectChaScene::SendDelChaToServer(const char szPassword2[])
{
	if (m_nCurChaIndex < m_MaxCharacters)
	{
		CS_DelCha(m_nCurChaIndex, szPassword2);
	}
}

//-----------------------------------------------------------------------
void CSelectChaScene::SendBeginPlayToServer()
{
	if (m_nCurChaIndex >= m_CharactorPtrs.size())
	{
		return;
	}

	const auto& chaFont = m_CharactorPtrs[m_nCurChaIndex];
	if (!chaFont.pCha)  return;

	CS_BeginPlay(m_nCurChaIndex);

	CCharacter* pCha = chaFont.pCha;

	//Debug output
	ToLogService("ui", "Client Send:{},{},{},{},{},{}", pCha->getName(), pCha->GetPartID(0), pCha->GetPartID(1),
		pCha->GetPartID(2), pCha->GetPartID(3), pCha->GetPartID(4));
}

bool CSelectChaScene::SelectCharacters(std::span<const NetChaBehave> characters)
{
	std::for_each_n(_pChaArray, _nChaCnt,
		[](CCharacter& cha)
		{
			if (cha.IsValid())
			{
				cha.SetValid(false);
			}
		}
	);


	m_FreePositions.assign(m_MaxCharacters, 0);

	m_CharactorPtrs.clear();
	m_CharactorPtrs.reserve(characters.size());

	for (auto i = 0; i < characters.size(); ++i)
	{
		auto& chaFont = m_CharactorPtrs.emplace_back();
		const auto& chaBehave = characters[i];

		chaFont.pCha = AddCharacter(chaBehave.look_minimal.typeID);
		if (!chaFont.pCha)
		{
			g_pGameApp->MsgBox("Could not create character, please contact the developers!");
			return false;
		}

		m_FreePositions[i] = 1;

		chaFont.pCha->SetScale(chaFont.pCha->GetScale() * 1.0f);
		chaFont.pCha->setName(chaBehave.sCharName);

		stNetChangeChaPart part{};
		part.sTypeID = chaBehave.look_minimal.typeID;
		
		//TODO(Ogge): static_asserting the size of SLink and equipIDs would be nice
		for (auto i = 0; i < std::size(part.SLink); ++i)
		{
			part.SLink[i].sID = chaBehave.look_minimal.equip_IDs[i];
		}

		chaFont.pCha->UpdataFace(part);
		chaFont.pCha->SetIsOnMount(false);
		chaFont.pCha->PlayPose(chaFont.pCha->GetPose(POSE_WAITING), PLAY_LOOP_SMOOTH);

		chaFont.iLevel = chaBehave.iDegree;
		chaFont.sProfession = chaBehave.sJob;
		chaFont.iPos = i;
		chaFont.iFontX = -1;
		chaFont.iFontY = -1;
		chaFont.isOnCurPage = false;

		/*
		if (m_CharactorPtrs[i]->iLevel >= highestLevel) highestLevel = m_CharactorPtrs[i]->iLevel;
		UpdateCharacterPositions();
		BYTE colors[3];
		pCha->GetColor(colors);
		// Following code is useless for now (colors are always 255,255,255 when highlighted):
		if (colors[0] != 129 && colors[1] != 121 && colors[2] != 114) {
			pCha->GetColor(m_ChaColors[i]); // Saves character "color" before it is set to dark (only if current color is not dark already (129, 121, 114)
		}
		if (i != m_nCurChaIndex)
			SetChaDark(pCha);
		*/
	}
	UpdateCharacterPositions();
	UpdateButton();

	return true;
}

void CSelectChaScene::UpdateCharacterPositions()
{
	std::stable_sort(m_CharactorPtrs.begin(), m_CharactorPtrs.end(),
		[](const ChaFont& a, const ChaFont& b) {
			return static_cast<bool>(a.pCha) > static_cast<bool>(b.pCha);
		});


	for (auto i = 0; i < m_CharactorPtrs.size(); ++i)
	{
		auto& cha = m_CharactorPtrs[i];
		const auto page = static_cast<int>(i / cha_per_page);
		const auto active_page = m_iCurPage == page;

		if (!active_page)
		{
			cha.isOnCurPage = false;
			if (cha.pCha)
			{
				cha.pCha->SetHide(true);
			}
			continue;
		}

		cha.isOnCurPage = true;
		cha.iFontX = -1;
		cha.iFontY = -1;
		m_FreePositions[i] = 0;
		if (cha.pCha)
		{
			cha.pCha->SetHide(false);
			m_FreePositions[i] = 1;

			const auto posIndex = i % cha_per_page;
			cha.pCha->setPos(m_XPositions[posIndex], m_YPositions[posIndex]);
			cha.pCha->setHeightOff(m_ZPositions[posIndex]);
			cha.pCha->setYaw(m_Yaws[posIndex]);
		}
	}
}

void CSelectChaScene::UpdatePageLabel()
{
	if (!labPage)
	{
		return;
	}

	const auto page = m_iCurPage + 1;
	const auto s = std::format("{}/{}", page, GetMaxPage());
	labPage->SetCaption(s.c_str());
}

int CSelectChaScene::GetMaxPage() const
{
	return m_MaxCharacters / cha_per_page;
}

//-----------------------------------------------------------------------
CSelectChaScene& CSelectChaScene::GetCurrScene()
{
	CSelectChaScene* pScene =
		dynamic_cast<CSelectChaScene*>(g_pGameApp->GetCurScene());

	if (!pScene) NULL;

	return *pScene;
}

void CSelectChaScene::SelectChaError(int error_no, const char* error_info)
{
	g_pGameApp->MsgBox("%s", g_GetServerError(error_no));
	ToLogService("errors", LogLevel::Error, "{} Error, Code:{}, Info: {}", error_info, error_no, g_GetServerError(error_no));
	CGameApp::Waiting(false);
}
void CSelectChaScene::SetChaDark(CCharacter* pCha)
{
	pCha->SetColor(129, 121, 114);
}

void CSelectChaScene::UpdateButton()
{
	UpdatePageLabel();
	btnCreate->SetIsEnabled(m_CharactorPtrs.size() < m_MaxCharacters);

	{
		const auto v = m_nCurChaIndex < m_CharactorPtrs.size();
		btnDel->SetIsEnabled(v);
		btnYes->SetIsEnabled(v);
	}

	if (!g_Config.m_IsDoublePwd)
	{
		btnCreate->SetIsEnabled(false);
		btnAlter->SetIsEnabled(false);
	}
	else
	{
		btnAlter->SetIsEnabled(true);
	}
}

int CSelectChaScene::GetChaCount()
{
	constexpr auto occupied_slot_value{ 1 };
	return std::ranges::count(m_FreePositions, occupied_slot_value);
}


void CSelectChaScene::ShowWelcomeNotice(bool bShow)
{
	if (frmWelcomeNotice)
	{
		frmWelcomeNotice->ShowModal();
	}
}


// Welcome notice event handler
void CSelectChaScene::_evtWelcomeNoticeEvent(CCompent* pSender, int nMsgType, int x, int y, DWORD dwKey)
{
	string strName = pSender->GetName();
	CSelectChaScene* pSelectChaScene = dynamic_cast<CSelectChaScene*>(g_pGameApp->GetCurScene());

	if (pSelectChaScene)
	{
		if (strName == "btnYes")
		{
			pSelectChaScene->frmWelcomeNotice->Close();
		}
	}
}


// First character creation success notice event handler
void CSelectChaScene::_evtCreateOKNoticeEvent(CCompent* pSender, int nMsgType, int x, int y, DWORD dwKey)
{
	string strName = pSender->GetName();
	CSelectChaScene* pSelectChaScene = dynamic_cast<CSelectChaScene*>(g_pGameApp->GetCurScene());

	if (pSelectChaScene)
	{
		if (strName == "btnYes")
		{
			pSelectChaScene->frmCreateOKNotice->Close();
		}
	}
}

// Character name alter event handler
void CSelectChaScene::_evtChaNameAlterMouseEvent(CCompent* pSender, int nMsgType, int x, int y, DWORD dwKey)
{
	string strName = pSender->GetName();
	CSelectChaScene* pSelectChaScene = dynamic_cast<CSelectChaScene*>(g_pGameApp->GetCurScene());

	if (pSelectChaScene)
	{
		if (strName == "btnYes")
		{
			pSelectChaScene->frmCreateOKNotice->Close();
		}
	}
}
