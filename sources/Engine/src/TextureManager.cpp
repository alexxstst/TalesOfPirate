#include "StdAfx.h"
#include "TextureManager.h"
#include "MPRender.h"
#include "lwSysGraphics.h"
#include <logutil.h>

#include <sys/stat.h>

extern MPRender g_Render;

TextureManager* TextureManager::_Instance = nullptr;

TextureManager::TextureManager() {
	_Instance = this;
	_nTextureLevel = D3DX_DEFAULT;
}

TextureManager::~TextureManager() {
	DynamicRelease(true);
	_Instance = nullptr;
}

// ============================================================================
// GetOrCreateID — аналог CRawDataSet::GetRawDataID
// ============================================================================

int TextureManager::GetOrCreateID(std::string_view path) {
	if (path.empty()) return 0;

	// Нормализация: lowercase
	std::string key(path);
	for (auto& c : key) c = static_cast<char>(tolower(static_cast<unsigned char>(c)));

	auto it = _nameIndex.find(key);
	if (it != _nameIndex.end())
		return it->second;

	int id = static_cast<int>(_entries.size());
	_entries.push_back({});
	_entries.back().path = key;
	_nameIndex[key] = id;
	return id;
}

// ============================================================================
// GetTexture — lazy load
// ============================================================================

lwITex* TextureManager::GetTexture(int id, bool bRequest) {
	if (id < 0 || id >= static_cast<int>(_entries.size()))
		return nullptr;

	auto& entry = _entries[id];
	entry.dwLastUseTick = GetTickCount();

	if (!entry.pTex) {
		entry.pTex = LoadTexture(entry);
		if (entry.pTex) {
			entry.dwLoadCnt++;
			_nLoadedCount++;
		}
	}
	return entry.pTex;
}

IDirect3DTextureX* TextureManager::GetD3DTexture(int id, bool bRequest) {
	lwITex* tex = GetTexture(id, bRequest);
	return tex ? tex->GetTex() : nullptr;
}

// ============================================================================
// Info accessors
// ============================================================================

TextureManager::Entry* TextureManager::GetInfo(int id) {
	if (id < 0 || id >= static_cast<int>(_entries.size()))
		return nullptr;
	return &_entries[id];
}

TextureManager::Entry* TextureManager::GetInfo(const char* name) {
	if (!name) return nullptr;
	std::string key(name);
	for (auto& c : key) c = static_cast<char>(tolower(static_cast<unsigned char>(c)));
	auto it = _nameIndex.find(key);
	if (it == _nameIndex.end()) return nullptr;
	return &_entries[it->second];
}

bool TextureManager::IsAlpha(int id) {
	auto* e = GetInfo(id);
	return e && e->bAlpha == 1;
}

SIZE TextureManager::GetSize(int id) {
	SIZE sz = {0, 0};
	lwITex* tex = GetTexture(id);
	if (tex) {
		lwTexInfo info;
		tex->GetTexInfo(&info);
		sz.cx = info.width;
		sz.cy = info.height;
	}
	return sz;
}

// ============================================================================
// DynamicRelease — выгрузка неиспользуемых текстур по таймауту
// ============================================================================

void TextureManager::DynamicRelease(bool bClearAll) {
	DWORD dwCurTick = GetTickCount();

	for (auto& entry : _entries) {
		if (!entry.pTex) continue;

		if (bClearAll || (IsMemoryFull() && (dwCurTick - entry.dwLastUseTick) > _dwReleaseInterval)) {
			ReleaseTexture(entry);
		}
	}
}

bool TextureManager::IsMemoryFull() {
	DWORD dwMem = g_Render.GetRegisteredDevMemSize();
	return dwMem >= 64 * 1024 * 1024;
}

// ============================================================================
// Reload / Level
// ============================================================================

void TextureManager::ReloadAll() {
	for (auto& entry : _entries) {
		if (!entry.pTex) continue;
		ReleaseTexture(entry);
		entry.pTex = LoadTexture(entry);
		if (entry.pTex) {
			entry.dwLoadCnt++;
			_nLoadedCount++;
		}
	}
}

void TextureManager::SetLevel(int level) {
	if (_nTextureLevel != level) {
		_nTextureLevel = level;
		ReloadAll();
	}
}

// ============================================================================
// ReleaseTexture
// ============================================================================

