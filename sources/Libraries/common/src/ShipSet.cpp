// ShipSet.cpp created by knight 2005.4.20
//---------------------------------------------------------

#include "ShipSet.h"
#include "AssetDatabase.h"
#include "ShipRecordStore.h"
#include "ShipPartRecordStore.h"

//---------------------------------------------------------
using namespace std;

BOOL xShipSet::_ReadRawDataInfo(CRawDataInfo *pRawDataInfo, vector<string> &ParamList)
{   
	if(ParamList.size()==0) return FALSE;

	int m = 0;
	xShipInfo *pInfo = (xShipInfo*)pRawDataInfo;	
	std::string strList[BOAT_MAXNUM_PARTITEM];        
	_tcsncpy( pInfo->szName, pRawDataInfo->szDataName, BOAT_MAXSIZE_NAME );
	pInfo->szName[BOAT_MAXSIZE_NAME - 1] = _TEXT('\0');
	
	pInfo->sItemID = (USHORT)Str2Int(ParamList[m++]);
	pInfo->sCharID = (USHORT)Str2Int(ParamList[m++]);
	pInfo->sPosID  = (USHORT)Str2Int(ParamList[m++]);
	pInfo->byIsUpdate = 1;//(BYTE)Str2Int(ParamList[m++]);
	pInfo->sBody = (USHORT)Str2Int(ParamList[m++]);

	// 
	int nNum = Util_ResolveTextLine( ParamList[m++].c_str(), strList, BOAT_MAXNUM_PARTITEM, ',' );
	pInfo->sNumEngine = ( nNum > 0 ) ? nNum : 0;
	memset( pInfo->sEngine, 0, sizeof(USHORT)*BOAT_MAXNUM_PARTITEM );
	for( int i = 0; i < pInfo->sNumEngine; i++ )
	{
		pInfo->sEngine[i] = (USHORT)Str2Int(strList[i]);	
	}

	nNum = Util_ResolveTextLine( ParamList[m++].c_str(), strList, BOAT_MAXNUM_PARTITEM, ',' );
	pInfo->sNumHeader = ( nNum > 0 ) ? nNum : 0;
	memset( pInfo->sHeader, 0, sizeof(USHORT)*BOAT_MAXNUM_PARTITEM );
	for( int i = 0; i < pInfo->sNumHeader; i++ )
	{
		pInfo->sHeader[i] = (USHORT)Str2Int(strList[i]);
	}

	nNum = Util_ResolveTextLine( ParamList[m++].c_str(), strList, BOAT_MAXNUM_PARTITEM, ',' );
	pInfo->sNumCannon = ( nNum > 0 ) ? nNum : 0;
	memset( pInfo->sCannon, 0, sizeof(USHORT)*BOAT_MAXNUM_PARTITEM );
	for( int i = 0; i < pInfo->sNumCannon; i++ )
	{
		pInfo->sCannon[i] = (USHORT)Str2Int(strList[i]);
	}

	nNum = Util_ResolveTextLine( ParamList[m++].c_str(), strList, BOAT_MAXNUM_PARTITEM, ',' );
	pInfo->sNumEquipment = ( nNum > 0 ) ? nNum : 0;
	memset( pInfo->sEquipment, 0, sizeof(USHORT)*BOAT_MAXNUM_PARTITEM );
	for( int i = 0; i < pInfo->sNumEquipment; i++ )
	{
		pInfo->sEquipment[i] = (USHORT)Str2Int(strList[i]);
	}

	pInfo->sLvLimit = (USHORT)Str2Int(ParamList[m++]);
	nNum = Util_ResolveTextLine( ParamList[m++].c_str(), strList, BOAT_MAXNUM_PARTITEM, ',' );
	pInfo->sNumPfLimit = ( nNum > 0 ) ? nNum : 0;
	memset( pInfo->sPfLimit, 0, sizeof(USHORT)*BOAT_MAXNUM_PARTITEM );
	for( int i = 0; i < pInfo->sNumPfLimit; i++ )
	{
		pInfo->sPfLimit[i] = (USHORT)Str2Int(strList[i]);
	}

	// 
	pInfo->sEndure = (USHORT)Str2Int(ParamList[m++]);
	pInfo->sResume = (USHORT)Str2Int(ParamList[m++]);
	pInfo->sDefence = (USHORT)Str2Int(ParamList[m++]);
	pInfo->sResist = (USHORT)Str2Int(ParamList[m++]);
	pInfo->sMinAttack = (USHORT)Str2Int(ParamList[m++]);
	pInfo->sMaxAttack = (USHORT)Str2Int(ParamList[m++]);
	pInfo->sDistance = (USHORT)Str2Int(ParamList[m++]);
	pInfo->sTime = (USHORT)Str2Int(ParamList[m++]);
	pInfo->sScope = (USHORT)Str2Int(ParamList[m++]);
	pInfo->sCapacity = (USHORT)Str2Int(ParamList[m++]);
	pInfo->sSupply = (USHORT)Str2Int(ParamList[m++]);
	pInfo->sConsume = (USHORT)Str2Int(ParamList[m++]);
	pInfo->sCannonSpeed = (USHORT)Str2Int(ParamList[m++]);
	pInfo->sSpeed = (USHORT)Str2Int(ParamList[m++]);

	_tcsncpy( pInfo->szDesp, ParamList[m++].c_str(), BOAT_MAXSIZE_DESP );
	pInfo->szDesp[BOAT_MAXSIZE_DESP - 1] = _TEXT('\0');

	pInfo->sParam = (USHORT)Str2Int(ParamList[m++]);

	ShipRecordStore::Insert(AssetDatabase::Instance()->GetDb(), *pInfo);
	return TRUE;
}

