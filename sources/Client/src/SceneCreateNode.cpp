#include "Stdafx.h"
#include "Scene.h"
#include "GameApp.h"
#include "CharacterPoseSet.h"
#include "CharacterRecord.h"
#include "SceneObjRecordStore.h"

#include "CharacterModel.h"
#include "Character.h"
#include "SceneObj.h"
#include "SceneItem.h"
#include "EffectObj.h"
#include "GameConfig.h"
#include "NetProtocol.h"

//-----------------------
// SceneNode 
// Add Character to scene
//----------------------- 
CCharacter* CGameScene::AddBoat(stNetChangeChaPart& part) {
	int nScriptID = part.sTypeID;
	CChaRecord* pInfo = GetChaRecordInfo(nScriptID);
	if (!pInfo) return NULL;

	CCharacter* pCha = _GetFirstInvalidCha(); // Cha	
	if (!pCha) {
		g_logManager.InternalLog(LogLevel::Error, "errors", GetLanguageString(341));
		return NULL;
	}

	pCha->Destroy();
	pCha->setTypeID(nScriptID);
	pCha->SetDefaultChaInfo(pInfo);

	if (!pCha->LoadBoat(part)) return NULL;

	pCha->EnableAI(TRUE);

	pCha->setChaModalType(pInfo->chModalType);

	HandleSceneMsg(SCENEMSG_CHA_CREATE, pCha->getID(), nScriptID);

	pCha->setYaw(0);
	pCha->FaceTo(0);
	pCha->PlayPose(1, PLAY_LOOP);

	pCha->setMoveSpeed(pInfo->lMSpd);

	pCha->SetOpaque((float)pInfo->chDiaphaneity / 100.0f);
	pCha->SetIsForUI(false);
	pCha->SetValid(TRUE);

	pCha->InitState();
	return pCha;
}

