#include "stdafx.h"
#include "FontRender.h"

#include "EffectFile.h"
#include "MPResManger.h"
#include "MPRender.h"
#include "lwIUtil.h"
#include "lwInterface.h"
#include "logutil.h"

#include <algorithm>
#include <cctype>
#include <chrono>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <format>
#include <stdexcept>

namespace {
	struct FontVertex {
		float X, Y, Z, Rhw;
		DWORD Color;
		float U, V;
	};

	constexpr DWORD kFontFVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;

	// Боковой отступ в DIB для отрицательных abcA (italic/caligraphy).
	constexpr int kDibPadLeft = 8;

	DWORD ColorToDword(const D3DXCOLOR& c) {
		return static_cast<DWORD>(c);
	}

	// ANTIALIASED_QUALITY пишет одинаковое значение в R=G=B (грейскейл AA),
	// так что любой канал даёт правильную интенсивность. Берём G как
	// "естественный" центральный канал (на случай если quality-флаг поменяют).
	BYTE PixelIntensity(DWORD p) {
		return static_cast<BYTE>((p >> 8) & 0xFF);
	}

	// Timestamp "YYYYMMDD_HHMMSS_mmm" для уникальных имён BMP-дампов.
	std::string TimestampMs() {
		const auto now = std::chrono::system_clock::now();
		const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
			now.time_since_epoch()) % 1000;
		const auto t = std::chrono::system_clock::to_time_t(now);
		std::tm lt{};
		::localtime_s(&lt, &t);
		return std::format("{:04}{:02}{:02}_{:02}{:02}{:02}_{:03}",
						   lt.tm_year + 1900, lt.tm_mon + 1, lt.tm_mday,
						   lt.tm_hour, lt.tm_min, lt.tm_sec,
						   static_cast<int>(ms.count()));
	}
} // namespace

bool FontRender::s_textDumpEnabled = false;
bool FontRender::s_shadowEnabled   = true;

FontRender::FontRender() = default;

FontRender::~FontRender() {
	ReleaseFont();
}

void FontRender::BindingRes(CMPResManger* pResMagr) {
	if (pResMagr) {
		_pEffect = pResMagr->GetEffectFile();
	}
}

