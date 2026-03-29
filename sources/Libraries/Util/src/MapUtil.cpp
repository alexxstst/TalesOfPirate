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
        ToLogService("common", LogLevel::Error, "{}!", pszDataName);
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
		//LG("mappatch", "msg, !\n");
		ToLogService("common", LogLevel::Error, " map size can't match, can't go on!");
		return FALSE;
	}

	string strOldName = pszOld;
	strOldName = strOldName.substr(0, strOldName.size() - 4);
	char szPatchName[255]; sprintf(szPatchName, "%s.pch", strOldName.c_str());
	FILE *fp = fopen(szPatchName, "wb");
	
	SMapPatchHeader header;
	strcpy(header.szMapName, oldmap.GetDataName());
	header.sSectionCntX = oldmap.GetSectionCntX();
	header.sSectionCntY = oldmap.GetSectionCntY();
	
	fwrite(&header, sizeof SMapPatchHeader, 1, fp);
	DWORD dwCnt = 0;
	for(int y = 0; y < oldmap.GetSectionCntY(); y++)
	{
		for(int x = 0; x < newmap.GetSectionCntX(); x++)
		{
			SDataSection *pOld = oldmap.GetSectionData(x, y);
			SDataSection *pNew = newmap.GetSectionData(x, y);
			
			if(pOld==NULL && pNew==NULL) continue;
			
			BYTE btType = 0; // 
			
			if(pNew==NULL && pOld!=NULL)
			{
				btType = 2;  // section
			}
			else
			{
				if(pOld==NULL || memcmp(pOld->pData, pNew->pData, oldmap.GetSectionDataSize())!=0)
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
		//LG("mappatch", "msg, !\n");
		ToLogService("common", LogLevel::Error, " the two file is the same!");
		DeleteFile(szPatchName);
	}
	else
	{
		//LG("mappatch", "msg, %d, [%s]\n", dwCnt, szPatchName); 
		ToLogService("common", LogLevel::Error, " map patch create ok, totle update {} area, patch file is [{}]", dwCnt, szPatchName); 
	}
	return TRUE;
}

// 
BOOL MU_PatchMapFile(const char *pszMap, const char *pszPatch)
{
	// 
	FILE *fp = fopen(pszPatch, "rb");
	if(fp==NULL)
	{
		//LG("mappatch", "msg[%s]!\n", pszPatch);
		ToLogService("common", LogLevel::Error, " open patch map file [{}] failed!", pszPatch);
		return FALSE;
	}

	SMapPatchHeader header;
	fread(&header,sizeof SMapPatchHeader, 1, fp);

	//LG("mappatch", "msg[%d]\n", header.dwUpdateCnt);
	ToLogService("common", LogLevel::Error, " totle update [{}] area", header.dwUpdateCnt);

	MPTerrainData oldmap; MU_LoadMapData(&oldmap, pszMap);

	DWORD dwDataSize = oldmap.GetSectionDataSize();
	BYTE *pBuf = new BYTE[dwDataSize];
	DWORD dwCnt = 0;
	for(DWORD i = 0; i < header.dwUpdateCnt; i++)
	{
		BYTE btType; fread(&btType, 1, 1, fp);
		DWORD x, y;  fread(&x, 4, 1, fp);
		fread(&y, 4, 1, fp);
		
		SDataSection *pSec = oldmap.GetSectionData(x, y);
		if(btType==1) // 
		{
			fread(pBuf, dwDataSize, 1, fp);
			if(pSec==NULL)
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

	//LG("mappatch", "msg, [%d]\n", dwCnt);
	ToLogService("common", LogLevel::Error, " map patch create ok, totle update [{}] area", dwCnt);
	return TRUE;
}





