//
#include "stdafx.h"
#include "lwfileutil.h"
#include <assert.h>
#include <tchar.h>

char* lwGetModuleBaseFileName(char* buf) {
	char tmp[256];

	GetModuleFileName(0, tmp, 256);

	return lwGetPathFileNameBase(buf, tmp);
}

char* lwGetModuleFilePath(char* buf) {
	GetModuleFileName(0, buf, 256);

	char* p = buf + _tcslen(buf);

	for (; *p != '//' && *p != '\\'; --p);
	*p = 0;

	return buf;
}

char* lwGetPathFileName(char* buf, const char* path) {
	return _tcscpy(buf, _tcsrchr(path, '\\') + 1);
}

char* lwGetPathFileNameBase(char* buf, const char* path) {
	int i;

	const char* p = path + _tcslen(path) - 1;

	for (; *p != '\\'; --p);

	for (i = 0, p += 1; *p != '.'; p++) {
		buf[i++] = *p;
	}

	buf[i] = '\0';

	return buf;
}

char* lwGetPathFileNameExt(char* buf, const char* path) {
	char* p = (char*)_tcsrchr(path, '.');
	if (p == 0)
		return 0;
	p++;
	_tcsncpy_s(buf, _tcslen(p) + 1, p, _TRUNCATE);
	return buf;
}

char* lwGetPathFilePath(char* buf, const char* path) {
	_tcscpy(buf, path);
	char* p = _tcsrchr(buf, '\\');
	*p = '\0';

	return buf;
}

char* lwGetPathSubPath(char* sub_path, const char* path, const char* compare_path) {
	char p;
	char c;
	const char* ptr_p = path;
	const char* ptr_c = compare_path;


	for (; ((*ptr_c) != '\0') && ((*ptr_p) != '\0'); ptr_c++, ptr_p++) {
		c = (*ptr_c);
		p = (*ptr_p);

		if ((c >= 0x41) && (c <= 0x5a)) {
			c += 0x20;
		}
		if ((p >= 0x41) && (p <= 0x5a)) {
			p += 0x20;
		}

		if (c != p)
			return 0;
	}

	if (*ptr_p == 0)
		return 0;

	if (*ptr_p == '\\') {
		if (++ptr_p == 0)
			return 0;
	}

	_tcscpy(sub_path, ptr_p);

	return sub_path;
}

char* lwGetIndexFileName(char* pri_name, const char* file, int id_len) {
	size_t len = _tcslen(file) - id_len;
	if (len <= 0)
		return 0;

	_tcsncpy(pri_name, file, len);
	pri_name[len] = 0;

	return pri_name;
}

int lwGetOpenFileName(HWND hwnd, char* buf, int num, const char* dir, const char* title, const char* filter, int flag) {
	OPENFILENAME ofn; // common dialog box structure
	memset(&ofn, 0, sizeof(ofn));

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFile = buf;
	ofn.nMaxFile = num;
	ofn.lpstrFilter = filter;
	ofn.lpstrTitle = title;
	ofn.lpstrInitialDir = dir;
	ofn.Flags = flag;
	ofn.nFilterIndex = 1;
	// Display the Open dialog box. 
	if (GetOpenFileName(&ofn) != TRUE)
		return 0;

	// MultiSelected style , break file and directory
	if (flag & OFN_FILEMUSTEXIST) {
		*(ofn.lpstrFile + ofn.nFileOffset - 1) = '\0';
	}

	return 1;
}

int lwGetSaveFileName(HWND hwnd, char* buf, int num, const char* dir, const char* title, const char* filter,
					  const char* ext, int flag) {
	OPENFILENAME ofn; // common dialog box structure
	memset(&ofn, 0, sizeof(OPENFILENAME));

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFile = buf;
	ofn.nMaxFile = num;
	ofn.lpstrFilter = filter;
	ofn.lpstrTitle = title;
	ofn.lpstrInitialDir = dir;
	ofn.lpstrDefExt = ext;
	ofn.Flags = flag;
	ofn.nFilterIndex = 1;
	// Display the Open dialog box. 
	if (GetSaveFileName(&ofn) != TRUE)
		return 0;

	return 1;
}

// function not tested 
int lwGetFolderName(char* folder, HWND hwnd, const char* title, LPITEMIDLIST pid_root) {
	char buf[MAX_PATH];
	LPITEMIDLIST pid = pid_root;

	if (pid == NULL)
		::SHGetSpecialFolderLocation(hwnd, CSIDL_DRIVES, &pid);

	BROWSEINFO bi;
	bi.hwndOwner = hwnd;
	bi.pidlRoot = pid;
	bi.pszDisplayName = buf;
	bi.lpszTitle = title;
	bi.ulFlags = BIF_EDITBOX;
	bi.lpfn = NULL;
	bi.lParam = 0l;
	bi.iImage = 0;

	LPITEMIDLIST item_list = ::SHBrowseForFolder(&bi);

	if (item_list)
		::SHGetPathFromIDList(item_list, folder);

	LPMALLOC pMalloc = NULL;
	::SHGetMalloc(&pMalloc);
	(pMalloc)->Free(pid);

	return item_list ? 1 : 0;
}

//load image file

HBITMAP LoadBitmapFile(const char* file) {
	HBITMAP bmp = (HBITMAP)LoadImage(GetModuleHandle(NULL),
									 file,
									 IMAGE_BITMAP, 0, 0,
									 LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
	return bmp;
}

OLE_HANDLE LoadImageFile(const char* file) {
	// open file
	HANDLE hFile = CreateFile(file, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	assert(INVALID_HANDLE_VALUE != hFile);

	// get file size
	DWORD dwFileSize = GetFileSize(hFile, NULL);
	assert(-1 != dwFileSize);

	LPVOID pvData = NULL;
	// alloc memory based on file size
	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, dwFileSize);
	assert(NULL != hGlobal);

	pvData = GlobalLock(hGlobal);
	assert(NULL != pvData);

	DWORD dwBytesRead = 0;
	// read file and store in global memory
	BOOL bRead = ReadFile(hFile, pvData, dwFileSize, &dwBytesRead, NULL);
	assert(FALSE != bRead);
	GlobalUnlock(hGlobal);
	CloseHandle(hFile);

	LPSTREAM pstm = NULL;
	// create IStream* from global memory
	HRESULT hr = CreateStreamOnHGlobal(hGlobal, TRUE, &pstm);
	assert(SUCCEEDED(hr) && pstm);

	// Create IPicture from image file
	LPPICTURE gpPicture;

	hr = ::OleLoadPicture(pstm, dwFileSize, FALSE, IID_IPicture, (LPVOID*)&gpPicture);
	assert(SUCCEEDED(hr) && gpPicture);
	pstm->Release();

	OLE_HANDLE m_picHandle = 0;

	gpPicture->get_Handle(&m_picHandle);

	return m_picHandle;
}
