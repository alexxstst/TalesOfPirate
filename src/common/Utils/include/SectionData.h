#pragma once

#include <cstdint>
#include "Util.h"

struct SDataSection {
	void* pData; //
	std::uint32_t dwDataOffset; //  = 0,
	std::uint32_t dwActiveTime; //
	std::uint32_t dwParam; //

	SDataSection()
		: pData(nullptr), dwDataOffset(0), dwActiveTime(0), dwParam(0) {
	}
};


class CSectionDataMgr;
typedef long (CALLBACK*MAP_LOADING_PROC)(int nFlag, int nSectionX,
										 int nSectionY, unsigned long dwParam, CSectionDataMgr* pMapData);

class CSectionDataMgr {
public:
	virtual ~CSectionDataMgr();

	// ,
	BOOL CreateFromFile(const char* pszMapName, BOOL bEdit = FALSE);

	// ,
	BOOL Create(int nSectionCntX, int nSectionCntY, const char* pszMapName);

	// , ,
	void Load(BOOL bFull, std::uint32_t dwTimeParam = 0);

	// Section
	void SetLoadSectionCallback(MAP_LOADING_PROC pfn) {
		_pfnProc = pfn;
	}

	// Section
	void CopySectionData(SDataSection* pDest, SDataSection* pSrc);

	// Section
	void SaveSectionData(int nSectionX, int nSectionY);

	// Section,
	void ClearSectionData(int nSectionX, int nSectionY);

	// Section,
	SDataSection* GetSectionData(int nSectionX, int nSectionY) const;

	// Section
	SDataSection* LoadSectionData(int nSectionX, int nSectionY);


	// map
	int GetSectionCntX() const {
		return _nSectionCntX;
	}

	int GetSectionCntY() const {
		return _nSectionCntY;
	}

	int GetSectionCnt() const {
		return _nSectionCntX * _nSectionCntY;
	}

	const std::string& GetFileName() {
		return _szFileName;
	}

	const std::string& GetDataName() {
		return _szDataName;
	}

	void SetDataName(const std::string& pszName) {
		_szDataName = pszName;
	}

	//
	// int     CalValidSectionCnt();

	//
	void SetLoadSize(int nWidth, int nHeight) {
		_nLoadWidth = nWidth;
		_nLoadHeight = nHeight;
	}

	int GetLoadWidth() const {
		return _nLoadWidth;
	}

	int GetLoadHeight() const {
		return _nLoadHeight;
	}

	void SetLoadCenter(int nX, int nY) {
		_nLoadCenterX = nX;
		_nLoadCenterY = nY;
	}

	int GetLoadCenterX() const {
		return _nLoadCenterX;
	}

	int GetLoadCenterY() const {
		return _nLoadCenterY;
	}

	BOOL IsValidLocation(int nX, int nY) const;

	int GetSectionDataSize() {
		return _GetSectionDataSize();
	}

protected:
	CSectionDataMgr();

	virtual int _GetSectionDataSize() = 0;
	virtual BOOL _ReadFileHeader() = 0;
	virtual void _WriteFileHeader() = 0;
	virtual std::uint32_t _ReadSectionIdx(std::uint32_t dwSectionNo) = 0;
	virtual void _WriteSectionIdx(std::uint32_t dwSectionNo, std::uint32_t dwDataOffset) = 0;

	virtual void _AfterReadSectionData(SDataSection* pSection, int nSectionX, int nSectionY) {
	}

	SDataSection* _GetDataSection(std::uint32_t dwSectionNo) const {
		return *(_SectionArray + dwSectionNo);
	}

	std::string _szFileName{}; //
	std::string _szDataName{}; //
	int _nSectionCntX; // Section
	int _nSectionCntY; // Section
	int _nLoadCenterX; //
	int _nLoadCenterY;
	int _nLoadWidth; //
	int _nLoadHeight;

	int _nSectionStartX;
	int _nSectionStartY;

	MAP_LOADING_PROC _pfnProc; //

	FILE* _fp; //
	BOOL _bEdit; //
	BOOL _bDebug;

	std::list<SDataSection*> _SectionList;
	SDataSection** _SectionArray;
};

inline SDataSection* CSectionDataMgr::GetSectionData(int nSectionX, int nSectionY) const {
	if (!IsValidLocation(nSectionX, nSectionY)) {
		return nullptr;
	}

	std::uint32_t dwLoc = (nSectionY * _nSectionCntX + nSectionX);
	return *(_SectionArray + dwLoc);
}

inline BOOL CSectionDataMgr::IsValidLocation(int nX, int nY) const {
	if (nX < 0 || nX >= _nSectionCntX || nY < 0 || nY >= _nSectionCntY) {
		return FALSE;
	}
	return TRUE;
}
