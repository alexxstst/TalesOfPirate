// #include "Stdafx.h"
#include "TerrainData.h"
#include "ObjectData.h"
#include "AttribData.h"
#include "MPMapDef.h"
using namespace std;

//
#define LW_RGB555_R(rgb) (BYTE)( ( rgb & 0x7c00) >> 7 )
#define LW_RGB555_G(rgb) (BYTE)( ( rgb & 0x3e0) >> 2 )
#define LW_RGB555_B(rgb) (BYTE)( ( rgb & 0x1f) << 3 )

#define LW_RGB565_R(rgb) (BYTE)( ( rgb & 0xf800) >> 8 )
#define LW_RGB565_G(rgb) (BYTE)( ( rgb & 0x7e0) >> 3 )
#define LW_RGB565_B(rgb) (BYTE)( ( rgb & 0x1f) << 3 )

#define LW_RGB555TODWORD(rgb) (DWORD)( LW_RGB555_R(rgb) | (LW_RGB555_G(rgb)<<8) | (LW_RGB555_B(rgb)<<16) )
#define LW_RGB565TODWORD(rgb) (DWORD)( LW_RGB565_R(rgb) | (LW_RGB565_G(rgb)<<8) | (LW_RGB565_B(rgb)<<16) )

#define LW_RGBDWORDTO555(color) (WORD)( ((color&0xf8)<<7) | ((color & 0xf800)>>6) | ((color & 0xf80000)>>19) )
#define LW_RGBDWORDTO565(color) (WORD)( ((color&0xf8)<<8) | ((color & 0xfC00)>>5) | ((color & 0xf80000)>>19) )

//     
void CheckMapFileVersion(const char *pszMapName)
{
    FILE *fp = fopen(pszMapName, "rb");
    if(fp==NULL)
    {
        //LG("error", "[%s]\n", pszMapName);
		LG("error", "can't open map [%s]\n", pszMapName);
        return;
    }

    MPMapFileHeader header;

    fread(&header, sizeof(header),1, fp);

    fclose(fp);
    if(header.nMapFlag==780624)
    {
        //LG("map_version", "msg!");
		LG("map_version", "msg this version file is deprecated!");
        return;
    }
    else if(header.nMapFlag==LAST_VERSION_NO)
    {
        //LG("map_version", "msg, !");
		LG("map_version", "msg old version file, now update the version");
    }
    else
    {
        return;
    }

    MPTerrainData map;
    map.CreateFromFile(pszMapName, TRUE);
    map.Load(TRUE);
    
    fp = fopen("map/temp.map", "wb");
    header.nMapFlag = CUR_VERSION_NO;

    fwrite(&header, sizeof(header), 1, fp);
    
    int nTotal = map.GetSectionCntY() * map.GetSectionCntX();
    DWORD *dwOffsetArray = new DWORD[nTotal];
    memset(dwOffsetArray, 0, sizeof(DWORD) * nTotal);

    fseek(fp, sizeof(header) + sizeof(DWORD) * nTotal, SEEK_SET);

    for(int y = 0; y < map.GetSectionCntY(); y++)
    {
        for(int x = 0; x < map.GetSectionCntX(); x++)
        {
            SDataSection *pSection = map.GetSectionData(x, y);
            if(pSection && pSection->dwDataOffset)
            {
                // LG("map", "Write Section [%d %d]\n", x, y);
                DWORD dwOffset = ftell(fp);
                *(dwOffsetArray + y * map.GetSectionCntX() + x) = dwOffset;
                SFileTile *pTileData = (SFileTile*)pSection->pData;
                SNewFileTile tile;
                for(int i = 0; i < 64; i++)
                {
                    SFileTile *pOld = pTileData + i;
                    
					TileInfo_8To5(pOld->t, tile.dwTileInfo, tile.btTileInfo);
					
					tile.cHeight  = (char)(pOld->sHeight / 10); // 10cm
                    tile.sColor   = LW_RGBDWORDTO565(pOld->dwColor);
                    tile.sRegion  = pOld->sRegion;
                    tile.btIsland = pOld->btIsland;
					memcpy(tile.btBlock, pOld->btBlock, 4);
                    fwrite(&tile, sizeof(tile), 1, fp);
                }
            }
        }
    }

    fseek(fp, sizeof(header), SEEK_SET);
    fwrite(dwOffsetArray, sizeof(DWORD), nTotal, fp);
    fclose(fp);

    string strOld = pszMapName;
    strOld+=".old";
    CopyFile(pszMapName, strOld.c_str(), FALSE);
    //MessageBox(NULL, ", .old!", strOld.c_str(), 0);
	MessageBox(NULL, "Map format convert ok, ole file backed OK(.old)!", strOld.c_str(), 0);
    CopyFile("map/temp.map", pszMapName, FALSE);
}

