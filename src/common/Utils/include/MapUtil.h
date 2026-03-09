#pragma once

#include <cstdint>

struct SMapPatchHeader {
	std::uint32_t dwFlag{};
	char szMapName[16]{};
	std::uint32_t dwUpdateCnt{};
	short sSectionCntX{};
	short sSectionCntY{};

	SMapPatchHeader()
		: dwFlag(20040228), dwUpdateCnt(0) {
	}
};

BOOL MU_CreateMapPatch(const char* pszOld, const char* pszNew);
BOOL MU_PatchMapFile(const char* pszMap, const char* pszPatch);
