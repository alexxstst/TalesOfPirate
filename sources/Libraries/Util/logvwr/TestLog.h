#pragma once
#include "afxwin.h"


// CTestLog dialog

class CTestLog : public CDialog
	{
	DECLARE_DYNAMIC(CTestLog)

public:
	CTestLog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTestLog();

	// Dialog Data
	enum { IDD = IDD_TESTLOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
private:
	CEdit m_LogType;
	CEdit m_LogCtx;
	};
