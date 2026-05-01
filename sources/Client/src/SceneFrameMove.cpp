#include "stdafx.h"
#include "Scene.h"
#include "DebugStateSystem.h"

#include "Character.h"
#include "SceneObj.h"
#include "SceneItem.h"
#include "EffectObj.h"

#include "GameApp.h"
#include "MPEditor.h"
#include "GlobalVar.h"
#include "UICursor.h"
#include "CharacterRecord.h"
#include "GameConfig.h"

#include "SmallMap.h"

#include "PacketCmd.h"

void CGameScene::_FrameMove(DWORD dwTimeParam) {
	_IsShowItemName = g_pGameApp->IsShiftPress();

	//if(GetTeamList(0))
	//{
	//	if(!(GetTeamList(0)->IsLoad()))
	//	{
	//		GetTeamList(0)->LoadCha(1);
	//		GetTeamList(1)->LoadCha(2);
	//		GetTeamList(2)->LoadCha(3);
	//		GetTeamList(3)->LoadCha(4);
	//	}
	//}


	DebugStateSystem::Instance().Clear(DebugStateSystem::Category::Game);

	if (_pTerrain) {
		MPTimer tMap;
		tMap.Begin();
		_pTerrain->DynamicLoading(GetTickCount());

		g_pGameApp->m_dwLoadingObjTime = tMap.End();

		GetPickPos(g_pGameApp->GetMouseX(), g_pGameApp->GetMouseY(), _vMousePos);

		//{ // scene obj hit test
		//    D3DXVECTOR3 iorg, iray, hit;
		//    g_Render.GetPickRayVector( g_pGameApp->GetMouseX(), g_pGameApp->GetMouseX(), &iorg, &iray );
		//    if( HitTestSceneObjTerrain(&hit, &iorg, &iray ) > 0 )
		//    {
		//        if(hit.z > _vMousePos.z)
		//        {
		//            _vMousePos = hit;
		//        }
		//    }
		//}

		_nMouseX = (int)(_vMousePos.x * 100.0f);
		_nMouseY = (int)(_vMousePos.y * 100.0f);

		// , 
		if (GlobalAppConfig.IsEditor() && !g_pGameApp->IsCameraFollow()) {
			static D3DXVECTOR3 vecPos;
			static int nScrFocus;
			nScrFocus = g_Render.GetScrHeight() / 2;
			if (GetPickPos(g_Render.GetScrWidth() / 2, nScrFocus, vecPos)) {
				_pTerrain->SetShowCenter(vecPos.x, vecPos.y);
			}
			//D3DXVec3Add(&vecCha,&vecCha,&(-pCam->m_vDir * 6));
			//_pTerrain->SetShowCenter(vecPos.x, vecPos.y); 
		}
		_pTerrain->FrameMove(dwTimeParam);
	}

	int i;
	for (i = 0; i < _nChaCnt; i++) {
		CCharacter* pCha = &_pChaArray[i];
		if (pCha->IsValid() && !pCha->GetIsForUI()) {
			pCha->FrameMove(dwTimeParam);
			//pCha->SetCircleColor(0xff0000ff);
		}
	}

	int nValidCnt = 0;
	for (i = 0; i < _nSceneObjCnt; i++) {
		CSceneObj* pObj = &_pSceneObjArray[i];
		if (pObj->IsValid()) {
			pObj->FrameMove(dwTimeParam);
			nValidCnt++;
		}
	}


	// scene item														   
	for (i = 0; i < _nSceneItemCnt; i++) {
		CSceneItem* pObj = &_pSceneItemArray[i];
		if (pObj->IsValid()) {
			//lwIPrimitive* pri = pObj->GetObject()->GetPrimitive();
			//if(pri->GetHelperObject())
			//    pri->GetHelperObject()->SetVisible(1);
			pObj->FrameMove(dwTimeParam);
			nValidCnt++;
		}
	}


	//if(ResMgr.GetCanFrame() == 1)
	{
		if (_nEffCnt > 0) {
			int nValidCnt = 0;
			for (i = 0; i < _nEffCnt; i++) {
				if (_pEffectArray[i].IsValid()) {
					_pEffectArray[i].FrameMove(dwTimeParam);
					nValidCnt++;
				}
			}
			m_dwValidEffCnt = nValidCnt;
			//LG("aalidEff","num==%d\n",m_dwValidEffCnt);

			static CTimeWork time(5 * 60 * 1000);
			if (time.IsTimeOut(dwTimeParam)) {
				for (i = 0; i < _nEffCnt; i++) {
					if (_pEffectArray[i].IsValid()) {
						_pEffectArray[i].SetInvalidByTime(dwTimeParam);
					}
				}
			}
		}

		for (i = 0; i < _nShadeCnt; ++i) {
			if (_pShadeArray[i].IsValid())
				_pShadeArray[i].FrameMove(dwTimeParam);
		}
	}

	//if( g_pGameApp->IsMouseButtonPress(0) && g_Editor.IsEnable()==FALSE)
	//{
	//static DWORD dwMouseAutoMoveTick = 0;
	//int nTargetX = (int)(_vMousePos.x * 100.0f);
	//int nTargetY = (int)(_vMousePos.y * 100.0f);

	//VECTOR3 vec(_vMousePos.x, _vMousePos.y, 10);
	//VECTOR3 vecHit;
	//D3DXVECTOR3 org, ray;
	//g_Render.GetPickRayVector( g_pGameApp->GetMouseX(), g_pGameApp->GetMouseY(), &org, &ray );
	//if( this->HitTestSceneObjTerrain(&vecHit, &org, &ray ) > 0 )
	//{
	//	//if( vecHit.z > _vMousePos.z )
	//	{
	//		nTargetX = (int)(vecHit.x * 100.0f);
	//		nTargetY = (int)(vecHit.y * 100.0f);
	//	}
	//}

	//if(dwTimeParam - dwMouseAutoMoveTick  > 100)
	//{
	//	dwMouseAutoMoveTick = dwTimeParam;

	//	CCharacter *pCha = SelectCharacter();
	//	if(pCha && pCha->IsNPC())
	//	{
	//		CCharacter *pMain = this->GetMainCha();            
	//		if(g_pGameApp->IsShiftPress()) pMain = pCha;
	//		if(pMain && pMain->DistanceFrom(pCha) < 400)
	//			return;
	//	}
	//}
	//}

	//if( g_pGameApp->IsMouseButtonPress(0) && g_Editor.IsEnable()==FALSE )
	//{
	//	g_pGameApp->GetCursor()->MoveTo( _vMousePos );
	//}

	if (_e3DMouseState == enumFollow) g_pGameApp->GetCursor()->MoveTo(_vMousePos);

	SelectCharacter();

	// begin debug by lsh
	// test lighting animation
	//extern SceneLight* g_pSceneLight;
	//extern AnimCtrlLight g_AnimCtrlLight;

	//if(g_pSceneLight)
	//{
	//    g_AnimCtrlLight.UpdateObject(g_pSceneLight);
	//}

	// end
	//lemon add@2005.2.17
	if (_pcPugMgr)
		_pcPugMgr->FrameMove(dwTimeParam);

	//for (i = 0; i < 4; i++)
	//{
	//	//if(_pCha2D[i])
	//	//	_pCha2D[i]->FrameMove(dwTimeParam);
	//}

	CSynchroManage::I()->FrameMove(dwTimeParam);

	_cSceenSign.FrameMove(dwTimeParam);

}
