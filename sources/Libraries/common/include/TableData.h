#pragma once

// Raw Data : ԭʼ����
// Raw Data Set : ������ʵ������ֻ��һ��, ʹ�ø����ݵ�ʵ��ȴ���Դ��������Ӧ�ó���
// ���� : Meshģ������, ��ͼ����, �Ǽ����ݵȵ�,  Ҳ�������ڷ���Ϸ�ĸ���������

// RawDataSet�����Ҫ����
//1. ԭʼ���ݵ���Դ��������(�ı�,������)
//2. ͨ��ID��������
//3. ��̬�ͷ�
//4  ��Դ����Ͱ����ݴ�ȡ

// Ŀǰ��Ӧ�û�ֻ��ΪһЩ��Դ������ʽ��Ϊ�򵥵�����������ӿ�
// ����:  ID  ������Դ(�ļ���) �򵥲������� ����ĸ�ʽ

// ������ʽ:
// ����ID = �����±�
// ͨ������ID������ԭʼ����


// ������ʹ��ǰ, ����̳����µķ���
// virtual int				_GetRawDataInfoSize()										      // ÿ�������RawDataInfo���в�ͬ, ȡ��RawDataInfo�����ݳߴ�
// virtual void*			_CreateNewRawData(CRawDataInfo *pRawInfo)		    		      // ȡ���µ�RawData����, ����ģ������ָ�룬Ҳ��������ͼ����ָ��
// virtual void				_ReadRawDataInfo(CRawDataInfo *pRawInfo, list<string> &ParamList) // ��Դ�ļ�ÿ�����µ�һ��, ������Եõ��Ļص�����
// virtual void				_DeleteRawData(void *pData);								      // ɾ����Դ, ��Դ��ɾ����ʽ����������в�ͬ	

// ����, ����������Լ��Ĺ��������_Init()����


#include <fstream>
#include <cryptlib.h>
#include <aes.h>
#include <filters.h>
#include <gcm.h>
#include <base64.h>
#include <util2.h>
#include <logutil.h>

class CRawDataInfo
{
public:
	BOOL	bExist{ false };				// ��Դ�Ƿ����
	int		nIndex{ 0 };				// ��Array�е�λ��				
	char	szDataName[72]{ "" };		// ������Դ(ͨ���������ļ���)
	DWORD	dwLastUseTick{ 0 };		// �ϴ�ʹ�õ�ʱ��
	BOOL	bEnable{ true };			// �Ƿ���Ч, ���Զ�̬����
	void* pData{ nullptr };				// ʵ������
	DWORD   dwPackOffset{ 0 };		// �ڰ��ļ��е�����ƫ��
	DWORD   dwDataSize{ 0 };			// ԭʼ���ݳߴ�(�ļ��ߴ�)
	int     nID{ 0 };				// ID
	DWORD   dwLoadCnt{ 0 };          // ��Դ��ȡ����
};



class CRawDataSet
{

protected:

	CRawDataSet(int nIDStart, int nIDCnt, int nFieldCnt = DEFAULT_FIELD_CNT) // һ��Ҫ�̳�ʹ��
	:_nIDStart(nIDStart),
	_nIDCnt(nIDCnt),
    _nIDLast(nIDCnt)
	{
		_nMaxFieldCnt = std::max<decltype(_nMaxFieldCnt)>(nFieldCnt, DEFAULT_FIELD_CNT);
   	}

public:

    void*			GetRawData(int nID, BOOL bRequest = FALSE);
	void*			GetRawData(const char* pszDataName, int *pnID);
	CRawDataInfo*	GetRawDataInfo(int nID);
    CRawDataInfo*   GetRawDataInfo(const char *pszDataName);
    int             GetRawDataID(const char *pszDataName);
	
	BOOL			LoadRawDataInfo(const char *pszFileName, BOOL bBinary = FALSE);
	BOOL			LoadRawDataInfoEx(const char *pszFileName, BOOL bBinary = FALSE);
	void			WriteBinFile(const char *pszFileName);
	
