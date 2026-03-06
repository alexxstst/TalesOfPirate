//=============================================================================
// FileName: SkillStateRecord.cpp
// Creater: ZhangXuedong
// Date: 2005.02.04
// Comment: Skill State Record
//=============================================================================

#include "SkillStateRecord.h"
using namespace std;
//---------------------------------------------------------------------------
// class CSkillStateRecord
//---------------------------------------------------------------------------
void CSkillStateRecord::RefreshPrivateData()
{
	_nActNum = 0;
	for (int i = 0; i < defSKILLSTATE_ACT_NUM; i++)
	{		
		if( nActBehave[i]!=0 )
		{
			_nActNum++;
		}
		else
		{
			break;
		}
	}
}

//---------------------------------------------------------------------------
// class CSkillStateRecordSet
//---------------------------------------------------------------------------
CSkillStateRecordSet * CSkillStateRecordSet::_Instance = NULL;

BOOL CSkillStateRecordSet::_ReadRawDataInfo(CRawDataInfo *pRawDataInfo, vector<string> &ParamList)
{   
	if(ParamList.size()==0) return FALSE;

	CSkillStateRecord *pInfo = (CSkillStateRecord*)pRawDataInfo;

	int m = 0, n = 0;
    string strList[80];
	string strLine;

	// 
	pInfo->chID = (char)pInfo->nID;
	// 
	strncpy(pInfo->szName, pInfo->szDataName, defSKILLSTATE_NAME_LEN);
	pInfo->szName[defSKILLSTATE_NAME_LEN - 1] = '\0';
	// 
	pInfo->sFrequency = Str2Int(ParamList[m++]);
	// 
	strncpy(pInfo->szOnTransfer, ParamList[m++].c_str(), defSKILLSTATE_SCRIPT_NAME);
	pInfo->szOnTransfer[defSKILLSTATE_SCRIPT_NAME - 1] = '\0';
	// 
	strncpy(pInfo->szAddState, ParamList[m++].c_str(), defSKILLSTATE_SCRIPT_NAME);
	pInfo->szAddState[defSKILLSTATE_SCRIPT_NAME - 1] = '\0';
	// 
	strncpy(pInfo->szSubState, ParamList[m++].c_str(), defSKILLSTATE_SCRIPT_NAME);
	pInfo->szSubState[defSKILLSTATE_SCRIPT_NAME - 1] = '\0';
	// 
	pInfo->chAddType = Str2Int(ParamList[m++]);
	// 
	pInfo->bCanCancel = Str2Int(ParamList[m++]) != 0 ? true : false;
	// 
	pInfo->bCanMove = Str2Int(ParamList[m++]) ? true : false;
	// 
	pInfo->bCanMSkill = Str2Int(ParamList[m++]) ? true : false;
	// 
	pInfo->bCanGSkill = Str2Int(ParamList[m++]) ? true : false;
	// 
	pInfo->bCanTrade = Str2Int(ParamList[m++]) ? true : false;
	// 
	pInfo->bCanItem = Str2Int(ParamList[m++]) ? true : false;
	// 
	pInfo->bCanUnbeatable = Str2Int(ParamList[m++]) ? true : false;
	// 
	pInfo->bCanItemmed = Str2Int(ParamList[m++]) ? true : false;
	// 
	pInfo->bCanSkilled = Str2Int(ParamList[m++]) ? true : false;
	// 
	pInfo->bNoHide = Str2Int(ParamList[m++]) ? true : false;
	// 
	pInfo->bNoShow = Str2Int(ParamList[m++]) ? true : false;
	// 
	pInfo->bOptItem = Str2Int(ParamList[m++]) ? true : false;
	// 
	pInfo->bTalkToNPC = Str2Int(ParamList[m++]) ? true : false;
	// 
	pInfo->bFreeStateID = Str2Int(ParamList[m++]);

	// 
	pInfo->chScreen = Str2Int(ParamList[m++]);
	// 
	memset( pInfo->nActBehave, 0, sizeof(pInfo->nActBehave) );
	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ',');
	n = n > defSKILLSTATE_ACT_NUM ? defSKILLSTATE_ACT_NUM : n;	
	for (int i = 0; i < n; i++)
	{		
		pInfo->nActBehave[i] = Str2Int(strList[i]);
	}

	//
    pInfo->sChargeLink = Str2Int(ParamList[m++]);

    // 
    pInfo->sAreaEffect = Str2Int(ParamList[m++]);
	// ,
    pInfo->IsShowCenter = Str2Int(ParamList[m++]) ? true : false;
	// POSE
	pInfo->IsDizzy = Str2Int(ParamList[m++]) ? true : false;
	// 
	pInfo->sEffect = Str2Int(ParamList[m++]);
	// dummy
	pInfo->sDummy1 = Str2Int(ParamList[m++]);
	// 
	pInfo->sBitEffect = Str2Int(ParamList[m++]);
	// dummy
	pInfo->sDummy2 = Str2Int(ParamList[m++]);
	// ICON
	pInfo->sIcon = Str2Int(ParamList[m++]);

	memset(pInfo->szIcon, 0, sizeof(pInfo->szIcon));
	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 10, ',');
	n = n > 10 ? 10 : n;
	for (int i = 0; i < n; i++)
	{
		strcpy(pInfo->szIcon[i], strList[i].c_str());
		pInfo->szIcon[defSKILLSTATE_NAME_LEN - 1][i] = _TEXT('\0');
	}

	strncpy(pInfo->szDesc, ParamList[m++].c_str(), defSKILLSTATE_DESC_NAME_LEN);
	pInfo->szDesc[defSKILLSTATE_DESC_NAME_LEN - 1] = '\0';

	pInfo->lColour = Str2Int(ParamList[m++]);
	
	return TRUE;
}

void CSkillStateRecordSet::_ProcessRawDataInfo(CRawDataInfo *pInfo)
{
	CSkillStateRecord *pState = (CSkillStateRecord*)pInfo;

    // 
	pState->RefreshPrivateData();	
}
