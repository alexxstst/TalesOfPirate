#include "util2.h"

#include <format>
#include <time.h>
#include <list>
#include "Iphlpapi.h"

HANDLE __hStdOut = nullptr;
BOOL g_bBinaryTable = FALSE;

BOOL g_bFirstLog2 = TRUE;

DWORD g_dwStartTime[32], g_dwEndTime[32], g_dwCurRecord = 0;

void StartTimeRecord() {
	if (g_dwCurRecord < 32) {
		g_dwStartTime[g_dwCurRecord] = timeGetTime();
		g_dwCurRecord++;
	}
}

DWORD EndTimeRecord() {
	if (g_dwCurRecord > 0) {
		g_dwCurRecord--;
		g_dwEndTime[g_dwCurRecord] = timeGetTime();
		DWORD dwElapsedTime = g_dwEndTime[g_dwCurRecord] - g_dwStartTime[g_dwCurRecord];

		return dwElapsedTime;
	}

	return 0;
}

void CreateConsole() {
	AllocConsole();
	SetConsoleTitle("Debug Window");
	__hStdOut = GetStdHandle(STD_OUTPUT_HANDLE); // ->standard output HANDLE
}


void Util_TrimString(std::string& str) {
	char* psz = (char*)(str.c_str());
	size_t nSize = str.size();
	char* pNew = new char[nSize + 1];
	int n = 0;
	for (int i = 0; i < (int)nSize; i++) {
		char c = psz[i];
		if (c != ' ' && c != '\t') {
			pNew[n] = c;
			n++;
		}
	}
	pNew[n] = '\0';
	str = pNew;
	delete[] pNew;
}


//  MAKEBIN   modify by Philip.Wu  2006-07-31
void Util_TrimTabString(std::string& str) {
	char* psz = (char*)(str.c_str());
	size_t nSize = str.size();
	char* pNew = new char[nSize + 1];
	int n = 0;
	for (int i = 0; i < (int)nSize; i++) {
		char c = psz[i];
		//if(c!=' ' && c!= '\t')
		if (c != '\t') {
			pNew[n] = c;
			n++;
		}
	}
	pNew[n] = '\0';
	str = pNew;
	delete[] pNew;
}


long Util_GetFileSize(FILE* fp) {
	if (fp == nullptr) return -1;
	auto dwOldLoc = ftell(fp);
	fseek(fp, 0, SEEK_END);
	auto dwSize = ftell(fp);
	fseek(fp, dwOldLoc, SEEK_SET);
	return dwSize;
}

long Util_GetFileSizeByName(char* pszFileName) {
	FILE* fp = fopen(pszFileName, "rb");
	long lSize = Util_GetFileSize(fp);
	fclose(fp);
	return lSize;
}

unsigned long Util_GetSysTick() {
#ifdef WIN32
	return GetTickCount();
#endif

#ifdef LINUX
	timeval tv;
	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
#endif
}


void Util_Sleep(int ms) {
#ifdef WIN32
	Sleep(ms);
#endif

#ifdef LINUX
	usleep(ms * 1000);
#endif
}

bool Util_IsFileExist(char* pszName) {
	if (_access(pszName, 0) == -1) return false;
	return true;
}

bool Util_MakeDir(const char* lpPath) {
	std::string pathname = lpPath;
	if (lpPath[pathname.size() - 1] != '/') {
		pathname += "/";
	}

	auto end = pathname.rfind('/');
	auto pt = pathname.find('/');

	if (pathname[pt - 1] == ':') pt = pathname.find('/', pt + 1);

	std::string path{};

	while (pt != -1 && pt <= end) {
		path = pathname.substr(0, pt + 1);
		if (_access(path.c_str(), 0) == -1) {
			_mkdir(path.c_str());
		}
		pt = pathname.find('/', pt + 1);
	}
	return true;
}

