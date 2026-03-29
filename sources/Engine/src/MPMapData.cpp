#include "Stdafx.h"
#include "MPTile.h"
#include "MPMap.h"
#include "lwgraphicsutil.h"
#include "assert.h" 
#include <sys/types.h>
#include <sys/stat.h>
#include "MPMapDef.h"

using namespace std;

#define MAX_DEL_SECTION				200
#define MAX_SECTION_BUFFER          16


BOOL MPMap::Load(const char *pszMapName, BOOL bEdit)
{
	if(_fp!=NULL) 
	{
		fclose(_fp); _fp = NULL;
	}
	
	FILE *fp = NULL;
	if(bEdit)
	{
		_chmod(pszMapName, _S_IWRITE );

        fp = fopen(pszMapName, "r+b");
	}
	else
	{
		fp = fopen(pszMapName, "rb");
	}
	if(fp==NULL) 
	{
		ToLogService("map", LogLevel::Error, "Load Map [{}] Error!", pszMapName);
		return FALSE;
	}
	
	MPMapFileHeader header;

	DWORD dwReadSize = 0;
	
	fread(&header, sizeof(MPMapFileHeader), 1, fp);
	dwReadSize += sizeof(MPMapFileHeader);

    if(header.nMapFlag==MP_MAP_FLAG+1)
    {
	    fclose(fp);
		ToLogService("map", LogLevel::Error, "[{}], MapTool!", pszMapName);
		return FALSE;
	}
    
#ifdef NEW_VERSION
    if(header.nMapFlag!=MP_MAP_FLAG + 3) 
#else
    if(header.nMapFlag!=MP_MAP_FLAG + 2)
#endif
	{
		fclose(fp);
		ToLogService("map", LogLevel::Error, "[{}] MindPower Map File!", pszMapName);
		return FALSE;
	}

	_nWidth			= header.nWidth;
	_nHeight		= header.nHeight;
	_nSectionWidth  = header.nSectionWidth;
	_nSectionHeight = header.nSectionHeight;

	_nSectionCntX   = _nWidth  / _nSectionWidth;
	_nSectionCntY   = _nHeight / _nSectionHeight;
	_nSectionCnt    = _nSectionCntX * _nSectionCntY;

	_bEdit = bEdit;

	_fp = fp;

    // 
    _pOffsetIdx = new DWORD[_nSectionCnt];
    fread(_pOffsetIdx, _nSectionCnt * 4, 1, _fp);
	dwReadSize += _nSectionCnt * 4;

	if( !_bEdit )
	{
		m_dwMapPos = ftell( _fp );

		fseek( _fp, 0, SEEK_END );
		DWORD dwPos = ftell( _fp );

		DWORD dwMapDataSize = dwPos - dwReadSize;

		if( dwMapDataSize > m_dwMapDataSize )
		{
			m_pMapData = std::make_unique<std::byte[]>(dwMapDataSize);
			m_dwMapDataSize = dwMapDataSize;
		}
		fseek( _fp, dwReadSize, SEEK_SET );
		fread(m_pMapData.get(), dwMapDataSize, 1, _fp);
	}
    ClearSectionArray();

	m_pBlock->Load(pszMapName,bEdit);

	return TRUE;
}

int MPMap::GetValidSectionCnt()
{
    int nValidCnt = 0;
    for(int i = 0; i < _nSectionCnt; i++)
    {
        DWORD dwDataOff = *(_pOffsetIdx + i);
        if(dwDataOff) nValidCnt++;
    }
    return nValidCnt;
}

void MPMap::SetSectionTileData(int nX, int nY, BYTE btTexNo)
{
	if(btTexNo==0 || !_bEdit) return;
	
	MPActiveMapSection *pSection = GetActiveSection(nX / _nSectionWidth, nY / _nSectionHeight);
	if(!pSection) return;

	SAFE_DELETE(pSection->pTileData);
	pSection->pTileData = new MPTile[_nSectionWidth * _nSectionHeight];
	for(int y = 0; y < _nSectionHeight; y++)
	{
		for(int x = 0; x < _nSectionWidth; x++)
		{
			MPTile *pTile = pSection->pTileData + y * _nSectionWidth + x;
			pTile->Init();
			pTile->TexLayer[0].btTexNo   = btTexNo;
			pTile->TexLayer[0].btAlphaNo = 15;
 			pTile->fHeight = 0.6f + (float)(rand()%30) / 100.0f;
			pTile->dwColor = 0xffffffff;
            pTile->sRegion = 1; // 
		}
	}
	// pSection->dwDataOffset = 0;

	_SaveSection(pSection);
}


