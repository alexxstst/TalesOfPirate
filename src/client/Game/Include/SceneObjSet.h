#pragma once

#include "TableData.h"


class CSceneObjInfo : public CRawDataInfo
{

public:

	CSceneObjInfo()
	{
	   strcpy(szName, g_oLangRec.GetString(351));
	   nPhotoTexID          = 0;
	   nAttachEffectID      = 0;
       bEnablePointLight    = FALSE;
	   nStyle               = 0;
       nType                = 0;
	   nFlag                = 0;
       nSizeFlag            = 0;
	   nEnvSoundDis			= 0;
    }
	
    char	szName[16];
	
    int     nType;              // 
   	BYTE    btPointColor[3];    // 
    BYTE    btEnvColor[3];      // 
    BYTE    btFogColor[3];      // 
    int     nRange;
    float   Attenuation1;
    int     nAnimCtrlID;        //id
    // .... 
    

    int     nStyle;             //  
    int		nAttachEffectID;
    BOOL    bEnablePointLight;  //  
	BOOL    bEnableEnvLight;    //      
    int		nFlag;              //  
    int     nSizeFlag;          //  , , 

    char    szEnvSound[11];	
	int		nEnvSoundDis;		// :
	int		nPhotoTexID;        //  ID
    BOOL    bShadeFlag;
	BOOL	bIsReallyBig;		// ,Added by clp

    int     nFadeObjNum;
    int     nFadeObjSeq[16];
    float   fFadeCoefficent;
};


class CSceneObjSet : public CRawDataSet
{

public:
	
	static CSceneObjSet* I() { return _Instance; }
	
	CSceneObjSet(int nIDStart, int nIDCnt)
	:CRawDataSet(nIDStart, nIDCnt)
	{
	    _Instance = this;
		_Init();
	}

protected:

	static CSceneObjSet* _Instance; // , 
   
	virtual CRawDataInfo* _CreateRawDataArray(int nCnt)
	{
		return new CSceneObjInfo[nCnt];
	}
	
	virtual void _DeleteRawDataArray()
	{
		delete[] (CSceneObjInfo*)_RawDataArray;
	}
	
	virtual int _GetRawDataInfoSize()
	{
		return sizeof(CSceneObjInfo);
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
		
        CSceneObjInfo *pInfo = (CSceneObjInfo*)pRawDataInfo;
		
		strncpy(pInfo->szName, ParamList[0].c_str(), sizeof(pInfo->szName));

        //  0  1  2 ....
        pInfo->nType             = Str2Int(ParamList[1]);
        
        // 1 = ParamList[2];
        std::string strList1[16], strList2[16];
        int n1 = Util_ResolveTextLine(ParamList[2].c_str(), strList1, 16+1, ',');
        int n2 = Util_ResolveTextLine(ParamList[3].c_str(), strList2, 16+1, ',');

		int i;
        switch(pInfo->nType)
        {
        case 3: // point light
            pInfo->btPointColor[0] = Str2Int(strList1[0]);
            pInfo->btPointColor[1] = Str2Int(strList1[1]);
            pInfo->btPointColor[2] = Str2Int(strList1[2]);
            pInfo->nRange = Str2Int(strList2[0]);
            pInfo->Attenuation1 = Str2Float(strList2[1]);
            pInfo->nAnimCtrlID = Str2Int(strList2[2]);
            break;
        case 4: // ambient light
            pInfo->btEnvColor[0] = Str2Int(strList1[0]);
            pInfo->btEnvColor[1] = Str2Int(strList1[1]);
            pInfo->btEnvColor[2] = Str2Int(strList1[2]);
            break;
        case 6: // 
			strcpy(pInfo->szEnvSound, strList1[0].c_str());
			pInfo->nEnvSoundDis = Str2Int( strList2[0] );
            LG("sceneobj", "Read Enviroment Sound [%s]\n", pInfo->szEnvSound);
            break;
        case 0:
            {
                pInfo->nFadeObjNum = Str2Int(strList1[0]);
                for( i = 0; i < pInfo->nFadeObjNum; i++)
                {
                    pInfo->nFadeObjSeq[i] = Str2Int(strList1[i+1]);
                }
                if(pInfo->nFadeObjNum > 0)
                {
                    pInfo->fFadeCoefficent = Str2Float(strList1[i+1]);
                }
            }
            break;
        }

        // 2 = ParamList[3];

	    pInfo->nAttachEffectID   = Str2Int(ParamList[4]);
       	pInfo->bEnableEnvLight   = Str2Int(ParamList[5]);
        pInfo->bEnablePointLight = Str2Int(ParamList[6]); 
        pInfo->nStyle            = Str2Int(ParamList[7]);
	    pInfo->nFlag             = Str2Int(ParamList[8]);
        pInfo->nSizeFlag         = Str2Int(ParamList[9]);
        pInfo->bShadeFlag        = Str2Int(ParamList[10]);
		pInfo->bIsReallyBig		 = Str2Int(ParamList[11]);

	    return TRUE;
    }

	virtual void _ProcessRawDataInfo(CRawDataInfo *pRawDataInfo)
	{
		CSceneObjInfo *pInfo = (CSceneObjInfo*)pRawDataInfo;
		char szPhoto[72]; sprintf(szPhoto, "texture/photo/sceneobj/%s.bmp", pInfo->szName);
		pInfo->nPhotoTexID       = GetTextureID(szPhoto);
		LG("sceneobj", "Read SceneObjInfo Model = [%s], Name = [%s], Type = %d, nStype = %d, nFlag = %d\n", pInfo->szDataName, pInfo->szName, pInfo->nType, pInfo->nStyle, pInfo->nFlag );
    }
};


inline CSceneObjInfo* GetSceneObjInfo(int nTypeID)
{
	return (CSceneObjInfo*)CSceneObjSet::I()->GetRawDataInfo(nTypeID);
}