	BOOL			IsValidID(int nID);
    int             GetLastID() const {return _nIDLast;}
	
	// ���ڶ�̬�ͷŵĲ�������
    void			SetReleaseInterval(DWORD dwInterval)	{ _dwReleaseInterval = dwInterval;	}
	void			SetMaxRawData(int nDataCnt)				{ _nMaxRawDataCnt	 = nDataCnt;	}
	
	int				GetLoadedRawDataCnt()					{ return _nLoadedRawDataCnt;		}
	void			DynamicRelease(BOOL bClearAll = FALSE);
	void			Release();
    void            FrameLoad(int nFrameLoad = 2);

	// ����й�
    void			EnablePack(const char *pszPackName);	// ���ڶ����������Դ�����ļ�����Ч
	void            Pack(const char *pszPackName, const char *pszBinName);
    void			PackFromDirectory(std::list<std::string> &DirList, const char *pszPackName, const char *pszBinName);
    BOOL            IsEnablePack()              { return _bEnablePack; } 
	
    // ��Դ��ȡ
    LPBYTE			LoadRawFileData(CRawDataInfo *pInfo);
	
    void            EnableRequest(BOOL bEnable)   { _bEnableRequest = bEnable; }
	
protected:

	virtual CRawDataInfo*	_CreateRawDataArray(int nIDCnt)								    = 0;	
	virtual void			_DeleteRawDataArray()										    = 0;	
	virtual int             _GetRawDataInfoSize()										    = 0;
	virtual void*			_CreateNewRawData(CRawDataInfo *pInfo)					  	    = 0;
	virtual BOOL			_ReadRawDataInfo(CRawDataInfo *pInfo, std::vector<std::string> &ParamList)= 0;
	virtual void			_ProcessRawDataInfo(CRawDataInfo *pInfo)                        {}
	virtual void			_DeleteRawData(CRawDataInfo *pInfo)							    = 0;
	virtual BOOL			_IsFull()
	{
		if(_nLoadedRawDataCnt <= _nMaxRawDataCnt) return FALSE;
		return TRUE;
	}
	virtual void            _AfterLoad(){}

	BOOL		_LoadRawDataInfo_Bin(const char *pszFileName);
	BOOL		_LoadRawDataInfo_Txt(const char *pszFileName, int nSep = '\t');
	void		_WriteRawDataInfo_Bin(const char *pszFileName);
    void        _Init();
    CRawDataInfo*	_GetRawDataInfo(int nID); // ����������Χ���

protected:
	
	int						    _nIDStart;
	int						    _nIDCnt;
	int						    _nUnusedIndex{ 0 };
	DWORD					    _dwReleaseInterval{1000 * 60 * 1};
	int						    _nMaxRawDataCnt{ 50 };
	int						    _nLoadedRawDataCnt{ 0 };
	DWORD                       _dwMaxFrameRawDataSize{ 0 };
	BOOL				    	_bEnablePack{ false };
	char                        _szPackName[64];
	BOOL						_bBinary{ false };
	CRawDataInfo* _RawDataArray{ nullptr };
    std::map<std::string, CRawDataInfo*>	_IDIdx;
    std::list<int>                   _RequestList;
	BOOL                        _bEnableRequest{ false };
    int                         _nIDLast;

	// add by claude
	enum {DEFAULT_FIELD_CNT = 80};
	int _nMaxFieldCnt;
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
	if (!IsValidID(nID)) return NULL;

    CRawDataInfo* pInfo = _GetRawDataInfo(nID);
	if (pInfo->bExist) return pInfo;
    else return NULL;
}

// ����������Χ��⣬��Ҫ��������
inline CRawDataInfo* CRawDataSet::_GetRawDataInfo(int nID)
{
    LPBYTE pbtData = (LPBYTE)_RawDataArray;

    CRawDataInfo *pInfo = (CRawDataInfo*)(pbtData + _GetRawDataInfoSize() * (nID - _nIDStart));
    return pInfo;
}

