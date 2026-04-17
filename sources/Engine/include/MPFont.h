#pragma once

#include <algorithm>
#include <windows.h>
#include <d3d9.h>

#include "FontRender.h"

bool GetIntersectRect(RECT* pdest, RECT* psrc, RECT* pclip);

namespace ui
{
#define UI_STATE_NOCLIP		0
#define UI_STATE_CLIP		1

	class UIClip
	{
	public:
		static UIClip* Instance()
		{
			if (m_pClip) return m_pClip;
			m_pClip = new UIClip;
			return m_pClip;
		}

		void SetClipRect(int x, int y, int w, int h)
		{
			m_rectClip.left   = x;
			m_rectClip.top    = y;
			m_rectClip.right  = x + w;
			m_rectClip.bottom = y + h;
			m_byState = UI_STATE_CLIP;
		}

		void Reset() { m_byState = UI_STATE_NOCLIP; }

		BYTE  GetClipState() { return m_byState; }
		RECT& GetClipRect()  { return m_rectClip; }

		static UIClip* GetCliper() { return m_pClip; }

		bool GetIntersectRect(RECT* pdest, RECT* psrc, BYTE* byFill = NULL);

	protected:
		UIClip();
		~UIClip();

	private:
		BYTE            m_byState;
		RECT            m_rectClip;
		static UIClip*  m_pClip;
	};
}

//===================================================================
// FVF и флаги — сохранены для совместимости со старыми call-sites.
#define D3DFVF_FONT   (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define D3DFVF_3DFONT (D3DFVF_XYZB1  | D3DFVF_DIFFUSE | D3DFVF_TEX1)

#ifndef MPFONT_BOLD
#define MPFONT_BOLD   0x0001
#define MPFONT_ITALIC 0x0002
#define MPFONT_UNLINE 0x0004
#endif

// CMPFont — alias-через-наследование к FontRender. Старые call-sites
// продолжают работать без изменений: pointer/container типа CMPFont*
// разрешаются как FontRender*, все методы приходят из базы.
class CMPFont : public FontRender {};