bool FontRender::CreateFont(MPRender* pd3dDevice, char* szFontName,
							int nSize, int nLevel, DWORD dwFlag) {
	if (_hdc || _hfont || _atlas) {
		const std::string msg = std::format(
			"FontRender::CreateFont called twice (existing='{}' size={}, new='{}' size={})",
			_fontName, _textSize,
			szFontName ? szFontName : "<null>", nSize);
		ToLogService("errors", LogLevel::Error, "{}", msg);
		throw std::logic_error(msg);
	}

	if (nLevel < 1 || nLevel > 5 || !pd3dDevice || !szFontName) {
		return false;
	}

	_dev         = pd3dDevice;
	_textSize    = nSize;
	// Размер атласа: nLevel=1 → 128, nLevel=2 → 256, nLevel=3 → 512, ...
	// 64 — слишком тесно для полной латиницы при 12pt (после упаковки
	// остаётся места ≤ 40 глифов, остальные идут в "atlas full skip").
	_textureSize = 64 << nLevel;
	_fontName.assign(szFontName);

	_hdc = ::CreateCompatibleDC(nullptr);
	if (!_hdc) {
		return false;
	}
	::SetMapMode(_hdc, MM_TEXT);

	LOGFONTA lf{};
	lf.lfHeight         = -_textSize;
	lf.lfWeight         = (dwFlag & MPFONT_BOLD)   ? FW_BOLD : FW_NORMAL;
	lf.lfItalic         = (dwFlag & MPFONT_ITALIC) ? TRUE    : FALSE;
	lf.lfUnderline      = (dwFlag & MPFONT_UNLINE) ? TRUE    : FALSE;
	lf.lfCharSet        = DEFAULT_CHARSET;
	lf.lfOutPrecision   = OUT_DEFAULT_PRECIS;
	lf.lfClipPrecision  = CLIP_DEFAULT_PRECIS;
	// CLEARTYPE_QUALITY — влияет только на TextOutW (debug-дампы в DIB).
	// Rasterize-путь использует GetGlyphOutline(GGO_GRAY8_BITMAP) — тот
	// всегда возвращает монохромный 65-уровневый grayscale независимо
	// от lfQuality.
	lf.lfQuality        = CLEARTYPE_QUALITY;
	lf.lfPitchAndFamily = VARIABLE_PITCH | FF_DONTCARE;
	std::strncpy(lf.lfFaceName, szFontName, LF_FACESIZE - 1);

	_hfont = ::CreateFontIndirectA(&lf);
	if (!_hfont) {
		::DeleteDC(_hdc);
		_hdc = nullptr;
		return false;
	}

	TEXTMETRICA tm{};
	HFONT hOld = (HFONT)::SelectObject(_hdc, _hfont);
	::GetTextMetricsA(_hdc, &tm);
	::SelectObject(_hdc, hOld);

	_lineHeight = tm.tmHeight;
	_baseline   = tm.tmAscent;
	_avgCharW   = tm.tmAveCharWidth;

	// Advance для пробела (ABC-метрики не всегда включают пробел).
	{
		SIZE sz{};
		HFONT hOld2 = (HFONT)::SelectObject(_hdc, _hfont);
		::GetTextExtentPoint32A(_hdc, " ", 1, &sz);
		::SelectObject(_hdc, hOld2);
		_spaceAdvance = sz.cx > 0 ? sz.cx : (_avgCharW > 0 ? _avgCharW : _textSize / 2);
	}

	_dibW = _textSize * 2 + kDibPadLeft * 2;
	_dibH = _lineHeight + 4;

	BITMAPINFO bmi{};
	bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth       = _dibW;
	bmi.bmiHeader.biHeight      = -_dibH; // top-down
	bmi.bmiHeader.biPlanes      = 1;
	bmi.bmiHeader.biBitCount    = 32;
	bmi.bmiHeader.biCompression = BI_RGB;

	_hBmp = ::CreateDIBSection(_hdc, &bmi, DIB_RGB_COLORS,
								reinterpret_cast<void**>(&_pBits), nullptr, 0);
	if (!_hBmp || !_pBits) {
		::DeleteObject(_hfont);
		::DeleteDC(_hdc);
		_hfont = nullptr;
		_hdc   = nullptr;
		return false;
	}

	_hBmpOld  = ::SelectObject(_hdc, _hBmp);
	_hFontOld = (HFONT)::SelectObject(_hdc, _hfont);

	::SetTextColor(_hdc, RGB(255, 255, 255));
	::SetBkColor(_hdc, 0x00000000);
	::SetBkMode(_hdc, TRANSPARENT);
	::SetTextAlign(_hdc, TA_TOP | TA_LEFT);

	// Атлас строго D3DFMT_A8R8G8B8 (после fix'а в lwResourceMgr формат уважается).
	lwTexInfo info{};
	info.stage         = 0;
	info.type          = TEX_TYPE_SIZE;
	info.level         = 1;
	info.usage         = 0;
	info.format        = D3DFMT_A8R8G8B8;
	info.pool          = D3DPOOL_MANAGED;
	info.colorkey_type = COLORKEY_TYPE_NONE;
	info.width         = _textureSize;
	info.height        = _textureSize;

	lwIResourceMgr* resMgr = _dev->GetInterfaceMgr()->res_mgr;
	if (LW_FAILED(lwLoadTex(&_atlas, resMgr, &info))) {
		ToLogService("errors", LogLevel::Error,
					 "FontRender[{}]: atlas create failed (size={})",
					 _fontName, _textureSize);
		ReleaseFont();
		return false;
	}

	_packX = _packY = _packRowH = 0;
	return true;
}

void FontRender::ReleaseFont() {
	if (_atlas) {
		SAFE_RELEASE(_atlas);
	}
	_glyphs.clear();

	if (_hdc && _hFontOld) {
		::SelectObject(_hdc, _hFontOld);
		_hFontOld = nullptr;
	}
	if (_hdc && _hBmpOld) {
		::SelectObject(_hdc, _hBmpOld);
		_hBmpOld = nullptr;
	}
	if (_hBmp) {
		::DeleteObject(_hBmp);
		_hBmp  = nullptr;
		_pBits = nullptr;
	}
	if (_hfont) {
		::DeleteObject(_hfont);
		_hfont = nullptr;
	}
	if (_hdc) {
		::DeleteDC(_hdc);
		_hdc = nullptr;
	}

	_packX = _packY = _packRowH = 0;
	_dev = nullptr;
}

