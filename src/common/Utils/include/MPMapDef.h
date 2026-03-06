#pragma once


// 
//  map 780625 -> 780626
//                obj 600    -> 700
// mapobj
// short, byte
// , 1/3


#define MP_MAP_FLAG		 780624   // Map
#define LAST_VERSION_NO  780626	  // 	
#define CUR_VERSION_NO   780627	  // 

// 
struct MPMapFileHeader
{
	int		nMapFlag;
	int		nWidth;
	int     nHeight;
	int		nSectionWidth;
	int		nSectionHeight;
};


#pragma pack(push)
#pragma pack(1)

struct SNewFileTile // 15
{
	DWORD	dwTileInfo;	// 3layertex noalpha no
	BYTE	btTileInfo; // tex no
	short	sColor;		// 32bit565
	char	cHeight;	// 10cm
	short	sRegion;	
	BYTE	btIsland;
	BYTE	btBlock[4];
	
	SNewFileTile(): sRegion(0), btIsland(0)
    {
        btBlock[0] = 0;
        btBlock[1] = 0;
        btBlock[2] = 0;
        btBlock[3] = 0;
    }
};

struct SFileTile
{
    BYTE  t[8];
    short sHeight;    // 
    DWORD dwColor;
    short sRegion;
    BYTE  btIsland;
    BYTE  btBlock[4]; // 4 x 1

    SFileTile(): sRegion(0), btIsland(0)
    {
        btBlock[0] = 0;
        btBlock[1] = 0;
        btBlock[2] = 0;
        btBlock[3] = 0;
    }
};

#pragma pack(pop)



// 8Tile, 5
// 6,  1 ~ 63, 63, 0
// Alpha4, 0 ~ 15, 16
inline void TileInfo_8To5(BYTE *pbtTile, DWORD &dwNew, BYTE &btNew)
{
	DWORD dwTex1   = *(pbtTile + 2);
	DWORD dwTex2   = *(pbtTile + 4);
	DWORD dwTex3   = *(pbtTile + 6);
	DWORD dwAlpha1 = *(pbtTile + 3);
	DWORD dwAlpha2 = *(pbtTile + 5);
	DWORD dwAlpha3 = *(pbtTile + 7);

	dwNew = dwTex1<<26 | dwAlpha1<<22 | dwTex2<<16 | dwAlpha2<<12 | dwTex3<<6 | dwAlpha3<<2;
	btNew = *(pbtTile + 0);
}

// 58
inline void TileInfo_5To8(DWORD dwNew, BYTE btNew, BYTE *pbtTile)
{
	*(pbtTile + 0) = btNew;
	*(pbtTile + 1) = 15;
	*(pbtTile + 2) = (BYTE)(dwNew>>26);
	*(pbtTile + 3) = (BYTE)(dwNew>>22) & 0x000f;
	*(pbtTile + 4) = (BYTE)(dwNew>>16) & 63;
	*(pbtTile + 5) = (BYTE)(dwNew>>12) & 0x000f;
	*(pbtTile + 6) = (BYTE)(dwNew>>6)  & 63;
	*(pbtTile + 7) = (BYTE)(dwNew>>2)  & 0x000f;
}

#define NEW_VERSION  // MindPower

