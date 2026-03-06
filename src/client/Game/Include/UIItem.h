//----------------------------------------------------------------------
// :
// :lh 2004-07-19
// :2004-10-09
//----------------------------------------------------------------------
#pragma once
#include "uiguidata.h"
#include <vector>
#include <algorithm>

struct txHighlight {
	int start;
	int len;
	int colour;
};

namespace GUI {
	// ItemObj
	class CItemRow {
	public:
		CItemRow(unsigned int max);
		CItemRow(const CItemRow& rhs);
		CItemRow& operator=(const CItemRow& rhs);
		~CItemRow();

		void SetBegin(CItemObj* p) {
			SetIndex(0, p);
		}

		CItemObj* GetBegin() {
			return _items[0];
		}


		void SetIndex(unsigned int v, CItemObj* p);

		CItemObj* GetIndex(unsigned int v) {
			return _items[v];
		}

		unsigned int GetMax() const {
			return _nMax;
		}

		void SetPointer(void* p) {
			_pTag = p;
		}

		void* GetPointer() {
			return _pTag;
		}

		void SetColor(DWORD c);

	private:
		void _Copy(const CItemRow& rhs);
		void _Clear();
		void _Init();

		CItemObj* _GetNullItem() {
			return &_NullObj;
		}

	private:
		static CItemObj _NullObj;

		CItemObj** _items;
		unsigned int _nMax;
		void* _pTag;
	};

	//
	class CItem : public CItemObj {
	public:
		CItem() : _dwColor(COLOR_RED), _nWidth(0), _nHeight(0) {
		}

		CItem(const std::string& str, DWORD c = COLOR_WHITE) : _str(str), _dwColor(c) {
			CGuiFont::s_Font.GetSize(str, _nWidth, _nHeight);
		}

		CItem(const CItem& rhs) : _str(rhs._str), _dwColor(rhs._dwColor), _nWidth(rhs._nWidth), _nHeight(rhs._nHeight) {
		}

		CItem& operator=(const CItem& rhs) {
			_str = rhs._str;
			_dwColor = rhs._dwColor;
			return *this;
		}

		ITEM_CLONE(CItem)

		virtual void Render(int x, int y);

		virtual int GetWidth() {
			return _nWidth;
		}

		virtual int GetHeight() {
			return _nHeight;
		}

		virtual void SetHeight(int n) {
			_nHeight = n;
		}

		virtual void SetWidth(int v) {
			_nWidth = v;
		}

		virtual void SetString(const std::string& str) {
			_str = str;
			CGuiFont::s_Font.GetSize(str, _nWidth, _nHeight);
		}

		virtual const std::string& GetString() {
			return _str;
		}

		void SetColor(DWORD c) {
			_dwColor = c;
		}

		DWORD GetColor() {
			return _dwColor;
		}

	protected:
		std::string _str;

		DWORD _dwColor;
		int _nWidth;
		int _nHeight;
	};

	class CColorItem : public CItem {
	public:
		CColorItem();

		virtual void Render(int x, int y);
		virtual void SetString(const char* str);

		virtual const std::string& GetString() {
			return _str;
		}

	private:
		struct ITEM_TEXT_DATA {
			ITEM_TEXT_DATA() {
				sxPos = 0;
				dwColor = 0xFF000000;
				strData = "";
			}

			USHORT sxPos;
			DWORD dwColor;
			std::string strData;
		};

		//
		typedef std::vector<ITEM_TEXT_DATA> ITEM_TEXT_ARRAY;
		ITEM_TEXT_ARRAY m_TextArray;

		void ParseScript(const char szScript[], USHORT sStartCom, DWORD dwDefColor);
	};

	//
	class CItemBar : public CItem {
	public:
		CItemBar();

		virtual void Render(int x, int y);

		void SetScale(float f) {
			if (f >= 0.0f && f <= 1.0f) _fScale = f;
		}

		static void LoadImage(const std::string& file, int w, int h, int tx = 0, int ty = 0);

	protected:
		static CGuiPic* _pBar;
		float _fScale;
	};

