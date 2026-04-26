#include "Stdafx.h"

#include "MPEditor.h"
#include "GameApp.h"
#include "GameConfig.h"
#include "UIFormMgr.h"
#include "GameAppMsg.h"
#include "GlobalVar.h"
#include "PacketCmd.h"
#include "Lua_Platform.h"
#include "DrawPointList.h"
#include "UIHeadSay.h"
#include "UIEditor.h"
#include "teaminviteformmgr.h"
#include "InputBox.h"
#include "uistartform.h"
#include "ui3dcompent.h"
#include "UIsystemform.h"
#include "EffectObj.h"
#include "chastate.h"
#include "cameractrl.h"
#include "Smallmap.h"
#include "TestDemo.h"

#ifdef TESTDEMO

void CGameApp::HandleKeyContinue() {
	if (_pConsole->IsVisible()) return;

	HandleContinueSuperKey();
}

void CGameApp::MouseButtonDown(int nButton) {

#ifdef _LUA_GAME
lua_platform_mousedown (nButton);
#endif

#ifdef APP_DEBUG
try
	{
#endif
if(!_IsSceneOk()) return;

		if(g_Editor.IsEnable()) 
		{
			if( CFormMgr::IsMouseInGui() ) return;

			g_Editor.MouseButtonDown(nButton);
		}
		else
		{      		
			if( CFormMgr::IsMouseInGui() ) return;

			g_stUIStart.CheckMouseDown( GetMouseX(), GetMouseY() );

			if( !_MouseInScene ) return;

			GetCurScene()->_MouseButtonDown( nButton );
			_stCursorMgr.MouseDown( nButton );
		}
#ifdef APP_DEBUG
}
	catch(...)
	{
		MessageBox( 0, "CGameApp MouseButtonDown exception!", "APP_DEBUG", 0 );
	}
#endif
}

void CGameApp::MouseButtonUp(int nButton) {
	_dwMouseDownTime[nButton] = 0;

	if (_IsSceneOk()) {
		if (CFormMgr::IsMouseInGui()) return;

		if (g_Editor.IsEnable()) {
			g_Editor.MouseButtonUp(nButton);
		}
		else {
			GetCurScene()->_MouseButtonUp(nButton);

			_stCursorMgr.MouseUp(nButton);
		}
	}
}

void CGameApp::MouseContinue(int nButton) {
	_dwMouseDownTime[nButton]++;
}

void CGameApp::MouseMove(int nOffsetX, int nOffsetY) {

#ifdef APP_DEBUG
try
	{
#endif
if(_IsSceneOk()) {
	if (IsMouseButtonPress(1)) {
		if (!IsCameraFollow()) {
			if (GetCurScene()->IsEnableCamDrag()) {
				float fSpeed = 0.1f;
				GetMainCam()->MoveRight(fSpeed * (float)nOffsetX, TRUE);
				GetMainCam()->MoveForward(fSpeed * (float)nOffsetY, TRUE);
			}
		}
		else {
			//lemon add
			CCharacter* pCha = CGameScene::GetMainCha();
			if (pCha) {
				GetMainCam()->RotationCameraLR((float)nOffsetX / 300);
			}
		}
	}

	if (CFormMgr::IsMouseInGui()) return;

	if (g_Editor.IsEnable()) {
		g_Editor.MouseMove(nOffsetX, nOffsetY);
	}
	GetCurScene()->_MouseMove(nOffsetX, nOffsetY);
}
#ifdef APP_DEBUG
}
	catch(...)
	{
		MessageBox( 0, "CGameApp MouseMove exception!", "APP_DEBUG", 0 );
	}
#endif
}


void CGameApp::MouseButtonDB(int nButton) {

#ifdef APP_DEBUG
try
	{
#endif
//OutputDebugString("ok");
if(_IsSceneOk()) {
	if (CFormMgr::IsMouseInGui()) return;

	GetCurScene()->_MouseButtonDB(nButton);

	//
	if ((nButton == 1) && (!GlobalAppConfig.IsEditor()) && (!_pMainCam->IsDefaultView())) {
		//_pMainCam->ResetCamera();
		ResetCamera();
	}

	//OutputDebugString("ok");
	//float f = sqrt((float)35);
	//LG("LOG","msgok%f",f);
	//CCameraCtrl *pCam = g_pGameApp->GetMainCam();
	//CCharacter *pCha = GetMainCha();
	//if(pCha)
	//{
	//	D3DXVECTOR3 vecCha = pCha->GetPos();

	//	g_pGameApp->Get ->Reset(vecCha.x,vecCha.y,vecCha.z);
	//	pCam->SetFollowObj(vecCha);
	//	pCam->FrameMove(0);
	//	g_Render.SetWorldViewFOV(Angle2Radian(pCam->m_ffov));
	//	g_Render.LookAt(pCam->m_EyePos, pCam->m_RefPos);
	//	g_Render.SetCurrentView(MPRender::VIEW_WORLD);
	//}
}
#ifdef APP_DEBUG
}
	catch(...)
	{
		MessageBox( 0, "CGameApp MouseButtonDB exception!", "APP_DEBUG", 0 );
	}
#endif
}

