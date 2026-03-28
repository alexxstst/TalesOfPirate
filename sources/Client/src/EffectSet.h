#pragma once

#include "TableData.h"

//class CEffectInfo : public CRawDataInfo
//{
//
//public:
//
//	CEffectInfo()
//	{
//	   strcpy(szName, "������Ч");
//	   nPhotoTexID = 0;
//	   nHeightOff  = 0;
//	   nEffType	= 0;
//	   nScaleX	= 100;
//	   nScaleY	= 100;
//	   nScaleZ	= 100;
//    }
//	char	szName[16];
//	int		nPhotoTexID;
//	int		nHeightOff;
//
//	int		nEffType;
//	int     nScaleX;
//    int     nScaleY;
//    int     nScaleZ;
//};
//
//
//class CEffectSet : public CRawDataSet
//{
//
//public:
//	
//	static CEffectSet* I() { return _Instance; }
//	
//	CEffectSet(int nIDStart, int nIDCnt)
//	:CRawDataSet(nIDStart, nIDCnt)
//	{
//	    _Instance = this;
//		_Init();
//	}
//
//protected:
//
//	static CEffectSet* _Instance; // �൱�ڵ���, ���Լ���ס
//   
//	virtual CRawDataInfo* _CreateRawDataArray(int nCnt)
//	{
//		return new CEffectInfo[nCnt];
//	}
//	
//	virtual void _DeleteRawDataArray()
//	{
//		delete[] (CEffectInfo*)_RawDataArray;
//	}
//	
//	virtual int _GetRawDataInfoSize()
//	{
//		return sizeof(CEffectInfo);
//	}
//
//	virtual void*	_CreateNewRawData(CRawDataInfo *pInfo)
//	{
//		return NULL;
//	}
//
//	virtual void  _DeleteRawData(CRawDataInfo *pInfo)
//	{
//		SAFE_DELETE(pInfo->pData);
//	}
//	
//	virtual BOOL _ReadRawDataInfo(CRawDataInfo *pRawDataInfo, vector<string> &ParamList)
//	{   
//        if(ParamList.size()==0) return FALSE;
//		
//        CEffectInfo *pInfo = (CEffectInfo*)pRawDataInfo;
//		
//		strcpy(pInfo->szName, ParamList[0].c_str());
//
//		pInfo->nHeightOff = Str2Int(ParamList[1].c_str());
//		
//		pInfo->nPhotoTexID = GetTextureID(szPhoto);
//
//        pInfo->nEffType	= Str2Int(ParamList[2].c_str());
//		pInfo->nScaleX =  Str2Int(ParamList[3].c_str());
//		pInfo->nScaleY =  Str2Int(ParamList[4].c_str());
//		pInfo->nScaleZ =  Str2Int(ParamList[5].c_str());
//
//		LG("effect", "Read Effect Info Name = [%s]\n", pInfo->szName);
//        return TRUE;
//    }
//};
//
//inline CEffectInfo* GetEffectInfo(int nTypeID)
//{
//	return (CEffectInfo*)CEffectSet::I()->GetRawDataInfo(nTypeID);
//}

//////////////////////////////////////////////////////////////////////////

#define 	CEffectInfo	CMagicInfo
#define 	CEffectSet	CMagicSet
#define 	GetEffectInfo	GetMagicInfo

class CMagicInfo : public CRawDataInfo
{

public:

	CMagicInfo()
	{
		strcpy(szName, g_oLangRec.GetString(59));
		strcpy(szPhotoName, g_oLangRec.GetString(59));

		nPhotoTexID = 0;
		nEffType	= 0;
		nObjType = 0;
		memset(nDummy,-1,sizeof(nDummy));
		nDummyNum = 0;
		//nDummy = 0;
		nDummy2 = 0;
		nHeightOff  = 0;
		fPlayTime = 0;
		LightID = 0;

		fBaseSize = 0;
	}
	char	szName[16];
	char	szPhotoName[16];
	int		nPhotoTexID;

	int		nEffType;
	int		nObjType;

	int		nDummyNum;
	int		nDummy[8];
	int		nDummy2;

	int		nHeightOff;

	float   fPlayTime;

	int		LightID;

