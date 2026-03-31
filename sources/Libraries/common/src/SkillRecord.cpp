#include "SkillRecord.h"
#include "SkillRecordStore.h"

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
	case enumSKILL_TYPE_SALVAGE:	_eSelectCha = enumSC_MONS_DBOAT;	break;

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

CSkillRecord* GetSkillRecordInfo(int nTypeID) {
	return SkillRecordStore::Instance()->Get(nTypeID);
}

CSkillRecord* GetSkillRecordInfo(const char* szName) {
	return SkillRecordStore::Instance()->Get(szName);
}