void CGameApp::MouseScroll(int nScroll) {

#ifdef APP_DEBUG
try
	{
#endif
if(_IsSceneOk()) {
	ihei += nScroll > 0 ? 1 : -1;
	if (CFormMgr::IsMouseInGui()) {
		CFormMgr::s_Mgr.MouseScroll(nScroll);
		return;
	}

	if (g_Editor.IsEnable()) {
		g_Editor.MouseScroll(nScroll);
	}
	else {
		GetCurScene()->_MouseScroll(nScroll);
	}

	if (!(g_Editor.IsEnable() && g_Editor.m_nSelTypeID)) {
		CCameraCtrl* pCam = g_pGameApp->GetMainCam();
		int f = nScroll<0 ? 30 : -30;
		pCam->Scale((float)f * 0.005f);
		pCam->MoveForwardBack((float)f * 0.001f);
	}
}
#ifdef APP_DEBUG
}
	catch(...)
	{
		MessageBox( 0, "CGameApp MouseScroll exception!", "APP_DEBUG", 0 );
	}
#endif
}
void CGameApp::HandleSuperKey() {
	if (!IsEnableSuperKey()) return;

#ifdef APP_DEBUG
try
	{
#endif

if(IsKeyDown(DIK_TAB)&& !GlobalAppConfig.IsEditor())
		{
			if(CGameScene::_pLargerMap)
				CGameScene::_pLargerMap->Show( !CGameScene::_pLargerMap->IsShow() );
		}
		if(g_pGameApp->IsKeyDown (DIK_C))
		{
			//GetCurScene()->SetMainCha(GetCurScene()->GetMainCha()->getID());

			if( CGameScene::GetMainCha() )
			{
				_bCameraFollow = !_bCameraFollow;
				SetCameraPos(CGameScene::GetMainCha()->GetPos());
			}
			//if(_bCameraFollow)
			//{
			//	CCameraCtrl *pCam = GetMainCam();
			//	CCharacter *pCha = GetCurScene()->GetMainCha();
			//	D3DXVECTOR3 vecCha = pCha->GetPos();

			//	pCam->InitBuf(vecCha.x,vecCha.y,vecCha.z);
			//	pCam->SetBufVel( pCha->getMoveSpeed() ,pCha->getID());

			//	g_pGameApp->ResetGameCamera( pCha->IsBoat() ? 1 : 0 );
			//	pCam->FrameMove(0);

			//	g_Render.SetWorldViewFOV(Angle2Radian(pCam->m_ffov));
			//	g_Render.LookAt(pCam->m_EyePos, pCam->m_RefPos);
			//	g_Render.SetCurrentView(MPRender::VIEW_WORLD);
			//	//g_pGameApp->ResetGameCamera( GetCurScene()->GetMainCha()->IsBoat() ? 1 : 0 );
			//}

			TipI(_bCameraFollow, GetLanguageString(106).c_str(), GetLanguageString(107).c_str());
		}
		else if(g_pGameApp->IsKeyDown (DIK_INSERT) && g_pGameApp->IsCtrlPress())
		{
			g_Render.EnableCaptureAVI(1 - g_Render.IsEnableCaptureAVI());
		}
		else if(g_pGameApp->IsKeyDown (DIK_F3) && GlobalAppConfig.IsEditor() && g_pGameApp->IsCtrlPress())
		{
			//  by lh test
			static int type = 0;
			type = GetCurScene()->GetSceneTypeID();
			type++;
			if( type >= enumSceneEnd ) type=0;

			g_pGameApp->LoadScriptScene( (eSceneType) type );
		}
#ifdef _DEBUG
else if (g_pGameApp->IsKeyDown (DIK_F4) && g_pGameApp->IsCtrlPress())
		{
			CFormMgr::s_Mgr.SetEnabled( !CFormMgr::s_Mgr.GetEnabled() );		
		}
#endif
else if (g_pGameApp->IsKeyDown (DIK_F10) )
		{
			CGameScene *pScene = GetCurScene();
			if(pScene)
			{
				// 
				FILE *fp = fopen("monster.txt", "wt");
				for(int i = 0; i < pScene->GetChaCnt(); i++)
				{
					CCharacter *pCha = pScene->GetCha(i);
					if(pCha->IsValid())
					{
						if (pCha!=pScene->GetMainCha())
						{
							fprintf(fp, "CreateCha(%d, %d, %d, %d, %d)\n", pCha->getTypeID(), pCha->GetCurX(), pCha->GetCurY(), pCha->getYaw(), 1200);
						}
					}
				}
				fclose(fp);
			}
		}
#ifdef _LOG_NAME_
else if(g_pGameApp->IsCtrlPress())
		{
			if( g_pGameApp->IsKeyDown(DIK_Y) )
			{
				_pDrawPoints->Clear();
			}
			else if( g_pGameApp->IsKeyDown(DIK_N) )
			{
				CCharacter::IsShowLogName=!CCharacter::IsShowLogName;
			}
			else if(g_pGameApp->IsKeyDown(DIK_L)) 
			{
				g_pGameApp->GetDrawPoints()->SetIsEnabled( !g_pGameApp->GetDrawPoints()->GetIsEnabled() );
				g_pGameApp->AddTipText( "%s", GetLanguageString(108).c_str() );
			}
			else if( g_pGameApp->IsKeyDown(DIK_T) )
			{
				g_pGameApp->SetIsRenderTipText( !g_pGameApp->GetIsRenderTipText() );
				g_pGameApp->AddTipText( "%s", GetLanguageString(109).c_str() );
			}
		}
#endif

if(_IsSceneOk()) GetCurScene()->_HandleSuperKey();
#ifdef APP_DEBUG
}
	catch(...)
	{
		MessageBox( 0, "CGameApp HandleSuperKey exception!", "APP_DEBUG", 0 );
	}
#endif
}
//pTerr->GetWidth()
void CGameApp::HandleContinueSuperKey() {
	CCameraCtrl* pCam = g_pGameApp->GetMainCam();
	CCharacter* pCha;
	if (GetCurScene())
		pCha = GetCurScene()->GetMainCha();
	if (IsKeyContinue(DIK_LSHIFT) || IsKeyContinue(DIK_RSHIFT)) {
		if (IsKeyContinue(DIK_UP))
			pCam->MoveForwardBack((float)30 * 0.001f);
		if (IsKeyContinue(DIK_DOWN))
			pCam->MoveForwardBack((float)-30 * 0.001f);

		if (IsKeyContinue(DIK_LEFT))
			pCam->RotationCameraLR(0.03f);
		if (IsKeyContinue(DIK_RIGHT))
			pCam->RotationCameraLR(-0.03f);
	}
	//CGameScene* pScene = GetCurScene();
	//if(pScene)
	//{
	//	CWorldScene* pWorld = (CWorldScene*)pScene;

	//	if(IsKeyContinue(DIK_W))
	//	{
	//		pWorld->GetMouseDown()->ActMove(pScene->GetMainCha(),100,100,false,true));

	//			//pCam->MoveForwardBack((float)-30 * 0.001f);
	//	}
	//	if(IsKeyContinue(DIK_S))
	//	{
	//		//pCam->MoveForwardBack((float)30 * 0.001f);
	//	}

	//}
	if (!IsEnableSuperKey()) {
		return;
	}
	// Continue Key Events Handle Routines...
	if (IsKeyContinue(DIK_W)) {
		MPVector3 pos;
		MPVector3 dir;
		CCharacter* cha = this->GetCurScene()->GetMainCha();
		pos = cha->GetPos();
		cha->GetFaceDir(&dir);
		pos += dir * 2.0f;
		((CWorldScene*)GetCurScene())->GetMouseDown().ActMove(cha, int(pos.x * 100), (int)(pos.y * 100), 0, 1);
	}
	else if (IsKeyContinue(DIK_S)) {
		MPVector3 pos;
		MPVector3 dir;
		CCharacter* cha = this->GetCurScene()->GetMainCha();
		pos = cha->GetPos();
		cha->GetFaceDir(&dir);
		pos -= dir * 2.0f;
		//cha->ForceMove(pos.x * 100, pos.y * 100);
		((CWorldScene*)GetCurScene())->GetMouseDown().ActBackMove(cha, (int)(pos.x * 100), (int)(pos.y * 100), 0);
	}
	if (IsKeyContinue(DIK_A)) {
		pCam->RotationCameraLR(0.03f);
		if (pCha)
			pCha->setYaw(int(pCam->m_fAngle * 57.29577f));
	}
	if (IsKeyContinue(DIK_D)) {
		pCam->RotationCameraLR(-0.03f);
		if (pCha)
			pCha->setYaw(int(pCam->m_fAngle * 57.29577f));
	}

	if (IsKeyContinue(DIK_E)) {
		GetMainCam()->MoveForward(0.3f, TRUE);
	}

	//if (IsKeyContinue(DIK_D))
	//{
	//	GetMainCam()->MoveForward(-0.3f, TRUE);  
	//}

	if (IsKeyContinue(DIK_S)) {
		float fSpeed = 0.20f;
		GetMainCam()->MoveRight(fSpeed, TRUE);
	}

	if (IsKeyContinue(DIK_F)) {
		float fSpeed = -0.20f;
		GetMainCam()->MoveRight(fSpeed, TRUE);
	}
	if (IsKeyContinue(DIK_Q)) {
		btest = !btest;
	}

	if (IsKeyContinue(DIK_LEFT)) {
		if (IsCameraFollow()) {
			CCharacter* pCha = _pCurScene->GetMainCha();
			if (pCha) {
				float fX = (float)pCha->GetCurX() / 100.0f;
				float fY = (float)pCha->GetCurY() / 100.0f;

				VECTOR3 vecPos = pCha->GetPos(); //  VECTOR3(fX, fY, );
				GetMainCam()->Turn(0.4f, &vecPos);
			}
		}
		else {
			VECTOR3 vecPos = GetMainCam()->m_RefPos;
			GetMainCam()->Turn(0.4f, &vecPos);
		}
	}

	if (IsKeyContinue(DIK_RIGHT)) {
		if (IsCameraFollow()) {
			CCharacter* pCha = _pCurScene->GetMainCha();
			if (pCha) {
				float fX = (float)pCha->GetCurX() / 100.0f;
				float fY = (float)pCha->GetCurY() / 100.0f;
				// VECTOR3 vecPos = VECTOR3(fX, fY, 0.0f);
				VECTOR3 vecPos = pCha->GetPos(); //  VECTOR3(fX, fY, );
				GetMainCam()->Turn(-0.4f, &vecPos);
			}
		}
		else {
			VECTOR3 vecPos = GetMainCam()->m_RefPos;
			GetMainCam()->Turn(-0.4f, &vecPos);
		}
	}

	// if(IsKeyContinue(DIK_UP))   GetMainCam()->MoveForward(0.2f, FALSE);
	// if(IsKeyContinue(DIK_DOWN)) GetMainCam()->MoveForward(-0.2f,FALSE);

	//
	//if(IsKeyContinue(DIK_A)) GetMainCam()->Move(MOVE_UP);
	//if(IsKeyContinue(DIK_Z)) GetMainCam()->Move(MOVE_DOWN);

	if (IsKeyContinue(DIK_O)) {
		g_Render.SetWorldViewFOV(g_Render.GetWorldViewFOV() - D3DX_PI / 180.0f);
	}
	else if (IsKeyContinue(DIK_P)) {
		g_Render.SetWorldViewFOV(g_Render.GetWorldViewFOV() + D3DX_PI / 180.0f);
	}

	g_Editor.HandleKeyContinue();
}

