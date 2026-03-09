#include "util.h"
#include "MapUtil.h"
#include "TerrainData.h"
#include "ObjectData.h"
using namespace std;

// , , 
BOOL MU_LoadMapData(CSectionDataMgr *pData, const char *pszDataName)
{
	if(!pData->CreateFromFile(pszDataName, TRUE)) 
    {
        ToLog("maputil: msg {}!", pszDataName);
        return FALSE;
    }
    
    pData->Load(TRUE);
	return TRUE;
}



// 
// :
// 1 ., section, section
// 2. , , , 
BOOL MU_CreateMapPatch(const char *pszOld, const char *pszNew)
{
	// 
	MPTerrainData oldmap, newmap;
 
	if(!MU_LoadMapData(&oldmap, pszOld))
    {
        return FALSE;
    }

	// 
	if(!MU_LoadMapData(&newmap, pszNew))
    {
        return FALSE;
    }

	if(oldmap.GetSectionCntX()!=newmap.GetSectionCntX() || oldmap.GetSectionCntY()!=newmap.GetSectionCntY())
	{
		ToLog("mappatch: msg map size can't match, can't go on!");
		return FALSE;
	}

	string strOldName = pszOld;
	strOldName = strOldName.substr(0, strOldName.size() - 4);
	std::string szPatchName = std::format("{}.pch", strOldName);
	FILE *fp = fopen(szPatchName.c_str(), "wb");
	
	SMapPatchHeader header;
	strcpy_s(header.szMapName, oldmap.GetDataName().c_str());
	header.sSectionCntX = oldmap.GetSectionCntX();
	header.sSectionCntY = oldmap.GetSectionCntY();
	
	fwrite(&header, sizeof SMapPatchHeader, 1, fp);
	std::uint32_t dwCnt = 0;
	for(int y = 0; y < oldmap.GetSectionCntY(); y++)
	{
		for(int x = 0; x < newmap.GetSectionCntX(); x++)
		{
			SDataSection *pOld = oldmap.GetSectionData(x, y);
			SDataSection *pNew = newmap.GetSectionData(x, y);
			
			if(pOld== nullptr && pNew== nullptr) continue;
			
			BYTE btType = 0; // 
			
			if(pNew== nullptr && pOld!= nullptr)
			{
				btType = 2;  // section
			}
			else
			{
				if(pOld== nullptr || memcmp(pOld->pData, pNew->pData, oldmap.GetSectionDataSize())!=0)
				{
					btType = 1; // 
				}
			}
			if(btType > 0)
			{
				fwrite(&btType,1, 1, fp);
				fwrite(&x, 4, 1, fp);
				fwrite(&y, 4, 1, fp);
				if(btType==1)
				{
					fwrite(pNew->pData, newmap.GetSectionDataSize(), 1, fp);
				}
				dwCnt++;
			}
		}
		
	}
	header.dwUpdateCnt = dwCnt;
	fseek(fp, SEEK_SET, 0);
	fwrite(&header, sizeof SMapPatchHeader, 1, fp);

	fclose(fp);

	if(dwCnt==0)
	{
		ToLog("mappatch: msg the two file is the same!");
		DeleteFile(szPatchName.c_str());
	}
	else
	{
		ToLog("mappatch: msg map patch create ok, totle update {} area, patch file is [{}]", dwCnt, szPatchName);
	}
	return TRUE;
}

// 
BOOL MU_PatchMapFile(const char *pszMap, const char *pszPatch)
{
	// 
	FILE *fp = fopen(pszPatch, "rb");
	if(fp== nullptr)
	{
		ToLog("mappatch: msg open patch map file [{}] failed!", pszPatch);
		return FALSE;
	}

	SMapPatchHeader header;
	fread(&header,sizeof SMapPatchHeader, 1, fp);

	ToLog("mappatch: msg totle update [{}] area", header.dwUpdateCnt);

	MPTerrainData oldmap; MU_LoadMapData(&oldmap, pszMap);

	std::uint32_t dwDataSize = oldmap.GetSectionDataSize();
	BYTE *pBuf = new BYTE[dwDataSize];
	std::uint32_t dwCnt = 0;
	for(std::uint32_t i = 0; i < header.dwUpdateCnt; i++)
	{
		BYTE btType; fread(&btType, 1, 1, fp);
		std::uint32_t x, y;  fread(&x, 4, 1, fp);
		fread(&y, 4, 1, fp);
		
		SDataSection *pSec = oldmap.GetSectionData(x, y);
		if(btType==1) // 
		{
			fread(pBuf, dwDataSize, 1, fp);
			if(pSec== nullptr)
			{
				pSec = oldmap.LoadSectionData(x, y);
			}
			if(pSec->dwDataOffset==0)
			{
				pSec->pData = new BYTE[dwDataSize];
			}
			memcpy(pSec->pData, pBuf, dwDataSize);
		}
		else if(btType==2) // Section
		{
			oldmap.ClearSectionData(x, y);
		}
		oldmap.SaveSectionData(x, y);
		dwCnt++;
	}
	
	fclose(fp);
	delete[] pBuf;

	ToLog("mappatch: msg map patch create ok, totle update [{}] area", dwCnt);
	return TRUE;
}





