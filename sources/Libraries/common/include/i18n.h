#pragma once

#include "ResourceBundleManage.h"		//Add by lark.li 20080303
#define RES_STRING(a) g_ResourceBundleManage.LoadResString("" #a  "")
#define RES_FORMAT_STRING(a, b, c) g_ResourceBundleManage.Format("" #a  "", b, c)

extern CResourceBundleManage g_ResourceBundleManage; //Add by lark.li 20080303
const char* ConvertResString(const char* str);

