#pragma once
/************************************************************************/
/*:
/*			
/*create by lemon@2005.6.3
/************************************************************************/
class CPackFile
{
public:
	CPackFile();
	
	~CPackFile();
	

	struct FileData  // 
	{
		s_string strFileName; // 
		DWORD offset;   //
		DWORD length;  // 
	};

	struct DirectoryData // 
	{
		s_string  strDirName; // 
		DWORD dwDirNum;  // 
		DWORD dwFileNum; // 
		std::vector<DirectoryData>	vecSubDir; // DirectoryData()vecSubDir
		std::vector<FileData>	vecFile; // FileData()vecFile
	};

	bool	SaveToPack(const char* pszDirName, const char* pszOutFile, const char* pszFilter = "*.bmp"); // 

	bool	LoadPackFile(char* pszFile); // 

	void	Clear(); 

	DirectoryData*	GetDir(char* pszDirName) // 
	{
		if(!_pPackFile)  
			return NULL;
		if(_RootDir.strDirName == pszDirName)
			return &_RootDir;
		return FindDir(pszDirName, &_RootDir);
	}
protected:
	bool	PackDirectory(FILE* ptf,DirectoryData* pParentDir,const char* pszDir); // 
	long	AddFileToTempPack(FILE* pf, const char* pszFileName); // 
	long	GetFileDataSize(FileData* pFileData); // 
	void	SaveFileData(FILE* pf, FileData* pFileData, long offset); //
	long	GetDirectorySize(DirectoryData* pDir); // 
	void	SaveDirData(FILE* pf, DirectoryData* pDir, long offset); // 


	bool	LoadFileData(FileData* pFileData); // 				
	bool	LoadDirData(DirectoryData* pDir);  // 	

	DirectoryData* FindDir(char* pszDirName, DirectoryData* pDir)  // 
	{
		if(pDir->strDirName == pszDirName)
			return pDir;
		WORD n;
		DirectoryData* ptDir;
		for (n = 0; n < pDir->dwDirNum; ++n)
		{
			if(ptDir = FindDir(pszDirName, &pDir->vecSubDir[n]))
				return ptDir;
		}
		return NULL;
	}
public:
	//pack
	s_string strName;
	s_string strFilter;

protected:

	FILE*			_pPackFile; // 
	DirectoryData	_RootDir; // 
}; 


class CMiniPack : public CPackFile // 
{
public:
	CMiniPack();
	~CMiniPack();

	bool 	Init(char* pszMapName);
	void	GetXY( char*pszName, int& x,int& y);
	bool	IsExit(int x,int y)
	{
		if(x >= _imaxX || y >= _imaxY)
			return false;
		if(!_pIdx[x + y * _imaxX])
		{
			return false;
		}
		return true;
	}
	void	GetTex(MPITex**	pTex, int x,int y)
	{
		int idx = x + y * _imaxX;
		if( idx > _imaxX * _imaxY)
		{
			*pTex = NULL;
			return;
		}
		FileData*	   _pFile = _pIdx[idx];
		if(!_pFile)
		{
			*pTex = NULL;
			return;
		}
		if(!_pTData || _lsize != _pFile->length)
		{
			_pTData = std::make_unique<std::byte[]>(_pFile->length);
			_lsize = _pFile->length;
		}
		fseek(_pPackFile,_pFile->offset, SEEK_SET); 
		fread(_pTData.get(), 1,_pFile->length,_pPackFile);

		texInfo.width = _pFile->length;
		texInfo.data = _pTData.get();

		lwLoadTex(pTex,g_Render.GetInterfaceMgr()->res_mgr, &texInfo);
	}
	s_string strMapName;
	lwTexInfo	texInfo;

protected:
	struct sXY
	{
		int x,y;
	};

	std::unique_ptr<std::byte[]> _pTData{};
	std::size_t _lsize{};
	std::unique_ptr<FileData* []> _pIdx{};
	int			  _imaxX{}, _imaxY{};

};
