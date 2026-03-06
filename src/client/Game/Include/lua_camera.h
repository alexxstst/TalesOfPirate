#pragma once

//-----------------
// 
//-----------------
inline int lua_camGetCenter(lua_State *L)
{
    lua_pushnumber(L, g_pGameApp->GetMainCam()->m_RefPos.x);
    lua_pushnumber(L, g_pGameApp->GetMainCam()->m_RefPos.y);
    return 2;
}

inline int lua_camSetCenter(lua_State *L)
{
    // 
    BOOL bValid = (lua_gettop (L)==2 && lua_isnumber(L, 1) && lua_isnumber(L,2)); 
    if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }
    float x = (float)lua_tonumber(L, 1);
    float y = (float)lua_tonumber(L, 2);
    float ref_x = g_pGameApp->GetMainCam()->m_RefPos.x;
    float ref_y = g_pGameApp->GetMainCam()->m_RefPos.y;
    float dis_x = x - ref_x;
    float dis_y = y - ref_y;
    g_pGameApp->GetMainCam()->m_RefPos.x = x; 
    g_pGameApp->GetMainCam()->m_RefPos.y = y;
    g_pGameApp->GetMainCam()->m_EyePos.x += dis_x; 
    g_pGameApp->GetMainCam()->m_EyePos.y += dis_y;
    return 0;
}

inline int lua_camFollow(lua_State *L)
{
    // 
    BOOL bValid = (lua_gettop (L)==1 && lua_isnumber(L, 1)); 
    if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }

    BOOL bEnable = (BOOL)(lua_tonumber(L, 1));
    g_pGameApp->EnableCameraFollow(bEnable);
    return 0;
}

inline int lua_camMoveForward(lua_State *L)
{
    BOOL bValid = (lua_gettop (L)==2 && lua_isnumber(L, 1) && lua_isnumber(L,2)); 
    if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }
    float fStep = (float)lua_tonumber(L, 1);
    BOOL bHang = (BOOL)lua_tonumber(L, 2);

    g_pGameApp->GetMainCam()->MoveForward(fStep, bHang);
    return 0;
}

inline int lua_camMoveLeft(lua_State *L)
{
    BOOL bValid = (lua_gettop (L)==2 && lua_isnumber(L, 1) && lua_isnumber(L,2)); 
    if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }
    float fStep = (float)lua_tonumber(L, 1);
    BOOL bHang = (BOOL)lua_tonumber(L, 2);

    g_pGameApp->GetMainCam()->MoveRight(fStep, bHang);
    return 0;
}

inline int lua_camSetAngle(lua_State *L)
{
    BOOL bValid = (lua_gettop (L)==1 && lua_isnumber(L, 1));
    if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }
    float fAngle = (float)lua_tonumber(L, 1);
    //g_pGameApp->GetMainCam()->InitAngle(fAngle);
    return 0;
}

inline int lua_camMoveUp(lua_State *L)
{
    BOOL bValid = (lua_gettop (L)==1 && lua_isnumber(L, 1));
    if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }
    float fStep = (float)lua_tonumber(L, 1);
    g_pGameApp->GetMainCam()->m_RefPos.z-=fStep;
    return 0;
}
