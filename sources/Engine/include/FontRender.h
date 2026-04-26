#pragma once

// FontRender — тонкая обёртка над общим fontstash-контекстом.
// Пайплайн:
//   - Регистрация шрифта и FONScontext — в FontManager (lazy init).
//   - FontRender хранит только fonsFontId + указатель на FONScontext.
//   - DrawText/GetTextSize идут через fonsDrawText/fonsTextBounds; атлас,
//     растеризация (FreeType через FONS_USE_FREETYPE) и draw-call — внутри
//     fontstash + DX9-бэкенда (см. fons::Dx9Backend).
// из fontstash через fonsVertMetrics / fonsTextBounds при CreateFont.

#include <d3d9.h>
#include <d3dx9math.h>
#include <string>
#include <unordered_set>

#include "lwHeader.h"

class MPRender;
class CMPResManger;

// Forward для fontstash (owned by FontManager).
struct FONScontext;

#ifndef MPFONT_BOLD
#define MPFONT_BOLD   0x0001
#define MPFONT_ITALIC 0x0002
#define MPFONT_UNLINE 0x0004
#endif

class FontRender {
public:
	FontRender();
	~FontRender();

	FontRender(const FontRender&) = delete;
	FontRender& operator=(const FontRender&) = delete;

	// Инициализация: запоминает размер, путь TTF (для логов), FONScontext +
	// fonsFontId от FontManager. sizeScale = (asc-desc)/em данного шрифта —
	// множитель, который нужно применить к nSize перед fonsSetSize, чтобы
	// получить em-size=nSize (GDI-совместимая интерпретация).
	// Возвращает false при невалидных аргументах или отсутствии fontstash.
	bool CreateFont(MPRender* pd3dDevice, char* szFontName,
					int nSize = 16, int nLevel = 3, DWORD dwFlag = 0,
					const std::string& ttfPath = {},
					FONScontext* fons = nullptr, int fonsFontId = -1,
					float sizeScale = 1.0f);

	// Legacy-заглушка: в старом GDI/Effect-пути здесь сохранялся CMPEffectFile.
	void BindingRes(CMPResManger*) {
	}

	void Begin() {
	}

	void End() {
	}

	void BeginClip() {
	}

	void EndClip() {
	}

	void Draw(char* szText, int x, int y, D3DXCOLOR color);
	void DrawTextClipOnce(char* szText, int nLen, LPRECT psrc, LPRECT pclip,
						  D3DXCOLOR color);

	bool DrawText(char* szText, int x, int y,
				  D3DXCOLOR color = 0xFFFFFFFF, float fScale = 1.0f,
				  DWORD* dwTime = nullptr);
	bool DrawText(int iNumber, int x, int y,
				  D3DXCOLOR color = 0xFFFFFFFF, float fScale = 1.0f);

	bool DrawTextShadow(char* szText, int x1, int y1, int x2, int y2,
						D3DXCOLOR color1, D3DXCOLOR color2);

	bool Draw3DText(char* szText, D3DXVECTOR3& vPos,
					D3DXCOLOR color = 0xFFFFFFFF, float fScale = 0.3f);

	SIZE* GetTextSize(const std::string& szText, SIZE* pSize, float fScale = 1.0f);
	int GetHzLength(float fscale = 1.0f);
	int GetAscLength(float fscale = 1.0f);

	void ReleaseFont();

	// Кодовая страница для MultiByteToWideChar при DrawText. По умолчанию
	// CP_UTF8; оставлена для Console-слота и legacy call-sites.
	void SetCodepage(UINT codepage) {
		_codepage = codepage;
	}

	UINT GetCodepage() const {
		return _codepage;
	}

	// Диагностический дамп атласа fontstash: чекерборд + альфа из атласа
	// (glyph-cache). Один BMP на весь FONScontext.
	bool DumpGlyphPreview(const std::string& path);
	bool DumpAtlas(const std::string& path);

	// Toggle'ы, управляющие Lua-скриптами через UIScript биндинги.
	static void SetTextDumpEnabled(bool enabled) {
		s_textDumpEnabled = enabled;
	}

	static bool IsTextDumpEnabled() {
		return s_textDumpEnabled;
	}

	// DrawTextShadow по умолчанию рендерит текст дважды (shadow+main). При
	// false — только main. Используется для диагностики "жирности" текста.
	static void SetShadowEnabled(bool enabled) {
		s_shadowEnabled = enabled;
	}

	static bool IsShadowEnabled() {
		return s_shadowEnabled;
	}

private:
	std::wstring _ToWide(const char* mbstr) const;
	std::string _ToUtf8(const std::wstring& w) const;
	void _DrawWide(const std::wstring& wtext, int x, int y,
				   D3DXCOLOR color, float fScale,
				   const RECT* clipRect = nullptr);

private:
	MPRender* _dev{nullptr};

	// fontstash-путь (единственный). FONScontext владеет FontManager.
	FONScontext* _fons{nullptr};
	int _fonsFontId{-1};
	// Множитель для fonsSetSize = (asc-desc)/em шрифта, чтобы em-size совпадал
	// с GDI-интерпретацией lfHeight. Для шрифтов с em==fh (редкость) = 1.0f.
	float _sizeScale{1.0f};

	int _textSize{0};

	// Метрики строки, вычисленные через fonsVertMetrics / fonsTextBounds.
	int _lineHeight{0};
	int _baseline{0};
	int _avgCharW{0};
	int _spaceAdvance{0};

	std::string _fontName;
	UINT _codepage{CP_UTF8};

	// Дедуп для debug-дампа (оставлен как no-op в будущих режимах).
	std::unordered_set<std::wstring> _dumpedTexts;

	static bool s_textDumpEnabled;
	static bool s_shadowEnabled;
};
