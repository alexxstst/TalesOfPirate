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

    // 得到LOG的类型，单行
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
        } // 最多8个字符

    // 得到LOG的内容（这里特殊化了）
    static char ctx[81] = "this is log context";

    // 输出调试信息
    g_logManager.InternalLog(LogLevel::Debug, type, ctx);
    //GPL(type, 20, 100, ctx);

    OnOK();
    }
