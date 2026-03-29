// Logvwr.cpp : 
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


// CLogvwrApp 

CLogvwrApp::CLogvwrApp()
    {
    // TODO: 
    //  InitInstance 
    }


//  CLogvwrApp 

CLogvwrApp theApp;

// CLogvwrApp 

BOOL CLogvwrApp::InitInstance()
    {
    // 
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

    // 
    hMutex = ::CreateMutex(NULL, 0, pszMutex);

    //  Windows XP 
    //  ComCtl32.dll  6 
    // InitCommonControls()
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
		//  printf  
		g_logManager.InternalLog(LogLevel::Debug, "common", RES_STRING(VW_LOGVWR_CPP_00001));
	}

    // 
    // 
    // 
    // 
    // 
    // TODO: 
    // 
    //SetRegistryKey(_T(""));
    if (!::mus_mgr_init(true))
        {
        AfxMessageBox("mus_mgr_init failed!");
        return FALSE;}

    CLogvwrDlg dlg;
    m_pMainWnd = &dlg;
    INT_PTR nResponse = dlg.DoModal();
    if (nResponse == IDOK)
        {
        // TODO: 
        //
        }
    else if (nResponse == IDCANCEL)
        {
        // TODO: 
        //
        }

    //  FALSE 
    // 
    ::mus_mgr_exit();
    ::CloseHandle(hMutex);
    return FALSE;
    }
