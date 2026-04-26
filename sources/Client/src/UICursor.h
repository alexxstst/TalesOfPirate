#pragma once

namespace GUI {
	class CCursor {
	public:
		enum eState {
			stNormal = 0, // 
			stActive = 1, // 
			stDrag = 2, // 
			stSize = 3, // 
			stVertical = 4, // 
			stLevel = 5, // 
			stWait = 6, // 	
			stAttack = 7, // 
			stSkillAttack = 8, // 
			stUpBank = 9, // 
			stUpBoat = 10, // 
			stStop = 11, // 
			stHover = 12, // 
			stPick = 13, // 
			stCamera = 14, // 
			stChat = 15, // 
			stMouse = 16, // 
			stButtonClick = 17, // 
			stHide = 18, // 
			stSearch = 19, // 
			stBlock = 20, // 
			stRepair = 21, // 
			stFeed = 22, // 
			stEnd, // 
		};

	public:
		CCursor();
		~CCursor();

		void Init();
		void SetCursor(eState v); // 
		eState GetCursor() {
			return _eActive;
		}

		bool SetFrame(eState v); // 
		void Restore() {
			_IsShowFrame = false;
		}

		int GetMax() {
			return stEnd;
		}

		int GetIndex() {
			return _eState;
		}

		void Render();

	public:
		static CCursor* I() {
			return &s_Cursor;
		}

	private:
		void _ShowCursor();

	private:
		static CCursor s_Cursor;

		bool _IsInit;
		bool _IsShowFrame;

		eState _eFrame; // 
		eState _eState; // 

		eState _eActive; // 
		HCURSOR _hCursor[stEnd];
	};

	class CWaitCursor {
	public:
		CWaitCursor() {
			CCursor::I()->SetCursor(CCursor::stWait);
		}

		CWaitCursor(CCursor::eState c) {
			CCursor::I()->SetCursor(c);
		}

		~CWaitCursor() {
			CCursor::I()->SetCursor(CCursor::stNormal);
		}
	};

	// 
	inline void CCursor::SetCursor(eState v) {
		if (v >= stNormal && v < stEnd) {
			_eState = v;
		}
	}

	inline bool CCursor::SetFrame(eState v) {
		if (v >= stNormal && v < stEnd) {
			_IsShowFrame = true;
			_eFrame = v;
			return true;
		}
		return false;
	}

	inline void CCursor::Render() {
		if (_IsShowFrame) {
			if (_eActive != _eFrame) {
				_eActive = _eFrame;
				_ShowCursor();
			}
		}
		else if (_eActive != _eState) {
			_eActive = _eState;
			_ShowCursor();
		}
	}
}
