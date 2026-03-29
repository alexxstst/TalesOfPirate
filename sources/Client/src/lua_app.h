
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

inline CGameScene* appGetCurScene()
{
    return g_pGameApp->GetCurScene();
}

inline void appSetCurScene(CGameScene* pScene)
{
    if (!pScene) return;
    g_pGameApp->GotoScene(pScene, false);
}

inline CGameScene* appCreateScene(const std::string& mapName, int maxCha, int maxObj, int maxItem, int maxEff)
{
    // Placeholder —
    CGameScene* pScene = NULL;
    return pScene;
}
