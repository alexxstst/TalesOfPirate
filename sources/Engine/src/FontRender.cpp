#include "stdafx.h"
#include "FontRender.h"

#include "MPRender.h"
#include "lwIUtil.h"
#include "lwInterface.h"
#include "logutil.h"

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstring>
#include <format>
#include <stdexcept>
#include <vector>

#include "fontstash.h"

namespace {
	unsigned int ColorToRgba(const D3DXCOLOR& c) {
		const DWORD argb = static_cast<DWORD>(c);
		const unsigned a = (argb >> 24) & 0xFFu;
		const unsigned r = (argb >> 16) & 0xFFu;
		const unsigned g = (argb >> 8) & 0xFFu;
		const unsigned b = (argb) & 0xFFu;
		// fontstash передаёт цвет как uint32 в FontVertex.Diffuse. DX9 ожидает
		// ARGB little-endian. Оставляем ARGB — цвет корректно интерпретируется
		// D3DFVF_DIFFUSE сразу, без swap'а каналов.
		return (a << 24) | (r << 16) | (g << 8) | b;
	}
} // namespace

bool FontRender::s_textDumpEnabled = false;
bool FontRender::s_shadowEnabled = true;

FontRender::FontRender() = default;

FontRender::~FontRender() {
	ReleaseFont();
}

bool FontRender::CreateFont(MPRender* pd3dDevice, char* szFontName,
							int nSize, int nLevel, DWORD dwFlag,
							const std::string& ttfPath,
							FONScontext* fons, int fonsFontId,
							float sizeScale) {
	(void)nLevel; // атлас теперь общий и сам ресайзится — уровень не нужен.
	(void)dwFlag; // bold/italic/underline пока не применяются (fonstash не имеет API).
	(void)ttfPath; // фактическая загрузка TTF — в FontManager.

	if (_fons) {
		const std::string msg = std::format(
			"FontRender::CreateFont called twice (existing='{}' size={}, new='{}' size={})",
			_fontName, _textSize,
			szFontName ? szFontName : "<null>", nSize);
		ToLogService("errors", LogLevel::Error, "{}", msg);
		throw std::logic_error(msg);
	}

	if (!pd3dDevice || !szFontName || nSize <= 0) {
		return false;
	}
	if (!fons || fonsFontId < 0) {
		// Известный баг: системные шрифты Windows (Arial, Tahoma, Segoe UI и т.п.)
		// в текущей FreeType+fontstash-инфраструктуре НЕ резолвятся автоматически.
		// FontManager::InstallFontFile грузит файлы из ./font/<Family>/, и только
		// эти семейства попадают в fontstash. Запрос на «Arial» (или любой другой
		// шрифт, не лежащий в ./font/) даёт fonsFontId<0 и сюда. Workaround в lua
		// (font_bootstrap.lua / console_bootstrap.lua): использовать семейства из
		// ./font/ — Roboto, Inter, NotoSans, OpenSans, PTSans, SourceSans3.
		// Долгосрочный fix: подружить fontstash с GDI/DirectWrite либо
		// автоматически копировать %WINDIR%/Fonts/<Family>.ttf в FontManager.
		ToLogService("errors", LogLevel::Error,
					 "FontRender::CreateFont('{}', size={}): fontstash не передан — "
					 "шрифт не зарегистрирован через FontManager::InstallFontFile? "
					 "(семейство нет в ./font/<Family>/; см. комментарий выше)",
					 szFontName, nSize);
		return false;
	}

	_dev = pd3dDevice;
	_textSize = nSize;
	_fons = fons;
	_fonsFontId = fonsFontId;
	_sizeScale = sizeScale > 0.0f ? sizeScale : 1.0f;
	_fontName.assign(szFontName);

	// Метрики строки через fontstash. Передаём size с учётом _sizeScale.
	fonsSetFont(_fons, _fonsFontId);
	fonsSetSize(_fons, static_cast<float>(_textSize) * _sizeScale);
	fonsSetAlign(_fons, FONS_ALIGN_LEFT | FONS_ALIGN_TOP);
	fonsSetSpacing(_fons, 0.0f);
	fonsSetBlur(_fons, 0.0f);

	float ascender = 0.0f, descender = 0.0f, lineH = 0.0f;
	fonsVertMetrics(_fons, &ascender, &descender, &lineH);
	// descender у fontstash возвращается отрицательный (от baseline вниз).
	// std::max конфликтует с макросом `max` из <windows.h>, поэтому условные выражения.
	_lineHeight = static_cast<int>(lineH > 1.0f ? lineH : 1.0f);
	_baseline = static_cast<int>(ascender > 0.0f ? ascender : 0.0f);

	// Advance для пробела и "M". end=nullptr → fontstash сам вычислит через
	// strlen(str) (см. fontstash.h:1520-1521). Передавать `" " + 1` НЕЛЬЗЯ:
	// два литерала `" "` в двух выражениях компилятор не обязан пулить в один
	// объект — диапазон [start, end) может охватывать разные объекты памяти,
	// это UB. В fontstash приводит к access violation в fons__decutf8
	// (строка 1524: *(const unsigned char*)str читает за пределами литерала).
	{
		float bounds[4]{};
		const float advance = fonsTextBounds(_fons, 0, 0, " ", nullptr, bounds);
		_spaceAdvance = static_cast<int>(advance > 1.0f ? advance : 1.0f);
	}
	{
		float bounds[4]{};
		const float advance = fonsTextBounds(_fons, 0, 0, "M", nullptr, bounds);
		_avgCharW = static_cast<int>(advance > 1.0f ? advance : 1.0f);
	}

	ToLogService("common",
				 "FontRender[{}]: fontstash fontId={} size={}px "
				 "(line={}, baseline={}, advance_space={})",
				 _fontName, _fonsFontId, _textSize,
				 _lineHeight, _baseline, _spaceAdvance);
	return true;
}