void MPMap::_WriteSectionDataOffset(int nSectionX, int nSectionY, DWORD dwDataOffset)
{
	DWORD dwLoc = (nSectionY * _nSectionCntX + nSectionX);
    *(_pOffsetIdx + dwLoc) = dwDataOffset;
    fseek(_fp, sizeof(MPMapFileHeader) + 4 * dwLoc, SEEK_SET);
	fwrite(&dwDataOffset, 4, 1, _fp);
}

DWORD MPMap::_ReadSectionDataOffset(int nSectionX, int nSectionY)
{
	DWORD dwLoc = (nSectionY * _nSectionCntX + nSectionX);
    return *(_pOffsetIdx + dwLoc);
    
    fseek(_fp, sizeof(MPMapFileHeader) + 4 * dwLoc, SEEK_SET);
	DWORD dwDataOffset; fread(&dwDataOffset, 4, 1, _fp);
	return dwDataOffset;
}

//-----------------
// 
//-----------------
void MPMap::_SaveSection(MPActiveMapSection *pSection)
{
	if(!_bEdit || _fp==NULL) return;
	
	// TileData
	if(pSection->dwDataOffset)
	{
		fseek(_fp, pSection->dwDataOffset, SEEK_SET);
	}
	else
	{
		fseek(_fp, 0, SEEK_END);
		pSection->dwDataOffset = ftell(_fp);
	}
#ifdef NEW_VERSION
	SNewFileTile tile;
#else
	SFileTile tile;
#endif
	for(int y = 0; y < _nSectionHeight; y++)
	{
		for(int x = 0; x < _nSectionWidth; x++)
		{
			MPTile *pTile = pSection->pTileData + _nSectionWidth * y + x;
#ifdef NEW_VERSION
			TileInfo_8To5((BYTE*)(&pTile->TexLayer[0]), tile.dwTileInfo, tile.btTileInfo);
			tile.cHeight = (char)(pTile->fHeight * 100 / 10);
			tile.sColor  = LW_RGBDWORDTO565(pTile->dwColor);
#else
            memcpy(&tile.t[0], &pTile->TexLayer[0], 8);
            tile.sHeight  = (short)(pTile->fHeight * 100);
            tile.dwColor  = pTile->dwColor;
#endif
            tile.sRegion  = pTile->sRegion;
            tile.btIsland = pTile->btIsland;
            memcpy(&(tile.btBlock[0]), &(pTile->btBlock[0]), 4);
            fwrite(&tile, sizeof(tile), 1, _fp);
		}
	}
	_WriteSectionDataOffset(pSection->nX, pSection->nY, pSection->dwDataOffset);
}

//-----------------
// 
//-----------------
void MPMap::_LoadSectionData(MPActiveMapSection *pSection)
{
	int nSectionX = pSection->nX;
	int nSectionY = pSection->nY;

	pSection->dwDataOffset = _ReadSectionDataOffset(nSectionX, nSectionY);
    
    if(pSection->dwDataOffset==0) return;

	DWORD dwPos = 0;
	if( _bEdit )
	{
		fseek(_fp, pSection->dwDataOffset, SEEK_SET);
	}
	else
	{
		dwPos = pSection->dwDataOffset - m_dwMapPos;
	}
	
	pSection->pTileData = new MPTile[_nSectionWidth * _nSectionHeight];

#ifdef NEW_VERSION
	SNewFileTile tile;
#else
	SFileTile tile;
#endif
	
	for(int y = 0; y < _nSectionHeight; y++)
	{
		for(int x = 0; x < _nSectionWidth; x++)
		{
			MPTile *pTile = pSection->pTileData + _nSectionWidth * y + x;
			pTile->Init();
			if( _bEdit )
			{
				fread(&tile, sizeof(tile), 1, _fp);
			}
			else
			{
				memcpy(&tile, m_pMapData.get() + dwPos, sizeof(tile));
				dwPos += sizeof(tile);
			}
#ifdef NEW_VERSION            
			TileInfo_5To8(tile.dwTileInfo, tile.btTileInfo, (BYTE*)(&pTile->TexLayer[0]));
            pTile->dwColor = LW_RGB565TODWORD(tile.sColor);
			pTile->dwColor|=0xff000000;
            pTile->fHeight  = (float)(tile.cHeight * 10) / 100.0f;
#else 			
			pTile->dwColor = tile.dwColor;
			memcpy(&pTile->TexLayer[0], &(tile.t[0]), 8);
            pTile->fHeight  = (float)(tile.sHeight) / 100.0f;
#endif            
			pTile->btIsland = tile.btIsland;
            pTile->sRegion  = tile.sRegion;
            memcpy(&pTile->btBlock[0], &tile.btBlock[0], 4); 
        }
	}
}