CCharacter* CGameScene::AddCharacter(int nScriptID) {
	BYTE loadtex_flag;
	BYTE loadmesh_flag;
	lwIByteSet* res_bs = 0;
	CCharacter* pCha = NULL;
	CChaRecord* pInfo = GetChaRecordInfo(nScriptID);
	if (!pInfo) {
		ToLogService("errors", LogLevel::Error, "msgCGameScene::AddCharacter() - GetChaRecordInfo({}) return NULL",
					 nScriptID);
		pCha = NULL;
		goto __ret;
	}

	pCha = _GetFirstInvalidCha(); // ????????????????????????Cha
	if (pCha == NULL) {
		g_logManager.InternalLog(LogLevel::Error, "errors", GetLanguageString(342));
		pCha = NULL;
		goto __ret;
	}

	pCha->Destroy();
	pCha->setTypeID(nScriptID);
	pCha->SetDefaultChaInfo(pInfo);

	res_bs = g_Render.GetInterfaceMgr()->res_mgr->GetByteSet();
	loadtex_flag = res_bs->GetValue(OPT_RESMGR_LOADTEXTURE_MT);
	loadmesh_flag = res_bs->GetValue(OPT_RESMGR_LOADMESH_MT);

	if (pInfo->chModalType == enumMODAL_MAIN_CHA) {
		// save loading res mt flag
		// res_bs->SetValue(OPT_RESMGR_LOADTEXTURE_MT, 0);
		// res_bs->SetValue(OPT_RESMGR_LOADMESH_MT, 0);

		// ??id?????
		DWORD part_buf[5] = {
			pInfo->sSkinInfo[0], pInfo->sSkinInfo[1], pInfo->sSkinInfo[2], pInfo->sSkinInfo[3], pInfo->sSkinInfo[4],
		};

		//  Что было: первый параметр Load*-вызовов получал pInfo->chModalType
		//  (enum 1..4), который потом сохранялся в CCharacterModel::_TypeID.
		//  _TypeID используется как primary key для GetChaRecordInfo() в
		//  Cull/LoadPose/setAttachedCharacterID, поэтому подмена id-архетипа
		//  enum-ом давала бессмысленный lookup. Для main_cha случайно
		//  совпадало (chModalType=1 ~ lID=1), для ship/empl нет, а ниже —
		//  LoadCha(&load_info) вообще не выставлял _TypeID, оставляя
		//  0xCDCDCDCD (Debug-маркер uninit heap), что засоряло канал store_miss.
		//  Что исправили: передаём nScriptID — уже подтверждённый primary key
		//  архетипа (рядом, на строке 62, по нему достаётся pInfo).
		if (((CCharacterModel*)pCha)->LoadCha(nScriptID, pInfo->sModel, part_buf) == 0) {
			g_logManager.InternalLog(LogLevel::Error, "errors",
									 SafeVFormat(GetLanguageString(26), nScriptID,
												 std::string_view(pInfo->DataName.c_str())));
			pCha = NULL;
			goto __ret;
		}
	}
	else if (pInfo->chModalType == enumMODAL_BOAT) {
		// res_bs->SetValue(OPT_RESMGR_LOADTEXTURE_MT, 0);
		// res_bs->SetValue(OPT_RESMGR_LOADMESH_MT, 0);
		DWORD part_buf[3] = {
			pInfo->sSkinInfo[0],
			pInfo->sSkinInfo[1],
			pInfo->sSkinInfo[2],
		};

		//  Было: LoadShip(pInfo->chModalType, ...) — _TypeID = 2 (enum BOAT),
		//  Cull искал CChaRecord с lID=2 (бессмысленно).
		//  Стало: передаём nScriptID — реальный primary key архетипа.
		if (((CCharacterModel*)pCha)->LoadShip(nScriptID, pInfo->sModel, part_buf) == 0) {
			g_logManager.InternalLog(LogLevel::Error, "errors",
									 SafeVFormat(GetLanguageString(26), nScriptID,
												 std::string_view(pInfo->DataName.c_str())));
			pCha = NULL;
			goto __ret;
		}
	}
	else if (pInfo->chModalType == enumMODAL_EMPL) {
		// res_bs->SetValue(OPT_RESMGR_LOADTEXTURE_MT, 0);
		// res_bs->SetValue(OPT_RESMGR_LOADMESH_MT, 0);
		DWORD part_buf[5] = {
			pInfo->sSkinInfo[0],
			pInfo->sSkinInfo[1],
			pInfo->sSkinInfo[2],
			pInfo->sSkinInfo[3],
		};

		//  Было: LoadTower(pInfo->chModalType, ...) — _TypeID = 3 (enum EMPL).
		//  Стало: передаём nScriptID — реальный primary key архетипа.
		if (((CCharacterModel*)pCha)->LoadTower(nScriptID, part_buf) == 0) {
			g_logManager.InternalLog(LogLevel::Error, "errors",
									 SafeVFormat(GetLanguageString(26), nScriptID,
												 std::string_view(pInfo->DataName.c_str())));
			pCha = NULL;
			goto __ret;
		}
	}
	else if (pInfo->chModalType == enumMODAL_OTHER) {
		// res_bs->SetValue(OPT_RESMGR_LOADTEXTURE_MT, 0);
		// res_bs->SetValue(OPT_RESMGR_LOADMESH_MT, 0);

		// ???????
		MPChaLoadInfo load_info;

		{
			auto r = std::format_to_n(load_info.bone, sizeof(load_info.bone) - 1, "{:04}.lab", pInfo->sModel);
			*r.out = 0;
		}

		for (DWORD i = 0; i < 5; i++) {
			if (pInfo->sSkinInfo[i] == 0)
				continue;

			DWORD file_id = pInfo->sModel * 1000000 + pInfo->sSuitID * 10000 + i;
			auto r = std::format_to_n(load_info.part[i], sizeof(load_info.part[i]) - 1, "{:010}.lgo", file_id);
			*r.out = 0;
		}

		//  Было: LoadCha(&load_info) — overload без передачи type_id,
		//  поэтому _TypeID оставался 0xCDCDCDCD (Debug-маркер uninit heap).
		//  Каждый кадр Cull() уходил в GetChaRecordInfo(0xCDCDCDCD) → MISS,
		//  поток store_miss про id=-842150451 в логах.
		//  Стало: используем новый overload LoadCha(info, type_id) с nScriptID.
		if (((CCharacterModel*)pCha)->LoadCha(&load_info, nScriptID) == 0) {
			g_logManager.InternalLog(LogLevel::Error, "errors",
									 SafeVFormat(GetLanguageString(26), nScriptID,
												 std::string_view(pInfo->DataName.c_str())));
			pCha = NULL;
			goto __ret;
		}
	}

	if (((CCharacterModel*)pCha)->LoadPose(pInfo->sActionID) == 0) {
		g_logManager.InternalLog(LogLevel::Error, "errors",
								 SafeVFormat(GetLanguageString(27), nScriptID,
											 std::string_view(pInfo->DataName.c_str())));
		pCha = NULL;
		goto __ret;
	}

	{
		pCha->SetValid(TRUE);
		pCha->EnableAI(TRUE);

		pCha->setChaModalType(pInfo->chModalType);

		HandleSceneMsg(SCENEMSG_CHA_CREATE, pCha->getID(), nScriptID);

		pCha->setYaw(0);
		pCha->FaceTo(0);
		pCha->PlayPose(1, PLAY_LOOP);

		pCha->setMoveSpeed(pInfo->lMSpd);

		pCha->SetOpaque(static_cast<float>(pInfo->chDiaphaneity) / 100.0f);
		pCha->SetIsForUI(false);
		pCha->InitState();
		pCha->SetScale(D3DXVECTOR3(pInfo->scaling[0], pInfo->scaling[1], pInfo->scaling[2]));
	}
__ret:
	// if( res_bs && (pInfo->chModalType==enumMODAL_MAIN_CHA) )
	if (res_bs) {
		res_bs->SetValue(OPT_RESMGR_LOADTEXTURE_MT, loadtex_flag);
		res_bs->SetValue(OPT_RESMGR_LOADMESH_MT, loadmesh_flag);
	}

	return pCha;
}

