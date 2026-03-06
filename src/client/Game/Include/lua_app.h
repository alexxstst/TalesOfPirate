#pragma once


//-----------------
// appscene
//-----------------
inline int lua_appSetCaption(lua_State * L)
{
    // 
    BOOL bValid = (lua_gettop (L)==1 && lua_isstring(L, 1));
    if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }
    g_pGameApp->SetCaption(lua_tostring(L, 1));
    return 0;
}

//-----------------
// appscene
//-----------------
inline int lua_appGetCurScene(lua_State * L)
{
    // 
    BOOL bValid = (lua_gettop (L)==0);
    if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }
    lua_pushlightuserdata(L, g_pGameApp->GetCurScene());
    return 1;
}

//-----------------
// appscene
//-----------------
inline int lua_appSetCurScene(lua_State * L)
{
    // 
    BOOL bValid = (lua_gettop (L)==1 && lua_islightuserdata(L, 1));
    if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }
    CGameScene *pScene = (CGameScene*)lua_touserdata(L, 1);
    g_pGameApp->GotoScene( pScene, false );
    return 1;
}

inline int lua_appPlaySound(lua_State *L)
{
    // 
    BOOL bValid = (lua_gettop (L)==1 && lua_isnumber(L, 1));
    if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }
    g_pGameApp->PlaySound((int)lua_tonumber(L, 1));
    return 0;
}


inline int lua_appCreateScene(lua_State *L)
{
    // 
    BOOL bValid = (lua_gettop (L)==5 && lua_isstring(L, 1) && 
              lua_isnumber(L,2) && lua_isnumber(L,3) && 
              lua_isnumber(L,4) && lua_isnumber(L,5));
    
    if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }
    char *pszMapName = (char*)lua_tostring(L, 1);
    
	int nMaxCha  = (int)lua_tonumber(L,2);
    int nMaxObj  = (int)lua_tonumber(L,3);
    int nMaxItem = (int)lua_tonumber(L,4);
    int nMaxEff  = (int)lua_tonumber(L,5);
   
    // Scene
    CGameScene *pScene = NULL;
    
    lua_pushlightuserdata(L, pScene);
    return 1;
}

inline int lua_appUpdateRender(lua_State *L)
{
    MPTerrain *pTerrain = g_pGameApp->GetCurScene()->GetTerrain();
    if(pTerrain) pTerrain->UpdateRender();
    return 0;
}
