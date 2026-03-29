// LogvwrDlg.cpp : 
//

#include "stdafx.h"
#include "Logvwr.h"
#include "LogvwrDlg.h"
#include "TestLog.h"
#include "LogTypeData.h"
#include "GplTypeData.h"
#include "GplViewDlg.h"


#include "packet.h"
#include "logutil.h"
#include "util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//  CAboutDlg 

class CAboutDlg : public CDialog
    {
public:
    CAboutDlg();

    // 
    enum {IDD = IDD_ABOUTBOX};

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 

    // 
protected:
    DECLARE_MESSAGE_MAP()
	};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
    {
    }

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
    {
    CDialog::DoDataExchange(pDX);
    }

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CLogvwrDlg 
CLogvwrDlg::CLogvwrDlg(CWnd* pParent /*=NULL*/) : CDialog(CLogvwrDlg::IDD, pParent),
                                                  m_CS()
    {
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

    m_pLogPacket = new CLogPacket();
    m_dwPacketCnt = 0;
    }

CLogvwrDlg::~CLogvwrDlg()
    {
    for (int i = 0; i < (int) m_LGData.size(); ++ i)
        delete m_LGData[i];

    for (i = 0; i < (int) m_GPLData.size(); ++ i)
        delete m_GPLData[i];

    m_pGplDlg->DestroyWindow();
	m_PopMenu.DestroyMenu();
    delete m_pGplDlg;
    delete m_pLogPacket;
    }

void CLogvwrDlg::DoDataExchange(CDataExchange* pDX)
    {
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_CONTAINER, m_Container);
    DDX_Control(pDX, IDC_LOGLIST, m_LogList);
    DDX_Control(pDX, IDC_TESTLOG, m_btnTestLog);
    DDX_Control(pDX, IDC_CLEARLOG, m_btnClearLog);
    }

BEGIN_MESSAGE_MAP(CLogvwrDlg, CDialog)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_WM_SIZE()
    ON_WM_COPYDATA()
    //}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_CLEARLOG, OnBnClickedClearlog)
    ON_BN_CLICKED(IDC_TESTLOG, OnBnClickedTestlog)
    ON_COMMAND(ID_FILE_TEST, OnFileTest)
    ON_COMMAND(ID_VIEW_ALL, OnViewAll)
    ON_NOTIFY(NM_RCLICK, IDC_LOGLIST, OnNMRclickLoglist)
    ON_NOTIFY(NM_DBLCLK, IDC_LOGLIST, OnNMDblclkLoglist)
    ON_COMMAND(ID_VIEW_GPL, OnViewGpl)
	ON_COMMAND(ID_VIEW_TEST, OnViewTest)
	ON_COMMAND(ID_VIEW_CLEAR, OnViewClear)
	ON_COMMAND(ID_VIEW_PLAYMP3, OnViewPlaymp3)
	ON_COMMAND(ID_VIEW_STOPMP3, OnViewStopmp3)
	ON_COMMAND(ID_VIEW_PLAYWAVE, OnViewPlaywave)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CLogvwrDlg 
const int FIELD_NUM = 6;
//static TCHAR FieldText[FIELD_NUM][16 * sizeof TCHAR] = {"NO", "", "",
//                                                        "",
//                                                        "", ""};
static TCHAR FieldText[FIELD_NUM][16 * sizeof TCHAR] = {"NO", "Time", "millisecond",
                                                        "interval(ms)",
                                                        "Type", "Content"};
static int FieldWidth[FIELD_NUM] = {40, 90, 70, 50, 70, 500};
static int FieldFormat[FIELD_NUM] = {LVCFMT_RIGHT, LVCFMT_CENTER,
                                     LVCFMT_RIGHT, LVCFMT_RIGHT,
                                     LVCFMT_CENTER, LVCFMT_LEFT};