inline CRawDataInfo* CRawDataSet::GetRawDataInfo(const char *pszDataName)
{
	std::map<std::string, CRawDataInfo*>::iterator it = _IDIdx.find(pszDataName);

	if(it!=_IDIdx.end()) // ��ID�Ѿ�����
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

inline int CRawDataSet::GetRawDataID(const char *pszDataName) // �����ֻ�ȡID, ���û�������һ��
{
	CRawDataInfo *pInfo;

    std::map<std::string, CRawDataInfo*>::iterator it = _IDIdx.find(pszDataName);

	if(it!=_IDIdx.end()) // ��ID�Ѿ�����
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

extern BOOL  g_bBinaryTable;

inline BOOL CRawDataSet::LoadRawDataInfo(const char *pszFile, BOOL bBinary)
{
	char szTxtName[255], szBinName[255];

	if(g_bBinaryTable) bBinary = TRUE;
	
	_bBinary  = bBinary;

	sprintf(szTxtName, "%s.txt", pszFile);
	sprintf(szBinName, "%s.bin", pszFile);

    BOOL bRet = FALSE;
	if(bBinary) 
	{
		bRet = _LoadRawDataInfo_Bin(szBinName);
	}
	else
	{
		bRet = _LoadRawDataInfo_Txt(szTxtName);
		if(bRet)
		{
			_WriteRawDataInfo_Bin(szBinName);
		}
	}

    try {
        _AfterLoad();
    } catch (...) {}

    return bRet;
}

inline BOOL CRawDataSet::LoadRawDataInfoEx(const char *pszFile, BOOL bBinary)
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

    std::list<int>::iterator it;
    std::list<int> FinishList;
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
    
    if(_IsFull()==FALSE) return;
	
	DWORD dwCurTick = GetTickCount();

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
			// LG2("debug", "Dynamic Release Raw Data [%s]\n", pInfo->szDataName);
		}
	}

    
}

