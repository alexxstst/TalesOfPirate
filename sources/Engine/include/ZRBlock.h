#ifndef _ZRBLOCK_H_
#define _ZRBLOCK_H_

#include "MPMap.h"
#include "assert.h" 
#include <sys/types.h>
#include <sys/stat.h>
#include "MPMapDef.h"
#include <array>

#define MAX_BLOCK_SECTION 512
#define MAX_BLOCK_RANGE 1024

//add by jze 2008.7.16
class ZRBlockData
{
public:
	short           sRegion{};     // 
	BYTE btBlock[4] = {}; // 4
public:
	ZRBlockData() {}
	~ZRBlockData(){}

    BYTE	IsBlock(BYTE no) const 
    {
        if(btBlock[no] & 128) return 1;
        return 0;
    }

    void	setBlock(BYTE no, BOOL bSet)
    {
        if(bSet)
        {
            btBlock[no]|=128;
        }
        else
        {
            btBlock[no]&=127;
        }
    }

    BOOL    IsRegion(int nRegionNo) const 
    {
        short s = 1;
        s<<=(nRegionNo - 1);
        return sRegion & s; 
    }

    short	GetRegionValue() const { return sRegion; }
};

//add by jze 2008.7.16
class ZRBlockSection
{
public:
	std::unique_ptr<ZRBlockData[]> blockData{};
	int			 nX{};									// MapSection
	int			 nY{};
	DWORD		 dwDataOffset{};						//  = 0, 
public:
	ZRBlockSection() = default;
	~ZRBlockSection() = default;
};

class ZRBlock
{
public:
	ZRBlock() = default;
	~ZRBlock() = default;
	BOOL                Load(const char *pszMapName, BOOL bEdit);
	void				GetBlockByRange(int CenterX, int CenterY, int range); //Block
	ZRBlockData*		GetBlock(int nX, int nY); //Block
    BYTE				IsGridBlock(int x, int y) const;      // x,y
	short				GetTileRegionAttr(int x, int y) const;// x,y
	void                SetGrid(int GridX, int GridY);
private:
	std::unique_ptr<ZRBlockSection>& GetBlockSection(int nSectionX, int nSectionY); //block
	std::unique_ptr<ZRBlockSection>& LoadBlockData(int nSectionX, int nSectionY); //block
	void				ClearSectionArray();

	void				_LoadBlockData(ZRBlockSection& pSection); //block
	DWORD				_ReadSectionDataOffset(int nSectionX, int nSectionY); //

public:
	BYTE                m_btBlockBuffer[MAX_BLOCK_RANGE][MAX_BLOCK_RANGE] = {};
	short				m_sTileRegionAttr[MAX_BLOCK_SECTION][MAX_BLOCK_SECTION] = {};
private:
	std::array<std::array<std::unique_ptr<ZRBlockSection>, MAX_BLOCK_SECTION>, MAX_BLOCK_SECTION> m_BlockSectionArray{};

	std::unique_ptr<ZRBlockData>        m_pDefaultBlock = std::make_unique<ZRBlockData>();     //block
	std::fstream fs;

	int					m_fShowCenterX{}; // 
	int					m_fShowCenterY{};
	int					m_nSectionWidth{};// Section
	int					m_nSectionHeight{};
	int					m_nSectionCntX{};// Section
	int					m_nSectionCntY{};
	int                 m_nSectionCnt{};
	int                 m_nLastGridStartX{};
	int                 m_nLastGridStartY{};
	int					m_nGridShowWidth{};
	int					m_nGridShowHeight{};
	BOOL				m_bEdit{ true };
	DWORD				m_dwMapPos{};
	DWORD				m_dwMapDataSize{};
	std::unique_ptr<BYTE[]> m_pMapData{};
	int					m_nWidth{};
	int					m_nHeight{};
	std::unique_ptr<DWORD[]> m_pOffsetIdx{};
};

inline BYTE ZRBlock::IsGridBlock(int x, int y) const // 
{
    int offx = x - m_nLastGridStartX;
    int offy = y - m_nLastGridStartY;
	
	if(offx < 0 || offx >= m_nGridShowWidth)  return 1;
    if(offy < 0 || offy >= m_nGridShowHeight) return 1;

    return m_btBlockBuffer[offy][offx];
}

inline short ZRBlock::GetTileRegionAttr(int x, int y) const // 
{
    int offx = x - m_nLastGridStartX/2;
    int offy = y - m_nLastGridStartY/2;

    if(offx < 0 || offx >= m_nGridShowWidth) return 0;
    if(offy < 0 || offy >= m_nGridShowHeight) return 0;

    return m_sTileRegionAttr[offy][offx];
}
#endif