BOOL CLogvwrDlg::OnInitDialog()
    {
    CDialog::OnInitDialog();

    // \...\

    // IDM_ABOUTBOX 
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != NULL)
        {
        CString strAboutMenu;
        strAboutMenu.LoadString(IDS_TOPMOST); //IDS_ABOUTBOX);
        if (!strAboutMenu.IsEmpty())
            {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
            }
        }

    // 
    //  
    SetIcon(m_hIcon, TRUE);         // 
    SetIcon(m_hIcon, FALSE);        // 

    // TODO: 
    // GPL view
    m_pGplDlg = new CGplViewDlg(this);
    m_pGplDlg->Create(IDD_GPLVIEW);
    m_pGplDlg->SetLgDataList(&m_LGData);
    m_pGplDlg->SetGplDataList(&m_GPLData);

    // 
	m_PopMenu.LoadMenu(IDR_MAINMENU);
    m_pMainMenu = &m_PopMenu;

    // ListCtrlheader
    LVCOLUMN lv;
    lv.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_FMT;
    for (int i = 0; i < FIELD_NUM; ++ i)
        {
        lv.cx = FieldWidth[i];
        lv.pszText = FieldText[i];
        lv.fmt = FieldFormat[i];
        m_LogList.InsertColumn(i, &lv);
        }
    m_LogList.SetExtendedStyle(LVS_EX_FULLROWSELECT |
                               LVS_EX_GRIDLINES |
                               LVS_EX_INFOTIP |
                               m_LogList.GetExtendedStyle());


    // ListCtrlm_LogList
    m_pActiveWnd = &m_LogList;

	// Timer#1
	SetTimer(1, 3000, NULL);

    return TRUE;  //  TRUE
    }