	float   fBaseSize;
};


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

	static CMagicSet* _Instance; // �൱�ڵ���, ���Լ���ס

	virtual CRawDataInfo* _CreateRawDataArray(int nCnt)
	{
		return new CMagicInfo[nCnt];
	}

	virtual void _DeleteRawDataArray()
	{
		delete[] (CMagicInfo*)_RawDataArray;
	}

	virtual int _GetRawDataInfoSize()
	{
		return sizeof(CMagicInfo);
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
		CMagicInfo *pInfo = (CMagicInfo*)pRawDataInfo;

		strncpy(pInfo->szName, ParamList[0].c_str(), sizeof(pInfo->szName));
		strncpy(pInfo->szPhotoName, ParamList[1].c_str(), sizeof(pInfo->szPhotoName));

		pInfo->nEffType	= Str2Int(ParamList[2].c_str());
		pInfo->nObjType	= Str2Int(ParamList[3].c_str());
		//pInfo->nDummy	= Str2Int(ParamList[3].c_str());
		std::string lstr1[8];
		pInfo->nDummyNum = Util_ResolveTextLine(ParamList[4].c_str(), lstr1, 8, ',');
		for (int n = 0; n < pInfo->nDummyNum; n++)
		{
			pInfo->nDummy[n] = Str2Int(lstr1[n].c_str());
			if(pInfo->nDummyNum == 1)
			{
				if (pInfo->nDummy[n] < -1)
				{
					pInfo->nDummyNum = 0;
				}
			}
		}

		pInfo->nDummy2	= Str2Int(ParamList[5].c_str());
		pInfo->nHeightOff = Str2Int(ParamList[6].c_str());
		pInfo->fPlayTime = Str2Float(ParamList[7].c_str());
		pInfo->LightID = Str2Int(ParamList[8].c_str());
		pInfo->fBaseSize = Str2Float(ParamList[9].c_str());

		ToLogService("common", "Read Magic Info Name = [{}]", pInfo->szName);
		return TRUE;
	}

	virtual void _ProcessRawDataInfo(CRawDataInfo *pRawDataInfo)
	{
		CMagicInfo *pInfo = (CMagicInfo*)pRawDataInfo;
		char szPhoto[72]; sprintf(szPhoto, "texture/photo/%s.bmp", pInfo->szPhotoName);
		pInfo->nPhotoTexID = GetTextureID(szPhoto);
	}
	

};

inline CMagicInfo* GetMagicInfo(int nTypeID)
{
	return (CMagicInfo*)CMagicSet::I()->GetRawDataInfo(nTypeID);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
class CShadeInfo : public CRawDataInfo
{

public:

	CShadeInfo()
	{
		strcpy(szName, g_oLangRec.GetString(60));
		nPhotoTexID = 0;

		fsize = 0;
		nAni = 0;
		nRow = 0;
		nCol  = 0;
		nUseAlphaTest = 0;//�Ƿ�ʹ��͸��ɫ
		nAlphaType = 0;//alpha  ����

		nColorR = 0;//��ɫ
		nColorG = 0;
		nColorB = 0;
		nColorA = 0;

		nType = 0;
	}
	char	szName[16];
	int		nPhotoTexID;

	float	fsize;
	int		nAni;
	int		nRow;
	int		nCol;

	int		nUseAlphaTest;//�Ƿ�ʹ��͸��ɫ
	int		nAlphaType;//alpha  ����

	int		nColorR;//��ɫ
	int		nColorG;
	int		nColorB;
	int		nColorA;

	int		nType;//0�Ǹ����ɫ�ģ�1�Ǹ�����Ч�ģ�
};


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

	static CShadeSet* _Instance; // �൱�ڵ���, ���Լ���ס

	virtual CRawDataInfo* _CreateRawDataArray(int nCnt)
	{
		return new CShadeInfo[nCnt];
	}

	virtual void _DeleteRawDataArray()
	{
		delete[] (CShadeInfo*)_RawDataArray;
	}

	virtual int _GetRawDataInfoSize()
	{
		return sizeof(CShadeInfo);
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

		CShadeInfo *pInfo = (CShadeInfo*)pRawDataInfo;

		strncpy(pInfo->szName, ParamList[0].c_str(), sizeof(pInfo->szName));


		//pInfo->nPhotoTexID = GetTextureID(szPhoto);

		pInfo->fsize	= Str2Float(ParamList[1].c_str());
		pInfo->nAni	    = Str2Int(ParamList[2].c_str());
		pInfo->nRow   	= Str2Int(ParamList[3].c_str());
		pInfo->nCol     = Str2Int(ParamList[4].c_str());

		pInfo->nUseAlphaTest = Str2Int(ParamList[5].c_str());//�Ƿ�ʹ��͸��ɫ
		pInfo->nAlphaType = Str2Int(ParamList[6].c_str());//alpha  ����

		pInfo->nColorR = Str2Int(ParamList[7].c_str());//��ɫ
		pInfo->nColorG = Str2Int(ParamList[8].c_str());
		pInfo->nColorB = Str2Int(ParamList[9].c_str());
		pInfo->nColorA = Str2Int(ParamList[10].c_str());
		pInfo->nType   = Str2Int(ParamList[11].c_str());


		ToLogService("common", "Read shade Info Name = [{}]", pInfo->szName);
		return TRUE;
	}
};

inline CShadeInfo* GetShadeInfo(int nTypeID)
{
	return (CShadeInfo*)CShadeSet::I()->GetRawDataInfo(nTypeID);
}

