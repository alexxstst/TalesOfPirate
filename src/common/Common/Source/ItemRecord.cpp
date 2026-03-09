//=============================================================================
// FileName: ItemRecord.h
// Creater: ZhangXuedong
// Date: 2004.09.01
// Comment: CItemRecordSet class
//=============================================================================

#include "ItemRecord.h"
using namespace std;
//---------------------------------------------------------------------------
// class CItemRecord
//---------------------------------------------------------------------------
CItemRecord::CItemRecord() {
}

const char* CItemRecord::GetIconFile() {
	static char buf[32] = {0};
	// sprintf( buf, "texture/icon/%s.tga", szICON );
	sprintf(buf, "texture/icon/%s.png", szICON);

	return buf;
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
	memset(_IsBody, 0, sizeof(_IsBody));
	//for some reason memset didnt work,
	//so manually set each value to false.
	for (int i = 0; i < 5; i++) {
		_IsBody[i] = false;
	}
	if (IsAllEquip()) {
		for (int i = 1; i < 5; i++) {
			_IsBody[i] = true;
		}
	}
	else {
		for (int i = 0; i < defITEM_BODY; i++)
			if (chBody[i] > 0 && chBody[i] < 5)
				_IsBody[chBody[i]] = true;
	}

	bool IsTmpBody[5] = {0};
	for (int i = 1; i < 5; i++) {
		IsTmpBody[i] = chModule[i][1] == '\0' ? false : true;
	}

	if (sType < 31) {
		switch (szAbleLink[0]) {
		case enumEQUIP_HEAD:
		case enumEQUIP_FACE:
		case enumEQUIP_BODY:
		case enumEQUIP_GLOVE:
		case enumEQUIP_SHOES:
		case enumEQUIP_LHAND:
		case enumEQUIP_RHAND: {
			for (int i = 1; i < 5; i++) {
				if (_IsBody[i] && !IsTmpBody[i]) {
					ToLogService("iteminfoerror", "{},{},cha ID:{}", lID, szName, i);
					return;
				}
			}
		}
		};
	}
}

