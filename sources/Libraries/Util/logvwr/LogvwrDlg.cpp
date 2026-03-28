// LogvwrDlg.cpp : 实现文件
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


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
    {
public:
    CAboutDlg();

    // 对话框数据
    enum {IDD = IDD_ABOUTBOX};

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

    // 实现
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


// CLogvwrDlg 对话框
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


// CLogvwrDlg 消息处理程序
const int FIELD_NUM = 6;
//static TCHAR FieldText[FIELD_NUM][16 * sizeof TCHAR] = {"NO", "时间", "毫秒",
//                                                        "间隔（毫秒）",
//                                                        "类型", "内容"};
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

    // 将\“关于...\”菜单项添加到系统菜单中。

    // IDM_ABOUTBOX 必须在系统命令范围内。
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

    // 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
    //  执行此操作
    SetIcon(m_hIcon, TRUE);         // 设置大图标
    SetIcon(m_hIcon, FALSE);        // 设置小图标

    // TODO: 在此添加额外的初始化代码
    // 创建GPL view的对话框
    m_pGplDlg = new CGplViewDlg(this);
    m_pGplDlg->Create(IDD_GPLVIEW);
    m_pGplDlg->SetLgDataList(&m_LGData);
    m_pGplDlg->SetGplDataList(&m_GPLData);

    // 得到当前主菜单对象指针
	m_PopMenu.LoadMenu(IDR_MAINMENU);
    m_pMainMenu = &m_PopMenu;

    // 初始化ListCtrl的header
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


    // 设置当前激活的ListCtrl为m_LogList
    m_pActiveWnd = &m_LogList;

	// 设置Timer#1
	SetTimer(1, 3000, NULL);

    return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
    }

