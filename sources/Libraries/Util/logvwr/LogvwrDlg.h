// LogvwrDlg.h : 
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CLogvwrDlg 
class CLogPacket;
class CLogTypeData;
class CGplTypeData;
class CGplViewDlg;
class CLogvwrDlg : public CDialog
    {
    // 
public:
    CLogvwrDlg(CWnd* pParent = NULL);   // 
	~CLogvwrDlg();

    // 
    enum {IDD = IDD_LOGVWR_DIALOG};

	// 
	CMenu* m_pMainMenu;

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 


    // 
protected:
    HICON m_hIcon;

    // 
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();

    // 
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	afx_msg BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
    DECLARE_MESSAGE_MAP()

private:
	// 
	CMenu m_PopMenu;

    // 
    CStatic m_Container;

	// 
	CWnd* m_pActiveWnd;

    // LOG
    CListCtrl m_LogList;

	// LOGListCtrl
	std::vector<CLogTypeData*> m_LGData;

	// GPL
	CGplViewDlg* m_pGplDlg;

	// GPL
	std::vector<CGplTypeData*> m_GPLData;

	// 
    CButton m_btnTestLog;
    CButton m_btnClearLog;

    // 
    CCriticalSection m_CS;

    // LOG
    CLogPacket* m_pLogPacket;

    // 
    DWORD m_dwPacketCnt;

public:
    afx_msg void OnBnClickedClearlog();
    afx_msg void OnBnClickedTestlog();
	afx_msg void OnFileTest();
	afx_msg void OnViewAll();
	afx_msg void OnNMRclickLoglist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkLoglist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnViewGpl();
	BOOL HaveThisLGType(const char* szTypeName, CLogTypeData** ppltd);
	BOOL HaveThisGPLType(const char* szTypeName, CGplTypeData** ppgtd);
	BOOL SwitchView(CWnd& ActiveWnd);
	BOOL SwitchView(CString& strActiveWnd);
    void ToggleTopMost();
	afx_msg void OnViewTest();
	afx_msg void OnViewClear();
	afx_msg void OnViewPlaymp3();
	afx_msg void OnViewStopmp3();
	afx_msg void OnViewPlaywave();
	afx_msg void OnTimer(UINT nIDEvent);
	};
