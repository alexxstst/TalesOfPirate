//----------------------------------------------------------------------
// :
// :lh 2004-07-19
// :2004-10-09
//----------------------------------------------------------------------
#pragma once
#include "uicompent.h"

namespace GUI {
	// 
	class CLabel : public CCompent {
	public:
		CLabel(CForm& frmOwn);
		CLabel(const CLabel& rhs);
		virtual ~CLabel(void);
		GUI_CLONE(CLabel)

		virtual void Render();

		const char* GetCaption() {
			return _caption.c_str();
		}

		void SetCaption(const std::string& str) {
			_caption = str;
		}

		virtual void SetAlpha(BYTE alpha) {
			_color = (_color & 0x00ffffff) & (alpha << 24);
		}

		virtual void SetTextColor(DWORD color) {
			_color = color;
		}

		DWORD GetTextColor() {
			return _color;
		}

	protected:
		std::string _caption; // 
		DWORD _color; // 
	};

	// ,
	class CLabelEx : public CLabel {
	public:
		CLabelEx(CForm& frmOwn);
		CLabelEx(const CLabelEx& rhs);
		GUI_CLONE(CLabelEx)

		virtual void Render();

		void SetIsShadow(bool v) {
			_IsShadow = v;
		}

		void SetShadowColor(DWORD cl) {
			_clShadow = cl;
		}

		void SetFont(unsigned int nFont) {
			_nFontIndex = nFont;
		}

		void SetIsCenter(bool v) {
			_IsCenter = v;
		}

		void SetIsFlash(bool v) {
			_IsFlash = v;
		}

	private:
		void _Copy(const CLabelEx& rhs);

	protected:
		bool _IsShadow;
		DWORD _clShadow;
		bool _IsCenter;
		unsigned int _nFontIndex;
		bool _IsFlash;
	};
}
