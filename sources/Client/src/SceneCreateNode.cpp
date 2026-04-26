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
CCharacter* CGameScene::AddBoat( stNetChangeChaPart& part )
{
	int nScriptID = part.sTypeID;
    CChaRecord* pInfo = GetChaRecordInfo( nScriptID );
	if( !pInfo ) return NULL;

    CCharacter *pCha = _GetFirstInvalidCha(); // Cha	
	if( !pCha ) 
	{
        g_logManager.InternalLog(LogLevel::Error, "errors", GetLanguageString(341));
		return NULL;
	}

    pCha->Destroy();
	pCha->setTypeID( nScriptID );
	pCha->SetDefaultChaInfo( pInfo );

	if( !pCha->LoadBoat( part ) ) return NULL;

    pCha->EnableAI(TRUE);

	pCha->setChaModalType( pInfo->chModalType );

    HandleSceneMsg(SCENEMSG_CHA_CREATE, pCha->getID(), nScriptID);

	pCha->setYaw( 0 );
	pCha->FaceTo( 0 );
	pCha->PlayPose( 1, PLAY_LOOP );

    pCha->setMoveSpeed( pInfo->lMSpd );

    pCha->SetOpaque( (float)pInfo->chDiaphaneity/100.0f );    
    pCha->SetIsForUI( false );
    pCha->SetValid(TRUE); 

	pCha->InitState();
	return pCha;
}

