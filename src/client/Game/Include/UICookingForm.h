
#pragma once

#include "uiglobalvar.h"
#include "uiform.h"
#include "uilabel.h"
#include "uifastcommand.h"
#include "uiitemcommand.h"



namespace GUI
{
    class CCookingMgr : public CUIInterface
    {
        static const int NO_USE = -1;
        static const int COOKING_COUNT = 7;
    public:
        CCookingMgr();
        virtual ~CCookingMgr();

        virtual void ShowCookingForm(bool bShow = true);
        virtual void CheckResult(short sRet);

        virtual int GetComIndex(COneCommand* oneCommand);
        virtual void DragToEquipGrid(int index);
        virtual void ClearCommand(bool bRetry = false);

        virtual void StartTime(short time);

    protected:
        virtual bool Init();
        virtual void CloseForm();

        virtual void PopItem(int iIndex, bool bRetry = false);
        virtual void PushItem(int iIndex, CItemCommand& rItem, bool bRetry = false);
        virtual void SetCookingUI();

    private:
        static void _evtbtnForgeYes(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
        static void _evtCloseCooking(CForm* pForm, bool& IsClose);
        static void _ProTimeArriveEvt(CGuiData *pSender);
        static void _evtDragItemBase1(CGuiData *pSender,CCommandObj* pItem,bool& isAccept);
        static void _evtDragItemBase2(CGuiData *pSender,CCommandObj* pItem,bool& isAccept);
        static void _evtDragItemBase3(CGuiData *pSender,CCommandObj* pItem,bool& isAccept);
        static void _evtDragItemBase4(CGuiData *pSender,CCommandObj* pItem,bool& isAccept);
        static void _evtDragItemBase5(CGuiData *pSender,CCommandObj* pItem,bool& isAccept);
        static void _evtDragItemBase6(CGuiData *pSender,CCommandObj* pItem,bool& isAccept);

		CForm*          frmCooking;
        COneCommand*    cmdCooking[COOKING_COUNT];
        int             iCookingPos[COOKING_COUNT];
        CTextButton*	btnForgeYes;
        CTextButton*	btnForgeNo;
        CProgressBar*   proCooking;
        long            sTime;
        DWORD           dwStartTime;
        DWORD           dwEndTime;
        bool            bLock;
    };
};

