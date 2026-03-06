#pragma once



/*

key_0 key_1 .... key_shift key_ctrl key_alt....

is_key_down(key)
is_left_mbutton_down()
is_right_mbutton_down()
is_mid_mbutton_down()

*/

inline int lua_IsKeyDown(lua_State *L)
{
    BOOL bValid = (lua_gettop (L)==1 && lua_isnumber (L, 1));
    if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }
   
    BYTE btKey = (BYTE)lua_tonumber(L, 1);
    if(g_pGameApp->IsKeyContinue(btKey))
    {
        lua_pushnumber(L, 1);
    }
    else
    {
        lua_pushnumber(L, 0);
    }
    return 1;
}
