#pragma once

#ifndef UI_HAIRCUT_FORM_H
#define UI_HAIRCUT_FORM_H

#include "UIGlobalVar.h"
#include "hairrecord.h"

class CHairTools;
namespace GUI
{

	class CForm;
	class CLabel;
	class CEdit;
	class CTextButton;

	class CHaircutMgr : public CUIInterface
	{
	public:
		CHaircutMgr();
		~CHaircutMgr();

		void ShowHaircutForm();			// 
		void Clear();					// 

		void Refresh(DWORD dwHairType, DWORD dwHairColor);
		void RenderCha(int x,int y);
	protected:
		virtual bool Init();
		virtual void CloseForm();
		virtual void SwitchMap();

	private:	
		enum eDirectType
		{
			LEFT = -1, RIGHT = 1,
		};

		
		void RotateCha(eDirectType enumDirect = LEFT);
		void ChangeHairColor(eDirectType enumDirect = LEFT);
		void ChangeHairType(eDirectType enumDirect = LEFT);

		//UI
		static void _MainMouseHaircutEvent(CCompent *pSender, int nMsgType, 
										   int x, int y, DWORD dwKey);
		static void _MainOnCloseEvent( CForm* pForm, bool& IsClose );

		static void	_gui_event_left_color(CGuiData *sender, int x, int y, DWORD key );
		static void	_gui_event_right_color(CGuiData *sender, int x, int y, DWORD key );
		static void	_gui_event_left_hair(CGuiData *sender, int x, int y, DWORD key );
		static void	_gui_event_right_hair(CGuiData *sender, int x, int y, DWORD key );
		static void _gui_event_left_rotate(CGuiData *sender, int x, int y, DWORD key );
		static void _gui_event_right_rotate(CGuiData *sender, int x, int y, DWORD key );
		static void _gui_event_left_continue_rotate(CGuiData *sender );
		static void _gui_event_right_continue_rotate(CGuiData *sender );


		static void	_cha_render_event( C3DCompent *pSender, int x, int y);


		//UI
		CForm			* frmHaircut;
		COneCommand		* cmdProp[defHAIR_MAX_ITEM];
		CLabel			* lblHairColor;
		CLabel			* lblHairType;
		CLabel			* lblHairFare;

		CCharacter		* m_pCurrMainCha;
		int				  m_nChaRotate;

		CHairTools		* m_pHairTools;
		DWORD			  m_dwHairTypeIndex;
		DWORD			  m_dwHairTypeMaxNum;
		DWORD			  m_dwHairColorIndex;
		DWORD			  m_dwHairColorMaxNum;

		short			  m_sScriptID;
		int				  m_iGoodsIndex[defHAIR_MAX_ITEM];

	};	// end of class CHaircutMgr

}	// end of namespace GUI

#endif // end of UI_HAIRCUT_FORM_H
