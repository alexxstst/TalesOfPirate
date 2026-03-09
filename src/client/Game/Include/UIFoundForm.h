
#pragma once

#include "uiglobalvar.h"
#include "uiform.h"
#include "uilabel.h"
#include "uifastcommand.h"
#include "uiitemcommand.h"


namespace GUI
{
    class CFoundMgr : public CUIInterface
    {
        static const int NO_USE = -1;
        static const int FOUND_COUNT = 7;
        static const int ERNIE_SPEED = 50;
    public:
        CFoundMgr();
        virtual ~CFoundMgr();

        virtual void ShowFoundForm(bool bShow = true);
        virtual void CheckResult(short sRet, const char* num1, const char* num2, const char* num3);
        virtual void StartTime(short time);

        virtual int GetComIndex(COneCommand* oneCommand);
        virtual void DragToEquipGrid(int index);
        virtual void ClearCommand(bool bRetry = false);

    protected:
        virtual bool Init();
        virtual void CloseForm();

        virtual void PopItem(int iIndex, bool bRetry = false);
        virtual void PushItem(int iIndex, CItemCommand& rItem, bool bRetry = false);
        virtual void SetFoundUI();

        virtual void FrameMove(DWORD dwTime);

        virtual void ResetDice();

    private:
        static void _evtbtnForgeYes(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
        static void _evtCloseFound(CForm* pForm, bool& IsClose);
        static void _ProTimeArriveEvt(CGuiData *pSender);
        static void _evtDragItemBase1(CGuiData *pSender,CCommandObj* pItem,bool& isAccept);
        static void _evtDragItemBase2(CGuiData *pSender,CCommandObj* pItem,bool& isAccept);
        static void _evtDragItemBase3(CGuiData *pSender,CCommandObj* pItem,bool& isAccept);
        static void _evtDragItemBase4(CGuiData *pSender,CCommandObj* pItem,bool& isAccept);
        static void _evtDragItemBase5(CGuiData *pSender,CCommandObj* pItem,bool& isAccept);
        static void _evtDragItemBase6(CGuiData *pSender,CCommandObj* pItem,bool& isAccept);

		CForm*          frmFound;
        COneCommand*    cmdFound[FOUND_COUNT];
        int             iFoundPos[FOUND_COUNT];
        CImage*         Dice1[6];
        CImage*         Dice2[6];
        CImage*         Dice3[6];
        CTextButton*	btnForgeYes;
        CTextButton*	btnForgeBig;
        CTextButton*	btnForgeSmall;
        CProgressBar*   proFound;
        DWORD           m_dwLastTickCount;
        long            sTime;
        bool            bRnd;
        bool            bLock;
    };
};