std::wstring FontRender::_ToWide(const char* mbstr) const {
	if (!mbstr) {
		return {};
	}
	const int len = ::MultiByteToWideChar(_codepage, 0, mbstr, -1, nullptr, 0);
	if (len <= 1) {
		return {};
	}
	std::wstring out(static_cast<size_t>(len - 1), L'\0');
	::MultiByteToWideChar(_codepage, 0, mbstr, -1, out.data(), len);
	return out;
}

const FontRender::Glyph* FontRender::_GetOrRasterize(wchar_t ch) {
	auto it = _glyphs.find(ch);
	if (it != _glyphs.end()) {
		return &it->second;
	}
	Glyph g{};
	if (!_RasterizeGlyph(ch, g)) {
		return nullptr;
	}
	auto [ins, _] = _glyphs.emplace(ch, g);
	return &ins->second;
}

bool FontRender::_RasterizeGlyph(wchar_t ch, Glyph& out) {
	out = {};
	if (!_hdc || !_atlas) {
		return false;
	}

	// GetGlyphOutlineW(GGO_GRAY8_BITMAP) даёт чистый монохромный AA:
	// 65 уровней серого (0..64), без субпиксельного разложения по RGB.
	// Метрики глифа берём прямо из GLYPHMETRICS — точнее и быстрее
	// чем TextOutW в DIB + поиск bbox сканом.
	static const MAT2 matIdentity = { {0,1}, {0,0}, {0,0}, {0,1} };

	HFONT hOldFont = (HFONT)::SelectObject(_hdc, _hfont);

	GLYPHMETRICS gm{};
	const DWORD bufSize = ::GetGlyphOutlineW(
		_hdc, ch, GGO_GRAY8_BITMAP, &gm, 0, nullptr, &matIdentity);

	if (bufSize == GDI_ERROR) {
		::SelectObject(_hdc, hOldFont);
		// Fallback: используем средние метрики.
		out.Width    = 0;
		out.Height   = 0;
		out.OffsetX  = 0;
		out.OffsetY  = 0;
		out.AdvanceX = _avgCharW;
		out.Valid    = true;
		return true;
	}

	std::vector<BYTE> glyphBits;
	if (bufSize > 0) {
		glyphBits.resize(bufSize);
		const DWORD ret = ::GetGlyphOutlineW(
			_hdc, ch, GGO_GRAY8_BITMAP, &gm, bufSize,
			glyphBits.data(), &matIdentity);
		if (ret == GDI_ERROR) {
			::SelectObject(_hdc, hOldFont);
			return false;
		}
	}

	::SelectObject(_hdc, hOldFont);

	const int gw      = static_cast<int>(gm.gmBlackBoxX);
	const int gh      = static_cast<int>(gm.gmBlackBoxY);
	const int originX = gm.gmptGlyphOrigin.x;
	const int originY = gm.gmptGlyphOrigin.y;
	const int advance = gm.gmCellIncX;

	// Shelf-packer. ВАЖНО: _packRowH сбрасываем только ПОСЛЕ подтверждённой
	// записи (секция ниже). Если сбросить при wrap до проверки overflow —
	// и overflow сработает (return false), _packRowH останется 0 и следующий
	// wrap уйдёт на ay + 0 + 1 вместо ay + prevRowH + 1, перезатирая строку.
	int ax = _packX;
	int ay = _packY;
	bool wrapped = false;
	if (gw > 0) {
		if (ax + gw > _textureSize) {
			ax = 0;
			ay += _packRowH + 1;
			wrapped = true;
		}
		if (ay + gh > _textureSize) {
			ToLogService("errors", LogLevel::Warning,
						 "FontRender[{}]: atlas full, glyph U+{:04X} skipped",
						 _fontName, static_cast<unsigned>(ch));
			return false;
		}
	}

	// Записываем bitmap в атлас. GGO_GRAY8_BITMAP — строки выровнены на
	// 4-байтовую границу, значения 0..64 → scale до 0..255.
	if (gw > 0 && gh > 0) {
		const int rowStride = (gw + 3) & ~3;

		D3DLOCKED_RECT lr{};
		if (FAILED(_atlas->GetTex()->LockRect(0, &lr, nullptr, 0))) {
			return false;
		}

		BYTE* dstBase = static_cast<BYTE*>(lr.pBits) + ay * lr.Pitch + ax * 4;
		for (int y = 0; y < gh; ++y) {
			DWORD* d = reinterpret_cast<DWORD*>(dstBase + y * lr.Pitch);
			const BYTE* srcRow = glyphBits.data() + y * rowStride;
			for (int x = 0; x < gw; ++x) {
				// 0..64 → 0..255: умножаем на 4, max 64 → 255.
				const BYTE v = srcRow[x];
				const BYTE a = (v >= 64) ? 255 : static_cast<BYTE>(v * 4);
				d[x] = a > 0 ? ((static_cast<DWORD>(a) << 24) | 0x00FFFFFF) : 0u;
			}
		}
		_atlas->GetTex()->UnlockRect(0);

		_packX = ax + gw + 1;
		_packY = ay;
		if (wrapped) {
			_packRowH = gh;
		} else if (gh > _packRowH) {
			_packRowH = gh;
		}
	}

	out.Width    = gw;
	out.Height   = gh;
	out.u1       = static_cast<float>(ax)      / static_cast<float>(_textureSize);
	out.v1       = static_cast<float>(ay)      / static_cast<float>(_textureSize);
	out.u2       = static_cast<float>(ax + gw) / static_cast<float>(_textureSize);
	out.v2       = static_cast<float>(ay + gh) / static_cast<float>(_textureSize);
	// originX ≈ abcA — горизонтальный отступ от pen до левого края глифа.
	// originY > 0 — glyph top выше baseline; baseline = _baseline от top-of-line.
	// → offset от top-of-line до glyph-top = _baseline - originY.
	out.OffsetX  = originX;
	out.OffsetY  = _baseline - originY;
	out.AdvanceX = advance;
	out.Valid    = true;
	return true;
}