//-----------------------
// SceneNode 
// Add SceneObj to scene
//----------------------- 
CSceneObj* CGameScene::AddSceneObj(int nScriptID) {
	CSceneObjInfo* pInfo = GetSceneObjInfo(nScriptID);
	if (pInfo == NULL) {
		g_logManager.InternalLog(LogLevel::Error, "errors", SafeVFormat(GetLanguageString(343), nScriptID));
		return NULL;
	}

	//  Раньше тут был блок IsCheckOvermax с двумя предупреждениями:
	//  «sceneobj count >= 290» и «sceneobj poly count >= 9000» в канал "ui"
	//  на каждый AddSceneObj. Это legacy-предупреждения 2004 года для
	//  слабых DX9 GPU; на современном железе пороги бессмысленны (миллионы
	//  полигонов в кадре — норма), а на богатой карте предупреждение
	//  срабатывало десятки раз в кадр и засоряло "ui"_*.log. Удалено.

	BOOL bCreate = FALSE;
	CSceneObj* pObj = _GetFirstInvalidSceneObj(nScriptID, bCreate);
	if (pObj) {
		if (bCreate) {
			pObj->Destroy();
			if (!pObj->_CreateNode(nScriptID, pInfo->_type, this)) {
				return NULL;
			}
		}
		else {
			ToLogService("common", "Found Same Type Object [{}]", nScriptID);
		}

		if (pInfo->_type == 3) {
			pObj->SetSceneLightInfo(nScriptID);
			//g_pSceneLight = (SceneLight*)pObj;
			//g_AnimCtrlLight.Load(".\\scripts\\aaa.txt");
		}
		else if (pInfo->_type == 4) {
			pObj->SetSceneLightInfo(nScriptID);
		}


		// last
		pObj->setObjType(pInfo->_type);
		pObj->SetValid(TRUE);
#if(defined OPT_CULL_2)
		pObj->SetCullingFlag(1);
#else
		pObj->SetCullingFlag(0);
#endif
		HandleSceneMsg(SCENEMSG_SCENEOBJ_CREATE, pObj->getID(), nScriptID);
	}
	else {
		g_logManager.InternalLog(LogLevel::Debug, "ui", SafeVFormat(GetLanguageString(346), _nSceneObjCnt));
	}

	// 
	if (0) {
		const DWORD no_transp_num = 1;
		DWORD no_transp[no_transp_num] =
		{
			449, // 
		};

		for (DWORD i = 0; i < no_transp_num; i++) {
			if (nScriptID == no_transp[i]) {
				DWORD num = pObj->GetPrimitiveNum();
				for (DWORD j = 0; j < num; j++) {
					if (pObj->GetPrimitive(j)->GetID() < 22) {
						pObj->GetPrimitive(j)->SetState(STATE_TRANSPARENT, 0);
						pObj->GetPrimitive(j)->SetState(STATE_UPDATETRANSPSTATE, 0);
					}
				}

				break;
			}
		}
	}

	// Added by clp
	if (pInfo->_isReallyBig) {
		CCharacter* tempChar = GetMainCha();
		if (tempChar) {
			float height = GetTerrainHeight(tempChar->getPos().x, tempChar->getPos().y);
			pObj->setRBOHeight(height);
		}
		AddRBO(pObj);
	}

	return pObj;
}


