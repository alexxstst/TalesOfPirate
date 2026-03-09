
#pragma once

#include "uiglobalvar.h"
#include "uiform.h"
#include "uilabel.h"
#include "uifastcommand.h"
#include "uiitemcommand.h"


namespace GUI
{
    class CBreakMgr : public CUIInterface
    {
        static const int NO_USE = -1;
        static const int BREAK_COUNT = 4;
        static const int ERNIE_SPEED = 50;
    public:
        CBreakMgr();
        virtual ~CBreakMgr();

        virtual void ShowBreakForm(bool bShow = true);
        virtual void CheckResult(short sRet, const char* num);
        virtual void StartTime(short time, const char* num);

        virtual int GetComIndex(COneCommand* oneCommand);
        virtual void DragToEquipGrid(int index);
        virtual void ClearCommand(bool bRetry = false);

    protected:
        virtual bool Init();
        virtual void CloseForm();

        virtual void PopItem(int iIndex, bool bRetry = false);
        virtual void PushItem(int iIndex, CItemCommand& rItem, bool bRetry = false);
        virtual void SetBreakUI();

        virtual void FrameMove(DWORD dwTime);

    private:
        static void _evtbtnForgeYes(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
        static void _evtCloseBreak(CForm* pForm, bool& IsClose);
        static void _ProTimeArriveEvt(CGuiData *pSender);
        static void _evtDragItemBase1(CGuiData *pSender,CCommandObj* pItem,bool& isAccept);
        static void _evtDragItemBase2(CGuiData *pSender,CCommandObj* pItem,bool& isAccept);
        static void _evtDragItemBase3(CGuiData *pSender,CCommandObj* pItem,bool& isAccept);
        static void _evtDragItemBase4(CGuiData *pSender,CCommandObj* pItem,bool& isAccept);

		CForm*          frmBreak;
        COneCommand*    cmdBreak[BREAK_COUNT];
        int             iBreakPos[BREAK_COUNT];
        CTextButton*	btnForgeYes;
        CTextButton*	btnForgeNo;
        CLabelEx*       labNumInput;
        CLabelEx*       labUsrInput;
        CProgressBar*   proBreak;
        long            sTime;
        DWORD           m_dwLastTickCount;
        bool            bRnd;
        bool            bLock;
    };
};