void FontRender::_DrawWide(const std::wstring& wtext, int x, int y,
						   D3DXCOLOR color, float fScale,
						   const RECT* clipRect) {
	if (wtext.empty() || !_dev || !_atlas) {
		return;
	}

	_DumpTextRender(wtext);

	const DWORD dwColor = ColorToDword(color);

	std::vector<FontVertex> verts;
	verts.reserve(wtext.size() * 6);

	float pen = static_cast<float>(x);
	// lineY сдвигается на _lineHeight при каждом '\n'; baseY менять нельзя,
	// чтобы не сломать рассчитанный OffsetY глифа (Y-координата baseline строки).
	float lineY = static_cast<float>(y);

	for (wchar_t ch : wtext) {
		if (ch == L'\r') {
			continue;
		}
		if (ch == L'\n') {
			pen = static_cast<float>(x);
			lineY += static_cast<float>(_lineHeight) * fScale;
			continue;
		}
		if (ch == L' ') {
			pen += static_cast<float>(_spaceAdvance) * fScale;
			continue;
		}

		const Glyph* g = _GetOrRasterize(ch);
		if (!g || !g->Valid) {
			pen += static_cast<float>(_avgCharW) * fScale;
			continue;
		}

		if (g->Width > 0 && g->Height > 0) {
			// Half-pixel offset для D3DFVF_XYZRHW + LINEAR filter.
			// Без этого вершины лежат на целочисленных границах пикселей,
			// и bilinear семплит по 50/50 между соседними текселями → глиф
			// размазывается на 1 пиксель в каждую сторону (визуально жирнее).
			// Смещение на -0.5 попадает центром пикселя → LINEAR семплит
			// ровно один тексель.
			const float qx = pen + static_cast<float>(g->OffsetX) * fScale - 0.5f;
			const float qy = lineY + static_cast<float>(g->OffsetY) * fScale - 0.5f;
			const float qw = static_cast<float>(g->Width)  * fScale;
			const float qh = static_cast<float>(g->Height) * fScale;

			if (clipRect) {
				if (qx + qw < clipRect->left || qx > clipRect->right ||
					qy + qh < clipRect->top  || qy > clipRect->bottom) {
					pen += static_cast<float>(g->AdvanceX) * fScale;
					continue;
				}
			}

			verts.push_back({qx,      qy,      0.0f, 1.0f, dwColor, g->u1, g->v1});
			verts.push_back({qx + qw, qy,      0.0f, 1.0f, dwColor, g->u2, g->v1});
			verts.push_back({qx,      qy + qh, 0.0f, 1.0f, dwColor, g->u1, g->v2});

			verts.push_back({qx + qw, qy,      0.0f, 1.0f, dwColor, g->u2, g->v1});
			verts.push_back({qx + qw, qy + qh, 0.0f, 1.0f, dwColor, g->u2, g->v2});
			verts.push_back({qx,      qy + qh, 0.0f, 1.0f, dwColor, g->u1, g->v2});
		}

		pen += static_cast<float>(g->AdvanceX) * fScale;
	}

	if (verts.empty()) {
		return;
	}

	// Render-state выставляет HLSL-техника t5: alpha-blend SrcAlpha/InvSrcAlpha,
	// texstage MODULATE texture×diffuse, sampler POINT, Z off.
	if (!_pEffect) {
		ToLogService("errors", LogLevel::Error,
					 "FontRender[{}]: BindingRes не вызван до первого DrawText",
					 _fontName);
		return;
	}

	_pEffect->SetTechnique(_renderIdx);
	_pEffect->Begin(D3DXFX_DONOTSAVESTATE);
	_pEffect->Pass(0);

	_dev->SetVertexShader(nullptr);
	_dev->SetFVF(kFontFVF);
	// LINEAR filter — bilinear sampling размазывает AA-края на 1px, визуально
	// смягчает "слипание" соседних букв на малых кеглях без native-hinting.
	_dev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	_dev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	_dev->SetTexture(0, _atlas->GetTex());

	_dev->GetDevice()->DrawPrimitiveUP(
		D3DPT_TRIANGLELIST,
		static_cast<UINT>(verts.size() / 3),
		verts.data(),
		sizeof(FontVertex));

	_pEffect->End();
}

