//----------------------------------------------------------------------
// :
// :lh 2004-07-19
// :2004-10-09
//----------------------------------------------------------------------
#pragma once
#include "uicompent.h"

namespace GUI {
	// ,
	class CDragTitle : public CCompent {
	public:
		CDragTitle(CForm& frmOwn);
		CDragTitle(const CDragTitle& rhs);
		CDragTitle& operator=(const CDragTitle& rhs);
		virtual ~CDragTitle(void);
		GUI_CLONE(CDragTitle)

		virtual bool IsHandleMouse() {
			return true;
		}

		virtual bool MouseRun(int x, int y, DWORD key);
		virtual void Render();
		virtual void Refresh();

		void DragRender();

		void SetIsShowDrag(bool v) {
			_IsShowDrag = v;
		}

	public:
		CGuiPic* GetImage() {
			return _pImage;
		}

	private:
		void _SetSelf(const CDragTitle& rhs);

	protected:
		CGuiPic* _pImage;
		bool _IsShowDrag;
	};
}
