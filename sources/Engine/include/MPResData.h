#pragma once

// Raw Data : 原始数据
// Raw Data Set : 适用于实际数据只有一份, 使用该数据的实例却可以创建多个的应用场合
// 例如 : Mesh模型数据, 贴图数据, 骨架数据等等,  也可以用在非游戏的各种软件里

// RawDataSet类的主要功能
//1. 原始数据的资源描述读入(文本,二进制)
//2. 通过ID索引数据
//3. 动态释放
//4  资源打包和包数据存取

// 目前的应用还只能为一些资源描述格式较为简单的数据做基类接口
// 例如:  ID  数据来源(文件名) 简单参数集合 这类的格式

// 索引方式:
// 类型ID = 数组下标
// 通过类型ID索引到原始数据


// 子类在使用前, 必须继承以下的方法
// virtual int				_GetRawDataInfoSize()										      // 每个子类的RawDataInfo都有不同, 取得RawDataInfo的数据尺寸
// virtual void*			_CreateNewRawData(CRawDataInfo *pRawInfo)		    		      // 取得新的RawData数据, 可能模型数据指针，也可以是贴图表面指针
// virtual void				_ReadRawDataInfo(CRawDataInfo *pRawInfo, list<string> &ParamList) // 资源文件每读到新的一行, 子类可以得到的回调处理
// virtual void				_DeleteRawData(void *pData);								      // 删除资源, 资源的删除方式可能子类各有不同	

// 另外, 子类必须在自己的构造里调用_Init()函数


#include <fstream>


class MINDPOWER_API CRawDataInfo
{
public:	
	
	CRawDataInfo()
	:bExist(FALSE),
	dwLastUseTick(0),
	bEnable(TRUE),
	pData(NULL),
	dwPackOffset(0),
	dwDataSize(0),
	nIndex(0),
    dwLoadCnt(0)
	{
		strcpy(szDataName, "");	
	}
	
    BOOL	bExist;				// 资源是否存在
	int		nIndex;				// 在Array中的位置				
	char	szDataName[72];		// 数据来源(通常是数据文件名)
	DWORD	dwLastUseTick;		// 上次使用的时间
	BOOL	bEnable;			// 是否有效, 可以动态设置
	void*   pData;				// 实际数据
	DWORD   dwPackOffset;		// 在包文件中的数据偏移
	DWORD   dwDataSize;			// 原始数据尺寸(文件尺寸)
	int     nID;				// ID
    DWORD   dwLoadCnt;          // 资源读取次数
};



class MINDPOWER_API CRawDataSet
{

protected:

	CRawDataSet(int nIDStart, int nIDCnt) // 一定要继承使用
	:_nIDStart(nIDStart),
	_nIDCnt(nIDCnt),
	_bEnablePack(FALSE),
	_bBinary(FALSE),
	_nUnusedIndex(0),
	_RawDataArray(NULL)
	{
		_dwReleaseInterval  = 1000 * 60 * 1;		// 默认1分钟没有使用到的会被清除
		// _dwReleaseInterval  = 1000 * 30;			// 默认30秒没有使用到的会被清除
		_nMaxRawDataCnt          = 50;				// 默认内存中超过50个RawData时, 会按时间释放没有使用的RawData
		_nLoadedRawDataCnt       = 0;
        _dwMaxFrameRawDataSize   = 0;                // 单个Frame读入的最大尺寸, 0表示无限制
        _bEnableRequest          = FALSE;
   	}

public:

    void*			GetRawData(int nID, BOOL bRequest = FALSE);
	void*			GetRawData(const char* pszDataName, int *pnID);
	CRawDataInfo*	GetRawDataInfo(int nID);
    CRawDataInfo*   GetRawDataInfo(const char *pszDataName);
    int             GetRawDataID(const char *pszDataName);
	
	BOOL			LoadRawDataInfo(const char *pszFileName, BOOL bBinary = FALSE);
	void			WriteBinFile(const char *pszFileName);
	
