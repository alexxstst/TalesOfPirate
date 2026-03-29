#include "netprotocol.h"
#include "EffectObj.h"

//-----------------------------
// , ID
//-----------------------------
inline CSceneNode* sceAddObj(CGameScene* pScene, int nType, int nScriptID)
{
    if (!pScene) return nullptr;

    CSceneNode *pNode = NULL;
    switch(nType)
    {
        case 0:
        {
            pNode = pScene->AddCharacter(nScriptID);
            break;
        }
        case 1:
        {
            stNetItemCreate info;
            memset( &info, 0, sizeof(info) );
            info.SPos.x   = 0;
            info.SPos.y   = 0;
            info.lWorldID = 0;
            info.lID      = nScriptID;
            pNode = NetCreateItem(info);
            break;
        }
        case 2:
        {
            pNode = pScene->AddSceneEffect(nScriptID);
            break;
        }
    }
    return pNode;
}

//-------------------
// ID
//-------------------
inline CSceneNode* sceGetObj(CGameScene* pScene, int nType, int nArrayID)
{
    if (!pScene) return nullptr;

    CSceneNode *pNode = NULL;
    switch(nType)
    {
        case 0: { pNode = pScene->GetCha(nArrayID); break;       }
        case 1: { pNode = pScene->GetSceneItem(nArrayID); break; }
    }
    return pNode;
}

//---------
//
//---------
inline void sceRemoveObj(CSceneNode* pNode)
{
    if (!pNode) return;
    pNode->SetValid(FALSE);
}


//-----------------
//
//-----------------
inline void sceSetMainCha(CGameScene* pScene, CCharacter* pCha)
{
    if (!pScene) return;
    if (!pCha) return;
    pScene->SetMainCha(pCha->getID());
}

//-----------------
//
//-----------------
inline CCharacter* sceGetMainCha(CGameScene* pScene)
{
    if (!pScene) return nullptr;
    return pScene->GetMainCha();
}

inline CCharacter* sceGetHoverCha(CGameScene* pScene)
{
    if (!pScene) return nullptr;
    return pScene->GetSelectCha();
}

inline void sceEnableDefaultMouse(CGameScene* pScene, int bEnable)
{
    if (!pScene) return;
    if(bEnable)
    {
        pScene->GetUseLevel().SetTrue(LEVEL_MOUSE_RUN);
    }
    else
    {
        pScene->GetUseLevel().SetFalse(LEVEL_MOUSE_RUN);
    }
}
