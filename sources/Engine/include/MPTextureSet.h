#pragma once

#include "TextureManager.h"
#include "MPRender.h"

#pragma warning(disable: 4275)

// MPTexInfo сохранён для обратной совместимости.
// Не используется напрямую — TextureManager::Entry заменяет его.
struct MPTexInfo {
	BYTE  bAlpha{0};
	BYTE  btMipmap{5};
	short sWidth{0};
	short sHeight{0};
};


inline lwITex* GetTexByID(int nID, BOOL bRequest = FALSE)
{
	return TextureManager::I()->GetTexture(nID, bRequest != FALSE);
}

inline IDirect3DTextureX* GetTextureByID(int nID, BOOL bRequest = FALSE)
{
	return TextureManager::I()->GetD3DTexture(nID, bRequest != FALSE);
}

inline SIZE GetTextureSizeByID(int nID)
{
	return TextureManager::I()->GetSize(nID);
}

inline int GetTextureID(const char *pszDataName)
{
	if (TextureManager::I())
		return TextureManager::I()->GetOrCreateID(pszDataName);
	return 0;
}

inline TextureManager::Entry* GetTextureInfo(int nID)
{
	if (TextureManager::I())
		return TextureManager::I()->GetInfo(nID);
	return nullptr;
}

inline BOOL IsAlphaTexture(int nID)
{
	return TextureManager::I() && TextureManager::I()->IsAlpha(nID);
}

inline BOOL IsTextureExist(const char *pszDataName)
{
	return Util_IsFileExist((char*)pszDataName);
}

#pragma warning(default: 4275)
