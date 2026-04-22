#include "ItemRecord.h"
#include <format>
//---------------------------------------------------------------------------
// class CItemRecord
//---------------------------------------------------------------------------
CItemRecord::CItemRecord() {
	chBody.fill(static_cast<std::int8_t>(0xFE));
	szWork.fill(static_cast<std::int8_t>(0xFE));
	szAbleLink.fill(static_cast<std::int8_t>(0xFE));
	szNeedLink.fill(static_cast<std::int8_t>(0xFE));
}

std::string CItemRecord::GetIconFile() const {
	return std::format("texture/icon/{}.png", szICON);
}

bool CItemRecord::IsVaildFusionID(CItemRecord* pItem) {
	return false;
	if (pItem->sEndure[1] == enumItemFusionEndure) {
		return true;
	}
	else {
		return false;
	}
}


void CItemRecord::RefreshData() {
	std::fill(std::begin(_IsBody), std::end(_IsBody), false);

	if (IsAllEquip()) {
		for (int i = 1; i < 5; i++)
			_IsBody[i] = true;
	}
	else {
		for (int i = 0; i < defITEM_BODY; i++) {
			if (chBody[i] == cchItemRecordKeyValue) {
				break;
			}
			if (chBody[i] > 0 && chBody[i] < 5) {
				_IsBody[chBody[i]] = true;
			}
		}
	}

	bool hasChaModel[5]{};
	for (int i = 1; i < 5; i++)
		hasChaModel[i] = chModule[i].size() > 1;

	static constexpr const char* slotNames[] = {
		"Head", "Face", "Body", "Glove", "Shoes", nullptr, "LHand", nullptr, nullptr, "RHand"
	};

	if (sType < 31) {
		switch (szAbleLink[0]) {
		case enumEQUIP_HEAD:
		case enumEQUIP_FACE:
		case enumEQUIP_BODY:
		case enumEQUIP_GLOVE:
		case enumEQUIP_SHOES:
		case enumEQUIP_LHAND:
		case enumEQUIP_RHAND: {
			int slot = szAbleLink[0];
			const char* slotName = (slot >= 0 && slot < 10 && slotNames[slot]) ? slotNames[slot] : "Unknown";
			for (int i = 1; i < 5; i++) {
				if (_IsBody[i] && !hasChaModel[i]) {
					// ToLogService("common", LogLevel::Warning,
					// 	"Item '{}' (ID:{}, type:{}) equip slot '{}' — missing model for class {}",
					// 	szName, lID, sType, slotName, i);
				}
			}
			break;
		}
		}
	}
}

