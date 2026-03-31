#include "SkillStateRecord.h"
#include "SkillStateRecordStore.h"

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

CSkillStateRecord* GetCSkillStateRecordInfo(int nTypeID) {
	return SkillStateRecordStore::Instance()->Get(nTypeID);
}
