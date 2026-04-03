#include "CharacterRecord.h"
#include "ChaRecordStore.h"

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

CChaRecord* GetChaRecordInfo(int nTypeID, const std::source_location& loc) {
	return ChaRecordStore::Instance()->Get(nTypeID, loc);
}