BOOL xShipPartSet::_ReadRawDataInfo(CRawDataInfo *pRawDataInfo, vector<string> &ParamList)
{   
	if(ParamList.size()==0) return FALSE;

	int m = 0;
	xShipPartInfo *pInfo = (xShipPartInfo*)pRawDataInfo;	
	_tcsncpy( pInfo->szName, pRawDataInfo->szDataName, BOAT_MAXSIZE_NAME );	
	pInfo->szName[BOAT_MAXSIZE_NAME - 1] = _TEXT('\0');

	pInfo->dwModel = (DWORD)Str2Int(ParamList[m++]);
	for( int i = 0; i < BOAT_MAXNUM_MOTOR; i++ )
	{
		pInfo->sMotor[i] = (USHORT)Str2Int(ParamList[m++]);
	}

	pInfo->dwPrice = (DWORD)Str2Int(ParamList[m++]);

	// 
	pInfo->sEndure = (USHORT)Str2Int(ParamList[m++]);
	pInfo->sResume = (USHORT)Str2Int(ParamList[m++]);
	pInfo->sDefence = (USHORT)Str2Int(ParamList[m++]);
	pInfo->sResist = (USHORT)Str2Int(ParamList[m++]);
	pInfo->sMinAttack = (USHORT)Str2Int(ParamList[m++]);
	pInfo->sMaxAttack = (USHORT)Str2Int(ParamList[m++]);
	pInfo->sDistance = (USHORT)Str2Int(ParamList[m++]);
	pInfo->sTime = (USHORT)Str2Int(ParamList[m++]);
	pInfo->sScope = (USHORT)Str2Int(ParamList[m++]);
	pInfo->sCapacity = (USHORT)Str2Int(ParamList[m++]);
	pInfo->sSupply = (USHORT)Str2Int(ParamList[m++]);
	pInfo->sConsume = (USHORT)Str2Int(ParamList[m++]);
	pInfo->sCannonSpeed = (USHORT)Str2Int(ParamList[m++]);
	pInfo->sSpeed = (USHORT)Str2Int(ParamList[m++]);

	_tcsncpy( pInfo->szDesp, ParamList[m++].c_str(), BOAT_MAXSIZE_DESP );
	pInfo->szDesp[BOAT_MAXSIZE_DESP - 1] = _TEXT('\0');

	pInfo->sParam = (USHORT)Str2Int(ParamList[m++]);

	ShipPartRecordStore::Insert(AssetDatabase::Instance()->GetDb(), *pInfo);
	return TRUE;
}

xShipSet*      xShipSet::_Instance     = NULL;
xShipPartSet*  xShipPartSet::_Instance = NULL;