	//
	class CItemEx : public CItemObj {
	public:
		CItemEx() : _highlightColour(0x80FF0000), _isHighlighted(false), _isHighlightText(false), _dwColor(COLOR_WHITE),
					_bParseText(false), _nWidth(0), _nHeight(0), _bMultiLine(false), _nLineNum(1), m_Allign(eAlignTop),
					_HeadLen(0), _HeadColor(0) {
		}

		CItemEx(const char* str, DWORD c) : _highlightColour(0x80FF0000), _isHighlighted(false),
											_isHighlightText(false), _str(str), _dwColor(c), _bParseText(false),
											_bMultiLine(false), _nLineNum(1), m_Allign(eAlignTop) {
			CGuiFont::s_Font.GetSize(str, _nWidth, _nHeight);
		}

		CItemEx(const CItemEx& rhs) : _highlightColour(rhs._highlightColour), _isHighlighted(rhs._isHighlighted),
									  _isHighlightText(rhs._isHighlightText), _str(rhs._str), _dwColor(rhs._dwColor),
									  _nWidth(rhs._nWidth), _nHeight(rhs._nHeight), _bParseText(rhs._bParseText),
									  _bMultiLine(rhs._bMultiLine), _nLineNum(rhs._nLineNum), m_Allign(rhs.m_Allign) {
		}

		CItemEx& operator=(const CItemEx& rhs) {
			_highlightColour = rhs._highlightColour;
			_isHighlighted = rhs._isHighlighted;
			_isHighlightText = rhs._isHighlightText;
			_str = rhs._str;
			_dwColor = rhs._dwColor;
			m_Allign = rhs.m_Allign;
			return *this;
		}

		~CItemEx() {
		}

		ITEM_CLONE(CItemEx)

		virtual void Render(int x, int y);
		void RenderEx(int x, int y, int height, float scale);

		virtual void SetHeight(int n) {
			_nHeight = n;
		}

	public:
		int GetWidth();

		int GetHeight() {
			return _nHeight;
		}

		void SetString(const std::string& str) {
			_str = str;
			CGuiFont::s_Font.GetSize(str, _nWidth, _nHeight);
		}

		std::string& GetString() {
			return _str;
		}

		void SetColor(DWORD c) {
			_dwColor = c;
		}

		DWORD GetColor() {
			return _dwColor;
		}

		void SetIsParseText(bool v) {
			_bParseText = v;
		}

		bool GetIsParseText() {
			return _bParseText;
		}

		void SetHasHeadText(DWORD headLen, DWORD headColor);
		DWORD GetHeadLength();
		DWORD GetHeadColor();

		void SetIsMultiLine(bool v) {
			_bMultiLine = v;
		}

		bool GetIsMultiLine() {
			return _bMultiLine;
		}

		int GetLineNum() {
			return _nLineNum;
		}

		void SetAlignment(ALLIGN allign) {
			m_Allign = allign;
		}

		void SetItemName(const char* name) {
			_strItemName = name;
		}

		const char* GetItemName() {
			return _strItemName.c_str();
		}

		void ProcessString(int length); //


		//bool compareStart(const txHighlight &a, const txHighlight &b);
		void AddHighlightText(int start, int len, int colour);
		void renderHighlighted(int x, int y);
		bool _isHighlightText;
		bool _isHighlighted;
		int _highlightColour;
		void SetHighlighted(int colour = 0x80FF0000);


		std::vector<txHighlight> highlights;

	protected:
		std::string _str;
		std::string _strItemName;
		bool _bParseText; //
		bool _bMultiLine; //
		std::string _strLine[3]; //3
		DWORD _HeadLen;
		DWORD _HeadColor;

		DWORD _dwColor;
		int _nWidth;
		int _nHeight;
		int _nLineNum; //
		int _nMaxWidth; //
		ALLIGN m_Allign;
	};

	//
	inline void CItemRow::SetIndex(unsigned int v, CItemObj* p) {
		if (_items[v] != _GetNullItem() && _items[v] != p) {
			//delete _items[v];
			SAFE_DELETE(_items[v]); // UI
		}
		_items[v] = p;
	}

	inline int CItemEx::GetWidth() {
		if (!_bMultiLine)
			return _nWidth;
		else
			return _nMaxWidth;
	}
}
