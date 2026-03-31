#pragma once

#include "MindPowerAPI.h"
#include "lwInterface.h"
#include <string>
#include <vector>
#include <unordered_map>

LW_USING

// Менеджер текстур — замена MPTexSet (без наследования от CRawDataSet).
// Lazy loading: текстура загружается при первом обращении через GetTexture().
// DynamicRelease(): периодически выгружает неиспользуемые текстуры.
class MINDPOWER_API TextureManager {
public:
	struct Entry {
		std::string path;
		BYTE bAlpha{0};
		BYTE btMipmap{5};
		short sWidth{0};
		short sHeight{0};
		lwITex* pTex{nullptr};
		DWORD dwLastUseTick{0};
		DWORD dwLoadCnt{0};
	};

	TextureManager();
	~TextureManager();

	static TextureManager* I() { return _Instance; }

	// Получить ID по имени файла. Создаёт запись если нет.
	int GetOrCreateID(const char* path);

	// Lazy load — загружает текстуру при первом обращении.
	lwITex* GetTexture(int id, bool bRequest = false);

	// Convenience — возвращает DirectX текстуру.
	IDirect3DTextureX* GetD3DTexture(int id, bool bRequest = false);

	// Информация о текстуре.
	Entry* GetInfo(int id);
	Entry* GetInfo(const char* name);

	bool IsAlpha(int id);
	SIZE GetSize(int id);

	// Выгрузка неиспользуемых текстур.
	void DynamicRelease(bool bClearAll = false);

	// Перезагрузка всех текстур (смена качества).
	void ReloadAll();
	void SetLevel(int level);
	int  GetLevel() const { return _nTextureLevel; }

	void SetReleaseInterval(DWORD ms) { _dwReleaseInterval = ms; }

private:
	lwITex* LoadTexture(Entry& entry);
	void    ReleaseTexture(Entry& entry);
	std::string ResolveTexturePath(const char* filename);
	bool    IsMemoryFull();

	static TextureManager* _Instance;

	std::vector<Entry> _entries;
	std::unordered_map<std::string, int> _nameIndex;

	int   _nTextureLevel;
	DWORD _dwReleaseInterval{8000};
	int   _nLoadedCount{0};
};
