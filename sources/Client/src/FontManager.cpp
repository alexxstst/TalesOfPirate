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

#include <ft2build.h>
#include FT_FREETYPE_H

#include "fontstash.h"
#include "FonsDx9Backend.h"
#include "MPResManger.h"

#include <fstream>

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

	// Общий FT_Library на процесс. Инициализируется при первом InstallFontFile.
	// Явного FT_Done_FreeType нет — leak на shutdown безвреден и безопаснее, чем
	// состязания с FontRender, который может держать FT_Face дольше FontManager.
	FT_Library& FtLibrary() {
		static FT_Library lib = nullptr;
		if (!lib) {
			if (FT_Init_FreeType(&lib) != 0) {
				ToLogService("errors", LogLevel::Error,
							 "FontManager: FT_Init_FreeType failed");
				lib = nullptr;
			}
		}
		return lib;
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
	if (added <= 0) {
		ToLogService("errors", LogLevel::Warning,
					 "FontManager::InstallFontFile: AddFontResourceExA('{}') вернул 0",
					 path);
		return false;
	}
	_registeredPaths.push_back(path);

	// Читаем family_name через FreeType — чтобы FontRender потом открыл тот же
	// TTF напрямую. TTC/коллекции содержат несколько фейсов; регистрируем все.
	FT_Library lib = FtLibrary();
	if (lib) {
		FT_Long numFaces = 1;
		for (FT_Long i = 0; i < numFaces; ++i) {
			FT_Face face = nullptr;
			if (FT_New_Face(lib, path.c_str(), i, &face) != 0) {
				break;
			}
			if (i == 0) {
				numFaces = face->num_faces;
			}
			if (face->family_name && *face->family_name) {
				_familyToPath.emplace(std::string{face->family_name}, path);
			}
			FT_Done_Face(face);
		}
	}

	ToLogService("common",
				 "FontManager: '{}' зарегистрирован ({} фейсов)",
				 path, added);
	return true;
}

const std::string* FontManager::GetFontFilePath(const std::string& family) const {
	const auto it = _familyToPath.find(family);
	return (it == _familyToPath.end()) ? nullptr : &it->second;
}

std::string FontManager::FormatRegisteredFamilies() const {
	if (_familyToPath.empty()) {
		return " <нет зарегистрированных семейств — InstallFontFile/InstallFontsFromDir не вызывался?>";
	}
	std::string result;
	result.reserve(_familyToPath.size() * 64);
	for (const auto& [family, path] : _familyToPath) {
		result += std::format("\n  '{}' -> '{}'", family, path);
	}
	return result;
}

FONScontext* FontManager::GetFonsContext() {
	if (_fons) {
		return _fons;
	}
	// fontstash-бэкенд использует fixed-function DX9 (без Effect'а), поэтому
	// достаточно валидного g_Render. Effect/Technique оставлены в структуре
	// как задел, но на рендер не влияют.
	_fonsBackend = std::make_unique<fons::Dx9Backend>();
	_fonsBackend->Dev       = &g_Render;
	_fonsBackend->Effect    = nullptr;
	_fonsBackend->Technique = 0;
	// Стартовый размер атласа. fontstash сам расширяет при переполнении через
	// fonsExpandAtlas → renderResize.
	_fons = fons::CreateContext(_fonsBackend.get(), 512, 512);
	if (!_fons) {
		ToLogService("errors", LogLevel::Error,
					 "FontManager::GetFonsContext: fons::CreateContext failed");
		_fonsBackend.reset();
		return nullptr;
	}
	ToLogService("common",
				 "FontManager: FONScontext создан (512x512, DX9-бэкенд, fixed-function)");
	return _fons;
}

