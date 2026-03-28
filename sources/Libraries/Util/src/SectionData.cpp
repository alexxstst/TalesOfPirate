
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
        //LG(GetDataName(), "开始创建新的数据文件[%s], Size = (%d %d)\n", pszMapName, nSectionCntX, nSectionCntY);
		g_logManager.LogDebug("common", "begin create new data file[{}], Size = ({} {})", pszMapName, nSectionCntX, nSectionCntY);
    }
    
    _nSectionCntX   = nSectionCntX;
	_nSectionCntY   = nSectionCntY;

    
	_fp = fopen(pszMapName, "wb");
    if(_fp==NULL)
    {
        //LG(GetDataName(), "msg文件 %s 创建失败!\n", pszMapName);
		g_logManager.LogError("common", "file {} create failed!", pszMapName);
        return FALSE;
    }
	
	_bEdit = TRUE;
    
    _WriteFileHeader();

    int nTotal = GetSectionCnt();
    
    // 索引区全部填写为0
    _SectionArray = new SDataSection*[nTotal];
	
    memset(_SectionArray, 0, nTotal * sizeof(SDataSection*));

    for(int i = 0; i < nTotal; i++)
    {
        _WriteSectionIdx(i, 0);
    }

    if(_bDebug)
    {
        //LG(GetDataName(), "创建结束, TotalSectionCnt = %d!\n", nTotal);
		g_logManager.LogDebug("common", "create ok, TotalSectionCnt = {}!", nTotal);
    }
    return TRUE;
}

// 从文件读入, 并决定是否需要编辑操作
BOOL CSectionDataMgr::CreateFromFile(const char *pszMapName, BOOL bEdit)
{
    strcpy(_szFileName, pszMapName);
    
    if(_bDebug)
    {
       // LG(GetDataName(), "开始读取数据文件[%s]\n", pszMapName);
		 g_logManager.LogDebug("common", "begin read data file [{}]", pszMapName);
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
		g_logManager.LogError("map", "Load Map File[{}] Error!", pszMapName);
		return FALSE;
	}
	
    _fp = fp;

    if(!_ReadFileHeader())
    {
        //LG("map", "msg读取文件头失败, [%s] 可能是无效数据文件!\n", pszMapName);
		g_logManager.LogError("map", "read file failed, [{}] invalid file!", pszMapName);
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
    // 读取Section数据的文件偏移位置
    DWORD dwLoc = (nSectionY * _nSectionCntX + nSectionX);
    
    SDataSection *pSection = new SDataSection;
    *(_SectionArray + dwLoc) = pSection;
    // _SectionList.push_back(pSection);
    
    pSection->dwDataOffset = _ReadSectionIdx(dwLoc);
        
    if(pSection->dwDataOffset!=0)
    {
        pSection->pData = new BYTE[_GetSectionDataSize()];
		
        // 读取实际的Section数据
        fseek(_fp, pSection->dwDataOffset, SEEK_SET);
        if(_bDebug)
        {
            g_logManager.LogDebug("common", "Seek Offset = {}", pSection->dwDataOffset);
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
            //LG(GetDataName(), "[%d %d]区块数据索引值已经存在, 直接保存!\n", nSectionX, nSectionY);
			g_logManager.LogDebug("common", "[{} {}] exsit, save directly!", nSectionX, nSectionY);
        }
        
        fseek(_fp, pSection->dwDataOffset, SEEK_SET);
    }
    else
    {
	    if(_bDebug)
        {
            //LG(GetDataName(), "[%d %d]到文件末尾分配新的区块数据空间\n", nSectionX, nSectionY);
			g_logManager.LogDebug("common", "[{} {}] alloc new area at file tail", nSectionX, nSectionY);
        }
        // 首先应该寻找废弃的Data数据段
        // 但缺乏废弃区域索引, 所以只能到文件末尾去添加
        // 这样会在粘贴数据的过程中产生空间的浪费
        // 需要在发布时把数据Trim一次, 此过程有点像文件磁盘碎片
	
        // 到文件末尾添加新的数据块
	    fseek(_fp, 0, SEEK_END);
	    pSection->dwDataOffset = ftell(_fp);
    }
    fwrite(pSection->pData, _GetSectionDataSize(), 1, _fp);
   	
    _WriteSectionIdx(nSectionY * _nSectionCntX + nSectionX, pSection->dwDataOffset);
}

void CSectionDataMgr::ClearSectionData(int nSectionX, int nSectionY)
{
    // 清除文件里的记录
    DWORD dwLoc = (nSectionY * _nSectionCntX + nSectionX);
    _WriteSectionIdx(dwLoc, 0);
	
    // 清除内存里的记录
    SDataSection *pSection = GetSectionData(nSectionX, nSectionY);
	if(pSection) // 此Section已经读入内存
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
    
    pDest->dwDataOffset = dwLastOffset; // 数据偏移位置保持不变
    
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
        // 读取全部Section数据
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