int CItemRecord::GetTypeValue(int nType) {
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

//---------------------------------------------------------------------------
// class CItemRecordSet
//---------------------------------------------------------------------------
CItemRecordSet* CItemRecordSet::_Instance = NULL;

BOOL CItemRecordSet::_ReadRawDataInfo(CRawDataInfo* pRawDataInfo, vector<string>& ParamList) {
	if (ParamList.size() == 0) return FALSE;

	CItemRecord* pInfo = (CItemRecord*)pRawDataInfo;

	int m = 0, n = 0;
	string strList[80];
	string strLine;

	//
	pInfo->lID = pInfo->nID;
	//
	_tcsncpy(pInfo->szName, pInfo->szDataName, defITEM_NAME_LEN);
	pInfo->szName[defITEM_NAME_LEN - 1] = _TEXT('\0');
	// ICON
	_tcsncpy(pInfo->szICON, ParamList[m++].c_str(), defITEM_ICON_NAME_LEN);
	pInfo->szICON[defITEM_ICON_NAME_LEN - 1] = _TEXT('\0');
	//
	for (int i = 0; i < defITEM_MODULE_NUM; i++) {
		_tcsncpy(pInfo->chModule[i], ParamList[m++].c_str(), defITEM_MODULE_LEN);
		pInfo->chModule[i][defITEM_MODULE_LEN - 1] = _TEXT(0);
	}
	//
	pInfo->sShipFlag = Str2Int(ParamList[m++]);
	//
	pInfo->sShipType = Str2Int(ParamList[m++]);
	//
	pInfo->sType = Str2Int(ParamList[m++]);

	//
	strLine = Str2Int(ParamList[m++]);
	//
	strLine = Str2Int(ParamList[m++]);
	//
	pInfo->chForgeLv = Str2Int(ParamList[m++]);
	//
	pInfo->chForgeSteady = Str2Int(ParamList[m++]);
	// ID
	pInfo->chExclusiveID = Str2Int(ParamList[m++]);
	//
	pInfo->chIsTrade = Str2Int(ParamList[m++]);
	//
	pInfo->chIsPick = Str2Int(ParamList[m++]);
	//
	pInfo->chIsThrow = Str2Int(ParamList[m++]);
	//
	pInfo->chIsDel = Str2Int(ParamList[m++]);
	//
	pInfo->nPileMax = Str2Int(ParamList[m++]);
	//
	pInfo->chInstance = Str2Int(ParamList[m++]);
	//
	pInfo->lPrice = Str2Int(ParamList[m++]);
	//
	memset(pInfo->chBody, cchItemRecordKeyValue, sizeof(pInfo->chBody));
	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ',');
	n = n > defITEM_BODY ? defITEM_BODY : n;
	for (int i = 0; i < n; i++) {
		pInfo->chBody[i] = Str2Int(strList[i]);
	}
	//
	pInfo->sNeedLv = Str2Int(ParamList[m++]);
	//
	memset(pInfo->szWork, cchItemRecordKeyValue, sizeof(char) * MAX_JOB_TYPE);
	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ',');
	n = n > MAX_JOB_TYPE ? MAX_JOB_TYPE : n;
	for (int i = 0; i < n; i++) {
		pInfo->szWork[i] = Str2Int(strList[i]);
	}
	//
	strLine = Str2Int(ParamList[m++]);
	//
	strLine = Str2Int(ParamList[m++]);
	//
	memset(pInfo->szAbleLink, cchItemRecordKeyValue, sizeof(char) * enumEQUIP_NUM);
	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ',');
	n = n > enumEQUIP_NUM ? enumEQUIP_NUM : n;
	for (int i = 0; i < n; i++) {
		pInfo->szAbleLink[i] = Str2Int(strList[i]);
	}
	//
	memset(pInfo->szNeedLink, cchItemRecordKeyValue, sizeof(char) * enumEQUIP_NUM);
	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ',');
	n = n > enumEQUIP_NUM ? enumEQUIP_NUM : n;
	for (int i = 0; i < n; i++) {
		pInfo->szNeedLink[i] = Str2Int(strList[i]);
	}

	//
	pInfo->chPickTo = Str2Int(ParamList[m++]);

	//
	pInfo->sStrCoef = Str2Int(ParamList[m++]);
	//
	pInfo->sAgiCoef = Str2Int(ParamList[m++]);
	//
	pInfo->sDexCoef = Str2Int(ParamList[m++]);
	//
	pInfo->sConCoef = Str2Int(ParamList[m++]);
	//
	pInfo->sStaCoef = Str2Int(ParamList[m++]);
	//
	pInfo->sLukCoef = Str2Int(ParamList[m++]);
	//
	pInfo->sASpdCoef = Str2Int(ParamList[m++]);
	//
	pInfo->sADisCoef = Str2Int(ParamList[m++]);
	//
	pInfo->sMnAtkCoef = Str2Int(ParamList[m++]);
	//
	pInfo->sMxAtkCoef = Str2Int(ParamList[m++]);
	//
	pInfo->sDefCoef = Str2Int(ParamList[m++]);
	// Hp
	pInfo->sMxHpCoef = Str2Int(ParamList[m++]);
	// Sp
	pInfo->sMxSpCoef = Str2Int(ParamList[m++]);
	//
	pInfo->sFleeCoef = Str2Int(ParamList[m++]);
	//
	pInfo->sHitCoef = Str2Int(ParamList[m++]);
	//
	pInfo->sCrtCoef = Str2Int(ParamList[m++]);
	//
	pInfo->sMfCoef = Str2Int(ParamList[m++]);
	// hp
	pInfo->sHRecCoef = Str2Int(ParamList[m++]);
	// sp
	pInfo->sSRecCoef = Str2Int(ParamList[m++]);
	//
	pInfo->sMSpdCoef = Str2Int(ParamList[m++]);
	//
	pInfo->sColCoef = Str2Int(ParamList[m++]);

	//
	memset(pInfo->sStrValu, 0, sizeof(pInfo->sStrValu));
	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ',');
	n = n > 2 ? 2 : n;
	for (int i = 0; i < n; i++) {
		pInfo->sStrValu[i] = Str2Int(strList[i]);
	}
	//
	memset(pInfo->sAgiValu, 0, sizeof(pInfo->sAgiValu));
	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ',');
	n = n > 2 ? 2 : n;
	for (int i = 0; i < n; i++) {
		pInfo->sAgiValu[i] = Str2Int(strList[i]);
	}
	//
	memset(pInfo->sDexValu, 0, sizeof(pInfo->sDexValu));
	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ',');
	n = n > 2 ? 2 : n;
	for (int i = 0; i < n; i++) {
		pInfo->sDexValu[i] = Str2Int(strList[i]);
	}
	//
	memset(pInfo->sConValu, 0, sizeof(pInfo->sConValu));
	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ',');
	n = n > 2 ? 2 : n;
	for (int i = 0; i < n; i++) {
		pInfo->sConValu[i] = Str2Int(strList[i]);
	}
	//
	memset(pInfo->sStaValu, 0, sizeof(pInfo->sStaValu));
	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ',');
	n = n > 2 ? 2 : n;
	for (int i = 0; i < n; i++) {
		pInfo->sStaValu[i] = Str2Int(strList[i]);
	}
	//
	memset(pInfo->sLukValu, 0, sizeof(pInfo->sLukValu));
	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ',');
	n = n > 2 ? 2 : n;
	for (int i = 0; i < n; i++) {
		pInfo->sLukValu[i] = Str2Int(strList[i]);
	}
	//
	memset(pInfo->sASpdValu, 0, sizeof(pInfo->sASpdValu));
	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ',');
	n = n > 2 ? 2 : n;
	for (int i = 0; i < n; i++) {
		pInfo->sASpdValu[i] = Str2Int(strList[i]);
	}
	//
	memset(pInfo->sADisValu, 0, sizeof(pInfo->sADisValu));
	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ',');
	n = n > 2 ? 2 : n;
	for (int i = 0; i < n; i++) {
		pInfo->sADisValu[i] = Str2Int(strList[i]);
	}
	//
	memset(pInfo->sMnAtkValu, 0, sizeof(pInfo->sMnAtkValu));
	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ',');
	n = n > 2 ? 2 : n;
	for (int i = 0; i < n; i++) {
		pInfo->sMnAtkValu[i] = Str2Int(strList[i]);
	}
	//
	memset(pInfo->sMxAtkValu, 0, sizeof(pInfo->sMxAtkValu));
	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ',');
	n = n > 2 ? 2 : n;
	for (int i = 0; i < n; i++) {
		pInfo->sMxAtkValu[i] = Str2Int(strList[i]);
	}
	//
	memset(pInfo->sDefValu, 0, sizeof(pInfo->sDefValu));
	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ',');
	n = n > 2 ? 2 : n;
	for (int i = 0; i < n; i++) {
		pInfo->sDefValu[i] = Str2Int(strList[i]);
	}
	// Hp
	memset(pInfo->sMxHpValu, 0, sizeof(pInfo->sMxHpValu));
	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ',');
	n = n > 2 ? 2 : n;
	for (int i = 0; i < n; i++) {
		pInfo->sMxHpValu[i] = Str2Int(strList[i]);
	}
	// Sp
	memset(pInfo->sMxSpValu, 0, sizeof(pInfo->sMxSpValu));
	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ',');
	n = n > 2 ? 2 : n;
	for (int i = 0; i < n; i++) {
		pInfo->sMxSpValu[i] = Str2Int(strList[i]);
	}
	//
	memset(pInfo->sFleeValu, 0, sizeof(pInfo->sFleeValu));
	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ',');
	n = n > 2 ? 2 : n;
	for (int i = 0; i < n; i++) {
		pInfo->sFleeValu[i] = Str2Int(strList[i]);
	}
	//
	memset(pInfo->sHitValu, 0, sizeof(pInfo->sHitValu));
	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ',');
	n = n > 2 ? 2 : n;
	for (int i = 0; i < n; i++) {
		pInfo->sHitValu[i] = Str2Int(strList[i]);
	}
	//
	memset(pInfo->sCrtValu, 0, sizeof(pInfo->sCrtValu));
	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ',');
	n = n > 2 ? 2 : n;
	for (int i = 0; i < n; i++) {
		pInfo->sCrtValu[i] = Str2Int(strList[i]);
	}
	//
	memset(pInfo->sMfValu, 0, sizeof(pInfo->sMfValu));
	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ',');
	n = n > 2 ? 2 : n;
	for (int i = 0; i < n; i++) {
		pInfo->sMfValu[i] = Str2Int(strList[i]);
	}
	// hp
	memset(pInfo->sHRecValu, 0, sizeof(pInfo->sHRecValu));
	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ',');
	n = n > 2 ? 2 : n;
	for (int i = 0; i < n; i++) {
		pInfo->sHRecValu[i] = Str2Int(strList[i]);
	}
	// sp
	memset(pInfo->sSRecValu, 0, sizeof(pInfo->sSRecValu));
	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ',');
	n = n > 2 ? 2 : n;
	for (int i = 0; i < n; i++) {
		pInfo->sSRecValu[i] = Str2Int(strList[i]);
	}
	//
	memset(pInfo->sMSpdValu, 0, sizeof(pInfo->sMSpdValu));
	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ',');
	n = n > 2 ? 2 : n;
	for (int i = 0; i < n; i++) {
		pInfo->sMSpdValu[i] = Str2Int(strList[i]);
	}
	//
	memset(pInfo->sColValu, 0, sizeof(pInfo->sColValu));
	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ',');
	n = n > 2 ? 2 : n;
	for (int i = 0; i < n; i++) {
		pInfo->sColValu[i] = Str2Int(strList[i]);
	}

	//
	memset(pInfo->sPDef, 0, sizeof(pInfo->sPDef));
	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ',');
	n = n > 2 ? 2 : n;
	for (int i = 0; i < n; i++) {
		pInfo->sPDef[i] = Str2Int(strList[i]);
	}
	//
	pInfo->sLHandValu = Str2Int(ParamList[m++]);
	//
	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ',');
	for (int i = 0; i < 2; i++) {
		if (i < n)
			pInfo->sEnergy[i] = Str2Int(strList[i]);
		else
			pInfo->sEnergy[i] = 0;
	}
	//
	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ',');
	for (int i = 0; i < 2; i++) {
		if (i < n)
			pInfo->sEndure[i] = Str2Int(strList[i]);
		else
			pInfo->sEndure[i] = 0;
	}
	//
	pInfo->sHole = Str2Int(ParamList[m++]);
	//
	strLine = Str2Int(ParamList[m++]);
	//
	strLine = Str2Int(ParamList[m++]);
	//
	strLine = Str2Int(ParamList[m++]);
	//
	strLine = Str2Int(ParamList[m++]);
	//
	strLine = Str2Int(ParamList[m++]);
	//
	strLine = Str2Int(ParamList[m++]);
	//
	strLine = Str2Int(ParamList[m++]);
	//
	strLine = Str2Int(ParamList[m++]);
	//
	strncpy(pInfo->szAttrEffect, ParamList[m++].c_str(), defITEM_ATTREFFECT_NAME_LEN);
	pInfo->szAttrEffect[defITEM_ATTREFFECT_NAME_LEN - 1] = _TEXT('\0');

	//drap
	pInfo->sDrap = Str2Int(ParamList[m++]);
	//
	memset(pInfo->sEffect, 0, sizeof(pInfo->sEffect));
	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ',');
	if (n > defITEM_BIND_EFFECT_NUM) n = defITEM_BIND_EFFECT_NUM;
	for (int e = 0; e < n; e++) {
		pInfo->sEffect[e][0] = Str2Int(strList[e]);
	}
	if (n == 1 && pInfo->sEffect[0] == 0) {
		pInfo->sEffNum = 0;
	}
	else {
		pInfo->sEffNum = n;
	}

	// dummy
	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ',');
	if (n > defITEM_BIND_EFFECT_NUM) n = defITEM_BIND_EFFECT_NUM;
	for (int e = 0; e < n; e++) {
		pInfo->sEffect[e][1] = Str2Int(strList[e]);
	}

	//
	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ',');
	int ItemNum = sizeof(pInfo->sItemEffect) / sizeof(pInfo->sItemEffect[0]);
	if (n > ItemNum) n = ItemNum;
	memset(pInfo->sItemEffect, 0, sizeof(pInfo->sItemEffect));
	for (int e = 0; e < n; e++) {
		pInfo->sItemEffect[e] = Str2Int(strList[e]);
	}

	//
	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ',');
	int AreaNum = sizeof(pInfo->sAreaEffect) / sizeof(pInfo->sAreaEffect[0]);
	if (n > AreaNum) n = AreaNum;
	memset(pInfo->sAreaEffect, 0, sizeof(pInfo->sAreaEffect));
	for (int e = 0; e < n; e++) {
		pInfo->sAreaEffect[e] = Str2Int(strList[e]);
	}

	//
	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ',');
	int nNum = sizeof(pInfo->sUseItemEffect) / sizeof(pInfo->sUseItemEffect[0]);
	if (n > nNum) n = nNum;
	memset(pInfo->sUseItemEffect, 0, sizeof(pInfo->sUseItemEffect));
	for (int e = 0; e < n; e++) {
		pInfo->sUseItemEffect[e] = Str2Int(strList[e]);
	}

	//
	strLine = ParamList[m++];

	if (strLine == "0") {
		pInfo->szDescriptor[0] = '\0';
	}
	else {
		strncpy(pInfo->szDescriptor, strLine.c_str(), defITEM_DESCRIPTOR_NAME_LEN);
		pInfo->szDescriptor[defITEM_DESCRIPTOR_NAME_LEN - 1] = _TEXT('\0');
	}

	pInfo->nCooldown = Str2Float(ParamList[m++]);

	if (pInfo->szAttrEffect[0] != '\0' && pInfo->nCooldown == 0) {
		pInfo->nCooldown = 1.0f;
	}

	/*memset(pInfo->sSeteffect, cchItemRecordKeyValue, sizeof(pInfo->sSeteffect));
	string strSub[10];
	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ';');
	n = n > enumEQUIP_NUM ? enumEQUIP_NUM : n;
	for (int i = 0; i < n; i++)
	{
		Util_ResolveTextLine(strList[i].c_str(), strSub, 2, ',');
		pInfo->sSeteffect[i][0] = Str2Int(strSub[0]);
		pInfo->sSeteffect[i][1] = Str2Int(strSub[1]);
	}*/

	return TRUE;
}

void CItemRecordSet::_ProcessRawDataInfo(CRawDataInfo* pInfo) {
	CItemRecord* pItem = (CItemRecord*)pInfo;

	pItem->RefreshData();
}
