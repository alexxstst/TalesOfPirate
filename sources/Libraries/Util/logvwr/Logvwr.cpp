// Logvwr.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "Logvwr.h"
#include "LogvwrDlg.h"

#include "LogUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CResourceBundleManage  g_ResourceBundleManage;

// CLogvwrApp

BEGIN_MESSAGE_MAP(CLogvwrApp, CWinApp)
    ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CLogvwrApp ����

CLogvwrApp::CLogvwrApp()
    {
    // TODO: �ڴ˴����ӹ�����룬
    // ��������Ҫ�ĳ�ʼ�������� InitInstance ��
    }


// Ψһ��һ�� CLogvwrApp ����

CLogvwrApp theApp;

// CLogvwrApp ��ʼ��

BOOL CLogvwrApp::InitInstance()
    {
    // ��ֻ֤����һ��ʵ��
    HANDLE hPrevMutex;
    HANDLE hMutex;
    LPCTSTR pszMutex = "mUtEx Of kOp";

    hPrevMutex = ::OpenMutex(MUTEX_ALL_ACCESS, FALSE, pszMutex);
    if (hPrevMutex)
        {
        ::CloseHandle(hPrevMutex);
        HWND hWnd = ::FindWindow(NULL, "kIng Of pIrAtEs lOgvIEwEr");
        if (hWnd)
            ::ShowWindow(hWnd, SW_SHOWMAXIMIZED);
        return FALSE;
        }

    // ����������
    hMutex = ::CreateMutex(NULL, 0, pszMutex);

    // ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
    // ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
    //����Ҫ InitCommonControls()�����򣬽��޷��������ڡ�
    InitCommonControls();

    CWinApp::InitInstance();

    if (!AfxSocketInit())
        {
        AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
        return FALSE;
        }

    AfxEnableControlContainer();

	// Add by lark.li
	if(!g_ResourceBundleManage.Init("E:\\Item\\Source\\VC2003\\Unicode\\Project_Kop\\vss_cs\\client\\bin\\system","zh_Hans_CN"))
	//if(!g_ResourceBundleManage.Init(".","zh_Hans_CN"))
	{
		AfxMessageBox("Cloud't load resource!\n");
		return -1;
	}
	else
	{
		// Заменено printf → логирование
		g_logManager.InternalLog(LogLevel::Debug, "common", RES_STRING(VW_LOGVWR_CPP_00001));
	}

    // ��׼��ʼ��
    // ���δʹ����Щ���ܲ�ϣ����С
    // ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
    // ����Ҫ���ض���ʼ������
    // �������ڴ洢���õ�ע�����
    // TODO: Ӧ�ʵ��޸ĸ��ַ�����
    // �����޸�Ϊ��˾����֯��
    //SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
    if (!::mus_mgr_init(true))
        {
        AfxMessageBox("mus_mgr_init failed!");
        return FALSE;}

    CLogvwrDlg dlg;
    m_pMainWnd = &dlg;
    INT_PTR nResponse = dlg.DoModal();
    if (nResponse == IDOK)
        {
        // TODO: �ڴ˷��ô�����ʱ�á�ȷ�������ر�
        //�Ի���Ĵ���
        }
    else if (nResponse == IDCANCEL)
        {
        // TODO: �ڴ˷��ô�����ʱ�á�ȡ�������ر�
        //�Ի���Ĵ���
        }

    // ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
    // ����������Ӧ�ó������Ϣ�á�
    ::mus_mgr_exit();
    ::CloseHandle(hMutex);
    return FALSE;
    }
