#pragma once

#include "TableData.h"
#include <windows.h>


#define MONSTER_LIST_MAX   8

//
class CMonsterInfo : public CRawDataInfo
{
public:

	CMonsterInfo()
	{
        memset(szName, 0, sizeof(szName));
        memset(&ptStart, 0, sizeof(POINT));
        memset(&ptEnd, 0, sizeof(POINT));
        memset(nMonsterList, 0, sizeof(nMonsterList));
        memset(szArea, 0, sizeof(szArea));
    }

    char    szName[32];
    POINT   ptStart;
    POINT   ptEnd;
    int     nMonsterList[MONSTER_LIST_MAX];
    char    szArea[32];
};

class CMonsterSet : public CRawDataSet
{
public:

	static CMonsterSet* I() { return _Instance; }

	CMonsterSet(int nIDStart, int nIDCnt)
	:CRawDataSet(nIDStart, nIDCnt)
	{
	    _Instance = this;
		_Init();
	}

protected:

	static CMonsterSet* _Instance; // 

	virtual CRawDataInfo* _CreateRawDataArray(int nCnt)
	{
		return new CMonsterInfo[nCnt];
	}

	virtual void _DeleteRawDataArray()
	{
		delete[] (CMonsterInfo*)_RawDataArray;
	}
	
	virtual int _GetRawDataInfoSize()
	{
		return sizeof(CMonsterInfo);
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
        int nCol = 0;
        std::string strList[MONSTER_LIST_MAX];

        CMonsterInfo* pInfo = (CMonsterInfo*)pRawDataInfo;

        //  ID
        //strncpy(pInfo->szName, ParamList[nCol++].c_str(), sizeof(pInfo->szName));

        // 
        if(2 == Util_ResolveTextLine(ParamList[nCol++].c_str(), strList, 2, ','))
        {
            pInfo->ptStart.x = Str2Int(strList[0]) / 100;
            pInfo->ptStart.y = Str2Int(strList[1]) / 100;
        }

        // 
        if(2 == Util_ResolveTextLine(ParamList[nCol++].c_str(), strList, 2, ','))
        {
            pInfo->ptEnd.x = Str2Int(strList[0]) / 100;
            pInfo->ptEnd.y = Str2Int(strList[1]) / 100;
        }

        // 
        int nCount = Util_ResolveTextLine(ParamList[nCol++].c_str(), strList, MONSTER_LIST_MAX, ',');
        for(int i = 0; i < nCount; ++i)
        {
            pInfo->nMonsterList[i] = Str2Int(strList[i]);
        }

        // 
        strncpy(pInfo->szArea, ParamList[nCol++].c_str(), sizeof(pInfo->szArea));
		//strncpy_s(pInfo->szArea,sizeof(pInfo->szArea) ,ParamList[nCol++].c_str(),_TRUNCATE );

        //LG("MonsterSet", "Read Monster Set [%d][%s]\n", pInfo->nID, pInfo->szDataName);

        return TRUE;
    }
};

inline CMonsterInfo* GetMonsterInfo(int nMapID)
{
	return (CMonsterInfo*)CMonsterSet::I()->GetRawDataInfo(nMapID);
}

inline CMonsterInfo* GetMonsterInfo(const char *pszMapName)
{
    return (CMonsterInfo*)CMonsterSet::I()->GetRawDataInfo(pszMapName);
}


