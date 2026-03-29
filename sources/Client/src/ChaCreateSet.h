

class CChaCreateInfo : public CRawDataInfo
{

public:

    //		    
	CChaCreateInfo()
	{
	}

    DWORD type;
    DWORD bone;
    DWORD hair[64];
    DWORD face[64];
    DWORD body[64];
    DWORD hand[64];
    DWORD foot[64];
    DWORD hair_num;
    DWORD face_num;
    DWORD body_num;
    DWORD hand_num;
    DWORD foot_num;
    DWORD profession;
    char description[1024];
};

class CChaCreateSet : public CRawDataSet
{

public:

	static CChaCreateSet* I() { return _Instance; }

	CChaCreateSet(int nIDStart, int nIDCnt)
		:CRawDataSet(nIDStart, nIDCnt)
	{
		_Instance = this;
		_Init();
	}

protected:

	static CChaCreateSet* _Instance; // , 

	virtual CRawDataInfo* _CreateRawDataArray(int nCnt)
	{
		return new CChaCreateInfo[nCnt];
	}

	virtual void _DeleteRawDataArray()
	{
		delete[] (CChaCreateInfo*)_RawDataArray;
	}

	virtual int _GetRawDataInfoSize()
	{
		return sizeof(CChaCreateInfo);
	}

	virtual void*	_CreateNewRawData(CRawDataInfo *pInfo)
	{
		return NULL;
	}

	virtual void  _DeleteRawData(CRawDataInfo *pInfo)
	{
		SAFE_DELETE(pInfo->pData);
	}

	virtual BOOL _ReadRawDataInfo(CRawDataInfo *pRawDataInfo, std::vector<std::string> &ParamList)
	{   
		if(ParamList.size()==0) return FALSE;

		CChaCreateInfo *pInfo = (CChaCreateInfo*)pRawDataInfo;

        const DWORD buf_size = 64;
        std::string str[buf_size];

        pInfo->bone = atoi( ParamList[0].c_str() );


        pInfo->face_num = Util_ResolveTextLine(ParamList[1].c_str(), str, buf_size, ',');
        for( DWORD i = 0; i < pInfo->face_num; i++ )
        {
            pInfo->face[i] = atoi( str[i].c_str() );
        }
        pInfo->hair_num = Util_ResolveTextLine(ParamList[2].c_str(), str, buf_size, ',');
        for( DWORD i = 0; i < pInfo->hair_num; i++ )
        {
            pInfo->hair[i] = atoi( str[i].c_str() );
        }
        pInfo->body_num = Util_ResolveTextLine(ParamList[3].c_str(), str, buf_size, ',');
        for( DWORD i = 0; i < pInfo->body_num; i++ )
        {
            pInfo->body[i] = atoi( str[i].c_str() );
        }
        pInfo->hand_num = Util_ResolveTextLine(ParamList[4].c_str(), str, buf_size, ',');
        for( DWORD i = 0; i < pInfo->hand_num; i++ )
        {
            pInfo->hand[i] = atoi( str[i].c_str() );
        }
        pInfo->foot_num = Util_ResolveTextLine(ParamList[5].c_str(), str, buf_size, ',');
        for( DWORD i = 0; i < pInfo->foot_num; i++ )
        {
            pInfo->foot[i] = atoi( str[i].c_str() );
        }

		return TRUE;
	}
};

inline CChaCreateInfo* GetChaCreateInfo(int nTypeID)
{
	return (CChaCreateInfo*)CChaCreateSet::I()->GetRawDataInfo(nTypeID);
}
