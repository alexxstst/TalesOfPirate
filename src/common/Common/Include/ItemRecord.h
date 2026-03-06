#pragma once

//=============================================================================
// FileName: ItemRecord.h
// Creater: ZhangXuedong
// Date: 2004.09.01
// Comment: CItemRecordSet class
//=============================================================================

#ifndef	ITEMRECORD_H
#define ITEMRECORD_H

#include <tchar.h>
#include "util.h"
#include "TableData.h"
#include "CompCommand.h"
#include "JobType.h"

const char cchItemRecordKeyValue = (char)0xfe;	// -1

#define defITEM_NAME_LEN	80
#define defITEM_MODULE_NUM	5
#define defITEM_MODULE_LEN	19
#define defITEM_ICON_NAME_LEN	17
#define defITEM_ATTREFFECT_NAME_LEN	33

#define defITEM_DESCRIPTOR_NAME_LEN      257
#define defITEM_DESCRIPTOR_SET_LEN      33

#define defITEM_BIND_EFFECT_NUM			8

#define defITEM_BODY 4

enum EItemType
{
	enumItemTypeSword		= 1,	// 
	enumItemTypeGlave		= 2,	// 
	enumItemTypeBow			= 3,	// 
	enumItemTypeHarquebus	= 4,	// 
	enumItemTypeFalchion	= 5,	// 
	enumItemTypeMitten		= 6,	// 
	enumItemTypeStylet		= 7,	// 
	enumItemTypeMoneybag	= 8,	// 
	enumItemTypeCosh		= 9,	// 
	enumItemTypeSinker		= 10,	// 
	enumItemTypeShield		= 11,	// 
	enumItemTypeArrow		= 12,	// 
	enumItemTypeAmmo		= 13,	// 
	enumItemTypeHeadpiece	= 19,	// 
	enumItemTypeHair		= 20,	// 
	enumItemTypeFace		= 21,	// 
	enumItemTypeClothing	= 22,	// 
	enumItemTypeGlove		= 23,	// 
	enumItemTypeBoot		= 24,	// 
	enumItemTypeNecklace	= 25,
	enumItemTypeRing		= 26,
	enumItemTypeTattoo		= 27,
	enumItemTypeConch		= 29,	// 
	enumItemTypeMedicine	= 31,	// 
	enumItemTypeOvum		= 32,	// 
	enumItemTypeUnknownConsumable = 33, // TODO: Proper naming
	enumItemTypeScroll		= 36,	// 
	enumItemTypeGeneral		= 41,	// 
    enumItemTypeMission     = 42,   // 
	enumItemTypeBoat		= 43,	// 
	enumItemTypeWing		= 44,	// 
	enumItemTypeTrade		= 45,	// 
	enumItemTypeBravery		= 46,	// 
	enumItemTypeHull		= 51,	// 
	enumItemTypeEmbolon		= 52,	// 
	enumItemTypeEngine		= 53,	// 
	enumItemTypeArtillery	= 54,	// 
	enumItemTypeAirscrew	= 55,	// 
	enumItemTypeBoatSign	= 56,	// 
	enumItemTypePetFodder	= 57,	// 
	enumItemTypePetSock		= 58,	// 
	enumItemTypePet			= 59,	// 

	enumItemCloak			= 88,
	enumItemMount			= 90,

	// Add by lark.li 20080514 begin
	enumItemTypeNo			= 99,	// 
	// End
};

enum EItemPickTo
{
	enumITEM_PICKTO_KITBAG,
	enumITEM_PICKTO_CABIN,
};

class CItemRecord : public CRawDataInfo
{
public:
	enum EItemExtendInfo
	{
		enumItemNormalStart		= 0,		// 
		enumItemNormalEnd		= 4999,
		enumItemFusionEndure   = 23000,      // //Modify by ning.yan 20080821 
		enumItemFusionStart		= 5000,		// 
		enumItemFusionEnd		= 9800,		// 
		// Modify by lark.li 20080703
		enumItemMax				= 32768,		// 
		// End

	};

	CItemRecord();

	long	lID;							// 
	_TCHAR	szName[defITEM_NAME_LEN];		// 
	char    szICON[defITEM_ICON_NAME_LEN];	// ICON
	_TCHAR	chModule[defITEM_MODULE_NUM][defITEM_MODULE_LEN];	// 
	short	sShipFlag;						// 
	short	sShipType;						// 
	short	sType;							// 

	char	chForgeLv;						// 
	char	chForgeSteady;					// 
	char	chExclusiveID;					// ID
    char    chIsTrade;                      // 
    char    chIsPick;                       // 
    char    chIsThrow;                      // 
	char	chIsDel;						// 
    long    lPrice;                         // 
	char	chBody[defITEM_BODY];			// 
	short	sNeedLv;						// 
    char    szWork[MAX_JOB_TYPE];           // 

	int		nPileMax;						// 
	char	chInstance;						// 
	char	szAbleLink[enumEQUIP_NUM];		// 
	char	szNeedLink[enumEQUIP_NUM];		// 
	char	chPickTo;						// 0 1 

