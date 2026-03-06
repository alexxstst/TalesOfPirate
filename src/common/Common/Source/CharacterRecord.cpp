//======================================================================================================================
// FileName: CharacterRecord.cpp
// Creater: ZhangXuedong
// Date: 2004.09.01
// Comment: CChaRecordSet class
//======================================================================================================================

#include "CharacterRecord.h"
#include <memory.h>
using namespace std;
//---------------------------------------------------------------------------
// class CChaRecord
//---------------------------------------------------------------------------
void CChaRecord::RefreshPrivateData()
{
	_HaveEffectFog = false;
	for( int i=0; i<defCHA_HP_EFFECT_NUM; i++ )
	{
		if( _nHPEffect[i] != 0 ) 
		{
			_HaveEffectFog = true;
			break;
		}
	}
}

//---------------------------------------------------------------------------
// class CChaRecordSet
//---------------------------------------------------------------------------
CChaRecordSet * CChaRecordSet::_Instance = NULL;

BOOL CChaRecordSet::_ReadRawDataInfo(CRawDataInfo *pRawDataInfo, vector<string> &ParamList)
{T_B
	if(ParamList.size()==0) return FALSE;

	CChaRecord *pInfo = (CChaRecord*)pRawDataInfo;

	pInfo->lID = pInfo->nID;
	_tcsncpy(pInfo->szName, pInfo->szDataName, defCHA_NAME_LEN);
	pInfo->szName[defCHA_NAME_LEN - 1] = _TEXT('\0');

	int m = 0, n = 0;
    string strList[80];
	string strLine;

	// 
	_tcsncpy(pInfo->szIconName, ParamList[m++].c_str(), defCHA_ICON_NAME_LEN);
	pInfo->szIconName[defCHA_ICON_NAME_LEN - 1] = _TEXT('\0');
	// 
	pInfo->chModalType = Str2Int(ParamList[m++]);
	// 
	pInfo->chCtrlType = Str2Int(ParamList[m++]);
	// 
	pInfo->sModel = Str2Int(ParamList[m++]);
	// 
	pInfo->sSuitID = Str2Int(ParamList[m++]);
	// 
	pInfo->sSuitNum = Str2Int(ParamList[m++]);
	// 
	memset(pInfo->sSkinInfo, cchChaRecordKeyValue, sizeof(pInfo->sSkinInfo));
	for (int i = 0; i < defCHA_SKIN_NUM; i++)
	{
		pInfo->sSkinInfo[i] = Str2Int(ParamList[m++]);
	}
	//// FeffID
	//pInfo->sFeffID = Str2Int(ParamList[m++]);
	// //EeffID
	//pInfo->sEeffID = Str2Int(ParamList[m++]);
	string lstr[4]; 
	strLine = ParamList[m++];

	n = Util_ResolveTextLine(strLine.c_str(), lstr, 4, ',');
	memset(pInfo->sFeffID,0 ,sizeof(pInfo->sFeffID));
	for (int e = 0; e < n; e++)
	{
		pInfo->sFeffID[e] = Str2Int(lstr[e]);
	}
	pInfo->sEeffID = n;
	m++;

	// 
	memset(pInfo->sEffectActionID, 0, sizeof(pInfo->sEffectActionID));
	strLine = ParamList[m++];
	n = Util_ResolveTextLine( strLine.c_str(), strList, 80, ',' );
	int nCount = 
	n = n > 3 ? 3 : n;
	for (int i = 0; i < n; i++)
	{
		pInfo->sEffectActionID[i] = Str2Int(strList[i]);
	}
	// 
	pInfo->sShadow = Str2Int(ParamList[m++]);
    // 
    pInfo->sActionID = Str2Int(ParamList[m++]);    
    // 
    pInfo->chDiaphaneity = Str2Int(ParamList[m++]);
	// 
	pInfo->sFootfall = Str2Int(ParamList[m++]);
	// 
	pInfo->sWhoop = Str2Int(ParamList[m++]);
	// 
	pInfo->sDirge = Str2Int(ParamList[m++]);
	// 
	pInfo->chControlAble = Str2Int(ParamList[m++]);
	//// 
	//pInfo->chMoveAble = Str2Int(ParamList[m++]);
	// 
	pInfo->chTerritory = Str2Int(ParamList[m++]);
	// 
	pInfo->sSeaHeight = Str2Int(ParamList[m++]);	
	// 
	memset(pInfo->sItemType, cchChaRecordKeyValue, sizeof(pInfo->sItemType));
	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ',');
	n = n > defCHA_ITEM_KIND_NUM ? defCHA_ITEM_KIND_NUM : n;
	for (int i = 0; i < n; i++)
	{
		pInfo->sItemType[i] = Str2Int(strList[i]);
	}
	// 
	pInfo->fLengh = Str2Float(ParamList[m++]);
	// 
	pInfo->fWidth = Str2Float(ParamList[m++]);
	// 
	pInfo->fHeight = Str2Float(ParamList[m++]);
	// 
	pInfo->sRadii = Str2Int(ParamList[m++]);

	// 
	memset( pInfo->nBirthBehave, 0, sizeof(pInfo->nBirthBehave) );
	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ',');
	n = n > defCHA_BIRTH_EFFECT_NUM ? defCHA_BIRTH_EFFECT_NUM : n;	
	for (int i = 0; i < n; i++)
	{		
		pInfo->nBirthBehave[i] = Str2Int(strList[i]);
	}

	// 
	memset( pInfo->nDiedBehave, 0, sizeof(pInfo->nDiedBehave) );
	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ',');
	n = n > defCHA_DIE_EFFECT_NUM ? defCHA_DIE_EFFECT_NUM : n;	
	for (int i = 0; i < n; i++)
	{		
		pInfo->nDiedBehave[i] = Str2Int(strList[i]);
	}

    // 
    pInfo->sBornEff = Str2Int(ParamList[m++]);

    // 
    pInfo->sDieEff = Str2Int(ParamList[m++]);

	// 
    pInfo->sDormancy = Str2Int(ParamList[m++]);

    // 
    pInfo->chDieAction = Str2Int(ParamList[m++]);

	// hp
	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ',');
	n = n > defCHA_HP_EFFECT_NUM ? defCHA_HP_EFFECT_NUM : n;
	memset( pInfo->_nHPEffect, 0, sizeof(pInfo->_nHPEffect) );
	for (int i = 0; i < n; i++)
	{
		pInfo->_nHPEffect[i] = Str2Int(strList[i]);
	}

	// 
	pInfo->_IsFace = Str2Int(ParamList[m++]) != 0 ? true : false;

	// 
	pInfo->_IsCyclone = Str2Int(ParamList[m++]) != 0 ? true : false;

	// ID
	pInfo->lScript = Str2Int(ParamList[m++]);
	// ID
	pInfo->lWeapon = Str2Int(ParamList[m++]);

	// 
	memset(pInfo->lSkill, cchChaRecordKeyValue, sizeof(pInfo->lSkill));
	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ',');
	n = n > defCHA_INIT_SKILL_NUM ? defCHA_INIT_SKILL_NUM : n;
	for (int i = 0; i < n; i++)
	{
		pInfo->lSkill[i][0] = Str2Int(strList[i]);
	}

	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ',');
	n = n > defCHA_INIT_SKILL_NUM ? defCHA_INIT_SKILL_NUM : n;
	for (int i = 0; i < n; i++)
	{
		pInfo->lSkill[i][1] = Str2Int(strList[i]);
	}

	// 
	for (int i = 0; i < defCHA_INIT_ITEM_NUM; i++)
	{
		pInfo->lItem[i][0] = cchChaRecordKeyValue;
		pInfo->lItem[i][1] = cchChaRecordKeyValue;
	}
	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 160, ',');
	n = n > defCHA_INIT_ITEM_NUM ? defCHA_INIT_ITEM_NUM : n;
	for (int i = 0; i < n; i++)
	{
		pInfo->lItem[i][0] = Str2Int(strList[i]);
	}

	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 160, ',');
	n = n > defCHA_INIT_ITEM_NUM ? defCHA_INIT_ITEM_NUM : n;
	for (int i = 0; i < n; i++)
	{
		pInfo->lItem[i][1] = Str2Int(strList[i]);
	}

	// 
	pInfo->lMaxShowItem = Str2Int(ParamList[m++]);
	// 
	pInfo->fAllShow = Str2Float(ParamList[m++]);
	// 
	pInfo->lPrefix = Str2Int(ParamList[m++]);

	// 
	for (int i = 0; i < defCHA_INIT_ITEM_NUM; i++)
	{
		pInfo->lTaskItem[i][0] = cchChaRecordKeyValue;
		pInfo->lTaskItem[i][1] = cchChaRecordKeyValue;
	}
	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 160, ',');
	n = n > defCHA_INIT_ITEM_NUM ? defCHA_INIT_ITEM_NUM : n;
	for (int i = 0; i < n; i++)
	{
		pInfo->lTaskItem[i][0] = Str2Int(strList[i]);
	}

	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 160, ',');
	n = n > defCHA_INIT_ITEM_NUM ? defCHA_INIT_ITEM_NUM : n;
	for (int i = 0; i < n; i++)
	{
		pInfo->lTaskItem[i][1] = Str2Int(strList[i]);
	}

	// AI
	pInfo->lAiNo = Str2Int(ParamList[m++]);
	// 
	pInfo->chCanTurn = Str2Int(ParamList[m++]);
	// 
	pInfo->lVision = Str2Int(ParamList[m++]);
	// 
	pInfo->lNoise = Str2Int(ParamList[m++]);
	// EXP
	pInfo->lGetEXP = Str2Int(ParamList[m++]);
	// 
	pInfo->bLight = Str2Int(ParamList[m++]) ? true : false;

	// exp
	pInfo->lMobexp = Str2Int(ParamList[m++]);
	// 
	pInfo->lLv = Str2Int(ParamList[m++]);
	// HP
	pInfo->lMxHp = Str2Int(ParamList[m++]);
	// HP
	pInfo->lHp = Str2Int(ParamList[m++]);
	// SP
	pInfo->lMxSp = Str2Int(ParamList[m++]);
	// SP
	pInfo->lSp = Str2Int(ParamList[m++]);
	// 
	pInfo->lMnAtk = Str2Int(ParamList[m++]);
	// 
	pInfo->lMxAtk = Str2Int(ParamList[m++]);
	// 
	pInfo->lPDef = Str2Int(ParamList[m++]);
	// 
	pInfo->lDef = Str2Int(ParamList[m++]);
	// 
	pInfo->lHit = Str2Int(ParamList[m++]);
	// 
	pInfo->lFlee = Str2Int(ParamList[m++]);
	// 
	pInfo->lCrt = Str2Int(ParamList[m++]);
	// 
	pInfo->lMf = Str2Int(ParamList[m++]);
	// hp
	pInfo->lHRec = Str2Int(ParamList[m++]);
	// sp
	pInfo->lSRec = Str2Int(ParamList[m++]);
	// 
	pInfo->lASpd = Str2Int(ParamList[m++]);
	// 
	pInfo->lADis = Str2Int(ParamList[m++]);
	// 
	pInfo->lCDis = Str2Int(ParamList[m++]);
	// 
	pInfo->lMSpd = Str2Int(ParamList[m++]);
	// 
	pInfo->lCol = Str2Int(ParamList[m++]);
	// 
	pInfo->lStr = Str2Int(ParamList[m++]);
	// 
	pInfo->lAgi = Str2Int(ParamList[m++]);
	// 
	pInfo->lDex = Str2Int(ParamList[m++]);
	// 
	pInfo->lCon = Str2Int(ParamList[m++]);
	// 
	pInfo->lSta = Str2Int(ParamList[m++]);
	// 
	pInfo->lLuk = Str2Int(ParamList[m++]);
	// 
	pInfo->lLHandVal = Str2Int(ParamList[m++]);

	// 
	_tcsncpy(pInfo->szGuild, ParamList[m++].c_str(), defCHA_GUILD_NAME_LEN);
	pInfo->szGuild[defCHA_GUILD_NAME_LEN - 1] = _TEXT('\0');
	// 
	_tcsncpy(pInfo->szTitle, ParamList[m++].c_str(), defCHA_TITLE_NAME_LEN);
	pInfo->szGuild[defCHA_GUILD_NAME_LEN - 1] = _TEXT('\0');
	// 
	_tcsncpy(pInfo->szJob, ParamList[m++].c_str(), defCHA_JOB_NAME_LEN);
	pInfo->szGuild[defCHA_GUILD_NAME_LEN - 1] = _TEXT('\0');

	// 
	pInfo->lCExp = Str2Int(ParamList[m++].c_str());
	// 
	pInfo->lNExp = Str2Int(ParamList[m++].c_str());
	// 
	pInfo->lFame = Str2Int(ParamList[m++]);
	// 
	pInfo->lAp = Str2Int(ParamList[m++]);
	// 
	pInfo->lTp = Str2Int(ParamList[m++]);
	// 
	pInfo->lGd = Str2Int(ParamList[m++]);
	// 
	pInfo->lSpri = Str2Int(ParamList[m++]);
	// ()
	pInfo->lStor = Str2Int(ParamList[m++]);
	// 
	pInfo->lMxSail = Str2Int(ParamList[m++]);
	// 
	pInfo->lSail = Str2Int(ParamList[m++]);
	// 
	pInfo->lStasa = Str2Int(ParamList[m++]);
	// 
	pInfo->lScsm = Str2Int(ParamList[m++]);

	// 
	pInfo->lTStr = Str2Int(ParamList[m++]);
	// 
	pInfo->lTAgi = Str2Int(ParamList[m++]);
	// 
	pInfo->lTDex = Str2Int(ParamList[m++]);
	// 
	pInfo->lTCon = Str2Int(ParamList[m++]);
	// 
	pInfo->lTSta = Str2Int(ParamList[m++]);
	// 
	pInfo->lTLuk = Str2Int(ParamList[m++]);
	// 
	pInfo->lTMxHp = Str2Int(ParamList[m++]);
	// 
	pInfo->lTMxSp = Str2Int(ParamList[m++]);
	// 
	pInfo->lTAtk = Str2Int(ParamList[m++]);
	// 
	pInfo->lTDef = Str2Int(ParamList[m++]);
	// 
	pInfo->lTHit = Str2Int(ParamList[m++]);
	// 
	pInfo->lTFlee = Str2Int(ParamList[m++]);
	// 
	pInfo->lTMf = Str2Int(ParamList[m++]);
	// 
	pInfo->lTCrt = Str2Int(ParamList[m++]);
	// hp
	pInfo->lTHRec = Str2Int(ParamList[m++]);
	// sp
	pInfo->lTSRec = Str2Int(ParamList[m++]);
	// 
	pInfo->lTASpd = Str2Int(ParamList[m++]);
	// 
	pInfo->lTADis = Str2Int(ParamList[m++]);
	// 
	pInfo->lTSpd = Str2Int(ParamList[m++]);
	// 
	pInfo->lTSpri = Str2Int(ParamList[m++]);
	// 
	pInfo->lTScsm = Str2Int(ParamList[m++]);

	// 
	memset(pInfo->scaling, 0, sizeof(pInfo->scaling));
	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ',');
	n = n > 3 ? 3 : n;
	for (int i = 0; i < n; i++)
	{
		pInfo->scaling[i] = Str2Float(strList[i]);
	}

	return TRUE;
T_E}

void CChaRecordSet::_ProcessRawDataInfo(CRawDataInfo *pInfo)
{
	CChaRecord *pChaInfo = (CChaRecord*)pInfo;

    // 
	pChaInfo->RefreshPrivateData();	
}
