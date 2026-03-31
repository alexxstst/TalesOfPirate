#pragma once
#include "ServerRecord.h"
#include "ServerRecordStore.h"
#include "AssetDatabase.h"

#include <vector>
#include <map>
typedef std::vector<std::string>	ReginList;
typedef std::map<int, ReginList> ReginListMap;

extern std::string g_serverset;

class CServerSet : public CRawDataSet
{
public:
    static CServerSet* I() { return _Instance; }

    CServerSet(int nIDStart, int nIDCnt)
        :CRawDataSet(nIDStart, nIDCnt)
    {
        _Instance = this;
        _Init();

		memset( m_nCurGroupList, 0, sizeof(m_nCurGroupList) );
		memset( m_nCurGroupCnt, 0, sizeof(m_nCurGroupCnt) );

		std::ifstream in;
		in.open(g_serverset.c_str());

		for(int i = 0; i < MAX_REGION; i++)
			m_nCurGroupCnt[i] = 0;

		std::string strLine;
		char   szRegion[256] = { 0 };
		if(in.is_open())
		{
			m_nRegionCnt = 0;
			while(!in.eof())
			{
				in.getline( szRegion, sizeof(szRegion) );
				if( strlen(szRegion)==0 ) break;

				std::string strList[2];
				Util_ResolveTextLine(szRegion, strList, 2, ',');

				strcpy(m_szRegionName[m_nRegionCnt], strList[0].c_str());
				strcpy(m_szRegionID[m_nRegionCnt++], strList[1].c_str());

				if(m_nRegionCnt>=MAX_REGION) break;
			}
			in.close();
    	}
	}

public:

	int		m_nCurGroupList[MAX_REGION][MAX_REGION_GROUP]; // Group
	int		m_nCurGroupCnt[MAX_REGION];

	char    m_szRegionName[MAX_REGION][32];
	char    m_szRegionID[MAX_REGION][32];

	int     m_nRegionCnt;

protected:
    
	static CServerSet* _Instance; // , 

    virtual CRawDataInfo* _CreateRawDataArray(int nCnt)
    {
        return new CServerGroupInfo[nCnt];
    }

    virtual void _DeleteRawDataArray()
    {
        delete[] (CServerGroupInfo*)_RawDataArray;
    }

    virtual int _GetRawDataInfoSize()
    {
        return sizeof(CServerGroupInfo);
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
        CServerGroupInfo *pInfo = (CServerGroupInfo*)pRawDataInfo;
        
		strcpy(pInfo->szRegion, ParamList[0].c_str());
		for(int i = 0; i < MAX_GROUP_GATE; i++) // 5gate ip
		{
			strcpy(pInfo->szGateIP[i], ParamList[i + 1].c_str());
			if(strcmp(pInfo->szGateIP[i], "0")==0)
			{
				break;
			}
			pInfo->cValidGateCnt++;
			
		}
		
		return TRUE;
    }

	virtual void _ProcessRawDataInfo(CRawDataInfo *pRawDataInfo)
	{
	    CServerGroupInfo *pInfo = (CServerGroupInfo*)pRawDataInfo;

		for (size_t i = 0; i < m_nRegionCnt; i++)
		{
			if (strcmp(m_szRegionName[i], pInfo->szRegion) == 0)
			{
				m_nCurGroupList[i][m_nCurGroupCnt[i]] = pInfo->nID;
				m_nCurGroupCnt[i]++;
				break;
			}
		}

		ServerRecordStore::Insert(AssetDatabase::Instance()->GetDb(), *pInfo);
	}
};

// , GateIP
inline CServerGroupInfo* GetServerGroupInfo(int nGroupID)
{
    return (CServerGroupInfo*)CServerSet::I()->GetRawDataInfo(nGroupID);
}

// , GateIP
inline CServerGroupInfo* GetServerGroupInfo(const char *pszGroupName)
{
    return (CServerGroupInfo*)CServerSet::I()->GetRawDataInfo(pszGroupName);
}

inline int GetCurServerGroupCnt(int nRegionNo)
{
	return CServerSet::I()->m_nCurGroupCnt[nRegionNo];
}

inline const char* GetCurServerGroupName(int nRegionNo, int nGroupNo)
{
	if(nGroupNo>=GetCurServerGroupCnt(nRegionNo)) return 0;

	int nNo = CServerSet::I()->m_nCurGroupList[nRegionNo][nGroupNo];
	return GetServerGroupInfo(nNo)->szDataName;
}

//  Michael Chen 2005-06-01
inline int GetRegionCnt()
{
	return CServerSet::I()->m_nRegionCnt;
}

inline const char* GetCurRegionName(int nRegionNo)
{
	if (nRegionNo < 0 && nRegionNo > GetRegionCnt())
		return 0;

	return CServerSet::I()->m_szRegionName[nRegionNo];
}

// , GateIP
inline const char *SelectGroupIP(int nRegionNo, int nGroupNo)
{
	ToLogService("connections", "Select Region {} Group {}", nRegionNo, nGroupNo);
	if(nGroupNo>=GetCurServerGroupCnt(nRegionNo)) 
	{
		{ char _buf[512]; snprintf(_buf, sizeof(_buf), g_oLangRec.GetString(387), GetCurServerGroupCnt(nRegionNo), nGroupNo); g_logManager.InternalLog(LogLevel::Debug, "connections", _buf); }
		return 0;
	}

	int nNo = CServerSet::I()->m_nCurGroupList[nRegionNo][nGroupNo];
	CServerGroupInfo *pGroup = GetServerGroupInfo(nNo);
	if(!pGroup) 
	{
		ToLogService("connections", "Group Not Found!");
		return NULL;
	}

	if(pGroup->cValidGateCnt==0) 
	{
		g_logManager.InternalLog(LogLevel::Debug, "connections", g_oLangRec.GetString(388));
		return NULL;
	}

	srand(GetTickCount());

	int nGateNo = rand()%(int)(pGroup->cValidGateCnt); 

	{ char _buf[512]; snprintf(_buf, sizeof(_buf), g_oLangRec.GetString(389), pGroup->szDataName, nGateNo, pGroup->szGateIP[nGateNo], pGroup->cValidGateCnt); g_logManager.InternalLog(LogLevel::Debug, "connections", _buf); }

	return pGroup->szGateIP[nGateNo];
}