int Util_ResolveTextLine1(const char* pszText, std::string strList[],
						  int nMax, int nSep) {
	if (pszText == nullptr) {
		return 0;
	}
	int nResult = 0;
	char* pszDest = (char*)pszText;
	char* pszFound = nullptr;
	do {
		pszFound = strchr(pszDest, nSep);
		if (pszFound != nullptr) {
			int p = pszFound - pszDest;
			pszDest[p] = '\0'; // trick , we need save result string
			strList[nResult] = pszDest;
			pszDest[p] = nSep; // we should not change source string
			pszDest = pszFound + 1;
			int nSkip = Util_StringSkip(&pszDest, nSep);
			if (pszDest[0] == '\0') {
				nResult++;
				break;
			}
		}
		else {
			strList[nResult] = pszDest;
		}
		nResult++;
		if (nResult >= nMax) {
			break;
		}
	}
	while (pszFound);

	return nResult;
}

int Util_ResolveTextLine(const char* pszText, std::string strList[],
						 int nMax, int nSep, int nSep2) {
	if (pszText == nullptr || strlen(pszText) == 0) return 0;

	// 1
	if ((nSep == 0 && nSep2 == 0) || (strlen(pszText) == 1)) {
		strList[0] = pszText;
		return 1;
	}

	int nResult = 0;
	if ((nSep != 0) && (nSep2 != 0) && (nSep != nSep2)) {
		//
		std::string lstrList[1024];
		int n1 = 0;
		int n2 = 0;

		//
		char* pszFound1 = nullptr;
		char* pszFound2 = nullptr;
		pszFound1 = (char*)strchr(pszText, nSep);
		pszFound2 = (char*)strchr(pszText, nSep2);

		if ((pszFound1 == nullptr) || (pszFound2 == nullptr)) {
			//
			if (pszFound1 != nullptr)
				nResult = Util_ResolveTextLine1(pszText, strList, nMax, nSep);
			else if (pszFound2 != nullptr)
				nResult = Util_ResolveTextLine1(pszText, strList, nMax, nSep2);
			else {
				//
				strList[0] = pszText;
				return 1;
			}
		}
		else {
			//
			std::string newString;

			//
			n1 = Util_ResolveTextLine1(pszText, lstrList, nMax, nSep);

			//
			for (int i = 0; i < n1; ++i) {
				newString += lstrList[i] + char(nSep2);
			}

			//
			char const* p = newString.c_str();
			n2 = Util_ResolveTextLine1(newString.c_str(), strList, nMax, nSep2);
			nResult = n2;
		}
	}
	else {
		// nSepnSep2
		int sep = 0;
		if (nSep != 0) sep = nSep;
		else sep = nSep2;

		nResult = Util_ResolveTextLine1(pszText, strList, nMax, sep);
	}

	return nResult;
}

const struct tm* Util_GetCurTime() {
	static tm* g_tm;
	time_t l;
	time(&l);
	g_tm = localtime(&l);
	return g_tm;
}


const char* Util_CurTime2String(int nFlag) {
	static char g_szTime[96];
	time_t l;
	time(&l);
	struct tm* t = localtime(&l);
	switch (nFlag) {
	case 0: {
		strcpy(g_szTime, asctime(t));
		break;
	}
	case 1: {
		*std::format_to(g_szTime, "{:04}-{:02}-{:02}_{:02}-{:02}-{:02}", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec) = '\0';
		break;
	}
	case 2: {
		*std::format_to(g_szTime, "{:02}-{:02}-{:02}", t->tm_hour, t->tm_min, t->tm_sec) = '\0';
		break;
	}
	case 3: {
		*std::format_to(g_szTime, "{}", t->tm_wday) = '\0';
		break;
	}
	default: {
		strcpy(g_szTime, "");
		break;
	}
	}
	return g_szTime;
}

std::string GetMacString() {
	std::string strRet = "";
	IP_ADAPTER_INFO CheckBuf;
	ULONG outLen = 0;
	if (GetAdaptersInfo(&CheckBuf, &outLen) != ERROR_SUCCESS) {
		PIP_ADAPTER_INFO pAdpterInfo = (IP_ADAPTER_INFO*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, outLen);
		if (GetAdaptersInfo(pAdpterInfo, &outLen) == ERROR_SUCCESS) {
			for (int i = 0; i < MAX_ADAPTER_ADDRESS_LENGTH; i++) {
				strRet += std::format("{:02X}", pAdpterInfo->Address[i]);
				if (i + 1 < MAX_ADAPTER_ADDRESS_LENGTH) {
					strRet += "-";
				}
			}
		}
		HeapFree(GetProcessHeap(), 0, pAdpterInfo);
	}
	return strRet;
}