void FontRender::Draw(char* szText, int x, int y, D3DXCOLOR color) {
	if (!szText) return;
	_DrawWide(_ToWide(szText), x, y, color, 1.0f, nullptr);
}

bool FontRender::DrawText(char* szText, int x, int y, D3DXCOLOR color,
						  float fScale, DWORD* /*dwTime*/) {
	if (!szText || !szText[0]) return false;
	_DrawWide(_ToWide(szText), x, y, color, fScale, nullptr);
	return true;
}

bool FontRender::DrawText(int iNumber, int x, int y, D3DXCOLOR color, float fScale) {
	char buf[32];
	std::snprintf(buf, sizeof(buf), "%d", iNumber);
	return DrawText(buf, x, y, color, fScale);
}

bool FontRender::DrawTextShadow(char* szText, int x1, int y1, int x2, int y2,
								D3DXCOLOR color1, D3DXCOLOR color2) {
	if (!szText) return false;
	const std::wstring w = _ToWide(szText);
	if (s_shadowEnabled) {
		_DrawWide(w, x1, y1, color1, 1.0f, nullptr);
	}
	_DrawWide(w, x2, y2, color2, 1.0f, nullptr);
	return true;
}

bool FontRender::Draw3DText(char* /*szText*/, D3DXVECTOR3& /*vPos*/,
							D3DXCOLOR /*color*/, float /*fScale*/) {
	// TODO: проект 3D-точки на экран и отрисовка. Пока заглушка.
	return false;
}

void FontRender::DrawTextClipOnce(char* szText, int /*nLen*/,
								  LPRECT psrc, LPRECT pclip, D3DXCOLOR color) {
	if (!szText || !psrc) return;
	_DrawWide(_ToWide(szText), psrc->left, psrc->top, color, 1.0f, pclip);
}

SIZE* FontRender::GetTextSize(const std::string& szText, SIZE* pSize, float fScale) {
	if (!pSize) return nullptr;
	pSize->cx = 0;
	pSize->cy = static_cast<long>(_lineHeight * fScale);

	if (szText.empty()) return pSize;

	const std::wstring w = _ToWide(szText.c_str());
	float pen     = 0;
	long  maxPen  = 0;
	long  lines   = 1;
	for (wchar_t ch : w) {
		if (ch == L'\r') continue;
		if (ch == L'\n') {
			++lines;
			if (static_cast<long>(pen) > maxPen) maxPen = static_cast<long>(pen);
			pen = 0;
			continue;
		}
		if (ch == L' ') {
			pen += static_cast<float>(_spaceAdvance) * fScale;
			continue;
		}
		const Glyph* g = _GetOrRasterize(ch);
		pen += (g && g->Valid ? static_cast<float>(g->AdvanceX) : static_cast<float>(_avgCharW)) * fScale;
	}
	if (static_cast<long>(pen) > maxPen) maxPen = static_cast<long>(pen);

	pSize->cx = maxPen;
	pSize->cy = static_cast<long>(_lineHeight * fScale * lines);
	return pSize;
}

