//
#pragma once

#include <tchar.h>
#include <shlobj.h>
#include <olectl.h>
//#include <windows.h>

#include "lwutilsexp.h"
///////////////////////////////////

/////////////////

LWUTILS_API char* lwGetModuleFilePath( char* buf ); // buf size need be more than 256 byte
LWUTILS_API char* lwGetModuleBaseFileName( char* buf ); // exclude extension

LWUTILS_API char* lwGetPathFileName( char* buf, const char* path );
LWUTILS_API char* lwGetPathFileNameBase( char* buf, const char* path); // return name exclude ext name
LWUTILS_API char* lwGetPathFileNameExt( char* buf,const char* path); // return extension name of file name

LWUTILS_API char* lwGetPathFilePath( char* buf, const char* path );
LWUTILS_API char* lwGetPathSubPath( char* sub_path, const char* path, const char* compare_path );

// get sub_name of indexed file(et. test00 -->sub_name : test )
// pri_name[out]: return value, file[in]: name, id_len[in]: index character length(test00 id_len = 2)
LWUTILS_API char* lwGetIndexFileName( char* pri_name, const char* file, int id_len ); 

int LWUTILS_API lwGetOpenFileName(HWND hwnd,
                               char* buf,int num,
                               const char* dir,
                               const char* title = 0,
                               const char* filter = "all files(*.*)\0*.*\0\0",
                               int flag = OFN_PATHMUSTEXIST | 
                               OFN_EXPLORER 
                               //OFN_FILEMUSTEXIST | 
                               //OFN_ALLOWMULTISELECT |
                               //OFN_HIDEREADONLY
                               );

int LWUTILS_API lwGetSaveFileName(HWND hwnd,char* buf,int num,
                               const char* dir,
                               const char* title = 0,
                               const char* filter = "all files(*.*)\0*.*\0\0",
                               const char* ext = 0,
                               int flag = OFN_PATHMUSTEXIST | 
                               OFN_EXPLORER 
                               //OFN_FILEMUSTEXIST | 
                               //OFN_ALLOWMULTISELECT |
                               //OFN_HIDEREADONLY
                               );

int LWUTILS_API LwGetFolderName(char *folder, HWND hwnd,const char *title,LPITEMIDLIST pid_root=0);

HBITMAP LWUTILS_API LoadBitmapFile(const char* file);
OLE_HANDLE LWUTILS_API LoadImageFile(const char* file);


//
