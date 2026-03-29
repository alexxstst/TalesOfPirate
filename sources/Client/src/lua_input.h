#pragma once

// ---   (LuaBridge ) ---

inline int IsKeyDown(int key)
{
    BYTE btKey = (BYTE)key;
    if(g_pGameApp->IsKeyContinue(btKey))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