	BOOL			IsValidID(int nID);
	
	// 对于动态释放的参数设置
    void			SetReleaseInterval(DWORD dwInterval)	{ _dwReleaseInterval = dwInterval;	}
	void			SetMaxRawData(int nDataCnt)				{ _nMaxRawDataCnt	 = nDataCnt;	}
	
	int				GetLoadedRawDataCnt()					{ return _nLoadedRawDataCnt;		}
	void			DynamicRelease(BOOL bClearAll = FALSE);
	void			Release();
    void            FrameLoad(int nFrameLoad = 2);

	// 打包有关
    void			EnablePack(const char *pszPackName);	// 仅在读入二进制资源描述文件后有效
	void            Pack(const char *pszPackName, const char *pszBinName);
    void			PackFromDirectory(list<string> &DirList, const char *pszPackName, const char *pszBinName);
    BOOL            IsEnablePack()              { return _bEnablePack; } 
	
    // 资源读取
    LPBYTE			LoadRawFileData(CRawDataInfo *pInfo);
	
    void            EnableRequest(BOOL bEnable)   { _bEnableRequest = bEnable; }
protected:

	virtual CRawDataInfo*	_CreateRawDataArray(int nIDCnt)								    = 0;	
	virtual void			_DeleteRawDataArray()										    = 0;	
	virtual int             _GetRawDataInfoSize()										    = 0;
	virtual void*			_CreateNewRawData(CRawDataInfo *pInfo)					  	    = 0;
	virtual BOOL			_ReadRawDataInfo(CRawDataInfo *pInfo, vector<string> &ParamList)= 0;
	virtual void			_DeleteRawData(CRawDataInfo *pInfo)							    = 0;

	BOOL		_LoadRawDataInfo_Bin(const char *pszFileName);
	BOOL		_LoadRawDataInfo_Txt(const char *pszFileName);
	void		_WriteRawDataInfo_Bin(const char *pszFileName);
    void        _Init();

protected:
	
	int						    _nIDStart;
	int						    _nIDCnt;
	int						    _nUnusedIndex;
	DWORD					    _dwReleaseInterval;
	int						    _nMaxRawDataCnt;
	int						    _nLoadedRawDataCnt;
    DWORD                       _dwMaxFrameRawDataSize;
	BOOL				    	_bEnablePack;
	char                        _szPackName[64];
	BOOL						_bBinary;
    CRawDataInfo*			    _RawDataArray;
    map<string, CRawDataInfo*>	_IDIdx;
    list<int>                   _RequestList;
    BOOL                        _bEnableRequest;
};

inline void CRawDataSet::_Init()
{
    _DeleteRawDataArray();

	_RawDataArray = _CreateRawDataArray(_nIDCnt);

	LPBYTE pbtData = (LPBYTE)_RawDataArray;
	for(int i = 0; i < _nIDCnt; i++)
	{
		CRawDataInfo *pInfo = (CRawDataInfo*)(pbtData + _GetRawDataInfoSize() * i);
		pInfo->nIndex = i;
		pInfo->nID    = _nIDStart + i;
	}
}

inline CRawDataInfo* CRawDataSet::GetRawDataInfo(int nID)
{
	if(!IsValidID(nID)) return NULL;
	
	LPBYTE pbtData = (LPBYTE)_RawDataArray;
	
	CRawDataInfo *pInfo = (CRawDataInfo*)(pbtData + _GetRawDataInfoSize() * (nID - _nIDStart));

	return pInfo;
}

inline CRawDataInfo* CRawDataSet::GetRawDataInfo(const char *pszDataName)
{
	map<string, CRawDataInfo*>::iterator it = _IDIdx.find(pszDataName);

	if(it!=_IDIdx.end()) // 此ID已经存在
	{
		return (*it).second;
    }
    return NULL;
}

