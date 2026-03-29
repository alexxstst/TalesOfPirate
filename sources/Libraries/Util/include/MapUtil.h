#pragma once

struct SMapPatchHeader
{
	DWORD	dwFlag;
	char	szMapName[16];
	DWORD	dwUpdateCnt;	
	short	sSectionCntX;
	short	sSectionCntY;

	SMapPatchHeader()
		:dwFlag(20040228), dwUpdateCnt(0)
	{
			
	}
};

BOOL MU_CreateMapPatch(const char *pszOld, const char *pszNew);
BOOL MU_PatchMapFile(const char *pszMap, const char *pszPatch);
