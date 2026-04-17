#include "stdafx.h"
#include "FontManager.h"

#include "IniFile.h"
#include "logutil.h"

#include <filesystem>

extern "C" {
#include "lua.h"
}

namespace {
	struct FontFamilyEntry {
		std::string              IniKey;
		std::string              SubDir;
		std::string              GdiFamily;
		std::vector<std::string> Files;
	};

	const std::vector<FontFamilyEntry>& FamilyTable() {
		static const std::vector<FontFamilyEntry> table = {
			{ "PTSans",      "PTSans",      "PT Sans",
			  { "PT_Sans-Regular.ttf", "PT_Sans-Bold.ttf" } },
			{ "OpenSans",    "OpenSans",    "Open Sans",
			  { "OpenSans-Regular.ttf", "OpenSans-Bold.ttf" } },
			{ "Roboto",      "Roboto",      "Roboto",
			  { "Roboto-Regular.ttf", "Roboto-Bold.ttf" } },
			{ "NotoSans",    "NotoSans",    "Noto Sans",
			  { "NotoSans-Regular.ttf", "NotoSans-Bold.ttf" } },
			{ "SourceSans3", "SourceSans3", "Source Sans 3",
			  { "SourceSans3-Regular.ttf", "SourceSans3-Bold.ttf" } },
			{ "Inter",       "Inter",       "Inter",
			  { "Inter-Regular.ttf", "Inter-Bold.ttf" } },
		};
		return table;
	}

	const FontFamilyEntry* FindByIniKey(const std::string& key) {
		for (const auto& entry : FamilyTable()) {
			if (entry.IniKey == key) {
				return &entry;
			}
		}
		return nullptr;
	}
} // namespace

FontManager& FontManager::Instance() {
	static FontManager inst;
	return inst;
}

void FontManager::Init(dbc::IniFile& ini) {
	// Снять предыдущие регистрации, если Init вызывается повторно.
	for (const auto& path : _registeredPaths) {
		RemoveFontResourceExA(path.c_str(), FR_PRIVATE, nullptr);
	}
	_registeredPaths.clear();
	_resolvedFamily = "Arial";

	const std::string fontDir    = ini["Fonts"].GetString("FontDir",    "./font");
	const std::string systemFont = ini["Fonts"].GetString("SystemFont", "PTSans");

	const FontFamilyEntry* entry = FindByIniKey(systemFont);
	if (entry == nullptr) {
		ToLogService("common", LogLevel::Warning,
					 "FontManager: SystemFont='{}' не в таблице, fallback '{}'",
					 systemFont, _resolvedFamily);
		return;
	}

	namespace fs = std::filesystem;
	const fs::path dir = fs::path{fontDir} / entry->SubDir;

	std::vector<std::string> registered;
	for (const auto& file : entry->Files) {
		const fs::path full = dir / file;
		if (!fs::exists(full)) {
			ToLogService("common", LogLevel::Warning,
						 "FontManager: файл '{}' отсутствует, регистрация пропущена",
						 full.string());
			continue;
		}
		const std::string fullStr = full.string();
		const int added = AddFontResourceExA(fullStr.c_str(), FR_PRIVATE, nullptr);
		if (added > 0) {
			registered.push_back(fullStr);
			ToLogService("common",
						 "FontManager: '{}' зарегистрирован ({} фейсов)",
						 fullStr, added);
		}
		else {
			ToLogService("common", LogLevel::Warning,
						 "FontManager: AddFontResourceExA('{}') вернул 0",
						 fullStr);
		}
	}

	if (registered.empty()) {
		ToLogService("common", LogLevel::Warning,
					 "FontManager: ни один файл семейства '{}' не зарегистрирован, fallback '{}'",
					 entry->IniKey, _resolvedFamily);
		return;
	}

	_registeredPaths = std::move(registered);
	_resolvedFamily  = entry->GdiFamily;
	ToLogService("common", "FontManager: system font = '{}'", _resolvedFamily);
}

void FontManager::PushToLua(lua_State* L) const {
	if (L == nullptr) {
		return;
	}
	lua_pushlstring(L, _resolvedFamily.data(), _resolvedFamily.size());
	lua_setglobal(L, "g_SystemFont");
}

FontManager::~FontManager() {
	for (const auto& path : _registeredPaths) {
		RemoveFontResourceExA(path.c_str(), FR_PRIVATE, nullptr);
	}
}
