#pragma once

class CLogTypeData
	{
public:
	CLogTypeData(CWnd* pMainWnd, LPSTR pszTypeName, UINT nID);
	~CLogTypeData(void);

	BOOL Is(LPCTSTR pszType) {return (*m_TypeName) == pszType;}
	operator CListCtrl*() {return m_ListCtrl;}

private:
	CString* m_TypeName;
	CListCtrl* m_ListCtrl;
	};