void CLogvwrDlg::OnSysCommand(UINT nID, LPARAM lParam)
    {
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
        {
        DWORD style = GetExStyle();
        if (style & WS_EX_TOPMOST)
            { //  TOPMOST 
            SetWindowPos(&wndNoTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
            CMenu* pSysMenu = GetSystemMenu(FALSE);
            if (pSysMenu != NULL)
                pSysMenu->CheckMenuItem(IDM_ABOUTBOX, MF_UNCHECKED);}
        else{ //  TOPMOST 
            SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
            CMenu* pSysMenu = GetSystemMenu(FALSE);
            if (pSysMenu != NULL)
                pSysMenu->CheckMenuItem(IDM_ABOUTBOX, MF_CHECKED);}
        }
    else{CDialog::OnSysCommand(nID, lParam);}
    }

// 
//  / MFC 
//  

void CLogvwrDlg::OnPaint()
    {
    if (IsIconic())
        {
        CPaintDC dc(this); // 

        SendMessage(WM_ICONERASEBKGND,
                    reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // 
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // 
        dc.DrawIcon(x, y, m_hIcon);
        }
    else
        {
        CDialog::OnPaint();
        }
    }

//
HCURSOR CLogvwrDlg::OnQueryDragIcon()
    {
    return static_cast<HCURSOR>(m_hIcon);
    }

// 
void CLogvwrDlg::OnSize(UINT nType, int cx, int cy)
    {
    if (m_Container.GetSafeHwnd())
        {
        int btn_area = 0;
        m_Container.MoveWindow(0, 0, cx, cy - btn_area);
        RECT rcCntnr;
        m_Container.GetClientRect(&rcCntnr);

        // 
        try
            {
            if (m_pActiveWnd->GetSafeHwnd())
                {
                m_pActiveWnd->MoveWindow(&rcCntnr);
                }
            }
        catch (...)
            {
            // LOG
            if (m_LogList.GetSafeHwnd())
                {
                m_LogList.MoveWindow(&rcCntnr);
                }
            }

        // 
        int btn_width;
        int btn_height;
        RECT rc;
        if (m_btnTestLog.GetSafeHwnd())
            {
            m_btnTestLog.GetWindowRect(&rc);
            btn_width = rc.right - rc.left;
            btn_height = rc.bottom - rc.top;
            m_btnTestLog.MoveWindow(cx / 10, cy - btn_area * 3 / 4, btn_width,
                                    btn_height);
            }
        if (m_btnClearLog.GetSafeHwnd())
            {
            m_btnClearLog.GetWindowRect(&rc);
            btn_width = rc.right - rc.left;
            btn_height = rc.bottom - rc.top;
            m_btnClearLog.MoveWindow(cx / 10 + 3 * btn_width / 2,
                                     cy - btn_area * 3 / 4, btn_width,
                                     btn_height);
            }
        }
    }

BOOL CLogvwrDlg::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
    {
    LOGPACKETHDR hdr;

    m_CS.Lock();

    try
        {
        static CLogString type_str; // LOG

        // LOG
        (*m_pLogPacket) << (*pCopyDataStruct);

        // 
        (*m_pLogPacket) >> hdr;

        // dwReserved
        // 
        if (hdr.dwReserved == LG_CALL)
            {
            // LG
            // LG("init", "(%d,%d)", x, y);

            // ListCtrl
            ++ m_dwPacketCnt;

            // itemCLogString
            int num = min(FIELD_NUM - 1, hdr.dwItemCount);
            CLogString** pStrList = new CLogString*[num];

            // 
            for (int i = 0; i < num; ++ i)
                {
                pStrList[i] = new CLogString();

                // Item
                (*m_pLogPacket) >> (*pStrList[i]);

                // 
                if (i == 3)
                    type_str = (*pStrList[i]);
                }


            // LOGListCtrl
            // CLogTypeData
            CLogTypeData* pltd;
            CListCtrl* pList;
            if (HaveThisLGType(LPSTR(type_str), &pltd))
                {
                // ListCtrl
                pList = (CListCtrl *) (*pltd);
                }
            else
                {
                // CLogTypeData
                UINT nListID = (UINT) (MY_LGLIST_BASE + 1 + m_LGData.size());
                UINT nMenuID = (UINT) (MY_LGMENU_BASE + 1 + m_LGData.size());
                pltd = new CLogTypeData(this, LPSTR(type_str), nListID);
                m_LGData.push_back(pltd);

                pList = (CListCtrl *) (*pltd);
                LVCOLUMN lv;
                lv.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_FMT;
                for (int i = 0; i < FIELD_NUM; ++ i)
                    {
                    lv.cx = FieldWidth[i];
                    lv.pszText = FieldText[i];
                    lv.fmt = FieldFormat[i];
                    pList->InsertColumn(i, &lv);
                    }

                // type_strView
                CMenu* pSubMenu = m_pMainMenu->GetSubMenu(1); // File0View1
                pSubMenu->AppendMenu(MF_STRING, nMenuID, LPSTR(type_str));
                }

			m_pActiveWnd->SetRedraw(FALSE);
			
            // ListCtrl
			// 1) All ListCtrl
            char buf[20];
            LVITEMA lv;
            lv.mask = LVIF_TEXT;
            // 1) All LOG ListCtrl
            lv.iItem = m_LogList.GetItemCount();
            lv.iSubItem = 0;
            lv.pszText = itoa(m_dwPacketCnt, buf, 10);
            m_LogList.InsertItem(&lv);
            for (i = 0; i < num; ++ i)
                {
                lv.iSubItem = i + 1;
                lv.pszText = LPSTR(*pStrList[i]);
                m_LogList.SetItem(&lv);
                }
			// 100
			int n = m_LogList.GetItemCount();
			if (n > 100)
				m_LogList.DeleteItem(0); // 

			
            // 2) This type ListCtrl
            // ListCtrl
            int type_cnt = pList->GetItemCount();
            static CString strLastPoint;
            static DWORD dwLastPoint;
            static CString strNewPoint;
            static DWORD dwNewPoint;
            static DWORD dwInterval;
            if (type_cnt > 0)
                {
                // LOG
                strLastPoint = pList->GetItemText(type_cnt - 1, 2);
                dwLastPoint = (DWORD) atoi(LPCSTR(strLastPoint));
                // LOG
                strNewPoint = LPSTR(*pStrList[1]);
                dwNewPoint = (DWORD) atoi(LPCSTR(strNewPoint));
                if (dwNewPoint < dwLastPoint) // system start-up time wrap around
                    dwInterval = 0;
                else
                    dwInterval = dwNewPoint - dwLastPoint;

                // 
                itoa(dwInterval, buf, 10);
                (*pStrList[2]) = buf;
                }
            else if (type_cnt == 0)
                {
                // "0"
                (*pStrList[2]) = "0";
                }
            else
                {
                TRACE0("fault : wrong item count!");
                }
            lv.iItem = pList->GetItemCount();
            lv.iSubItem = 0;
            lv.pszText = itoa(m_dwPacketCnt, buf, 10);
            pList->InsertItem(&lv);
            for (i = 0; i < num; ++ i)
                {
                lv.iSubItem = i + 1;
                lv.pszText = LPSTR(*pStrList[i]);
                pList->SetItem(&lv);
                }
			// 100
			n = pList->GetItemCount();
			if (n > 100)
				pList->DeleteItem(0); // 

            // pStrList
            for (i = 0; i < num; ++ i)
                delete pStrList[i];
            delete pStrList;
            }
        else if (hdr.dwReserved == GPL_CALL)
            {
            // GPL
            // GPL("FPS", 20, 20, "FPS : %d", fps);

            int num = hdr.dwItemCount;
            ASSERT(num == 3);

            CLogString** pStrList = new CLogString*[num];
            for (int i = 0; i < num; ++ i)
                {
                pStrList[i] = new CLogString();

                // Item
                (*m_pLogPacket) >> (*pStrList[i]);
                }

            // pStrList[0]
            // pStrList[1]id
            // pStrList[2]
            CGplTypeData* pgtd;
            int id = atoi(LPSTR(*pStrList[1]));
            if (HaveThisGPLType(LPSTR(*pStrList[0]), &pgtd))
                {
                // 
                }
            else
                {
                // CGplTypeDatam_GPLData
                UINT nMenuID = (UINT)
                               (MY_GPLMENU_BASE + 1 + m_GPLData.size());
                pgtd = new CGplTypeData(LPSTR(*pStrList[0]));
                ASSERT(pgtd != NULL);

                // m_GPLData
                m_GPLData.push_back(pgtd);

                // View
                CMenu* pSubMenu = m_pMainMenu->GetSubMenu(1); // File0View1
                UINT mcnt = pSubMenu->GetMenuItemCount();
                ASSERT(mcnt >= 3); // GPLLG

                // 
                UINT sid = -1;
                int pos;
                for (pos = 0; pos < int(mcnt); ++ pos)
                    {
                    sid = pSubMenu->GetMenuItemID(pos);
                    if (sid == 0)
                        break;
                    }
                ASSERT(sid == 0);

                // 
                BOOL bRet = pSubMenu->InsertMenu(pos, MF_BYPOSITION, nMenuID,
                                                 LPSTR(*pStrList[0]));
                ASSERT(bRet);

                // 
                if (pgtd->IsEnabled())
                    {
                    // 
                    pSubMenu->CheckMenuItem(nMenuID, MF_CHECKED);
                    }
                }

            // 
            pgtd->m_Map[id] = LPSTR(*pStrList[2]);


            // pStrList
            for (i = 0; i < num; ++ i)
                delete pStrList[i];
            delete pStrList;

            // GplView
            if (m_pActiveWnd->m_hWnd == m_pGplDlg->m_hWnd)
                {
                m_pGplDlg->Invalidate(FALSE);
				//m_pGplDlg->InvalidateRect(CRect(0, 0, 20, 20), FALSE);
                }
            }
        }
    catch (...)
        {
        OutputDebugString("exception from OnCopyData\n");
        }

    m_CS.Unlock();

    return TRUE;
    }

// 
BOOL CLogvwrDlg::OnCommand(WPARAM wParam, LPARAM lParam)
    {
    UINT nID = LOWORD(wParam); // ID
    int nCode = HIWORD(wParam); // 10
    HWND hWndCtrl = (HWND) lParam; // 00

    if ((nID > MY_LGMENU_BASE) &&
        (nID <= MY_LGMENU_BASE + UINT(m_LGData.size())))
        {
        if ((nCode == 0) && (hWndCtrl == NULL))
            {
            // LGMENU
            CString txt;
            m_pMainMenu->GetMenuString(nID, txt, MF_BYCOMMAND); // 

            // 
            SwitchView(txt);
            return TRUE;
            }
        return FALSE;
        }
    else if ((nID > MY_GPLMENU_BASE) &&
             (nID <= MY_GPLMENU_BASE + UINT(m_GPLData.size())))
        {
        if ((nCode == 0) && (hWndCtrl == NULL))
            {
            // GPLMENU
            CString txt;
            m_pMainMenu->GetMenuString(nID, txt, MF_BYCOMMAND); // 

            // GPL
            CGplTypeData* pgtd = NULL;
            if (HaveThisGPLType((LPCTSTR) txt, &pgtd))
                {
                //
                if (pgtd->IsEnabled())
                    {
                    // 
                    pgtd->Enable(FALSE);
                    m_pMainMenu->CheckMenuItem(nID, MF_UNCHECKED);
                    }
                else
                    {
                    // 
                    pgtd->Enable();
                    m_pMainMenu->CheckMenuItem(nID, MF_CHECKED);
                    }

                // Gpl view
                m_pGplDlg->Invalidate(FALSE);
                }
            else
                {
                // 
                }

            return TRUE;
            }
        return FALSE;
        }
    else
        {
        return CDialog::OnCommand(wParam, lParam);
        }
    }

// ListCtrl
BOOL CLogvwrDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
    {
    ASSERT(pResult != NULL);
    UINT nID = (UINT) wParam;

    if ((nID > MY_LGLIST_BASE) &&
        (nID <= MY_LGLIST_BASE + UINT(m_LGData.size())))
        {
        NMHDR* pNMHDR = (NMHDR*) lParam;
        HWND hWndCtrl = pNMHDR->hwndFrom;

        if (pNMHDR->code == NM_RCLICK)
            {
            // 
            POINT pt;
            GetCursorPos(&pt);
            CMenu* pSub = m_pMainMenu->GetSubMenu(1); // View
            pSub->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y,
                                 this);
            return TRUE;
            }
        else if (pNMHDR->code == NM_DBLCLK)
            {
            // LOG
            OnViewAll();
            return TRUE;
            }

        return FALSE;
        }
    else
        {
        return CDialog::OnNotify(wParam, lParam, pResult);
        }
    }

// m_LogList
void CLogvwrDlg::OnBnClickedClearlog()
    {
    // TODO: Add your control notification handler code here

    }

// 
void CLogvwrDlg::OnBnClickedTestlog()
    {
    // TODO: Add your control notification handler code here

    }


// 
void CLogvwrDlg::OnFileTest()
    {
    // TODO: Add your command handler code here
    CTestLog testLog;
    testLog.DoModal();
    }

BOOL CLogvwrDlg::HaveThisLGType(const char* szTypeName, CLogTypeData** ppltd)
    {
    std::vector<CLogTypeData*>::iterator i;

    for (i = m_LGData.begin(); i != m_LGData.end(); ++ i)
        if ((*i)->Is(szTypeName))
            {
            *ppltd = *i;
            return TRUE;
            }

    return FALSE;
    }

BOOL CLogvwrDlg::HaveThisGPLType(const char* szTypeName, CGplTypeData** ppgtd)
    {
    std::vector<CGplTypeData*>::iterator i;

    for (i = m_GPLData.begin(); i != m_GPLData.end(); ++ i)
        if ((*i)->Is(szTypeName))
            {
            *ppgtd = *i;
            return TRUE;
            }

    return FALSE;
    }

BOOL CLogvwrDlg::SwitchView(CWnd& ActiveWnd)
    {
    if (ActiveWnd.m_hWnd == m_pActiveWnd->m_hWnd)
        {
        return TRUE;
        }

    RECT rcMainWnd;
    GetClientRect(&rcMainWnd);

    // 
    //OnSize(0, rcMainWnd.right - rcMainWnd.left, rcMainWnd.bottom - rcMainWnd.top);
    OnSize(0, rcMainWnd.right, rcMainWnd.bottom);

    // 
    RECT rcCntnr;
    m_Container.GetClientRect(&rcCntnr);

    try
        {
        // 
        std::vector<CLogTypeData*>::iterator i;
        CListCtrl* pList;
        for (i = m_LGData.begin(); i != m_LGData.end(); ++ i)
            {
            pList = (CListCtrl *) (**i);
            pList->ShowWindow(SW_HIDE);
            }
        m_LogList.ShowWindow(SW_HIDE);
        m_pGplDlg->ShowWindow(SW_HIDE);

        // 
        if ((ActiveWnd.m_hWnd != m_LogList.m_hWnd) &&
            (ActiveWnd.m_hWnd != m_pGplDlg->m_hWnd))
            {
            // LOG
            rcCntnr.left += 1;
            rcCntnr.right -= 1;
            rcCntnr.top += 1;
            rcCntnr.bottom -= 1;
            }

        m_pActiveWnd = &ActiveWnd;
        ActiveWnd.MoveWindow(&rcCntnr); // 
        ActiveWnd.ShowWindow(SW_SHOW);
        }
    catch (...)
        {
        OutputDebugString("exception from SwitchView");
        }

    return TRUE;
    }

// LOG
BOOL CLogvwrDlg::SwitchView(CString& strActiveWnd)
    {
    std::vector<CLogTypeData*>::iterator i;
    CListCtrl* pList = NULL;
    for (i = m_LGData.begin(); i != m_LGData.end(); ++ i)
        if ((*i)->Is(LPCSTR(strActiveWnd)))
            {
            pList = (CListCtrl *) (**i);
            break;
            }

    if (pList == NULL)
        {
        return FALSE;
        }
    else
        {
        return SwitchView(*pList);
        }
    }

void CLogvwrDlg::OnViewAll()
    {
    // TODO: Add your command handler code here

    // LOG
    SwitchView(m_LogList);
    }

void CLogvwrDlg::OnNMRclickLoglist(NMHDR* pNMHDR, LRESULT* pResult)
    {
    // TODO: Add your control notification handler code here
    CListCtrl* pList = (CListCtrl*) GetDlgItem(int(pNMHDR->idFrom));
    ASSERT(pList != NULL);
    POINT pt;
    GetCursorPos(&pt);
    CMenu* pSub = m_pMainMenu->GetSubMenu(1); // View
    pSub->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, this); 

    *pResult = 0;
    }