inline void CRawDataSet::Release()
{
	if( _nLoadedRawDataCnt > 0 ) //��ȫ�ͷ��ڴ� by Waiting 2009-06-18
	{
		for(int i = 0; i < _nIDCnt; i++)
		{
			CRawDataInfo *pInfo = GetRawDataInfo(_nIDStart + i);
			if( NULL==pInfo || NULL==pInfo->pData )  //��ȫ�ͷ��ڴ� by Waiting 2009-06-18
				continue;

			_DeleteRawData(pInfo);
			pInfo->pData = NULL;
			_nLoadedRawDataCnt--;
			if(_nLoadedRawDataCnt < 0)
			{
				ToLogService("errors", LogLevel::Error, "LoadedRawDataCnt = {} , < 0 ?", _nLoadedRawDataCnt);
			}
		}
	}
    //��ȫ�ͷ��ڴ� by Waiting 2009-06-18
	if( _RawDataArray )
	{
		_DeleteRawDataArray();
		_RawDataArray = NULL;
	}
	_nUnusedIndex = 0;
}
/*
inline BOOL CRawDataSet::_LoadRawDataInfo_Bin(const char *pszFileName)
{

	const unsigned char cluTableKey[] = { 0x32, 0x72, 0x35, 0x75, 0x38, 0x78, 0x2f, 0x41, 0x3f, 0x44, 0x28, 0x47, 0x2b, 0x4b, 0x62, 0x50 };
	const unsigned char cluTableIV[] = { 0x43, 0x2a, 0x46, 0x29, 0x4a, 0x40, 0x4e, 0x63, 0x52, 0x66, 0x55, 0x6a, 0x58, 0x6e, 0x32, 0x72 };
	
	FILE* fp = fopen(pszFileName, "rb");
	char szMsg[MAX_PATH] = {0};

	if(fp==NULL) 
	{
		ToLogService("errors", LogLevel::Error, "Load Raw Data Info Bin File [{}] Failed!", pszFileName);
		//MessageBox(NULL, szMsg, "����", MB_OK | MB_ICONERROR);
		sprintf(szMsg, "Open table file failed:%s\nProgram will exit!\n", pszFileName);
		MessageBox(NULL, szMsg, "Error", MB_OK | MB_ICONERROR);
		return FALSE;
	}
	
    int nSize     = Util_GetFileSize(fp);
    int nInfoSize = _GetRawDataInfoSize();
    int nResCnt   = (nSize-12) / nInfoSize;

    LPBYTE pbtResInfo = new BYTE[nSize];
    
	//DWORD dwInfoSize = 0; 
	//fread(&dwInfoSize, 4, 1, fp);
	/*
	if(dwInfoSize!=_GetRawDataInfoSize())
	{
		//MessageBox(NULL, szMsg, "Error2", MB_OK | MB_ICONERROR);
		//LG2("table", "msg��ȡ�����ļ�[%s]ʱ, ���ְ汾��һ��!\n", pszFileName);
		ToLogService("common", LogLevel::Error, " read table file [{}], version can't match!", pszFileName);
		fclose(fp);
		//MessageBox(NULL, szMsg, "����", MB_OK | MB_ICONERROR);
		sprintf(szMsg, "Open table file failed:%s\nProgram will exit!\n", pszFileName);
		MessageBox(NULL, szMsg, "Error", MB_OK | MB_ICONERROR);
		exit(0);
		return FALSE;
	}
	
	int test = fread(pbtResInfo, nSize, 1, fp);

	FILE* fp2 = fopen("test2.bin", "wb");
	fwrite(pbtResInfo, nSize, 1, fp2);

	std::string base64;
	std::string sink;
	CryptoPP::GCM<CryptoPP::AES>::Decryption d;
	d.SetKeyWithIV(cluTableKey, 16, cluTableIV, 16);

	CryptoPP::StringSource(pbtResInfo, nSize, true, new CryptoPP::Base64Decoder(new CryptoPP::StringSink(base64)));
	CryptoPP::AuthenticatedDecryptionFilter df(d, new CryptoPP::StringSink(sink), CryptoPP::AuthenticatedDecryptionFilter::DEFAULT_FLAGS, 12);
	CryptoPP::StringSource(base64,true, new CryptoPP::Redirector(df));

	//if (sink.size() != nSize - 12) {
	//	MessageBox(NULL, szMsg, "Error", MB_OK | MB_ICONERROR);
	//}
	memset(pbtResInfo, 0, nSize);
	memcpy(pbtResInfo, sink.c_str(), sink.size());

    for(int i = 0; i < nResCnt; i++)
    {
        CRawDataInfo *pInfo = (CRawDataInfo*)(pbtResInfo + i * _GetRawDataInfoSize());
		// modify by lark.li 20080424 begin
		//strcpy(pInfo->szDataName, ConvertResString(pInfo->szDataName));
		// End

        if(!pInfo->bExist) continue;
		if(IsValidID(i)==FALSE) continue;
		CRawDataInfo* pCurInfo = _GetRawDataInfo(pInfo->nID);
		memcpy(pCurInfo, pInfo, nInfoSize); // ���ԭ�е���Ϣ
         _IDIdx[pCurInfo->szDataName] = pCurInfo;
        //vector<string> ParamList; _ReadRawDataInfo(pCurInfo, ParamList);
        _ProcessRawDataInfo(pCurInfo);
		ToLogService("common", "Load Bin RawData [{}] = {}", pCurInfo->szDataName, pCurInfo->nID);
    }
    
    delete pbtResInfo;
    
    fclose(fp);
	return TRUE;
}

inline void CRawDataSet::_WriteRawDataInfo_Bin(const char *pszFileName)
{

	const unsigned char cluTableKey[] = { 0x32, 0x72, 0x35, 0x75, 0x38, 0x78, 0x2f, 0x41, 0x3f, 0x44, 0x28, 0x47, 0x2b, 0x4b, 0x62, 0x50 };
	const unsigned char cluTableIV[] =	{ 0x43, 0x2a, 0x46, 0x29, 0x4a, 0x40, 0x4e, 0x63, 0x52, 0x66, 0x55, 0x6a, 0x58, 0x6e, 0x32, 0x72 };
	FILE* fp = fopen(pszFileName, "wb");
	if(fp==NULL) return;
	char szMsg[MAX_PATH] = { 0 };
	DWORD dwInfoSize = _GetRawDataInfoSize();

	//fwrite(&dwInfoSize, 4, 1, fp);
	LPBYTE buffer = (LPBYTE)malloc((dwInfoSize * _nIDCnt) + 12);
	for (int i = 0; i < _nIDCnt; i++) {
		CRawDataInfo* pInfo = (CRawDataInfo*)((LPBYTE)_RawDataArray + i * _GetRawDataInfoSize());
		if (pInfo->bExist)
		{
			memcpy(buffer + (i * dwInfoSize), pInfo, dwInfoSize);
		}

	}
		
	CryptoPP::GCM<CryptoPP::AES>::Encryption e;
	std::string sink;
	std::string base64;
	e.SetKeyWithIV(cluTableKey, 16, cluTableIV, 16);
	CryptoPP::StringSource(buffer, (dwInfoSize * _nIDCnt), true, new CryptoPP::AuthenticatedEncryptionFilter(e, new CryptoPP::StringSink(sink), false, 12));
	CryptoPP::StringSource(sink, true, new CryptoPP::Base64Encoder(new CryptoPP::StringSink(base64), false));
    fwrite(base64.c_str(), base64.size(), 1, fp);

	//if (sink.size() != (dwInfoSize * _nIDCnt) + 12) {
	//	MessageBox(NULL, szMsg, "Error", MB_OK | MB_ICONERROR);
	//}
	fclose(fp);
}
*/

