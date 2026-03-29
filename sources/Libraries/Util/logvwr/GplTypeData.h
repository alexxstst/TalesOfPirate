#pragma once

typedef std::map<int, std::string> MY_MAP;

class CGplTypeData
	{
public:
	CGplTypeData(LPSTR pszTypeName);
	~CGplTypeData(void);

	BOOL Is(LPCTSTR pszType) {return (*m_TypeName) == pszType;}
	void Enable(BOOL Enable = TRUE) {m_Enable = Enable;}
	BOOL IsEnabled() const {return m_Enable;}
	MY_MAP m_Map;

protected:
private:
	CString* m_TypeName;
	BOOL m_Enable;
	};