void CLogvwrDlg::OnNMDblclkLoglist(NMHDR* pNMHDR, LRESULT* pResult)
    {
    // TODO: Add your control notification handler code here
    CListCtrl* pList = (CListCtrl*) GetDlgItem(int(pNMHDR->idFrom));
    ASSERT(pList != NULL);
    POSITION pos = pList->GetFirstSelectedItemPosition();
    if (pos == NULL)
        {
        TRACE0("No items were selected!\n");
        }
    else
        {
        while (pos)
            {
            int nItem = pList->GetNextSelectedItem(pos);
            TRACE1("Item %d was selected!\n", nItem);

            // ListCtrl
            CString txt = pList->GetItemText(nItem, 4);
            SwitchView(txt);
            }
        }

    *pResult = 0;
    }

// View the output of GPL(type, x, y, ...);
void CLogvwrDlg::OnViewGpl()
    {
    // TODO: Add your command handler code here

    // GPL
    SwitchView(*m_pGplDlg);
    }



void CLogvwrDlg::OnViewTest()
	{
	// TODO: Add your command handler code here
#if 0
    static int cnt = 0;
    int t = 0;
    srand((unsigned int) time(NULL));
#define RNDTIME (100 + (rand() % 9) * 10)
#define RSLEEP Sleep(RNDTIME)

    if (cnt >= 15) {cnt = 0;}

	char* type = "init";
	EnableAllLG();
	EnableLG(type, false);
	g_logManager.InternalLog(LogLevel::Debug, type, "you can't see this message");
	EnableLG(type);
	g_logManager.InternalLog(LogLevel::Debug, type, "you can see this message");
#endif

#if 0
    string dir;
    GetLGDir(dir);

    char input[80] = "claude";
    int in_len = int(strlen(input));
    string p;
    string cp;

    dbpswd_in(input, in_len, cp);

    dbpswd_out(cp.c_str(), cp.length(), p);

    
    //
    char md[21] = {0};
    char buf[256];
    int buf_len;

#define CHECK_CNT 300
    long sid[CHECK_CNT];
    long* ptr = (long *)md;

    char out_buf[256];

    for (int i = 0; i < CHECK_CNT; ++ i)
        {
        buf_len = sprintf(buf, "%s%d\n", input, i);//::GetTickCount());
        Sleep(2);
        sha1((unsigned char *)buf, buf_len, (unsigned char *)md);

        sid[i] = ptr[0];

        }

    // check
    int j;
    for (i = 0; i < CHECK_CNT; ++ i)
        for (j = i + 1; j < CHECK_CNT - i; ++ j)
            {
            if (sid[i] == sid[j])
                AfxMessageBox("equal!");
            }
#endif

#if 1
    KCHAPc cli("claude", "123456");
    KCHAPs svr;

    char buf[1024];    
    int out_len = 0;
    char key[1024];
    int key_len = 0;

    cli.SetChapString("20050606102931");
    cli.GetStep1Data(buf, sizeof buf, out_len);

    if (svr.DoAuth("claude", "1234567890", buf, out_len, cli.GetPwdDigest().c_str()))
    {
        svr.GetSessionClrKey(buf, sizeof buf, out_len);
        svr.GetSessionEncKey(buf, sizeof buf, out_len);
    }

    cli.GetSessionClrKey(buf, out_len, key, sizeof key, key_len);

    

#endif

#if 0
    cfl_db dbconn;
    string err_info;
    //if (!dbconn.connect("GameTEST", "usr", "22222", err_info))
    if (!dbconn.connect("192.168.1.228", "GameDB", "usr", "22222", err_info))
    {
        AfxMessageBox("Connect to Database error\n");
        return;
    }

    friend_tbl ftbl(&dbconn);
#define MAX_FRIEND_NUM 20
    friend_dat farray[MAX_FRIEND_NUM];
    int array_num = MAX_FRIEND_NUM;
    ftbl.get_friend_dat(farray, array_num,  1);
#endif

    unsigned char dig[16];
    char str[33];
    md5("12345678", dig);
    md5string("12345678", str);    

#if 0
    SetLGDir("d:/log");

    ToLogService("common", "hehe");

    ToLogService("common", "{}LOG", ++ cnt);
    GPL("FPS", 20, 20, "FPS : %dLOG\n", ++ cnt);
    ToLogService("common", "{}LOG", ++ cnt);
    GPL("CPU", 20, 80, "CPU : %dLOG\n", ++ cnt);
#endif
#if 0
	// 
	char Record[80] = {0};
	strcpy(Record, "1;");
	string strList[16];
	int n = Util_ResolveTextLine(Record, strList, 80, ';');
#endif 
	}

