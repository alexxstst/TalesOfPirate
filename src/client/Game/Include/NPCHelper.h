#pragma once
#include "util.h"
#include "TableData.h"

#define NPC_MAXSIZE_NAME			128 // 

class NPCData : public CRawDataInfo
{
public:
	char szName[NPC_MAXSIZE_NAME];		//
	char szArea[NPC_MAXSIZE_NAME];		// 
	DWORD dwxPos0, dwyPos0;				// 
	char szMapName[NPC_MAXSIZE_NAME];	// 
};


class NPCHelper : public CRawDataSet
{
public:

	static NPCHelper* I() { return _Instance; }

	NPCHelper(int nIDStart, int nIDCnt, int nCol = 128)
		: CRawDataSet(nIDStart, nIDCnt, nCol)
	{
		_Instance = this;
		_Init();
	}

protected:

	static NPCHelper* _Instance; // 

	virtual CRawDataInfo* _CreateRawDataArray(int nCnt)
	{
		return new NPCData[nCnt];
	}

	virtual void _DeleteRawDataArray()
	{
		delete[] (NPCData*)_RawDataArray;
	}

	virtual int _GetRawDataInfoSize()
	{
		return sizeof(NPCData);
	}

	virtual void*	_CreateNewRawData(CRawDataInfo *pInfo)
	{
		return NULL;
	}

	virtual void  _DeleteRawData(CRawDataInfo *pInfo)
	{
		SAFE_DELETE(pInfo->pData);
	}

	virtual BOOL _ReadRawDataInfo(CRawDataInfo *pRawDataInfo, std::vector<std::string> &ParamList);
};


inline NPCData* GetNPCDataInfo( int nTypeID )
{
	return (NPCData*)NPCHelper::I()->GetRawDataInfo(nTypeID);
}
