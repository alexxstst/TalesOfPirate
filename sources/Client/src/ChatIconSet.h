#pragma  once

#include "TableData.h"

class CChatIconInfo : public CRawDataInfo
{
public:
    CChatIconInfo()
    {
        memset( szSmall, 0, sizeof(szSmall) );
        memset( szSmallOff, 0, sizeof(szSmallOff) );
        memset( szBig, 0, sizeof(szBig) );
        memset( szHint, 0, sizeof(szHint) );
    }

    char	szSmall[16];
	int		nSmallX;
	int		nSmallY;

    char	szSmallOff[16];		// 
	int		nSmallOffX;
	int		nSmallOffY;

	char	szBig[16];
	int		nBigX;
	int		nBigY;

	char	szHint[32];

};


class CChatIconSet : public CRawDataSet
{
public:
    static CChatIconSet* I() { return _Instance; }

    CChatIconSet(int nIDStart, int nIDCnt)
        :CRawDataSet(nIDStart, nIDCnt)
    {
        _Instance = this;
        _Init();
    }

protected:
    static CChatIconSet* _Instance; // , 

    virtual CRawDataInfo* _CreateRawDataArray(int nCnt)
    {
        return new CChatIconInfo[nCnt];
    }

    virtual void _DeleteRawDataArray()
    {
        delete[] (CChatIconInfo*)_RawDataArray;
    }

    virtual int _GetRawDataInfoSize()
    {
        return sizeof(CChatIconInfo);
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
        // if(ParamList.size()==0) return FALSE;

        CChatIconInfo *pInfo = (CChatIconInfo*)pRawDataInfo;

        strncpy(pInfo->szSmall, pInfo->szDataName, sizeof(pInfo->szSmall));

		int m = 0;
		pInfo->nSmallX = Str2Int(ParamList[m++]);
		pInfo->nSmallY = Str2Int(ParamList[m++]);

		strncpy(pInfo->szSmallOff, ParamList[m++].c_str(), sizeof(pInfo->szSmallOff));

		pInfo->nSmallOffX = Str2Int(ParamList[m++]);
		pInfo->nSmallOffY = Str2Int(ParamList[m++]);

		strncpy(pInfo->szBig, ParamList[m++].c_str(), sizeof(pInfo->szBig));

		pInfo->nBigX = Str2Int(ParamList[m++]);
		pInfo->nBigY = Str2Int(ParamList[m++]);
		
 		strncpy(pInfo->szHint, ParamList[m++].c_str(), sizeof(pInfo->szHint));

	   return TRUE;
    }
};

inline CChatIconInfo* GetChatIconInfo(int nIconID)
{
    return (CChatIconInfo*)CChatIconSet::I()->GetRawDataInfo(nIconID);
}
