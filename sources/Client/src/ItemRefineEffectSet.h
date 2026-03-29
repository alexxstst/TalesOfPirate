#pragma once
//----------------------------------------------------------------------
// :
// :lh 2005-11-03
// :
//----------------------------------------------------------------------
#include "TableData.h"

#define REFINE_EFFECT_NUM		4
#define REFINE_EFFECT_CHA_NUM	4

class CItemRefineEffectSet;
class CItemRefineEffectInfo : public CRawDataInfo
{
friend class CItemRefineEffectSet;
public:
	CItemRefineEffectInfo()
	{
		memset( _sEffectNum, 0, sizeof(_sEffectNum) );
		memset( sEffectID, 0, sizeof(sEffectID) );
		memset( chDummy, 0, sizeof(chDummy) );
    }

	int		nLightID;													// 
	short	sEffectID[REFINE_EFFECT_CHA_NUM][REFINE_EFFECT_NUM];		// 
	char	chDummy[REFINE_EFFECT_NUM];

	int		GetEffectNum(int nCharID)		{ return _sEffectNum[nCharID];			}

private:
	int		_sEffectNum[REFINE_EFFECT_CHA_NUM];

};

class CItemRefineEffectSet : public CRawDataSet
{
public:
	static CItemRefineEffectSet* I() { return _Instance; }
	
	CItemRefineEffectSet(int nIDStart, int nIDCnt)
	:CRawDataSet(nIDStart, nIDCnt)
	{
	    _Instance = this;
		_Init();
	}

protected:
	static CItemRefineEffectSet* _Instance; // , 
   
	virtual CRawDataInfo* _CreateRawDataArray(int nCnt)
	{
		return new CItemRefineEffectInfo[nCnt];
	}
	
	virtual void _DeleteRawDataArray()
	{
		delete[] (CItemRefineEffectInfo*)_RawDataArray;
	}
	
	virtual int _GetRawDataInfoSize()
	{
		return sizeof(CItemRefineEffectInfo);
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
		
        CItemRefineEffectInfo *pInfo = (CItemRefineEffectInfo*)pRawDataInfo;
		int m=0;
		pInfo->nLightID = Str2Int(ParamList[m++]);
		for( int i=0; i<REFINE_EFFECT_NUM; i++ )
		{
			for( int j=0; j<REFINE_EFFECT_CHA_NUM; j++ )
			{
				pInfo->sEffectID[j][i] = Str2Int(ParamList[m++]);
				if( pInfo->sEffectID[j][i]!=0 ) pInfo->_sEffectNum[j]++;
			}
			pInfo->chDummy[i] = Str2Int(ParamList[m++]);
		}
        return TRUE;
    }
};

inline CItemRefineEffectInfo* GetItemRefineEffectInfo(int nRefineID)
{
	return (CItemRefineEffectInfo*)CItemRefineEffectSet::I()->GetRawDataInfo(nRefineID);
}

