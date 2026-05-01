#pragma once

// UIText — замена CGuiFont::s_Font. Свободные функции в namespace ui,
// рутят все вызовы в FontManager + сохраняют DrawConvert-пересчёт
// экранных координат через GetRender().
//
// Handle — int, совпадает с тем, что возвращает UI_CreateFont из Lua.
//
// API:
//   ui::Render(handle, str, x, y, color, scale = 1.0f)
//   ui::BRender(handle, str, x, y, color, shadowColor)
//   ui::Render3d(handle, str, pos, color)
//   ui::GetSize(handle, str, w, h)
//   ui::GetWidth(handle, str)
//   ui::GetHeight(handle, str)
//   ui::FrameRender(handle, str, x, y)       — рамка + текст
//   ui::TipRender(handle, str, x, y)         — tip с центровкой
//
// Если handle не зарегистрирован, FontManager::Get вернёт nullptr с логом
// ошибки — функции делают безопасный ранний return.

#include "FontManager.h"
#include "MPFont.h"
#include "UIRender.h"

#include <d3dx9math.h>
#define NOMINMAX
#include <windows.h>
#include <string>
#include <string_view>

namespace ui {
	inline void Render(int handle, std::string_view str, int x, int y,
					   DWORD color, float scale = 1.0f) {
		CMPFont* pFont = FontManager::Instance().Get(handle);
		if (!pFont) {
			return;
		}
		GetRender().DrawConvert(x, y);
		pFont->DrawText(str, x, y, color, scale);
	}

	inline void Render(FontSlot slot, std::string_view str, int x, int y,
					   DWORD color, float scale = 1.0f) {
		CMPFont* pFont = FontManager::Instance().Get(slot);
		if (!pFont) {
			return;
		}
		GetRender().DrawConvert(x, y);
		pFont->DrawText(str, x, y, color, scale);
	}

	inline void BRender(int handle, std::string_view str, int x, int y,
						DWORD color, DWORD shadowColor) {
		CMPFont* pFont = FontManager::Instance().Get(handle);
		if (!pFont) {
			return;
		}
		GetRender().DrawConvert(x, y);
		pFont->DrawTextShadow(str, x + 1, y + 1, x, y, shadowColor, color);
	}

	inline void Render3d(int handle, std::string_view str,
						 D3DXVECTOR3& pos, DWORD color) {
		CMPFont* pFont = FontManager::Instance().Get(handle);
		if (!pFont) {
			return;
		}
		pFont->Draw3DText(str, pos, color);
	}

	inline bool GetSize(int handle, std::string_view str, int& w, int& h) {
		w = 0;
		h = 0;
		CMPFont* pFont = FontManager::Instance().Get(handle);
		if (!pFont) {
			return false;
		}
		SIZE sz{};
		pFont->GetTextSize(str, &sz);
		w = static_cast<int>(sz.cx);
		h = static_cast<int>(sz.cy);
		return true;
	}

	inline int GetWidth(int handle, std::string_view str) {
		int w = 0, h = 0;
		GetSize(handle, str, w, h);
		return w;
	}

	inline int GetHeight(int handle, std::string_view str) {
		int w = 0, h = 0;
		GetSize(handle, str, w, h);
		return h;
	}

	// Удобные перегрузки для "дефолтного" шрифта (FontSlot::TipText).
	inline int GetWidth(std::string_view str) {
		CMPFont* pFont = FontManager::Instance().Get(FontSlot::TipText);
		if (!pFont) {
			return 0;
		}
		SIZE sz{};
		pFont->GetTextSize(str, &sz);
		return static_cast<int>(sz.cx);
	}

	inline int GetHeight(std::string_view str) {
		CMPFont* pFont = FontManager::Instance().Get(FontSlot::TipText);
		if (!pFont) {
			return 0;
		}
		SIZE sz{};
		pFont->GetTextSize(str, &sz);
		return static_cast<int>(sz.cy);
	}

	inline void GetSize(std::string_view str, int& w, int& h) {
		w = 0;
		h = 0;
		CMPFont* pFont = FontManager::Instance().Get(FontSlot::TipText);
		if (!pFont) {
			return;
		}
		SIZE sz{};
		pFont->GetTextSize(str, &sz);
		w = static_cast<int>(sz.cx);
		h = static_cast<int>(sz.cy);
	}

	inline void Render(std::string_view str, int x, int y, DWORD color, float scale = 1.0f) {
		Render(FontSlot::TipText, str, x, y, color, scale);
	}

	inline void BRender(std::string_view str, int x, int y, DWORD color, DWORD shadowColor) {
		CMPFont* pFont = FontManager::Instance().Get(FontSlot::TipText);
		if (!pFont) {
			return;
		}
		GetRender().DrawConvert(x, y);
		pFont->DrawTextShadow(str, x + 1, y + 1, x, y, shadowColor, color);
	}

	inline void FrameRender(std::string_view str, int x, int y) {
		int w = 0, h = 0;
		GetSize(str, w, h);
		const int offx = GetRender().GetScreenWidth() - x - w - 10;
		if (offx < 0) {
			x += offx;
		}
		GetRender().FillFrame(x - 5, y - 3, x + w + 10, y + h + 5, 0x90000000);
		Render(str, x - 1, y - 1, 0xffffffff);
	}

	inline void TipRender(std::string_view str, int x, int y) {
		int w = 0, h = 0;
		GetSize(str, w, h);
		x -= w / 2;
		GetRender().FillFrame(x - 2, y - 1, x + w + 2, y + h + 1, 0x90000000);
		Render(str, x - 1, y - 1, 0xffffffff);
	}

	// Алиасы для миграции со старого CGuiFont API.
	inline void RenderScale(std::string_view str, int x, int y, DWORD color, float scale) {
		Render(str, x, y, color, scale);
	}

	inline void RenderScale(int handle, std::string_view str, int x, int y,
							DWORD color, float scale) {
		Render(handle, str, x, y, color, scale);
	}

	inline CMPFont* GetFont(int handle) {
		return FontManager::Instance().Get(handle);
	}

	inline void Clear() {
		FontManager::Instance().ClearFonts();
	}
} // namespace ui
