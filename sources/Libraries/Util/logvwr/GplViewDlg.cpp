// GplViewDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Logvwr.h"
#include "LogvwrDlg.h"
#include "GplViewDlg.h"
#include "GplTypeData.h"


// CGplViewDlg dialog

IMPLEMENT_DYNAMIC(CGplViewDlg, CDialog)
CGplViewDlg::CGplViewDlg(CWnd* pParent /*=NULL*/) : CDialog(CGplViewDlg::IDD, pParent)
    {
    m_Brush.CreateSolidBrush(RGB(0, 0, 0));
    m_pGPLData = NULL;
    m_pParent = (CLogvwrDlg *) pParent;
    }

CGplViewDlg::~CGplViewDlg()
    {
    }

void CGplViewDlg::DoDataExchange(CDataExchange* pDX)
    {
    CDialog::DoDataExchange(pDX);
    }


BEGIN_MESSAGE_MAP(CGplViewDlg, CDialog)
    ON_WM_PAINT()
    ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()


// CGplViewDlg message handlers

void CGplViewDlg::OnPaint()
    {
    CPaintDC dc(this); // device context for painting
    // TODO: Add your message handler code here
    // Do not call CDialog::OnPaint() for painting messages

    GetClientRect(m_RC);
    dc.FillRect(m_RC, &m_Brush); // 

    DrawGplData(dc);
    }

BOOL CGplViewDlg::DrawGplData(CPaintDC& dc)
    {
    if (m_pGPLData == NULL)
        {
        TRACE0("\n");
        return FALSE;
        }

    // 
    int size = int(m_pGPLData->size());
    if (size == 0)
        {
        TRACE0("\n");
        return FALSE;
        }

    // 
    dc.SetBkMode(TRANSPARENT);
	dc.SelectObject(GetFont());
    dc.SetTextColor(RGB(255, 0, 0));

    char buf[100] = {0};

    GPLDATALISTIT i;
    for (i = m_pGPLData->begin(); i != m_pGPLData->end(); ++ i)
        if ((*i)->IsEnabled())
            {
            // 
            for (std::map<int, std::string>::iterator it = (*i)->m_Map.begin();
                 it !=
                 (*i)->m_Map.end();
                 ++ it)
                {
                int id = (*it).first;
                std::string& str = (*it).second;
                int y = id / 2000;
                int x = id % 2000;
                dc.TextOut(x, y, str.c_str(), (int) str.size());
                }
            }

    return TRUE;
    }

void CGplViewDlg::OnRButtonDown(UINT nFlags, CPoint point)
    {
    // TODO: Add your message handler code here and/or call default

    try
        {
        // 
        POINT pt;
        ::GetCursorPos(&pt);

        CMenu* pSub = (m_pParent->m_pMainMenu)->GetSubMenu(1); // View
        pSub->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y,
                             m_pParent);
        }
    catch (...)
        {
        }

    CDialog::OnRButtonDown(nFlags, point);
    }

BOOL CGplViewDlg::OnCommand(WPARAM wParam, LPARAM lParam)
    {
    UINT nID = LOWORD(wParam); // ID
    int nCode = HIWORD(wParam); // 10
    HWND hWndCtrl = (HWND) lParam; // 00

    if ((nID > MY_LGMENU_BASE) &&
        (nID <= MY_LGMENU_BASE + UINT(m_pLGData->size())) &&
        (nCode == 0) &&
        (hWndCtrl == NULL))
        {
        // LGMENU
        m_pParent->PostMessage(WM_COMMAND, wParam, lParam);
        return TRUE;
        }
    else if ((nID > MY_GPLMENU_BASE) &&
             (nID <= MY_GPLMENU_BASE + UINT(m_pGPLData->size())) &&
             (nCode == 0) &&
             (hWndCtrl == NULL))
        {
        // GPLMENU
        m_pParent->PostMessage(WM_COMMAND, wParam, lParam);
        return TRUE;
        }
    else
        {
        return CDialog::OnCommand(wParam, lParam);
        }
    }

