//
#pragma once

#include <tchar.h>
#include <shlobj.h>
#include <olectl.h>



char* lwGetModuleFilePath( char* buf ); // buf size need be more than 256 byte
char* lwGetModuleBaseFileName( char* buf ); // exclude extension

char* lwGetPathFileName( char* buf, const char* path );
char* lwGetPathFileNameBase( char* buf, const char* path); // return name exclude ext name
char* lwGetPathFileNameExt( char* buf,const char* path); // return extension name of file name

char* lwGetPathFilePath( char* buf, const char* path );
char* lwGetPathSubPath( char* sub_path, const char* path, const char* compare_path );

// get sub_name of indexed file(et. test00 -->sub_name : test )
// pri_name[out]: return value, file[in]: name, id_len[in]: index character length(test00 id_len = 2)
char* lwGetIndexFileName( char* pri_name, const char* file, int id_len ); 

int lwGetOpenFileName(HWND hwnd,
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

int lwGetSaveFileName(HWND hwnd,char* buf,int num,
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

int LwGetFolderName(char *folder, HWND hwnd,const char *title,LPITEMIDLIST pid_root=0);

HBITMAP LoadBitmapFile(const char* file);
OLE_HANDLE LoadImageFile(const char* file);

