//======================================================================================================================
// FileName: SkillRecord.cpp
// Creater: ZhangXuedong
// Date: 2004.09.01
// Comment: CSkillRecordSet class
//======================================================================================================================

#include "SkillRecord.h"
using namespace std;
//---------------------------------------------------------------------------
// class CSkillRecord
//---------------------------------------------------------------------------
CSkillRecord::CSkillRecord()	
{ 
	_nUpgrade=rand() % 3;		

    SSkillGridEx& Skill = GetSkillGrid();
	memset( &Skill, 0, sizeof(Skill) );

    Skill.chLv = 1;
    Skill.chState = 1;

	_IsActive = false;

	_dwAttackTime = 0;

	_nPoseNum = 0;
}

void CSkillRecord::Refresh( int nJob )
{
	_nUpgrade = enumSKILL_UPGRADE_NONE;

	if(chPhase == enumSKILL_NOT_MANUAL_ADD)
	{
		// 
		return;
	}

	if( chJobSelect[0][0]==-1 )
	{
		if (_Skill.chLv < chJobSelect[0][1])
		{
			_nUpgrade = enumSKILL_UPGRADE_CAN;
		}
		else
		{
			_nUpgrade = enumSKILL_UPGRADE_MAX;
		}
		return;
	}

	for (char i = 0; i < defSKILL_JOB_SELECT_NUM; i++)
	{
		if (chJobSelect[i][0]==cchSkillRecordKeyValue)
		{
			return;
		}
		else if( (chJobSelect[i][0]==nJob) )
		{
			if (_Skill.chLv < chJobSelect[i][1])
				_nUpgrade = enumSKILL_UPGRADE_CAN;
			else
				_nUpgrade = enumSKILL_UPGRADE_MAX;
			return;
		}
	}
}

void CSkillRecord::RefreshPrivateData()
{
    SSkillGridEx& Skill = GetSkillGrid();

	_eSelectCha = enumSC_NONE;
	switch( chApplyTarget )
	{
	case enumSKILL_TYPE_SELF:		_eSelectCha = enumSC_SELF;	break;
	case enumSKILL_TYPE_TEAM:		_eSelectCha = enumSC_TEAM;	break;
	case enumSKILL_TYPE_PLAYER_DIE: _eSelectCha = enumSC_PLAYER_ASHES;	break;
	case enumSKILL_TYPE_REPAIR:		_eSelectCha = enumSC_MONS_REPAIRABLE;	break;
	case enumSKILL_TYPE_TREE:		_eSelectCha = enumSC_MONS_TREE;	break;
	case enumSKILL_TYPE_MINE:		_eSelectCha = enumSC_MONS_MINE;	break;
	case enumSKILL_TYPE_FISH:		_eSelectCha = enumSC_MONS_FISH;	break;
	case enumSKILL_TYPE_SALVAGE:		_eSelectCha = enumSC_MONS_DBOAT;	break;

	case enumSKILL_TYPE_SCENE:		
	case enumSKILL_TYPE_ALL:		
		_eSelectCha = enumSC_ALL;	
		break;

	case enumSKILL_TYPE_ENEMY:		_eSelectCha = enumSC_ENEMY;	break;
	case enumSKILL_TYPE_TRADE:		_eSelectCha = enumSC_NONE;	break;
	};

	_nPoseNum=0;
	for( int i=0; i<defSKILL_POSE_NUM; i++ )
	{
		if( sActionPose[i]!=0 )
			_nPoseNum++;
		else 
			break;
	}
}

//---------------------------------------------------------------------------
// class CSkillRecordSet
//---------------------------------------------------------------------------
CSkillRecordSet * CSkillRecordSet::_Instance = NULL;

