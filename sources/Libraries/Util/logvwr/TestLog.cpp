// TestLog.cpp : implementation file
//

#include "stdafx.h"
#include "Logvwr.h"
#include "TestLog.h"

// CTestLog dialog

IMPLEMENT_DYNAMIC(CTestLog, CDialog)
CTestLog::CTestLog(CWnd* pParent /*=NULL*/) : CDialog(CTestLog::IDD, pParent)
    {
    }

CTestLog::~CTestLog()
    {
    }

void CTestLog::DoDataExchange(CDataExchange* pDX)
    {
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LOGTYPE, m_LogType);
    DDX_Control(pDX, IDC_LOGCXT, m_LogCtx);
    }


BEGIN_MESSAGE_MAP(CTestLog, CDialog)
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CTestLog message handlers

void CTestLog::OnBnClickedOk()
    {
    // TODO: Add your control notification handler code here

    // LOG
    static char type[81] = {0};
    int cnt = 0;
    cnt = m_LogType.GetLine(0, type, sizeof type);
    if (cnt <= 0)
        {
        strcpy(type, "unknown");
        }
    else
        {
        type[min(cnt, 8)] = 0;
        } // 8

    // LOG
    static char ctx[81] = "this is log context";

    // 
    g_logManager.InternalLog(LogLevel::Debug, type, ctx);
    //GPL(type, 20, 100, ctx);

    OnOK();
    }
