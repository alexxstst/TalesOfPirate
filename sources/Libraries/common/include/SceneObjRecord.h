#pragma once

#include "TableData.h"

// Запись таблицы объектов сцены
class CSceneObjInfo : public CRawDataInfo
{
public:
	char    szName[16]{};
	int     nType{0};
	BYTE    btPointColor[3]{};
	BYTE    btEnvColor[3]{};
	BYTE    btFogColor[3]{};
	int     nRange{0};
	float   Attenuation1{0};
	int     nAnimCtrlID{0};

	int     nStyle{0};
	int     nAttachEffectID{0};
	BOOL    bEnablePointLight{FALSE};
	BOOL    bEnableEnvLight{FALSE};
	int     nFlag{0};
	int     nSizeFlag{0};

	char    szEnvSound[11]{};
	int     nEnvSoundDis{0};
	int     nPhotoTexID{0};
	BOOL    bShadeFlag{FALSE};
	BOOL    bIsReallyBig{FALSE};

	int     nFadeObjNum{0};
	int     nFadeObjSeq[16]{};
	float   fFadeCoefficent{0};
};