inline void* CRawDataSet::GetRawData(int nID, BOOL bRequest)
{
	CRawDataInfo *pInfo =  GetRawDataInfo(nID);
	if(pInfo==NULL) return NULL;
	
	pInfo->dwLastUseTick = GetTickCount();
	if(!pInfo->pData)
	{
	    if(bRequest && _bEnableRequest)
        {
            ToLogService("common", "Push Request RawData!");
            _RequestList.push_back(nID);
            return NULL;
        }
        
        pInfo->pData = _CreateNewRawData(pInfo);
        pInfo->dwLoadCnt++;
		if(pInfo->pData==NULL)
		{
			ToLogService("errors", LogLevel::Error, "Load Raw Data [{}] Failed! (ID = {})", pInfo->szDataName, nID);
		}
		else
		{
			_nLoadedRawDataCnt++;
		}
	}
	return pInfo->pData;
}

inline int CRawDataSet::GetRawDataID(const char *pszDataName) // 从名字获取ID, 如果没有则分配一个
{
	CRawDataInfo *pInfo;

    map<string, CRawDataInfo*>::iterator it = _IDIdx.find(pszDataName);

	if(it!=_IDIdx.end()) // 此ID已经存在
	{
		pInfo = (*it).second;
	}
	else
	{
		if(_nUnusedIndex >= _nIDCnt)
		{
			ToLogService("errors", LogLevel::Error, "RawDataSet OverMax Dynamic ID, MaxIDCnt = {}, Index = {}", _nIDCnt, _nUnusedIndex);
			return -1;
		}
		
		LPBYTE pbtData = (LPBYTE)_RawDataArray;
		pInfo = (CRawDataInfo*)(pbtData + _GetRawDataInfoSize() * _nUnusedIndex);
		strcpy(pInfo->szDataName, pszDataName);

		_IDIdx[pInfo->szDataName] = pInfo;
		_nUnusedIndex++;
	}
	return pInfo->nIndex + _nIDStart;
}


inline void* CRawDataSet::GetRawData(const char* pszDataName, int *pnID)
{
	int nID = GetRawDataID(pszDataName);
    if(pnID) *pnID = nID;
    if(nID==-1)
    {
        return NULL;
    }
    return GetRawData(nID);
}

inline BOOL CRawDataSet::IsValidID(int nID)
{
	if(nID < _nIDStart || nID >= (_nIDStart + _nIDCnt)) return FALSE;
	return TRUE;
}


inline BOOL CRawDataSet::LoadRawDataInfo(const char *pszFile, BOOL bBinary)
{
	char szTxtName[255], szBinName[255];

	_bBinary  = bBinary;
	
	sprintf(szTxtName, "%s.txt", pszFile);
	sprintf(szBinName, "%s.bin", pszFile);
	if(bBinary) 
	{
		return _LoadRawDataInfo_Bin(szBinName);
	}
	else
	{
		BOOL bLoad = _LoadRawDataInfo_Txt(szTxtName);
		if(bLoad)
		{
			_WriteRawDataInfo_Bin(szBinName);
		}
		return bLoad;
	}
	return TRUE;
}

inline void CRawDataSet::FrameLoad(int nFrameLoad)
{
    int nMaxLoadPerFrame = nFrameLoad;

    list<int>::iterator it;
    list<int> FinishList;
    int n = 0;
    for(it = _RequestList.begin(); it!=_RequestList.end(); it++)
    {
        int nID = (*it);
        GetRawData(nID, FALSE);
        FinishList.push_back(nID);
        n++;
        if(n > nFrameLoad) break;
    }
    
    for(it = FinishList.begin(); it!=FinishList.end(); it++)
    {
        int nID = (*it);
        _RequestList.remove(nID);
    }
}

