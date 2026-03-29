#include "netprotocol.h"
#include "EffectObj.h"

//-----------------------------
// , ID
//-----------------------------
inline int sceAddObj(lua_State * L)
{
    // 
    BOOL bValid = (lua_gettop (L)==3 && lua_islightuserdata(L, 1) && lua_isnumber (L, 2) &&  lua_isnumber (L, 3));
    if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }
    
    CGameScene *pScene = (CGameScene*)lua_touserdata(L, 1);
    if(pScene==NULL)
    {
        SCENE_NULL_ERROR
        return 0;
    }

    int nType     = (int)lua_tonumber(L, 2);
    int nScriptID = (int)lua_tonumber(L, 3);
    
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
    lua_pushlightuserdata (L, pNode);
    return 1;
}

//-------------------
// ID
//-------------------
inline int sceGetObj(lua_State *L)
{
    // 
    BOOL bValid = (lua_gettop (L)==3 && lua_islightuserdata(L, 1) && lua_isnumber(L,2) && lua_isnumber(L, 3));
    if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }
    
    CGameScene *pScene = (CGameScene*)lua_touserdata(L, 1);
    if(pScene==NULL)
    {
        SCENE_NULL_ERROR
        return 0;
    }
    
    int nType    = (int)lua_tonumber(L, 2);
    int nArrayID = (int)lua_tonumber(L, 3);
    
    CSceneNode *pNode = NULL;
    switch(nType)
    {
        case 0: { pNode = pScene->GetCha(nArrayID); break;       }
        case 1: { pNode = pScene->GetSceneItem(nArrayID); break; }
    }
    lua_pushlightuserdata(L, pNode);
    return 1;
}

//---------
// 
//---------
inline int sceRemoveObj(lua_State *L)
{
    // 
    BOOL bValid = (lua_gettop (L)==1 && lua_islightuserdata(L, 1)); 
    if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }
    
    CSceneNode *pNode = (CSceneNode*)lua_touserdata(L, 1);
    pNode->SetValid(FALSE);
    return 0;
}


//-----------------
// 
//-----------------
inline int sceSetMainCha(lua_State *L)
{
    // 
    BOOL bValid = (lua_gettop (L)==2 && lua_islightuserdata(L, 1) && lua_islightuserdata(L, 2)); 
    if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }
    
    CGameScene *pScene = (CGameScene*)lua_touserdata(L, 1);
    if(pScene==NULL)
    {
        SCENE_NULL_ERROR
        return 0;
        
    }
    
    CCharacter *pCha = (CCharacter*)lua_touserdata(L, 2);
    if(!pCha) return 0;

    pScene->SetMainCha(pCha->getID());
    return 0;
}

//-----------------
// 
//-----------------
inline int sceGetMainCha(lua_State *L)
{
    // 
    BOOL bValid = (lua_gettop (L)==1 && lua_islightuserdata(L, 1)); 
    if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }
    CGameScene *pScene = (CGameScene*)lua_touserdata(L, 1);
    if(pScene==NULL)
    {
        SCENE_NULL_ERROR
        return 0;
    }
    CCharacter *pCha = pScene->GetMainCha();
    if(!pCha) return 0;

    lua_pushlightuserdata(L, pCha);
    return 1;
}

inline int sceGetHoverCha(lua_State *L)
{
    // 
    BOOL bValid = (lua_gettop (L)==1 && lua_islightuserdata(L, 1)); 
    if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }
    CGameScene *pScene = (CGameScene*)lua_touserdata(L, 1);
    if(pScene==NULL)
    {
        SCENE_NULL_ERROR
        return 0;
    }
    CCharacter *pCha = pScene->GetSelectCha();
    if(!pCha) return 0;

    lua_pushlightuserdata(L, pCha);
    return 1;
}

inline int sceEnableDefaultMouse(lua_State *L)
{
    // 
    BOOL bValid = (lua_gettop (L)==2 && lua_islightuserdata(L, 1) && lua_isnumber(L,2)); 
    if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }
    CGameScene *pScene = (CGameScene*)lua_touserdata(L, 1);
    if(pScene==NULL)
    {
        SCENE_NULL_ERROR
        return 0;
    }
    BOOL bEnable = (BOOL)lua_tonumber(L, 2);
    if(bEnable)
    {
        pScene->GetUseLevel().SetTrue(LEVEL_MOUSE_RUN);
    }
    else
    {
        pScene->GetUseLevel().SetFalse(LEVEL_MOUSE_RUN);
    }
    return 0;
}

