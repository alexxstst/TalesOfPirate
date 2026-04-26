//----------------------------------------------------------------------
// :
// :lh 2004-08-21
// :2004-10-09
//----------------------------------------------------------------------
#pragma once
#include "uiGuidata.h"

class CCharacter;

namespace GUI {
	const int FACE_MAX = 50;
	const int EVIL_MAX = 5;

	class CGuiPic;
	class CItemEx;

	class CHeadSay {
	public:
		CHeadSay(CCharacter* p);

		bool isTeamMember;
		bool isGuildMember;

		static bool Init();
		static bool Clear();

		void Reset() {
			_nShowTime = 0;
			_nFaceTime = 0;
		}

		void AddItem(CItemEx* obj);
		void Render(D3DXVECTOR3& pos);
		void RenderStateIcons(CCharacter* cha, int x, int y, float scale, float spacing, int rowSize, bool Rendertimer);
		// :nummax,attacknum
		void SetLifeNum(int num, int max);
		void SetManaNum(int num, int max);

		void SetIsShowLife(bool v) {
			_IsShowLife = v;
		}

		void SetIsShowMana(bool v) {
			_IsShowMana = v;
		}

		bool GetIsShowLife() {
			return _IsShowLife;
		}

		bool GetIsShowMana() {
			return _IsShowMana;
		}

		static bool GetIsShowEnemyNames() {
			return _ShowEnemyNames;
		} // Add by Mdr.st May 2020 - FPO alpha
		static bool GetIsShowBars() {
			return _ShowBars;
		}

		static bool GetIsShowPercentages() {
			return _ShowPercentages;
		}

		static bool GetIsShowInfo() {
			return _ShowInfo;
		}


		void SetIsShowName(bool v) {
			_IsShowName = v;
		}

		static void SetIsShowEnemyNames(bool v) {
			_ShowEnemyNames = v;
		} //Add by Mdr.st May 2020 - FPO alpha
		static void SetIsShowBars(bool v) {
			_ShowBars = v;
		}

		static void SetIsShowPercentages(bool v) {
			_ShowPercentages = v;
		}

		static void SetIsShowInfo(bool v) {
			_ShowInfo = v;
		}


		bool GetIsShowName() {
			return _IsShowName;
		}

		void SetNameColor(DWORD v) {
			_dwNameColor = v;
		}

		static void SetMaxShowTime(int v) {
			_nMaxShowTime = v;
		}

		static void SetMaxShowLiftTime(int n) {
			_nMaxShowLifeTime = n;
		}

		static void SetFaceFrequency(int n) {
			if (n > 0) _nFaceFrequency = n;
		}

		static CGuiPic* GetFacePic(unsigned int n) {
			if (n >= FACE_MAX) return NULL;
			return &_pImgFace[n];
		}

		static CGuiPic* GetShopPic(unsigned int n) {
			if (n >= 3) return NULL;
			return &_ImgShop[n];
		}

		static CGuiPic* GetShopPic2(unsigned int n) {
			if (n >= 3) return NULL;
			return &_ImgShop2[n];
		}

		static CGuiPic* GetLifePic() {
			return _pImgLife;
		}

		static CGuiPic* GetManaPic() {
			return _pImgMana;
		}

		static CGuiPic* GetLeaderPic() {
			return _pImgTeamLeaderFlag;
		}

		static CGuiPic* GetGuildLeaderPic() {
			return _pImgGuildLeaderFlag;
		}

		static void SetBkgColor(DWORD v) {
			_dwBkgColor = v;
		}

		bool SetFaceID(unsigned int faceid);

		int GetFaceID() {
			return _nCurFaceID;
		} //ID.by billy
		void SetRenderScale(float f) {
			_fScale = f;
		}

		void SetName(const std::string& name);

		bool InShop(int MouseX, int MouseY);

		static void RenderText(const char* szShopName, int x, int y);

		void SetEvilLevel(short sMaxEnergy);
		void SetIsShowEvil(bool bShow);

	private:
		static int _nMaxShowTime; // 

	private:
		int _nShowTime; // 
		float _fScale; //
		CItemEx* _pObj;
		std::string _str; //
		CCharacter* _pOwn;

	private: // 
		static int _nMaxShowLifeTime; // 

		static CGuiPic* _pImgLife;
		static CGuiPic* _pImgMana;
		static CGuiPic* _pImgTeamLeaderFlag; // 
		static CGuiPic* _pImgGuildLeaderFlag; // 
		static CGuiPic* _pImgShopHidden;

		float _fLifeW;
		int _fCurHp;
		int _fMxHp;

		float _fManaW;
		int _fCurSp;
		int _fMxSp;


		static unsigned int _nFaceFrequency;

		static CGuiPic* _pImgFace;
		static CGuiPic* _pImgEvil;
		static DWORD _dwBkgColor;

		int _nFaceTime; // 
		int _nCurFaceID; //ID
		DWORD _dwNameColor;

		CGuiPic* _pCurFace; // 
		unsigned int _nCurFaceFrame; // 
		unsigned int _nCurFaceCycle;

		bool _IsShowLife; // 
		bool _IsShowMana; // 
		bool _IsShowName; // added by billy
		static bool _ShowEnemyNames; // Add by Mdr.st May 2020 - FPO alpha
		static bool _ShowBars;
		static bool _ShowPercentages;
		static bool _ShowInfo;
		bool RenderDebuff;

		int _nChaNameOffX; // X

		// 
		enum {
			PRENAME_SEP1_INDEX = 0, //	(
			PRENAME_INDEX = 1, //	
			PRENAME_SEP2_INDEX = 2, //	)
			NAME_INDEX = 3, //	
			MOTTO_NAME_SEP1_INDEX = 4, //	(
			MOTTO_NAME_INDEX = 5, //	
			MOTTO_NAME_SEP2_INDEX = 6, //	)
			BOAT_NAME_SEP1_INDEX = 7, //	[
			BOAT_NAME_INDEX = 8, //	
			BOAT_NAME_SEP2_INDEX = 9, //	]

			NAME_PART_NUM = 10, //	
		};

		//
		// ()   + ()  []
		static char s_sNamePart[NAME_PART_NUM][64];
		//,
		static DWORD s_dwNamePartsColors[NAME_PART_NUM][2];
		//
		static char s_szName[1024];
		//
		static char s_szConsortiaNamePart[4][64];
		static char s_szConsortiaName[256];

		enum {
			NAME_LENGTH = 64,
		};

	private: // 
		static CGuiPic _ImgShop[3]; // 012
		static CGuiPic _ImgShop2[3];
		static int _nShopFrameWidth;
		static int _nShopFontYOff;
		void _RenderShop(const char* szShopName, int x, int y);
		void _RenderShop2(const char* szShopName, int x, int y);

		int _nShopX0, _nShopY0, _nShopX1, _nShopY1;

		short _sEvilLevel;
		bool _IsShowEvil;
	};

	inline bool CHeadSay::InShop(int nMouseX, int nMouseY) {
		return nMouseX > _nShopX0 && nMouseX < _nShopX1 && nMouseY > _nShopY0 && nMouseY < _nShopY1;
	}
}