inline void CRawDataSet::DynamicRelease(BOOL bClearAll)
{
	if(bClearAll)
    {
        for(int i = 0; i < _nIDCnt; i++)
	    {
		    CRawDataInfo *pInfo = GetRawDataInfo(_nIDStart + i);
		    if(pInfo->pData==NULL)  continue;
            _DeleteRawData(pInfo);
			pInfo->pData = NULL;
			_nLoadedRawDataCnt--;
			if(_nLoadedRawDataCnt < 0)
			{
			    ToLogService("errors", LogLevel::Error, "LoadedRawDataCnt = {} , < 0 ?", _nLoadedRawDataCnt);
			}
      	}
        return;
    }
    
    DWORD dwCurTick = GetTickCount();

	if(_nLoadedRawDataCnt <= _nMaxRawDataCnt) return;
	
	for(int i = 0; i < _nIDCnt; i++)
	{
		CRawDataInfo *pInfo = GetRawDataInfo(_nIDStart + i);
		if(pInfo->pData==NULL)  continue;
		
		if((dwCurTick - pInfo->dwLastUseTick) > _dwReleaseInterval)
		{
			_DeleteRawData(pInfo);
			pInfo->pData = NULL;
			_nLoadedRawDataCnt--;
			if(_nLoadedRawDataCnt < 0)
			{
				ToLogService("errors", LogLevel::Error, "LoadedRawDataCnt = {} , < 0 ?", _nLoadedRawDataCnt);
			}
			// LG("debug", "Dynamic Release Raw Data [%s]\n", pInfo->szDataName);
		}
	}

    
}

inline void CRawDataSet::Release()
{
	for(int i = 0; i < _nIDCnt; i++)
	{
		CRawDataInfo *pInfo = GetRawDataInfo(_nIDStart + i);
		if(pInfo->pData==NULL)  continue;
		
		_DeleteRawData(pInfo);
		pInfo->pData = NULL;
		_nLoadedRawDataCnt--;
		if(_nLoadedRawDataCnt < 0)
		{
			ToLogService("errors", LogLevel::Error, "LoadedRawDataCnt = {} , < 0 ?", _nLoadedRawDataCnt);
		}
	}
	_DeleteRawDataArray();
	_nUnusedIndex = 0;
    delete this;
}

inline BOOL CRawDataSet::_LoadRawDataInfo_Bin(const char *pszFileName)
{
	FILE* fp = fopen(pszFileName, "rb");
	if(fp==NULL) 
	{
		ToLogService("errors", LogLevel::Error, "Load Raw Data Info Bin File [{}] Failed!", pszFileName);
		return FALSE;
	}
	
    int nSize     = Util_GetFileSize(fp);
    int nInfoSize = _GetRawDataInfoSize();
    int nResCnt   = nSize / nInfoSize;

    LPBYTE pbtResInfo = new BYTE[nSize];
    
    fread(pbtResInfo, nSize, 1, fp);

	for (int i = 0; i < nSize; ++i)
    {
		// Shift back chars
        pbtResInfo[i] -= 15;
    }

    for(int i = 0; i < nResCnt; i++)
    {
        CRawDataInfo *pInfo = (CRawDataInfo*)(pbtResInfo + i * _GetRawDataInfoSize());
        if(!pInfo->bExist) continue;
        CRawDataInfo *pCurInfo = GetRawDataInfo(pInfo->nID);
        memcpy(pCurInfo, pInfo, nInfoSize); // 替代原有的信息
        _IDIdx[pCurInfo->szDataName] = pCurInfo;
        vector<string> ParamList; _ReadRawDataInfo(pCurInfo, ParamList);
        ToLogService("common", "Load Bin RawData [{}] = {}", pCurInfo->szDataName, pCurInfo->nID);
    }
    
    delete pbtResInfo;
    
    fclose(fp);
	return TRUE;
}

