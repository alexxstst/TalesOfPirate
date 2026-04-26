//----------------------------------------------------------------------
// :
// :lh 2004-10-27
// :,,
// :
//----------------------------------------------------------------------
#pragma once
#include "UICommandCompent.h"
#include "UIScroll.h"

namespace GUI {
	typedef void (*GuiThrowItemEvent)(CGuiData* pSender, int id, bool& isThrow);
	typedef void (*GuiSwapItemEvent)(CGuiData* pSender, int nFirst, int nSecond, bool& isSwap);
	typedef void (*GuiDragInGridEvent)(CGuiData* pSender, CCommandObj* pItem, int nGridID, bool& isAccept);
	typedef void (*GuiRMouseGridEvent)(CGuiData* pSender, CCommandObj* pItem, int nGridID);

	class CCommandObj;

	class CGoodsGrid : public CCommandCompent {
	public:
		enum eShowStyle {
			enumSmall, // 
			enumSale, // 
			enumOwnDef // 
		};


		CGoodsGrid(CForm& frmOwn);
		CGoodsGrid(const CGoodsGrid& rhs);
		CGoodsGrid& operator=(const CGoodsGrid& rhs);
		~CGoodsGrid();
		GUI_CLONE(CGoodsGrid)

		virtual void Init();
		virtual void Render();
		virtual void Refresh();
		virtual bool MouseRun(int x, int y, DWORD key);
		virtual bool MouseScroll(int nScroll);
		virtual eAccept SetCommand(CCommandObj* p, int x, int y);
		virtual void Reset();

		virtual bool IsHandleMouse() {
			return true;
		}

		virtual CGuiPic* GetImage() {
			return _pImage;
		}

		virtual void SetMargin(int left, int top, int right, int bottom);

		virtual void SetAlpha(BYTE alpha);
		virtual void DragRender();
		virtual CCompent* GetHintCompent(int x, int y);
		virtual void RenderHint(int x, int y);

		virtual UseComandEvent GetUseCommantEvent() {
			return evtUseCommand;
		}

		CScroll* GetScroll() {
			return _pScroll;
		}

		CGuiPic* GetUnitImage() {
			return _pUnit;
		}

		void SetIsHint(bool v) {
			_IsShowHint = v;
		}

		void Clear();

	public:
		void SetShowStyle(eShowStyle v) {
			_eShowStyle = v;
		}

		bool SetContent(int nRow, int nCol);
		void SetSpace(int x, int y);
		void SetUnitSize(int w, int h);

		//Item selection
		void SetSelectEnable(bool v = false) {
			selectorEnabled = v;
		}

		bool IsItemSelected(int index) const;

		int GetSelectedItemCount() const;

		void ResetItemSelections();

		int GetFreeIndex();
		void GetFreeIndex(int* nFree, int& nCount, int nSize); // nFreeint,nCount,nSizenFree
		bool DelItem(int nIndex);
		bool SetItem(unsigned int nIndex, CCommandObj* pItem);
		bool SwapItem(unsigned int nFirst, unsigned int nSecond);

	public:
		// HACK: Right click menu in inventory needs to be able to set drag index to sell item
		// Initially a friend declaration was used in top-newera src, but here it didnt want to work
		void SetDragIndex(int index) {
			_nDragIndex = index;
		};

		int GetDragIndex() const {
			return _nDragIndex;
		}

		int GetCol() const {
			return _nCol;
		}

		int GetRow() const {
			return _nRow;
		}

		CCommandObj* GetItem(unsigned int nIndex);

		int GetMaxNum() const {
			return _nMaxNum;
		}

		int GetCurNum() const {
			return _nCurNum;
		}

		int FindCommand(CCommandObj* p); // ,-1 

		void SetItemValid(bool v);

		int GetFirstShow() const {
			return _nFirst;
		}

		int GetEmptyGridCount(); // 
		int GetUsedGridCount(); // 

	public: // 
		GuiThrowItemEvent evtThrowItem;
		GuiSwapItemEvent evtSwapItem;
		GuiDragInGridEvent evtBeforeAccept;
		GuiRMouseGridEvent evtRMouseEvent;

		UseComandEvent evtUseCommand;

	private:
		void _Copy(const CGoodsGrid& rhs);
		void _SetSelf();
		void _ClearItem();
		int _GetHitItem(int x, int y); // -1

	protected:
		static void _OnScrollChange(CGuiData* pSender) {
			((CGoodsGrid*)(pSender->GetParent()))->_OnScrollChange();
		}

		void _OnScrollChange();

	protected:
		CGuiPic* _pImage; // 
		CScroll* _pScroll;
		CGuiPic* _pUnit; // 

		CCommandObj** _pItems;

	protected:
		eShowStyle _eShowStyle;

		int _nUnitHeight, _nUnitWidth; //  
		int _nSpaceX, _nSpaceY; // 

		int _nRow, _nCol; // 
		int _nMaxNum; // ,

		// 
		int _nLeftMargin;
		int _nTopMargin;
		int _nRightMargin;
		int _nBottomMargin;

	private:
		static void _DragEnd(CGuiData* pSender, int x, int y, DWORD key) {
			((CGoodsGrid*)(pSender))->_DragEnd(x, y, key);
		}

		void _DragEnd(int x, int y, DWORD key);

	private:
		int _nStartX, _nStartY;
		int _nPageShowNum; // 
		int _nTotalW, _nTotalH;

		int _nFirst, _nLast; // 

		CCommandObj* _pDragItem;
		int _nDragIndex; // 
		int _nDragOffX, _nDragOffY;
		int _nDragRow, _nDragCol;

		int _nCurNum; // 
		bool _IsShowHint;

	private:
		static int _nTmpX, _nTmpY, _nTmpRow, _nTmpCol;

	private:
		int _nTmpIndex;

		bool selectorEnabled{false};
		std::unique_ptr<CGuiPic> selectItemImage{nullptr};
		std::unique_ptr<bool[]> selectedItemIndicies;
	};

	// 
	inline void CGoodsGrid::SetSpace(int x, int y) {
		if (x >= 0) _nSpaceX = x;
		if (y >= 0) _nSpaceY = y;
	}

	inline void CGoodsGrid::SetUnitSize(int w, int h) {
		if (w > 0) _nUnitWidth = w;
		if (h > 0) _nUnitHeight = h;
	}

	inline CCommandObj* CGoodsGrid::GetItem(unsigned int nIndex) {
		if (nIndex >= (unsigned int)_nMaxNum)
			return NULL;
		return _pItems[nIndex];
	}
}