int FontRender::GetHzLength(float fscale) {
	return static_cast<int>(_textSize * fscale);
}

int FontRender::GetAscLength(float fscale) {
	return static_cast<int>((_avgCharW > 0 ? _avgCharW : _textSize / 2) * fscale);
}

bool FontRender::DumpGlyphPreview(const std::string& path) {
	if (!_hdc || !_hfont) {
		return false;
	}

	// Кириллица через \u-escape (не зависит от source-encoding).
	std::wstring ruUpper;
	ruUpper.push_back(L'\u0401'); // Ё
	for (wchar_t c = L'\u0410'; c <= L'\u042F'; ++c) {
		ruUpper.push_back(c);
	}
	std::wstring ruLower;
	ruLower.push_back(L'\u0451'); // ё
	for (wchar_t c = L'\u0430'; c <= L'\u044F'; ++c) {
		ruLower.push_back(c);
	}

	const std::wstring lines[] = {
		L"0123456789",
		L"ABCDEFGHIJKLMNOPQRSTUVWXYZ",
		L"abcdefghijklmnopqrstuvwxyz",
		std::move(ruUpper),
		std::move(ruLower),
	};
	constexpr int lineCount = sizeof(lines) / sizeof(lines[0]);

	// Измеряем ширину самой длинной строки.
	int maxW = 0;
	for (int i = 0; i < lineCount; ++i) {
		SIZE sz{};
		::GetTextExtentPoint32W(_hdc, lines[i].c_str(),
								static_cast<int>(lines[i].size()), &sz);
		if (sz.cx > maxW) {
			maxW = sz.cx;
		}
	}

	constexpr int pad = 8;
	const int w = maxW + pad * 2;
	const int h = _lineHeight * lineCount + pad * 2;
	if (w <= 0 || h <= 0) {
		return false;
	}

	BITMAPINFO bmi{};
	bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth       = w;
	bmi.bmiHeader.biHeight      = -h; // top-down
	bmi.bmiHeader.biPlanes      = 1;
	bmi.bmiHeader.biBitCount    = 32;
	bmi.bmiHeader.biCompression = BI_RGB;

	DWORD* pBits = nullptr;
	HBITMAP hBmp = ::CreateDIBSection(_hdc, &bmi, DIB_RGB_COLORS,
									   reinterpret_cast<void**>(&pBits),
									   nullptr, 0);
	if (!hBmp || !pBits) {
		if (hBmp) {
			::DeleteObject(hBmp);
		}
		return false;
	}

	// Подменяем DIB в _hdc — шрифт уже выбран, не трогаем.
	HGDIOBJ oldBmp = ::SelectObject(_hdc, hBmp);

	// Белый фон, чёрный текст.
	const RECT rc{ 0, 0, w, h };
	::FillRect(_hdc, &rc, static_cast<HBRUSH>(::GetStockObject(WHITE_BRUSH)));

	::SetTextColor(_hdc, RGB(0, 0, 0));
	::SetBkColor(_hdc, RGB(255, 255, 255));
	::SetBkMode(_hdc, TRANSPARENT);

	for (int i = 0; i < lineCount; ++i) {
		::TextOutW(_hdc, pad, pad + i * _lineHeight,
				   lines[i].c_str(), static_cast<int>(lines[i].size()));
	}
	::GdiFlush();

	// Пишем BMP как есть — GDI в 32bpp DIB записал BGRA, альфа = 0 (игнорируем).
	// Для просмотра в стандартных вьюверах перезаполняем альфу = 255.
	const int rowBytes = w * 4;
	std::vector<BYTE> pixels(static_cast<size_t>(rowBytes) * h);
	for (int y = 0; y < h; ++y) {
		const BYTE* src = reinterpret_cast<const BYTE*>(pBits)
						+ static_cast<size_t>(y) * rowBytes;
		BYTE* dst = pixels.data() + static_cast<size_t>(y) * rowBytes;
		for (int x = 0; x < w; ++x) {
			dst[x * 4 + 0] = src[x * 4 + 0]; // B
			dst[x * 4 + 1] = src[x * 4 + 1]; // G
			dst[x * 4 + 2] = src[x * 4 + 2]; // R
			dst[x * 4 + 3] = 255;            // A
		}
	}

	bool ok = false;
	FILE* f = nullptr;
	if (fopen_s(&f, path.c_str(), "wb") == 0 && f) {
		BITMAPFILEHEADER fh{};
		BITMAPINFOHEADER ih{};
		fh.bfType    = 0x4D42; // 'BM'
		fh.bfOffBits = sizeof(fh) + sizeof(ih);
		fh.bfSize    = fh.bfOffBits + rowBytes * h;

		ih.biSize        = sizeof(ih);
		ih.biWidth       = w;
		ih.biHeight      = -h;
		ih.biPlanes      = 1;
		ih.biBitCount    = 32;
		ih.biCompression = BI_RGB;
		ih.biSizeImage   = rowBytes * h;

		std::fwrite(&fh, sizeof(fh), 1, f);
		std::fwrite(&ih, sizeof(ih), 1, f);
		std::fwrite(pixels.data(), 1, pixels.size(), f);
		std::fclose(f);
		ok = true;
	}

	::SelectObject(_hdc, oldBmp);
	::DeleteObject(hBmp);
	return ok;
}