inline void CRawDataSet::_WriteRawDataInfo_Bin(const char *pszFileName)
{
	FILE* fp = fopen(pszFileName, "wb");
	if(fp==NULL) return;
	
	for(int i = 0; i < _nIDCnt; i++)
    {
        CRawDataInfo *pInfo = (CRawDataInfo*)((LPBYTE)_RawDataArray + i * _GetRawDataInfoSize());
        if(pInfo->bExist)
        {
			LPBYTE scramble = (LPBYTE)pInfo;
			for(int k = 0; k < _GetRawDataInfoSize(); k++){
			// Let's scramble chars here
			// Don't care about over/underflows, it will just loop on 0-255
			scramble[k]  += 15; // value pointed is shifted in the ascii table
			}
			//}
			//Done, let client/engine save it.
            fwrite(pInfo, _GetRawDataInfoSize(), 1, fp);
        }
    }
	fclose(fp);
}


inline BOOL CRawDataSet::_LoadRawDataInfo_Txt(const char *pszFileName)
{
	ifstream in(pszFileName);
    if(in.is_open()==0)
    {
        ToLogService("errors", LogLevel::Error, "Load Raw Data Info Txt File [{}] Fail!", pszFileName);
        return FALSE;
    }
	
	char szLine[255];
    string strList[16];
	string strComment;

	vector<string> ParamList;
    
    while(!in.eof())
    {
		in.getline(szLine, 255);
		string strLine = szLine;
		
		int p = (int)strLine.find("//");
		if(p!=-1)
		{
			string strLeft = strLine.substr(0, p);
			strComment = strLine.substr(p + 2, strLine.size() - p - 2);
			strLine = strLeft;
		}
		else
		{
			strComment = "";
		}
		
		int n = Util_ResolveTextLine(strLine.c_str(), strList, 15, ' ');

		if (n < 2) continue;

        int	nID = Str2Int(strList[0]);

        CRawDataInfo *pInfo = GetRawDataInfo(nID);
		pInfo->bExist = TRUE;
		
        ParamList.clear();
		int i;
        for(i = 0; i < n - 2; i++)
        {
			ParamList.push_back(strList[i + 2]);
		}
		for(i = 0; i < 15; i++) ParamList.push_back(""); // 放置空串,如果被后面代码读到, 表示格式不对
		
		Util_TrimString(strList[1]);
		strcpy(pInfo->szDataName, strList[1].c_str());
		char *pszDataName = _strlwr( _strdup( pInfo->szDataName ) );
		strcpy(pInfo->szDataName, pszDataName);
        free(pszDataName);

		_IDIdx[pInfo->szDataName] = pInfo;
        if(!_ReadRawDataInfo(pInfo, ParamList))
        {
            ToLogService("errors", LogLevel::Error, "解析资源文件[{}]失败, 请检查格式和版本!", pszFileName);
            in.close();
	        return FALSE;
        }
	}
    in.close();
	return TRUE;
}


//----------------------------------------------------------------------------------------------------------
//												打包相关处理
//----------------------------------------------------------------------------------------------------------
inline LPBYTE Util_LoadFile(const char *pszFileName, DWORD* pdwFileSize)
{
	if(strlen(pszFileName)==0) return NULL;
	FILE *fp = fopen(pszFileName, "rb");
	if(fp==NULL)
	{
		pdwFileSize = 0;
		return NULL;
	}
	fseek(fp, 0, SEEK_END);
	long lSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	LPBYTE pbtBuf = new BYTE[lSize];
	fread(pbtBuf, lSize, 1, fp);
	fclose(fp);
	*pdwFileSize = lSize;
	return pbtBuf;
}

inline LPBYTE Util_LoadFilePart(const char *pszFileName, DWORD dwStart, DWORD dwSize)
{
	FILE *fp = fopen(pszFileName, "rb");
	if(fp==NULL)
	{
		return NULL;
	}
	LPBYTE pbtBuf = new BYTE[dwSize];
	fseek(fp, dwStart, SEEK_SET);
	fread(pbtBuf, dwSize, 1, fp);
	fclose(fp);
	return pbtBuf;
}


