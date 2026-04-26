// SAFE_DELETE/RELEASE macros — единственное живое содержимое старого DXUtil.h.
// Все остальные DXUtil_* и DEBUG_MSG/DXTRACE удалены как dead code (см. историю).
#pragma once

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
#endif
