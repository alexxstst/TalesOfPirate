// stdafx.h : 
// 
// 

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN        //  Windows 
#endif

// 
//  MSDN
#ifndef WINVER              //  Windows 95  Windows NT 4 
#define WINVER 0x0400       // Windows98  Windows 2000 
#endif

#ifndef _WIN32_WINNT        //  Windows NT 4 
#define _WIN32_WINNT 0x0400     // Windows98  Windows 2000 
#endif                      

#ifndef _WIN32_WINDOWS      //  Windows 98 
#define _WIN32_WINDOWS 0x0410 // Windows Me 
#endif

#ifndef _WIN32_IE           //  IE 4.0 
#define _WIN32_IE 0x0400    // IE 5.0 
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS  //  CString 

//  MFC 
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 
#include <afxext.h>         // MFC 
#include <afxdisp.h>        // MFC 
#include <afxmt.h>

#include <afxdtctl.h>       // Internet Explorer 4  MFC 
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>         // Windows  MFC 
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxsock.h>        // MFC 

#include <vector>
#include <map>
#include <string>

// LGID
enum {MY_LGMENU_BASE = 6000};
// LGListCtrlID
enum {MY_LGLIST_BASE = 6500};
// GPLID
enum {MY_GPLMENU_BASE = 7000};

#include "util.h"
#pragma comment(lib, "winmm.lib")

#include "ResourceBundleManage.h"			// Add by lark.li 20080228
#define RES_STRING(a) g_ResourceBundleManage.LoadResString("" #a  "")

//extern CResourceBundleManage  g_ResourceBundleManage; //Add by lark.li 20080128