BOOL CSkillRecordSet::_ReadRawDataInfo(CRawDataInfo *pRawDataInfo, vector<string> &ParamList)
{   
	if(ParamList.size()==0) return FALSE;

	CSkillRecord *pInfo = (CSkillRecord*)pRawDataInfo;

	pInfo->sID = pInfo->nID;
	_tcsncpy(pInfo->szName, pInfo->szDataName, defSKILL_NAME_LEN);
	pInfo->szName[defSKILL_NAME_LEN - 1] = _TEXT('\0');

	int m = 0, n = 0;
    string strList[80], strSub[10];
	string strLine;

	// 
	pInfo->chFightType = Str2Int(ParamList[m++]);

	// 
	memset(pInfo->chJobSelect, cchSkillRecordKeyValue, sizeof(char) * defSKILL_JOB_SELECT_NUM);
	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ';');
	n = n > defSKILL_JOB_SELECT_NUM ? defSKILL_JOB_SELECT_NUM : n;
	for (int i = 0; i < n; i++)
	{
		Util_ResolveTextLine(strList[i].c_str(), strSub, 2, ',');
		pInfo->chJobSelect[i][0] = Str2Int(strSub[0]);
		pInfo->chJobSelect[i][1] = Str2Int(strSub[1]);
	}

	// 
	for( int i=0; i<3; i++ )
	{
		for( int j=0; j<defSKILL_ITEM_NEED_NUM; j++ )
		{
			for( int k=0; k<2; k++ )
			{
				pInfo->sItemNeed[i][j][k] = cchSkillRecordKeyValue;
			}
		}
	}
	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ';');
	n = n > defSKILL_ITEM_NEED_NUM ? defSKILL_ITEM_NEED_NUM : n;
	for (int i = 0; i < n; i++)
	{
		Util_ResolveTextLine(strList[i].c_str(), strSub, 2, ',');
		pInfo->sItemNeed[0][i][0] = Str2Int(strSub[0]);
		pInfo->sItemNeed[0][i][1] = Str2Int(strSub[1]);
	}

	// 
	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ';');
	n = n > defSKILL_ITEM_NEED_NUM ? defSKILL_ITEM_NEED_NUM : n;
	for (int i = 0; i < n; i++)
	{
		Util_ResolveTextLine(strList[i].c_str(), strSub, 2, ',');
		pInfo->sItemNeed[1][i][0] = Str2Int(strSub[0]);
		pInfo->sItemNeed[1][i][1] = Str2Int(strSub[1]);
	}

	// 
	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ';');
	n = n > defSKILL_ITEM_NEED_NUM ? defSKILL_ITEM_NEED_NUM : n;
	for (int i = 0; i < n; i++)
	{
		Util_ResolveTextLine(strList[i].c_str(), strSub, 2, ',');
		pInfo->sItemNeed[2][i][0] = Str2Int(strSub[0]);
		pInfo->sItemNeed[2][i][1] = Str2Int(strSub[1]);
	}

	// 
	pInfo->sConchNeed[0][0] = cchSkillRecordKeyValue;
	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ';');
	n = n > defSKILL_ITEM_NEED_NUM ? defSKILL_ITEM_NEED_NUM : n;
	for (int i = 0; i < defSKILL_ITEM_NEED_NUM; i++)
	{
		if (i < n)
		{
			Util_ResolveTextLine(strList[i].c_str(), strSub, 3, ',');
			pInfo->sConchNeed[i][0] = Str2Int(strSub[0]);
			pInfo->sConchNeed[i][1] = Str2Int(strSub[1]);
			pInfo->sConchNeed[i][2] = Str2Int(strSub[2]);
		}
		else
		{
			pInfo->sConchNeed[i][0] = cchSkillRecordKeyValue;
			break;
		}
	}

	// 
	pInfo->chPhase = Str2Int(ParamList[m++]);

	// 
	pInfo->chType = Str2Int(ParamList[m++]);

	// 
	pInfo->chHelpful = Str2Int(ParamList[m++]);

	// 
	pInfo->sLevelDemand = Str2Int(ParamList[m++]);

	//
	for (int i = 0; i < defSKILL_PRE_SKILL_NUM; i++)
	{
		pInfo->sPremissSkill[i][0] = cchSkillRecordKeyValue;
		pInfo->sPremissSkill[i][1] = cchSkillRecordKeyValue;
	}
    string strSubList[2];
	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ';');
	n = n > defSKILL_PRE_SKILL_NUM ? defSKILL_PRE_SKILL_NUM : n;
	for (int i = 0; i < n; i++)
	{
		Util_ResolveTextLine(strList[i].c_str(), strSub, 2, ',');
		pInfo->sPremissSkill[i][0] = Str2Int(strSub[0]);
		pInfo->sPremissSkill[i][1] = Str2Int(strSub[1]);
	}

	// 
	pInfo->chPointExpend = Str2Int(ParamList[m++]);

	// 
	pInfo->chSrcType = Str2Int(ParamList[m++]);

	// 
	pInfo->chTarType = Str2Int(ParamList[m++]);

	// 
	pInfo->sApplyDistance = Str2Int(ParamList[m++]);

	// 
	pInfo->chApplyTarget = Str2Int(ParamList[m++]);

	// 
	pInfo->chApplyType = Str2Int(ParamList[m++]);

	// 
	pInfo->sAngle = Str2Int(ParamList[m++]);

	// 
	pInfo->sRadii = Str2Int(ParamList[m++]);

	// 
	pInfo->chRange = Str2Int(ParamList[m++]);

	// 
	strncpy(pInfo->szPrepare, ParamList[m++].c_str(), defSKILL_RANGE_SET_SCRIPT);
	pInfo->szPrepare[defSKILL_RANGE_SET_SCRIPT - 1] = _TEXT('\0');

	//// 
	strncpy(pInfo->szRangeState, ParamList[m++].c_str(), defSKILL_EFFECT_SCRIPT_LEN);
	pInfo->szRangeState[defSKILL_EFFECT_SCRIPT_LEN - 1] = _TEXT('\0');

	// SP
	strncpy(pInfo->szUseSP, ParamList[m++].c_str(), defSKILL_EFFECT_SCRIPT_LEN);
	pInfo->szUseSP[defSKILL_EFFECT_SCRIPT_LEN - 1] = _TEXT('\0');

	// 
	strncpy(pInfo->szUseEndure, ParamList[m++].c_str(), defSKILL_EFFECT_SCRIPT_LEN);
	pInfo->szUseEndure[defSKILL_EFFECT_SCRIPT_LEN - 1] = _TEXT('\0');

	// 
	strncpy(pInfo->szUseEnergy, ParamList[m++].c_str(), defSKILL_EFFECT_SCRIPT_LEN);
	pInfo->szUseEnergy[defSKILL_EFFECT_SCRIPT_LEN - 1] = _TEXT('\0');

	// 
	strncpy(pInfo->szSetRange, ParamList[m++].c_str(), defSKILL_EFFECT_SCRIPT_LEN);
	pInfo->szSetRange[defSKILL_EFFECT_SCRIPT_LEN - 1] = _TEXT('\0');

	// 
	strncpy(pInfo->szUse, ParamList[m++].c_str(), defSKILL_RANGE_SET_SCRIPT);
	pInfo->szUse[defSKILL_RANGE_SET_SCRIPT - 1] = _TEXT('\0');

	// 
	strncpy(pInfo->szEffect, ParamList[m++].c_str(), defSKILL_RANGE_SET_SCRIPT);
	pInfo->szEffect[defSKILL_RANGE_SET_SCRIPT - 1] = _TEXT('\0');

	// 
	strncpy(pInfo->szActive, ParamList[m++].c_str(), defSKILL_RANGE_SET_SCRIPT);
	pInfo->szActive[defSKILL_RANGE_SET_SCRIPT - 1] = _TEXT('\0');

	// 
	strncpy(pInfo->szInactive, ParamList[m++].c_str(), defSKILL_RANGE_SET_SCRIPT);
	pInfo->szInactive[defSKILL_RANGE_SET_SCRIPT - 1] = _TEXT('\0');

	// 
	pInfo->nStateID = Str2Int(ParamList[m++]);

	// 
	strLine = ParamList[m++];

	// 
	strLine = ParamList[m++];

	// 
	strLine = ParamList[m++];

	// 
	strLine = ParamList[m++];

	// 
	strLine = ParamList[m++];

	// 
	pInfo->sSplashPara = Str2Int(ParamList[m++]);

	// 
	pInfo->sTargetEffect = Str2Int(ParamList[m++]);

	// 
	pInfo->sSplashEffect = Str2Int(ParamList[m++]);

	// 
	pInfo->sVariation = Str2Int(ParamList[m++]);

	// 
	pInfo->sSummon = Str2Int(ParamList[m++]);

	// 
	strLine = ParamList[m++];

	// 
	strncpy(pInfo->szFireSpeed, ParamList[m++].c_str(), defSKILL_EFFECT_SCRIPT_LEN);
	pInfo->szFireSpeed[defSKILL_EFFECT_SCRIPT_LEN - 1] = _TEXT('\0');

	// 
	pInfo->sActionHarm = Str2Int(ParamList[m++]);

	pInfo->chActionPlayType = Str2Int(ParamList[m++]);

    // 
    strLine = ParamList[m++];
    memset( pInfo->sActionPose, 0, sizeof(pInfo->sActionPose) );
    n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ',');
    n = n > defSKILL_POSE_NUM ? defSKILL_POSE_NUM : n;
    for (int i = 0; i < n; i++)
    {
        pInfo->sActionPose[i] = Str2Int(strList[i]);
    }

	pInfo->sActionKeyFrme = Str2Int(ParamList[m++]);

	// 
	pInfo->sWhop = Str2Int(ParamList[m++]);

	// link
	memset(pInfo->sActionDummyLink, 0, sizeof(pInfo->sActionDummyLink));
	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ',');
	n = n > defSKILL_ITEM_EFFECT ? defSKILL_ITEM_EFFECT : n;
	for (int i = 0; i < n; i++)
	{
		pInfo->sActionDummyLink[i] = Str2Int(strList[i]);
	}

	// 
	memset(pInfo->sActionEffect, 0, sizeof(pInfo->sActionEffect));
	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ',');
	n = n > defSKILL_ITEM_EFFECT ? defSKILL_ITEM_EFFECT : n;
	for (int i = 0; i < n; i++)
	{
		pInfo->sActionEffect[i] = Str2Int(strList[i]);
	}

	// :0-,1-
	memset(pInfo->sActionEffectType, 0, sizeof(pInfo->sActionEffectType));
	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ',');
	n = n > defSKILL_ITEM_EFFECT ? defSKILL_ITEM_EFFECT : n;
	for (int i = 0; i < n; i++)
	{
		pInfo->sActionEffectType[i] = Str2Int(strList[i]);
	}

	pInfo->sItemDummyLink = Str2Int(ParamList[m++]);

	memset(pInfo->sItemEffect1, 0, sizeof(pInfo->sItemEffect1));
	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ',');
	n = n > defSKILL_ITEM_EFFECT ? defSKILL_ITEM_EFFECT : n;
	for (int i = 0; i < n; i++)
	{
		pInfo->sItemEffect1[i] = Str2Int(strList[i]);
	}

	memset(pInfo->sItemEffect2, 0, sizeof(pInfo->sItemEffect2));
	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ',');
	n = n > defSKILL_ITEM_EFFECT ? defSKILL_ITEM_EFFECT : n;
	for (int i = 0; i < n; i++)
	{
		pInfo->sItemEffect2[i] = Str2Int(strList[i]);
	}

	pInfo->sSkyEffectActionKeyFrame = Str2Int(ParamList[m++]);

	pInfo->sSkyEffectActionDummyLink = Str2Int(ParamList[m++]);

	pInfo->sSkyEffectItemDummyLink = Str2Int(ParamList[m++]);

	pInfo->sSkyEffect = Str2Int(ParamList[m++]);

	pInfo->sSkySpd = Str2Int(ParamList[m++]);

	// 
	pInfo->sWhoped = Str2Int(ParamList[m++]);

	pInfo->sTargetDummyLink = Str2Int(ParamList[m++]);

	pInfo->sTargetEffectID = Str2Int(ParamList[m++]);

	pInfo->chTargetEffectTime = Str2Int(ParamList[m++]);

	pInfo->sAgroundEffectID = Str2Int(ParamList[m++]);
    
	pInfo->sWaterEffectID = Str2Int(ParamList[m++]);

	//short	sICON;												// 
	_tcsncpy(pInfo->szICON, ParamList[m++].c_str(), defSKILL_ICON_NAME_LEN);
	pInfo->szICON[defSKILL_ICON_NAME_LEN - 1] = _TEXT('\0');

	// 
	pInfo->chPlayTime = Str2Int(ParamList[m++]);

	// 
	memset(pInfo->chOperate, 0, sizeof(char) * defSKILL_OPERATE_NUM);
	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ',');
	n = n > defSKILL_OPERATE_NUM ? defSKILL_OPERATE_NUM : n;
	for (int i = 0; i < n; i++)
	{
		pInfo->chOperate[i] = Str2Int(strList[i]);
	}

	// 
	_tcsncpy(pInfo->szDescribeHint, ParamList[m++].c_str(), sizeof(pInfo->szDescribeHint) );
	pInfo->szDescribeHint[sizeof(pInfo->szDescribeHint)-1] = '\0';

	// 
	_tcsncpy(pInfo->szEffectHint, ParamList[m++].c_str(), sizeof(pInfo->szEffectHint) );
	pInfo->szEffectHint[sizeof(pInfo->szEffectHint)-1] = '\0';

	// 
	_tcsncpy(pInfo->szExpendHint, ParamList[m++].c_str(), sizeof(pInfo->szExpendHint) );
	pInfo->szExpendHint[sizeof(pInfo->szExpendHint)-1] = '\0';
	return TRUE;
}

void CSkillRecordSet::_ProcessRawDataInfo(CRawDataInfo *pInfo)
{
	CSkillRecord *pSkill = (CSkillRecord*)pInfo;

    // 
	pSkill->RefreshPrivateData();	
}
