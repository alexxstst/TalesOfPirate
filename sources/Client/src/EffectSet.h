#pragma once

#include "EffectRecord.h"
#include "EffectRecordStore.h"
#include "ShadeRecord.h"
#include "ShadeRecordStore.h"
#include "AssetDatabase.h"
#include "TableData.h"

//////////////////////////////////////////////////////////////////////////

class CMagicSet : public CRawDataSet
{
public:
	static CMagicSet* I() { return _Instance; }

	CMagicSet(int nIDStart, int nIDCnt)
		:CRawDataSet(nIDStart, nIDCnt)
	{
		_Instance = this;
		_Init();
	}

protected:
	static CMagicSet* _Instance;

	virtual CRawDataInfo* _CreateRawDataArray(int nCnt) { return new CMagicInfo[nCnt]; }
	virtual void _DeleteRawDataArray() { delete[] (CMagicInfo*)_RawDataArray; }
	virtual int _GetRawDataInfoSize() { return sizeof(CMagicInfo); }
	virtual void* _CreateNewRawData(CRawDataInfo *pInfo) { return NULL; }
	virtual void _DeleteRawData(CRawDataInfo *pInfo) { SAFE_DELETE(pInfo->pData); }

	virtual BOOL _ReadRawDataInfo(CRawDataInfo *pRawDataInfo, std::vector<std::string> &ParamList)
	{
		CMagicInfo *pInfo = (CMagicInfo*)pRawDataInfo;

		strncpy(pInfo->szName, ParamList[0].c_str(), sizeof(pInfo->szName));
		strncpy(pInfo->szPhotoName, ParamList[1].c_str(), sizeof(pInfo->szPhotoName));

		pInfo->nEffType	= Str2Int(ParamList[2].c_str());
		pInfo->nObjType	= Str2Int(ParamList[3].c_str());

		std::string lstr1[8];
		pInfo->nDummyNum = Util_ResolveTextLine(ParamList[4].c_str(), lstr1, 8, ',');
		for (int n = 0; n < pInfo->nDummyNum; n++)
		{
			pInfo->nDummy[n] = Str2Int(lstr1[n].c_str());
			if(pInfo->nDummyNum == 1)
			{
				if (pInfo->nDummy[n] < -1)
					pInfo->nDummyNum = 0;
			}
		}

		pInfo->nDummy2	  = Str2Int(ParamList[5].c_str());
		pInfo->nHeightOff = Str2Int(ParamList[6].c_str());
		pInfo->fPlayTime  = Str2Float(ParamList[7].c_str());
		pInfo->LightID    = Str2Int(ParamList[8].c_str());
		pInfo->fBaseSize  = Str2Float(ParamList[9].c_str());

		return TRUE;
	}

	virtual void _ProcessRawDataInfo(CRawDataInfo *pRawDataInfo)
	{
		CMagicInfo *pInfo = (CMagicInfo*)pRawDataInfo;
		char szPhoto[72]; sprintf(szPhoto, "texture/photo/%s.bmp", pInfo->szPhotoName);
		pInfo->nPhotoTexID = GetTextureID(szPhoto);
		EffectRecordStore::Insert(AssetDatabase::Instance()->GetDb(), *pInfo);
	}
};

//////////////////////////////////////////////////////////////////////////

class CShadeSet : public CRawDataSet
{
public:
	static CShadeSet* I() { return _Instance; }

	CShadeSet(int nIDStart, int nIDCnt)
		:CRawDataSet(nIDStart, nIDCnt)
	{
		_Instance = this;
		_Init();
	}

protected:
	static CShadeSet* _Instance;

	virtual CRawDataInfo* _CreateRawDataArray(int nCnt) { return new CShadeInfo[nCnt]; }
	virtual void _DeleteRawDataArray() { delete[] (CShadeInfo*)_RawDataArray; }
	virtual int _GetRawDataInfoSize() { return sizeof(CShadeInfo); }
	virtual void* _CreateNewRawData(CRawDataInfo *pInfo) { return NULL; }
	virtual void _DeleteRawData(CRawDataInfo *pInfo) { SAFE_DELETE(pInfo->pData); }

	virtual BOOL _ReadRawDataInfo(CRawDataInfo *pRawDataInfo, std::vector<std::string> &ParamList)
	{
		if(ParamList.size()==0) return FALSE;

		CShadeInfo *pInfo = (CShadeInfo*)pRawDataInfo;

		strncpy(pInfo->szName, ParamList[0].c_str(), sizeof(pInfo->szName));

		pInfo->fsize         = Str2Float(ParamList[1].c_str());
		pInfo->nAni          = Str2Int(ParamList[2].c_str());
		pInfo->nRow          = Str2Int(ParamList[3].c_str());
		pInfo->nCol          = Str2Int(ParamList[4].c_str());
		pInfo->nUseAlphaTest = Str2Int(ParamList[5].c_str());
		pInfo->nAlphaType    = Str2Int(ParamList[6].c_str());
		pInfo->nColorR       = Str2Int(ParamList[7].c_str());
		pInfo->nColorG       = Str2Int(ParamList[8].c_str());
		pInfo->nColorB       = Str2Int(ParamList[9].c_str());
		pInfo->nColorA       = Str2Int(ParamList[10].c_str());
		pInfo->nType         = Str2Int(ParamList[11].c_str());

		return TRUE;
	}

	virtual void _ProcessRawDataInfo(CRawDataInfo *pRawDataInfo)
	{
		ShadeRecordStore::Insert(AssetDatabase::Instance()->GetDb(), *(CShadeInfo*)pRawDataInfo);
	}
};
