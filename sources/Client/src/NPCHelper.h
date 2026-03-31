#pragma once

#include "NPCDataRecord.h"
#include "MonsterListRecordStore.h"
#include "NPCListRecordStore.h"
#include "AssetDatabase.h"
#include "TableData.h"

enum class NPCHelperType { MonsterList, NPCList };

class NPCHelper : public CRawDataSet
{
public:
	static NPCHelper* I() { return _Instance; }

	NPCHelper(int nIDStart, int nIDCnt, NPCHelperType type, int nCol = 128)
		: CRawDataSet(nIDStart, nIDCnt, nCol), _type(type)
	{
		_Instance = this;
		_Init();
	}

protected:
	static NPCHelper* _Instance;
	NPCHelperType _type;

	virtual CRawDataInfo* _CreateRawDataArray(int nCnt) { return new NPCData[nCnt]; }
	virtual void _DeleteRawDataArray() { delete[] (NPCData*)_RawDataArray; }
	virtual int _GetRawDataInfoSize() { return sizeof(NPCData); }
	virtual void* _CreateNewRawData(CRawDataInfo *pInfo) { return NULL; }
	virtual void _DeleteRawData(CRawDataInfo *pInfo) { SAFE_DELETE(pInfo->pData); }

	virtual BOOL _ReadRawDataInfo(CRawDataInfo *pRawDataInfo, std::vector<std::string> &ParamList);

	virtual void _ProcessRawDataInfo(CRawDataInfo *pRawDataInfo) {
		auto& db = AssetDatabase::Instance()->GetDb();
		if (_type == NPCHelperType::MonsterList)
			MonsterListRecordStore::Insert(db, *(NPCData*)pRawDataInfo);
		else
			NPCListRecordStore::Insert(db, *(NPCData*)pRawDataInfo);
	}
};

inline NPCData* GetNPCDataInfo(int nTypeID)
{
	return (NPCData*)NPCHelper::I()->GetRawDataInfo(nTypeID);
}
