//----------------------------------------------------------------------
// :Edit
// :lh 2004-07-12
// :
//----------------------------------------------------------------------
#pragma once

namespace GUI {
	class CEditParse;
	class CEditStrategy;
	class CRichEdit;

	// 
	class CEditObj {
	public:
		virtual ~CEditObj() {
			Clear();
		}

		virtual void SetPos(int x, int y) {
		}

		virtual void GetSize(int& w, int& h) {
		}

		virtual void Render() {
		}

		virtual void Clear() {
		}

		virtual const char* GetCaption() {
			return "";
		}

		virtual DWORD GetWordCount() {
			return 0;
		}

		// 
		virtual void PushUnit(CEditObj* pObj) {
		}

		virtual void ClearChilds() {
		}

		virtual bool IsEmpty() {
			return true;
		}

		virtual void Parse(CEditStrategy* pParse);
	};

	// 
	class CEditTextObj : public CEditObj {
	public:
		CEditTextObj();

		virtual void SetCaption(const char*) {
		}

		virtual void AddCaption(const char*) {
		}

		virtual void Parse(CEditStrategy* pParse);

	public:
		void SetFont(DWORD dwFont) {
			_dwFont = dwFont;
		}

		DWORD GetFont() {
			return _dwFont;
		}

		void SetColor(DWORD dwColor) {
			_dwColor = dwColor;
		}

		DWORD GetColor() {
			return _dwColor;
		}

		// 
		bool IsSameType(CEditTextObj* pObj) {
			return pObj->GetColor() == GetColor() && pObj->GetFont() == GetFont();
		}

	private:
		DWORD _dwFont; // 
		DWORD _dwColor; // 
	};


	// ,
	class CEditChar : public CEditTextObj {
	public:
		CEditChar(char c1, char c2 = 0) {
			_szChar[0] = c1;
			_szChar[1] = c2;
			_szChar[2] = 0;
		}

		const char* GetCaption() {
			return _szChar;
		}

		DWORD GetWordCount() {
			return _szChar[1] == 0 ? 1 : 2;
		}

		void SetWord(char c1, char c2 = 0);

		const char* GetWord() {
			return _szChar;
		}

		int GetWidth() {
			return _nWidth;
		}

		// 
		void InvertRedner(int x, int y);

	protected:
		char _szChar[3];
		int _nWidth;
	};

	// 
	class CEditControl : public CEditObj {
	public:
		CEditControl(char c) : _cChar(c) {
		}

		virtual void Parse(CEditStrategy* pParse);

		virtual DWORD GetWordCount() {
			return 1;
		}

		char GetControl() {
			return _cChar;
		}

	private:
		char _cChar;
	};

	// 
	class CEditSentence : public CEditTextObj {
	public:
		CEditSentence();
		virtual void Render();

		virtual void SetPos(int x, int y) {
			_nX = x;
			_nY = y;
		}

		virtual void GetSize(int& w, int& h);

		void SetCaption(const char* str) {
			_szString = str;
		}

		const char* GetCaption() {
			return _szString.c_str();
		}

		void AddCaption(const char* str) {
			_szString += str;
		}

		bool AddChar(DWORD dwIndex, CEditChar* pChar);

	private:
		typedef std::vector<CEditChar*> chars;
		chars _chars;

		std::string _szString; // 
		int _nX, _nY;
	};

	// 
	class CEditArticle : public CEditObj {
	public:
		CEditArticle();
		~CEditArticle();

		virtual void Init();
		virtual void Render();
		virtual void Clear();

		void SetEdit(CRichEdit* pEdit) {
			_pEdit = pEdit;
		}

		bool AddControl(char c);
		bool AddChar(char c1, char c2 = 0);
		bool DelChar(unsigned int nIndex);

		void SetFont(DWORD dwFont) {
			_dwFontIndex = dwFont;
		}

		DWORD GetFont() {
			return _dwFontIndex;
		}

		void SetColor(DWORD dwColor) {
			_dwFontColor = dwColor;
		}

		DWORD GetColor() {
			return _dwFontColor;
		}

	public: // 
		void OnKeyDown(int key, bool IsShiftPress);
		void DelSelect();

	private:
		void RefreshText();

	private:
		void _ToSelectMode();
		bool _AddObj(CEditObj* pObj);

	private:
		enum eRunType {
			enumNormal, // 
			enumSelect, // 
		};

		CRichEdit* _pEdit;
		CEditStrategy* _pStrategy;

		typedef std::vector<CEditObj*> memorys;
		memorys _memorys; // ,

		eRunType _eRunType;

		unsigned int _nCursor; // 
		unsigned int _nSelStart; // 
		unsigned int _nSelEnd;

		unsigned int _dwWordMax; // ,0
		unsigned int _dwWordNum; // 

		DWORD _dwFontColor; // 
		int _dwFontIndex; // 
	};

	inline void CEditArticle::_ToSelectMode() {
		if (_eRunType == enumNormal) {
			_eRunType = enumSelect;
			_nSelStart = _nSelEnd = _nCursor;
		}
	}

	inline bool CEditArticle::AddControl(char c) {
		CEditControl* pChar = new CEditControl(c);
		if (!_AddObj(pChar)) {
			//delete pChar;
			SAFE_DELETE(pChar); // UI
			return false;
		}
		return true;
	}

	inline bool CEditArticle::AddChar(char c1, char c2) {
		CEditChar* pChar = new CEditChar(c1, c2);
		pChar->SetColor(_dwFontColor);
		pChar->SetFont(_dwFontIndex);
		if (!_AddObj(pChar)) {
			//delete pChar;
			SAFE_DELETE(pChar); // UI
			return true;
		}
		return false;
	}

	inline bool CEditSentence::AddChar(DWORD dwIndex, CEditChar* pChar) {
		if (!IsSameType(pChar)) return false;

		if (dwIndex >= _chars.size()) {
			_szString += pChar->GetWord();
			_chars.push_back(pChar);
		}
		else {
			chars::iterator where = _chars.begin() + dwIndex;
			_chars.insert(where, pChar);
			_szString = "";
			for (chars::iterator it = _chars.begin(); it != _chars.end(); it++)
				_szString = (*it)->GetWord();
		}
		return true;
	}
}