MPActiveMapSection *MPMap::LoadSectionData(int nSectionX, int nSectionY)
{
    MPActiveMapSection *pSection = new MPActiveMapSection;
	pSection->Init();
	pSection->nX = nSectionX;
	pSection->nY = nSectionY;
	
    _LoadSectionData(pSection);
    _ActiveSectionArray[nSectionY][nSectionX] = pSection;
    _ActiveSectionList.push_back(pSection);
    return pSection;
}

void MPMap::ClearSectionData(int nSectionX, int nSectionY)
{
    // 
    _WriteSectionDataOffset(nSectionX, nSectionY, 0);
    
    // 
    MPActiveMapSection *pSection = GetActiveSection(nSectionX, nSectionY);
	if(pSection) // Section
    {
        _ActiveSectionList.remove(pSection);
        _ActiveSectionArray[nSectionY][nSectionX] = NULL;
        SAFE_DELETE(pSection);
    }
}

void MPMap::FullLoading()
{
    if(_fp==NULL) return;

    // Section
    for(int i = 0; i < _nSectionCnt; i++)
    {
        int nSectionX = i % _nSectionCntX;
        int nSectionY = i / _nSectionCntX;
        if(*(_pOffsetIdx + i)!=0)
        {
            MPActiveMapSection *pSection = LoadSectionData(nSectionX, nSectionY);
            _pfnProc(0, pSection->nX, pSection->nY, (DWORD)(pSection), this);
        }
    }
}