void TextureManager::ReleaseTexture(Entry& entry) {
	if (!entry.pTex) return;

	ToLogService("common", "Release Texture [{}], size = {} {}", entry.path, entry.sWidth, entry.sHeight);

	if (entry.path.ends_with(".wsd")) {
		auto p = reinterpret_cast<char*>(entry.pTex->GetUserData());
		if (p) delete[] p;
	}

	entry.pTex->Release();
	entry.pTex = nullptr;
	_nLoadedCount--;
}

// ============================================================================
// ResolveTexturePath — резолв `.wsd` на оригинальный `.tga`/`.png`/`.bmp`/`.dds`
// ============================================================================
//
// Исторически UI-текстуры лежали как AES-GCM-`.wsd`, и engine при первой
// загрузке расшифровывал их в `.dec`. Миграция (AssetLoaderTests --decrypt-wsd,
// 2026-05-05) перенесла все 498 файлов обратно в исходные расширения; вызовы
// crypto::AesGcmDecryptFile удалены, `.wsd` больше нет в Client/texture/.
// Этот резолвер остаётся для совместимости с asset-references в коде, которые
// продолжают именовать файлы как `*.wsd`: подменяем на первое существующее
// `*.{tga,png,bmp,dds}`.

std::string TextureManager::ResolveTexturePath(const char* filename) {
	struct stat st;
	std::string path(filename);

	size_t len = path.length();
	if (len > 3 && path[len - 1] == 'd' && path[len - 2] == 's' && path[len - 3] == 'w') {
		const char* exts[] = {"tga", "png", "bmp", "dds"};
		for (auto ext : exts) {
			std::string origPath = path.substr(0, len - 3) + ext;
			if (stat(origPath.c_str(), &st) == 0)
				return origPath;
		}
	}

	return path;
}

// ============================================================================
// LoadTexture — загрузка через lwITex (managed texture)
// ============================================================================

lwITex* TextureManager::LoadTexture(Entry& entry) {
	lwIResourceMgr* res_mgr = g_Render.GetInterfaceMgr()->res_mgr;

	lwITex* tex = nullptr;
	res_mgr->CreateTex(&tex);

	lwTexInfo tex_info;
	lwTexInfo_Construct(&tex_info);

	std::string resolved = ResolveTexturePath(entry.path.c_str());
	_tcscpy(tex_info.file_name, resolved.c_str());
	_tcslwr(tex_info.file_name);

	tex_info.pool = D3DPOOL_MANAGED;
	tex_info.usage = 0;
	tex_info.level = D3DX_DEFAULT;
	tex_info.type = TEX_TYPE_FILE;

	D3DFORMAT tex_fmt[2];
	tex_fmt[0] = g_Render.GetTexSetFormat(0);
	tex_fmt[1] = g_Render.GetTexSetFormat(1);

	size_t l = _tcslen(tex_info.file_name);
	if (l > 3 && tex_info.file_name[l - 1] == 'a' && tex_info.file_name[l - 2] == 'g' && tex_info.file_name[l - 3] ==
		't') {
		tex_info.format = tex_fmt[1];
	}
	else {
		tex_info.format = tex_fmt[0];
	}

	if (_tcsstr(tex_info.file_name, "ui")) {
		tex_info.level = D3DX_DEFAULT;
		size_t str_len = _tcslen(tex_info.file_name);
		if (_tcsicmp(&tex_info.file_name[str_len - 3], "bmp") == 0) {
			tex_info.colorkey_type = COLORKEY_TYPE_COLOR;
			tex_info.colorkey.color = 0xffff00ff;
			tex_info.format = tex_fmt[1];
		}
	}

	if (LW_RESULT r = tex->LoadTexInfo(&tex_info, std::string_view{}); LW_FAILED(r)) {
		ToLogService("errors", LogLevel::Error,
					 "[{}] tex->LoadTexInfo failed: file={}, ret={}",
					 __FUNCTION__, tex_info.file_name, static_cast<long long>(r));
	}
	if (LW_RESULT r = tex->LoadVideoMemory(); LW_FAILED(r)) {
		ToLogService("errors", LogLevel::Error,
					 "[{}] tex->LoadVideoMemory failed: file={}, ret={}",
					 __FUNCTION__, tex_info.file_name, static_cast<long long>(r));
	}

	tex->GetTexInfo(&tex_info);

	entry.sWidth = static_cast<short>(tex_info.width);
	entry.sHeight = static_cast<short>(tex_info.height);

	ToLogService("common", "Load Texture [{}] size = {} {}", entry.path, entry.sWidth, entry.sHeight);
	return tex;
}
