//----------------------------------------------------------------------
// :
// :lh 2004-08-02
// :CPageItem,CPageCPageItem,CPageItem
// :2004-10-09
//----------------------------------------------------------------------
#pragma once
#include "uicompent.h"
#include "uitextbutton.h"
#include "uiimage.h"

namespace GUI {
	class CPage;

	// CPage
	class CPageItem : public CContainer {
		friend class CPage;

	public:
		enum eTitleState {
			tsNormal,
			tsActive,
			tsEnd,
		};

	public:
		CPageItem(CForm& frmOwn);
		CPageItem(const CPageItem& rhs);
		CPageItem& operator=(const CPageItem& rhs);
		virtual ~CPageItem(void);
		GUI_CLONE(CPageItem)

		virtual void Render();
		virtual void Refresh();
		virtual bool MouseRun(int x, int y, DWORD key);
		virtual void SetAlpha(BYTE alpha);

		virtual CCompent* GetHintCompent(int x, int y);

	public:
		void RenderTitle() {
			_pTitle->Render();
		}

		void SetIndex(unsigned int n) {
			_nIndex = n;
		}

		unsigned int GetIndex() {
			return _nIndex;
		}

		CImage* GetTitle() {
			return _pTitle;
		}

		CImage* GetBkgImage() {
			return _pImage;
		}

	protected:
		void _InActive(bool isActive);

	private:
		void _SetSelf();
		static void _OnTitleDown(CGuiData* pSender, int x, int y, DWORD key);
		void _OnTitleDown();

	protected:
		CImage* _pTitle; // ,CPage
		CImage* _pImage; // ,
		unsigned int _nIndex; // Page
	};

	// 
	class CPage : public CCompent {
	public:
		enum eButtonPos // 
		{
			bpLeftUp = 0, // 
			bpUpFull, // 
			bpCustom, // 
			bpEnd,
		};

	public:
		CPage(CForm& frmOwn);
		CPage(const CPage& rhs);
		CPage& operator=(const CPage& rhs);
		virtual ~CPage(void);
		GUI_CLONE(CPage)

		virtual void Init();
		virtual void Render();
		virtual void Refresh();
		virtual bool MouseRun(int x, int y, DWORD key);

		virtual bool IsHandleMouse() {
			return true;
		}

		virtual void SetAlpha(BYTE alpha);

		virtual CCompent* GetHintCompent(int x, int y);

		virtual CGuiPic* GetImage() {
			return _pImage;
		}

		virtual CCompent* GetHitCommand(int x, int y);

	public:
		void SetIndex(unsigned int n); // 
		int GetIndex() {
			return _nIndex;
		}

		CPageItem* GetPage(unsigned int n);

		CPageItem* NewPage(); // 

		void SetButtonSize(int w, int h);

		void SetButtonPutStyle(eButtonPos style) {
			_eButtonPos = style;
		}

		void SetButtonMargin(int n) {
			if (n > 0) _nButtonMargin = n;
		}

	public:
		GuiEvent evtSelectPage; // 

	private:
		void _RefreshButtonPos();

	protected:
		unsigned int _nIndex; // 		
		unsigned int _nMaxPage; // 

		CGuiPic* _pImage;

		typedef std::vector<CPageItem*> items;
		items _pages;

		eButtonPos _eButtonPos; // 
		int _nButtonWidth, _nButtonHeight; // 
		int _nButtonMargin; // 
	};

	// 
	inline void CPageItem::_OnTitleDown(CGuiData* pSender, int x, int y, DWORD key) {
		((CPageItem*)(pSender->GetParent()))->_OnTitleDown();
	}

	inline CPageItem* CPage::GetPage(unsigned int n) {
		if (n < _nMaxPage) return _pages[n];
		return NULL;
	}

	inline void CPage::SetButtonSize(int w, int h) {
		if ((w > 0) && (h > 0)) _nButtonWidth = w, _nButtonHeight = h;
	}
}