void ProcessDirectory(const char* pszDir, std::list<std::string>* pFileList, int nOperateFlag) {
#ifdef WIN32
	_finddata_t filestruct;
	int p = 0;
	int fn = 0;
	char szSearch[255];
	if (strlen(pszDir) == 0) {
		strcpy(szSearch, "*.*");
	}
	else {
		strcpy(szSearch, pszDir);
		strcat(szSearch, "/*.*");
	}

	int hnd = _findfirst(szSearch, &filestruct);
	if (hnd == -1) {
		return;
	}
	do {
		std::string szFullName;
		if (strlen(pszDir) > 0) {
			szFullName = std::format("{}/{}", pszDir, filestruct.name);
		}
		else {
			szFullName = filestruct.name;
		}

		if (!(filestruct.attrib & _A_SUBDIR)) {
			switch (nOperateFlag) {
			case DIRECTORY_OP_QUERY: {
				if (pFileList) pFileList->push_back(szFullName);
				break;
			}
			case DIRECTORY_OP_DELETE: {
				remove(szFullName.c_str());
				break;
			}
			}
		}
		else // Directory
		{
			if (strcmp(filestruct.name, "..") != 0 && strcmp(filestruct.name, ".") != 0) {
				ProcessDirectory(szFullName.c_str(), pFileList, nOperateFlag);
			}
		}
	}
	while (!_findnext(hnd, &filestruct));
#endif

#ifdef LINUX
	int nLen = strlen(pszDir);
	DIR* dir;
	if (nLen == 0) {
		dir = opendir(".");
	}
	else {
		dir = opendir(pszDir);
	}
	if (dir != NULL) {
		int n;
		direct* dd;
		while (dd = readdir(dir)) {
			if (strcmp(dd->d_name, ".") != 0 && strcmp(dd->d_name, "..") != 0) {
				char szFullName[255];
				strcpy(szFullName, pszDir);
				if (pszDir[nLen - 1] != '/' && (nLen > 0)) {
					strcat(szFullName, "/");
				}
				strcat(szFullName, dd->d_name);
				struct stat stat_p;
				stat(szFullName, &stat_p);
				if (S_ISDIR(stat_p.st_mode)) // is directory
				{
					ProcessDirectory(szFullName.c_str(), pFileList, nOperateFlag);
				}
				else if (S_ISREG(stat_p.st_mode)) // is file
				{
					switch (nOperateFlag) {
					case DIRECTORY_OP_QUERY: {
						if (pFileList) pFileList->push_back(szFullName);
						break;
					}
					case DIRECTORY_OP_DELETE: {
						remove(szFullName.c_str());
						break;
					}
					}
				}
			} // end !("."|"..")
		} // end white
	} // dir is exist
	else {
		// Log("ERR : Directory [%s] is not exist\n" ,  pszDir);
	}
#endif
}


//---------------------------------------------------------------
// <example>
//
// string str; Util_RelaceString("abcdefg%naf%", "%", "\%", str);
//
//---------------------------------------------------------------
size_t Util_RelaceString(const char* pszSrc, const char* s1, const char* s2, std::string& str) {
	std::string::size_type nReplace = 0;
	std::string s(pszSrc);
	std::string::size_type nLen1 = strlen(s1);
	std::string::size_type nLen2 = strlen(s2);
	std::string::size_type nBegin = 0;
	while (true) {
		std::string::size_type p = s.find(s1, nBegin);
		if (p == std::string::npos)
			break;
		s.replace(p, nLen1, s2, nLen2);
		nReplace++;
		nBegin = p + nLen2;
	}
	str = s;
	return nReplace;
}

DWORD MPTimer::dwFrequency = 1;
