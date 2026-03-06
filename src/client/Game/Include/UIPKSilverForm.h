
#pragma once

#include <string>
#include "uiglobalvar.h"
#include "uiform.h"
#include "uilabel.h"

#define MAX_PKSILVER_PLAYER     5


namespace GUI
{
    class CPKSilverMgr : public CUIInterface
    {
    public:
        CPKSilverMgr();
        virtual ~CPKSilverMgr();

        virtual void ShowPKSilverForm(bool bShow = true);
        virtual bool AddFormAttribute(int idx, const std::string& szName, long sLevel, const std::string& szJob, long lPkval);

	protected:
		virtual bool Init();
		virtual void CloseForm();

    private:
        //static void _evtPKSilverSortBtn(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);

    private:
        CForm*      frmPKSilver;
        CLabelEx*   labName[MAX_PKSILVER_PLAYER];
        CLabelEx*   labLevel[MAX_PKSILVER_PLAYER];
        CLabelEx*   labJob[MAX_PKSILVER_PLAYER];
        CLabelEx*   labDate[MAX_PKSILVER_PLAYER];

    };
};

