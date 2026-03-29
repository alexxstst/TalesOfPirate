#ifndef TAG_UTIL
#define TAG_UTIL

#ifdef WIN32
#pragma warning(disable : 4251)
#pragma warning(disable : 4786)

#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)			{ if((p) != NULL) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p)	{ if((p) != NULL) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)			{ if((p) != NULL) { (p)->Release(); (p)=NULL; } }
#endif

#include <stdio.h>
#include <math.h>

#include <string>
#include <list>
#include <vector>
#include <map>

//
namespace GAME {
	//==========================================
	// Singleton
	//==========================================
	template <typename T>
	class Singleton {
	public:
		Singleton() {
		}

		Singleton(int p1, int p2) {
		}

		static T* Instance();

		operator T*() {
			return _instance;
		}

	protected:
		static T* _instance;
	};

	template <typename T>
	T* Singleton<T>::_instance = NULL;


	template <typename T>
	T* Singleton<T>::Instance() {
		if (_instance == NULL) {
			_instance = new T;
		}
		return _instance;
	}
}


#include <io.h>
#include <direct.h>

#ifndef _MFC
#ifndef _WINDOWS_
#include <windows.h>
#endif
#endif


void StartTimeRecord();
DWORD EndTimeRecord();

void Util_TrimString(std::string& str);
void Util_TrimTabString(std::string& str);
long Util_GetFileSize(FILE* fp);
long Util_GetFileSizeByName(char* pszFileName);
unsigned long Util_GetSysTick();
void Util_Sleep(int ms);
bool Util_IsFileExist(char* pszName);
bool Util_MakeDir(const char* lpPath);
int Util_ResolveTextLine(const char* pszText, std::string strListp[], int nMax, int nSep = ' ', int nSep2 = '\t');

inline int Util_StringSkip(char** ppszDest, int nSep = ' ') {
	unsigned int i;
	unsigned int nLen = (unsigned int)(strlen(*ppszDest));
	for (i = 0; i < nLen; i++) {
		if (*(*ppszDest) != nSep) {
			break;
		}
		(*ppszDest)++;
	}
	return i;
}

inline float Str2Float(const std::string& str) {
	return static_cast<float>(atof(str.c_str()));
}

inline int Str2Int(const std::string& str) {
	return atoi(str.c_str());
}

inline void Util_ClearStrEndline(std::string& str) {
	size_t p = str.find('\r');
	if (p == std::string::npos) {
		p = str.find('\n');
	}
	if (p != std::string::npos) // 
	{
		str[p] = '\0';
	}
}

int Util_RelaceString(const char* pszSrc, const char* s1, const char* s2, std::string& str);

const char* Util_CurTime2String(int nFlag);
const struct tm* Util_GetCurTime();


#define DIRECTORY_OP_QUERY  1
#define DIRECTORY_OP_DELETE 2

static int g_nBeginTick = 0;

inline void BEGIN_TICK() {
	g_nBeginTick = GetTickCount();
}

inline int END_TICK(bool bLog = true, const char* pszTag = "") {
	int nInterval = GetTickCount() - g_nBeginTick;
	if (bLog) {
		//Log("%s Time Use %d ms\n", pszTag, nInterval);
	}
	return nInterval;
}

inline DWORD RNRGB(DWORD r, DWORD g, DWORD b, DWORD a = 255) {
	return (a << 24) | (r << 16) | (g << 8) | b;
}

inline DWORD RGB24TORGB15(DWORD dwRGB) {
	return ((dwRGB & 0xf80000) >> 9) | ((dwRGB & 0xf800) >> 6) | ((dwRGB & 0xff) >> 3);
}

inline DWORD RGB24TORGB12(DWORD dwRGB) {
	return ((dwRGB & 0xf00000) >> 12) | ((dwRGB & 0xf000) >> 8) | ((dwRGB & 0xf0) >> 4);
}

inline bool OutRect(const RECT& Rect, const int nX, const int nY) {
	return nX < Rect.left || nX >= Rect.right || nY < Rect.top || nY >= Rect.bottom;
}

inline float GetMixValue(float f1, float f2, float fProgress) {
	return f1 * (1.0f - fProgress) + f2 * fProgress;
}

inline DWORD GetMixValue(DWORD dw1, DWORD dw2, float fProgress) {
	return (DWORD)((float)dw1 * (1.0f - fProgress) + (float)dw2 * fProgress);
}

void ProcessDirectory(const char* pszDir, std::list<std::string>* pFileList, int nOperateFlag);

std::string GetMacString();

class MPTimer {
public:
	static DWORD dwFrequency;

	MPTimer() {
		if (dwFrequency == 1) {
			LARGE_INTEGER l;
			QueryPerformanceFrequency(&l); // current frequency
			dwFrequency = (DWORD)((double)l.QuadPart / (double)1000);
			// LG2("init", "CPU Frequency = %d\n", dwFrequency);
		}
		_dwTimeCount = 0;
	}


	void Begin() {
		QueryPerformanceCounter(&_liStartTime);
	}

	DWORD End() {
		LARGE_INTEGER liEndTime;
		QueryPerformanceCounter(&liEndTime);
		_dwTimeCount = (DWORD)(liEndTime.QuadPart - _liStartTime.QuadPart);
		_dwTimeCount = _dwTimeCount / dwFrequency;
		return _dwTimeCount;
	}

	DWORD GetTimeCount() {
		return _dwTimeCount;
	}

protected:
	LARGE_INTEGER _liStartTime;
	DWORD _dwTimeCount;
};


class CBlockData // , 
{
public:
	CBlockData() {
	}

	~CBlockData() {
		SAFE_DELETE_ARRAY(_pbtData);
	}

	BOOL Load(const char* pszFile) {
		FILE* fp = fopen(pszFile, "rb");
		if (fp == NULL) {
			return FALSE;
		}
		SAFE_DELETE(_pbtData);

		fread(&_nWidth, 4, 1, fp);
		fread(&_nHeight, 4, 1, fp);

		_nByteWidth = _nWidth / 8;

		BYTE* pBuf = new BYTE[_nWidth / 8 * _nHeight];
		fread(pBuf, _nWidth / 8, _nHeight, fp);
		fclose(fp);

		_pbtData = new BYTE[_nWidth * _nHeight];

		for (int y = 0; y < _nHeight; y++) {
			for (int x = 0; x < _nWidth; x++) {
				int nLocByte = y * _nWidth + x;

				*(_pbtData + nLocByte) = _IsBlock(x, y, pBuf);
			}
		}

		delete[] pBuf;

		_bValid = TRUE;

		return TRUE;
	}

	BYTE IsBlock(int x, int y) {
		if (!_bValid) return 0;
		return *(_pbtData + y * _nWidth + x);
	}

	BOOL IsValid() {
		return _bValid;
	}

	int getWidth() {
		return _nWidth;
	}

	int getHeight() {
		return _nHeight;
	}

protected:
	BYTE _IsBlock(int x, int y, BYTE* pBuf) {
		int nByteWidth = _nWidth / 8;

		int nBit = 7 - x % 8;
		BYTE data = *(pBuf + nByteWidth * y + x / 8);

		BYTE mask = 1 << nBit;

		if (data & mask) {
			return 1;
		}
		return 0;
	}

	BYTE* _pbtData{nullptr};
	int _nByteWidth{};
	int _nWidth{};
	int _nHeight{};
	BOOL _bValid{FALSE};
};

//---------------------------------------------------------------------------------------------------------------------------


extern BOOL g_bBinaryTable;
#endif