int FontManager::GetOrRegisterFonsFont(const std::string& ttfPath) {
	if (ttfPath.empty()) {
		return -1;
	}
	const auto it = _pathToFonsFontId.find(ttfPath);
	if (it != _pathToFonsFontId.end()) {
		return it->second;
	}
	FONScontext* fons = GetFonsContext();
	if (!fons) {
		return -1;
	}
	// Читаем TTF целиком в буфер. fontstash хранит указатель — буфер должен
	// жить, пока жив FONScontext. _fontBuffers хранит владение.
	std::ifstream f(ttfPath, std::ios::binary | std::ios::ate);
	if (!f) {
		ToLogService("errors", LogLevel::Warning,
					 "FontManager::GetOrRegisterFonsFont: не открыт '{}'",
					 ttfPath);
		return -1;
	}
	const auto size = static_cast<std::streamsize>(f.tellg());
	if (size <= 0) {
		return -1;
	}
	std::vector<unsigned char> buf(static_cast<size_t>(size));
	f.seekg(0, std::ios::beg);
	f.read(reinterpret_cast<char*>(buf.data()), size);
	if (!f) {
		ToLogService("errors", LogLevel::Warning,
					 "FontManager::GetOrRegisterFonsFont: чтение '{}' failed",
					 ttfPath);
		return -1;
	}

	// Имя в fontstash — путь к файлу (уникален). freeData=0: fontstash не
	// владеет буфером, FontManager держит его сам.
	_fontBuffers.emplace_back(std::move(buf));
	auto& stored = _fontBuffers.back();
	const int id = fonsAddFontMem(fons, ttfPath.c_str(),
								  stored.data(),
								  static_cast<int>(stored.size()), 0);
	if (id == FONS_INVALID) {
		ToLogService("errors", LogLevel::Warning,
					 "FontManager::GetOrRegisterFonsFont: fonsAddFontMem('{}') failed",
					 ttfPath);
		_fontBuffers.pop_back();
		return -1;
	}
	_pathToFonsFontId.emplace(ttfPath, id);

	// Вычисление SizeScale = (ascender - descender) / units_per_EM.
	// Открываем face отдельно через FreeType (fontstash не экспонирует).
	float sizeScale = 1.0f;
	FT_Library lib = FtLibrary();
	if (lib) {
		FT_Face face = nullptr;
		if (FT_New_Memory_Face(lib, stored.data(),
							   static_cast<FT_Long>(stored.size()),
							   0, &face) == 0) {
			if (face->units_per_EM > 0) {
				const float fh = static_cast<float>(
					face->ascender - face->descender);
				const float em = static_cast<float>(face->units_per_EM);
				if (em > 0.0f) {
					sizeScale = fh / em;
				}
			}
			FT_Done_Face(face);
		}
	}
	_fonsIdToSizeScale.emplace(id, sizeScale);

	ToLogService("common",
				 "FontManager: '{}' → fontstash id={} (sizeScale={:.4f})",
				 ttfPath, id, sizeScale);
	return id;
}

float FontManager::GetFonsSizeScale(int fonsFontId) const {
	const auto it = _fonsIdToSizeScale.find(fonsFontId);
	return (it == _fonsIdToSizeScale.end()) ? 1.0f : it->second;
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
	// Если family зарегистрирован через InstallFontFile — получаем путь к TTF
	// и регистрируем его в общем fontstash → FontRender рисует через fonsDrawText.
	// Иначе — пустой путь + FONS_INVALID, FontRender откатится на прямой FreeType
	// или GDI-путь.
	const std::string* ttfPath    = GetFontFilePath(family);
	const std::string  path       = ttfPath ? *ttfPath : std::string{};
	FONScontext*       fons       = path.empty() ? nullptr : GetFonsContext();
	const int          fonsFontId = path.empty() ? -1 : GetOrRegisterFonsFont(path);
	const float        sizeScale  = GetFonsSizeScale(fonsFontId);
	const bool ok = font->CreateFont(&g_Render,
									 const_cast<char*>(family.c_str()),
									 size, level, static_cast<DWORD>(flags),
									 path, fons, fonsFontId, sizeScale);
	if (!ok) {
		ToLogService("errors", LogLevel::Error,
					 "FontManager::CreateFont('{}', family='{}', size={}): CreateFont failed. "
					 "Зарегистрированные семейства:{}",
					 name, family, size, FormatRegisteredFamilies());
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
	// Порядок: сначала CMPFont'ы (они могут ссылаться на FONScontext), потом
	// удаляем FONScontext, только затем освобождаем буферы TTF.
	_fonts.clear();
	_fontNames.clear();
	_byName.clear();
	if (_fons) {
		fonsDeleteInternal(_fons);
		_fons = nullptr;
	}
	_fonsBackend.reset();
	_pathToFonsFontId.clear();
	_fontBuffers.clear();
	for (const auto& path : _registeredPaths) {
		RemoveFontResourceExA(path.c_str(), FR_PRIVATE, nullptr);
	}
}
