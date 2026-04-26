#pragma once

#include "NPCDataRecord.h"
#include "MonsterListRecordStore.h"
#include "NPCListRecordStore.h"

enum class NPCHelperType { MonsterList, NPCList };

inline NPCData* GetNPCDataInfo(int nTypeID, NPCHelperType type) {
	if (type == NPCHelperType::MonsterList)
		return MonsterListRecordStore::Instance()->Get(nTypeID);
	else
		return NPCListRecordStore::Instance()->Get(nTypeID);
}

inline int GetNPCMaxId(NPCHelperType type) {
	if (type == NPCHelperType::MonsterList)
		return MonsterListRecordStore::Instance()->GetMaxId();
	else
		return NPCListRecordStore::Instance()->GetMaxId();
}