void MPMap::DynamicLoading(DWORD dwTimeParam)
{
    if(_fp==NULL) return;

	int nCenterSectionX = _fShowCenterX / _nSectionWidth;
	int nCenterSectionY = _fShowCenterY / _nSectionHeight;

	if( nCenterSectionX == _nLastSectionX && nCenterSectionY == _nLastSectionY )
	{
		return;
	}
	else
	{
	   _nLastSectionX = nCenterSectionX;
	   _nLastSectionY = nCenterSectionY;
	}
	
	MPTimer t;
    t.Begin();
    int nCurSectionX = (int)(_fShowCenterX - (float)_nShowWidth  / 2.0f)  / _nSectionWidth;
	int nCurSectionY = (int)(_fShowCenterY - (float)_nShowHeight / 2.0f)  / _nSectionHeight;

    int nEndSectionX = (int)(_fShowCenterX + (float)_nShowWidth  / 2.0f)  / _nSectionWidth;
	int nEndSectionY = (int)(_fShowCenterY + (float)_nShowHeight / 2.0f)  / _nSectionHeight;

	int nShowSectionCntX = nEndSectionX - nCurSectionX;
	int nShowSectionCntY = nEndSectionY - nCurSectionY;
	
	if(_nShowWidth  % _nSectionWidth!=0)   nShowSectionCntX++;
	if(_nShowHeight % _nSectionHeight!=0)  nShowSectionCntY++;
	
	list<MPActiveMapSection*> _NewList;

    for(int y = 0; y < nShowSectionCntY; y++)
	{
		int nSectionY = nCurSectionY + y;

		if(nSectionY < 0 || nSectionY >= _nSectionCntY) continue;
		for(int x = 0; x < nShowSectionCntX; x++)
		{
			int nSectionX = nCurSectionX + x;

			if(nSectionX < 0 || nSectionX >= _nSectionCntX) continue;
		
			MPActiveMapSection *pSection = GetActiveSection(nSectionX, nSectionY);
			if(!pSection)
			{
				pSection = LoadSectionData(nSectionX, nSectionY);
                _NewList.push_back(pSection);
           	}
			pSection->dwActiveTime = dwTimeParam;
		}
	}

    if((int)(_ActiveSectionList.size()) >= _nSectionBufferSize)
	{
        static MPActiveMapSection *DelSectionList[MAX_DEL_SECTION];
		int n = 0;
        BOOL bDelFail = FALSE;
		for(list<MPActiveMapSection*>::iterator it = _ActiveSectionList.begin(); it!=_ActiveSectionList.end(); it++)
		{
			MPActiveMapSection *pCur = (*it);
			if(pCur->dwActiveTime!=dwTimeParam)
			{
				DelSectionList[n] = pCur;
				n++;
                if(n >= MAX_DEL_SECTION)
                {
                    bDelFail = TRUE;
                    break;
                }
			}
		}
        if(bDelFail) ToLogService("map", "Section, Buffer, n = {}", n);
	
		for(int i = 0; i < n; i++) // Section Tile Data
		{
		    UpdateRender(TRUE);
        	_ActiveSectionList.remove(DelSectionList[i]);
			if(DelSectionList[i]->dwDataOffset!=0)
			{
				_SaveSection(DelSectionList[i]);
			}
		    if(_pfnProc)	_pfnProc(1, DelSectionList[i]->nX, DelSectionList[i]->nY, (DWORD)(DelSectionList[i]),this);
			_ActiveSectionArray[DelSectionList[i]->nY][DelSectionList[i]->nX] = NULL;
        	SAFE_DELETE(DelSectionList[i]->pTileData);
			SAFE_DELETE(DelSectionList[i]);
		}
	}

    
    // MapNotice
    for(list<MPActiveMapSection*>::iterator it = _NewList.begin(); it!=_NewList.end(); it++)
	{
        MPActiveMapSection *pNewSection = (*it); 
        if(_pfnProc) _pfnProc(0, pNewSection->nX, pNewSection->nY, (DWORD)(pNewSection),this);
	    UpdateRender(TRUE);
    }

    _NewList.clear();
    
    // g_Render.Print(INFO_DEBUG, 10, 30, "Active Map Section = %d\n", _ActiveSectionList.size());

    m_dwActiveSectionCnt = (DWORD)(_ActiveSectionList.size());
    
    
    DWORD dwLoadingTime = t.End();
    if(dwLoadingTime > m_dwMaxLoadingTime)
    {
        m_dwMaxLoadingTime = dwLoadingTime;
    }
    
    if(dwLoadingTime)
    {
        m_dwLoadingTime[_dwLoadingCnt] = dwLoadingTime;
        _dwLoadingCnt++;
        if(_dwLoadingCnt>=3) _dwLoadingCnt = 0;
    }

	m_pBlock->GetBlockByRange(_fShowCenterX,_fShowCenterY,m_iRange);
}

void MPMap::ClearAllSection(BOOL bSaveFlag)
{
    for(list<MPActiveMapSection*>::iterator it = _ActiveSectionList.begin(); it!=_ActiveSectionList.end(); it++)
	{
		MPActiveMapSection *pSection = (*it);
		if(pSection->dwDataOffset)
		{
			if(bSaveFlag) _SaveSection((*it));
		}
	    if(bSaveFlag)
        {
            if(_pfnProc) _pfnProc(1, pSection->nX, pSection->nY, (DWORD)(pSection),this);
        }
        _ActiveSectionArray[pSection->nY][pSection->nX] = NULL;
    	SAFE_DELETE(pSection->pTileData);
		SAFE_DELETE(pSection);
	}
    
    ClearSectionArray();
   _ActiveSectionList.clear();

   m_pMapData.reset();
}

void MPMap::ClearSectionArray()
{
	memset(&_ActiveSectionArray, 0, 512 * 512 * 4);
}