bool FontRender::DumpAtlas(const std::string& path) {
	if (!_atlas || !_atlas->GetTex()) {
		return false;
	}
	D3DLOCKED_RECT lr{};
	if (FAILED(_atlas->GetTex()->LockRect(0, &lr, nullptr, D3DLOCK_READONLY))) {
		return false;
	}

	const int w = _textureSize;
	const int h = _textureSize;
	const int rowBytes = w * 4;
	std::vector<BYTE> pixels(static_cast<size_t>(rowBytes) * h);

	// Шахматный фон (чтобы было видно прозрачные области); поверх —
	// глифы (A из альфы, RGB из атласа = белый).
	for (int y = 0; y < h; ++y) {
		const BYTE* src = static_cast<const BYTE*>(lr.pBits) + y * lr.Pitch;
		BYTE* dst = pixels.data() + static_cast<size_t>(y) * rowBytes;
		for (int x = 0; x < w; ++x) {
			const BYTE a = src[x * 4 + 3]; // A
			// Чекерборд: светлый/тёмный серый клетки 8×8.
			const BYTE bg = ((x / 8 + y / 8) & 1) ? 200 : 230;
			// Альфа=alpha глифа → чёрные пиксели поверх шахматки.
			const BYTE r = static_cast<BYTE>((bg * (255 - a)) / 255);
			dst[x * 4 + 0] = r; // B
			dst[x * 4 + 1] = r; // G
			dst[x * 4 + 2] = r; // R
			dst[x * 4 + 3] = 255;
		}
	}

	_atlas->GetTex()->UnlockRect(0);

	bool ok = false;
	FILE* f = nullptr;
	if (fopen_s(&f, path.c_str(), "wb") == 0 && f) {
		BITMAPFILEHEADER fh{};
		BITMAPINFOHEADER ih{};
		fh.bfType    = 0x4D42;
		fh.bfOffBits = sizeof(fh) + sizeof(ih);
		fh.bfSize    = fh.bfOffBits + rowBytes * h;

		ih.biSize        = sizeof(ih);
		ih.biWidth       = w;
		ih.biHeight      = -h;
		ih.biPlanes      = 1;
		ih.biBitCount    = 32;
		ih.biCompression = BI_RGB;
		ih.biSizeImage   = rowBytes * h;

		std::fwrite(&fh, sizeof(fh), 1, f);
		std::fwrite(&ih, sizeof(ih), 1, f);
		std::fwrite(pixels.data(), 1, pixels.size(), f);
		std::fclose(f);
		ok = true;
	}
	return ok;
}

