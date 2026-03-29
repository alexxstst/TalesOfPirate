#pragma once

// --- Typed functions (LuaBridge auto-marshaling) ---

inline void MsgBox(const std::string& content)
{
    MessageBox(NULL, content.c_str(), "msg", 0);
}

inline int GetTickCount_Lua()
{
    return (int)GetTickCount();
}

inline int Rand_Lua(int nRange)
{
    return rand() % nRange;
}

inline void ToDebugLog(const std::string& message)
{
    g_logManager.InternalLog(LogLevel::Debug, "lua", message);
}
