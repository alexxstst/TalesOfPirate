#include "util.h"
#include "MapUtil.h"
#include "TerrainData.h"
#include "ObjectData.h"
using namespace std;

// 地图工具函数, 根据文件名, 读入地图数据
BOOL MU_LoadMapData(CSectionDataMgr *pData, const char *pszDataName)
{
	if(!pData->CreateFromFile(pszDataName, TRUE)) 
    {
        ToLogService("common", LogLevel::Error, "读取数据文件{}失败!", pszDataName);
        return FALSE;
    }
    
    pData->Load(TRUE);
	return TRUE;
}



// 地图补丁工具
// 主要工作原理:
// 1 .遍历两张地图, 找出不同的section, 把这些section数据生成地图补丁文件
// 2. 游戏客户端启动时, 发现存在地图补丁文件, 则读入并吸收进地图文件, 然后删除地图补丁文件
BOOL MU_CreateMapPatch(const char *pszOld, const char *pszNew)
{
	// 打开老的地图文件
	MPTerrainData oldmap, newmap;
 
	if(!MU_LoadMapData(&oldmap, pszOld))
    {
        return FALSE;
    }

	// 打开新的地图文件
	if(!MU_LoadMapData(&newmap, pszNew))
    {
        return FALSE;
    }

	if(oldmap.GetSectionCntX()!=newmap.GetSectionCntX() || oldmap.GetSectionCntY()!=newmap.GetSectionCntY())
	{
		//LG("mappatch", "msg地图尺寸不匹配, 无法继续生成补丁!\n");
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
			
			BYTE btType = 0; // 不更新
			
			if(pNew==NULL && pOld!=NULL)
			{
				btType = 2;  // 清除原地图的上此section
			}
			else
			{
				if(pOld==NULL || memcmp(pOld->pData, pNew->pData, oldmap.GetSectionDataSize())!=0)
				{
					btType = 1; // 需要更新
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
		//LG("mappatch", "msg此两张地图完全相同, 无需生成补丁文件!\n");
		ToLogService("common", LogLevel::Error, " the two file is the same!");
		DeleteFile(szPatchName);
	}
	else
	{
		//LG("mappatch", "msg地图补丁文件创建成功, 总计需要更新%d个区域, 补丁文件为[%s]\n", dwCnt, szPatchName); 
		ToLogService("common", LogLevel::Error, " map patch create ok, totle update {} area, patch file is [{}]", dwCnt, szPatchName); 
	}
	return TRUE;
}

// 给一个地图文件打入补丁
BOOL MU_PatchMapFile(const char *pszMap, const char *pszPatch)
{
	// 打开补丁文件
	FILE *fp = fopen(pszPatch, "rb");
	if(fp==NULL)
	{
		//LG("mappatch", "msg打开地图补丁文件[%s]失败!\n", pszPatch);
		ToLogService("common", LogLevel::Error, " open patch map file [{}] failed!", pszPatch);
		return FALSE;
	}

	SMapPatchHeader header;
	fread(&header,sizeof SMapPatchHeader, 1, fp);

	//LG("mappatch", "msg总计需要更新区域[%d]个\n", header.dwUpdateCnt);
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
		if(btType==1) // 有数据的更新
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
		else if(btType==2) // 清空Section
		{
			oldmap.ClearSectionData(x, y);
		}
		oldmap.SaveSectionData(x, y);
		dwCnt++;
	}
	
	fclose(fp);
	delete[] pBuf;

	//LG("mappatch", "msg地图补丁成功执行, 总计更新区域[%d]个\n", dwCnt);
	ToLogService("common", LogLevel::Error, " map patch create ok, totle update [{}] area", dwCnt);
	return TRUE;
}





