#pragma once

#include "TableData.h"


//  ID   
class CAreaInfo : public CRawDataInfo
{
public:

	CAreaInfo()
	{
        dwColor = 0;
    }

    DWORD dwColor;
    int   nMusic;
    DWORD dwEnvColor;
    DWORD dwLightColor;
    float fLightDir[3];
	char  chType;			// 0-,1-,
};


class CAreaSet : public CRawDataSet
{
public:	
	static CAreaSet* I() { return _Instance; }
	
	CAreaSet(int nIDStart, int nIDCnt)
	:CRawDataSet(nIDStart, nIDCnt)
	{
	    _Instance = this;
		_Init();
	}

protected:

	static CAreaSet* _Instance; // , 
   
	virtual CRawDataInfo* _CreateRawDataArray(int nCnt)
	{
		return new CAreaInfo[nCnt];
	}
	
	virtual void _DeleteRawDataArray()
	{
		delete[] (CAreaInfo*)_RawDataArray;
	}
	
	virtual int _GetRawDataInfoSize()
	{
		return sizeof(CAreaInfo);
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
        CAreaInfo *pArea = (CAreaInfo*)pRawDataInfo;
		std::string strList[3];
		int m=0;
        int n = Util_ResolveTextLine(ParamList[m++].c_str(), strList, 3, ','); 
        DWORD r = Str2Int(strList[0]);
        DWORD g = Str2Int(strList[1]);
        DWORD b = Str2Int(strList[2]);
        pArea->dwColor = RGB(r, g, b);

        pArea->nMusic = Str2Int(ParamList[m++]);

        // environment color
        n = Util_ResolveTextLine(ParamList[m++].c_str(), strList, 3, ','); 
        BYTE br = Str2Int(strList[0]);
        BYTE bg = Str2Int(strList[1]);
        BYTE bb = Str2Int(strList[2]);
        pArea->dwEnvColor = (DWORD)(((BYTE)0xff << 24) | ((BYTE)br << 16) | ((BYTE)bg << 8) | (BYTE)bb);

        // light color
        n = Util_ResolveTextLine(ParamList[m++].c_str(), strList, 3, ','); 
        br = Str2Int(strList[0]);
        bg = Str2Int(strList[1]);
        bb = Str2Int(strList[2]);
        pArea->dwLightColor = (DWORD)(((BYTE)0xff << 24) | ((BYTE)br << 16) | ((BYTE)bg << 8) | (BYTE)bb);
 
        // light direction
        n = Util_ResolveTextLine(ParamList[m++].c_str(), strList, 3, ','); 
        pArea->fLightDir[0] = (float)atof(strList[0].c_str());
        pArea->fLightDir[1] = (float)atof(strList[1].c_str());
        pArea->fLightDir[2] = (float)atof(strList[2].c_str());

		pArea->chType = Str2Int(ParamList[m++]);
        return TRUE;
    }
};

inline CAreaInfo* GetAreaInfo(int nAreaID)
{
	return (CAreaInfo*)CAreaSet::I()->GetRawDataInfo(nAreaID);
}