void FontRender::_DumpTextRender(const std::wstring& wtext) {
	if (!s_textDumpEnabled) {
		return;
	}
	if (wtext.empty() || !_hdc || !_hfont) {
		return;
	}
	// Дедуп — один BMP на уникальный текст per-instance.
	if (_dumpedTexts.find(wtext) != _dumpedTexts.end()) {
		return;
	}
	_dumpedTexts.insert(wtext);

	namespace fs = std::filesystem;
	const fs::path dir = "./font/debug";
	std::error_code ec;
	fs::create_directories(dir, ec);

	SIZE sz{};
	::GetTextExtentPoint32W(_hdc, wtext.c_str(),
							static_cast<int>(wtext.size()), &sz);
	constexpr int pad = 8;
	const int w = sz.cx + pad * 2;
	const int h = _lineHeight + pad * 2;
	if (w <= 0 || h <= 0) {
		return;
	}

	BITMAPINFO bmi{};
	bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth       = w;
	bmi.bmiHeader.biHeight      = -h;
	bmi.bmiHeader.biPlanes      = 1;
	bmi.bmiHeader.biBitCount    = 32;
	bmi.bmiHeader.biCompression = BI_RGB;

	DWORD* pBits = nullptr;
	HBITMAP hBmp = ::CreateDIBSection(_hdc, &bmi, DIB_RGB_COLORS,
									   reinterpret_cast<void**>(&pBits),
									   nullptr, 0);
	if (!hBmp || !pBits) {
		if (hBmp) {
			::DeleteObject(hBmp);
		}
		return;
	}

	HGDIOBJ oldBmp = ::SelectObject(_hdc, hBmp);

	const RECT rc{ 0, 0, w, h };
	::FillRect(_hdc, &rc, static_cast<HBRUSH>(::GetStockObject(WHITE_BRUSH)));

	::SetTextColor(_hdc, RGB(0, 0, 0));
	::SetBkColor(_hdc, RGB(255, 255, 255));
	::SetBkMode(_hdc, TRANSPARENT);

	::TextOutW(_hdc, pad, pad, wtext.c_str(),
			   static_cast<int>(wtext.size()));
	::GdiFlush();

	// 32bpp BGRA, дополняем alpha=255 для совместимости с просмотрщиками.
	const int rowBytes = w * 4;
	std::vector<BYTE> pixels(static_cast<size_t>(rowBytes) * h);
	for (int y = 0; y < h; ++y) {
		const BYTE* src = reinterpret_cast<const BYTE*>(pBits)
						+ static_cast<size_t>(y) * rowBytes;
		BYTE* dst = pixels.data() + static_cast<size_t>(y) * rowBytes;
		for (int x = 0; x < w; ++x) {
			dst[x * 4 + 0] = src[x * 4 + 0];
			dst[x * 4 + 1] = src[x * 4 + 1];
			dst[x * 4 + 2] = src[x * 4 + 2];
			dst[x * 4 + 3] = 255;
		}
	}

	// Санитизация имени шрифта (пробелы и т.п. → _) для filename.
	std::string fontSan;
	fontSan.reserve(_fontName.size());
	for (char c : _fontName) {
		const bool ok = std::isalnum(static_cast<unsigned char>(c)) != 0
					 || c == '-' || c == '_';
		fontSan += ok ? c : '_';
	}
	if (fontSan.empty()) {
		fontSan = "unknown";
	}

	const fs::path path = dir / std::format("render.{}.{}.bmp",
											 fontSan, TimestampMs());
	FILE* f = nullptr;
	if (fopen_s(&f, path.string().c_str(), "wb") == 0 && f) {
		BITMAPFILEHEADER fh{};
		BITMAPINFOHEADER ih{};
		fh.bfType    = 0x4D42;
		fh.bfOffBits = sizeof(fh) + sizeof(ih);
		fh.bfSize    = fh.bfOffBits + rowBytes * h;

		ih.biSize        = sizeof(ih);
		ih.biWidth       = w;
		ih.biHeight      = -h;
		ih.biPlanes      = 1;
		ih.biBitCount    = 32;
		ih.biCompression = BI_RGB;
		ih.biSizeImage   = rowBytes * h;

		std::fwrite(&fh, sizeof(fh), 1, f);
		std::fwrite(&ih, sizeof(ih), 1, f);
		std::fwrite(pixels.data(), 1, pixels.size(), f);
		std::fclose(f);
	}

	::SelectObject(_hdc, oldBmp);
	::DeleteObject(hBmp);
}
