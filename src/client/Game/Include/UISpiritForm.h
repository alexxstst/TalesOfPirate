#pragma once


#include "uiglobalvar.h"
#include "uiform.h"
#include "uiformmgr.h"
#include "uilabel.h"
#include "uitextbutton.h"
#include "uicheckbox.h"
#include "uiimage.h"
#include "uicommand.h"
#include "uifastcommand.h"


namespace GUI
{

	class CSpiritMgr : public CUIInterface
	{
	public:
		CSpiritMgr(void);
		~CSpiritMgr(void);

		static const int NO_USE = -1;
		static const int SPIRIT_MARRY_TYPE = 6;			// 

		static const int SPIRIT_MARRY_CELL_COUNT = 3;	// 
		static const int SPIRIT_MARRY_ITEM = 0;			// 
		static const int SPIRIT_MARRY_ONE  = 1;			// 1
		static const int SPIRIT_MARRY_TWO  = 2;			// 2

		static const int ERNIE_IMAGE_COUNT = 5;			// 
		static const int ERNIE_SPEED       = 50;		// 
		static const int ERNIE_COIN_COUNT  = 5;			// 5 
		static const int ERNIE_EMPTY_COUNT = 5;			// 5 

		void ClearAllCommand();
		void ShowMarryForm(bool bShow = true);
		void ShowErnieForm(bool bShow = true);

		void UpdateErnieNumber(short nNum, short nID1, short nID2, short nID3);	// 
		void UpdateErnieString(const char* szText);		// 
		void ShowErnieHighLight();

		int GetType() {  return SPIRIT_MARRY_TYPE;	}	// 

	protected:
		virtual bool Init();
		virtual void CloseForm();
		virtual void FrameMove(DWORD dwTime);


	private:

		//
		//  
		//
		CForm*		    frmSpiritMarry;
		CLabel*			labMoneyShow;
		CTextButton*	btnForgeYes;
		COneCommand*    cmdSpiritMarry[SPIRIT_MARRY_CELL_COUNT];

		int             m_iSpiritItemPos[SPIRIT_MARRY_CELL_COUNT];

		void		PushItem(int iIndex, CItemCommand& rItem);
		void		PopItem(int iIndex);
		void		SetSpiritUI();

		bool		IsValidSpiritItem(CItemCommand& rItem);	// 
		bool		IsValidSpirit(CItemCommand& rItem);		// LV > 20

		void		SendSpiritMarryProtocol();

		static void _evtDragMarryItem(CGuiData *pSender,CCommandObj* pItem,bool& isAccept);	// 
		static void _evtDragMarryOne(CGuiData *pSender,CCommandObj* pItem,bool& isAccept);	// 1
		static void _evtDragMarryTwo(CGuiData *pSender,CCommandObj* pItem,bool& isAccept);	// 2

		static void _evtMainMouseButton(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);// 
		static void _evtCloseMarryForm(CForm* pForm, bool& IsClose);	// 

	private:

		//
		//  
		//
		CForm*			frmSpiritErnie;
		CImage*			imgLine[9][ERNIE_IMAGE_COUNT];
		CCheckBox*		chkSetmoney[3];
		CTextButton*	btnStop[3];
		COneCommand*	cmdItem[9];
		CTextButton*	btnStart;

		CLabelEx*		labUsemoney[3];
		CLabelEx*		labLastshow1;
		CLabelEx*		labLastshow2;

		bool			m_bIsRunning[3];

		DWORD			m_dwLastTickCount;
		int				m_nCurrImage;

		void			AddTigerItem(short nNum, short sItemID);
		void			ClearTigerItem(void);
		void			ErnieHightLight(int nNum, bool b = true);

		static void _evtErnieMouseButton(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);// 
		static void _evtCloseErnieForm(CForm* pForm, bool& IsClose);	// 

	};

}

