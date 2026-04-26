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
#include <windows.h>
#include <string>

namespace ui {
	inline void Render(int handle, const char* str, int x, int y,
					   DWORD color, float scale = 1.0f) {
		CMPFont* pFont = FontManager::Instance().Get(handle);
		if (!pFont || !str) {
			return;
		}
		GetRender().DrawConvert(x, y);
		pFont->DrawText(const_cast<char*>(str), x, y, color, scale);
	}

	inline void Render(FontSlot slot, const char* str, int x, int y,
					   DWORD color, float scale = 1.0f) {
		CMPFont* pFont = FontManager::Instance().Get(slot);
		if (!pFont || !str) {
			return;
		}
		GetRender().DrawConvert(x, y);
		pFont->DrawText(const_cast<char*>(str), x, y, color, scale);
	}

	inline void BRender(int handle, const char* str, int x, int y,
						DWORD color, DWORD shadowColor) {
		CMPFont* pFont = FontManager::Instance().Get(handle);
		if (!pFont || !str) {
			return;
		}
		GetRender().DrawConvert(x, y);
		pFont->DrawTextShadow(const_cast<char*>(str),
							  x + 1, y + 1, x, y, shadowColor, color);
	}

	inline void Render3d(int handle, const char* str,
						 D3DXVECTOR3& pos, DWORD color) {
		CMPFont* pFont = FontManager::Instance().Get(handle);
		if (!pFont || !str) {
			return;
		}
		pFont->Draw3DText(const_cast<char*>(str), pos, color);
	}

	inline bool GetSize(int handle, const char* str, int& w, int& h) {
		w = 0;
		h = 0;
		CMPFont* pFont = FontManager::Instance().Get(handle);
		if (!pFont || !str) {
			return false;
		}
		SIZE sz{};
		pFont->GetTextSize(std::string{str}, &sz);
		w = static_cast<int>(sz.cx);
		h = static_cast<int>(sz.cy);
		return true;
	}

	inline int GetWidth(int handle, const char* str) {
		int w = 0, h = 0;
		GetSize(handle, str, w, h);
		return w;
	}

	inline int GetHeight(int handle, const char* str) {
		int w = 0, h = 0;
		GetSize(handle, str, w, h);
		return h;
	}

	// Удобные перегрузки для "дефолтного" шрифта (FontSlot::TipText).
	inline int GetWidth(const char* str) {
		CMPFont* pFont = FontManager::Instance().Get(FontSlot::TipText);
		if (!pFont || !str) {
			return 0;
		}
		SIZE sz{};
		pFont->GetTextSize(std::string{str}, &sz);
		return static_cast<int>(sz.cx);
	}

	inline int GetWidth(const std::string& str) {
		return GetWidth(str.c_str());
	}

	inline int GetHeight(const char* str) {
		CMPFont* pFont = FontManager::Instance().Get(FontSlot::TipText);
		if (!pFont || !str) {
			return 0;
		}
		SIZE sz{};
		pFont->GetTextSize(std::string{str}, &sz);
		return static_cast<int>(sz.cy);
	}

	inline void GetSize(const char* str, int& w, int& h) {
		w = 0;
		h = 0;
		CMPFont* pFont = FontManager::Instance().Get(FontSlot::TipText);
		if (!pFont || !str) {
			return;
		}
		SIZE sz{};
		pFont->GetTextSize(std::string{str}, &sz);
		w = static_cast<int>(sz.cx);
		h = static_cast<int>(sz.cy);
	}

	inline void Render(const char* str, int x, int y, DWORD color, float scale = 1.0f) {
		Render(FontSlot::TipText, str, x, y, color, scale);
	}

	inline void BRender(const char* str, int x, int y, DWORD color, DWORD shadowColor) {
		CMPFont* pFont = FontManager::Instance().Get(FontSlot::TipText);
		if (!pFont || !str) {
			return;
		}
		GetRender().DrawConvert(x, y);
		pFont->DrawTextShadow(const_cast<char*>(str),
							  x + 1, y + 1, x, y, shadowColor, color);
	}

	inline void FrameRender(const char* str, int x, int y) {
		int w = 0, h = 0;
		GetSize(str, w, h);
		const int offx = GetRender().GetScreenWidth() - x - w - 10;
		if (offx < 0) {
			x += offx;
		}
		GetRender().FillFrame(x - 5, y - 3, x + w + 10, y + h + 5, 0x90000000);
		Render(str, x - 1, y - 1, 0xffffffff);
	}

	inline void TipRender(const char* str, int x, int y) {
		int w = 0, h = 0;
		GetSize(str, w, h);
		x -= w / 2;
		GetRender().FillFrame(x - 2, y - 1, x + w + 2, y + h + 1, 0x90000000);
		Render(str, x - 1, y - 1, 0xffffffff);
	}

	// Алиасы для миграции со старого CGuiFont API.
	inline void RenderScale(const char* str, int x, int y, DWORD color, float scale) {
		Render(str, x, y, color, scale);
	}

	inline void RenderScale(int handle, const char* str, int x, int y,
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
