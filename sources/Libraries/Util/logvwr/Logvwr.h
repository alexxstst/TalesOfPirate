// Logvwr.h : PROJECT_NAME 
//

#pragma once

#ifndef __AFXWIN_H__
#error  PCH stdafx.h
#endif

#include "resource.h"       // 


// CLogvwrApp:
//  Logvwr.cpp
//

class CLogvwrApp : public CWinApp
    {
public:
    CLogvwrApp();

    // 
public:
    virtual BOOL InitInstance();

    // 

    DECLARE_MESSAGE_MAP()
    };

extern CLogvwrApp theApp;

class CLogUtil;
extern CLogUtil theLog;
