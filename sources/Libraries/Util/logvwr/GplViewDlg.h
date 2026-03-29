#pragma once
#include "atltypes.h"

// CGplViewDlg dialog

class CLogvwrDlg;

class CLogTypeData;
typedef std::vector<CLogTypeData*> LGDATALIST;
typedef LGDATALIST* PLGDATALIST;
typedef std::vector<CLogTypeData*>::iterator LGDATALISTIT;

class CGplTypeData;
typedef std::vector<CGplTypeData*> GPLDATALIST;
typedef GPLDATALIST* PGPLDATALIST;
typedef std::vector<CGplTypeData*>::iterator GPLDATALISTIT;

class CGplViewDlg : public CDialog
	{
	DECLARE_DYNAMIC(CGplViewDlg)

public:
	CGplViewDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGplViewDlg();

	void SetGplDataList(PGPLDATALIST pGPLData) {m_pGPLData = pGPLData;}
	void SetLgDataList(PLGDATALIST pLGData) {m_pLGData = pLGData;}

// Dialog Data
	enum { IDD = IDD_GPLVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
private:
	// 
	CLogvwrDlg* m_pParent;

	CRect m_RC;
	CBrush m_Brush;
	
	// 
	PGPLDATALIST m_pGPLData;
	PLGDATALIST m_pLGData;

public:
	BOOL DrawGplData(CPaintDC& dc);
	};
