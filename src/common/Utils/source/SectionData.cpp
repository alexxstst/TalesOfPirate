
#include "SectionData.h"
#include <sys/types.h>
#include <sys/stat.h>

CSectionDataMgr::CSectionDataMgr()
:_bEdit(FALSE),
 _fp(NULL),
 _pfnProc(NULL),
 _nLoadCenterX(0),
 _nLoadCenterY(0),
 _SectionArray(NULL),
 _nLoadWidth(0),
 _nLoadHeight(0),
 _bDebug(FALSE)
{
    strcpy(_szDataName, "noname");
}

CSectionDataMgr::~CSectionDataMgr()
{
    if(_fp) fclose(_fp);
}

BOOL CSectionDataMgr::Create(int nSectionCntX, int nSectionCntY, const char *pszMapName)
{
    strcpy(_szFileName, pszMapName);
   	
    if(_bDebug)
    {
        //LG(GetDataName(), "[%s], Size = (%d %d)\n", pszMapName, nSectionCntX, nSectionCntY);
		LG(GetDataName(), "begin create new data file[%s], Size = (%d %d)\n", pszMapName, nSectionCntX, nSectionCntY);
    }
    
    _nSectionCntX   = nSectionCntX;
	_nSectionCntY   = nSectionCntY;

    
	_fp = fopen(pszMapName, "wb");
    if(_fp==NULL)
    {
        //LG(GetDataName(), "msg %s !\n", pszMapName);
		LG(GetDataName(), "msg file %s create failed!\n", pszMapName);
        return FALSE;
    }
	
	_bEdit = TRUE;
    
    _WriteFileHeader();

    int nTotal = GetSectionCnt();
    
    // 0
    _SectionArray = new SDataSection*[nTotal];
	
    memset(_SectionArray, 0, nTotal * sizeof(SDataSection*));

    for(int i = 0; i < nTotal; i++)
    {
        _WriteSectionIdx(i, 0);
    }

    if(_bDebug)
    {
        //LG(GetDataName(), ", TotalSectionCnt = %d!\n", nTotal);
		LG(GetDataName(), "create ok, TotalSectionCnt = %d!\n", nTotal);
    }
    return TRUE;
}

// , 
BOOL CSectionDataMgr::CreateFromFile(const char *pszMapName, BOOL bEdit)
{
    strcpy(_szFileName, pszMapName);
    
    if(_bDebug)
    {
       // LG(GetDataName(), "[%s]\n", pszMapName);
		 LG(GetDataName(), "begin read data file [%s]\n", pszMapName);
    }
    
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
		LG("map", "Load Map File[%s] Error!\n", pszMapName);
		return FALSE;
	}
	
    _fp = fp;

    if(!_ReadFileHeader())
    {
        //LG("map", "msg, [%s] !\n", pszMapName);
		LG("map", "msg read file failed, [%s] invalid file!\n", pszMapName);
        fclose(fp);
        return FALSE;
    }
    
    _bEdit = bEdit;
    
    _SectionArray = new SDataSection*[GetSectionCnt()];
	
    memset(_SectionArray, 0, GetSectionCnt() * sizeof(SDataSection*));
    return TRUE;
}

SDataSection *CSectionDataMgr::LoadSectionData(int nSectionX, int nSectionY)
{
    // Section
    DWORD dwLoc = (nSectionY * _nSectionCntX + nSectionX);
    
    SDataSection *pSection = new SDataSection;
    *(_SectionArray + dwLoc) = pSection;
    // _SectionList.push_back(pSection);
    
    pSection->dwDataOffset = _ReadSectionIdx(dwLoc);
        
    if(pSection->dwDataOffset!=0)
    {
        pSection->pData = new BYTE[_GetSectionDataSize()];
		
        // Section
        fseek(_fp, pSection->dwDataOffset, SEEK_SET);
        if(_bDebug)
        {
            LG(GetDataName(), "Seek Offset = %d\n", pSection->dwDataOffset);
        }
        fread(pSection->pData, _GetSectionDataSize(), 1, _fp);
        
        _AfterReadSectionData(pSection, nSectionX, nSectionY);
    }

    return pSection;
}