CCharacter* CGameScene::AddCharacter(int nScriptID)
{
	BYTE loadtex_flag;
	BYTE loadmesh_flag;
	lwIByteSet* res_bs = 0;
	CCharacter* pCha = NULL;
	CChaRecord* pInfo = GetChaRecordInfo(nScriptID);
	if (!pInfo)
	{
		ToLogService("errors", LogLevel::Error, "msgCGameScene::AddCharacter() - GetChaRecordInfo({}) return NULL", nScriptID);
		pCha = NULL;
		goto __ret;
	}

	pCha = _GetFirstInvalidCha(); // ????????????????????????Cha
	if (pCha == NULL)
	{
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

	if (pInfo->chModalType == enumMODAL_MAIN_CHA)
	{
		// save loading res mt flag
		// res_bs->SetValue(OPT_RESMGR_LOADTEXTURE_MT, 0);
		// res_bs->SetValue(OPT_RESMGR_LOADMESH_MT, 0);

		// ??id?????
		DWORD part_buf[5] = {
			pInfo->sSkinInfo[0], pInfo->sSkinInfo[1], pInfo->sSkinInfo[2], pInfo->sSkinInfo[3], pInfo->sSkinInfo[4],
		};

		if (((CCharacterModel*)pCha)->LoadCha(pInfo->chModalType, pInfo->sModel, part_buf) == 0)
		{
			g_logManager.InternalLog(LogLevel::Error, "errors", SafeVFormat(GetLanguageString(26), nScriptID, std::string_view(pInfo->DataName.c_str())));
			pCha = NULL;
			goto __ret;
		}
	}
	else if (pInfo->chModalType == enumMODAL_BOAT)
	{
		// res_bs->SetValue(OPT_RESMGR_LOADTEXTURE_MT, 0);
		// res_bs->SetValue(OPT_RESMGR_LOADMESH_MT, 0);
		DWORD part_buf[3] = {
			pInfo->sSkinInfo[0],
			pInfo->sSkinInfo[1],
			pInfo->sSkinInfo[2],
		};

		if (((CCharacterModel*)pCha)->LoadShip(pInfo->chModalType, pInfo->sModel, part_buf) == 0)
		{
			g_logManager.InternalLog(LogLevel::Error, "errors", SafeVFormat(GetLanguageString(26), nScriptID, std::string_view(pInfo->DataName.c_str())));
			pCha = NULL;
			goto __ret;
		}
	}
	else if (pInfo->chModalType == enumMODAL_EMPL)
	{
		// res_bs->SetValue(OPT_RESMGR_LOADTEXTURE_MT, 0);
		// res_bs->SetValue(OPT_RESMGR_LOADMESH_MT, 0);
		DWORD part_buf[5] = {
			pInfo->sSkinInfo[0],
			pInfo->sSkinInfo[1],
			pInfo->sSkinInfo[2],
			pInfo->sSkinInfo[3],
		};

		if (((CCharacterModel*)pCha)->LoadTower(pInfo->chModalType, part_buf) == 0)
		{
			g_logManager.InternalLog(LogLevel::Error, "errors", SafeVFormat(GetLanguageString(26), nScriptID, std::string_view(pInfo->DataName.c_str())));
			pCha = NULL;
			goto __ret;
		}
	}
	else if (pInfo->chModalType == enumMODAL_OTHER)
	{
		// res_bs->SetValue(OPT_RESMGR_LOADTEXTURE_MT, 0);
		// res_bs->SetValue(OPT_RESMGR_LOADMESH_MT, 0);

		// ???????
		MPChaLoadInfo load_info;

		sprintf(load_info.bone, "%04d.lab", pInfo->sModel);

		for (DWORD i = 0; i < 5; i++)
		{
			if (pInfo->sSkinInfo[i] == 0)
				continue;

			DWORD file_id = pInfo->sModel * 1000000 + pInfo->sSuitID * 10000 + i;
			sprintf(load_info.part[i], "%010d.lgo", file_id);
		}

		if (((CCharacterModel*)pCha)->LoadCha(&load_info) == 0)
		{
			g_logManager.InternalLog(LogLevel::Error, "errors", SafeVFormat(GetLanguageString(26), nScriptID, std::string_view(pInfo->DataName.c_str())));
			pCha = NULL;
			goto __ret;
		}
	}

	if (((CCharacterModel*)pCha)->LoadPose(pInfo->sActionID) == 0)
	{
		g_logManager.InternalLog(LogLevel::Error, "errors", SafeVFormat(GetLanguageString(27), nScriptID, std::string_view(pInfo->DataName.c_str())));
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
	if (res_bs)
	{
		res_bs->SetValue(OPT_RESMGR_LOADTEXTURE_MT, loadtex_flag);
		res_bs->SetValue(OPT_RESMGR_LOADMESH_MT, loadmesh_flag);
	}

	return pCha;
}

//-----------------------
// SceneNode 
// Add SceneObj to scene
//----------------------- 
CSceneObj* CGameScene::AddSceneObj(int nScriptID)
{
	CSceneObjInfo *pInfo = GetSceneObjInfo(nScriptID);
    if(pInfo==NULL)
    {
        g_logManager.InternalLog(LogLevel::Error, "errors", SafeVFormat(GetLanguageString(343), nScriptID));
        return NULL;
    }

	if(GlobalAppConfig.IsCheckOvermax())
	{
		if(m_dwValidSceneObjCnt>=290)
		{
			g_logManager.InternalLog(LogLevel::Debug, "ui", SafeVFormat(GetLanguageString(344), 299));
		}
		if(m_dwSceneObjPolyCnt>=9000)
		{
			g_logManager.InternalLog(LogLevel::Debug, "ui", SafeVFormat(GetLanguageString(345), 9000));
		}
	}

	BOOL bCreate = FALSE;
    CSceneObj *pObj = _GetFirstInvalidSceneObj(nScriptID, bCreate);
	if(pObj)
	{
        if(bCreate)
        {
		    pObj->Destroy();
		    if(!pObj->_CreateNode(nScriptID, pInfo->_type, this))
            {
                return NULL;
            }
        }
        else
        {
            ToLogService("common", "Found Same Type Object [{}]", nScriptID);
        }

        if(pInfo->_type == 3)
        {
            pObj->SetSceneLightInfo(nScriptID);
            //g_pSceneLight = (SceneLight*)pObj;
            //g_AnimCtrlLight.Load(".\\scripts\\aaa.txt");

        }
        else if(pInfo->_type == 4)
        {
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
	else
	{
		g_logManager.InternalLog(LogLevel::Debug, "ui", SafeVFormat(GetLanguageString(346), _nSceneObjCnt));
	}

    // 
    if(0)
    {
        const DWORD no_transp_num = 1;
        DWORD no_transp[no_transp_num] =
        {
            449, // 
        };

        for(DWORD i = 0; i < no_transp_num; i++)
        {
            if(nScriptID == no_transp[i])
            {
                DWORD num = pObj->GetPrimitiveNum();
                for(DWORD j = 0; j < num; j++)
                {
                    if(pObj->GetPrimitive(j)->GetID() < 22)
                    {
                        pObj->GetPrimitive(j)->SetState(STATE_TRANSPARENT, 0);
                        pObj->GetPrimitive(j)->SetState(STATE_UPDATETRANSPSTATE, 0);
                    }
                }

                break;
            }
        }
    }

	// Added by clp
	if ( pInfo->_isReallyBig )
	{
		CCharacter * tempChar = GetMainCha();
		if( tempChar )
		{
			float height = GetTerrainHeight( tempChar->getPos().x, tempChar->getPos().y );
			pObj->setRBOHeight( height );
		}
		AddRBO( pObj );
	}

	return pObj;
}


//-----------------------
// SceneNode 
// Add EffectObj to scene
//----------------------- 
CEffectObj* CGameScene::AddSceneEffect(int nEffectTypeID)
{
	CEffectObj *pEff = GetFirstInvalidEffObj();
	if(pEff)
	{
		if(!pEff->Create(nEffectTypeID))
		{
			return NULL;
		}
		//pEff->Emission(0,&g_pGameApp->GetMainCam()->m_RefPos,NULL);
		auto v = D3DXVECTOR3(0, 0, 0);
		pEff->Emission(0,&v,nullptr);
        pEff->SetValid(TRUE);
    }
	else
	{
		g_logManager.InternalLog(LogLevel::Debug, "ui", GetLanguageString(347));
	}
	return pEff;
}

//-----------------------
// SceneNode 
// Add SceneItem to scene
//----------------------- 
CSceneItem* CGameScene::AddSceneItem(int nScriptID, int nType)
{
	CSceneItem *pObj = _GetFirstInvalidSceneItem();
	if(pObj)
	{
		//HandleSceneMsg(SCENEMSG_SCENEOBJ_DESTROY, pObj->getEffectID(),pObj->getID());
		
		pObj->Destroy();
		if(!pObj->_CreateNode(nScriptID, nType, this))
        {
            return NULL;
        }

		//pObj->SetValid(TRUE);
		//HandleSceneMsg(SCENEMSG_SCENEOBJ_CREATE, pObj->getID());
		HandleSceneMsg(SCENEMSG_SCENEITEM_CREATE, pObj->getID(),nType);
	}
	else
	{
		g_logManager.InternalLog(LogLevel::Debug, "common", GetLanguageString(348));
	}
	return pObj;
}

CSceneItem* CGameScene::AddSceneItem( const char* file )
{
	CSceneItem *pObj = _GetFirstInvalidSceneItem();
	if(pObj)
	{		
		pObj->Destroy();
        pObj->SetScene( this );
        if( LW_FAILED( pObj->Load( file ) ) )
            return NULL;
        pObj->SetValid( 1 );
        pObj->SetItemInfo( 1 );
	}
	else
	{
		g_logManager.InternalLog(LogLevel::Debug, "common", GetLanguageString(348));
	}
	return pObj;

}