void CLogvwrDlg::OnViewClear()
	{
	// TODO: Add your command handler code here
    m_CS.Lock();
    try
        {
        std::vector<CLogTypeData*>::iterator i;
        CListCtrl* pList;
        for (i = m_LGData.begin(); i != m_LGData.end(); ++ i)
            {
            pList = (CListCtrl *) (**i);
            pList->DeleteAllItems();
            }
        m_LogList.DeleteAllItems();
        OnViewAll();
        m_dwPacketCnt = 0;
        }
    catch (...)
        {
        }
    m_CS.Unlock();
	}

#if defined(_DEBUG)
#pragma comment(lib, "utild.lib")
#else
#pragma comment(lib, "util.lib")
#endif

void CLogvwrDlg::OnViewPlaymp3()
	{
	// TODO: Add your command handler code here
	// mp3wav
	//int nRet = PlayBkMusic("west96.mp2");
	}

void CLogvwrDlg::OnViewStopmp3()
	{
	// TODO: Add your command handler code here
	//int nRet = StopBkMusic();
	}

void CLogvwrDlg::OnViewPlaywave()
	{
	// TODO: Add your command handler code here
#if 0
	PlayMusic("logon.wav");
	::Sleep(1500);
	PlayMusic("logon2.wav");
	::Sleep(1500);
	PlayMusic("logon3.wav");
	::Sleep(1500);
	PlayMusic("logon3.wav");
#endif
#if 0
    int id = env_snd_add("logon.wav");
    env_snd_play(id);
    Sleep(5000);
    env_snd_del(id);
#endif
    //cmn_snd_play("logon.wav", 128);
    //Sleep(5000);
    //cmn_snd_play("logon2.wav");
    //Sleep(5000);
    //cmn_snd_play("logon3.wav");
    //Sleep(5000);
    //cmn_snd_play("logon4.wav");
	}

void CLogvwrDlg::OnTimer(UINT nIDEvent)
	{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == 1)
		{
		m_pActiveWnd->SetRedraw(TRUE);
		m_pActiveWnd->Invalidate(FALSE);
		}

	CDialog::OnTimer(nIDEvent);
	}
