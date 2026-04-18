#include "stdafx.h"
#include "FontManager.h"

#include "IniFile.h"
#include "logutil.h"
#include "MPFont.h"
#include "MPRender.h"
#include "GlobalVar.h"

#include <algorithm>
#include <array>
#include <cctype>
#include <filesystem>
#include <format>

extern "C" {
#include "lua.h"
}

extern CMPResManger ResMgr;

namespace {
	// IniKey → GDI family name. Если значение SystemFont в ini не
	// совпадает ни с одним ключом — используется как есть (ожидается,
	// что это уже GDI family name вида "PT Sans").
	struct FamilyAlias {
		std::string_view IniKey;
		std::string_view GdiFamily;
	};

	constexpr std::array kFamilyAliases{
		FamilyAlias{ "PTSans",      "PT Sans"       },
		FamilyAlias{ "OpenSans",    "Open Sans"     },
		FamilyAlias{ "Roboto",      "Roboto"        },
		FamilyAlias{ "NotoSans",    "Noto Sans"     },
		FamilyAlias{ "SourceSans3", "Source Sans 3" },
		FamilyAlias{ "Inter",       "Inter"         },
	};

	std::string ResolveGdiFamily(const std::string& iniValue) {
		for (const auto& a : kFamilyAliases) {
			if (iniValue == a.IniKey) {
				return std::string{a.GdiFamily};
			}
		}
		return iniValue; // уже GDI family name либо кастомное
	}

	bool IsFontFile(const std::filesystem::path& p) {
		const auto ext = p.extension().string();
		std::string lower;
		lower.reserve(ext.size());
		for (char c : ext) {
			lower.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(c))));
		}
		return lower == ".ttf" || lower == ".ttc" || lower == ".otf";
	}
} // namespace

FontManager& FontManager::Instance() {
	static FontManager inst;
	return inst;
}

void FontManager::Init(dbc::IniFile& ini) {
	const std::string systemFont = ini["Fonts"].GetString("SystemFont", "PTSans");
	_resolvedFamily = ResolveGdiFamily(systemFont);
	if (_resolvedFamily.empty()) {
		_resolvedFamily = "Arial";
	}
	ToLogService("common", "FontManager: resolved family = '{}'", _resolvedFamily);
}

void FontManager::PushToLua(lua_State* L) const {
	if (L == nullptr) {
		return;
	}
	lua_pushlstring(L, _resolvedFamily.data(), _resolvedFamily.size());
	lua_setglobal(L, "g_SystemFont");
}

bool FontManager::InstallFontFile(const std::filesystem::path& ttf) {
	namespace fs = std::filesystem;
	if (!fs::exists(ttf)) {
		ToLogService("errors", LogLevel::Warning,
					 "FontManager::InstallFontFile: файл '{}' не найден",
					 ttf.string());
		return false;
	}
	const std::string path = ttf.string();
	const int added = AddFontResourceExA(path.c_str(), FR_PRIVATE, nullptr);
	if (added > 0) {
		_registeredPaths.push_back(path);
		ToLogService("common",
					 "FontManager: '{}' зарегистрирован ({} фейсов)",
					 path, added);
		return true;
	}
	ToLogService("errors", LogLevel::Warning,
				 "FontManager::InstallFontFile: AddFontResourceExA('{}') вернул 0",
				 path);
	return false;
}

int FontManager::InstallFontsFromDir(const std::filesystem::path& dir) {
	namespace fs = std::filesystem;
	if (!fs::exists(dir) || !fs::is_directory(dir)) {
		ToLogService("errors", LogLevel::Warning,
					 "FontManager::InstallFontsFromDir: директория '{}' не найдена",
					 dir.string());
		return 0;
	}
	int count = 0;
	for (const auto& entry : fs::recursive_directory_iterator(dir)) {
		if (!entry.is_regular_file()) {
			continue;
		}
		if (!IsFontFile(entry.path())) {
			continue;
		}
		if (InstallFontFile(entry.path())) {
			++count;
		}
	}
	ToLogService("common",
				 "FontManager::InstallFontsFromDir('{}'): установлено {} шрифт(ов)",
				 dir.string(), count);
	return count;
}

int FontManager::_CurrentSize(int size800, int size1024) {
	const int w = g_Render.GetScrWidth();
	return (w <= TINY_RES_X) ? size800 : size1024;
}