inline void CRawDataSet::Pack(const char *pszPackName, const char *pszBinName)
{
	FILE *fp = fopen(pszPackName, "wb");
	for(int i = 0; i < _nIDCnt; i++)
	{
		CRawDataInfo *pInfo = GetRawDataInfo(i);
		if(!pInfo->bExist)  continue;
		DWORD  dwFileSize = 0;
		LPBYTE pbtFileContent = Util_LoadFile(pInfo->szDataName, &dwFileSize);
		if(pbtFileContent)
		{
            pInfo->dwPackOffset = ftell(fp);
			pInfo->dwDataSize   = dwFileSize;
			fwrite(pbtFileContent, dwFileSize, 1, fp);
			delete pbtFileContent;
		}
	}
	fclose(fp);
	
	_WriteRawDataInfo_Bin(pszBinName); // 打包之后重写RawDataSet Bin文件
}


//--------------------------------------------
//  从目录中读取文件, 每个文件作为一个资源, 并
//  生成资源描述信息文件 xxx.bin
//--------------------------------------------
inline void	CRawDataSet::PackFromDirectory(list<string> &DirList, const char *pszPackName, const char *pszBinName)
{
	list<string> FileList;
	for(list<string>::iterator itD = DirList.begin(); itD!=DirList.end(); itD++)
	{
		string strDirName = (*itD);
		ProcessDirectory(strDirName.c_str(), &FileList, DIRECTORY_OP_QUERY);
	}
	
	int nFileCnt = (int)(FileList.size());
	
	FILE *fp = fopen(pszPackName, "wb");
		
	int i = 0;
	for(list<string>::iterator it = FileList.begin(); it!=FileList.end(); it++,i++)
	{
		CRawDataInfo *pInfo = GetRawDataInfo(i + _nIDStart);
		
		strcpy(pInfo->szDataName, (*it).c_str());
		
		char *pszDataName = _strlwr( _strdup( pInfo->szDataName ) );
		strcpy(pInfo->szDataName, pszDataName);
		free(pszDataName);
		
		DWORD  dwFileSize = 0;
		LPBYTE pbtFileContent = Util_LoadFile(pInfo->szDataName, &dwFileSize);
		if(pbtFileContent)
		{
		    pInfo->bExist       = TRUE;
            pInfo->dwPackOffset = ftell(fp);
			pInfo->dwDataSize   = dwFileSize;
			fwrite(pbtFileContent, dwFileSize, 1, fp);
			delete pbtFileContent;
		}
        ToLogService("common", "Pack File (index = {}) ID = {} [{}]", pInfo->nIndex, pInfo->nID, pInfo->szDataName);
	}
	
	fclose(fp);
	
	_WriteRawDataInfo_Bin(pszBinName); // 打包之后重写RawDataSet Bin文件
}	

inline void CRawDataSet::EnablePack(const char *pszPackName)
{
	if(pszPackName)
    {
        _bEnablePack = TRUE;
        strcpy(_szPackName, pszPackName);
    }
    else
    {
        _bEnablePack = FALSE;
    }
}
	


//-----------------------------------------------------------------------------
// 读取RawData数据到内存(一般用在把整个文件读入, 或从包中取出完整文件内容的场合
// 子类视自身需要来使用此函数, 也可以完全不用)
//-----------------------------------------------------------------------------
inline LPBYTE CRawDataSet::LoadRawFileData(CRawDataInfo *pInfo)
{
	LPBYTE pbtBuf    = NULL;
	DWORD  dwBufSize = 0;
	if(_bEnablePack) // 从包中读取
	{
		pbtBuf    = Util_LoadFilePart(_szPackName, pInfo->dwPackOffset, pInfo->dwDataSize);
		dwBufSize = pInfo->dwDataSize;
	}
	else
	{
		pbtBuf    = Util_LoadFile(pInfo->szDataName, &dwBufSize);
		pInfo->dwDataSize = dwBufSize;
	}
	return pbtBuf;
}