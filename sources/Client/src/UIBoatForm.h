#pragma once
#include "UIGlobalVar.h"
#include "uiform.h"
#include "uigoodsgrid.h"

class CCharacter;

namespace GUI {
#define defMaxBoat 3 // 

	class CBoat // 
	{
	public:
		CBoat();
		bool Init(int n, CForm* ship, GuiDragInGridEvent evt);

		void Reset(); // 
		void Link(CCharacter* pBoat) {
			_chtBoat = pBoat;
		} // 
		bool GetIsValid() {
			return _chtBoat != NULL;
		} // 
		void UnLink() {
			_chtBoat = NULL;
		}

		CCharacter* GetCha() {
			return _chtBoat;
		} // 
		CGoodsGrid* GetGoodsGrid() {
			return _grdHold;
		} // 
		CForm* GetForm() {
			return _frmShipRoom;
		} // 

	private:
		static void _evtHoldGridRMouse(CGuiData* pSender, CCommandObj* pItem, int nGridID);
		static void _evtEscClose(CForm* pForm); //  ESC   add by Philip.Wu  2006-06-22

	private:
		int _nIndex;

		CCharacter* _chtBoat;
		CForm* _frmShipRoom;
		CGoodsGrid* _grdHold;
	};

	class CBoatMgr : public CUIInterface {
	public:
		void Clear();

		bool AddBoat(CCharacter* pBoat); // 
		CBoat* GetBoat(unsigned int n) {
			return &_cBoats[n];
		} // 
		CBoat* FindBoat(unsigned int ulWorldID); // 

		void SetHuman(CCharacter* p) {
			_pHuman = p;
		}

		CCharacter* GetHuman() {
			return _pHuman;
		} // 

		CGoodsGrid* FindGoodsGrid(unsigned int ulWorldID); // ID
		CCharacter* FindCha(unsigned int ulWorldID); // 
		CCharacter* FindCha(CGoodsGrid* pGoods); // 

		CCharacter* ChangeMainCha(unsigned int ulWorldID); // 

		CBoat* GetOtherBoat() {
			return &_cOther;
		} // 

	protected:
		virtual bool Init(); //
		virtual void End();

		virtual void SwitchMap() {
			Clear();
		} // 

	private:
		CBoat* GetFreeBoat(); // 

	private:
		CCharacter* _pHuman;
		CBoat _cBoats[defMaxBoat]; // 
		CBoat _cOther; // 
	};
}