int FontManager::CreateFont(std::string name, const std::string& family,
							int size800, int size1024, int level,
							unsigned long flags) {
	if (name.empty()) {
		ToLogService("errors", LogLevel::Error,
					 "FontManager::CreateFont: пустое имя");
		return -1;
	}

	const int size = _CurrentSize(size800, size1024);
	auto font = std::make_unique<CMPFont>();
	const bool ok = font->CreateFont(&g_Render,
									 const_cast<char*>(family.c_str()),
									 size, level, static_cast<DWORD>(flags));
	if (!ok) {
		ToLogService("errors", LogLevel::Error,
					 "FontManager::CreateFont('{}', family='{}', size={}): CreateFont failed",
					 name, family, size);
		return -1;
	}
	font->BindingRes(&ResMgr);

	const auto it = _byName.find(name);
	if (it != _byName.end()) {
		ToLogService("common", LogLevel::Warning,
					 "FontManager::CreateFont: имя '{}' уже зарегистрировано, заменяю",
					 name);
		const int idx = it->second;
		_fonts[idx] = std::move(font);
		_fontNames[idx] = std::move(name);
		return idx;
	}

	const int idx = static_cast<int>(_fonts.size());
	_fonts.push_back(std::move(font));
	_fontNames.push_back(name);
	_byName.emplace(std::move(name), idx);
	return idx;
}

int FontManager::FindHandle(const std::string& name) const {
	const auto it = _byName.find(name);
	return (it == _byName.end()) ? -1 : it->second;
}

CMPFont* FontManager::Get(int handle) {
	if (handle < 0 || handle >= static_cast<int>(_fonts.size())) {
		ToLogService("errors", LogLevel::Error,
					 "FontManager::Get(handle={}): out of range (size={})",
					 handle, _fonts.size());
		return nullptr;
	}
	return _fonts[handle].get();
}

CMPFont* FontManager::Get(const std::string& name) {
	const int idx = FindHandle(name);
	if (idx < 0) {
		ToLogService("errors", LogLevel::Error,
					 "FontManager::Get('{}'): не зарегистрирован",
					 name);
		return nullptr;
	}
	return _fonts[idx].get();
}

CMPFont* FontManager::Get(FontSlot slot) {
	const std::string name{SlotName(slot)};
	int idx = FindHandle(name);
	if (idx < 0) {
		ToLogService("errors", LogLevel::Error,
					 "FontManager::Get(FontSlot::{}): слот не сконфигурирован, "
					 "auto-create с дефолтами (family='{}', size=12, level=1)",
					 name, _resolvedFamily);
		idx = CreateFont(name, _resolvedFamily, 12, 12, 1, 0);
		if (idx < 0) {
			return nullptr;
		}
	}
	return _fonts[idx].get();
}

std::string_view FontManager::SlotName(FontSlot slot) {
	switch (slot) {
	case FontSlot::TipText:      return "TipText";
	case FontSlot::MidAnnounce:  return "MidAnnounce";
	case FontSlot::BottomShadow: return "BottomShadow";
	case FontSlot::Console:      return "Console";
	default:                     return "Unknown";
	}
}

void FontManager::DumpAllSlots(const std::filesystem::path& dir) {
	namespace fs = std::filesystem;
	std::error_code ec;
	fs::create_directories(dir, ec);
	for (size_t i = 0; i < _fonts.size(); ++i) {
		if (!_fonts[i]) {
			continue;
		}
		// Санитизация имени в filename (пробелы и др. → _).
		std::string sanitized;
		sanitized.reserve(_fontNames[i].size());
		for (char c : _fontNames[i]) {
			sanitized += (std::isalnum(static_cast<unsigned char>(c))
						  || c == '-' || c == '_') ? c : '_';
		}
		const fs::path path = dir / std::format("font_dump_{}.bmp", sanitized);
		if (!_fonts[i]->DumpGlyphPreview(path.string())) {
			ToLogService("errors", LogLevel::Warning,
						 "FontManager::DumpAllSlots: dump '{}' failed",
						 path.string());
		}
	}
}

void FontManager::DumpAllAtlases(const std::filesystem::path& dir) {
	namespace fs = std::filesystem;
	std::error_code ec;
	fs::create_directories(dir, ec);
	for (size_t i = 0; i < _fonts.size(); ++i) {
		if (!_fonts[i]) {
			continue;
		}
		std::string sanitized;
		sanitized.reserve(_fontNames[i].size());
		for (char c : _fontNames[i]) {
			sanitized += (std::isalnum(static_cast<unsigned char>(c))
						  || c == '-' || c == '_') ? c : '_';
		}
		const fs::path path = dir / std::format("atlas_{}.bmp", sanitized);
		if (!_fonts[i]->DumpAtlas(path.string())) {
			ToLogService("errors", LogLevel::Warning,
						 "FontManager::DumpAllAtlases: dump '{}' failed",
						 path.string());
		}
	}
}

void FontManager::ClearFonts() {
	_fonts.clear();
	_fontNames.clear();
	_byName.clear();
}

void FontManager::SetAllCodepage(unsigned int codepage) {
	for (auto& fp : _fonts) {
		if (fp) {
			fp->SetCodepage(static_cast<UINT>(codepage));
		}
	}
}

FontManager::~FontManager() {
	_fonts.clear();
	_fontNames.clear();
	_byName.clear();
	for (const auto& path : _registeredPaths) {
		RemoveFontResourceExA(path.c_str(), FR_PRIVATE, nullptr);
	}
}
