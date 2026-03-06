#pragma once

#pragma pack(push)
#pragma pack(1)

struct SAttribFileHeader
	{
	unsigned int width;
	unsigned int height;
	};
typedef SAttribFileHeader terrain_attr_hdr;


typedef struct _Tile_Attrib
	{
	unsigned short attrib; 
    unsigned char island;

	} STILE_ATTRIB;
typedef _Tile_Attrib terrain_attr_dat;

#pragma pack(pop)

#include "SectionData.h"

class CAttribData:public CSectionDataMgr
{
protected:

    int     _GetSectionDataSize()   {   return 64 * sizeof(_Tile_Attrib);       }
    BOOL    _ReadFileHeader();
    void    _WriteFileHeader();
    DWORD   _ReadSectionIdx(DWORD dwSectionNo);
    void    _WriteSectionIdx(DWORD dwSectionNo, DWORD dwOffset);
	void*	_GetHeaderPtr()		{ return &_header;			}
	int		_GetHeaderSize()	{ return sizeof(_header);	}

    
    SAttribFileHeader _header;
    
};

inline BOOL CAttribData::_ReadFileHeader()
{
    fread(&_header, sizeof(_header), 1, _fp);
    _nSectionCntX = _header.width  / 8;
    _nSectionCntY = _header.height / 8;
    return TRUE;
}

inline void CAttribData::_WriteFileHeader()
{
    _header.width  = _nSectionCntX * 8;
    _header.height = _nSectionCntY * 8;

    fwrite(&_header, sizeof(_header), 1, _fp);
}

inline DWORD CAttribData::_ReadSectionIdx(DWORD dwSectionNo)
{
    fseek(_fp, sizeof(_header) + sizeof(DWORD) * dwSectionNo, SEEK_SET);
    DWORD dwOffset = 0; fread(&dwOffset, sizeof(DWORD), 1, _fp);
    if(_bDebug)
    {
        //LG(GetDataName(), "[%d %d], Offset = %d\n", dwSectionNo % _nSectionCntX, dwSectionNo / _nSectionCntY, dwOffset);
		LG(GetDataName(), "read index data [%d %d], Offset = %d\n", dwSectionNo % _nSectionCntX, dwSectionNo / _nSectionCntY, dwOffset);
    }
    return dwOffset;
}

inline void CAttribData::_WriteSectionIdx(DWORD dwSectionNo, DWORD dwOffset)
{
    fseek(_fp, sizeof(_header) + sizeof(DWORD) * dwSectionNo, SEEK_SET);
    fwrite(&dwOffset, sizeof(DWORD), 1, _fp);
    if(_bDebug)
    {
        //LG(GetDataName(), "[%d %d], Offset = %d\n", dwSectionNo % _nSectionCntX, dwSectionNo / _nSectionCntY, dwOffset);
		LG(GetDataName(), "write index data [%d %d], Offset = %d\n", dwSectionNo % _nSectionCntX, dwSectionNo / _nSectionCntY, dwOffset);
    }
}