inline BOOL CRawDataSet::_LoadRawDataInfo_Txt(const char *pszFileName, int nSep)
{
	BOOL bRet = TRUE;
	std::ifstream in(pszFileName);
    if(in.is_open()==0)
    {
       // LG2("error", "msgLoad Raw Data Info Txt File [%s] Fail!\n", pszFileName);
        return FALSE;
    }
	
const int LINE_SIZE = 2048;
	char szLine[LINE_SIZE];
	std::string* pstrList = new std::string[_nMaxFieldCnt + 1];
	std::string strComment;

	std::vector<std::string> ParamList;

	// add by claude at 2004-9-1
	BOOL bSaveFieldCnt = FALSE;
	int nFieldCnt = 0;

    
    while(!in.eof())
    {
		in.getline(szLine, LINE_SIZE);
		std::string strLine = szLine;
		
		int p = (int)strLine.find("//");
		if(p!=-1)
		{
			std::string strLeft = strLine.substr(0, p);
			strComment = strLine.substr(p + 2, strLine.size() - p - 2);
			strLine = strLeft;
		}
		else
		{
			strComment = "";
		}
		
		int n = Util_ResolveTextLine(strLine.c_str(), pstrList, _nMaxFieldCnt + 1, nSep);
		if (n < 2) continue;
		if (n > _nMaxFieldCnt)
			{
			//LG2("error", "msg����Դ�ļ�[%s]�У�ʵ���ֶ�������Ԥ�����ֶ���\n", pszFileName);
				ToLogService("errors", LogLevel::Error, " in resource [{}], the field num is greater than predefine count ", pszFileName);
			bRet = FALSE;
            break;}

		// ��¼�������ֶ���Ŀ
		if (!bSaveFieldCnt)
			{
			nFieldCnt = n;
            bSaveFieldCnt = TRUE;}
		else {
			// �Ƚϴ����ֶ���Ŀ���һ���ֶ���Ŀ�Ƿ���ͬ
			if (nFieldCnt != n)
				{
				// �����ͬ��˵������Դ�ļ��������ݴ���
				//LG2("error", "msg������Դ�ļ�[%s]ʧ��,���[%s], �����ʽ�Ͱ汾!\n",
					ToLogService("errors", LogLevel::Error, " parse resource file [{}] failed ,No [{}], please chech format and version!", pszFileName, pstrList[0].c_str());

				bRet = FALSE;
                break;}
			}

        int	nID = Str2Int(pstrList[0]);
        if (!IsValidID(nID))
            {
            //LG2("error", "msg����[%d]����Ԥ����Χ��������Դ�ļ�[%s]\n", nID, pszFileName);
				ToLogService("errors", LogLevel::Error, " index [{}] overflow,please check resource file [{}]", nID, pszFileName);
            bRet = FALSE;
            break;}

        _nIDLast = nID;

        CRawDataInfo *pInfo = _GetRawDataInfo(nID);
		pInfo->bExist = TRUE;
		
        ParamList.clear();
		int i;
        for(i = 0; i < n - 2; i++)
        {
			ParamList.push_back(pstrList[i + 2]);
		}
		for(i = 0; i < 15; i++)
			ParamList.push_back(""); // ���ÿմ�,���������������, ��ʾ��ʽ����

		//Util_TrimString(pstrList[1]);
		Util_TrimTabString(pstrList[1]); // ����Ӣ�� MAKEBIN �ո�ʧ����  modify by Philip.Wu  2006-07-31

		strcpy(pInfo->szDataName, pstrList[1].c_str());
		// char *pszDataName = _strupr( _strdup( pInfo->szDataName ) );
		
		char *pszDataName = ( _strdup( pInfo->szDataName ) );
		strcpy(pInfo->szDataName, pszDataName);
        free(pszDataName);

		_IDIdx[pInfo->szDataName] = pInfo;

        BOOL bRet = FALSE;
        // try 
		{
			bRet = _ReadRawDataInfo(pInfo, ParamList);
			_ProcessRawDataInfo(pInfo);
		}
        //catch (...)
        {
		//	LG2("error", "msg������Դ�ļ�[%s]����δ֪���쳣,����ʧ�ܣ����[%s], �����ʽ!\n", pszFileName, pstrList[0].c_str());
		//	bRet = FALSE; break;
    	}        
        if(!bRet)
        {
            //LG2("error", "msg������Դ�ļ�[%s]ʧ��,���[%s], �����ʽ�Ͱ汾!\n",
			ToLogService("errors", LogLevel::Error, " parse resource file [{}] failed ,No [{}], please chech format and version!", pszFileName, pstrList[0].c_str());
			bRet = FALSE; break;
        }
	}

	delete[] pstrList;
	in.close();
	return bRet;
}


