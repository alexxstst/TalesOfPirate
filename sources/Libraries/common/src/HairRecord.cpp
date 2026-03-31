#include "HairRecord.h"
#include "HairRecordStore.h"

CHairRecord::CHairRecord()
{
	memset( dwNeedItem, 0, sizeof(dwNeedItem) );
	memset( IsChaUse, 0, sizeof(IsChaUse) );
	memset( dwFailItemID, 0, sizeof(dwFailItemID) );

	dwItemID = 0;
	dwMoney = 0;
	_nFailNum = 0;
}

void CHairRecord::RefreshPrivateData()
{
	_nFailNum = 0;
	for( int i=0; i<defHAIR_MAX_FAIL_ITEM; i++ )
	{
		if( dwFailItemID[i]!=0 )
		{
			_nFailNum++;
		}
		else
		{
			break;
		}
	}
}

CHairRecord* GetHairRecordInfo(int nTypeID) {
	return HairRecordStore::Instance()->Get(nTypeID);
}
