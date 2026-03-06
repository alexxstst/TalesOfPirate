#pragma once

#include "TableData.h"

#define TERRAIN_TYPE_NORMAL			0
#define TERRAIN_TYPE_UNDERWATER		1
#define MAX_WATER_LOOP				30

#pragma warning(disable: 4275)

class MINDPOWER_API MPTerrainInfo : public CRawDataInfo
{
public:
	
	MPTerrainInfo()
	{
		btType		= TERRAIN_TYPE_NORMAL;
		nTextureID	= 0;
		btAttr = 0;
	}

public:	

	BYTE    btType;
	int		nTextureID;
	BYTE	btAttr;
};


class MINDPOWER_API MPTerrainSet : public CRawDataSet
{
public:
	
	static MPTerrainSet* I() { return _Instance; }
	~MPTerrainSet();

    MPTerrainSet(int nIDStart, int nIDCnt)
	:CRawDataSet(nIDStart, nIDCnt)
	{
	    _Init();
        _Instance = this;

		int i;
		char szName[64];
		for(i = 0; i < MAX_WATER_LOOP; i++)
		{
			//sprintf(szName, "texture/terrain/water/waterbump%04d.bmp", i);
			sprintf(szName, "texture/terrain/water/ocean_h.%02d.bmp", i + 1);
			_nWaterBumpTexture[i] = GetTextureID(szName);
		}

		for(i = 1; i < 15; i++)
		{
			sprintf(szName, "texture/terrain/alpha/%d.tga", i);
			_nAlphaTexture[i] = GetTextureID(szName);
		}

        m_nAlphaTextureI = GetTextureID("texture/terrain/alpha/total.tga");
	}

	int GetWaterBumpTextureID(int nFrame)
	{
		return _nWaterBumpTexture[nFrame];
	}

	int GetAlphaTextureID(int nAlphaNo)
	{
		return _nAlphaTexture[nAlphaNo];
	}

    int     m_nAlphaTextureI;

    int GetIDStart() { return _nIDStart; }
    int GetIDCnt() { return _nIDCnt; }


protected:

	static MPTerrainSet* _Instance; // , 
    
    virtual CRawDataInfo* _CreateRawDataArray(int nIDCnt)
	{
		return new MPTerrainInfo[nIDCnt];
	}
	
	virtual void _DeleteRawDataArray()
	{
		delete[] (MPTerrainInfo*)_RawDataArray;
	}
	
	virtual int _GetRawDataInfoSize()
	{
		return sizeof(MPTerrainInfo);
	}

	
    virtual void* _CreateNewRawData(CRawDataInfo *pInfo)
	{
        MPTerrainInfo *pTerrainInfo = (MPTerrainInfo*)pInfo;
	    return NULL; // pTerrainInfo;
	}

	virtual void  _DeleteRawData(CRawDataInfo *pInfo)
	{
	}
	
	virtual BOOL _ReadRawDataInfo(CRawDataInfo *pRawDataInfo, std::vector<std::string> &ParamList)
	{
        if(ParamList.size()==0) return FALSE;
		
		MPTerrainInfo *pTerrainInfo = (MPTerrainInfo*)pRawDataInfo;
		
		std::string &str = ParamList.front();

		pTerrainInfo->btType = (BYTE)(Str2Int(str));
		str = ParamList[1];
		pTerrainInfo->btAttr = (BYTE)(Str2Int(str));

	    return TRUE;
    }

	virtual void _ProcessRawDataInfo(CRawDataInfo *pRawDataInfo)
	{
		MPTerrainInfo *pTerrainInfo = (MPTerrainInfo*)pRawDataInfo;
		pTerrainInfo->nTextureID = GetTextureID(pTerrainInfo->szDataName);
	
		LG("terrain", "Load Terrain [%s], ID = %d, Type = %d, Texture ID = %d\n", pTerrainInfo->szDataName, pTerrainInfo->nID, pTerrainInfo->btType, pTerrainInfo->nTextureID);
    }

protected:
	
	int		_nWaterBumpTexture[MAX_WATER_LOOP];
	int		_nAlphaTexture[40];
};

//--------------
// :
//--------------
inline MPTerrainInfo* GetTerrainInfo(int nID) // IDTerrainInfo
{
    return (MPTerrainInfo*)MPTerrainSet::I()->GetRawDataInfo(nID);
}

inline int GetTerrainTextureID(int nID)
{
	MPTerrainInfo *pInfo = (MPTerrainInfo*)MPTerrainSet::I()->GetRawDataInfo(nID);	
	if(!pInfo) 
	{
		return 0;
	}
	return pInfo->nTextureID;
}

inline int GetWaterBumpTextureID(int nFrame)
{
	return MPTerrainSet::I()->GetWaterBumpTextureID(nFrame);
}

inline int GetTerrainAlphaTextureID(int nAlphaNo)
{
	return MPTerrainSet::I()->GetAlphaTextureID(nAlphaNo);
}

inline int GetTerrainAlphaTextureID_I() { return MPTerrainSet::I()->m_nAlphaTextureI; }

#pragma warning(default: 4275)
