#pragma once

#include "Util.h"

// MapData 

// , 

// , 

// , , 


struct SDataSection
{
    void*		pData;				// 
    DWORD		dwDataOffset;		//  = 0, 
	DWORD		dwActiveTime;		// 
    DWORD       dwParam;            // 
	
	SDataSection()
    :pData(NULL),dwActiveTime(0),dwDataOffset(0),dwParam(0)
	{
	}
};


class CSectionDataMgr;
typedef long (CALLBACK* MAP_LOADING_PROC)(int nFlag, int nSectionX, 
								  int nSectionY, unsigned long dwParam, CSectionDataMgr* pMapData);

class CSectionDataMgr
{
public:

    ~CSectionDataMgr();
    
    // , 
    BOOL    CreateFromFile(const char *pszMapName, BOOL bEdit = FALSE);
    
    // , 
    BOOL    Create(int nSectionCntX, int nSectionCntY, const char *pszMapName);
	
    // , , 
    void	Load(BOOL bFull, DWORD dwTimeParam = 0);
    
    // Section
    void	SetLoadSectionCallback(MAP_LOADING_PROC pfn)	{	_pfnProc = pfn;				        }
    
    // Section
    void    CopySectionData(SDataSection* pDest, SDataSection *pSrc);
    
    // Section
    void    SaveSectionData(int nSectionX, int nSectionY);

    // Section, 
    void    ClearSectionData(int nSectionX, int nSectionY);

    // Section, 
    SDataSection* GetSectionData(int nSectionX, int nSectionY);
    
    // Section
    SDataSection* LoadSectionData(int nSectionX, int nSectionY);

	
	// map
	int		GetSectionCntX()						        { return _nSectionCntX;			        }
	int		GetSectionCntY()						        { return _nSectionCntY;			        }
    int     GetSectionCnt()                                 { return _nSectionCntX * _nSectionCntY; }
    char*   GetFileName()                                   { return _szFileName;                   }
    char*   GetDataName()                                   { return _szDataName;                    }
    void    SetDataName(const char *pszName)                { strcpy(_szDataName, pszName);          }   

    // 
    // int     CalValidSectionCnt();
    
    // 
    void	SetLoadSize(int nWidth, int nHeight)	{ _nLoadWidth  = nWidth;_nLoadHeight = nHeight; }
	int		GetLoadWidth()							{ return _nLoadWidth;					        }
	int		GetLoadHeight()							{ return _nLoadHeight;					        }
	void	SetLoadCenter(int nX, int nY)		    { _nLoadCenterX = nX; _nLoadCenterY = nY;       }
	int 	GetLoadCenterX()						{ return _nLoadCenterX;					        }
	int 	GetLoadCenterY()						{ return _nLoadCenterY;					        }
	BOOL    IsValidLocation(int nX, int nY);        
	int		GetSectionDataSize()					{ return _GetSectionDataSize();					}

protected:

    CSectionDataMgr();

    virtual int     _GetSectionDataSize()   = 0;
    virtual BOOL    _ReadFileHeader()       = 0;
    virtual void    _WriteFileHeader()      = 0;
    virtual DWORD   _ReadSectionIdx(DWORD dwSectionNo) = 0;
    virtual void    _WriteSectionIdx(DWORD dwSectionNo, DWORD dwDataOffset) = 0;
    virtual void    _AfterReadSectionData(SDataSection *pSection, int nSectionX, int nSectionY) {}
    
protected:

    SDataSection*   _GetDataSection(DWORD dwSectionNo)
    {
        return *(_SectionArray + dwSectionNo);
    }

	char                _szFileName[255];   // 
    char                _szDataName[32];     // 
	int					_nSectionCntX;		// Section
	int					_nSectionCntY;		// Section
	int					_nLoadCenterX;		// 
	int 				_nLoadCenterY;
	int					_nLoadWidth;		// 
	int					_nLoadHeight;
	
    int					_nSectionStartX;
	int					_nSectionStartY;
    
	MAP_LOADING_PROC	_pfnProc;			// 
	
	FILE*				_fp;				// 
	BOOL				_bEdit;				// 
    BOOL                _bDebug;
	
    std::list<SDataSection*>	_SectionList;
	SDataSection**       	    _SectionArray;
};

inline SDataSection* CSectionDataMgr::GetSectionData(int nSectionX, int nSectionY)
{
    if(!IsValidLocation(nSectionX, nSectionY))
    {
        return NULL;
    }

    DWORD dwLoc = (nSectionY * _nSectionCntX + nSectionX);
    return (SDataSection*)(*(_SectionArray + dwLoc));
}

inline BOOL CSectionDataMgr::IsValidLocation(int nX, int nY)
{
    if(nX < 0 || nX >= _nSectionCntX || nY < 0 || nY >= _nSectionCntY)
    {
        return FALSE;
    }
    return TRUE;
}

