
#include "StdAfx.h"
#include "LogTypeData.h"

CLogTypeData::CLogTypeData(CWnd* pMainWnd, LPSTR pszTypeName, UINT nID)
    {
    m_TypeName = new CString((LPCSTR) pszTypeName);

    CRect rc(0, 0, 0, 0);
    m_ListCtrl = new CListCtrl();
    m_ListCtrl->Create(WS_BORDER | LVS_SINGLESEL | LVS_REPORT | LVS_ALIGNLEFT,
                       rc, pMainWnd, nID);
    m_ListCtrl->SetExtendedStyle(LVS_EX_FULLROWSELECT |
                                 LVS_EX_GRIDLINES |
                                 LVS_EX_INFOTIP |
                                 m_ListCtrl->GetExtendedStyle());
    }

CLogTypeData::~CLogTypeData(void)
    {
    if (m_ListCtrl)
        {
        m_ListCtrl->DestroyWindow();
        delete m_ListCtrl;
        }

    if (m_TypeName)
        {
        delete m_TypeName;
        }
    }