void FontRender::ReleaseFont() {
	// FONScontext владеет FontManager — не удаляем здесь, только забываем.
	_fons = nullptr;
	_fonsFontId = -1;
	_sizeScale = 1.0f;
	_dev = nullptr;
	_textSize = 0;
	_lineHeight = 0;
	_baseline = 0;
	_avgCharW = 0;
	_spaceAdvance = 0;
	_dumpedTexts.clear();
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

std::string FontRender::_ToUtf8(const std::wstring& w) const {
	if (w.empty()) {
		return {};
	}
	const int u8len = ::WideCharToMultiByte(
		CP_UTF8, 0, w.data(), static_cast<int>(w.size()),
		nullptr, 0, nullptr, nullptr);
	if (u8len <= 0) {
		return {};
	}
	std::string out(static_cast<size_t>(u8len), '\0');
	::WideCharToMultiByte(CP_UTF8, 0,
						  w.data(), static_cast<int>(w.size()),
						  out.data(), u8len, nullptr, nullptr);
	return out;
}

void FontRender::_DrawWide(const std::wstring& wtext, int x, int y,
						   D3DXCOLOR color, float fScale,
						   const RECT* clipRect) {
	if (wtext.empty() || !_dev || !_fons || _fonsFontId < 0) {
		return;
	}

	const std::string utf8 = _ToUtf8(wtext);
	if (utf8.empty()) {
		return;
	}

	fonsSetFont(_fons, _fonsFontId);
	fonsSetSize(_fons, static_cast<float>(_textSize) * fScale * _sizeScale);
	fonsSetColor(_fons, ColorToRgba(color));
	fonsSetAlign(_fons, FONS_ALIGN_LEFT | FONS_ALIGN_TOP);
	fonsSetSpacing(_fons, 0.0f);
	fonsSetBlur(_fons, 0.0f);

	// Поддержка '\n': fontstash не ломает строки сам. Разбиваем вручную.
	const char* p = utf8.c_str();
	const char* pend = utf8.c_str() + utf8.size();
	float penY = static_cast<float>(y);
	const float lineH = static_cast<float>(_lineHeight) * fScale;

	while (p < pend) {
		const char* nl = p;
		while (nl < pend && *nl != '\n') {
			++nl;
		}
		// clipRect: грубый отсев по y-строке. fontstash не клипит — частично
		// видимые строки рисуются целиком; для UI-контейнеров приемлемо.
		if (!clipRect ||
			(penY + lineH >= static_cast<float>(clipRect->top) &&
				penY <= static_cast<float>(clipRect->bottom))) {
			fonsDrawText(_fons, static_cast<float>(x), penY, p, nl);
		}
		penY += lineH;
		p = (nl < pend) ? nl + 1 : pend;
	}
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
	// TODO: проекция 3D-точки на экран + 2D-рендер. Пока заглушка.
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

	if (szText.empty() || !_fons || _fonsFontId < 0) {
		return pSize;
	}

	// Конверсия в UTF-8, если codepage не UTF-8.
	std::string utf8;
	if (_codepage == CP_UTF8) {
		utf8 = szText;
	}
	else {
		utf8 = _ToUtf8(_ToWide(szText.c_str()));
	}
	if (utf8.empty()) {
		return pSize;
	}

	fonsSetFont(_fons, _fonsFontId);
	fonsSetSize(_fons, static_cast<float>(_textSize) * fScale * _sizeScale);
	fonsSetAlign(_fons, FONS_ALIGN_LEFT | FONS_ALIGN_TOP);
	fonsSetSpacing(_fons, 0.0f);
	fonsSetBlur(_fons, 0.0f);

	long maxWidth = 0;
	long lines = 1;
	const char* p = utf8.c_str();
	const char* pend = utf8.c_str() + utf8.size();
	while (p < pend) {
		const char* nl = p;
		while (nl < pend && *nl != '\n') {
			++nl;
		}
		float bounds[4]{};
		fonsTextBounds(_fons, 0, 0, p, nl, bounds);
		const long w = static_cast<long>(bounds[2] - bounds[0]);
		if (w > maxWidth) {
			maxWidth = w;
		}
		if (nl < pend) {
			++lines;
		}
		p = (nl < pend) ? nl + 1 : pend;
	}
	pSize->cx = maxWidth;
	pSize->cy = static_cast<long>(_lineHeight * fScale) * lines;
	return pSize;
}

int FontRender::GetHzLength(float fscale) {
	return static_cast<int>(_textSize * fscale);
}

int FontRender::GetAscLength(float fscale) {
	return static_cast<int>((_avgCharW > 0 ? _avgCharW : _textSize / 2) * fscale);
}

bool FontRender::DumpAtlas(const std::string& path) {
	if (!_fons) {
		return false;
	}
	int w = 0, h = 0;
	const unsigned char* data = fonsGetTextureData(_fons, &w, &h);
	if (!data || w <= 0 || h <= 0) {
		return false;
	}

	const int rowBytes = w * 4;
	std::vector<BYTE> pixels(static_cast<size_t>(rowBytes) * h);
	// Чекерборд-фон + альфа из атласа (glyph visible как тёмные пятна).
	for (int y = 0; y < h; ++y) {
		const unsigned char* src = data + y * w;
		BYTE* dst = pixels.data() + static_cast<size_t>(y) * rowBytes;
		for (int x = 0; x < w; ++x) {
			const BYTE a = src[x];
			const BYTE bg = ((x / 8 + y / 8) & 1) ? 200 : 230;
			const BYTE v = static_cast<BYTE>((bg * (255 - a)) / 255);
			dst[x * 4 + 0] = v; // B
			dst[x * 4 + 1] = v; // G
			dst[x * 4 + 2] = v; // R
			dst[x * 4 + 3] = 255;
		}
	}

	bool ok = false;
	FILE* f = nullptr;
	if (fopen_s(&f, path.c_str(), "wb") == 0 && f) {
		BITMAPFILEHEADER fh{};
		BITMAPINFOHEADER ih{};
		fh.bfType = 0x4D42;
		fh.bfOffBits = sizeof(fh) + sizeof(ih);
		fh.bfSize = fh.bfOffBits + rowBytes * h;

		ih.biSize = sizeof(ih);
		ih.biWidth = w;
		ih.biHeight = -h; // top-down
		ih.biPlanes = 1;
		ih.biBitCount = 32;
		ih.biCompression = BI_RGB;
		ih.biSizeImage = rowBytes * h;

		std::fwrite(&fh, sizeof(fh), 1, f);
		std::fwrite(&ih, sizeof(ih), 1, f);
		std::fwrite(pixels.data(), 1, pixels.size(), f);
		std::fclose(f);
		ok = true;
	}
	return ok;
}

bool FontRender::DumpGlyphPreview(const std::string& path) {
	// Переиспользуем DumpAtlas — в fontstash все растеризованные глифы лежат
	// в общем атласе, отдельное превью per-font недоступно через публичный API.
	return DumpAtlas(path);
}
