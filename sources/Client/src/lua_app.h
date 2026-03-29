
// ---   (LuaBridge ) ---

inline void appSetCaption(const std::string& caption)
{
    g_pGameApp->SetCaption(caption.c_str());
}

inline void appPlaySound(int soundId)
{
    g_pGameApp->PlaySound(soundId);
}

inline void appUpdateRender()
{
    MPTerrain *pTerrain = g_pGameApp->GetCurScene()->GetTerrain();
    if(pTerrain) pTerrain->UpdateRender();
}

// --- lua_CFunction (lightuserdata  LuaBridge    ) ---

inline int appGetCurScene(lua_State * L)
{
    BOOL bValid = (lua_gettop (L)==0);
    if(!bValid)
    {
        PARAM_ERROR
        return 0;
    }
    lua_pushlightuserdata(L, g_pGameApp->GetCurScene());
    return 1;
}

inline int appSetCurScene(lua_State * L)
{
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

inline int appCreateScene(lua_State *L)
{
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

    CGameScene *pScene = NULL;

    lua_pushlightuserdata(L, pScene);
    return 1;
}
