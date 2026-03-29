#include "StdAfx.h"
#include "GplTypeData.h"

CGplTypeData::CGplTypeData(LPSTR pszTypeName)
    {
    m_TypeName = new CString((LPCSTR) pszTypeName);
    m_Enable = TRUE;
    }

CGplTypeData::~CGplTypeData(void)
    {
    if (m_TypeName)
        {
        delete m_TypeName;
        }
    }
