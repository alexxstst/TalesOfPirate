//=============================================================================
// FileName: ItemRecord.h
// Creater: ZhangXuedong
// Date: 2004.09.01
// Comment: CItemRecordSet class
//=============================================================================

#ifndef	ITEMRECORD_H
#define ITEMRECORD_H

#include <tchar.h>
#include <string>
#include <array>
#include <cstdint>
#include <source_location>
#include "util.h"
#include "TableData.h"
#include "CompCommand.h"
#include "JobType.h"

const char cchItemRecordKeyValue = (char)0xfe; // -1

#define defITEM_NAME_LEN	80
#define defITEM_MODULE_NUM	5
#define defITEM_MODULE_LEN	19
#define defITEM_ICON_NAME_LEN	17
#define defITEM_ATTREFFECT_NAME_LEN	33

#define defITEM_DESCRIPTOR_NAME_LEN      257
#define defITEM_DESCRIPTOR_SET_LEN      33

#define defITEM_BIND_EFFECT_NUM			8

#define defITEM_BODY 4

enum EItemType {
	enumItemTypeSword = 1, //
	enumItemTypeGlave = 2, //
	enumItemTypeBow = 3, //
	enumItemTypeHarquebus = 4, //
	enumItemTypeFalchion = 5, //
	enumItemTypeMitten = 6, //
	enumItemTypeStylet = 7, //
	enumItemTypeMoneybag = 8, //
	enumItemTypeCosh = 9, //
	enumItemTypeSinker = 10, //
	enumItemTypeShield = 11, //
	enumItemTypeArrow = 12, //
	enumItemTypeAmmo = 13, //
	enumItemTypeHeadpiece = 19, //
	enumItemTypeHair = 20, //
	enumItemTypeFace = 21, //
	enumItemTypeClothing = 22, //
	enumItemTypeGlove = 23, //
	enumItemTypeBoot = 24, //
	enumItemTypeNecklace = 25,
	enumItemTypeRing = 26,
	enumItemTypeTattoo = 27,
	enumItemTypeConch = 29, //
	enumItemTypeMedicine = 31, //
	enumItemTypeOvum = 32, //
	enumItemTypeUnknownConsumable = 33, // TODO: Proper naming
	enumItemTypeScroll = 36, //
	enumItemTypeGeneral = 41, //
	enumItemTypeMission = 42, //
	enumItemTypeBoat = 43, //
	enumItemTypeWing = 44, //
	enumItemTypeTrade = 45, //
	enumItemTypeBravery = 46, //
	enumItemTypeHull = 51, //
	enumItemTypeEmbolon = 52, //
	enumItemTypeEngine = 53, //
	enumItemTypeArtillery = 54, //
	enumItemTypeAirscrew = 55, //
	enumItemTypeBoatSign = 56, //
	enumItemTypePetFodder = 57, //
	enumItemTypePetSock = 58, //
	enumItemTypePet = 59, //

	enumItemCloak = 88,
	enumItemMount = 90,

	// Add by lark.li 20080514 begin
	enumItemTypeNo = 99, //
	// End
};

enum EItemPickTo {
	enumITEM_PICKTO_KITBAG,
	enumITEM_PICKTO_CABIN,
};

class CItemRecord : public CRawDataInfo {
public:
	enum EItemExtendInfo {
		enumItemNormalStart = 0, //
		enumItemNormalEnd = 4999,
		enumItemFusionEndure = 23000, // //Modify by ning.yan 20080821
		enumItemFusionStart = 5000, //
		enumItemFusionEnd = 9800, //
		// Modify by lark.li 20080703
		enumItemMax = 32768, //
		// End
	};

	CItemRecord();

	long lID{};
	std::string szName;
	std::string szICON;
	std::array<std::string, defITEM_MODULE_NUM> chModule;
	short sShipFlag{};
	short sShipType{};
	short sType{};

	char chForgeLv{};
	char chForgeSteady{};
	char chExclusiveID{};
	char chIsTrade{};
	char chIsPick{};
	char chIsThrow{};
	char chIsDel{};
	long lPrice{};
	std::array<std::int8_t, defITEM_BODY> chBody{};
	short sNeedLv{};
	std::array<std::int8_t, MAX_JOB_TYPE> szWork{};

	int nPileMax{};
	char chInstance{};
	std::array<std::int8_t, enumEQUIP_NUM> szAbleLink{};
	std::array<std::int8_t, enumEQUIP_NUM> szNeedLink{};
	char chPickTo{};

	short sStrCoef{};
	short sAgiCoef{};
	short sDexCoef{};
	short sConCoef{};
	short sStaCoef{};
	short sLukCoef{};
	short sASpdCoef{};
	short sADisCoef{};
	short sMnAtkCoef{};
	short sMxAtkCoef{};
	short sDefCoef{};
	short sMxHpCoef{};
	short sMxSpCoef{};
	short sFleeCoef{};
	short sHitCoef{};
	short sCrtCoef{};
	short sMfCoef{};
	short sHRecCoef{};
	short sSRecCoef{};
	short sMSpdCoef{};
	short sColCoef{};

	short sStrValu[2]{};
	short sAgiValu[2]{};
	short sDexValu[2]{};
	short sConValu[2]{};
	short sStaValu[2]{};
	short sLukValu[2]{};
	short sASpdValu[2]{};
	short sADisValu[2]{};
	short sMnAtkValu[2]{};
	short sMxAtkValu[2]{};
	short sDefValu[2]{};
	short sMxHpValu[2]{};
	short sMxSpValu[2]{};
	short sFleeValu[2]{};
	short sHitValu[2]{};
	short sCrtValu[2]{};
	short sMfValu[2]{};
	short sHRecValu[2]{};
	short sSRecValu[2]{};
	short sMSpdValu[2]{};
	short sColValu[2]{};

	short sPDef[2]{};
	short sLHandValu{};
	short sEndure[2]{};
	short sEnergy[2]{};
	short sHole{};
	std::string szAttrEffect;
	short sDrap{};
	short sEffect[defITEM_BIND_EFFECT_NUM][2]{};
	short sItemEffect[2]{};
	short sAreaEffect[2]{};
	short sUseItemEffect[2]{};
	std::string szDescriptor{};

	float nCooldown{};

public:
	std::string GetIconFile() const;
	static bool IsVaildFusionID(CItemRecord* pItem);

	bool GetIsPile() const {
		return nPileMax > 1;
	}

	bool IsAllowEquip(unsigned int nChaID) const;

	bool IsAllEquip() const {
		return chBody[0] == static_cast<std::int8_t>(0xFF);
	}

	void RefreshData();
	int GetTypeValue(int nType) const;

	bool IsFaceItem() const {
		return sItemEffect[0] != 0;
	}

	bool IsSendUseItem() const {
		return sUseItemEffect[0] != 0;
	}

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
	short sEffNum{};

private:
	bool _IsBody[5]{};
};

CItemRecord* GetItemRecordInfo(int nTypeID, const std::source_location& loc = std::source_location::current());
CItemRecord* GetItemRecordInfo(std::string_view itemName, const std::source_location& loc = std::source_location::current());

inline bool CItemRecord::IsAllowEquip(unsigned int nChaID) const {
	if (nChaID < 5) return _IsBody[nChaID];
	return false;
}

#endif //ITEMRECORD_H
