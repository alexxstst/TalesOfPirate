
#pragma once

#include "uiglobalvar.h"
#include "uiform.h"
#include "uilabel.h"
#include "uifastcommand.h"
#include "uiitemcommand.h"

namespace GUI
{
    class CComposeMgr : public CUIInterface
    {
        static const int NO_USE = -1;
        static const int COMPOSE_COUNT = 7;
        static const int ERNIE_SPEED = 50;
    public:
        CComposeMgr();
        virtual ~CComposeMgr();

        virtual void ShowComposeForm(bool bShow = true);
        virtual void CheckResult(short sRet, const char* txt);
        virtual void StartTime(short time);

        virtual int GetComIndex(COneCommand* oneCommand);
        virtual void DragToEquipGrid(int index);
        virtual void ClearCommand(bool bRetry = false);

    protected:
		virtual bool Init();
		virtual void CloseForm();

        virtual void PopItem(int iIndex, bool bRetry = false);
        virtual void PushItem(int iIndex, CItemCommand& rItem, bool bRetry = false);
        virtual void SetComposeUI();

        virtual void FrameMove(DWORD dwTime);

    private:
        static void _evtbtnForgeYes(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
        static void _evtbtnForgeSend(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
        static void _evtCloseCompose(CForm* pForm, bool& IsClose);
        static void _ProTimeArriveEvt(CGuiData *pSender);
        static void _evtEnterEvent(CGuiData *pSender);
        static void _evtDragItemBase1(CGuiData *pSender,CCommandObj* pItem,bool& isAccept);
        static void _evtDragItemBase2(CGuiData *pSender,CCommandObj* pItem,bool& isAccept);
        static void _evtDragItemBase3(CGuiData *pSender,CCommandObj* pItem,bool& isAccept);
        static void _evtDragItemBase4(CGuiData *pSender,CCommandObj* pItem,bool& isAccept);
        static void _evtDragItemBase5(CGuiData *pSender,CCommandObj* pItem,bool& isAccept);
        static void _evtDragItemBase6(CGuiData *pSender,CCommandObj* pItem,bool& isAccept);

    private:
        CForm*          frmCompose;
        COneCommand*    cmdCompose[COMPOSE_COUNT];
        int             iComposePos[COMPOSE_COUNT];
        CTextButton*	btnForgeYes;
        CTextButton*	btnForgeNo;
        CProgressBar*   proCompose;
        CLabelEx*       labInput;
        CEdit*          edtNumInput;
        DWORD           m_dwLastTickCount;
        long            sTime;
        long            lTimes;
        bool            bLock;
        bool            bRnd;
    };
};

