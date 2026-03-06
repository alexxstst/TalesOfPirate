#pragma once

#include "TableData.h"
#include <windows.h>
#include <array>


// ?????? ID ???? ?????? 
class CMountInfo : public CRawDataInfo
{
public:
	CMountInfo() = default;

	short mountID{};
	short boneID{};
	std::array<short, 4> height{ /* Lance, Carsise, Phyllis, Ami */};
	short offsetX{};
	short offsetY{};
	std::array<short, 4> poseID{ /* Lance, Carsise, Phyllis, Ami */};
};


class CMountSet : public CRawDataSet
{
public:
	static CMountSet* I() { return _Instance; }

	CMountSet(int nIDStart, int nIDCnt)
		:CRawDataSet(nIDStart, nIDCnt)
	{
		_Instance = this;
		_Init();
	}

protected:

	static CMountSet* _Instance; // ???????, ????????

	virtual CRawDataInfo* _CreateRawDataArray(int nCnt)
	{
		return new CMountInfo[nCnt];
	}

	virtual void _DeleteRawDataArray()
	{
		delete[](CMountInfo*)_RawDataArray;
	}

	virtual int _GetRawDataInfoSize()
	{
		return sizeof(CMountInfo);
	}

	virtual void* _CreateNewRawData(CRawDataInfo* pInfo)
	{
		return NULL;
	}

	virtual void  _DeleteRawData(CRawDataInfo* pInfo)
	{
		SAFE_DELETE(pInfo->pData);
	}

	virtual BOOL _ReadRawDataInfo(CRawDataInfo * pRawDataInfo, std::vector < std::string > & ParamList) 
	{
	    CMountInfo * pMount = (CMountInfo * )(pRawDataInfo);
	    int m = 0;
	    pMount->mountID = Str2Int(ParamList[m++]);
	    pMount->boneID = Str2Int(ParamList[m++]);

	    std::array < std::string, decltype(CMountInfo::height)().size() > split_strings;
	    auto n = Util_ResolveTextLine(ParamList[m++].c_str(), split_strings.data(), split_strings.size(), ',');
	    std::transform(split_strings.begin(), split_strings.begin() + n, pMount->height.begin(),
	        [](const std::string & str) {
	            return Str2Int(str);
	        });

	    pMount->offsetX = Str2Int(ParamList[m++]);
	    pMount->offsetY = Str2Int(ParamList[m++]);

	    n = Util_ResolveTextLine(ParamList[m++].c_str(), split_strings.data(), split_strings.size(), ',');
	    std::transform(split_strings.begin(), split_strings.begin() + n, pMount->poseID.begin(),
	        [](const std::string & str) {
	            return Str2Int(str);
	        });

	    return TRUE;
	}
};

//inline CMountInfo* GetMountInfo(int nMountID)
//{
//	return (CMountInfo*)CMountSet::I()->GetRawDataInfo(nMountID);
//}

inline CMountInfo* GetMountInfo(const char* pszItemName)
{
	return (CMountInfo*)CMountSet::I()->GetRawDataInfo(pszItemName);
}