void CLogvwrDlg::OnSysCommand(UINT nID, LPARAM lParam)
    {
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
        {
        DWORD style = GetExStyle();
        if (style & WS_EX_TOPMOST)
            { // 取消 TOPMOST 属性
            SetWindowPos(&wndNoTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
            CMenu* pSysMenu = GetSystemMenu(FALSE);
            if (pSysMenu != NULL)
                pSysMenu->CheckMenuItem(IDM_ABOUTBOX, MF_UNCHECKED);}
        else{ // 设置 TOPMOST 属性
            SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
            CMenu* pSysMenu = GetSystemMenu(FALSE);
            if (pSysMenu != NULL)
                pSysMenu->CheckMenuItem(IDM_ABOUTBOX, MF_CHECKED);}
        }
    else{CDialog::OnSysCommand(nID, lParam);}
    }

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CLogvwrDlg::OnPaint()
    {
    if (IsIconic())
        {
        CPaintDC dc(this); // 用于绘制的设备上下文

        SendMessage(WM_ICONERASEBKGND,
                    reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // 使图标在工作矩形中居中
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // 绘制图标
        dc.DrawIcon(x, y, m_hIcon);
        }
    else
        {
        CDialog::OnPaint();
        }
    }

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR CLogvwrDlg::OnQueryDragIcon()
    {
    return static_cast<HCURSOR>(m_hIcon);
    }

// 自添加消息处理函数
void CLogvwrDlg::OnSize(UINT nType, int cx, int cy)
    {
    if (m_Container.GetSafeHwnd())
        {
        int btn_area = 0;
        m_Container.MoveWindow(0, 0, cx, cy - btn_area);
        RECT rcCntnr;
        m_Container.GetClientRect(&rcCntnr);

        // 调整当前激活窗口
        try
            {
            if (m_pActiveWnd->GetSafeHwnd())
                {
                m_pActiveWnd->MoveWindow(&rcCntnr);
                }
            }
        catch (...)
            {
            // 如果出现异常，则显示所有LOG窗口
            if (m_LogList.GetSafeHwnd())
                {
                m_LogList.MoveWindow(&rcCntnr);
                }
            }

        // 调整按钮：测试和清除
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
        static CLogString type_str; // 此次LOG的类别

        // 解析LOG数据包
        (*m_pLogPacket) << (*pCopyDataStruct);

        // 取出包头
        (*m_pLogPacket) >> hdr;

        // 根据包头的dwReserved域判断对端的函数调用类型
        // 并做不同处理
        if (hdr.dwReserved == LG_CALL)
            {
            // 对端使用了LG函数，如：
            // LG("init", "(%d,%d)", x, y);

            // 增加接收包计数，并将序号放入所有包的ListCtrl
            ++ m_dwPacketCnt;

            // 根据包头中item的个数分配CLogString对象
            int num = min(FIELD_NUM - 1, hdr.dwItemCount);
            CLogString** pStrList = new CLogString*[num];

            // 将数据取出
            for (int i = 0; i < num; ++ i)
                {
                pStrList[i] = new CLogString();

                // 取出Item
                (*m_pLogPacket) >> (*pStrList[i]);

                // 第四个是类型，另存
                if (i == 3)
                    type_str = (*pStrList[i]);
                }


            // 寻找此次LOG的类别所对应的ListCtrl窗口对象，没有则会新建
            // 一个CLogTypeData对象
            CLogTypeData* pltd;
            CListCtrl* pList;
            if (HaveThisLGType(LPSTR(type_str), &pltd))
                {
                // 直接找到，可到ListCtrl窗口对象指针
                pList = (CListCtrl *) (*pltd);
                }
            else
                {
                // 要新建一个新的CLogTypeData对象，会在窗口退出时自动销毁
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

                // 将type_str插入到View菜单中
                CMenu* pSubMenu = m_pMainMenu->GetSubMenu(1); // File是位置0，View是位置1
                pSubMenu->AppendMenu(MF_STRING, nMenuID, LPSTR(type_str));
                }

			m_pActiveWnd->SetRedraw(FALSE);
			
            // 开始将数据插入两个ListCtrl中
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
			// 最多支持100条记录
			int n = m_LogList.GetItemCount();
			if (n > 100)
				m_LogList.DeleteItem(0); // 删除第一条

			
            // 2) This type ListCtrl
            // 注意：要重新计算时间间隔（专门针对此类ListCtrl）
            int type_cnt = pList->GetItemCount();
            static CString strLastPoint;
            static DWORD dwLastPoint;
            static CString strNewPoint;
            static DWORD dwNewPoint;
            static DWORD dwInterval;
            if (type_cnt > 0)
                {
                // 得到上一个此类LOG的毫秒点
                strLastPoint = pList->GetItemText(type_cnt - 1, 2);
                dwLastPoint = (DWORD) atoi(LPCSTR(strLastPoint));
                // 得到当前此类LOG的毫秒点
                strNewPoint = LPSTR(*pStrList[1]);
                dwNewPoint = (DWORD) atoi(LPCSTR(strNewPoint));
                if (dwNewPoint < dwLastPoint) // system start-up time wrap around
                    dwInterval = 0;
                else
                    dwInterval = dwNewPoint - dwLastPoint;

                // 替换时间间隔
                itoa(dwInterval, buf, 10);
                (*pStrList[2]) = buf;
                }
            else if (type_cnt == 0)
                {
                // 第一次，简单放个"0"
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
			// 最多支持100条记录
			n = pList->GetItemCount();
			if (n > 100)
				pList->DeleteItem(0); // 删除第一条

            // 销毁pStrList对象及内部子对象
            for (i = 0; i < num; ++ i)
                delete pStrList[i];
            delete pStrList;
            }
        else if (hdr.dwReserved == GPL_CALL)
            {
            // 对端使用了GPL函数，如：
            // GPL("FPS", 20, 20, "FPS : %d", fps);

            int num = hdr.dwItemCount;
            ASSERT(num == 3);

            CLogString** pStrList = new CLogString*[num];
            for (int i = 0; i < num; ++ i)
                {
                pStrList[i] = new CLogString();

                // 取出Item
                (*m_pLogPacket) >> (*pStrList[i]);
                }

            // pStrList[0]是本信息所属的类型名
            // pStrList[1]是唯一标识id
            // pStrList[2]是要输出的文本
            CGplTypeData* pgtd;
            int id = atoi(LPSTR(*pStrList[1]));
            if (HaveThisGPLType(LPSTR(*pStrList[0]), &pgtd))
                {
                // 已有，无需重建
                }
            else
                {
                // 创建一个新的CGplTypeData对象，并放入m_GPLData中
                UINT nMenuID = (UINT)
                               (MY_GPLMENU_BASE + 1 + m_GPLData.size());
                pgtd = new CGplTypeData(LPSTR(*pStrList[0]));
                ASSERT(pgtd != NULL);

                // 加入到m_GPLData中
                m_GPLData.push_back(pgtd);

                // 在View菜单中的分隔符前插入个菜单
                CMenu* pSubMenu = m_pMainMenu->GetSubMenu(1); // File是位置0，View是位置1
                UINT mcnt = pSubMenu->GetMenuItemCount();
                ASSERT(mcnt >= 3); // 一个GPL，一个分隔符，一个LG

                // 寻找分隔符的位置
                UINT sid = -1;
                int pos;
                for (pos = 0; pos < int(mcnt); ++ pos)
                    {
                    sid = pSubMenu->GetMenuItemID(pos);
                    if (sid == 0)
                        break;
                    }
                ASSERT(sid == 0);

                // 在分隔符前插入
                BOOL bRet = pSubMenu->InsertMenu(pos, MF_BYPOSITION, nMenuID,
                                                 LPSTR(*pStrList[0]));
                ASSERT(bRet);

                // 更新它的状态
                if (pgtd->IsEnabled())
                    {
                    // 打个对勾
                    pSubMenu->CheckMenuItem(nMenuID, MF_CHECKED);
                    }
                }

            // 插入数据
            pgtd->m_Map[id] = LPSTR(*pStrList[2]);


            // 销毁pStrList对象及内部子对象
            for (i = 0; i < num; ++ i)
                delete pStrList[i];
            delete pStrList;

            // 给GplView发个刷新消息来更新显示
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

// 处理自己的动态菜单消息
BOOL CLogvwrDlg::OnCommand(WPARAM wParam, LPARAM lParam)
    {
    UINT nID = LOWORD(wParam); // 控件、快捷键、菜单的ID
    int nCode = HIWORD(wParam); // 消息、1、0
    HWND hWndCtrl = (HWND) lParam; // 窗口句柄、0、0

    if ((nID > MY_LGMENU_BASE) &&
        (nID <= MY_LGMENU_BASE + UINT(m_LGData.size())))
        {
        if ((nCode == 0) && (hWndCtrl == NULL))
            {
            // 自己的动态LG的MENU消息
            CString txt;
            m_pMainMenu->GetMenuString(nID, txt, MF_BYCOMMAND); // 得到选定菜单的字符串

            // 切换到此窗口
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
            // 自己的动态GPL的MENU消息
            CString txt;
            m_pMainMenu->GetMenuString(nID, txt, MF_BYCOMMAND); // 得到选定菜单的字符串

            // 搜索是否有此类GPL
            CGplTypeData* pgtd = NULL;
            if (HaveThisGPLType((LPCTSTR) txt, &pgtd))
                {
                //
                if (pgtd->IsEnabled())
                    {
                    // 变为失效
                    pgtd->Enable(FALSE);
                    m_pMainMenu->CheckMenuItem(nID, MF_UNCHECKED);
                    }
                else
                    {
                    // 变为有效
                    pgtd->Enable();
                    m_pMainMenu->CheckMenuItem(nID, MF_CHECKED);
                    }

                // 刷新Gpl view
                m_pGplDlg->Invalidate(FALSE);
                }
            else
                {
                // 什么都不作
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

// 处理自己的动态ListCtrl消息
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
            // 弹出右键菜单
            POINT pt;
            GetCursorPos(&pt);
            CMenu* pSub = m_pMainMenu->GetSubMenu(1); // 得到View子菜单
            pSub->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y,
                                 this);
            return TRUE;
            }
        else if (pNMHDR->code == NM_DBLCLK)
            {
            // 回到所有LOG的窗口
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

// 用户点击清除按钮，将清除m_LogList中的所有内容
void CLogvwrDlg::OnBnClickedClearlog()
    {
    // TODO: Add your control notification handler code here

    }

// 测试给自己窗口发个数据包
void CLogvwrDlg::OnBnClickedTestlog()
    {
    // TODO: Add your control notification handler code here

    }


// 测试，弹出对话框，输入自定义消息，发出
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

    // 做一个虚假调用
    //OnSize(0, rcMainWnd.right - rcMainWnd.left, rcMainWnd.bottom - rcMainWnd.top);
    OnSize(0, rcMainWnd.right, rcMainWnd.bottom);

    // 找到容器的位置
    RECT rcCntnr;
    m_Container.GetClientRect(&rcCntnr);

    try
        {
        // 隐藏所有窗口
        std::vector<CLogTypeData*>::iterator i;
        CListCtrl* pList;
        for (i = m_LGData.begin(); i != m_LGData.end(); ++ i)
            {
            pList = (CListCtrl *) (**i);
            pList->ShowWindow(SW_HIDE);
            }
        m_LogList.ShowWindow(SW_HIDE);
        m_pGplDlg->ShowWindow(SW_HIDE);

        // 显示激活窗口
        if ((ActiveWnd.m_hWnd != m_LogList.m_hWnd) &&
            (ActiveWnd.m_hWnd != m_pGplDlg->m_hWnd))
            {
            // 动态LOG类别窗口调整下大小
            rcCntnr.left += 1;
            rcCntnr.right -= 1;
            rcCntnr.top += 1;
            rcCntnr.bottom -= 1;
            }

        m_pActiveWnd = &ActiveWnd;
        ActiveWnd.MoveWindow(&rcCntnr); // 放到容器中
        ActiveWnd.ShowWindow(SW_SHOW);
        }
    catch (...)
        {
        OutputDebugString("exception from SwitchView");
        }

    return TRUE;
    }

// 此函数只会被动态创建的LOG菜单处理例程调用
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

    // 切换到所有LOG窗口
    SwitchView(m_LogList);
    }

void CLogvwrDlg::OnNMRclickLoglist(NMHDR* pNMHDR, LRESULT* pResult)
    {
    // TODO: Add your control notification handler code here
    CListCtrl* pList = (CListCtrl*) GetDlgItem(int(pNMHDR->idFrom));
    ASSERT(pList != NULL);
    POINT pt;
    GetCursorPos(&pt);
    CMenu* pSub = m_pMainMenu->GetSubMenu(1); // 得到View子菜单
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

            // 切换到相应的ListCtrl窗口
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

    // 切换到GPL窗口
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

    ToLogService("common", "第{}条LOG消息", ++ cnt);
    GPL("FPS", 20, 20, "FPS : 第%d条LOG消息\n", ++ cnt);
    ToLogService("common", "第{}条LOG消息", ++ cnt);
    GPL("CPU", 20, 80, "CPU : 第%d条LOG消息\n", ++ cnt);
#endif
#if 0
	// 测试切分方法
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
	// 测试mp3和wav音乐播放
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
