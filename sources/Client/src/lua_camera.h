// ---   (LuaBridge ) ---

inline void camSetCenter(float x, float y)
{
    float ref_x = g_pGameApp->GetMainCam()->m_RefPos.x;
    float ref_y = g_pGameApp->GetMainCam()->m_RefPos.y;
    float dis_x = x - ref_x;
    float dis_y = y - ref_y;
    g_pGameApp->GetMainCam()->m_RefPos.x = x;
    g_pGameApp->GetMainCam()->m_RefPos.y = y;
    g_pGameApp->GetMainCam()->m_EyePos.x += dis_x;
    g_pGameApp->GetMainCam()->m_EyePos.y += dis_y;
}

inline void camFollow(int bEnable)
{
    g_pGameApp->EnableCameraFollow((BOOL)bEnable);
}

inline void camMoveForward(float fStep, int bHang)
{
    g_pGameApp->GetMainCam()->MoveForward(fStep, (BOOL)bHang);
}

inline void camMoveLeft(float fStep, int bHang)
{
    g_pGameApp->GetMainCam()->MoveRight(fStep, (BOOL)bHang);
}

inline void camMoveUp(float fStep)
{
    g_pGameApp->GetMainCam()->m_RefPos.z -= fStep;
}

inline void camSetAngle(float fAngle)
{
    //g_pGameApp->GetMainCam()->InitAngle(fAngle);
}

inline std::tuple<float, float> camGetCenter()
{
    auto* cam = g_pGameApp->GetMainCam();
    return {cam->m_RefPos.x, cam->m_RefPos.y};
}