//-----------------------
// SceneNode 
// Add EffectObj to scene
//----------------------- 
CEffectObj* CGameScene::AddSceneEffect(int nEffectTypeID) {
	CEffectObj* pEff = GetFirstInvalidEffObj();
	if (pEff) {
		if (!pEff->Create(nEffectTypeID)) {
			return NULL;
		}
		//pEff->Emission(0,&g_pGameApp->GetMainCam()->m_RefPos,NULL);
		auto v = D3DXVECTOR3(0, 0, 0);
		pEff->Emission(0, &v, nullptr);
		pEff->SetValid(TRUE);
	}
	else {
		g_logManager.InternalLog(LogLevel::Debug, "ui", GetLanguageString(347));
	}
	return pEff;
}

//-----------------------
// SceneNode 
// Add SceneItem to scene
//----------------------- 
CSceneItem* CGameScene::AddSceneItem(int nScriptID, int nType) {
	CSceneItem* pObj = _GetFirstInvalidSceneItem();
	if (pObj) {
		//HandleSceneMsg(SCENEMSG_SCENEOBJ_DESTROY, pObj->getEffectID(),pObj->getID());

		pObj->Destroy();
		if (!pObj->_CreateNode(nScriptID, nType, this)) {
			return NULL;
		}

		//pObj->SetValid(TRUE);
		//HandleSceneMsg(SCENEMSG_SCENEOBJ_CREATE, pObj->getID());
		HandleSceneMsg(SCENEMSG_SCENEITEM_CREATE, pObj->getID(), nType);
	}
	else {
		g_logManager.InternalLog(LogLevel::Debug, "common", GetLanguageString(348));
	}
	return pObj;
}

CSceneItem* CGameScene::AddSceneItem(std::string_view file) {
	CSceneItem* pObj = _GetFirstInvalidSceneItem();
	if (pObj) {
		pObj->Destroy();
		pObj->SetScene(this);
		if (LW_FAILED(pObj->Load( file )))
			return NULL;
		pObj->SetValid(1);
		pObj->SetItemInfo(1);
	}
	else {
		g_logManager.InternalLog(LogLevel::Debug, "common", GetLanguageString(348));
	}
	return pObj;
}
