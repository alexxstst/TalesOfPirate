#include "SectionData.h"
#include <sys/types.h>
#include <sys/stat.h>

CSectionDataMgr::CSectionDataMgr()
	: _bEdit(FALSE),
	  _fp(nullptr),
	  _pfnProc(nullptr),
	  _nLoadCenterX(0),
	  _nLoadCenterY(0),
	  _SectionArray(nullptr),
	  _nLoadWidth(0),
	  _nLoadHeight(0),
	  _bDebug(FALSE) {
	_szDataName = "noname";
}

CSectionDataMgr::~CSectionDataMgr() {
	if (_fp) fclose(_fp);
}

BOOL CSectionDataMgr::Create(int nSectionCntX, int nSectionCntY, const char* pszMapName) {
	_szFileName = pszMapName;

	if (_bDebug) {
		ToLog("{}: begin create new data file[{}], Size = ({} {})", GetDataName(), pszMapName, nSectionCntX, nSectionCntY);
	}

	_nSectionCntX = nSectionCntX;
	_nSectionCntY = nSectionCntY;


	_fp = fopen(pszMapName, "wb");
	if (_fp == nullptr) {
		ToLog("{}: msg file {} create failed!", GetDataName(), pszMapName);
		return FALSE;
	}

	_bEdit = TRUE;

	_WriteFileHeader();

	int nTotal = GetSectionCnt();

	// 0
	_SectionArray = new SDataSection*[nTotal];

	memset(_SectionArray, 0, nTotal * sizeof(SDataSection*));

	for (int i = 0; i < nTotal; i++) {
		_WriteSectionIdx(i, 0);
	}

	if (_bDebug) {
		ToLog("{}: create ok, TotalSectionCnt = {}!", GetDataName(), nTotal);
	}
	return TRUE;
}

// ,
BOOL CSectionDataMgr::CreateFromFile(const char* pszMapName, BOOL bEdit) {
	_szFileName = pszMapName;

	if (_bDebug) {
		ToLog("{}: begin read data file [{}]", GetDataName(), pszMapName);
	}

	if (_fp != nullptr) {
		fclose(_fp);
		_fp = nullptr;
	}

	FILE* fp = nullptr;
	if (bEdit) {
		_chmod(pszMapName, _S_IWRITE);
		fp = fopen(pszMapName, "r+b");
	}
	else {
		fp = fopen(pszMapName, "rb");
	}
	if (fp == nullptr) {
		ToLog("map: Load Map File[{}] Error!", pszMapName);
		return FALSE;
	}

	_fp = fp;

	if (!_ReadFileHeader()) {
		ToLog("map: msg read file failed, [{}] invalid file!", pszMapName);
		fclose(fp);
		return FALSE;
	}

	_bEdit = bEdit;

	_SectionArray = new SDataSection*[GetSectionCnt()];

	memset(_SectionArray, 0, GetSectionCnt() * sizeof(SDataSection*));
	return TRUE;
}

SDataSection* CSectionDataMgr::LoadSectionData(int nSectionX, int nSectionY) {
	// Section
	std::uint32_t dwLoc = (nSectionY * _nSectionCntX + nSectionX);

	SDataSection* pSection = new SDataSection;
	*(_SectionArray + dwLoc) = pSection;
	// _SectionList.push_back(pSection);

	pSection->dwDataOffset = _ReadSectionIdx(dwLoc);

	if (pSection->dwDataOffset != 0) {
		pSection->pData = new BYTE[_GetSectionDataSize()];

		// Section
		fseek(_fp, pSection->dwDataOffset, SEEK_SET);
		if (_bDebug) {
			ToLog("{}: Seek Offset = {}", GetDataName(), pSection->dwDataOffset);
		}
		fread(pSection->pData, _GetSectionDataSize(), 1, _fp);

		_AfterReadSectionData(pSection, nSectionX, nSectionY);
	}

	return pSection;
}

void CSectionDataMgr::SaveSectionData(int nSectionX, int nSectionY) {
	if (!_bEdit || _fp == nullptr) return;

	SDataSection* pSection = GetSectionData(nSectionX, nSectionY);
	if (!pSection) return;

	if (pSection->dwDataOffset) {
		if (_bDebug) {
			ToLog("{}: [{} {}] exsit, save directly!", GetDataName(), nSectionX, nSectionY);
		}

		fseek(_fp, pSection->dwDataOffset, SEEK_SET);
	}
	else {
		if (_bDebug) {
			ToLog("{}: [{} {}] alloc new area at file tail", GetDataName(), nSectionX, nSectionY);
		}
		// Data
		// ,
		//
		// Trim,

		//
		fseek(_fp, 0, SEEK_END);
		pSection->dwDataOffset = ftell(_fp);
	}
	fwrite(pSection->pData, _GetSectionDataSize(), 1, _fp);

	_WriteSectionIdx(nSectionY * _nSectionCntX + nSectionX, pSection->dwDataOffset);
}

void CSectionDataMgr::ClearSectionData(int nSectionX, int nSectionY) {
	//
	std::uint32_t dwLoc = (nSectionY * _nSectionCntX + nSectionX);
	_WriteSectionIdx(dwLoc, 0);

	//
	SDataSection* pSection = GetSectionData(nSectionX, nSectionY);
	if (pSection) // Section
	{
		// _SectionList.remove(pSection);
		*(_SectionArray + dwLoc) = nullptr;
		SAFE_DELETE(pSection);
	}
}

void CSectionDataMgr::CopySectionData(SDataSection* pDest, SDataSection* pSrc) {
	std::uint32_t dwLastOffset = pDest->dwDataOffset;
	memcpy(pDest, pSrc, sizeof(SDataSection));

	pDest->dwDataOffset = dwLastOffset; //

	if (pSrc->pData == nullptr) {
		if (pDest->pData != nullptr) SAFE_DELETE(pDest->pData);
	}
	else {
		if (pDest->pData == nullptr) pDest->pData = new BYTE[_GetSectionDataSize()];
		memcpy(pDest->pData, pSrc->pData, _GetSectionDataSize());
	}
}


void CSectionDataMgr::Load(BOOL bFull, std::uint32_t dwTimeParam) {
	if (_fp == nullptr) return;

	if (bFull) {
		int nTotal = _nSectionCntX * _nSectionCntY;
		// Section
		for (int i = 0; i < nTotal; i++) {
			int nSectionX = i % _nSectionCntX;
			int nSectionY = i / _nSectionCntX;
			std::uint32_t dwOffset = _ReadSectionIdx(i);
			if (dwOffset != 0) {
				SDataSection* pSection = LoadSectionData(nSectionX, nSectionY);
			}
			if (i % 50 == 0) {
				if (_pfnProc) _pfnProc(0, nSectionX, nSectionY, 0, this);
			}
		}
		return;
	}
}


/*
LPBYTE Util_LoadFile(FILE *fp)
{
    DWORD dwBufSize = 1024 * 1024 * 4;
    
    fseek(fp, 0, SEEK_END);
    DWORD dwFileSize = ftell(fp);
    DWORD dwLeft = dwFileSize;

    LPBYTE pbtFileBuf = new BYTE[dwFileSize]; 
    
    fseek(fp, 0, SEEK_SET);
    LPBYTE pbtBuf = pbtFileBuf;
    while(!feof(fp))
    {
        DWORD dwRead = dwBufSize;
        if(dwLeft - dwBufSize < 0) dwRead = dwLeft;
        fread(pbtBuf, dwRead, 1, fp);
        dwLeft-=dwRead;
        pbtBuf+=dwRead;
        if(dwLeft<=0) break;
    }
    return pbtFileBuf;    
}*/
