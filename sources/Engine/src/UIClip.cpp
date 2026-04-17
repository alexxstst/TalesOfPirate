#include "stdafx.h"
#include "MPFont.h"

// UIClip — клиппер для UI-текста. Реализация вынесена из MPFont.cpp при
// переходе на FontRender. GetIntersectRect оставлен как глобальная функция
// совместимости (используется в других файлах).

ui::UIClip* ui::UIClip::m_pClip = NULL;

namespace ui {
	UIClip::UIClip() {
		m_byState = 0;
	}

	UIClip::~UIClip() {
		SAFE_DELETE(m_pClip);
	}
}

bool GetIntersectRect(RECT* pdest, RECT* psrc, RECT* pclip) {
	int x1 = 0, x2 = 0, y1 = 0, y2 = 0;
	if (psrc->left >= pclip->right || psrc->right <= pclip->left
		|| psrc->top >= pclip->bottom || psrc->bottom <= pclip->top) {
		return false;
	}
	if (psrc->left < pclip->left) {
		x1 = psrc->left - pclip->left;
	}
	if (psrc->right > pclip->right) {
		x2 = pclip->right - psrc->right;
	}
	if (psrc->top < pclip->top) {
		y1 = psrc->top - pclip->top;
	}
	if (psrc->bottom > pclip->bottom) {
		y2 = pclip->bottom - psrc->bottom;
	}
	pdest->left   = psrc->left   - x1;
	pdest->right  = psrc->right  + x2;
	pdest->top    = psrc->top    - y1;
	pdest->bottom = psrc->bottom + y2;
	return true;
}

bool ui::UIClip::GetIntersectRect(RECT* pdest, RECT* psrc, BYTE* byFill) {
	int x1 = 0, x2 = 0, y1 = 0, y2 = 0;
	RECT* prc = &GetClipRect();
	if (psrc->left >= prc->right || psrc->right <= prc->left
		|| psrc->top >= prc->bottom || psrc->bottom <= prc->top) {
		return false;
	}
	if (byFill) {
		if (psrc->left >= prc->left && psrc->right <= prc->right
			&& psrc->top >= prc->top && psrc->bottom <= prc->bottom) {
			*byFill = 1;
			return true;
		}
	}

	if (psrc->left < prc->left) {
		x1 = psrc->left - prc->left;
	}
	if (psrc->right > prc->right) {
		x2 = prc->right - psrc->right;
	}
	if (psrc->top < prc->top) {
		y1 = psrc->top - prc->top;
	}
	if (psrc->bottom > prc->bottom) {
		y2 = prc->bottom - psrc->bottom;
	}
	pdest->left   = psrc->left   - x1;
	pdest->right  = psrc->right  + x2;
	pdest->top    = psrc->top    - y1;
	pdest->bottom = psrc->bottom + y2;
	return true;
}
