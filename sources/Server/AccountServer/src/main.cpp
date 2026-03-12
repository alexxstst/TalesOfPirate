// ������,����Ŀ�ṹ������������,�жദ�ط�Ƿȱ�߳�ͬ������,������д - by Arcol

// main.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "resource.h"
#include "AccountServer2.h"
#include <signal.h>
#include <CommCtrl.h>
//#include "BillThread.h"
#include "BTIService.h"
#include "GlobalVariable.h"

#include "inifile.h"

#define AUTHUPDATE_TIMER 1
#define GROUPUPDATE_TIMER 2

ThreadPool* comm = NULL;
ThreadPool* proc = NULL;
AuthThreadPool atp;
bool bExit = false;

//#pragma init_seg( lib )
pi_LeakReporter pi_leakReporter("accountmemleak.log");
CResourceBundleManage  g_ResourceBundleManage("AccountServer.loc");	//Add by lark.li 20080330

//BillThread bt;


//std::string g_BTSAddr[2] = {"61.152.115.79:7243", "61.152.115.79:7243"}; �ɵ�BTS��ַ
//std::string g_BTSAddr[2] = {"61.152.115.172:7243", "61.152.115.173:7243"};

void __cdecl Ctrlc_Dispatch(int sig)
{
    if (!bExit) {
		C_PRINT(RES_STRING(AS_MAIN_CPP_00007));
	    PostMessage(g_hMainWnd, WM_CLOSE, 0, 0);
        bExit = TRUE;
	}
}
LRESULT OnInitDialog(HWND hwnd)
{
    //SetWindowText(hwnd, "AccountServer");
    SetWindowText(hwnd, RES_STRING(AS_MAIN_CPP_00008));

	//��֤
	SetDlgItemText(hwnd,IDC_TOP,RES_STRING(AS_MAIN_CPP_00018));
	//���а��� 0
	SetDlgItemText(hwnd,IDC_QUEUECAP,RES_STRING(AS_MAIN_CPP_00019));
	//�������� 0
	SetDlgItemText(hwnd,IDC_TASKCNT,RES_STRING(AS_MAIN_CPP_00020));
	//GroupServer
	SetDlgItemText(hwnd,IDC_MID,RES_STRING(AS_MAIN_CPP_00021));
	//�Ʒ�
	SetDlgItemText(hwnd,IDC_BOTTOM,RES_STRING(AS_MAIN_CPP_00022));
	//�˳�
	SetDlgItemText(hwnd,IDOK,RES_STRING(AS_MAIN_CPP_00023));
    // Auth thread report list
    {
        HWND hAuthList = GetDlgItem(hwnd, IDC_AUTHLIST);

        DWORD dwStyle = ListView_GetExtendedListViewStyle(hAuthList);
        ListView_SetExtendedListViewStyle(hAuthList,
            dwStyle | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

        LVCOLUMN lv;
        lv.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_FMT;

        lv.cx = 50;
        //lv.pszText = "Thread";
        lv.pszText = (LPSTR)RES_STRING(AS_MAIN_CPP_00009);
        ListView_InsertColumn(hAuthList, 0, &lv);

        lv.cx = 100;
        //lv.pszText = "Run Label";
        lv.pszText = (LPSTR)RES_STRING(AS_MAIN_CPP_00010);
        lv.fmt = LVCFMT_CENTER;
        ListView_InsertColumn(hAuthList, 1, &lv);

        lv.cx = 150;
       // lv.pszText = "Last/Max Consume (ms)";
        lv.pszText = (LPSTR)RES_STRING(AS_MAIN_CPP_00011);
        lv.fmt = LVCFMT_CENTER;
        ListView_InsertColumn(hAuthList, 2, &lv);

        LVITEM item;
        item.mask = LVIF_TEXT;
        item.iSubItem = 0;
        char buf[80] = {0};
        for (char i = 0; i < AuthThreadPool::AT_MAXNUM; ++ i) {
            item.iItem = i;
            sprintf(buf, "#%02d", i + 1);
            item.pszText = (LPSTR)buf;
            ListView_InsertItem(hAuthList, &item);
        }
    }

    // Group report list
    {
        HWND hGroupList = GetDlgItem(hwnd, IDC_GROUPLIST);

        DWORD dwStyle = ListView_GetExtendedListViewStyle(hGroupList);
        ListView_SetExtendedListViewStyle(hGroupList,
            dwStyle | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

        LVCOLUMN lv;
        lv.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_FMT;

        lv.cx = 100;
        //lv.pszText = "Name";
        lv.pszText = (LPSTR)RES_STRING(AS_MAIN_CPP_00012);
        ListView_InsertColumn(hGroupList, 0, &lv);

        lv.cx = 100;
        //lv.pszText = "Group IP";
        lv.pszText = (LPSTR)RES_STRING(AS_MAIN_CPP_00013);
        lv.fmt = LVCFMT_CENTER;
        ListView_InsertColumn(hGroupList, 1, &lv);

        lv.cx = 100;
        //lv.pszText = "Status";
        lv.pszText = (LPSTR)RES_STRING(AS_MAIN_CPP_00014);
        lv.fmt = LVCFMT_CENTER;
        ListView_InsertColumn(hGroupList, 2, &lv);
    }

    // Bill report list
    {
        HWND hBillList = GetDlgItem(hwnd, IDC_BILLLIST);

        DWORD dwStyle = ListView_GetExtendedListViewStyle(hBillList);
        ListView_SetExtendedListViewStyle(hBillList,
            dwStyle | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

        LVCOLUMN lv;
        lv.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_FMT;

        lv.cx = 50;
        //lv.pszText = "BTS";
        lv.pszText = (LPSTR)RES_STRING(AS_MAIN_CPP_00015);
        ListView_InsertColumn(hBillList, 0, &lv);

        lv.cx = 150;
        //lv.pszText = "Server IP";
        lv.pszText = (LPSTR)RES_STRING(AS_MAIN_CPP_00016);
        lv.fmt = LVCFMT_CENTER;
        ListView_InsertColumn(hBillList, 1, &lv);

        lv.cx = 100;
        //lv.pszText = "Bill/Disconn Count";
        lv.pszText = (LPSTR)RES_STRING(AS_MAIN_CPP_00017);
        lv.fmt = LVCFMT_CENTER;
        ListView_InsertColumn(hBillList, 2, &lv);

        LVITEM item;
        item.mask = LVIF_TEXT;
        item.iSubItem = 0;
        char buf[80] = {0};
        for (char i = 0; i < 2; ++ i) {
            item.iItem = i;
            sprintf(buf, "#%02d", i + 1);
            item.pszText = (LPSTR)buf;
            ListView_InsertItem(hBillList, &item);
            //ListView_SetItemText(hBillList, i, 1, (LPSTR)g_BillThread.GetServerIP(i));
			ListView_SetItemText(hBillList, i, 1, (LPSTR)g_BillService.GetServerIP(i));
        }
    }

    // Update Timer
    SetTimer(hwnd, AUTHUPDATE_TIMER, 1000, NULL);
    SetTimer(hwnd, GROUPUPDATE_TIMER, 3000, NULL);

    return 0;
}
void ClearGroupList()
{
    HWND hGroupList = GetDlgItem(g_hMainWnd, IDC_GROUPLIST);
    ListView_DeleteAllItems(hGroupList);
}
BOOL AddGroupToList(char const* strName, char const* strAddr, char const* strStatus)
{
    HWND hGroupList = GetDlgItem(g_hMainWnd, IDC_GROUPLIST);

    LVITEM item;
    item.mask = LVIF_TEXT;
    item.iItem = 0;
	item.iSubItem = 0;
    item.pszText = (LPSTR)strName;
    int index = ListView_InsertItem(hGroupList, &item);
    ListView_SetItemText(hGroupList, index, 1, (LPSTR)strAddr);
    ListView_SetItemText(hGroupList, index, 2, (LPSTR)strStatus);

    return TRUE;
}
BOOL UpdateBTS(int index, char const* strStat)
{
    HWND hBTSList = GetDlgItem(g_hMainWnd, IDC_BILLLIST);
    ListView_SetItemText(hBTSList, index, 2, (LPSTR)strStat);
    return TRUE;
}
LRESULT OnTimer(HWND hwnd, UINT idEvent)
{
    if (idEvent == AUTHUPDATE_TIMER) {
        LVITEM item;
        item.mask = LVIF_TEXT;
        char buf[80] = {0};
        HWND hAuthList = GetDlgItem(hwnd, IDC_AUTHLIST);
        for (char i = 0; i < AuthThreadPool::AT_MAXNUM; ++ i) {
            item.iItem = i;

            item.iSubItem = 1;
            sprintf(buf, "%02d", AuthThreadPool::RunLabel[i]);
            item.pszText = (LPSTR)buf;
            ListView_SetItem(hAuthList, &item);

            item.iSubItem = 2;
            sprintf(buf, "%04d/%04d", AuthThreadPool::RunLast[i], AuthThreadPool::RunConsume[i]);
            item.pszText = (LPSTR)buf;
            ListView_SetItem(hAuthList, &item);
        }

        HWND hQueueCap = GetDlgItem(hwnd, IDC_QUEUECAP);
        //sprintf(buf, "���а��� %d", g_Auth.GetPkTotal());
        sprintf(buf, RES_STRING(AS_MAIN_CPP_00024), g_Auth.GetPkTotal());
        SetWindowText(hQueueCap, (LPCTSTR)buf);

        HWND hTaskCnt = GetDlgItem(hwnd, IDC_TASKCNT);
        //sprintf(buf, "�������� %d", proc->GetTaskCount());
        sprintf(buf, RES_STRING(AS_MAIN_CPP_00025), proc->GetTaskCount());
        SetWindowText(hTaskCnt, (LPCTSTR)buf);
    }
	else if (idEvent == GROUPUPDATE_TIMER)
	{
        g_As2->DisplayGroup();
    }

    return 0;
}
BOOL CALLBACK MainDlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    WORD wNotify = HIWORD(wParam);
    WORD wID = LOWORD(wParam);

    switch(uMsg) {
    case WM_INITDIALOG:
        OnInitDialog(hwndDlg);
        break;

    case WM_KEYDOWN:
        break;

    case WM_TIMER:
        OnTimer(hwndDlg, (UINT)wParam);
        break;

    case WM_COMMAND:
        if (wNotify == BN_CLICKED) {
            if (wID == IDOK) {
                PostMessage(hwndDlg, WM_CLOSE, 0, 0);
            }
            /*
            if (wID == IDC_CLOSEBTI) {
                g_bti.Close();
            } else if (wID == IDC_STARTBTI) {
                g_bti.Start("LckveRy", g_passport.c_str());
            } else if (wID == IDC_STOPBTI) {
                g_bti.Stop("LckveRy");
            } else if (wID == IDC_GETCREDIT) {
                DWORD tid = 0;
                CreateThread(NULL, 0, GetCredit, (LPVOID)5000, 0, &tid);
            } else if (wID == IDC_TEST) {
                DWORD tid = 0;
                for (int i = 0; i < 10; ++ i) {
                    CreateThread(NULL, 0, TestPerf, (LPVOID)i, 0, &tid);
                }
            } else if (wID == IDC_BUYS) {
                printf("TaskID = %d\n", g_bti.Buys(USERNAME, 50, "normal-gun"));
            } else if (wID == IDC_SALE) {
                printf("TaskID = %d\n", g_bti.Sale(USERNAME, 100, "gun"));
            } else if (wID == IDC_GIVE) {
                printf("TaskID = %d\n", g_bti.Give(USERNAME, 100, "green-gun"));
            } else if (wID == IDC_BILL) {
                printf("TaskID = %d\n", g_bti.Bill(USERNAME, 100, "blue-gun"));
            } else if (wID == IDC_DEAL) {
                printf("TaskID = %d\n", g_bti.Deal(USERNAME, "sam", 100, "orange-gun"));
            } else if (wID == IDC_OPENBTI) {
            }*/
        }
        break;

    case WM_CLOSE:
        KillTimer(hwndDlg, GROUPUPDATE_TIMER);
        KillTimer(hwndDlg, AUTHUPDATE_TIMER);
        PostQuitMessage(0);
        break;

    case cBTI_Opened:
        printf("cBTI_Opened [WPARAM=%d, LPARAM=%d]\n", wParam, lParam);
        break;

    case cBTI_Connected:
        printf("cBTI_Connected [WPARAM=%d, LPARAM=%d]\n", wParam, lParam);
        break;

    case cBTI_GAME:
        //printf("cBTI_GAME [WPARAM=%d, LPARAM=%d]\n", wParam, lParam);
		g_BillService.VerifyBillingCode((long)wParam, lParam);
        break;

    case cBTI_STOP:
        //printf("cBTI_STOP [WPARAM=%d, LPARAM=%d]\n", wParam, lParam);
        break;

    case cBTI_DATA:
        printf("cBTI_DATA [WPARAM=%d, LPARAM=%d]\n", wParam, lParam);
        break;

    case cBTI_BUYS:
        printf("cBTI_BUYS [WPARAM=%d, LPARAM=%d]\n", wParam, lParam);
        break;

    case cBTI_SALE:
        printf("cBTI_SALE [WPARAM=%d, LPARAM=%d]\n", wParam, lParam);
        break;

    case cBTI_DEAL:
        printf("cBTI_DEAL [WPARAM=%d, LPARAM=%d]\n", wParam, lParam);
        break;

    case cBTI_GIVE:
        printf("cBTI_GIVE [WPARAM=%d, LPARAM=%d]\n", wParam, lParam);
        break;

    case cBTI_BILL:
        printf("cBTI_BILL [WPARAM=%d, LPARAM=%d]\n", wParam, lParam);
        break;

    case cBTI_Disconnected:
		printf("cBTI_Disconnected [WPARAM=%d, LPARAM=%d]\n", wParam, lParam);
        /*
        printf("cBTI_Disconnected [WPARAM=%d, LPARAM=%d]\n", wParam, lParam);
        ++ g_nDisconn;
        char buf[20]; sprintf(buf, "�Ͽ�������%d", g_nDisconn);
        SetWindowText(GetDlgItem(hwndDlg, IDC_DISCONNCNT), buf);*/
        break;

    case cBTI_Closed:
        printf("cBTI_Closed [WPARAM=%d, LPARAM=%d]\n", wParam, lParam);
        break;
    case cBTI_TIME:     //  ������
        g_BillService.AdjustExpScale((long)wParam, (long)lParam);
        break;
	case WM_USER_LOG:
		{
			sUserLog *pUserLog=(sUserLog*)lParam;
			if (pUserLog->bLogin)
			{
				g_MainDBHandle.UserLogin(pUserLog->nUserID, pUserLog->strUserName, pUserLog->strLoginIP);
			}
			else
			{
				g_MainDBHandle.UserLogout(pUserLog->nUserID);
			}
			delete pUserLog;
			break;
		}
	case WM_USER_LOG_MAP:
		{
			sUserLog *pUserLog=(sUserLog*)lParam;
			if (pUserLog->bLogin)
			{
                g_MainDBHandle.UserLoginMap(pUserLog->strUserName, "Enter_Map");
			}
			else
			{
				g_MainDBHandle.UserLogoutMap(pUserLog->strUserName);
			}
			delete pUserLog;
			break;
		}
    }
    return FALSE;
}
HWND CreateMainDialog()
{
    HINSTANCE hInst = GetModuleHandle(NULL);
    HWND hwnd = ::CreateDialog(hInst, MAKEINTRESOURCE(IDD_MAINDIALOG), NULL, MainDlgProc);
    ::ShowWindow(hwnd, SW_SHOW);
    return hwnd;
}

//#include <string.h>

HANDLE hConsole = NULL;

int main(int argc, char* argv[])
{
	setvbuf(stdout, nullptr, _IONBF, 0);
	setvbuf(stderr, nullptr, _IONBF, 0);

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	C_TITLE("AccountServer.exe")
	C_PRINT("Loading AccountServer.cfg...\n");

	SEHTranslator translator;

	T_B

    // ����������
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );		//Add by Arcol (2005-12-2)

    g_hMainWnd = CreateMainDialog();
    signal(SIGINT, Ctrlc_Dispatch);
	g_MainThreadID=GetCurrentThreadId();

	if (!g_MainDBHandle.CreateObject())
	{
		C_PRINT("failed\n");
		//printf("Main database handler create failed, AccountServer hang!\n");
		printf(RES_STRING(AS_MAIN_CPP_00003));
		system("pause");
		return -1;
	}

    // ��ʼ��BTI
	//if (!g_BillThread.CreateBillingSystem(g_hMainWnd))
	if (!g_BillService.CreateBillingSystem(g_hMainWnd))
	{
		//printf("Cannot Create Billing System, AccountServer hang!\n");
		printf(RES_STRING(AS_MAIN_CPP_00004));
		system("pause");
		return -1;
	}

    // ������֤�Ʒ��߳�
	//bt.Launch();
	//g_BillThread.Launch();
    atp.Launch();

    // ��ʼ������
    TcpCommApp::WSAStartup();
    comm = ThreadPool::CreatePool(10, 10, 256);

    int nProcCnt = 2 * AuthThreadPool::AT_MAXNUM;
    proc = ThreadPool::CreatePool(nProcCnt, nProcCnt, 2048);
    try {
        g_As2 = new AccountServer2(proc, comm);
    } catch (excp& e) {
        printf("%s\n", e.what());
        comm->DestroyPool();
        TcpCommApp::WSACleanup();
        Sleep(10 * 1000);
        return -1;
    } catch (...) {
        //printf("AccountServer��ʼ���ڼ䷢��δ֪������֪ͨ������\n");
   		printf(RES_STRING(AS_MAIN_CPP_00006));
		comm->DestroyPool();
        TcpCommApp::WSACleanup();
        Sleep(10 * 1000);
        return -2;
    }

    // ��Ϣѭ��
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // ж������
    delete g_As2;
    if (comm != NULL) comm->DestroyPool();
    if (proc != NULL) proc->DestroyPool();
    TcpCommApp::WSACleanup();

    // ��֤���Ʒ��߳��˳�
    //bt.NotifyToExit();
	//g_BillThread.NotifyToExit();
    atp.NotifyToExit();

    atp.WaitForExit();
	//g_BillThread.WaitForExit(-1);

    // ��¼���״̬
    LG("RunLabel", "\n");
    for (char i = 0; i < AuthThreadPool::AT_MAXNUM; ++ i) {
        LG("RunLabel", "%02d %04d\n", AuthThreadPool::RunLabel[i],
            AuthThreadPool::RunConsume[i]);
    }
    LG("RunLabel", "\n");

	g_MainDBHandle.ReleaseObject();

	T_FINAL

    return 0;
}


