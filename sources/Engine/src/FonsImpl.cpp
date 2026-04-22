// Реализационный TU fontstash.h — ровно один на Engine. Здесь же
// включается FreeType-бэкенд (FONS_USE_FREETYPE) вместо встроенного stb_truetype.
//
// ВАЖНО: этот файл компилируется БЕЗ PCH stdafx.h (see MindPower3D.vcxproj),
// потому что fontstash.h в IMPL-режиме тянет свои заголовки C runtime и
// определяет макросы (FONS_MALLOC/FREE), которые не должны утекать в общий PCH.

#define FONTSTASH_IMPLEMENTATION
#define FONS_USE_FREETYPE

// fontstash.h в impl-режиме на Windows использует MAX_PATH / CP_UTF8 /
// MultiByteToWideChar (ветка _WIN32 в fons__tmpfopen). Минимальный include.
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "fontstash.h"