int CItemRecord::GetTypeValue(int nType) const {
	switch (nType) {
	case ITEMATTR_COE_STR: return sStrCoef;
	case ITEMATTR_COE_AGI: return sAgiCoef;
	case ITEMATTR_COE_DEX: return sDexCoef;
	case ITEMATTR_COE_CON: return sConCoef;
	case ITEMATTR_COE_STA: return sStaCoef;
	case ITEMATTR_COE_LUK: return sLukCoef;
	case ITEMATTR_COE_ASPD: return sASpdCoef;
	case ITEMATTR_COE_ADIS: return sADisCoef;
	case ITEMATTR_COE_MNATK: return sMnAtkCoef;
	case ITEMATTR_COE_MXATK: return sMxAtkCoef;
	case ITEMATTR_COE_DEF: return sDefCoef;
	case ITEMATTR_COE_MXHP: return sMxHpCoef;
	case ITEMATTR_COE_MXSP: return sMxSpCoef;
	case ITEMATTR_COE_FLEE: return sFleeCoef;
	case ITEMATTR_COE_HIT: return sHitCoef;
	case ITEMATTR_COE_CRT: return sCrtCoef;
	case ITEMATTR_COE_MF: return sMfCoef;
	case ITEMATTR_COE_HREC: return sHRecCoef;
	case ITEMATTR_COE_SREC: return sSRecCoef;
	case ITEMATTR_COE_MSPD: return sMSpdCoef;
	case ITEMATTR_COE_COL: return sColCoef;

	case ITEMATTR_COE_PDEF: return 0;

	case ITEMATTR_VAL_STR: return sStrValu[0];
	case ITEMATTR_VAL_AGI: return sAgiValu[0];
	case ITEMATTR_VAL_DEX: return sDexValu[0];
	case ITEMATTR_VAL_CON: return sConValu[0];
	case ITEMATTR_VAL_STA: return sStaValu[0];
	case ITEMATTR_VAL_LUK: return sLukValu[0];
	case ITEMATTR_VAL_ASPD: return sASpdValu[0];
	case ITEMATTR_VAL_ADIS: return sADisValu[0];
	case ITEMATTR_VAL_MNATK: return sMnAtkValu[0];
	case ITEMATTR_VAL_MXATK: return sMxAtkValu[0];
	case ITEMATTR_VAL_DEF: return sDefValu[0];
	case ITEMATTR_VAL_MXHP: return sMxHpValu[0];
	case ITEMATTR_VAL_MXSP: return sMxSpValu[0];
	case ITEMATTR_VAL_FLEE: return sFleeValu[0];
	case ITEMATTR_VAL_HIT: return sHitValu[0];
	case ITEMATTR_VAL_CRT: return sCrtValu[0];
	case ITEMATTR_VAL_HREC: return sHRecValu[0];
	case ITEMATTR_VAL_SREC: return sSRecValu[0];
	case ITEMATTR_VAL_MSPD: return sMSpdValu[0];
	case ITEMATTR_VAL_COL: return sColValu[0];
	case ITEMATTR_VAL_PDEF: return sPDef[0];
	}
	return 0;
}

bool CItemRecord::IsWeapon() const {
	using E = EItemType;
	return sType == E::enumItemTypeSword ||
		sType == E::enumItemTypeGlave ||
		sType == E::enumItemTypeBow ||
		sType == E::enumItemTypeHarquebus ||
		sType == E::enumItemTypeFalchion ||
		sType == E::enumItemTypeMitten ||
		sType == E::enumItemTypeStylet ||
		sType == E::enumItemTypeMoneybag ||
		sType == E::enumItemTypeCosh ||
		sType == E::enumItemTypeSinker;
}

bool CItemRecord::IsArmor() const {
	using E = EItemType;
	return sType == E::enumItemTypeClothing ||
		sType == E::enumItemTypeShield ||
		sType == E::enumItemTypeTattoo;
}

bool CItemRecord::IsNecklace() const {
	using E = EItemType;
	return sType == E::enumItemTypeNecklace;
}

bool CItemRecord::IsRing() const {
	using E = EItemType;
	return sType == E::enumItemTypeRing;
}

bool CItemRecord::IsGlove() const {
	using E = EItemType;
	return sType == E::enumItemTypeGlove;
}

bool CItemRecord::IsBoot() const {
	using E = EItemType;
	return sType == E::enumItemTypeBoot;
}

bool CItemRecord::IsHair() const {
	using E = EItemType;
	return sType == E::enumItemTypeHair;
}

bool CItemRecord::IsConsumable() const {
	using E = EItemType;
	return sType == E::enumItemTypeMedicine ||
		sType == E::enumItemTypeOvum ||
		sType == E::enumItemTypeUnknownConsumable;
}

bool CItemRecord::IsMission() const {
	using E = EItemType;
	return sType == E::enumItemTypeMission;
}

bool CItemRecord::IsLockable() const {
	return IsWeapon() ||
		IsHair() ||
		IsArmor() ||
		IsGlove() ||
		IsBoot() ||
		sType == EItemType::enumItemTypeConch ||
		sType == EItemType::enumItemTypePet;
}

// ============================================================================
// GetItemRecordInfo — доступ через ItemRecordStore (SQLite)
// ============================================================================

#include "ItemRecordStore.h"

CItemRecord* GetItemRecordInfo(int nTypeID, const std::source_location& loc) {
	return ItemRecordStore::Instance()->Get(nTypeID, loc);
}

CItemRecord* GetItemRecordInfo(std::string_view itemName, const std::source_location& loc) {
	return ItemRecordStore::Instance()->Get(itemName, loc);
}