	short	sStrCoef;						// 
	short	sAgiCoef;						// 
	short	sDexCoef;						// 
	short	sConCoef;						// 
	short	sStaCoef;						// 
	short	sLukCoef;						// 	
	short	sASpdCoef;						// 
	short	sADisCoef;						// 
	short	sMnAtkCoef;						// 
	short	sMxAtkCoef;						// 
	short	sDefCoef;						// 
	short	sMxHpCoef;						// Hp
	short	sMxSpCoef;						// Sp
	short	sFleeCoef;						// 
	short	sHitCoef;						// 
	short	sCrtCoef;						// 
	short	sMfCoef;						// 
	short	sHRecCoef;						// hp
	short	sSRecCoef;						// sp
	short	sMSpdCoef;						// 
	short	sColCoef;						// 

	short	sStrValu[2];					// 
	short	sAgiValu[2];					// 
	short	sDexValu[2];					// 
	short	sConValu[2];					// 
	short	sStaValu[2];					// 
	short	sLukValu[2];					// 
	short	sASpdValu[2];					// 
	short	sADisValu[2];					// 
	short	sMnAtkValu[2];					// 
	short	sMxAtkValu[2];					// 
	short	sDefValu[2];					// 
	short	sMxHpValu[2];					// Hp
	short	sMxSpValu[2];					// Sp
	short	sFleeValu[2];					// 
	short	sHitValu[2];					// 
	short	sCrtValu[2];					// 
	short	sMfValu[2];						// 
	short	sHRecValu[2];					// hp
	short	sSRecValu[2];					// sp
	short	sMSpdValu[2];					// 
	short	sColValu[2];					// 

	short	sPDef[2];						// 
	short	sLHandValu;						// 
	short	sEndure[2];						// 
	short	sEnergy[2];						// 
	short	sHole;							// 
	char	szAttrEffect[defITEM_ATTREFFECT_NAME_LEN];	// 
	short	sDrap;							// drap
	short	sEffect[defITEM_BIND_EFFECT_NUM][2];		// dummy
	short	sItemEffect[2];					// 
	short	sAreaEffect[2];					// 
	short	sUseItemEffect[2];				// 
    _TCHAR  szDescriptor[defITEM_DESCRIPTOR_NAME_LEN];   // 

	float		nCooldown;
	//short	sSeteffect[enumEQUIP_NUM][2];
	//_TCHAR  szDescriptor[5][defITEM_DESCRIPTOR_SET_LEN];

public:
    const char* GetIconFile();              // icon
	static bool	IsVaildFusionID(CItemRecord* pItem); // add by ning.yan  2008-08-21
	
	bool		GetIsPile()							{ return nPileMax>1;	}

	// 
	bool		IsAllowEquip( unsigned int nChaID );

	// 
	bool		IsAllEquip()						{ return chBody[0]==-1;	}

	void		RefreshData();

	int			GetTypeValue( int nType );

	bool		IsFaceItem()						{ return sItemEffect[0]!=0;	}

	bool		IsSendUseItem()						{ return sUseItemEffect[0]!=0;	}

	bool IsWeapon() const;
	bool IsArmor() const;
	bool IsNecklace() const;
	bool IsRing() const;
	bool IsGlove() const;
	bool IsBoot() const;
	bool IsHair() const;
	bool IsConsumable() const;
	bool IsMission() const;
	bool IsLockable() const;

public:
	short		sEffNum;					// 

private:
	bool		_IsBody[5];

};

class CItemRecordSet : public CRawDataSet
{
public:

	static CItemRecordSet* I() { return _Instance; }

	CItemRecordSet(int nIDStart, int nIDCnt, int nCol = 128)
		:CRawDataSet(nIDStart, nIDCnt, nCol)
	{
		_Instance = this;
		_Init();
	}

protected:

	static CItemRecordSet* _Instance; // , 

	virtual CRawDataInfo* _CreateRawDataArray(int nCnt)
	{
		return new CItemRecord[nCnt];
	}

	virtual void _DeleteRawDataArray()
	{
		delete[] (CItemRecord*)_RawDataArray;
	}

	virtual int _GetRawDataInfoSize()
	{
		return sizeof(CItemRecord);
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
	virtual void _ProcessRawDataInfo(CRawDataInfo *pInfo);
};

inline CItemRecord* GetItemRecordInfo( int nTypeID )
{
	return (CItemRecord*)CItemRecordSet::I()->GetRawDataInfo(nTypeID);
}

inline CItemRecord* GetItemRecordInfo(const char *pszItemName)
{
    return (CItemRecord*)CItemRecordSet::I()->GetRawDataInfo(pszItemName);
}

inline bool CItemRecord::IsAllowEquip( unsigned int nChaID )	
{ 
	if( nChaID<5 ) return _IsBody[nChaID];

	return false;
}

#endif //ITEMRECORD_H
