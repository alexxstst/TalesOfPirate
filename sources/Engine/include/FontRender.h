#pragma once

// FontRender — per-glyph атлас-рендер замена CMPFont.
//
// Пайплайн:
//   - GDI рендерит глиф в DIB 32bpp (CLEARTYPE_QUALITY для 256-уровневой AA).
//   - Ищется tight bbox в DIB (max по RGB — ClearType распределяет intensity
//     по каналам).
//   - Глиф копируется в атлас D3DFMT_A8R8G8B8 (RGB=white, A=intensity).
//   - ABC-ширины через GetCharABCWidthsW дают точный advance per-glyph.
//   - Рендер — fullscreen triangles через CMPEffectFile::SetTechnique(5)
//     (alpha blend + modulate с diffuse, настраивается HLSL-техникой).
//
// Публичный API совместим с CMPFont — callsite'ы не меняются.

#include <d3d9.h>
#include <d3dx9math.h>
#include <windows.h>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "lwHeader.h"

class MPRender;
class CMPResManger;
class CMPEffectFile;

LW_BEGIN
class lwITex;
LW_END
using MindPower::lwITex;

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

	bool CreateFont(MPRender* pd3dDevice, char* szFontName,
					int nSize = 16, int nLevel = 3, DWORD dwFlag = 0);
	// Сохраняет CMPEffectFile — используется для настройки render-state
	// (blend/texstage/sampler) через HLSL-технику.
	void BindingRes(CMPResManger* pResMagr);

	void Begin() {}
	void End() {}

	void BeginClip() {}
	void EndClip() {}

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
	int   GetHzLength(float fscale = 1.0f);
	int   GetAscLength(float fscale = 1.0f);

	void ReleaseFont();

	void UseSoft(bool bUseSoft) { _useSoft = bUseSoft; }
	bool IsUseSoft()            { return _useSoft; }

	lwITex* GetTexture() { return _atlas; }

	void SetRenderIdx(int iIdx) { _renderIdx = iIdx; }

	// Диагностический дамп: рендерит набор глифов (0-9, A-Z, a-z, А-Я, а-я)
	// в отдельный DIB через GDI и сохраняет 32bpp BMP. Фон белый, текст чёрный.
	bool DumpGlyphPreview(const std::string& path);

	// Сохраняет содержимое D3D-атласа в BMP: альфа отображается поверх
	// серой шахматной сетки (чекерборд 8×8). Чёрный текст на светлом фоне.
	bool DumpAtlas(const std::string& path);

	// Глобальный toggle для дампа каждого рендеренного текста в
	// ./font/debug/render.<timestamp>.bmp. Dедупликация — per-instance
	// (каждый FontRender ведёт свой set уже задампленных wstring).
	static void SetTextDumpEnabled(bool enabled) { s_textDumpEnabled = enabled; }
	static bool IsTextDumpEnabled() { return s_textDumpEnabled; }

	// Toggle для shadow-режима. DrawTextShadow по умолчанию рендерит текст
	// дважды (shadow за главным). При false — одиночный рендер (только main).
	// Используется для диагностики "жирности" текста.
	static void SetShadowEnabled(bool enabled) { s_shadowEnabled = enabled; }
	static bool IsShadowEnabled() { return s_shadowEnabled; }

private:
	struct Glyph {
		float u1{0}, v1{0}, u2{0}, v2{0};
		int   OffsetX{0};   // от pen-позиции до левого края глифа
		int   OffsetY{0};   // от top-of-line до верха глифа
		int   Width{0};     // ширина глифа в пикселях
		int   Height{0};    // высота глифа в пикселях
		int   AdvanceX{0};  // шаг до следующего глифа
		bool  Valid{false};
	};

	const Glyph* _GetOrRasterize(wchar_t ch);
	bool         _RasterizeGlyph(wchar_t ch, Glyph& out);
	std::wstring _ToWide(const char* mbstr) const;
	void         _DrawWide(const std::wstring& wtext, int x, int y,
						   D3DXCOLOR color, float fScale,
						   const RECT* clipRect = nullptr);
	// DEBUG: рендер каждого текста в BMP ./font/debug/render.<timestamp>.bmp
	// через GDI (белый фон, чёрный текст).
	void         _DumpTextRender(const std::wstring& wtext);

private:
	MPRender* _dev{nullptr};

	HDC     _hdc{nullptr};
	HFONT   _hfont{nullptr};
	HFONT   _hFontOld{nullptr};
	HBITMAP _hBmp{nullptr};
	HGDIOBJ _hBmpOld{nullptr};
	DWORD*  _pBits{nullptr};

	int _textSize{0};
	int _textureSize{0};

	int _lineHeight{0};
	int _baseline{0};
	int _avgCharW{0};
	int _spaceAdvance{0};

	int _dibW{0};
	int _dibH{0};

	lwITex* _atlas{nullptr};

	// Shelf-packer state.
	int _packX{0}, _packY{0}, _packRowH{0};

	std::unordered_map<wchar_t, Glyph> _glyphs;

	// Дедуп для _DumpTextRender — один BMP на уникальный wstring.
	std::unordered_set<std::wstring> _dumpedTexts;

	std::string _fontName;

	UINT _codepage{CP_ACP};
	bool _useSoft{false};

	CMPEffectFile* _pEffect{nullptr};
	int            _renderIdx{5}; // индекс HLSL-техники в шейдере

	static bool s_textDumpEnabled;
	static bool s_shadowEnabled;
};