CTestDemo::CTestDemo() {
	_skybox = 0;
}
CTestDemo::~CTestDemo() {
	SAFE_RELEASE(_skybox);
}

BOOL CTestDemo::Init() {
	_res_mgr = g_Render.GetInterfaceMgr()->res_mgr;
	_res_mgr->CreateModel(&_skybox);
	if (FAILED(_skybox->Load("sky.lmo")))
		return 0;
	_skybox->PlayDefaultAnimation(!g_stUISystem.m_sysProp.m_gameOption.bFramerate);

	DWORD num = _skybox->GetPrimitiveNum();
	for (DWORD i = 0; i < num; i++) {
		_skybox->GetPrimitive(i)->SetState(STATE_TRANSPARENT, 0);
		_skybox->GetPrimitive(i)->SetState(STATE_UPDATETRANSPSTATE, 0);
	}

	return 1;
}
void CTestDemo::FrameMove() {
	if (_skybox) {
		lwMatrix44 mat = lwMatrix44Translate(g_pGameApp->GetMainCam()->m_RefPos);
		_skybox->SetMatrix(&mat);
		_skybox->Update();
	}
}
void CTestDemo::Render() {
	if (_skybox) {
		_skybox->Render();
	}
}

CTestDemo* g_pTestDemo = 0;

void InitTestDemo() {
	g_pTestDemo = new CTestDemo();
	g_pTestDemo->Init();
}

void ReleaseTestDemo() {
	SAFE_DELETE(g_pTestDemo);
}

#endif