void CSectionDataMgr::SaveSectionData(int nSectionX, int nSectionY)
{
	if(!_bEdit || _fp==NULL) return;
	
	SDataSection *pSection = GetSectionData(nSectionX, nSectionY);
    if(!pSection) return;
    
    if(pSection->dwDataOffset)
	{
		if(_bDebug)
        {
            //LG(GetDataName(), "[%d %d], !\n", nSectionX, nSectionY);
			LG(GetDataName(), "[%d %d] exsit, save directly!\n", nSectionX, nSectionY);
        }
        
        fseek(_fp, pSection->dwDataOffset, SEEK_SET);
    }
    else
    {
	    if(_bDebug)
        {
            //LG(GetDataName(), "[%d %d]\n", nSectionX, nSectionY);
			LG(GetDataName(), "[%d %d] alloc new area at file tail \n", nSectionX, nSectionY);
        }
        // Data
        // , 
        // 
        // Trim, 
	
        // 
	    fseek(_fp, 0, SEEK_END);
	    pSection->dwDataOffset = ftell(_fp);
    }
    fwrite(pSection->pData, _GetSectionDataSize(), 1, _fp);
   	
    _WriteSectionIdx(nSectionY * _nSectionCntX + nSectionX, pSection->dwDataOffset);
}

void CSectionDataMgr::ClearSectionData(int nSectionX, int nSectionY)
{
    // 
    DWORD dwLoc = (nSectionY * _nSectionCntX + nSectionX);
    _WriteSectionIdx(dwLoc, 0);
	
    // 
    SDataSection *pSection = GetSectionData(nSectionX, nSectionY);
	if(pSection) // Section
    {
        // _SectionList.remove(pSection);
         *(_SectionArray + dwLoc) = NULL;
		 SAFE_DELETE(pSection);
    }
}

void CSectionDataMgr::CopySectionData(SDataSection *pDest, SDataSection *pSrc)
{
    DWORD dwLastOffset = pDest->dwDataOffset;
    memcpy(pDest, pSrc, sizeof(SDataSection));
    
    pDest->dwDataOffset = dwLastOffset; // 
    
    if(pSrc->pData==NULL)
    {
        if(pDest->pData!=NULL) SAFE_DELETE(pDest->pData);
    }
    else
    {
        if(pDest->pData==NULL) pDest->pData = new BYTE[_GetSectionDataSize()];
        memcpy(pDest->pData, pSrc->pData, _GetSectionDataSize());
    }
}


void CSectionDataMgr::Load(BOOL bFull, DWORD dwTimeParam)
{
    if(_fp==NULL) return;

    if(bFull)
    {
        int nTotal = _nSectionCntX * _nSectionCntY;
        // Section
        for(int i = 0; i < nTotal; i++)
        {
            int nSectionX = i % _nSectionCntX;
            int nSectionY = i / _nSectionCntX;
            DWORD dwOffset = _ReadSectionIdx(i);
            if(dwOffset!=0)
            {
                SDataSection *pSection = LoadSectionData(nSectionX, nSectionY);
            }
            if(i%50==0)
            {
                if(_pfnProc) _pfnProc(0, nSectionX, nSectionY, 0, this);
            }
        }
        return;
    }
}





/*
LPBYTE Util_LoadFile(FILE *fp)
{
    DWORD dwBufSize = 1024 * 1024 * 4;
    
    fseek(fp, 0, SEEK_END);
    DWORD dwFileSize = ftell(fp);
    DWORD dwLeft = dwFileSize;

    LPBYTE pbtFileBuf = new BYTE[dwFileSize]; 
    
    fseek(fp, 0, SEEK_SET);
    LPBYTE pbtBuf = pbtFileBuf;
    while(!feof(fp))
    {
        DWORD dwRead = dwBufSize;
        if(dwLeft - dwBufSize < 0) dwRead = dwLeft;
        fread(pbtBuf, dwRead, 1, fp);
        dwLeft-=dwRead;
        pbtBuf+=dwRead;
        if(dwLeft<=0) break;
    }
    return pbtFileBuf;    
}*/