//----------------------------------------------------------------------------------------------------------
//												�����ش���
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
	
	_WriteRawDataInfo_Bin(pszBinName); // ���֮����дRawDataSet Bin�ļ�
}


//--------------------------------------------
//  ��Ŀ¼�ж�ȡ�ļ�, ÿ���ļ���Ϊһ����Դ, ��
//  ������Դ������Ϣ�ļ� xxx.bin
//--------------------------------------------
inline void	CRawDataSet::PackFromDirectory(std::list<std::string> &DirList, const char *pszPackName, const char *pszBinName)
{
	std::list<std::string> FileList;
	for(std::list<std::string>::iterator itD = DirList.begin(); itD!=DirList.end(); itD++)
	{
		std::string strDirName = (*itD);
		ProcessDirectory(strDirName.c_str(), &FileList, DIRECTORY_OP_QUERY);
	}
	
	int nFileCnt = (int)(FileList.size());
	
	FILE *fp = fopen(pszPackName, "wb");
		
	int i = 0;
	for(std::list<std::string>::iterator it = FileList.begin(); it!=FileList.end(); it++,i++)
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
	
	_WriteRawDataInfo_Bin(pszBinName); // ���֮����дRawDataSet Bin�ļ�
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
// ��ȡRawData���ݵ��ڴ�(һ�����ڰ������ļ�����, ��Ӱ���ȡ�������ļ����ݵĳ���
// ������������Ҫ��ʹ�ô˺���, Ҳ������ȫ����)
//-----------------------------------------------------------------------------
inline LPBYTE CRawDataSet::LoadRawFileData(CRawDataInfo *pInfo)
{
	LPBYTE pbtBuf    = NULL;
	DWORD  dwBufSize = 0;
	if(_bEnablePack) // �Ӱ��ж�ȡ
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